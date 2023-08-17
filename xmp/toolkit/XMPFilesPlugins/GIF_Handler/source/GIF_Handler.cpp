// =================================================================================================
// Copyright Adobe
// Copyright 2011 Adobe
// All Rights Reserved
//
// NOTICE: Adobe permits you to use, modify, and distribute this file in accordance with the terms
// of the Adobe license agreement accompanying it. If you have received this file from a source other 
// than Adobe, then your use, modification, or distribution of it requires the prior written permission
// of Adobe.
// =================================================================================================

#include "PluginBase.h"
#include "PluginRegistry.h"
#include "HostAPIAccess.h"

#include "HostAPIAccess.h"

#include "source/XMPFiles_IO.hpp"
#include "source/XIO.hpp"
#include "source/XMP_LibUtils.hpp"

#include "XMPCore/source/XMPMeta.hpp"

#include "GIF_Support.hpp"


namespace XMP_PLUGIN
{

// Plugin template file handler class. 
// This handler can be used to read/write XMP from/to text files
// All file handlers should be derived from PluginBase.
class GIF_Handler : public PluginBase
{
public:
	GIF_Handler( const std::string& filePath, XMP_Uns32 openFlags, XMP_Uns32 format, XMP_Uns32 handlerFlags )
		: PluginBase( filePath, openFlags, format, handlerFlags ) {}
	~GIF_Handler() {}

	//Minimum required virtual functions which need to be implemented by the plugin developer.
	virtual void cacheFileData( const IOAdapter& file, std::string& xmpStr );
	virtual void updateFile( const IOAdapter& file, bool doSafeUpdate, const std::string& xmpStr );

	// In case you want to directly support crash-safe updates
	virtual void writeTempFile( const IOAdapter& srcFile, const IOAdapter& tmpFile, const std::string& xmpStr );

	//Static functions which also need to be implemented by the plugin developer.
	
	// This function is called to initialize the file handler. Don't get confused by the file handler instance. 
	// It may be an empty function if nothing needs to be initialized.
	//@return true on success otherwise false.
	static bool initialize();
	
	//This function is called to terminate the file handler.
	//@return true on success otherwise false.
	static bool terminate();

	//The following two functions need to be implemented to check the format. Though file handler 
	//will need only one of these, both of them need to be implemented. The actual check format 
	//function should be implemented properly while the other one will just return false.
	
	//@return true on success otherwise false.
	static bool checkFileFormat( const std::string& filePath, const IOAdapter& file );
	
	// This function is not needed by a normal handler but an "owning" handler needs to implement it.
	static bool checkFolderFormat(	const std::string& rootPath, 
									const std::string& gpName, 
									const std::string& parentName, 
									const std::string& leafName );

private:
	void saveWriteFile( const IOAdapter& file, const std::string& xmpStr );
	void ReconcileXMP( const std::string &xmpStr, std::string *outStr );

	static int init;
	static const std::string XPACKET_HEADER;
	static const std::string XMPMETA_HEADER;
};

int GIF_Handler::init = 0;
const std::string GIF_Handler::XPACKET_HEADER = "<?xpacket";
const std::string GIF_Handler::XMPMETA_HEADER = "<x:xmpmeta";

//============================================================================
// registerHandler GIF_Handler
//============================================================================

// Return module identifier string. This string should be same as the string present in 
// the resource file otherwise plugin won't be loaded.
// This function needs to be implemented by the plugin developer.
const char* GetModuleIdentifier()
{
	static const char* kModuleIdentifier = "com.adobe.xmp.plugins.gif";
	return kModuleIdentifier;
}

// All the file handlers present in this module will be registered. There may be many file handlers
// inside one module. Only those file handlers which are registered here will be visible to XMPFiles.
// This function need to be implemented by the plugin developer.
//@param sXMPTemplate uid of the file handler, 
void RegisterFileHandlers()
{
	//uid of the file handler. It's different from the module identifier.
	const char* sXMP_GIFHandler = "adobe.coretech.xmp.gif"; 
	PluginRegistry::registerHandler( new PluginCreator<GIF_Handler>(sXMP_GIFHandler) );
}

//============================================================================
// GIF_Handler specific functions
//============================================================================

bool GIF_Handler::initialize()
{
	init++;

	if(init != 1) return true;

	{
		SXMPMeta::Initialize();
	}

	return true;
}


bool GIF_Handler::terminate()
{
	init--;

	if(init != 0) return true;

	SXMPMeta::Terminate();

	return true;
}


bool GIF_Handler::checkFileFormat( const std::string& filePath, const IOAdapter& file )
{
	try 
	{
		const XMP_Uns8 SIGNATURE_LEN = 6;
        const char* const SIGNATURE_DATA = "\x47\x49\x46\x38\x39\x62"; // GIF89a, older format doesn't support custom data
		

		if ( file.Length() < SIGNATURE_LEN ) return false;

		char buffer[SIGNATURE_LEN];
		XMP_Int64 offset = 0;
		file.Seek ( offset, kXMP_SeekFromStart );
		file.Read ( buffer, SIGNATURE_LEN, true );

		return ( memcmp(buffer, SIGNATURE_DATA, SIGNATURE_LEN) == 0 );
	} 
	catch ( ... ) 
	{
		return false;
	}

	return false;
}


// This function is not needed by a normal handler but an "owning" handler needs to implement it.
bool GIF_Handler::checkFolderFormat( const std::string& rootPath, 
										 const std::string& gpName,
										 const std::string& parentName,
										 const std::string& leafName )
{
	return false;
}


void GIF_Handler::cacheFileData( const IOAdapter& file, std::string& xmpStr )
{
	xmpStr.erase();
		
	// Navigate through the GIF blocks to find the XMP block.
	GIF_Support::BlockState blockState;
	long numBlocks = GIF_Support::OpenGIF ( file, blockState );
	if ( numBlocks == 0 ) return;
    
	XMP_Uns32 xmpLength = blockState.xmpLen;
	if ( xmpLength > 1024*1024*1024 ) 
	{	// Sanity check and to not overflow std::string.
		XMP_Throw ( "XMP file is too large", kXMPErr_PluginCacheFileData );
	}

	if (xmpLength > 0)
	{
		xmpStr.assign(xmpLength, ' ');
        
		XMP_Int64 offset = blockState.xmpPos;
		file.Seek ( offset, kXMP_SeekFromStart );
		file.Read ( (void*)xmpStr.c_str(), xmpLength, true ); //TODO: unsafe to modify array pointed to by c_str()
	}
}


void GIF_Handler::updateFile( const IOAdapter& file, bool doSafeUpdate, const std::string& xmpStr )
{
	// safe update option only relevant for handlers which "own" the file I/O
	// for other handlers the method writeTempFile is called in case the client wants a safe update
	if ( doSafeUpdate )
	{
		XMP_Throw ( "GIF_Handler::updateFile: Safe update not supported", kXMPErr_Unavailable );
	}
	
	GIF_Support::BlockState blockState;
	long numBlocks = GIF_Support::OpenGIF ( file, blockState );
	if ( numBlocks == 0 ) return;
	
	// Example how to use XMPCore to edit the XMP data before export
	// Please note that if you do not want to explicitely edit the XMP at this point
	// you can write it directly to the file as it is already correctly serialized
	// as indicated by the plugin manifest serialize options
	std::string outStr;
	ReconcileXMP(xmpStr, &outStr);
	if ( outStr.empty() ) return;
	
	XMP_Uns32 strLen = static_cast<XMP_Uns32>(outStr.length());
	
	//TODO: smarter write to just append XMP block before GIF trailer
	
	// write/update block(s)
	if (blockState.xmpLen == 0)
	{
		// no current chunk -> inject
		saveWriteFile(file, outStr);
	}
	else if (blockState.xmpLen >= strLen)
	{
		// current chunk size is sufficient -> write XMP in place
		XMP_Uns32 padLen = blockState.xmpLen - strLen;
		outStr.append(padLen, ' ');
		XMP_Int64 pos = blockState.xmpPos;
		GIF_Support::WriteBuffer( file, pos, outStr.length(), outStr.c_str() );
		
	}
	else if (blockState.xmpLen < strLen)
	{
		// XMP is too large for current block -> expand
		saveWriteFile(file, outStr);
	}
}

void GIF_Handler::saveWriteFile( const IOAdapter& file, const std::string& xmpStr )
{
	IOAdapter tmpFile( file.DeriveTemp() );
	try
	{
		this->writeTempFile( file, tmpFile, xmpStr );
		file.AbsorbTemp();
	}
	catch ( ... )
	{
		file.DeleteTemp();
		throw;
	}
}

void GIF_Handler::writeTempFile( const IOAdapter& srcFile, const IOAdapter& tmpFile, const std::string& xmpStr )
{
	GIF_Support::BlockState blockState;
	long numBlocks = GIF_Support::OpenGIF ( srcFile, blockState );
	if ( numBlocks == 0 ) return;
	
	// example how to use XMPCore to edit the XMP data before export
	std::string outStr;
	ReconcileXMP(xmpStr, &outStr);
	
	tmpFile.Truncate ( 0 );
	
	GIF_Support::BlockIterator curPos = blockState.blocks.begin();
	GIF_Support::BlockIterator endPos = blockState.blocks.end();
	
	long blockCount;
	
	for (blockCount = 0; (curPos != endPos); ++curPos, ++blockCount)
	{
		GIF_Support::BlockData block = *curPos;
		
		// discard existing XMP block
		if (! block.xmp)
		{
			// copy any other block
			GIF_Support::CopyBlock(srcFile, tmpFile, block);
		}
		
		// place XMP block immediately before trailer
		if (blockCount == numBlocks - 2)
		{
			GIF_Support::WriteXMPBlock(tmpFile, static_cast<XMP_Uns32>(outStr.length()), outStr.c_str() );
		}
	}
}


void GIF_Handler::ReconcileXMP( const std::string& xmpStr, std::string* outStr ) 
{
	// This is an example how to use XMPCore to edit the XMP data
	SXMPMeta xmp;
	xmp.ParseFromBuffer( xmpStr.c_str(), xmpStr.length() );

	xmp.SetProperty( kXMP_NS_XMP, "CreatorTool", "XMP GIFPlugin" );

	xmp.SerializeToBuffer( outStr, kXMP_UseCompactFormat | kXMP_OmitPacketWrapper );
}

	
} //namespace XMP_PLUGIN
