// =================================================================================================
// Copyright Adobe
// Copyright 2015 Adobe
// All Rights Reserved
//
// NOTICE: Adobe permits you to use, modify, and distribute this file in accordance with the terms
// of the Adobe license agreement accompanying it. If you have received this file from a source other 
// than Adobe, then your use, modification, or distribution of it requires the prior written permission
// of Adobe.
// =================================================================================================

#include "public/include/XMP_Environment.h"	// ! XMP_Environment.h must be the first included header.
#include <sstream>
#include <cmath>

#include "public/include/XMP_Const.h"
#include "public/include/XMP_IO.hpp"

#include "XMPFiles/source/XMPFiles_Impl.hpp"
#include "source/XMPFiles_IO.hpp"
#include "source/XIO.hpp"

#include "source/IOUtils.hpp"
#include "XMPFiles/source/FileHandlers/AVCUltra_Handler.hpp"
#include "XMPFiles/source/FormatSupport/PackageFormat_Support.hpp"

// =================================================================================================
/// An AVC-Ultra content file should have following structure :
///
///		/
/// 	CONTENTS/
///			AVCLIP 
/// 			CLIPNAME.MXF
///				...
///			CLIP
/// 			CLIPNAME.XML
/// 			CLIPNAME.XMP
///				...		
/// 		ICON/
/// 			CLIPNAME.BMP
///				...
/// 		PROXY/
/// 			CLIPNAME.MP4
///				CLIPNAME.BIN
///				...
///
// =================================================================================================

//	IsValidClipName
static bool IsValidClipName( const std::string & clipName )
{
	if ( clipName.length() == 8 && IsNum( clipName.at( 0 ) ) && IsNum( clipName.at( 1 ) ) && IsNum( clipName.at( 2 ) )
		&& IsUpperAlpha( clipName.at( 3 ) ) && IsUpperAlpha( clipName.at( 4 ) ) && IsNum( clipName.at( 5 ) )
		&& ( (IsUpperAlpha( clipName.at( 6 ) ) && IsUpperAlpha( clipName.at( 7 ) )) || (IsNum( clipName.at( 6 ) ) && IsNum( clipName.at( 7 ) )) ) )
		return true;
	return false;

}	// IsValidClipName


// =================================================================================================
// AVCUltra_CheckFormat
// ==================
// It will first check for folder CONTENTS and then sub-directories AVCLIP and CLIP.
// Clip name is then checked for dddccdcc or dddccddd where 'd' stands for digit and 'c'stands for upper case character
// If checks return true then root path and clipName would be temporary stored for future processing

bool AVCUltra_CheckFormat( XMP_FileFormat format,
	const std::string & rootPath,
	const std::string & groupName,
	const std::string & parentName,
	const std::string & leafName,
	XMPFiles * parent )
{
	// These should be both empty for logical path and both non-empty for absolute path
	if ( groupName.empty() != parentName.empty() ) 
		return false;

	std::string tempPath = rootPath;
	std::string clipName = leafName;

	// Checking for 'CONTENTS' directory for absolute path
	if ( !groupName.empty() )
	{
		if ( groupName != "CONTENTS" )
			return false;

		// Supporting files under directories "AVCLIP", "PROXY", "CLIP" and "ICON"
		if ( !( parentName == "AVCLIP" || parentName == "PROXY" || parentName == "CLIP" || parentName == "ICON" ) )
			return false;
		
		std::string ext;
		XIO::SplitFileExtension( &clipName, &ext );
	}
	else if ( Host_IO::GetChildMode( tempPath.c_str(), "CONTENTS" ) != Host_IO::kFMode_IsFolder )
		return false;
	
	tempPath += kDirChar;
	tempPath += "CONTENTS";

	// Checking for Existence of 'CLIP' directory under 'CONTENTS' directory
	if ( Host_IO::GetChildMode( tempPath.c_str(), "AVCLIP" ) != Host_IO::kFMode_IsFolder )
		return false;

	// Checking for Existence of 'CLIP' directory under 'CONTENTS' directory
	if ( Host_IO::GetChildMode( tempPath.c_str(), "CLIP" ) != Host_IO::kFMode_IsFolder )
		return false;

	// Checking the validity of File name
	if ( ! IsValidClipName( clipName ) )
		return false;

	// Filling the tempPtr for filling RootPath and ClipName for further processing
	tempPath = rootPath;
	tempPath += kDirChar;
	tempPath += clipName;

	size_t pathLen = tempPath.size() + 1;	// Include a terminating nul.
	parent->tempPtr = malloc( pathLen );
	if ( parent->tempPtr == 0 ) XMP_Throw( "No memory for AVC-Ultra clip path", kXMPErr_NoMemory );
	memcpy( parent->tempPtr, tempPath.c_str(), pathLen );	// AUDIT: Safe, allocated above.

	return true;

}	// AVCUltra_CheckFormat

// =================================================================================================
// AVC_MetaHandlerCTor
// ==================

XMPFileHandler * AVCUltra_MetaHandlerCTor( XMPFiles * parent )
{
	return new AVCUltra_MetaHandler( parent );

}	// AVC_MetaHandlerCTor


// =================================================================================================
// AVCUltra_MetaHandler::~AVCUltra_MetaHandler
// ==================

AVCUltra_MetaHandler::~AVCUltra_MetaHandler()
{
	
	if ( avcManager != 0 )
	{
		delete avcManager;
		avcManager = 0;
	}
}	// AVCUltra_MetaHandler::~AVCUltra_MetaHandler


// =================================================================================================
// AVCUltra_MetaHandler::AVCUltra_MetaHandler
// ==============================

AVCUltra_MetaHandler::AVCUltra_MetaHandler( XMPFiles * _parent ) : isDigestExists( false ), isSpannedClip( false ), avcManager( 0 )
{
	this->handlerFlags = kAVCUltra_HandlerFlags;
	this->parent = _parent;
	this->stdCharForm = kXMP_Char8Bit;

	std::string tempClip;
	if ( this->parent->tempPtr != 0 )
	{
		this->rootPath.assign((char *)(this->parent->tempPtr));
		XIO::SplitLeafName( &this->rootPath, &this->clipName );
		free( this->parent->tempPtr );
		this->parent->tempPtr = 0;
	}
	else
	{
		// Client forcefully ignored CheckFormat, So just update fields
		CreatePseudoClipPath( this->parent->GetFilePath() );
	}

	// Check whether the clip is spanned or not,
	// If yes then get the top clips name for getting metadata
	this->isSpannedClip = AVC_Manager::IsClipSpanned( this->clipName );

	//	Storing file path for mNRT and XMP file
	this->CreateSidecarPaths();

	// Checking for existence of clip's XML file
	if ( !Host_IO::Exists( mNRTPath.c_str() ) )
		XMP_Throw( "AVC-Ultra: Clip metadata XML must be present ", kXMPErr_BadFileFormat );

	avcManager = new AVC_Manager( this->mNRTPath );
	if ( this->isSpannedClip )
		avcManager->ParseSpannedClips();

}	// AVCUltra_MetaHandler::AVCUltra_MetaHandler

// =================================================================================================
// AVCUltra_MetaHandler::CreatePseudoClipPath
// ==============================

void AVCUltra_MetaHandler::CreatePseudoClipPath( const std::string & clientPath )
{
	std::string tempPath = clientPath;
	std::string ignored;

	if ( Host_IO::Exists( tempPath.c_str() ) )
	{
		// Absolute Path case

		// Getting clip name from file path
		XIO::SplitLeafName( &tempPath, &this->clipName );

		// Splitting the file extension
		XIO::SplitFileExtension( &this->clipName, &ignored );

		// Getting sub-directory of CONTENTS
		XIO::SplitLeafName( &tempPath, &ignored );

		// Getting 'CONTENTS' directory and Root Node
		XIO::SplitLeafName( &tempPath, &ignored );
		if ( ignored != "CONTENTS" )
			XMP_Throw( "AVC-Ultra XMP file open failure", kXMPErr_BadFileFormat );

		this->rootPath = tempPath;

	}
	else
	{
		// Logical path

		// Getting clip name from file path
		XIO::SplitLeafName( &tempPath, &this->clipName );
		this->rootPath = tempPath;

	}

}	// AVCUltra_MetaHandler::CreatePseudoClipPath


// =================================================================================================
// AVCUltra_MetaHandler::CreateSidecarPaths
// ==============================

void AVCUltra_MetaHandler::CreateSidecarPaths()
{
	std::string tempPath = this->rootPath;
	
	tempPath += kDirChar;
	tempPath += "CONTENTS";
	
	tempPath += kDirChar;
	tempPath += "CLIP";

	tempPath += kDirChar;
	tempPath += this->clipName;

	this->mNRTPath = tempPath + ".XML";
	this->mXMPPath = tempPath + ".XMP";
	this->mReadXMPPath = this->mXMPPath;

}	// AVCUltra_MetaHandler::CreateSidecarPaths


// =================================================================================================
// AVCUltra_MetaHandler::enableBackwardCompatibility
// ==============================

bool AVCUltra_MetaHandler::enableBackwardCompatibility()
{
	std::string mxfxmpPath = this->rootPath;

	mxfxmpPath += kDirChar;
	mxfxmpPath += "CONTENTS";

	mxfxmpPath += kDirChar;
	mxfxmpPath += "AVCLIP";

	mxfxmpPath += kDirChar;
	mxfxmpPath += this->clipName;

	mxfxmpPath += ".MXF";
	mxfxmpPath += ".xmp";

	if (Host_IO::Exists(mxfxmpPath.c_str()))    //.MFX.xmp of main clip exists in AVCLIP folder. It is being looked for to provide backward compatibility.
	{
		this->mReadXMPPath = mxfxmpPath;
		return true;

	}
	else
	{
		if (!this->isSpannedClip)
			return false;
		//If it is a spanned clip, then clips other than main clip might have their .MXF.xmp in AVCLIP folder. Search for one in ordered manner
		//and pick the first one

		std::string tempString;
		XIO::SplitFileExtension(&mxfxmpPath, &tempString);
		XIO::SplitFileExtension(&mxfxmpPath, &tempString);
		XIO::SplitLeafName(&mxfxmpPath, &tempString);

		XMP_Assert(tempString.length() == 8);
		tempString = tempString.erase(6);

		std::string regVec[] = { tempString + "\\d\\d.MXF.xmp$", tempString + "\\d\\d.MXF.XMP$", tempString + "\\d\\d.MXF.XMp$", tempString + "\\d\\d.MXF.XmP$", tempString + "\\d\\d.MXF.xMP$", tempString + "\\d\\d.MXF.Xmp$", tempString + "\\d\\d.MXF.xMp$", tempString + "\\d\\d.MXF.xmP$" };
		XMP_StringVector regExpVec(regVec, regVec + sizeof(regVec) / sizeof(regVec[0]));

		std::vector<std::string> spannedClipList;
		IOUtils::GetMatchingChildren(spannedClipList, mxfxmpPath, regExpVec, false, true, true);

		//If .MXF.xmp of main clip does not exist in AVCLIP folder, that is why here. Therefore, pick the first one.
		if (!spannedClipList.empty())
		{
			this->mReadXMPPath = *(spannedClipList.begin());
			return true;
		}
		else
			return false;

	}

} //AVCUltra_MetaHandler::enableBackwardCompatibility



#define kMaxXMPLength 100 * 1024 * 1024

// =================================================================================================
// AVCUltra_MetaHandler::CacheFileData
// ==============================

void AVCUltra_MetaHandler::CacheFileData()
{

	XMP_Assert( !this->containsXMP );

	if ( this->parent->UsesClientIO() ) {
		XMP_Throw( "AVC-Ultra cannot be used with client-managed I/O", kXMPErr_InternalFailure );
	}
	
	// Caching sidecar XMP data
	bool bwdCompatibility = false; 
	if ( !Host_IO::Exists( mXMPPath.c_str() ) ) 
	{
		bwdCompatibility = enableBackwardCompatibility();
		if (!bwdCompatibility)
			return; // No XMP.
	}

	// Read the entire .XMP file. We know the XMP sidecar exists, New_XMPFiles_IO is supposed to return 0
	// only if the file does not exist.

	bool readOnly = XMP_OptionIsClear( this->parent->openFlags, kXMPFiles_OpenForUpdate );

	XMP_Assert( this->parent->ioRef == 0 );
	XMPFiles_IO* xmpFile = XMPFiles_IO::New_XMPFiles_IO( mReadXMPPath.c_str(), readOnly );
	if ( xmpFile == 0 )
		XMP_Throw( "AVC-Ultra XMP file open failure", kXMPErr_InternalFailure );

	XMP_Int64 xmpLen = xmpFile->Length();
	if ( xmpLen > kMaxXMPLength ) {
		XMP_Throw( "AVC-Ultra XMP is outrageously large", kXMPErr_InternalFailure );	// Sanity check.
	}

	this->xmpPacket.erase();
	this->xmpPacket.append( ( size_t ) xmpLen, ' ' );

	XMP_Uns32 ioCount = xmpFile->ReadAll( ( void* )this->xmpPacket.data(), ( XMP_Int32 ) xmpLen );
	XMP_Assert( ioCount == xmpLen );

	this->packetInfo.offset = 0;
	this->packetInfo.length = ( XMP_Int32 ) xmpLen;
	FillPacketInfo( this->xmpPacket, &this->packetInfo );

	if (bwdCompatibility)
	{
		xmpFile->Close();
		delete xmpFile;
	}
	else
		this->parent->ioRef = xmpFile;

	this->containsXMP = true;

}	// AVCUltra_MetaHandler::CacheFileData

// =================================================================================================
// AVCUltra_MetaHandler::ImportLegacyXMLProp
// ==============================

void AVCUltra_MetaHandler::ImportLegacyXMLProp( XMP_StringPtr ns, XMP_StringPtr elementName, const std::string &value )
{
	// If digest exists then it means new legacy values would be imported into XMP
	// else, retain previous XMP data

	if ( this->isDigestExists || !this->xmpObj.DoesPropertyExist( ns, elementName ) )
	{
		this->xmpObj.SetProperty( ns, elementName, value, kXMP_DeleteExisting );
		this->containsXMP = true;
	}

}	// AVCUltra_MetaHandler::ImportLegacyXMLProp

// =================================================================================================
// AVCUltra_MetaHandler::ImportLegacyXMLProp
// ==============================

void AVCUltra_MetaHandler::ImportLegacyXMLProp( XMP_StringPtr ns, XMP_StringPtr elementName, const XMP_Uns32 &value )
{
	// If digest exists then it means new legacy values would be imported into XMP
	// else, retain previous XMP data

	if ( this->isDigestExists || !this->xmpObj.DoesPropertyExist( ns, elementName ) )
	{
		this->xmpObj.SetProperty_Int( ns, elementName, value, kXMP_DeleteExisting );
		this->containsXMP = true;
	}

}	// AVCUltra_MetaHandler::ImportLegacyXMLProp

// =================================================================================================
// AVCUltra_MetaHandler::ImportLegacyXMLStructProp
// ==============================

void AVCUltra_MetaHandler::ImportLegacyXMLStructProp( XMP_StringPtr schemaNS, XMP_StringPtr structName, XMP_StringPtr fieldNS, XMP_StringPtr fieldName, const std::string &value )
{
	// If digest exists then it means new legacy values would be imported into XMP
	// else, retain previous XMP data

	if ( this->isDigestExists || !this->xmpObj.DoesStructFieldExist( schemaNS, structName, fieldNS, fieldName ) )
	{
		this->xmpObj.SetStructField( schemaNS, structName, fieldNS, fieldName, value, kXMP_DeleteExisting );
		this->containsXMP = true;
	}

}	// AVCUltra_MetaHandler::ImportLegacyXMLStructProp

// =================================================================================================
// AVCUltra_MetaHandler::ImportLegacyXMLStructProp
// ==============================

void AVCUltra_MetaHandler::ImportLegacyXMLStructProp( XMP_StringPtr schemaNS, XMP_StringPtr structName, XMP_StringPtr fieldNS, XMP_StringPtr fieldName, const XMP_Uns32 &value )
{

	// If digest exists then it means new legacy values would be imported into XMP
	// else, retain previous XMP data

	if ( this->isDigestExists || !this->xmpObj.DoesStructFieldExist( schemaNS, structName, fieldNS, fieldName ) )
	{

		std::ostringstream stream;
		stream << value;

		this->xmpObj.SetStructField( schemaNS, structName, fieldNS, fieldName, stream.str(), kXMP_DeleteExisting );
		this->containsXMP = true;
	}

}	// AVCUltra_MetaHandler::ImportLegacyXMLStructProp

// =================================================================================================
// AVCUltra_MetaHandler::ImportLegacyXMLArray
// ==============================

void AVCUltra_MetaHandler::ImportLegacyXMLArray( XMP_StringPtr ns, XMP_StringPtr elementName, const std::string &value, XMP_Bool requireDelete /*= false*/ )
{
	// If digest exists then it means new legacy values would be imported into XMP
	// else, retain previous XMP data

	if ( ( this->isDigestExists || !this->xmpObj.DoesPropertyExist( ns, elementName ) ) )
	{
		if ( requireDelete )
			this->xmpObj.DeleteProperty( ns, elementName );
		if ( !value.empty() )
		{
			this->xmpObj.AppendArrayItem( ns, elementName, kXMP_PropArrayIsUnordered, value );
			this->containsXMP = true;
		}
	}

}	// AVCUltra_MetaHandler::ImportLegacyXMLArray

// =================================================================================================
// AVCUltra_MetaHandler::ImportFrameSize
// ==============================

void AVCUltra_MetaHandler::ImportFrameSize()
{
		
	XMP_Uns32 frameHeight = this->avcManager->GetClip()->videoEssence.activeLine;
	XMP_Uns32 frameWidth = this->avcManager->GetClip()->videoEssence.activePixel;
	std::string aspectRatio;

	switch ( frameHeight )
	{
	case 720:
		if ( frameWidth == 0 )		// HD
			frameWidth = 1280;
		if ( frameWidth == 1280 )	// HD
			aspectRatio = "16:9";
		else if ( frameWidth != 0 )
		{
			std::ostringstream stream;
			stream << frameWidth << ':' << frameHeight;
			aspectRatio = stream.str();
		}
		break;
	case 1080:
		if ( frameWidth == 0 )		// Full HD
			frameWidth = 1920;
		if ( frameWidth == 1920 )	// Full HD
			aspectRatio = "16:9";
		else if ( frameWidth != 0 )
		{
			std::ostringstream stream;
			stream << frameWidth << ':' << frameHeight;
			aspectRatio = stream.str();

		}
		break;
	case 2160:
		// It could be 2k or UHD or 4k so can't decide based upon only frameHeight
		if ( frameWidth == 2048 )		// 2K
			aspectRatio = "17:9";
		else if ( frameWidth == 3840 )		// UHD
			aspectRatio = "16:9";
		else if ( frameWidth == 4096 )		// 4K
			aspectRatio = "256:135";
		else if ( frameWidth != 0 )
		{
			std::ostringstream stream;
			stream << frameWidth << ':' << frameHeight;
			aspectRatio = stream.str();
		}
		else
			XMP_Throw( " AVC_Metahandler: Not able to calculate frame width for frame width 2160p ",kXMPErr_BadFileFormat );
		break;
	default:
		XMP_Throw( " AVC_Metahandler: Invalid frame height ", kXMPErr_BadFileFormat );
		return;
	}

	ImportLegacyXMLStructProp( kXMP_NS_DM, "videoFrameSize", kXMP_NS_XMP_Dimensions, "w", frameWidth );
	ImportLegacyXMLStructProp( kXMP_NS_DM, "videoFrameSize", kXMP_NS_XMP_Dimensions, "h", frameHeight );
	ImportLegacyXMLStructProp( kXMP_NS_DM, "videoFrameSize", kXMP_NS_XMP_Dimensions, "unit", "pixel" );
	ImportLegacyXMLProp( kXMP_NS_DM, "videoPixelAspectRatio", aspectRatio );

}	// AVCUltra_MetaHandler::ImportFrameSize

// =================================================================================================
// AVCUltra_MetaHandler::AdjustTimeCode
// ===========================================

void AVCUltra_MetaHandler::AdjustTimeCode( std::string & startTimecode, const XMP_Bool & isXMPtoXMLConversion )
{
	/*
	XMP is storing frame number for 50P and 59.94P as [0-49] and [0-59] respectively,
	but NRT XML can store frame number for these format as [0-29].
	So, XMP need to adjust values for these cases.
	*/

	XMP_Int64 strLength = startTimecode.length();
	XMP_Int64 index = strLength - 1;
	for ( ; index > 0; --index )
	{
		char elem = startTimecode.at( index );
		if ( elem == ':' || elem == ';' )
			break;
	}

	std::string ffValue;
	if ( index != 8 )
	{
		XMP_Throw( "AVCUltra_metahandler Invalid Timecode.", kXMPErr_InternalFailure );
		return;
	}
	if ( index == strLength - 2 )								// HH:MM:SS:F
		ffValue = startTimecode.substr( index + 1, 1 );
	else if ( index == strLength - 3 )
		ffValue = startTimecode.substr( index + 1, 2 );				// HH:MM:SS:FF
	else
		XMP_Throw( "AVCUltra_metahandler Invalid Timecode.", kXMPErr_InternalFailure );
	std::stringstream timeCodeStream( ffValue );
	XMP_Int32 frameNumber = -1;
	timeCodeStream >> frameNumber;
	if ( isXMPtoXMLConversion )									// Conversion from XMP to XML so doing half the value
	{
		if ( frameNumber < 0 || frameNumber > 59 )
			XMP_Throw( "AVCUltra_metahandler Invalid Timecode.", kXMPErr_InternalFailure );
		frameNumber /= 2;
	}
	else														// Conversion from XML to XMP so doubling the value
	{
		frameNumber *= 2;
		if ( frameNumber < 0 || frameNumber > 59 )
			XMP_Throw( "AVCUltra_metahandler Invalid Timecode.", kXMPErr_InternalFailure );
	}
	timeCodeStream.clear();
	timeCodeStream << startTimecode.substr( 0, index + 1 );
	if ( frameNumber < 10 )
		timeCodeStream << '0';
	timeCodeStream << frameNumber;
	startTimecode = timeCodeStream.str();

}	// AVCUltra_MetaHandler::AdjustTimeCode


// =================================================================================================
// AVCUltra_MetaHandler::ImportStartTimecode
// ===========================================

void AVCUltra_MetaHandler::ImportStartTimecode()
{
	//	Translate start timecode to the format specified by the dynamic media schema.
	if ( this->isDigestExists || ( !this->xmpObj.DoesPropertyExist( kXMP_NS_DM, "startTimecode" ) ) ) {

		std::string startTimecode = this->avcManager->GetClip()->videoEssence.startTimecode;
		if ( ! startTimecode.empty() ) 
		{
			const std::string frameRate = this->avcManager->GetClip()->videoEssence.frameRate;
			if ( !frameRate.empty() )
			{
				const XMP_Bool isDropType = XMP_LitMatch( this->avcManager->GetClip()->videoEssence.timecodeType.c_str(), "Drop" );
				if ( !isDropType )
					XMP_Assert( XMP_LitMatch( this->avcManager->GetClip()->videoEssence.timecodeType.c_str(), "NonDrop" ) );

				std::string dmTimeFormat;

				if ( frameRate == "50i" || frameRate == "25p" )
					dmTimeFormat = "25Timecode";
				else if ( frameRate == "50p" )
				{
					dmTimeFormat = "50Timecode";
					this->AdjustTimeCode( startTimecode, false );
				}
				else if ( frameRate == "23.98p" )
					dmTimeFormat = "23976Timecode";
				else if ( frameRate == "59.94p" )
				{
					if ( isDropType )
					{
						dmTimeFormat = "5994DropTimecode";
						//	Drop frame timecode uses semicolons instead of colons as delimiter
						for ( std::string::iterator iter = startTimecode.begin(); iter != startTimecode.end(); ++iter )
							if ( *iter == ':' )
								*iter = ';';
					}
					else
						dmTimeFormat = "5994NonDropTimecode";
					this->AdjustTimeCode( startTimecode, false );
				}
				else if ( frameRate == "59.94i" || frameRate == "29.97p" )
				{
					if ( !isDropType )
						dmTimeFormat = "2997NonDropTimecode";
					else
					{
						//	Drop frame timecode uses semicolons instead of colons as delimiter
						for ( std::string::iterator iter = startTimecode.begin(); iter != startTimecode.end(); ++iter )
							if ( *iter == ':' )
								*iter = ';';
						dmTimeFormat = "2997DropTimecode";

					}

				}

				if ( !startTimecode.empty() && !dmTimeFormat.empty() )
				{
					this->xmpObj.SetStructField( kXMP_NS_DM, "startTimecode", kXMP_NS_DM, "timeValue", startTimecode, 0 );
					this->xmpObj.SetStructField( kXMP_NS_DM, "startTimecode", kXMP_NS_DM, "timeFormat", dmTimeFormat, 0 );
					this->containsXMP = true;
				}
			}
		}

	}
}	// AVCUltra_MetaHandler::ImportStartTimecode

// =================================================================================================
// AVCUltra_MetaHandler::ImportRelationMetadata
// ===========================================

void AVCUltra_MetaHandler::ImportRelationMetadata()
{
	AVC_Clip * clip = this->avcManager->GetClip();
	if ( this->isDigestExists || !this->xmpObj.DoesPropertyExist( kXMP_NS_DC, "relation" ) )
	{
		this->xmpObj.DeleteProperty( kXMP_NS_DC, "relation" );
		if ( !clip->relationStruct.globalShotId.empty() )
			this->xmpObj.AppendArrayItem( kXMP_NS_DC, "relation", kXMP_PropArrayIsUnordered, "globalShotID:" + clip->relationStruct.globalShotId );
		if ( !clip->relationStruct.topClipId.empty() )
			this->xmpObj.AppendArrayItem( kXMP_NS_DC, "relation", kXMP_PropArrayIsUnordered, "topGlobalClipID:" + clip->relationStruct.topClipId );
		if ( !clip->relationStruct.prevClipId.empty() )
			this->xmpObj.AppendArrayItem( kXMP_NS_DC, "relation", kXMP_PropArrayIsUnordered, "previousGlobalClipID:" + clip->relationStruct.prevClipId );
		if ( !clip->relationStruct.nextClipId.empty() )
			this->xmpObj.AppendArrayItem( kXMP_NS_DC, "relation", kXMP_PropArrayIsUnordered,  "nextGlobalClipID:" + clip->relationStruct.nextClipId );
	}

}	// AVCUltra_MetaHandler::ImportRelationMetadata

// =================================================================================================
// AVCUltra_MetaHandler::ImportAudioMetadata
// ===========================================

void AVCUltra_MetaHandler::ImportAudioMetadata()
{
	AVC_Clip * clip = this->avcManager->GetClip();
	ImportLegacyXMLProp( kXMP_NS_DM, "audioSampleRate", clip->audioEssence.samplingRate );
	if ( clip->audioEssence.bitsPerSample == "16" || clip->audioEssence.bitsPerSample == "24" )
		ImportLegacyXMLProp( kXMP_NS_DM, "audioSampleType", clip->audioEssence.bitsPerSample + "Int" );

}	// AVCUltra_MetaHandler::ImportAudioMetadata

// =================================================================================================
// AVCUltra_MetaHandler::ImportDescMetadata
// ===========================================

void AVCUltra_MetaHandler::ImportDescMetadata()
{
	AVC_Clip * clip = this->avcManager->GetClip();

	ImportLegacyXMLArray( kXMP_NS_DC, "creator", clip->descMetadata.creator,true );

	// TO DO: Mapping issue with TIFF:Artist

	ImportLegacyXMLProp( kXMP_NS_XMP, "CreateDate", clip->descMetadata.creationDate );
	ImportLegacyXMLProp( kXMP_NS_XMP, "ModifyDate", clip->descMetadata.lastUpdateDate );
	
	const std::string tempString = clip->descMetadata.shotMark;
	if ( tempString.empty() )
		this->xmpObj.DeleteProperty( kXMP_NS_DM, "good" );
	else if ( tempString == "true" || tempString == "1" )
		this->xmpObj.SetProperty_Bool( kXMP_NS_DM, "good", true, kXMP_DeleteExisting );
	else if ( tempString == "false" || tempString == "0" )
		this->xmpObj.SetProperty_Bool( kXMP_NS_DM, "good", false, kXMP_DeleteExisting );

	ImportLegacyXMLProp( kXMP_NS_DM, "shotLocation", clip->descMetadata.placeName );
	ImportLegacyGPSProp( kXMP_NS_EXIF, "GPSLongitude", clip->descMetadata.longitude );
	ImportLegacyGPSProp( kXMP_NS_EXIF, "GPSLatitude", clip->descMetadata.latitude );

	if ( ( this->isDigestExists || !this->xmpObj.DoesPropertyExist( kXMP_NS_EXIF, "GPSAltitude" ) ) && clip->descMetadata.isAltitudeExists )
	{
		XMP_Int32 altitude = clip->descMetadata.altitude;
		if ( altitude >= 0 )
		{
			this->xmpObj.SetProperty_Int( kXMP_NS_EXIF, "GPSAltitudeRef", 0, kXMP_DeleteExisting );
			this->xmpObj.SetProperty_Int( kXMP_NS_EXIF, "GPSAltitude", altitude, kXMP_DeleteExisting );
		}
		else
		{
			this->xmpObj.SetProperty_Int( kXMP_NS_EXIF, "GPSAltitudeRef", 1, kXMP_DeleteExisting );
			this->xmpObj.SetProperty_Int( kXMP_NS_EXIF, "GPSAltitude", -altitude, kXMP_DeleteExisting );
		}
	}
		
	ImportLegacyXMLProp( kXMP_NS_TIFF, "Make", clip->descMetadata.manufacture );
	ImportLegacyXMLProp( kXMP_NS_EXIF_Aux, "SerialNumber", clip->descMetadata.serialNum );
	ImportLegacyXMLProp( kXMP_NS_TIFF, "Model", clip->descMetadata.modelName );
	if ( clip->descMetadata.sceneNum != 0 )
		ImportLegacyXMLProp( kXMP_NS_DM, "scene", clip->descMetadata.sceneNum );
	if ( clip->descMetadata.takeNum != 0 )
		ImportLegacyXMLProp( kXMP_NS_DM, "takeNumber", clip->descMetadata.takeNum );

}	// AVCUltra_MetaHandler::ImportDescMetadata

// =================================================================================================
// AVCUltra_MetaHandler::ImportLegacyGPSProp
// ===========================================

void AVCUltra_MetaHandler::ImportLegacyGPSProp( XMP_StringPtr ns, XMP_StringPtr elementName, const std::string &value )
{
	if ( ( this->isDigestExists || !this->xmpObj.DoesPropertyExist( ns, elementName ) ) && !value.empty() )
	{
		//	Convert from decimal to sexagesimal GPS coordinates
		char direction = '\0';
		double degrees = 0.0;
		const int numFieldsRead = sscanf( value.c_str(), "%c%lf", &direction, &degrees );

		if ( numFieldsRead == 2 )
		{
			double wholeDegrees = 0.0;
			const double fractionalDegrees = std::modf( degrees, &wholeDegrees );
			const double minutes = fractionalDegrees * 60.0;
			char xmpValue[ 128 ];

			////////////////////////////////////////////////////////////////
			// ACR: replaced sprintf with sprintf_safe
			#if 0
			sprintf( xmpValue, "%d,%.5lf%c", static_cast<int>( wholeDegrees ), minutes, direction );
			#else
			sprintf_safe ( xmpValue, sizeof (xmpValue), "%d,%.5lf%c", static_cast<int>( wholeDegrees ), minutes, direction );
			#endif
			////////////////////////////////////////////////////////////////
			this->xmpObj.SetProperty( ns, elementName, xmpValue );
			this->containsXMP = true;

		}

	}
}	// AVCUltra_MetaHandler::ImportLegacyGPSProp

// =================================================================================================
// AVCUltra_MetaHandler::ImportCodecInfo
// ===========================================

void AVCUltra_MetaHandler::ImportCodecInfo()
{
	std::string codec = this->avcManager->GetClip()->videoEssence.codec;
	XMP_Uns32 classField = this->avcManager->GetClip()->videoEssence.classField;
	if ( this->avcManager->GetClip()->videoEssence.isClassExists && codec.compare( 0, 9, "AVC-LongG" ) == 0 )
	{
		std::ostringstream stream;
		stream << codec << "-G" << classField;
		codec = stream.str();
	}
	
	this->xmpObj.SetProperty( kXMP_NS_DM, "videoCompressor", codec, kXMP_DeleteExisting );

}	// AVCUltra_MetaHandler::ImportCodecInfo

// =================================================================================================
// AVCUltra_MetaHandler::ForceLegacyProp
// =================================
// Based upon P2_handler::ForceChildElement

void AVCUltra_MetaHandler::ForceLegacyProp( XML_NodePtr parent, XMP_StringPtr localName, XMP_StringPtr value, XMP_Int32 indent, XMP_Bool insertAtFront /*= false*/ )
{
	XML_Node * wsNodeBefore, *wsNodeAfter;
	wsNodeBefore = wsNodeAfter = NULL;
	XMP_StringPtr ns = this->avcManager->GetRootNode()->ns.c_str();
	XML_Node * childNode = parent->GetNamedElement( ns, localName );

	if ( childNode == 0 ) 
	{
		// The indenting is a hack, assuming existing 2 spaces per level.
		try {
			wsNodeBefore = new XML_Node( parent, "", kCDataNode );
			wsNodeBefore->value = "  ";	// Add 2 spaces to the existing WS before the parent's close tag.

			childNode = new XML_Node( parent, localName, kElemNode );
			childNode->ns = parent->ns;
			childNode->nsPrefixLen = parent->nsPrefixLen;
			childNode->name.insert( 0, parent->name, 0, parent->nsPrefixLen );

			wsNodeAfter = new XML_Node( parent, "", kCDataNode );
		}
		catch ( ... ) {
			if ( wsNodeBefore )
				delete wsNodeBefore;
			if ( childNode )
				delete childNode;

			throw;
		}
		wsNodeAfter->value = '\n';
		for ( ; indent > 1; --indent ) wsNodeAfter->value += "  ";	// Indent less 1, to "outdent" the parent's close.

		if ( insertAtFront ){
			// we are asked to insert this child as the first child of it's parent.So if P is the parent and B,C are children
			// already present. Then if we add a new child A as it's first child then we need to first add new line character right
			// after "<P>" and then proper indentation to bring them on the level of other children.
			//<P>
			//	  <B>
			//	  <C>
			//</P>
			std::vector<XML_Node *> indentedNode;
			indentedNode.push_back( wsNodeAfter );
			indentedNode.push_back( wsNodeBefore );
			indentedNode.push_back( childNode );
			parent->content.insert( parent->content.begin(), indentedNode.begin(), indentedNode.end() );
		}
		else{
			parent->content.push_back( wsNodeBefore );
			parent->content.push_back( childNode );
			parent->content.push_back( wsNodeAfter );
		}

	}

	childNode->SetLeafContentValue( value );

}	// AVCUltra_MetaHandler::ForceLegacyProp

// =================================================================================================
// AVCUltra_MetaHandler::IsMetadataWritable
// =======================================

bool AVCUltra_MetaHandler::IsMetadataWritable()
{
	return Host_IO::Writable( this->mXMPPath.c_str(), true ) && Host_IO::Writable( this->mNRTPath.c_str(), false );

}	// AVCUltra_MetaHandler::IsMetadataWritable

// =================================================================================================
// AVCUltra_MetaHandler::FillAssociatedResources
// ======================================
void AVCUltra_MetaHandler::FillAssociatedResources( std::vector<std::string> * resourceList )
{
	
	// The possible associated resources:
	//	CONTENTS/
	//		CLIP/
	//			XXXXXXXX.XML			XXXXXXXX is clip name
	//			XXXXXXXX.XMP
	//		AVCLIP/
	//			XXXXXXXX.MXF
	//		ICON/
	//			XXXXXXXX.BMP
	//		PROXY/
	//			XXXXXXXX.MP4
	//			XXXXXXXX.BIN
	
	XMP_VarString contentsPath = this->rootPath + kDirChar + "CONTENTS" + kDirChar;
	XMP_VarString path;

	//Add RootPath
	path = this->rootPath + kDirChar;
	PackageFormat_Support::AddResourceIfExists( resourceList, path );

	std::string temp = this->avcManager->GetClip()->clipPath;
	std::vector<AVC_Clip * >::iterator iter = this->avcManager->spannedClips.begin();
	bool isClipChildExists = false, isAVCChildExists = false, isIconChildExists = false, isProxyChildExists = false;
	while ( true )
	{
		std::string clipNameStr;
		XIO::SplitLeafName( &temp, &clipNameStr );
		XIO::SplitFileExtension( &clipNameStr, &temp );

		// Get the files present inside CLIP folder.
		path = contentsPath + "CLIP" + kDirChar + clipNameStr + ".XML";
		isClipChildExists |= PackageFormat_Support::AddResourceIfExists( resourceList, path );
		path = contentsPath + "CLIP" + kDirChar + clipNameStr + ".XMP";
		isClipChildExists |= PackageFormat_Support::AddResourceIfExists( resourceList, path );


		// Get the files present inside AVCLIP folder.
		path = contentsPath + "AVCLIP" + kDirChar + clipNameStr + ".MXF";
		isAVCChildExists |= PackageFormat_Support::AddResourceIfExists( resourceList, path );

		// Get the files present inside ICON folder.
		path = contentsPath + "ICON" + kDirChar + clipNameStr + ".BMP";
		isIconChildExists |= PackageFormat_Support::AddResourceIfExists( resourceList, path );


		// Get the files present inside PROXY folder.
		path = contentsPath + "PROXY" + kDirChar + clipNameStr + ".MP4";
		isProxyChildExists |= PackageFormat_Support::AddResourceIfExists( resourceList, path );
		path = contentsPath + "PROXY" + kDirChar + clipNameStr + ".BIN";
		isProxyChildExists |= PackageFormat_Support::AddResourceIfExists( resourceList, path );
		if ( iter == this->avcManager->spannedClips.end() )
			break;
		temp = ( *iter )->clipPath;
		++iter;
	}

	if ( !isClipChildExists )
	{
		path = contentsPath + "CLIP" + kDirChar;
		PackageFormat_Support::AddResourceIfExists( resourceList, path );
	}
	if ( !isAVCChildExists )
	{
		path = contentsPath + "AVCLIP" + kDirChar;
		PackageFormat_Support::AddResourceIfExists( resourceList, path );
	}
	if ( !isIconChildExists )
	{
		path = contentsPath + "ICON" + kDirChar;
		PackageFormat_Support::AddResourceIfExists( resourceList, path );
	}
	if ( !isProxyChildExists )
	{
		path = contentsPath + "PROXY" + kDirChar;
		PackageFormat_Support::AddResourceIfExists( resourceList, path );
	}
}	// AVCUltra_MetaHandler::FillAssociatedResources


// =================================================================================================
// AVCUltra_MetaHandler::ProcessXMP
// ==========================

void AVCUltra_MetaHandler::ProcessXMP()
{
	
	if ( this->processedXMP ) return;
	this->processedXMP = true;	// Make sure only called once.

	if ( this->containsXMP ) {
		this->xmpObj.ParseFromBuffer( this->xmpPacket.c_str(), ( XMP_StringLen )this->xmpPacket.size() );
	}

	AVC_Clip * clip = this->avcManager->GetClip();
	
	// Duration/Edit unit
	XMP_Uns32 totalDur = clip->duration;
	if ( this->isSpannedClip )
	{
		std::vector<AVC_Clip *>::iterator iter;
		for ( iter = this->avcManager->spannedClips.begin(); iter != this->avcManager->spannedClips.end(); ++iter )
			totalDur += ( *iter )->duration;

		// Storing total duration for digest field
		clip->duration = totalDur;
	}

	// First check for Digest match
	std::string oldDigest, newDigest;
	this->isDigestExists = this->xmpObj.GetStructField( kXMP_NS_XMP, "NativeDigests", kXMP_NS_XMP, "AVCUltra", &oldDigest, 0 );
	if ( this->isDigestExists ) {
		avcManager->CreateClipDigest( newDigest );
		if ( oldDigest == newDigest ) return;
	}

	// If digest matches then no need to process anything,
	// If digest mismatch then it means legacy XML has been modified so need to reconcile it,
	// If no digest present then it means this is the first time when XMP is processing over it so save previous information

	// Global Clip ID
	ImportLegacyXMLProp( kXMP_NS_DC, "identifier", clip->globalClipId );

	
	ImportLegacyXMLStructProp( kXMP_NS_DM, "duration", kXMP_NS_DM, "value", totalDur );
	ImportLegacyXMLStructProp( kXMP_NS_DM, "duration", kXMP_NS_DM, "scale", clip->editUnit );

	// Relation array
	ImportRelationMetadata();

	// DM:shotname
	ImportLegacyXMLProp( kXMP_NS_DM, "shotName", clip->descMetadata.userClipName );

	// Audio Essence
	ImportAudioMetadata();

	// Video Essence
	ImportFrameSize();
	// Import codec with class
	ImportCodecInfo();
	ImportStartTimecode();
	ImportLegacyXMLProp( kXMP_NS_DM, "videoFrameRate", clip->videoEssence.frameRate );

	// Descriptive metadata
	ImportDescMetadata();

}	// AVCUltra_MetaHandler::ProcessXMP


// =================================================================================================
// AVCUltra_MetaHandler::UpdateFile
// ==========================
void AVCUltra_MetaHandler::UpdateFile( bool doSafeUpdate )
{
	if ( !this->needsUpdate ) return;
	this->needsUpdate = false;	// Make sure only called once.

	XMP_Assert( this->parent->UsesLocalIO() );

	if ( this->mNRTPath.empty() )
		return;

	if ( avcManager == 0 )
		XMP_Throw( "AVC-Ultra Manager is not initialised", kXMPErr_InternalFailure );

	// Update the internal legacy XML tree if we have one, and set the digest in the XMP.

	bool updateLegacyXML = false;

	AVC_Clip * clip = this->avcManager->GetClip();

	// creator array
	std::string newValue;
	std::string oldValue = clip->descMetadata.creator;
	if ( this->xmpObj.GetArrayItem( kXMP_NS_DC, "creator", 1, &newValue, 0 ) )
	{
		if ( newValue != oldValue )
		{
			this->ForceLegacyProp( clip->descMetadata.accessPtr, "Creator", newValue.c_str(), 3, true );
			updateLegacyXML = true;
		}
	}
	else if ( !oldValue.empty() )
	{
		this->xmpObj.DeleteArrayItem( kXMP_NS_DC, "creator", 1 );
		updateLegacyXML = true;
	}

	// Write Digest
	// To do: if digest already exists and no Updation is required then no need to calculate digest
	std::string digestStr;
	this->avcManager->CreateClipDigest( digestStr );
	this->xmpObj.SetStructField( kXMP_NS_XMP, "NativeDigests", kXMP_NS_XMP, "AVCUltra", digestStr.c_str(), kXMP_DeleteExisting );

	this->xmpObj.SerializeToBuffer( &this->xmpPacket, this->GetSerializeOptions() );

	// -----------------------------------------------------------------------
	// Update the XMP file first, don't let legacy XML failures block the XMP.
	bool isXMPExists = Host_IO::Exists( this->mXMPPath.c_str() );
	if ( !isXMPExists )
	{
		XMP_Assert( this->parent->ioRef == 0 );
		Host_IO::Create( this->mXMPPath.c_str() );
		this->parent->ioRef = XMPFiles_IO::New_XMPFiles_IO( this->mXMPPath.c_str(), Host_IO::openReadWrite );
		if ( this->parent->ioRef == 0 ) XMP_Throw( "Failure opening AVCUltra XMP file", kXMPErr_ExternalFailure );
	}

	XMP_IO* xmpFile = this->parent->ioRef;
	XMP_Assert( xmpFile != 0 );
	XIO::ReplaceTextFile( xmpFile, this->xmpPacket, ( isXMPExists & doSafeUpdate ) );

	// Now update the legacy XML file if necessary.
	if ( updateLegacyXML )
		this->avcManager->SerialiseNRTfile( this->mNRTPath, doSafeUpdate );

}	// AVCUltra_MetaHandler::UpdateFile

// =================================================================================================
// AVCUltra_MetaHandler::WriteTempFile
// =============================

void AVCUltra_MetaHandler::WriteTempFile( XMP_IO* tempRef )
{

	// ! WriteTempFile is not supposed to be called for handlers that own the file.
	XMP_Throw( "AVC_MetaHandler::WriteTempFile should not be called", kXMPErr_InternalFailure );

}	// AVCUltra_MetaHandler::WriteTempFile

// ===================================================================================================================
