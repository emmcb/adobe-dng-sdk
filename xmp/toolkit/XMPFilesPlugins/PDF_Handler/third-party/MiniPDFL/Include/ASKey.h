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

 ASKey.h

 - Definition of standard key codes for Mac and Windows.

*********************************************************************/

#ifndef _H_ASKey
#define _H_ASKey

#include "Environ.h"
#if WIN_PLATFORM
#include <windows.h>
#endif

/** @ingroup KeyCodeAllPlatforms */
#define ASKEY_ESCAPE	27
/** @ingroup KeyCodeAllPlatforms */
#define ASKEY_SPACE		32
/** @ingroup KeyCodeAllPlatforms */
#define ASKEY_TAB		9
/** @ingroup KeyCodeAllPlatforms */
#define ASKEY_ARROW_R	29
/** @ingroup KeyCodeAllPlatforms */
#define ASKEY_ARROW_D	31
/** @ingroup KeyCodeAllPlatforms */
#define ASKEY_ARROW_L	28
/** @ingroup KeyCodeAllPlatforms */
#define ASKEY_ARROW_U	30
/** @ingroup KeyCodeAllPlatforms */
#define ASKEY_PAGE_U	11
/** @ingroup KeyCodeAllPlatforms */
#define ASKEY_PAGE_D	12
/** @ingroup KeyCodeAllPlatforms */
#define ASKEY_HELP		5

#if WIN_PLATFORM
/** @ingroup KeyCodeWindows */
#define ASKEY_ENTER		13
/** 
	Deprecated. Use ASKEY_RIGHT_DELETE or ASKEY_BACKSPACE.
	@ingroup KeyCodeWindows 
*/
#define ASKEY_DEL		127
/** @ingroup KeyCodeWindows */
#define ASKEY_RIGHT_DELETE 127
/** @ingroup KeyCodeWindows */
#define ASKEY_INSERT 126
/** @ingroup KeyCodeWindows */
#define ASKEY_BACKSPACE	8
/** @ingroup KeyCodeWindows */
#define ASKEY_HOME		4
/** @ingroup KeyCodeWindows */
#define ASKEY_END		1
/** @ingroup KeyCodeWindows */
#define ASKEY_MENU		2
#endif

#if MAC_PLATFORM
/** @ingroup KeyCodeMacintosh */
#define ASKEY_ENTER		3
/** 
	Deprecated. Please use ASKEY_RIGHT_DELETE or ASKEY_BACKSPACE.
	@ingroup KeyCodeMacintosh
*/
#define ASKEY_DEL		8	
/** @ingroup KeyCodeMacintosh */
#define ASKEY_RIGHT_DELETE 127
/** @ingroup KeyCodeMacintosh */
#define ASKEY_INSERT 126
/** @ingroup KeyCodeMacintosh */
#define ASKEY_BACKSPACE	8
/** @ingroup KeyCodeMacintosh */
#define ASKEY_HOME		1
/** @ingroup KeyCodeMacintosh */
#define ASKEY_END		4
/** @ingroup KeyCodeMacintosh */
#define ASKEY_CR		13
/** @ingroup KeyCodeMacintosh */
#define ASKEY_CLEAR		27
#endif

#if UNIX_PLATFORM
/** @ingroup KeyCodeUNIX */
#define ASKEY_ENTER             10
/** @ingroup KeyCodeUNIX */
#define ASKEY_CR                13
/** @ingroup KeyCodeUNIX */
#define ASKEY_CLEAR             27
/** 
	Deprecated. Please use ASKEY_RIGHT_DELETE or ASKEY_BACKSPACE.
	@ingroup KeyCodeUNIX 
*/
#define ASKEY_DEL				8	
/** @ingroup KeyCodeUNIX */
#define ASKEY_RIGHT_DELETE 		127
/** @ingroup KeyCodeUNIX */
#define ASKEY_BACKSPACE			8
/** @ingroup KeyCodeUNIX */
#define ASKEY_HOME              1
/** @ingroup KeyCodeUNIX */
#define ASKEY_END               4

#define ASKEY_MENU              2
#define ASKEY_INSERT            17

#endif

#ifdef ASKEY_CR
#define ASKEYIsReturnOrEnter(key)	((key == ASKEY_CR) || (key == ASKEY_ENTER))
#else
#define ASKEYIsReturnOrEnter(key)	(key == ASKEY_ENTER)
#endif /* ASKEY_CR */

#endif /* _H_ASKey */
