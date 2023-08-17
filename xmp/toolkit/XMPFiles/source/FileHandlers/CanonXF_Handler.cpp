#if AdobePrivate	// ! This entire handler is private, it must not go in the public SDK.

// =================================================================================================
// Copyright Adobe
// Copyright 2007 Adobe
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
//  PEL Peter Lee
//  AWL Alan Lillich
//  JEH Joerg Ehrlich
//  SKP Sunil Kishor Pathak
//  ADC Amandeep Chawla
//  IJS Inder Jeet Singh
//	HK  Honey Kansal
//	SKP	Sunil Kishor Pathak
//
// mm-dd-yy who Description of changes, most recent on top
//
// 03-26-14 IJS 5.6-f100 [3699475] Writing Old Digest in CanonXF XMP results in re-import of dc:title from .CIF file in the next read.
// 07-01-13 SKP 5.6-f070 [3578790] Fix issues raised from Veracode static scan
// 04-26-13 ADC 5.6-f054 [3526891] Read-only checks in IsMetadataWritable() API try creating temporary files if no metadata exists, instead of checking for file permissions.
//						 [3525961] IsMetaDataWritable checks for Plugins and embedded handlers are different.
// 04-04-13 IJS 5.6-f051 Move common I/O methods to IOUtils.
//						 [3534631] Optimize GetAssociated Resources for RED_Handler
// 02-13-13 ADC 5.6-f035 [3498686, 3497304] XMP GetFileModDate() is extremely slow.
// 01-16-13 HK  5.6-f022 Implemented IsMetadataWritable API for XDCAM-EX, P2, CanonXF and AVCHD formats.
// 01-07-13 IJS 5.6-f018 Changed name of API IsWritable to IsMetadataWritable.
//                       Added root Path folder as associated Resource for folder based Formats.
// 01-03-13 IJS 5.6-f015 [3429812] Added GetAssociatedResources API for CononXF format.
//
// 10-10-12 ADC 5.5-f045 Implement the internal infrastructure for XMPFiles error notifications.
// 10-09-12 SKP 5.5-f044 Refactored LocateMetadataFiles API.
// 09-19-12 AWL 5.5-f033 [3211021] Fix the folder handlers complain about read-only XMP files when opening for update.
// 01-16-12	JEH	5.5-f007 [3092566] Fix GetFileModDate for CanonXF.
// 01-16-12	AWL	5.5-f006 [2980767] Improve sidecar file update for disk full situations.
//
// 11-16-11 JEH 5.4-f037 [2747840] CanonXF handler maps native value directly to xmp:CreatorTool now
// 10-27-11 AWL 5.4-f030 [3007461] Improve performance of GetFileModDate.
// 10-21-11 AWL 5.4-f027 [2891842] Fix case sensitivity in Canon XF and XDCAM EX handlers.
// 09-29-11 AWL 5.4-f015 Add AVCHD and CanonXF support for GetFileModDate.
//
// 02-01-11	AWL	5.3-f025 [2800994] Apply fixes to Canon XF handler from Vivek Neelamegam.
// 10-19-10	AWL	5.3-f012 Canon XF handler fixes from Peter Lee for improper property modeling.
// 09-30-10	AWL	5.3-f010 Add Canon XF handler from Peter Lee.
//
// 05-03-10 PEL Initial implementation based on P2 handler.
//
// =================================================================================================
#endif

#include "public/include/XMP_Environment.h"	// ! XMP_Environment.h must be the first included header.

#include "public/include/XMP_Const.h"
#include "public/include/XMP_IO.hpp"

#include "XMPFiles/source/XMPFiles_Impl.hpp"
#include "source/XMPFiles_IO.hpp"
#include "source/XIO.hpp"
#include "source/IOUtils.hpp"

#include "XMPFiles/source/FileHandlers/CanonXF_Handler.hpp"
#include "XMPFiles/source/FormatSupport/PackageFormat_Support.hpp"

#include <cmath>

using namespace std;

// =================================================================================================
/// \file CanonXF_Handler.cpp
/// \brief Folder format handler for Canon XF media.
///
/// This handler is for the video format used by Canon's line of Professional video cameras, which
/// currently have model names starting with "XF." This is a pseudo-package, visible files but with
/// a very well-defined layout and naming rules. A typical Canon XF example looks like:
///
/// .../MyMovie
/// 	CONTENTS/
/// 	    CLIPS001/
/// 	        INDEX.MIF
/// 	        NZ0001/
/// 	            NZ0001.CIF
/// 	            NZ0001.XML
/// 	            NZ0001.CPF
/// 	            NZ0001.THM
/// 	            NZ000101.MXF
/// 	            NZ000101.SIF
/// 	            NZ000102.MXF
/// 	            NZ000102.SIF
/// 	        NZ0002/
/// 	            NZ0002.CIF
/// 	            NZ0002.XML
/// 	            NZ0002.CPF
/// 	            NZ0002.THM
/// 	            NZ000201.MXF
/// 	            NZ000201.SIF
///
/// From the user's point of view, .../MyMovie contains Canon XF stuff, in this case 2 clips whose
/// raw names are NZ0001 and NZ0002. NZ0001 is divided into 2 "streams," NZ000101 and NZ000102.
/// Streams in Canon's terminology are chunks of a single take which are created automatically by
/// the camera whenever an MXF media file reaches 2GB in size. There may be mapping information for
/// nicer clip names to the raw names, but that can be ignored for now. Each clip is stored as a
/// collection of files, each file holding some specific aspect of the clip's data.
///
/// The Canon XF handler operates on clips. The path from the client of XMPFiles can be either a
/// logical clip path, like ".../MyMovie/NZ0001", or a full path to one of the files. In the latter
/// case the handler must figure out the intended clip, it must not blindly use the named file.
///
/// Once the Canon XF structure and intended clip are identified, the handler only deals with the
/// .XMP, .CIF and .XML files in the clip folder (e.g. "NZ0001/" above). The .XMP file, if present,
/// contains the XMP for the clip. The .CIF file must be present to define the existance of the
/// clip. It contains a variety of information about the clip, including some legacy metadata. The
/// .XML file is optional. If present, it provides additional metadata that suplements and may
/// override data stored in the .CIF file. The .SIF ("Stream Information File"), THM (thumbnail --
/// optional), and INDEX.MIF (information about the entire directory tree) files are not currently
/// used by this handler.
// =================================================================================================

// Structures representing the contents of a binary Canon XF .CIF file. See Annex D.2 of the
// "Content Specification for File Based Canon Video Camera" version 1.0.0 for details. the document
// is currently marked "Canon Confidential" and should not be shared outside of Adobe.

// Figure D. 2 – Coding_Info structure
struct CIF_CodingInfo
{
	XMP_Uns8	mVideoTypeAndAspectRatio;
	XMP_Uns8	mVideoFormatAndRate;
	XMP_Uns8	mAudioTypeAndFrequency;
	XMP_Uns8	mAudioChannel;
	XMP_Uns8	mReserved [4];
};

// Table D. 12 – Member_Stream_Info structure
struct CIF_MemberStreamInfo
{
	XMP_Uns8	mStreamNumber;
	XMP_Uns8	mReserved [3];
	XMP_Uns32	mNumberOfFramesInStream;
};

// Figure D. 3 – Record_Time_and_Date structure
struct CIF_RecordDateAndTime
{
	XMP_Uns8	mTimeZoneInfo;
	XMP_Uns8	mRecordDateYear_Hundreds;
	XMP_Uns8	mRecordDateYear_Tens;
	XMP_Uns8	mRecordDateMonth;
	XMP_Uns8	mRecordDateDay;
	XMP_Uns8	mRecordDateHour;
	XMP_Uns8	mRecordDateMinutes;
	XMP_Uns8	mRecordDateSeconds;
};

// Figure D. 4 – Recording_Mode_Information structure
struct CIF_RecordingModeInformation
{
	XMP_Uns8	mRecordingRate;
	XMP_Uns8	mRecordingMode [2];
	XMP_Uns8	mPreRecTime;
	XMP_Uns8	mIntervalFrameNum;
	XMP_Uns8	mIntervalUnit;
	XMP_Uns8	mIntervalTime;
	XMP_Uns8	mFrameRecNum;
	XMP_Uns16	mVariableFrameRate;
	XMP_Uns8	mAutoExposureMode;
	XMP_Uns8	mWhiteBalanceInfo;
	XMP_Uns16	mColorTemperature;
	XMP_Uns8	mReserved [10];
};

// Figure D. 7 – Clip_Attributes structure
struct CIF_ClipAttributes
{
	XMP_Uns8	mAttributes;
	XMP_Uns8	mReserved [7];
};

// Table D. 31 – Relay_Information Structure (Card spanning information)
struct CIF_RelayInformation
{
	XMP_Uns8	mRecMediaID [16];
	XMP_Uns8	mRelayPreviousMediaID [16];
	XMP_Uns8	mRelayPreviousClipID [6];
	XMP_Uns8	mReserved [2];
	XMP_Uns8	mRelayPreviousUMID [32];
	XMP_Uns8	mRelayNextMediaID [16];
	XMP_Uns8	mRelayNextClipID [6];
	XMP_Uns8	mReserved2 [2];
	XMP_Uns8	mRelayNextUMID [32];
};

// Table D. 32 – Shot_Mark structure
struct CIF_ShotMark
{
	XMP_Uns8	mMarkAttribute;
	XMP_Uns8	mFrameNumber [3];
};

// Table D. 33 – Event_Mark structure
struct CIF_EventMark
{
	XMP_Uns8	mEventType;
	XMP_Uns8	mEventData [3];
	XMP_Uns32	mFrameNumber;
};

// Table D. 13 – Clip_Metadata Structure
struct CIF_ClipMetadata
{
	XMP_Uns8						mClipMetadataID [2];
	XMP_Uns8						mVersionNumber [2];
	XMP_Uns8						mMakerName [32];
	XMP_Uns8						mModelName [32];
	XMP_Uns8						mSerialNumber [32];
	XMP_Uns8						mFirmVersion [8];
	CIF_RecordDateAndTime			mRecordDataAndTime;
	XMP_Uns8						mStartTimecode [4];
	XMP_Uns8						mStopTimecode [4];
	XMP_Uns32						mThumbnailFrameNumber;
	CIF_RecordingModeInformation	mRecordingModeInformation;
	CIF_ClipAttributes				mClipAttributes;
	CIF_RelayInformation			mRelayInformation;
	XMP_Uns8						mNumberOfShotMarks;
	XMP_Uns8						mNumberOfEventMarks;
	XMP_Uns8						mReserved [2];
	CIF_ShotMark					mShotMarks [100];
	CIF_EventMark					mEventMarks [100];
	XMP_Uns8						mReserved2 [216];
};

// Table D. 4 – Clip Information File Structure
struct CIF_LegacyMetadata
{
	XMP_Uns8				mClipInfoID [2];
	XMP_Uns8				mVersionNumber_Major;
	XMP_Uns8				mVersionNumber_Minor;
	CIF_CodingInfo			mCodingInfo;
	XMP_Uns32				mNumberOfFramesInClip;
	XMP_Uns8				mReserved [40];
	XMP_Uns32				mNumberOfStreamFiles;
	CIF_MemberStreamInfo	mMemberStreamInfo [99];
	CIF_ClipMetadata		mClipMetadata;
};

// =================================================================================================
// InternalMakeClipFilePath
// ========================
//
// CanonXF_CheckFormat can't use the member function.

static void InternalMakeClipFilePath ( std::string * path,
									   const std::string & rootPath,
									   const std::string & clipName,
									   XMP_StringPtr suffix )
{

	*path = rootPath;
	*path += kDirChar;
	*path += "CONTENTS";
	*path += kDirChar;
	*path += "CLIPS001";
	*path += kDirChar;
	*path += clipName;
	*path += kDirChar;
	*path += clipName;
	*path += suffix;

}	// InternalMakeClipFilePath

// =================================================================================================
// ReadCanonCIFCodingInfo
// ======================

static inline bool ReadCanonCIFCodingInfo ( XMPFiles_IO& cpiFile, CIF_CodingInfo& cifCodingInfo )
{
	cpiFile.ReadAll ( &cifCodingInfo.mVideoTypeAndAspectRatio,  1 );
	cpiFile.ReadAll ( &cifCodingInfo.mVideoFormatAndRate,  1 );
	cpiFile.ReadAll ( &cifCodingInfo.mAudioTypeAndFrequency,  1 );
	cpiFile.ReadAll ( &cifCodingInfo.mAudioChannel,  1 );
	cpiFile.ReadAll ( cifCodingInfo.mReserved,  4 );

	return true;
}

// =================================================================================================
// ReadCanonCIFMemberStreamInfo
// ============================

static inline bool ReadCanonCIFMemberStreamInfo ( XMPFiles_IO& cpiFile, CIF_MemberStreamInfo* cifMemberStreamInfo )
{
	for ( int i = 0; i < 99; ++i ) {
		cpiFile.ReadAll ( &cifMemberStreamInfo [i].mStreamNumber,  1 );
		cpiFile.ReadAll ( cifMemberStreamInfo [i].mReserved,  3 );
		cifMemberStreamInfo [i].mNumberOfFramesInStream = XIO::ReadUns32_BE ( &cpiFile );
	}

	return true;
}

// =================================================================================================
// ReadCanonCIFRecordDateAndTime
// =============================

static inline bool ReadCanonCIFRecordDateAndTime ( XMPFiles_IO& cpiFile, CIF_RecordDateAndTime& cifRecordDateAndTime )
{
	cpiFile.ReadAll ( &cifRecordDateAndTime.mTimeZoneInfo,  1 );
	cpiFile.ReadAll ( &cifRecordDateAndTime.mRecordDateYear_Hundreds,  1 );
	cpiFile.ReadAll ( &cifRecordDateAndTime.mRecordDateYear_Tens,  1 );
	cpiFile.ReadAll ( &cifRecordDateAndTime.mRecordDateMonth,  1 );
	cpiFile.ReadAll ( &cifRecordDateAndTime.mRecordDateDay,  1 );
	cpiFile.ReadAll ( &cifRecordDateAndTime.mRecordDateHour,  1 );
	cpiFile.ReadAll ( &cifRecordDateAndTime.mRecordDateMinutes,  1 );
	cpiFile.ReadAll ( &cifRecordDateAndTime.mRecordDateSeconds,  1 );

	return true;
}

// =================================================================================================
// ReadCanonCIFRecordingModeInformation
// ====================================

static inline bool ReadCanonCIFRecordingModeInformation ( XMPFiles_IO& cpiFile, CIF_RecordingModeInformation& cifRecordingModeInformation )
{
	cpiFile.ReadAll ( &cifRecordingModeInformation.mRecordingRate,  1 );
	cpiFile.ReadAll ( cifRecordingModeInformation.mRecordingMode,  2 );
	cpiFile.ReadAll ( &cifRecordingModeInformation.mPreRecTime,  1 );
	cpiFile.ReadAll ( &cifRecordingModeInformation.mIntervalFrameNum,  1 );
	cpiFile.ReadAll ( &cifRecordingModeInformation.mIntervalUnit,  1 );
	cpiFile.ReadAll ( &cifRecordingModeInformation.mIntervalTime,  1 );
	cpiFile.ReadAll ( &cifRecordingModeInformation.mFrameRecNum,  1 );
	cifRecordingModeInformation.mVariableFrameRate = XIO::ReadUns16_BE ( &cpiFile );
	cpiFile.ReadAll ( &cifRecordingModeInformation.mAutoExposureMode,  1 );
	cpiFile.ReadAll ( &cifRecordingModeInformation.mWhiteBalanceInfo,  1 );
	cifRecordingModeInformation.mColorTemperature = XIO::ReadUns16_BE ( &cpiFile );
	cpiFile.ReadAll ( cifRecordingModeInformation.mReserved,  10 );

	return true;
}

// =================================================================================================
// ReadCanonCIFClipAttributes
// ==========================

static inline bool ReadCanonCIFClipAttributes ( XMPFiles_IO& cpiFile, CIF_ClipAttributes& cifClipAttributes )
{
	cpiFile.ReadAll ( &cifClipAttributes.mAttributes,  1 );
	cpiFile.ReadAll ( cifClipAttributes.mReserved,  7 );

	return true;
}

// =================================================================================================
// ReadCanonCIFRelayInformation
// ============================

static inline bool ReadCanonCIFRelayInformation ( XMPFiles_IO& cpiFile, CIF_RelayInformation& cifRelayInformation )
{
	cpiFile.ReadAll ( cifRelayInformation.mRecMediaID,  16 );
	cpiFile.ReadAll ( cifRelayInformation.mRelayPreviousMediaID,  16 );
	cpiFile.ReadAll ( cifRelayInformation.mRelayPreviousClipID,  6 );
	cpiFile.ReadAll ( cifRelayInformation.mReserved,  2 );
	cpiFile.ReadAll ( cifRelayInformation.mRelayPreviousUMID,  32 );
	cpiFile.ReadAll ( cifRelayInformation.mRelayNextMediaID,  16 );
	cpiFile.ReadAll ( cifRelayInformation.mRelayNextClipID,  6 );
	cpiFile.ReadAll ( cifRelayInformation.mReserved2,  2 );
	cpiFile.ReadAll ( cifRelayInformation.mRelayNextUMID,  32 );

	return true;
}

// =================================================================================================
// ReadCanonCIFShotMarks
// =====================

static inline bool ReadCanonCIFShotMarks ( XMPFiles_IO& cpiFile, CIF_ShotMark* cifShotMarks, int numShotMarks )
{
	for ( int i = 0; i < numShotMarks; ++i ) {
		cpiFile.ReadAll ( &cifShotMarks[i].mMarkAttribute,  1 );
		cpiFile.ReadAll ( cifShotMarks[i].mFrameNumber,  3 );
	}

	return true;
}

// =================================================================================================
// ReadCanonCIFEventMarks
// ======================

static inline bool ReadCanonCIFEventMarks ( XMPFiles_IO& cpiFile, CIF_EventMark* cifEventMarks, int numEventMarks )
{
	for ( int i = 0; i < numEventMarks; ++i ) {
		cpiFile.ReadAll ( &cifEventMarks[i].mEventType,  1 );
		cpiFile.ReadAll ( cifEventMarks[i].mEventData,  3 );
		cifEventMarks[i].mFrameNumber = XIO::ReadUns32_BE ( &cpiFile );
	}

	return true;
}

// =================================================================================================
// ReadCanonCIFClipMetadata
// ========================

static inline bool ReadCanonCIFClipMetadata ( XMPFiles_IO& cpiFile, CIF_ClipMetadata& cifClipMetadata )
{
	cpiFile.ReadAll ( cifClipMetadata.mClipMetadataID,  2 );
	cpiFile.ReadAll ( cifClipMetadata.mVersionNumber,  2 );
	cpiFile.ReadAll ( cifClipMetadata.mMakerName,  32 );
	cpiFile.ReadAll ( cifClipMetadata.mModelName,  32 );
	cpiFile.ReadAll ( cifClipMetadata.mSerialNumber,  32 );
	cpiFile.ReadAll ( cifClipMetadata.mFirmVersion,  8 );

	if ( ! ReadCanonCIFRecordDateAndTime ( cpiFile, cifClipMetadata.mRecordDataAndTime ) ) return false;

	cpiFile.ReadAll ( cifClipMetadata.mStartTimecode,  4 );
	cpiFile.ReadAll ( cifClipMetadata.mStopTimecode,  4 );
	cifClipMetadata.mThumbnailFrameNumber = XIO::ReadUns32_BE ( &cpiFile );

	if ( ! ReadCanonCIFRecordingModeInformation ( cpiFile, cifClipMetadata.mRecordingModeInformation ) ) return false;
	if ( ! ReadCanonCIFClipAttributes ( cpiFile, cifClipMetadata.mClipAttributes ) ) return false;
	if ( ! ReadCanonCIFRelayInformation ( cpiFile, cifClipMetadata.mRelayInformation ) ) return false;

	cpiFile.ReadAll ( &cifClipMetadata.mNumberOfShotMarks,  1 );
	cpiFile.ReadAll ( &cifClipMetadata.mNumberOfEventMarks,  1 );
	cpiFile.ReadAll ( cifClipMetadata.mReserved,  2 );

	if ( ! ReadCanonCIFShotMarks ( cpiFile, cifClipMetadata.mShotMarks, 100 ) ) return false;
	if ( ! ReadCanonCIFEventMarks ( cpiFile, cifClipMetadata.mEventMarks, 100 ) ) return false;

	cpiFile.ReadAll ( cifClipMetadata.mReserved2,  216 );

	return true;
}

// =================================================================================================
// ReadCanonXFLegacyClipFile
// =========================

static bool ReadCanonXFLegacyClipFile ( const std::string& strPath, CIF_LegacyMetadata& cifLegacyData )
{
	bool success = false;

	try {

		Host_IO::FileRef hostRef = Host_IO::Open ( strPath.c_str(), Host_IO::openReadOnly );
		if ( hostRef == Host_IO::noFileRef ) return false;	// The open failed.
		XMPFiles_IO cpiFile ( hostRef, strPath.c_str(), Host_IO::openReadOnly );

		memset ( &cifLegacyData, 0, sizeof(CIF_LegacyMetadata) );

		// Read the CIF data
		cpiFile.ReadAll ( cifLegacyData.mClipInfoID,  2 );
		cpiFile.ReadAll ( &cifLegacyData.mVersionNumber_Major,  1 );
		cpiFile.ReadAll ( &cifLegacyData.mVersionNumber_Minor,  1 );

		if ( ( cifLegacyData.mClipInfoID[0] != 'C' ) || ( cifLegacyData.mClipInfoID[1] != 'I' ) ) return false;
		if ( cifLegacyData.mVersionNumber_Major != 1 ) return false;

		if ( ! ReadCanonCIFCodingInfo ( cpiFile, cifLegacyData.mCodingInfo ) ) return false;
		cifLegacyData.mNumberOfFramesInClip = XIO::ReadUns32_BE ( &cpiFile );
		cpiFile.ReadAll ( cifLegacyData.mReserved,  40 );
		cifLegacyData.mNumberOfStreamFiles = XIO::ReadUns32_BE ( &cpiFile );
		if ( ! ReadCanonCIFMemberStreamInfo ( cpiFile, cifLegacyData.mMemberStreamInfo ) ) return false;
		if ( ! ReadCanonCIFClipMetadata ( cpiFile, cifLegacyData.mClipMetadata ) ) return false;

		success = true;

	} catch ( ... ) {

		// Ignore all exceptions.

	}

	return success;
}

// =================================================================================================
// SetXMPVideoRatesAndSizes
// ========================
static void SetXMPVideoRatesAndSizes ( SXMPMeta& xmpObj, bool& containsXMP, const CIF_CodingInfo& cifCodingInfo,
									   XMP_Uns32 frameCount, const XMP_Uns8* startTimecode, XMP_Uns8 clipAttributes )
{
	const XMP_Uns8 videoType = cifCodingInfo.mVideoTypeAndAspectRatio >> 4;
	const XMP_Uns8 videoAspectRatio = cifCodingInfo.mVideoTypeAndAspectRatio & 0xF;
	const XMP_Uns8 videoFormat = cifCodingInfo.mVideoFormatAndRate >> 4;
	const XMP_Uns8 videoFrameRate = cifCodingInfo.mVideoFormatAndRate & 0xF;

	if ( videoType == 1 ) {
		xmpObj.SetProperty ( kXMP_NS_DM, "videoCompressor", "MPEG2", kXMP_DeleteExisting );
		containsXMP = true;
	}

	if ( videoAspectRatio == 1 ) {

		std::string dmPixelAspectRatio;
		std::string dmWidth;
		std::string dmHeight;

		switch ( videoFormat ) {
			case 1:		dmWidth = "1920"; dmHeight = "1080"; dmPixelAspectRatio = "1/1";	break;
			case 2:		dmWidth = "1280"; dmHeight = "720"; dmPixelAspectRatio = "1/1";	break;
			case 3:		dmWidth = "1440"; dmHeight = "1080"; dmPixelAspectRatio = "1920/1440";	break;
		}

		if ( ! dmPixelAspectRatio.empty() ) {
			xmpObj.SetProperty ( kXMP_NS_DM, "videoPixelAspectRatio", dmPixelAspectRatio, kXMP_DeleteExisting );
			xmpObj.SetStructField ( kXMP_NS_DM, "videoFrameSize", kXMP_NS_XMP_Dimensions, "w", dmWidth, 0 );
			xmpObj.SetStructField ( kXMP_NS_DM, "videoFrameSize", kXMP_NS_XMP_Dimensions, "h", dmHeight, 0 );
			xmpObj.SetStructField ( kXMP_NS_DM, "videoFrameSize", kXMP_NS_XMP_Dimensions, "unit", "pixel", 0 );
			containsXMP = true;
		}

	}

	const bool isDropFrame = ( ( clipAttributes >> 1 ) & 0x01 );

	{
		std::string dmTimeFormat;
		std::string dmTimeScale;
		std::string dmTimeSampleSize;
		std::string dmVideoFrameRate;

		switch ( videoFrameRate ) {
			case 1:		// 59.94i
				dmVideoFrameRate = "59.94i";
				dmTimeFormat = isDropFrame ? "2997DropTimecode" : "2997NonDropTimecode";
				dmTimeScale = "30000";	dmTimeSampleSize = "1001";
				break;
			case 2:		// 50i
				dmVideoFrameRate = "50i";
				dmTimeFormat = "25Timecode";
				dmTimeScale = "25";	dmTimeSampleSize = "1";
				break;
			case 3:		// 29.97p
				dmVideoFrameRate = "29.97p";
				dmTimeFormat = isDropFrame ? "2997DropTimecode" : "2997NonDropTimecode";
				dmTimeScale = "30000";	dmTimeSampleSize = "1001";
				break;
			case 4:		// 25p
				dmVideoFrameRate = "25p";
				dmTimeFormat = "25Timecode";
				dmTimeScale = "25";	dmTimeSampleSize = "1";
				break;
			case 5:		// 23.98p
				dmVideoFrameRate = "23.98p";
				dmTimeFormat = "23976Timecode";
				dmTimeScale = "24000";	dmTimeSampleSize = "1001";
				break;
			case 6:		// 59.94p
				dmVideoFrameRate = "59.94p";
				dmTimeFormat = isDropFrame ? "5994DropTimecode" : "5994NonDropTimecode";
				dmTimeScale = "60000";	dmTimeSampleSize = "1001";
				break;
			case 7:		// 50p
				dmVideoFrameRate = "50p";
				dmTimeFormat = "50Timecode";
				dmTimeScale = "50";	dmTimeSampleSize = "1";
				break;
			case 8:		// 23.98p (pull down)
				dmVideoFrameRate = "59.94i";
				dmTimeFormat = isDropFrame ? "2997DropTimecode" : "2997NonDropTimecode";
				dmTimeScale = "30000";	dmTimeSampleSize = "1001";
				break;
		}

		if ( ! dmTimeFormat.empty() ) {

			xmpObj.SetProperty ( kXMP_NS_DM, "videoFrameRate", dmVideoFrameRate, kXMP_DeleteExisting );

			// Set the XMP start time.
			{
				const XMP_Uns8 frameTens = ( startTimecode[0] >> 4 ) & 0x03;
				const XMP_Uns8 frameUnits = startTimecode[0] & 0x0f;
				const XMP_Uns8 secondTens = ( startTimecode[1] >> 4 ) & 0x07;
				const XMP_Uns8 secondUnits = startTimecode[1] & 0x0f;
				const XMP_Uns8 minuteTens = ( startTimecode[2] >> 4 ) & 0x07;
				const XMP_Uns8 minuteUnits = startTimecode[2] & 0x0f;
				const XMP_Uns8 hourTens = ( startTimecode[3] >> 4 ) & 0x03;
				const XMP_Uns8 hourUnits = startTimecode[3] & 0x0f;
				const char tcSeparator = isDropFrame ? ';' : ':';
				char timecodeBuff [12];

				sprintf_safe ( timecodeBuff, sizeof(timecodeBuff), "%d%d%c%d%d%c%d%d%c%d%d", hourTens, hourUnits, tcSeparator,
					minuteTens, minuteUnits, tcSeparator, secondTens, secondUnits, tcSeparator, frameTens, frameUnits);
				xmpObj.SetProperty( kXMP_NS_DM, "startTimeScale", dmTimeScale, kXMP_DeleteExisting );
				xmpObj.SetProperty( kXMP_NS_DM, "startTimeSampleSize", dmTimeSampleSize, kXMP_DeleteExisting );
				xmpObj.SetStructField ( kXMP_NS_DM, "startTimecode", kXMP_NS_DM, "timeValue", timecodeBuff, 0 );
				xmpObj.SetStructField ( kXMP_NS_DM, "startTimecode", kXMP_NS_DM, "timeFormat", dmTimeFormat, 0 );
			}

			// Set the XMP duration.
			{
				char frameCountBuff [11];
				const std::string dmTimeScaleRatio ( dmTimeSampleSize + "/" + dmTimeScale );

				sprintf_safe ( frameCountBuff, sizeof(frameCountBuff), "%d", frameCount );
				xmpObj.DeleteProperty ( kXMP_NS_DM, "duration" );
				xmpObj.SetStructField ( kXMP_NS_DM, "duration", kXMP_NS_DM, "value", frameCountBuff );
				xmpObj.SetStructField ( kXMP_NS_DM, "duration", kXMP_NS_DM, "scale", dmTimeScaleRatio );
			}

			containsXMP = true;

		}

	}

}

// =================================================================================================
// SetXMPAudioInfo
// ===============

static void SetXMPAudioInfo ( SXMPMeta& xmpObj, bool& containsXMP, const CIF_CodingInfo& cifCodingInfo )
{
	const XMP_Uns8 audioType = cifCodingInfo.mAudioTypeAndFrequency >> 4;
	const XMP_Uns8 audioFrequency = cifCodingInfo.mAudioTypeAndFrequency & 0xF;
	const XMP_Uns8 audioChannel = cifCodingInfo.mAudioChannel >> 4;

	if ( audioType == 1 ) {
		xmpObj.SetProperty( kXMP_NS_DM, "audioCompressor", "LPCM", kXMP_DeleteExisting );
		containsXMP = true;
	}

	if ( audioFrequency == 1 ) {
		xmpObj.SetProperty( kXMP_NS_DM, "audioSampleRate", "48000", kXMP_DeleteExisting );
		containsXMP = true;
	}

	if ( audioChannel == 1 ) {
		xmpObj.SetProperty( kXMP_NS_DM, "audioChannelType", "Stereo", kXMP_DeleteExisting );
		containsXMP = true;
	} else if ( audioChannel == 2 ) {
		xmpObj.SetProperty( kXMP_NS_DM, "audioChannelType", "4 Channel", kXMP_DeleteExisting );
		containsXMP = true;
	}
}

// =================================================================================================
// SetXMPRatesAndSizes
// ===================

static void SetXMPRatesAndSizes ( SXMPMeta& xmpObj, bool& containsXMP, const CIF_CodingInfo& cifCodingInfo,
								  XMP_Uns32 frameCount, const XMP_Uns8* startTimecode, XMP_Uns8 clipAttributes )
{
	SetXMPVideoRatesAndSizes ( xmpObj, containsXMP, cifCodingInfo, frameCount, startTimecode, clipAttributes );
	SetXMPAudioInfo ( xmpObj, containsXMP, cifCodingInfo );
}

// =================================================================================================
// SetXMPClipMetadata
// ==================

static void SetXMPFixedSpaceCIFField ( SXMPMeta& xmpObj, bool& containsXMP, const XMP_Uns8* cifField, int cifFieldLen,
									   XMP_StringPtr xmpNameSpace, XMP_StringPtr xmpPropName )
{
	XMP_Assert ( cifFieldLen >= 4 );

	if ( * reinterpret_cast<const XMP_Uns32*> ( cifField ) != 0xFFFFFFFF ) {

		// Skip any trailing spaces from the CIF field and copy it to a C string.
		int i = cifFieldLen - 1;

		for ( ; (i >= 0) && (cifField [i] == ' '); --i ) {	/* Nothing extra */ }

		if ( i >= 0 ) {
			const std::string strField ( reinterpret_cast<const char*> ( cifField ), ( i + 1 ) );
			xmpObj.SetProperty ( xmpNameSpace, xmpPropName, strField, kXMP_DeleteExisting );
			containsXMP = true;
		}

	}

}

// =================================================================================================
// SetXMPStartTimecode
// ===================

static void SetXMPStartTimecode ( SXMPMeta& xmpObj, bool& containsXMP, const CIF_RecordDateAndTime& inRecordDateAndTime )
{
	if ( ( * reinterpret_cast<const XMP_Uns32*> ( &inRecordDateAndTime ) == 0xFFFFFFFF ) ) return;

	const XMP_Uns8 daylightSavingsTime = ( inRecordDateAndTime.mTimeZoneInfo >> 6 ) & 0x01;
	const XMP_Uns8 timezoneSign = ( inRecordDateAndTime.mTimeZoneInfo >> 5 ) & 0x01;
	const XMP_Uns8 timezoneValue = ( inRecordDateAndTime.mTimeZoneInfo >> 1 ) & 0x0F;
	const XMP_Uns8 halfHourFlag = inRecordDateAndTime.mTimeZoneInfo & 0x01;
	int utcOffsetHours = 0;
	unsigned int utcOffsetMinutes = 0;

	// It's not entirely clear how to interpret the daylightSavingsTime flag from the documentation
	// -- my best guess is that it should only be used if trying to display local time, not the
	// UTC-relative time that XMP specifies.
	if ( timezoneValue != 0xF ) {
		utcOffsetHours = timezoneSign ? -timezoneValue : timezoneValue;
		utcOffsetMinutes = 30 * halfHourFlag;
	}

	char dateBuff [26];

	sprintf_safe ( dateBuff, sizeof(dateBuff),
			  "%01d%01d%01d%01d-%01d%01d-%01d%01dT%01d%01d:%01d%01d:%01d%01d%+02d:%02d",
			  (inRecordDateAndTime.mRecordDateYear_Hundreds >> 4), (inRecordDateAndTime.mRecordDateYear_Hundreds & 0x0F),
			  (inRecordDateAndTime.mRecordDateYear_Tens >> 4), (inRecordDateAndTime.mRecordDateYear_Tens & 0x0F),
			  (inRecordDateAndTime.mRecordDateMonth >> 4), (inRecordDateAndTime.mRecordDateMonth & 0x0F),
			  (inRecordDateAndTime.mRecordDateDay >> 4), (inRecordDateAndTime.mRecordDateDay & 0x0F),
			  (inRecordDateAndTime.mRecordDateHour >> 4), (inRecordDateAndTime.mRecordDateHour & 0x0F),
			  (inRecordDateAndTime.mRecordDateMinutes >> 4), (inRecordDateAndTime.mRecordDateMinutes & 0x0F),
			  (inRecordDateAndTime.mRecordDateSeconds >> 4), (inRecordDateAndTime.mRecordDateSeconds & 0x0F),
			  utcOffsetHours, utcOffsetMinutes );

	xmpObj.SetProperty ( kXMP_NS_XMP, "CreateDate", dateBuff, kXMP_DeleteExisting );
	containsXMP = true;

}

// =================================================================================================
// SetXMPFileDataRate
// ==================

static void SetXMPFileDataRate ( SXMPMeta& xmpObj, bool& containsXMP, XMP_Uns8 cifRecordingRate )
{
	std::string dmRecordingRate;

	// Note: dynamicmedia recording rate is expressed in MegaBytes per second, while the CIF data rate is in
	// MegaBits per second, so we must divide by 8.
	switch ( ( cifRecordingRate & 0x7F ) ) {
		case 1:	dmRecordingRate = "25/8";	break;
		case 2:	dmRecordingRate = "35/8";	break;
		case 3:	dmRecordingRate = "50/8";	break;
	}

	if ( ! dmRecordingRate.empty() ) {
		xmpObj.SetProperty ( kXMP_NS_DM, "fileDataRate", dmRecordingRate.c_str(), kXMP_DeleteExisting );
		containsXMP = true;
	}

}

// =================================================================================================
// SetXMPWhiteBalance
// ==================

static void SetXMPWhiteBalance ( SXMPMeta& xmpObj, bool& containsXMP, XMP_Uns8 cifWhiteBalance )
{
	const XMP_Uns8 cifWBMode = (cifWhiteBalance >> 5);
	std::string crsWhiteBalance;

	// Translate Canon white balance constants to their nearest Camera Raw equivalents. For known
	// preset values with no known translation, use "custom." The field is not set at all for completely
	// unknown white balance values.

	if ( cifWBMode == 0 ) {

		crsWhiteBalance = "Auto";

	} else if ( cifWBMode < 3 ) {

		crsWhiteBalance = "Custom";

	} else if ( cifWBMode == 3 ) {

		const XMP_Uns8 cifWBPreset = (cifWhiteBalance & 0x1F);

		switch ( cifWBPreset ) {
			case 1:		crsWhiteBalance = "Tungsten";		break;
			case 2:		crsWhiteBalance = "Fluorescent";	break;
			case 3:		crsWhiteBalance = "Fluorescent";	break;
			case 4:		crsWhiteBalance = "Daylight";		break;
			case 5:		crsWhiteBalance = "Cloudy";			break;
			case 6:		crsWhiteBalance = "Shade";			break;
			default:	crsWhiteBalance = "Custom";
		}

	}

	if ( ! crsWhiteBalance.empty() ) {
		xmpObj.SetProperty ( kXMP_NS_CameraRaw, "WhiteBalance", crsWhiteBalance.c_str(), kXMP_DeleteExisting );
		containsXMP = true;
	}
}

// =================================================================================================
// SetXMPGoodMark
// ==============

static void SetXMPGoodMark ( SXMPMeta& xmpObj, bool& containsXMP, XMP_Uns8 cifClipAttributes )
{
	const XMP_Uns8 cifOKMark = (cifClipAttributes >> 6);

	if ( cifOKMark == 1 ) {
		xmpObj.SetProperty_Bool ( kXMP_NS_DM, "good", true, kXMP_DeleteExisting );
		containsXMP = true;
	} else if ( cifOKMark == 2 ) {
		xmpObj.SetProperty_Bool ( kXMP_NS_DM, "good", false, kXMP_DeleteExisting );
		containsXMP = true;
	}
}

// =================================================================================================
// BytesToHex
// ==========

#define kHexDigits "0123456789ABCDEF"

static std::string BytesToHex ( const XMP_Uns8* inClipIDBytes, int inNumBytes )
{
	const int numChars = ( inNumBytes * 2 );
	std::string hexStr;

	hexStr.reserve(numChars);

	for ( int i = 0; i < inNumBytes; ++i ) {
		const XMP_Uns8 byte = inClipIDBytes[i];
		hexStr.push_back ( kHexDigits [byte >> 4] );
		hexStr.push_back ( kHexDigits [byte & 0xF] );
	}

	return hexStr;
}

// =================================================================================================
// SetXMPRelations
// ===============

static void SetXMPRelations ( SXMPMeta& xmpObj, bool& containsXMP, const CIF_RelayInformation& cifRelayInformation )
{
	std::string cifIDString = BytesToHex ( cifRelayInformation.mRecMediaID, 16 );

	xmpObj.SetProperty ( kXMP_NS_DC, "identifier", cifIDString.c_str(), kXMP_DeleteExisting );

	const bool hasPreviousMediaID = ( * reinterpret_cast<const XMP_Uns32*> ( cifRelayInformation.mRelayPreviousMediaID ) != 0xFFFFFFFF );
	const bool hasNextMediaID = ( * reinterpret_cast<const XMP_Uns32*> ( cifRelayInformation.mRelayNextMediaID ) != 0xFFFFFFFF );

	if ( hasPreviousMediaID | hasNextMediaID ) {

		xmpObj.DeleteProperty ( kXMP_NS_DC, "relation" );

		if ( hasPreviousMediaID ) {
			cifIDString = BytesToHex ( cifRelayInformation.mRelayPreviousMediaID, 16 );
			xmpObj.AppendArrayItem ( kXMP_NS_DC, "relation", kXMP_PropArrayIsUnordered, std::string ( "previousMediaID:" ) + cifIDString );
		}

		if ( cifRelayInformation.mRelayPreviousClipID [0] != 0xFF ) {
			cifIDString.assign ( reinterpret_cast<const char*> ( cifRelayInformation.mRelayPreviousClipID ), 6 );
			xmpObj.AppendArrayItem ( kXMP_NS_DC, "relation", kXMP_PropArrayIsUnordered, std::string ( "previousClipID:" ) + cifIDString );
		}

		if ( hasNextMediaID ) {
			cifIDString = BytesToHex ( cifRelayInformation.mRelayNextMediaID, 16 );
			xmpObj.AppendArrayItem ( kXMP_NS_DC, "relation", kXMP_PropArrayIsUnordered, std::string ( "nextMediaID:" ) + cifIDString );
		}

		if ( cifRelayInformation.mRelayNextClipID [0] != 0xFF ) {
			cifIDString.assign ( reinterpret_cast<const char*> ( cifRelayInformation.mRelayNextClipID ), 6 );
			xmpObj.AppendArrayItem ( kXMP_NS_DC, "relation", kXMP_PropArrayIsUnordered, std::string ( "nextClipID:" ) + cifIDString );
		}

	}

	containsXMP = true;
}

// =================================================================================================
// SetXMPFirmwareVersion
// =====================

static void SetXMPFirmwareVersion ( SXMPMeta& xmpObj, bool& containsXMP, const XMP_Uns8* cifFirmwareVersion )
{
	if ( * reinterpret_cast<const XMP_Uns32*> ( cifFirmwareVersion ) != 0xFFFFFFFF ) {
		std::string softwareStr = "Firmware v";

		softwareStr.append ( reinterpret_cast<const char*> ( cifFirmwareVersion ), 8 );
		xmpObj.SetProperty ( kXMP_NS_XMP, "CreatorTool", softwareStr.c_str(), kXMP_DeleteExisting );
		containsXMP = true;
	}
}

// =================================================================================================
// SetXMPClipMetadata
// ==================

static void SetXMPClipMetadata ( SXMPMeta& xmpObj, bool& containsXMP, const CIF_ClipMetadata& cifClipMetadata )
{
	SetXMPFixedSpaceCIFField ( xmpObj, containsXMP, cifClipMetadata.mMakerName, 32, kXMP_NS_TIFF, "Make" );
	SetXMPFixedSpaceCIFField ( xmpObj, containsXMP, cifClipMetadata.mModelName, 32, kXMP_NS_TIFF, "Model" );
	SetXMPFixedSpaceCIFField ( xmpObj, containsXMP, cifClipMetadata.mSerialNumber, 32, kXMP_NS_EXIF_Aux, "SerialNumber" );
	SetXMPFirmwareVersion ( xmpObj, containsXMP, cifClipMetadata.mFirmVersion );
	SetXMPStartTimecode ( xmpObj, containsXMP, cifClipMetadata.mRecordDataAndTime );
	SetXMPFileDataRate ( xmpObj, containsXMP, cifClipMetadata.mRecordingModeInformation.mRecordingRate );
	SetXMPWhiteBalance ( xmpObj, containsXMP, cifClipMetadata.mRecordingModeInformation.mWhiteBalanceInfo );
	SetXMPGoodMark ( xmpObj, containsXMP, cifClipMetadata.mClipAttributes.mAttributes );
	SetXMPRelations ( xmpObj, containsXMP, cifClipMetadata.mRelayInformation );
}

// =================================================================================================
// CanonXF_CheckFormat
// ===================
//
// This version does fairly simple checks. The top level folder (.../MyMovie) must a child folder
// called CONTENTS. This must have a subfolder called CLIPS001 which must contain a subfolder
// containing a .CIF file. The name of the .CIF file must match the name of the subfolder that
// contains it. The name checks are case insensitive.
//
// The state of the string parameters depends on the form of the path passed by the client. If the
// client passed a logical clip path, like ".../MyMovie/NZ0001", the parameters are:
//   rootPath   - ".../MyMovie"
//   gpName     - empty
//   parentName - empty
//   leafName   - "NZ0001"
// If the client passed a full file path, like ".../MyMovie/CONTENTS/CLIPS001/NZ0001/NZ000101.MXF", they are:
//   rootPath   - ".../MyMovie/CONTENTS"
//   gpName     - "CLIPS001"
//   parentName - "NZ0001"
//   leafName   - "NZ000101"	- The file extension has been removed from the leafname!
//
// For files that describe an entire clip (.CIF and .XML), the filename is the base clip name (e.g.
// "NZ0001.CIF"). For files that describe a single "stream" (.SIF and .MXF), the filename is the
// base clip name plus a two digit stream number (e.g. "NZ000101.SIF"). Note that in this context
// "stream" describes a sequence of audio and video that was recorded by the camera as a single
// file. If there's less than 2GB of data there should be one stream per clip. There should only be
// multiple "streams" for a single clip if it has more than 2GB of media, requiring the camera to
// split it across multiple files.

// ! The common code has shifted the gpName, parentName, and leafName strings to upper case. It has
// ! also made sure that for a logical clip path the rootPath is an existing folder, and that the
// ! file exists for a full file path. The file extension has been removed from the leafname.

bool CanonXF_CheckFormat ( XMP_FileFormat format,
					  const std::string & _rootPath,
					  const std::string & gpName,
					  const std::string & parentName,
					  const std::string & leafName,
					  XMPFiles * parent )
{
	std::string rootPath = _rootPath;
	std::string tempPath, childName;
	std::string clipName = leafName;

	// Do some basic checks on the gpName and parentName, extract the logical clip name.

	if ( gpName.empty() != parentName.empty() ) return false;	// Must be both empty or both non-empty.

	if ( ! gpName.empty() ) {
	
		// Make sure the rootPath ends in "/CONTENTS", the gpName is "CLIPS001", and that the
		// parentName is the prefix of the leafname. Use the leafName as the clipName to preserve
		// lower case, but truncate to the parent's length to remove any suffix.
	
		if ( gpName != "CLIPS001" ) return false;
		std::string greatGP;
		XIO::SplitLeafName ( &rootPath, &greatGP );
		MakeUpperCase ( &greatGP );
		if ( greatGP != "CONTENTS" ) return false;

		if ( ! XMP_LitNMatch ( parentName.c_str(), clipName.c_str(), parentName.size() ) ) {
			std::string tempName = clipName;
			MakeUpperCase ( &tempName );
			if ( ! XMP_LitNMatch ( parentName.c_str(), tempName.c_str(), parentName.size() ) ) return false;
		}
		
		clipName.erase ( parentName.size() );
	
	}

	tempPath = rootPath;
	tempPath += kDirChar;
	tempPath += "CONTENTS";
	tempPath += kDirChar;
	tempPath += "CLIPS001";
	tempPath += kDirChar;
	tempPath += clipName;
	if ( Host_IO::GetFileMode ( tempPath.c_str() ) != Host_IO::kFMode_IsFolder ) return false;

	// Make sure the clip's .CIF file exists.

	InternalMakeClipFilePath ( &tempPath, rootPath, clipName, ".CIF" );
	if ( Host_IO::GetFileMode ( tempPath.c_str() ) != Host_IO::kFMode_IsFile ) return false;

	// Make a bogus path to pass the root path and clip name to the handler. A bit of a hack, but
	// the only way to get info from here to there.

	tempPath = rootPath;
	tempPath += kDirChar;
	tempPath += clipName;

	size_t pathLen = tempPath.size() + 1;	// Include a terminating nul.
	parent->tempPtr = malloc ( pathLen );
	if ( parent->tempPtr == 0 ) XMP_Throw ( "No memory for Canon XF clip path", kXMPErr_NoMemory );
	memcpy ( parent->tempPtr, tempPath.c_str(), pathLen );	// AUDIT: Safe, allocated above.

	return true;

}	// CanonXF_CheckFormat

// =================================================================================================

static void* CreatePseudoClipPath ( const std::string & clientPath ) {

	// Used to create the clip pseudo path when the CheckFormat function is skipped.
	
	std::string pseudoPath = clientPath;

	size_t pathLen;
	void* tempPtr = 0;
	
	if ( Host_IO::Exists ( pseudoPath.c_str() ) ) {
	
		// The client passed a physical path. The logical clip name is the last folder name, the
		// parent of the file. This is best since some files have suffixes.
		
		std::string clipName, ignored;
		
		XIO::SplitLeafName ( &pseudoPath, &ignored );	// Ignore the file name.
		XIO::SplitLeafName ( &pseudoPath, &clipName );	// Use the parent folder name.

		XIO::SplitLeafName ( &pseudoPath, &ignored );	// Remove the 2 intermediate folder levels.
		XIO::SplitLeafName ( &pseudoPath, &ignored );
		
		pseudoPath += kDirChar;
		pseudoPath += clipName;
	
	}

	pathLen = pseudoPath.size() + 1;	// Include a terminating nul.
	tempPtr = malloc ( pathLen );
	if ( tempPtr == 0 ) XMP_Throw ( "No memory for Canon XF clip info", kXMPErr_NoMemory );
	memcpy ( tempPtr, pseudoPath.c_str(), pathLen );
	
	return tempPtr;

}	// CreatePseudoClipPath

// =================================================================================================
// CanonXF_MetaHandlerCTor
// =======================

XMPFileHandler * CanonXF_MetaHandlerCTor ( XMPFiles * parent )
{
	return new CanonXF_MetaHandler ( parent );

}	// CanonXF_MetaHandlerCTor

// =================================================================================================
// CanonXF_MetaHandler::CanonXF_MetaHandler
// ========================================

CanonXF_MetaHandler::CanonXF_MetaHandler ( XMPFiles * _parent ) : expat(0), videoClipElement(0)
{

	this->parent = _parent;	// Inherited, can't set in the prefix.
	this->handlerFlags = kCanonXF_HandlerFlags;
	this->stdCharForm  = kXMP_Char8Bit;

	// Extract the root path and clip name from tempPtr.

	if ( this->parent->tempPtr == 0 ) {
		// The CheckFormat call might have been skipped.
		this->parent->tempPtr = CreatePseudoClipPath ( this->parent->GetFilePath() );
	}

	this->rootPath.assign ( (char*) this->parent->tempPtr );
	free ( this->parent->tempPtr );
	this->parent->tempPtr = 0;

	XIO::SplitLeafName ( &this->rootPath, &this->clipName );

}	// CanonXF_MetaHandler::CanonXF_MetaHandler

// =================================================================================================
// CanonXF_MetaHandler::~CanonXF_MetaHandler
// =========================================

CanonXF_MetaHandler::~CanonXF_MetaHandler()
{

	this->CleanupLegacyXML();
	if ( this->parent->tempPtr != 0 ) {
		free ( this->parent->tempPtr );
		this->parent->tempPtr = 0;
	}

}	// CanonXF_MetaHandler::~CanonXF_MetaHandler

// =================================================================================================
// CanonXF_MetaHandler::MakeClipFilePath
// =====================================

bool CanonXF_MetaHandler::MakeClipFilePath ( std::string * path, XMP_StringPtr suffix, bool checkFile /* = false */ )
{

	InternalMakeClipFilePath ( path, this->rootPath, this->clipName, suffix );
	if ( ! checkFile ) return true;
	
	return Host_IO::Exists ( path->c_str() );

}	// CanonXF_MetaHandler::MakeClipFilePath

// =================================================================================================
// CanonXF_MetaHandler::CleanupLegacyXML
// =====================================

void CanonXF_MetaHandler::CleanupLegacyXML()
{

	if ( this->expat != 0 ) { delete ( this->expat ); this->expat = 0; }

	videoClipElement = 0;	// ! Was a pointer into the expat tree.

}	// CanonXF_MetaHandler::CleanupLegacyXML

// =================================================================================================
// CanonXF_MetaHandler::ReadClipXMLFile
// ====================================

bool CanonXF_MetaHandler::ReadClipXMLFile ( const std::string& xmlPath, bool digestFound )
{

	Host_IO::FileRef hostRef = Host_IO::Open ( xmlPath.c_str(), Host_IO::openReadOnly );
	if ( hostRef == Host_IO::noFileRef ) return false;	// The open failed.
	XMPFiles_IO xmlFile ( hostRef, xmlPath.c_str(), Host_IO::openReadOnly );

	XMP_Uns8 buffer [64*1024];
	while ( true ) {
		XMP_Int32 ioCount = xmlFile.Read ( buffer, sizeof(buffer) );
		if ( ioCount == 0 ) break;
		this->expat->ParseBuffer ( buffer, ioCount, false /* not the end */ );
	}
	this->expat->ParseBuffer ( 0, 0, true );	// End the parse.

	xmlFile.Close();

	// The root element should be VideoClip in the namespace http://www.canon.com/ns/VideoClip.

	XML_Node & xmlTree = this->expat->tree;
	XML_NodePtr rootElem = 0;

	for ( size_t i = 0, limit = xmlTree.content.size(); i < limit; ++i ) {
		if ( xmlTree.content[i]->kind == kElemNode ) {
			rootElem = xmlTree.content[i];
		}
	}

	if ( rootElem == 0 ) return false;
	XMP_StringPtr rootLocalName = rootElem->name.c_str() + rootElem->nsPrefixLen;
	if ( ! XMP_LitMatch ( rootLocalName, "VideoClip" ) ) return false;

	this->canonXFNS = rootElem->ns;
	if ( ! XMP_LitMatch ( this->canonXFNS.c_str(), "http://www.canon.com/ns/VideoClip" ) ) return false;

	XMP_StringPtr pCanonXFNS = this->canonXFNS.c_str();

	this->videoClipElement = rootElem;	// ! Save the legacy VideoClip XML element pointer for other use.

	// If we get here we need find and import the actual legacy elements using the current namespace.
	// Either there is no old digest in the XMP, or the digests differ. In the former case keep any
	// existing XMP, in the latter case take new legacy values.
	this->SetXMPPropertyFromLegacyXML ( digestFound, this->videoClipElement, kXMP_NS_DM, "shotName", "Title", false );

	XML_NodePtr legacyProp = 0;

	if ( digestFound || (! this->xmpObj.DoesPropertyExist ( kXMP_NS_DC, "creator" )) ) {
		legacyProp = this->videoClipElement->GetNamedElement ( pCanonXFNS, "Creator" );
		if ( (legacyProp != 0) && legacyProp->IsLeafContentNode() ) {
			this->xmpObj.DeleteProperty ( kXMP_NS_DC, "creator" );
			this->xmpObj.AppendArrayItem ( kXMP_NS_DC, "creator", kXMP_PropArrayIsOrdered,
										   legacyProp->GetLeafContentValue() );
			this->containsXMP = true;
		}
	}

	this->SetXMPPropertyFromLegacyXML ( digestFound, this->videoClipElement, kXMP_NS_DM, "comment", "Description", false );

	XML_Node * deviceElement = this->videoClipElement->GetNamedElement ( pCanonXFNS, "Device" );

	if ( deviceElement != 0 ) {
		this->SetXMPPropertyFromLegacyXML ( digestFound, deviceElement, kXMP_NS_TIFF, "Make", "Manufacturer", false );
		this->SetXMPPropertyFromLegacyXML ( digestFound, deviceElement, kXMP_NS_EXIF_Aux, "SerialNumber", "SerialNo", false );
		this->SetXMPPropertyFromLegacyXML ( digestFound, deviceElement, kXMP_NS_TIFF, "Model", "ModelName", false );

		if ( digestFound || (! this->xmpObj.DoesPropertyExist ( kXMP_NS_XMP, "CreatorTool" )) ) {

			XMP_StringPtr canonXFNSptr = this->canonXFNS.c_str();
			XML_NodePtr pLegacyProp = deviceElement->GetNamedElement ( canonXFNSptr, "FirmVersion" );

			if ( (pLegacyProp != 0) && pLegacyProp->IsLeafContentNode() ) {
				XMP_StringPtr legacyValue = pLegacyProp->GetLeafContentValue();

				if ( ( legacyValue != 0 ) &&
					 ( ( *legacyValue != 0 ) || (! this->xmpObj.DoesPropertyExist ( kXMP_NS_XMP, "CreatorTool" )) )) {
						std::string xmpFirmwareString ( std::string ( "Firmware v" ) + std::string ( legacyValue ) );

						this->xmpObj.SetProperty ( kXMP_NS_XMP, "CreatorTool", xmpFirmwareString, kXMP_DeleteExisting );
						this->containsXMP = true;
				}
			}
		}
	}

	XML_Node * locationElement = this->videoClipElement->GetNamedElement ( pCanonXFNS, "Location" );

	if ( locationElement != 0 ) {
		this->SetAltitudeFromLegacyXML ( locationElement, digestFound );
		this->SetXMPPropertyFromLegacyXML ( digestFound, locationElement, kXMP_NS_DM, "shotLocation", "PlaceName", false );
		this->SetGPSPropertyFromLegacyXML ( locationElement, digestFound, "GPSLongitude", "Longitude" );
		this->SetGPSPropertyFromLegacyXML ( locationElement, digestFound, "GPSLatitude", "Latitude" );
		this->SetXMPPropertyFromLegacyXML ( digestFound, locationElement, kXMP_NS_EXIF, "GPSProcessingMethod", "Source", false );
	}

	return true;

}	// CanonXF_MetaHandler::ReadClipXMLFile


// =================================================================================================
// CanonXF_MetaHandler::ReadClipInfoFile
// =====================================

bool CanonXF_MetaHandler::ReadClipInfoFile ( bool digestFound )
{

	// Canon's .CIF file is a binary file with fixed-size blocks. See Annex D.2 of the
	// "Content Specification for File Based Canon Video Camera" for details. the document is currently
	// marked "Canon Confidential" and should not be shared outside of Adobe.
	std::string strClipInfoPath;
	CIF_LegacyMetadata cifLegacyData;
	this->MakeClipFilePath ( &strClipInfoPath, ".CIF" );

	// Read the .CIF contents into in-memory structures
	if ( ! ReadCanonXFLegacyClipFile ( strClipInfoPath, cifLegacyData ) ) return false;

	// Convert from the .CIF structures into XML
	this->xmpObj.SetLocalizedText ( kXMP_NS_DC, "title", "", "x-default", this->clipName.c_str(), kXMP_DeleteExisting );
	this->xmpObj.SetProperty ( kXMP_NS_DM, "shotName", this->clipName.c_str(), kXMP_DeleteExisting );
	SetXMPRatesAndSizes ( this->xmpObj, this->containsXMP, cifLegacyData.mCodingInfo,
						  cifLegacyData.mNumberOfFramesInClip, cifLegacyData.mClipMetadata.mStartTimecode,
						  cifLegacyData.mClipMetadata.mClipAttributes.mAttributes );
	SetXMPClipMetadata ( this->xmpObj, this->containsXMP, cifLegacyData.mClipMetadata );

	return true;
}

// =================================================================================================
// CanonXF_MetaHandler::SetXMPPropertyFromLegacyXML
// ================================================

void CanonXF_MetaHandler::SetXMPPropertyFromLegacyXML ( bool digestFound,
												   XML_NodePtr legacyContext,
												   XMP_StringPtr schemaNS,
												   XMP_StringPtr propName,
												   XMP_StringPtr legacyPropName,
												   bool isLocalized )
{

	// Note: values in Canon XF XML files override existing values set in the CIF file as long as they are non-empty.
	XMP_StringPtr pCanonXFNS = this->canonXFNS.c_str();
	XML_NodePtr legacyProp = legacyContext->GetNamedElement ( pCanonXFNS, legacyPropName );

	if ( (legacyProp != 0) && legacyProp->IsLeafContentNode() ) {
		XMP_StringPtr legacyValue = legacyProp->GetLeafContentValue();

		if ( ( legacyValue != 0 ) &&
			 ( ( *legacyValue != 0 ) || (! this->xmpObj.DoesPropertyExist ( schemaNS, propName )) )) {
			if ( isLocalized ) {
				this->xmpObj.SetLocalizedText ( schemaNS, propName, "", "x-default", legacyValue, kXMP_DeleteExisting );
			} else {
				this->xmpObj.SetProperty ( schemaNS, propName, legacyValue, kXMP_DeleteExisting );
			}
			this->containsXMP = true;
		}
	}

}	// CanonXF_MetaHandler::SetXMPPropertyFromLegacyXML

// =================================================================================================
// CanonXF_MetaHandler::SetGPSPropertyFromLegacyXML
// ================================================

void CanonXF_MetaHandler::SetGPSPropertyFromLegacyXML  ( XML_NodePtr legacyLocationContext, bool digestFound, XMP_StringPtr propName, XMP_StringPtr legacyPropName )
{

	if ( digestFound || (! this->xmpObj.DoesPropertyExist ( kXMP_NS_EXIF, propName )) ) {

		XMP_StringPtr pCanonXFNS = this->canonXFNS.c_str();
		XML_NodePtr legacyGPSProp = legacyLocationContext->GetNamedElement ( pCanonXFNS, legacyPropName );

		if ( ( legacyGPSProp != 0 ) && legacyGPSProp->IsLeafContentNode() ) {

			this->xmpObj.DeleteProperty ( kXMP_NS_EXIF, propName );

			const std::string legacyGPSValue = legacyGPSProp->GetLeafContentValue();

			if ( ! legacyGPSValue.empty() ) {

				//	Convert from decimal to sexagesimal GPS coordinates
				char direction = '\0';
				double degrees = 0.0;
				const int numFieldsRead = sscanf ( legacyGPSValue.c_str(), "%c%lf", &direction, &degrees );

				if ( numFieldsRead == 2 ) {
					double wholeDegrees = 0.0;
					const double fractionalDegrees = modf ( degrees, &wholeDegrees );
					const double minutes = fractionalDegrees * 60.0;
					char xmpValue [128];

					sprintf_safe ( xmpValue, sizeof(xmpValue), "%d,%.5lf%c", static_cast<int>(wholeDegrees), minutes, direction );
					this->xmpObj.SetProperty ( kXMP_NS_EXIF, propName, xmpValue );
					this->containsXMP = true;

				}

			}

		}

	}

}	// CanonXF_MetaHandler::SetGPSPropertyFromLegacyXML

// =================================================================================================
// CanonXF_MetaHandler::SetAltitudeFromLegacyXML
// =============================================

void CanonXF_MetaHandler::SetAltitudeFromLegacyXML  ( XML_NodePtr legacyLocationContext, bool digestFound )
{

	if ( digestFound || (! this->xmpObj.DoesPropertyExist ( kXMP_NS_EXIF, "GPSAltitude" )) ) {

		XMP_StringPtr pCanonXFNS = this->canonXFNS.c_str();
		XML_NodePtr legacyAltitudeProp = legacyLocationContext->GetNamedElement ( pCanonXFNS, "Altitude" );

		if ( ( legacyAltitudeProp != 0 ) && legacyAltitudeProp->IsLeafContentNode() ) {

			this->xmpObj.DeleteProperty ( kXMP_NS_EXIF, "GPSAltitude" );

			const std::string legacyGPSValue = legacyAltitudeProp->GetLeafContentValue();

			if ( ! legacyGPSValue.empty() ) {

				int altitude = 0;

				if ( sscanf ( legacyGPSValue.c_str(), "%d", &altitude ) == 1) {

					if ( altitude >= 0 ) {
						// At or above sea level.
						this->xmpObj.SetProperty ( kXMP_NS_EXIF, "GPSAltitudeRef", "0" );
					} else {
						// Below sea level.
						altitude = -altitude;
						this->xmpObj.SetProperty ( kXMP_NS_EXIF, "GPSAltitudeRef", "1" );
					}

					char xmpValue [128];

					sprintf_safe ( xmpValue, sizeof(xmpValue), "%d/1", altitude );
					this->xmpObj.SetProperty ( kXMP_NS_EXIF, "GPSAltitude", xmpValue );
					this->containsXMP = true;

				}

			}

		}

	}

}	// CanonXF_MetaHandler::SetAltitudeFromLegacyXML

// =================================================================================================
// CanonXF_MetaHandler::ForceChildElement
// =======================================

XML_Node * CanonXF_MetaHandler::ForceChildElement ( XML_Node * parent, XMP_StringPtr localName, int indent /* = 0 */ )
{
	XML_Node * wsNode;
	XML_Node * childNode = parent->GetNamedElement ( this->canonXFNS.c_str(), localName );

	if ( childNode == 0 ) {

		// The indenting is a hack, assuming existing 2 spaces per level.

		wsNode = new XML_Node ( parent, "", kCDataNode );
		wsNode->value = "  ";	// Add 2 spaces to the existing WS before the parent's close tag.
		parent->content.push_back ( wsNode );

		childNode = new XML_Node ( parent, localName, kElemNode );
		childNode->ns = parent->ns;
		childNode->nsPrefixLen = parent->nsPrefixLen;
		childNode->name.insert ( 0, parent->name, 0, parent->nsPrefixLen );
		parent->content.push_back ( childNode );

		wsNode = new XML_Node ( parent, "", kCDataNode );
		wsNode->value = '\n';
		for ( ; indent > 1; --indent ) wsNode->value += "  ";	// Indent less 1, to "outdent" the parent's close.
		parent->content.push_back ( wsNode );

	}

	return childNode;

}	// CanonXF_MetaHandler::ForceChildElement

// =================================================================================================
// CanonXF_MetaHandler::MakeLegacyDigest
// =====================================

#define kHexDigits "0123456789ABCDEF"

void CanonXF_MetaHandler::MakeLegacyDigest ( std::string * digestStr )
{
	// Very simple digest -- just concatenate the .XML and .CIF files, both of which
	// are generally only a few kb in size.
	std::vector<XMP_Uns8> legacyBuff;

	try {

		{
			std::string strClipXMLPath;
			this->MakeClipFilePath ( &strClipXMLPath, ".XML" );

			Host_IO::FileRef hostRef = Host_IO::Open ( strClipXMLPath.c_str(), Host_IO::openReadOnly );
			if ( hostRef == Host_IO::noFileRef ) return;	// The open failed.
			XMPFiles_IO clipXMLFile ( hostRef, strClipXMLPath.c_str(), Host_IO::openReadOnly );

			// Read at most the first 1k of data from the XML file to use in the digest
			// (every xml file I've seen is less than 1k).
			const XMP_Int64 clipXMLLen = clipXMLFile.Length();
			const XMP_Int64 buffLen = (clipXMLLen <= 2048) ? clipXMLLen : 2048;

			legacyBuff.resize ( static_cast<unsigned int> ( buffLen ) );
			clipXMLFile.Read ( &(legacyBuff[0]), static_cast<XMP_Int32> ( buffLen ) );
			clipXMLFile.Close();
		}

		{
			std::string strClipInfoPath;
			this->MakeClipFilePath ( &strClipInfoPath, ".CIF" );

			Host_IO::FileRef hostRef = Host_IO::Open ( strClipInfoPath.c_str(), Host_IO::openReadOnly );
			if ( hostRef == Host_IO::noFileRef ) return;	// The open failed.
			XMPFiles_IO clipInfoFile ( hostRef, strClipInfoPath.c_str(), Host_IO::openReadOnly );

			// Read at most the first 4k of data from the CIF file to use in the digest
			// (every CIF file I've seen is less than 3k).
			const XMP_Int64 clipInfoLen = clipInfoFile.Length();
			const XMP_Int64 buffLen = (clipInfoLen <= 2048) ? clipInfoLen : 2048;
			const XMP_Int64 clipBuffLen = legacyBuff.size();

			legacyBuff.resize ( (unsigned int) (clipBuffLen + buffLen) );
			clipInfoFile.Read ( &( legacyBuff [(unsigned int)clipBuffLen] ), (XMP_Int32)buffLen );
			clipInfoFile.Close();
		}

	} catch (...) {

		return;

	}

	MD5_CTX context;
	unsigned char digestBin [16];

	MD5Init ( &context );
	MD5Update ( &context, (XMP_Uns8*)&(legacyBuff[0]), (unsigned int) legacyBuff.size() );
	MD5Final ( digestBin, &context );

	*digestStr = BytesToHex ( digestBin, 16 );

}	// CanonXF_MetaHandler::MakeLegacyDigest

// =================================================================================================
// CanonXF_MetaHandler::GetFileModDate
// ===================================

static inline bool operator< ( const XMP_DateTime & left, const XMP_DateTime & right ) {
	int compare = SXMPUtils::CompareDateTime ( left, right );
	return (compare < 0);
}

bool CanonXF_MetaHandler::GetFileModDate ( XMP_DateTime * modDate )
{

	// The CanonXF locations of metadata:
	//	CONTENTS/
	//		CLIPS001/
	//			INDEX.MIF
	//			RE0001/
	//				RE0001.CIF	// Has non-XMP metadata.
	//				RE0001.XML	// Has non-XMP metadata.
	//				RE0001.XMP
	//				RE000101.MXF
	//				RE000101.SIF

	bool ok, haveDate = false;
	std::string fullPath;
	XMP_DateTime oneDate, junkDate;
	if ( modDate == 0 ) modDate = &junkDate;

	ok = this->MakeClipFilePath ( &fullPath, ".CIF", true /* checkFile */ );
	if ( ok ) ok = Host_IO::GetModifyDate ( fullPath.c_str(), &oneDate );
	if ( ok ) {
		if ( *modDate < oneDate ) *modDate = oneDate;
		haveDate = true;
	}

	ok = this->MakeClipFilePath ( &fullPath, ".XML", true /* checkFile */ );
	if ( ok ) ok = Host_IO::GetModifyDate ( fullPath.c_str(), &oneDate );
	if ( ok ) {
		if ( (! haveDate) || (*modDate < oneDate) ) *modDate = oneDate;
		haveDate = true;
	}

	ok = this->MakeClipFilePath ( &fullPath, ".XMP", true /* checkFile */ );
	if ( ok ) ok = Host_IO::GetModifyDate ( fullPath.c_str(), &oneDate );
	if ( ok ) {
		if ( (! haveDate) || (*modDate < oneDate) ) *modDate = oneDate;
		haveDate = true;
	}

	return haveDate;

}	// CanonXF_MetaHandler::GetFileModDate

// =================================================================================================
// CanonXF_MetaHandler::FillMetadataFiles
// ======================================
void CanonXF_MetaHandler::FillMetadataFiles ( std::vector<std::string> * metadataFiles )
{
	std::string noExtPath, filePath;

	noExtPath = rootPath + kDirChar + "CONTENTS" + kDirChar + "CLIPS001" +
						   kDirChar + clipName + kDirChar + clipName;

	filePath = noExtPath + ".XMP";
	metadataFiles->push_back ( filePath );
	filePath = noExtPath + ".CIF";
	metadataFiles->push_back ( filePath );
	filePath = noExtPath + ".XML";
	metadataFiles->push_back ( filePath );

}	// 	FillMetadataFiles_CanonXF

// =================================================================================================
// CanonXF_MetaHandler::IsMetadataWritable
// =======================================

bool CanonXF_MetaHandler::IsMetadataWritable ( )
{
	std::vector<std::string> metadataFiles;
	FillMetadataFiles(&metadataFiles);
	std::vector<std::string>::iterator itr = metadataFiles.begin();
	// Check whether sidecar is writable, if not then check if it can be created.
	bool xmpWritable = Host_IO::Writable( itr->c_str(), true );
	// Check if legacy metadata file is writable.
	bool xmlWritable = Host_IO::Writable( (itr+2)->c_str(), false );
	return ( xmlWritable && xmpWritable );
}// CanonXF_MetaHandler::IsMetadataWritable

// =================================================================================================
// CanonXF_MetaHandler::FillAssociatedResources
// ======================================
void CanonXF_MetaHandler::FillAssociatedResources ( std::vector<std::string> * resourceList )
{
	/// The possible associated resources:
	/// .../MyMovie
	/// 	CONTENTS/
	/// 	    CLIPS001/
	/// 	        INDEX.MIF			NNXXXX -> Clip Name
	/// 	        NNXXXX/             NN     -> Alphanumeric two characters
	/// 	            NNXXXX.CIF		XXXX   -> Numeric Four Characters
	/// 	            NNXXXX.XML		SS     -> Stream Id in the range 01 to 99
	/// 	            NNXXXXSS.MXF
	/// 	            NNXXXXSS.SIF
	/// 	            NNXXXX.CPF
	/// 	            NNXXXX.THM

	//
	XMP_VarString clips01Path = rootPath + kDirChar + "CONTENTS" + kDirChar + "CLIPS001" + kDirChar ;
	XMP_VarString path;
	
	//Add RootPath
	path = rootPath + kDirChar;
	PackageFormat_Support::AddResourceIfExists( resourceList, path );

	// Add existing files under the folder "CLIPS001"
	path = clips01Path + "INDEX.MIF";
	PackageFormat_Support::AddResourceIfExists ( resourceList, path ) ;

	path = clips01Path + this->clipName + kDirChar + this->clipName  + ".CIF";
	PackageFormat_Support::AddResourceIfExists ( resourceList, path ) ;
		
	path = clips01Path + this->clipName + kDirChar + this->clipName  + ".XML";
	PackageFormat_Support::AddResourceIfExists ( resourceList, path ) ;

	path = clips01Path + this->clipName + kDirChar + this->clipName  + ".XMP";
	PackageFormat_Support::AddResourceIfExists ( resourceList, path ) ;

	path = clips01Path + this->clipName + kDirChar + this->clipName  + ".CPF";
	PackageFormat_Support::AddResourceIfExists ( resourceList, path ) ;

	path = clips01Path + this->clipName + kDirChar + this->clipName  + ".THM";
	PackageFormat_Support::AddResourceIfExists ( resourceList, path ) ;

	path = clips01Path + this->clipName + kDirChar;
	XMP_VarString regExp;
	XMP_StringVector regExpVec;
	regExp = "^" + this->clipName + "\\d\\d.MXF$";
	regExpVec.push_back ( regExp );
	regExp = "^" + this->clipName + "\\d\\d.SIF$";
	regExpVec.push_back ( regExp );
	IOUtils::GetMatchingChildren ( *resourceList,  path, regExpVec, false, true, true );
}

// =================================================================================================
// CanonXF_MetaHandler::CacheFileData
// ==================================

void CanonXF_MetaHandler::CacheFileData()
{
	XMP_Assert ( ! this->containsXMP );

	if ( this->parent->UsesClientIO() ) {
		XMP_Throw ( "Canon XF cannot be used with client-managed I/O", kXMPErr_InternalFailure );
	}

	// Make sure the clip's .XMP file exists.

	std::string xmpPath;
	this->MakeClipFilePath ( &xmpPath, ".XMP" );
	if ( ! Host_IO::Exists ( xmpPath.c_str() ) ) return;	// No XMP.

	// Read the entire .XMP file. We know the XMP exists, New_XMPFiles_IO is supposed to return 0
	// only if the file does not exist.

	bool readOnly = XMP_OptionIsClear ( this->parent->openFlags, kXMPFiles_OpenForUpdate );

	XMP_Assert ( this->parent->ioRef == 0 );
	XMPFiles_IO* xmpFile =  XMPFiles_IO::New_XMPFiles_IO ( xmpPath.c_str(), readOnly );
	if ( xmpFile == 0 ) XMP_Throw ( "Canon XF XMP file open failure", kXMPErr_InternalFailure );
	this->parent->ioRef = xmpFile;

	XMP_Int64 xmpLen = xmpFile->Length();
	if ( xmpLen > 100*1024*1024 ) {
		XMP_Throw ( "Canon XF XMP is outrageously large", kXMPErr_InternalFailure );	// Sanity check.
	}

	this->xmpPacket.erase();
	this->xmpPacket.reserve ( (size_t)xmpLen );
	this->xmpPacket.append ( (size_t)xmpLen, ' ' );

	xmpFile->ReadAll ( (void*)this->xmpPacket.data(), (XMP_Int32)xmpLen );

	this->packetInfo.offset = 0;
	this->packetInfo.length = (XMP_Int32)xmpLen;
	FillPacketInfo ( this->xmpPacket, &this->packetInfo );

	this->containsXMP = true;

}	// CanonXF_MetaHandler::CacheFileData

// =================================================================================================
// CanonXF_MetaHandler::ProcessXMP
// ===============================

void CanonXF_MetaHandler::ProcessXMP()
{

	// Some versions of gcc can't tolerate goto's across declarations.
	// *** Better yet, avoid this cruft with self-cleaning objects.
	#define CleanupAndExit	\
		{																								\
			bool openForUpdate = XMP_OptionIsSet ( this->parent->openFlags, kXMPFiles_OpenForUpdate );	\
			if ( ! openForUpdate ) this->CleanupLegacyXML();											\
			return;																						\
		}

	if ( this->processedXMP ) return;
	this->processedXMP = true;	// Make sure only called once.

	if ( this->containsXMP ) {
		this->xmpObj.ParseFromBuffer ( this->xmpPacket.c_str(), (XMP_StringLen)this->xmpPacket.size() );
	}

	bool digestFound = false;

	{
		std::string oldDigest, newDigest;
		digestFound = this->xmpObj.GetStructField ( kXMP_NS_XMP, "NativeDigests", kXMP_NS_XMP, "CanonXF", &oldDigest, 0 );
		if ( digestFound ) {
			this->MakeLegacyDigest ( &newDigest );
			if ( oldDigest == newDigest ) CleanupAndExit
		}
	}

	// Read the contents of the .CIF file first since certain fields will be overridden if they also appear
	// in the .XML file and are non-empty.
	if ( ! this->ReadClipInfoFile ( digestFound ) ) CleanupAndExit

	// Create an expat adapter for reading and writing the Canon XML file
	this->expat = XMP_NewExpatAdapter ( ExpatAdapter::kUseLocalNamespaces );
	if ( this->expat == 0 ) XMP_Throw ( "CanonXF_MetaHandler: Can't create Expat adapter", kXMPErr_NoMemory );

	// Now read the contents of the .XML file, if present, which supplements and may override fields from the .CIF file.
	{
		std::string xmlPath;
		this->MakeClipFilePath ( &xmlPath, ".XML" );

		if ( Host_IO::GetFileMode ( xmlPath.c_str() ) == Host_IO::kFMode_IsFile ) {
			if ( ! this->ReadClipXMLFile ( xmlPath, digestFound ) ) CleanupAndExit
		}
	}

	CleanupAndExit
	#undef CleanupAndExit

}	// CanonXF_MetaHandler::ProcessXMP

// =================================================================================================
// CanonXF_MetaHandler::UpdateFile
// ===============================
//
// Note that UpdateFile is only called from XMPFiles::CloseFile, so it is OK to close the file here.

void CanonXF_MetaHandler::UpdateFile ( bool doSafeUpdate )
{
	if ( ! this->needsUpdate ) return;
	this->needsUpdate = false;	// Make sure only called once.

	XMP_Assert ( this->parent->UsesLocalIO() );

	// Update the internal legacy XML tree if we have one, and set the digest in the XMP.

	bool updateLegacyXML = false;

	if ( this->videoClipElement != 0 ) {

		XMP_Assert ( this->expat != 0 );

		bool xmpFound;
		std::string xmpValue;
		XML_Node * xmlNode;

		try {

			xmpFound = this->xmpObj.GetProperty( kXMP_NS_DM, "shotName", &xmpValue, 0 );

			if ( xmpFound ) {

				xmlNode = this->ForceChildElement ( this->videoClipElement, "Title", 3 );

				if ( xmpValue != xmlNode->GetLeafContentValue() ) {
					xmlNode->SetLeafContentValue ( xmpValue.c_str() );
					updateLegacyXML = true;
				}

			}

			xmpFound = this->xmpObj.GetArrayItem ( kXMP_NS_DC, "creator", 1, &xmpValue, 0 );

			if ( xmpFound ) {
				xmlNode = this->ForceChildElement ( this->videoClipElement, "Creator", 3 );
				if ( xmpValue != xmlNode->GetLeafContentValue() ) {
					xmlNode->SetLeafContentValue ( xmpValue.c_str() );
					updateLegacyXML = true;
				}
			}

			xmpFound = this->xmpObj.GetProperty ( kXMP_NS_DM, "comment", &xmpValue, 0 );

			if ( xmpFound ) {
				xmlNode = this->ForceChildElement ( this->videoClipElement, "Description", 3 );
				if ( xmpValue != xmlNode->GetLeafContentValue() ) {
					xmlNode->SetLeafContentValue ( xmpValue.c_str() );
					updateLegacyXML = true;
				}
			}

			xmpFound = this->xmpObj.GetProperty ( kXMP_NS_DM, "shotLocation", &xmpValue, 0 );

			if ( xmpFound ) {
				xmlNode = this->ForceChildElement ( this->videoClipElement, "Location", 3 );
				xmlNode = this->ForceChildElement ( xmlNode, "PlaceName", 3 );
				if ( xmpValue != xmlNode->GetLeafContentValue() ) {
					xmlNode->SetLeafContentValue ( xmpValue.c_str() );
					updateLegacyXML = true;
				}
			}

		} catch (...) {

			// Ignore all exceptions.

		}

	}

	// --------------------------------------------
	// Now update the legacy XML file if necessary.

	if ( updateLegacyXML ) {

		std::string legacyXML, xmlPath;
		this->expat->tree.Serialize ( &legacyXML );
		this->MakeClipFilePath ( &xmlPath, ".XML" );

		bool haveXML = Host_IO::Exists ( xmlPath.c_str() );
		if ( ! haveXML ) Host_IO::Create ( xmlPath.c_str() );

		Host_IO::FileRef hostRef = Host_IO::Open ( xmlPath.c_str(), Host_IO::openReadWrite );
		if ( hostRef == Host_IO::noFileRef ) XMP_Throw ( "Failure opening Canon XF legacy XML file", kXMPErr_ExternalFailure );
		XMPFiles_IO origXML ( hostRef, xmlPath.c_str(), Host_IO::openReadWrite );
		XIO::ReplaceTextFile ( &origXML, legacyXML, (haveXML & doSafeUpdate) );
		origXML.Close();

	}

	std::string newDigest;
	this->MakeLegacyDigest ( &newDigest );
	this->xmpObj.SetStructField ( kXMP_NS_XMP, "NativeDigests", kXMP_NS_XMP, "CanonXF", newDigest.c_str(), kXMP_DeleteExisting );

	this->xmpObj.SerializeToBuffer ( &this->xmpPacket, this->GetSerializeOptions() );

	// -----------------------------------------------------------------------
	// Update the XMP file first, don't let legacy XML failures block the XMP.

	std::string xmpPath;
	this->MakeClipFilePath ( &xmpPath, ".XMP" );

	bool haveXMP = Host_IO::Exists ( xmpPath.c_str() );
	if ( ! haveXMP ) {
		XMP_Assert ( this->parent->ioRef == 0 );
		Host_IO::Create ( xmpPath.c_str() );
		this->parent->ioRef = XMPFiles_IO::New_XMPFiles_IO ( xmpPath.c_str(), Host_IO::openReadWrite );
		if ( this->parent->ioRef == 0 ) XMP_Throw ( "Failure opening Canon XF XMP file", kXMPErr_ExternalFailure );
	}

	XMP_IO* xmpFile = this->parent->ioRef;
	XMP_Assert ( xmpFile != 0 );
	XIO::ReplaceTextFile ( xmpFile, this->xmpPacket, (haveXMP & doSafeUpdate) );

	

}	// CanonXF_MetaHandler::UpdateFile

// =================================================================================================
// CanonXF_MetaHandler::WriteTempFile
// ==================================

void CanonXF_MetaHandler::WriteTempFile ( XMP_IO* tempRef )
{

	// ! WriteTempFile is not supposed to be called for handlers that own the file.
	XMP_Throw ( "CanonXF_MetaHandler::WriteTempFile should not be called", kXMPErr_InternalFailure );

}	// CanonXF_MetaHandler::WriteTempFile

// =================================================================================================

#endif	// AdobePrivate	// ! This entire handler is private, it must not go in the public SDK.
