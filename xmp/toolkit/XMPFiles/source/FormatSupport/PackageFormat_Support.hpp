#ifndef __PackageFormat_Support_hpp__
#define __PackageFormat_Support_hpp__	1

// =================================================================================================
// Copyright Adobe
// Copyright 2013 Adobe
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
//	HK  Honey Kansal
//	IJS Inder Jeet Singh
//
// mm-dd-yy who Description of changes, most recent on top
//
// 04-04-13 IJS 5.6-f051 Move common I/O methods to IOUtils.
//						 [3534631] Optimize GetAssociated Resources for RED_Handler
// 02-13-13 ADC 5.5-f034 [3498686, 3497304] XMP GetFileModDate() is extremely slow.
// 02-08-13 HK  5.6-f032 Rework for SonyHDV and XDCAM-EX spanning support in GetAssociatedResources() API.
// 01-29-13 HK  5.6-f027 [3491927] Support spanning for XDCAM-EX GetAssociatedResources() API.
// 01-22-13 HK  5.6-f025 Rework of GetAssociatedResources() API done for XDACM-EX and P2 formats.
// 01-16-13 HK  5.6-f022 Implemented IsMetadataWritable API for XDCAM-EX, P2, CanonXF and AVCHD formats.
// 01-10-13 HK  5.6-f019 Worked on comments in review#1094601:"Added GetAssociatedResources API for XDCAM-EX format."
// 01-02-13 HK  5.6-f013 Create PackageFormat_Support namespace to have common code for folder based handlers.
//
// =================================================================================================
#endif // AdobePrivate

#include "public/include/XMP_Environment.h"	// ! This must be the first include.

#include "source/XMP_LibUtils.hpp"

// =================================================================================================
/// \file PackageFormat_Support.hpp
/// \brief XMPFiles support for folder based formats.
///
// =================================================================================================

namespace PackageFormat_Support 
{

	// Checks if the file at path "file" exists.
	// If it exists then it adds to "resourceList" and returns true.
	bool AddResourceIfExists ( XMP_StringVector * resourceList, const XMP_VarString & file );

	// This function adds all the existing files in the specified folder whose name starts with prefix and ends with postfix.
	bool AddResourceIfExists ( XMP_StringVector * resourceList, const XMP_VarString & folderPath,
		XMP_StringPtr prefix, XMP_StringPtr postfix);


} // namespace PackageFormat_Support

// =================================================================================================

#endif	// __PackageFormat_Support_hpp__
