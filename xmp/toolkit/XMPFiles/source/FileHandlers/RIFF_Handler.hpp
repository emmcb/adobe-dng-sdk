// =================================================================================================
// Copyright Adobe
// Copyright 2009 Adobe
// All Rights Reserved
//
// NOTICE: Adobe permits you to use, modify, and distribute this file in accordance with the terms
// of the Adobe license agreement accompanying it. If you have received this file from a source other 
// than Adobe, then your use, modification, or distribution of it requires the prior written permission
// of Adobe.
// =================================================================================================
#ifndef __RIFF_Handler_hpp__
#define __RIFF_Handler_hpp__	1

#if AdobePrivate
// =================================================================================================
// Change history
// ==============
//
// Writers:
//	AWL Alan Lillich
//  FNO Frank Nocke
//
// mm-dd-yy who Description of changes, most recent on top
//
// 08-18-10 AWL 5.3-f002 Don't include XIO.hpp in any headers, only .cpp files.
// 08-17-10 AWL 5.3-f001 Integrate I/O revamp to main.
//
// 11-16-09 AWL 5.0-f112 [2448942] Tolerate limited amounts of trailing garbage in RIFF files.
// 11-06-09 FNO 5.0-f105 [2473381] Fixed Cr8r,PrmL,TDat last-chunk-creation.
// 11-06-09 FNO 5.0-f104 [2473381] Assert 2/4GB border, prevent main growth, remove Index Adjustments.
// 11-06-09 FNO 5.0-f103 [2457482] Cr8r,PrmL read support.
// 11-03-09 FNO 5.0-f097 RIFF: Minor functional optimizations, comments, indentation, cleanup.
// 10-30-09 FNO 5.0-f096 [2465689] Fix avi indices. Metadata export re-enabled.
// 07-29-09 FNO 5.0-f058 [2389942] XMP props not properly preserved in ServerMode.
// 07-10-09 FNO 5.0-f052 Move RIFF (AVI/WAV) handler from NewHandlers to regular Handlers.
// 05-07-09 FNO 5.0-f036 Start combined RIFF(AVI/WAV) Handler rewrite.
//
// =================================================================================================
#endif // AdobePrivate

#include "public/include/XMP_Environment.h"	// ! XMP_Environment.h must be the first included header.
#include "public/include/XMP_Const.h"

#include "XMPFiles/source/FormatSupport/RIFF_Support.hpp"
#include "source/XIO.hpp"

// =================================================================================================
/// \file RIFF_Handler.hpp
/// \brief File format handler for RIFF (AVI, WAV).
// =================================================================================================

extern XMPFileHandler * RIFF_MetaHandlerCTor ( XMPFiles * parent );

extern bool RIFF_CheckFormat ( XMP_FileFormat format,
							  XMP_StringPtr  filePath,
			                  XMP_IO *       fileRef,
			                  XMPFiles *     parent );

static const XMP_OptionBits kRIFF_HandlerFlags = (kXMPFiles_CanInjectXMP |
												  kXMPFiles_CanExpand |
												  kXMPFiles_PrefersInPlace |
												  kXMPFiles_AllowsOnlyXMP |
												  kXMPFiles_ReturnsRawPacket |
												  kXMPFiles_CanReconcile
												 );

class RIFF_MetaHandler : public XMPFileHandler
{
public:
	RIFF_MetaHandler ( XMPFiles* parent );
	~RIFF_MetaHandler();

	void CacheFileData();
	void ProcessXMP();

	void UpdateFile ( bool doSafeUpdate );
    void WriteTempFile ( XMP_IO* tempRef );


	////////////////////////////////////////////////////////////////////////////////////
	// instance vars
	// most often just one RIFF:* (except for AVI,[AVIX] >1 GB)
	std::vector<RIFF::ContainerChunk*> riffChunks;
	XMP_Int64 oldFileSize, newFileSize, trailingGarbageSize;

	// state variables, needed during parsing
	XMP_Uns8  level;

	RIFF::ContainerChunk *listInfoChunk, *listTdatChunk,*listHdlrChunk;
	RIFF::ValueChunk* dispChunk;
	RIFF::ValueChunk* bextChunk;
	RIFF::ValueChunk* cr8rChunk;
	RIFF::ValueChunk* prmlChunk;
	RIFF::ValueChunk* iditChunk;
	RIFF::XMPChunk* xmpChunk;
	RIFF::ContainerChunk* lastChunk;
	bool hasListInfoINAM; // needs to be known for the special 3-way merge around dc:title

};	// RIFF_MetaHandler

// =================================================================================================

#endif /* __RIFF_Handler_hpp__ */
