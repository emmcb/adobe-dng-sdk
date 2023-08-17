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

 PDSWriteProcs.h

 - Catalog of functions exported by the PDSWrite HFT.

*********************************************************************/
/*
 * Structure Tree Root methods.
 */



#if !PLUGIN 
#undef XNPROC
#if !READER
#define XNPROC NPROC
#else
#define XNPROC(returnType, name, params) NOPROC(name)
#endif
#endif

/**
	Creates a new StructTreeRoot element. 

	<p>If PDDocCreateStructTreeRoot() is called on a PDDoc that already 
	has a structure tree root, it returns without modifying 
	the document. </p>

	<p>It raises an exception if <code>pdDoc</code> already has a StructTreeRoot. </p>

	@param pdDoc IN/OUT The PDDoc for which the StructTreeRoot element 
	is created. 
	@param treeRoot IN/OUT (Filled by the method) The newly-created 
	StructTreeRoot element. 
	
	@see PDDocGetStructTreeRoot 
	@see PDSTreeRootGetRoleMap 
	@see PDSTreeRootGetClassMap 
	@see PDDocRemoveStructTreeRoot 
	@see PDSTreeRootCreateRoleMap 
	@see PDSTreeRootRemoveRoleMap 
	@see PDSTreeRootCreateClassMap 
	@see PDSTreeRootRemoveClassMap 
	@since PI_PDS_WRITE_VERSION >= 0x00040000
*/
UNPROC (void,	PDDocCreateStructTreeRoot,	(IN PDDoc pdDoc, OUT PDSTreeRoot *treeRoot))

/**
	Removes, but does not destroy, the specified StructTreeRoot 
	element from the specified PDDoc. 
	@param pdDoc IN/OUT The PDDoc for which the StructTreeRoot element 
	is removed. 
	@see PDDocCreateStructTreeRoot 
	@see PDDocGetStructTreeRoot 
	@since PI_PDS_WRITE_VERSION >= 0x00040000
*/
UNPROC (void,	PDDocRemoveStructTreeRoot,	(IN PDDoc pdDoc))


/**
	Inserts the specified kid element after the given position 
	as a kid of the specified structure tree root. 

	<p>This may raise various exceptions.</p>

	@param treeRoot IN/OUT The structure tree root in which a kid 
	is inserted. 
	@param kid IN/OUT The kid to insert. 
	@param insertAfter IN/OUT The position after which the kid is inserted. 
	If <code>element</code> currently has no kids, <code>insertAfter</code> is ignored. 
	
	@see PDSTreeRootRemoveKid 
	@see PDSTreeRootReplaceKid 
	@since PI_PDS_WRITE_VERSION >= 0x00040000
*/
UNPROC (void,    PDSTreeRootInsertKid,           (IN PDSTreeRoot treeRoot,
                                                 IN PDSElement kid,
                                                 IN ASInt32 insertAfter))


/**
	Removes the specified kid element from the specified structure 
	tree root. 

	<p>This may raise various exceptions.</p>

	@param treeRoot IN/OUT The structure tree root whose kid is removed. 
	
	@param kid IN/OUT The kid to remove. 
	@see PDSTreeRootInsertKid 
	@see PDSTreeRootReplaceKid 
	@since PI_PDS_WRITE_VERSION >= 0x00040000
*/
UNPROC (void,    PDSTreeRootRemoveKid,           (IN PDSTreeRoot treeRoot,
                                                 IN PDSElement kid))

/**
	Replaces structural element <code>oldKid</code> with <code>element</code> <code>newKid</code> as 
	a kid of <code>treeRoot</code>. 
	<p>This may raise various exceptions.</p>

	@param treeRoot IN/OUT The structure tree root whose kid is replaced. 
	
	@param oldKid IN/OUT The kid to replace. 
	@param newKid IN/OUT The kid that is replacing <code>oldKid</code>. 
	@see PDSTreeRootInsertKid 
	@see PDSTreeRootRemoveKid 
	@since PI_PDS_WRITE_VERSION >= 0x00040000
*/
UNPROC (void,    PDSTreeRootReplaceKid,          (IN PDSTreeRoot treeRoot,
                                                 IN PDSElement oldKid,
                                                 IN PDSElement newKid))

/**
	Creates and sets the PDSRoleMap of the specified StructTreeRoot 
	element. Any previously existing PDSRoleMap is unlinked. 

	<p>This may raise various exceptions.</p>
	
	@param treeRoot The structure tree root in which to create 
	a PDSRoleMap. 
	@param roleMap (Filled by the method) The newly created 
	PDSRoleMap.
	@see PDSTreeRootGetRoleMap 
	@see PDSTreeRootRemoveRoleMap 
	@since PI_PDS_WRITE_VERSION >= 0x00040000
*/
UNPROC (void,	PDSTreeRootCreateRoleMap,	(IN PDSTreeRoot treeRoot, OUT PDSRoleMap *roleMap))


/**
	Removes the PDSRoleMap of the specified structure tree root 
	element. It does nothing if one does not exist. 

	<p>This may raise various exceptions.</p>

	@param treeRoot The structure tree root whose PDSRoleMap 
	is removed.
	@see PDSTreeRootCreateRoleMap 
	@see PDSTreeRootGetRoleMap 
	@since PI_PDS_WRITE_VERSION >= 0x00040000
*/
UNPROC (void,	PDSTreeRootRemoveRoleMap,	(IN PDSTreeRoot treeRoot))


/**
	Creates a PDSClassMap in the specified tree root. 

	<p>Any previously existing PDSClassMap is unlinked. </p>

	<p>This may raise various exceptions.</p>

	@param treeRoot The structure tree root in which to create 
	a PDSClassMap. 
	@param classMap (Filled by the method) The newly created 
	PDSClassMap.
	@see PDSTreeRootGetClassMap 
	@see PDSTreeRootRemoveClassMap 
	@since PI_PDS_WRITE_VERSION >= 0x00040000
*/
UNPROC (void,	PDSTreeRootCreateClassMap,	(IN PDSTreeRoot treeRoot, OUT PDSClassMap *classMap))


/**
	Removes the PDSClassMap of the specified structure tree 
	root element. It does nothing if one does not exist. 

	<p>This may raise various exceptions.</p>

	@param treeRoot The structure tree root whose PDSClassMap 
	is removed.
	@see PDSTreeRootCreateClassMap 
	@since PI_PDS_WRITE_VERSION >= 0x00040000
*/
UNPROC (void,	PDSTreeRootRemoveClassMap,	(IN PDSTreeRoot treeRoot))
/*
 * PDSElement methods.
 */


/**
	Creates a new (but empty) PDSElement. 

	<p>This may raise various exceptions.</p>

	@param pdDoc The PDDoc in which the PDSElement is created. 
	
	@param element (Filled by the method) The newly created 
	PDSElement.
	@since PI_PDS_WRITE_VERSION >= 0x00040000
*/
UNPROC (void,	PDSElementCreate,			(IN PDDoc pdDoc, OUT PDSElement *element))

/**
	Sets an element's type value to the specified type. The 
	type corresponds to the Subtype key in the structure element 
	dictionary. 

	<p>PDSElementSetType() sets the value of the Subtype key, not 
	the Type key, in the structure element dictionary. All PDSElement objects 
	have a Type value of StructElem. </p>

	@param element The element whose type is set. 
	@param type The ASAtom representing the element's type.
	@exception pdsErrWrongTypeParameter is raised if <code>element</code> is not a valid 
	PDSElement. 
	@see PDSElementGetType 
	@since PI_PDS_WRITE_VERSION >= 0x00040000
*/
UNPROC (void,	PDSElementSetType,			(IN PDSElement element, IN ASAtom type))

/**
	Sets an element's title. 
	@param element IN/OUT The element whose title is set. 
	@param buffer IN/OUT A pointer to a buffer containing a string to 
	be made the element's title. 
	@param nBytes IN/OUT The number of bytes in <code>buffer</code> to use as the <code>element</code> parameter's 
	new title. It may be zero. It sets a title even if the buffer 
	length is zero, but such a title looks like no title according 
	to PDSElementGetTitle(). 
	@exception pdsErrWrongTypeParameter is raised if <code>element</code> is not a valid 
	PDSElement. 
	@see PDSElementGetTitle 
	@since PI_PDS_WRITE_VERSION >= 0x00040000
*/
UNPROC (void,	PDSElementSetTitle,			(IN PDSElement element, IN const ASUns8 *buffer, IN ASInt32 nBytes))

/**
	Increments an element's revision count by one. 

	<p>This may raise various exceptions.</p>

	@param element The element whose revision count is incremented.
	@since PI_PDS_WRITE_VERSION >= 0x00040000
*/
UNPROC (void,	PDSElementIncrementRevision,(IN PDSElement element))

/**
	Associates the specified attribute object with an element 
	at the element's current revision value. 

	<p>This may raise various exceptions.</p>

	@param element The element with which <code>attrObj</code> is associated. 
	
	@param attrObj The attribute object to associate with <code>element</code>.
	@see PDSElementGetAttrObj 
	@see PDSElementRemoveAttrObj 
	@since PI_PDS_WRITE_VERSION >= 0x00040000
*/
UNPROC (void,	PDSElementAddAttrObj,		(IN PDSElement element, IN PDSAttrObj attrObj))

/**
	Removes the specified attribute object from an element. 
	If <code>element</code> does not have an <code>attrObj</code> attribute, this method 
	does nothing. 
	
	@param element The element whose attribute is removed. 
	@param attrObj The attribute object to remove.
	@exception pdsErrWrongTypeParameter is raised if <code>element</code> is not a valid PDSElement or <code>attrObj</code> is not a valid attribute object. 
	@see PDSElementAddAttrObj 
	@see PDSElementGetAttrObj 
	@see PDSElementRemoveAllAttrObjs 

	@note Calling PDSElementRemoveAttrObj() while iterating over 
	the attribute objects of an element will change the relationship 
	between the attribute object indices and attribute objects. 
	Although it is possible to track this change in indices 
	in a single loop, it is more straightforward to accumulate 
	a list of attribute objects to remove during one pass over 
	the attribute objects and to carry out the actual removals 
	during a subsequent iteration over the accumulated list. 
	@since PI_PDS_WRITE_VERSION >= 0x00040000
*/
UNPROC (void,	PDSElementRemoveAttrObj,	(IN PDSElement element, IN PDSAttrObj attrObj))

/**
	Removes all attribute objects directly associated with the 
	specified element. 
	@param element The element whose attributes are removed.
	@exception pdsErrWrongTypeParameter is raised if <code>element</code> is not a valid 
	PDSElement. 
	@see PDSElementRemoveAttrObj 
	@since PI_PDS_WRITE_VERSION >= 0x00040000
*/
UNPROC (void,	PDSElementRemoveAllAttrObjs,(IN PDSElement element))

/**
	Adds a class name to the element's list of classes to which 
	it belongs at the element's current revision value. 

	<p>This may raise various exceptions.</p>

	@param element IN/OUT The element to which a class is added. 
	@param classAtom IN/OUT The ASAtom representing the class to add 
	to <code>element</code>. If <code>classAtom</code> is already present among the <code>element</code> parameter's 
	classes, it will not be added again. 
	@see PDSElementGetClass 
	@see PDSElementGetNumClasses 
	@see PDSElementRemoveAllClasses 
	@see PDSElementRemoveClass 
	@since PI_PDS_WRITE_VERSION >= 0x00040000
*/
UNPROC (void,	PDSElementAddClass,			(IN PDSElement element, IN ASAtom classAtom))

/**
	Removes the specified class name from the element's list 
	of classes to which it belongs. 
	@param element The element from which the specified class 
	is removed. 
	@param classAtom The ASAtom representing the class to 
	remove.
	@exception pdsErrWrongTypeParameter is raised if <code>element</code> is not a valid 
	PDSElement. 
	@see PDSElementAddClass 
	@see PDSElementGetClass 
	@see PDSElementGetNumClasses 
	@see PDSElementRemoveAllClasses 

	@note Calling PDSElementRemoveClass() while iterating over 
	the classes of an element will change the relationship between 
	class indices and classes. Although it is possible to track 
	this change in indices in a single loop, it is more straightforward 
	to accumulate a list of classes to remove during one pass 
	over the classes and to carry out the actual removals during 
	a subsequent iteration over the accumulated list. 
	@since PI_PDS_WRITE_VERSION >= 0x00040000
*/
UNPROC (void,	PDSElementRemoveClass,		(IN PDSElement element, IN ASAtom classAtom))

/**
	Removes all classes from the specified element. 
	@param element IN/OUT The element whose classes are removed. 
	@exception pdsErrWrongTypeParameter is raised if <code>element</code> is not a valid 
	PDSElement. 
	@see PDSElementAddClass 
	@see PDSElementGetClass 
	@see PDSElementGetNumClasses 
	@see PDSElementRemoveClass 
	@since PI_PDS_WRITE_VERSION >= 0x00040000
*/
UNPROC (void,	PDSElementRemoveAllClasses,	(IN PDSElement element))

/**
	Sets the alternate text representation of an element's contents. 
	
	@param element IN/OUT The element whose alternate text representation 
	is set. 
	@param buffer IN/OUT A pointer to a buffer containing a string to 
	be made the element's alternate text representation. 
	@param nBytes IN/OUT The number of bytes in <code>buffer</code> to use as the <code>element</code> parameter's 
	new alternate text representation. It may be zero. It sets an 
	Alt string even if the buffer length is zero, but such an 
	Alt string looks like no Alt string according to PDSElementGetAlt(). 
	
	@exception pdsErrWrongTypeParameter is raised if <code>element</code> is not a valid 
	PDSElement. 
	@see PDSElementGetAlt 
	@see PDSElementHasAlt 
	@since PI_PDS_WRITE_VERSION >= 0x00040000
*/
UNPROC (void,	PDSElementSetAlt,			(IN PDSElement element, IN const ASUns8 *buffer, IN ASInt32 nBytes))

/**
	Inserts the specified kid PDSElement object into the specified 
	element after position <code>insertAfter</code>. 
	@param element The element in which the specified kid is inserted. 
	
	@param kid The kid to insert. 
	@param insertAfter The position after which the kid is inserted. 
	If <code>element</code> currently has no kids, <code>insertAfter</code> is ignored.
	@exception pdsErrWrongTypeParameter 
	@see PDSElementGetFirstPage 
	@see PDSElementGetKid 
	@see PDSElementInsertMCAsKid 
	@see PDSElementInsertOBJAsKid 
	@see PDSElementInsertStmMCAsKid 
	@see PDSElementRemoveKid 
	@see PDSElementReplaceKid 
	@since PI_PDS_WRITE_VERSION >= 0x00040000
*/
UNPROC (void,	PDSElementInsertKid,		(IN PDSElement element, IN PDSElement kid, IN ASInt32 insertAfter))

/**
	Inserts a reference to the specified PDSMC (marked content) 
	in the specified element after position <code>insertAfter</code>. 

	<p>This method automatically creates MCR objects if needed. </p>

	<p>This may raise various exceptions.</p>

	@param element The element in which the reference is inserted. 
	
	@param cosPage The CosObj for the page containing the 
	reference to insert. 
	@param mc The marked content to insert. 
	@param insertAfter The position after which the reference 
	is inserted. If <code>element</code> currently has no kids, <code>insertAfter</code> 
	is ignored.
	@see PDSElementInsertKid 
	@see PDSElementInsertOBJAsKid 
	@see PDSElementInsertStmMCAsKid 
	@see PDSElementReplaceKidMC 
	@since PI_PDS_WRITE_VERSION >= 0x00040000
*/
UNPROC (void,	PDSElementInsertMCAsKid,	(IN PDSElement element,
                                                 IN CosObj cosPage,
                                                 IN PDSMC mc,
                                                 IN ASInt32 insertAfter))

/**
	Inserts a reference to the specified PDF object as a kid 
	into the specified element. 

	<p>This may raise various exceptions.</p>

	@param element IN/OUT The element in which the reference is inserted. 
	
	@param cosPage IN/OUT The CosObj for the page containing the reference 
	to insert. 
	@param obj IN/OUT The CosObj to insert. 
	@param insertAfter IN/OUT The position after which the reference is 
	inserted in <code>element</code>. If <code>element</code> currently has no kids, <code>insertAfter</code> 
	is ignored. 
	@see PDSElementReplaceKidOBJ 
	@since PI_PDS_WRITE_VERSION >= 0x00040000
*/
UNPROC (void,    PDSElementInsertOBJAsKid,       (IN PDSElement element,
                                                 IN CosObj cosPage,
                                                 IN CosObj obj,
                                                 IN ASInt32 insertAfter))

/**
	Removes the specified kid from an element. 

	<p>This may raise various exceptions.</p>

	@param element The element whose kid is removed. 
	@param kid The kid to remove.
	@see PDSElementGetKid 
	@see PDSElementInsertKid 
	@see PDSElementRemoveKidMC 
	@see PDSElementRemoveKidOBJ 

	@note The approved method of removing OBJ kids is PDSElementRemoveKidOBJ(). 
	@since PI_PDS_WRITE_VERSION >= 0x00040000
*/
UNPROC (void,	PDSElementRemoveKid,		(IN PDSElement element, IN CosObj kid))

/**
	Removes the specified PDSMC (marked content) from an element's 
	kids, if it has any. 

	<p>After calling this method, use PDPageSetPDEContent() to commit 
	any changes that have been made to the page contents. </p>

	<p>This may raise various exceptions.</p>

	@param element The element whose reference is removed. 
	@param cosPage The CosObj for the page containing the 
	reference to remove. 
	@param mc The marked content to remove.
	@see PDSElementInsertMCAsKid 
	@see PDSElementReplaceKidMC 
	@see PDSElementRemoveKid 
	@since PI_PDS_WRITE_VERSION >= 0x00040000
*/
UNPROC (void,	PDSElementRemoveKidMC,		(IN PDSElement element, IN CosObj cosPage, IN PDSMC mc))

/**
	Replaces the specified kid in the specified element. 

	<p>This may raise various exceptions.</p>

	@param element IN/OUT The element whose kid is replaced. 
	@param oldKid IN/OUT The kid to replace. 
	@param newKid IN/OUT The kid that is replacing <code>oldKid</code>. 
	@see PDSElementInsertKid 
	@see PDSElementGetFirstPage 
	@see PDSElementGetKid 
	@see PDSElementRemoveKid 
	@see PDSElementReplaceKidMC 
	@see PDSElementReplaceKidOBJ 

	@note The approved method of replacing OBJ kids is PDSElementReplaceKidOBJ(). 
	@since PI_PDS_WRITE_VERSION >= 0x00040000
*/
UNPROC (void,	PDSElementReplaceKid,		(IN PDSElement element, IN CosObj oldKid, IN CosObj newKid))

/**
	Replaces the specified PDSMC (on <code>oldCosPage</code>) with a new 
	PDSMC (on <code>newCosPage</code>) in the specified element. 

	<p>This may raise various exceptions.</p>

	@param element The element whose reference is replaced. 
	@param oldCosPage The CosObj for the page holding the 
	reference to replace. 
	@param oldMC The marked content to replace. 
	@param newCosPage The CosObj for the page holding the 
	reference that is replacing <code>oldMC</code>. 
	@param newMC The marked content that is replacing <code>oldMC</code>.
	@see PDSElementInsertMCAsKid 
	@see PDSElementRemoveKidMC 
	@see PDSElementReplaceKid 
	@since PI_PDS_WRITE_VERSION >= 0x00040000
*/
UNPROC (void,	PDSElementReplaceKidMC,		(IN PDSElement element, IN CosObj oldCosPage, IN PDSMC oldMC, IN CosObj newCosPage, IN PDSMC newMC))

/**
	Replaces <code>oldObj</code> with <code>newObj</code> on the specified page in the 
	specified element. 

	<p>This may raise various exceptions.</p>

	@param element IN/OUT The element whose object is replaced. 
	@param oldObj IN/OUT The object to replace. 
	@param newObj IN/OUT The object that is replacing <code>oldObj</code>. 
	@param newPage IN/OUT The CosObj for the page holding the reference 
	that is replacing <code>oldObj</code>. 
	@see PDSElementInsertOBJAsKid 
	@see PDSElementReplaceKid 
	@see PDSElementReplaceKidMC 
	@since PI_PDS_WRITE_VERSION >= 0x00040000
*/
UNPROC (void,    PDSElementReplaceKidOBJ,        (IN PDSElement element,
                                                 IN CosObj oldObj,
                                                 IN CosObj newObj,
                                                 IN CosObj newPage))

/**
	Sets the ID of an element to the given Cos string. 
	@param element The element whose ID is set. 
	@param buffer A pointer to a buffer containing a string 
	to be made the element's ID. 
	@param nBytes The number of bytes in <code>buffer</code> to use as the <code>element</code> parameter's 
	new ID. It may be zero. It sets an ID even if the buffer length 
	is zero, but such an ID looks like no ID according to PDSElementGetID().
	@exception ErrSysPDSEdit is raised if another element already has the ID as 
	its ID. 
	@exception pdsErrWrongTypeParameter is raised if <code>element</code> is not a valid 
	PDSElement. 
	@see PDSElementGetID 
	@see PDSElementClearID 
	@since PI_PDS_WRITE_VERSION >= 0x00040000
*/
UNPROC (void,    PDSElementSetID,                (IN PDSElement element,
                                                 IN const ASUns8 *buffer,
                                                 IN ASInt32 nBytes))

/**
	Removes an element's ID, if it exists. 
	@param element The element whose ID is removed.
	@exception pdsErrWrongTypeParameter is raised if <code>element</code> is not a valid 
	PDSElement. 
	@exception pdsErrBadPDF is raised if an error is found in the PDF file. 
	
	@see PDSElementGetID 
	@see PDSElementSetID 
	@since PI_PDS_WRITE_VERSION >= 0x00040000
*/
UNPROC (void,    PDSElementClearID,              (IN PDSElement element))

/*
 * Attribute Object methods
 */

/**
	Creates a new attribute object with the specified owner. 
	
	@param pdDoc The document in which the attribute object is 
	created. 

	<p>This may raise various exceptions.</p>

	@param owner The owner of the new attribute object. 
	@param indirect If <code>true</code>, it creates the attribute object 
	as an indirect Cos object and sets the <code>pdDoc</code> parameter's PDDocNeedsSave 
	flag (see PDDocFlags). If <code>false</code>, it creates the attribute object 
	as a direct object. 
	@param attrObj (Filled by the method) The newly created 
	attribute object.
	@see PDSAttrObjCreateFromStream 
	@since PI_PDS_WRITE_VERSION >= 0x00040000
*/
UNPROC (void,	PDSAttrObjCreate,			(IN PDDoc pdDoc, IN ASAtom owner, IN ASBool indirect, OUT PDSAttrObj *attrObj))

/**
	Creates an attribute object with the specified owner from 
	the specified Cos stream. 
	@param owner The owner of the new attribute object. 
	@param cosStreamObj The Cos stream containing the data 
	with which to create the attribute. The dictionary of this 
	stream is modified. 
	@param attrObj (Filled by the method) A pointer to the newly 
	created attribute object. This actually points to <code>cosStreamObj</code>. 
	May be <code>NULL</code>.
	@exception pdsErrWrongTypeParameter is raised if <code>cosStreamObj</code> is not a Cos stream. It may raise other exceptions as well.
	@see PDSAttrObjCreate 
	@since PI_PDS_WRITE_VERSION >= 0x00040000
*/
UNPROC (void,	PDSAttrObjCreateFromStream,	(IN ASAtom owner, IN OUT CosObj cosStreamObj, OUT PDSAttrObj *attrObj))


/*
 * RoleMap methods.
 */

/**
	Maps an element type (<code>src</code>) to another element type (<code>dst</code>) in 
	the specified PDSRoleMap. 
	@param roleMap The PDSRoleMap in which to create a new 
	mapping. 
	@param src The element type to map to <code>dst</code>. 
	@param dst The element type that <code>src</code> maps onto. Note that this 
	may be a standard element type, such as P.
	@exception ErrSysPDSEdit is raised if <code>src</code> is already mapped. 
	@see PDSRoleMapDoesMap 
	@since PI_PDS_WRITE_VERSION >= 0x00040000
*/
UNPROC (void,	PDSRoleMapMap,				(IN PDSRoleMap roleMap, IN ASAtom src, IN ASAtom dst))

/**
	Makes the specified element type have no mapping. 

	<p>This may raise various exceptions.</p>

	@param roleMap IN/OUT The PDSRoleMap in which to un-map the <code>src</code> 
	element type. 
	@param src IN/OUT The element type whose mapping is removed. 
	@param fixupOthers IN/OUT If <code>true</code>, any element type that was directly 
	mapped to <code>src</code> is mapped to whatever <code>src</code> previously mapped 
	to. If <code>false</code>, PDSRoleMapUnMapSrc() only un-maps <code>src</code>. 
	@see PDSRoleMapUnMapDst 
	@since PI_PDS_WRITE_VERSION >= 0x00040000
*/
UNPROC (void,	PDSRoleMapUnMapSrc,			(IN PDSRoleMap roleMap, IN ASAtom src, IN ASBool fixupOthers))

/**
	Makes the specified element type have no mapping. 
	@param roleMap The PDSRoleMap in which to un-map all element 
	types that map onto the <code>dst</code> element type. 
	@param dst The element type to which all mappings are removed. 
	All element types that map to the <code>dst</code> element type are unmapped.
	@exception pdsErrWrongTypeParameter 
	@see PDSRoleMapUnMapSrc 
	@since PI_PDS_WRITE_VERSION >= 0x00040000
*/
UNPROC (void,	PDSRoleMapUnMapDst,			(IN PDSRoleMap roleMap, IN ASAtom dst))

/**
	Makes a copy of a PDSRoleMap, making it the PDSRoleMap of 
	the specified StructTreeRoot. 

	<p>This may raise various exceptions.</p>

	@param srcRoleMap The PDSRoleMap to copy. 
	@param dstTreeRoot The structure tree root in which to 
	place srcRoleMap. 
	@param dstRoleMap (Filled by the method) If not <code>NULL</code>, 
	it points to the new, copied PDSRoleMap.
	@see PDSTreeRootGetRoleMap 
	@since PI_PDS_WRITE_VERSION >= 0x00040000
*/
UNPROC (void,	PDSRoleMapCopy,				(IN PDSRoleMap srcRoleMap, IN PDSTreeRoot dstTreeRoot, OUT PDSRoleMap *dstRoleMap))

/*
 * ClassMap methods.
 */

/**
	Adds the specified attribute object to the specified PDSClassMap 
	for the given class name. If the attribute object is already 
	present, it is not added a second time. 
	@param classMap The PDSClassMap to which the specified 
	attribute object is added. 
	@param classAtom The ASAtom representing the class name. 
	
	@param attrObj The attribute object to add to the class in 
	<code>classAtom</code>.
	@exception pdsErrBadPDF is raised if an error is found in the PDF file. 
	
	@see PDSClassMapGetAttrObj 
	@see PDSClassMapRemoveAttrObj 
	@since PI_PDS_WRITE_VERSION >= 0x00040000
*/
UNPROC (void,	PDSClassMapAddAttrObj,		(IN PDSClassMap classMap,
                                             IN ASAtom classAtom,
                                             IN PDSAttrObj attrObj))

/**
	Removes the specified class from the specified PDSClassMap, 
	if it exists. 

	<p>This may raise various exceptions.</p>

	@param classMap IN/OUT The PDSClassMap from which a class is removed. 
	
	@param classAtom IN/OUT The ASAtom representing the class to remove 
	from <code>classMap</code>. 
	@see PDSClassMapRemoveAttrObj 
	@since PI_PDS_WRITE_VERSION >= 0x00040000
*/
UNPROC (void,	PDSClassMapRemoveClass,		(IN PDSClassMap classMap, IN ASAtom classAtom))

/**
	Removes the specified attribute object from the specified 
	PDSClassMap. If classAtom is ASAtomNull, it removes all occurrences 
	of <code>attrObj</code> in the entire <code>classMap</code>. 
	@param classMap The PDSClassMap from which the specified 
	attribute object is removed. 
	@param classAtom The ASAtom of a class name for which 
	the associated attribute object is found. 
	@param attrObj The attribute object to remove from <code>classMap</code>.
	@exception pdsErrBadPDF is raised if an error is found in the PDF file. 
	
	@see PDSClassMapAddAttrObj 
	@see PDSClassMapRemoveClass 
	@since PI_PDS_WRITE_VERSION >= 0x00040000
*/
UNPROC (void,	PDSClassMapRemoveAttrObj,	(IN PDSClassMap classMap,
                                             IN ASAtom classAtom,
                                             IN PDSAttrObj attrObj))
/*
 * New in Acrobat 5
 */


/**
	Sets the actual text representation of the specified PDSElement object's 
	contents to <code>buffer</code> (from <code>0</code> to <code>nBytes</code>). 
	@param element The PDSElement whose contents are being 
	set to <code>buffer</code>. 
	@param buffer The buffer to which the PDSElement object's contents 
	are being set. 
	@param nBytes The number of bytes in the text representation.
	@see PDSElementGetActualText 
	@since PI_ASEXTRA_VERSION >= 0x00050000
*/
UNPROC (void,	PDSElementSetActualText,	(IN PDSElement element, IN const ASUns8 *buffer, IN ASInt32 nBytes))

/**
	Sets the language field associated with the PDSElement to 
	the <code>buffer</code> parameter's contents (from 0 to nBytes). 
	@param element The PDSElement whose language field is 
	set to <code>buffer</code>. 
	@param buffer A pointer to a buffer containing a string 
	to be made the element's language field. The empty string 
	indicates that the language is unknown. The string should be 
	in the format <code><IETF RFC-1766-language-code></code>. Note that the ISO 639 language 
	codes can be found at http://lcweb.loc.gov/standards/iso639-2.
	@param nBytes The size of <code>buffer</code>. It may be zero. It sets the language 
	even if the buffer length is zero, but such a language setting 
	looks like no language according to PDSElementGetLanguage.
	@see PDSElementGetLanguage 
	@see PDSElementHasLanguage 

	@note IANA registered language codes can be found at http://www.isi.edu.
	@note The IETF Standard for Language Element Values (RFC 1766) can be found at http://www.ietf.org/rfc/rfc1766.txt?number=1766.
	@since PI_PDS_WRITE_VERSION >= 0x00050000
*/
UNPROC (void,	PDSElementSetLanguage,		(IN PDSElement element, IN const ASUns8 *buffer, IN ASInt32 nBytes))

/**
	Removes an OBJ from among the kids of a given element. It does 
	nothing if the given OBJ is not a kid of the given element. 

	<p>This may raise various exceptions.</p>

	@param element The element whose kid is having an OBJ removed. 
	
	@param kid The kid whose OBJ is removed.
	@see PDSElementInsertMCAsKid 
	@see PDSElementReplaceKidMC 
	@see PDSElementRemoveKid 
	@since PI_PDS_WRITE_VERSION >= 0x00050000
*/
UNPROC (void, PDSElementRemoveKidOBJ, (IN PDSElement element, IN CosObj kid))

/*
 * New since Acrobat 5
 */

/**
	Extends PDSElementInsertMCAsKid(), inserting content that is in a
	stream other than a page content stream. This function is the same 
	as PDSElementInsertStmMCAsKid().

	<p>This may raise various exceptions.</p>

	@param element The element in which the reference is inserted.
	@param cosPage The CosObj for the page containing the reference to insert.
	@param mc The marked content to insert.
	@param insertAfter The position after which the reference is inserted. If
	<code>element</code> currently has no kids, <code>insertAfter</code> is ignored.
	@param cosStream The stream containing the content given by <code>mc</code>.
	@param streamOwner A Cos object to record as the owner of the content. It can
	be CosNull if the owner is not important.
	@see PDSElementInsertKid
	@see PDSElementInsertMCAsKid
	@see PDSElementInsertOBJAsKid
	@see PDSElementInsertStmMCAsKid
	@see PDSElementReplaceKidMC
	@since PI_PDS_WRITE_VERSION >= 0x00060000
*/
UNPROC (void,  PDSElementInsertMCAsKidEx, (IN PDSElement element,
                                          IN CosObj cosPage,
                                          IN PDSMC mc,
                                          IN ASInt32 insertAfter,
                                          IN CosObj cosStream,
                                          IN CosObj streamOwner))

/**
	Inserts a marked content sequence from a non-page-content 
	stream as a kid of the specified element. 

	<p>This may raise various exceptions.</p>

	@param element The element in which the reference is inserted. 
	
	@param cosPage The CosObj for the page containing the 
	reference to insert. 
	@param containingStm The stream containing the content given 
	by <code>mc</code>. 
	@param stmOwner The PDF object owning the stream given 
	in <code>cosStream</code> (for example, the annotation to which an appearance 
	stream belongs). It can be CosNull if the owner is not important. 
	
	@param mc The marked content to insert. 
	@param insertAfter The position after which the reference 
	is inserted. If <code>element</code> currently has no kids, <code>insertAfter</code> 
	is ignored.
	@see PDSElementInsertKid 
	@see PDSElementInsertMCAsKid 
	@see PDSElementInsertOBJAsKid 
	@see PDSElementReplaceKidMC 
	@since PI_PDS_WRITE_VERSION >= 0x00060000
*/
UNPROC (void, PDSElementInsertStmMCAsKid, (PDSElement element,
                                          CosObj cosPage,
                                          CosObj containingStm,
                                          CosObj stmOwner,
                                          PDSMC mc,
                                          ASInt32 insertAfter))

/**
	Updates the stream entries (Stm) in marked content reference 
	dictionaries to reference a new Cos stream object. It replaces 
	references to the old stream with refererences to the new 
	stream. 

	<p>This may raise various exceptions.</p>

	@param treeRoot The structure tree root in which stream 
	references are updated. 
	@param oldStream The stream reference to replace. 
	@param newStream The stream reference that is replacing 
	<code>oldStream</code>.
	@since PI_PDS_WRITE_VERSION >= 0x00060000
*/
UNPROC (void, PDSTreeRootReplaceStreamRef, (PDSTreeRoot treeRoot,
                                           CosObj oldStream,
                                           CosObj newStream))

/**
	Creates a reference handle to a piece of marked content that 
	can be used to associate the content with structure. The handle 
	can persist beyond the lifetime of the marked contents, allowing 
	greater flexibility about when structure information can be 
	created.

	<p>This may raise various exceptions.</p>

	@param container The marked content to create a reference for. 
	It must be either a PDEContainer or PDEBeginContainer.
	@param cosDoc	The document within which the reference will be used.
	@param mcid		The mcid to set for the container.
	@see PDSMCRefDestroy
	@see PDSElementInsertMCRefAsKid

	@note This must be called before placing the container within
	the content stream that owns it.

	@note The handle will persist until PDSMCRefDestroy is called.

	@note All values in the PDSMCInfo object apart from mcid are currently ignored.
	@since PI_PDS_WRITE_VERSION >= 0x00070000
*/
UNPROC (PDSMCRef, PDSMCRefCreate,	(IN PDEElement container , 
									 IN CosDoc     cosDoc,
									 IN ASInt32	   mcid))

/**
	Destroys a marked content reference created with PDSMCRefCreate(). 
	This should only be called once the reference has been placed in
	the structure tree or if the reference is no longer needed.

	@param ref The marked content reference to destroy.
	@exception Unknown 
	@see PDSMCRefCreate
	@see PDSElementInsertMCRefAsKid

	@note If the PDSMCRef is associated with a PDSMC, it will be set as 
	invalid and ignored on subsequent processing.
	@since PI_PDS_WRITE_VERSION >= 0x00070000
*/
UNPROC (void, PDSMCRefDestroy, (IN PDSMCRef ref))

/**
	Takes a marked content reference and places the content 
	that it identifies in the structure as a child of the element.

	<p>This may raise various exceptions.</p>

	@param element The structure element with which to associate marked content.
	@param ref The marked content reference describing the content on the page.
	It must have had a valid MCID, and must have been completed by subsequent content 
	stream processing calls.
	@param insertAfter The position after which the marked content is inserted into 
	the element's kids. If the element has no children, <code>insertAfter</code> is ignored.
	@see PDSMCRefCreate

	@note the content reference handle will be filled out automatically
	if PDPageSetPDEContent(), PDEFormSetContent(), or PDEGroupSetContent() is 
	called. Otherwise, PDEContentSetPage() or PDEContentSetContainingStmAndOwner()
	must be called explicitly.
	@since PI_PDS_WRITE_VERSION >= 0x00070000
*/
UNPROC (ASBool, PDSElementInsertMCRefAsKid,	(IN PDSElement element, 
											IN PDSMCRef ref, 
											IN ASInt32 insertAfter))

/**
	Sets an element's title. 

	@param element The element whose title is being set.
	@param title The text object containing the string to 
	be made the element's title.
	
	@exception Raises pdsErrWrongTypeParameter if element is not a valid 
	PDSElement. 
	@see PDSElementSetTitle
	@see PDSElementGetTitleASText 
	@see PDSElementGetTitle
	@since PI_PDS_WRITE_VERSION >= 0x00080000
*/
UNPROC (void,	PDSElementSetTitleASText, (PDSElement element, const ASText title))

/**
	Sets an element's actual text. 

	@param element The element whose content is being set.
	@param text The text object containing the string to 
	be made the element's actual text.
	
	@see PDSElementSetActualText
	@see PDSElementGetActualTextASText
	@see PDSElementGetActualText
	@since PI_PDS_WRITE_VERSION >= 0x00080000
*/
UNPROC (void,  PDSElementSetActualTextASText, (PDSElement element, const ASText text))

/**
	Sets the alternate text representation of an element's contents (ASText version of PDSElementSetAlt).  

	@param element The element whose alternate text representation is being set.
	@param text  The text object containing the string to  
	be set as the element's alternate text representation.
	
	@exception Raises pdsErrWrongTypeParameter if element is not a valid 
	PDSElement.
	@see PDSElementSetAlt
	@see PDSElementGetAltASText
	@see PDSElementGetAlt
	@since PI_PDS_WRITE_VERSION >= 0x00080000
*/
UNPROC (void, PDSElementSetAltASText, (PDSElement element, const ASText text))

/**
Associates the specified associated file with an element
at the element's current revision value.

<p>This may raise various exceptions.</p>

@param element The element with which <code>afObj</code> is associated.

@param afObj The associated file to associate with <code>element</code>.
@see PDSElementGetAssociatedFile
@see PDSElementRemoveAssociatedFile
@since PI_PDS_WRITE_VERSION >= 0x000C0000
*/
UNPROC(void, PDSElementAddAssociatedFile, (PDSElement element, PDFileSpec fsObj))

/**
Removes the specified associated file from an element.
If <code>element</code> does not have an <code>afObj</code> associated file, this method
does nothing.

@param element The element whose associated file is removed.
@param afObj The associated file to remove.
@exception pdsErrWrongTypeParameter is raised if <code>element</code> is not a valid PDSElement or <code>afObj</code> is not a valid associated file.
@see PDSElementAddAssociatedFile
@see PDSElementGetAssociatedFile
@see PDSElementRemoveAllAssociatedFiles

@note Calling PDSElementRemoveAssociatedFile() while iterating over
the associated files of an element will change the relationship
between the associated file indices and associated files.
Although it is possible to track this change in indices
in a single loop, it is more straightforward to accumulate
a list of associated files to remove during one pass over
the associated files and to carry out the actual removals
during a subsequent iteration over the accumulated list.
@since PI_PDS_WRITE_VERSION >= 0x000C0000
*/
UNPROC(void, PDSElementRemoveAssociatedFile, (PDSElement element, PDFileSpec fsObj))

/**
Removes all associated files directly associated with the
specified element.
@param element The element whose associated files are removed.
@exception pdsErrWrongTypeParameter is raised if <code>element</code> is not a valid
PDSElement.
@see PDSElementRemoveAssociatedFile
@since PI_PDS_WRITE_VERSION >= 0x000C0000
*/
UNPROC(void, PDSElementRemoveAllAssociatedFiles, (PDSElement element))
#undef XNPROC
