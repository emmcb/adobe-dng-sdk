#ifndef __AVCHD_Handler_hpp__
#define __AVCHD_Handler_hpp__	1

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
//	JPM James Mork
//	AWL Alan Lillich
//  SKP Sunil Kishor Pathak
//  IJS Inder Jeet Singh
//	HK  Honey Kansal
//
// mm-dd-yy who Description of changes, most recent on top
//
// 01-16-13 HK 5.6-f022 Implemented IsMetadataWritable API for XDCAM-EX, P2, CanonXF and AVCHD formats.
// 01-03-13 IJS 5.6-f014 [3429815] Added GetAssociatedResources API for AVCHD format.
//
// 10-09-12 SKP 5.5-f044 Refactored LocateMetadataFiles API.
// 09-29-11 AWL 5.4-f015 Add AVCHD support for GetFileModDate.
//
// 08-17-10 AWL 5.3-f001 Integrate I/O revamp to main.
//
// 08-11-09 AWL 5.0-f061 Integrate P2 and AVCHD changes from Peter Lee.
// 01-29-09 AWL 5.0-f022 [1699977] Fixes for UNIX file case sensitivity.
//
// 02-22-08 AWL 4.2-f082 Fix XDCAM_CheckFormat to be case insensitive for PROAV. Remove the CUEUP.XML
//				check from XDCAMEX_CheckFormat. Fix handerTemp leaks for all folder-oriented handlers.
// 02-15-08 AWL 4.2-f075 Integrate more folder-oriented handler updates. Initial changes to create
//				generic UNIX builds for XMPFiles.
// 02-07-08 AWL 4.2-f070 Test, fix bugs, and move the advanced video handlers out of NewHandlers.
// 02-05-08 AWL 4.2-f069 Integrate latest advanced video handlers.
// 01-24-08 JPM Add initial AVCHD handler.
//
// =================================================================================================
#endif // AdobePrivate

#include "public/include/XMP_Environment.h"	// ! This must be the first include.

#include "XMPFiles/source/XMPFiles_Impl.hpp"

#include "source/ExpatAdapter.hpp"

// =================================================================================================
/// \file AVCHD_Handler.hpp
/// \brief Folder format handler for AVCHD.
///
/// This header ...
///
// =================================================================================================

extern XMPFileHandler * AVCHD_MetaHandlerCTor ( XMPFiles * parent );

extern bool AVCHD_CheckFormat ( XMP_FileFormat format,
								const std::string & rootPath,
								const std::string & gpName,
								const std::string & parentName,
								const std::string & leafName,
								XMPFiles * parent );

static const XMP_OptionBits kAVCHD_HandlerFlags = (kXMPFiles_CanInjectXMP |
												   kXMPFiles_CanExpand |
												   kXMPFiles_CanRewrite |
												   kXMPFiles_PrefersInPlace |
												   kXMPFiles_CanReconcile |
												   kXMPFiles_AllowsOnlyXMP |
												   kXMPFiles_ReturnsRawPacket |
												   kXMPFiles_HandlerOwnsFile |
												   kXMPFiles_AllowsSafeUpdate |
												   kXMPFiles_FolderBasedFormat);

class AVCHD_MetaHandler : public XMPFileHandler
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

	AVCHD_MetaHandler ( XMPFiles * _parent );
	virtual ~AVCHD_MetaHandler();

private:

	AVCHD_MetaHandler() {};	// Hidden on purpose.

	bool MakeClipInfoPath ( std::string * path, XMP_StringPtr suffix, bool checkFile = false ) const;
	bool MakeClipStreamPath ( std::string * path, XMP_StringPtr suffix, bool checkFile = false ) const;
	bool MakePlaylistPath ( std::string * path, XMP_StringPtr suffix, bool checkFile = false ) const;

	void MakeLegacyDigest ( std::string * digestStr );

	std::string rootPath, clipName;

};	// AVCHD_MetaHandler

// =================================================================================================

#endif /* __AVCHD_Handler_hpp__ */
