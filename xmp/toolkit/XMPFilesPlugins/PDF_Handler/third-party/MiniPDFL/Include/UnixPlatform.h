/*********************************************************************

 ADOBE SYSTEMS INCORPORATED
 Copyright (C) 1996-2006 Adobe Systems Incorporated
 All rights reserved.

 NOTICE: Adobe permits you to use, modify, and distribute this file
 in accordance with the terms of the Adobe license agreement
 accompanying it. If you have received this file from a source other
 than Adobe, then your use, modification, or distribution of it
 requires the prior written permission of Adobe.

 ---------------------------------------------------------------------

 UnixPlatform.h

 - PLATFORM file for Unix development.

*********************************************************************/

#ifndef _H_UnixPlatform
#define _H_UnixPlatform
/*
** Platform defines
**
** Predefined symbols can be split into three general categories,
** predefined (compiler) hardware specific, Acrobat defined hardware 
** specific, and operating system interface specific (acrobat defined).
** 
**
** Hardware specific defines are predefined by the compiler and 
** include:
**		__i386		Intel 80x86 processors
**		__alpha		Digital 21x64 processors (alpha)
**		__sparc		Sun Sparc compatible processors
**		__mips		Mips (a tiny division of SGI) processors
**
** Acrobat hardware specific defines are defined in this module 
** and use only the predefined hardware defines or each other to
** decide their value.  They should not use the operating system
** symbols.  They include:
**		IEEEFLOAT	Should be true for all high-level processors
**		AS_LITTLEENDIAN	True if most significant byte is in low byte
**		POINTER_64_BITS True if a pointer is represented by 64 bits
**
** Operating system interface specific symbols are defined in the
** system specific makefiles for each platform and include:
**		SOLARIS	
**		LINUX
**		HPUX
**		SGI
**		AIX
**		OSF
**		SUN		(For SunOS not Solaris)
**
** The exception (isn't there always one) is SWAPBITS.  SWAPBITS is an AGM
** thing for the most part and is defined in IntCnfig.h in the AGM world but
** AcroCore filters which came from the PS world also use it.  For simplicities
** sake, SWAPBITS is defined here so that it doesn't have to be put on the
** compiler line with a -D.
**
** Note: SOLARIS is an operating system interface and has nothing necessarily
**       to due hardware since Solaris runs on Sparc and Intel.  Conversely,
**       __alpha does not imply OSF as the operating system since an Alpha
**       can run OSF (Digital Unix), Linux, or NT.
*/ 

#define IEEEFLOAT	1

#if i386 || defined(__i386) || defined(__alpha) || defined(ANDROID)
#ifndef AS_LITTLEENDIAN
#define AS_LITTLEENDIAN	1
#endif
#ifndef SWAPBITS
#define SWAPBITS 1
#endif

#else
#define AS_LITTLEENDIAN	0
#endif /* end if intel or alpha */

#if __alpha
#define ASIntPtrSize long int
#define POINTER_64_BITS 1
#endif

#if (defined(__arch64__) || defined(_LP64) || defined(__x86_64__)) && !defined(AS_ARCH_64BIT)
#define AS_ARCH_64BIT 1
#endif

#define IEEESOFT	0
#define UNSIGNEDCHARS	0
#define FIXED_DEFINED	0
#define FRACT_DEFINED	0

#define WORD_ALIGN (sizeof(void *))

#define HUGEPTRTYPE
#define nENDLINE_CHARS	1
#define sENDLINE	"\n"
#define nENDLINE_CHARS_UCS 2
#define sENDLINE_UCS "\0\n"
#define REPLACEABLE
#define ACROEXPORT
#define ACROEXPORTCDECL
#define ACROEXPORTDATA
#define ACROEXPORTPRIV
#define ACROCALLBACK
#define AVEXPORT
#define AVEXPORTCDECL
#define AVEXPORTDATA
#define AVEXPORTPRIV
#define AVEX1
#define AVEX2
#define AVCB1
#define AVCB2
#define ASKEY "ASKeyUnix.h"
#define ACEX1
#define ACEX2
#define ACCB1
#define ACCB2
#define ACCBPROTO1
#define ACCBPROTO2

#define ACROEXPORTDATA1
#define ACROEXPORTDATA2
#define ACROIMPORTDATA1
#define ACROIMPORTDATA2
#define AVEXPORTDATA1
#define AVEXPORTDATA2
#define AVIMPORTDATA1
#define AVIMPORTDATA2

/**
	(UNIX only) Defined if the client is being compiled for a UNIX platform, undefined otherwise. MAC_PLATFORM, WIN_PLATFORM, and UNIX_PLATFORM 
	should be used by client developers to conditionally compile platform-dependent code.

	<p>UNIX_PLATFORM must be defined in the arguments to the C compiler. The make files for the sample clients in the Acrobat SDK do this automatically.</p>

	@see MAC_PLATFORM
	@see WIN_PLATFORM
*/
#define UNIX_PLATFORM 1

#define MULTI_TASKS	1

#define HAS_LEAVE_NOTIFY 1
#define USE_NULLDOC 0

/* The following are required for Filter */
#ifndef TOOLKIT
//#define ENVIRONMENT "PS2Prefix.h" //removing this, as PS2Prefix.h does not exist in local codebase
#define ACROBAT 1
#endif

/* The following are used (but not required) by Error.h */
#define MDSYSERR "UnixSysE.h"
#define DefineMDSysErr(unixErrNum, mdErrName, msg) mdErrName = unixErrNum,

/* The following is used (but not required) by Error.h */
#define MDAPPERR "UnixAppE.h"

/* The following is used (but not required by Error.h */
#define MDERRINC <errno.h>

/* Define the maximum number of characters allowed in a directory 
** path specification. POSIX standard is at least 255 as defined in limits.h.
*/
#include <limits.h>
#ifndef _MAX_PATH
#ifdef IA64HPUX
#define _MAX_PATH _POSIX_PATH_MAX
#else
#define _MAX_PATH PATH_MAX
#endif
#endif

#endif /* _H_UnixPlatform */

