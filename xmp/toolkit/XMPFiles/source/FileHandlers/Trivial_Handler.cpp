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
// 08-19-10 AWL 5.3-f004 Move the seek mode constants to XMP_Const.
// 08-19-10 AWL 5.3-f003 Remove all use of the LFA_* names.
// 08-17-10 AWL 5.3-f001 Integrate I/O revamp to main.
//
// 04-07-06 AWL 4.0-f002 Add XMPFiles::GetThumbnail. Change XMPFiles::OpenFile to close the disk file
//				early for read-only access. Change XMPFileHandler::ExtractXMP to CacheFileData.
// 03-24-06 AWL 4.0-f001 Adapt for move to ham-perforce, integrate XMPFiles, bump version to 4.
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

#include "public/include/XMP_Environment.h"	// ! XMP_Environment.h must be the first included header.

#include "public/include/XMP_Const.h"
#include "public/include/XMP_IO.hpp"

#include "XMPFiles/source/XMPFiles_Impl.hpp"
#include "source/XIO.hpp"

#include "XMPFiles/source/FileHandlers/Trivial_Handler.hpp"

using namespace std;

// =================================================================================================
/// \file Trivial_Handler.cpp
/// \brief Base class for trivial handlers that only process in-place XMP.
///
/// This header ...
///
// =================================================================================================

// =================================================================================================
// Trivial_MetaHandler::~Trivial_MetaHandler
// =========================================

Trivial_MetaHandler::~Trivial_MetaHandler()
{
	// Nothing to do.

}	// Trivial_MetaHandler::~Trivial_MetaHandler

// =================================================================================================
// Trivial_MetaHandler::UpdateFile
// ===============================

void Trivial_MetaHandler::UpdateFile ( bool doSafeUpdate )
{
	IgnoreParam ( doSafeUpdate );
	XMP_Assert ( ! doSafeUpdate );	// Not supported at this level.
	if ( ! this->needsUpdate ) return;

	XMP_IO*      fileRef    = this->parent->ioRef;
	XMP_PacketInfo & packetInfo = this->packetInfo;
	std::string &    xmpPacket  = this->xmpPacket;

	fileRef->Seek ( packetInfo.offset, kXMP_SeekFromStart );
	fileRef->Write ( xmpPacket.c_str(), packetInfo.length );
	XMP_Assert ( xmpPacket.size() == (size_t)packetInfo.length );

	this->needsUpdate = false;

}	// Trivial_MetaHandler::UpdateFile

// =================================================================================================
// Trivial_MetaHandler::WriteTempFile
// ==================================

void Trivial_MetaHandler::WriteTempFile ( XMP_IO* tempRef )
{
	IgnoreParam ( tempRef );

	XMP_Throw ( "Trivial_MetaHandler::WriteTempFile: Not supported", kXMPErr_Unavailable );

}	// Trivial_MetaHandler::WriteTempFile

// =================================================================================================
