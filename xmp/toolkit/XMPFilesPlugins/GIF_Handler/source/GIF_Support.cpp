// =================================================================================================
// Copyright Adobe
// Copyright 2008 Adobe
// All Rights Reserved
//
// NOTICE: Adobe permits you to use, modify, and distribute this file in accordance with the terms
// of the Adobe license agreement accompanying it. If you have received this file from a source other 
// than Adobe, then your use, modification, or distribution of it requires the prior written permission
// of Adobe.
//
// Derived from PNG_Support.cpp by Ian Jacobi
// =================================================================================================
//
//
#if AdobePrivate
// =================================================================================================
// Change history 
// ==============
//
// Writers:
//	FNO Frank Nocke
//
// mm-dd-yy who Description of changes, most recent on top
//
// 08-19-10 AWL 5.3-f004 Move the seek mode constants to XMP_Const.
// 08-19-10 AWL 5.3-f003 Remove all use of the LFA_* names.
// 08-17-10 AWL 5.3-f001 Integrate I/O revamp to main.
//
// 02-25-08 FNO 4.2-f084 Integrate minor tweaks for Solaris
//
// =================================================================================================
#endif // AdobePrivate

#include "public/include/XMP_Environment.h"	// ! Must be the first #include!

#include "GIF_Support.hpp"
#include "HostAPIAccess.h"
#include "source/XIO.hpp"

#include <string.h>

typedef std::basic_string<unsigned char> filebuffer;

//
// see http://www.w3.org/Graphics/GIF/spec-gif89a.txt
//

namespace GIF_Support
{
        // This only really counts for extension blocks.
	enum blockType
    {
		bGraphicControl = 0xF9,
		bComment        = 0xFE,
		bPlainText      = 0x01,
		bApplication    = 0xFF,
		// Hacky.  Don't like the following, but there's no easy way.
		bImage          = 0x2C,
		bExtension      = 0x21,
		bTerminator     = 0x3B,
		bHeader         = 0x47
	};

	// =============================================================================================

	long OpenGIF ( const XMP_PLUGIN::IOAdapter& file, BlockState & inOutBlockState )
	{
		XMP_Uns64 pos = 0;
		unsigned char name;
		XMP_Uns32 len;
		BlockData newBlock;

		try
		{
			XMP_Int64 offset = 0;
			file.Seek ( offset, kXMP_SeekFromStart );

			// header needs to be a block, mostly for our safe write.
			pos = ReadHeader ( file );
			if (pos < 13) return 0;

			newBlock.pos = 0;
			newBlock.len = (XMP_Uns32)pos;
			newBlock.type = bHeader;
			inOutBlockState.blocks.push_back(newBlock);

			// read first and following blocks
			while ( ReadBlock ( file, inOutBlockState, &name, &len, pos) ) {}

			return (long)inOutBlockState.blocks.size(); //* change return type?
		}
		catch (...)
		{
			return 0;
		}
	}

	// =============================================================================================

	long ReadHeader ( const XMP_PLUGIN::IOAdapter& file ) // throw XMPError
	{
		XMP_Uns32 bytesRead;
		XMP_Uns32 headerSize;
		long tableSize = 0;
		long bytesPerColor = 0;
		unsigned char buffer[768];
		
		headerSize = 0;

		// Header
		bytesRead = file.Read ( buffer, GIF_SIGNATURE_LEN, true );
		if ( memcmp ( buffer, GIF_SIGNATURE_DATA, GIF_SIGNATURE_LEN) != 0 ) return 0;
		headerSize += bytesRead;
		
		// Logical Screen Descriptor
		bytesRead = file.Read ( buffer, 4, true );
		headerSize += bytesRead;
		bytesRead = file.Read ( buffer, 3, true );
		headerSize += bytesRead;

		// Global Color Table (optional)
		bool hasGlobalColorTable = ((buffer[0] & 0x80) != 0);
		if ( hasGlobalColorTable ) tableSize = (1 << ((buffer[0] & 0x07) + 1)) * 3;
		bytesRead = file.Read ( buffer, tableSize, true );
		headerSize += bytesRead;
		
		return headerSize;
	}

	// =============================================================================================

	bool  ReadBlock ( const XMP_PLUGIN::IOAdapter& file, BlockState & inOutBlockState, unsigned char * blockType, XMP_Uns32 * blockLength, XMP_Uns64 & inOutPosition )
	{
        // 0x00-0x7F (0-127) are the Graphic Rendering blocks, excluding the Trailer (0x3B);
        // 0x80-0xF9 (128-249) are the Control blocks;
        // 0xFA-0xFF (250-255) are the Special Purpose blocks
		
        try
		{
			XMP_Uns64 startPosition = inOutPosition;
			XMP_Uns32 bytesRead;
			unsigned char buffer[768];

			bytesRead = file.Read ( buffer, 1, true );
			inOutPosition += 1;
			if ( buffer[0] == bImage )
			{
				// Image is a special case.
                long tableSize = 0;

                // Image Left/Top
				bytesRead = file.Read ( buffer, 4, true );
				inOutPosition += 4;

                // Image Width/Height
                bytesRead = file.Read ( buffer, 4, true );
				inOutPosition += 4;

                // Flags
				bytesRead = file.Read ( buffer, 1, true );
				inOutPosition += 1;

                // Local Color Table
                bool hasLocalColorTable = ((buffer[0] & 0x80) != 0);
				if ( hasLocalColorTable ) tableSize = (1 << ((buffer[0] & 0x07) + 1)) * 3;
				bytesRead = file.Read ( buffer, tableSize, true );
				inOutPosition += tableSize;

                // TODO: check if Graphic Control Extension before Table Based Image Data is supported
				bytesRead = file.Read ( buffer, 1, true );
				inOutPosition += 1;
				bytesRead = file.Read ( buffer, 1, true );
				inOutPosition += 1;
				tableSize = buffer[0];
				while ( tableSize != 0x00 )
				{
					bytesRead = file.Read ( buffer, tableSize, true );
					inOutPosition += tableSize;
					bytesRead = file.Read ( buffer, 1, true );
					inOutPosition += 1;
					tableSize = buffer[0];
				}

				BlockData newBlock;

				newBlock.pos = startPosition;
				newBlock.len = (XMP_Uns32)(inOutPosition - startPosition);
				newBlock.type = bImage;

				inOutBlockState.blocks.push_back ( newBlock );
			}
			else if ( buffer[0] == bExtension )
			{
				unsigned char type;
				long tableSize = 0;

				BlockData newBlock;

				newBlock.pos = startPosition;

				bytesRead = file.Read ( buffer, 1, true );
				inOutPosition += 1;
				type = buffer[0];
				newBlock.type = type;

				bytesRead = file.Read ( buffer, 1, true );
				inOutPosition += 1;
				tableSize = buffer[0];
				while ( tableSize != 0x00 )
				{
					bytesRead = file.Read ( buffer, tableSize, true );
					inOutPosition += tableSize;
					if ( inOutPosition - startPosition == 14 && type == bApplication )
					{
						// Check for XMP identifier...
						CheckApplicationBlockHeader ( file, inOutBlockState, newBlock, inOutPosition );

						if ( newBlock.xmp == true )
						{
							newBlock.len = (XMP_Uns32)(inOutPosition - startPosition);
							inOutBlockState.blocks.push_back ( newBlock );
							return true;
						}
					}
					bytesRead = file.Read ( buffer, 1, true );
					inOutPosition += 1;
					tableSize = buffer[0];
				}

				newBlock.len = (XMP_Uns32)(inOutPosition - startPosition);
				inOutBlockState.blocks.push_back ( newBlock );
			}
			else if ( buffer[0] == bTerminator )
			{
				BlockData newBlock;

				newBlock.pos = startPosition;
				newBlock.len = 1;
				newBlock.type = bTerminator;

				inOutBlockState.blocks.push_back ( newBlock );
			}

		} catch ( ... ) {

			return false;

		}
	
		return true;

	}

	// =============================================================================================

	bool WriteXMPBlock ( const XMP_PLUGIN::IOAdapter& file, XMP_Uns32 len, const char* inBuffer )
	{
		bool ret = false;
		unsigned long datalen = (APPLICATION_HEADER_LEN + len + MAGIC_TRAILER_LEN);
		unsigned char* buffer = new unsigned char[datalen];

		try
		{
			size_t pos = 0;
			memcpy(&buffer[pos], APPLICATION_HEADER_DATA, APPLICATION_HEADER_LEN);
			pos += APPLICATION_HEADER_LEN;
			memcpy(&buffer[pos], inBuffer, len);
			pos += len;
			memcpy(&buffer[pos], MAGIC_TRAILER_DATA, MAGIC_TRAILER_LEN);

			file.Write ( buffer, datalen );

			ret = true;
		}
		catch ( ... )
		{
		}

		delete [] buffer;

		return ret;
	}

	// =============================================================================================

	bool CopyBlock ( const XMP_PLUGIN::IOAdapter& source, const XMP_PLUGIN::IOAdapter& dest, BlockData& block )
	{
		try
		{
			XMP_Int64 offset = block.pos;
			source.Seek ( offset, kXMP_SeekFromStart  );
			
			// copy, TODO: check abort
			XMP_Int64 length = block.len;
			XMP_Uns8 buffer [64*1024];
			
			while ( length > 0 )
			{
				XMP_Uns32 ioCount = sizeof(buffer);
				if ( length < ioCount ) ioCount = (XMP_Uns32)length;
				
				source.Read ( buffer, ioCount, true );
				dest.Write ( buffer, ioCount );
				length -= ioCount;
				
			}
		}
		catch ( ... )
		{
			return false;
		}
	
		return true;
	}

	// =============================================================================================

	// Don't need CRC.

	// =============================================================================================

	unsigned long CheckApplicationBlockHeader ( const XMP_PLUGIN::IOAdapter& file, BlockState& inOutBlockState, BlockData& inOutBlockData, XMP_Uns64& inOutPosition )
	{
		try
		{
			XMP_Int64 offset = inOutBlockData.pos;
			file.Seek ( offset, kXMP_SeekFromStart );

			unsigned char buffer[256];
			long bytesRead = file.Read ( buffer, APPLICATION_HEADER_LEN, true );

			if (memcmp(buffer, APPLICATION_HEADER_DATA, APPLICATION_HEADER_LEN) == 0)
			{
				// We still have to go through all of the data...
				long tableSize = 0;
				
				inOutPosition = inOutBlockData.pos + APPLICATION_HEADER_LEN;
				inOutBlockState.xmpPos = inOutPosition;
				bytesRead = file.Read ( buffer, 1, true );
				inOutPosition += 1;
				tableSize = buffer[0];
				while ( tableSize != 0x00 )
				{
					bytesRead = file.Read ( buffer, tableSize, true );
					inOutPosition += tableSize;
					bytesRead = file.Read ( buffer, 1, true );
					inOutPosition += 1;
					tableSize = buffer[0];
				}
				inOutBlockState.xmpLen = (XMP_Uns32)(inOutPosition - inOutBlockData.pos - APPLICATION_HEADER_LEN - MAGIC_TRAILER_LEN);
				inOutBlockState.xmpBlock = inOutBlockData;
				inOutBlockData.xmp = true;
			}
		}
		catch ( ... )
		{
		}
	
		return 0;
	}

	bool ReadBuffer ( const XMP_PLUGIN::IOAdapter& file, XMP_Int64 & pos, XMP_Uns32 len, char * outBuffer )
	{
		try
		{
			if ( outBuffer == 0 ) return false;
		
			file.Seek ( pos, kXMP_SeekFromStart  );
			file.Read ( outBuffer, len, true );
		
			return true;
		}
		catch ( ... )
		{
		}

		return false;
	}

	bool WriteBuffer ( const XMP_PLUGIN::IOAdapter& file, XMP_Int64 & pos, XMP_Uns32 len, const char * inBuffer )
	{
		try
		{
			if ( inBuffer == 0 ) return false;
		
			file.Seek ( pos, kXMP_SeekFromStart  );
			file.Write ( (void*)inBuffer, len  );
		
			return true;
		}
		catch ( ... ) {}

		return false;
	}

} // namespace GIF_Support
