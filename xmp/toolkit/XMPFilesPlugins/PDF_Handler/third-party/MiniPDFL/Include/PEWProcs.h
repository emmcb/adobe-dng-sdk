/*************************************************************************
* WARNING: All additions to this file must go at the end of the file.
* This file is #included to build a table of function pointers (an HFT).
* All components which make use of this HFT rely on the order and
* positioning of these functions (e.g. plug-ins). Adding entries in the 
* middle of this file or modifying existing entries will cause these 
* components to break.
**************************************************************************/

/*********************************************************************

 ADOBE SYSTEMS INCORPORATED
 Copyright (C) 1998-2006 Adobe Systems Incorporated
 All rights reserved.

 NOTICE: Adobe permits you to use, modify, and distribute this file
 in accordance with the terms of the Adobe license agreement
 accompanying it. If you have received this file from a source other
 than Adobe, then your use, modification, or distribution of it
 requires the prior written permission of Adobe.

 ---------------------------------------------------------------------

 PEWProcs.h

 - Catalog of functions exported by the PDFEditWrite HFT.

*********************************************************************/

/*------------------------------------------------------------------------
	PDFEdit Public Methods - Generic PDEContent and PDEElement methods.
------------------------------------------------------------------------*/

/**
	Creates an empty content object. 

	<p>Call PDERelease() to dispose of the returned content object 
	when finished with it. </p>
	@return An empty content object. 
	@exception peErrPStackUnderflow 
	@see PDEContentCreateFromCosObj 
	@see PDPageAcquirePDEContent 

	@note Do not use this method to create a PDEContent to be 
	put into a PDPage. Instead, call PDPageAcquirePDEContent(). 
	@since PI_PDFEDIT_WRITE_VERSION >= 0x00040000

*/
UNPROC(PDEContent, PDEContentCreate, (void))

/**
	This is the main method for converting a PDEContent into 
	PDF contents and resources. 

	<p>This method does not change the PDEContent object or its 
	reference count. </p>

	<p>The caller of this function is responsible for adding the 
	contents and the resources returned from this method to 
	the Page Object. </p>

	@param pdeContent IN/OUT A content object. 
	@param flags IN/OUT Flags specifying the type of object to create 
	(page contents, form, or charproc) and how it is created. 
	It must be one or more of PDEContentToCosObjFlags. 
	@param attrs IN/OUT A pointer to a PDEContentAttrs structure that 
	contains the appropriate form attributes or cache device/char-width attributes, and so on. 
	If it is zero, no attributes are set. 
	@param attrsSize IN/OUT The size of the <code>attrs</code> buffer in bytes. Zero 
	if <code>attrs</code> is zero. 
	@param cosDoc IN/OUT The document in which the contents and resources 
	are created. 
	@param filtersP IN/OUT A pointer to a PDEFilterArray structure 
	that specifies which filters to use in encoding the contents; 
	it may be <code>NULL</code>. If <code>filtersP</code> contains any <code>encodeParms</code>, they 
	must belong to <code>cosDoc</code>. 
	@param contentsP IN/OUT (Filled by the method) The Cos object for 
	the resulting contents in <code>pdeContent</code>. 
	@param resourcesP IN/OUT (Filled by the method) The Cos object for 
	the resulting resources in <code>pdeContent</code>. Note that the client 
	is responsible for putting the <code>resourcesP</code> dictionary into 
	the <code>contentsP</code> stream for non-page objects. The client must 
	do this for XObject Forms and appearance dictionaries in 
	annotations. For Type 3 fonts, the resource dictionaries 
	must be merged and put into the Type 3 font dictionary. 
	For a page, the contents and resources must be put into 
	the page object. 
	@exception peErrUnknownResType 
	@exception pageErrErrorParsingImage 
	@exception pdErrBadResMetrics 
	@exception peErrWrongPDEObjectType 
	@exception peErrUnknownPDEColorSpace 
	@see PDEContentCreateFromCosObj 

	@note Do not use this method to put a PDEContent into a 
	PDPage. Instead, call PDPageSetPDEContent(). 
	@since PI_PDFEDIT_WRITE_VERSION >= 0x00040000

*/
UNPROC(void, PDEContentToCosObj, (
			IN	PDEContent pdeContent, 
			IN	ASUns32 flags, 			/* kPDEContentToPage, kPDEContentToForm, kPDEToContentCharProc */
			IN	PDEContentAttrsP attrs, 
			IN	ASUns32 attrsSize,
			IN	CosDoc cosDoc, 
			IN	PDEFilterArrayP filtersP,
			OUT	CosObj *contentsP, 
			OUT	CosObj *resourcesP)
	)

/**
	Removes an element from a PDEContent. 
	@param pdeContent IN/OUT A content object. 
	@param index IN/OUT The index in <code>pdeContent</code> of the element to remove 
	whose reference count is decremented. 
	@exception peErrWrongPDEObjectType 
	@see PDEContentAddElem 

	@note This decrements the reference count of the element 
	removed. 
	@since PI_PDFEDIT_WRITE_VERSION >= 0x00040000

*/
UNPROC(void, PDEContentRemoveElem,(
			IN	PDEContent pdeContent, 
			IN	ASInt32 index)
	)

/**
	Inserts an element into a PDEContent. 
	
	@param pdeContent The content to which <code>pdeElement</code> is added. 
	
	@param addAfterIndex The location after which <code>pdeElement</code> is 
	added. It should be kPDEBeforeFirst to add to the beginning 
	of the display list. 
	@param pdeElement The element to add to <code>pdeContent</code>. The 
	reference count of <code>pdeElement</code> is incremented.
	@exception peErrWrongPDEObjectType 
	@exception genErrBadParm 
	@see PDEContentRemoveElem 
	@since PI_PDFEDIT_WRITE_VERSION >= 0x00040000
	@see PDEContentRemoveElem 

	@note This method increments the reference count of <code>pdeElement</code>. 
	@since PI_PDFEDIT_WRITE_VERSION >= 0x00040000
*/
UNPROC(void, PDEContentAddElem, (
			IN	PDEContent pdeContent, 
			IN	ASInt32 addAfterIndex, 
			IN	PDEElement pdeElement)
	)
/*------------------------------------------------------------------------
	General methods for PDEPath, PDEImage and PDEXObject.
------------------------------------------------------------------------*/

/**
	Sets the graphics state information for an element. 

	<p>This method is valid only for PDEForm, PDEImage, PDEPath, and 
	PDEShading elements. </p>

	@param pdeElement An element whose graphics state is set. 
	
	@param stateP A pointer to a PDEGraphicStateP structure 
	with graphics state information to set for <code>pdeContent</code>. Any 
	of the <code>stateP</code> parameter's color space or ExtGState objects have their 
	reference count incremented. 
	@param stateSize The size of the <code>stateP</code> buffer in bytes.
	@exception peErrWrongPDEObjectType 
	@exception genErrBadParm will be raised if the first parameter, <code>pdeElement</code>, 
	does not have a graphics state associated with it. 
	@see PDEElementGetGState 

	@note This method causes any of the <code>stateP</code> parameter's color space or 
	ExtGState objects to have their reference count incremented, 
	and previous graphic state objects to be decremented. 
	@since PI_PDFEDIT_WRITE_VERSION >= 0x00040000
*/
UNPROC(void, PDEElementSetGState, (
			IN	PDEElement pdeElement, 
			IN	PDEGraphicStateP stateP, 
			IN	ASUns32 stateSize)
	)

/**
	Superseded by PDEElementSetMatrixEx() in Acrobat 10.0.
	
	Sets the transformation matrix for an element. 

	<p>The element may not be a PDEContainer, a PDEGroup, a PDEPlace, 
	or a PDEText. </p>

	@param pdeElement IN/OUT An element whose transformation matrix 
	is set. 
	@param matrixP IN/OUT A pointer to an ASFixedMatrix that holds the 
	transformation matrix to set for <code>pdeContent</code>. 
	@exception peErrWrongPDEObjectType 
	@exception genErrBadParm 
	@see PDEElementGetMatrix 
	@see PDEElementSetMatrixEx
	@since PI_PDFEDIT_WRITE_VERSION >= 0x00040000

*/
#if ADD_ASFIXED //PKG
UNPROC(void, PDEElementSetMatrix, (
			IN	PDEElement pdeElement, 
			IN	ASFixedMatrixP matrixP)
	)
#endif //ADD_ASFIXED //PKG

/**
	Sets the current clip for an element. 

	The <code>pdeElement</code> parameter's previous clip's reference count is decremented 
	(if it had one), and the <code>pdeClip</code> parameter's reference count is incremented. 
	
	@param pdeElement IN/OUT An element whose clip is set. 
	@param pdeClip IN/OUT The clip to set for <code>pdeContent</code>. 
	@exception peErrWrongPDEObjectType 
	@exception genErrBadParm 
	@see PDEElementGetClip 
	@since PI_PDFEDIT_WRITE_VERSION >= 0x00040000

*/
UNPROC(void, PDEElementSetClip, (
			IN	PDEElement pdeElement, 
			IN	PDEClip pdeClip)
	)

/**
	Makes a copy of an element. The caller is responsible for 
	releasing the copy with PDERelease(). 
	@param pdeElement IN/OUT The element to copy. 
	@param flags IN/OUT A bit field of PDEElementCopyFlags. 
	@return A copy of <code>pdeElement</code>. 
	@exception peErrWrongPDEObjectType 
	@exception genErrBadParm 
	@see PDEContentGetElem 
	@since PI_PDFEDIT_WRITE_VERSION >= 0x00040000

*/
UNPROC(PDEElement, PDEElementCopy, (
			IN	PDEElement pdeElement,
			IN	ASUns32 flags)
	)
		
/*------------------------------------------------------------------------
	PDEText methods.
------------------------------------------------------------------------*/

/**
	Sets the graphics state of a text run. 

	@note This method increments the reference count of objects 
	in <code>stateP</code>. 
	@param pdeText The text object containing a text run whose 
	graphics state is set. 
	@param runIndex The index of the text run. 
	@param stateP A pointer to a PDEGraphicStateP structure 
	with the graphics state to set. 
	@param stateSize The size of the <code>stateP</code> buffer in bytes.
	@exception peErrWrongPDEObjectType 
	@exception genErrBadParm 
	@see PDETextGetGState 
	@since PI_PDFEDIT_WRITE_VERSION >= 0x00040000
*/
UNPROC(void, PDETextRunSetGState, (
			IN	PDEText pdeText, 
			IN	ASInt32 runIndex, 
			IN	PDEGraphicStateP stateP, 
			IN	ASUns32 stateSize)
	)

/**
	Sets the text state of a text run. 
	@param pdeText The text object containing a text run whose 
	text state is set. 
	@param runIndex The index of the text run. 
	@param stateP A pointer to a PDETextState structure with 
	text state. 
	@param stateSize The size of the <code>stateP</code> buffer in bytes.
	@exception peErrWrongPDEObjectType 
	@exception genErrBadParm 
	@see PDETextGetTextState 
	@see PDETextRunSetState 

	@note This method has the following side effect (Acrobat 
	5 and later): It modifies the text matrix of the run. In order to 
	maintain backward compatibility, this method only directly 
	operates on the first four fields of PDETextState. When 
	it is called, it calculates a new text matrix with three 
	additional fields: <code>fontSize</code>, <code>hScale</code>, and <code>textRise</code> (see PDETextState). 
	To avoid this behavior, use PDETextRunSetState() instead (which 
	was added to address this problem). 
	@since PI_PDFEDIT_WRITE_VERSION >= 0x00040000
*/
UNPROC(void, PDETextRunSetTextState, (
			IN	PDEText pdeText, 
			IN	ASInt32 runIndex, 
			IN	PDETextStateP stateP, 
			IN	ASUns32 stateSize)
	)

/**
	Sets the font of a text run. 

	@note This method decrements the reference count of the 
	previous font and increments the reference count of the 
	new font. 
	@param pdeText IN/OUT The text object containing a text run whose 
	font is set. 
	@param runIndex IN/OUT The index of the text run. 
	@param font IN/OUT The font set for the text run. 
	@exception peErrWrongPDEObjectType 
	@exception genErrBadParm 
	@see PDETextGetFont 
	@since PI_PDFEDIT_WRITE_VERSION >= 0x00040000

*/
UNPROC(void, PDETextRunSetFont, (
			IN	PDEText pdeText,
			IN	ASInt32 runIndex,
			IN	PDEFont font)
	)

/**
	Superseded by PDETextRunSetTextMatrixEx() in Acrobat 10.0.
	
	Sets the text matrix of a text run. 
	@param pdeText IN/OUT The text object containing a text run whose 
	text matrix is set. 
	@param runIndex IN/OUT The index of the text run. 
	@param matrixP IN/OUT A pointer to an ASFixedMatrix that holds the 
	text matrix. 
	@exception peErrWrongPDEObjectType 
	@exception genErrBadParm 
	@see PDETextGetTextMatrix 
	@see PDETextRunSetMatrix 
	@see PDETextRunSetTextMatrixEx
	@since PI_PDFEDIT_WRITE_VERSION >= 0x00040000

*/
#if ADD_ASFIXED //PKG
UNPROC(void, PDETextRunSetTextMatrix, (
			IN	PDEText pdeText, 
			IN	ASInt32 runIndex, 
			IN	ASFixedMatrixP matrixP)
	)

/**
	Superseded by PDETextRunSetStrokeMatrixEx() in Acrobat 10.0.
	
	Sets the stroke matrix of a text run. 
	@param pdeText The text object containing a text run whose 
	stroke matrix is set. 
	@param runIndex The index of the text run. 
	@param matrixP A pointer to an ASFixedMatrix that holds the 
	stroke matrix.
	@exception peErrWrongPDEObjectType 
	@exception genErrBadParm 
	@see PDETextGetStrokeMatrix
	@see PDETextRunSetStrokeMatrixEx

	@note Currently this method is not implemented (Acrobat 5 and later).
	@since PI_PDFEDIT_WRITE_VERSION >= 0x00040000
*/
UNPROC(void, PDETextRunSetStrokeMatrix, (
			IN	PDEText pdeText, 
			IN	ASInt32 runIndex, 
			IN	ASFixedMatrixP matrixP)
	)

/**
	Superseded by PDETextAddEx() in Acrobat 10.0.
	
	Adds a character or a text run to a PDEThe text object. 

	@note This method does not change the reference count of 
	<code>pdeText</code>; however, the reference count of the objects in 
	the <code>gstateP</code> parameter are incremented. 
	@param pdeText The text object to which a character or text 
	run is added. 
	@param flags A PDETextFlags that specifies what kind of 
	text to add. It must be one of the following values: 

	<TABLE rules="all" cellspacing="1">
	<TR><TH>Value</TH><TH>Description</TH></TR>
	<TR><TD>kPDETextChar</TD><TD>Used for a text character.</TD></TR>
	<TR><TD>kPDETextRun</TD><TD>Used for a text run.</TD></TR>
	</TABLE>

	@param index The index after which to add the character or text 
	run. 
	@param text A pointer to the characters to add. Note that passing 
	<code>NULL</code> for the text can invalidate the text object, but will not 
	raise an error. Callers must not pass <code>NULL</code> for this parameter. 
	
	@param textLen The length of the text in bytes. 
	@param font The font for the element. 
	@param gstateP A pointer to a PDEGraphicStateP structure 
	with the graphics state for the element. 
	@param gstateLen The length of the graphics state for the element.
	@param tstateP A pointer to a PDETextState structure with 
	the text state for the element. Note that PDFEdit ignores the <code>wasSetFlags</code> 
	flag of the PDETextState structure, so you must initialize 
	the PDETextState fields. 
	@param tstateLen The length of the text state for the element. 
	
	@param textMatrixP A pointer to an ASFixedMatrix that holds 
	the matrix for the element. 
	@param strokeMatrixP A pointer to an ASFixedMatrix that holds 
	the matrix for the line width when stroking text. It may be 
	<code>NULL</code>. Note that this field is not currently used (Acrobat 5 and later).
	@exception pdErrBadResMetrics 
	@exception peErrWrongPDEObjectType 
	@exception genErrBadParm 
	@see PDETextIsAtPoint 
	@see PDETextReplaceChars 
	@see PDETextSplitRunAt 
	@see PDETextAddEx
	@since PI_PDFEDIT_WRITE_VERSION >= 0x00040000
*/
UNPROC(void, PDETextAdd, (
			IN	PDEText pdeText, 
			IN	ASUns32 flags,			/* kPDETextRun, kPDETextChar */ 
			IN	ASInt32 index,
			IN	ASUns8 *text, 
			IN	ASInt32 textLen, 
			IN	PDEFont font,
			IN	PDEGraphicStateP gstateP,
			IN	ASUns32 gstateLen,
			IN	PDETextStateP tstateP,
			IN	ASUns32 tstateLen,
			IN	ASFixedMatrixP textMatrixP,
			IN	ASFixedMatrixP strokeMatrixP)
	)
#endif //ADD_ASFIXED //PKG

/**
	Removes characters or text runs from a text object. 
	
	@param pdeText IN/OUT The text object from which text is removed. 
	
	@param flags IN/OUT A PDETextFlags that specifies whether <code>index</code> 
	refers to the character offset from the beginning of the 
	text object or the index of the text run in the text object. 
	It must be one of the following values: 

	<TABLE rules="all" cellspacing="1">
	<TR><TH>Value</TH><TH>Description</TH></TR>
	<TR><TD>kPDETextChar</TD><TD>Used for a text character.</TD></TR>
	<TR><TD>kPDETextRun</TD><TD>Used for a text run.</TD></TR>
	</TABLE>

	@param index IN/OUT The index of the character or text run in <code>pdeText</code>. 
	
	@param count IN/OUT The number of characters or text runs to remove. 
	
	@exception peErrWrongPDEObjectType 
	@exception genErrBadParm 
	@exception pdErrBadResMetrics 
	@see PDETextAdd 
	@see PDETextReplaceChars 
	@see PDETextSplitRunAt 

	@note This method decrements the reference count of objects 
	associated with the <code>pdeText</code> in the graphic state and font. 
	@since PI_PDFEDIT_WRITE_VERSION >= 0x00040000

*/
UNPROC(void, PDETextRemove, (
			IN	PDEText pdeText, 
			IN	ASUns32 flags,			/* kPDETextRun, kPDETextChar */ 
			IN	ASInt32 index, 
			IN	ASInt32 count)
	)

/**
	Creates an empty text object. 

	<p>Call PDERelease() to dispose of the returned text object when 
	finished with it. </p>

	@return An empty text object. 
	@since PI_PDFEDIT_WRITE_VERSION >= 0x00040000

*/
UNPROC(PDEText, PDETextCreate,
			(void)
	)

/*------------------------------------------------------------------------
	PDEPath methods.
------------------------------------------------------------------------*/
			

/**
	Sets new path data for a path element. 
	@param path IN/OUT The path whose data is set. 
	@param data IN/OUT A pointer to the path data. It is a variable-sized 
	array of path operators and operands. The format is a 32-bit operator 
	followed by zero to three ASFixedPoint values, depending 
	on the operator. Operators are codes for <code>moveto</code>, <code>lineto</code>, 
	<code>curveto</code>, <code>rect</code>, or <code>closepath</code> operators, and must be one of 
	PDEPathElementType. Operands are ASFixedPoint values. The 
	data is copied into the PDEPath object. 
	@param dataSize IN/OUT The size of the new path data in bytes. 
	@exception peErrWrongPDEObjectType 
	@exception genErrBadParm 
	@see PDEPathGetData 
	@see PDEPathSetDataEx 
	@see PDEPathGetDataEx 
	@since PI_PDFEDIT_WRITE_VERSION >= 0x00040000

*/
#if ADD_ASFIXED //PKG
UNPROC(void, PDEPathSetData, (
			IN	PDEPath path, 
			IN	ASInt32 *data, 
			IN	ASUns32 dataSize)
	)
#endif //ADD_ASFIXED //PKG

/**
	Sets the fill and stroke attributes of a path. 
	@param path IN/OUT The path whose fill and stroke attributes are 
	set. 
	@param op IN/OUT The operation to set; it must be one of PDEPathOpFlags. 
	
	@exception peErrWrongPDEObjectType 
	@see PDEPathGetPaintOp 
	@since PI_PDFEDIT_WRITE_VERSION >= 0x00040000

*/
UNPROC(void, PDEPathSetPaintOp, (
			IN	PDEPath path, 
			IN	ASUns32 op)
	)

/**
	Creates an empty path element. 

	<p>Call PDERelease() to dispose of the returned path object when 
	finished with it. </p>
	@return An empty path element. 
	@since PI_PDFEDIT_WRITE_VERSION >= 0x00040000

*/
UNPROC(PDEPath, PDEPathCreate, (void)
	)

/*------------------------------------------------------------------------
	PDEImage methods.
------------------------------------------------------------------------*/

/* Image filtering -- see the notes in PERProcs.h
*/

/**
	Sets data for an image. 
	@param image IN/OUT The image whose data is set. 
	@param flags IN/OUT A set of PDEImageDataFlags flags. If kPDEImageEncodedData 
	is set, the data must be encoded for the current filters, 
	and <code>encodedLen</code> is the length of the encoded data. If the 
	kPDEImageEncodedData flag is not set, data is not encoded 
	and <code>encodedLen</code> is the size of the decoded data. 
	@param buffer IN/OUT The image data. 
	@param encodedLen IN/OUT The length of the encoded data. 
	@exception peErrUnknownPDEColorSpace 
	@exception genErrBadParm 
	@exception peErrWrongPDEObjectType 
	@see PDEImageGetData 
	@see PDEImageGetDataLen 
	@see PDEImageGetDataStm 
	@since PI_PDFEDIT_WRITE_VERSION >= 0x00040000

*/
UNPROC(void, PDEImageSetData, (
			IN	PDEImage image, 
			IN	ASUns32 flags,
			IN	ASUns8 *buffer,
			IN	ASUns32 encodedLen)
	)

/**
	Sets a data stream for an image. It can only be used for XObject 
	images. 

	<p>The caller must dispose of the stream by calling ASStmClose(). </p>
	
	@param image IN/OUT The image whose data stream is set. 
	@param flags IN/OUT PDEImageDataFlags flags. If the kPDEImageEncodedData 
	flag is set, the stream must be encoded. 
	@param filtersP IN/OUT A pointer to a PDEFilterArray structure. If 
	it is not <code>NULL</code>, it is used to build Cos objects for the Filter, 
	DecodeParms, and EncodeParms objects. If <code>filtersP</code> is 
	<code>NULL</code> and <code>kPDEImageEncodedData</code>	is set 
	in <code>flags</code>, the existing Filter and DecodeParms are used. 
	If <code>kPDEImageEncodedData</code> is not set	and <code>filtersP</code>
	is <code>NULL</code>, the existing Cos objects (if any) for Filter and 
	DecodeParms are removed and the resulting image is no longer compressed.
	EncodeParms is set it to DecodeParms if it exists (unless the filter is DCTDecode, 
	for which EncodeParms is mandatory).
	@param stm IN/OUT The stream for the image data. 
	@exception peErrUnknownPDEColorSpace 
	@exception peErrWrongPDEObjectType 
	@exception genErrBadParm 
	@see PDEImageGetData 
	@see PDEImageGetDataLen 
	@see PDEImageGetDataStm 
	@since PI_PDFEDIT_WRITE_VERSION >= 0x00040000

*/
UNPROC(void, PDEImageSetDataStm, (
			IN	PDEImage image, 
			IN	ASUns32 flags,
			IN	PDEFilterArrayP filtersP,
			IN	ASStm stm)
	)

/**
	Superseded by PDEImageCreateEx() in Acrobat 10.0.
	
	Creates an image object. 

	<p>The image data may be specified as a stream or as a buffer. 
	If <code>data</code> is non-<code>NULL</code>, <code>dataStm</code> is ignored. </p>

	<p>See PDEImageSetDataStm() for information on handling the stream. </p>
	
	<p>The caller must dispose of <code>dataStm</code> after calling this method.</p> 
	
	<p>Call PDERelease() to dispose of the returned image object 
	when finished with it. </p>

	@param attrsP IN/OUT A pointer to a PDEImageAttrs object with attributes 
	of the image. 
	@param attrsSize IN/OUT The size of the <code>attrsP</code> buffer in bytes. 
	@param matrixP IN/OUT A pointer to an ASFixedMatrix that holds the 
	transformation matrix to use for the image. 
	@param flags IN/OUT PDEImageDataFlags flags. If the kPDEImageEncodedData 
	flag is set, and the data is provided directly (not as a 
	stream), then <code>encodedLen</code> must specify the length of data. 
	
	@param colorSpace IN/OUT The color space of the image. When the image 
	is an image mask, <code>colorSpace</code> is the color space of the <code>colorValueP</code> 
	argument. 
	@param colorValueP IN/OUT A pointer to a PDEColorValue structure. 
	If the image is an image mask, <code>colorValueP</code> must be provided. 
	
	@param filtersP IN/OUT A pointer to a PDEFilterArray structure that 
	specifies which filters to use in encoding the contents; 
	it may be <code>NULL</code>. Filters will be used to encode the data in 
	the order in which they are specified in the array. 
	@param dataStm IN/OUT The stream holding the image data. 
	@param data IN/OUT The image data. If <code>data</code> is non-<code>NULL</code>, <code>dataStm</code> is 
	ignored. If there is a great deal of data, as for a large 
	image, it is recommended you use the <code>dataStm</code> parameter for 
	the image data or use the PDEImageCreateFromCosObj() method. 
	
	@param encodedLen IN/OUT The encoded length of <code>data</code> in bytes. 
	@return The image. 
	@exception peErrUnknownPDEColorSpace 
	@exception pageErrReadLessImageData 
	@exception peErrWrongPDEObjectType 
	@exception genErrBadParm 
	@see PDEImageCreateFromCosObj 
	@see PDEImageCreateEx
	@since PI_PDFEDIT_WRITE_VERSION >= 0x00040000

*/
#if ADD_ASFIXED //PKG
UNPROC(PDEImage, PDEImageCreate, (
			IN	PDEImageAttrsP attrsP, 
			IN	ASUns32 attrsSize,
			IN	ASFixedMatrixP matrixP,
			IN	ASUns32 flags,
			IN	PDEColorSpace colorSpace,
			IN	PDEColorValueP colorValueP,
			IN	PDEFilterArrayP filtersP,
			IN	ASStm dataStm, 
			IN	ASUns8 *data,
			IN	ASUns32 encodedLen)
	)

/**
	Superseded by PDEImageCreateFromCosObjEx() in Acrobat 10.0.
	
	Creates an image object from a Cos object. 

	<p>Call PDERelease() to dispose of the returned image object 
	when finished with it. </p>
	@param imageObjP IN/OUT The Cos object for the image. 
	@param matrixP IN/OUT A pointer to an ASFixedMatrix that holds the 
	transformation matrix to use for the image. 
	@param colorSpace IN/OUT The color space used for the image, if the 
	image is an image mask; otherwise, set it to <code>NULL</code>. 
	@param colorValueP IN/OUT A pointer to a PDEColorValue structure. 
	If the image is an image mask, <code>colorValueP</code> must be provided. 
	
	@return An image corresponding to the Cos object. 
	@exception peErrUnknownPDEColorSpace 
	@exception pageErrReadLessImageData 
	@exception peErrWrongPDEObjectType 
	@exception genErrBadParm 
	@see PDEImageCreate 
	@see PDEImageGetCosObj
	@see PDEImageCreateFromCosObjEx
	@since PI_PDFEDIT_WRITE_VERSION >= 0x00040000

*/
UNPROC(PDEImage, PDEImageCreateFromCosObj, (
			IN	const CosObj *imageObjP,
			IN	ASFixedMatrixP matrixP,
			IN	PDEColorSpace colorSpace,
			IN	PDEColorValueP colorValueP)
	)
#endif //ADD_ASFIXED //PKG


/*------------------------------------------------------------------------
	PDEClip methods.
------------------------------------------------------------------------*/

/* PDEClip objects can contain PDEContainers and PDEGroups to an  
** arbitrary level of nesting.  This allows PDEContainers to be used
** to mark clip objects.  
**
** PDEGroups inside PDEClips which contain at least one PDEText,
** and no PDEPaths have a special meaning.  All	PDEThe text objects contained 
** in such a PDEGroup are considered to be part of the same BT,ET 
** block.  This means that the union of these PDETexts makes up a single
** clipping path, as opposed to the intersection of the PDETexts.
*/

/**
	Adds an element to a clip path. 
	@param clip IN/OUT The clip path to which an element is added. 
	
	@param addAfterIndex IN/OUT The index after which to add <code>pdeElement</code>. 
	Use kPDEBeforeFirst to insert an element at the beginning 
	of the clip object. 
	@param pdeElement IN/OUT The element added, which may be a PDEPath, 
	a PDEText, a PDEContainer, a PDEGroup, or a PDEPlace object. 
	
	@exception peErrWrongPDEObjectType 
	@exception genErrBadParm 
	@see PDEClipRemoveElems 

	@note This method increments the reference count of <code>pdeElement</code>. 
	@since PI_PDFEDIT_WRITE_VERSION >= 0x00040000

*/
UNPROC(void, PDEClipAddElem, (
			IN	PDEClip clip, 
			IN	ASInt32 addAfterIndex, 		/* kPDEBeforeFirst */
			IN	PDEElement pdeElement)
	)

/**
	Removes one or more elements from a clip object. 

	@note This method decrements the reference count of each 
	of the elements. 
	@param clip IN/OUT The clip object from which an element is removed. 
	
	@param index IN/OUT The first element to remove. 
	@param count IN/OUT The number of elements to remove. 
	@exception peErrWrongPDEObjectType 
	@exception genErrBadParm 
	@see PDEClipAddElem 
	@since PI_PDFEDIT_WRITE_VERSION >= 0x00040000

*/
UNPROC(void, PDEClipRemoveElems, (
			IN	PDEClip clip, 
			IN	ASInt32 index, 
			IN	ASInt32 count)
	)

/**
	Creates an empty clip object. This represents a clipping 
	object that has no effect on elements that refer to it. 
	
	<p>Call PDERelease() to dispose of the returned clip object when 
	finished with it. </p>

	<p>It raises an exception if it is unable to allocate memory.</p>

	@return The newly created clip object. 
	@since PI_PDFEDIT_WRITE_VERSION >= 0x00040000

*/
UNPROC(PDEClip, PDEClipCreate, (void)
	)

/*------------------------------------------------------------------------
	PDEXObject methods.
------------------------------------------------------------------------*/

/**
	Creates a new PDEXObject from a Cos object. 

	<p>Call PDERelease() to dispose of the returned PDEXObject when 
	finished with it. </p>

	@param cosObjP IN/OUT The Cos object for the PDEXObject. 
	@return A PDEXObject corresponding to <code>cosObjP</code>. 
	@see PDEXObjectGetCosObj 
	@since PI_PDFEDIT_WRITE_VERSION >= 0x00040000

*/
UNPROC(PDEXObject, PDEXObjectCreate, (
			IN	const CosObj *cosObjP)
	)

/*------------------------------------------------------------------------
	PDEForm methods.
------------------------------------------------------------------------*/

/**
	Superseded by PDEFormCreateFromCosObjEx() in Acrobat 10.0.
	
	Creates a new form from an existing Cos object. 

	<p>Call PDERelease() to dispose of the returned form object when 
	finished with it. </p>

	@param xObjectP The Cos object from which a PDEForm is created. 
	
	@param resourcesP The <code>xObjectP</code> parameter's Resources dictionary. 
	If you do not pass in a Resource object, subsequent calls 
	to PDPageAcquirePDEContent() will fail (after the file is 
	saved). 
	@param matrixP A pointer to an ASFixedMatrix that holds the 
	transformation matrix to use for the form.
	@return The newly created form object. 
	@see PDEFormCreateClone 
	@see PDEFormGetCosObj 
	@see PDEFormCreateFromCosObjEx
	@since PI_PDFEDIT_WRITE_VERSION >= 0x00040000
*/
#if ADD_ASFIXED //PKG
UNPROC(PDEForm, PDEFormCreateFromCosObj, (
			IN	const CosObj *xObjectP,
			IN	const CosObj *resourcesP,
			IN	ASFixedMatrixP matrixP)
	)
#endif //ADD_ASFIXED //PKG

/**
	Gets a PDEContent object for a form. 
	
	@param form The form whose content is obtained.
	@return The content for <code>form</code>. 
	@exception peErrWrongPDEObjectType 
	@exception peErrPStackUnderflow 

	@note Unlike other <code>GetContent</code> methods, this method does 
	increment the reference count of the returned PDEContent. Call PDERelease() to dispose of the returned PDEContent object when finished with it.
	@since PI_PDFEDIT_WRITE_VERSION >= 0x00040000
*/
UNPROC(PDEContent, PDEFormGetContent, (
 			IN	PDEForm form)
	  )

					
/*------------------------------------------------------------------------
	PDEPS methods. (Passthrough PostScript)
------------------------------------------------------------------------*/

/**
	The following PDEPS methods have been deprecated and do nothing. 
*/
UNPROC(void, PDEPSSetData, (
			IN	PDEPS ps, 
			IN	ASUns8 *buffer, 
			IN	ASUns32 bufferSize)
	)

UNPROC(void, PDEPSSetDataStm, (
			IN	PDEPS ps, 
			IN	ASStm stm )
	)

UNPROC(PDEPS, PDEPSCreate, (
			IN	PDEPSAttrsP attrsP, 
			IN	ASUns32 attrsSize,
			IN	ASStm dataStm,
			IN	ASUns8 *data, 
			IN	ASUns32 dataSize)
	)

/**
	Creates a PDEPS object from a CosObj object. 
	<p>Call PDERelease() to dispose of the returned PDEPS object when finished with it. </p>

	@param cosObjP IN/OUT An object of type CosObj. 
	@return An object of type PDEPS. 
	@exception genErrBadParm 
	@see PDEPSCreate 
	@since PI_PDFEDIT_WRITE_VERSION >= 0x00020000

*/
UNPROC(PDEPS, PDEPSCreateFromCosObj, (
			const CosObj *cosObjP)
	)
			
/*------------------------------------------------------------------------
	PDEFont methods.
------------------------------------------------------------------------*/
#if 0

/**
	Creates a new PDEFont from the specified parameters. 

	<p>The PDEFont may be represented as an embedded font (a FontFile 
	entry in the font descriptor of the PDF file). To create 
	a PDEFont that is stored as an embedded font, the FontFile 
	stream may be passed in <code>fontStm</code>, and the <code>len1</code>, <code>len2</code>, and 
	<code>len3</code> parameters contain the Length1, Length2, and Length3 
	values of the FontFile stream attributes dictionary. See 
	Section 5.8 in the <i>PDF Reference</i> for more information about 
	embedded fonts. </p>

	<p>The caller must close <code>fontStm</code> with ASStmClose() after invoking 
	PDEFontCreate(). </p>

	<p>Call PDERelease() to dispose of the returned font object when 
	finished with it. </p>

	@param attrsP A pointer to a PDEFontAttrs structure for 
	the font attributes. 
	@param attrsSize The size of the <code>attrsP</code> buffer in bytes. 
	
	@param firstChar The first character index for the widths 
	array, <code>widthsP</code>. 
	@param lastChar The last character index for the widths array, 
	<code>widthsP</code>. 
	@param widthsP A pointer to the widths array. 
	@param encoding An array of 256 pointers to glyph names 
	specifying the custom encoding. If any pointer is <code>NULL</code>, 
	no encoding information is written for that entry. 
	@param encodingBaseName The encoding base name if the encoding 
	is a custom encoding. If the encoding is <code>NULL</code>, <code>encodingBaseName</code> 
	is used as the value of the encoding, and must be one of 
	<code>WinAnsiEncoding</code>, <code>MacRomanEncoding</code>, or <code>MacExpertEncoding</code>. 
	If no encoding value is desired, use ASAtomNull. 
	@param fontStm The stream with font information.
	@param len1 The length in bytes of the ASCII portion of the 
	Type 1 font file after it has been decoded. For other font 
	formats, such as TrueType or CFF, only <code>len1</code> is used, and 
	it is the size of the font. 
	@param len2 The length in bytes of the encrypted portion of 
	the Type 1 font file after it has been decoded. 
	@param len3 The length in bytes of the portion of the Type 
	1 font file that contains the 512 zeros, plus the <code>cleartomark</code> 
	operator, plus any following data. 
	@return The specified PDEFont. 
	@exception peErrCantCreateFontSubset 
	@exception peErrCantGetAttrs 
	@exception peErrCantGetWidths 
	@exception peErrCantEmbedFont 
	@exception genErrResourceLoadFailed 
	@see PDEFontCreateFromCosObj 
	@see PDEFontCreateFromSysFont 
	@see PDEFontCreateFromSysFontEx 
	@see PDEFontCreateFromSysFontWithParams 
	@see PDEFontCreateWithParams 
	@since PI_PDFEDIT_WRITE_VERSION >= 0x00040000
*/
UNPROC(PDEFont, PDEFontCreate, (
			IN	PDEFontAttrsP attrsP,
			IN	ASUns32 attrsSize,
			IN	ASInt32 firstChar,
			IN	ASInt32 lastChar,
			IN	ASInt16 *widthsP, 
			IN	char **encoding,
			IN	ASAtom encodingBaseName, 
			IN	ASStm fontStm,
			IN	ASInt32 len1,
			IN	ASInt32 len2,
			IN	ASInt32 len3)
	)

/**
	Creates a PDEFont corresponding to a Cos object of type 
	Font. 

	<p>Call PDERelease() to dispose of the returned font object when 
	finished with it. </p>

	@param cosObjP IN/OUT The Cos object for which a PDEFont is created. 
	
	@return The PDEFont created from cosObjP. 
	@exception peErrCantCreateFontSubset 
	@exception peErrCantGetAttrs 
	@exception peErrCantGetWidths 
	@exception peErrCantEmbedFont 
	@exception genErrBadParm 
	@exception genErrResourceLoadFailed 
	@see PDEFontCreate 
	@see PDEFontCreateFromSysFont 
	@see PDEFontCreateFromSysFontWithParams 
	@see PDEFontCreateWithParams 
	@see PDEFontGetCosObj 
	@since PI_PDFEDIT_WRITE_VERSION >= 0x00040000

*/
#endif

UNPROC(PDEFont, PDEFontCreateFromCosObj, (
			const CosObj *cosObjP)
	)

#if 0 //PKG
/**
	Gets a PDEFont corresponding to a font in the system. 

	<p>Call PDERelease() to dispose of the returned font object when 
	finished with it. </p>

	<p>The PDEFontCreateFlags flags kPDEFontCreateEmbedded and 
	kPDEFontWillSubset must both be set in order to subset a 
	font.</p> 

	<p>If you create a PDEFont that is a subset, call PDEFontSubsetNow() 
	on this font afterwards. </p>
	@param sysFont A PDSysFont object referencing a system 
	font. 
	@param flags Indicates whether to embed the font and whether 
	to subset the font. It must be one of PDEFontCreateFlags. If 
	you want to subset a font, set both the kPDEFontCreateEmbedded 
	and kPDEFontWillSubset flags.
	@return The PDEFont corresponding to sysFont. 
	@exception peErrCantCreateFontSubset 
	@exception peErrCantGetAttrs 
	@exception peErrCantGetWidths 
	@exception peErrCantEmbedFont 
	@exception genErrBadParm 
	@exception genErrResourceLoadFailed 
	@see PDEFontCreate 
	@see PDEFontCreateFromCosObj 
	@see PDEFontCreateFromSysFontAndEncoding 
	@see PDEFontCreateFromSysFontWithParams 
	@see PDEnumSysFonts 

	@note If you have an environment with no Acrobat Language kit 
	installed, trying to acquire a PDEFont from the system font 
	may raise an exception for some of the operating system fonts. In other 
	words, if you use PDEnumSysFonts() to get the system font 
	attributes, not all of the system fonts will necessarily 
	be used to create the PDEFont. 

	@note If you want to use <code>WinAnsiEncoding</code> on UNIX, do not 
	use this method. Use PDEFontCreateFromSysFontWithParams() 
	or PDEFontCreateFromSysFontAndEncoding() instead. 
	@since PI_PDFEDIT_WRITE_VERSION >= 0x00040000
*/
UNPROC(PDEFont, PDEFontCreateFromSysFont, (
			IN	PDSysFont sysFont,
			IN	ASUns32 flags)
	)			
#endif			
			
/*------------------------------------------------------------------------
	PDEColorSpace methods.
------------------------------------------------------------------------*/

/**
	Creates a new color space object. 

	<p>Call PDERelease() to dispose of the returned color space object 
	when finished with it. </p>

	@param name IN/OUT The ASAtom for the name of the color space 
	created. The name must be one of the following: DeviceCMYK, 
	DeviceGray, or DeviceRGB. 
	@return The newly created color space object. 
	@exception cosErrExpectedName 
	@exception genErrBadParm 
	@exception peErrUnknownPDEColorSpace 
	@see PDEColorSpaceCreate 
	@see PDEColorSpaceCreateFromCosObj 
	@since PI_PDFEDIT_WRITE_VERSION >= 0x00040000

*/
UNPROC(PDEColorSpace, PDEColorSpaceCreateFromName, (
			IN	ASAtom	name)
	)

/**
	Creates a new color space object from a Cos object. 

	<p>Call PDERelease() to dispose of the returned color space object 
	when finished with it. </p>

	@param cosObjP IN/OUT Supports all PDF 1.3 color spaces, which 
	include: 

	<TABLE rules="all" cellspacing="1">
	<TR><TH>Type of names</TH><TH>Names</TH></TR>
	<TR><TD>Device-dependent names</TD><TD><ul><li><code>DeviceCMYK</code></li><li><code>DeviceGray</code></li><li><code>DeviceN</code></li><li><code>DeviceRGB</code></li></ul></TD></TR>
	<TR><TD>Device-independent names</TD><TD><ul><li><code>CalGray</code></li><li><code>CalRGB</code></li><li><code>Lab</code></li><li><code>ICCBased</code></li></ul></TD></TR>
	<TR><TD>Special names</TD><TD><ul><li><code>Indexed</code></li><li><code>Pattern</code></li><li><code>Separation</code></li></ul></TD></TR>
	</TABLE>

	@return The newly created color space object. 
	@exception cosErrExpectedArray 
	@exception genErrBadParm 
	@exception peErrUnknownPDEColorSpace 
	@see PDEColorSpaceCreate 
	@see PDEColorSpaceCreateFromName 
	@since PI_PDFEDIT_WRITE_VERSION >= 0x00040000

*/
UNPROC(PDEColorSpace, PDEColorSpaceCreateFromCosObj, (
			IN	const CosObj *cosObjP)
	)
/*------------------------------------------------------------------------
	Client Tag methods.
------------------------------------------------------------------------*/

/**
	Adds an identifier-value pair to an object. 

	<p>The clientID-tag combination is a unique identifier for 
	the value. Each client has its own identifier space. It 
	is often convenient to use ASAtoms as tags. </p>
	
	@param object The element to tag. The object may be a PDEElement, 
	PDEContent, PDEFont, PDEColorSpace, and so on. 
	@param clientID Identifies the caller/client. For clients, 
	this should be the gExtensionID extension. For the Adobe 
	PDF Library, if there is only one client of the PDFEdit 
	subsystem, <code>clientID</code> should be zero. If there are multiple 
	clients, each should specify a nonzero, non-negative <code>clientID</code>. 
	(A negative <code>clientID</code> is reserved for the implementation.) 
	
	@param tag The tag to add to <code>object</code>. If <code>tag</code> is <code>0</code>, this 
	is the same as calling PDERemoveTag(). In other words, you 
	cannot tell the difference between a tag whose value is 
	zero and a tag that is nonexistent. 
	@param value A pointer to a value to associate with <code>object</code>. 
	Only the pointer is stored. If the pointer points to data, 
	it is the responsibility of the client to manage the data 
	and its memory.
	@exception peErrWrongPDEObjectType 
	@see PDEGetTag 
	@see PDERemoveTag 

	@note Tags are a purely memory-resident feature. In addition, 
	management of tags is the responsibility of the client. 
	A client must manage any memory pointed to by a tag. This 
	method only contains a pointer to the data passed in by 
	the client. The data and the pointer will not be saved to 
	a file. The generic pointer type is not in the PDF specification. 
	@since PI_PDFEDIT_WRITE_VERSION >= 0x00040000
*/
UNPROC(void, PDEAddTag, (
			IN	PDEObject object,
			IN	ExtensionID clientID,
			IN	ASUns32 tag,
			IN	void *value)
	)

/**
	Gets an object's value for a given clientID-tag identifier 
	that was added by PDEAddTag. 
	@param object The element whose value is obtained. 
	@param clientID Identifies the caller/client. For clients, 
	this should be the gExtensionID extension. For the Adobe 
	PDF Library, if there is only one client of the PDFEdit 
	subsystem, <code>clientID</code> should be zero. If there are multiple 
	clients, each should specify a nonzero, non-negative <code>clientID</code>. 
	(A negative <code>clientID</code> is reserved for the implementation.) 
	
	@param tag The object's tag. If object has no tag, this 
	is <code>0</code>.
	@return The value associated with the clientID-tag identifier. 
	@exception peErrWrongPDEObjectType 
	@see PDEAddTag 
	@see PDERemoveTag 
	@since PI_PDFEDIT_WRITE_VERSION >= 0x00040000
*/
UNPROC(void *, PDEGetTag, (
			IN	PDEObject object,
			IN	ExtensionID clientID,
			IN	ASUns32 tag)
	)

/**
	Removes an object's value for a given clientID-tag identifier 
	that was added by PDEAddTag. 

	<p>If PDEAddTag is called with a <code>0</code> tag, this is the same as 
	calling PDERemoveTag(). </p>
	@param object The element whose tag is removed. 
	@param clientID Identifies the caller/client. For clients, 
	this should be the gExtensionID extension. For the Adobe 
	PDF Library, if there is only one client of the PDFEdit 
	subsystem, <code>clientID</code> should be zero. If there are multiple 
	clients, each should specify a nonzero, non-negative <code>clientID</code>. 
	(A negative <code>clientID</code> is reserved for the implementation.) 
	
	@param tag The tag value.
	@exception peErrWrongPDEObjectType 
	@see PDEAddTag 
	@see PDEGetTag 
	@since PI_PDFEDIT_WRITE_VERSION >= 0x00040000
*/
UNPROC(void, PDERemoveTag, (
			IN	PDEObject object,
			IN	ExtensionID clientID,
			IN	ASUns32 tag)
	)

/*------------------------------------------------------------------------
	Utility methods.
------------------------------------------------------------------------*/

/**
	Merges two Resources dictionaries in the same CosDoc; you 
	cannot merge two resource dictionaries from different CosDocs. 
	
	<p>Both dictionaries and what they reference must be in <code>cosDoc</code>. 
	The objects referenced by <code>newResP</code> are appended to <code>resDictP</code>. </p>
	
	<p>This method only operates on the Cos dictionaries. It assumes 
	there are no resource name conflicts. </p>

	<p>This method is useful for adding form resources to page 
	resource dictionaries. This is only necessary if creating 
	PDF 1.1 or earlier files for use with Acrobat 2.1 or earlier. 
	This is unnecessary if creating PDF 1.2 or later documents. </p>
	
	@param resDictP IN/OUT (Filled by the method) The dictionary to which 
	<code>newResP</code> is merged. When the method completes, <code>resDictP</code> is 
	the merged dictionary result. 
	@param cosDoc IN/OUT The CosDoc containing both dictionaries. 
	@param newResP IN/OUT The dictionary to merge with <code>resDictP</code>. 
	@exception genErrBadParm 

	@note Since PDFEdit resolves resource names across PDEContent 
	objects, this routine is safe for using with PDFEdit methods. 
	This method may be unsafe if you modify streams and dictionaries 
	outside of the PDFEdit API. 
	@since PI_PDFEDIT_WRITE_VERSION >= 0x00040000

*/
#if 0 //PKG
UNPROC(void, PDEMergeResourcesDict, (
			OUT	CosObj *resDictP,
			IN	CosDoc cosDoc,
			IN	const CosObj *newResP)
	)
#endif //ADD_ASFIXED //PKG

/*------------------------------------------------------------------------
	Notification methods.
------------------------------------------------------------------------*/


/*------------------------------------------------------------------------
	Methods added after version 0.2
------------------------------------------------------------------------*/

/**
	Creates a new PDEExtGState from a Cos object. See Section 
	4.3.4 in the <i>PDF Reference</i> for more information about extended 
	graphics states. 

	<p>Call PDERelease() to dispose of the returned PDEExtGState 
	when finished with it. </p>

	@param cosObjP A Cos object for a dictionary of type ExtGState.
	@return The PDEExtGState for <code>cosObjP</code>. 
	@see PDEElementSetGState 
	@see PDEExtGStateGetCosObj 
	@since PI_PDFEDIT_WRITE_VERSION >= 0x00040000
*/
UNPROC(PDEExtGState, PDEExtGStateCreate, (
			IN CosObj *cosObjP)
	)

/**
	Creates a place object. 

	<p>Call PDERelease() to dispose of the returned place object 
	when finished with it. </p>

	@param mcTag IN/OUT The tag name for the place. It must not contain any 
	white space characters (for example, spaces or tabs). 
	@param cosObjP IN/OUT An optional Marked Content dictionary associated 
	with the place. 
	@param isInline If <code>true</code>, it emits the place's dictionary 
	into the content stream inline. If <code>false</code>, then the dictionary 
	is emitted outside of the content stream and referenced by name.  
	See the Property Lists section in the <i>PDF Reference</i> for more details.
	@return The place object. 
	@since PI_PDFEDIT_WRITE_VERSION >= 0x00040000

*/
UNPROC(PDEPlace, PDEPlaceCreate, (
			IN ASAtom mcTag,
			IN CosObj *cosObjP,
			IN ASBool isInline)
	)

/**
	Sets the Marked Content tag for a PDEPlace. 
	@param pdePlace IN/OUT The place whose Marked Content tag is set. 
	
	@param mcTag IN/OUT The tag for <code>pdePlace</code>. 
	@exception peErrWrongPDEObjectType 
	@see PDEPlaceGetMCTag 
	@since PI_PDFEDIT_WRITE_VERSION >= 0x00040000

*/
UNPROC(void, PDEPlaceSetMCTag, (
			IN	PDEPlace pdePlace,
			IN	ASAtom mcTag)
	)

/** 
	Sets the Marked Content dictionary for a PDEPlace. The dictionary can be 
	emitted inline or referenced from the <code>\\Properties</code> key in the <code>\\Resources</code> 
	dictionary of the containing stream.

	@param pdePlace IN/OUT The place whose Marked Content dictionary 
	is set. 
	@param placeDictP IN/OUT The Marked Content dictionary for <code>pdePlace</code>. 
	
	@param isInline If <code>true</code>, it emits the place's dictionary 
	into the content stream inline. If <code>false</code>, then the dictionary 
	is emitted outside of the content stream and referenced by name.  
	See the <i>Property Lists</i> section in the <i>PDF Reference</i> for more details. 
	
	@exception peErrWrongPDEObjectType 
	@see PDEPlaceGetDict 
	@since PI_PDFEDIT_WRITE_VERSION >= 0x00040000

*/
UNPROC(void, PDEPlaceSetDict, (
			IN	PDEPlace pdePlace,
			IN	CosObj *placeDictP,
			IN	ASBool isInline)
	)

/**
	Creates a container object. 

	<p>Call PDERelease() to dispose of the returned container object 
	when finished with it. </p>
	@param mcTag IN/OUT The tag name for the container. 
	@param cosObjP IN/OUT An optional Marked Content dictionary for the 
	container. 
	@param isInline If <code>true</code>, it emits the container's dictionary 
	into the content stream inline. If <code>false</code>, then the dictionary 
	is emitted outside of the content stream and referenced by name. 
	See the Property Lists section in the <i>PDF Reference</i> for more details.
	@return The newly created container object. 
	@exception pdErrOpNotPermitted 
	@see PDEContainerGetMCTag 
	@see PDEContainerSetMCTag 
	@since PI_PDFEDIT_WRITE_VERSION >= 0x00040000

*/
UNPROC(PDEContainer, PDEContainerCreate, (
			IN ASAtom mcTag,
			IN CosObj *cosObjP,
			IN ASBool isInline)
	)

/**
	Sets the Marked Content tag for a PDEContainer. 
	@param pdeContainer IN/OUT The container to tag. 
	@param mcTag IN/OUT The Marked Content tag. 
	@exception peErrWrongPDEObjectType 
	@exception genErrBadParm 
	@see PDEContainerCreate 
	@see PDEContainerGetMCTag 
	@since PI_PDFEDIT_WRITE_VERSION >= 0x00040000

*/
UNPROC(void, PDEContainerSetMCTag, (
			IN	PDEContainer pdeContainer,
			IN	ASAtom mcTag)
	)

/**
	Sets the Marked Content dictionary for a PDEContainer. The dictionary can be emitted inline 
	or referenced from the <code>\\Properties</code> key in the <code>\\Resources</code> 
	dictionary of the containing stream.
 
	@param pdeContainer The container whose dictionary is 
	changed. 
	@param placeDictP The Marked Content dictionary being set 
	into <code>pdeContainer</code>. 
	@param isInline If <code>true</code>, it emits the container's dictionary 
	into the content stream inline. If <code>false</code>, then the dictionary 
	is emitted outside of the content stream and referenced by name.  
	See the <i>Property Lists</i> section in the <i>PDF Reference</i> for more details.
	@exception peErrWrongPDEObjectType 
	@exception genErrBadParm 
	@see PDEContainerGetDict 
	@since PI_PDFEDIT_WRITE_VERSION >= 0x00040000
*/
UNPROC(void, PDEContainerSetDict, (
			IN	PDEContainer pdeContainer,
			IN	CosObj *placeDictP,
			IN	ASBool isInline)
	)

/**
	Sets the content for a container. The existing PDEContent 
	is released by this method. 

	@note This method decrements the reference count of the 
	previous content of the container and increments the reference 
	count of the new PDEContent. 
	@param pdeContainer IN/OUT A container. 
	@param pdeContent IN/OUT The content of <code>pdeContainer</code>. 
	@exception peErrWrongPDEObjectType 
	@exception genErrBadParm 
	@see PDEContainerGetContent 
	@since PI_PDFEDIT_WRITE_VERSION >= 0x00040000

*/
UNPROC(void, PDEContainerSetContent, (
			IN	PDEContainer pdeContainer,
			IN	PDEContent pdeContent)
	)

/**
	Splits a text run into two text runs. 
	@param pdeText The text object containing a text run to split. 
	
	@param splitLoc The split location, relative to the text object. 
	The first text run is from character index <code>0</code> up to <code>splitLoc</code>. 
	The second text run is from <code>splitLoc + 1</code> to the end of the 
	run. 
	@exception peErrWrongPDEObjectType 
	@exception genErrBadParm 
	@exception pdErrBadResMetrics 
	@see PDETextIsAtPoint 
	@see PDETextReplaceChars 
	@since PI_PDFEDIT_WRITE_VERSION >= 0x00040000
*/
UNPROC(void, PDETextSplitRunAt, (
			IN	PDEText 	pdeText, 
			IN	ASInt32 		splitLoc)
	)

/**
	Creates a pattern object that can be used for a Pattern 
	color space. See Section 4.6 in the <i>PDF Reference</i> for more 
	information about patterns. 

	<p>Call PDERelease() to dispose of the returned pattern object 
	when finished with it. </p>
	@param cosObjP IN/OUT A CosObj stream for the pattern. 
	@return A pattern. 
	@see PDEPatternGetCosObj 
	@since PI_PDFEDIT_WRITE_VERSION >= 0x00040000

*/
UNPROC(PDEPattern, PDEPatternCreate, (
			const CosObj *cosObjP)
	)

#if 0
/**
	Replaces characters in a text object. 

	<p>This method does not change the number of characters in 
	the text object; extra characters are ignored. </p>

	@param pdeText IN/OUT The text object in which characters are replaced. 
	
	@param flags IN/OUT A PDETextFlags that specifies whether <code>index</code> 
	refers to the character offset from the beginning of the 
	text object or the index of the text run in the text object. 
	It must be one of the following values: 

	<TABLE rules="all" cellspacing="1">
	<TR><TH>Value</TH><TH>Description</TH></TR>
	<TR><TD>kPDETextChar</TD><TD>Used for a text character.</TD></TR>
	<TR><TD>kPDETextRun</TD><TD>Used for a text run.</TD></TR>
	</TABLE>

	@param index IN/OUT The index of the character or text run in <code>pdeText</code>. 
	
	@param textBuffer IN/OUT Replacement text. 
	@param numChars IN/OUT The number of bytes to replace. 
	@exception peErrWrongPDEObjectType 
	@exception genErrBadParm 
	@see PDETextAdd 
	@see PDETextIsAtPoint 
	@see PDETextSplitRunAt 
	@since PI_PDFEDIT_WRITE_VERSION >= 0x00040000

*/
UNPROC(void, PDETextReplaceChars, (
			IN	PDEText pdeText, 
			IN	ASUns32 flags,			/* kPDETextRun, kPDETextChar */ 
			IN	ASInt32 index, 
			IN	ASUns8 *textBuffer,
			IN	ASInt32 numChars)
	)

/**
	Clears the PDE Cache of this PDDoc. This method is only 
	of interest to clients. 

	@note It is not recommended that you call this method directly; 
	it is provided only for backwards compatibility. 
	@param doc A PDDoc whose cache is purged.
	@since PI_PDFEDIT_WRITE_VERSION >= 0x00040000
*/
UNPROC(void, PDEPurgeCache, (
			IN	PDDoc doc)
	)

/**
	Subsets a given PDEFont in a CosDoc. 

	<p>If you created font with PDEFontCreateFromSysFont(), you must 
	have set both the kPDEFontCreateEmbedded and kPDEFontWillSubset 
	set in the <code>flags</code> parameter, to be able to subset the font. </p>
	@param font IN/OUT The PDEFont to subset. 
	@param cosDoc IN/OUT The CosDoc whose font is subsetted. 
	@exception peErrCantCreateFontSubset 
	@exception peErrCantGetAttrs 
	@exception peErrCantGetWidths 
	@exception peErrCantEmbedFont 
	@exception genErrBadParm 
	@exception genErrResourceLoadFailed 
	@see PDEFontCreateFromSysFont 

	@note This method does not change the reference count. 
	@since PI_PDFEDIT_WRITE_VERSION >= 0x00040000

*/
UNPROC(void, PDEFontSubsetNow, (
			IN	PDEFont	font,
			IN	CosDoc	cosDoc)
	)
#endif

/**
	Adds a segment to a path. The number of ASReal values used 
	depends upon <code>segType</code>:

	<TABLE rules="all" cellspacing="1">
	<TR><TH><code>segType</code></TH><TH>ASReal values</TH></TR>
	<TR><TD>kPDEMoveTo</TD><TD><ul><li><code>x1</code></li><li><code>y1</code></li></ul></TD></TR>
	<TR><TD>kPDELineTo</TD><TD><ul><li><code>x1</code></li><li><code>y1</code></li></ul></TD></TR>
	<TR><TD>kPDECurveTo</TD><TD><ul><li><code>x1</code></li><li><code>y1</code></li><li><code>x2</code></li><li><code>y2</code></li><li><code>y3</code></li></ul></TD></TR>
	<TR><TD>kPDECurveToV</TD><TD><ul><li><code>x1</code></li><li><code>y1</code></li><li><code>x2</code></li><li><code>y2</code></li></ul></TD></TR>
	<TR><TD>kPDECurveToY</TD><TD><ul><li><code>x1</code></li><li><code>y1</code></li><li><code>x2</code></li><li><code>y2</code></li></ul></TD></TR>
	<TR><TD>kPDERect</TD><TD><ul><li><code>x1</code></li><li><code>y1</code></li><li><code>x2</code> (width)</li><li><code>y2</code> (height)</li></ul></TD></TR>
	<TR><TD>kPDEClosePath</TD><TD>None</TD></TR>
	</TABLE>

	@param path IN/OUT The path to which a segment is added. 
	@param segType IN/OUT A PDEPathElementType value indicating the type 
	of path to add. 
	@param x1 IN/OUT The x-coordinate of the first point. 
	@param y1 IN/OUT The y-coordinate of the first point. 
	@param x2 IN/OUT The x-coordinate of the second point. 
	@param y2 IN/OUT The y-coordinate of the second point. 
	@param x3 IN/OUT The x-coordinate of the third point. 
	@param y3 IN/OUT The y-coordinate of the third point. 
	@exception genErrBadParm 
	@see PDEPathSetData 
	@since PI_PDFEDIT_WRITE_VERSION >= 0x00040000

*/
UNPROC(void, PDEPathAddSegment, (
			IN	PDEPath path, 
			IN	ASUns32 segType, 
			IN	ASReal	x1,
			IN	ASReal	y1,
			IN	ASReal	x2,
			IN	ASReal	y2,
			IN	ASReal	x3,
			IN	ASReal	y3)
	)

UNPROC(void, PDEPathAddSegmentEx, (
			IN	PDEPath path, 
			IN	ASUns32 segType, 
			IN	ASReal	x1,
			IN	ASReal	y1,
			IN	ASReal	x2,
			IN	ASReal	y2,
			IN	ASReal	x3,
			IN	ASReal	y3)
	)

/*------------------------------------------------------------------------
	Methods added after version 0.9
------------------------------------------------------------------------*/

#if 0
/**
	Creates a new PDEFont from <code>params</code>. 

	<p>The PDEFont may be represented as an embedded font (a FontFile 
	value in PDF). To create a PDEFont that will be stored as 
	an embedded font, the FontFile stream may be passed as <code>fontStm</code>, 
	and the <code>len1</code>, <code>len2</code>, and <code>len3</code> parameters contain the Length1, 
	Length2, and Length3 values of the FontFile. The caller 
	must close the <code>fontStm</code> after calling this method. This method 
	supports multi-byte fonts. </p>

	<p>This method extends PDEFontCreate() to support multi-byte 
	fonts. </p>

	<p>Call PDERelease() to dispose of the returned font object when 
	finished with it. </p>

	@param params IN/OUT A pointer to a structure containing all font 
	parameters necessary to fully define a font. 
	@return A PDEFont object of the font described by the parameters. 
	
	@exception peErrCantCreateFontSubset 
	@exception peErrCantGetAttrs 
	@exception peErrCantGetWidths 
	@exception peErrCantEmbedFont 
	@exception genErrBadParm 
	@exception genErrResourceLoadFailed 
	@see PDEFontCreate 
	@see PDEFontCreateFromCosObj 
	@see PDEFontCreateFromSysFont 
	@see PDEFontCreateFromSysFontEx 
	@since PI_PDFEDIT_WRITE_VERSION >= 0x00040000

*/
UNPROC(PDEFont, PDEFontCreateWithParams, (
			IN	PDEFontCreateParams params)
	)
#endif

/**
	Creates a new color space object of the specified type. 
	
	<p>Call PDERelease() to dispose of the returned color space object 
	when finished with it. </p>

	@param family IN/OUT Supports all PDF 1.3 color spaces, which 
	include: 

	<TABLE rules="all" cellspacing="1">
	<TR><TH>Type of names</TH><TH>Names</TH></TR>
	<TR><TD>Device-dependent names</TD><TD><ul><li><code>DeviceCMYK</code></li><li><code>DeviceGray</code></li><li><code>DeviceN</code></li><li><code>DeviceRGB</code></li></ul></TD></TR>
	<TR><TD>Device-independent names</TD><TD><ul><li><code>CalGray</code></li><li><code>CalRGB</code></li><li><code>Lab</code></li><li><code>ICCBased</code></li></ul></TD></TR>
	<TR><TD>Special names</TD><TD><ul><li><code>Indexed</code></li><li><code>Pattern</code></li><li><code>Separation</code></li></ul></TD></TR>
	</TABLE>

	@param csStruct IN/OUT Data for the type of color space you want 
	to create. 
	@return The newly created color space object. 
	@exception cosErrExpectedArray 
	@exception genErrBadParm 
	@exception peErrUnknownPDEColorSpace 
	@see PDEColorSpaceCreateFromCosObj 
	@since PI_PDFEDIT_WRITE_VERSION >= 0x00040000

*/
#if 0 //PKG
UNPROC(PDEColorSpace, PDEColorSpaceCreate, (
			ASAtom family, PDEColorSpaceStruct *csStruct)
	)
#endif //ADD_ASFIXED //PKG

/**
	Creates an object that can be used to store <code>n</code> color components 
	when in a DeviceN color space. 
	<p>Call PDERelease() to dispose of the returned PDEDeviceNColors object when finished with it. </p>

	@param pColorValues IN/OUT A pointer to an array of ASReal values. 
	
	@param numValues IN/OUT The length of the array. 
	@return An object containing values specifying a color in a PDEDeviceNColors 
	color space. 
	@exception genErrNoMemory 
	@see PDEDeviceNColorsGetColorValue 
	@since PI_PDFEDIT_WRITE_VERSION >= 0x00040000

*/
UNPROC(PDEDeviceNColors, PDEDeviceNColorsCreate, (
			IN ASReal *pColorValues,
			IN ASInt32 numValues)
	)

/**
	Superseded by PDEShadingCreateFromCosObjEx() in Acrobat 10.0.
	
	Creates a smooth shading object. 
	<p>Call PDERelease() to dispose of the returned PDEShading object when finished with it. </p>
	@param shadingP IN/OUT The shading dictionary. 
	@param matrixP IN/OUT The location and transformation matrix of 
	the shading object. 
	@return A smooth shading object. 
	@exception peErrUnknownPDEColorSpace 
	@exception cosErrInvalidObj 
	@exception cosErrExpectedName 
	@exception genErrBadParm 
	@see PDEShadingCreateFromCosObjEx
	@since PI_PDFEDIT_WRITE_VERSION >= 0x00040000

*/
#if ADD_ASFIXED //PKG
UNPROC(PDEShading, PDEShadingCreateFromCosObj, (
			IN	const CosObj *shadingP,
			IN	ASFixedMatrixP matrixP)
	)
#endif //ADD_ASFIXED //PKG

/**
	Creates a PDEGroup object. 
	<p>Call PDERelease() to dispose of the returned PDEGroup object when finished with it. </p>
	@return The newly created PDEGroup. 
	@see PDEGroupSetContent 
	@since PI_PDFEDIT_WRITE_VERSION >= 0x00040000

*/
UNPROC(PDEGroup, PDEGroupCreate, (void))

/**
	Sets the PDEContent for a PDEGroup. The existing PDEContent 
	is released by this method. 

	@note This method increments the reference count of <code>pdeContent</code>. 
	
	@param pdeGroup IN/OUT A container object. 
	@param pdeContent IN/OUT The content to set for <code>pdeGroup</code>. 
	@exception peErrWrongPDEObjectType 
	@see PDEGroupGetContent 
	@since PI_PDFEDIT_WRITE_VERSION >= 0x00040000

*/
UNPROC(void, PDEGroupSetContent, (
			IN	PDEGroup pdeGroup,
			IN	PDEContent pdeContent)
	)


#if 0
/**
	Creates a PDEFont corresponding to a font in the system. 
	
	<p>If the font is a Multiple Master font, <code>mmDesignVector</code> points 
	to the design vector, whose length must equal the number 
	of design axes of the font. </p>

	<p>Call PDERelease() to dispose of the returned font object when 
	finished with it. </p>

	<p>The PDEFontCreateFlags flags kPDEFontCreateEmbedded and 
	kPDEFontWillSubset must both be set in order to subset a 
	font. </p>

	<p>If you create a PDEFont that is subsetted, call PDEFontSubsetNow() 
	on this font afterwards. </p>

	@param sysFont IN/OUT A PDSysFont object referencing a system 
	font. 
	@param flags IN/OUT Indicates whether to embed the font and whether 
	to subset the font. It must be one of PDEFontCreateFlags. If 
	you want to subset a font, set both the kPDEFontCreateEmbedded 
	and kPDEFontWillSubset flags. 
	@param snapshotName IN/OUT The name to be associated with this particular 
	instantiation of the PDEFont. 
	@param mmDesignVec IN/OUT A pointer to the Multiple Master font design 
	vector. 
	@return The PDEFont corresponding to <code>sysFont</code>. 
	@exception peErrCantCreateFontSubset 
	@exception peErrCantGetAttrs 
	@exception peErrCantGetWidths 
	@exception peErrCantEmbedFont 
	@exception genErrBadParm 
	@exception genErrResourceLoadFailed 
	@see PDEFontCreateFromCosObj 
	@see PDEFontCreateFromSysFont 
	@see PDEFontCreateFromSysFontWithParams 
	@see PDEFontCreateWithParams 
	@see PDEnumSysFonts 

	@note If you have environment with no Acrobat Language kit 
	installed, trying to acquire a PDEFont from the system font 
	may raise an exception for some of the system fonts. In 
	other words, if you use PDEnumSysFonts() to get the system 
	font attributes, not all of the system fonts are necessarily 
	used to create the PDEFont. 
	@since PI_PDFEDIT_WRITE_VERSION >= 0x00040000

*/
UNPROC(PDEFont, PDEFontCreateFromSysFontEx, (
			IN	PDSysFont sysFont,
			IN	ASUns32 flags,
			IN	ASAtom snapshotName,
			IN	ASReal *mmDesignVec)
	)
#endif

/**
	Makes a deep copy of a PDEClip object. 

	<p>Call PDERelease() to dispose of the returned clip object when 
	finished with it. </p>

	<p>It raises an exception if it is unable to allocate memory.</p>
	@param srcClip IN/OUT The clipping path to copy. 
	@return The deep copy of <code>srcClip</code>. 
	@since PI_PDFEDIT_WRITE_VERSION >= 0x00040000

*/
UNPROC(PDEClip, PDEClipCopy, (
			IN	PDEClip srcClip)
	)

/**
	Sets the decode array of an image. 

	<p>Normally, the decode array is accessed through the <code>decode</code> 
	field in the PDEImageAttrs structure. However, this method 
	defines a decode array to handle images with a color space 
	that has more than four components. </p>

	@param image The image whose decode array is set. 
	@param decode A pointer to the decode array. 
	@param decodeSize The number of elements in the decode array.
	@see PDEImageGetDecodeArray 
	@see PDEImageGetFilterArray 
	@since PI_PDFEDIT_WRITE_VERSION >= 0x00040000
*/
UNPROC(void, PDEImageSetDecodeArray, (
			IN   PDEImage image,
			IN   ASReal  *decode,
			IN   ASUns32  decodeSize)
    )

/**
	Superseded by PDEContentAddPageEx() in Acrobat 10.0.
	
	Adds the specfied PDPage to the PDEContent as an Xobject form.
	It adds a reference to the Xobject form at the indicated index in
	the PDE Content; the index may be less than <code>0</code>, which indicates the object is
	to be appended to the content.
	@param theContent The content to set for the page.
	@param insertAfterIndex The index indicates the location after which the specified
	element is to be added. The index should be kPDBeforeFirst to add to the beginning
	of the display list.
	@param containerDoc The CosDoc in which the page is contained.
	@param srcPage The page that will be inserted at <code>insertAfterIndex</code> in <code>theContent</code>.
	@param dstMatrix (May be <code>NULL</code>) The matrix applied to the default matrix of the PDPage that is inserted
	into the CosDoc.
	@param annotTypes If the page contains annotations, the <code>annotTypes</code> list is used to determine
	which annotation types are pumped into the page contents of the CosDoc.
	@param flags (May be <code>0</code>) kAnnotAll specifies all annotation types.  If this is not set, then
	the <code>annotTypes</code> list will be consulted.
	@param bbox (May be <code>NULL</code>) specifies the destination <code>BBox</code> for the page being inserted.
	If it is <code>NULL</code>, the new page's media box is used.
	@see PDEContentAddPageEx
	@since PI_PDFEDIT_WRITE_VERSION >= 0x00040000
*/
#if ADD_ASFIXED //PKG
XNPROC(void, PDEContentAddPage, (OUT PDEContent theContent, 
			IN	ASInt32 insertAfterIndex, 
			IN	CosDoc containerDoc,
			IN	PDPage srcPage, 
			IN	ASFixedMatrix * dstMatrix,
			IN	ASAtom annotTypes[],
			IN	ASInt32 flags,
			IN	ASFixedRect * bbox)
	  )
#endif //ADD_ASFIXED //PKG

#if 0
/**
	Embeds the given PDEFont inside doc without creating a subset. 
	Use this method instead of PDEFontSubsetNow() if you created the 
	font with the <code>willSubset</code> flag but changed your mind. 
	@param font The font to embed. 
	@param cosDoc The container document.
	@see PDEFontEmbedNow 
	@see PDEFontIsEmbedded 
	@since PI_PDFEDIT_WRITE_VERSION >= 0x00050000
*/
UNPROC(void, PDEFontEmbedNowDontSubset, (
			IN	PDEFont	font,
			IN	CosDoc	cosDoc)
	)

/**
	Gets a Type0 font's width information for only those characters 
	used in the file. Call this routine when the font was created 
	with the kPDEFontDeferWidths flag but without the kPDEFontCreateEmbedded 
	flag (if the font is to be embedded, call PDEFontSubsetNow(), 
	which also gets the width info). 
	@param font The font whose widths are found. 
	@param cosDoc The container document.
	@since PI_PDFEDIT_WRITE_VERSION >= 0x00050000
*/
UNPROC(void, PDEFontGetWidthsNow, (
			IN	PDEFont	font,
			IN	CosDoc	cosDoc)
	)
#endif

/* APIs added for 5.0 start here */

/**
	Creates a new PDEBeginContainer object. Call PDERelease to dispose of the 
	returned PDEBeginContainer object when finished with it.
	<p>Call PDERelease() to dispose of the returned PDEBeginContainer object when finished with it. </p>
	@param mcTag IN/OUT The tag name for the marked-content sequence. 
	@param cosObjP IN/OUT (May be <code>NULL</code>) A CosDict object containing 
	the property list for the sequence. 
	@param isInline If <code>true</code>, it emits the container's dictionary 
	into the content stream inline. If <code>false</code>, then the dictionary 
	is emitted outside of the content stream and referenced by name.  
	See the Property Lists section in the <i>PDF Reference</i> for more details. 
	@return The newly created object. 
	@since PI_PDFEDIT_WRITE_VERSION >= 0x00050000

*/
UNPROC( PDEBeginContainer, PDEBeginContainerCreate, (
			IN	ASAtom mcTag,
			IN	CosObj *cosObjP,
			IN	ASBool isInline)
	)

/**
	Sets the marked content tag for a PDEBeginContainer. 
	@param pdeBeginContainer IN/OUT The PDEBeginContainer object. 
	
	@param mcTag IN/OUT The tag name. 
	@exception peErrWrongPDEObjectType if <code>pdeBeginContainer</code> is <code>NULL</code> or 
	is not the right type. 
	@since PI_PDFEDIT_WRITE_VERSION >= 0x00050000

*/
UNPROC( void, PDEBeginContainerSetMCTag, (
			IN	PDEBeginContainer pdeBeginContainer,
			IN	ASAtom mcTag)
	)

/**
	Sets the property list for a PDEBeginContainer. The property 
	list is passed as a Cos dictionary that can be emitted inline 
	or referenced from the <code>\\Properties</code> key in the <code>\\Resources</code> 
	dictionary of the containing stream.

	@note If cosObjP is <code>NULL</code>, the property list is cleared. 
	@param pdeBeginContainer IN/OUT The PDEBeginContainer object. 
	@param pdeBeginContainerDictP IN/OUT (May be <code>NULL</code>) The Cos dictionary containing 
	the property list. 
	@param isInline If <code>true</code>, it emits the container's dictionary 
	into the content stream inline. If <code>false</code>, then the dictionary 
	is emitted outside of the content stream and referenced by name.  
	See the <i>Property Lists</i> section in the <i>PDF Reference</i> for more details.
	@exception peErrWrongPDEObjectType is raised if <code>pdeBeginContainer</code> is <code>NULL</code> or 
	not the right type. 
	@since PI_PDFEDIT_WRITE_VERSION >= 0x00050000

*/
UNPROC( void, PDEBeginContainerSetDict, (
			IN	PDEBeginContainer pdeBeginContainer,
			IN	CosObj *pdeBeginContainerDictP,
			IN	ASBool isInline)
	)

/**
	Creates a new soft mask object from its Cos representation. 
	<p>Call PDERelease() to dispose of the returned PDESoftMask object when finished with it. </p>
	
	@param cosObjP IN/OUT The soft mask dictionary. 
	@return The newly created object. 
	@since PI_PDFEDIT_WRITE_VERSION >= 0x00050000

*/
UNPROC( PDESoftMask, PDESoftMaskCreateFromCosObj, (
			IN  const CosObj *cosObjP)
	)

/**
	Creates a new soft mask object. 
	<p>Call PDERelease() to dispose of the returned PDESoftMask object when finished with it. </p>
	@param cosDoc IN/OUT The container document. 
	@param type IN/OUT Specifies how the mask is to be computed. It is one 
	of the PDESoftMaskCreateFlags. 
	@param pdeForm IN/OUT The form XObject that defines the soft mask. 
	It is the source of the mask values and the PDColorSpace 
	in which the composite computation is to be done. 
	@return The newly created object. 
	@since PI_PDFEDIT_WRITE_VERSION >= 0x00050000

*/
UNPROC( PDESoftMask, PDESoftMaskCreate, (
            IN  CosDoc cosDoc,
            IN  PDESoftMaskCreateFlags type,
            IN  PDEForm pdeForm)
	)

/**
	Sets the PDEForm that defines the soft mask. 
	@param pdeSoftMask IN/OUT The soft mask object. 
	@param pdeForm IN/OUT The form XObject. 
	@since PI_PDFEDIT_WRITE_VERSION >= 0x00050000

*/
UNPROC( void, PDESoftMaskSetXGroup, (
			IN	PDESoftMask pdeSoftMask,
            IN  PDEForm pdeForm)
	)

/**
	Sets the backdrop color values. 
	@param pdeSoftMask IN/OUT The soft mask object. 
	@param pColorValues IN/OUT A series of color values. 
	@param numValues IN/OUT The number of values pointed to by <code>pColorValues</code>. 
	
	@since PI_PDFEDIT_WRITE_VERSION >= 0x00050000

*/
UNPROC( void, PDESoftMaskSetBackdropColor, (
			IN	PDESoftMask pdeSoftMask,
			IN	ASReal *pColorValues, 
			IN  ASInt32 numValues)
	)

/**
	Sets the transfer function associated with the soft mask. 
	
	@param pdeSoftMask IN/OUT The soft mask object. 
	@param cosTransferFunction IN/OUT The transfer function dictionary. 
	
	@since PI_PDFEDIT_WRITE_VERSION >= 0x00050000

*/
UNPROC( void, PDESoftMaskSetTransferFunction, (
			IN	PDESoftMask pdeSoftMask,
			IN	CosObj cosTransferFunction)
    )

/*------------------------------------------------------------------------
	PDEXGroup Methods added to support Transparency
------------------------------------------------------------------------*/

/**
	Creates a new XGroup object from its Cos representation. 
	<p>Call PDERelease() to dispose of the returned PDEXGroup object when finished with it. </p>
	
	@param cosObjP IN/OUT The XGroup object dictionary. 
	@return The PDEXGroup object. 
	@since PI_PDFEDIT_WRITE_VERSION >= 0x00050000

*/
UNPROC( PDEXGroup, PDEXGroupCreateFromCosObj, (
			IN	const CosObj *cosObjP)
	)

/**
	Create a new XGroup of the given type. 
	<p>Call PDERelease() to dispose of the returned PDEXGroup object when finished with it. </p>
	@param cosDoc The document in which the object will be created. 
	
	@param type It must be kPDEXGroupTypeTransparency.
	@return The newly created transparency group object. 
	@since PI_PDFEDIT_READ_VERSION >= 0x00050000
*/
UNPROC( PDEXGroup, PDEXGroupCreate, (
            IN  CosDoc cosDoc,
            IN  PDEXGroupCreateFlags type)
	)

/**
	Sets the knockout value. 
	@param pdeXGroup IN/OUT The transparency group object. 
	@param knockout IN/OUT The knockout value. 
	@since PI_PDFEDIT_WRITE_VERSION >= 0x00050000

*/
UNPROC( void, PDEXGroupSetKnockout, (
			IN  PDEXGroup pdeXGroup,
			IN	ASBool knockout)
	)

/**
	Sets the XGroup to be isolated or not. It corresponds to the 
	/ I key within the XGroup's dictionary. 
	@param pdeXGroup IN/OUT The transparency group object. 
	@param isolated IN/OUT <code>true</code> to isolate the XGroup, <code>false</code> otherwise. 
	
	@since PI_PDFEDIT_WRITE_VERSION >= 0x00050000

*/
UNPROC( void, PDEXGroupSetIsolated, (
            IN  PDEXGroup pdeXGroup,
            IN	ASBool isolated)
	)

/**
	Sets the PDEXObject that defines the color space into which 
	colors are converted when painted into this group. 
	@param pdeXGroup The transparency group object. 
	@param pdeColorSpace The color space to associate with 
	the XGroup.
	@since PI_PDFEDIT_WRITE_VERSION >= 0x00050000
*/
UNPROC( void, PDEXGroupSetColorSpace, (
			IN	PDEXGroup pdeXGroup,
			IN  PDEColorSpace pdeColorSpace)
	)

/**
	Sets the transparency group dictionary of the form XObject. 
	
	@param pdeForm IN/OUT The font XObject. 
	@param pdeXGroup IN/OUT The transparency dictionary. 
	@since PI_PDFEDIT_WRITE_VERSION >= 0x00050000

*/
UNPROC( void, PDEFormSetXGroup, (
			IN	PDEForm pdeForm,
			IN  PDEXGroup pdeXGroup)
	)

/*------------------------------------------------------------------------
	Methods added to support ExtGState
------------------------------------------------------------------------*/

/**
	Creates a new extended graphics state object. 
	<p>Call PDERelease() to dispose of the returned PDEExtGState object when finished with it. </p>
	@param cosDoc IN/OUT The document within which the object will be used. 
	
	@return The newly created object. 
	@since PI_PDFEDIT_WRITE_VERSION >= 0x00050000

*/
UNPROC( PDEExtGState, PDEExtGStateCreateNew, (
            IN CosDoc cosDoc)
    )

/**
	Sets the overprint mode. It corresponds to the / OPM key within 
	the ExtGState's dictionary. 
	@param pdeExtGState IN/OUT The extended graphics state object. 
	
	@param opm IN/OUT Overprint mode. 
	@since PI_PDFEDIT_WRITE_VERSION >= 0x00050000

*/
UNPROC( void, PDEExtGStateSetOPM, (
			IN	PDEExtGState pdeExtGState,
            IN  ASInt32 opm)
    )

/**
	Specifies if overprint is enabled for painting operations 
	other than stroking. It corresponds to the / op key within 
	the ExtGState's dictionary. 
	@param pdeExtGState IN/OUT The extended graphics state object. 
	
	@param overprint IN/OUT Pass <code>true</code> to enable overprint, <code>false</code> to disable overprint. 
	
	@since PI_PDFEDIT_WRITE_VERSION >= 0x00050000

*/
UNPROC( void, PDEExtGStateSetOPFill, (
			IN	PDEExtGState pdeExtGState,
            IN  ASBool overprint)
    )

/**
	Specifies if overprint is enabled for stroke operations. 
	It corresponds to the / OP key within the ExtGState's dictionary. 
	
	@param pdeExtGState IN/OUT The extended graphics state object. 
	
	@param overprint IN/OUT Pass <code>true</code> to enable overprint, <code>false</code> to disable overprint. 
	
	@since PI_PDFEDIT_WRITE_VERSION >= 0x00050000

*/
UNPROC( void, PDEExtGStateSetOPStroke, (
			IN	PDEExtGState pdeExtGState,
            IN  ASBool overprint)
    )

/**
	Sets the opacity value for painting operations other than 
	stroking. The value must be in the range from <code>0</code> to <code>1</code> inclusive. 
	It corresponds to the / ca key within the ExtGState's dictionary. 
	The value from <code>0</code> to <code>1</code> refers to a float number (not an ASReal 
	value) that should be converted to ASReal using FloatToASFixed(). 
	
	@param pdeExtGState IN/OUT The extended graphics state object. 
	
	@param opacity IN/OUT The new opacity value. 
	@since PI_PDFEDIT_WRITE_VERSION >= 0x00050000

*/
UNPROC( void, PDEExtGStateSetOpacityFill, (
			IN	PDEExtGState pdeExtGState,
            IN  ASReal opacity)
    )

/**
	Sets the opacity value for stroke operations. The value 
	must be in the range from <code>0</code> to <code>1</code> inclusive. It corresponds 
	to the / CA key within the ExtGState's dictionary. The value 
	from <code>0</code> to <code>1</code> refers to a float number (not an ASReal value) 
	that should be converted to ASReal using FloatToASFixed(). 
	
	@param pdeExtGState IN/OUT The extended graphics state object. 
	
	@param opacity IN/OUT The new opacity value. 
	@since PI_PDFEDIT_WRITE_VERSION >= 0x00050000

*/
UNPROC( void, PDEExtGStateSetOpacityStroke, (
			IN	PDEExtGState pdeExtGState,
            IN  ASReal opacity)
    )

/**
	Sets the blend mode for the color composites for each object 
	painted. 

	<p>The following mode names are valid:</p>
	<ul>
	<li>Compatible</li>
	<li>Normal</li>
	<li>Multiply</li>
	<li>Screen</li>
	<li>Difference</li>
	<li>Darken</li>
	<li>Lighten</li>
	<li>ColorDodge</li>
	<li>ColorBurn</li>
	<li>Exclusion </li>
	<li>HardLight</li>
	<li>Overlay</li>
	<li>SoftLight</li>
	<li>Luminosity</li>
	<li>Hue</li>
	<li>Saturation</li> 
	<li>Color </li>
	</ul>

	@param pdeExtGState IN/OUT The extended graphics state object. 
	
	@param blendMode IN/OUT The new blend mode. 
	@since PI_PDFEDIT_WRITE_VERSION >= 0x00050000

*/
UNPROC( void, PDEExtGStateSetBlendMode, (
			IN	PDEExtGState pdeExtGState,
            IN  ASAtom blendMode)
    )

/**
	Specifies if the alpha is to be interpreted as a shape or 
	opacity mask. 
	@param pdeExtGState The extended graphics state object. 
	
	@param alphaIsShape Indicates whether the sources of alpha 
	are to be treated as shape (<code>true</code>) or opacity (<code>false</code>). This 
	determines the interpretation of the constant alpha (ca 
	or CA) and soft mask (SMask) parameters of the graphics 
	state, as well as a soft-mask image (Smask entry) of an 
	image XObject.
	@since PI_PDFEDIT_WRITE_VERSION >= 0x00050000
*/
UNPROC( void, PDEExtGStateSetAIS, (
			IN	PDEExtGState pdeExtGState,
			IN ASBool alphaIsShape)
    )

/**
	Sets the soft mask of the extended graphics state. 
	@param pdeExtGState IN/OUT The extended graphics state object. 
	
	@param pdeSoftMask IN/OUT The soft mask object. 
	@since PI_PDFEDIT_WRITE_VERSION >= 0x00050000

*/
UNPROC( void, PDEExtGStateSetSoftMask, (
			IN	PDEExtGState pdeExtGState,
            IN  PDESoftMask pdeSoftMask)
    )

/*------------------------------------------------------------------------
	PDEImage Methods added to support SMask and Mattes
------------------------------------------------------------------------*/

/**
	Sets the soft mask. 
	@param image IN/OUT The image XObject. 
	@param sMask IN/OUT The soft mask. 
	@exception peErrWrongPDEObjectType 
	@since PI_PDFEDIT_WRITE_VERSION >= 0x00050000

*/
UNPROC(void, PDEImageSetSMask, (
            IN	PDEImage image,
            IN	PDEImage sMask)
      )

/**
	Sets the matte array for the image XObject. 
	@param image IN/OUT The image XObject. 
	@param matte IN/OUT An array of values. 
	@param numComp IN/OUT The number of values in mArray. 
	@since PI_PDFEDIT_WRITE_VERSION >= 0x00050000

*/
UNPROC(void, PDEImageSetMatteArray, (
			IN   PDEImage image,
			IN   ASReal  *matte,
			IN   ASUns32  numComp)
	)

/**
	Creates a new PDEEndContainer object. Call PDERelease to dispose of the 
	returned PDEEndContainer object when finished with it.
	<p>Call PDERelease() to dispose of the returned PDEEndContainer object when finished with it. </p>
	@return The newly created object. 
	@since PI_PDFEDIT_WRITE_VERSION >= 0x00050000

*/
UNPROC( PDEEndContainer, PDEEndContainerCreate, ()
	)

/**
	Creates a new begin group object. 
	<p>Call PDERelease() to dispose of the returned PDEBeginGroup object when finished with it. </p>
	@return The newly created object. 
	@since PI_PDFEDIT_WRITE_VERSION >= 0x00050000

*/
UNPROC( PDEBeginGroup, PDEBeginGroupCreate, ()
	)

/**
	Creates a new end group object. 
	<p>Call PDERelease() to dispose of the returned PDEEndGroup object when finished with it. </p>
	@return The newly created object. 
	@since PI_PDFEDIT_WRITE_VERSION >= 0x00050000

*/
UNPROC( PDEEndGroup, PDEEndGroupCreate, ()
	)


#if 0
/**
	Used to obtain a PDEFont corresponding to a font in the 
	system. 
	<p>Call PDERelease() to dispose of the returned PDEFont object when finished with it. </p>
	@param sysFont The system font. 
	@param params The parameters structure.
	@return The newly created PDEFont object. 
	@exception peErrCantCreateFontSubset 
	@exception genErrBadParm 

	@note If you want to use <code>WinAnsiEncoding</code> on UNIX, use this 
	method or PDEFontCreateFromSysFontAndEncoding() instead. 
	@since PI_PDFEDIT_WRITE_VERSION >= 0x00050000
*/
UNPROC(PDEFont, PDEFontCreateFromSysFontWithParams, (
			IN	PDSysFont sysFont,
			IN	PDEFontCreateFromSysFontParams params)
	)

/**
	Translates a string to Unicode values. The PDEFont must 
	have a / ToUnicode table. 
	@param font IN/OUT The font. 
	@param text IN/OUT The string to convert. 
	@param textLen IN/OUT The length of <code>text</code> in bytes. 
	@param unicodeStr IN/OUT (Filled by the method) A buffer to hold 
	the translated string. 
	@param size IN/OUT The size of the <code>unicodeStr</code> buffer. 
	@return <code>0</code> if the string was successfully translated. If <code>unicodeStr</code> 
	is too small for the translated string, it returns the number 
	of bytes required. 
	@exception genErrBadParm 
	@since PI_PDFEDIT_WRITE_VERSION >= 0x00050000

*/
UNPROC(ASUns32, PDEFontTranslateGlyphIdsToUnicode, (
			IN	PDEFont font,
			IN	ASUns8 *text,
			IN	ASUns32 textLen,
			OUT	ASUns8 *unicodeStr,
			IN	ASUns32 size)
	)

/**
	Specifies whether text knockout is enabled in the graphics 
	state. This corresponds to the / TK key in the ExtGState's 
	dictionary. 
	@param pdeExtGState IN/OUT The extended graphics state object. 
	
	@param bk IN/OUT Pass <code>true</code> to enable text knockout, <code>false</code> to disable text knockout. 
	@since PI_PDFEDIT_WRITE_VERSION >= 0x00050000

*/
UNPROC( void, PDEExtGStateSetTK, (
			IN	PDEExtGState pdeExtGState,
            IN  ASBool bk)
    )

/**
	Sets the text state of a text run. 
	@param pdeText The text object containing a text run whose 
	state is set. 
	@param runIndex The index of the text run. 
	@param stateP A pointer to a PDETextState structure with 
	the state to set. 
	@param stateSize The size of the <code>stateP</code> buffer in bytes.
	@exception peErrWrongPDEObjectType 
	@exception genErrBadParm 
	@see PDETextRunSetTextState 
	@since PI_PDFEDIT_WRITE_VERSION >= 0x00050000
*/
UNPROC(void, PDETextRunSetState, (
			IN	PDEText pdeText, 
			IN	ASInt32 runIndex, 
			IN	PDETextStateP stateP, 
			IN	ASUns32 stateSize)
	)

/**
	Create an encoding object from the base name. 
	<p>Call PDERelease() to dispose of the returned PDSysEncoding object when finished with it. </p>
	@param baseEncName IN/OUT The base encoding. See Section 5.5.5 
	in the <i>PDF Reference</i>. 
	@param diffEnc IN/OUT An array of 256 <code>const char*</code> describing the 
	differences from the encoding specified by <code>baseEncName</code>. 
	It may be <code>NULL</code>. 
	@return An object of type PDSysEncoding. 
	@exception genErrBadParm 
	@since PI_PDFEDIT_WRITE_VERSION >= 0x00050000

*/
UNPROC(PDSysEncoding, PDSysEncodingCreateFromBaseName, (
			IN	ASAtom			baseEncName,
			IN	const char		**diffEnc)
	)

/**
	Create an encoding object from a PDF CMap name. 
	<p>Call PDERelease() to dispose of the returned PDSysEncoding object when finished with it. </p>
	@param cmapName The CMap name.
	@return An object of type PDSysEncoding. 
	@exception genErrBadParm 
	@since PI_PDFEDIT_WRITE_VERSION >= 0x00050000
*/
UNPROC(PDSysEncoding, PDSysEncodingCreateFromCMapName, (
			IN	ASAtom			cmapName)
	)

/**
	This function returns a <code>createFlags</code> that can be passed to 
	PDEFontCreateFromSysFontAndEncoding(). If the combination 
	of sysFont and sysEnc is not allowed, <code>-1</code> is returned. 
	@param sysFont IN/OUT An object of type PDSysFont. 
	@param sysEnc IN/OUT An object of type PDSysEncoding. 
	@return See above. 
	@exception peErrWrongPDEObjectType 
	@since PI_PDFEDIT_WRITE_VERSION >= 0x00050000

*/
UNPROC(ASInt32, PDSysFontGetCreateFlags, (
			IN	PDSysFont		sysFont,
			IN	PDSysEncoding	sysEnc)
	)

/**
	Similar to PDSysFontGetCreateFlags but avoids compatibility
	issues with changing PDSysFontGetCreateFlags. If the combination
	of sysFont and sysEnc is not allowed, <code>-1</code> is returned. If the
	combination is ok, then <code>0</code> is returned. If the combination only
	works if the font is embedded, kPDEFontCreateEmbedded is returned.
	@param sysFont IN/OUT An object of type PDSysFont. 
	@param sysEnc IN/OUT An object of type PDSysEncoding. 
	@return See above. 
	@exception peErrWrongPDEObjectType 
	@since PI_PDFEDIT_WRITE_VERSION >= 0x00070000

*/
UNPROC(ASInt32, PDSysFontVerifyEncoding, (
			IN	PDSysFont	sysFont,
			IN	PDSysEncoding	sysEnc)
	)

/**
	Create a PDEFont from <code>sysFont</code> and <code>sysEnc</code>. If it fails, it 
	raises an exception. User can call PDSysFontGetCreateFlags() 
	to see if the combination of sysFont and sysEnc makes sense. 
	<p>Call PDERelease() to dispose of the returned PDEFont object when finished with it. </p>
	
	@note If you want to use <code>WinAnsiEncoding</code> on UNIX, use this 
	method or PDEFontCreateFromSysFontWithParams(). 
	@param sysFont A PDSysFont object referencing a system 
	font. 
	@param sysEnc A PDSysEncoding object. 
	@param useThisBaseFont The base font. An exception will 
	be raised if the base font name passed is a subset name 
	<code>(XXXXXX+FontName)</code> or an empty string. 
	@param createFlags One of the PDEFontCreateFlags.
	@return The newly created PDEFont object. 
	@since PI_PDFEDIT_WRITE_VERSION >= 0x00050000
*/
UNPROC(PDEFont, PDEFontCreateFromSysFontAndEncoding, (
			IN	PDSysFont		sysFont,
			IN	PDSysEncoding	sysEnc,
			IN	ASAtom			useThisBaseFont,
			IN	ASUns32			createFlags)
	)

/**
	This function returns flags indicating what needs to be 
	done to make PDEFont complete. kPDEFontCreateNeedWidths 
	can be cleared by PDEFontCreateWidthsNow(). kPDEFontCreateNeedToUnicode 
	can be cleared by PDEFontCreateToUnicodeNow(). kPDEFontCreateNeedEmbed 
	can be cleared by PDEFontEmbedNow(). 
	@param font The font object.
	@return A value corresponding to PDEFontCreateNeedFlags(). 
	@since PI_PDFEDIT_WRITE_VERSION >= 0x00050000
*/
UNPROC(ASUns32, PDEFontGetCreateNeedFlags, (
			IN	PDEFont			font)
	)

/**
	This function embeds a font stream. User can check the return 
	value of PDEFontGetCreateNeedFlags() to see if calling PDEFontEmbedNow() 
	is needed. 
	@param font The font to embed. 
	@param cosDoc The container document.
	@exception genErrBadParm 
	@exception peErrWrongPDEObjectType 
	@see PDEFontEmbedNowDontSubset 
	@see PDEFontIsEmbedded 
	@since PI_PDFEDIT_WRITE_VERSION >= 0x00050000
*/
UNPROC(void, PDEFontEmbedNow, (
			IN	PDEFont			font,
			IN	CosDoc			cosDoc)
	)

/**
	This function creates width entries for <code>font</code>. User can check 
	the return value of PDEFontGetCreateNeedFlags() to see if 
	calling PDEFontCreateWidthsNow() is needed. 
	@param font IN/OUT The font for which to create width entries. 
	@param cosDoc IN/OUT The container document. 
	@exception genErrBadParm 
	@exception peErrWrongPDEObjectType 
	@since PI_PDFEDIT_WRITE_VERSION >= 0x00040000

*/
UNPROC(void, PDEFontCreateWidthsNow, (
			IN	PDEFont			font,
			IN	CosDoc			cosDoc)
	)

/**
	This function creates the / ToUnicode table. The user can 
	check the return value of PDEFontGetCreateNeedFlags() to see 
	if calling PDEFontCreateToUnicodeNow() is needed. 
	@param font IN/OUT An object of type PDEFont. 
	@param cosDoc IN/OUT The container document. 
	@exception genErrBadParm 
	@exception peErrWrongPDEObjectType 
	@since PI_PDFEDIT_WRITE_VERSION >= 0x00050000

*/
UNPROC(void, PDEFontCreateToUnicodeNow, (
			IN	PDEFont			font,
			IN	CosDoc			cosDoc)
	)
#endif

/**
	Sets the color space of the image. 
	@param image IN/OUT The image whose color space is obtained. 
	@param space IN/OUT An object of type PDEColorSpace. 
	@exception peErrWrongPDEObjectType 
	@see PDEImageGetColorSpace 
	@see PDEImageSetColorSpace 
	@since PI_PDFEDIT_WRITE_VERSION >= 0x00050000

*/
UNPROC(void, PDEImageSetColorSpace, (
			IN	PDEImage			image,
			IN	PDEColorSpace		space)
	)

/**
	Specifies whether stroke adjustment is enabled in the graphics 
	state. 
	@param pdeExtGState IN/OUT The extended graphics state object. 
	
	@param strokeAdjust IN/OUT Pass <code>true</code> to enable stroke adjustment, <code>false</code> to disable stroke adjustment. 
	
	@exception peErrWrongPDEObjectType 
	@since PI_PDFEDIT_WRITE_VERSION >= 0x00050000

*/
UNPROC( void, PDEExtGStateSetSA, (
			IN	PDEExtGState pdeExtGState,
            IN  ASBool strokeAdjust)
    )

/**
	Create a new soft mask from a name. 
	<p>Call PDERelease() to dispose of the returned PDESoftMask object when finished with it. </p>
	@param name IN/OUT The new name for the soft mask. Note that, currently, 
	the only valid name is <code>None</code>. 
	@return The newly created object. 
	@since PI_PDFEDIT_WRITE_VERSION >= 0x00050000

*/
UNPROC( PDESoftMask, PDESoftMaskCreateFromName, (
			IN  ASAtom name)
	)

/**
	Superseded by PDETextRunSetMatrixEx() in Acrobat 10.0.
	
	Sets the matrix of a text run. Unlike PDETextRunSetTextMatrix(), 
	this function does not change <code>fontSize</code>, <code>hScale</code>, and <code>textRise</code> 
	in the <code>textState</code> of PDEText. 
	@param pdeText IN/OUT The text object containing a text run. 
	@param runIndex IN/OUT The index of the text run. 
	@param matrixP IN/OUT ASFixedMatrixP pointer. 
	@exception peErrWrongPDEObjectType 
	@exception genErrBadParm 
	@see PDETextRunSetTextMatrix 
	@see PDETextRunSetMatrixEx
	@since PI_PDFEDIT_WRITE_VERSION >= 0x00050000

*/
#if ADD_ASFIXED //PKG
UNPROC(void, PDETextRunSetMatrix, (
			IN	PDEText pdeText, 
			IN	ASInt32 runIndex, 
			IN	ASFixedMatrixP matrixP)
	)
#endif //ADD_ASFIXED //PKG

/* BEGIN Optional Content API calls */
#if ADD_OCG
/**
	Associates an optional-content membership dictionary (OCMD) 
	object with the element. The element must be a PDEForm, 
	a PDEImage (XObject image), or a PDEContainer. 
	
	<p>If it is not one of these, nothing happens: </p>

	<ul>
	<li> If the element is a PDEForm or PDEImage, the method attaches 
	the dictionary to the element's Cos XObject dictionary. </li>
	<li> If the element is a PDEContainer, and it is already for 
	optional content, the optional-content information is replaced. </li>
	<li>If it is not already for optional content, a new PDEContainer 
	for optional content is created and nested inside the specified 
	container. </li>
	</ul>

	@param elem The element for which to set the dictionary. 
	
	@param pdOCMD The new dictionary.
	@see PDEElementGetOCMD 
	@see PDEElementRemoveOCMD 
	@see PDOCMDFindOrCreate 
	@since PI_PDFEDIT_WRITE_VERSION >= 0x00060000
*/
UNPROC( void, PDEElementSetOCMD, (PDEElement elem, PDOCMD pdOCMD) )
/**
	Dissociates an optional-content membership dictionary (OCMD) 
	object from the element. The element must be a PDEForm, 
	a PDEImage (XObject image), or a PDEContainer. 
	
	<p>If it is not one of these, nothing happens: </p>

	<ul>
	<li> If the element is a PDEForm or PDEImage, the method removes 
	the dictionary from the element's Cos XObject dictionary. </li>
	<li> If the element is a PDEContainer for optional content, 
	the method removes the dictionary, but does not destroy 
	the container. </li>
	</ul>

	@param elem The element for which to remove the dictionary.
	@see PDEElementGetOCMD 
	@see PDEElementSetOCMD 
	@see PDOCMDFindOrCreate 
	@since PI_PDFEDIT_WRITE_VERSION >= 0x00060000
*/
UNPROC( void, PDEElementRemoveOCMD, (PDEElement elem) )

/**
	Flattens the content, removing any PDEElement objects that are 
	not visible in the given optional-content context, and removing 
	the optional-content information from any visible PDFElement objects. 
	
	@param content The content to be modified. 
	@param context The optional-content context in which <code>content</code> 
	is checked for visibility.
	@return <code>true</code> if the operation is successful, <code>false</code> otherwise. 
	@see PDDocFlattenOC 
	@see PDPageFlattenOC 
*/
UNPROC( ASBool, PDEContentFlattenOC, (PDEContent content, PDOCContext context) )
#endif
/* END Optional Content API calls */


/**
	Create an encoding object from a code page. 
	<p>Call PDERelease() to dispose of the returned PDSysEncoding object when finished with it. </p>
	@param codePage The code page character-mapping construct. 
	See Code Page Values. 
	@param wMode <code>0</code> for horizontal writing, <code>1</code> for vertical 
	writiing.
	@return An object of type PDSysEncoding. 
	@exception genErrBadParm 
	@since PI_PDFEDIT_WRITE_VERSION >= 0x00060000
*/
#if 0
UNPROC(PDSysEncoding, PDSysEncodingCreateFromCodePage, (
			IN	ASInt32			codePage,
			IN	ASInt16			wMode)
	)


/**
	Sets the system font object to be used with a font object 
	that does not currently have a system font associated with 
	it. 
	@param pdeFont A PDEFont whose system font is set. 
	@param sysFont The new system font object.
	@exception genErrBadParm 
	@exception genErrResourceLoadFailed 
	@see PDEFontGetSysFont 
	@since PI_PDFEDIT_WRITE_VERSION >= 0x00060000
*/
UNPROC(void, PDEFontSetSysFont, (
			IN	PDEFont			pdeFont,
			IN	PDSysFont		sysFont)
	)

/**
	Sets the system encoding object associated with a font object. 
	@param pdeFont A PDEFont whose system encoding is set. 
	
	@param sysEnc The new system encoding object.
	@exception genErrBadParm 
	@exception genErrResourceLoadFailed 
	@see PDEFontGetSysEncoding 
	
	@note Changing the system encoding may produce unexpected 
	results. 
	@since PI_PDFEDIT_WRITE_VERSION >= 0x00060000
*/
UNPROC(void, PDEFontSetSysEncoding, (
			IN	PDEFont			pdeFont,
			IN	PDSysEncoding	sysEnc)
	)
#endif

/**
	Superseded by PDETextItemCreateEx() in Acrobat 10.0.
	
	Creates a text item element containing a character or text 
	run, which can be added to a PDEText text object. 
	<p>Call PDERelease() to dispose of the returned PDETextItem object when finished with it. </p>
	@param text A pointer to the characters to add. Note that passing 
	<code>NULL</code> for text can invalidate the text object but will not 
	raise an error. Callers must not pass <code>NULL</code> for this parameter. 
	
	@param textLen The length of the text in bytes. 
	@param font The font for the element. 
	@param gStateP A pointer to a PDEGraphicStateP structure 
	with the graphics state for the element. 
	@param gStateLen The length of the graphics state for the element. 
	
	@param textStateP A pointer to a PDETextState structure with the
	text state for the element. Note that PDFEdit ignores the <code>wasSetFlags</code> 
	flag of the PDETextState structure, so you must initialize 
	the PDETextState fields. 
	@param textStateLen The length of the text state for the element. 
	
	@param textMatrixP A pointer to an ASFixedMatrix that holds 
	the matrix for the element.
	@exception pdErrBadResMetrics 
	@exception peErrWrongPDEObjectType 
	@exception genErrBadParm 
	@see PDETextAdd 
	@see PDETextAddItem
	@see PDETextItemCreateEx
	@since PI_PDFEDIT_WRITE_VERSION >= 0x00060000
*/
#if ADD_ASFIXED //PKG
UNPROC(PDETextItem, PDETextItemCreate, (
			IN	ASUns8				*text,
			IN	ASUns32				textLen,
			IN	PDEFont				font,
			IN	PDEGraphicStateP	gStateP,
			IN	ASUns32				gStateLen,
			IN	PDETextStateP		textStateP,
			IN	ASUns32				textStateLen,
			IN	ASFixedMatrix		*textMatrixP))
#endif //ADD_ASFIXED //PKG

/**
	Sets the font for a text item. 
	@param textItem The text item whose font is set. 
	@param font The new font object.
	@exception peErrWrongPDEObjectType 
	@exception genErrBadParm 
	@exception pdErrBadResMetrics 
	@see PDETextItemGetFont 

	@note This method decrements the reference count of the 
	previous font and increments the reference count of the 
	new font. 
	@since PI_PDFEDIT_WRITE_VERSION >= 0x00060000
*/
UNPROC(void, PDETextItemSetFont, (
			IN	PDETextItem			textItem,
			IN	PDEFont				font))

/**
	Superseded by PDETextItemSetTextMatrixEx() in Acrobat 10.0.
	
	Sets the text matrix for a text item. 
	@param textItem The text item whose text matrix is set. 
	
	@param textMatrixP A pointer to a ASFixedMatrix structure 
	with the new text matrix of the text item.
	@exception peErrWrongPDEObjectType 
	@exception genErrBadParm 
	@exception pdErrBadResMetrics 
	@see PDETextItemGetTextMatrix 
	@see PDETextItemSetTextMatrixEx
	@since PI_PDFEDIT_WRITE_VERSION >= 0x00060000
*/
#if ADD_ASFIXED //PKG
UNPROC(void, PDETextItemSetTextMatrix, (
			IN	PDETextItem			textItem,
			IN	ASFixedMatrix		*textMatrixP))
#endif //ADD_ASFIXED //PKG

/**
	Sets the text state for a text item. 
	@param textItem The text item whose text state is set. 
	
	@param textStateP A PDETextStateP structure with the new 
	text state of the text item. 
	@param textStateSize The size of the <code>textStateP</code> structure in bytes.
	@exception peErrWrongPDEObjectType 
	@exception genErrBadParm 
	@exception pdErrBadResMetrics 
	@see PDETextItemGetTextState 
	@since PI_PDFEDIT_WRITE_VERSION >= 0x00060000
*/
UNPROC(void, PDETextItemSetTextState, (
			IN	PDETextItem			textItem,
			IN	PDETextStateP		textStateP,
			IN	ASUns32				textStateSize))

/**
	Sets the graphics state for a text item. 
	@param textItem Text item whose graphics state is set. 
	@param stateP A pointer to a PDEGraphicStateP structure 
	with graphics state of the text item. 
	@param stateSize The size of the <code>stateP</code> buffer in bytes.
	@exception peErrWrongPDEObjectType 
	@exception genErrBadParm 
	@exception pdErrBadResMetrics 
	@see PDETextItemGetGState 
	@since PI_PDFEDIT_WRITE_VERSION >= 0x00060000
*/
UNPROC(void, PDETextItemSetGState, (
			IN	PDETextItem			textItem,
			OUT	PDEGraphicStateP	stateP,
			IN	ASUns32				stateSize))

/**
	Replaces all of the text in a text item. 
	@param textItem The text item whose text are replaced. 
	
	@param newText The replacement text. 
	@param newTextLen The number of bytes to replace. 
	@exception peErrWrongPDEObjectType 
	@exception genErrBadParm 
	@exception pdErrBadResMetrics 
	@see PDETextItemRemoveChars 
	@see PDETextItemReplaceChars 
	@since PI_PDFEDIT_WRITE_VERSION >= 0x00060000
*/
UNPROC(void, PDETextItemReplaceText, (
			IN	PDETextItem			textItem,
			IN	ASUns8				*newText,
			IN	ASUns32				newTextLen))

/**
	Replaces characters in a text item. 

	<p>This method does not change the number of characters in 
	the text item; extra characters are ignored. </p>

	@param textItem The text item whose characters are replaced. 
	
	@param charIndex The index position of the characters to replace. 
	
	@param newChar The replacement text. 
	@param newCharLen The number of bytes to replace. 
	@exception peErrWrongPDEObjectType 
	@exception genErrBadParm 
	@exception pdErrBadResMetrics 
	@see PDETextItemRemoveChars 
	@see PDETextItemReplaceText 
	@since PI_PDFEDIT_WRITE_VERSION >= 0x00060000
*/
UNPROC(void, PDETextItemReplaceChars, (
			IN	PDETextItem			textItem,
			IN	ASUns32				charIndex,
			IN	ASUns8				*newChar,
			IN	ASUns32				newCharLen))

/**
	Removes contiguous characters from a text item. 
	@param textItem The text item whose characters are removed. 
	
	@param charOffset The offset of the first character to remove. 
	
	@param count The number of characters to remove.
	@exception peErrWrongPDEObjectType 
	@exception genErrBadParm 
	@exception pdErrBadResMetrics 
	@see PDETextItemReplaceChars 
	@see PDETextItemReplaceText 
	@since PI_PDFEDIT_WRITE_VERSION >= 0x00060000
*/
UNPROC(void, PDETextItemRemoveChars, (
			IN	PDETextItem			textItem,
			IN	ASUns32				charOffset,
			IN	ASUns32				count))

/**
	Adds a text item to a text element at a given index position. 
	@param text The text object to which the text item is added. 
	@param addIndex The index of the text item in <code>pdeText</code>. 
	@param textItem The text item to add.
	@exception peErrWrongPDEObjectType 
	@exception genErrBadParm 
	@exception pdErrBadResMetrics 
	@see PDETextGetItem 
	@see PDETextRemoveItems 
	@see PDETextItemCreate 
	@since PI_PDFEDIT_WRITE_VERSION >= 0x00060000
*/
UNPROC(void, PDETextAddItem, (
			IN	PDEText 			text, 
			IN	ASInt32 			addIndex,
			IN	PDETextItem			textItem))

/**
	Removes contiguous text items from a text element starting 
	at a given index position. 
	@param text The text object from which the text items are 
	removed. 
	@param index The index of the first text item in <code>pdeText</code> to 
	remove. 
	@param count The number of text items to remove.
	@exception peErrWrongPDEObjectType 
	@exception genErrBadParm 
	@exception pdErrBadResMetrics 
	@see PDETextAddItem 
	@see PDETextGetItem 
	@since PI_PDFEDIT_WRITE_VERSION >= 0x00060000
*/
UNPROC(void, PDETextRemoveItems, (
			IN	PDEText				 text, 
			IN	ASUns32				index, 
			IN	ASUns32				count))

/****************** For Acrobat 6.0 feature : 513825**********/

/**
	Sets the underlying CosStream of the form using the specified 
	content object. 
	@param form The form whose content is set. 
	@param content The new content for <code>form</code>.
	@exception peErrWrongPDEObjectType 
	@exception peErrPStackUnderflow 
	@see PDEFormGetContent 
	@since PI_PDFEDIT_WRITE_VERSION >= 0x00060000
*/
UNPROC(void, PDEFormSetContent, (
			IN	PDEForm form,
			IN	PDEContent content)
	)
	
/**
	Creates a new form from an existing form object. Creates 
	a copy of the PDEForm, including the underlying CosStream. 
	<p>Call PDERelease() to dispose of the returned PDEForm object when finished with it. </p>
	
	@param form The form object from which a new PDEForm is created.
	@return The newly created form object. 
	@see PDEFormCreateFromCosObj 
	@since PI_PDFEDIT_WRITE_VERSION >= 0x00060000
*/
UNPROC(PDEForm, PDEFormCreateClone, (
			IN PDEForm form)
	)

#if 0
/**
	Superseded by PDETextAddGlyphsEx() in Acrobat 10.0.
	
	Adds Unicode text to a PDEText object. 

	@param pdeText The text object to which a character or text 
	run is added. 
	@param flags A PDETextFlags that specifies what kind of 
	text to add. 
	It must be one of the following values: 

	<TABLE rules="all" cellspacing="1">
	<TR><TH>Value</TH><TH>Description</TH></TR>
	<TR><TD>kPDETextChar</TD><TD>Used for a text character.</TD></TR>
	<TR><TD>kPDETextRun</TD><TD>Used for a text run.</TD></TR>
	</TABLE>

	@param index The index after which to add the character or text 
	run. 
	@param glyphRun A pointer to a PDEGlyphRun structure
	with Unicode data, GlyphIDs and their correspondence.
	@param font The font for the element. 
	@param gstateP A pointer to a PDEGraphicStateP structure 
	with the graphics state for the element. 
	@param gstateLen The length of the graphics state for the element.
	@param tstateP A pointer to a PDETextState structure with 
	text state for the element. Note that PDFEdit ignores the <code>wasSetFlags</code> 
	flag of the PDETextState structure, so you must initialize 
	the PDETextState fields. 
	@param tstateLen The length of the text state for the element. 
	@param textMatrixP A pointer to an ASFixedMatrix that holds 
	the matrix for the element. 
	@param strokeMatrixP A pointer to an ASFixedMatrix that holds 
	the matrix for the line width when stroking text. It may be 
	<code>NULL</code>. Note that, currently, this field is not used. (Acrobat 5)
	@exception pdErrBadResMetrics 
	@exception peErrWrongPDEObjectType 
	@exception genErrBadParm 
	@see PDETextAdd
	@see PDETextAddGlyphsEx

	@note This method does not change the reference count of 
	<code>pdeText</code>; however, the reference count of the objects in 
	the <code>gstateP</code> parameter are incremented. 
	@since PI_PDFEDIT_WRITE_VERSION >= 0x00070000
*/
UNPROC(void, PDETextAddGlyphs, (
			IN	PDEText pdeText, 
			IN	ASUns32 flags,			/* kPDETextRun, kPDETextChar */ 
			IN	ASInt32 index,
			IN	PDEGlyphRunP glyphRun,
			IN	PDEFont font,
			IN	PDEGraphicStateP gstateP,
			IN	ASUns32 gstateLen,
			IN	PDETextStateP tstateP,
			IN	ASUns32 tstateLen,
			IN	ASFixedMatrixP textMatrixP,
			IN	ASFixedMatrixP strokeMatrixP)
	)

/**
	Adds glyphs to a PDEFont object for embedding a PDEFont.

	<p>This is used by clients that use PDEFEdit calls to embed
	the font but create their own content stream. The glyphs
	added by this routine will be included in the font when
	PDEFontSubsetNow() is called. It is up to the client to ensure
	that the encoding used by the PDEFont matches the character
	codes used in the string arguments to the text operators in
	the content stream.</p>

	<p>This routine is used to specify which glyphs should be included
	in the font when embedded. Additionally, it specifies the mapping
	from the GlyphIDs to Unicode values. This mapping will be used
	to create the ToUnicode entry in the embedded font object. In
	the cases where the ToUnicode table cannot accurately reproduce
	the Unicode string in the PDEGlyphRun structure, this routine
	will return an array of spans that describe the contents of the
	ActualText spans that must be included in the content stream.
	Each span indicates a contiguous range of glyphs and a
	corresponding contiguous range of Unicode values that correspond
	to the glyphs. For example, the following ActualText span
	replace two glyphs with three Unicode values.</p>

	<p><code>/Span<</ActualText(U+vvvvU+xxxU+yyyy)>></code></p>
	<p><code>	BDC [Giii Gjjj] TJ EMC</code></p>

	<p>Note that the routine must be called with the PDEGlyphRuns in display order.</p>

	@param pdeFont The font for the element.

	@param glyphRun A pointer to a PDEGlyphRun structure
	with Unicode data, GlyphIDs and their correspondence. Note that
	the <code>xPosition</code> and <code>yPosition</code> fields in the PDEGlyphDescription
	structure are ignored.

	@param flags Unused, reserved for later use.

	@return A pointer to a PDESpanSet. The span can be released with PDEReleaseSpan().
	@exception genErrBadParm
	@see PDEFontSubsetNow
	@see PDEFontCreateFromSysFont
	@see PDEReleaseSpan
	@since PI_PDFEDIT_WRITE_VERSION >= 0x00070000
*/
UNPROC(PDESpanSetP, PDEFontAddGlyphs, (
			IN	PDEFont pdeFont,
			IN	PDEGlyphRunP glyphRun,
			IN	ASUns32 flags)
	)

/**
	Releases a PDESpan object that is returned by PDEFontAddGlyphs().

	@exception genErrBadParm
	@see PDEFontAddGlyphs
	@since PI_PDFEDIT_WRITE_VERSION >= 0x00070000
*/
UNPROC(void, PDEReleaseSpan, (
			IN	PDESpanSetP pdeSpan)
	)
#endif

/**
	Sets the page on which marked content is drawn upon for any marked content
	reference handles attached to containers within the content.

	@param pdeContent The content stream whose marked content reference
	handles should be updated.
	@param pageObj The page object upon which contents are drawn.
	@see PDEContentSetContainingStream
	@see PDEContentSetStreamOwner
	@see PDSMCRefCreate

	@note If content is set with PDPageSetPDEContent(), PDEFormSetContent(), or
	PDEGroupSetContent(), this step occurs automatically.

	@note This call should only be used when the content is being directly added to a
	page.
	@since PI_PDFEDIT_WRITE_VERSION >= 0x00070000
*/
UNPROC(void, PDEContentSetPage, (
			IN PDEContent pdeContent,
			IN CosObj pageObj)
	)

/**
	Sets the containing stream and owner stream for any marked content reference
	handles attached to containers within the content.

	@param pdeContent The content stream within which to update marked content references.
	@param containingStm The containing stream object for the content stream.
	@see PDEContentSetPage
	@see PDEContentSetStreamOwner
	@see PDEContentSetStreamOwner

	@note This call should not be used when the content is being directly added to a
	page.

	@note If the content is set with PDPageSetPDEContent(), PDEFormSetContent(), or
	PDEGroupSetContent(), this step occurs automatically.
	@since PI_PDFEDIT_WRITE_VERSION >= 0x00070000
*/
UNPROC(void, PDEContentSetContainingStream, (
			IN PDEContent pdeContent,
			IN CosObj containingStm)
	)

/**
	Sets the stream owner for any marked content reference handles 
	attached to containers within the content.

	@param pdeContent The content stream within which to update marked content references.
	@param streamOwner The owner object for any references attached to the content.
	@see PDEContentSetPage
	@see PDEContentSetContainingStream

	@note This call should not be used when the content is being directly added to a
	page.

	@note If content is set with PDPageSetPDEContent(), PDEFormSetContent(), or
	PDEGroupSetContent(), this step occurs automatically.
	@since PI_PDFEDIT_WRITE_VERSION >= 0x00070000
*/
UNPROC(void, PDEContentSetStreamOwner, (
			IN PDEContent pdeContent,
			IN CosObj streamOwner)
	)



/**
	Superseded by PDEFormGetMatrixEx() in Acrobat 10.0.
	
	Gets the matrix for a PDEform. 
	<p>The result is a concatenation of the CTM and the Cos level form matrix, resulting in the transformation from the form space to the device space.</p>
	@note For the other elements, PDEElementGetMatrix() would give correct results.
	@param form The form for which the matrix is required.
	@param matrixP The resultant matrix.

	@see PDEElemetGetMatrix
	@see PDEFormGetBBox
	@see PDEFormGetMatrixEx

	@since PI_PDFEDIT_WRITE_VERSION >= 0x00070005

*/

#if ADD_ASFIXED //PKG
UNPROC(void, PDEFormGetMatrix, (
			IN PDEForm form,
			OUT ASFixedMatrixP	matrixP)
	)
#endif //ADD_ASFIXED //PKG
/**
	Gets the bounding box for a PDEform. The result is the concatenation of the CTM and the Cos level form matrix applied on 
	cos level bounding box. The returned bounding box is guaranteed to encompass the PDEForm, but is not guaranteed to be the smallest box that 
	could contain the form object.
	Note: For other elements, PDEElementGetBBox() would return the correct bounding box values.
	@param form The PDEForm for which the bounding box is required.
	@param bboxP The resulting bounding box.

	@see PDEElementGetBBox
	@see PDEFormGetMatrix

	@since PI_PDFEDIT_WRITE_VERSION >= 0x00070005
*/

UNPROC(void, PDEFormGetBBox, (
			IN PDEForm form,
			OUT ASFixedRectP	bboxP)
	)

	/**
	Superseded by PDEImageCreateInCosDocEx() in Acrobat 10.0.
	
	Creates an image object like PDEImageCreate(), except that the client can specify the CosDoc
	in which the image is created.
	
	<p>The image data may be specified as a stream or as a buffer. 
	If <code>data</code> is non-<code>NULL</code>, <code>dataStm</code> is ignored. </p>

	<p>See PDEImageSetDataStm() for information on handling the stream. </p>
	
	<p>The caller must dispose of <code>dataStm</code> after calling this method.</p> 
	
	<p>Call PDERelease() to dispose of the returned image object 
	when finished with it. </p>

	@param attrsP IN/OUT A pointer to a PDEImageAttrs object with attributes 
	of the image. 
	@param attrsSize IN/OUT The size of the <code>attrsP</code> buffer in bytes. 
	@param matrixP IN/OUT A pointer to an ASFixedMatrix that holds the 
	transformation matrix to use for the image. 
	@param flags IN/OUT PDEImageDataFlags flags. If the kPDEImageEncodedData 
	flag is set, and the data is provided directly (not as a 
	stream), then <code>encodedLen</code> must specify the length of data. 
	
	@param colorSpace IN/OUT The color space of the image. When the image 
	is an image mask, <code>colorSpace</code> is the color space of the <code>colorValueP</code> 
	argument. 
	@param colorValueP IN/OUT A pointer to a PDEColorValue structure. 
	If the image is an image mask, <code>colorValueP</code> must be provided. 
	
	@param filtersP IN/OUT A pointer to a PDEFilterArray structure that 
	specifies which filters to use in encoding the contents; 
	it may be <code>NULL</code>. Filters will be used to encode the data in 
	the order in which they are specified in the array. 
	@param dataStm IN/OUT The stream holding the image data. 
	@param data IN/OUT The image data. If <code>data</code> is non-<code>NULL</code>, <code>dataStm</code> is 
	ignored. If there is a great deal of data, as for a large 
	image, it is recommended you use the <code>dataStm</code> parameter for 
	the image data or use the PDEImageCreateFromCosObj() method. 
	
	@param encodedLen IN/OUT The encoded length of <code>data</code> in bytes. 

	@param cosDoc IN/OUT The document in which the image is created.
	@param cosDoc IN/OUT Document in which to put Cos representation of resource. May be NULL. 

	@return The image. 
	@exception peErrUnknownPDEColorSpace 
	@exception pageErrReadLessImageData 
	@exception peErrWrongPDEObjectType 
	@exception genErrBadParm 
	@see PDEImageCreateFromCosObj 
	@see PDEImageCreateInCosDocEx
	@since PI_PDFEDIT_WRITE_VERSION >= 0x00080000

*/
#if ADD_ASFIXED //PKG
UNPROC(PDEImage, PDEImageCreateInCosDoc, (
			IN	PDEImageAttrsP attrsP, 
			IN	ASUns32 attrsSize,
			IN	ASFixedMatrixP matrixP,
			IN	ASUns32 flags,
			IN	PDEColorSpace colorSpace,
			IN	PDEColorValueP colorValueP,
			IN	PDEFilterArrayP filtersP,
			IN	ASStm dataStm, 
			IN	ASUns8 *data,
			IN	ASUns32 encodedLen,
			IN	CosDoc cosDoc)
	)
#endif //ADD_ASFIXED //PKG

#if 0
/**
	Creates a font object like PDEFontCreate(), except that the client can specify the CosDoc
	in which the font is created.

	<p>The PDEFont may be represented as an embedded font (a FontFile 
	entry in the font descriptor of the PDF file). To create 
	a PDEFont that is stored as an embedded font, the FontFile 
	stream may be passed in <code>fontStm</code>, and the <code>len1</code>, <code>len2</code>, and 
	<code>len3</code> parameters contain the <code>Length1</code>, <code>Length2</code>, and <code>Length3</code> 
	values of the FontFile stream attributes dictionary. See 
	Section 5.8 in the <i>PDF Reference</i> for more information about 
	embedded fonts. </p>

	<p>The caller must close <code>fontStm</code> with ASStmClose() after invoking 
	PDEFontCreate(). </p>

	<p>Call PDERelease() to dispose of the returned font object when 
	finished with it. </p>

	@param attrsP A pointer to a PDEFontAttrs structure for 
	the font attributes. 
	@param attrsSize The size of the <code>attrsP</code> buffer in bytes. 
	
	@param firstChar The first character index for the widths 
	array, <code>widthsP</code>. 
	@param lastChar The last character index for the widths array, 
	<code>widthsP</code>. 
	@param widthsP A pointer to the widths array. 
	@param encoding An array of 256 pointers to glyph names 
	specifying the custom encoding. If any pointer is <code>NULL</code>, 
	no encoding information is written for that entry. 
	@param encodingBaseName The encoding base name if the encoding 
	is a custom encoding. If the encoding is <code>NULL</code>, <code>encodingBaseName</code> 
	is used as the value of the encoding, and must be one of 
	<code>WinAnsiEncoding</code>, <code>MacRomanEncoding</code>, or <code>MacExpertEncoding</code>. 
	If no encoding value is desired, use ASAtomNull. 
	@param fontStm The stream with font information.
	@param len1 The length in bytes of the ASCII portion of the 
	Type 1 font file after it has been decoded. For other font 
	formats, such as TrueType or CFF, only <code>len1</code> is used, and 
	it is the size of the font. 
	@param len2 The length in bytes of the encrypted portion of 
	the Type 1 font file after it has been decoded. 
	@param len3 The length in bytes of the portion of the Type 
	1 font file that contains the 512 zeros, plus the <code>cleartomark</code> 
	operator, plus any following data. 
	@param cosDoc IN/OUT The document in which to put the Cos representation of resource. It may be <code>NULL</code>. 
	@return The specified PDEFont. 
	@exception peErrCantCreateFontSubset 
	@exception peErrCantGetAttrs 
	@exception peErrCantGetWidths 
	@exception peErrCantEmbedFont 
	@exception genErrResourceLoadFailed 
	@see PDEFontCreateFromCosObj 
	@see PDEFontCreateFromSysFont 
	@see PDEFontCreateFromSysFontEx 
	@see PDEFontCreateFromSysFontWithParams 
	@see PDEFontCreateWithParams 
	@since PI_PDFEDIT_WRITE_VERSION >= 0x00080000
*/
UNPROC(PDEFont, PDEFontCreateInCosDoc, (
			IN	PDEFontAttrsP attrsP,
			IN	ASUns32 attrsSize,
			IN	ASInt32 firstChar,
			IN	ASInt32 lastChar,
			IN	ASInt16 *widthsP, 
			IN	char **encoding,
			IN	ASAtom encodingBaseName, 
			IN	ASStm fontStm,
			IN	ASInt32 len1,
			IN	ASInt32 len2,
			IN	ASInt32 len3,
			IN	CosDoc cosDoc)
	)

/**
	Creates a font object like PDEFontCreateFromSysFont(), except that the client can specify the CosDoc
	in which the font is created.

	<p>Call PDERelease() to dispose of the returned font object when 
	finished with it. </p>

	<p>The PDEFontCreateFlags flags kPDEFontCreateEmbedded and 
	kPDEFontWillSubset must both be set in order to subset a 
	font.</p> 

	<p>If you create a PDEFont that is a subset, call PDEFontSubsetNow() 
	on this font afterwards. </p>
	@param sysFont A PDSysFont object referencing a system 
	font. 
	@param flags Indicates whether to embed the font and whether 
	to subset the font. It must be one of PDEFontCreateFlags. If 
	you want to subset a font, set both the kPDEFontCreateEmbedded 
	and kPDEFontWillSubset flags.
	@param cosDoc IN/OUT The document in which to put the Cos representation of resource. It may be <code>NULL</code>. 
	@return The PDEFont corresponding to <code>sysFont</code>. 
	@exception peErrCantCreateFontSubset 
	@exception peErrCantGetAttrs 
	@exception peErrCantGetWidths 
	@exception peErrCantEmbedFont 
	@exception genErrBadParm 
	@exception genErrResourceLoadFailed 
	@see PDEFontCreate 
	@see PDEFontCreateFromCosObj 
	@see PDEFontCreateFromSysFontAndEncoding 
	@see PDEFontCreateFromSysFontWithParams 
	@see PDEnumSysFonts 

	@note If you have an environment with no Acrobat Language kit 
	installed, trying to acquire a PDEFont from the system font 
	may raise an exception for some of the operating system fonts. In other 
	words, if you use PDEnumSysFonts() to get the system font 
	attributes, not all of the system fonts will necessarily 
	be used to create the PDEFont. 

	@note If you want to use <code>WinAnsiEncoding</code> on UNIX, do not 
	use this method. Use PDEFontCreateFromSysFontWithParams() 
	or PDEFontCreateFromSysFontAndEncoding() instead. 
	@since PI_PDFEDIT_WRITE_VERSION >= 0x00080000
*/
UNPROC(PDEFont, PDEFontCreateFromSysFontInCosDoc, (
			IN	PDSysFont sysFont,
			IN	ASUns32 flags,
			IN	CosDoc cosDoc)
	)			
			
/**
	Creates a font object like PDEFontCreateFromSysFontEx(), except that the client can specify the CosDoc
	in which the font is created.
	
	<p>If the font is a Multiple Master font, <code>mmDesignVector</code> points 
	to the design vector, whose length must equal the number 
	of design axes of the font. </p>

	<p>Call PDERelease() to dispose of the returned font object when 
	finished with it. </p>

	<p>The PDEFontCreateFlags flags kPDEFontCreateEmbedded and 
	kPDEFontWillSubset must both be set in order to subset a 
	font. </p>

	<p>If you create a PDEFont that is subsetted, call PDEFontSubsetNow() 
	on this font afterwards. </p>

	@param sysFont IN/OUT A PDSysFont object referencing a system 
	font. 
	@param flags IN/OUT Indicates whether to embed the font and whether 
	to subset the font. It must be one of PDEFontCreateFlags. If 
	you want to subset a font, set both the kPDEFontCreateEmbedded 
	and kPDEFontWillSubset flags. 
	@param snapshotName IN/OUT The name to be associated with this particular 
	instantiation of the PDEFont. 
	@param mmDesignVec IN/OUT A pointer to the Multiple Master font design 
	vector. 
	@param cosDoc IN/OUT The document in which to put the Cos representation of resource. It may be <code>NULL</code>. 
	@return The PDEFont corresponding to <code>sysFont</code>. 
	@exception peErrCantCreateFontSubset 
	@exception peErrCantGetAttrs 
	@exception peErrCantGetWidths 
	@exception peErrCantEmbedFont 
	@exception genErrBadParm 
	@exception genErrResourceLoadFailed 
	@see PDEFontCreateFromCosObj 
	@see PDEFontCreateFromSysFont 
	@see PDEFontCreateFromSysFontWithParams 
	@see PDEFontCreateWithParams 
	@see PDEnumSysFonts 

	@note If you have environment with no Acrobat Language kit 
	installed, trying to acquire a PDEFont from the system font 
	may raise an exception for some of the system fonts. In 
	other words, if you use PDEnumSysFonts() to get the system 
	font attributes, not all of the system fonts are necessarily 
	used to create the PDEFont. 
	@since PI_PDFEDIT_WRITE_VERSION >= 0x00080000

*/
UNPROC(PDEFont, PDEFontCreateFromSysFontExInCosDoc, (
			IN	PDSysFont sysFont,
			IN	ASUns32 flags,
			IN	ASAtom snapshotName,
			IN	ASReal *mmDesignVec,
			IN	CosDoc cosDoc)
	)


/**
	Creates a font object like PDEFontCreateWithParams(), except that the client can specify the CosDoc
	in which the font is created.
	Creates a new PDEFont from <code>params</code>. 

	<p>The PDEFont may be represented as an embedded font (a FontFile 
	value in PDF). To create a PDEFont that will be stored as 
	an embedded font, the FontFile stream may be passed as <code>fontStm</code>, 
	and the <code>len1</code>, <code>len2</code>, and <code>len3</code> parameters contain the <code>Length1</code>, 
	<code>Length2</code>, and <code>Length3</code> values of the FontFile. The caller 
	must close the <code>fontStm</code> after calling this method. This method 
	supports multi-byte fonts. </p>

	<p>This method extends PDEFontCreate() to support multi-byte 
	fonts. </p>

	<p>Call PDERelease() to dispose of the returned font object when 
	finished with it. </p>

	@param params IN/OUT A pointer to a structure containing all font 
	parameters necessary to fully define a font. 
	@param cosDoc IN/OUT The document in which to put the Cos representation of resource. It may be <code>NULL</code>. 
	@return A PDEFont object of the font described by the parameters. 
	
	@exception peErrCantCreateFontSubset 
	@exception peErrCantGetAttrs 
	@exception peErrCantGetWidths 
	@exception peErrCantEmbedFont 
	@exception genErrBadParm 
	@exception genErrResourceLoadFailed 
	@see PDEFontCreate 
	@see PDEFontCreateFromCosObj 
	@see PDEFontCreateFromSysFont 
	@see PDEFontCreateFromSysFontEx 
	@since PI_PDFEDIT_WRITE_VERSION >= 0x00080000

*/
UNPROC(PDEFont, PDEFontCreateWithParamsInCosDoc, (
			IN	PDEFontCreateParams params,
			IN	CosDoc cosDoc)
	)



/**
	Creates a font object like PDEFontCreateFromSysFontAndEncoding(), except that the client can specify the CosDoc
	in which the font is created.
	Create a PDEFont from <code>sysFont</code> and <code>sysEnc</code>. If it fails, it 
	raises an exception. User can call PDSysFontGetCreateFlags() 
	to see if the combination of sysFont and sysEnc makes sense. 
	
	<p>Call PDERelease() to dispose of the returned PDEFont object when finished with it. </p>
	@note If you want to use <code>WinAnsiEncoding</code> on UNIX, use this 
	method or PDEFontCreateFromSysFontWithParams(). 
	@param sysFont A PDSysFont object referencing a system 
	font. 
	@param sysEnc A PDSysEncoding object. 
	@param useThisBaseFont The base font. An exception will 
	be raised if the base font name passed is a subset name 
	<code>(XXXXXX+FontName)</code> or an empty string. 
	@param createFlags One of the PDEFontCreateFlags.
	@param cosDoc IN/OUT The document in which to put the Cos representation of resource. It may be <code>NULL</code>. 
	@return The newly created PDEFont object. 
	@since PI_PDFEDIT_WRITE_VERSION >= 0x00080000
*/
UNPROC(PDEFont, PDEFontCreateFromSysFontAndEncodingInCosDoc, (
			IN	PDSysFont		sysFont,
			IN	PDSysEncoding	sysEnc,
			IN	ASAtom			useThisBaseFont,
			IN	ASUns32			createFlags,
			IN	CosDoc			cosDoc)
		)
#endif

/**
	Creates a color space object like PDEColorSpaceCreate(), except that the client can specify the CosDoc
	in which the color space object is created.
	
	<p>Call PDERelease() to dispose of the returned color space object 
	when finished with it. </p>

	@param family IN/OUT Supports all PDF 1.3 color spaces, which 
	include: 

	<TABLE rules="all" cellspacing="1">
	<TR><TH>Type of names</TH><TH>Names</TH></TR>
	<TR><TD>Device-dependent names</TD><TD><ul><li><code>DeviceCMYK</code></li><li><code>DeviceGray</code></li><li><code>DeviceN</code></li><li><code>DeviceRGB</code></li></ul></TD></TR>
	<TR><TD>Device-independent names</TD><TD><ul><li><code>CalGray</code></li><li><code>CalRGB</code></li><li><code>Lab</code></li><li><code>ICCBased</code></li></ul></TD></TR>
	<TR><TD>Special names</TD><TD><ul><li><code>Indexed</code></li><li><code>Pattern</code></li><li><code>Separation</code></li></ul></TD></TR>
	</TABLE>

	@param csStruct IN/OUT Data for the type of color space you want 
	to create. 
	@param cosDoc IN/OUT The document in which to put the Cos representation of resource. It may be <code>NULL</code>. 
	@return The newly created color space object. 
	@exception cosErrExpectedArray 
	@exception genErrBadParm 
	@exception peErrUnknownPDEColorSpace 
	@see PDEColorSpaceCreateFromCosObj 
	@since PI_PDFEDIT_WRITE_VERSION >= 0x00080000

*/
#if 0 //PKG
UNPROC(PDEColorSpace, PDEColorSpaceCreateInCosDoc, (
			IN ASAtom				family,
			IN PDEColorSpaceStruct	*csStruct,
			IN CosDoc				cosDoc)
	)
#endif //ADD_ASFIXED //PKG

/**
	Removes unused objects from the PDFEdit scratch document, which is used to 
	hold representations of PDFEdit resources associated with a specific
	document.
	@since PI_PDFEDIT_WRITE_VERSION >= 0x00080000
	
*/
UNPROC(void, PDEScratchDocCleanup, (void)
	)

/**	Sets the default color space in a PDEContent object. 
	The reference count on any existing default color space
	is decremented, and the reference count on the new
	color space is incremented. Note that the new color
	space can be <code>NULL</code>, indicating that there is no default color space.

	<p>See <i>Section 4.5.4</i> in the <i>PDF Reference</i> for more information 
	about default color spaces.</p> 
	@param pdeContent     IN A content object. 
	@param colorSpaceName IN An ASAtom for the name of the desired 
	color space. It must be an ASAtom for one of the following:
	<ul>
		<li><code>DefaultRGB</code></li>
		<li><code>DefaultCMYK</code></li>
		<li><code>DefaultGray</code></li>
	</ul>
	@param colorSpace     IN The color space to use as the default.

	@since PI_PDFEDIT_WRITE_VERSION >= 0x00080000

*/
/* Note: This function was a friends function from 7.0 to 8.0 and was made public in 8.0*/

#if 0 //PKG
	UNPROC(void, PDEContentSetDefaultColorSpace_PEWCalls_, (
	IN	PDEContent pdeContent, 
	IN  ASAtom colorSpaceName,
	IN  PDEColorSpace colorSpace)
	)
#endif //0 //PKG

/** Gets the length of data for an image. 
	@param image IN/OUT The image whose data length is obtained. 
	@return The number of bytes of image data, specified by the width, height, 
	bits per component, and color space of the image. 
	Clients should switch to this routine. PDEImageGetDataLen() will raise an error if it encounters an image with a length that is larger than <code>2^31 - 1</code>.
	@exception peErrUnknownPDEColorSpace 
	@exception genErrBadParm 
	@exception peErrWrongPDEObjectType 
	@see PDEImageGetDataLen
	@since PI_PDFEDIT_READ_VERSION >= 0x00080000

*/

	UNPROC(ASInt64, PDEImageGetDataLen64, (
	IN PDEImage image)
	)

/** 
	Superseded by PDEImageCreateInCosDocEx() in Acrobat 10.0.
	
	Creates an image object like PDEImageCreateInCosDoc(), except that the client can create an image
	with a large amount of data.

	<p>The image data may be specified as a stream or as a buffer. 
	If <code>data</code> is non-<code>NULL</code>, <code>dataStm</code> is ignored. </p>

	<p>See PDEImageSetDataStm() for information on handling the stream. </p>

	<p>The caller must dispose of <code>dataStm</code> after calling this method.</p> 

	<p>Call PDERelease() to dispose of the returned image object 
	when finished with it. </p>


	@param attrsP IN/OUT A pointer to a PDEImageAttrs object with attributes 
	of the image. 
	@param attrsSize IN/OUT The size of the <code>attrsP</code> buffer in bytes. 
	@param matrixP IN/OUT A pointer to an ASFixedMatrix that holds the 
	transformation matrix to use for the image. 
	@param flags IN/OUT PDEImageDataFlags flags. If the kPDEImageEncodedData 
	flag is set, and the data is provided directly (not as a 
	stream), then <code>encodedLen</code> must specify the length of data. 

	@param colorSpace IN/OUT The color space of the image. When the image 
	is an image mask, <code>colorSpace</code> is the color space of the <code>colorValueP</code> 
	argument. 
	@param colorValueP IN/OUT A pointer to a PDEColorValue structure. 
	If the image is an image mask, <code>colorValueP</code> must be provided. 

	@param filtersP IN/OUT A pointer to a PDEFilterArray structure that 
	specifies which filters to use in encoding the contents; 
	it may be <code>NULL</code>. Filters will be used to encode the data in 
	the order in which they are specified in the array. 
	@param dataStm IN/OUT The stream holding the image data. 
	@param data IN/OUT The image data. If <code>data</code> is non-<code>NULL</code>, <code>dataStm</code> is 
	ignored. If there is a great deal of data, as for a large 
	image, it is recommended you use the <code>dataStm</code> parameter for 
	the image data or use the PDEImageCreateFromCosObj() method. 

	@param encodedLen IN/OUT The encoded length of <code>data</code> in bytes. 
	
	@param cosDoc IN/OUT The document in which the image is created.
	@param cosDoc IN/OUT The document in which to put the Cos representation of the resource. It may be <code>NULL</code>. 


	@return The image. 
	@exception peErrUnknownPDEColorSpace 
	@exception pageErrReadLessImageData 
	@exception peErrWrongPDEObjectType 
	@exception genErrBadParm 
	@see PDEImageCreateFromCosObj 
	@see PDEImageCreateInCosDocEx
	@since PI_PDFEDIT_WRITE_VERSION >= 0x00080000

*/

#if ADD_ASFIXED //PKG
	UNPROC(PDEImage, PDEImageCreateInCosDoc64, (
	IN PDEImageAttrsP attrsP, 
	IN ASUns32 attrsSize,
	IN ASFixedMatrixP matrixP,
	IN ASUns32 flags,
	IN PDEColorSpace colorSpace,
	IN PDEColorValueP colorValueP,
	IN PDEFilterArrayP filtersP,
	IN ASStm dataStm, 
	IN ASUns8 *data,
	IN ASUns64 encodedLen,
	IN CosDoc cosDoc)
	)
#endif //ADD_ASFIXED //PKG

/**
	<p>Creates an encoding object from a given PDF CMap stream.</p>

	<p>Call <code>PDERelease()</code> to dispose of the returned <code>PDSysEncoding</code> object when it is no longer needed. </p>
	@param cmapStream The CMap stream from which to create the encoding object.
	@return The encoding object to be created. 
	@since PI_PDFEDIT_WRITE_VERSION >= 0x00090000
*/
#if 0
	UNPROC(PDSysEncoding, PDSysEncodingCreateFromCMapStream, (
	IN	CosObj cmapStream)
	)
#endif

/**
	Sets the <code>PDEContentToCosObjFlags</code> for this form.
	@since PI_PDFEDIT_WRITE_VERSION >= 0x00090000
	@exception genErrBadParm
*/
	UNPROC(void, PDEFormSetContentToCosObjFlags, (
	IN	PDEForm form,
	IN	ASUns32 flags)
	)

/**
	Supersedes PDEFormGetMatrix() in Acrobat 10.0.
	
	Gets the matrix for a PDEform. 
	<p>The result is a concatenation of the CTM and the Cos level form matrix, resulting in the transformation from the form space to the device space.</p>
	@note For the other elements, PDEElementGetMatrixEx() would give correct results.
	@param form The form for which the matrix is required.
	@param matrixP The resultant matrix.

	@see PDEElemetGetMatrix
	@see PDEFormGetBBox
	@see PDEFormGetMatrix

	@since PI_PDFEDIT_WRITE_VERSION >= 0x000A0000

*/

UNPROC(void, PDEFormGetMatrixEx, (
			IN PDEForm form,
			OUT ASDoubleMatrixP	matrixP)
	)
/**
	Supersedes PDEContentAddPage() in Acrobat 10.0.
	
	Adds the specfied PDPage to the PDEContent as an Xobject form.
	It adds a reference to the Xobject form at the indicated index in
	the PDE Content; the index may be less than <code>0</code>, which indicates the object is
	to be appended to the content.
	@param theContent The content to set for the page.
	@param insertAfterIndex The index indicates the location after which the specified
	element is to be added. The index should be kPDBeforeFirst to add to the beginning
	of the display list.
	@param containerDoc The CosDoc in which the page is contained.
	@param srcPage The page that will be inserted at <code>insertAfterIndex</code> in <code>theContent</code>.
	@param dstMatrix (May be <code>NULL</code>) The matrix applied to the default matrix of the PDPage that is inserted
	into the CosDoc.
	@param annotTypes If the page contains annotations, the <code>annotTypes</code> list is used to determine
	which annotation types are pumped into the page contents of the CosDoc.
	@param flags (May be <code>0</code>) kAnnotAll specifies all annotation types.  If this is not set, then
	the <code>annotTypes</code> list will be consulted.
	@param bbox (May be <code>NULL</code>) specifies the destination <code>BBox</code> for the page being inserted.
	If it is <code>NULL</code>, the new page's media box is used.
	@see PDEContentAddPage
	@since PI_PDFEDIT_WRITE_VERSION >= 0x000A0000
*/
XNPROC(void, PDEContentAddPageEx, (OUT PDEContent theContent, 
			IN	ASInt32 insertAfterIndex, 
			IN	CosDoc containerDoc,
			IN	PDPage srcPage, 
			IN	ASDoubleMatrix * dstMatrix,
			IN	ASAtom annotTypes[],
			IN	ASInt32 flags,
			IN	ASDoubleRect * bbox)
	  )

/**
	Supersedes PDEShadingCreateFromCosObj() in Acrobat 10.0.
	
	Creates a smooth shading object. 
	<p>Call PDERelease() to dispose of the returned PDEShading object when finished with it. </p>
	@param shadingP IN/OUT The shading dictionary. 
	@param matrixP IN/OUT The location and transformation matrix of 
	the shading object. 
	@return A smooth shading object. 
	@exception peErrUnknownPDEColorSpace 
	@exception cosErrInvalidObj 
	@exception cosErrExpectedName 
	@exception genErrBadParm 
	@see PDEShadingCreateFromCosObj
	@since PI_PDFEDIT_WRITE_VERSION >= 0x000A0000

*/
UNPROC(PDEShading, PDEShadingCreateFromCosObjEx, (
			IN	const CosObj *shadingP,
			IN	ASDoubleMatrixP matrixP)
	)

/** 
	Supersedes PDEImageCreateInCosDoc() and PDEImageCreateInCosDoc64() in Acrobat 10.0.
	
	Creates an image object like PDEImageCreateInCosDoc(), except that the client can create an image
	with a large amount of data, and using a double precision transformation matrix.

	<p>The image data may be specified as a stream or as a buffer. 
	If <code>dataStm</code> is non-<code>NULL</code>, <code>data</code> is ignored. </p>

	<p>See PDEImageSetDataStm() for information on handling the stream. </p>

	<p>The caller must dispose of <code>dataStm</code> after calling this method.</p> 

	<p>Call PDERelease() to dispose of the returned image object 
	when finished with it. </p>


	@param attrsP IN/OUT A pointer to a PDEImageAttrs object with attributes 
	of the image. 
	@param attrsSize IN/OUT The size of the <code>attrsP</code> buffer in bytes. 
	@param matrixP IN/OUT A pointer to an ASDoubleMatrix that holds the 
	transformation matrix to use for the image. 
	@param flags IN/OUT PDEImageDataFlags flags. If the kPDEImageEncodedData 
	flag is set, and the data is provided directly (not as a 
	stream), then <code>encodedLen</code> must specify the length of data. 

	@param colorSpace IN/OUT The color space of the image. When the image 
	is an image mask, <code>colorSpace</code> is the color space of the <code>colorValueP</code> 
	argument. 
	@param colorValueP IN/OUT A pointer to a PDEColorValue structure. 
	If the image is an image mask, <code>colorValueP</code> must be provided. 

	@param filtersP IN/OUT A pointer to a PDEFilterArray structure that 
	specifies which filters to use in encoding the contents; 
	it may be <code>NULL</code>. Filters will be used to encode the data in 
	the order in which they are specified in the array. 
	@param dataStm IN/OUT The stream holding the image data. 
	@param data IN/OUT The image data. If <code>dataStm</code> is non-<code>NULL</code>, <code>data</code> is 
	ignored. If there is a great deal of data, as for a large 
	image, it is recommended you use the <code>dataStm</code> parameter for 
	the image data or use the PDEImageCreateFromCosObjEx() method. 

	@param encodedLen IN/OUT The encoded length of <code>data</code> in bytes. 
	
	@param cosDoc IN/OUT The document in which the image is created.
	@param cosDoc IN/OUT The document in which to put the Cos representation of the resource. It may be <code>NULL</code>. 


	@return The image. 
	@exception peErrUnknownPDEColorSpace 
	@exception pageErrReadLessImageData 
	@exception peErrWrongPDEObjectType 
	@exception genErrBadParm 
	@see PDEImageCreateFromCosObjEx
	@see PDEImageCreateInCosDoc
	@see PDEImageCreateInCosDoc64
	@since PI_PDFEDIT_WRITE_VERSION >= 0x000A0000

*/

	UNPROC(PDEImage, PDEImageCreateInCosDocEx, (
	IN PDEImageAttrsP attrsP, 
	IN ASUns32 attrsSize,
	IN ASDoubleMatrixP matrixP,
	IN ASUns32 flags,
	IN PDEColorSpace colorSpace,
	IN PDEColorValueP colorValueP,
	IN PDEFilterArrayP filtersP,
	IN ASStm dataStm, 
	IN ASUns8 *data,
	IN ASUns64 encodedLen,
	IN CosDoc cosDoc)
	)

/**
	Supersedes PDEImageCreate() in Acrobat 10.0.
	
	Creates an image object. 

	<p>The image data may be specified as a stream or as a buffer. 
	If <code>dataStm</code> is non-<code>NULL</code>, <code>data</code> is ignored. </p>

	<p>See PDEImageSetDataStm() for information on handling the stream. </p>
	
	<p>The caller must dispose of <code>dataStm</code> after calling this method.</p> 
	
	<p>Call PDERelease() to dispose of the returned image object 
	when finished with it. </p>

	@param attrsP IN/OUT A pointer to a PDEImageAttrs object with attributes 
	of the image. 
	@param attrsSize IN/OUT The size of the <code>attrsP</code> buffer in bytes. 
	@param matrixP IN/OUT A pointer to an ASDoubleMatrix that holds the 
	transformation matrix to use for the image. 
	@param flags IN/OUT PDEImageDataFlags flags. If the kPDEImageEncodedData 
	flag is set, and the data is provided directly (not as a 
	stream), then <code>encodedLen</code> must specify the length of data. 
	
	@param colorSpace IN/OUT The color space of the image. When the image 
	is an image mask, <code>colorSpace</code> is the color space of the <code>colorValueP</code> 
	argument. 
	@param colorValueP IN/OUT A pointer to a PDEColorValue structure. 
	If the image is an image mask, <code>colorValueP</code> must be provided. 
	
	@param filtersP IN/OUT A pointer to a PDEFilterArray structure that 
	specifies which filters to use in encoding the contents; 
	it may be <code>NULL</code>. Filters will be used to encode the data in 
	the order in which they are specified in the array. 
	@param dataStm IN/OUT The stream holding the image data. 
	@param data IN/OUT The image data. If <code>dataStm</code> is non-<code>NULL</code>, <code>data</code> is 
	ignored. If there is a great deal of data, as for a large 
	image, it is recommended you use the <code>dataStm</code> parameter for 
	the image data or use the PDEImageCreateFromCosObjEx() method. 
	
	@param encodedLen IN/OUT The encoded length of <code>data</code> in bytes. 
	@return The image. 
	@exception peErrUnknownPDEColorSpace 
	@exception pageErrReadLessImageData 
	@exception peErrWrongPDEObjectType 
	@exception genErrBadParm 
	@see PDEImageCreateFromCosObjEx
	@see PDEImageCreate
	@since PI_PDFEDIT_WRITE_VERSION >= 0x000A0000

*/
UNPROC(PDEImage, PDEImageCreateEx, (
			IN	PDEImageAttrsP attrsP, 
			IN	ASUns32 attrsSize,
			IN	ASDoubleMatrixP matrixP,
			IN	ASUns32 flags,
			IN	PDEColorSpace colorSpace,
			IN	PDEColorValueP colorValueP,
			IN	PDEFilterArrayP filtersP,
			IN	ASStm dataStm, 
			IN	ASUns8 *data,
			IN	ASUns64 encodedLen)
	)


/**
	Supersedes PDETextRunSetTextMatrix() in Acrobat 10.0.
	
	Sets the text matrix of a text run. 
	@param pdeText IN/OUT The text object containing a text run whose 
	text matrix is set. 
	@param runIndex IN/OUT The index of the text run. 
	@param matrixP IN/OUT A pointer to an ASDoubleMatrix that holds the 
	text matrix. 
	@exception peErrWrongPDEObjectType 
	@exception genErrBadParm 
	@see PDETextGetTextMatrixEx 
	@see PDETextRunSetMatrixEx 
	@see PDETextRunSetTextMatrix
	@since PI_PDFEDIT_WRITE_VERSION >= 0x000A0000

*/
UNPROC(void, PDETextRunSetTextMatrixEx, (
			IN	PDEText pdeText, 
			IN	ASInt32 runIndex, 
			IN	ASDoubleMatrixP matrixP)
	)

/**
	Supersedes PDETextRunSetStrokeMatrix() in Acrobat 10.0.
	
	Sets the stroke matrix of a text run. 
	@param pdeText The text object containing a text run whose 
	stroke matrix is set. 
	@param runIndex The index of the text run. 
	@param matrixP A pointer to an ASDoubleMatrix that holds the 
	stroke matrix.
	@exception peErrWrongPDEObjectType 
	@exception genErrBadParm 
	@see PDETextGetStrokeMatrixEx 
	@See PDETextRunSetStrokeMatrix

	@note Currently this method is not implemented (Acrobat 10 and later).
	@since PI_PDFEDIT_WRITE_VERSION >= 0x000A0000
*/
UNPROC(void, PDETextRunSetStrokeMatrixEx, (
			IN	PDEText pdeText, 
			IN	ASInt32 runIndex, 
			IN	ASDoubleMatrixP matrixP)
	)

/**
	Supersedes PDEImageCreateFromCosObj() in Acrobat 10.0.
	
	Creates an image object from a Cos object. 

	<p>Call PDERelease() to dispose of the returned image object 
	when finished with it. </p>
	@param imageObjP IN/OUT The Cos object for the image. 
	@param matrixP IN/OUT A pointer to an ASDoubleMatrix that holds the 
	transformation matrix to use for the image. 
	@param colorSpace IN/OUT The color space used for the image, if the 
	image is an image mask; otherwise, set it to <code>NULL</code>. 
	@param colorValueP IN/OUT A pointer to a PDEColorValue structure. 
	If the image is an image mask, <code>colorValueP</code> must be provided. 
	
	@return An image corresponding to the Cos object. 
	@exception peErrUnknownPDEColorSpace 
	@exception pageErrReadLessImageData 
	@exception peErrWrongPDEObjectType 
	@exception genErrBadParm 
	@see PDEImageCreateEx 
	@see PDEImageGetCosObj
	@see PDEImageCreateFromCosObj
	@since PI_PDFEDIT_WRITE_VERSION >= 0x000A0000

*/
UNPROC(PDEImage, PDEImageCreateFromCosObjEx, (
			IN	const CosObj *imageObjP,
			IN	ASDoubleMatrixP matrixP,
			IN	PDEColorSpace colorSpace,
			IN	PDEColorValueP colorValueP)
	)

/**
	Supersedes PDETextItemCreate() in Acrobat 10.0.
	
	Creates a text item element containing a character or text 
	run, which can be added to a PDEText text object. 
	<p>Call PDERelease() to dispose of the returned PDETextItem object when finished with it. </p>
	@param text A pointer to the characters to add. Note that passing 
	<code>NULL</code> for text can invalidate the text object but will not 
	raise an error. Callers must not pass <code>NULL</code> for this parameter. 
	
	@param textLen The length of the text in bytes. 
	@param font The font for the element. 
	@param gStateP A pointer to a PDEGraphicStateP structure 
	with the graphics state for the element. 
	@param gStateLen The length of the graphics state for the element. 
	
	@param textStateP A pointer to a PDETextState structure with the
	text state for the element. Note that PDFEdit ignores the <code>wasSetFlags</code> 
	flag of the PDETextState structure, so you must initialize 
	the PDETextState fields. 
	@param textStateLen The length of the text state for the element. 
	
	@param textMatrixP A pointer to an ASDoubleMatrix that holds 
	the matrix for the element.
	@exception pdErrBadResMetrics 
	@exception peErrWrongPDEObjectType 
	@exception genErrBadParm 
	@see PDETextAddEx 
	@see PDETextAddItem 
	@see PDETextItemCreate
	@since PI_PDFEDIT_WRITE_VERSION >= 0x000A0000
*/
UNPROC(PDETextItem, PDETextItemCreateEx, (
			IN	ASUns8				*text,
			IN	ASUns32				textLen,
			IN	PDEFont				font,
			IN	PDEGraphicStateP	gStateP,
			IN	ASUns32				gStateLen,
			IN	PDETextStateP		textStateP,
			IN	ASUns32				textStateLen,
			IN	ASDoubleMatrix		*textMatrixP))

/**
	Supersedes PDETextItemSetTextMatrix() in Acrobat 10.0.
	
	Sets the text matrix for a text item. 
	@param textItem The text item whose text matrix is set. 
	
	@param textMatrixP A pointer to a ASDoubleMatrix structure 
	with the new text matrix of the text item.
	@exception peErrWrongPDEObjectType 
	@exception genErrBadParm 
	@exception pdErrBadResMetrics 
	@see PDETextItemGetTextMatrixEx
	@see PDETextItemSetTextMatrix
	@since PI_PDFEDIT_WRITE_VERSION >= 0x000A0000
*/
UNPROC(void, PDETextItemSetTextMatrixEx, (
			IN	PDETextItem			textItem,
			IN	ASDoubleMatrix		*textMatrixP))


/**
	Supersedes PDETextAddGlyphs() in Acrobat 10.0.
	
	Adds Unicode text to a PDEText object. 

	@param pdeText The text object to which a character or text 
	run is added. 
	@param flags A PDETextFlags that specifies what kind of 
	text to add. 
	It must be one of the following values: 

	<TABLE rules="all" cellspacing="1">
	<TR><TH>Value</TH><TH>Description</TH></TR>
	<TR><TD>kPDETextChar</TD><TD>Used for a text character.</TD></TR>
	<TR><TD>kPDETextRun</TD><TD>Used for a text run.</TD></TR>
	</TABLE>

	@param index The index after which to add the character or text 
	run. 
	@param glyphRun A pointer to a PDEGlyphRun structure
	with Unicode data, GlyphIDs and their correspondence.
	@param font The font for the element. 
	@param gstateP A pointer to a PDEGraphicStateP structure 
	with the graphics state for the element. 
	@param gstateLen The length of the graphics state for the element.
	@param tstateP A pointer to a PDETextState structure with 
	text state for the element. Note that PDFEdit ignores the <code>wasSetFlags</code> 
	flag of the PDETextState structure, so you must initialize 
	the PDETextState fields. 
	@param tstateLen The length of the text state for the element. 
	@param textMatrixP A pointer to an ASDoubleMatrix that holds 
	the matrix for the element. 
	@param strokeMatrixP A pointer to an ASDoubleMatrix that holds 
	the matrix for the line width when stroking text. It may be 
	<code>NULL</code>. Note that, currently, this field is not used. (Acrobat 5)
	@exception pdErrBadResMetrics 
	@exception peErrWrongPDEObjectType 
	@exception genErrBadParm 
	@see PDETextAddEx
	@see PDETextAddGlyphs

	@note This method does not change the reference count of 
	<code>pdeText</code>; however, the reference count of the objects in 
	the <code>gstateP</code> parameter are incremented. 
	@since PI_PDFEDIT_WRITE_VERSION >= 0x000A0000
*/
#if 0
UNPROC(void, PDETextAddGlyphsEx, (
			IN	PDEText pdeText, 
			IN	ASUns32 flags,			/* kPDETextRun, kPDETextChar */ 
			IN	ASInt32 index,
			IN	PDEGlyphRunP glyphRun,
			IN	PDEFont font,
			IN	PDEGraphicStateP gstateP,
			IN	ASUns32 gstateLen,
			IN	PDETextStateP tstateP,
			IN	ASUns32 tstateLen,
			IN	ASDoubleMatrixP textMatrixP,
			IN	ASDoubleMatrixP strokeMatrixP)
	)

#endif

/**
	Supersedes PDETextAdd() in Acrobat 10.0.
	
	Adds a character or a text run to a PDEPath text object. 

	@note This method does not change the reference count of 
	<code>pdeText</code>; however, the reference count of the objects in 
	the <code>gstateP</code> parameter are incremented. 
	@param pdeText The text object to which a character or text 
	run is added. 
	@param flags A PDETextFlags that specifies what kind of 
	text to add. It must be one of the following values: 

	<TABLE rules="all" cellspacing="1">
	<TR><TH>Value</TH><TH>Description</TH></TR>
	<TR><TD>kPDETextChar</TD><TD>Used for a text character.</TD></TR>
	<TR><TD>kPDETextRun</TD><TD>Used for a text run.</TD></TR>
	</TABLE>

	@param index The index after which to add the character or text 
	run. 
	@param text A pointer to the characters to add. Note that passing 
	<code>NULL</code> for the text can invalidate the text object, but will not 
	raise an error. Callers must not pass <code>NULL</code> for this parameter. 
	
	@param textLen The length of the text in bytes. 
	@param font The font for the element. 
	@param gstateP A pointer to a PDEGraphicStateP structure 
	with the graphics state for the element. 
	@param gstateLen The length of the graphics state for the element.
	@param tstateP A pointer to a PDETextState structure with 
	the text state for the element. Note that PDFEdit ignores the <code>wasSetFlags</code> 
	flag of the PDETextState structure, so you must initialize 
	the PDETextState fields. 
	@param tstateLen The length of the text state for the element. 
	
	@param textMatrixP A pointer to an ASDoubleMatrix that holds 
	the matrix for the element. 
	@param strokeMatrixP A pointer to an ASDoubleMatrix that holds 
	the matrix for the line width when stroking text. It may be 
	<code>NULL</code>. Note that this field is not currently used (Acrobat 5 and later).
	@exception pdErrBadResMetrics 
	@exception peErrWrongPDEObjectType 
	@exception genErrBadParm 
	@see PDETextIsAtPoint 
	@see PDETextReplaceChars 
	@see PDETextSplitRunAt
	@see PDETextAdd
	@since PI_PDFEDIT_WRITE_VERSION >= 0x000A0000
*/
UNPROC(void, PDETextAddEx, (
			IN	PDEText pdeText, 
			IN	ASUns32 flags,			/* kPDETextRun, kPDETextChar */ 
			IN	ASInt32 index,
			IN	ASUns8 *text, 
			IN	ASInt32 textLen, 
			IN	PDEFont font,
			IN	PDEGraphicStateP gstateP,
			IN	ASUns32 gstateLen,
			IN	PDETextStateP tstateP,
			IN	ASUns32 tstateLen,
			IN	ASDoubleMatrixP textMatrixP,
			IN	ASDoubleMatrixP strokeMatrixP)
	)


/**
	Supersedes PDEElementSetMatrix() in Acrobat 10.0.
	
	Sets the transformation matrix for an element. 

	<p>The element may not be a PDEContainer, a PDEGroup, a PDEPlace, 
	or a PDEText. </p>

	@param pdeElement IN/OUT An element whose transformation matrix 
	is set. 
	@param matrixP IN/OUT A pointer to an ASDoubleMatrix that holds the 
	transformation matrix to set for <code>pdeContent</code>. 
	@exception peErrWrongPDEObjectType 
	@exception genErrBadParm 
	@see PDEElementGetMatrixEx 
	@see PDEElementSetMatrix
	@since PI_PDFEDIT_WRITE_VERSION >= 0x000A0000

*/
UNPROC(void, PDEElementSetMatrixEx, (
			IN	PDEElement pdeElement, 
			IN	ASDoubleMatrixP matrixP)
	)


/**
	Supersedes PDEFormCreateFromCosObj() in Acrobat 10.0.
	
	Creates a new form from an existing Cos object. 

	<p>Call PDERelease() to dispose of the returned form object when 
	finished with it. </p>

	@param xObjectP The Cos object from which a PDEForm is created. 
	
	@param resourcesP The <code>xObjectP</code> parameter's Resources dictionary. 
	If you do not pass in a Resource object, subsequent calls 
	to PDPageAcquirePDEContent() will fail (after the file is 
	saved). 
	@param matrixP A pointer to an ASDoubleMatrix that holds the 
	transformation matrix to use for the form.
	@return The newly created form object. 
	@see PDEFormCreateClone 
	@see PDEFormGetCosObj 
	@see PDEFormCreateFromCosObj
	@since PI_PDFEDIT_WRITE_VERSION >= 0x000A0000
*/
UNPROC(PDEForm, PDEFormCreateFromCosObjEx, (
			IN	const CosObj *xObjectP,
			IN	const CosObj *resourcesP,
			IN	ASDoubleMatrixP matrixP)
	)


/**
	Supersedes PDETextRunSetMatrix() in Acrobat 10.0.
	
	Sets the matrix of a text run. Unlike PDETextRunSetTextMatrixEx(), 
	this function does not change <code>fontSize</code>, <code>hScale</code>, and <code>textRise</code> 
	in the <code>textState</code> of PDEText. 
	@param pdeText IN/OUT The text object containing a text run. 
	@param runIndex IN/OUT The index of the text run. 
	@param matrixP IN/OUT ASDoubleMatrixP pointer. 
	@exception peErrWrongPDEObjectType 
	@exception genErrBadParm 
	@see PDETextRunSetTextMatrixEx 
	@see PDETextRunSetMatrix
	@since PI_PDFEDIT_WRITE_VERSION >= 0x000A0000

*/
UNPROC(void, PDETextRunSetMatrixEx, (
			IN	PDEText pdeText, 
			IN	ASInt32 runIndex, 
			IN	ASDoubleMatrixP matrixP)
	)

/**
	Sets new path data for a path element. This API is an extension 
	to the <code>PDEPathSetData</code> API.
	@param path IN/OUT The path whose data is set. 
	@param data IN/OUT A pointer to the path data. It is a variable-sized 
	array of path operators and operands. The format is a 32-bit operator 
	followed by zero to three ASReal values, depending 
	on the operator. Operators are codes for <code>moveto</code>, <code>lineto</code>, 
	<code>curveto</code>, <code>rect</code>, or <code>closepath</code> operators, and must be one of 
	PDEPathElementType. Operands are ASReal values. The 
	data is copied into the PDEPath object. 
	@param dataSize IN/OUT The size of the new path data in bytes. 
	@exception peErrWrongPDEObjectType 
	@exception genErrBadParm 
	@see PDEPathGetData 
	@see PDEPathSetData 
	@see PDEPathGetDataEx 
	@since PI_PDFEDIT_WRITE_VERSION >= 0x000A0000

*/
UNPROC(void, PDEPathSetDataEx, (
			IN	PDEPath path, 
			IN	ASReal *data, 
			IN	ASUns32 dataSize)
	)


/**
	Sets the black point compensation.
	Valid names are ON, OFF and Default.
	An exception will be raised if the name is invalid.

	@param extGS The extended graphics state object.
	@param BPC New value for BPC
	@exception genErrBadParm

	@see PDEExtGStateGetBPC
	@since PI_PDFEDIT_WRITE_VERSION >= 0x000C0000
*/
UNPROC(void, PDEExtGStateSetBPC, (
	IN	PDEExtGState	extGS,
	IN	ASAtom			BPC))
	
/**
	Sets HalfTone Co-ordinate point.

	@param extGS The extended graphics state object.
	@param <code>ASRealPoint*</code> for x,y coordinate value of HalfToneOrigin.
	@exception genErrBadParm

	@see PDEExtGStateSetHalfToneOrigin
	@since PI_PDFEDIT_WRITE_VERSION >= 0x000B0000
*/
	UNPROC(void, PDEExtGStateSetHalfToneOrigin, (
		IN	PDEExtGState	extGS,
		IN	ASRealPoint 	hto_point))

