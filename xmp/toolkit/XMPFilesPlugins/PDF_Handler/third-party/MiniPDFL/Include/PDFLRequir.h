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

 PDFLRequir.h

 - Used to determine which PDFL specific HFTs (and which versions of those HFTS)
   the application requires.

 - The SDK provided version of this file provides minimum requirements
   to run on Acrobat 5.0.  If your application is designed to run on earlier
   versions you will need to lower the version numbers, while if your
   application requires routines from higher versions, you will need to raise
   the version numbers.  See the API Overview for details on how to build
   for multiple versions of Acrobat.

   To reduce the footprint and overhead of your application, as
   well as provide for maximum future compatibility, we suggest that
   when your application is "done" you edit this file to remove references
   to HFTs that are not being used. This is simply accomplished by
   setting the version number of the HFT to zero.

   For instance, if you don't use any of the PDFL HFT functions you should
   change the #define for PDFL_VERSION to:

		#define PDFL_VERSION		0

*********************************************************************/

#ifndef _H_PDFLRequir
#define _H_PDFLRequir

#ifdef __cplusplus
extern "C" {
#endif

#include "Environ.h"
#include "PIRequir.h"


#ifdef MI_VERSN
#include MI_VERSN
#endif

/* 
** PDFL HFT 
*/
#ifndef PDFL_VERSION
#define PDFL_VERSION	((6L<<16) + 0)
#endif

#ifdef __cplusplus
}
#endif

#endif /* _H_PDFLRequir */
