#ifndef __XDCAMEX_Handler_hpp__
#define __XDCAMEX_Handler_hpp__	1

// =================================================================================================
// Copyright Adobe
// Copyright 2008 Adobe
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
//	GEM Gerry Miller
//	AWL Alan Lillich
//  SKP Sunil Kishor Pathak
//  IJS Inder Jeet Singh
//	HK	Honey Kansal
//
// mm-dd-yy who Description of changes, most recent on top
//
// 01-16-13 HK 5.6-f022 Implemented IsMetadataWritable API for XDCAM-EX, P2, CanonXF and AVCHD formats.
// 01-02-13 HK 5.6-f010 [3429813] Added GetAssociatedResources API for XDCAM-EX format.
// 12-13-12 IJS 5.6-f003 [3424244] Fixed initialization of XDCAMEX Handler which was causing a crash.
//
// 10-09-12 SKP 5.5-f044 Refactored LocateMetadataFiles API.
// 09-30-11 AWL 5.4-f017 Add XDCAM (EX/FAM/SAM) support for GetFileModDate.
//
// 11-24-10	AWL	5.3-f020 [2755623,2755632] Add XDCAM fixes for non-XMP metadata problems.
// 08-17-10 AWL 5.3-f001 Integrate I/O revamp to main.
//
// 06-11-09 AWL 5.0-f043 Finish threading revamp, implement friendly reader/writer locking.
//
// 02-22-08 AWL 4.2-f082 Fix XDCAM_CheckFormat to be case insensitive for PROAV. Remove the CUEUP.XML
//				check from XDCAMEX_CheckFormat. Fix handerTemp leaks for all folder-oriented handlers.
// 02-07-08 AWL 4.2-f070 Test, fix bugs, and move the advanced video handlers out of NewHandlers.
// 02-05-08 AWL 4.2-f069 Integrate latest advanced video handlers.
// 01-11-08 GEM Add basic XDCAMEX handler.
//
// =================================================================================================
#endif // AdobePrivate

#include "public/include/XMP_Environment.h"	// ! This must be the first include.

#include "XMPFiles/source/XMPFiles_Impl.hpp"

#include "source/ExpatAdapter.hpp"

// =================================================================================================
/// \file XDCAMEX_Handler.hpp
/// \brief Folder format handler for XDCAMEX.
// =================================================================================================

extern XMPFileHandler * XDCAMEX_MetaHandlerCTor ( XMPFiles * parent );

extern bool XDCAMEX_CheckFormat ( XMP_FileFormat format,
								  const std::string & rootPath,
								  const std::string & gpName,
								  const std::string & parentName,
								  const std::string & leafName,
								  XMPFiles * parent );

static const XMP_OptionBits kXDCAMEX_HandlerFlags = (kXMPFiles_CanInjectXMP |
													 kXMPFiles_CanExpand |
													 kXMPFiles_CanRewrite |
													 kXMPFiles_PrefersInPlace |
													 kXMPFiles_CanReconcile |
													 kXMPFiles_AllowsOnlyXMP |
													 kXMPFiles_ReturnsRawPacket |
													 kXMPFiles_HandlerOwnsFile |
													 kXMPFiles_AllowsSafeUpdate |
													 kXMPFiles_FolderBasedFormat);

class XDCAMEX_MetaHandler : public XMPFileHandler
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

	XDCAMEX_MetaHandler ( XMPFiles * _parent );
	virtual ~XDCAMEX_MetaHandler();

private:

	XDCAMEX_MetaHandler() : expat(0), clipMetadata(0) {};	// Hidden on purpose.

	bool MakeClipFilePath ( std::string * path, XMP_StringPtr suffix, bool checkFile = false );
	bool MakeMediaproPath ( std::string * path, bool checkFile = false );
	void MakeLegacyDigest ( std::string * digestStr );

	void GetTakeUMID ( const std::string& clipUMID, std::string& takeUMID, std::string& takeXMLURI );
	void GetTakeDuration ( const std::string& takeUMID, std::string& duration );
	bool GetMediaProMetadata ( SXMPMeta * xmpObjPtr, const std::string& clipUMID, bool digestFound );

	void CleanupLegacyXML();

	std::string rootPath, clipName, xdcNS, legacyNS, clipUMID;

	// Used to Parse the Non-XMP /non real time metadata file associated with the clip 
	ExpatAdapter * expat;
	XML_Node * clipMetadata;

};	// XDCAMEX_MetaHandler

// =================================================================================================

#endif /* __XDCAMEX_Handler_hpp__ */
