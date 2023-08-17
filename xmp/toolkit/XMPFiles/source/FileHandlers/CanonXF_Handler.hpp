#if AdobePrivate	// ! This entire handler is private, it must not go in the public SDK.

#ifndef __CanonXF_Handler_hpp__
#define __CanonXF_Handler_hpp__	1

// =================================================================================================
// Copyright Adobe
// Copyright 2010 Adobe
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
//	PEL Peter Lee
//  SKP Sunil Kishor Pathak
//  IJS Inder Jeet Singh
//	HK  Honey Kansal
//
// mm-dd-yy who Description of changes, most recent on top
//
// 01-16-13 HK 5.6-f022 Implemented IsMetadataWritable API for XDCAM-EX, P2, CanonXF and AVCHD formats.
// 01-03-13 IJS 5.6-f015 [3429812] Added GetAssociatedResources API for CononXF format.
//
// 10-09-12 SKP 5.5-f044 Refactored LocateMetadataFiles API.
// 09-28-12 AWL 5.5-f039 Add LocateMetadataFiles, correct handler flags.
//
// 09-29-11 AWL 5.4-f015 Add AVCHD and CanonXF support for GetFileModDate.
//
// 09-30-10	AWL	5.3-f010 Add Canon XF handler from Peter Lee.
// 05-03-10 PEL Initial implementation based on P2 and AVCHD handlers.
//
// =================================================================================================
#endif

#include "public/include/XMP_Environment.h"	// ! This must be the first include.

#include "XMPFiles/source/XMPFiles_Impl.hpp"

#include "source/ExpatAdapter.hpp"

#include "third-party/zuid/interfaces/MD5.h"

// =================================================================================================
/// \file CanonXF_Handler.hpp
/// \brief Folder format handler for CanonXF.
///
/// This header ...
///
// =================================================================================================

extern XMPFileHandler * CanonXF_MetaHandlerCTor ( XMPFiles * parent );

extern bool CanonXF_CheckFormat ( XMP_FileFormat format,
								  const std::string & rootPath,
								  const std::string & gpName,
								  const std::string & parentName,
								  const std::string & leafName,
								  XMPFiles * parent );

static const XMP_OptionBits kCanonXF_HandlerFlags = (kXMPFiles_CanInjectXMP |
													 kXMPFiles_CanExpand |
													 kXMPFiles_CanRewrite |
													 kXMPFiles_PrefersInPlace |
													 kXMPFiles_CanReconcile |
													 kXMPFiles_AllowsOnlyXMP |
													 kXMPFiles_ReturnsRawPacket |
													 kXMPFiles_HandlerOwnsFile |
													 kXMPFiles_AllowsSafeUpdate |
													 kXMPFiles_FolderBasedFormat);

class CanonXF_MetaHandler : public XMPFileHandler
{
public:

	bool GetFileModDate ( XMP_DateTime * modDate );
	void FillMetadataFiles ( std::vector<std::string> * metadataFiles );
    void FillAssociatedResources ( std::vector<std::string> * resourceList );
	bool IsMetadataWritable ( );

	void CacheFileData();
	void ProcessXMP();

	void UpdateFile ( bool doSafeUpdate );
    void WriteTempFile ( XMP_IO* tempRef );

	XMP_OptionBits GetSerializeOptions()	// *** These should be standard for standalone XMP files.
		{ return (kXMP_UseCompactFormat | kXMP_OmitPacketWrapper); };

	CanonXF_MetaHandler ( XMPFiles * _parent );
	virtual ~CanonXF_MetaHandler();

private:

	CanonXF_MetaHandler() : expat(0), videoClipElement(0) {};	// Hidden on purpose.

	bool MakeClipFilePath ( std::string * path, XMP_StringPtr suffix, bool checkFile = false );
	void MakeLegacyDigest ( std::string * digestStr );
	void CleanupLegacyXML();

	bool ReadClipInfoFile ( bool digestFound );
	bool ReadClipXMLFile ( const std::string& xmlPath, bool digestFound );

	void DigestLegacyItem ( MD5_CTX & md5Context, XML_NodePtr legacyContext, XMP_StringPtr legacyPropName );
	void DigestLegacyRelations ( MD5_CTX & md5Context );

	void SetXMPPropertyFromLegacyXML ( bool digestFound,
									   XML_NodePtr legacyContext,
									   XMP_StringPtr schemaNS,
									   XMP_StringPtr propName,
									   XMP_StringPtr legacyPropName,
									   bool isLocalized );

	void SetGPSPropertyFromLegacyXML  ( XML_NodePtr legacyLocationContext, bool digestFound, XMP_StringPtr propName, XMP_StringPtr legacyPropName );
	void SetAltitudeFromLegacyXML  ( XML_NodePtr legacyLocationContext, bool digestFound );

	XML_Node * ForceChildElement ( XML_Node * parent, XMP_StringPtr localName, int indent = 0 );

	std::string rootPath, clipName, canonXFNS;

	ExpatAdapter * expat;
	XML_Node * videoClipElement;		// ! Don't delete, points into the Expat tree.

};	// CanonXF_MetaHandler

// =================================================================================================

#endif // __CanonXF_Handler_hpp__

#endif	// AdobePrivate	// ! This entire handler is private, it must not go in the public SDK.
