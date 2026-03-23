/*
//
//  ADOBE SYSTEMS INCORPORATED
//  Copyright (C) 2000-2003 Adobe Systems Incorporated
//  All rights reserved.
//
//  NOTICE: Adobe permits you to use, modify, and distribute this file
//  in accordance with the terms of the Adobe license agreement
//  accompanying it. If you have received this file from a source other
//  than Adobe, then your use, modification, or distribution of it
//  requires the prior written permission of Adobe.
//
*/
/*
 * MyPDFLibUtils.cpp
 *
 */
 
#include "MyPDFLibUtils.h"
#include "PDFInit.h"
#include <stdlib.h>
#include <limits.h>

/**************************************************************************

	Memory Allocators
	Very simple memory management w/o error checking.

 **************************************************************************/

static void* MyPDFLMemAllocProc(void* clientData, ASSize_t size)
{
	return malloc(size);
}

static void* MyPDFLMemRealloc(void* clientData, void* ptr, ASSize_t size)
{
	return realloc(ptr, size);
}

static void MyPDFLMemFree(void *clientData, void* ptr)
{
	if (ptr != NULL)
		free(ptr);
}

static ASSize_t MyPDFLMemAvail(void *clientData)
{
	return INT_MAX;
}

/* This variable is passed as a part of PDFLData for PDFLInit. */
static TKAllocatorProcs gMyPDFLMemAllocProcs = {
	MyPDFLMemAllocProc,		/* ASMemAllocProc */
	MyPDFLMemRealloc,		/* ASMemReallocProc */
	MyPDFLMemFree,			/* ASMemFreeProc */
	MyPDFLMemAvail,			/* ASMemAvailProc */
	NULL					/* clientData */
};

static TKAllocatorProcs* PDFLGetMemAllocProcs()
{
	return &gMyPDFLMemAllocProcs;
}


/**************************************************************************

	PDFLInit and PDFLTerm wrapper

 **************************************************************************/

int MyPDFLInit()
{
	PDFLDataRec	pdflData;
	memset(&pdflData, 0, sizeof(PDFLDataRec));

	pdflData.size = sizeof(PDFLDataRec);
    pdflData.allocator = PDFLGetMemAllocProcs();

#ifdef PLUGIN
	return PDFLInitHFT(&pdflData);
#else
	return PDFLInit(&pdflData);
#endif
}

void MyPDFLTerm()
{
#ifdef PLUGIN
	PDFLTermHFT();
#else
	PDFLTerm();
#endif
}

#include "PDFLInitCommon.h"

#if WIN_ENV && MiniPDFL_DLL
static HINSTANCE hInstance = 0;
static HINSTANCE miniPDFL = 0;
#define PATH_LEN 1024

#ifdef __cplusplus
extern "C" {
#endif

BOOL APIENTRY DllMain( HANDLE hModule, 
                        DWORD ul_reason_for_call, 
                        LPVOID lpReserved )
{
    hInstance = (HINSTANCE) hModule;

    switch( ul_reason_for_call ) {

    case DLL_PROCESS_ATTACH: //A new process is attempting to access the DLL; one thread is assumed.

	break;	
    case DLL_THREAD_ATTACH://A new thread of an existing process is attempting to access the DLL; this call is made beginning with the second thread of a process attaching to the DLL.
    	break;
    case DLL_THREAD_DETACH://One of the additional threads (not the first thread) of a process is detaching from the DLL.
	break;
    
    case DLL_PROCESS_DETACH://A process is detaching from the DLL.
    	break;
    }
    return TRUE;
}

#ifdef __cplusplus
}
#endif
#endif // #if WIN_ENV


#ifdef PLUGIN
ASInt32 PDFLInitHFT(PDFLData data)
{
	InitProcs procs;
	memset( &procs, 0, sizeof(InitProcs) );
		
#if 0 //WIN_ENV

#if 0
	WCHAR pluginDir[PATH_LEN];
	GetModuleFileNameW( hInstance, pluginDir, PATH_LEN );

	// Get plug-in directory name by erasing file name
	for( size_t i = wcslen(pluginDir); pluginDir[i] != L'\\'; pluginDir[i--] = 0 );
	
	// Get MiniPDFL file location 
	wcscat_s( pluginDir, PATH_LEN, L"MiniPDFL.dll" );
#else
	WCHAR pluginDir[] = L"C:\\Work\\minipdfl_android\\pdfl\\public\\libraries\\windows\\debug\\MiniPDFL.dll";
	//WCHAR pluginDir[] = L"C:\\Work\\minipdfl_android\\pdfl\\public\\libraries\\windows\\release\\MiniPDFL.dll";
#endif
	
	miniPDFL = LoadLibrary( pluginDir );

	if( miniPDFL )
	{
		procs.initProc = (PDFLInitType) ::GetProcAddress( miniPDFL, "PDFLInit" );
		procs.getCoreHFTProc = (PDFLGetCoreHFTType) ::GetProcAddress( miniPDFL, "PDFLGetCoreHFT" );
		procs.initThreadLocalData = (PDFLInitThreadLocalDataType) ::GetProcAddress( miniPDFL, "PDFLInitThreadLocalData" );
	}

#else
	/* Get location of functions needed to initialize the HFT mechanism */
	procs.initProc = (PDFLInitType) PDFLInit;
	procs.getCoreHFTProc = (PDFLGetCoreHFTType) PDFLGetCoreHFT;
	procs.initThreadLocalData = PDFLInitThreadLocalData;
#endif

	if ( !procs.getCoreHFTProc )
		return GenError(ERR_GENERAL);
	
	/* Initialize the library */
	ASInt32 errorCode;
	if ( (errorCode = PDFLInitCommon(data, &procs, sizeof(HFTLocations))) != 0)
		return errorCode;
	return 0;

}

void PDFLTermHFT(void)
{
	
	TermProcs termProcs;

#if 0 //WIN_ENV
	if( miniPDFL )
	{
		termProcs.termProc = (PDFLTermType) ::GetProcAddress( miniPDFL, "PDFLTerm" );
		PDFLTermCommon(&termProcs);
		FreeLibrary( miniPDFL );
		miniPDFL = NULL;
	}
#else
	termProcs.termProc = (PDFLTermType) PDFLTerm;
	PDFLTermCommon(&termProcs);
#endif
	
}

#endif
