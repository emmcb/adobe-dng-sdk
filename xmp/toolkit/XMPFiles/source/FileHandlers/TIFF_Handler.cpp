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
//  IJS Inder Jeet Singh
//  ADC	Amandeep Chawla
//	SKP Sunil Kishor Pathak
//
// mm-dd-yy who Description of changes, most recent on top
//
// 11-05-14 SKP 5.6-f121 Implemented MWG spec to read IPTC irrespective of IIM digest matching or not.
// 03-06-14 ADC 5.6-f095 Reverting changes done via CL # 155650 (MWG specification related).
// 09-14-11	AWL	5.5-f030 Remove IOBuffer from the JPEG, TIFF, and PSD handlers.
// 09-04-12 IJS 5.5-f028 Add file update progress tracking to the TIFF handler.
// 05-09-12	AWL	5.5-f013 Make the JPEG, TIFF, and PSD handlers ignore XMP parsing errors.
//
// 08-19-10 AWL 5.3-f004 Move the seek mode constants to XMP_Const.
// 08-19-10 AWL 5.3-f003 Remove all use of the LFA_* names.
// 08-17-10 AWL 5.3-f001 Integrate I/O revamp to main.
//
// 10-06-09 AWL 5.0-f084 [2426060,2434421,2424514] Fix TrimTrailingSpaces in ReconcileTIFF.cpp to handle all
//				blank input. Fix Mac script conversions to use the right script. Fix MPEG-4 imports
//				to not let one failure abort all. Fix TIFF handler to keep the XMP the same size if possible.
// 12-18-08 AWL 5.0-f010 [1932925] Fix TIFF handler to ignore trailing zero bytes for LONG tag 33723.
//				Don't parse read-only IPTC if the digest matches, always parse for update.
// 11-13-08 AWL 5.0-f004 Add server mode support that ignores local text. Enable all handlers except
//				MOV for generic UNIX - that will be handled as part of the rewrite.
//
// 10-23-08 AWL 4.4-f015 MWG compliance changes: Don't keep device properties in the file's XMP;
//				mapping changes for 3-way properties, especially description and date/time.
// 10-14-08 AWL 4.4-f014 MWG compliance changes: simplified block selection.
// 10-13-08 AWL 4.4-f013 Remove internals of GetThumbnail.
//
// 02-27-08 AWL 4.2-f085 [1700696] Fix in-place update logic for JPEG, PSD, and TIFF handlers.
// 02-18-08 AWL 4.2-f077 More changes to generic UNIX builds for XMPFiles.
// 01-30-08 AWL 4.2-f065 [1643402] Always strip IIM and PSIR from DNG. Have TIFF handler reject DNG
//				if DNGBackwardVersion has a high order byte that is not 1 (>= 2.0.0.0).
// 10-10-07 AWL 4.2-f027 Add handler method GetSerializeOptions. Introduce FillPacketInfo to replace
//				GetPacketCharForm, GetPacketRWMode, and GetPacketPadSize. And tolerate no wrapper.
// 01-30-07 AWL 4.2-f014 [1471832] Change the JPEG/PSD/TIFF handlers to do a forced legacy import if
//				the XMP portion won't parse.
// 11-01-06 AWL 4.1-f059 [1409577] Fix JPEG and PSD legacy import to better match Photoshop. There
//				are different JPEG/PSD/TIFF file policies for TIFF tags 270 (dc:description), 315
//				(dc:creator), and 33432 (dc:rights).
// 10-16-06 AWL 4.1-f044 [1388170] Fix a major regression in the previous f042 changes. That caused
//				TIFF and Exif imports to stop for plain camera files - ones with TIFF and Exif but
//				no PSIR or XMP.
// 09-21-06 AWL 4.1-f037 [1378220,1367149] Fix the legacy metadata output to allow proper detection
//				of XMP-only changes and thus take advantage of an in-place XMP update for unsafe
//				saves. Fix the PSIR 1034 copyright flag handling to match Photoshop. Add CR<->LF
//				normalization hackery to match Photoshop.
// 09-14-06 AWL 4.1-f034 Finish the support for XMP-only in-place updates for JPEG/TIFF/PSD.
// 08-31-06 AWL 4.1-f032 More work on in-place updates. Have ReconcileXMPtoJTP only update the IPTC
//				and PSIR blocks if they have changed. Move the XMP updating out to the file handler.
//
// 07-10-06 AWL 4.0-f014 Initial version of new read-write JPEG handler and underpinnings. Reasonably
//				but not thoroughly tested, still within NewHandlers conditional.
// 06-09-06 AWL 4.0-f011 Add hackery to support TIFF files written by Photoshop 6.
// 05-26-06 AWL Initial creation.
//
// =================================================================================================
#endif // AdobePrivate

#include "public/include/XMP_Environment.h"	// ! This must be the first include.
#include "public/include/XMP_Const.h"
#include "public/include/XMP_IO.hpp"

#include "XMPFiles/source/XMPFiles_Impl.hpp"
#include "source/XIO.hpp"

#include "XMPFiles/source/FileHandlers/TIFF_Handler.hpp"

#include "XMPFiles/source/FormatSupport/TIFF_Support.hpp"
#include "XMPFiles/source/FormatSupport/PSIR_Support.hpp"
#include "XMPFiles/source/FormatSupport/IPTC_Support.hpp"
#include "XMPFiles/source/FormatSupport/ReconcileLegacy.hpp"
#include "XMPFiles/source/FormatSupport/Reconcile_Impl.hpp"

#include "third-party/zuid/interfaces/MD5.h"

using namespace std;

// =================================================================================================
/// \file TIFF_Handler.cpp
/// \brief File format handler for TIFF.
///
/// This handler ...
///
// =================================================================================================

// =================================================================================================
// TIFF_CheckFormat
// ================

// For TIFF we just check for the II/42 or MM/42 in the first 4 bytes and that there are at least
// 26 bytes of data (4+4+2+12+4).
//
// ! The CheckXyzFormat routines don't track the filePos, that is left to ScanXyzFile.

bool TIFF_CheckFormat ( XMP_FileFormat format,
	                    XMP_StringPtr  filePath,
                        XMP_IO*    fileRef,
                        XMPFiles *     parent )
{
	IgnoreParam(format); IgnoreParam(filePath); IgnoreParam(parent);
	XMP_Assert ( format == kXMP_TIFFFile );

	enum { kMinimalTIFFSize = 4+4+2+12+4 };	// Header plus IFD with 1 entry.

	fileRef->Rewind ( );
	if ( ! XIO::CheckFileSpace ( fileRef, kMinimalTIFFSize ) ) return false;

	XMP_Uns8 buffer [4];
	fileRef->Read ( buffer, 4 );
	
	bool leTIFF = CheckBytes ( buffer, "\x49\x49\x2A\x00", 4 );
	bool beTIFF = CheckBytes ( buffer, "\x4D\x4D\x00\x2A", 4 );

	return (leTIFF | beTIFF);

}	// TIFF_CheckFormat

// =================================================================================================
// TIFF_MetaHandlerCTor
// ====================

XMPFileHandler * TIFF_MetaHandlerCTor ( XMPFiles * parent )
{
	return new TIFF_MetaHandler ( parent );

}	// TIFF_MetaHandlerCTor

// =================================================================================================
// TIFF_MetaHandler::TIFF_MetaHandler
// ==================================

TIFF_MetaHandler::TIFF_MetaHandler ( XMPFiles * _parent ) : psirMgr(0), iptcMgr(0)
{
	this->parent = _parent;
	this->handlerFlags = kTIFF_HandlerFlags;
	this->stdCharForm  = kXMP_Char8Bit;

}	// TIFF_MetaHandler::TIFF_MetaHandler

// =================================================================================================
// TIFF_MetaHandler::~TIFF_MetaHandler
// ===================================

TIFF_MetaHandler::~TIFF_MetaHandler()
{

	if ( this->psirMgr != 0 ) delete ( this->psirMgr );
	if ( this->iptcMgr != 0 ) delete ( this->iptcMgr );

}	// TIFF_MetaHandler::~TIFF_MetaHandler

// =================================================================================================
// TIFF_MetaHandler::CacheFileData
// ===============================
//
// The data caching for TIFF is easy to explain and implement, but does more processing than one
// might at first expect. This seems unavoidable given the need to close the disk file after calling
// CacheFileData. We parse the TIFF stream and cache the values for all tags of interest, and note
// whether XMP is present. We do not parse the XMP, Photoshop image resources, or IPTC datasets.

// *** This implementation simply returns when invalid TIFF is encountered. Should we throw instead?

void TIFF_MetaHandler::CacheFileData()
{
	XMP_IO*      fileRef    = this->parent->ioRef;
	XMP_PacketInfo & packetInfo = this->packetInfo;

	XMP_AbortProc abortProc  = this->parent->abortProc;
	void *        abortArg   = this->parent->abortArg;
	const bool    checkAbort = (abortProc != 0);

	XMP_Assert ( ! this->containsXMP );
	// Set containsXMP to true here only if the XMP tag is found.

	if ( checkAbort && abortProc(abortArg) ) {
		XMP_Throw ( "TIFF_MetaHandler::CacheFileData - User abort", kXMPErr_UserAbort );
	}

	this->tiffMgr.ParseFileStream ( fileRef );

	TIFF_Manager::TagInfo dngInfo;
	if ( this->tiffMgr.GetTag ( kTIFF_PrimaryIFD, kTIFF_DNGVersion, &dngInfo ) ) {

		// Reject DNG files that are version 2.0 or beyond, this is being written at the time of
		// DNG version 1.2. The DNG team says it is OK to use 2.0, not strictly 1.2. Use the
		// DNGBackwardVersion if it is present, else the DNGVersion. Note that the version value is
		// supposed to be type BYTE, so the file order is always essentially big endian.

		XMP_Uns8 majorVersion = *((XMP_Uns8*)dngInfo.dataPtr);	// Start with DNGVersion.
		if ( this->tiffMgr.GetTag ( kTIFF_PrimaryIFD, kTIFF_DNGBackwardVersion, &dngInfo ) ) {
			majorVersion = *((XMP_Uns8*)dngInfo.dataPtr);	// Use DNGBackwardVersion if possible.
		}
		if ( majorVersion > 1 ) XMP_Throw ( "DNG version beyond 1.x", kXMPErr_BadTIFF );

	}

	TIFF_Manager::TagInfo xmpInfo;
	bool found = this->tiffMgr.GetTag ( kTIFF_PrimaryIFD, kTIFF_XMP, &xmpInfo );

	if ( found ) {

		this->packetInfo.offset    = this->tiffMgr.GetValueOffset ( kTIFF_PrimaryIFD, kTIFF_XMP );
		this->packetInfo.length    = xmpInfo.dataLen;
		this->packetInfo.padSize   = 0;				// Assume for now, set these properly in ProcessXMP.
		this->packetInfo.charForm  = kXMP_CharUnknown;
		this->packetInfo.writeable = true;

		this->xmpPacket.assign ( (XMP_StringPtr)xmpInfo.dataPtr, xmpInfo.dataLen );

		this->containsXMP = true;

	}

}	// TIFF_MetaHandler::CacheFileData

// =================================================================================================
// TIFF_MetaHandler::ProcessXMP
// ============================
//
// Process the raw XMP and legacy metadata that was previously cached. The legacy metadata in TIFF
// is messy because there are 2 copies of the IPTC and because of a Photoshop 6 bug/quirk in the way
// Exif metadata is saved.

void TIFF_MetaHandler::ProcessXMP()
{

	this->processedXMP = true;	// Make sure we only come through here once.

	// Set up everything for the legacy import, but don't do it yet. This lets us do a forced legacy
	// import if the XMP packet gets parsing errors.

	// ! Photoshop 6 wrote annoyingly wacky TIFF files. It buried a lot of the Exif metadata inside
	// ! image resource 1058, itself inside of tag 34377 in the 0th IFD. Take care of this before
	// ! doing any of the legacy metadata presence or priority analysis. Delete image resource 1058
	// ! to get rid of the buried Exif, but don't mark the XMPFiles object as changed. This change
	// ! should not trigger an update, but should be included as part of a normal update.

	bool found;
	bool readOnly = ((this->parent->openFlags & kXMPFiles_OpenForUpdate) == 0);

	if ( readOnly ) {
		this->psirMgr = new PSIR_MemoryReader();
		this->iptcMgr = new IPTC_Reader();
	} else {
		this->psirMgr = new PSIR_FileWriter();
		this->iptcMgr = new IPTC_Writer();	// ! Parse it later.
	}

	TIFF_Manager & tiff = this->tiffMgr;	// Give the compiler help in recognizing non-aliases.
	PSIR_Manager & psir = *this->psirMgr;
	IPTC_Manager & iptc = *this->iptcMgr;

	TIFF_Manager::TagInfo psirInfo;
	bool havePSIR = tiff.GetTag ( kTIFF_PrimaryIFD, kTIFF_PSIR, &psirInfo );

	if ( havePSIR ) {	// ! Do the Photoshop 6 integration before other legacy analysis.
		psir.ParseMemoryResources ( psirInfo.dataPtr, psirInfo.dataLen );
		PSIR_Manager::ImgRsrcInfo buriedExif;
		found = psir.GetImgRsrc ( kPSIR_Exif, &buriedExif );
		if ( found ) {
			tiff.IntegrateFromPShop6 ( buriedExif.dataPtr, buriedExif.dataLen );
			if ( ! readOnly ) psir.DeleteImgRsrc ( kPSIR_Exif );
		}
	}

	TIFF_Manager::TagInfo iptcInfo;
	bool haveIPTC = tiff.GetTag ( kTIFF_PrimaryIFD, kTIFF_IPTC, &iptcInfo );	// The TIFF IPTC tag.
	int iptcDigestState = kDigestMatches;

	if ( haveIPTC ) {

		bool haveDigest = false;
		PSIR_Manager::ImgRsrcInfo digestInfo;
		if ( havePSIR ) haveDigest = psir.GetImgRsrc ( kPSIR_IPTCDigest, &digestInfo );
		if ( digestInfo.dataLen != 16 ) haveDigest = false;

		if ( ! haveDigest ) {

			iptcDigestState = kDigestMissing;

		} else {

			// Older versions of Photoshop wrote tag 33723 with type LONG, but ignored the trailing
			// zero padding for the IPTC digest. If the full digest differs, recheck without the padding.

			iptcDigestState = PhotoDataUtils::CheckIPTCDigest ( iptcInfo.dataPtr, iptcInfo.dataLen, digestInfo.dataPtr );

			if ( (iptcDigestState == kDigestDiffers) && (kTIFF_TypeSizes[iptcInfo.type] > 1) && iptcInfo.dataLen > 0) {
				XMP_Uns8 * endPtr = (XMP_Uns8*)iptcInfo.dataPtr + iptcInfo.dataLen - 1;
				XMP_Uns8 * minPtr = endPtr - kTIFF_TypeSizes[iptcInfo.type] + 1;
				while ( (endPtr >= minPtr) && (*endPtr == 0) ) --endPtr;
				XMP_Uns32 unpaddedLen = (XMP_Uns32) (endPtr - (XMP_Uns8*)iptcInfo.dataPtr + 1);
				iptcDigestState = PhotoDataUtils::CheckIPTCDigest ( iptcInfo.dataPtr, unpaddedLen, digestInfo.dataPtr );
			}

		}

	}

	XMP_OptionBits options = k2XMP_FileHadExif;	// TIFF files are presumed to have Exif legacy.
	if ( haveIPTC ) options |= k2XMP_FileHadIPTC;
	if ( this->containsXMP ) options |= k2XMP_FileHadXMP;

	// Process the XMP packet. If it fails to parse, do a forced legacy import but still throw an
	// exception. This tells the caller that an error happened, but gives them recovered legacy
	// should they want to proceed with that.

	bool haveXMP = false;

	if ( ! this->xmpPacket.empty() ) {
		XMP_Assert ( this->containsXMP );
		// Common code takes care of packetInfo.charForm, .padSize, and .writeable.
		XMP_StringPtr packetStr = this->xmpPacket.c_str();
		XMP_StringLen packetLen = (XMP_StringLen)this->xmpPacket.size();
		try {
			this->xmpObj.ParseFromBuffer ( packetStr, packetLen );
		} catch ( ... ) { /* Ignore parsing failures, someday we hope to get partial XMP back. */ }
		haveXMP = true;
	}

	// Process the legacy metadata.

	if ( haveIPTC && (! haveXMP) && (iptcDigestState == kDigestMatches) ) iptcDigestState = kDigestMissing;
	if (iptcInfo.dataLen) iptc.ParseMemoryDataSets ( iptcInfo.dataPtr, iptcInfo.dataLen );
	ImportPhotoData ( tiff, iptc, psir, iptcDigestState, &this->xmpObj, options );

	this->containsXMP = true;	// Assume we now have something in the XMP.

}	// TIFF_MetaHandler::ProcessXMP

// =================================================================================================
// TIFF_MetaHandler::UpdateFile
// ============================
//
// There is very little to do directly in UpdateFile. ExportXMPtoJTP takes care of setting all of
// the necessary TIFF tags, including things like the 2nd copy of the IPTC in the Photoshop image
// resources in tag 34377. TIFF_FileWriter::UpdateFileStream does all of the update-by-append I/O.

// *** Need to pass the abort proc and arg to TIFF_FileWriter::UpdateFileStream.

void TIFF_MetaHandler::UpdateFile ( bool doSafeUpdate )
{
	XMP_Assert ( ! doSafeUpdate );	// This should only be called for "unsafe" updates.

	XMP_IO*   destRef    = this->parent->ioRef;
	XMP_AbortProc abortProc  = this->parent->abortProc;
	void *        abortArg   = this->parent->abortArg;

	XMP_Int64 oldPacketOffset = this->packetInfo.offset;
	XMP_Int32 oldPacketLength = this->packetInfo.length;

	if ( oldPacketOffset == kXMPFiles_UnknownOffset ) oldPacketOffset = 0;	// ! Simplify checks.
	if ( oldPacketLength == kXMPFiles_UnknownLength ) oldPacketLength = 0;

	bool fileHadXMP = ((oldPacketOffset != 0) && (oldPacketLength != 0));

	// Update the IPTC-IIM and native TIFF/Exif metadata. ExportPhotoData also trips the tiff: and
	// exif: copies from the XMP, so reserialize the now final XMP packet.

	ExportPhotoData ( kXMP_TIFFFile, &this->xmpObj, &this->tiffMgr, this->iptcMgr, this->psirMgr );

	try {
		XMP_OptionBits options = kXMP_UseCompactFormat;
		if ( fileHadXMP ) options |= kXMP_ExactPacketLength;
		this->xmpObj.SerializeToBuffer ( &this->xmpPacket, options, oldPacketLength );
	} catch ( ... ) {
		this->xmpObj.SerializeToBuffer ( &this->xmpPacket, kXMP_UseCompactFormat );
	}

	// Decide whether to do an in-place update. This can only happen if all of the following are true:
	//	- There is an XMP packet in the file.
	//	- The are no changes to the legacy tags. (The IPTC and PSIR are in the TIFF tags.)
	//	- The new XMP can fit in the old space.

	bool doInPlace = (fileHadXMP && (this->xmpPacket.size() <= (size_t)oldPacketLength));
	if ( this->tiffMgr.IsLegacyChanged() ) doInPlace = false;
	
	bool localProgressTracking = false;
	XMP_ProgressTracker* progressTracker = this->parent->progressTracker;

	if ( ! doInPlace ) {

		#if GatherPerformanceData
			sAPIPerf->back().extraInfo += ", TIFF append update";
		#endif

		if ( (progressTracker != 0) && (! progressTracker->WorkInProgress()) ) {
			localProgressTracking = true;
			progressTracker->BeginWork();
		}

		this->tiffMgr.SetTag ( kTIFF_PrimaryIFD, kTIFF_XMP, kTIFF_UndefinedType, (XMP_Uns32)this->xmpPacket.size(), this->xmpPacket.c_str() );
		this->tiffMgr.UpdateFileStream ( destRef, progressTracker );

	} else {

		#if GatherPerformanceData
			sAPIPerf->back().extraInfo += ", TIFF in-place update";
		#endif

		if ( this->xmpPacket.size() < (size_t)this->packetInfo.length ) {
			// They ought to match, cheap to be sure.
			size_t extraSpace = (size_t)this->packetInfo.length - this->xmpPacket.size();
			this->xmpPacket.append ( extraSpace, ' ' );
		}

		XMP_IO* liveFile = this->parent->ioRef;

		XMP_Assert ( this->xmpPacket.size() == (size_t)oldPacketLength );	// ! Done by common PutXMP logic.

		if ( progressTracker != 0 ) {
			if ( progressTracker->WorkInProgress() ) {
				progressTracker->AddTotalWork ( (float(this->xmpPacket.size())) );
			} else {
				localProgressTracking = true;
				progressTracker->BeginWork ( (float(this->xmpPacket.size())) );
			}
		}

		liveFile->Seek ( oldPacketOffset, kXMP_SeekFromStart  );
		liveFile->Write ( this->xmpPacket.c_str(), (XMP_Int32)this->xmpPacket.size() );

	}
	
	if ( localProgressTracking ) progressTracker->WorkComplete();
	this->needsUpdate = false;

}	// TIFF_MetaHandler::UpdateFile

// =================================================================================================
// TIFF_MetaHandler::WriteTempFile
// ===============================
//
// The structure of TIFF makes it hard to do a sequential source-to-dest copy with interleaved
// updates. So, copy the existing source to the destination and call UpdateFile.

void TIFF_MetaHandler::WriteTempFile ( XMP_IO* tempRef )
{
	XMP_IO* origRef = this->parent->ioRef;

	XMP_AbortProc abortProc  = this->parent->abortProc;
	void *        abortArg   = this->parent->abortArg;

	XMP_Int64 fileLen = origRef->Length();
	if ( fileLen > 0xFFFFFFFFLL ) {	// Check before making a copy of the file.
		XMP_Throw ( "TIFF fles can't exceed 4GB", kXMPErr_BadTIFF );
	}
	
	XMP_ProgressTracker* progressTracker = this->parent->progressTracker;
	if ( progressTracker != 0 ) progressTracker->BeginWork ( (float)fileLen );

	origRef->Rewind ( );
	tempRef->Truncate ( 0 );
	XIO::Copy ( origRef, tempRef, fileLen, abortProc, abortArg );

	try {
		this->parent->ioRef = tempRef;	// ! Make UpdateFile update the temp.
		this->UpdateFile ( false );
		this->parent->ioRef = origRef;
	} catch ( ... ) {
		this->parent->ioRef = origRef;
		throw;
	}
	
	if ( progressTracker != 0 ) progressTracker->WorkComplete();

}	// TIFF_MetaHandler::WriteTempFile

// =================================================================================================
