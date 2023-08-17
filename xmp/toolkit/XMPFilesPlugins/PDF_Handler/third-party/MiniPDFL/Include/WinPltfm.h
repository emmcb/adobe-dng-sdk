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

 WinPltfm.h

 - PLATFORM file for Windows development.

*********************************************************************/

#define MDSYSERR "wfileerr.h"
#define DefineMDSysErr(mdNum, mdName, msg) mdName = mdNum,

#define ASKEY "askeywin.h"

#define AS_LITTLEENDIAN 1
#define IEEEFLOAT 0
#define IEEESOFT 0
#define UNSIGNEDCHARS 0
#define FRACT_DEFINED 0  

#ifdef _WIN32
#define MEMSET_IMPLEMENTED      0
#define MEMCPY_IMPLEMENTED      0
#else
#define MEMSET_IMPLEMENTED      1
#define MEMCPY_IMPLEMENTED      1
#endif  // _WIN32

/*  32 bit mode changes */
#ifdef _WIN32
#define WORD_ALIGN 4
#define os_size_t_IS_Uns32      1
#define HUGEPTRTYPE 
#else
#define WORD_ALIGN 2
#define os_size_t_IS_Uns32      0
#define HUGEPTRTYPE __huge
#endif  /* _WIN32 */

/* Define the linefeed termination for this system */
#define nENDLINE_CHARS 2
#define sENDLINE "\015\012"

#define REPLACEABLE

#ifdef _WIN32  
#define far
#define _huge 
#define __huge
#define AVEX1 __declspec(dllexport)
#define AVEX2 
#else
#define AVEX1 
#define AVEX2 __export
#endif

#define ACROCALLBACKPROTO
/**
	A macro used when declaring function prototypes. Its definition is platform-dependent. Use this macro in every function prototype you declare.
	Use <code>ACCBPROTO1</code> before the return value in a function prototype.

	@example	<code>static ACCBPROTO1 void (ACCBPROTO2 *DrawImageSelectionCallback)(AVPageView pageView, AVRect* updateRect, void *data);</code>

	@see ACCB1
	@see ACCB2
	@see ACCBPROTO2
*/
#define ACCBPROTO1
/**
	A macro used when declaring function prototypes. Its definition is platform-dependent. Use this macro in every function prototype you declare.
	Use <code>ACCBPROTO2</code> after the return value in a function prototype.

	@example	<code>static ACCBPROTO1 void (ACCBPROTO2 *DrawImageSelectionCallback)(AVPageView pageView, AVRect* updateRect, void *data);</code>

	@see ACCB1
	@see ACCB2
	@see ACCBPROTO1
*/
#define ACCBPROTO2

#ifdef _WIN32
#define ACROCALLBACK  
/**
	A macro used when declaring callback functions. Its definition is platform-dependent. Use this macro in every callback function you declare.
	Use <code>ACCB2</code> after the return value in a function declaration.

	@example	<code>static ACCB1 ASAtom ACCB2 SnapZoomToolGetType(AVTool tool){...}</code>

	@see ACCB1
	@see ACCBPROTO1
	@see ACCBPROTO2
*/
#define ACCB2                  
#else
#define ACROCALLBACK    __loadds
#define ACCB2           __loadds
#endif
/**
	A macro used when declaring callback functions. Its definition is platform-dependent. Use this macro in every callback function you declare.
	Use <code>ACCB1</code> before the return value in a function declaration.

	@example	<code>static ACCB1 ASAtom ACCB2 SnapZoomToolGetType(AVTool tool){...}</code>

	@see ACCB2
	@see ACCBPROTO1
	@see ACCBPROTO2
*/
#define ACCB1

#ifdef _WIN32
#define ACROEXPORT AVEX1   
#define ACEX1      AVEX1
#define ACEX2      
#else
#define ACROEXPORT __loadds __export
#define ACEX1     
#define ACEX2      __loadds __export
#endif

#ifdef _WIN32
#define ACROEXPORTPRIV  AVEX1        
#define ACROEXPORTCDECL AVEX1
#define ACROEXPORTDATA  
#define ACROIMPORTDATA  extern __declspec(dllimport)
#define ACROIMPORTDATA1 __declspec(dllimport)
#define ACROIMPORTDATA2 
#else
#define ACROEXPORTPRIV  __loadds  __export
#define ACROEXPORTCDECL __cdecl   __loadds  __export
#define ACROEXPORTDATA  __export
#define ACROIMPORTDATA  extern
#define ACROIMPORTDATA1 
#define ACROIMPORTDATA2 
#endif

/**
	(Windows only, previously known as <code>WIN_ENV</code>) Defined if the client is being compiled for a Windows machine, undefined otherwise. 
	MAC_PLATFORM, WIN_PLATFORM, and UNIX_PLATFORM should be used by client developers to conditionally compile platform-dependent code.

	<p>WIN_PLATFORM must be defined in the arguments to the C compiler. The make files for the sample clients in the Acrobat SDK do this automatically.</p>

	@see MAC_PLATFORM
	@see UNIX_PLATFORM
*/
#define WIN_PLATFORM 1

/* XXX we should only have DEBUG in ACROCORE */
#if _DEBUG
	/**
		Enables and disables compile-time type-checking in various declarations.
		<p>Define DEBUG as <code>1</code> to enable type-checking (when developing and testing clients), and as <code>0</code> to disable 
		type-checking (before shipping your client).</p>

		@example	<code>#define DEBUG 1</code>

		@see ASCallbackCreateNotification
		@see ASCallbackCreateProto
		@see ASCallbackCreateReplacement
	*/
	#ifdef DEBUG
	#undef DEBUG
	#endif
	#define DEBUG 1
	#define DODEBUG 1
#endif

/* The following defines are required to compile the Filter sources */
#define ENVIRONMENT "PS2Prefi.h"
#define CAROUSEL 1
#define ANSI_C 1
#define WIN 1
