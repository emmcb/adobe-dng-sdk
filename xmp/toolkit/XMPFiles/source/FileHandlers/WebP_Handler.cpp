#if AdobePrivate	// ! This entire handler is private, it must not go in the public SDK.

// =================================================================================================
// Copyright Adobe
// Copyright 2020 Adobe
// All Rights Reserved
//
// NOTICE: Adobe permits you to use, modify, and distribute this file in accordance with the terms
// of the Adobe license agreement accompanying it. If you have received this file from a source other
// than Adobe, then your use, modification, or distribution of it requires the prior written permission
// of Adobe.
// =================================================================================================


#include "public/include/XMP_Environment.h"	// ! XMP_Environment.h must be the first included header.
#include "public/include/XMP_Const.h"

#include "XMPFiles/source/FileHandlers/WebP_Handler.hpp"
#include "XMPFiles/source/FormatSupport/WebP/WebPBehavior.h"
#include "source/XIO.hpp"
#include "XMPFiles/source/FormatSupport/ISOBaseMedia_Support.hpp"
#include "XMPFiles/source/FormatSupport/TIFF_Support.hpp"
#include "XMPFiles/source/FormatSupport/Reconcile_Impl.hpp"


// =================================================================================================
/// \file WebP_Handler.cpp
/// \brief File format handler for WebP (RIFF container based).
// =================================================================================================

/* The WebP file format is based on the RIFF (resource interchange file format)
 * document format.
 *
 * The basic element of a RIFF file is a _chunk_.
 *
 * WebP File Header Format
 * ----------------
 *
 *  0                   1                   2                   3
 * 0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1
 *  +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
 *  |      'R'      |      'I'      |      'F'      |      'F'      |
 *  +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
 *  |                           File Size                           |
 *  +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
 *  |      'W'      |      'E'      |      'B'      |      'P'      |
 *  +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
 *
 *  WebP is structured in Chunk, sub-chunk hierarchy.
 *  Every chunk contains ChunkID, ChunkSize, and ChunkData.
 *  First four bytes of a WebP file type must be ASCII values corresponding to the letters,
 *  RIFF followed at offset 8 (in bytes) by the ASCII values corresponding to the letters WebP.
 *  This is the ChunkId parent of every other sub-chunk. Similar to the rules dominating RIFF structure which describe
 *  how a typical RIFF and WebP file are:
 *  Every chunk should have ChunkId(four bytes), ChunkSize(four bytes).
 *  Do not include size of ChunkId and ChunkSize in ChunkSize.
 *  ChunkData having odd number of bytes should be padded with one more dummy byte, and ChunkSize is always even.
 *  Exiv2 seamlessly parses and presents the metadata in a WebP file in the same way that it does for JPEG.
 */

using namespace IFF_RIFF;

#define RIFF_HEADER_SIZE   12

// =================================================================================================
// Static inits of Chunk Identifier
// ====================

// RIFF:WEBP/XMP
const ChunkIdentifier WebP_MetaHandler::kRIFFXMP[2] = {
	{ (static_cast<XMP_Uns32>(kChunk_RIFF)), (static_cast<XMP_Uns32>(kType_WebP)) },
	{ (static_cast<XMP_Uns32>(kChunk_WebP_XMP)),(static_cast<XMP_Uns32>(kType_NONE))} };

// RIFF:WEBP/EXIF
const ChunkIdentifier WebP_MetaHandler::kRIFFExif[2] = {
	{ static_cast<XMP_Uns32>(kChunk_RIFF), static_cast<XMP_Uns32>(kType_WebP) },
	{ static_cast<XMP_Uns32>(kChunk_EXIF), static_cast<XMP_Uns32>(kType_NONE) } };

// =================================================================================================
// WebP_MetaHandlerCTor
// ====================

XMPFileHandler* WebP_MetaHandlerCTor ( XMPFiles* parent )
{
	return new WebP_MetaHandler ( parent );
}

// =================================================================================================
// WebP_CheckFormat
// ===============
//
// An WebP file must begin with "RIFF", a 4 byte length, then the chunkType WebP
// The specified length MUST (in practice: SHOULD) match fileSize-8, but we don't bother checking this here.

bool WebP_CheckFormat ( XMP_FileFormat  format,
					   XMP_StringPtr    filePath,
					   XMP_IO*      	file,
					   XMPFiles*        parent )
{
	XMP_Assert ( (format == kXMP_WebPFile) );
	bool openStrictly = XMP_OptionIsSet(parent->openFlags, kXMPFiles_OpenStrictly);
	
	if ( file->Length() < RIFF_HEADER_SIZE ) return false;
	file ->Rewind();
	
	XMP_Uns8 chunkID[RIFF_HEADER_SIZE];
	file->ReadAll ( chunkID, RIFF_HEADER_SIZE );
	if ( ! CheckBytes( &chunkID[0], "RIFF", 4 ) ) return false;
	if ( ! CheckBytes(&chunkID[8],"WEBP",4) ) return false;
	if (openStrictly && (format != kXMP_WebPFile)) return false;
	
	return true;
}	// WebP_CheckFormat


// =================================================================================================
// WebP_MetaHandler::WebP_MetaHandler
// ================================

WebP_MetaHandler::WebP_MetaHandler ( XMPFiles * _parent )
		: mExifMgr(0), mChunkController(NULL), mChunkBehavior(NULL),
		  mXMPChunk(NULL), mExifChunk(NULL)
{
	this->parent = _parent;
	this->handlerFlags = kWebP_HandlerFlags;
	this->stdCharForm  = kXMP_Char8Bit;
	
	this->mChunkBehavior = new WebPBehavior();
	this->mChunkController = new ChunkController( mChunkBehavior, false );
	
}

// =================================================================================================
// WebP_MetaHandler::~~WebP_MetaHandler
// =================================

WebP_MetaHandler::~WebP_MetaHandler()
{
	if( mExifMgr != NULL ) delete (mExifMgr);
	if( mChunkController != NULL ) delete mChunkController;
	if( mChunkBehavior != NULL ) delete mChunkBehavior;
}

// =================================================================================================
// WebP_MetaHandler::CacheFileData
// ==============================

void WebP_MetaHandler::CacheFileData ()
{
	XMP_IO* fileRef = parent->ioRef;
	XMP_PacketInfo & packetInfo = this->packetInfo;
	
	XMPFiles * parent = this->parent;
	XMP_OptionBits openFlags = parent->openFlags;
	
	XMP_AbortProc abortProc = parent->abortProc;
	void *        abortArg = parent->abortArg;
	const bool    checkAbort = (abortProc != 0);
	
	XMP_Int64 fileLength = fileRef->Length();
	
	if ( fileLength > 0xFFFFFFFD ) //4GB -2bytes
		XMP_Throw ( "WebP_MetaHandler::CacheFileData: WebP Files larger (4GB - 2bytes) not supported", kXMPErr_Unimplemented );
	
	fileRef->Rewind();
	// parse top-level chunks
	
	mWebPXMPChunkPath.append( kRIFFXMP, SizeOfCIArray(kRIFFXMP) );
	mWebPExifChunkPath.append( kRIFFExif, SizeOfCIArray(kRIFFExif) );
	
	mChunkController->addChunkPath( mWebPXMPChunkPath );
	mChunkController->addChunkPath( mWebPExifChunkPath );
	
	// Parse the given file throws exception if the file cannot be parsed
	mChunkController->parseFile( this->parent->ioRef, &this->parent->openFlags );
	
	// Retrieve the file type, it must have at least RIFF:WEBP
	std::vector<XMP_Uns32> typeList = mChunkController->getTopLevelTypes();
	
	// If file not WebP, throw exception
	XMP_Validate( typeList.at(0)  == kType_WebP , "File is not of type WebP", kXMPErr_BadFileFormat );
	
	//Check, If there are duplicate XMP chunks present in file, consider the first one.
	mXMPChunk = mChunkController->getChunk( mWebPXMPChunkPath );

	// Retrieve XMP packet info
	if( mXMPChunk != NULL  )
	{
		this->packetInfo.length = static_cast<XMP_Int32>(mXMPChunk->getSize());
		this->packetInfo.charForm  = kXMP_Char8Bit;
		this->packetInfo.writeable = true;
		
		// Get actual the XMP packet
		this->xmpPacket.assign ( mXMPChunk->getString( this->packetInfo.length) );
		
		// set state
		this->containsXMP = true;
	}
	
}	// WebP_MetaHandler::CacheFileData


// =================================================================================================
// WebP_MetaHandler::ProcessXMP
// ===========================
//
// Process the raw XMP and legacy metadata that was previously cached.

void WebP_MetaHandler::ProcessXMP()
{
	// Must be done only once
	if ( this->processedXMP ) {
		return;
	}
	// Set the status at start, in case something goes wrong in this method
	this->processedXMP = true;
	
	XMPFiles * parent = this->parent;
	XMP_OptionBits openFlags = parent->openFlags;
	mExifMgr = new TIFF_MemoryReader();

	/*
	 To be used whe adding update metadata support
	 bool readOnly = (!(openFlags & kXMPFiles_OpenForUpdate));
	if (mExifMgr == 0) {
		if (readOnly)
			mExifMgr = new TIFF_MemoryReader();
		else
			mExifMgr = new TIFF_FileWriter();
	}
	*/
	bool exifPresent = false, xmpPresent = false;
	
	// Parse the XMP
	if ( ! this->xmpPacket.empty() ) {
		xmpPresent = ImportXMPData();
	}
	
	// Parse EXIF chunk
	mExifChunk = mChunkController->getChunk( mWebPExifChunkPath );
	if( mExifChunk != NULL ) {
		exifPresent = ImportExifData();
	}
	
	IPTC_Reader iptc;
	PSIR_MemoryReader psir;
	XMP_OptionBits options = 0;
	if (xmpPresent) options |= k2XMP_FileHadXMP;
	if(exifPresent)	options |= k2XMP_FileHadExif;
	
	ImportPhotoData(*mExifMgr, iptc, psir, kDigestDiffers, &this->xmpObj, options);
	
	this->containsXMP = xmpPresent | exifPresent;
	
}	// WebP_MetaHandler::ProcessXMP

// =================================================================================================
// WebP_MetaHandler::UpdateFile
// ===========================

void WebP_MetaHandler::UpdateFile ( bool doSafeUpdate )
{
	
	XMP_Throw("Currently update of .webp files is not implemented", kXMPErr_Unimplemented);

}	// WebP_MetaHandler::UpdateFile

// =================================================================================================
// WebP_MetaHandler::WriteTempFile
// ==============================

void WebP_MetaHandler::WriteTempFile ( XMP_IO* tempRef )
{
	XMP_Throw ( "WebP_MetaHandler::WriteTempFile is not implemented", kXMPErr_Unimplemented );
	
}	// WebP_MetaHandler::WriteTempFile


// =================================================================================================
// WebP_MetaHandler::ImportExifData
// =============================
bool WebP_MetaHandler::ImportExifData() {
	
	const XMP_Uns8* buffer = NULL;
	XMP_Uns64 size = 0;
	size = mExifChunk->getData( &buffer );
	try {
		mExifMgr->ParseMemoryStream ( buffer, size );
		return true;
	}
	catch (...) {
		//skip parsing failure and try to get XMP if prrsent
		return false;
	}
}

// =================================================================================================
// WebP_MetaHandler::ImportXMPData
// =============================
bool WebP_MetaHandler::ImportXMPData() {
	XMP_Assert ( this->containsXMP );
	
	FillPacketInfo ( this->xmpPacket, &this->packetInfo );
	try {
		this->xmpObj.ParseFromBuffer ( this->xmpPacket.c_str(), (XMP_StringLen)this->xmpPacket.size() );
	}catch (...) {
		/* Ignore parsing failures, Try to process Exif block */
		return false;
	}
	this->containsXMP = true;
	return true;
}

// =================================================================================================

#endif	// AdobePrivate	// ! This entire handler is private, it must not go in the public SDK.
