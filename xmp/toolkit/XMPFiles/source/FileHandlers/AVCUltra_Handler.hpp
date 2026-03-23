#ifndef __AVCUltra_Handler_hpp__
#define __AVCUltra_Handler_hpp__	1

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

#include "public/include/XMP_Environment.h"	// ! This must be the first include.
#include "XMPFiles/source/XMPFiles_Impl.hpp"
#include "source/ExpatAdapter.hpp"
#include "XMPFiles/source/FormatSupport/AVCUltra_Support.hpp"

extern XMPFileHandler * AVCUltra_MetaHandlerCTor( XMPFiles * parent );

extern bool AVCUltra_CheckFormat( XMP_FileFormat format,
	const std::string & rootPath,
	const std::string & gpName,
	const std::string & parentName,
	const std::string & leafName,
	XMPFiles * parent );

static const XMP_OptionBits kAVCUltra_HandlerFlags = ( kXMPFiles_CanInjectXMP |
	kXMPFiles_CanExpand |
	kXMPFiles_CanRewrite |
	kXMPFiles_PrefersInPlace |
	kXMPFiles_CanReconcile |
	kXMPFiles_AllowsOnlyXMP |
	kXMPFiles_ReturnsRawPacket |
	kXMPFiles_HandlerOwnsFile |
	kXMPFiles_AllowsSafeUpdate |
	kXMPFiles_FolderBasedFormat );

class AVCUltra_MetaHandler : public XMPFileHandler
{

#ifdef _ENABLE_XMPFILE_UNIT_TEST
	friend class AVCUltra_UnitTest;
#endif // _ENABLE_XMPFILE_UNIT_TEST

public:

	void FillAssociatedResources( std::vector<std::string> * resourceList );
	bool IsMetadataWritable();

	void CacheFileData();
	void ProcessXMP();

	void UpdateFile( bool doSafeUpdate );
	void WriteTempFile( XMP_IO* tempRef );

	XMP_OptionBits GetSerializeOptions()	// *** These should be standard for standalone XMP files.
	{
		return ( kXMP_UseCompactFormat | kXMP_OmitPacketWrapper );
	};

	AVCUltra_MetaHandler( XMPFiles * _parent );
	virtual ~AVCUltra_MetaHandler();

private:

	AVCUltra_MetaHandler() {};	// Hidden on purpose.

	void CreatePseudoClipPath( const std::string & clientPath );
	void CreateSidecarPaths();
	bool enableBackwardCompatibility();

	void ImportFrameSize();
	void ImportCodecInfo();
	void ImportStartTimecode();
	void ImportDescMetadata();
	void ImportRelationMetadata();
	void ImportAudioMetadata();
	void ImportLegacyGPSProp( XMP_StringPtr ns, XMP_StringPtr elementName, const std::string &value );
	void ImportLegacyXMLProp( XMP_StringPtr ns, XMP_StringPtr elementName, const std::string &value );
	void ImportLegacyXMLProp( XMP_StringPtr ns, XMP_StringPtr elementName, const XMP_Uns32 &value );
	void ImportLegacyXMLStructProp( XMP_StringPtr schemaNS, XMP_StringPtr structName, XMP_StringPtr fieldNS, XMP_StringPtr fieldName, const std::string &value );
	void ImportLegacyXMLStructProp( XMP_StringPtr schemaNS, XMP_StringPtr structName, XMP_StringPtr fieldNS, XMP_StringPtr fieldName, const XMP_Uns32 &value );
	void ImportLegacyXMLArray( XMP_StringPtr ns, XMP_StringPtr elementName, const std::string &value, XMP_Bool requireDelete = false );

	void ForceLegacyProp( XML_NodePtr parent, XMP_StringPtr localName, XMP_StringPtr value, XMP_Int32 indent, XMP_Bool insertAtFront = false );
	void AdjustTimeCode( std::string & startTimecode, const XMP_Bool & isXMPtoXMLConversion );

	AVC_Manager * avcManager;
	XMP_Bool isSpannedClip;
	XMP_Bool isDigestExists;

	std::string rootPath, clipName, mNRTPath, mXMPPath, mReadXMPPath;

};	// AVCUltra_MetaHandler

#endif /* __AVCUltra_Handler_hpp__ */

// =================================================================================================

