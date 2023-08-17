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

 Except.h

 - #define's of the setjmp, longjmp and jmp_buf equivalents for Acrobat
   plug-ins. Plug-in writers will probably not directly #include this
   file; most likely they will #include "CorCalls.h" and use the
   DURING/HANDLER/END_HANDLER macros.

*********************************************************************/

#ifndef _H_Except
#define _H_Except

#if WIN_PLATFORM

	/* If Windows.h has already been included, these things have been defined.
	 * Otherwise, just define what we need.
	 */
	#ifndef _WINDOWS_       /* NT  define for windows.h */
	#ifndef _INC_WINDOWS    /* 3.1 define for windows.h */

		#ifdef _WIN32

			#define _far 
			#define WINAPI   __stdcall

		#else 

			/* For 32-bit Watcom, identified by the __386__ conditional, we must use
			   setjmp/longjmp, and cannot use Catch/Throw. */
			#ifndef __386__
				#define WINAPI              _far _pascal
				#define FAR                 _far
			#endif

			typedef int CATCHBUF[9];
			typedef int FAR* LPCATCHBUF;

			int     WINAPI Catch(int FAR*);
			void    WINAPI Throw(const int FAR*, int);

			#undef WINAPI
			#undef FAR

		#endif

	#endif /* _INC_WINDOWS */
	#endif /* _WINDOWS_    */

	#ifdef __386__

		/* 32-bit WatCom doesn't implement Catch/Throw correctly, so setjmp/longjmp
		   must be used. */
		#include <setjmp.h>

	#elif _WIN32

		/* Windows NT doesn't implement Catch/Throw so use setjmp/longjmp */
		#include <setjmp.h>

	#else   /* Win16 */

		/* define setjmp.h names to be Windows equivalents */
		#define jmp_buf CATCHBUF
		#define setjmp Catch
		#define longjmp Throw

	#endif

	#define ACROsetjmp setjmp
	#define ACROlongjmp longjmp
	#define ACROjmp_buf jmp_buf

#elif MAC_PLATFORM

	#include <setjmp.h>
	
	#define ACROsetjmp setjmp
	#define ACROlongjmp longjmp
	#define ACROjmp_buf jmp_buf
	
#else	/* !WIN_PLATFORM && !MAC_PLATFORM */

	#include <setjmp.h>
		
	#define ACROsetjmp setjmp
	#define ACROlongjmp longjmp
	#define ACROjmp_buf jmp_buf

#endif

#endif /* defined(_H_Except) */
