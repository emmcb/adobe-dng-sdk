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

 PDSReadCalls.h

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

#ifndef _H_PDSReadCalls
#define _H_PDSReadCalls
#include "acroassert.h"

/* for Adobe use only */
#define _PDSRead_LATEST_VERSION 0x000D0000
#define _PDSRead_LAST_BETA_COMPATIBLE_VERSION 0x000D0000
#define _PDSRead_IS_BETA 0

/* for public use */
#define PDSReadHFT_LATEST_VERSION (_PDSRead_IS_BETA ? (kHFT_IN_BETA_FLAG | _PDSRead_LATEST_VERSION) : _PDSRead_LATEST_VERSION)

#define PDSRead_VERSION_2   0x00020000
#define PDSRead_VERSION_5   0x00050000
#define PDSRead_VERSION_6   0x00060000
#define PDSRead_VERSION_7	0x00070000
#define PDSRead_VERSION_8   0x00080000
#define PDSRead_VERSION_13  PDSReadHFT_LATEST_VERSION


#include "PDSReadHFTVers.h"
#include "PDSExpT.h"
#include "PEExpT.h"

#ifdef __cplusplus
extern "C" {
#endif

#if PLUGIN
#define PEX1 ACCB1
#define PEX2 ACCB2
#endif

#ifndef PEX1
#define PEX1 ACEX1
#define PEX2 ACEX2
#endif

#ifdef NPROC /* This might be defined in sys/procs.h */
#undef NPROC
#endif /* NPROC */
	
#if PDSEDIT_IN_CORE || ACROBAT_LIBRARY
#if HAS_PDSEDIT_READ_PROCS
	/* Implementation or static link */
	#define NPROC(returnType, name, params)		\
		ACEX1 returnType ACEX2 name params;
	#include "PDSReadProcs.h"
	#undef NPROC
#endif /* HAS_PDSEDIT_READ_PROCS */
#else
#if PLUGIN
#include "PIRequir.h"
#ifdef THREAD_SAFE_PDFL
#include "PDFLInitCommon.h"
#endif /* defined THREAD_SAFE_PDFL */

#ifdef NPROC /* This might be defined in sys/procs.h (and the above includes seem to bring that file in) */
#undef NPROC
#endif /* NPROC */

	/* Enumerate the selectors */
	#define NPROC(returnType, name, params)			\
		name##SEL,
	
	enum {
		PDSReadBAD_SELECTOR,
		#include "PDSReadProcs.h"
		PDSReadNUMSELECTORSplusOne
	};
	
	#define PDSReadNUMSELECTORS (PDSReadNUMSELECTORSplusOne - 1)
	
	/* Create the prototypes */
	#undef NPROC
	#define NPROC(returnType, name, params)		\
		typedef ACCBPROTO1 returnType (ACCBPROTO2 *name##SELPROTO)params;
		#include "PDSReadProcs.h"
	#undef NPROC

#if PI_PDSEDIT_READ_VERSION != 0

#ifdef THREAD_SAFE_PDFL
	#define gPDSReadHFT (GetHFTLocations()->pdsReadHFT)
	#define gPDSReadVersion (GetHFTLocations()->pdsReadVersion)
#else
	extern HFT gPDSReadHFT;
	extern ASUns32 gPDSReadVersion;
#endif /* defined THREAD_SAFE_PDFL */

	/* Define the macros */
	#define PDDocGetStructTreeRoot (ACROASSERT(gPDSReadVersion >=PDSRead_VERSION_2), *((PDDocGetStructTreeRootSELPROTO)(gPDSReadHFT[PDDocGetStructTreeRootSEL])))
    #define PDSTreeRootGetNumKids (ACROASSERT(gPDSReadVersion >=PDSRead_VERSION_2), *((PDSTreeRootGetNumKidsSELPROTO)(gPDSReadHFT[PDSTreeRootGetNumKidsSEL])))
    #define PDSTreeRootGetKid (ACROASSERT(gPDSReadVersion >=PDSRead_VERSION_2), *((PDSTreeRootGetKidSELPROTO)(gPDSReadHFT[PDSTreeRootGetKidSEL])))
	#define PDSTreeRootGetRoleMap (ACROASSERT(gPDSReadVersion >=PDSRead_VERSION_2), *((PDSTreeRootGetRoleMapSELPROTO)(gPDSReadHFT[PDSTreeRootGetRoleMapSEL])))
	#define PDSTreeRootGetClassMap (ACROASSERT(gPDSReadVersion >=PDSRead_VERSION_2), *((PDSTreeRootGetClassMapSELPROTO)(gPDSReadHFT[PDSTreeRootGetClassMapSEL])))
    #define PDSTreeRootGetElementFromID (ACROASSERT(gPDSReadVersion >=PDSRead_VERSION_2), *((PDSTreeRootGetElementFromIDSELPROTO)(gPDSReadHFT[PDSTreeRootGetElementFromIDSEL])))

	#define PDSElementGetType (ACROASSERT(gPDSReadVersion >=PDSRead_VERSION_2), *((PDSElementGetTypeSELPROTO)(gPDSReadHFT[PDSElementGetTypeSEL])))
	#define PDSElementGetParent (ACROASSERT(gPDSReadVersion >=PDSRead_VERSION_2), *((PDSElementGetParentSELPROTO)(gPDSReadHFT[PDSElementGetParentSEL])))
	#define PDSElementGetTitle (ACROASSERT(gPDSReadVersion >=PDSRead_VERSION_2), *((PDSElementGetTitleSELPROTO)(gPDSReadHFT[PDSElementGetTitleSEL])))
	#define PDSElementGetRevision (ACROASSERT(gPDSReadVersion >=PDSRead_VERSION_2), *((PDSElementGetRevisionSELPROTO)(gPDSReadHFT[PDSElementGetRevisionSEL])))
	#define PDSElementGetNumAttrObjs (ACROASSERT(gPDSReadVersion >=PDSRead_VERSION_2), *((PDSElementGetNumAttrObjsSELPROTO)(gPDSReadHFT[PDSElementGetNumAttrObjsSEL])))
	#define PDSElementGetAttrObj (ACROASSERT(gPDSReadVersion >=PDSRead_VERSION_2), *((PDSElementGetAttrObjSELPROTO)(gPDSReadHFT[PDSElementGetAttrObjSEL])))
	#define PDSElementGetNumClasses (ACROASSERT(gPDSReadVersion >=PDSRead_VERSION_2), *((PDSElementGetNumClassesSELPROTO)(gPDSReadHFT[PDSElementGetNumClassesSEL])))
	#define PDSElementGetClass (ACROASSERT(gPDSReadVersion >=PDSRead_VERSION_2), *((PDSElementGetClassSELPROTO)(gPDSReadHFT[PDSElementGetClassSEL])))
	#define PDSElementGetAlt (ACROASSERT(gPDSReadVersion >=PDSRead_VERSION_2), *((PDSElementGetAltSELPROTO)(gPDSReadHFT[PDSElementGetAltSEL])))
	#define PDSElementGetNumKids (ACROASSERT(gPDSReadVersion >=PDSRead_VERSION_2), *((PDSElementGetNumKidsSELPROTO)(gPDSReadHFT[PDSElementGetNumKidsSEL])))
	#define PDSElementGetKid (ACROASSERT(gPDSReadVersion >=PDSRead_VERSION_2), *((PDSElementGetKidSELPROTO)(gPDSReadHFT[PDSElementGetKidSEL])))
    #define PDSElementGetFirstPage (ACROASSERT(gPDSReadVersion >=PDSRead_VERSION_2), *((PDSElementGetFirstPageSELPROTO)(gPDSReadHFT[PDSElementGetFirstPageSEL])))

    #define PDSElementGetID (ACROASSERT(gPDSReadVersion >=PDSRead_VERSION_2), *((PDSElementGetIDSELPROTO)(gPDSReadHFT[PDSElementGetIDSEL])))
    #define PDSElementGetStructTreeRoot (ACROASSERT(gPDSReadVersion >=PDSRead_VERSION_2), *((PDSElementGetStructTreeRootSELPROTO)(gPDSReadHFT[PDSElementGetStructTreeRootSEL])))

	#define PDSAttrObjGetOwner (ACROASSERT(gPDSReadVersion >=PDSRead_VERSION_2), *((PDSAttrObjGetOwnerSELPROTO)(gPDSReadHFT[PDSAttrObjGetOwnerSEL])))

	#define PDSMCGetParent (ACROASSERT(gPDSReadVersion >=PDSRead_VERSION_2), *((PDSMCGetParentSELPROTO)(gPDSReadHFT[PDSMCGetParentSEL])))

	#define PDSOBJGetParent (ACROASSERT(gPDSReadVersion >=PDSRead_VERSION_2), *((PDSOBJGetParentSELPROTO)(gPDSReadHFT[PDSOBJGetParentSEL])))

	#define PDSRoleMapGetDirectMap (ACROASSERT(gPDSReadVersion >=PDSRead_VERSION_2), *((PDSRoleMapGetDirectMapSELPROTO)(gPDSReadHFT[PDSRoleMapGetDirectMapSEL])))
	#define PDSRoleMapDoesMap (ACROASSERT(gPDSReadVersion >=PDSRead_VERSION_2), *((PDSRoleMapDoesMapSELPROTO)(gPDSReadHFT[PDSRoleMapDoesMapSEL])))

	#define PDSClassMapGetNumAttrObjs (ACROASSERT(gPDSReadVersion >=PDSRead_VERSION_2), *((PDSClassMapGetNumAttrObjsSELPROTO)(gPDSReadHFT[PDSClassMapGetNumAttrObjsSEL])))
	#define PDSClassMapGetAttrObj (ACROASSERT(gPDSReadVersion >=PDSRead_VERSION_2), *((PDSClassMapGetAttrObjSELPROTO)(gPDSReadHFT[PDSClassMapGetAttrObjSEL])))
	/*	#define XXX (*((XXXSELPROTO)(gPDSReadHFT[XXXSEL]))) */

/*
** Acrobat 5.0 additions
*/

/* PI_PDSEDIT_READ_VERSION >= 0x00050000	*/

	#define PDSElementGetKidEx		(ACROASSERT(gPDSReadVersion >=PDSRead_VERSION_5), *((PDSElementGetKidExSELPROTO)(gPDSReadHFT[PDSElementGetKidExSEL])))
	#define PDSElementGetActualText	(ACROASSERT(gPDSReadVersion >=PDSRead_VERSION_5), *((PDSElementGetActualTextSELPROTO)(gPDSReadHFT[PDSElementGetActualTextSEL])))
	#define PDSElementGetLanguage	(ACROASSERT(gPDSReadVersion >=PDSRead_VERSION_5), *((PDSElementGetLanguageSELPROTO)(gPDSReadHFT[PDSElementGetLanguageSEL])))

	#define PDSElementHasAlt		(ACROASSERT(gPDSReadVersion >=PDSRead_VERSION_5), *((PDSElementHasAltSELPROTO)(gPDSReadHFT[PDSElementHasAltSEL])))
	#define PDSElementHasActualText	(ACROASSERT(gPDSReadVersion >=PDSRead_VERSION_5), *((PDSElementHasActualTextSELPROTO)(gPDSReadHFT[PDSElementHasActualTextSEL])))
	#define PDSElementHasLanguage	(ACROASSERT(gPDSReadVersion >=PDSRead_VERSION_5), *((PDSElementHasLanguageSELPROTO)(gPDSReadHFT[PDSElementHasLanguageSEL])))


/* PI_PDSEDIT_READ_VERSION >= 0x00060000 */

	#define PDSElementGetKidWithMCInfo (ACROASSERT(gPDSReadVersion >=PDSRead_VERSION_6), *((PDSElementGetKidWithMCInfoSELPROTO)(gPDSReadHFT[PDSElementGetKidWithMCInfoSEL])))
	#define PDSMCGetInfo (ACROASSERT(gPDSReadVersion >=PDSRead_VERSION_6), *((PDSMCGetInfoSELPROTO)(gPDSReadHFT[PDSMCGetInfoSEL])))
	#define PDSMCIDGetParent (ACROASSERT(gPDSReadVersion >=PDSRead_VERSION_6), *((PDSMCIDGetParentSELPROTO)(gPDSReadHFT[PDSMCIDGetParentSEL])))
	#define PDSMCGetPDEContainer (ACROASSERT(gPDSReadVersion >=PDSRead_VERSION_6), *((PDSMCGetPDEContainerSELPROTO)(gPDSReadHFT[PDSMCGetPDEContainerSEL])))
	#define PDSElementGetCosObj (ACROASSERT(gPDSReadVersion >=PDSRead_VERSION_6), *((PDSElementGetCosObjSELPROTO)(gPDSReadHFT[PDSElementGetCosObjSEL])))
	#define PDSAttrObjGetCosObj (ACROASSERT(gPDSReadVersion >=PDSRead_VERSION_6), *((PDSAttrObjGetCosObjSELPROTO)(gPDSReadHFT[PDSAttrObjGetCosObjSEL])))
    

/* PI_PDSEDIT_READ_VERSION >= 0x00070000 */
	#define PDDocHasUserProperties (ACROASSERT(gPDSReadVersion >=PDSRead_VERSION_7), *((PDDocHasUserPropertiesSELPROTO)(gPDSReadHFT[PDDocHasUserPropertiesSEL])))
	#define PDDocEnumPDSElementsWithUserProperties (ACROASSERT(gPDSReadVersion >=PDSRead_VERSION_7), *((PDDocEnumPDSElementsWithUserPropertiesSELPROTO)(gPDSReadHFT[PDDocEnumPDSElementsWithUserPropertiesSEL])))
	#define PDSElementHasUserProperties (ACROASSERT(gPDSReadVersion >=PDSRead_VERSION_7), *((PDSElementHasUserPropertiesSELPROTO)(gPDSReadHFT[PDSElementHasUserPropertiesSEL])))
	#define PDSElementEnumUserPropertiesAsASText (ACROASSERT(gPDSReadVersion >=PDSRead_VERSION_7), *((PDSElementEnumUserPropertiesAsASTextSELPROTO)(gPDSReadHFT[PDSElementEnumUserPropertiesAsASTextSEL])))
	#define PDSElementEnumUserPropertiesAsCosObj (ACROASSERT(gPDSReadVersion >=PDSRead_VERSION_7), *((PDSElementEnumUserPropertiesAsCosObjSELPROTO)(gPDSReadHFT[PDSElementEnumUserPropertiesAsCosObjSEL])))
	#define PDSElementFindAncestorWithUserProperties (ACROASSERT(gPDSReadVersion >=PDSRead_VERSION_7), *((PDSElementFindAncestorWithUserPropertiesSELPROTO)(gPDSReadHFT[PDSElementFindAncestorWithUserPropertiesSEL])))
	#define PDSElementEnumKidsWithUserProperties (ACROASSERT(gPDSReadVersion >=PDSRead_VERSION_7), *((PDSElementEnumKidsWithUserPropertiesSELPROTO)(gPDSReadHFT[PDSElementEnumKidsWithUserPropertiesSEL])))

/* PI_PDSEDIT_READ_VERSION >= 0x00080000 */
    #define PDSElementGetTitleASText (ACROASSERT(gPDSReadVersion >=PDSRead_VERSION_8), *((PDSElementGetTitleASTextSELPROTO)(gPDSReadHFT[PDSElementGetTitleASTextSEL])))
    #define PDSElementGetAltASText (ACROASSERT(gPDSReadVersion >=PDSRead_VERSION_8), *((PDSElementGetAltASTextSELPROTO)(gPDSReadHFT[PDSElementGetAltASTextSEL])))
    #define PDSElementGetActualTextASText	(ACROASSERT(gPDSReadVersion >=PDSRead_VERSION_8), *((PDSElementGetActualTextASTextSELPROTO)(gPDSReadHFT[PDSElementGetActualTextASTextSEL])))
    #define PDSElementExportUserProperties	(ACROASSERT(gPDSReadVersion >=PDSRead_VERSION_8), *((PDSElementExportUserPropertiesSELPROTO)(gPDSReadHFT[PDSElementExportUserPropertiesSEL])))

/* PI_PDSEDIT_READ_VERSION >= 0x000D0000 */
    #define PDSElementGetNumAssociatedFiles (ACROASSERT(gPDSReadVersion >=PDSRead_VERSION_13), *((PDSElementGetNumAssociatedFilesSELPROTO)(gPDSReadHFT[PDSElementGetNumAssociatedFilesSEL])))
    #define PDSElementGetAssociatedFile (ACROASSERT(gPDSReadVersion >=PDSRead_VERSION_13), *((PDSElementGetAssociatedFileSELPROTO)(gPDSReadHFT[PDSElementGetAssociatedFileSEL])))
    #define PDSElementHasAssociatedFile (ACROASSERT(gPDSReadVersion >=PDSRead_VERSION_13), *((PDSElementHasAssociatedFileSELPROTO)(gPDSReadHFT[PDSElementHasAssociatedFileSEL])))

/* If you add, delete, or modify procs listed in this file please read the instructions at the top about how to properly change the version number */

#endif /* PI_PDSEDIT_READ_VERSION != 0 */

#endif /* PLUGIN */
#endif /* PDSEDIT_IN_CORE || ACROBAT_LIBRARY */
#ifdef __cplusplus
}
#endif

#endif /* _H_PDSReadCalls */

