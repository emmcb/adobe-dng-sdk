#ifndef __RIFF_Support_hpp__
#define __RIFF_Support_hpp__ 1

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
// 10-30-09 FNO 5.0-f096 [2465689] Fix avi indices. Metadata export re-enabled.
// 10-23-09 FNO 5.0-f092 RIFF: Substantial rewrite of chunk logic to prepare for index adjustments.
// 07-10-09 FNO 5.0-f052 Move RIFF (AVI/WAV) handler from NewHandlers to regular Handlers.
// 05-07-09 FNO 5.0-f036 Start combined RIFF(AVI/WAV) Handler rewrite.
//
// =================================================================================================
#endif // AdobePrivate

#include "public/include/XMP_Environment.h"	// ! This must be the first include.
#include <vector>
#include "XMPFiles/source/XMPFiles_Impl.hpp"

// ahead declaration:
class RIFF_MetaHandler;

namespace RIFF {

	// declare ahead
	class Chunk;
	class ContainerChunk;
	class ValueChunk;
	class XMPChunk;
	
	/* This rountines imports the properties found into the 
		xmp packet. Use after parsing. */
	void importProperties( RIFF_MetaHandler* handler );

	/* This rountines exports XMP properties to the respective Chunks,
	   creating those if needed. No writing to file here. */
	void exportAndRemoveProperties( RIFF_MetaHandler* handler );

	/* will relocated a wrongly placed  chunk (one of XMP, LIST:Info, LIST:Tdat=
	   from RIFF::avix back to main chunk. Chunk itself not touched. */
	void relocateWronglyPlacedXMPChunk( RIFF_MetaHandler* handler );

} // namespace RIFF

#endif	// __RIFF_Support_hpp__
