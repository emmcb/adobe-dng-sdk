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

 PIMain.h

 - Include file for PIMain.c.
 - Contains #defines, macros, function protos, and global variable
   declarations.

*********************************************************************/

#ifndef _H_PIMain
#define _H_PIMain

#ifdef __cplusplus
extern "C" {
#endif

#include "PICommon.h"
#include "PIRequir.h"

#include "CoreExpT.h"

extern HFT 			gCoreHFT;
extern ASUns32 		gCoreVersion;

/* The following globals are only needed if functions from them are being accessed */

#if PI_ACROSUPPORT_VERSION != 0
extern HFT gAcroSupportHFT;
extern ASUns32 gAcroSupportVersion;
#endif

#if PI_COS_VERSION != 0
extern HFT gCosHFT;
extern ASUns32 gCosVersion;
#endif

#if PI_PDMODEL_VERSION != 0
extern HFT gPDModelHFT;
extern ASUns32 gPDModelVersion;
#endif

#if PI_PDFEDIT_READ_VERSION != 0
extern HFT gPDFEditReadHFT;
extern ASUns32 gPDFEditReadVersion;
#endif

#if PI_PDFEDIT_WRITE_VERSION != 0
extern HFT gPDFEditWriteHFT;
extern ASUns32 gPDFEditWriteVersion;
#endif

#if PI_PDSYSFONT_VERSION != 0
extern HFT gPDSysFontHFT;
extern ASUns32 gPDSysFontVersion;
#endif

#if PI_PAGE_PDE_CONTENT_VERSION != 0
extern HFT gPagePDEContentHFT;
extern ASUns32 gPagePDEContentVersion;
#endif

#if PI_ACROVIEW_VERSION != 0
extern HFT gAcroViewHFT;
extern ASUns32 gAcroViewVersion;
#endif

#if PI_PDSEDIT_WRITE_VERSION != 0
extern HFT gPDSWriteHFT;
extern ASUns32 gPDSWriteVersion;
#endif

#if PI_PDSEDIT_READ_VERSION != 0
extern HFT gPDSReadHFT;
extern ASUns32 gPDSReadVersion;
#endif

#if PI_MACINTOSH_VERSION != 0
extern HFT gMacintoshHFT;
extern ASUns32 gMacintoshVersion;
#endif

#if PI_WIN_VERSION
extern HFT gWinHFT;
extern ASUns32 gWinVersion;
#endif

#if PI_OS2_VERSION
extern HFT gOS2HFT;
extern ASUns32 gOS2Version;
#endif

#if PI_ASEXTRA_VERSION
extern HFT gASExtraHFT;
extern ASUns32 gASExtraVersion;
#endif

#if PI_ACROVIEWSWEETPEA_VERSION
extern HFT gAcroViewSweetPeaHFT;
extern ASUns32 gAcroViewSweetPeaVersion;
#endif

#if PI_PDMETADATA_VERSION != 0
extern HFT gPDMetadataHFT;
extern ASUns32 gPDMetadataVersion;
#endif

#if PI_ACROCOLOR_PRIVATE_VERSION != 0
extern HFT gAcroColorPrivateHFT;
extern ASUns32 gAcroColorPrivateVersion;
#endif


#if WIN_PLATFORM
#include <windows.h>
extern HINSTANCE 	gHINSTANCE;
extern HWND 		gHWND;
#endif

#if UNIX_PLATFORM
extern void *gHandle;
#endif
ASBool GetRequestedHFT(const char* table, ASUns32 requiredVer, ASUns32 *resultingVer, HFT *resultHFT);

/*
** Prototypes for PI-provided functions in the handshaking.
*/

ACCB1 ASBool ACCB2 PISetupSDK(ASUns32 handshakeVersion, void *sdkData);

#if MAC_PLATFORM

/*
	Acrobat's bundle.  Please do not use CFBundleGetMainBundle() -- If you are running in the
	browser, CFBundleGetMainBundle() returns the bundle of the browser and not Acrobat's.  If you're
	going to hold onto this, remember to call CFRetain() and CFRelease() on it.
*/
extern CFBundleRef	gAppBundle;

/*
	The plug-ins bundle.  If you're going to hold onto this, remember to call CFRetain() and 
	CFRelease() on it.
*/
extern CFBundleRef	gPluginBundle;

/*
	GetAcroPluginResourceMap
	
	This routine opens the resource map of the plug-in.  Opening resources is expensive (performance wise)
	so only do so if resources are really needed.
	
	@returns	The resource reference number for the plug-ins resource file or kResFileNotOpened if the file cannot be opened
*/
ResFileRefNum GetAcroPluginResourceMap();

#endif /* MAC_PLATFORM */

#ifdef __cplusplus
}
#endif

#endif /* _H_PIMAIN */
