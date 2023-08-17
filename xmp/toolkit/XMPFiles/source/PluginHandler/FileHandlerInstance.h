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

#if AdobePrivate
// =================================================================================================
// Change history
// ==============
//
// Writers:
//	PKG Praveen Kumar Goyal
//	ADC Amandeep Chawla
//  IJS Inder Jeet Singh
//	AB  Amit Bhatti
//
// mm-dd-yy who Description of changes, most recent on top
//
// 01-05-15	AB	5.6-f122 Provide more functionalities to Plugin( Existing XMP packet, PacketInfo, OpenFlags, Error Callback and progress notification),
//						 more standard handler access API getFileModDate,IsMetadataWritable,putXMP,getAssociatedResources.
//						 New plugin handler for MPEG4 with Exif support.
// 01-11-13 IJS 5.6-f020 Added IsMetadataWritable Plugin API in the Plugin Architechture.Bumped the Plugin API to version 3.
// 12-21-12 IJS 5.6-f009 Added FillAssociatedResources support in plugins 
//
// 11-05-12 ADC 5.5-f056 XMP Plug-ins support for LocateMetadataFiles API.
//
// 09-25-11 PW  5.4-f013 Add support to access replaced file handler
// 09-23-11 AWL 5.4-f012 Add GetFileModDate.
// 06-27-11 PKG 5.4-f001 Initial checkin of plugin architecture.
//
// =================================================================================================
#endif // AdobePrivate

#ifndef PLUGINHANDLERINSTANCE_H
#define PLUGINHANDLERINSTANCE_H
#include "FileHandler.h"

namespace XMP_PLUGIN
{

/** @class FileHandlerInstance
 *  @brief This class is equivalent to the native file handlers like JPEG_MetaHandler or the simialr one.
 *
 *  This class is equivalent to the native file handler. This class is supposed to support all the 
 *  function which a native file handler support.
 *  As of now, it support only the functions required for OwningFileHandler.
 */
class FileHandlerInstance : public XMPFileHandler
{
public:
	FileHandlerInstance ( SessionRef object, FileHandlerSharedPtr handler, XMPFiles * parent );
	virtual ~FileHandlerInstance();

	virtual bool GetFileModDate ( XMP_DateTime * modDate );

	virtual void CacheFileData();
	virtual void ProcessXMP();
	//virtual XMP_OptionBits GetSerializeOptions(); //It should not be needed as its required only inside updateFile.
	virtual void UpdateFile ( bool doSafeUpdate );
	virtual void WriteTempFile ( XMP_IO* tempRef );
	virtual void FillMetadataFiles ( std::vector<std::string> * metadataFiles );
	virtual void FillAssociatedResources ( std::vector<std::string> * resourceList );
	virtual bool IsMetadataWritable ( );
	virtual void SetErrorCallback ( ErrorCallbackBox errorCallbackBox );
	virtual void SetProgressCallback ( XMP_ProgressTracker::CallbackInfo * progCBInfoPtr );

	inline SessionRef				GetSession() const	{ return mObject; }
	inline FileHandlerSharedPtr		GetHandlerInfo() const	{ return mHandler; }

private:
	SessionRef			mObject;
	FileHandlerSharedPtr	mHandler;
};

} //namespace XMP_PLUGIN
#endif
