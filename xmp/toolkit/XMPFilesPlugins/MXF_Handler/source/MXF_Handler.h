#if AdobePrivate	// *** All plugins are fully private for the time being.

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

#ifndef _MXF_HANDLER_H_
#define _MXF_HANDLER_H_
#if AdobePrivate
// =================================================================================================
// Change history
// ==============
//
// Writers:
//	AWL Alan Lillich
//	ADC Amandeep Chawla
//
// mm-dd-yy who Description of changes, most recent on top
//
// 05-15-13 ADC 5.6-f061 Integrating changes from DVA to MXF_Handler code base, Re factored class declaration and definition into separate files,
//						 and Added MXF_Handler into top level CMAKE.
//
//
// =================================================================================================
#endif // AdobePrivate

#include "public/include/XMP_Environment.h"	// ! XMP_Environment.h must be the first included header.

#include "public/include/XMP_Const.h"
#include "public/include/XMP_IO.hpp"

#include "source/EndianUtils.hpp"
#include "source/XMP_LibUtils.hpp"
#include "source/Host_IO.hpp"
#include "source/XMPFiles_IO.hpp"
#include "source/XIO.hpp"

#include "XMPFilesPlugins/api/source/PluginBase.h"
#include "XMPFilesPlugins/api/source/PluginRegistry.h"
#include "XMPFilesPlugins/api/source/HostAPIAccess.h"

namespace XMP_PLUGIN {

// =================================================================================================

class MXF_MetaHandler : public PluginBase {

public:

	MXF_MetaHandler ( const std::string& filePath, XMP_OptionBits openFlags, XMP_Uns32 format, XMP_Uns32 handlerFlags );
	virtual ~MXF_MetaHandler();

	// Minimum required virtual functions which need to be implemented by the plugin developer.
	virtual bool getFileModDate ( XMP_DateTime * modDate );
	virtual void cacheFileData ( const IOAdapter& file, std::string& xmpStr );
	virtual void updateFile ( const IOAdapter& file, bool doSafeUpdate, const std::string& xmpStr );
	virtual void FillMetadataFiles ( std::vector<std::string> * metadataFiles );
	virtual void FillAssociatedResources ( std::vector<std::string> * resourceList );
	virtual bool IsMetadataWritable ( ) ;

	// Static functions which also need to be implemented by the plugin developer.

	// This function is called to initialize the file handler. Don't get confuse by the file handler instance. 
	// This function is called to initialize the global stuff of the file handler. It may be am empty function
	// if nothing need to be initialized.
	// @return true on success otherwise false.
	static bool initialize();

	// This function is called to terminate the file handler.
	// @return true on success otherwise false.
	static bool terminate();

	// The following two functions need to be implemented to check the format. Though file handler 
	// will need only one of these but both of them need to be implemented. The actual check format 
	// function should be implemented properly while the other one will just return false.

	// @return true on success otherwise false.
	static bool checkFileFormat ( const std::string& filePath, const IOAdapter& file );

	// It's a dummy function and just return false. This function is not needed by the NormalHandler 
	// and OwningHandler but it still need to be implemented. 
	static bool checkFolderFormat ( const std::string& rootPath,
									const std::string& gpName,
									const std::string& parentName,
									const std::string& leafName );

	static int sInitCount;

private:

	std::string sidecarPath;
	XMP_IO * ioRef;

	void SetSidecarPath();

};

}
// =================================================================================================
#endif

#endif
