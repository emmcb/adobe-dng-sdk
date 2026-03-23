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

 ASCalls.h

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

#ifndef _H_ASCalls
#define _H_ASCalls
#include "acroassert.h"
#ifdef THREAD_SAFE_PDFL
#include "PDFLInitCommon.h"
#endif /* defined THREAD_SAFE_PDFL */

/* for Adobe use only */
#define _ASCallsHFT_LATEST_VERSION 0x000A0000
#define _ASCallsHFT_LAST_BETA_COMPATIBLE_VERSION 0x000A0000
#define _ASCallsHFT_IS_BETA 0

/* for public use */
#define ASCallsHFT_LATEST_VERSION (_ASCallsHFT_IS_BETA ? (kHFT_IN_BETA_FLAG | _ASCallsHFT_LATEST_VERSION) : _ASCallsHFT_LATEST_VERSION)

#define ASCallsHFT_VERSION_2   0x00020000
#define ASCallsHFT_VERSION_2_2 0x00020002
#define ASCallsHFT_VERSION_4   0x00040000
#define ASCallsHFT_VERSION_5   0x00050000
#define ASCallsHFT_VERSION_6   0x00060000
#define ASCallsHFT_VERSION_7   0x00070000
#define ASCallsHFT_VERSION_8   0x00080000
#define ASCallsHFT_VERSION_9   0x00090000
#define ASCallsHFT_VERSION_10  ASCallsHFT_LATEST_VERSION

#include "ASExpT.h"
#include "CorCalls.h"		/* For ASCallbackCreateProto */

#ifdef __cplusplus
extern "C" {
#endif

#ifdef NPROC /* may be already defined */
#undef NPROC
#endif

#if !PLUGIN
	/* Static link */

	/* These functions are exported with different names */
	#define ASFileSysCopyPath ASFileSysCopyPathName
	#define ASFileSysReleasePath ASFileSysReleasePathName
	#define ASFileSysRemoveFile ASFileSysRemove
	#define ASGetSecs ASSecs

	#define NPROC(returnType, name, params)		\
		ACEX1 returnType ACEX2 name params;
	#define SPROC(returnType, name, params, stubProc) \
		extern ACEX1 returnType ACEX2 name params;
	
	#include "ASProcs.h"

	#undef NPROC
	#undef SPROC

#endif /* !PLUGIN */

#if PLUGIN
	/* HFT version */
	#include "PIRequir.h"
	
	/* Enumerate the selectors */
	#define NPROC(returnType, name, params)			\
		name##SEL,
	#define SPROC(returnType, name, params, stubProc) \
		name##SEL,
	
	enum {
		ASBAD_SELECTOR,
		#include "ASProcs.h"
		ASNUMSELECTORSplusOne
	};
	
	#define ASNUMSELECTORS (ASNUMSELECTORSplusOne - 1)
	
	/* Create the prototypes */
	#undef NPROC
   #undef SPROC	
	#define NPROC(returnType, name, params)		\
		typedef ACCBPROTO1 returnType (ACCBPROTO2 *name##SELPROTO)params;
	#define SPROC(returnType, name, params, stubProc) \
		typedef ACCBPROTO1 returnType (ACCBPROTO2 *name##SELPROTO)params;
	
		#include "ASProcs.h"
	#undef NPROC
   #undef SPROC

#if PI_ACROSUPPORT_VERSION != 0
#ifdef THREAD_SAFE_PDFL
	#define gAcroSupportHFT (GetHFTLocations()->acroSupportHFT)
	#define gAcroSupportVersion (GetHFTLocations()->acroSupportVersion)
#else
	extern HFT gAcroSupportHFT;
	extern ASVersion gAcroSupportVersion;
#endif

#if !STATIC_HFT
#define ASSERT_AS_VER(X) (ACROASSERT(gAcroSupportVersion>=X))

/* ASmalloc */
#define ASmalloc (ASSERT_AS_VER(0),*((ASmallocSELPROTO)(gAcroSupportHFT[ASmallocSEL])))

/* ASrealloc*/
#define ASrealloc (ASSERT_AS_VER(0),*((ASreallocSELPROTO)(gAcroSupportHFT[ASreallocSEL])))

/* ASfree */
#define ASfree (ASSERT_AS_VER(0),*((ASfreeSELPROTO)(gAcroSupportHFT[ASfreeSEL])))


/* ASGetErrorString */
#define ASGetErrorString (ASSERT_AS_VER(0),*((ASGetErrorStringSELPROTO)(gAcroSupportHFT[ASGetErrorStringSEL])))

/* ASRegisterErrorString */
#define ASRegisterErrorString (ASSERT_AS_VER(0),*((ASRegisterErrorStringSELPROTO)(gAcroSupportHFT[ASRegisterErrorStringSEL])))


/* HFTServerNew */
#define HFTServerNew (ASSERT_AS_VER(0),*((HFTServerNewSELPROTO)(gAcroSupportHFT[HFTServerNewSEL])))

/* HFTServerDestroy */
#define HFTServerDestroy (ASSERT_AS_VER(0),*((HFTServerDestroySELPROTO)(gAcroSupportHFT[ HFTServerDestroySEL])))


/* HFTNew */
#define HFTNew (ASSERT_AS_VER(0),*((HFTNewSELPROTO)(gAcroSupportHFT[ HFTNewSEL])))

/* HFTDestroy */
#define HFTDestroy (ASSERT_AS_VER(0),*((HFTDestroySELPROTO)(gAcroSupportHFT[HFTDestroySEL])))

/* HFTReplaceEntry
** Replace the entry in the given HFTEntry for the given selector with the newEntry.
** Use ASCallbackCreateReplacement to create newEntry.  Currently, only the lowest
** bit of "flags" is meaningful:  it is HFTEntryReplaceable if the new entry is to
** be, like the old one, replaceable, or 0 if you want this replacement to be
** unreplaceable.  Usually you should use HFTEntryReplaceable.
*/
#define HFTReplaceEntry (ASSERT_AS_VER(0),*((HFTReplaceEntrySELPROTO)(gAcroSupportHFT[HFTReplaceEntrySEL])))

/* HFTGetReplacedEntry
** Retrieves the HFTEntry that newEntry replaced.  Make sure newEntry is the value
** that you passed into HFTReplaceEntry as "newEntry"--in other words, the ASCallback
** you created, not a pointer to a function you passed into ASCallbackCreate().
*/
#define HFTGetReplacedEntry (*((HFTGetReplacedEntrySELPROTO)(gAcroSupportHFT[HFTGetReplacedEntrySEL])))

#if ADD_ASFIXED //PKG
/* ASFixedMul */
#define ASFixedMul (ASSERT_AS_VER(0),*((ASFixedMulSELPROTO)(gAcroSupportHFT[ASFixedMulSEL])))

/* ASFixedDiv */
#define ASFixedDiv (ASSERT_AS_VER(0),*((ASFixedDivSELPROTO)(gAcroSupportHFT[ASFixedDivSEL])))

/* ASFixedToCString */
#define ASFixedToCString (ASSERT_AS_VER(0),*((ASFixedToCStringSELPROTO)(gAcroSupportHFT[ASFixedToCStringSEL])))

/* ASCStringToFixed */
#define ASCStringToFixed (ASSERT_AS_VER(0),*((ASCStringToFixedSELPROTO)(gAcroSupportHFT[ASCStringToFixedSEL])))


/* ASFixedMatrixConcat */
#define ASFixedMatrixConcat (ASSERT_AS_VER(0),*((ASFixedMatrixConcatSELPROTO)(gAcroSupportHFT[ASFixedMatrixConcatSEL])))

/* ASFixedMatrixInvert */
#define ASFixedMatrixInvert (ASSERT_AS_VER(0),*((ASFixedMatrixInvertSELPROTO)(gAcroSupportHFT[ASFixedMatrixInvertSEL])))

/* ASFixedMatrixTransform */
#define ASFixedMatrixTransform (ASSERT_AS_VER(0),*((ASFixedMatrixTransformSELPROTO)(gAcroSupportHFT[ASFixedMatrixTransformSEL])))

/* ASFixedMatrixTransformRect */
#define ASFixedMatrixTransformRect (ASSERT_AS_VER(0),*((ASFixedMatrixTransformRectSELPROTO)(gAcroSupportHFT[ASFixedMatrixTransformRectSEL])))
#endif //ADD_ASFIXED //PKG


/* ASPathFromPlatformPath */
#define ASPathFromPlatformPath (ASSERT_AS_VER(0),*((ASPathFromPlatformPathSELPROTO)(gAcroSupportHFT[ASPathFromPlatformPathSEL])))


/* ASGetDefaultFileSys */
#define ASGetDefaultFileSys (ASSERT_AS_VER(0),*((ASGetDefaultFileSysSELPROTO)(gAcroSupportHFT[ASGetDefaultFileSysSEL])))

/* ASGetTempFileSys */
#define ASGetTempFileSys (*((ASGetTempFileSysSELPROTO)(gAcroSupportHFT[ASGetTempFileSysSEL])))

/* ASSetTempFileSys */
#define ASSetTempFileSys (*((ASSetTempFileSysSELPROTO)(gAcroSupportHFT[ASSetTempFileSysSEL])))

/* ASGetRamFileSys */
#define ASGetRamFileSys (*((ASGetRamFileSysSELPROTO)(gAcroSupportHFT[ASGetRamFileSysSEL])))

/* ASRamFileSysSetLimitKB */
#define ASRamFileSysSetLimitKB (ASSERT_AS_VER(ASCallsHFT_VERSION_7),*((ASRamFileSysSetLimitKBSELPROTO)(gAcroSupportHFT[ASRamFileSysSetLimitKBSEL])))

/* ASFileSysDIPathFromPath */
#define ASFileSysDIPathFromPath (ASSERT_AS_VER(0),*((ASFileSysDIPathFromPathSELPROTO)(gAcroSupportHFT[ASFileSysDIPathFromPathSEL])))

/* ASFileSysPathFromDIPath */
#define ASFileSysPathFromDIPath (ASSERT_AS_VER(0),*((ASFileSysPathFromDIPathSELPROTO)(gAcroSupportHFT[ASFileSysPathFromDIPathSEL])))

/* ASFileSysCopyPath */
#define ASFileSysCopyPath (ASSERT_AS_VER(0),*((ASFileSysCopyPathSELPROTO)(gAcroSupportHFT[ASFileSysCopyPathSEL])))

/* ASFileSysReleasePath */
#define ASFileSysReleasePath (ASSERT_AS_VER(0),*((ASFileSysReleasePathSELPROTO)(gAcroSupportHFT[ASFileSysReleasePathSEL])))

/* ASFileSysOpenFile */
#define ASFileSysOpenFile (ASSERT_AS_VER(0),*((ASFileSysOpenFileSELPROTO)(gAcroSupportHFT[ASFileSysOpenFileSEL])))

/* ASFileSysRemoveFile */
#define ASFileSysRemoveFile (ASSERT_AS_VER(0),*((ASFileSysRemoveFileSELPROTO)(gAcroSupportHFT[ASFileSysRemoveFileSEL])))

/* ASFileReopen */
#define ASFileReopen (ASSERT_AS_VER(0),*((ASFileReopenSELPROTO)(gAcroSupportHFT[ASFileReopenSEL])))

/* ASFileClose */
#define ASFileClose (ASSERT_AS_VER(0),*((ASFileCloseSELPROTO)(gAcroSupportHFT[ASFileCloseSEL])))

/* ASFileSetPos */
#define ASFileSetPos (ASSERT_AS_VER(0),*((ASFileSetPosSELPROTO)(gAcroSupportHFT[ASFileSetPosSEL])))

/* ASFileGetPos */
#define ASFileGetPos (ASSERT_AS_VER(0),*((ASFileGetPosSELPROTO)(gAcroSupportHFT[ASFileGetPosSEL])))

/* ASFileSetEOF */
#define ASFileSetEOF (ASSERT_AS_VER(0),*((ASFileSetEOFSELPROTO)(gAcroSupportHFT[ASFileSetEOFSEL])))

/* ASFileGetEOF */
#define ASFileGetEOF (ASSERT_AS_VER(0),*((ASFileGetEOFSELPROTO)(gAcroSupportHFT[ASFileGetEOFSEL])))

/* ASFileRead */
#define ASFileRead (ASSERT_AS_VER(0),*((ASFileReadSELPROTO)(gAcroSupportHFT[ASFileReadSEL])))

/* ASFileWrite */
#define ASFileWrite (ASSERT_AS_VER(0),*((ASFileWriteSELPROTO)(gAcroSupportHFT[ASFileWriteSEL])))

/* ASFileFlush */
#define ASFileFlush (ASSERT_AS_VER(0),*((ASFileFlushSELPROTO)(gAcroSupportHFT[ASFileFlushSEL])))

/* ASFileAcquirePathName */
#define ASFileAcquirePathName (ASSERT_AS_VER(0),*((ASFileAcquirePathNameSELPROTO)(gAcroSupportHFT[ASFileAcquirePathNameSEL])))

/* ASFileGetFileSys */
#define ASFileGetFileSys (ASSERT_AS_VER(0),*((ASFileGetFileSysSELPROTO)(gAcroSupportHFT[ASFileGetFileSysSEL])))

/* ASFileStmRdOpen */
#define ASFileStmRdOpen (ASSERT_AS_VER(0),*((ASFileStmRdOpenSELPROTO)(gAcroSupportHFT[ASFileStmRdOpenSEL])))

/* ASMemStmRdOpen */
#define ASMemStmRdOpen (ASSERT_AS_VER(0),*((ASMemStmRdOpenSELPROTO)(gAcroSupportHFT[ASMemStmRdOpenSEL])))

/* ASProcStmRdOpen */
#define ASProcStmRdOpen (ASSERT_AS_VER(0),*((ASProcStmRdOpenSELPROTO)(gAcroSupportHFT[ASProcStmRdOpenSEL])))

/* ASStmRead */
#define ASStmRead (ASSERT_AS_VER(0),*((ASStmReadSELPROTO)(gAcroSupportHFT[ASStmReadSEL])))

/* ASStmWrite */
#define ASStmWrite (ASSERT_AS_VER(0),*((ASStmWriteSELPROTO)(gAcroSupportHFT[ASStmWriteSEL])))

/* ASStmClose */
#define ASStmClose (ASSERT_AS_VER(0),*((ASStmCloseSELPROTO)(gAcroSupportHFT[ ASStmCloseSEL])))

/* PI_ACROSUPPORT_VERSION >= 0x00020002 */

/* ASFileUnregisterFileSys */
#define ASFileUnregisterFileSys (ASSERT_AS_VER(ASCallsHFT_VERSION_2_2),*((ASFileUnregisterFileSysSELPROTO)(gAcroSupportHFT[ASFileUnregisterFileSysSEL])))

/* ASFilePushData */
#define ASFilePushData (ASSERT_AS_VER(ASCallsHFT_VERSION_2_2),*((ASFilePushDataSELPROTO)(gAcroSupportHFT[ASFilePushDataSEL])))

/* ASFileRegisterFileSys */
#define ASFileRegisterFileSys (ASSERT_AS_VER(ASCallsHFT_VERSION_2_2),*((ASFileRegisterFileSysSELPROTO)(gAcroSupportHFT[ASFileRegisterFileSysSEL])))

/* ASFileGetFileSysByName */
#define ASFileGetFileSysByName (ASSERT_AS_VER(ASCallsHFT_VERSION_2_2),*((ASFileGetFileSysByNameSELPROTO)(gAcroSupportHFT[ASFileGetFileSysByNameSEL])))

/* ASFileFromMDFile */
#define ASFileFromMDFile (ASSERT_AS_VER(ASCallsHFT_VERSION_2_2),*((ASFileFromMDFileSELPROTO)(gAcroSupportHFT[ASFileFromMDFileSEL])))

/* ASFileGetMDFile */
#define ASFileGetMDFile (ASSERT_AS_VER(ASCallsHFT_VERSION_2_2),*((ASFileGetMDFileSELPROTO)(gAcroSupportHFT[ASFileGetMDFileSEL])))

/* ASFileSysCreatePathName */
#define ASFileSysCreatePathName (ASSERT_AS_VER(ASCallsHFT_VERSION_2_2),*((ASFileSysCreatePathNameSELPROTO)(gAcroSupportHFT[ASFileSysCreatePathNameSEL])))

/* ASFileSysAcquireFileSysPath */
#define ASFileSysAcquireFileSysPath (ASSERT_AS_VER(ASCallsHFT_VERSION_2_2),*((ASFileSysAcquireFileSysPathSELPROTO)(gAcroSupportHFT[ASFileSysAcquireFileSysPathSEL])))

/* ASFileSetMode */
#define ASFileSetMode (ASSERT_AS_VER(ASCallsHFT_VERSION_2_2),*((ASFileSetModeSELPROTO)(gAcroSupportHFT[ASFileSetModeSEL])))

/* end PI_ACROSUPPORT_VERSION >= 0x00020002 */

/* begin PI_ACROSUPPORT_VERSION >= 0x00040000 */

/* ASFileStmWrOpen */
#define ASFileStmWrOpen (ASSERT_AS_VER(ASCallsHFT_VERSION_4),*((ASFileStmWrOpenSELPROTO)(gAcroSupportHFT[ASFileStmWrOpenSEL])))

/* ASProcStmWrOpen */
#define ASProcStmWrOpen (ASSERT_AS_VER(ASCallsHFT_VERSION_4),*((ASProcStmWrOpenSELPROTO)(gAcroSupportHFT[ASProcStmWrOpenSEL])))

/* HFTIsValid */
#define HFTIsValid (ASSERT_AS_VER(ASCallsHFT_VERSION_4),*((HFTIsValidSELPROTO)(gAcroSupportHFT[HFTIsValidSEL])))


/* end PI_ACROSUPPORT_VERSION >= 0x00040000 */

/* begin PI_ACROSUPPORT_VERSION >= 0x00050000 */

#define ASFileSysGetItemProps			(ASSERT_AS_VER(ASCallsHFT_VERSION_5),*((ASFileSysGetItemPropsSELPROTO)(gAcroSupportHFT[ASFileSysGetItemPropsSEL])))

#define ASFileSysFirstFolderItem		(ASSERT_AS_VER(ASCallsHFT_VERSION_5),*((ASFileSysFirstFolderItemSELPROTO)(gAcroSupportHFT[ASFileSysFirstFolderItemSEL])))

#define ASFileSysNextFolderItem			(ASSERT_AS_VER(ASCallsHFT_VERSION_5),*((ASFileSysNextFolderItemSELPROTO)(gAcroSupportHFT[ASFileSysNextFolderItemSEL])))

#define ASFileSysDestroyFolderIterator	(ASSERT_AS_VER(ASCallsHFT_VERSION_5),*((ASFileSysDestroyFolderIteratorSELPROTO)(gAcroSupportHFT[ASFileSysDestroyFolderIteratorSEL])))

#define ASFileSysAcquireParent			(ASSERT_AS_VER(ASCallsHFT_VERSION_5),*((ASFileSysAcquireParentSELPROTO)(gAcroSupportHFT[ASFileSysAcquireParentSEL])))

#define ASFileIsSame					(ASSERT_AS_VER(ASCallsHFT_VERSION_5),*((ASFileIsSameSELPROTO)(gAcroSupportHFT[ASFileIsSameSEL])))

#define ASFileSysGetNameFromPath		(ASSERT_AS_VER(ASCallsHFT_VERSION_5),*((ASFileSysGetNameFromPathSELPROTO)(gAcroSupportHFT[ASFileSysGetNameFromPathSEL])))

#define ASFileSysGetTempPathName		(ASSERT_AS_VER(ASCallsHFT_VERSION_5),*((ASFileSysGetTempPathNameSELPROTO)(gAcroSupportHFT[ASFileSysGetTempPathNameSEL])))

#define ASFileSysGetStorageFreeSpace	(ASSERT_AS_VER(ASCallsHFT_VERSION_5),*((ASFileSysGetStorageFreeSpaceSELPROTO)(gAcroSupportHFT[ASFileSysGetStorageFreeSpaceSEL])))
	
#define ASFileSysFlushVolume			(ASSERT_AS_VER(ASCallsHFT_VERSION_5),*((ASFileSysFlushVolumeSELPROTO)(gAcroSupportHFT[ASFileSysFlushVolumeSEL])))

#define ASFileMReadRequest				(ASSERT_AS_VER(ASCallsHFT_VERSION_5),*((ASFileMReadRequestSELPROTO)(gAcroSupportHFT[ASFileMReadRequestSEL])))

#define ASFileClearOutstandingMReads	(ASSERT_AS_VER(ASCallsHFT_VERSION_5),*((ASFileClearOutstandingMReadsSELPROTO)(gAcroSupportHFT[ASFileClearOutstandingMReadsSEL])))

#define ASFileSysURLFromPath			(ASSERT_AS_VER(ASCallsHFT_VERSION_5),*((ASFileSysURLFromPathSELPROTO)(gAcroSupportHFT[ASFileSysURLFromPathSEL])))

#define ASFileGetURL					(ASSERT_AS_VER(ASCallsHFT_VERSION_5),*((ASFileGetURLSELPROTO)(gAcroSupportHFT[ASFileGetURLSEL])))

#define ASFileSysCreateFolder			(ASSERT_AS_VER(ASCallsHFT_VERSION_5),*((ASFileSysCreateFolderSELPROTO)(gAcroSupportHFT[ASFileSysCreateFolderSEL])))

#define ASFileSysRemoveFolder			(ASSERT_AS_VER(ASCallsHFT_VERSION_5),*((ASFileSysRemoveFolderSELPROTO)(gAcroSupportHFT[ASFileSysRemoveFolderSEL])))

#define ASFileGetOpenMode				(ASSERT_AS_VER(ASCallsHFT_VERSION_5),*((ASFileGetOpenModeSELPROTO)(gAcroSupportHFT[ASFileGetOpenModeSEL])))

#define ASFileSysDisplayStringFromPath	(ASSERT_AS_VER(ASCallsHFT_VERSION_5),*((ASFileSysDisplayStringFromPathSELPROTO)(gAcroSupportHFT[ASFileSysDisplayStringFromPathSEL])))

#define ASGetSecs						(ASSERT_AS_VER(ASCallsHFT_VERSION_5),*((ASGetSecsSELPROTO)(gAcroSupportHFT[ASGetSecsSEL])))

#define ASFileSysSetTypeAndCreator		(ASSERT_AS_VER(ASCallsHFT_VERSION_5),*((ASFileSysSetTypeAndCreatorSELPROTO)(gAcroSupportHFT[ASFileSysSetTypeAndCreatorSEL])))
#define ASFileSysGetTypeAndCreator		(ASSERT_AS_VER(ASCallsHFT_VERSION_5),*((ASFileSysGetTypeAndCreatorSELPROTO)(gAcroSupportHFT[ASFileSysGetTypeAndCreatorSEL])))

#define ASHostMBLen		(ASSERT_AS_VER(ASCallsHFT_VERSION_5),*((ASHostMBLenSELPROTO)(gAcroSupportHFT[ASHostMBLenSEL])))

#define ASFileHardFlush (ASSERT_AS_VER(ASCallsHFT_VERSION_5),*((ASFileHardFlushSELPROTO)(gAcroSupportHFT[ASFileHardFlushSEL])))

#define HFTReplaceEntryEx (ASSERT_AS_VER(ASCallsHFT_VERSION_5),*((HFTReplaceEntryExSELPROTO)(gAcroSupportHFT[HFTReplaceEntryExSEL])))

#define HFTUnreplaceEntry (ASSERT_AS_VER(ASCallsHFT_VERSION_5),*((HFTUnreplaceEntrySELPROTO)(gAcroSupportHFT[HFTUnreplaceEntrySEL])))

/* end PI_ACROSUPPORT_VERSION >= 0x00050000 */

/* begin PI_ACROSUPPORT_VERSION >= 0x00060000 */

#define ASProcStmRdOpenEx (ASSERT_AS_VER(ASCallsHFT_VERSION_6),*((ASProcStmRdOpenExSELPROTO)(gAcroSupportHFT[ASProcStmRdOpenExSEL])))
#define ASUUIDGenUnique (ASSERT_AS_VER(ASCallsHFT_VERSION_6),*((ASUUIDGenUniqueSELPROTO)(gAcroSupportHFT[ASUUIDGenUniqueSEL])))
#define ASUUIDGenFromName (ASSERT_AS_VER(ASCallsHFT_VERSION_6),*((ASUUIDGenFromNameSELPROTO)(gAcroSupportHFT[ASUUIDGenFromNameSEL])))
#define ASUUIDGenFromHash (ASSERT_AS_VER(ASCallsHFT_VERSION_6),*((ASUUIDGenFromHashSELPROTO)(gAcroSupportHFT[ASUUIDGenFromHashSEL])))
#define ASUUIDFromCString (ASSERT_AS_VER(ASCallsHFT_VERSION_6),*((ASUUIDFromCStringSELPROTO)(gAcroSupportHFT[ASUUIDFromCStringSEL])))
#define ASUUIDToCString (ASSERT_AS_VER(ASCallsHFT_VERSION_6),*((ASUUIDToCStringSELPROTO)(gAcroSupportHFT[ASUUIDToCStringSEL])))
#define ASFileSysGetPlatformThing (ASSERT_AS_VER(ASCallsHFT_VERSION_6),*((ASFileSysGetPlatformThingSELPROTO)(gAcroSupportHFT[ASFileSysGetPlatformThingSEL])))
#define ASFileSysAcquirePlatformPath (ASSERT_AS_VER(ASCallsHFT_VERSION_6),*((ASFileSysAcquirePlatformPathSELPROTO)(gAcroSupportHFT[ASFileSysAcquirePlatformPathSEL])))
#define ASFileSysReleasePlatformPath (ASSERT_AS_VER(ASCallsHFT_VERSION_6),*((ASFileSysReleasePlatformPathSELPROTO)(gAcroSupportHFT[ASFileSysReleasePlatformPathSEL])))
#define ASPlatformPathGetCstringPtr (ASSERT_AS_VER(ASCallsHFT_VERSION_6),*((ASPlatformPathGetCstringPtrSELPROTO)(gAcroSupportHFT[ASPlatformPathGetCstringPtrSEL])))
#if  !MAC_PLATFORM || (MAC_PLATFORM && !AS_ARCH_64BIT)
#define ASPlatformPathGetFSSpecPtr (ASSERT_AS_VER(ASCallsHFT_VERSION_6),*((ASPlatformPathGetFSSpecPtrSELPROTO)(gAcroSupportHFT[ASPlatformPathGetFSSpecPtrSEL])))
#endif
#define ASPlatformPathGetFSRefPtr (ASSERT_AS_VER(ASCallsHFT_VERSION_6),*((ASPlatformPathGetFSRefPtrSELPROTO)(gAcroSupportHFT[ASPlatformPathGetFSRefPtrSEL])))
#define ASPlatformPathGetFSRefWithCFStringRefRecPtr (ASSERT_AS_VER(ASCallsHFT_VERSION_6),*((ASPlatformPathGetFSRefWithCFStringRefRecPtrSELPROTO)(gAcroSupportHFT[ASPlatformPathGetFSRefWithCFStringRefRecPtrSEL])))
#define ASPlatformPathGetCFURLRefRecPtr (ASSERT_AS_VER(ASCallsHFT_VERSION_6),*((ASPlatformPathGetCFURLRefRecPtrSELPROTO)(gAcroSupportHFT[ASPlatformPathGetCFURLRefRecPtrSEL])))
#define ASPlatformPathGetPOSIXPathPtr (ASSERT_AS_VER(ASCallsHFT_VERSION_6),*((ASPlatformPathGetPOSIXPathPtrSELPROTO)(gAcroSupportHFT[ASPlatformPathGetPOSIXPathPtrSEL])))
#define ASFileSysGetNameFromPathAsASText (ASSERT_AS_VER(ASCallsHFT_VERSION_6),*((ASFileSysGetNameFromPathAsASTextSELPROTO)(gAcroSupportHFT[ASFileSysGetNameFromPathAsASTextSEL])))
#define ASFileSysDisplayASTextFromPath (ASSERT_AS_VER(ASCallsHFT_VERSION_6),*((ASFileSysDisplayASTextFromPathSELPROTO)(gAcroSupportHFT[ASFileSysDisplayASTextFromPathSEL])))

#define ASStmFlush (ASSERT_AS_VER(ASCallsHFT_VERSION_6),*((ASStmFlushSELPROTO)(gAcroSupportHFT[ASStmFlushSEL])))
#define ASFileHasOutstandingMReads (ASSERT_AS_VER(ASCallsHFT_VERSION_6),*((ASFileHasOutstandingMReadsSELPROTO)(gAcroSupportHFT[ASFileHasOutstandingMReadsSEL])))
#define ASFileCanSetEOF (ASSERT_AS_VER(ASCallsHFT_VERSION_6),*((ASFileCanSetEOFSELPROTO)(gAcroSupportHFT[ASFileCanSetEOFSEL])))
#define HFTGetVersion (ASSERT_AS_VER(ASCallsHFT_VERSION_6),*((HFTGetVersionSELPROTO)(gAcroSupportHFT[HFTGetVersionSEL])))
#define HFTNewEx (ASSERT_AS_VER(ASCallsHFT_VERSION_6),*((HFTNewExSELPROTO)(gAcroSupportHFT[HFTNewExSEL])))
#define ASFileSysDIPathFromPathEx (ASSERT_AS_VER(ASCallsHFT_VERSION_6),*((ASFileSysDIPathFromPathExSELPROTO)(gAcroSupportHFT[ASFileSysDIPathFromPathExSEL])))
#define ASFileSysPathFromDIPathEx (ASSERT_AS_VER(ASCallsHFT_VERSION_6),*((ASFileSysPathFromDIPathExSELPROTO)(gAcroSupportHFT[ASFileSysPathFromDIPathExSEL])))
/* end PI_ACROSUPPORT_VERSION >= 0x00060000 */

/* begin PI_ACROSUPPORT_VERSION >= 0x00070000 */
#if PI_ACROSUPPORT_VERSION >= 0x00070000
#if ADD_ASFIXED //PKG
#define ASFixedToFloat (ASSERT_AS_VER(ASCallsHFT_VERSION_7),*((ASFixedToFloatSELPROTO)(gAcroSupportHFT[ASFixedToFloatSEL])))
#define FloatToASFixed (ASSERT_AS_VER(ASCallsHFT_VERSION_7),*((FloatToASFixedSELPROTO)(gAcroSupportHFT[FloatToASFixedSEL])))
#endif
#else
/* Obsolete macros. Switch to HFT instead */
#define FloatToASFixed(d) ((ASFixed) ((d) * fixedOne + 0.5))
#define ASFixedToFloat(f) ((float) ((f) / (double) fixedOne))
#endif

#define ASFileSysOpenFile64 (ASSERT_AS_VER(ASCallsHFT_VERSION_7),*((ASFileSysOpenFile64SELPROTO)(gAcroSupportHFT[ASFileSysOpenFile64SEL])))
#define ASFileSysGetFilePosLimit (ASSERT_AS_VER(ASCallsHFT_VERSION_7),*((ASFileSysGetFilePosLimitSELPROTO)(gAcroSupportHFT[ASFileSysGetFilePosLimitSEL])))
#define ASFileSetPos64 (ASSERT_AS_VER(ASCallsHFT_VERSION_7),*((ASFileSetPos64SELPROTO)(gAcroSupportHFT[ASFileSetPos64SEL])))
#define ASFileGetPos64 (ASSERT_AS_VER(ASCallsHFT_VERSION_7),*((ASFileGetPos64SELPROTO)(gAcroSupportHFT[ASFileGetPos64SEL])))
#define ASFileSetEOF64 (ASSERT_AS_VER(ASCallsHFT_VERSION_7),*((ASFileSetEOF64SELPROTO)(gAcroSupportHFT[ASFileSetEOF64SEL])))
#define ASFileGetEOF64 (ASSERT_AS_VER(ASCallsHFT_VERSION_7),*((ASFileGetEOF64SELPROTO)(gAcroSupportHFT[ASFileGetEOF64SEL])))

#define ASFileSysGetNameFromPathForDisplay (ASSERT_AS_VER(ASCallsHFT_VERSION_7),*((ASFileSysGetNameFromPathForDisplaySELPROTO)(gAcroSupportHFT[ASFileSysGetNameFromPathForDisplaySEL])))

/* end PI_ACROSUPPORT_VERSION >= 0x00070000 */

/* begin PI_ACROSUPPORT_VERSION >= 0x00080000 */
#define ASGetDefaultUnicodeFileSys (ASSERT_AS_VER(ASCallsHFT_VERSION_8),*((ASGetDefaultUnicodeFileSysSELPROTO)(gAcroSupportHFT[ASGetDefaultUnicodeFileSysSEL])))
#define ASGetErrorStringASText (ASSERT_AS_VER(ASCallsHFT_VERSION_8),*((ASGetErrorStringASTextSELPROTO)(gAcroSupportHFT[ASGetErrorStringASTextSEL])))
#define ASRegisterErrorStringASText (ASSERT_AS_VER(ASCallsHFT_VERSION_8),*((ASRegisterErrorStringASTextSELPROTO)(gAcroSupportHFT[ASRegisterErrorStringASTextSEL])))
#define ASGetDefaultFileSysForPath (ASSERT_AS_VER(ASCallsHFT_VERSION_8),*((ASGetDefaultFileSysForPathSELPROTO)(gAcroSupportHFT[ASGetDefaultFileSysForPathSEL])))
#define ASFileSysIsLocal (ASSERT_AS_VER(ASCallsHFT_VERSION_8),*((ASFileSysIsLocalSELPROTO)(gAcroSupportHFT[ASFileSysIsLocalSEL])))
#define ASFileSysGetStorageFreeSpace64 (ASSERT_AS_VER(ASCallsHFT_VERSION_8),*((ASFileSysGetStorageFreeSpace64SELPROTO)(gAcroSupportHFT[ASFileSysGetStorageFreeSpace64SEL])))
/* end PI_ACROSUPPORT_VERSION >= 0x00080000 */
	
/* begin PI_ACROSUPPORT_VERSION >= 0x000A0000 */
#define ASDoubleMatrixConcat (ASSERT_AS_VER(ASCallsHFT_VERSION_10),*((ASDoubleMatrixConcatSELPROTO)(gAcroSupportHFT[ASDoubleMatrixConcatSEL])))
#define ASDoubleMatrixInvert (ASSERT_AS_VER(ASCallsHFT_VERSION_10),*((ASDoubleMatrixInvertSELPROTO)(gAcroSupportHFT[ASDoubleMatrixInvertSEL])))
#define ASDoubleMatrixTransform (ASSERT_AS_VER(ASCallsHFT_VERSION_10),*((ASDoubleMatrixTransformSELPROTO)(gAcroSupportHFT[ASDoubleMatrixTransformSEL])))
#define ASDoubleMatrixTransformRect (ASSERT_AS_VER(ASCallsHFT_VERSION_10),*((ASDoubleMatrixTransformRectSELPROTO)(gAcroSupportHFT[ASDoubleMatrixTransformRectSEL])))
#define ASFileOpenWithVirtualEOF (ASSERT_AS_VER(ASCallsHFT_VERSION_10),*((ASFileOpenWithVirtualEOFSELPROTO)(gAcroSupportHFT[ASFileOpenWithVirtualEOFSEL])))
/* end PI_ACROSUPPORT_VERSION >= 0x000A0000 */
	
#endif /* !STATIC_HFT */


/* ASCallbackCreateReplacement
** Type-checking replacement callback creation.  Will cause a compiler
** error if the proc is not of the same type as the proc it's replacing.
** sel is a Selector, not a selector name, so be sure to use the SEL suffix!
*/
#define ASCallbackCreateReplacement(sel, proc) ASCallbackCreateProto(sel##PROTO, proc)

#define ASFloatToFixed FloatToASFixed /* For compatibility with older name */

/*
** HFTCallReplacedEntry
** Convenience macro for calling a replaced entry in the HFT.
** sel is a Selector, not a selector name, so be sure to use the SEL suffix!
*/
#define HFTCallReplacedEntry(hft, sel, oldProc)  (*((sel##PROTO)(HFTGetReplacedEntry((hft), sel, (oldProc)))))

#endif /* PI_ACROSUPPORT_VERSION != 0 */

#endif /* PLUGIN */

#ifdef __cplusplus
}
#endif

#endif /* !defined(_H_ASCalls) */
