#ifndef __XDCAMSAM_Handler_hpp__
#define __XDCAMSAM_Handler_hpp__	1

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

#if AdobePrivate
// =================================================================================================
// Change history
// ==============
//
// Writers:
//	AB  Amit Bhatti
//
// mm-dd-yy who Description of changes, most recent on top
//
// 02-06-15 AB  5.6-f130 Redesign XDCAM Handler by dividing it into 2 child handlers XDCAMFAM_Handler and XDCAMSAM_Handler and making it more consistent as per latest spec.
//						 More history could be found at XDCAM_Handler.hpp.
//
// =================================================================================================
#endif // AdobePrivate

#include "public/include/XMP_Environment.h"	// ! This must be the first include.
#include "XMPFiles/source/FileHandlers/XDCAM_Handler.hpp"


extern XMPFileHandler * XDCAMSAM_MetaHandlerCTor ( XMPFiles * parent );

extern bool XDCAMSAM_CheckFormat ( XMP_FileFormat format,
								const std::string & rootPath,
								const std::string & groupName,
								const std::string & parentName,
								const std::string & leafName,
								XMPFiles * parent );

static const XMP_OptionBits kXDCAMSAM_HandlerFlags = (kXMPFiles_CanInjectXMP |
												   kXMPFiles_CanExpand |
												   kXMPFiles_CanRewrite |
												   kXMPFiles_PrefersInPlace |
												   kXMPFiles_CanReconcile |
												   kXMPFiles_AllowsOnlyXMP |
												   kXMPFiles_ReturnsRawPacket |
												   kXMPFiles_HandlerOwnsFile |
												   kXMPFiles_AllowsSafeUpdate |
												   kXMPFiles_FolderBasedFormat);

class XDCAMSAM_MetaHandler : public XDCAM_MetaHandler
{

public:
	
	void FillAssociatedResources ( std::vector<std::string> * resourceList );
	XDCAMSAM_MetaHandler ( XMPFiles * _parent );
	virtual ~XDCAMSAM_MetaHandler()	{ };

private:

	bool MakeClipFilePath ( std::string * path, XMP_StringPtr suffix, bool checkFile = false );
	void SetPathVariables ( const std::string & clientPath );
	bool GetEditInfoFiles ( std::vector<std::string> &editInfoList );
	bool GetClipUmid ( std::string &clipUmid );

	XDCAMSAM_MetaHandler() : XDCAM_MetaHandler() {};	// Hidden on purpose.

};	// XDCAMSAM_MetaHandler

// =================================================================================================
#endif /* __XDCAMSAM_Handler_hpp__ */
