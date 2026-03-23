#ifndef __Generic_Handler_hpp__
#define __Generic_Handler_hpp__	1

// =================================================================================================
// Copyright Adobe
// Copyright 2014 Adobe
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
//	IJS Inder Jeet Singh
//
// mm-dd-yy who Description of changes, most recent on top
//
// 01-31-14 IJS 5.6-f092 Modified Generic Handler Serialize Options by adding kXMP_IncludeRDFHash as default flag
// 01-16-14 IJS 5.6-f088 Generic Handling Feature - Adding support for kXMPFiles_OpenUseGenericHandler OpenFile() flag.
//
// =================================================================================================
#endif // AdobePrivate

#include "XMPFiles/source/XMPFiles_Impl.hpp"

#if EnableGenericHandling
// =================================================================================================
/// \file Generic_MetaHandler.hpp
/// \brief File format handler for handling Unknown file format.
// =================================================================================================

extern XMPFileHandler * Generic_MetaHandlerCTor ( XMPFiles * parent );

static const XMP_OptionBits kGeneric_HandlerFlags = ( kXMPFiles_CanInjectXMP |
													kXMPFiles_CanExpand |
													kXMPFiles_CanRewrite |
													kXMPFiles_AllowsOnlyXMP |
													kXMPFiles_ReturnsRawPacket |
													kXMPFiles_HandlerOwnsFile |
													kXMPFiles_AllowsSafeUpdate |
													kXMPFiles_UsesSidecarXMP );;

class Generic_MetaHandler : public XMPFileHandler
{
public:

	Generic_MetaHandler () {};
	Generic_MetaHandler ( XMPFiles * parent );

	~Generic_MetaHandler();

	void FillAssociatedResources ( std::vector<std::string> * resourceList );
	bool IsMetadataWritable ( ) ;

	void CacheFileData();

	XMP_OptionBits GetSerializeOptions();
	void UpdateFile ( bool doSafeUpdate );
	void WriteTempFile ( XMP_IO * tempRef );
private:
	std::string genericPath;

};	// Generic_MetaHandler

// =================================================================================================

#endif //EnableGenericHandling


#endif /* __Generic_Handler_hpp__ */

