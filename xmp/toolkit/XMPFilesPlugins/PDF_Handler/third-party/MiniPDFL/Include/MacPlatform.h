/*********************************************************************

 ADOBE SYSTEMS INCORPORATED
 Copyright (C) 1994-2006 Adobe Systems Incorporated
 All rights reserved.

 NOTICE: Adobe permits you to use, modify, and distribute this file
 in accordance with the terms of the Adobe license agreement
 accompanying it. If you have received this file from a source other
 than Adobe, then your use, modification, or distribution of it
 requires the prior written permission of Adobe.

 ---------------------------------------------------------------------

 MacPlatform.h

 - PLATFORM file for Macintosh development.

*********************************************************************/

#ifndef _H_MacPlatform
#define _H_MacPlatform

#ifndef AS_LITTLEENDIAN
#define AS_LITTLEENDIAN TARGET_RT_LITTLE_ENDIAN
#endif

#define IEEEFLOAT 0
#define IEEESOFT 0
#define UNSIGNEDCHARS 0

#if __LP64__
#define AS_ARCH_64BIT	1
#define ASIntPtrSize	long
#endif

#define ACCB1
#define ACCB2

#define ACEX1
#define ACEX2

#define ACCBPROTO1
#define ACCBPROTO2

/**
	(Mac OS only, previously known as <code>MAC_ENV</code>) Defined if the client is being compiled for Mac OS, undefined otherwise. 
	MAC_PLATFORM, WIN_PLATFORM, and UNIX_PLATFORM should be used by client developers to conditionally compile platform-dependent code.
	MAC_PLATFORM is automatically set by the header files.

	@see WIN_PLATFORM
	@see UNIX_PLATFORM
*/
#define MAC_PLATFORM 1

#endif /* defined(_H_MacPlatform) */

