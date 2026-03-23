#if AdobePrivate	// ! This entire handler is private, it must not go in the public SDK.

#ifndef __WebP_Handler_h__
#define __WebP_Handler_h__ 1

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

#include "source/Endian.h"
#include "XMPFiles/source/FormatSupport/IFF/ChunkController.h"
#include "XMPFiles/source/FormatSupport/IFF/IChunkBehavior.h"
#include "XMPFiles/source/FormatSupport/IFF/IChunkData.h"
#include "XMPFiles/source/FormatSupport/TIFF_Support.hpp"
#include "XMPFiles/source/XMPFiles_Impl.hpp"

// =================================================================================================
/// \file WebP_Handler.hpp
/// \brief File format handler for WebP (RIFF container based).
// =================================================================================================

using namespace IFF_RIFF;

extern XMPFileHandler * WebP_MetaHandlerCTor ( XMPFiles* parent );

extern bool WebP_CheckFormat ( XMP_FileFormat format,
							   XMP_StringPtr  filePath,
							   XMP_IO*       fileRef,
							   XMPFiles*     parent );

static const XMP_OptionBits kWebP_HandlerFlags = (
												  //kXMPFiles_CanInjectXMP |
												  //kXMPFiles_CanExpand |
												  // kXMPFiles_CanRewrite |
												  // kXMPFiles_PrefersInPlace |
												  kXMPFiles_CanReconcile |
												  kXMPFiles_AllowsOnlyXMP |
												  kXMPFiles_ReturnsRawPacket //|
												  //kXMPFiles_AllowsSafeUpdate |
												  //kXMPFiles_CanNotifyProgress
												  );

class WebP_MetaHandler : public XMPFileHandler
{
	
public:
	
	void CacheFileData();
	void ProcessXMP();
	
	void UpdateFile ( bool doSafeUpdate );
	void WriteTempFile ( XMP_IO* tempRef );
	
	WebP_MetaHandler ( XMPFiles* parent );
	virtual ~WebP_MetaHandler();
	
private:
	
	/** private standard Ctor, not to be used */
	WebP_MetaHandler (): mChunkController(NULL), mChunkBehavior(NULL),
	mXMPChunk(NULL), mExifChunk(NULL), mExifMgr(NULL) {};
	
	bool ImportExifData();
	bool ImportXMPData();
	
	// ----- MEMBERS ----- //
	
	/** container for EXIF data */
	TIFF_Manager * mExifMgr;
	
	/** pointer to the XMP chunk */
	IChunkData *mXMPChunk;
	
	/** pointer to EXIF chunks */
	IChunkData *mExifChunk;
	
	/** Controls the parsing and writing of the passed stream. */
	ChunkController *mChunkController;
	
	/** Represents the rules how chunks are added, removed or rearranged */
	IChunkBehavior *mChunkBehavior;
	
	// ----- CONSTANTS ----- //
	
	/** Chunk path identifier of interest in WebP */
	static const ChunkIdentifier kRIFFXMP[2];   
	static const ChunkIdentifier kRIFFExif[2];

	/** Path to XMP chunk */
	ChunkPath mWebPXMPChunkPath;
	
	/** Path to EXIF chunk */
	ChunkPath mWebPExifChunkPath;

	
};	// WebP_MetaHandler

#endif /* WebP_Handler_h */

#endif	// AdobePrivate	// ! This entire handler is private, it must not go in the public SDK.




