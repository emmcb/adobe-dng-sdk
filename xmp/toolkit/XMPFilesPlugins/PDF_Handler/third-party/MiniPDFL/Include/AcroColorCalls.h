/*
** AcroColorCalls.h
**
**
**
********************************************************************

 ADOBE SYSTEMS INCORPORATED
 Copyright (C) 2002-2006 Adobe Systems Incorporated
 All rights reserved.

 NOTICE: Adobe permits you to use, modify, and distribute this file
 in accordance with the terms of the Adobe license agreement
 accompanying it. If you have received this file from a source other
 than Adobe, then your use, modification, or distribution of it
 requires the prior written permission of Adobe.

 **************************************************************************************** 
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

#ifndef _H_AcroColorCalls
#define _H_AcroColorCalls


# include "CoreExpT.h"

/* for Adobe use only */
#define _AcroColorHFT_LATEST_VERSION 0x000C0000
#define _AcroColorHFT_LAST_BETA_COMPATIBLE_VERSION 0x000A0000
#define _AcroColorHFT_IS_BETA 0

/* for public use */
#define AcroColorHFT_LATEST_VERSION (_AcroColorHFT_IS_BETA ? (kHFT_IN_BETA_FLAG | _AcroColorHFT_LATEST_VERSION) : _AcroColorHFT_LATEST_VERSION)

/* for public use */
#define AcroColorHFT_VERSION_6   0x00060000
#define AcroColorHFT_VERSION_7   0x00070000
#define AcroColorHFT_VERSION_8   0x00080000
#define AcroColorHFT_VERSION_9   0x00090000
#define AcroColorHFT_VERSION_10  0x000A0000
#define AcroColorHFT_VERSION_11  0x000B0000
#define AcroColorHFT_VERSION_12  AcroColorHFT_LATEST_VERSION

#ifdef __cplusplus
extern "C" {
#endif

#ifdef NPROC /* This might be defined in sys/procs.h */
#undef NPROC
#endif
	
#define EXTERNAL_ACROCOLORPROCS_USER 1 /* External user of AcroColorProcs.h header file */
#include "AcroColorExpT.h"

#if !PLUGIN
	/* Static link */
	#define NPROC(returnType, name, params)	\
		extern ACEX1 returnType ACEX2 name params;
	#define SPROC(returnType, name, params, stubProc) \
  		extern ACEX1 returnType ACEX2 name params;
   #define XNPROC NPROC
   #define PROC	NPROC
   #define XPROC NPROC
	#define ENPROC	NPROC		
	#define NOPROC(name)
	#include "AcroColorProcs.h"
	#undef NPROC
   #undef XNPROC
   #undef SPROC
   #undef PROC
   #undef NOPROC	
   #undef XPROC
	#undef ENPROC	

#else

	/* HFT version */
   #include "PIRequir.h"
#ifdef THREAD_SAFE_PDFL
   #include "PDFLInitCommon.h"
#endif /* defined THREAD_SAFE_PDFL */
	
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
	enum {
		AcroColorBAD_SELECTOR,
		#include "AcroColorProcs.h"
		AcroColorNUMSELECTORSplusOne
	};
	
	#define AcroColorNUMSELECTORS (AcroColorNUMSELECTORSplusOne - 1)
	
	/* Create the prototypes */

	#undef NPROC
   #undef XNPROC
   #undef SPROC
   #undef PROC
   #undef XPROC
   #undef NOPROC
	#undef ENPROC
	
	#define NPROC(returnType, name, params)		\
		typedef ACCBPROTO1 returnType (ACCBPROTO2 *name##SELPROTO)params;
	#define SPROC(returnType, name, params, stubProc) \
		typedef ACCBPROTO1 returnType (ACCBPROTO2 *name##SELPROTO)params;
	
   #define PROC	NPROC
   #define ENPROC NPROC
	#define NOPROC(name)
#if READER_PLUGIN
   /* Force an error for Exchange procs */
   #define XNPROC(returnType, name, params)
   #define XPROC(returnType, name, params)
#else
   #define XNPROC NPROC
   #define XPROC NPROC
#endif
	#include "AcroColorProcs.h"
	#undef NPROC
   #undef XNPROC
   #undef SPROC
   #undef PROC
   #undef NOPROC	
   #undef XPROC		
	#undef ENPROC
	#include "PIRequir.h"

#if PI_ACROCOLOR_VERSION != 0

#include "AcroColorExpT.h"
#include "ASExpT.h" // For ASText
//#include "CorCalls.h"		/* For ASCallbackCreateProto */

#ifdef THREAD_SAFE_PDFL
	#define gAcroColorHFT (GetHFTLocations()->acroColorHFT)
	#define gAcroColorVersion (GetHFTLocations()->acroColorVersion)
#else /* defined THREAD_SAFE_PDFL */
	extern HFT gAcroColorHFT;
	extern ASUns32 gAcroColorVersion;
#endif /* defined THREAD_SAFE_PDFL */

#define ACEngineCount (ACROASSERT(gAcroColorVersion >=AcroColorHFT_VERSION_6), *((ACEngineCountSELPROTO) (gAcroColorHFT[ACEngineCountSEL])))
#define ACEngineInfo (ACROASSERT(gAcroColorVersion >=AcroColorHFT_VERSION_6), *((ACEngineInfoSELPROTO) (gAcroColorHFT[ACEngineInfoSEL])))
#define ACSetEngine (ACROASSERT(gAcroColorVersion >=AcroColorHFT_VERSION_6), *((ACSetEngineSELPROTO) (gAcroColorHFT[ACSetEngineSEL])))

#define ACMakeProfileList (ACROASSERT(gAcroColorVersion >=AcroColorHFT_VERSION_6), *((ACMakeProfileListSELPROTO) (gAcroColorHFT[ACMakeProfileListSEL])))
#define ACProfileListCount (ACROASSERT(gAcroColorVersion >=AcroColorHFT_VERSION_6), *((ACProfileListCountSELPROTO) (gAcroColorHFT[ACProfileListCountSEL])))
#define ACProfileListItemDescription (ACROASSERT(gAcroColorVersion >=AcroColorHFT_VERSION_6), *((ACProfileListItemDescriptionSELPROTO) (gAcroColorHFT[ACProfileListItemDescriptionSEL])))
#define ACProfileListItemCode (ACROASSERT(gAcroColorVersion >=AcroColorHFT_VERSION_6), *((ACProfileListItemCodeSELPROTO) (gAcroColorHFT[ACProfileListItemCodeSEL])))
#define ACUnReferenceProfileList (ACROASSERT(gAcroColorVersion >=AcroColorHFT_VERSION_6), *((ACUnReferenceProfileListSELPROTO) (gAcroColorHFT[ACUnReferenceProfileListSEL])))

#define ACMakePresetList (ACROASSERT(gAcroColorVersion >=AcroColorHFT_VERSION_6), *((ACMakePresetListSELPROTO) (gAcroColorHFT[ACMakePresetListSEL])))
#define ACPresetListCount (ACROASSERT(gAcroColorVersion >=AcroColorHFT_VERSION_6), *((ACPresetListCountSELPROTO) (gAcroColorHFT[ACPresetListCountSEL])))
#if !MAC_PLATFORM || !AS_ARCH_64BIT	
#define ACPresetListItemFile (ACROASSERT(gAcroColorVersion >=AcroColorHFT_VERSION_6), *((ACPresetListItemFileSELPROTO) (gAcroColorHFT[ACPresetListItemFileSEL])))
#define ACPresetFileToName (ACROASSERT(gAcroColorVersion >=AcroColorHFT_VERSION_6), *((ACPresetFileToNameSELPROTO) (gAcroColorHFT[ACPresetFileToNameSEL])))
#endif
#define ACUnReferencePresetList (ACROASSERT(gAcroColorVersion >=AcroColorHFT_VERSION_6), *((ACUnReferencePresetListSELPROTO) (gAcroColorHFT[ACUnReferencePresetListSEL])))


#define ACGetSettingsProfile (ACROASSERT(gAcroColorVersion >=AcroColorHFT_VERSION_6), *((ACGetSettingsProfileSELPROTO) (gAcroColorHFT[ACGetSettingsProfileSEL])))
#define ACMakeSettings (ACROASSERT(gAcroColorVersion >=AcroColorHFT_VERSION_6), *((ACMakeSettingsSELPROTO) (gAcroColorHFT[ACMakeSettingsSEL])))
#if !MAC_PLATFORM || !AS_ARCH_64BIT	
#define ACLoadSettings (ACROASSERT(gAcroColorVersion >=AcroColorHFT_VERSION_6), *((ACLoadSettingsSELPROTO) (gAcroColorHFT[ACLoadSettingsSEL])))
#endif	
#define ACGetSettingsString (ACROASSERT(gAcroColorVersion >=AcroColorHFT_VERSION_6), *((ACGetSettingsStringSELPROTO) (gAcroColorHFT[ACGetSettingsStringSEL])))
#define ACGetSettingsUnsigned32 (ACROASSERT(gAcroColorVersion >=AcroColorHFT_VERSION_6), *((ACGetSettingsUnsigned32SELPROTO) (gAcroColorHFT[ACGetSettingsUnsigned32SEL])))
#define ACUnReferenceSettings (ACROASSERT(gAcroColorVersion >=AcroColorHFT_VERSION_6), *((ACUnReferenceSettingsSELPROTO) (gAcroColorHFT[ACUnReferenceSettingsSEL])))

#define ACProfileDescription (ACROASSERT(gAcroColorVersion >=AcroColorHFT_VERSION_6), *((ACProfileDescriptionSELPROTO) (gAcroColorHFT[ACProfileDescriptionSEL])))
#define ACProfileFromDescription (ACROASSERT(gAcroColorVersion >=AcroColorHFT_VERSION_6), *((ACProfileFromDescriptionSELPROTO) (gAcroColorHFT[ACProfileFromDescriptionSEL])))
#define ACProfileFromCode (ACROASSERT(gAcroColorVersion >=AcroColorHFT_VERSION_6), *((ACProfileFromCodeSELPROTO) (gAcroColorHFT[ACProfileFromCodeSEL])))
#if !MAC_PLATFORM || !AS_ARCH_64BIT
#define ACMonitorProfile (ACROASSERT(gAcroColorVersion >=AcroColorHFT_VERSION_6), *((ACMonitorProfileSELPROTO) (gAcroColorHFT[ACMonitorProfileSEL])))
#endif
#define ACMakeBufferProfile (ACROASSERT(gAcroColorVersion >=AcroColorHFT_VERSION_6), *((ACMakeBufferProfileSELPROTO) (gAcroColorHFT[ACMakeBufferProfileSEL])))

#define ACMakeCalRGB (ACROASSERT(gAcroColorVersion >=AcroColorHFT_VERSION_6), *((ACMakeCalRGBSELPROTO) (gAcroColorHFT[ACMakeCalRGBSEL])))
#define ACMakeCalGray (ACROASSERT(gAcroColorVersion >=AcroColorHFT_VERSION_6), *((ACMakeCalGraySELPROTO) (gAcroColorHFT[ACMakeCalGraySEL])))
#define ACMakeCalLab (ACROASSERT(gAcroColorVersion >=AcroColorHFT_VERSION_6), *((ACMakeCalLabSELPROTO) (gAcroColorHFT[ACMakeCalLabSEL])))
#define ACProfileColorSpace (ACROASSERT(gAcroColorVersion >=AcroColorHFT_VERSION_6), *((ACProfileColorSpaceSELPROTO) (gAcroColorHFT[ACProfileColorSpaceSEL])))
#define ACProfileSize (ACROASSERT(gAcroColorVersion >=AcroColorHFT_VERSION_6), *((ACProfileSizeSELPROTO) (gAcroColorHFT[ACProfileSizeSEL])))
#define ACProfileData (ACROASSERT(gAcroColorVersion >=AcroColorHFT_VERSION_6), *((ACProfileDataSELPROTO) (gAcroColorHFT[ACProfileDataSEL])))
#define ACUnReferenceProfile (ACROASSERT(gAcroColorVersion >=AcroColorHFT_VERSION_6), *((ACUnReferenceProfileSELPROTO) (gAcroColorHFT[ACUnReferenceProfileSEL])))

#define ACMakeColorTransform (ACROASSERT(gAcroColorVersion >=AcroColorHFT_VERSION_6), *((ACMakeColorTransformSELPROTO) (gAcroColorHFT[ACMakeColorTransformSEL])))
#define ACApplyTransform (ACROASSERT(gAcroColorVersion >=AcroColorHFT_VERSION_6), *((ACApplyTransformSELPROTO) (gAcroColorHFT[ACApplyTransformSEL])))
#define ACUnReferenceTransform (ACROASSERT(gAcroColorVersion >=AcroColorHFT_VERSION_6), *((ACUnReferenceTransformSELPROTO) (gAcroColorHFT[ACUnReferenceTransformSEL])))

#define ACMakeString (ACROASSERT(gAcroColorVersion >=AcroColorHFT_VERSION_6), *((ACMakeStringSELPROTO) (gAcroColorHFT[ACMakeStringSEL])))
#define ACStringASCII (ACROASSERT(gAcroColorVersion >=AcroColorHFT_VERSION_6), *((ACStringASCIISELPROTO) (gAcroColorHFT[ACStringASCIISEL])))
#define ACStringLocalized (ACROASSERT(gAcroColorVersion >=AcroColorHFT_VERSION_6), *((ACStringLocalizedSELPROTO) (gAcroColorHFT[ACStringLocalizedSEL])))
#define ACStringUnicode (ACROASSERT(gAcroColorVersion >=AcroColorHFT_VERSION_6), *((ACStringUnicodeSELPROTO) (gAcroColorHFT[ACStringUnicodeSEL])))
#define ACUnReferenceString (ACROASSERT(gAcroColorVersion >=AcroColorHFT_VERSION_6), *((ACUnReferenceStringSELPROTO) (gAcroColorHFT[ACUnReferenceStringSEL])))

#define ACGetWorkingSpaceProfile (ACROASSERT(gAcroColorVersion >=AcroColorHFT_VERSION_6), *((ACGetWorkingSpaceProfileSELPROTO) (gAcroColorHFT[ACGetWorkingSpaceProfileSEL])))
    
#define ACProfilesMatch (ACROASSERT(gAcroColorVersion >=AcroColorHFT_VERSION_6), *((ACProfilesMatchSELPROTO) (gAcroColorHFT[ACProfilesMatchSEL])))

/* PDDocColorConvertPage */
#define PDDocColorConvertPage (ACROASSERT(gAcroColorVersion >=AcroColorHFT_VERSION_8), *((PDDocColorConvertPageSELPROTO)(gAcroColorHFT[PDDocColorConvertPageSEL])))

/* PDDocColorConvertEmbedeOuptutIntent */
#define PDDocColorConvertEmbedOutputIntent (ACROASSERT(gAcroColorVersion >=AcroColorHFT_VERSION_8), *((PDDocColorConvertEmbedOutputIntentSELPROTO)(gAcroColorHFT[PDDocColorConvertEmbedOutputIntentSEL])))


/* PDColorConvertPDEElement*/
#define PDColorConvertPDEElement (ACROASSERT(gAcroColorVersion >=AcroColorHFT_VERSION_8), *((PDColorConvertPDEElementSELPROTO)(gAcroColorHFT[PDColorConvertPDEElementSEL])))

/* Swatchbook API */
#define ACSwatchBooksFind (ACROASSERT(gAcroColorVersion >=AcroColorHFT_VERSION_9), *((ACSwatchBooksFindSELPROTO)(gAcroColorHFT[ACSwatchBooksFindSEL])))
#define ACSwatchBookCount (ACROASSERT(gAcroColorVersion >=AcroColorHFT_VERSION_9), *((ACSwatchBookCountSELPROTO)(gAcroColorHFT[ACSwatchBookCountSEL])))
#define ACSwatchBookTitle (ACROASSERT(gAcroColorVersion >=AcroColorHFT_VERSION_9), *((ACSwatchBookTitleSELPROTO)(gAcroColorHFT[ACSwatchBookTitleSEL])))
#define ACSwatchBookDescription (ACROASSERT(gAcroColorVersion >=AcroColorHFT_VERSION_9), *((ACSwatchBookDescriptionSELPROTO)(gAcroColorHFT[ACSwatchBookDescriptionSEL])))
#define ACSwatchBookDBDestroy (ACROASSERT(gAcroColorVersion >=AcroColorHFT_VERSION_9), *((ACSwatchBookDBDestroySELPROTO)(gAcroColorHFT[ACSwatchBookDBDestroySEL])))
#define ACSwatchBookLoad (ACROASSERT(gAcroColorVersion >=AcroColorHFT_VERSION_9), *((ACSwatchBookLoadSELPROTO)(gAcroColorHFT[ACSwatchBookLoadSEL])))
#define ACSwatchBookLoadFromPath (ACROASSERT(gAcroColorVersion >=AcroColorHFT_VERSION_9), *((ACSwatchBookLoadFromPathSELPROTO)(gAcroColorHFT[ACSwatchBookLoadFromPathSEL])))
#define ACSwatchBookDestroy (ACROASSERT(gAcroColorVersion >=AcroColorHFT_VERSION_9), *((ACSwatchBookDestroySELPROTO)(gAcroColorHFT[ACSwatchBookDestroySEL])))
#define ACSwatchBookNumberOfColors (ACROASSERT(gAcroColorVersion >=AcroColorHFT_VERSION_9), *((ACSwatchBookNumberOfColorsSELPROTO)(gAcroColorHFT[ACSwatchBookNumberOfColorsSEL])))
#define ACSwatchBookColorSpace (ACROASSERT(gAcroColorVersion >=AcroColorHFT_VERSION_9), *((ACSwatchBookColorSpaceSELPROTO)(gAcroColorHFT[ACSwatchBookColorSpaceSEL])))
#define ACSwatchBookIsProcess (ACROASSERT(gAcroColorVersion >=AcroColorHFT_VERSION_9), *((ACSwatchBookIsProcessSELPROTO)(gAcroColorHFT[ACSwatchBookIsProcessSEL])))
#define ACSwatchBookGetSwatchName (ACROASSERT(gAcroColorVersion >=AcroColorHFT_VERSION_9), *((ACSwatchBookGetSwatchNameSELPROTO)(gAcroColorHFT[ACSwatchBookGetSwatchNameSEL])))
#define ACSwatchBookGetSwatchValues (ACROASSERT(gAcroColorVersion >=AcroColorHFT_VERSION_9), *((ACSwatchBookGetSwatchValuesSELPROTO)(gAcroColorHFT[ACSwatchBookGetSwatchValuesSEL])))
    
/* PDDocColorConvertPageEx */
#define PDDocColorConvertPageEx (ACROASSERT(gAcroColorVersion >=AcroColorHFT_VERSION_9), *((PDDocColorConvertPageExSELPROTO)(gAcroColorHFT[PDDocColorConvertPageExSEL])))
	
/* Replacements for deprecated APIs */
#define ACPresetListItemFileAndNameU (ACROASSERT(gAcroColorVersion >=AcroColorHFT_VERSION_10), *((ACPresetListItemFileAndNameUSELPROTO)(gAcroColorHFT[ACPresetListItemFileAndNameUSEL])))
#define ACLoadSettingsU (ACROASSERT(gAcroColorVersion >=AcroColorHFT_VERSION_10), *((ACLoadSettingsUSELPROTO)(gAcroColorHFT[ACLoadSettingsUSEL])))
#define ACMonitorProfileN (ACROASSERT(gAcroColorVersion >=AcroColorHFT_VERSION_10), *((ACMonitorProfileNSELPROTO) (gAcroColorHFT[ACMonitorProfileNSEL])))

/* PDColorConvertPDEElementEx*/
#define PDColorConvertPDEElementEx (ACROASSERT(gAcroColorVersion >=AcroColorHFT_VERSION_11), *((PDColorConvertPDEElementExSELPROTO)(gAcroColorHFT[PDColorConvertPDEElementExSEL])))

#define ACGetBlackPointCompensation (ACROASSERT(gAcroColorVersion >=AcroColorHFT_VERSION_12), *((ACGetBlackPointCompensationSELPROTO)(gAcroColorHFT[ACGetBlackPointCompensationSEL])))
#define ACSetBlackPointCompensation (ACROASSERT(gAcroColorVersion >=AcroColorHFT_VERSION_12), *((ACSetBlackPointCompensationSELPROTO)(gAcroColorHFT[ACSetBlackPointCompensationSEL])))

/* If you add, delete, or modify procs listed in this file please read the instructions at the top about how to properly change the version number */

#endif /* PI_ACROCOLOR_VERSION != 0 */

#endif /* PLUGIN */

#ifdef __cplusplus
}
#endif

#undef EXTERNAL_ACROCOLORPROCS_USER

#endif /* !defined(_H_AcroColorCalls) */
