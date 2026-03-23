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

 PEWCalls.h

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
   ~HFT_LATEST_VERSION.  For example, suppose the last release of Acrobat was version 4, version 5
   is under development, and you add a new routine for version 5.  Increment _~HFT_LATEST_VERSION 
   to 0x00040001 and set _~_IS_BETA to 1.  Add "#define ~HFT_VERSION_5 ~HFT_LATEST_VERSION".  
   Add your routine and assert that g~Version >= ~HFT_VERSION_5.  Leave 
   _~_LAST_BETA_COMPATIBLE_VERSION unchanged (0x00040000 in this example).

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
   the _~HFT_LATEST_VERSION to the final version (0x00050000 in our example), do the same for
   _~_LAST_BETA_COMPATIBLE_VERSION, and set the 
   ~HFT_VERSION_# to the final version number (0x00050000 in this example).  Once the HFT
   has left beta, the routines cannot be changed and a new beta must ensue (beta for version
   6 in this example).

*********************************************************************/

#ifndef _H_PEWCalls
#define _H_PEWCalls
#include "acroassert.h"
#ifdef THREAD_SAFE_PDFL
#include "PDFLInitCommon.h"
#endif /* defined THREAD_SAFE_PDFL */

/* for Adobe use only */
#define _PDFEditWriteHFT_LATEST_VERSION 0x000B0000
#define _PDFEditWriteHFT_LAST_BETA_COMPATIBLE_VERSION 0x000B0000
#define _PDFEditWriteHFT_IS_BETA 0

/* for public use */
#define PDFEditWriteHFT_LATEST_VERSION (_PDFEditWriteHFT_IS_BETA ? (kHFT_IN_BETA_FLAG | _PDFEditWriteHFT_LATEST_VERSION) : _PDFEditWriteHFT_LATEST_VERSION)

#define PDFEditWriteHFT_VERSION_4   0x00040000
#define PDFEditWriteHFT_VERSION_5   0x00050000
#define PDFEditWriteHFT_VERSION_6   0x00060000
#define PDFEditWriteHFT_VERSION_7   0x00070000
#define PDFEditWriteHFT_VERSION_7_5 0x00070005
#define PDFEditWriteHFT_VERSION_8	0x00080000
#define PDFEditWriteHFT_VERSION_9	0x00090000
#define PDFEditWriteHFT_VERSION_10	0x000A0000
#define PDFEditWriteHFT_VERSION_11	PDFEditWriteHFT_LATEST_VERSION

#include "PDBasicExpT.h"
#include "PEExpT.h"
#include "PEVers.h"

#ifdef __cplusplus
extern "C" {
#endif

#ifdef NPROC /* This might be defined in sys/procs.h */
#undef NPROC
#endif /* NPROC */


#if !PLUGIN
	/* Static link */
	#define NPROC(returnType, name, params)		\
		ACEX1 returnType ACEX2 name params;
	#define UNPROC NPROC
#if !READER
	#define XNPROC NPROC
#else
	#define XNPROC(returnType, name, params)
#endif
	#define NOPROC(name)
	#include "PEWProcs.h"
	#undef NPROC
	#undef UNPROC
	#undef NOPROC
	#undef XNPROC

#endif /* !PLUGIN */

#if PLUGIN

	/* HFT version */
   #include "PIRequir.h"
	
	/* Enumerate the selectors */
	#define NPROC(returnType, name, params)			\
		name##SEL,
    #define UNPROC NPROC
	#define XNPROC NPROC
    #define NOPROC(name)	\
		name##SEL,

	enum {
		PDFEditBAD_SELECTOR,
		#include "PEWProcs.h"
		PDFEditNUMSELECTORSplusOne
	};
	
	#define PDFEditNUMSELECTORS (PDFEditNUMSELECTORSplusOne - 1)
	
	/* Create the prototypes */
	#undef NPROC
	#undef UNPROC
	#undef NOPROC
	#undef XNPROC

	#define NPROC(returnType, name, params)		\
		typedef ACCBPROTO1 returnType (ACCBPROTO2 *name##SELPROTO)params;
	#define NOPROC(name)
#if READER_PLUGIN
    #define UNPROC(returnType, name, params) 
    #define XNPROC(returnType, name, params) 
#else
    #define UNPROC NPROC
	#define XNPROC NPROC
#endif

		#include "PEWProcs.h"
	#undef NPROC
	#undef NOPROC
	#undef UNPROC
	#undef XNPROC

#ifdef THREAD_SAFE_PDFL
	#define gPDFEditWriteHFT (GetHFTLocations()->pdfEditWriteHFT)
	#define gPDFEditWriteVersion (GetHFTLocations()->pdfEditWriteVersion)
#else
	extern HFT gPDFEditWriteHFT;
	extern ASUns32 gPDFEditWriteVersion;
#endif /* defined THREAD_SAFE_PDFL */



#if PI_PDFEDIT_WRITE_VERSION != 0 && !STATIC_HFT
/* PI_PDFEDIT_WRITE_VERSION >= 0x00040000 */


	/* Define the macros */
	#define PDEContentCreate (ACROASSERT(gPDFEditWriteVersion >=PDFEditWriteHFT_VERSION_4), *((PDEContentCreateSELPROTO)(gPDFEditWriteHFT[PDEContentCreateSEL]))) 
	#define PDEContentToCosObj (ACROASSERT(gPDFEditWriteVersion >=PDFEditWriteHFT_VERSION_4), *((PDEContentToCosObjSELPROTO)(gPDFEditWriteHFT[PDEContentToCosObjSEL]))) 
	#define PDEContentRemoveElem (ACROASSERT(gPDFEditWriteVersion >=PDFEditWriteHFT_VERSION_4), *((PDEContentRemoveElemSELPROTO)(gPDFEditWriteHFT[PDEContentRemoveElemSEL]))) 
	#define PDEContentAddElem (ACROASSERT(gPDFEditWriteVersion >=PDFEditWriteHFT_VERSION_4), *((PDEContentAddElemSELPROTO)(gPDFEditWriteHFT[PDEContentAddElemSEL]))) 
	
	#define PDEElementSetGState (ACROASSERT(gPDFEditWriteVersion >=PDFEditWriteHFT_VERSION_4), *((PDEElementSetGStateSELPROTO)(gPDFEditWriteHFT[PDEElementSetGStateSEL]))) 
#if ADD_ASFIXED //PKG
	#define PDEElementSetMatrix (ACROASSERT(gPDFEditWriteVersion >=PDFEditWriteHFT_VERSION_4), *((PDEElementSetMatrixSELPROTO)(gPDFEditWriteHFT[PDEElementSetMatrixSEL]))) 
#endif //ADD_ASFIXED //PKG
	#define PDEElementSetClip (ACROASSERT(gPDFEditWriteVersion >=PDFEditWriteHFT_VERSION_4), *((PDEElementSetClipSELPROTO)(gPDFEditWriteHFT[PDEElementSetClipSEL]))) 
	#define PDEElementCopy (ACROASSERT(gPDFEditWriteVersion >=PDFEditWriteHFT_VERSION_4), *((PDEElementCopySELPROTO)(gPDFEditWriteHFT[PDEElementCopySEL]))) 
	
	#define PDETextCreate (ACROASSERT(gPDFEditWriteVersion >=PDFEditWriteHFT_VERSION_4), *((PDETextCreateSELPROTO)(gPDFEditWriteHFT[PDETextCreateSEL]))) 
	#define PDETextRunSetGState (ACROASSERT(gPDFEditWriteVersion >=PDFEditWriteHFT_VERSION_4), *((PDETextRunSetGStateSELPROTO)(gPDFEditWriteHFT[PDETextRunSetGStateSEL]))) 
	#define PDETextRunSetTextState (ACROASSERT(gPDFEditWriteVersion >=PDFEditWriteHFT_VERSION_4), *((PDETextRunSetTextStateSELPROTO)(gPDFEditWriteHFT[PDETextRunSetTextStateSEL]))) 
	#define PDETextRunSetFont (ACROASSERT(gPDFEditWriteVersion >=PDFEditWriteHFT_VERSION_4), *((PDETextRunSetFontSELPROTO)(gPDFEditWriteHFT[PDETextRunSetFontSEL]))) 
	#define PDETextRunSetTextMatrix (ACROASSERT(gPDFEditWriteVersion >=PDFEditWriteHFT_VERSION_4), *((PDETextRunSetTextMatrixSELPROTO)(gPDFEditWriteHFT[PDETextRunSetTextMatrixSEL]))) 
	#define PDETextRunSetStrokeMatrix (ACROASSERT(gPDFEditWriteVersion >=PDFEditWriteHFT_VERSION_4), *((PDETextRunSetStrokeMatrixSELPROTO)(gPDFEditWriteHFT[PDETextRunSetStrokeMatrixSEL]))) 
	#define PDETextAdd (ACROASSERT(gPDFEditWriteVersion >=PDFEditWriteHFT_VERSION_4), *((PDETextAddSELPROTO)(gPDFEditWriteHFT[PDETextAddSEL]))) 
	#define PDETextRemove (ACROASSERT(gPDFEditWriteVersion >=PDFEditWriteHFT_VERSION_4), *((PDETextRemoveSELPROTO)(gPDFEditWriteHFT[PDETextRemoveSEL]))) 
	
	#define PDEPathCreate (ACROASSERT(gPDFEditWriteVersion >=PDFEditWriteHFT_VERSION_4), *((PDEPathCreateSELPROTO)(gPDFEditWriteHFT[PDEPathCreateSEL]))) 
	#define PDEPathSetData (ACROASSERT(gPDFEditWriteVersion >=PDFEditWriteHFT_VERSION_4), *((PDEPathSetDataSELPROTO)(gPDFEditWriteHFT[PDEPathSetDataSEL]))) 
	#define PDEPathSetPaintOp (ACROASSERT(gPDFEditWriteVersion >=PDFEditWriteHFT_VERSION_4), *((PDEPathSetPaintOpSELPROTO)(gPDFEditWriteHFT[PDEPathSetPaintOpSEL]))) 
	
	#define PDEImageCreateFromCosObj (ACROASSERT(gPDFEditWriteVersion >=PDFEditWriteHFT_VERSION_4), *((PDEImageCreateFromCosObjSELPROTO)(gPDFEditWriteHFT[PDEImageCreateFromCosObjSEL]))) 
	#define PDEImageSetData (ACROASSERT(gPDFEditWriteVersion >=PDFEditWriteHFT_VERSION_4), *((PDEImageSetDataSELPROTO)(gPDFEditWriteHFT[PDEImageSetDataSEL]))) 
	#define PDEImageSetDataStm (ACROASSERT(gPDFEditWriteVersion >=PDFEditWriteHFT_VERSION_4), *((PDEImageSetDataStmSELPROTO)(gPDFEditWriteHFT[PDEImageSetDataStmSEL]))) 
	#define PDEImageCreate (ACROASSERT(gPDFEditWriteVersion >=PDFEditWriteHFT_VERSION_4), *((PDEImageCreateSELPROTO)(gPDFEditWriteHFT[PDEImageCreateSEL]))) 
	
	#define PDEClipCreate (ACROASSERT(gPDFEditWriteVersion >=PDFEditWriteHFT_VERSION_4), *((PDEClipCreateSELPROTO)(gPDFEditWriteHFT[PDEClipCreateSEL]))) 
	#define PDEClipAddElem (ACROASSERT(gPDFEditWriteVersion >=PDFEditWriteHFT_VERSION_4), *((PDEClipAddElemSELPROTO)(gPDFEditWriteHFT[PDEClipAddElemSEL]))) 
	#define PDEClipRemoveElems (ACROASSERT(gPDFEditWriteVersion >=PDFEditWriteHFT_VERSION_4), *((PDEClipRemoveElemsSELPROTO)(gPDFEditWriteHFT[PDEClipRemoveElemsSEL]))) 
	
	#define PDEXObjectCreate (ACROASSERT(gPDFEditWriteVersion >=PDFEditWriteHFT_VERSION_4), *((PDEXObjectCreateSELPROTO)(gPDFEditWriteHFT[PDEXObjectCreateSEL]))) 
	
	#define PDEFormCreateFromCosObj (ACROASSERT(gPDFEditWriteVersion >=PDFEditWriteHFT_VERSION_4), *((PDEFormCreateFromCosObjSELPROTO)(gPDFEditWriteHFT[PDEFormCreateFromCosObjSEL]))) 
	#define PDEFormGetContent (ACROASSERT(gPDFEditWriteVersion >=PDFEditWriteHFT_VERSION_4), *((PDEFormGetContentSELPROTO)(gPDFEditWriteHFT[PDEFormGetContentSEL]))) 
	
	#define PDEPSCreate (ACROASSERT(gPDFEditWriteVersion >=PDFEditWriteHFT_VERSION_4), *((PDEPSCreateSELPROTO)(gPDFEditWriteHFT[PDEPSCreateSEL]))) 
	#define PDEPSCreateFromCosObj (ACROASSERT(gPDFEditWriteVersion >=PDFEditWriteHFT_VERSION_4), *((PDEPSCreateFromCosObjSELPROTO)(gPDFEditWriteHFT[PDEPSCreateFromCosObjSEL]))) 
	#define PDEPSSetData (ACROASSERT(gPDFEditWriteVersion >=PDFEditWriteHFT_VERSION_4), *((PDEPSSetDataSELPROTO)(gPDFEditWriteHFT[PDEPSSetDataSEL]))) 
	#define PDEPSSetDataStm (ACROASSERT(gPDFEditWriteVersion >=PDFEditWriteHFT_VERSION_4), *((PDEPSSetDataStmSELPROTO)(gPDFEditWriteHFT[PDEPSSetDataStmSEL]))) 
	
	#define PDEFontCreate (ACROASSERT(gPDFEditWriteVersion >=PDFEditWriteHFT_VERSION_4), *((PDEFontCreateSELPROTO)(gPDFEditWriteHFT[PDEFontCreateSEL]))) 
	#define PDEFontCreateFromCosObj (ACROASSERT(gPDFEditWriteVersion >=PDFEditWriteHFT_VERSION_4), *((PDEFontCreateFromCosObjSELPROTO)(gPDFEditWriteHFT[PDEFontCreateFromCosObjSEL]))) 
	#define PDEFontCreateFromSysFont (ACROASSERT(gPDFEditWriteVersion >=PDFEditWriteHFT_VERSION_4), *((PDEFontCreateFromSysFontSELPROTO)(gPDFEditWriteHFT[PDEFontCreateFromSysFontSEL]))) 
	
	#define PDEColorSpaceCreateFromName (ACROASSERT(gPDFEditWriteVersion >=PDFEditWriteHFT_VERSION_4), *((PDEColorSpaceCreateFromNameSELPROTO)(gPDFEditWriteHFT[PDEColorSpaceCreateFromNameSEL]))) 
	#define PDEColorSpaceCreateFromCosObj (ACROASSERT(gPDFEditWriteVersion >=PDFEditWriteHFT_VERSION_4), *((PDEColorSpaceCreateFromCosObjSELPROTO)(gPDFEditWriteHFT[PDEColorSpaceCreateFromCosObjSEL]))) 
	
	#define PDEAddTag (ACROASSERT(gPDFEditWriteVersion >=PDFEditWriteHFT_VERSION_4), *((PDEAddTagSELPROTO)(gPDFEditWriteHFT[PDEAddTagSEL]))) 
	#define PDEGetTag (ACROASSERT(gPDFEditWriteVersion >=PDFEditWriteHFT_VERSION_4), *((PDEGetTagSELPROTO)(gPDFEditWriteHFT[PDEGetTagSEL]))) 
	#define PDERemoveTag (ACROASSERT(gPDFEditWriteVersion >=PDFEditWriteHFT_VERSION_4), *((PDERemoveTagSELPROTO)(gPDFEditWriteHFT[PDERemoveTagSEL]))) 
	#define PDEMergeResourcesDict (ACROASSERT(gPDFEditWriteVersion >=PDFEditWriteHFT_VERSION_4), *((PDEMergeResourcesDictSELPROTO)(gPDFEditWriteHFT[PDEMergeResourcesDictSEL])))
	#define PDEExtGStateCreate (ACROASSERT(gPDFEditWriteVersion >=PDFEditWriteHFT_VERSION_4), *((PDEExtGStateCreateSELPROTO)(gPDFEditWriteHFT[PDEExtGStateCreateSEL])))
	#define PDEPlaceCreate (ACROASSERT(gPDFEditWriteVersion >=PDFEditWriteHFT_VERSION_4), *((PDEPlaceCreateSELPROTO)(gPDFEditWriteHFT[PDEPlaceCreateSEL])))
	#define PDEPlaceSetMCTag (ACROASSERT(gPDFEditWriteVersion >=PDFEditWriteHFT_VERSION_4), *((PDEPlaceSetMCTagSELPROTO)(gPDFEditWriteHFT[PDEPlaceSetMCTagSEL])))
	#define PDEPlaceSetDict (ACROASSERT(gPDFEditWriteVersion >=PDFEditWriteHFT_VERSION_4), *((PDEPlaceSetDictSELPROTO)(gPDFEditWriteHFT[PDEPlaceSetDictSEL])))
	#define PDEContainerCreate (ACROASSERT(gPDFEditWriteVersion >=PDFEditWriteHFT_VERSION_4), *((PDEContainerCreateSELPROTO)(gPDFEditWriteHFT[PDEContainerCreateSEL])))
	#define PDEContainerSetMCTag (ACROASSERT(gPDFEditWriteVersion >=PDFEditWriteHFT_VERSION_4), *((PDEContainerSetMCTagSELPROTO)(gPDFEditWriteHFT[PDEContainerSetMCTagSEL])))
	#define PDEContainerSetDict (ACROASSERT(gPDFEditWriteVersion >=PDFEditWriteHFT_VERSION_4), *((PDEContainerSetDictSELPROTO)(gPDFEditWriteHFT[PDEContainerSetDictSEL])))
	#define PDEContainerSetContent (ACROASSERT(gPDFEditWriteVersion >=PDFEditWriteHFT_VERSION_4), *((PDEContainerSetContentSELPROTO)(gPDFEditWriteHFT[PDEContainerSetContentSEL])))
	#define PDETextSplitRunAt (ACROASSERT(gPDFEditWriteVersion >=PDFEditWriteHFT_VERSION_4), *((PDETextSplitRunAtSELPROTO)(gPDFEditWriteHFT[PDETextSplitRunAtSEL])))
	#define PDETextRunSetGState (ACROASSERT(gPDFEditWriteVersion >=PDFEditWriteHFT_VERSION_4), *((PDETextRunSetGStateSELPROTO)(gPDFEditWriteHFT[PDETextRunSetGStateSEL])))
	#define PDEPatternCreate (ACROASSERT(gPDFEditWriteVersion >=PDFEditWriteHFT_VERSION_4), *((PDEPatternCreateSELPROTO)(gPDFEditWriteHFT[PDEPatternCreateSEL])))
	#define PDETextReplaceChars (ACROASSERT(gPDFEditWriteVersion >=PDFEditWriteHFT_VERSION_4), *((PDETextReplaceCharsSELPROTO)(gPDFEditWriteHFT[PDETextReplaceCharsSEL])))
	#define PDEPurgeCache (ACROASSERT(gPDFEditWriteVersion >=PDFEditWriteHFT_VERSION_4), *((PDEPurgeCacheSELPROTO)(gPDFEditWriteHFT[PDEPurgeCacheSEL]))) 
	#define PDEFontSubsetNow (ACROASSERT(gPDFEditWriteVersion >=PDFEditWriteHFT_VERSION_4), *((PDEFontSubsetNowSELPROTO)(gPDFEditWriteHFT[PDEFontSubsetNowSEL])))
	#define PDEPathAddSegment (ACROASSERT(gPDFEditWriteVersion >=PDFEditWriteHFT_VERSION_4), *((PDEPathAddSegmentSELPROTO)(gPDFEditWriteHFT[PDEPathAddSegmentSEL])))
	#define PDEPathAddSegmentEx (ACROASSERT(gPDFEditWriteVersion >=PDFEditWriteHFT_VERSION_4), *((PDEPathAddSegmentExSELPROTO)(gPDFEditWriteHFT[PDEPathAddSegmentExSEL])))
	#define PDEFontCreateWithParams (ACROASSERT(gPDFEditWriteVersion >=PDFEditWriteHFT_VERSION_4), *((PDEFontCreateWithParamsSELPROTO)(gPDFEditWriteHFT[PDEFontCreateWithParamsSEL]))) 
	#define PDEColorSpaceCreate (ACROASSERT(gPDFEditWriteVersion >=PDFEditWriteHFT_VERSION_4), *((PDEColorSpaceCreateSELPROTO)(gPDFEditWriteHFT[PDEColorSpaceCreateSEL]))) 
	#define PDEDeviceNColorsCreate (ACROASSERT(gPDFEditWriteVersion >=PDFEditWriteHFT_VERSION_4), *((PDEDeviceNColorsCreateSELPROTO)(gPDFEditWriteHFT[PDEDeviceNColorsCreateSEL])))
	#define PDEShadingCreateFromCosObj (ACROASSERT(gPDFEditWriteVersion >=PDFEditWriteHFT_VERSION_4), *((PDEShadingCreateFromCosObjSELPROTO)(gPDFEditWriteHFT[PDEShadingCreateFromCosObjSEL]))) 

	#define PDEGroupCreate (ACROASSERT(gPDFEditWriteVersion >=PDFEditWriteHFT_VERSION_4), *((PDEGroupCreateSELPROTO)(gPDFEditWriteHFT[PDEGroupCreateSEL])))
 	#define PDEGroupSetContent (ACROASSERT(gPDFEditWriteVersion >=PDFEditWriteHFT_VERSION_4), *((PDEGroupSetContentSELPROTO)(gPDFEditWriteHFT[PDEGroupSetContentSEL])))

	#define PDEFontCreateFromSysFontEx (ACROASSERT(gPDFEditWriteVersion >=PDFEditWriteHFT_VERSION_4), *((PDEFontCreateFromSysFontExSELPROTO)(gPDFEditWriteHFT[PDEFontCreateFromSysFontExSEL]))) 

	#define PDEClipCopy (ACROASSERT(gPDFEditWriteVersion >=PDFEditWriteHFT_VERSION_4), *((PDEClipCopySELPROTO)(gPDFEditWriteHFT[PDEClipCopySEL]))) 

	#define PDEImageSetDecodeArray (ACROASSERT(gPDFEditWriteVersion >=PDFEditWriteHFT_VERSION_4), *((PDEImageSetDecodeArraySELPROTO)(gPDFEditWriteHFT[PDEImageSetDecodeArraySEL])))
	#define PDEContentAddPage (ACROASSERT(gPDFEditWriteVersion >=PDFEditWriteHFT_VERSION_4), *((PDEContentAddPageSELPROTO)(gPDFEditWriteHFT[PDEContentAddPageSEL]))) 
	#define PDEFontEmbedNowDontSubset (ACROASSERT(gPDFEditWriteVersion >=PDFEditWriteHFT_VERSION_4), *((PDEFontEmbedNowDontSubsetSELPROTO)(gPDFEditWriteHFT[PDEFontEmbedNowDontSubsetSEL])))
	#define PDEFontGetWidthsNow (ACROASSERT(gPDFEditWriteVersion >=PDFEditWriteHFT_VERSION_4), *((PDEFontGetWidthsNowSELPROTO)(gPDFEditWriteHFT[PDEFontGetWidthsNowSEL])))
	

/* PI_PDFEDIT_WRITE_VERSION >= 0x00050000 */

	#define PDEBeginContainerCreate (ACROASSERT(gPDFEditWriteVersion >=PDFEditWriteHFT_VERSION_5), *((PDEBeginContainerCreateSELPROTO)(gPDFEditWriteHFT[PDEBeginContainerCreateSEL])))
	#define PDEBeginContainerSetMCTag (ACROASSERT(gPDFEditWriteVersion >=PDFEditWriteHFT_VERSION_5), *((PDEBeginContainerSetMCTagSELPROTO)(gPDFEditWriteHFT[PDEBeginContainerSetMCTagSEL])))
	#define PDEBeginContainerSetDict (ACROASSERT(gPDFEditWriteVersion >=PDFEditWriteHFT_VERSION_5), *((PDEBeginContainerSetDictSELPROTO)(gPDFEditWriteHFT[PDEBeginContainerSetDictSEL])))

	#define PDESoftMaskCreateFromCosObj (ACROASSERT(gPDFEditWriteVersion >=PDFEditWriteHFT_VERSION_5), *((PDESoftMaskCreateFromCosObjSELPROTO)(gPDFEditWriteHFT[PDESoftMaskCreateFromCosObjSEL])))
	#define PDESoftMaskCreate (ACROASSERT(gPDFEditWriteVersion >=PDFEditWriteHFT_VERSION_5), *((PDESoftMaskCreateSELPROTO)(gPDFEditWriteHFT[PDESoftMaskCreateSEL])))
	#define PDESoftMaskSetXGroup (ACROASSERT(gPDFEditWriteVersion >=PDFEditWriteHFT_VERSION_5), *((PDESoftMaskSetXGroupSELPROTO)(gPDFEditWriteHFT[PDESoftMaskSetXGroupSEL])))
	#define PDESoftMaskSetBackdropColor (ACROASSERT(gPDFEditWriteVersion >=PDFEditWriteHFT_VERSION_5), *((PDESoftMaskSetBackdropColorSELPROTO)(gPDFEditWriteHFT[PDESoftMaskSetBackdropColorSEL])))
	#define PDESoftMaskSetTransferFunction (ACROASSERT(gPDFEditWriteVersion >=PDFEditWriteHFT_VERSION_5), *((PDESoftMaskSetTransferFunctionSELPROTO)(gPDFEditWriteHFT[PDESoftMaskSetTransferFunctionSEL])))

	#define PDEXGroupCreateFromCosObj (ACROASSERT(gPDFEditWriteVersion >=PDFEditWriteHFT_VERSION_5), *((PDEXGroupCreateFromCosObjSELPROTO)(gPDFEditWriteHFT[PDEXGroupCreateFromCosObjSEL])))
	#define PDEXGroupCreate (ACROASSERT(gPDFEditWriteVersion >=PDFEditWriteHFT_VERSION_5), *((PDEXGroupCreateSELPROTO)(gPDFEditWriteHFT[PDEXGroupCreateSEL])))
	#define PDEXGroupSetKnockout (ACROASSERT(gPDFEditWriteVersion >=PDFEditWriteHFT_VERSION_5), *((PDEXGroupSetKnockoutSELPROTO)(gPDFEditWriteHFT[PDEXGroupSetKnockoutSEL])))
	#define PDEXGroupSetIsolated (ACROASSERT(gPDFEditWriteVersion >=PDFEditWriteHFT_VERSION_5), *((PDEXGroupSetIsolatedSELPROTO)(gPDFEditWriteHFT[PDEXGroupSetIsolatedSEL])))
	#define PDEXGroupSetColorSpace (ACROASSERT(gPDFEditWriteVersion >=PDFEditWriteHFT_VERSION_5), *((PDEXGroupSetColorSpaceSELPROTO)(gPDFEditWriteHFT[PDEXGroupSetColorSpaceSEL])))

	#define PDEFormSetXGroup (ACROASSERT(gPDFEditWriteVersion >=PDFEditWriteHFT_VERSION_5), *((PDEFormSetXGroupSELPROTO)(gPDFEditWriteHFT[PDEFormSetXGroupSEL])))

	#define PDEExtGStateCreateNew (ACROASSERT(gPDFEditWriteVersion >=PDFEditWriteHFT_VERSION_5), *((PDEExtGStateCreateNewSELPROTO)(gPDFEditWriteHFT[PDEExtGStateCreateNewSEL])))
	#define PDEExtGStateSetOPM (ACROASSERT(gPDFEditWriteVersion >=PDFEditWriteHFT_VERSION_5), *((PDEExtGStateSetOPMSELPROTO)(gPDFEditWriteHFT[PDEExtGStateSetOPMSEL])))
	#define PDEExtGStateSetOPFill (ACROASSERT(gPDFEditWriteVersion >=PDFEditWriteHFT_VERSION_5), *((PDEExtGStateSetOPFillSELPROTO)(gPDFEditWriteHFT[PDEExtGStateSetOPFillSEL])))
	#define PDEExtGStateSetOPStroke (ACROASSERT(gPDFEditWriteVersion >=PDFEditWriteHFT_VERSION_5), *((PDEExtGStateSetOPStrokeSELPROTO)(gPDFEditWriteHFT[PDEExtGStateSetOPStrokeSEL])))
	#define PDEExtGStateSetOpacityFill (ACROASSERT(gPDFEditWriteVersion >=PDFEditWriteHFT_VERSION_5), *((PDEExtGStateSetOpacityFillSELPROTO)(gPDFEditWriteHFT[PDEExtGStateSetOpacityFillSEL])))
	#define PDEExtGStateSetOpacityStroke (ACROASSERT(gPDFEditWriteVersion >=PDFEditWriteHFT_VERSION_5), *((PDEExtGStateSetOpacityStrokeSELPROTO)(gPDFEditWriteHFT[PDEExtGStateSetOpacityStrokeSEL])))
	#define PDEExtGStateSetBlendMode (ACROASSERT(gPDFEditWriteVersion >=PDFEditWriteHFT_VERSION_5), *((PDEExtGStateSetBlendModeSELPROTO)(gPDFEditWriteHFT[PDEExtGStateSetBlendModeSEL])))
	#define PDEExtGStateSetAIS (ACROASSERT(gPDFEditWriteVersion >=PDFEditWriteHFT_VERSION_5), *((PDEExtGStateSetAISSELPROTO)(gPDFEditWriteHFT[PDEExtGStateSetAISSEL])))
	#define PDEExtGStateSetSoftMask (ACROASSERT(gPDFEditWriteVersion >=PDFEditWriteHFT_VERSION_5), *((PDEExtGStateSetSoftMaskSELPROTO)(gPDFEditWriteHFT[PDEExtGStateSetSoftMaskSEL])))

	#define PDEImageSetSMask (ACROASSERT(gPDFEditWriteVersion >=PDFEditWriteHFT_VERSION_5), *((PDEImageSetSMaskSELPROTO)(gPDFEditWriteHFT[PDEImageSetSMaskSEL])))
	#define PDEImageSetMatteArray (ACROASSERT(gPDFEditWriteVersion >=PDFEditWriteHFT_VERSION_5), *((PDEImageSetMatteArraySELPROTO)(gPDFEditWriteHFT[PDEImageSetMatteArraySEL])))

	#define PDEEndContainerCreate (ACROASSERT(gPDFEditWriteVersion >=PDFEditWriteHFT_VERSION_5), *((PDEEndContainerCreateSELPROTO)(gPDFEditWriteHFT[PDEEndContainerCreateSEL])))
	#define PDEBeginGroupCreate (ACROASSERT(gPDFEditWriteVersion >=PDFEditWriteHFT_VERSION_5), *((PDEBeginGroupCreateSELPROTO)(gPDFEditWriteHFT[PDEBeginGroupCreateSEL])))
	#define PDEEndGroupCreate (ACROASSERT(gPDFEditWriteVersion >=PDFEditWriteHFT_VERSION_5), *((PDEEndGroupCreateSELPROTO)(gPDFEditWriteHFT[PDEEndGroupCreateSEL])))

	#define PDEFontCreateFromSysFontWithParams (ACROASSERT(gPDFEditWriteVersion >=PDFEditWriteHFT_VERSION_5), *((PDEFontCreateFromSysFontWithParamsSELPROTO)(gPDFEditWriteHFT[PDEFontCreateFromSysFontWithParamsSEL]))) 
	#define PDEFontTranslateGlyphIdsToUnicode (ACROASSERT(gPDFEditWriteVersion >=PDFEditWriteHFT_VERSION_5), *((PDEFontTranslateGlyphIdsToUnicodeSELPROTO)(gPDFEditWriteHFT[PDEFontTranslateGlyphIdsToUnicodeSEL]))) 

	#define PDEExtGStateSetTK (ACROASSERT(gPDFEditWriteVersion >=PDFEditWriteHFT_VERSION_5), *((PDEExtGStateSetTKSELPROTO)(gPDFEditWriteHFT[PDEExtGStateSetTKSEL])))

	#define PDETextRunSetState (ACROASSERT(gPDFEditWriteVersion >=PDFEditWriteHFT_VERSION_5), *((PDETextRunSetStateSELPROTO)(gPDFEditWriteHFT[PDETextRunSetStateSEL]))) 

	#define PDSysEncodingCreateFromBaseName	(ACROASSERT(gPDFEditWriteVersion >=PDFEditWriteHFT_VERSION_5), *((PDSysEncodingCreateFromBaseNameSELPROTO)(gPDFEditWriteHFT[PDSysEncodingCreateFromBaseNameSEL]))) 
	#define PDSysEncodingCreateFromCMapName	(ACROASSERT(gPDFEditWriteVersion >=PDFEditWriteHFT_VERSION_5), *((PDSysEncodingCreateFromCMapNameSELPROTO)(gPDFEditWriteHFT[PDSysEncodingCreateFromCMapNameSEL]))) 
	#define PDSysFontGetCreateFlags	(ACROASSERT(gPDFEditWriteVersion >=PDFEditWriteHFT_VERSION_5), *((PDSysFontGetCreateFlagsSELPROTO)(gPDFEditWriteHFT[PDSysFontGetCreateFlagsSEL])))
	#define PDEFontCreateFromSysFontAndEncoding	(ACROASSERT(gPDFEditWriteVersion >=PDFEditWriteHFT_VERSION_5), *((PDEFontCreateFromSysFontAndEncodingSELPROTO)(gPDFEditWriteHFT[PDEFontCreateFromSysFontAndEncodingSEL])))
	#define PDEFontGetCreateNeedFlags	(ACROASSERT(gPDFEditWriteVersion >=PDFEditWriteHFT_VERSION_5), *((PDEFontGetCreateNeedFlagsSELPROTO)(gPDFEditWriteHFT[PDEFontGetCreateNeedFlagsSEL])))
	#define PDEFontEmbedNow	(ACROASSERT(gPDFEditWriteVersion >=PDFEditWriteHFT_VERSION_5), *((PDEFontEmbedNowSELPROTO)(gPDFEditWriteHFT[PDEFontEmbedNowSEL])))
	#define PDEFontCreateWidthsNow	(ACROASSERT(gPDFEditWriteVersion >=PDFEditWriteHFT_VERSION_5), *((PDEFontCreateWidthsNowSELPROTO)(gPDFEditWriteHFT[PDEFontCreateWidthsNowSEL])))
	#define PDEFontCreateToUnicodeNow	(ACROASSERT(gPDFEditWriteVersion >=PDFEditWriteHFT_VERSION_5), *((PDEFontCreateToUnicodeNowSELPROTO)(gPDFEditWriteHFT[PDEFontCreateToUnicodeNowSEL])))

	#define PDEImageSetColorSpace	(ACROASSERT(gPDFEditWriteVersion >=PDFEditWriteHFT_VERSION_5), *((PDEImageSetColorSpaceSELPROTO)(gPDFEditWriteHFT[PDEImageSetColorSpaceSEL])))
	#define PDEExtGStateSetSA	(ACROASSERT(gPDFEditWriteVersion >=PDFEditWriteHFT_VERSION_5), *((PDEExtGStateSetSASELPROTO)(gPDFEditWriteHFT[PDEExtGStateSetSASEL])))
	#define PDESoftMaskCreateFromName	(ACROASSERT(gPDFEditWriteVersion >=PDFEditWriteHFT_VERSION_5), *((PDESoftMaskCreateFromNameSELPROTO)(gPDFEditWriteHFT[PDESoftMaskCreateFromNameSEL])))

	#define PDETextRunSetMatrix	(ACROASSERT(gPDFEditWriteVersion >=PDFEditWriteHFT_VERSION_5), *((PDETextRunSetMatrixSELPROTO)(gPDFEditWriteHFT[PDETextRunSetMatrixSEL])))

/* BEGIN Optional Content API calls */
	#define PDEElementSetOCMD (ACROASSERT(gPDFEditWriteVersion >= PDFEditWriteHFT_VERSION_6), *((PDEElementSetOCMDSELPROTO)(gPDFEditWriteHFT[PDEElementSetOCMDSEL])))
	#define PDEElementRemoveOCMD (ACROASSERT(gPDFEditWriteVersion >= PDFEditWriteHFT_VERSION_6), *((PDEElementRemoveOCMDSELPROTO)(gPDFEditWriteHFT[PDEElementRemoveOCMDSEL])))
	#define PDEContentFlattenOC (ACROASSERT(gPDFEditWriteVersion >= PDFEditWriteHFT_VERSION_6), *((PDEContentFlattenOCSELPROTO)(gPDFEditWriteHFT[PDEContentFlattenOCSEL])))
/* END Optional Content API calls */

	#define PDSysEncodingCreateFromCodePage (ACROASSERT(gPDFEditWriteVersion >= PDFEditWriteHFT_VERSION_6), *((PDSysEncodingCreateFromCodePageSELPROTO)(gPDFEditWriteHFT[PDSysEncodingCreateFromCodePageSEL])))
	#define PDEFontSetSysFont (ACROASSERT(gPDFEditWriteVersion >= PDFEditWriteHFT_VERSION_6), *((PDEFontSetSysFontSELPROTO)(gPDFEditWriteHFT[PDEFontSetSysFontSEL])))
	#define PDEFontSetSysEncoding (ACROASSERT(gPDFEditWriteVersion >= PDFEditWriteHFT_VERSION_6), *((PDEFontSetSysEncodingSELPROTO)(gPDFEditWriteHFT[PDEFontSetSysEncodingSEL])))
	#define PDETextItemCreate (ACROASSERT(gPDFEditWriteVersion >= PDFEditWriteHFT_VERSION_6), *((PDETextItemCreateSELPROTO)(gPDFEditWriteHFT[PDETextItemCreateSEL])))
	#define PDETextItemSetFont (ACROASSERT(gPDFEditWriteVersion >= PDFEditWriteHFT_VERSION_6), *((PDETextItemSetFontSELPROTO)(gPDFEditWriteHFT[PDETextItemSetFontSEL])))
#if ADD_ASFIXED //PKG
	#define PDETextItemSetTextMatrix (ACROASSERT(gPDFEditWriteVersion >= PDFEditWriteHFT_VERSION_6), *((PDETextItemSetTextMatrixSELPROTO)(gPDFEditWriteHFT[PDETextItemSetTextMatrixSEL])))
#endif //ADD_ASFIXED //PKG
	#define PDETextItemSetTextState (ACROASSERT(gPDFEditWriteVersion >= PDFEditWriteHFT_VERSION_6), *((PDETextItemSetTextStateSELPROTO)(gPDFEditWriteHFT[PDETextItemSetTextStateSEL])))
	#define PDETextItemSetGState (ACROASSERT(gPDFEditWriteVersion >= PDFEditWriteHFT_VERSION_6), *((PDETextItemSetGStateSELPROTO)(gPDFEditWriteHFT[PDETextItemSetGStateSEL])))
	#define PDETextItemReplaceText (ACROASSERT(gPDFEditWriteVersion >= PDFEditWriteHFT_VERSION_6), *((PDETextItemReplaceTextSELPROTO)(gPDFEditWriteHFT[PDETextItemReplaceTextSEL])))
	#define PDETextItemReplaceChars (ACROASSERT(gPDFEditWriteVersion >= PDFEditWriteHFT_VERSION_6), *((PDETextItemReplaceCharsSELPROTO)(gPDFEditWriteHFT[PDETextItemReplaceCharsSEL])))
	#define PDETextItemRemoveChars (ACROASSERT(gPDFEditWriteVersion >= PDFEditWriteHFT_VERSION_6), *((PDETextItemRemoveCharsSELPROTO)(gPDFEditWriteHFT[PDETextItemRemoveCharsSEL])))
	#define PDETextAddItem (ACROASSERT(gPDFEditWriteVersion >= PDFEditWriteHFT_VERSION_6), *((PDETextAddItemSELPROTO)(gPDFEditWriteHFT[PDETextAddItemSEL])))
	#define PDETextRemoveItems (ACROASSERT(gPDFEditWriteVersion >= PDFEditWriteHFT_VERSION_6), *((PDETextRemoveItemsSELPROTO)(gPDFEditWriteHFT[PDETextRemoveItemsSEL])))

/* Newport feature 513825 & ECO 519466 */
	#define PDEFormSetContent (ACROASSERT(gPDFEditWriteVersion >=PDFEditWriteHFT_VERSION_6), *((PDEFormSetContentSELPROTO)(gPDFEditWriteHFT[PDEFormSetContentSEL]))) 
	#define PDEFormCreateClone (ACROASSERT(gPDFEditWriteVersion >=PDFEditWriteHFT_VERSION_6), *((PDEFormCreateCloneSELPROTO)(gPDFEditWriteHFT[PDEFormCreateCloneSEL]))) 

/* PDETextAddGlyphs */
	#define PDETextAddGlyphs (ACROASSERT(gPDFEditWriteVersion >=PDFEditWriteHFT_VERSION_7), *((PDETextAddGlyphsSELPROTO)(gPDFEditWriteHFT[PDETextAddGlyphsSEL]))) 

/* PDEFontAddGlyphs */
	#define PDEFontAddGlyphs (ACROASSERT(gPDFEditWriteVersion >=PDFEditWriteHFT_VERSION_7), *((PDEFontAddGlyphsSELPROTO)(gPDFEditWriteHFT[PDEFontAddGlyphsSEL]))) 
	#define PDEReleaseSpan (ACROASSERT(gPDFEditWriteVersion >=PDFEditWriteHFT_VERSION_7), *((PDEReleaseSpanSELPROTO)(gPDFEditWriteHFT[PDEReleaseSpanSEL])))
/* New calls for InDesign */
	#define PDEContentSetPage (ACROASSERT(gPDFEditWriteVersion >=PDFEditWriteHFT_VERSION_7), *((PDEContentSetPageSELPROTO)(gPDFEditWriteHFT[PDEContentSetPageSEL]))) 
	#define PDEContentSetContainingStream (ACROASSERT(gPDFEditWriteVersion >=PDFEditWriteHFT_VERSION_7), *((PDEContentSetContainingStreamSELPROTO)(gPDFEditWriteHFT[PDEContentSetContainingStreamSEL]))) 
	#define PDEContentSetStreamOwner (ACROASSERT(gPDFEditWriteVersion >=PDFEditWriteHFT_VERSION_7), *((PDEContentSetStreamOwnerSELPROTO)(gPDFEditWriteHFT[PDEContentSetStreamOwnerSEL]))) 

/* PDSysFontVerifyEncoding */
	#define PDSysFontVerifyEncoding (ACROASSERT(gPDFEditWriteVersion >=PDFEditWriteHFT_VERSION_7), *((PDSysFontVerifyEncodingSELPROTO)(gPDFEditWriteHFT[PDSysFontVerifyEncodingSEL]))) 
	
/*Calls for PDEForm*/
	#define PDEFormGetMatrix (ACROASSERT(gPDFEditWriteVersion >=PDFEditWriteHFT_VERSION_7_5), *((PDEFormGetMatrixSELPROTO)(gPDFEditWriteHFT[PDEFormGetMatrixSEL]))) 
	#define PDEFormGetBBox (ACROASSERT(gPDFEditWriteVersion >=PDFEditWriteHFT_VERSION_7_5), *((PDEFormGetBBoxSELPROTO)(gPDFEditWriteHFT[PDEFormGetBBoxSEL]))) 

/* Calls for creating objects in specific CosDocs */
	#define PDEImageCreateInCosDoc (ACROASSERT(gPDFEditWriteVersion >=PDFEditWriteHFT_VERSION_8), *((PDEImageCreateInCosDocSELPROTO)(gPDFEditWriteHFT[PDEImageCreateInCosDocSEL]))) 
	#define PDEFontCreateInCosDoc (ACROASSERT(gPDFEditWriteVersion >=PDFEditWriteHFT_VERSION_8), *((PDEFontCreateInCosDocSELPROTO)(gPDFEditWriteHFT[PDEFontCreateInCosDocSEL]))) 
	#define PDEFontCreateFromSysFontInCosDoc (ACROASSERT(gPDFEditWriteVersion >=PDFEditWriteHFT_VERSION_8), *((PDEFontCreateFromSysFontInCosDocSELPROTO)(gPDFEditWriteHFT[PDEFontCreateFromSysFontInCosDocSEL]))) 
	#define PDEFontCreateFromSysFontExInCosDoc (ACROASSERT(gPDFEditWriteVersion >=PDFEditWriteHFT_VERSION_8), *((PDEFontCreateFromSysFontExInCosDocSELPROTO)(gPDFEditWriteHFT[PDEFontCreateFromSysFontExInCosDocSEL]))) 
	#define PDEFontCreateWithParamsInCosDoc (ACROASSERT(gPDFEditWriteVersion >=PDFEditWriteHFT_VERSION_8), *((PDEFontCreateWithParamsInCosDocSELPROTO)(gPDFEditWriteHFT[PDEFontCreateWithParamsInCosDocSEL]))) 
	#define PDEFontCreateFromSysFontAndEncodingInCosDoc	(ACROASSERT(gPDFEditWriteVersion >=PDFEditWriteHFT_VERSION_8), *((PDEFontCreateFromSysFontAndEncodingInCosDocSELPROTO)(gPDFEditWriteHFT[PDEFontCreateFromSysFontAndEncodingInCosDocSEL])))
	#define PDEColorSpaceCreateInCosDoc (ACROASSERT(gPDFEditWriteVersion >=PDFEditWriteHFT_VERSION_8), *((PDEColorSpaceCreateInCosDocSELPROTO)(gPDFEditWriteHFT[PDEColorSpaceCreateInCosDocSEL]))) 

#if !defined (_H_PEPCalls) && PDFEditWriteHFT_LATEST_VERSION >= PDFEditWriteHFT_VERSION_8 
	#define PDEContentSetDefaultColorSpace (ACROASSERT(gPDFEditWriteVersion >=PDFEditWriteHFT_VERSION_8), *((PDEContentSetDefaultColorSpace_PEWCalls_SELPROTO)(gPDFEditWriteHFT[PDEContentSetDefaultColorSpace_PEWCalls_SEL])))
#endif
	
	#define PDEImageGetDataLen64 (ACROASSERT(gPDFEditWriteVersion >=PDFEditWriteHFT_VERSION_8), *((PDEImageGetDataLen64SELPROTO)(gPDFEditWriteHFT[PDEImageGetDataLen64SEL]))) 
	#define PDEImageCreateInCosDoc64 (ACROASSERT(gPDFEditWriteVersion >=PDFEditWriteHFT_VERSION_8), *((PDEImageCreateInCosDoc64SELPROTO)(gPDFEditWriteHFT[PDEImageCreateInCosDoc64SEL]))) 

	#define PDEScratchDocCleanup (ACROASSERT(gPDFEditWriteVersion >=PDFEditWriteHFT_VERSION_8), *((PDEScratchDocCleanupSELPROTO)(gPDFEditWriteHFT[PDEScratchDocCleanupSEL]))) 

	#define PDSysEncodingCreateFromCMapStream (ACROASSERT(gPDFEditWriteVersion >=PDFEditWriteHFT_VERSION_9), *((PDSysEncodingCreateFromCMapStreamSELPROTO)(gPDFEditWriteHFT[PDSysEncodingCreateFromCMapStreamSEL]))) 
	#define PDEFormSetContentToCosObjFlags (ACROASSERT(gPDFEditWriteVersion >=PDFEditWriteHFT_VERSION_9), *((PDEFormSetContentToCosObjFlagsSELPROTO)(gPDFEditWriteHFT[PDEFormSetContentToCosObjFlagsSEL])))
/* If you add, delete, or modify procs listed in this file please read the instructions at the top about how to properly change the version number */

	#define PDEFormGetMatrixEx (ACROASSERT(gPDFEditWriteVersion >=PDFEditWriteHFT_VERSION_10), *((PDEFormGetMatrixExSELPROTO)(gPDFEditWriteHFT[PDEFormGetMatrixExSEL]))) 
	#define PDEContentAddPageEx (ACROASSERT(gPDFEditWriteVersion >=PDFEditWriteHFT_VERSION_10), *((PDEContentAddPageExSELPROTO)(gPDFEditWriteHFT[PDEContentAddPageExSEL]))) 
	#define PDEShadingCreateFromCosObjEx (ACROASSERT(gPDFEditWriteVersion >=PDFEditWriteHFT_VERSION_10), *((PDEShadingCreateFromCosObjExSELPROTO)(gPDFEditWriteHFT[PDEShadingCreateFromCosObjExSEL]))) 
	#define PDEImageCreateInCosDocEx (ACROASSERT(gPDFEditWriteVersion >=PDFEditWriteHFT_VERSION_10), *((PDEImageCreateInCosDocExSELPROTO)(gPDFEditWriteHFT[PDEImageCreateInCosDocExSEL]))) 
	#define PDEImageCreateEx (ACROASSERT(gPDFEditWriteVersion >=PDFEditWriteHFT_VERSION_10), *((PDEImageCreateExSELPROTO)(gPDFEditWriteHFT[PDEImageCreateExSEL]))) 
	#define PDETextRunSetTextMatrixEx (ACROASSERT(gPDFEditWriteVersion >=PDFEditWriteHFT_VERSION_10), *((PDETextRunSetTextMatrixExSELPROTO)(gPDFEditWriteHFT[PDETextRunSetTextMatrixExSEL]))) 
	#define PDETextRunSetStrokeMatrixEx (ACROASSERT(gPDFEditWriteVersion >=PDFEditWriteHFT_VERSION_10), *((PDETextRunSetStrokeMatrixExSELPROTO)(gPDFEditWriteHFT[PDETextRunSetStrokeMatrixExSEL]))) 
	#define PDEImageCreateFromCosObjEx (ACROASSERT(gPDFEditWriteVersion >=PDFEditWriteHFT_VERSION_10), *((PDEImageCreateFromCosObjExSELPROTO)(gPDFEditWriteHFT[PDEImageCreateFromCosObjExSEL]))) 
	#define PDETextItemCreateEx (ACROASSERT(gPDFEditWriteVersion >= PDFEditWriteHFT_VERSION_10), *((PDETextItemCreateExSELPROTO)(gPDFEditWriteHFT[PDETextItemCreateExSEL])))
	#define PDETextItemSetTextMatrixEx (ACROASSERT(gPDFEditWriteVersion >= PDFEditWriteHFT_VERSION_10), *((PDETextItemSetTextMatrixExSELPROTO)(gPDFEditWriteHFT[PDETextItemSetTextMatrixExSEL])))
	#define PDETextAddGlyphsEx (ACROASSERT(gPDFEditWriteVersion >=PDFEditWriteHFT_VERSION_10), *((PDETextAddGlyphsExSELPROTO)(gPDFEditWriteHFT[PDETextAddGlyphsExSEL]))) 
	#define PDETextAddEx (ACROASSERT(gPDFEditWriteVersion >=PDFEditWriteHFT_VERSION_10), *((PDETextAddExSELPROTO)(gPDFEditWriteHFT[PDETextAddExSEL]))) 
	#define PDEElementSetMatrixEx (ACROASSERT(gPDFEditWriteVersion >=PDFEditWriteHFT_VERSION_10), *((PDEElementSetMatrixExSELPROTO)(gPDFEditWriteHFT[PDEElementSetMatrixExSEL]))) 
	#define PDEFormCreateFromCosObjEx (ACROASSERT(gPDFEditWriteVersion >=PDFEditWriteHFT_VERSION_10), *((PDEFormCreateFromCosObjExSELPROTO)(gPDFEditWriteHFT[PDEFormCreateFromCosObjExSEL]))) 
	#define PDETextRunSetMatrixEx (ACROASSERT(gPDFEditWriteVersion >=PDFEditWriteHFT_VERSION_10), *((PDETextRunSetMatrixExSELPROTO)(gPDFEditWriteHFT[PDETextRunSetMatrixExSEL])))

	#define PDEPathSetDataEx (ACROASSERT(gPDFEditWriteVersion >=PDFEditWriteHFT_VERSION_10), *((PDEPathSetDataExSELPROTO)(gPDFEditWriteHFT[PDEPathSetDataExSEL])))

	#define PDEExtGStateSetBPC (ACROASSERT(gPDFEditWriteVersion >= PDFEditWriteHFT_VERSION_11), *((PDEExtGStateSetBPCSELPROTO)(gPDFEditWriteHFT[PDEExtGStateSetBPCSEL])))
	#define PDEExtGStateSetHalfToneOrigin (ACROASSERT(gPDFEditWriteVersion >=PDFEditWriteHFT_VERSION_11), *((PDEExtGStateSetHalfToneOriginSELPROTO)(gPDFEditWriteHFT[PDEExtGStateSetHalfToneOriginSEL]))) 


#endif /* PI_PDFEDIT_WRITE_VERSION != 0 && !STATIC_HFT */
	
#endif


	
#ifdef __cplusplus
}
#endif

#endif /* _H_PEWCalls */

