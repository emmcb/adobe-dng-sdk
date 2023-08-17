#ifndef __InDesign_Handler_hpp__
#define __InDesign_Handler_hpp__	1

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
// 08-18-10 AWL 5.3-f002 Don't include XIO.hpp in any headers, only .cpp files.
// 08-17-10 AWL 5.3-f001 Integrate I/O revamp to main.
//
// 04-07-06 AWL 4.0-f002 Add XMPFiles::GetThumbnail. Change XMPFiles::OpenFile to close the disk file
//				early for read-only access. Change XMPFileHandler::ExtractXMP to CacheFileData.
// 03-24-06 AWL 4.0-f001 Adapt for move to ham-perforce, integrate XMPFiles, bump version to 4.
//
// 09-01-05 AWL 1.3-003 [1225858] Don't implicitly set the SafeUpdate flag for non-owning handlers.
//
// 04-13-05 AWL 1.1-052 Add tweaks to the CheckProc needed by the MDKit-based JPEG/TIFF/PSD handler.
//
// 01-06-05 AWL 1.0-027 [1014854] Cleanup handling for UTF-16 and UTF-32. This involves some general
//				changes to the separation of work between the common code and handlers, making the
//				storage of the SXMPMeta object and packet string more visible.
// 10-07-04 AWL 1.0-009 Add first cut of support for crash safe file update.
// 08-24-04 AWL 1.0-003 Change InDesign handler to a Basic_Handler.
//
// 08-19-04 AWL First draft.
//
// =================================================================================================
#endif // AdobePrivate

#include "public/include/XMP_Environment.h"	// ! XMP_Environment.h must be the first included header.

#include "public/include/XMP_Const.h"
#include "public/include/XMP_IO.hpp"

#include "XMPFiles/source/XMPFiles_Impl.hpp"
#include "source/XMPFiles_IO.hpp"

#include "XMPFiles/source/FileHandlers/Basic_Handler.hpp"

// =================================================================================================
/// \file InDesign_Handler.hpp
/// \brief File format handler for InDesign files.
///
/// This header ...
///
// =================================================================================================

extern XMPFileHandler * InDesign_MetaHandlerCTor ( XMPFiles * parent );

extern bool InDesign_CheckFormat ( XMP_FileFormat format,
								   XMP_StringPtr  filePath,
			                       XMP_IO*        fileRef,
			                       XMPFiles *     parent );

static const XMP_OptionBits kInDesign_HandlerFlags = kBasic_HandlerFlags & (~kXMPFiles_CanInjectXMP);	// ! InDesign can't inject.

class InDesign_MetaHandler : public Basic_MetaHandler
{
public:

	InDesign_MetaHandler ( XMPFiles * parent );
	~InDesign_MetaHandler();

	void CacheFileData();

protected:

	void WriteXMPPrefix ( XMP_IO* fileRef );
	void WriteXMPSuffix ( XMP_IO* fileRef );

	void NoteXMPRemoval ( XMP_IO* fileRef );
	void NoteXMPInsertion ( XMP_IO* fileRef );

	void CaptureFileEnding ( XMP_IO* fileRef );
	void RestoreFileEnding ( XMP_IO* fileRef );

	bool streamBigEndian;	// Set from master page's fObjectStreamEndian.
	XMP_Uns32 xmpObjID;		// Set from contiguous object's fObjectID, still as little endian.
	XMP_Uns32 xmpClassID;	// Set from contiguous object's fObjectClassID, still as little endian.

};	// InDesign_MetaHandler

// =================================================================================================

#endif /* __InDesign_Handler_hpp__ */
