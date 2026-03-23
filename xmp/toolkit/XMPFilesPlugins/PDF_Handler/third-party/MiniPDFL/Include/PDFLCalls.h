/*
**      PDFLCalls.h
**
**      Toolkit-specific API calls
**
 *********************************************************************

 ADOBE SYSTEMS INCORPORATED
 Copyright (C) 2003-2006 Adobe Systems Incorporated
 All rights reserved.

 NOTICE: Adobe permits you to use, modify, and distribute this file
 in accordance with the terms of the Adobe license agreement
 accompanying it. If you have received this file from a source other
 than Adobe, then your use, modification, or distribution of it
 requires the prior written permission of Adobe.
 ******************************************************************************************
 *                      **** Instructions for Plugin Developers **** 
 * (In the instructions below ~ is used to refer to the HFT this file is for.  For
 *   example this file would be called "~Calls.h")
 *
 * To use this file you must declare two global variables g~HFT and g~Version.  You can
 * see them declared as extern about one page down from here.  Your plugin should set a 
 * #define of PI_~_VERSION to some non zero value.  Suggested values are given below in
 * the "for public use" section.  ~HFT_LATEST_VERSION is not recommended because you will 
 * not be able to support backwards compatible versions.  Better is to use the lowest 
 * ~HFT_VERSION you require.  Later versions are compatible with earlier versions so if 
 * you require ~HFT_VERSION_4 your plugin will work with ~HFT_VERSION_5, ~HFT_VERSION_6, etc.
 *
 * You can support old versions, yet still use newer versions of this HFT by checking the 
 * value of g~Version.  If you use the standard PiMain.c supplied in the SDK this will be
 * set to the actual version of the HFT returned to you (For example, you require version 4,
 * you are returned version 7 which is compatible; g~Version is set to 7).  You can write
 * code that looks something like this:
 *   if (g~Version >= ~HFT_VERSION_5) 
 *      CallNewSpeedyCode();
 *   else {
 *      assert(g~Version >= ~HFT_VERSION_4);  //PI_~_VERSION was defined as ~HFT_VERSION_4
 *      CallOldSlowCode();
 *   }
 ******************************************************************************************
 *                         **** Instructions for HFT Developer **** 
 *   (In the instructions below ~ is used to refer to the HFT this file is for.  For
 *     example this file would be called "~Calls.h")
 *
 *   Most importantly, routines that have been released can never be deleted or changed.
 *   If you want to make a new version create a new call, add to the end of this file and
 *   increment the _~_LATEST_VERSION (note the leading underscore).  
 *   
 *   If this is the first new routine in a new version, you should change the _~_IS_BETA flag
 *   to 1.  Next, create a new ~_VERSION_# for plugins to use and set it to 
 *   ~HFT_LATEST_VERSION.  For example the last release of Acrobat was version 20.  Version 21
 *   is under development.  You add a new routine for version 21.  Increment _~HFT_LATEST_VERSION 
 *   to 0x00200001 and set _~_IS_BETA to 1.  Add "#define ~HFT_VERSION_21 ~HFT_LATEST_VERSION".  
 *   Add your routine and assert that g~Version >= ~HFT_VERSION_21.  Leave 
 *   _~_LAST_BETA_COMPATIBLE_VERSION unchanged (0x00200000 in our example).
 *
 *   If the ~_IS_BETA flag is set to 1, you may change or delete the beta routines at will.
 *   Before checking in the modifications however, increment the _~HFT_LATEST_VERSION number.
 *   If the change is not compatible (delete, change, etc.) set _~_LAST_BETA_COMPATIBLE_VERSION equal
 *   to the new _~HFT_LATEST_VERSION.  If the change is compatible, leave the LAST_BETA version
 *   alone.
 *
 *   Plugins that require a BETA HFT will be refused unless they ask for a beta version
 *   >= LAST_BETA_COMPATIBLE_VERSION and <= HFT_LATEST_VERSION.
 *   By incrementing the version number you ensure the plugin will refuse to load until it
 *   has been recompiled with your changes.  You also ensure plugins compiled with your changes
 *   will not load on older versions of Acrobat.  In other words in makes sure both sides are in sync.  
 *
 *   Again, whenever you make a change to this file, you must increment _~HFT_LATEST_VERSION.
 *  
 *   Once the product reaches RC or similar milestone, change the _~_IS_BETA flag to 0, jump
 *   The _~HFT_LATEST_VERSION to the final version (0x00210000 in our example), do the same for
 *   _~_LAST_BETA_COMPATIBLE_VERSION, and set the 
 *   ~HFT_VERSION_# to the final version number (0x00210000 in our example).  Once the HFT
 *   has left beta, the routines cannot be changed and a new beta must ensue (beta for version
 *   22 in our example).
 *
 *******************************************************************************************/

#ifndef _H_PDFLCalls
#define _H_PDFLCalls

#include "acroassert.h"

#if ACROBAT_LIBRARY || PDFL_EXTENSION
#include "PDCalls.h"
#include "ASCalls.h"
#endif

/* for Adobe use only */
#define _PDFLHFT_LATEST_VERSION 0x000D0000
#define _PDFLHFT_LAST_BETA_COMPATIBLE_VERSION 0x000D0000
#define _PDFLHFT_IS_BETA 0

/* for public use */
#define PDFLHFT_LATEST_VERSION (_PDFLHFT_IS_BETA ? (kHFT_IN_BETA_FLAG | _PDFLHFT_LATEST_VERSION) : _PDFLHFT_LATEST_VERSION)

#define PDFLHFT_VERSION_13  PDFLHFT_LATEST_VERSION
#define PDFLHFT_VERSION_10  0x000A0000
#define PDFLHFT_VERSION_9   0x00090000
#define PDFLHFT_VERSION_8   0x00080000
#define PDFLHFT_VERSION_7_5 0x00070005
#define PDFLHFT_VERSION_7_1 0x00070001
#define PDFLHFT_VERSION_7   0x00070000
#define PDFLHFT_VERSION_6   0x00060000
#define PDFLHFT_VERSION_5   0x00050000

#ifdef __cplusplus
extern "C" {
#endif

#include "Environ.h"
#include "CoreExpT.h"
#include "PDFLExpT.h"

#ifdef NPROC /* This might be defined in sys/procs.h */
#undef NPROC
#endif /* NPROC */
	
#if !PLUGIN
	/* Static link */
	#define NPROC(returnType, name, params)		\
		ACEX1 returnType ACEX2 name params;
	#define NOPROC(name)
	#include "PDFLProcs.h"
	#undef NPROC
	#undef NOPROC
#else

	/* HFT version */
	#include "PIRequir.h"
	#include "PDFLRequir.h"
	
	/* Enumerate the selectors */
	#define NPROC(returnType, name, params)			\
		name##SEL2,
	
   #define NOPROC(name)	\
			name##SEL2,
	enum {
		PDFLBAD_SELECTOR,
		#include "PDFLProcs.h"
		PDFLNUMSELECTORSplusOne
	};
	
	#define PDFLNUMSELECTORS (PDFLNUMSELECTORSplusOne - 1)
	
	/* Create the prototypes */
	#undef NPROC
	#undef NOPROC
	#define NPROC(returnType, name, params)		\
		typedef ACCBPROTO1 returnType (ACCBPROTO2 *name##SEL2PROTO)params;
	#define NOPROC(name)
		#include "PDFLProcs.h"
	#undef NOPROC
	#undef NPROC

#if PDFL_VERSION != 0
#ifdef THREAD_SAFE_PDFL
	#define gPDFLHFT (GetHFTLocations()->pdflHFT)
	#define gPDFLVersion (GetHFTLocations()->pdflVersion)
#else
	extern HFT gPDFLHFT;
	extern ASUns32 gPDFLVersion;
#endif
/* PDFLibraryRegisterNotification */
#define PDFLibraryRegisterNotification (ACROASSERT(gPDFLVersion >=PDFLHFT_VERSION_5), *((PDFLibraryRegisterNotificationSEL2PROTO)(gPDFLHFT[PDFLibraryRegisterNotificationSEL2])))

/* PDFLibraryUnregisterNotification */
#define PDFLibraryUnregisterNotification (ACROASSERT(gPDFLVersion >=PDFLHFT_VERSION_5), *((PDFLibraryUnregisterNotificationSEL2PROTO)(gPDFLHFT[PDFLibraryUnregisterNotificationSEL2])))

/* ASPurgeMemory */
#define ASPurgeMemory (ACROASSERT(gPDFLVersion >=PDFLHFT_VERSION_6), *((ASPurgeMemorySEL2PROTO)(gPDFLHFT[ASPurgeMemorySEL2])))

/* ASAtomGetCount */
#define ASAtomGetCount (ACROASSERT(gPDFLVersion >=PDFLHFT_VERSION_6), *((ASAtomGetCountSEL2PROTO)(gPDFLHFT[ASAtomGetCountSEL2])))

/* ASFileSysSetDefaultTempPath */
#define ASFileSysSetDefaultTempPath (ACROASSERT(gPDFLVersion >=PDFLHFT_VERSION_6), *((ASFileSysSetDefaultTempPathSEL2PROTO)(gPDFLHFT[ASFileSysSetDefaultTempPathSEL2])))

/* ASFileSysGetDefaultTempPath */
#define ASFileSysGetDefaultTempPath (ACROASSERT(gPDFLVersion >=PDFLHFT_VERSION_6), *((ASFileSysGetDefaultTempPathSEL2PROTO)(gPDFLHFT[ASFileSysGetDefaultTempPathSEL2])))

#if !defined(IOS_ENV)
/* PDDocPrintPages */
#define PDDocPrintPages  (ACROASSERT(gPDFLVersion >=PDFLHFT_VERSION_6), *((PDDocPrintPagesSEL2PROTO)(gPDFLHFT[PDDocPrintPagesSEL2])))
    
/* PDPageEmitPSOrient */
#define PDPageEmitPSOrient  (ACROASSERT(gPDFLVersion >=PDFLHFT_VERSION_6), *((PDPageEmitPSOrientSEL2PROTO)(gPDFLHFT[PDPageEmitPSOrientSEL2])))

#endif
#if !MAC_PLATFORM || !AS_ARCH_64BIT 	
/* PDPageDrawContentsPlacedToWindow */
#define PDPageDrawContentsPlacedToWindow (ACROASSERT(gPDFLVersion >=PDFLHFT_VERSION_6), *((PDPageDrawContentsPlacedToWindowSEL2PROTO)(gPDFLHFT[PDPageDrawContentsPlacedToWindowSEL2])))
#endif
	
/* PDFontStreamPS */
#define PDFontStreamPS (ACROASSERT(gPDFLVersion >=PDFLHFT_VERSION_6), *((PDFontStreamPSSEL2PROTO)(gPDFLHFT[PDFontStreamPSSEL2])))

/* PDFontDownloadContextCreate */
#define PDFontDownloadContextCreate (ACROASSERT(gPDFLVersion >=PDFLHFT_VERSION_6), *((PDFontDownloadContextCreateSEL2PROTO)(gPDFLHFT[PDFontDownloadContextCreateSEL2])))

/* PDFontDownloadContextDestroy */
#define PDFontDownloadContextDestroy (ACROASSERT(gPDFLVersion >=PDFLHFT_VERSION_6), *((PDFontDownloadContextDestroySEL2PROTO)(gPDFLHFT[PDFontDownloadContextDestroySEL2])))

/* PDPageGetSize */
#define PDPageGetSize (ACROASSERT(gPDFLVersion >=PDFLHFT_VERSION_6), *((PDPageGetSizeSEL2PROTO)(gPDFLHFT[PDPageGetSizeSEL2])))

/* PDFontWasExtracted */
#define PDFontWasExtracted (ACROASSERT(gPDFLVersion >=PDFLHFT_VERSION_6), *((PDFontWasExtractedSEL2PROTO)(gPDFLHFT[PDFontWasExtractedSEL2])))

/* PDFontWasFauxed */
#define PDFontWasFauxed (ACROASSERT(gPDFLVersion >=PDFLHFT_VERSION_6), *((PDFontWasFauxedSEL2PROTO)(gPDFLHFT[PDFontWasFauxedSEL2])))

/* PDPrefSetGreekLevel */
#define PDPrefSetGreekLevel (ACROASSERT(gPDFLVersion >=PDFLHFT_VERSION_6), *((PDPrefSetGreekLevelSEL2PROTO)(gPDFLHFT[PDPrefSetGreekLevelSEL2])))

/* PDPrefSetAntialiasLevel */
#define PDPrefSetAntialiasLevel (ACROASSERT(gPDFLVersion >=PDFLHFT_VERSION_6), *((PDPrefSetAntialiasLevelSEL2PROTO)(gPDFLHFT[PDPrefSetAntialiasLevelSEL2])))

/* PDPrefGetGreekLevel */
#define PDPrefGetGreekLevel (ACROASSERT(gPDFLVersion >=PDFLHFT_VERSION_6), *((PDPrefGetGreekLevelSEL2PROTO)(gPDFLHFT[PDPrefGetGreekLevelSEL2])))

/* PDPrefGetAntialiasLevel */
#define PDPrefGetAntialiasLevel (ACROASSERT(gPDFLVersion >=PDFLHFT_VERSION_6), *((PDPrefGetAntialiasLevelSEL2PROTO)(gPDFLHFT[PDPrefGetAntialiasLevelSEL2])))

#if ACRO_SDK_LEVEL < 0x00080000 /* Moved to PDModel */
/* PDPrefSetBlackPointCompensation */
#define PDPrefSetBlackPointCompensation (ACROASSERT(gPDFLVersion >=PDFLHFT_VERSION_6), *((PDPrefSetBlackPointCompensationSEL2PROTO)(gPDFLHFT[PDPrefSetBlackPointCompensationSEL2])))

/* PDPrefGetBlackPointCompensation */
#define PDPrefGetBlackPointCompensation (ACROASSERT(gPDFLVersion >=PDFLHFT_VERSION_6), *((PDPrefGetBlackPointCompensationSEL2PROTO)(gPDFLHFT[PDPrefGetBlackPointCompensationSEL2])))
#endif /* ACRO_SDK_LEVEL < 0x00080000 */

/* PDPrefSetUseLocalFonts */
#define PDPrefSetUseLocalFonts (ACROASSERT(gPDFLVersion >=PDFLHFT_VERSION_6), *((PDPrefSetUseLocalFontsSEL2PROTO)(gPDFLHFT[PDPrefSetUseLocalFontsSEL2])))

#if !defined(IOS_ENV)
/* PDFontPSEmitGlyphsIncr */
#define PDFontPSEmitGlyphsIncr (ACROASSERT(gPDFLVersion >=PDFLHFT_VERSION_6), *((PDFontPSEmitGlyphsIncrSEL2PROTO)(gPDFLHFT[PDFontPSEmitGlyphsIncrSEL2])))

/* PDFontPSGetComponentFontList */
#define PDFontPSGetComponentFontList (ACROASSERT(gPDFLVersion >=PDFLHFT_VERSION_6), *((PDFontPSGetComponentFontListSEL2PROTO)(gPDFLHFT[PDFontPSGetComponentFontListSEL2])))

/* PDFontPSFlushIncrGlyphList */
#define PDFontPSFlushIncrGlyphList (ACROASSERT(gPDFLVersion >=PDFLHFT_VERSION_6), *((PDFontPSFlushIncrGlyphListSEL2PROTO)(gPDFLHFT[PDFontPSFlushIncrGlyphListSEL2])))
#endif
    
/* PDPageDrawContentsToMemory */
#define PDPageDrawContentsToMemory (ACROASSERT(gPDFLVersion >=PDFLHFT_VERSION_6), *((PDPageDrawContentsToMemorySEL2PROTO)(gPDFLHFT[PDPageDrawContentsToMemorySEL2])))

#if !MAC_PLATFORM || !AS_ARCH_64BIT	
/* PDPageDrawContentsPlacedWithParams */
#define PDPageDrawContentsPlacedWithParams  (ACROASSERT(gPDFLVersion >=PDFLHFT_VERSION_6), *((PDPageDrawContentsPlacedWithParamsSEL2PROTO)(gPDFLHFT[PDPageDrawContentsPlacedWithParamsSEL2])))
#endif
	
#if ACRO_SDK_LEVEL < 0x00080000 /* Moved to PDModel */
/* PDPrefSetUseOutputIntents */
#define PDPrefSetUseOutputIntents  (ACROASSERT(gPDFLVersion >=PDFLHFT_VERSION_6), *((PDPrefSetUseOutputIntentsSEL2PROTO)(gPDFLHFT[PDPrefSetUseOutputIntentsSEL2])))

/* PDPrefGetUseOutputIntents */
#define PDPrefGetUseOutputIntents  (ACROASSERT(gPDFLVersion >=PDFLHFT_VERSION_6), *((PDPrefGetUseOutputIntentsSEL2PROTO)(gPDFLHFT[PDPrefGetUseOutputIntentsSEL2])))
#endif /* ACRO_SDK_LEVEL < 0x00080000 */
    
/* PDPrefSetSuppressICCSpaces */
#define PDPrefSetSuppressICCSpaces  (ACROASSERT(gPDFLVersion >=PDFLHFT_VERSION_6), *((PDPrefSetSuppressICCSpacesSEL2PROTO)(gPDFLHFT[PDPrefSetSuppressICCSpacesSEL2])))
    
/* PDPrefGetSuppressICCSpaces */
#define PDPrefGetSuppressICCSpaces  (ACROASSERT(gPDFLVersion >=PDFLHFT_VERSION_6), *((PDPrefGetSuppressICCSpacesSEL2PROTO)(gPDFLHFT[PDPrefGetSuppressICCSpacesSEL2])))

/* ASCallbackCreateNotification
** Type-checking notification callback creation.  Will cause a compiler
** error if the proc's signature does not match the signature of the given
** notification (if DEBUG is 1).
*/
#define ASCallbackCreateNotification(nsel, proc)	ASCallbackCreateProto(nsel##NPROTO, proc)

/* AVExtensionMgrRegisterNotification */
#define AVExtensionMgrRegisterNotification (ACROASSERT(gPDFLVersion >=PDFLHFT_VERSION_6), *((AVExtensionMgrRegisterNotificationSEL2PROTO)(gPDFLHFT[AVExtensionMgrRegisterNotificationSEL2])))

/* AVExtensionMgrUnregisterNotification */
#define AVExtensionMgrUnregisterNotification (ACROASSERT(gPDFLVersion >=PDFLHFT_VERSION_6), *((AVExtensionMgrUnregisterNotificationSEL2PROTO)(gPDFLHFT[AVExtensionMgrUnregisterNotificationSEL2])))

#if ACRO_SDK_LEVEL < 0x00080000 /* Moved to PDModel */
/* PDPrefSetWorkingRGB */
#define PDPrefSetWorkingRGB (ACROASSERT(gPDFLVersion >=PDFLHFT_VERSION_6), *((PDPrefSetWorkingRGBSEL2PROTO)(gPDFLHFT[PDPrefSetWorkingRGBSEL2])))

/* PDPrefSetWorkingCMYK */
#define PDPrefSetWorkingCMYK (ACROASSERT(gPDFLVersion >=PDFLHFT_VERSION_6), *((PDPrefSetWorkingCMYKSEL2PROTO)(gPDFLHFT[PDPrefSetWorkingCMYKSEL2])))

/* PDPrefSetWorkingGray */
#define PDPrefSetWorkingGray (ACROASSERT(gPDFLVersion >=PDFLHFT_VERSION_6), *((PDPrefSetWorkingGraySEL2PROTO)(gPDFLHFT[PDPrefSetWorkingGraySEL2])))
#endif /* ACRO_SDK_LEVEL < 0x00080000 */

/* PDFLGetVersion */
#define PDFLGetVersion	(ACROASSERT(gPDFLVersion >=PDFLHFT_VERSION_6), *((PDFLGetVersionSEL2PROTO)(gPDFLHFT[PDFLGetVersionSEL2])))

/* PDFLTerm */
#define PDFLTerm  (ACROASSERT(gPDFLVersion >=PDFLHFT_VERSION_6), *((PDFLTermSEL2PROTO)(gPDFLHFT[PDFLTermSEL2])))

/* PDFLGetInitCount */
#define PDFLGetInitCount  (ACROASSERT(gPDFLVersion >=PDFLHFT_VERSION_6), *((PDFLGetInitCountSEL2PROTO)(gPDFLHFT[PDFLGetInitCountSEL2])))

/* PDFLGetFlags */
#define PDFLGetFlags  (ACROASSERT(gPDFLVersion >=PDFLHFT_VERSION_6), *((PDFLGetFlagsSEL2PROTO)(gPDFLHFT[PDFLGetFlagsSEL2])))

#if !defined(IOS_ENV)
/* PDFLPrintDoc */
#define PDFLPrintDoc  (ACROASSERT(gPDFLVersion >=PDFLHFT_VERSION_6), *((PDFLPrintDocSEL2PROTO)(gPDFLHFT[PDFLPrintDocSEL2])))
    
/* PDFLPrintPDF */
#define PDFLPrintPDF  (ACROASSERT(gPDFLVersion >=PDFLHFT_VERSION_6), *((PDFLPrintPDFSEL2PROTO)(gPDFLHFT[PDFLPrintPDFSEL2])))
#endif
    
/* PDFLibraryRegisterNotificationEX */
#define PDFLibraryRegisterNotificationEx (ACROASSERT(gPDFLVersion >=PDFLHFT_VERSION_6), *((PDFLibraryRegisterNotificationExSEL2PROTO)(gPDFLHFT[PDFLibraryRegisterNotificationExSEL2])))

#ifndef ANDROID_ENV
/* CosSetExternalFilePermissionProc */
#define CosSetExternalFilePermissionProc (ACROASSERT(gPDFLVersion >=PDFLHFT_VERSION_6), *((CosSetExternalFilePermissionProcSEL2PROTO)(gPDFLHFT[CosSetExternalFilePermissionProcSEL2])))
#endif

#if PDFLHFT_VERSION_7_1 >= 0x00070001

#ifndef ANDROID_ENV
/* PDFLibraryRegisterRNG */
#define PDFLibraryRegisterRNG (ACROASSERT(gPDFLVersion >=PDFLHFT_VERSION_7_1), *((PDFLibraryRegisterRNGSEL2PROTO)(gPDFLHFT[PDFLibraryRegisterRNGSEL2])))
#endif

/* PDOCRegisterFindOutZoomProc */
#define PDOCRegisterFindOutZoomProc (ACROASSERT(gPDFLVersion >=PDFLHFT_VERSION_7_1), *((PDOCRegisterFindOutZoomProcSEL2PROTO)(gPDFLHFT[PDOCRegisterFindOutZoomProcSEL2])))
#if 0
/*PDOCRegisterFindOutZoomProc*/
#define PDOCRegisterFindOutZoomProc (ACROASSERT(gPDFLVersion >=PDFLHFT_VERSION_7_1), *((PDOCRegisterFindOutZoomProcSEL2PROTO)(gPDFLHFT[PDOCRegisterFindOutZoomProcSEL2])))
#endif
/*PDOCRegisterFindOutLanguageProc*/
#define PDOCRegisterFindOutLanguageProc (ACROASSERT(gPDFLVersion >=PDFLHFT_VERSION_7_1), *((PDOCRegisterFindOutLanguageProcSEL2PROTO)(gPDFLHFT[PDOCRegisterFindOutLanguageProcSEL2])))

/*PDOCRegisterFindOutUserProc*/
#define PDOCRegisterFindOutUserProc (ACROASSERT(gPDFLVersion >=PDFLHFT_VERSION_7_1), *((PDOCRegisterFindOutUserProcSEL2PROTO)(gPDFLHFT[PDOCRegisterFindOutUserProcSEL2])))

/*PDOCRegisterFindOutAutoStatePrefProc*/
#define PDOCRegisterFindOutAutoStatePrefProc (ACROASSERT(gPDFLVersion >=PDFLHFT_VERSION_7_1), *((PDOCRegisterFindOutAutoStatePrefProcSEL2PROTO)(gPDFLHFT[PDOCRegisterFindOutAutoStatePrefProcSEL2])))

#if PDFLHFT_LATEST_VERSION >= 0x00080000

#if PDFLHFT_LATEST_VERSION >= 0x00090000
/* PDPrefSetRefXObj */
#define PDPrefSetRefXObj (ACROASSERT(gPDFLVersion >= PDFLHFT_VERSION_9), *((PDPrefSetRefXObjSEL2PROTO)(gPDFLHFT[PDPrefSetRefXObjSEL2])))

/* PDPrefGetRefXObj */
#define PDPrefGetRefXObj (ACROASSERT(gPDFLVersion >= PDFLHFT_VERSION_9), *((PDPrefGetRefXObjSEL2PROTO)(gPDFLHFT[PDPrefGetRefXObjSEL2])))

/* PDPrefSetEnableThinLineHeuristics */
#define PDPrefSetEnableThinLineHeuristics (ACROASSERT(gPDFLVersion >= PDFLHFT_VERSION_9), *((PDPrefSetEnableThinLineHeuristicsSEL2PROTO)(gPDFLHFT[PDPrefSetEnableThinLineHeuristicsSEL2])))

/* PDPrefGetEnableThinLineHeuristics */
#define PDPrefGetEnableThinLineHeuristics (ACROASSERT(gPDFLVersion >= PDFLHFT_VERSION_9), *((PDPrefGetEnableThinLineHeuristicsSEL2PROTO)(gPDFLHFT[PDPrefGetEnableThinLineHeuristicsSEL2])))

#if PDFLHFT_LATEST_VERSION >= 0x000A0000
/* PDPageDrawContentsToMemoryEx */
#define PDPageDrawContentsToMemoryEx (ACROASSERT(gPDFLVersion >= PDFLHFT_VERSION_10), *((PDPageDrawContentsToMemoryExSEL2PROTO)(gPDFLHFT[PDPageDrawContentsToMemoryExSEL2])))

#if PDFLHFT_LATEST_VERSION >= 0x000D0000
/* PDPrefSetTranslationRasterizationDPI */
#define PDPrefSetTranslationRasterizationDPI (ACROASSERT(gPDFLVersion >= PDFLHFT_VERSION_13), *((PDPrefSetTranslationRasterizationDPISEL2PROTO)(gPDFLHFT[PDPrefSetTranslationRasterizationDPISEL2])))
    
/* PDPrefGetTranslationRasterizationDPI */
#define PDPrefGetTranslationRasterizationDPI (ACROASSERT(gPDFLVersion >= PDFLHFT_VERSION_13), *((PDPrefGetTranslationRasterizationDPISEL2PROTO)(gPDFLHFT[PDPrefGetTranslationRasterizationDPISEL2])))
    
#endif /* PDFL_VERSION >= 0x000D0000 */
#endif /* PDFL_VERSION >= 0x000A0000 */
#endif /* PDFL_VERSION >= 0x00090000 */
#endif /* PDFL_VERSION >= 0x00080000 */
#endif /* PDFL_VERSION >= 0x00070001 */

#endif /* PDFL_VERSION != 0 */	

#endif /* PLUGIN */


#ifdef __cplusplus
}
#endif

#endif /* _H_PDFLCalls */
