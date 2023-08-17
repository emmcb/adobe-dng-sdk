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
//  SDS Sankar Dey Sarkar
//  AWL Alan Lillich
//  SKP Sunil Kishor Pathak
//  ADC Amandeep Chawla
//	HK  Honey Kansal
//
// mm-dd-yy who Description of changes, most recent on top
//
// 04-26-13 ADC 5.6-f054 [3526891] Read-only checks in IsMetadataWritable() API try creating temporary files if no metadata exists, instead of checking for file permissions.
//						 [3525961] IsMetaDataWritable checks for Plugins and embedded handlers are different.
// 04-04-13 IJS 5.6-f051 Move common I/O methods to IOUtils.
//						 [3534631] Optimize GetAssociated Resources for RED_Handler
// 02-21-13 HK  5.6-f041 [3503922] Support GetAssociatedResources and IsMetadataWritable API's for MPEG2.
// 10-10-12 ADC 5.5-f046 Fixing compile time issues which creeped into while doing merging of my changelist.
// 10-10-12 ADC 5.5-f045 Implement the internal infrastructure for XMPFiles error notifications.
// 10-09-12 SKP 5.5-f044 Refactored LocateMetadataFiles API.
// 01-16-12	AWL	5.5-f006 [2980767] Improve sidecar file update for disk full situations.
//
// 09-27-11 AWL 5.4-f014 Add MPEG-2 support for GetFileModDate.
//
// 08-27-10 AWL 5.3-f007 Add comments to Host_IO.hpp, fix semantic irregularities in the code.
// 08-19-10 AWL 5.3-f004 Move the seek mode constants to XMP_Const.
// 08-19-10 AWL 5.3-f003 Remove all use of the LFA_* names.
// 08-17-10 AWL 5.3-f001 Integrate I/O revamp to main.
//
// 11-13-07 AWL 4.2-f035 Add MPEG-4 skeleton handler. Rename MPEG handler to MPEG-2.
//
// 10-12-06 AWL 4.1-f040 [1394229] Make sure all handlers accept only "proper" files. Preferably
//				only by file content. Where we must, by "known" file extension.
// 09-19-06 AWL 4.1-f036 [1379782] Fix the MPEG handler to truncate an existing sidecar before rewriting.
// 09-15-06 AWL 4.1-f035 Revamp the MPEG handler to be platform neutral.
//
// 04-07-06 AWL 4.0-f002 Add XMPFiles::GetThumbnail. Change XMPFiles::OpenFile to close the disk file
//				early for read-only access. Change XMPFileHandler::ExtractXMP to CacheFileData.
// 03-24-06 AWL 4.0-f001 Adapt for move to ham-perforce, integrate XMPFiles, bump version to 4.
//
// 01-24-06 AWL 1.3-012 Adapt for VC8.
//
// 03-29-05 SDS First draft.
//
// =================================================================================================
#endif // AdobePrivate

#if XMP_WinBuild
	#pragma warning ( disable : 4996 )	// '...' was declared deprecated
#endif

#include "public/include/XMP_Environment.h"	// ! XMP_Environment.h must be the first included header.

#include "public/include/XMP_Const.h"
#include "public/include/XMP_IO.hpp"

#include "XMPFiles/source/XMPFiles_Impl.hpp"
#include "source/XMPFiles_IO.hpp"
#include "source/XIO.hpp"
#include "source/IOUtils.hpp"

#include "XMPFiles/source/FileHandlers/MPEG2_Handler.hpp"
#include "../FormatSupport/PackageFormat_Support.hpp"
using namespace std;

// =================================================================================================
/// \file MPEG2_Handler.cpp
/// \brief File format handler for MPEG2.
///
/// BLECH! YUCK! GAG! MPEG-2 is done using a sidecar and recognition only by file extension! BARF!!!!!
///
// =================================================================================================

// =================================================================================================
// FindFileExtension
// =================

static inline XMP_StringPtr FindFileExtension ( XMP_StringPtr filePath )
{

	XMP_StringPtr pathEnd = filePath + strlen(filePath);
	XMP_StringPtr extPtr;

	for ( extPtr = pathEnd-1; extPtr > filePath; --extPtr ) {
		if ( (*extPtr == '.') || (*extPtr == '/') ) break;
		#if XMP_WinBuild
			if ( (*extPtr == '\\') || (*extPtr == ':') ) break;
		#endif
	}

	if ( (extPtr < filePath) || (*extPtr != '.') ) return pathEnd;
	return extPtr;

}	// FindFileExtension

// =================================================================================================
// MPEG2_MetaHandlerCTor
// =====================

XMPFileHandler * MPEG2_MetaHandlerCTor ( XMPFiles * parent )
{
	return new MPEG2_MetaHandler ( parent );

}	// MPEG2_MetaHandlerCTor

// =================================================================================================
// MPEG2_CheckFormat
// =================

// The MPEG-2 handler uses just the file extension, not the file content. Worse yet, it also uses a
// sidecar file for the XMP. This works better if the handler owns the file, we open the sidecar
// instead of the actual MPEG-2 file.

bool MPEG2_CheckFormat ( XMP_FileFormat format,
						XMP_StringPtr  filePath,
						XMP_IO*    fileRef,
						XMPFiles *     parent )
{
	IgnoreParam(format); IgnoreParam(filePath); IgnoreParam(fileRef);

	XMP_Assert ( (format == kXMP_MPEGFile) || (format == kXMP_MPEG2File) );
	XMP_Assert ( fileRef == 0 );

	return ( (parent->format == kXMP_MPEGFile) || (parent->format == kXMP_MPEG2File) );	// ! Just use the first call's format hint.

}	// MPEG2_CheckFormat

// =================================================================================================
// MPEG2_MetaHandler::MPEG2_MetaHandler
// ====================================

MPEG2_MetaHandler::MPEG2_MetaHandler ( XMPFiles * _parent )
{
	this->parent = _parent;
	this->handlerFlags = kMPEG2_HandlerFlags;
	this->stdCharForm  = kXMP_Char8Bit;

	XMP_StringPtr filePath = this->parent->GetFilePath().c_str();
	XMP_StringPtr extPtr = FindFileExtension ( filePath );
	this->sidecarPath.assign ( filePath, (extPtr - filePath) );
	this->sidecarPath += ".xmp";
}	// MPEG2_MetaHandler::MPEG2_MetaHandler

// =================================================================================================
// MPEG2_MetaHandler::~MPEG2_MetaHandler
// =====================================

MPEG2_MetaHandler::~MPEG2_MetaHandler()
{
	// Nothing to do.

}	// MPEG2_MetaHandler::~MPEG2_MetaHandler

// =================================================================================================
// MPEG2_MetaHandler::GetFileModDate
// =================================

bool MPEG2_MetaHandler::GetFileModDate ( XMP_DateTime * modDate )
{
	if ( ! Host_IO::Exists ( this->sidecarPath.c_str() ) ) return false;
	return Host_IO::GetModifyDate ( this->sidecarPath.c_str(), modDate );

}	// MPEG2_MetaHandler::GetFileModDate

// =================================================================================================
// MPEG2_MetaHandler::FillAssociatedResources
// =================================
void MPEG2_MetaHandler::FillAssociatedResources ( std::vector<std::string> * resourceList )
{
	resourceList->push_back(this->parent->GetFilePath());
	PackageFormat_Support::AddResourceIfExists(resourceList, this->sidecarPath);
}	// MPEG2_MetaHandler::FillAssociatedResources

// =================================================================================================
// MPEG2_MetaHandler::IsMetadataWritable
// =================================
bool MPEG2_MetaHandler::IsMetadataWritable ( )
{
	return Host_IO::Writable( this->sidecarPath.c_str(), true );
}	// MPEG2_MetaHandler::IsMetadataWritable

// =================================================================================================
// MPEG2_MetaHandler::CacheFileData
// ================================

void MPEG2_MetaHandler::CacheFileData()
{
	bool readOnly = (! (this->parent->openFlags & kXMPFiles_OpenForUpdate));

	if ( this->parent->UsesClientIO() ) {
		XMP_Throw ( "MPEG2 cannot be used with client-managed I/O", kXMPErr_InternalFailure );
	}

	this->containsXMP = false;
	this->processedXMP = true;	// Whatever we do here is all that we do for XMPFiles::OpenFile.

	// Try to open the sidecar XMP file. Tolerate an open failure, there might not be any XMP.
	// Note that MPEG2_CheckFormat can't save the sidecar path because the handler doesn't exist then.

	if ( ! Host_IO::Exists ( this->sidecarPath.c_str() ) ) return;	// OK to not have XMP.

	XMPFiles_IO * localFile = XMPFiles_IO::New_XMPFiles_IO ( this->sidecarPath.c_str(), readOnly );
	if ( localFile == 0 ) XMP_Throw ( "Failure opening MPEG-2 XMP file", kXMPErr_ExternalFailure );
	this->parent->ioRef = localFile;

	// Extract the sidecar's contents and parse.

	this->packetInfo.offset = 0;	// We take the whole sidecar file.
	this->packetInfo.length = (XMP_Int32) localFile->Length();

	if ( this->packetInfo.length > 0 ) {

		this->xmpPacket.assign ( this->packetInfo.length, ' ' );
		localFile->ReadAll ( (void*)this->xmpPacket.c_str(), this->packetInfo.length );

		this->xmpObj.ParseFromBuffer ( this->xmpPacket.c_str(), (XMP_StringLen)this->xmpPacket.size() );
		this->containsXMP = true;

	}

	if ( readOnly ) {
		localFile->Close();
		delete localFile;
		this->parent->ioRef = 0;
	}

}	// MPEG2_MetaHandler::CacheFileData

// =================================================================================================
// MPEG2_MetaHandler::UpdateFile
// =============================

void MPEG2_MetaHandler::UpdateFile ( bool doSafeUpdate )
{
	if ( ! this->needsUpdate ) return;

	XMP_Assert ( this->parent->UsesLocalIO() );

	if ( this->parent->ioRef == 0 ) {
		XMP_Assert ( ! Host_IO::Exists ( this->sidecarPath.c_str() ) );
		Host_IO::Create ( this->sidecarPath.c_str() );
		this->parent->ioRef = XMPFiles_IO::New_XMPFiles_IO ( this->sidecarPath.c_str(), Host_IO::openReadWrite );
		if ( this->parent->ioRef == 0 ) XMP_Throw ( "Failure opening MPEG-2 XMP file", kXMPErr_ExternalFailure );
	}

	XMP_IO* fileRef = this->parent->ioRef;
	XMP_Assert ( fileRef != 0 );
	XIO::ReplaceTextFile ( fileRef, this->xmpPacket, doSafeUpdate );

	XMPFiles_IO* localFile = (XMPFiles_IO*)fileRef;
	localFile->Close();
	delete localFile;
	this->parent->ioRef = 0;

	this->needsUpdate = false;

}	// MPEG2_MetaHandler::UpdateFile

// =================================================================================================
// MPEG2_MetaHandler::WriteTempFile
// ================================

void MPEG2_MetaHandler::WriteTempFile ( XMP_IO* tempRef )
{
	IgnoreParam(tempRef);

	XMP_Throw ( "MPEG2_MetaHandler::WriteTempFile: Should never be called", kXMPErr_Unavailable );

}	// MPEG2_MetaHandler::WriteTempFile
