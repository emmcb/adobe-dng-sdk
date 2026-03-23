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

 PDExpT.h

 - Types, macros, structures, etc. required to use the PDModel HFT.

*********************************************************************/

#ifndef _H_PDExpT
#define _H_PDExpT

#include "ASExpT.h"
#include "CosExpT.h"
#include "PDBasicExpT.h"
#include "ASExtraExpT.h"
#include "AcroColorExpT.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
	A <code>0</code>-based page number for use in AVPageView and AVDoc methods. 
	Negative for special values. 
	@see AVDocGetPageText 
	@see AVDocGetViewDef 
	@see AVPageViewGetFirstVisiblePageNum 
	@see AVPageViewGetLastVisiblePageNum 
	@see AVPageViewGetNextView 
	@see AVPageViewGetPageNum 
	@see AVPageViewGetSelectedAnnotPageNum 
	@see AVPageViewGetVisibleAnnotPage 
	@see AVPageViewGoTo 
	@see AVPageViewPageNumIsVisible 
	@see AVPageViewSetPageNum 
	@see AVDocSelectionGetAVRectProc 
	@see AVSelectionPageRangeEnumProc
*/
typedef ASInt32 PDPageNumber; 

/**
	A flag value for use in PDDocInsertPagesParams(). 
	@see PDDocInsertPages
*/
typedef ASUns16 PDSmallFlagBits;

/** 
	An unsigned measurement of a font characteristic (for example, width).
*/
typedef ASUns16 PDFontMetric; 

/**
	An italic angle value in degrees, for use in PDFontMetrics. 
*/
typedef ASInt16 PDFontAngle; 

/**
	A font offset value, for use in PDFontMetrics. 
*/
typedef ASInt16 PDFontOffset;

/**
	A font metric value (which is never negative), for use in PDFontMetrics. 	
*/
typedef ASInt16 PDiFontMetric; 

/**
	A signed measurement of an image offset, for use in PDImageAttrs. 	
*/
typedef ASInt32 PDImageScalar; 

/** A <code>signed int</code> (which is never negative), for historical reasons. */
typedef ASInt16 PDDocVersion; 

/**
	A <code>signed int</code> (which is never negative), for historical reasons.
	@see PDDocClearFlags 
	@see PDDocGetFlags 
	@see PDDocSetFlags 
*/
typedef ASInt32 PDDocFlags; 

/** */
typedef ASUns16 PDCharOffset;

/**
	A numeric count value for use in PDImageAttrs. 
*/
typedef ASInt32 PDCount; 


/************************************************************************************\
|*																					*|
|*									PDAction										*|
|*																					*|
\************************************************************************************/

/**
	Actions are what happens when a user clicks on a link or bookmark. In addition, the
	Acrobat viewer allows a document to have an action that is executed automatically
	when the document is opened. Applications can also support actions in custom
	annotation types they add.

	@see PDActionNew
	@see PDActionNewFromDest
	@see PDActionNewFromFileSpec
	@see PDLinkAnnotGetAction
	@see PDBookmarkGetAction
	@see PDDocGetOpenAction
	@see PDActionDestroy
*/
typedef OPAQUE_64_BITS PDAction;
/* Forward declarations */
typedef struct _t_PDActionHandler *PDActionHandler;
typedef struct _t_PDActionClipboardData *PDActionClipboardData;

/**
	Used to store PDAction data for copy and paste operations. 
	
	@see PDActionHandlerCanPasteProc 
	@see PDActionHandlerCopyProc 
	@see PDActionHandlerDestroyDataProc 
	@see PDActionHandlerPasteProc
*/
typedef void *PDActionHandlerData;

/**
	A callback for PDActionHandler. It returns an ASAtom indicating 
	the action type for which the handler is responsible. Types 
	are defined by the client when registering the action handler. 
	
	@param pdah The action handler whose type is returned.
	@return The action type for which this handler is responsible. 
	@see PDRegisterActionHandler 
*/
typedef ACCBPROTO1 ASAtom (ACCBPROTO2 *PDActionHandlerGetTypeProc)(PDActionHandler pdah);

/**
	(Optional) A callback for PDActionHandler. It returns <code>true</code> if 
	the copy operation is expected to succeed. It tests, for example, 
	whether copying is allowed by document permissions. 

	@param pdah The action handler. 
	@param action The action object.
	@return <code>true</code> if the action can be copied, <code>false</code> if it cannot. 
	@see PDActionHandlerCanPasteProc 
	@see PDActionHandlerCopyProc 
	@see PDActionHandlerDestroyDataProc 
	@see PDActionHandlerPasteProc 
	@see PDActionCanCopy 
	@see PDRegisterActionHandler 

	@note The handler is not expected to test other actions (if any) in the action chain. 
*/
typedef ACCBPROTO1 ASBool (ACCBPROTO2 *PDActionHandlerCanCopyProc)(
	PDActionHandler pdah, PDAction action);

/**
	(Optional) A callback for PDActionHandler. It copies data from 
	an action object to a new data structure, from which it 
	can be pasted to a new document. The PDActionHandlerData 
	does not store any information related to a Next action. 
	Rebuilding the action chain is the responsibility of the 
	caller and can be ignored by the PDActionHandler. 
	@param pdah The action handler. 
	@param action The action object.
	@return The new structure containing the action data. 
	@see PDActionHandlerCanCopyProc 
	@see PDActionHandlerCanPasteProc 
	@see PDActionHandlerDestroyDataProc 
	@see PDActionHandlerPasteProc 
	@see PDActionCopy 
	@see PDRegisterActionHandler 
*/
typedef ACCBPROTO1 PDActionHandlerData(ACCBPROTO2 *PDActionHandlerCopyProc)(
	PDActionHandler pdah, PDAction action);

/**
	(Optional) A callback for PDActionHandler. It returns <code>true</code> if 
	the paste operation is expected to succeed. It tests, for example, 
	whether pasting is allowed by document permissions. 
	@param pdah The action handler. 
	@param dest The destination document. 
	@param data The action data to test.
	@return <code>true</code> if the action can be pasted to the document, <code>false</code> 
	if it cannot. 
	@see PDActionHandlerCanCopyProc 
	@see PDActionHandlerCopyProc 
	@see PDActionHandlerDestroyDataProc 
	@see PDActionHandlerPasteProc 
	@see PDActionCanPaste 
	@see PDRegisterActionHandler 

	@note The handler is not expect to test other actions (if any) in the action chain. 
*/
typedef ACCBPROTO1 ASBool (ACCBPROTO2 *PDActionHandlerCanPasteProc)(
	PDActionHandler pdah, PDDoc dest, PDActionHandlerData data);

/**
	(Optional) A callback for PDActionHandler. It creates a new PDAction 
	in the destination document using data that was placed in 
	a PDActionClipboardA data structure using the PDActionCopy() 
	function, and returns the new action object. 
	@param pdah The action handler. 
	@param dest The destination document. 
	@param data The action data to paste.
	@return The new action object containing the pasted data and associated 
	with the document. 
	@see PDActionHandlerCanCopyProc 
	@see PDActionHandlerCanPasteProc 
	@see PDActionHandlerCopyProc 
	@see PDActionHandlerDestroyDataProc 
	@see PDActionPaste 
	@see PDRegisterActionHandler 
*/
typedef ACCBPROTO1 PDAction (ACCBPROTO2 *PDActionHandlerPasteProc)(
	PDActionHandler pdah, PDDoc dest, PDActionHandlerData data);

/**
	A callback for PDActionHandler. It destroys data copied into 
	an action clipboard data structure after it has been successfully 
	pasted to a new document. 
	@param pdah The action handler. 
	@param data The action data to destroy.
	@see PDActionHandlerCanCopyProc 
	@see PDActionHandlerCanPasteProc 
	@see PDActionHandlerCopyProc 
	@see PDActionHandlerPasteProc 
	@see PDActionPaste 
	@see PDRegisterActionHandler 
*/
typedef ACCBPROTO1 void (ACCBPROTO2 *PDActionHandlerDestroyDataProc)(
	PDActionHandler pdah, PDActionHandlerData data);

/**
	(Optional) A callback for PDActionHandler. It destroys the action 
	handler structure when the application no longer requires 
	it. The handler should destroy any dynamic memory stored 
	in the <code>userData</code> field. 
	@param pdah The action handler to destroy.
	@see PDRegisterActionHandler 
*/
typedef ACCBPROTO1 void (ACCBPROTO2 *PDActionHandlerDestroyProc)(
	PDActionHandler pdah);

/**
	A data structure containing callbacks that implement an action 
	manager. The callbacks provide copy-and-paste functionality 
	for a particular type of action. 
	@see PDRegisterActionHandler 
*/
typedef struct _t_PDActionHandler {

	/** The size of the data structure. It must be set to <code>sizeof(PDAnnotHandlerRec)</code>. */
	ASSize_t size;

	/** A pointer to a block of user-supplied data. */
	void *userData;

	/** */
	PDActionHandlerGetTypeProc GetType;

	/** */
	PDActionHandlerCanCopyProc CanCopy;

	/** */
	PDActionHandlerCopyProc Copy;

	/** */
	PDActionHandlerCanPasteProc CanPaste;

	/** */
	PDActionHandlerPasteProc Paste;

	/** */
	PDActionHandlerDestroyDataProc DestroyData;

	/** */
	PDActionHandlerDestroyProc Destroy;
} PDActionHandlerRec;

/************************************************************************************\
|*																					*|
|*							PDAnnot, PDTextAnnot, PDLinkAnnot						*|
|*																					*|
|*	Opaque designators for PDF annotation objects.
|*	You may freely copy or destroy an instance of PDAnnot.
|*	Two instances of a PDAnnot designating the same annotation are guaranteed to
|*	contain identical bits.															*|
|*																					*|
\************************************************************************************/

/**
	An annotation on a page in a PDF file. Acrobat viewers have two built-in annotation
	types: PDTextAnnot and PDLinkAnnot. Physical attributes of the annotation can
	be set and queried. Plug-ins add movie and Widget (form field) annotations.
	Developers can define new annotation subtypes by creating new annotation handlers.
	
	@see AVPageViewIsAnnotAtPoint
	@see PDAnnotFromCosObj
	@see PDPageAddNewAnnot
	@see PDPageCreateAnnot
	@see PDPageGetAnnot
	@see PDPageRemoveAnnot
*/
typedef OPAQUE_64_BITS PDAnnot;

/**
	A PDF text annotation on a page in a PDF file. You can use any PDAnnot method on
	a PDTextAnnot.
	<p>Applications can:</p>
	<ul>
	<li>Get and set attributes including the rectangle, textual contents, and whether 
	the annotation is open.</li>
	<li>Create new text annotations, and delete or move existing ones using PDAnnot
	methods.</li>
	<li>Manipulate the behavior of text annotations by modifying the Text Annotation
	Handler.</li>
	</ul>
	
	<p>To obtain a PDF text annotation, use any of the PDAnnot calls, followed by CastToPDTextAnnot(). The annotation
	passed to CastToPDTextAnnot() must be a text annotation; it will not convert other
	annotation types into text annotations.</p>
	
	@see CastToPDTextAnnot 
	@see PDPageRemoveAnnot
*/
typedef OPAQUE_64_BITS PDTextAnnot;

/**
	A link annotation on a page in a PDF file. You can use any PDAnnot method on a
	PDLinkAnnot.
	
	<p>Applications can:</p>
	<ul>
	<li>Get and set the bounding rectangle and color using PDAnnot methods.</li>
	<li>Get and set the action that occurs when the link is activated, and the link's border,
	using PDLinkAnnot methods.</li>
	<li>Create new link annotations and delete existing ones using the PDPage methods.</li>
	</ul>

	<p>To obtain a link annotation, use any of the PDAnnot calls, followed by CastToPDLinkAnnot(). The annotation passed
	to CastToPDLinkAnnot() must be a link annotation; other annotation types are not
	converted into link annotations.</p>
		
	@see CastToPDLinkAnnot
	@see PDPageRemoveAnnot
*/
typedef OPAQUE_64_BITS PDLinkAnnot;

/** If there is no annotation handler, the annotation is invisible. 
	@ingroup PDAnnotFlags
*/
#define pdAnnotInvisible		0x01		

/** The annotation is not visible and does not print. 
	@ingroup PDAnnotFlags
*/
#define pdAnnotHidden			0x02		

/** The annotation prints. 
	@ingroup PDAnnotFlags
*/
#define pdAnnotPrint			0x04		

/** The annotation does not zoom with the view. 
	@ingroup PDAnnotFlags
*/
#define pdAnnotNoZoom			0x08		

/** The annotation does not rotate with the page. 
	@ingroup PDAnnotFlags
*/
#define pdAnnotNoRotate			0x10		

/** The annotation does not view but can print. 
	@ingroup PDAnnotFlags
*/
#define pdAnnotNoView			0x20		

/** The annotation does not interact with the user. 
	@ingroup PDAnnotFlags
*/
#define pdAnnotReadOnly			0x40		

/** The annotation does not move or resize with the view. Currently only form fields respect this flag.
	If the annotation is locked, the user cannot delete, move or change its associated form field's properties.
	@ingroup PDAnnotFlags
*/
#define pdAnnotLock				0x80		

/** A mouse-over or selection causes the <code>noView</code> bit to toggle.
	@ingroup PDAnnotFlags
*/
#define pdAnnotToggleNoView		0x100		

/** If the annotation is content-locked, the user can not change its content key.
	@ingroup PDAnnotFlags
*/
#define pdAnnotLockContents		0x200

/** A place holder used only at runtime. Do not set this bit in PDF/FDF files. 
	@ingroup PDAnnotFlags
*/
#define pdAnnotSequenceAdjust	0x80000000	
											

/**
	Casts a link annotation or a text annotation to a generic 
	annotation. 
	@param a The link annotation or text annotation to cast.
	@see CastToPDLinkAnnot 
	@see CastToPDTextAnnot 
*/
#define CastToPDAnnot(a) *(PDAnnot *)&(a)
/**
	Casts a link annotation or a generic annotation to a text 
	annotation. 
	@param a The link annotation or generic annotation to 
	cast.
	@see CastToPDAnnot 
	@see CastToPDLinkAnnot 
*/
#define CastToPDTextAnnot(a) *(PDTextAnnot *)&(a)
/**
	Casts a generic annotation or a text annotation to a link 
	annotation. 
	@param a The generic annotation or text annotation to 
	cast.
	@see CastToPDAnnot 
	@see CastToPDTextAnnot 
*/
#define CastToPDLinkAnnot(a) *(PDLinkAnnot *)&(a)

/** It is okay to summarize annotations.
	@ingroup AnnotationOperations	
*/
#define PDAnnotOperationSummarize	0x0001 

/** It is okay to filter annotations.
	@ingroup AnnotationOperations	
*/
#define PDAnnotOperationFilter	0x0002 

/** It is okay to manage annotations.
	@ingroup AnnotationOperations	
*/
#define PDAnnotOperationManager	0x0004 

/** Allows modifying this annotation type in a write-protected document.
	@ingroup AnnotationOperations	
*/
#define PDAnnotIgnorePerms		0x0008 

/** When creating a flattened page include this annot
	@ingroup AnnotationOperations	
*/
#define PDAnnotOperationFlatten		0x0010

/** All operations are allowed. 
	@ingroup AnnotationOperations	
*/
#define PDAnnotOperationAll		0xFFFF

/**
	Information associated with an annotation. 
	@see PDAnnotHandlerDeleteAnnotInfoProc 
	@see PDAnnotHandlerGetAnnotInfoProc 
	@see PDRegisterAnnotHandler 
*/
typedef struct _PDAnnotInfoRec {
	/** The size of the data structure. It must be set to <code>sizeof(PDAnnotInfoRec)</code>. */
	ASSize_t size;

	/** The allowed operations.
		@see AnnotationOperations
	*/
	ASFlagBits nOperationFlags;

	/** The author of this annotation. */
	unsigned char *cAuthor;	

	/** The associated text for this annotation. */
	unsigned char *cContents;

	/** The modification date of this annotation. */
	unsigned char *cDate;	

	/** The layer of this annotation. */
	ASFixed fxLayer;			
} PDAnnotInfoRec, *PDAnnotInfo;

/* It is assumed that the cAuthor and cContents passed back by the call
** will be in either PDFDocEncoding or UnicodeEncoding (with the 0xFEFF
** prefix). The date should be the standard date format as specified in
** section 7.2, "Date", of the <i>PDF Reference</i> Manual. */
#define PDAnnotInfoInit(x)				\
	do { \
	ASmemset(x, 0, sizeof(PDAnnotInfoRec));	\
	x->size = sizeof(PDAnnotInfoRec); \
	x->fxLayer = fixedTwo; \
	} while (0)

/**
	A data structure containing callbacks that implement an annotation 
	manager. The callbacks implement the annotation manager 
	functions (for example, view, delete, or export the annotations 
	of a document as a list, sorted by type, author, or date). 
	
	<p>To fully use a PDAnnotHandler, the AVAnnotHandler associated 
	with this annotation must have its AVAnnotHandlerGetInfoProc() 
	and AVAnnotHandlerDeleteInfoProc() callbacks defined.</p> 
	@see PDRegisterAnnotHandler 
*/
typedef struct _t_PDAnnotHandler *PDAnnotHandler;

/**
	Used to store PDAnnot data for copy and paste operations. 
	
	@see PDAnnotCanPaste 
	@see PDAnnotCopy 
	@see PDAnnotDestroyClipboardData 
	@see PDAnnotPaste
*/
typedef struct _t_PDAnnotClipboardData *PDAnnotClipboardData;

/** Opaque data used by PDAnnotHandlers. */
typedef void *PDAnnotHandlerClipboardData;

/**
	PDAnnotPrintOp is passed to the PDAnnotHandlerGetPrintAppearanceProc()
	callback to specify the type of print operation being performed.
	
	@see PDAnnotHandlerGetPrintAppearanceProc
*/
enum {

	/** A standard print operation. */
	kPDAnnotPrintStandard = 1,	

	/** The user selected Form Fields Only. */
	kPDAnnotPrintVariableData	
};
typedef ASEnum16 PDAnnotPrintOp;

/**
	A callback for PDAnnotHandler. It gets the annotation information
	for an annotation.
	@param pdanh IN/OUT The annotation handler responsible for this
	annotation.
	@param pdan IN/OUT The annotation for which information is obtained.

	@param pdpage IN/OUT The page associated with the annotation for
	which information is obtained. If the page associated with
	the annotation is not known, pass <code>NULL</code>.
	@return The annotation information, described in a PDAnnotInfo structure.

	@see AVAnnotHandlerGetInfoProc
	@see PDAnnotHandlerDeleteAnnotInfoProc
	@see PDRegisterAnnotHandler
*/
typedef ACCBPROTO1 PDAnnotInfo (ACCBPROTO2 *PDAnnotHandlerGetAnnotInfoProc)(
	PDAnnotHandler pdanh, PDAnnot pdan, PDPage pdpage);

/**
	(Optional) A callback for PDAnnotHandler. It deletes information
	associated with an annotation. It frees all the memory associated
	with the annotation information.
	@param pdanh IN/OUT The annotation handler responsible for this
	annotation.
	@param information IN/OUT Information associated with the annotation.

	@see AVAnnotHandlerDeleteInfoProc
	@see PDAnnotHandlerGetAnnotInfoProc
	@see PDRegisterAnnotHandler
*/
typedef ACCBPROTO1 void (ACCBPROTO2 *PDAnnotHandlerDeleteAnnotInfoProc)(
	PDAnnotHandler pdanh, PDAnnotInfo info);

/**
	A callback for PDAnnotHandler. It gets an ASAtom indicating the
	annotation type for which the handler is responsible. This
	corresponds to the annotation's Subtype key in the PDF file.

	@param pdanh IN/OUT The annotation handler whose type is returned.

	@return The annotation type for which this handler is responsible.

	@see PDAnnotHandlerGetAnnotInfoProc
	@see PDRegisterAnnotHandler
*/
typedef ACCBPROTO1 ASAtom (ACCBPROTO2 *PDAnnotHandlerGetTypeProc)(PDAnnotHandler pdanh);

/**
	A callback for PDAnnotHandler. It determines whether an annotation
	is exported.

	@param pdanh IN/OUT The annotation handler of the annotation type
	to export.
	@param src IN/OUT The annotation that may be exported.
	@param dst IN/OUT A copy of <code>src</code>, which is actually exported.
	@return <code>true</code> to export <code>dst</code>, <code>false</code> to not export <code>dst</code>.
	@see PDAnnotWillPrintProc
	@see PDDocWillImportAnnotProc
	@see PDRegisterAnnotHandler

	@note This is a different callback than PDDocWillImportAnnotCallback().
*/
typedef ACCBPROTO1 ASBool (ACCBPROTO2 *PDDocWillExportAnnotProc)(PDAnnotHandler pdanh,
	PDAnnot src, PDAnnot dst);

/**
	A callback for PDAnnotHandler. It determines whether an annotation
	will be imported.

	@param pdanh IN/OUT The annotation handler of the annotation type
	to import.
	@param doc IN/OUT The document into which annotations may be imported.
	@param pdpage IN/OUT The page in which the annotation may be imported.
	@param annot IN/OUT The annotation that may be imported.
	@return <code>true</code> to import annot, <code>false</code> to not import annot.
	@see PDAnnotWillPrintProc
	@see PDDocWillExportAnnotProc
	@see PDRegisterAnnotHandler

	@note This is a different callback than PDDocWillImportAnnotCallback().
*/
typedef ACCBPROTO1 ASBool (ACCBPROTO2 *PDDocWillImportAnnotProc)(PDAnnotHandler pdanh,
	PDDoc doc, PDPage pdpage, PDAnnot annot);

/**
	A callback for PDAnnotHandler. This method is called to determine 
	whether an annotation is printed. 
	@param pdanh The annotation handler of the annotation 
	type to print. 
	@param annot The annotation to print.
	@return <code>true</code> if the annotation is printed, <code>false</code> if the annotation 
	is not printed. 
	@see PDDocWillExportAnnotProc 
	@see PDDocWillImportAnnotProc 
	@see PDRegisterAnnotHandler 
*/
typedef ACCBPROTO1 ASBool (ACCBPROTO2 *PDAnnotWillPrintProc)(PDAnnotHandler pdanh,
	PDAnnot annot);

/**
	A callback for PDAnnotHandler. It gets the annotation handler
	information flags, which indicate the operations allowed with annotations
	of this type.
	@param pdanh IN/OUT The annotation handler responsible for the
	annotation.
	@param pdan IN/OUT The annotation.
	@return The operations allowed. 
	<p>The value is an OR of the following flags:</p>

	<TABLE rules="all" cellspacing="1">
	<TR><TH>Operation</TH><TH>Description</TH></TR>
	<TR><TD><code>PDAnnotOperationSummarize</code></TD><TD>It is okay to summarize annotations.</TD></TR>
	<TR><TD><code>PDAnnotOperationFilter</code></TD><TD>It is okay to filter annotations.</TD></TR>
	<TR><TD><code>PDAnnotOperationManager</code></TD><TD>It is okay to manage annotations.</TD></TR>
	<TR><TD><code>PDAnnotIgnorePerms</code></TD><TD>Allow modifying this annotation type in a write-protected document.</TD></TR>
	<TR><TD><code>PDAnnotOperationAll</code></TD><TD>All operations are allowed.</TD></TR>
	</TABLE>

	@see PDAnnotHandlerGetAnnotInfoProc
	@see PDRegisterAnnotHandler
*/
typedef ACCBPROTO1 ASFlagBits (ACCBPROTO2 *PDAnnotHandlerGetAnnotInfoFlagsProc)(
	PDAnnotHandler pdanh, PDAnnot pdan);

/** (Optional) A callback for PDAnnotHandler. It returns <code>true</code> if the copy operation is expected
	to succeed. It tests, for example, whether copying is allowed by document permissions.
	@param pdanh IN/OUT The annotation handler responsible for this annotation.
	@param sourcePage The page the annotation is on.
	@param pdan IN/OUT The annotation object.
	@return <code>true</code> if the annotation can be copied, <code>false</code> if it cannot.

	@see PDAnnotHandlerCanPasteProc
	@see PDAnnotHandlerCopyProc
	@see PDAnnotHandlerPasteProc
	@see PDAnnotHandlerDestroyDataProc
	@see PDAnnotCanCopy
	@see PDRegisterAnnotHandlerCanCopy  
*/
typedef ACCBPROTO1 ASBool (ACCBPROTO2 *PDAnnotHandlerCanCopyProc)(
	PDAnnotHandler pdanh, PDPage sourcePage, PDAnnot pdan);

/**
	(Optional) A callback for PDAnnotHandler. It copies data from 
	the annotation object to a new clipboard structure, and 
	returns the clipboard structure. 
	@param pdanh The annotation handler responsible for this 
	annotation. 
	@param sourcePage The page containing the annotation to 
	be copied. 
	@param pdan The annotation object.
	@return The clipboard structure containing the copied data. 
	@see PDAnnotHandlerCanCopyProc 
	@see PDAnnotHandlerCanPasteProc 
	@see PDAnnotHandlerCopyProc 
	@see PDAnnotHandlerDestroyDataProc 
	@see PDAnnotHandlerPasteProc 
	@see PDAnnotCopy 
	@see PDRegisterAnnotHandler 
*/
typedef ACCBPROTO1 PDAnnotHandlerClipboardData(ACCBPROTO2 *PDAnnotHandlerCopyProc)(
	PDAnnotHandler pdanh, PDPage sourcePage, PDAnnot pdan);

/**
	(Optional) A callback for PDAnnotHandler. It returns <code>true</code> if 
	the paste operation is expected to succeed. It tests whether 
	data from an annotation that has been copied to a clipboard 
	can be pasted to a location on a page. Pasting can be disallowed 
	by document permissions, or because the annotation cannot 
	be accurately reproduced in the destination document. 
	@param pdanh The annotation handler responsible for this 
	annotation. 
	@param destPage The page to which the annotation would 
	be pasted. 
	@param center The location for the center of the annotation 
	on the destination page, or a <code>NULL</code> pointer to center the 
	annotation on the destination page's crop box. 
	@param data The copied annotation data to test.
	@return <code>true</code> if the annotation data can be pasted, <code>false</code> otherwise. 
	
	@see PDAnnotHandlerCanCopyProc 
	@see PDAnnotHandlerCopyProc 
	@see PDAnnotHandlerPasteProc 
	@see PDAnnotHandlerDestroyDataProc 
	@see PDAnnotCanPaste 
	@see PDRegisterAnnotHandler 
*/
typedef ACCBPROTO1 ASBool (ACCBPROTO2 *PDAnnotHandlerCanPasteProc)(
	PDAnnotHandler pdanh, PDPage destPage, const ASFixedPoint *center, PDAnnotHandlerClipboardData data);

/**
	(Optional) A callback for PDAnnotHandler. It creates a new annotation 
	on the specified page using clipboard data generated by 
	PDAnnotCopy(). 
	@param pdanh The annotation handler responsible for this 
	annotation. 
	@param destPage The page to which the annotation is pasted. 
	@param center The location for the center of the annotation 
	on the destination page, or a <code>NULL</code> pointer to center the 
	annotation in the destination page's crop box. 
	@param data The copied annotation data to paste.
	@return The new annotation object associated with the page's document. 
	
	@see PDAnnotHandlerCanCopyProc 
	@see PDAnnotHandlerCanPasteProc 
	@see PDAnnotHandlerCopyProc 
	@see PDAnnotHandlerDestroyDataProc 
	@see PDAnnotCopy 
	@see PDAnnotPaste 
	@see PDRegisterAnnotHandler 
*/
typedef ACCBPROTO1 PDAnnot (ACCBPROTO2 *PDAnnotHandlerPasteProc)(
	PDAnnotHandler pdanh, PDPage destPage, const ASFixedPoint *center, PDAnnotHandlerClipboardData data);

/**
	(Optional) A callback for PDAnnotHandler. It destroys data from 
	an annotation that has been copied to a clipboard. This 
	callback may be executed regardless of how many times (if 
	any) the annotation was pasted. 
	@param pdanh The annotation handler responsible for this 
	annotation. 
	@param data The copied annotation data to destroy.
	@see PDAnnotHandlerCanCopyProc 
	@see PDAnnotHandlerCanPasteProc 
	@see PDAnnotHandlerCopyProc 
	@see PDAnnotHandlerPasteProc 
	@see PDAnnotDestroyClipboardData 
	@see PDRegisterAnnotHandler 
*/
typedef ACCBPROTO1 void (ACCBPROTO2 *PDAnnotHandlerDestroyDataProc)(
	PDAnnotHandler pdanh, PDAnnotHandlerClipboardData data);

/**
	(Optional) A callback for PDAnnotHandler. It destroys the annotation 
	handler structure when the application no longer requires 
	it. The handler should destroy any dynamic memory stored 
	in the <code>userData</code> field. 
	@param pdanh The annotation handler to destroy.
	@see PDRegisterAnnotHandler 
*/
typedef ACCBPROTO1 void (ACCBPROTO2 *PDAnnotHandlerDestroyProc)(
	PDAnnotHandler pdanh);

/**
	A callback for PDAnnotHandler. It gets the heel point (the focus 
	or starting point) for the annotation. For a rectangular 
	annotation, this is usually the top left corner. 
	@param pdanh The annotation handler whose heel point is 
	obtained. 
	@param pdan The annotation object. 
	@param point (Filled by the method) The heel point for 
	the annotation.
	@see PDRegisterAnnotHandler 
*/
typedef ACCBPROTO1 void (ACCBPROTO2 *PDAnnotHandlerGetHeelPointProc)(
	PDAnnotHandler pdanh, PDAnnot pdan, ASFixedPoint *point);

/**
	A callback for PDAnnotHandler. It is called by the host application 
	to obtain a print appearance (a Form XObject). If this callback 
	is not implemented, the default appearance (if any) is used. 
	
	@param pdanh The annotation handler whose print appearance 
	is obtained. 
	@param pdan The annotation object. 
	@param frAnnot The rectangle in user space in which the 
	annotation will be printed. The handler is free to modify 
	it. 
	@param op The print operation being performed. This is 
	kPDAnnotPrintStandard for a standard print operation, or 
	kPDAnnotPrintVariableData if the user selected Form Fields 
	Only.
	@return The Form XObject representing the print appearance of the 
	annotation. 
	@see PDRegisterAnnotHandler 
*/
typedef ACCBPROTO1 CosObj (ACCBPROTO2 *PDAnnotHandlerGetPrintAppearanceProc)(
	PDAnnotHandler pdanh, PDAnnot pdan, ASFixedRect *frAnnot, PDAnnotPrintOp op);


/**
	A data structure containing callbacks that implement an annotation 
	manager. The callbacks implement the annotation manager 
	functions (for example, view, delete, or export the annotations 
	of a document as a list, sorted by type, author, or date). 

	<p>To fully use a PDAnnotHandler, the AVAnnotHandler associated 
	with this annotation must have its AVAnnotHandlerGetInfoProc() 
	and AVAnnotHandlerDeleteInfoProc() callbacks defined. </p>
	@see PDRegisterAnnotHandler 
*/
typedef struct _t_PDAnnotHandler {

	/** The size of the data structure. It must be set to <code>sizeof(PDAnnotHandlerRec)</code>. */
	ASSize_t size;

	/** A pointer to a block of user-supplied data. */
	void* userData;

	/** */
	PDAnnotHandlerGetTypeProc GetType;

	/** */
	PDAnnotHandlerGetAnnotInfoProc GetAnnotInfo;

	/** */
	PDAnnotHandlerDeleteAnnotInfoProc DeleteAnnotInfo;

	/** */
	PDDocWillExportAnnotProc WillExportAnnot;

	/** */
	PDDocWillImportAnnotProc WillImportAnnot;

	/** */
	PDAnnotWillPrintProc WillPrintAnnot;

	/** */
	PDAnnotHandlerGetAnnotInfoFlagsProc GetAnnotInfoFlags;

	/** */
	PDAnnotHandlerCanCopyProc CanCopy;

	/** */
	PDAnnotHandlerCopyProc Copy;

	/** */
	PDAnnotHandlerCanPasteProc CanPaste;

	/** */
	PDAnnotHandlerPasteProc Paste;

	/** */
	PDAnnotHandlerDestroyDataProc DestroyData;

	/** */
	PDAnnotHandlerDestroyProc Destroy;

	/** */
	PDAnnotHandlerGetHeelPointProc GetHeelPoint;

	/** */
	PDAnnotHandlerGetPrintAppearanceProc GetPrintAppearance;
} PDAnnotHandlerRec;


/**
	An enumerated data type that specifies the color space in which a color value is specified
	(for example, RGB or grayscale).
*/
enum {

	/** Grayscale. It requires one value entry to specify the color. */
	PDDeviceGray,

	/** Red-Green-Blue color specification. It requires three value entries to specify the color.*/
	PDDeviceRGB,

	/** Cyan-Magenta-Yellow-Black color specification. It requires four value entries to specify the color.*/
	PDDeviceCMYK
};
typedef ASEnum8 PDColorSpace;

/*
** PDColorValue
**
** Value elements must be interpreted according to the color space.
**   PDDeviceGray	- 1 value
**   PDDeviceRGB	- 3 values
**   PDDeviceCMYK	- 4 values
*/

/**
	A data structure representing a color. The number of elements 
	needed in the value field depends on the color space type 
	(specified in the space field). See PDColorSpace for more 
	information. 

	See also AVPrefsType. 
	@see AVAppBeginFullScreen 
	@see AVPageViewGetColor 
	@see AVPageViewSetColor 
	@see PDAnnotGetColor 
	@see PDAnnotSetColor 
	@see PDStyleGetColor
*/
typedef struct _t_PDColorValueRec {

	/** */
	PDColorSpace	space;

	/** */
	ASFixed			value[4];
} PDColorValueRec, *PDColorValue;

typedef const struct _t_PDColorValueRec *PDConstColorValue;

/** An enumerated data type that specifies the type of changes that occurred for the
	PDDocPrintingTiledPage() and PDDocDidChangePages() notifications. Not all
	<code>Did</code> notifications have corresponding <code>Will</code> notifications. 
	@see PDDocDeletePages
	@see PDDocInsertPages
	@see PDDocMovePage
	@see PDPageAddCosContents
	@see PDPageAddCosResource
	@see PDPageRemoveCosContents
	@see PDPageRemoveCosResource
	@see PDPageSetRotate
	@see PDPageSetCropBox
	@see PDPageSetMediaBox
*/
enum {
	/** Page insertion. */
	pdOpInsertPages,

	/** Page deletion. */
	pdOpDeletePages,

	/** Page replacement. */
	pdOpReplacePages,

	/** Page rearrangment. */
	pdOpMovePages,

	/** Page rotation. */
	pdOpRotatePages,

	/** Page cropping. */
	pdOpCropPages,

	/** Only PDDocDidChangePages() exists, not PDDocWillChangePages().*/
	pdOpAddResources,

	/** Only PDDocDidChangePages() exists, not PDDocWillChangePages().*/
	pdOpRemoveResources,

	/** Only PDDocDidChangePages() exists, not PDDocWillChangePages().*/
	pdOpAddContents,

	/** Only PDDocDidChangePages() exists, not PDDocWillChangePages().*/
	pdOpRemoveContents,

	/** Page media box modification. */
	pdOpSetMediaBox,

	/** Page bleed box modification. */
	pdOpSetBleedBox,

	/** Page trim box modification. */
	pdOpSetTrimBox,

	/** Page art box modification. */
	pdOpSetArtBox,

	/** Page tab order modification. */
	pdOpSetTabOrder
};
typedef ASEnum8 PDOperation;

/* "defines" for two old (incorrect) PDOperation names previously used in the plug-in API */
#define pdOpRemoveResource pdOpRemoveResources
#define pdOpAddResource pdOpAddResources

#define PDAnnotMaxDashes			10

/**
	The border's dash pattern is specified by <code>dashArray</code> and 
	<code>dashArrayLen</code>. This is a normal PostScript dash pattern (an 
	array of on/off stroke lengths used cyclically) except that 
	zero is always used as the offset (<i>phase</i>) and the number 
	of array entries is limited to PDAnnotMaxDashes. The number 
	of valid dashArray entries is specified by <code>dashArrayLen</code>; a 
	<code>dashArrayLen</code> of <code>0</code> specifies a solid border. 
	@see PDLinkAnnotGetBorder 
	@see PDLinkAnnotSetBorder
*/
typedef struct _t_PDLinkAnnotBorder
{

  /** */
  ASInt32 hCornerRadius;

  /** */
  ASInt32 vCornerRadius;

  /** */
  ASInt32 width;

  /** */
  ASInt32	dashArrayLen;

  /** */
  ASFixed	dashArray[PDAnnotMaxDashes];
} PDLinkAnnotBorder;

/************************************************************************************\
|*																					*|
|*									PDBookmark										*|
|*																					*|
|*	You may freely copy or destroy an instance of PDBookmark.						*|
|*	Two instances of a PDBookmark designating the same bookmark are guaranteed to 	*|
|*	contain identical bits.															*|
|*																					*|
\************************************************************************************/

/**
	A bookmark on a page in a PDF file. Each bookmark has a title that appears on
	screen, and an action that specifies what happens when a user clicks on the
	bookmark. Bookmarks can either be created interactively by the user through the
	Acrobat viewer's user interface or programmatically generated. The typical action for a
	user-created bookmark is to move to another location in the current document,
	although any action (see PDAction) can be specified.

	@see PDDocGetBookmarkRoot
	@see PDBookmarkAddNewSibling
	@see PDBookmarkAddNewChild
	@see PDBookmarkFromCosObj
	@see PDBookmarkGetByTitle
	@see PDBookmarkGetParent
	@see PDBookmarkGetFirstChild
	@see PDBookmarkGetLastChild
	@see PDBookmarkGetNext
	@see PDBookmarkGetPrev
	@see PDBookmarkDestroy
	@see PDBookmarkUnlink
*/
typedef OPAQUE_64_BITS PDBookmark;

/* 
	Constants that are returned from PDBookmarkGetFlags(). 
	@see PDBookmarkGetFlags
*/
enum 
{
	kPDBookmarkFontNormal		= 0x00,	
	kPDBookmarkFontItalic		= 0x01,	
	kPDBookmarkFontBold			= 0x02,	
	kPDBookmarkFontBoldItalic	= 0x03
};
typedef ASEnum8 PDBookmarkFlags;

/************************************************************************************\
|*																					*|
|*									PDFileSpec										*|
|*																					*|
\************************************************************************************/

/**
	The PDF file specification object. It is used to specify a file in an action (see
	PDAction). A file specification in a PDF file can take two forms:
	
	<ul>
	<li>A single platform-independent path.</li>
	<li>A data structure containing one or more alternative ways to locate the file on different platforms.</li>
	</ul>
	
	PDFileSpec objects can be created from ASPathName objects or from Cos objects.

	@see PDActionGetFileSpec
	@see PDFileSpecNewFromASPath
	@see PDFileSpecFromCosObj
*/
typedef OPAQUE_64_BITS PDFileSpec;

/************************************************************************************\
|*																					*|
|*									PDFileSpecHandler								*|
|*																					*|
\************************************************************************************/

/**
	A callback for PDFileSpecHandler. It creates a file specification 
	from an ASPath. 
	@param fileSpecHandlerObj User-supplied data passed in 
	the call to PDRegisterFileSpecHandler(). 
	@param pdDoc The PDDoc in which the file specification 
	is created. 
	@param path The ASPathName for which a corresponding file 
	specification is created. 
	@param relativeToThisPath A path name relative to which 
	path is interpreted. If <code>NULL</code>, <code>path</code> is assumed to be an absolute path, 
	not a relative path.
	@return The file specification corresponding to the specified ASPathName. 
	
	@see PDFileSpecNewFromASPath 
*/
typedef ACCBPROTO1 PDFileSpec (ACCBPROTO2 *PDFileSpecNewFromASPathProc)(void *fileSpecHandlerObj, PDDoc pdDoc, ASPathName path, ASPathName relativeToThisPath);

/**
	A callback for PDFileSpecHandler. It aquires the ASPath corresponding
	to a file specification.
	@param fileSpecHandlerObj IN/OUT User-supplied data passed in
	the call to PDRegisterFileSpecHandler().
	@param fileSpec IN/OUT The PDFileSpec for which an ASPath is acquired.

	@param relativeToThisPath IN/OUT A path name relative to which
	the PDFileSpec is interpreted. If <code>NULL</code>, <code>fileSpec</code> is assumed
	to be an absolute path, not a relative path.
	@return The ASPathName corresponding to the specified path.
	@see PDFileSpecAcquireASPath
*/
typedef ACCBPROTO1 ASPathName (ACCBPROTO2 *PDFileSpecAcquireASPathProc)(void *fileSpecHandlerObj, PDFileSpec fileSpec, ASPathName relativeToThisPath);

/**
	(Optional) A callback for PDFileSpecHandler. It launches a specified
	file. It is called when the Acrobat viewer encounters a Launch
	(GoTo File) action. If this callback is <code>NULL</code>, no launch
	action is performed.
	@param fileSpecHandlerObj IN/OUT The registered PDFileSpecHandler.
	@param pdDoc IN/OUT The document containing the Launch action.
	@param pdAction IN/OUT The action dictionary.
	@return <code>true</code> if the handler can perform the Launch, <code>false</code> otherwise.
*/
typedef ACCBPROTO1 ASBool (ACCBPROTO2 *PDLaunchActionProc)(void *fileSpecHandlerObj, PDDoc pdDoc, PDAction pdAction);

/**
	A data structure that implements a file specification handler. It contains callbacks that
	implement the file specification handler's functions (converting from a file specification to an
	ASPathName, creating a new file specification from an ASPathName, and launching
	the specified file).
	@see PDRegisterFileSpecHandlerByName
*/
typedef struct _t_PDFileSpecHandler {
	/** Must be <code>sizeof(PDFileSpecHandlerRec)</code>. */
	ASSize_t size;	

	/** */
	PDFileSpecNewFromASPathProc	NewFromASPath;

	/** */
	PDFileSpecAcquireASPathProc	AcquireASPath;

	/** */
	PDLaunchActionProc			LaunchAction;

	/** The file system that is to be used with this file specification handler. */
	ASFileSys					fileSys; 
} PDFileSpecHandlerRec, *PDFileSpecHandler;

/**
	A callback used by PDDocOpen. It is called when an encrypted 
	document is being opened to determine whether the user is 
	authorized to open the file. 

	<p>This callback implements whatever authorization strategy 
	you choose and calls the callbacks of the appropriate security 
	handler (the one that was used to secure the document) to 
	obtain and check authorization data. </p>

	<p>The PDAuthProc() must call the security handler's PDCryptGetAuthDataProc() 
	to obtain whatever authorization data is needed (such as 
	a password), then call PDDocAuthorize() (which is mostly a 
	call to the security handler's PDCryptAuthorizeProc()) to 
	determine whether this data authorizes access to the file 
	(for example, it verifies if the user provided the correct password). 
	The PDAuthProc() must also free the authorization data by 
	calling the security handler's PDCryptFreeAuthDataProc() (or 
	ASfree(), if the handler does not have a PDCryptFreeAuthDataProc()). </p>
	
	<p>For Acrobat 3.0 and earlier, the correct way to obtain the 
	security handler in a PDAuthProc() is to call PDDocGetNewCryptHandler(), 
	relying on the fact that it returns the security handler 
	if the document has no new security handler, and the fact 
	that at the time the file is opened, it cannot yet have 
	a new security handler (in the future, one or more new 
	methods may be added to make this procedure more straightforward). The 
	Acrobat viewer's built-in authorization procedure works 
	according to the following algorithm: </p>

	<p>Call the security handler's PDCryptAuthorizeProc() with <code>NULL</code> 
	authorization data to automatically handle the case where 
	no authorization data is needed (for example, the file has 
	a <code>NULL</code> password). </p>

	<p>If PDCryptAuthorizeProc() returns <code>true</code>, open the file. </p>

	<p>If PDCryptAuthorizeProc() returns <code>false</code> then Loop for <code>i 
	= 1 to 3</code> {Call the security handler's PDCryptGetAuthDataProc().
	If PDCryptGetAuthDataProc returns <code>true</code> { Call PDDocAuthorize().</p>

	<p>If it returns <code>true</code> (authorized) call the security 
	handler's PDCryptFreeAuthDataProc(). Exit the loop and return 
	from PDAuthProc().} Call the security handler's PDCryptFreeAuthDataProc().}</p>
	
	<p>If it failed to get authorization after three attempts,  
	display a dialog box indicating that user is not authorized 
	to open the file. 
	}return from PDAuthProc(). </p>

	@param pdDoc The PDDoc to open.
	@return <code>true</code> if the user is authorized to open the document, <code>false</code> 
	otherwise. 
	@see PDDocAuthorize 
	@see PDDocOpen 
	@see PDDocOpenEx
*/
#if !defined(__SUNPRO_CC) || !defined(_H_THREADSAFEDATA)
//SunStudio doesn't accept multiple definition if one is under extern "C" while other is not.
typedef ACCBPROTO1 ASBool (ACCBPROTO2 *PDAuthProc)(PDDoc pdDoc);
#endif

/**
	A callback used by PDDocOpenEx(). It is called when an encrypted 
	document is opened, to determine whether the user is authorized 
	to open the file. 

	<p>This callback implements whatever authorization strategy 
	you choose and calls the callbacks of the appropriate security 
	handler (the one that was used to secure the document) to 
	obtain and check authorization data. </p>

	<p>The PDAuthProcEx() should obtain the authorization data (usually 
	a password) and call PDDocAuthorize(). PDDocAuthorize() in turn 
	calls the document encryption handler's <code>Authorize</code> function, 
	which returns the permissions that the authorization data 
	enables. PDDocAuthorize() adds these permissions to those 
	currently allowed, and returns the new set of allowed permissions. </p>
	
	@param pdDoc The PDDoc to open. 
	@param clientData User-supplied data that was passed in 
	the call to PDDocOpenEx().
	@return <code>true</code> if the user is authorized to open the document, <code>false</code> 
	otherwise. 
	@see PDDocAuthorize 
	@see PDDocOpen 
	@see PDDocOpenEx 
*/

#if !defined(__SUNPRO_CC) || !defined(_H_THREADSAFEDATA)
//SunStudio doesn't accept multiple definition if one is under extern "C" while other is not.
typedef ACCBPROTO1 ASBool (ACCBPROTO2 *PDAuthProcEx)(PDDoc pdDoc, void *clientData);
#endif

/**	The user can open and decrypt the document. 
	@ingroup PDPermsFlags
*/
#define pdPermOpen		0x01

/**	The user can change the document's security settings. 
	@ingroup PDPermsFlags
*/
#define pdPermSecure	0x02

/**	The user can print the document. Page Setup access is unaffected 
	by this permission, since that affects Acrobat's preferences - not 
	the document's. In the Document Security dialog, this corresponds 
	to the Printing entry. 
	@ingroup PDPermsFlags
*/
#define pdPermPrint		0x04

/**	The user can edit the document more than adding or modifying 
	text notes (see also pdPermEditNotes). In the Document Security 
	dialog, this corresponds to the Changing the Document entry. 
	@ingroup PDPermsFlags
*/
#define pdPermEdit		0x08

/**	The user can copy information from the document to the clipboard. 
	In the document restrictions, this corresponds to the 
	Content Copying or Extraction entry. 
	@ingroup PDPermsFlags
*/
#define pdPermCopy		0x10

/**	The user can add, modify, and delete text notes (see also 
	pdPermEdit). In the document restrictions, this corresponds 
	to the Authoring Comments and Form Fields entry. 	
	@ingroup PDPermsFlags
*/
#define pdPermEditNotes	0x20

/**	The user can perform a Save As.... If both pdPermEdit and 
	pdPermEditNotes are disallowed, Save will be disabled 
	but Save As... will be enabled. The Save As... menu item is not 
	necessarily disabled even if the user is not permitted to 
	perform a Save As.... 
	@note This cannot be set by clients.
	@ingroup PDPermsFlags
*/
#define pdPermSaveAs	0x40

/** 
	@ingroup PDPermsFlags
*/
#define pdPermExt		0x80
/* Added perm bits (PV2) used for standard security handler
** These bits only applies to standard security handler and they are
** used only when kenLength > 5 (40bits)
** Note: When keyLength > 5 is specified, standard security handler willl
** Encrypt Dict with Revision 3.  Rev 3 is support by Acrobat 5.0 and up.
*/

/** Overrides other PDPerm bits. It allows the user to fill in
	or sign existing form or signature fields.
	@ingroup PDPermsFlags
*/
#define pdPrivPermFillandSign	0x100

/** Overrides pdPermCopy to enable the Accessibility API.
	If a document is saved in Rev2 format (Acrobat 4.0 compatible),
	only the pdPermCopy bit is checked to determine the Accessibility API state.
	@ingroup PDPermsFlags
*/
#define pdPrivPermAccessible	0x200

/** Overrides various pdPermEdit bits and allows the following
	operations: page insert/delete/rotate and create bookmark and thumbnail.
	@ingroup PDPermsFlags
*/
#define pdPrivPermDocAssembly	0x400

/** This bit is a supplement to pdPermPrint. If it is clear
	(disabled) only low quality printing (Print As Image) is allowed. On UNIX
	platforms where Print As Image doesn't exist, printing is disabled.
	@ingroup PDPermsFlags
*/
#define pdPrivPermHighPrint		0x800

/* Added perm bit (PV3) used for standard security handler
** These bits only apply to standard security handler Revision 4
** R4 is support by Acrobat 6.0 and up.
*/

/** The user is permitted to perform all operations, regardless of
	the permissions specified by the document. Unless this
	permission is set, the document's permissions will be reset to
	those in the document after a full save.
	@ingroup PDPermsFlags
*/
#define pdPermOwner		0x8000

/** This should be set if the user can submit forms outside of the browser. This bit is
	a supplement to pdPrivPermFillandSign.
	@ingroup PDPermsFlags
*/
#define pdPrivPermFormSubmit       0x10000

/** This should be set if the user can spawn template pages. This bit will allow
	page template spawning even if pdPermEdit and pdPermEditNotes are clear.
	@ingroup PDPermsFlags
*/
#define pdPrivPermFormSpawnTempl   0x20000

/** 
	@ingroup PDPermsFlags
*/
#define pdPermAll		0xFFFFFFFF

/** The OR of all operations that can be set by the user in the
	security restrictions (pdPermPrint + pdPermEdit + pdPermCopy + pdPermEditNotes).
	@ingroup PDPermsFlags
*/
#define pdPermSettable	(pdPermPrint + pdPermEdit + pdPermCopy + pdPermEditNotes)

/** All permissions.
	@ingroup PDPermsFlags
*/
#define pdPermUser		(pdPermAll - pdPermOpen - pdPermSecure)


/** Constant values that specify permissions which allow operations on a document file.
	@see AVCryptGetPassword 
	@see PDDocAuthorize 
	@see PDDocGetPermissions (obsolete) 
	@see PDDocPermRequest 
	@see PDCryptAuthorizeProc 
	@see PDCryptGetAuthDataProc 
*/
typedef ASUns32 PDPerms;

/************************************************************************************\
|*																					*|
|*									PDDocSaveWithParams								*|
|*																					*|
\************************************************************************************/


/** Flags for the PDDocSave <code>saveFlags</code> parameter.
	All undefined flags should be set to zero.
	@see PDDocSave
	@see PDSaveFlags2
*/
enum {
	/** Save only those portions of the document that have changed. 
		This is provided only as the <i>opposite</i> of <code>PDSaveFull</code>, since there
		is no bit value of <code>0</code>. */
	PDSaveIncremental	= 0x00,	

	/** Save the entire document.
		Plug-ins that set <code>PDSaveFull</code> are also encouraged to set
		<code>PDSaveCollectGarbage</code>.
	*/
	PDSaveFull			= 0x01,		

	/** Save a copy of the document (the PDDoc continues to use the old file).
		This flag is ignored if <code>PDSaveFull</code> is off.
	*/
	PDSaveCopy			= 0x02,	

	/** Write the PDF file in a format that is optimized for page-served
		remote (network) access (<i>linearized</i>).
		This flag is ignored if <code>PDSaveFull</code> is off.

		<p>Prior to Acrobat 7, linearizing a file caused Cos objects to be
		invalidated, which required that some plug-ins use notifications to
		release and re-acquire objects. Acrobat 7 onwards, Cos objects
		are no longer invalidated after a linearized save.</p>
	*/
	PDSaveLinearized	= 0x04,	

	/** (Obsolete. In effect, it is always off). Write a PostScript header as part of the saved file.*/
	PDSaveWithPSHeader	= 0x08,		

	/** (Obsolete. In effect, it is always on). It is okay to store binary data in the PDF file.*/
	PDSaveBinaryOK		   = 0x10,	

	/** Remove unreferenced objects, often reducing file
		size. Plug-ins are encouraged to use this flag.
		This flag is ignored if <code>PDSaveFull</code> is off.
	*/
	PDSaveCollectGarbage   = 0x20,	

	/** Perform an incremental save even if the save is to a
		different file or the document's version number has
		changed.
	*/
	PDSaveForceIncremental = 0x40,	

	/** Do not update ModDate in InfoDict.*/
 	PDSaveKeepModDate      = 0x80,

	/** Leave the file open after the save (do not Close the file) */
	PDSaveLeaveOpen			= 0x100,
	
	/** This flag is currently reserved.  DO NOT USE  */
	PDSaveReserved0x200		= 0x200
};
typedef ASEnum16 PDSaveFlags;


/** More flags for the PDDocSave() <code>saveFlags</code> parameter (<code>PDSaveFlags2</code>).
	All undefined flags should be set to zero.
	The first three flags, <code>PDSaveUncompressed</code>, <code>PDSaveCompressed</code>, and
	<code>PDSaveCompressStructureOnly</code>, are mutually exclusive; they can all
	be off, but at most one can be on.
	@see PDDocSave
	@see PDSaveFlags
*/
enum	
{
	/**	Do not use object streams when saving the document (<i>decompress</i> all objects). 
		The result is compatible with all versions of PDF and Acrobat. 
		This flag is ignored if <code>PDSaveFull</code> is off.
	*/
	PDSaveUncompressed			= 1 << 0,

	/**	Compress objects, without restrictions about which objects to compress.
		The result is compatible only with PDF 1.5 (Acrobat 6) or later. 
		This flag is ignored if <code>PDSaveFull</code> is off.
	*/
	PDSaveCompressed			= 1 << 1,	

	/**	Compress only those objects that are 
		related to logical structure (for example, tagged PDF).
		The result is compatible with any version of PDF or Acrobat,
		but the compressed objects are usable only in PDF 1.5 (Acrobat 6). 
		This flag is ignored if <code>PDSaveFull</code> is off.
	*/
	PDSaveCompressStructureOnly	= 1 << 2,

	/**	Remove ASCII85 filters from all streams.
		This flag is ignored if <code>PDSaveFull</code> is off. 
	*/
	PDSaveRemoveASCIIFilters	= 1 << 3,

	/**	Encode any unencoded stream with Flate, except for metadata streams,
		which are never encoded, and for streams that would be larger if
		encoded.
		This flag is ignored if <code>PDSaveFull</code> is off. 
	*/
	PDSaveAddFlate				= 1 << 4,

	/**	Replace all LZW filters with FlateEncode filters.
		This flag is ignored if <code>PDSaveFull</code> is off.
	*/
	PDSaveReplaceLZW			= 1 << 5,

	/**	Merge identical forms and images, as determined by an MD5 hash of 
		their contents (it causes OptimizeXObjects() to be called). 
	*/
	PDSaveOptimizeXObjects					= 1 << 6,

	/**	Look for common initial sub-sequences among content streams (the sequences
		of marking operators), and generate <i>substreams</i> that can be shared (it causes OptimizeGraphics() to be called). 
	*/
	PDSaveOptimizeContentStreams			= 1 << 7,

	/**	Merge identical font descriptors and encodings. Does not merge the top-level
		font dictionary (it causes OptimizeFonts() to be called). 
	*/
	PDSaveOptimizeFonts						= 1 << 8,

	/**	Delete symbols specific to deleted images from JBIG2 dictionaries that
		could not be processed at the time of image deletion. It is currently only
		effective after deleting pages or extracting pages (it causes OptimizeMarkedJBIG2Dictionaries() to be called). 
	*/
	PDSaveOptimizeMarkedJBIG2Dictionaries	= 1 << 9,

	/**	(Obsolete. In effect, it is always off). 
	*/
	PDSaveEnsure7bitASCII					= 1 << 10,

	/** The <code>PDSaveAutoSave</code> flag is used to indicate that the save that occurred is an auto-save event.
		It is only set when an auto-save occurs.  It is a read-only flag.
	*/
	PDSaveAutoSave							= 1 << 11,

	/**	The <code>PDSaveOverrideCollections</code> flag controls whether <code>CosObjCollection</code> objects
		set up prior to saving are honored when doing a non-linearized save. Linearized save
		always uses its own rules for assigning objects to collections and object streams, so
		this flag is only used when the <code>PDSaveLinearized</code> flag is off. Furthermore, it is only
		used if either the <code>PDSaveCompressed</code> or <code>PDSaveCompressStructureOnly</code> flags is set.
		If this flag is set, the <code>PDDocSave</code> will remove all <code>CosObj</code> objects from their <code>CosObjCollection</code> objects
		and reassign objects to <code>CosObjCollection</code> objects (and object streams) using its own partitioning
		algorithms. If the flag is not set, the partitioning algorithms will preserve <code>CosObj</code> objects'
		existing membership in collections.
	*/
	PDSaveOverrideCollections				= 1 << 12
};
/**
	This enumeration defines the flags used in the <code>saveFlags2</code> bitfield of the PDDocSaveParams structure 
	passed to PDDocSaveWithParams(). These flags are an extension to those defined by the PDSaveFlags enumeration, 
	which are stored in the <code>saveFlags</code> bitfield of the PDDocSaveParams structure.
	@see PDDocSaveWithParams 
	@see PDDocInsertPages
	@see PDDocSaveParams
	@see PDSaveFlags
*/
typedef ASFlagBits PDSaveFlags2;

/** A structure used to flag Cos objects you wish to access while a PDDoc is being saved.
	It contains a pointer to a CosObjSetCallbackFlagProc().
	In your pre-save callback, you can use this exposed proc to set a flag in the
	CosObj objects that you care about, so that your callback will be called back during the save
	and given their offset and length.

	@see CosObjSetCallbackFlagProc
	@see PDDocOpenWithParams 
*/
typedef struct _t_PDDocPreSaveInfo {

	/** The size of structure. Set it to <code>sizeof(PDDocPreSaveInfoRec)</code>. */
	ASSize_t size;

	/** */
	CosObjSetCallbackFlagProc  callbackProc;
} PDDocPreSaveInfoRec, *PDDocPreSaveInfo;

/**
	A callback in the PDDocSaveParams structure used by PDDocSaveWithParams().
	Use this callback to flag Cos objects you wish to access
	while a PDDoc is being saved.
	@param pdDoc IN/OUT The PDDoc to be saved.
	@param preSaveInfo IN/OUT A PDDocPreSaveInfo structure containing
	information to use during processing before the save.
	@param clientData IN/OUT User-supplied data that was specified
	in <code>preSaveProcClientData</code> of the PDDocSaveParams structure.

	@see PDDocSaveWithParams
*/
typedef ACCBPROTO1 void  (ACCBPROTO2 *PDDocPreSaveProc)(
	PDDoc pdDoc, PDDocPreSaveInfo preSaveInfo, void *clientData);

/**
	A callback in the PDDocSaveParams structure. It is invoked by PDDocSaveWithParams() 
	immediately before a PDDoc is saved to disk. 
	@param pdDoc The PDDoc to be saved. 
	@param clientData User-supplied data that was specified 
	in <code>preWriteProcClientData</code> of the PDDocSaveParams structure.
	@see PDDocSaveWithParams 
*/
typedef ACCBPROTO1 void  (ACCBPROTO2 *PDDocPreWriteProc)(
	PDDoc pdDoc, void *clientData);

/**
	Parameters used when saving a file with PDDocSaveWithParams() 
	and returned by the PDDocWillSaveEx() notification. Most of 
	these parameters are the same as the parameters for PDDocSave(). 
	
	@see PDDocSaveWithParams 
*/
typedef struct _t_PDDocSaveParams {

	/** Set this to be the size of this <code>struct</code>. */
	ASSize_t			size;			

	/** It must be one of the PDSaveFlags values.
		@see PDSaveFlags
	*/
	PDSaveFlags			saveFlags;	

	/** The path to which the file is saved. A path must be
		specified when either <code>PDSaveFull</code> or <code>PDSaveCopy</code> are
		used for <code>saveFlags</code>. If <code>PDSaveIncremental</code> is
		specified in <code>saveFlags</code>, then <code>newPath</code> should be
		<code>NULL</code>.
		<p>If <code>PDSaveFull</code> is specified and <code>newPath</code> is the same
		as the file's original path, the new file is saved to a file
		system determined temporary path. Then the old file is
		deleted and the new file is renamed to <code>newPath</code>.</p>
	*/
	ASPathName			newPath;	

	/** The file system. If it is <code>NULL</code>, it uses the file system of the
		document's current backing file.
		@note Implementation restriction: Files can only be saved
		to the same file system, thus <code>fileSys</code> must be <code>NULL</code> or
		an error is raised.
	*/
	ASFileSys			fileSys;	

	/** Progress monitor. Use
		AVAppGetDocProgressMonitor() to obtain the
		default. It may be <code>NULL</code>.
		@see AVAppGetDocProgressMonitor
	*/
	ASProgressMonitor	mon;	

	/** A pointer to user-supplied data to pass to <code>mon</code> each time it
		is called. It must be <code>NULL</code> if <code>mon</code> is <code>NULL</code>. 
	*/
	void				*monClientData;

	/**	A callback to test whether an operation should be cancelled. A <code>CancelProc</code> 
		is typically passed to some method that takes a long time 
		to complete. At frequent intervals, the method calls the 
		<code>CancelProc</code>. If it returns <code>true</code>, then the method cancels 
		its operation; if <code>false</code>, it continues. 

		@see PDFLPrintCancelProc (Only available with PDF Library SDK) 
		@see AVAppGetCancelProc 
*/
	ASCancelProc		cancelProc;	

	/** A pointer to user-specified data to pass to <code>cancelProc</code>
		each time it is called. It must be <code>NULL</code> if <code>cancelProc</code> is
		<code>NULL</code>.
	*/
	void				*cancelProcClientData;

	/** A callback to flag Cos objects you wish to access while
		the PDDoc is being saved. 
	*/
	PDDocPreSaveProc	preSaveProc;	

	/** A pointer to user-specified data to pass to <code>preSaveProc</code>
		each time it is called.
	*/
	void				*preSaveProcClientData;

	/** A callback to get information about Cos
		objects while the PDDoc is being saved.
	*/
	CosObjOffsetProc	offsetProc;	

	/** A pointer to user-specified data to pass to <code>offsetProc</code>
		each time it is called.
	*/
	void				*offsetProcClientData;

	/** The major PDF version number of the document. If <code>major</code>
		equals <code>0</code>, both <code>major</code> and <code>minor</code> are ignored. Make
		sure that the document conforms to the version number
		you are setting.
	*/
	PDDocVersion		major;			

	/** The minor PDF version number of the document.*/
	PDDocVersion		minor;		

	/** If <code>true</code>, it makes the XML packet containing the
        XMP metadata for the document read-only and
        suppresses the generation of padding.  If <code>false</code>,
        the XML packet is made writable and the number of
        padding bytes specified by the value of
        the <code>XMPPacketPaddingBytes</code> field is included in
        the XML packet. 
	*/
    ASBool              XMPPacketReadOnly;  

	/** Specifies the number of bytes of padding to
        include in the XML packet containing the XMP
        metadata for the document, when
        <code>XMPPacketReadOnly</code> is <code>false</code> (ignored when
        <code>XMPPacketReadOnly</code> is <code>true</code>).  Non-positive values
        specify the default padding of 2048 bytes;
        positive values specify the number of bytes of
        padding directly. 
	*/
    ASInt32             XMPPacketPaddingBytes; 

	/** A callback that is called just before the document is written to disk. */
    PDDocPreWriteProc   preWriteProc;  

	/** Client data to pass to the pre-write proc. */
    void                *preWriteProcClientData;

	/** More flags for PDDocSave(). */
	PDSaveFlags2		saveFlags2;	/*	*/

	/** The number of incremental saves to <i>compact</i>. It will eliminate
		the last <code>numSubFilesToCompact</code> subFiles (as opposed to the update section, which
		does not always correspond to a valid subFile), and merge the changes in them into
		the current append save. 
		
		<p>This parameter is taken into account only if:</p>
	
		<ul>
		<li>The current save operation is an incremental save AND </li>
		<li>The current save operation is a saveAs. </li>
		</ul>
		
		<p>More precisely, compaction of incremental saves will only happen only if:</p>
	
		<ul>
		<li><code>newPath != NULL</code> AND </li>
		<li>either <code>PDSaveForceIncremental</code> is set or the document has digital signatures in it, or both. </li>
		</ul>
		
		<p>Moreover, if there are any signatures added in any of the incremental saves that are compacted,
		these signatures will be invalidated. It is up to the client to determine the incremental
		save at which it should stop compacting to avoid this.</p>
	*/
	ASUns32				numSubFilesToCompact;	
		
	/* Acrobat 10 addition */
	/** A 64-bit callback to get information about
		Cos objects while the PDDoc is being saved.
	*/
	CosObjOffsetProc64	offsetProc64;	

} PDDocSaveParamsRec;
typedef struct _t_PDDocSaveParams *PDDocSaveParams;


/**
	A structure used to pass information to the PDDocWillInsertPagesEx() 
	and PDDocDidInsertPagesEx() notifications. 
	@see PDDocInsertPages 
*/
typedef struct _t_PDDocInsertPagesParams {

	/** The size of the data structure. It must be set to
		<code>sizeof(PDDocInsertPagesParamsRec)</code>.
	*/
	ASSize_t	size;			

	/** The document into which pages are inserted. This document
		must have at least one page.
	*/
	PDDoc		targetDoc;

	/** The page number in <code>targetDoc</code> after which pages from
		<code>srcDoc</code> are inserted. The first page is <code>0</code>. If
		PDBeforeFirstPage (see PDExpT.h) is used, the pages are
		inserted before the first page in <code>targetDoc</code>. Use PDLastPage
		to insert pages after the last page in <code>targetDoc</code>. 
	*/
	PDPageNumber		insertAfterThisPage;

	/** The document containing the pages that are inserted into <code>targetDoc</code>.*/
	PDDoc		srcDoc;

	/** The page number of the first page in <code>srcDoc</code> to insert into
		<code>targetDoc</code>. The first page is <code>0</code>.
	*/
	PDPageNumber		srcFromPage;

	/** The page number of the last page in <code>srcDoc</code> to insert into targetDoc.*/
	PDPageNumber		srcToPage;

	/** Flags that determine what additional information is copied from
		<code>srcDoc</code> into <code>targetDoc</code>. It must be an OR of the following:

		<TABLE rules="all" cellspacing="1">
		<TR><TH>Flag</TH><TH>Description</TH></TR>
		<TR><TD><code>PDInsertAll</code></TD><TD>Inserts the entire document <code>srcDoc</code> into the
		document <code>targetDoc</code>. This operation not only inserts the
		pages themselves, but also merges other document data from
		<code>srcDoc</code> into <code>targetDoc</code>. In particular, the following happens:
		<ul>
		<li>The bookmark tree of <code>srcDoc</code> is merged into the bookmark
		tree of <code>targetDoc</code> by copying it as a new first-level sub-tree of the
		<code>targetDoc</code> object's bookmark tree root, of which it becomes the last
		child. If <code>targetDoc</code> has no bookmark tree, it acquires one
		identical to the bookmark tree from <code>srcDoc</code>.</li>
		<li>Named destinations from <code>srcDoc</code> (of PDF 1.1 and later) are
		copied into <code>targetDoc</code>. If there are named destinations in
		<code>srcDoc</code> with the same name as some named destination in
		<code>targetDoc</code>, the ones in <code>targetDoc</code> retain their names and the
		copied named destinations are given new names based on the
		old ones with distinguishing digits added. Actions and
		bookmarks referring to the old names are made to refer to the
		new names after being copied into <code>targetDoc</code>.</li>
		<li>Document logical structure from <code>srcDoc</code> is copied into
		<code>targetDoc</code>. The top-level children of the structure tree root of
		<code>srcDoc</code> are copied as new top-level children of the structure
		tree root of <code>targetDoc</code>; a structure tree root is created in
		<code>targetDoc</code> if there was none before. The role maps of the two
		structure trees are merged, with name conflicts resolved in favor
		of the role mappings present in <code>targetDoc</code>. Attribute objects
		are not copied, nor is class map information from <code>srcDoc</code>
		merged into that for <code>targetDoc</code>.</li>
		</ul>
		If the <code>PDInsertAll</code> flag is not set, pages copied from <code>srcDoc</code> into
		<code>targetDoc</code> will have their structure back-pointer information
		stripped off.</TD></TR>
		<TR><TD><code>PDInsertBookmarks</code></TD><TD>Inserts bookmarks as well as pages.</TD></TR>
		<TR><TD><code>PDInsertThreads</code></TD><TD>Inserts threads as well as pages.</TD></TR>
		</TABLE>
		
	*/
	PDSmallFlagBits		insertFlags;

	/** An error code which is only valid for the PDDocDidInsertPagesEx() notification.*/
	ASErrorCode		error;			
} PDDocInsertPagesParamsRec;
typedef struct _t_PDDocInsertPagesParams *PDDocInsertPagesParams;

/**
	A structure used by PDDocOpenWithParams() to specify file open 
	information. The parameters are very similar to those in 
	PDDocOpenEx() and PDDocOpenFromASFileEx(). 
	@see PDDocOpenWithParams 
*/
typedef struct _t_PDDocOpenParams *PDDocOpenParams;
/**
	A structure used by PDDocOpenWithParams() to specify file open 
	information. The parameters are very similar to those in 
	PDDocOpenEx() and PDDocOpenFromASFileEx(). 
	@see PDDocOpenWithParams 
*/
typedef struct _t_PDDocOpenParams {

	/** The size of the data structure. It must be set to <code>sizeof(PDDocOpenParamsRec)</code>.*/
	ASSize_t		size;     

	/** The path name to the file, specified in whatever format is correct for <code>fileSys</code>.*/
	ASFile			file;		

	/** The path name to the file, specified in whatever format is correct for <code>fileSys</code>.*/
	ASPathName		fileName;	

	/** A pointer to an ASFileSysRec containing the file system in which the file resides.*/
	ASFileSys		fileSys;	

	/** An authorization callback, called only if the file is encrypted.
		This callback should obtain whatever information is
		needed to determine whether the user is
		authorized to open the file, then call PDDocAuthorize()
		(which returns the permissions that the authentication
		data enables). If the file is encrypted and <code>authProcEx</code> is
		<code>NULL</code> or returns <code>false</code>, <code>pdErrPassword</code> is raised.
		The Acrobat viewer's built-in authorization procedure
		requires the user to enter a password, and allows the
		user to try three times before giving up.
	*/
	PDAuthProcEx	authProcEx;

	/** A pointer to user-specified data to pass to authProcEx() each time it is called.*/
	void			*authProcClientData;

	/** If <code>true</code>, attempt to repair the file if it is damaged; if <code>false</code>, do not attempt to repair the file if it is damaged.*/
	ASBool			doRepair;

	/** The permissions to remove from the document.*/
	PDPerms			restrictPerms; 
} PDDocOpenParamsRec;

/**
	An enumerated data type that specifies whether thumbnail images or bookmarks are
	shown.
	@see AVDocGetViewMode
	@see AVDocSetViewMode
	@see PDDocGetPageMode
	@see PDDocSetPageMode
*/
enum {

	/** Leaves the view mode as is. */
	PDDontCare,

	/** Displays the document, but displays neither thumbnails nor bookmarks.*/
	PDUseNone,

	/** Displays the document plus thumbnails.*/
	PDUseThumbs,

	/** Displays the document plus bookmarks.*/
	PDUseBookmarks,

	/** Displays the document in full-screen viewing mode. This is equivalent to AVAppBeginFullScreen(). */
	PDFullScreen,

	/** */
	PDContents,

	/** */
	PDUseOC,

	/** Displays the document plus attachments. */
	PDUseAttachments
};
typedef ASEnum8 PDPageMode;

/** 
		A structure that defines the layout of a document. The layout can be set as the viewer's
		<code>avpPageViewLayoutMode</code> preference (set by AVAppSetPreference()) or in a view
		of a document by the <code>pageViewLayoutMode</code> field in AVDocViewDef (set by
		AVDocGetViewDef()).
		@see AVDocGetViewDef
		@see AVPageViewGetLayoutMode
		@see AVPageViewSetLayoutMode
*/
enum {

	/** (Default) Use the user preference when opening the
		file, as specified in the <code>avpPageViewLayoutMode</code>
		preference, set by AVAppSetPreference().
	*/
	PDLayoutDontCare,

	/** Use single-page mode, as in pre-Acrobat 3.0 viewers.*/
	PDLayoutSinglePage,

	/** Use one-column continuous mode.*/
	PDLayoutOneColumn,

	/** Use two-column continuous mode with the first page on the left.*/
	PDLayoutTwoColumnLeft,

	/** Use two-column continuous mode with the first page on the right.*/
	PDLayoutTwoColumnRight,

	/** */
	PDLayoutTwoPageLeft,

	/** */
	PDLayoutTwoPageRight
};
typedef ASEnum8 PDLayoutMode;

/** An enumerated data type that specifies various file status attributes.
	These flags indicate the state of the document and whether it needs
	to be saved or deleted on close, and so on.
	The flags are used as a bit field. More than one value may be set.
	Some flags may be set or get only. Most can be either set or get.
	@see PDDocClearFlags
	@see PDDocGetFlags
	@see PDDocSetFlags
*/
enum {

	/** The document has been modified and needs to be saved. (get/set) */
	PDDocNeedsSave			= 0x0001,	

	/** The document cannot be saved incrementally;
		when it is saved using PDDocSave(), the
		<code>PDSaveFull</code> flag must be specified (see
		PDSaveFlags). (get/set)
	*/
	PDDocRequiresFullSave	= 0x0002,	

	/** The document has been modified slightly (for example, bookmarks or text annotations have been
		opened or closed), but not in a way that
		warrants saving. (get only)
	*/
	PDDocIsModified			= 0x0004,	

	/** The document is based on a temporary file that
		must be deleted when the document is closed
		or saved. (get/set)
	*/
	PDDocDeleteOnClose		= 0x0008,	

	/** The document was repaired when it was opened. (get only) */
	PDDocWasRepaired		= 0x0010,	

	/** The document's major version is newer than current. (get only) */
	PDDocNewMajorVersion	= 0x0020,	

	/** The document's minor version is newer than current. (get only)*/
	PDDocNewMinorVersion	= 0x0040,	

	/** The document's version is older than current. (get only) */
	PDDocOldVersion			= 0x0080,	

	/** Do not display errors. (get/set) */
	PDDocSuppressErrors		= 0x0100,	

	/** The document is embedded in a compound document (OLE, OpenDoc). (get/set) */
	PDDocIsEmbedded			= 0x0200,	

	/** The document is linearized (optimized) for page-served remote (network) access. (get only) */
	PDDocIsLinearized		= 0x0400,

	/** The document is optimized. If this flag is cleared,
		the Batch Optimizer plug-in and the Adobe
		PDF Library do not save the file optimized.
		You can, therefore, linearize a PDF file
		without optimizing it. Optimizing without
		linearizing is not allowed, however. (set only) 
	*/
	PDDocIsOptimized		= 0x0800,

	/** The underlying file is PxDF (get/set). */
	PDDocIsPXDF				= 0x1000,

	/** The document is fully opened (didOpen). (get only) */
	PDDocIsOpen				= 0x2000
};


/** Used by PDDocInsertPages(). 
	@see PDDOcInsertPages
*/
typedef enum {

	/** Insert bookmarks as well as pages. */
	PDInsertBookmarks		= 0x0001,	

	/** Insert all Catalog and Info dictionary values as well as pages. */
	PDInsertAll				= 0x1000,	

	/** Insert articles as well. */
	PDInsertThreads			= 0x0002	
} PDInsertFlags;

/*
** PDPageNumber specification defines -- use where a page number or range or count
** is required.
*/
#define PDBeforeFirstPage	((PDPageNumber) -1)
#define PDLastPage			((PDPageNumber) -2)
#define PDAllPages			((PDPageNumber) -3)
#define PDOddPagesOnly      ((PDPageNumber) -4)
#define PDEvenPagesOnly     ((PDPageNumber) -5)

/************************************************************************************\
|*																					*|
|*									PDFind											*|
|*																					*|
\************************************************************************************/
typedef struct _t_PDFind *PDFind;

/** Passed to PDFindText(). */
enum {

	/** Find whole words only. */
	PDFindWholeWords	= 0x0001,		

	/** Perform a case-sensitive search. */
	PDFindCaseSens		= 0x0002,		

	/** Perform a reverse order search. */
	PDFindReverse		= 0x0004,		

	/** Return a PDTextSelect with all found words on the page. */
	PDFindAllOnPage		= 0x0008,		

	/** Do not perform a match of full-width/half-width Kana characters. */
	PDFindIgnoreFH		= 0x0100,		

	/* Ignore diacritics. */
	PDFindIgnoreDiacritics = 0x0200,
	
	/** Reset to the beginning of the document. */
	PDFindReset			= 0x0800
	
};
typedef ASEnum8 PDFindFlags;


/************************************************************************************\
|*																					*|
|*									PDFont											*|
|*																					*|
\************************************************************************************/


/*
** These flags define the bit masks for the flags field in structure PDFontFlags.
*/
#define PDFONTFLAGS_USEDBYFORM 0x00000001

/**
	A data structure containing additional information about a font. 
	@see PDDocEnumFonts
	@see PDFontEnumProc 
*/
typedef struct _t_PDFontFlags {

	/** Not used. For backward compatibility. */
	ASInt32  notUsed;

	/** It must be an OR of the <code>PDFONTFLAGS_</code> values. All unused flags must be off. */
	ASUns32 flags;				
} PDFontFlags;

/**
	A callback used by PDDocEnumFonts() and PDDocEnumLoadedFonts(). 
	It is called once for each font. 
	@param font The font currently being enumerated. 
	@param fontFlags PDFontFlags used when PDDocEnumFonts() was called. 
	@param clientData User-supplied data passed in the call 
	to PDDocEnumFonts() or PDDocEnumLoadedFonts().
	@return <code>true</code> to continue enumeration, <code>false</code> to halt enumeration. 
	
	@see PDDocEnumFonts 
	@see PDDocEnumLoadedFonts 
*/
typedef ACCBPROTO1 ASBool (ACCBPROTO2 *PDFontEnumProc)(PDFont font, PDFontFlags *fontFlags, void *clientData);

/*
** PSNAMESIZE is a typical maximum length for a font name.
*/
#define PSNAMESIZE 128

/*
** These flags define the bit masks for the flags field returned by PDFontGetFlags().
*/
#define     PD_FIXED_WIDTH      0x00000001
#define     PD_SERIF		    0x00000002
#define     PD_PI		     	0x00000004
#define     PD_SCRIPT		 	0x00000008
#define     PD_STD_ENCODING     0x00000020
#define     PD_ITALIC			0x00000040
#define     PD_ALL_CAP			0x00010000
#define     PD_SMALL_CAP		0x00020000
#define     PD_FORCE_BOLD       0x00040000


/** An enumerated data type that specifies a font's encoding. */
enum {

	/** The encoding specified internally in the font. In the
		case of a Type 1 or MMType 1 font, this is specified
		by the Encoding value in the font's <code>fontdict</code>. In the
		case of TrueType fonts, this is the encoding specified
		by the default one-byte <code>CMap</code> for the platform.
	*/
	PDBuiltInEncoding = -1,

	/** MacRomanEncoding, as defined in Appendix D in
		the <i>PDF Reference</i>.
	*/
	PDMacRomanEncoding = 0,

	/** MacExpertEncoding, as defined in Appendix D in
		the <i>PDF Reference</i>.
	*/
	PDMacExpertEncoding = 1,

	/** WinAnsiEncoding, as defined in Appendix D in
		the <i>PDF Reference</i>.
	*/
	PDWinAnsiEncoding = 2,

	/** StandardEncoding, as defined in Appendix D in
		the <i>PDF Reference</i>.
	*/
	PDStdEncoding = 3,

	/** PDFDocEncoding, as defined in Appendix D in the
		<i>PDF Reference</i>. This will never be returned for a
		font; it is used internally.
	*/
	PDFDocEncoding = 4,		

	/** */
	PDLastKnownEncoding
};
typedef ASEnum8 PDFontEncoding;

#define PDLastOneByteEncoding PDLastKnownEncoding
#define	PDUnicodeEncoding PDLastKnownEncoding

/** An enumerated data type that identifies the character set of a Type 1, Multiple Master Type 1,
	or TrueType font.
*/
enum {

	/** The font does not use Adobe standard encoding.*/
	PDUnknownCharSet = 0,

	/** The font uses Adobe standard encoding. This is
		determined by the <code>"Uses Adobe Standard
		Encoding"</code> bit in the font descriptor. 
	*/
	PDStandardRomanCharSet = 1,

	/** Currently unused. */
	PDAdobeExpertCharSet = 2,

	/** */
	PDLastCharSet
};

typedef ASEnum8 PDCharSet;

/* Font Embedding -- segment types */
#define PD_SEGASCII  ((ASUns8) 1)
#define PD_SEGBINARY ((ASUns8) 2)
#define PD_SEGEOF    ((ASUns8) 3)

/**
	A data structure containing PANOSE and sFamily class values 
	for a font. It is used in the PDFontMetrics structure. See Section 
	5.7.2 in the <i>PDF Reference</i> for more information. For additional 
	details on the PANOSE number, see the <i>Japanese TrueType Font 
	Property Selection Guidelines</i> by the TrueType Conference 
	Technical Committee. 
	@see PDFontGetMetrics 
	@see PDFontSetMetrics 
*/
typedef struct _t_PDFontStyles {

	/** A number that identifies the font family and determines the
		meaning of the remaining PANOSE digits. The possible families
		are Latin, Kanji, Hebrew, and so forth.
	*/
	ASUns8 sFamilyClassID;	

	/** A number to identify the kind of family: text, decorative,
		handwritten, symbols, and so on.
	*/
	ASUns8 sFamilySubclassID;

	/** A number to identify the family type: text, decorative,
		handwritten, symbols, and so on.
	*/
	ASUns8 bFamilyType;	       

	/** A number that specifies the font's serif style, such as cove,
		obtuse cove, square, bone, and so forth.
	*/
	ASUns8 bSerifStyle;

	/** A number that specifies the font's weight, such as very
		light, heavy, black, and so on.
	*/
	ASUns8 bWeight;

	/** A number that specifies the font's proportions, such as
		modern, expanded, condensed, mono-spaced, and so on.
	*/
	ASUns8 bProportion;
} PDFontStyles;

/**
	A data structure containing information about a font's metrics. 
	See Section 5.5.1 in the <i>PDF Reference</i> for more information 
	about font metrics.You also can find information about Adobe 
	Font Metrics (AFM) at http://partners.adobe.com/asn. 
	@see PDFontGetMetrics 
	@see PDFontSetMetrics 
*/
typedef struct _t_PDFontMetrics {

	/** It must be an OR of the Font Flags values. All unused flags must be off. */
	ASFlagBits			flags;				

	/** A font bounding box in 1000 EM units. An EM is a typographic
		unit of measurement equal to the size of a font. In a 12-point
		font, an EM is 12 points.	
	*/
	ASFixedRect			fontBBox;			

	/** The width of the missing character (<code>.notdef</code>) */
	PDiFontMetric  		missingWidth;		

	/** The vertical stem width.				*/
	PDiFontMetric  		stemV;				

	/** The horizontal stem width.			*/
	PDiFontMetric  		stemH;				

	/** The capital height.					*/
	PDiFontMetric  		capHeight;			

	/** The X height.							*/
	PDiFontMetric  		xHeight;			

	/** The maximum ascender height.				*/
	PDiFontMetric  		ascent;				

	/** The maximum descender depth.				*/
	PDiFontMetric  		descent;			

	/** The additional leading between lines.	*/
	PDiFontMetric  		leading;			

	/** The maximum character width.			*/
	PDiFontMetric  		maxWidth;			

	/** The average character width.			*/
	PDiFontMetric  		avgWidth;			

	/** The italic angle in degrees, if any.	*/
	PDFontAngle  		italicAngle;		

	/** The PANOSE and sFamily class values.  */
	PDFontStyles		style;				

	/** The baseline adjustment, which is a vertical adjustment for font
		baseline difference and writing mode 1 (vertical). This should
		only be used for CIDFontType 2 fonts with font substitution. 
	*/
	PDFontOffset		baseLineAdj;		
} PDFontMetrics, *PDFontMetricsP;

/************************************************************************************\
|*																					*|
|*						PDPage & Contents & Resources								*|
|*																					*|
\************************************************************************************/


/** Specifies page rotation, in degrees. It is used for routines that
	set or get the value of a page's Rotate key.
	@see PDPageGetRotate
	@see PDPageSetRotate 
*/
enum {
	/** */
	pdRotate0 = 0,

	/** */
	pdRotate90 = 90,

	/** */
	pdRotate180 = 180,

	/** */
	pdRotate270 = 270
};
typedef ASEnum16 PDRotate;

/** All graphic objects that comprise page, charproc, and PDForm descriptions.
	@see PDGraphicEnumMonitor
 */
typedef struct _t_PDGraphic *PDGraphic,
							/* as well as the following subtypes of PDGraphic */
							*PDText,
							*PDPath,
							*PDInlineImage;

/** A superclass used for PDF XObjects. Acrobat currently uses two XObject subclasses:
	PDImage and PDForm. You can use any PDXObject method on these three objects.
	@see PDXObjectEnumFilters
	@see PDXObjectGetData 
*/
typedef struct _t_PDXObject *PDXObject;	/* Note that a PDFont is really a PDXObject */

/* Type 3 Font CharProc */
typedef struct _t_PDCharProc *PDCharProc;

/**
	A path object consists of a sequence of segment operators (moveto, lineto, and so on),
	as well as a set of operations to be performed with the path.
	Note that the operations include doing nothing, closing, stroking, filling
	and using the path as a clip.
	@see PDPathPaintOp
*/
enum {
	/** */
	pdSegMoveTo,
	/** */
	pdSegLineTo,
	/** */
	pdSegCurveTo,
	/** */
	pdSegVCurveTo,
	/** */
	pdSegYCurveTo,
	/** */
	pdSegRect,
	/** */
	pdSegClosePath
};
typedef ASEnum8 PDPathSegmentOp;

/** 
	A path object consists of a sequence of segment operators (moveto, lineto, an so on),
	as well as a set of operations to be performed with the path.
	Note that the operations include doing nothing, closing, stroking, filling
	and using the path as a clip.
	@see PDPathSegmentOp
	@see PDPathGetPaintOp 
*/
enum {
	/** The path is not painted. */
	pdPathNoPaint = 0,
	/** The path contains a closepath operator. */
	pdPathOpClose = 1,
	/** The path contains a stroke operator. */
	pdPathStroke = 2,
	/** The path contains a fill operator. */
	pdPathFill = 4,
	/** The path contains an eofill operator. */
	pdPathEOFill = 8,
	/** The path contains a clip operator. */
	pdPathClip = 16,
	/** The path contains an eoclip operator. */
	pdPathEOClip = 32
};
typedef ASEnum8 PDPathPaintOp;

/** A data structure containing information about the current graphics state.
	@see PDGraphicGetState
*/
typedef struct _t_PDGraphicState {
	/** Current transformation matrix. */
	ASFixedMatrix		ctm;

	/** Fill color. */
	ASFixed			fillColor[4];

	/** Stroke color. */
	ASFixed			strokeColor[4];

	/** Fill color space. */
	ASAtom			fillCSpace;

	/** Stroke color space. */
	ASAtom			strokeCSpace;

	/** Flatness tolerance. */
	ASFixed			flatness;

	/** Line cap style. */
	ASInt32			lineCap;

	/** Dash phase. */
	ASFixed			dashPhase;

	/** Length of dash array. */
	ASTArraySize	dashLen;

	/** Fixed to 10. The viewer has been extended to support dashes of any length. */
	ASFixed			dashes[10];

	/** Line join style. */
	ASInt32			lineJoin;

	/** Line width. */
	ASFixed			lineWidth;

	/** Miter limit. */
	ASFixed			miterLimit;
} PDGraphicState, *PDGraphicStateP;

/** A data structure containing information about the current text state.
	@see PDTextGetState
*/
typedef struct _t_PDTextState {
	/** Text font. */
	PDFont			font;

	/** Character spacing. */
	ASFixed			charSpacing;

	/** Word spacing. */
	ASFixed			wordSpacing;

	/** Horizontal scaling. */
	ASFixed			horizontalScale;

	/** Leading. */
	ASFixed			leading;

	/** Text rise. */
	ASFixed			textRise;

	/** Text font size. */
	ASFixed			textSize;

	/** Text rendering mode. */
	ASInt32			renderMode;

	/** Text matrix. */
	ASFixedMatrix		textMatrix;
} PDTextState, *PDTextStateP;

/**
	A data structure containing information about the attributes 
	of an image. See Section 4.8 in the <i>PDF Reference</i> for more 
	information. 
	@see PDImageGetAttrs 
	@see PDInlineImageGetAttrs 
*/
typedef struct _t_PDImageAttrs {
	/** (Required) The width of the source image in samples. */
	PDImageScalar	width;
	/** (Required) The height of the source image in samples. */
	PDImageScalar	height;
	/** (Required) The number of bits used to represent each color component. */
	PDCount			bitsPerComponent;
	/** (Optional) <code>true</code> if the image should be treated as a mask, <code>false</code> otherwise.*/
	ASBool			imageMask;
	/** (Optional) <code>true</code> if interpolation is performed, <code>false</code> otherwise.
		Interpolation attempts to smooth transitions between sample values.
	*/
	ASBool			interpolate;
	/** <code>true</code> if <code>decode</code> is used, <code>false</code> otherwise.*/
	ASBool			haveDecode;
	/** (Optional) An array of numbers specifying the mapping from
		sample values in the image to values appropriate for the current color space.
	*/
	ASFixed			decode[8];
	/** An ASAtom representing the color space name.*/
	ASAtom			colorSpaceName;
	/** <code>true</code> if the color space is indexed, <code>false</code> otherwise.*/
	ASBool			isIndexed;
	/** (Optional) This is used if <code>isIndexed</code> is <code>true</code>. Colors are specified by integers in the range <code>0</code> to <code>hival</code>.*/
	PDCount			hiVal;
	/** (Required for images, not allowed for image masks) A Cos object of the color space used for the image samples.*/
	CosObj			colorSpace;
	/** The length of the sample data in bytes. */
	ASTArraySize	dataLen;
	/* Added in Acrobat 4.0 */
	/** The number of components in <code>colorSpace</code>. For instance, <code>comps</code> is <code>3</code> for an RGB color space.*/
	PDCount			comps;
} PDImageAttrs, *PDImageAttrsP;

/**
	A callback for PDResourceEnumMonitor. It is a procedure called for
	font resources.
	@param font IN/OUT The font.
	@param name IN/OUT The name of the font as it appears in the Resources
	dictionary.
	@param clientData IN/OUT User-supplied data that was passed in
	the call to PDFormEnumResources().
	@return <code>true</code> to continue enumeration, <code>false</code> to halt enumeration.
	@see PDFormEnumResources
*/
typedef ACCBPROTO1 ASBool (ACCBPROTO2 *PDResourceEnumFontProc)		(PDFont font, char *name, void *clientData);

/**
	A callback for PDResourceEnumMonitor. It is a procedure called for
	XObject resources.
	@param xObject IN/OUT The XObject.
	@param name IN/OUT The name of the XObject as it appears in the
	Resources dictionary.
	@param clientData IN/OUT User-supplied data that was passed in
	the call to PDFormEnumResources().
	@return <code>true</code> to continue enumeration, <code>false</code> to halt enumeration.
	@see PDFormEnumResources
*/
typedef ACCBPROTO1 ASBool (ACCBPROTO2 *PDResourceEnumXObjectProc)	(PDXObject xObject, char *name, void *clientData);

/**
	A callback for PDResourceEnumMonitor. It is a procedure called for
	ProcSet resources.
	@param name IN/OUT The name of the ProcSet as it appears in the
	Resources dictionary.
	@param clientData IN/OUT User-supplied data that was passed in
	the call to PDFormEnumResources().
	@return <code>true</code> to continue enumeration, <code>false</code> to halt enumeration.

	@see PDFormEnumResources
*/
typedef ACCBPROTO1 ASBool (ACCBPROTO2 *PDResourceEnumProcSetProc)	(char *name, void *clientData);

/**
	A callback for PDResourceEnumMonitor. It is called for color
	space resources.
	@param name IN/OUT The color space name.
	@param colorSpace IN/OUT The name of the color space as it appears
	in the Resources dictionary.
	@param clientData IN/OUT User-supplied data that was passed in
	the call to PDFormEnumResources().
	@return <code>true</code> to continue enumeration, <code>false</code> to halt enumeration.

	@see PDFormEnumResources
*/
typedef ACCBPROTO1 ASBool (ACCBPROTO2 *PDResourceEnumColorSpaceProc)	(char *name, CosObj colorSpace, void *clientData);

/**
	A data structure containing callbacks used when enumerating 
	the resources of a form with PDFormEnumResources() or PDPageEnumResources(). 
	
	@note PDPageEnumResources is provided only for backwards 
	compatibility. It has not been updated beyond PDF Version 
	1.1 and may not work correctly for newly-created PDF 1.2 
	or later files. You should use the PDFEdit API to enumerate 
	page resources. 
	@see PDFormEnumResources 
*/
typedef struct _t_PDResourceEnumMonitor *PDResourceEnumMonitor;
/**
	A data structure containing callbacks used when enumerating 
	the resources of a form with PDFormEnumResources() or PDPageEnumResources(). 

	@note PDPageEnumResources() is provided only for backwards 
	compatibility. It has not been updated beyond PDF Version 
	1.1 and may not work correctly for newly-created PDF 1.2 
	or later files. You should use the PDFEdit API to enumerate 
	page resources. 
	@see PDFormEnumResources 
*/
typedef struct _t_PDResourceEnumMonitor {
	/** The size of the data structure. It must be set to <code>sizeof(PDResourceEnumMonitorRec)</code>. */
	ASSize_t size;
	/** */
	PDResourceEnumFontProc			EnumFont;
	/** */
	PDResourceEnumXObjectProc		EnumXObject;
	/** */
	PDResourceEnumProcSetProc		EnumProcSet;
	/** */
	PDResourceEnumColorSpaceProc	EnumColorSpace;
} PDResourceEnumMonitorRec;

/**
	A callback for PDGraphicEnumMonitor. It is called for every text
	operator.
	@param obj IN/OUT The text object.
	@param clientData IN/OUT User-supplied data that was passed in
	the call to PDPageEnumContents().
	@return <code>true</code> to continue enumeration, <code>false</code> to halt enumeration.

	@see PDPageEnumContents
*/
typedef ACCBPROTO1 ASBool (ACCBPROTO2 *PDGraphicEnumTextProc)			(PDText obj, void *clientData);

/**
	A callback for PDGraphicEnumMonitor. It is called for every path
	operator.
	@param obj IN/OUT The path data.
	@param clientData IN/OUT User-supplied data that was passed in
	the call to PDPageEnumContents().
	@return <code>true</code> to continue enumeration, <code>false</code> to halt enumeration.

	@see PDPageEnumContents
*/
typedef ACCBPROTO1 ASBool (ACCBPROTO2 *PDGraphicEnumPathProc)			(PDPath obj, void *clientData);

/**
	A callback for PDGraphicEnumMonitor. It is called for every image
	operator.
	@param obj IN/OUT Image data.
	@param clientData IN/OUT User-supplied data that was passed in
	the call to PDPageEnumContents().
	@return <code>true</code> to continue enumeration, <code>false</code> to halt enumeration.

	@see PDPageEnumContents
*/
typedef ACCBPROTO1 ASBool (ACCBPROTO2 *PDGraphicEnumImageProc)			(PDInlineImage obj, void *clientData);

/**
	A callback for PDGraphicEnumMonitor. It is called for every XObject
	(Do) operator.
	@param name IN/OUT The XObject's name.
	@param bbox IN/OUT The XObject's bounding box, describing the
	bounding box of the XObject in user space. This is only
	the case for top-level XObjects. If a Form XObject refers
	to another XObject, the second XObject's bounding box is
	the 'infinity' bounding box.
	@param clientData IN/OUT User-supplied data that was passed in
	the call to PDPageEnumContents().
	@return <code>true</code> to continue enumeration, <code>false</code> to halt enumeration.

	@see PDPageEnumContents
*/
typedef ACCBPROTO1 ASBool (ACCBPROTO2 *PDGraphicEnumXObjectRefProc)	(char *name, ASFixedRect *bbox, void *clientData);

/**
	A callback for PDGraphicEnumMonitor. It is called for every Q (save)
	operator.
	@param clientData IN/OUT User-supplied data that was passed in
	the call to PDPageEnumContents().
	@return <code>true</code> to continue enumeration, <code>false</code> to halt enumeration.

	@see PDPageEnumContents
*/
typedef ACCBPROTO1 ASBool (ACCBPROTO2 *PDGraphicEnumSaveProc)			(void *clientData);

/**
	A callback for PDGraphicEnumMonitor. It is called for every Q (restore)
	operator.
	@param clientData IN/OUT User-supplied data that was passed in
	the call to PDPageEnumContents().
	@return <code>true</code> to continue enumeration, <code>false</code> to halt enumeration.

	@see PDPageEnumContents
*/
typedef ACCBPROTO1 ASBool (ACCBPROTO2 *PDGraphicEnumRestoreProc)		(void *clientData);

/**
	A callback for PDGraphicEnumMonitor. It is called for every d0 (<code>setcharwidth</code>) operator.
	@param width IN/OUT Array of numbers containing the two parameters
	passed to the d0 operator.
	@param clientData IN/OUT User-supplied data that was passed in
	the call to PDPageEnumContents().
	@return <code>true</code> to continue enumeration, <code>false</code> to halt enumeration.
	@see PDPageEnumContents
*/
typedef ACCBPROTO1 ASBool (ACCBPROTO2 *PDGraphicEnumCharWidthProc)		(ASFixedPoint width, void *clientData);

/**
	A callback for PDGraphicEnumMonitor. It is called for every d1 (<code>setcachedevice</code>) operator.
	@param parms IN/OUT An array of numbers containing the 6 parameters
	passed to the d1 operator.
	@param clientData IN/OUT User-supplied data that was passed in
	the call to PDPageEnumContents().
	@return <code>true</code> to continue enumeration, <code>false</code> to halt enumeration.

	@see PDPageEnumContents
*/
typedef ACCBPROTO1 ASBool (ACCBPROTO2 *PDGraphicEnumCacheDeviceProc)	(ASFixed *parms, void *clientData);/* Added in Acrobat 3.0 */

/**
	A callback for PDGraphicEnumMonitor. It gets the current matrix
	for the subsequent XObject. It is called immediately before PDGraphicEnumXObjectRefProc().

	@param matrix IN/OUT (Filled by the callback) The current transformation
	matrix for the subsequent XObject whose name is obtained
	by PDGraphicEnumXObjectRefProc().
	@param clientData IN/OUT User-supplied data that was passed in
	the call to PDPageEnumContents().
	@return <code>true</code> to continue enumeration, <code>false</code> to halt enumeration.

	@see PDPageEnumContents
*/
typedef ACCBPROTO1 ASBool (ACCBPROTO2 *PDGraphicEnumXObjectRefMatrixProc)	(ASFixedMatrix *matrix, void *clientData);

typedef struct _t_PDGraphicEnumMonitor *PDGraphicEnumMonitor;
#if PLUGIN || ACROBAT_LIBRARY || TOOLKIT_LITE || PDFL_EXTENSION
/**
	An array of callbacks to pass to PDCharProcEnum(), PDFormEnumPaintProc() 
	or PDPageEnumContents(). One of the callbacks is called for 
	every renderable object in the page contents. Pass <code>NULL</code> 
	for a callback to not enumerate that type of object. Each 
	array element must be either <code>NULL</code> or a valid callback. Enumeration 
	of the page contents halts if the callback returns <code>false</code>. 
	
	@note PDPageEnumContents is provided only for backwards 
	compatibility. It has not been updated beyond PDF Version 
	1.1 and may not work correctly for newly-created PDF 1.2 
	or later files. You should use the PDFEdit API to enumerate 
	page contents. 

	@note In versions at least through Acrobat 2.1, enumeration 
	does not stop even if a method returns <code>false</code>. 
	@see PDCharProcEnum 
	@see PDFormEnumPaintProc 
	@see PDPageEnumContents 
*/
typedef struct _t_PDGraphicEnumMonitor {

	/** The size of the data structure. It must be set to <code>sizeof(PDGraphicEnumMonitorRec)</code>. */
	ASSize_t size;

	/** */
	PDGraphicEnumTextProc			EnumText;

	/** */
	PDGraphicEnumPathProc			EnumPath;

	/** */
	PDGraphicEnumImageProc			EnumImage;

	/** */
	PDGraphicEnumXObjectRefProc		EnumXObjectRef;

	/** */
	PDGraphicEnumSaveProc			EnumSave;

	/** */
	PDGraphicEnumRestoreProc		EnumRestore;

	/** */
	PDGraphicEnumCharWidthProc		EnumCharWidth;

	/** */
	PDGraphicEnumCacheDeviceProc	EnumCacheDevice;
	/* Added in Acrobat 3.0 */

	/** If non-<code>NULL</code>, <code>EnumXObjectRefMatrix</code> is called immediately before <code>EnumXObjectRef</code>.
		It returns the current matrix for the subsequent XObject ref.
	*/
	PDGraphicEnumXObjectRefMatrixProc	EnumXObjectRefMatrix;
} PDGraphicEnumMonitorRec;
#endif

/**
	A callback for PDPathEnumMonitor. It is called for every m operator.

	@param p1 IN/OUT The one point needed to specify the location
	to move to.
	@param clientData IN/OUT User-supplied data that was passed in
	the call to PDPathEnum().
	@return <code>true</code> to continue enumeration, <code>false</code> to halt enumeration.

	@see PDPathEnum
*/
typedef ACCBPROTO1 ASBool (ACCBPROTO2 *PDPathMoveToProc)	(ASFixedPoint *p1, void *clientData);

/**
	A callback for PDPathEnumMonitor. It is called for every l operator.

	@param p1 IN/OUT The one point needed to specify the line's ending
	point.
	@param clientData IN/OUT User-supplied data that was passed in
	the call to PDPathEnum().
	@return <code>true</code> to continue enumeration, <code>false</code> to halt enumeration.

	@see PDPathEnum
*/
typedef ACCBPROTO1 ASBool (ACCBPROTO2 *PDPathLineToProc)	(ASFixedPoint *p1, void *clientData);

/**
	A callback for PDPathEnumMonitor. It is called for every c operator.

	@param p1 The first point needed to specify the curve.
	@param p2 The second point needed to specify the curve.
	@param p3 The third point needed to specify the curve.
	@param clientData IN/OUT User-supplied data that was passed in
	the call to PDPathEnum().
	@return <code>true</code> to continue enumeration, <code>false</code> to halt enumeration.

	@see PDPathEnum
*/
typedef ACCBPROTO1 ASBool (ACCBPROTO2 *PDPathCurveToProc)	(ASFixedPoint *p1, ASFixedPoint *p2, ASFixedPoint *p3, void *clientData);

/**
	A callback for PDPathEnumMonitor. It is called for every v operator.

	@param p1 The first of two points needed to specify the curve.
	@param p2 The second of two points needed to specify the curve.
	@param clientData IN/OUT User-supplied data that was passed in
	the call to PDPathEnum().
	@return <code>true</code> to continue enumeration, <code>false</code> to halt enumeration.

	@see PDPathEnum
*/
typedef ACCBPROTO1 ASBool (ACCBPROTO2 *PDPathVCurveToProc)	(ASFixedPoint *p1, ASFixedPoint *p2, void *clientData);

/**
	A callback for PDPathEnumMonitor. It is called for every y operator.

	@param p1 The first of two points needed to specify the curve.
	@param p2 The second of two points needed to specify the curve.
	@param clientData IN/OUT User-supplied data passed in the call
	to PDPathEnum().
	@return <code>true</code> to continue enumeration, <code>false</code> to halt enumeration.

	@see PDPathEnum
*/
typedef ACCBPROTO1 ASBool (ACCBPROTO2 *PDPathYCurveToProc)	(ASFixedPoint *p1, ASFixedPoint *p2, void *clientData);

/**
	A callback for PDPathEnumMonitor. It is called for every re operator.

	@param p1 The first of two points needed to specify the rectangle.
	@param p2 The second of two points needed to specify the rectangle.
	@param clientData IN/OUT User-supplied data that was passed in
	the call to PDPathEnum().
	@return <code>true</code> to continue enumeration, <code>false</code> to halt enumeration.

	@see PDPathEnum
*/
typedef ACCBPROTO1 ASBool (ACCBPROTO2 *PDPathRectProc)		(ASFixedPoint *p1, ASFixedPoint *p2, void *clientData);

/**
	A callback for PDPathEnumMonitor. It is called for every path closing
	operator.
	@param clientData IN/OUT User-supplied data that was passed in
	the call to PDPathEnum().
	@return <code>true</code> to continue enumeration, <code>false</code> to halt enumeration.

	@see PDPathEnum
*/
typedef ACCBPROTO1 ASBool (ACCBPROTO2 *PDPathClosePathProc)(void *clientData);

typedef struct _t_PDPathEnumMonitor *PDPathEnumMonitor;

/**
	A data structure containing callbacks used by PDPathEnum(). 
	One callback is called for each path operator encountered; 
	the callback to call depends on the operator. 
	@see PDPathEnum 
*/
typedef struct _t_PDPathEnumMonitor {
	/** The size of the data structure. It must be set to <code>sizeof(PDPathEnumMonitorRec)</code>. */
	ASSize_t size;
	/** */
	PDPathMoveToProc		MoveTo;
	/** */
	PDPathLineToProc		LineTo;
	/** */
	PDPathCurveToProc		CurveTo;
	/** */
	PDPathVCurveToProc		VCurveTo;
	/** */
	PDPathYCurveToProc		YCurveTo;
	/** */
	PDPathRectProc			Rect;
	/** */
	PDPathClosePathProc		ClosePath;
} PDPathEnumMonitorRec;

/**
	A callback for PDTextEnum(). It is called once for each string in
	a text object.
	@param font IN/OUT The font used for <code>string</code>.
	@param string IN/OUT The string. It may be converted
	using PDFontXlateToHost() or PDFontXlateToUCS().
	@param stringLen IN/OUT The number of bytes in <code>string</code>.
	@param delta IN/OUT The difference, in thousandths of an EM, from
	the end of the previous string to the beginning of the current
	string. An EM is a typographic unit of measurement equal
	to the size of a font. For example, in a 12-point font,
	an EM is 12 points. See the description of the TJ operator
	in Section 5.3.2 in the <i>PDF Reference</i>.
	@param clientData IN/OUT User-supplied data that was passed in
	the call to PDTextEnum().
	@return <code>true</code> to continue enumeration, <code>false</code> to halt enumeration.

	@see PDTextEnum
*/
typedef	ACCBPROTO1 ASBool (ACCBPROTO2 *PDStringEnumProc)(PDFont font,
	char *string, ASInt32 stringLen, ASFixed delta, void *clientData);

/**
	A callback for PDXObjectEnumFilters(). It is called once for each
	filter that has been applied to an XObject's data.
	@param filter IN/OUT The filter's name.
	@param decodeParms IN/OUT The dictionary Cos object containing
	the filter's decode parameters.
	@param clientData IN/OUT User-supplied data that was passed in
	the call to PDXObjectEnumFilters().
	@return <code>true</code> to continue enumeration, <code>false</code> to halt enumeration.

	@see PDXObjectEnumFilters
*/
typedef	ACCBPROTO1 ASBool (ACCBPROTO2 *PDXObjectFilterEnumProc)(char *filter,
	CosObj decodeParms, void *clientData);

/**
	A callback for PDFontEnumCharProcs(). It is called once for
	each character in a Type 3 font.
	@param name IN/OUT The name of the current character.
	@param obj IN/OUT A stream Cos object containing the PDF drawing
	operators that draw the character.
	@param clientData IN/OUT User-supplied data that was passed in
	the call to PDFontEnumCharProcs().
	@return <code>true</code> to continue enumerating, <code>false</code> to halt enumeration.

	@see PDFontEnumCharProcs
*/
typedef	ACCBPROTO1 ASBool (ACCBPROTO2 *PDCharProcEnumProc)(char *name, PDCharProc obj,
	void *clientData);

/**
	A callback for PDXObjectGetData(). It is passed the XObject's
	data. Currently, the XObject's data is read 1 kB at a time
	and passed to this callback.
	@param data IN/OUT A buffer containing the XObject's data.
	@param lenData IN/OUT The amount of data in data in bytes.
	@param clientData IN/OUT User-supplied data that was passed in
	the call to PDXObjectGetData().
	@return <code>true</code> to continue reading the XObject's data, <code>false</code> to halt
	it.
	@see PDXObjectGetData
*/
typedef	ACCBPROTO1 ASBool (ACCBPROTO2 *PDGetDataProc)(char *data, ASUns32 lenData, void *clientData);


/************************************************************************************\
|*																					*|
|*								PDTextSelect										*|
|*																					*|
\************************************************************************************/

/**
	A data structure representing a single entry (the starting location and length) in a highlight
	list.
	@see PDTextSelectCreatePageHilite
	@see PDTextSelectCreateWordHilite
*/
typedef struct _t_HiliteEntry {
	/** */
	PDCharOffset	offset;
	/** */
	PDCharOffset	length;
} HiliteEntry;

/*
** PDTextSelectRange
**
** Word- and character-oriented text selection.  
*/

/**
	A data structure used to specify a range of text in a text 
	selection. 

	<p>Use <code>0</code> for <code>ofsStart</code> and <code>ofsEnd</code> for whole-word selections. 
	Nonzero values for <code>ofsStart</code> and <code>ofsEnd</code> are supported by 
	PDText but are currently ignored by the Acrobat viewer's 
	user interface code (which highlights only whole-word selections). 
	If <code>ofsEnd</code> is <code>0</code>, <code>end</code> is the first word not selected. </p>
	@see PDTextSelectCreateRanges 
	@see PDTextSelectGetRange 
*/
typedef struct _t_PDTextSelectRange {
	/** A word containing the start of the selection. */
	ASInt32	start;			
	/** A word containing the end of the selection. */
	ASInt32	end;			
	/** An offset into the word at the start of the selection. */
	ASInt32	ofsStart;		
	/** An offset into the word at the end of the selection. */
	ASInt32	ofsEnd;			
} PDTextSelectRangeRec, *PDTextSelectRange;

/**
	A callback for PDTextSelectEnumQuads(). It is called once for each
	quad in a text selection.
	@param procObj IN/OUT User-supplied data that was passed in the
	call to PDTextSelectEnumQuads().
	@param page IN/OUT The page on which the text selection is located.

	@param quad IN/OUT The quad being enumerated.
	@return <code>true</code> to continue enumeration, <code>false</code> to halt enumeration.

	@see PDTextSelectEnumQuads
*/
typedef ACCBPROTO1 ASBool (ACCBPROTO2 *PDTextSelectEnumQuadProc)(void *procObj,
	ASInt32 page, ASFixedQuad *quad);

/**
	A callback for PDTextSelectEnumText() and PDTextSelectEnumTextUCS().
	It is called once for each text run (which is text in the same font, size,
	color, and on the same line) in a text selection.
	@param procObj IN/OUT User-supplied data that was passed in the
	call to PDTextSelectEnumText() or PDTextSelectEnumTextUCS().

	@param font IN/OUT The text's font.
	@param size IN/OUT The text's size in points.
	@param color IN/OUT The text's color.
	@param text IN/OUT The text in the current run. This string
	is not necessarily <code>NULL</code>-terminated.
	@param textLen IN/OUT The number of bytes in text.
	@return <code>true</code> to continue enumeration, <code>false</code> to halt enumeration.

	@see PDTextSelectEnumText
	@see PDTextSelectEnumTextUCS
*/
typedef ACCBPROTO1 ASBool (ACCBPROTO2 *PDTextSelectEnumTextProc)(void *procObj,
	PDFont font, ASFixed size, PDColorValue color, char *text, ASInt32 textLen);
	
/** */
typedef ACCBPROTO1 ASBool (ACCBPROTO2 *PDTextSelectEnumRTFTextProc)(void *procObj,
	PDFont font, ASFixed size, PDColorValue color, char *text, ASUns32 rtfCntFlag, ASInt32 textLen);

/************************************************************************************\
|*																					*|
|*							PDThread & PDBead										*|
|*																					*|
\************************************************************************************/

/**
	An article in the Acrobat viewer's user interface. It contains an ordered sequence of
	rectangles that bound the article. Each rectangle is called a bead. Threads can be
	created either interactively, by the user, or programmatically.
	@see PDDocGetThread
	@see PDThreadNew
	@see PDThreadFromCosObj
	@see PDBeadGetThread
	@see PDDocRemoveThread
	@see PDThreadDestroy
*/
typedef OPAQUE_64_BITS PDThread;

/**
	A single rectangle in an article thread. (Article threads are known simply as articles in
	the Acrobat viewer's user interface). A bead remains valid as long as a thread is
	<i>current</i> and <i>active</i>.
	@see AVPageViewGetActiveBead
	@see AVPageViewIsBeadAtPoint
	@see PDBeadNew
	@see PDBeadGetNext
	@see PDBeadGetPrev
	@see PDThreadGetFirstBead
	@see PDBeadDestroy
*/
typedef OPAQUE_64_BITS PDBead;

/************************************************************************************\
|*																					*|
|*									PDThumb											*|
|*																					*|
\************************************************************************************/

/*
** PDThumbCreationServer
** The create thumb server is passed in at thumb creation time.
*/

/**
	(Optional) A callback for PDThumbCreationServer. It is called before
	processing each page. It may be <code>NULL</code>.
	@param pageNum IN/OUT The page for which to create a thumbnail
	image.
	@param clientData IN/OUT User-supplied data that was passed in
	the call to PDDocCreateThumbs().
	@return <code>true</code> to continue thumbnail image creation, <code>false</code> to halt
	thumbnail image creation.
	@see PDDocCreateThumbs
*/
typedef ACCBPROTO1 ASBool (ACCBPROTO2 *PDThumbCreationNotifyPageProc)(ASInt32 pageNum,
	void *clientData);

/**
	(Optional) A callback for PDThumbCreationServer. It is called for
	each page that does not currently contain a thumbnail image.
	It may be <code>NULL</code>. If it is <code>NULL</code>, the thumbnail data is generated
	by the default thumbnail generator.
	@param page IN/OUT The page for which to create a thumbnail image.

	@param thumbScale IN/OUT The scale to map from the page size to
	the thumbnail size, which is either 1/8 of
	the page size or is limited to <code>MAX_THUMBPAGE_WIDTH</code> and
	<code>MAX_THUMBPAGE_HEIGHT</code>, whichever is smaller.
	@param width IN/OUT The width of the thumbnail image to create.

	@param height IN/OUT The height of the thumbnail image to create.

	@param thumbData IN/OUT A buffer into which the thumbnail data
	is copied. This buffer has the following values: 
	<ul>
	<li><code>rowBytes = (width *	bitsPerPixel + 7) / 8; </code></li>
	<li><code>size = rowBytes * height; </code></li>
	</ul>
	<p>where <code>bitsPerPixel</code>
	is specified as <code>numComponents * bitsPerComponent</code>. <code>numComponents</code>
	is dependent upon the color space. For DeviceRGB, <code>numComponents</code>
	is <code>3</code>. For an indexed color space, <code>numComponents</code> is <code>1</code>.</p>
	@param clientData IN/OUT User-supplied data that was passed in
	the call to PDDocCreateThumbs().
	@return <code>true</code> to continue thumbnail image creation, <code>false</code> to halt
	thumbnail image creation.
	@see PDDocCreateThumbs
*/
typedef ACCBPROTO1 ASBool (ACCBPROTO2 *PDThumbCreationGetThumbDataProc)(
	PDPage page, ASFixed thumbScale, ASInt32 width, ASInt32 height, void *thumbData,
	void *clientData);

/**
	(Optional) A callback for PDThumbCreationServer. It is called after
	PDThumbCreationGetThumbDataProc() and after a PDThumb has
	been created. It gives the server a chance to draw the thumbnail
	image in a status window. It may be <code>NULL</code>.
	@param thumb IN/OUT The thumbnail image to draw.
	@param clientData IN/OUT User-supplied data that was passed in
	the call to PDDocCreateThumbs().
	@see PDThumbCreationGetThumbDataProc
	@see PDDocCreateThumbs
*/
typedef ACCBPROTO1 void (ACCBPROTO2 *PDThumbCreationDrawThumbProc)(PDThumb thumb,
	void *clientData);

typedef struct _t_PDThumbCreationServer *PDThumbCreationServer;
/**
	A data structure containing callbacks that implement a creation server. The callbacks
	implement the creation server functions.
	@see PDDocCreateThumbs
*/
typedef struct _t_PDThumbCreationServer {
	/** Set this field to <code>sizeof(PDThumbCreationServerRec)</code>. */
	ASSize_t size; 
	/** */
	PDThumbCreationNotifyPageProc	NotifyPage;
	/** */
	PDThumbCreationGetThumbDataProc	GetThumbData;
	/** */
	PDThumbCreationDrawThumbProc	DrawThumb;
} PDThumbCreationServerRec;

/************************************************************************************\
|*																					*|
|*							PDViewDestination										*|
|*																					*|
|*  Opaque designator for a view destination object. Note that this API does not    *|
|*  export routines that construct or acquire view destination objects, just the    *|
|*  definition of the object and its methods. The client may freely copy or destroy *|
|*  an instance of a PDViewDestination; two instances of a PDViewDestination        *|
|*  designating the same view destination are guaranteed to contain the identical   *|
|*  bits.                                                                           *|
|*																					*|
\************************************************************************************/

/**
	A particular view of a page in a document. It contains a reference to a page, a
	rectangle on that page, and information specifying how to adjust the view to fit the
	window's size and shape. It corresponds to a PDF Dest array and can be considered
	a special form of a PDAction.
	@see AVPageViewToViewDest
	@see PDActionGetDest
	@see PDViewDestCreate
	@see PDViewDestFromCosObj
	@see PDViewDestResolve
	@see PDViewDestDestroy
*/
typedef OPAQUE_64_BITS PDViewDestination;

typedef OPAQUE_64_BITS PDStructureDestination;
    
/*
** Value indicating a null entry for VDXYZ destination attributes
*/
#define PDViewDestNULL		fixedNegativeInfinity


/************************************************************************************\
|*																					*|
|*									PDWord											*|
|*																					*|
\************************************************************************************/

/**
	Extracts words from a PDF file, and enumerates the words on a single page or on all
	pages in a document.
	@see PDDocCreateWordFinder
	@see PDDocCreateWordFinderUCS
	@see PDDocGetWordFinder
	@see PDWordFinderDestroy
	@see PDWordFinderEnumWords
*/
typedef struct _t_PDWordFinder *PDWordFinder;

/**
	A word in a PDF file. Each word contains a sequence of characters in one or more
	styles (see PDStyle).
	@see PDWordFinderGetNthWord
	@see PDWordFinderEnumWords
	@see PDWordFinderEnumWords
*/
typedef struct _t_PDWord *PDWord;

/**
	Provides access to information about the fonts, font sizes, and colors used in a
	PDWord.
	@see PDWordGetNthCharStyle
*/
typedef struct _t_PDStyle *PDStyle;

/**
	A callback for PDWordFinderEnumWords. It is called once for each
	word.
	@param wObj IN/OUT The word finder.
	@param wInfo IN/OUT The current word in the enumeration.
	@param pgNum IN/OUT The page number on which <code>wInfo</code> is located.
	@param clientData IN/OUT User-supplied data that was passed in
	the call to PDWordFinderEnumWords().
	@return <code>true</code> to continue enumeration, <code>false</code> to halt enumeration.

	@see PDWordFinderEnumWords
*/
typedef ACCBPROTO1 ASBool (ACCBPROTO2 *PDWordProc)( PDWordFinder wObj, PDWord wInfo,
	ASInt32 pgNum, void *clientData );

/**
  	PDFindTranslateStringProc() is passed to PDFindText().
*/
typedef ACCBPROTO1 ASBool (ACCBPROTO2 *PDFindTranslateStringProc)(char *string,
	ASInt32 stringLength, PDWord pdWord, void *clientData);

/* Wordy Character types */

/** A control code.
	@ingroup CharacterTypeCodes
*/
#define W_CNTL             0x1

/** A lowercase letter.	
	@ingroup CharacterTypeCodes
*/
#define W_LETTER           0x2

/** An uppercase letter. 
	@ingroup CharacterTypeCodes
*/
#define W_UPPERCASE        0x4

/** A digit. 
	@ingroup CharacterTypeCodes
*/
#define W_DIGIT            0x8

/** A punctuation mark. 
	@ingroup CharacterTypeCodes
*/
#define W_PUNCTUATION      0x10

/** A hyphen. 
	@ingroup CharacterTypeCodes
*/
#define W_HYPHEN           0x20

/** A hyphen that is only present because a word is broken across two lines of text. 
	@ingroup CharacterTypeCodes
*/
#define W_SOFT_HYPHEN      0x40

/** A ligature. 
	@ingroup CharacterTypeCodes
*/
#define W_LIGATURE         0x80

/** A white space glyph. 
	@ingroup CharacterTypeCodes
*/
#define W_WHITE            0x100

/** A comma. Commas and periods are treated separately
	from other punctuation marks because they are used
	both as word punctuation marks and as delimiters in
	numbers, and need to be treated differently in the two
	cases.
	@ingroup CharacterTypeCodes
*/
#define W_COMMA            0x200

/** A period. 
	@ingroup CharacterTypeCodes
*/
#define W_PERIOD           0x400

/** An accent mark. 
	@ingroup CharacterTypeCodes
*/
#define W_ACCENT           0x800

/** A glyph that cannot be represented in the destination font encoding. 
	@ingroup CharacterTypeCodes
*/
#define W_UNMAPPED         0x1000

/** An end-of-phrase glyph (for example, <code>"."</code>, <code>"?"</code>, <code>"!"</code>, <code>":"</code>, and <code>";"</code>). 
	@ingroup CharacterTypeCodes
*/
#define W_END_PHRASE       0x2000

/** A wildcard glyph (for example, <code>"*"</code> and <code>"?"</code>) that should not be treated as a normal punctuation mark.
	@ingroup CharacterTypeCodes
*/
#define W_WILD_CARD        0x4000

/** A glyph that acts as a delimiter between words. 
	@see GlyphNamesofWordSeparators 
	@ingroup CharacterTypeCodes
*/
#define W_WORD_BREAK       0x8000


/* WEChar encoding information flags (8-bit) */
#define WXE_ENC_UNMAPPED	0x01	/* The font has no useful encoding information. Word Finder copied
			the original character string from the PDF content. In this case, Word Finder always assumes
			the original string is single-byte characters. If the Word Finder is extracting the text in Unicode,
			it inserts additional bytes, 0x00s, to form 16-bit characters. (i.e. "0x41, 0x42" a "0x00, 0x41,
			0x00, 0x42") A common example of this case is a Symbol font without ToUnicode table.Since there is
			no valid Unicode values are available, during the word-finding process, these characters are treated
			as symbolic characters and each character becomes a word. */
#define WXE_ENC_MISSING		0x02	/* The character code is not available in the output encoding space.
									   This character is replaced by a space character. */
#define WXE_ENC_NO_UCS		0x04	/* Word Finder is not able to find a reliable Unicode value
			from the character. The character encoding is determined by "guessing" due to insufficient
			encoding information available in the PDF file. */
#define WXE_FROM_ACTUALT	0x08	/* The character comes from an ActualText, rather than the page content. */


/* Word's Flags (Group ID = 0) */

/** The word contains a character outside the range of A-Z, a-Z, 0-9.
	@ingroup WordAttributes
*/
#define WXE_HAS_NONALPHANUM     0X1

/** The word contains a character between A-Z or a-z.
	@ingroup WordAttributes
*/
#define WXE_HAS_LETTER			0x2

/** The word contains a character between A-Z.
	@ingroup WordAttributes
*/
#define WXE_HAS_UPPERCASE		0x4

/** One or more characters in the word are digits.
	@ingroup WordAttributes
*/
#define WXE_HAS_DIGIT           0x8

/** One or more characters in the word are
	punctuation marks. Other flag bits can be
	checked to test whether the punctuation was at
	the beginning of the word
	(<code>WXE_HAS_LEADING_PUNC</code>), the end of the word
	(<code>WXE_HAS_TRAILING_PUNC</code>), or elsewhere in
	the word.
	@ingroup WordAttributes
*/
#define WXE_HAS_PUNCTUATION     0x10

/** There is a hyphen in the word.
	@ingroup WordAttributes
*/
#define WXE_HAS_HYPHEN          0x20

/** There is a soft hyphen in the word.
	@ingroup WordAttributes
*/
#define WXE_HAS_SOFT_HYPHEN     0x40

/** The word contains a ligature.
	@ingroup WordAttributes
*/
#define WXE_HAS_LIGATURE        0x80

/** The first character in the word is a punctuation
	mark. If this bit is set, <code>WXE_HAS_PUNCTUATION</code>
	will also be set.
	@ingroup WordAttributes
*/
#define WXE_HAS_LEADING_PUNC    0x100

/** The last character in the word is a punctuation
	mark. If this bit is set, <code>WXE_HAS_PUNCTUATION</code>
	will also be set.
	@ingroup WordAttributes
*/
#define WXE_HAS_TRAILING_PUNC   0x200

/** One or more characters in the word cannot be
	represented in the output font encoding.
	@ingroup WordAttributes
*/
#define WXE_HAS_UNMAPPED_CHAR   0x400

/** The character following the end of the word is a
	space (either an explicit space character encoded
	in a string, or one that appears implicitly because
	the drawing point was moved). 
	@ingroup WordAttributes
*/
#define WXE_ADJACENT_TO_SPACE   0x800
/* Extended wFlags */

/** The writing direction of the word is not in a
	multiple of 90 degrees, or the bounding box of the
	text is skewed. This flag indicates that the quads
	of the word should be used to specify the highlight
	area correctly.
	@ingroup WordAttributes
*/
#define WXE_ROTATED   			0x1000

/** The writing direction of the word is either 90 or
	180 degrees. This flag ignores the page rotation
	parameter of the page dictionary. Therefore, if the
	page is rotated 90 degrees, this flag will be set on
	each word that appears horizonally on the screen.
	@ingroup WordAttributes
*/
#define WXE_VERTICAL_FLOW       0x2000

/** 
	@ingroup WordAttributes
*/
#define WXE_WBREAK_WORD         0x4000

/** The word is at the end of the current text line (for example,
	the word is followed by a line break).
	@ingroup WordAttributes
*/
#define WXE_LAST_WORD_ON_LINE   0x8000

/* Word's Flags (Group ID = 1) */
#define WXE_FRONT_TAB				0x01	/* Insert a tab character before this word */
#define WXE_ENCODING_WARNING		0x02	/* Unreliable encoding conversion happened in this word.
											   Check the encoding flags of each character for detail. */
#define WXE_REVERSE_DIRECTION		0x04	/* The writing direction is right-left or top-down */
#define WXE_WORD_IS_UNICODE			0x08	/* The text is in Unicode format. */
#define WXE_EXT_CHAR_OFFSETS		0x10	/* The word has extened character offset information and
											   can be used for character based PDWord methods like
											   PDWordGetCharOffsetEx(), PDWordGetCharQuad(),
											   PDWordGetNumHiliteChar(), and PDWordGetByteIdxFromHiliteChar(). */
/* 0x0100 ~ 0x8000 are reserved for Acrobat internal usage. */


/* Context  flags */
/**
*/
#define  WXE_STREAM          0x1

/**
*/
#define  WXE_PDF_ORDER       0x2

/**
*/
#define  WXE_XY_SORT         0x4

/**
*/
#define  WXE_RD_ORDER_SORT   0x8

/** Used to obtain the latest available version.
*/
#define WF_LATEST_VERSION     0

/** The version used for Acrobat 3.x, 4.x.
*/
#define WF_VERSION_2	2	

/** For Acrobat 5.0 without accessibility enabled. 
*/
#define WF_VERSION_3	3

/** For Acrobat 5.0 with accessibility enabled. 
*/
#define WF_VERSION_4	4	

/**
	A callback for PDEnumDocs(). It is called once for each open
	PDDoc.
	@param pdDoc IN/OUT The PDDoc currently being enumerated.
	@param clientData IN/OUT User-supplied data that was passed in
	the call to PDEnumDocs().
	@return <code>true</code> to continue enumeration, <code>false</code> to halt enumeration.

	@see PDEnumDocs
*/
typedef ACCBPROTO1 ASBool (ACCBPROTO2 *PDDocEnumProc)(PDDoc pdDoc, void *clientData);

/**
	A word finder configuration that customizes the way the 
	extraction is performed. In the default configuration, all 
	options are <code>false</code>. 
	@see PDDocCreateWordFinderEx 
*/
typedef struct _t_PDWordFinderConfig {

	/** This is always <code>sizeof(PDWordFinderConfigRec)</code>. */
	ASSize_t		recSize;	
	/** When <code>true</code>, it disables tagged PDF 
		support and treats the document as non-tagged PDF. Use this 
		to keep the word finder in legacy mode when it is created 
		with the latest algorithm version (WF_LATEST_VERSION).
	*/
	ASBool			disableTaggedPDF;	

	/** When <code>true</code>, it disables generating an XY-ordered 
		word list. This option replaces the sort order flags in 
		the older version of the word finder creation command (PDDocCreateWordFinder()). 
		Setting this option is equivalent to omitting the <code>WXE_XY_SORT</code> 
		flag. 
	*/
	ASBool			noXYSort;
	/** When <code>true</code>, the word finder preserves 
		space characters during word breaking. Otherwise, spaces 
		are removed from output text. When <code>false</code> (the default), you 
		can add spaces later by considering the word attribute flag 
		<code>WXE_ADJACENT_TO_SPACE</code>, but there is no way to restore the 
		exact number of consecutive space characters.
	*/
	ASBool			preserveSpaces;		

	/** 
		<p>When <code>true</code>, and the font has a ToUnicode table, it disables the expansion 
		of ligatures using the default ligatures. The default ligatures 
		are:</p>

		<ul>
		<li>fi</li> 
		<li>ff</li> 
		<li>fl</li> 
		<li>ffi</li> 
		<li>ffl</li> 
		<li>st</li> 
		<li>oe</li> 
		<li>OE</li>
		</ul>
		
		<p>When <code>noLigatureExp</code> is <code>true</code> and the font does not have a
		ToUnicode table, the ligature is expanded based on whether there is a representation
		of the ligature in the defined <code>codePage</code>. If there is no representation,
		the ligature is expanded; otherwise, the ligature is not expanded.</p>
	*/
	ASBool			noLigatureExp;		

	/** When <code>true</code>, it disables guessing encoding 
		of fonts that have unknown or custom encoding when there 
		is no ToUnicode table. Inappropriate encoding conversions 
		can cause the word finder to mistakenly recognize non-Roman 
		single-byte fonts as Standard Roman encoding fonts and extract 
		the text in an unusable format. When this option is selected, 
		the word finder avoids such unreliable encoding conversions 
		and tries to provide the original characters without any 
		encoding conversion for a client with its own encoding handling. 
		Use the PDWordGetCharEncFlags() method to detect such characters. 
	*/
	ASBool			noEncodingGuess;	

	/** When <code>true</code>, it assumes any font with 
		unknown or custom encoding to be Standard Roman. This option 
		overrides the <code>noEncodingGuess</code> option.
	*/
	ASBool			unknownToStdEnc;	

	/** When <code>true</code>, it disables converting large 
		character gaps to space characters, so that the word finder 
		reports a character space only when a space character appears 
		in the original PDF content. This option has no effect on 
		tagged PDF. 
	*/
	ASBool			ignoreCharGaps;		

	/** When <code>true</code>, it disables treating vertical 
		movements as line breaks, so that the word finder determines 
		a line break only when a line break character or special 
		tag information appears in the original PDF content. This 
		option has no effect on tagged PDF. 
	*/
	ASBool			ignoreLineGaps;		

	/** When <code>true</code>, it disables extracting text from 
		text annotations. Normally, the word finder extracts text 
		from the normal appearances of text annotations that are 
		inside the page crop box. 
	*/
	ASBool			noAnnots;			

	/** When <code>true</code>, it disables finding and 
		removing soft hyphens in non-tagged PDF, so that the word 
		finder trusts hard hyphens as non-soft hyphens. This option 
		has no effect on tagged PDF files. Normally, the word finder 
		does not differentiate between soft and hard hyphen characters in 
		non-tagged PDF files, because these are often misused. 
	*/
	ASBool			noHyphenDetection;	

	/** When <code>true</code>, it disables treating non-breaking 
		space characters as regular space characters in non-tagged 
		PDF files, so that the word finder preserves the space without 
		breaking the word. This option has no effect on tagged PDF 
		files. Normally, the word finder does not differentiate between 
		breaking and non-breaking space characters in non-tagged 
		PDF files, because these are often misused. 
	*/
	ASBool			trustNBSpace;		

	/** When <code>true</code>, it disables generating 
		extended character offset information to improve text extraction 
		performance. The extended character offset information is 
		necessary to determine exact character offset for character-by-character 
		text selection. The beginning character offset of each word 
		is always available regardless of this option, and can be 
		used for word-by-word text selection with reasonable accuracy. 
		When a client has no need for the detailed character offset 
		information, it can use this option to improve the text 
		extraction efficiency. There is a minor difference in the 
		text extraction performance, and less memory is needed for 
		the extracted word list.
	*/
	ASBool			noExtCharOffset;	

	/** When <code>true</code>, it disables generating character 
		style information to improve text extraction performance 
		and memory efficiency. When you select this option, you 
		cannot use PDWordGetNthCharStyle() and PDWordGetStyleTransition() 
		with the output of the word finder.
	*/
	ASBool			noStyleInfo;		

	/**
		A custom UTF-16 decomposition table. 
		This table can be used to expand Unicode ligatures not included 
		in the default ligature list. Each decomposition record 
		contains a UTF-16 character code (either a 16-bit or 32-bit 
		surrogate), a replacement UTF16 string, and the delimiter 
		<code>0x0000</code>. 
		
		@example <code>const ASUns16 myDecompTbl[] = {0x00b2, 0x0032, 0x0000, 0x00c6, 0x0061, 0x0065, 0x0000, 0xFB01, 0xFB01, 0x0000}; </code>
		<p>This replaces superscript <code>'2'</code> with <code>'2'</code>, 
		the <code>'AE'</code> ligature with <code>'a'</code> and <code>'e'</code>, and disables the default 
		handling of the <code>'fi'</code> ligature. The word finder applies this 
		substitution after identifying the character types, so the 
		word-breaking process uses the character attributes of the 
		original, rather than the replacement characters. See <code>charTypeTble</code> 
		below. </p>
	*/
	const ASUns16	*decomposeTbl;		

	/** The size of the <code>decomposeTbl</code> in bytes. */
	ASSize_t		decomposeTblSize;	

	/**
		A custom character type table to enhance 
		word breaking quality. Each character type record contains 
		a region start value, a region end value, and a character 
		type flag as defined in PDExpT.h. A character code is in 
		UTF-16, and is either a 16-bit or a 32-bit surrogate. 
		
		@example <code>const ASUns16 myCharTypeTbl[] = {0x0082, 0x0082, W_CNTL+W_WORD_BREAK, 0x00b2, 0x00b3, W_DIGIT}</code> 
		<p>This identifies <code>0x0082</code> as a control-plus-word 
		break character, and <code>0x00b2</code> and <code>b3</code> as digits. If you need 
		to change a character's type along with its character code, 
		you must define the original character code in the custom 
		character type table. For example, if <code>'a'</code> is transformed 
		to <code>'1'</code> in the decomposition table, <code>'a'</code> should be transformed 
		to <code>W_DIGIT</code> here, so that the word finder can recognize the 
		replaced character, <code>'1'</code>, as a number. </p>
	*/
	const ASUns16	*charTypeTbl;		

	/** The size of the <code>charTypeTbl</code> in bytes. */
	ASSize_t		charTypeTblSize;	

	/** When <code>true</code>, it disables detecting and removing redundant characters.
		Some PDF pages have the same text drawn multiple times on the same spot 
		to get a special visual effect. Normally, those redundant characters are 
		removed from the word finder output.
		<p>Since this option may leave extra characters with overlapping 
		bounding boxes, using it together with the <code>disableCharReordering</code> option 
		is recommended for more consistent text extraction results.</p>
	*/
	ASBool			preserveRedundantChars;

	/** When <code>true</code>, it disables reconstructing the character orders, 
		and the word finding algorithm is applied to the characters 
		in the drawing order. By default, word finder reorders characters 
		on a single line by the relative horizontal character locations. 
		Most of the time, the character reordering feature improves 
		the text extraction quality. However, on a PDF page with heavily 
		overlapped character bounding boxes, the outcome becomes somewhat 
		unpredictable. In such case, disabling the character reordering 
		(<code>disableCharReordering = true</code>) may produce a more static result.
	*/
	ASBool			disableCharReordering;
} PDWordFinderConfigRec, *PDWordFinderConfig;

/** This is passed to PDWordFinderSetCtrlProc().
	<p>This is the callback function called by Word Finder when its page enumeration process takes longer
	than the specified time (in seconds). Return <code>true</code> to continue the enumeration process, or <code>false</code>
	to stop. <code>startTime</code> is the value that was set by ASGetSecs() when the Word Finder started processing
	the current page.</p>
*/
typedef ACCBPROTO1 ASBool (ACCBPROTO2 *PDWordFinderCtrlProc)(ASUns32 startTime, void *clientData);

/*------------------------------------------------------------------------
	Color calibration types.
------------------------------------------------------------------------*/

/**
	A data structure containing a monitor's chromaticity, for use in displaying device-independent
	color.
	<p><code>x</code> and <code>y</code> are the two values needed to specify the chromaticity.</p>
	@see PDColorCalP
	@see PDPrefGetColorCal
	@see PDPrefSetColorCal
*/
typedef struct _t_PDChromaticity
{

	/** The x-axis component of the monitor's chromaticity. It must be in [0,1). <code>x + y</code> must be less than or equal to 1.*/
	ASFixed	x;

	/** The y-axis component of the monitor's chromaticity. It must be in (0,1]. <code>x + y</code> must be less than or equal 1. */
	ASFixed	y;
} PDChromaticity;

/** A data structure used to represent the characteristics of an output device; it is needed for
	device-independent color.
	@see PDPrefGetColorCal
	@see PDPrefSetColorCal
*/
typedef struct _t_PDColorCal
{
	/** */
	PDChromaticity 	whiteChrom;
	/** */
	PDChromaticity 	redChrom;
	/** */
	PDChromaticity 	greenChrom;
	/** */
	PDChromaticity 	blueChrom;
	/** */
	ASFixed		redGamma;
	/** */
	ASFixed		greenGamma;
	/** */
	ASFixed		blueGamma;
} PDColorCal, *PDColorCalP;

/*------------------------------------------------------------------------
	PDPageStm types
------------------------------------------------------------------------*/

#define kPDPageStmStringMax	256

/* Flag values for PDPageStmGetToken */
#define kPDPageStmSkipComments	0x0001

/* Flag values for PDPageStmGetInlineImage */
/* -- none defined yet -- */

/* Flag values for PDPageStmToken */
#define kPDPageStmTokenHexString	0x0001

/**
	A data structure used by PDPageStmGetToken(). It contains information about the
	current page contents token.

	@see PDPageStmGetToken
*/
typedef struct _t_PDPageStmToken {
	/** The size of this record. It is to be filled in by the caller. */
	ASSize_t	size;	
	/** The type of token. */
	CosType	type;		
	/** Additional information about the token (for example, indicating a hex string). */
	ASUns32	flags;		
	/** The value of the token if it is a CosInteger, ASFixed, or Name. */
	ASInt32	iVal;		
	/** The value of the token if it is a CosString. */
	char	sVal[kPDPageStmStringMax];	
	/** The number of bytes in <code>sVal</code>. */
	ASSize_t	sValLen;	
} PDPageStmTokenRec, *PDPageStmToken;

/**
	A callback used by PDPageStmGetToken(). It is called when the
	length of a string token exceeds <code>kPDPageStmStringMax</code> bytes
	(see PDExpT.h) in PDPageStmGetToken().
	@param sVal IN/OUT The string value read so far.
	@param sValLen IN/OUT The length of <code>sVal</code> in bytes.
	@param clientData IN/OUT User-supplied data that was passed in
	the call to PDPageStmGetToken().
	@see PDPageStmGetToken
*/

#if !defined(__SUNPRO_CC) || !defined(_H_THREADSAFEDATA)
//SunStudio doesn't accept multiple definition if one is under extern "C" while other is not.
typedef ACCBPROTO1 void (ACCBPROTO2 *PDPageStmStringOverflowProc)(char *sVal,
	ASSize_t sValLen, void *clientData);
#endif

/**
	A callback for PDPageStmGetInlineImage(). It should be called when
	inline image data is encountered in PDPageStmGetToken(). This
	method may be called multiple times for one inline image.
	If so, each call provides sequential data for the image.

	@param data IN/OUT The image data read so far.
	@param dataLen IN/OUT The length of <code>data</code> in bytes.
	@param clientData IN/OUT User-supplied data that was passed in
	the call to PDPageStmGetInlineImage() (which may have been
	passed in the PDPageStmGetToken() method).
	@return <code>true</code> to continue reading the image's data, <code>false</code> to stop
	reading.
	@see PDPageStmGetInlineImage
	@see PDPageStmGetToken
*/

#if !defined(__SUNPRO_CC) || !defined(_H_THREADSAFEDATA)
//SunStudio doesn't accept multiple definition if one is under extern "C" while other is not.
typedef ACCBPROTO1 ASBool (ACCBPROTO2 *PDPageStmImageDataProc)(ASUns8 *data,
	ASSize_t dataLen, void *clientData);
#endif

/**
	A transition to a page. The Trans key in a Page dictionary specifies a Transition
	dictionary, which describes the effect to use when going to a page and the amount of
	time the transition should take.
	@see PDPageGetTransition
	@see PDTransFromCosObj
	@see PDTransNew
	@see PDTransNewFromCosDoc
	@see PDTransNull
*/
typedef OPAQUE_64_BITS PDTrans;

#define fxDefaultPageDuration	-fixedOne
#define fxDefaultTransDuration	fixedOne

/*------------------------------------------------------------------------
	PDDocReadAhead types
------------------------------------------------------------------------*/

/* Flag values for PDDocReadAhead */
/**	Allows the AcroForm client to request that 
	all the AcroForm data be read <i>ahead</i>, before the viewer 
	needs it. This flag is ignored if the PDF file does not 
	contain a Forms hint table. See Section F.3.5 in the PDF 
	Reference for information about the Forms hint table. 
	
*/
#define kPDDocReadAheadAcroForms	0x0001

/**	Requests that the PDF file's Forms Template data be read 
	<i>ahead</i>, before the viewer needs it. There is currently 
	no Template hint table defined, so this flag simply causes 
	the rest of the file to be read. 
	
*/
#define kPDDocReadAheadTemplates	0x0002

/**	Requests that the PDF file's page label data be read <i>ahead</i>, 
	before the viewer needs it. There is currently no page label 
	hint table defined, so this flag simply causes the rest 
	of the file to be read. 
	
*/
#define kPDDocReadAheadPageLabels	0x0004

/**	Requests that the PDF file's logical structure data be read 
	<i>ahead</i>, before the viewer needs it. There is currently 
	no logical structure hint table defined, so this flag simply 
	causes the rest of the file to be read.
*/
#define kPDDocReadAheadStructure	0x0008

/**
*/
#define kPDDocReadAheadRenditions	0x0010

/*------------------------------------------------------------------------
	PD Related Security and Encryption: New permission methods
------------------------------------------------------------------------*/

/** 
	The set of valid PDPermRequestStatus values providing the status of 
	PDDoc-related permissions methods.
	@see PDDocPermRequest
	@see PDCryptAuthorizeExProc
	@see PDCryptGetAuthDataExProc
*/
typedef ASInt16	PDPermReqStatus;
/** <code>-1</code> The request was denied.*/
#define PDPermReqDenied (PDPermReqStatus)(-1)
/** <code>0</code> The request was granted.*/
#define PDPermReqGranted (PDPermReqStatus)(0)
/** <code>1</code> The object is unknown.*/
#define PDPermReqUnknownObject (PDPermReqStatus)(1)
/** <code>2</code> The operation is unknown.*/
#define PDPermReqUnknownOperation (PDPermReqStatus)(2)
/** <code>3</code> The operation is not applicable for the specified object.*/
#define PDPermReqOperationNA (PDPermReqStatus)(3)	/* Not applicable operation for specified object */
/** The handler does not have enough information to determine an answer 
    at this point. Try again later. */
#define PDPermReqPending (PDPermReqStatus)(4)

/* The version of the PDPermReqObj and PDPermReqOpr */
#define PDPermReqVersion    0x0004

/**	The document object type. The applicable operations are:
	<ul>
	<li><code>PDPermReqOprAll </code></li>
	<li><code>PDPermReqOprModify</code> (Doc Info, open action, page label, modifying document)</li> 
	<li><code>PDPermReqOprCopy</code> (Copy to clipboard) </li>
	<li><code>PDPermReqOprAccessible</code> </li>
	<li><code>PDPermReqOprSelect</code> (Selection only) </li>
	<li><code>PDPermReqOprOpen</code> </li>
	<li><code>PDPermReqOprSecure</code> </li>
	<li><code>PDPermReqOprPrintHigh</code> </li>
	<li><code>PDPermReqOprPrintLow</code> </li>
	<li><code>PDPermReqOprFullSave</code> </li>
	<li><code>PDPermReqOprImport</code> (Non-PDF)</li> 
	<li><code>PDPermReqOprExport</code> (Non-PDF and text extraction API, Search & Catalog) </li>
	<li><code>PDPermReqOprAny</code> </li>
	</ul>
*/
enum {
	/** */
	PDPermReqObjDoc = 1,
	/** */
	PDPermReqObjPage,
	/** */
	PDPermReqObjLink,
	/** */
	PDPermReqObjBookmark,
	/** */
	PDPermReqObjThumbnail,
	/** */
	PDPermReqObjAnnot,
	/** Form fields other than signature form fields. */
	PDPermReqObjForm,
	/** Signature form fields. */
	PDPermReqObjSignature,

	/* Acrobat 7.0 additions */
	/** Named embedded files. */
	PDPermReqObjEF,

	/* Reader 10 additions */
	/** Annots : Few Annots will be available in Reader.Refer  
	PDPermGetReqObjFromAnnotType*/
	PDPermReqObjReaderAnnot,
	
	/** Used for checking cache size. */
	PDPermReqObjLast	
};
typedef ASUns32 PDPermReqObj;

/** An enumerated data type used to describe the target operation of a permissions request.
	@see PDCryptAuthorizeExProc
	@see PDCryptGetAuthDataExProc
	@see PDDocPermRequest
*/
enum {
	/** Check all operations. */
	PDPermReqOprAll = 1,	
	/** Generic. */
	PDPermReqOprCreate, 
	/** Delete. */
	PDPermReqOprDelete,
	/** Modify. */
	PDPermReqOprModify,
	/** Copy. */
	PDPermReqOprCopy,
	/** For accessibility use. */
	PDPermReqOprAccessible,	
	/** For document or page, selecting (not copying) text or graphics. */
	PDPermReqOprSelect, 
	/** For document open. */
	PDPermReqOprOpen, 
	/** For the document: changing security settings. */
	PDPermReqOprSecure, 
	/** For the document: regular printing. */
	PDPermReqOprPrintHigh, 
	/** For the document: low quality printing. */
	PDPermReqOprPrintLow, 
	/** Form fill-in, or sign the existing field. */
	PDPermReqOprFillIn,	
	/** Rotate. */
	PDPermReqOprRotate,
	/** Crop. */
	PDPermReqOprCrop,
	/** For summarizing notes. */
	PDPermReqOprSummarize,	
	/** Insert. */
	PDPermReqOprInsert,
	/** For the page. */
	PDPermReqOprReplace,	
	/** For the page. */
	PDPermReqOprReorder,	
	/** For the document. */
	PDPermReqOprFullSave,	
	/** For the notes and image. */
	PDPermReqOprImport, 
	/** For the notes. ExportPS should check print. */
	PDPermReqOprExport, 
	/** Used for checking to see if any operation is allowed. */
	PDPermReqOprAny,		
	/** Used for error checking. */
	PDPermReqOprUnknownOpr,	

	/* Acrobat 5.1 additions.  */
	/** Submit forms outside of the browser. */
	PDPermReqOprSubmitStandalone,	
	/** Allow the form to spawn template pages. */
	PDPermReqOprSpawnTemplate,		
	/** This should be always the last item  */

	/* Acrobat 7.0 additions */
	/** For annotations and forms: enabling online functionality. */
	PDPermReqOprOnline,
	/** For annotations: enabling a summary view of annotations in Adobe Reader. */
	PDPermReqOprSummaryView,
	/** For forms: enables form appearance to be rendered as a plain text barcode. */
	PDPermReqOprBarcodePlaintext,

	/* Acrobat 10.0 additions */
	/** For controlling the "save" UI from a UIPerms handler **/
	PDPermReqOprUIsave,
	/** For controlling the "print" UI from a UIPerms handler **/
	PDPermReqOprUIprint,
	/** For controlling the "email" UI from a UIPerms handler **/
	PDPermReqOprUIemail,

	PDPermReqOprLast	
};
typedef ASUns32 PDPermReqOpr;

/*------------------------------------------------------------------------
	PD Related Security and Encryption
------------------------------------------------------------------------*/

/* Flags used by PDDocGetNewSecurityInfo */

/** The document has a user password.
	@ingroup SecurityInfoFlags
*/
#define pdInfoHasUserPW		pdPermOpen

/** The document has an owner password.
	@ingroup SecurityInfoFlags
*/
#define pdInfoHasOwnerPW	pdPermSecure

/** The document can be printed.
	@ingroup SecurityInfoFlags
*/
#define pdInfoCanPrint		pdPermPrint

/** The document can be modified (for example, by adding notes, links, or bookmarks). 
	@see pdInfoCanEditNotes
	@ingroup SecurityInfoFlags
*/
#define pdInfoCanEdit		pdPermEdit

/** The document text and graphics can be copied to the clipboard. 
	@ingroup SecurityInfoFlags
*/
#define pdInfoCanCopy		pdPermCopy

/** The document's notes, but nothing else, can be modified.
	@see pdInfoCanEdit
	@ingroup SecurityInfoFlags
*/
#define pdInfoCanEditNotes	pdPermEditNotes

/* This is the standard data used to fill in the Encrypt dict.
 */

#define MAX_PWCHARS 255

typedef char StdPassword[MAX_PWCHARS+1];

/**
	A structure describing the data for the standard security 
	handler provided in the Acrobat viewer. 
*/
typedef struct _t_StdSecurityData {
	/** The size of this stucture. */
	ASSize_t	size;			
	/** <code>true</code> if the user password should be changed. */
	ASBool	newUserPW;		
	/** <code>true</code> if there is a user password. */
	ASBool	hasUserPW;		
	/** The user password. */
	StdPassword	userPW;	
	/** <code>true</code> if the owner password should be changed, <code>false</code> otherwise.*/
	ASBool	newOwnerPW;
	/** <code>true</code> if an owner password is provided, <code>false</code> otherwise. */
	ASBool	hasOwnerPW;
	/** The owner password. */
	StdPassword	ownerPW;
	/** The permissions to allow. */
	PDPerms	perms;			
	/** The encryption key length in bytes. New for Acrobat 5.0. */
	Int32	keyLength;		
	/** Indicates a /R value. New for Acrobat 6.0. */
	Int32	revision;
	/** A flag that indicates whether document metadata will be encrypted. */
	ASBool	encryptMetadata;
	/** New for Acrobat 7.0.
		<p>The method of encryption for filters to use. It is only valid if the version is 4 or greater.</p>
		<p>The valid values are:</p>

		<TABLE rules="all" cellspacing="1">
		<TR><TH>Value</TH><TH>Description</TH></TR>
		<TR><TD><code>CF_METHOD_RC4_V2</code></TD><TD>Use the RC4 algorithm for encryption.</TD></TR>
		<TR><TD><code>CF_METHOD_AES_V1</code></TD><TD>Use the AES algorithm for encryption with a zero initialization vector.</TD></TR>
		<TR><TD><code>CF_METHOD_AES_V2</code></TD><TD>Use the AES algorithm for encryption with a 16 byte random initialization vector.</TD></TR>
		<TR><TD><code>CF_METHOD_AES_V3</code></TD><TD>Use the AES algorithm for encryption with a 4 byte random initialization vector.</TD></TR>
		</TABLE>
     */
	Int32	encryptMethod;
	/** A flag to indicate that only attachments are encrypted. <code>encryptMetadata</code> and <code>encryptAttachmentsOnly</code> cannot both be <code>true</code>. */
	ASBool	encryptAttachmentsOnly;
	/** Indicates a /V value. New for Acrobat 9.0. */
	Int32	version;
} StdSecurityDataRec, *StdSecurityData;

/* These values mirror values in CosCrypt.h, and must be kept in Sync - MTK (2 April 2008) */
#define STDSEC_METHOD_RC4_V2	2	/* CF_METHOD_RC4_V2 */
#define STDSEC_METHOD_AES_V1	5	/* CF_METHOD_AES_V1 (AES128) */
#define STDSEC_METHOD_AES_V2	6	/* CF_METHOD_AES_V2 (AES128) */
/** New encryption method for Acrobat 9.0 */
#define STDSEC_METHOD_AES_V3	7	/* CF_METHOD_AES_V3 (AES256) */

#define STDSEC_KEYLENGTH_RC4_V1		5	/* 40 bits */
#define STDSEC_KEYLENGTH_RC4_V2		16	/* 128 bits */
#define STDSEC_KEYLENGTH_AES128		16	/* 128 bits for AESV1 and AESV2 */
/** New encryption method for Acrobat 9.0 */
#define STDSEC_KEYLENGTH_AES256		32	/* 256 bits for AESV3 */

/* Version flags */
#define STDSEC_CryptVersionV1	1
#define STDSEC_CryptVersionV2	2
#define STDSEC_CryptVersionV3	3
/* Crypt filter support is in V4 */
#define	STDSEC_CryptVersionV4	4
/** New encryption method for Acrobat 9.0 */
#define STDSEC_CryptVersionV5	5

/* Revision flags */
#define STDSEC_CryptRevision1	1
#define STDSEC_CryptRevision2	2
#define STDSEC_CryptRevision3	3
#define STDSEC_CryptRevision4	4
/* New Password and encryption in Acrobat 9.0
 *   256-bit AES encryption -V5
 *   Unicode passwords -R5
 * Password may be 127 UTF-8 bytes, no key mixing with Object ID
 * This means that R5 and later algorithms must use an initialization
 * vector and/or salt to mask identical strings.
 */
#define STDSEC_CryptRevision5	5
/* Acrobat 10 - password checking slow down */
#define STDSEC_CryptRevision6	6


/* used for GetInfoText PDCrypt callback */
typedef enum {
	kGCHTTipText = 1,
	kGCHTMiniText,
	kGCHTLargeText
} GCHTextType;

/**
	A callback for PDCryptHandler. It is called by PDDocAuthorize() when 
	a user tries to set security for an encrypted document and 
	by a PDAuthProc() when a user tries to open a file. 

	<p>It must decide, based on the contents of the authorization 
	data structure, whether the user is permitted to open the 
	file, and what permissions the user has for this file. The 
	authorization data structure is available for making this 
	decision. Alternate implementations may not require authorization 
	data and may, for example, make authorization decisions 
	based on data contained in the security data structure (use 
	PDCryptNewSecurityDataProc()). </p>

	<p>This callback must not obtain the authorization data (for 
	example, by displaying a user interface into which a user 
	can type a password). Obtaining authorization data is handled 
	by the security handler's PDCryptGetAuthDataProc(), which 
	must be called before this callback. Instead, PDCryptAuthorizeProc() 
	must work with whatever authorization data is passed to 
	it. </p>

	<p>It is legitimate for this callback to be called with <code>NULL</code> 
	authorization data; the Acrobat viewer's built-in <code>authProc</code> 
	does this in order to support authorization methods that 
	do not require authorization data. </p>

	<p>When this callback is invoked to determine whether a user 
	is permitted to open a file, <code>permWanted</code> is set to <code>pdPermOpen</code>. 
	In this case, the file's contents are not yet decrypted 
	(since this callback is being asked to permit decryption), 
	and some calls must be avoided. For example, a call that 
	causes a page to be parsed results in an error, since the 
	encrypted contents are parsed. In general, it is safe to 
	obtain information about the presence or absence of things, 
	or the number of things, and to examine any part of a document 
	at the Cos level. </p>
	@param pdDoc The document for which authorized permissions 
	are being requested. 
	@param authData Authorization data. Its format is security 
	handler-specific; each handler can select its own authorization 
	data format. 
	@param permWanted The permissions being requested. It is 
	either <code>pdPermOpen</code> (if the file is being opened) or <code>pdPermSecure</code> 
	(if a request is being made to change the document's security 
	settings).
	@return The permissions granted based on <code>authData</code>. For opening, 
	the permissions returned usually should be <code>pdPermOpen</code> and 
	some or all of <code>pdPermPrint</code>, <code>pdPermEdit</code>, and <code>pdPermCopy</code>. 
	For setting security, permissions returned should be <code>pdPermAll</code>. 
	However, if authorization fails, <code>0</code> should be returned. 
	@see PDCryptAuthorizeExProc 
	@see PDCryptAuthorizeFilterAccess 
	@see PDDocAuthorize 
	@see PDDocOpen 
	@see PDDocOpenEx
*/
typedef ACCBPROTO1 PDPerms (ACCBPROTO2 *PDCryptAuthorizeProc)(PDDoc pdDoc,
	void *authData, PDPerms permWanted);

/**
	(Optional) A callback for PDCryptHandler. It creates a new empty 
	authorization data structure. This structure is subsequently 
	filled by PDCryptGetAuthDataProc(), then passed to PDCryptAuthorizeProc() 
	and eventually to ASfree(). 

	<p>This callback is not called by the Acrobat viewer, but a 
	security handler may use it if it wishes. The Acrobat viewer's 
	standard security handler does not use this method. </p>
	@param pdDoc The document for which a new authorization 
	data structure is created.
	@return The newly created authorization data structure. 
	@see PDCryptFreeAuthDataProc 
	@see PDCryptGetAuthDataProc 
*/
typedef ACCBPROTO1 void	* (ACCBPROTO2 *PDCryptNewAuthDataProc)(PDDoc pdDoc);

/**
	A callback for PDCryptHandler. This callback is called from 
	a PDAuthProc when a file is opened after PDCryptNewSecurityDataProc() 
	is called. 

	<p>The callback must determine the user's authorization properties 
	for the document by obtaining authorization data, such as 
	a user interface log in or password entry. It populates 
	an authorization data structure with this data. </p>

	<p>This callback may call the security handler's PDCryptNewAuthDataProc() 
	to allocate the authorization data structure. The use of an 
	authorization data structure is optional (an implementation 
	may wish to contain authorization data within the security 
	data structure). The authorization data structure is subsequently 
	used by the security handler's PDCryptAuthorizeProc() to determine 
	whether the user is authorized to open the file. </p>

	<p>A security handler can specify the standard password dialog box 
	by using AVCryptGetPassword(). In this case, <code>authData</code> 
	is a <code>char*</code>. </p>
	@param pdDoc The document to open. 
	@param permWanted Either <code>pdPermOpen</code> or <code>pdPermSecure</code>. Since 
	this value is also passed to PDCryptAuthorizeProc(), it may 
	not be necessary for this callback to use <code>permWanted</code>. 
	@param authDataP A pointer to the authorization data structure. 
	Set it to <code>NULL</code> if it is not used.
	@return <code>true</code> unless the operation should be cancelled (for example, 
	if the user cancels a dialog), <code>false</code> otherwise. 
	@see PDCryptGetAuthDataExProc 
	@see PDCryptNewAuthDataProc 
	@see PDDocOpen 
	@see PDDocOpen 
	@see PDDocOpenEx 
*/
typedef ACCBPROTO1 ASBool (ACCBPROTO2 *PDCryptGetAuthDataProc)(PDDoc pdDoc,
	PDPerms permWanted, void **authDataP);

/**
	(Optional) A callback for PDCryptHandler. It creates and populates 
	a new structure that contains whatever security-related 
	information the security handler requires (for example, 
	permissions, whether the file has owner and/or user passwords, 
	owner and/or user passwords, or other data used internally 
	by the security handler). If <code>encryptDict</code> is not <code>NULL</code>, the 
	structure should be populated based on the <code>encryptDict</code> parameter's contents. 
	This method is intended only to initialize the security 
	data structure. 

	<p>This callback is called under two circumstances:</p> 
	<ul>
	<li>When a document is opened, it is called with encryptDict 
	set to the document's Encryption dictionary. The handler 
	should then populate the new security data structure with 
	data that is obtained from the Encryption dictionary. </li>
	<li>When the user chooses a new encryption method, it is called 
	without an encryptDict. The handler should return a security 
	data structure with default values.</li>
	</ul>
	
	<p>If a security handler does not have this callback, the document's 
	<code>newSecurityData</code> field is set to <code>NULL</code>. </p>

	<p>If a file is to be saved, then PDCryptUpdateSecurityDataProc() 
	is subsequently called to allow user interface modification 
	of the contents. </p>

	<p>Security data is freed using PDCryptFreeSecurityDataProc(). 
	If PDCryptFreeSecurityDataProc() is not defined, ASfree() is 
	used. </p>
	@param pdDoc The document for which a new security data 
	structure is created. 
	@param encryptDict If <code>encryptDict</code> is a dictionary, this 
	callback must initialize the security data so that it corresponds 
	to the dictionary. Otherwise, it must set up default values 
	in the security data structure. 
	@return The newly created security data structure. 
	@see PDCryptFreeSecurityDataProc 
*/
typedef ACCBPROTO1 void	* (ACCBPROTO2 *PDCryptNewSecurityDataProc)(PDDoc pdDoc,
	CosObj encryptDict);

/**
	(Optional) A callback for PDCryptHandler. It validates the security
	data structure, which specifies the user's permissions.
	This callback may modify the security data structure (for
	example, because the user is not authorized to change the
	security as they requested). A client may have called PDDocNewSecurityData()
	to obtain a new security data structure, then modified it,
	and then called PDDocSetNewSecurityData() to change the document
	security. This callback should be called before actually
	setting the document's security data.

	<p>This callback is not called automatically by the Acrobat
	viewer. It must be called, if desired, by the security handler's
	PDCryptUpdateSecurityDataProc().</p>
	@param pdDoc IN/OUT The document whose security data is validated.

	@param secData IN/OUT (May be modified by the callback) The document's
	security data.
	@see PDCryptUpdateSecurityDataProc
	@see PDDocNewSecurityData
	@see PDDocSetNewSecurityData
*/
typedef ACCBPROTO1 void (ACCBPROTO2 *PDCryptValidateSecurityDataProc)(PDDoc pdDoc,
	void *secData);

/**
	A callback for PDCryptHandler. It updates the security data structure 
	that was created by PDCryptNewSecurityDataProc(). This structure 
	can be obtained by calling PDDocGetNewSecurityData(). The 
	security data structure of the previously saved file can 
	be obtained with a call to PDDocGetSecurityData(). 

	<p>The security data structure should be updated to reflect 
	the encryption parameters that will be used when saving 
	the file (this information is usually obtained via dialogs). 
	The encryption parameters are transferred to the Encrypt 
	dictionary by a subsequent callback to PDCryptFillEncryptDictProc(). </p>
	
	<p>The security data should be allocated by ASmalloc() or a related 
	function. Security data is freed using PDCryptFreeSecurityDataProc(). 
	If PDCryptFreeSecurityDataProc() is not defined, ASfree() is 
	used.</p> 

	<p>The callback can also update the security handler itself. 
	For example, the standard encryption handler switches to 
	no encryption if no passwords or permissions are set in 
	the security dialog box. Return ASAtomNull in <code>cryptHandler</code> 
	if no encryption is used in the saved file. </p>
	@param pdDoc The document whose security data is updated. 
	
	@param cryptHandler The current security handler for <code>pdDoc</code>. 
	It can be modified to change the security handler. Encryption 
	is turned off if ASAtomNull is set. 
	@param secDataP (Required) A security data structure. Its 
	content and organization is up to the security handler.
	@return <code>true</code> unless the operation should be cancelled (for example, 
	the user clicked on the Cancel button). 
	@see PDCryptValidateSecurityDataProc 
	@see PDDocGetSecurityData 
*/
typedef ACCBPROTO1 ASBool (ACCBPROTO2 *PDCryptUpdateSecurityDataProc)(PDDoc pdDoc,
	ASAtom *cryptHandler, void **secDataP);

/**
	A callback for PDCryptHandler. It sets up the key to be passed
	to initialize the RC4 cipher for encryption and decryption
	of a PDF file. It is called when an encrypted document is
	opened or saved.
	@param pdDoc IN/OUT The document for which the key is set.
	@param cryptData IN/OUT (Filled by the callback) The key. <code>cryptData</code>
	must be allocated by ASmalloc() because the Acrobat viewer
	will free it using ASfree().
	@param cryptDataLen IN/OUT (Filled by the callback) The number
	of bytes in cryptData. It cannot be greater than <code>5</code> bytes.
	@see PDCryptNewAuthDataProc
*/
typedef ACCBPROTO1 void (ACCBPROTO2 *PDCryptNewCryptDataProc)(PDDoc pdDoc,
	char **cryptData, ASInt32 *cryptDataLen);

/**
	A callback for PDCryptHandler. It is called when an encrypted document
	is saved. It fills the document's Encryption dictionary with
	whatever information the security handler wants to store
	in the document.

	<p>Normally this callback is called after PDCryptUpdateSecurityDataProc().
	The security data structure can be obtained with a call
	to PDDocGetNewSecurityData(), and <code>encryptDict</code> is filled
	based on this data.</p>

	<p>The sequencing of events that the viewer performs during
	creation of the encryptDict is as follows:</p>
	<ul>
	<li>The viewer creates the <code>encryptDict</code>.</li>
	<li>The viewer adds the Filter attribute to the dictionary.</li>
	<li>The viewer calls this PDCryptFillEncryptDictProc() to allow the security
	handler to add its own attributes to the dictionary.</li>
	<li>The viewer calls the PDCryptNewCryptDataExProc() (the PDCryptNewCryptDataProc()
	if unsuccessful) to get the algorithm version, key, and
	key length.</li>
	<li>The viewer checks if the V attribute has been added to the dictionary
	and, if not, it sets V to the algorithm version.</li>
	<li>The viewer sets the Length attribute if V is <code>2</code> or greater.</li>
	<li>The viewer adds the encryptDict to the document.</li>
	</ul>
	
	@param pdDoc IN/OUT The document to save.
	@param encryptDict IN/OUT A dictionary Cos object to fill with
	whatever information the security handler wants to store
	in the PDF file. Unlike all other strings and streams, direct
	object elements of the <code>encryptDict</code> are not encrypted automatically.
	If you want them encrypted, you must encrypt them before
	inserting them into the dictionary.
	@see PDDocSave
*/
typedef ACCBPROTO1 void	(ACCBPROTO2 *PDCryptFillEncryptDictProc)(PDDoc pdDoc,
	CosObj encryptDict);

/**
	(Optional) A callback for PDCryptHandler. It is called by PDDocGetNewSecurityInfo(). 
	It extracts the security information from the security data 
	structure, and returns the security information. 

	<p>This function is also used after a Save As... to reset the 
	permissions according to the current document. </p>

	<p>A default set of permissions is used if this callback is 
	absent: </p>

	<p><code>pdInfoCanPrint|pdInfoCanEdit|pdInfoCanCopy|pdInfoCanEditNotes </code></p>
	
	<p>See PDPerms. </p>
	@param pdDoc The document whose security information is obtained. 
	
	@param secInfo (Filled by the callback) The document's 
	security information. The value must be an OR of the Security Info 
	Flags. All unused bits in must be set to <code>1</code>.
	@see PDDocGetNewSecurityInfo 
*/
typedef ACCBPROTO1 void (ACCBPROTO2 *PDCryptGetSecurityInfoProc)(PDDoc pdDoc,
	ASUns32 *secInfo);

/**
	(Optional) A callback for PDCryptHandler. It is used to free security
	data acquired via PDCryptNewSecurityDataProc(). If this callback
	is omitted, the viewer defaults to freeing the data using
	ASfree().
	@param pdDoc IN/OUT The document whose security data is freed.

	@param secData IN/OUT (Filled by the callback) A pointer to the
	document's security data.
	@see PDCryptNewSecurityDataProc
	@see PDDocGetNewSecurityInfo
*/
typedef ACCBPROTO1 void (ACCBPROTO2 *PDCryptFreeSecurityDataProc)(PDDoc pdDoc,
	void *secData);

/**
	(Optional) A callback for PDCryptHandler. It is used to free authorization
	data acquired via PDCryptNewAuthDataProc(). If this callback
	is omitted, the viewer defaults to freeing the data using
	ASfree().
	@param pdDoc IN/OUT The document whose authorization data is freed.

	@param authData IN/OUT (Filled by the callback) A pointer to the
	document's authorization data.
	@see PDCryptNewAuthDataProc
	@see PDDocGetNewSecurityInfo
*/
typedef ACCBPROTO1 void (ACCBPROTO2 *PDCryptFreeAuthDataProc)(PDDoc pdDoc, void *authData);

/**
	(Optional) A callback for PDCryptHandler. It is used to free authorization
	data acquired via PDCryptNewCryptDataProc(). If this callback
	is omitted, the viewer defaults to freeing the data using
	ASfree().
	@param pdDoc IN/OUT The document whose encryption/decryption
	data is freed.
	@param cryptData IN/OUT (Filled by the callback) A pointer to the
	document's encryption/decryption data.
	@see PDCryptNewCryptDataProc
	@see PDDocGetNewSecurityInfo
*/
typedef ACCBPROTO1 void (ACCBPROTO2 *PDCryptFreeCryptDataProc)(PDDoc pdDoc, char *cryptData);

/**
	A callback for PDCryptHandler. It sets up the key to be passed
	to initialize the RC4 cipher for encryption and decryption
	of a PDF file. It is called when an encrypted document is
	opened or saved.

	<p>The key is truncated when the length is greater than the
	viewer currently supports. Data is freed by PDCryptFreeCryptDataProc()
	if provided. Otherwise, ASfree() is used.</p>
	@param pdDoc IN/OUT The document for which the key is set.
	@param cryptData IN/OUT (Filled by the callback) The key. <code>cryptData</code>
	must be allocated by ASmalloc() because the Acrobat viewer
	will free it using ASfree().
	@param cryptDataLen IN/OUT (Filled by the callback) The number
	of bytes in <code>cryptData</code>. It cannot be greater than <code>5</code> bytes.
	@param cryptVersion IN/OUT The Cos crypt version, which is the version
	of the algorithm that is used to encrypt and decrypt document
	data. <code>cryptVersion</code> equal to <code>0</code> is treated as <code>cryptVersion</code>
	equal to <code>1</code> to maintain backward compatibility.
	@see PDCryptFreeCryptDataProc
*/
typedef ACCBPROTO1 void (ACCBPROTO2 *PDCryptNewCryptDataExProc)(PDDoc pdDoc,
	char **cryptData, ASInt32 *cryptDataLen, ASInt32 *cryptVersion);

/**
	Replaces PDCryptAuthorizeProc. PDPerms are now obsolete 
	because Acrobat 5.0 introduces new permission controls. 
	However, Acrobat still supports old security handlers. 

	<p>It is called whenever Acrobat needs to get authorization data 
	and/or check permissions for operations. </p>
	@param pdDoc The document for which the request is made. 
	
	@param reqObj The object type that is the focus of the request: 
	it is one of the PDPermReqObj values. 
	@param reqOpr The operation type that is the focus of the 
	request: it is one of the PDPermReqOpr values. 
	@param authData Authorization data. Its format is security 
	handler-specific.
	@return The status of the request. It is one of the PDPermReqStatus values. 
	
	@see PDCryptAuthorizeProc 
	@see PDDocPermRequest 
*/
typedef ACCBPROTO1 PDPermReqStatus (ACCBPROTO2 *PDCryptAuthorizeExProc)(PDDoc pdDoc, PDPermReqObj reqObj,
	PDPermReqOpr reqOpr, void *authData);

/**
	Replaces PDCryptGetAuthDataProc(). It is called whenever Acrobat 
	needs to get authorization data and/or check permissions 
	for operations. 

	<p>PDPerms are now obsolete because Acrobat 5.0 introduces 
	new permission controls. However, Acrobat still supports 
	old security handlers. </p>
	@param pdDoc The document for which the request is made. 
	
	@param reqObj The object type that is the focus of the request: 
	it is one of the PDPermReqObj values. 
	@param reqOpr The operation type that is the focus of the 
	request: it is one of the PDPermReqOpr values. 
	@param authDataP A pointer to an authorization data structure. 
	Its format is security handler-specific.
	@return <code>true</code> unless the operation should be cancelled (for example, 
	if the user cancels a dialog), <code>false</code> otherwise. 
	@see PDCryptGetAuthDataProc 
*/
typedef ACCBPROTO1 ASBool (ACCBPROTO2 *PDCryptGetAuthDataExProc)(PDDoc pdDoc, PDPermReqObj reqObj,
	PDPermReqOpr reqOpr, void **authDataP);

/**
	Called when the application needs to open a rolled back portion of the original document. 
	A <i>rolled back</i> document is the original portion of the document when it is digitally signed. This 
	functionality is used for document modification detection. 
	
	<p>A rolled back document still requires authorization data which should be identical to the original document's. 
	However, the <code>authDataP</code> structure is unique to each security handler; 
	therefore, it cannot be duplicated by the application.</p> 
	
	<p>This callback is intended for opening a rolled back document silently by asking the security handler to 
	provide authorization data for it. The security handler should be able to duplicate the security data associated 
	with the original document and supply for the rolled back document. The callee is expected to authorize  
	subsequent callbacks, including Crypt Filters.</p>

	<p>If this callback is not provided, the security handler is asked for authorization data via a normal call such as PDCryptGetAuthDataExProc().
	The side effect might include the security handler's prompting for a password for the rolled back document.</p>

	@param pdDoc The document for which the request is made (a rolled back document).
	@param encryptDict An encryption dictionary for <code>pdDoc</code>. It is here just in case the security handler might need it.
	@param alreadyOpenedDoc The original PDDoc from which the rolled back document is extracted. 
	It is provided so that the security handler	can get security data out for the rolled back document.
	@param openedEncryptDict The encryption dictionary of the original PDDoc.
	@param authDataP Used to return security data for the rolled back document.
	@return <code>true</code> unless the security handler cannot provide authorization data for the requested document. 
*/
typedef ACCBPROTO1 ASBool (ACCBPROTO2 *PDCryptNewSecurityDataFromOriginalDocProc)(PDDoc pdDoc, CosObj encryptDict, 
	PDDoc alreadyOpenedDoc, CosObj openedEncryptDict, void **authDataP);

/**
	Called when the security handler should bring up a document
	(security) information dialog box with the current settings. It also
	should return <code>true</code> when the user wants to change the settings.

	<p>If this callback is not supplied, the default information dialog
	is displayed with PDPerms bits information (an Acrobat 4.x-equivalent dialog).</p>
	@param pdDoc IN/OUT The document whose information is displayed.
	@param cryptHandler IN/OUT The registered name of the handler.

	@return <code>true</code> if the handler wishes a callback to change the settings,
	<code>false</code> otherwise.
*/
typedef ACCBPROTO1 ASBool (ACCBPROTO2 *PDCryptDisplaySecurityDataProc)(PDDoc pdDoc,	ASAtom cryptHandler);

/**
	(Optional) A callback for PDCryptHandler. It is used by the Acrobat
	WebBuy proprietary method of passing crypt data.
*/
typedef ACCBPROTO1 void * (ACCBPROTO2 *PDCryptReservedProc)(void);

/**
	(Optional) This call is used to provide PDCrypt handler interoperability.  
	When an encrypted document is being opened and the security handler 
	specified in the encryption dictionary is not present, this callback is 
	used to determine if one of the registered security handlers can be used 
	to open the document.

	@param pdDoc The document being opened.
	@param encryptDict The encryption dictionary.

	@return <code>true</code> if the handler supports the format of <code>encryptDict</code>.
*/
typedef ACCBPROTO1 ASBool (ACCBPROTO2 *PDCryptCanParseEncryptDictProc)(
	PDDoc pdDoc,
	CosObj encryptDict);

/**
	A callback for PDCryptHandler. This function should extract 
	and return information about the document permissions to 
	display for the user: whether the user can print, edit, copy 
	text and graphics, edit notes and do form fill in and signing. 
	
	<p>The permissions returned are logically AND-ed with the document 
	permissions returned by any other permissions handlers, and 
	displayed to the user. All crypt handlers should implement 
	this call so that consolidated permissions can be displayed. 
	To display your own crypt handler's permissions, implement 
	PDCryptDisplaySecurityDataProc(). </p>

	<p>If this callback is absent, Acrobat assumes that all the 
	operations on the document are allowed. </p>
	@param pdDoc The document whose permissions are obtained. 
	
	@param perms (Filled by the callback) An array of the 
	document's permissions. For each combination of PDPermReqObj 
	and PDPermReqOpr, the value is <code>true</code> if the operation is 
	allowed for the object, <code>false</code> if it is not. 
	@param version (Filled by the callback) A pointer to the 
	version number of PDPermReqObj and PDPermReqOpr with which 
	this crypt handler is compatible (specified by the constant 
	PDPermReqVersion).
	@see PDDocPermRequest 
*/
typedef ACCBPROTO1 void (ACCBPROTO2 *PDCryptGetDocPermsProc) (
    PDDoc pdDoc, ASBool perms[PDPermReqObjLast][PDPermReqOprLast], ASInt16 *version );

/**
	(Optional) A callback for PDCryptHandler.
	It determines whether a document's metadata will be encrypted.  If this call is not 
	implemented, the metadata is always encrypted.  Note that documents with plain text 
	metadata can be opened only by Acrobat versions 6.0 and later.

	@param pdDoc IN The document being encrypted.

	@return <code>true</code> if document metadata should be encrypted.
*/

typedef ACCBPROTO1 ASBool (ACCBPROTO2 *PDCryptEncryptDocMetadata)(PDDoc pdDoc);

/**
	(Optional) A callback for PDCryptHandler.
	It provides information for display about document security settings.

	@param pdDoc IN The document.
	@param textType IN The text that should be used:


	<TABLE rules="all" cellspacing="1">
	<TR><TH>Value</TH><TH>Description</TH></TR>
	<TR><TD><code>kGCHTTipText</code></TD><TD>Text used for a security tool tip. It should be short. Its default is <code>"This document is secured."</code></TD></TR>
	<TR><TD><code>kGCHTMiniText</code></TD><TD>Text used for a security hover. It should be medium length. Its default is <code>"This document has been encrypted and may use..."</code></TD></TR>
	<TR><TD><code>kGCHTLargeText</code></TD><TD>Text use for Toast on Windows. It can have a longer length. There is no default.</TD></TR>
	</TABLE>

	@return The ASText value owned by the caller. Use a default value of <code>NULL</code>.
*/

typedef ACCBPROTO1 ASText (ACCBPROTO2 *PDCryptGetInfoTextProc)(PDDoc pdDoc, GCHTextType textType); 

/**
	(Optional) Used by Acrobat for Automated Permission Testing.
*/
typedef ACCBPROTO1 void * (ACCBPROTO2 *PDCryptReservedProc2)(PDDoc pdDoc, ASCab settings);

/**
	A callback for PDCryptBatchHandler. This callback puts up
	a dialog box that allows a user to enter data that will be used
	to batch secure a series of files. The data is stored in
	an ASCab which is part of a batch sequence file. The actual
	security data, including password(s), should be stored
	as a pointer in the ASCab so that password information is
	not serialized to disk. Pointers are not serialized from
	ASCab objects, but ASText objects, ASInt32 objects, and ASBool objects are serialized.

	@param settings IN/OUT An object of type ASCab.
	@return <code>true</code> indicates success.
*/

typedef ACCBPROTO1 ASBool (ACCBPROTO2 *PDCryptBatchShowDialogProc)(ASCab settings);

/**
	A callback for PDCryptBatchHandler. The developer should provide
	information about the current batch settings for the security
	handler. Batch settings are provided as a read-only ASCab
	that is passed to the function. A writable ASCab is also
	provided, which should be used to store parameter information
	about the security handler. The description information
	should be stored starting in the <code>paramDesc</code> ASCab using ASText
	objects starting with key <code>" 1"</code>. 
	@example <code>key=" 1", value="Title: API Reference"</code> (ASText object)
	@param settings IN/OUT Batch settings.
	@param paramDesc IN/OUT Description information.
*/
typedef ACCBPROTO1 void (ACCBPROTO2 *PDCryptBatchParamDescProc)(const ASCab settings, ASCab paramDesc);

/**
	A callback for PDCryptBatchHandler. It is different from the regular
	PDCryptHandler <code>NewAuthData</code> function. It creates and returns a
	<code>void*</code> which is the authorization data for the batch security
	handler. This data should be used to batch both open files
	and secure files. Therefore, make sure to provide
	password information for both the <code>pdPermOpen</code> and <code>pdPermSecure</code>
	cases. This data will be passed to the PDCryptBatchAuthorizeProc()
	callback and eventually to PDCryptBatchFreeAuthDataProc()
	to free the data. This authorization data is collected before
	any files are opened in the batch sequence. It is permitted
	to display a user interface at this point since the batch operation
	has not started yet. The data applies to all files, and
	therefore could represent one or more passwords which can
	be enumerated in the <code>BatchAuthorize</code> function which receives
	the batch authorization data.
*/
typedef ACCBPROTO1 void	* (ACCBPROTO2 *PDCryptBatchNewAuthDataProc)(void);

/**
	A callback for PDCryptBatchHandler. It is called when a PDF file is
	opened. It is first called with <code>NULL</code> <code>authData</code> for the case without
	a user password. It is called again with authorization data provided
	for the security handler that matches the one used in the
	PDF file.

	@note This function is called a batch operation and therefore
	should not display any user interface. During a batch operation,
	a file will first be opened with the <code>pdPermSecure</code> bit set.
	It will then be opened with the <code>pdPermOpen</code> bit set.
	@param pdDoc IN/OUT The document being opened.
	@param reqObj IN/OUT The object type that is the focus of the request:
	it is one of the PDPermReqObj values.
	@param reqOpr IN/OUT The operation type that is the focus of the request:
	it is one of the PDPermReqOpr values.
	@param authData IN/OUT Authorization data. Its format is security
	handler-specific.
	@return PDPermReqStatus
*/
typedef ACCBPROTO1 PDPermReqStatus (ACCBPROTO2 *PDCryptBatchAuthorizeProc)(PDDoc pdDoc,
	PDPermReqObj reqObj, PDPermReqOpr reqOpr, void *authData);

/**
	A callback for PDCryptBatchHandler. If provided, it must be used
	to free <code>authData</code> acquired via BatchGetAuthData() or BatchNewAuthData().
	If no BatchFreeAuthData() function is provided, a default
	one will be used which calls ASfree() on the <code>authData</code> if it
	is non-<code>NULL</code>.
	@param authData IN/OUT Authorization data. Its format is security
	handler-specific.
*/
typedef ACCBPROTO1 void (ACCBPROTO2 *PDCryptBatchFreeAuthDataProc)(void *authData);

/**
	A callback for PDCryptBatchHandler. This function is called
	at the beginning of a batch sequence before any files have
	been opened. This allows a security handler to be called
	back with the ASCab of settings that were filled out by
	the BatchShowDialog() function, or by an ASCab that was read
	in from disk. Pointers of security information are not serialized
	to disk, and therefore a security information structure
	may need to be regenerated based on other security information
	in the ASCab. It is permitted for the BatchPreSequence() callback
	to put up a user interface asking the user for more information since
	the batch sequence has not started yet. If this function
	returns <code>false</code>, the viewer will assume that the command cannot
	be executed and will cancel the sequence.
	@param settings IN/OUT Batch settings.
	@return See above.
*/
typedef ACCBPROTO1 ASBool (ACCBPROTO2 *PDCryptBatchPreSequenceProc)(ASCab settings);

/**
	A callback for PDCryptBatchHandler. This function is called
	at the end of a batch sequence after all files have been
	processed. Any memory that was allocated in the BatchPreSequence()
	call should be cleaned up in this callback.
	@param settings IN/OUT Batch settings.
*/
typedef ACCBPROTO1 void (ACCBPROTO2 *PDCryptBatchPostSequenceProc)(ASCab settings);

/**
	A callback for PDCryptBatchHandler. This function should update 
	the crypt handler's security data without bringing up a 
	dialog. This data is provided by a PDCryptBatchShowDialogProc(). 
	The current security data can be obtained by calling PDDocGetNewSecurityData(). 

	@note This function is called a batch operation and therefore 
	should not display any user interface. 
	@param pdDoc The document whose data is updated. 
	@param settings An object of type ASCab. 
	@param cryptHandler An object of type ASAtom. 
	@param secDataP A pointer to the document's security data.
	@return This function should return <code>true</code> unless there is a problem 
	with the batch data for this security handler. 
 
*/
typedef ACCBPROTO1 ASBool (ACCBPROTO2 *PDCryptBatchUpdateSecurityDataProc)(
	PDDoc pdDoc, ASCab settings, ASAtom *cryptHandler, void **secDataP);

/* Call back routine prototypes for Crypt Filter support */

/**
	(Optional) A callback for PDCryptFilterHandler. Callbacks 
	that conform to this prototype are called to encrypt or 
	decrypt streams from a document. 

	<p>The first call to a procedure of this type must fill out 
	an ASCryptStmRec structure with pointers to callback routines 
	for various types of stream access; see ASCryptStmProcs(). </p>
	
	@param dP The document containing the stream. 
	@param filterName The name of the security filter in use. 
	
	@param stm The security stream structure containing the 
	stream itself, access information, and an ASCryptStmRec. 
	This callback function is called upon opening the stream; 
	the first call must initialize the stream. 
	@param handOff <code>true</code> if the method should close the base 
	stream when it closes the filter, <code>false</code> otherwise. 
	@param params A Cos object containing parameters for the 
	operation, as specified for the filter. 
	@param stmLength For a decryption operation, the requested 
	number of bytes; for an encryption operation, the number 
	of bytes in the stream contained in <code>stm</code>. 
	@see PDCryptFilterAuthorizeProc 
	@see PDCryptFilterGetDataProc 
	@see PDCryptFilterStringProc 
	@see PDDocSetNewCryptFilterData 
	@see PDDocSetNewCryptFilterMethod 
	@see PDDocSetNewDefaultFilters
*/
typedef ACCBPROTO1 void (ACCBPROTO2 *PDCryptFilterStreamProc)(
	CosDoc dP, ASAtom filterName, ASCryptStm stm, ASBool handOff, CosObj params, ASInt32 stmLength);

/**
	(Optional) A callback for PDCryptFilterHandler. Acrobat's 
	security mechanism calls this method to determine whether 
	the user should have access to this filter. 
	@param dP The document for which to perform authorization. 
	
	@param filterName The name of the security filter. 
	@param encryptDict The encryption dictionary to use. 
	@param bEncrypt <code>true</code> if authorization is for an encryption 
	operation, <code>false</code> if it is for a decryption operation. 
	@param bUIAllowed <code>true</code> if the caller can bring up the 
	security dialogs as needed, <code>false</code> otherwise.
	@return <code>true</code> if the user has access to this filter, <code>false</code> otherwise. 
	
	@see PDCryptFilterGetDataProc 
	@see PDCryptFilterStreamProc 
	@see PDCryptFilterStringProc 
	@see PDCryptAuthorizeFilterAccess 
	@see PDDocSetNewCryptFilterMethod 
	@see PDDocSetNewDefaultFilters 
*/
typedef ACCBPROTO1 ASBool (ACCBPROTO2 *PDCryptFilterAuthorizeProc)(
	CosDoc dP, ASAtom filterName, CosObj encryptDict, ASBool bEncrypt, ASBool bUIAllowed);

/**
	(Optional) A callback for PDCryptFilterHandler. Acrobat's 
	security mechanism calls this method to retrieve the encryption/decryption 
	key for this filter. It is called only when the filter's encryption 
	method is V2. 
	@param dP The document whose data is retrieved. 
	@param filterName The name of the security filter. 
	@param key (Filled by the method) A pointer to the encryption/decryption 
	key. 
	@param bNewKey <code>true</code> if you want to retrieve a new key 
	(changed since opening the file), <code>false</code> otherwise. 
	@param bUIAllowed <code>true</code> if the caller can bring up the 
	security dialogs as needed, <code>false</code> otherwise.
	@see PDCryptFilterAuthorizeProc 
	@see PDCryptFilterStreamProc 
	@see PDCryptFilterStringProc 
	@see PDDocSetNewCryptFilterData 
	@see PDDocSetNewCryptFilterMethod 
	@see PDDocSetNewDefaultFilters 
*/
typedef ACCBPROTO1 ASInt32 (ACCBPROTO2 *PDCryptFilterGetDataProc)(
	CosDoc dP, ASAtom filterName, char **key, ASBool bNewKey, ASBool bUIAllowed);

#define PDCryptFilterStringProc	CosCryptStringProc

/**   PDCrypt Filter support:

	<ul>
	<li>Encryption callback routines are optional. However, the security handler is responsible for not allowing the application to cause a Full Save.</li>
	<li>String encryption/decryption support is not required.</li>
	<li>The <code>DecryptStream</code> callback can be <code>NULL</code> if the crypt filter handler does not perform encryption/decryption on its own.  
	In such cases, <code>GetDataProc</code> must be supplied.</li>
	<li>Crypt Filter handlers are expected to return <code>0</code> length buffer (or EOF) when unauthorized access is made.</li>
	</ul>

	<p>The <code>CryptStringProcs</code> string callback is expected to return the desired buffer size (the required destination buffer length) 
	when <code>NULL</code> is passed as the destination buffer.  This mechanism is to allow a different buffer length upon 
	encryption/decryption.</p>

	<p>About the ASCryptStm in the callback: upon the first call (back) to Encrypt/Decrypt stream procs, the handler is expected to 
	fill out an ASCryptStmRec. ASCryptStm has pointers to callback routines for various stream access. Subsequent stream 
	access is made to these callback routines.  See ASExpT.h for the ASCryptStmRec definition.</p>
*/
typedef struct _t_PDCryptFilterHandler {
	/** Set this to <code>sizeof(PDCryptFilterHandlerRec)</code>. */
	ASSize_t size;
	/** */
	PDCryptFilterAuthorizeProc	Authorize;
	/** Called to get crypt data when the application's built-in method is used. */
	PDCryptFilterGetDataProc	GetData;	
	/** */
	PDCryptFilterStreamProc		DecryptStream;	
	/** */
	PDCryptFilterStreamProc		EncryptStream;
	/** */
	PDCryptFilterStringProc		DecryptString;
	/** */
	PDCryptFilterStringProc		EncryptString;
} PDCryptFilterHandlerRec;

typedef struct _t_PDCryptFilterHandler *PDCryptFilterHandler;

/**
	Callbacks used to open secured files and to modify security 
	settings while batch processing a list of files. These 
	callbacks are not called while opening files through the 
	user interface. In addition, the regular PDCryptHandler functions are 
	not called during batch operations. 
	@see PDRegisterCryptHandler 
	@see PDRegisterCryptHandlerEx 
*/
typedef struct _t_PDCryptBatchHandler {
	/** Set this to <code>sizeof(PDCryptBatchHandlerRec)</code>.*/
	ASSize_t size;

	/** This function should display a dialog box that allows a user to enter data
		that will be used to batch secure a series of files.  The data is stored in an
		ASCab which is part of a Batch sequence file.  The actual security data, including
		password(s), should be stored as a pointer in the ASCabinet so that password information
		is not serialized to disk.  Pointers are not serialized from ASCab objects, but ASText objects, ASInt32 objects,
		and ASBool objects are serialized.
	*/
	PDCryptBatchShowDialogProc BatchShowDialog;

	/** The developer should provide information about the current batch settings for the security handler.
		Batch settings are provided as a read-only ASCab which is passed in to the function.  A
		writeable ASCab is also provided, which should be used to store parameter information about
		the security handler. The description information should be stored starting in the
		<code>paramDesc</code> ASCab using ASText objects starting with key <code>"1"</code>.
		
		@example <code>key="1", value="Title: API Reference"</code> (ASText object)
	*/
	PDCryptBatchParamDescProc BatchParamDesc;

	/** This is different from the regular PDCryptHandler <code>NewAuthData</code> function.
		It creates and returns a <code>void*</code> which is the authorization data for the batch security handler.
		This data should be used to batch both open files and secure files.  Therefore,
		make sure to provide password information for both the <code>pdPermOpen</code> and <code>pdPermSecure</code> cases,
		and pass to <code>Authorize</code> and eventually to ASfree().
		
		<p>This authorization data is collected before any files are opened in the Batch sequence.
		It is permitted to display a user interface at this point since the Batch operation has not started yet.
		The data applies to all files, and therefore could represent one or more passwords which
		can be enumerated in the <code>BatchAuthorize</code> function which receives the Batch <code>authData</code>.</p>
	*/
	PDCryptBatchNewAuthDataProc BatchNewAuthData;

	/** Called when a PDF file is opened. It is first called with <code>NULL</code> <code>authData</code> for the case without a
		user password. It is called again with authorization data provided for the security handler
		that matches the one used in the PDF file.
		
		During a Batch operation, a file will first be opened with the <code>pdPermSecure</code> bit set.  It
		will then be opened with the <code>pdPermOpen</code> bit set.
		
		@note This function is called a Batch operation and therefore should not display any user interface.
	*/
	PDCryptBatchAuthorizeProc BatchAuthorize;

	/** If provided, this must be used to free the <code>authData</code> acquired via
		<code>BatchGetAuthData</code> or <code>BatchNewAuthData</code>.  If no <code>BatchFreeAuthData</code> function is provided,
		a default one will be used which calls ASfree() on the <code>authData</code> if it is non-<code>NULL</code>.
	*/
	PDCryptBatchFreeAuthDataProc BatchFreeAuthData;

	/** This function should update the crypt handler's security data without bringing
		up a dialog.  This data is provided by a PDCryptShowBatchDialogProc().
		The current security data can be obtained by calling PDDocGetNewSecurityData().
		This function should return <code>true</code> unless there is a problem with the batch data
		for this security handler.
		
		@note This function is called a Batch operation and therefore should not display any user interface.
	*/
	PDCryptBatchUpdateSecurityDataProc BatchUpdateSecurityData;

	/** This function is called at the beginning of a Batch sequence before any files have been
		opened. This allows a security handler to be called back with the ASCab of settings that
		were filled out by the BatchShowDialog() function, or by an ASCabinet that was read in
		from disk. Pointers of security information are not serialized to disk, and therefore
		a security information structure may need to be regenerated based on other security
		information in the ASCabinet.
		
		<p>It is permitted for the <code>BatchPreSequence</code> callback to display a user interface asking the user for
		more information since the Batch sequence has not started yet.</p>
		
		<p>If this function returns <code>false</code>, the viewer will assume that the command cannot be executed,
		and will cancel the sequence.</p>
	*/
	PDCryptBatchPreSequenceProc BatchPreSequence;

	/** This function is called at the end of a Batch sequence after all files have been
		processed.  Any memory that was allocated in the <code>BatchPreSequence</code> call should be
		cleaned up in this callback.
	*/
	PDCryptBatchPostSequenceProc BatchPostSequence;
} PDCryptBatchHandlerRec;

typedef struct _t_PDCryptBatchHandler *PDCryptBatchHandler;


/**
	A data structure containing callbacks that implement a security handler. The callbacks
	implement the security handler functions. For example, they get authorization data such as
	a password from the user, set permissions, read and write security-related data in a
	PDF file, and so on.
	@see PDRegisterCryptHandler
	@see PDRegisterCryptHandlerEx
*/
typedef struct _t_PDCryptHandler {
	/** Set this to <code>sizeof(PDCryptHandlerRec)</code>. PDRegisterCryptHandler
		uses this to determine if the caller was compiled with an old
		version of this structure declaration. PDRegisterCryptHandler()
		will raise <code>genErrBadParam</code> if the size does not correspond to
		a known size of this <code>struct</code>.
	*/
	ASSize_t size;
		
	/** This function will be called when a user tries to open or set
		security for an encrypted document. <code>PermsWanted</code> will be either
		<code>pdPermOpen</code> or <code>pdPermSecure</code>. This function should return the
		permissions granted based on the <code>authData</code>. For opening, the
		permissions returned usually should be <code>pdPermOpen</code> and some or
		all of <code>pdPermPrint</code>, <code>pdPermEdit</code>, and <code>pdPermCopy</code>. For setting
		security, permissions returned should be <code>pdPermAll</code>. However,
		if authorization fails, <code>0</code> should be returned. The function is
		first called with <code>authData</code> equal to <code>NULL</code>. If that fails, <code>GetAuthData</code>
		is called and the <code>authData</code> from it is passed to <code>Authorize</code>.
		
		<p>If this function is called to authorize Open, decryption will
		not yet have been installed.  So while any part of the document
		may be examined, some calls must be avoided.  For example, a
		call that causes a page to be parsed will probably result in an
		error since the encrypted contents will be parsed.  In general,
		it is safe to obtain information about the presence or absence
		of things, or the number of things, and to examine any part of
		a document at the Cos level.</p>
	*/
	PDCryptAuthorizeProc Authorize;

	/** Creates and returns a new <code>struct</code> that can be filled out
		and passed to <code>Authorize</code> and eventually to ASfree().
		This function is not called by the standard security mechanism
		but may be called by extensions that want to gain access
		to a protected document.  This function need not be
		implemented if clients can simply allocate data using
		ASmalloc(). In fact, the standard CryptHandler does not.
	*/
	PDCryptNewAuthDataProc NewAuthData;
		
	/** This function obtains authorization data from the user. As with
		<code>Authorize</code>, <code>permsWanted</code> will be either <code>pdPermOpen</code> or
		<code>pdPermSecure</code>. This function should allocate <code>authData</code> to be used
		by <code>Authorize</code>. The function
		should return <code>true</code> unless the operation should be cancelled
		(for example, if the user cancels a dialog).
		A crypt handler can specify the standard password dialog box by using
		AVCryptGetPassword(). In this case, the <code>authData</code> will be a <code>char *</code>
		that should be freed using ASfree() after <code>Authorize</code> is called.
	*/
	PDCryptGetAuthDataProc GetAuthData;

	/** Creates a new <code>struct</code> that contains information corresponding to information
		in the security dialog. If <code>encryptDict</code> is a dictionary, initialize
		the security data to correspond to the dictionary. Otherwise, set up
		defaults. This function will be called when opening a document
		with <code>encryptDict</code> set to the document's <code>encryptDict</code>. It will also
		be called when a user chooses new encryption.
	*/
	PDCryptNewSecurityDataProc NewSecurityData;

	/** Validates the security data, modifying it as necessary. A client
		may have called PDDocNewSecurityData() to obtain a new security
		data structure, then modified it, and then called
		PDDocSetNewSecurityData() to change the document security.
		This is called before actually setting the document's
		security data.
	*/
	PDCryptValidateSecurityDataProc ValidateSecurityData;		

	/** This function should update the crypt handler's security data,
		usually by displaying a dialog. The current security data
		can be obtained by calling PDDocGetNewSecurityData().
		Like <code>GetAuthData</code>, this function should return <code>true</code> unless cancelled.
		The security data should be created with ASmalloc() so that it can
		later be freed by ASfree().
		The function can also update the <code>cryptHandler</code> itself.
		For example, the built-in encryption
		switches to no encryption if no passwords or permissions are
		set in the security dialog.
	*/
	PDCryptUpdateSecurityDataProc UpdateSecurityData;

	/** Sets up the key to be passed to initialize the RC4 cipher for
		encryption and decryption. The length may not be greater than
		<code>5</code> to satisfy the current US export control regulations.
		Data should be allocated by ASmalloc() or a relative
		so that it may be freed by ASfree().
	*/
	PDCryptNewCryptDataProc NewCryptData;

	/** This function should fill the encryption dictionary with whatever
		information is to be stored in the document. Unlike all other
		strings and streams, direct object elements of the encryption dictionary
		are not encrypted automatically.  They must be encrypted before
		they are inserted into the dictionary.
	*/
	PDCryptFillEncryptDictProc FillEncryptDict;
	
	/** This function should return information about security for
		display to the user:  whether the document has owner and
		user passwords and whether the user password enables
		printing, editing, copying text and graphics, and editing notes.
		See PDexpt.h for possible permissions. All other bits in <code>secInfo</code>
		should be set to <code>1</code>.
		This function is also used after a SaveAs to reset the
		permissions according to the current document.
	*/
	PDCryptGetSecurityInfoProc GetSecurityInfo;
		
	/* New calls for Acrobat 3.0 are below. */

	/** If provided, this must be used to free <code>securityData</code> acquired via
		<code>NewSecurityData</code>.
	*/
	PDCryptFreeSecurityDataProc FreeSecurityData;
		
	/** If provided, must be used to free <code>authData</code> acquired via
		<code>GetAuthData</code> or <code>NewAuthData</code>.
	*/
	PDCryptFreeAuthDataProc FreeAuthData;
		
	/** If provided, this must be used to free <code>cryptData</code> acquired via
		<code>NewCryptData</code>.
	*/
	PDCryptFreeCryptDataProc FreeCryptData;
		
	/* New call for Acrobat 4.05 are below. */

	/** Sets up the key to be passed to initialize the RC4 cipher and
		the version of algorithm for encryption and decryption.  The
		key will be truncated when the length is greater than the viewer
		currently supports.
		Data will be freed by <code>FreeCryptData</code> if provided. Otherwise,
		ASfree() is used.
	*/
	PDCryptNewCryptDataExProc NewCryptDataEx;		

	/* New call for Acrobat 5.0 and later. */

	/** This was added to replace PDCryptGetAuthDataProc().  There are now new permission
		controls. PDPerms are obsolete. Yet, the viewer still supports the old security
		handler. It is called whenever the viewer needs to get authorization data and/or check
		permission for operation(s).
	*/
	PDCryptAuthorizeExProc AuthorizeEx;
		
	/** This was added to replace PDCryptGetAuthDataProc().  There are now new permission
		controls. PDPerms are obsolete. Yet, the viewer still supports the old security
		handler. It is called whenever the viewer needs to get authorization data and/or check
		permission for operation(s).
	*/
	PDCryptGetAuthDataExProc GetAuthDataEx;

	/** When the security handler is called, it should pop up a dialog box with current
		permission settings. This callback was added to provide the security handler with a
		way to display its custom permission settings. It should return <code>true</code> if it wants
		a callback to modify security settings. Otherwise, it should return <code>false</code>.
	*/
	PDCryptDisplaySecurityDataProc DisplaySecurityData;

	/**
		Used for the Acrobat Web Buy proprietary method of passing encrypted data.
	*/
	PDCryptReservedProc GetReservedData;

	/** A pointer to the PDCryptBatchHandler structure.  If this parameter is non-<code>NULL</code>, 
		the security handler will work in a Batch environment, either for decrypting
		PDF files or encrypting them on Save with a new security handler.
	*/
	PDCryptBatchHandler CryptBatchHandler;
		
	/* New call for Acrobat 6.0 and later */

	/** This call is used to provide PDCrypt handler interoperability.  When an encrypted 
		ducument is being opened and the the security handler specified in the encryption
		dictionary is not present, the viewer will call this function on existing security
		handlers to see if one of them can be used to open the document. 
	*/
	PDCryptCanParseEncryptDictProc CanParseEncryptDict;

	/** A pointer to the PDCryptFilterHandler structure.  If this parameter is non-<code>NULL</code>, 
		the security handler supports the Crypt Filter.
	*/
	PDCryptFilterHandler CryptFilterHandler;		

	/** This function should return information about the document permissions
		for display to the user: whether the user can print, edit, copy text and 
        graphics, edit notes and do form fill in and signing. See PDexpt.h for 
        possible permissions.
	*/
	PDCryptGetDocPermsProc GetDocPerms;

	/** It determines whether a document's metadata will be encrypted.  If this call is not 
	    implemented, the metadata is always encrypted.
	*/
	PDCryptEncryptDocMetadata EncryptDocMetadata;

	/** A new call added for Acrobat 7.0, used for opening a rolled back document (the original portion of a signed document). If 
		this call is not implemented, the security handler is asked for authorization. Since 
		authorization is already requested for the document, this would be the second authorization call
		for the rolled back part.
		<p>The callee is expected to authorize the opening of this document using an already opened document, 
		including the subsequent callback to authorize the Crypt Filter(s).</p>
	*/
	PDCryptNewSecurityDataFromOriginalDocProc	NewSecurityDataFromOrgDoc;

	/** New calls added for Acrobat 7.0.5 */
	
	/** Used by Acrobat to display user interface information about the document.
	*/

	PDCryptGetInfoTextProc GetInfoText;
	
	/* Used by Acrobat for automated permission testing.
	**/
	PDCryptReservedProc2  SetTestPerms;

} PDCryptHandlerRec;

typedef struct _t_PDCryptHandler *PDCryptHandler;

/*------------------------------------------------------------------------
	PDNameTree
------------------------------------------------------------------------*/

/**
	The dictionary used to store all of the Named Destinations in a PDF file. A name tree
	is used to map Cos strings to Cos objects just as a Cos dictionary is used to map Cos
	names to Cos objects. However, a name tree can have many more entries than a Cos
	dictionary can. You create a PDNameTree and locate it where you think is appropriate
	(perhaps under a page, but most often right under the catalog).
	Name trees use Cos-style strings (not <code>NULL</code>-terminated C strings), which may use
	Unicode encoding, and these may contain bytes with zeroes in them (high bytes of
	ASCII characters).
	@see PDDocCreateNameTree
	@see PDNameTreeNew
	@see PDNameTreeFromCosObj
	@see PDNameTreeEnum
*/
typedef OPAQUE_64_BITS PDNameTree;

/*------------------------------------------------------------------------
	PDNumTree
------------------------------------------------------------------------*/

/**
	An object that points to the root node of a number tree inside a PDF file. A number
	tree is used to map integers to arbitrary Cos objects just as a Cos dictionary is used
	to map Cos names to Cos objects. However, a number tree can have many more
	entries than a Cos dictionary can.
	@see PDNumTreeNew
	@see PDNumTreeFromCosObj
	@see PDNumTreeEnum
*/
typedef OPAQUE_64_BITS PDNumTree;

/*------------------------------------------------------------------------
    PDPageLabel
--------------------------------------------------------------------------*/

/**
A label used to describe a page. This is used to allow for non-sequential page
numbering or the addition of arbitrary labels for a page (such as the inclusion of
Roman numerals at the beginning of a book). A PDPageLabel specifies the
numbering style to use (for example, upper-case or lower-case Roman, decimal, and so
on), the starting number for the first page, and an arbitrary prefix to be preappended
to each number (for example, <code>"A-"</code> is used to generate <code>"A-1"</code>, <code>"A-2"</code>, <code>"A-3"</code>, and so
on).
@see PDDocGetPageLabel
@see PDDocGetLabelForPageNum
@see PDPageLabelFromCosObj
@see PDPageLabelNew
@see PDDocRemovePageLabel
*/
typedef OPAQUE_64_BITS PDPageLabel;

/*------------------------------------------------------------------------
    PDPageRange
	Used to specify a range of pages when printing
--------------------------------------------------------------------------*/

/**
	Specifies a range of pages in a document. Page numbers begin 
	with <code>0</code>. 
*/
typedef struct _t_PDPageRange {
	/** The starting page number.*/
	ASInt32				startPage;
	/** The ending page number.*/
	ASInt32				endPage;
	/** The pages in the range to print. It must be one of: PDAllPages, PDEvenPagesOnly, or PDOddPagesOnly.*/
	ASInt32				pageSpec;	
} PDPageRange;

/**
	Passed to the PDDocWillPrintDocInMode() notification
	to specify the type of print operation being performed.
	
	@see PDDocWillPrintDocInMode
*/
enum {

	/** Print only the document. */
	PDPrintWhat_DOCUMENT, 

	/** Print the document and associated annotations (WYSIWYG -- the default for Acrobat 8.0 and later). */
	PDPrintWhat_DOCUMENT_AND_COMMENTS, 

	/** Print the document and stamp annotations. */
	PDPrintWhat_DOCUMENT_AND_STAMPS, 

	/** Print only the data within form fields. */
	PDPrintWhat_FORM_FIELDS_ONLY, 

	/** */
	PDPrintWhat_COUNT,

	/** */
	PDPrintWhat_MIN=PDPrintWhat_DOCUMENT
};
typedef ASEnum8 PDPrintWhat;

/*------------------------------------------------------------------------

	Optional Content API typedefs, defines, etc. PDF 1.5, Acrobat 6.0

--------------------------------------------------------------------------*/
/** A PDOCG represents a named object whose state can be toggled in a user interface to affect
	changes in visibility of content.
 */
typedef struct _t_PDOCG *PDOCG;

/** A PDOCMD is an object that is attached to content to indicate membership in
	an OCG or group of OCGs.
 */
typedef struct _t_PDOCMD *PDOCMD;

/** A PDOCContext is an object that keeps track the on/off states of all of the OCGs
	in a document. There can be more than one PDOCContext object, representing
	different combinations of OCG states. The PDDoc contains an internal PDOCContext
	that is used for on-screen drawing and as the default state used for any other
	drawing or content enumeration. Clients can change the states of OCGs within
	any PDOCContext. Clients can build (and save in the PDF file) PDOCContext objects with their
	own combination of OCG states, and issue drawing or enumeration commands using
	their own PDOCContext instead of the document's internal PDOCContext.
	All discussion of <i>visibility</i> of content is therefore meant to be with respect
	to the OCG states stored in a specific PDOCContext.
 */
typedef struct _t_PDOCContext *PDOCContext;

/** A PDOCConfig represents a set of states and other information that is saved in
	a PDF file for future use. There is a document default configuration, saved
	in the /D entry in the OCProperties dictionary, and a list of other client configurations,
	saved as an array of configurations in the /Configs entry in the OCProperties dictionary.
	PDOCConfig objects are typically used to initialize the OCG states for a client's
	PDOCContext.
 */
typedef struct _t_PDOCConfig *PDOCConfig;

/** PDOCMDVisPolicy represents the four legal values for the /P key in an Optional Content Membership Dictionary (OCMD) dictionary.
	They specify the visibility of content with respect to the on/off state of the Optional Content Groups (OCGs)
	listed in the OCMD.
 */
enum {
	/** */
	kOCMDVisibility_AllOn,
	/** */
	kOCMDVisibility_AnyOn,
	/** */
	kOCMDVisibility_AnyOff,
	/** */
	kOCMDVisibility_AllOff
};
typedef ASUns8 PDOCMDVisPolicy;

/** PDOCContextInitPolicy is used to specify how to initialize the states of Optional Content Groups (OCGs)
	when calling PDOCContextNew() or PDOCContextInit().
 */
enum {
	/** */
	kOCCInit_OFF,
	/** */
	kOCCInit_ON,
	/** */
	kOCCInit_FromOtherContext,
	/** */
	kOCCInit_FromConfig
};
typedef ASUns8 PDOCContextInitPolicy;

/** PDOCBaseState enumerates the three legal values for the BaseState key in an optional
	content configuration dictionary (PDOCConfig). When initializing a PDOCContext
	using KOCCInit_FromConfig(), this enumeration represents the starting state of the
	Optional Content Groups (OCGs) before the contents of the config's ON and OFF OCG lists are processed.
	If the BaseState is Unchanged, and the PDOCConfig is just being constructed,
	the current states of the OCGs from the PDDoc's own PDOCConfig are used.
 */
enum {
	/** */
	kPDOCBaseState_OFF,
	/** */
	kPDOCBaseState_ON,
	/** */
	kPDOCBaseState_Unchanged
};
typedef ASUns8 PDOCConfigBaseState;

/** PDOCDrawEnumType controls drawing or enumerating the page with respect to optional content.
	It is an enumerated type that, together with the <code>NonOCDrawing</code> value, 
	controls drawing or enumerating content on a page with optional 
	content: 
	
	<ul>
	<li>Content that is marked as optional content is drawn or 
	not drawn according to the PDOCDrawEnumType and the visibility 
	state as determined by the Optional Content Groups (OCGs) and OCMDs. </li>
	<li>Content that is not marked as optional content is drawn 
	when <code>NonOCDrawing</code> is <code>true</code>, and not drawn when <code>NonOCDrawing</code> 
	is <code>false</code>. </li>
	</ul>
	
	@see PDOCContextGetOCDrawEnumType 
	@see PDOCContextSetOCDrawEnumType 
	@see PDOCContextGetNonOCDrawing 
	@see PDOCContextSetNonOCDrawing 
*/
enum {
	/**  Draw or enumerate optional content 
		that is visible, according to the current state of Optional Content Groups (OCGs) 
		and Optional Content Membership Dictionaries (OCMDs). This is the normal default mode. 
	*/
	kPDOC_VisibleOC = 0,
	/** Draw or enumerate all optional content, 
		regardless of its visibility state. If the context's <code>NonOCDrawing</code> 
		is <code>true</code>, all contents of document are shown. 
	*/
	kPDOC_AllOC,		
	/**	Draw or enumerate no optional content, 
		regardless of its visibility state. If the context's <code>NonOCDrawing</code> 
		is <code>false</code>, nothing is drawn, resulting in a blank page.
	*/
	kPDOC_NoOC,		
	/** */
	kPDOC_LastDrawEnumType = kPDOC_NoOC
};
typedef ASUns8 PDOCDrawEnumType;

/**	The optional-content group (OCG) state is changing. 
*/
enum {
	/** The OCGs' states are changing. */
	kPDOCGState,    
	/** The PDOCContext object's PDDrawEnumType is changing. */
	kPDOCContextDrawEnumType, 
	/** The PDOCContext object's non-optional content drawing is changing. */
	kPDOCContextNonOCDrawing, 
	/** The PDOCContext object's intent is changing. */
	kPDOCContextIntent,      
	/** The PDOCContext is being reset using PDOCContextInit(). */
	kPDOCContextInit,        
	/** */
	kPDOC_LastContextChangeType = kPDOCContextInit
};
typedef ASUns8 PDOCContextChangeType;

/** PDDocOCChangeType is an enumeration of types of changes to the optional content structures
	of a PDDoc. These types of changes may effect visibility in <i>all</i> PDOCContext objects.
	This enumeration is used in the <code>PDDocOCWillChange()</code> and <code>PDDocOCDidChange()</code> notifications.
	These notifications typically pass in the affected page, or <code>PDAllPages</code> if all
	pages may be affected.
 */
enum {
	/** Optional Content Groups (OCGs) created.
	*/
	kPDOCGCreate,				
	/** OCG properties changed.
	*/
	kPDOCGProperties,			
	/** An OCG was replaced by another.
	*/
	kPDOCGReplace,				
	/** An OCG was destroyed.
	*/
	kPDOCGDestroy,				
	/** Content was made optional.
	*/
	kPDOCMDAttach,				
	/** Content was made optional.
	*/
	kPDOCMDRemove,				
	/** An OC config was created.
	*/
	kPDOCConfigCreate,			
	/** An OC config was changed.
	*/
	kPDOCConfigChange,			
	/** An OC config was destroyed.
	*/
	kPDOCConfigDestroy,			
	/** OC was removed from document.
	*/
	kPDDocRemoveOC,				
	/**
	*/
	kPDOC_LastDocChangeType = kPDDocRemoveOC
};
typedef ASUns8 PDDocOCChangeType;

/* These callbacks are used for enumerating PDOCGs, PDOCMDs and PDOCConfigs.
 * Enumeration stops when all PDOCGs (PDOCMDs, PDOCConfigs) have been enumerated,
 * or if the callback returns <code>false</code>.
 */
/**
	A callback used for enumerating optional-content groups (OCGs). Enumeration stops when
	all OCGs have been enumerated, or when the callback returns <code>false</code>.
	@param ocg IN/OUT The optional-content group object.
	@param clientData IN/OUT A pointer to user-supplied data to pass to the proc each time it is called.
	@return <code>true</code> to continue enumeration, <code>false</code> to halt enumeration.
	@see PDDocEnumOCGs
	@see PDPageEnumOCGs
*/
typedef ACCBPROTO1 ASBool (ACCBPROTO2 *PDOCGEnumProc )(PDOCG ocg, void *clientData);
/**
	A callback used for enumerating optional-content configurations. 
	Enumeration stops when all configurations have been enumerated, 
	or when the callback returns <code>false</code>. 
	@param occonfig The optional-content configuration object. 
	
	@param clientData A pointer to user-supplied data to pass 
	to the proc each time it is called.
	@return <code>true</code> to continue enumeration, <code>false</code> to halt enumeration. 
	
	@see PDDocEnumOCConfigs 
*/
typedef ACCBPROTO1 ASBool (ACCBPROTO2 *PDOCConfigEnumProc)(PDOCConfig occonfig, void *clientData);
/* Controlling Acrobat trap presets */
enum {

/** */
kPDJoinMiter,

/** */
kPDJoinRound,

/** */
kPDJoinBevel
};
typedef ASEnum8 PDJoinStyle;
enum {

/** */
kPDEndMiter,

/** */
kPDEndOverlap
};
typedef ASEnum8 PDEndStyle;

enum {
kPDPlacementCenter,
kPDPlacementChoke,
kPDPlacementNeutralDensity,
kPDPlacementSpread
};
typedef ASEnum8 PDPlacementTypes;

typedef struct {
	ASBool noTrap;
	ASBool defaultTrap;
	float trapWidth;
	float blackWidth;
	float imageTrapWidth;
	ASUns32 trapJoinStyle;
	ASUns32 trapEndStyle;
	ASUns32 stepLimit;
	ASUns32 blackColorLimit;
	float blackDensityLimit;
	ASUns32 slidingTrapLimit;
	ASUns32 trapColorScaling;
	ASUns32 trapPlacement;
	ASBool imageToImageTrapping;
	ASBool imageToObjectTrapping;
	ASBool imageInternalTrapping;
	ASBool imagemaskTrapping;
	ASAtom trapStyleName;
} PDTrapPresetRec, *PDTrapPreset;

/*------------------------------------------------------------------------
	Controlling acrobat-based separations
--------------------------------------------------------------------------*/
enum { /* options for font inclusion */
	/** Embed no fonts.
	*/
	kHSEmitFontNoFonts,	
	/** Emit all embedded fonts.
	*/
	kHSEmitFontEmbeddedFonts,	
	/** Emit all fonts.
	*/
	kHSEmitFontAllFonts		
};

enum { /* Options for what to do with a plate when doing separations */
	/**
	*/
	kEmitPlate,
	/**
	*/
	kDontEmitPlate,
	/**
	Represents an ink used on a page. 
	@see AVPageViewGetNumVisibleInks 
	@see AVPageViewGetVisibleInks 
	@see AVPageViewSetInkPreview 
	@see AVPageViewSetVisibleInks 
	@see PDPageEnumInks 
*/
	kConvertToProcess,

	/**
	Can be used while doing color convert only. This is a matching flag for kColorConvToAltSpace flag in PDColorConvertActionType
	kConvertToProcess matches kColorConvConvert in PDColorConvertActionType
	kEmitPlate & kDontEmitPlate matches kColorConvPreserve in PDColorConvertActionType
	*/
	kConvertToAltSpace
};

/** Ink types. */
enum { 
	/** */
	kNormal,
	/** */
	kTransparent,
	/** */
	kOpaqueInk,
	/** */
	kOpaqueIgnore
};

enum {	/* marksStyle choices */
	/** No flags means InDesign style printer marks. */
	kPDDefaultMarkType = 0,		/* Acrobat defaults to InDesign style marks */
	kPDInDesignJ1MarkType,		/* InDesignJ1 */
	kPDInDesignJ2MarkType,		/* InDesignJ2 */
	kPDIllustratorMarkType,  	/* Illustrator */
	kPDIllustratorJ,			/* IllustratorJ */
	kPDQuarkXPress				/* QuarkXPress */
};
/**
	Represents an ink used on a page. 
	@see AVPageViewGetNumVisibleInks 
	@see AVPageViewGetVisibleInks 
	@see AVPageViewSetInkPreview 
	@see AVPageViewSetVisibleInks 
	@see PDPageEnumInks 
*/
typedef struct _t_PDPageInkRec {
	/** The size of the structure. It must be set to
		<code>sizeof(PDPageInkRec)</code>. 
	*/
	ASSize_t size;			
	/** The name of the colorant from the Separation or DeviceN colorspace, or
		the process color name.
	*/
	ASAtom colorantName;		
	/** <code>true</code> if this is a process color, <code>false</code> if this is a spot color.
	*/
	ASBool isProcessColor;		
	/** How to handle the colorant for a separation preview. Its value can be:
		<ul>
		<li>kEmitColorant</li>
		<li>kDontEmitColorant</li>
		<li>kConvertToProcess</li>
		</ul>
	*/
	ASUns8 whatToDo;            
	/** RGB values for on-screen display of a colorswatch.
	*/
	ASUns8 r, g, b;				
	/** The frequency for this ink.
	*/
	float	frequency;			
	/** The angle for this ink.
	*/
	float angle;				
	/** The ink type for this ink.
	*/
	ASUns8 inkType;				
	/** The trap sequence for this ink.
	*/
	ASInt32 trapSequence;		
	/** The neutral density for this ink.
	*/
	/* CMYK values for spot inks, provided so that neutral density can be calculated */
	float cyan;
	float magenta;
	float yellow;
	float black;

	float density;				
	/** Alias this spot to another.
	*/
	ASAtom alias;				

} PDPageInkRec, *PDPageInk;

/** Used for enumerating the inks on a page via PDPageEnumInks(). */
typedef ACCBPROTO1 ASBool (ACCBPROTO2 *PDPageEnumInksCallback) (PDPageInk ink, void *clientData);

/** Used to specify per-plate options when doing separations. */
typedef struct _t_PDHostSepsPlateRec {
	/** The size of the structure. It must be set to
		<code>sizeof(PDHostSepsPlateRec)</code>.
	*/
	ASSize_t size;				
	/** The name of the colorant from the Separation or DeviceN
		colorspace, or a spot or process color name (Cyan,
		Magenta, Yellow, Black).
	*/
	ASAtom colorantName;		
	/** How to handle the colorant. Its value can be:
		<ul>
		<li>kEmitPlate</li>
		<li>kDontEmitPlate</li>
		<li>kConvertToProcess</li>
		</ul>
	*/
	ASUns8 whatToDo;			
	/** Set internally. It is used to determine whether marks
		were made on this plate to aid in blank detection.
	*/
	ASUns32 wasColorSet;		
	/** Set internally. It is the stream into which Acrobat emits
		PostScript for this plate. It is <code>NULL</code> if <code>whatToDo</code> is not
		set to kEmitPlate.
	*/
	ASStm epsStm;				
	/** Set internally. It is used only when separating
		documents, to close the file afterward.
	*/
	ASFile file;				
	/** Set internally. It is used only when separating
		documents, to reopen the stream on the next page.
	*/
	ASPathName path;			
	/** The frequency for this ink. Set it to <code>-1</code> to use the default value.
	*/
	float	frequency;			
	/** The angle for this ink. Set it <code>-1</code> to use the default value.
	*/
	float	angle;				
	/** The ink type for this ink.
	*/
	ASUns8 inkType;				
	/** The trap sequence for this ink.
	*/
	ASInt32 trapSequence;		
	/** The neutral density for this ink.
	*/
	float density;				
	/** Alias this spot to another.
	*/
	ASAtom alias;				
} PDHostSepsPlateRec, *PDHostSepsPlate;

/** Used to control the generation of separations from Acrobat. */
typedef struct _t_PDHostSepsSpecRec {
	/** The size of the structure. It must be set to
		<code>sizeof(PDHostSepsSpecRec)</code>.
	*/
	ASSize_t size;						

	/** The PostScript printing level. <code>2</code> means emit as level
		2, <code>3</code> means level 3.
		<p>It is used if the <code>emitToPrinter</code> or <code>emitToFile</code>
		print parameter is <code>true</code>.</p>
	*/
	ASUns32 psLevel;					
	/** <code>true</code> if a binary channel to the printer is
		supported, <code>false</code> otherwise.
		<p>It is used if the <code>emitToPrinter</code> or <code>emitToFile</code>
		print parameter is <code>true</code>.</p>
	*/
	ASBool binaryOK;					

	/** When <code>true</code>, emit annotations.
	*/
	ASBool emitAnnots;
	/** When <code>true</code>, emit halftones.
	*/
	ASBool emitHalftones;
	/** When <code>true</code>, emit transfer functions.
	*/
	ASBool emitTransferFuncs;
	/** When <code>true</code>, emit separable images only.
	*/
	ASBool emitSeparableImagesOnly;

	/** When <code>true</code>, suppress CJK substitution.
	*/
	ASBool suppressCJKSubstitution;
	/** Font output options. Its values can be:

		<TABLE rules="all" cellspacing="1">
		<TR><TH>Value</TH><TH>Description</TH></TR>
		<TR><TD><code>kHSEmitFontNoFonts</code></TD><TD>Embed no fonts.</TD></TR>
		<TR><TD><code>kHSEmitFontEmbeddedFonts</code></TD><TD>Emit all embedded fonts.</TD></TR>
		<TR><TD><code>kHSEmitFontAllFonts</code></TD><TD>Emit all fonts.</TD></TR>
		</TABLE>
	*/
	ASEnum8 emitFontOption;				
	/** When <code>true</code>, send TrueType fonts as TrueType
		fonts (level 3 and most level 2 PS printers). When
		<code>false</code>, convert TrueType to T1 (typically desirable only
		for Level 1 PS where no TrueType handling is present).
	*/
	ASBool TTasT42;						
	/** <code>true</code> means do not include Far East fonts.
	*/
	ASBool  printerHasFarEastFonts;		

	/** The transparency level, which can be <code>0-100</code>.
	*/
	ASUns32 transparencyLevel;			

	/** When <code>true</code>, color manage DeviceCMYK. When
		<code>false</code>, pass it directly onto the process plates.
	*/
	ASBool	useCMYKWorkingColorspace;	

	/** The profile description of a valid CMYK profile, such
		as the strings seen in the color management preferences in Acrobat (for example,
		<code>"U.S. Web Coated (SWOP) v2"</code>).
	*/
	char    destProfile[256];			

	/** When <code>true</code>, use the overprint preview (OPP) for
		converting to process, which better simulates what would
		happen if the spot ink were really used.
	*/
	ASBool convertToProcessUsingOPP;	

	/** The number of items in the <code>plates</code> array.
	*/
	ASUns32 numPlates;					
	/** A list of the colorant names and what to do with
		them for separations.
	*/
	PDHostSepsPlate *plates;		
	/** The optional-content context to use for visibility
		state information, or <code>NULL</code> to use the document's
		current states in the default context.
	*/
	PDOCContext ocContext;	
	/** Print-specific visiblity state settings are always applied from the 
		optional-content group. applyOCGPrintOverrides is no longer honored.
		It is always considered as true whatever may be the value passed to it.
	*/
	ASBool      applyOCGPrintOverrides; 
	/** When <code>true</code>, invert the plate.
	*/
	ASBool		negative;				
	/** One of the following constants:

		<TABLE rules="all" cellspacing="1">
		<TR><TH>Constant</TH><TH>Value</TH></TR>
		<TR><TD><code>kPDPrintFlipNone</code></TD><TD><code>0x01</code></TD></TR>
		<TR><TD><code>kPDPrintFlipX</code></TD><TD><code>0x02</code></TD></TR>
		<TR><TD><code>kPDPrintFlipY</code></TD><TD><code>0x04</code></TD></TR>
		<TR><TD><code>kPDPrintFlipXY</code></TD><TD><code>0x08</code></TD></TR>
		</TABLE>

		<p>Mirroring is done in the PostScript output stream.</p>
	*/
	ASEnum8		mirrorprint;			
	/** A page mark indication. It is a bit-wise OR of the
		PDPageMarkFlags values.
	*/
	ASUns32		whichMarks;				
	/** When <code>true</code>, use the western style for page marks.
	*/
	ASBool		western;	/* obsolete */		
	ASBool		doInRipTrapping;	/* obsolete for Acrobat 8, use trapType */
	ASInt32		marksStyle;	/* 0 == default, indicates printer marks style to use */
	ASInt32		trapType;	/* for Acrobat 8, in-RIP or none */
	ASPathName	customMarksPath;	/* if non-NULL, specifies marks schema to use when whichMarks is non-NULL */
} PDHostSepsSpecRec, *PDHostSepsSpec;

/*------------------------------------------------------------------------*/

/**
	A callback for PDDocExportNotes. It determines whether an annotation
	is exported.

	@param doc IN/OUT The document from which annotations may be exported.

	@param pdpage IN/OUT The page from which the annotation may be
	exported.
	@param src IN/OUT The annotation that may be exported.
	@param dict IN/OUT A copy of the annotation in a Cos object.
	@return <code>true</code> to export the annotation, <code>false</code> to not export the annotation.
	@see PDDocWillImportAnnotCallback

	@note This is a different callback than PDDocWillExportAnnotProc().
*/
typedef ACCBPROTO1 ASBool (ACCBPROTO2 *PDDocWillExportAnnotCallback)(
	PDDoc doc, PDPage pdpage, PDAnnot src, CosObj dict);

/**
	A callback for PDDocImportCosDocNotes() and PDDocImportNotes().
	It determines whether an annotation will be imported.

	@param doc IN/OUT The document into which annotations may be imported.

	@param pdPage IN/OUT The page in which the annotation may be imported.

	@param annot IN/OUT The annotation that may be imported.
	@return <code>true</code> to import the annotation, <code>false</code> to not import the annotation.
	@see PDDocWillExportAnnotCallback

	@note This is a different callback than PDDocWillImportAnnotProc().
*/
typedef ACCBPROTO1 ASBool (ACCBPROTO2 *PDDocWillImportAnnotCallback)(
	PDDoc doc, PDPage pdPage, PDAnnot annot);

/**
	A structure used by PDDocCopyToFile() to specify file copy information. 
	
	@see PDDocCopyToFile 
*/
typedef struct _t_PDDocCopyParams {

	/** The size of the data structure. It must be set to <code>sizeof(PDDocCopyParamsRec)</code>.  */
	ASSize_t			size;	

	/** The path name to copy to, specified in whatever format is correct for <code>fileSys</code>.  */
	ASPathName			newPath;	

	/** A pointer to an ASFileSysRec containing the file system that does the copy. */
	ASFileSys			fileSys;	

	/** A progress monitor. It may be <code>NULL</code> */
	ASProgressMonitor	progMon;	

	/** A pointer to user-supplied data to pass to <code>progMon</code> each time it is
		called. It must be <code>NULL</code> if <code>progMon</code> is <code>NULL</code>. */
	void				*progMonData;

	/** A cancel procedure. It may be <code>NULL</code>. */
	ASCancelProc		cancelProc;	

	/** A pointer to user-specified data to pass to <code>cancelProc</code> each time it is called. It must be <code>NULL</code> 
		if <code>cancelProc</code> is <code>NULL</code>. 
		*/
	void				*cancelProcData;

	/** Determines whether changes should be saved if 
		the document is dirty. When it is <code>true</code>, if the document is dirty 
		and the product is Acrobat, save all in-memory changes to 
		the new copy. Otherwise, just copy the on-disk bytes. It is ignored 
		in Adobe Reader.
	*/
	ASBool				saveChanges;	
} PDDocCopyParamsRec, *PDDocCopyParams;


/** Nothing.
	@ingroup PrinterEmitFlags
*/
#define kPDEmitNoMarks			0	

/** Tile marks.
	@ingroup PrinterEmitFlags
*/
#define	kPDEmitWesternTileMarks	0x0001	

/** Tile marks.
	@ingroup PrinterEmitFlags
*/
#define	kPDEmitEasternTileMarks	0x0002	

/** Emit information about the document, name, page number, and so on. 
	@ingroup PrinterEmitFlags
*/
#define kPDEmitSlug			0x0004	

/** Printing flags 

@see PDDocDidPrintTiledPage
@see PDDocPrintingTiledPage
@see PDDocWillPrintTiledPage
*/
typedef struct {

	/** The number of points to overlap (user interface units may be anything, clients convert to points). */
	ASUns32	overlap;	

	/** Center the pages' contents on the physical paper. */
	ASBool	center;		

	/** Determines which printer marks to emit.
		@see PrinterEmitFlags
	*/
	ASInt32	marksflags;		

	/** The width of the paper (points). It is client-provided since the client has PPD access. */
	ASInt32 paperWidth;	

	/** The height of the paper (points). */
	ASInt32 paperHeight; 

	/** The title string for the slug (optional). */
	char *docTitle;		

	/** The date string for the slug (optional). */
	char *docDate;		

	/** The time string for the slug (optional). */
	char *docTime;		
	/* the remaining fields are for communicating during print time
	the current page's state; which page is being printed, etc. */

	/** The current column <code>(0 - numcols-1)</code>. */
	ASInt32 col;		

	/** The current row. */
	ASInt32 row;		

	/** The number of columns for this page. */
	ASInt32 numCols;	

	/** The number of rows for this page. */
	ASInt32 numRows;	

	/** The amount to shift right. First tile to center the entire image on the sheets. */
	ASInt32 xOffset;	

	/** The amount to shift down. */
	ASInt32 yOffset;	
}PDTileRec, *PDTile;

/*****************************
** Definitions for logical page areas
******************************/

/** Different logical areas on a page. */
enum {

	/** */
	kPDPageArea,

	/** */
	kPDClipArea,

	/** */
	kPDNumAreas
};
typedef ASEnum16 PDPageArea;

/**
	Used by PDDocExportSomeNotes(). It represents an array of PDAnnot objects. 
	
	@see PDDocExportSomeNotes 
*/
typedef struct _s_PDAnnotArray
{

	/** The annotation count. */
	ASTArraySize annotCount;

	/** A pointer to an array of PDAnnots. */
	PDAnnot *annots;
} PDAnnotArrayRec, *PDAnnotArray;


#if !MAC_PLATFORM || !__LP64__
	
/************************************************************
 ***
 *** Drawing params for PDPageDrawWithParams, PDDrawCosObjWithParams. Acrobat 6.0
 ***
 ************************************************************/
/**
	Parameters used for optional-content drawing control. The 
	parameters are the same as those passed to the original 
	version of the method, with the addition of an optional-content 
	context that determines what contents are visible. 
	This structure is not available on Mac 64.
	@see PDDrawCosObjWithParams 
	@see PDPageEnumContents 
*/
typedef struct _t_PDDrawParams
{
	/** The size of the data structure.
	*/
	ASUns32 size;			
	/** A pointer to a platform-dependent window object
		(<code>HWND</code> on Windows<code>WindowPtr</code>, or <code>CWindowPtr</code> on Mac OS). 
		On Windows, to draw into an offscreen <code>DC</code>, pass <code>NULL</code> for <code>window</code>.
		On Mac OS, to draw into an offscreen
		<code>GWorld</code>, pass <code>NULL</code> in window and pass the
		<code>GWorldPtr</code> in <code>displayContext</code>. 
	*/
	void *window;			
	/** A platform-dependent display context structure
		(<code>HDC</code> on Windows, <code>GWorldPtr</code> on Mac OS). On Mac OS,
		<code>displayContext</code> is ignored if <code>window</code> is non-<code>NULL</code>.
		@note <code>displayContext</code> cannot be reliably used as the <code>hDC</code> for a printer device.
	*/
	void *displayContext;	
	/** A pointer to the matrix to concatenate onto the default
		page matrix. It is useful for converting from page to
		window coordinates and for scaling.
	*/
	ASFixedMatrix matrix;	
	/** See above. The only value you should use is <code>0</code>.
	*/
	ASUns32 flags;			
	/** A rectangle represented by the coordinates of its four sides.
	*/
	ASFixedRect updateRect;	
	/**
		A procedure called periodically to check for the user's cancelling of
		the drawing operation. The default cancel procedure
		can be obtained using AVAppGetCancelProc(). It may
		be <code>NULL</code>, in which case no cancel procedure is used.
	*/
	CancelProc cancelProc;	
	/** A pointer to user-supplied data to pass to <code>cancelProc</code>
		each time it is called. It should be <code>NULL</code> if <code>cancelProc</code>
		is <code>NULL</code>.
	*/
	void *cancelProcClientData;

	/** An optional-content context that determines what
		contents are visible. <code>NULL</code> uses the document's
		optional-content context, as returned by
		<code>PDDocGetOCContext(pddoc)</code>, which is equivalent
		to calling the version of the method without optional content
		parameters.
		<p>This context is copied and the copy is used in drawing.
		This allows a client to change its copy of the context
		without raising an exception.</p>
	*/
	PDOCContext clientOCContext;
} PDDrawParamsRec, *PDDrawParams;

#endif //!MAC_PLATFORM || !__LP64__	
	
/************************************************************
 ***
 *** Enumeration params for PDFormEnumPaintProcWithParams, PDCharProcEnumWithParams,
 *** PDPageEnumContentsWithParams.
 ***
 ************************************************************/

	/**
	Enumeration parameters used for optional-content drawing 
	control in PDFormEnumPaintProcWithParams() and PDCharProcEnumWithParams(). 
	The parameters are the same as those passed to the original 
	versions of these methods (PDFormEnumPaintProc() and PDCharProcEnum()), 
	with the addition of an optional-content context that determines 
	what contents are visible. 
	@see PDFormEnumPaintProcWithParams 
	@see PDCharProcEnumWithParams 
*/
typedef struct _t_PDGraphicEnumParams {
	/**The size of the data structure. 
	*/
	ASUns32 size;			     
	/** An optional-content context that 
		determines what contents are visible. <code>NULL</code> uses the document's 
		optional-content context, as returned by <code>PDDocGetOCContext(pddoc)</code>, 
		which is equivalent to calling the version of the method 
		without optional-content parameters. This context is copied 
		and the copy is used in drawing. This allows a client to 
		change its copy of the context without raising an exception.
	*/
	PDOCContext clientOCContext;  

	/** Filled by the method with the context 
		that will be used during enumeration. This is a copy of 
		the context specified by <code>clientOCContext</code>. 
	*/
	PDOCContext usedOCContext;    

	/** The graphic enumeration monitor. 
	*/
	PDGraphicEnumMonitor mon;     
	/** A pointer to user-supplied data to pass to the monitor.
	*/
	void *monObj;                 
} PDGraphicEnumParamsRec, *PDGraphicEnumParams;


/************************************************************/
/************************************************************
 ***
 *** Callbacks for PDDocRequestPages and PDDocRequestEntireFile
 ***
 ************************************************************/

/** This tells the callback why it is being called. */
enum {
	/** The request is still being processed. 
	*/
	kPDDocRequestUnderway = 0, 
	/** The requested data has arrived.
	*/
	kPDDocRequestComplete, 
	/** The request is cancelled because the file is being closed.
	*/
	kPDDocRequestCancelled, 
	/** An error occurred. 
	*/
	kPDDocRequestError		
};
typedef ASEnum8	PDDocRequestReason;

/** A callback for PDDocRequestPages(). */
typedef ACCBPROTO1 ASInt32 (ACCBPROTO2 *PDDocRequestPagesProc)
(PDDoc pdDoc,  ASInt32 startPage,  ASInt32 nPages, PDDocRequestReason reason,  void *clientData);

/**
	A callback used by PDDocRequestEntireFile. Use this callback 
	to process a document file. 
	@param pdDoc The PDDoc to be saved.
	@param reason A constant representing the status 
	of the request. It can have one of the following values:
	<ul>
	<li>kPDDocRequestUnderway</li>
	<li>kPDDocRequestComplete </li>
	<li>kPDDocRequestCancelled</li>
	<li>kPDDocRequestError </li>
	</ul>
	
	@param clientData User-supplied data passed in the PDDocRequestPages() 
	method.
	@return <code>0</code> when successful, a non-zero error code otherwise. 
	@see PDDocRequestEntireFile 
*/
typedef ACCBPROTO1 ASInt32 (ACCBPROTO2 *PDDocRequestEntireFileProc)
(PDDoc pdDoc,  PDDocRequestReason reason, void *clientData);

/************************************************************/

/* This typedef appears in both PDExpT.h and PEExpT.h now, so avoid multiple typedef's here */
#ifndef _T_PDEFONT_
#define _T_PDEFONT_
typedef struct _t_PDEFont *PDEFont;
#endif

enum {
	kPDHorizLeft = 0, 
	kPDHorizCenter, 
	kPDHorizRight
};
typedef ASEnum8	PDHorizAlign;

enum {
	kPDVertTop = 0, 
	kPDVertCenter, 
	kPDVertBottom
};
typedef ASEnum8	PDVertAlign;

/**
	Parameters used for describing text-based watermarks.
	@see PDDocAddWatermarkFromText
*/
typedef struct _t_PDDocWatermarkTextParams {
	/** The size of the data structure. 
	*/
	ASSize_t		size;

	/** The text to be used when generating a text-based watermark.
	*/
	ASText			srcText;

	/** The alignment to be used when justifying a text-based watermark.
	*/
	PDHorizAlign	textAlign;

	/** The PDEFont to be used when generating a text-based watermark.
		If it is <code>NULL</code>, the font specified by <code>sysFontName</code> will be used.
	*/
	PDEFont			pdeFont;

	/** The name of a system font to be used when generating a text-based
		watermark. The font will be embedded/subsetted when possible.
		This parameter is ignored if <code>pdeFont</code> is non-<code>NULL</code>.
	*/
	ASAtom			sysFontName;

	/** The size of the font, in points, to be used when generating a text-based
		watermark.
	*/
	float			fontSize;

	/** The color to be used when generating a text-based watermark.
	*/
	PDColorValueRec	color;
} PDDocWatermarkTextParamsRec, *PDDocWatermarkTextParams;


/**
	Parameters used for adding and describing watermarks.
	@see PDDocAddWatermarkFromPDPage
	@see PDDocAddWatermarkFromText
*/
typedef struct _t_PDDocAddWatermarkParams {
	/** The size of the data structure. 
	*/
	ASSize_t		size;

	/** The page range of the document to which the watermark should be added.
	*/
	PDPageRange		targetRange;

	/** A boolean specifying whether this watermark is a FixedPrint watermark.
		FixedPrint watermarks maintain their size and position regardless of the
		dimensions of the target media.
	*/
	ASBool			fixedPrint;

	/** A boolean specifying where in the page z-order the watermark should be added.
		If it is <code>true</code>, the watermark is added to the front of the page; otherwise, it is
		added as a background.
		<p>This parameter is ignored if <code>fixedPrint</code> is <code>true</code>, as all FixedPrint watermarks are
		added to the front of the page.</p>
	*/
	ASBool			zOrderTop;

	/** A boolean specifying whether the watermark should be visible when viewing
		on screen.
	*/
	ASBool			showOnScreen;

	/** A boolean specifying whether the watermark should be printed.
	*/
	ASBool			showOnPrint;

	/** The horizontal alignment to be used when adding the watermark to a page.
	*/
	PDHorizAlign	horizAlign;

	/** The vertical alignment to be used when adding the watermark to a page.
	*/
	PDVertAlign		vertAlign;

	/** The horizontal offset value to be used when adding the watermark on a page.  If
		<code>percentageVals</code> is <code>true</code>, this value is a percentage of the page width,
		with <code>1.0</code> meaning 100%.  Otherwise this value is in user units.
	*/
	float			horizValue;

	/** The vertical offset value to be used when adding the watermark on a page.  If
		<code>percentageVals</code> is <code>true</code>, this value is a percentage of the page height,
		with <code>1.0</code> meaning 100%.  Otherwise this value is in user units.
	*/
	float			vertValue;

	/** A boolean specifying the units of <code>horizValue</code> and <code>vertValue</code>. If it is <code>true</code>, 
		<code>horizValue</code> and <code>vertValue</code> represent percentages of the page dimensions.
		Otherwise <code>horizValue</code> and <code>vertValue</code> are in user units.
	*/
	ASBool			percentageVals;

	/** The scale factor to be used when adding the watermark, with <code>1.0</code> meaning 100%.
	*/
	float			scale;

	/** The counter-clockwise rotation, in degrees, to be used when adding the watermark.
	*/
	float			rotation;

	/** The opacity to be used when adding the watermark, with <code>0.0</code> meaning fully
		transparent and <code>1.0</code> meaning fully opaque.
	*/
	float			opacity;

	/** The progress monitor to be updated when adding the watermark.
		It may be <code>NULL</code>.
	*/
	ASProgressMonitor	progMon;

	/** The private data to be passed to <code>progMon</code>.
		This parameter is ignored if <code>progMon</code> is <code>NULL</code>.
	*/
	void*				progMonData;

	/** The cancel procedure to be checked when adding the watermark.
		It may be <code>NULL</code>.
	*/
	ASCancelProc		cancelProc;

	/** The private data to be passed to <code>cancelProc</code>.
		This parameter is ignored if <code>cancelProc</code> is <code>NULL</code>.
	*/
	void*				cancelProcData;
} PDDocAddWatermarkParamsRec,  *PDDocAddWatermarkParams;

enum {
	/**  Erase the page while rendering only as needed.*/
	kPDPageDoLazyErasePI			   = 0x00000001,

	/** Ignore Isolated and Knockout transparency at page boundary.*/
	kPDPageIgnoreIsolatedAndKnockoutTransparencyGroupPI
								   = 0x00000010,

	/** Draw annotation appearances.*/
	kPDPageUseAnnotFacesPI		   = 0x00000040,

	/** The page is being printed.*/
	kPDPageIsPrintingPI			   = 0x00000080,

	/** Display overprint preview. */
	kPDPageDisplayOverPrintPreviewPI = 0x00000100,

	/** Use trap network annotations. */
	kPDPageUseTrapAnnotsPI		   = 0x00002000,	

	/** Directly imposed page. */
	kPDPageDirectlyImposedPI         = 0x00004000,

	/** PostScript printing. */
	kPDPageIsPSPrintingPI			   = 0x00008000,

	/** Emit a page group. */
	kPDPageEmitPageGroupPI		   = 0x00010000,

	/** User printer's mark annotations. */
	kPDPageUsePrinterMarkAnnotsPI	   = 0x00020000,

	/** Pass open prepress interface (OPI) to AGM port. */
	kPDPagePassOPItoAGMPortPI		   = 0x00040000,

	/** Pass metadata to AGM port. */
	kPDPagePassMetadatatoAGMPortPI   = 0x00080000,

	/** Pass optional content to AGM port. */
	kPDPagePassOCtoAGMPortPI		   = 0x00100000,

    /** Do not substitute working spaces. */
    kPDPageDoNotSubstituteWorkingSpacesPI =
                                     0x00800000,

    /** Render colors in BGR order rather than RGB. It is only valid for calls to
	    PDPageDrawContentsToMemory() and only when outputing to an RGB colorspace. */
	kPDPageSwapComponentsPI          = 0x01000000,

    /** Supress raster alpha. */
    kPDPageSuppressRasterAlphaPI     = 0x02000000,

    /** If this is set, only use a working space instead of a device space
        if the process color model of the target device is different than
        that of the source */
    kPDPageWorkingSpacesOnlyForChangePI = 0x04000000,

	/** If set, only consider Stamp annotations. This overrides kPDPageUseAnnotFaces. */
	kPDPageUseStampAnnotsOnlyPI		= 0x08000000
		
};
typedef ASUns32 PDPageDrawFlagsPI;

/**
 Const strings used as ASCab keys when passing drawing flags. These are valid only
 for calls to PDDrawPageOrCosObjectToBuffer.
 @see PDDrawPageOrCosObjectToBuffer
 */

/**
 Erase the page while rendering only as needed.
 @ingroup PDPageDrawFlagsStr
 @see PDDrawPageOrCosObjectToBuffer
 */
#define kPDPageDoLazyEraseStr "DoLazyErase"
    
/**
 Ignore Isolated and Knockout transparency at page boundary.
 @ingroup PDPageDrawFlagsStr
 @see PDDrawPageOrCosObjectToBuffer
 */
#define kPDPageIgnoreIsolatedAndKnockoutTransparencyGroupStr "IgnoreIsolatedAndKnockoutTransparencyGroup"
    
/**
 Draw annotation appearances.
 @ingroup PDPageDrawFlagsStr
 @see PDDrawPageOrCosObjectToBuffer
 */
#define kPDPageUseAnnotFacesStr "UseAnnotFaces"
    
/**
 The page is being printed.
 @ingroup PDPageDrawFlagsStr
 @see PDDrawPageOrCosObjectToBuffer
 */
#define kPDPageIsPrintingStr "IsPrinting"
    
/**
 Display overprint preview.
 @ingroup PDPageDrawFlagsStr
 @see PDDrawPageOrCosObjectToBuffer
 */
#define kPDPageDisplayOverPrintPreviewStr "DisplayOverPrintPreview"
    
/**
 PostScript printing.
 @ingroup PDPageDrawFlagsStr
 @see PDDrawPageOrCosObjectToBuffer
 */
#define kPDPageIsPSPrintingStr "IsPSPrinting"
    
/**
 Emit a page group.
 @ingroup PDPageDrawFlagsStr
 @see PDDrawPageOrCosObjectToBuffer
 */
#define kPDPageEmitPageGroupStr "EmitPageGroup"
    
/**
 Do not substitute working spaces.
 @ingroup PDPageDrawFlagsStr
 @see PDDrawPageOrCosObjectToBuffer
 */
#define kPDPageDoNotSubstituteWorkingSpacesStr "DoNotSubstituteWorkingSpaces"
    
/**
 Render colors in BGR order rather than RGB. It is only valid for calls to
 PDDrawPageOrCosObjectToBuffer() and only when outputing to an RGB colorspace.
 @ingroup PDPageDrawFlagsStr
 @see PDDrawPageOrCosObjectToBuffer
 */
#define kPDPageSwapComponentsStr "SwapComponents"
    
/**
 Supress raster alpha.
 @ingroup PDPageDrawFlagsStr
 @see PDDrawPageOrCosObjectToBuffer
 */
#define kPDPageSuppressRasterAlphaStr "SuppressRasterAlpha"
    
/**
 If this is set, only use a working space instead of a device space if the process
 color model of the target device is different than that of the source.
 @ingroup PDPageDrawFlagsStr
 */
#define kPDPageWorkingSpacesOnlyForChangeStr "WorkingSpacesOnlyForChange"
    
/**
 If set, only consider Stamp annotations. This overrides kPDPageUseAnnotFaces.
 @ingroup PDPageDrawFlagsStr
 @see PDDrawPageOrCosObjectToBuffer
 */
#define kPDPageUseStampAnnotsOnlyStr "UseStampAnnotsOnly"
    
/**
 Takes ASext as input. If set we use this profile as blending profile.
 kPDPageDisplayOverPrintPreviewStr must be set for this to get honored.
 @ingroup PDPageDrawFlagsStr
 @see PDDrawPageOrCosObjectToBuffer
 */
#define kPDPageBlendingProfileValueStr "BlendingProfileDesc"
    
/**
 Draw smooth text.
 @ingroup PDPageDrawSmoothFlagsStr
 @see PDDrawPageOrCosObjectToBuffer
 @see PDPageDrawSmoothFlags
 */
#define kPDPageDrawSmoothTextStr "SmoothText"
    
/**
 Draw smooth line art.
 @ingroup PDPageDrawSmoothFlagsStr
 @see PDDrawPageOrCosObjectToBuffer
 @see PDPageDrawSmoothFlags
 */
#define kPDPageDrawSmoothLineArtStr "SmoothLineArt"
    
/**
 Draw smooth image.
 @ingroup PDPageDrawSmoothFlagsStr
 @see PDDrawPageOrCosObjectToBuffer
 @see PDPageDrawSmoothFlags
 */
#define kPDPageDrawSmoothImageStr "SmoothImage"
    
/**
 Draw smooth image using bicubic resampling. This option can have performance overhead.
 @ingroup PDPageDrawSmoothFlagsStr
 @see PDDrawPageOrCosObjectToBuffer
 @see PDPageDrawSmoothFlags
 */
#define kPDPageDrawSmoothBicubicImageStr "SmoothImageUsingBicubicResampling"
    
/**
	Parameters used to represent the properties for a redaction mark.
	@see PDDocCreateRedaction
	@see PDRedactionGetProps
	@see PDRedactionSetProps
*/
typedef struct _t_PDRedactParams
{
	/** The size of the data structure. */
	ASUns32 size;

	/** The page number where this redaction mark is found. */
	ASInt32 pageNum;

	/** An array of <code>ASFixedQuad</code> objects representing the regions on the page to be redacted. */
	ASFixedQuad* redactQuads;

	/** The length of the array specified by <code>redactQuad</code>. */
	ASUns32 numQuads;
	
	/** The color of the redaction mark. This color is also used as the fill color when
		generating the replacement form for this redaction mark. The replacement form is the content
		that will be placed on the page when this mark is applied and the underlying content is removed.
	*/
	PDColorValueRec* colorVal;
	
	/** The overlay text to be used when generating the replacement form for this redaction mark.
		The replacement form is the content that will be placed on the page when this mark is applied
		and the underlying content is removed.
	*/
	ASText overlayText;

	/** The horizontal alignment to be used for the overlay text when generating the replacement
		form for this redaction mark.
	*/
	PDHorizAlign horizAlign;

} PDRedactParamsRec, *PDRedactParams;

/**
	Parameters used for applying redaction marks.
	@see PDDocApplyRedactions
*/
typedef struct _t_PDApplyRedactionParams
{
	/** The size of the data structure. */
	ASUns32 size;

	/** A pointer to a set of redaction annotations representing the redaction marks to be applied.
		If <code>NULL</code>, then all redaction marks present in the document should be applied.
	*/
	PDAnnot* redactionAnnots;

	/** The number of redaction marks in <code>redactionAnnots</code>. This value is ignored if
		<code>redactionAnnots</code> is <code>NULL</code>.
	*/
	ASUns32 redactionCount;

	/** A boolean value indicating that the redaction marks themselves should be not removed. */
	ASBool keepMarks;

	/** A set of callbacks for providing feedback to the caller. It may be <code>NULL</code>. */
	ASStatusMonitorProcs statusProcs;

	/** The text to be displayed for the progress monitor in <code>statusProcs</code>. It may be
		<code>NULL</code>. This value is ignored if <code>statusProcs</code> is <code>NULL</code>.
	*/
	ASText progressText;

	/** The amount that the current value for the progress monitor in <code>statusProcs</code>
		should be incremented during the application process. If it is less than or equal to <code>0</code>, then
		<code>beginOperation</code> will be called, an unspecified positive duration will be
		set, the current value will be incremented up to that duration, and
		<code>endOperation</code> will be called.
		This value is ignored if <code>statusProcs</code> is <code>NULL</code>.
	*/
	ASInt32 progressDuration;
} PDApplyRedactionParamsRec, *PDApplyRedactionParams;

/************************************************************************************\
|*																					*|
|*									PDFileAttachment								*|
|*																					*|
\************************************************************************************/

/**
	A <code>PDFileAttachment</code> represents an embedded file stored in a PDF file, and may be
	stored at various locations in a PDF file, including the <code>EmbeddedFiles</code> name tree,
	<code>FileAttachment</code> annotation types, and <code>Multimedia</code> annotations.

	@see PDFileAttachmentNewFromFile
	@see PDFileAttachmentUpdateFromFile
	@see PDFileAttachmentSaveToFile
*/

typedef OPAQUE_64_BITS PDFileAttachment;


/************************************************************************************\
|*																					*|
|*									PDCollection									*|
|*																					*|
\************************************************************************************/

/**
	<p>A <code>PDCollection</code> represents a collection dictionary in a PDF file.</p>

	<p>Collection view types and split types are unusual due to the way the <i>PDF 
	Reference</i> is written. A straight mapping of the View key and the Split
	key would result in the key/value of <code>/View/H</code> being part of the view type enumeration,
	but that would result in ambiguity when setting View to H and Split to N
	(<code>/View/H</code> maps to "preview" in Acrobat 8, but <code>/Split/N</code> is documented to map
	to "no split, show navigator" in Acrobat 9. Because of this, and the fact that
	<code>/View/H</code> is really interpreted as a kind of split rather than a style of navigator,
	Preview was moved to the split type enumeration in the API.
	This interacts with the underlying file as follows: when a <code>/View/H</code> value is read it is mapped
	to "Tile" and "Preview" for the navigator style and split position.
	When the Tile/Preview combination is written out, it is recorded back to <code>/View/H</code> for consistency
	with Acrobat 8.</p>

	@see PDDocGetPDCollection
	@see PDDocCreatePDCollection
	@see PDDocDeleteCollection
*/
typedef OPAQUE_64_BITS PDCollection;

/** The name of a field in a collection sort dictionary, and its associated value for ascending. */
typedef struct _t_PDCollectionSchemaSortPair {
	ASAtom fieldName;
	ASBool ascending;
} PDCollectionSchemaSortPairRec;

/** Opaque object representing a collection navigator dictionary. */
typedef OPAQUE_64_BITS PDNavigator;

/** Collection view types. */
enum {
	/** Tile navigator. */
	kCollectionViewTile = 0x1000,
	/** Details navigator. */
	kCollectionViewDetails,	
	/** Custom navigator. */
	kCollectionViewCustom		
};
typedef ASUns16 PDCollectionViewType;

/** Collection split types. */
enum {
	/** Default split based on view. */
	kCollectionSplitDefault = 0x2000,	
	/** Split vertical. */
	kCollectionSplitHorizontal,	
	/** Split horizontal. */
	kCollectionSplitVertical,	
	/** No split; show the navigator. */
	kCollectionSplitNone,	
	/** No split; show the preview. */
	kCollectionSplitPreview		
};
typedef ASUns16 PDCollectionSplitType;

/** Collection view data record. */
typedef struct _t_PDCollectionViewDataRec {
	/** The size of the <code>PDCollectionViewDataRec</code>. */
	ASSize_t size;	
	/** The view type of the collection. */
	PDCollectionViewType view;  
	/** The navigator to use with the collection. This is ignored if <code>view != custom</code>. */
	PDNavigator navigator;	     
	/** The split type of the collection. */
	PDCollectionSplitType split;    

	/** A percentage from <code>0-100</code>. It is ignored if the position is not a split view.
	    If the position is a split view, <code>-1</code> indicates an application-defined position. */
	ASInt32 splitPos;
} PDCollectionViewDataRec;

/** 
	<p>An opaque pointer to a collection schema object. </p>

	<p>A <code>PDCollectionSchema</code> is ordered with the following considerations.
    The <code>PDCollectionSchema</code> has characteristics of both a collection of named
    fields and an array. Field names are guaranteed to be unique.
    Indexing is guaranteed to be 0-based, in ascending order, with no gaps.
    When repositioning a field by changing its index, the schema will be
    re-indexed as necessary to conform to this convention. If a field is assigned
    an out-of-bound index, the index is adjusted to be in bounds rather
    than raising an error.</p>
*/
typedef struct CPDCollectionSchema *PDCollectionSchema;

/** A structure corresponding to a field in the collection schema. */
typedef struct _t_PDCollectionField {
	/** The size of the <code>PDCollectionFieldRec</code>. */
	ASSize_t size;		
	/** The ID of the field. */
	ASAtom fieldName;	
	/** The 0-based index of the field in the <code>PDCollectionSchema</code>. */
	ASInt32 index;	
	/** The subtype of the field. For example <code>/N</code> represents a number and <code>/S</code> represents a string. */
	ASAtom subtype;		
	/** The name to use in the user interface. */
	ASText fieldText;	
	/** Default visibility. */
	ASBool visible;		
	/* <code>true</code> if the user may edit the field, <code>false</code> otherwise. */
	ASBool canEdit;		
} PDCollectionFieldRec;

/** An opaque object representing a collection folder dictionary.
    Folders are used to provide grouping for files in a portable collection.
*/
typedef OPAQUE_64_BITS PDFolder;


#ifdef __cplusplus
}
#endif

#endif /* _H_PDExpT */
