#ifndef __XMPCore_ChangeLog_hpp__
#define __XMPCore_ChangeLog_hpp__ 1

// =================================================================================================
// Copyright 2003 Adobe
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

#define kXMPCore_EngBuild      0
#define kXMPCore_EngString  "000"

#if ! XMP_DebugBuild
	#define kXMPCore_DebugSuffix
	#define kXMPCore_DebugString "        "
#else
	#define kXMPCore_DebugSuffix (debug)
	#define kXMPCore_DebugString " (debug)"
#endif

#endif	/* __XMPCore_ChangeLog_hpp__ */
