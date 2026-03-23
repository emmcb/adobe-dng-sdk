#ifndef __Trivial_Handler_hpp__
#define __Trivial_Handler_hpp__	1

// =================================================================================================
// Copyright Adobe
// Copyright 2004 Adobe
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
//
// mm-dd-yy who Description of changes, most recent on top
//
// 08-17-10 AWL 5.3-f001 Integrate I/O revamp to main.
//
// 04-07-06 AWL 4.0-f002 Add XMPFiles::GetThumbnail. Change XMPFiles::OpenFile to close the disk file
//				early for read-only access. Change XMPFileHandler::ExtractXMP to CacheFileData.
// 03-24-06 AWL 4.0-f001 Adapt for move to ham-perforce, integrate XMPFiles, bump version to 4.
//
// 09-01-05 AWL 1.3-003 [1225858] Don't implicitly set the SafeUpdate flag for non-owning handlers.
//
// 01-06-05 AWL 1.0-027 [1014854] Cleanup handling for UTF-16 and UTF-32. This involves some general
//				changes to the separation of work between the common code and handlers, making the
//				storage of the SXMPMeta object and packet string more visible.
// 10-07-04 AWL 1.0-009 Add first cut of support for crash safe file update.
// 08-13-04 AWL Checkpoint, almost full implementation, compiles.
// 08-12-04 AWL First draft.
//
// =================================================================================================
#endif // AdobePrivate

#include "XMPFiles/source/XMPFiles_Impl.hpp"

// =================================================================================================
/// \file Trivial_Handler.hpp
/// \brief Base class for trivial handlers that only process in-place XMP.
///
/// This header ...
///
/// \note There is no general promise here about crash-safe I/O. An update to an existing file might
/// have invalid partial state while rewriting existing XMP in-place. Crash-safe updates are managed
/// at a higher level of XMPFiles, using a temporary file and final swap of file content.
///
// =================================================================================================

static const XMP_OptionBits kTrivial_HandlerFlags = ( kXMPFiles_AllowsOnlyXMP |
                                                      kXMPFiles_ReturnsRawPacket |
                                                      kXMPFiles_AllowsSafeUpdate );

class Trivial_MetaHandler : public XMPFileHandler
{
public:

	Trivial_MetaHandler() {};
	~Trivial_MetaHandler();

	virtual void CacheFileData() = 0;

	void UpdateFile ( bool doSafeUpdate );
    void WriteTempFile ( XMP_IO* tempRef );

};	// Trivial_MetaHandler

// =================================================================================================

#endif /* __Trivial_Handler_hpp__ */
