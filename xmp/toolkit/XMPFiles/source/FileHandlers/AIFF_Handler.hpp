// =================================================================================================
// Copyright Adobe
// Copyright 2010 Adobe
// All Rights Reserved
//
// NOTICE: Adobe permits you to use, modify, and distribute this file in accordance with the terms
// of the Adobe license agreement accompanying it. If you have received this file from a source other 
// than Adobe, then your use, modification, or distribution of it requires the prior written permission
// of Adobe.
// =================================================================================================

#ifndef __AIFF_Handler_hpp__
#define __AIFF_Handler_hpp__	1

#if AdobePrivate
// =================================================================================================
// Change history
// ==============
//
// Writers:
//	JEH Joerg Ehrlich
//	IJS Inder Jeet Singh
//
// mm-dd-yy who Description of changes, most recent on top
//
// 09-20-12 IJS 5.5-f035 Add Progress Notification flag to MPEG-4,PSD,AIFF,WAVE,ASF,TIFF,FLV and PS Handlers.
// 11-22-11 SAM 5.4-f039 [3014738] RIFF: always read/write the last chunk if there are duplicates
// 10-26-10 PW  5.3-f015 Integration from winwood
// 08-19-10 AWL 5.3-f003 Remove all use of the LFA_* names.
// 08-18-10 AWL 5.3-f002 Don't include XIO.hpp in any headers, only .cpp files.
// 08-17-10 AWL 5.3-f001 Integrate I/O revamp to main.
// 07-28-10	JEH	5.2-f001 Initial creation of new AIFF handler.
//
// =================================================================================================
#endif // AdobePrivate

#include "public/include/XMP_Environment.h"	// ! XMP_Environment.h must be the first included header.
#include "public/include/XMP_Const.h"

#include "XMPFiles/source/FormatSupport/IFF/ChunkController.h"
#include "XMPFiles/source/FormatSupport/IFF/IChunkBehavior.h"
#include "XMPFiles/source/FormatSupport/IFF/IChunkData.h"
#include "source/Endian.h"
#include "XMPFiles/source/FormatSupport/IFF/ChunkPath.h"
#include "XMPFiles/source/FormatSupport/AIFF/AIFFMetadata.h"
#include "source/XIO.hpp"

#include "XMPFiles/source/XMPFiles_Impl.hpp"

using namespace IFF_RIFF;

// =================================================================================================
/// \file AIFF_Handler.hpp
/// \brief File format handler for AIFF.
// =================================================================================================

/**
 * Contructor for the handler.
 */
extern XMPFileHandler * AIFF_MetaHandlerCTor ( XMPFiles * parent );

/**
 * Checks the format of the file, see common code.
 */
extern bool AIFF_CheckFormat ( XMP_FileFormat format,
							  XMP_StringPtr  filePath,
			                  XMP_IO*    fileRef,
			                  XMPFiles *     parent );

/** AIFF does not need kXMPFiles_CanRewrite as we can always use UpdateFile to either do
 *  in-place update or append to the file. */
static const XMP_OptionBits kAIFF_HandlerFlags = (kXMPFiles_CanInjectXMP |
												  kXMPFiles_CanExpand |
												  kXMPFiles_PrefersInPlace |
												  kXMPFiles_CanReconcile |
												  kXMPFiles_AllowsSafeUpdate |
												  kXMPFiles_CanNotifyProgress
												 );

/**
 * Main class for the the AIFF file handler.
 */
class AIFF_MetaHandler : public XMPFileHandler
{
public:
	AIFF_MetaHandler ( XMPFiles* parent );
	~AIFF_MetaHandler();

	void CacheFileData();
	void ProcessXMP();

	void UpdateFile ( bool doSafeUpdate );
    void WriteTempFile ( XMP_IO* tempRef );

	/**
	* Checks if the first 4 bytes of the given buffer are either type AIFF or AIFC
	* @param buffer a byte buffer that must contain at least 4 bytes and point to the correct byte
	* @return Either kType_AIFF, kType_AIFC 0 if no type could be determined
	*/
	static XMP_Uns32 whatAIFFFormat( XMP_Uns8* buffer );

private:
	/**
	 * Updates/creates/deletes a given legacy chunk depending on the given new legacy value
	 * If the Chunk exists and the value is not empty, it is updated. If the value is empty the
	 * Chunk is removed from the tree. If the Chunk does not exist but a value is given, it is created
	 * and initialized with that value
	 *
	 * @param chunk OUT pointer to the legacy chunk
	 * @param chunkID Id of the Chunk if it needs to be created
	 * @param legacyId ID of the legacy value
	 */
	void updateLegacyChunk( IChunkData **chunk, XMP_Uns32 chunkID, XMP_Uns32 legacyId );

	/**
	 * Finds the last non-empty annotation chunk in the given list
	 * @param annoChunks list of annotation chunks
	 * @return pointer to the first non-empty chunk or NULL
	 */
	IChunkData* selectLastNonEmptyAnnoChunk( const std::vector<IChunkData*> &annoChunks );

	/** private standard Ctor, not to be used */
	AIFF_MetaHandler (): mChunkController(NULL), mChunkBehavior(NULL),
						mAiffMeta(), mXMPChunk(NULL),
						mNameChunk(NULL), mAuthChunk(NULL),
						mCprChunk(NULL), mAnnoChunk(NULL) {};


	// ----- MEMBERS ----- //

	/** Controls the parsing and writing of the passed stream. */
	ChunkController *mChunkController;
	/** Represents the rules how chunks are added, removed or rearranged */
	IChunkBehavior *mChunkBehavior;
	/** container for Legacy metadata */
	AIFFMetadata mAiffMeta;

	/** pointer to the XMP chunk */
	IChunkData *mXMPChunk;
	/** pointer to legacy chunks */
	IChunkData *mNameChunk;
	IChunkData *mAuthChunk;
	IChunkData *mCprChunk;
	IChunkData *mAnnoChunk;

	/** Type of the file, either AIFF or AIFC */
	//XMP_Uns32 mFileType;


	// ----- CONSTANTS ----- //

	/** Chunk path identifier of interest in AIFF/AIFC */
	static const ChunkIdentifier kAIFFXMP[2];
	static const ChunkIdentifier kAIFCXMP[2];
	static const ChunkIdentifier kAIFFName[2];
	static const ChunkIdentifier kAIFCName[2];
	static const ChunkIdentifier kAIFFAuth[2];
	static const ChunkIdentifier kAIFCAuth[2];
	static const ChunkIdentifier kAIFFCpr[2];
	static const ChunkIdentifier kAIFCCpr[2];
	static const ChunkIdentifier kAIFFAnno[2];
	static const ChunkIdentifier kAIFCAnno[2];

	/** Path to XMP chunk */
	ChunkPath mAIFFXMPChunkPath;

	/** Path to NAME chunk */
	ChunkPath mAIFFNameChunkPath;

	/** Path to AUTH chunk */
	ChunkPath mAIFFAuthChunkPath;

	/** Path to COPYRIGHT chunk */
	ChunkPath mAIFFCprChunkPath;

	/** Path to ANNOTATION chunk */
	ChunkPath mAIFFAnnoChunkPath;

};	// AIFF_MetaHandler

// =================================================================================================

#endif /* __AIFF_Handler_hpp__ */
