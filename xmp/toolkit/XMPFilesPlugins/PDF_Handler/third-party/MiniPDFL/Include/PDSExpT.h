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

 PDSExpT.h

 - Types, macros, structures, etc. required to use the PDSEdit HFT.

*********************************************************************/

#ifndef _H_PDSExpT
#define _H_PDSExpT

#include "Environ.h"
#if PLUGIN || ACROBAT_LIBRARY
#include "CoreExpT.h"
#include "ASExpT.h"
#include "CosExpT.h"
#include "PDExpT.h"
#else
#include "PubTypes.h"
#include "ASTypes.h"
#include "Cos.h"
#include "PDExpT.h"
#endif
#include "PEExpT.h"

/* 
** Function prototypes
** The following defines are used in function prototypes
** for labelling function parameters as input or output.
** Note that function arguments which are pointers and are
** marked as 'OUT' are pointers to buffers and the buffers 
** will contain the output data.
*/
#define IN
#define OUT 

/* Note on integer types.
** The following integer types are used for the following purposes.
**	Index into a structure or array:		ASInt32
**	Count, number of elements:				ASInt32
**	Variable or argument of enum:			ASInt32
**	Size in bytes of a structure or data:	ASUns32
**	Bitfield flag:							ASUns32
**
** I find it easier to use these existing types than to define
** new types such as Count, Index and Size, and to remember to 
** use them consistently.
**
** I use an ASInt32 for index and enum because both may be negative.
** (Although I don't use negative indixes in this API.)
** I use ASInt32 for enums because enums can have different sizes
** on different compilers; I want my API to be independent of compiler.
** ASInt32 is used for count because it cannot be greater than an index.
** ASUns32 is the obvious choice for size and bitfield (os_size_t has 
** too many underlines in it!-) 
*/

/*------------------------------------------------------------------------
	PDS Public Types.
------------------------------------------------------------------------*/


/**
	Represents PDF structural elements, which are nodes in a tree giving a PDF
	document's logical structure.
	@see PDSElementCreate
	@see PDSElementGetParent
	@see PDSMCGetParent
	@see PDSOBJGetParent
	@see PDSTreeRootGetElementFromID
*/
typedef CosObj PDSElement;


/**
	The root of the structure tree, which is a central repository for information related to a
	PDF document's logical structure. There is at most one PDSTreeRoot in each
	document.
	@see PDDocCreateStructTreeRoot
	@see PDDocGetStructTreeRoot
	@see PDDocRemoveStructTreeRoot
*/
typedef CosObj PDSTreeRoot;



/**
	Represents PDF logical structure attribute objects, which are dictionaries containing
	application-specific data that can be attached to PDSElement objects.
	@see PDSAttrObjCreate
	@see PDSAttrObjCreateFromStream
	@see PDSClassMapGetAttrObj
	@see PDSElementGetAttrObj
	@see PDSElementRemoveAttrObj 
*/
typedef CosObj PDSAttrObj;


/**
 *	PDSMCR
 */
typedef CosObj PDSMCR;


/**
	Represents marked content, which are portions of the graphic content of a PDF document that may
	be included in the document's logical structure hierarchy. This type is identical with the
	PDFEdit layer type PDEContainer.
	@note The write functions in the <code>PDSEdit</code> API are not available in Adobe Reader.
 */
typedef PDEContainer PDSMC;


/**
 *	PDSOBJR
 */
typedef CosObj PDSOBJR;


/**
	Represents mappings of structural element types present in a PDF document to standard
	element types having similar uses. There is one PDSClassMap per document, associated
	with the PDSTreeRoot.
	@note The write functions in the <code>PDSEdit</code> API are not available in Adobe Reader.
 */
typedef CosObj PDSRoleMap;


/**
	Associates class identifiers, which are names, with objects of type PDSAttrObj. Structural
	elements maintain a list of names identifying classes to which they belong. The associated
	attributes are thus shared by all structural elements belonging to a given class. There is one
	class map per document, associated with the PDSTreeRoot.
	@note The write functions in the <code>PDSEdit</code> API are not available in Adobe Reader.
 */
typedef CosObj PDSClassMap;

/** 
	An opaque pointer type to a marked content reference handle.
*/
#ifdef __cplusplus

class PDSMCRefRec;
typedef PDSMCRefRec* PDSMCRef; 

#else

typedef struct _t_PDSMCRef *PDSMCRef;

#endif // __cplusplus


/*------------------------------------------------------------------------
	PDS Attributes.
------------------------------------------------------------------------*/


/*------------------------------------------------------------------------
	PDS Flags and Enums.
------------------------------------------------------------------------*/


/** PDS object types. */
typedef enum {
	/** */
	kPDSElement,
	/** */
	kPDSAttrObj,
	/** */
	kPDSMCR,
	/** */
	kPDSMC,	
	/** */
	kPDSRoleMap,
	/** */
	kPDSClassMap,
	/** */
	kPDSLastType
} PDSType;

/*----------------------------------------------------------------------
    Description of how an MC kid is included in its parent
----------------------------------------------------------------------*/

/**
	Information about how a marked content PDS object (a marked 
	content kid) is included in its parent. 
	@see PDSMCGetInfo 
*/
typedef struct _t_PDSMCInfo
{
    /** The size of this data structure. */
    ASSize_t  size;
    /** The MCID of the content kid in question. */
    ASInt32   mcid;
    /** If <code>true</code>, content is contained within the page's content stream, and the
       fields <code>containingStm</code> and <code>stmOwner</code> are meaningless.
       If <code>false</code>, content is contained within some other content stream,
       such as a Form XObject or an annotation. */
    ASBool    directContent;
    /** The stream object containing the marked content in question. */
    CosObj    containingStm;
    /** The object owning the stream, as defined by the StmOwn key in an OBJR. */
    CosObj    stmOwner;
    /** The page on which the marked content is drawn, whether directly as part of
       page content or indirectly by being in a Form XObject or annotation drawn
       on that page .
	*/
    CosObj page;    
}
PDSMCInfo, *PDSMCInfoP;

/*----------------------------------------------------------------------*/

/* Handy defines for inserting at the beginning and end of lists */
#define kPDSBeforeFirst	((ASInt32) -1)
#define kPDSAfterLast	(ASMAXInt32 - 1)

/*----------------------------------------------------------------------*/
/* Additions for "UserProperties" structure elements */
/**
	Callback for PDSElementEnumUserPropertiesAsASText().

	@param propName The property's name.
	@param propVal The property's value, formatted for display.
	@param clientData Client-supplied data passed in to PDSElementEnumUserPropertiesAsASText().
	@return <code>true</code> to continue enumeration, <code>false</code> to halt.
	@ingroup Enumerators
*/
typedef ACCBPROTO1 ASBool (ACCBPROTO2 *PDSElementEnumUserPropertiesAsASTextProc)(ASText propName, ASText propVal, void *clientData);

/**
	A callback for PDSElementEnumUserPropertiesAsCosObj().

	@param propDict The property dictionary, containing the property's name and its value.
	@param clientData Client-supplied data passed in to PDSElementEnumUserPropertiesAsASText().
	@return <code>true</code> to continue enumeration, <code>false</code> to halt.
	@ingroup Enumerators
*/
typedef ACCBPROTO1 ASBool (ACCBPROTO2 *PDSElementEnumUserPropertiesAsCosObjProc)(CosObj propDict, void *clientData);

/**
	A callback for PDDocEnumPDSElementsWithUserProperties() and	PDSElementEnumKidsWithUserProperties().

	@param elem A PDS element that contains user properties, attributes, and classes.
	@param closestAncestorWithUserProperties The nearest PDS element
 		above <code>elem</code> in the structure tree that also contains user
 		properties, attributes, and classes, or CosNewNull() if no
		such element exists.
	@param clientData Client-supplied data passed in to PDDocEnumPDSElementsWithUserProperties() and PDSElementEnumKidsWithUserProperties().
	@return <code>true</code> to continue enumeration, <code>false</code> to halt.
	@ingroup Enumerators
*/
typedef ACCBPROTO1 ASBool (ACCBPROTO2 *EnumElementsWithUserPropertiesProc)(PDSElement elem, PDSElement closestAncestorWithUserProperties, void *clientData);

/** 
	Information about XML labels for exporting user properties.
        
	<TABLE rules="all" cellspacing="1">
	<TR><TH>Tag</TH><TH>Name</TH></TR>
	<TR><TD><code>nodeTag</code></TD><TD><code>"xyz_Node"</code></TD></TR>
	<TR><TD><code>nodeNameTag</code></TD><TD><code>"xyz_NodeName"</code></TD></TR>
	<TR><TD><code>propTag</code></TD><TD><code>"xyz_Property"</code></TD></TR>
	<TR><TD><code>propNameTag</code></TD><TD><code>"xyz_PropertyName"</code></TD></TR>
	<TR><TD><code>propValTag</code></TD><TD><code>"xyz_Value"</code></TD></TR>
	</TABLE>

	@example	&lt;xyz_Node xyz_NodeName = "StructElem"&gt;
			<p>	   &lt;xyz_Property xyz_PropertyName = "Part Number" xyz_Value = "123"/&gt;</p>
			<p>	   &lt;xyz_Property xyz_PropertyName = "Cost"    xyz_Value = "$4.99"/&gt;</p>
			<p>&lt;/xyz_Node&gt;   </p>
*/
typedef struct _t_PDUserPropertiesXMLLabels 
{  // tells us what to use for the xml node types and attribute names
    ASSize_t size;   // Size of the structure in bytes 
	ASText rootTag;
	ASText nodeTag;
	ASText propTag;
	ASText classTag;
	ASText classDefTag;
	ASText nodeNameTag;
	ASText propNameTag;
	ASText propValueTag;
	ASText propTypeTag;
	ASText propFormatTag;
	ASText propHiddenTag;
	ASText classNameTag;
	ASText classDefNameTag;
} *PDUserPropertiesXMLLabels;

#endif /* _H_PDSExpT */

