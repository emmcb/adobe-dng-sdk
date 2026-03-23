/*********************************************************************

 ADOBE SYSTEMS INCORPORATED
 Copyright (C) 2006 Adobe Systems Incorporated
 All rights reserved.

 NOTICE: Adobe permits you to use, modify, and distribute this file
 in accordance with the terms of the Adobe license agreement
 accompanying it. If you have received this file from a source other
 than Adobe, then your use, modification, or distribution of it
 requires the prior written permission of Adobe.

 ---------------------------------------------------------------------

 PDFError.h

 - PDF Error codes.

*********************************************************************/


#ifndef _H_PDFErrorTypes
#define _H_PDFErrorTypes

#if UNIX_ENV
#include <errno.h>
#endif

#ifdef __cplusplus
extern "C" {
#endif



#undef DefineErr
#define DefineErr(name,string)	name,

#ifndef PDFErrorsStart
#define PDFErrorsStart 0
#endif

#define ASFileE_Start		PDFErrorsStart
/* ASTypes.h */
enum {
	#include "ASFileE.h"
	asFileErrLast
};

#define ErrGenE_Start 		(ASFileE_Start + asFileErrLast)

/* ASGenE.h */
enum {
	#include "ASGenE.h"
	asGenErrLast
};

#define CosGenE_Start 		(ErrGenE_Start + asGenErrLast)

/* CostTypes.h */
enum {
	#include "CosGenE.h"
	cosErrLast
};

#define CosSynE_Start 		(CosGenE_Start + cosErrLast)

enum {
	#include	"CosSynE.h"
	cosSynErrLast
};

#define EncConvE_Start 		(CosSynE_Start + cosSynErrLast)

enum {
	// #include "EncConvE.h"
	encConvErrLast
};

#define FontSvrE_Start 		(EncConvE_Start + encConvErrLast)

enum {
	#include "FontSvrE.h"
	fsErrLast
};

#define PageE_Start 		(FontSvrE_Start + fsErrLast)

enum {
	#include "PageE.h"
	pageErrLast
};

#define PDDocE_Start 		(PageE_Start + pageErrLast)

enum {
	#include "PDDocE.h"
	pdErrLast
};

#define PDFXE_Start 		(PDDocE_Start + pdErrLast)

enum {
	#include "PDFXE.h"
	pdfXEErrLast
};

#define PDMetadataE_Start 		(PDFXE_Start + pdfXEErrLast)

enum {
    #include "PDMetadataError.h"
    pdMetadataErrLast
};

#define PDModel_Start 		(PDMetadataE_Start + pdMetadataErrLast)

enum {
	#include "PDModE.h"
	pdModErrLast
};

#define PDPageE_Start 		(PDModel_Start + pdModErrLast)

enum {
	#include "PDPageE.h"
	pdPErrLast
};

#define PDSEditE_Start 		(PDPageE_Start + pdPErrLast)

enum {
    #include "PDSError.h"
    pdsEditErrLast
};

#define PDFEditE_Start 		(PDSEditE_Start + pdsEditErrLast)

enum {
	#include "PEError.h"
	pdfEditErrLast
};

#define RasterE_Start 		(PDFEditE_Start + pdfEditErrLast)

enum {
	#include "RasterE.h"
	rasErrLast
};

#define ToolkitInitE_Start 		(RasterE_Start + rasErrLast)

enum {
	#include "ToolkitInitE.h"
	tkErrLast
};

#define XtnMgrE_Start 		(ToolkitInitE_Start + tkErrLast)

enum {
	#include "XtnMgrE.h"
	xmErrLast
};

#define MDSysErr_Use_DefineErr 1

#define WFileErr_Start 		(XtnMgrE_Start + xmErrLast)

enum {
	#include "WFileErr.h"
	wFileErrLast
};

#define MacAppE_Start 		(WFileErr_Start + wFileErrLast)

enum {
	#include "MacAppE.h"
	macAppErrLast
};

#define MacSysE_Start 		(MacAppE_Start + macAppErrLast)

enum {
	#include "MacSysE.h"
	macSysErrLast
};

#define UnixAppE_Start 		(MacSysE_Start + macSysErrLast)

enum {
	#include "UnixAppE.h"
	unixAppErrLast
};

#define UnixSysE_Start 		(UnixAppE_Start + unixAppErrLast)

enum {
	#include "UnixSysE.h"
	unixSysErrLast
};

#undef MDSysErr_Use_DefineErr

#define pdfLErrLast (UnixSysE_Start + unixSysErrLast)

#ifdef MDAPPERR

	#if UNIX_ENV
		#define mdAppErrLast unixAppErrLast
		#define MDAppErr_Start 	UnixAppE_Start
		#define mdAppErrNoError mdUnixAppErrNoError
	#elif MAC_ENV
		#define mdAppErrLast macAppErrLast
		#define MDAppErr_Start MacAppE_Start
		#define mdAppErrNoError mdMacAppErrNoError
	#else
		#define mdAppErrNoError 0
		#define mdAppErrLast 0
		#define MDAppErr_Start 	WFileErr_Start
	#endif

#else /* MDAPPERR */
	#define mdAppErrNoError 0
	#define mdAppErrLast 0
	#define MDAppErr_Start 		WFileErr_Start

#endif /* MDAPPERR */

typedef struct {
	ASInt16 sysErrNum;
	ASUns32 errCode;

} SysErrMapEntry;


#ifdef MDSYSERR

	#if UNIX_ENV
		#define mdSysErrLast unixSysErrLast
		#define MDSystemErr_Start 	UnixSysE_Start
		#define MDGetSystemErrorCode ASErrGetCode
	#elif MAC_ENV
		#define mdSysErrLast macSysErrLast
		#define MDSystemErr_Start MacSysE_Start
		#define MDGetSystemErrorCode ASErrGetSignedCode
	#elif WIN_ENV
		#define mdSysErrLast wFileErrLast
		#define MDSystemErr_Start 	WFileErr_Start
		#define MDGetSystemErrorCode ASErrGetCode
	#else
		#define mdSysErrLast 0
		#define MDSystemErr_Start 	(MDAppErr_Start + mdAppErrLast)
		#define MDGetSystemErrorCode ASErrGetCode
	#endif

	#ifdef DefineMDSysErr
		#undef DefineMDSysErr
	#endif

	#define DefineMDSysErr(mdNum, mdName, msg) {mdNum, mdName},

	/* Define array that maps from the ids in the system erro enum to the system error numbers. */
	static const SysErrMapEntry MDSysErrMap[] = {
		#include MDSYSERR
		{0, 0xffffffff /* this is an unsigned 32-bit value "-1" */}
	};
	
	#undef DefineMDSysErr

#else /* MDSYSERR */
	#define MDSystemErr_Start 		(MDAppErr_Start + mdAppErrLast)
	#define mdSysErrLast	0
	#define MDGetSystemErrorCode ASErrGetCode
	static const SysErrMapEntry MDSysErrMap[] = {
		{0, 0xffffffff /* this is an unsigned 32-bit value "-1" */}
	};
#endif /* MDSYSERR */




#undef DefineErr
#ifdef DefineMDSysErr
	#undef DefineMDSysErr
#endif

#ifdef __cplusplus
}
#endif

#endif /* _H_PDFErrorTypes */

