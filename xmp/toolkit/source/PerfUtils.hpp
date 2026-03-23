#ifndef __PerfUtils_hpp__
#define __PerfUtils_hpp__ 1

// =================================================================================================
// Copyright 2006 Adobe
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
//	AWL Alan Lillich
//	ADC Amandeep Chawla
//
// mm-dd-yy who Description of changes, most recent on top.
//
// 03-21-13 ADC 5.6-f047 [3526905] In MAC and Linux, progress notifications are not returned to the registered callbacks as per the set time-interval.
// 03-13-13 ADC 5.6-f046 Remove deprecated OSX APIs from XMP toolkit code.
//
// 09-21-06 AWL Initial creation.
//
// =================================================================================================
#endif

#include "public/include/XMP_Environment.h"

#if XMP_MacBuild
	#include <CoreServices/CoreServices.h>
#elif XMP_WinBuild
	#include <Windows.h>
#elif XMP_UNIXBuild | XMP_iOSBuild | XMP_AndroidBuild
	#include <time.h>
#endif

namespace PerfUtils {

	#if XMP_WinBuild
//		typedef LARGE_INTEGER MomentValue;
		typedef LONGLONG MomentValue;
		static const MomentValue kZeroMoment = 0;
	#elif XMP_UNIXBuild | XMP_AndroidBuild
		typedef struct timespec MomentValue;
		static const MomentValue kZeroMoment = {0, 0};
	#elif XMP_iOSBuild | XMP_MacBuild
		typedef uint64_t MomentValue;
		static const MomentValue kZeroMoment = 0;
	#endif

	const char * GetTimerInfo();

	MomentValue NoteThisMoment();
	
	double GetElapsedSeconds ( MomentValue start, MomentValue finish );

};	// PerfUtils

#endif	// __PerfUtils_hpp__
