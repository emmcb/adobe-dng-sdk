#ifndef __ASF_Handler_hpp__
#define __ASF_Handler_hpp__	1

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
//  IJS Inder Jeet Singh
//
// mm-dd-yy who Description of changes, most recent on top
//
// 09-20-12 IJS 5.5-f035 Add Progress Notification flag to MPEG-4,PSD,AIFF,WAVE,ASF,TIFF,FLV and PS Handlers.
// 08-19-10 AWL 5.3-f003 Remove all use of the LFA_* names.
// 08-17-10 AWL 5.3-f001 Integrate I/O revamp to main.
//
// 10-13-08 AWL 4.4-f013 Remove internals of GetThumbnail.
//
// 09-12-06 ALB Initial creation.
//
// =================================================================================================
#endif // AdobePrivate

#include "XMPFiles/source/XMPFiles_Impl.hpp"
#include "XMPFiles/source/FormatSupport/ASF_Support.hpp"

// =================================================================================================
/// \file ASF_Handler.hpp
/// \brief File format handler for ASF.
///
/// This header ...
///
// =================================================================================================

// *** Could derive from Basic_Handler - buffer file tail in a temp file.

extern XMPFileHandler* ASF_MetaHandlerCTor ( XMPFiles* parent );

extern bool ASF_CheckFormat ( XMP_FileFormat format,
							  XMP_StringPtr  filePath,
                              XMP_IO*    fileRef,
                              XMPFiles *     parent );

static const XMP_OptionBits kASF_HandlerFlags = ( kXMPFiles_CanInjectXMP |
												  kXMPFiles_CanExpand |
												  kXMPFiles_PrefersInPlace |
                                                  kXMPFiles_CanReconcile |
												  kXMPFiles_AllowsOnlyXMP |
												  kXMPFiles_ReturnsRawPacket |
												  kXMPFiles_NeedsReadOnlyPacket |
												  kXMPFiles_CanNotifyProgress );

class ASF_MetaHandler : public XMPFileHandler
{
public:

	void CacheFileData();
	void ProcessXMP();

	void UpdateFile ( bool doSafeUpdate );
    void WriteTempFile ( XMP_IO* tempRef );

	bool SafeWriteFile ();

	ASF_MetaHandler ( XMPFiles* parent );
	virtual ~ASF_MetaHandler();

private:

	ASF_LegacyManager legacyManager;

};	// ASF_MetaHandler

// =================================================================================================

#endif /* __ASF_Handler_hpp__ */
