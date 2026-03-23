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

 CorCalls.h

 - Exception handling methods (ASPushExceptionFrame and
   ASPopExceptionFrame) should typically be accessed through the
   DURING/HANDLER/END_HANDLER macros.

 ******************************************************************************************
                        **** Instructions for Plugin Developers **** 
   (In the instructions below ~ is used to refer to the HFT this file is for.  For
     example, this file would be called "~Calls.h")
  
   To use this file you must declare two global variables: g~HFT and g~Version.  You can
   see them declared as extern about one page down from here.  Your plugin should set a 
   #define of PI_~_VERSION to some non zero value.  Suggested values are given below in
   the "for public use" section.  ~HFT_LATEST_VERSION is not recommended because you will 
   not be able to support backwards compatible versions.  It is recommended that you use the lowest 
   ~HFT_VERSION you require.  Later versions are compatible with earlier versions, so if 
   you require ~HFT_VERSION_4, your plugin will work with ~HFT_VERSION_5, ~HFT_VERSION_6, and so on.
  
   You can support old versions and still use newer versions of this HFT by checking the 
   value of g~Version.  If you use the standard PiMain.c supplied in the SDK, this will be
   set to the actual version of the HFT returned to you (For example, if you require version 4,
   you are returned version 7, which is compatible, and g~Version is set to 7).  You can write
   code that looks something like this:
     if (g~Version >= ~HFT_VERSION_5) 
        CallNewSpeedyCode();
     else {
        assert(g~Version >= ~HFT_VERSION_4);  //PI_~_VERSION was defined as ~HFT_VERSION_4
        CallOldSlowCode();
     }
 ******************************************************************************************
                         **** Instructions for HFT Developer **** 
   (In the instructions below ~ is used to refer to the HFT this file is for.  For
     example, this file would be called "~Calls.h")

   Important: routines that have been released can never be deleted or changed.
   If you want to make a new version, create a new call, add it to the end of this file and
   increment _~_LATEST_VERSION (note the leading underscore).  
   
   If this is the first new routine in a new version, change the _~_IS_BETA flag
   to 1.  Next, create a new ~_VERSION_# for plugins to use and set it to 
   ~HFT_LATEST_VERSION.  For example, suppose the last release of Acrobat was version 20, version 21
   is under development, and you add a new routine for version 21.  Increment _~HFT_LATEST_VERSION 
   to 0x00200001 and set _~_IS_BETA to 1.  Add "#define ~HFT_VERSION_21 ~HFT_LATEST_VERSION".  
   Add your routine and assert that g~Version >= ~HFT_VERSION_21.  Leave 
   _~_LAST_BETA_COMPATIBLE_VERSION unchanged (0x00200000 in this example).

   If the ~_IS_BETA flag is set to 1, you may change or delete the beta routines at will.
   Before checking in the modifications, however, increment the _~HFT_LATEST_VERSION number.
   If the change is not compatible (delete, change, and so on) set _~_LAST_BETA_COMPATIBLE_VERSION equal
   to the new _~HFT_LATEST_VERSION.  If the change is compatible, leave the LAST_BETA version
   as is.

   Plugins that require a BETA HFT will be refused unless they ask for a beta version greater than or equal to
   (>=) LAST_BETA_COMPATIBLE_VERSION and less than or equal to (<=) HFT_LATEST_VERSION.
   By incrementing the version number, you ensure the plugin will refuse to load until it
   has been recompiled with your changes.  You also ensure plugins compiled with your changes
   will not load on older versions of Acrobat.  In other words, it makes sure both sides are in sync.  

   Important: Whenever you make a change to this file, you must increment _~HFT_LATEST_VERSION.
  
   Once the product reaches RC or a similar milestone, change the _~_IS_BETA flag to 0, jump
   the _~HFT_LATEST_VERSION to the final version (0x00210000 in our example), do the same for
   _~_LAST_BETA_COMPATIBLE_VERSION, and set the 
   ~HFT_VERSION_# to the final version number (0x00210000 in this example).  Once the HFT
   has left beta, the routines cannot be changed and a new beta must ensue (beta for version
   22 in this example).

*********************************************************************/

#ifndef _H_CorCalls
#define _H_CorCalls

#include "acroassert.h"

/* This header includes templates and hence would not compile if 
   included inside of a extern "C" block. Hence CorCalls.h must 
   not be included inside of a extern "C" block */
#include "EReturnValidator.h"

	/* for Adobe use only */
#define _CoreHFT_LATEST_VERSION 0x00050000
#define _CoreHFT_LAST_BETA_COMPATIBLE_VERSION 0x00050000
#define _CoreHFT_IS_BETA 0

/* for public use */
#define CoreHFT_LATEST_VERSION (_CoreHFT_IS_BETA ? (kHFT_IN_BETA_FLAG | _CoreHFT_LATEST_VERSION) : _CoreHFT_LATEST_VERSION)

#define CoreHFT_VERSION_2   0x00020000
#define CoreHFT_VERSION_4   0x00040000
#define CoreHFT_VERSION_5   0x00050000  /* note: version 5 is same as 4.  Provided to support old plugins that erroneously required 5 */

#ifndef _CORVERSION_ID_ONLY
#include "PIExcept.h"
#include "CoreExpT.h"
#include "ASExpT.h"

/* The _E_SUPPRESS_NESTED_DURING_HANDLER_WARNINGS is used to suppress 
   the warning C6244: local declaration of <variable> hides previous declaration at <line> of <file> and
   the warning C6246: Local declaration of <variable> hides declaration of same name in outer scope,
   which are generated by Microsoft Visual C++ Code Analysis tool.

   Note that __pragma(warning(suppress:6244 6246)) is equivalent to:

			 __pragma(warning(push))
			 __pragma(warning(disable:6244 6246))
			 <single line of code>
			 __pragma(warning(pop))

   The pragma warning directive with the suppress specifier is supported only for Code Analysis warnings
   and suppresses the warning only for the line of code that immediately follows the directive.
*/
#if WIN_PLATFORM
	#define _E_SUPPRESS_NESTED_DURING_HANDLER_WARNINGS	__pragma(warning(suppress:6244 6246)) 
#else
	#define _E_SUPPRESS_NESTED_DURING_HANDLER_WARNINGS
#endif

#if !defined(USE_CPLUSPLUS_EXCEPTIONS_FOR_ASEXCEPTIONS)
	#if MAC_PLATFORM
		#define USE_CPLUSPLUS_EXCEPTIONS_FOR_ASEXCEPTIONS 1
	#elif UNIX_PLATFORM
		#define USE_CPLUSPLUS_EXCEPTIONS_FOR_ASEXCEPTIONS 1
	#else
		#define USE_CPLUSPLUS_EXCEPTIONS_FOR_ASEXCEPTIONS 1
	#endif
#endif
	
#ifdef __cplusplus

#if USE_CPLUSPLUS_EXCEPTIONS_FOR_ASEXCEPTIONS
	
	/* The C++ version of the exception frame structure contains only the
	information necessary to execute a successful throw. The exception
	error code and message are tracked in a separate class defined
	here. */
	
	struct _miExceptionInfo {
		_miExceptionInfo() {}
		int dummy;
	};
	
	/* This routine knows how to take an exception and 'throw' it to
	a C++ DURING frame.
	*/
	#if 1 //UNIX_PLATFORM
	inline void miThrowExceptionToHandler(void *asEnviron) {throw _miExceptionInfo();}
	#else
	extern void miThrowExceptionToHandler(void *asEnviron);
	#endif
	
#endif /* __cplusplus */
#endif /* USE_CPLUSPLUS_EXCEPTIONS_FOR_ASEXCEPTIONS */

#ifdef __cplusplus
extern "C" {
#endif

#if ACROBAT_LIBRARY
/* Toolkit specific Error-handling function prototypes */

/* RestorePlugInFrame
** This is a function that the plug-in must provide (if you are using the
** standard government-issue PIMain.c, this function is implemented there.)
** It takes a jmp_buf as registered through ASPushExceptionFrame and restores
** state based on that jmp_buf, usually through using longjmp.
*/
extern ACCB1 void ACCB2 RestorePlugInFrame(void *environ);
typedef ACCBPROTO1 void (ACCBPROTO2 *ACRestoreEnvironProc)(void *excEnviron);

extern ACCB1 void ACCB2 ACPushExceptionFrame(void *excEnviron, ACRestoreEnvironProc restoreFunc);
extern ACCB1 void ACCB2 ACPopExceptionFrame(void);
extern ACCB1 ASInt32 ACCB2 ACGetExceptionErrorCode(void);

#if USE_CPLUSPLUS_EXCEPTIONS_FOR_ASEXCEPTIONS
#ifdef __cplusplus

struct _miExceptionPushPop {
	_miExceptionPushPop() { ACPushExceptionFrame(NULL, (ACRestoreEnvironProc)&miThrowExceptionToHandler); }
	~_miExceptionPushPop() { ACPopExceptionFrame(); }
};

/**
	Begins the section of code where Acrobat APIs may throw an exception. After calling an Acrobat API method, execution may jump into the <code>HANDLER</code> clause. 
	This macro is similar to the <code>try</code> clause in the C++ language.

	@see END_HANDLER
	@see E_RETURN
	@see E_RTRN_VOID
	@see HANDLER
*/
#define	DURING {											\
	try {													\
		_E_SUPPRESS_NESTED_DURING_HANDLER_WARNINGS			\
		_miExceptionPushPop __exceptionPushPop;
	
#define _E_RESTORE /*int _E_RESTORE__BogusLocalDoesNothing = 0*/
#define _E_HANDLER } catch (_miExceptionInfo) { 

/**
	Follows a <code>DURING</code> macro. Code inserted between <code>HANDLER</code> and <code>END_HANDLER</code> macros will be executed only if an Acrobat function or other function 
	throws an exception. This macro is similar to the <code>catch</code> clause in the C++ language.

	@see END_HANDLER
	@see DURING
	@see E_RTRN_VOID
	@see E_RETURN
*/
#define	HANDLER	_E_RESTORE; _E_HANDLER _E_RESTORE;
/**
	Ends a <code>DURING/HANDLER/END_HANDLER</code> clause.

	@see DURING
	@see END_HANDLER
	@see E_RETURN
	@see HANDLER
*/
#define	END_HANDLER }}

#else /* __cplusplus */

#define DURING		DURING usage must be compiled with C++
#define HANDLER		HANDLER usage must be compiled with C++
#define END_HANDLER	END_HANDLER usage must be compiled with C++

#endif /* __cplusplus */
#else	/* USE_CPLUSPLUS_EXCEPTIONS_FOR_ASEXCEPTIONS */

#if MAC_PLATFORM
#error Mac Xcode should no longer use this
#endif

#if UNIX_PLATFORM
#error Unix should no longer use this
#endif
		
/**
	Begins the section of code where Acrobat APIs may throw an exception. After calling an Acrobat API method, execution may jump into the <code>HANDLER</code> clause. 
	This macro is similar to the <code>try</code> clause in the C++ language.

	@see END_HANDLER
	@see E_RETURN
	@see E_RTRN_VOID
	@see HANDLER
*/
#define DURING {																		\
	_E_SUPPRESS_NESTED_DURING_HANDLER_WARNINGS											\
	ACROjmp_buf ASException;															\
	ACPushExceptionFrame(&ASException, (ACRestoreEnvironProc)&RestorePlugInFrame);		\
	if (!ACROsetjmp(ASException)) {

#define _E_RESTORE      ACPopExceptionFrame()

/**
	Follows a <code>DURING</code> macro. Code inserted between <code>HANDLER</code> and <code>END_HANDLER</code> macros will be executed only if an Acrobat function or other function 
	throws an exception. This macro is similar to the <code>catch</code> clause in the C++ language.

	@see END_HANDLER
	@see DURING
	@see E_RTRN_VOID
	@see E_RETURN
*/
#define HANDLER _E_RESTORE; } else { _E_RESTORE;
/**
	Ends a <code>DURING/HANDLER/END_HANDLER</code> clause.

	@see DURING
	@see END_HANDLER
	@see E_RETURN
	@see HANDLER
*/
#define END_HANDLER }}

#endif	/* USE_CPLUSPLUS_EXCEPTIONS_FOR_ASEXCEPTIONS */

/**
	A macro defined to call ASGetExceptionErrorCode(). It returns an ASInt32 object containing an exception error code.

	@see DURING
	@see END_HANDLER
	@see E_RTRN_VOID
	@see HANDLER
*/
#define ERRORCODE	(ACGetExceptionErrorCode())

/**
	Returns a value from an enclosing function when nested inside a DURING/HANDLER clause.

	@see DURING
	@see END_HANDLER
	@see E_RTRN_VOID
	@see HANDLER
*/
#define E_RETURN(x) { _E_RESTORE; return(x); }

/**
	Returns from an enclosing function when nested inside a DURING/HANDLER clause, without returning a value.

	@see ErrGetCode
	@see ErrGetSeverity
	@see ErrGetSignedCode
	@see ErrGetSystem
*/
#define E_RTRN_VOID { _E_RESTORE; return; }

#else /* ACROBAT_LIBRARY */

#define ACRestoreEnvironProc restoreEnvironProc

#if !STATIC_HFT && PLUGIN
#define ACPushExceptionFrame ASPushExceptionFrame
#define ACPopExceptionFrame  ASPopExceptionFrame
#define ACGetExceptionErrorCode ASGetExceptionErrorCode
#endif /* STATIC_HFT */

#endif /* ACROBAT_LIBRARY */

#ifdef NPROC /* may be defined in sys/procs.h */
#undef NPROC
#endif /* NPROC */
	
#if !PLUGIN
	/* Static link */
	#define NPROC(returnType, name, params)		\
		ACEX1 returnType ACEX2 name params;
	#define SPROC(returnType, name, params, stubProc) \
  		extern ACEX1 returnType ACEX2 stubProc params;
   #define ANPROC NPROC

   #include "CorProcs.h"

   #undef NPROC
   #undef ANPROC
   #undef SPROC	

   #define ASCallbackCreateProto(proto, proc) (proc)
   #define ASCallbackCreate(proc) (proc)
   #define ASCallbackDestroy(proc) 

	/* These functions have a different name internally */
	#define ASGetExceptionErrorCode ACGetExceptionErrorCode
	#define ASExtensionMgrGetHFT ASGetHFTByNameAndVersion
#endif /* !PLUGIN */

#if PLUGIN
	/* HFT version */
	#include "PIRequir.h"
	
	/* Enumerate the selectors */
	#define NPROC(returnType, name, params)			\
		name##SEL,
	#define SPROC(returnType, name, params, stubProc) \
		name##SEL,
	
   #define ANPROC NPROC	
	enum {
		CoreBAD_SELECTOR,
		#include "CorProcs.h"
		CoreNUMSELECTORSplusOne
	};
	
	#define CoreNUMSELECTORS (CoreNUMSELECTORSplusOne - 1)
	
	/* Create the prototypes */
	#undef NPROC
   #undef ANPROC
   #undef SPROC	
	#define NPROC(returnType, name, params)		\
		typedef ACCBPROTO1 returnType (ACCBPROTO2 *name##SELPROTO)params;
	#define SPROC(returnType, name, params, stubProc) \
		typedef ACCBPROTO1 returnType (ACCBPROTO2 *name##SELPROTO)params;
	
   #define ANPROC NPROC	
		#include "CorProcs.h"
	#undef NPROC
   #undef ANPROC
   #undef SPROC

#if PI_CORE_VERSION != 0

#ifdef THREAD_SAFE_PDFL
	#define gCoreHFT (GetHFTLocations()->coreHFT)
	#define gCoreVersion (GetHFTLocations()->coreVersion)
#include "PDFLInitHFT.h"
#else
	extern HFT gCoreHFT;
	extern ASUns32 gCoreVersion;
#endif

#include "PICommon.h"
#include "PIExcept.h"

#if !STATIC_HFT

/* Exception-handling functions */

/* ASRaise
** Raises an exception.  Subsequent calls to ASGetExceptionErrorCode() will
** return the error value passed in to this function.
** This function is called directly by the macro RERAISE().
*/
#define ASRaiseImpl (ACROASSERT(gCoreVersion >=CoreHFT_VERSION_2), *((ASRaiseSELPROTO)(gCoreHFT[ASRaiseSEL])))

#ifdef __cplusplus
#define ASRaise(err) (ASRaiseImpl(err), throw 0)
#else
#define ASRaise(err) ASRaiseImpl(err)
#endif

/* ASPushExceptionFrame
**
** Push an exception frame buffer and a frame-restoration callback onto the stack.
** The restoreFunc should be a function matching the following prototype:
**
** ACCB1 void ACCB2 RestorePlugInFrame(void *asEnviron);
**
** You will probably never call ASPushExceptionFrame() directly; use the DURING
** macro instead.
*/
#define ASPushExceptionFrame (ACROASSERT(gCoreVersion >=CoreHFT_VERSION_2), *((ASPushExceptionFrameSELPROTO)(gCoreHFT[ASPushExceptionFrameSEL])))

/* ASPopExceptionFrame
** Pop and exception frame off the stack.
** You will probably never call ASPopExceptionFrame() directly; it is called for
** you as appropriate from within the HANDLER, E_RETURN and E_RTRN_VOID macros.
*/
#define ASPopExceptionFrame (ACROASSERT(gCoreVersion >=CoreHFT_VERSION_2), *((ASPopExceptionFrameSELPROTO)(gCoreHFT[ASPopExceptionFrameSEL])))

/* ASGetExceptionErrorCode
** Returns the value of the error parameter as passed in to the most
** recent call to ASRaise().  You might then pass this value to ASGetErrorString()
** in order to report the exception to the user.
*/
#define ASGetExceptionErrorCode (ACROASSERT(gCoreVersion >=CoreHFT_VERSION_2), *((ASGetExceptionErrorCodeSELPROTO)(gCoreHFT[ASGetExceptionErrorCodeSEL])))

/* ASAtom management */

/* ASAtomFromString
** Returns the ASAtom associated with the given string.  You can compare
** ASAtoms directly, rather than passing their associated strings to
** strcmp().
*/
#define ASAtomFromString (ACROASSERT(gCoreVersion >=CoreHFT_VERSION_2), *((ASAtomFromStringSELPROTO)(gCoreHFT[ASAtomFromStringSEL])))

/* ASAtomExistsForString
** Returns true if and only if there already exists an ASAtom for the
** given string.  You might use this to prevent capricious overpopulation
** of the internal ASAtom table (a non-renewable resource!)
*/
#define ASAtomExistsForString (ACROASSERT(gCoreVersion >=CoreHFT_VERSION_2), *((ASAtomExistsForStringSELPROTO)(gCoreHFT[ASAtomExistsForStringSEL])))

/* ASAtomGetString
** Returns the char * associated with the given ASAtom.  This routine
** permanently allocates storage for new strings, so don't call this function
** unnecessarily!
*/
#define ASAtomGetString (ACROASSERT(gCoreVersion >=CoreHFT_VERSION_2), *((ASAtomGetStringSELPROTO)(gCoreHFT[ASAtomGetStringSEL])))

/* ASCallbackCreate
** Takes a pointer to a function and returns an ASCallback--a universally
** entrant function pointer which can be called from any globals context.
*/
/**
	Deprecated in Acrobat 8.0.
*/
#define ASCallbackCreate(x) (ACROASSERT(gCoreVersion >=CoreHFT_VERSION_2), *((ASCallbackCreateSELPROTO)(gCoreHFT[ASCallbackCreateSEL])))(gExtensionID, (void *)(x))

/* ASCallbackCreateProto
** Type-checking callback creation.  Will cause a compiler error if the
** proc is not of the given function-type.  DEBUG must be on for checking to occur.
** Requires four bytes of scratch global space in which to make a bogus
** assignment; this is declared in PIMain.c.
*/
/**
	Deprecated in Acrobat 8.0.
*/
#if DEBUG
#define ASCallbackCreateProto(funcType, proc)				\
	((funcType)ASCallbackCreate((CHECKTYPE(funcType, proc))))
#else
#define ASCallbackCreateProto(funcType, proc)				\
	((funcType)ASCallbackCreate(((void*)proc)))
#endif

/* ASCallbackDestroy
** De-allocates memory associated with an ASCallback.  Use this if you're
** sure you're completely done with an ASCallback.
*/
/**
	Deprecated in Acrobat 8.0.
*/
#define ASCallbackDestroy (ACROASSERT(gCoreVersion >=CoreHFT_VERSION_2), *((ASCallbackDestroySELPROTO)(gCoreHFT[ASCallbackDestroySEL])))

/* ASExtensionMgrGetHFT
** Get an HFT object with the given name and version number.
*/
#define ASExtensionMgrGetHFT (ACROASSERT(gCoreVersion >=CoreHFT_VERSION_2), *((ASExtensionMgrGetHFTSELPROTO)(gCoreHFT[ASExtensionMgrGetHFTSEL])))

/* ASGetConfiguration
** Returns a void * based on the ASAtom passed in.  The void *
** may be cast to a meaningful value based on the input parameter:
**
** key                 result   	                 return type
** Product             Name of product type 	     const char *
**                       (Exchange, Reader, etc.)
** CanEdit             Whether or not editing   	 ASBool
**                       is allowed
** <<anything else>>   UNDEFINED_CONFIGURATION_SELECTOR
*/
#define ASGetConfiguration (ACROASSERT(gCoreVersion >=CoreHFT_VERSION_2), *((ASGetConfigurationSELPROTO)(gCoreHFT[ASGetConfigurationSEL])))


/* begin PI_CORE_VERSION >= 0x00040000 */

/* ASEnumExtensions */
#define ASEnumExtensions (ACROASSERT(gCoreVersion >=CoreHFT_VERSION_4), *((ASEnumExtensionsSELPROTO)(gCoreHFT[ASEnumExtensionsSEL])))

/* ASExtensionGetFileName */
#define ASExtensionGetFileName (ACROASSERT(gCoreVersion >=CoreHFT_VERSION_4), *((ASExtensionGetFileNameSELPROTO) (gCoreHFT[ASExtensionGetFileNameSEL])))

/* ASExtensionGetRegisteredName */
#define ASExtensionGetRegisteredName (ACROASSERT(gCoreVersion >=CoreHFT_VERSION_4), *((ASExtensionGetRegisteredNameSELPROTO) (gCoreHFT[ASExtensionGetRegisteredNameSEL])))

/* end PI_CORE_VERSION >= 0x00040000 */

/* If you add, delete, or modify procs listed in this file please read the instructions at the top about how to properly change the version number */

#endif /* !STATIC_HFT */

/* Exception-handling macros */
/**
	Re-raises the most recently raised exception and passes it to the next exception handler in the stack.

	@see ErrGetCode
	@see ErrGetSeverity
	@see ErrGetSignedCode
	@see ErrGetSystem
	@see ASRaise
*/
#define RERAISE() ASRaise(ERRORCODE)

#define ERRORCODE	(ASGetExceptionErrorCode())

/* RestorePlugInFrame
** This is a function that the plug-in must provide (if you are using the
** standard government-issue PIMain.c, this function is implemented there.)
** It takes a jmp_buf as registered through ASPushExceptionFrame and restores
** state based on that jmp_buf, usually through using longjmp.
*/
extern ACCB1 void ACCB2 RestorePlugInFrame(void *asEnviron);

#if !USE_CPLUSPLUS_EXCEPTIONS_FOR_ASEXCEPTIONS && MAC_PLATFORM
	#error Mac MUST define USE_CPLUSPLUS_EXCEPTIONS_FOR_ASEXCEPTIONS as true
#endif
#if !USE_CPLUSPLUS_EXCEPTIONS_FOR_ASEXCEPTIONS && UNIX_PLATFORM
	#error Unix MUST define USE_CPLUSPLUS_EXCEPTIONS_FOR_ASEXCEPTIONS as true
#endif


#if defined(__cplusplus) && USE_CPLUSPLUS_EXCEPTIONS_FOR_ASEXCEPTIONS
	
	struct _miExceptionPushPop {
		_miExceptionPushPop() { ASPushExceptionFrame(NULL, &miThrowExceptionToHandler); }
		~_miExceptionPushPop() { ASPopExceptionFrame(); }
	};
	
	#define	DURING {									\
		try {											\
			_E_SUPPRESS_NESTED_DURING_HANDLER_WARNINGS	\
			_miExceptionPushPop __exceptionPushPop;
				
	#define _E_RESTORE
	#define _E_HANDLER } catch (_miExceptionInfo) { 
		
	#define	HANDLER	_E_RESTORE; _E_HANDLER _E_RESTORE;
	#define	END_HANDLER }}
	
	#define E_RETURN(x) { return(x); }
	#define E_RTRN_VOID { return; }

#else /* not doing USE_CPLUSPLUS_EXCEPTIONS_FOR_ASEXCEPTIONS */

	#define _E_RESTORE      ASPopExceptionFrame()
	
	#if DEBUG
	
		/* **ER - 11/29/1997
		
		   For debug builds, the macros below enable compile-time checking of common problems
		   with DURING and HANDLER blocks (in order of frequency):
		
				1. "normal" return statements inside DURING blocks
				2. E_RETURN or E_RTRN_VOID statements outside any DURING block
				3. E_RETURN or E_RTRN_VOID statements in HANDLER blocks
				4. HANDLER without matching DURING 
				5. HANDLER without matching END_HANDLER 
		*/
		
		/* Declaration of global integer used below.  This variable needs to be defined
		   somewhere in the plug-in.  If you are using the standard government-issue PIMain.c,
		   it will already be defined there. */
		extern int gBadReturnCatcher;
		
		struct _BadReturnCatcherStruct {
			int E_RETURNOutsideDURINGBlock;		/* This name chosen so it rings a bell if you
												   see it in the compiler output */
		};
		
		/* What's the point of this? Since E_RETURN and E_RTRN_VOID need to actually
		   call return, we need to redefine gBadReturnCatcher as an integer in the
		   scope around the actual call to return so our macro-replaced version of
		   return will work.  Otherwise, when the "return" in the E_RETURN macro 
		   (for example), was replaced by our redefined macro, it'd fail to compile
		   just as if return was placed inside the DURING block */ 
		#define DO_RETURN(x) {										\
						_E_SUPPRESS_NESTED_DURING_HANDLER_WARNINGS	\
						int gBadReturnCatcher;						\
						return(x); }

		#define DO_RETURN_NO_ARGS {									\
						_E_SUPPRESS_NESTED_DURING_HANDLER_WARNINGS	\
						int gBadReturnCatcher;						\
						return; }
		
		/* This definition of DURING includes a struct with the same name as the global 
		   integer above.  The struct must be declared inside the inner scope of the DURING
		   so it is NOT defined in the HANDLER! */
		#define DURING {															\
						_E_SUPPRESS_NESTED_DURING_HANDLER_WARNINGS					\
						ACROjmp_buf ASException;									\
						ASPushExceptionFrame(&ASException, &RestorePlugInFrame);	\
						if (!ACROsetjmp(ASException)) {								\
						_E_SUPPRESS_NESTED_DURING_HANDLER_WARNINGS					\
						struct _BadReturnCatcherStruct gBadReturnCatcher;			   
		
		#define _E_HANDLER } else {
		
		#if DO_ADDITIONAL_RAISEAWARE_CHECKS
		
			/* Make sure that ASRaiseAware classes are not returned as part of the E_RETURN
			   mechanism as it leaves the wrong handler on the top of the exception stack.*/
			#define E_RETURN(x) { IsNotRaiseAwareClassHelperFunction( x ); gBadReturnCatcher.E_RETURNOutsideDURINGBlock = 1; _E_RESTORE; DO_RETURN(x); }
		
		#else
		
			/* Return calls which are legal inside a DURING block attempt
			   to modify the member field of the gBadReturnCatcher struct in the local 
			   scope.  This will cause a compile-time error if there is no such struct 
			   (and thus, the return call is not enclosed by a DURING block) */
			#define E_RETURN(x) { gBadReturnCatcher.E_RETURNOutsideDURINGBlock = 1; _E_RESTORE; DO_RETURN(x); }
		
		#endif
		
		#define E_RTRN_VOID { gBadReturnCatcher.E_RETURNOutsideDURINGBlock = 1; _E_RESTORE; DO_RETURN_NO_ARGS; }
		
		/* returns which are illegal inside a DURING block attempt
		   to modify the global integer gBadReturnCatcher.  This will cause a 
		   compile-time error if the return is inside a DURING block because
		   it will actually be an attempt to modify the local-scope struct of the
		   same name.  The = operator is not defined for assigning constant integers to structs.
		
		   Why are we using a for loop? Here are the considerations involved in choosing
		   this macro:
		
		   1. It must support return statements of the forms "return;", "return <expr>;", and
			  "return( <expr> );".  This is why "return" is present at the end of the macro
		   2. We must use only 1 statement in replacing return.  Why? Consider this code:
		
			  if ( <expr> ) 
				  return; 
			  else {
				  ...
			  }
		
			  In order to support this case, we must not replace the "return" statement with
			  more than one statement, because this will cause the "else" statement to become
			  untied from the "if" statement.  The following code snippet is an example of
			  the above code snippet after a macro expansion where return is defined as multiple
			  statements.  
		
			  if ( <expr> )
				   gBadReturnCatcher = 1;
				   return;
			  else {
				...
			  }
			3. If we are constrained to replace the "return" statement with only one statement,
			   there may only be one path of control, and it must reach the return statement
			   at the end of the macro (so that it can pick up its arguments, if there are any)
		
			The do-nothing for loop below meets these criteria.   
		*/
		#define return for( gBadReturnCatcher = 1;; ) return
		
		/* Why is HANDLER redefined? 
		   1. So you can't put a HANDLER in without a matching DURING.  This would likely be caught
		   by the compiler anyhow, since there'd likely be an "else" without a matching "if"
		   2. To facilitate catching END_HANDLERs without matching HANDLERs
		   3) To get rid of pesky "unreferenced local variable" warnings stemming from 
		   DURING blocks which don't contain E_RETURN or E_RTRN_VOID and thus don't 
		   reference the local gBadReturnCatcher structure. */
		#define HANDLER _E_RESTORE;	gBadReturnCatcher.E_RETURNOutsideDURINGBlock = 1; _E_HANDLER	\
						_E_SUPPRESS_NESTED_DURING_HANDLER_WARNINGS									\
						int ENDHANDLEROutsideHANDLER;												\
						_E_RESTORE;
		
		/* END_HANDLER is redefined to modify the BadEndHandlerCatcher variable defined by the HANDLER
		   macro.  This will prevent END_HANDLERs without matching HANDLERs (unless, of course, 
		   multiple END_HANDLER statements are found inside the same HANDLER, END_HANDLER block */
		#define END_HANDLER ENDHANDLEROutsideHANDLER = 1; }}
	
	#else	/* !DEBUG */
	
		#define DURING {												\
			_E_SUPPRESS_NESTED_DURING_HANDLER_WARNINGS					\
			ACROjmp_buf ASException;									\
			ASPushExceptionFrame(&ASException, &RestorePlugInFrame);	\
			if (!ACROsetjmp(ASException)) {
		
		#define HANDLER _E_RESTORE; } else { _E_RESTORE;
		#define END_HANDLER }}
		
		#define E_RETURN(x) { _E_RESTORE; return(x); }
		#define E_RTRN_VOID { _E_RESTORE; return; }
	
	#endif /* DEBUG */

#endif /* USE_CPLUSPLUS_EXCEPTIONS_FOR_ASEXCEPTIONS vs !USE_CPLUSPLUS_EXCEPTIONS_FOR_ASEXCEPTIONS */

#endif /* PI_CORE_VERSION != 0 */

#endif /* PLUGIN */

#ifdef __cplusplus
}
#endif

#endif /* _CORVERSION_ID_ONLY */
#endif /* !defined(_H_CorCalls) */

