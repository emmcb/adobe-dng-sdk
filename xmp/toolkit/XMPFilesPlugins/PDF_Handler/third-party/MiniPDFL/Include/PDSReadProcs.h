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

 PDSReadProcs.h

 - Catalog of functions exported by PDSEdit.

*********************************************************************/

/*------------------------------------------------------------------------
	PDS Objects (see PDSExpT.h).
------------------------------------------------------------------------*/
/*
	PDSTreeRoot	- Root of a structural tree.
	PDSElement	- Member of the structural tree (and super-class of PDSTreeRoot)
	PDSAttrObj	- Attribute Object
	PDSMC		- Marked Content (cover for PDEContainer)
	PDSMCR		- Marked Content Refence
	PDSOBJR		- PDF Object Reference
	PDSRoleMap	- RoleMap
	PDSClassMap	- ClassMap
*/

/* 
 * Struct Tree Root methods.
 */


/**
	Gets the structure tree root for a document. 
	@param pdDoc The PDDoc whose root is obtained. 
	@param treeRoot (Filled by the method) The structure tree 
	root.
	@return <code>true</code> if structure tree root found, <code>false</code> otherwise. 
	
	@see PDDocCreateStructTreeRoot 
	@see PDDocRemoveStructTreeRoot 
	@since PI_PDS_READ_VERSION >= 0x00040000
*/
NPROC (ASBool,	PDDocGetStructTreeRoot,		(IN  PDDoc pdDoc,
											 OUT PDSTreeRoot *treeRoot))

/**
	Gets the number of kids of the structure tree root.
	<p>This may throw various exceptions.</p>

	@param treeRoot IN/OUT The structure tree root whose number of 
	kids is obtained. 
	@return The number of kids of the structure tree root. 
	@see PDSTreeRootGetKid 
	@since PI_PDS_READ_VERSION >= 0x00040000
*/
NPROC (ASInt32,	PDSTreeRootGetNumKids,		(IN  PDSTreeRoot treeRoot))

/**
	Gets the kid at an array index in the specified structure 
	tree root. 
	@param treeRoot The structure tree root whose kid is obtained. 
	
	@param index The index of the kid to obtain. 
	@param kid (Filled by the method) A pointer to the kid at 
	<code>index</code>.
	@exception pdsErrBadPDF is raised if an error is found in the PDF file. 
	
	@see PDSTreeRootGetNumKids 
	@since PI_PDS_READ_VERSION >= 0x00040000
*/
NPROC (void,	PDSTreeRootGetKid,			(IN  PDSTreeRoot treeRoot,
											 IN  ASInt32 index,
											 OUT PDSElement *kid))

/**
	Gets the PDSRoleMap object for the specified structure tree 
	root. 
	<p>This may throw various exceptions.</p>

	@param treeRoot The structure tree root whose PDSRoleMap 
	is obtained. 
	@param roleMap (Filled by the method) A pointer to a location 
	in which to return the role map, if one exists. Set it to CosNull 
	if there is no role map. If a <code>NULL</code> pointer is passed, no 
	retrieval will take place.
	@return <code>true</code> if there is a role map, <code>false</code> otherwise. 
	@see PDSTreeRootCreateRoleMap 
	@since PI_PDS_READ_VERSION >= 0x00040000
*/
NPROC (ASBool,	PDSTreeRootGetRoleMap,		(IN  PDSTreeRoot treeRoot,
											 OUT PDSRoleMap *roleMap))

/**
	Gets the PDSClassMap object for the specified structure 
	tree root. 
	<p>This may throw various exceptions.</p>

	@param treeRoot The structure tree root whose PDSClassMap 
	is obtained. 
	@param classMap (Filled by the method) A pointer to a location 
	in which to return the class map, if one exists. Set it to 
	CosNull if there is no class map. If a <code>NULL</code> pointer is passed, 
	no retrieval will take place.
	@return <code>true</code> if there is a class map, <code>false</code> otherwise. 
	@see PDSTreeRootCreateClassMap 
	@since PI_PDS_READ_VERSION >= 0x00040000
*/
NPROC (ASBool,	PDSTreeRootGetClassMap,		(IN  PDSTreeRoot treeRoot,
											 OUT PDSClassMap *classMap))

/**
	Gets the element associated with the given ID, if any. 
	@param treeRoot The structure tree root in which to search 
	for <code>id</code>. 
	@param id A pointer to a buffer containing the ID to search 
	for. 
	@param numChars The number of characters in <code>id</code>. 
	@param element (Filled by the method) The element corresponding 
	to <code>id</code>. It is undefined if no element has the specified <code>id</code>.
	@return <code>true</code> if an element for <code>id</code> is found, or <code>false</code> with element undefined 
	if the tree root contains no IDTree value. 
	@exception pdsErrWrongTypeParameter is raised if <code>id</code> is <code>NULL</code> or <code>numChars</code> 
	is zero or less. 
	@exception pdsErrWrongTypeEntry is raised if the <code>IDTree</code> value in <code>treeRoot</code> 
	is not a dictionary. 
	@see PDSElementGetID 
	@since PI_PDS_READ_VERSION >= 0x00040000
*/
NPROC (ASBool,	PDSTreeRootGetElementFromID,(IN  PDSTreeRoot treeRoot,
											 IN  const char *id,
											 IN  ASInt32 numChars,
											 OUT PDSElement *element))
/*
 * PDSElement methods.
 */

/**
	Gets the element's structural element type. The type corresponds 
	to the Subtype key in the structure element dictionary. 
	
	<p>PDSElementGetType() gets the value of the Subtype key (not 
	the Type key) in the structure element dictionary. All PDSElement objects 
	have a Type value of StructElem. </p>

	@param element The element whose structural element type 
	is obtained.
	@return The ASAtom representing element's type. 
	@exception pdsErrWrongTypeParameter is raised if <code>element</code> is not a valid 
	PDSElement. 
	@see PDSElementSetType 
	@since PI_PDS_READ_VERSION >= 0x00040000
*/
NPROC (ASAtom,	PDSElementGetType,			(IN  PDSElement element))

/**
	Gets the immediate ancestor element of the specified element 
	in the tree. 

	<p>If the element's parent is another element, <code>parent</code> is set 
	to that parent and <code>parentIsTreeRoot</code> is set to <code>false</code>. If 
	the element's parent is the structure tree root, <code>parent</code> 
	is set to CosNull and <code>parentIsTreeRoot</code> is set to <code>true</code>. If 
	<code>parentIsTreeRoot</code> is <code>NULL</code>, it is not set. </p>

	@param element The element whose parent is obtained. 
	@param parent (Filled by the method) The element's parent. 
	
	@param parentIsTreeRoot (Filled by the method) The element's 
	parent is the structure tree root.
	@exception pdsErrWrongTypeParameter is raised if <code>element</code> is not a valid 
	PDSElement. 
	@see PDSElementGetKid 
	@see PDSElementGetStructTreeRoot 
	@see PDSMCGetInfo 
	@see PDSOBJGetParent 
	@since PI_PDS_READ_VERSION >= 0x00040000
*/
NPROC (void,	PDSElementGetParent,		(IN  PDSElement element,
									         OUT PDSElement *parent,
											 OUT ASBool *parentIsTreeRoot))

/**
	Gets the title of the specified element, returning the number 
	of bytes in the title. 

	<p>It can first be called with a <code>NULL</code> buffer to find the title 
	size, so that buffer can be appropriately sized as one greater 
	than the title's length. </p>
	@param element IN/OUT The element whose title is obtained. 
	@param buffer IN/OUT (Filled by the method) A buffer into which 
	the title text is placed. It may be <code>NULL</code>, in which case the 
	number of bytes in the title is returned. 
	@return The number of bytes in the <code>element</code> parameter's title, or zero if <code>element</code> has 
	no title. 
	@exception pdsErrWrongTypeParameter is raised if <code>element</code> is not a valid 
	PDSElement. 
	@see PDSElementSetTitle 

	@note Due to implementation issues, make the buffer one 
	byte larger than the required size. 
	@since PI_PDS_READ_VERSION >= 0x00040000
*/
NPROC (ASInt32,	PDSElementGetTitle,			(IN  PDSElement element,
											 OUT ASUns8 *buffer))

/**
	Gets the revision number of an element. 
	@param element IN/OUT The element whose revision is obtained. 
	
	@return The revision number of <code>element</code>. 
	@exception pdsErrWrongTypeParameter is raised if <code>element</code> is not a valid 
	PDSElement. 
	@see PDSElementIncrementRevision 
	@since PI_PDS_READ_VERSION >= 0x00040000
*/
NPROC (ASInt32,	PDSElementGetRevision,		(IN  PDSElement element))

/**
	Gets the number of attribute objects directly attached to 
	the specified element. 
	@param element IN/OUT The element whose number of attributes is 
	obtained. 
	@return The number of attribute objects directly attached to <code>element</code>. 
	
	@exception pdsErrWrongTypeParameter is raised if <code>element</code> is not a valid 
	PDSElement. 
	@see PDSElementGetAttrObj 
	@since PI_PDS_READ_VERSION >= 0x00040000
*/
NPROC (ASInt32,	PDSElementGetNumAttrObjs,	(IN  PDSElement element))

/**
	Gets the attribute object at a specified array index in 
	the specified element. 

	<p>If there is only one attribute object (that is, there is 
	no array of attributes), and <code>index</code> is zero, that attribute 
	object is obtained. </p>

	@param element IN/OUT The element whose attribute is obtained. 
	
	@param index IN/OUT The index of the attribute object to obtain. 
	@param attrObj IN/OUT (Filled by the method) The attribute object 
	at <code>index</code>. 
	@return The revision number of <code>element</code> at time of last association. 
	
	@exception pdsErrRequiredMissing 
	@exception pdsErrWrongTypeParameter is raised if <code>element</code> is not a valid 
	PDSElement. 
	@see PDSElementAddAttrObj 
	@see PDSElementGetNumAttrObjs 
	@see PDSElementRemoveAttrObj 
	@since PI_PDS_READ_VERSION >= 0x00040000
*/
NPROC (ASInt32,	PDSElementGetAttrObj,		(IN  PDSElement element,
											 IN  ASInt32 index,
											 OUT PDSAttrObj *attrObj))

/**
	Gets the number of classes to which the specified element 
	belongs. 
	@param element The element whose number of classes is 
	obtained.
	@return The number of classes to which <code>element</code> belongs. 
	@exception pdsErrWrongTypeParameter is raised if <code>element</code> is not a valid 
	PDSElement. 
	@see PDSElementGetClass 
	@since PI_PDS_READ_VERSION >= 0x00040000
*/
NPROC (ASInt32,	PDSElementGetNumClasses,	(IN  PDSElement element))

/**
	Gets the class name at an array index in the specified element. 
	
	<p>If there is only one attribute object (that is, there is 
	no array), and <code>index</code> is zero, that class name is obtained. </p>
	
	@param element The element whose class is obtained. 
	@param index The index of the class to obtain. 
	@param classAtom (Filled by the method) The ASAtom describing 
	the class.
	@return The revision number of <code>element</code> at the time of the last association. 
	
	@exception pdsErrRequiredMissing 
	@exception pdsErrWrongTypeParameter is raised if <code>element</code> is not a valid 
	PDSElement. 
	@see PDSElementAddClass 
	@see PDSElementGetNumClasses 
	@see PDSElementRemoveAllClasses 
	@see PDSElementRemoveClass 
	@since PI_PDS_READ_VERSION >= 0x00040000
*/
NPROC (ASInt32,	PDSElementGetClass,			(IN  PDSElement element,
											 IN  ASInt32 index,
											 OUT ASAtom *classAtom))

/**
	Gets the alternate text associated with an element. 

	<p>It can first be called with a <code>NULL</code> buffer to find the size, 
	so that buffer can then be appropriately sized. </p>
	@param element The element whose alternate text is obtained. 
	
	@param buffer (Filled by the method) A buffer into which 
	the alternate text is placed. It may be <code>NULL</code>, if the method 
	is called only to find the length of the element's alternate 
	text. If it is not <code>NULL</code>, <code>buffer</code> contains the element's actual 
	text. The string is <code>NULL</code>-terminated (but not correctly for 
	Unicode). This is not a C-style string, so normal string 
	handling functions may not work; the buffer may contain 
	a Unicode string.
	@return The number of bytes in the <code>element</code> parameter's alternate text. 
	@exception pdsErrWrongTypeParameter is raised if <code>element</code> is not a valid 
	PDSElement. 
	@see PDSElementSetAlt 
	@see PDSElementHasAlt 

	@note The Alt text can be legally defined as an empty string. 
	To differentiate between an Alt text string of zero length 
	and no Alt text being defined, call PDSElementHasAlt() first. 
	
	@note Due to implementation issues, make the buffer one 
	byte larger than the required size. The code will not <code>NULL</code>-terminate 
	the string correctly in the case of Unicode strings. 
	@since PI_PDS_READ_VERSION >= 0x00040000
*/
NPROC (ASInt32,	PDSElementGetAlt,			(IN  PDSElement element,
											 IN  ASUns8 *buffer))

/**
	Gets the number of kids of the specified element. 
	@param element IN/OUT The element  whose number of kids is obtained. 
	
	@return The number of direct kids of <code>element</code>. 
	@exception pdsErrWrongTypeParameter is raised if <code>element</code> is not a valid 
	PDSElement. 
	@see PDSElementGetKid 
	@since PI_PDS_READ_VERSION >= 0x00040000
*/
NPROC (ASInt32,	PDSElementGetNumKids,		(IN  PDSElement element))

/**
	Gets the kid at an array index in the specified element. 
	
	<p>A PDF structural element, unlike the structure tree root, can 
	have several different kinds of children: marked content, 
	another element, or an entire PDF object. The parameter 
	in which the kid is placed depends on the type of kid. If 
	the kid is a structural element or an object reference, 
	PDSElementGetKid() places the result in <code>cosObjKid</code>; if the 
	kid is page content, it is placed in <code>pointerKid</code>. </p>

	<p>Any or all of cosObjKid, pointerKid, and cosPage can be 
	<code>NULL</code> to get the kid's type without setting that parameter. </p>

	@param element The element whose specified kid is found. 
	@param index The index of the kid to obtain. 
	@param cosObjKid (Filled by the method) The CosObj of 
	the specified kid, if that kid is a PDSElement or an OBJR. 
	If <code>cosObjKid</code> is <code>NULL</code>, it is not filled in, but the type 
	of the kid is returned regardless. Note that this CosObj can 
	be treated as a PDSElement or a PDSObjR. Use the return 
	type to decide which to use. 
	@param pointerKid (Filled by the method) A pointer to the 
	kid at <code>index</code>, if that kid is an MC. If <code>pointerKid</code> is <code>NULL</code>, 
	it is not filled in, but the type of the kid is returned 
	regardless. 
	@param cosPage (Filled by the method) A pointer to the CosObj 
	of the page containing the kid. If <code>cosPage</code> is <code>NULL</code>, it is 
	not filled in, but the type of the kid is returned regardless.
	@return The ASAtom representing the kid's Type value: StructElem, 
	MC, or OBJR. MCR is never returned. 
	@exception pdsErrWrongTypeParameter is raised if <code>element</code> is not a valid 
	PDSElement. 
	@exception pdsErrBadPDF is raised if an error is found in the PDF file. 
	
	@see PDSElementGetFirstPage 
	@see PDSElementGetKidEx 
	@see PDSElementGetKidWithMCInfo 
	@see PDSElementGetNumKids 
	@see PDSElementInsertKid 
	
	@note When the kid is an MC, it is actually a pointer of 
	the type PDEContainer. As with all PDFEdit objects, you 
	must be careful to manage the reference count of the object 
	by calling PDEAcquire() and PDERelease(). PDSElementGetKid() does 
	not call PDEAcquire() for you. 

	@note This method cannot access marked content inside a 
	Form XObject. 
	@since PI_PDS_READ_VERSION >= 0x00040000
*/
NPROC (ASAtom,	PDSElementGetKid,			(IN  PDSElement element,
											 IN  ASInt32 index,
											 OUT CosObj *cosObjKid,
											 OUT void **pointerKid,
											 OUT CosObj *cosPage))

/**
	Gets the Cos object for the page of the first kid of the 
	element. 
	
	<p>This may throw various exceptions.</p>

	@param pdsElement IN/OUT The element whose kid's first page is found. 
	
	@param firstKidType IN/OUT (Filled by the method) A pointer to an 
	ASAtom for the name that appears as the Type entry of the 
	actual first kid of <code>element</code>. Possible values are the values 
	that PDSElementGetKid() can return. Pass <code>NULL</code> to inhibit setting 
	<code>firstKidType</code>. 
	@param firstCosObjKidOnAPage IN/OUT (Filled by the method) The 
	kid whose content determined that the page returned was 
	the first page with content, if that kid is a CosObj. Pass 
	<code>NULL</code> to inhibit setting <code>firstCosObjKidOnAPage</code>. 
	@param firstMCKidOnAPage IN/OUT (Filled by the method) The kid 
	whose content determined that the page returned was the 
	first page with content, if that kid is marked content 
	that is not a CosObj. Pass <code>NULL</code> to inhibit setting <code>firstMCKidOnAPage</code>. 
	
	@return The CosObj of the page found, CosObjNull if the element 
	has no page content. 
	@see PDSElementGetKid 

	@note The order in which the returned page is first is the 
	order of kids, not the order of pages. That is, the first 
	descendant with page content determines which page is returned. 
	@since PI_PDS_READ_VERSION >= 0x00040000
*/
NPROC (CosObj,	PDSElementGetFirstPage,		(IN  PDSElement pdsElement, 
											 OUT ASAtom *firstKidType, 
											 OUT CosObj *firstCosObjKidOnAPage, 
											 OUT PDEContainer *firstMCKidOnAPage))

/**
	Gets the ID of an element, or CosObjNull if there is no ID 
	set. 
	@param pdsElement The element whose ID is obtained. 
	@param idBuf (Filled by the method) A pointer to the buffer 
	containing the element's ID.
	@return The number of bytes in the ID, or zero if the element has 
	no ID. 
	@exception pdsErrWrongTypeParameter is raised if <code>element</code> is not a valid 
	PDSElement. 
	@exception pdsErrBadPDF is raised if an error is found in the PDF file. 
	
	@see PDSElementClearID 
	@see PDSElementSetID 
	@see PDSTreeRootGetElementFromID 
	@since PI_PDS_READ_VERSION >= 0x00040000
*/
NPROC (ASInt32, PDSElementGetID,			(IN  PDSElement pdsElement,
											 OUT ASUns8 *idBuf))

/**
	Gets the structure tree root of the document containing 
	element. 
	@param element The element whose title is obtained. 
	@param treeRoot (Filled by the method) The structure tree 
	root.
	@return <code>true</code> if the document has a structure tree root, <code>false</code> otherwise. 
	If there is a structure tree root, it sets <code>treeRoot</code> to be the 
	structure tree root. 
	@exception pdsErrWrongTypeParameter is raised if <code>element</code> is not a valid 
	PDSElement. 
	@see PDSTreeRootGetKid 
	@since PI_PDS_READ_VERSION >= 0x00040000
*/
NPROC (ASBool,	PDSElementGetStructTreeRoot,(IN  PDSElement element,
											 OUT PDSTreeRoot *treeRoot))

/*
 * Atrribute Object method
*/

/**
	Gets the value of the key (Owner) in the specified attribute 
	object. 
	<p>This may throw various exceptions.</p>

	@param element The attribute object whose owner is obtained.
	@return The ASAtom for the owner's name. 
	@see PDSAttrObjCreate 
	@since PI_PDS_READ_VERSION >= 0x00040000
*/
NPROC (ASAtom,	PDSAttrObjGetOwner,			(IN PDSAttrObj element))
/*
 * Marked Content Container method
 */


/**
	Gets the parent element of the specified marked content. 
	
	@param containingObj The CosObj containing the MC whose 
	parent is obtained. For marked content on a page, this is 
	the Cos object representing the page. For marked content 
	elsewhere, this is the stream in which the marked content 
	resides. 
	@param mc The marked content whose parent is obtained. 
	
	@param parent (Filled by the method) The parent element of 
	<code>containingObj</code>.
	@exception pdsErrBadPDF is raised if an error is found in the PDF file. It will also raise the error if the PDSMC passed to it is not in the structure tree. 
	@see PDSElementGetParent 
	@see PDSElementInsertMCAsKid 
	@see PDSElementRemoveKidMC 
	@since PI_PDS_READ_VERSION >= 0x00040000
*/
NPROC (void,	PDSMCGetParent,				(IN  CosObj containingObj, 
											 IN  PDSMC mc,
											 OUT PDSElement *parent))
/*
 * PDSOBJR methods
 */

/**
	Gets the parent element of the specified PDF object. 
	<p>This may throw various exceptions.</p>

	@param obj IN/OUT The PDF object whose parent element is obtained. 
	It must be referred to via an OBJR from some element (that 
	is, it has a <code>struct</code> parent key), otherwise it is undefined. 
	@param parent IN/OUT (Filled by the method) The parent element of 
	<code>obj</code>. 
	@see PDSElementGetParent 
	@since PI_PDS_READ_VERSION >= 0x00040000
*/
NPROC (void,	PDSOBJGetParent,			(IN  CosObj obj, 
											 OUT PDSElement *parent))

/*
 * RoleMap methods.
 */


/**
	Gets the type, if any, directly mapped in the specified 
	PDSRoleMap for the given element type. 
	@param roleMap The PDSRoleMap. 
	@param type The ASAtom for an element type whose mapping 
	is found.
	@return The ASAtom for the equivalent type specified in <code>roleMap</code>, 
	or ASAtomNull if type has no mapping in <code>roleMap</code>. 
	@exception pdsErrBadPDF is raised if an error is found in the PDF file. 
	
	@see PDSRoleMapDoesMap 
	@since PI_PDS_READ_VERSION >= 0x00040000
*/
NPROC (ASAtom,	PDSRoleMapGetDirectMap,		(IN  PDSRoleMap roleMap, 
											 IN  ASAtom type))

/**
	Determines whether the specified PDSRoleMap provides any 
	mapping path for two given element types. 
	@param roleMap IN/OUT The PDSRoleMap. 
	@param src IN/OUT The ASAtom for an element type whose mapping 
	is tested. 
	@param dst IN/OUT The ASAtom for an element type. Note that this may 
	be a standard element type. 
	@return <code>true</code> if an mapping path was found, <code>false</code> otherwise. 
	@exception pdsErrBadPDF is raised if an error is found in the PDF file. 
	
	@see PDSRoleMapMap 
	@since PI_PDS_READ_VERSION >= 0x00040000
*/
NPROC (ASBool,	PDSRoleMapDoesMap,			(IN  PDSRoleMap roleMap, 
											 IN  ASAtom src, 
											 IN  ASAtom dst))
/*
 * ClassMap methods.
 */


/**
	Gets the number of attribute objects associated with a class 
	name. 
	<p>This may throw various exceptions.</p>

	@param classMap IN/OUT The PDSClassMap. 
	@param classAtom IN/OUT The ASAtom of a class name for which the 
	number of associated attribute objects is found. 
	@return The number of attribute objects associated with the class in 
	<code>classAtom</code>. 
	@see PDSClassMapGetAttrObj 
	@since PI_PDS_READ_VERSION >= 0x00040000
*/
NPROC (ASInt32,	PDSClassMapGetNumAttrObjs,	(IN  PDSClassMap classMap, 
											 IN  ASAtom classAtom))

/**
	Gets the attribute object associated with the specified 
	class name at an index in the class. 

	<p>If there is only one object and index is zero, that object 
	is retrieved. </p>

	<p>This may throw various exceptions.</p>

	@param classMap The PDSClassMap. 
	@param classAtom The ASAtom of a class name for which 
	an associated attribute objects is found. 
	@param index The index of the desired attribute object in 
	the class. 
	@param attrObj (Filled by the method) The attribute object 
	at <code>index</code>. Set it to CosNull if there is no attribute object 
	at the specified location.
	@see PDSClassMapAddAttrObj 
	@see PDSClassMapGetNumAttrObjs 
	@since PI_PDS_READ_VERSION >= 0x00040000
*/
NPROC (void,	PDSClassMapGetAttrObj,		(IN  PDSClassMap classMap, 
											 IN  ASAtom classAtom,
											 IN  ASInt32 index, 
											 OUT PDSAttrObj *attrObj))
/*
 * New in Acrobat 5
 */


/**
	Functions identically to PDSElementGetKid(), but for children 
	that are marked contents can return the <code>mcid</code> as well as 
	or instead of the actual object. 

	@note This method cannot access marked content inside a 
	Form XObject. 
	@param element The PDSElement containing the kid that 
	is retrieved. 
	@param index The index of the kid. 
	@param cosObjKid (Filled in by method) The kid being accessed 
	(depending on the kid's type) or <code>NULL</code>. 
	@param mcid (Filled in by method) The kid's <code>mcid</code> or <code>NULL</code>. 
	
	@param pointerKid (Filled in by method) A pointer to the 
	kid, or <code>NULL</code>. 
	@param cosPage (Filled in by method) The CosObj of the 
	page containing the kid, or <code>NULL</code>.
	@return An ASAtom representing the Type value of the kid. See above. 
	
	@exception pdsErrWrongTypeParameter is raised if <code>element</code> is not a valid 
	PDSElement. 
	@exception pdsErrBadPDF is raised if an error is found in the PDF file. 
	
	@see PDSElementGetKid 
	@see PDSElementGetKidWithMCInfo 
	@since PI_PDS_READ_VERSION >= 0x00050000
*/
NPROC (ASAtom,	PDSElementGetKidEx,			(IN  PDSElement	element,
											 IN  ASInt32	index,
											 OUT CosObj*	cosObjKid,
											 OUT ASInt32*	mcid,
											 OUT void**		pointerKid,
											 OUT CosObj*	cosPage))
/**
	Gets the actual text associated with the specified PDSElement. 
	It returns the number of bytes in the text, or <code>0</code> if the element 
	has no actual text or has an empty string. 

	<p>To check for the existence of alternate text, check for 
	a non-zero return value. To get the needed size of <code>buffer</code>, 
	call this method with a <code>NULL</code> buffer. </p>

	@param element The structural element whose actual text 
	is sought. 
	@param buffer If not <code>NULL</code>, <code>buffer</code> contains the element's 
	actual text. The string is <code>NULL</code>-terminated (but not correctly 
	for Unicode). This is not a C-style string, so normal string 
	handling functions may not work; the buffer may contain 
	a Unicode string.
	@return An ASInt32 representing the number of bytes in the text, 
	or <code>0</code> if the element has no actual text. 
	@see PDSElementSetActualText 

	@note Due to implementation issues, make the buffer one 
	byte larger than the required size. Code will not <code>NULL</code>-terminate 
	the string correctly in the case of Unicode strings. 
	@since PI_PDS_READ_VERSION >= 0x00050000
*/
NPROC (ASInt32,	PDSElementGetActualText,	(IN  PDSElement element,
											 IN  ASUns8 *buffer))

/**
	Gets the language associated with the specified PDSElement. 
	
	<p>It returns the number of bytes in the language string, or <code>0</code> 
	if the element has no language or has an empty string. </p>

	<p>To check for the existence of expansion text, call PDSElementHasLanguage(). 
	To get the needed buffer size, call this method with a <code>NULL</code> 
	buffer. </p>
	@param element The structural element whose expansion 
	text is sought. 
	@param buffer (Filled by the method) A buffer containing 
	the element's expansion text, or <code>NULL</code>. See PDSElementSetLanguage() 
	for format and languages. If not <code>NULL</code>, buffer contains the 
	element's expansion text. The string is <code>NULL</code>-terminated 
	(but not correctly for Unicode). This is not a C-style string, 
	so normal string handling functions may not work; the buffer 
	may contain a Unicode string.
	@return An ASInt32 representing the number of bytes in the language 
	string. 
	@see PDSElementSetLanguage 
	@see PDSElementHasLanguage 

	@note Due to implementation issues, make the buffer one 
	byte larger than the required size. Code will not <code>NULL</code>-terminate 
	the string correctly in the case of Unicode strings. 
	@since PI_PDS_READ_VERSION >= 0x00050000
*/
NPROC (ASInt32,	PDSElementGetLanguage,	(IN  PDSElement element,
										 IN  ASUns8 *buffer))

/**
	Tests whether Alt text is defined for a given PDSElement. 
	
	@param element The PDSElement being tested.
	@return <code>true</code> if text exists (including the empty string); <code>false</code> 
	otherwise. 
	@see PDSElementGetAlt 
	@since PI_PDS_READ_VERSION >= 0x00050000
*/
NPROC (ASBool, PDSElementHasAlt,			(IN PDSElement element))

/**
	Tests whether ActualText is defined for a given PDSElement. 
	
	@param element The PDSElement being tested.
	@return <code>true</code> if text exists (including the empty string); <code>false</code> 
	otherwise. 
	@since PI_PDS_READ_VERSION >= 0x00050000
*/
NPROC (ASBool, PDSElementHasActualText,	(IN PDSElement element))

/**
	Tests whether a language string is defined for a given PDSElement. 
	
	@param element The PDSElement being tested.
	@return <code>true</code> if text exists (including the empty string); <code>false</code> 
	otherwise. 
	@since PI_PDS_READ_VERSION >= 0x00050000
*/
NPROC (ASBool, PDSElementHasLanguage,	(IN PDSElement element))

/**
	Functions identically to PDSElementGetKidEx(), but returns 
	additional information about marked content kids that are 
	in streams other than the page content streams. 
	@param element The PDSElement containing the kid that 
	is retrieved. 
	@param index The index of the kid. 
	@param cosObjKid (Filled in by method) The kid being accessed 
	(depending on the kid's type), or <code>NULL</code>. 
	@param mcidInfo (Filled in by method) The kid's information 
	object, or <code>NULL</code>. 
	@param pointerKid (Filled in by method) A pointer to the 
	kid, or <code>NULL</code>. 
	@param cosPage (Filled in by method) The CosObj of the 
	page containing the kid, or <code>NULL</code>.
	@return An ASAtom representing the Type value of the kid. 
	@exception pdsErrWrongTypeParameter is raised if <code>element</code> is not a valid 
	PDSElement. 
	@exception pdsErrBadPDF is raised if an error is found in the PDF file. 
	
	@see PDSElementGetKid 
	@see PDSElementGetKidEx 
	@since PI_PDS_READ_VERSION >= 0x00060000
*/
NPROC (ASAtom, PDSElementGetKidWithMCInfo, (PDSElement element,
                                            ASInt32 index,
                                            CosObj* cosObjKid,
                                            PDSMCInfoP mcidInfo,
                                            void** pointerKid,
                                            CosObj* cosPage))
/**
	Gets information about how the specified marked content 
	is contained in its parent. 
	@param containingObj The CosObj containing the MC whose 
	information is obtained. For marked content on a page, this 
	is the Cos object representing the page. For marked content 
	elsewhere, this is the stream in which the marked content 
	resides. 
	@param mc The marked content whose information is obtained. 
	
	@param info (Filled by the method) A pointer to a structure 
	that the method fills with information about <code>containingObj</code>.
	@exception pdsErrBadPDF is raised if an error is found in the PDF file. It will also raise the error if the PDSMC passed to it is not in the structure tree. 

	@note This method cannot access marked content inside a 
	Form XObject. 
	@since PI_PDS_READ_VERSION >= 0x00060000
*/
NPROC (void, PDSMCGetInfo, (CosObj containingObj,
                            PDSMC mc,
                            PDSMCInfoP info))

/**
	Gets the parent element of the specified marked content, 
	referred to by its containing object and marked-content 
	identifier. 
	@param mcid The identifier (MCID) of the marked content 
	whose parent is obtained. 
	@param containingObj The CosObj containing the marked 
	content whose parent is obtained. For marked content on 
	a page, this is the Cos object representing the page. For 
	marked content elsewhere, this is the stream in which the 
	marked content resides. 
	@param parent (Filled by the method) The parent element of 
	<code>containingObj</code>.
	@return <code>true</code> if the parent is successfully obtained, <code>false</code> otherwise. 
	
	@exception pdsErrBadPDF is raised if an error is found in the PDF file. It will also raise the error if the PDSMC passed to it is not in the structure tree. 
	@see PDSMCGetParent 
	@see PDSElementGetParent 
	@since PI_PDS_READ_VERSION >= 0x00060000
*/
NPROC (ASBool, PDSMCIDGetParent, (ASInt32 mcid,
                                  CosObj containingObj,
                                  PDSElement* parent))
/**
	Gets the PDE container object for the specified marked content. 
	
	@param mc The marked content whose container is obtained.
	@return The PDE container object. 
	@since PI_PDS_READ_VERSION >= 0x00060000
*/
NPROC (PDEContainer, PDSMCGetPDEContainer, (PDSMC mc))

/**
	Gets the Cos object corresponding to the specified element 
	object. This method does not copy the object, but is instead 
	the logical equivalent of a type cast. 
	@param element The element object whose Cos object is 
	obtained.
	@return The dictionary Cos object for the element object. 
	@since PI_PDS_READ_VERSION >= 0x00060000
*/
NPROC (CosObj, PDSElementGetCosObj, (PDSElement element))
/**
	Gets the Cos object corresponding to the specified attribute 
	object. This method does not copy the object, but is instead 
	the logical equivalent of a type cast. 
	@param attrObj The attribute object whose Cos object is 
	obtained.
	@return The dictionary Cos object for the attribute object. 
	@since PI_PDS_READ_VERSION >= 0x00060000
*/
NPROC (CosObj, PDSAttrObjGetCosObj, (PDSAttrObj attrObj))

/**
	<p>Returns <code>true</code> if the document declares that it has
	structure elements that conform to the UserProperties
	attributes or class conventions.</p>

	<p>This is based on both the presence of StructTreeRoot,
	and a value of <code>"true"</code> for the UserProperties
	key in the document's MarkInfo dictionary.</p>

	@param doc The PDDoc to be examined.
	@return An ASBool indicating whether the document declares that it has structure elements with UserProperties attributes or classes.
	@since PI_PDS_READ_VERSION >= 0x00070000
*/
NPROC (ASBool, PDDocHasUserProperties, (PDDoc doc))

/**
	Enumerates the elements in the document's structure
	tree that have UserProperties attributes or classes,
	calling the supplied enumeration procedure for each
	such element found. The procedure returns <code>true</code> to
	continue enumeration, or <code>false</code> to halt enumeration.

	@param doc The PDDoc whose structure elements are to be enumerated.
	@param proc The procedure to call for each PDSElement found to have UserProperties.
	@param clientData Client-supplied data to be passed to the client callback.
	@return <code>true</code> if the enumeration completes, <code>false</code> if the enumeration callback returns <code>false</code>.
  	@ingroup Enumerators
	@since PI_PDS_READ_VERSION >= 0x00070000
*/
NPROC (ASBool, PDDocEnumPDSElementsWithUserProperties, (PDDoc doc,
					EnumElementsWithUserPropertiesProc proc,
					void *clientData))

/**
	Returns <code>true</code> if the PDSElement has attribute objects
	or class objects with an owner of UserProperties.

	@param elem The PDSElement to examine.
	@return ASBool indicating that some attribute objects or class objects have an owner of UserProperties.
	@since PI_PDS_READ_VERSION >= 0x00070000
*/
NPROC (ASBool, PDSElementHasUserProperties, (PDSElement elem))

/**
	Enumerates the PDSElement object's user properties by traversing
	the list of attribute objects and class objects, calling
	the caller-supplied procedure for each entry in the
	properties array. The enumeration proc receives the
	property information as a pair of ASText objects, for the
	property name and the property value. The enumeration
	continues as long as the callback returns <code>true</code>, and halts
	when the proc returns <code>false</code> or all properties have been
	enumerated.

	@param elem The PDSElement whose user properties will be enumerated.
	@param proc The callback that is called for each user property item.
	@param clientData Client-supplied data to be passed to the client callback.
	@param includeHidden A boolean value indicating whether the client wants to be given property items that are marked as hidden.
	@return <code>true</code> if the enumeration completes, <code>false</code> if the enumeration callback returns <code>false</code>.
	@see PDSElementEnumUserPropertiesAsCosObj
	@since PI_PDS_READ_VERSION >= 0x00070000
*/
NPROC (ASBool, PDSElementEnumUserPropertiesAsASText, (PDSElement elem,
					PDSElementEnumUserPropertiesAsASTextProc proc,
					void *clientData,
					ASBool includeHidden))

/**
	Enumerates the PDSElement object's user properties by traversing
	the list of attribute objects and class objects, calling
	the caller-supplied procedure for each entry in the
	properties array. The enumeration proc receives the
	property information as a Cos Dictionary, with contents
	as described in the <i>PDF Reference</i>. The enumeration
	continues as long as the callback returns <code>true</code>, and halts
	when the proc returns <code>false</code> or all properties have been
	enumerated.

	@param elem The PDSElement whose user properties will be enumerated.
	@param proc The callback that is called for each user property item.
	@param clientData Client-supplied data to be passed to the client callback.
	@param includeHidden A boolean value indicating whether the client wants to be given property items that are marked as hidden.
	@return <code>true</code> if the enumeration completes, <code>false</code> if the enumeration callback returns <code>false</code>.
	@see PDSElementEnumUserPropertiesAsASText
	@since PI_PDS_READ_VERSION >= 0x00070000
*/
NPROC (ASBool, PDSElementEnumUserPropertiesAsCosObj, (PDSElement elem,
					PDSElementEnumUserPropertiesAsCosObjProc proc,
					void *clientData,
					ASBool includeHidden))

/**
	Starting at the supplied structure element, this procedure
	follows the chain of parents (see PDSElementGetParent()) until
	a structure element is found that has user properties. If no
	such element is found (for example, the chain ended at the structure
	tree root), CosNull is returned.

	@param elem The PDSElement at which to start searching upwards through the tree.
	@return The first ancestor of <code>elem</code> that contains UserProperties attributes or class information, or CosNull if none is found.
	@see PDSElementEnumKidsWithUserProperties
	@since PI_PDS_READ_VERSION >= 0x00070000
*/
NPROC (PDSElement, PDSElementFindAncestorWithUserProperties, (PDSElement elem))

/**
	Enumerates PDSElement objects, beneath the supplied PDSElement, that
	have user properties attributes/classes.
	
	<p>The elements in a structure tree that have user properties
	form a virtual tree themselves; this procedure enumerates
	the children of the given structure element in this virtual
	tree. In other words, this procedure enumerates all the
	descendents(<code>d</code>) of the supplied structure element(<code>e</code>) such that
	<code>PDSElementFindAncestorWithUserProperties(d)</code> would return (<code>e</code>).
	The enumeration continues as long as the callback returns
	<code>true</code>, and halts when the proc returns <code>false</code> or all virtual
	children have been enumerated.</p>

	@param elem The PDSElement below which to search for elements with user properties.
	@param proc The client-supplied callback to call for each element found.
	@param clientData Client-supplied data to be passed to the client callback.
	@return <code>true</code> if the enumeration completes, <code>false</code> if the enumeration callback returns <code>false</code>.
	@see PDSElementFindAncestorWithUserProperties
  	@ingroup Enumerators
	@since PI_PDS_READ_VERSION >= 0x00070000
*/
NPROC (ASBool, PDSElementEnumKidsWithUserProperties, (PDSElement elem,
					EnumElementsWithUserPropertiesProc proc,
					void *clientData))

/**
	Gets the title associated with the specified PDSElement as an ASText object.

	@param element The element whose title is sought.
	@param title (Filled by the method) The text object containing the title. 
	The client must pass a valid ASText object. The routine does not allocate it.

	@exception pdsErrWrongTypeParameter is raised if <code>element</code> is not a valid 
	PDSElement.
	@see PDSElementGetTitle
	@see PDSElementSetTitleASText
	@see PDSElementSetTitle
	@since PI_PDS_READ_VERSION >= 0x00080000
*/
NPROC (void,	PDSElementGetTitleASText, (PDSElement element, ASText title))

/**
	Gets the actual text associated with the specified PDSElement as an ASText object.

	@param element The element whose actual text is sought.
	@param text (Filled by the method) The text object containing the element's actual text. 
	The client must pass a valid ASText object. The routine does not allocate it.
	
	@see PDSElementGetActualText
	@see PDSElementSetActualTextASText 
	@see PDSElementSetActualText
	@since PI_PDS_READ_VERSION >= 0x00080000
*/
NPROC (void,  PDSElementGetActualTextASText, (PDSElement element, ASText text))

/**
	Gets the alternate text associated with the specified PDSElement as an ASText object.

	@param element The element whose alternate text is sought. 
	@param text (Filled by the method) The text object containing 
	the element's alternate text. The client must pass a valid ASText object.
	The routine does not allocate it.
	 
	@exception pdsErrWrongTypeParameter is raised if <code>element</code> is not a valid 
	PDSElement.
	@see PDSElementGetAlt
	@see PDSElementSetAltASText
	@see PDSElementSetAlt 
	@see PDSElementHasAlt
	@since PI_PDS_READ_VERSION >= 0x00080000
*/
NPROC (void, PDSElementGetAltASText, (PDSElement element, ASText text))

/** 
    Exports user properties of the specified PDSElement in XML.
    
    @param  userPropsElement The element whose user properties are to be exported 
                            in XML format.   
    @param wholeSubtree A boolean value indicating whether to export user properties of 
                        the whole structure tree which contains <code>userPropsElement</code>, or 
                        just the subtree which starts from <code>userPropsElement</code>. 
    @param includeHidden    A boolean value indicating whether the client wants to be given 
                            property items that are marked as hidden.  
    @param flattenClasses   A boolean value indicating whether to flatten the attribute classes for each structure
                            element.  If <code>true</code>, the user properties for that class will be listed as properties for 
                            that stucture element. If <code>false</code>, the class will be listed for that structure element, and 
                            a list of classes and their properties will be listed near the end of the XML.
    @param xmlLabels The XML tag/label information for exporting user properties. These labels are output as is.
                     There is no XML escaping done.  It is the caller's responsibility to make sure they conform
                     to the XML standard.
    @param output The output stream to which user properties are written. The encoding of the characters is UTF-8.

    @return An ASErrorCode to indicate the success of exporting user properties in XML format. 
        If ASErrorCode is <code>0</code>, it indicates success in exporting user properties; non-zero 
        value indicates otherwise.

    @see PDUserPropertiesXMLLabels
    @since PI_PDS_READ_VERSION >= 0x00080000
*/
NPROC (ASErrorCode, PDSElementExportUserProperties, (PDSElement userPropsElement, ASBool wholeSubtree, ASBool includeHidden, ASBool flattenClasses, PDUserPropertiesXMLLabels xmlLabels, ASStm output))

/**
Gets the number of associated files directly attached to
the specified element.
@param element IN/OUT The element whose number of associated files is
obtained.
@return The number of associated files directly attached to <code>element</code>.

@exception pdsErrWrongTypeParameter is raised if <code>element</code> is not a valid
PDSElement.
@see PDSElementGetAssociatedFile
@since PI_PDS_READ_VERSION >= 0x000C0000
*/
NPROC(ASInt32, PDSElementGetNumAssociatedFiles, (PDSElement element))

/**
Gets the associated file at a specified array index in
the specified element.

<p>If there is only one associated file (that is, there is
no array of associated files), and <code>index</code> is zero, that associated file
object is obtained. </p>

@param element IN The element whose associated file is obtained.

@param index IN The index of the associated file object to obtain.

@return The associated file of <code>element</code> at the index.

@exception pdsErrRequiredMissing
@exception pdsErrWrongTypeParameter is raised if <code>element</code> is not a valid
PDSElement.
@see PDSElementAddAssociatedFile
@see PDSElementGetNumAssociatedFiles
@see PDSElementRemoveAssociatedFile
@since PI_PDS_READ_VERSION >= 0x000C0000
*/
NPROC(PDFileSpec, PDSElementGetAssociatedFile, (PDSElement element,
                                                ASInt32 index))

/**
 Test if the specified elemnt has the associated file.
 
 @param element IN The element that is checked if it has the associated file.
 @param afObj IN The assocaited file
 @return <code>true</code> if the element has the associated file, <code>false</code> if it does not.

 @exception pdsErrRequiredMissing
 @exception pdsErrWrongTypeParameter is raised if <code>element</code> is not a valid
 PDSElement.
 @see PDSElementAddAssociatedFile
 @see PDSElementGetNumAssociatedFiles
 @see PDSElementRemoveAssociatedFile
 @since PI_PDS_READ_VERSION >= 0x000C0000
*/
NPROC(ASBool, PDSElementHasAssociatedFile, (PDSElement element, PDFileSpec fsObj))
