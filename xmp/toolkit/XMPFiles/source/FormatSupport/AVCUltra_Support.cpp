
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

#include "public/include/XMP_Const.h"
#include "public/include/XMP_IO.hpp"
#include "source/ExpatAdapter.hpp"
#include "source/IOUtils.hpp"

#include "XMPFiles/source/FormatSupport/AVCUltra_Support.hpp"
#include "source/XMPFiles_IO.hpp"
#include "XMPFiles/source/FormatSupport/PackageFormat_Support.hpp"

// =================================================================================================
// AVC_Manager::AVC_Manager
// ==================

AVC_Manager::AVC_Manager( const std::string &origClipXML ) : mainClipAdapter( 0 ), mainRootNode( 0 )
{

	try
	{
		this->mainClip = new AVC_Clip();
		this->mainClipAdapter = XMP_NewExpatAdapter( ExpatAdapter::kUseLocalNamespaces );
		CacheClipContent( origClipXML, this->mainClip, this->mainClipAdapter, this->mainRootNode );
	}
	catch ( ... )
	{
		if ( this->mainClipAdapter != 0 )
		{
			delete this->mainClipAdapter;
			this->mainClipAdapter = 0;
		}
		if ( this->mainClip != 0 )
		{
			delete this->mainClip;
			this->mainClip = 0;
		}
		this->mainRootNode = 0;
		throw;
	}

}	// AVC_Manager::AVC_Manager

// =================================================================================================
// AVC_Manager::CacheClipContent
// ==================

void AVC_Manager::CacheClipContent( const std::string &origClipXML, AVC_Clip * clipToFill, ExpatAdapter * xmlAdapter, XML_NodePtr &rootNode )
{

	if ( xmlAdapter == 0 ) XMP_Throw( "AVCUltra_MetaHandler: Can't create Expat adapter", kXMPErr_NoMemory );

	XMP_Uns8 buffer[ 64 * 1024 ];
	Host_IO::FileRef hostRef = Host_IO::Open( origClipXML.c_str(), Host_IO::openReadOnly );
	XMPFiles_IO xmlFile( hostRef, origClipXML.c_str(), Host_IO::openReadOnly );

	while ( true ) {
		XMP_Int32 ioCount = xmlFile.Read( buffer, sizeof( buffer ) );
		if ( ioCount == 0 ) break;
		xmlAdapter->ParseBuffer( buffer, ioCount, false /* not the end */ );
	}
	xmlAdapter->ParseBuffer( 0, 0, true );

	xmlFile.Close();

	for ( size_t index = 0, limit = xmlAdapter->tree.content.size(); index < limit; ++index )
		if ( xmlAdapter->tree.content[ index ]->kind == kElemNode )
			rootNode = xmlAdapter->tree.content[ index ];

	if ( rootNode == 0 ) XMP_Throw( "AVCUltra_MetaHandler: Can't get root node of NRT XML", kXMPErr_InternalFailure );
	XMP_StringPtr rootLocalName = rootNode->name.c_str() + rootNode->nsPrefixLen;
	if ( !XMP_LitMatch( rootLocalName, "P2Main" ) )
		XMP_Throw( "AVCUltra_MetaHandler: Not supported this version of AVC-Ultra", kXMPErr_BadFileFormat );

	clipToFill->clipPath = origClipXML;

	XML_NodePtr node, contentNode, tempNode;
	XMP_StringPtr nameSpace = rootNode->ns.c_str();
	contentNode = rootNode->GetNamedElement( nameSpace, "ClipContent" );
	if ( contentNode == 0 )  
		XMP_Throw( "AVCUltra_MetaHandler: Required field not present", kXMPErr_BadFileFormat );;

	// Required
	node = contentNode->GetNamedElement( nameSpace, "GlobalClipID" );
	GetRequiredValue( node, clipToFill->globalClipId );

	// Required
	node = contentNode->GetNamedElement( nameSpace, "Duration" );
	GetRequiredValue( node, clipToFill->duration );

	// Required
	node = contentNode->GetNamedElement( nameSpace, "EditUnit" );
	GetRequiredValue( node, clipToFill->editUnit );

	// Optional
	tempNode = contentNode->GetNamedElement( nameSpace, "Relation" );
	if ( tempNode != 0 )
	{
		clipToFill->relationStruct.isExists = true;
		
		node = tempNode->GetNamedElement( nameSpace, "OffsetInShot" );
		GetRequiredValue( node, clipToFill->relationStruct.offsetInShot );

		node = tempNode->GetNamedElement( nameSpace, "GlobalShotID" );
		GetRequiredValue( node, clipToFill->relationStruct.globalShotId );

		XML_NodePtr connectionNode = tempNode->GetNamedElement( nameSpace, "Connection" );
		if ( connectionNode != 0 )
		{
			node = connectionNode->GetNamedElement( nameSpace, "Top" );
			if ( node != 0 )
				GetRequiredValue( node->GetNamedElement( nameSpace, "GlobalClipID" ), clipToFill->relationStruct.topClipId );
			else
				XMP_Throw( "AVCUltra_MetaHandler: Required field not present", kXMPErr_BadFileFormat );

			node = connectionNode->GetNamedElement( nameSpace, "Previous" );
			if ( node != 0 )
				GetRequiredValue( node->GetNamedElement( nameSpace, "GlobalClipID" ), clipToFill->relationStruct.prevClipId );

			node = connectionNode->GetNamedElement( nameSpace, "Next" );
			if ( node != 0 )
				GetRequiredValue( node->GetNamedElement( nameSpace, "GlobalClipID" ), clipToFill->relationStruct.nextClipId );
		}
		else
			XMP_Throw( "AVCUltra_MetaHandler: Required field not present", kXMPErr_BadFileFormat );
	}

	// Required
	tempNode = contentNode->GetNamedElement( nameSpace, "EssenceList" );
	if ( tempNode == 0 )
		XMP_Throw( "AVCUltra_MetaHandler: Required field not present", kXMPErr_BadFileFormat );

	// Required
	node = tempNode->GetNamedElement( nameSpace, "Video" );
	if ( node != 0 )
	{
		XML_NodePtr codecPtr = node->GetNamedElement( nameSpace, "Codec" );
		GetRequiredValue( codecPtr, clipToFill->videoEssence.codec );
		if ( codecPtr->attrs.size() == 1 )
		{
			clipToFill->videoEssence.isClassExists = true;
			GetAttributeValue( codecPtr->attrs[ 0 ], clipToFill->videoEssence.classField );
		}
		GetOptionalValue( node->GetNamedElement( nameSpace, "ActiveLine" ), clipToFill->videoEssence.activeLine );
		GetOptionalValue( node->GetNamedElement( nameSpace, "ActivePixel" ), clipToFill->videoEssence.activePixel );
		GetRequiredValue( node->GetNamedElement( nameSpace, "FrameRate" ), clipToFill->videoEssence.frameRate );
		GetRequiredValue( node->GetNamedElement( nameSpace, "TimecodeType" ), clipToFill->videoEssence.timecodeType );
		clipToFill->videoEssence.startTimecodePtr = node->GetNamedElement( nameSpace, "StartTimecode" );
		GetRequiredValue( clipToFill->videoEssence.startTimecodePtr, clipToFill->videoEssence.startTimecode );
	}
	else
		XMP_Throw( "AVCUltra_MetaHandler: Required field not present", kXMPErr_BadFileFormat );

	// Optional
	node = tempNode->GetNamedElement( nameSpace, "Audio" );
	if ( node != 0 )
	{
		GetRequiredValue( node->GetNamedElement( nameSpace, "SamplingRate" ), clipToFill->audioEssence.samplingRate );
		GetRequiredValue( node->GetNamedElement( nameSpace, "BitsPerSample" ), clipToFill->audioEssence.bitsPerSample );
	}

	// Required Clip Metadata
	node = contentNode->GetNamedElement( nameSpace, "ClipMetadata" );
	if ( node != 0 )
	{
		GetOptionalValue( node->GetNamedElement( nameSpace, "UserClipName" ), clipToFill->descMetadata.userClipName );
		
		// Data Source
		std::string dataSource;
		GetRequiredValue( node->GetNamedElement( nameSpace, "DataSource" ), dataSource );

		GetOptionalValue( node->GetNamedElement( nameSpace, "ShotMark" ), clipToFill->descMetadata.shotMark );
		
		// Required
		tempNode = node->GetNamedElement( nameSpace, "Access" );
		if ( tempNode != 0 )
		{
			clipToFill->descMetadata.accessPtr = tempNode;
			GetOptionalValue( tempNode->GetNamedElement( nameSpace, "Creator" ), clipToFill->descMetadata.creator );
			
			GetRequiredValue( tempNode->GetNamedElement( nameSpace, "CreationDate" ), clipToFill->descMetadata.creationDate );
			GetRequiredValue( tempNode->GetNamedElement( nameSpace, "LastUpdateDate" ), clipToFill->descMetadata.lastUpdateDate );

		}
		else
			XMP_Throw( "AVCUltra_MetaHandler: Required field not present", kXMPErr_BadFileFormat );

		// Optional
		tempNode = node->GetNamedElement( nameSpace, "Device" );
		if ( tempNode != 0 )
		{
			GetOptionalValue( tempNode->GetNamedElement( nameSpace, "Manufacturer" ), clipToFill->descMetadata.manufacture );
			GetOptionalValue( tempNode->GetNamedElement( nameSpace, "SerialNo." ), clipToFill->descMetadata.serialNum );
			GetOptionalValue( tempNode->GetNamedElement( nameSpace, "ModelName" ), clipToFill->descMetadata.modelName );
		}

		// Required if DataSource is "SHOOTING"
		tempNode = node->GetNamedElement( nameSpace, "Shoot" );
		if ( tempNode != 0 )
		{
			GetOptionalValue( tempNode->GetNamedElement( nameSpace, "Shooter" ), clipToFill->descMetadata.shooter );
			tempNode = tempNode->GetNamedElement( nameSpace, "Location" );
			if ( tempNode != 0 )
			{
				GetOptionalValue( tempNode->GetNamedElement( nameSpace, "PlaceName" ), clipToFill->descMetadata.placeName );

				XML_NodePtr altitudeNode = tempNode->GetNamedElement( nameSpace, "Altitude" );
				if ( altitudeNode != 0 )
				{
					clipToFill->descMetadata.isAltitudeExists = true;
					GetOptionalValue( altitudeNode, clipToFill->descMetadata.altitude );
				}

				XMP_Bool isRequired = clipToFill->descMetadata.placeName.empty();
				if ( isRequired )
				{
					GetRequiredValue( tempNode->GetNamedElement( nameSpace, "Longitude" ), clipToFill->descMetadata.longitude );
					GetRequiredValue( tempNode->GetNamedElement( nameSpace, "Latitude" ), clipToFill->descMetadata.latitude );
				}
				else
				{
					GetOptionalValue( tempNode->GetNamedElement( nameSpace, "Longitude" ), clipToFill->descMetadata.longitude );
					GetOptionalValue( tempNode->GetNamedElement( nameSpace, "Latitude" ), clipToFill->descMetadata.latitude );
				}
			}

		}
		else if ( dataSource == "SHOOTING" )
			XMP_Throw( "AVCUltra_MetaHandler: Required field not present", kXMPErr_BadFileFormat );

		// Optional
		tempNode = node->GetNamedElement( nameSpace, "Scenario" );
		if ( tempNode != 0 )
		{
			GetOptionalValue( tempNode->GetNamedElement( nameSpace, "SceneNo." ), clipToFill->descMetadata.sceneNum );
			GetOptionalValue( tempNode->GetNamedElement( nameSpace, "TakeNo." ), clipToFill->descMetadata.takeNum );
		}
	}
	else
		XMP_Throw( "AVCUltra_MetaHandler: Required field not present", kXMPErr_BadFileFormat );

}	// AVC_Manager::CacheClipContent

// =================================================================================================
// AVC_Manager::IsClipSpanned
// ==================
// Here we checked on the basis upon last 2 characters of the clip name.
// Last 2 characters shall represent valid clip number from 00-99 for clip to be spanned

/*static*/
XMP_Bool AVC_Manager::IsClipSpanned( std::string & topClip )
{
	if ( topClip.length() == 8 )
	{
		if ( IsNum( topClip.at( 6 ) ) && IsNum( topClip.at( 7 ) ) )
		{
			topClip = topClip.erase( 6 );
			topClip = topClip + "00";
			return true;
		}
		else
			return false;
	}
	else
		XMP_Throw( "AVC-Ultra Clip metadata file name should have 8 characters", kXMPErr_BadFileFormat );

}	// AVC_Manager::IsClipSpanned

// =================================================================================================
// AVC_Manager::GetRequiredValue
// ==================

void AVC_Manager::GetRequiredValue( XML_NodePtr node, std::string &value )
{
	if ( node != 0 && node->content.size() != 0 )
		value = node->content[ 0 ]->value;
	else
		XMP_Throw( "AVCUltra_MetaHandler: Required field not present", kXMPErr_BadFileFormat );

}	// AVC_Manager::GetRequiredValue

// =================================================================================================
// AVC_Manager::GetOptionalValue
// ==================

void AVC_Manager::GetOptionalValue( XML_NodePtr node, std::string &value )
{
	if ( node != 0 && node->content.size() != 0 )
		value = node->content[0]->value;

}	// AVC_Manager::GetOptionalValue

// =================================================================================================
// AVC_Manager::GetAttributeValue
// ==================

void AVC_Manager::GetAttributeValue( XML_NodePtr node, XMP_Uns32 &value )
{
	if ( node != 0 )
	{
		std::istringstream stream( node->value );
		stream >> value;
	}
}	// AVC_Manager::GetAttributeValue

// =================================================================================================
// AVC_Manager::GetRequiredValue
// ==================

void AVC_Manager::GetRequiredValue( XML_NodePtr node, XMP_Uns32 &value )
{
	if ( node != 0 && node->content.size() != 0 )
	{
		std::istringstream stream( node->content[0]->value );
		stream >> value;
	}
	else
		XMP_Throw( "AVCUltra_MetaHandler: Required field not present", kXMPErr_BadFileFormat );

}	// AVC_Manager::GetRequiredValue

// =================================================================================================
// AVC_Manager::GetOptionalValue
// ==================

void AVC_Manager::GetOptionalValue( XML_NodePtr node, XMP_Uns32 &value )
{
	if ( node != 0 && node->content.size() != 0 )
	{
		std::istringstream stream( node->content[ 0 ]->value );
		stream >> value;
	}
}	// AVC_Manager::GetOptionalValue

// =================================================================================================
// AVC_Manager::GetOptionalValue
// ==================

void AVC_Manager::GetOptionalValue( XML_NodePtr node, XMP_Int32 &value )
{
	if ( node != 0 && node->content.size() != 0 )
	{
		std::istringstream stream( node->content[ 0 ]->value );
		stream >> value;
	}
}	// AVC_Manager::GetOptionalValue

// =================================================================================================
// AVC_Manager::ParseSpannedClips
// ==================
// Here we parse clips XML file and store their properties if they make up a shot
void AVC_Manager::ParseSpannedClips()
{
	XMP_VarString regExp;
	XMP_StringVector regExpVec;

	std::string tempPath = this->mainClip->clipPath;
	if ( tempPath.empty() )
		XMP_Throw( "AVC_Ultra: Clip's XML path was not set", kXMPErr_InternalFailure );

	std::string tempString;
	XIO::SplitFileExtension( &tempPath, &tempString );
	XIO::SplitLeafName( &tempPath, &tempString );

	XMP_Assert( tempString.length() == 8 );
	tempString = tempString.erase( 6 );

	regExp = tempString + "\\d\\d.XML$";
	regExpVec.push_back( regExp );

	std::vector<std::string> spannedClipList;
	IOUtils::GetMatchingChildren( spannedClipList, tempPath, regExpVec, false, true, false );

	for ( std::vector<std::string>::iterator iter = spannedClipList.begin(); iter != spannedClipList.end(); ++iter )
	{
		std::string fullPath = tempPath;
		fullPath += kDirChar;
		fullPath += ( *iter );
		if ( fullPath != this->mainClip->clipPath )
		{
			AVC_Clip * clip = new AVC_Clip();
			ExpatAdapter * expatAdapter = 0;
			XML_NodePtr node = 0;
			try
			{
				expatAdapter = XMP_NewExpatAdapter( ExpatAdapter::kUseLocalNamespaces );
				CacheClipContent( fullPath, clip, expatAdapter, node );
				this->spannedClips.push_back( clip );
				if ( expatAdapter != 0 )
				{
					delete expatAdapter;
					expatAdapter = 0;
				}
				node = 0;

			}
			catch ( ... )
			{
				if ( expatAdapter != 0 )
				{
					delete expatAdapter;
					expatAdapter = 0;
				}
				if ( clip != 0 )
				{
					delete clip;
					clip = 0;
				}
				node = 0;
				throw;
			}
		}
	}

}	// AVC_Manager::ParseSpannedClips

// =================================================================================================
// AVC_Manager::GetClip
// ==================
// Getting the main top clip if exists
AVC_Clip * AVC_Manager::GetClip()
{
	return this->mainClip;
}
// AVC_Manager::GetClip

// =================================================================================================
// AVC_Manager::GetRootNode
// ==================
// Retriving the root node for main clip
XML_NodePtr AVC_Manager::GetRootNode()
{
	return this->mainRootNode;

}// AVC_Manager::GetRootNode

// =================================================================================================
// AVC_Manager::CreateXMPUns32Digest
// ==================

void AVC_Manager::CreateXMPUns32Digest( MD5_CTX & context, XMP_Uns32 value )
{
	std::ostringstream stream;
	stream << value;
	if ( ! stream.str().empty() )
		MD5Update( &context, ( XMP_Uns8* ) stream.str().c_str(), ( unsigned int ) stream.str().length() );

}	// AVC_Manager::CreateXMPUns32Digest

#define kHexDigits "0123456789ABCDEF"

// =================================================================================================
// AVC_Manager::CreateClipDigest
// ==================

void AVC_Manager::CreateClipDigest( std::string &digestStr )
{
	digestStr.erase();

	const AVC_Clip * clip = this->mainClip;
	if ( clip == 0 )
		return;

	MD5_CTX    md5Context;
	unsigned char digestBin[ 16 ];
	MD5Init( &md5Context );

	if ( !clip->globalClipId.empty() )
		MD5Update( &md5Context, ( XMP_Uns8* ) clip->globalClipId.c_str(), ( unsigned int ) clip->globalClipId.length() );

	if ( !clip->editUnit.empty() )
		MD5Update( &md5Context, ( XMP_Uns8* ) clip->editUnit.c_str(), ( unsigned int ) clip->editUnit.length() );

	if ( clip->duration != 0 )
		this->CreateXMPUns32Digest( md5Context, clip->duration );

	// Relation elements
	if ( clip->relationStruct.isExists )
	{
		if ( !clip->relationStruct.globalShotId.empty() )
			MD5Update( &md5Context, ( XMP_Uns8* ) clip->relationStruct.globalShotId.c_str(), ( unsigned int ) clip->relationStruct.globalShotId.length() );
		if ( !clip->relationStruct.topClipId.empty() )
			MD5Update( &md5Context, ( XMP_Uns8* ) clip->relationStruct.topClipId.c_str(), ( unsigned int ) clip->relationStruct.topClipId.length() );
		if ( !clip->relationStruct.prevClipId.empty() )
			MD5Update( &md5Context, ( XMP_Uns8* ) clip->relationStruct.prevClipId.c_str(), ( unsigned int ) clip->relationStruct.prevClipId.length() );
		if ( !clip->relationStruct.nextClipId.empty() )
			MD5Update( &md5Context, ( XMP_Uns8* ) clip->relationStruct.nextClipId.c_str(), ( unsigned int ) clip->relationStruct.nextClipId.length() );
		this->CreateXMPUns32Digest( md5Context, clip->relationStruct.offsetInShot );
	}

	// Video Essence
	if ( !clip->videoEssence.codec.empty() )
		MD5Update( &md5Context, ( XMP_Uns8* ) clip->videoEssence.codec.c_str(), ( unsigned int ) clip->videoEssence.codec.length() );
	if ( clip->videoEssence.isClassExists )
		this->CreateXMPUns32Digest( md5Context, clip->videoEssence.classField );
	if ( clip->videoEssence.activeLine != 0 )
		this->CreateXMPUns32Digest( md5Context, clip->videoEssence.activeLine );
	if ( clip->videoEssence.activePixel != 0 )
		this->CreateXMPUns32Digest( md5Context, clip->videoEssence.activePixel );
	if ( !clip->videoEssence.frameRate.empty() )
		MD5Update( &md5Context, ( XMP_Uns8* ) clip->videoEssence.frameRate.c_str(), ( unsigned int ) clip->videoEssence.frameRate.length() );
	if ( !clip->videoEssence.startTimecode.empty() )
		MD5Update( &md5Context, ( XMP_Uns8* ) clip->videoEssence.startTimecode.c_str(), ( unsigned int ) clip->videoEssence.startTimecode.length() );
	if ( !clip->videoEssence.timecodeType.empty() )
		MD5Update( &md5Context, ( XMP_Uns8* ) clip->videoEssence.timecodeType.c_str(), ( unsigned int ) clip->videoEssence.timecodeType.length() );

	// Audio Essence
	if ( !clip->audioEssence.bitsPerSample.empty() )
		MD5Update( &md5Context, ( XMP_Uns8* ) clip->audioEssence.bitsPerSample.c_str(), ( unsigned int ) clip->audioEssence.bitsPerSample.length() );
	if ( !clip->audioEssence.samplingRate.empty() )
		MD5Update( &md5Context, ( XMP_Uns8* ) clip->audioEssence.samplingRate.c_str(), ( unsigned int ) clip->audioEssence.samplingRate.length() );

	// Clip Descriptive metadata
	if ( !clip->descMetadata.userClipName.empty() )
		MD5Update( &md5Context, ( XMP_Uns8* ) clip->descMetadata.userClipName.c_str(), ( unsigned int ) clip->descMetadata.userClipName.length() );
	if ( !clip->descMetadata.shotMark.empty() )
		MD5Update( &md5Context, ( XMP_Uns8* ) clip->descMetadata.shotMark.c_str(), ( unsigned int ) clip->descMetadata.shotMark.length() );
	if ( !clip->descMetadata.creator.empty() )
		MD5Update( &md5Context, ( XMP_Uns8* ) clip->descMetadata.creator.c_str(), ( unsigned int ) clip->descMetadata.creator.length() );
	if ( !clip->descMetadata.creationDate.empty() )
		MD5Update( &md5Context, ( XMP_Uns8* ) clip->descMetadata.creationDate.c_str(), ( unsigned int ) clip->descMetadata.creationDate.length() );
	if ( !clip->descMetadata.lastUpdateDate.empty() )
		MD5Update( &md5Context, ( XMP_Uns8* ) clip->descMetadata.lastUpdateDate.c_str(), ( unsigned int ) clip->descMetadata.lastUpdateDate.length() );
	if ( !clip->descMetadata.manufacture.empty() )
		MD5Update( &md5Context, ( XMP_Uns8* ) clip->descMetadata.manufacture.c_str(), ( unsigned int ) clip->descMetadata.manufacture.length() );
	if ( !clip->descMetadata.serialNum.empty() )
		MD5Update( &md5Context, ( XMP_Uns8* ) clip->descMetadata.serialNum.c_str(), ( unsigned int ) clip->descMetadata.serialNum.length() );
	if ( !clip->descMetadata.modelName.empty() )
		MD5Update( &md5Context, ( XMP_Uns8* ) clip->descMetadata.modelName.c_str(), ( unsigned int ) clip->descMetadata.modelName.length() );
	if ( !clip->descMetadata.shooter.empty() )
		MD5Update( &md5Context, ( XMP_Uns8* ) clip->descMetadata.shooter.c_str(), ( unsigned int ) clip->descMetadata.shooter.length() );
	if ( clip->descMetadata.isAltitudeExists )
		this->CreateXMPUns32Digest( md5Context, clip->descMetadata.altitude );
	if ( !clip->descMetadata.longitude.empty() )
		MD5Update( &md5Context, ( XMP_Uns8* ) clip->descMetadata.longitude.c_str(), ( unsigned int ) clip->descMetadata.longitude.length() );
	if ( !clip->descMetadata.latitude.empty() )
		MD5Update( &md5Context, ( XMP_Uns8* ) clip->descMetadata.latitude.c_str(), ( unsigned int ) clip->descMetadata.latitude.length() );
	if ( !clip->descMetadata.placeName.empty() )
		MD5Update( &md5Context, ( XMP_Uns8* ) clip->descMetadata.placeName.c_str(), ( unsigned int ) clip->descMetadata.placeName.length() );
	if ( clip->descMetadata.sceneNum != 0 )
		this->CreateXMPUns32Digest( md5Context, clip->descMetadata.sceneNum );
	if ( clip->descMetadata.takeNum != 0 )
		this->CreateXMPUns32Digest( md5Context, clip->descMetadata.takeNum );

	MD5Final( digestBin, &md5Context );

	char buffer[ 40 ];
	for ( int in = 0, out = 0; in < 16; in += 1, out += 2 ) {
		XMP_Uns8 byte = digestBin[ in ];
		buffer[ out ] = kHexDigits[ byte >> 4 ];
		buffer[ out + 1 ] = kHexDigits[ byte & 0xF ];
	}
	buffer[ 32 ] = 0;
	digestStr.append( buffer );

}	// AVC_Manager::CreateClipDigest

// =================================================================================================
// AVC_Manager::SerialiseNRTfile
// ==================

void AVC_Manager::SerialiseNRTfile( const std::string & filePath , bool doSafeUpdate )
{
	if ( this->mainClipAdapter == 0 )
		return;
	// Inserting XSI attribute which was removed while parsing the XML
	XML_NodePtr attrWithXSINamespace = new XML_Node( this->mainRootNode, "xsi:", kCDataNode );
	attrWithXSINamespace->value = "";
	attrWithXSINamespace->ns = "http://www.w3.org/2001/XMLSchema-instance";
	this->mainRootNode->attrs.insert( this->mainRootNode->attrs.begin(), attrWithXSINamespace );
	
	std::string buffer;
	this->mainClipAdapter->tree.Serialize( &buffer );

	Host_IO::FileRef hostRef = Host_IO::Open( filePath.c_str(), Host_IO::openReadWrite );
	if ( hostRef == Host_IO::noFileRef ) XMP_Throw( "Failure opening legacy AVC_Ultra's NRT XML file", kXMPErr_ExternalFailure );

	XMPFiles_IO origXML( hostRef, filePath.c_str(), Host_IO::openReadWrite );
	XIO::ReplaceTextFile( &origXML, buffer, ( doSafeUpdate ) );
	origXML.Close();

}	// AVC_Manager::SerialiseNRTfile

// =================================================================================================
// AVC_Manager::AVC_Manager
// ==================

AVC_Manager::~AVC_Manager()
{

	std::vector<AVC_Clip*>::iterator iter = this->spannedClips.begin();
	while ( iter != this->spannedClips.end() )
	{
		delete *iter;
		iter = this->spannedClips.erase( iter );
	}

	if ( this->mainClipAdapter != 0 )
	{
		delete ( this->mainClipAdapter );
		this->mainClipAdapter = 0;
	}
	if ( this->mainClip != 0 )
	{
		delete this->mainClip;
		this->mainClip = 0;
	}
	this->mainRootNode = 0;

}	// AVC_Manager::~AVC_Manager

