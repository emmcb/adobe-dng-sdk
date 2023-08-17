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
// 07-27-11 AWL 5.4-f002 Add support for new and modified Exif 2.3 tags.
//
// 08-18-10 AWL 5.3-f002 Don't include XIO.hpp in any headers, only .cpp files.
//
// 02-25-10 AWL 5.1-f004 [2558846] Fix ImportPhotoData to not trip over new alias for exif:DateTimeDigitized to xmp:CreateDate.
//
// 12-18-09 AWL 5.0-f124 [2505903] Redo the date/time fix to separate Exif DateTimeOriginal and IPTC DateCreated.
// 12-03-09 AWL 5.0-f120 [2499192] Add special case support for exif:ISOSpeedRatings over 65535.
// 01-06-09 AWL 5.0-f015 [1937288] Do delete all of tiff: and exif: on import, saving new writebacks.
//				Change import of Exif RelatedSoundFile to accept any length.
// 01-05-09 AWL 5.0-f013 [1937288] Don't delete all of tiff: and exif: on import, just the digests.
// 12-18-08 AWL 5.0-f010 [1932925] Fix TIFF handler to ignore trailing zero bytes for LONG tag 33723.
//				Don't parse read-only IPTC if the digest matches, always parse for update.
// 11-18-08 AWL 5.0-f008 Fix server mode to not delete XMP for non-ASCII input that is ignored.
// 11-13-08 AWL 5.0-f005 Fix a couple of missed cases of UNIX build hacks.
// 11-13-08 AWL 5.0-f004 Add server mode support that ignores local text. Enable all handlers except
//				MOV for generic UNIX - that will be handled as part of the rewrite.
// 10-31-08 AWL 5.0-f002 MWG compliance changes: Fix Exif Artist and date/time mapping bugs.
//
// 10-23-08 AWL 4.4-f015 MWG compliance changes: Don't keep device properties in the file's XMP;
//				mapping changes for 3-way properties, especially description and date/time.
// 10-14-08 AWL 4.4-f014 MWG compliance changes: simplified block selection.
//
// 02-18-08 AWL 4.2-f077 More changes to generic UNIX builds for XMPFiles.
// 01-30-08 AWL 4.2-f065 [1643402] Always strip IIM and PSIR from DNG. Have TIFF handler reject DNG
//				if DNGBackwardVersion has a high order byte that is not 1 (>= 2.0.0.0).
// 01-29-08 AWL 4.2-f064 [1643402] Don't write IIM or PSIR blocks into DNG files.
// 01-25-07 AWL 4.2-f011 [1465694] Change legacy import logic to be a little closer to Photoshop in
//				an edge case where the IPTC digest is missing.
//
// 11-10-06 AWL 4.1-f066 [1417221] Tweak the "legacy digest missing" logic to import legacy values
//				that do not have corresponding XMP.
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
// 08-31-06 AWL 4.1-f032 More work on in-place updates. Have ReconcileXMPtoJTP only update the IPTC
//				and PSIR blocks if they have changed. Move the XMP updating out to the file handler.
//
// 07-10-06 AWL 4.0-f014 Initial version of new read-write JPEG handler and underpinnings. Reasonably
//				but not thoroughly tested, still within NewHandlers conditional.
// 04-21-06 AWL First draft.
//
// =================================================================================================
#endif // AdobePrivate

#include "public/include/XMP_Environment.h"	// ! XMP_Environment.h must be the first included header.
#include "public/include/XMP_Const.h"

#include "XMPFiles/source/FormatSupport/ReconcileLegacy.hpp"
#include "XMPFiles/source/FormatSupport/Reconcile_Impl.hpp"
#include "source/XIO.hpp"

// =================================================================================================
/// \file ReconcileLegacy.cpp
/// \brief Top level parts of utilities to reconcile between XMP and legacy metadata forms such as
/// TIFF/Exif and IPTC.
///
// =================================================================================================

// =================================================================================================
// ImportPhotoData
// ===============
//
// Import legacy metadata for JPEG, TIFF, and Photoshop files into the XMP. The caller must have
// already done the file specific processing to select the appropriate sources of the TIFF stream,
// the Photoshop image resources, and the IPTC.

#define SaveExifTag(ns,prop)	\
	if ( xmp->DoesPropertyExist ( ns, prop ) ) SXMPUtils::DuplicateSubtree ( *xmp, &savedExif, ns, prop )
#define RestoreExifTag(ns,prop)	\
	if ( savedExif.DoesPropertyExist ( ns, prop ) ) SXMPUtils::DuplicateSubtree ( savedExif, xmp, ns, prop )

void ImportPhotoData ( const TIFF_Manager & exif,
					   const IPTC_Manager & iptc,
					   const PSIR_Manager & psir,
					   int                  iptcDigestState,
					   SXMPMeta *		    xmp,
					   XMP_OptionBits	    options /* = 0 */ )
{
	bool haveXMP  = XMP_OptionIsSet ( options, k2XMP_FileHadXMP );
	bool haveExif = XMP_OptionIsSet ( options, k2XMP_FileHadExif );
	bool haveIPTC = XMP_OptionIsSet ( options, k2XMP_FileHadIPTC );
	
	// Save some new Exif writebacks that can be XMP-only from older versions, delete all of the
	// XMP's tiff: and exif: namespaces (they should only reflect native Exif), then put back the
	// saved writebacks (which might get replaced by the native Exif values in the Import calls).
	// The value of exif:ISOSpeedRatings is saved for special case handling of ISO over 65535.

	bool haveOldExif = true;	// Default to old Exif if no version tag.
	TIFF_Manager::TagInfo tagInfo;
	bool found = exif.GetTag ( kTIFF_ExifIFD, kTIFF_ExifVersion, &tagInfo );
	if ( found && (tagInfo.type == kTIFF_UndefinedType) && (tagInfo.count == 4) ) {
		haveOldExif = (strncmp ( (char*)tagInfo.dataPtr, "0230", 4 ) < 0);
	}
	
	SXMPMeta savedExif;
	
	SaveExifTag ( kXMP_NS_EXIF, "DateTimeOriginal" );
	SaveExifTag ( kXMP_NS_EXIF, "GPSLatitude" );
	SaveExifTag ( kXMP_NS_EXIF, "GPSLongitude" );
	SaveExifTag ( kXMP_NS_EXIF, "GPSTimeStamp" );
	SaveExifTag ( kXMP_NS_EXIF, "GPSAltitude" );
	SaveExifTag ( kXMP_NS_EXIF, "GPSAltitudeRef" );
	SaveExifTag ( kXMP_NS_EXIF, "ISOSpeedRatings" );
	
	SXMPUtils::RemoveProperties ( xmp, kXMP_NS_TIFF, 0, kXMPUtil_DoAllProperties );
	SXMPUtils::RemoveProperties ( xmp, kXMP_NS_EXIF, 0, kXMPUtil_DoAllProperties );
	if ( ! haveOldExif ) SXMPUtils::RemoveProperties ( xmp, kXMP_NS_ExifEX, 0, kXMPUtil_DoAllProperties );

	RestoreExifTag ( kXMP_NS_EXIF, "DateTimeOriginal" );
	RestoreExifTag ( kXMP_NS_EXIF, "GPSLatitude" );
	RestoreExifTag ( kXMP_NS_EXIF, "GPSLongitude" );
	RestoreExifTag ( kXMP_NS_EXIF, "GPSTimeStamp" );
	RestoreExifTag ( kXMP_NS_EXIF, "GPSAltitude" );
	RestoreExifTag ( kXMP_NS_EXIF, "GPSAltitudeRef" );
	RestoreExifTag ( kXMP_NS_EXIF, "ISOSpeedRatings" );

	// Not obvious here, but the logic in PhotoDataUtils follows the MWG reader guidelines.
	
	PhotoDataUtils::ImportPSIR ( psir, xmp, iptcDigestState );

	if ( haveIPTC ) PhotoDataUtils::Import2WayIPTC ( iptc, xmp, iptcDigestState );
	if ( haveExif ) PhotoDataUtils::Import2WayExif ( exif, xmp, iptcDigestState );

	if ( haveExif | haveIPTC ) PhotoDataUtils::Import3WayItems ( exif, iptc, xmp, iptcDigestState );

	// If photoshop:DateCreated does not exist try to create it from exif:DateTimeOriginal.
	
	if ( ! xmp->DoesPropertyExist ( kXMP_NS_Photoshop, "DateCreated" ) ) {
		std::string exifValue;
		bool haveExifDTO = xmp->GetProperty ( kXMP_NS_EXIF, "DateTimeOriginal", &exifValue, 0 );
		if ( haveExifDTO ) xmp->SetProperty ( kXMP_NS_Photoshop, "DateCreated", exifValue.c_str() );
	}

}	// ImportPhotoData

// =================================================================================================
// ExportPhotoData
// ===============

void ExportPhotoData ( XMP_FileFormat destFormat,
					   SXMPMeta *     xmp,
					   TIFF_Manager * exif, // Pass 0 if not wanted.
					   IPTC_Manager * iptc, // Pass 0 if not wanted.
					   PSIR_Manager * psir, // Pass 0 if not wanted.
					   XMP_OptionBits options /* = 0 */ )
{
	XMP_Assert ( (destFormat == kXMP_JPEGFile) || (destFormat == kXMP_TIFFFile) || (destFormat == kXMP_PhotoshopFile) );

	// Do not write IPTC-IIM or PSIR in DNG files (which are a variant of TIFF).

	if ( (destFormat == kXMP_TIFFFile) && (exif != 0) &&
		 exif->GetTag ( kTIFF_PrimaryIFD, kTIFF_DNGVersion, 0 ) ) {

		iptc = 0;	// These prevent calls to ExportIPTC and ExportPSIR.
		psir = 0;

		exif->DeleteTag ( kTIFF_PrimaryIFD, kTIFF_IPTC );	// These remove any existing IPTC and PSIR.
		exif->DeleteTag ( kTIFF_PrimaryIFD, kTIFF_PSIR );

	}

	// Export the individual metadata items to the non-XMP forms. Set the IPTC digest whether or not
	// it changed, it might not have been present or correct before.

	bool iptcChanged = false;	// Save explicitly, internal flag is reset by UpdateMemoryDataSets.

	void *    iptcPtr = 0;
	XMP_Uns32 iptcLen = 0;
	
	if ( iptc != 0 ) {
		PhotoDataUtils::ExportIPTC ( *xmp, iptc );
		iptcChanged = iptc->IsChanged();
		if ( iptcChanged ) iptc->UpdateMemoryDataSets();
		iptcLen = iptc->GetBlockInfo ( &iptcPtr );
		if ( psir != 0 ) PhotoDataUtils::SetIPTCDigest ( iptcPtr, iptcLen, psir );
	}

	if ( exif != 0 ) PhotoDataUtils::ExportExif ( xmp, exif );
	if ( psir != 0 ) PhotoDataUtils::ExportPSIR ( *xmp, psir );

	// Now update the non-XMP collections of metadata according to the file format. Do not update
	// the XMP here, that is done in the file handlers after deciding if an XMP-only in-place
	// update should be done.
	// - JPEG has the IPTC in PSIR 1028, the Exif and PSIR are marker segments.
	// - TIFF has the IPTC and PSIR in primary IFD tags.
	// - PSD has everything in PSIRs.

	if ( destFormat == kXMP_JPEGFile ) {

		if ( iptcChanged && (psir != 0) ) psir->SetImgRsrc ( kPSIR_IPTC, iptcPtr, iptcLen );

	} else if ( destFormat == kXMP_TIFFFile ) {

		XMP_Assert ( exif != 0 );

		if ( iptcChanged ) exif->SetTag ( kTIFF_PrimaryIFD, kTIFF_IPTC, kTIFF_UndefinedType, iptcLen, iptcPtr );

		if ( (psir != 0) && psir->IsChanged() ) {
			void* psirPtr;
			XMP_Uns32 psirLen = psir->UpdateMemoryResources ( &psirPtr );
			exif->SetTag ( kTIFF_PrimaryIFD, kTIFF_PSIR, kTIFF_UndefinedType, psirLen, psirPtr );
		}

	} else if ( destFormat == kXMP_PhotoshopFile ) {

		XMP_Assert ( psir != 0 );

		if ( iptcChanged ) psir->SetImgRsrc ( kPSIR_IPTC, iptcPtr, iptcLen );

		if ( (exif != 0) && exif->IsChanged() ) {
			void* exifPtr;
			XMP_Uns32 exifLen = exif->UpdateMemoryStream ( &exifPtr );
			psir->SetImgRsrc ( kPSIR_Exif, exifPtr, exifLen );
		}

	}
	
	// Strip the tiff: and exif: namespaces from the XMP, we're done with them. Save the Exif
	// ISOSpeedRatings if any of the values are over 0xFFFF, the native tag is SHORT. Lower level
	// code already kept or stripped the XMP form.

	bool haveOldExif = true;	// Default to old Exif if no version tag.
	if ( exif != 0 ) {
		TIFF_Manager::TagInfo tagInfo;
		bool found = exif->GetTag ( kTIFF_ExifIFD, kTIFF_ExifVersion, &tagInfo );
		if ( found && (tagInfo.type == kTIFF_UndefinedType) && (tagInfo.count == 4) ) {
			haveOldExif = (strncmp ( (char*)tagInfo.dataPtr, "0230", 4 ) < 0);
		}
	}
	
	SXMPMeta savedExif;
	SaveExifTag ( kXMP_NS_EXIF, "ISOSpeedRatings" );
	
	SXMPUtils::RemoveProperties ( xmp, kXMP_NS_TIFF, 0, kXMPUtil_DoAllProperties );
	SXMPUtils::RemoveProperties ( xmp, kXMP_NS_EXIF, 0, kXMPUtil_DoAllProperties );
	if ( ! haveOldExif ) SXMPUtils::RemoveProperties ( xmp, kXMP_NS_ExifEX, 0, kXMPUtil_DoAllProperties );

	RestoreExifTag ( kXMP_NS_EXIF, "ISOSpeedRatings" );

}	// ExportPhotoData
