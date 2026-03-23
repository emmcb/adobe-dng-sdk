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
//	ADC Amandeep Chawla
//  IJS Inder Jeet Singh
//  JKR Jens Krueger
//
// mm-dd-yy who Description of changes, most recent on top
//
// 02-11-13 JKR 5.6-f033 Reworked plugin versioning to be able to load all supported older and newer plugins.
// 01-11-13 IJS 5.6-f020 Reworked GetFileModDate, GetAssociatedResources and IsMetadataWritable APIs.
//                       Added IsMetadataWritable Plugin API in the Plugin Architechture.Bumped the Plugin API to version 3.
// 12-21-12 IJS 5.6-f009 Added FillAssociatedResources support in plugins 
//
// 11-05-12 ADC 5.5-f056 XMP Plug-ins support for LocateMetadataFiles API.
//
// =================================================================================================
#endif // AdobePrivate

#ifndef __PLUGIN_CONST_H__
#define __PLUGIN_CONST_H__

#include "XMP_Const.h"

typedef void * StringVectorRef;
typedef void (* SetStringVectorProc) ( StringVectorRef vectorRef, XMP_StringPtr * arrayPtr, XMP_Uns32 stringCount );

enum 
{
	/// Plugin-internal failures
	kXMPErr_PluginInternal			      = 500,
	/// 
	kXMPErr_PluginInitialized		      = 501,
	/// 
	kXMPErr_PluginTerminate			      = 502,
	/// 
	kXMPErr_PluginSessionInit		      = 503,
	/// 
	kXMPErr_PluginSessionTerm		      = 504,
	/// 
	kXMPErr_PluginCacheFileData		      = 505,
	/// 
	kXMPErr_PluginUpdateFile		      = 506,
	/// 
	kXMPErr_PluginWriteTempFile		      = 507,
	/// 
	kXMPErr_PluginImportToXMP		      = 508,
	/// 
	kXMPErr_PluginExportFromXMP		      = 509,
	/// 
	kXMPErr_PluginCheckFileFormat	      = 510,
	/// 
	kXMPErr_PluginCheckFolderFormat	      = 511,
	///
	kXMPErr_SetHostAPI	                  = 512,
	///
	kXMPErr_PluginGetFileModDate          = 513,
	///
	kXMPErr_PluginFillMetadataFiles       = 514,
	///
	kXMPErr_PluginFillAssociatedResources = 515,
	///
	kXMPErr_PluginIsMetadataWritable	  = 516,
	
	/// last plugin error, please add new errors before this one
	kXMPErr_PluginLastError
};

#endif	// __PLUGIN_CONST_H__
