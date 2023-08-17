/*********************************************************************

 ADOBE SYSTEMS INCORPORATED
 Copyright (C) 1998-2007 Adobe Systems Incorporated
 All rights reserved.

 NOTICE: Adobe permits you to use, modify, and distribute this file
 in accordance with the terms of the Adobe license agreement
 accompanying it. If you have received this file from a source other
 than Adobe, then your use, modification, or distribution of it
 requires the prior written permission of Adobe.

 ---------------------------------------------------------------------

 PERCalls.h

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

#ifndef _H_PERCalls
#define _H_PERCalls
#include "acroassert.h"
#ifdef THREAD_SAFE_PDFL
#include "PDFLInitCommon.h"
#endif /* defined THREAD_SAFE_PDFL */

/* for Adobe use only */
#define _PDFEditReadHFT_LATEST_VERSION 0x000C0000
#define _PDFEditReadHFT_LAST_BETA_COMPATIBLE_VERSION 0x000C0000
#define _PDFEditReadHFT_IS_BETA 0

/* for public use */
#define PDFEditReadHFT_LATEST_VERSION (_PDFEditReadHFT_IS_BETA ? (kHFT_IN_BETA_FLAG | _PDFEditReadHFT_LATEST_VERSION) : _PDFEditReadHFT_LATEST_VERSION)

#define PDFEditReadHFT_VERSION_4   0x00040000
#define PDFEditReadHFT_VERSION_5   0x00050000
#define PDFEditReadHFT_VERSION_6   0x00060000
#define PDFEditReadHFT_VERSION_8_1   0x00080001
#define PDFEditReadHFT_VERSION_9   0x00090000
#define PDFEditReadHFT_VERSION_10  0x000A0000
#define PDFEditReadHFT_VERSION_11  0x000B0000
#define PDFEditReadHFT_VERSION_12  PDFEditReadHFT_LATEST_VERSION

#include "PDBasicExpT.h"
#include "PEExpT.h"
#include "PEVers.h"

#ifdef __cplusplus
extern "C" {
#endif

#ifdef NPROC /* may be already defined */
#undef NPROC
#endif

#if !PLUGIN
	/* Static link */
	#define NPROC(returnType, name, params)		\
		ACEX1 returnType ACEX2 name params;
	#define UNPROC	NPROC
	#include "PERProcs.h"
	#undef NPROC
	#undef UNPROC
#endif /* !PLUGIN */

#if PLUGIN
	/* HFT version */
	#include "PIRequir.h"

	/* Enumerate the selectors */
	#define NPROC(returnType, name, params)			\
		name##SEL,
	#define UNPROC	NPROC
	
	enum {
		PDFEditReadBAD_SELECTOR,
		#include "PERProcs.h"
		PDFEditReadNUMSELECTORSplusOne
	};
	
	#define PDFEditReadNUMSELECTORS (PDFEditReadNUMSELECTORSplusOne - 1)
	
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
		#include "PERProcs.h"
	#undef NPROC
	#undef UNPROC
	

#if PI_PDFEDIT_READ_VERSION != 0
#ifdef THREAD_SAFE_PDFL
	#define gPDFEditReadHFT (GetHFTLocations()->pdfEditReadHFT)
	#define gPDFEditReadVersion (GetHFTLocations()->pdfEditReadVersion)
#else
	extern HFT gPDFEditReadHFT;
	extern ASUns32 gPDFEditReadVersion;
#endif /* defined THREAD_SAFE_PDFL */
/*# PDFEDIT_READ_VERSION >= 0x00040000 */

#if !STATIC_HFT
	/* Define the macros */

	#define PDEContentCreateFromCosObj (ACROASSERT(gPDFEditReadVersion >=PDFEditReadHFT_VERSION_4), *((PDEContentCreateFromCosObjSELPROTO)(gPDFEditReadHFT[PDEContentCreateFromCosObjSEL]))) 
	#define PDEContentGetAttrs (ACROASSERT(gPDFEditReadVersion >=PDFEditReadHFT_VERSION_4), *((PDEContentGetAttrsSELPROTO)(gPDFEditReadHFT[PDEContentGetAttrsSEL]))) 
	#define PDEContentGetBBox (ACROASSERT(gPDFEditReadVersion >=PDFEditReadHFT_VERSION_4), *((PDEContentGetBBoxSELPROTO)(gPDFEditReadHFT[PDEContentGetBBoxSEL]))) 
	#define PDEContentGetResources (ACROASSERT(gPDFEditReadVersion >=PDFEditReadHFT_VERSION_4), *((PDEContentGetResourcesSELPROTO)(gPDFEditReadHFT[PDEContentGetResourcesSEL]))) 
	#define PDEContentGetNumElems (ACROASSERT(gPDFEditReadVersion >=PDFEditReadHFT_VERSION_4), *((PDEContentGetNumElemsSELPROTO)(gPDFEditReadHFT[PDEContentGetNumElemsSEL]))) 
	#define PDEContentGetElem (ACROASSERT(gPDFEditReadVersion >=PDFEditReadHFT_VERSION_4), *((PDEContentGetElemSELPROTO)(gPDFEditReadHFT[PDEContentGetElemSEL]))) 
	
	#define PDEElementGetBBox (ACROASSERT(gPDFEditReadVersion >=PDFEditReadHFT_VERSION_4), *((PDEElementGetBBoxSELPROTO)(gPDFEditReadHFT[PDEElementGetBBoxSEL]))) 
	#define PDEElementGetGState (ACROASSERT(gPDFEditReadVersion >=PDFEditReadHFT_VERSION_4), *((PDEElementGetGStateSELPROTO)(gPDFEditReadHFT[PDEElementGetGStateSEL]))) 
	#define PDEElementGetMatrix (ACROASSERT(gPDFEditReadVersion >=PDFEditReadHFT_VERSION_4), *((PDEElementGetMatrixSELPROTO)(gPDFEditReadHFT[PDEElementGetMatrixSEL]))) 
	#define PDEElementGetClip (ACROASSERT(gPDFEditReadVersion >=PDFEditReadHFT_VERSION_4), *((PDEElementGetClipSELPROTO)(gPDFEditReadHFT[PDEElementGetClipSEL]))) 
	
	#define PDETextGetNumChars (ACROASSERT(gPDFEditReadVersion >=PDFEditReadHFT_VERSION_4), *((PDETextGetNumCharsSELPROTO)(gPDFEditReadHFT[PDETextGetNumCharsSEL]))) 
	#define PDETextGetNumRuns (ACROASSERT(gPDFEditReadVersion >=PDFEditReadHFT_VERSION_4), *((PDETextGetNumRunsSELPROTO)(gPDFEditReadHFT[PDETextGetNumRunsSEL]))) 
	#define PDETextRunGetCharOffset (ACROASSERT(gPDFEditReadVersion >=PDFEditReadHFT_VERSION_4), *((PDETextRunGetCharOffsetSELPROTO)(gPDFEditReadHFT[PDETextRunGetCharOffsetSEL]))) 
	#define PDETextGetRunForChar (ACROASSERT(gPDFEditReadVersion >=PDFEditReadHFT_VERSION_4), *((PDETextGetRunForCharSELPROTO)(gPDFEditReadHFT[PDETextGetRunForCharSEL]))) 
	#define PDETextRunGetNumChars (ACROASSERT(gPDFEditReadVersion >=PDFEditReadHFT_VERSION_4), *((PDETextRunGetNumCharsSELPROTO)(gPDFEditReadHFT[PDETextRunGetNumCharsSEL])))
	#define PDETextGetBBox (ACROASSERT(gPDFEditReadVersion >=PDFEditReadHFT_VERSION_4), *((PDETextGetBBoxSELPROTO)(gPDFEditReadHFT[PDETextGetBBoxSEL]))) 
	#define PDETextGetGState (ACROASSERT(gPDFEditReadVersion >=PDFEditReadHFT_VERSION_4), *((PDETextGetGStateSELPROTO)(gPDFEditReadHFT[PDETextGetGStateSEL]))) 
	#define PDETextGetTextState (ACROASSERT(gPDFEditReadVersion >=PDFEditReadHFT_VERSION_4), *((PDETextGetTextStateSELPROTO)(gPDFEditReadHFT[PDETextGetTextStateSEL]))) 
	#define PDETextGetFont (ACROASSERT(gPDFEditReadVersion >=PDFEditReadHFT_VERSION_4), *((PDETextGetFontSELPROTO)(gPDFEditReadHFT[PDETextGetFontSEL]))) 
	#define PDETextGetTextMatrix (ACROASSERT(gPDFEditReadVersion >=PDFEditReadHFT_VERSION_4), *((PDETextGetTextMatrixSELPROTO)(gPDFEditReadHFT[PDETextGetTextMatrixSEL]))) 
	#define PDETextGetStrokeMatrix (ACROASSERT(gPDFEditReadVersion >=PDFEditReadHFT_VERSION_4), *((PDETextGetStrokeMatrixSELPROTO)(gPDFEditReadHFT[PDETextGetStrokeMatrixSEL]))) 
	#define PDETextGetAdvanceWidth (ACROASSERT(gPDFEditReadVersion >=PDFEditReadHFT_VERSION_4), *((PDETextGetAdvanceWidthSELPROTO)(gPDFEditReadHFT[PDETextGetAdvanceWidthSEL]))) 
	#define PDETextGetText (ACROASSERT(gPDFEditReadVersion >=PDFEditReadHFT_VERSION_4), *((PDETextGetTextSELPROTO)(gPDFEditReadHFT[PDETextGetTextSEL]))) 
	
	#define PDEPathGetData (ACROASSERT(gPDFEditReadVersion >=PDFEditReadHFT_VERSION_4), *((PDEPathGetDataSELPROTO)(gPDFEditReadHFT[PDEPathGetDataSEL]))) 
	#define PDEPathGetPaintOp (ACROASSERT(gPDFEditReadVersion >=PDFEditReadHFT_VERSION_4), *((PDEPathGetPaintOpSELPROTO)(gPDFEditReadHFT[PDEPathGetPaintOpSEL]))) 
	
	#define PDEImageGetAttrs (ACROASSERT(gPDFEditReadVersion >=PDFEditReadHFT_VERSION_4), *((PDEImageGetAttrsSELPROTO)(gPDFEditReadHFT[PDEImageGetAttrsSEL]))) 
	#define PDEImageGetColorSpace (ACROASSERT(gPDFEditReadVersion >=PDFEditReadHFT_VERSION_4), *((PDEImageGetColorSpaceSELPROTO)(gPDFEditReadHFT[PDEImageGetColorSpaceSEL]))) 
	#define PDEImageIsCosObj (ACROASSERT(gPDFEditReadVersion >=PDFEditReadHFT_VERSION_4), *((PDEImageIsCosObjSELPROTO)(gPDFEditReadHFT[PDEImageIsCosObjSEL]))) 
	#define PDEImageDataIsEncoded (ACROASSERT(gPDFEditReadVersion >=PDFEditReadHFT_VERSION_4), *((PDEImageDataIsEncodedSELPROTO)(gPDFEditReadHFT[PDEImageDataIsEncodedSEL]))) 
	#define PDEImageGetData (ACROASSERT(gPDFEditReadVersion >=PDFEditReadHFT_VERSION_4), *((PDEImageGetDataSELPROTO)(gPDFEditReadHFT[PDEImageGetDataSEL]))) 
	#define PDEImageGetDataStm (ACROASSERT(gPDFEditReadVersion >=PDFEditReadHFT_VERSION_4), *((PDEImageGetDataStmSELPROTO)(gPDFEditReadHFT[PDEImageGetDataStmSEL]))) 
	#define PDEImageGetDataLen (ACROASSERT(gPDFEditReadVersion >=PDFEditReadHFT_VERSION_4), *((PDEImageGetDataLenSELPROTO)(gPDFEditReadHFT[PDEImageGetDataLenSEL]))) 
	#define PDEImageGetFilterArray (ACROASSERT(gPDFEditReadVersion >=PDFEditReadHFT_VERSION_4), *((PDEImageGetFilterArraySELPROTO)(gPDFEditReadHFT[PDEImageGetFilterArraySEL]))) 
	#define PDEImageGetCosObj (ACROASSERT(gPDFEditReadVersion >=PDFEditReadHFT_VERSION_4), *((PDEImageGetCosObjSELPROTO)(gPDFEditReadHFT[PDEImageGetCosObjSEL]))) 
	
	#define PDEClipGetNumElems (ACROASSERT(gPDFEditReadVersion >=PDFEditReadHFT_VERSION_4), *((PDEClipGetNumElemsSELPROTO)(gPDFEditReadHFT[PDEClipGetNumElemsSEL]))) 
	#define PDEClipGetElem (ACROASSERT(gPDFEditReadVersion >=PDFEditReadHFT_VERSION_4), *((PDEClipGetElemSELPROTO)(gPDFEditReadHFT[PDEClipGetElemSEL]))) 
	
	#define PDEXObjectGetCosObj (ACROASSERT(gPDFEditReadVersion >=PDFEditReadHFT_VERSION_4), *((PDEXObjectGetCosObjSELPROTO)(gPDFEditReadHFT[PDEXObjectGetCosObjSEL]))) 
	
	#define PDEFormGetCosObj (ACROASSERT(gPDFEditReadVersion >=PDFEditReadHFT_VERSION_4), *((PDEFormGetCosObjSELPROTO)(gPDFEditReadHFT[PDEFormGetCosObjSEL]))) 
	
	#define PDEPSGetAttrs (ACROASSERT(gPDFEditReadVersion >=PDFEditReadHFT_VERSION_4), *((PDEPSGetAttrsSELPROTO)(gPDFEditReadHFT[PDEPSGetAttrsSEL]))) 
	#define PDEPSGetData (ACROASSERT(gPDFEditReadVersion >=PDFEditReadHFT_VERSION_4), *((PDEPSGetDataSELPROTO)(gPDFEditReadHFT[PDEPSGetDataSEL]))) 
	#define PDEPSGetDataStm (ACROASSERT(gPDFEditReadVersion >=PDFEditReadHFT_VERSION_4), *((PDEPSGetDataStmSELPROTO)(gPDFEditReadHFT[PDEPSGetDataStmSEL]))) 
	
	#define PDEFontGetAttrs (ACROASSERT(gPDFEditReadVersion >=PDFEditReadHFT_VERSION_4), *((PDEFontGetAttrsSELPROTO)(gPDFEditReadHFT[PDEFontGetAttrsSEL]))) 
	#define PDEFontGetWidths (ACROASSERT(gPDFEditReadVersion >=PDFEditReadHFT_VERSION_4), *((PDEFontGetWidthsSELPROTO)(gPDFEditReadHFT[PDEFontGetWidthsSEL]))) 
	#define PDEFontGetCosObj (ACROASSERT(gPDFEditReadVersion >=PDFEditReadHFT_VERSION_4), *((PDEFontGetCosObjSELPROTO)(gPDFEditReadHFT[PDEFontGetCosObjSEL]))) 
	
	#define PDEColorSpaceGetName (ACROASSERT(gPDFEditReadVersion >=PDFEditReadHFT_VERSION_4), *((PDEColorSpaceGetNameSELPROTO)(gPDFEditReadHFT[PDEColorSpaceGetNameSEL]))) 
	#define PDEColorSpaceGetCosObj (ACROASSERT(gPDFEditReadVersion >=PDFEditReadHFT_VERSION_4), *((PDEColorSpaceGetCosObjSELPROTO)(gPDFEditReadHFT[PDEColorSpaceGetCosObjSEL]))) 
	#define PDEColorSpaceGetNumComps (ACROASSERT(gPDFEditReadVersion >=PDFEditReadHFT_VERSION_4), *((PDEColorSpaceGetNumCompsSELPROTO)(gPDFEditReadHFT[PDEColorSpaceGetNumCompsSEL]))) 
	#define PDEColorSpaceGetBase (ACROASSERT(gPDFEditReadVersion >=PDFEditReadHFT_VERSION_4), *((PDEColorSpaceGetBaseSELPROTO)(gPDFEditReadHFT[PDEColorSpaceGetBaseSEL]))) 
	#define PDEColorSpaceGetHiVal (ACROASSERT(gPDFEditReadVersion >=PDFEditReadHFT_VERSION_4), *((PDEColorSpaceGetHiValSELPROTO)(gPDFEditReadHFT[PDEColorSpaceGetHiValSEL]))) 
	#define PDEColorSpaceGetCTable (ACROASSERT(gPDFEditReadVersion >=PDFEditReadHFT_VERSION_4), *((PDEColorSpaceGetCTableSELPROTO)(gPDFEditReadHFT[PDEColorSpaceGetCTableSEL]))) 
	
	#define PDEObjectGetType (ACROASSERT(gPDFEditReadVersion >=PDFEditReadHFT_VERSION_4), *((PDEObjectGetTypeSELPROTO)(gPDFEditReadHFT[PDEObjectGetTypeSEL]))) 
	#define PDEObjectDump (ACROASSERT(gPDFEditReadVersion >=PDFEditReadHFT_VERSION_4), *((PDEObjectDumpSELPROTO)(gPDFEditReadHFT[PDEObjectDumpSEL]))) 
	
	#define PDEAcquire (ACROASSERT(gPDFEditReadVersion >=PDFEditReadHFT_VERSION_4), *((PDEAcquireSELPROTO)(gPDFEditReadHFT[PDEAcquireSEL]))) 
	#define PDERelease (ACROASSERT(gPDFEditReadVersion >=PDFEditReadHFT_VERSION_4), *((PDEReleaseSELPROTO)(gPDFEditReadHFT[PDEReleaseSEL]))) 
	
	#define PDELogDump (ACROASSERT(gPDFEditReadVersion >=PDFEditReadHFT_VERSION_4), *((PDELogDumpSELPROTO)(gPDFEditReadHFT[PDELogDumpSEL]))) 
	#define PDEAttrEnumTable (ACROASSERT(gPDFEditReadVersion >=PDFEditReadHFT_VERSION_4), *((PDEAttrEnumTableSELPROTO)(gPDFEditReadHFT[PDEAttrEnumTableSEL]))) 

	/* Methods added after 0.2 */		
	#define PDEExtGStateGetCosObj (ACROASSERT(gPDFEditReadVersion >=PDFEditReadHFT_VERSION_4), *((PDEExtGStateGetCosObjSELPROTO)(gPDFEditReadHFT[PDEExtGStateGetCosObjSEL]))) 
	#define PDETextGetQuad (ACROASSERT(gPDFEditReadVersion >=PDFEditReadHFT_VERSION_4), *((PDETextGetQuadSELPROTO)(gPDFEditReadHFT[PDETextGetQuadSEL]))) 
	#define PDEPlaceGetMCTag (ACROASSERT(gPDFEditReadVersion >=PDFEditReadHFT_VERSION_4), *((PDEPlaceGetMCTagSELPROTO)(gPDFEditReadHFT[PDEPlaceGetMCTagSEL]))) 
	#define PDEPlaceGetDict (ACROASSERT(gPDFEditReadVersion >=PDFEditReadHFT_VERSION_4), *((PDEPlaceGetDictSELPROTO)(gPDFEditReadHFT[PDEPlaceGetDictSEL]))) 
	#define PDEContainerGetMCTag (ACROASSERT(gPDFEditReadVersion >=PDFEditReadHFT_VERSION_4), *((PDEContainerGetMCTagSELPROTO)(gPDFEditReadHFT[PDEContainerGetMCTagSEL]))) 
	#define PDEContainerGetDict (ACROASSERT(gPDFEditReadVersion >=PDFEditReadHFT_VERSION_4), *((PDEContainerGetDictSELPROTO)(gPDFEditReadHFT[PDEContainerGetDictSEL]))) 
	#define PDEContainerGetContent (ACROASSERT(gPDFEditReadVersion >=PDFEditReadHFT_VERSION_4), *((PDEContainerGetContentSELPROTO)(gPDFEditReadHFT[PDEContainerGetContentSEL]))) 
	#define PDEColorSpaceGetBaseNumComps (ACROASSERT(gPDFEditReadVersion >=PDFEditReadHFT_VERSION_4), *((PDEColorSpaceGetBaseNumCompsSELPROTO)(gPDFEditReadHFT[PDEColorSpaceGetBaseNumCompsSEL])))
	/* Methods added after 0.4 */
	#define PDEDefaultGState (ACROASSERT(gPDFEditReadVersion >=PDFEditReadHFT_VERSION_4), *((PDEDefaultGStateSELPROTO)(gPDFEditReadHFT[PDEDefaultGStateSEL]))) 
	/* Methods added after 0.5 */
	#define PDEPatternGetCosObj (ACROASSERT(gPDFEditReadVersion >=PDFEditReadHFT_VERSION_4), *((PDEPatternGetCosObjSELPROTO)(gPDFEditReadHFT[PDEPatternGetCosObjSEL]))) 
	/* Methods added after 0.6 */
	#define PDEEnumElements (ACROASSERT(gPDFEditReadVersion >=PDFEditReadHFT_VERSION_4), *((PDEEnumElementsSELPROTO)(gPDFEditReadHFT[PDEEnumElementsSEL]))) 
	/* Methods added after 0.7 */
	#define PDEFontSumWidths (ACROASSERT(gPDFEditReadVersion >=PDFEditReadHFT_VERSION_4), *((PDEFontSumWidthsSELPROTO)(gPDFEditReadHFT[PDEFontSumWidthsSEL]))) 
	#define PDEFontGetNumCodeBytes (ACROASSERT(gPDFEditReadVersion >=PDFEditReadHFT_VERSION_4), *((PDEFontGetNumCodeBytesSELPROTO)(gPDFEditReadHFT[PDEFontGetNumCodeBytesSEL]))) 
	#define PDEDeviceNColorsGetColorValue (ACROASSERT(gPDFEditReadVersion >=PDFEditReadHFT_VERSION_4), *((PDEDeviceNColorsGetColorValueSELPROTO)(gPDFEditReadHFT[PDEDeviceNColorsGetColorValueSEL]))) 
	#define PDEFontIsMultiByte (ACROASSERT(gPDFEditReadVersion >=PDFEditReadHFT_VERSION_4), *((PDEFontIsMultiByteSELPROTO)(gPDFEditReadHFT[PDEFontIsMultiByteSEL]))) 
	#define PDETextGetNumBytes (ACROASSERT(gPDFEditReadVersion >=PDFEditReadHFT_VERSION_4), *((PDETextGetNumBytesSELPROTO)(gPDFEditReadHFT[PDETextGetNumBytesSEL]))) 

	#define PDEGroupGetContent (ACROASSERT(gPDFEditReadVersion >=PDFEditReadHFT_VERSION_4), *((PDEGroupGetContentSELPROTO)(gPDFEditReadHFT[PDEGroupGetContentSEL]))) 
	#define PDEClipFlattenedEnumElems (ACROASSERT(gPDFEditReadVersion >=PDFEditReadHFT_VERSION_4), *((PDEClipFlattenedEnumElemsSELPROTO)(gPDFEditReadHFT[PDEClipFlattenedEnumElemsSEL]))) 	

	#define PDEElementIsAtPoint (ACROASSERT(gPDFEditReadVersion >=PDFEditReadHFT_VERSION_4), *((PDEElementIsAtPointSELPROTO)(gPDFEditReadHFT[PDEElementIsAtPointSEL]))) 	
	#define PDEElementIsAtRect (ACROASSERT(gPDFEditReadVersion >=PDFEditReadHFT_VERSION_4), *((PDEElementIsAtRectSELPROTO)(gPDFEditReadHFT[PDEElementIsAtRectSEL]))) 	
	#define PDETextIsAtPoint (ACROASSERT(gPDFEditReadVersion >=PDFEditReadHFT_VERSION_4), *((PDETextIsAtPointSELPROTO)(gPDFEditReadHFT[PDETextIsAtPointSEL]))) 	
	#define PDETextIsAtRect (ACROASSERT(gPDFEditReadVersion >=PDFEditReadHFT_VERSION_4), *((PDETextIsAtRectSELPROTO)(gPDFEditReadHFT[PDETextIsAtRectSEL]))) 	
	#define PDEFontGetOneByteEncoding (ACROASSERT(gPDFEditReadVersion >=PDFEditReadHFT_VERSION_4), *((PDEFontGetOneByteEncodingSELPROTO)(gPDFEditReadHFT[PDEFontGetOneByteEncodingSEL]))) 	
	#define PDEShadingGetCosObj (ACROASSERT(gPDFEditReadVersion >=PDFEditReadHFT_VERSION_4), *((PDEShadingGetCosObjSELPROTO)(gPDFEditReadHFT[PDEShadingGetCosObjSEL]))) 	

	#define PDEUnknownGetOpName (ACROASSERT(gPDFEditReadVersion >=PDFEditReadHFT_VERSION_4), *((PDEUnknownGetOpNameSELPROTO)(gPDFEditReadHFT[PDEUnknownGetOpNameSEL]))) 	
	#define PDEContentGetDefaultColorSpace (ACROASSERT(gPDFEditReadVersion >=PDFEditReadHFT_VERSION_4), *((PDEContentGetDefaultColorSpaceSELPROTO)(gPDFEditReadHFT[PDEContentGetDefaultColorSpaceSEL]))) 	
	#define PDEImageGetDecodeArray (ACROASSERT(gPDFEditReadVersion >=PDFEditReadHFT_VERSION_4), *((PDEImageGetDecodeArraySELPROTO)(gPDFEditReadHFT[PDEImageGetDecodeArraySEL]))) 	


/* PI_PDFEDIT_READ_VERSION >= 0x00050000 */

	#define PDEBeginContainerGetMCTag (ACROASSERT(gPDFEditReadVersion >=PDFEditReadHFT_VERSION_5), *((PDEBeginContainerGetMCTagSELPROTO)(gPDFEditReadHFT[PDEBeginContainerGetMCTagSEL]))) 
	#define PDEBeginContainerGetDict (ACROASSERT(gPDFEditReadVersion >=PDFEditReadHFT_VERSION_5), *((PDEBeginContainerGetDictSELPROTO)(gPDFEditReadHFT[PDEBeginContainerGetDictSEL]))) 

	#define PDESoftMaskGetCosObj (ACROASSERT(gPDFEditReadVersion >=PDFEditReadHFT_VERSION_5), *((PDESoftMaskGetCosObjSELPROTO)(gPDFEditReadHFT[PDESoftMaskGetCosObjSEL])))
	#define PDESoftMaskAcquireForm (ACROASSERT(gPDFEditReadVersion >=PDFEditReadHFT_VERSION_5), *((PDESoftMaskAcquireFormSELPROTO)(gPDFEditReadHFT[PDESoftMaskAcquireFormSEL])))
	#define PDESoftMaskGetBackdropColor (ACROASSERT(gPDFEditReadVersion >=PDFEditReadHFT_VERSION_5), *((PDESoftMaskGetBackdropColorSELPROTO)(gPDFEditReadHFT[PDESoftMaskGetBackdropColorSEL])))
	#define PDESoftMaskGetTransferFunction (ACROASSERT(gPDFEditReadVersion >=PDFEditReadHFT_VERSION_5), *((PDESoftMaskGetTransferFunctionSELPROTO)(gPDFEditReadHFT[PDESoftMaskGetTransferFunctionSEL])))

	#define PDEXGroupGetCosObj (ACROASSERT(gPDFEditReadVersion >=PDFEditReadHFT_VERSION_5), *((PDEXGroupGetCosObjSELPROTO)(gPDFEditReadHFT[PDEXGroupGetCosObjSEL])))
	#define PDEXGroupGetKnockout (ACROASSERT(gPDFEditReadVersion >=PDFEditReadHFT_VERSION_5), *((PDEXGroupGetKnockoutSELPROTO)(gPDFEditReadHFT[PDEXGroupGetKnockoutSEL])))
	#define PDEXGroupGetIsolated (ACROASSERT(gPDFEditReadVersion >=PDFEditReadHFT_VERSION_5), *((PDEXGroupGetIsolatedSELPROTO)(gPDFEditReadHFT[PDEXGroupGetIsolatedSEL])))
	#define PDEXGroupAcquireColorSpace (ACROASSERT(gPDFEditReadVersion >=PDFEditReadHFT_VERSION_5), *((PDEXGroupAcquireColorSpaceSELPROTO)(gPDFEditReadHFT[PDEXGroupAcquireColorSpaceSEL])))

	#define PDEFormAcquireXGroup (ACROASSERT(gPDFEditReadVersion >=PDFEditReadHFT_VERSION_5), *((PDEFormAcquireXGroupSELPROTO)(gPDFEditReadHFT[PDEFormAcquireXGroupSEL])))
	#define PDEFormHasXGroup (ACROASSERT(gPDFEditReadVersion >=PDFEditReadHFT_VERSION_5), *((PDEFormHasXGroupSELPROTO)(gPDFEditReadHFT[PDEFormHasXGroupSEL])))

	#define PDEElementHasGState (ACROASSERT(gPDFEditReadVersion >=PDFEditReadHFT_VERSION_5), *((PDEElementHasGStateSELPROTO)(gPDFEditReadHFT[PDEElementHasGStateSEL]))) 

	#define PDEExtGStateGetOPM (ACROASSERT(gPDFEditReadVersion >=PDFEditReadHFT_VERSION_5), *((PDEExtGStateGetOPMSELPROTO)(gPDFEditReadHFT[PDEExtGStateGetOPMSEL])))
	#define PDEExtGStateGetOPFill (ACROASSERT(gPDFEditReadVersion >=PDFEditReadHFT_VERSION_5), *((PDEExtGStateGetOPFillSELPROTO)(gPDFEditReadHFT[PDEExtGStateGetOPFillSEL])))
	#define PDEExtGStateGetOPStroke (ACROASSERT(gPDFEditReadVersion >=PDFEditReadHFT_VERSION_5), *((PDEExtGStateGetOPStrokeSELPROTO)(gPDFEditReadHFT[PDEExtGStateGetOPStrokeSEL])))
	#define PDEExtGStateGetOpacityFill (ACROASSERT(gPDFEditReadVersion >=PDFEditReadHFT_VERSION_5), *((PDEExtGStateGetOpacityFillSELPROTO)(gPDFEditReadHFT[PDEExtGStateGetOpacityFillSEL])))
	#define PDEExtGStateGetOpacityStroke (ACROASSERT(gPDFEditReadVersion >=PDFEditReadHFT_VERSION_5), *((PDEExtGStateGetOpacityStrokeSELPROTO)(gPDFEditReadHFT[PDEExtGStateGetOpacityStrokeSEL])))
	#define PDEExtGStateGetBlendMode (ACROASSERT(gPDFEditReadVersion >=PDFEditReadHFT_VERSION_5), *((PDEExtGStateGetBlendModeSELPROTO)(gPDFEditReadHFT[PDEExtGStateGetBlendModeSEL])))
	#define PDEExtGStateGetAIS (ACROASSERT(gPDFEditReadVersion >=PDFEditReadHFT_VERSION_5), *((PDEExtGStateGetAISSELPROTO)(gPDFEditReadHFT[PDEExtGStateGetAISSEL])))
	#define PDEExtGStateHasSoftMask (ACROASSERT(gPDFEditReadVersion >=PDFEditReadHFT_VERSION_5), *((PDEExtGStateHasSoftMaskSELPROTO)(gPDFEditReadHFT[PDEExtGStateHasSoftMaskSEL])))
	#define PDEExtGStateAcquireSoftMask (ACROASSERT(gPDFEditReadVersion >=PDFEditReadHFT_VERSION_5), *((PDEExtGStateAcquireSoftMaskSELPROTO)(gPDFEditReadHFT[PDEExtGStateAcquireSoftMaskSEL])))

	#define PDEImageHasSMask (ACROASSERT(gPDFEditReadVersion >=PDFEditReadHFT_VERSION_5), *((PDEImageHasSMaskSELPROTO)(gPDFEditReadHFT[PDEImageHasSMaskSEL])))
	#define PDEImageGetSMask (ACROASSERT(gPDFEditReadVersion >=PDFEditReadHFT_VERSION_5), *((PDEImageGetSMaskSELPROTO)(gPDFEditReadHFT[PDEImageGetSMaskSEL])))
	#define PDEImageGetMatteArray (ACROASSERT(gPDFEditReadVersion >=PDFEditReadHFT_VERSION_5), *((PDEImageGetMatteArraySELPROTO)(gPDFEditReadHFT[PDEImageGetMatteArraySEL])))

	#define PDEExtGStateGetTK (ACROASSERT(gPDFEditReadVersion >=PDFEditReadHFT_VERSION_5), *((PDEExtGStateGetTKSELPROTO)(gPDFEditReadHFT[PDEExtGStateGetTKSEL])))

	#define PDETextGetState (ACROASSERT(gPDFEditReadVersion >=PDFEditReadHFT_VERSION_5), *((PDETextGetStateSELPROTO)(gPDFEditReadHFT[PDETextGetStateSEL]))) 

	#define PDSysEncodingGetWMode (ACROASSERT(gPDFEditReadVersion >=PDFEditReadHFT_VERSION_5), *((PDSysEncodingGetWModeSELPROTO)(gPDFEditReadHFT[PDSysEncodingGetWModeSEL]))) 
	#define PDSysEncodingIsIdentity (ACROASSERT(gPDFEditReadVersion >=PDFEditReadHFT_VERSION_5), *((PDSysEncodingIsIdentitySELPROTO)(gPDFEditReadHFT[PDSysEncodingIsIdentitySEL]))) 
	#define PDSysEncodingIsMultiByte (ACROASSERT(gPDFEditReadVersion >=PDFEditReadHFT_VERSION_5), *((PDSysEncodingIsMultiByteSELPROTO)(gPDFEditReadHFT[PDSysEncodingIsMultiByteSEL]))) 
	#define PDEExtGStateGetSA (ACROASSERT(gPDFEditReadVersion >=PDFEditReadHFT_VERSION_5), *((PDEExtGStateGetSASELPROTO)(gPDFEditReadHFT[PDEExtGStateGetSASEL])))
	#define PDESoftMaskGetName (ACROASSERT(gPDFEditReadVersion >=PDFEditReadHFT_VERSION_5), *((PDESoftMaskGetNameSELPROTO)(gPDFEditReadHFT[PDESoftMaskGetNameSEL])))

	#define PDETextGetMatrix (ACROASSERT(gPDFEditReadVersion >=PDFEditReadHFT_VERSION_5), *((PDETextGetMatrixSELPROTO)(gPDFEditReadHFT[PDETextGetMatrixSEL])))

/* BEGIN Optional Content API calls */
	#define PDEElementGetOCMD (ACROASSERT(gPDFEditReadVersion >= PDFEditReadHFT_VERSION_6), *((PDEElementGetOCMDSELPROTO)(gPDFEditReadHFT[PDEElementGetOCMDSEL])))
	#define PDEElementIsCurrentlyVisible (ACROASSERT(gPDFEditReadVersion >= PDFEditReadHFT_VERSION_6), *((PDEElementIsCurrentlyVisibleSELPROTO)(gPDFEditReadHFT[PDEElementIsCurrentlyVisibleSEL])))
	#define PDEElementGetAllVisibilities (ACROASSERT(gPDFEditReadVersion >= PDFEditReadHFT_VERSION_6), *((PDEElementGetAllVisibilitiesSELPROTO)(gPDFEditReadHFT[PDEElementGetAllVisibilitiesSEL])))
	#define PDEElementMakeVisible (ACROASSERT(gPDFEditReadVersion >= PDFEditReadHFT_VERSION_6), *((PDEElementMakeVisibleSELPROTO)(gPDFEditReadHFT[PDEElementMakeVisibleSEL])))
/* END Optional Content API calls */

	#define PDEFontIsEmbedded (ACROASSERT(gPDFEditReadVersion >=PDFEditReadHFT_VERSION_6), *((PDEFontIsEmbeddedSELPROTO)(gPDFEditReadHFT[PDEFontIsEmbeddedSEL])))
	#define PDEFontGetSysFont (ACROASSERT(gPDFEditReadVersion >=PDFEditReadHFT_VERSION_6), *((PDEFontGetSysFontSELPROTO)(gPDFEditReadHFT[PDEFontGetSysFontSEL])))
	#define PDEFontGetSysEncoding (ACROASSERT(gPDFEditReadVersion >=PDFEditReadHFT_VERSION_6), *((PDEFontGetSysEncodingSELPROTO)(gPDFEditReadHFT[PDEFontGetSysEncodingSEL])))
	#define PDETextGetAdvance (ACROASSERT(gPDFEditReadVersion >=PDFEditReadHFT_VERSION_6), *((PDETextGetAdvanceSELPROTO)(gPDFEditReadHFT[PDETextGetAdvanceSEL])))
	#define PDETextItemGetFont (ACROASSERT(gPDFEditReadVersion >=PDFEditReadHFT_VERSION_6), *((PDETextItemGetFontSELPROTO)(gPDFEditReadHFT[PDETextItemGetFontSEL])))
	#define PDETextItemGetTextMatrix (ACROASSERT(gPDFEditReadVersion >=PDFEditReadHFT_VERSION_6), *((PDETextItemGetTextMatrixSELPROTO)(gPDFEditReadHFT[PDETextItemGetTextMatrixSEL])))
	#define PDETextItemGetTextState (ACROASSERT(gPDFEditReadVersion >=PDFEditReadHFT_VERSION_6), *((PDETextItemGetTextStateSELPROTO)(gPDFEditReadHFT[PDETextItemGetTextStateSEL])))
	#define PDETextItemGetTextLen (ACROASSERT(gPDFEditReadVersion >=PDFEditReadHFT_VERSION_6), *((PDETextItemGetTextLenSELPROTO)(gPDFEditReadHFT[PDETextItemGetTextLenSEL])))
	#define PDETextItemCopyText (ACROASSERT(gPDFEditReadVersion >=PDFEditReadHFT_VERSION_6), *((PDETextItemCopyTextSELPROTO)(gPDFEditReadHFT[PDETextItemCopyTextSEL])))
	#define PDETextItemGetGState (ACROASSERT(gPDFEditReadVersion >=PDFEditReadHFT_VERSION_6), *((PDETextItemGetGStateSELPROTO)(gPDFEditReadHFT[PDETextItemGetGStateSEL])))
	#define PDETextGetItem (ACROASSERT(gPDFEditReadVersion >=PDFEditReadHFT_VERSION_6), *((PDETextGetItemSELPROTO)(gPDFEditReadHFT[PDETextGetItemSEL])))

/* Support for 16 bpc Flate and JPX images. */
      /* #define XXXXXX (ACROASSERT(gPDFEditReadVersion >=PDFEditReadHFT_VERSION_6), *((XXXXXXSELPROTO)(gPDFEditReadHFT[XXXXXXSEL]))) */
    #define PDEImageGetType (ACROASSERT(gPDFEditReadVersion >=PDFEditReadHFT_VERSION_6), *((PDEImageGetTypeSELPROTO)(gPDFEditReadHFT[PDEImageGetTypeSEL])))
    #define PDEImageAcquireImageFlate (ACROASSERT(gPDFEditReadVersion >=PDFEditReadHFT_VERSION_6), *((PDEImageAcquireImageFlateSELPROTO)(gPDFEditReadHFT[PDEImageAcquireImageFlateSEL])))
    #define PDEImageAcquireImageJPX (ACROASSERT(gPDFEditReadVersion >=PDFEditReadHFT_VERSION_6), *((PDEImageAcquireImageJPXSELPROTO)(gPDFEditReadHFT[PDEImageAcquireImageJPXSEL])))

    #define PDEImageJPXGetCosObj (ACROASSERT(gPDFEditReadVersion >=PDFEditReadHFT_VERSION_6), *((PDEImageJPXGetCosObjSELPROTO)(gPDFEditReadHFT[PDEImageJPXGetCosObjSEL])))

    #define PDEImageFlateGetCosObj (ACROASSERT(gPDFEditReadVersion >=PDFEditReadHFT_VERSION_6), *((PDEImageFlateGetCosObjSELPROTO)(gPDFEditReadHFT[PDEImageFlateGetCosObjSEL])))
    #define PDEImageFlateGetAttrs (ACROASSERT(gPDFEditReadVersion >=PDFEditReadHFT_VERSION_6), *((PDEImageFlateGetAttrsSELPROTO)(gPDFEditReadHFT[PDEImageFlateGetAttrsSEL])))
    #define PDEImageFlateAcquireColorSpace (ACROASSERT(gPDFEditReadVersion >=PDFEditReadHFT_VERSION_6), *((PDEImageFlateAcquireColorSpaceSELPROTO)(gPDFEditReadHFT[PDEImageFlateAcquireColorSpaceSEL])))
    #define PDEImageFlateGetDataStm (ACROASSERT(gPDFEditReadVersion >=PDFEditReadHFT_VERSION_6), *((PDEImageFlateGetDataStmSELPROTO)(gPDFEditReadHFT[PDEImageFlateGetDataStmSEL])))

    #define PDEImageJPXGetAttrs (ACROASSERT(gPDFEditReadVersion >=PDFEditReadHFT_VERSION_6), *((PDEImageJPXGetAttrsSELPROTO)(gPDFEditReadHFT[PDEImageJPXGetAttrsSEL])))
    #define PDEImageJPXAcquireColorSpace (ACROASSERT(gPDFEditReadVersion >=PDFEditReadHFT_VERSION_6), *((PDEImageJPXAcquireColorSpaceSELPROTO)(gPDFEditReadHFT[PDEImageJPXAcquireColorSpaceSEL])))
    #define PDEImageJPXGetDataStm (ACROASSERT(gPDFEditReadVersion >=PDFEditReadHFT_VERSION_6), *((PDEImageJPXGetDataStmSELPROTO)(gPDFEditReadHFT[PDEImageJPXGetDataStmSEL])))
    #define PDEImageJPXGetNumColorSpaces (ACROASSERT(gPDFEditReadVersion >=PDFEditReadHFT_VERSION_6), *((PDEImageJPXGetNumColorSpacesSELPROTO)(gPDFEditReadHFT[PDEImageJPXGetNumColorSpacesSEL])))
    #define PDEImageJPXAcquireJPXColorSpace (ACROASSERT(gPDFEditReadVersion >=PDFEditReadHFT_VERSION_6), *((PDEImageJPXAcquireJPXColorSpaceSELPROTO)(gPDFEditReadHFT[PDEImageJPXAcquireJPXColorSpaceSEL])))
    #define PDEImageJPXHasPalette (ACROASSERT(gPDFEditReadVersion >=PDFEditReadHFT_VERSION_6), *((PDEImageJPXHasPaletteSELPROTO)(gPDFEditReadHFT[PDEImageJPXHasPaletteSEL])))
    #define PDEImageJPXAcquirePalette (ACROASSERT(gPDFEditReadVersion >=PDFEditReadHFT_VERSION_6), *((PDEImageJPXAcquirePaletteSELPROTO)(gPDFEditReadHFT[PDEImageJPXAcquirePaletteSEL])))

    #define JPXPaletteGetNumEntries (ACROASSERT(gPDFEditReadVersion >=PDFEditReadHFT_VERSION_6), *((JPXPaletteGetNumEntriesSELPROTO)(gPDFEditReadHFT[JPXPaletteGetNumEntriesSEL])))
    #define JPXPaletteGetBitDepths (ACROASSERT(gPDFEditReadVersion >=PDFEditReadHFT_VERSION_6), *((JPXPaletteGetBitDepthsSELPROTO)(gPDFEditReadHFT[JPXPaletteGetBitDepthsSEL])))
    #define JPXPaletteGetNumComponents (ACROASSERT(gPDFEditReadVersion >=PDFEditReadHFT_VERSION_6), *((JPXPaletteGetNumComponentsSELPROTO)(gPDFEditReadHFT[JPXPaletteGetNumComponentsSEL])))
    #define JPXPaletteGetTable (ACROASSERT(gPDFEditReadVersion >=PDFEditReadHFT_VERSION_6), *((JPXPaletteGetTableSELPROTO)(gPDFEditReadHFT[JPXPaletteGetTableSEL])))

    #define JPXColorSpaceAcquireNext (ACROASSERT(gPDFEditReadVersion >=PDFEditReadHFT_VERSION_6), *((JPXColorSpaceAcquireNextSELPROTO)(gPDFEditReadHFT[JPXColorSpaceAcquireNextSEL])))
    #define JPXColorSpaceGetType (ACROASSERT(gPDFEditReadVersion >=PDFEditReadHFT_VERSION_6), *((JPXColorSpaceGetTypeSELPROTO)(gPDFEditReadHFT[JPXColorSpaceGetTypeSEL])))
    #define JPXColorSpaceGetEnumAttrs (ACROASSERT(gPDFEditReadVersion >=PDFEditReadHFT_VERSION_6), *((JPXColorSpaceGetEnumAttrsSELPROTO)(gPDFEditReadHFT[JPXColorSpaceGetEnumAttrsSEL])))
    #define JPXColorSpaceGetProfile (ACROASSERT(gPDFEditReadVersion >=PDFEditReadHFT_VERSION_6), *((JPXColorSpaceGetProfileSELPROTO)(gPDFEditReadHFT[JPXColorSpaceGetProfileSEL])))

	#define PDEColorSpaceGetStruct (ACROASSERT(gPDFEditReadVersion >=PDFEditReadHFT_VERSION_8_1), *((PDEColorSpaceGetStructSELPROTO) (gPDFEditReadHFT[PDEColorSpaceGetStructSEL])))

	#define PDEImageGetColorSpaceEx (ACROASSERT(gPDFEditReadVersion >=PDFEditReadHFT_VERSION_9), *((PDEImageGetColorSpaceExSELPROTO)(gPDFEditReadHFT[PDEImageGetColorSpaceExSEL])))
	#define PDEFormGetContentToCosObjFlags (ACROASSERT(gPDFEditReadVersion >=PDFEditReadHFT_VERSION_9), *((PDEFormGetContentToCosObjFlagsSELPROTO)(gPDFEditReadHFT[PDEFormGetContentToCosObjFlagsSEL])))
	#define PDEElementGetMatrixEx (ACROASSERT(gPDFEditReadVersion >=PDFEditReadHFT_VERSION_10), *((PDEElementGetMatrixExSELPROTO)(gPDFEditReadHFT[PDEElementGetMatrixExSEL]))) 
	#define PDETextGetTextMatrixEx (ACROASSERT(gPDFEditReadVersion >=PDFEditReadHFT_VERSION_10), *((PDETextGetTextMatrixExSELPROTO)(gPDFEditReadHFT[PDETextGetTextMatrixExSEL]))) 
	#define PDETextGetStrokeMatrixEx (ACROASSERT(gPDFEditReadVersion >=PDFEditReadHFT_VERSION_10), *((PDETextGetStrokeMatrixExSELPROTO)(gPDFEditReadHFT[PDETextGetStrokeMatrixExSEL]))) 
	#define PDESoftMaskAcquireFormEx (ACROASSERT(gPDFEditReadVersion >=PDFEditReadHFT_VERSION_10), *((PDESoftMaskAcquireFormExSELPROTO)(gPDFEditReadHFT[PDESoftMaskAcquireFormExSEL])))
	#define PDETextItemGetTextMatrixEx (ACROASSERT(gPDFEditReadVersion >=PDFEditReadHFT_VERSION_10), *((PDETextItemGetTextMatrixExSELPROTO)(gPDFEditReadHFT[PDETextItemGetTextMatrixExSEL])))
	#define PDETextGetMatrixEx (ACROASSERT(gPDFEditReadVersion >=PDFEditReadHFT_VERSION_10), *((PDETextGetMatrixExSELPROTO)(gPDFEditReadHFT[PDETextGetMatrixExSEL])))

	#define PDEPathGetDataEx (ACROASSERT(gPDFEditReadVersion >=PDFEditReadHFT_VERSION_10), *((PDEPathGetDataExSELPROTO)(gPDFEditReadHFT[PDEPathGetDataExSEL])))
	
	#define PDEExtGStateGetBPC (ACROASSERT(gPDFEditReadVersion >=PDFEditReadHFT_VERSION_11), *((PDEExtGStateGetBPCSELPROTO)(gPDFEditReadHFT[PDEExtGStateGetBPCSEL]))) 
	#define PDEExtGStateGetHalfToneOrigin (ACROASSERT(gPDFEditReadVersion >=PDFEditReadHFT_VERSION_11), *((PDEExtGStateGetHalfToneOriginSELPROTO)(gPDFEditReadHFT[PDEExtGStateGetHalfToneOriginSEL]))) 

/* If you add, delete, or modify procs listed in this file please read the instructions at the top about how to properly change the version number */

#endif /* !STATIC_HFT */

#endif /* PI_PDFEDIT_READ_VERSION != 0 */

#endif /* PLUGIN */

#ifdef __cplusplus
}
#endif

#endif /* _H_PERCalls */

