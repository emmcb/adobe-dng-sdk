/*********************************************************************

 ADOBE SYSTEMS INCORPORATED
 Copyright (C) 2000-2006 Adobe Systems Incorporated
 All rights reserved.

 NOTICE: Adobe permits you to use, modify, and distribute this file
 in accordance with the terms of the Adobe license agreement
 accompanying it. If you have received this file from a source other
 than Adobe, then your use, modification, or distribution of it
 requires the prior written permission of Adobe.

 ---------------------------------------------------------------------

 PDMetadataCalls.h

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

#ifndef _H_PDMetadataCalls
#define _H_PDMetadataCalls
#include "acroassert.h"
#ifdef THREAD_SAFE_PDFL
#include "PDFLInitCommon.h"
#endif /* defined THREAD_SAFE_PDFL */

/* for Adobe use only */
#define _PDMetadataHFT_LATEST_VERSION 0x00080000
#define _PDMetadataHFT_LAST_BETA_COMPATIBLE_VERSION 0x00080000
#define _PDMetadataHFT_IS_BETA 0

/* for public use */
#define PDMetadataHFT_LATEST_VERSION (_PDMetadataHFT_IS_BETA ? (kHFT_IN_BETA_FLAG | _PDMetadataHFT_LATEST_VERSION) : _PDMetadataHFT_LATEST_VERSION)

#define PDMetadataHFT_VERSION_5   0x00050000
#define PDMetadataHFT_VERSION_6   0x00060000
#define PDMetadataHFT_VERSION_7   0x00070000
#define PDMetadataHFT_VERSION_8   0x00080000

#include "ASExpT.h"
#include "PDMetadataHFTVers.h"
#include "PDMetadataExpT.h"

#define IN
#define OUT

#ifdef __cplusplus
extern "C" {
#endif

#ifdef NPROC /* This might be defined in sys/procs.h */
#undef NPROC
#endif /* NPROC */

#if PLUGIN
#define PEX1 ACCB1
#define PEX2 ACCB2
#endif

#ifndef PEX1
#define PEX1 ACEX1
#define PEX2 ACEX2
#endif

#if ! PLUGIN /* ACROBAT_LIBRARY */
	/* Implementation or static link */
	#define NPROC(returnType, name, params)		\
		ACEX1 returnType ACEX2 name params;
	#define NOPROC(name)
	#include "PDMetadataProcs.h"
	#undef NPROC
	#undef NOPROC

#else
#include "PIRequir.h"
	/* Enumerate the selectors */
	#define NPROC(returnType, name, params)			\
		name##SEL,
	#define XNPROC NPROC
	
	enum {
		PDMetadataBAD_SELECTOR,
		#include "PDMetadataProcs.h"
		PDMetadataNUMSELECTORSplusOne
	};
	
	#define PDMetadataNUMSELECTORS (PDMetadataNUMSELECTORSplusOne - 1)
	
	/* Create the prototypes */
	#undef NPROC
	#undef XNPROC

	#define NPROC(returnType, name, params)		\
		typedef ACCBPROTO1 returnType (ACCBPROTO2 *name##SELPROTO)params;
#if READER_PLUGIN
   /* Force an error for Exchange procs */
   #define XNPROC(returnType, name, params)
#else
   #define XNPROC NPROC
#endif
		#include "PDMetadataProcs.h"
	#undef NPROC
	#undef XNPROC

#if PI_PDMETADATA_VERSION != 0
	
#ifdef THREAD_SAFE_PDFL
	#define gPDMetadataHFT (GetHFTLocations()->pdMetadataHFT)
	#define gPDMetadataVersion (GetHFTLocations()->pdMetadataVersion)
#else
	extern HFT gPDMetadataHFT;
	extern ASUns32 gPDMetadataVersion;
#endif /* defined THREAD_SAFE_PDFL */

	/* Define the macros */
    #define PDDocGetXAPMetadata (ACROASSERT(gPDMetadataVersion >=PDMetadataHFT_VERSION_5), *((PDDocGetXAPMetadataSELPROTO)(gPDMetadataHFT[PDDocGetXAPMetadataSEL])))
    #define PDDocSetXAPMetadata (ACROASSERT(gPDMetadataVersion >=PDMetadataHFT_VERSION_5), *((PDDocSetXAPMetadataSELPROTO)(gPDMetadataHFT[PDDocSetXAPMetadataSEL])))
    #define CosDictGetXAPMetadata (ACROASSERT(gPDMetadataVersion >=PDMetadataHFT_VERSION_5), *((CosDictGetXAPMetadataSELPROTO)(gPDMetadataHFT[CosDictGetXAPMetadataSEL])))
    #define CosDictSetXAPMetadata (ACROASSERT(gPDMetadataVersion >=PDMetadataHFT_VERSION_5), *((CosDictSetXAPMetadataSELPROTO)(gPDMetadataHFT[CosDictSetXAPMetadataSEL])))
    #define PDDocCalculateImplicitMetadata (ACROASSERT(gPDMetadataVersion >=PDMetadataHFT_VERSION_5), *((PDDocCalculateImplicitMetadataSELPROTO)(gPDMetadataHFT[PDDocCalculateImplicitMetadataSEL])))
    #define PDEContainerGetXAPMetadata (ACROASSERT(gPDMetadataVersion >=PDMetadataHFT_VERSION_5), *((PDEContainerGetXAPMetadataSELPROTO)(gPDMetadataHFT[PDEContainerGetXAPMetadataSEL])))
    #define PDEContainerSetXAPMetadata (ACROASSERT(gPDMetadataVersion >=PDMetadataHFT_VERSION_5), *((PDEContainerSetXAPMetadataSELPROTO)(gPDMetadataHFT[PDEContainerSetXAPMetadataSEL])))

/* new with PI_PDMETADATA_VERSION >= 0x00060000 */

    #define PDDocGetXAPMetadataProperty (ACROASSERT(gPDMetadataVersion >=PDMetadataHFT_VERSION_6),*((PDDocGetXAPMetadataPropertySELPROTO)(gPDMetadataHFT[PDDocGetXAPMetadataPropertySEL])))
    #define PDDocSetXAPMetadataProperty (ACROASSERT(gPDMetadataVersion >=PDMetadataHFT_VERSION_6),*((PDDocSetXAPMetadataPropertySELPROTO)(gPDMetadataHFT[PDDocSetXAPMetadataPropertySEL])))

/* PI_PDMETADATA_VERSION >= 0x00060000 */

/* new with PI_PDMETADATA_VERSION >= 0x00070000 */

    #define PDDocGetMergedXAPKeywords (ACROASSERT(gPDMetadataVersion >=PDMetadataHFT_VERSION_7),*((PDDocGetMergedXAPKeywordsSELPROTO)(gPDMetadataHFT[PDDocGetMergedXAPKeywordsSEL])))
	#define PDDocMergeXAPKeywords (ACROASSERT(gPDMetadataVersion >=PDMetadataHFT_VERSION_7),*((PDDocMergeXAPKeywordsSELPROTO)(gPDMetadataHFT[PDDocMergeXAPKeywordsSEL])))
    
/* PI_PDMETADATA_VERSION >= 0x00070000 */

/* new with PI_PDMETADATA_VERSION >= 0x00080000 */
#define PDDocGetXAPMetadataArrayItem (ACROASSERT(gPDMetadataVersion >=PDMetadataHFT_VERSION_8),*((PDDocGetXAPMetadataArrayItemSELPROTO)(gPDMetadataHFT[PDDocGetXAPMetadataArrayItemSEL])))
#define PDDocSetXAPMetadataArrayItem (ACROASSERT(gPDMetadataVersion >=PDMetadataHFT_VERSION_8),*((PDDocSetXAPMetadataArrayItemSELPROTO)(gPDMetadataHFT[PDDocSetXAPMetadataArrayItemSEL])))
#define PDDocCountXAPMetadataArrayItems (ACROASSERT(gPDMetadataVersion >=PDMetadataHFT_VERSION_8),*((PDDocCountXAPMetadataArrayItemsSELPROTO)(gPDMetadataHFT[PDDocCountXAPMetadataArrayItemsSEL])))

/* PI_PDMETADATA_VERSION >= 0x00080000 */

/* If you add, delete, or modify procs listed in this file please read the instructions at the top about how to properly change the version number */

#endif /* PI_PDMETADATA_VERSION != 0 */

#endif /* PLUGIN */

#ifdef __cplusplus
}
#endif

#endif /* _H_PDMetadataCalls */

