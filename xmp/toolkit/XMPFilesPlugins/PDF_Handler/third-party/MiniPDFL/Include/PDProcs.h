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
 Copyright (C) 1994-2006 Adobe Systems Incorporated
 All rights reserved.

 NOTICE: Adobe permits you to use, modify, and distribute this file
 in accordance with the terms of the Adobe license agreement
 accompanying it. If you have received this file from a source other
 than Adobe, then your use, modification, or distribution of it
 requires the prior written permission of Adobe.

 ---------------------------------------------------------------------

 PDProcs.h

 - Catalog of functions exported by the PDModel HFT.

*********************************************************************/

#if !EXTERNAL_PDPROCS_USER  /* External user of this header file, e.g. PISDK */

#if CAN_EDIT && !READER /* Restore X Macros -- was !CAN_EDIT */
#define XNPROC NPROC
#define XPROC PROC
#define XSPROC SPROC
#else
#define XNPROC(returnType, name, params) NOPROC(name)
#define XPROC(returnType, name, params) NOPROC(name)
#define XSPROC(returnType, name, params, stubProc) NOPROC(name)
#endif /* READER */

#if CAN_ENUM_OBJECTS
#define ENPROC NPROC
#define EPROC PROC
#define ESPROC SPROC
#else
#define ENPROC(returnType, name, params) NOPROC(name)
#define EPROC(returnType, name, params) NOPROC(name)
#define ESPROC(returnType, name, params, stubProc) NOPROC(name)
#endif /* CAN_ENUM_OBJECTS */

#endif

/**
	Creates a new action object. 
	@param doc The document in which the action is created. 
	
	@param type The ASAtom corresponding to the action's subtype. 
	The ASAtom can be obtained from a string using ASAtomFromString().
	@return The newly created PDAction. 
	@see PDActionNewFromDest 
	@see PDActionNewFromFileSpec 
	@see PDActionFromCosObj 
	@see PDActionDestroy 
	@since PI_PDMODEL_VERSION >= 0x00020000
*/
UNPROC(PDAction, PDActionNew, (PDDoc doc, ASAtom type))

/**
	Creates a new action that takes the user to the specified 
	destination view. This method can only be used for destinations 
	in the same document as the source document. Cross-document 
	links must be built up from the Cos level, populating the 
	Action dictionary for the GotoR action as described in Section 
	8.5.3 in the <i>PDF Reference</i>. 
	@param doc The document in which the action is created 
	and used. 
	@param dest The destination view. 
	@param destDoc The destination document. <code>destDoc</code> must be the 
	same as <code>doc</code>.
	@return The newly created action. 
	@see PDActionNew 
	@see PDActionNewFromFileSpec 
	@see PDActionFromCosObj 
	@see PDActionDestroy 
	@since PI_PDMODEL_VERSION >= 0x00020000
*/
UNPROC(PDAction, PDActionNewFromDest, (PDDoc doc, PDViewDestination dest, PDDoc destDoc))

/**
	Creates an action of the specified type from a file specification. 
	
	@param containingDoc The document in which the action 
	is created and used. 
	@param type The type of action to create. 
	@param fileSpec The file specification that is made part 
	of an action.
	@return The newly created PDAction. 
	@see PDActionNew 
	@see PDActionNewFromDest 
	@see PDActionFromCosObj 
	@see PDActionDestroy 
	@since PI_PDMODEL_VERSION >= 0x00020000
*/
UNPROC(PDAction, PDActionNewFromFileSpec, (PDDoc containingDoc, ASAtom type, PDFileSpec fileSpec))

/**
	Destroys an action object. 
	@param action IN/OUT The action to destroy. 
	@see PDActionNew 
	@see PDActionNewFromDest 
	@see PDActionNewFromFileSpec 
	@see PDActionFromCosObj 
	@since PI_PDMODEL_VERSION >= 0x00020000
*/
UNPROC(void, PDActionDestroy, (PDAction action))

/**
	Tests whether an action is valid. This method can be used 
	in the following cases:
	<ul>
	<li>To determine whether a PDAction returned from a method 
	is really an action. For example, calling PDLinkAnnotGetAction() 
	returns an invalid action if no action is associated with 
	the link annotation. </li>
	<li>To ensure that an action has not been deleted.</li>
	</ul>
	
	@param action The action whose validity is determined.
	@return <code>true</code> if the action is valid, <code>false</code> otherwise. 
	@see PDActionEqual 
	@since PI_PDMODEL_VERSION >= 0x00020000
*/
NPROC(ASBool, PDActionIsValid, (PDAction action))

/**
	Gets an action's subtype. 
	@param action IN/OUT The action whose subtype is obtained. 
	@return The ASAtom corresponding to the action's subtype. The ASAtom 
	can be converted to a string using ASAtomGetString(). 
	@since PI_PDMODEL_VERSION >= 0x00020000
*/
NPROC(ASAtom, PDActionGetSubtype, (PDAction action))

/**
	Compares two actions for equality. Two actions are equal 
	only if their Cos objects are equal (see CosObjEqual()). 
	
	@param action The first actions to be compared. 
	@param action2 The second action to be compared.
	@return <code>true</code> if the actions are equal, <code>false</code> otherwise. 
	
	@see CosObjEqual 
	@since PI_PDMODEL_VERSION >= 0x00020000
*/
NPROC(ASBool, PDActionEqual, (PDAction action, PDAction action2))

/**
	Gets an action's destination view. This only works for actions 
	that contain a view destination; that is, actions whose subtype 
	is GoTo. 

	<p>For named destinations, this method may return a Cos string 
	object or a Cos name object. See Section 8.2.1 in the <i>PDF 
	Reference</i> for more information on named destinations. </p>

	@param action The action whose destination is obtained.
	@return The action's destination, which may be a PDViewDestination, or 
	for named destinations, a Cos string object or a Cos name 
	object. Use the PDViewDestResolve() method on this returned 
	value to obtain a PDViewDestination. 
	@see PDActionGetFileSpec 
	@see PDViewDestResolve 

	@note Since this method may not return a PDViewDestination, 
	use the PDViewDestResolve() method on the returned value to 
	obtain a PDViewDestination. 

	@since PI_PDMODEL_VERSION >= 0x00020000
*/
NPROC(PDViewDestination, PDActionGetDest, (PDAction action))

/**
	Gets the Cos object corresponding to an action. This method 
	does not copy the object, but is instead the logical equivalent 
	of a type cast. 
	@param action IN/OUT The action whose Cos object is obtained. 
	
	@return The dictionary Cos object for the action. The contents of the 
	dictionary can be enumerated using CosObjEnum(). 
	@see PDActionFromCosObj 
	@since PI_PDMODEL_VERSION >= 0x00020000
*/
NPROC(CosObj, PDActionGetCosObj, (PDAction action))

/**
	Converts a dictionary Cos object to an action and verifies 
	that the action is valid. This method does not copy the 
	object, but is instead the logical equivalent of a type 
	cast. 
	@param obj The dictionary Cos object whose action is obtained.
	@return The PDAction corresponding to <code>obj</code>. 
	@exception pdErrBadAction is raised if the action is invalid as determined 
	by PDActionIsValid(). 
	@see PDActionGetCosObj 
	@see PDActionNew 
	@see PDActionNewFromDest 
	@see PDActionNewFromFileSpec 
	@since PI_PDMODEL_VERSION >= 0x00020000
*/
NPROC(PDAction, PDActionFromCosObj, (CosObj obj))

/**
	Gets a file specification from an action. 

	<p>Not all types of actions have file specifications; this 
	method only works for actions that contain a file specification. 
	See Section 8.5 in the <i>PDF Reference</i> for more information 
	on the contents of various types of actions. </p>

	@param action The action whose file specification is obtained.
	@return The action's file specification. 
	@see PDActionGetDest 
	@since PI_PDMODEL_VERSION >= 0x00020000
*/
NPROC(PDFileSpec, PDActionGetFileSpec, (PDAction action))


/**
	Broadcasts a PDAnnotWillChange() notification. Clients must 
	call this method before making any change to a custom annotation. 
	
	@param annot The annotation that has changed. 
	@param key The ASAtom corresponding to the name of the 
	key in the annotation's Cos dictionary that is changing.
	@notify PDAnnotWillChange 
	@see PDAnnotNotifyDidChange 
	@see AVAppRegisterNotification 
	@since PI_PDMODEL_VERSION >= 0x00020000
*/
NPROC(void, PDAnnotNotifyWillChange, (PDAnnot annot, ASAtom key))

/**
	Broadcasts a PDAnnotDidChange() notification. Clients must 
	call this method after making any change to a custom annotation. 
	
	@param annot The annotation that has changed. 
	@param key The ASAtom corresponding to the name of the 
	key in the annotation's Cos dictionary that is changing. 
	
	@param err An error code to pass to any method registered 
	to receive the PDAnnotDidChange() notification. Pass zero 
	if the annotation was changed successfully. Pass a nonzero 
	value if an error occurred while changing the annotation.
	@notify PDAnnotDidChange 
	@see PDAnnotNotifyWillChange 
	@see AVAppRegisterNotification 
	@since PI_PDMODEL_VERSION >= 0x00020000
*/
NPROC(void, PDAnnotNotifyDidChange, (PDAnnot annot, ASAtom key, ASInt32 err))


/**
	Creates a new annotation, associated with the specified 
	page's CosDoc, but not added to the page. Use PDPageAddAnnot() 
	to add the annotation to the page. 

	<p>If you want to create an annotation that prints even if 
	the annotation handler is not present, you must provide 
	an appearance for it. To do this, add an appearance key 
	(AP) to the annotation dictionary, in which you place the 
	Forms XObject for the Normal (N), Rollover (R), and Down 
	(D) appearances; only the Normal appearance is required. 
	Also, add a flags field (F) to the annotation dictionary 
	and set the appropriate value for the bit field. A value 
	of <code>4</code>, which displays the annotation if the handler is not 
	present, shows the annotation, and allows printing it, is 
	recommended. </p>

	@param aPage The page to whose PDDoc the annotation is 
	added. 
	@param subType The subtype of annotation to create. 
	@param initialLocation A pointer to a rectangle specifying 
	the annotation's bounds, specified in user space coordinates.
	@return The newly created annotation. 
	@exception pdErrOpNotPermitted is raised if:
	<ul>
	<li>The annotation is of subtype Text and the document's permissions 
	do not include pdPermEditNotes (see PDPerms), or</li> 
	<li>The annotation is of any other subtype and the document's 
	permissions do not include pdPermEdit.</li>
	</ul>
	
	@notify PDAnnotWasCreated 
	@see PDPageAddAnnot 
	@see PDPageAddNewAnnot 
	@since PI_PDMODEL_VERSION >= 0x00020000
*/
UNPROC(PDAnnot, PDPageCreateAnnot, (PDPage aPage, ASAtom subType, const ASFixedRect *initialLocation))

/**
	Tests whether an annotation is valid. This is intended only 
	to ensure that the annotation has not been deleted, not 
	to ensure that all necessary information is present and 
	valid. 
	@param anAnnot The annotation whose validity is tested.
	@return <code>true</code> if <code>anAnnot</code> is a valid annotation object, <code>false</code> 
	otherwise. An annotation is valid if it is a Cos dictionary 
	object and has a Rect key. 
	@see PDAnnotEqual 
	@since PI_PDMODEL_VERSION >= 0x00020000
*/
NPROC(ASBool, PDAnnotIsValid, (PDAnnot anAnnot))

/**
	Gets an annotation's subtype. 
	@param anAnnot The annotation whose subtype is obtained.
	@return The ASAtom for the annotation's subtype. This can be converted 
	to a string using ASAtomGetString(). The storage pointed to 
	by the return value is owned by the Acrobat viewer and should 
	be assumed to be valid only until the next call into any 
	client API method; it should be immediately copied by the 
	client if the client wants to reference it later. 

	@return ASAtomNull if the annotation does not have a Subtype 
	key or its value is not a name object. 
	@exception pdErrBadAnnotation 
	@since PI_PDMODEL_VERSION >= 0x00020000
*/
NPROC(ASAtom, PDAnnotGetSubtype, (PDAnnot anAnnot))

/**
	Gets the size and location of an annotation on its page. 
	
	@param anAnnot IN/OUT The annotation whose location and size are 
	set. 
	@param boxP IN/OUT (Filled by the method) A pointer to a rectangle 
	that specifies the annotation's bounding rectangle, specified 
	in user space coordinates. 
	@exception pdErrBadAnnotation 
	@see PDAnnotSetRect 
	@see PDAnnotGetColor 
	@see PDAnnotGetDate 
	@see PDAnnotGetFlags 
	@see PDAnnotGetTitle 
	@since PI_PDMODEL_VERSION >= 0x00020000
*/
NPROC(void, PDAnnotGetRect, (PDAnnot anAnnot, ASFixedRect *boxP))

/**
	Sets the size and location of an annotation on its page. 
	
	@param anAnnot IN/OUT The annotation whose location and size are 
	set. 
	@param newBox IN/OUT A pointer to a rectangle that specifies the 
	annotation's bounding rectangle, specified in user space 
	coordinates. 
	@notify PDAnnotWillChange 
	@notify PDAnnotDidChange 
	@see PDAnnotGetRect 
	@see PDAnnotSetColor 
	@see PDAnnotSetDate 
	@see PDAnnotSetFlags 
	@see PDAnnotSetTitle 
	@since PI_PDMODEL_VERSION >= 0x00020000
*/
UNPROC(void, PDAnnotSetRect, (PDAnnot anAnnot, const ASFixedRect *newBox))

/**
	Tests whether two annotations are identical. 
	@param anAnnot The first annotation to compare. 
	@param annot2 The second annotation to compare.
	@return <code>true</code> if the annotations are equal, <code>false</code> otherwise. 
	Two annotations are equal only if their Cos objects 
	are equal (see CosObjEqual()). 
	@exception pdErrBadAnnotation 
	@see CosObjEqual 
	@since PI_PDMODEL_VERSION >= 0x00020000
*/
NPROC(ASBool, PDAnnotEqual, (PDAnnot anAnnot, PDAnnot annot2))

/**
	Gets a note or link annotation's color. If the annotation 
	does not specify an explicit color, a default color is returned. 
	Text annotations return <i>default yellow</i>; all others return 
	black. Only RGB color specifications are currently supported. 
	
	@param anAnnot The note or link annotation whose color 
	is obtained. 
	@param color (Filled by the method) The annotation's color, 
	which is used as follows:
		
	<TABLE rules="all" cellspacing="1">
	<TR><TH>Annotation</TH><TH>Use</TH></TR>
	<TR><TD>Closed text note</TD><TD>The icon background color.</TD></TR>
	<TR><TD>Open, un-selected text note</TD><TD>The bounding rectangle color.</TD></TR>
	<TR><TD>Open, selected text note</TD><TD>The color of the annotation's title bar.</TD></TR>
	<TR><TD>Link annotation</TD><TD>The link border color.</TD></TR>
	</TABLE>

	@return <code>true</code> if the annotation specifies an explicit color, 
	<code>false</code> if a default color was used. 
	@see PDAnnotSetColor 
	@see PDAnnotGetDate 
	@see PDAnnotGetFlags 
	@see PDAnnotGetRect 
	@see PDAnnotGetTitle 
	@since PI_PDMODEL_VERSION >= 0x00020000
*/
NPROC(ASBool, PDAnnotGetColor, (PDAnnot anAnnot, PDColorValue color))

/**
	Sets a note or link annotation's color. Only RGB color specifications 
	are currently supported. 
	@param anAnnot IN/OUT The note or link annotation whose color 
	is set. 
	@param color IN/OUT The annotation's color, which is used as follows:
		
	<TABLE rules="all" cellspacing="1">
	<TR><TH>Annotation</TH><TH>Use</TH></TR>
	<TR><TD>Closed text note</TD><TD>The icon background color.</TD></TR>
	<TR><TD>Open, un-selected text note</TD><TD>The bounding rectangle color.</TD></TR>
	<TR><TD>Open, selected text note</TD><TD>The color of the annotation's title bar.</TD></TR>
	<TR><TD>Link annotation</TD><TD>The link border color.</TD></TR>
	</TABLE>

	@notify PDAnnotWillChange 
	@notify PDAnnotDidChange 
	@see PDAnnotGetColor 
	@see PDAnnotSetDate 
	@see PDAnnotSetFlags 
	@see PDAnnotSetRect 
	@see PDAnnotSetTitle 
	@since PI_PDMODEL_VERSION >= 0x00020000
*/
UNPROC(void, PDAnnotSetColor, (PDAnnot anAnnot, const PDColorValue color))

/**
	Gets an annotation's label text. 

	@param anAnnot IN/OUT The annotation whose label is obtained. 
	
	@param buffer IN/OUT (Filled by the method) The string into which 
	the annotation's label string is copied. If the string is non-<code>NULL</code>, 
	up to <code>bufsSize</code> bytes are copied into the buffer. 
	@param bufSize IN/OUT The buffer size in bytes. Up to <code>bufSize</code> bytes 
	of the annotation label string are copied into the string 
	and an ASCII <code>NULL</code> character is appended. The caller is expected 
	to have allocated <code>bufSize + 1</code> bytes to allow for the <code>NULL</code>. 
	If <code>buffer</code> is <code>NULL</code>, it copies nothing. 
	@return If the string is non-<code>NULL</code>, it returns the number of bytes copied, not 
	counting the trailing <code>NULL</code>. If the string is <code>NULL</code>, it returns the number 
	of bytes that would be copied if the string were not <code>NULL</code>. 
	@exception pdErrBadAnnotation 
	@see PDAnnotSetTitle 
	@see PDAnnotGetColor 
	@see PDAnnotGetDate 
	@see PDAnnotGetRect 
	@see PDAnnotGetFlags 

	@note For Roman viewers, this text is always stored in the 
	<code>PDFDocEncoding</code>. For non-Roman character set viewers, this 
	text is stored as <code>PDFDocEncoding</code> or Unicode, depending on 
	the file's creator. Files created in a non-Roman environment 
	contain Unicode versions of these strings; in a Roman environment, 
	files contain <code>PDFDocEncoding</code> versions of these strings. 
	
	@since PI_PDMODEL_VERSION >= 0x00020000
*/
NPROC(ASInt32, PDAnnotGetTitle, (PDAnnot anAnnot, char *buffer, ASInt32 bufSize))

/**
	Sets an annotation's label text. 

	@param anAnnot IN/OUT The annotation whose label is set. 
	@param str IN/OUT The string containing the label to set. 
	@param nBytes IN/OUT The length of the label. The first <code>nBytes</code> bytes of <code>str</code> 
	are used as the label. 
	@notify PDAnnotWillChange 
	@notify PDAnnotDidChange 
	@see PDAnnotGetTitle 
	@see PDAnnotSetColor 
	@see PDAnnotSetDate 
	@see PDAnnotSetFlags 
	@see PDAnnotSetRect 

	@note For Roman viewers, this text is always stored in the 
	<code>PDFDocEncoding</code>. For non-Roman character set viewers, this 
	text is stored as <code>PDFDocEncoding</code> or Unicode, depending on 
	the file's creator. Files created in a non-Roman environment 
	contain Unicode versions of these strings; in a Roman environment, 
	files contain <code>PDFDocEncoding</code> versions of these strings. 
	
	@since PI_PDMODEL_VERSION >= 0x00020000
*/
UNPROC(void, PDAnnotSetTitle, (PDAnnot anAnnot, const char *str, ASInt32 nBytes))

/**
	Gets an annotation's date. 
	@param anAnnot The annotation whose date is obtained. 
	
	@param date (Filled by the method) The annotation's time 
	and date.
	@return <code>true</code> if the annotation contains a date key and the 
	value of that key can be successfully parsed as a date string, 
	<code>false</code> otherwise. 
	@exception pdErrBadAnnotation is raised if the annotation is not valid 
	or if the value of the annotation's M (ModDate) key is not a string. 
	@exception genErrBadParm is raised if <code>date</code> is <code>NULL</code>. 
	@see PDAnnotSetDate 
	@see PDAnnotGetColor 
	@see PDAnnotGetFlags 
	@see PDAnnotGetRect 
	@see PDAnnotGetTitle 
	@since PI_PDMODEL_VERSION >= 0x00020000
*/
NPROC(ASBool, PDAnnotGetDate, (PDAnnot anAnnot, ASTimeRecP date))

/**
	Sets an annotation's date. 
	@param anAnnot IN/OUT The annotation whose date is set. 
	@param date IN/OUT The annotation's time and date. 
	@notify PDAnnotWillChange 
	@notify PDAnnotDidChange 
	@see PDAnnotGetDate 
	@see PDAnnotSetColor 
	@see PDAnnotSetFlags 
	@see PDAnnotSetRect 
	@see PDAnnotSetTitle 
	@since PI_PDMODEL_VERSION >= 0x00020000
*/
UNPROC(void, PDAnnotSetDate, (PDAnnot anAnnot, const ASTimeRecP date))

/**
	Gets the Cos object corresponding to an annotation. This 
	method does not copy the object, but is instead the logical 
	equivalent of a type cast. 
	@param annot IN/OUT The annotation whose Cos object is obtained. 
	
	@return The dictionary Cos object for the annotation. The contents of 
	the dictionary can be enumerated using CosObjEnum(). It returns 
	a <code>NULL</code> Cos object if the annotation is not valid, as determined 
	by PDAnnotIsValid(). 
	@see PDAnnotFromCosObj 
	@since PI_PDMODEL_VERSION >= 0x00020000
*/
NPROC(CosObj, PDAnnotGetCosObj, (PDAnnot annot))

/**
	Converts a dictionary Cos object to an annotation. This 
	method does not copy the object, but is instead the logical 
	equivalent of a type cast. 
	@param obj The dictionary Cos object whose annotation 
	is obtained.
	@return The PDAnnot corresponding to the Cos object. 
	@exception pdErrBadAnnotation is raised if the annotation is invalid, 
	as determined by  PDAnnotIsValid(). 
	@see PDAnnotGetCosObj 
	@since PI_PDMODEL_VERSION >= 0x00020000
*/
NPROC(PDAnnot, PDAnnotFromCosObj, (CosObj obj))


/**
	Gets the text of a text annotation. 

	@param aTextAnnot The text annotation whose text is obtained. 
	
	@param buffer (Filled by the method) A buffer into which 
	the text is placed. If the text is encoded using <code>PDFDocEncoding</code>, 
	it can be converted to a platform's native encoding using 
	PDXlateToHost() or PDXlateToHostEx(). 
	@param bufSize The maximum number of characters that <code>buffer</code> 
	can hold.
	@return The number of characters written into <code>buffer</code>. 
	@see PDTextAnnotSetContents 
	@see PDXlateToPDFDocEnc 
	@see PDXlateToPDFDocEncEx 

	@note The contents of text annotations are encoded in <code>PDFDocEncoding</code> 
	if they are created in a Roman environment, Unicode if they are created in 
	a non-Roman environment. 
	@since PI_PDMODEL_VERSION >= 0x00020000
*/
NPROC(ASInt32, PDTextAnnotGetContents, (PDTextAnnot aTextAnnot, char *buffer,ASInt32 bufSize))

/**
	Sets the text of a text annotation. This method also sets the modification date of the annotation 
	to the current date and time. 

	@note The text must be encoded using <code>PDFDocEncoding</code> or Unicode. 
	
	@param aTextAnnot The text annotation whose text is set. 
	
	@param str A string containing the new text. The string 
	must be encoded using <code>PDFDocEncoding</code> or Unicode. The strings 
	in a platform's native encoding can be converted to <code>PDFDocEncoding</code> 
	using PDXlateToPDFDocEnc() or PDXlateToPDFDocEncEx(). 
	@param nBytes The number of characters in <code>str</code>.
	@notify PDAnnotWillChange 
	@notify PDAnnotDidChange 
	@see PDTextAnnotGetContents 
	@see PDXlateToPDFDocEnc 
	@see PDXlateToPDFDocEncEx 
	@since PI_PDMODEL_VERSION >= 0x00020000
*/
UNPROC(void, PDTextAnnotSetContents, (PDTextAnnot aTextAnnot, const char *str,ASInt32 nBytes))

/**
	Tests whether a text annotation is open. 

	@note In versions later than Acrobat 4.0, this method cannot 
	always correctly determine a text annotation's open state. 
	Beginning with Acrobat 4 (PDF 1.3), text annotations (and 
	other annotations) have an associated Popup annotation which 
	maintains the open state of the popup window; the method 
	works correctly when you pass it the popup annotation itself. 
	You can use Cos-level routines to find the Popup entry in 
	the annotation dictionary, which is itself a dictionary 
	containing an Open entry. 
	@param aTextAnnot The text annotation whose open/closed 
	state is obtained.
	@return <code>true</code> if the annotation is open, <code>false</code> otherwise. 
	
	@see PDTextAnnotSetOpen 
	@since PI_PDMODEL_VERSION >= 0x00020000
*/
NPROC(ASBool, PDTextAnnotIsOpen, (PDTextAnnot aTextAnnot))

/**
	Opens or closes a text annotation. 

	@note In versions later than Acrobat 4.0, this method cannot 
	always correctly set a text annotation's open state. Beginning 
	with Acrobat 4 (PDF 1.3), text annotations (and other annotations) 
	have an associated Popup annotation which maintains the 
	open state of the popup window; the method works correctly 
	when you pass it the popup annotation itself. You can use 
	Cos-level routines to find the Popup entry in the annotation 
	dictionary, which is itself a dictionary containing an Open 
	entry. 
	@param aTextAnnot The annotation to open or close. 
	@param isOpen <code>true</code> if the annotation is opened, <code>false</code> 
	if the annotation is closed.
	@notify PDAnnotWillChange 
	@notify PDAnnotDidChange 
	@see PDTextAnnotIsOpen 
	@since PI_PDMODEL_VERSION >= 0x00020000
*/
UNPROC(void, PDTextAnnotSetOpen, (PDTextAnnot aTextAnnot, ASBool isOpen))

/**
	Gets the border of a link annotation. 
	@param aLinkAnnot IN/OUT The link annotation whose border is obtained. 
	
	@param border IN/OUT (Filled by the method) A pointer to a structure 
	containing the link border. Link corner radii are ignored 
	by the Acrobat viewers. 
	@see PDLinkAnnotSetBorder 
	@since PI_PDMODEL_VERSION >= 0x00020000
*/
NPROC(void, PDLinkAnnotGetBorder, (PDLinkAnnot aLinkAnnot, PDLinkAnnotBorder *border))

/**
	Sets a link annotation's border. 
	@param aLinkAnnot IN/OUT The link annotation whose border is set. 
	
	@param border IN/OUT A pointer to a structure containing the link 
	border. Link corner radii are ignored by the Acrobat viewers. 
	
	@exception PDAnnotDidChange 
	@exception PDAnnotWillChange 
	@notify PDAnnotWillChange 
	@notify PDAnnotDidChange 
	@see PDLinkAnnotGetBorder 
	@since PI_PDMODEL_VERSION >= 0x00020000
*/
UNPROC(void, PDLinkAnnotSetBorder, (PDLinkAnnot aLinkAnnot, const PDLinkAnnotBorder *border))

/**
	Sets a link annotation's action. 
	@param aLinkAnnot IN/OUT The link annotation whose action is set. 
	
	@param action IN/OUT The new action for the link annotation. 
	@notify PDAnnotWillChange 
	@notify PDAnnotDidChange 
	@see PDLinkAnnotGetAction 
	@since PI_PDMODEL_VERSION >= 0x00020000
*/
UNPROC(void, PDLinkAnnotSetAction, (PDLinkAnnot aLinkAnnot, PDAction action))

/**
	Gets a link annotation's action. After you obtain the action, 
	you can execute it with AVDocPerformAction(). 
	@param aLinkAnnot IN/OUT The link annotation whose action is obtained. 
	
	@return The link annotation's action. 
	@see AVDocPerformAction 
	@see PDLinkAnnotSetAction 
	@since PI_PDMODEL_VERSION >= 0x00020000
*/
NPROC(PDAction, PDLinkAnnotGetAction, (PDLinkAnnot aLinkAnnot))

/**
	Gets an annotation's flags. 
	@param anAnnot IN/OUT The annotation whose flags are obtained. 
	
	@return The flags, or <code>0</code> if the annotation does not have a flags 
	key. 
	@see PDAnnotSetFlags 
	@see PDAnnotGetColor 
	@see PDAnnotGetDate 
	@see PDAnnotGetRect 
	@see PDAnnotGetTitle 
	@since PI_PDMODEL_VERSION >= 0x00020000
*/
NPROC(ASUns32, PDAnnotGetFlags, (PDAnnot anAnnot))

/**
	Sets an annotation's flags. 
	@param anAnnot IN/OUT The annotation whose flags are set. 
	@param flags IN/OUT An <code>OR</code> of the PDAnnot Flags values. 
	@notify PDAnnotWillChange 
	@notify PDAnnotDidChange 
	@see PDAnnotGetFlags 
	@see PDAnnotSetColor 
	@see PDAnnotSetDate 
	@see PDAnnotSetRect 
	@see PDAnnotSetTitle 
	@since PI_PDMODEL_VERSION >= 0x00020000
*/
UNPROC(void, PDAnnotSetFlags, (PDAnnot anAnnot, ASUns32 flags))


/**
	Adds a new bookmark to the tree containing <code>aBookmark</code>, 
	as the new right sibling. 
	@param aBookmark The bookmark that will be the left sibling 
	of the new bookmark. 
	@param initialText The new bookmark's title.
	@return The newly created bookmark. 
	@notify PDBookmarkDidChangePosition 
	@notify PDBookmarkWasCreated 
	@see PDBookmarkAddChild 
	@see PDBookmarkAddNewChild 
	@see PDBookmarkAddNext 
	@see PDBookmarkAddPrev 
	@see PDBookmarkAddSubtree 
	@see PDBookmarkUnlink 
	@since PI_PDMODEL_VERSION >= 0x00020000
*/
UNPROC(PDBookmark, PDBookmarkAddNewSibling, (PDBookmark aBookmark, char *initialText))

/**
	Adds a new bookmark to the tree containing <code>aBookmark</code>, as 
	the new last child of <code>aBookmark</code>. If <code>aBookmark</code> previously 
	had no children, it will be open after the child is added. 
	
	@param aBookmark The bookmark to which a new last child 
	is added. 
	@param initialText The new bookmark's title.
	@return The newly created bookmark. 
	@notify PDBookmarkDidChangePosition 
	@notify PDBookmarkWasCreated 
	@see PDBookmarkAddNewSibling 
	@see PDBookmarkAddSubtree 
	@see PDBookmarkAddPrev 
	@see PDBookmarkAddNext 
	@see PDBookmarkAddChild 
	@see PDBookmarkUnlink 
	@since PI_PDMODEL_VERSION >= 0x00020000
*/
UNPROC(PDBookmark, PDBookmarkAddNewChild, (PDBookmark aBookmark, const char *initialText))

/**
	Adds a copy of the bookmark subtree source to <code>aBookmark</code>, 
	as a new last child of <code>aBookmark</code>. This new item will have 
	the text value <code>sourceTitle</code>, will be open, and will have 
	no destination attribute. <code>source</code> must have been previously 
	unlinked. If <code>aBookmark</code> previously had no children, it will 
	be open after the subtree is added. 
	@param aBookmark IN/OUT The bookmark to which the subtree source 
	will be added as a new last child. 
	@param source IN/OUT The bookmark subtree to add. 
	@param sourceTitle IN/OUT The new bookmark's title. 
	@notify PDBookmarkWillChange 
	@notify PDBookmarkDidChange 
	@notify PDBookmarkDidChangePosition 
	@see PDBookmarkAddNewSibling 
	@see PDBookmarkAddNewChild 
	@see PDBookmarkAddPrev 
	@see PDBookmarkAddNext 
	@see PDBookmarkAddChild 
	@see PDBookmarkUnlink 
	@since PI_PDMODEL_VERSION >= 0x00020000
*/
UNPROC(void, PDBookmarkAddSubtree, (PDBookmark aBookmark, PDBookmark source,char *sourceTitle))

/**
	Removes a bookmark subtree from the bookmark tree containing 
	it. 
	@param aBookmark IN/OUT The root bookmark of the subtree to remove. 
	
	@notify PDBookmarkWillDestroy 
	@notify PDBookmarkDidDestroy 
	@see PDBookmarkAddNewChild 
	@see PDBookmarkAddNewSibling 
	@see PDBookmarkFromCosObj 
	@see PDBookmarkUnlink 
	@since PI_PDMODEL_VERSION >= 0x00020000
*/
UNPROC(void, PDBookmarkDestroy, (PDBookmark aBookmark))

/**
	Gets the first bookmark whose title is <code>aName</code>.

	@param aBookmark IN/OUT The root of the bookmark subtree to search. 	
	@param aname IN/OUT The text value to search for. Character codes 
	in <code>aName</code> are interpreted using the <code>PDFDocEncoding</code>. 
	@param nameLen IN/OUT The length of <code>aName</code>. 
	@param maxdepth IN/OUT The number of subtree levels to search, 
	not counting the root level:
		
	<TABLE rules="all" cellspacing="1">
	<TR><TH>Value</TH><TH>Description</TH></TR>
	<TR><TD><code>0</code></TD><TD>Only look at <code>aBookmark</code>, not at any of its children.</TD></TR>
	<TR><TD><code>1</code></TD><TD>Check <code>aBookmark</code> and its children, but not any grandchildren or great grandchildren, and so on.</TD></TR>
	<TR><TD><code>-1</code></TD><TD>Check the entire subtree.</TD></TR>
	</TABLE>

	@return The bookmark with the specified title, or a <code>NULL</code> Cos object 
	if there is no such bookmark. 

	@note For Roman viewers, this text is always stored in the 
	<code>PDFDocEncoding</code>. For non-Roman character set viewers, this 
	text is stored as <code>PDFDocEncoding</code> or Unicode, depending on 
	the file's creator. Files created in a non-Roman environment 
	contain Unicode versions of these strings; in a Roman environment, 
	files contain <code>PDFDocEncoding</code> versions of these strings. 	
	@since PI_PDMODEL_VERSION >= 0x00020000
*/
NPROC(PDBookmark, PDBookmarkGetByTitle, (PDBookmark aBookmark, const char *aname,ASInt32 nameLen,ASInt32	maxdepth))

/**
	Gets the number of open bookmarks in a subtree. 
	@param aBookmark The root bookmark of a subtree to count.
	@return The number of open bookmarks in the subtree (not including <code>aBookmark</code>). 
	
	@see PDBookmarkGetIndent 
	@see PDBookmarkHasChildren 
	@since PI_PDMODEL_VERSION >= 0x00020000
*/
NPROC(ASInt32, PDBookmarkGetCount, (PDBookmark aBookmark))

/**
	Adds <code>newPrev</code> as the new left sibling to <code>aBookmark</code>, adjusting 
	the tree containing <code>aBookmark</code> appropriately. 
	@param aBookmark The bookmark that will receive a new 
	left sibling <code>newPrev</code>. 
	@param newPrev The bookmark to become the new left sibling 
	of <code>aBookmark</code>. <code>newPrev</code> must have been previously unlinked.
	@notify PDBookmarkDidChangePosition 
	@see PDBookmarkAddNewSibling 
	@see PDBookmarkAddNewChild 
	@see PDBookmarkAddSubtree 
	@see PDBookmarkAddNext 
	@see PDBookmarkAddChild 
	@see PDBookmarkUnlink 
	@since PI_PDMODEL_VERSION >= 0x00020000
*/
UNPROC(void, PDBookmarkAddPrev, (PDBookmark aBookmark, PDBookmark newPrev))

/**
	Adds <code>newNext</code> as the new right sibling to <code>aBookmark</code>. 
	@param aBookmark IN/OUT The bookmark that will receive a new right 
	sibling. 
	@param newNext IN/OUT The bookmark to become the new right sibling 
	of <code>aBookmark</code>. <code>newNext</code> must have been previously unlinked. 
	
	@notify PDBookmarkDidChangePosition 
	@see PDBookmarkAddNewSibling 
	@see PDBookmarkAddChild 
	@see PDBookmarkAddSubtree 
	@see PDBookmarkAddPrev 
	@see PDBookmarkAddNewChild 
	@see PDBookmarkUnlink 
	@since PI_PDMODEL_VERSION >= 0x00020000
*/
UNPROC(void, PDBookmarkAddNext, (PDBookmark aBookmark, PDBookmark newNext))

/**
	Adds <code>aBookmark</code> as the last child of <code>parent</code>, adjusting the 
	tree containing <code>parent</code> appropriately. If <code>parent</code> previously 
	had no children, it is open after the child is added. 
	@param parent The parent of the bookmark being added. 
	
	@param aBookmark The bookmark that will become the last 
	child of <code>aBookmark</code>. <code>aBookmark</code> must have been previously 
	unlinked.
	@notify PDBookmarkDidChangePosition 
	@see PDBookmarkAddNewSibling 
	@see PDBookmarkAddNewChild 
	@see PDBookmarkAddSubtree 
	@see PDBookmarkAddPrev 
	@see PDBookmarkAddNext 
	@see PDBookmarkUnlink 
	@since PI_PDMODEL_VERSION >= 0x00020000
*/
UNPROC(void, PDBookmarkAddChild, (PDBookmark parent, PDBookmark aBookmark))

/**
	Unlinks a bookmark from the bookmark tree that contains 
	it, and adjusts the tree appropriately. 
	@param aBookmark IN/OUT The bookmark to unlink. 
	@see PDBookmarkDestroy 
	@since PI_PDMODEL_VERSION >= 0x00020000
*/
UNPROC(void, PDBookmarkUnlink, (PDBookmark aBookmark))

/**
	Tests whether a bookmark is valid. This is intended only 
	to ensure that the bookmark has not been deleted, not to 
	ensure that all necessary information is present and valid. 
	
	@param aBookmark The bookmark whose validity is tested.
	@return <code>true</code> if the bookmark is valid, <code>false</code> otherwise. 
	
	@see PDBookmarkEqual 
	@since PI_PDMODEL_VERSION >= 0x00020000
*/
NPROC(ASBool, PDBookmarkIsValid, (PDBookmark aBookmark))

/**
	Gets a bookmark's parent bookmark. 
	@param aBookmark The bookmark whose parent is obtained.
	@return The parent bookmark of <code>aBookmark</code>, or a <code>NULL</code> Cos object if <code>aBookmark</code> 
	is the root of its tree. 
	@see PDBookmarkGetFirstChild 
	@see PDBookmarkGetLastChild 
	@see PDBookmarkGetNext 
	@see PDBookmarkGetPrev 
	@since PI_PDMODEL_VERSION >= 0x00020000
*/
NPROC(PDBookmark, PDBookmarkGetParent, (PDBookmark aBookmark))

/**
	Gets a bookmark's first child. 
	@param aBookmark IN/OUT The bookmark whose first child is obtained. 
	
	@return The first child of <code>aBookmark</code>, or a <code>NULL</code> Cos object if <code>aBookmark</code> 
	has no children. 
	@see PDBookmarkGetParent 
	@see PDBookmarkGetLastChild 
	@see PDBookmarkHasChildren 
	@see PDBookmarkGetNext 
	@see PDBookmarkGetPrev 
	@since PI_PDMODEL_VERSION >= 0x00020000
*/
NPROC(PDBookmark, PDBookmarkGetFirstChild, (PDBookmark aBookmark))

/**
	Gets a bookmark's last child. 
	@param aBookmark IN/OUT The bookmark whose last child is obtained. 
	
	@return The last child of <code>aBookmark</code>, or a <code>NULL</code> Cos object if <code>aBookmark</code> 
	has no children. 
	@see PDBookmarkGetParent 
	@see PDBookmarkGetFirstChild 
	@see PDBookmarkGetNext 
	@see PDBookmarkGetPrev 
	@since PI_PDMODEL_VERSION >= 0x00020000
*/
NPROC(PDBookmark, PDBookmarkGetLastChild, (PDBookmark aBookmark))

/**
	Gets a bookmark's next (right) sibling. 
	@param aBookmark The bookmark whose right sibling is obtained.
	@return The <code>aBookmark</code> object's next (right) sibling. It returns a <code>NULL</code> Cos object 
	if <code>aBookmark</code> has no next sibling (that is, it is its parent's 
	last child). 
	@see PDBookmarkGetParent 
	@see PDBookmarkGetFirstChild 
	@see PDBookmarkGetLastChild 
	@see PDBookmarkGetPrev 
	@since PI_PDMODEL_VERSION >= 0x00020000
*/
NPROC(PDBookmark, PDBookmarkGetNext, (PDBookmark aBookmark))

/**
	Returns a bookmark's previous (left) sibling. 
	@param aBookmark IN/OUT The bookmark whose left sibling is obtained. 
	
	@return Previous (left) sibling of <code>aBookmark</code>, or a <code>NULL</code> Cos object 
	if <code>aBookmark</code> has no previous sibling (it is its parent's 
	first child). 
	@see PDBookmarkGetParent 
	@see PDBookmarkGetFirstChild 
	@see PDBookmarkGetLastChild 
	@see PDBookmarkGetNext 
	@since PI_PDMODEL_VERSION >= 0x00020000
*/
NPROC(PDBookmark, PDBookmarkGetPrev, (PDBookmark aBookmark))

/**
	Returns the indentation level of a bookmark in its containing 
	tree. 
	@param aBookmark IN/OUT The bookmark whose indentation level is 
	obtained. 
	@return The indentation level of <code>aBookmark</code> in its containing tree. 
			The root and its direct children have an indentation level of zero.	
	@since PI_PDMODEL_VERSION >= 0x00020000
*/
NPROC(ASInt32, PDBookmarkGetIndent, (PDBookmark aBookmark))

/**
	Gets a bookmark's title. 
	
	@param aBookmark The bookmark whose title is obtained. 
	
	@param buffer (Filled by the method) The buffer into which 
	the title will be written. If <code>buffer</code> is non-<code>NULL</code>, its length 
	is assumed to be <code>bufSize + 1</code>, because a <code>NULL</code> byte is appended 
	to the title. The returned text remains valid only until the
	next PDModel method call. The text may be converted to a 
	platform's native encoding using PDXlateToHost() or PDXlateToHostEx(). 
	
	@param bufSize The size of the buffer.
	@return The number of bytes copied into <code>buffer</code>, not counting the 
	trailing <code>NULL</code> byte. If <code>buffer</code> is <code>NULL</code>, the number of bytes 
	in the bookmark is returned. 
	@see PDBookmarkSetTitle 
	@see PDXlateToHost 
	@see PDXlateToHostEx 

	@note For Roman viewers, this title text is always stored 
	in <code>PDFDocEncoding</code>. For non-Roman character set viewers, 
	this text is stored as <code>PDFDocEncoding</code> or Unicode, depending 
	on the file's creator. Files created in a non-Roman environment 
	contain Unicode versions of these strings; in a Roman environment, 
	files contain <code>PDFDocEncoding</code> versions of these strings. 

	@since PI_PDMODEL_VERSION >= 0x00020000
*/
NPROC(ASInt32, PDBookmarkGetTitle, (PDBookmark aBookmark, char *buffer,ASInt32 bufSize))

/**
	Sets a bookmark's title. 

	@param aBookmark The bookmark whose title is set. 
	@param str A read-only string containing the bookmark's 
	new title. The text must be encoded using <code>PDFDocEncoding</code>. 
	Strings encoded using a platform's native encoding can be 
	converted to <code>PDFDocEncoding</code> using PDXlateToPDFDocEnc() or 
	PDXlateToPDFDocEncEx(). 
	@param nBytes The number of bytes of <code>str</code> to copy.
	@exception pdErrBookmarksError is raised if there is an error setting 
	the title. 
	@notify PDBookmarkWillChange 
	@notify PDBookmarkDidChange 
	@see PDBookmarkGetTitle 

	@note For Roman viewers, this title text is always stored 
	in the <code>PDFDocEncoding</code>. For non-Roman character set viewers, 
	this text is stored as <code>PDFDocEncoding</code> or Unicode, depending 
	on the file's creator. Files created in a non-Roman environment 
	contain Unicode versions of these strings; in a Roman environment, 
	files contain <code>PDFDocEncoding</code> versions of these strings. 
	
	@since PI_PDMODEL_VERSION >= 0x00020000
*/
UNPROC(void, PDBookmarkSetTitle, (PDBookmark aBookmark, const char *str, ASInt32 nBytes))

/**
	Tests whether a bookmark has children. 
	@param aBookmark The bookmark to test.
	@return <code>true</code> if <code>aBookmark</code> has any children, <code>false</code> otherwise. 
	
	@since PI_PDMODEL_VERSION >= 0x00020000
*/
NPROC(ASBool, PDBookmarkHasChildren, (PDBookmark aBookmark))

/**
	Tests whether a bookmark is open. An open bookmark shows 
	all its children. 
	@param aBookmark The bookmark to test.
	@return <code>true</code> if the bookmark is open, <code>false</code> otherwise. 
	@see PDBookmarkSetOpen 
	@since PI_PDMODEL_VERSION >= 0x00020000
*/
NPROC(ASBool, PDBookmarkIsOpen, (PDBookmark aBookmark))

/**
	Opens or closes a bookmark. An open bookmark shows its children, 
	while a closed bookmark does not. 
	@param aBookmark IN/OUT The bookmark to open or close. 
	@param isOpen IN/OUT <code>true</code> if the bookmark is opened, <code>false</code> if 
	the bookmark is closed. 
	@notify PDBookmarkWillChange 
	@notify PDBookmarkDidChange 
	@see PDBookmarkIsOpen 
	@since PI_PDMODEL_VERSION >= 0x00020000
*/
NPROC(void, PDBookmarkSetOpen, (PDBookmark aBookmark, ASBool isOpen))

/**
	Gets a bookmark's action. After you obtain the action, you 
	can execute it with AVDocPerformAction(). 
	@param aBookmark IN/OUT The bookmark whose action is obtained. 
		
	@return The bookmark's action. 
	@see AVDocPerformAction 
	@see PDBookmarkSetAction 
	@see PDLinkAnnotGetAction 
	@since PI_PDMODEL_VERSION >= 0x00020000
*/
NPROC(PDAction, PDBookmarkGetAction, (PDBookmark aBookmark))

/**
	Sets a bookmark's action. 
	@param aBookmark IN/OUT The bookmark whose action is set. 
	@param action IN/OUT The bookmark's action. 
	@notify PDBookmarkWillChange 
	@notify PDBookmarkDidChange 
	@see PDBookmarkGetAction 
	@since PI_PDMODEL_VERSION >= 0x00020000
*/
UNPROC(void, PDBookmarkSetAction, (PDBookmark aBookmark, PDAction action))

/**
	Tests whether two bookmarks are equal. Two bookmarks are 
	equal only if their Cos objects are equal (see CosObjEqual()). 
	
	@param aBookmark The first bookmark to compare. 
	@param bookmark2 The second bookmark to compare.
	@return <code>true</code> if the bookmarks are equal, <code>false</code> otherwise. 
	
	@see CosObjEqual 
	@since PI_PDMODEL_VERSION >= 0x00020000
*/
NPROC(ASBool, PDBookmarkEqual, (PDBookmark aBookmark, PDBookmark bookmark2))

/**
	Gets the Cos object for a bookmark. This method does not 
	copy the object, but is instead the logical equivalent of 
	a type cast. 
	@param aBookmark IN/OUT The bookmark whose Cos object is obtained. 
	
	@return The dictionary Cos object for the bookmark. The contents of 
	the dictionary can be enumerated using CosObjEnum(). 
	@see PDBookmarkFromCosObj 
	@see CosObjEnum 
	@since PI_PDMODEL_VERSION >= 0x00020000
*/
NPROC(CosObj, PDBookmarkGetCosObj, (PDBookmark aBookmark))

/**
	Converts a Cos dictionary object to a bookmark and checks 
	the validity of the bookmark. This method does not copy 
	the object, but is instead the logical equivalent of a type 
	cast. 
	@param obj The dictionary Cos object whose bookmark is 
	obtained.
	@return The bookmark corresponding to the Cos object. 
	@exception pdErrBadBookmark is raised if the bookmark is not valid as 
	determined by PDBookmarkIsValid(). 
	@see PDBookmarkGetCosObj 
	@since PI_PDMODEL_VERSION >= 0x00020000
*/
NPROC(PDBookmark, PDBookmarkFromCosObj, (CosObj obj))


/**
	Enumerates the PDDoc objects that are currently open, calling a 
	user-supplied procedure for each open document. 
	@param proc IN/OUT A user-supplied callback to call for each open 
	PDDoc. Enumeration halts if <code>proc</code> returns <code>false</code>. 
	@param clientData IN/OUT A pointer to user-supplied data to pass 
	to <code>proc</code> each time it is called. 
	@see AVAppEnumDocs 
	@see PDDocOpen 
  	@ingroup Enumerators
	@since PI_PDMODEL_VERSION >= 0x00020000
*/
NPROC(void, PDEnumDocs, (PDDocEnumProc proc, void *clientData))

/**
	Opens the specified document. If the document is already 
	open, it returns a reference to the already opened PDDoc. You 
	must call PDDocClose() once for every successful open. If 
	the call fails and the exception is pdErrNeedRebuild, then 
	call again with <code>doRepair</code> set to <code>true</code>. This allows the application 
	to decide whether to perform the time-consuming repair operation. 
	
	@param fileName A path name to the file, specified in whatever 
	format is correct for <code>fileSys</code>. 
	@param fileSys A pointer to an ASFileSysRec containing the 
	file system in which the file resides. If it is <code>NULL</code>, it uses the 
	default file system. 
	@param authProc An authorization callback, called only if 
	the file has been secured (that is, if the file has either 
	the user or the master password set). This callback should 
	obtain whatever information is needed to determine whether 
	the user is authorized to open the file, then call PDDocPermRequest(). 
	The Acrobat viewer's built-in authorization procedure 
	requires the user to enter a password, and allows the user 
	to try three times before giving up. If the <code>authProc</code> requires 
	data, use PDDocOpenEx() instead of PDDocOpen(). 
	@param doRepair If <code>true</code>, attempt to repair the file if 
	it is damaged. If <code>false</code>, do not attempt to repair the file 
	if it is damaged.
	@return The newly-opened document. 
	@exception pdErrNeedPassword or other errors are raised if the file is encrypted and <code>authProc</code> 
	is <code>NULL</code> or returns <code>false</code>. 
	@exception pdErrNotEnoughMemoryToOpenDoc or genErrNoMemory is raised if 
	there is insufficient memory to open the document. 
	@exception pdErrNeedRebuild is raised if the document needs to be rebuilt 
	and <code>doRepair</code> is <code>false</code>. 
	@exception pdErrBadOutlineObj is raised if the Outlines object appears 
	to be invalid (if the value of the Outlines key in the Catalog is not a <code>NULL</code> or dictionary 
	object). 
	@exception pdErrBadRootObj is raised if the Catalog object (as returned 
	by CosDocGetRoot()) is not a dictionary. 
	@exception pdErrBadBaseObj is raised if the Pages tree appears to be invalid 
	(if the value of the  Pages key in the Catalog is not a <code>NULL</code> or dictionary object). 
	@exception pdErrTooManyPagesForOpen is raised if the document contains 
	too many pages. 
	@exception cosSynErrNoHeader is raised if the document's header appears 
	to be bad. 
	@exception cosSynErrNoStartXRef is raised if no end-of-file line can be 
	located. 
	@exception cosErrRebuildFailed is raised if <code>doRepair</code> is <code>true</code> and rebuild 
	failed. 
	@see PDDocClose 
	@see PDDocCreate 
	@see PDDocPermRequest 
	@see PDDocOpenEx 
	@see PDDocOpenFromASFile 
	@see PDDocOpenFromASFileEx 
	@since PI_PDMODEL_VERSION >= 0x00020000
*/
NPROC(PDDoc, PDDocOpen, (ASPathName fileName, ASFileSys fileSys,PDAuthProc authProc, ASBool doRepair))

/**
	Gets the value of the OpenAction key in the Catalog dictionary, 
	which is the action performed when the document is opened. 
	After you obtain the action, you can execute it with AVDocPerformAction(). 
	
	@param doc IN/OUT The document whose open action is obtained. 
	
	@return The document's open action. It is invalid if there is no OpenAction 
	key in the Catalog dictionary (this can be tested with PDActionIsValid()). 
	
	@see AVDocPerformAction 
	@see PDDocSetOpenAction 
	@since PI_PDMODEL_VERSION >= 0x00020000
*/
NPROC(PDAction, PDDocGetOpenAction, (PDDoc doc))

/**
	Sets the value of the OpenAction key in the Catalog dictionary, 
	which is the action performed when the document is opened. 
	
	@param doc The document whose open action is set.
	@param action The open action you want to set.
	@see PDDocGetOpenAction 
	@since PI_PDMODEL_VERSION >= 0x00020000
*/
UNPROC(void, PDDocSetOpenAction, (PDDoc doc, PDAction action))

/**
	Creates a new document. The only Cos object in the document 
	will be a Catalog. See Section 3.6 in the <i>PDF Reference</i>. 
	After the document is created, at least one page must be 
	added using PDDocCreatePage() or PDDocInsertPages() before the 
	Acrobat viewer can display or save the document. 

	<p>When you are done with the document, you must call PDDocClose() 
	to release the resources used by the PDDoc; do not call 
	PDDocRelease(). </p>

	@return The newly created document. 
	@see PDDocClose 
	@see PDDocOpen 
	@see PDDocSave 
	@see PDDocCreatePage 
	@see PDDocInsertPages 
	@since PI_PDMODEL_VERSION >= 0x00020000
*/
UNPROC(PDDoc, PDDocCreate, (void))

/**
	Saves a document to disk. If a full save is requested to 
	the original path, the file is saved to a file system-determined 
	temporary file, the old file is deleted, and the temporary 
	file is renamed to <code>newPath</code>. You must call PDDocClose() to 
	release resources; do not call PDDocRelease(). 

	<p>If the document was created with PDDocCreate(), at least one 
	page must be added using PDDocCreatePage() or PDDocInsertPages() 
	before Acrobat can save the document. </p>

	<p>You can replace this method with your own version, using 
	HFTReplaceEntry(). </p>

	<p>A full save with linearization optimizes the PDF file. During 
	optimization, all objects in a PDF file are rearranged, 
	many of them acquiring not only a new file position, but 
	also a new Cos object number. At the end of the save operation, 
	Acrobat flushes its information of the PD layer and below 
	to synchronize its in-memory state with the new disk file 
	just written. </p>

	<p>It is crucial that all objects that have been acquired from 
	a PDDoc be released before Acrobat attempts to flush its 
	in-memory state. This includes any object that was acquired 
	with a <code>PD*Acquire</code> method, such as PDDocAcquirePage() or PDBeadAcquirePage(). 
	Failing to release these objects before the full save results 
	in a save error, and the resulting PDF file is not valid. 
	In addition, all PD level objects and Cos objects derived 
	from the PDDoc are no longer valid after the full save. 
	Attempting to use these objects after a full save produces 
	undefined results. </p>

	<p>Clients and applications should register for the PDDocWillSaveEx() 
	and PDDocDidSave() notifications so that they can clean up 
	appropriately. See these notifications for more information 
	on releasing and reacquiring objects from the PDDoc. </p>
	@ingroup ReplaceableMethods
	@param doc The document to save. 
	@param saveFlags A bit field composed of an <code>OR</code> of the 
	PDSaveFlags values. 
	@param newPath The path to which the file is saved. A 
	path must be specified when either PDSaveFull or PDSaveCopy 
	are used for saveFlags. If PDSaveIncremental is specified 
	in saveFlags, then <code>newPath</code> should be <code>NULL</code>. If PDSaveFull 
	is specified and <code>newPath</code> is the same as the file's original 
	path, the new file is saved to a file system-determined 
	temporary path, then the old file is deleted and the new 
	file is renamed to <code>newPath</code>.
	@param fileSys The file system. If it is <code>NULL</code>, uses the <code>fileSys</code> 
	of the document's current backing file. Files can only be 
	saved to the same file system. <code>fileSys</code> must be either <code>NULL</code> 
	or the default file system obtained with ASGetDefaultFileSys(), 
	otherwise an error is raised. 
	@param progMon A progress monitor. Use AVAppGetDocProgressMonitor() 
	to obtain the default. <code>NULL</code> may be passed, in which case 
	no progress monitor is used. 
	@param progMonClientData A pointer to user-supplied data 
	to pass to <code>progMon</code> each time it is called. It should be <code>NULL</code> 
	if <code>progMon</code> is <code>NULL</code>.
	@exception pdErrAfterSave is raised if the save was completed successfully, 
	but there were problems cleaning up afterwards. The document is no longer 
	consistent and cannot be changed. It must be closed and reopened. 
	@exception pdErrOpNotPermitted is raised if saving is not permitted. Saving 
	is permitted if either <code>edit</code> or <code>editNotes</code> (see PDPerms) is allowed, or you are doing 
	a full save and saveAs is allowed. 
	@exception pdErrAlreadyOpen is raised if PDSaveFull is used, and the file 
	specified by <code>newPath</code> is already open. 
	@notify PDDocWillSave 
	@notify PDDocDidSave 
	@see PDDocClose 
	@see PDDocSaveWithParams 
	@note Not replaceable in Adobe Reader.
	@since PI_PDMODEL_VERSION >= 0x00020000
*/
UPROC(void, PDDocSave, (PDDoc doc, PDSaveFlags saveFlags, ASPathName newPath, ASFileSys fileSys, ProgressMonitor progMon, void *progMonClientData))

/**
	Closes a document and releases its resources. If <code>doc</code> is 
	<code>NULL</code>, it does nothing. Changes are not saved. You must use 
	PDDocSave() to save any modifications before calling PDDocClose(). 
	
	<p>If the document has been modified but you wish to mark it 
	as clean, use PDDocClearFlags(). </p>
	@param doc The document to close.
	@exception pdErrUnableToCloseDueToRefs is raised if there are any outstanding 
	references to objects in the document, and the document will still be 
	valid (its resources will not be released). 
	@exception genErrBadUnlock is raised if the document's open count is less 
	than one. 
	@see PDDocSave 
	@see PDDocOpen 
	@see PDDocCreate 
	@see PDDocClearFlags 
	@see PDDocSetFlags 
	@since PI_PDMODEL_VERSION >= 0x00020000
*/
NPROC(void, PDDocClose, (PDDoc doc))

/**
	Increments a document's reference count. The document will 
	not be closed until the reference count is zero, or the 
	application terminates. 
	@param doc IN/OUT The document whose reference count is incremented. 
	
	@see PDDocRelease 
	@since PI_PDMODEL_VERSION >= 0x00020000
*/
NPROC(void, PDDocAcquire, (PDDoc doc))

/**
	Decrements a document's reference count. The document will 
	not be closed until the reference count is zero, or the 
	application terminates. 
	@param doc IN/OUT The document whose reference count is decremented. 
	
	@exception genErrBadParm 
	@see PDDocAcquire 
	@since PI_PDMODEL_VERSION >= 0x00020000
*/
NPROC(void, PDDocRelease, (PDDoc doc))


/**
	Gets information about the document's file and its state. 
	
	@param doc IN/OUT The document whose flags are obtained. 
	@return Flags field, containing an <code>OR</code> of the PDDocFlags values. 
	
	@see PDDocSetFlags 
	@see PDDocClearFlags 
	@since PI_PDMODEL_VERSION >= 0x00020000
*/
NPROC(ASInt32, PDDocGetFlags, (PDDoc doc))

/**
	Sets information about the document's file and its state. 
	This method can only be used to set, not clear, flags. As 
	a result, it is not possible, for example, to use this method 
	to clear the flag that indicates that a document has been 
	modified and needs to be saved. Instead, use PDDocClearFlags() 
	to clear flags. 
	@param doc IN/OUT The document whose flags are set. 
	@param flags IN/OUT A bit field composed of an <code>OR</code> of the PDDocFlags 
	values. 
	@see PDDocGetFlags 
	@see PDDocClearFlags 
	@since PI_PDMODEL_VERSION >= 0x00020000
*/
NPROC(void, PDDocSetFlags, (PDDoc doc, ASInt32 flags))

/**
	Gets the value of the PageMode key in the Catalog dictionary. 
	
	@note PDDocGetFullScreen should be used when the page mode 
	is set to full screen. 
	@param doc IN/OUT The document whose page mode is obtained. 
	@return Page mode value from PDF Catalog dictionary. 
	@see PDDocSetPageMode 
	@see AVDocSetViewMode 
	@since PI_PDMODEL_VERSION >= 0x00020000
*/
NPROC(PDPageMode, PDDocGetPageMode, (PDDoc doc))

/**
	Sets the value of the PageMode key in the Catalog dictionary. 
	
	@param doc IN/OUT The document whose page mode is set. 
	@param mode IN/OUT The page mode to set. 
	@see PDDocGetPageMode 
	@see AVDocSetViewMode 
	@since PI_PDMODEL_VERSION >= 0x00020000
*/
UNPROC(void, PDDocSetPageMode, (PDDoc doc, PDPageMode mode))

/**
	Gets a document's Cos-level document object. 
	@param doc The document whose CosDoc is obtained.
	@return The document's CosDoc. 
	@see CosObjGetDoc 
	@since PI_PDMODEL_VERSION >= 0x00020000
*/
NPROC(CosDoc, PDDocGetCosDoc, (PDDoc doc))

/**
	Gets the file object for a document. 
	@param doc The document whose ASFile is obtained.
	@return The document's ASFile. 
	@since PI_PDMODEL_VERSION >= 0x00020000
*/
NPROC(ASFile, PDDocGetFile, (PDDoc doc))

/**
	Gets an element of a document's file identifier. See Section 
	10.3 in the <i>PDF Reference</i> for a description of file IDs. 
	
	@param doc The document whose file ID is obtained. 
	@param nElemNum The element number to get from the document's 
	file ID. It must be one of the following: 
		
	<TABLE rules="all" cellspacing="1">
	<TR><TH>Value</TH><TH>Description</TH></TR>
	<TR><TD><code>0</code></TD><TD>The permanent ID.</TD></TR>
	<TR><TD><code>1</code></TD><TD>The permanent ID.</TD></TR>
	</TABLE>

	@param buffer (Filled by the method) If <code>buffer</code> is non-<code>NULL</code>, 
	then up to <code>bufferSize</code> bytes of the ID will be written 
	to the buffer. 
	@param bufferSize The length of <code>buffer</code> in bytes.
	@return The number of bytes in the ID element. 
	@since PI_PDMODEL_VERSION >= 0x00020000
*/
NPROC(ASInt32, PDDocGetID, (PDDoc doc, ASInt32 nElemNum, ASUns8 *buffer, ASInt32 bufferSize))

/**
	Gets the major and minor PDF document versions. This is 
	the PDF version of the document, which is specified in the 
	header of a PDF file in the string <code>"%PDF-xx. yy"</code> where <code>xx</code> 
	is the major version and <code>yy</code> is the minor version. For example, 
	version 1.2 has the string <code>"%PDF<code>-1</code>.2"</code>. See Section H.1 
	in the <i>PDF Reference</i>. 
	@param doc IN/OUT The document whose version is obtained. 
	@param majorP IN/OUT (Filled by the method) The major version 
	number. 
	@param minorP IN/OUT (Filled by the method) The minor version 
	number. 
	@since PI_PDMODEL_VERSION >= 0x00020000
*/
NPROC(void, PDDocGetVersion, (PDDoc doc, ASInt16 *majorP, ASInt16 *minorP))

/**
	Gets the root of the document's bookmark tree. The return 
	value is valid even if the document's bookmark tree is empty 
	(meaning that there is no Outlines key in the underlying 
	PDF file). 
	@param pdDoc IN/OUT Document whose root bookmark is obtained. 
	
	@return The document's root bookmark. 
	@since PI_PDMODEL_VERSION >= 0x00020000
*/
NPROC(PDBookmark, PDDocGetBookmarkRoot, (PDDoc pdDoc))

/**
	Gets the number of pages in a document. 
	@param doc IN/OUT The document for which the number of pages is 
	obtained. 
	@return The number of pages in the document. Remember to subtract 
	<code>1</code> from this value if you are going to pass it to a PD-
	level method that takes a zero-based page number. 
	@since PI_PDMODEL_VERSION >= 0x00020000
*/
NPROC(ASInt32, PDDocGetNumPages, (PDDoc doc))


/**
	Gets a PDPage from a document. It increments the page's reference 
	count. After you are done using the page, release it using 
	PDPageRelease(). If PDPageRelease() is not called, it could block
	the	document containing the page from being closed. To avoid such 
	problems, use the <code>CSmartPDPage</code> class, as it ensures that the page is 
	released as it goes out of scope. 
	@param doc The document containing the page to acquire. 
	
	@param pageNum The page number of the page to acquire. 
	The first page is <code>0</code>.
	@return The acquired page. 
	@exception genErrBadParm 
	@see CSmartPDPage 
	@see PDPageRelease 
	@since PI_PDMODEL_VERSION >= 0x00020000
*/
NPROC(PDPage, PDDocAcquirePage, (PDDoc doc, ASInt32 pageNum))

/**
	Creates and acquires a new page. The page is inserted into 
	the document at the specified location. Call PDPageRelease() 
	when you are done using the page. 
	@param doc The document in which the page is created. 
	
	@param afterPageNum The page number after which the new 
	page is inserted. The first page is <code>0</code>. Use PDBeforeFirstPage() 
	(see PDExpT.h) to insert the new page at the beginning of 
	a document. 
	@param mediaBox A rectangle specifying the page's media 
	box, specified in user space coordinates.
	@return The newly created page. 
	@notify PDDocWillInsertPages 
	@notify PDDocDidInsertPages 
	@notify PDDocDidChangePages 
	@notify PDDocDidChangeThumbs 
	@see PDPageRelease 
	@see PDDocDeletePages 
	@see PDDocInsertPages 
	@see PDDocReplacePages 
	@since PI_PDMODEL_VERSION >= 0x00020000
*/
UNPROC(PDPage, PDDocCreatePage, (PDDoc doc, ASInt32 afterPageNum, ASFixedRect mediaBox))

/**
	Deletes the specified pages. 
	@param doc The document from which pages are deleted. 
	
	@param firstPage The page number of the first page to 
	delete. The first page is <code>0</code>. 
	@param lastPage The page number of the last page to delete. 
	
	@param progMon A progress monitor. Use AVAppGetDocProgressMonitor() 
	to obtain the default progress monitor. <code>NULL</code> may be passed, 
	in which case no progress monitor is used. 
	@param progMonClientData A pointer to user-supplied data 
	passed to <code>progMon</code> each time it is called. It should be <code>NULL</code> if 
	progMon is <code>NULL</code>.
	@notify PDDocWillChangePages 
	@notify PDDocWillDeletePages 
	@notify PDDocDidDeletePages 
	@notify PDDocDidChangePages 
	@see PDDocInsertPages 
	@see PDDocReplacePages 
	@see PDDocMovePage 
	@since PI_PDMODEL_VERSION >= 0x00020000
*/
UNPROC(void, PDDocDeletePages, (PDDoc doc, ASInt32 firstPage, ASInt32 lastPage, ProgressMonitor progMon, void *progMonClientData))

/**
	Moves one page in a document. 
	@param doc The document in which the page is moved. 
	@param moveToAfterThisPage The new location of the page 
	to move. The first page is <code>0</code>. It may either be a page number, 
	or the constant PDBeforeFirstPage (see PDExpT.h). 
	@param pageToMove The page number of the page to move.
	@exception genErrBadParm is raised if <code>moveAfterThisPage</code> or <code>pageToMove</code> 
	is invalid. Other exceptions may be raised as well.
	@notify PDDocWillMovePages 
	@notify PDDocDidMovePages 
	@notify PDDocDidChangePages 
	@notify PDDocWillChangePages 
	@see PDDocInsertPages 
	@see PDDocReplacePages 
	@see PDDocDeletePages 
	@since PI_PDMODEL_VERSION >= 0x00020000
*/
XNPROC(void, PDDocMovePage, (PDDoc doc, ASInt32 moveToAfterThisPage, ASInt32 pageToMove))

/**
	Inserts <code>numPages</code> pages from <code>doc2</code> into <code>doc</code>. All annotations, 
	and anything else associated with the page (such as a thumbnail 
	image) are copied from the <code>doc2</code> pages to the new pages in 
	<code>doc</code>. This method does not insert pages if <code>doc</code> equals <code>doc2</code>. 

	<p>The <code>insertFlags</code> parameter controls whether bookmarks and threads are 
	inserted along with the specified pages. Setting The PDInsertAll 
	flag has two effects:</p>
	
	<ul>
	<li>The parameters indicating which pages to insert are
	ignored: all the pages of <code>doc2</code> are inserted.</li>
	<li><p>In addition to inserting the pages themselves, 
	it also merges other document data from <code>doc2</code> into <code>doc</code>:</p>
		<ul>
		<li>Named destinations from <code>doc2</code> (of PDF 1.1 and later) are 
		copied into <code>doc</code>. If there are named destinations in <code>doc2</code> 
		with the same name as some named destination in <code>doc</code>, the 
		ones in <code>doc</code> retain their names and the copied named destinations 
		are given new names based on the old ones, with distinguishing 
		digits added. Actions and bookmarks referring to the old 
		names are made to refer to the new names after being copied 
		into <code>doc</code>.</li>
		<li>If it is also the case that <code>mergeAfterThisPage</code> denotes the
		last page of the document, then document metadata is merged, and
		the optional content properties are merged in a more symmetrical
		manner than would otherwise be the case.</li>
		</ul>
	</li>
	</ul>

	<p>Document logical structure from <code>doc2</code> is copied into <code>doc</code>.
	If less than the whole of <code>doc2</code> is being inserted, only those
    structure elements having content on the copied pages, and
	the ancestors of those elements, are copied into the
	logical structure tree of <code>doc</code>.
	The top-level children of the structure tree root of <code>doc2</code> 
	are copied as new top-level children of the structure tree 
	root of <code>doc</code>; a structure tree root is created in <code>doc</code> if 
	there was none before. The role maps of the two structure 
	trees are merged, with name conflicts resolved in favor 
	of the role mappings present in <code>doc</code>. Attribute objects
	having scalar values, or values that are arrays of scalar values, are 
	copied. Class map information from <code>doc2</code> is also merged 
	into that for <code>doc</code>. </p>
	@param doc The document into which pages are inserted. 
	This document must have at least one page. 
	@param mergeAfterThisPage The page number in <code>doc</code> after 
	which pages from <code>doc2</code> are inserted. The first page is <code>0</code>. 
	If PDBeforeFirstPage (see PDExpT.h) is used, the pages are 
	inserted before the first page in <code>doc</code>. Use PDLastPage to 
	insert pages after the last page in <code>doc</code>.
	@param doc2 The document containing the pages that are 
	inserted into <code>doc</code>. 
	@param startPage The page number of the first page in 
	<code>doc2</code> to insert into <code>doc</code>. The first page is <code>0</code>. 
	@param numPages The number of pages in <code>doc2</code> to insert 
	into <code>doc</code>. Use PDAllPages to insert all pages from <code>doc2</code> into 
	<code>doc</code>. 
	@param insertFlags Flags that determine what additional 
	information is copied from <code>doc2</code> into <code>doc</code>. It is an <code>OR</code> of the following 
	constants (see PDExpT.h):
	
	<TABLE rules="all" cellspacing="1">
	<TR><TH>Constant</TH><TH>Description</TH></TR>
	<TR><TD>PDInsertBookmarks</TD><TD>Inserts bookmarks as well as pages.
	The bookmark tree of <code>doc2</code> is merged into the bookmark 
	tree of <code>doc</code> by copying it as a new first-level subtree of the 
	<code>doc</code> parameter's bookmark tree root, of which it becomes the last child. 
	If <code>doc</code> has no bookmark tree, it acquires one identical to 
	the bookmark tree from <code>doc2</code>.</TD></TR>
	<TR><TD>PDInsertThreads</TD><TD>Inserts threads as well as pages.</TD></TR>
	<TR><TD>PDInsertAll</TD><TD>Inserts document data from pages.</TD></TR>
	</TABLE>

	@param progMon A progress monitor. Use AVAppGetDocProgressMonitor() 
	to obtain the default progress monitor. <code>NULL</code> may be passed, 
	in which case no progress monitor is used. 
	@param progMonClientData A pointer to user-supplied data 
	to pass to <code>progMon</code> each time it is called. It should be <code>NULL</code> 
	if <code>progMon</code> is <code>NULL</code>. 
	@param cancelProc A cancel procedure. Use AVAppGetCancelProc() 
	to obtain the current cancel procedure. It may be <code>NULL</code>, in 
	which case no cancel proc is used. 
	@param cancelProcClientData A pointer to user-supplied data 
	to pass to <code>cancelProc</code> each time it is called. It should be 
	<code>NULL</code> if <code>cancelProc</code> is <code>NULL</code>.
	@exception pdErrOpNotPermitted is raised unless <code>doc</code> is editable and <code>doc2</code> 
	is not encrypted or the owner opened it. 
	@exception pdErrCantUseNewVersion is raised if <code>doc2</code> is a newer major version 
	than the Acrobat viewer understands. 
	@exception pdErrTooManyPagesForInsert is raised if the insertion would 
	result in a document with too many pages. 
	@exception genErrBadParm is raised if <code>mergeAfterThisPage</code> is an invalid 
	page number or <code>doc</code> has no pages. 
	@exception genErrNoMemory is raised if there is insufficient memory to 
	perform the insertion. 
	@exception pdErrWhileRecoverInsertPages is raised if an error occurs while 
	trying to recover from an error during inserting. 
	@notify PDDocWillInsertPages 
	@notify PDDocDidInsertPages 
	@notify PDDocDidChangePages 
	@notify PDDocPrintingTiledPage 
	@notify PDDocDidChangeThumbs 
	@notify PDDocDidAddThread 
	@see AVAppGetCancelProc 
	@see AVAppGetDocProgressMonitor 
	@see PDDocCreatePage 
	@see PDDocDeletePages 
	@see PDDocMovePage 
	@see PDDocReplacePages 
	@since PI_PDMODEL_VERSION >= 0x00020000
*/

XNPROC(void, PDDocInsertPages, (PDDoc doc, ASInt32 mergeAfterThisPage, PDDoc doc2, ASInt32 startPage, ASInt32 numPages, ASUns16 insertFlags, ProgressMonitor
	progMon, void *progMonClientData, CancelProc cancelProc, void *cancelProcClientData))

/**
	Replaces the specified range of pages in one document with 
	pages from another. The contents, resources, size and rotation 
	of the pages are replaced. The bookmarks are not copied, because 
	they are attached to the document, not to individual pages. 

	@param doc The document in which pages are replaced. 
	@param startPage The first page number in <code>doc</code> to replace. 
	The first page is <code>0</code>. 
	@param doc2 The document from which pages are copied into 
	<code>doc</code>. 
	@param startPageDoc2 The page number of the first page 
	in <code>doc2</code> to copy. The first page is <code>0</code>. 
	@param numPages The number of pages to replace. 
	@param mergeTextAnnots If <code>true</code>, text annotations from 
	<code>doc2</code> are appended if they are different than all existing 
	annotations on the page in <code>doc</code>. No other types of annotations 
	are copied. 
	@param progMon A progress monitor. Use AVAppGetDocProgressMonitor() 
	to obtain the default progress monitor. <code>NULL</code> may be passed, 
	in which case no progress monitor is used. 
	@param progMonClientData A pointer to user-supplied data 
	to pass to <code>progMon</code> each time it is called. It should be <code>NULL</code> 
	if <code>progMon</code> is <code>NULL</code>. 
	@param cancelProc Currently unused. A cancel procedure. 
	Use AVAppGetCancelProc() to obtain the current cancel procedure. 
	It may be <code>NULL</code>, in which case no cancel proc is used.
	@param cancelProcClientData A pointer to user-supplied data 
	to pass to <code>cancelProc</code> each time it is called. It should be 
	<code>NULL</code> if <code>cancelProc</code> is <code>NULL</code>.
	@exception pdErrOpNotPermitted is raised unless <code>doc</code> is editable and <code>doc2</code> 
	is not encrypted or the owner opened it. 
	@exception pdErrCantUseNewVersion is raised if <code>doc2</code> is a newer major version 
	than the Acrobat viewer understands. 
	@exception genErrBadParm is raised if one of the following conditions is true:
	<ul>	
	<li><code>numPages &lt; 1</code> </li> 
	<li><code>startPage &lt; 0</code> </li> 
	<li><code>startPage + numPages</code> is greater than the number of pages in <code>doc</code> </li> 
	<li><code>startPageDoc2 &lt; 0</code> </li> 
	<li><code>startPageDoc2 + numPages</code> is greater than the number of pages in <code>doc2</code> </li> 
	</ul>

	@exception genErrNoMemory is raised if there is insufficient memory to perform the insertion. 
	@notify PDDocWillReplacePages 
	@notify PDDocDidReplacePages 
	@notify PDDocDidChangePages 
	@notify PDDocWillChangePages 
	@see PDDocInsertPages 
	@see PDDocMovePage 
	@see PDDocDeletePages 
	@see PDDocCreatePage 
	
	@note Annotations in the replaced pages are not replaced 
	and remain with the page. Use PDDocDeletePages() to remove 
	annotations. 
	@since PI_PDMODEL_VERSION >= 0x00020000
*/
XNPROC(void, PDDocReplacePages, (PDDoc doc, ASInt32 startPage, PDDoc doc2, ASInt32 startPageDoc2, ASInt32 numPages, ASBool mergeTextAnnots, ProgressMonitor progMon, void *progMonClientData,
	CancelProc cancelProc, void *cancelProcClientData))


/**
	Gets the number of article threads in a document. 
	@param doc IN/OUT The document whose article thread count is obtained. 
	
	@return The number of article threads in the document. 
	@since PI_PDMODEL_VERSION >= 0x00020000
*/
NPROC(ASInt32, PDDocGetNumThreads, (PDDoc doc))

/**
	Gets an article thread having the specified index. 
	@param doc IN/OUT The document containing the article thread. 
	
	@param index IN/OUT The index of the article thread to obtain. 
	
	@return The specified article thread. 
	@see PDDocAddThread 
	@see PDDocRemoveThread 
	@see PDThreadIsValid 
	@since PI_PDMODEL_VERSION >= 0x00020000
*/
NPROC(PDThread, PDDocGetThread, (PDDoc doc, ASInt32 index))

/**
	Gets the index of the specified article thread. 
	@param doc IN/OUT The document containing the thread. 
	@param thread IN/OUT The thread whose index is obtained. 
	@return The index of thread in doc. It returns <code>-1</code> if <code>thread</code> is not 
	in <code>doc</code>. 
	@since PI_PDMODEL_VERSION >= 0x00020000
*/
NPROC(ASInt32, PDDocGetThreadIndex, (PDDoc doc, PDThread thread))

/**
	Adds an article thread to a document after the specified 
	thread index. 
	@param doc IN/OUT The document in which the thread is added. It must 
	match the document used in the call to PDThreadNew() that 
	created the thread. 
	@param addAfterIndex IN/OUT The index of the thread after which 
	<code>thread</code> is added. 
	@param thread IN/OUT The thread to add. 
	@notify PDDocDidAddThread 
	@see PDThreadNew 
	@see PDThreadFromCosObj 
	@see PDDocRemoveThread 
	@since PI_PDMODEL_VERSION >= 0x00020000
*/
UNPROC(void, PDDocAddThread, (PDDoc doc, ASInt32 addAfterIndex, PDThread thread))

/**
	Removes an article thread from a document. If you also wish 
	to destroy the thread, use PDThreadDestroy() after calling 
	PDDocRemoveThread(). 
	@param doc IN/OUT The document from which the thread is removed. 
	
	@param index IN/OUT The index of the thread to remove. 
	@notify PDDocWillRemoveThread 
	@notify PDDocDidRemoveThread 
	@see PDThreadDestroy 
	@see PDDocAddThread 
	@see PDBeadGetThread 
	@since PI_PDMODEL_VERSION >= 0x00020000
*/
UNPROC(void, PDDocRemoveThread, (PDDoc doc, ASInt32 index))


/**
	Enumerates all the fonts in the specified page range. This 
	may take a considerable amount of time for a large page 
	range. 
	@param doc The document whose fonts are enumerated. 
	@param firstPage The page number of the first page for 
	which fonts are enumerated. The first page is <code>0</code>. 
	@param lastPage The page number of the last page for which 
	fonts are enumerated. 
	@param eproc A user-supplied callback to call for each font. 
	Enumeration terminates if <code>eproc</code> returns <code>false</code>. 
	@param clientData A pointer to user-supplied data to pass 
	to <code>eproc</code> each time it is called. 
	@param progMon A progress monitor. Use AVAppGetDocProgressMonitor() 
	to obtain the standard progress monitor. <code>NULL</code> may be passed, 
	in which case no progress monitor is used. 
	@param progMonClientData A pointer to user-supplied data 
	to pass to <code>progMon</code> each time it is called. It should be <code>NULL</code> 
	if <code>progMon</code> is <code>NULL</code>.
	@see PDDocEnumLoadedFonts 
	@see PDFontEnumCharProcs 
  	@ingroup Enumerators
	@since PI_PDMODEL_VERSION >= 0x00020000
*/
NPROC(void, PDDocEnumFonts, (PDDoc doc, ASInt32 firstPage, ASInt32 lastPage, PDFontEnumProc eproc, void *clientData, ProgressMonitor progMon, void *progMonClientData))

/**
	Enumerates all the fonts that have been encountered so far. 
	A font is loaded when a page that uses it is processed. 
	This typically happens when a page is drawn or its thumbnail 
	image is created. 
	@param doc IN/OUT The document whose loaded fonts are enumerated. 
	
	@param proc IN/OUT A user-supplied callback to call for each loaded 
	font. Enumeration terminates if <code>proc</code> returns <code>false</code>. 
	@param clientData IN/OUT A pointer to user-supplied data to pass 
	to <code>proc</code> each time it is called. 
	@see PDDocEnumFonts 
	@see PDFontEnumCharProcs 
  	@ingroup Enumerators
	@since PI_PDMODEL_VERSION >= 0x00020000
*/
NPROC(void, PDDocEnumLoadedFonts, (PDDoc doc, PDFontEnumProc proc, void *clientData))


/**
	Creates thumbnail images for the specified range of pages. 
	Thumbnail images are only created for pages that have none. 
	
	<p>Use as large a page range as possible, because the color 
	space object is shared by all the thumbnails created by 
	a single invocation of this method. This means that if you call 
	this method separately for each page, there will be duplicate 
	color space objects. </p>

	<p>See Section 4.5 in the <i>PDF Reference</i> for more details on 
	color spaces. </p>

	See <i>Developing Plug-ins and Applications</i> for additional important information 
	about creating thumbnails. 
	@param doc IN/OUT The document for which thumbnail images are 
	created. 
	@param firstPage IN/OUT The page number of the first page for 
	which thumbnails are created. The first page is <code>0</code>. 
	@param lastPage IN/OUT The page number of the last page for which 
	thumbnails are created. The constant PDLastPage (see PDExpT.h) can also be used. 
	@param server IN/OUT A server (set of callback procedures) that 
	provides the sampled image used as the thumbnail image. 
	Pass <code>NULL</code> to use the default server. 
	@param serverClientData IN/OUT User-supplied data to pass to the 
	thumbnail creation server. 
	@param colorSpace IN/OUT The color space in which the thumbnail 
	data is represented. It must be <code>DeviceRGB</code>. Thumbnails may 
	be created in either a direct or an indexed color space; 
	however, it is strongly recommended that you use indexed 
	color spaces over direct color spaces. Using direct color 
	spaces with this version of Acrobat may cause bad looking 
	thumbnails. To specify a direct color space, pass <code>0</code> for 
	<code>hiVal</code> and <code>NULL</code> for <code>lookupTable</code>. To specify an indexed color 
	space, pass the appropriate values in <code>hiVal</code> and <code>lookupTable</code>. 
	Direct color spaces on Windows are supported in Acrobat 
	3.0. Prior to Acrobat 3.0 on Windows, you had to use indexed 
	color spaces. 
	@param bitsPerComponent IN/OUT The number of bits per color component 
	in the thumbnail image's data. <code>8</code> is the only valid value. 
	
	@param hiVal IN/OUT Used only for indexed color space; pass <code>0</code> 
	for direct color spaces, as described in <code>colorSpace</code>. <code>hiVal</code> 
	specifies the highest valid index in <code>lookupTable</code>. Because 
	indices start at <code>0</code>, the number of entries in <code>lookupTable</code> 
	is <code>hiVal + 1</code>. <code>hiVal</code> must be <code>0</code> for device color spaces. 
	@param lookupTable IN/OUT Used only for indexed color space; pass 
	<code>NULL</code> for direct color spaces, as described in <code>colorSpace</code>. 
	<code>lookupTable</code> is a table that maps data values to colors. 
	It is used only for indexed color spaces. It must be <code>NULL</code> for device 
	color spaces. For an indexed color space, the size of the 
	lookup table must be <code>(hiVal + 1)</code>:
	
	<ul>
	<li><code>sizeof(ASUns8)</code></li>
	<li><code>3</code>, where the <code>3</code> arises because an RGB color space has three color components.</li>
	</ul>
	
	@param progMon IN/OUT A monitor to call to display thumbnail creation 
	progress. Use AVAppGetDocProgressMonitor() to obtain the standard 
	progress monitor to pass for this parameter. <code>NULL</code> may be 
	passed, in which case no progress monitor is used. 
	@param progMonClientData IN/OUT A user-supplied data to pass to 
	<code>progMon</code> each time it is called. It should be <code>NULL</code> if <code>progMon</code> 
	is <code>NULL</code>. 
	@param cancelProc IN/OUT A procedure to call frequently to allow 
	the user to cancel thumbnail creation. Use AVAppGetCancelProc() 
	to obtain the default cancel proc for this parameter. It may 
	be <code>NULL</code>, in which case no cancel proc is used. 
	@param cancelProcClientData IN/OUT A user-supplied data to pass to 
	<code>cancelProc</code> each time it is called. It should be <code>NULL</code> if <code>cancelProc</code> is <code>NULL</code>. 
	
	@notify PDDocDidChangeThumbs 
	@see PDDocDeleteThumbs 
	@since PI_PDMODEL_VERSION >= 0x00020000
*/
UNPROC(void, PDDocCreateThumbs, (PDDoc doc, ASInt32 firstPage, ASInt32 lastPage,
	PDThumbCreationServer server, void *serverClientData,
	ASAtom colorSpace, ASInt32 bitsPerComponent, ASInt32 hiVal,
	char *lookupTable, ProgressMonitor progMon, void *progMonClientData,
	CancelProc cancelProc, void *cancelProcClientData))

/**
	Deletes thumbnail images for a range of pages in a document. 
	
	@param doc IN/OUT The document from which thumbnail images are 
	deleted. 
	@param firstPage IN/OUT The page number of the first page in <code>doc</code> 
	whose thumbnail image is deleted. The first page is <code>0</code>. 
	@param lastPage IN/OUT The page number of the last page in <code>doc</code> 
	whose thumbnail image is deleted. 
	@param progMon IN/OUT A monitor to call to display thumbnail deletion 
	progress. Use AVAppGetDocProgressMonitor() to obtain the standard 
	progress monitor to pass for this parameter. <code>NULL</code> may be 
	passed, in which case no progress monitor is used. 
	@param progMonClientData IN/OUT A pointer to user-supplied data 
	to pass to <code>progMon</code>. It should be <code>NULL</code> if <code>progMon</code> is <code>NULL</code>. 
	@notify PDDocDidChangeThumbs 
	@see PDDocCreateThumbs 
	@since PI_PDMODEL_VERSION >= 0x00020000
*/
UNPROC(void, PDDocDeleteThumbs, (PDDoc doc, ASInt32 firstPage, ASInt32 lastPage, ProgressMonitor progMon, void *progMonClientData))


/**
	Gets the word finder associated with a document. It is not 
	necessary to destroy the word finder returned by this method. 
	
	@param docP The document whose word finder is obtained. 
	
	@param WXEVersion The version of the word finder to get.
	@return The document's word finder. It returns <code>NULL</code> if the document 
	does not have a word finder or its version does not match 
	the version requested. 
	@exception genErrBadParm is thrown if an invalid version number is passed. 
	
	@see PDDocCreateWordFinder 
	@see PDDocCreateWordFinderUCS 
	@since PI_PDMODEL_VERSION >= 0x00020000
*/
SPROC(PDWordFinder, PDDocGetWordFinder, (PDDoc docP, ASInt16 WXEVersion), PDDocGetWordFinderHost)

/**
	Creates a word finder that is used to extract text in the 
	host encoding from a PDF file. The word finder may either 
	be used by PDWordFinderEnumWords() (which enumerates words 
	one-by-one) or by PDWordFinderAcquireWordList() (which fills 
	a table with all the words on a page). 
	
	<p>A default ligature table is used, containing the following 
	ligatures: </p> 
	<ul>
	<li>fi</li> 
	<li>ff</li> 
	<li>fl</li> 
	<li>ffi</li> 
	<li>ffl</li> 
	<li>ch</li> 
	<li>cl</li> 
	<li>ct</li> 
	<li>ll</li> 
	<li>ss</li> 
	<li>fs</li> 
	<li>st</li> 
	<li>oe</li> 
	<li>OE</li>
	</ul>

	<p>The glyph name is substituted for the ligature. </p>

	<p>This method also works for non-Roman (CJK or Chinese-Japanese-Korean) 
	viewers. In this case, words are extracted to the host encoding. 
	Developers desiring Unicode output must use PDDocCreateWordFinderUCS(), 
	which does the extraction for Roman or non-Roman text. </p>

	<p>The type of PDWordFinder determines the encoding of the 
	string returned by PDWordGetString(). For instance, if PDDocCreateWordFinderUCS() 
	is used to create the word finder, PDWordGetString() returns 
	only Unicode. </p>

	<p>For CJK viewers, words are stored internally using CID encoding. 
	For more information on CIDFonts and related topics, see 
	Section 5.6 in the <i>PDF Reference</i>. For detailed information 
	on CIDFonts, see <i>Technical Note #5092, CID-Keyed Font Technology 
	Overview</i>, and <i>Technical Note #5014, Adobe CMap and CIDFont 
	Files Specification</i>. </p>
	@param doc The document on which the word finder is used. 
	
	@param outEncInfo An array of 256 flags, specifying the type 
	of character at each position in the encoding. Each flag 
	is an <code>OR</code> of the Character Type Codes. If <code>outEncInfo</code> is <code>NULL</code>, 
	the platform's default encoding info is used. Use <code>outEncInfo</code> 
	and <code>outEncVec</code> together; for every <code>outEncInfo</code> use a corresponding 
	<code>outEncVec</code> to specify the character at that position in the 
	encoding. Regardless of the characters specified in <code>outEncInfo</code> 
	as word separators, a default set of word separators is 
	used (see Glyph Names of Word Separators). There is no way 
	to change the list of characters that are considered to 
	be word separators.
	@param outEncVec Array of 256 <code>NULL</code>-terminated strings 
	that are the glyph names in encoding order. See the discussion 
	of character names in the <i>PostScript Language 
	Reference Manual</i>. If <code>outEncVec</code> is <code>NULL</code>, the 
	platform's default encoding vector is used. For non-UNIX 
	Roman systems, it is <code>WinAnsiEncoding</code> 
	on Windows and <code>MacRomanEncoding</code> on Mac OS. On UNIX (except HP-UX) Roman systems, it is 
	<code>ISO8859-1</code> (ISO Latin-1); for HP-UX, it is <code>HP-ROMAN8</code>. See 
	Appendix D in the <i>PDF Reference</i> for descriptions of  
	<code>WinAnsiEncoding</code>, <code>MacRomanEncoding</code>, and <code>PDFDocEncoding</code>. Use this parameter 
	with <code>outEncInfo</code>. See <code>outEncInfo</code> for more information. 
	@param ligatureTbl A <code>NULL</code>-terminated array of <code>NULL</code>-terminated 
	strings. Each string is the glyph name of a ligature in 
	the font. When a word contains a ligature, the glyph name 
	of the ligature is substituted for the ligature (for example, 
	<code>ff</code> is substituted for the ff ligature). This table must 
	be terminated with <code>NULL</code>. If <code>ligatureTbl</code> is <code>NULL</code>, a default 
	ligature table is used, containing the following ligatures: 
	<ul>
	<li>fi</li> 
	<li>ff</li> 
	<li>fl</li> 
	<li>ffi</li> 
	<li>ffl</li> 
	<li>ch</li> 
	<li>cl</li> 
	<li>ct</li> 
	<li>ll</li> 
	<li>ss</li> 
	<li>fs</li> 
	<li>st</li> 
	<li>oe</li> 
	<li>OE</li>
	</ul>
	
	@param algVersion The version of the word-finding algorithm 
	to use (see PDExpT.h), as follows (pass <code>0</code> if your client 
	does not care): 
	
	<TABLE rules="all" cellspacing="1">
	<TR><TH>Version</TH><TH>Description</TH></TR>
	<TR><TD><code>WF_LATEST_VERSION</code></TD><TD>To obtain the latest available version.</TD></TR>
	<TR><TD><code>WF_VERSION_2</code></TD><TD>Version used for Acrobat 3.x, 4.x.</TD></TR>
	<TR><TD><code>WF_VERSION_3</code></TD><TD>Available in Acrobat 5.0 without accessibility enabled. Includes some improved word-piecing algorithms.</TD></TR>
	<TR><TD><code>WF_VERSION_4</code></TD><TD>For Acrobat 5.0 with accessibility enabled. Includes advanced word-ordering algorithms in addition to improved word-piecing algorithms.</TD></TR>
	</TABLE>

	@param rdFlags Word-finding options that determine the 
	tables filled when using PDWordFinderAcquireWordList(). It must 
	be an <code>OR</code> of one or more of the WordFinder Sort Order Flags. 
	In Acrobat 5.0 this parameter is ignored and you should 
	pass in <code>NULL</code>. 
	@param clientData A pointer to user-supplied data to pass 
	to the newly created word finder.
	@return The newly created word finder. 
	@see PDDocCreateWordFinderUCS 
	@see PDWordFinderEnumWords 
	@see PDWordFinderAcquireWordList 
	@see PDWordFinderDestroy 
	@see PDWordFilterWord 

	@note The word finder also extracts text from Form XObjects 
	that are executed in the page contents. For information 
	about Form XObjects, see Section 4.9 in the <i>PDF Reference</i>. 
  	@ref GlyphNames
  	@ref CharacterTypeCodes
  	@ref WordFinderSortOrderFlags
	@since PI_PDMODEL_VERSION >= 0x00020000
*/
NPROC(PDWordFinder, PDDocCreateWordFinder,(PDDoc doc, ASUns16 *outEncInfo, char **outEncVec, char **ligatureTbl, ASInt16 algVersion, ASUns16 rdFlags, void *clientData ))

/**
	Gets the nth word in the word list obtained using PDWordFinderAcquireWordList(). 
	
	@param wObj IN/OUT The word finder whose nth word is obtained. 
	
	@param nTh IN/OUT The index of the word to obtain. The first word 
	on a page has an index of zero. Words are counted in PDF 
	order. See the description of the <code>wInfoP</code> parameter in PDWordFinderAcquireWordList(). 
	
	@return The nth word. It returns <code>NULL</code> when the end of the list is reached. 
	
	@see PDWordFinderAcquireWordList 
	@see PDWordFinderEnumWords 
	@since PI_PDMODEL_VERSION >= 0x00020000
*/
NPROC(PDWord, PDWordFinderGetNthWord, (PDWordFinder wObj, ASInt32 nTh))

/**
	Splits the specified string into words by substituting spaces 
	for word separator characters. The list of characters considered 
	to be word separators can be specified, or a default list 
	can be used. 

	<p>The characters <code>','</code> and <code>'.'</code> are context-sensitive word separators. 
	If surrounded by digits (for example, <code>654,096.345</code>), they 
	are not considered word separators. </p>

	<p>For non-Roman character set viewers, this method currently 
	supports only SHIFT-JIS encoding on a Japanese system. </p>
	@param infoArray A character information table. It specifies 
	each character's type; word separator characters must be 
	marked as <code>W_WORD_BREAK</code> (see Character Type Codes). This 
	table can be identical to the table to pass to PDDocCreateWordFinder(). 
	If <code>infoArray</code> is <code>NULL</code>, a default table is used (see Glyph 
	Names of Word Separators). 
	@param cNewWord (Filled by the method) The word that has been 
	split. Word separator characters have been replaced with 
	spaces. 
	@param cOldWord The word to split. 
	@param nMaxLen The number of characters that <code>cNewWord</code> can 
	hold. Word splitting stops when <code>cOldWord</code> is completely processed 
	or <code>nMaxLen</code> characters have been placed in <code>cNewWord</code>, whichever 
	occurs first.
	@return The number of splits that occurred. 
	@exception genErrGeneral is raised if <code>infoArray</code> is <code>NULL</code>, but host encoding 
	cannot be obtained. 
	@see PDWordGetString 
  	@ref CharacterTypeCodes
	@since PI_PDMODEL_VERSION >= 0x00020000
*/
NPROC(ASUns16, PDWordSplitString, (ASUns16 *infoArray, char *cNewWord, char *cOldWord, ASUns16 nMaxLen))

/**
	Creates a text selection that includes all words totally 
	or partially enclosed by a rectangle. The text selection 
	can then be set as the current selection using AVDocSetSelection(). 
	
	@param doc The document in which a text selection is created. 
	
	@param pageNum The page number on which the text selection 
	is created. 
	@param boundingRect A pointer to a rectangle specifying 
	the text selection's bounding rectangle, specified in user 
	space coordinates.
	@return The newly created text selection. 
	@see PDTextSelectDestroy 
	@see AVDocSetSelection 
	@see PDTextSelectEnumQuads 
	@see PDTextSelectEnumText 
	@see PDWordCreateTextSelect 
	
	@note When this method is used to create a text selection 
	on a rotated page, you must pass in a rotated <code>boundingRect</code>. 
	@since PI_PDMODEL_VERSION >= 0x00020000
*/
SPROC(PDTextSelect, PDDocCreateTextSelect, (PDDoc doc, ASInt32 pageNum, ASFixedRect *boundingRect), PDDocCreateTextSelectHost)



/**
	Gets the value of a key in a document's Info dictionary, 
	or the value of this same key in the XMP metadata, whichever 
	is later. However, it is preferable to use PDDocGetXAPMetadataProperty(), 
	because it also allows accessing XMP properties that are 
	not duplicated in the Info dictionary. 

	<p>See Section 1<code>0</code>.2.1 in the <i>PDF Reference</i> for information 
	about Info dictionaries. All values in the Info dictionary 
	should be strings; other data types such as numbers and 
	booleans should not be used as values in the Info dictionary. </p>
	
	<p>Users may define their own Info dictionary entries. In this 
	case, it is strongly recommended that the key have the developer's 
	prefix assigned by the Adobe Solutions Network. </p>
	@param doc The document whose Info dictionary key is obtained. 
	
	@param infoKey The name of the Info dictionary key whose 
	value is obtained. 
	@param buffer (Filled by the method) The buffer containing 
	the value associated with <code>infoKey</code>. If <code>buffer</code> is <code>NULL</code>, the 
	method will just return the number of bytes required. 
	@param bufSize The maximum number of bytes that can be 
	written into <code>buffer</code>.
	@return If <code>buffer</code> is <code>NULL</code>, it returns the number of bytes in the specified 
	key's value. If <code>buffer</code> is not <code>NULL</code>, it returns the number of 
	bytes copied into <code>buffer</code>, excluding the terminating <code>NULL</code>. 
	You must pass at least the <code>length + 1</code> as the buffer size 
	since the routine adds a <code>'\\0'</code> terminator to the data, even 
	though the data is not a C string (it can contain embedded 
	<code>'\\0'</code> values). 
	@see PDDocGetXAPMetadataProperty 
	@see PDDocSetInfo 

	@note For Roman viewers, this text is always stored in the 
	<code>PDFDocEncoding</code>. For non-Roman character set viewers, this 
	text is stored as <code>PDFDocEncoding</code> or Unicode, depending on 
	the file's creator. 
	@since PI_PDMODEL_VERSION >= 0x00020000
*/
NPROC(ASInt32, PDDocGetInfo, (PDDoc doc, const char *infoKey, char *buffer, ASInt32 bufSize))

/**
	Sets the value of a key in a document's Info dictionary. 
	However, it is preferable to use PDDocSetXAPMetadataProperty(), 
	because it also allows accessing XMP properties that are 
	not duplicated in the Info dictionary. 

	<p>See Section 1<code>0</code>.2.1 on Info dictionaries in the <i>PDF Reference</i> 
	for information about Info dictionaries. All values in the 
	Info dictionary should be strings; other data types such 
	as numbers and Boolean values should not be used as values in 
	the Info dictionary. If an Info dictionary key is specified 
	that is not currently in the Info dictionary, it is added 
	to the dictionary. </p>

	<p>Users may define their own Info dictionary entries. In this 
	case, it is strongly recommended that the key have the developer's 
	prefix assigned by the Adobe Developers Association. </p>

	@note For Roman viewers, this text is always stored in the 
	<code>PDFDocEncoding</code>. For non-Roman character set viewers, this 
	text is stored as <code>PDFDocEncoding</code> or Unicode, depending on 
	the file's creator. 
	@param doc The document whose Info dictionary key is set. 
	
	@param infoKey The name of the Info dictionary key whose 
	value is set. 
	@param buffer The buffer containing the value to associate 
	with <code>infoKey</code>. 
	@param nBytes The number of bytes in <code>buffer</code>.
	@see PDDocGetInfo 
	@see PDDocSetXAPMetadataProperty 
	@since PI_PDMODEL_VERSION >= 0x00020000
*/
UNPROC(void, PDDocSetInfo, (PDDoc doc, const char *infoKey, const char *buffer, ASInt32 nBytes))

#ifndef ANDROID_ENV
/**
	Superseded in Acrobat 5.0 by PDDocPermRequest. 

	<p>Gets the security data structure for the specified document's 
	current security handler. Use PDDocGetNewSecurityData() to 
	get the data structure for the document's new security handler. </p>
	
	@param doc The document whose security data structure 
	is obtained.
	@return A pointer to the document's current security data structure. 
	
	@see PDDocGetNewSecurityData 
	@see PDDocGetCryptHandler
	@since PI_PDMODEL_VERSION >= 0x00020000
*/
NPROC(void *, PDDocGetSecurityData, (PDDoc doc))

/**
	Gets the security data structure for the specified document's 
	new security handler. Use PDDocGetSecurityData() to get the 
	security data structure for the document's current security 
	handler. 
	@param doc The document whose new security data structure 
	is obtained.
	@return The security data structure for the document's new security 
	handler. 
	@see PDDocGetSecurityData 
	@see PDDocNewSecurityData
	@see PDDocGetNewCryptHandler
	@see PDDocSetNewCryptHandler 
	@see PDDocSetNewSecurityData 
	@see PDDocPermRequest 
	@since PI_PDMODEL_VERSION >= 0x00020000
*/
NPROC(void *, PDDocGetNewSecurityData, (PDDoc doc))

/**
	Deprecated in Acrobat 7.0. Use PDDocPermRequest() instead. 

	<p>Adds permissions to the specified document, if permitted. 
	It calls the PDCryptAuthorizeProc() callback of the document's 
	security handler to determine which of the specified permissions 
	will actually be granted. After calling this method, the 
	document's permissions will be the <code>OR</code> of the previous permissions 
	and the permissions granted by the PDCryptAuthorizeProc() 
	callback. </p>

	<p>Use PDDocPermRequest() to determine if a document's permissions 
	allow a particular operation for a particular object. </p>
	@param pdDoc The document for which new permissions are 
	requested. 
	@param permsWanted The new permissions being requested. 
	It must be an <code>OR</code> of the PDPerms values. 
	@param authData A pointer to data to pass to the PDCryptAuthorizeProc() 
	callback of the document's security handler. For the Acrobat 
	viewer's built-in security handler, <code>authData</code> is a <code>char*</code> 
	containing the password.
	@return The <code>OR</code> of the previous value of the document's permissions 
	field, and the permissions granted by the PDCryptAuthorizeProc() 
	callback of the document's security handler. The result 
	will be an <code>OR</code> of the PDPerms values. 
	@exception pdErrNeedCryptHandler is raised if no security handler is associated 
	with <code>pdDoc</code>. It also raises whatever exceptions are raised by the security handler's 
	PDCryptAuthorizeProc() callback. 
	@see PDDocGetNewCryptHandler 
	@see PDDocGetPermissions (obsolete) 
	@see PDDocPermRequest 
	@see PDDocSetNewCryptHandler 
	@see PDDocSetNewCryptHandlerEx 
	@see PDRegisterCryptHandler 
	@see PDRegisterCryptHandlerEx 
	@since PI_PDMODEL_VERSION >= 0x00020000
*/
NPROC(PDPerms, PDDocAuthorize, (PDDoc pdDoc, PDPerms permsWanted, void *authData))

/**
	Creates a security data structure appropriate for the specified 
	document's new security handler. The new security handler 
	must have been previously set using PDDocSetNewCryptHandler(). 
	The structure is created by calling the new security handler's 
	PDCryptNewSecurityDataProc(). 

	<p>After calling PDDocNewSecurityData(), fill the structure as 
	appropriate, call PDDocSetNewSecurityData() with the 
	structure, and then free the structure using ASfree(). </p>
	@param doc The document for which a security data structure 
	is created.
	@return The newly created security data structure. 
	@exception pdErrOpNotPermitted is raised if pdPermSecure (see PDPerms) 
	has not been granted for <code>doc</code>. 
	@exception pdErrNeedCryptHandler is raised if the document does not have 
	a new security handler. 
	@see PDDocSetNewCryptHandler 
	@see PDDocSetNewSecurityData 
	@since PI_PDMODEL_VERSION >= 0x00020000
*/
NPROC(void *, PDDocNewSecurityData, (PDDoc doc))

/**
	Sets the security data structure for the specified document's 
	new security handler. Use PDDocSetNewCryptHandler() to set 
	a new security handler for a document. 
	@param pdDoc IN/OUT The document whose new security data structure 
	is set. 
	@param secData IN/OUT A pointer to the new security data structure 
	to set for <code>doc</code>. See PDDocNewSecurityData() for information 
	on creating and filling this structure. 
	@exception pdErrNeedCryptHandler is raised if the document does not have 
	a new security handler. 
	@exception pdErrOpNotPermitted is raised if the document's permissions 
	cannot be changed. 
	@see PDDocGetNewSecurityData 
	@see PDDocGetSecurityData 
	@see PDDocNewSecurityData 
	@see PDDocSetNewCryptHandler 
	@since PI_PDMODEL_VERSION >= 0x00020000
*/
UNPROC(void, PDDocSetNewSecurityData, (PDDoc pdDoc, void *secData))

/**
	Sets the specified document's new security handler (that is, 
	the security handler that will be used after the document 
	is saved). 

	<p>This method returns with no action if the new security handler 
	is the same as the old one. Otherwise, it calls the new 
	security handler's PDCryptNewSecurityDataProc() to set the 
	document's <code>newSecurityData</code> field. If the new security handler 
	does not have this callback, the document's <code>newSecurityData</code> 
	field is set to <code>0</code>.</p> 
	@param pdDoc The document whose new security handler is 
	set. 
	@param newCryptHandler The ASAtom for the name of the 
	new security handler to use for the document. This name 
	must be the same as the <code>pdfName</code> used when the security handler 
	was registered using PDRegisterCryptHandler(). Use ASAtomNull 
	to remove security from the document.
	@exception pdErrNoCryptHandler is raised if there is no security handler 
	registered with the specified name and the name is not ASAtomNull. 
	@exception pdErrOpNotPermitted is raised if the document's permissions 
	do not allow its security to be modified. 
	@see PDDocGetNewCryptHandler 
	@see PDDocPermRequest 
	@see PDDocSetNewCryptHandlerEx 
	@see PDRegisterCryptHandler 
	@see PDRegisterCryptHandlerEx 
	@since PI_PDMODEL_VERSION >= 0x00020000
*/
UNPROC(void, PDDocSetNewCryptHandler, (PDDoc pdDoc, ASAtom newCryptHandler))

/**
	Gets the specified document's new security handler (that 
	is, the security handler that will be used after the document 
	is saved). 

	<p>If the document does not have a new security handler, it returns 
	the document's current security handler. </p>
	@param doc The document whose new security handler is 
	obtained.
	@return The ASAtom corresponding to the <code>pdfName</code> of the document's 
	new security handler. It returns ASAtomNull if the document 
	does not have a new security handler. 
	@see PDDocPermRequest 
	@see PDDocSetNewCryptHandler 
	@see PDDocSetNewCryptHandlerEx 
	@see PDRegisterCryptHandler 
	@see PDRegisterCryptHandlerEx 
	@since PI_PDMODEL_VERSION >= 0x00020000
*/
NPROC(ASAtom, PDDocGetNewCryptHandler, (PDDoc doc))

/**
	Gets the security information from the specified document's 
	new security handler. It calls the PDCryptGetSecurityInfoProc() 
	callback of the document's new security handler. No permissions 
	are required to call this method. 

	<p>It raises only those exceptions raised by the new security 
	handler's PDCryptGetSecurityInfoProc() callback. </p>
	@param pdDoc IN/OUT The document whose new security information 
	is obtained. 
	@param secInfo IN/OUT (Filled by the method) The document's new 
	security information. The value must be an <code>OR</code> of the Security 
	Info Flags. It is set to <code>pdInfoCanPrint | pdInfoCanEdit | pdInfoCanCopy 
	| pdInfoCanEditNotes</code> (see PDPerms) if the document's new 
	security handler does not have a PDCryptGetSecurityInfoProc() 
	callback. 
	@see PDRegisterCryptHandler 
	@since PI_PDMODEL_VERSION >= 0x00020000
*/
NPROC(void, PDDocGetNewSecurityInfo, (PDDoc pdDoc, ASUns32 *secInfo))

/**
	Deprecated in Acrobat 5.0. Use PDDocPermRequest() instead. 

	<p>Gets the permissions for the specified document. You can 
	set permissions with PDDocAuthorize(). </p>
	
	@param doc The document whose permissions are obtained.
	@return A bit field indicating the document's permissions. It is 
	an <code>OR</code> of the PDPerms values. 
	@see PDDocAuthorize 
	@see PDDocGetSecurityData 
	@see PDDocPermRequest 
	@since PI_PDMODEL_VERSION >= 0x00020000
*/
NPROC(PDPerms, PDDocGetPermissions, (PDDoc doc))


/**
	Registers a new security handler with the Acrobat viewer. 
	
	@param handler A pointer to a structure that contains the 
	security handler's callback functions. This structure must 
	not be freed after calling PDRegisterCryptHandler(). 
	@param pdfName The name of the security handler as it 
	will appear in the PDF file. This name is also used by PDDocSetNewCryptHandler(). 
	Storage for this name can be freed after PDRegisterCryptHandler() 
	has been called. 
	@param userName The name of the security handler as it 
	will be shown in menus. This name can be localized into 
	different languages. Storage for this name can be freed 
	after PDRegisterCryptHandler() has been called.
	@exception genErrBadParm is raised if the security handler's size field 
	is incorrect. 
	@exception ErrSysPDSEdit is raised if either <code>pdfName</code> or <code>userName</code> are already 
	in use by a registered security handler. 
	@exception genErrNoMemory is raised is memory is exhausted. Other exceptions may be raised as well.
	@see PDDocGetNewCryptHandler 
	@see PDDocPermRequest 
	@see PDDocSetNewCryptHandler 
	@see PDDocSetNewCryptHandlerEx 
	@see PDRegisterCryptHandlerEx 
	@since PI_PDMODEL_VERSION >= 0x00020000
*/
NPROC(void, PDRegisterCryptHandler, (PDCryptHandler handler, const char *pdfName, const char *userName))
#endif

/**
	Translates a string from host encoding to <code>PDFDocEncoding</code>. 
	This method is useful when setting or retrieving displayed 
	text that must be in <code>PDFDocEncoding</code> (or Unicode), such as 
	text that appears in a text annotation or bookmark. 

	<p>A character that cannot be converted to the destination 
	encoding is replaced with a space. For example, PDXlateToPDFDocEnc() 
	converts <code>'\\n'</code> to a space character (<code>'\\r'</code> is present in <code>PDFDocEncoding</code> 
	and is left unchanged). </p>

	<p>Host encoding is a platform-dependent encoding for the host 
	machine. For non-UNIX Roman systems, it is <code>WinAnsiEncoding</code> on Windows and <code>MacRomanEncoding</code> 
	on Mac OS. On UNIX (except HP-UX) Roman systems, it is <code>ISO8859-1</code> (ISO Latin-1); for 
	HP-UX, it is <code>HP-ROMAN8</code>. See Appendix D in the <i>PDF Reference</i> 
	for descriptions of <code>WinAnsiEncoding</code>, <code>MacRomanEncoding</code>, and 
	<code>PDFDocEncoding</code>. </p>

	<p>For non-Roman systems, the host encoding may be a variety 
	of encodings, which are defined by a CMap (character map). 
	See Section 5.6.4 in the <i>PDF Reference</i> for a list of predefined 
	CMaps. Use PDGetHostEncoding() to determine if a system's 
	host encoding is Roman. For non-Roman systems, use 
	PDXlateToPDFDocEncEx(). </p>

	<p>In general, PDXlateToPDFDocEncEx() can be called instead of 
	PDXlateToPDFDocEnc(), since PDXlateToPDFDocEncEx() works for 
	PDFDocEncoding or Unicode. </p>
	@param in The string to translate (it may point to the same 
	memory as <code>out</code>, allowing strings to translate in place). 
	
	@param out (Filled by the method) The translated string 
	(it may point to the same memory as <code>in</code>). 
	@param numBytes The number of bytes in the string to translate.
	@see PDGetHostEncoding 
	@see PDXlateToHost 
	@see PDXlateToHostEx 
	@see PDXlateToPDFDocEncEx 
	@since PI_PDMODEL_VERSION >= 0x00020000
*/
NPROC(void, PDXlateToPDFDocEnc, (char *in, char *out, ASInt32 numBytes))

/**
	Translates a string from <code>PDFDocEncoding</code> to host encoding. 
	This method is useful when setting or retrieving displayed 
	text that must be in <code>PDFDocEncoding</code> (or Unicode), such as 
	text that appears in a text annotation or bookmark. 

	<p>A character that cannot be converted to the destination 
	encoding is replaced with a space. </p>

	<p>Host encoding is a platform-dependent encoding for the host 
	machine. For non-UNIX Roman systems, it is <code>MacRomanEncoding</code> 
	on Mac OS and <code>WinAnsiEncoding</code> on Windows. On UNIX (except 
	HP-UX) Roman systems, it is <code>ISO8859-1</code> (ISO Latin-1); for 
	HP-UX, it is <code>HP-ROMAN8</code>. See Appendix D in the <i>PDF Reference</i> 
	for descriptions of <code>MacRomanEncoding</code>, <code>WinAnsiEncoding</code>, and 
	<code>PDFDocEncoding</code>. </p>

	<p>For non-Roman systems, the host encoding may be a variety 
	of encodings, which are defined by a CMap (character map). 
	See Section 5.6.4 in the <i>PDF Reference</i> for a list of predefined 
	CMaps. </p>

	<p>Use PDGetHostEncoding() to determine if a system's host encoding 
	is Roman. For non-Roman systems, use PDXlateToHostEx().</p> 
	
	<p>In general, PDXlateToHostEx() can be called instead of PDXlateToHost() 
	since PDXlateToHostEx() works for any host encoding.</p>

	@param in The string to translate (it may point to the same 
	memory as <code>out</code>, allowing strings to translate in place). 
	
	@param out (Filled by the method) The translated string 
	(it may point to the same memory as <code>in</code>). 
	@param numBytes The number of bytes in the string to translate.
	@see PDGetHostEncoding 
	@see PDXlateToHostEx 
	@see PDXlateToPDFDocEnc 
	@see PDXlateToPDFDocEncEx 
	@since PI_PDMODEL_VERSION >= 0x00020000
*/
NPROC(void, PDXlateToHost, (char *in, char *out, ASInt32 numBytes))


/**
	Gets the name of a font. The behavior depends on the font 
	type; for a Type 3 font it gets the value of the Name key 
	in a PDF Font resource. See Section 5.5.4 in the <i>PDF Reference</i>. 
	For other types it gets the value of the BaseFont key in 
	a PDF font resource. 
	@param font IN/OUT The font whose name is obtained. 
	@param buffer IN/OUT (Filled by the method) The buffer into which 
	the font's name is stored. The client may pass in <code>NULL</code> to 
	obtain the buffer size, excluding the terminating <code>NULL</code>,
	and then call the method with a buffer of the appropriate size. 
	You must pass at least the <code>bufSize + 1</code> as the buffer size 
	@param bufSize IN/OUT The length of <code>buffer</code> in bytes. The maximum 
	font name length that the Acrobat viewer will return is 
	<code>PSNAMESIZE</code> (see PDExpT. h). 
	@return The number of characters in the font name. If the font name 
	is too long to fit into <code>buffer</code>, <code>bufSize - 1</code> bytes are copied 
	into <code>buffer</code>, and <code>buffer</code> is <code>NULL</code>-terminated. 
	@see PDDocEnumFonts 
	@see PDFontGetEncodingName 
	@since PI_PDMODEL_VERSION >= 0x00020000
*/
NPROC(ASInt32, PDFontGetName, (PDFont font, char *buffer, ASInt32 bufSize))

/**
	Gets a font's subtype. 
	@param font IN/OUT The font whose subtype is obtained. 
	@return The font's subtype. The ASAtom returned can be converted 
	to a string using ASAtomGetString(). It must be one of the Font Subtypes. 
	@see PDDocEnumFonts 
  	@ref FontSubtypes
	@since PI_PDMODEL_VERSION >= 0x00020000
*/
NPROC(ASAtom, PDFontGetSubtype, (PDFont font))

/**
	Gets the font's character set. This is derived from the 
	'Uses Adobe standard encoding' bit in the font descriptor 
	(if the font has a font descriptor) or from the font's name 
	(if the font is one of the base 14 fonts and does not have 
	a font descriptor). 

	<p>For non-Roman character set viewers, call PDFontGetEncodingName() 
	instead. </p>
	@param font IN/OUT The font whose character set is obtained. 
	@return The font's character set. For non-Roman character set viewers, 
	it returns PDUnknownCharSet. 
	@see PDFontGetEncodingName 
	@since PI_PDMODEL_VERSION >= 0x00020000
*/
NPROC(PDCharSet, PDFontGetCharSet, (PDFont font))

/**
	Gets a font's encoding index. 

	<p>For non-Roman character set viewers, call PDFontGetEncodingName() instead. </p>
	
	@param font IN/OUT The font whose encoding index is obtained. 
	
	@return A font encoding index. If the index is greater than PDLastKnownEncoding, 
	it is a custom encoding, and is unique within the document. 
	If the index is less than PDLastKnownEncoding, it must be 
	one of the PDFontEncoding values. 
	@see PDFontAcquireEncodingArray 
	@since PI_PDMODEL_VERSION >= 0x00020000
*/
NPROC(ASInt32, PDFontGetEncodingIndex, (PDFont font))

/**
	Acquires a font's encoding array (the mapping of character 
	codes to glyphs). When you are done with this array, call PDFontEncodingArrayRelease() 
	to release it. 

	<p>The array contains 256 pointers. If a pointer is not <code>NULL</code>, 
	it points to a C string containing the name of the glyph 
	for the code point corresponding to the index. If it is 
	<code>NULL</code>, then the name of the glyph is unchanged from that 
	specified by the font's built-in encoding. </p>

	<p>For a Type 3 font, all glyph names will be present in the 
	encoding array, and <code>NULL</code> entries correspond to un-encoded 
	code points. </p>

	<p>For non-Roman character set viewers, it is not appropriate 
	to call this method. </p>
	@param font The font whose encoding array is obtained.
	@return The font's encoding array. It returns <code>NULL</code> if there is no encoding 
	array associated with the font. 
	@see PDFontEncodingArrayRelease 
	@see PDFontGetEncodingIndex 
	@since PI_PDMODEL_VERSION >= 0x00020000
*/
NPROC(ASUns8 **, PDFontAcquireEncodingArray, (PDFont font))

/**
	Releases a font's encoding array (the mapping of character 
	codes to glyphs). Call this method after you are done using 
	an encoding array acquired using PDFontAcquireEncodingArray(). 
	
	@param array IN/OUT The encoding array to release. 
	@see PDFontAcquireEncodingArray 
	@since PI_PDMODEL_VERSION >= 0x00020000
*/
NPROC(void, PDFontEncodingArrayRelease, (ASUns8 **array))

/**
	Gets a font's metrics, which provide the information needed 
	to create a substitute Multiple Master font when the original 
	font is unavailable. See Section 5.7 in the <i>PDF Reference</i> 
	for a discussion of font descriptors. 
	@param font IN/OUT The font whose metrics are being obtained. 
	
	@param metricsP IN/OUT (Filled by the method) A pointer to a PDFontMetrics 
	structure containing the font's metrics. The font metric 
	values may be patched before being returned. If the actual 
	values in the PDF file are required, use Cos instead to 
	get trustworthy metrics. 
	@param sizeMetrics IN/OUT It must be <code>sizeof(PDFontMetrics)</code>. 
	@see PDFontGetWidths 
	@see PDFontSetMetrics 
	@since PI_PDMODEL_VERSION >= 0x00020000
*/
NPROC(void, PDFontGetMetrics, (PDFont font, PDFontMetricsP metricsP, os_size_t sizeMetrics))

/**
	Gets a Type 3 font's bounding box, which is the smallest 
	rectangle that would enclose every character in the font 
	if they were overlaid and painted. 
	@param font IN/OUT The font whose bounding box is obtained. 
	@param bboxP IN/OUT (Filled by the method) A pointer to a rectangle 
	specifying the font's bounding box. 
	@since PI_PDMODEL_VERSION >= 0x00020000
*/
NPROC(void, PDFontGetBBox, (PDFont font, ASFixedRect *bboxP))

/**
	Gets the advance width of every glyph in a font. The advance 
	width is the amount by which the current point advances 
	when the glyph is drawn. The advance width may not correspond 
	to the visible width of the glyph (for example, a glyph 
	representing an accent mark might have an advance width 
	of zero so that characters can be drawn under it). For this 
	reason, the advance width cannot be used to determine the 
	glyphs' bounding boxes. 

	<p>For non-Roman character set viewers, this method gets the 
	width for a single byte range (<code>0</code> through <code>255</code>). </p>
	@param font IN/OUT The font whose glyph advance widths are obtained. 
	
	@param widths IN/OUT (Filled by the method) An array of glyph 
	advance widths, measured in character space units. Un-encoded 
	code points will have a width of zero. For non-Roman character 
	set viewers, an array for a single byte range (<code>0</code> through 
	<code>255</code>). 
	@see PDFontGetMetrics 
	@see PDFontSetMetrics 
	@see PDFontXlateWidths 
	@since PI_PDMODEL_VERSION >= 0x00020000
*/
NPROC(void, PDFontGetWidths, (PDFont font, ASInt16 *widths))

/**
	Gets an array describing the differences between the platform's 
	host encoding and <code>PDFDocEncoding</code>. 

	<p>Host encoding is a platform-dependent encoding for the host 
	machine. For non-UNIX Roman systems, it is <code>WinAnsiEncoding</code> on Windows and <code>MacRomanEncoding</code> 
	on Mac OS. On UNIX (except HP-UX) Roman systems, it is <code>ISO8859-1</code> (ISO Latin-1); for 
	HP-UX, it is <code>HP-ROMAN8</code>. See Appendix D in the <i>PDF Reference</i> 
	for descriptions of <code>WinAnsiEncoding</code>, <code>MacRomanEncoding</code>, and 
	<code>PDFDocEncoding</code>. </p>

	<p>For non-Roman systems, the host encoding may be a variety 
	of encodings, which are defined by a CMap (character map). 
	See Section 5.6.4 in the <i>PDF Reference</i> for a list of predefined 
	CMaps. </p>

	<p>Use PDGetHostEncoding() to determine whether a system's host encoding 
	is Roman. </p>
	@return An array containing 256 elements. Each element corresponds 
	to a code point in the <code>PDFDocEncoding</code>, and is either <code>NULL</code> 
	or a pointer to a string. 

	<p>If the element is <code>NULL</code>, the code point refers to the same 
	glyph in both host encoding and <code>PDFDocEncoding</code>. If the element 
	is non-<code>NULL</code>, it points to a string containing the glyph 
	name for the code point. </p>
	@see PDGetHostEncoding 
	@see AVAppGetLanguageEncoding 
	@since PI_PDMODEL_VERSION >= 0x00020000
*/
NPROC(ASUns8 **, PDGetPDFDocEncoding, (void))

/**
	Tests whether the specified font is embedded in the PDF 
	file, meaning that the font is stored as a font file, which 
	is a stream embedded in the PDF file. Only Type 1 and TrueType 
	fonts can be embedded. 
	@param font The font to test.
	@return <code>true</code> if the font is embedded in the file, <code>false</code> 
	otherwise. 
	@see PDDocEnumFonts 
	@since PI_PDMODEL_VERSION >= 0x00020000
*/
NPROC(ASBool, PDFontIsEmbedded, (PDFont font))


/**
	Translates an array of 256 glyph advance widths (obtained 
	from PDFontGetWidths()) from their order in the PDF file into 
	host encoding order. If the widths are already in host encoding 
	order, the widths are merely copied. All un-encoded code 
	points are given a width of zero. 

	<p>For non-Roman character set viewers, it is not appropriate 
	to call this method. </p>
	@param font IN/OUT The font whose glyph widths are translated. 
	
	@param inP IN/OUT The array of glyph advance widths to rearrange. 
	
	@param outP IN/OUT (Filled by the method) The rearranged array of 
	glyph advance widths. 
	@see PDFontGetWidths 
	@since PI_PDMODEL_VERSION >= 0x00020000
	
*/
NPROC(void, PDFontXlateWidths, (PDFont font, ASInt16 *inP, ASInt16 *outP))

/**
	Translates a string from the PDFont's encoding into host 
	encoding. If any characters cannot be represented in host 
	encoding, they are replaced with space characters. If no 
	XlateTable exists in the font, the function returns <code>false</code> 
	and <code>outP</code> is not written. 

	<p>For non-Roman character set viewers, it is not appropriate 
	to call this method. Instead call one of the following: 
	PDFontXlateToHost(), PDFontXlateToUCS(), PDXlateToHostEx(), or 
	PDXlateToPDFDocEncEx(). </p>
	@param font The font (and hence, the encoding) that <code>inP</code> uses. 
	
	@param inP The string to translate. 
	@param outP (Filled by the method) The translated string. 
	<code>outP</code> may point to the same buffer as <code>inP</code>, to allow in-place 
	translation. 
	@param len The length of <code>inP</code> and <code>outP</code>.
	@return <code>true</code> if an XlateTable exists in the font, <code>false</code> 
	otherwise. If no XlateTable exists in the font, <code>outP</code> is 
	not written. 
	@see PDFontXlateToHost 
	@see PDFontAcquireXlateTable 
	@see PDFontXlateToUCS 
	@see PDXlateToHostEx 
	@see PDXlateToPDFDocEncEx 
	@since PI_PDMODEL_VERSION >= 0x00020000
*/
NPROC(ASBool, PDFontXlateString, (PDFont font, ASUns8 *inP, ASUns8 *outP, ASInt32 len))

/**
	Increments the specified font's XlateTable reference count 
	and also returns the XlateTable, which is a 256-entry table 
	that maps characters from their encoding in the PDF file 
	to host encoding. If a character cannot be mapped to host 
	encoding, then the table entry will (for that character) 
	contain <code>-1</code>. When you are done using the XlateTable, call 
	PDFontXlateTableRelease() to release it. 

	<p>For non-Roman character set viewers, it is not appropriate 
	to call this method. </p>
	@param font IN/OUT The font whose XlateTable is obtained. 
	@return A pointer to the font's XlateTable, if any. Otherwise it returns <code>NULL</code>. 
	
	@see PDFontXlateTableRelease 
	@see PDFontXlateString 
	@since PI_PDMODEL_VERSION >= 0x00020000
	
*/
NPROC(ASInt16 *, PDFontAcquireXlateTable, (PDFont font))

/**
	Decrements the specified font's XlateTable reference count. 
	The XlateTable is a 256-entry table that maps characters 
	from their encoding in the PDF file to host encoding. If 
	a character cannot be mapped to host encoding, then the 
	table entry will (for that character) contain <code>-1</code>. 
	@param table IN/OUT The XlateTable to release. 
	@see PDFontAcquireXlateTable 
	@see PDFontXlateString 
	@since PI_PDMODEL_VERSION >= 0x00020000
	
*/
NPROC(void, PDFontXlateTableRelease, (ASInt16 *table))


/**
	Gets a font's matrix, which specifies the transformation 
	from character space to text space. See Section 5.5.4 in 
	the <i>PDF Reference</i>. This is only valid for Type 3 fonts. 
	
	@param fontP IN/OUT The font whose matrix is obtained. 
	@param matrixP IN/OUT (Filled by the method) A pointer to the font's 
	matrix. 
	@see PDDocEnumFonts 
	@since PI_PDMODEL_VERSION >= 0x00020000
	
*/
ENPROC(void, PDFontGetFontMatrix, (PDFont fontP, ASFixedMatrix *matrixP))

/**
	Sets a font's metrics, which provide the information needed 
	to create a substitute Multiple Master font when the original 
	font is unavailable. See Section 5.7 in the <i>PDF Reference</i> 
	for a discussion of font descriptors. This method can only 
	be used on Type 1, Multiple Master Type 1, and TrueType 
	fonts; it cannot be used on Type 3 fonts. 
	@param font IN/OUT The font whose metrics are being set. 
	@param metricsP IN/OUT A pointer to a PDFontMetrics structure containing 
	the font's metrics. 
	@param sizeMetrics IN/OUT It must be <code>sizeof(PDFontMetrics)</code>. 
	@see PDFontGetMetrics 
	@since PI_PDMODEL_VERSION >= 0x00020000
	
*/
UNPROC(void, PDFontSetMetrics, (PDFont font, PDFontMetricsP metricsP, os_size_t sizeMetrics))

/**
	Gets the Cos object for a font. This method does not copy 
	the object, but is instead the logical equivalent of a type 
	cast. 
	@param font IN/OUT The font whose Cos object is obtained. 
	@return The dictionary Cos object for the font. The dictionary's 
	contents may be enumerated with CosObjEnum(). 
	@see PDDocEnumFonts 
	@since PI_PDMODEL_VERSION >= 0x00020000
	
*/
NPROC(CosObj, PDFontGetCosObj, (PDFont font))


/**
	Broadcasts a PDPageContentsDidChange() notification. If the 
	Acrobat viewer is version 2.1 or later, also broadcasts 
	a PDPageContentsDidChangeEx() notification with <code>invalidateViews</code> 
	set to <code>true</code>. 

	<p>You must use this method after using Cos methods to change 
	a page's contents. Do not use this method if you use PDPageAddCosContents() 
	or PDPageRemoveCosContents() to change a page's contents, 
	because those methods automatically generate the appropriate 
	notifications. </p>

	<p>Use PDPageNotifyContentsDidChangeEx() instead of this method 
	if you wish to suppress the Acrobat viewer's immediate redraw 
	of the page. </p>
	@param page IN/OUT The page that changed. 
	@notify PDPageContentsDidChange 
	@notify PDPageContentsDidChangeEx (in version 2.1 and later) 
	@see PDPageAddCosContents 
	@see PDPageRemoveCosContents 
	@see PDPageNotifyContentsDidChangeEx 
	@since PI_PDMODEL_VERSION >= 0x00020000
	
*/
NPROC(void, PDPageNotifyContentsDidChange, (PDPage page))


/**
	Gets the page number for the specified page. 
	@param page IN/OUT The page whose page number is obtained. 
	@return The page within the document. The first page is <code>0</code>. 
	@see PDPageNumFromCosObj 
	@since PI_PDMODEL_VERSION >= 0x00020000
	
*/
NPROC(ASInt32, PDPageGetNumber, (PDPage page))

/**
	Decrements the specified page's reference count. 
	@param page IN/OUT The page whose reference count is decremented. 
	
	@see PDBeadAcquirePage 
	@see PDDocAcquirePage 
	@since PI_PDMODEL_VERSION >= 0x00020000
	
*/
NPROC(void, PDPageRelease, (PDPage page))

/**
	Gets the document that contains the specified page. 
	@param page IN/OUT The page whose document is obtained. 
	@return The document that contains the page. 
	@see PDDocAcquirePage 
	@since PI_PDMODEL_VERSION >= 0x00020000
	
*/
NPROC(PDDoc, PDPageGetDoc, (PDPage page))

/**
	Gets the dictionary Cos object associated with a page. This 
	method does not copy the object, but is instead the logical 
	equivalent of a type cast. 
	@param page IN/OUT The page whose Cos object is obtained. 
	@return The dictionary Cos object associated with page. 
	@since PI_PDMODEL_VERSION >= 0x00020000
	
*/
NPROC(CosObj, PDPageGetCosObj, (PDPage page))

/**
	Gets the page number of the page specified by a Cos object. 
	
	@param pageObj IN/OUT The dictionary Cos object for the page whose 
	number is obtained. 
	@return The page within the document (the first page in a document is page number zero).  
	@note Do not call this method with a <code>NULL</code> Cos object.
	@see PDPageGetNumber 
	@since PI_PDMODEL_VERSION >= 0x00020000
	
*/
NPROC(ASInt32, PDPageNumFromCosObj, (CosObj pageObj))


/**
	Gets the rotation value for a page. 
	@param page IN/OUT The page whose rotation is obtained. 
	@return Rotation value for the given page. It must be one of the PDRotate 
	values. 
	@see PDPageSetRotate 
	@since PI_PDMODEL_VERSION >= 0x00020000
	
*/
NPROC(PDRotate, PDPageGetRotate, (PDPage page))

/**
	Sets the rotation value for a page. 
	@param page The page whose rotation is set.
	@param angle Rotation value to be set for a given page.  It must be one of the
	PDRotate values.
	@notify PDDocWillChangePages 
	@notify PDDocDidChangePages 
	@see PDPageGetRotate 
	@since PI_PDMODEL_VERSION >= 0x00020000
	
*/
UNPROC(void, PDPageSetRotate, (PDPage page, PDRotate angle))

/**
	Gets the media box for a page. The media box is the <i>natural 
	size</i> of the page (for example, the dimensions of an A4 
	sheet of paper). 
	@param page IN/OUT The page whose media box is obtained. 
	@param mediaBoxP IN/OUT (Filled by the method) A pointer to a rectangle 
	specifying the page's media box, specified in user space 
	coordinates. 
	@see PDPageSetMediaBox 
	@see PDPageGetCropBox 
	@see PDPageGetBBox 
	@since PI_PDMODEL_VERSION >= 0x00020000
	
*/
NPROC(void, PDPageGetMediaBox, (PDPage page, ASFixedRect *mediaBoxP))

/**
	Sets the media box for a page. The media box is the <i>natural 
	size</i> of the page, for example, the dimensions of an A4 
	sheet of paper. 
	@param page IN/OUT The page whose media box is set. 
	@param mediaBox IN/OUT Rectangle specifying the page's media box, 
	specified in user space coordinates. 
	@notify PDDocWillChangePages 
	@notify PDDocDidChangePages 
	@see PDPageGetMediaBox 
	@see PDPageSetCropBox 
	@see PDPageGetBBox 
	@since PI_PDMODEL_VERSION >= 0x00020000
	
*/
UNPROC(void, PDPageSetMediaBox, (PDPage page, ASFixedRect mediaBox))

/**
	Gets the crop box for a page. The crop box is the region 
	of the page to display and print. 
	@param page The page whose crop box is obtained. 
	@param cropBoxP (Filled by the method) A pointer to a rectangle 
	specifying the page's crop box, specified in user space 
	coordinates.
	@see PDPageSetCropBox 
	@see PDPageGetMediaBox 
	@see PDPageGetBBox 
	@since PI_PDMODEL_VERSION >= 0x00020000
*/
NPROC(void, PDPageGetCropBox, (PDPage page, ASFixedRect *cropBoxP))

/**
	Sets the crop box for a page. The crop box is the region 
	of the page to display and print. This method ignores the 
	request if either the width or height of cropBox is less 
	than 72 points (one inch). 
	@param page The page whose crop box is set. 
	@param cropBox A rectangle specifying the page's crop box, 
	specified in user space coordinates.
	@notify PDDocWillChangePages 
	@notify PDDocDidChangePages 
	@see PDPageGetCropBox 
	@see PDPageSetMediaBox 
	@since PI_PDMODEL_VERSION >= 0x00020000
*/
UNPROC(void, PDPageSetCropBox, (PDPage page, ASFixedRect cropBox))

/**
	Gets the bounding box for a page. The bounding box is the 
	rectangle that encloses all text, graphics, and images on 
	the page. 
	@param page The page whose bounding box is obtained. 
	@param bboxP (Filled by the method) A pointer to a rectangle 
	specifying the page's bounding box, specified in user space 
	coordinates.
	@see PDPageGetMediaBox 
	@see PDPageGetCropBox 
	@see PDPageGetVisibleBBox 
	@since PI_PDMODEL_VERSION >= 0x00020000
*/
NPROC(void, PDPageGetBBox, (PDPage page, ASFixedRect *bboxP))

/**
	Gets the matrix that transforms user space coordinates to 
	rotated and cropped coordinates. The origin of this space 
	is the bottom-left of the rotated, cropped page. <code>Y</code> is increasing. 
	
	@param pdPage The page whose default transformation matrix 
	is obtained. 
	@param defaultMatrix (Filled by the method) A pointer to 
	the default transformation matrix. 
	@see PDPageGetFlippedMatrix 
	@since PI_PDMODEL_VERSION >= 0x00020000
	
*/
NPROC(void, PDPageGetDefaultMatrix, (PDPage pdPage, ASFixedMatrix *defaultMatrix))

/**
	Gets the matrix that transforms user space coordinates to 
	rotated and cropped coordinates. The origin of this space 
	is the top-left of the rotated, cropped page. <code>Y</code> is decreasing. 
	
	@param pdPage The page whose flipped transformation matrix 
	is obtained. 
	@param flipped (Filled by the method) A pointer to the flipped 
	transformation matrix. 
	@see PDPageGetDefaultMatrix 
	@since PI_PDMODEL_VERSION >= 0x00020000
	
*/
NPROC(void, PDPageGetFlippedMatrix, (PDPage pdPage, ASFixedMatrix *flipped))

#if !MAC_PLATFORM || (MAC_PLATFORM && !AS_ARCH_64BIT)
/**
	Draws the contents of a page into the specified window. 
	
	<p>This method just draws a bitmap to the window. If you want 
	a live document, you need to open an AVDoc for the PDF file. </p>
	
	<p>The page can also be derived from a PDDoc. </p>

	<p>On UNIX, this method cannot be used to draw into a window. 
	UNIX developers should instead use AVDocOpenFromASFileWithParamString() 
	to draw PDF files into their own window from a client. </p>
	@param page The page to draw into <code>window</code>. 
	@param window A pointer to a platform-dependent window object 
	(<code>HWND</code> on Windows, or <code>WindowPtr</code> or <code>CWindowPtr</code> on Mac OS). 
	On Windows, to draw into an offscreen <code>DC</code>, pass <code>NULL</code> for <code>window</code>.
	On Mac 32, to draw into an offscreen <code>GWorld</code>, pass <code>NULL</code> in 
	<code>window</code> and pass the <code>GWorldPtr</code> in <code>displayContext</code>.  
	@param displayContext A platform-dependent display context 
	structure (<code>HDC</code> on Windows, <code>GWorldPtr</code> on Mac OS). On Mac OS, 
	<code>displayContext</code> is ignored if <code>window</code> is non-<code>NULL</code>. Note that 
	<code>displayContext</code> cannot be reliably used as the <code>hDC</code> for a 
	printer device. This API is not available on Mac 64. 
	@param isDPS Currently unused. Always set it to <code>false</code>. 
	@param matrix A pointer to the matrix to concatenate onto 
	the default page matrix. It is useful for converting from 
	page to window coordinates and for scaling.
	@param updateRect A pointer to the rectangle to draw, defined 
	in user space coordinates. Any objects outside of <code>updateRect</code> 
	will not be drawn. All objects are drawn if <code>updateRect</code> is 
	<code>NULL</code>. 
	@param cancelProc The procedure called periodically to check 
	for the user's cancelling of the drawing operation. The default cancel 
	proc can be obtained using AVAppGetCancelProc(). It may be <code>NULL</code>, 
	in which case no cancel proc is used. 
	@param cancelProcClientData A pointer to user-supplied data 
	to pass to <code>cancelProc</code> each time it is called. It should be 
	<code>NULL</code> if <code>cancelProc</code> is <code>NULL</code>.
	@see AVAppGetCancelProc 
	@see PDDrawCosObjToWindow 
	@see PDPageDrawContentsToWindowEx 

	@note This method cannot be reliably used to print to a 
	device. 
	@since PI_PDMODEL_VERSION >= 0x00020000
*/
NPROC(void, PDPageDrawContentsToWindow, (PDPage page, void *window, void *displayContext,
				ASBool isDPS, ASFixedMatrix *matrix, ASFixedRect *updateRect, CancelProc cancelProc, void *cancelProcClientData))
#endif

/**
	Gets the <code>annotIndex</code> annotation on the page. 
	@param aPage IN/OUT The page on which the annotation is located. 
	
	@param annotIndex IN/OUT The index of the annotation to get on 
	a page. The first annotation on a page has an index of zero. 
	
	@return The indexed annotation object. 
	@see PDPageGetNumAnnots 
	@since PI_PDMODEL_VERSION >= 0x00020000
	
*/
NPROC(PDAnnot, PDPageGetAnnot, (PDPage aPage, ASInt32 annotIndex))

/**
	Adds an annotation to the page. To make the annotation visible 
	after adding it, convert the coordinates of <code>initialRect</code> 
	to device coordinates using AVPageViewRectToDevice(), then 
	call AVPageViewInvalidateRect() using the converted rectangle. 
	This method is equivalent to calling PDPageCreateAnnot() followed 
	by PDPageAddAnnot(). 

	<p>The PDPageWillAddAnnot() and PDPageDidAddAnnot() notifications 
	are broadcast before the PDPageAddNewAnnot() method returns. 
	If you want to finish formatting the annotation before these 
	notifications are called, for example, by adding additional 
	key-value pairs to the annotation dictionary, you should 
	call PDPageCreateAnnot() followed by PDPageAddAnnot() instead 
	of PDPageAddNewAnnot(). </p>
	@param aPage The page to which the annotation is added.
	@param addAfter Where to add the annotation in the page's 
	annotation array. See Section 8.4 in the <i>PDF Reference</i> for 
	a description of the annotation array. Passing a value of 
	<code>-2</code> adds the annotation to the end of the array (this is 
	generally what you should do unless you have a need to place 
	the annotation at a special location in the array). Passing 
	a value of <code>-1</code> adds the annotation to the beginning of the 
	array. Passing other negative values produces undefined 
	results. 
	@param subType The subtype of the annotation to add. 
	@param initialRect A pointer to a rectangle specifying the 
	annotation's bounds, specified in user space coordinates.
	@return The newly created annotation. 
	@exception pdErrOpNotPermitted is raised if:
	
	<ul>
	<li>The annotation is of subtype Text and the document's permissions do not include pdPermEditNotes (see PDPerms). </li>
	<li>The annotation is of any other subtype and the document's permissions do not include pdPermEdit. </li>
	</ul>
	
	@notify PDPageWillAddAnnot 
	@notify PDPageDidAddAnnot 
	@see PDPageCreateAnnot 
	@see PDPageAddAnnot 
	@see PDPageRemoveAnnot 
	@since PI_PDMODEL_VERSION >= 0x00020000
*/
UNPROC(PDAnnot, PDPageAddNewAnnot, (PDPage aPage, ASInt32 addAfter, ASAtom subType, const ASFixedRect *initialRect))

/**
	Adds an annotation at the specified location in a page's 
	annotation array. 
	@param aPage The page to which the annotation is added. 
	
	@param addAfter The index into the page's annotation array 
	where the annotation is added. See Section 8.4 in the PDF 
	Reference for a description of the annotation array. The 
	first annotation in the array has an index of zero. Passing 
	a value of <code>-2</code> adds the annotation to the end of the array. 
	Passing other negative values produces undefined results. 
	
	@param annot The annotation to add.
	@exception pdErrOpNotPermitted is raised if: 
	<ul>
	<li>The annotation is of subtype Text and the document's permissions do not include pdPermEditNotes (see PDPerms). </li>
	<li>The annotation is of any other subtype and the document's permissions do not include pdPermEdit. </li>
	</ul>
	@notify PDPageWillAddAnnot 
	@notify PDPageDidAddAnnot 
	@see PDPageCreateAnnot 
	@see PDPageAddNewAnnot 
	@see PDPageRemoveAnnot 
	@since PI_PDMODEL_VERSION >= 0x00020000
*/
UNPROC(void, PDPageAddAnnot, (PDPage aPage, ASInt32 addAfter, PDAnnot annot))

/**
	Removes an annotation from the specified page. Annotations 
	are stored in Cos arrays, which are automatically compressed 
	when an annotation is removed (see CosArrayRemove()). For 
	this reason, if you use a loop in which you remove annotations, 
	structure the code so the loop processes from the highest 
	to the lowest index. If you loop the other direction, you 
	will skip over annotations immediately following ones you 
	remove. 
	@param aPage The page from which the annotation is removed. 
	
	@param annotIndex The index (into the page's annotation 
	array) of the annotation to remove.
	@exception pdErrOpNotPermitted is raised if:
	<ul>
	<li>The annotation is of subtype Text and the document's permissions 
	do not include pdPermEditNotes (see PDPerms). </li>
	<li>The annotation is of any other subtype and the document's 
	permissions do not include pdPermEdit. </li>
	</ul>
	@notify PDPageWillRemoveAnnot 
	@notify PDPageDidRemoveAnnot 
	@see PDPageGetAnnotIndex 
	@see PDPageAddNewAnnot 
	@see PDPageAddAnnot 
	@since PI_PDMODEL_VERSION >= 0x00020000
*/
UNPROC(void, PDPageRemoveAnnot, (PDPage aPage, ASInt32 annotIndex))

/**
	Gets the index of a given annotation object on a given page. 
	
	@param aPage IN/OUT The page to which the annotation is attached. 
	
	@param anAnnot IN/OUT The annotation whose index is obtained. 
	
	@return The annotation's index. It returns <code>-1</code> if the annotation is 
	not on the page. 
	@see PDPageGetAnnot 
	@see PDPageGetAnnotSequence 
	@since PI_PDMODEL_VERSION >= 0x00020000
	
*/
NPROC(ASInt32, PDPageGetAnnotIndex, (PDPage aPage, PDAnnot anAnnot))

/**
	Gets the number of annotations on a page. Annotations associated 
	with pop-up windows (such as strikeouts) are counted as 
	two annotations. Widget annotations (form fields) are included 
	in the count. 
	@param aPage The page for which the number of annotations 
	is obtained.
	@return The number of annotations on <code>aPage</code>. 
	@see PDPageGetAnnot 
	@since PI_PDMODEL_VERSION >= 0x00020000
*/
NPROC(ASInt32, PDPageGetNumAnnots, (PDPage aPage))


/**
	Gets the Cos object corresponding to a page's resource dictionary. 
	A page's resource Cos object may either be directly in the 
	Page Cos object and apply only to the page. Or, it may be 
	in the Pages tree, be shared by multiple pages, and applies 
	to all Page nodes below the point in the Pages tree where 
	it is located. 
	@param page IN/OUT The page whose Cos resources are obtained. 
	
	@return The dictionary Cos object associated with the page's resource. 
	
	@see PDPageAddCosResource 
	@since PI_PDMODEL_VERSION >= 0x00020000
	
*/
NPROC(CosObj, PDPageGetCosResources, (PDPage page))

/**
	Adds a Cos resource to a page object. See Section 3.7.2 
	in the <i>PDF Reference</i> for a description of page resources. 
	
	<p>The necessary dictionaries are created automatically if 
	the page does not already have any resources of the type 
	specified by <code>resType</code>, or does not have a Resources dictionary. 
	For example, if you specify a font resource, but the page 
	does not already have a font resource dictionary, this method 
	automatically creates one and puts the font you specify 
	into it. </p>

	<p>ProcSet resources cannot be added using this method; they 
	must be added using Cos-level methods to: </p>

	<ul>
	<li>Get the page's Resources dictionary. </li> 
	<li>Get the ProcSet array from the Resources dictionary. </li>
	<li>Add an element to the ProcSet array. </li>
	</ul>

	@param page The page to which a resource is added. 
	@param resType The resource type. The named resource types 
	in PDF are: ExtGState, ColorSpace, Pattern, Shading, XObject, 
	Font, and Properties. Although ProcSet is also a valid resource 
	type, it cannot be added by this method. 
	@param resName The resource name (for example, the name of 
	a font might be <code>F1</code>). 
	@param resObj The Cos object being added as a resource 
	to page.
	@notify PDDocDidChangePages 
	@see PDPageRemoveCosResource 
	@see PDPageGetCosResources 
	@since PI_PDMODEL_VERSION >= 0x00020000
*/
UNPROC(void, PDPageAddCosResource, (PDPage page, const char *resType, const char *resName, CosObj resObj))

/**
	Completely replaces the contents of the specified page with 
	<code>newContents</code>. 
	@param page IN/OUT The page whose Cos contents are replaced. 
	@param newContents IN/OUT A stream Cos object or an array Cos 
	object containing the new contents (stream Cos objects) 
	for <code>page</code>. 
	@notify PDDocDidChangePages 
	@see PDPageRemoveCosContents 
	@since PI_PDMODEL_VERSION >= 0x00020000
	
*/
UNPROC(void, PDPageAddCosContents, (PDPage page, CosObj newContents))

/**
	Removes a Cos resource from a page object. See Section 3.7.2 
	in the <i>PDF Reference</i> for a description of page resources. 
	
	@param page The page whose Cos resources are removed. 
	
	@param resType The resource type. The named 
	resource types in PDF are: ExtGState, ColorSpace, Pattern,
	Shading, XObject, Font, ProcSet, and Properties.
	@param resName The resource name (for example, the name of 
	a font might be <code>F1</code>).
	@notify PDDocDidChangePages 
	@see PDPageAddCosResource 
	@see PDPageGetCosResources 
	@since PI_PDMODEL_VERSION >= 0x00020000
*/
UNPROC(void, PDPageRemoveCosResource, (PDPage page, const char *resType, const char *resName))

/**
	Removes the contents of the specified page. 
	@param page IN/OUT The page whose Cos contents are removed. 
	@notify PDDocDidChangePages 
	@see PDPageAddCosContents 
	@since PI_PDMODEL_VERSION >= 0x00020000
	
*/
UNPROC(void, PDPageRemoveCosContents, (PDPage page))

/* PDPageEnumResources is obsolete as of Acrobat 4.0.  Please use the PDFEdit API. */

/**
	(Obsolete, provided only for backwards compatibility) Enumerates 
	the page's resources, calling an enumeration procedure for 
	each resource. 

	<p>Instead of this method, use PDDocEnumOCGs(). </p>
	
	@param page The page whose Cos resources are enumerated. 
	
	@param mon A pointer to a structure containing user-supplied 
	callbacks that are called for each of the page's resources. 
	Enumeration ends if any procedure returns <code>false</code>. 
	@param clientData A pointer to user-supplied data to pass 
	to each procedure in <code>mon</code> when it is called.
	@see PDDocEnumOCGs 
	@see PDPageGetCosResources 

	@note This method is provided only for backwards compatibility. 
	It has not been updated beyond PDF Version 1.1 and may not 
	work correctly for newly created PDF 1.2 or later files. 
  	@ingroup Enumerators
	@since PI_PDMODEL_VERSION >= 0x00020000
*/
ENPROC(void, PDPageEnumResources, (PDPage page, PDResourceEnumMonitor mon, void *clientData))

/**
	Enumerates the contents of a page, calling a procedure for 
	each drawing object in the page description. 
	
	@param page IN/OUT The page whose contents are enumerated. 
	@param mon IN/OUT A pointer to a structure containing user-supplied 
	callbacks that are called for each drawing operator on a 
	page. Enumeration ends if any procedure returns <code>false</code>. 
	@param clientData IN/OUT A pointer to user-supplied data to pass 
	to <code>mon</code> each time it is called. 

	@note This method is provided only for backwards compatibility. 
	It has not been updated beyond PDF Version 1.1 and may not 
	work correctly for newly created PDF 1.2 or later files. 
	You should use the PDFEdit API to enumerate page contents. 
  	@ingroup Enumerators
	@since PI_PDMODEL_VERSION >= 0x00020000
*/
ENPROC(void, PDPageEnumContents, (PDPage page, PDGraphicEnumMonitor mon, void *clientData))


/**
	Gets a bounding box for the specified graphic object. 
	@param obj The graphic object whose bounding box is obtained. 
	
	@param bboxP (Filled by the method) A pointer to a rectangle 
	containing the bounding box for <code>obj</code>. If it is called during PDFormEnumPaintProc() 
	or PDCharProcEnum(), the coordinates are specified in the 
	object space, meaning that they are relative to the object's matrix.
	@see PDCharProcEnum 
	@see PDFormEnumPaintProc 
	@since PI_PDMODEL_VERSION >= 0x00020000
*/
ENPROC(void, PDGraphicGetBBox, (PDGraphic obj, ASFixedRect *bboxP))

/**
	Gets the current transformation matrix in effect for a graphic 
	object; the matrix is relative to user space. 
	@param obj IN/OUT The graphic object for which transformation 
	matrix is obtained. 
	@param matrix IN/OUT (Filled by the method) A pointer to a matrix 
	containing the transformation matrix for <code>obj</code>. 
	@since PI_PDMODEL_VERSION >= 0x00020000
	
*/
ENPROC(void, PDGraphicGetCurrentMatrix, (PDGraphic obj, ASFixedMatrix *matrix))

/**
	Gets the graphics state associated with a graphic object. 
	See Section 4.3 in the <i>PDF Reference</i> for a discussion of 
	the graphics state parameters. 
	@param obj IN/OUT The graphic object whose graphics state is obtained. 
	
	@param stateP IN/OUT (Filled by the method) A pointer to a PDGraphicState 
	structure containing the graphics state. 
	@param stateLen IN/OUT It must be <code>sizeof(PDGraphicsState)</code>. 
	@since PI_PDMODEL_VERSION >= 0x00020000
	
*/
ENPROC(void, PDGraphicGetState, (PDGraphic obj, PDGraphicStateP stateP, ASInt32 stateLen))

/**
	Enumerates the strings of a text object, calling a procedure 
	for each string. The PDText object may be obtained from 
	the PDGraphicEnumTextProc() callback of PDGraphicEnumMonitor. 
	
	@param text IN/OUT The text object whose strings are enumerated. 
	
	@param enumProc IN/OUT A user-supplied callback to call for each 
	text string in the text object. Enumeration ends if <code>enumProc</code> 
	returns <code>false</code>. 
	@param clientData IN/OUT A pointer to user-supplied data to pass 
	to <code>enumProc</code> each time it is called. 
	@since PI_PDMODEL_VERSION >= 0x00020000
	
*/
ENPROC(void, PDTextEnum, (PDText text, PDStringEnumProc enumProc, void *clientData))

/**
	Gets the text state for a text object. See Section 5.2 in 
	the <i>PDF Reference</i> for a discussion of the text state parameters. 
	
	@param obj IN/OUT The text object whose text state is obtained. 
	
	@param stateP IN/OUT (Filled by the method) A pointer to a PDTextState 
	structure containing the text state information. 
	@param stateLen IN/OUT It must be <code>sizeof(PDTextState)</code>. 
	@since PI_PDMODEL_VERSION >= 0x00020000
	
*/
ENPROC(void, PDTextGetState, (PDText obj, PDTextStateP stateP, ASInt32 stateLen))

/**
	Enumerates the specified path's operators, calling one of 
	several user-supplied callbacks for each operator. The callback 
	that is called depends on which operator is encountered. 
	
	@param obj IN/OUT The path whose operators are enumerated. 
	@param mon IN/OUT A pointer to a structure that contains callbacks. 
	One of the callbacks will be called for each path segment 
	operator in the path. Enumeration ends if any of the monitor's 
	callbacks returns <code>false</code>. 
	@param clientData IN/OUT A pointer to user-supplied data to pass 
	to the monitor's callbacks each time one is called. 
  	@ingroup Enumerators
	@since PI_PDMODEL_VERSION >= 0x00020000
	
*/
ENPROC(void, PDPathEnum, (PDPath obj, PDPathEnumMonitor mon, void *clientData))

/**
	Gets flags that indicate which paint/close/clip operators 
	are used for the specified path. For a description of the 
	path painting operators, see Section 4.4.2 in the <i>PDF Reference</i>. 
	
	@param obj IN/OUT The path whose painting operators are obtained. 
	
	@return A bit-wise <code>OR</code> of the PDPathPaintOp flags. 
	@since PI_PDMODEL_VERSION >= 0x00020000
	
*/
ENPROC(ASInt32, PDPathGetPaintOp, (PDPath obj))

/**
	Gets an inline image's attributes. 
	@param obj IN/OUT The inline image whose attributes are obtained. 
	
	@param attrsP IN/OUT (Filled by the method) A pointer to a PDImageAttrs 
	structure containing the image attributes. Note that this structure 
	contains a Cos object that is subject to the warning below. 
	
	@param attrsLen IN/OUT It must be <code>sizeof(PDImageAttrs)</code>. 
	@see PDImageGetAttrs 
	@see PDInlineImageGetData 

	@note This method is provided only for backwards compatibility. 
	It has not been updated beyond PDF Version 1.1 and may not 
	work correctly for newly created PDF 1.2 or later files. 
	You should use the PDFEdit API to enumerate page contents. 
	
	@note The attribute for a color space is a CosObj. Cos objects 
	that are the result of parsing inline dictionaries in the 
	PDF page contents are a special class of Cos objects. You 
	should never depend on these objects lasting the lifetime 
	of the document. You should extract the information you 
	need from the object immediately and refer to it no further 
	in your code. 

	@since PI_PDMODEL_VERSION >= 0x00020000
	
*/
ENPROC(void, PDInlineImageGetAttrs, (PDInlineImage obj, PDImageAttrsP attrsP, ASInt32 attrsLen))

/**
	Gets the image data for an inline image. 
	@param obj IN/OUT The inline image whose data is obtained. 
	@param data IN/OUT (Filled by the method) A buffer into which 
	the image data will be placed. 
	@param dataLen IN/OUT The number of bytes that <code>data</code> can hold. It must 
	be large enough to hold the entire inline image. Use PDInlineImageGetAttrs() 
	to determine how much data is in the image. 
	@exception genErrBadParm is raised if <code>dataLen</code> is less than the amount of data in the image. 
	@see PDInlineImageGetAttrs 
	@since PI_PDMODEL_VERSION >= 0x00020000
	
*/
ENPROC(void, PDInlineImageGetData, (PDInlineImage obj, ASUns8 *data, ASInt32 dataLen))

/**
	Gets the lookup table for an indexed color space. The table 
	will contain the number of entries specified by the index 
	size, and there will be 1 byte for each color component 
	for each entry. The number of color components depends on 
	the color space: 
	
	<TABLE rules="all" cellspacing="1">
	<TR><TH>Color</TH><TH>Number of components</TH></TR>
	<TR><TD>gray</TD><TD>1</TD></TR>
	<TR><TD>RGB</TD><TD>3</TD></TR>
	<TR><TD>CMYK</TD><TD>4</TD></TR>
	<TR><TD>Lab</TD><TD>3</TD></TR>
	</TABLE>
	
	<p>For additional information on indexed color space, see Section 4.5.5 in 
	the <i>PDF Reference</i>. There is also some useful discussion 
	in the <i>PostScript Language Reference Manual</i> 
	under indexed color spaces. </p>
	@param image IN/OUT The inline image whose lookup table is obtained.
	@param data IN/OUT (Filled by the method) The lookup table for 
	image. 
	@param dataLen IN/OUT The length of <code>data</code> in bytes. 
	@see PDImageColorSpaceGetIndexLookup 
	@since PI_PDMODEL_VERSION >= 0x00020000
	
*/
ENPROC(void, PDInlineImageColorSpaceGetIndexLookup, (PDInlineImage image, ASUns8 *data, ASInt32 dataLen))

/**
	(Obsolete, provided only for backwards compatibility) Gets 
	the subtype of an XObject. Examples of a subtype are Image 
	and Form. 
	@param xObj The XObject whose subtype is obtained.
	@return The ASAtom corresponding to the XObject's subtype. It can be 
	converted into a string using ASAtomGetString(). 
	@see PDXObjectGetData 
	@since PI_PDMODEL_VERSION >= 0x00020000
*/
ENPROC(ASAtom, PDXObjectGetSubtype, (PDXObject xObj))

/**
	(Obsolete, provided only for backwards compatibility) Gets 
	the Cos object associated with an XObject. This method does 
	not copy the object, but is instead the logical equivalent 
	of a type cast. 
	@param xObj The XObject whose Cos object is obtained.
	@return The dictionary Cos object for the XObject. 
	@since PI_PDMODEL_VERSION >= 0x00020000
*/
ENPROC(CosObj, PDXObjectGetCosObj, (PDXObject xObj))

/**
	(Obsolete, provided only for backwards compatibility) Gets 
	the value of the XObject stream's <code>length</code> key, which specifies 
	the amount of data in the PDF file (that is, after all compression/encoding 
	filters have been applied). 
	@param xObj The XObject whose data length is obtained.
	@return The XObject's data length. 
	@see PDXObjectGetData 
	@since PI_PDMODEL_VERSION >= 0x00020000
*/
ENPROC(ASInt32, PDXObjectGetDataLength, (PDXObject xObj))

/**
	(Obsolete, provided only for backwards compatibility) Passes 
	the data from an XObject to a user-supplied procedure. 
	@param obj The XObject whose data is read. 
	@param getDataProc A user-supplied callback to call with 
	the XObject's data. Enumeration ends if <code>getDataProc</code> returns 
	<code>false</code>. 
	@param clientData A pointer to user-supplied data to pass 
	to <code>getDataProc</code>.
	@see PDXObjectGetDataLength 
	@since PI_PDMODEL_VERSION >= 0x00020000
*/
ENPROC(void, PDXObjectGetData, (PDXObject obj, PDGetDataProc getDataProc, void *clientData))

/**
	(Obsolete, provided only for backwards compatibility) Enumerates 
	the filters attached to an XObject, calling a user-supplied 
	procedure for each filter. 
	@param obj The XObject whose filters are enumerated. 
	@param proc A user-supplied callback to call for each filter 
	attached to the XObject. Enumeration ends if <code>proc</code> returns 
	<code>false</code>. <code>proc</code> will not be called if there are no filters attached 
	to the XObject. 
	@param clientData A pointer to user-supplied data to pass 
	to <code>proc</code> each time it is called.
	@since PI_PDMODEL_VERSION >= 0x00020000
*/
ENPROC(void, PDXObjectEnumFilters, (PDXObject obj, PDXObjectFilterEnumProc proc, void *clientData))

/**
	(Obsolete, provided only for backwards compatibility. Use 
	PDEImageGetAttrs and/or Cos-level calls instead.) Gets the 
	attributes of an image (for example, Type, Subtype, Name, 
	Width, Height, BitsPerComponent, ColorSpace, Decode, Interpolate, or
	ImageMask). 
	@param obj The image whose attributes are obtained. 
	@param attrsP (Filled by the method) A pointer to a PDImageAttrs 
	structure containing the image attributes. 
	@param attrsLen It must be <code>sizeof(PDImageAttrs)</code>.
	@see PDInlineImageGetAttrs 
	@since PI_PDMODEL_VERSION >= 0x00020000
*/
ENPROC(void, PDImageGetAttrs, (PDXObject obj, PDImageAttrsP attrsP, ASInt32 attrsLen))

/**
	(Obsolete, provided only for backwards compatibility) 
	Gets the lookup table for an indexed color space. The table 
	will contain the number of entries specified by the index 
	size, and there will be 1 byte for each color component 
	for each entry. The number of color components depends on 
	the color space: 
	
	<TABLE rules="all" cellspacing="1">
	<TR><TH>Color</TH><TH>Number of components</TH></TR>
	<TR><TD>gray</TD><TD>1</TD></TR>
	<TR><TD>RGB</TD><TD>3</TD></TR>
	<TR><TD>CMYK</TD><TD>4</TD></TR>
	<TR><TD>Lab</TD><TD>3</TD></TR>
	</TABLE>
	
	<p>For additional information on indexed color space, see Section 4.5.5 in 
	the <i>PDF Reference</i>. There is also some useful discussion 
	in the <i>PostScript Language Reference Manual</i> 
	under indexed color spaces. </p>

	@param xobj The image whose lookup table is obtained. 
	@param data (Filled by the method) An array for the returned 
	color space information. 
	@param dataLen The length of data in bytes.
	@see PDInlineImageColorSpaceGetIndexLookup 
	@since PI_PDMODEL_VERSION >= 0x00020000
*/
ENPROC(void, PDImageColorSpaceGetIndexLookup, (PDXObject xobj, ASUns8 *data, ASInt32 dataLen))

/**
	(Obsolete, provided only for backwards compatibility) Gets 
	the value of a form's FormType attribute. 
	@param obj The form whose type is obtained.
	@return The form type (the value of the PDF FormType key). This value is 
	<code>1</code> for PDF 1.0, 1.1, and 1.2. 
	@since PI_PDMODEL_VERSION >= 0x00020000
*/
ENPROC(ASInt32, PDFormGetFormType, (PDXObject obj))

/**
	(Obsolete, provided only for backwards compatibility) Gets 
	a form's bounding box. 
	@param obj The form whose bounding box is obtained. 
	@param bboxP (Filled by the method) A pointer to a rectangle 
	containing the form's bounding box, specified in user space 
	coordinates.
	@since PI_PDMODEL_VERSION >= 0x00020000
*/
ENPROC(void, PDFormGetBBox, (PDXObject obj, ASFixedRect *bboxP))

/**
	(Obsolete, provided only for backwards compatibility) Gets 
	the specified form's transformation matrix. 
	@param obj The form whose transformation matrix is obtained. 
	
	@param matrixP (Filled by the method) A pointer to a matrix 
	containing the form's transformation matrix, which specifies 
	the transformation from form space to user space. See Section 
	4.9 in the <i>PDF Reference</i>.
	@since PI_PDMODEL_VERSION >= 0x00020000
*/
ENPROC(void, PDFormGetMatrix, (PDXObject obj, ASFixedMatrix *matrixP))

/**
	(Obsolete, provided only for backwards compatibility) Gets 
	the array Cos object corresponding to a form's XUID. An 
	XUID is an array of numbers that uniquely identify the form 
	in order to allow it to be cached. 
	@param obj The form whose XUID is obtained.
	@return The array Cos object for the form's XUID. 
	@since PI_PDMODEL_VERSION >= 0x00020000
*/
ENPROC(CosObj, PDFormGetXUIDCosObj, (PDXObject obj))

/**
	(Obsolete, provided only for backwards compatibility) Enumerates 
	the resources used by a form. 
	@param obj The form whose resources are enumerated. 
	@param mon A structure containing user-supplied callbacks 
	that are called for each of the form's resources. Enumeration 
	ends if any procedure returns <code>false</code>. 
	@param clientData A pointer to user-supplied data to pass 
	to <code>mon</code> each time it is called.
	@see PDFormEnumPaintProc 
  	@ingroup Enumerators
	@since PI_PDMODEL_VERSION >= 0x00020000
*/
ENPROC(void, PDFormEnumResources, (PDXObject obj, PDResourceEnumMonitor mon, void *clientData))

/**
	(Obsolete, provided only for backwards compatibility) Enumerates 
	a form's drawing operations. 
	@param obj The form whose drawing operations are enumerated. 
	
	@param mon A structure containing user-supplied callbacks 
	that are called for each drawing operator on a page. Enumeration 
	ends if any procedure returns <code>false</code>. 
	@param clientData A pointer to user-supplied data to pass 
	to <code>mon</code> each time it is called.
	@see PDFormEnumResources 
	@see PDFormEnumPaintProcWithParams 
  	@ingroup Enumerators
	@since PI_PDMODEL_VERSION >= 0x00020000
*/
ENPROC(void, PDFormEnumPaintProc, (PDXObject obj, PDGraphicEnumMonitor mon, void *clientData))

/**
	Enumerates a Type 3 font's character drawing procedures. 
	The elements of a single character procedure can be enumerated 
	using PDCharProcEnum(). 
	@param font IN/OUT The Type 3 font's character drawing procedures 
	are being enumerated. 
	@param proc IN/OUT A user-supplied callback to call for each character 
	in the font. Enumeration ends if <code>proc</code> returns <code>false</code>. If 
	the font contains no characters, <code>proc</code> will not be called. 
	
	@param clientData IN/OUT A pointer to user-supplied data passed 
	to <code>proc</code> each time it is called. 
	@see PDCharProcEnum 
	@see PDDocEnumFonts 
	@see PDDocEnumLoadedFonts 
  	@ingroup Enumerators
	@since PI_PDMODEL_VERSION >= 0x00020000
	
*/
ENPROC(void, PDFontEnumCharProcs, (PDFont font, PDCharProcEnumProc proc, void *clientData))

/**
	Enumerates the graphic description of a single character 
	procedure for a Type 3 font. To enumerate all the character 
	procedures in a Type 3 font (but not their graphic descriptions), 
	use PDFontEnumCharProcs(). 
	@param obj The character procedure whose graphic 
	description is enumerated. 
	@param mon A pointer to a structure containing user-supplied 
	callbacks that are called for each drawing operator on a 
	page. Enumeration halts if any procedure returns <code>false</code>. 
	
	@param clientData A pointer to user-supplied data to pass 
	to each monitor routine that is called.
	@see PDCharProcEnumWithParams 
	@see PDFontEnumCharProcs 
	@see PDDocEnumFonts 
	@see PDDocEnumLoadedFonts 
  	@ingroup Enumerators
	@since PI_PDMODEL_VERSION >= 0x00020000
*/
ENPROC(void, PDCharProcEnum, (PDCharProc obj, PDGraphicEnumMonitor mon, void *clientData))

/**
	Get the stream Cos object associated with the PDCharProc. 
	This method does not copy the object, but is instead the 
	logical equivalent of a type cast. 
	@param obj IN/OUT The character procedure whose Cos object is 
	obtained. 
	@return The character procedure's stream Cos object. 
	@see PDFontEnumCharProcs 
	@see PDCharProcEnum 
	@since PI_PDMODEL_VERSION >= 0x00020000
	
*/
ENPROC(CosObj, PDCharProcGetCosObj, (PDCharProc obj))


/**
	Creates a new article thread. Use PDDocAddThread() to add 
	the thread to a document. 
	@param aDocP The document in which the thread is created.
	@return The newly created thread. 
	@see PDThreadDestroy 
	@see PDThreadFromCosObj 
	@see PDDocAddThread 
	@since PI_PDMODEL_VERSION >= 0x00020000
*/
UNPROC(PDThread, PDThreadNew, (PDDoc aDocP))

/**
	Deletes an article thread from its document. You must call 
	PDDocRemoveThread() to remove the thread from the document 
	(if it was added to one using PDDocAddThread()) before calling 
	PDThreadDestroy(). 
	@param thread IN/OUT The thread to destroy. 
	@see PDThreadNew 
	@see PDThreadFromCosObj 
	@see PDDocRemoveThread 
	@since PI_PDMODEL_VERSION >= 0x00020000
	
*/
UNPROC(void, PDThreadDestroy, (PDThread thread))

/**
	Gets an article thread's first bead. 
	@param thread IN/OUT The thread whose first bead is obtained. 
	
	@return The thread's first bead, or a <code>NULL</code> Cos object if the thread 
	has no beads. 
	@see PDThreadSetFirstBead 
	@see PDBeadGetNext 
	@see PDBeadGetPrev 
	@since PI_PDMODEL_VERSION >= 0x00020000
	
*/
NPROC(PDBead, PDThreadGetFirstBead, (PDThread thread))

/**
	Sets an article thread's first bead. 
	@param thread IN/OUT The thread whose first bead is set. 
	@param newFirstBead IN/OUT The bead to use as the first in thread. 
	
	@see PDThreadGetFirstBead 
	@see PDBeadInsert 
	@since PI_PDMODEL_VERSION >= 0x00020000
	
*/
UNPROC(void, PDThreadSetFirstBead, (PDThread thread, PDBead newFirstBead))

/**
	Gets the specified article thread's info. 
	
	@param thread IN/OUT The thread whose thread info is obtained. 
	
	@param infoKey IN/OUT The key whose value is obtained. 
	@param buffer IN/OUT (Filled by the method) The value associated 
	with <code>infoKey</code>. 
	@param bufSize IN/OUT The maximum number of characters that <code>buffer</code> 
	can hold. 
	@return The number of characters written into <code>buffer</code>. 
	@see PDThreadSetInfo 

	@note For Roman viewers, this text is always stored in the 
	<code>PDFDocEncoding</code>. For non-Roman character set viewers, this 
	text is stored as <code>PDFDocEncoding</code> or Unicode, depending on 
	the file's creator. Files created in a non-Roman environment 
	contain Unicode versions of these strings; in a Roman environment, 
	files contain <code>PDFDocEncoding</code> versions of these strings. 
	@since PI_PDMODEL_VERSION >= 0x00020000
	
*/
NPROC(ASInt32, PDThreadGetInfo, (PDThread thread, const char *infoKey, char *buffer, ASInt32 bufSize))

/**
	Sets the specified article thread's info. 
	
	@param thread IN/OUT The thread whose thread info is set. 
	@param infoKey IN/OUT The key whose value is set. 
	@param buffer IN/OUT The value to set. 
	@param bufSize IN/OUT The number of characters in <code>buffer</code>. 
	@notify PDThreadDidChange 
	@see PDThreadGetInfo 

	@note For Roman viewers, this text is always stored in the 
	<code>PDFDocEncoding</code>. For non-Roman character set viewers, this 
	text is stored as <code>PDFDocEncoding</code> or Unicode, depending on 
	the file's creator. Files created in a non-Roman environment 
	contain Unicode versions of these strings; in a Roman environment, 
	files contain <code>PDFDocEncoding</code> versions of these strings. 
	@since PI_PDMODEL_VERSION >= 0x00020000
	
*/
UNPROC(void, PDThreadSetInfo, (PDThread thread, const char *infoKey, const char *buffer, ASInt32 bufSize))

/**
	Tests whether a thread is valid. This is intended only to 
	ensure that the thread has not been deleted, not to ensure 
	that all necessary information is present and valid. 
	@param thread The thread whose validity is tested.
	@return <code>true</code> if the thread is valid, <code>false</code> otherwise. 
	@since PI_PDMODEL_VERSION >= 0x00020000
*/
NPROC(ASBool, PDThreadIsValid, (PDThread thread))

/**
	Gets the Cos object corresponding to a thread. This method 
	does not copy the object, but is instead the logical equivalent 
	of a type cast. 
	@param thread IN/OUT The thread whose Cos object is to obtained. 
	
	@return The dictionary Cos object for the thread. The contents of the 
	dictionary can be enumerated using CosObjEnum(). It returns a 
	<code>NULL</code> Cos object if <code>PDThreadIsValid(thread)</code> returns <code>false</code>. 
	
	@see CosObjEnum 
	@see PDThreadFromCosObj 
	@since PI_PDMODEL_VERSION >= 0x00020000
	
*/
NPROC(CosObj, PDThreadGetCosObj, (PDThread thread))

/**
	Gets the thread object corresponding to the specified Cos 
	object. This method does not copy the object, but is instead 
	the logical equivalent of a type cast. 
	@param obj The dictionary Cos object for the thread.
	@return The PDThread object for the thread. 
	@exception pdErrBadThread is raised if the thread is not valid as defined 
	by PDThreadIsValid. 
	@see PDThreadGetCosObj 
	@since PI_PDMODEL_VERSION >= 0x00020000
*/
NPROC(PDThread, PDThreadFromCosObj, (CosObj obj))


/**
	Creates a new bead on the specified page. The newly created 
	bead is not linked to a thread or another bead. Use PDThreadSetFirstBead() 
	to make the bead the first bead in a thread. Use PDBeadInsert() 
	to link it to another bead. 
	@param page The page on which the bead is created. 
	@param destRect A pointer to a ASFixedRect specifying the 
	bead's bounding rectangle, specified in user space coordinates.
	@return The newly created bead. 
	@see PDThreadSetFirstBead 
	@see PDBeadInsert 
	@see PDBeadFromCosObj 
	@see PDBeadDestroy 
	@since PI_PDMODEL_VERSION >= 0x00020000
*/
UNPROC(PDBead, PDBeadNew, (PDPage page, const ASFixedRectP destRect))

/**
	Destroys a bead. 
	@param bead IN/OUT The bead to destroy. 
	@exception pdErrBadBead 
	@see PDBeadNew 
	@see PDBeadFromCosObj 
	@since PI_PDMODEL_VERSION >= 0x00020000
	
*/
UNPROC(void, PDBeadDestroy, (PDBead bead))

/**
	Gets the next bead in a thread. 
	@param bead IN/OUT The bead for which the next bead is obtained. 
	
	@return The next bead, or a <code>NULL</code> Cos object (cast to a PDBead using 
	PDBeadFromCosObj()). On the last bead, PDBeadGetNext() returns 
	the first bead. 
	@see PDBeadGetPrev 
	@see PDThreadGetFirstBead 
	@see PDBeadEqual 
	@since PI_PDMODEL_VERSION >= 0x00020000
	
*/
NPROC(PDBead, PDBeadGetNext, (PDBead bead))

/**
	Gets the previous bead in a thread. 
	@param bead IN/OUT The bead for which the previous bead is obtained. 
	
	@return The previous bead, or a <code>NULL</code> Cos object (cast to a PDBead 
	using PDBeadFromCosObj()) if this is the first bead in the 
	thread. 
	@see PDBeadGetNext 
	@see PDThreadGetFirstBead 
	@see PDBeadEqual 
	@since PI_PDMODEL_VERSION >= 0x00020000
	
*/
NPROC(PDBead, PDBeadGetPrev, (PDBead bead))

/**
	Inserts a bead after the specified bead. 
	@param bead IN/OUT The bead after which newNext will be inserted. 
	
	@param newNext IN/OUT The bead to insert. 
	@see PDBeadNew 
	@see PDThreadSetFirstBead 
	@since PI_PDMODEL_VERSION >= 0x00020000
	
*/
UNPROC(void, PDBeadInsert, (PDBead bead, PDBead newNext))

/**
	Acquires the page on which a bead is located. 
	@param bead IN/OUT The bead whose page is acquired. 
	@param pdDoc IN/OUT The document in which bead is located. 
	@return The page on which the bead resides. The acquired page must 
	be freed using PDPageRelease() when it is no longer needed. 
	
	@see PDPageRelease 
	@since PI_PDMODEL_VERSION >= 0x00020000
	
*/
NPROC(PDPage, PDBeadAcquirePage, (PDBead bead, PDDoc pdDoc))

/**
	Sets the page for a bead. 
	@param bead IN/OUT The bead whose page is set. 
	@param newPage IN/OUT The page on which bead is located. 
	@exception pdErrBadBead 
	@see PDBeadSetRect 
	@since PI_PDMODEL_VERSION >= 0x00020000
	
*/
UNPROC(void, PDBeadSetPage, (PDBead bead, PDPage newPage))

/**
	Gets a bead's bounding rectangle. 
	@param bead IN/OUT The bead whose bounding rectangle is obtained. 
	
	@param rectP IN/OUT (Filled by the method) A pointer to a ASFixedRect 
	specifying the bead's bounding rectangle, specified in user 
	space coordinates. 
	@since PI_PDMODEL_VERSION >= 0x00020000
	
*/
NPROC(void, PDBeadGetRect, (PDBead bead, ASFixedRectP rectP))

/**
	Sets a bead's bounding rectangle. 
	@param bead IN/OUT The bead whose bounding rectangle is set. 
	@param newDestRect IN/OUT A pointer to a ASFixedRect specifying 
	the bead's bounding rectangle, specified in user space coordinates. 
	
	@see PDBeadGetRect 
	@see PDBeadSetPage 
	@since PI_PDMODEL_VERSION >= 0x00020000
	
*/
UNPROC(void, PDBeadSetRect, (PDBead bead, const ASFixedRectP newDestRect))

/**
	Tests a bead's validity. This is intended only to ensure 
	that the bead has not been deleted, not to ensure that all 
	necessary information is present and valid. 
	@param bead The bead whose validity is tested.
	@return <code>true</code> if the bead is valid, <code>false</code> otherwise. 
	@see PDBeadEqual 
	@since PI_PDMODEL_VERSION >= 0x00020000
*/
NPROC(ASBool, PDBeadIsValid, (PDBead bead))

/**
	Gets the thread containing the specified bead. 
	@param bead IN/OUT The bead whose thread is obtained. 
	@return The bead's thread, or a <code>NULL</code> Cos object if the bead does 
	not belong to a thread. 
	@see PDBeadGetRect 
	@see PDBeadGetIndex 
	@see PDBeadGetNext 
	@see PDBeadGetPrev 
	@since PI_PDMODEL_VERSION >= 0x00020000
	
*/
NPROC(PDThread, PDBeadGetThread, (PDBead bead))

/**
	Gets the index of a bead in its thread. 
	@param bead IN/OUT The bead whose index is obtained. 
	@return The index of the bead in its thread. The first bead in a 
	thread has an index of zero. 
	@since PI_PDMODEL_VERSION >= 0x00020000
	
*/
NPROC(ASInt32, PDBeadGetIndex, (PDBead bead))

/**
	Tests two beads for equality. This method is useful to detect 
	the end of a thread since the last bead in a thread points 
	to the first. 
	@param bead The first bead to compare. 
	@param bead2 The second bead to compare.
	@return <code>true</code> if the two beads are identical, <code>false</code> otherwise. 
	Two beads are equal only if their Cos objects are 
	equal (see CosObjEqual()). 
	@see CosObjEqual 
	@since PI_PDMODEL_VERSION >= 0x00020000
*/
NPROC(ASBool, PDBeadEqual, (PDBead bead, PDBead bead2))

/**
	Gets the Cos object corresponding to a bead. This method 
	does not copy the object, but is instead the logical equivalent 
	of a type cast. 
	@param bead IN/OUT The bead whose Cos object is obtained. 
	@return The dictionary Cos object for the bead. The contents of the 
	dictionary can be enumerated using CosObjEnum(). It returns a 
	<code>NULL</code> Cos object if <code>PDBeadIsValid(bead)</code> returns <code>false</code>. 
	@see PDBeadFromCosObj 
	@since PI_PDMODEL_VERSION >= 0x00020000
	
*/
NPROC(CosObj, PDBeadGetCosObj, (PDBead bead))

/**
	Gets the PDBead corresponding to a Cos object, after checking 
	the bead's validity. This method does not copy the object, 
	but is instead the logical equivalent of a type cast. 
	@param obj The dictionary Cos object whose bead is obtained.
	@return The PDBead object for the bead. 
	@exception pdErrBadBead is raised if the bead is not valid, as determined 
	by PDBeadIsValid(). 
	@see PDBeadGetCosObj 
	@since PI_PDMODEL_VERSION >= 0x00020000
*/
NPROC(PDBead, PDBeadFromCosObj, (CosObj obj))


/**
	Creates a new view destination object. 
	@param doc The document in which the destination is used. 
	@param initialPage The destination page. 
	@param initialFitType The destination fit type. It must be one 
	of the View Destination Fit Types, which must be converted 
	into an ASAtom with ASAtomFromString(). 
	@param initialRect A pointer to an ASFixedRect specifying 
	the destination rectangle, specified in user space coordinates. 
	The appropriate information will be extracted from <code>initialRect</code>, 
	depending on <code>initialFitType</code>, to create the destination. 
	All four of the <code>initialRect</code> parameter's components should be set; using 
	PDViewDestNULL for any components can result in incorrect 
	results for rotated pages. 
	@param initialZoom The zoom factor to set for the destination. 
	Used only if <code>initialFitType</code> is <code>XYZ</code>. Use the predefined value 
	PDViewDestNULL (see PDExpT.h) to indicate a <code>NULL</code> zoom factor, 
	described in Table 8.2 in the <i>PDF Reference</i>. 
	@param pageNumber Currently unused.
	@return The newly created view destination. 
	@see PDViewDestFromCosObj 
	@see PDViewDestDestroy 
  	@ref ViewDestinationFitTypes
	@since PI_PDMODEL_VERSION >= 0x00020000
*/
UNPROC(PDViewDestination, PDViewDestCreate, (PDDoc doc, PDPage initialPage, ASAtom initialFitType, const ASFixedRectP initialRect, const ASFixed
	initialZoom, ASInt32 pageNumber))

/**
	Deletes a view destination object. Before deleting a view 
	destination, ensure that no link or bookmark refers to it. 
	
	@param dest IN/OUT The view destination to destroy. 
	@see PDViewDestFromCosObj 
	@see PDViewDestCreate 
	@since PI_PDMODEL_VERSION >= 0x00020000
	
*/
UNPROC(void, PDViewDestDestroy, (PDViewDestination dest))

/**
	Tests whether a view destination is valid. This is intended 
	only to ensure that the view destination has not been deleted, 
	not to ensure that all necessary information is present 
	and valid. 
	@param dest The view destination whose validity is determined.
	@return <code>true</code> if the view destination is valid, <code>false</code> otherwise. 
	
	@since PI_PDMODEL_VERSION >= 0x00020000
*/
NPROC(ASBool, PDViewDestIsValid, (PDViewDestination dest))

/**
	Gets a view destination's fit type, destination rectangle, 
	and zoom factor. The destination must be represented by 
	an array, which is the case for a GoToR action. 
	@param dest IN/OUT The view destination whose attributes are obtained. 
	
	@param pageNum IN/OUT (Filled by the method) The page number of 
	the destination's page. 
	@param fitType IN/OUT (Filled by the method) The destination fit type. 
	One of the values listed in View Destination Fit Types. 
	
	@param destRect IN/OUT (Filled by the method) A pointer to a ASFixedRect 
	containing the destination's rectangle, specified in user 
	space coordinates. 
	@param zoom IN/OUT (Filled by the method) The destination's zoom 
	factor. 
	@exception genErrBadParm is raised if the destination is not represented 
	by an array. 
	@see PDViewDestCreate 
  	@ref ViewDestinationFitTypes
	@since PI_PDMODEL_VERSION >= 0x00020000
*/
NPROC(void, PDViewDestGetAttr, (PDViewDestination dest, ASInt32* pageNum, ASAtom *fitType, ASFixedRectP destRect, ASFixed *zoom))

/**
	Gets the Cos object corresponding to a view destination 
	and verifies that the view destination is valid. This method 
	does not copy the object, but is instead the logical equivalent 
	of a type cast. 
	@param dest IN/OUT The view destination whose Cos object is obtained. 
	
	@return Array Cos object for the view destination. The contents 
	of the array can be enumerated using CosObjEnum(). It returns 
	a <code>NULL</code> Cos object if the view destination is invalid, as 
	determined by PDViewDestIsValid(). 
	@see PDViewDestFromCosObj 
	@see PDViewDestIsValid 
	@since PI_PDMODEL_VERSION >= 0x00020000
	
*/
NPROC(CosObj, PDViewDestGetCosObj, (PDViewDestination dest))

/**
	Converts the specified Cos object to a view destination 
	and verifies that the view destination is valid. This method 
	does not copy the object, but is instead the logical equivalent 
	of a type cast. 
	@param obj IN/OUT The dictionary Cos object to convert to a view destination. 
	
	@return An array Cos object for the view destination. 
	@exception pdErrBadAction is raised if the destination is invalid, as 
	determined by PDViewDestIsValid(). 
	@see PDViewDestGetCosObj 
	@since PI_PDMODEL_VERSION >= 0x00020000
	
*/
NPROC(PDViewDestination, PDViewDestFromCosObj, (CosObj obj))


/**
	Deletes a text selection object (the text on the page remains 
	unchanged). Do not use this method to destroy a text selection 
	that was passed to AVDocSetSelection(); such text selections 
	are automatically destroyed when a new selection is made 
	or the selection is cleared. 
	@param text IN/OUT The text selection to destroy. 
	@see PDDocCreateTextSelect 
	@see PDTextSelectCreatePageHilite 
	@see PDTextSelectCreateRanges 
	@see PDTextSelectCreateWordHilite 
	@since PI_PDMODEL_VERSION >= 0x00020000
	
*/
NPROC(void, PDTextSelectDestroy, (PDTextSelect text))

/**
	Enumerates the bounding quads in a text selection. <code>proc</code> 
	is called for each quad. If a word is on a curve it may 
	have a quad for each character, but it may also have two 
	characters per quad. An upright word will have only one 
	quad for all the characters. An upright hyphenated word 
	will have two quads. 
	@param text IN/OUT The text selection whose bounding quads are 
	enumerated. 
	@param proc IN/OUT A user-supplied callback to call for each quad. 
	Enumeration halts if <code>proc</code> returns <code>false</code>. 
	@param procObj IN/OUT A user-supplied data to pass to <code>proc</code> each 
	time it is called. 
	@exception genErrBadParm 
	@see PDDocCreateTextSelect 
	@since PI_PDMODEL_VERSION >= 0x00020000
	
*/
SPROC(void, PDTextSelectEnumQuads, (PDTextSelect text, PDTextSelectEnumQuadProc proc, void *procObj), PDTextSelectEnumQuadsHost)

/**
	Enumerates the strings of the specified text select object, 
	calling a procedure for each string. A string, in this context, 
	is the set of like-styled characters within a word. It is 
	never larger than a single word. A word containing three 
	styles is enumerated as three strings. There is no guaranteed 
	correspondence between these strings and the actual show 
	strings in the PDF file. Acrobat enumerates text in the 
	order it appears in the PDF file, which is often not the 
	same as the order in which a person would read the text. 
	
	@param text IN/OUT The text selection whose strings are enumerated. 
	
	@param proc IN/OUT A user-supplied callback to call for each string 
	in the text object. Enumeration ends if <code>proc</code> returns <code>false</code>. 
	
	@param procObj IN/OUT User-supplied data to pass to <code>proc</code> each 
	time it is called. 
	@exception genErrBadParm 
	@exception pdErrOpNotPermitted 
	@see PDDocCreateTextSelect 
	@since PI_PDMODEL_VERSION >= 0x00020000
	
*/
SPROC(void, PDTextSelectEnumText, (PDTextSelect text, PDTextSelectEnumTextProc proc, void *procObj), PDTextSelectEnumTextHost)

/**
	Gets the page number of a text selection's page. 
	@param text IN/OUT The text selection whose page number is obtained. 
	
	@return The page number of the text selection's page. 
	@see PDTextSelectGetBoundingRect 
	@see PDTextSelectGetRange 
	@see PDTextSelectGetRangeCount 
	@since PI_PDMODEL_VERSION >= 0x00020000
	
*/
NPROC(ASInt32, PDTextSelectGetPage, (PDTextSelect text))

/**
	Gets a text selection's bounding rectangle. This is the 
	smallest rectangle that completely encloses all characters 
	in the selection. 
	@param text IN/OUT The text selection whose bounding rectangle 
	is determined. 
	@param boundRectP IN/OUT (Filled by the method) A pointer to the 
	text selection's bounding rectangle, specified in user space 
	coordinates. 
	@see PDTextSelectGetPage 
	@see PDTextSelectGetRange 
	@see PDTextSelectGetRangeCount 
	@since PI_PDMODEL_VERSION >= 0x00020000
	
*/
SPROC(void, PDTextSelectGetBoundingRect, (PDTextSelect text, ASFixedRect *boundRectP), PDTextSelectGetBoundingRectHost)

/**
	Creates a text selection from a page and a list of highlights 
	specified as character offsets from the start of the page. 
	Character offsets are a well-defined quantity in the PDF 
	file, and are therefore stable against revisions of the 
	word-finding algorithm, which makes them a good way to isolate 
	yourself from changes in the algorithm. 

	<p>This method does not highlight the text selection. That 
	occurs when you pass the PDTextSelect returned by this method 
	to AVDocSetSelection(). </p>
	@param page The page on which the highlights appear. 
	@param hList A pointer to an array of highlight entries. 
	If the <code>length</code> field of a HiliteEntry is <code>0</code>, the entire word 
	is highlighted. <code>hList</code> should not contain multiple instances 
	of the same highlight; the display appearance is undefined 
	when it does. 
	@param listLen The number of highlight entries in <code>hList</code>.
	@return The newly created text selection. 
	@see AVDocSetSelection 
	@see PDTextSelectCreateWordHilite 
	@see PDTextSelectCreateWordHiliteEx 
	@see PDTextSelectCreateRanges 
	@see PDTextSelectCreateRangesEx 
	@see PDTextSelectCreatePageHiliteEx 
	@see PDDocCreateTextSelect 
	@see PDTextSelectDestroy 

	@note As is the case with the Acrobat viewer, the text selection is always 
	of whole words, not part of words. 
	@since PI_PDMODEL_VERSION >= 0x00020000
*/
SPROC(PDTextSelect, PDTextSelectCreatePageHilite, (PDPage page, HiliteEntry *hList, ASInt32 listLen), PDTextSelectCreatePageHiliteHost)

/**
	Creates a text selection from a list of highlights specified 
	as word offsets from the start of the page. Word offsets 
	are not well-defined in PDF files, but are calculated by 
	the word-finding algorithm. As a result, word offsets will, 
	in general, differ in different versions of the word-finding 
	algorithm. If you choose to store word offsets, you must 
	also store the version of the word-finding algorithm from 
	which they are obtained using PDWordFinderGetLatestAlgVersion(). 
	
	<p>This method does not highlight the text selection. That 
	occurs when you pass the PDTextSelect returned by this method 
	to AVDocSetSelection(). </p>
	@param page The page on which the highlights appear. 
	@param hList A pointer to an array of highlight entries. 
	<code>hList</code> should not contain multiple instances of the same highlight; 
	the display appearance is undefined when it does. 
	@param listLen The number of highlight entries in <code>hList</code>.
	@return The newly created text selection. 
	@see AVDocSetSelection 
	@see PDTextSelectCreateRanges 
	@see PDTextSelectCreateRangesEx 
	@see PDTextSelectCreatePageHilite 
	@see PDTextSelectCreatePageHiliteEx 
	@see PDTextSelectCreateWordHiliteEx 
	@see PDWordFinderGetLatestAlgVersion 
	@see PDTextSelectDestroy 
	@since PI_PDMODEL_VERSION >= 0x00020000
*/
SPROC(PDTextSelect, PDTextSelectCreateWordHilite, (PDPage page, HiliteEntry *hList, ASInt32 listLen), PDTextSelectCreateWordHiliteHost)

/**
	Extracts the range specified by index from a text selection. 
	Use PDTextSelectGetRangeCount() to determine the number of 
	ranges in a text selection. 
	@param textP IN/OUT The text selection from which a range is extracted. 
	
	@param index IN/OUT The index of the range to extract from <code>textP</code>. 
	
	@param range IN/OUT (Filled by the method) A pointer to a structure 
	that contains the specified range. 
	@see PDTextSelectGetBoundingRect 
	@see PDTextSelectGetPage 
	@see PDTextSelectGetRangeCount 
	@since PI_PDMODEL_VERSION >= 0x00020000
	
*/
NPROC(void, PDTextSelectGetRange, ( PDTextSelect textP, ASInt32 index, PDTextSelectRange range ))

/**
	Gets the number of ranges in a text selection. Use PDTextSelectGetRange() 
	to extract a single range from a text selection. 
	@param textP IN/OUT The text selection whose range count is obtained. 
	
	@return The number of ranges in the text selection. 
	@see PDTextSelectGetBoundingRect 
	@see PDTextSelectGetPage 
	@see PDTextSelectGetRange 
	@since PI_PDMODEL_VERSION >= 0x00020000
	
*/
NPROC(ASInt32, PDTextSelectGetRangeCount, ( PDTextSelect textP ))

/**
	Creates a text selection from one or more ranges. 

	<p>This method does not highlight the text selection. That 
	occurs when you pass the PDTextSelect returned by this method 
	to AVDocSetSelection(). </p>
	@param page IN/OUT The page on which the text appears. 
	@param range IN/OUT A pointer to an array of ranges that are used 
	to create a text selection. Each array element is a PDTextSelectRange 
	structure. 
	@param rangeCount IN/OUT The number of ranges in range. 
	@return A text selection created from the specified ranges. 
	@see AVDocSetSelection 
	@see PDTextSelectCreateRangesEx 
	@see PDTextSelectCreatePageHilite 
	@see PDTextSelectCreatePageHiliteEx 
	@see PDTextSelectCreateWordHilite 
	@see PDTextSelectCreateWordHiliteEx 
	@see PDTextSelectDestroy 
	@since PI_PDMODEL_VERSION >= 0x00020000
	
*/
SPROC(PDTextSelect, PDTextSelectCreateRanges, ( PDPage page, PDTextSelectRange range, ASInt32 rangeCount ), PDTextSelectCreateRangesHost)


/**
	Finds all words on the specified page and returns one or 
	more tables containing the words. One table contains the 
	words sorted in the order in which they appear in the PDF 
	file, while the other contains the words sorted by their 
	x- and y-coordinates on the page. 

	<p>Only words within or partially within the page's crop box 
	(see PDPageGetCropBox()) are enumerated. Words outside the 
	crop box are skipped. </p>

	<p>There can be only one word list in existence at a time; 
	clients must release the previous word list, using PDWordFinderReleaseWordList(), 
	before creating a new one. </p>

	<p>Use PDWordFinderEnumWords() instead of this method, if you 
	wish to find one word at a time instead of obtaining a table 
	containing all words on a page. </p>

	@param wObj The word finder (created using PDDocCreateWordFinder() 
	or PDDocCreateWordFinderUCS()) used to acquire the word list. 
	
	@param pgNum The page number for which words are found. 
	The first page is <code>0</code>, not <code>1</code> as designated in Acrobat. 

	@param wInfoP (Filled by the method) A user-supplied 
	PDWord variable. Acrobat will fill this in to point to an 
	Acrobat-allocated array of PDWord objects, which should <i>never</i> be 
	accessed directly.
	
	<p>Access the acquired list through PDWordFinderGetNthWord(). 
	The words are ordered in PDF order, which is the order in 
	which they appear in the PDF file's data. This is often, 
	but not always, the order in which a person would read the 
	words. Use PDWordFinderGetNthWord() to traverse this array; you 
	cannot access this array directly. This array is always 
	filled, regardless of the flags used in the call to PDDocCreateWordFinder() 
	or PDDocCreateWordFinderUCS().</p>

	@param xySortTable (Filled by the method) Acrobat fills 
	in this user-supplied pointer to a pointer with the location 
	of an Acrobat-allocated array of PDWords, sorted in x-y 
	order, meaning that all words on the first <i>line</i>, from left 
	to right, are followed by all words on the next line. This 
	array is only filled if the <code>WXE_XY_SORT</code> flag was set in 
	the call to PDDocCreateWordFinder() or PDDocCreateWordFinderUCS(). 
	PDWordFinderReleaseWordList() must be called to release allocated 
	memory for this return or there will be a memory leak. As 
	long as this parameter is non-<code>NULL</code>, the array is always 
	filled regardless of the value of the <code>rdFlags</code> parameter 
	in PDDocCreateWordFinder(). 
	@param rdOrderTable Currently unused. Pass <code>NULL</code> for this value. 
	@param numWords (Filled by the method) The number of words 
	found on the page.
	@exception pdErrOpNotPermitted 
	@see PDDocCreateWordFinder 
	@see PDDocCreateWordFinderUCS 
	@see PDWordFinderReleaseWordList 
	@see PDWordFinderEnumWords 
	@see PDWordFinderGetNthWord 
	@since PI_PDMODEL_VERSION >= 0x00020000
*/
NPROC(void, PDWordFinderAcquireWordList, ( PDWordFinder wObj, ASInt32 pgNum, PDWord *wInfoP, PDWord **xySortTable, PDWord **rdOrderTable, ASInt32 *numWords))

/**
	Gets the version number of the specified word finder, or 
	the version number of the latest word finder algorithm. 
	
	@param wObj IN/OUT The word finder whose algorithm's version is 
	obtained. Pass <code>NULL</code> to obtain the latest word finding algorithm 
	version number. 
	@return The algorithm version associated with <code>wObj</code>, or the version 
	of the latest word finder algorithm if <code>wObj</code> is <code>NULL</code>. 
	@see PDDocCreateWordFinder 
	@see PDDocCreateWordFinderUCS 
	@see PDDocGetWordFinder 
	@since PI_PDMODEL_VERSION >= 0x00020000
	
*/
NPROC(ASInt16, PDWordFinderGetLatestAlgVersion, ( PDWordFinder wObj ))

/**
	Releases the word list for a given page. Use this to release 
	a list created by PDWordFinderAcquireWordList() when you are 
	done using this list. 
	@param wObj A word finder object. 
	@param pgNum The number of pages for which a word list is 
	released.
	@exception genErrBadUnlock is raised if the list has already been released. 
	
	@see PDWordFinderAcquireWordList 
	@see PDDocCreateWordFinder 
	@see PDDocCreateWordFinderUCS 
	@see PDDocGetWordFinder 
	@since PI_PDMODEL_VERSION >= 0x00020000
*/
NPROC(void, PDWordFinderReleaseWordList, ( PDWordFinder wObj, ASInt32 pgNum ))

/**
	Destroys a word finder. Use this when you are done extracting 
	text in a file. 
	@param wObj IN/OUT The word finder to destroy. 
	@see PDDocCreateWordFinder 
	@see PDDocCreateWordFinderUCS 
	@see PDDocGetWordFinder 
	@since PI_PDMODEL_VERSION >= 0x00020000
	
*/
NPROC(void, PDWordFinderDestroy, ( PDWordFinder wObj ))

/**
	Extracts words, one at a time, from the specified page or 
	the entire document. It calls a user-supplied procedure once 
	for each word found. If you wish to extract all text from 
	a page at once, use PDWordFinderAcquireWordList() instead 
	of this method. 

	<p>Only words within or partially within the page's crop box 
	(see PDPageGetCropBox()) are enumerated. Words outside the 
	crop box are skipped. </p>

	@param wObj A word finder object. 
	@param PageNum The page number from which to extract words. 
	Pass PDAllPages (see PDExpT.h) to sequentially process all 
	pages in the document. 
	@param wordProc A user-supplied callback to call once for 
	each word found. Enumeration halts if <code>wordProc</code> returns <code>false</code>. 
	
	@param clientData A pointer to user-supplied data to pass 
	to <code>wordProc</code> each time it is called.
	@return <code>true</code> if enumeration was successfully completed, 
	<code>false</code> if enumeration was terminated because <code>wordProc</code> returned 
	<code>false</code>. 
	@exception genErrBadParm is raised if <code>wordProc</code> is <code>NULL</code>, or <code>pageNum</code> is 
	less than zero or greater than the total number of pages in the document. 
	@exception pdErrOpNotPermitted 
	@see PDDocCreateWordFinder 
	@see PDDocCreateWordFinderUCS 
	@see PDDocGetWordFinder 
	@see PDWordFinderAcquireWordList 
	@see PDWordFinderEnumVisibleWords 
	@see PDWordFinderEnumWordsStr 
	@since PI_PDMODEL_VERSION >= 0x00020000
*/
NPROC(ASBool, PDWordFinderEnumWords, ( PDWordFinder wObj, ASInt32 PageNum, PDWordProc wordProc, void *clientData ))


/**
	Gets the number of bytes in a word. This method also works 
	on non-Roman systems. 
	@param word IN/OUT The word object whose character count is obtained. 
	
	@return The number of characters in word. 
	@see PDWordGetCharDelta 
	@see PDWordGetCharOffset 
	@since PI_PDMODEL_VERSION >= 0x00020000
	
*/
NPROC(ASUns8, PDWordGetLength, ( PDWord word ))

/**
	This method gets a word's text. The string to return includes any 
	word break characters (such as space characters) that follow 
	the word, but not any that precede the word. The characters 
	that are treated as word breaks are defined in the <code>outEncInfo</code> 
	parameter of PDDocCreateWordFinder() method. Use PDWordFilterString() 
	to subsequently remove the word break characters.
	
	<p>This method produces a string in whatever encoding the PDWord 
	uses, for both Roman and non-Roman systems. </p>
	@param word The word whose string is obtained. 
	@param str (Filled by the method) The string. The encoding 
	of the string is the encoding used by the <code>PDWordFinder</code> that 
	supplied the PDWord. For instance, if PDDocCreateWordFinderUCS() 
	is used to create the word finder, PDWordGetString() returns 
	only Unicode. There is no way to detect Unicode strings 
	returned by PDWordGetString(), since there is no UCS header 
	(<code>FEFF</code>) added to each string returned. 
	@param len The length of <code>str</code> in bytes. Up to <code>len</code> characters 
	of word will be copied into <code>str</code>. If <code>str</code> is long enough, 
	it will be <code>NULL</code>-terminated.
	@exception genErrBadParm is raised if either <code>word</code> or <code>str</code> is <code>NULL</code>. 
	@see PDWordGetLength 
	@see PDWordGetCharDelta 
	@see PDWordSplitString 
	@since PI_PDMODEL_VERSION >= 0x00020000
*/
NPROC(void, PDWordGetString, ( PDWord word, char *str, ASInt32 len ))

/**
	Gets a bit field containing information on the types of 
	characters in a word. Use PDWordGetCharacterTypes() if you 
	wish to check each character's type individually. 
	@param word IN/OUT The word whose character types are obtained. 
	
	@return A bit field containing information on the types of characters 
	in word. The value is a logical <code>OR</code> of the Word Attributes. 
	
	@see PDWordGetCharacterTypes 
	@see PDWordGetStyleTransition 
	@see PDWordGetNthCharStyle 
	@since PI_PDMODEL_VERSION >= 0x00020000
	
	@note PDWordGetAttr() may return an attribute value greater 
	than the maximum of all of the public attributes since there 
	can be private attributes added on. It is recommended to 
	<code>AND</code> the result with the attribute you are interested in. 
*/
NPROC(ASUns16, PDWordGetAttr, ( PDWord word ))

/**
	Gets the character type for each character in a word. 
	@param word The word whose character types are obtained. 
	
	@param cArr (Filled by the method) An array of character 
	types. This array contains one element for each character 
	in the word. Use PDWordGetLength() to determine the number 
	of elements that must be in the array. Each element is the 
	logical <code>OR</code> of one or more of the Character Type Codes. For 
	non-Roman character set viewers, meaningful values are returned 
	only for Roman characters. For non-Roman characters, it 
	returns <code>0</code>, which is the same as <code>W_CNTL</code>. If the character 
	is 2 bytes, both bytes indicate the same character type. 
	
	@param size The number of elements in <code>cArr</code>.
	@see PDWordGetAttr 
	@see PDWordGetLength 
  	@ref CharacterTypeCodes
	@since PI_PDMODEL_VERSION >= 0x00020000
*/
NPROC(void, PDWordGetCharacterTypes, ( PDWord word, ASUns16 *cArr, ASInt16 size ))

/**
	Returns a word's character offset from the beginning of 
	its page. This information, together with the character 
	delta obtained from PDWordGetCharDelta(), can be used to highlight 
	a range of words on a page, using PDTextSelectCreatePageHilite(). 
	
	@param word IN/OUT The word whose character offset is obtained. 
	
	@return The word's character offset. On multi-byte systems, it points 
	to the first byte. 
	@see PDWordGetCharDelta 
	@see PDWordGetLength 
	@see PDTextSelectCreatePageHilite 
	@since PI_PDMODEL_VERSION >= 0x00020000
	
*/
NPROC(ASUns16, PDWordGetCharOffset, ( PDWord word ))

/**
	Gets the difference between the word length (the number 
	of printed characters in the word) and the PDF word length 
	(the number of character codes in the word). For instance, 
	if the PDF word is <code>fi (ligature) sh</code> the mapped word will 
	be <code>"fish"</code>. The ligature occupies only one character code, 
	so in this case the character delta will be <code>3-4 = -1</code>. 
	@param word IN/OUT The word whose character delta is obtained. 
	
	@return The character delta for word. Cast the return value to an 
	ASInt8 before using. 

	If the PDWord's character set has no ligatures, such as 
	on a non-Roman viewer supporting Japanese, returns <code>0</code>. 
	@see PDWordGetLength 
	@see PDWordGetCharOffset 
	@since PI_PDMODEL_VERSION >= 0x00020000
	
*/
NPROC(ASInt8, PDWordGetCharDelta, ( PDWord word ))

/**
	Gets the locations of style transitions in a word. Every 
	word has at least one style transition, at character position 
	zero in the word. 
	@param word IN/OUT The word whose style transition list is obtained. 
	
	@param transTbl IN/OUT (Filled by the method) An array of style transitions. 
	Each element is the character offset in word where the style 
	changes. The offset specifies the first character in the 
	word that has the new style. The first character in a word 
	has an offset of zero. 
	@param size IN/OUT The number of entries that <code>transTbl</code> can hold. The 
	word is searched only until this number of style transitions 
	have been found. 
	@return The number of style transition offsets copied to <code>transTbl</code>. 
	@see PDWordGetNthCharStyle 
	@since PI_PDMODEL_VERSION >= 0x00020000
	
*/
NPROC(ASInt16, PDWordGetStyleTransition, ( PDWord word, ASInt16 *transTbl, ASInt16 size ))

/**
	Returns a PDStyle object for the nth style in a word. 
	@param wObj IN/OUT A word finder object. 
	@param word IN/OUT The word whose nth style is obtained. 
	@param dex IN/OUT The index of the style to obtain. The first 
	style in a word has an index of zero. 
	@return The nth style in the word. It returns <code>NULL</code> if <code>dex</code> is greater 
	than the number of styles in the word. 
	@exception genErrBadParm is raised if <code>dex < 0</code>. 
	@see PDWordGetStyleTransition 
	@since PI_PDMODEL_VERSION >= 0x00020000
	
*/
NPROC(PDStyle, PDWordGetNthCharStyle, ( PDWordFinder wObj,PDWord word, ASInt32 dex ))

/**
	Gets the number of quads in a word. A quad is a quadrilateral 
	bounding a contiguous piece of a word. Every word has at 
	least one quad. A word has more than one quad, for example, 
	if it is hyphenated and split across multiple lines or if 
	the word is set on a curve rather than on a straight line. 
	
	@param word IN/OUT The word whose quad count is obtained. 
	@return The number of quads in word. 
	@see PDWordGetNthQuad 
	@since PI_PDMODEL_VERSION >= 0x00020000
	
*/
NPROC(ASInt16, PDWordGetNumQuads,( PDWord word ))

/**
	Gets the specified word's nth quad, specified in user space 
	coordinates. See PDWordGetNumQuads() for a description of 
	a quad. 

	<p>The quad's height is the height of the font's bounding box, 
	not the height of the tallest character used in the word. 
	The font's bounding box is determined by the glyphs in the 
	font that extend farthest above and below the baseline; 
	it often extends somewhat above the top of <code>'A'</code> and below 
	the bottom of <code>'y'</code>. </p>

	<p>The quad's width is determined from the characters actually 
	present in the word. </p>

	<p>For example, the quads for the words <code>"AWAY"</code> and <code>"away"</code> 
	have the same height, but generally do not have the same 
	width unless the font is a mono-spaced font (a font in which 
	all characters have the same width). </p>

	Despite the names of the fields in an ASFixedQuad (<code>tl</code> for 
	top left, <code>bl</code> for bottom left, and so forth) the corners 
	of <code>quad</code> do not necessarily have these positions. 
	@param word The word whose nth quad is obtained. 
	@param nTh The quad to obtain. A word's first quad has 
	an index of zero. 
	@param quad (Filled by the method) A pointer to the word's 
	nth quad, specified in user-space coordinates.
	@return <code>true</code> if the word has an nth quad, <code>false</code> otherwise. 
	
	@see PDWordGetNumQuads 
	@since PI_PDMODEL_VERSION >= 0x00020000
*/
NPROC(ASBool, PDWordGetNthQuad,( PDWord word, ASInt16 nTh, ASFixedQuad *quad ))

/**
	Tests whether a word is rotated. 
	@param word The word to test.
	@return <code>true</code> if the word is rotated, <code>false</code> otherwise. 
	@see PDWordGetNthQuad 
	@since PI_PDMODEL_VERSION >= 0x00020000
*/
NPROC(ASBool, PDWordIsRotated,( PDWord word ))


/**
	Removes leading and trailing spaces and leading and trailing 
	punctuation (including soft hyphens) from the specified 
	word. It does not remove wildcard characters (<code><code>'*'</code></code> and <code><code>'?'</code></code>) 
	or any punctuation surrounded by alphanumeric characters 
	within the word.

	<p>The determination of which characters are alphanumeric, 
	wildcard, punctuation, and so forth, is made by the values 
	in <code>infoArray</code>. </p>

	<p>Although this method seems very similar to PDWordFilterWord(), 
	the two methods treat letters and digits slightly differently. 
	PDWordFilterWord() uses the encoding info array but also does 
	a straight character code test for any characters that have 
	not been mapped to anything. It does this to catch letters 
	and digits from non-standard character sets, and is necessary 
	to avoid removing words with non-standard character sets. </p>
	
	<p>PDWordFilterString(), on the other hand, was designed for 
	known character sets such as WinAnsi and Mac Roman. </p>

	<p>For non-Roman character set viewers, this method currently 
	supports only SHIFT-JIS encoding on a Japanese system. </p>

	@param infoArray An array specifying the type of each 
	character in the font. Each entry in this table must be 
	one of the Character Type Codes. If <code>infoArray</code> is set to 
	<code>NULL</code>, a default table is used. For non-UNIX Roman systems, 
	it is <code>WinAnsiEncoding</code> on Windows<code> and MacRomanEncoding</code> on Mac OS. 
	On UNIX (except HP-UX) Roman systems, it is <code>ISO8859-1</code> 
	(ISO Latin-1); for HP-UX, it is <code>HP-ROMAN8</code>. See Appendix 
	D in the <i>PDF Reference</i> for descriptions of <code>MacRomanEncoding</code> 
	and <code>WinAnsiEncoding</code>. 
	@param cNewWord (Filled by the method) The filtered word. 
	
	@param cOldWord The unfiltered word. This value must be 
	passed to the method.
	@return <code>true</code> if the string required filtering, <code>false</code> if 
	the filtered string is the same as the unfiltered string. 
	
	@see PDWordFilterWord 

	@note In Acrobat 6.0, the method PDWordFinderEnumWordsStr() 
	is preferred to this method, which remains for backward 
	compatability. 
  	@ref CharacterTypeCodes
	@since PI_PDMODEL_VERSION >= 0x00020000
*/
NPROC(ASBool, PDWordFilterString, (ASUns16 *infoArray, char *cNewWord, char *cOldWord))

/**
	Removes leading and trailing spaces and leading and trailing 
	punctuation (including soft hyphens) from the specified 
	word. It does not remove wildcard characters (<code>'*'</code> and <code>'?'</code>) 
	or any punctuation surrounded by alphanumeric characters 
	within the word. It also converts ligatures to their constituent 
	characters. The determination of which characters to remove 
	is made by examining the flags in the <code>outEncInfo</code> array passed 
	to PDDocCreateWordFinder(). As a result, this method is most 
	useful after you have been called with words obtained by 
	calling PDWordFinderGetNthWord(), in the callback for PDWordFinderEnumWords(), 
	and words in the pXYSortTable returned by PDWordFinderAcquireWordList(). 
	See the description of PDWordFilterString() for further information, 
	and for a description of how the two methods differ. 

	<p>The Acrobat Catalog program uses this method to filter words 
	before indexing them. </p>

	<p>This method works with non-Roman systems. </p>
	@param word The PDWord to filter. 
	@param buffer (Filled by the method) The filtered string. 
	
	@param bufferLen The maximum number of characters that 
	<code>buffer</code> can hold. 
	@param newLen (Filled by the method) The number of characters 
	actually written into <code>buffer</code>.
	@return <code>true</code> if the word required filtering, <code>false</code> if the 
	filtered string is the same as the unfiltered string. 
	@see PDWordFilterString 

	@note In Acrobat 6.0 and later, the method PDWordFinderEnumWords() is 
	preferred to this method, which remains for backward compatability. 
	@since PI_PDMODEL_VERSION >= 0x00020000
*/
NPROC(ASBool, PDWordFilterWord, (PDWord word, char *buffer, ASInt16 bufferLen, ASInt16 *newLen))


/**
	Gets the specified style's font. 
	@param obj IN/OUT The style whose font is obtained. 
	@return The font for the specified style. 
	@see PDStyleGetColor 
	@see PDStyleGetFontSize 
	@since PI_PDMODEL_VERSION >= 0x00020000
	
*/
NPROC(PDFont, PDStyleGetFont, (PDStyle obj))

/**
	Get a style's font size. 
	@param obj The style whose font size is obtained.
	@return The size of the font in points. 
	@see PDStyleGetColor 
	@see PDStyleGetFont 
	@since PI_PDMODEL_VERSION >= 0x00020000
*/
NPROC(ASFixed, PDStyleGetFontSize, ( PDStyle obj ))

/**
	Gets a style's color. 
	@param obj IN/OUT The style whose color is obtained. 
	@param color IN/OUT (Filled by the method) A pointer to a structure 
	that contains the style's color. 
	@see PDStyleGetFont 
	@see PDStyleGetFontSize 
	@since PI_PDMODEL_VERSION >= 0x00020000
	
*/
NPROC(void, PDStyleGetColor, ( PDStyle obj, PDColorValue color ))


/**
	Creates a new file specification from the specified ASPathName, 
	using the PDFileSpecNewFromASPathProc() of the specified file 
	system's file specification handler. 
	@param pdDoc The document in which the new file specification 
	will be used. 
	@param fileSys A pointer to an ASFileSysRec specifying the 
	file system responsible for the newly created file specification. 
	
	@param path The path to convert into a file specification. 
	
	@param relativeToThisPath A path name relative to which <code>path</code> 
	is interpreted. If it is <code>NULL</code>, <code>path</code> is interpreted as an absolute 
	path name, not a relative path name.
	@return The newly created file spec, or an invalid file spec if 
	the ASPathName cannot be converted to a PDFileSpec (use 
	PDFileSpecIsValid() to test whether the conversion was successful). 
	
	@see PDFileSpecIsValid 
	@since PI_PDMODEL_VERSION >= 0x00020000
*/
UNPROC(PDFileSpec, PDFileSpecNewFromASPath, (PDDoc pdDoc, ASFileSys fileSys, ASPathName path, ASPathName relativeToThisPath))

/**
	Converts an appropriate string or dictionary Cos object 
	to a file specification. This method does not copy the object, 
	but is instead the logical equivalent of a type cast. 
	@param obj IN/OUT The Cos object to convert to a file specification. 
	
	@return The file specification corresponding to <code>obj</code>. 
	@exception pdErrBadFileSpec is raised if the file specification is not 
	valid, as determined by PDFileSpecIsValid(). 
	@see PDFileSpecGetCosObj 
	@see PDFileSpecIsValid 
	@since PI_PDMODEL_VERSION >= 0x00020000
	
*/
NPROC(PDFileSpec, PDFileSpecFromCosObj, (CosObj obj))

/**
	Gets the file system that services the specified file specification. 
	
	@param fileSpec IN/OUT The file specification whose file system 
	is obtained. 
	@return The file system that services <code>fileSpec</code>. 
	@see PDFileSpecGetFileSysName 
	@since PI_PDMODEL_VERSION >= 0x00020000
	
*/
NPROC(ASFileSys, PDFileSpecGetFileSys, (PDFileSpec fileSpec))

/**
	Acquires an ASPathName for the specified file specification 
	and relative path. 
	@param fileSpec IN/OUT The file specification for which an ASPathName 
	is acquired. 
	@param relativeToThisPath IN/OUT A path name relative to which 
	the <code>fileSpec</code> is interpreted. If it is <code>NULL</code>, <code>fileSpec</code> is assumed 
	to be an absolute, not a relative, path. 
	@return The ASPathName corresponding to <code>fileSpec</code>. 

	<p>After you are done using the ASPathName, you must free it 
	using ASFileSysReleasePath(). </p>

	@see ASFileSysReleasePath 
	@since PI_PDMODEL_VERSION >= 0x00020000
	
*/
NPROC(ASPathName, PDFileSpecAcquireASPath, (PDFileSpec fileSpec, ASPathName relativeToThisPath))

/**
	Gets the Cos object associated with a file specification. 
	This method does not copy the object, but is instead the 
	logical equivalent of a type cast. 
	@param fileSpec IN/OUT The file specification whose Cos object 
	is obtained. 
	@return The string or dictionary Cos object corresponding to the 
	file specification. 
	@see PDFileSpecFromCosObj 
	@since PI_PDMODEL_VERSION >= 0x00020000
	
*/
NPROC(CosObj, PDFileSpecGetCosObj, (PDFileSpec fileSpec))

/**
	Tests whether a file specification is valid. This is intended 
	only to ensure that the file specification has not been deleted, 
	not to ensure that all necessary information is present 
	and valid. 
	@param fileSpec The file specification whose validity 
	is tested.
	@return <code>true</code> if <code>fileSpec</code> is valid, <code>false</code> otherwise. 
	@since PI_PDMODEL_VERSION >= 0x00020000
*/
NPROC(ASBool, PDFileSpecIsValid, (PDFileSpec fileSpec))

/**
	Registers a new file specification handler with the Acrobat 
	viewer. In version 3.0 and later of the Acrobat viewer, 
	use the PDRegisterFileSpecHandlerByName method instead. 
	
	@param contextFileSys The file system that specifies the 
	context in which the file specification handler is used. 
	This is the file system on which the PDF document resides. 
	It is sometimes necessary to use different file specification 
	handlers depending on the file system in which the document 
	is open. For example, when a document is opened in a web 
	browser, the Acrobat viewer may use the browser's HTTP stack 
	when it needs to use HTTP. When a document is opened outside 
	of the browser, however, the Acrobat viewer must use a different 
	HTTP stack. 
	@param fileSpecHandler A pointer to a structure that contains 
	the handler's callbacks. This structure must not be freed 
	after calling PDRegisterFileSpecHandler(). 
	@param fileSpecHandlerObj A pointer to user-supplied data 
	to pass to the file specification handler's callbacks each 
	time they are called.
	@exception genErrNoMemory 
	@see PDRegisterFileSpecHandlerByName 
	@since PI_PDMODEL_VERSION >= 0x00020000
*/
NPROC(void, PDRegisterFileSpecHandler, (ASFileSys contextFileSys, PDFileSpecHandler fileSpecHandler, void *fileSpecHandlerObj))

/**
	Gets the device-independent path name from a file specification. 
	
	@param fileSpec IN/OUT The file specification whose device-independent 
	path name is obtained. 
	@param buffer IN/OUT (Filled by the method) <code>NULL</code>-terminated device-
	independent path name. If <code>buffer</code> is <code>NULL</code>, the method simply 
	returns the length of the path name. 
	@param bufLen IN/OUT The length of <code>buffer</code> in bytes. If the device-
	independent path name is longer than this, only the first 
	<code>bufLen - 1</code> bytes are copied into <code>buffer</code>, plus a <code>NULL</code> at 
	the end of the buffer. 
	@return The number of characters (excluding the <code>NULL</code>) copied into <code>buffer</code>. 
	
	@since PI_PDMODEL_VERSION >= 0x00020000
	
*/
NPROC(ASInt32, PDFileSpecGetDIPath, (PDFileSpec fileSpec, char* buffer, ASInt32 bufLen))


/**
	Sets the values to use for displaying the calibrated color and 
	grayscale. These values are the chromaticities and gammas 
	of the phosphors in the monitor. 

	<p>These values do not necessarily correspond to the monitor 
	being used; it is the responsibility of the client that 
	sets these values to provide the correct values. </p>

	<p>These values are used for rendering if calibrated color 
	is enabled by the preferences file item avpDoCalibratedColor 
	(see AVAppSetPreference()). </p>

	@param colorCal A pointer to a structure that contains the 
	color calibration information. For RGB devices, the red, 
	green, and blue chromaticities and gammas are used; for 
	grayscale, <code>whiteChrom</code> and <code>greenGamma</code> are used.
	@return <code>true</code> if the values were successfully set and installed 
	for the currently active display device, <code>false</code> otherwise. 
	
	@see PDPrefGetColorCal 
	@see AVAppSetPreference 
	@since PI_PDMODEL_VERSION >= 0x00020000
*/
NPROC(ASBool, PDPrefSetColorCal, (PDColorCalP colorCal))

/**
	Gets the values to use for displaying the calibrated color and 
	grayscale. These values are the chromaticity and gammas 
	of the phosphors in the monitor. 

	<p>These values are used for rendering if calibrated color 
	is enabled by the preferences file item avpDoCalibratedColor 
	(see AVAppGetPreference()). </p>

	@param colorCal IN/OUT (Filled by the method) A pointer to a structure 
	that contains the color calibration information. For RGB 
	devices, the red, green, and blue chromaticity and gammas 
	are used; for grayscale, <code>whiteChrom</code> and <code>greenGamma</code> are 
	used. You must allocate storage for the <code>colorCal</code> data structure 
	and pass a pointer to the memory you allocated. 
	@return Always returns <code>true</code>. 
	@see PDPrefSetColorCal 
	@see AVAppGetPreference 
	@since PI_PDMODEL_VERSION >= 0x00020000
	
*/
NPROC(ASBool, PDPrefGetColorCal, (PDColorCalP colorCal))

/* Acrobat 2.1 additions */

/**
	Broadcasts a PDPageContentsDidChange() notification and a 
	PDPageContentsDidChangeEx() notification. These notify the 
	Acrobat viewer that a page's contents have been modified, 
	and tells the Acrobat viewer whether to redraw the page 
	immediately. 

	<p>You must use this method after using Cos methods to change 
	a page's contents. Do not use this method if you use PDPageAddCosContents() 
	or PDPageRemoveCosContents() to change a page's contents, 
	because those methods automatically generate the appropriate 
	notifications. </p>

	<p>If your plug-in must be compatible with version 2.0 of the 
	Acrobat viewer, you must use PDPageNotifyContentsDidChange() 
	instead. </p>
	@param page The page that changed. 
	@param invalidateViews <code>true</code> if the Acrobat viewer redraws 
	the page view, <code>false</code> otherwise. This allows plug-ins to 
	make a sequence of modifications to a page's contents, without 
	having the entire page flash after each modification. Passing 
	<code>true</code> for <code>invalidateViews</code> is equivalent to calling PDPageNotifyContentsDidChange().
	@notify PDPageContentsDidChange 
	@notify PDPageContentsDidChangeEx 
	@see PDPageAddCosContents 
	@see PDPageRemoveCosContents 
	@see PDPageNotifyContentsDidChange 
	@since PI_PDMODEL_VERSION >= 0x00020001
*/
NPROC(void, PDPageNotifyContentsDidChangeEx, (PDPage page, ASBool invalidateViews))

/**
	Clears flags associated with a document. This method is 
	most frequently used to mark a modified document as clean 
	(by clearing the PDDocNeedsSave flag) to avoid bringing 
	up the Save dialog box when the file is closed. 
	@param doc IN/OUT The document whose flags are cleared. 
	@param flags IN/OUT The flags to clear. It must be an <code>OR</code> of the PDDocFlags 
	values. 
	@see PDDocSave 
	@see PDDocClose 
	@see PDDocGetFlags 
	@see PDDocSetFlags 
	@since PI_PDMODEL_VERSION >= 0x00020001
	
*/
NPROC(void, PDDocClearFlags, (PDDoc doc, ASInt32 flags))

#if !MAC_PLATFORM || (MAC_PLATFORM && !AS_ARCH_64BIT)

/**
	Draws the specified stream of PDF marking operators into 
	the specified window. This method is used for platform-independent 
	drawing of graphics and text. This API is not available on Mac 64.

	<p>This method changes the current page's CTM and zoom factor, 
	and dirties the file. </p>
	@param cosObj The stream Cos object to draw into the window. 
	This stream can be created using CosNewStream(). The stream's 
	dictionary must contain a Resources key whose value is a 
	dictionary specifying all the resources needed to draw the 
	Cos object (including a ProcSet entry). Its structure and 
	contents are the same as for the Resources dictionary for 
	a Page object. See Section 3.7.2 in the <i>PDF Reference</i> for 
	a description of a Page object's Resources dictionary. The 
	stream's data is a sequence of PDF marking operators. See 
	Appendix A in the <i>PDF Reference</i> for a description of these 
	operators. A pseudocode example of the stream object is:
	
	<p><code><< /Length 1000 /Filter</code></p>
	<p><code>[...filters...] /Resources <<</code></p>
	<p><code>/ProcSet [ /PDF /Text ]</code></p>
	<p><code>/Font <</F5 6 0 R /F9 12 0 R>> >></code></p>
	<p><code>>></code></p>
	<p><code>stream</code></p>
	<p><code>...stream data...</code></p>
	<p><code>endstream </code></p>
	

	@param window A pointer to a platform-dependent window object 
	(<code>HWND</code> on Windows, <code>WindowPtr</code> or <code>CWindowPtr</code> on Mac OS). On Windows, 
	to draw into an offscreen <code>DC</code>, pass <code>NULL</code> for <code>window</code>. On 
	Mac OS, to draw into an off screen <code>GWorld</code>, pass <code>NULL</code> in 
	window and pass the <code>GWorld</code>Ptr in displayContext.
	@param displayContext A pointer to a platform-dependent 
	display context structure (<code>hDC</code> on Windows, <code>GWorldPtr</code> on Mac OS). 
	On Mac OS, <code>displayContext</code> is ignored if <code>windows</code> is non-<code>NULL</code>. 
	
	@param isDPS Currently unused. Always set it to <code>false</code>. 
	@param matrix A pointer to a matrix to concatenate onto 
	the default page matrix. It is useful for scaling and for 
	converting from page to window coordinates. 
	@param updateRect A pointer to a rectangle, specified in 
	user space coordinates. Any objects outside of <code>updateRect</code> 
	will not be drawn. All objects are drawn if <code>updateRect</code> is 
	<code>NULL</code>. 
	@param cancelProc A procedure called periodically to check 
	for the user's cancelling of the drawing operation. The default cancel 
	proc can be obtained using AVAppGetCancelProc(). It may be <code>NULL</code>, 
	in which case no cancel proc is used. 
	@param cancelProcClientData A pointer to user-supplied data 
	to pass to <code>cancelProc</code> each time it is called. It should be 
	<code>NULL</code> if <code>cancelProc</code> is <code>NULL</code>.
	@see CosNewStream 
	@see PDPageDrawContentsToWindow 
	@see PDPageDrawContentsToWindowEx 
	@since PI_PDMODEL_VERSION >= 0x00020001
*/
NPROC(void, PDDrawCosObjToWindow, (CosObj cosObj, void *window, void *displayContext,
	ASBool isDPS, ASFixedMatrix *matrix, ASFixedRect *updateRect, CancelProc cancelProc,
	void *cancelProcClientData))

#endif /* #if !MAC_PLATFORM || (MAC_PLATFORM && !AS_ARCH_64BIT) */ 


/* Acrobat 3.0 additions */

/**
	Opens the document specified by the ASFile. <code>aFile</code> must be 
	a valid ASFile. It is the caller's responsibility to dispose 
	of the ASFile after calling PDDocClose(). 

	<p>This method is useful when the document referenced by the 
	ASFile is not on the local machine, and is being retrieved 
	incrementally using the multi-read protocol of an ASFileSys. 
	If the bytes required to open a PDDoc are not yet available, 
	this method will raise the exception fileErrBytesNotReady. 
	The client should call PDDocOpenFromASFile() until this exception 
	is no longer raised. </p>

	@param aFile The ASFile to open. The ASFile should be 
	released after the PDDoc is closed. 
	@param authProc An authorization callback, called only if 
	the file is encrypted. This callback should obtain whatever 
	information is needed to determine whether the user is authorized 
	to open the file, then call PDDocAuthorize() (which returns 
	the permissions that the authentication data enables). The 
	Acrobat viewer's built-in authorization procedure requires 
	the user to enter a password, and allows the user to try 
	three times before giving up. 
	@param doRepair If <code>true</code>, attempt to repair the file if 
	it is damaged. If <code>false</code>, do not attempt to repair the file 
	if it is damaged.
	@return A valid PDDoc if successfully opened. 
	@exception pdErrNeedPassword is raised if the file is encrypted and <code>authProc</code> 
	is <code>NULL</code> or returns <code>false</code>. 
	@exception fileErrBytesNotReady is raised if the bytes required to open 
	a PDDoc are not yet available. 
	@see PDDocClose 
	@see PDDocOpen 
	@see PDDocOpenEx 
	@see PDDocOpenFromASFileEx 
	@since PI_PDMODEL_VERSION >= 0x00020002
*/
NPROC(PDDoc, PDDocOpenFromASFile, (ASFile aFile, PDAuthProc authProc, ASBool doRepair))

/**
	Gets the PDDoc that contains <code>fileSpec</code>. 
	@param fileSpec IN/OUT A PDFileSpec in a document. 
	@return A PDDoc or <code>NULL</code> if the file specification CosObj is not 
	in a document. 
	@since PI_PDMODEL_VERSION >= 0x00020002
	
*/
NPROC(PDDoc, PDFileSpecGetDoc, (PDFileSpec fileSpec))

/**
	Gets the name of the file system that a PDFileSpec belongs 
	to. For a simple <code>fileSpec</code> (string form), the name of the 
	file system is the name of the document's file system if 
	the CosObj that is the <code>fileSpec</code> is contained in a document. 
	For a complex <code>fileSpec</code> (dictionary form) with an FS key, the name 
	of the file system is the atom associated with the FS key. 
	
	<p>The file system returned by PDFileSpecGetFileSys() is the 
	file system that has registered a PDFileSpecHandler() for 
	the file specification's file system name (if there is one), and is not 
	necessarily the same as <code>ASFileGetFileSysByName(PDFileSpecGetFileSysName(fileSpec));</code> .</p>
	
	@param fileSpec A PDFileSpec.
	@return An ASAtom representing the file system of <code>fileSpec</code>. 
	@see PDFileSpecGetFileSys 
	@since PI_PDMODEL_VERSION >= 0x00020002
*/
NPROC(ASAtom, PDFileSpecGetFileSysName, (PDFileSpec fileSpec))

/**
	Registers a new file specification handler with the Acrobat 
	viewer. The viewer calls the appropriate file specification 
	handler when it encounters a file specification in a PDF 
	file. The appropriate file specification handler is the 
	one whose <code>specSysName</code> matches the value of the FS key in the file 
	specification and whose <code>contextFileSys</code> matches the file system on which the PDF 
	file resides.

	<p>The file specification handler's file system, (passed as 
	the <code>fileSys</code> field of <code>fileSpecHandler</code>), is used to obtain 
	data from, or write data to, the file referred to by the 
	file specification. </p>

	@param specSysName The name (as an ASAtom) of a file system 
	with which this file specification works. 
	@param contextFileSys The file system that specifies the 
	context in which the file specification handler is used. 
	This is the file system on which the PDF document resides. 
	It is sometimes necessary to use different file specification 
	handlers depending on the file system in which the document 
	is open. For example, when a document is opened in a web 
	broswer, the Acrobat viewer may use the browser's HTTP stack 
	when it needs to use HTTP. When a document is opened outside 
	of the browser, however, the Acrobat viewer must use a different 
	HTTP stack. 
	@param fileSpecHandler A pointer to a structure that contains 
	the handler's callbacks. This structure must not be freed 
	after calling PDRegisterFileSpecHandlerByName(). 
	@param fileSpecHandlerObj A pointer to user-supplied data 
	to pass to the file specification handler's callbacks each 
	time they are called.
	@exception genErrNoMemory 
	@see PDRegisterFileSpecHandler 
	@since PI_PDMODEL_VERSION >= 0x00020002
*/
NPROC(void, PDRegisterFileSpecHandlerByName, (ASAtom specSysName, ASFileSys contextFileSys, PDFileSpecHandler fileSpecHandler, void *fileSpecHandlerObj))

/**
	Reads a PDF page content token from a stream. The stream 
	is typically obtained by getting the Cos stream for a page 
	contents or a Form contents, and calling CosStreamOpenStm() 
	to open the stream using the <i>filtered</i> mode. 

	<p>It begins reading at the current stream position, and reads 
	exactly one token. It returns the number of bytes read. 
	This is the number of bytes read from the stream and indicates 
	the amount by which the stream position has advanced. The 
	end-of-stream criteria (loop terminating condition) is the 
	following:</p>
	
	<ul>
	<li>A <code>NULL</code> object is returned (an object of type CosNull). </li>
	<li>The number of bytes read (return value) is <code>1</code>. </li>
	</ul>
	
	<p>If the token is an integer, real (ASFixed), or ASBool, then 
	the value is returned in <code>pageStmToken.iVal</code>. If the token 
	is a string or a name, the value is returned in <code>pageStmToken.sVal</code>, 
	and the length of the token is in <code>pageStmToken.sValLen</code>. 
	Strings are not <code>NULL</code>-terminated, but names are <code>NULL</code>-terminated. 
	If a string length is greater than kPDPageStmStringMax, 
	the PDPageStmStringOverflowProc() is called repeatedly with 
	portions of the string. On return from PDPageStmGetToken, 
	the value of <code>pageStmToken.sValLen</code> is zero, and <code>pageStmToken.sVal</code> 
	is empty (<code>ival</code>, <code>sVal</code>, and <code>sValLen</code> are components of the PDPageStmToken). 
	If there is no overflow proc, then the first kPDPageStmStringMax 
	bytes of the string will be returned in <code>pageStmToken.sVal</code>, 
	and the remaining bytes are lost. The value of <code>pageStmToken.sValLen</code> 
	is kPDPageStmStringMax in this case. </p>

	<p>If the token is BI (begin inline image), PDPageStmGetInlineImage() 
	should be called to parse the inline image. </p>

	<p>This method can raise memory, I/O, and parsing exceptions.</p>

	@param stm The stream from which data is read. 
	@param flags A bit field of options such as 'skip comments' 
	(kPDPageStmSkipComments means skip comments during 
	token generation). 
	@param proc A callback method to handle long strings. 
	@param procClientData Client data passed to the callback 
	method. 
	@param pageStmToken The returned token.
	@return The number of bytes read from <code>stm</code>. 
	@see CosStreamOpenStm 
	@see PDPageGetCosObj 
	@see PDPageStmGetInlineImage 
	@since PI_PDMODEL_VERSION >= 0x00020002
*/
NPROC(ASUns32,PDPageStmGetToken, (
						ASStm stm,
						ASUns32 flags,
						PDPageStmStringOverflowProc proc,
						void *procClientData,
						PDPageStmToken pageStmToken))

/**
	Reads a PDF page content inline image from a stream. The 
	stream is typically obtained by getting the Cos stream for 
	a page contents or a Form contents, and calling CosStreamOpenStm() 
	to open the stream using the <i>filtered</i> mode. This method 
	is called after a BI token has been read from the stream. 
	BI indicates that the following tokens comprise an inline 
	image dictionary and data. 

	<p>It begins reading at the current stream position. It returns the 
	number of bytes read. This is the number of bytes read from 
	the stream and indicates the amount by which the stream 
	position has advanced. </p>

	<p>The image attributes dictionary is returned in <code>imageDict</code>. 
	The image data is passed to the PDPageStmImageDataProc(); 
	if <code>proc</code> is not provided, the image data is discarded.</p> 

	<p><code>imageRawDataStmOffsetP</code> and <code>imageRawDataLenP</code> may be <code>NULL</code>, 
	in which case they are ignored. </p>

	<p>The caller should call CosObjDestroy() on <code>imageDict</code> when it is done. </p>

	<p>	This method can raise memory, I/O, and parsing exceptions. </p>

	@param stm The stream from which data is read. 
	@param cosDoc The CosDoc with the PDPage that contains 
	the inline image. 
	@param resDict The Resources dictionary in which to look up 
	ColorSpace resources for inline images. 
	@param flags Currently unused by this method (used by 
	PDPageStmGetToken().) 
	@param proc A callback method to handle inline image data. 
	
	@param procClientData Client data passed to <code>proc</code>. 
	@param imageRawDataStmOffsetP (Filled by the method) The offset 
	of the data stream, after BI, relative to the beginning 
	of <code>stm</code>. 
	@param imageRawDataLenP (Filled by the method) The offset 
	of the last byte of the data stream between the BI and EI 
	PDF operators. 
	@param imageDict (Filled by the method) The returned image 
	dictionary.
	@return The number of bytes read from <code>stm</code>. 
	@see CosStreamOpenStm 
	@see PDPageGetBox 
	@see PDPageStmGetToken 
	@since PI_PDMODEL_VERSION >= 0x00020002
*/
NPROC(ASUns32, PDPageStmGetInlineImage, (
						ASStm stm,
						ASUns32 flags,
						CosDoc cosDoc,
						CosObj resDict,
						PDPageStmImageDataProc proc,
						void *procClientData,
						ASUns32 *imageRawDataStmOffsetP,
						ASUns32 *imageRawDataLenP,
						CosObj *imageDict))

/**
	Resolves a destination. <code>dest</code> is the value of the D key in 
	an action. It can be a real destination (an array) or a 
	name. If it is a name, look it up in the <code>doc</code> parameter's Dests dictionary. 
	The value found there can be a real destination (an array) or a 
	dictionary. If it is a dictionary, look up the D key in that 
	dictionary. 

	<p>This method is useful for getting a PDViewDestination from 
	an action, as provided by PDActionGetDest(), since this method 
	may not return a PDViewDestination. </p>

	<p>	This method can raise memory, I/O, and parsing exceptions. </p>

	@param dest IN/OUT The destination to resolve. 
	@param doc IN/OUT The PDDoc that contains the destination. 
	@return The resolved view destination. 
	@see PDActionGetDest 
	@since PI_PDMODEL_VERSION >= 0x00020002
	
*/
NPROC(PDViewDestination, PDViewDestResolve, (
						PDViewDestination dest,
						PDDoc doc))
/* Transition API's */

/**
	Tests whether a transition is valid, meaning that the transition 
	has not been deleted. 
	@param pdt The transition dictionary whose validity is 
	tested.
	@return <code>true</code> if the transition is valid, <code>false</code> otherwise. 
	
	@see PDTransEqual 
	@since PI_PDMODEL_VERSION >= 0x00020002
*/
NPROC(ASBool, PDTransIsValid, (PDTrans pdt))

/**
	Gets a <code>NULL</code> transition. This can be used in conjunction 
	with PDTransEqual() to determine whether a transition is <code>NULL</code>. 
	
	@return A <code>NULL</code> transition. 
	@see PDTransNew 
	@since PI_PDMODEL_VERSION >= 0x00020002
	
*/
NPROC(PDTrans, PDTransNull, (void))

/**
	Converts the specified dictionary Cos object to a transition 
	and verifies that the transition is valid. This method does 
	not copy the object but is instead the logical equivalent 
	of a type cast. 
	@param coLayer The dictionary Cos object to convert to a transition. 
	@return The transition corresponding to the given dictionary object, 
	<code>obj</code>. 
	@exception pdErrBadBaseObj is raised if the transition is not valid as 
	determined by PDTransIsValid(). 
	@see PDTransGetCosObj 
	@see PDTransIsValid 
	@since PI_PDMODEL_VERSION >= 0x00020002
	
*/
NPROC(PDTrans, PDTransFromCosObj, (CosObj coLayer))

/**
	Gets the dictionary Cos object corresponding to the transition 
	and verifies that the transition is valid. This method does 
	not copy the object but is the logical equivalent of a type 
	cast. 
	@param pdl The transition whose Cos object is obtained. 
	@return The dictionary Cos object corresponding to the transition. 
	it returns the <code>NULL</code> Cos object if the transition is invalid 
	as determined by PDTransIsValid(). 
	@see PDTransFromCosObj 
	@see PDTransIsValid 
	@since PI_PDMODEL_VERSION >= 0x00020002
	
*/
NPROC(CosObj, PDTransGetCosObj, (PDTrans pdl))

/**
	Tests two transitions for equality. Two transitions are 
	equal only if their Cos objects are equal (see CosObjEqual()). 
	
	@param pdtOne A transition to test for equality.
	@param pdtTwo A transition to test for equality.
	@return <code>true</code> if the transitions are equal, <code>false</code> otherwise. 
	
	@see CosObjEqual 
	@since PI_PDMODEL_VERSION >= 0x00020002
*/
NPROC(ASBool, PDTransEqual, (PDTrans pdtOne, PDTrans pdtTwo))

/* OBSOLETE! Document level transition has/get/set routines  */
NOPROC(PDDocHasTransition)
NOPROC(PDDocGetTransition)
NOPROC(PDDocSetTransition)

/* Page level transition set/get routines. */

/**
	Tests whether a page has a transition. 
	@param pdp The page to test.
	@return <code>true</code> if the page has a transition, <code>false</code> otherwise. 
	
	@since PI_PDMODEL_VERSION >= 0x00020002
*/
NPROC(ASBool, PDPageHasTransition, (PDPage pdp))

/**
	Gets the transition for a given page. 
	@param pdp The page whose transition is obtained. 
	@return The page's transition. If the page has no transition, it returns 
	a <code>NULL</code> transition. 
	@see PDPageSetTransition 
	@see PDTransNull 
	@since PI_PDMODEL_VERSION >= 0x00020002
	
*/
NPROC(PDTrans, PDPageGetTransition, (PDPage pdp))

/**
	Sets the transition for a given page. 
	@param pdp The page whose transition is set. 
	@param pdt The transition for the page. 
	@see PDPageGetTransition 
	@see PDTransNull 
	@since PI_PDMODEL_VERSION >= 0x00020002
	
*/
UNPROC(void, PDPageSetTransition, (PDPage pdp, PDTrans pdt))

/**
	Gets the page's automatic-advance timing value, which is the maximum 
	amount of time the page is displayed before the viewer automatically 
	advances to the next page. 
	@param pdp The page whose timing value is obtained. 
	@return The automatic-advance timing for the page, in seconds. If 
	the page does not have an advance timing value, fxDefaultPageDuration is returned
	(representing positive infinity, meaning that it never advances). 
	@see PDPageSetDuration 
	@since PI_PDMODEL_VERSION >= 0x00020002
	
*/
NPROC(ASFixed, PDPageGetDuration, (PDPage pdp))

/**
	Sets the page's automatic-advance timing value, which is the maximum 
	amount of time the page is displayed before the viewer automatically 
	advances to the next page. 
	@param pdp The page whose timing is set. 
	@param fxDuration The auto-advance timing, in seconds. 
	If no advance timing is desired, <code>fxDuration</code> should be set 
	to fxDefaultPageDuration. 
	@see PDPageGetDuration 
	@since PI_PDMODEL_VERSION >= 0x00020002
	
*/
UNPROC(void, PDPageSetDuration, (PDPage pdp, ASFixed fxDuration))

/* Creation, set/get methods. */

/**
	Creates a new transition of the specified type and duration 
	associated with the given CosDoc. 
	@param cd The CosDoc to which the transition is added. 
	
	@param asaSubtype The transition subtype to create. This 
	subtype is returned by the AVTransHandlerGetTypeProc() callback 
	in AVTransHandler. 
	@param fxDuration The transition duration, in seconds.
	@return The newly created transition. 
	@exception pdErrBadBaseObj is raised if the transition is not valid as 
	determined by PDTransIsValid(). 
	@see PDTransNew 
	@since PI_PDMODEL_VERSION >= 0x00020002
*/
NPROC(PDTrans, PDTransNewFromCosDoc, (CosDoc cd, ASAtom asaSubtype, ASFixed fxDuration))

/**
	Creates a new transition of the specified type and duration 
	associated with the CosDoc of the given PDDoc. 
	@param pdd The PDDoc to whose CosDoc the transition is 
	added. 
	@param asaSubtype The transition subtype to create. It must 
	be one of the transition effects described in Section 8.3.3 
	in the <i>PDF Reference</i>. All implementations that support transitions 
	are required to support these transitions at a minimum. 
	Plug-ins can register new types or provide a new handler 
	for an existing type. 
	@param fxDuration The transition duration, in seconds.
	@return The newly created transition. 
	@exception pdErrBadBaseObj is raised if the transition is not valid as 
	determined by PDTransIsValid(). 
	@see PDTransNewFromCosDoc 
	@since PI_PDMODEL_VERSION >= 0x00020002
*/
UNPROC(PDTrans, PDTransNew, (PDDoc pdd, ASAtom asaSubtype, ASFixed fxDuration))

/**
	Gets a transition's subtype. 
	@param pdt IN/OUT The transition whose subtype is obtained. 
	@return The ASAtom for the transition's subtype. It can be converted 
	to a string using ASAtomGetString(). 
	@see PDTransGetDuration 
	@since PI_PDMODEL_VERSION >= 0x00020002
	
*/
NPROC(ASAtom, PDTransGetSubtype, (PDTrans pdt))

/**
	Gets the duration for the given transition. 
	@param pdt IN/OUT The transition for which the duration is obtained. 
	
	@return The transition duration, specified in seconds. If no duration 
	is specified in the transition, the return value is fxDefaultTransDuration 
	(<code>1</code> second).

	@note Standard durations are:

	<TABLE rules="all" cellspacing="1">
	<TR><TH>Duration</TH><TH>Meaning</TH></TR>
	<TR><TD>0 seconds</TD><TD>fast</TD></TR>
	<TR><TD>1 second</TD><TD>medium</TD></TR>
	<TR><TD>2 seconds</TD><TD>slow</TD></TR>
	</TABLE>

	@see PDTransGetSubtype 
	@since PI_PDMODEL_VERSION >= 0x00020002
	
*/
NPROC(ASFixed, PDTransGetDuration, (PDTrans pdt))
NOPROC(PDDocSetTextSelectVertical)

/**
	Used for page-at-a-time downloading and byte-serving Acrobat 
	data. If a document is being viewed over a slow file system, 
	PDDocReadAhead() issues a byte range request for all the data 
	associated with the flags in flags. 
	@param doc IN/OUT The document being read. 
	@param flags IN/OUT Flags describing type of data to read ahead. 
	It must be an <code>OR</code> of flags in PDDocReadAhead() Flags. 
	@param clientData IN/OUT Currently unused. 
	@since PI_PDMODEL_VERSION >= 0x00020002
	
*/
NPROC(void, PDDocReadAhead, (PDDoc doc, ASUns32 flags, void * clientData))

/**
	Opens the specified document. If the document is already 
	open, it returns a reference to the already opened PDDoc. You 
	must call PDDocClose() once for every successful open. If 
	the call fails and the exception is pdErrNeedRebuild, then 
	call again with <code>doRepair</code> equal to <code>true</code>. This allows the application 
	to decide whether to perform the time-consuming repair operation. 
	
	@param fileName A path name to the file, specified in whatever 
	format is correct for <code>fileSys</code>. 
	@param fileSys A pointer to an ASFileSysRec containing the 
	file system in which the file resides. 
	@param authProcEx An authorization callback, called only 
	if the file has been secured (meaning that the file has either 
	the user or the master password set). This callback should 
	obtain whatever information is needed to determine whether 
	the user is authorized to open the file, then call PDDocAuthorize()() 
	(which returns the permissions that the authentication data 
	enables). The Acrobat viewer's built-in authorization procedure 
	requires the user to enter a password, and allows the user 
	to try three times before giving up. 
	@param authProcClientData A pointer to user-supplied data 
	to pass to authProcEx() each time it is called. 
	@param doRepair If <code>true</code>, attempt to repair the file if 
	it is damaged. If <code>false</code>, do not attempt to repair the file 
	if it is damaged.
	@return The newly-opened document. 
	@exception pdErrNeedPassword is raised if the file is encrypted and <code>authProc</code>Ex() 
	is <code>NULL</code> or returns <code>false</code>. 
	@exception pdErrNotEnoughMemoryToOpenDoc or genErrNoMemory is raised if 
	there is insufficient memory to open the document. 
	@exception pdErrNeedRebuild is raised if the document needs to be rebuilt 
	and <code>doRepair</code> is <code>false</code>. 
	@exception pdErrBadOutlineObj is raised if the Outlines object appears 
	to be invalid (if the value of the Outlines key in the Catalog is not a <code>NULL</code> or dictionary 
	object). 
	@exception pdErrBadRootObj is raised if the Catalog object (as returned 
	by CosDocGetRoot()) is not a dictionary. 
	@exception pdErrBadBaseObj is raised if the Pages tree appears to be invalid 
	(if the value of the Pages key in the Catalog is not a <code>NULL</code> or dictionary object). 
	
	@exception pdErrTooManyPagesForOpen is raised if the document contains 
	too many pages. 
	@exception cosSynErrNoHeader is raised if the document's header appears 
	to be bad. 
	@exception cosSynErrNoStartXRef is raised if no end-of-file line can be 
	located. 
	@exception cosErrRebuildFailed is raised if <code>doRepair</code> is <code>true</code> and rebuild 
	failed. 
	@see PDDocClose 
	@see PDDocCreate 
	@see PDDocAuthorize 
	@see PDDocOpen 
	@see PDDocOpenFromASFile 
	@see PDDocOpenFromASFileEx 
	@since PI_PDMODEL_VERSION >= 0x00020002
*/
NPROC(PDDoc, PDDocOpenEx, (ASPathName fileName, ASFileSys fileSys,PDAuthProcEx authProcEx, void *authProcClientData, ASBool doRepair))

/**
	Opens the document specified by the ASFile. <code>aFile</code> must be 
	a valid ASFile. It is the caller's responsibility to dispose 
	of the ASFile after calling PDDocClose(). 

	<p>This method is useful when the document referenced by the 
	ASFile is not on the local machine, and is being retrieved 
	incrementally using the multiread protocol of an ASFileSys. 
	If the bytes required to open a PDDoc are not yet available, 
	this method will raise the exception fileErrBytesNotReady. 
	The client should call PDDocOpenFromASFile() until this exception 
	is no longer raised. </p>

	@param aFile The ASFile to open. The ASFile should be 
	released after the PDDoc is closed. 
	@param authProcEx An authorization callback, called only 
	if the file is encrypted. This callback should obtain whatever 
	information is needed to determine whether the user is authorized 
	to open the file, then call PDDocAuthorize() (which returns 
	the permissions that the authentication data enables). The 
	Acrobat viewer's built-in authorization procedure requires 
	the user to enter a password, and allows the user to try 
	three times before giving up. 
	@param authProcClientData A pointer to user-supplied data 
	to pass to authProcEx() each time it is called. 
	@param doRepair If <code>true</code>, attempt to repair the file if 
	it is damaged. If <code>false</code>, do not attempt to repair the file 
	if it is damaged.
	@return A valid PDDoc if successfully opened. 
	@exception pdErrNeedPassword is raised if the file is encrypted and <code>authProc</code> 
	is <code>NULL</code> or returns <code>false</code>. 
	@exception fileErrBytesNotReady is raised if the bytes required to open 
	a PDDoc are not yet available. 
	@see PDDocClose 
	@see PDDocOpen 
	@see PDDocOpenEx 
	@see PDDocOpenFromASFile 
	@since PI_PDMODEL_VERSION >= 0x00020002
*/
NPROC(PDDoc, PDDocOpenFromASFileEx, (ASFile aFile, PDAuthProcEx authProcEx, void *authProcClientData, ASBool doRepair))

#ifndef ANDROID_ENV
/**
	Registers a new security handler with the Acrobat viewer. 
	It is the same as PDRegisterCryptHandler() except that it accepts a 
	client data parameter. 
	@param handler A pointer to a structure that contains the 
	security handler's callback functions. This structure must 
	not be freed after calling PDRegisterCryptHandlerEx(). 
	@param pdfName The name of the security handler as it 
	will appear in the PDF file. This name is also used by PDDocSetNewCryptHandler(). 
	Storage for this name can be freed after PDRegisterCryptHandlerEx() 
	has been called. 
	@param userName The name of the security handler as it 
	will be shown in menus. This name can be localized to different 
	languages. Storage for this name can be freed after PDRegisterCryptHandlerEx() 
	has been called. 
	@param clientData A pointer to user-supplied data to store 
	with the handler.
	@exception genErrBadParm is raised if the security handler's size field 
	is incorrect. 
	@exception ErrSysPDSEdit is raised if either pdfName or userName are already 
	in use by a registered security handler. 
	@exception genErrNoMemory is raised is memory is exhausted.
	@see PDDocSetNewCryptHandler 
	@see PDDocPermRequest 
	@see PDRegisterCryptHandler 
	@since PI_PDMODEL_VERSION >= 0x00020002
*/
NPROC(void, PDRegisterCryptHandlerEx, (PDCryptHandler handler, const char *pdfName, const char *userName, void *clientData))

/**
	Gets the client data for the encryption handler associated 
	with the PDDoc. This is the client data provided as a parameter 
	in PDRegisterCryptHandlerEx(). 
	@param pdDoc The document whose encryption handler client 
	data is obtained.
	@return Client data for the encryption handler associated with the 
	PDDoc. It returns <code>NULL</code> if there is no encryption handler or 
	no client data was provided. 
	@see PDRegisterCryptHandlerEx 
	@since PI_PDMODEL_VERSION >= 0x00020002
*/
NPROC(void *, PDDocGetCryptHandlerClientData, (PDDoc pdDoc))
#endif
/**
	Tests whether the document will open in full-screen mode. 
	This provides an alternative to calling PDDocGetPageMode() 
	to test for PDFullScreen. 
	@param pdDoc The document to test.
	@return <code>true</code> if the PDDoc is in full-screen mode, <code>false</code> 
	otherwise. 
	@see PDDocSetFullScreen 
	@since PI_PDMODEL_VERSION >= 0x00020002
*/
NPROC(ASBool, PDDocGetFullScreen, (PDDoc pdDoc))

/**
	Sets whether this document opens in full-screen mode. This 
	provides an alternative to calling PDDocSetPageMode() with 
	PDFullScreen. 
	@param pdDoc The document to set. 
	@param fs <code>true</code> if the document is set to open in full-screen 
	mode, <code>false</code> otherwise.
	@see PDDocGetFullScreen 
	@since PI_PDMODEL_VERSION >= 0x00020002
*/
UNPROC(void, PDDocSetFullScreen, (PDDoc pdDoc, ASBool fs))

/**
	Saves a document to disk as specified in a parameter's structure. 
	This is essentially the same as PDDocSave() with two additional 
	parameters: a cancel proc and cancel proc client data (so 
	you could cut and paste description information and other 
	information from PDDocSave()). 

	<p>You can replace this method with your own version, using 
	HFTReplaceEntry(). </p>
	@ingroup ReplaceableMethods
	@param doc The document to save. 
	@param inParams A PDDocSaveParams structure specifying how 
	the document should be saved.
	@see PDDocSave 

	@note Saving a PDDoc invalidates all objects derived from 
	it. See PDDocSave() for important information about releasing 
	objects that you may have acquired or used from a PDDoc 
	before it is saved. 
	@note Not replaceable in Adobe Reader.
	@since PI_PDMODEL_VERSION >= 0x00020002
*/
UPROC(void, PDDocSaveWithParams, (PDDoc doc, PDDocSaveParams inParams))

/**
	Given a name tree (such as the Dests tree in the Names dictionary) 
	and a string, find the CosObj in the tree that matches the 
	string. 

	<p>See Section 3.8.5 in the <i>PDF Reference</i> for more information 
	on name trees. </p>
	@param nameTree The name tree in which to search. 
	@param string The name to search for. The name tree uses 
	Cos-style strings, which may use Unicode encoding, and these 
	may contain bytes with zeroes in them (high bytes of ASCII 
	characters). Note that <code>name</code> is not a C-style string. Cos string 
	objects can contain <code>NULL</code> chars. Standard C string-handling 
	functions may not work as expected. 
	@param stringLen The length of <code>name</code> in bytes.
	@return The Cos object associated with the specified name, which 
	is the array element following the name. 
	@see PDNameTreeGet 
	@see PDNameTreePut 
	@since PI_PDMODEL_VERSION >= 0x00020002
*/
NPROC(CosObj, PDNameTreeLookup, (CosObj nameTree, char *string, ASInt32 stringLen))

/* Support for CJK viewers */

/**
	Gets a Type 0 font's descendant, which may be a CIDType0 
	or CIDType2 font. 

	<p>Type 0 fonts support single-byte or multi-byte encodings 
	and can refer to one or more descendant fonts. These fonts 
	are analogous to the Type 0 or composite fonts supported 
	by Level 2 PostScript interpreters. However, PDF Type 0 
	fonts only support character encodings defined by a CMap. 
	The CMap specifies the mappings between character codes 
	and the glyphs in the descendant fonts. </p>

	<p>For information on Type 0 fonts, see Section 5.6 in the 
	<i>PDF Reference</i>. See Section 5.6.4 for more details on CMaps. </p>
	
	@param font IN/OUT The font whose descendant is obtained. 
	@return The font's descendant font. 
	@see PDFontGetCIDSystemInfo 
	@see PDFontGetCIDSystemSupplement 
	@since PI_PDMODEL_VERSION >= 0x00020003
	
*/
SPROC(PDFont, PDFontGetDescendant, (PDFont font), PDFontGetDescendantInt)

/**
	Gets a string representing a font's encoding. 

	<p>Use PDFontGetEncodingIndex() to get encoding information for 
	Roman viewers. </p>

	<p>Host encoding is a platform-dependent encoding for the host 
	machine. For non-UNIX Roman systems, it is <code>WinAnsiEncoding</code> on Windows and <code>MacRomanEncoding</code> 
	on Mac OS. On UNIX (except HP-UX) Roman systems, it is <code>ISO8859-1</code> (ISO Latin-1); for 
	HP-UX, it is <code>HP-ROMAN8</code>. See Appendix D in the <i>PDF Reference</i> 
	for descriptions of <code>WinAnsiEncoding</code>, <code>MacRomanEncoding</code>, and 
	<code>PDFDocEncoding</code>. </p>

	<p>For non-Roman systems, the host encoding may be a variety 
	of encodings, which are defined by a CMap (character map). 
	See Section 5.6.4 in the <i>PDF Reference</i> for a list of predefined 
	CMaps. In this case, the encoding string contains values 
	such as <code>"90ms-RKSJ-H"</code>, <code>"90msp-RKSJ-H"</code>, <code>"Identity-V"</code>, or 
	<code>"90pv-RKSJ-H"</code>; it does not return a string like <code>"Shift-JIS"</code>. </p>
	
	@param font The font whose encoding name is obtained.
	@return The string representing the font's encoding. 
	@see PDFontGetEncodingIndex 
	@see PDGetHostEncoding 
	@since PI_PDMODEL_VERSION >= 0x00020003
*/
NPROC(const ASUns8*, PDFontGetEncodingName, (PDFont font))

/**
	Gets an ASAtom representing Registry and Ordering for a 
	CIDFont. This information resides in the CIDSystemInfo entry 
	of the CIDFont dictionary, which describes a CIDFont. 

	<p>PDFontGetCIDSystemInfo() takes either a Type 0 font or a descendant 
	font (CIDType0 or CIDType2) as an argument. This information 
	is always present for any Type 0 font; the actual registry 
	ordering information is a part of the descendant font. </p>

	<p>This method provides one way to identify a font's language. </p>
	
	<p>The CIDSystemInfo entry uses three components to identify 
	a character collection uniquely:</p>
	<ul>
	<li>A registry name to identify an issuer of ordering information. </li>
	<li>An ordering name to identify an ordered character collection. </li>
	<li>A supplement number to indicate that the ordered character 
	collection for a registry, ordering, and previous supplement 
	has been changed to add new characters assigned CIDs beginning 
	with the next available CID. </li>
	</ul>
	
	<p>The PDFontGetCIDSystemInfo() method obtains the first two 
	of these components. </p>

	<p>A CIDFont is designed to contain a large number of glyph 
	procedures. Instead of being accessed by a name, each glyph 
	procedure is accessed by an integer known as a character 
	identifier or CID. Instead of a font encoding, CIDFonts 
	use a CMap with a Type 0 composite font to define the mapping 
	from character codes to a font number and a character selector. </p>
	

	<p>For more information on Type 0 fonts, CIDFonts, and CMaps, 
	see Section 5.6 in the <i>PDF Reference</i>. For detailed information 
	on CIDFonts, see <i>Technical Note # 5092, CID-Keyed Font Technology 
	Overview</i>, and <i>Technical Note # 5014, Adobe CMap and CIDFont 
	Files Specification</i>. </p>
	@param font IN/OUT The font whose Registry and Ordering information 
	is obtained. 
	@return The ASAtom representing the CIDFont's Registry and Ordering 
	information (for example, <code>"Adobe-Japan1"</code>). 
	@see PDFontGetCIDSystemSupplement 
	@see PDFontGetDescendant 
	@since PI_PDMODEL_VERSION >= 0x00020003
	
*/
NPROC(ASAtom, PDFontGetCIDSystemInfo, (PDFont font))

/**
	Gets the SystemSupplement number of a CIDFont. This field 
	resides in the CIDSystemInfo entry of the CIDFont dictionary, 
	which describes a CIDFont. 

	<p>The CIDSystemInfo entry uses three components to identify 
	a character collection uniquely:</p>
	<ul>
	<li>A registry name to identify an issuer of orderings. </li>
	<li>An ordering name to identify an ordered character collection. </li>
	<li>A supplement number to indicate that the ordered character 
	collection for a registry, ordering, and previous supplement 
	has been changed to add new characters assigned CIDs beginning 
	with the next available CID. </li>
	</ul>
	
	<p>PDFontGetCIDSystemInfo() provides character collection information, 
	and PDFontGetCIDSystemSupplement() specifies the version of 
	the ordering. </p>

	<p>A CIDFont is designed to contain a large number of glyph 
	procedures. Instead of being accessed by a name, each glyph 
	procedure is accessed by an integer known as a character 
	identifier or CID. Instead of a font encoding, CIDFonts 
	use a CMap with a Type 0 composite font to define the mapping 
	from character codes to a font number and a character selector. </p>
	
	<p>For more information on Type 0 fonts, CIDFonts, and CMaps, 
	see Section 5.6 in the <i>PDF Reference</i>. For detailed information 
	on CIDFonts, see <i>Technical Note # 5092, CID-Keyed Font Technology 
	Overview</i>, and <i>Technical Note # 5014, Adobe CMap and CIDFont 
	Files Specification</i>. </p>
	@param font IN/OUT The font whose <code>SystemSupplement</code> field is obtained. 
	
	@return The <code>SystemSupplement</code> field from the CIDFont. 
	@see PDFontGetDescendant 
	@see PDFontGetCIDSystemInfo 
	@since PI_PDMODEL_VERSION >= 0x00020003
	
*/
NPROC(ASInt32, PDFontGetCIDSystemSupplement, (PDFont font))

/**
	Translates a string from Unicode or <code>PDFDocEncoding</code> to host 
	encoding. This method is useful when setting or retrieving 
	displayed text that might be in Unicode, such as text that 
	appears in a text annotation or bookmark. 

	<p>A character that cannot be converted to the destination 
	encoding is replaced with a space.</p> 

	<p>Host encoding is a platform-dependent encoding for the host 
	machine. For non-UNIX Roman systems, it is <code>WinAnsiEncoding</code> on Windows and <code>MacRomanEncoding</code> 
	on Mac OS. On UNIX (except HP-UX) Roman systems, it is <code>ISO8859-1</code> (ISO Latin-1); for 
	HP-UX, it is <code>HP-ROMAN8</code>. See Appendix D in the <i>PDF Reference</i> 
	for descriptions of <code>WinAnsiEncoding</code>, <code>MacRomanEncoding</code>, and 
	<code>PDFDocEncoding</code>. For non-Roman systems, the host encoding 
	may be a variety of encodings, which are defined by a CMap 
	(character map). See Section 5.6.4 in the <i>PDF Reference</i> 
	for information on CMaps. </p>

	<p>For non-Roman systems, use PDXlatetoHostEx(). Use PDGetHostEncoding() 
	to determine whether the host encoding for a system is Roman. </p>

	<p>In general, PDXlatetoHostEx() operates in the same way as PDXlateToHost() 
	but requires an extra argument, since the sizes of the input 
	and translated strings may differ. This method can be called 
	instead of PDXlateToHost(), and must be called for multi-byte character set systems. </p>

	@param inPdfStr IN/OUT A pointer to the string to translate (it may 
	point to the same memory as <code>outHostStr</code>, allowing strings 
	to translate in place). 
	@param inPdfStrSize IN/OUT The length of <code>inPdfStr</code> in bytes. 
	@param outHostStr IN/OUT (Filled by the method) A pointer to the 
	translated string (it may point to the same memory as <code>inPdfStr</code>). 
	
	@param outHostStrSize IN/OUT The length of the <code>outHostStr</code> buffer in bytes. 
	@return The number of bytes in the translated string <code>outHostStr</code>. 
	@see PDFontXlateToHost 
	@see PDFontXlateToUCS 
	@see PDGetHostEncoding 
	@see PDXlateToHost 
	@see PDXlateToPDFDocEnc 
	@see PDXlateToPDFDocEncEx 
	@since PI_PDMODEL_VERSION >= 0x00020003
	
*/
NPROC(ASInt32, PDXlateToHostEx, (const char* inPdfStr, ASInt32 inPdfStrSize, char* outHostStr, ASInt32 outHostStrSize ))

/**
	Translates a string from host encoding to <code>PDFDocEncoding</code> 
	or Unicode. This method is useful when using text that must 
	be in <code>PDFDocEncoding</code> or Unicode, such as text in a text 
	annotation, bookmark, or article title. 

	<p>A character that cannot be converted to the destination 
	encoding is replaced with a space. For example, PDXlateToPDFDocEncEx() 
	converts <code>'\\n'</code> to a space character (<code>'\\r'</code> is present in <code>PDFDocEncoding</code> 
	and is left unchanged). </p>

	<p>Host encoding is a platform-dependent encoding for the host 
	machine. For non-UNIX Roman systems, it is <code>WinAnsiEncoding</code> on Windows and <code>MacRomanEncoding</code> 
	on Mac OS. On UNIX (except HP-UX) Roman systems, it is <code>ISO8859-1</code> (ISO Latin-1); for 
	HP-UX, it is <code>HP-ROMAN8</code>. See Appendix D in the <i>PDF Reference</i> 
	for descriptions of <code>WinAnsiEncoding</code>, <code>MacRomanEncoding</code>, and 
	<code>PDFDocEncoding</code>. </p>

	<p>For non-Roman systems, the host encoding may be a variety 
	of encodings, which are defined by a CMap (character map). 
	See Section 5.6.4 in the <i>PDF Reference</i> for a list of predefined 
	CMaps.</p> 

	<p>For non-Roman systems, use PDXlateToPDFDocEncEx(). You can 
	use PDGetHostEncoding() to determine whether a system's host 
	encoding is Roman. </p>

	<p>In general, PDXlateToPDFDocEncEx() can be called instead of 
	PDXlateToPDFDocEnc() since PDXlateToPDFDocEncEx() works for 
	PDFDocEncoding or Unicode. </p>
	@param bUseUnicode If <code>true</code>, translate the string to Unicode; 
	otherwise use <code>PDFDocEncoding</code>. 
	@param inHostStr A pointer to the string to translate (it may 
	point to the same memory as <code>outPDFStr</code>, allowing strings 
	to translate in place). 
	@param inHostStrSize The number of bytes in the string to 
	translate. 
	@param outPDFStr (Filled by the method) A pointer to the 
	translated string (it may point to the same memory as <code>inHostStr</code>). 
	
	@param outPDFStrSize The length of the <code>outPDFStr</code> buffer, 
	in bytes.
	@return The number of bytes in the translated string <code>outPDFStr</code>. 
	@see PDFontXlateToHost 
	@see PDFontXlateToUCS 
	@see PDGetHostEncoding 
	@see PDXlateToHost 
	@see PDXlateToHostEx 
	@see PDXlateToPDFDocEnc 
	@since PI_PDMODEL_VERSION >= 0x00020003
*/
NPROC(ASInt32, PDXlateToPDFDocEncEx, (ASBool bUseUnicode,const char *inHostStr, ASInt32 inHostStrSize, char* outPDFStr, ASInt32 outPDFStrSize ))

/**
	Gets the number of additional bytes required for the multi-byte 
	character pointed to by <code>cp</code>. If <code>cp</code> points to a single-byte 
	character, <code>0</code> is returned. This method makes it possible to determine 
	the length of multi-byte character strings to allocate space 
	for them. 

	<p>This function is similar to the ANSI-C code: </p>

	<p><code>mblen(cp, MB_LEN_MAX) - 1</code> </p>

	<p>or the Windows function: </p>

	<p><code>IsDBCSLeadByte(cp) </code></p>

	@param cp The character to examine.
	@return The number of bytes in the multi-byte character. 
	@since PI_PDMODEL_VERSION >= 0x00020003
*/
NPROC(ASInt32, PDHostMBLen, (const char* cp))

/**
	Indicates what kind of host encoding a system uses. It allows 
	you to determine whether a system is Roman or non-Roman. 
	(Non-Roman is also known as CJK-capable, which means that it is capable 
	of handling multi-byte character sets such as Chinese, 
	Japanese, or Korean). 

	<p>Host encoding is a platform-dependent encoding for the host 
	machine. For non-UNIX Roman systems, it is <code>WinAnsiEncoding</code> on Windows and <code>MacRomanEncoding</code> 
	on Mac OS. On UNIX (except HP-UX) Roman systems, it is <code>ISO8859-1</code> (ISO Latin-1); for 
	HP-UX, it is <code>HP-ROMAN8</code>. See Appendix D in the <i>PDF Reference</i> 
	for descriptions of <code>WinAnsiEncoding</code>, <code>MacRomanEncoding</code>, and 
	<code>PDFDocEncoding</code>. </p>

	<p>For non-Roman systems, the host encoding may be a variety 
	of encodings, which are defined by a CMap (character map). 
	See Section 5.6.4 in the <i>PDF Reference</i> for a list of predefined 
	CMaps. </p>

	<p>Use PDGetHostEncoding() to determine if a system's host encoding 
	is Roman. </p>
	@return <code>0</code> for a Roman system, nonzero for a non-Roman system (a 
	structure that depends on the host encoding). Users should 
	simply test whether this value is <code>0</code>. 
	@see PDGetPDFDocEncoding 
	@see AVAppGetLanguageEncoding 
	@since PI_PDMODEL_VERSION >= 0x00020003
*/
NPROC(ASHostEncoding, PDGetHostEncoding, (void))

/**
	Creates a word finder that is used to extract text in Unicode 
	format from a PDF file. The word finder may either be used 
	by PDWordFinderEnumWords() (which enumerates words one-by-one) 
	or by PDWordFinderAcquireWordList() (which fills a table with 
	all the words on a page). 
	
	<p>PDDocCreateWordFinder() also works for non-Roman character 
	set viewers. For PDDocCreateWordFinder(), words are extracted 
	to the host encoding. Users desiring Unicode output should 
	use PDDocCreateWordFinderUCS(). </p>

	<p>The type of PDWordFinder determines the encoding of the 
	string returned by PDWordGetString(). If PDDocCreateWordFinderUCS() 
	is used to create the word finder, PDWordGetString() returns 
	only Unicode. Note that there is no way to detect Unicode strings returned 
	by PDWordGetString(), since there is no UCS header (FEFF) 
	added to each string returned. </p>

	<p>In CJK viewers, words are stored internally using CID encoding. 
	For more information on CIDFonts and related topics, see 
	Section 5.6 in the <i>PDF Reference</i>. For detailed information 
	on CIDFonts, see <i>Technical Note #5092, CID-Keyed Font Technology 
	Overview</i>, and <i>Technical Note #5014, Adobe CMap and CIDFont 
	Files Specification</i>. </p>
	@param doc The document on which the word finder is used. 
	
	@param algVersion The version of the word-finding algorithm 
	to use. If it is WF_LATEST_VERSION (see PDExpT.h), the most recent 
	version is used. Set to <code>0</code> to ignore the version. 
	
	@param rdFlags Word-finding options that determine the 
	tables filled when using PDWordFinderAcquireWordList(). It must 
	be an <code>OR</code> of one or more of the WordFinder Sort Order Flags. 
	
	@param clientData A pointer to user-supplied data to pass 
	to the newly created word finder.
	@return The newly created word finder. 
	@see PDDocCreateWordFinder 
	@see PDWordFinderEnumWords 
	@see PDWordFinderAcquireWordList 
	@see PDWordFinderDestroy 
	@see PDWordFilterWord 

	@note The word finder also extracts text from Form XObjects 
	that are executed in the page contents. For information 
	about Form XObjects, see Section 4.9 in the <i>PDF Reference</i>. 
	
	@note PDDocCreateWordFinderUCS() is useful for converting 
	non-Roman text (CJK or Chinese-Japanese-Korean) to Unicode. 
	This method also converts Roman text to Unicode in any document. 
  	@ref WordFinderSortOrderFlags
	@since PI_PDMODEL_VERSION >= 0x00020003
*/
NPROC(PDWordFinder, PDDocCreateWordFinderUCS, (PDDoc doc,ASInt16 algVersion, ASUns16 rdFlags, void *clientData))

/**
	Translates a string from the PDFont's encoding to host encoding. 
	This is useful for converting the text from a PDWord into 
	host encoding. In the same way that PDXlateToHostEx() converts 
	text from bookmark titles to host encoding, PDFontXlateToHost() 
	converts text from a page contents stream to host encoding. 
	Use PDFontXlateToUCS() to translate from the PDFont's encoding 
	to Unicode. 

	<p>Non-Roman fonts, such as PostScript composite fonts, can 
	be encoded in different ways, such as SHIFT-JIS, RKSJ, and 
	so on. To use PDFontXlateToHost(), the caller does not need 
	to know which encoding he is converting from, since that 
	information is contained in the PDFont. </p>

	<p>Host encoding is a platform-dependent encoding for the host 
	machine. For non-UNIX Roman systems, it is <code>WinAnsiEncoding</code> on Windows 
	and <code>MacRomanEncoding</code> on Mac OS. On UNIX (except 
	HP-UX) Roman systems, it is <code>ISO8859-1</code> (ISO Latin-1); for 
	HP-UX, it is <code>HP-ROMAN8</code>. See Appendix D in the <i>PDF Reference</i> 
	for descriptions of <code>MacRomanEncoding</code>, <code>WinAnsiEncoding</code>, and 
	<code>PDFDocEncoding</code>. </p>

	<p>For non-Roman systems, the host encoding may be a variety 
	of encodings, which are defined by a CMap (character map). 
	See Section 5.6.4 in the <i>PDF Reference</i> for a list of predefined 
	CMaps. </p>

	Use PDGetHostEncoding() to determine if a system's host encoding 
	is Roman. 
	@param fontP The font used in the input string <code>inP</code>. 
	@param inP A pointer to the string to translate. 
	@param inLen The length of the <code>inP</code> buffer in bytes. 
	@param outP (Filled by the method) A pointer to the translated 
	string. 
	@param outLen The length of the <code>outP</code> buffer in bytes.
	@return The number of bytes in the translated string <code>outP</code>. 
	@see PDFontXlateString 
	@see PDFontXlateToUCS 
	@see PDGetHostEncoding 
	@see PDXlateToHostEx 
	@see PDXlateToPDFDocEncEx 
	@since PI_PDMODEL_VERSION >= 0x00020003
*/
NPROC(ASInt32, PDFontXlateToHost, (PDFont fontP, ASUns8 *inP, ASInt32 inLen, ASUns8 *outP, ASInt32 outLen))

/**
	Translates a string from whatever encoding the PDFont uses 
	to Unicode encoding. This is useful for converting the text 
	from a PDWord into Unicode. Use PDFontXlateToHost() to translate 
	from the PDFont's encoding to host encoding. 

	<p>Non-Roman fonts, like PostScript composite fonts, can be 
	encoded in different ways, such as SHIFT-JIS, RKSJ, and 
	so on. The caller does not need to know which encoding they're 
	converting from, since that information is contained in 
	the PDFont. </p>

	<p>Host encoding is a platform-dependent encoding for the host 
	machine. For non-UNIX Roman systems, it is <code>WinAnsiEncoding</code> on Windows
	and <code>MacRomanEncoding</code> on Mac OS. On UNIX (except 
	HP-UX) Roman systems, it is <code>ISO8859-1</code> (ISO Latin-1); for 
	HP-UX, it is <code>HP-ROMAN8</code>. See Appendix D in the <i>PDF Reference</i> 
	for descriptions of <code>WinAnsiEncoding</code>, <code>MacRomanEncoding</code>, and 
	<code>PDFDocEncoding</code>. </p>

	<p>For non-Roman systems, the host encoding may be a variety 
	of encodings, which are defined by a CMap (character map). 
	See Section 5.6.4 in the <i>PDF Reference</i> for a list of predefined 
	CMaps. </p>

	<p>Use PDGetHostEncoding() to determine if a system's host encoding 
	is Roman. </p>

	@param fontP The font of the input string <code>inP</code>. 
	@param inP A pointer to the string to translate. 
	@param inLen The length of the <code>inP</code> buffer in bytes. 
	@param outP (Filled by the method) A pointer to the translated 
	string. If it is <code>NULL</code>, the method returns the size of the translated 
	string. 
	@param outLen The length of the <code>outP</code> buffer in bytes. 
	If it is <code>0</code>, the method returns the size of the translated string.
	@return The number of bytes in the translated string in <code>outP</code>. 
	@see PDFontXlateToHost 
	@see PDGetHostEncoding 
	@see PDXlateToHostEx 
	@see PDXlateToPDFDocEncEx 
	@since PI_PDMODEL_VERSION >= 0x00020003
*/
NPROC(ASInt32, PDFontXlateToUCS, (PDFont fontP, ASUns8 *inP, ASInt32 inLen, ASUns8 *outP, ASInt32 outLen))

/* Acrobat 4.0 Additions */

/**
	Gets the PDDoc associated with a CosDoc. 
	@param cosDoc The Cos-level document object for which 
	a PDDoc is obtained. This object represents the PDF.
	@return The PDDoc associated with cosDoc. 
	@exception genErrBadParm is raised if the CosDoc is not valid. 
	@exception pdErrNoPDDocForCosDoc is raised if there is no PDDoc associated 
	with this CosDoc. 
	@see AVDocGetPDDoc 
	@see PDPageGetDoc 
	@see PDDocGetCosDoc 
	@since PI_PDMODEL_VERSION >= 0x00040000
*/
NPROC(PDDoc, PDDocFromCosDoc, (CosDoc cosDoc))

/**
	Enumerates the specified type of page resources, for a specified 
	range of pages. 

	<p>This method enumerates resources in each page's Resources 
	dictionary (ColorSpace resources, Fonts, ExtGState objects, 
	or others). In addition, it looks inside in-line images 
	and page contents to enumerate ColorSpace resources that 
	are not in the Resources dictionary, such as DeviceGray, 
	DeviceRGB, and DeviceCMYK. </p>

	@param pdDoc IN/OUT The document whose resources are enumerated. 
	
	@param startPage IN/OUT The first page whose resources are enumerated. 
	The first page in a document is <code>0</code>.
	@param endPage IN/OUT The last page whose resources are enumerated. 
	
	@param resourceType IN/OUT Resource type to enumerate. It must be 
	one of the valid PDF resource types, such as Font, ColorSpace, 
	XObject, Pattern, and so on, described in Section 3.7 in 
	the <i>PDF Reference</i>. Pass ASAtomNull to enumerate all resource 
	types. 
	@param enumProc IN/OUT A user-supplied callback to call once for 
	each resource of the specified type. The resource is presented 
	as a CosObj, and it is the first parameter of <code>enumProc</code> (the second parameter is unused). 
	@param clientData IN/OUT User-supplied data to pass to <code>enumProc</code> 
	each time it is called. 
	@exception genErrBadParm 
	@exception pdErrOpNotPermitted 
	@see PDDocEnumFonts 
	@see PDEEnumElements 
	@see PDELogDump 
	@see PDEObjectDump 
	@since PI_PDMODEL_VERSION >= 0x00040000
	
*/
NPROC(void,  PDDocEnumResources,
			(PDDoc pdDoc,
			ASInt32 startPage,
			ASInt32 endPage,
			ASAtom resourceType,
			CosObjEnumProc enumProc,
			void *clientData))

/**
	Adds text annotations from <code>sourceDoc</code> to <code>doc</code>. 

	<p>It raises an exception if the given object has the wrong Cos 
	type. It also raises exceptions if storage is exhausted or file access fails. </p>

	@param doc The document that will receive the imported 
	annotations. 
	@param src The document from which the annotations will 
	be imported. 
	@param noteTitle Not currently used. 
	@param noteTitleLen Not currently used. 
	@param color If non-<code>NULL</code>, the color attribute of imported 
	annotations. <code>color</code> indicates the color space (PDDeviceGray, 
	PDDeviceRGB, PDDeviceCMYK), and color values for the annotation. 
	
	@param progMon If supplied, it is a procedure to call regularly 
	to update a progress bar for the user. 
	@param monClientData If supplied, it is a pointer to the private 
	data buffer used by <code>progMon</code>. 
	@param importFilter A user-supplied procedure that will 
	be called to provide a filtering process, allowing only 
	desired annotations to import.
	@return The number of notes imported. 
	@notify PDDocDidImportAnnots 
	@notify PDDocWillImportAnnots 
	@see PDDocImportNotes 
	@see PDDocExportNotes 
	@since PI_PDMODEL_VERSION >= 0x00040000
*/
UNPROC(ASInt32, PDDocImportCosDocNotes,
	  		   (PDDoc doc,
				CosDoc src,
				const char* noteTitle,
				ASInt32 noteTitleLen,
				PDColorValue color,
				void* progMon, /* ASProgressMonitor */
				void* monClientData,
				PDDocWillImportAnnotCallback importFilter))

/**
	Creates a document containing empty pages plus text annotations 
	(notes) from <code>sourceDoc</code>. It does not create a new document if 
	<code>sourceDoc</code> contains no notes. 
	@param doc The document from which notes are exported. 
	
	@param unused1 Currently unused. 
	@param unused2 Currently unused. 
	@param unused3 Currently unused. 
	@param unused4 Currently unused. 
	@param exportFilter A user-supplied routine that selects 
	which notes to export. 
	@param numNotesP If non-<code>NULL</code>, the number of notes exported.
	@return The CosDoc of the document created to hold the exported 
	notes. 
	@notify PDDocWillExportAnnots 
	@notify PDDocDidExportAnnots 
	@see PDDocImportCosDocNotes 
	@see PDDocImportNotes 
	@see PDDocExportSomeNotes 
	@since PI_PDMODEL_VERSION >= 0x00040000
*/
UNPROC(CosDoc, PDDocExportNotes,
	  		 (PDDoc doc,
			  ASFileSys unused1,
			  ASPathName unused2,
			  void* unused3, /* ASProgressMonitor */
			  void* unused4,
			  PDDocWillExportAnnotCallback exportFilter,
			  ASInt32* numNotesP))

/**
	Returns the sequence number of the specified annotation 
	for the given page. It is applicable only to annotations 
	that are listed in Acrobat's Comments pane and therefore 
	cannot be summarized using Summarize command 
	(as would be the case for link and widget annotations, for example). 

	<p>This method is similar to PDPageGetAnnotIndex() but it checks 
	the information flags from the annotation handler's PDAnnotHandlerGetAnnotInfoFlagsProc() 
	to determine whether the PDAnnotOperationSummarize flag 
	is set, meaning that the annotation has a sequence number.</p> 
	
	@param page The page on which the annotation exists. 
	@param annot The annotation for which the sequence number 
	is desired.
	@return The sequence number of the specified annotation; or <code>-1</code> if 
	the annotation is not in the page or if it is an annotation that cannot be summarized. 

	@note The sequence number is one-based, while the index 
	returned by PDPageGetAnnotIndex() is zero-based. 
	@see PDPageGetAnnot 
	@see PDPageGetAnnotIndex 
	@since PI_PDMODEL_VERSION >= 0x00040000
*/
UNPROC(ASInt32, PDPageGetAnnotSequence, (PDPage page, PDAnnot annot))

/**
	Registers a handler for an annotation subtype, replacing 
	any previous handler that had been registered for that subtype. 
	The annotation handler is not registered if its PDAnnotHandlerGetTypeProc() 
	returns <code>NULL</code>. 

	<p>To effectively use a PDAnnotHandler, the AVAnnotHandler 
	associated with this annotation must have its AVAnnotHandlerGetInfoProc() 
	and AVAnnotHandlerDeleteInfoProc() callbacks defined. </p>

	<p>PDF Library applications can use this method to register their annotation handlers. 
	Link and Watermark annotations have default handlers. For other annotation types, the applications should register their own handlers.</p>
	@param handler IN/OUT A pointer to a structure containing the annotation 
	handler's callbacks. This structure must not be freed after 
	this call, but must be retained. 
	@see PDGetAnnotHandlerByName 
	@see AVAppGetAnnotHandlerByName 
	@since PI_PDMODEL_VERSION >= 0x00040000
	
*/
UNPROC(void, PDRegisterAnnotHandler, (PDAnnotHandler handler))

/**
	Gets the annotation handler that handles the specified annotation 
	type. 
	@param name IN/OUT The name of the requested annotation handler. The 
	character string for the name can be converted to an ASAtom 
	using ASAtomFromString(). 
	@return The annotation handler that services annotations of type 
	<code>name</code>. It returns the default annotation handler if no handler 
	services the specified annotation type. 
	@see PDRegisterAnnotHandler 
	@see AVAppRegisterAnnotHandler 
	@since PI_PDMODEL_VERSION >= 0x00040000
	
*/
UNPROC(PDAnnotHandler, PDGetAnnotHandlerByName, (ASAtom name))

/* PDNameTree related methods */

/**
	Creates a new name tree in the document. 
	@param pdDoc The document for which a new name tree is 
	desired.
	@return The newly created name tree or a <code>NULL</code> CosObj if Acrobat 
	is unable to create a PDNameTree for the document specified 
	by <code>pdDoc</code>. 

	<p>PDNameTreeIsValid() should be called to determine if the name 
	tree returned by PDNameTreeNew() is usable. </p>
	@see PDNameTreeFromCosObj 
	@see PDNameTreeGetCosObj 
	@see PDNameTreeIsValid 
	@see PDNameTreeRemove 
	@since PI_PDMODEL_VERSION >= 0x00040000
*/
UNPROC(PDNameTree, PDNameTreeNew, (PDDoc pdDoc))

/**
	Creates a type cast of the CosObj to a name tree. This does 
	not copy the object. 
	@param obj IN/OUT The CosObj for which a PDNameTree representation 
	is desired. 
	@return A PDNameTree representation of <code>obj</code>. 
	@see PDNameTreeNew 
	@see PDNameTreeGetCosObj 
	@since PI_PDMODEL_VERSION >= 0x00040000
	
*/
NPROC(PDNameTree, PDNameTreeFromCosObj, (CosObj obj))

/**
	Creates a type cast of the name tree to a CosObj. This does 
	not copy the object. 
	@param theTree IN/OUT The PDNameTree for which a CosObj representation 
	is desired. 
	@return A CosObj representation of <code>theTree</code>. 
	@see PDNameTreeNew 
	@see PDNameTreeFromCosObj 
	@since PI_PDMODEL_VERSION >= 0x00040000
	
*/
NPROC(CosObj, PDNameTreeGetCosObj, (PDNameTree theTree))

/**
	Validates whether a PDNameTree is a CosDict Cos object.

	@param theTree The PDNameTree whose validity is desired.
	@return <code>true</code> if the name tree is a CosDict, <code>false</code> otherwise. 
	
	@see PDDocCreateNameTree 
	@see PDDocGetNameTree 
	@see PDNameTreeEqual 
	@since PI_PDMODEL_VERSION >= 0x00040000
*/
NPROC(ASBool, PDNameTreeIsValid, (PDNameTree theTree))

/**
	Compares two name trees to determine if they are the same 
	object. 
	@param tree1 A name tree. 
	@param tree2 Another name tree.
	@return <code>true</code> if the two name trees are equivalent, <code>false</code> 
	otherwise. 
	@see PDNameTreeIsValid 
	@since PI_PDMODEL_VERSION >= 0x00040000
*/
NPROC(ASBool, PDNameTreeEqual, (PDNameTree tree1, PDNameTree tree2))

/**
	Puts a new entry in the name tree. If an entry with this 
	name is already in the tree, it is replaced. 
	@param theTree IN/OUT The name tree for which a new entry is added. 
	
	@param key IN/OUT The name of the object to put in the tree. This 
	is a Cos-style string, not a C string. This allows the use 
	of an existing indirect object for the key rather than forcing 
	the creation of a new object. 
	@param value IN/OUT The Cos object to be associated with <code>key</code>. 
	
	@notify PDNameTreeNameAdded 
	@see PDNameTreeGet 
	@since PI_PDMODEL_VERSION >= 0x00040000
	
*/
UNPROC(void, PDNameTreePut, (PDNameTree theTree, CosObj key, CosObj value))

/**
	Retrieves an object from the name tree. 
	@param theTree The PDNameTree from which an object is 
	retrieved. 
	@param name The name of the object within <code>theTree</code> to get. 
	This is a Cos-style string, not a C string. 
	@param nameLen The length of <code>name</code>. 
	@param value (Filled by the method) The Cos object corresponding 
	to <code>name</code> within <code>theTree</code>.
	@return <code>true</code> if the object was retrieved, <code>false</code> if no object 
	with this name exists. 
	@see PDNameTreePut 
	@see PDNameTreeRemove 
	@since PI_PDMODEL_VERSION >= 0x00040000
*/
NPROC(ASBool, PDNameTreeGet, (PDNameTree theTree, const char *name, ASInt32 nameLen, CosObj *value))

/**
	Removes the specified object from the tree. It does nothing 
	if no object with that name exists. 
	@param theTree IN/OUT The name tree from which an entry is removed. 
	
	@param key IN/OUT The name of the entry to remove. This is a Cos-
	style string, not a C string. 
	@param keyLen IN/OUT The length of <code>key</code> in bytes. 
	@notify PDNameTreeNameRemoved 
	@see PDNameTreeGet 
	@see PDNameTreePut 
	@since PI_PDMODEL_VERSION >= 0x00040000
	
*/
UNPROC(void, PDNameTreeRemove, (PDNameTree theTree, const char *key, ASInt32 keyLen))

/**
	Enumerates the entries in the tree. 
	@param theTree IN/OUT A name tree. 
	@param proc IN/OUT A procedure to call once for each name/object 
	pair in <code>theTree</code>. The obj/value pair in <code>proc</code> correspond 
	to the Cos string and CosObj values of each leaf in the 
	tree. 
	@param clientData IN/OUT Data used by the enumeration procedure. 
	<code>clientData</code> is passed to the enumeration procedure proc each 
	time an entry is encountered. 
	@see PDNameTreeGet 
	@see PDNameTreePut 
	@see PDNameTreeRemove 
  	@ingroup Enumerators
	@since PI_PDMODEL_VERSION >= 0x00040000
	
*/
NPROC(void, PDNameTreeEnum, (PDNameTree theTree, CosObjEnumProc proc, void* clientData))

/**
	Retrieves a name tree, with the key name specified in <code>theTree</code>, 
	from the Names dictionary of <code>thePDDoc</code>. 
	@param thePDDoc IN/OUT The document containing the name tree desired. 
	
	@param theTree IN/OUT The name of the tree requested. This can 
	be created by passing a string to the ASAtomFromString() method. 
	
	@return The PDNameTree requested. 
	@see PDNameTreeIsValid 
	@see PDDocCreateNameTree 
	@see PDDocRemoveNameTree 
	@since PI_PDMODEL_VERSION >= 0x00040000
	
*/
NPROC(PDNameTree, PDDocGetNameTree, (PDDoc thePDDoc, ASAtom theTree))

/**
	Retrieves the name tree inside the Names dictionary with 
	the specified key name, or creates it if it does not exist. 
	
	@param thePDDoc The document in which the name tree is 
	created. 
	@param theTree The name of the name tree to create. A 
	string can be converted to an ASAtom using ASAtomFromString().
	@return The newly created PDNameTree for the PDDoc. It returns a <code>NULL</code> 
	PDNameTree if <code>pdDoc</code> has no root dictionary. The return value 
	should be tested with PDNameTreeIsValid(). 
	@see PDNameTreeIsValid 
	@see PDDocGetNameTree 
	@see PDDocRemoveNameTree 
	@since PI_PDMODEL_VERSION >= 0x00040000
*/
UNPROC(PDNameTree, PDDocCreateNameTree, (PDDoc thePDDoc, ASAtom theTree))

/**
	Removes the name tree inside the Names dictionary with the 
	specified key name. It does nothing if no object with that 
	name exists. 
	@param thePDDoc IN/OUT The document from which a name tree is 
	removed. 
	@param theTree IN/OUT The name tree to remove. 
	@see PDDocCreateNameTree 
	@see PDDocGetNameTree 
	@since PI_PDMODEL_VERSION >= 0x00040000
	
*/
UNPROC(void, PDDocRemoveNameTree, (PDDoc thePDDoc, ASAtom theTree))

/* PDPageLabel related methods */

/**
	Determines whether a page label is valid. 

	<p>A page label is valid if its values correspond to the specification 
	for page label dictionaries in Table 8.6 in the <i>PDF Reference</i>. </p>
	
	<p>It raises an exception if storage is exhausted or file access 
	fails.</p>

	@param pgLabel The page label whose validity is determined.
	@return <code>true</code> if the label is valid, <code>false</code> otherwise. 
	@see PDPageLabelEqual 
	@since PI_PDMODEL_VERSION >= 0x00040000
*/
NPROC(ASBool, PDPageLabelIsValid, (PDPageLabel pgLabel))

/**
	Compares two page labels to see if they are equivalent. 
	Two labels are equivalent if they have the same style, starting 
	number (the numeric value of the first page associated with 
	the label), and prefix strings which are the same byte-for-byte. 
	
	@param pdlOne A page label. 
	@param pdlTwo Another page label.
	@return <code>true</code> if the two labels are valid and equivalent, 
	<code>false</code> otherwise. 
	@see PDPageLabelIsValid 
	@since PI_PDMODEL_VERSION >= 0x00040000
*/
NPROC(ASBool, PDPageLabelEqual, (PDPageLabel pdlOne, PDPageLabel pdlTwo))

/**
	Creates a type cast of the page label object to a Cos object. 
	
	@param pdl IN/OUT A PDPageLabel representation of a page label. 
	
	@return A CosObj representation of <code>pdl</code> if the page label is valid, 
	<code>NULL</code> CosObj otherwise. 
	@see PDPageLabelFromCosObj 
	@since PI_PDMODEL_VERSION >= 0x00040000
	
*/
NPROC(CosObj, PDPageLabelGetCosObj, (PDPageLabel pdl))

/**
	Creates a type cast of the CosObj to a PDPageLabel object. 
	
	@param cosLabel IN/OUT The Cos object level representation of a page 
	label. 
	@return The page label representation of <code>cosLabel</code>. 
	@exception pdErrBadBaseObj is raised if <code>cosLabel</code> is not a valid page label. 
	
	@see PDPageLabelGetCosObj 
	@since PI_PDMODEL_VERSION >= 0x00040000
	
*/
NPROC(PDPageLabel, PDPageLabelFromCosObj, (CosObj cosLabel))

/**
	Returns an ASAtom for the style of the label. 

	<p>It raises an exception if storage is exhausted or file access fails. </p>

	@param pgLabel IN/OUT The page label whose style is desired. 
	@return An ASAtom for the label style. If no style is specified, 
	it returns <code>ASAtomFromString(" None")</code>. 
	@see PDPageLabelGetPrefix 
	@see PDPageLabelGetStart 
	@since PI_PDMODEL_VERSION >= 0x00040000
	
*/
NPROC(ASAtom,  PDPageLabelGetStyle, (PDPageLabel pgLabel))

/**
	Returns the prefix string for the label. The prefix string 
	is transitory and should be copied immediately. 
	@param pgLabel The label for the page whose prefix is desired. 
	
	@param prefixLen (Filled by the method) The length, in 
	bytes, of the prefix string. It is zero if the page label is not valid.
	@return The prefix string for the label, or <code>NULL</code> if none is specified. 
	
	@see PDPageLabelGetStart 
	@see PDPageLabelGetStyle 
	@since PI_PDMODEL_VERSION >= 0x00040000
*/
NPROC(const char *, PDPageLabelGetPrefix, (PDPageLabel pgLabel, ASInt32 *prefixLen))

/**
	Gets the starting number of a given page label. 
	@param pgLabel The page label for the page whose starting number 
	is desired.
	@return The starting number of the page label; that is, the numeric 
	value of the first page associated with the label. It returns 
	<code>1</code> if the page label is not valid or unknown. 
	@see PDPageLabelGetPrefix 
	@see PDPageLabelGetStyle 
	@since PI_PDMODEL_VERSION >= 0x00040000
*/
NPROC(ASInt32, PDPageLabelGetStart, (PDPageLabel pgLabel))

/**
	Returns the label that is in effect for the given page. 
	
	@param pdDoc The document for which a page label is desired. 
	
	@param pageNum The page number of the page for which a 
	label is requested. 
	@param firstPage (Filled by the method) If non-<code>NULL</code>, it is the 
	number of the first page that the page label is attached 
	to. 
	@param lastPage (Filled by the method) If non-<code>NULL</code>, it is the 
	number of the last page that the page label is attached 
	to. Setting <code>lastPage</code> to non-<code>NULL</code> forces the implementation 
	to perform another traverse of the page label tree, with 
	some slight performance impact.
	@return The label that is in effect for the given page. If there 
	is no label object in effect, this method returns an invalid 
	page label object, and <code>firstPage</code> and <code>lastPage</code> will be set 
	to <code>-1</code>. 
	@see PDDocFindPageNumForLabel 
	@see PDDocGetLabelForPageNum 
	@see PDDocRemovePageLabel 
	@see PDDocSetPageLabel 
	@see PDPageLabelNew 
	@since PI_PDMODEL_VERSION >= 0x00040000
*/
NPROC(PDPageLabel, PDDocGetPageLabel, (PDDoc	pdDoc, ASInt32 pageNum,
										    ASInt32 *firstPage, ASInt32 *lastPage))

/**
	Constructs a new label object in the document with the specified 
	style, prefix, and starting page number. 
	@param pdDoc The document that contains the new page label. 
	
	@param style The numbering system to use for the numeric 
	portion of each label in this range of pages. The possible values 
	are <code>D</code> for decimal numbers, <code>R</code> for upper-case Roman numbers, 
	<code>r</code> for lower-case Roman numbers, <code>A</code> for upper-case alphabetic 
	numbers, or <code>a</code> for lower-case alphabetic numbers. If it is <code>NULL</code>, 
	the labels for this range will not have a numeric portion. 
	
	@param prefix A string to prefix to the numeric portion 
	of the page label. It may be a <code>NULL</code> string. 
	@param prefixLen The length in bytes of the prefix string. 
	
	@param startAt The value to use when generating the numeric 
	portion of the first label in this range; it must be greater 
	than or equal to <code>1</code>.
	@return The newly created page label. 
	@exception pdErrBadBaseObj is raised if the base pages object is missing 
	or invalid. 
	@see PDDocRemovePageLabel 
	@see PDDocGetPageLabel 
	@see PDDocFindPageNumForLabel 
	@see PDDocGetLabelForPageNum 
	@see PDDocSetPageLabel 
	@since PI_PDMODEL_VERSION >= 0x00040000
*/
UNPROC(PDPageLabel, PDPageLabelNew, (PDDoc	pdDoc, ASAtom style, const char *prefix,
									 ASInt32 prefixLen, ASInt32 startAt))

/**
	Attaches a label to a page. This establishes the numbering 
	scheme for that page and all pages following it, until another 
	page label is encountered. This label allows PDF producers 
	to define a page numbering system other than the Acrobat 
	default. 

	<p>If <code>pageNum</code> is less than <code>0</code> or greater than the number of 
	pages in <code>pdDoc</code>, the method does nothing. </p>

	@param pdDoc The document containing the page to label. 
	
	@param pageNum The number of the page to label. 
	@param pgLabel The label for the page specified by <code>pageNum</code>.
	@exception genErrBadParm is raised if <code>pgLabel</code> is not a valid PDPageLabel. 
	
	@notify PDDocPageLabelDidChange 
	@see PDDocFindPageNumForLabel 
	@see PDDocGetPageLabel 
	@see PDDocGetLabelForPageNum 
	@see PDDocRemovePageLabel 
	@see PDPageLabelNew 
	@since PI_PDMODEL_VERSION >= 0x00040000
*/
UNPROC(void, PDDocSetPageLabel, (PDDoc pdDoc, ASInt32 pageNum, PDPageLabel pgLabel))

/**
	Removes the page label that is attached to the specified 
	page, effectively merging the specified range with the previous 
	page label sequence. 
	@param pdDoc The document from which a page label is removed. 
	
	@param pageNum The page from which the page label is removed.
	@see PDDocSetPageLabel 
	@see PDDocGetPageLabel 
	@see PDPageLabelNew 
	@since PI_PDMODEL_VERSION >= 0x00040000
*/
UNPROC(void, PDDocRemovePageLabel, (PDDoc pdDoc, ASInt32 pageNum))

/* PDDocOpenWithParams */

/**
	Opens the document specified by the ASFile or ASFileSys/ASPathName. 
	If both are set, the ASFile is used and the <code>fileSys</code> and <code>pathName</code> are ignored. 
	@param openParams IN/OUT A structure that defines which PDF file 
	is opened. It contains parameters such as a file name, a file system, an authorization 
	procedure, and a set of flags that define what permissions 
	the user has on a file. 
	@return The PDDoc for the PDF document described by the structure 
	passed in <code>openParams</code>. 
	@see PDDocOpen 
	@see PDDocOpenEx 
	@see PDDocOpenFromASFile 
	@see PDDocOpenFromASFileEx 
	@since PI_PDMODEL_VERSION >= 0x00040000
	
*/
NPROC(PDDoc, PDDocOpenWithParams, (PDDocOpenParams openParams))

/* PDDocReadAhead */

/**
	Reads ahead <code>nPages</code> starting at <code>startPage</code> (if the file is 
	linearized). 
	@param doc IN/OUT The document for which pages are read ahead. 
	
	@param startPage IN/OUT The page for which read ahead is initiated. 
	
	@param nPages IN/OUT The number of pages to read ahead. 
	@since PI_PDMODEL_VERSION >= 0x00040000
	
*/
NPROC(void, PDDocReadAheadPages, (PDDoc doc, ASInt32 startPage, ASInt32 nPages))

/* Further PDPageLabel API's */

/**

	Superseded by PDDocGetLabelForPageNumEx() in Acrobat 6.0. 

	<p>Retrieves the label string associated with the given page 
	number. The page number is returned in host encoding and 
	is truncated to the length of the buffer. </p>

	@param pdDoc The document containing the page for which a 
	label is requested. 
	@param pageNum The number of the page whose label is requested. 
	
	@param buffer If a label exists for <code>pageNum</code>, it will be 
	placed in this buffer. 
	@param bufferLen The length of the label (<code>NULL</code>-terminated).
	@return The length of the resulting label. If no such page number 
	exists, the resulting string will be the ASCII representation 
	of <code>pageNum + 1</code>. 
	@see PDDocGetLabelForPageNumEx 
	@see PDDocFindPageNumForLabel 
	@see PDDocGetPageLabel 
	@see PDDocRemovePageLabel 
	@see PDDocSetPageLabel 
	@see PDPageLabelNew 
	@since PI_PDMODEL_VERSION >= 0x00040000
*/
NPROC(ASInt32, PDDocGetLabelForPageNum, (
	PDDoc	pdDoc,
	ASInt32	pageNum,
	char	*buffer,
	ASInt32	bufferLen))

/**
	Superseded by PDDocFindPageNumForLabelEx() in Acrobat 6.0. 

	<p>Finds the first page in the document with a specified label. </p>
	
	@param pdDoc The document to search for the page named 
	in <code>labelStr</code>. 
	@param labelStr The label of the page to find. 
	@param labelLen The length of <code>labelStr</code>.
	@return The page number of the first page with the specified label, 
	or <code>-1</code> if no such page exists. 
	@see PDDocGetLabelForPageNum 
	@see PDDocGetPageLabel 
	@see PDDocRemovePageLabel 
	@see PDDocSetPageLabel 
	@see PDPageLabelNew 
	@since PI_PDMODEL_VERSION >= 0x00040000
*/
NPROC(ASInt32, PDDocFindPageNumForLabel, (
	PDDoc	pdDoc,
	const	char *labelStr,
	ASInt32	labelLen))

/**
	Adds text annotations (notes) from <code>sourceDoc</code> to <code>doc</code>. 

	<p>It raises an exception if the given object has the wrong Cos 
	type. Also raises exceptions if storage is exhausted or file access fails. </p>

	@param doc The document to which the notes are exported. 
	
	@param sourceDoc The document from which the notes are 
	exported. 
	@param progMon A user-supplied progress monitor. 
	@param monClientData Data supplied by the monitoring routine. 
	
	@param importFilter A user-supplied filter which determines 
	what type of notes will be exported.
	@return The number of notes imported. 
	@notify PDDocDidImportAnnots 
	@notify PDDocWillImportAnnots 
	@see PDDocExportNotes 
	@see PDDocImportCosDocNotes 
	@since PI_PDMODEL_VERSION >= 0x00040000
*/
UNPROC(ASInt32, PDDocImportNotes,
	  		   (PDDoc doc,
				PDDoc sourceDoc,
				void* progMon, /* ASProgressMonitor */
				void* monClientData,
				PDDocWillImportAnnotCallback importFilter))

/* Alternate Images */

/**
	<p>This method is obsolete and never called in Acrobat 8.</p>

	(Obsolete, provided only for backwards compatibility)
	Selects which Alternate image to use. 
	<p>This method can do one of three things:</p>
	<ul>
	<li>Return an existing Alternate.</li> 
	<li>Create and return an image XObject.</li>
	<li>Indicate that this XObject should be skipped.</li>
	</ul>
	
	<p>This method is called each time the Acrobat viewer draws 
	an XObject image, regardless of whether the image XObject 
	has an Alternates key.</p>

	<p>You can replace this method with your own version, using 
	HFTReplaceEntry(). </p>

	@param image The Cos object for this image XObject's base 
	image. Under some circumstances, PDImageSelectAlternate() 
	can be called with an XObject type other than an image; 
	for example, a form. Because of this, your code must check 
	the XObject's subtype (you can use CosDictGet() to read the 
	value of the XObject's Subtype key). If the subtype is not 
	Image, your code must not modify the XObject, but simply 
	return the CosObj that was passed to you. 
	@param print <code>true</code> if printing, <code>false</code> if displaying. 
	@param tickLimit The amount of time, in ticks, before the 
	image selector must return control to the Acrobat viewer. 
	This parameter is not relevant for image selectors that 
	simply choose an existing alternate or create a new image 
	XObject using Cos methods, but it is relevant for image 
	selectors that create a new image XObject by calculating 
	or reading data from a network or a slow device.
	
	<ul>
	<li>If <code>tickLimit</code> is zero, the image selector must not return
	control to the Acrobat viewer until the selector can provide
	the alternate image to use.</li>
	<li>If <code>tickLimit</code> is nonzero, the image selector 
	does not have to provide the image XObject within <code>tickLimit</code>, 
	but it must raise the fileErrBytesNotReady exception if 
	it cannot. This returns control to the Acrobat viewer and 
	informs it that the data is not ready. The Acrobat viewer 
	then calls the image selector periodically until the image 
	selector returns the selected image XObject.</li>
	</ul>
	
	@ingroup ReplaceableMethods
	@param cacheImageP If <code>true</code>, the image data returned to 
	the Acrobat viewer is cached for future use. If <code>false</code>, it 
	is not. Pass <code>true</code> if you expect your image data to remain 
	valid for at least several calls to your client. Pass <code>false</code> 
	if you expect your image data to change on the next call 
	to your client. If you create and return a Cos stream object 
	with an external file system and the stream's data will 
	not always be the same, you must set <code>cacheImageP</code> to <code>false</code>. 
	If you fail to do this, the Acrobat viewer may use cached 
	image data when it should not. 
	@param callData An opaque pointer containing data that 
	must be passed in other image selector calls.
	@return The image XObject to use. Returning a <code>NULL</code> Cos object (obtained 
	using CosNewNull) tells the Acrobat viewer to skip this 
	XObject entirely. 
	@exception fileErrBytesNotReady 
	@see PDImageSelGetGeoAttr (obsolete)
	@see PDImageSelGetDeviceAttr (obsolete)
	@see PDImageSelAdjustMatrix (obsolete) 
	@since PI_PDMODEL_VERSION >= 0x00040000
*/
PROC(CosObj, PDImageSelectAlternate,
	   (CosObj image, ASBool print, ASUns32 tickLimit, ASBool *cacheImageP, void *callData))

/**
	<p>This method is obsolete and never called in Acrobat 8.</p>

	(Obsolete, provided only for backwards compatibility)  
	Requests geometry-related attributes of an image XObject. 
	This method can be used by an image selector client to obtain 
	additional information to help the selector determine which 
	alternate image to choose. 
	@param callData (Filled by the method) The value passed 
	to the image selector as a parameter to PDImageSelectAlternate(). 
	@param updateBBoxP (Filled by the method) The rectangle bounding 
	the region of the page to update. This is the intersection 
	of the visible portion of the page, any update regions, 
	and any clipping paths that have been explicitly set in 
	the PDF file. Its coordinates are specified in default user 
	space. 
	@param imageToDefaultMatrixP (Filled by the method) A matrix 
	specifying the transformation from image space (the space 
	in which all images are 1x1 units) to default user space 
	(the space with 72 units per inch). It contains sufficient 
	information for the image selector plug-in to determine 
	the image's size and location on the page. For a <i>normal 
	page and image</i> (an <i>upright</i> image on a non-rotated page), 
	the following is true: 
	
	<ul>
	<li><code>imageToDefaultMatrixP->a</code> is equal to the image horizontal size in 1/72 of an inch units.</li> 
	<li><code>imageToDefaultMatrixP->b = 0</code> </li>
	<li><code>imageToDefaultMatrixP->c = 0</code> </li>
	<li><code>imageToDefaultMatrixP->d</code> is equal to the image vertical size in 1/72 of an inch units.</li> 
	<li><code>imageToDefaultMatrixP->h</code> is equal to the left edge of image.</li> 
	<li><code>imageToDefaultMatrixP->v</code> is equal to the bottom edge of the image.</li>  
	</ul>
	
	In other words, this matrix 
	provides the image's height, width, and position on the page, all in 
	units of points (compare to <code>imageToDefaultMatrixP</code>). The 
	intersection of the rectangle obtained by transforming a 
	1x1 unit rectangle (the image) through <code>imageToDeviceMatrixP</code> 
	and the <code>updateBBoxP</code> rectangle is the region of the image 
	that is actually drawn. This is the region of the image 
	for which data is required. 
	@param imageToDevMatrixP (Filled by the method) A matrix 
	specifying the transformation from image space (the space 
	in which all images are 1x1 unit) to device space (the space 
	in which one unit is one pixel). This matrix provides the 
	image's height, width, and position on the page, all in 
	pixels (compare to <code>imageToDefaultMatrixP</code>).
	@see PDImageSelAdjustMatrix (obsolete) 
	@see PDImageSelGetDeviceAttr (obsolete) 
	@see PDImageSelectAlternate (obsolete) 
	@since PI_PDMODEL_VERSION >= 0x00040000
*/
NPROC(void, PDImageSelGetGeoAttr, (void *callData, ASFixedRect *updateBBoxP, 
	    ASFixedMatrix *imageToDefaultMatrixP, ASFixedMatrix *imageToDevMatrixP))

/**
	<p>This method is obsolete and never called in Acrobat 8.</p>

	(Obsolete, provided only for backwards compatibility)  
	Gets device-related attributes for a particular image XObject. 
	It must only be used from within an image selector client, 
	since it returns information that is only valid in that 
	context. This method can be used by an image selector client 
	to obtain additional information to help the selector determine 
	which alternate image to choose. 

	<p>If an image is displayed on two devices simultaneously (for 
	example, if the window containing the image is split across 
	two monitors in a multi-monitor system), the values returned 
	for <code>colorSpaceP</code> and <code>bitsPerPixelP</code> are each the maximum for 
	the devices on which the image is currently displayed. For 
	example, if the image is currently split across the following 
	devices:</p>
	<ul>
	<li>8-bit gray scale monitor </li>
	<li>4-bit RGB color monitor </li>
	</ul>
	
	<p><code>colorSpaceP</code> would be <code>DeviceRGB</code>, because that is the <i>highest</i> 
	color space on which the image is currently displayed. </p>

	<p><code>bitsPerPixelP</code> would be <code>8</code>, because that is the highest bit 
	depth on which the image is currently displayed. </p>

	@param callData (Filled by the method) The value passed 
	to the image selector as a parameter to PDImageSelectAlternate(). 
	
	@param colorSpaceP (Filled by the method) The destination 
	device's color space. It will be one of the following: 

	<TABLE rules="all" cellspacing="1">
	<TR><TH>Value</TH><TH>Description</TH></TR>
	<TR><TD>PDDeviceGray</TD><TD>Grayscale device</TD></TR>
	<TR><TD>PDDeviceRGB</TD><TD>RGB device</TD></TR>
	<TR><TD>PDDeviceCMYK</TD><TD>CMYK device</TD></TR>
	</TABLE>

	<p>If the device has some other color space or 
	its color space cannot be determined, PDDeviceRGB is returned. </p>
	
	@param bitsPerPixelP (Filled by the method) The number of 
	bits used for each pixel. For example, a device with 8 bits 
	red, 8 bits green, and 8 bits blue would have a <code>bitsPerPixel</code> 
	of <code>24</code>. If <code>bitsPerPixelP</code> has a value of <code>0</code> (instead of the 
	more standard <code>1</code>, <code>8</code>, or <code>24</code>), the number of bits per pixel 
	on the output device could not be determined.
	@param deviceTypeP (Filled by the method) The output device 
	type. It will be one of the following: 

	<TABLE rules="all" cellspacing="1">
	<TR><TH>Value</TH><TH>Description</TH></TR>
	<TR><TD><code>Display</code></TD><TD>A display device such as a monitor</TD></TR>
	<TR><TD><code>PostScript</code></TD><TD>A PostScript printer or PostScript file</TD></TR>
	<TR><TD><code>nonPostScriptPrinter</code></TD><TD>A non-PostScript printer</TD></TR>
	</TABLE>
	
	@see PDImageSelAdjustMatrix (obsolete)
	@see PDImageSelGetDeviceAttr (obsolete)
	@see PDImageSelectAlternate (obsolete) 
	@since PI_PDMODEL_VERSION >= 0x00040000
*/
NPROC(void, PDImageSelGetDeviceAttr, (void *callData, PDColorSpace *colorSpaceP,
		ASUns32 *bitsPerPixelP, ASAtom *deviceTypeP))

/**
	<p>This method is obsolete and never called in Acrobat 8.</p>

	(Obsolete, provided only for backwards compatibility)  
	Allows an image selector client to change the region of 
	the page occupied by an image. It must only be used by image 
	selector clients that return data for only the visible part 
	of an image, to set the region of the page that the sub-image 
	occupies. It must not be used otherwise. 

	<p>This method only has an effect while displaying on the screen. 
	It does nothing when printing. </p>

	<p>The matrix set by this call remains in effect only for the 
	current image. The Acrobat viewer automatically replaces 
	it after the image has been drawn. </p>

	@param callData The value passed to the image selector 
	as a parameter to PDImageSelectAlternate(). 
	@param newUserMatrix The matrix that will replace the 
	<code>imageToUserMatri</code> (see PDImageSelGetGeoAttr()). The <code>imageToDevMatrix</code> 
	is automatically calculated from <code>newUserMatrix</code>.
	@see PDImageSelGetGeoAttr (obsolete) 
	@see PDImageSelGetDeviceAttr (obsolete) 
	@see PDImageSelectAlternate (obsolete) 
	@since PI_PDMODEL_VERSION >= 0x00040000
*/
NPROC(void, PDImageSelAdjustMatrix, (void *callData, ASFixedMatrix newUserMatrix))


/**
	Given a CosObj that represents a function, it applies the function 
	to the supplied values. 

	<p>It raises an error if the CosObj is malformed. </p>

	@param funcDict The CosObj representing a function. 
	@param inVals Input values. 
	@param outVals Output values.
	@exception genErrBadParm is raised if the CosObj is not a function dictionary. 
	
	@since PI_PDMODEL_VERSION >= 0x00050000
	@deprecated use PDApplyFunctionEx instead
*/
NPROC(void, PDApplyFunction,
		(CosObj funcDict, const float inVals[], float outVals[]))


/* PDNumTree calls */

/**
	Creates a new number tree in the document. 
	@param pdDoc The document for which a new number tree 
	is desired.
	@return The newly created number tree, or a <code>NULL</code> CosObj if Acrobat 
	is unable to create a PDNumTree for the document specified 
	by <code>pdDoc</code>. 

	<p>PDNumTreeIsValid() should be called to determine if the number 
	tree returned by PDNumTreeNew() is usable. </p>

	@see PDNumTreeFromCosObj 
	@see PDNumTreeGetCosObj 
	@see PDNumTreeIsValid 
	@since PI_PDMODEL_VERSION >= 0x00040000
*/
UNPROC(PDNumTree, PDNumTreeNew, (PDDoc pdDoc))

/**
	Creates a type cast of the CosObj to a number tree. 
	@param obj IN/OUT The CosObj for which a PDNumTree representation 
	is desired. 
	@return A PDNumTree representation of <code>obj</code>. 
	@see PDNumTreeNew 
	@see PDNumTreeGetCosObj 
	@since PI_PDMODEL_VERSION >= 0x00040000
	
*/
NPROC(PDNumTree, PDNumTreeFromCosObj, (CosObj obj))

/**
	Creates a type cast of the number tree to a CosObj. 
	@param theTree IN/OUT The PDNumTree for which a CosObj representation 
	is desired. 
	@return A CosObj representation of <code>theTree</code>. 
	@see PDNumTreeNew 
	@see PDNumTreeFromCosObj 
	@since PI_PDMODEL_VERSION >= 0x00040000
	
*/
NPROC(CosObj, PDNumTreeGetCosObj, (PDNumTree theTree))

/**
	Validates whether a PDNumTree is a CosDict Cos object.

	@param theTree The PDNumTree whose validity is desired.
	@return <code>true</code> if the number tree is a CosDict, <code>false</code> otherwise. 
	
	@see PDNumTreeEqual 
	@since PI_PDMODEL_VERSION >= 0x00040000
*/
NPROC(ASBool, PDNumTreeIsValid, (PDNumTree theTree))

/**
	Compares two number trees to determine if they are the same 
	object. 
	@param tree1 A number tree. 
	@param tree2 Another number tree.
	@return <code>true</code> if the two number trees are equivalent, <code>false</code> 
	otherwise. 
	@see PDNumTreeIsValid 
	@since PI_PDMODEL_VERSION >= 0x00040000
*/
NPROC(ASBool, PDNumTreeEqual, (PDNumTree tree1, PDNumTree tree2))

/**
	Puts a new entry in the number tree. If an entry with this 
	number is already in the tree, it is replaced. 
	@param theTree IN/OUT The number tree for which a new entry is 
	added 
	@param key IN/OUT The number of the entry. 
	@param value IN/OUT The value associated with <code>key</code>. 
	@notify PDNumTreeNumAdded 
	@see PDNumTreeGet 
	@see PDNumTreeRemove 
	@since PI_PDMODEL_VERSION >= 0x00040000
	
*/
UNPROC(void, PDNumTreePut, (PDNumTree theTree, ASInt32 key, CosObj value))

/**
	Retrieves an object from the number tree. 
	@param theTree The PDNumTree requested. 
	@param key The number of the entry to retrieve. 
	@param value (Filled by the method) The value associated 
	with <code>key</code>.
	@return <code>true</code> if the object was retrieved, <code>false</code> if no object 
	with this number exists. 
	@see PDNumTreePut 
	@see PDNumTreeRemove 
	@since PI_PDMODEL_VERSION >= 0x00040000
*/
NPROC(ASBool, PDNumTreeGet, (PDNumTree theTree, ASInt32 key, CosObj *value))

/**
	Removes the specified object from the tree. It does nothing 
	if no object with that number exists. 
	@param theTree IN/OUT The number tree from which an entry is removed. 
	
	@param key IN/OUT The number of the entry to remove. 
	@notify PDNumTreeNumRemoved 
	@see PDNumTreeGet 
	@see PDNumTreePut 
	@since PI_PDMODEL_VERSION >= 0x00040000
	
*/
UNPROC(void, PDNumTreeRemove, (PDNumTree theTree, ASInt32 key))

/**
	Enumerates the entries in the tree. 
	@param theTree IN/OUT A number tree. 
	@param proc IN/OUT A procedure to call once for each number destination 
	pair in <code>theTree</code>. 
	@param clientData IN/OUT Data used by the enumeration procedure. 
	<code>clientData</code> is passed to the enumeration procedure proc each 
	time a number tree is encountered. 
	@see PDNumTreeGet 
	@see PDNumTreePut 
  	@ingroup Enumerators
	@since PI_PDMODEL_VERSION >= 0x00040000
	
*/
NPROC(void, PDNumTreeEnum, (PDNumTree theTree, CosObjEnumProc proc, void* clientData))


/**
	Converts a dictionary Cos object to a font. This method 
	does not copy the object, but is instead the logical equivalent 
	of a type cast. 
	@param fontObj IN/OUT The dictionary Cos object for the font. 
	@return The PDFont for <code>fontObj</code>. It returns <code>NULL</code> if there is no CosDoc 
	or PDDoc containing <code>fontObj</code>. 
	@since PI_PDMODEL_VERSION >= 0x00040000
	
*/
NPROC(PDFont, PDFontFromCosObj, (CosObj fontObj))


/**
	In Adobe Reader or for documents that are not dirty, 
	this method copies the bytes from the document's ASFile 
	to the specified location. This also occurs if the <code>saveChanges</code> 
	field in <code>params</code> is <code>false</code>. If <code>saveChanges</code> is <code>true</code>, the document 
	is dirty, and the product is Acrobat, a full save is performed 
	to the specified file. The resulting file is linearized (optimized for the web). 
	If the file already exists, it is overwritten. 
	@param pdDoc The document to copy. 
	@param params A structure that defines how the PDF file 
	is copied.
	@exception genErrBadParm is raised if an invalid argument is passed in 
	params. 
	@exception pdErrAlreadyOpen is raised if the target and source files are 
	the same. 
	@exception fileErrDiskFull is raised if there is no space for the copy. 
	
	@exception pdErrCancelSave is raised if the save was canceled (<code>cancelProc</code> 
	in <code>params</code> returned <code>true</code>). 
	@exception pdErrUnableToRead is raised if a read error occurred on the 
	source. 
	@exception pdErrUnableToWrite is raised if a write error occurred on the 
	destination. 
	@since PI_PDMODEL_VERSION >= 0x00040005
*/
NPROC(void, PDDocCopyToFile, (PDDoc pdDoc, PDDocCopyParams params))


/**
	This method supersedes PDDocGetPermissions(). 

	<p>Checks the permissions associated with the specified 
	document using the latest permissions format, and determines 
	whether the requested operation is allowed for the specified 
	object in the document. </p>

	<p>This method first checks the requested object and operation 
	in a cached permissions list. If a value is not found, it calls the 
	document's permission handlers, followed by security handlers via PDCryptAuthorizeExProc() to 
	request permissions for the operation. The final permission is a logical <code>AND</code> of the permissions
	granted by individual permissions and/or security handlers. If the document's 
	security handler does not support this Acrobat 5.0 call, 
	the method calls PDCryptAuthorizeProc() instead. The method 
	then interprets the returned <code>PDPerms</code> to determine whether 
	the requested operation is allowed for the specified object 
	in the document. </p>

	<p>This method may throw exceptions.</p>

	@param pdDoc The PDDoc whose permissions are being requested. 
	
	@param reqObj The target object of the permissions request. 
	
	@param reqOpr The target operation of the permissions 
	request. 
	@param authData A pointer to an authorization data structure.
	@return The request status constant, <code>0</code> if the requested operation 
	is allowed, a non-zero status code otherwise. 
	@see PDDocAuthorize 
	@see PDDocGetPermissions (obsolete) 
	@see PDDocGetNewCryptHandler 
	@see PDDocSetNewCryptHandler 
	@see PDDocSetNewCryptHandlerEx 
	@see PDRegisterCryptHandler 
	@see PDRegisterCryptHandlerEx 
	@since PI_PDMODEL_VERSION >= 0x00050000
*/
NPROC(PDPermReqStatus, PDDocPermRequest, (PDDoc pdDoc, PDPermReqObj reqObj, PDPermReqOpr reqOpr, void *authData))


/**
	Returns the box specified for the page object intersected 
	with the media box. If the value for <code>boxName</code> is <code>CropBox</code>, 
	this call is equivalent to PDPageGetCropBox(); if the value 
	is <code>MediaBox</code>, this call is equivalent to PDPageGetMediaBox(). 
	
	@param page The page whose box is obtained. 
	@param boxName An ASAtom representing the type of box. 
	It can have values such as <code>ArtBox</code>, <code>BleedBox</code>, <code>CropBox</code>, <code>TrimBox</code>, or <code>MediaBox</code>. 
	
	@param box (Filled by the method) An ASFixedRect specifying 
	the page's box.
	@return <code>true</code> if the requested box was specified for the 
	page, <code>false</code> otherwise. 
	@see PDPageSetBox 
	@since PI_PDMODEL_VERSION >= 0x00050000
*/
NPROC(ASBool, PDPageGetBox, (PDPage page, ASAtom boxName,  ASFixedRect *box))

/**
	Sets the box specified by <code>boxName</code> for the page.

	<p>This method may throw exceptions.</p>

	@param page IN/OUT The page for which the box is to be set. 
	@param boxName IN/OUT An ASAtom representing the type of box. 
	The box names are: 
	<ul>
	<li><code>ArtBox</code></li>
	<li><code>BleedBox</code></li>
	<li><code>CropBox</code></li>
	<li><code>TrimBox</code></li>
	<li><code>MediaBox</code> </li>
	</ul>
	
	@param box IN/OUT An ASFixedRect specifying the coordinates to 
	set for the box. 
	@notify PDDocWillChangePages 
	@notify PDDocDidChangePages 
	@see PDPageGetBox 
	@since PI_PDMODEL_VERSION >= 0x00050000
	
*/
UNPROC(void, PDPageSetBox, (PDPage page, ASAtom boxName,  ASFixedRect box))


/**
	Removes a link annotation's action. 
	@param aLinkAnnot The link annotation whose action is 
	removed.
	@exception pdErrBadAction 
	@notify PDAnnotWillChange 
	@notify PDAnnotDidChange 
	@see PDLinkAnnotGetAction 
	@see PDLinkAnnotSetAction 
	@since PI_PDMODEL_VERSION >= 0x00050000
*/
UNPROC(void, PDLinkAnnotRemoveAction, (PDLinkAnnot aLinkAnnot))

/**
	Removes a bookmark's action. 
	@param aBookmark The bookmark whose action is removed.
	@exception pdErrBadAction 
	@notify PDBookmarkDidChange 
	@notify PDBookmarkWillChange 
	@since PI_PDMODEL_VERSION >= 0x00050000
*/
UNPROC(void, PDBookmarkRemoveAction, (PDBookmark aBookmark))

/**
	Removes the value of the OpenAction key in the Catalog dictionary. 
	The value is the action performed when the document is opened. 
	
	@param doc IN/OUT The document whose open action is removed. 
	@exception genErrBadParm 
	@exception pdErrOpNotPermitted 
	@see PDDocGetOpenAction 
	@see PDDocSetOpenAction 
	@since PI_PDMODEL_VERSION >= 0x00050000
	
*/
UNPROC(void, PDDocRemoveOpenAction, (PDDoc doc))

/**
	Sends a PDNameTreeNameAdded() notification. 
	@param theTree The PDNameTree to which a name had been 
	added. 
	@param key The name of the object within <code>theTree</code> that 
	was added. This is a Cos string, not a C string. 
	@param value (Filled by the method) The Cos object corresponding 
	to the object name that was added to <code>theTree</code>. 
	@notify PDNameTreeNameAdded 
	@see PDNameTreeNotifyNameRemoved 
	@since PI_PDMODEL_VERSION >= 0x00050000
*/
UNPROC(void, PDNameTreeNotifyNameAdded, (PDNameTree theTree, CosObj key, CosObj value))

/**
	Sends a PDNameTreeNameRemoved() notification. 
	@param theTree The PDNameTree from which the name had 
	been removed. 
	@param removedName The name within <code>theTree</code> that was removed.
	@notify PDNameTreeNameRemoved 
	@see PDNameTreeNotifyNameAdded 
	@since PI_PDMODEL_VERSION >= 0x00050000
*/
UNPROC(void, PDNameTreeNotifyNameRemoved, (PDNameTree theTree, CosObj removedName))


/**
	Returns the page Cos object corresponding to the given page 
	number. 
	@param pdd The PDDoc containing the given page. 
	@param nPage The page number.
	@return A Cos object representing the page, or an object of type 
	CosNull if the page does not exist. 
	@exception genErrBadParm 
	@exception pdErrBadPageObj 
	@since PI_PDMODEL_VERSION >= 0x00050000
*/
NPROC(CosObj, PDDocGetPageObjByNum, (PDDoc pdd, ASInt32 nPage))

/**
	Same as PDTextSelectEnumText(), except the output is forced to 
	UCS. 
	@param textP IN/OUT The text selection whose strings are enumerated. 
	
	@param proc IN/OUT A user-supplied callback to call for each string 
	in the text object. Enumeration ends if <code>proc</code> returns <code>false</code>. 
	
	@param procData IN/OUT User-supplied data to pass to <code>proc</code> each 
	time it is called. 
	@exception genErrBadParm 
	@see PDDocCreateTextSelect 
	@see PDTextSelectEnumText 
	@since PI_PDMODEL_VERSION >= 0x00050000
	
*/
NPROC(void, PDTextSelectEnumTextUCS, (PDTextSelect textP, PDTextSelectEnumTextProc proc, void *procData))

/* for 5.<code>0</code>, provide control over whether annotations are rendered */

#if !MAC_PLATFORM || (MAC_PLATFORM && !AS_ARCH_64BIT)
/**
	Provides control over the rendering of annotations on the 
	page to be drawn into <code>window</code>. It provides the ability to specify 
	the flags passed in to the PDPageDrawContentsToWindows() function. 
	@param page The page to draw into <code>window</code>. 
	@param window A pointer to a platform-dependent window object 
	(<code>HWND</code> on Windows, or <code>WindowPtr</code> or <code>CWindowPtr</code> on Mac OS). 
	On Windows, to draw into an offscreen <code>DC</code>, pass <code>NULL</code> for <code>window</code>.
	On Mac 32, to draw into an offscreen <code>GWorld</code>, pass <code>NULL</code> in 
	<code>window</code> and pass the <code>GWorld</code>Ptr in <code>displayContext</code>.  
	@param displayContext A platform-dependent display context 
	structure (<code>HDC</code> on Windows, <code>GWorldPtr</code> on Mac OS). On Mac 32, 
	<code>displayContext</code> is ignored if <code>window</code> is non-<code>NULL</code> Note that 
	<code>displayContext</code> cannot be reliably used as the <code>hDC</code> for a 
	printer device. This API is not available on Mac 64. 
	@param isDPS Currently unused. Always set it to <code>false</code>. 
	@param matrix A pointer to the matrix to concatenate onto 
	the default page matrix. It is useful for converting from 
	page to window coordinates and for scaling. 
	@param flags See PDPageDrawFlagsPI for possible values.
	@param updateRect A rectangle represented by the coordinates 
	of its four sides.
	@param cancelProc A procedure called periodically to check 
	for the user's cancelling of the drawing operation. The default cancel 
	procedure can be obtained using AVAppGetCancelProc(). It may 
	be <code>NULL</code> in which case no cancel procedure is used. 
	@param cancelProcClientData A pointer to user-supplied data 
	to pass to <code>cancelProc</code> each time it is called. It should be 
	<code>NULL</code> if <code>cancelProc</code> is <code>NULL</code>.
	@exception pdPErrUnableToCreateRasterPort 
	@see AVAppGetCancelProc 
	@see PDDrawCosObjToWindow 
	@see PDPageDrawContentsToWindow 
	
	@note <b>(Acrobat 5.0 Implementation)</b> This function can 
	only be called with a flags value of <code>0</code>. The function is 
	not supported with any other values for <code>flags</code>. <code>flags = 0</code> means do not render 
	the annotation faces. If you want to draw to a window with annotations, 
	you should call the original PDPageDrawContentsToWindow(). 
	In general, Adobe recommends that you not use PDPageDrawContentsToWindowsEx() 
	in the Acrobat 5.0 release unless you have a specific need 
	to prevent the drawing of annotations. 
	@since PI_PDMODEL_VERSION >= 0x00050000
*/
NPROC(void, PDPageDrawContentsToWindowEx,
	  (PDPage page, void *window, void *displayContext,
	   ASBool isDPS, ASFixedMatrix *matrix, ASUns32 flags,
	   ASFixedRect *updateRect, CancelProc cancelProc, void *cancelProcClientData))
#endif /* #if !MAC_PLATFORM || (MAC_PLATFORM && !AS_ARCH_64BIT) */

/**
	Retrieves the color of the specified bookmark. 

	<p>An exception is thrown if the bookmark is invalid or the existing color is malformed in the PDF file. </p>

	@param bm The bookmark in question. 
	@param pdcvOut (Filled by the method) The color of the bookmark 
	in PDColorValue format.
	@return <code>true</code> if the color was specified, <code>false</code> otherwise (the default 
	color is returned in <code>pdcvOut</code>). 
	@see PDBookmarkSetColor 
	@since PI_PDMODEL_VERSION >= 0x00050000
*/
NPROC(ASBool, PDBookmarkGetColor, (PDBookmark bm, PDColorValue pdcvOut))

/**
	Sets the color of the specified bookmark. 
	@param bm The bookmark whose color is set. 
	@param pdcvIn The new color. It must be in <code>DeviceRGB</code>.
	@exception If color is <code>NULL</code> or not in <code>DeviceRGB</code>, the bookmark is invalid or 
	the existing bookmark color is malformed. 
	@notify PDBookmarkWillChange with C as the key. 
	@notify PDBookmarkDidChange with C as the key. 
	@see PDBookmarkGetColor 
	@since PI_PDMODEL_VERSION >= 0x00020000
*/
UNPROC(void, PDBookmarkSetColor, (PDBookmark bm, PDColorValue pdcvIn))

/**
	Retrieves the flags of the specified bookmark. 

	<p>An exception is thrown if the bookmark is invalid or the existing style is malformed in the PDF file. </p>

	@param bm The bookmark whose flags are obtained.
	@return The bookmark's flags. Bit 1 (the least significant bit) indicates 
	an italic font; bit 2 indicates a bold font. 
	@see PDBookmarkSetFlags 
	@since PI_PDMODEL_VERSION >= 0x00050000
*/
NPROC(ASInt32, PDBookmarkGetFlags, (PDBookmark bm))

/**
	Sets the flags of the specified bookmark. 

	<p>An exception is thrown if the bookmark is invalid or the existing style is malformed in the PDF file. </p>

	@param bm The bookmark whose flags are set. 
	@param nFlags The new bookmark flags. Bit 1 (the least significant 
	bit) indicates an italic font; bit 2 indicates a bold font.
	@notify PDBookmarkWillChange with F as the key. 
	@notify PDBookmarkDidChange with F as the key 
	@see PDBookmarkGetFlags 
	@since PI_PDMODEL_VERSION >= 0x00050000
*/
UNPROC(void, PDBookmarkSetFlags, (PDBookmark bm, ASInt32 nFlags))

/**
	Like PDDocExportNotes(), but the caller provides the list of 
	annotations to export. This is useful in scenarios when it may 
	be inappropriate to use PDDocExportNotes() and look for annotations 
	on every page. This is an especially important consideration 
	when in a browser. 
	@param doc The document from which notes are exported. 
	
	@param unused1 Currently unused. 
	@param unused2 Currently unused. 
	@param unused3 Currently unused. 
	@param unused4 Currently unused. 
	@param exportFilter A user-supplied routine that selects 
	which notes to export. 
	@param pdanArray An array of PDAnnotArrayRec objects; the number 
	of items in the arrray is the number of pages in <code>doc</code>. 
	@param numNotesP (Filled by the method) If non-<code>NULL</code>, the 
	number of notes exported.
	@return The CosDoc of the document created to hold the exported 
	notes. 
	@notify PDDocWillExportAnnots 
	@notify PDDocDidExportAnnots 
	@see PDDocGetXAPMetadata 
	@see PDDocImportNotes 
	@see PDDocExportNotes 

	@note Make sure to explicitly include popups. 
	@since PI_PDMODEL_VERSION >= 0x00050000
*/
UNPROC(CosDoc, PDDocExportSomeNotes,
	  		 (PDDoc doc,
			  ASFileSys unused1,
			  ASPathName unused2,
			  void* unused3, /* ASProgressMonitor */
			  void* unused4,
			  PDDocWillExportAnnotCallback exportFilter,
			  PDAnnotArray pdanArray,
			  ASInt32* numNotesP))

/**
	Checks whether a page uses any transparency features. 
	
	@param pdPage The page to check. 
	@param includeAnnotAppearances If <code>true</code>, annotation appearances 
	are included in the check; if <code>false</code> annotation appearances 
	will be ignored.
	@return <code>true</code> only if the page uses any transparency 
	features. 

	@note To determine whether the page uses transparency, the 
	resources of the page must be enumerated (though the page 
	contents do not need to be parsed). The page resources may 
	not be optimized for slow (browser-based) connections, so 
	calling PDPageHasTransparency() before the page has been downloaded 
	may cause unpleasant read behavior and performance problems. 
	@since PI_PDMODEL_VERSION >= 0x00050000
*/
NPROC(ASBool, PDPageHasTransparency, (PDPage pdPage, ASBool includeAnnotAppearances))

/**
	Adds the <code>WFVersion</code> parameter to PDTextSelectCreatePageHilite(). 
	It is the same as PDTextSelectCreatePageHilite(), but it creates a WordFinder 
	using the specified version number. It is intended to be 
	used by plug-ins that want to do text highlighting with previous 
	versions of the word finder algorithm. 
	@param page The page on which the highlights appear. 
	@param hList A pointer to an array of highlight entries. 
	If the <code>length</code> field of a HiliteEntry is <code>0</code>, the entire word 
	is highlighted. <code>hList</code> should not contain multiple instances 
	of the same highlight; the display appearance is undefined 
	when it does. 
	@param listLen The number of highlight entries in <code>hList</code>. 
	
	@param WFVersion The WordFinder version: 
		
	<TABLE rules="all" cellspacing="1">
	<TR><TH>Annotation</TH><TH>Use</TH></TR>
	<TR><TD>WF_LATEST_VERSION</TD><TD>To obtain the latest available version.</TD></TR>
	<TR><TD>WF_VERSION_2</TD><TD>Version used for Acrobat 3.x, 4.x.</TD></TR>
	<TR><TD>WF_VERSION_3</TD><TD>Available in Acrobat 5.0 without Accessibility enabled. Includes some improved word-piecing algorithms.</TD></TR>
	<TR><TD>WF_VERSION_4</TD><TD>For Acrobat 5.0 with Accessibility enabled. Includes advanced word-ordering algorithms in addition to improved word-piecing algorithms.</TD></TR>
	</TABLE>

	@return The newly created text selection. 
	@see AVDocSetSelection 
	@see PDTextSelectCreateWordHilite 
	@see PDTextSelectCreateWordHiliteEx 
	@see PDTextSelectCreatePageHilite 
	@see PDTextSelectCreateRanges 
	@see PDTextSelectCreateRangesEx 
	@see PDDocCreateTextSelect 
	@see PDTextSelectDestroy 
	@since PI_PDMODEL_VERSION >= 0x00050000
*/
NPROC(PDTextSelect, PDTextSelectCreatePageHiliteEx, (PDPage page, HiliteEntry *hList, ASInt32 listLen, ASInt16 WFVersion))

/**
	Adds the <code>WFVersion</code> parameter to PDTextSelectCreateWordHilite(). 
	
	@param page The page on which the highlights appear. 
	@param hList A pointer to an array of highlight entries. 
	<code>hList</code> should not contain multiple instances of the same highlight; 
	the display appearance is undefined when it does. 
	@param listLen The number of highlight entries in <code>hList</code>. 
	
	@param WFVersion The WordFinder version: 
			
	<TABLE rules="all" cellspacing="1">
	<TR><TH>Annotation</TH><TH>Use</TH></TR>
	<TR><TD>WF_LATEST_VERSION</TD><TD>To obtain the latest available version.</TD></TR>
	<TR><TD>WF_VERSION_2</TD><TD>Version used for Acrobat 3.x, 4.x.</TD></TR>
	<TR><TD>WF_VERSION_3</TD><TD>Available in Acrobat 5.0 without Accessibility enabled. Includes some improved word-piecing algorithms.</TD></TR>
	<TR><TD>WF_VERSION_4</TD><TD>For Acrobat 5.0 with Accessibility enabled. Includes advanced word-ordering algorithms in addition to improved word-piecing algorithms.</TD></TR>
	</TABLE>

	@return The newly created text selection. 
	@see AVDocSetSelection 
	@see PDTextSelectCreateRanges 
	@see PDTextSelectCreateRangesEx 
	@see PDTextSelectCreatePageHilite 
	@see PDTextSelectCreatePageHiliteEx 
	@see PDTextSelectCreateWordHilite 
	@see PDWordFinderGetLatestAlgVersion 
	@see PDTextSelectDestroy 
	@since PI_PDMODEL_VERSION >= 0x00050000
*/
NPROC(PDTextSelect, PDTextSelectCreateWordHiliteEx, (PDPage page, HiliteEntry *hList, ASInt32 listLen, ASInt16 WFVersion))

/**
	Adds the <code>WFVersion</code> parameter to PDTextSelectCreateRanges(). 
	It is the same as PDTextSelectCreateRanges() but it creates a WordFinder 
	using the specified version number. It is intended to be used 
	by plug-ins that want to do text highlighting with previous 
	versions of the word finder algorithm. 
	@param page IN/OUT The page on which the text appears. 
	@param range IN/OUT A pointer to an array of ranges that are used 
	to create a text selection. Each array element is a PDTextSelectRange 
	structure. 
	@param rangeCount IN/OUT The number of ranges in range. 
	@param WFVersion IN/OUT The WordFinder version:
			
	<TABLE rules="all" cellspacing="1">
	<TR><TH>Annotation</TH><TH>Use</TH></TR>
	<TR><TD>WF_LATEST_VERSION</TD><TD>To obtain latest the available version.</TD></TR>
	<TR><TD>WF_VERSION_2</TD><TD>Version used for Acrobat 3.x, 4.x.</TD></TR>
	<TR><TD>WF_VERSION_3</TD><TD>Available in Acrobat 5.0 without Accessibility enabled. Includes some improved word-piecing algorithms.</TD></TR>
	<TR><TD>WF_VERSION_4</TD><TD>For Acrobat 5.0 with Accessibility enabled. Includes advanced word-ordering algorithms in addition to improved word-piecing algorithms.</TD></TR>
	</TABLE>

	
	@return A text selection created from the specified ranges. 
	@see AVDocSetSelection 
	@see PDTextSelectCreatePageHilite 
	@see PDTextSelectCreatePageHiliteEx 
	@see PDTextSelectCreateRanges 
	@see PDTextSelectCreateWordHilite 
	@see PDTextSelectCreateWordHiliteEx 
	@see PDTextSelectDestroy 
	@since PI_PDMODEL_VERSION >= 0x00050000
	
*/
NPROC(PDTextSelect, PDTextSelectCreateRangesEx, ( PDPage page, PDTextSelectRange range, ASInt32 rangeCount, ASInt16 WFVersion))

#if !MAC_PLATFORM || (MAC_PLATFORM && !AS_ARCH_64BIT)
/**
	Useful for obtaining the static, platform-specific palette; 
	the bitmap must be already selected into the <code>displayContext</code> 
	to get the palette. This API was exposed for the purpose 
	of the ImageConversion plug-in. When that code uses PDPageDrawContentsToWindow() 
	to get a bitmap from AGM, it needs the palette that AGM 
	used in order to get the correct results. This API is not available on Mac 64.
	@param page The page whose palette is obtained. 
	@param displayContext The bitmap. 
	@param table (Filled by the method) The palette.
	@return <code>true</code> if the palette was returned, <code>false</code> otherwise. 
	
	@since PI_PDMODEL_VERSION >= 0x00050000
*/
NPROC(ASBool, PDPageGetPalette, ( PDPage page, void *displayContext, char *table ))
#endif /* #if !MAC_PLATFORM || (MAC_PLATFORM && !AS_ARCH_64BIT) */


/* Separations API
**
*/

/**
	Enumerates the inks for a page, calling the supplied procedure 
	for each PDPageInk structure. 

	<p>For the DeviceCMYK_K process color model, it always finds the 
	four inks Cyan, Magenta, Yellow, and Black, which are marked 
	as process inks. The RGB values in the PDPageInk structure 
	are the RGB equivalents (in system monitor space) of 100% 
	of the ink, which can be used to show color swatches for 
	a given ink. </p>

	<p>If the inks are part of a DeviceN colorspace which has not 
	been defined in a Colorants dictionary or elsewhere in a 
	Separation colorspace, the color of the swatch is undefined. </p>
	
	<p>This call finds all color spaces that are in a color space 
	dictionary within the page, even if they are not used by 
	the page contents. </p>
	@param pdPage The page whose contents are enumerated. 
	@param proc The user-supplied callback procedure to be 
	applied to each ink. Enumeration ends if any procedure returns 
	<code>false</code>. 
	@param clientData A pointer to user-supplied data to pass 
	to <code>proc</code> each time it is called.
	@param includeOPI If <code>true</code>, enumerate inks contained in OPI dictionaries.
	@see PDPageMakeSeparations 
	@see AVPageViewGetNumVisibleInks 
	@see AVPageViewGetVisibleInks 
	@see AVPageViewGetPixelInformationAtPoint 
	@see AVPageViewSetInkPreview 
	@see AVPageViewSetVisibleInks 
	@since PI_PDMODEL_VERSION >= 0x00060000
*/
NPROC(void, PDPageEnumInks, ( PDPage pdPage, PDPageEnumInksCallback proc, void *clientData, ASBool includeOPI ))

#if !defined(IOS_ENV)
/**
	Generates print color separations for a page. 

	<p>This is the entry point for creating separations for a single 
	page. The spec structure contains an array of PDHostSepsPlate 
	pointers, (typically based on the page inks reported by 
	PDPageEnumInks()), with settings such as what to do on each 
	plate and the output stream for plates that are being produced. 
	The client owns the memory for the array and all of the 
	records in it, and is responsible for disposing of all allocated 
	memory. </p>

	<p>On completion, the marked flags in the <code>wasColorSet</code> field 
	of the plates indicate whether each plate was marked, meaning that 
	any marking operation happened, even if it was clipped 
	away or knocked out later. The special All colorant in a 
	Separation color space does not affect the marked flags.</p> 
	
	<p>For Adobe Reader and Acrobat Standard, this method 
	does nothing. </p>
	@param pdPage The page. 
	@param spec The separation specification structure containing 
	parameters for the generation.
	@see AVDocPrintSeparations 
	@see PDPageEnumInks 
	@see PDPageMakeSeparations 
	@since PI_PDMODEL_VERSION >= 0x00060000
*/
NPROC(void, PDPageMakeSeparations, ( PDPage pdPage, PDHostSepsSpec spec ))
#endif
/* Additional Wordy APIs for Acrobat 6.0 */
/**
	This is a version 6.0 replacement for PDDocCreateWordFinder() 
	and PDDocCreateWordFinderUCS() that adds configurable word-breaking 
	behavior. This method creates a word finder that is used 
	to extract text from a PDF file, according to the given 
	configuration. The word finder can be used to enumerate 
	words one-by-one or to fill a table with all the words on 
	a page. You can choose to find only words that are visible 
	in a given context. 

	<p>You can use version 6.0 methods such as PDWordGetCharOffsetEx() 
	to extract character information from words if you create 
	the word finder with WF_VERSION_3 or later. </p>
	
	@param doc The document on which the word finder is used. 
	
	@param algVersion The version of the word-finding algorithm 
	to use (see PDExpT.h), as follows (pass <code>0</code> if your client 
	does not care): 
			
	<TABLE rules="all" cellspacing="1">
	<TR><TH>Annotation</TH><TH>Use</TH></TR>
	<TR><TD>WF_LATEST_VERSION</TD><TD>To obtain latest available version.</TD></TR>
	<TR><TD>WF_VERSION_2</TD><TD>Version used for Acrobat 3.x, 4.x.</TD></TR>
	<TR><TD>WF_VERSION_3</TD><TD>Available in Acrobat 5.0 and 6.0 without Tagged PDF support. </TD></TR>
	<TR><TD>WF_VERSION_4</TD><TD>For Acrobat 5.0 and 6.0 with Tagged PDF support.</TD></TR>
	</TABLE>
	
	@param outUnicode Whether to return Unicode. When <code>true</code>, 
	the word finder encodes the extracted text in Unicode format. 
	Otherwise, the word finder extracts the text in the host 
	encoding. 
	@param wbConfig A pointer to a configuration record for 
	the new word finder that customizes the way the extraction 
	is performed. The configuration is only used if the algorithm 
	version is WF_VERSION_3 or higher. When it is <code>NULL</code>, the default 
	configuration is used.
	@return The newly created word finder object. 
	@see PDDocCreateWordFinder 
	@see PDDocCreateWordFinderUCS 
	@see PDWordFinderEnumWords 
	@see PDWordFinderAcquireWordList 
	@see PDWordFinderDestroy 
	@see PDWordFilterWord 
	@see PDWordGetCharOffsetEx 

	@note The word finder also extracts text from Form XObjects 
	that are executed in the page contents. For information 
	about Form XObjects, see Section 4.9 in the <i>PDF Reference</i>. 
	@since PI_PDMODEL_VERSION >= 0x00060000
*/
NPROC(PDWordFinder, PDDocCreateWordFinderEx, (PDDoc doc, ASInt16 algVersion, ASBool outUnicode, PDWordFinderConfig wbConfig))

/**
	This is a version 6.0 extension of PDWordGetCharOffset() that 
	can be used only with a word finder created with algorithm 
	version WF_VERSION_3 or higher. 

	<p>It returns the character offset for a character identified 
	by its index number, and the number of bytes (length) used 
	for that character. The length is usually <code>1</code> for single-byte 
	characters and <code>2</code> for double-byte characters. If multiple 
	bytes are used to construct one character, only the first 
	byte has valid character offset information and the other 
	bytes have zero offset length with the same character offset 
	of the first byte. If the returned offset length is zero, 
	it means the specified byte in the word is a part (other 
	than the first byte) of a multi-byte character. </p>

	<p>The character offset is the character position calculated 
	in bytes from the beginning of a page. Because of the encoding 
	conversions and character replacements applied by the word 
	finder, some characters may have different byte lengths 
	from the original PDF content. The character offset itself 
	can locate a character in the PDF content. However, without 
	the offset length (that is the number of bytes in the PDF 
	content), clients cannot tell whether two characters are 
	next to each other in the PDF content. For example, 
	suppose you want to create a Text Select object of two characters 
	at character offset <code>1</code> and <code>3</code>. You can create an object with 
	two disconnected ranges of <code>[Offset 1, The length 1]</code> and <code>[Offset 
	3, The length 1]</code>. However, if you know that the offset length 
	of both characters is <code>2</code>, you can create a simpler object 
	with a single range of <code>[Offset 1, The length 4]</code>. </p>

	@param word The word whose character offset is obtained. 
	
	@param byteIdx The byte index within the word of the character 
	whose offset is obtained. Valid values are <code>0</code> to <code>PDWordGetLength(word)-1</code>. 
	@param bytesConsumed (Filled by method) Returns the number 
	of bytes in the word that are occupied by the specified 
	character. It can be <code>NULL</code> if it is not needed. Use <code>(byteIdx + *bytesConsumed)</code> 
	to get the byte index of the next character in the word. 
	
	@param offsetLen (Filled by the method) Returns the number 
	of bytes occupied by the specified character in the original 
	PDF content. This is <code>0</code> if the specified byte is not the 
	starting byte of a character in the PDF content. It can be 
	<code>NULL</code> if it is not needed.
	@return The word's character offset and the number of bytes occupied 
	by the character. 
	@see PDWordGetCharOffset 
	@see PDWordGetCharDelta 
	@see PDWordGetLength 
	@see PDTextSelectCreatePageHilite 
	@since PI_PDMODEL_VERSION >= 0x00060000
*/
NPROC(ASUns32, PDWordGetCharOffsetEx, (PDWord word, ASUns32 byteIdx, ASUns32* bytesConsumed, ASUns32* offsetLen))

/**
	Gets the quadrilateral bounding of the character at a given 
	index position in the word. If the specified character is 
	constructed with multiple bytes, only the first byte returns 
	a valid quad. Otherwise, this method returns <code>false</code>. 

	<p>This method can be used only with a word finder created 
	with algorithm version WF_VERSION_3 or higher. </p>

	@param word The word whose character offset is obtained. 
	
	@param byteIdx The byte index within the word of the character 
	whose quad is obtained. Valid values are <code>0</code> to <code>PDWordGetLength(word)-1</code>.
	@param quad (Filled by method) A pointer to an existing 
	quad structure in which to return the character's quad specified 
	in user-space coordinates.
	@return <code>true</code> if the provided byte index is the beginning 
	byte of a character and a valid quad is returned, <code>false</code> 
	otherwise. 
	@see PDWordGetCharOffsetEx 
	@see PDWordGetNthQuad 
	@see PDWordGetNumQuads 
	@since PI_PDMODEL_VERSION >= 0x00060000
*/
NPROC(ASBool, PDWordGetCharQuad, (PDWord word, ASUns32 byteIdx, ASFixedQuad* quad))

/**
	<p>Gets the number of <i>highlightable</i> characters in a word. A 
	highlightable character is the minimum text unit that Acrobat 
	can select and highlight. This method can be used only with a word finder created 
	with algorithm version WF_VERSION_3 or higher.</p>

	<p>Because of the encoding conversion, the characters in a 
	word finder word list do not have a 1-to-1 correspondence 
	to the characters displayed by Acrobat. For example, if 
	the word is <code>"fish"</code> and the text operation in PDF content 
	is <code>"fi"</code>(ligature) <code>+ 's' + 'h'</code>, this method returns the number 
	of highlightable characters as <code>3</code>, counting <code>"fi"</code> as one character. 
	For the same word, the PDWordGetLength() method returns the 
	byte-length as <code>4</code>. </p>

	@param word The word whose highlightable character count 
	is obtained.
	@return The number of highlightable characters in <code>word</code>. 
	@see PDWordGetLength 
	@since PI_PDMODEL_VERSION >= 0x00060000
*/
NPROC(ASUns32, PDWordGetNumHiliteChar, (PDWord word))

/**
	<p>Returns the byte offset within the specified word of the 
	highlightable character at the specified character offset. 
	The first character of a word is at byte offset <code>0</code>. 
	This method can be used only with a word finder created 
	with algorithm version WF_VERSION_3 or higher.</p>

	<p>The returned byte offset can be passed to PDWordGetCharOffsetEx() 
	and PDWordGetCharQuad() to get additional information. Use 
	PDWordGetNumHiliteChar() to get the number of highlightable 
	characters in a word. </p>

	@param word The word containing the character. 
	@param charIdx The character index within the word.
	@return The byte offset of the specified character within the word, 
	or <code>0</code> if the character index is out of range. 
	@see PDWordGetCharOffsetEx 
	@see PDWordGetCharQuad 
	@see PDWordGetNumHiliteChar 
	@since PI_PDMODEL_VERSION >= 0x00060000
*/
NPROC(ASUns32, PDWordGetByteIdxFromHiliteChar, (PDWord word, ASUns32 charIdx))

/**
	Copies the text from a word into an ASText object. It automatically 
	performs the necessary encoding conversions from the specified 
	word (either in Unicode or Host Encoding) to the ASText 
	object. 
	@param word The word whose text becomes the new ASText. 
	
	@param filter Character types to be dropped from the output 
	string. For example, the following returns text without 
	soft hyphens and accent marks: 
	<p><code>PDWordGetASText(word, W_SOFT_HYPHEN + W_ACCENT, mystr);</code> </p>
	@param str An existing ASText object whose content will 
	be replaced by the new text.
	@since PI_PDMODEL_VERSION >= 0x00060000
*/
NPROC(void, PDWordGetASText, (PDWord word, ASUns32 filter, ASText str))

/**
	Gets the WordFinder Character Encoding Flags for each character 
	in a word, which specify how reliably the word finder identified 
	the character encoding. 

	<p>This method can be used only with a word finder created 
	with algorithm version WF_VERSION_3 or higher. </p>

	@param word The word whose character encoding flags are 
	obtained. 
	@param fList (Filled by the method) An array of character 
	encoding flags types. This array contains one element for 
	each byte of text in the word. The byte length of the text 
	can be determined with PDWordGetLength(). Each element is 
	the logical <code>OR</code> of one or more of the character encoding 
	flags. 
	@param size The maximum number of elements in the array 
	<code>fList</code>.
	@see PDWordGetAttrEx 
	@see PDWordGetLength 
  	@ref WordFinderCharacterEncodingFlags
	@since PI_PDMODEL_VERSION >= 0x00060000
*/
NPROC(void, PDWordGetCharEncFlags, (PDWord word, ASUns32 *fList, ASUns32 size))

/**
	This is a version 6.0 extension of PDWordGetAttr() that can 
	be used only with a word finder created with algorithm version 
	WF_VERSION_3 or higher. It can get an additional 16-bit 
	flag group defined in Acrobat 6. 

	<p>It gets a bit field containing information on the types of 
	characters in a word. Use PDWordGetCharacterTypes() if you 
	wish to check each character's type individually. </p>

	@param word The word whose character types are obtained. 
	
	@param groupID The group number of the Word Attributes 
	flags:
	<ul>
	<li><code>0</code>, the default, is the first 16-bit group, and is the same as PDWordGetAttr().</li>
	<li><code>1</code> gets the second group defined in Acrobat 6.</li>
	</ul>
	
	@return A bit field containing information on the types of characters 
	in <code>word</code>. The value is a logical <code>OR</code> of the Word Attributes. 
	@note PDWordGetAttr() may return an attribute value greater 
	than the maximum of all of the public attributes, since there 
	can be private attributes added on. It is recommended that 
	you <code>AND</code> the result with the attribute you are interested 
	in. 
	@see PDWordGetCharacterTypes 
	@see PDWordGetStyleTransition 
	@see PDWordGetNthCharStyle 
	@since PI_PDMODEL_VERSION >= 0x00060000
*/
NPROC(ASUns16, PDWordGetAttrEx, (PDWord word, ASUns32 groupID))

/**
	Creates a text selection object for a given page that includes 
	all words in a word list, as returned from a <code>PDWordFinder</code> 
	method. The text selection can then be set as the current 
	selection using AVDocSetSelection().

	@param page The page on which to select the words. 
	@param wList The word list to be selected. 
	@param wListLen The number of words in the word list.
	@return The newly created text selection. 
	@see PDDocCreateTextSelect 
	@see PDTextSelectDestroy 
	@see AVDocSetSelection 
	@see PDTextSelectEnumQuads 
	@see PDTextSelectEnumText 

	@note For consistent text selection behavior, avoid using 
	other PDTextSelect creation methods which depend on the 
	word finder versions and word offsets. These include PDTextSelectCreatePageHiliteEx(), 
	PDTextSelectCreateRanges(), PDTextSelectCreateRangesEx(), PDTextSelectCreateWordHilite(), 
	and PDTextSelectCreateWordHiliteEx(). 
	@since PI_PDMODEL_VERSION >= 0x00060000
*/
NPROC(PDTextSelect, PDWordCreateTextSelect, (PDPage page, PDWord* wList, ASUns32 wListLen))

/**
	Constructs a PDWord list from a Unicode string, and calls 
	a user-supplied procedure once for each word found. 

	<p>The words extracted by this method do not have quads, text 
	style, or text selection information. The character offset 
	is calculated from the beginning of the input string, and 
	is increased by <code>2</code> on every 16 bits of data (the character 
	offset of a character in a PDWord is the byte offset of 
	the character in the source Unicode string). </p>

	@param wObj A word finder object. 
	@param ucsStr A pointer to the Unicode string. 
	@param strLen The length of the string in bytes. 
	@param charOffsetAdj The character offset value of the 
	first character in the input Unicode string. This value 
	is added to the word character offsets, and is used to maintain 
	contiguous word character offsets when multiple strings 
	(and multiple calls to this method) are combined into one 
	word list. 
	
	<p>For example:</p> 
	<code>
	<p>PDWordFinderEnumWordsStr(wf, str1, stelen(str1), <code>0</code>, wp, d); </p>
	<p>PDWordFinderEnumWordsStr(wf, str2, stelen(str2), stelen(str1), wp, d); </p>
	</code>

	@param wordProc A user-supplied callback to call once for 
	each word found. Enumeration halts if <code>wordProc</code> returns <code>false</code>. 
	
	@param clientData A pointer to user-supplied data to pass 
	to <code>wordProc</code> each time it is called.
	@return <code>true</code> if the enumeration was successfully completed, 
	<code>false</code> if the enumeration was terminated because <code>wordProc</code> returned 
	<code>false</code>. 
	@exception genErrBadParm is raised if <code>wordProc</code> is <code>NULL</code>. 
	@exception pdErrOpNotPermitted 
	@see PDDocCreateWordFinder 
	@see PDDocCreateWordFinderUCS 
	@see PDDocGetWordFinder 
	@see PDWordFinderAcquireWordList 
	@see PDWordFinderEnumVisibleWords 
	@see PDWordFinderEnumWords 
	@since PI_PDMODEL_VERSION >= 0x00060000
*/
NPROC(ASBool, PDWordFinderEnumWordsStr, (PDWordFinder wObj, const ASUTF16Val *ucsStr, ASUns32 strLen, ASUns32 charOffsetAdj, PDWordProc wordProc, void* clientData))

/* Routines for copying and pasting PDActions */

/**
	Registers a handler for PDAction operations. 
	@param handler A pointer to a structure containing the action 
	handler's callbacks. This structure must not be freed after 
	this call, but must be retained.
	@since PI_PDMODEL_VERSION >= 0x00060000
*/
UNPROC(void, PDRegisterActionHandler, (PDActionHandler handler))

/**
	Tests whether the data from an action object can be copied 
	to a clipboard for pasting. If the action is part of an 
	action chain, the method tests all actions in the chain, 
	and returns <code>true</code> only if all actions in the chain can be 
	copied. 
	@param action The action to test.
	@return <code>true</code> if the action object or all actions in the action chain 
	can be copied, <code>false</code> otherwise. 
	@see PDActionCanPaste 
	@see PDActionCopy 
	@see PDActionPaste 
	@see PDActionDestroyClipboardData 
	@since PI_PDMODEL_VERSION >= 0x00060000
*/
UNPROC(ASBool, PDActionCanCopy, (PDAction action))

/**
	Copies action object data to a clipboard structure, from 
	which it can be pasted. When the PDActionClipboardData is 
	no longer required, it must be explicitly freed using PDActionDestroyClipboardData(). 
	
	@param action The action to copy.
	@return The action clipboard data object. 
	@see PDActionCanCopy 
	@see PDActionCanPaste 
	@see PDActionPaste 
	@see PDActionDestroyClipboardData 
	@since PI_PDMODEL_VERSION >= 0x00060000
*/
UNPROC(PDActionClipboardData, PDActionCopy, (PDAction action))

/**
	Tests whether data from an action object that has been copied 
	to a clipboard can be pasted into a destination document. 
	It tests, for example, whether pasting is allowed by document 
	permissions. 
	@param dest The destination document.
	@param data The action data to test.
	@return <code>true</code> if the action data can be pasted to the document, <code>false</code> 
	otherwise. 
	@see PDActionCanCopy 
	@see PDActionCopy 
	@see PDActionPaste 
	@see PDActionDestroyClipboardData 
	@since PI_PDMODEL_VERSION >= 0x00060000
*/
UNPROC(ASBool, PDActionCanPaste, (PDDoc dest, PDActionClipboardData data))

/**
	Creates a new PDAction in the destination document, using 
	clipboard data generated by PDActionCopy(). If the original 
	PDAction was an action chain, the entire action chain is 
	recreated. The returned PDAction is the first item in the 
	chain. 

	<p>When the data is no longer needed, use PDActionDestroyClipboardData() 
	to free the structure. </p>

	@param dest The destination document for the paste operation. 
	
	@param data The clipboard structure holding the copied 
	action data.
	@return A newly created action object (or the first such object 
	in the action chain) associated with the specified document, 
	containing the same data as the copied action. 
	@see PDActionCanCopy 
	@see PDActionCanPaste 
	@see PDActionCopy 
	@see PDActionDestroyClipboardData 
	@since PI_PDMODEL_VERSION >= 0x00060000
*/
UNPROC(PDAction, PDActionPaste, (PDDoc dest, PDActionClipboardData data))

/**
	Destroys data that has been copied from an action object 
	into a clipboard. Use this method when the clipboard data 
	is no longer needed. 
	@param data The clipboard action data to destroy.
	@see PDActionCanCopy 
	@see PDActionCanPaste 
	@see PDActionCopy 
	@see PDActionPaste 
	@since PI_PDMODEL_VERSION >= 0x00060000
*/
UNPROC(void, PDActionDestroyClipboardData, (PDActionClipboardData data))

/* BEGIN Optional Content API calls */

/**
	Creates a new optional-content group (OCG) object in the 
	document. The order of the groups (as returned by PDDocGetOCGs()) 
	is not guaranteed, and is not the same as the display order 
	(see PDOCConfigGetOCGOrder()). 
	@param pdDoc The document in which the group is used. 
	@param name The name of the optional-content group.
	@return The newly created group object. 
	@see PDDocGetOCGs 
	@see PDOCGCreateFromCosObj 
	@see PDOCGDestroy 
	@since PI_PDMODEL_VERSION >= 0x00060000
*/
NPROC( PDOCG, PDOCGCreate, (PDDoc pdDoc, ASConstText name) )

/**
	Creates a new optional-content group (OCG) object from a 
	Cos object. 
	@param ocgObj The Cos object.
	@return The newly created OCG object. 
	@see PDOCGCreate 
	@see PDOCGGetCosObj 
	@see PDOCGDestroy 
	@since PI_PDMODEL_VERSION >= 0x00060000
*/
NPROC( PDOCG, PDOCGCreateFromCosObj, (CosObj ocgObj) )

/**
	Destroys an optional-content group (OCG) object. This does 
	not delete any content, but deletes the PDOCG object, destroys 
	the corresponding Cos object, and invalidates references 
	from optional-content membership dictionaries (OCMDs). 
	@param pdocg The optional-content group object.
	@see PDOCGCreate 
	@see PDOCGCreateFromCosObj 
	@since PI_PDMODEL_VERSION >= 0x00060000
*/
NPROC( void, PDOCGDestroy, (PDOCG pdocg) )

/**
	Gets the Cos object associated with the optional-content 
	group (OCG) object. 
	@param pdocg The optional-content group object.
	@return The Cos object. 
	@see PDOCGCreateFromCosObj 
	@since PI_PDMODEL_VERSION >= 0x00060000
*/
NPROC( CosObj, PDOCGGetCosObj, (PDOCG pdocg) )

/**
	Gets an optional-content group (OCG) object from the associated 
	Cos object. If you call this multiple times for the same 
	PDOCG, it returns the same object. 
	@param obj The Cos object.
	@return The OCG object. 
	@see PDOCGCreate 
	@see PDOCGCreateFromCosObj 
	@see PDOCGGetCosObj 
	@see PDOCGDestroy 
	@since PI_PDMODEL_VERSION >= 0x00060000
*/
NPROC( PDOCG, PDOCGGetFromCosObj, (CosObj obj) )

/**
	Gets the document that contains an optional-content group. 
	
	@param pdocg The optional-content group object for which 
	the document is desired.
	@return The document object. 
	@see PDDocGetOCGs 
	@see PDOCMDGetPDDoc 
	@since PI_PDMODEL_VERSION >= 0x00060000
*/
NPROC( PDDoc, PDOCGGetPDDoc, (PDOCG pdocg) )

/**
	Sets the name of an optional-content group. 
	@param pdocg The optional-content group object. 
	@param name The new name string.
	@see PDOCGGetName 
	@since PI_PDMODEL_VERSION >= 0x00060000
*/
NPROC( void, PDOCGSetName, (PDOCG pdocg, ASConstText name) )

/**
	Gets the name of an optional-content group. The returned 
	ASText is a copy of the OCG's name. The client is free to 
	modify it and responsible for destroying it. 
	@param pdocg The optional-content group object for which 
	the name is desired.
	@return The name string. 
	@see PDOCGSetName 
	@since PI_PDMODEL_VERSION >= 0x00060000
*/
NPROC( ASText, PDOCGGetName, (PDOCG pdocg) )

/**
	Sets the initial state (<code>ON</code> or <code>OFF</code>) of the optional-content 
	group (OCG) object in a given configuration. 
	@param pdocg The optional-content group object. 
	@param pdOCCfg The configuration for which to set the 
	group's initial state. 
	@param onOff The new initial state, <code>true</code> if the state 
	is <code>ON</code>, <code>false</code> if it is <code>OFF</code>.
	@see PDOCContextGetOCGStates 
	@see PDOCContextSetOCGStates 
	@see PDOCGGetCurrentState 
	@see PDOCGGetInitialState 
	@see PDOCGRemoveInitialState 
	@see PDOCGSetCurrentState 
	@since PI_PDMODEL_VERSION >= 0x00060000
*/
NPROC( void, PDOCGSetInitialState, (PDOCG pdocg, PDOCConfig pdOCCfg, ASBool onOff) )

/**
	Gets a initial state (<code>ON</code> or <code>OFF</code>) of the optional-content 
	group (OCG) object in a given configuration. If the configuration 
	has a <code>BaseState</code> of <code>Unchanged</code>, and the OCG is not listed 
	explicitly in its <code>ON</code> list or <code>OFF</code> list, then the initial 
	state is taken from the OCG's current state in the document's 
	default context, and the method returns <code>false</code>. 
	@param pdocg The optional-content group object. 
	@param pdOCCfg The configuration for which to get the 
	group's initial state. 
	@param initState (Filled by the method) The initial state, 
	<code>true</code> if the state is <code>ON</code>, <code>false</code> if it is <code>OFF</code>. 
	@return <code>true</code> if the initial state is unambiguously defined 
	in the configuration, <code>false</code> otherwise. 
	@see PDOCGGetCurrentState 
	@see PDOCGGetUsageEntry 
	@see PDOCGRemoveInitialState 
	@see PDOCGSetInitialState 
	@see PDOCContextGetOCGStates 
	@see PDOCContextSetOCGStates 
	@since PI_PDMODEL_VERSION >= 0x00060000
*/
NPROC( ASBool, PDOCGGetInitialState, (PDOCG pdocg,  PDOCConfig pdOCCfg, ASBool* initState) )

/**
	Removes the initial <code>ON-OFF</code> state information for the optional-content 
	group (OCG) object in a given configuration. 
	@param pdocg The optional-content group object. 
	@param pdOCCfg The configuration for which to remove the 
	group's initial state.
	@see PDOCGGetCurrentState 
	@see PDOCGGetInitialState 
	@see PDOCGGetUsageEntry 
	@see PDOCContextGetOCGStates 
	@see PDOCContextSetOCGStates 
	@see PDOCGSetCurrentState 
	@see PDOCGSetInitialState 
	@since PI_PDMODEL_VERSION >= 0x00060000
*/
NPROC( void, PDOCGRemoveInitialState, (PDOCG pdocg, PDOCConfig pdOCCfg) )

/**
	Sets a Usage dictionary entry in an optional-content group 
	(OCG) object. The entry associates usage information with 
	an entry key for retrieval. If a dictionary does not exist, 
	the method creates one. 

	<p>A Usage dictionary entry provides more specific intended 
	usage information than an intent entry. The possible key values 
	are:</p>
	<ul>
	<li><code>CreatorInfo</code></li>
	<li><code>Language</code></li>
	<li><code>Export</code></li>
	<li><code>Zoom</code></li>
	<li><code>Print</code></li>
	<li><code>View</code></li>
	<li><code>User</code></li>
	<li><code>PageElement</code></li> 
	</ul>
	
	<p>The usage value can act as a kind of metadata, describing 
	the sort of things that belong to the group, such as 
	text in French, fine detail on a map, or a watermark. The 
	usage values can also be used by the <code>AutoState</code> mechanism 
	to make decisions about what groups should be on and what 
	groups should be off. The <code>AutoState</code> mechanism considers 
	the usage information in the OCGs, the AS array of the configuration, 
	and external factors; for example, the language the application 
	is running in, the current zoom level on the page, or if 
	the page is being printed. </p>

	@param pdocg The optional-content group object. 
	@param usagekey The usage entry key. 
	@param usageinfo The usage information to associate with 
	the key.
	@see PDOCGGetUsageEntry 
	@see PDOCGHasUsageInfo 
	@since PI_PDMODEL_VERSION >= 0x00060000
*/
NPROC( void, PDOCGSetUsageDictEntry, (PDOCG pdocg, ASAtom usagekey, CosObj usageinfo) )

/**
	Tests whether an optional-content group (OCG) object is 
	associated with a Usage dictionary. 
	@param pdocg The optional-content group object.
	@return <code>true</code> if the group has a Usage dictionary, <code>false</code> 
	otherwise. 
	@see PDOCGGetUsageEntry 
	@see PDOCGSetUsageDictEntry 
	@since PI_PDMODEL_VERSION >= 0x00060000
*/
NPROC( ASBool, PDOCGHasUsageInfo, (PDOCG pdocg) )

/**
	Gets usage information from an optional-content group (OCG) 
	object. A Usage dictionary entry provides more specific 
	intended usage information than an intent entry. The possible 
	key values are: 

	<ul>
    <li><code>CreatorInfo</code></li>
	<li><code>Language</code></li>
	<li><code>Export</code></li>
	<li><code>Zoom</code></li>
	<li><code>Print</code></li>
	<li><code>View</code></li>
	<li><code>User</code></li>
	<li><code>PageElement</code></li> 
	</ul>
	
	@param pdocg The optional-content group object. 
	@param entry The usage key in the usage dictionary entry.
	@return The usage information associated with the given key in the 
	Usage dictionary for the group, or a <code>NULL</code> Cos object if 
	the operation fails (because the OCG is malformed or has 
	no dictionary, or because the dictionary has no entry corresponding 
	to the given key). 
	@see PDOCGHasUsageInfo 
	@see PDOCGSetUsageDictEntry 
	@since PI_PDMODEL_VERSION >= 0x00060000
*/
NPROC( CosObj, PDOCGGetUsageEntry, (PDOCG pdocg, ASAtom entry) )

/**
	Sets the Intent entry in an optional-content group's Cos 
	dictionary. An intent is an ASAtom value broadly describing 
	the intended use, which can be either <code>View</code> or <code>Design</code>. 

	<p>A group's content is considered to be optional (that is, 
	the group's state is considered in its visibility) if any 
	intent in its list matches an intent of the context. The 
	intent list of the context is usually set from the intent 
	list of the document configuration. </p>

	@param pdocg The optional-content group object for which 
	the intent is desired. 
	@param intent The new Intent entry value, an array of 
	atoms terminated with ASAtomNull.
	@see PDOCGGetIntent 
	@see PDOCContextSetIntent 
	@see PDOCConfigSetIntent 
	@see PDOCGUsedInOCConfig 
	@see PDOCGUsedInOCContext 
	@since PI_PDMODEL_VERSION >= 0x00060000
*/
NPROC( void, PDOCGSetIntent, (PDOCG pdocg, ASAtom *intent) )

/**
	Gets the intent list for an optional-content group. An intent 
	is an ASAtom value broadly describing the intended use, 
	either <code>View</code> or <code>Design</code>. 

	<p>A group's content is considered to be optional (that is, 
	the group's state is considered in its visibility) if any 
	intent in its list matches an intent of the context. The 
	intent list of the context is usually set from the intent 
	list of the document configuration. </p>

	@param pdocg The optional-content group object for which 
	the intent is desired.
	@return An array containing intent entries (ASAtom objects) terminated 
	by ASAtomNull. The client is responsible for freeing it 
	using ASfree(). 
	@see PDOCGSetIntent 
	@see PDOCContextGetIntent 
	@see PDOCConfigGetIntent 
	@see PDOCGUsedInOCConfig 
	@see PDOCGUsedInOCContext 
	@since PI_PDMODEL_VERSION >= 0x00060000
*/
NPROC( ASAtom*, PDOCGGetIntent, (PDOCG pdocg) )

/**
	Gets the current <code>ON-OFF</code> state of the optional-content group 
	(OCG) object in a given context. 
	@param pdocg The optional-content group object. 
	@param ocContext The context for which to get the group's 
	state.
	@return <code>true</code> if the state is <code>ON</code>, <code>false</code> if it is <code>OFF</code>. 
	@see PDOCGGetInitialState 
	@see PDOCGGetUsageEntry 
	@see PDOCContextGetOCGStates 
	@see PDOCContextSetOCGStates 
	@see PDOCGSetCurrentState 
	@since PI_PDMODEL_VERSION >= 0x00060000
*/
NPROC( ASBool, PDOCGGetCurrentState, (PDOCG pdocg, PDOCContext ocContext) )

/**
	Sets the current <code>ON-OFF</code> state of the optional-content group 
	(OCG) object in a given context. 
	@param pdocg The optional-content group object. 
	@param ocContext The context for which to set the group's 
	state. 
	@param newState The new state.
	@see PDOCContextGetOCGStates 
	@see PDOCContextSetOCGStates 
	@see PDOCGGetCurrentState 
	@see PDOCGGetInitialState 
	@see PDDocGetOCContext 
	@see PDOCGRemoveInitialState 
	@see PDOCGSetInitialState 
	@since PI_PDMODEL_VERSION >= 0x00060000
*/
NPROC( void, PDOCGSetCurrentState, (PDOCG pdocg, PDOCContext ocContext, ASBool newState) )

/**
	Tests whether an optional-content group (OCG) object is 
	used in a given context. 

	<p>A group's content is considered to be optional (that is, 
	the group's state is considered in its visibility) if any 
	intent in its list matches an intent of the context. The 
	intent list of the context is usually set from the intent 
	list of the document configuration. </p>

	@param pdocg The optional-content group object. 
	@param pdocctx The optional-content context.
	@return <code>true</code> if the group is taken into consideration when 
	determining the visibility of content, <code>false</code> otherwise. 
	
	@see PDOCConfigGetIntent 
	@see PDOCContextGetIntent 
	@see PDOCGGetIntent 
	@see PDOCGUsedInOCConfig 
	@since PI_PDMODEL_VERSION >= 0x00060000
*/
NPROC( ASBool, PDOCGUsedInOCContext, (PDOCG pdocg, PDOCContext pdocctx) )

/**
	Tests whether an optional-content group (OCG) object is 
	used in a context initialized using the given configuration. 
	
	<p>A group's content is considered to be optional (that is, 
	the group's state is considered in its visibility) if any 
	intent in its list matches an intent of the context. The 
	intent list of the context is usually set from the intent 
	list of the document configuration. </p>

	@param pdocg The optional-content group object. 
	@param pdoccfg The optional-content configuration.
	@return <code>true</code> if the group is taken into consideration when 
	determining the visibility of content, <code>false</code> otherwise. 
	
	@see PDOCConfigGetIntent 
	@see PDOCContextGetIntent 
	@see PDOCGGetIntent 
	@see PDOCGUsedInOCContext 
	@since PI_PDMODEL_VERSION >= 0x00060000
*/
NPROC( ASBool, PDOCGUsedInOCConfig, (PDOCG pdocg, PDOCConfig pdoccfg) )

/**
	Enumerates the optional-content groups for the page, calling 
	the supplied procedure for each one. Enumeration continues 
	until all groups have been enumerated, or until <code>enumProc</code> 
	returns <code>false</code>. Each group is reported once, even if it is 
	referenced multiple times in the page. 
	@param pdPage The page whose groups are enumerated. 
	@param enumProc A user-supplied callback to call for each 
	group. Enumeration terminates if <code>proc</code> returns <code>false</code>. 
	@param clientData A pointer to user-supplied data to pass 
	to <code>enumProc</code> each time it is called.
	@see PDDocEnumOCGs 
	@see PDPageGetOCGs 
	@since PI_PDMODEL_VERSION >= 0x00060000
*/
NPROC( void, PDPageEnumOCGs, (PDPage pdPage, PDOCGEnumProc enumProc, void* clientData) )

/**
	Enumerates the optional-content groups for the document, 
	calling the supplied procedure for each one. Enumeration 
	continues until all groups have been enumerated, or until 
	<code>enumProc</code> returns <code>false</code>. Each group is reported once, even 
	if it is referenced multiple times in a page, or on multiple 
	pages. 
	@param pdDoc The document whose groups are enumerated. 
	
	@param enumProc A user-supplied callback to call for each 
	group. Enumeration terminates if <code>enumProc</code> returns <code>false</code>. 
	@param clientData A pointer to user-supplied data to pass 
	to <code>enumProc</code> each time it is called.
	@see PDDocGetOCGs 
	@see PDDocEnumOCConfigs 
	@see PDPageEnumOCGs 
  	@ingroup Enumerators
	@since PI_PDMODEL_VERSION >= 0x00060000
*/
NPROC( void, PDDocEnumOCGs, (PDDoc pdDoc, PDOCGEnumProc enumProc, void* clientData) )

/**
	Gets the optional-content groups for the document. 
	@param pdPage The page whose OCGs are obtained.
	@return A <code>NULL</code>-terminated array of PDOCG objects. The client is 
	responsible for freeing the array with ASfree(). 
	@see PDDocGetOCGs 
	@see PDPageEnumOCGs 
	@since PI_PDMODEL_VERSION >= 0x00060000
*/
NPROC( PDOCG*, PDPageGetOCGs, (PDPage pdPage) )

/**
	Creates a new optional-content membership dictionary (OCMD) object 
	in the given document for the given groups and visibility 
	policy. 

	<p>To add a group to an existing OCMD, get the current OCG 
	list, modify it, then create a new OCMD with the new list 
	of groups. </p>

	@param pdDoc The document in which the dictionary is used. 
	
	@param ocgs A <code>NULL</code>-terminated array of optional-content 
	groups (OCGs) to be members of the dictionary. 
	@param policy The visibility policy that determines the 
	visibility of content with respect to the <code>ON-OFF</code> state of 
	OCGs listed in the dictionary.
	@return The newly created dictionary object, or <code>NULL</code> if no groups 
	are supplied. 
	@see PDOCMDFindOrCreate 
	@see PDOCMDGetFromCosObj 
	@since PI_PDMODEL_VERSION >= 0x00060000
*/
NPROC( PDOCMD, PDOCMDCreate, (PDDoc pdDoc, PDOCG* ocgs, PDOCMDVisPolicy policy) )

/**
	Locates an existing optional-content membership dictionary 
	(OCMD) object that references the given groups, and that 
	uses the same visibility policy. If no such dictionary is 
	found, the method creates one. 

	<p>If only one group is supplied, the policy is kOCMDVisibility_AnyOn 
	or kOCMDVisibility_AllOn, and no matching dictionary is 
	found, the method creates an OCMD that directly contains 
	the group without the level of indirection normally introduced 
	by an OCMD. If the indirection is needed to add more groups 
	to the OCMD, use PDOCMDCreate(). </p>

	<p>To add a group to an existing OCMD, get the current OCG 
	list, modify it, then create a new OCMD with the new list 
	of groups. </p>

	@param pdDoc The document in which the dictionary is used. 
	
	@param ocgs A <code>NULL</code>-terminated array of optional-content 
	groups (OCGs) to be members of the dictionary. 
	@param policy The visibility policy that determines the 
	visibility of content with respect to the <code>ON-OFF</code> state of 
	OCGs listed in the dictionary.
	@return The newly created or existing dictionary object, or <code>NULL</code> 
	if no groups are supplied. 
	@see PDOCMDCreate 
	@see PDOCMDGetFromCosObj 
	@see PDAnnotGetOCMD 
	@see PDEElementGetOCMD 
	@since PI_PDMODEL_VERSION >= 0x00060000
*/
NPROC( PDOCMD, PDOCMDFindOrCreate, (PDDoc pdDoc, PDOCG* ocgs, PDOCMDVisPolicy policy) )

/**
	Gets the Cos object associated with the optional-content 
	membership dictionary (OCMD) object. 
	@param pdocmd The dictionary object.
	@return The Cos object. 
	@see PDOCMDGetFromCosObj 
	@since PI_PDMODEL_VERSION >= 0x00060000
*/
NPROC( CosObj, PDOCMDGetCosObj, (PDOCMD pdocmd) )

/**
	Gets the document that contains an optional-content membership 
	dictionary. 
	@param pdocmd The dictionary for which the document is 
	desired.
	@return The document object. 
	@see PDDocGetOCGs 
	@see PDOCGGetPDDoc 
	@since PI_PDMODEL_VERSION >= 0x00060000
*/
NPROC( PDDoc, PDOCMDGetPDDoc, (PDOCMD pdocmd) )

/**
	Gets an optional-content membership dictionary (OCMD) object 
	from the associated Cos object. 
	@param obj The Cos object.
	@return The dictionary object. 
	@see PDOCMDCreate 
	@see PDOCMDGetCosObj 
	@since PI_PDMODEL_VERSION >= 0x00060000
*/
NPROC( PDOCMD, PDOCMDGetFromCosObj, (CosObj obj) )

/**
	Gets the optional-content groups listed in a membership 
	dictionary. 
	@param pdocmd The membership dictionary whose OCGs are 
	obtained.
	@return A <code>NULL</code>-terminated array of the document's optional-content 
	groups. The client is responsible for freeing the array 
	using ASfree(). 
	@since PI_PDMODEL_VERSION >= 0x00060000
*/
NPROC( PDOCG*, PDOCMDGetOCGs, (PDOCMD pdocmd) )

/**
	Gets the optional-content membership dictionary's visibility 
	policy, which determines the visibility of content with 
	respect to the <code>ON-OFF</code> state of OCGs listed in the dictionary. 
	
	@param pdocmd The dictionary whose policy is obtained.
	@return The visibility policy. 
	@see PDOCMDIsCurrentlyVisible 
	@see PDOCMDsAreCurrentlyVisible 
	@since PI_PDMODEL_VERSION >= 0x00060000
*/
NPROC( PDOCMDVisPolicy, PDOCMDGetVisPolicy, (PDOCMD pdocmd) )

/**
	Associates an optional-content membership dictionary (OCMD) 
	object with the annotation, making it optionally visible 
	according to the OCMD's visibility policy. If the annotation 
	already has a dictionary, the method replaces it. 
	@param annot The annotation for which to set the dictionary. 
	
	@param pdocmd The new dictionary.
	@see PDAnnotGetOCMD 
	@see PDOCMDFindOrCreate 
	@see PDAnnotRemoveOCMD 
	@since PI_PDMODEL_VERSION >= 0x00060000
*/
NPROC( void, PDAnnotSetOCMD, (PDAnnot annot, PDOCMD pdocmd) )

/**
	Gets an optional-content membership dictionary (OCMD) object 
	associated with the annotation. 
	@param annot The annotation from which the dictionary 
	is obtained.
	@return The dictionary object, or <code>NULL</code> if the annotation does not 
	contain a dictionary. 
	@see PDAnnotSetOCMD 
	@see PDAnnotRemoveOCMD 
	@see PDEElementGetOCMD 
	@see PDOCMDFindOrCreate 
	@since PI_PDMODEL_VERSION >= 0x00060000
*/
NPROC( PDOCMD, PDAnnotGetOCMD, (PDAnnot annot) )

/**
	Dissociates any optional-content membership dictionary (OCMD) 
	object from the annotation. 
	@param annot The annotation for which to remove the dictionary.
	@see PDAnnotGetOCMD 
	@see PDAnnotSetOCMD 
	@see PDOCMDFindOrCreate 
	@since PI_PDMODEL_VERSION >= 0x00060000
*/
NPROC( void, PDAnnotRemoveOCMD, (PDAnnot annot) )

/**
	Based on the optional-content groups listed in the dictionary, 
	the current <code>ON-OFF</code> state of those groups within the specified 
	context, and the dictionary's visibility policy, test whether 
	the content tagged with this dictionary would be visible. 
	
	<p>It ignores the context's current PDOCDrawEnumType and NonOCDrawing 
	settings. </p>
	@param pdocmd The dictionary. 
	@param ocContext The context in which the visibility of 
	content is tested. 
	@return <code>true</code> if content tagged with this dictionary is visible 
	in the given context, <code>false</code> if it is hidden. 
	@see PDOCMDGetVisPolicy 
	@see PDOCMDsAreCurrentlyVisible 
	@since PI_PDMODEL_VERSION >= 0x00060000
*/
NPROC( ASBool, PDOCMDIsCurrentlyVisible, (PDOCMD pdocmd,  PDOCContext ocContext) )

/**
	Tests a set of optional-content membership dictionaries 
	to determine whether contents tagged with any of them is 
	visible in a given optional-content context. The method 
	calls PDOCMDIsCurrentlyVisible() on each of the dictionaries. 
	If content is visible in the given context in any of the 
	dictionaries, this method returns <code>true</code>. 
	@param pdocmds A <code>NULL</code>-terminated array of dictionaries 
	to test. 
	@param ocContext The context in which visibility is tested.
	@return <code>true</code> if content using any of the dictionaries is visible 
	in the given context, <code>false</code> if it is hidden for all of the 
	dictionaries. 
	@see PDOCMDGetVisPolicy 
	@see PDOCMDIsCurrentlyVisible 
	@since PI_PDMODEL_VERSION >= 0x00060000
*/
NPROC( ASBool, PDOCMDsAreCurrentlyVisible, (PDOCMD* pdocmds, PDOCContext ocContext) )

/**
	Tests whether an annotation with an OC entry is visible 
	in a given optional-content context, considering the current 
	<code>ON-OFF</code> states of the optional-content groups in the optional-content 
	dictionary (OCMD) and the dictionary's visibility policy. 
	
	@param annot The annotation to test. 
	@param ocContext The optional-content context in which the visibility 
	is tested.
	@return <code>true</code> if the annotation is visible in the given context 
	or if the annotation has no OC entry, <code>false</code> if it is hidden. 
	
	@see PDAnnotGetOCMD 
	@since PI_PDMODEL_VERSION >= 0x00060000
*/
NPROC( ASBool, PDAnnotIsCurrentlyVisible, (PDAnnot annot,  PDOCContext ocContext) )

/**
	Makes content that uses any of a set of optional-content 
	membership dictionaries visible in a given optional-content 
	context. The method manipulates the states of optional-content 
	groups in the dictionaries so that any content controlled 
	by any of the dictionaries will be visible in the given 
	context. There can be more than one combination of states 
	that satisfies the request. The particular combination of 
	states is not guaranteed from one call to the next. 

	<p>The method returns <code>false</code> if it is not possible to make the 
	content visible (for example, if there are nested dictionaries 
	where one specifies <code>"show if the group state is ON"</code> and 
	the other specifies <code>"show if the group state is OFF"</code>). In 
	such a case, visibility is always off, so no state setting 
	can make the content visible. </p>

	<p>This method ignores the context's draw type. </p>

	@param ocmds A <code>NULL</code>-terminated array of dictionaries to 
	act upon. 
	@param ocContext The context in which the contents are 
	made visible.
	@return <code>true</code> if successful or if the OCMD list is empty, <code>false</code> otherwise. 
	
	@see PDOCMDGetVisPolicy 
	@see PDOCMDIsCurrentlyVisible 
	@since PI_PDMODEL_VERSION >= 0x00060000
*/
NPROC( ASBool, PDOCMDsMakeContentVisible, (PDOCMD* ocmds, PDOCContext ocContext) )

/**
	Creates a context object that represents an optional-content 
	state of the document, initializing it in the same way as 
	PDOCContextInit(). 
	@param policy The initialization policy for the new context. 
	This value determines whether optional-content groups (OCGs) 
	are initially <code>ON</code> or <code>OFF</code>. 
	@param otherCtx Another context from which to take initial 
	OCG states when the policy is kPDOCInit_FromOtherContext. 
	It is ignored for other policies. 
	@param pdOCCfg A configuration from which to take initial 
	OCG states when the policy is kPDOCInit_FromConfig. It is ignored 
	for other policies. 
	@param pdDoc The document for which to create a context.
	@return The new PDOCContext object. The client is responsible for 
	freeing the context using PDOCContextFree(). 
	@see PDOCContextInit 
	@see PDOCContextFree 
	@since PI_PDMODEL_VERSION >= 0x00060000
*/
NPROC( PDOCContext, PDOCContextNew, (PDOCContextInitPolicy policy, PDOCContext otherCtx, PDOCConfig pdOCCfg, PDDoc pdDoc) )

/**
	Destroys an optional-content context object and frees the 
	associated memory as needed. 
	@param ocContext The context object to free.
	@see PDOCContextInit 
	@see PDOCContextNew 
	@since PI_PDMODEL_VERSION >= 0x00060000
*/
NPROC( void, PDOCContextFree, (PDOCContext ocContext) )

/**
	Gets the built-in default optional-content context for the 
	document. This context is used by all content drawing and 
	enumeration calls that do not take an optional-content context 
	parameter, or for which no context is specified. 
	@param pdDoc The document whose context is obtained.
	@return The document's current optional-content context. 
	@see PDDocGetOCConfig 
	@see PDDocGetOCGs 
	@see PDOCContextGetPDDoc 
	@since PI_PDMODEL_VERSION >= 0x00060000
*/
NPROC( PDOCContext, PDDocGetOCContext, (PDDoc pdDoc) )

/**
	Gets the document that contains an optional-content context. 
	
	@param ocContext The context for which a document is desired.
	@return The document object. 
	@see PDDocGetOCContext 
	@since PI_PDMODEL_VERSION >= 0x00060000
*/
NPROC( PDDoc, PDOCContextGetPDDoc, (PDOCContext ocContext) )

/**
	Initializes the <code>ON-OFF</code> states of all optional-content groups 
	(OCGs) within an existing context. 
	@param ocContext The context to initialize. 
	@param policy The initialization policy for the context. 
	This value determines whether optional-content groups are 
	initially <code>ON</code> or <code>OFF</code>. 
	@param otherCtx Another context from which to take initial 
	OCG states when the policy is kPDOCInit_FromOtherContext. 
	It is ignored for other policies. 
	@param pdOCCfg A configuration from which to take initial 
	OCG states when the policy is kPDOCInit_FromConfig. It is ignored 
	for other policies. 
	@see PDOCContextNew 
	@see PDOCContextFree 
	@since PI_PDMODEL_VERSION >= 0x00060000
*/
NPROC( void, PDOCContextInit, (PDOCContext ocContext, PDOCContextInitPolicy policy, PDOCContext otherCtx, PDOCConfig pdOCCfg) )

/**
	Creates a new context object to represent an optional-content 
	state of the document, using an existing context as a template. 
	
	<p>This is the same as the following call: </p>

	<p><code>PDOCCOntextNew(kOCCInit_FromOtherContext, ocContext, <code>NULL</code>, PDOCContextGetPDDoc(ocContext)); </code></p>

	@param ocContext The context to copy.
	@return The new PDOCContext object. The client is responsible for 
	freeing the context using PDOCContextFree(). 
	@see PDDocGetOCContext 
	@see PDOCContextInit 
	@see PDOCContextNew 
	@see PDOCContextFree 
	@since PI_PDMODEL_VERSION >= 0x00060000
*/
NPROC( PDOCContext, PDOCContextMakeCopy, (PDOCContext ocContext) )

/**
	Creates a context object that represents an optional-content 
	state of the document, with the PDOCDrawEnumType property 
	set to kPDOC_NoOC, so that no content marked as optional 
	content is drawn, regardless of the visibility according 
	to the OCGs and OCMDs. 

	<p>Content that is not marked as optional content may still 
	be drawn, depending on the NonOCDrawing property. </p>

	@param pdDoc The document for which to create a context.
	@return The new PDOCContext object. The client is responsible for 
	freeing the context using PDOCContextFree(). 
	@see PDOCContextInit 
	@see PDOCContextNew 
	@see PDOCContextNewWithInitialState 
	@see PDOCContextFree 
	@see PDOCContextGetNonOCDrawing 
	@see PDOCContextGetOCDrawEnumType 
	@see PDOCContextSetNonOCDrawing 
	@see PDOCContextSetOCDrawEnumType 
	@since PI_PDMODEL_VERSION >= 0x00060000
*/
NPROC( PDOCContext, PDOCContextNewWithOCDisabled, (PDDoc pdDoc) )

/**
	Creates a context object that represents an optional-content 
	state of the document, using the current state as the initial 
	state for each group (OCG), as determined by the document's 
	optional-content configuration (returned by <code>PDDocGetOCConfig(pdDoc)</code>). 
	
	@param pdDoc The document for which to create a context.
	@return The new PDOCContext object. The client is responsible for 
	freeing the context using PDOCContextFree(). 
	@see PDDocGetOCConfig 
	@see PDOCContextInit 
	@see PDOCContextNew 
	@see PDOCContextNewWithOCDisabled 
	@see PDOCContextFree 
	@since PI_PDMODEL_VERSION >= 0x00060000
*/
NPROC( PDOCContext, PDOCContextNewWithInitialState, (PDDoc pdDoc) )

/**
	Gets the <code>ON-OFF</code> states for the given optional-content groups 
	(OCGs) in the given optional-content context. It returns the 
	states in the <code>states</code> array, which must be large enough to 
	hold as many ASBool values as there are OCGs. 
	@param ocContext The context for which the OCG states 
	are desired. 
	@param pdocgs A <code>NULL</code>-terminated array of optional-content 
	groups whose states are obtained. 
	@param states (Filled by the method) An array of OCG states 
	corresponding to the array of OCGs, <code>true</code> for ON and <code>false</code> 
	for <code>OFF</code>. The array must be large enough to hold as many 
	states as there are non-<code>NULL</code> OCGs.
	@see PDOCContextSetOCGStates 
	@since PI_PDMODEL_VERSION >= 0x00060000
*/
NPROC( void, PDOCContextGetOCGStates, (PDOCContext ocContext, PDOCG *pdocgs, ASBool *states) )

/**
	Sets the <code>ON-OFF</code> states for the given optional-content groups 
	(OCGs) in the given optional-content context. The <code>newStates</code> 
	array must be large enough to hold as many ASBool values 
	as there are OCGs. 
	@param ocContext The context for which the OCG states 
	are set. 
	@param pdocgs A <code>NULL</code>-terminated array of optional-content 
	groups. 
	@param newStates An array of new OCG states corresponding 
	to the array of OCGs, <code>true</code> for ON and <code>false</code> for <code>OFF</code>. The 
	array must contain as many states as there are non-<code>NULL</code> 
	OCGs in the <code>pdocgs</code> array.
	@see PDOCContextGetOCGStates 
	@since PI_PDMODEL_VERSION >= 0x00060000
*/
NPROC( void, PDOCContextSetOCGStates, (PDOCContext ocContext, PDOCG *pdocgs, ASBool *newStates) )

/**
	Creates a new context object that represents an optional-content 
	state of the document, using an existing context as a template, 
	but applying an automatic state change for the specified 
	event. An automatic state change toggles all groups' <code>ON-OFF</code> 
	states when the triggering event occurs. 

	<p>A configuration's AS array defines how usage entries are 
	used to automatically manipulate the OCG states. It associates 
	an event (<code>View</code>, <code>Print</code>, or <code>Export</code>) with a list of OCGs and 
	a category, or list of usage keys identifying OCG usage 
	dictionary entries. See the <i>PDF Reference</i>, and the <b>OCTextAutoStateSnip</b> 
	example in the Snippet Runner. </p>

	@param inCtx The context to copy. 
	@param cfg The configuration in which the automatic state 
	change applies. 
	@param event The event for which state will automatically 
	change. Events are <code>View</code>, <code>Export</code>, and <code>Print</code>.
	@return The new PDOCContext object. The client is responsible for 
	freeing the context using PDOCContextFree(). 
	@see PDOCContextApplyAutoStateChanges 
	@see PDOCContextFindAutoStateChanges 
	@see PDOCContextClearAllUserOverrides 
	@see PDOCGGetUserOverride 
	@see PDOCGSetUserOverride 
	@since PI_PDMODEL_VERSION >= 0x00060000
*/
NPROC( PDOCContext, PDOCContextMakeCopyWithAutoStateChanges, (PDOCContext inCtx, PDOCConfig cfg, ASAtom event) )

/**
	Finds optional-content groups whose <code>ON-OFF</code> states should 
	be toggled in the context, based on usage application directives 
	contained in the configuration's AS array. 

	<p>The AS array defines how usage entries are used to automatically 
	manipulate the OCG states. It associates an event (<code>View</code>, 
	<code>Print</code>, or <code>Export</code>) with a list of OCGs and a category, or 
	list of usage keys identifying OCG usage dictionary entries. 
	See the <i>PDF Reference</i>, and the <b>OCTextAutoStateSnip</b> example 
	in the Snippet Runner. </p>
	@param ctx The context for which the visibility state 
	should be changed. 
	@param cfg The configuration whose usage directives are 
	used. 
	@param event The event for which an <code>ON-OFF</code> state is automatically 
	changed. Events are <code>View</code>, <code>Export</code>, and <code>Print</code>. 
	@return A <code>NULL</code>-terminated array of optional-content group objects. 
	The client is responsible for freeing it using ASfree(). 
	@see PDOCContextApplyAutoStateChanges 
	@see PDOCContextMakeCopyWithAutoStateChanges 
	@see PDOCContextClearAllUserOverrides 
	@see PDOCGGetUserOverride 
	@see PDOCGSetUserOverride 
	@since PI_PDMODEL_VERSION >= 0x00060000
*/
NPROC( PDOCG*, PDOCContextFindAutoStateChanges, (PDOCContext ctx, PDOCConfig cfg, ASAtom event) )

/**
	Calls PDOCContextFindAutoStateChanges() to find optional-content 
	groups whose <code>ON-OFF</code> states should be toggled, based on usage 
	application directives contained in the configuration's 
	AS array, and applies the changes within the given context. 
	
	<p>The AS array defines how usage entries are used to automatically 
	manipulate the OCG states. It associates an event (<code>View</code>, 
	<code>Print</code>, or <code>Export</code>) with a list of OCGs and a category, or 
	list of usage keys identifying OCG usage dictionary entries. 
	See the <i>PDF Reference</i>, and the <b>OCTextAutoStateSnip</b> example 
	in the Snippet Runner.</p>

	@param ctx The context for which the visibility state 
	is changed. 
	@param cfg The configuration whose usage directives are 
	used. 
	@param event The event for which an <code>ON-OFF</code> state is automatically 
	changed. Events are <code>View</code>, <code>Export</code>, and <code>Print</code>.
	@return <code>true</code> if successful, <code>false</code> otherwise. 
	@see PDOCContextFindAutoStateChanges 
	@see PDOCContextMakeCopyWithAutoStateChanges 
	@see PDOCContextClearAllUserOverrides 
	@see PDOCGGetUserOverride 
	@see PDOCGSetUserOverride 
	@since PI_PDMODEL_VERSION >= 0x00060000
*/
NPROC( ASBool, PDOCContextApplyAutoStateChanges, (PDOCContext ctx, PDOCConfig cfg, ASAtom event) )

/**
	Sets the drawing and enumeration type for an optional-content 
	context. This type, together with the visibility determined 
	by the OCG and OCMD states, controls whether content that 
	is marked as optional content is drawn or enumerated. 

	<p>Together, this value and the NonOCDrawing value of the context 
	determine how both optional and non-optional content on 
	a page is drawn or enumerated. See PDOCDrawEnumType(). </p>

	@param ocContext The context for which the drawing and 
	enumeration type is desired. 
	@param dt The new drawing and enumeration type.
	@see PDOCContextGetOCDrawEnumType 
	@see PDOCContextSetNonOCDrawing 
	@since PI_PDMODEL_VERSION >= 0x00060000
*/
NPROC( void, PDOCContextSetOCDrawEnumType, (PDOCContext ocContext, PDOCDrawEnumType dt) )

/**
	Gets the drawing and enumeration type for an optional-content 
	context. This type, together with the visibility determined 
	by the OCG and Optional Content Membership Dictionary (OCMD) states, controls whether content that 
	is marked as optional content is drawn or enumerated. 

	<p>Together, this value and the NonOCDrawing value of the context 
	determine how both optional and non-optional content on 
	a page is drawn or enumerated. See PDOCDrawEnumType(). </p>

	@param ocContext The context for which the drawing and 
	enumeration type is desired.
	@return The drawing and enumeration type value. 
	@see PDOCContextGetNonOCDrawing 
	@see PDOCContextSetOCDrawEnumType 
	@since PI_PDMODEL_VERSION >= 0x00060000
*/
NPROC( PDOCDrawEnumType, PDOCContextGetOCDrawEnumType, (PDOCContext ocContext) )

/**
	Sets the Intent entry in an optional-content context's Cos 
	dictionary. An intent is an ASAtom value broadly describing 
	the intended use, either <code>View</code> or <code>Design</code>. 

	<p>A group's content is considered to be optional (that is, 
	the group's state is considered in its visibility) if any 
	intent in its list matches an intent of the context. The 
	intent list of the context is usually set from the intent 
	list of the document configuration. </p>

	<p>It raises an exception if the context is busy. </p>

	@param ocContext The context for which to set the intent. 
	
	@param intent The new Intent entry value, an array of 
	atoms terminated with ASAtomNull.
	@see PDOCContextGetIntent 
	@see PDOCConfigSetIntent 
	@see PDOCGSetIntent 
	@see PDOCGUsedInOCConfig 
	@see PDOCGUsedInOCContext 
	@since PI_PDMODEL_VERSION >= 0x00060000
*/
NPROC( void, PDOCContextSetIntent, (PDOCContext ocContext, ASAtom* intent) )

/**
	Gets the intent list for an optional-content context. An 
	intent is an ASAtom value broadly describing the intended 
	use, either <code>View</code> or <code>Design</code>. 

	<p>A group's content is considered to be optional (that is, 
	the group's state is considered in its visibility) if any 
	intent in its list matches an intent of the context. The 
	intent list of the context is usually set from the intent 
	list of the document configuration. </p>

	@param ocContext The context for which an intent is desired.
	@return An array containing intent entries (ASAtom objects) terminated 
	by ASAtomNull. The client is responsible for freeing it 
	using ASfree(). 
	@see PDOCContextSetIntent 
	@see PDOCConfigGetIntent 
	@see PDOCGGetIntent 
	@see PDOCGUsedInOCConfig 
	@see PDOCGUsedInOCContext 
	@since PI_PDMODEL_VERSION >= 0x00060000
*/
NPROC( ASAtom*, PDOCContextGetIntent, (PDOCContext ocContext) )

/**
	Sets the non-OC status for an optional-content context. 
	Content that is not marked as optional content is drawn 
	when NonOCDrawing is <code>true</code>, and not drawn when NonOCDrawing 
	is <code>false</code>. 

	<p>Together, this value and the PDOCDrawEnumType value of the 
	context determine how both optional and non-optional content 
	on a page is drawn or enumerated. See PDOCDrawEnumType(). </p>
	
	@param ocContext The context for which to set the non-OC 
	drawing status. 
	@param drawNonOC The new value for the non-OC drawing 
	status, <code>true</code> or <code>false</code>.
	@see PDOCContextGetNonOCDrawing 
	@see PDOCContextGetOCDrawEnumType 
	@since PI_PDMODEL_VERSION >= 0x00060000
*/
NPROC( void, PDOCContextSetNonOCDrawing, (PDOCContext ocContext, ASBool drawNonOC) )

/**
	Gets the non-OC drawing status for an optional-content context. 
	Content that is not marked as optional content is drawn 
	when NonOCDrawing is <code>true</code>, and not drawn when NonOCDrawing 
	is <code>false</code>. 

	<p>Together, this value and the PDOCDrawEnumType value of the 
	context determine how both optional and non-optional content 
	on a page is drawn or enumerated. See PDOCDrawEnumType(). </p>
	
	@param ocContext The context for which the non-OC drawing 
	status is desired.
	@return <code>true</code> if the context's NonOCDrawing is <code>true</code>, <code>false</code> 
	otherwise. 
	@see PDOCContextSetNonOCDrawing 
	@see PDOCContextGetOCDrawEnumType 
	@see PDOCContextNewWithOCDisabled 
	@since PI_PDMODEL_VERSION >= 0x00060000
*/
NPROC( ASBool, PDOCContextGetNonOCDrawing, (PDOCContext ocContext) )

/**
	Clears the Optional Content Membership Dictionary (OCMD) stack for an optional-content context, and 
	resets the current visibility for the context based on the 
	context's non-OC drawing setting (see PDOCContextSetNonOCDrawing()). 
	Call this method at the start of an enumeration or drawing 
	operation that uses a given context. 

	<p>The OCMD stack contains optional-content membership dictionary 
	objects. The OCMD stack methods and the various methods 
	that test visibility (such as PDAnnotIsCurrentlyVisible()) 
	work together as content is being enumerated or drawn to 
	determine whether particular graphical elements are visible 
	or not. Visibility is based on the context's collection 
	of <code>ON-OFF</code> states for optional-content groups, the context's 
	current settings for NonOCDrawing and PDOCDrawEnumType, 
	and the state of the OCMD stack. </p>

	<p>Any custom drawing or enumerating code that needs to keep 
	track of visibility of content must make a private copy 
	of the PDOCContext if that context could be accessed by 
	some other client, in order to avoid conflicting state changes. 
	In particular, you must copy the document's default context 
	(as returned by PDDocGetOCContext()). To enforce this, this 
	reset method does nothing when given a document's default 
	context. Similarly, the push and pop stack operations raise 
	an error for the default context. </p>

	<p>If you are using the PD-level draw and enumeration methods, 
	you do not need to copy the context or explicitly call the 
	OCMD stack methods, as the PD-level methods do this internally. </p>
	
	<p>Clients of PDFEdit and other libraries that enumerate contents 
	need to use these three methods when traversing the PDEContent 
	structure. When entering a new PDEContent, call PDOCContextPushOCMD() 
	(passing an OCMD object or <code>NULL</code>). Upon finishing the traversal, 
	call PDOCContextPopOCMD(). </p>

	@param pdOCContext The context for which to reset the OCMD 
	stack.
	@see PDOCContextPopOCMD 
	@see PDOCContextPushOCMD 
	@see PDOCContextContentIsVisible 
	@see PDOCContextXObjectIsVisible 
	@since PI_PDMODEL_VERSION >= 0x00060000
*/
NPROC( void, PDOCContextResetOCMDStack, (PDOCContext pdOCContext) )

/**
	Pushes a new optional-content membership dictionary (OCMD) 
	onto the stack for an optional-content context. 

	<p>The stack is used to track nesting of optional-content states 
	as contents are enumerated or drawn. Call this method when 
	entering the BDC for optional content or beginning to process 
	a form or annotation that has a OC entry. Call PDOCContextPopOCMD() 
	when encountering an EMC, or finishing the processing of 
	a form or annotation appearance. </p>

	<p>To make it easier to track nested content that is not for 
	optional content, pass <code>NULL</code> for pdOCMD when encountering 
	BMC, patterns, and charprocs. Also pass <code>NULL</code> for a BDC with 
	no optional content or for forms or annotations that do 
	not have an OC entry. When finished processing any of these 
	objects, you can call PDOCContextPopOCMD() without worrying 
	about whether the content was optional. </p>

	@param pdOCContext The context containing the OCMD stack. 
	
	@param pdOCMD The OCMD to push onto the stack.
	@see PDOCContextPopOCMD 
	@see PDOCContextResetOCMDStack 
	@see PDOCContextContentIsVisible 
	@see PDOCContextXObjectIsVisible 
	@since PI_PDMODEL_VERSION >= 0x00060000
*/
NPROC( void, PDOCContextPushOCMD, (PDOCContext pdOCContext, PDOCMD pdOCMD) )

/**
	Pops the optional-content membership dictionary (OCMD) stack 
	for an optional-content context. The stack is used to track 
	nesting of optional-content states as contents are enumerated 
	or drawn:
	
	<ul>
	<li>Call the PDOCContextPushOCMD() method when entering BDC 
	for optional content or beginning to process a form or annotation 
	that has a OC entry. </li>
	<li>Call this method to pop the stack when encountering EMC, 
	or finishing the processing of a form or annotation appearance. </li>
	</ul>
	
	<p>To track nested content that is not for optional content, 
	pass in <code>NULL</code> for pdOCMD when pushing the OCMD stack for 
	BMC, patterns, and charprocs, for BDC with no optional content, 
	or for forms or annotations that do not have an OC entry. 
	When finished processing any of these objects, you can call 
	PDOCContextPopOCMD() without worrying about whether the content 
	was optional. </p>

	@param ocContext The context for which to pop the OCMD 
	stack.
	@see PDOCContextPushOCMD 
	@see PDOCContextResetOCMDStack 
	@see PDOCContextContentIsVisible 
	@see PDOCContextXObjectIsVisible 
	@since PI_PDMODEL_VERSION >= 0x00060000
*/
NPROC( void, PDOCContextPopOCMD, (PDOCContext ocContext) )

/**
	Tests whether content is visible in the optional-content 
	context. The method considers the context's current OCMD 
	stack, the group <code>ON-OFF</code> states, the non-OC drawing status, 
	the drawing and enumeration type, and the intent. 

	<p>Use this method in conjunction with the OCMD stack methods. </p>
	
	@param ocContext The context for which the visibility 
	state is desired.
	@return <code>true</code> if the content is visible, <code>false</code> otherwise. 
	
	@see PDOCContextPopOCMD 
	@see PDOCContextPushOCMD 
	@see PDOCContextResetOCMDStack 
	@see PDOCContextXObjectIsVisible 
	@since PI_PDMODEL_VERSION >= 0x00060000
*/
NPROC( ASBool, PDOCContextContentIsVisible, (PDOCContext ocContext) )

/**
	Tests whether an XObject form or image contained in <code>obj</code> 
	is visible in the optional-content context. The method considers 
	the context's current OCMD stack, optional-content group 
	<code>ON-OFF</code> states, the non-OC drawing status, the drawing and 
	enumeration type, the intent, and the specific OCG. 

	<p>Use this method in conjunction with the OCMD stack methods. </p>
	
	@param pdOCContext The context for which to test visibility. 
	
	@param obj The external object.
	@return <code>true</code> if the external object is visible, <code>false</code> otherwise. 
	
	@see PDOCContextContentIsVisible 
	@see PDOCContextPopOCMD 
	@see PDOCContextPushOCMD 
	@see PDOCContextResetOCMDStack 
	@since PI_PDMODEL_VERSION >= 0x00060000
*/
NPROC( ASBool, PDOCContextXObjectIsVisible, (PDOCContext pdOCContext, CosObj obj) )

/**
	Creates a new optional-content configuration object. 
	@param pdDoc The document in which the configuration is 
	used.
	@return The newly created configuration object. 
	@see PDOCConfigDestroy 
	@since PI_PDMODEL_VERSION >= 0x00060000
*/
NPROC( PDOCConfig, PDOCConfigCreate, (PDDoc pdDoc) )

/**
	Removes an optional-content configuration object and destroys 
	the Cos objects associated with it. If you pass this method 
	the document's default configuration object (as returned 
	by PDDocGetOCConfig()), nothing happens. 
	@param pdOCCfg The configuration to destroy.
	@see PDOCConfigCreate 
	@since PI_PDMODEL_VERSION >= 0x00060000
*/
NPROC( void, PDOCConfigDestroy, (PDOCConfig pdOCCfg) )

/**
	Gets the built-in default optional-content configuration 
	for the document from the OCProperties D entry. 
	@param pdDoc The document whose configuration is obtained.
	@return The document's current optional-content configuration. 
	@see PDDocGetOCContext 
	@see PDDocGetOCGs 
	@see PDDocEnumOCGs 
	@see PDDocEnumOCConfigs 
	@see PDOCConfigGetPDDoc 
	@since PI_PDMODEL_VERSION >= 0x00060000
*/
NPROC( PDOCConfig, PDDocGetOCConfig, (PDDoc pdDoc) )

/**
	Gets the document to which the optional-content configuration 
	belongs. 
	@param pdOCCfg The configuration for which a document 
	is desired.
	@return The document object. 
	@see PDDocGetOCConfig 
	@since PI_PDMODEL_VERSION >= 0x00060000
*/
NPROC( PDDoc, PDOCConfigGetPDDoc, (PDOCConfig pdOCCfg) )

/**
	Gets the Cos object associated with the optional-content 
	configuration. 
	@param pdOCCfg The configuration for which a CosObj representation 
	is desired.
	@return A CosObj representation of pdOCCfg. 
	@since PI_PDMODEL_VERSION >= 0x00060000
*/
NPROC( CosObj, PDOCConfigGetCosObj, (PDOCConfig pdOCCfg) )

/**
	Sets the user interface display order of optional-content groups (OCGs) 
	in an optional-content configuration. This is the order 
	in which the group names are displayed in the Layers panel 
	of Acrobat 6.0 and later. 
	@param pdOCCfg The configuration for which a OCG is desired. 
	
	@param orderArray The Cos object containing the OCG order 
	array. For more information, see the <i>PDF Reference</i>. Pass 
	NULL to remove any existing order entry.
	@see PDOCConfigGetOCGOrder 
	@since PI_PDMODEL_VERSION >= 0x00060000
*/
NPROC( void, PDOCConfigSetOCGOrder, (PDOCConfig pdOCCfg, CosObj orderArray) )

/**
	Gets the user interface display order of optional-content groups (OCGs) 
	in an optional-content configuration. This is the order 
	in which the group names are displayed in the Layers panel 
	of Acrobat 6.0 and later. 
	@param pdOCCfg The configuration for which an OCG display 
	order is desired. 
	@param orderObj (Filled by the method) A pointer to 
	the Cos object containing the OCG order array. For more 
	information, see the <i>PDF Reference</i>.
	@return <code>true</code> if the order belongs directly to this configuration, 
	<code>false</code> if it is inherited from the document's default configuration. 
	
	@see PDOCConfigSetOCGOrder 
	@since PI_PDMODEL_VERSION >= 0x00060000
*/
NPROC( ASBool, PDOCConfigGetOCGOrder, (PDOCConfig pdOCCfg, CosObj *orderObj) )

/**
	Configures a mutually exclusive set of optional-content 
	groups in an optional-content configuration. The set behaves 
	like a radio button group, where only one OCG from the set 
	can be <code>ON</code> at a time. A client must enforce this in the user interface-level 
	code, not the PD-level code. 
	@param pdOCCfg The optional-content configuration. 
	@param ocgs A <code>NULL</code>-terminated array of optional-content 
	groups to be included in the group.
	@see PDOCConfigGetAllRadioButtonGroups 
	@see PDOCConfigGetRadioButtonGroupForOCG 
	@since PI_PDMODEL_VERSION >= 0x00060000
*/
NPROC( void, PDOCConfigMakeRadioButtonGroup, (PDOCConfig pdOCCfg, PDOCG* ocgs) )

/**
	Returns an array of optional-content groups in the configuration 
	that contains the specified group, and is configured to 
	behave like a radio button group, where only one member 
	of the set can be <code>ON</code> at one time. 
	@param pdOCCfg The optional-content configuration. 
	@param ocg The optional-content group for which to obtain 
	the radio-button group.
	@return A <code>NULL</code>-terminated array of PDOCG objects, or <code>NULL</code> if the 
	specified group does not belong to any radio button group. 
	The client is responsible for freeing the array using ASfree(). 
	
	@see PDOCConfigGetAllRadioButtonGroups 
	@see PDOCConfigMakeRadioButtonGroup 
	@since PI_PDMODEL_VERSION >= 0x00060000
*/
NPROC( PDOCG*, PDOCConfigGetRadioButtonGroupForOCG, (PDOCConfig pdOCCfg, PDOCG ocg) )

/**
	Returns an array of pointers to sets of optional-content 
	groups in the configuration that are configured to be mutually 
	exclusive. A set behaves like a radio button group, where 
	only one member can be <code>ON</code> at one time. 
	@param pdOCCfg The configuration.
	@return A <code>NULL</code>-terminated array of pointers to <code>NULL</code>-terminated arrays 
	of optional-content groups (OCGs). The client is responsible 
	for freeing all arrays using ASfree(). 
	@see PDOCConfigGetRadioButtonGroupForOCG 
	@see PDOCConfigMakeRadioButtonGroup 
	@since PI_PDMODEL_VERSION >= 0x00060000
*/
NPROC( PDOCG**, PDOCConfigGetAllRadioButtonGroups, (PDOCConfig pdOCCfg) )

/**
	Sets the initial <code>ON-OFF</code> states of optional-content groups 
	to be saved in an optional-content configuration. 
	@param pdOCCfg The configuration for which to set the 
	initial state. 
	@param bs An existing PDOCConfigBaseState structure containing 
	the initialization information. 
	@param onOCGs A <code>NULL</code>-terminated array of optional-content 
	groups (OCGs) that have an initial state of <code>ON</code> when that 
	is not the base state, or <code>NULL</code>. 
	@param offOCGs A <code>NULL</code>-terminated array of OCGs that have 
	an initial state of <code>OFF</code> when that is not the base state, 
	or <code>NULL</code>.
	@see PDOCConfigGetInitState 
	@see PDOCGSetInitialState 
	@since PI_PDMODEL_VERSION >= 0x00060000
*/
NPROC( void, PDOCConfigSetInitState, (PDOCConfig pdOCCfg, PDOCConfigBaseState bs, PDOCG* onOCGs, PDOCG* offOCGs) )

/**
	Gets the initial <code>ON-OFF</code> states of optional-content groups 
	in an optional-content configuration. 

	<p>The client is responsible for freeing storage for the arrays using ASfree(). </p>

	@param pdOCCfg The configuration for which the initial 
	state is desired. 
	@param bs (Filled by the method) An existing PDOCConfigBaseState 
	structure in which to store the initialization information. 
	
	@param onOCGs (Filled by the method) A <code>NULL</code>-terminated 
	array of OCGs that have an initial state of <code>ON</code>, or <code>NULL</code> 
	if there are no such groups. 
	@param offOCGs (Filled by the method) A <code>NULL</code>-terminated 
	array of OCGs that have an initial state of <code>OFF</code>, or <code>NULL</code> 
	if there are no such groups.
	@see PDOCConfigSetInitState 
	@see PDOCGGetInitialState 
	@since PI_PDMODEL_VERSION >= 0x00060000
*/
NPROC( void, PDOCConfigGetInitState, (PDOCConfig pdOCCfg, PDOCConfigBaseState *bs, PDOCG** onOCGs, PDOCG** offOCGs) )

/**
	Sets the name of an optional-content configuration. It stores 
	the specified string as the Name entry in the configuration's 
	Cos dictionary. 
	@param pdOCCfg The configuration for which to set the 
	name. 
	@param name The new name string.
	@see PDOCConfigGetName 
	@see PDOCConfigSetCreator 
	@since PI_PDMODEL_VERSION >= 0x00060000
*/
NPROC( void, PDOCConfigSetName, (PDOCConfig pdOCCfg, ASConstText name) )

/**
	Gets the name of an optional-content configuration. 
	@param pdOCCfg The configuration for which a name is desired.
	@return An ASText object containing the name string from Name entry 
	of the configuration's Cos dictionary, or <code>NULL</code> if there 
	is no Name entry. The client is responsible for freeing 
	the ASText object using ASTextDestroy(). 
	@see PDOCConfigSetName 
	@see PDOCConfigGetCreator 
	@since PI_PDMODEL_VERSION >= 0x00060000
*/
NPROC( ASText, PDOCConfigGetName, (PDOCConfig pdOCCfg) )

/**
	Sets the creator property of an optional-content configuration. 
	Stores the specified string as the Creator entry in the 
	configuration's Cos dictionary. 
	@param pdOCCfg The configuration for which to set a creator. 
	
	@param creator The new creator string.
	@see PDOCConfigGetCreator 
	@see PDOCConfigSetName 
	@since PI_PDMODEL_VERSION >= 0x00060000
*/
NPROC( void, PDOCConfigSetCreator, (PDOCConfig pdOCCfg, ASConstText creator) )

/**
	Gets the creator property for an optional-content configuration. 
	
	@param pdOCCfg The configuration for which a creator is 
	desired.
	@return An ASText object containing the creator string from the 
	Creator entry in the configuration's Cos dictionary, or 
	<code>NULL</code> if there is no such entry. The client is responsible 
	for freeing the ASText using ASTextDestroy(). 
	@see PDOCConfigSetCreator 
	@see PDOCConfigGetName 
	@since PI_PDMODEL_VERSION >= 0x00060000
*/
NPROC( ASText, PDOCConfigGetCreator, (PDOCConfig pdOCCfg) )

/**
	Sets the Intent entry in an optional-content configuration's 
	Cos dictionary. An intent is an ASAtom value broadly describing 
	the intended use, either <code>View</code> or <code>Design</code>. 

	<p>A group's content is considered to be optional (that is, 
	the group's state is considered in its visibility) if any 
	intent in its list matches an intent of the context. The 
	intent list of the context is usually set from the intent 
	list of the document configuration. </p>

	<p>If the configuration has no Intent entry, the default value 
	of <code>View</code> is used. In this case, optional content is disabled 
	for contexts initialized with this configuration. </p>

	@param pdOCCfg The configuration for which to set an intent. 
	
	@param intent The new Intent entry value, an array of 
	atoms terminated with ASAtomNull. To remove the Intent entry, 
	pass an array with only one element, ASAtom<code>NULL</code>.
	@see PDOCConfigGetIntent 
	@see PDOCContextSetIntent 
	@see PDOCGSetIntent 
	@see PDOCGUsedInOCConfig 
	@see PDOCGUsedInOCContext 
	@since PI_PDMODEL_VERSION >= 0x00060000
*/
NPROC( void, PDOCConfigSetIntent, (PDOCConfig pdOCCfg, ASAtom *intent) )

/**
	Gets the Intent entry for an optional-content configuration. 
	An intent is an ASAtom value broadly describing the intended 
	use, either <code>View</code> or <code>Design</code>. A group's content is considered 
	to be optional (that is, the group's state is considered 
	in its visibility) if any intent in its list matches an 
	intent of the context. The intent list of the context is 
	usually set from the intent list of the document configuration. 
	
	<p>The intent array contains entries (atoms) terminated by 
	ASAtomNull. </p>

	<p>If the configuration has no Intent entry, the default value 
	of <code>View</code> is used. In this case, optional content is disabled 
	for contexts initialized with this configuration. </p>

	@param pdOCCfg The configuration for which an intent list 
	is desired.
	@return The ASAtomNull-terminated intent array. The client is responsible 
	for freeing it using ASfree(). 
	@see PDOCConfigSetIntent 
	@see PDOCContextGetIntent 
	@see PDOCGGetIntent 
	@see PDOCGUsedInOCConfig 
	@see PDOCGUsedInOCContext 
	@since PI_PDMODEL_VERSION >= 0x00060000
*/
NPROC( ASAtom*, PDOCConfigGetIntent, (PDOCConfig pdOCCfg) )

/**
	Enumerates the optional-content configurations for the document, 
	calling the supplied procedure for each one. These include 
	the configuration for the D configuration dictionary and 
	those for all entries in the Configs array dictionary. 
	@param pdDoc The document whose configurations are enumerated. 
	
	@param enumProc A user-supplied callback to call for each 
	configuration. Enumeration terminates if <code>enumProc</code> returns <code>false</code>. 
	
	@param clientData A pointer to user-supplied data to pass 
	to <code>proc</code> each time it is called.
	@see PDDocGetOCConfig 
	@see PDDocEnumOCGs 
	@since PI_PDMODEL_VERSION >= 0x00060000
*/
NPROC( void, PDDocEnumOCConfigs, (PDDoc pdDoc, PDOCConfigEnumProc enumProc, void *clientData) )

/**
	Determines whether the optional content feature is associated 
	with the document. The document is considered to have optional 
	content if there is an OCProperties dictionary in the document's 
	catalog, and that dictionary has one or more entries in 
	the OCGs array. 
	@param pdDoc The document whose OC status is obtained.
	@return <code>true</code> if the document has optional content, <code>false</code> otherwise. 
	
	@see PDDocGetOCConfig 
	@see PDDocGetOCContext 
	@see PDDocGetOCGs 
	@since PI_PDMODEL_VERSION >= 0x00060000
*/
NPROC( ASBool, PDDocHasOC, (PDDoc pdDoc) )

/**
	Returns the number of optional-content groups associated 
	with a document, which is the number of unique entries in 
	the document's OCProperties OCGs array. 
	@param pdDoc The document whose groups are counted.
	@return The number of OCGs for the document. 
	@see PDDocHasOC 
	@see PDDocGetOCGs 
	@see PDDocReplaceOCG 
	@see PDDocEnumOCGs 
	@see PDDocGetOCConfig 
	@see PDDocGetOCContext 
	@since PI_PDMODEL_VERSION >= 0x00060000
*/
NPROC( ASUns32, PDDocGetNumOCGs, (PDDoc pdDoc) )

/**
	Gets the optional-content groups for the document. The order 
	of the groups is not guaranteed to be the creation order, 
	and is not the same as the display order (see PDOCConfigGetOCGOrder()). 
	
	@param pdDoc The document whose OCGs are obtained.
	@return A <code>NULL</code>-terminated array of PDOCG objects. The client is 
	responsible for freeing the array using ASfree(). 
	@see PDDocHasOC 
	@see PDDocReplaceOCG 
	@see PDDocEnumOCGs 
	@see PDDocGetNumOCGs 
	@see PDDocGetOCConfig 
	@see PDDocGetOCContext 
	@see PDOCGGetPDDoc 
	@see PDOCMDGetPDDoc 
	@since PI_PDMODEL_VERSION >= 0x00060000
*/
NPROC( PDOCG*, PDDocGetOCGs, (PDDoc pdDoc) )

/**
	In the document associated with a specified optional-content 
	group, replaces that group with another group. 
	@param replaceOCG The OCG to replace. 
	@param keepOCG The replacement OCG.
	@see PDDocHasOC 
	@see PDDocGetOCGs 
	@see PDDocEnumOCGs 
	@see PDDocGetNumOCGs 
	@see PDDocGetOCConfig 
	@see PDDocGetOCContext 
	@since PI_PDMODEL_VERSION >= 0x00060000
*/
NPROC( void, PDDocReplaceOCG, (PDOCG replaceOCG, PDOCG keepOCG) )

/**
	Marks the optional-content group as having had its state 
	set directly by client code in the specified context (as 
	opposed to automatically by the optional-content AutoState 
	mechanism). 

	<p>When a group is so marked, automatic state changes caused 
	by the <code>View</code> event are prevented. When a group's automatic 
	state change is caused by the <code>Export</code> or <code>Print</code> event, the 
	user-override setting for the group is ignored. </p>

	<p>A configuration's AS array defines how usage entries are 
	used to automatically manipulate the OCG states. It associates 
	an event (<code>View</code>, <code>Print</code>, or <code>Export</code>) with a list of OCGs and 
	a category, or list of usage keys identifying OCG usage 
	dictionary entries. See the <i>PDF Reference</i>, and the <b>OCTextAutoStateSnip</b> 
	example in the Snippet Runner. </p>

	@param ocg The optional-content group object. 
	@param ctx The context for which the group is marked. 
	
	@param overridden <code>true</code> to mark the group as having had 
	its state set manually, <code>false</code> to clear the mark.
	@see PDOCGGetUserOverride 
	@see PDOCContextClearAllUserOverrides 
	@see PDOCContextFindAutoStateChanges 
	@see PDOCContextApplyAutoStateChanges 
	@see PDOCContextMakeCopyWithAutoStateChanges 
	@since PI_PDMODEL_VERSION >= 0x00060000
*/
NPROC( void, PDOCGSetUserOverride, (PDOCG ocg, PDOCContext ctx, ASBool overridden) )

/**
	Tests whether the optional-content group is marked as having 
	had its state set directly by client code in the specified 
	context (as opposed to automatically by the optional-content 
	AutoState mechanism). 

	<p>When a group is so marked, automatic state changes caused 
	by the <code>View</code> event are prevented. When a group's automatic 
	state change is caused by the <code>Export</code> or <code>Print</code> event, the 
	user-override setting for the group is ignored. </p>

	<p>A configuration's AS array defines how usage entries are 
	used to automatically manipulate the OCG states. It associates 
	an event (<code>View</code>, <code>Print</code>, or <code>Export</code>) with a list of OCGs and 
	a category, or list of usage keys identifying OCG usage 
	dictionary entries. See the <i>PDF Reference</i>, and the <b>OCTextAutoStateSnip</b> 
	example in the Snippet Runner. </p>

	@param ocg The optional-content group object. 
	@param ctx The context for which the group is tested.
	@return <code>true</code> if the group is marked as being overridden in the context, 
	<code>false</code> otherwise. 
	@see PDOCGSetUserOverride 
	@see PDOCContextClearAllUserOverrides 
	@see PDOCContextFindAutoStateChanges 
	@see PDOCContextApplyAutoStateChanges 
	@see PDOCContextMakeCopyWithAutoStateChanges 
	@since PI_PDMODEL_VERSION >= 0x00060000
*/
NPROC( ASBool, PDOCGGetUserOverride, (PDOCG ocg, PDOCContext ctx) )

/**
	Removes usage override marks in all optional-content groups 
	in the given context. 

	<p>When an optional-content group is marked as having had its 
	state set explicitly in a specified context, automatic state 
	changes caused by the <code>View</code> event are prevented. When a group's 
	automatic state change is caused by the <code>Export</code> or <code>Print</code> 
	event, the user-override setting for the group is ignored. </p>
	
	<p>A configuration's AS array defines how usage entries are 
	used to automatically manipulate the OCG states. It associates 
	an event (<code>View</code>, <code>Print</code>, or <code>Export</code>) with a list of OCGs and 
	a category, or list of usage keys identifying OCG usage 
	dictionary entries. See the <i>PDF Reference</i>, and the <b>OCTextAutoStateSnip</b> 
	example in the Snippet Runner. </p>

	@param ctx The context for which the user override marks 
	are removed.
	@see PDOCGGetUserOverride 
	@see PDOCGSetUserOverride 
	@see PDOCContextApplyAutoStateChanges 
	@see PDOCContextFindAutoStateChanges 
	@see PDOCContextMakeCopyWithAutoStateChanges 
	@since PI_PDMODEL_VERSION >= 0x00060000
*/
NPROC( void, PDOCContextClearAllUserOverrides, (PDOCContext ctx) )

/**
	Replaces the page's contents with a version that has no 
	optional content, containing only what was visible on the 
	page when the call was made. 
	@param pdPage The page to be modified. 
	@param context The optional-content context in which content 
	is checked for visibility.
	@return <code>true</code> if the operation is successful, <code>false</code> otherwise. 
	@see PDDocFlattenOC 
	@see PDEContentFlattenOC 
	@since PI_PDMODEL_VERSION >= 0x00060000
*/
NPROC( ASBool, PDPageFlattenOC, (PDPage pdPage, PDOCContext context) )

/**
	Replaces the contents of every page in the document with 
	a version that has no optional content, containing only 
	what was visible on the page when the call was made, and 
	removes all other optional-content information. 
	@param pdDoc The document to be modified. 
	@param context The optional-content context in which content 
	is checked for visibility.
	@return <code>true</code> if the operation is successful, <code>false</code> otherwise. 
	@see PDPageFlattenOC 
	@see PDEContentFlattenOC 
*/
NPROC( ASBool, PDDocFlattenOC, (PDDoc pdDoc, PDOCContext context) )

/* END Optional Content API calls */

/* BEGIN Extensible PD Draw / Enum calls */

#if !MAC_PLATFORM || (MAC_PLATFORM && !AS_ARCH_64BIT)
/**
	Provides control over the rendering of contents on the page, 
	including both those parameters you would pass to PDPageDrawContentsToWindowEx(), 
	and an optional-content context that determines which contents 
	are visible. This API is not available on Mac 64.
	@param page The page to draw. 
	@param params The parameters with which to draw the page, 
	including the optional-content context to use for content 
	visibility.
	@exception pdPErrUnableToCreateRasterPort 
	@see PDPageDrawContentsToWindow 
	@see PDPageDrawContentsToWindowEx 
	@since PI_PDMODEL_VERSION >= 0x00060000
*/
NPROC( void, PDPageDrawContentsWithParams, (PDPage page, PDDrawParams params) )
#endif /* #if !MAC_PLATFORM || (MAC_PLATFORM && !AS_ARCH_64BIT) */

#if !MAC_PLATFORM || (MAC_PLATFORM && !AS_ARCH_64BIT)
/**
	Provides control over the rendering of contents, including 
	both those parameters you would pass to PDDrawCosObjWithParams(), 
	and an optional-content context that determines which contents 
	are visible.  This API is not available on Mac 64.
	@param cosObj The object to draw. 
	@param params The parameters with which to draw the object, 
	including the optional-content context to use for content 
	visibility.
	@exception pdPErrUnableToCreateRasterPort 
	@see PDDrawCosObjToWindow 
	@see PDPageEnumContents 
	@since PI_PDMODEL_VERSION >= 0x00060000
*/
NPROC( void, PDDrawCosObjWithParams, (CosObj cosObj, PDDrawParams params) )
#endif /* #if !MAC_PLATFORM || (MAC_PLATFORM && !AS_ARCH_64BIT) */

/**
	Enumerates a form's drawing operations for those contents 
	that are visible in a given optional-content context. The 
	parameters include both the monitor and data you would pass 
	to PDFormEnumPaintProc(), and an optional-content context 
	that determines which contents are visible. 
	@param obj The form whose drawing operations are enumerated. 
	
	@param params The parameters, including the optional-content 
	context to use for content visibility.
	@exception pdPErrUnableToCreateRasterPort 
	@see PDFormEnumPaintProc 
	@see PDCharProcEnumWithParams 
	@since PI_PDMODEL_VERSION >= 0x00060000
*/
NPROC( void, PDFormEnumPaintProcWithParams, (PDXObject obj, PDGraphicEnumParams params) )

/**
	Enumerates the graphic description of a single character 
	procedure for a Type 3 font, for those contents that are 
	visible in a given optional-content context. The parameters 
	include both the monitor and data you would pass to PDCharProcEnum(), 
	and an optional-content context that determines which contents 
	are visible. 
	@param obj The character procedure whose graphic 
	descriptions are enumerated. 
	@param params The parameters, including the optional-content 
	context to use for content visibility.
	@exception pdPErrUnableToCreateRasterPort 
	@see PDCharProcEnum 
	@see PDFormEnumPaintProcWithParams 
	@see PDPageEnumContents 
	@since PI_PDMODEL_VERSION >= 0x00060000
*/
NPROC( void, PDCharProcEnumWithParams, (PDCharProc obj, PDGraphicEnumParams params) )
/* END Extensible PD Draw / Enum calls */


/**
	Finds all words on the specified page that are visible in 
	the given optional-content context and returns one or more 
	tables containing the words. One table contains the words 
	sorted in the order in which they appear in the PDF file, 
	while the other contains the words sorted by their x- and 
	y-coordinates on the page. 

	<p>The list contains only words that are visible in the given 
	context. If the word states change in the given context, 
	the word list will have to be released and re-acquired to 
	reflect the changed set of visible words. </p>

	<p>There can be only one word list in existence at a time; 
	clients must release the previous word list, using PDWordFinderReleaseWordList(), 
	before creating a new one. </p>

	<p>Use PDWordFinderEnumWords() instead of this method if you 
	wish to find one word at a time instead of obtaining a table 
	containing all visible words on a page. </p>

	<p>This procedure is intended to replace the call to PDWordFinderAcquireWordList() 
	in most cases where you want to work only with the content 
	that is visible on screen (such as a text selection). Change 
	this call to update an application to work with the Optional 
	Content feature. </p>

	@param wObj The word finder (created using PDDocCreateWordFinder() 
	or PDDocCreateWordFinderUCS()) used to acquire the word list. 
	
	@param pgNum The page number for which words are found. 
	First page is <code>0</code>, not <code>1</code> as designated in Acrobat. 
	@param ocContext The context within which the words are 
	in a visible state. <code>NULL</code> is equivalent to passing <code>PDDocGetOCContext(pdDoc)</code>.
	@param wInfoP (Filled by the method) A user-supplied PDWord 
	variable. Acrobat will fill this in to point to an Acrobat-allocated 
	array of PDWord objects, which should <i>never</i> be accessed directly.

	<p>Access the acquired list through PDWordFinderGetNthWord(). 
	The words are ordered in PDF order, which is the order in 
	which they appear in the PDF file's data. This is often, 
	but not always, the order in which a person would read the 
	words. Use PDWordFinderGetNthWord to traverse this array; you 
	cannot access this array directly. This array is always 
	filled, regardless of the flags used in the call to PDDocCreateWordFinder() 
	or PDDocCreateWordFinderUCS(). </p>

	@param xySortTable (Filled by the method) Acrobat fills 
	in this user-supplied pointer to a pointer with the location 
	of an Acrobat-allocated array of PDWords, sorted in x-y 
	order, meaning that all words on the first <i>line</i>, from left 
	to right, followed by all words on the next line. This 
	array is only filled if the WXE_XY_SORT flag was set in 
	the call to PDDocCreateWordFinder() or PDDocCreateWordFinderUCS(). 
	PDWordFinderReleaseWordList() <i>must</i> be called to release allocated 
	memory for this return or there will be a memory leak. As 
	long as this parameter is non-<code>NULL</code>, the array is always 
	filled regardless of the value of the rdFlags parameter 
	in PDDocCreateWordFinder(). 
	@param rdOrderTable Currently unused. Pass <code>NULL</code> for its value. 
	@param numWords (Filled by the method) The number of visible 
	words found on the page.
	@exception pdErrOpNotPermitted 
	@see PDDocCreateWordFinder 
	@see PDDocCreateWordFinderUCS 
	@see PDWordFinderAcquireWordList 
	@see PDWordFinderReleaseWordList 
	@see PDWordFinderEnumWords 
	@see PDWordFinderGetNthWord 
	@since PI_PDMODEL_VERSION >= 0x00060000
*/
NPROC(void, PDWordFinderAcquireVisibleWordList, ( PDWordFinder wObj, ASInt32 pgNum, PDOCContext ocContext, PDWord *wInfoP, PDWord **xySortTable, PDWord **rdOrderTable, ASInt32 *numWords))

/**
	Tests whether a word is visible in a given optional-content 
	context on a given page. 
	@param word The word to test. 
	@param pageNum The page number for which the word is tested. 
	
	@param ctx The context in which the word is tested, as 
	returned by <code>PDDocGetOCContext(pdDoc)</code>.
	@return <code>true</code> if the word is visible in the given context, 
	<code>false</code> if it is hidden. 
	@see PDWordMakeVisible 
	@see PDWordFinderAcquireVisibleWordList 
	@see PDWordFinderEnumVisibleWords 
	@since PI_PDMODEL_VERSION >= 0x00060000
*/
NPROC(ASBool, PDWordIsCurrentlyVisible, (PDWord word, ASInt32 pageNum, PDOCContext ctx))

/**
	Makes a word visible in a given optional-content context 
	on a given page. 
	@param word The word to test. 
	@param pageNum The page number for which the word is to 
	be made visible. 
	@param ctx The context in which the word is to be made 
	visible, as returned by <code>PDDocGetOCContext(pdDoc)</code>.
	@return <code>true</code> if the word can be made visible in the given 
	context, <code>false</code> otherwise. 
	@see PDWordIsCurrentlyVisible 
	@see PDWordFinderAcquireVisibleWordList 
	@see PDWordFinderEnumVisibleWords 
	@since PI_PDMODEL_VERSION >= 0x00060000
*/
NPROC(ASBool, PDWordMakeVisible, (PDWord word, ASInt32 pageNum, PDOCContext ctx))

/**
	Extracts visible words, one at a time, from the specified 
	page or the entire document. It calls a user-supplied procedure 
	once for each word found. If you wish to extract all text 
	from a page at once, use PDWordFinderAcquireWordList() instead 
	of this method. 

	<p>Only words that are visible in the given optional-content 
	context are enumerated. </p>

	@param wObj A word finder object. 
	@param PageNum The page number from which to extract words. 
	Pass PDAllPages (see PDExpT.h) to sequentially process all 
	pages in the document. 
	@param ocContext The context within which the words are 
	in a visible state. <code>NULL</code> is equivalent to passing <code>PDDocGetOCContext(pdDoc)</code>. 
	
	@param wordProc A user-supplied callback to call once for 
	each word found. Enumeration halts if <code>wordProc</code> returns <code>false</code>. 
	
	@param clientData A pointer to user-supplied data to pass 
	to <code>wordProc</code> each time it is called.
	@return <code>true</code> if enumeration was successfully completed, 
	<code>false</code> if enumeration was terminated because <code>wordProc</code> returned 
	<code>false</code>. 
	@exception genErrBadParm is raised if <code>wordProc</code> is <code>NULL</code>, or <code>pageNum</code> is 
	less than zero or greater than the total number of pages in the document. 
	@exception pdErrOpNotPermitted 
	@see PDDocCreateWordFinder 
	@see PDDocCreateWordFinderUCS 
	@see PDDocGetWordFinder 
	@see PDWordFinderAcquireWordList 
	@see PDWordFinderEnumWords 
	@see PDWordFinderEnumWordsStr 
	@since PI_PDMODEL_VERSION >= 0x00060000
*/
NPROC(ASBool, PDWordFinderEnumVisibleWords, ( PDWordFinder wObj, ASInt32 PageNum, PDOCContext ocContext, PDWordProc wordProc, void *clientData ))

/**
	Gets the bounding box for a given page for those contents that are
	visible in the given optional-content context. The bounding box is
	the rectangle that encloses the visible text, graphics, and images
	on the page.
	@param page The page whose visible-content bounding box is obtained. 
	@param ocContext The context within which the contents are visible. 
	@param includeAnnots When <code>true</code>, include annotations as content that
	must be visible to affect the bounding box. When <code>false</code>, annotations
	are not considered at all.
	@param fr (Filled by the method) A pointer to a rectangle specifying the
	page's visible content bounding box, specified in user space coordinates.
	The client must not pass <code>NULL</code>.
	@see PDPageGetBBox
	@since PI_PDMODEL_VERSION >= 0x00060002
*/
NPROC(void, PDPageGetVisibleBBox, (PDPage page, PDOCContext ocContext, ASBool includeAnnots, ASFixedRect *fr))
#ifndef ANDROID_ENV
/* BEGIN Crypt filter support */

/**
	Sets or resets the specified document's security filter 
	method, used for encryption and decryption of the document's 
	data. 
	@param doc The document whose new security filter method 
	is set.
	@param filterName The ASAtom corresponding to the name of 
	the security filter to use.
	@param method One of five supported security methods:
	<ul>
	<li>None (default)</li>
	<li>V2 (RC4)</li>
	<li>V3 (RC4)</li>
	<li>AESV1</li>
	<li>AESV2 (128 bit)</li>
	<li>AESV3 (256 bit)</li>
	</ul>
	@exception pdErrNoCryptHandler is raised if there is no security handler 
	registered for the document. 
	@see PDCryptAuthorizeFilterAccess 
	@see PDDocSetNewCryptFilterData 
	@see PDDocSetNewDefaultFilters 

	@product_exclude RDR 
	@since PI_PDMODEL_VERSION >= 0x00060000
*/
NPROC( void, PDDocSetNewCryptFilterMethod, (PDDoc doc, ASAtom filterName, ASAtom method) )

/**
	Sets the encrypted data for the specified document's encryption 
	filter to decrypt. Call this before accessing the stream to be 
	decrypted. 
	@param doc The document whose new encrypted data is set. 
	
	@param filterName The ASAtom corresponding to the name 
	of the security filter used by the document. 
	@param cryptData The new encrypted data for the document. 
	
	@param cryptDataLen The length of <code>cryptData</code> in bytes.
	@exception pdErrNoCryptHandler is raised if there is no security handler 
	registered for the document. 
	@exception pdErrOpNotPermitted is raised if the document's permissions 
	do not allow its data to be modified. 
	@see PDCryptAuthorizeFilterAccess 
	@see PDDocSetNewCryptFilterMethod 
	@see PDDocSetNewDefaultFilters 

	@product_exclude RDR 
	@since PI_PDMODEL_VERSION >= 0x00060000
*/
NPROC( void, PDDocSetNewCryptFilterData, (PDDoc doc, ASAtom filterName, char *cryptData, ASInt32 cryptDataLen) )

/**
	Sets or resets the document's default security filter methods 
	for streams and strings, used to encrypt and decrypt the 
	document's data. This method is only valid with version 
	4 algorithms (/V 4 in the Encrypt dictionary). 
	@param doc The document whose new security filter is set. 
	
	@param defaultStmFilterName The ASAtom corresponding to 
	the name of the default security filter to use for streams. 
	The filter must exist and be registered. 
	@param defaultStrFilterName The ASAtom corresponding to 
	the name of the default security filter to use for strings. 
	The filter must exist and be registered.
	@exception pdErrNoCryptHandler is raised if there is no security handler 
	registered for the document. 
	@see PDCryptAuthorizeFilterAccess 
	@see PDDocSetNewCryptFilterData 
	@see PDDocSetNewCryptFilterMethod 
	@product_exclude RDR
	@since PI_PDMODEL_VERSION >= 0x00060000
*/
NPROC( void, PDDocSetNewDefaultFilters, (PDDoc doc, ASAtom defaultStmFilterName, ASAtom defaultStrFilterName) )

/**
	Gets authorization to encrypt or decrypt an embedded file, 
	where that file's cryptographic filter is not the one used 
	to open the document in which the file is embedded. 
	@param doc The document containing the embedded file whose 
	filter access is requested. 
	@param handlerName The security handler containing the 
	Authorize() callback procedure to run. 
	@param filterName The ASAtom corresponding to the name 
	of the security filter used by the embedded file. 
	@param bEncrypt When <code>true</code>, the access is required for 
	an encryption operation. When <code>false</code>, it is a decryption 
	operation.
	@return <code>true</code> if the authorization succeeds, <code>false</code> otherwise. 
	@exception pdErrNoCryptHandler is raised if there is no security handler 
	registered for the document. 
	@see PDDocSetNewCryptFilterData 
	@see PDDocSetNewCryptFilterMethod 
	@see PDDocSetNewDefaultFilters 
	@since PI_PDMODEL_VERSION >= 0x00060000
*/
NPROC( ASBool, PDCryptAuthorizeFilterAccess, (PDDoc doc, ASAtom handlerName, ASAtom filterName, ASBool bEncrypt) )

/* END Crypt Filter support */
#endif
/* BEGIN PDDocRequestXX support */

/**
	Requests <code>nPages</code> starting at <code>startPage</code>, and performs a specified 
	procedure on them. 
	@param doc The document for which pages are read ahead. 
	
	@param startPage The first page requested. 
	@param nPages The number of pages requested. 
	@param requestProc The procedure to call to process the 
	request. 
	@param clientData A pointer to user-defined data to pass 
	to the <code>requestProc</code>.
	@since PI_PDMODEL_VERSION >= 0x00060000
*/
NPROC(void, PDDocRequestPages,(PDDoc doc, ASInt32 startPage, ASInt32 nPages, PDDocRequestPagesProc requestProc,	 void * clientData))

/**
	Requests the document file and performs the specified procedure 
	on it. 
	@param doc The document for which pages are read ahead. 
	@param requestProc The procedure to call to process the 
	request. 
	@param clientData A pointer to user-defined data to pass 
	to the <code>requestProc</code>.
	@since PI_PDMODEL_VERSION >= 0x00060000
*/
NPROC(void, PDDocRequestEntireFile, (PDDoc doc,  PDDocRequestEntireFileProc requestProc, void * clientData))


/* END PDDocRequestXX support */


/**
	For internal use only.
	@since PI_PDMODEL_VERSION >= 0x00060000
*/
NPROC(void, PDSetHostEncoding, (ASHostEncoding encoding, char* parseTable))

/**
	Tests whether the data from an annotation on a given page 
	can be copied to a clipboard for pasting. This depends on 
	whether there is a PDAnnotHandler with copy and paste support 
	for the annotation, and whether copying is allowed by document 
	permissions. 
	@param sourcePage The page containing the annotation to 
	test. It can be <code>NULL</code> (as when copying annotations while spawning 
	a hidden template). 
	@param annot The annotation to test.
	@return <code>true</code> if the annotation object can be copied, <code>false</code> otherwise. 
	
	@see PDAnnotCanPaste 
	@see PDAnnotCopy 
	@see PDAnnotDestroyClipboardData 
	@see PDAnnotPaste 
	@since PI_PDMODEL_VERSION >= 0x00060000
*/
UNPROC(ASBool, PDAnnotCanCopy, (PDPage sourcePage, PDAnnot annot))

/**
	Copies action object data to a clipboard structure, from 
	which it can be pasted. 
	@param sourcePage The page containing the annotation to 
	copy. It can be <code>NULL</code> (as when copying annotations while spawning 
	a hidden template). 
	@param annot The annotation to copy.
	@return The annotation clipboard data object. 
	@see PDAnnotCanCopy 
	@see PDAnnotCanPaste 
	@see PDAnnotDestroyClipboardData 
	@see PDAnnotPaste 
	@since PI_PDMODEL_VERSION >= 0x00060000
*/
UNPROC(PDAnnotClipboardData, PDAnnotCopy, (PDPage sourcePage, PDAnnot annot))

/**
	Tests whether data from an annotation that has been copied 
	to a clipboard can be pasted to a location on a page. Pasting 
	can be disallowed by document permissions, or because the 
	annotation cannot be accurately reproduced in the destination 
	document. 
	@param destPage The page to which the annotation would 
	be pasted. 
	@param center The location for the center of the annotation 
	on the destination page, or a <code>NULL</code> pointer to center the 
	annotation on the destination page. 
	@param data The copied annotation data to test.
	@return <code>true</code> if the annotation data can be pasted, <code>false</code> otherwise. 
	
	@see PDAnnotCanCopy 
	@see PDAnnotCopy 
	@see PDAnnotDestroyClipboardData 
	@see PDAnnotPaste 
	@since PI_PDMODEL_VERSION >= 0x00060000
*/
UNPROC(ASBool, PDAnnotCanPaste, (PDPage destPage, const ASFixedPoint *center, PDAnnotClipboardData data))

/**
	Pastes copied annotation data from a clipboard structure 
	to a new annotation object in a specified document. After 
	successfully pasting the data, use PDAnnotDestroyClipboardData() 
	to free the associated memory. 
	@param destPage The page to which the annotation is pasted. 
	
	@param center The location for the center of the annotation 
	on the destination page, or a <code>NULL</code> pointer to center the 
	annotation on the destination page. 
	@param data The copied annotation data to paste.
	@return A newly created annotation object associated with the specified 
	document, containing the same data as the copied annotation. 
	
	@see PDAnnotCanCopy 
	@see PDAnnotCanPaste 
	@see PDAnnotCopy 
	@see PDAnnotDestroyClipboardData 
	@since PI_PDMODEL_VERSION >= 0x00060000
*/
UNPROC(PDAnnot, PDAnnotPaste, (PDPage destPage, const ASFixedPoint *center, PDAnnotClipboardData data))

/**
	Destroys data that has been copied from an annotation object 
	into a clipboard. Use this method after successfully pasting 
	the data to a new document. 
	@param data The clipboard annotation data to destroy.
	@see PDAnnotCanCopy 
	@see PDAnnotCanPaste 
	@see PDAnnotCopy 
	@see PDAnnotPaste 
	@since PI_PDMODEL_VERSION >= 0x00060000
*/
UNPROC(void, PDAnnotDestroyClipboardData, (PDAnnotClipboardData data))


/**
	Used for page-at-a-time downloading and byte-serving Acrobat 
	data. If a document is being viewed over a slow file system, 
	the method issues a byte range request for all the data 
	associated with an embedded file. 
	@param doc The document being read. 
	@param embeddedFileObj The Cos object of the embedded 
	file stream (the stream referenced by entries in the EF 
	dictionary).
	@since PI_PDMODEL_VERSION >= 0x00060000
*/
NPROC(void, PDDocReadAheadEmbeddedFile, (PDDoc doc, CosObj embeddedFileObj))

#ifndef ANDROID_ENV
/**
	Extends PDDocSetNewCryptHandler() for Acrobat 6.0. It sets the specified 
	document's new security handler (that is, the security handler 
	that will be used after the document is saved). This method 
	should be called when the current document's security handler 
	requires authorization data to validate permission to change 
	security handlers. 

	<p>This method returns with no action if the new security handler 
	is the same as the old one. Otherwise, the new security 
	handler's PDCryptNewSecurityDataProc() is called to set the 
	document's <code>newSecurityData</code> field. If the new security handler 
	does not have this callback, the document's <code>newSecurityData</code> 
	field is set to <code>0</code>. </p>

	@param pdDoc The document whose new security handler is 
	set. 
	@param newCryptHandler The ASAtom corresponding to the 
	name of the new security handler to use for the document. 
	This name must be the same as the <code>pdfName</code> used when the 
	security handler was registered using PDRegisterCryptHandler(). 
	Use ASAtomNull to remove security from the document. 
	@param currentAuthData A pointer to authorization data 
	to be passed to the PDCryptAuthorizeProc() callback for the 
	document's current security handler. For the Acrobat viewer's 
	built-in security handler, the password is passed in the 
	<code>authData</code> parameter.
	@exception pdErrNoCryptHandler is raised if there is no security handler 
	registered with the specified name and the name is not ASAtomNull. 
	@exception pdErrOpNotPermitted is raised if the document's permissions 
	do not allow its security to be modified. 
	@see PDDocGetNewCryptHandler 
	@see PDDocPermRequest 
	@see PDDocSetNewCryptHandler 
	@see PDRegisterCryptHandler 
	@see PDRegisterCryptHandlerEx 
	@since PI_PDMODEL_VERSION >= 0x00060000
*/
UNPROC(void, PDDocSetNewCryptHandlerEx, (PDDoc pdDoc, ASAtom newCryptHandler, void *currentAuthData))
#endif

/**
	Gets the value of the Trapped key in the Info dictionary. 
	
	@param pdDoc The document whose Trapped key value is obtained.
	@return The value of the Trapped key in the Info dictionary if the 
	entry exists and is a name, or ASAtomNull if the entry does 
	not exist or is not a name. 
	@see PDDocSetTrapped 
	@since PI_PDMODEL_VERSION >= 0x00060000
*/

NPROC(ASAtom, PDDocGetTrapped, (PDDoc pdDoc))

/**
	Sets the value of the Trapped key in the Info dictionary 
	to the specified ASAtom. 

	<p>This method causes the corresponding XMP metadata item to 
	be set to a string reflecting the characters in the ASAtom. </p>
	
	@param pdDoc The document whose Trapped key value to set. 
	
	@param newValue The new value of the Trapped key in the 
	Info dictionary, or ASAtomNull to remove any existing entry. 
	The method does not check that the value is one of the allowed 
	values for the key.
	@see PDDocGetTrapped 
	@since PI_PDMODEL_VERSION >= 0x00060000
*/

UNPROC(void, PDDocSetTrapped, (PDDoc pdDoc, ASAtom newValue))


/**
	Supersedes PDDocGetLabelForPageNum() in Acrobat 6.0. 

	<p>Retrieves the label string associated with the given page 
	number. The page number is returned in host encoding as 
	a ASText object. </p>

	@param pdDoc The document containing the page for which a 
	label is requested. 
	@param pageNum The number of the page whose label is requested. 
	
	@param text If a label exists for <code>pageNum</code>, it is returned 
	in this object.
	@see PDDocFindPageNumForLabelEx 
	@see PDDocGetPageLabel 
	@see PDDocRemovePageLabel 
	@see PDDocSetPageLabel 
	@see PDPageLabelNew 
	@since PI_PDMODEL_VERSION >= 0x00060000
*/
NPROC(void, PDDocGetLabelForPageNumEx, (
	PDDoc	pdDoc,
	ASInt32	pageNum,
	ASText	text))

/**
	Supersedes PDDocFindPageNumForLabel in Acrobat 6.0. 

	<p>Finds the first page in the document with a specified label. </p>
	
	@param pdDoc The document to search for the page named 
	in <code>labelStr</code>. 
	@param labelText The label of the page to find.
	@return The page number of the first page with the specified label, 
	or <code>-1</code> if no such page exists. 
	@see PDDocGetLabelForPageNumEx 
	@see PDDocGetPageLabel 
	@see PDDocRemovePageLabel 
	@see PDDocSetPageLabel 
	@see PDPageLabelNew 
	@since PI_PDMODEL_VERSION >= 0x00060000
*/
NPROC(ASInt32, PDDocFindPageNumForLabelEx, (
	PDDoc	pdDoc,
	ASConstText labelText))

/**
	Fills in an ASText object with the font name, to be used 
	in displaying lists or menus. 

	<p>In PDF 1.5, the font name can be represented with a UTF8 
	byte sequence. In previous versions of Acrobat the name 
	could also be represented by host encodings such as Shift- 
	JIS, Big5, KSC, and so on. This routine tries to return 
	a text object that uses the correct script, but cannot always 
	do so. </p>

	<p>The ASText object is owned by the caller. </p>

	@param font The font whose name is obtained. 
	@param removePrefix Whether to remove the subset prefix, 
	if present. For example, when <code>true</code>, the name <code>"ABCDEF+Myriad"</code> 
	is returned as <code>"Myriad"</code>. 
	@param nameToFill (Filled by the method) The ASText object 
	for the font's name.
	@since PI_PDMODEL_VERSION >= 0x00060000
*/
NPROC(void, PDFontGetASTextName, (PDFont font, ASBool removePrefix, ASText nameToFill))

/**
	Increments the page's reference count. After you are done using the page, 
	release it using PDPageRelease(). If PDPageRelease() is not called, it could
	block the document containing the page from being closed. To avoid such 
	problems use the CSmartPDPage class as it ensures that the page is released
	as it goes out of scope. 
	@param pdPage IN The page whose reference count is to be incremented. 
	@see CSmartPDPage
	@see PDPageRelease 
	@since PI_PDMODEL_VERSION >= 0x0007000<code>0</code>.
*/
NPROC(void, PDPageAcquirePage, (PDPage pdPage))

/**
	Returns the locked state of an OCG in a given configuration.
	The on/off state of a locked OCG cannot be toggled by the user through the user interface.
	@param ocg IN The PDOCG whose locked state is requested.
	@param pdOCCfg IN The optional-content configuration. 
	@return An ASBool that is <code>true</code> if the OCG is locked,
	and <code>false</code> if it is unlocked.
	
	@see PDOCGSetLocked
	@see PDOCConfigGetLockedArray 
	@see PDOCConfigSetLockedArray
	@since PI_PDMODEL_VERSION >= 0x00070000
*/
NPROC(ASBool, PDOCGGetLocked, (PDOCG ocg, PDOCConfig pdOCCfg))

/**
	Sets the locked state of an OCG in a given configuration.
	The on/off state of a locked OCG cannot be toggled by the user through the user interface.
	@param ocg IN The PDOCG whose locked state is to be set.
	@param pdOCCfg IN/OUT The optional-content configuration.
	@param locked IN An ASBool that is <code>true</code> if the OCG should be locked,
	and <code>false</code> if it should be unlocked.
	
	@see PDOCGGetLocked
	@see PDOCConfigGetLockedArray 
	@see PDOCConfigSetLockedArray
	@since PI_PDMODEL_VERSION >= 0x00070000
*/
NPROC(void, PDOCGSetLocked, (PDOCG ocg, PDOCConfig pdOCCfg, ASBool locked))

/**
	Returns a PDOCConfig object's list of locked OCGs.
	The on/off state of a locked OCG cannot be toggled by the user through the user interface.
	@param pdOCCfg The optional-content configuration. 
	@return A <code>NULL</code>-terminated array of PDOCG objects, or <code>NULL</code> if the 
	specified configuration does not contain a list of locked OCGs. 
	The client is responsible for freeing the array using ASfree(). 
	
	@see PDOCConfigSetLockedArray 
	@see PDOCGGetLocked
	@see PDOCGSetLocked
	@since PI_PDMODEL_VERSION >= 0x00070000
*/
NPROC(PDOCG*, PDOCConfigGetLockedArray, (PDOCConfig pdOCCfg))

/**
	Sets a PDOCConfig's list of locked OCGs.
	The on/off state of a locked OCG cannot be toggled by the user through the user interface.
	@param pdOCCfg The optional-content configuration.
	@param lockedOCGs A <code>NULL</code>-terminated array of PDOCG objects to be used
	as the locked OCGs for the specified configuration, or <code>NULL</code> if the
	configuration should not contain a list of locked OCGs.
	
	@see PDOCConfigGetLockedArray 
	@see PDOCGGetLocked
	@see PDOCGSetLocked
	@since PI_PDMODEL_VERSION >= 0x00070000
*/
NPROC(void, PDOCConfigSetLockedArray, (PDOCConfig pdOCCfg, PDOCG* lockedOCGs))

/**
	Locates an existing optional-content membership dictionary 
	(PDOCMD) object that references the given groups, uses the
	same visibility policy, and uses the same visibility expression.
	If no such PDOCMD is found, the method creates one. 

	<p>The fourth parameter, <code>veObj</code> must be a CosNull object or a CosArray object.
	If it is a CosNull object, this call is identical to PDOCMDFindOrCreate().
	If it is an array object, it is used as the OCMD's visibility expression.</p>

	@param pdDoc The PDDoc in which to create the PDOCMD.
	@param ocgs A list of OCGs, or <code>NULL</code> if only a visibility expression is to be used.
	@param policy The visibility policy to use. unused if <code>ocgs</code> is <code>NULL</code>.
	@param veObj A CosObj containing a visibility expression.

	@see PDOCMDFindOrCreate 
	@since PI_PDMODEL_VERSION >= 0x00070000
*/
NPROC(PDOCMD, PDOCMDFindOrCreateEx, (PDDoc pdDoc, PDOCG *ocgs, PDOCMDVisPolicy policy, CosObj veObj))

/**
	If the PDOCMD has a visibility expression entry, the function returns <code>true</code>, and
	if <code>veObj</code> is non-<code>NULL</code>, <code>*veObj</code> is set to the CosObj for the visibility expression.
	If the PDOCMD does not have a visibility expression entry, the function returns <code>false</code>.

	@param ocmd The PDOCMD in which to check for a visibility expression.
	@param veObj The cos object in which to return the visibility expression.
	@return <code>true</code> if there is a visibility expression, <code>false</code> otherwise.

	@see PDOCMDFindOrCreateEx
	@since PI_PDMODEL_VERSION >= 0x00070000
*/
NPROC(ASBool, PDOCMDGetVisibilityExpression, (PDOCMD ocmd, CosObj *veObj))

/**
	Returns the UserUnit value for the page. If the key is
	not present in the page dictionary the default of <code>1.0</code>
	is returned.
	@param page The page whose UserUnit value is being obtained. 
	@return The value of UserUnit from the page dictionary, or a default value of <code>1.0</code> if not present.
*/
NPROC(float, PDPageGetUserUnitSize, (PDPage page))

/**
	Set the UserUnit value for a page. 
	@param page The page whose UserUnit value is being set.
	@param unitSize UserUnit value to be set for the page dictionary. The default value is <code>1.0</code>.
	@exception genErrBadParm is raised if <code>unitSize <= 0.0</code> .
*/
NPROC(void, PDPageSetUserUnitSize, (PDPage page, float unitSize))

#ifndef ANDROID_ENV
/**
	<p>PDDocPermRequestNoUB() indicates whether the permission would have been granted had the document not been Rights Enabled.</p>

	<p>This may throw numerous exceptions.</p>

	@param pdDoc The PDDoc whose permissions are being requested. 
	
	@param reqObj The target object of the permissions request. 
	
	@param reqOpr The target operation of the permissions 
	request. 
	@param authData A pointer to an authorization data structure.
	@return The request status constant: <code>0</code> if the requested operation 
	is allowed, a non-zero status code otherwise. 
	@see PDDocPermRequest
	@since PI_PDMODEL_VERSION >= 0x00070000
*/
NPROC(PDPermReqStatus, PDDocPermRequestNoUB, (PDDoc pdDoc, PDPermReqObj reqObj, PDPermReqOpr reqOpr, void *authData))
#endif

/**
	Enumerates the inks for a page, calling the supplied procedure 
	for each PDPageInk structure. This differs from PDPageEnumInks()
	in that it allows the process color model to be passed in.

	<p>For the DeviceCMYK_K process color model, it always finds the 
	four inks Cyan, Magenta, Yellow, and Black, which are marked 
	as process inks. The RGB values in the PDPageInk structure 
	are the RGB equivalents (in system monitor space) of 100% 
	of the ink, which can be used to show color swatches for 
	a given ink. </p>

	<p>If the inks are part of a DeviceN color space which has not 
	been defined in a Colorants dictionary or elsewhere in a 
	Separation color space, the color of the swatch is undefined. </p>
	
	<p>This call finds all color spaces that are in a color space 
	dictionary within the page, even if they are not used by 
	the page contents. </p>

	@param pdPage The page whose contents are enumerated. 
	@param proc The user-supplied callback procedure to be 
	applied to each ink. Enumeration ends if any procedure returns 
	<code>false</code>. 
	@param clientData A pointer to user-supplied data to pass 
	to <code>proc</code> each time it is called.
	@param includeOPI If <code>true</code>, enumerate inks contained in OPI dictionaries.
	@param colorModel <code>CMYK_K</code>, <code>RGB_K</code>, or <code>Gray_K</code>.
	@see PDPageMakeSeparations 
	@see AVPageViewGetNumVisibleInks 
	@see AVPageViewGetVisibleInks 
	@see AVPageViewGetPixelInformationAtPoint 
	@see AVPageViewSetInkPreview 
	@see AVPageViewSetVisibleInks 
  	@ingroup Enumerators
	@since PI_PDMODEL_VERSION >= 0x00060000
*/

NPROC(void, PDPageEnumInksEx, ( PDPage pdPage, PDPageEnumInksCallback proc, void *clientData, ASBool includeOPI, ASAtom colorModel ))

/** 
Adds a PDPage as a watermark to a page range in the given document.
 @param pdDoc The document onto which the watermark will be added.
 @param pdPage The page to be added as a watermark.
 @param pParams Structure specifying how the watermark should be added to the document.
*/
XNPROC(void, PDDocAddWatermarkFromPDPage, (PDDoc pdDoc, PDPage pdPage, PDDocAddWatermarkParamsRec *pParams))

/** 
Adds a text-based watermark to a page range in the given document.
 @param pdDoc The document onto which the watermark will be added.
 @param pTextParams Structure describing the text-based watermark to be added.
 @param pParams Structure specifying how the watermark should be added to the document.
*/
XNPROC(void, PDDocAddWatermarkFromText, (PDDoc pdDoc, PDDocWatermarkTextParamsRec *pTextParams, PDDocAddWatermarkParamsRec *pParams))

/**
	Sets the value of the PageLayout key in the Catalog dictionary. 
	
	@param doc IN The document whose page mode is set. 
	@param mode IN The layout mode to set. 
	@see PDDocGetLayoutMode 
	@since PI_PDMODEL_VERSION >= 0x00070000
*/
UNPROC(void, PDDocSetLayoutMode, (PDDoc doc, PDLayoutMode mode))

/**
	Gets the value of the PageLayout key in the Catalog dictionary. 
	
	@param doc IN The document whose layout mode is obtained. 
	@return Layout mode value from the PDF Catalog dictionary. 
	@see PDDocSetLayoutMode 
	@since PI_PDMODEL_VERSION >= 0x00070000
*/
NPROC(PDLayoutMode, PDDocGetLayoutMode, (PDDoc doc))

#ifndef ANDROID_ENV
/**
	Gets the specified document's current security handler (that 
	is, the security handler that was used to open the document).

	@param doc The document whose new security handler is 
	obtained.
	@return The ASAtom corresponding to the name of the document's 
	security handler. It returns ASAtomNull if the document does not have
	a current security handler. 
	@see PDDocPermRequest 
	@see PDDocGetSecurityData
	@see PDDocSetNewCryptHandler 
	@see PDDocSetNewCryptHandlerEx 
	@see PDRegisterCryptHandler 
	@see PDRegisterCryptHandlerEx 
	@since PI_PDMODEL_VERSION >= 0x00070000
*/
NPROC(ASAtom, PDDocGetCryptHandler, (PDDoc doc))
#endif

/**
	Creates a new file specification from the specified ASPathName, 
	using the PDFileSpecNewFromASPathProc() of the specified file 
	system's file specification handler. 
	@param pdDoc The document in which the new file specification 
	will be used. 
	@param fileSys A pointer to an ASFileSysRec specifying the 
	file system responsible for the newly created file specification. 
	@param path The path to convert into a file specification. 
	@param relPathFileSys The file system that owns <code>relativeToThisPath</code>.
	@param relativeToThisPath A path name relative to which 
	the <code>fileSpec</code> is interpreted. If it is <code>NULL</code>, <code>fileSpec</code> is assumed 
	to be an absolute, not a relative, path.  If it is not <code>NULL</code>
	and <code>fileSys</code> and <code>relPathFileSys</code> are not the same, then an attempt
	is made to fabricate a <code>relPathName</code> in terms of <code>fileSys</code>, and if that is
	not possible, <code>NULL</code> is used.
	@return The newly created file specification, or an invalid file specification if 
	the ASPathName cannot be converted to a PDFileSpec (use 
	PDFileSpecIsValid() to test whether the conversion was successful). 
	
	@see PDFileSpecIsValid 
	@since PI_PDMODEL_VERSION >= 0x00080000
*/
UNPROC(PDFileSpec, PDFileSpecNewFromASPathEx, (PDDoc pdDoc, ASFileSys fileSys, ASPathName path, ASFileSys relPathFileSys, ASPathName relativeToThisPath))

/**
	Acquires an ASPathName for the specified file specification 
	and relative path. 
	@param fileSpec IN/OUT The file specification for which an ASPathName 
	is acquired. 
	@param relPathFileSys The file system that owns <code>relativeToThisPath</code>.
	@param relativeToThisPath IN/OUT A path name relative to which 
	the <code>fileSpec</code> is interpreted. If it is <code>NULL</code>, <code>fileSpec</code> is assumed 
	to be an absolute, not a relative, path.  If it is not <code>NULL</code>
	and <code>fileSys</code> and <code>relPathFileSys</code> are not the same, then an attempt
	is made to fabricate a <code>relPathName</code> in terms of <code>fileSys</code>, and if that is
	not possible, <code>NULL</code> is used.
	@param retFileSys IN/OUT The file system that owns the returned ASPathName.
	@param pathMustExist IN/OUT If it is <code>true</code> and the result
	ASPathName does not exist, then the return value is <code>NULL</code>.
	@return The ASPathName corresponding to <code>fileSpec</code>. 

	<p>After you are done using the ASPathName, you must free it 
	using ASFileSysReleasePath(). </p>

	@see ASFileSysReleasePath 
	@since PI_PDMODEL_VERSION >= 0x00080000
	
*/
NPROC(ASPathName, PDFileSpecAcquireASPathEx, (PDFileSpec fileSpec, ASFileSys relPathFileSys, ASPathName relativeToThisPath, ASFileSys* retFileSys, ASBool pathMustExist))

/**
	Gets the device-independent path name from a file specification. 
	
	@param fileSpec IN The file specification whose device-independent 
	path name is obtained. 
	@param diPath IN/OUT An existing ASText object whose content is set to the path name
	obtained from from <code>fileSpec</code>.
	
	@since PI_PDMODEL_VERSION >= 0x00080000
	
*/
NPROC(void, PDFileSpecGetDIPathEx, (PDFileSpec fileSpec, ASText diPath))


/**
	Gets an annotation's label text as an ASText object. 

	@param anAnnot The annotation whose label is obtained. 
	@param title (Filled by the method) The text object containing
	the annotation's label string. The client must pass a 
	valid ASText object title. The routine does not allocate it.
	
	@exception pdErrBadAnnotation
	@see PDAnnotGetTitle
	@see PDAnnotSetTitleASText 
	@see PDAnnotSetTitle 
	@see PDAnnotGetColor 
	@see PDAnnotGetDate 
	@see PDAnnotGetRect 
	@see PDAnnotGetFlags 
	@since PI_PDMODEL_VERSION >= 0x00080000
*/
NPROC(void, PDAnnotGetTitleASText, (PDAnnot anAnnot,ASText title))

/**
	Sets an annotation's label text. 

	@param anAnnot The annotation whose label is set. 
	@param title The text object containing the label to set. 
	
	@notify PDAnnotWillChange 
	@notify PDAnnotDidChange 
	@see PDAnnotSetTitle 
	@see PDAnnotGetTitleASText
	@see PDAnnotGetTitle 
	@see PDAnnotSetColor 
	@see PDAnnotSetDate 
	@see PDAnnotSetFlags 
	@see PDAnnotSetRect 
	@since PI_PDMODEL_VERSION >= 0x00080000
*/
NPROC(void, PDAnnotSetTitleASText, (PDAnnot anAnnot, const ASText title))

/**
	Gets the text of a text annotation as an ASText object. 

	@param aTextAnnot The text annotation whose text is obtained. 
	@param contents (Filled by the method) The text object containing the contents.
	The client must pass a valid ASText object title.
	The routine does not allocate it.
	
	@see PDTextAnnotGetContents
	@see PDTextAnnotSetContentsASText
	@see PDTextAnnotSetContents
	@since PI_PDMODEL_VERSION >= 0x00080000
*/
NPROC(void, PDTextAnnotGetContentsASText, (PDTextAnnot aTextAnnot, ASText contents))

/**
	Sets the text of a text annotation. 
	
	@param aTextAnnot The text annotation whose text is set. 
	@param contents The text object containing the new text. 

	@notify PDAnnotWillChange 
	@notify PDAnnotDidChange 
	@see PDTextAnnotSetContents
	@see PDTextAnnotGetContentsASText
	@see PDTextAnnotGetContents 
	@since PI_PDMODEL_VERSION >= 0x00080000
*/
NPROC(void, PDTextAnnotSetContentsASText, (PDTextAnnot aTextAnnot, const ASText contents))

/**
	Gets the value of a key in a document's Info dictionary, 
	or the value of this same key in the XMP metadata, whichever 
	is latest as an ASText object. 

	@param doc The document whose Info dictionary key is obtained. 	
	@param key The name of the Info dictionary key whose 
	value is obtained. 
	@param value (Filled by the method) The text object containing 
	the value associated with <code>key</code>. The client must pass a 
	valid ASText object value. The routine does not allocate it.
	
	@see PDDocGetInfo
	@see PDDocGetXAPMetadataProperty 
	@see PDDocSetInfoASText
	@see PDDocSetInfo
	@since PI_PDMODEL_VERSION >= 0x00080000
*/
NPROC(void,PDDocGetInfoASText, (PDDoc doc, const ASText key, ASText value))


/**
	Sets the value of a key in a document's Info dictionary. 

	@param doc The document whose Info dictionary key is set. 	
	@param infoKey The name of the Info dictionary key whose 
	value is set. 
	@param value The text object containing the value to associate 
	with <code>infoKey</code>. 
	
	@see PDDocSetInfo 
	@see PDDocGetInfoASText 
	@see PDDocGetInfo 
	@see PDDocSetXAPMetadataProperty 
	@since PI_PDMODEL_VERSION >= 0x00080000
*/
NPROC(void,PDDocSetInfoAsASText, (PDDoc doc, const ASText infoKey, const ASText value))


/**
	Gets a bookmark's title as an ASText object. 

	@param aBookmark The bookmark whose title is obtained. 	
	@param title (Filled by the method) The text object containing 
	the title. The client must pass a valid ASText 
	object title. The routine does not allocate it.
	
	@see PDBookmarkGetTitle
	@see PDBookmarkSetTitleASText
	@see PDBookmarkSetTitle 
	@see PDXlateToHost 
	@see PDXlateToHostEx 
	@since PI_PDMODEL_VERSION >= 0x00080000
*/
NPROC(void,PDBookmarkGetTitleASText, (PDBookmark aBookmark, ASText title)) 

/**
	Sets a bookmark's title. 

	@param aBookmark The bookmark whose title is set. 
	@param title The text object containing the bookmark's 
	new title.
	
	@exception pdErrBookmarksError is raised if there is an error setting 
	the title. 
	@notify PDBookmarkWillChange 
	@notify PDBookmarkDidChange 
    @see PDBookmarkSetTitle 
	@see PDBookmarkGetTitleASText 
	@see PDBookmarkGetTitle 
	@since PI_PDMODEL_VERSION >= 0x00080000
*/
NPROC(void,PDBookmarkSetTitleASText, (PDBookmark aBookmark, const ASText title)) 


/**
	Adds a new bookmark to the tree containing <code>aBookmark</code>, 
	as the new right sibling. 
	@param aBookmark The bookmark that will be the left sibling 
	of the new bookmark. 
	@param initialText The text object containing the new bookmark's title.
	@return The newly created bookmark.

	@notify PDBookmarkDidChangePosition 
	@notify PDBookmarkWasCreated 
	
	@see PDBookmarkAddNewSibling
	@see PDBookmarkAddChild 
	@see PDBookmarkAddNewChildASText
	@see PDBookmarkAddNewChild
	@see PDBookmarkAddNext 
	@see PDBookmarkAddPrev 
	@see PDBookmarkAddSubtreeASText 
	@see PDBookmarkAddSubtree 
	@see PDBookmarkUnlink 
	@since PI_PDMODEL_VERSION >= 0x00080000
*/
NPROC(PDBookmark,PDBookmarkAddNewSiblingASText, (PDBookmark aBookmark, const ASText initialText))

/**
	Adds a new bookmark to the tree containing <code>aBookmark</code>, as 
	the new last child of aBookmark. If <code>aBookmark</code> previously 
	had no children, it will be open after the child is added. 
	
	@param aBookmark The bookmark to which a new last child 
	is added. 
	@param initialText The text object containing the new bookmark's title.
	@return The newly created bookmark. 

	@notify PDBookmarkDidChangePosition 
	@notify PDBookmarkWasCreated 

	@see PDBookmarkAddNewChild
	@see PDBookmarkAddNewSiblingASText 
	@see PDBookmarkAddNewSibling 
	@see PDBookmarkAddSubtreeASText 
	@see PDBookmarkAddSubtree 
	@see PDBookmarkAddPrev 
	@see PDBookmarkAddNext 
	@see PDBookmarkAddChild 
	@see PDBookmarkUnlink 
	@since PI_PDMODEL_VERSION >= 0x00080000
*/
NPROC(PDBookmark,PDBookmarkAddNewChildASText, (PDBookmark aBookmark, const ASText initialText))

/**
	Adds a copy of the bookmark subtree source to <code>aBookmark</code>, 
	as a new last child of <code>aBookmark</code>. This new item will have 
	the text value <code>sourceTitle</code>, will be open, and will have 
	no destination attribute. <code>source</code> must have been previously 
	unlinked. If <code>aBookmark</code> previously had no children, it will 
	be open after the subtree is added. 

	@param aBookmark The bookmark to which the subtree source 
	will be added as a new last child. 
	@param source The bookmark subtree to add. 
	@param sourceTitle The text object containing the new bookmark's title.

	@notify PDBookmarkWillChange 
	@notify PDBookmarkDidChange 
	@notify PDBookmarkDidChangePosition 
    @see PDBookmarkAddSubtree
	@see PDBookmarkAddNewSiblingASText 
	@see PDBookmarkAddNewSibling 
	@see PDBookmarkAddNewChildASText 
	@see PDBookmarkAddNewChild 
	@see PDBookmarkAddPrev 
	@see PDBookmarkAddNext 
	@see PDBookmarkAddChild 
	@see PDBookmarkUnlink 
	@since PI_PDMODEL_VERSION >= 0x00080000
*/
NPROC(void,PDBookmarkAddSubtreeASText, (PDBookmark aBookmark, PDBookmark source, const ASText sourceTitle))

/**
	Gets the first bookmark whose title is set in passed the ASText object.

	@param aBookmark The root of the bookmark subtree to search. 	
	@param title The text object containing the title value to search for. 
	@param maxDepth The number of subtree levels to search, 
	not counting the root level.

	<TABLE rules="all" cellspacing="1">
	<TR><TH>Value</TH><TH>Description</TH></TR>
	<TR><TD><code>0</code></TD><TD>Only look at <code>aBookmark</code>, not at any of its children.</TD></TR>
	<TR><TD><code>1</code></TD><TD>Check <code>aBookmark</code> and its children, but not any grandchildren or great grandchildren, and so on.</TD></TR>
	<TR><TD><code>-1</code></TD><TD>Check the entire subtree.</TD></TR>
	</TABLE>

	@return The bookmark with the specified title, or a <code>NULL</code> Cos object 
	if there is no such bookmark.

    @see PDBookmarkGetByTitle   
	@since PI_PDMODEL_VERSION >= 0x00080000
*/
NPROC(PDBookmark,PDBookmarkGetByTitleASText, (PDBookmark aBookmark, const ASText title,ASInt32 maxDepth))

/**
	Gets the specified article thread's info as an ASText object. 

	@param thread The thread whose thread info is being obtained. 	
	@param infoKey The key whose value is being obtained. 
	@param value (Filled by the method) The text object containing the value associated 
	with <code>infoKey</code> is set. The client must pass a valid ASText object value.
	The routine does not allocate it.
	
	@see PDThreadGetInfo
	@see PDThreadSetInfoASText 
	@see PDThreadSetInfo 
	@since PI_PDMODEL_VERSION >= 0x00080000
	
*/
NPROC(void,PDThreadGetInfoASText, (PDThread thread, const ASText infoKey, ASText value))

/**
	Sets the specified article thread's info. 

	@param thread The thread whose thread info is being set. 
	@param infoKey The key whose value is being set. 
	@param value The text object containing the value to set. 
	
	@see PDThreadSetInfo 
	@see PDThreadGetInfoASText 
	@see PDThreadGetInfo 
	@since PI_PDMODEL_VERSION >= 0x00080000
	
*/
NPROC(void,PDThreadSetInfoASText, (PDThread thread, const ASText infoKey, const ASText value))

/**
	Returns a host encoded string corresponding to an ASText object. 
	
	@param inPdfString The text object. 
	@param outHostString (Filled by the method) A pointer to the 
	translated string. 
	@param outHostStringSize The length of the <code>outHostString</code> buffer, 
	in bytes. 
	@return The number of bytes in the translated string <code>outHostString</code>. 
	
	@see PDXlateToHostEx
	@see PDFontXlateToHost 
	@see PDFontXlateToUCS 
	@see PDGetHostEncoding 
	@see PDXlateToHost 
	@see PDXlateToPDFDocEnc 
	@see PDXlateToPDFDocEncEx 
	@since PI_PDMODEL_VERSION >= 0x00080000	
*/
NPROC(ASInt32,PDXlateToHostASText, (const ASText inPdfString,char* outHostString, ASInt32 outHostStringSize ))

/**
	Returns an ASText object corresponding to a host encoded string.
 
	@param inHostString A pointer to the string to translate (it may 
	point to the same memory as <code>outPDFString</code>, allowing strings 
	to translate in place). 
	@param inHostStringSize The number of bytes in the string to 
	translate. 
	@param outPDFString (Filled by the method) The text object corresponding to <code>inHostStringSize</code>.
	The client must pass a valid ASText object title. The routine does not allocate it.
	
	@return The number of bytes in the translated string <code>outPDFString</code>. 
	@see PDFontXlateToHost 
	@see PDFontXlateToUCS 
	@see PDGetHostEncoding 
	@see PDXlateToHost 
	@see PDXlateToHostEx 
	@see PDXlateToPDFDocEnc 
	@since PI_PDMODEL_VERSION >= 0x00080000
*/
NPROC(void, PDXlateToASText, (const char *inHostString, ASInt32 inHostStringSize, ASText outPDFString))

/**
	Constructs a new label object in the document with the specified 
	style, prefix, and starting page number. 
	
	@param pdDoc The document that contains the new page label. 
	@param style The numbering system to use for the numeric 
	portion of each label in this range of pages. The possible values are:

	<TABLE rules="all" cellspacing="1">
	<TR><TH>Value</TH><TH>Description</TH></TR>
	<TR><TD><code>D</code></TD><TD>Decimal numbers.</TD></TR>
	<TR><TD><code>R</code></TD><TD>Upper-case Roman numbers.</TD></TR>
	<TR><TD><code>r</code></TD><TD>Lower-case Roman numbers.</TD></TR>
	<TR><TD><code>A</code></TD><TD>Upper-case alphabetic numbers.</TD></TR>
	<TR><TD><code>a</code></TD><TD>Lower-case alphabetic numbers.</TD></TR>
	</TABLE>

	<p>If it is <code>NULL</code>, the labels for this range will not have a numeric portion. </p>

	@param prefix The text object containing the string to prefix to the numeric portion 
	of the page label. 
	@param startAt The value to use when generating the numeric 
	portion of the first label in this range; it must be greater 
	than or equal to <code>1</code>.
	@return The newly created page label. 

	@exception pdErrBadBaseObj is raised if the base pages object is missing 
	or invalid. 
    @see PDPageLabelNew
	@see PDDocRemovePageLabel 
	@see PDDocGetPageLabel 
	@see PDDocFindPageNumForLabel 
	@see PDDocGetLabelForPageNum 
	@see PDDocSetPageLabel 
	@since PI_PDMODEL_VERSION >= 0x00080000
*/
NPROC(PDPageLabel, PDPageLabelNewASText, (PDDoc	pdDoc, ASAtom style, const ASText prefix, ASInt32 startAt))

/**
	Returns the prefix string for the label as an ASText object. The prefix string 
	is transitory and should be copied immediately. 

	@param pgLabel The label for the page whose prefix is desired. 
	@param prefix (Filled by the method) The text object containing the prefix string.
	The client must pass a valid ASText object title. The routine does not allocate it.

	@see PDPageLabelGetPrefix
	@see PDPageLabelGetStart 
	@see PDPageLabelGetStyle 
	@since PI_PDMODEL_VERSION >= 0x00080000
*/
NPROC(void , PDPageLabelGetPrefixASText, (PDPageLabel pgLabel, ASText prefix))


#if !defined (_H_PDFLPrivCalls)
 /**
 	Gets an ASStm from a thumbnail data. 
 	@param thumb The thumb for which image data is to be retrieved.
 	@param height (Filled by the method) The height of the thumbnail.
 	@param width (Filled by the method) The width of the thumbnail.
 	@param bpc (Filled by the method) The number of bits per component in the thumbnail image's data.
 	@param csName (Filled by the method) The color space in which thumnail data is represented.
 	@return An ASStm for the thumbnail data.
 	@see PDThumbGetIndexedColorSpace
 	@see PDThumbCreationDrawThumbProc
 */
 UNPROC(ASStm, PDThumbGetImageData, (
 		PDThumb		thumb,
 		ASInt32		*height,
 		ASInt32		*width,
 		ASInt32		*bpc,
 		ASAtom		*csName))
 
/**
 	Gets an ASStm from a thumbnail's indexed color space table. 
 	@param thumb The thumb for which image data is to be retrieved.
 	@param  hival (Filled by the method) The highest valid index in the lookup table for the Indexed color space.
 	@param baseColorSpaceName (Filled by the method) The base color space in which the values in the color table are to be interpreted.
 	@return An ASStm from the thumbnail's indexed color space table.
 	@see PDThumbGetImageData
 	@see PDThumbCreationDrawThumbProc
 */
 UNPROC(ASStm, PDThumbGetIndexedColorSpace, (
 		PDThumb		thumb,
 		ASInt32		*hival,
 		ASAtom		*baseColorSpaceName))
#else
NOPROC(PDThumbGetImageData_InPDProcs) // Fix 1251180
NOPROC(PDThumbGetIndexedColorSpace_InPDProcs)
#endif

/**
	Checks whether a page contains overprint (with qualifications). 

	@param pdPage The page to check. 
	@return <code>true</code> only if the page contains overprint. 
	@since PI_PDMODEL_VERSION >= 0x00070000
*/
NPROC(ASBool, PDPageHasOverprintExt, (PDPage pdPage))

/**
	Sets the PDF minor version to the greater of its current value and the requested value.
	This function should be called when any feature requiring a PDF version of 1.7 or higher is applied to a document.

	@param pdDoc The document.
	@param minor The minimum required minor version
	@since PI_PDMODEL_VERSION >= 0x00080000
*/
NPROC(void, PDDocSetMinorVersion, (PDDoc pdDoc, ASInt16 minor))

/**
	Sets the Output Intent flag. 
	@param flag When <code>true</code>, use Output Intent to override a 
	working space if it is present. 
	@see PDPrefGetUseOutputIntents 
*/
NPROC(void, PDPrefSetUseOutputIntents , (ASBool flag))


/**
	Returns the value of the Output Intent flag. When this flag 
	is <code>true</code>, the system overrides the working space with the Output 
	Intent, if it is present. 
	@return The Output Intent flag value. 
	@see PDPrefSetUseOutputIntents
*/
NPROC(ASBool, PDPrefGetUseOutputIntents, (void))

/**
	Sets the black-point compensation flag, which controls whether 
	to adjust for differences in black points when converting 
	colors between color spaces. When enabled, the full dynamic 
	range of the source space is mapped into the full dynamic 
	range of the destination space. When disabled, the dynamic 
	range of the source space is simulated in the destination 
	space (which can result in blocked or gray shadows). 
	@param kbpc <code>true</code> to enable black-point compensation, <code>false</code> 
	otherwise.
	@see PDPrefGetBlackPointCompensation 
*/
NPROC(void, PDPrefSetBlackPointCompensation, (ASBool kbpc))

/**
	Returns the black-point compensation flag. 
	@return <code>true</code> if black-point compensation is done. 
	@see PDPrefSetBlackPointCompensation
*/
NPROC(ASBool, PDPrefGetBlackPointCompensation, (void))

/**
	Set the current RGB working space to a given ICC profile. 
	An RGB working space in PDF is defined as a profile to substitute 
	for a corresponding <code>/DeviceRGB</code> space. 
	@param profile A pointer to a buffer containing the ICC 
	color profile. 
	@param profileLength The length in bytes of the profile.
	@see PDPrefSetWorkingCMYK 
	@see PDPrefSetWorkingGray 
*/
NPROC(void, PDPrefSetWorkingRGB, (void *profile, ASUns32 profileLength)) 

/**
	Sets the current CMYK working space to a given ICC profile. 
	A CMYK working space in PDF is defined as a profile to substitute 
	for a corresponding <code>/DeviceCMYK</code> space. 
	@param profile A pointer to a buffer containing the ICC 
	color profile. 
	@param profileLength The length in bytes of the profile.
	@see PDPrefSetWorkingGray 
	@see PDPrefSetWorkingRGB 
*/
NPROC(void, PDPrefSetWorkingCMYK, (void *profile, ASUns32 profileLength))

/**
	Sets the current gray working space to a given ICC profile. 
	A Gray working space in PDF is defined as a profile to substitute 
	for a corresponding <code>/DeviceGray</code> space. When rendering with 
	overprint preview, the gray substitution is suppressed, 
	to avoid converting grayscale to <i>rich black</i>. 
	@param profile A pointer to a buffer containing the ICC 
	color profile. 
	@param profileLength The length in bytes of the profile.
	@see PDPrefSetWorkingCMYK 
	@see PDPrefSetWorkingRGB 
*/
NPROC(void, PDPrefSetWorkingGray, (void *profile, ASUns32 profileLength))

/**
	Applies a set of redaction marks to the document, permanently removing the affected
	document content and the marks themselves.
	@param pdDoc IN/OUT The document to which the redaction marks should be applied.
	@param applyParams IN/OUT A pointer to a <code>PDApplyRedactionParams</code> specifying which
	redaction marks to apply and what parameters to use when applying them. If <code>NULL</code>,
	then all redaction marks present in the document will be applied.
	@return <code>true</code> if the document's content was changed, <code>false</code> otherwise.
	@exception pdErrBadAnnotation is raised if any specified redaction marks are invalid
	@see PDDocCreateRedaction 
	@see PDRedactionGetProps 
	@see PDRedactionSetProps 
*/
XNPROC(ASBool, PDDocApplyRedactions, (PDDoc pdDoc, PDApplyRedactionParams applyParams))

/**
	Creates a redaction mark on a given page. The resulting annotation will be added to the page,
	but the affected content will not be removed until <code>PDDocApplyRedactions</code> is
	called with this mark.
	@param pdDoc IN/OUT The document for which the new redaction mark should be created.
	@param redactionProps IN A set of properties to be used for the new redaction mark.
	@return The new annotation representing the redaction mark.
	@exception genErrBadParm is raised if <code>redactionProps</code> is <code>NULL</code> or if contains an
	invalid page number or an empty list of quads.
	@see PDDocApplyRedactions 
	@see PDRedactionGetProps 
	@see PDRedactionSetProps 
*/
NPROC(PDAnnot, PDDocCreateRedaction, (PDDoc pdDoc, PDRedactParams redactionProps))

/**
	Retrieves a set of properties for a given redaction mark.
	@param redactionAnnot IN The redaction mark whose properties are to be returned.
	@param redactionProps OUT The set of properties to be filled by this method.
	@return <code>true</code> if the properties were successfully returned, <code>false</code> otherwise.
	@exception pdErrBadAnnotation
	@see PDDocApplyRedactions 
	@see PDDocCreateRedaction 
	@see PDRedactionSetProps 
*/
NPROC(ASBool, PDRedactionGetProps, (PDAnnot redactionAnnot, PDRedactParams redactionProps))

/**
	Assigns a set of properties to a given redaction mark.
	@param redactionAnnot IN/OUT The redaction mark whose properties are to be assigned.
	@param redactionProps IN The set of properties to be assigned by this method.
	@return <code>true</code> if the properties were successfully assigned, <code>false</code> otherwise.
	@exception pdErrBadAnnotation
	@see PDDocApplyRedactions 
	@see PDDocCreateRedaction 
	@see PDRedactionGetProps 
*/
NPROC(ASBool, PDRedactionSetProps, (PDAnnot redactionAnnot, PDRedactParams redactionProps))

/**
	Resets the cached ink (spot color) usage information in a document. This should be called
	when the set of non-process colorants for a document have been changed. Calling this will
	force the cached information to be recomputed.
	@param doc IN The document on which to reset set the ink usage.
*/
XNPROC(void, PDDocResetInkUsage, (PDDoc doc))

/**
	Return the number of non-fatal errors encountered since the document was opened, or <code>PDDocClearErrors</code> was called.
	@param doc The document in which the non-fatal errors have occurred.
	@since PI_PDMODEL_VERSION >= 0x00090000
*/	

NPROC(ASInt32, PDDocGetNumErrors, (PDDoc doc))

/**
	Returns the error code and string for the nth non-fatal error encountered since the document was opened, or <code>PDDocClearErrors</code> was called.
	@param doc The document in which the error has occurred.
	@param errNumber This is the serial number of the non-fatal error to be returned.
	@param errorP The error code. Use <code>ASGetErrorString()</code> to get the error message.
	@param buffer If there is a string associated with this error, the string is copied to this buffer if it is non-<code>NULL</code> and is <code>NULL</code>-terminated.
	@param bufSize The maximum number of bytes that will be written to the buffer.
	@return If there is a string associated with this error, the length of that string is returned. If there is no string, the return value is zero.
	@since PI_PDMODEL_VERSION >= 0x00090000
*/	

NPROC(ASInt32, PDDocGetNthError, (PDDoc doc, ASInt32 errNumber, ASInt32 *errorP, char *buffer, ASInt32 bufSize))

/**

	Returns the Adobe version of the PDF format to which the PDF file conforms. For PDF versions
	1.0 through 1.7, this method will return a major version of 1, a minor version in the
	range of 0 through 7, and an <code>adbeExtensionLevel</code> of 0.
	For Acrobat 9, this method will return a major version of 1 and a minor version of 8.
 	For Acrobat 10, this method will return a major version of 1 and a minor version of 9.
	Starting with Acrobat 9, Adobe extensions to the PDF format will be identified via
	the Extensions dictionary in the catalog; in this case, the major and minor versions will
	be returned, and the Adobe extension level will be returned via the last argument.

	@param doc IN/OUT The document whose version is obtained. 
	@param majorP IN/OUT (Filled by the method) The major version number. 
	@param minorP IN/OUT (Filled by the method) The minor version number.
	@param adbeExtensionBaseP IN/OUT (Filled in by the method) The value of the BaseVersion  
	entry in the ADBE dictionary. If there is no Extensions dictionary or no ADBE sub-dictionary, the value returned will 
	be a null CosObj.
	@param adbeExtensionLevelP IN/OUT (Filled in by the method) The values of the ExtensionLevel
	entry in the ADBE dictionary. 
	If there is no Extensions dictionary or no ADBE sub-dictionary, the value returned will 
	be zero.
	@since PI_PDMODEL_VERSION >= 0x00090000
*/

NPROC(void, PDDocGetVersionEx, (PDDoc doc, ASUns32 *majorP, ASUns32 *minorP, CosObj *adbeExtensionBaseP, ASUns32 *adbeExtensionLevelP))

/**

	Returns true if the document contains an Extensions dictionary as defined by ISO 32000 for
	specifying the inclusion of features beyond the ISO 32000 specification.
	Starting with Acrobat 9, Adobe extensions to the PDF format will be identified via
	this Extensions dictionary in the catalog.

	@param doc IN/OUT The document tested for ISO extensions. 
	@since PI_PDMODEL_VERSION >= 0x00090000
*/

NPROC(ASBool, PDDocHasISOExtensions, (PDDoc doc))



/************************************************************************************\
|*																					*|
|*									PDFileAttachment								*|
|*																					*|
\************************************************************************************/

/** Tests a file attachment for validity.
    @param attachment The file attachment.
    @return <code>true</code> if the file attachment is a dictionary, <code>false</code> otherwise.
*/
NPROC(ASBool, PDFileAttachmentIsValid, (PDFileAttachment attachment))

/**
   <p>Creates a new file attachment from the given file.
   The resulting file specification dictionary is created for the given document,
   but is not referenced. The client must reference the resulting file specification
   dictionary by attaching it to another object in the PDF file, such as an annotation
   or name tree. An exception is raised if the file could not be read or the attachment stream could not be created.</p>

   <p>Note that permissions must be checked by the caller before invoking this function. </p>

   <p>For example, to have an attachment flate compressed and then ASCII base-85 encoded:</p>

   <pre>
        ASAtom filterNames[2];
        filterNames[0] = ASAtomFromString("ASCII85Decode");
        filterNames[1] = ASAtomFromString("FlateDecode");
        PDFileAttachmentNewFromFile(parentDoc, sourceFile, filterNames, 2,
                              CosNewNull(), NULL, NULL, NULL);
   </pre>

   @param parentDoc The <code>CosDoc</code> in which the file attachment dictionary will be created.
   @param sourceFile The <code>ASFile</code> from which to create the file attachment.
   @param filterNames An array of filters to apply to the file attachment stream. Filters
                      are indentified by name, in accordance with section 3.3 of the
                      <i>PDF Reference</i>.
   @param numFilters The number of elements in <code>filterNames</code>.
   @param filterParams The filter parameters, represented as a <code>CosArray</code> or <code>CosNull</code>.
                       When the array form is used, the array must contain the same
                       number of elements as <code>filterNames</code>. Each element in the <code>CosArray</code>
                       is a <code>CosDict</code> representing the filter parameters for the corresponding
                       filter in the <code>filterNames</code> array, or <code>CosNull</code> to indicate that
                       default parameters should be used for that filter.
                       When the <code>CosNull</code> form is used for <code>filterParams</code>, default parameters
                       are used for every filter in the filterNames array.
   @param monitor The <code>ASProgressMonitor</code> to use for the duration of the call. The monitor
                  object is owned by the caller. <code>NULL</code> indicates that no progress updates
                  are needed.
   @param monitorText The text for the monitor to display, or <code>NULL</code> if no text is needed.
                      The text object is owned by the caller.
   @param monitorData Opaque data that is specific to the monitor object.
   @return The new file attachment.

*/
NPROC(PDFileAttachment, PDFileAttachmentNewFromFile, (CosDoc parentDoc, ASFile sourceFile,
   const ASAtom* filterNames, const ASArraySize numFilters, CosObj filterParams,
   ASProgressMonitor monitor, ASConstText monitorText, void* monitorData))

/**
   <p>Updates a file attachment from the given file.
   The attachment uses the filters specified in the attachment to encode the data.
   An exception is raised if the file could not be read or the attachment stream could not be updated.</p>

   <p>Note that permissions must be checked by the caller before invoking this function. </p>

   @param attachment The file attachment.
   @param sourceFile The file to use as input for the update operation.
   @param monitor The <code>ASProgressMonitor</code> to use for the duration of the call. The monitor
                  object is owned by the caller. <code>NULL</code> indicates that no progress updates
                  are needed.
   @param monitorText The text for the monitor to display, or <code>NULL</code> if no text is needed.
                      The text object is owned by the caller.
   @param monitorData Opaque data that is specific to the monitor object.

*/
NPROC(void, PDFileAttachmentUpdateFromFile,(PDFileAttachment attachment, ASFile sourceFile,
   ASProgressMonitor monitor, ASConstText monitorText, void* monitorData))

/**
   Copies the data embedded in the file attachment to the specified file.
   The file must be open for write or append.  The caller is responsible for closing the
   file after this call returns.  If an error is encountered during the write, some data may
   have been written to the destination file.  This call will make no attempt at restoring
   the file after failure. An exception is raised if the file attachment has no embedded stream or if a
   file write error occurs.

   @param attachment The file attachment.
   @param destFile The file that will be written with the file attachment data.
*/
NPROC(void, PDFileAttachmentSaveToFile, (PDFileAttachment attachment, ASFile destFile))

/** Converts a file specification dictionary to a <code>PDFileAttachment</code> object.
    An exception is raised if the parameter is not a file specification dictionary.
    @param attachment The <code>CosObj</code> to convert.
    @return The file attachment object.
*/
NPROC(PDFileAttachment, PDFileAttachmentFromCosObj, (CosObj cosAttachment))

/** Returns a <code>CosObj</code> representing the file specification dictionary of the file attachment.
    @param attachment The file attachment object.
    @return The <code>CosObj</code> representation of the file attachment.
*/
NPROC(CosObj, PDFileAttachmentGetCosObj, (PDFileAttachment attachment))

/** Returns a stream for reading the data from an existing file attachment.
    An exception is raised if the file attachment does not have a stream (it is not embedded) or the stream
    could not be opened.  The caller is responsible for closing the returned stream.
    @param attachment The file attachment object.
    @return The file attachment stream.
*/
NPROC(ASStm, PDFileAttachmentOpenStream, (PDFileAttachment attachment))

/** Returns the size, in bytes, that the file will occupy if exported to disk.
    
    @param attachment The file attachment object
    @return The size of the file attachment. It returns <code>0</code> if the 
	<code>PDFileAttachment</code> object does not specify a file size and one cannot be
    determined.
*/
NPROC(ASUns32, PDFileAttachmentGetFileSize, (PDFileAttachment attachment))

/** Gets the creation date of the file attachment.
    @param attachment The file attachment object.
    @param date A pointer to a date that will receive the creation date.
    @return <code>true</code> if the file attachment has a creation date,
	<code>false</code> otherwise. A creation date may be absent for various reasons.
	For example, the file attachment may have originated from a file system that does
	not provide creation date information, such as Unix.
*/
NPROC(ASBool, PDFileAttachmentGetCreationDate, (PDFileAttachment attachment, ASTimeRec *date))

/** Gets the modification date of the file attachment.
    @param attachment The file attachment object.
    @param date A pointer to a date that will receive the modification date.
    @return <code>true</code> if the file attachment has a modification date, <code>false</code> otherwise.
*/
NPROC(ASBool, PDFileAttachmentGetModDate, (PDFileAttachment attachment, ASTimeRec *date))

/** Gets the file name of the file attachment.
    @param attachment The file attachment.
    @return An <code>ASText</code> copy of the file name of the file attachment.
*/
NPROC(ASText, PDFileAttachmentGetFileName, (PDFileAttachment attachment))

/** Sets the specified text field in the file attachment.
    @param attachment The file attachment.
    @param fieldID The field identifier.
    @param text The text to use as the new value for the specified field.
	@exception genErrBadParm is raised if the field does not exist in the collection schema
	or the field type is not <code>S</code> (text).
*/
NPROC(void, PDFileAttachmentSetFieldText, (PDFileAttachment attachment, ASAtom fieldID, ASText text))

/** Sets the specified text field in the file attachment using styled text.
    @param attachment The file attachment.
    @param fieldID The field identifier.
    @param text Text Layout Format XML containing the text to use as the new value for the specified field. 
	@exception genErrBadParm is raised if the field does not exist in the collection schema
	or the field type is not <code>S</code> (text).
*/
NPROC(void, PDFileAttachmentSetFieldStyledText, (PDFileAttachment attachment, ASAtom fieldID, ASConstText text))

/** Gets the value of the specified text field in the file attachment.
    @param attachment The file attachment.
    @param fieldID The field identifier.
    @param text The text object that will receive the field value.
    @return <code>true</code> if the field value was found, <code>false</code> otherwise.
*/
NPROC(ASBool, PDFileAttachmentGetFieldText, (PDFileAttachment attachment, ASAtom fieldID, ASText text))

/** Gets the value of the specified text field in the file attachment as styled text, in XML Text Layout Format.
    @param attachment The file attachment.
    @param fieldID The field identifier.
    @param text The text object that will receive the field value.
    @return <code>true</code> if the field value was found, <code>false</code> otherwise.
*/
NPROC(ASBool, PDFileAttachmentGetFieldStyledText, (PDFileAttachment attachment, ASAtom fieldID, ASText text))

/** Sets the specified text field in the file attachment using styled text.
    @param attachment The file attachment.
    @param fieldID The field identifier.
    @param styles ASConstCab containing field styles for the specified field. 
	@exception genErrBadParm is raised if the field does not exist in the collection schema
	or the field type is not <code>S</code> (text).
*/
NPROC(void, PDFileAttachmentSetFieldStyle, (PDFileAttachment attachment, ASAtom fieldID, ASConstCab styles))

/** Gets the value of the specified text field in the file attachment.
    @param attachment The file attachment.
    @param fieldID The field identifier.
    @param styles ASCab object that will receive the field styles
    @return <code>true</code> if the field value was found, <code>false</code> otherwise.
*/
NPROC(ASBool, PDFileAttachmentGetFieldStyle, (PDFileAttachment attachment, ASAtom fieldID, ASCab styles))

/** Sets the specified numeric field in the file attachment.
    @param attachment The file attachment.
    @param fieldID The field identifier.
    @param number The number to use as the new value for the specified field.
	@exception genErrBadParm is raised if the field does not exist in the collection schema
	or the field type is not <code>N</code> (number).
*/
NPROC(void, PDFileAttachmentSetFieldNumber, (PDFileAttachment attachment, ASAtom fieldID, float number))

/** Gets the value of the specified numeric field in the file attachment.
    @param attachment The file attachment.
    @param fieldID The field identifier.
    @param number The number that will receive the field value.
    @return <code>true</code> if the field value was found, <code>false</code> otherwise.
*/
NPROC(ASBool, PDFileAttachmentGetFieldNumber, (PDFileAttachment attachment, ASAtom fieldID, float *number))

/** Sets the specified date field in the file attachment.
    @param attachment The file attachment.
    @param fieldID The field identifier.
    @param date The date to use as the new value for the specified field.
	@exception genErrBadParm is raised if the field does not exist in the collection schema
	or the field type is not <code>D</code> (date).
*/
NPROC(void, PDFileAttachmentSetFieldDate, (PDFileAttachment attachment, ASAtom fieldID, const ASTimeRec *date))

/** Gets the value of the specified date field in the file attachment.
    @param attachment The file attachment.
    @param fieldID The field identifier.
    @param date The date that will receive the field value.
    @return <code>true</code> if the field value was found, <code>false</code> otherwise.
*/
NPROC(ASBool, PDFileAttachmentGetFieldDate, (PDFileAttachment attachment, ASAtom fieldID, ASTimeRec *date))

/** Sets the specified prefix field in the file attachment. The prefix allows additional
    text to be prepended to the visual appearance of a field without affecting its actual value.
    @param attachment The file attachment.
    @param fieldName The field identifier.
    @param text The prefix to use as the new value for the specified field. Note that if a <code>NULL</code> value
	is passed into this parameter, the prefix is removed and an exception will not be thrown.
	@exception genErrBadParm is raised if the field does not exist in the collection schema.
*/
NPROC(void, PDFileAttachmentSetFieldPrefix, (PDFileAttachment attachment, ASAtom fieldName, ASText text))

/** Gets the specified prefix field in the file attachment.
    @param attachment The file attachment.
    @param fieldID The field identifier.
    @param prefix The text object that will receive the prefix.
*/
NPROC(ASBool, PDFileAttachmentGetFieldPrefix, (PDFileAttachment attachment, ASAtom fieldName, ASText prefix))


/************************************************************************************\
|*																					*|
|*									PDCollection									*|
|*																					*|
\************************************************************************************/

/** Determines if a collection is valid.
    @param collection The collection
    @return <code>true</code> if the collection is valid, <code>false</code> otherwise.
*/
NPROC(ASBool, PDCollectionIsValid, (PDCollection collection))

/** Gets the collection object in a document.
    @param pdDoc The document.
    @return The collection. If the document does not have a collection, the returned
            collection is invalid.
*/
NPROC(PDCollection, PDDocGetPDCollection, (PDDoc pdDoc))

/** Creates a collection in a document. It replaces any existing collection.
    @param pdDoc The document that will host the new collection.
    @return The new collection object.
*/
NPROC(PDCollection, PDDocCreatePDCollection, (PDDoc pdDoc))

/** Removes a collection dictionary from a document.
    @param pdDoc The document whose collection dictionary is to be removed.
*/
NPROC(void, PDDocDeleteCollection, (PDDoc pdDoc))

/** Gets the contents of the collection sort dictionary.
    @param collection The collection object.
    @param pairs The array of pairs. It may be <code>NULL</code>.
    @arrayLen The length of the pairs array.
    @return If <code>pairs</code> is <code>NULL</code>, the number of items in the collection sort dictionary is returned;
            otherwise, the number of items stored in the <code>pairs</code> array is returned.
*/
NPROC(ASArraySize, PDCollectionGetSortOrder, (PDCollection collection, PDCollectionSchemaSortPairRec *pairs, ASArraySize arrayLen))

/** Set the contents of the collection sort dictionary.
    @param collection The collection object.
    @param pairs The array of pairs. If it is <code>NULL</code>, the collection sort dictionary is removed.
    @arrayLen The length of the <code>pairs</code> array.
*/
NPROC(void, PDCollectionSetSortOrder, (PDCollection collection, const PDCollectionSchemaSortPairRec *pairs, ASArraySize arrayLen))

/** Gets the view data for the collection.
    @param collection The collection object.
    @param data The collection view data.
*/
NPROC(void, PDCollectionGetViewData, (PDCollection collection, PDCollectionViewDataRec *data))

/** Set the view data for the collection.
    @param collection The collection object.
    @param data The collection view data.
*/
NPROC(void, PDCollectionSetViewData, (PDCollection collection, const PDCollectionViewDataRec *data))

/** Acquires the <code>PDCollectionSchema</code> object for a collection.
    @param collection The collection object.
    @return The collection object.
*/
NPROC(PDCollectionSchema, PDCollectionSchemaAcquire, (PDCollection collection))

/** Destroys a <code>PDCollectionSchema</code> object.
    @param collection The collection object.
    @return The collection object.
*/
NPROC(void, PDCollectionSchemaDestroy, (PDCollectionSchema schema))

/** Gets the number of fields in the schema.
    @param collection The collection object.
    @return The number of fields in the schema.
 */
NPROC(ASArraySize, PDCollectionSchemaGetLength, (PDCollectionSchema schema))

/** Gets a field by name or position.
    The caller must set <code>field.size</code> to <code>sizeof(PDCollectionFieldRec)</code>.
    To look up a field by name, set <code>field.fieldName</code> to the appropriate name.
    To look up a field by position, set <code>field.fieldName</code> to <code>ASAtomNull</code>, 
	and set <code>field.index</code> to the position.
    The caller owns (and must destroy) <code>field.fieldText</code> if it is not <code>NULL</code>.
    @param schema The collection schema object.
    @param field The field to be obtained.
    @return <code>true</code> if the field was found, <code>false</code> otherwise.
*/
NPROC(ASBool, PDCollectionSchemaGetField, (PDCollectionSchema schema, PDCollectionFieldRec *field))

/** Sets a field with new values.
    The target field is identified by the <code>field.fieldName</code> member. If the target field exists, it is
    overwritten; otherwise a new field is added.
    The caller must set <code>field.size</code> to <code>sizeof(PDCollectionFieldRec)</code>.
    Specifying a new value for <code>field.index</code> will affect other field values
    as necessary to maintain the correct overall ordering. See
    <code>PDCollectionSchema</code> for information about ordering.
    @param schema The collection schema object.
    @param field The field to add or modify in the collection schema.
	@see PDCollectionSchema
*/
NPROC(void, PDCollectionSchemaSetField, (PDCollectionSchema schema, const PDCollectionFieldRec *field))

/** Removes a field from the collection schema.
    @param schema The collection schema.
    @param fieldName The name of the field to remove from the collection schema.
*/
NPROC(void, PDCollectionSchemaRemoveField, (PDCollectionSchema schema, ASAtom fieldName))

/** Determines if a <code>PDFolder</code> is valid.
    @param folder The folder object
    @return <code>true</code> if the folder is a dictionary, <code>false</code> otherwise.
*/
NPROC(ASBool, PDFolderIsValid, (PDFolder folder))

/** Creates a new folder.
    @param collection The collection that will be associated with the new folder.
    @param path The path and name of the folder. The path syntax for folders takes the
                form <code>[parent/]folder</code>, where the mandatory <code>parent/</code> section may be repeated
                as necessary to provide a complete path to the new folder. The path
                is always interpreted as being relative to the root level of the folder
                hierarchy. Paths that specify simply a new folder name are located
                in the root folder. The character set for folder names is subject to
                the limitations imposed by the PDF Reference: folders may not contain
                any of the characters in the set <code>/ \ : ? * " &lt; &gt; |</code> and may not end with a <code>.</code> (period).
				Please note that a folder name cannot consist entirely of spaces.
				To specify the root folder itself, assign a value of <code>"/"</code>.
     @return The new folder object.
*/
NPROC(PDFolder, PDCollectionCreateFolder, (PDCollection collection, ASConstText path))

/** Removes a folder and its descendant folders and associated file attachments.
    @param collection The collection associated with the folder that will be removed.
    @param path The path to the folder.
    @return <code>0</code> if the folder hierarchy was successfully removed; otherwise an
              error code identifying the failure condition is returned.
*/
NPROC(void, PDCollectionRemoveFolder, (PDCollection collection, ASConstText path))

/** Gets an existing folder.
    @param collection The collection associated with the folder to be obtained.
    @param path The path to the folder. To specify the root folder itself, assign a value of <code>"/"</code>.
				The path syntax for folders takes the
				form <code>[parent/]folder</code>, where the mandatory <code>parent/</code> section may be repeated
				as necessary to provide a complete path to the new folder.
    @return The specified folder. If the folder does not exist, the returned folder is invalid.
 */
NPROC(PDFolder, PDCollectionGetFolder, (PDCollection collection, ASConstText path))


/** Sets the initial style dictionary for the collection.
    @param collection The collection object.
    @param style Dictionary to fill
*/
NPROC(void, PDCollectionSetInitialStyle, (PDCollection collection, ASConstCab style))

/** Gets the initial style dictionary for the collection.
    @param collection The collection object.
    @param style Dictionary to set
	@return true if the dictionary is present, if false the passed cabinet is unchanged
*/
NPROC(ASBool, PDCollectionGetInitialStyle, (PDCollection collection, ASCab style))

/** Removes the initial style dictionary from a collect, if present
	@param collection The collection object.
*/
NPROC(void,  PDCollectionRemoveInitialStyle, (PDCollection collection))

/** Gets the parent of the specified folder.
    @param folder The folder object.
    @return The parent of the specified folder. If no parent exists, the returned folder is invalid.
*/
NPROC(PDFolder, PDFolderGetParent, (PDFolder folder))

/** Sets the parent of the specified folder.
    @param folder The folder that will receive a new parent.
    @param parent The new parent folder.
*/
NPROC(void, PDFolderSetParent, (PDFolder folder, PDFolder parent))

/** Gets the first child of a folder.
    @param folder The folder.
    @return The first child of the folder. If no child exists, the returned folder is invalid.
*/
NPROC(PDFolder, PDFolderGetFirstChild, (PDFolder folder))

/** Gets the next sibling of a folder.
    @param folder The folder.
    @return The next sibling of the folder. If no next sibling exists, the returned folder is invalid.
*/
NPROC(PDFolder, PDFolderGetNextSibling, (PDFolder folder))

/** Sets the name of a folder.
    @param folder The folder.
    @param folderName The name of the folder.
*/
NPROC(void, PDFolderSetName, (PDFolder folder, ASConstText folderName))

/** Gets the name of a folder.
    @param folder The folder.
    @param folderName The name of the folder.
*/
NPROC(void, PDFolderGetName, (PDFolder folder, ASText name))

/** Gets the ID number of a folder.
    @param folder The folder.
    @return The folder ID.
*/
NPROC(ASInt32, PDFolderGetID, (PDFolder folder))

/** Gets the path of the folder.
    @param folder The folder.
    @param path The folder path.
*/
NPROC(void, PDFolderGetPathText, (PDFolder folder, ASText path))

/** Gets the modification date of the folder.
    @param folder The folder.
    @param date A pointer to an <code>ASTimeRec</code> that will be filled with the folder modification date.
    @return <code>true</code> if the folder has a modification date, <code>false</code> otherwise.
*/
NPROC(ASBool, PDFolderGetModDate, (PDFolder folder, ASTimeRec *date))

/** Sets the modification date of the folder.
    @param folder The folder.
    @param date A pointer to an <code>ASTimeRec</code> that will be used to set the modification date.
*/
NPROC(void, PDFolderSetModDate, (PDFolder folder, const ASTimeRec *date))

/** Gets the creation date of the folder.
    @param folder The folder.
    @param date A pointer to an <code>ASTimeRec</code> that will be filled with the folder creation date.
    @return <code>true</code> if the folder has a creation date, <code>false</code> otherwise.
*/
NPROC(ASBool, PDFolderGetCreationDate, (PDFolder folder, ASTimeRec *date))

/** Sets the creation date of the folder.
    @param folder The folder.
    @param date A pointer to an <code>ASTimeRec</code> will be used to set the creation date.
*/
NPROC(void, PDFolderSetCreationDate, (PDFolder folder, const ASTimeRec *date))

/** Gets the description of the folder. 
    @param folder The folder.
    @param text A text object that will receive the folder description.
    @return <code>true</code> if the folder has a description, <code>false</code> otherwise.
*/
NPROC(ASBool, PDFolderGetDescription, (PDFolder folder, ASText text))

/** Gets the description of the folder as styled text, in XML Text Layout Format.
    @param folder The folder.
    @param text A text object that will receive the folder description.
    @return <code>true</code> if the folder has a description, <code>false</code> otherwise.
*/
NPROC(ASBool, PDFolderGetDescriptionStyled, (PDFolder folder, ASText text))

/** Sets the description of the folder. Removes a styled version if present.
    @param folder The folder.
    @param text The new description for the folder.
*/
NPROC(void, PDFolderSetDescription, (PDFolder folder, ASConstText text))

/** Sets the description of the folder using styled text. Keeps the non-styled description in sync.
    with the styled version.
    @param folder The folder.
    @param text The new description for the folder in XML Text Layout Format.
*/
NPROC(void, PDFolderSetDescriptionStyled, (PDFolder folder, ASConstText text))

/** Gets the value of the specified text field in the folder.
    @param folder The folder.
    @param fieldID The field identifier.
    @param text The text object that will receive the field value.
    @return <code>true</code> if the field value was found, <code>false</code> otherwise.
*/
NPROC(ASBool, PDFolderGetFieldText, (PDFolder folder, ASAtom fieldID, ASText text))

/** Sets the specified text field in the folder.
    @param folder The folder.
    @param fieldID The field identifier.
    @param text The text to use as the new value for the specified field.
	@exception genErrBadParm is raised if the field does not exist in the collection schema
	or the field type is not <code>S</code> (text).
*/
NPROC(void, PDFolderSetFieldText, (PDFolder folder, ASAtom fieldID, ASConstText text))

/** Sets the specified text field in the folder
    @param folder The folder.
    @param fieldID The field identifier.
    @param text Text Layout Format XML containing the text to use as the new value for the specified field. 
	@exception genErrBadParm is raised if the field does not exist in the collection schema
	or the field type is not <code>S</code> (text).
*/
NPROC(void, PDFolderSetFieldStyledText, (PDFolder folder, ASAtom fieldID, ASConstText text))

/** Gets the value of the specified text field in the folder as styled text, in XML Text Layout Format.
    @param folder The folder.
    @param fieldID The field identifier.
    @param text The text object that will receive the field value.
    @return <code>true</code> if the field value was found, <code>false</code> otherwise.
*/
NPROC(ASBool, PDFolderGetFieldStyledText, (PDFolder attachment, ASAtom fieldID, ASText text))

/** Sets the style dictionary for the specified field in the folder. 
    @param folder The folder.
    @param fieldID The field identifier.
    @param styles ASConstCab containing field styles for the specified field. 
	@exception genErrBadParm is raised if the field does not exist in the collection schema
	or the field type is not <code>S</code> (text).
*/
NPROC(void, PDFolderSetFieldStyle, (PDFolder folder, ASAtom fieldID, ASConstCab styles))

/** Gets the style dictionary for the specified field in the folder
    @param folder The folder.
    @param fieldID The field identifier.
    @param styles ASCab object that will receive the field styles
    @return <code>true</code> if the field value was found, <code>false</code> otherwise.
*/
NPROC(ASBool, PDFolderGetFieldStyle, (PDFolder folder, ASAtom fieldID, ASCab styles))

/** Gets the value of the specified numeric field in the folder.
    @param folder The folder.
    @param fieldID The field identifier.
    @param number The number that will receive the field value.
    @return <code>true</code> if the field value was found, <code>false</code> otherwise.
*/
NPROC(ASBool, PDFolderGetFieldNumber, (PDFolder folder, ASAtom fieldID, float *number))

/** Sets the specified numeric field in the folder.
    @param folder The folder.
    @param fieldID The field identifier.
    @param number The number to use as the new value for the specified field.
	@exception genErrBadParm is raised if the field does not exist in the collection schema
	or the field type is not <code>N</code> (number).
*/
NPROC(void, PDFolderSetFieldNumber, (PDFolder folder, ASAtom fieldID, float number))

/** Gets the value of the specified date field in the folder.
    @param folder The folder.
    @param fieldID The field identifier.
    @param date The date that will receive the field value
    @return <code>true</code> if the field value was found, <code>false</code> otherwise.
*/
NPROC(ASBool, PDFolderGetFieldDate, (PDFolder folder, ASAtom fieldID, ASTimeRec *date))

/** Sets the specified date field in the folder.
    @param folder The folder.
    @param fieldID The field identifier.
    @param date The date to use as the new value for the specified field.
	@exception genErrBadParm is raised if the field does not exist in the collection schema
	or the field type is not <code>D</code> (date).
*/
NPROC(void, PDFolderSetFieldDate, (PDFolder folder, ASAtom fieldID, const ASTimeRec *date))



 /** Creates an <code>ASPathName</code> corresponding to the specified file or folder in the collection.
    @param pdDoc The document containing the specified file or folder.
    @param pathText The text-based representation of the path. The path consists of a file
                    name or sequence of file names. A file name may not contain any of the characters
                    <code>'\', '/', ':', '*', '?', '&lt;', '&gt;', '|'</code>, and may not contain <code>'.'</code> as the final character.
                    When <code>'/'</code> appears in a path, it signifies that the preceding file name is a folder, and
                    that the subsequent file name is a child of that folder. The root of a collection may
                    be identified by passing <code>"/"</code>, <code>NULL</code>, or an empty string for <code>pathText</code>.
    @param fileSys A pointer that will be filled by the function. This parameter provides
                   the caller with the <code>ASFileSys</code> to use in conjunction with the specified file or folder.
    @param pathName A pointer that will be filled by the function. This parameter provides
                   the caller with the <code>ASPathName</code> to use in conjunction with the specified file or folder.
*/
NPROC(void, ASFileAttachmentCreatePathName, (PDDoc pdDoc, ASText pathText, ASFileSys *fileSys, ASPathName *pathName))

/** Produces a <code>PDFileAttachment</code> corresponding to the <code>ASFileSys</code> and <code>ASPathName</code>. 
	It raises an exception if the <code>ASFileSys</code> is
    is not the embedded files file system, or if the <code>ASPathName</code> or <code>PDFileAttachment</code> parameters
    are <code>NULL</code>. 
    @param fileSys The <code>ASFileSys</code> for file attachments.
    @param pathName The <code>ASPathName</code> identifying the file attachment of interest.
    @param attachment A pointer that will be receive the file attachment.
    @return <code>true</code> if the file attachment was found, <code>false</code> otherwise.
*/
NPROC(ASBool, ASFileAttachmentGetPDFileAttachment, (ASFileSys fileSys, ASPathName pathName, PDFileAttachment *attachment))

/** Produces a <code>PDFolder</code> corresponding to the <code>ASFileSys</code> and <code>ASPathName</code>. 
	It raises an exception if the <code>ASFileSys</code> is
    is not the embedded files file system, or if the <code>ASPathName</code> or <code>PDFolder</code> parameters
    are <code>NULL</code>. 
    @param fileSys The <code>ASFileSys</code> for file attachments
    @param pathName The <code>ASPathName</code> identifying the folder of interest.
    @param attachment A pointer that will be receive the folder.
    @return <code>true</code> if the folder was found, <code>false</code> otherwise.
*/
NPROC(ASBool, ASFileAttachmentGetPDFolder, (ASFileSys fileSys, ASPathName pathName, PDFolder *folder))

/**
      Given a CosObj that represents a function, it applies the function 
      to the supplied values. 

      <p>It raises an error if the CosObj is malformed. </p>

      @param funcDict The CosObj representing a function. 
      @param inVals Input values. 
      @param nInput Number of input values.
      @param outVals Output values.
      @param nOutput Number of output values.
      @exception genErrBadParm is raised if the CosObj is not a function dictionary or the number of i/o do not match with that of the function.

      @since PI_PDMODEL_VERSION >= 0x00100000
*/


NPROC(void, PDApplyFunctionEx,
            (CosObj funcDict, const float inVals[], const ASArraySize nInput, float outVals[], const ASArraySize nOutput))

#ifndef ANDROID_ENV
/** Sets the <code>cryptVersion</code> param based on the Security handler of the document.  This is
	either retrieved directly from the Security handler or read from the encrypt dict.
    @param pdDoc The document.
	@return The Crypt Version retreived from the Security handler or encrypt dict or 0 if not found or not encrypted
	@since PI_PDMODEL_VERSION >= 0x000A0000
*/
NPROC(ASInt32, PDDocGetCryptVersion, (PDDoc pdDoc))

/** Sets the <code>cryptRevision</code> param based on the Security handler of the document. This is
	either retrieved directly from the Security handler or read from the encrypt dict.
    @param pdDoc The document.
	@return The Crypt Revision retreived from the Security handler or encrypt dict or 0 if not found or not encrypted
	@since PI_PDMODEL_VERSION >= 0x000A0000
*/
NPROC(ASInt32, PDDocGetCryptRevision, (PDDoc pdDoc))
#endif

/**
	Clears all the non-fatal errors encountered since the document was opened, or <code>PDDocClearErrors</code> was called.
	@param doc The document in which the non-fatal errors have occurred.
	@since PI_PDMODEL_VERSION >= 0x000A0000
*/	
NPROC(void, PDDocClearErrors, (PDDoc doc))

#if !MAC_PLATFORM || (MAC_PLATFORM && !AS_ARCH_64BIT)
/**
	Draws the contents of a page into the specified window. 
	
	<p>This method just draws a bitmap to the window. If you want 
	a live document, you need to open an AVDoc for the PDF file. </p>
	
	<p>The page can also be derived from a PDDoc. </p>

	<p>On UNIX, this method cannot be used to draw into a window. 
	UNIX developers should instead use AVDocOpenFromASFileWithParamString() 
	to draw PDF files into their own window from a client. </p>
	@param page The page to draw into <code>window</code>. 
	@param window A pointer to a platform-dependent window object 
	(<code>HWND</code> on Windows, or <code>WindowPtr</code>). 
	On Windows, to draw into an offscreen <code>DC</code>, pass <code>NULL</code> for <code>window</code>.
	@param displayContext A platform-dependent display context 
	structure (<code>HDC</code> on Windows). Note that 
	<code>displayContext</code> cannot be reliably used as the <code>hDC</code> for a 
	printer device. This API is not available on Mac 64. 
	This API will return error "Creation of a raster port failed." on Mac 32.
	@param isDPS Currently unused. Always set it to <code>false</code>. 
	@param matrix A pointer to the matrix to concatenate onto 
	the default page matrix. It is useful for converting from 
	page to window coordinates and for scaling.
	@param flags See PDPageDrawFlagsPI for possible values.
	@param updateRect A pointer to the rectangle to draw, defined 
	in user space coordinates. Any objects outside of <code>updateRect</code> 
	will not be drawn. All objects are drawn if <code>updateRect</code> is 
	<code>NULL</code>. 
	@param cancelProc The procedure called periodically to check 
	for the user's cancelling of the drawing operation. The default cancel 
	proc can be obtained using AVAppGetCancelProc(). It may be <code>NULL</code>, 
	in which case no cancel proc is used. 
	@param cancelProcClientData A pointer to user-supplied data 
	to pass to <code>cancelProc</code> each time it is called. It should be 
	<code>NULL</code> if <code>cancelProc</code> is <code>NULL</code>.
	@see AVAppGetCancelProc 
	@see PDDrawCosObjToWindow 
	@see PDPageDrawContentsToWindowEx 

	@note This method cannot be reliably used to print to a 
	device. 
	@since PI_PDMODEL_VERSION >= 0x00020000
*/
NPROC(void, PDPageDrawContentsToWindowEx2, (PDPage page,
						void *window, void *displayContext,
						ASBool isDPS,
						ASDoubleMatrix *matrix, 
						ASCab flags,
						ASDoubleRect *updateRect, 
						ASCancelProc cancelProc,
						void *cancelProcClientData))
#endif

/** Sets the default blending color space  to working CMYK or working RGB
	@param dBCSIndex An index to set blending color space. Pass 1 to set bcs as working RGB.
	Pass 0 or other value to set bcs as working CMYK.
*/
NPROC(void, PDPrefSetDefaultBlendingColorSpace, (ASInt32 dBCSIndex))

/** Get the default blending color space Index
	@return value equal to 0 means default blending colorspace is Working CMYK,
	value equal to 1 means default blending colorspace is Working RGB.
*/
NPROC(ASInt32, PDPrefGetDefaultBlendingColorSpace, (void))

/**
    PDDrawPageOrCosObjectToBuffer

	It is designed to provide a rich conversion path between a a CosObject (e.g.,
	a FormXObject) or a PDPage and its rendered equivalent. The user may provide an 
	output color profile. The client must also indicate the color space to work in.

	Renders a CosObj or PDPage to the buffer. 

	***********************************************************************
	IMPORTANT: The buffer is padded to be 32-bit aligned; the width of the 
			   image in samples should be calculated as follows:
	
	width = ceil(params->destRect:xMax) - floor(params->destRect:xMin));
	width = ((((width * bpc * nComps)+31) / 32) * 4) * 8 / (bpc * nComps);
		nComps = 1 for DeviceGray, 3 for DeviceRGB, 4 for DeviceCMYK
		bpc = params->bitsPerComponent, see PDRenderParams.
		ceil(x) = the smallest integer that is greater than or equal to x;
		floor(x) = the largest integer that is less than or equal to x;
		Examples:
			floor(2.8) = 2.000000, 	floor(-2.8) = -3.000000
			ceil(2.8) = 3.000000, 	ceil(-2.8) = -2.000000
	***********************************************************************

@param pageP
    The page to render; it should be NULL if a valid cosStreamObj is passed 
    as the second parameter
 
@param cosStreamObj
	The stream Cos Object to render, this parameter is used only if it is 
	not NULL CosObj. This stream can be created using CosNewStream. The 
	streams dictionary may contain a Resources dictionary specifying all 
	the resources needed to draw the Cos object. Its structure and contents 
	are the same as for the Resources dictionary for a Page object. 
    If pageP is non-NULL, this parameter should be CosNewNull()

@param csFamily
    The color space Family in which the bitmap data is represented.
    It must be one of DeviceGray, DeviceRGB, or DeviceCMYK. This parameter must be
    consistent with iccProfile.

@param iccProfile
    A Color Profile of the same family as CSFamily ASAtom. This is the destination
    profile the contents will be drawn to. If it is NULL, the working color space
    matching the csFamily will be used, unless the flags parameter below sets
	option to not substitute working spaces, in which case the uncalibrated 
	color space of csFamily type will be used.

@param renderIntent
    It is used only if an ICCProfile is not NULL. If renderIntent is not set, 
    default rendering intent is AC_RelColorimetric

@param bitsPerComponent
    The number of bits per color component in the bitmap data.
    8 is the only valid value for CMYK and RGB color space families.
    1, 8, and 24 are valid for Gray color space family
 
@param flags
    ASCab field used for passing drawing flags; All possible flags are listed in 
	PDExpT.h for in groups PDPageDrawFlagsStr and PDPageDrawSmoothsFlagsStr

@param greekTextLevel
    Size in points below which text is greeked

@param ocContext
    An optional-content context that determines what contents are visible. NULL uses
    the document's optional-content context, as returned by PDDocGetOCContext(pddoc).
    This context is copied and the copy is used in drawing. This allows a
    client to change its copy of the context without raising an exception

@param matrix
    Pointer to the matrix to be concatenated onto the default matrix to map from
    default user space to the resulting bitmap's image space. For example,
    if the page is to be rendered at 144 pixels/inch, the Matrix should be
    [2 0 0 2 0 0]. 
	If NULL, API will do nothing and return 0

@param destRect
    Pointer to the rectangle of the bitmap. Defined in device space coordinates.
    In the rectangle, destRect.left is xMin, destRect.right is xMax,
    destRect.bottom is yMin, and destRect.top is yMax.
	If NULL, API will do nothing and return 0

@param updateRect
    Pointer to the rectangle to draw, defined in user space coordinates. Any
    objects outside of updateRect will not be drawn. All objects are drawn
    if all entries in updateRect are 0.0.
    In the rectangle, updateRect.left is xMin, updateRect.right is xMax,
    updateRect.bottom is yMin, and updateRect.top is yMax.
	If NULL, API will do nothing and return 0

@param buffer
    Pointer to the bitmap data. If NULL, this function returns the size of buffer
    needed for the bitmap

@param bufferSize
    The size of the buffer

@param statusMonitor
    Only the cancelProc is in use for now.
	
	  statusMonitor->cancelProc 
		A method to call to check whether drawing should be canceled. If the method returns true, drawing 
		is stopped; nothing is erased, and the buffer contains whatever 
		was drawn up to the current state.

	  statusMonitor->cancelProcClientData 
		User-supplied data to pass to CancelProc each time it is called (about every 0.5 second.)

@return The size of the bitmap in bytes.  
@since PI_PDMODEL_VERSION >= 0x000C0000
*/
NPROC(ASUns32, PDDrawPageOrCosObjectToBuffer,
      (PDPage pdPage, CosObj cosStreamObj, ASAtom csFamily, AC_Profile iccProfile, AC_RenderIntent *renderIntent,
       ASInt32 bitsPerComponent, ASCab flags, ASInt32 greekTextLevel, PDOCContext ocContext, const ASRealMatrix* matrix,
       const ASRealRect* destRect, const ASRealRect* updateRect, char *buffer, ASUns32 bufferSize,
	   ASStatusMonitorProcs statusMonitor))

#ifdef ANDROID_ENV
/*Sets the temp location provided by Client*/
NPROC(void, PDSetTempLocation, (char* tempDir))

/*Gets the temp location provided by Client where temp files are being stored*/
NPROC(char*, PDGetTempLocation, (void))
#endif
	   
/**
	Creates a new action that takes the user to the specified
	destination view. 
 
	@param doc The document in which the action is created and used.
	@param dest The destination view.
	@param destDoc The destination document.
	@return The newly created action.
	@see PDActionNew
	@see PDActionNewFromFileSpec
	@see PDActionFromCosObj
	@see PDActionDestroy
 
	@since PI_PDMODEL_VERSION >= 0x000D0000
 */
NPROC(PDAction, PDActionNewFromStructureDest, (PDDoc doc, PDStructureDestination sDest, PDDoc destDoc))

/**
	Sets view destination on an action which may have been created from
 structure destination (or another view destination)
 
	@param action The action whose destination is set.
	@param dest The view destination to be set. If the action already has
 a view destination, the new one replaces the existing one.
	@see PDActionNewFromDest
 @see PDActionNewFromStructureDest
 
	@since PI_PDMODEL_VERSION >= 0x000D0000
 */
NPROC(void, PDActionSetViewDest, (PDAction action, PDViewDestination dest))

/**
	Sets structure destination on an action which may have been created from
 view destination (or another structure destination)
 
	@param action The action whose destination is set.
	@param dest The structure destination to be set. If the action already has
 a structure destination, the new one replaces the existing one.
	@see PDActionNewFromDest
 @see PDActionNewFromStructureDest
 
	@since PI_PDMODEL_VERSION >= 0x000D0000
 */
NPROC(void, PDActionSetStructureDest, (PDAction action, PDStructureDestination sDest))

/**
	Gets an action's structure destination. This only works for actions
	that contain a structure destination; that is, actions whose subtype
	is GoTo.
 
	@param action The action whose destination is obtained.
	@return The action's destination
	@see PDActionGetFileSpec
 
	@since PI_PDMODEL_VERSION >= 0x000D0000
 */
NPROC(PDStructureDestination, PDActionGetStructureDest, (PDAction action))

/**
	Creates a new structure destination object.
 
	@param doc The document in which the destination is used.
	@param structElem The structure element which will become target of the destination;
 it can be from the same document as the destination, or from a different document.
	@param initialFitType The destination fit type. It must be one
	of the View Destination Fit Types, which must be converted
	into an ASAtom with ASAtomFromString().
	@param initialRect A pointer to an ASFixedRect specifying
	the destination rectangle, specified in user space coordinates.
	The appropriate information will be extracted from <code>initialRect</code>,
	depending on <code>initialFitType</code>, to create the destination.
	All four of the <code>initialRect</code> parameter's components should be set; using
	PDViewDestNULL for any components can result in incorrect
	results for rotated pages.
	@param initialZoom The zoom factor to set for the destination.
	Used only if <code>initialFitType</code> is <code>XYZ</code>. Use the predefined value
	PDViewDestNULL (see PDExpT.h) to indicate a <code>NULL</code> zoom factor,
	described in Table 8.2 in the <i>PDF Reference</i>.
	@return The newly created structure destination.
	@see PDStructureDestFromCosObj
	@see PDStructureDestDestroy
 @ref ViewDestinationFitTypes
 
	@since PI_PDMODEL_VERSION >= 0x000D0000
 */
NPROC(PDStructureDestination, PDStructureDestCreate, (PDDoc doc, PDSElement structElem, ASAtom initialFitType, const ASFixedRectP initialRect, const ASFixed initialZoom))

/**
	Deletes a structure destination object. Before deleting a view
	destination, ensure that no link or bookmark refers to it.
	
	@param dest IN/OUT The structure destination to destroy.
	@see PDStructureDestFromCosObj
	@see PDStructureDestCreate
 
	@since PI_PDMODEL_VERSION >= 0x000D0000
 */
NPROC(void, PDStructureDestDestroy, (PDStructureDestination dest))

/**
	Tests whether a structure destination is valid. This is intended
	only to ensure that the structure destination has not been deleted,
	not to ensure that all necessary information is present
	and valid.
 
	@param dest The structure destination whose validity is determined.
	@return <code>true</code> if the structure destination is valid, <code>false</code> otherwise.
	
	@since PI_PDMODEL_VERSION >= 0x000D0000
 */
NPROC(ASBool, PDStructureDestIsValid, (PDStructureDestination dest))

/**
	Gets structure element to which a structure destination's
 
	@param dest IN/OUT The structure destination whose structure element is obtained.
	@param elemID IN/OUT (Filled by the method) If the structure destination points to a
 structure element in a different document, elemID will be filled with the element's ID.
 @return The structure element destination points to if the element is in the same doc .
 as destination; Invalid destination returned otherwise so result should be checked
 using PDStructureDestIsValid()
	@exception genErrBadParm is raised if the destination is not represented by an array.
	@see PDStructureDestCreate
 @see PDStructureDestIsValid
 
	@since PI_PDMODEL_VERSION >= 0x000D0000
 */
NPROC(PDSElement, PDStructureDestGetStructureElement, (PDStructureDestination dest, ASText* elemID))

/**
	Gets a structure destination's fit type, destination rectangle,
	and zoom factor. The destination must be represented by
	an array, which is the case for a GoToR action.
 
	@param dest IN/OUT The structure destination whose attributes are obtained.
	@param fitType IN/OUT (Filled by the method) The destination fit type.
	One of the values listed in View Destination Fit Types.
	@param destRect IN/OUT (Filled by the method) A pointer to a ASFixedRect
	containing the destination's rectangle, specified in user
	space coordinates.
	@param zoom IN/OUT (Filled by the method) The destination's zoom factor.
	@exception genErrBadParm is raised if the destination is not represented
	by an array.
	@see PDStructureDestCreate
 @ref ViewDestinationFitTypes
 
	@since PI_PDMODEL_VERSION >= 0x000D0000
 */
NPROC(void, PDStructureDestGetAttr, (PDStructureDestination dest, ASAtom* fitType, ASFixedRectP destRect, ASFixed* zoom))
/**
	Gets the Cos object corresponding to a structure destination
	and verifies that the structure destination is valid. This method
	does not copy the object, but is instead the logical equivalent
	of a type cast.
	
 @param dest IN/OUT The structure destination whose Cos object is obtained.
	@return Array Cos object for the structure destination. The contents
	of the array can be enumerated using CosObjEnum(). It returns
	a <code>NULL</code> Cos object if the structure destination is invalid, as
	determined by PDStructureDestIsValid().
	@see PDStructureDestFromCosObj
	@see PDStructureDestIsValid
 
	@since PI_PDMODEL_VERSION >= 0x000D0000
	
 */
NPROC(CosObj, PDStructureDestGetCosObj, (PDStructureDestination dest))

/**
	Converts the specified Cos object to a structure destination
	and verifies that the structure destination is valid. This method
	does not copy the object, but is instead the logical equivalent
	of a type cast.

	@param obj IN/OUT The dictionary Cos object to convert to a structure destination.
	@return An array Cos object for the structure destination.
	@exception pdErrBadAction is raised if the destination is invalid, as
	determined by PDStructureDestIsValid().
	@see PDStructureDestGetCosObj
 
	@since PI_PDMODEL_VERSION >= 0x000D0000
	
 */
NPROC(PDStructureDestination, PDStructureDestFromCosObj, (CosObj obj))

/**
 Gets the AFRelationship field text.
 
 @param attachment The file attachment.
 @return An <code>ASText</code> copy of the AFRelationship field.
 
	@since PI_PDMODEL_VERSION >= 0x000D0000
 */
NPROC(ASText, PDFileAttachmentGetRelationshipText, (PDFileAttachment pdfa))

/**
 Sets the AFRelationship field text.
 
 @param attachment The file attachment.
 @param An <code>ASText</code> copy to set AFRelationship field to.
 
	@since PI_PDMODEL_VERSION >= 0x000D0000
 */
NPROC(void, PDFileAttachmentSetRelationshipText, (PDFileAttachment pdfa, ASConstText descText))


/**

@param doc The document in which the action is created.

@param doc -	PDdoc to operate on. The Type3 subsitute Type1 fonts will be embedded if corresponding Type1 
				font is found on system, search by fontname.
				To keep the changes the document must be saved. Fonts on the user's
				system that match the original font definition will be embedded.
				If the font has information that indicates the font cannot be embedded for print
				and preview, then the font will not be embedded in the document.

@param flags - (1) If set fonts will be subset when embedded. Otherwise the entire font
					will be embedded. Full fonts should only be embedded if
					the information in the fonts indicates it can be used for editible
					embedding. Fonts that have more than 2048 characters
					will be subset embedded regardless of the setting of this flag.

			   (2) If set, fonts that have already been embedded but are not subset fonts,
				will be reembedded as a subset font.
@param enablefontfaux - Set parameter to true or false to enable or disable font fauxing of Type3 fonts
@param progMon A progress monitor. Use AVAppGetDocProgressMonitor()
to obtain the default. <code>NULL</code> may be passed, in which case
no progress monitor is used.
@param progMonClientData A pointer to user-supplied data
to pass to <code>progMon</code> each time it is called. It should be <code>NULL</code>
if <code>progMon</code> is <code>NULL</code>.
@param cancelProc A cancel procedure. Use AVAppGetCancelProc()
to obtain the current cancel procedure. It may be <code>NULL</code>, in
which case no cancel proc is used.
@param cancelProcClientData A pointer to user-supplied data
to pass to <code>cancelProc</code> each time it is called. It should be
<code>NULL</code> if <code>cancelProc</code> is <code>NULL</code>.
@param reportProc The report procedure.<code>NULL</code> may be passed, in which case
no reporting is done.
@param reportProcClientData A pointer to client data for the report procedure.

@since PI_PDMODEL_VERSION >= 0x000D0000
*/

NPROC(void, PDDocSubstituteType3Fonts, (PDDoc doc, ASUns32 flags, ASBool enablefontfaux, ProgressMonitor progMon,
	void *progMonClientData, CancelProc cancelProc, void *cancelProcClientData, ASReportProc reportProc, void *reportProcClientData))

/** Sets the preference to install the graphics state that was in effect at the beginning of the pattern’s parent content stream
	@param bInstallPatternParentGStateFlag
	Pass 1 to install GState was in effect at the beginning of the pattern’s parent content stream
*/
NPROC(void, PDPrefSetInstallPatternParentGState, (ASBool bInstallPatternParentGStateFlag))

/** Get the preference whether to install the graphics state that was in effect at the beginning of the pattern’s parent content stream
	@return value equal to 0 means apply current GState,
	value equal to 1 means install GState was in effect at the beginning of the pattern’s parent content stream.
*/
NPROC(ASBool, PDPrefGetInstallPatternParentGState, (void))

#undef ENPROC
#undef EPROC
#undef ESPROC
#undef XSPROC
#undef XNPROC
#undef XPROC
