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

#include "XMPFiles/source/FormatSupport/WebP/WebPBehavior.h"
#include "XMPFiles/source/FormatSupport/IFF/Chunk.h"
#include "source/XMP_LibUtils.hpp"
#include "source/XIO.hpp"

#include <algorithm>

using namespace IFF_RIFF;

//
// Static init
//
const LittleEndian& WebPBehavior::mEndian = LittleEndian::getInstance();

//-----------------------------------------------------------------------------
// 
// WebPBehavior::getRealSize(...)
// 
// Purpose: Validate the passed in size value, identify the valid size if the 
//			passed in isn't valid and return the valid size.
//			Throw an exception if the passed in size isn't valid and there's 
//			no way to identify a valid size.
// 
//-----------------------------------------------------------------------------

XMP_Uns64 WebPBehavior::getRealSize( const XMP_Uns64 size, const ChunkIdentifier& id, IChunkContainer& tree, XMP_IO* stream )
{
	if( (size & 0xFFFFFFF5) > 0 ) // 4GB -2 - 4 -4
	{
		XMP_Throw( "Unknown size value", kXMPErr_BadFileFormat );
	}

	return size;
}

//-----------------------------------------------------------------------------
// 
// WebPBehavior::isValidTopLevelChunk(...)
// 
// Purpose: Return true if the passed identifier is valid for top-level chunks 
//			of a certain format.
// 
//-----------------------------------------------------------------------------

bool WebPBehavior::isValidTopLevelChunk( const ChunkIdentifier& id, XMP_Uns32 chunkNo )
{
	return (chunkNo == 0) && (id.id == kChunk_RIFF) && ((id.type == kType_WebP) );
}

//-----------------------------------------------------------------------------
// 
// WebPBehavior::getMaxChunkSize(...)
// 
// Purpose: Return the maximum size of a single chunk, i.e. the maximum size 
//			of a top-level chunk.
// 
//-----------------------------------------------------------------------------

XMP_Uns64 WebPBehavior::getMaxChunkSize() const
{
	return 0xFFFFFFF5LL;	// 4GB - 10
}

void WebPBehavior::fixHierarchy( IChunkContainer& tree )
{
	// Not required for read, not implemented
	XMP_Throw("Not implemented", kXMPErr_Unimplemented);

}

void WebPBehavior::insertChunk( IChunkContainer& tree, Chunk& chunk )
{
	// Not required for read, not implemented
	XMP_Throw("Not implemented", kXMPErr_Unimplemented);
}

bool WebPBehavior::removeChunk( IChunkContainer& tree, Chunk& chunk )
{
	// Not required for read, not implemented
	XMP_Throw("Not implemented", kXMPErr_Unimplemented);
}

XMP_Bool WebPBehavior::isFREEChunk( const Chunk& chunk ) const
{
	// Not required for read, not implemented
	XMP_Throw("Not implemented", kXMPErr_Unimplemented);
}

Chunk* WebPBehavior::createFREE( XMP_Uns64 chunkSize )
{
	// Not required for read, not implemented
	XMP_Throw("Not implemented", kXMPErr_Unimplemented);

}

XMP_Uns64 WebPBehavior::getMinFREESize() const
{
	// Not required for read, not implemented
	XMP_Throw("Not implemented", kXMPErr_Unimplemented);
}

#endif // AdobePrivate
