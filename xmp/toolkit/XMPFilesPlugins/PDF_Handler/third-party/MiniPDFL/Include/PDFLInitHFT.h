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

 PDFLInitHFT.h  -- derived from:
 PIMain.c

 - Source code that must be compiled into PDFL HFT clients.

*********************************************************************/

#ifndef _H_PDFLInitHFT
#define _H_PDFLInitHFT

#if 0
#include "ASExpT.h"
#include "PDFInit.h"
#endif
#include "PDFLRequir.h"
#include "ASExpT.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct _t_HFTLocations
{
#ifdef THREAD_SAFE_PDFL
	HFT coreHFT;
	ASUns32 coreVersion;
	
	HFT acroSupportHFT;
	ASUns32 acroSupportVersion;
	
	HFT pdflHFT;
	ASUns32 pdflVersion;
	
	HFT cosHFT;
	ASUns32 cosVersion;
	
	HFT pdModelHFT;
	ASUns32 pdModelVersion;
	
	HFT pdfEditReadHFT;
	ASUns32 pdfEditReadVersion;
	
	HFT pdfEditWriteHFT;
	ASUns32 pdfEditWriteVersion;
	
	HFT pdSysFontHFT;
	ASUns32 pdSysFontVersion;
	
	HFT pagePDEContentHFT;
	ASUns32 pagePDEContentVersion;
	
	HFT pdsWriteHFT;
	ASUns32 pdsWriteVersion;
	
	HFT pdsReadHFT;
	ASUns32 pdsReadVersion;
	
	HFT asExtraHFT;
	ASUns32 asExtraVersion;
	
	HFT pdMetadataHFT;
	ASUns32 pdMetadataVersion;
	
	HFT acroColorHFT;
	ASUns32 acroColorVersion;
	
	HFT trustFrameworkHFT;
	ASUns32 trustFrameworkVersion;

#else /* defined THREAD_SAFE_PDFL */
	ASUns8 dummy;
#endif /* defined THREAD_SAFE_PDFL */
} HFTLocations, *PHFTLocations;


#ifdef THREAD_SAFE_PDFL
/*
** This function gets the locatopn of the table of HFT address for the current thread
** when running in a multi-threaded environment. */
PHFTLocations GetHFTLocations(void);

#endif /* defined THREAD_SAFE_PDFL */

#ifdef __cplusplus
}
#endif

#endif /* _H_PDFLInitHFT */
