#ifndef __AVC_Ultra_Support_hpp__
#define __AVC_Ultra_Support_hpp__ 1

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

#include "public/include/XMP_Environment.h"
#include "public/include/XMP_Const.h"

#include "XMPFiles/source/XMPFiles_Impl.hpp"
#include "source/XMLParserAdapter.hpp"
#include "source/ExpatAdapter.hpp"
#include "third-party/zuid/interfaces/MD5.h"
#include "source/XIO.hpp"
#include <vector>

// IsNum
static XMP_Bool IsNum( const XMP_Uns8 & ch )
{
	if ( ch >= '0' && ch <= '9' )
		return true;
	else
		return false;
}	// IsNum

// IsUpperAlpha
static XMP_Bool IsUpperAlpha( const XMP_Uns8 & ch )
{
	if ( ch >= 'A' && ch <= 'Z' )
		return true;
	else
		return false;
}	// IsUpperAlpha

// RelationStruct
struct RelationStruct
{
	RelationStruct() : offsetInShot( 0 ), isExists( false ) { }

	XMP_Uns32   offsetInShot;
	std::string globalShotId;

	std::string topClipId;
	std::string prevClipId;
	std::string nextClipId;

	bool isExists;
};	// RelationStruct

// VideoEssence
struct VideoEssence
{
	VideoEssence() : activeLine( 0 ), activePixel( 0 ), startTimecodePtr( 0 ), classField( 0 ), isClassExists( false ) { }

	std::string codec;
	XMP_Uns32 classField;
	bool isClassExists;

	XMP_Uns32 activeLine;	// closed choice
	XMP_Uns32 activePixel;	// closed choice
	std::string frameRate;
	std::string timecodeType;
	XML_NodePtr startTimecodePtr;
	std::string startTimecode;

};	// VideoEssence

// AudioEssence
struct AudioEssence
{
	std::string samplingRate;
	std::string bitsPerSample;

};	// AudioEssence

// DescMetadata
struct DescMetadata
{
	DescMetadata() : sceneNum( 0 ), takeNum( 0 ), altitude( 0 ), accessPtr( 0 ), isAltitudeExists( false ) { }

	std::string userClipName;

	std::string shotMark;

	std::string creator;
	XML_NodePtr accessPtr;
	std::string creationDate;
	std::string lastUpdateDate;
	
	std::string manufacture;
	std::string serialNum;
	std::string modelName;
	
	std::string shooter;
	std::string placeName;
	std::string longitude;
	std::string latitude;
	XMP_Int32 altitude;
	bool isAltitudeExists;

	// Non-Zero
	XMP_Uns32 sceneNum;
	// Non-Zero
	XMP_Uns32 takeNum;

};	// DescMetadata

//	AVC_Clip
class AVC_Clip
{
public:
	AVC_Clip() : duration( 0 )	{}

	std::string clipPath;

	std::string globalClipId;
	XMP_Uns32   duration;	// Non-zero
	std::string editUnit;

	RelationStruct relationStruct;
	VideoEssence videoEssence;
	AudioEssence audioEssence;
	DescMetadata descMetadata;

};	// AVC_Clip

//	AVC_Manager
class AVC_Manager
{

#ifdef _ENABLE_XMPFILE_UNIT_TEST
	friend class AVCUltra_UnitTest;
#endif // _ENABLE_XMPFILE_UNIT_TEST

public:
	AVC_Manager( const std::string &origClipXML );
	~AVC_Manager();

	std::vector<AVC_Clip *> spannedClips;
	void ParseSpannedClips();
	AVC_Clip * GetClip();
	XML_NodePtr GetRootNode();
	static XMP_Bool IsClipSpanned( std::string & topClip );
	void CreateClipDigest( std::string &digestStr );
	void SerialiseNRTfile( const std::string & filePath, bool doSafeUpdate );

private:
	void CacheClipContent( const std::string &origClipXML, AVC_Clip * clipToFill, ExpatAdapter * xmlAdapter, XML_NodePtr &rootNode );
	
	void GetRequiredValue( XML_NodePtr node, std::string &value );
	void GetOptionalValue( XML_NodePtr node, std::string &value );
	void GetRequiredValue( XML_NodePtr node, XMP_Uns32 &value );
	void GetOptionalValue( XML_NodePtr node, XMP_Uns32 &value );
	void GetOptionalValue( XML_NodePtr node, XMP_Int32 &value );
	void GetAttributeValue( XML_NodePtr node, XMP_Uns32 &value );

	void CreateXMPUns32Digest( MD5_CTX & context, XMP_Uns32 value );

	ExpatAdapter *mainClipAdapter;
	XML_NodePtr mainRootNode;
	AVC_Clip * mainClip;

};	// AVC_Manager

#endif	// __AVC_Ultra_Support_hpp__

