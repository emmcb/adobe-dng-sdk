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

 PDSWriteCalls.h

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

#ifndef _H_PDSWCalls
#define _H_PDSWCalls
#include "acroassert.h"

/* for Adobe use only */
#define _PDSWrite_LATEST_VERSION 0x000D0000
#define _PDSWrite_LAST_BETA_COMPATIBLE_VERSION 0x000D0000
#define _PDSWrite_IS_BETA 0

/* for public use */
#define PDSWriteHFT_LATEST_VERSION (_PDSWrite_IS_BETA ? (kHFT_IN_BETA_FLAG | _PDSWrite_LATEST_VERSION) : _PDSWrite_LATEST_VERSION)

#define PDSWrite_VERSION_B   0x00000006
#define PDSWrite_VERSION_5   0x00050000
#define PDSWrite_VERSION_6   0x00060000
#define PDSWrite_VERSION_7   0x00070000
#define PDSWrite_VERSION_8   0x00080000
#define PDSWrite_VERSION_13  PDSWriteHFT_LATEST_VERSION

#ifdef __cplusplus
extern "C" {
#endif

#include "PDSWriteHFTVers.h"
#include "PDSExpT.h"
#include "PEExpT.h"

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
#if HAS_PDSEDIT_WRITE_PROCS
	/* Implementation or static link */
	#define NPROC(returnType, name, params)		\
		ACEX1 returnType ACEX2 name params;
	#define XNPROC NPROC
	#define UNPROC NPROC
	#define NOPROC(name)
	#include "PDSWriteProcs.h"
	#undef NPROC
	#undef XNPROC
	#undef UNPROC
	#undef NOPROC

#endif /* HAS_PDSEDIT_WRITE_PROCS */
#else
#if PLUGIN
#include "PIRequir.h"
#ifdef THREAD_SAFE_PDFL
#include "PDFLInitCommon.h"
#endif /* defined THREAD_SAFE_PDFL */

	/* Enumerate the selectors */
	#define NPROC(returnType, name, params)			\
		name##SEL,
	#define XNPROC NPROC
	#define UNPROC NPROC
	#define NOPROC(name)	\
 		name##SEL,

	
	enum {
		PDSWriteBAD_SELECTOR,
		#include "PDSWriteProcs.h"
		PDSWriteNUMSELECTORSplusOne
	};
	
	#define PDSNUMSELECTORS (PDSNUMSELECTORSplusOne - 1)
	
	/* Create the prototypes */
	#undef NPROC
	#undef XNPROC
	#undef UNPROC
	#undef NOPROC

	#define NPROC(returnType, name, params)		\
		typedef ACCBPROTO1 returnType (ACCBPROTO2 *name##SELPROTO)params;
	#define NOPROC(name)
#if READER_PLUGIN
   /* Force an error for Exchange procs */
   #define XNPROC(returnType, name, params)
   #define UNPROC(returnType, name, params)
#else
   #define XNPROC NPROC
   #define UNPROC NPROC
#endif
		#include "PDSWriteProcs.h"
	#undef NPROC
	#undef XNPROC
	#undef UNPROC
	#undef NOPROC

#if PI_PDSEDIT_WRITE_VERSION != 0
	
#ifdef THREAD_SAFE_PDFL
	#define gPDSWriteHFT (GetHFTLocations()->pdsWriteHFT)
	#define gPDSWriteVersion (GetHFTLocations()->pdsWriteVersion)
#else
	extern HFT gPDSWriteHFT;
	extern ASUns32 gPDSWriteVersion;
#endif /* defined THREAD_SAFE_PDFL */

	/* Define the macros */
	#define PDDocCreateStructTreeRoot (ACROASSERT(gPDSWriteVersion >=PDSWrite_VERSION_B), *((PDDocCreateStructTreeRootSELPROTO)(gPDSWriteHFT[PDDocCreateStructTreeRootSEL])))
	#define PDDocRemoveStructTreeRoot (ACROASSERT(gPDSWriteVersion >=PDSWrite_VERSION_B), *((PDDocRemoveStructTreeRootSELPROTO)(gPDSWriteHFT[PDDocRemoveStructTreeRootSEL])))
    #define PDSTreeRootInsertKid (ACROASSERT(gPDSWriteVersion >=PDSWrite_VERSION_B), *((PDSTreeRootInsertKidSELPROTO)(gPDSWriteHFT[PDSTreeRootInsertKidSEL])))
    #define PDSTreeRootRemoveKid (ACROASSERT(gPDSWriteVersion >=PDSWrite_VERSION_B), *((PDSTreeRootRemoveKidSELPROTO)(gPDSWriteHFT[PDSTreeRootRemoveKidSEL])))
    #define PDSTreeRootReplaceKid (ACROASSERT(gPDSWriteVersion >=PDSWrite_VERSION_B), *((PDSTreeRootReplaceKidSELPROTO)(gPDSWriteHFT[PDSTreeRootReplaceKidSEL])))


	#define PDSTreeRootCreateRoleMap (ACROASSERT(gPDSWriteVersion >=PDSWrite_VERSION_B), *((PDSTreeRootCreateRoleMapSELPROTO)(gPDSWriteHFT[PDSTreeRootCreateRoleMapSEL])))
	#define PDSTreeRootRemoveRoleMap (ACROASSERT(gPDSWriteVersion >=PDSWrite_VERSION_B), *((PDSTreeRootRemoveRoleMapSELPROTO)(gPDSWriteHFT[PDSTreeRootRemoveRoleMapSEL])))

	#define PDSTreeRootCreateClassMap (ACROASSERT(gPDSWriteVersion >=PDSWrite_VERSION_B), *((PDSTreeRootCreateClassMapSELPROTO)(gPDSWriteHFT[PDSTreeRootCreateClassMapSEL])))
	#define PDSTreeRootRemoveClassMap (ACROASSERT(gPDSWriteVersion >=PDSWrite_VERSION_B), *((PDSTreeRootRemoveClassMapSELPROTO)(gPDSWriteHFT[PDSTreeRootRemoveClassMapSEL])))

	#define PDSElementCreate (ACROASSERT(gPDSWriteVersion >=PDSWrite_VERSION_B), *((PDSElementCreateSELPROTO)(gPDSWriteHFT[PDSElementCreateSEL]))) 
	#define PDSElementSetType (ACROASSERT(gPDSWriteVersion >=PDSWrite_VERSION_B), *((PDSElementSetTypeSELPROTO)(gPDSWriteHFT[PDSElementSetTypeSEL])))
	#define PDSElementSetTitle (ACROASSERT(gPDSWriteVersion >=PDSWrite_VERSION_B), *((PDSElementSetTitleSELPROTO)(gPDSWriteHFT[PDSElementSetTitleSEL])))
	#define PDSElementIncrementRevision (ACROASSERT(gPDSWriteVersion >=PDSWrite_VERSION_B), *((PDSElementIncrementRevisionSELPROTO)(gPDSWriteHFT[PDSElementIncrementRevisionSEL])))
	#define PDSElementAddAttrObj (ACROASSERT(gPDSWriteVersion >=PDSWrite_VERSION_B), *((PDSElementAddAttrObjSELPROTO)(gPDSWriteHFT[PDSElementAddAttrObjSEL])))
	#define PDSElementRemoveAttrObj (ACROASSERT(gPDSWriteVersion >=PDSWrite_VERSION_B), *((PDSElementRemoveAttrObjSELPROTO)(gPDSWriteHFT[PDSElementRemoveAttrObjSEL])))
	#define PDSElementRemoveAllAttrObjs (ACROASSERT(gPDSWriteVersion >=PDSWrite_VERSION_B), *((PDSElementRemoveAllAttrObjsSELPROTO)(gPDSWriteHFT[PDSElementRemoveAllAttrObjsSEL])))
	#define PDSElementAddClass (ACROASSERT(gPDSWriteVersion >=PDSWrite_VERSION_B), *((PDSElementAddClassSELPROTO)(gPDSWriteHFT[PDSElementAddClassSEL])))
	#define PDSElementRemoveClass (ACROASSERT(gPDSWriteVersion >=PDSWrite_VERSION_B), *((PDSElementRemoveClassSELPROTO)(gPDSWriteHFT[PDSElementRemoveClassSEL])))
	#define PDSElementRemoveAllClasses (ACROASSERT(gPDSWriteVersion >=PDSWrite_VERSION_B), *((PDSElementRemoveAllClassesSELPROTO)(gPDSWriteHFT[PDSElementRemoveAllClassesSEL])))
	#define PDSElementSetAlt (ACROASSERT(gPDSWriteVersion >=PDSWrite_VERSION_B), *((PDSElementSetAltSELPROTO)(gPDSWriteHFT[PDSElementSetAltSEL])))
	#define PDSElementInsertKid (ACROASSERT(gPDSWriteVersion >=PDSWrite_VERSION_B), *((PDSElementInsertKidSELPROTO)(gPDSWriteHFT[PDSElementInsertKidSEL])))
	#define PDSElementInsertMCAsKid (ACROASSERT(gPDSWriteVersion >=PDSWrite_VERSION_B), *((PDSElementInsertMCAsKidSELPROTO)(gPDSWriteHFT[PDSElementInsertMCAsKidSEL])))
    #define PDSElementInsertOBJAsKid (ACROASSERT(gPDSWriteVersion >=PDSWrite_VERSION_B), *((PDSElementInsertOBJAsKidSELPROTO)(gPDSWriteHFT[PDSElementInsertOBJAsKidSEL])))

	#define PDSElementRemoveKid (ACROASSERT(gPDSWriteVersion >=PDSWrite_VERSION_B), *((PDSElementRemoveKidSELPROTO)(gPDSWriteHFT[PDSElementRemoveKidSEL])))
	#define PDSElementRemoveKidMC (ACROASSERT(gPDSWriteVersion >=PDSWrite_VERSION_B), *((PDSElementRemoveKidMCSELPROTO)(gPDSWriteHFT[PDSElementRemoveKidMCSEL])))
	#define PDSElementReplaceKid (ACROASSERT(gPDSWriteVersion >=PDSWrite_VERSION_B), *((PDSElementReplaceKidSELPROTO)(gPDSWriteHFT[PDSElementReplaceKidSEL])))
	#define PDSElementReplaceKidMC (ACROASSERT(gPDSWriteVersion >=PDSWrite_VERSION_B), *((PDSElementReplaceKidMCSELPROTO)(gPDSWriteHFT[PDSElementReplaceKidMCSEL])))
    #define PDSElementReplaceKidOBJ (ACROASSERT(gPDSWriteVersion >=PDSWrite_VERSION_B), *((PDSElementReplaceKidOBJSELPROTO)(gPDSWriteHFT[PDSElementReplaceKidOBJSEL])))
    #define PDSElementSetID (ACROASSERT(gPDSWriteVersion >=PDSWrite_VERSION_B), *((PDSElementSetIDSELPROTO)(gPDSWriteHFT[PDSElementSetIDSEL])))
    #define PDSElementClearID (ACROASSERT(gPDSWriteVersion >=PDSWrite_VERSION_B), *((PDSElementClearIDSELPROTO)(gPDSWriteHFT[PDSElementClearIDSEL])))

	#define PDSAttrObjCreate (ACROASSERT(gPDSWriteVersion >=PDSWrite_VERSION_B), *((PDSAttrObjCreateSELPROTO)(gPDSWriteHFT[PDSAttrObjCreateSEL]))) 
	#define PDSAttrObjCreateFromStream (ACROASSERT(gPDSWriteVersion >=PDSWrite_VERSION_B), *((PDSAttrObjCreateFromStreamSELPROTO)(gPDSWriteHFT[PDSAttrObjCreateFromStreamSEL]))) 

	#define PDSRoleMapMap (ACROASSERT(gPDSWriteVersion >=PDSWrite_VERSION_B), *((PDSRoleMapMapSELPROTO)(gPDSWriteHFT[PDSRoleMapMapSEL])))
	#define PDSRoleMapUnMapSrc (ACROASSERT(gPDSWriteVersion >=PDSWrite_VERSION_B), *((PDSRoleMapUnMapSrcSELPROTO)(gPDSWriteHFT[PDSRoleMapUnMapSrcSEL])))
	#define PDSRoleMapUnMapDst (ACROASSERT(gPDSWriteVersion >=PDSWrite_VERSION_B), *((PDSRoleMapUnMapDstSELPROTO)(gPDSWriteHFT[PDSRoleMapUnMapDstSEL])))
	#define PDSRoleMapCopy (ACROASSERT(gPDSWriteVersion >=PDSWrite_VERSION_B), *((PDSRoleMapCopySELPROTO)(gPDSWriteHFT[PDSRoleMapCopySEL])))

	#define PDSClassMapAddAttrObj (ACROASSERT(gPDSWriteVersion >=PDSWrite_VERSION_B), *((PDSClassMapAddAttrObjSELPROTO)(gPDSWriteHFT[PDSClassMapAddAttrObjSEL])))
	#define PDSClassMapRemoveClass (ACROASSERT(gPDSWriteVersion >=PDSWrite_VERSION_B), *((PDSClassMapRemoveClassSELPROTO)(gPDSWriteHFT[PDSClassMapRemoveClassSEL])))
	#define PDSClassMapRemoveAttrObj (ACROASSERT(gPDSWriteVersion >=PDSWrite_VERSION_B), *((PDSClassMapRemoveAttrObjSELPROTO)(gPDSWriteHFT[PDSClassMapRemoveAttrObjSEL])))

/*
** Acrobat 5.0 additions
*/

	/* PI_PDSEDIT_WRITE_VERSION >= 0x00050000	*/

	#define PDSElementSetActualText (ACROASSERT(gPDSWriteVersion >=PDSWrite_VERSION_5), *((PDSElementSetActualTextSELPROTO)(gPDSWriteHFT[PDSElementSetActualTextSEL])))
	#define PDSElementSetLanguage (ACROASSERT(gPDSWriteVersion >=PDSWrite_VERSION_5), *((PDSElementSetLanguageSELPROTO)(gPDSWriteHFT[PDSElementSetLanguageSEL])))
    #define PDSElementRemoveKidOBJ (ACROASSERT(gPDSWriteVersion >=PDSWrite_VERSION_5), *((PDSElementRemoveKidOBJSELPROTO)(gPDSWriteHFT[PDSElementRemoveKidOBJSEL])))
    
/*
** Acrobat 6.0 additions
*/
    #define PDSElementInsertMCAsKidEx (ACROASSERT(gPDSWriteVersion >=PDSWrite_VERSION_6), *((PDSElementInsertMCAsKidExSELPROTO)(gPDSWriteHFT[PDSElementInsertMCAsKidExSEL])))
	#define PDSElementInsertStmMCAsKid (ACROASSERT(gPDSWriteVersion >=PDSWrite_VERSION_6), *((PDSElementInsertStmMCAsKidSELPROTO)(gPDSWriteHFT[PDSElementInsertStmMCAsKidSEL])))
	#define PDSTreeRootReplaceStreamRef (ACROASSERT(gPDSWriteVersion >=PDSWrite_VERSION_6), *((PDSTreeRootReplaceStreamRefSELPROTO)(gPDSWriteHFT[PDSTreeRootReplaceStreamRefSEL])))

/*
** Acrobat 7.0 additions
*/
    #define PDSMCRefCreate             (ACROASSERT(gPDSWriteVersion >=PDSWrite_VERSION_7), *((PDSMCRefCreateSELPROTO)            (gPDSWriteHFT[PDSMCRefCreateSEL])))
	#define PDSMCRefDestroy			   (ACROASSERT(gPDSWriteVersion >=PDSWrite_VERSION_7), *((PDSMCRefDestroySELPROTO)           (gPDSWriteHFT[PDSMCRefDestroySEL])))
	#define PDSElementInsertMCRefAsKid (ACROASSERT(gPDSWriteVersion >=PDSWrite_VERSION_7), *((PDSElementInsertMCRefAsKidSELPROTO)(gPDSWriteHFT[PDSElementInsertMCRefAsKidSEL])))

/*
** Acrobat 8.0 additions
*/
    #define PDSElementSetAltASText (ACROASSERT(gPDSWriteVersion >=PDSWrite_VERSION_8), *((PDSElementSetAltASTextSELPROTO)(gPDSWriteHFT[PDSElementSetAltASTextSEL])))
	#define PDSElementSetActualTextASText (ACROASSERT(gPDSWriteVersion >=PDSWrite_VERSION_8), *((PDSElementSetActualTextASTextSELPROTO)(gPDSWriteHFT[PDSElementSetActualTextASTextSEL])))
    #define PDSElementSetTitleASText (ACROASSERT(gPDSWriteVersion >=PDSWrite_VERSION_8), *((PDSElementSetTitleASTextSELPROTO)(gPDSWriteHFT[PDSElementSetTitleASTextSEL])))

/*
** Acrobat DC additions(support pdf 2.0)
*/
    #define PDSElementAddAssociatedFile (ACROASSERT(gPDSWriteVersion >=PDSWrite_VERSION_13), *((PDSElementAddAssociatedFileSELPROTO)(gPDSWriteHFT[PDSElementAddAssociatedFileSEL])))
    #define PDSElementRemoveAssociatedFile (ACROASSERT(gPDSWriteVersion >=PDSWrite_VERSION_13), *((PDSElementRemoveAssociatedFileSELPROTO)(gPDSWriteHFT[PDSElementRemoveAssociatedFileSEL])))
    #define PDSElementRemoveAllAssociatedFiles (ACROASSERT(gPDSWriteVersion >=PDSWrite_VERSION_13), *((PDSElementRemoveAllAssociatedFilesSELPROTO)(gPDSWriteHFT[PDSElementRemoveAllAssociatedFilesSEL])))

/* If you add, delete, or modify procs listed in this file please read the instructions at the top about how to properly change the version number */

#endif /* PI_PDSEDIT_WRITE_VERSION != 0 */

#endif /* PLUGIN */
#endif /* PDSEDIT_IN_CORE || ACROBAT_LIBRARY */

#ifdef __cplusplus
}
#endif

#endif /* _H_PDSWCalls */

