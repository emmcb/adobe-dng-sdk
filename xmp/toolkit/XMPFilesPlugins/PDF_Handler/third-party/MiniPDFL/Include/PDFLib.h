/*
** PDFLib.h
**
** PRODUCT file CoreTech PDFLib 
**
**
** (c) Copyright 2002-2003 Adobe Systems Inc.  All Rights Reserved.
*/

#ifndef _H_PDFLIB
#define _H_PDFLIB

#define CAN_PRINT 1
#define CAN_COPY 0 
#define CAN_CREATE_NOTEFILE 0
#define CAN_PRINT_FORM_FIELD_ONLY 0

#define HAS_32BIT_ATOMS 1
#define HAS_ACTION_PROPS 0
#define HAS_ADM 0
#define HAS_PAGECACHE 0
#define AGM_SAVVY

/* uses AGM for printing */
#define AGMPRINT_INTERFACE 1 

#define CTJPEG 1

/* All other flags are default */

#include "CommonConfig.h"

#endif
