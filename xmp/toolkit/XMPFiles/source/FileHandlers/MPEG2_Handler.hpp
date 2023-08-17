#ifndef __MPEG2_Handler_hpp__
#define __MPEG2_Handler_hpp__	1

// =================================================================================================
// Copyright Adobe
// Copyright 2005 Adobe
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
//	SDS Sankar Dey Sarkar
//	AWL Alan Lillich
//  SKP Sunil Kishor Pathak
//
// mm-dd-yy who Description of changes, most recent on top
//
// 02-21-13 HK  5.6-f041 [3503922] Support GetAssociatedResources and IsMetadataWritable API's for MPEG2.
// 10-09-12 SKP 5.5-f044 Refactored LocateMetadataFiles API.
// 09-27-11 AWL 5.4-f014 Add MPEG-2 support for GetFileModDate.
//
// 08-17-10 AWL 5.3-f001 Integrate I/O revamp to main.
//
// 11-13-07 AWL 4.2-f035 Add MPEG-4 skeleton handler. Rename MPEG handler to MPEG-2.
//
// 09-15-06 AWL 4.1-f035 Revamp the MPEG2 handler to be platform neutral.
//
// 04-07-06 AWL 4.0-f002 Add XMPFiles::GetThumbnail. Change XMPFiles::OpenFile to close the disk file
//				early for read-only access. Change XMPFileHandler::ExtractXMP to CacheFileData.
// 03-24-06 AWL 4.0-f001 Adapt for move to ham-perforce, integrate XMPFiles, bump version to 4.
//
// 03-29-05 SDS First draft.
//
// =================================================================================================
#endif // AdobePrivate

#include "public/include/XMP_Environment.h"	// ! XMP_Environment.h must be the first included header.

#include "public/include/XMP_Const.h"
#include "public/include/XMP_IO.hpp"

#include "XMPFiles/source/XMPFiles_Impl.hpp"

// =================================================================================================
/// \file MPEG2_Handler.hpp
/// \brief File format handler for MPEG2.
///
/// This header ...
///
// =================================================================================================

extern XMPFileHandler * MPEG2_MetaHandlerCTor ( XMPFiles * parent );

extern bool MPEG2_CheckFormat ( XMP_FileFormat format,
							    XMP_StringPtr  filePath,
							    XMP_IO *       fileRef,
							    XMPFiles *     parent);

static const XMP_OptionBits kMPEG2_HandlerFlags = ( kXMPFiles_CanInjectXMP |
													kXMPFiles_CanExpand |
													kXMPFiles_CanRewrite |
													kXMPFiles_AllowsOnlyXMP |
													kXMPFiles_ReturnsRawPacket |
													kXMPFiles_HandlerOwnsFile |
													kXMPFiles_AllowsSafeUpdate |
													kXMPFiles_UsesSidecarXMP );

class MPEG2_MetaHandler : public XMPFileHandler
{
public:

	std::string sidecarPath;

	MPEG2_MetaHandler ( XMPFiles * parent );
	~MPEG2_MetaHandler();

	bool GetFileModDate ( XMP_DateTime * modDate );
	void FillAssociatedResources ( std::vector<std::string> * resourceList );
	bool IsMetadataWritable ( ) ;

	void CacheFileData();

	void UpdateFile ( bool doSafeUpdate );
    void WriteTempFile ( XMP_IO * tempRef );


};	// MPEG2_MetaHandler

// =================================================================================================

#endif /* __MPEG2_Handler_hpp__ */
