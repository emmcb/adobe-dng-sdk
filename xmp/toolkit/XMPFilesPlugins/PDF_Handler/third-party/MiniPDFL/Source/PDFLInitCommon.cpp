/*********************************************************************

 ADOBE SYSTEMS INCORPORATED
 Copyright (C) 1994-2006 Adobe Systems Incorporated
 All rights reserved.

 NOTICE: Adobe permits you to use, modify, and distribute this file
 in accordance with the terms of the Adobe license agreement
 accompanying it. If you have received this file from a source other
 than Adobe, then your use, modification, or distribution of it
 requires the prior written permission of Adobe.

*********************************************************************/

#include "Environ.h"
#include <assert.h>

#if MAC_PLATFORM && !defined(__cplusplus)
#error This file needs to be compiled as C++ on Macintosh  
#elif UNIX_PLATFORM && !defined(__cplusplus)
#error This file needs to be compiled as C++ on Unix
#endif

#if MAC_PLATFORM
//#include <CoreServices/CoreServices.h>
#endif	/* MAC_PLATFORM */

#include "CorCalls.h"

#if USE_CPLUSPLUS_EXCEPTIONS_FOR_ASEXCEPTIONS

#ifdef __cplusplus
extern "C" {
#endif

#if 1 //UNIX_PLATFORM
/* miThrowExceptionToHandler is inlined in CorCalls.h on UNIX_PLATFORMS */
#else
void miThrowExceptionToHandler(void *asEnviron)
{
    /* serre, 11/7/06: outer parentheses were removed from the throw
       expression because they cause a compile error with gcc 3.2 on solaris. 
       Internal builds use the -fpermissive compiler parameter to avoid the 
       compile error, but it seems better to just remove the parentheses to 
       avoid client problems.  */
	throw _miExceptionInfo();
}
#endif

#ifdef __cplusplus
}
#endif

#endif /* if USE_CPLUSPLUS_EXCEPTIONS_FOR_ASEXCEPTIONS */

ACCB1 void ACCB2 RestorePlugInFrame(void *asEnviron)
{
	ACROlongjmp(*(ACROjmp_buf *)asEnviron, 1);
}

/* The following code is only required for HFT access to the library */
#ifdef PDFL_EXTENSION

#include "PIRequir.h"
#include "PDFLRequir.h"
#include "PDFInit.h"
#include "PDFLInitCommon.h"
/* get version number ids {*/
#include "ASExtraCalls.h"
#include "ASCalls.h"
#include "CosCalls.h"
#include "PDCalls.h"
#include "PERCalls.h"
#include "PEWCalls.h"
#include "PagePDECntCalls.h"

/* get version number ids }*/

/* This is for testing only.  Leave it set of 0xFFFFFFFF for final version */
#define TEST_OLD_VERSION 0xFFFFFFFF /*set lower to test old versions.  set to 0x00050000 to simulate acrobat 5.0*/

#if DEBUG
/* For special DURING/HANDLER catching */
int gBadReturnCatcher;
#endif

// Normally gHFTAddrs would be a static, but GoLive is using it to provide PDFL linkage to subsidary plugins to
// they don't have to link to PDFL.lib directly. They had developed this linkage previously before THREAD_SAFT_PDFL
// was defined and accesss the gXXXHFT globals directly. See bug #1116586.
#ifdef THREAD_SAFE_PDFL
ThreadLocalKey gHFTAddrs = 0;

#if WIN_ENV
#define ThreadLocalStorageGet() TlsGetValue(gHFTAddrs)
#elif UNIX_PLATFORM || IOS || MAC_PLATFORM
#include <pthread.h>
#define ThreadLocalStorageGet() pthread_getspecific(gHFTAddrs)
#else
#error "Platform not supported!"
#endif

PHFTLocations GetHFTLocations()
{

    return ((PHFTLocations) ThreadLocalStorageGet());

}

#else /* defined THREAD_SAFE_PDFL */

/* For the PDF Library, additional library specific HFTs are available. */

HFT gCoreHFT = 0;
ASUns32 gCoreVersion = 0;

HFT gAcroSupportHFT;
ASUns32 gAcroSupportVersion =0;

#if PDFL_VERSION != 0
HFT gPDFLHFT;
ASUns32 gPDFLVersion =0;
#endif

#if PI_COS_VERSION != 0
HFT gCosHFT = 0;
ASUns32 gCosVersion = 0;
#endif

#if PI_PDMODEL_VERSION != 0
HFT gPDModelHFT = 0;
ASUns32 gPDModelVersion = 0;
#endif

#if PI_PDFEDIT_READ_VERSION != 0
HFT gPDFEditReadHFT = 0;
ASUns32 gPDFEditReadVersion = 0;
#endif

#if PI_PDFEDIT_WRITE_VERSION != 0
HFT gPDFEditWriteHFT = 0;
ASUns32 gPDFEditWriteVersion = 0;
#endif

#if PI_PDSYSFONT_VERSION != 0
HFT gPDSysFontHFT = 0;
ASUns32 gPDSysFontVersion = 0;
#endif

#if PI_PAGE_PDE_CONTENT_VERSION != 0
HFT gPagePDEContentHFT = 0;
ASUns32 gPagePDEContentVersion = 0;
#endif

#if PI_PDSEDIT_WRITE_VERSION != 0
HFT gPDSWriteHFT = 0;
ASUns32 gPDSWriteVersion = 0;
#endif

#if PI_PDSEDIT_READ_VERSION != 0
HFT gPDSReadHFT = 0;
ASUns32 gPDSReadVersion = 0;
#endif

#if PI_ASEXTRA_VERSION != 0
HFT gASExtraHFT = 0;
ASUns32 gASExtraVersion = 0;
#endif

#if PI_PDMETADATA_VERSION != 0
HFT gPDMetadataHFT = 0;
ASUns32 gPDMetadataVersion = 0;
#endif

#if PI_ACROCOLOR_VERSION != 0
HFT gAcroColorHFT;
ASUns32 gAcroColorVersion = 0;
#endif
#endif /* not defined THREAD_SAFE_PDFL */

#ifdef __cplusplus
extern "C" {
#endif


/* pass in name of hft and minimum required version.  If the hft has a GetVersion routine pass
   in the version of the hft where it first appears.  Pass 0 if there is no GetVersion call.
   returns hft and version of the returned hft (>= requiredVer) and true if successful
   on failure, both resultHFT and resultingVer return as NULL
*/
ASBool GetRequestedHFT(const char* table, ASUns32 requiredVer, ASUns32 *resultingVer, HFT *resultHFT)
{
	ASAtom tablename = ASAtomFromString(table);
	ASVersion resultVer = HFT_ERROR_NO_VERSION;
	/* these are the versions of Acrobat where we did not support GetVersion of an HFT. */
	/* XXX DO NOT ADD TO THIS LIST.  If you find yourself adding to this list, stop!       XXX 
	** XXX Instead, use HFTNewEx where the HFT is created so GetVersion will work.         XXX */
	static ASUns32 versionLessVersions[] = {0x00050001, 0x00050000, 0x00040005, 0x00040000, 0x00020003, 0x00020002, 0x00020001};
	/* XXX DO NOT ADD TO THE ADOVE LIST XXX */
	static ASInt32 kNUMVERSIONS = sizeof(versionLessVersions) / sizeof(ASUns32);

	ASInt32 i;
	HFT thft = NULL; /* we use a temp hft in case we are replacing gCoreHFT */

#if !DEBUG || STRICT_HFT_VERSION_CHECKING

	if (gAcroSupportVersion >= ASCallsHFT_VERSION_6) {
		thft = ASExtensionMgrGetHFT(tablename, requiredVer); 
		if (thft) {
			resultVer = HFTGetVersion(thft);
			assert(resultVer != HFT_ERROR_NO_VERSION); /* all Acrobat HFTs support HFTGetVersion */
		}
	}
#else
	/* XXX Definitely only for internal developers, this allows a plugin to run with any viewer, even ones with incompatible HFTs */
//#pragma message("warning: not checking HFT version numbers at runtime, application may crash if out of sync")
	/* XXX This code can cause unexplained crashes due to incompatible mismatched versions. Use at own risk! */
	ASUns32 foundRev = 0x00040000;

	if (gAcroSupportVersion >= ASCallsHFT_VERSION_6) {
		thft = ASExtensionMgrGetHFT(tablename, foundRev);
		if (thft) {
			resultVer  = HFTGetVersion(thft);
			if (resultVer < requiredVer)
				resultVer = requiredVer; /* bump up returned beta version */
		}
	}
#endif
	if (resultVer == HFT_ERROR_NO_VERSION){
		/* without a GetVersion version of the HFT, we must try all versions from latest on down and see what we get */
		assert(kNUMVERSIONS == 7); /* XXX read comments above! XXX this assert can be removed, along with the comments above, just before Newport SDK ships. */
		for (i=0;i<kNUMVERSIONS;i++) {
			if (versionLessVersions[i]<requiredVer)
				break;
			thft = ASExtensionMgrGetHFT(tablename, versionLessVersions[i]);
			if (thft)
				break;
		}
		if (thft) 
			resultVer = versionLessVersions[i];
		else 
			resultVer = 0;
	}
	*resultHFT = thft;
	*resultingVer = resultVer;
	if (TEST_OLD_VERSION < 0xFFFFFFFF && *resultingVer > TEST_OLD_VERSION)
		*resultingVer = TEST_OLD_VERSION;
	if (*resultingVer ==0) 
	{
#if WIN_PLATFORM
		char err[255];
		wsprintfA(err,"failed on %s version %s%x\n",table,(requiredVer & kHFT_IN_BETA_FLAG) ?"(beta)":"",requiredVer & ~kHFT_IN_BETA_FLAG);
		OutputDebugStringA(err);
#endif
	}

	return *resultingVer != 0;
}

/* 
** This routine is called by the host application to set up the plug-in's SDK-provided functionality.
*/

ASInt32 PDFLInitCommon(PDFLData data, InitProcsP procs, os_size_t sizeHFTLocTab)
{
	
	ASBool bSuccess;
	ASInt32 errorCode;
	
	if ( (errorCode = (*procs->initProc)(data)) != 0 )
		return errorCode;
	errorCode = ERR_GENERAL;

#ifdef THREAD_SAFE_PDFL
	/* 64-bit cast to quiet compiler warning about size_t -> ASUns32 possible loss of data */
	if ( (*procs->initThreadLocalData)(&gHFTAddrs, (ASUns32)sizeHFTLocTab) == NULL )
		return errorCode;
#endif /* defined THREAD_SAFE_PDFL */
	
	/* Get our globals out */
	gCoreHFT = (*procs->getCoreHFTProc)();
	gCoreVersion = 0x00020000; /* lowest version that supports v0200 handshake */
	
							   /*
							   ** Note that we just got the Core HFT, so now we can, and are expected to, ASCallbackCreate()
							   ** every function we pass back to the viewer.
							   ** We can now also call functions in the Core HFT (level 2), such as ASExtensionMgrGetHFT().
	*/
	/* Get the HFTs we want */
	/* this file wants AcroSupport for the HFTGetVersion call so get max(version with HFTGetVersion, version of PI) if possible */
	gAcroSupportHFT = ASExtensionMgrGetHFT(ASAtomFromString("AcroSupport"), ASCallsHFT_VERSION_6); 
	if (gAcroSupportHFT) {
		gAcroSupportVersion = ASCallsHFT_VERSION_6; //to clear assert in next call
		gAcroSupportVersion = HFTGetVersion(gAcroSupportHFT);
		bSuccess = true;
#if	PI_ACROSUPPORT_VERSION != 0
		/* got version 6, now check to see if acrosupport is also compatible with rest of plugin */
		bSuccess = GetRequestedHFT("AcroSupport",PI_ACROSUPPORT_VERSION,&gAcroSupportVersion,&gAcroSupportHFT);
#endif
	}
	else {
#if	PI_ACROSUPPORT_VERSION == 0
		bSuccess = true;
#else
		bSuccess = GetRequestedHFT("AcroSupport",PI_ACROSUPPORT_VERSION,&gAcroSupportVersion,&gAcroSupportHFT);
#endif
	}
#ifndef PI_ACROSUPPORT_OPTIONAL
	if (!bSuccess)
		return errorCode;
#endif
	
	/* we'll rerequest the CoreHFT at the plugin's required level */
#if PI_CORE_VERSION == 0
#error Define PI_CORE_VERSION to 0x00020000 or over for core HFT.  This is not an optional HFT
#else
	bSuccess = GetRequestedHFT("Core",PI_CORE_VERSION,&gCoreVersion,&gCoreHFT);
	if (!bSuccess)
		return errorCode;
#endif
	
#if PDFL_VERSION != 0
	//bSuccess = GetRequestedHFT("PDFLibrary",PDFL_VERSION,&gPDFLVersion,&gPDFLHFT);
#ifndef PDFL_OPTIONAL
	if (!bSuccess)
		return errorCode;
#endif
#endif /* PDFL_VERSION  */
	
#if PI_COS_VERSION != 0
	bSuccess = GetRequestedHFT("Cos",PI_COS_VERSION,&gCosVersion,&gCosHFT);
#ifndef PI_COS_OPTIONAL
	if (!bSuccess)
		return errorCode;
#endif
#endif /* PI_COS_VERSION  */

#if PI_PDMODEL_VERSION != 0
	bSuccess = GetRequestedHFT("PDModel",PI_PDMODEL_VERSION,&gPDModelVersion,&gPDModelHFT);
#ifndef PI_PDMODEL_OPTIONAL
	if (!bSuccess)
		return errorCode;
#endif
#endif /* PI_PDMODEL_VERSION  */
	
#if PI_PDFEDIT_WRITE_VERSION != 0
	bSuccess = GetRequestedHFT("PDFEditWrite",PI_PDFEDIT_WRITE_VERSION,&gPDFEditWriteVersion,&gPDFEditWriteHFT);
#ifndef PI_PDFEDIT_WRITE_OPTIONAL
	if (!bSuccess)
		return errorCode;
#endif
#endif /* PI_PDFEDIT_WRITE_VERSION  */
	
#if PI_PDFEDIT_READ_VERSION != 0
	bSuccess = GetRequestedHFT("PDFEditRead",PI_PDFEDIT_READ_VERSION,&gPDFEditReadVersion,&gPDFEditReadHFT);
#ifndef PI_PDFEDIT_READ_OPTIONAL
	if (!bSuccess)
		return errorCode;
#endif
#endif /* PI_PDFEDIT_READ_VERSION  */
	
#if 0 //pk	
#if PI_PDSYSFONT_VERSION != 0
	bSuccess = GetRequestedHFT("PDSysFont",PI_PDSYSFONT_VERSION,&gPDSysFontVersion,&gPDSysFontHFT);
#ifndef PI_PDSYSFONT_OPTIONAL
	if (!bSuccess)
		return errorCode;
#endif
#endif /* PI_PDSYSFONT_VERSION  */
#endif //pk
	
#if PI_PAGE_PDE_CONTENT_VERSION != 0
	bSuccess = GetRequestedHFT("PagePDEContent",PI_PAGE_PDE_CONTENT_VERSION,&gPagePDEContentVersion,&gPagePDEContentHFT);
#ifndef PI_PAGE_PDE_CONTENT_OPTIONAL
	if (!bSuccess)
		return errorCode;
#endif
#endif /* PI_PAGE_PDE_CONTENT_VERSION  */
	
#if 0 //pk	
#if PI_PDSEDIT_WRITE_VERSION != 0
	bSuccess = GetRequestedHFT("PDSWrite",PI_PDSEDIT_WRITE_VERSION,&gPDSWriteVersion,&gPDSWriteHFT);
#ifndef PI_PDSEDIT_WRITE_OPTIONAL
	if (!bSuccess)
		return errorCode;
#endif
#endif /* PI_PDSEDIT_WRITE_VERSION  */
	
#if PI_PDSEDIT_READ_VERSION != 0
	bSuccess = GetRequestedHFT("PDSRead",PI_PDSEDIT_READ_VERSION,&gPDSReadVersion,&gPDSReadHFT);
#ifndef PI_PDSEDIT_READ_OPTIONAL
	if (!bSuccess)
		return errorCode;
#endif
#endif /* PI_PDSEDIT_READ_VERSION  */
#endif //pk
	
#if PI_ASEXTRA_VERSION != 0
	bSuccess = GetRequestedHFT("ASExtra",PI_ASEXTRA_VERSION,&gASExtraVersion,&gASExtraHFT);
#ifndef PI_ASEXTRA_OPTIONAL
	if (!bSuccess)
		return errorCode;
#endif
#endif /* PI_ASEXTRA_VERSION  */
	return 0;
	
#if PI_PDMETADATA_VERSION != 0
	bSuccess = GetRequestedHFT("PDMetadata",PI_PDMETADATA_VERSION,&gPDMetadataVersion,&gPDMetadataHFT);
#ifndef PI_PDMETADATA_OPTIONAL
	if (!bSuccess)
		return errorCode;
#endif
#endif /* PI_PDMETADATA_VERSION  */
	
#if PI_ACROCOLOR_VERSION != 0
	bSuccess = GetRequestedHFT("AcroColorHFT",PI_ACROCOLOR_VERSION,&gAcroColorVersion,&gAcroColorHFT);
#ifndef PI_ACROCOLOR_OPTIONAL
	if (!bSuccess)
		return errorCode;
#endif
#endif /* PI_ACROCOLOR_VERSION  */
	
	/* Return success */
	return 0;
		
}

void PDFLTermCommon(TermProcsP termProcs)
{
	/* Terminate the PDF Library */
	if( termProcs->termProc != NULL )
	{
		(*termProcs->termProc)();
	}

#ifndef THREAD_SAFE_PDFL
	//* Set all HFTs back to zero */
#if	PI_ACROSUPPORT_VERSION != 0
	gAcroSupportHFT = 0;
#endif
	
#if PDFL_VERSION != 0
	gPDFLHFT = 0;
#endif
	
#if PI_COS_VERSION != 0
	gCosHFT = 0;
#endif
	
#if PI_PDMODEL_VERSION != 0
	gPDModelHFT = 0;
#endif
	
#if PI_PDFEDIT_READ_VERSION != 0
	gPDFEditReadHFT = 0;
#endif
	
#if PI_PDFEDIT_WRITE_VERSION != 0
	gPDFEditWriteHFT = 0;
#endif
	
#if PI_PDSYSFONT_VERSION != 0
	gPDSysFontHFT = 0;
#endif
	
#if PI_PAGE_PDE_CONTENT_VERSION != 0
	gPagePDEContentHFT = 0;
#endif
	
#if PI_PDSEDIT_WRITE_VERSION != 0
	gPDSWriteHFT = 0;
#endif
	
#if PI_PDSEDIT_READ_VERSION != 0
	gPDSReadHFT = 0;
#endif
	
#if PI_ASEXTRA_VERSION != 0
	gASExtraHFT = 0;
#endif
	
#if PI_PDMETADATA_VERSION != 0
	gPDMetadataHFT = 0;
#endif

#if PI_ACROCOLOR_VERSION != 0
	gAcroColorHFT = 0;
#endif

	gCoreHFT = 0;

#endif /* not defined THREAD_SAFE_PDFL */

}

#ifdef __cplusplus
}
#endif

#endif /* ifdef PDFL_EXTENSION */



