#ifndef __JPEG_Handler_hpp__
#define __JPEG_Handler_hpp__	1

// =================================================================================================
// Copyright Adobe
// Copyright 2004 Adobe
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
// 09-14-11	AWL	5.5-f030 Remove IOBuffer from the JPEG, TIFF, and PSD handlers.
//
// 08-19-10 AWL 5.3-f003 Remove all use of the LFA_* names.
// 08-17-10 AWL 5.3-f001 Integrate I/O revamp to main.
//
// 10-13-08 AWL 4.4-f013 Remove internals of GetThumbnail.
//
// 09-14-06 AWL 4.1-f034 Finish the support for XMP-only in-place updates for JPEG/TIFF/PSD.
//
// 05-18-06 AWL 4.0-f005 Add support for JPEG/Exif thumbnails.
// 05-15-06 AWL 4.0-f004 Working, but minimally tested read-only smart JPEG handler under NewHandlers.
// 04-07-06 AWL 4.0-f002 Add XMPFiles::GetThumbnail. Change XMPFiles::OpenFile to close the disk file
//				early for read-only access. Change XMPFileHandler::ExtractXMP to CacheFileData.
// 03-24-06 AWL 4.0-f001 Adapt for move to ham-perforce, integrate XMPFiles, bump version to 4.
//
// 09-01-05 AWL 1.3-003 [1225858] Don't implicitly set the SafeUpdate flag for non-owning handlers.
//
// 04-13-05 AWL 1.1-052 Add tweaks to the CheckProc needed by the MDKit-based JPEG/TIFF/PSD handler.
//
// 01-06-05 AWL 1.0-027 [1014854] Cleanup handling for UTF-16 and UTF-32. This involves some general
//				changes to the separation of work between the common code and handlers, making the
//				storage of the SXMPMeta object and packet string more visible.
//
// 08-13-04 AWL Checkpoint, almost full implementation, compiles.
// 08-12-04 AWL First draft.
//
// =================================================================================================
#endif // AdobePrivate

#include "public/include/XMP_Environment.h"	// ! Must be the first #include!

#include "public/include/XMP_Const.h"
#include "public/include/XMP_IO.hpp"

#include "XMPFiles/source/FormatSupport/TIFF_Support.hpp"
#include "XMPFiles/source/FormatSupport/PSIR_Support.hpp"
#include "XMPFiles/source/FormatSupport/IPTC_Support.hpp"

// =================================================================================================
/// \file JPEG_Handler.hpp
/// \brief File format handler for JPEG.
///
/// This header ...
///
// =================================================================================================

// *** Could derive from Basic_Handler - buffer file tail in a temp file.

extern XMPFileHandler * JPEG_MetaHandlerCTor ( XMPFiles * parent );

extern bool JPEG_CheckFormat ( XMP_FileFormat format,
							   XMP_StringPtr  filePath,
                               XMP_IO *       fileRef,
                               XMPFiles *     parent );

static const XMP_OptionBits kJPEG_HandlerFlags = (kXMPFiles_CanInjectXMP |
                                                  kXMPFiles_CanExpand |
                                                  kXMPFiles_CanRewrite |
                                                  kXMPFiles_PrefersInPlace |
                                                  kXMPFiles_CanReconcile |
                                                  kXMPFiles_AllowsOnlyXMP |
                                                  kXMPFiles_ReturnsRawPacket |
                                                  kXMPFiles_AllowsSafeUpdate);

class JPEG_MetaHandler : public XMPFileHandler
{
public:

	void CacheFileData();
	void ProcessXMP();

	void UpdateFile    ( bool doSafeUpdate );
    void WriteTempFile ( XMP_IO* tempRef );

	struct GUID_32 {	// A hack to get an assignment operator for an array.
		char data [32];
		void operator= ( const GUID_32 & in )
		{
			memcpy ( this->data, in.data, sizeof(this->data) );	// AUDIT: Use of sizeof(this->data) is safe.
		};
		bool operator< ( const GUID_32 & right ) const
		{
			return (memcmp ( this->data, right.data, sizeof(this->data) ) < 0);
		};
		bool operator== ( const GUID_32 & right ) const
		{
			return (memcmp ( this->data, right.data, sizeof(this->data) ) == 0);
		};
	};

	JPEG_MetaHandler ( XMPFiles * parent );
	virtual ~JPEG_MetaHandler();

private:

	JPEG_MetaHandler() : exifMgr(0), psirMgr(0), iptcMgr(0), skipReconcile(false) {};	// Hidden on purpose.

	std::string exifContents;
	std::string psirContents;

	TIFF_Manager * exifMgr;	// The Exif manager will be created by ProcessTNail or ProcessXMP.
	PSIR_Manager * psirMgr;	// Need to use pointers so we can properly select between read-only and
	IPTC_Manager * iptcMgr;	//	read-write modes of usage.

	bool skipReconcile;	// ! Used between UpdateFile and WriteFile.

	typedef std::map < GUID_32, std::string > ExtendedXMPMap;

	ExtendedXMPMap extendedXMP;	// ! Only contains those with complete data.
//	void CacheExtendedXMP ( ExtendedXMPInfo * extXMP, XMP_Uns8 * buffer, size_t bufferLen );

};	// JPEG_MetaHandler

// =================================================================================================

#endif /* __JPEG_Handler_hpp__ */
