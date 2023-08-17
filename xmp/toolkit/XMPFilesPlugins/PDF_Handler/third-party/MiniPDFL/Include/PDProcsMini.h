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


#if ADD_ANNOTS

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

UNPROC(PDAction, PDActionNewFromURI, (PDDoc doc, const char* uri, ASBool bMap))

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
//UNPROC(PDAction, PDActionNewFromFileSpec, (PDDoc containingDoc, ASAtom type, PDFileSpec fileSpec))

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

#endif // ADD_ANNOTS


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
//XNPROC(void, PDDocMovePage, (PDDoc doc, ASInt32 moveToAfterThisPage, ASInt32 pageToMove))

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
//XNPROC(void, PDDocReplacePages, (PDDoc doc, ASInt32 startPage, PDDoc doc2, ASInt32 startPageDoc2, ASInt32 numPages, ASBool mergeTextAnnots, ProgressMonitor progMon, void *progMonClientData,
//	CancelProc cancelProc, void *cancelProcClientData))

/**
	Clears all the non-fatal errors encountered since the document was opened, or <code>PDDocClearErrors</code> was called.
	@param doc The document in which the non-fatal errors have occurred.
	@since PI_PDMODEL_VERSION >= 0x000A0000
*/	

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

#if ADD_ASFIXED //PKG
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
#endif //ADD_ASFIXED //PKG

#if ADD_ANNOTS
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

#endif // ADD_ANNOTS

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

#if ADD_ASFIXED //PKG
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
#endif //ADD_ASFIXED //PKG

/* BEGIN Optional Content API calls */

#if ADD_OCG
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

#endif // ADD_OCG

#if ADD_ANNOTS && ADD_OCG
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
#endif // ADD_ANNOTS && ADD_OCG

#if ADD_OCG
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
#if ADD_ANNOTS
NPROC( ASBool, PDAnnotIsCurrentlyVisible, (PDAnnot annot,  PDOCContext ocContext) )
#endif

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
//NPROC( ASBool, PDPageFlattenOC, (PDPage pdPage, PDOCContext context) )

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
//NPROC( ASBool, PDDocFlattenOC, (PDDoc pdDoc, PDOCContext context) )

/* END Optional Content API calls */
#endif // ADD_OCG



NPROC(void, PDDocClearErrors, (PDDoc doc))
NPROC(ASHostEncoding, PDGetHostEncoding, (void))

#undef ENPROC
#undef EPROC
#undef ESPROC
#undef XSPROC
#undef XNPROC
#undef XPROC
