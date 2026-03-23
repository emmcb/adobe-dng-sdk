#ifndef __IOUtils_hpp__
#define __IOUtils_hpp__	1

// =================================================================================================
// Copyright 2013 Adobe
// All Rights Reserved.
//
// NOTICE:  Adobe permits you to use, modify, and distribute this file in accordance with the terms
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
//
//	IJS Inder Jeet Singh
//	ADC Amandeep Chawla
//
// mm-dd-yy who Description of changes, most recent on top.
//
// 04-26-13 ADC 5.6-f054 [3526891] Read-only checks in IsMetadataWritable() API try creating temporary files if no metadata exists, instead of checking for file permissions.
//						 [3525961] IsMetaDataWritable checks for Plugins and embedded handlers are different.
// 04-04-13 IJS 5.6-f051 Move common I/O methods to IOUtils.
//						 [3534631] Optimize GetAssociated Resources for RED_Handler
//
// =================================================================================================
#endif

#include "public/include/XMP_Environment.h"	
#include "public/include/XMP_Const.h"

#include "source/XMP_LibUtils.hpp"

//Helper class for common IO function
class IOUtils
{
public:
	// Returns the list of folders or files matching particular string format in the given Directory
	static void GetMatchingChildren ( XMP_StringVector & matchingChildList, const XMP_VarString & rootPath,
		const XMP_StringVector & regExStringVec, XMP_Bool includeFolders, XMP_Bool includeFiles, XMP_Bool prefixRootPath );

	// Returns the list of folders or files matching particular string format in the given Directory
	static void GetMatchingChildren ( XMP_StringVector & matchingChildList, const XMP_VarString & rootPath,
		const XMP_VarString & regExpStr, XMP_Bool includeFolders, XMP_Bool includeFiles, XMP_Bool prefixRootPath );
};

#endif	// __IOUtils_hpp__
