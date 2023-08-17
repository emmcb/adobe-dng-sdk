/*********************************************************************

 ADOBE SYSTEMS INCORPORATED
 Copyright (C) 1994-2003 Adobe Systems Incorporated
 All rights reserved.

 NOTICE: Adobe permits you to use, modify, and distribute this file
 in accordance with the terms of the Adobe license agreement
 accompanying it. If you have received this file from a source other
 than Adobe, then your use, modification, or distribution of it
 requires the prior written permission of Adobe.

 ---------------------------------------------------------------------


*********************************************************************/
#ifndef _H_PDFLInitCommonPriv
#define _H_PDFLInitCommonPriv

#include "PDFLInitCommon.h"
#include "PDFLPrivRequir.h"


#ifdef THREAD_SAFE_PDFL

#if PDFL_PRIV_VERSION != 0
#define gPDFLPrivHFT (((PPrivHFTLocations)(GetHFTLocations()))->pdflPrivHFT)
#define gPDFLPrivVersion (((PPrivHFTLocations)(GetHFTLocations()))->pdflPrivVersion)
#endif

#if PI_PDFEDIT_PRIVATE_VERSION != 0
#define gPDFEditPrivateHFT (((PPrivHFTLocations)(GetHFTLocations()))->pdfEditPrivateHFT)
#define gPDFEditPrivateVersion (((PPrivHFTLocations)(GetHFTLocations()))->pdfEditPrivateVersion)
#endif
#endif /* defined THREAD_SAFE_PDFL */

typedef struct _t_PrivHFTLocations
{
#ifdef THREAD_SAFE_PDFL
	HFTLocations hftLocs;

#if PDFL_PRIV_VERSION != 0
	HFT pdflPrivHFT;
	ASUns32  pdflPrivVersion;
#endif

#if PI_PDFEDIT_PRIVATE_VERSION != 0
	HFT pdfEditPrivateHFT;
	ASUns32  pdfEditPrivateVersion;
#endif
#else /* defined THREAD_SAFE_PDFL */
	ASUns8	dummy;
#endif /* defined THREAD_SAFE_PDFL */
} PrivHFTLocations, *PPrivHFTLocations;



#endif /* _H_PDFLInitCommonPriv */
