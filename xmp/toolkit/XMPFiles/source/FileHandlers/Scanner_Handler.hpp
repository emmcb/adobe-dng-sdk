#ifndef __Scanner_Handler_hpp__
#define __Scanner_Handler_hpp__	1

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
// 08-16-04 AWL First draft.
//
// =================================================================================================
#endif // AdobePrivate

#include "XMPFiles/source/FileHandlers/Trivial_Handler.hpp"

// =================================================================================================
/// \file Scanner_Handler.hpp
/// \brief File format handler for packet scanning.
///
/// This header ...
///
// =================================================================================================

extern XMPFileHandler * Scanner_MetaHandlerCTor ( XMPFiles * parent );

static const XMP_OptionBits kScanner_HandlerFlags = kTrivial_HandlerFlags;

class Scanner_MetaHandler : public Trivial_MetaHandler
{
public:

	Scanner_MetaHandler () {};
	Scanner_MetaHandler ( XMPFiles * parent );

	~Scanner_MetaHandler();

	void CacheFileData();

};	// Scanner_MetaHandler

// =================================================================================================

#endif /* __Scanner_Handler_hpp__ */
