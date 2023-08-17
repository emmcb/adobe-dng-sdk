#ifndef __Reconcile_Impl_hpp__
#define __Reconcile_Impl_hpp__ 1

// =================================================================================================
// Copyright Adobe
// Copyright 2006 Adobe
// All Rights Reserved
//
// NOTICE: Adobe permits you to use, modify, and distribute this file in accordance with the terms
// of the Adobe license agreement accompanying it. If you have received this file from a source other 
// than Adobe, then your use, modification, or distribution of it requires the prior written permission
// of Adobe.
// =================================================================================================

#if AdobePrivate
// =================================================================================================
// Change history
// ==============
//
// Writers:
//	AWL Alan Lillich
//
// mm-dd-yy who Description of changes, most recent on top
//
// 12-04-13 HK  5.6-f085 [3677602] Fixing Unicode conversion issues on IOS.
// 12-03-09 AWL 5.0-f120 [2499192] Add special case support for exif:ISOSpeedRatings over 65535.
// 10-08-09 AWL 5.0-f086 [2410350] Tweak MWG policy for better backwards compatibility.
// 04-20-09 AWL 5.0-f033 [2307195] Add language parameter for Mac text conversions.
// 03-10-09 AWL 5.0-f027 Initial integration of QuickTime support into MPEG-4 handler.
// 12-18-08 AWL 5.0-f010 [1932925] Fix TIFF handler to ignore trailing zero bytes for LONG tag 33723.
//				Don't parse read-only IPTC if the digest matches, always parse for update.
// 11-18-08 AWL 5.0-f008 Fix server mode to not delete XMP for non-ASCII input that is ignored.
// 11-13-08 AWL 5.0-f004 Add server mode support that ignores local text. Enable all handlers except
//				MOV for generic UNIX - that will be handled as part of the rewrite.
// 10-31-08 AWL 5.0-f002 MWG compliance changes: Fix Exif Artist and date/time mapping bugs.
//
// 10-23-08 AWL 4.4-f015 MWG compliance changes: Don't keep device properties in the file's XMP;
//				mapping changes for 3-way properties, especially description and date/time.
// 10-14-08 AWL 4.4-f014 MWG compliance changes: simplified block selection.
//
// 02-18-08 AWL 4.2-f077 More changes to generic UNIX builds for XMPFiles.
//
// 11-10-06 AWL 4.1-f066 [1417221] Tweak the "legacy digest missing" logic to import legacy values
//				that do not have corresponding XMP.
// 11-01-06 AWL 4.1-f059 [1409577] Fix JPEG and PSD legacy import to better match Photoshop. There
//				are different JPEG/PSD/TIFF file policies for TIFF tags 270 (dc:description), 315
//				(dc:creator), and 33432 (dc:rights).
// 10-18-06 AWL 4.1-f046 [1395599] Add ISO Latin-1 conversions, change ID3 support to use it.
// 09-21-06 AWL 4.1-f037 [1378220,1367149] Fix the legacy metadata output to allow proper detection
//				of XMP-only changes and thus take advantage of an in-place XMP update for unsafe
//				saves. Fix the PSIR 1034 copyright flag handling to match Photoshop. Add CR<->LF
//				normalization hackery to match Photoshop.
//
// 07-10-06 AWL 4.0-f014 Initial version of new read-write JPEG handler and underpinnings. Reasonably
//				but not thoroughly tested, still within NewHandlers conditional.
// 04-21-06 AWL First draft.
//
// =================================================================================================
#endif // AdobePrivate

#include "public/include/XMP_Environment.h"	// ! This must be the first include.

#include "XMPFiles/source/FormatSupport/ReconcileLegacy.hpp"
#include "third-party/zuid/interfaces/MD5.h"

// =================================================================================================
/// \file Reconcile_Impl.hpp
/// \brief Implementation utilities for the legacy metadata reconciliation support.
///
// =================================================================================================

typedef XMP_Uns8 MD5_Digest[16];	// ! Should be in MD5.h.

enum {
	kDigestMissing = -1,	// A partial import is done, existing XMP is left alone.
	kDigestDiffers =  0,	// A full import is done, existing XMP is deleted or replaced.
	kDigestMatches = +1		// No importing is done.
};

namespace ReconcileUtils {

	// *** These ought to be with the Unicode conversions.

	static const char * kHexDigits = "0123456789ABCDEF";

	bool IsASCII      ( const void * _textPtr, size_t textLen );
	bool IsUTF8       ( const void * _textPtr, size_t textLen );

	void UTF8ToLocal  ( const void * _utf8Ptr, size_t utf8Len, std::string * local );
	void UTF8ToLatin1 ( const void * _utf8Ptr, size_t utf8Len, std::string * latin1 );
	void LocalToUTF8  ( const void * _localPtr, size_t localLen, std::string * utf8 );
	void Latin1ToUTF8 ( const void * _latin1Ptr, size_t latin1Len, std::string * utf8 );
	
	// 
	// Checks if the input string is UTF-8 encoded. If not, it tries to convert it to UTF-8
	// This is only done, if Server Mode is not active!
	// @param input the native input string
	// @return The input if it is already UTF-8, the converted input 
	//			or an empty string if no conversion is possible because of ServerMode
	//
	void NativeToUTF8 ( const std::string & input, std::string & output );

	#if XMP_WinBuild
		void UTF8ToWinEncoding ( UINT codePage, const XMP_Uns8 * utf8Ptr, size_t utf8Len, std::string * host );
		void WinEncodingToUTF8 ( UINT codePage, const XMP_Uns8 * hostPtr, size_t hostLen, std::string * utf8 );
	#elif XMP_MacBuild
		void UTF8ToMacEncoding ( XMP_Uns16 macScript, XMP_Uns16 macLang, const XMP_Uns8 * utf8Ptr, size_t utf8Len, std::string * host );
		void MacEncodingToUTF8 ( XMP_Uns16 macScript, XMP_Uns16 macLang, const XMP_Uns8 * hostPtr, size_t hostLen, std::string * utf8 );
    #elif XMP_iOSBuild
        void IOSConvertEncoding(XMP_Uns32 srcEncoding, XMP_Uns32 destEncoding, const XMP_Uns8 * inputPtr, size_t inputLen, std::string * output);
	#endif

};	// ReconcileUtils

namespace PhotoDataUtils {

	int CheckIPTCDigest ( const void * newPtr, const XMP_Uns32 newLen, const void * oldDigest );
	void SetIPTCDigest  ( void * iptcPtr, XMP_Uns32 iptcLen, PSIR_Manager * psir );

	bool GetNativeInfo ( const TIFF_Manager & exif, XMP_Uns8 ifd, XMP_Uns16 id, TIFF_Manager::TagInfo * info );
	size_t GetNativeInfo ( const IPTC_Manager & iptc, XMP_Uns8 id, int digestState,
						   bool haveXMP, IPTC_Manager::DataSetInfo * info );
	
	bool IsValueDifferent ( const TIFF_Manager::TagInfo & exifInfo,
							const std::string & xmpValue, std::string * exifValue );
	bool IsValueDifferent ( const IPTC_Manager & newIPTC, const IPTC_Manager & oldIPTC, XMP_Uns8 id );

	void ImportPSIR ( const PSIR_Manager & psir, SXMPMeta * xmp, int iptcDigestState );

	void Import2WayIPTC ( const IPTC_Manager & iptc, SXMPMeta * xmp, int iptcDigestState );
	void Import2WayExif ( const TIFF_Manager & exif, SXMPMeta * xmp, int iptcDigestState );

	void Import3WayItems ( const TIFF_Manager & exif, const IPTC_Manager & iptc, SXMPMeta * xmp, int iptcDigestState );

	void ExportPSIR ( const SXMPMeta & xmp, PSIR_Manager * psir );
	void ExportIPTC ( const SXMPMeta & xmp, IPTC_Manager * iptc );
	void ExportExif ( SXMPMeta * xmp, TIFF_Manager * exif );
	
	// These need to be exposed for use in Import3WayItem:

	void ImportIPTC_Simple ( const IPTC_Manager & iptc, SXMPMeta * xmp,
							 XMP_Uns8 id, const char * xmpNS, const char * xmpProp );
	
	void ImportIPTC_LangAlt ( const IPTC_Manager & iptc, SXMPMeta * xmp,
							  XMP_Uns8 id, const char * xmpNS, const char * xmpProp );
	
	void ImportIPTC_Array ( const IPTC_Manager & iptc, SXMPMeta * xmp,
							XMP_Uns8 id, const char * xmpNS, const char * xmpProp );
	
	void ImportIPTC_Date ( XMP_Uns8 dateID, const IPTC_Manager & iptc, SXMPMeta * xmp );

};	// PhotoDataUtils

#endif	// __Reconcile_Impl_hpp__
