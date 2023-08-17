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

#include "public/include/XMP_Environment.h"	

#include "public/include/XMP_Const.h"
#include "public/include/XMP_IO.hpp"

#include "XMPFiles/source/XMPFiles_Impl.hpp"
#include "source/XMPFiles_IO.hpp"
#include "source/XIO.hpp"

#include "XMPFiles/source/FormatSupport/PackageFormat_Support.hpp"
#include "XMPFiles/source/FileHandlers/Generic_Handler.hpp"

#include <vector>

using namespace std;

#if EnableGenericHandling

// =================================================================================================
/// \file Generic_MetaHandler.hpp
/// \brief File format handler for handling Unknown file format.
// =================================================================================================

// =================================================================================================
// FindFileExtension - TODO remove the redundant method
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
// Generic_MetaHandlerCTor
// =======================

XMPFileHandler * Generic_MetaHandlerCTor ( XMPFiles * parent )
{
	return new Generic_MetaHandler ( parent );

}	// Generic_MetaHandlerCTor

// =================================================================================================
// Generic_MetaHandler::Generic_MetaHandler
// ========================================
Generic_MetaHandler::Generic_MetaHandler ( XMPFiles * _parent )
{
	this->parent = _parent;
	this->handlerFlags = kGeneric_HandlerFlags;
	this->stdCharForm  = kXMP_Char8Bit;

	this->genericPath = this->parent->GetFilePath() + ".xmp";

}// Generic_MetaHandler::Generic_MetaHandler	// Generic_MetaHandler::Generic_MetaHandler

// =================================================================================================
// Generic_MetaHandler::~Generic_MetaHandler
// =========================================
Generic_MetaHandler::~Generic_MetaHandler()
{

}	// Generic_MetaHandler::~Generic_MetaHandler


// =================================================================================================
// Generic_MetaHandler::FillAssociatedResources
// ============================================
void Generic_MetaHandler::FillAssociatedResources ( std::vector<std::string> * resourceList )
{
	resourceList->push_back(this->parent->GetFilePath());
	PackageFormat_Support::AddResourceIfExists(resourceList, this->genericPath);
}	// Generic_MetaHandler::FillAssociatedResources

// =================================================================================================
// Generic_MetaHandler::IsMetadataWritable
// =======================================
bool Generic_MetaHandler::IsMetadataWritable ( )
{
	return Host_IO::Writable( this->genericPath.c_str(), true );
}	// Generic_MetaHandler::IsMetadataWritable

// =================================================================================================
// Generic_MetaHandler::CacheFileData
// ==================================
void Generic_MetaHandler::CacheFileData()
{
	bool readOnly = (! (this->parent->openFlags & kXMPFiles_OpenForUpdate));

	if ( this->parent->UsesClientIO() ) {
		XMP_Throw ( "Generic Handler cannot be used with client-managed I/O", kXMPErr_InternalFailure );
	}

	this->containsXMP = false;
	this->processedXMP = true;

	// Try to open the Generic XMP file. Tolerate an open failure, there might not be any XMP.

	if ( ! Host_IO::Exists ( this->genericPath.c_str() ) ) return;	

	XMPFiles_IO * localFile = XMPFiles_IO::New_XMPFiles_IO ( this->genericPath.c_str(), readOnly );
	if ( localFile == 0 ) XMP_Throw ( "Failure opening Generic XMP file", kXMPErr_ExternalFailure );
	this->parent->ioRef = localFile;


	this->packetInfo.offset = 0;
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

}	// Generic_MetaHandler::CacheFileData

// =================================================================================================
// Generic_MetaHandler::GetSerializeOptions
// ========================================
XMP_OptionBits Generic_MetaHandler::GetSerializeOptions()
{
	return XMPFileHandler::GetSerializeOptions()|kXMP_IncludeRDFHash;
}

// =================================================================================================
// Generic_MetaHandler::UpdateFile
// ===============================
void Generic_MetaHandler::UpdateFile ( bool doSafeUpdate )
{
	if ( ! this->needsUpdate ) return;

	XMP_Assert ( this->parent->UsesLocalIO() );

	if ( this->parent->ioRef == 0 ) {
		XMP_Assert ( ! Host_IO::Exists ( this->genericPath.c_str() ) );
		Host_IO::Create ( this->genericPath.c_str() );
		this->parent->ioRef = XMPFiles_IO::New_XMPFiles_IO ( this->genericPath.c_str(), Host_IO::openReadWrite );
		if ( this->parent->ioRef == 0 ) XMP_Throw ( "Failure opening Generic XMP file", kXMPErr_ExternalFailure );
	}

	XMP_IO* fileRef = this->parent->ioRef;
	XMP_Assert ( fileRef != 0 );
	XIO::ReplaceTextFile ( fileRef, this->xmpPacket, doSafeUpdate );

	XMPFiles_IO* localFile = (XMPFiles_IO*)fileRef;
	localFile->Close();
	delete localFile;
	this->parent->ioRef = 0;

	this->needsUpdate = false;

}	// Generic_MetaHandler::UpdateFile

// =================================================================================================
// Generic_MetaHandler::WriteTempFile
// ==================================
void Generic_MetaHandler::WriteTempFile ( XMP_IO* tempRef )
{
	IgnoreParam(tempRef);

	XMP_Throw ( "Generic_MetaHandler::WriteTempFile: Should never be called", kXMPErr_Unavailable );

}	// Generic_MetaHandler::WriteTempFile

// =================================================================================================

#endif	// EnableGenericHandling


