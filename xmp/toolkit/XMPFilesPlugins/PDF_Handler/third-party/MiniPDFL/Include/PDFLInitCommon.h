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

 PDFLInitHFT.c  -- derived from:
 PIMain.c

 - Source code that must be linked into every plug-in.

*********************************************************************/

#ifndef _H_PDFLInitCommon
#define _H_PDFLInitCommon

#include "ASExpT.h"
#include "PDFInit.h"
#include "PDFLRequir.h"
#include "PDFLInitHFT.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef ASInt32 (*PDFLInitType)(PDFLData data);
typedef HFT (*PDFLGetCoreHFTType)();
typedef void *(*PDFLInitThreadLocalDataType)(ThreadLocalKey *key, ASUns32 size);
typedef void (*PDFLTermType)();

typedef struct _t_InitProcs
{
	PDFLInitType	initProc;
	PDFLGetCoreHFTType	getCoreHFTProc;
	PDFLInitThreadLocalDataType initThreadLocalData;
} InitProcs, *InitProcsP;

typedef struct _t_TermProcs
{
	PDFLTermType termProc;
} TermProcs, *TermProcsP;


/* 
** This function provides common portions of the initialization and termination
** process. It should only be called by other parts of the Adobe supplied client
** side code
*/
ASInt32 PDFLInitCommon(PDFLData data, InitProcsP procs, os_size_t sizeHFTLocTab);

/* 
** This function provides common portions of the initialization and termination
** process. It should only be called by other parts of the Adobe supplied client
** side code
*/
void PDFLTermCommon(TermProcsP termProcs);

/* pass in name of hft and minimum required version.  If the hft has a GetVersion
** routine pass in the version of the hft where it first appears.  Pass 0 if there
** is no GetVersion call. returns hft and version of the returned hft (>= requiredVer)
** and true if successful on failure, both resultHFT and resultingVer return as NULL.
** This function should only be called by other parts of the Adobe supplied client
** side code
*/
ASBool GetRequestedHFT(const char* table, ASUns32 requiredVer, ASUns32 *resultingVer,
					   HFT *resultHFT);

#ifdef __cplusplus
}
#endif

#endif /* _H_PDFLInitCommon */
