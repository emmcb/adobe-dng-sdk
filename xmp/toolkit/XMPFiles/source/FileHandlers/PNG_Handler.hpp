#ifndef __PNG_Handler_hpp__
#define __PNG_Handler_hpp__	1

// =================================================================================================
// Copyright Adobe
// Copyright 2006 Adobe
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
//	ALB Alexander Born
//
// mm-dd-yy who Description of changes, most recent on top
//
// 08-18-10 AWL 5.3-f002 Don't include XIO.hpp in any headers, only .cpp files.
// 08-17-10 AWL 5.3-f001 Integrate I/O revamp to main.
//
// 10-13-08 AWL 4.4-f013 Remove internals of GetThumbnail.
//
// 07-02-06 ALB Initial creation.
// 07-19-06 ALB Fixes and endian support for MacTel.
//
// =================================================================================================
#endif // AdobePrivate

#include "public/include/XMP_Environment.h"	// ! XMP_Environment.h must be the first included header.
#include "public/include/XMP_Const.h"

#include "XMPFiles/source/FormatSupport/PNG_Support.hpp"
#include "source/XIO.hpp"

// =================================================================================================
/// \file PNG_Handler.hpp
/// \brief File format handler for PNG.
///
/// This header ...
///
// =================================================================================================

// *** Could derive from Basic_Handler - buffer file tail in a temp file.

extern XMPFileHandler* PNG_MetaHandlerCTor ( XMPFiles* parent );

extern bool PNG_CheckFormat ( XMP_FileFormat format,
							  XMP_StringPtr  filePath,
                              XMP_IO *       fileRef,
                              XMPFiles *     parent );

static const XMP_OptionBits kPNG_HandlerFlags = ( kXMPFiles_CanInjectXMP |
												  kXMPFiles_CanExpand |
												  kXMPFiles_PrefersInPlace |
												  kXMPFiles_AllowsOnlyXMP |
												  kXMPFiles_ReturnsRawPacket |
												  kXMPFiles_NeedsReadOnlyPacket );

class PNG_MetaHandler : public XMPFileHandler
{
public:

	void CacheFileData();
	void ProcessXMP();

	void UpdateFile ( bool doSafeUpdate );
    void WriteTempFile ( XMP_IO* tempRef );

	bool SafeWriteFile();

	PNG_MetaHandler ( XMPFiles* parent );
	virtual ~PNG_MetaHandler();

};	// PNG_MetaHandler

// =================================================================================================

#endif /* __PNG_Handler_hpp__ */
