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

 PagePDEContentCalls.h

 - HFT and prototypes for Page PDEContent methods.

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

#ifndef _H_PAGEPDECONTENTCALLS
#define _H_PAGEPDECONTENTCALLS

#include "Environ.h"

#if PLUGIN
#include "acroassert.h"
#endif
#ifdef THREAD_SAFE_PDFL
#include "PDFLInitCommon.h"
#endif /* not defined THREAD_SAFE_PDFL */

/* for Adobe use only */
#define _PagePDEContentHFT_LATEST_VERSION 0x00060000
#define _PagePDEContentHFT_LAST_BETA_COMPATIBLE_VERSION 0x00060000
#define _PagePDEContentHFT_IS_BETA 0

/* for public use */
#define PagePDEContentHFT_LATEST_VERSION (_PagePDEContentHFT_IS_BETA ? (kHFT_IN_BETA_FLAG | _PagePDEContentHFT_LATEST_VERSION) : _PagePDEContentHFT_LATEST_VERSION)

#define PagePDEContentHFT_VERSION_4   0x00040000
#define PagePDEContentHFT_VERSION_5   0x00050000
#define PagePDEContentHFT_VERSION_6   0x00060000

#ifdef __cplusplus
extern "C" {
#endif

#include "PEExpT.h"
#include "PEVers.h"

/* Prototypes for notification procs (mirrored in PagePDEContent.h) */
typedef ACCBPROTO1 void (ACCBPROTO2 *PagePDEContentDidChangeNPROTO)(PDPage pdPage, PDEContent pagesPDEContent);

typedef ACCBPROTO1 void (ACCBPROTO2 *PagePDEContentNotCachedNPROTO)(PDPage pdPage, PDEContent pagesPDEContent);

#ifdef NPROC /* may be already defined */
#undef NPROC
#endif

#if !PLUGIN
	/* Static link */
	#define NPROC(returnType, name, params)		\
		ACEX1 returnType ACEX2 name params;
    #define UNPROC NPROC
	#include "PgCntProcs.h"
	#undef NPROC
	#undef UNPROC
#else
	/* HFT version */
   #include "PIRequir.h"

	/* Enumerate the selectors */
	#define NPROC(returnType, name, params)			\
		name##SEL,
	#define UNPROC NPROC
	enum {
		PageContentServerBAD_SELECTOR,
		#include "PgCntProcs.h"
		PageContentServerNUMSELECTORSplusOne
	};
	
	#define PageContentServerNUMSELECTORS (PageContentServerNUMSELECTORSplusOne - 1)
	
	/* Create the prototypes */
	#undef NPROC
	#undef UNPROC
	#define NPROC(returnType, name, params)		\
		typedef ACCBPROTO1 returnType (ACCBPROTO2 *name##SELPROTO)params;
#if READER_PLUGIN
    #define UNPROC(returnType, name, params) 
#else
   #define UNPROC NPROC
#endif
		#include "PgCntProcs.h"
	#undef NPROC
	#undef UNPROC
	
#ifdef THREAD_SAFE_PDFL
	#define gPagePDEContentHFT (GetHFTLocations()->pagePDEContentHFT)
	#define gPagePDEContentVersion (GetHFTLocations()->pagePDEContentVersion)
#else
	extern HFT gPagePDEContentHFT;
	extern ASUns32 gPagePDEContentVersion;
#endif /* defined THREAD_SAFE_PDFL */

	/* Define the macros */
#if  PI_PAGE_PDE_CONTENT_VERSION != 0 	
	/*  PI_PAGE_PDE_CONTENT_VERSION >= 0x00040000 */
	#define PDPageAcquirePDEContent (ACROASSERT(gPagePDEContentVersion >=PagePDEContentHFT_VERSION_4), *((PDPageAcquirePDEContentSELPROTO)(gPagePDEContentHFT[PDPageAcquirePDEContentSEL]))) 
	#define PDPageReleasePDEContent (ACROASSERT(gPagePDEContentVersion >=PagePDEContentHFT_VERSION_4), *((PDPageReleasePDEContentSELPROTO)(gPagePDEContentHFT[PDPageReleasePDEContentSEL]))) 
	#define PDPageSetPDEContent (ACROASSERT(gPagePDEContentVersion >=PagePDEContentHFT_VERSION_4), *((PDPageSetPDEContentSELPROTO)(gPagePDEContentHFT[PDPageSetPDEContentSEL]))) 
	#define PDPagePDEContentWasChanged (ACROASSERT(gPagePDEContentVersion >=PagePDEContentHFT_VERSION_4), *((PDPagePDEContentWasChangedSELPROTO)(gPagePDEContentHFT[PDPagePDEContentWasChangedSEL]))) 
	#define PDPageRegisterForPDEContentChanged (ACROASSERT(gPagePDEContentVersion >=PagePDEContentHFT_VERSION_4), *((PDPageRegisterForPDEContentChangedSELPROTO)(gPagePDEContentHFT[PDPageRegisterForPDEContentChangedSEL]))) 
	#define PDPageUnRegisterForPDEContentChanged (ACROASSERT(gPagePDEContentVersion >=PagePDEContentHFT_VERSION_4), *((PDPageUnRegisterForPDEContentChangedSELPROTO)(gPagePDEContentHFT[PDPageUnRegisterForPDEContentChangedSEL]))) 
	#define PDPageRegisterForPDEContentNotCached (ACROASSERT(gPagePDEContentVersion >=PagePDEContentHFT_VERSION_4), *((PDPageRegisterForPDEContentNotCachedSELPROTO)(gPagePDEContentHFT[PDPageRegisterForPDEContentNotCachedSEL]))) 
	#define PDPageUnRegisterForPDEContentNotCached (ACROASSERT(gPagePDEContentVersion >=PagePDEContentHFT_VERSION_4), *((PDPageUnRegisterForPDEContentNotCachedSELPROTO)(gPagePDEContentHFT[PDPageUnRegisterForPDEContentNotCachedSEL]))) 

	#define PDPageGetPDEContentFlags (ACROASSERT(gPagePDEContentVersion >=PagePDEContentHFT_VERSION_4), *((PDPageGetPDEContentFlagsSELPROTO)(gPagePDEContentHFT[PDPageGetPDEContentFlagsSEL]))) 
	#define PDPageSetPDEContentFlags (ACROASSERT(gPagePDEContentVersion >=PagePDEContentHFT_VERSION_4), *((PDPageSetPDEContentFlagsSELPROTO)(gPagePDEContentHFT[PDPageSetPDEContentFlagsSEL]))) 
	#define PDPageGetPDEContentFilters (ACROASSERT(gPagePDEContentVersion >=PagePDEContentHFT_VERSION_4), *((PDPageGetPDEContentFiltersSELPROTO)(gPagePDEContentHFT[PDPageGetPDEContentFiltersSEL]))) 
	#define PDPageSetPDEContentFilters (ACROASSERT(gPagePDEContentVersion >=PagePDEContentHFT_VERSION_4), *((PDPageSetPDEContentFiltersSELPROTO)(gPagePDEContentHFT[PDPageSetPDEContentFiltersSEL]))) 

	/*  PI_PAGE_PDE_CONTENT_VERSION >= 0x00050000 */
	#define PDPageSuspendPDEContentChanged (ACROASSERT(gPagePDEContentVersion >=PagePDEContentHFT_VERSION_5), *((PDPageSuspendPDEContentChangedSELPROTO)(gPagePDEContentHFT[PDPageSuspendPDEContentChangedSEL]))) 
	#define PDPageResumePDEContentChanged (ACROASSERT(gPagePDEContentVersion >=PagePDEContentHFT_VERSION_5), *((PDPageResumePDEContentChangedSELPROTO)(gPagePDEContentHFT[PDPageResumePDEContentChangedSEL]))) 
	/*  PI_PAGE_PDE_CONTENT_VERSION >= 0x00060000 */
	#define PDPageSetPDEContentCanRaise (ACROASSERT(gPagePDEContentVersion >=PagePDEContentHFT_VERSION_6), *((PDPageSetPDEContentCanRaiseSELPROTO)(gPagePDEContentHFT[PDPageSetPDEContentCanRaiseSEL]))) 
#endif /* PI_PAGE_PDE_CONTENT_VERSION != 0 */
	
#endif /* PLUGIN */

#ifdef __cplusplus
}
#endif

#endif /* _H_PAGEPDECONTENTCALLS */

