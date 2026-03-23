#ifndef __XMPCompareAndMerge_ChangeLog_hpp__
#define __XMPCompareAndMerge_ChangeLog_hpp__ 1

// =================================================================================================
// Copyright 2014 Adobe
// All Rights Reserved.
//
// NOTICE:  Adobe permits you to use, modify, and distribute this file in accordance with the terms
// of the Adobe license agreement accompanying it. If you have received this file from a source other 
// than Adobe, then your use, modification, or distribution of it requires the prior written permission
// of Adobe.
// =================================================================================================

// =================================================================================================
// This file has two purposes, to provide the current engineering build number and to maintain a log
// of source changes going into each build. All but the most trivial changes should be noted here.
// This is the primary source for release notes. It is not necessary to change the build number for
// every change, but it should be updated prior to weekly builds if there are changes. The build
// number is maintained by hand. It reflects the state of the source. Official builds also include
// a Perforce changelist number and timestamp, so that separate builds from identical source can be
// differentiated.
// =================================================================================================

#if 0
	// ! Can't use this because Xcode does not do includes for plist processing. 
	#include "public/include/XMP_Environment.h"	// ! Must be the first include.
#else
	#ifndef XMP_DebugBuild
		#if NDEBUG
			#define XMP_DebugBuild 0
		#else
			#define XMP_DebugBuild 1
		#endif
	#endif
#endif

#define kXMPCompareAndMerge_EngBuild     0
#define kXMPCompareAndMerge_EngString "000"

#if ! XMP_DebugBuild
	#define kXMPCompareAndMerge_DebugSuffix
	#define kXMPCompareAndMerge_DebugString "        "
#else
	#define kXMPCompareAndMerge_DebugSuffix (debug)
	#define kXMPCompareAndMerge_DebugString " (debug)"
#endif

#endif  // __XMPCompareAndMerge_ChangeLog_hpp__


#if AdobePrivate
// =================================================================================================
// Change history
// ==============
//
// Writers:
//  ADC	Amandeep Chawla
//  HK	Honey Kansal
//  ANS	Ankita Sharma
//  DKG Deepak Kumar Garg
//
// mm-dd-yy who M.m-bbb Description of changes, most recent on top.
//
// 08-07-14 ANS 1.0-m030 Fixing compile time issues in headers on Mac 32 bit
// 08-03-14 ADC 1.0-m029 Fixing crashers on Macintosh.
// 07-15-14 ADC 1.0-m028 Fixing problems with GetMergedMetadata functionality in case of no or pending conflicts.
// 07-14-14 ANS 1.0-m027 Marking functions inside XMPCompareAndMerge.h as inline, fixing header issues and
//						 removing unnecessary code from RDF Parser
// 07-14-14 DKG 1.0-m026 Fixing build issues related to missing definations of class in public headers.
// 07-11-14 ADC 1.0-m025 Fixing issues related to release and 64 bit build on Mac.
// 07-10-14 ADC 1.0-m024 Fixing issues related to Mac build.
// 07-10-14 ADC 1.0-m023 Refactoring of XMPCompareAndMerge C++ APIs.
// 05-20-14 ADC 1.0-m022 Fixing compile time issues with TestXMPCompareAndMergeAPI.
// 05-20-14 ADC 1.0-m021 Fixing compile time issues with XMPCompareAndMerge.
// 05-19-14 ADC 1.0-m020 Refactoring XMPCommon code and appropriate changes in XMPCore and XMPCompareAndMerge.
// 04-11-14 ADC 1.0-m019 [3739647] Find a Strategy to resolve the issues of "multiple definitions" faced
//						 by clients while integrating the new builds containing Compare and Merge.
// 03-11-14 ADC 1.0-m018 Fixing some warnings reported on Windows while compiling with Warning Level 4.
// 03-10-14 ADC 1.0-m017 [3719347] The Merge results appear different for debug and release configurations
//						 in case of array of struct.
// 03-06-14 ADC 1.0-m016 Fixed some issues found while implementing Anywhere History Strategy.
// 03-06-14 ADC 1.0-m015 [3718094] In some cases the correct resolution is not happening for array nodes (bag type)
// 03-05-14 ADC 1.0-m014 Implemented a small change in Resolution strategy.
// 03-03-14 ADC 1.0-m013 Implemented GetMergedMetadata function of ThreeWayMerge.
// 02-27-14 ADC 1.0-m012 [3714523] For structure properties, when the entire struct node is deleted from
//						 either of user or latest version, incorrect results are returned.
// 02-25-14 ADC 1.0-m011 Changes required for compiling on Visual Studio 2010.
// 02-25-14 ADC 1.0-m010 Porting XMPCompareAndMerge to Mac Environment.
// 02-24-14 HK  1.0-m009 Added new internal XMP DOM interfaces + fixed memory leaks + implemented qualifiers for new XMP DOM.
// 02-24-14 ANS 1.0-m008 Fixing the issue where the shared pointers were getting compared instead of the sizes
// 02-19-14 ADC 1.0-m007 Public Header files were referring to couple of private header files.
// 02-18-14 HK  1.0-m006 Implementing Serialize(), Clone() and all setter/modifier methods of XMP DOM.
// 02-18-14 ADC 1.0-m005 Fixes for problems reported by QE during the basic sanity tests
// 02-18-14 ADC 1.0-m004 Three Way Conflict Identification Feature.
// 02-09-14 ADC 1.0-m003 Re-factoring of XMPCommon Framework
// 02-05-14 ADC 1.0-m002 Some changes in the common framework
// 01-30-14 ADC 1.0-m001 First Version of the file
//
// =================================================================================================
#endif
