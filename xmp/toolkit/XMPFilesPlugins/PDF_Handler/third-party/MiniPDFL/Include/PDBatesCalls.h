/*
** PDBatesCalls.h
** Copyright (C) 2004, Adobe Systems, Inc.  All Rights Reserved.
*/

#ifndef _H_PDBatesCalls
#define _H_PDBatesCalls

#include "PDBatesExpT.h"

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

#define PDBatesHFTName				"PDBates"

/* for Adobe use only */
#define _PDBATESHFT_LATEST_VERSION 0x00090000
#define _PDBATESHFT_LAST_BETA_COMPATIBLE_VERSION 0x00090000
#define _PDBATESHFT_IS_BETA 0

/* for public use */
#define PDBATESHFT_LATEST_VERSION (_PDBATESHFT_IS_BETA ? (kHFT_IN_BETA_FLAG | _PDBATESHFT_LATEST_VERSION) : _PDBATESHFT_LATEST_VERSION)

#define PDBATESHFT_VERSION_9   0x00090000

/* Enumerate the selectors */
#define NPROC(returnType, name, params)			\
	name##SEL,
#define SPROC(returnType, name, params, stubProc) \
	name##SEL,
#define NOPROC(name)	\
	name##SEL,

#define XNPROC NPROC
#define PROC	NPROC
#define XPROC NPROC
#define ENPROC	NPROC
	enum {
		PDBATESBAD_SELECTOR,
#include "PDBatesProcs.h"
		PDBATESNUMSELECTORSplusOne
	};
#define PDBATESNUMSELECTORS (PDBATESNUMSELECTORSplusOne - 1)

#undef NPROC
#undef XNPROC
#undef SPROC
#undef PROC
#undef XPROC
#undef NOPROC
#undef ENPROC

#if !PLUGIN
	/* Static link */
	#define NPROC(returnType, name, params)	\
		extern  returnType name params;
//		extern ACEX1 returnType ACEX2 name params;
	#define SPROC(returnType, name, params, stubProc) \
		extern ACEX1 returnType ACEX2 name params;
	#define XNPROC NPROC
	#define PROC	NPROC
	#define XPROC NPROC
	#define ENPROC	NPROC		
	#define NOPROC(name)
	#include "PDBatesProcs.h"
	#undef NPROC
	#undef XNPROC
	#undef SPROC
	#undef PROC
	#undef NOPROC	
	#undef XPROC
	#undef ENPROC	
#else

	/* HFT version */
	#include "PIRequir.h"

	/* Create the prototypes */
	#define NPROC(returnType, name, params)		\
		typedef ACCBPROTO1 returnType (ACCBPROTO2 *name##SELPROTO)params;
	#define SPROC(returnType, name, params, stubProc) \
		typedef ACCBPROTO1 returnType (ACCBPROTO2 *name##SELPROTO)params;

#if READER_PLUGIN
   /* Force Error for Exchange procs */
   #define XPROC(returnType, name, params)
   #define XNPROC(returnType, name, params)
	#define UPROC(returnType, name, params) 
    #define UNPROC(returnType, name, params) 
    #define USPROC(returnType, name, params, stubProc) 
#else
   #define XPROC NPROC
   #define XNPROC NPROC
   #define UPROC NPROC
   #define UNPROC NPROC
   #define USPROC SPROC

#endif
	#define NOPROC(name)	
	#include "PDBatesProcs.h"	
	#undef NPROC
	#undef XNPROC
	#undef SPROC
	#undef PROC
	#undef NOPROC	
	#undef XPROC		
	#undef ENPROC
	#include "PIRequir.h"

	#include "PDBatesExpT.h"
	#include "NSelExpT.h"
	#include "CorCalls.h"		/* For ASCallbackCreateProto */
#ifndef THREAD_SAFE_PDFL
	extern HFT gPDBatesHFT;
#endif

	/* PDDocAddBatesNumbering */
	#define PDDocAddBatesNumbering (*((PDDocAddBatesNumberingSELPROTO) (gPDBatesHFT[PDDocAddBatesNumberingSEL])))
	/* PDDocRemoveBatesNumbering */
	#define PDDocRemoveBatesNumbering (*((PDDocRemoveBatesNumberingSELPROTO) (gPDBatesHFT[PDDocRemoveBatesNumberingSEL])))

#endif /* PLUGIN */

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* !defined(_H_PDBatesCalls) */
