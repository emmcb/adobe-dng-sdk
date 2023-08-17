/*********************************************************************

 ADOBE SYSTEMS INCORPORATED
 Copyright (C) 1999-2006 Adobe Systems Incorporated
 All rights reserved.

 NOTICE: Adobe permits you to use, modify, and distribute this file
 in accordance with the terms of the Adobe license agreement
 accompanying it. If you have received this file from a source other
 than Adobe, then your use, modification, or distribution of it
 requires the prior written permission of Adobe.

 ---------------------------------------------------------------------

 ASExtraCalls.h

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

#ifndef _H_ASExtraCalls
#define _H_ASExtraCalls

#include "ASExtraExpT.h"
#include "acroassert.h"
#ifdef THREAD_SAFE_PDFL
#include "PDFLInitCommon.h"
#endif /* defined THREAD_SAFE_PDFL */

/* for Adobe use only */
#define _ASExtraHFT_LATEST_VERSION 0x00090000
#define _ASExtraHFT_LAST_BETA_COMPATIBLE_VERSION 0x00090000
#define _ASEXTRAHFT_IS_BETA 0

/* for public use */
#define ASExtraHFT_LATEST_VERSION (_ASEXTRAHFT_IS_BETA ? (kHFT_IN_BETA_FLAG | _ASExtraHFT_LATEST_VERSION) : _ASExtraHFT_LATEST_VERSION)

#define ASExtraHFT_VERSION_5 0x00050000
#define ASExtraHFT_VERSION_6 0x00060000
#define ASExtraHFT_VERSION_7 0x00070000
#define ASExtraHFT_VERSION_9 ASExtraHFT_LATEST_VERSION

#ifdef __cplusplus
extern "C" {
#endif

	/* "Routines" described as macros on top of real
	   HFT calls.
	*/
	#define ASTextEqual(a, b) (ASTextCmp((a), (b)) == 0)

#ifdef NPROC
#undef NPROC
#endif

#if !PLUGIN
	/* Static link */
	#define NPROC(returnType, name, params)		\
		ACEX1 returnType ACEX2 name params;
	#include "ASExtraProcs.h"
	#undef NPROC
#endif /* !PLUGIN */

#if PLUGIN
	/* HFT version */
   #include "PIRequir.h"
	
	/* Enumerate the selectors */
	#define NPROC(returnType, name, params)			\
		name##SEL,
	
	enum {
		ASExtraBAD_SELECTOR,
		#include "ASExtraProcs.h"
		ASExtraNUMSELECTORSplusOne
	};
	
	#define ASExtraNUMSELECTORS (ASExtraNUMSELECTORSplusOne - 1)
	
	/* Create the prototypes */
	#undef NPROC
	#define NPROC(returnType, name, params)		\
		typedef ACCBPROTO1 returnType (ACCBPROTO2 *name##SELPROTO)params;
		#include "ASExtraProcs.h"
	#undef NPROC
	
#if PI_ASEXTRA_VERSION != 0
#ifdef THREAD_SAFE_PDFL
	#define gASExtraHFT (GetHFTLocations()->asExtraHFT)
	#define gASExtraVersion (GetHFTLocations()->asExtraVersion)
#else
	extern HFT gASExtraHFT;
	extern ASUns32 gASExtraVersion;
#endif /* defined THREAD_SAFE_PDFL */

#define ASEXTRAROUTINE(level, name) (ACROASSERT(gASExtraVersion >=level), *((name##SELPROTO)(gASExtraHFT[name##SEL])))

#if !STATIC_HFT
	/* version 5.0 routines */
	#define ASScriptToHostEncoding ASEXTRAROUTINE(ASExtraHFT_VERSION_5,ASScriptToHostEncoding)
	#define ASScriptFromHostEncoding ASEXTRAROUTINE(ASExtraHFT_VERSION_5,ASScriptFromHostEncoding)
	#define ASTextNew ASEXTRAROUTINE(ASExtraHFT_VERSION_5,ASTextNew)
	#define ASTextFromUnicode ASEXTRAROUTINE(ASExtraHFT_VERSION_5,ASTextFromUnicode)
	#define ASTextFromSizedUnicode ASEXTRAROUTINE(ASExtraHFT_VERSION_5,ASTextFromSizedUnicode)
	#define ASTextFromEncoded ASEXTRAROUTINE(ASExtraHFT_VERSION_5,ASTextFromEncoded)
	#define ASTextFromSizedEncoded ASEXTRAROUTINE(ASExtraHFT_VERSION_5,ASTextFromSizedEncoded)
	#define ASTextFromScriptText ASEXTRAROUTINE(ASExtraHFT_VERSION_5,ASTextFromScriptText)
	#define ASTextFromSizedScriptText ASEXTRAROUTINE(ASExtraHFT_VERSION_5,ASTextFromSizedScriptText)
	#define ASTextFromPDText ASEXTRAROUTINE(ASExtraHFT_VERSION_5,ASTextFromPDText)
	#define ASTextFromSizedPDText ASEXTRAROUTINE(ASExtraHFT_VERSION_5,ASTextFromSizedPDText)
	#define ASTextDestroy ASEXTRAROUTINE(ASExtraHFT_VERSION_5,ASTextDestroy)
	#define ASTextSetUnicode ASEXTRAROUTINE(ASExtraHFT_VERSION_5,ASTextSetUnicode)
	#define ASTextSetSizedUnicode ASEXTRAROUTINE(ASExtraHFT_VERSION_5,ASTextSetSizedUnicode)
	#define ASTextSetEncoded ASEXTRAROUTINE(ASExtraHFT_VERSION_5,ASTextSetEncoded)
	#define ASTextSetSizedEncoded ASEXTRAROUTINE(ASExtraHFT_VERSION_5,ASTextSetSizedEncoded)
	#define ASTextSetScriptText ASEXTRAROUTINE(ASExtraHFT_VERSION_5,ASTextSetScriptText)
	#define ASTextSetSizedScriptText ASEXTRAROUTINE(ASExtraHFT_VERSION_5,ASTextSetSizedScriptText)
	#define ASTextSetPDText ASEXTRAROUTINE(ASExtraHFT_VERSION_5,ASTextSetPDText)
	#define ASTextSetSizedPDText ASEXTRAROUTINE(ASExtraHFT_VERSION_5,ASTextSetSizedPDText)
	#define ASTextGetUnicode ASEXTRAROUTINE(ASExtraHFT_VERSION_5,ASTextGetUnicode)
	#define ASTextGetUnicodeCopy ASEXTRAROUTINE(ASExtraHFT_VERSION_5,ASTextGetUnicodeCopy)
	#define ASTextGetEncoded ASEXTRAROUTINE(ASExtraHFT_VERSION_5,ASTextGetEncoded)
    #define ASTextGetEncodedCopy ASEXTRAROUTINE(ASExtraHFT_VERSION_5,ASTextGetEncodedCopy)
    #define ASTextGetScriptText ASEXTRAROUTINE(ASExtraHFT_VERSION_5,ASTextGetScriptText)
	#define ASTextGetScriptTextCopy ASEXTRAROUTINE(ASExtraHFT_VERSION_5,ASTextGetScriptTextCopy)
	#define ASTextGetPDTextCopy ASEXTRAROUTINE(ASExtraHFT_VERSION_5,ASTextGetPDTextCopy)
	#define ASTextGetBestEncoding ASEXTRAROUTINE(ASExtraHFT_VERSION_5,ASTextGetBestEncoding)
	#define ASTextGetBestScript ASEXTRAROUTINE(ASExtraHFT_VERSION_5,ASTextGetBestScript)
	#define ASTextGetCountry ASEXTRAROUTINE(ASExtraHFT_VERSION_5,ASTextGetCountry)
	#define ASTextSetCountry ASEXTRAROUTINE(ASExtraHFT_VERSION_5,ASTextSetCountry)
	#define ASTextGetLanguage ASEXTRAROUTINE(ASExtraHFT_VERSION_5,ASTextGetLanguage)
	#define ASTextSetLanguage ASEXTRAROUTINE(ASExtraHFT_VERSION_5,ASTextSetLanguage)
	#define ASTextCat ASEXTRAROUTINE(ASExtraHFT_VERSION_5,ASTextCat)
	#define ASTextCatMany ASEXTRAROUTINE(ASExtraHFT_VERSION_5,ASTextCatMany)
	#define ASTextCopy ASEXTRAROUTINE(ASExtraHFT_VERSION_5,ASTextCopy)
	#define ASTextDup ASEXTRAROUTINE(ASExtraHFT_VERSION_5,ASTextDup)
	#define ASTextCmp ASEXTRAROUTINE(ASExtraHFT_VERSION_5,ASTextCmp)
	#define ASTextReplace ASEXTRAROUTINE(ASExtraHFT_VERSION_5,ASTextReplace)
	#define ASTextReplaceASCII ASEXTRAROUTINE(ASExtraHFT_VERSION_5,ASTextReplaceASCII)

	#define ASCabNew ASEXTRAROUTINE(ASExtraHFT_VERSION_5,ASCabNew)
	#define ASCabFromEntryList ASEXTRAROUTINE(ASExtraHFT_VERSION_5,ASCabFromEntryList)
	#define ASCabDestroy ASEXTRAROUTINE(ASExtraHFT_VERSION_5,ASCabDestroy)
	#define ASCabNumEntries ASEXTRAROUTINE(ASExtraHFT_VERSION_5,ASCabNumEntries)
	#define ASCabKnown ASEXTRAROUTINE(ASExtraHFT_VERSION_5,ASCabKnown)
	#define ASCabGetType ASEXTRAROUTINE(ASExtraHFT_VERSION_5,ASCabGetType)
	#define ASCabEnum ASEXTRAROUTINE(ASExtraHFT_VERSION_5,ASCabEnum)
	#define ASCabRemove ASEXTRAROUTINE(ASExtraHFT_VERSION_5,ASCabRemove)
	#define ASCabGetBool ASEXTRAROUTINE(ASExtraHFT_VERSION_5,ASCabGetBool)
	#define ASCabPutBool ASEXTRAROUTINE(ASExtraHFT_VERSION_5,ASCabPutBool)
	#define ASCabGetInt ASEXTRAROUTINE(ASExtraHFT_VERSION_5,ASCabGetInt)
	#define ASCabPutInt ASEXTRAROUTINE(ASExtraHFT_VERSION_5,ASCabPutInt)
	#define ASCabGetAtom ASEXTRAROUTINE(ASExtraHFT_VERSION_5,ASCabGetAtom)
	#define ASCabPutAtom ASEXTRAROUTINE(ASExtraHFT_VERSION_5,ASCabPutAtom)
	#define ASCabGetDouble ASEXTRAROUTINE(ASExtraHFT_VERSION_5,ASCabGetDouble)
	#define ASCabPutDouble ASEXTRAROUTINE(ASExtraHFT_VERSION_5,ASCabPutDouble)
	#define ASCabGetPointerRaw ASEXTRAROUTINE(ASExtraHFT_VERSION_5,ASCabGetPointerRaw)
	#define ASCabDetachPointerRaw ASEXTRAROUTINE(ASExtraHFT_VERSION_5,ASCabDetachPointerRaw)
	#define ASCabPutPointerRaw ASEXTRAROUTINE(ASExtraHFT_VERSION_5,ASCabPutPointerRaw)
	#define ASCabGetPointerDestroyProc ASEXTRAROUTINE(ASExtraHFT_VERSION_5,ASCabGetPointerDestroyProc)
	#define ASCabGetPointerType ASEXTRAROUTINE(ASExtraHFT_VERSION_5,ASCabGetPointerType)
	#define ASCabPutCab ASEXTRAROUTINE(ASExtraHFT_VERSION_5,ASCabPutCab)
	#define ASCabGetCab ASEXTRAROUTINE(ASExtraHFT_VERSION_5,ASCabGetCab)
	#define ASCabDetachCab ASEXTRAROUTINE(ASExtraHFT_VERSION_5,ASCabDetachCab)
	#define ASCabGetString ASEXTRAROUTINE(ASExtraHFT_VERSION_5,ASCabGetString)
	#define ASCabGetStringCopy ASEXTRAROUTINE(ASExtraHFT_VERSION_5,ASCabGetStringCopy)
	#define ASCabDetachString ASEXTRAROUTINE(ASExtraHFT_VERSION_5,ASCabDetachString)
	#define ASCabPutString ASEXTRAROUTINE(ASExtraHFT_VERSION_5,ASCabPutString)
	#define ASCabGetText ASEXTRAROUTINE(ASExtraHFT_VERSION_5,ASCabGetText)
	#define ASCabDetachText ASEXTRAROUTINE(ASExtraHFT_VERSION_5,ASCabDetachText)
	#define ASCabPutText ASEXTRAROUTINE(ASExtraHFT_VERSION_5,ASCabPutText)
	#define ASCabGetBinary ASEXTRAROUTINE(ASExtraHFT_VERSION_5,ASCabGetBinary)
	#define ASCabGetBinaryCopy ASEXTRAROUTINE(ASExtraHFT_VERSION_5,ASCabGetBinaryCopy)
	#define ASCabDetachBinary ASEXTRAROUTINE(ASExtraHFT_VERSION_5,ASCabDetachBinary)
	#define ASCabPutBinary ASEXTRAROUTINE(ASExtraHFT_VERSION_5,ASCabPutBinary)
	#define ASCabPutNull ASEXTRAROUTINE(ASExtraHFT_VERSION_5,ASCabPutNull)
	#define ASCabMakeEmpty ASEXTRAROUTINE(ASExtraHFT_VERSION_5,ASCabMakeEmpty)
	#define ASCabDestroyEmpties ASEXTRAROUTINE(ASExtraHFT_VERSION_5,ASCabDestroyEmpties)
	#define ASCabCopy ASEXTRAROUTINE(ASExtraHFT_VERSION_5,ASCabCopy)
	#define ASCabDup ASEXTRAROUTINE(ASExtraHFT_VERSION_5,ASCabDup)
	#define ASCabValueEqual ASEXTRAROUTINE(ASExtraHFT_VERSION_5,ASCabValueEqual)
	#define ASCabEqual ASEXTRAROUTINE(ASExtraHFT_VERSION_5,ASCabEqual)
	#define ASCabMunge ASEXTRAROUTINE(ASExtraHFT_VERSION_5,ASCabMunge)
	#define ASCabPutPathName ASEXTRAROUTINE(ASExtraHFT_VERSION_5,ASCabPutPathName)
	#define ASCabGetPathNameCopy ASEXTRAROUTINE(ASExtraHFT_VERSION_5,ASCabGetPathNameCopy)
	#define ASCabDetachPathName ASEXTRAROUTINE(ASExtraHFT_VERSION_5,ASCabDetachPathName)
	#define ASCabWriteToStream ASEXTRAROUTINE(ASExtraHFT_VERSION_5,ASCabWriteToStream)
	#define ASCabReadFromStream ASEXTRAROUTINE(ASExtraHFT_VERSION_5,ASCabReadFromStream)

	#define ASCabRename ASEXTRAROUTINE(ASExtraHFT_VERSION_5,ASCabRename)
	#define ASTextIsEmpty ASEXTRAROUTINE(ASExtraHFT_VERSION_5,ASTextIsEmpty)
	#define ASTextNormalizeEndOfLine ASEXTRAROUTINE(ASExtraHFT_VERSION_5,ASTextNormalizeEndOfLine)
	
	#define ASTextFromInt32 ASEXTRAROUTINE(ASExtraHFT_VERSION_5,ASTextFromInt32)
	#define ASTextFromUns32 ASEXTRAROUTINE(ASExtraHFT_VERSION_5,ASTextFromUns32)
	
	#define ASTextMakeEmpty ASEXTRAROUTINE(ASExtraHFT_VERSION_5,ASTextMakeEmpty)

	/* version 6.0 routines */

	#define ASTextReplaceBadChars ASEXTRAROUTINE(ASExtraHFT_VERSION_6,ASTextReplaceBadChars)
	#define ASCabGetUns ASEXTRAROUTINE(ASExtraHFT_VERSION_6,ASCabGetUns)
	#define ASCabPutUns ASEXTRAROUTINE(ASExtraHFT_VERSION_6,ASCabPutUns)
/* ASDate API #defines. 
*/
	#define ASDateNew ASEXTRAROUTINE(ASExtraHFT_VERSION_6,ASDateNew)
	#define ASDateCopy ASEXTRAROUTINE(ASExtraHFT_VERSION_6,ASDateCopy)
	#define ASDateDup ASEXTRAROUTINE(ASExtraHFT_VERSION_6,ASDateDup)
	#define ASDateClear ASEXTRAROUTINE(ASExtraHFT_VERSION_6,ASDateClear)
	#define ASDateDestroy ASEXTRAROUTINE(ASExtraHFT_VERSION_6,ASDateDestroy)
	#define ASTimeSpanNew ASEXTRAROUTINE(ASExtraHFT_VERSION_6,ASTimeSpanNew)
	#define ASTimeSpanCopy ASEXTRAROUTINE(ASExtraHFT_VERSION_6,ASTimeSpanCopy)
	#define ASTimeSpanDup ASEXTRAROUTINE(ASExtraHFT_VERSION_6,ASTimeSpanDup)
	#define ASTimeSpanDestroy ASEXTRAROUTINE(ASExtraHFT_VERSION_6,ASTimeSpanDestroy)
	#define ASDateSetToCurrentUTCTime ASEXTRAROUTINE(ASExtraHFT_VERSION_6,ASDateSetToCurrentUTCTime)
	#define ASDateSetToCurrentLocalTime ASEXTRAROUTINE(ASExtraHFT_VERSION_6,ASDateSetToCurrentLocalTime)
	#define ASDateSetLocalTimeOffset ASEXTRAROUTINE(ASExtraHFT_VERSION_6,ASDateSetLocalTimeOffset)
	#define ASDateSetTimeFromString ASEXTRAROUTINE(ASExtraHFT_VERSION_6,ASDateSetTimeFromString)
	#define ASDateSetTimeFromRec ASEXTRAROUTINE(ASExtraHFT_VERSION_6,ASDateSetTimeFromRec)
	#define ASDateSubtractCalendarTimeSpan ASEXTRAROUTINE(ASExtraHFT_VERSION_6,ASDateSubtractCalendarTimeSpan)
	#define ASDateAddCalendarTimeSpan ASEXTRAROUTINE(ASExtraHFT_VERSION_6,ASDateAddCalendarTimeSpan)
	#define ASDateSubtractTimeSpan ASEXTRAROUTINE(ASExtraHFT_VERSION_6,ASDateSubtractTimeSpan)
	#define ASDateAddTimeSpan ASEXTRAROUTINE(ASExtraHFT_VERSION_6,ASDateAddTimeSpan)
	#define ASDateCalendarDiff ASEXTRAROUTINE(ASExtraHFT_VERSION_6,ASDateCalendarDiff)
	#define ASDateExactDiff ASEXTRAROUTINE(ASExtraHFT_VERSION_6,ASDateExactDiff)
	#define ASDateGetTimeString ASEXTRAROUTINE(ASExtraHFT_VERSION_6,ASDateGetTimeString)
	#define ASDateGetUTCTime ASEXTRAROUTINE(ASExtraHFT_VERSION_6,ASDateGetUTCTime)
	#define ASDateGetLocalTimeNoDST ASEXTRAROUTINE(ASExtraHFT_VERSION_6,ASDateGetLocalTimeNoDST)
	#define ASDateGetLocalTime ASEXTRAROUTINE(ASExtraHFT_VERSION_6,ASDateGetLocalTime)
	#define ASCalendarTimeSpanCompare ASEXTRAROUTINE(ASExtraHFT_VERSION_6,ASCalendarTimeSpanCompare)
	#define ASTimeSpanCompare ASEXTRAROUTINE(ASExtraHFT_VERSION_6,ASTimeSpanCompare)
	#define ASCalendarTimeSpanAddWithBase ASEXTRAROUTINE(ASExtraHFT_VERSION_6,ASCalendarTimeSpanAddWithBase)
	#define ASTimeSpanAdd ASEXTRAROUTINE(ASExtraHFT_VERSION_6,ASTimeSpanAdd)
	#define ASCalendarTimeSpanDiff ASEXTRAROUTINE(ASExtraHFT_VERSION_6,ASCalendarTimeSpanDiff)
	#define ASTimeSpanDiff ASEXTRAROUTINE(ASExtraHFT_VERSION_6,ASTimeSpanDiff)
	#define ASDateCompare ASEXTRAROUTINE(ASExtraHFT_VERSION_6,ASDateCompare)
	#define ASTimeSpanSetFromASInt32 ASEXTRAROUTINE(ASExtraHFT_VERSION_6,ASTimeSpanSetFromASInt32)
	#define ASTimeSpanSetFromString ASEXTRAROUTINE(ASExtraHFT_VERSION_6,ASTimeSpanSetFromString)
	#define ASTimeSpanSet ASEXTRAROUTINE(ASExtraHFT_VERSION_6,ASTimeSpanSet)
	#define ASTimeSpanNegate ASEXTRAROUTINE(ASExtraHFT_VERSION_6,ASTimeSpanNegate)


	#define ASTextEval ASEXTRAROUTINE(ASExtraHFT_VERSION_6,ASTextEval)
	#define ASFileSysGetItemPropsAsCab ASEXTRAROUTINE(ASExtraHFT_VERSION_6,ASFileSysGetItemPropsAsCab)
	#define ASFileSysConvertCabToItemProps ASEXTRAROUTINE(ASExtraHFT_VERSION_6,ASFileSysConvertCabToItemProps)
	#define ASFileSysConvertItemPropsToCab ASEXTRAROUTINE(ASExtraHFT_VERSION_6,ASFileSysConvertItemPropsToCab)
	#define ASFileSysCanPerformOpOnItem ASEXTRAROUTINE(ASExtraHFT_VERSION_6,ASFileSysCanPerformOpOnItem)
	#define ASFileSysPerformOpOnItem ASEXTRAROUTINE(ASExtraHFT_VERSION_6,ASFileSysPerformOpOnItem)

	#define ASIsValidUTF8 ASEXTRAROUTINE(ASExtraHFT_VERSION_6,ASIsValidUTF8)
	#define ASTextCaseSensitiveCmp ASEXTRAROUTINE(ASExtraHFT_VERSION_6,ASTextCaseSensitiveCmp)

	#define ASConstCabEnum ASEXTRAROUTINE(ASExtraHFT_VERSION_6,ASConstCabEnum)

	#define ASTextFilter ASEXTRAROUTINE(ASExtraHFT_VERSION_6,ASTextFilter)

	#define ASTimeSpanGetASInt32 ASEXTRAROUTINE(ASExtraHFT_VERSION_6,ASTimeSpanGetASInt32)

	#define ASCabGetInt64 ASEXTRAROUTINE(ASExtraHFT_VERSION_7,ASCabGetInt64)
	#define ASCabPutInt64 ASEXTRAROUTINE(ASExtraHFT_VERSION_7,ASCabPutInt64)
	#define ASCabGetUns64 ASEXTRAROUTINE(ASExtraHFT_VERSION_7,ASCabGetUns64)
	#define ASCabPutUns64 ASEXTRAROUTINE(ASExtraHFT_VERSION_7,ASCabPutUns64)

	/* version 9.0 routines */

	#define ASTextMakeEmptyClear ASEXTRAROUTINE(ASExtraHFT_VERSION_9,ASTextMakeEmptyClear)
	#define ASUCS_GetPasswordFromUnicode ASEXTRAROUTINE(ASExtraHFT_VERSION_9,ASUCS_GetPasswordFromUnicode)

/* If you add, delete, or modify procs listed in this file please read the instructions at the top about how to properly change the version number */

#endif /* !STATIC_HFT */

#endif /* PI_ASEXTRA_VERSION */
#endif /* PLUGIN */

#ifdef __cplusplus
}
#endif

	/* You shouldn't call the RAW forms of these routines
	   but instead use the macros below.
	*/
    /* 
	   HPUX note: 11/28/06, see watson bug 1419712. 
	   With HPUX's aCC compiler, the templates below cause compile errors for 
	   client code if the client has PI_ASEXTRA_VERSION = 0.  The simple fix
	   of doing something like wrapping the code below in if PI_ASEXTRA_VERSION
	   doesn't work because 

	   - the internal implementation depends on these macros

	   - the conditions under which the code below should be included even on
	   clients is more complicted than just looking at PI_ASEXTRA_VERSION.

	   - it's pretty hard to test to make sure a general change in this public 
	   header hasn't broken anything on the various platforms and clients. 

	   therefore, the HPUX_ACC version will just fall back to the C-style 
	   macros. 
	*/
#if ADD_ASCAB //PKG
#if defined(__cplusplus) && !defined(HPUX_ACC)
	template <class T>
	inline T ASCabGetPointerTypeSafe(ASCab theCab, const char *keyName, const char *expectedType)
	{
#if !defined(__GNUG__) && !defined(_xlC__)
		return reinterpret_cast<T>(ASCabGetPointerRaw(theCab, keyName, expectedType));
#else
		return (T)(ASCabGetPointerRaw(theCab, keyName, expectedType));
#endif
	}
	#define ASCabGetPointer(theCab, theKey, theType) \
		ASCabGetPointerTypeSafe<theType>(theCab, theKey, #theType)
	
	template <class T>
	inline void ASCabPutPointerTypeSafe(ASCab theCab, const char *keyName, const char *theType,
										T ptrValue, ASCabPointerDestroyProc destroyProc)
	{
		ASCabPutPointerRaw(theCab, keyName, theType, (void *) ptrValue, destroyProc);
	}
	#define ASCabPutPointer(theCab, theKey, theType, thePtr, destroyProc) \
		ASCabPutPointerTypeSafe<theType>(theCab, theKey, #theType, thePtr, destroyProc)
	
	template <class T>
	inline T ASCabDetachPointerTypeSafe(ASCab theCab, const char *keyName, const char *expectedType, 
										ASBool *isFree)
	{
		return reinterpret_cast<T>(ASCabDetachPointerRaw(theCab, keyName, expectedType, isFree));
	}
	#define ASCabDetachPointer(theCab, theKey, theType, noRefs) ((theType) \
		ASCabDetachPointerTypeSafe<theType>(theCab, theKey, #theType, noRefs))
	
#else
	#define ASCabGetPointer(theCab, theKey, theType) \
		((theType) ASCabGetPointerRaw((theCab), (theKey), #theType))
	#define ASCabPutPointer(theCab, theKey, theType, thePtr, destroyProc) \
		ASCabPutPointerRaw((theCab), (theKey), #theType, (thePtr), (destroyProc))
	#define ASCabDetachPointer(theCab, theKey, theType, noRefs) ((theType) \
		ASCabDetachPointerRaw((theCab), (theKey), #theType, (noRefs)))
#endif /* __cplusplus */
#endif //ADD_ASCAB //PKG

#endif /* _H_ASExtraCalls */
