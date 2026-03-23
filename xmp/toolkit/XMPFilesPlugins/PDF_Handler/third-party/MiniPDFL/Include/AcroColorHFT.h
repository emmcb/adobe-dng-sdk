/*
** AcroColorHFT.h
**
** The Acrobat color Function Table
**
** (c) Copyright 2002-2006, Adobe Systems, Inc.  All Rights Reserved.
** Author: Mark Donohoe
*/

#ifndef _H_AcroColorHFT
#define _H_AcroColorHFT

#if PLUGIN

#include "AcroColorExpT.h"

#else
#include "HFTTypes.h"

#endif

#ifdef __cplusplus
extern "C" {
#endif

#ifdef NPROC /* may be already defined */
#undef NPROC
#endif

/* Enumerate the selectors */
#define PROC(returnType, name, params)			\
	name##SEL,
#define NPROC PROC
#define ANPROC NPROC
#define SPROC(returnType, name, params, stubProc) PROC(returnType, name, params)
#define NOPROC(name) PROC(ignored, DONOTUSE_##name, ignored)
#define UPROC PROC
#define UNPROC NPROC
#define USPROC SPROC
enum {
	AcroColorBAD_SELECTOR,
	#include "AcroColorProcs.h"
    AcroColorNUMSELECTORSplusOne
};

#define AcroColorNUMSELECTORS (AcroColorNUMSELECTORSplusOne - 1)

#undef PROC
#undef NPROC
#undef ANPROC
#undef SPROC
#undef NOPROC
#undef UPROC
#undef UNPROC
#undef USPROC

#define NPROC(returnType, name, params)		\
	typedef ACCBPROTO1 returnType (ACCBPROTO2 *name##SELPROTO)params;
#define ANPROC NPROC
#define PROC NPROC
#define SPROC(returnType, name, params, stubProc) PROC(returnType, name, params)
#define NOPROC(name)
	#include "AcroColorProcs.h"
#undef PROC
#undef NPROC
#undef ANPROC
#undef SPROC
#undef NOPROC

extern HFT gAcroColorHFT;

void SetUpAcroColorHFTServer(void);

#ifdef __cplusplus
}
#endif

#endif /* _H_AcroColorHFT */
