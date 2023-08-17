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

 PIVersn.h

 - Data structures, types, and other things related to plug-ins and
   version changes. This file is shared between Acrobat applications
   and plug-ins.

 - Contains handshaking versioning types and data.

*********************************************************************/

#ifndef _H_PIVersn
#define _H_PIVersn

#ifdef __cplusplus
extern "C" {
#endif

#include "Environ.h"
#include "CoreExpT.h"

/*
** Prototypes for plug-in supplied functions.
** These are not expected to change, but they could if we really needed them to.
*/
/* The implementation of this proc is provided in the SDK */
typedef ACCBPROTO1 ASBool (ACCBPROTO2 *PISetupSDKProcType)(ASUns32 handshakeVersion, void *sdkData);

/* The implemention of the following is the responsibility of the Plug-in author */
typedef ACCBPROTO1 ASBool (ACCBPROTO2 *PIHandshakeProcType)(ASUns32 handshakeVersion, void *handshakeData);
typedef ACCBPROTO1 ASBool (ACCBPROTO2 *PIExportHFTsProcType)(void);
typedef ACCBPROTO1 ASBool (ACCBPROTO2 *PIImportReplaceAndRegisterProcType)(void);
typedef ACCBPROTO1 ASBool (ACCBPROTO2 *PIInitProcType)(void);
typedef ACCBPROTO1 ASBool (ACCBPROTO2 *PIUnloadProcType)(void);


/*
** Known handshake versions and data structures should be listed below:
** Top 2 bytes contains the major version; bottom 2 bytes the minor version.
** All shipping versions have a zero minor version.
** All apps and plugs-ins must support all previous shipping versions.
** Interpet last 4 digits of the name as MMmm, where MM is the major #; mm is the minor.
*/

/*
** Exporting of HFT support. Old plug-ins could be supported, but we decided to break
** them anyway.
*/
#define HANDSHAKE_V0200			((ASUns32)((2L<<16) + 0))

/* Data Types follow: */
/*
** SDKData_V0200
** Structure containing data to be passed in and out of the plug-in's PISetupSDK() routine.
** This routine is implemented in the PIMain.c routine supplied with the Plug-in SDK.
** The first field of any SDKData_VMMmm struct must be handshakeVersion.
*/
typedef struct {
	ASUns32				handshakeVersion;	/* IN  - Will always be HANDSHAKE_VERSION_V0200 */
	ExtensionID			extensionID;		/* IN  - Opaque to extensions, used to identify the Extension */
	HFT					coreHFT;			/* IN  - Host Function Table for "core" functions */
	ASCallback			handshakeCallback;	/* OUT - Address of PIHandshake() */
} PISDKData_V0200;


/* PIHandshakeData_V0200
** Structure containing data top be passed in and out of the plug-ins PIHandshake() routine.
** This routine MUST be implemented with that name by the plug-in writer.
** The first field of any PIHandshakeData_VMMmm struct must be handshakeVersion.
*/
typedef struct {
	ASUns32				handshakeVersion;					/* IN  - Will always be HANDSHAKE_V0200 */
	ASAtom				appName;							/* IN  - Name of host application */
	ASAtom				extensionName;						/* OUT - Name of the plug-in */
	ASCallback			exportHFTsCallback;					/* OUT - Routine to register HFTs this plug-in is providing */
	ASCallback			importReplaceAndRegisterCallback;	/* OUT - Routine to import other plug-in's HFTs, replace HFT functions, and register notifications */
	ASCallback			initCallback;						/* OUT - Routine for plug-in to initialize itself */
	ASCallback			unloadCallback;						/* OUT - Routine to unreplace, etc. the plug-in */
} PIHandshakeData_V0200;

/*
** If handshake version changes, insert subsequent handshake #defines and structure 
** definitions above, and change the HANDSHAKE_VERSION #define, below.
**
** If you add a shipping version, you can safely remove ALL of the non-shipping stuff:
** Non-shipping built plug-ins will operate in the previous (if any) shipping application.
** Non-shipping applications will load all previous shipping-built plug-ins.
** Naturally, for version 01.00 we don't have any previous anythings to worry about, but
** the version 01.00 app does not need to support any version 00.XX plug-ins.
*/

/* 
** Current handshake version number.
*/
#define HANDSHAKE_VERSION		HANDSHAKE_V0200

#ifdef __cplusplus
}
#endif

#endif /* _H_PIVersn */
