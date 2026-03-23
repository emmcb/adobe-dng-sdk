/*********************************************************************

 ADOBE SYSTEMS INCORPORATED
 Copyright (C) 1998-2006 Adobe Systems Incorporated
 All rights reserved.

 NOTICE: Adobe permits you to use, modify, and distribute this file
 in accordance with the terms of the Adobe license agreement
 accompanying it. If you have received this file from a source other
 than Adobe, then your use, modification, or distribution of it
 requires the prior written permission of Adobe.

 ---------------------------------------------------------------------

 PSFCalls.h

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

#ifndef _H_PSFCalls
#define _H_PSFCalls
#include "acroassert.h"
#ifdef THREAD_SAFE_PDFL
#include "PDFLInitCommon.h"
#endif /* defined THREAD_SAFE_PDFL */

/* for Adobe use only */
#define _PDSysFontHFT_LATEST_VERSION 0x00040000
#define _PDSysFontHFT_LAST_BETA_COMPATIBLE_VERSION 0x00040000
#define _PDSysFontHFT_IS_BETA 0

/* for public use */
#define PDSysFontHFT_LATEST_VERSION (_PDSysFontHFT_IS_BETA ? (kHFT_IN_BETA_FLAG | _PDSysFontHFT_LATEST_VERSION) : _PDSysFontHFT_LATEST_VERSION)

#define PDSysFontHFT_VERSION_4   0x00040000

#ifdef __cplusplus
extern "C" {
#endif

#include "PEExpT.h"
#include "PDSysFontExpT.h"
#include "PEVers.h"

#ifdef NPROC /* This might be defined in sys/procs.h */
#undef NPROC
#endif /* NPROC */
	
#if !PLUGIN
	/* Static link */
	#define NPROC(returnType, name, params)		\
		ACEX1 returnType ACEX2 name params;
	#define UNPROC	NPROC
	#include "PSFProcs.h"
	#undef NPROC
	#undef UNPROC
#endif /* !PLUGIN */

#if PLUGIN
	/* HFT version */
	#include "PIRequir.h"
	
	/* Enumerate the selectors */
	#define NPROC(returnType, name, params)			\
		name##SEL,
   #define UNPROC NPROC
	
	enum {
		PDSysFontBAD_SELECTOR,
		#include "PSFProcs.h"
		PDSysFontNUMSELECTORSplusOne
	};
	
	#define PDSysFontNUMSELECTORS (PDSysFontNUMSELECTORSplusOne - 1)
	
	/* Create the prototypes */
	#undef NPROC
	#undef UNPROC
	#define NPROC(returnType, name, params)		\
		typedef ACCBPROTO1 returnType (ACCBPROTO2 *name##SELPROTO)params;
#if READER_PLUGIN
	/* Force an error for Exchange procs */
	#define UNPROC(returnType, name, params)
#else
	#define UNPROC NPROC
#endif
		#include "PSFProcs.h"
	#undef NPROC
	#undef UNPROC

#if PI_PDSYSFONT_VERSION != 0
#ifdef THREAD_SAFE_PDFL
	#define gPDSysFontHFT (GetHFTLocations()->pdSysFontHFT)
	#define gPDSysFontVersion (GetHFTLocations()->pdSysFontVersion)
#else
	extern HFT gPDSysFontHFT;
	extern ASUns32 gPDSysFontVersion;
#endif /* defined THREAD_SAFE_PDFL */

#if !STATIC_HFT
	/* Define the macros */
	#define PDEnumSysFonts (ACROASSERT(gPDSysFontVersion >=PDSysFontHFT_VERSION_4), ((PDEnumSysFontsSELPROTO)(gPDSysFontHFT[PDEnumSysFontsSEL]))) 
	#define PDFindSysFont (ACROASSERT(gPDSysFontVersion >=PDSysFontHFT_VERSION_4), *((PDFindSysFontSELPROTO)(gPDSysFontHFT[PDFindSysFontSEL]))) 
	#define PDFindSysFontEx (ACROASSERT(gPDSysFontVersion >=PDSysFontHFT_VERSION_4), *((PDFindSysFontExSELPROTO)(gPDSysFontHFT[PDFindSysFontExSEL]))) 
	#define PDFindSysFontForPDEFont (ACROASSERT(gPDSysFontVersion >=PDSysFontHFT_VERSION_4), *((PDFindSysFontForPDEFontSELPROTO)(gPDSysFontHFT[PDFindSysFontForPDEFontSEL]))) 
	#define PDSysFontGetAttrs (ACROASSERT(gPDSysFontVersion >=PDSysFontHFT_VERSION_4), *((PDSysFontGetAttrsSELPROTO)(gPDSysFontHFT[PDSysFontGetAttrsSEL]))) 
	#define PDSysFontGetWidths (ACROASSERT(gPDSysFontVersion >=PDSysFontHFT_VERSION_4), *((PDSysFontGetWidthsSELPROTO)(gPDSysFontHFT[PDSysFontGetWidthsSEL]))) 
	#define PDSysFontGetWidthsEx (ACROASSERT(gPDSysFontVersion >=PDSysFontHFT_VERSION_4), *((PDSysFontGetWidthsExSELPROTO)(gPDSysFontHFT[PDSysFontGetWidthsExSEL]))) 
	#define PDSysFontGetEncoding (ACROASSERT(gPDSysFontVersion >=PDSysFontHFT_VERSION_4), *((PDSysFontGetEncodingSELPROTO)(gPDSysFontHFT[PDSysFontGetEncodingSEL]))) 
	#define PDSysFontGetInfo (ACROASSERT(gPDSysFontVersion >=PDSysFontHFT_VERSION_4), *((PDSysFontGetInfoSELPROTO)(gPDSysFontHFT[PDSysFontGetInfoSEL]))) 
	#define PDSysFontGetName (ACROASSERT(gPDSysFontVersion >=PDSysFontHFT_VERSION_4), *((PDSysFontGetNameSELPROTO)(gPDSysFontHFT[PDSysFontGetNameSEL]))) 
	#define PDSysFontAcquirePlatformData (ACROASSERT(gPDSysFontVersion >=PDSysFontHFT_VERSION_4), *((PDSysFontAcquirePlatformDataSELPROTO)(gPDSysFontHFT[PDSysFontAcquirePlatformDataSEL])))
	#define PDSysFontReleasePlatformData (ACROASSERT(gPDSysFontVersion >=PDSysFontHFT_VERSION_4), *((PDSysFontReleasePlatformDataSELPROTO)(gPDSysFontHFT[PDSysFontReleasePlatformDataSEL])))
	#define PDSysFontGetCIDSystemInfo (ACROASSERT(gPDSysFontVersion >=PDSysFontHFT_VERSION_4), *((PDSysFontGetCIDSystemInfoSELPROTO)(gPDSysFontHFT[PDSysFontGetCIDSystemInfoSEL]))) 
	#define PDSysFontGetType0Widths (ACROASSERT(gPDSysFontVersion >=PDSysFontHFT_VERSION_4), *((PDSysFontGetType0WidthsSELPROTO)(gPDSysFontHFT[PDSysFontGetType0WidthsSEL]))) 
	#define PDEmbedSysFontForPDEFont (ACROASSERT(gPDSysFontVersion >=PDSysFontHFT_VERSION_4), *((PDEmbedSysFontForPDEFontSELPROTO)(gPDSysFontHFT[PDEmbedSysFontForPDEFontSEL])))

/* If you add, delete, or modify procs listed in this file please read the instructions at the top about how to properly change the version number */

#endif /* !STATIC_HFT */	

#endif /* PI_PDSYSFONT_VERSION != 0 */

#endif

#ifdef __cplusplus
}
#endif

#endif /* _H_PSFCalls */

