/*********************************************************************************
	File: 		ASRaiseAware.h
	Created:	June 16, 2003
	Purpose:	This file contains code needed for making a class safe across 
				exceptions.

*
* ___________________
*
*  (c) Copyright 2003-2006 Adobe Systems, Inc.
*  All Rights Reserved.
*
* NOTICE: Adobe permits you to use, modify, and distribute this file
* in accordance with the terms of the Adobe license agreement
* accompanying it. If you have received this file from a source other
* than Adobe, then your use, modification, or distribution of it
* requires the prior written permission of Adobe.
************************************************************************************/
#ifndef _H_ASRAISEAWARE
#define _H_ASRAISEAWARE

/* ============================ C++ compatibility ======================== */

#if defined (__cplusplus)

/* 
   NEEDS_RAISEAWARE can be set to 0 by modules that wrap all outgoing 
   calls at the module boundary with a DURING/HANDLER and convert them
   to C++ exceptions or error return values. This shields the classes 
   within from ASRaise related issues and hence does not require 
   RAISEAWARE on any platform

   The only downside to NEEDS_RAISEAWARE being 1 is a small 
   performance hit. Hence it is safe, not to disable it by setting it 
   to 0 in case there is doubt on whether it is required.
*/

#ifndef NEEDS_RAISEAWARE

#if (defined(WIN_PLATFORM) && defined(_MSC_VER))
/*
   For modules compiled with VC++, make sure that the module is compiled
   using the /EHsc- switch. Not having that set could result in stack 
   objects not getting cleaned up during a Raise. If that setting is not
   set, then force enable NEEDS_RAISEAWARE by setting it to 1 in the 
   project settings.
*/
	#define NEEDS_RAISEAWARE 0

#elif MAC_PLATFORM
/*
   Since we now use real C++ exceptions for DURING/HANDLER/ASRAISE, we no
   long need RAISEAWARE on Mac.
*/
	#define NEEDS_RAISEAWARE 0

#elif UNIX_PLATFORM

/* serre, 10/6/06
   When DURING/HANDLER/ASRAISE are called from C++ code, a C++
   exception is thrown and we don't need RAISEAWARE to destruct objects
   constructed "on the stack".  
   If DURING/HANDLER/ASRAISE are called from C code, then the setjmp/longjmp
   version of exceptions are used, and there may be resource leaks.  
   We would like to have users only call PDF lib functions from C++ code so
   we don't have to use this RAISEAWARE code. If this turns out to not be 
   acceptable, then we will switch back (or do something else). 
 */
	#define NEEDS_RAISEAWARE 0

#else
	#define NEEDS_RAISEAWARE 1
#endif /* (defined(WIN_PLATFORM) && defined(_MSC_VER)) */

#endif /* NEEDS_RAISEAWARE */

#if NEEDS_RAISEAWARE
	#warning "Evaluate need for RaiseAware."
#endif

#define RAISEAWARECLASS(X)

#if defined(AIX_VACPP) || defined(__SUNPRO_CC)

// On AIX, RAISEAWARE(foo); causes complaints about the 
// trailing semi-colon if it is defined to nothing
#define RAISEAWARE(X) private: \
void raiseaware_compile_stub() {}\

#define RAISEAWARE_WITH_COPY_CONSTRUCTOR(X) private: \
void raiseaware_compile_stub() {}\

#define RAISEAWARE_STRUCT(X) private: \
void raiseaware_compile_stub() {}\

#define RAISEAWARE_STRUCT_WITH_COPY_CONSTRUCTOR(X) private: \
void raiseaware_compile_stub() {}\

#else /* AIX_VACPP || __SUNPRO_CC */

#define RAISEAWARE(X)
#define RAISEAWARE_WITH_COPY_CONSTRUCTOR(X)
#define RAISEAWARE_STRUCT(X)
#define RAISEAWARE_STRUCT_WITH_COPY_CONSTRUCTOR(X)

#endif /* AIX_VACPP || __SUNPRO_CC */


#define CTOR
#define DTOR

/****************************************************************************/

#endif	/* defined (__cplusplus) */

/* ======================================================================= */

#endif /* _H_ASRAISEAWARE */
