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

 CosCalls.h

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

#ifndef _H_CosCalls
#define _H_CosCalls
#include "acroassert.h"
#ifdef THREAD_SAFE_PDFL
#include "PDFLInitCommon.h"
#endif /* defined THREAD_SAFE_PDFL */

/* for Adobe use only */
#define _CosHFT_LATEST_VERSION 0x00090000
#define _CosHFT_LAST_BETA_COMPATIBLE_VERSION 0x00090000
#define _CosHFT_IS_BETA 0

/* for public use */
#define CosHFT_LATEST_VERSION (_CosHFT_IS_BETA ? (kHFT_IN_BETA_FLAG | _CosHFT_LATEST_VERSION) : _CosHFT_LATEST_VERSION)

#define CosHFT_VERSION_2   0x00020000
#define CosHFT_VERSION_3   0x00020002
#define CosHFT_VERSION_4   0x00040000
#define CosHFT_VERSION_4_5 0x00040005
#define CosHFT_VERSION_5   0x00050000
#define CosHFT_VERSION_5_1 0x00050001
#define CosHFT_VERSION_6   0x00060000
#define CosHFT_VERSION_7   0x00070000
#define CosHFT_VERSION_8   0x00080000
#define CosHFT_VERSION_9   CosHFT_LATEST_VERSION

#ifdef __cplusplus
extern "C" {
#endif

#include "CosExpT.h"

#ifdef NPROC /* This might be defined in sys/procs.h */
#undef NPROC
#endif /* NPROC */
	
#if !PLUGIN
	/* Static link */
	#define NPROC(returnType, name, params)		\
		ACEX1 returnType ACEX2 name params;
	#define NOPROC(name)

	#include "CosProcs.h"

	#undef NPROC
	#undef NOPROC
#endif /*!PLUGIN */

#if PLUGIN
	/* HFT version */
	#include "PIRequir.h"
	
	/* Enumerate the selectors */
	#define NPROC(returnType, name, params)			\
		name##SEL,
	
   #define NOPROC(name)	\
			name##SEL,
	enum {
		COSBAD_SELECTOR,
		#include "CosProcs.h"
		COSNUMSELECTORSplusOne
	};
	
	#define COSNUMSELECTORS (COSNUMSELECTORSplusOne - 1)
	
	/* Create the prototypes */
	#undef NPROC
	#undef NOPROC
	#define NPROC(returnType, name, params)		\
		typedef ACCBPROTO1 returnType (ACCBPROTO2 *name##SELPROTO)params;
	#define NOPROC(name)
		#include "CosProcs.h"
	#undef NOPROC
	#undef NPROC
	


#if PI_COS_VERSION != 0

#ifdef THREAD_SAFE_PDFL
	#define gCosHFT (GetHFTLocations()->cosHFT)
	#define gCosVersion (GetHFTLocations()->cosVersion)
#else
	extern HFT gCosHFT;
	extern ASUns32 gCosVersion;
#endif /* defined THREAD_SAFE_PDFL */

#if !STATIC_HFT
/* CosObjEqual */
#define CosObjEqual (ACROASSERT(gCosVersion >=CosHFT_VERSION_2), *((CosObjEqualSELPROTO)(gCosHFT[CosObjEqualSEL])))

/* CosObjGetType */
#define CosObjGetType (ACROASSERT(gCosVersion >=CosHFT_VERSION_2), *((CosObjGetTypeSELPROTO)(gCosHFT[CosObjGetTypeSEL])))

/* CosObjIsIndirect */
#define CosObjIsIndirect (ACROASSERT(gCosVersion >=CosHFT_VERSION_2), *((CosObjIsIndirectSELPROTO)(gCosHFT[CosObjIsIndirectSEL])))

/* CosObjEnum */
#define CosObjEnum (ACROASSERT(gCosVersion >=CosHFT_VERSION_2), *((CosObjEnumSELPROTO)(gCosHFT[CosObjEnumSEL])))

/* CosObjGetDoc */
#define CosObjGetDoc (ACROASSERT(gCosVersion >=CosHFT_VERSION_2), *((CosObjGetDocSELPROTO)(gCosHFT[CosObjGetDocSEL])))

/* CosNewNull */
#define CosNewNull (ACROASSERT(gCosVersion >=CosHFT_VERSION_2), *((CosNewNullSELPROTO)(gCosHFT[CosNewNullSEL])))

/* CosNewInteger */
#define CosNewInteger (ACROASSERT(gCosVersion >=CosHFT_VERSION_2), *((CosNewIntegerSELPROTO)(gCosHFT[CosNewIntegerSEL])))

/* CosNewFixed */
//#define CosNewFixed (ACROASSERT(gCosVersion >=CosHFT_VERSION_2), *((CosNewFixedSELPROTO)(gCosHFT[CosNewFixedSEL])))

/* CosNewBoolean */
#define CosNewBoolean (ACROASSERT(gCosVersion >=CosHFT_VERSION_2), *((CosNewBooleanSELPROTO)(gCosHFT[CosNewBooleanSEL])))

/* CosNewName */
#define CosNewName (ACROASSERT(gCosVersion >=CosHFT_VERSION_2), *((CosNewNameSELPROTO)(gCosHFT[CosNewNameSEL])))

/* CosNewString */
#define CosNewString (ACROASSERT(gCosVersion >=CosHFT_VERSION_2), *((CosNewStringSELPROTO)(gCosHFT[CosNewStringSEL])))

/* CosNewArray */
#define CosNewArray (ACROASSERT(gCosVersion >=CosHFT_VERSION_2), *((CosNewArraySELPROTO)(gCosHFT[CosNewArraySEL])))

/* CosNewDict */
#define CosNewDict (ACROASSERT(gCosVersion >=CosHFT_VERSION_2), *((CosNewDictSELPROTO)(gCosHFT[CosNewDictSEL])))

/* CosNewStream */
#define CosNewStream (ACROASSERT(gCosVersion >=CosHFT_VERSION_2), *((CosNewStreamSELPROTO)(gCosHFT[CosNewStreamSEL])))

/* CosObjDestroy */
#define CosObjDestroy (ACROASSERT(gCosVersion >=CosHFT_VERSION_2), *((CosObjDestroySELPROTO)(gCosHFT[CosObjDestroySEL])))

/* CosIntegerValue */
#define CosIntegerValue (ACROASSERT(gCosVersion >=CosHFT_VERSION_2), *((CosIntegerValueSELPROTO)(gCosHFT[CosIntegerValueSEL])))

/* CosFixedValue */
#define CosFixedValue (ACROASSERT(gCosVersion >=CosHFT_VERSION_2), *((CosFixedValueSELPROTO)(gCosHFT[CosFixedValueSEL])))

/* CosBooleanValue */
#define CosBooleanValue (ACROASSERT(gCosVersion >=CosHFT_VERSION_2), *((CosBooleanValueSELPROTO)(gCosHFT[CosBooleanValueSEL])))

/* CosNameValue */
#define CosNameValue (ACROASSERT(gCosVersion >=CosHFT_VERSION_2), *((CosNameValueSELPROTO)(gCosHFT[CosNameValueSEL])))

/* CosStringValue */
#define CosStringValue (ACROASSERT(gCosVersion >=CosHFT_VERSION_2), *((CosStringValueSELPROTO)(gCosHFT[CosStringValueSEL])))

/* CosDictGet */
#define CosDictGet (ACROASSERT(gCosVersion >=CosHFT_VERSION_2), *((CosDictGetSELPROTO)(gCosHFT[CosDictGetSEL])))

/* CosDictPut */
#define CosDictPut (ACROASSERT(gCosVersion >=CosHFT_VERSION_2), *((CosDictPutSELPROTO)(gCosHFT[CosDictPutSEL])))

/* CosDictRemove */
#define CosDictRemove (ACROASSERT(gCosVersion >=CosHFT_VERSION_2), *((CosDictRemoveSELPROTO)(gCosHFT[CosDictRemoveSEL])))

/* CosDictKnown */
#define CosDictKnown (ACROASSERT(gCosVersion >=CosHFT_VERSION_2), *((CosDictKnownSELPROTO)(gCosHFT[CosDictKnownSEL])))

/* CosArrayGet */
#define CosArrayGet (ACROASSERT(gCosVersion >=CosHFT_VERSION_2), *((CosArrayGetSELPROTO)(gCosHFT[CosArrayGetSEL])))

/* CosArrayPut */
#define CosArrayPut (ACROASSERT(gCosVersion >=CosHFT_VERSION_2), *((CosArrayPutSELPROTO)(gCosHFT[CosArrayPutSEL])))

/* CosArrayInsert */
#define CosArrayInsert (ACROASSERT(gCosVersion >=CosHFT_VERSION_2), *((CosArrayInsertSELPROTO)(gCosHFT[CosArrayInsertSEL])))

/* CosArrayRemove */
#define CosArrayRemove (ACROASSERT(gCosVersion >=CosHFT_VERSION_2), *((CosArrayRemoveSELPROTO)(gCosHFT[CosArrayRemoveSEL])))

/* CosArrayLength */
#define CosArrayLength (ACROASSERT(gCosVersion >=CosHFT_VERSION_2), *((CosArrayLengthSELPROTO)(gCosHFT[CosArrayLengthSEL])))

/* CosStreamLength */
#define CosStreamLength (ACROASSERT(gCosVersion >=CosHFT_VERSION_2), *((CosStreamLengthSELPROTO)(gCosHFT[CosStreamLengthSEL])))

/* CosStreamDict */
#define CosStreamDict (ACROASSERT(gCosVersion >=CosHFT_VERSION_2), *((CosStreamDictSELPROTO)(gCosHFT[CosStreamDictSEL])))

/* CosStreamOpenStm */
#define CosStreamOpenStm (ACROASSERT(gCosVersion >=CosHFT_VERSION_2), *((CosStreamOpenStmSELPROTO)(gCosHFT[CosStreamOpenStmSEL])))

/* CosStreamPos */
#define CosStreamPos (ACROASSERT(gCosVersion >=CosHFT_VERSION_2), *((CosStreamPosSELPROTO)(gCosHFT[CosStreamPosSEL])))

/* CosDocGetRoot */
#define CosDocGetRoot (ACROASSERT(gCosVersion >=CosHFT_VERSION_2), *((CosDocGetRootSELPROTO)(gCosHFT[CosDocGetRootSEL])))

/* CosDocGetInfoDict */
#define CosDocGetInfoDict (ACROASSERT(gCosVersion >=CosHFT_VERSION_2), *((CosDocGetInfoDictSELPROTO)(gCosHFT[CosDocGetInfoDictSEL])))

#ifndef ANDROID_ENV
/* CosDecryptData */
#define CosDecryptData (ACROASSERT(gCosVersion >=CosHFT_VERSION_2), *((CosDecryptDataSELPROTO)(gCosHFT[CosDecryptDataSEL])))

/* CosEncryptData */
#define CosEncryptData (ACROASSERT(gCosVersion >=CosHFT_VERSION_2), *((CosEncryptDataSELPROTO)(gCosHFT[CosEncryptDataSEL])))
#endif

/* Acrobat 3.0 additions */
#if PI_COS_VERSION >= 0x00020002

/* CosDocOpenWithParams */
#define CosDocOpenWithParams (ACROASSERT(gCosVersion >=CosHFT_VERSION_3), *((CosDocOpenWithParamsSELPROTO)(gCosHFT[CosDocOpenWithParamsSEL])))

/* CosDocClose */
#define CosDocClose (ACROASSERT(gCosVersion >=CosHFT_VERSION_3), *((CosDocCloseSELPROTO)(gCosHFT[CosDocCloseSEL])))

/* CosDocCreate */
#define CosDocCreate (ACROASSERT(gCosVersion >=CosHFT_VERSION_3), *((CosDocCreateSELPROTO)(gCosHFT[CosDocCreateSEL])))

/* CosDocSaveToFile */
#define CosDocSaveToFile (ACROASSERT(gCosVersion >=CosHFT_VERSION_3), *((CosDocSaveToFileSELPROTO)(gCosHFT[CosDocSaveToFileSEL])))

/* CosDocSetDirty */
#define CosDocSetDirty (ACROASSERT(gCosVersion >=CosHFT_VERSION_3), *((CosDocSetDirtySELPROTO)(gCosHFT[CosDocSetDirtySEL])))	


#endif /* PI_COS_VERSION 0x00020002 */


/* Acrobat 4.0 additions */

/* CosObjGetID */
#define CosObjGetID (ACROASSERT(gCosVersion >=CosHFT_VERSION_4), *((CosObjGetIDSELPROTO)(gCosHFT[CosObjGetIDSEL])))

/* CosObjGetGeneration */
#define CosObjGetGeneration (ACROASSERT(gCosVersion >=CosHFT_VERSION_4), *((CosObjGetGenerationSELPROTO)(gCosHFT[CosObjGetGenerationSEL])))

/* CosDocGetObjByID */
#define CosDocGetObjByID (ACROASSERT(gCosVersion >=CosHFT_VERSION_4), *((CosDocGetObjByIDSELPROTO)(gCosHFT[CosDocGetObjByIDSEL])))

/* CosDocSaveWithParams */
#define CosDocSaveWithParams (ACROASSERT(gCosVersion >=CosHFT_VERSION_4), *((CosDocSaveWithParamsSELPROTO)(gCosHFT[CosDocSaveWithParamsSEL])))


/* CosDocEnumEOFs */
#define CosDocEnumEOFs (ACROASSERT(gCosVersion >=CosHFT_VERSION_4), *((CosDocEnumEOFsSELPROTO)(gCosHFT[CosDocEnumEOFsSEL])))

/* CosStringSetHexFlag */
#define CosStringSetHexFlag (ACROASSERT(gCosVersion >=CosHFT_VERSION_4), *((CosStringSetHexFlagSELPROTO) (gCosHFT[CosStringSetHexFlagSEL])))

/* CosStringGetHexFlag */
#define CosStringGetHexFlag (ACROASSERT(gCosVersion >=CosHFT_VERSION_4), *((CosStringGetHexFlagSELPROTO) (gCosHFT[CosStringGetHexFlagSEL])))

/* CosObjHash */
#define CosObjHash (ACROASSERT(gCosVersion >=CosHFT_VERSION_4), *((CosObjHashSELPROTO) (gCosHFT[CosObjHashSEL])))

/* CosObjCopy */
#define CosObjCopy (ACROASSERT(gCosVersion >=CosHFT_VERSION_4), *((CosObjCopySELPROTO) (gCosHFT[CosObjCopySEL])))

/* CosArrayRemove */
#define CosArrayRemoveNth (ACROASSERT(gCosVersion >=CosHFT_VERSION_4), *((CosArrayRemoveNthSELPROTO)(gCosHFT[CosArrayRemoveNthSEL])))

/* CosDocEnumIndirect */
#define CosDocEnumIndirect (ACROASSERT(gCosVersion >=CosHFT_VERSION_4), *((CosDocEnumIndirectSELPROTO)(gCosHFT[CosDocEnumIndirectSEL])))


/* Acrobat 4.01 additions */

#ifndef ANDROID_ENV
/* CosCryptGetVersion */
#define CosCryptGetVersion       (ACROASSERT(gCosVersion >=CosHFT_VERSION_4_5), *((CosCryptGetVersionSELPROTO)      (gCosHFT[CosCryptGetVersionSEL])))

/* CosDecryptGetMaxKeyBytes */
#define CosDecryptGetMaxKeyBytes (ACROASSERT(gCosVersion >=CosHFT_VERSION_4_5), *((CosDecryptGetMaxKeyBytesSELPROTO)(gCosHFT[CosDecryptGetMaxKeyBytesSEL])))

/* CosEncryptGetMaxKeyBytes */
#define CosEncryptGetMaxKeyBytes (ACROASSERT(gCosVersion >=CosHFT_VERSION_4_5), *((CosEncryptGetMaxKeyBytesSELPROTO)(gCosHFT[CosEncryptGetMaxKeyBytesSEL])))
#endif

/* Acrobat 5.0 additions */

/* CosStringValueCopy */
#define CosCopyStringValue (ACROASSERT(gCosVersion >=CosHFT_VERSION_5), *((CosCopyStringValueSELPROTO)(gCosHFT[CosCopyStringValueSEL])))

/* CosStringValueSafe */
#define CosStringValueSafe (ACROASSERT(gCosVersion >=CosHFT_VERSION_5), *((CosStringValueSafeSELPROTO)(gCosHFT[CosStringValueSafeSEL])))

/* CosDocGetID */
#define CosDocGetID (ACROASSERT(gCosVersion >=CosHFT_VERSION_5), *((CosDocGetIDSELPROTO)(gCosHFT[CosDocGetIDSEL])))

/* CosObjCmp */
#define CosObjCmp (ACROASSERT(gCosVersion >=CosHFT_VERSION_5), *((CosObjCmpSELPROTO)(gCosHFT[CosObjCmpSEL])))

/* PDFLib 5.01 additions */
/* CosSetMaxDocStorage */
#define CosSetMaxDocStorage (ACROASSERT(gCosVersion >=CosHFT_VERSION_5_1), *((CosSetMaxDocStorageSELPROTO)(gCosHFT[CosSetMaxDocStorageSEL])))

/* Acrobat 6 additions */
/* CosDocObjIsWithinRange */
#define CosDocObjIsWithinRange (ACROASSERT(gCosVersion >=CosHFT_VERSION_6), *((CosDocObjIsWithinRangeSELPROTO)(gCosHFT[CosDocObjIsWithinRangeSEL])))

/* CosObjIsCompressed */
#define CosObjIsCompressed (ACROASSERT(gCosVersion >=CosHFT_VERSION_6), *((CosObjIsCompressedSELPROTO)(gCosHFT[CosObjIsCompressedSEL])))

/* CosNewObjCollection */
#define CosNewObjCollection (ACROASSERT(gCosVersion >=CosHFT_VERSION_6), *((CosNewObjCollectionSELPROTO)(gCosHFT[CosNewObjCollectionSEL])))

/* CosObjCollectionIsNull */
#define CosObjCollectionIsNull (ACROASSERT(gCosVersion >=CosHFT_VERSION_6), *((CosObjCollectionIsNullSELPROTO)(gCosHFT[CosObjCollectionIsNullSEL])))

/* CosObjGetCollection */
#define CosObjGetCollection (ACROASSERT(gCosVersion >=CosHFT_VERSION_6), *((CosObjGetCollectionSELPROTO)(gCosHFT[CosObjGetCollectionSEL])))

/* CosObjAddToCollection */
#define CosObjAddToCollection (ACROASSERT(gCosVersion >=CosHFT_VERSION_6), *((CosObjAddToCollectionSELPROTO)(gCosHFT[CosObjAddToCollectionSEL])))

/* CosObjRemoveFromCollection */
#define CosObjRemoveFromCollection (ACROASSERT(gCosVersion >=CosHFT_VERSION_6), *((CosObjRemoveFromCollectionSELPROTO)(gCosHFT[CosObjRemoveFromCollectionSEL])))

/* CosObjSetCompressibility */
#define CosObjSetCompressibility (ACROASSERT(gCosVersion >=CosHFT_VERSION_6), *((CosObjSetCompressibilitySELPROTO)(gCosHFT[CosObjSetCompressibilitySEL])))

/* CosObjGetCompressibility */
#define CosObjGetCompressibility (ACROASSERT(gCosVersion >=CosHFT_VERSION_6), *((CosObjGetCompressibilitySELPROTO)(gCosHFT[CosObjGetCompressibilitySEL])))

/* CosObjCollectionSize */
#define CosObjCollectionSize (ACROASSERT(gCosVersion >=CosHFT_VERSION_6), *((CosObjCollectionSizeSELPROTO)(gCosHFT[CosObjCollectionSizeSEL])))

/* CosObjCollectionEqual */
#define CosObjCollectionEqual (ACROASSERT(gCosVersion >=CosHFT_VERSION_6), *((CosObjCollectionEqualSELPROTO)(gCosHFT[CosObjCollectionEqualSEL])))

/* CosObjCollectionEnum */
#define CosObjCollectionEnum (ACROASSERT(gCosVersion >=CosHFT_VERSION_6), *((CosObjCollectionEnumSELPROTO)(gCosHFT[CosObjCollectionEnumSEL])))

/* CosObjRefreshAfterLinearizedSave */
#define CosObjRefreshAfterLinearizedSave (ACROASSERT(gCosVersion >=CosHFT_VERSION_6), *((CosObjRefreshAfterLinearizedSaveSELPROTO)(gCosHFT[CosObjRefreshAfterLinearizedSaveSEL])))

/* CosDocHasFullCompression */
#define CosDocHasFullCompression (ACROASSERT(gCosVersion >=CosHFT_VERSION_6), *((CosDocHasFullCompressionSELPROTO)(gCosHFT[CosDocHasFullCompressionSEL])))

/* CosDocHasPartialCompression */
#define CosDocHasPartialCompression (ACROASSERT(gCosVersion >=CosHFT_VERSION_6), *((CosDocHasPartialCompressionSELPROTO)(gCosHFT[CosDocHasPartialCompressionSEL])))


/* Acrobat 7 additions */

#define CosCallMacro(fname, version) (ACROASSERT(gCosVersion >=CosHFT_VERSION_##version), *((fname##SELPROTO)(gCosHFT[fname##SEL])))

/* 64-bit integers */
#define CosNewInteger64		CosCallMacro(CosNewInteger64,	7)
#define CosInteger64Value	CosCallMacro(CosInteger64Value,	7)

/* IEEE floats */
#define CosNewFloat			CosCallMacro(CosNewFloat,		7)
#define CosFloatValue		CosCallMacro(CosFloatValue,		7)

/* "Key as object" versions of CosDictGet et al. */
#define CosDictGetKey		CosCallMacro(CosDictGetKey,		7)
#define CosDictPutKey		CosCallMacro(CosDictPutKey,		7)
#define CosDictRemoveKey	CosCallMacro(CosDictRemoveKey,	7)
#define CosDictKnownKey		CosCallMacro(CosDictKnownKey,	7)

/* "Key as string" versions of CosDictGet et al. */
#define CosDictGetKeyString		CosCallMacro(CosDictGetKeyString,	7)
#define CosDictPutKeyString		CosCallMacro(CosDictPutKeyString,	7)
#define CosDictRemoveKeyString	CosCallMacro(CosDictRemoveKeyString,7)
#define CosDictKnownKeyString	CosCallMacro(CosDictKnownKeyString,	7)

/* Weak references */
#define CosDictSetWeakReference		CosCallMacro(CosDictSetWeakReference,	7)
#define CosDictIsWeakReference		CosCallMacro(CosDictIsWeakReference,	7)

#define CosArraySetWeakReference	CosCallMacro(CosArraySetWeakReference,	7)
#define CosArrayIsWeakReference		CosCallMacro(CosArrayIsWeakReference,	7)

/* Strong references */
#define CosObjAcquire			CosCallMacro(CosObjAcquire,	7)
#define CosObjRelease			CosCallMacro(CosObjRelease,	7)

/* Names from strings */
#define CosNewNameFromString	CosCallMacro(CosNewNameFromString,	7)
#define CosCopyNameStringValue	CosCallMacro(CosCopyNameStringValue,7)

/* Support for 64-bit file positions */
#define CosDocEnumEOFs64		CosCallMacro(CosDocEnumEOFs64,	7)

/* Support for numerical type-checking */
#define CosNumberIsWithinASInt32Range	CosCallMacro(CosNumberIsWithinASInt32Range,	7)
#define CosNumberIsWithinASFixedRange	CosCallMacro(CosNumberIsWithinASFixedRange,	7)

#define CosDocObjIsWithinRange64	CosCallMacro(CosDocObjIsWithinRange64,	7)
#define CosNewStream64				CosCallMacro(CosNewStream64,			7)
#define CosStreamLength64			CosCallMacro(CosStreamLength64,			7)
#define CosStreamPos64				CosCallMacro(CosStreamPos64,			7)

/* Acrobat 9 additions */

/* IEEE double-precision floats */
#define CosNewDouble		CosCallMacro(CosNewDouble,		9)
#define CosNewDoubleEx		CosCallMacro(CosNewDoubleEx,	9)
#define CosDoubleValue		CosCallMacro(CosDoubleValue,	9)

/* Extensions / ADBE version support */
#define CosDocGetAdobeExtensionLevel	CosCallMacro(CosDocGetAdobeExtensionLevel, 9)
#define CosDocSetAdobeExtensionLevel	CosCallMacro(CosDocSetAdobeExtensionLevel, 9)
#define CosDocHasISOExtensions			CosCallMacro(CosDocHasISOExtensions, 9)

//#undef CosCallMacro

/* If you add, delete, or modify procs listed in this file please read the instructions at the top about how to properly change the version number */

#endif /* !STATIC_HFT */
#endif /* PI_COS_VERSION != 0 */	

#endif /* PLUGIN */

#ifdef __cplusplus
}
#endif

#endif /* !defined(_H_CosCalls) */

