/*************************************************************************
* WARNING: All additions to this file must go at the end of the file.
* This file is #included to build a table of function pointers (an HFT).
* All components which make use of this HFT rely on the order and
* positioning of these functions (e.g. plug-ins). Adding entries in the 
* middle of this file or modifying existing entries will cause these 
* components to break.
**************************************************************************/

/*
** PDBatesProcs.h
** (c) Copyright 2004, Adobe Systems, Inc.  All Rights Reserved.
*/

/**
* Deprecated The following function has been deprecated, do not use.
*
* <p> Adds Bates Numbering to a PDF document. </p>
* @param pdDoc			The PDF document to which Bates Numbering is added.
* @param batesParams	Bates Numbering specific parameters, which include the start number, number of digits, prefix, and suffix.
* @param params			All layout parameters that are related to how Bates Numbering is placed on the page.
* @return <code>true</code> if Bates Numbering was successfully added, <code>false</code> otherwise.
* @exception pdErrBadAction
* @see PDDocRemoveBatesNumbering 
* @since PI_PDMODEL_VERSION >= 0x00090000 
*
**/ 
XNPROC(ASBool, PDDocAddBatesNumbering, (PDDoc pdDoc, PDDocBatesNumberingParams batesParams, PDDocLayoutParamsRec params))
/**
* Deprecated The following function has been deprecated, do not use.
*
* <p> Remove Bates Numbering from a PDF document. </p>
* @param pdDoc   The PDF document from which Bates Numbering is to be removed.
* @return <code>true</code> if Bates Numbering was successfully removed, <code>false</code> otherwise.
* @exception pdErrBadAction
* @see PDDocAddBatesNumbering 
* @since PI_PDMODEL_VERSION >= 0x00090000 
*
*/
XNPROC(ASBool, PDDocRemoveBatesNumbering, (PDDoc pdDoc))
