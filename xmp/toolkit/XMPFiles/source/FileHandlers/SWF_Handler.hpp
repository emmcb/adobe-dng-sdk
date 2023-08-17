#ifndef __SWF_Handler_hpp__
#define __SWF_Handler_hpp__	1

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
//  PKA Patrick Kamin
//
// mm-dd-yy who Description of changes, most recent on top
//
// 10-10-11 AWL 5.4-f019 [2831943] Fix SWF handler bugs causing it to further corrupt ill-formed files.
//
// 08-18-10 AWL 5.3-f002 Don't include XIO.hpp in any headers, only .cpp files.
// 08-17-10 AWL 5.3-f001 Integrate I/O revamp to main.
//
// 10-13-08 AWL 4.4-f013 Remove internals of GetThumbnail.
//
// 02-13-08 PKA 4.2-f074 [1668559] Added consideration of file size to compressed SWF handling.
// 12-13-07 PKA 4.2-f046 Fixed mishandling of existing metadata tags in SWF files without file attribute tag.
// 11-14-07 PKA 4.2-f036 Add multiple metadata tag support to Swf Handler. Write XMP tag at the beginning of the file.
//				SWF handler moved out of NewHandler section
// 10-16-07 PKA	4.2-f029 Added compressed swf support to SWF handler. Considering of HasMetadata flag.
//
// =================================================================================================
#endif // AdobePrivate

#include "public/include/XMP_Environment.h"	// ! XMP_Environment.h must be the first included header.

#include "public/include/XMP_Const.h"
#include "public/include/XMP_IO.hpp"

#include "XMPFiles/source/XMPFiles_Impl.hpp"
#include "XMPFiles/source/FormatSupport/SWF_Support.hpp"

// =================================================================================================
/// \file SWF_Handler.hpp
/// \brief File format handler for SWF.
///
/// This header ...
///
// =================================================================================================

extern XMPFileHandler* SWF_MetaHandlerCTor ( XMPFiles* parent );

extern bool SWF_CheckFormat ( XMP_FileFormat format,
							  XMP_StringPtr  filePath,
                              XMP_IO *       fileRef,
                              XMPFiles *     parent );

static const XMP_OptionBits kSWF_HandlerFlags = ( kXMPFiles_CanInjectXMP |
												  kXMPFiles_CanExpand |
												  kXMPFiles_PrefersInPlace |
												  kXMPFiles_AllowsOnlyXMP |
												  kXMPFiles_ReturnsRawPacket );

class SWF_MetaHandler : public XMPFileHandler {

public:

	void CacheFileData();
	void ProcessXMP();

	void UpdateFile ( bool doSafeUpdate );
    void WriteTempFile ( XMP_IO* tempRef );

	XMP_OptionBits GetSerializeOptions();

	SWF_MetaHandler ( XMPFiles* parent );
	virtual ~SWF_MetaHandler();
	
private:

	SWF_MetaHandler() : isCompressed(false), hasFileAttributes(false), hasMetadata(false), brokenSWF(false),
						expandedSize(0), firstTagOffset(0) {};

	bool isCompressed, hasFileAttributes, hasMetadata, brokenSWF;
	XMP_Uns32 expandedSize, firstTagOffset;
	RawDataBlock expandedSWF;
	
	SWF_IO::TagInfo fileAttributesTag, metadataTag;

};	// SWF_MetaHandler

// =================================================================================================

#endif /* __SWF_Handler_hpp__ */
