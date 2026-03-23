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

 PDCalls.h

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

#ifndef _H_PDCalls
#define _H_PDCalls
#include "acroassert.h"
#ifdef THREAD_SAFE_PDFL
#include "PDFLInitCommon.h"
#endif /* defined THREAD_SAFE_PDFL */


/* for Adobe use only */
#define _PDModelHFT_LATEST_VERSION 0x000D0000
#define _PDModelHFT_LAST_BETA_COMPATIBLE_VERSION 0x000C0000
#define _PDModelHFT_IS_BETA 1

/* for public use */
#define PDModelHFT_LATEST_VERSION (_PDModelHFT_IS_BETA ? (kHFT_IN_BETA_FLAG | _PDModelHFT_LATEST_VERSION) : _PDModelHFT_LATEST_VERSION)

#define PDModelHFT_VERSION_2   0x00020000
#define PDModelHFT_VERSION_2_1 0x00020001
#define PDModelHFT_VERSION_2_2 0x00020002
#define PDModelHFT_VERSION_2_3 0x00020003
#define PDModelHFT_VERSION_4   0x00040000
#define PDModelHFT_VERSION_4_5 0x00040005
#define PDModelHFT_VERSION_5   0x00050000
#define PDModelHFT_VERSION_6   0x00060000
#define PDModelHFT_VERSION_7   0x00070000
#define PDModelHFT_VERSION_7_5 0x00070005
#define PDModelHFT_VERSION_8   0x00080000
#define PDModelHFT_VERSION_9   0x00090000
#define PDModelHFT_VERSION_10  0x000A0000
#define PDModelHFT_VERSION_12  0x000C0000
#define PDModelHFT_VERSION_13  0x000D0000 /* DO NOT ALIAS TO PDModelHFT_LATEST_VERSION FROM VERSION 13 ONWARDS */

#ifdef __cplusplus
extern "C" {
#endif

#ifdef NPROC /* This might be defined in sys/procs.h */
#undef NPROC
#endif
	
#define EXTERNAL_PDPROCS_USER 1 /* External user of PDProcs.h header file */
	
#include "PDExpT.h"
#include "PDSExpT.h"

#if !PLUGIN
	/* Static link */
	#define NPROC(returnType, name, params)	\
		extern ACEX1 returnType ACEX2 name params;
	#define SPROC(returnType, name, params, stubProc) \
  		extern ACEX1 returnType ACEX2 stubProc params;
   #define XNPROC NPROC
   #define PROC	NPROC
   #define XPROC NPROC
#ifndef ENPROC
	#define ENPROC	NPROC		
#endif
	#define NOPROC(name)
	#define UPROC PROC
    #define UNPROC NPROC
    #define USPROC SPROC

	#include "PDProcsMini.h"

	#undef NPROC
	#undef XNPROC
	#undef SPROC
	#undef PROC
	#undef NOPROC	
	#undef XPROC
	#undef ENPROC
	#undef XSPROC
    #undef UPROC
    #undef UNPROC
    #undef USPROC

	/* These functions have a different name internally */
	#define PDDocGetWordFinder PDDocGetWordFinderHost
	#define PDDocCreateTextSelect PDDocCreateTextSelectHost
	#define PDTextSelectEnumQuads PDTextSelectEnumQuadsHost
	#define PDTextSelectEnumText PDTextSelectEnumTextHost
	#define PDTextSelectGetBoundingRect PDTextSelectGetBoundingRectHost
	#define PDTextSelectCreatePageHilite PDTextSelectCreatePageHiliteHost
	#define PDTextSelectCreateWordHilite PDTextSelectCreateWordHiliteHost
	#define PDTextSelectCreateRanges PDTextSelectCreateRangesHost
	#define PDFontGetDescendant PDFontGetDescendantInt

#endif /* !PLUGIN */

#if PLUGIN
	/* HFT version */
   #include "PIRequir.h"
	
	/* Enumerate the selectors */
	#define NPROC(returnType, name, params)			\
		name##SEL,
	#define SPROC(returnType, name, params, stubProc) \
		name##SEL,
   #define NOPROC(name)	\
			name##SEL,
	
   #define XNPROC NPROC
   #define PROC	NPROC
   #define XPROC NPROC
   #define ENPROC	NPROC
   #define UPROC NPROC
   #define UNPROC NPROC
   #define USPROC SPROC

	enum {
		PDModelBAD_SELECTOR,
		#include "PDProcsMini.h"
		PDModelNUMSELECTORSplusOne
	};
	
	#define PDModelNUMSELECTORS (PDModelNUMSELECTORSplusOne - 1)
	
	/* Create the prototypes */

	#undef NPROC
   #undef XNPROC
   #undef SPROC
   #undef PROC
   #undef XPROC
   #undef NOPROC
	#undef ENPROC
    #undef UPROC
    #undef UNPROC
    #undef USPROC

	
	#define NPROC(returnType, name, params)		\
		typedef ACCBPROTO1 returnType (ACCBPROTO2 *name##SELPROTO)params;
	#define SPROC(returnType, name, params, stubProc) \
		typedef ACCBPROTO1 returnType (ACCBPROTO2 *name##SELPROTO)params;

   #define PROC	NPROC
   #define ENPROC NPROC
#if READER_PLUGIN
   /* Force Error for Exchange procs */
   #define XPROC(returnType, name, params)
   #define XNPROC(returnType, name, params)
	#define UPROC(returnType, name, params) 
    #define UNPROC(returnType, name, params) 
    #define USPROC(returnType, name, params, stubProc) 
#else
   #define XPROC NPROC
   #define XNPROC NPROC
   #define UPROC NPROC
   #define UNPROC NPROC
   #define USPROC SPROC

#endif
	#define NOPROC(name)	
	#include "PDProcsMini.h"
	#undef NPROC
   #undef XNPROC
   #undef SPROC
   #undef PROC
   #undef NOPROC	
   #undef XPROC		
	#undef ENPROC
    #undef UPROC
    #undef UNPROC
    #undef USPROC

	
#if PI_PDMODEL_VERSION != 0

#ifdef THREAD_SAFE_PDFL
	#define gPDModelHFT (GetHFTLocations()->pdModelHFT)
	#define gPDModelVersion (GetHFTLocations()->pdModelVersion)
#else
	extern HFT gPDModelHFT;
	extern ASUns32 gPDModelVersion;
#endif

#if !STATIC_HFT
/* PDActionNew */
#define PDActionNew (ACROASSERT(gPDModelVersion >=PDModelHFT_VERSION_2), *((PDActionNewSELPROTO)(gPDModelHFT[PDActionNewSEL])))

/* PDActionNewFromDest */
#define PDActionNewFromDest (ACROASSERT(gPDModelVersion >=PDModelHFT_VERSION_2), *((PDActionNewFromDestSELPROTO)(gPDModelHFT[PDActionNewFromDestSEL])))

/* PDActionNewFromURI */
#define PDActionNewFromURI (ACROASSERT(gPDModelVersion >=PDModelHFT_VERSION_2), *((PDActionNewFromURISELPROTO)(gPDModelHFT[PDActionNewFromURISEL])))

/* PDActionNewFromFileSpec */
#define PDActionNewFromFileSpec (ACROASSERT(gPDModelVersion >=PDModelHFT_VERSION_2), *((PDActionNewFromFileSpecSELPROTO)(gPDModelHFT[PDActionNewFromFileSpecSEL])))

/* PDActionDestroy */
#define PDActionDestroy (ACROASSERT(gPDModelVersion >=PDModelHFT_VERSION_2), *((PDActionDestroySELPROTO)(gPDModelHFT[PDActionDestroySEL])))

/* PDActionIsValid */
#define PDActionIsValid (ACROASSERT(gPDModelVersion >=PDModelHFT_VERSION_2), *((PDActionIsValidSELPROTO)(gPDModelHFT[PDActionIsValidSEL])))

/* PDActionGetSubtype */
#define PDActionGetSubtype (ACROASSERT(gPDModelVersion >=PDModelHFT_VERSION_2), *((PDActionGetSubtypeSELPROTO)(gPDModelHFT[PDActionGetSubtypeSEL])))

/* PDActionEqual */
#define PDActionEqual (ACROASSERT(gPDModelVersion >=PDModelHFT_VERSION_2), *((PDActionEqualSELPROTO)(gPDModelHFT[PDActionEqualSEL])))

/* PDActionGetDest */
#define PDActionGetDest (ACROASSERT(gPDModelVersion >=PDModelHFT_VERSION_2), *((PDActionGetDestSELPROTO)(gPDModelHFT[PDActionGetDestSEL])))

/* PDActionGetCosObj */
#define PDActionGetCosObj (ACROASSERT(gPDModelVersion >=PDModelHFT_VERSION_2), *((PDActionGetCosObjSELPROTO)(gPDModelHFT[PDActionGetCosObjSEL])))

/* PDActionFromCosObj */
#define PDActionFromCosObj (ACROASSERT(gPDModelVersion >=PDModelHFT_VERSION_2), *((PDActionFromCosObjSELPROTO)(gPDModelHFT[PDActionFromCosObjSEL])))

/* PDActionGetFileSpec */
#define PDActionGetFileSpec (ACROASSERT(gPDModelVersion >=PDModelHFT_VERSION_2), *((PDActionGetFileSpecSELPROTO)(gPDModelHFT[PDActionGetFileSpecSEL])))

/* PDAnnotNotifyWillChange */
#define PDAnnotNotifyWillChange (ACROASSERT(gPDModelVersion >=PDModelHFT_VERSION_2), *((PDAnnotNotifyWillChangeSELPROTO)(gPDModelHFT[PDAnnotNotifyWillChangeSEL])))

/* PDAnnotNotifyDidChange */
#define PDAnnotNotifyDidChange (ACROASSERT(gPDModelVersion >=PDModelHFT_VERSION_2), *((PDAnnotNotifyDidChangeSELPROTO)(gPDModelHFT[PDAnnotNotifyDidChangeSEL])))

/* PDPageCreateAnnot */
#define PDPageCreateAnnot (ACROASSERT(gPDModelVersion >=PDModelHFT_VERSION_2), *((PDPageCreateAnnotSELPROTO)(gPDModelHFT[PDPageCreateAnnotSEL])))

/* PDAnnotIsValid */
#define PDAnnotIsValid (ACROASSERT(gPDModelVersion >=PDModelHFT_VERSION_2), *((PDAnnotIsValidSELPROTO)(gPDModelHFT[PDAnnotIsValidSEL])))

/* PDAnnotGetSubtype */
#define PDAnnotGetSubtype (ACROASSERT(gPDModelVersion >=PDModelHFT_VERSION_2), *((PDAnnotGetSubtypeSELPROTO)(gPDModelHFT[PDAnnotGetSubtypeSEL])))

/* PDAnnotGetRect */
#define PDAnnotGetRect (ACROASSERT(gPDModelVersion >=PDModelHFT_VERSION_2), *((PDAnnotGetRectSELPROTO)(gPDModelHFT[PDAnnotGetRectSEL])))

/* PDAnnotSetRect */
#define PDAnnotSetRect (ACROASSERT(gPDModelVersion >=PDModelHFT_VERSION_2), *((PDAnnotSetRectSELPROTO)(gPDModelHFT[PDAnnotSetRectSEL])))

/* PDAnnotEqual */
#define PDAnnotEqual (ACROASSERT(gPDModelVersion >=PDModelHFT_VERSION_2), *((PDAnnotEqualSELPROTO)(gPDModelHFT[PDAnnotEqualSEL])))

/* PDAnnotGetColor */
#define PDAnnotGetColor (ACROASSERT(gPDModelVersion >=PDModelHFT_VERSION_2), *((PDAnnotGetColorSELPROTO)(gPDModelHFT[PDAnnotGetColorSEL])))

/* PDAnnotSetColor */
#define PDAnnotSetColor (ACROASSERT(gPDModelVersion >=PDModelHFT_VERSION_2), *((PDAnnotSetColorSELPROTO)(gPDModelHFT[PDAnnotSetColorSEL])))

/* PDAnnotGetTitle */
#define PDAnnotGetTitle (ACROASSERT(gPDModelVersion >=PDModelHFT_VERSION_2), *((PDAnnotGetTitleSELPROTO)(gPDModelHFT[PDAnnotGetTitleSEL])))

/* PDAnnotSetTitle */
#define PDAnnotSetTitle (ACROASSERT(gPDModelVersion >=PDModelHFT_VERSION_2), *((PDAnnotSetTitleSELPROTO)(gPDModelHFT[PDAnnotSetTitleSEL])))

/* PDAnnotGetDate */
#define PDAnnotGetDate (ACROASSERT(gPDModelVersion >=PDModelHFT_VERSION_2), *((PDAnnotGetDateSELPROTO)(gPDModelHFT[PDAnnotGetDateSEL])))

/* PDAnnotSetDate */
#define PDAnnotSetDate (ACROASSERT(gPDModelVersion >=PDModelHFT_VERSION_2), *((PDAnnotSetDateSELPROTO)(gPDModelHFT[PDAnnotSetDateSEL])))

/* PDAnnotGetCosObj */
#define PDAnnotGetCosObj (ACROASSERT(gPDModelVersion >=PDModelHFT_VERSION_2), *((PDAnnotGetCosObjSELPROTO)(gPDModelHFT[PDAnnotGetCosObjSEL])))

/* PDAnnotFromCosObj */
#define PDAnnotFromCosObj (ACROASSERT(gPDModelVersion >=PDModelHFT_VERSION_2), *((PDAnnotFromCosObjSELPROTO)(gPDModelHFT[PDAnnotFromCosObjSEL])))

/* PDTextAnnotGetContents */
#define PDTextAnnotGetContents (ACROASSERT(gPDModelVersion >=PDModelHFT_VERSION_2), *((PDTextAnnotGetContentsSELPROTO)(gPDModelHFT[PDTextAnnotGetContentsSEL])))

/* PDTextAnnotSetContents */
#define PDTextAnnotSetContents (ACROASSERT(gPDModelVersion >=PDModelHFT_VERSION_2), *((PDTextAnnotSetContentsSELPROTO)(gPDModelHFT[PDTextAnnotSetContentsSEL])))

/* PDTextAnnotIsOpen */
#define PDTextAnnotIsOpen (ACROASSERT(gPDModelVersion >=PDModelHFT_VERSION_2), *((PDTextAnnotIsOpenSELPROTO)(gPDModelHFT[PDTextAnnotIsOpenSEL])))

/* PDTextAnnotSetOpen */
#define PDTextAnnotSetOpen (ACROASSERT(gPDModelVersion >=PDModelHFT_VERSION_2), *((PDTextAnnotSetOpenSELPROTO)(gPDModelHFT[PDTextAnnotSetOpenSEL])))

/* PDLinkAnnotGetBorder */
#define PDLinkAnnotGetBorder (ACROASSERT(gPDModelVersion >=PDModelHFT_VERSION_2), *((PDLinkAnnotGetBorderSELPROTO)(gPDModelHFT[PDLinkAnnotGetBorderSEL])))

/* PDLinkAnnotSetBorder */
#define PDLinkAnnotSetBorder (ACROASSERT(gPDModelVersion >=PDModelHFT_VERSION_2), *((PDLinkAnnotSetBorderSELPROTO)(gPDModelHFT[PDLinkAnnotSetBorderSEL])))

/* PDLinkAnnotSetAction */
#define PDLinkAnnotSetAction (ACROASSERT(gPDModelVersion >=PDModelHFT_VERSION_2), *((PDLinkAnnotSetActionSELPROTO)(gPDModelHFT[PDLinkAnnotSetActionSEL])))

/* PDLinkAnnotGetAction */
#define PDLinkAnnotGetAction (ACROASSERT(gPDModelVersion >=PDModelHFT_VERSION_2), *((PDLinkAnnotGetActionSELPROTO)(gPDModelHFT[PDLinkAnnotGetActionSEL])))

/* PDAnnotGetFlags */
#define PDAnnotGetFlags (ACROASSERT(gPDModelVersion >=PDModelHFT_VERSION_2), *((PDAnnotGetFlagsSELPROTO)(gPDModelHFT[PDAnnotGetFlagsSEL])))

/* PDAnnotSetFlags */
#define PDAnnotSetFlags (ACROASSERT(gPDModelVersion >=PDModelHFT_VERSION_2), *((PDAnnotSetFlagsSELPROTO)(gPDModelHFT[PDAnnotSetFlagsSEL])))

/* PDBookmarkAddNewSibling */
#define PDBookmarkAddNewSibling (ACROASSERT(gPDModelVersion >=PDModelHFT_VERSION_2), *((PDBookmarkAddNewSiblingSELPROTO)(gPDModelHFT[PDBookmarkAddNewSiblingSEL])))

/* PDBookmarkAddNewChild */
#define PDBookmarkAddNewChild (ACROASSERT(gPDModelVersion >=PDModelHFT_VERSION_2), *((PDBookmarkAddNewChildSELPROTO)(gPDModelHFT[PDBookmarkAddNewChildSEL])))

/* PDBookmarkAddSubtree */
#define PDBookmarkAddSubtree (ACROASSERT(gPDModelVersion >=PDModelHFT_VERSION_2), *((PDBookmarkAddSubtreeSELPROTO)(gPDModelHFT[PDBookmarkAddSubtreeSEL])))

/* PDBookmarkDestroy */
#define PDBookmarkDestroy (ACROASSERT(gPDModelVersion >=PDModelHFT_VERSION_2), *((PDBookmarkDestroySELPROTO)(gPDModelHFT[PDBookmarkDestroySEL])))

/* PDBookmarkGetByTitle */
#define PDBookmarkGetByTitle (ACROASSERT(gPDModelVersion >=PDModelHFT_VERSION_2), *((PDBookmarkGetByTitleSELPROTO)(gPDModelHFT[PDBookmarkGetByTitleSEL])))

/* PDBookmarkGetCount */
#define PDBookmarkGetCount (ACROASSERT(gPDModelVersion >=PDModelHFT_VERSION_2), *((PDBookmarkGetCountSELPROTO)(gPDModelHFT[PDBookmarkGetCountSEL])))

/* PDBookmarkAddPrev */
#define PDBookmarkAddPrev (ACROASSERT(gPDModelVersion >=PDModelHFT_VERSION_2), *((PDBookmarkAddPrevSELPROTO)(gPDModelHFT[PDBookmarkAddPrevSEL])))

/* PDBookmarkAddNext */
#define PDBookmarkAddNext (ACROASSERT(gPDModelVersion >=PDModelHFT_VERSION_2), *((PDBookmarkAddNextSELPROTO)(gPDModelHFT[PDBookmarkAddNextSEL])))

/* PDBookmarkAddChild */
#define PDBookmarkAddChild (ACROASSERT(gPDModelVersion >=PDModelHFT_VERSION_2), *((PDBookmarkAddChildSELPROTO)(gPDModelHFT[PDBookmarkAddChildSEL])))

/* PDBookmarkUnlink */
#define PDBookmarkUnlink (ACROASSERT(gPDModelVersion >=PDModelHFT_VERSION_2), *((PDBookmarkUnlinkSELPROTO)(gPDModelHFT[PDBookmarkUnlinkSEL])))

/* PDBookmarkIsValid */
#define PDBookmarkIsValid (ACROASSERT(gPDModelVersion >=PDModelHFT_VERSION_2), *((PDBookmarkIsValidSELPROTO)(gPDModelHFT[PDBookmarkIsValidSEL])))

/* PDBookmarkGetParent */
#define PDBookmarkGetParent (ACROASSERT(gPDModelVersion >=PDModelHFT_VERSION_2), *((PDBookmarkGetParentSELPROTO)(gPDModelHFT[PDBookmarkGetParentSEL])))

/* PDBookmarkGetFirstChild */
#define PDBookmarkGetFirstChild (ACROASSERT(gPDModelVersion >=PDModelHFT_VERSION_2), *((PDBookmarkGetFirstChildSELPROTO)(gPDModelHFT[PDBookmarkGetFirstChildSEL])))

/* PDBookmarkGetLastChild */
#define PDBookmarkGetLastChild (ACROASSERT(gPDModelVersion >=PDModelHFT_VERSION_2), *((PDBookmarkGetLastChildSELPROTO)(gPDModelHFT[PDBookmarkGetLastChildSEL])))

/* PDBookmarkGetNext */
#define PDBookmarkGetNext (ACROASSERT(gPDModelVersion >=PDModelHFT_VERSION_2), *((PDBookmarkGetNextSELPROTO)(gPDModelHFT[PDBookmarkGetNextSEL])))

/* PDBookmarkGetPrev */
#define PDBookmarkGetPrev (ACROASSERT(gPDModelVersion >=PDModelHFT_VERSION_2), *((PDBookmarkGetPrevSELPROTO)(gPDModelHFT[PDBookmarkGetPrevSEL])))

/* PDBookmarkGetIndent */
#define PDBookmarkGetIndent (ACROASSERT(gPDModelVersion >=PDModelHFT_VERSION_2), *((PDBookmarkGetIndentSELPROTO)(gPDModelHFT[PDBookmarkGetIndentSEL])))

/* PDBookmarkGetTitle */
#define PDBookmarkGetTitle (ACROASSERT(gPDModelVersion >=PDModelHFT_VERSION_2), *((PDBookmarkGetTitleSELPROTO)(gPDModelHFT[PDBookmarkGetTitleSEL])))

/* PDBookmarkSetTitle */
#define PDBookmarkSetTitle (ACROASSERT(gPDModelVersion >=PDModelHFT_VERSION_2), *((PDBookmarkSetTitleSELPROTO)(gPDModelHFT[PDBookmarkSetTitleSEL])))

/* PDBookmarkHasChildren */
#define PDBookmarkHasChildren (ACROASSERT(gPDModelVersion >=PDModelHFT_VERSION_2), *((PDBookmarkHasChildrenSELPROTO)(gPDModelHFT[PDBookmarkHasChildrenSEL])))

/* PDBookmarkIsOpen */
#define PDBookmarkIsOpen (ACROASSERT(gPDModelVersion >=PDModelHFT_VERSION_2), *((PDBookmarkIsOpenSELPROTO)(gPDModelHFT[PDBookmarkIsOpenSEL])))

/* PDBookmarkSetOpen */
#define PDBookmarkSetOpen (ACROASSERT(gPDModelVersion >=PDModelHFT_VERSION_2), *((PDBookmarkSetOpenSELPROTO)(gPDModelHFT[PDBookmarkSetOpenSEL])))

/* PDBookmarkGetAction */
#define PDBookmarkGetAction (ACROASSERT(gPDModelVersion >=PDModelHFT_VERSION_2), *((PDBookmarkGetActionSELPROTO)(gPDModelHFT[PDBookmarkGetActionSEL])))

/* PDBookmarkSetAction */
#define PDBookmarkSetAction (ACROASSERT(gPDModelVersion >=PDModelHFT_VERSION_2), *((PDBookmarkSetActionSELPROTO)(gPDModelHFT[PDBookmarkSetActionSEL])))

/* PDBookmarkEqual */
#define PDBookmarkEqual (ACROASSERT(gPDModelVersion >=PDModelHFT_VERSION_2), *((PDBookmarkEqualSELPROTO)(gPDModelHFT[PDBookmarkEqualSEL])))

/* PDBookmarkGetCosObj */
#define PDBookmarkGetCosObj (ACROASSERT(gPDModelVersion >=PDModelHFT_VERSION_2), *((PDBookmarkGetCosObjSELPROTO)(gPDModelHFT[PDBookmarkGetCosObjSEL])))

/* PDBookmarkFromCosObj */
#define PDBookmarkFromCosObj (ACROASSERT(gPDModelVersion >=PDModelHFT_VERSION_2), *((PDBookmarkFromCosObjSELPROTO)(gPDModelHFT[PDBookmarkFromCosObjSEL])))

/* PDEnumDocs */
#define PDEnumDocs (ACROASSERT(gPDModelVersion >=PDModelHFT_VERSION_2), *((PDEnumDocsSELPROTO)(gPDModelHFT[PDEnumDocsSEL])))

/* PDDocOpen */
#define PDDocOpen (ACROASSERT(gPDModelVersion >=PDModelHFT_VERSION_2), *((PDDocOpenSELPROTO)(gPDModelHFT[PDDocOpenSEL])))

/* PDDocGetOpenAction */
#define PDDocGetOpenAction (ACROASSERT(gPDModelVersion >=PDModelHFT_VERSION_2), *((PDDocGetOpenActionSELPROTO)(gPDModelHFT[PDDocGetOpenActionSEL])))

/* PDDocSetOpenAction */
#define PDDocSetOpenAction (ACROASSERT(gPDModelVersion >=PDModelHFT_VERSION_2), *((PDDocSetOpenActionSELPROTO)(gPDModelHFT[PDDocSetOpenActionSEL])))

/* PDDocCreate */
#define PDDocCreate (ACROASSERT(gPDModelVersion >=PDModelHFT_VERSION_2), *((PDDocCreateSELPROTO)(gPDModelHFT[PDDocCreateSEL])))

/* PDDocSave */
#define PDDocSave (ACROASSERT(gPDModelVersion >=PDModelHFT_VERSION_2), *((PDDocSaveSELPROTO)(gPDModelHFT[PDDocSaveSEL])))

/* PDDocClose */
#define PDDocClose (ACROASSERT(gPDModelVersion >=PDModelHFT_VERSION_2), *((PDDocCloseSELPROTO)(gPDModelHFT[PDDocCloseSEL])))

/* PDDocAcquire */
#if UNIX_PLATFORM
#ifdef __cplusplus
extern "C" {
#endif
#endif
#define PDDocAcquire (ACROASSERT(gPDModelVersion >=PDModelHFT_VERSION_2), *((PDDocAcquireSELPROTO)(gPDModelHFT[PDDocAcquireSEL])))
#if UNIX_PLATFORM
#ifdef __cplusplus
}
#endif
#endif


/* PDDocRelease */
#define PDDocRelease (ACROASSERT(gPDModelVersion >=PDModelHFT_VERSION_2), *((PDDocReleaseSELPROTO)(gPDModelHFT[PDDocReleaseSEL])))

/* PDDocGetFlags */
#define PDDocGetFlags (ACROASSERT(gPDModelVersion >=PDModelHFT_VERSION_2), *((PDDocGetFlagsSELPROTO)(gPDModelHFT[PDDocGetFlagsSEL])))

/* PDDocSetFlags */
#define PDDocSetFlags (ACROASSERT(gPDModelVersion >=PDModelHFT_VERSION_2), *((PDDocSetFlagsSELPROTO)(gPDModelHFT[PDDocSetFlagsSEL])))

/* PDDocGetPageMode */
#define PDDocGetPageMode (ACROASSERT(gPDModelVersion >=PDModelHFT_VERSION_2), *((PDDocGetPageModeSELPROTO)(gPDModelHFT[PDDocGetPageModeSEL])))

/* PDDocSetPageMode */
#define PDDocSetPageMode (ACROASSERT(gPDModelVersion >=PDModelHFT_VERSION_2), *((PDDocSetPageModeSELPROTO)(gPDModelHFT[PDDocSetPageModeSEL])))

/* PDDocGetCosDoc */
#define PDDocGetCosDoc (ACROASSERT(gPDModelVersion >=PDModelHFT_VERSION_2), *((PDDocGetCosDocSELPROTO)(gPDModelHFT[PDDocGetCosDocSEL])))

/* PDDocGetFile */
#define PDDocGetFile (ACROASSERT(gPDModelVersion >=PDModelHFT_VERSION_2), *((PDDocGetFileSELPROTO)(gPDModelHFT[PDDocGetFileSEL])))

/* PDDocGetID */
#define PDDocGetID (ACROASSERT(gPDModelVersion >=PDModelHFT_VERSION_2), *((PDDocGetIDSELPROTO)(gPDModelHFT[PDDocGetIDSEL])))

/* PDDocGetVersion */
#define PDDocGetVersion (ACROASSERT(gPDModelVersion >=PDModelHFT_VERSION_2), *((PDDocGetVersionSELPROTO)(gPDModelHFT[PDDocGetVersionSEL])))

/* PDDocGetBookmarkRoot */
#define PDDocGetBookmarkRoot (ACROASSERT(gPDModelVersion >=PDModelHFT_VERSION_2), *((PDDocGetBookmarkRootSELPROTO)(gPDModelHFT[PDDocGetBookmarkRootSEL])))

/* PDDocGetNumPages */
#define PDDocGetNumPages (ACROASSERT(gPDModelVersion >=PDModelHFT_VERSION_2), *((PDDocGetNumPagesSELPROTO)(gPDModelHFT[PDDocGetNumPagesSEL])))

/* PDDocAcquirePage */
#define PDDocAcquirePage (ACROASSERT(gPDModelVersion >=PDModelHFT_VERSION_2), *((PDDocAcquirePageSELPROTO)(gPDModelHFT[PDDocAcquirePageSEL])))

/* PDDocCreatePage */
#define PDDocCreatePage (ACROASSERT(gPDModelVersion >=PDModelHFT_VERSION_2), *((PDDocCreatePageSELPROTO)(gPDModelHFT[PDDocCreatePageSEL])))

/* PDDocDeletePages */
#define PDDocDeletePages (ACROASSERT(gPDModelVersion >=PDModelHFT_VERSION_2), *((PDDocDeletePagesSELPROTO)(gPDModelHFT[PDDocDeletePagesSEL])))

/* PDDocMovePage */
#define PDDocMovePage (ACROASSERT(gPDModelVersion >=PDModelHFT_VERSION_2), *((PDDocMovePageSELPROTO)(gPDModelHFT[PDDocMovePageSEL])))

/* PDDocInsertPages */
#define PDDocInsertPages (ACROASSERT(gPDModelVersion >=PDModelHFT_VERSION_2), *((PDDocInsertPagesSELPROTO)(gPDModelHFT[PDDocInsertPagesSEL])))

/* PDDocReplacePages */
#define PDDocReplacePages (ACROASSERT(gPDModelVersion >=PDModelHFT_VERSION_2), *((PDDocReplacePagesSELPROTO)(gPDModelHFT[PDDocReplacePagesSEL])))

/* PDDocGetNumThreads */
#define PDDocGetNumThreads (ACROASSERT(gPDModelVersion >=PDModelHFT_VERSION_2), *((PDDocGetNumThreadsSELPROTO)(gPDModelHFT[PDDocGetNumThreadsSEL])))

/* PDDocGetThread */
#define PDDocGetThread (ACROASSERT(gPDModelVersion >=PDModelHFT_VERSION_2), *((PDDocGetThreadSELPROTO)(gPDModelHFT[PDDocGetThreadSEL])))

/* PDDocGetThreadIndex */
#define PDDocGetThreadIndex (ACROASSERT(gPDModelVersion >=PDModelHFT_VERSION_2), *((PDDocGetThreadIndexSELPROTO)(gPDModelHFT[PDDocGetThreadIndexSEL])))

/* PDDocAddThread */
#define PDDocAddThread (ACROASSERT(gPDModelVersion >=PDModelHFT_VERSION_2), *((PDDocAddThreadSELPROTO)(gPDModelHFT[PDDocAddThreadSEL])))

/* PDDocRemoveThread */
#define PDDocRemoveThread (ACROASSERT(gPDModelVersion >=PDModelHFT_VERSION_2), *((PDDocRemoveThreadSELPROTO)(gPDModelHFT[PDDocRemoveThreadSEL])))

/* PDDocEnumFonts */
#define PDDocEnumFonts (ACROASSERT(gPDModelVersion >=PDModelHFT_VERSION_2), *((PDDocEnumFontsSELPROTO)(gPDModelHFT[PDDocEnumFontsSEL])))

/* PDDocEnumLoadedFonts */
#define PDDocEnumLoadedFonts (ACROASSERT(gPDModelVersion >=PDModelHFT_VERSION_2), *((PDDocEnumLoadedFontsSELPROTO)(gPDModelHFT[PDDocEnumLoadedFontsSEL])))

/* PDDocCreateThumbs */
#define PDDocCreateThumbs (ACROASSERT(gPDModelVersion >=PDModelHFT_VERSION_2), *((PDDocCreateThumbsSELPROTO)(gPDModelHFT[PDDocCreateThumbsSEL])))

/* PDDocDeleteThumbs */
#define PDDocDeleteThumbs (ACROASSERT(gPDModelVersion >=PDModelHFT_VERSION_2), *((PDDocDeleteThumbsSELPROTO)(gPDModelHFT[PDDocDeleteThumbsSEL])))

/* PDDocGetWordFinder */
#define PDDocGetWordFinder (ACROASSERT(gPDModelVersion >=PDModelHFT_VERSION_2), *((PDDocGetWordFinderSELPROTO)(gPDModelHFT[PDDocGetWordFinderSEL])))

/** PDDocCreateWordFinder 
	@ref GlyphNames
*/
#define PDDocCreateWordFinder (ACROASSERT(gPDModelVersion >=PDModelHFT_VERSION_2), *((PDDocCreateWordFinderSELPROTO)(gPDModelHFT[PDDocCreateWordFinderSEL])))

/* PDWordFinderGetNthWord */
#define PDWordFinderGetNthWord (ACROASSERT(gPDModelVersion >=PDModelHFT_VERSION_2), *((PDWordFinderGetNthWordSELPROTO)(gPDModelHFT[PDWordFinderGetNthWordSEL])))

/** PDWordSplitString 
	@ingroup GlyphNames
*/
#define PDWordSplitString (ACROASSERT(gPDModelVersion >=PDModelHFT_VERSION_2), *((PDWordSplitStringSELPROTO)(gPDModelHFT[PDWordSplitStringSEL])))

/* PDDocCreateTextSelect */
#define PDDocCreateTextSelect (ACROASSERT(gPDModelVersion >=PDModelHFT_VERSION_2), *((PDDocCreateTextSelectSELPROTO)(gPDModelHFT[ PDDocCreateTextSelectSEL])))

/* PDDocGetInfo */
#define PDDocGetInfo (ACROASSERT(gPDModelVersion >=PDModelHFT_VERSION_2), *((PDDocGetInfoSELPROTO)(gPDModelHFT[PDDocGetInfoSEL])))

/* PDDocSetInfo */
#define PDDocSetInfo (ACROASSERT(gPDModelVersion >=PDModelHFT_VERSION_2), *((PDDocSetInfoSELPROTO)(gPDModelHFT[PDDocSetInfoSEL])))

#ifndef ANDROID_ENV
/* PDDocGetSecurityData */
#define PDDocGetSecurityData (ACROASSERT(gPDModelVersion >=PDModelHFT_VERSION_2), *((PDDocGetSecurityDataSELPROTO)(gPDModelHFT[PDDocGetSecurityDataSEL])))

/* PDDocGetNewSecurityData */
#define PDDocGetNewSecurityData (ACROASSERT(gPDModelVersion >=PDModelHFT_VERSION_2), *((PDDocGetNewSecurityDataSELPROTO)(gPDModelHFT[PDDocGetNewSecurityDataSEL])))

/* PDDocAuthorize  */
#define PDDocAuthorize (ACROASSERT(gPDModelVersion >=PDModelHFT_VERSION_2), *((PDDocAuthorizeSELPROTO)(gPDModelHFT[PDDocAuthorizeSEL])))

/* PDDocNewSecurityData */
#define PDDocNewSecurityData (ACROASSERT(gPDModelVersion >=PDModelHFT_VERSION_2), *((PDDocNewSecurityDataSELPROTO)(gPDModelHFT[PDDocNewSecurityDataSEL])))

/* PDDocSetNewSecurityData  */
#define PDDocSetNewSecurityData (ACROASSERT(gPDModelVersion >=PDModelHFT_VERSION_2), *((PDDocSetNewSecurityDataSELPROTO)(gPDModelHFT[PDDocSetNewSecurityDataSEL])))

/* PDDocSetNewCryptHandler */
#define PDDocSetNewCryptHandler (ACROASSERT(gPDModelVersion >=PDModelHFT_VERSION_2), *((PDDocSetNewCryptHandlerSELPROTO)(gPDModelHFT[PDDocSetNewCryptHandlerSEL])))

/* PDDocSetNewCryptHandlerEx */
#define PDDocSetNewCryptHandlerEx (ACROASSERT(gPDModelVersion >=PDModelHFT_VERSION_6), *((PDDocSetNewCryptHandlerExSELPROTO)(gPDModelHFT[PDDocSetNewCryptHandlerExSEL])))

/* PDDocGetNewCryptHandler */
#define PDDocGetNewCryptHandler (ACROASSERT(gPDModelVersion >=PDModelHFT_VERSION_2), *((PDDocGetNewCryptHandlerSELPROTO)(gPDModelHFT[PDDocGetNewCryptHandlerSEL])))

/* PDDocGetNewSecurityInfo  */
#define PDDocGetNewSecurityInfo (ACROASSERT(gPDModelVersion >=PDModelHFT_VERSION_2), *((PDDocGetNewSecurityInfoSELPROTO)(gPDModelHFT[PDDocGetNewSecurityInfoSEL])))


/* PDDocGetPermissions */
#define PDDocGetPermissions (ACROASSERT(gPDModelVersion >=PDModelHFT_VERSION_2), *((PDDocGetPermissionsSELPROTO)(gPDModelHFT[PDDocGetPermissionsSEL])))

/* PDRegisterCryptHandler */
#define PDRegisterCryptHandler (ACROASSERT(gPDModelVersion >=PDModelHFT_VERSION_2), *((PDRegisterCryptHandlerSELPROTO)(gPDModelHFT[PDRegisterCryptHandlerSEL])))
#endif

/* PDXlateToPDFDocEnc */
#define PDXlateToPDFDocEnc (ACROASSERT(gPDModelVersion >=PDModelHFT_VERSION_2), *((PDXlateToPDFDocEncSELPROTO)(gPDModelHFT[PDXlateToPDFDocEncSEL])))

/* PDXlateToHost */
#define PDXlateToHost (ACROASSERT(gPDModelVersion >=PDModelHFT_VERSION_2), *((PDXlateToHostSELPROTO)(gPDModelHFT[PDXlateToHostSEL])))


/* PDFontGetName */
#define PDFontGetName (ACROASSERT(gPDModelVersion >=PDModelHFT_VERSION_2), *((PDFontGetNameSELPROTO)(gPDModelHFT[PDFontGetNameSEL])))

/** PDFontGetSubtype 
	@ref FontSubtypes
*/
#define PDFontGetSubtype (ACROASSERT(gPDModelVersion >=PDModelHFT_VERSION_2), *((PDFontGetSubtypeSELPROTO)(gPDModelHFT[PDFontGetSubtypeSEL])))

/* PDFontGetCharSet */
#define PDFontGetCharSet (ACROASSERT(gPDModelVersion >=PDModelHFT_VERSION_2), *((PDFontGetCharSetSELPROTO)(gPDModelHFT[PDFontGetCharSetSEL])))

/* PDFontGetEncodingIndex */
#define PDFontGetEncodingIndex (ACROASSERT(gPDModelVersion >=PDModelHFT_VERSION_2), *((PDFontGetEncodingIndexSELPROTO)(gPDModelHFT[PDFontGetEncodingIndexSEL])))

/* PDFontAcquireEncodingArray */
#define PDFontAcquireEncodingArray (ACROASSERT(gPDModelVersion >=PDModelHFT_VERSION_2), *((PDFontAcquireEncodingArraySELPROTO)(gPDModelHFT[PDFontAcquireEncodingArraySEL])))

/* PDFontEncodingArrayRelease */
#define PDFontEncodingArrayRelease (ACROASSERT(gPDModelVersion >=PDModelHFT_VERSION_2), *((PDFontEncodingArrayReleaseSELPROTO)(gPDModelHFT[PDFontEncodingArrayReleaseSEL])))

/* PDFontGetMetrics */
#define PDFontGetMetrics (ACROASSERT(gPDModelVersion >=PDModelHFT_VERSION_2), *((PDFontGetMetricsSELPROTO)(gPDModelHFT[PDFontGetMetricsSEL])))

/* PDFontGetBBox */
#define PDFontGetBBox (ACROASSERT(gPDModelVersion >=PDModelHFT_VERSION_2), *((PDFontGetBBoxSELPROTO)(gPDModelHFT[PDFontGetBBoxSEL])))

/* PDFontGetWidths */
#define PDFontGetWidths (ACROASSERT(gPDModelVersion >=PDModelHFT_VERSION_2), *((PDFontGetWidthsSELPROTO)(gPDModelHFT[PDFontGetWidthsSEL])))

/* PDGetPDFDocEncoding */
#define PDGetPDFDocEncoding (ACROASSERT(gPDModelVersion >=PDModelHFT_VERSION_2), *((PDGetPDFDocEncodingSELPROTO)(gPDModelHFT[PDGetPDFDocEncodingSEL])))

/* PDFontIsEmbedded */
#define PDFontIsEmbedded (ACROASSERT(gPDModelVersion >=PDModelHFT_VERSION_2), *((PDFontIsEmbeddedSELPROTO)(gPDModelHFT[PDFontIsEmbeddedSEL])))

/* PDFontXlateWidths */
#define PDFontXlateWidths (ACROASSERT(gPDModelVersion >=PDModelHFT_VERSION_2), *((PDFontXlateWidthsSELPROTO)(gPDModelHFT[PDFontXlateWidthsSEL])))

/* PDFontXlateString */
#define PDFontXlateString (ACROASSERT(gPDModelVersion >=PDModelHFT_VERSION_2), *((PDFontXlateStringSELPROTO)(gPDModelHFT[PDFontXlateStringSEL])))

/* PDFontAcquireXlateTable */
#define PDFontAcquireXlateTable (ACROASSERT(gPDModelVersion >=PDModelHFT_VERSION_2), *((PDFontAcquireXlateTableSELPROTO)(gPDModelHFT[PDFontAcquireXlateTableSEL])))

/* PDFontXlateTableRelease */
#define PDFontXlateTableRelease (ACROASSERT(gPDModelVersion >=PDModelHFT_VERSION_2), *((PDFontXlateTableReleaseSELPROTO)(gPDModelHFT[PDFontXlateTableReleaseSEL])))

/* PDFontGetFontMatrix */
#define PDFontGetFontMatrix (ACROASSERT(gPDModelVersion >=PDModelHFT_VERSION_2), *((PDFontGetFontMatrixSELPROTO)(gPDModelHFT[PDFontGetFontMatrixSEL])))

/* PDFontSetMetrics */
#define PDFontSetMetrics (ACROASSERT(gPDModelVersion >=PDModelHFT_VERSION_2), *((PDFontSetMetricsSELPROTO)(gPDModelHFT[PDFontSetMetricsSEL])))

/* PDFontGetCosObj */
#define PDFontGetCosObj (ACROASSERT(gPDModelVersion >=PDModelHFT_VERSION_2), *((PDFontGetCosObjSELPROTO)(gPDModelHFT[PDFontGetCosObjSEL])))

/* PDPageNotifyContentsDidChange */
#define PDPageNotifyContentsDidChange (ACROASSERT(gPDModelVersion >=PDModelHFT_VERSION_2), *((PDPageNotifyContentsDidChangeSELPROTO)(gPDModelHFT[PDPageNotifyContentsDidChangeSEL])))

/* PDPageGetNumber */
#define PDPageGetNumber (ACROASSERT(gPDModelVersion >=PDModelHFT_VERSION_2), *((PDPageGetNumberSELPROTO)(gPDModelHFT[PDPageGetNumberSEL])))

/* PDPageRelease */
#define PDPageRelease (ACROASSERT(gPDModelVersion >=PDModelHFT_VERSION_2), *((PDPageReleaseSELPROTO)(gPDModelHFT[PDPageReleaseSEL])))

/* PDPageGetDoc */
#define PDPageGetDoc (ACROASSERT(gPDModelVersion >=PDModelHFT_VERSION_2), *((PDPageGetDocSELPROTO)(gPDModelHFT[PDPageGetDocSEL])))

/* PDPageGetCosObj */
#define PDPageGetCosObj (ACROASSERT(gPDModelVersion >=PDModelHFT_VERSION_2), *((PDPageGetCosObjSELPROTO)(gPDModelHFT[PDPageGetCosObjSEL])))

/* PDPageNumFromCosObj */
#define PDPageNumFromCosObj (ACROASSERT(gPDModelVersion >=PDModelHFT_VERSION_2), *((PDPageNumFromCosObjSELPROTO)(gPDModelHFT[PDPageNumFromCosObjSEL])))

/* PDPageGetRotate */
#define PDPageGetRotate (ACROASSERT(gPDModelVersion >=PDModelHFT_VERSION_2), *((PDPageGetRotateSELPROTO)(gPDModelHFT[PDPageGetRotateSEL])))

/* PDPageSetRotate */
#define PDPageSetRotate (ACROASSERT(gPDModelVersion >=PDModelHFT_VERSION_2), *((PDPageSetRotateSELPROTO)(gPDModelHFT[PDPageSetRotateSEL])))

/* PDPageGetMediaBox */
#define PDPageGetMediaBox (ACROASSERT(gPDModelVersion >=PDModelHFT_VERSION_2), *((PDPageGetMediaBoxSELPROTO)(gPDModelHFT[PDPageGetMediaBoxSEL])))

/* PDPageSetMediaBox */
#define PDPageSetMediaBox (ACROASSERT(gPDModelVersion >=PDModelHFT_VERSION_2), *((PDPageSetMediaBoxSELPROTO)(gPDModelHFT[PDPageSetMediaBoxSEL])))

/* PDPageGetCropBox */
#define PDPageGetCropBox (ACROASSERT(gPDModelVersion >=PDModelHFT_VERSION_2), *((PDPageGetCropBoxSELPROTO)(gPDModelHFT[PDPageGetCropBoxSEL])))

/* PDPageSetCropBox */
#define PDPageSetCropBox (ACROASSERT(gPDModelVersion >=PDModelHFT_VERSION_2), *((PDPageSetCropBoxSELPROTO)(gPDModelHFT[PDPageSetCropBoxSEL])))

/* PDPageGetBBox */
#define PDPageGetBBox (ACROASSERT(gPDModelVersion >=PDModelHFT_VERSION_2), *((PDPageGetBBoxSELPROTO)(gPDModelHFT[PDPageGetBBoxSEL])))

/* PDPageGetDefaultMatrix */
#define PDPageGetDefaultMatrix (ACROASSERT(gPDModelVersion >=PDModelHFT_VERSION_2), *((PDPageGetDefaultMatrixSELPROTO)(gPDModelHFT[PDPageGetDefaultMatrixSEL])))

/* PDPageGetFlippedMatrix */
#define PDPageGetFlippedMatrix (ACROASSERT(gPDModelVersion >=PDModelHFT_VERSION_2), *((PDPageGetFlippedMatrixSELPROTO)(gPDModelHFT[PDPageGetFlippedMatrixSEL])))

#if !MAC_PLATFORM || (MAC_PLATFORM && !AS_ARCH_64BIT)
/* PDPageDrawContentsToWindow */
#define PDPageDrawContentsToWindow (ACROASSERT(gPDModelVersion >=PDModelHFT_VERSION_2), *((PDPageDrawContentsToWindowSELPROTO)(gPDModelHFT[PDPageDrawContentsToWindowSEL])))
#endif /* #if !MAC_PLATFORM || (MAC_PLATFORM && !AS_ARCH_64BIT) */

/* PDPageGetAnnot */
#define PDPageGetAnnot (ACROASSERT(gPDModelVersion >=PDModelHFT_VERSION_2), *((PDPageGetAnnotSELPROTO)(gPDModelHFT[PDPageGetAnnotSEL])))

/* PDPageAddNewAnnot */
#define PDPageAddNewAnnot (ACROASSERT(gPDModelVersion >=PDModelHFT_VERSION_2), *((PDPageAddNewAnnotSELPROTO)(gPDModelHFT[PDPageAddNewAnnotSEL])))

/* PDPageAddAnnot */
#define PDPageAddAnnot (ACROASSERT(gPDModelVersion >=PDModelHFT_VERSION_2), *((PDPageAddAnnotSELPROTO)(gPDModelHFT[PDPageAddAnnotSEL])))

/* PDPageRemoveAnnot */
#define PDPageRemoveAnnot (ACROASSERT(gPDModelVersion >=PDModelHFT_VERSION_2), *((PDPageRemoveAnnotSELPROTO)(gPDModelHFT[PDPageRemoveAnnotSEL])))

/* PDPageGetAnnotIndex */
#define PDPageGetAnnotIndex (ACROASSERT(gPDModelVersion >=PDModelHFT_VERSION_2), *((PDPageGetAnnotIndexSELPROTO)(gPDModelHFT[PDPageGetAnnotIndexSEL])))

/* PDPageGetNumAnnots */
#define PDPageGetNumAnnots (ACROASSERT(gPDModelVersion >=PDModelHFT_VERSION_2), *((PDPageGetNumAnnotsSELPROTO)(gPDModelHFT[PDPageGetNumAnnotsSEL])))

/* PDPageGetCosResources */
#define PDPageGetCosResources (ACROASSERT(gPDModelVersion >=PDModelHFT_VERSION_2), *((PDPageGetCosResourcesSELPROTO)(gPDModelHFT[PDPageGetCosResourcesSEL])))

/* PDPageAddCosResource */
#define PDPageAddCosResource (ACROASSERT(gPDModelVersion >=PDModelHFT_VERSION_2), *((PDPageAddCosResourceSELPROTO)(gPDModelHFT[PDPageAddCosResourceSEL])))

/* PDPageAddCosContents */
#define PDPageAddCosContents (ACROASSERT(gPDModelVersion >=PDModelHFT_VERSION_2), *((PDPageAddCosContentsSELPROTO)(gPDModelHFT[PDPageAddCosContentsSEL])))

/* PDPageRemoveCosResource */
#define PDPageRemoveCosResource (ACROASSERT(gPDModelVersion >=PDModelHFT_VERSION_2), *((PDPageRemoveCosResourceSELPROTO)(gPDModelHFT[PDPageRemoveCosResourceSEL])))

/* PDPageRemoveCosContents */
#define PDPageRemoveCosContents (ACROASSERT(gPDModelVersion >=PDModelHFT_VERSION_2), *((PDPageRemoveCosContentsSELPROTO)(gPDModelHFT[PDPageRemoveCosContentsSEL])))

/* PDGraphicGetBBox */
#define PDGraphicGetBBox (ACROASSERT(gPDModelVersion >=PDModelHFT_VERSION_2), *((PDGraphicGetBBoxSELPROTO)(gPDModelHFT[PDGraphicGetBBoxSEL])))

/* PDGraphicGetCurrentMatrix */
#define PDGraphicGetCurrentMatrix (ACROASSERT(gPDModelVersion >=PDModelHFT_VERSION_2), *((PDGraphicGetCurrentMatrixSELPROTO)(gPDModelHFT[PDGraphicGetCurrentMatrixSEL])))

/* PDGraphicGetState */
#define PDGraphicGetState (ACROASSERT(gPDModelVersion >=PDModelHFT_VERSION_2), *((PDGraphicGetStateSELPROTO)(gPDModelHFT[PDGraphicGetStateSEL])))

/** PDTextEnum 
	@ingroup Enumerators
*/
#define PDTextEnum (ACROASSERT(gPDModelVersion >=PDModelHFT_VERSION_2), *((PDTextEnumSELPROTO)(gPDModelHFT[PDTextEnumSEL])))

/* PDTextGetState */
#define PDTextGetState (ACROASSERT(gPDModelVersion >=PDModelHFT_VERSION_2), *((PDTextGetStateSELPROTO)(gPDModelHFT[PDTextGetStateSEL])))

/* PDPathEnum */
#define PDPathEnum (ACROASSERT(gPDModelVersion >=PDModelHFT_VERSION_2), *((PDPathEnumSELPROTO)(gPDModelHFT[PDPathEnumSEL])))

/* PDPathGetPaintOp */
#define PDPathGetPaintOp (ACROASSERT(gPDModelVersion >=PDModelHFT_VERSION_2), *((PDPathGetPaintOpSELPROTO)(gPDModelHFT[PDPathGetPaintOpSEL])))

/* PDInlineImageGetAttrs */
#define PDInlineImageGetAttrs (ACROASSERT(gPDModelVersion >=PDModelHFT_VERSION_2), *((PDInlineImageGetAttrsSELPROTO)(gPDModelHFT[PDInlineImageGetAttrsSEL])))

/* PDInlineImageGetData */
#define PDInlineImageGetData (ACROASSERT(gPDModelVersion >=PDModelHFT_VERSION_2), *((PDInlineImageGetDataSELPROTO)(gPDModelHFT[PDInlineImageGetDataSEL])))

/* PDInlineImageColorSpaceGetIndexLookup */
#define PDInlineImageColorSpaceGetIndexLookup (ACROASSERT(gPDModelVersion >=PDModelHFT_VERSION_2), *((PDInlineImageColorSpaceGetIndexLookupSELPROTO)(gPDModelHFT[PDInlineImageColorSpaceGetIndexLookupSEL])))

/* PDXObjectGetSubtype */
#define PDXObjectGetSubtype (ACROASSERT(gPDModelVersion >=PDModelHFT_VERSION_2), *((PDXObjectGetSubtypeSELPROTO)(gPDModelHFT[PDXObjectGetSubtypeSEL])))

/* PDXObjectGetCosObj */
#define PDXObjectGetCosObj (ACROASSERT(gPDModelVersion >=PDModelHFT_VERSION_2), *((PDXObjectGetCosObjSELPROTO)(gPDModelHFT[PDXObjectGetCosObjSEL])))

/* PDXObjectGetDataLength */
#define PDXObjectGetDataLength (ACROASSERT(gPDModelVersion >=PDModelHFT_VERSION_2), *((PDXObjectGetDataLengthSELPROTO)(gPDModelHFT[PDXObjectGetDataLengthSEL])))

/** PDXObjectGetData 
	@ingroup Enumerators
*/
#define PDXObjectGetData (ACROASSERT(gPDModelVersion >=PDModelHFT_VERSION_2), *((PDXObjectGetDataSELPROTO)(gPDModelHFT[PDXObjectGetDataSEL])))

/** PDXObjectEnumFilters 
	@ingroup Enumerators
*/
#define PDXObjectEnumFilters (ACROASSERT(gPDModelVersion >=PDModelHFT_VERSION_2), *((PDXObjectEnumFiltersSELPROTO)(gPDModelHFT[PDXObjectEnumFiltersSEL])))

/* PDImageGetAttrs */
#define PDImageGetAttrs (ACROASSERT(gPDModelVersion >=PDModelHFT_VERSION_2), *((PDImageGetAttrsSELPROTO)(gPDModelHFT[PDImageGetAttrsSEL])))

/* PDImageColorSpaceGetIndexLookup */
#define PDImageColorSpaceGetIndexLookup (ACROASSERT(gPDModelVersion >=PDModelHFT_VERSION_2), *((PDImageColorSpaceGetIndexLookupSELPROTO)(gPDModelHFT[PDImageColorSpaceGetIndexLookupSEL])))

/* PDFormGetFormType */
#define PDFormGetFormType (ACROASSERT(gPDModelVersion >=PDModelHFT_VERSION_2), *((PDFormGetFormTypeSELPROTO)(gPDModelHFT[PDFormGetFormTypeSEL])))

/* PDFormGetBBox */
#define PDFormGetBBox (ACROASSERT(gPDModelVersion >=PDModelHFT_VERSION_2), *((PDFormGetBBoxSELPROTO)(gPDModelHFT[PDFormGetBBoxSEL])))

/* PDFormGetMatrix */
#define PDFormGetMatrix (ACROASSERT(gPDModelVersion >=PDModelHFT_VERSION_2), *((PDFormGetMatrixSELPROTO)(gPDModelHFT[ PDFormGetMatrixSEL ])))

/* PDFormGetXUIDCosObj */
#define PDFormGetXUIDCosObj (ACROASSERT(gPDModelVersion >=PDModelHFT_VERSION_2), *((PDFormGetXUIDCosObjSELPROTO)(gPDModelHFT[PDFormGetXUIDCosObjSEL])))

/* PDFormEnumResources */
#define PDFormEnumResources (ACROASSERT(gPDModelVersion >=PDModelHFT_VERSION_2), *((PDFormEnumResourcesSELPROTO)(gPDModelHFT[PDFormEnumResourcesSEL])))

/* PDFormEnumPaintProc */
#define PDFormEnumPaintProc (ACROASSERT(gPDModelVersion >=PDModelHFT_VERSION_2), *((PDFormEnumPaintProcSELPROTO)(gPDModelHFT[PDFormEnumPaintProcSEL])))

/* PDFontEnumCharProcs */
#define PDFontEnumCharProcs (ACROASSERT(gPDModelVersion >=PDModelHFT_VERSION_2), *((PDFontEnumCharProcsSELPROTO)(gPDModelHFT[PDFontEnumCharProcsSEL])))

/* PDCharProcEnum */
#define PDCharProcEnum (ACROASSERT(gPDModelVersion >=PDModelHFT_VERSION_2), *((PDCharProcEnumSELPROTO)(gPDModelHFT[PDCharProcEnumSEL])))

/* PDCharProcGetCosObj */
#define PDCharProcGetCosObj (ACROASSERT(gPDModelVersion >=PDModelHFT_VERSION_2), *((PDCharProcGetCosObjSELPROTO)(gPDModelHFT[PDCharProcGetCosObjSEL])))

/* PDThreadNew */
#define PDThreadNew (ACROASSERT(gPDModelVersion >=PDModelHFT_VERSION_2), *((PDThreadNewSELPROTO)(gPDModelHFT[PDThreadNewSEL])))

/* PDThreadDestroy */
#define PDThreadDestroy (ACROASSERT(gPDModelVersion >=PDModelHFT_VERSION_2), *((PDThreadDestroySELPROTO)(gPDModelHFT[PDThreadDestroySEL])))

/* PDThreadGetFirstBead */
#define PDThreadGetFirstBead (ACROASSERT(gPDModelVersion >=PDModelHFT_VERSION_2), *((PDThreadGetFirstBeadSELPROTO)(gPDModelHFT[PDThreadGetFirstBeadSEL])))

/* PDThreadSetFirstBead */
#define PDThreadSetFirstBead (ACROASSERT(gPDModelVersion >=PDModelHFT_VERSION_2), *((PDThreadSetFirstBeadSELPROTO)(gPDModelHFT[PDThreadSetFirstBeadSEL])))

/* PDThreadGetInfo */
#define PDThreadGetInfo (ACROASSERT(gPDModelVersion >=PDModelHFT_VERSION_2), *((PDThreadGetInfoSELPROTO)(gPDModelHFT[PDThreadGetInfoSEL])))

/* PDThreadSetInfo */
#define PDThreadSetInfo (ACROASSERT(gPDModelVersion >=PDModelHFT_VERSION_2), *((PDThreadSetInfoSELPROTO)(gPDModelHFT[PDThreadSetInfoSEL])))

/* PDThreadIsValid */
#define PDThreadIsValid (ACROASSERT(gPDModelVersion >=PDModelHFT_VERSION_2), *((PDThreadIsValidSELPROTO)(gPDModelHFT[PDThreadIsValidSEL])))

/* PDThreadGetCosObj */
#define PDThreadGetCosObj (ACROASSERT(gPDModelVersion >=PDModelHFT_VERSION_2), *((PDThreadGetCosObjSELPROTO)(gPDModelHFT[PDThreadGetCosObjSEL])))

/* PDThreadFromCosObj */
#define PDThreadFromCosObj (ACROASSERT(gPDModelVersion >=PDModelHFT_VERSION_2), *((PDThreadFromCosObjSELPROTO)(gPDModelHFT[PDThreadFromCosObjSEL])))

/* PDBeadNew */
#define PDBeadNew (ACROASSERT(gPDModelVersion >=PDModelHFT_VERSION_2), *((PDBeadNewSELPROTO)(gPDModelHFT[PDBeadNewSEL])))

/* PDBeadDestroy */
#define PDBeadDestroy (ACROASSERT(gPDModelVersion >=PDModelHFT_VERSION_2), *((PDBeadDestroySELPROTO)(gPDModelHFT[PDBeadDestroySEL])))

/* PDBeadGetNext */
#define PDBeadGetNext (ACROASSERT(gPDModelVersion >=PDModelHFT_VERSION_2), *((PDBeadGetNextSELPROTO)(gPDModelHFT[PDBeadGetNextSEL])))

/* PDBeadGetPrev */
#define PDBeadGetPrev (ACROASSERT(gPDModelVersion >=PDModelHFT_VERSION_2), *((PDBeadGetPrevSELPROTO)(gPDModelHFT[PDBeadGetPrevSEL])))

/* PDBeadInsert */
#define PDBeadInsert (ACROASSERT(gPDModelVersion >=PDModelHFT_VERSION_2), *((PDBeadInsertSELPROTO)(gPDModelHFT[PDBeadInsertSEL])))

/* PDBeadAcquirePage */
#define PDBeadAcquirePage (ACROASSERT(gPDModelVersion >=PDModelHFT_VERSION_2), *((PDBeadAcquirePageSELPROTO)(gPDModelHFT[PDBeadAcquirePageSEL])))

/* PDBeadSetPage */
#define PDBeadSetPage (ACROASSERT(gPDModelVersion >=PDModelHFT_VERSION_2), *((PDBeadSetPageSELPROTO)(gPDModelHFT[PDBeadSetPageSEL])))

/* PDBeadGetRect */
#define PDBeadGetRect (ACROASSERT(gPDModelVersion >=PDModelHFT_VERSION_2), *((PDBeadGetRectSELPROTO)(gPDModelHFT[PDBeadGetRectSEL])))
 
/* PDBeadSetRect */
#define PDBeadSetRect (ACROASSERT(gPDModelVersion >=PDModelHFT_VERSION_2), *((PDBeadSetRectSELPROTO)(gPDModelHFT[ PDBeadSetRectSEL])))

/* PDBeadIsValid */
#define PDBeadIsValid (ACROASSERT(gPDModelVersion >=PDModelHFT_VERSION_2), *((PDBeadIsValidSELPROTO)(gPDModelHFT[PDBeadIsValidSEL])))

/* PDBeadGetThread */
#define PDBeadGetThread (ACROASSERT(gPDModelVersion >=PDModelHFT_VERSION_2), *((PDBeadGetThreadSELPROTO)(gPDModelHFT[PDBeadGetThreadSEL])))

/* PDBeadGetIndex */
#define PDBeadGetIndex (ACROASSERT(gPDModelVersion >=PDModelHFT_VERSION_2), *((PDBeadGetIndexSELPROTO)(gPDModelHFT[PDBeadGetIndexSEL])))

/* PDBeadEqual */
#define PDBeadEqual (ACROASSERT(gPDModelVersion >=PDModelHFT_VERSION_2), *((PDBeadEqualSELPROTO)(gPDModelHFT[PDBeadEqualSEL])))

/* PDBeadGetCosObj */
#define PDBeadGetCosObj (ACROASSERT(gPDModelVersion >=PDModelHFT_VERSION_2), *((PDBeadGetCosObjSELPROTO)(gPDModelHFT[PDBeadGetCosObjSEL])))

/* PDBeadFromCosObj */
#define PDBeadFromCosObj (ACROASSERT(gPDModelVersion >=PDModelHFT_VERSION_2), *((PDBeadFromCosObjSELPROTO)(gPDModelHFT[PDBeadFromCosObjSEL])))

/* PDViewDestCreate */
#define PDViewDestCreate (ACROASSERT(gPDModelVersion >=PDModelHFT_VERSION_2), *((PDViewDestCreateSELPROTO)(gPDModelHFT[PDViewDestCreateSEL])))

/* PDViewDestDestroy */
#define PDViewDestDestroy (ACROASSERT(gPDModelVersion >=PDModelHFT_VERSION_2), *((PDViewDestDestroySELPROTO)(gPDModelHFT[PDViewDestDestroySEL])))

/* PDViewDestIsValid */
#define PDViewDestIsValid (ACROASSERT(gPDModelVersion >=PDModelHFT_VERSION_2), *((PDViewDestIsValidSELPROTO)(gPDModelHFT[PDViewDestIsValidSEL])))

/* PDViewDestGetAttr */
#define PDViewDestGetAttr (ACROASSERT(gPDModelVersion >=PDModelHFT_VERSION_2), *((PDViewDestGetAttrSELPROTO)(gPDModelHFT[PDViewDestGetAttrSEL])))

/* PDViewDestGetCosObj */
#define PDViewDestGetCosObj (ACROASSERT(gPDModelVersion >=PDModelHFT_VERSION_2), *((PDViewDestGetCosObjSELPROTO)(gPDModelHFT[PDViewDestGetCosObjSEL])))

/* PDViewDestFromCosObj */
#define PDViewDestFromCosObj (ACROASSERT(gPDModelVersion >=PDModelHFT_VERSION_2), *((PDViewDestFromCosObjSELPROTO)(gPDModelHFT[PDViewDestFromCosObjSEL])))

/* PDTextSelectDestroy */
#define PDTextSelectDestroy (ACROASSERT(gPDModelVersion >=PDModelHFT_VERSION_2), *((PDTextSelectDestroySELPROTO)(gPDModelHFT[PDTextSelectDestroySEL])))

/** PDTextSelectEnumQuads 
	@ingroup Enumerators
*/
#define PDTextSelectEnumQuads (ACROASSERT(gPDModelVersion >=PDModelHFT_VERSION_2), *((PDTextSelectEnumQuadsSELPROTO)(gPDModelHFT[PDTextSelectEnumQuadsSEL ])))

/** PDTextSelectEnumText 
	@ingroup Enumerators
*/
#define PDTextSelectEnumText (ACROASSERT(gPDModelVersion >=PDModelHFT_VERSION_2), *((PDTextSelectEnumTextSELPROTO)(gPDModelHFT[PDTextSelectEnumTextSEL])))

/* PDTextSelectGetPage */
#define PDTextSelectGetPage (ACROASSERT(gPDModelVersion >=PDModelHFT_VERSION_2), *((PDTextSelectGetPageSELPROTO)(gPDModelHFT[PDTextSelectGetPageSEL])))

/* PDTextSelectGetBoundingRect */
#define PDTextSelectGetBoundingRect (ACROASSERT(gPDModelVersion >=PDModelHFT_VERSION_2), *((PDTextSelectGetBoundingRectSELPROTO)(gPDModelHFT[PDTextSelectGetBoundingRectSEL])))

/* PDTextSelectCreatePageHilite */
#define PDTextSelectCreatePageHilite (ACROASSERT(gPDModelVersion >=PDModelHFT_VERSION_2), *((PDTextSelectCreatePageHiliteSELPROTO)(gPDModelHFT[PDTextSelectCreatePageHiliteSEL])))

/* PDTextSelectCreateWordHilite */
#define PDTextSelectCreateWordHilite (ACROASSERT(gPDModelVersion >=PDModelHFT_VERSION_2), *((PDTextSelectCreateWordHiliteSELPROTO)(gPDModelHFT[PDTextSelectCreateWordHiliteSEL])))

/* PDTextSelectGetRange */
#define PDTextSelectGetRange (ACROASSERT(gPDModelVersion >=PDModelHFT_VERSION_2), *((PDTextSelectGetRangeSELPROTO)(gPDModelHFT[PDTextSelectGetRangeSEL])))

/* PDTextSelectGetRangeCount */
#define PDTextSelectGetRangeCount (ACROASSERT(gPDModelVersion >=PDModelHFT_VERSION_2), *((PDTextSelectGetRangeCountSELPROTO)(gPDModelHFT[PDTextSelectGetRangeCountSEL])))

/* PDTextSelectCreateRanges */
#define PDTextSelectCreateRanges (ACROASSERT(gPDModelVersion >=PDModelHFT_VERSION_2), *((PDTextSelectCreateRangesSELPROTO)(gPDModelHFT[PDTextSelectCreateRangesSEL])))

/* PDWordFinderAcquireWordList */
#define PDWordFinderAcquireWordList (ACROASSERT(gPDModelVersion >=PDModelHFT_VERSION_2), *((PDWordFinderAcquireWordListSELPROTO)(gPDModelHFT[PDWordFinderAcquireWordListSEL])))

/* PDWordFinderGetLatestAlgVersion */
#define PDWordFinderGetLatestAlgVersion (ACROASSERT(gPDModelVersion >=PDModelHFT_VERSION_2), *((PDWordFinderGetLatestAlgVersionSELPROTO)(gPDModelHFT[PDWordFinderGetLatestAlgVersionSEL])))

/* PDWordFinderReleaseWordList */
#define PDWordFinderReleaseWordList (ACROASSERT(gPDModelVersion >=PDModelHFT_VERSION_2), *((PDWordFinderReleaseWordListSELPROTO)(gPDModelHFT[PDWordFinderReleaseWordListSEL])))

/* PDWordFinderDestroy */
#define PDWordFinderDestroy (ACROASSERT(gPDModelVersion >=PDModelHFT_VERSION_2), *((PDWordFinderDestroySELPROTO)(gPDModelHFT[PDWordFinderDestroySEL])))

/* PDWordFinderEnumWords */
#define PDWordFinderEnumWords (ACROASSERT(gPDModelVersion >=PDModelHFT_VERSION_2), *((PDWordFinderEnumWordsSELPROTO)(gPDModelHFT[PDWordFinderEnumWordsSEL])))

/* PDWordGetLength */
#define PDWordGetLength (ACROASSERT(gPDModelVersion >=PDModelHFT_VERSION_2), *((PDWordGetLengthSELPROTO)(gPDModelHFT[PDWordGetLengthSEL])))

/* PDWordGetString */
#define PDWordGetString (ACROASSERT(gPDModelVersion >=PDModelHFT_VERSION_2), *((PDWordGetStringSELPROTO)(gPDModelHFT[PDWordGetStringSEL])))

/* PDWordGetAttr */
#define PDWordGetAttr (ACROASSERT(gPDModelVersion >=PDModelHFT_VERSION_2), *((PDWordGetAttrSELPROTO)(gPDModelHFT[PDWordGetAttrSEL])))

/* PDWordGetCharacterTypes */
#define PDWordGetCharacterTypes (ACROASSERT(gPDModelVersion >=PDModelHFT_VERSION_2), *((PDWordGetCharacterTypesSELPROTO)(gPDModelHFT[PDWordGetCharacterTypesSEL])))

/* PDWordGetCharOffset */
#define PDWordGetCharOffset (ACROASSERT(gPDModelVersion >=PDModelHFT_VERSION_2), *((PDWordGetCharOffsetSELPROTO)(gPDModelHFT[ PDWordGetCharOffsetSEL])))

/* PDWordGetCharDelta */
#define PDWordGetCharDelta (ACROASSERT(gPDModelVersion >=PDModelHFT_VERSION_2), *((PDWordGetCharDeltaSELPROTO)(gPDModelHFT[PDWordGetCharDeltaSEL])))

/* PDWordGetStyleTransition */
#define PDWordGetStyleTransition (ACROASSERT(gPDModelVersion >=PDModelHFT_VERSION_2), *((PDWordGetStyleTransitionSELPROTO)(gPDModelHFT[PDWordGetStyleTransitionSEL])))

/* PDWordGetNthCharStyle */
#define PDWordGetNthCharStyle (ACROASSERT(gPDModelVersion >=PDModelHFT_VERSION_2), *((PDWordGetNthCharStyleSELPROTO)(gPDModelHFT[PDWordGetNthCharStyleSEL])))

/* PDWordGetNumQuads */
#define PDWordGetNumQuads (ACROASSERT(gPDModelVersion >=PDModelHFT_VERSION_2), *((PDWordGetNumQuadsSELPROTO)(gPDModelHFT[PDWordGetNumQuadsSEL])))

/* PDWordGetNthQuad */
#define PDWordGetNthQuad (ACROASSERT(gPDModelVersion >=PDModelHFT_VERSION_2), *((PDWordGetNthQuadSELPROTO)(gPDModelHFT[PDWordGetNthQuadSEL])))

/* PDWordIsRotated */
#define PDWordIsRotated (ACROASSERT(gPDModelVersion >=PDModelHFT_VERSION_2), *((PDWordIsRotatedSELPROTO)(gPDModelHFT[PDWordIsRotatedSEL])))

/* PDWordFilterString */
#define PDWordFilterString (ACROASSERT(gPDModelVersion >=PDModelHFT_VERSION_2), *((PDWordFilterStringSELPROTO)(gPDModelHFT[PDWordFilterStringSEL])))

/* PDWordFilterWord */
#define PDWordFilterWord (ACROASSERT(gPDModelVersion >=PDModelHFT_VERSION_2), *((PDWordFilterWordSELPROTO)(gPDModelHFT[PDWordFilterWordSEL])))

/* PDStyleGetFont */
#define PDStyleGetFont (ACROASSERT(gPDModelVersion >=PDModelHFT_VERSION_2), *((PDStyleGetFontSELPROTO)(gPDModelHFT[PDStyleGetFontSEL])))

/* PDStyleGetFontSize */
#define PDStyleGetFontSize (ACROASSERT(gPDModelVersion >=PDModelHFT_VERSION_2), *((PDStyleGetFontSizeSELPROTO)(gPDModelHFT[PDStyleGetFontSizeSEL])))

/* PDStyleGetColor */
#define PDStyleGetColor (ACROASSERT(gPDModelVersion >=PDModelHFT_VERSION_2), *((PDStyleGetColorSELPROTO)(gPDModelHFT[PDStyleGetColorSEL])))

/* PDFileSpecNewFromASPath */
#define PDFileSpecNewFromASPath (ACROASSERT(gPDModelVersion >=PDModelHFT_VERSION_2), *((PDFileSpecNewFromASPathSELPROTO)(gPDModelHFT[PDFileSpecNewFromASPathSEL])))

/* PDFileSpecFromCosObj */
#define PDFileSpecFromCosObj (ACROASSERT(gPDModelVersion >=PDModelHFT_VERSION_2), *((PDFileSpecFromCosObjSELPROTO)(gPDModelHFT[PDFileSpecFromCosObjSEL])))

/* PDFileSpecGetFileSys */
#define PDFileSpecGetFileSys (ACROASSERT(gPDModelVersion >=PDModelHFT_VERSION_2), *((PDFileSpecGetFileSysSELPROTO)(gPDModelHFT[ PDFileSpecGetFileSysSEL])))

/* PDFileSpecAcquireASPath */
#define PDFileSpecAcquireASPath (ACROASSERT(gPDModelVersion >=PDModelHFT_VERSION_2), *((PDFileSpecAcquireASPathSELPROTO)(gPDModelHFT[PDFileSpecAcquireASPathSEL])))

/* PDFileSpecGetCosObj */
#define PDFileSpecGetCosObj (ACROASSERT(gPDModelVersion >=PDModelHFT_VERSION_2), *((PDFileSpecGetCosObjSELPROTO)(gPDModelHFT[PDFileSpecGetCosObjSEL])))

/* PDFileSpecIsValid */
#define PDFileSpecIsValid (ACROASSERT(gPDModelVersion >=PDModelHFT_VERSION_2), *((PDFileSpecIsValidSELPROTO)(gPDModelHFT[PDFileSpecIsValidSEL])))

/* PDRegisterFileSpecHandler */
#define PDRegisterFileSpecHandler (ACROASSERT(gPDModelVersion >=PDModelHFT_VERSION_2), *((PDRegisterFileSpecHandlerSELPROTO)(gPDModelHFT[PDRegisterFileSpecHandlerSEL])))

/* PDFileSpecGetDIPath */
#define PDFileSpecGetDIPath (ACROASSERT(gPDModelVersion >=PDModelHFT_VERSION_2), *((PDFileSpecGetDIPathSELPROTO)(gPDModelHFT[PDFileSpecGetDIPathSEL])))

/* PDPrefSetColorCal */
#define PDPrefSetColorCal (ACROASSERT(gPDModelVersion >=PDModelHFT_VERSION_2), *((PDPrefSetColorCalSELPROTO)(gPDModelHFT[PDPrefSetColorCalSEL])))

/* PDPrefGetColorCal */
#define PDPrefGetColorCal (ACROASSERT(gPDModelVersion >=PDModelHFT_VERSION_2), *((PDPrefGetColorCalSELPROTO)(gPDModelHFT[PDPrefGetColorCalSEL])))

/* If you need to use PDPageNotifyContentsDidChangeEx or PDDocClearFlags
** from within your plug-in, you will need to bump up the value of
** PI_PDModelHFT_VERSION in PIRequir.h to 0x00020001.
*/

/* PDPageNotifyContentsDidChangeEx */
#define PDPageNotifyContentsDidChangeEx (ACROASSERT(gPDModelVersion >=PDModelHFT_VERSION_2_1), *((PDPageNotifyContentsDidChangeExSELPROTO)(gPDModelHFT[PDPageNotifyContentsDidChangeExSEL])))

/* PDDocClearFlags */
#define PDDocClearFlags (ACROASSERT(gPDModelVersion >=PDModelHFT_VERSION_2_1), *((PDDocClearFlagsSELPROTO)(gPDModelHFT[PDDocClearFlagsSEL ])))

#if !MAC_PLATFORM || (MAC_PLATFORM && !AS_ARCH_64BIT)
/* PDDrawCosObjToWindow */
#define PDDrawCosObjToWindow (ACROASSERT(gPDModelVersion >=PDModelHFT_VERSION_2_1), *((PDDrawCosObjToWindowSELPROTO)(gPDModelHFT[ PDDrawCosObjToWindowSEL])))
#endif /* #if !MAC_PLATFORM || (MAC_PLATFORM && !AS_ARCH_64BIT) */

/* If you need to use PDDocOpenFromASFile
** from within your plug-in, you will need to bump up the value of
** PI_PDModelHFT_VERSION in PIRequir.h to 0x00020002.
*/

/* PDDocOpenFromASFile */
#define PDDocOpenFromASFile (ACROASSERT(gPDModelVersion >=PDModelHFT_VERSION_2_2), *((PDDocOpenFromASFileSELPROTO)(gPDModelHFT[PDDocOpenFromASFileSEL])))

/* PDFileSpecGetDoc */
#define PDFileSpecGetDoc (ACROASSERT(gPDModelVersion >=PDModelHFT_VERSION_2_2), *((PDFileSpecGetDocSELPROTO)(gPDModelHFT[PDFileSpecGetDocSEL])))

/* PDFileSpecGetFileSysName */
#define PDFileSpecGetFileSysName (ACROASSERT(gPDModelVersion >=PDModelHFT_VERSION_2_2), *((PDFileSpecGetFileSysNameSELPROTO)(gPDModelHFT[PDFileSpecGetFileSysNameSEL])))

/* PDRegisterFileSpecHandlerByName */
#define PDRegisterFileSpecHandlerByName (ACROASSERT(gPDModelVersion >=PDModelHFT_VERSION_2_2), *((PDRegisterFileSpecHandlerByNameSELPROTO)(gPDModelHFT[PDRegisterFileSpecHandlerByNameSEL])))

/* PDPageStmGetToken */
#define PDPageStmGetToken (ACROASSERT(gPDModelVersion >=PDModelHFT_VERSION_2_2), *((PDPageStmGetTokenSELPROTO)(gPDModelHFT[PDPageStmGetTokenSEL])))

/* PDPageStmGetInlineImage */
#define PDPageStmGetInlineImage (ACROASSERT(gPDModelVersion >=PDModelHFT_VERSION_2_2), *((PDPageStmGetInlineImageSELPROTO)(gPDModelHFT[PDPageStmGetInlineImageSEL])))

/* PDViewDestResolve */
#define PDViewDestResolve (ACROASSERT(gPDModelVersion >=PDModelHFT_VERSION_2_2), *((PDViewDestResolveSELPROTO)(gPDModelHFT[PDViewDestResolveSEL])))


/* Document level transition routines  */
#define PDTransIsValid (ACROASSERT(gPDModelVersion >=PDModelHFT_VERSION_2_2), *((PDTransIsValidSELPROTO)(gPDModelHFT[PDTransIsValidSEL])))

#define PDTransNull (ACROASSERT(gPDModelVersion >=PDModelHFT_VERSION_2_2), *((PDTransNullSELPROTO)(gPDModelHFT[PDTransNullSEL])))

#define PDTransFromCosObj (ACROASSERT(gPDModelVersion >=PDModelHFT_VERSION_2_2), *((PDTransFromCosObjSELPROTO)(gPDModelHFT[PDTransFromCosObjSEL])))

#define PDTransGetCosObj (ACROASSERT(gPDModelVersion >=PDModelHFT_VERSION_2_2), *((PDTransGetCosObjSELPROTO)(gPDModelHFT[PDTransGetCosObjSEL])))

#define PDTransEqual (ACROASSERT(gPDModelVersion >=PDModelHFT_VERSION_2_2), *((PDTransEqualSELPROTO)(gPDModelHFT[PDTransEqualSEL])))


/* Page level transition set/get routines. */
#define PDPageHasTransition (ACROASSERT(gPDModelVersion >=PDModelHFT_VERSION_2_2), *((PDPageHasTransitionSELPROTO)(gPDModelHFT[PDPageHasTransitionSEL])))

#define PDPageGetTransition (ACROASSERT(gPDModelVersion >=PDModelHFT_VERSION_2_2), *((PDPageGetTransitionSELPROTO)(gPDModelHFT[PDPageGetTransitionSEL])))

#define PDPageSetTransition (ACROASSERT(gPDModelVersion >=PDModelHFT_VERSION_2_2), *((PDPageSetTransitionSELPROTO)(gPDModelHFT[PDPageSetTransitionSEL])))

#define PDPageGetDuration (ACROASSERT(gPDModelVersion >=PDModelHFT_VERSION_2_2), *((PDPageGetDurationSELPROTO)(gPDModelHFT[PDPageGetDurationSEL])))

#define PDPageSetDuration (ACROASSERT(gPDModelVersion >=PDModelHFT_VERSION_2_2), *((PDPageSetDurationSELPROTO)(gPDModelHFT[PDPageSetDurationSEL])))

/* Creation, set/get methods. */
#define PDTransNewFromCosDoc (ACROASSERT(gPDModelVersion >=PDModelHFT_VERSION_2_2), *((PDTransNewFromCosDocSELPROTO)(gPDModelHFT[PDTransNewFromCosDocSEL])))

#define PDTransNew (ACROASSERT(gPDModelVersion >=PDModelHFT_VERSION_2_2), *((PDTransNewSELPROTO)(gPDModelHFT[PDTransNewSEL])))

#define PDTransGetSubtype (ACROASSERT(gPDModelVersion >=PDModelHFT_VERSION_2_2), *((PDTransGetSubtypeSELPROTO)(gPDModelHFT[PDTransGetSubtypeSEL])))

#define PDTransGetDuration (ACROASSERT(gPDModelVersion >=PDModelHFT_VERSION_2_2), *((PDTransGetDurationSELPROTO)(gPDModelHFT[PDTransGetDurationSEL])))


/* PDDocReadAhead */
#define PDDocReadAhead (ACROASSERT(gPDModelVersion >=PDModelHFT_VERSION_2_2), *((PDDocReadAheadSELPROTO)(gPDModelHFT[PDDocReadAheadSEL])))

#define PDDocOpenEx (ACROASSERT(gPDModelVersion >=PDModelHFT_VERSION_2_2), *((PDDocOpenExSELPROTO)(gPDModelHFT[PDDocOpenExSEL])))

#define PDDocOpenFromASFileEx (ACROASSERT(gPDModelVersion >=PDModelHFT_VERSION_2_2), *((PDDocOpenFromASFileExSELPROTO)(gPDModelHFT[PDDocOpenFromASFileExSEL])))


/* PDRegisterCryptHandlerEx */
#define PDRegisterCryptHandlerEx (ACROASSERT(gPDModelVersion >=PDModelHFT_VERSION_2_2), *((PDRegisterCryptHandlerExSELPROTO)(gPDModelHFT[PDRegisterCryptHandlerExSEL])))

/* PDDocGetCryptHandlerClientData */
#define PDDocGetCryptHandlerClientData (ACROASSERT(gPDModelVersion >=PDModelHFT_VERSION_2_2), *((PDDocGetCryptHandlerClientDataSELPROTO)(gPDModelHFT[PDDocGetCryptHandlerClientDataSEL])))



#define PDDocGetFullScreen (ACROASSERT(gPDModelVersion >=PDModelHFT_VERSION_2_2), *((PDDocGetFullScreenSELPROTO)(gPDModelHFT[PDDocGetFullScreenSEL])))



#define PDDocSetFullScreen (ACROASSERT(gPDModelVersion >=PDModelHFT_VERSION_2_2), *((PDDocSetFullScreenSELPROTO)(gPDModelHFT[PDDocSetFullScreenSEL])))

/* PDDocSaveWithParams */
#define PDDocSaveWithParams (ACROASSERT(gPDModelVersion >=PDModelHFT_VERSION_2_2), *((PDDocSaveWithParamsSELPROTO)(gPDModelHFT[PDDocSaveWithParamsSEL])))

/* PDNameTreeLookup */
#define PDNameTreeLookup (ACROASSERT(gPDModelVersion >=PDModelHFT_VERSION_2_2), *((PDNameTreeLookupSELPROTO)(gPDModelHFT[PDNameTreeLookupSEL])))
	

/* PDFontGetDescendant */
#define PDFontGetDescendant (ACROASSERT(gPDModelVersion >=PDModelHFT_VERSION_2_3), *((PDFontGetDescendantSELPROTO)(gPDModelHFT[PDFontGetDescendantSEL])))

/* PDFontGetEncodingName */
#define PDFontGetEncodingName (ACROASSERT(gPDModelVersion >=PDModelHFT_VERSION_2_3), *((PDFontGetEncodingNameSELPROTO)(gPDModelHFT[PDFontGetEncodingNameSEL])))

/* PDFontGetCIDSystemInfo */
#define PDFontGetCIDSystemInfo (ACROASSERT(gPDModelVersion >=PDModelHFT_VERSION_2_3), *((PDFontGetCIDSystemInfoSELPROTO)(gPDModelHFT[PDFontGetCIDSystemInfoSEL])))

/* PDFontGetCIDSystemSupplement */
#define PDFontGetCIDSystemSupplement (ACROASSERT(gPDModelVersion >=PDModelHFT_VERSION_2_3), *((PDFontGetCIDSystemSupplementSELPROTO)(gPDModelHFT[PDFontGetCIDSystemSupplementSEL])))

/* PDXlateToHostEx */
#define PDXlateToHostEx (ACROASSERT(gPDModelVersion >=PDModelHFT_VERSION_2_3), *((PDXlateToHostExSELPROTO)(gPDModelHFT[PDXlateToHostExSEL])))

/* PDXlateToPDFDocEncEx */
#define PDXlateToPDFDocEncEx (ACROASSERT(gPDModelVersion >=PDModelHFT_VERSION_2_3), *((PDXlateToPDFDocEncExSELPROTO)(gPDModelHFT[PDXlateToPDFDocEncExSEL])))

/* PDHostMBLen */
#define PDHostMBLen (ACROASSERT(gPDModelVersion >=PDModelHFT_VERSION_2_3), *((PDHostMBLenSELPROTO)(gPDModelHFT[PDHostMBLenSEL])))

/* PDGetHostEncoding */
#define PDGetHostEncoding (ACROASSERT(gPDModelVersion >=PDModelHFT_VERSION_2_3), *((PDGetHostEncodingSELPROTO)(gPDModelHFT[PDGetHostEncodingSEL])))

/** PDDocCreateWordFinderUCS 
	@ref GlyphNames
*/
#define PDDocCreateWordFinderUCS (ACROASSERT(gPDModelVersion >=PDModelHFT_VERSION_2_3), *((PDDocCreateWordFinderUCSSELPROTO)(gPDModelHFT[PDDocCreateWordFinderUCSSEL])))

/* PDFontXlateToHost */
#define PDFontXlateToHost (ACROASSERT(gPDModelVersion >=PDModelHFT_VERSION_2_3), *((PDFontXlateToHostSELPROTO)(gPDModelHFT[PDFontXlateToHostSEL])))

/* PDFontXlateToUCS */
#define PDFontXlateToUCS (ACROASSERT(gPDModelVersion >=PDModelHFT_VERSION_2_3), *((PDFontXlateToUCSSELPROTO)(gPDModelHFT[PDFontXlateToUCSSEL])))



/* Acrobat 4.0 Additions */  

/* PDDocFromCosDoc */
#define PDDocFromCosDoc (ACROASSERT(gPDModelVersion >=PDModelHFT_VERSION_4), *((PDDocFromCosDocSELPROTO)(gPDModelHFT[PDDocFromCosDocSEL])))

/* PDDocEnumResources */
#define PDDocEnumResources (ACROASSERT(gPDModelVersion >=PDModelHFT_VERSION_4), *((PDDocEnumResourcesSELPROTO)(gPDModelHFT[PDDocEnumResourcesSEL])))
	
#define PDDocImportCosDocNotes (ACROASSERT(gPDModelVersion >=PDModelHFT_VERSION_4), *((PDDocImportCosDocNotesSELPROTO)(gPDModelHFT[PDDocImportCosDocNotesSEL])))

#define PDDocExportNotes (ACROASSERT(gPDModelVersion >=PDModelHFT_VERSION_4), *((PDDocExportNotesSELPROTO)(gPDModelHFT[PDDocExportNotesSEL])))

#define PDPageGetAnnotSequence (ACROASSERT(gPDModelVersion >=PDModelHFT_VERSION_4), *((PDPageGetAnnotSequenceSELPROTO)(gPDModelHFT[PDPageGetAnnotSequenceSEL])))

#define PDRegisterAnnotHandler (ACROASSERT(gPDModelVersion >=PDModelHFT_VERSION_4), *((PDRegisterAnnotHandlerSELPROTO)(gPDModelHFT[PDRegisterAnnotHandlerSEL])))

#define PDGetAnnotHandlerByName (ACROASSERT(gPDModelVersion >=PDModelHFT_VERSION_4), *((PDGetAnnotHandlerByNameSELPROTO)(gPDModelHFT[PDGetAnnotHandlerByNameSEL])))

/* PDNameTree related calls */
#define PDNameTreeNew (ACROASSERT(gPDModelVersion >=PDModelHFT_VERSION_4), *((PDNameTreeNewSELPROTO)(gPDModelHFT[PDNameTreeNewSEL])))

#define PDNameTreeFromCosObj (ACROASSERT(gPDModelVersion >=PDModelHFT_VERSION_4), *((PDNameTreeFromCosObjSELPROTO)(gPDModelHFT[PDNameTreeFromCosObjSEL])))

#define PDNameTreeGetCosObj (ACROASSERT(gPDModelVersion >=PDModelHFT_VERSION_4), *((PDNameTreeGetCosObjSELPROTO)(gPDModelHFT[PDNameTreeGetCosObjSEL])))

#define PDNameTreeIsValid (ACROASSERT(gPDModelVersion >=PDModelHFT_VERSION_4), *((PDNameTreeIsValidSELPROTO)(gPDModelHFT[PDNameTreeIsValidSEL])))

#define PDNameTreeEqual (ACROASSERT(gPDModelVersion >=PDModelHFT_VERSION_4), *((PDNameTreeEqualSELPROTO)(gPDModelHFT[PDNameTreeEqualSEL])))

#define PDNameTreePut (ACROASSERT(gPDModelVersion >=PDModelHFT_VERSION_4), *((PDNameTreePutSELPROTO)(gPDModelHFT[PDNameTreePutSEL])))

#define PDNameTreeGet (ACROASSERT(gPDModelVersion >=PDModelHFT_VERSION_4), *((PDNameTreeGetSELPROTO)(gPDModelHFT[PDNameTreeGetSEL])))

#define PDNameTreeRemove (ACROASSERT(gPDModelVersion >=PDModelHFT_VERSION_4), *((PDNameTreeRemoveSELPROTO)(gPDModelHFT[PDNameTreeRemoveSEL])))

#define PDNameTreeEnum (ACROASSERT(gPDModelVersion >=PDModelHFT_VERSION_4), *((PDNameTreeEnumSELPROTO)(gPDModelHFT[PDNameTreeEnumSEL])))

#define PDDocGetNameTree (ACROASSERT(gPDModelVersion >=PDModelHFT_VERSION_4), *((PDDocGetNameTreeSELPROTO)(gPDModelHFT[PDDocGetNameTreeSEL])))

#define PDDocCreateNameTree (ACROASSERT(gPDModelVersion >=PDModelHFT_VERSION_4), *((PDDocCreateNameTreeSELPROTO)(gPDModelHFT[PDDocCreateNameTreeSEL])))

#define PDDocRemoveNameTree (ACROASSERT(gPDModelVersion >=PDModelHFT_VERSION_4), *((PDDocRemoveNameTreeSELPROTO)(gPDModelHFT[PDDocRemoveNameTreeSEL])))


/* PDPageLabel related calls */
#define PDPageLabelIsValid (ACROASSERT(gPDModelVersion >=PDModelHFT_VERSION_4), *((PDPageLabelIsValidSELPROTO)(gPDModelHFT[PDPageLabelIsValidSEL])))

#define PDPageLabelEqual (ACROASSERT(gPDModelVersion >=PDModelHFT_VERSION_4), *((PDPageLabelEqualSELPROTO)(gPDModelHFT[PDPageLabelEqualSEL])))

#define PDPageLabelGetCosObj (ACROASSERT(gPDModelVersion >=PDModelHFT_VERSION_4), *((PDPageLabelGetCosObjSELPROTO)(gPDModelHFT[PDPageLabelGetCosObjSEL])))

#define PDPageLabelFromCosObj (ACROASSERT(gPDModelVersion >=PDModelHFT_VERSION_4), *((PDPageLabelFromCosObjSELPROTO)(gPDModelHFT[PDPageLabelFromCosObjSEL])))

#define PDPageLabelGetStyle (ACROASSERT(gPDModelVersion >=PDModelHFT_VERSION_4), *((PDPageLabelGetStyleSELPROTO)(gPDModelHFT[PDPageLabelGetStyleSEL])))

#define PDPageLabelGetPrefix (ACROASSERT(gPDModelVersion >=PDModelHFT_VERSION_4), *((PDPageLabelGetPrefixSELPROTO)(gPDModelHFT[PDPageLabelGetPrefixSEL])))

#define PDPageLabelGetStart (ACROASSERT(gPDModelVersion >=PDModelHFT_VERSION_4), *((PDPageLabelGetStartSELPROTO)(gPDModelHFT[PDPageLabelGetStartSEL])))

#define PDDocGetPageLabel (ACROASSERT(gPDModelVersion >=PDModelHFT_VERSION_4), *((PDDocGetPageLabelSELPROTO)(gPDModelHFT[PDDocGetPageLabelSEL])))

#define PDPageLabelNew (ACROASSERT(gPDModelVersion >=PDModelHFT_VERSION_4), *((PDPageLabelNewSELPROTO)(gPDModelHFT[PDPageLabelNewSEL])))

#define PDDocSetPageLabel (ACROASSERT(gPDModelVersion >=PDModelHFT_VERSION_4), *((PDDocSetPageLabelSELPROTO)(gPDModelHFT[PDDocSetPageLabelSEL])))

#define PDDocRemovePageLabel (ACROASSERT(gPDModelVersion >=PDModelHFT_VERSION_4), *((PDDocRemovePageLabelSELPROTO)(gPDModelHFT[PDDocRemovePageLabelSEL])))

#define PDDocOpenWithParams (ACROASSERT(gPDModelVersion >=PDModelHFT_VERSION_4), *((PDDocOpenWithParamsSELPROTO)(gPDModelHFT[PDDocOpenWithParamsSEL])))

#define PDDocReadAheadPages (ACROASSERT(gPDModelVersion >=PDModelHFT_VERSION_4), *((PDDocReadAheadPagesSELPROTO)(gPDModelHFT[PDDocReadAheadPagesSEL])))

#define PDDocGetLabelForPageNum (ACROASSERT(gPDModelVersion >=PDModelHFT_VERSION_4), *((PDDocGetLabelForPageNumSELPROTO)(gPDModelHFT[PDDocGetLabelForPageNumSEL])))
#define PDDocFindPageNumForLabel (ACROASSERT(gPDModelVersion >=PDModelHFT_VERSION_4), *((PDDocFindPageNumForLabelSELPROTO)(gPDModelHFT[PDDocFindPageNumForLabelSEL])))

#define PDDocImportNotes (ACROASSERT(gPDModelVersion >=PDModelHFT_VERSION_4), *((PDDocImportNotesSELPROTO)(gPDModelHFT[PDDocImportNotesSEL])))

#define PDImageSelectAlternate (ACROASSERT(gPDModelVersion >=PDModelHFT_VERSION_4), *((PDImageSelectAlternateSELPROTO)(gPDModelHFT[PDImageSelectAlternateSEL])))
#define PDImageSelGetGeoAttr (ACROASSERT(gPDModelVersion >=PDModelHFT_VERSION_4), *((PDImageSelGetGeoAttrSELPROTO)(gPDModelHFT[PDImageSelGetGeoAttrSEL])))
#define PDImageSelGetDeviceAttr (ACROASSERT(gPDModelVersion >=PDModelHFT_VERSION_4), *((PDImageSelGetDeviceAttrSELPROTO)(gPDModelHFT[PDImageSelGetDeviceAttrSEL])))
#define PDImageSelAdjustMatrix (ACROASSERT(gPDModelVersion >=PDModelHFT_VERSION_4), *((PDImageSelAdjustMatrixSELPROTO)(gPDModelHFT[PDImageSelAdjustMatrixSEL])))
#define PDApplyFunction (ACROASSERT(gPDModelVersion >=PDModelHFT_VERSION_4), *((PDApplyFunctionSELPROTO)(gPDModelHFT[PDApplyFunctionSEL])))

#define PDNumTreeNew (ACROASSERT(gPDModelVersion >=PDModelHFT_VERSION_4), *((PDNumTreeNewSELPROTO)(gPDModelHFT[PDNumTreeNewSEL])))
#define PDNumTreeFromCosObj (ACROASSERT(gPDModelVersion >=PDModelHFT_VERSION_4), *((PDNumTreeFromCosObjSELPROTO)(gPDModelHFT[PDNumTreeFromCosObjSEL])))
#define PDNumTreeGetCosObj (ACROASSERT(gPDModelVersion >=PDModelHFT_VERSION_4), *((PDNumTreeGetCosObjSELPROTO)(gPDModelHFT[PDNumTreeGetCosObjSEL])))
#define PDNumTreeIsValid (ACROASSERT(gPDModelVersion >=PDModelHFT_VERSION_4), *((PDNumTreeIsValidSELPROTO)(gPDModelHFT[PDNumTreeIsValidSEL])))
#define PDNumTreeEqual (ACROASSERT(gPDModelVersion >=PDModelHFT_VERSION_4), *((PDNumTreeEqualSELPROTO)(gPDModelHFT[PDNumTreeEqualSEL])))
#define PDNumTreePut (ACROASSERT(gPDModelVersion >=PDModelHFT_VERSION_4), *((PDNumTreePutSELPROTO)(gPDModelHFT[PDNumTreePutSEL])))
#define PDNumTreeGet (ACROASSERT(gPDModelVersion >=PDModelHFT_VERSION_4), *((PDNumTreeGetSELPROTO)(gPDModelHFT[PDNumTreeGetSEL])))
#define PDNumTreeRemove (ACROASSERT(gPDModelVersion >=PDModelHFT_VERSION_4), *((PDNumTreeRemoveSELPROTO)(gPDModelHFT[PDNumTreeRemoveSEL])))
#define PDNumTreeEnum (ACROASSERT(gPDModelVersion >=PDModelHFT_VERSION_4), *((PDNumTreeEnumSELPROTO)(gPDModelHFT[PDNumTreeEnumSEL])))

#define PDFontFromCosObj (ACROASSERT(gPDModelVersion >=PDModelHFT_VERSION_4), *((PDFontFromCosObjSELPROTO)(gPDModelHFT[PDFontFromCosObjSEL])))


#define PDDocCopyToFile (ACROASSERT(gPDModelVersion >=PDModelHFT_VERSION_4_5), *((PDDocCopyToFileSELPROTO)(gPDModelHFT[PDDocCopyToFileSEL])))


#define PDDocPermRequest (ACROASSERT(gPDModelVersion >=PDModelHFT_VERSION_5), *((PDDocPermRequestSELPROTO)(gPDModelHFT[PDDocPermRequestSEL])))

/* Support for Bleed, Trim, Art boxes */
#define PDPageSetBox (ACROASSERT(gPDModelVersion >=PDModelHFT_VERSION_5), *((PDPageSetBoxSELPROTO)(gPDModelHFT[PDPageSetBoxSEL])))
#define PDPageGetBox (ACROASSERT(gPDModelVersion >=PDModelHFT_VERSION_5), *((PDPageGetBoxSELPROTO)(gPDModelHFT[PDPageGetBoxSEL])))

#define PDLinkAnnotRemoveAction (ACROASSERT(gPDModelVersion >=PDModelHFT_VERSION_5), *((PDLinkAnnotRemoveActionSELPROTO)(gPDModelHFT[PDLinkAnnotRemoveActionSEL])))
#define PDBookmarkRemoveAction (ACROASSERT(gPDModelVersion >=PDModelHFT_VERSION_5), *((PDBookmarkRemoveActionSELPROTO)(gPDModelHFT[PDBookmarkRemoveActionSEL])))
#define PDDocRemoveOpenAction (ACROASSERT(gPDModelVersion >=PDModelHFT_VERSION_5), *((PDDocRemoveOpenActionSELPROTO)(gPDModelHFT[PDDocRemoveOpenActionSEL])))
#define PDNameTreeNotifyNameAdded (ACROASSERT(gPDModelVersion >=PDModelHFT_VERSION_5), *((PDNameTreeNotifyNameAddedSELPROTO)(gPDModelHFT[PDNameTreeNotifyNameAddedSEL])))
#define PDNameTreeNotifyNameRemoved (ACROASSERT(gPDModelVersion >=PDModelHFT_VERSION_5), *((PDNameTreeNotifyNameRemovedSELPROTO)(gPDModelHFT[PDNameTreeNotifyNameRemovedSEL])))

/* PDDocGetPageObjByNum */
#define PDDocGetPageObjByNum (ACROASSERT(gPDModelVersion >=PDModelHFT_VERSION_5), *((PDDocGetPageObjByNumSELPROTO)(gPDModelHFT[PDDocGetPageObjByNumSEL])))
/** @ingroup Enumerators */
#define PDTextSelectEnumTextUCS (ACROASSERT(gPDModelVersion >=PDModelHFT_VERSION_5), *((PDTextSelectEnumTextUCSSELPROTO)(gPDModelHFT[PDTextSelectEnumTextUCSSEL])))

#if !MAC_PLATFORM || (MAC_PLATFORM && !AS_ARCH_64BIT)
/* PDPageDrawContentsToWindowEx */
#define PDPageDrawContentsToWindowEx (ACROASSERT(gPDModelVersion >=PDModelHFT_VERSION_5), *((PDPageDrawContentsToWindowExSELPROTO)(gPDModelHFT[PDPageDrawContentsToWindowExSEL])))
#endif /* #if !MAC_PLATFORM || (MAC_PLATFORM && !AS_ARCH_64BIT) */

#define PDBookmarkGetColor (ACROASSERT(gPDModelVersion >=PDModelHFT_VERSION_5), *((PDBookmarkGetColorSELPROTO)(gPDModelHFT[PDBookmarkGetColorSEL])))
#define PDBookmarkSetColor (ACROASSERT(gPDModelVersion >=PDModelHFT_VERSION_5), *((PDBookmarkSetColorSELPROTO)(gPDModelHFT[PDBookmarkSetColorSEL])))
#define PDBookmarkGetFlags (ACROASSERT(gPDModelVersion >=PDModelHFT_VERSION_5), *((PDBookmarkGetFlagsSELPROTO)(gPDModelHFT[PDBookmarkGetFlagsSEL])))
#define PDBookmarkSetFlags (ACROASSERT(gPDModelVersion >=PDModelHFT_VERSION_5), *((PDBookmarkSetFlagsSELPROTO)(gPDModelHFT[PDBookmarkSetFlagsSEL])))

#define PDDocExportSomeNotes (ACROASSERT(gPDModelVersion >=PDModelHFT_VERSION_5), *((PDDocExportSomeNotesSELPROTO)(gPDModelHFT[PDDocExportSomeNotesSEL])))

#define PDPageHasTransparency (ACROASSERT(gPDModelVersion >=PDModelHFT_VERSION_5), *((PDPageHasTransparencySELPROTO)(gPDModelHFT[PDPageHasTransparencySEL])))

#define PDTextSelectCreatePageHiliteEx (ACROASSERT(gPDModelVersion >=PDModelHFT_VERSION_5), *((PDTextSelectCreatePageHiliteExSELPROTO)(gPDModelHFT[PDTextSelectCreatePageHiliteExSEL])))
#define PDTextSelectCreateWordHiliteEx (ACROASSERT(gPDModelVersion >=PDModelHFT_VERSION_5), *((PDTextSelectCreateWordHiliteExSELPROTO)(gPDModelHFT[PDTextSelectCreateWordHiliteExSEL])))
#define PDTextSelectCreateRangesEx (ACROASSERT(gPDModelVersion >=PDModelHFT_VERSION_5), *((PDTextSelectCreateRangesExSELPROTO)(gPDModelHFT[PDTextSelectCreateRangesExSEL])))
#if !MAC_PLATFORM || (MAC_PLATFORM && !AS_ARCH_64BIT)
#define PDPageGetPalette (ACROASSERT(gPDModelVersion >=PDModelHFT_VERSION_5), *((PDPageGetPaletteSELPROTO)(gPDModelHFT[PDPageGetPaletteSEL])))
#endif /* #if !MAC_PLATFORM || (MAC_PLATFORM && !AS_ARCH_64BIT) */

/* Additional Wordy APIs for Acrobat 6.0 */
#define PDDocCreateWordFinderEx (ACROASSERT(gPDModelVersion >=PDModelHFT_VERSION_6), *((PDDocCreateWordFinderExSELPROTO)(gPDModelHFT[PDDocCreateWordFinderExSEL])))
#define PDWordGetCharOffsetEx (ACROASSERT(gPDModelVersion >=PDModelHFT_VERSION_6), *((PDWordGetCharOffsetExSELPROTO)(gPDModelHFT[PDWordGetCharOffsetExSEL])))
#define PDWordGetCharQuad (ACROASSERT(gPDModelVersion >=PDModelHFT_VERSION_6), *((PDWordGetCharQuadSELPROTO)(gPDModelHFT[PDWordGetCharQuadSEL])))
#define PDWordGetNumHiliteChar (ACROASSERT(gPDModelVersion >=PDModelHFT_VERSION_6), *((PDWordGetNumHiliteCharSELPROTO)(gPDModelHFT[PDWordGetNumHiliteCharSEL])))
#define PDWordGetByteIdxFromHiliteChar (ACROASSERT(gPDModelVersion >=PDModelHFT_VERSION_6), *((PDWordGetByteIdxFromHiliteCharSELPROTO)(gPDModelHFT[PDWordGetByteIdxFromHiliteCharSEL])))
#define PDWordGetASText (ACROASSERT(gPDModelVersion >=PDModelHFT_VERSION_6), *((PDWordGetASTextSELPROTO)(gPDModelHFT[PDWordGetASTextSEL])))
#define PDWordGetCharEncFlags (ACROASSERT(gPDModelVersion >=PDModelHFT_VERSION_6), *((PDWordGetCharEncFlagsSELPROTO)(gPDModelHFT[PDWordGetCharEncFlagsSEL])))
#define PDWordGetAttrEx (ACROASSERT(gPDModelVersion >=PDModelHFT_VERSION_6), *((PDWordGetAttrExSELPROTO)(gPDModelHFT[PDWordGetAttrExSEL])))
#define PDWordCreateTextSelect (ACROASSERT(gPDModelVersion >=PDModelHFT_VERSION_6), *((PDWordCreateTextSelectSELPROTO)(gPDModelHFT[PDWordCreateTextSelectSEL])))
/** @ingroup Enumerators */
#define PDWordFinderEnumWordsStr (ACROASSERT(gPDModelVersion >=PDModelHFT_VERSION_6), *((PDWordFinderEnumWordsStrSELPROTO)(gPDModelHFT[PDWordFinderEnumWordsStrSEL])))

/* Separations preview for Acrobat 6.0 */
#define PDPageEnumInks (ACROASSERT(gPDModelVersion >=PDModelHFT_VERSION_6), *((PDPageEnumInksSELPROTO)(gPDModelHFT[PDPageEnumInksSEL])))
#define PDPageMakeSeparations (ACROASSERT(gPDModelVersion >=PDModelHFT_VERSION_6), *((PDPageMakeSeparationsSELPROTO)(gPDModelHFT[PDPageMakeSeparationsSEL])))

/* Routines for copying and pasting PDActions */
#define PDRegisterActionHandler (ACROASSERT(gPDModelVersion >=PDModelHFT_VERSION_6), *((PDRegisterActionHandlerSELPROTO)(gPDModelHFT[PDRegisterActionHandlerSEL])))
#define PDActionCanCopy (ACROASSERT(gPDModelVersion >=PDModelHFT_VERSION_6), *((PDActionCanCopySELPROTO)(gPDModelHFT[PDActionCanCopySEL])))
#define PDActionCopy (ACROASSERT(gPDModelVersion >=PDModelHFT_VERSION_6), *((PDActionCopySELPROTO)(gPDModelHFT[PDActionCopySEL])))
#define PDActionCanPaste (ACROASSERT(gPDModelVersion >=PDModelHFT_VERSION_6), *((PDActionCanPasteSELPROTO)(gPDModelHFT[PDActionCanPasteSEL])))
#define PDActionPaste (ACROASSERT(gPDModelVersion >=PDModelHFT_VERSION_6), *((PDActionPasteSELPROTO)(gPDModelHFT[PDActionPasteSEL])))
#define PDActionDestroyClipboardData (ACROASSERT(gPDModelVersion >=PDModelHFT_VERSION_6), *((PDActionDestroyClipboardDataSELPROTO)(gPDModelHFT[PDActionDestroyClipboardDataSEL])))

/* Routines for copying and pasting PDAnnots */
#define PDAnnotCanCopy (ACROASSERT(gPDModelVersion >=PDModelHFT_VERSION_6), *((PDAnnotCanCopySELPROTO)(gPDModelHFT[PDAnnotCanCopySEL])))
#define PDAnnotCopy (ACROASSERT(gPDModelVersion >=PDModelHFT_VERSION_6), *((PDAnnotCopySELPROTO)(gPDModelHFT[PDAnnotCopySEL])))
#define PDAnnotCanPaste (ACROASSERT(gPDModelVersion >=PDModelHFT_VERSION_6), *((PDAnnotCanPasteSELPROTO)(gPDModelHFT[PDAnnotCanPasteSEL])))
#define PDAnnotPaste (ACROASSERT(gPDModelVersion >=PDModelHFT_VERSION_6), *((PDAnnotPasteSELPROTO)(gPDModelHFT[PDAnnotPasteSEL])))
#define PDAnnotDestroyClipboardData (ACROASSERT(gPDModelVersion >=PDModelHFT_VERSION_6), *((PDAnnotDestroyClipboardDataSELPROTO)(gPDModelHFT[PDAnnotDestroyClipboardDataSEL])))

/* BEGIN Optional Content API calls */
#define PDOCGCreate (ACROASSERT(gPDModelVersion >= PDModelHFT_VERSION_6), *((PDOCGCreateSELPROTO)(gPDModelHFT[PDOCGCreateSEL])))
#define PDOCGCreateFromCosObj (ACROASSERT(gPDModelVersion >= PDModelHFT_VERSION_6), *((PDOCGCreateFromCosObjSELPROTO)(gPDModelHFT[PDOCGCreateFromCosObjSEL])))
#define PDOCGDestroy (ACROASSERT(gPDModelVersion >= PDModelHFT_VERSION_6), *((PDOCGDestroySELPROTO)(gPDModelHFT[PDOCGDestroySEL])))
#define PDOCGGetCosObj (ACROASSERT(gPDModelVersion >= PDModelHFT_VERSION_6), *((PDOCGGetCosObjSELPROTO)(gPDModelHFT[PDOCGGetCosObjSEL])))
#define PDOCGGetFromCosObj (ACROASSERT(gPDModelVersion >= PDModelHFT_VERSION_6), *((PDOCGGetFromCosObjSELPROTO)(gPDModelHFT[PDOCGGetFromCosObjSEL])))
#define PDOCGGetPDDoc (ACROASSERT(gPDModelVersion >= PDModelHFT_VERSION_6), *((PDOCGGetPDDocSELPROTO)(gPDModelHFT[PDOCGGetPDDocSEL])))
#define PDOCGSetName (ACROASSERT(gPDModelVersion >= PDModelHFT_VERSION_6), *((PDOCGSetNameSELPROTO)(gPDModelHFT[PDOCGSetNameSEL])))
#define PDOCGGetName (ACROASSERT(gPDModelVersion >= PDModelHFT_VERSION_6), *((PDOCGGetNameSELPROTO)(gPDModelHFT[PDOCGGetNameSEL])))
#define PDOCGSetInitialState (ACROASSERT(gPDModelVersion >= PDModelHFT_VERSION_6), *((PDOCGSetInitialStateSELPROTO)(gPDModelHFT[PDOCGSetInitialStateSEL])))
#define PDOCGGetInitialState (ACROASSERT(gPDModelVersion >= PDModelHFT_VERSION_6), *((PDOCGGetInitialStateSELPROTO)(gPDModelHFT[PDOCGGetInitialStateSEL])))
#define PDOCGRemoveInitialState (ACROASSERT(gPDModelVersion >= PDModelHFT_VERSION_6), *((PDOCGRemoveInitialStateSELPROTO)(gPDModelHFT[PDOCGRemoveInitialStateSEL])))
#define PDOCGSetUsageDictEntry (ACROASSERT(gPDModelVersion >= PDModelHFT_VERSION_6), *((PDOCGSetUsageDictEntrySELPROTO)(gPDModelHFT[PDOCGSetUsageDictEntrySEL])))
#define PDOCGHasUsageInfo (ACROASSERT(gPDModelVersion >= PDModelHFT_VERSION_6), *((PDOCGHasUsageInfoSELPROTO)(gPDModelHFT[PDOCGHasUsageInfoSEL])))
#define PDOCGGetUsageEntry (ACROASSERT(gPDModelVersion >= PDModelHFT_VERSION_6), *((PDOCGGetUsageEntrySELPROTO)(gPDModelHFT[PDOCGGetUsageEntrySEL])))
#define PDOCGSetIntent (ACROASSERT(gPDModelVersion >= PDModelHFT_VERSION_6), *((PDOCGSetIntentSELPROTO)(gPDModelHFT[PDOCGSetIntentSEL])))
#define PDOCGGetIntent (ACROASSERT(gPDModelVersion >= PDModelHFT_VERSION_6), *((PDOCGGetIntentSELPROTO)(gPDModelHFT[PDOCGGetIntentSEL])))
#define PDOCGGetCurrentState (ACROASSERT(gPDModelVersion >= PDModelHFT_VERSION_6), *((PDOCGGetCurrentStateSELPROTO)(gPDModelHFT[PDOCGGetCurrentStateSEL])))
#define PDOCGSetCurrentState (ACROASSERT(gPDModelVersion >= PDModelHFT_VERSION_6), *((PDOCGSetCurrentStateSELPROTO)(gPDModelHFT[PDOCGSetCurrentStateSEL])))
#define PDOCGUsedInOCContext (ACROASSERT(gPDModelVersion >= PDModelHFT_VERSION_6), *((PDOCGUsedInOCContextSELPROTO)(gPDModelHFT[PDOCGUsedInOCContextSEL])))
#define PDOCGUsedInOCConfig (ACROASSERT(gPDModelVersion >= PDModelHFT_VERSION_6), *((PDOCGUsedInOCConfigSELPROTO)(gPDModelHFT[PDOCGUsedInOCConfigSEL])))
#define PDOCContextApplyAutoStateChanges (ACROASSERT(gPDModelVersion >= PDModelHFT_VERSION_6), *((PDOCContextApplyAutoStateChangesSELPROTO)(gPDModelHFT[PDOCContextApplyAutoStateChangesSEL])))
#define PDPageEnumOCGs (ACROASSERT(gPDModelVersion >= PDModelHFT_VERSION_6), *((PDPageEnumOCGsSELPROTO)(gPDModelHFT[PDPageEnumOCGsSEL])))
#define PDDocEnumOCGs (ACROASSERT(gPDModelVersion >= PDModelHFT_VERSION_6), *((PDDocEnumOCGsSELPROTO)(gPDModelHFT[PDDocEnumOCGsSEL])))
#define PDPageGetOCGs (ACROASSERT(gPDModelVersion >= PDModelHFT_VERSION_6), *((PDPageGetOCGsSELPROTO)(gPDModelHFT[PDPageGetOCGsSEL])))
#define PDOCMDCreate (ACROASSERT(gPDModelVersion >= PDModelHFT_VERSION_6), *((PDOCMDCreateSELPROTO)(gPDModelHFT[PDOCMDCreateSEL])))
#define PDOCMDFindOrCreate (ACROASSERT(gPDModelVersion >= PDModelHFT_VERSION_6), *((PDOCMDFindOrCreateSELPROTO)(gPDModelHFT[PDOCMDFindOrCreateSEL])))
#define PDOCMDGetCosObj (ACROASSERT(gPDModelVersion >= PDModelHFT_VERSION_6), *((PDOCMDGetCosObjSELPROTO)(gPDModelHFT[PDOCMDGetCosObjSEL])))
#define PDOCMDGetPDDoc (ACROASSERT(gPDModelVersion >= PDModelHFT_VERSION_6), *((PDOCMDGetPDDocSELPROTO)(gPDModelHFT[PDOCMDGetPDDocSEL])))
#define PDOCMDGetFromCosObj (ACROASSERT(gPDModelVersion >= PDModelHFT_VERSION_6), *((PDOCMDGetFromCosObjSELPROTO)(gPDModelHFT[PDOCMDGetFromCosObjSEL])))
#define PDOCMDGetOCGs (ACROASSERT(gPDModelVersion >= PDModelHFT_VERSION_6), *((PDOCMDGetOCGsSELPROTO)(gPDModelHFT[PDOCMDGetOCGsSEL])))
#define PDOCMDGetVisPolicy (ACROASSERT(gPDModelVersion >= PDModelHFT_VERSION_6), *((PDOCMDGetVisPolicySELPROTO)(gPDModelHFT[PDOCMDGetVisPolicySEL])))
#define PDAnnotSetOCMD (ACROASSERT(gPDModelVersion >= PDModelHFT_VERSION_6), *((PDAnnotSetOCMDSELPROTO)(gPDModelHFT[PDAnnotSetOCMDSEL])))
#define PDAnnotGetOCMD (ACROASSERT(gPDModelVersion >= PDModelHFT_VERSION_6), *((PDAnnotGetOCMDSELPROTO)(gPDModelHFT[PDAnnotGetOCMDSEL])))
#define PDAnnotRemoveOCMD (ACROASSERT(gPDModelVersion >= PDModelHFT_VERSION_6), *((PDAnnotRemoveOCMDSELPROTO)(gPDModelHFT[PDAnnotRemoveOCMDSEL])))
#define PDOCMDIsCurrentlyVisible (ACROASSERT(gPDModelVersion >= PDModelHFT_VERSION_6), *((PDOCMDIsCurrentlyVisibleSELPROTO)(gPDModelHFT[PDOCMDIsCurrentlyVisibleSEL])))
#define PDOCMDsAreCurrentlyVisible (ACROASSERT(gPDModelVersion >= PDModelHFT_VERSION_6), *((PDOCMDsAreCurrentlyVisibleSELPROTO)(gPDModelHFT[PDOCMDsAreCurrentlyVisibleSEL])))
#define PDAnnotIsCurrentlyVisible (ACROASSERT(gPDModelVersion >= PDModelHFT_VERSION_6), *((PDAnnotIsCurrentlyVisibleSELPROTO)(gPDModelHFT[PDAnnotIsCurrentlyVisibleSEL])))
#define PDOCMDsMakeContentVisible (ACROASSERT(gPDModelVersion >= PDModelHFT_VERSION_6), *((PDOCMDsMakeContentVisibleSELPROTO)(gPDModelHFT[PDOCMDsMakeContentVisibleSEL])))
#define PDOCContextNew (ACROASSERT(gPDModelVersion >= PDModelHFT_VERSION_6), *((PDOCContextNewSELPROTO)(gPDModelHFT[PDOCContextNewSEL])))
#define PDOCContextFree (ACROASSERT(gPDModelVersion >= PDModelHFT_VERSION_6), *((PDOCContextFreeSELPROTO)(gPDModelHFT[PDOCContextFreeSEL])))
#define PDDocGetOCContext (ACROASSERT(gPDModelVersion >= PDModelHFT_VERSION_6), *((PDDocGetOCContextSELPROTO)(gPDModelHFT[PDDocGetOCContextSEL])))
#define PDOCContextGetPDDoc (ACROASSERT(gPDModelVersion >= PDModelHFT_VERSION_6), *((PDOCContextGetPDDocSELPROTO)(gPDModelHFT[PDOCContextGetPDDocSEL])))
#define PDOCContextInit (ACROASSERT(gPDModelVersion >= PDModelHFT_VERSION_6), *((PDOCContextInitSELPROTO)(gPDModelHFT[PDOCContextInitSEL])))
#define PDOCContextMakeCopy (ACROASSERT(gPDModelVersion >= PDModelHFT_VERSION_6), *((PDOCContextMakeCopySELPROTO)(gPDModelHFT[PDOCContextMakeCopySEL])))
#define PDOCContextNewWithOCDisabled (ACROASSERT(gPDModelVersion >= PDModelHFT_VERSION_6), *((PDOCContextNewWithOCDisabledSELPROTO)(gPDModelHFT[PDOCContextNewWithOCDisabledSEL])))
#define PDOCContextNewWithInitialState (ACROASSERT(gPDModelVersion >= PDModelHFT_VERSION_6), *((PDOCContextNewWithInitialStateSELPROTO)(gPDModelHFT[PDOCContextNewWithInitialStateSEL])))
#define PDOCContextGetOCGStates (ACROASSERT(gPDModelVersion >= PDModelHFT_VERSION_6), *((PDOCContextGetOCGStatesSELPROTO)(gPDModelHFT[PDOCContextGetOCGStatesSEL])))
#define PDOCContextSetOCGStates (ACROASSERT(gPDModelVersion >= PDModelHFT_VERSION_6), *((PDOCContextSetOCGStatesSELPROTO)(gPDModelHFT[PDOCContextSetOCGStatesSEL])))
#define PDOCContextMakeCopyWithAutoStateChanges (ACROASSERT(gPDModelVersion >= PDModelHFT_VERSION_6), *((PDOCContextMakeCopyWithAutoStateChangesSELPROTO)(gPDModelHFT[PDOCContextMakeCopyWithAutoStateChangesSEL])))
#define PDOCContextFindAutoStateChanges (ACROASSERT(gPDModelVersion >= PDModelHFT_VERSION_6), *((PDOCContextFindAutoStateChangesSELPROTO)(gPDModelHFT[PDOCContextFindAutoStateChangesSEL])))
#define PDOCContextSetOCDrawEnumType (ACROASSERT(gPDModelVersion >= PDModelHFT_VERSION_6), *((PDOCContextSetOCDrawEnumTypeSELPROTO)(gPDModelHFT[PDOCContextSetOCDrawEnumTypeSEL])))
#define PDOCContextGetOCDrawEnumType (ACROASSERT(gPDModelVersion >= PDModelHFT_VERSION_6), *((PDOCContextGetOCDrawEnumTypeSELPROTO)(gPDModelHFT[PDOCContextGetOCDrawEnumTypeSEL])))
#define PDOCContextSetIntent (ACROASSERT(gPDModelVersion >= PDModelHFT_VERSION_6), *((PDOCContextSetIntentSELPROTO)(gPDModelHFT[PDOCContextSetIntentSEL])))
#define PDOCContextGetIntent (ACROASSERT(gPDModelVersion >= PDModelHFT_VERSION_6), *((PDOCContextGetIntentSELPROTO)(gPDModelHFT[PDOCContextGetIntentSEL])))
#define PDOCContextSetNonOCDrawing (ACROASSERT(gPDModelVersion >= PDModelHFT_VERSION_6), *((PDOCContextSetNonOCDrawingSELPROTO)(gPDModelHFT[PDOCContextSetNonOCDrawingSEL])))
#define PDOCContextGetNonOCDrawing (ACROASSERT(gPDModelVersion >= PDModelHFT_VERSION_6), *((PDOCContextGetNonOCDrawingSELPROTO)(gPDModelHFT[PDOCContextGetNonOCDrawingSEL])))
#define PDOCContextResetOCMDStack (ACROASSERT(gPDModelVersion >= PDModelHFT_VERSION_6), *((PDOCContextResetOCMDStackSELPROTO)(gPDModelHFT[PDOCContextResetOCMDStackSEL])))
#define PDOCContextPushOCMD (ACROASSERT(gPDModelVersion >= PDModelHFT_VERSION_6), *((PDOCContextPushOCMDSELPROTO)(gPDModelHFT[PDOCContextPushOCMDSEL])))
#define PDOCContextPopOCMD (ACROASSERT(gPDModelVersion >= PDModelHFT_VERSION_6), *((PDOCContextPopOCMDSELPROTO)(gPDModelHFT[PDOCContextPopOCMDSEL])))
#define PDOCContextContentIsVisible (ACROASSERT(gPDModelVersion >= PDModelHFT_VERSION_6), *((PDOCContextContentIsVisibleSELPROTO)(gPDModelHFT[PDOCContextContentIsVisibleSEL])))
#define PDOCContextXObjectIsVisible (ACROASSERT(gPDModelVersion >= PDModelHFT_VERSION_6), *((PDOCContextXObjectIsVisibleSELPROTO)(gPDModelHFT[PDOCContextXObjectIsVisibleSEL])))
#define PDOCConfigCreate (ACROASSERT(gPDModelVersion >= PDModelHFT_VERSION_6), *((PDOCConfigCreateSELPROTO)(gPDModelHFT[PDOCConfigCreateSEL])))
#define PDOCConfigDestroy (ACROASSERT(gPDModelVersion >= PDModelHFT_VERSION_6), *((PDOCConfigDestroySELPROTO)(gPDModelHFT[PDOCConfigDestroySEL])))
#define PDDocGetOCConfig (ACROASSERT(gPDModelVersion >= PDModelHFT_VERSION_6), *((PDDocGetOCConfigSELPROTO)(gPDModelHFT[PDDocGetOCConfigSEL])))
#define PDOCConfigGetPDDoc (ACROASSERT(gPDModelVersion >= PDModelHFT_VERSION_6), *((PDOCConfigGetPDDocSELPROTO)(gPDModelHFT[PDOCConfigGetPDDocSEL])))
#define PDOCConfigGetCosObj (ACROASSERT(gPDModelVersion >= PDModelHFT_VERSION_6), *((PDOCConfigGetCosObjSELPROTO)(gPDModelHFT[PDOCConfigGetCosObjSEL])))
#define PDOCConfigSetOCGOrder (ACROASSERT(gPDModelVersion >= PDModelHFT_VERSION_6), *((PDOCConfigSetOCGOrderSELPROTO)(gPDModelHFT[PDOCConfigSetOCGOrderSEL])))
#define PDOCConfigGetOCGOrder (ACROASSERT(gPDModelVersion >= PDModelHFT_VERSION_6), *((PDOCConfigGetOCGOrderSELPROTO)(gPDModelHFT[PDOCConfigGetOCGOrderSEL])))
#define PDOCConfigMakeRadioButtonGroup (ACROASSERT(gPDModelVersion >= PDModelHFT_VERSION_6), *((PDOCConfigMakeRadioButtonGroupSELPROTO)(gPDModelHFT[PDOCConfigMakeRadioButtonGroupSEL])))
#define PDOCConfigGetRadioButtonGroupForOCG (ACROASSERT(gPDModelVersion >= PDModelHFT_VERSION_6), *((PDOCConfigGetRadioButtonGroupForOCGSELPROTO)(gPDModelHFT[PDOCConfigGetRadioButtonGroupForOCGSEL])))
#define PDOCConfigGetAllRadioButtonGroups (ACROASSERT(gPDModelVersion >= PDModelHFT_VERSION_6), *((PDOCConfigGetAllRadioButtonGroupsSELPROTO)(gPDModelHFT[PDOCConfigGetAllRadioButtonGroupsSEL])))
#define PDOCConfigSetInitState (ACROASSERT(gPDModelVersion >= PDModelHFT_VERSION_6), *((PDOCConfigSetInitStateSELPROTO)(gPDModelHFT[PDOCConfigSetInitStateSEL])))
#define PDOCConfigGetInitState (ACROASSERT(gPDModelVersion >= PDModelHFT_VERSION_6), *((PDOCConfigGetInitStateSELPROTO)(gPDModelHFT[PDOCConfigGetInitStateSEL])))
#define PDOCConfigSetName (ACROASSERT(gPDModelVersion >= PDModelHFT_VERSION_6), *((PDOCConfigSetNameSELPROTO)(gPDModelHFT[PDOCConfigSetNameSEL])))
#define PDOCConfigGetName (ACROASSERT(gPDModelVersion >= PDModelHFT_VERSION_6), *((PDOCConfigGetNameSELPROTO)(gPDModelHFT[PDOCConfigGetNameSEL])))
#define PDOCConfigSetCreator (ACROASSERT(gPDModelVersion >= PDModelHFT_VERSION_6), *((PDOCConfigSetCreatorSELPROTO)(gPDModelHFT[PDOCConfigSetCreatorSEL])))
#define PDOCConfigGetCreator (ACROASSERT(gPDModelVersion >= PDModelHFT_VERSION_6), *((PDOCConfigGetCreatorSELPROTO)(gPDModelHFT[PDOCConfigGetCreatorSEL])))
#define PDOCConfigSetIntent (ACROASSERT(gPDModelVersion >= PDModelHFT_VERSION_6), *((PDOCConfigSetIntentSELPROTO)(gPDModelHFT[PDOCConfigSetIntentSEL])))
#define PDOCConfigGetIntent (ACROASSERT(gPDModelVersion >= PDModelHFT_VERSION_6), *((PDOCConfigGetIntentSELPROTO)(gPDModelHFT[PDOCConfigGetIntentSEL])))
#define PDDocEnumOCConfigs (ACROASSERT(gPDModelVersion >= PDModelHFT_VERSION_6), *((PDDocEnumOCConfigsSELPROTO)(gPDModelHFT[PDDocEnumOCConfigsSEL])))
#define PDDocHasOC (ACROASSERT(gPDModelVersion >= PDModelHFT_VERSION_6), *((PDDocHasOCSELPROTO)(gPDModelHFT[PDDocHasOCSEL])))
#define PDDocGetNumOCGs (ACROASSERT(gPDModelVersion >= PDModelHFT_VERSION_6), *((PDDocGetNumOCGsSELPROTO)(gPDModelHFT[PDDocGetNumOCGsSEL])))
#define PDDocGetOCGs (ACROASSERT(gPDModelVersion >= PDModelHFT_VERSION_6), *((PDDocGetOCGsSELPROTO)(gPDModelHFT[PDDocGetOCGsSEL])))
#define PDDocReplaceOCG (ACROASSERT(gPDModelVersion >= PDModelHFT_VERSION_6), *((PDDocReplaceOCGSELPROTO)(gPDModelHFT[PDDocReplaceOCGSEL])))
#define PDOCGSetUserOverride (ACROASSERT(gPDModelVersion >= PDModelHFT_VERSION_6), *((PDOCGSetUserOverrideSELPROTO)(gPDModelHFT[PDOCGSetUserOverrideSEL])))
#define PDOCGGetUserOverride (ACROASSERT(gPDModelVersion >= PDModelHFT_VERSION_6), *((PDOCGGetUserOverrideSELPROTO)(gPDModelHFT[PDOCGGetUserOverrideSEL])))
#define PDOCContextClearAllUserOverrides (ACROASSERT(gPDModelVersion >= PDModelHFT_VERSION_6), *((PDOCContextClearAllUserOverridesSELPROTO)(gPDModelHFT[PDOCContextClearAllUserOverridesSEL])))
#define PDPageFlattenOC (ACROASSERT(gPDModelVersion >= PDModelHFT_VERSION_6), *((PDPageFlattenOCSELPROTO)(gPDModelHFT[PDPageFlattenOCSEL])))
#define PDDocFlattenOC (ACROASSERT(gPDModelVersion >= PDModelHFT_VERSION_6), *((PDDocFlattenOCSELPROTO)(gPDModelHFT[PDDocFlattenOCSEL])))
/* END Optional Content API calls */

/* BEGIN Extensible PD Draw / Enum calls */
#if !MAC_PLATFORM || (MAC_PLATFORM && !AS_ARCH_64BIT)
#define PDPageDrawContentsWithParams (ACROASSERT(gPDModelVersion >= PDModelHFT_VERSION_6), *((PDPageDrawContentsWithParamsSELPROTO)(gPDModelHFT[PDPageDrawContentsWithParamsSEL])))
#define PDDrawCosObjWithParams (ACROASSERT(gPDModelVersion >= PDModelHFT_VERSION_6), *((PDDrawCosObjWithParamsSELPROTO)(gPDModelHFT[PDDrawCosObjWithParamsSEL])))
#endif /* #if !MAC_PLATFORM || (MAC_PLATFORM && !AS_ARCH_64BIT) */
#define PDFormEnumPaintProcWithParams (ACROASSERT(gPDModelVersion >= PDModelHFT_VERSION_6), *((PDFormEnumPaintProcWithParamsSELPROTO)(gPDModelHFT[PDFormEnumPaintProcWithParamsSEL])))
#define PDCharProcEnumWithParams (ACROASSERT(gPDModelVersion >= PDModelHFT_VERSION_6), *((PDCharProcEnumWithParamsSELPROTO)(gPDModelHFT[PDCharProcEnumWithParamsSEL])))
/* END Extensible PD Draw / Enum calls */

#define PDWordFinderAcquireVisibleWordList (ACROASSERT(gPDModelVersion >=PDModelHFT_VERSION_6), *((PDWordFinderAcquireVisibleWordListSELPROTO)(gPDModelHFT[PDWordFinderAcquireVisibleWordListSEL])))
#define PDWordIsCurrentlyVisible (ACROASSERT(gPDModelVersion >=PDModelHFT_VERSION_6), *((PDWordIsCurrentlyVisibleSELPROTO)(gPDModelHFT[PDWordIsCurrentlyVisibleSEL])))
#define PDWordMakeVisible (ACROASSERT(gPDModelVersion >=PDModelHFT_VERSION_6), *((PDWordMakeVisibleSELPROTO)(gPDModelHFT[PDWordMakeVisibleSEL])))
#define PDWordFinderEnumVisibleWords (ACROASSERT(gPDModelVersion >=PDModelHFT_VERSION_6), *((PDWordFinderEnumVisibleWordsSELPROTO)(gPDModelHFT[PDWordFinderEnumVisibleWordsSEL])))

/* PDPageGetVisibleBBox */
#define PDPageGetVisibleBBox (ACROASSERT(gPDModelVersion >=PDModelHFT_VERSION_6), *((PDPageGetVisibleBBoxSELPROTO)(gPDModelHFT[PDPageGetVisibleBBoxSEL])))

/* Crypt Filter support */
#define PDDocSetNewCryptFilterMethod (ACROASSERT(gPDModelVersion >=PDModelHFT_VERSION_6), *((PDDocSetNewCryptFilterMethodSELPROTO)(gPDModelHFT[PDDocSetNewCryptFilterMethodSEL])))
#define PDDocSetNewCryptFilterData (ACROASSERT(gPDModelVersion >=PDModelHFT_VERSION_6), *((PDDocSetNewCryptFilterDataSELPROTO)(gPDModelHFT[PDDocSetNewCryptFilterDataSEL])))
#define PDDocSetNewDefaultFilters (ACROASSERT(gPDModelVersion >=PDModelHFT_VERSION_6), *((PDDocSetNewDefaultFiltersSELPROTO)(gPDModelHFT[PDDocSetNewDefaultFiltersSEL])))
#define PDCryptAuthorizeFilterAccess (ACROASSERT(gPDModelVersion >=PDModelHFT_VERSION_6), *((PDCryptAuthorizeFilterAccessSELPROTO)(gPDModelHFT[PDCryptAuthorizeFilterAccessSEL])))

/* PDDocRequestPages and PDDocRequestEntireFile */
#define PDDocRequestPages (ACROASSERT(gPDModelVersion >=PDModelHFT_VERSION_6), *((PDDocRequestPagesSELPROTO)(gPDModelHFT[PDDocRequestPagesSEL])))
#define PDDocRequestEntireFile (ACROASSERT(gPDModelVersion >=PDModelHFT_VERSION_6), *((PDDocRequestEntireFileSELPROTO)(gPDModelHFT[PDDocRequestEntireFileSEL])))

#define PDSetHostEncoding (ACROASSERT(gPDModelVersion >= PDModelHFT_VERSION_6), *((PDSetHostEncodingSELPROTO)(gPDModelHFT[PDSetHostEncodingSEL])))

/*PDDocReadAheadEmbeddedFile */
#define PDDocReadAheadEmbeddedFile (ACROASSERT(gPDModelVersion >= PDModelHFT_VERSION_6), *((PDDocReadAheadEmbeddedFileSELPROTO)(gPDModelHFT[PDDocReadAheadEmbeddedFileSEL])))

/* PDDocGetTrapped and PDDocSetTrapped */
#define PDDocGetTrapped (ACROASSERT(gPDModelVersion >= PDModelHFT_VERSION_6), *((PDDocGetTrappedSELPROTO)(gPDModelHFT[PDDocGetTrappedSEL])))
#define PDDocSetTrapped (ACROASSERT(gPDModelVersion >= PDModelHFT_VERSION_6), *((PDDocSetTrappedSELPROTO)(gPDModelHFT[PDDocSetTrappedSEL])))

/* PDDocGetLabelForPageNumEx and PDDocFindPageNumForLabelEx */
#define PDDocGetLabelForPageNumEx (ACROASSERT(gPDModelVersion >=PDModelHFT_VERSION_6), *((PDDocGetLabelForPageNumExSELPROTO)(gPDModelHFT[PDDocGetLabelForPageNumExSEL])))
#define PDDocFindPageNumForLabelEx (ACROASSERT(gPDModelVersion >=PDModelHFT_VERSION_6), *((PDDocFindPageNumForLabelExSELPROTO)(gPDModelHFT[PDDocFindPageNumForLabelExSEL])))

/* PDFontGetASTextName */
#define PDFontGetASTextName (ACROASSERT(gPDModelVersion >=PDModelHFT_VERSION_6), *((PDFontGetASTextNameSELPROTO)(gPDModelHFT[PDFontGetASTextNameSEL])))

/* PDPageAcquirePage */
#define PDPageAcquirePage (ACROASSERT(gPDModelVersion >=PDModelHFT_VERSION_7), *((PDPageAcquirePageSELPROTO)(gPDModelHFT[PDPageAcquirePageSEL])))

/* PDOCGGetLocked, -SetLocked, PDOCConfigGetLockedArray, -SetLockedArray */
#define PDOCGGetLocked (ACROASSERT(gPDModelVersion >= PDModelHFT_VERSION_7), *((PDOCGGetLockedSELPROTO)(gPDModelHFT[PDOCGGetLockedSEL])))
#define PDOCGSetLocked (ACROASSERT(gPDModelVersion >= PDModelHFT_VERSION_7), *((PDOCGSetLockedSELPROTO)(gPDModelHFT[PDOCGSetLockedSEL])))
#define PDOCConfigGetLockedArray (ACROASSERT(gPDModelVersion >= PDModelHFT_VERSION_7), *((PDOCConfigGetLockedArraySELPROTO)(gPDModelHFT[PDOCConfigGetLockedArraySEL])))
#define PDOCConfigSetLockedArray (ACROASSERT(gPDModelVersion >= PDModelHFT_VERSION_7), *((PDOCConfigSetLockedArraySELPROTO)(gPDModelHFT[PDOCConfigSetLockedArraySEL])))

/* PDOCMDFindOrCreateEx */
#define PDOCMDFindOrCreateEx (ACROASSERT(gPDModelVersion >= PDModelHFT_VERSION_7), *((PDOCMDFindOrCreateExSELPROTO)(gPDModelHFT[PDOCMDFindOrCreateExSEL])))
#define PDOCMDGetVisibilityExpression (ACROASSERT(gPDModelVersion >= PDModelHFT_VERSION_7), *((PDOCMDGetVisibilityExpressionSELPROTO)(gPDModelHFT[PDOCMDGetVisibilityExpressionSEL])))


/* PDPageGetUserUnitSize */
#define PDPageGetUserUnitSize (ACROASSERT(gPDModelVersion >=PDModelHFT_VERSION_7), *((PDPageGetUserUnitSizeSELPROTO)(gPDModelHFT[PDPageGetUserUnitSizeSEL])))

/* PDPageSetUserUnitSize */
#define PDPageSetUserUnitSize (ACROASSERT(gPDModelVersion >=PDModelHFT_VERSION_7), *((PDPageSetUserUnitSizeSELPROTO)(gPDModelHFT[PDPageSetUserUnitSizeSEL])))

#define PDDocPermRequestNoUB (ACROASSERT(gPDModelVersion >=PDModelHFT_VERSION_7), *((PDDocPermRequestNoUBSELPROTO)(gPDModelHFT[PDDocPermRequestNoUBSEL])))

/* PDPageEnumInksEx */
#define PDPageEnumInksEx (ACROASSERT(gPDModelVersion >=PDModelHFT_VERSION_7), *((PDPageEnumInksExSELPROTO)(gPDModelHFT[PDPageEnumInksExSEL])))

#define PDDocAddWatermarkFromPDPage (ACROASSERT(gPDModelVersion >= PDModelHFT_VERSION_7), *((PDDocAddWatermarkFromPDPageSELPROTO)(gPDModelHFT[PDDocAddWatermarkFromPDPageSEL])))
#define PDDocAddWatermarkFromText (ACROASSERT(gPDModelVersion >= PDModelHFT_VERSION_7), *((PDDocAddWatermarkFromTextSELPROTO)(gPDModelHFT[PDDocAddWatermarkFromTextSEL])))




/* PDDocGetLayoutMode */
#define PDDocGetLayoutMode (ACROASSERT(gPDModelVersion >=PDModelHFT_VERSION_7), *((PDDocGetLayoutModeSELPROTO)(gPDModelHFT[PDDocGetLayoutModeSEL])))

/* PDDocSetLayoutMode */
#define PDDocSetLayoutMode (ACROASSERT(gPDModelVersion >=PDModelHFT_VERSION_7), *((PDDocSetLayoutModeSELPROTO)(gPDModelHFT[PDDocSetLayoutModeSEL])))

/* PDDocGetCryptHandler */
#define PDDocGetCryptHandler (ACROASSERT(gPDModelVersion >=PDModelHFT_VERSION_7), *((PDDocGetCryptHandlerSELPROTO)(gPDModelHFT[PDDocGetCryptHandlerSEL])))


/* PDFileSpecNewFromASPathEx */
#define PDFileSpecNewFromASPathEx (ACROASSERT(gPDModelVersion >=PDModelHFT_VERSION_8), *((PDFileSpecNewFromASPathExSELPROTO)(gPDModelHFT[PDFileSpecNewFromASPathExSEL])))

/* PDFileSpecAcquireASPathEx */
#define PDFileSpecAcquireASPathEx (ACROASSERT(gPDModelVersion >=PDModelHFT_VERSION_8), *((PDFileSpecAcquireASPathExSELPROTO)(gPDModelHFT[PDFileSpecAcquireASPathExSEL])))

/* PDFileSpecGetDIPathEx */
#define PDFileSpecGetDIPathEx (ACROASSERT(gPDModelVersion >=PDModelHFT_VERSION_8), *((PDFileSpecGetDIPathExSELPROTO)(gPDModelHFT[PDFileSpecGetDIPathExSEL])))

#if !defined (_H_PDFLPrivCalls)
/* PDThumbGetImageData */
#define PDThumbGetImageData (ACROASSERT(gPDModelVersion >= PDModelHFT_VERSION_7_5), \
 			(*((PDThumbGetImageDataSELPROTO)(gPDModelHFT[PDThumbGetImageDataSEL]))))
 
 /* PDThumbGetIndexedColorSpace */
 #define PDThumbGetIndexedColorSpace (ACROASSERT(gPDModelVersion >= PDModelHFT_VERSION_7_5), \
 			(*((PDThumbGetIndexedColorSpaceSELPROTO)(gPDModelHFT[PDThumbGetIndexedColorSpaceSEL]))))
 #endif

/* Additional ASText APIs for Acrobat 8.0 */

/* Bookmark APIs */
/* PDBookmarkAddNewChild */
#define PDBookmarkAddNewChildASText (ACROASSERT(gPDModelVersion >=PDModelHFT_VERSION_8), *((PDBookmarkAddNewChildASTextSELPROTO)(gPDModelHFT[PDBookmarkAddNewChildASTextSEL])))
/* PDBookmarkAddNewSiblingASText */
#define PDBookmarkAddNewSiblingASText (ACROASSERT(gPDModelVersion >=PDModelHFT_VERSION_8), *((PDBookmarkAddNewSiblingASTextSELPROTO)(gPDModelHFT[PDBookmarkAddNewSiblingASTextSEL])))
/* PDBookmarkAddSubtreeASText */
#define PDBookmarkAddSubtreeASText (ACROASSERT(gPDModelVersion >=PDModelHFT_VERSION_8), *((PDBookmarkAddSubtreeASTextSELPROTO)(gPDModelHFT[PDBookmarkAddSubtreeASTextSEL])))
/* PDBookmarkGetTitleASText */
#define PDBookmarkGetTitleASText (ACROASSERT(gPDModelVersion >=PDModelHFT_VERSION_8), *((PDBookmarkGetTitleASTextSELPROTO)(gPDModelHFT[PDBookmarkGetTitleASTextSEL])))
/* PDBookmarkSetTitleASText */
#define PDBookmarkSetTitleASText (ACROASSERT(gPDModelVersion >=PDModelHFT_VERSION_8), *((PDBookmarkSetTitleASTextSELPROTO)(gPDModelHFT[PDBookmarkSetTitleASTextSEL])))
/* PDBookmarkGetByTitleASText */
#define PDBookmarkGetByTitleASText (ACROASSERT(gPDModelVersion >=PDModelHFT_VERSION_8), *((PDBookmarkGetByTitleASTextSELPROTO)(gPDModelHFT[PDBookmarkGetByTitleASTextSEL])))

/* Annotation APIs */
/* PDAnnotGetTitleASText */
#define PDAnnotGetTitleASText (ACROASSERT(gPDModelVersion >=PDModelHFT_VERSION_8), *((PDAnnotGetTitleASTextSELPROTO)(gPDModelHFT[PDAnnotGetTitleASTextSEL])))
/* PDAnnotSetTitleASText */
#define PDAnnotSetTitleASText (ACROASSERT(gPDModelVersion >=PDModelHFT_VERSION_8), *((PDAnnotSetTitleASTextSELPROTO)(gPDModelHFT[PDAnnotSetTitleASTextSEL])))
/* PDTextAnnotGetContentsASText */
#define PDTextAnnotGetContentsASText (ACROASSERT(gPDModelVersion >=PDModelHFT_VERSION_8), *((PDTextAnnotGetContentsASTextSELPROTO)(gPDModelHFT[PDTextAnnotGetContentsASTextSEL])))
/* PDTextAnnotSetContentsASText */
#define PDTextAnnotSetContentsASText (ACROASSERT(gPDModelVersion >=PDModelHFT_VERSION_8), *((PDTextAnnotSetContentsASTextSELPROTO)(gPDModelHFT[PDTextAnnotSetContentsASTextSEL])))

/* DocInfo APIs */
/* PDDocGetInfoASText */
#define PDDocGetInfoASText (ACROASSERT(gPDModelVersion >=PDModelHFT_VERSION_8), *((PDDocGetInfoASTextSELPROTO)(gPDModelHFT[PDDocGetInfoASTextSEL])))
/* PDDocSetInfoAsASText */
#define PDDocSetInfoAsASText (ACROASSERT(gPDModelVersion >=PDModelHFT_VERSION_8), *((PDDocSetInfoAsASTextSELPROTO)(gPDModelHFT[PDDocSetInfoAsASTextSEL])))


/* PageLabel APIs */
#define PDPageLabelGetPrefixASText (ACROASSERT(gPDModelVersion >=PDModelHFT_VERSION_8), *((PDPageLabelGetPrefixASTextSELPROTO)(gPDModelHFT[PDPageLabelGetPrefixASTextSEL])))
#define PDPageLabelNewASText (ACROASSERT(gPDModelVersion >=PDModelHFT_VERSION_8), *((PDPageLabelNewASTextSELPROTO)(gPDModelHFT[PDPageLabelNewASTextSEL])))

/* PDThread APIs */
/* PDThreadGetInfoASText */
#define PDThreadGetInfoASText (ACROASSERT(gPDModelVersion >=PDModelHFT_VERSION_8), *((PDThreadGetInfoASTextSELPROTO)(gPDModelHFT[PDThreadGetInfoASTextSEL])))
/* PDThreadSetInfoASText */
#define PDThreadSetInfoASText (ACROASSERT(gPDModelVersion >=PDModelHFT_VERSION_8), *((PDThreadSetInfoASTextSELPROTO)(gPDModelHFT[PDThreadSetInfoASTextSEL])))

/* PDXlateToHostASText */
#define PDXlateToHostASText (ACROASSERT(gPDModelVersion >=PDModelHFT_VERSION_8), *((PDXlateToHostASTextSELPROTO)(gPDModelHFT[PDXlateToHostASTextSEL])))
/* PDXlateToASText */
#define PDXlateToASText (ACROASSERT(gPDModelVersion >=PDModelHFT_VERSION_8), *((PDXlateToASTextSELPROTO)(gPDModelHFT[PDXlateToASTextSEL])))

/* PDPageHasOverprintExt */
#define PDPageHasOverprintExt (ACROASSERT(gPDModelVersion >=PDModelHFT_VERSION_8), *((PDPageHasOverprintExtSELPROTO)(gPDModelHFT[PDPageHasOverprintExtSEL])))

/* PDDocSetMinorVersion */
#define PDDocSetMinorVersion (ACROASSERT(gPDModelVersion >= PDModelHFT_VERSION_8), *((PDDocSetMinorVersionSELPROTO) (gPDModelHFT[PDDocSetMinorVersionSEL])))

#if ACRO_SDK_LEVEL >= 0x00080000 /* Previously in PDFL HFT */
/* PDPrefSetUseOutputIntents */
#define PDPrefSetUseOutputIntents  (ACROASSERT(gPDModelVersion >=PDModelHFT_VERSION_8), *((PDPrefSetUseOutputIntentsSELPROTO)(gPDModelHFT[PDPrefSetUseOutputIntentsSEL])))

/* PDPrefGetUseOutputIntents */
#define PDPrefGetUseOutputIntents  (ACROASSERT(gPDModelVersion >=PDModelHFT_VERSION_8), *((PDPrefGetUseOutputIntentsSELPROTO)(gPDModelHFT[PDPrefGetUseOutputIntentsSEL])))

/* PDPrefSetWorkingRGB */
#define PDPrefSetWorkingRGB (ACROASSERT(gPDModelVersion >=PDModelHFT_VERSION_8), *((PDPrefSetWorkingRGBSELPROTO)(gPDModelHFT[PDPrefSetWorkingRGBSEL])))

/* PDPrefSetWorkingCMYK */
#define PDPrefSetWorkingCMYK (ACROASSERT(gPDModelVersion >=PDModelHFT_VERSION_8), *((PDPrefSetWorkingCMYKSELPROTO)(gPDModelHFT[PDPrefSetWorkingCMYKSEL])))

/* PDPrefSetWorkingGray */
#define PDPrefSetWorkingGray (ACROASSERT(gPDModelVersion >=PDModelHFT_VERSION_8), *((PDPrefSetWorkingGraySELPROTO)(gPDModelHFT[PDPrefSetWorkingGraySEL])))

/* PDPrefSetBlackPointCompensation */
#define PDPrefSetBlackPointCompensation (ACROASSERT(gPDModelVersion >=PDModelHFT_VERSION_8), *((PDPrefSetBlackPointCompensationSELPROTO)(gPDModelHFT[PDPrefSetBlackPointCompensationSEL])))

/* PDPrefGetBlackPointCompensation */
#define PDPrefGetBlackPointCompensation (ACROASSERT(gPDModelVersion >=PDModelHFT_VERSION_8), *((PDPrefGetBlackPointCompensationSELPROTO)(gPDModelHFT[PDPrefGetBlackPointCompensationSEL])))
#endif /* if ACRO_SDK_LEVEL >= 0x00080000 */
    
/* If you add, delete, or modify procs listed in this file please read the instructions at the top about how to properly change the version number */

/* Redaction APIs */
/* PDDocApplyRedactions */
#define PDDocApplyRedactions (ACROASSERT(gPDModelVersion >=PDModelHFT_VERSION_9), *((PDDocApplyRedactionsSELPROTO)(gPDModelHFT[PDDocApplyRedactionsSEL])))
/* PDPageCreateRedaction */
#define PDDocCreateRedaction (ACROASSERT(gPDModelVersion >=PDModelHFT_VERSION_9), *((PDDocCreateRedactionSELPROTO)(gPDModelHFT[PDDocCreateRedactionSEL])))
/* PDRedactionGetProps */
#define PDRedactionGetProps (ACROASSERT(gPDModelVersion >=PDModelHFT_VERSION_9), *((PDRedactionGetPropsSELPROTO)(gPDModelHFT[PDRedactionGetPropsSEL])))
/* PDRedactionGetProps */
#define PDRedactionSetProps (ACROASSERT(gPDModelVersion >=PDModelHFT_VERSION_9), *((PDRedactionSetPropsSELPROTO)(gPDModelHFT[PDRedactionSetPropsSEL])))

/* PDDocResetInkUsage */
#define PDDocResetInkUsage (ACROASSERT(gPDModelVersion >=PDModelHFT_VERSION_9), *((PDDocResetInkUsageSELPROTO)(gPDModelHFT[PDDocResetInkUsageSEL])))

/* PDDocGetNumErrors */
#define PDDocGetNumErrors (ACROASSERT(gPDModelVersion >=PDModelHFT_VERSION_9), *((PDDocGetNumErrorsSELPROTO)(gPDModelHFT[PDDocGetNumErrorsSEL])))
/* PDDocGetNthError */
#define PDDocGetNthError (ACROASSERT(gPDModelVersion >=PDModelHFT_VERSION_9), *((PDDocGetNthErrorSELPROTO)(gPDModelHFT[PDDocGetNthErrorSEL])))
/* PDDocGetVersionEx */
#define PDDocGetVersionEx (ACROASSERT(gPDModelVersion >=PDModelHFT_VERSION_9), *((PDDocGetVersionExSELPROTO)(gPDModelHFT[PDDocGetVersionExSEL])))

#define PDFileAttachmentNewFromFile (ACROASSERT(gPDModelVersion >=PDModelHFT_VERSION_9), *((PDFileAttachmentNewFromFileSELPROTO)(gPDModelHFT[PDFileAttachmentNewFromFileSEL])))
#define PDFileAttachmentUpdateFromFile (ACROASSERT(gPDModelVersion >=PDModelHFT_VERSION_9), *((PDFileAttachmentUpdateFromFileSELPROTO)(gPDModelHFT[PDFileAttachmentUpdateFromFileSEL])))
#define PDFileAttachmentSaveToFile (ACROASSERT(gPDModelVersion >=PDModelHFT_VERSION_9), *((PDFileAttachmentSaveToFileSELPROTO)(gPDModelHFT[PDFileAttachmentSaveToFileSEL])))
#define PDFileAttachmentFromCosObj (ACROASSERT(gPDModelVersion >=PDModelHFT_VERSION_9), *((PDFileAttachmentFromCosObjSELPROTO)(gPDModelHFT[PDFileAttachmentFromCosObjSEL])))
#define PDFileAttachmentGetCosObj (ACROASSERT(gPDModelVersion >=PDModelHFT_VERSION_9), *((PDFileAttachmentGetCosObjSELPROTO)(gPDModelHFT[PDFileAttachmentGetCosObjSEL])))
#define PDFileAttachmentOpenStream (ACROASSERT(gPDModelVersion >=PDModelHFT_VERSION_9), *((PDFileAttachmentOpenStreamSELPROTO)(gPDModelHFT[PDFileAttachmentOpenStreamSEL])))
#define PDFileAttachmentGetFileSize (ACROASSERT(gPDModelVersion >=PDModelHFT_VERSION_9), *((PDFileAttachmentGetFileSizeSELPROTO)(gPDModelHFT[PDFileAttachmentGetFileSizeSEL])))
#define PDFileAttachmentGetCreationDate (ACROASSERT(gPDModelVersion >=PDModelHFT_VERSION_9), *((PDFileAttachmentGetCreationDateSELPROTO)(gPDModelHFT[PDFileAttachmentGetCreationDateSEL])))
#define PDFileAttachmentGetModDate (ACROASSERT(gPDModelVersion >=PDModelHFT_VERSION_9), *((PDFileAttachmentGetModDateSELPROTO)(gPDModelHFT[PDFileAttachmentGetModDateSEL])))
#define PDFileAttachmentIsValid (ACROASSERT(gPDModelVersion >=PDModelHFT_VERSION_9), *((PDFileAttachmentIsValidSELPROTO)(gPDModelHFT[PDFileAttachmentIsValidSEL])))
#define PDFileAttachmentGetFileName (ACROASSERT(gPDModelVersion >=PDModelHFT_VERSION_9), *((PDFileAttachmentGetFileNameSELPROTO)(gPDModelHFT[PDFileAttachmentGetFileNameSEL])))
#define PDFileAttachmentSetFieldText (ACROASSERT(gPDModelVersion >=PDModelHFT_VERSION_9), *((PDFileAttachmentSetFieldTextSELPROTO)(gPDModelHFT[PDFileAttachmentSetFieldTextSEL])))
#define PDFileAttachmentGetFieldText (ACROASSERT(gPDModelVersion >=PDModelHFT_VERSION_9), *((PDFileAttachmentGetFieldTextSELPROTO)(gPDModelHFT[PDFileAttachmentGetFieldTextSEL])))
#define PDFileAttachmentSetFieldNumber (ACROASSERT(gPDModelVersion >=PDModelHFT_VERSION_9), *((PDFileAttachmentSetFieldNumberSELPROTO)(gPDModelHFT[PDFileAttachmentSetFieldNumberSEL])))
#define PDFileAttachmentGetFieldNumber (ACROASSERT(gPDModelVersion >=PDModelHFT_VERSION_9), *((PDFileAttachmentGetFieldNumberSELPROTO)(gPDModelHFT[PDFileAttachmentGetFieldNumberSEL])))
#define PDFileAttachmentSetFieldDate (ACROASSERT(gPDModelVersion >=PDModelHFT_VERSION_9), *((PDFileAttachmentSetFieldDateSELPROTO)(gPDModelHFT[PDFileAttachmentSetFieldDateSEL])))
#define PDFileAttachmentGetFieldDate (ACROASSERT(gPDModelVersion >=PDModelHFT_VERSION_9), *((PDFileAttachmentGetFieldDateSELPROTO)(gPDModelHFT[PDFileAttachmentGetFieldDateSEL])))
#define PDFileAttachmentSetFieldPrefix (ACROASSERT(gPDModelVersion >=PDModelHFT_VERSION_9), *((PDFileAttachmentSetFieldPrefixSELPROTO)(gPDModelHFT[PDFileAttachmentSetFieldPrefixSEL])))
#define PDFileAttachmentGetFieldPrefix (ACROASSERT(gPDModelVersion >=PDModelHFT_VERSION_9), *((PDFileAttachmentGetFieldPrefixSELPROTO)(gPDModelHFT[PDFileAttachmentGetFieldPrefixSEL])))

#define PDCollectionIsValid (ACROASSERT(gPDModelVersion >=PDModelHFT_VERSION_9), *((PDCollectionIsValidSELPROTO)(gPDModelHFT[PDCollectionIsValidSEL])))
#define PDDocGetPDCollection (ACROASSERT(gPDModelVersion >=PDModelHFT_VERSION_9), *((PDDocGetPDCollectionSELPROTO)(gPDModelHFT[PDDocGetPDCollectionSEL])))
#define PDDocCreatePDCollection (ACROASSERT(gPDModelVersion >=PDModelHFT_VERSION_9), *((PDDocCreatePDCollectionSELPROTO)(gPDModelHFT[PDDocCreatePDCollectionSEL])))
#define PDDocDeleteCollection (ACROASSERT(gPDModelVersion >=PDModelHFT_VERSION_9), *((PDDocDeleteCollectionSELPROTO)(gPDModelHFT[PDDocDeleteCollectionSEL])))
#define PDCollectionGetSortOrder (ACROASSERT(gPDModelVersion >=PDModelHFT_VERSION_9), *((PDCollectionGetSortOrderSELPROTO)(gPDModelHFT[PDCollectionGetSortOrderSEL])))
#define PDCollectionSetSortOrder (ACROASSERT(gPDModelVersion >=PDModelHFT_VERSION_9), *((PDCollectionSetSortOrderSELPROTO)(gPDModelHFT[PDCollectionSetSortOrderSEL])))
#define PDCollectionGetViewData (ACROASSERT(gPDModelVersion >=PDModelHFT_VERSION_9), *((PDCollectionGetViewDataSELPROTO)(gPDModelHFT[PDCollectionGetViewDataSEL])))
#define PDCollectionSetViewData (ACROASSERT(gPDModelVersion >=PDModelHFT_VERSION_9), *((PDCollectionSetViewDataSELPROTO)(gPDModelHFT[PDCollectionSetViewDataSEL])))

#define PDCollectionSchemaAcquire (ACROASSERT(gPDModelVersion >=PDModelHFT_VERSION_9), *((PDCollectionSchemaAcquireSELPROTO)(gPDModelHFT[PDCollectionSchemaAcquireSEL])))
#define PDCollectionSchemaDestroy (ACROASSERT(gPDModelVersion >=PDModelHFT_VERSION_9), *((PDCollectionSchemaDestroySELPROTO)(gPDModelHFT[PDCollectionSchemaDestroySEL])))
#define PDCollectionSchemaGetLength (ACROASSERT(gPDModelVersion >=PDModelHFT_VERSION_9), *((PDCollectionSchemaGetLengthSELPROTO)(gPDModelHFT[PDCollectionSchemaGetLengthSEL])))
#define PDCollectionSchemaGetField (ACROASSERT(gPDModelVersion >=PDModelHFT_VERSION_9), *((PDCollectionSchemaGetFieldSELPROTO)(gPDModelHFT[PDCollectionSchemaGetFieldSEL])))
#define PDCollectionSchemaSetField (ACROASSERT(gPDModelVersion >=PDModelHFT_VERSION_9), *((PDCollectionSchemaSetFieldSELPROTO)(gPDModelHFT[PDCollectionSchemaSetFieldSEL])))
#define PDCollectionSchemaRemoveField (ACROASSERT(gPDModelVersion >=PDModelHFT_VERSION_9), *((PDCollectionSchemaRemoveFieldSELPROTO)(gPDModelHFT[PDCollectionSchemaRemoveFieldSEL])))

#define PDFolderIsValid (ACROASSERT(gPDModelVersion >=PDModelHFT_VERSION_9), *((PDFolderIsValidSELPROTO)(gPDModelHFT[PDFolderIsValidSEL])))
#define PDCollectionCreateFolder (ACROASSERT(gPDModelVersion >=PDModelHFT_VERSION_9), *((PDCollectionCreateFolderSELPROTO)(gPDModelHFT[PDCollectionCreateFolderSEL])))
#define PDCollectionRemoveFolder (ACROASSERT(gPDModelVersion >=PDModelHFT_VERSION_9), *((PDCollectionRemoveFolderSELPROTO)(gPDModelHFT[PDCollectionRemoveFolderSEL])))
#define PDCollectionGetFolder (ACROASSERT(gPDModelVersion >=PDModelHFT_VERSION_9), *((PDCollectionGetFolderSELPROTO)(gPDModelHFT[PDCollectionGetFolderSEL])))
#define PDFolderGetParent (ACROASSERT(gPDModelVersion >=PDModelHFT_VERSION_9), *((PDFolderGetParentSELPROTO)(gPDModelHFT[PDFolderGetParentSEL])))
#define PDFolderSetParent (ACROASSERT(gPDModelVersion >=PDModelHFT_VERSION_9), *((PDFolderSetParentSELPROTO)(gPDModelHFT[PDFolderSetParentSEL])))
#define PDFolderGetFirstChild (ACROASSERT(gPDModelVersion >=PDModelHFT_VERSION_9), *((PDFolderGetFirstChildSELPROTO)(gPDModelHFT[PDFolderGetFirstChildSEL])))
#define PDFolderGetNextSibling (ACROASSERT(gPDModelVersion >=PDModelHFT_VERSION_9), *((PDFolderGetNextSiblingSELPROTO)(gPDModelHFT[PDFolderGetNextSiblingSEL])))
#define PDFolderSetName (ACROASSERT(gPDModelVersion >=PDModelHFT_VERSION_9), *((PDFolderSetNameSELPROTO)(gPDModelHFT[PDFolderSetNameSEL])))
#define PDFolderGetName (ACROASSERT(gPDModelVersion >=PDModelHFT_VERSION_9), *((PDFolderGetNameSELPROTO)(gPDModelHFT[PDFolderGetNameSEL])))
#define PDFolderGetID (ACROASSERT(gPDModelVersion >=PDModelHFT_VERSION_9), *((PDFolderGetIDSELPROTO)(gPDModelHFT[PDFolderGetIDSEL])))
#define PDFolderGetPathText (ACROASSERT(gPDModelVersion >=PDModelHFT_VERSION_9), *((PDFolderGetPathTextSELPROTO)(gPDModelHFT[PDFolderGetPathTextSEL])))
#define PDFolderGetModDate (ACROASSERT(gPDModelVersion >=PDModelHFT_VERSION_9), *((PDFolderGetModDateSELPROTO)(gPDModelHFT[PDFolderGetModDateSEL])))
#define PDFolderSetModDate (ACROASSERT(gPDModelVersion >=PDModelHFT_VERSION_9), *((PDFolderSetModDateSELPROTO)(gPDModelHFT[PDFolderSetModDateSEL])))
#define PDFolderGetCreationDate (ACROASSERT(gPDModelVersion >=PDModelHFT_VERSION_9), *((PDFolderGetCreationDateSELPROTO)(gPDModelHFT[PDFolderGetCreationDateSEL])))
#define PDFolderSetCreationDate (ACROASSERT(gPDModelVersion >=PDModelHFT_VERSION_9), *((PDFolderSetCreationDateSELPROTO)(gPDModelHFT[PDFolderSetCreationDateSEL])))
#define PDFolderGetDescription (ACROASSERT(gPDModelVersion >=PDModelHFT_VERSION_9), *((PDFolderGetDescriptionSELPROTO)(gPDModelHFT[PDFolderGetDescriptionSEL])))
#define PDFolderSetDescription (ACROASSERT(gPDModelVersion >=PDModelHFT_VERSION_9), *((PDFolderSetDescriptionSELPROTO)(gPDModelHFT[PDFolderSetDescriptionSEL])))
#define PDFolderSetFieldText (ACROASSERT(gPDModelVersion >=PDModelHFT_VERSION_9), *((PDFolderSetFieldTextSELPROTO)(gPDModelHFT[PDFolderSetFieldTextSEL])))
#define PDFolderGetFieldText (ACROASSERT(gPDModelVersion >=PDModelHFT_VERSION_9), *((PDFolderGetFieldTextSELPROTO)(gPDModelHFT[PDFolderGetFieldTextSEL])))
#define PDFolderSetFieldNumber (ACROASSERT(gPDModelVersion >=PDModelHFT_VERSION_9), *((PDFolderSetFieldNumberSELPROTO)(gPDModelHFT[PDFolderSetFieldNumberSEL])))
#define PDFolderGetFieldNumber (ACROASSERT(gPDModelVersion >=PDModelHFT_VERSION_9), *((PDFolderGetFieldNumberSELPROTO)(gPDModelHFT[PDFolderGetFieldNumberSEL])))
#define PDFolderSetFieldDate (ACROASSERT(gPDModelVersion >=PDModelHFT_VERSION_9), *((PDFolderSetFieldDateSELPROTO)(gPDModelHFT[PDFolderSetFieldDateSEL])))
#define PDFolderGetFieldDate (ACROASSERT(gPDModelVersion >=PDModelHFT_VERSION_9), *((PDFolderGetFieldDateSELPROTO)(gPDModelHFT[PDFolderGetFieldDateSEL])))

#define ASFileAttachmentCreatePathName (ACROASSERT(gPDModelVersion >=PDModelHFT_VERSION_9), *((ASFileAttachmentCreatePathNameSELPROTO)(gPDModelHFT[ASFileAttachmentCreatePathNameSEL])))
#define ASFileAttachmentGetPDFileAttachment (ACROASSERT(gPDModelVersion >=PDModelHFT_VERSION_9), *((ASFileAttachmentGetPDFileAttachmentSELPROTO)(gPDModelHFT[ASFileAttachmentGetPDFileAttachmentSEL])))
#define ASFileAttachmentGetPDFolder (ACROASSERT(gPDModelVersion >=PDModelHFT_VERSION_9), *((ASFileAttachmentGetPDFolderSELPROTO)(gPDModelHFT[ASFileAttachmentGetPDFolderSEL])))

/* PDDocHasISOExtensions */
#define PDDocHasISOExtensions (ACROASSERT(gPDModelVersion >=PDModelHFT_VERSION_9), *((PDDocHasISOExtensionsSELPROTO)(gPDModelHFT[PDDocHasISOExtensionsSEL])))

#define PDApplyFunctionEx (ACROASSERT(gPDModelVersion >=PDModelHFT_VERSION_10), *((PDApplyFunctionExSELPROTO)(gPDModelHFT[PDApplyFunctionExSEL])))

#define PDDocGetCryptVersion (ACROASSERT(gPDModelVersion >=PDModelHFT_VERSION_10), *((PDDocGetCryptVersionSELPROTO)(gPDModelHFT[PDDocGetCryptVersionSEL])))
#define PDDocGetCryptRevision (ACROASSERT(gPDModelVersion >=PDModelHFT_VERSION_10), *((PDDocGetCryptRevisionSELPROTO)(gPDModelHFT[PDDocGetCryptRevisionSEL])))

/* Styled fields in collections */
#define PDFileAttachmentSetFieldStyledText (ACROASSERT(gPDModelVersion >=PDModelHFT_VERSION_10), *((PDFileAttachmentSetFieldStyledTextSELPROTO)(gPDModelHFT[PDFileAttachmentSetFieldStyledTextSEL])))
#define PDFileAttachmentGetFieldStyledText (ACROASSERT(gPDModelVersion >=PDModelHFT_VERSION_10), *((PDFileAttachmentGetFieldStyledTextSELPROTO)(gPDModelHFT[PDFileAttachmentGetFieldStyledTextSEL])))
#define PDFileAttachmentSetFieldStyle (ACROASSERT(gPDModelVersion >=PDModelHFT_VERSION_10), *((PDFileAttachmentSetFieldStyleSELPROTO)(gPDModelHFT[PDFileAttachmentSetFieldStyleSEL])))
#define PDFileAttachmentGetFieldStyle (ACROASSERT(gPDModelVersion >=PDModelHFT_VERSION_10), *((PDFileAttachmentGetFieldStyleSELPROTO)(gPDModelHFT[PDFileAttachmentGetFieldStyleSEL])))

#define PDFolderSetFieldStyledText (ACROASSERT(gPDModelVersion >=PDModelHFT_VERSION_10), *((PDFolderSetFieldStyledTextSELPROTO)(gPDModelHFT[PDFolderSetFieldStyledTextSEL])))
#define PDFolderGetFieldStyledText (ACROASSERT(gPDModelVersion >=PDModelHFT_VERSION_10), *((PDFolderGetFieldStyledTextSELPROTO)(gPDModelHFT[PDFolderGetFieldStyledTextSEL])))
#define PDFolderSetFieldStyle (ACROASSERT(gPDModelVersion >=PDModelHFT_VERSION_10), *((PDFolderSetFieldStyleSELPROTO)(gPDModelHFT[PDFolderSetFieldStyleSEL])))
#define PDFolderGetFieldStyle (ACROASSERT(gPDModelVersion >=PDModelHFT_VERSION_10), *((PDFolderGetFieldStyleSELPROTO)(gPDModelHFT[PDFolderGetFieldStyleSEL])))
#define PDFolderGetDescriptionStyled (ACROASSERT(gPDModelVersion >=PDModelHFT_VERSION_10), *((PDFolderGetDescriptionStyledSELPROTO)(gPDModelHFT[PDFolderGetDescriptionStyledSEL])))
#define PDFolderSetDescriptionStyled (ACROASSERT(gPDModelVersion >=PDModelHFT_VERSION_10), *((PDFolderSetDescriptionStyledSELPROTO)(gPDModelHFT[PDFolderSetDescriptionStyledSEL])))

#define PDCollectionSetInitialStyle (ACROASSERT(gPDModelVersion >=PDModelHFT_VERSION_10), *((PDCollectionSetInitialStyleSELPROTO)(gPDModelHFT[PDCollectionSetInitialStyleSEL])))
#define PDCollectionGetInitialStyle (ACROASSERT(gPDModelVersion >=PDModelHFT_VERSION_10), *((PDCollectionGetInitialStyleSELPROTO)(gPDModelHFT[PDCollectionGetInitialStyleSEL])))
#define PDCollectionRemoveInitialStyle (ACROASSERT(gPDModelVersion >=PDModelHFT_VERSION_10), *((PDCollectionRemoveInitialStyleSELPROTO)(gPDModelHFT[PDCollectionRemoveInitialStyleSEL])))

/* PDDocClearErrors */
#define PDDocClearErrors (ACROASSERT(gPDModelVersion >=PDModelHFT_VERSION_10), *((PDDocClearErrorsSELPROTO)(gPDModelHFT[PDDocClearErrorsSEL])))
#define PDPrefSetDefaultBlendingColorSpace (ACROASSERT(gPDModelVersion >=PDModelHFT_VERSION_11), *((PDPrefSetDefaultBlendingColorSpaceSELPROTO)(gPDModelHFT[PDPrefSetDefaultBlendingColorSpaceSEL])))
#define PDPrefGetDefaultBlendingColorSpace (ACROASSERT(gPDModelVersion >=PDModelHFT_VERSION_11), *((PDPrefGetDefaultBlendingColorSpaceSELPROTO)(gPDModelHFT[PDPrefGetDefaultBlendingColorSpaceSEL])))

/*	New in VERSION_12: Drawing API that support drawing either a whole page or a single stream CosObj to a buffer; can be used together with client-side
    platform code to replace PDPageDrawContentsToWindow and PDDrawCosObjToWindow which do not work on Mac any more; available on all supported platforms*/
#define PDDrawPageOrCosObjectToBuffer (ACROASSERT(gPDModelVersion >=PDModelHFT_VERSION_12), *((PDDrawPageOrCosObjectToBufferSELPROTO)(gPDModelHFT[PDDrawPageOrCosObjectToBufferSEL])))

#ifdef ANDROID_ENV
#define PDSetTempLocation (ACROASSERT(gPDModelVersion >= PDModelHFT_VERSION_12), *((PDSetTempLocationSELPROTO)(gPDModelHFT[PDSetTempLocationSEL])))
#define PDGetTempLocation (ACROASSERT(gPDModelVersion >= PDModelHFT_VERSION_12), *((PDGetTempLocationSELPROTO)(gPDModelHFT[PDGetTempLocationSEL])))
#endif
    
#define PDActionNewFromStructureDest (ACROASSERT(gPDModelVersion >=PDModelHFT_VERSION_13), *((PDActionNewFromStructureDestSELPROTO)(gPDModelHFT[PDActionNewFromStructureDestSEL])))
#define PDActionSetViewDest (ACROASSERT(gPDModelVersion >=PDModelHFT_VERSION_13), *((PDActionSetViewDestSELPROTO)(gPDModelHFT[PDActionSetViewDestSEL])))
#define PDActionSetStructureDest (ACROASSERT(gPDModelVersion >=PDModelHFT_VERSION_13), *((PDActionSetStructureDestSELPROTO)(gPDModelHFT[PDActionSetStructureDestSEL])))
#define PDActionGetStructureDest (ACROASSERT(gPDModelVersion >=PDModelHFT_VERSION_13), *((PDActionGetStructureDestSELPROTO)(gPDModelHFT[PDActionGetStructureDestSEL])))
#define PDStructureDestCreate (ACROASSERT(gPDModelVersion >=PDModelHFT_VERSION_13), *((PDStructureDestCreateSELPROTO)(gPDModelHFT[PDStructureDestCreateSEL])))
#define PDStructureDestDestroy (ACROASSERT(gPDModelVersion >=PDModelHFT_VERSION_13), *((PDStructureDestDestroySELPROTO)(gPDModelHFT[PDStructureDestDestroySEL])))
#define PDStructureDestIsValid (ACROASSERT(gPDModelVersion >=PDModelHFT_VERSION_13), *((PDStructureDestIsValidSELPROTO)(gPDModelHFT[PDStructureDestIsValidSEL])))
#define PDStructureDestGetStructureElement (ACROASSERT(gPDModelVersion >=PDModelHFT_VERSION_13), *((PDStructureDestGetStructureElementSELPROTO)(gPDModelHFT[PDStructureDestGetStructureElementSEL])))
#define PDStructureDestGetAttr (ACROASSERT(gPDModelVersion >=PDModelHFT_VERSION_13), *((PDStructureDestGetAttrSELPROTO)(gPDModelHFT[PDStructureDestGetAttrSEL])))
#define PDStructureDestGetCosObj (ACROASSERT(gPDModelVersion >=PDModelHFT_VERSION_13), *((PDStructureDestGetCosObjSELPROTO)(gPDModelHFT[PDStructureDestGetCosObjSEL])))
#define PDStructureDestFromCosObj (ACROASSERT(gPDModelVersion >=PDModelHFT_VERSION_13), *((PDStructureDestFromCosObjSELPROTO)(gPDModelHFT[PDStructureDestFromCosObjSEL])))

#define PDFileAttachmentGetRelationshipText (ACROASSERT(gPDModelVersion >=PDModelHFT_VERSION_13), *((PDFileAttachmentGetRelationshipTextSELPROTO)(gPDModelHFT[PDFileAttachmentGetRelationshipTextSEL])))
#define PDFileAttachmentSetRelationshipText (ACROASSERT(gPDModelVersion >=PDModelHFT_VERSION_13), *((PDFileAttachmentSetRelationshipTextSELPROTO)(gPDModelHFT[PDFileAttachmentSetRelationshipTextSEL])))

#define PDDocSubstituteType3Fonts (ACROASSERT(gPDModelVersion >=PDModelHFT_VERSION_13), *((PDDocSubstituteType3FontsSELPROTO)(gPDModelHFT[PDDocSubstituteType3FontsSEL])))

/* PDPrefSetInstallPatternParentGState */
#define PDPrefSetInstallPatternParentGState (ACROASSERT(gPDModelVersion >=PDModelHFT_VERSION_13), *((PDPrefSetInstallPatternParentGStateSELPROTO)(gPDModelHFT[PDPrefSetInstallPatternParentGStateSEL])))
/* PDPrefGetInstallPatternParentGState */
#define PDPrefGetInstallPatternParentGState (ACROASSERT(gPDModelVersion >=PDModelHFT_VERSION_13), *((PDPrefGetInstallPatternParentGStateSELPROTO)(gPDModelHFT[PDPrefGetInstallPatternParentGStateSEL])))

#endif /* !STATIC_HFT */
#endif /* PI_PDMODEL_VERSION != 0 */
#endif /* PLUGIN */

#ifdef __cplusplus
}
#endif

#undef EXTERNAL_PDPROCS_USER
#endif /* !defined(_H_PDCalls) */

