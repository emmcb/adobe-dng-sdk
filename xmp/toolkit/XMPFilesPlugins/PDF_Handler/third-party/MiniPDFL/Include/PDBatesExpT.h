/*
** PDBatesExpT.h
** Copyright (C) 2004, Adobe Systems, Inc.  All Rights Reserved.
**
** Types, macros, structures, etc. required to use the PD3D private HFT.
*/

#ifndef _H_PDBatesExpT
#define _H_PDBatesExpT

#include "CoreExpT.h"
#include "ASExpT.h"
#include "PDExpT.h"
#if !PLUGIN
#include "ASEnv.h"
#endif /* !PLUGIN */

#ifdef __cplusplus
extern "C" {
#endif

/**
*	Parameters used for describing Bates Numbering.
*	As an example, Bates Numbering with <code>start=100 nDigits=6</code> would look like <code>000100, 000101, 000102, 000103...</code>.
*	@see PDDocAddBatesNumbering
*/
typedef struct _t_PDDocBatesNumberingParams {
	/** The size of the data structure. 
	*/
	ASSize_t		size;
	/**	The start number of the Bates Numbering (required).
	*/
	ASInt64 start;
	/**	The number of digits of the Bates Numbering (required).
	*/
	ASUns8	nDigits;
	/**	Bates Numbering prefix (optional).
	*/
	ASText	prefix;
	/**	Bates Numbering suffix (optional).
	*/
	ASText	suffix;
	/**	The original page index to which Bates Numbering was added(optional).
	*/
	ASUns8  pageIndex;

} PDDocBatesNumberingParamsRec, *PDDocBatesNumberingParams;

/**
	Parameters used for adding and describing Bates Numbering.
	@see PDDocAddBatesNumbering
*/
typedef struct _t_PDDocLayoutParams {
	/** The size of the data structure. 
	*/
	ASSize_t		size;

	/** The page range of the document to which Bates Numbering should be added.
	*/
	PDPageRange		targetRange;

	/**	The margin for placement of Bates.
	*/
	ASFixedRect		margins;

	/** The horizontal alignment to be used when adding Bates Numbering to a page.
	*/
	PDHorizAlign	horizAlign;

	/** The vertical alignment to be used when adding Bates Numbering to a page.
	*/
	PDVertAlign		vertAlign;

	/** The color setting for adding Bates Numbering to a page.
	*/
	PDColorValueRec color;

	/** The font size for adding Bates Numbering to a page.
	*/
	double	fontSize;	

	/** The font name for adding Bates Numbering to a page.
	*/
	ASAtom	fontName;

	/** The font type for adding Bates Numbering to a page.
	*/
	ASAtom	fontType;

	/** Determines whether to draw an underline to Bates Numbering in a page (optional).
	*/
	ASBool	underline;
} PDDocLayoutParamsRec,  *PDDocLayoutParams;

#ifdef __cplusplus
}
#endif

#endif /* _H_PDBatesExpT */
