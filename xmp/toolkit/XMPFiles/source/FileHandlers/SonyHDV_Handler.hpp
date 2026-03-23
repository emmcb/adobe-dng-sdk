#ifndef __SonyHDV_Handler_hpp__
#define __SonyHDV_Handler_hpp__	1

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
//	AWL Alan Lillich
//  SKP Sunil Kishor Pathak
//
// mm-dd-yy who Description of changes, most recent on top
//
// 01-17-13 HK  5.6-f023 Implemented GetAssociatedResources and IsMetadataWritable APIs for Sony-HDV format.
// 10-09-12 SKP 5.5-f044 Refactored LocateMetadataFiles API.
// 09-30-11 AWL 5.4-f016 Add P2 and SonyHDV support for GetFileModDate.
//
// 08-17-10 AWL 5.3-f001 Integrate I/O revamp to main.
//
// 04-14-08 AWL 4.2.2-f106 [1741260] Integrate updates to AVCHD, AVI, MOV, Sony HDV, and WAV.
//
// 02-22-08 AWL 4.2-f082 Fix XDCAM_CheckFormat to be case insensitive for PROAV. Remove the CUEUP.XML
//				check from XDCAMEX_CheckFormat. Fix handerTemp leaks for all folder-oriented handlers.
// 02-15-08 AWL 4.2-f075 Integrate more folder-oriented handler updates. Initial changes to create
//				generic UNIX builds for XMPFiles.
// 02-07-08 AWL 4.2-f070 Test, fix bugs, and move the advanced video handlers out of NewHandlers.
// 01-16-08 AWL 4.2-f057 Remove private parent pointer from MPEG-4, P2, SonyHDV, and XDCAM. Like all
//				handlers they must use the parent pointer in the XMPFileHandler base class.
// 12-31-07 AWL 4.2-f051 Add initial Sony HDV handler under NewHandlers.
//
// =================================================================================================
#endif // AdobePrivate

#include "public/include/XMP_Environment.h"	// ! This must be the first include.

#include "XMPFiles/source/XMPFiles_Impl.hpp"

#include "source/ExpatAdapter.hpp"

// =================================================================================================
/// \file SonyHDV_Handler.hpp
/// \brief Folder format handler for SonyHDV.
///
/// This header ...
///
// =================================================================================================

extern XMPFileHandler * SonyHDV_MetaHandlerCTor ( XMPFiles * parent );

extern bool SonyHDV_CheckFormat ( XMP_FileFormat format,
								  const std::string & rootPath,
								  const std::string & gpName,
								  const std::string & parentName,
								  const std::string & leafName,
								  XMPFiles * parent );

static const XMP_OptionBits kSonyHDV_HandlerFlags = (kXMPFiles_CanInjectXMP |
													 kXMPFiles_CanExpand |
													 kXMPFiles_CanRewrite |
													 kXMPFiles_PrefersInPlace |
													 kXMPFiles_CanReconcile |
													 kXMPFiles_AllowsOnlyXMP |
													 kXMPFiles_ReturnsRawPacket |
													 kXMPFiles_HandlerOwnsFile |
													 kXMPFiles_AllowsSafeUpdate |
													 kXMPFiles_FolderBasedFormat);

class SonyHDV_MetaHandler : public XMPFileHandler
{
public:

	bool GetFileModDate ( XMP_DateTime * modDate );
	void FillMetadataFiles(std::vector<std::string>* metadataFiles );
	void FillAssociatedResources ( std::vector<std::string> * resourceList );
	bool IsMetadataWritable ( );

	void CacheFileData();
	void ProcessXMP();

	void UpdateFile ( bool doSafeUpdate );
    void WriteTempFile ( XMP_IO* tempRef );

	XMP_OptionBits GetSerializeOptions()	// *** These should be standard for standalone XMP files.
		{ return (kXMP_UseCompactFormat | kXMP_OmitPacketWrapper); };

	SonyHDV_MetaHandler ( XMPFiles * _parent );
	virtual ~SonyHDV_MetaHandler();

private:

	SonyHDV_MetaHandler() {};	// Hidden on purpose.

	bool MakeClipFilePath ( std::string * path, XMP_StringPtr suffix, bool checkFile = false );
	bool MakeIndexFilePath ( std::string& idxPath, const std::string& rootPath, const std::string& leafName );
	void MakeLegacyDigest ( std::string * digestStr );

	std::string rootPath, clipName;

};	// SonyHDV_MetaHandler

// =================================================================================================

#endif /* __SonyHDV_Handler_hpp__ */
