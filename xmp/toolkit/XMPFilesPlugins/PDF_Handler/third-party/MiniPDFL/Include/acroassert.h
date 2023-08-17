/*********************************************************************

 ADOBE SYSTEMS INCORPORATED
 Copyright (C) 2003-2006 Adobe Systems Incorporated
 All rights reserved.

 NOTICE: Adobe permits you to use, modify, and distribute this file
 in accordance with the terms of the Adobe license agreement
 accompanying it. If you have received this file from a source other
 than Adobe, then your use, modification, or distribution of it
 requires the prior written permission of Adobe.

*********************************************************************/

#ifndef _H_CASSERT_ADOBE_API
#define _H_CASSERT_ADOBE_API

#if !DEBUG
/* ANSI assert.h requires preprocessor definition of NDEBUG in order to disable.
   We can use the DEBUG setting */
#if defined (__cplusplus)
inline void AcroAssertDoNothing() {}
#define ACROASSERT(ignore) (AcroAssertDoNothing())
#else
#define ACROASSERT(ignore) ((void*)0)
#endif

#else

#include "Environ.h"

#if defined(WIN_PLATFORM) || defined(WIN_ENV)
/* Win version of assert brings up dialog with file/line and allows user to go to debugger.  We'll use it */
#include <assert.h>
/**
	A platform-independent version of the ANSI <code>assert</code> function.
*/
#define ACROASSERT assert

#elif defined(MAC_PLATFORM) || defined(MAC_ENV)
/* Win version of assert brings up dialog with file/line and allows user to go to debugger.  We'll use it */
#include <assert.h>
/**
	A platform-independent version of the ANSI <code>assert</code> function.
*/
#define ACROASSERT assert

#elif defined(UNIX_PLATFORM) || defined(UNIX_ENV)
	/* Unix version of assert.  Use default implementation */

	/* There is a compiler issue we've seen on x86 Linux gcc 3.2 when using the native assert */
	#if ((__GNUC__ == 3) && (__GNUC_MINOR__ == 2) && (__i386__) && (defined(PLUGIN)))
		#include <stdio.h>
		#define ACROASSERT(expr) ((void) ((expr) ? 0 : (fprintf (stderr, "%s failed at %s:%d\n", #expr, __FILE__, __LINE__), (*(int *)0 = 1) )))

	#else /* not Linux/gcc 3.2/Plugins */

		/* normal Unix version -- just use assert() */
		#include <assert.h>
		#define ACROASSERT assert
	#endif /* not Linux/gcc 3.2/Plugins */
#else
/* should define one of the platforms above.  setting ACROASSERT to ANSI assert */
#pragma message("PLATFORM not defined")
#include <assert.h>
#define ACROASSERT assert
#endif /* PLATFORM */
#endif  /*!DEBUG */
   

#endif /*_H_CASSERT_ADOBE_API*/

