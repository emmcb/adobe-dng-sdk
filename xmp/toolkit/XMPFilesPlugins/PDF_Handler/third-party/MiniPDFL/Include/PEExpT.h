/*********************************************************************

 ADOBE SYSTEMS INCORPORATED
 Copyright (C) 1995-2007 Adobe Systems Incorporated
 All rights reserved.

 NOTICE: Adobe permits you to use, modify, and distribute this file
 in accordance with the terms of the Adobe license agreement
 accompanying it. If you have received this file from a source other
 than Adobe, then your use, modification, or distribution of it
 requires the prior written permission of Adobe.

 ---------------------------------------------------------------------

 PEExpT.h

 - Types, macros, structures, etc. required to use the PDFEdit HFTs.

*********************************************************************/

#ifndef _H_PEExpT
#define _H_PEExpT

#include "Environ.h"
#if PLUGIN || ACROBAT_LIBRARY
#include "CoreExpT.h"
#include "ASExpT.h"
#include "CosExpT.h"
#include "PDExpT.h"	/* ASFixedQuad */
#else
#include "PubTypes.h"
#include "ASTypes.h"
#include "ASExtndT.h"
#include "CosTypes.h"
//#include "ASFixed.h"
#include "FixedGeo.h"
#include "FixedMtx.h"
#include "DoubleMtx.h"
#include "DoubleRect.h"
#include "ASStm.h"
#endif

#include "PDSysFontExpT.h"

#if PLUGIN
#define PEX1 ACCB1
#define PEX2 ACCB2
#else
#define PEX1 ACEX1
#define PEX2 ACEX2
#endif

#if defined( WINDOWS ) || defined( WIN_ENV )
#pragma pack(push, peexpt, 8)
#endif

#ifdef __cplusplus
extern "C" {
#endif

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
	PDFEdit Public Types.
------------------------------------------------------------------------*/


/** The abstract super class of the PDFEdit classes. You can find the type of any object with
	the PDEObjectGetType() method. You can then cast and apply that class' methods
	to the object. In addition, you can cast any of the PDFEdit objects to a PDEObject
	and use it anywhere a PDEObject is called for, such as in the PDEObject methods.
	@see PDERelease
	@see PDEObjectDump
*/
typedef struct _t_PDEObject *PDEObject;

/** Contains the modifiable contents of a PDPage. A PDEContent object may be obtained from
	an existing page, from a Form XObject, or from a Type 3 CharProc. You can create
	an empty PDEContent object. A PDEContent object contains PDEElement objects. In addition, a
	PDEContent object may have attributes such as a Form matrix and <code>setcachedevice</code>
	parameters.
	@see PDEContentCreate
	@see PDEContainerGetContent
	@see PDEContentCreateFromCosObj
	@see PDEFormGetContent
	@see PDPageAcquirePDEContent
	@see PDERelease
*/
typedef struct _t_PDEContent *PDEContent;

/** The base class for elements of a page display list (PDEContent) and for clip objects.
The general PDEElement methods allow you to get and set general element
properties.
@see PDEContainer (subclass)
@see PDEForm (subclass)
@see PDEGroup (subclass)
@see PDEImage (subclass)
@see PDEPath (subclass)
@see PDEPlace (subclass)
@see PDEPS (subclass)
@see PDEShading (subclass)
@see PDEText (subclass)
@see PDEUnknown (subclass)
@see PDEXObject (subclass)
@see PDEClipGetElem (subclass)
@see PDEContentGetElem (subclass)
@see PDEElementCopy (subclass)
@see PDERelease
*/
typedef struct _t_PDEElement *PDEElement;

/* Subclasses of PDEElement.
** PDEText			- a collection of text strings or words
** PDEPath			- a graphic path
** PDEImage			- an image
** PDEForm			- an XObject form
** PDEPS			- a pass-through PostScript object
** PDEXObject		- an XObject of unknown type
** PDEPlace	   		- a marked content place operator
** PDEContainer		- a marked content container
** PDEGroup			- a container of PDEElement objects	
** PDEUnknown		- an unknown element
*/

/** A PDEElement representing text. It is a container for text as show strings or as
	individual characters. Each sub-element may have different graphics state properties.
	However, the same clip applies to all sub-elements of a PDEText. Also, the
	<code>charpath</code> of a PDEText can be used to represent a clip.
	@see PDEElement (superclass)
	@see PDETextCreate
	@see PDERelease
*/
typedef struct _t_PDEText *PDEText;

/** A PDEElement that contains a path. Path objects can be stroked, filled, and/or serve
	as clipping paths.
	@see PDEElement (superclass)
	@see PDEPathCreate
	@see PDERelease
*/
typedef struct _t_PDEPath *PDEPath;

/** A PDEElement that contains an Image XObject or an inline image. You can associate
	data or a stream with an image.
	@see PDEElement (superclass)
	@see PDEImageCreate
	@see PDEImageCreateFromCosObj
	@see PDERelease
*/
typedef struct _t_PDEImage *PDEImage;

/** A PDEElement that corresponds to an instance of an XObject Form on a page (or
	another containing stream such as another XObject Form or annotation form). The
	context associated with this instance includes the actual CosObj stream that
	represents the XObject Form and the initial conditions of the graphics state. The
	latter consists of the transformation matrix, initial color values, and so forth. It is
	possible to have two PDEForm objects that refer to the same XObject Form. The forms will
	exist at different places on the same page, depending on the transformation matrix.
	They may also have different colors or line stroking parameters. In the case of a
	transparency group, the opacity is specified in the <code>gstate</code>.
	Within a PDEForm, each PDEElement has its own <code>gstate</code> (or is a container, place,
	or group object). These <code>gstates</code> are independent of the parent PDEForm <code>gstate</code>.
	PDEForm elements within the PDEForm may have their own opacity.
	A PDEContent may be obtained from a PDEForm to edit the form's display list.
	@see PDEElement (superclass)
	@see PDEFormCreateFromCosObj
	@see PDERelease
*/
typedef struct _t_PDEForm *PDEForm;

/** An element representing inline or XObject pass-through PostScript object. XObject
	PostScripts are listed in page XObject resources.
	@see PDEElement (superclass)
	@see PDEPSCreate
	@see PDEPSCreateFromCosObj
*/
typedef struct _t_PDEPS *PDEPS;

/** A PDEElement representing an arbitrary XObject.
	@see PDEElement (superclass)
	@see PDEXObjectCreate
	@see PDERelease
*/
typedef struct _t_PDEXObject *PDEXObject;

/** A PDEElement that marks a place on a page in a PDF file. In a PDF file, a place is
	represented by the MP or DP Marked Content operators.
	
	<p>Marked content is useful for adding structure information to a PDF file. For instance, a
	drawing program may want to mark a point with information, such as the start of a
	path of a certain type. Marked content provides a way to retain this information in the
	PDF file. A DP operator functions the same as the MP operator and, in addition,
	allows a property list dictionary to be associated with a place.</p>

	@see PDEElement (superclass)
	@see PDEPlaceCreate
	@see PDERelease
*/
typedef struct _t_PDEPlace *PDEPlace;

/** A PDEElement representing an unknown element.
	@see PDEElement (superclass)
*/
typedef struct _t_PDEUnknown *PDEUnknown;

/** A group of PDEElement objects on a page in a PDF file. In the PDF file, containers are
	delimited by Marked Content BMC/EMC or BDC/EMC pairs. Every PDEContainer
	has a Marked Content tag associated with it. In addition to grouping a set of elements,
	a BDC/EMC pair specifies a property list to be associated with the grouping. Thus, a
	PDEContainer corresponding to a BDC/EMC pair also has a property list dictionary
	associated with it.
	@see PDEContainerCreate
	@see PDERelease
*/
typedef struct _t_PDEContainer *PDEContainer;

/** An in-memory representation of objects in a PDEContent object. It has no state and is not
	represented in any way in a PDF content stream (that is, PDEContent).
	When used in a PDEClip, this object is used to associate PDEText objects into a
	single clipping object.
	@see PDEElement (superclass)
	@see PDEGroupCreate
*/
typedef struct _t_PDEGroup *PDEGroup;

/** A PDEElement that represents smooth shading.
	@see PDEElement (superclass)
	@see PDEShadingCreateFromCosObj
	@see PDEShadingCreateFromCosObj
	@see PDEShadingGetCosObj
*/
typedef struct _t_PDEShading *PDEShading;

/** The PDFEdit representation of the opening bracket of a marked-content sequence.
	Elements of this type must be paired with elements of type PDEEndContainer.
	@see PDEElement (superclass)
	@see PDEBeginContainerCreate
	@see PDERelease
*/
typedef struct _t_PDEBeginContainer *PDEBeginContainer;

/** The PDFEdit representation of the closing bracket of a marked-content sequence.
	Elements of this type must be paired with elements of type PDEBeginContainer.
	@see PDEElement (superclass)
	@see PDEEndContainerCreate
	@see PDERelease
*/
typedef struct _t_PDEEndContainer *PDEEndContainer;

/** A group of PDEElement objects on a page in a PDF file.
	@see PDEElement (superclass)
	@see PDEBeginGroupCreate
	@see PDERelease
*/
typedef struct _t_PDEBeginGroup *PDEBeginGroup ;

/** A group of PDEElement objects on a page in a PDF file.
	@see PDEElement (superclass)
	@see PDEEndGroupCreate
	@see PDERelease
*/
typedef struct _t_PDEEndGroup *PDEEndGroup;
 

/* This typedef appears in both PDExpT.h and PEExpT.h now, so avoid multiple typedef's here */
#ifndef _T_PDEFONT_
#define _T_PDEFONT_

/** A reference to a font used on a page in a PDF file. It may be equated with a font in the
	system. A PDEFont is not the same as a PDFont; a PDFont is associated with a
	particular document.
	@see PDEFontCreate
	@see PDEFontCreateFromCosObj
	@see PDEFontCreateFromSysFont
	@see PDEFontCreateFromSysFontEx
	@see PDEFontCreateWithParams
	@see PDETextGetFont
	@see PDERelease
*/
typedef struct _t_PDEFont *PDEFont;
#endif


/** A reference to a color space used on a page in a PDF file. The color space is part of
	the graphics state attributes of a PDEElement.
	@see PDEColorSpaceCreate
	@see PDEColorSpaceCreateFromName
	@see PDEImageGetColorSpace
	@see PDERelease
*/
typedef struct _t_PDEColorSpace *PDEColorSpace;


/** A list of PDEElement objects containing a list of PDEPath objects and PDEText objects that describe a
	clip state. PDEClip objects can be created and built up with PDEClip methods. Any
	PDEElement object can have PDEClip associated with it.
	PDEClip objects can contain PDEContainer objects and PDEGroup objects to an arbitrary level
	of nesting. This allows PDEContainer objects to be used to mark clip objects.
	PDEGroup objects inside PDEClip objects that contain at least one PDEText and no PDEPath objects
	have a special meaning. All PDEText objects contained in such a PDEGroup are
	considered to be part of the same BT/ET block. This means that the union of these
	PDEText objects makes up a single clipping path, as opposed to the intersection of the
	PDEText objects.
	@see PDEClipCreate
	@see PDEElementGetClip
	@see PDERelease
	@see PDEClipFlattenedEnumElems
*/
typedef struct _t_PDEClip *PDEClip;


/** A reference to an ExtGState resource used on a page in a PDF file. It specifies a
	PDEElement object's extended graphics state, which is part of its graphics state.
	@see PDEExtGStateCreate
	@see PDERelease
*/
typedef struct _t_PDEExtGState *PDEExtGState;


/** A reference to a Pattern resource used on a page in a PDF file.
	@see PDEPatternCreate
	@see PDERelease
	@see PDEPatternGetCosObj
*/
typedef struct _t_PDEPattern *PDEPattern;


/** A color space with a variable number of device-dependent components. It is usually used
	to store multiple spot colors in a single color space.
	@see PDEDeviceNColorsCreate
*/
typedef struct _t_PDEDeviceNColors *PDEDeviceNColors;


/** A reference to the state of a reader.
*/
typedef struct _t_PDEState *PDEState;


/** A reference to the state of a writer.
*/
typedef struct _t_PDEEmitState *PDEEmitStateP;


/** An object for creating and manipulating a soft mask in a PDF file.
	@see PDEElement (superclass)
	@see PDESoftMaskCreate
	@see PDESoftMaskCreateFromCosObj
	@see PDESoftMaskCreateFromName
	@see PDERelease
*/
typedef struct _t_PDESoftMask *PDESoftMask;


/** A transparency (XGroup) resource.
	@see PDEElement (superclass)
	@see PDEXGroupCreate
	@see PDEXGroupCreateFromCosObj
	@see PDERelease
*/
typedef struct _t_PDEXGroup *PDEXGroup;


/** An object used to read streams of PDEElement objects from page contents.
*/
typedef struct _t_PDEReader *PDEReader;


/** An object used to write streams of PDEElement objects to page content.
*/
typedef struct _t_PDEWriter *PDEWriter;


/** A PDEElement that provides system encoding for a PDF file.
	@see PDEElement (superclass)
	@see PDSysEncodingCreateFromBaseName
	@see PDSysEncodingCreateFromCMapName
	@see PDERelease
*/
typedef struct _t_PDSysEncoding *PDSysEncoding;


/** A reference to a PDEDoc.
*/
typedef struct _t_PDEDoc *PDEDoc;


/** A reference to a PDEPage.
*/
typedef struct _t_PDEPage *PDEPage;


/** A reference to a PDETextItem.
*/
typedef struct _t_PDETextItem *PDETextItem;

/** A reference to a PDEImageFlate.
*/
typedef struct _t_PDEImageFlate *PDEImageFlate;


/** A reference to a PDEImageJPX.
*/
typedef struct _t_PDEImageJPX *PDEImageJPX;


/** A reference to a JPXColorSpace.
*/
typedef struct _t_JPXColorSpace *JPXColorSpace;


/** A reference to a JPXPalette.
*/
typedef struct _t_JPXPalette *JPXPalette;

/** A reference to a PDESVGContent
*/
typedef struct _t_PDESVGContent *PDESVGContent;

/*------------------------------------------------------------------------
	PDFEdit Attributes.
------------------------------------------------------------------------*/


/**
	A structure describing attributes of a PDEContent object. 
	
	@see PDEContentGetAttrs 
	@see PDEContentToCosObj 
*/
typedef struct _t_PDEContentAttrs {

	/** PDEContentFlags */
	ASUns32 flags;						
	
	/* CharProc attributes */

	/** If flags has kPDESetCacheDevice set, the first six cache
		device values contain the operands for the d1 (<code>setcachedevice</code>)
		page operator. If flags has kPDESetCacheDevice set,
		<code>cacheDevice</code> contains two <code>charwidth</code> values. 
	*/
	ASReal cacheDevice[8];				
	
	/* Form attributes */

	/** Only used if PDEContent contains a Form XObject.
		It corresponds to FormType key in the XObject Form attributes
		dictionary. 
	*/
	ASInt32			formType;

	/** Only used if PDEContent contains a Form. It is the bounding box of the
		PDEContent object. It corresponds to the BBox key in the XObject
		Form attributes dictionary. 
	*/
	ASFixedRect		bbox;

	/** Only used if PDEContent contains a Form. It is the transformation
		matrix for the PDEContent object. It corresponds to the Matrix key in
		the XObject Form attributes dictionary. 
	*/
	ASFixedMatrix	matrix;

	/** Only used if PDEContent contains a Form. It is the form's XUID, which is an
		ID that uniquely identifies the form. It corresponds to the XUID key in
		the XObject Form attributes dictionary. 
	*/
	CosObj			XUID;
	} PDEContentAttrs, *PDEContentAttrsP;
	

/**
	A structure describing a color value. 
*/
typedef struct _t_PDEColorValue {

	/** Color value components. For example, a Gray color space has one
		component, an RGB color space has three components, a CMYK
		has four components, and so on. Use <code>color[0]</code> for the Separation color space.
	*/
	ASReal			color[7];			

	/** Used if the color space is DeviceN. */
	PDEObject		colorObj2;			

	/** Used for color spaces whose color values do not have numeric
		values, such as the Pattern color space.
	*/
	PDEObject		colorObj;			
	} PDEColorValue, *PDEColorValueP;
	

/**
	A structure describing a color specification. 
	 
*/ 
typedef struct _t_PDEColorSpec {

	/** The specified PDEColorSpace. */
	PDEColorSpace	space;

	/** The color value. */
	PDEColorValue	value;
	} PDEColorSpec, *PDEColorSpecP;
	

/**
	A structure describing a dash specification, as described 
	in Table 4.8 in the <i>PDF Reference</i>. See Section 4.3.2 for 
	more information on line dash patterns. 
	 
*/
typedef struct _t_PDEDash {

	/** The dash phase. The phase is a number that specifies a distance in user
		space into the dash pattern at which to begin marking the path. 
	*/
	ASReal			dashPhase;

	/** The number of entries in the dash array, which is an element of the Border array. */
	ASInt32			dashLen;

	/** The dash array, which specifies distances in user space for the
		lengths of dashes and gaps. 
	*/
	ASReal			dashes[11];
	} PDEDash, *PDEDashP;


/** A structure describing the graphics state that was set.
	@see PDEDefaultGState
	@see PDETextAdd
*/
typedef enum {

	/**  A fill color space was set corresponding to the cs (<code>setcolorspace</code>) operator. */
	kPDEFillCSpaceWasSet 	= 0x0001,

	/** A color fill value was set corresponding to the sc (<code>setcolor</code>) operator. */
	kPDEFillCValueWasSet 	= 0x0002,

	/** A color space stroke value was set corresponding to the CS (<code>setcolorspace</code>) operator. */
	kPDEStrokeCSpaceWasSet	= 0x0004,

	/** A color stroke value was set corresponding to the SC (<code>setcolor</code>) operator. */
	kPDEStrokeCValueWasSet	= 0x0008,

	/** A dash specification was set corresponding to the d (<code>setdash</code>) operator. */
	kPDEDashWasSet			= 0x0010,

	/** The line width was set corresponding to the w (<code>setlinewidth</code>) operator. */
	kPDELineWidthWasSet		= 0x0020,

	/** The miter limit was set corresponding to the M (<code>setmiterlimit</code>) operator. */
	kPDEMiterLimitWasSet	= 0x0040,

	/** Line flatness was set corresponding to the i (<code>setflat</code>) operator. */
	kPDEFlatnessWasSet		= 0x0080,

	/** Line cap style was set corresponding to the J (<code>setlinecap</code>) operator. */
	kPDELineCapWasSet		= 0x0100,

	/** Line join style was set corresponding to the j (<code>setlinejoin</code>) operator. */
	kPDELineJoinWasSet		= 0x0200,

	/** A color rendering intent was set corresponding to the Intent key in the image dictionary. */
	kPDERenderIntentWasSet	= 0x0400,

	/** An extended graphics state was set corresponding to the gs operator. */
	kPDEExtGStateWasSet		= 0x0800,

	/** The soft mask matrix has been set */
	kPDESoftMaskMatrixWasSet = 0x1000 /* New in PDFLib 8.0 */
} PDEGraphicStateWasSetFlags;


/** A structure describing the text state that was set.
	@see PDETextAdd
	@see PDETextGetTextState
	@see PDETextRunSetTextState
*/
typedef enum {

	/** Character spacing was set corresponding to the Tc operator. */
	kPDECharSpacingWasSet	= 0x0001,

	/** Word spacing was set corresponding to the Tw operator. */
	kPDEWordSpacingWasSet	= 0x0002,

	/** Text rendering mode was set corresponding to the Tr operator. */
	kPDERenderModeWasSet	= 0x0004,

	/** TBD */
	kPDEFontSizeWasSet		= 0x0008,	/* new in PDFLib 5.0 */

	/** TBD */
	kPDEHScaleWasSet		= 0x0010,	/* new in PDFLib 5.0 */

	/** TBD */
	kPDETextRiseWasSet		= 0x0020	/* new in PDFLib 5.0 */
} PDETextStateWasSetFlags;

#define kPDFStateSetAll	((ASUns32) -1)
	

/**
	Attributes of a PDEElement or a PDEText sub-element. 
	@see PDEDefaultGState 
	@see PDETextAdd 
*/
typedef struct _t_PDEGraphicState {

	/** PDEGraphicStateWasSetFlags indicating if an attribute has been set.
		@note Support for these flags is not complete. For compatibility,
		you should set them, but do not depend on reading their
		values back. The intended use with XObject Forms is to
		indicate whether the value is inherited or explicitly set. 
	*/
	ASUns32			wasSetFlags;	

	/** The fill color specification. 
		The default value is DeviceGray, fixedZero. 
	*/
	PDEColorSpec	fillColorSpec;

	/** The stroke color specification. 
		The default value is DeviceGray, fixedZero. 
	*/
	PDEColorSpec	strokeColorSpec;

	/** The dash specification. 
		The default value is <code>[0, 0]</code>. 
	*/
	PDEDash			dash;

	/** The line width corresponding to the w (<code>setlinewidth</code>) operator. 
		The default value is <code>fixedOne</code>. 
	*/
	ASReal			lineWidth;

	/** The miter limit corresponding to the M (<code>setmiterlimit</code>) operator. 
		The default value is fixedTen. 
	*/
	ASReal			miterLimit;

	/** The line flatness corresponding to the i (<code>setflat</code>) operator. 
		The default value is fixedZero. 
	*/
	ASReal			flatness;

	/** The line cap style corresponding to the J (<code>setlinecap</code>) operator. 
		The default value is <code>0</code>. 
	*/
	ASInt32			lineCap;

	/** Line join style corresponding to the j (<code>setlinejoin</code>) operator. 
		The default value is <code>0</code>. 
	*/
	ASInt32			lineJoin;

	/** A color rendering intent corresponding to the Intent key in the image dictionary. 
		The default value is <code>0</code>. 
	*/
	ASAtom			renderIntent;

	/** An extended graphics flag corresponding to the <code>gs</code> operator. 
		The default value is CosObj, <code>NULL</code>. 
	*/
	PDEExtGState	extGState;

	/** The CTM at the time soft mask was established.
 		The default value is identity matrix.
	*/
	ASFixedMatrix	softMaskMatrix;
	} PDEGraphicState, *PDEGraphicStateP;

/* Attributes of a PDEText subelement */
/* PDETextGetTextState and PDETextRunSetTextState can handle up to renderMode for backward
   compatibility with PDFLib 4.0 or below.
   Use PDETextGetState and PDETextRunSetState for new PETextState fields. */

/**
	Attributes of a PDEText element. 
	@see PDETextAdd 
	@see PDETextGetTextState 
	@see PDETextRunSetTextState 
*/
typedef struct _t_PDETextState {

	/** PDETextStateWasSetFlags indicates whether an attribute has been set.

		@note Support for these flags is not complete. For compatibility,
		you should set them, but do not depend on reading their
		values back. The intended use with XObject Forms is to
		indicate whether the value is inherited or explicitly set.

		@note PDFEdit ignores the <code>wasSetFlags</code> flag, so you must
		initialize the PDETextState fields. 
	*/
	ASUns32			wasSetFlags;	

	/** Character spacing was set corresponding to the Tc operator. */
	ASReal			charSpacing;

	/** Word spacing corresponding to the Tw operator. */
	ASReal			wordSpacing;

	/** Text rendering mode corresponding to the Tr operator. */
	ASInt32			renderMode;

	/** Default is <code>1</code>. */
	ASReal			fontSize;			/* new in PDFLib 5.0. Default = 1 */

	/** Default is <code>100</code> (meaning 100%). */
	ASReal			hScale;				/* new in PDFLib 5.0. Default = 100 (== 100%) */

	/** Specifies the distance, in text space units that are not scaled, to
		move the baseline up or down from its default location. See Section 5.2.6 in the <i>PDF Reference</i>. 
	*/
	ASReal			textRise;			/* new in PDFLib 5.0 */
	} PDETextState, *PDETextStateP;


/**
	Attributes of a PDEImage. 
	@see PDEImageCreate 
	@see PDEImageGetAttrs 
	@see PDEImageGetData 
*/
typedef struct _t_PDEImageAttrs {

	/** PDEImageAttrFlags indicating image attributes. */
	ASUns32			flags;		

	/** The width of the image corresponding to the Width key in the image dictionary. */
	ASInt32			width;

	/** The height of the image corresponding to the Height key in the image dictionary. */
	ASInt32			height;

	/** The number of bits used to represent each color component in the
		image corresponding to the BitsPerComponent key in the image dictionary. 
	*/
	ASInt32			bitsPerComponent;

	/** An array of numbers specifying the mapping from sample values
		in the image to values appropriate for the current color space.
		These values correspond to the Decode key in the image dictionary. 
	*/
	ASReal			decode[8];

	/** The color rendering intent corresponding to the Intent key in the image dictionary. */
	ASAtom			intent;
	} PDEImageAttrs, *PDEImageAttrsP, PDEImageFlateAttrs, *PDEImageFlateAttrsP;


/**
	The filter element in a filter array. 
	@see CosNewStream 
	@see PDEContentToCosObj 
	@see PDEImageCreate 
*/
typedef struct _t_PDEFilterSpec {

	/** Parameters used by the decoding filters specified with the Filter
		key. It corresponds to the DecodeParms key in the stream
		dictionary.

		<p>It must be set to <code>NULL</code> if PDEFilterSpec is specified but no
		encode or decode parameters are specified. This can be done
		by passing CosNewNull for the unused encode and/or decode
		parameters.</p>

		<p>The required decode parameters for DCTDecode are <code>Columns</code>, <code>Rows</code>,
		and <code>Colors</code>. The parameters should be passed in a CosDict. </p>
	*/
	CosObj	decodeParms;

	/** Parameters used when encoding the stream. It is required for
		the DCTDecode filter, but is optional for other filters.

		<p>It must be set to <code>NULL</code> if PDEFilterSpec is specified but no
		encode or decode parameters are specified. This can be done
		by passing CosNewNull for the unused encode and/or decode parameters. </p>
	*/
	CosObj	encodeParms;

	/** The filter name. The supported filters are: 
		<ul>
		<li>ASCIIHexDecode</li>
		<li>ASCII85Decode</li>
		<li>LZWDecode</li> 
		<li>DCTDecode</li>
		<li>CCITTFaxDecode</li> 
		<li>RunLengthDecode</li>
		<li>FlateDecode</li>
		</ul>
	*/
	ASAtom	name;

	/** Reserved - used to align to 32 bits. */
	ASInt16	padding;			
	} PDEFilterSpec, *PDEFilterSpecP;
	

/** Filter information for streams.
	
	<p>Although the PDEFilterSpec is declared as a one member array in the header file,
	more members can be added by dynamically allocating the PDEFilterArray with
	space for as many filters as you would like to add.
	In practice, there is seldom need for more than two filters.</p>

	@see PDEContentToCosObj
	@see PDEImageCreate
*/
typedef struct _t_PDEFilterArray {

	/** The number of filters in the array. */
	ASInt32 numFilters;

	/** A variable length array of filter specifications. */
	PDEFilterSpec spec[1];	
	} PDEFilterArray, *PDEFilterArrayP;
	

/**
	Attributes of a PDEPS object. 
	@see PDEPSCreate 
	@see PDEPSGetAttrs 
*/
typedef struct _t_PDEPSAttrs {

	/** kPDEPSInline */
	ASUns32	flags;			
	} PDEPSAttrs, *PDEPSAttrsP;


/**
	Attributes of a PDEFont and of a PDSysFont. This structure 
	is also referenced in PDEFontCreateParams().
	@see PDEFontCreate 
	@see PDEFontCreateWithParams 
	@see PDEFontGetAttrs 
	@see PDFindSysFont 
	@see PDFindSysFontEx 
	@see PDSysFontGetAttrs 
	@see PDSysFontGetEncoding 
	@see PDSysFontAcquirePlatformData 
*/	
typedef struct _t_PDEFontAttrs {	

	/** An ASAtom for font name (for example, <code>"Times-Roman"</code>). It corresponds to
		the BaseFont key in the font dictionary of a PDF file (see
		Section 5.6.3 in the <i>PDF Reference</i>). 
	*/
	ASAtom	name;

	/** An ASAtom for the font type corresponding to the Subtype key in a
		a font dictionary. It may be:
		<ul>
		<li><code>"Type1"</code></li>
		<li><code>"TrueType"</code></li>
		<li><code>"MMType1"</code></li>
		<li><code>"Type0"</code></li>
		</ul>
	*/
	ASAtom	type;

	/** An ASAtom for <code>"Roman"</code>, or ASAtomNull. If it is <code>"Roman"</code>, the
		characters must be a subset of the Adobe Standard Roman Character Set. 
	*/
	ASAtom	charSet;

	/** An ASAtom for font encoding. It may be <code>MacRomanEncoding</code>,
		<code>WinAnsiEncoding</code>, or ASAtomNull. In the case of
		ASAtomNull, call PDSysFontGetEncoding() to get more
		information about the encoding. 
	*/
	ASAtom	encoding;
	
	/* Almost the same as PDFontMetrics in Acrobat 2.0 */

	/** The desired font flags; one or more of the Font Flags. */
	ASUns32		flags;				/* Use PD_SCRIPT, etc. to get flags	*/

	/** The font bounding box in 1000 EM units.	*/
	ASFixedRect	fontBBox;			

	/** The width of the missing character (<code>.notdef</code>). */
	ASInt16  	missingWidth;		

	/** The vertical stem width. */
	ASInt16  	stemV;				

	/** The horizontal stem width. */
	ASInt16  	stemH;				

	/** The capital height. */
	ASInt16  	capHeight;			

	/** The x height. */
	ASInt16  	xHeight;			

	/** The maximum ascender height. */
	ASInt16  	ascent;				

	/** The maximum descender depth. */
	ASInt16  	descent;			

	/** The additional leading between lines. */
	ASInt16  	leading;			

	/** The maximum character width. */
	ASInt16  	maxWidth;			

	/** The average character width. */
	ASInt16  	avgWidth;			

	/** The italic angle in degrees, if any.	*/
	ASInt16  	italicAngle;		

	/** CIDFontType0 or CIDFontType2. */
	ASAtom		cidFontType;		

	/** The writing mode. It must be one of:

		<TABLE rules="all" cellspacing="1">
		<TR><TH>Value</TH><TH>Description</TH></TR>
		<TR><TD><code>0</code></TD><TD>For horizontal writing.</TD></TR>
		<TR><TD><code>1</code></TD><TD>For vertical writing.</TD></TR>
		</TABLE>
	 */
	ASInt16		wMode;			

	/** An ASAtom representing the PostScript name of a TrueType font. */
	ASAtom		psName;			

	/** The platform name.  */
	ASAtom		platformName;	

	/** An ASAtom representing the ISO 639 language code. These are available from http://www.iso.ch. */
	ASAtom		lang;			

	/** An ASAtom representing the CIDFont's Registry information (for example, <code>"gAdobe-Japan"</code>. */
	ASAtom		registry;		

	/** An ASAtom representing the CIDFont's Ordering information (for example, <code>"g1"</code>). */
	ASAtom		ordering;

	/** The SystemSupplement field from the CIDFont. */
	ASInt32		supplement;

	/** A non-zero value means the font can't be embedded. */
	ASInt32		cantEmbed;

	/** The name of the base encoding; that is, the BaseEncoding entry
		in an encoding dictionary (see section 5.5.5 of the <i>PDF
		Reference</i>). The Differences entry of the encoding dictionary
		describes differences (deltas) from the base encoding.	 
	*/
	ASAtom		deltaEncoding;		

	/** Allows one of the following bits to be set in order to disable font embedding:

		<TABLE rules="all" cellspacing="1">
		<TR><TH>Bit</TH><TH>Description</TH></TR>
		<TR><TD><code>kPDEFontNoEmbedding = 1</code></TD><TD>The font should not be embedded.</TD></TR>
		<TR><TD><code>kPDEFontNoEditableEmbedding = 2</code></TD><TD>The font should not be embedded for editing purposes.</TD></TR>
		</TABLE>

	*/
	ASUns32		protection;		

	/** PDSysFontPackageType */
	ASInt32		packageType;	
	} PDEFontAttrs, *PDEFontAttrsP;
	

/**
	Parameters used for PDEFontCreateWithParams() to describe 
	a font. 
	@see PDEFontCreateWithParams 
*/
typedef struct _t_PDEFontCreateParams {

	/** A pointer to a PDEFontAttrs for the font attributes. */
	PDEFontAttrsP attrsP;

	/** The size of the data structure. It must be set to <code>sizeof(PDEFontAttrs)</code>. */
	ASUns32 attrsSize;

	/** The first character index for the widths array, <code>widthsP</code>. */
	ASInt32 firstChar;

	/** The last character index for the widths array, <code>widthsP</code>. */
	ASInt32 lastChar;

	/** A pointer to the widths array. */
	ASInt16 *widthsP; 

	/** An array of 256 pointers to glyph names specifying the custom
		encoding. If any pointer is <code>NULL</code>, no encoding information is written for that entry. 
	*/
	char **encoding;

	/** An ASAtom representing the encoding base name if the
		encoding is a custom encoding. If the encoding is <code>NULL</code>,
		<code>encodingBaseName</code> is used as the value of the encoding and
		must be one of <code>"WinAnsiEncoding"</code>, <code>"MacRomanEncoding"</code>, or
		<code>"MacExpertEncoding"</code>. If no encoding value is desired, use
		ASAtomNull. However, for Type 0 fonts, this field must be a
		valid CMap name, or PDEFontCreateWithParams() will fail. 
	*/
	ASAtom encodingBaseName;

	/** A stream with font information. */
	ASStm fontStm;

	/** The length in bytes of the ASCII portion of the Type 1 font file after it
		has been decoded. For other font formats, such as TrueType or
		CFF, only <code>len1</code> is used, and it is the size of the font. 
	*/
	ASInt32 len1;

	/** The length in bytes of the encrypted portion of the Type 1 font file after it has been decoded. */
	ASInt32 len2;

	/** The length in bytes of the portion of the Type 1 font file that contains
		the 512 zeros, plus the <code>cleartomark</code> operator, plus any following data. 
	*/
	ASInt32 len3;

	/** If <code>true</code>, the <code>dw</code> and <code>w</code> fields are used; if <code>false</code>, they are not used. */
	ASBool hasDW;

	/** (Optional) The default width for glyphs in a CIDFont. See
		Section 5.6.3 in the <i>PDF Reference</i> for more information. 
	*/
	ASInt32 dw;

	/** A Cos array of a set of lists that define the widths for the glyphs
		in the CIDFont. Each list can specify individual widths for
		consecutive CIDs, or one width for a range of CIDs. See
		Section 5.6.3 on character widths in CIDFonts in the <i>PDF
		Reference</i> for information on the format of this array. 
	*/
	CosObj w;

	/** If <code>true</code>, the <code>dw2</code> and <code>w2</code> fields are used; if <code>false</code>, they are not used. */
	ASBool hasDW2;

	/** (Optional: this applies only to CIDFonts that are used for vertical writing). 
		
		<p>The default metric for writing mode 1. This entry is an
		array of two numbers: the y-component of the position vector
		and the y component of the displacement vector for writing
		mode 1. The x-component of the position vector is always half
		the width of the character. The x-component of the displacement
		vector is always <code>0</code>. The default value is <code>[880-1000]</code>. For
		information on writing mode 1, see Section 5.6.3 on vertical
		writing in the <i>PDF Reference</i>.</p> 
	*/
	ASInt32 dw2[2];

	/** (Optional: this applies only to CIDFonts that are used for vertical writing) 
	
		<p>A Cos array defining the metrics for vertical writing. Its
		format is similar to the format of the array in <code>w</code>. It defines the x
		and y components of the position vector, and the y-component
		of the displacement vector. The x-component of the
		displacement vector is always <code>0</code>. See Section 5.6.3 on character
		widths in CIDFonts in the <i>PDF Reference</i> for information on the
		format of this array. </p>
	*/
	CosObj w2;

	/** (Optional) The length of <code>toUnicodeStm</code>. */
	ASInt32 toUnicodeLen;

	/** (Optional) A stream containing a CMap that defines the mapping
		from character codes to Unicode values. This entry is
		recommended for fonts that do not use one of the predefined
		CMaps. If present, this allows strings in the encoding to convert
		to Unicode values for export to other applications or plug-ins.
		For more information, see Section 5.6 on Type 0 fonts in the
		<i>PDF Reference</i>. 
	*/
	ASStm toUnicodeStm;

	/** A stream contain the mapping from a CID to <code>glyphindex</code> (<code>"GID"</code>).
		The <code>glyphindex</code> for a particular CID value <code>c</code> is a 2-byte value
		stored in bytes <code>2*c</code> and <code>2*c+1</code>; the first byte is the high-order byte. 
	*/
	ASStm cidToGidMapStm;

	/** A 12-byte string containing the Family Class ID, Family
		SubClass ID, and 10 bytes for the PANOSE classification number
		for the font. For additional details on the PANOSE number, see the 
		<i>Japanese TrueType Font Property Selection Guidelines</i> by the
		TrueType Conference Technical Committee. 
	*/
	char *panoseNo;	

	/** A Cos dictionary identifying a subset of characters in a CIDFont.
		The values are dictionaries with entries that override the values
		in the FontDescriptor dictionary for the subset of characters.
		See Section 5.6 in the <i>PDF Reference</i> for more information. 
	*/
	CosObj fd;

	/** A stream identifying which CIDs are present in the CIDFont file.
		It is required if the CIDFont file is embedded and only contains a
		subset of the glyphs in the character collection defined by the
		CIDSystemInfo. If this entry is missing, then it is assumed that
		the CIDFont file contains all the glyphs for the character
		collection. The stream's length should be rounded up to the
		nearest multiple of <code>8</code>. The bits should be stored in bytes with the
		high-order bit first. Each bit corresponds to a CID. The first bit of
		the first byte corresponds to CID 0, the next bit corresponds to
		CID 1, and so on. If the subset contains a CID, the bit for that
		CID should be set. For compactness, the stream may use one of
		the compression filters to encode the data. For more
		information, see Section 5.6 in the <i>PDF Reference</i>. 
	*/
	ASStm cidSetStm;

	/** One of the PDEFontCreateFlags describing how to embed
		and subset the font:
		
		<ul>
		<li>kPDEFontCreateEmbedded</li> 
		<li>kPDEFontWillSubset</li>
		<li>kPDEFontDoNotEmbed </li>
		</ul>
	*/
	ASUns32 flags;	/* 	*/

	/** A pointer to the Multiple Master font design vector. */
	ASReal *mmDesignVec;

	/**  */
	ASAtom sourceFontType;	/* K2 */
	} PDEFontCreateParamsRec, *PDEFontCreateParams;


/**
	A data structure used with PDEFont creation. 
	@see PDEFontCreateFromSysFontWithParams 
*/
typedef struct _t_PDEFontCreateFromSysFontParams {

	/** The size of the data structure. It must be set to <code>sizeof(PDEFontCreateFromSysFontParamsRec)</code>. */
	ASUns32		structSize;

	/** A bit mask of the PDEFontCreateFlags. */
	ASUns32		flags;

	/** The name of a Multiple Master snapshot. See the <i>PDF Reference</i>
		for more information on snapshots. 
	*/
	ASAtom		snapshotName;

	/** A pointer to the Multiple Master font design vector. */
	ASReal		*mmDesignVec;

	/** Used to select a specific code page supported by the font. When
		a non-zero code page is supplied, embedding must be turned
		on and the kPDEFontEncodeByGID flag must be set. 
	*/
	long		ctCodePage;

	/** Used to specify which encoding to use with a CID font. Pass
		ASAtomNull to use the platform default.	 
	*/
	ASAtom		encoding;

	/** Used to specify the CosDoc in which to create the font. Pass
		<code>NULL</code> to use the scratch doc.	 
	*/
	CosDoc		cosDoc;

	} PDEFontCreateFromSysFontParamsRec, *PDEFontCreateFromSysFontParams;

/**
	A data structure used for adding Unicode text.
	@see PDETextAddGlyphs
*/
typedef struct _t_PDEGlyphDescription {

	/** The GlyphID of a glyph being added. */
	ASUns32			glyphID;

	/** An array of indices to Unicode characters, from the <code>uniText</code> member
		of the PDEGlyphRun <code>struct</code>, represented by this glyph.
	*/
	ASInt16			*charIndices;

	/** The count of the indices in the <code>charIndices</code> array. */
	ASInt32			charIndicesLen;

	/** The x position of where to place this glyph. */
	ASReal			xPosition;

	/** The y position of where to place this glyph. */
	ASReal			yPosition;
	} PDEGlyphDescription, *PDEGlyphDescriptionP;

/**
	A data structure used for adding Unicode text.
	@see PDETextAddGlyphs
*/
typedef struct _t_PDEGlyphRun {

	/** A pointer to the array of PDEGlyphDescription items. */
	PDEGlyphDescriptionP	glyphs;

	/** The count of items in the PDEGlyphDescription array. */
	ASInt32			glyphLen;

	/** An array of Unicode characters in UTF32HE format. */
	ASUTF32Val		*uniText;

	/** The count of characters in uniText array. */
	ASInt32			uniTextLen;
	} PDEGlyphRun, *PDEGlyphRunP;

/* Utility structures for passing around ActualText information
   related to the new PDETextAddGlyphs API. */

typedef struct _t_PDESpanItem {

	/** The index into the glyphs array in a PDEGlyphRun object. */
	ASInt32			firstGlyph;

	/** The number of glyphs in the span starting from firstGlyph.
		This can be zero, indicating there are no glyphs for
		this Unicode string. The firstGlyph indicates the
		glyph that precedes the Unicode string.
	*/
	ASInt32			nGlyphs;

	/** The index into the <code>uniText</code> array in a PDEGlyphRun object. */
	ASInt32			firstUniChar;

	/** The number of Unicode characters in the span starting from
		<code>firstUniChar</code>. This count can be zero, indicating there
		are no Unicode values for the range of glyphs.
	*/
	ASInt32			nUniChars;
} PDESpanItem, *PDESpanItemP;

typedef struct _t_PDESpanSet {

	/** A pointer to array of spans corresponding to the PDEGlyphRun
		object.
	*/
	PDESpanItemP		spans;

	/** The count of spans in the <code>spans</code> array. */
	ASInt32			spanCount;
} PDESpanSet, *PDESpanSetP;

/*------------------------------------------------------------------------
	PDFEdit Flags and Enums.
------------------------------------------------------------------------*/


/** The types of PDEObject, which is the superclass for PDEContent, PDEElement,
	PDEClip, and so on.
	@see PDEObjectGetType
*/
typedef enum {

	/** PDEContent object */
	kPDEContent,
	

	/** PDEText object */
	kPDEText,

	/** PDEPath object */
	kPDEPath,

	/** PDEImage object */
	kPDEImage,

	/** PDEForm object */
	kPDEForm,

	/** PDEPS object */
	kPDEPS,

	/** PDEXObject object */
	kPDEXObject,
	

	/** PDEClip object */
	kPDEClip,
	

	/** PDEFont object */
	kPDEFont,

	/** PDEColorSpace object */
	kPDEColorSpace,

	/** PDEExtGState object */
	kPDEExtGState,


	/** PDEPlace object */
	kPDEPlace,

	/** PDEContainer object */
	kPDEContainer,


	/** PDESysFont object */
	kPDSysFont,
	

	/** PDEPattern object */
	kPDEPattern,

	/** PDEDevineNColors object */
	kPDEDeviceNColors,


	/** PDEShading object */
	kPDEShading,

	/** PDEGroup object */
 	kPDEGroup,


	/** PDEUnknown object */
	kPDEUnknown,


	/** PDEBeginContainer object */
	kPDEBeginContainer,

	/** PDEEndContainer object */
	kPDEEndContainer,


	/** PDEBeginGroup object */
	kPDEBeginGroup,

	/** PDEEndGroup object */
	kPDEEndGroup,
	

	/** PDEXGroup object */
	kPDEXGroup,

	/** PDESoftMask object */
	kPDESoftMask,


	/** PDSysEcoding object */
	kPDSysEncoding,


	/** PDEDoc object */
	kPDEDoc,

	/** PDEPage object */
	kPDEPage,


	/** PDEReader object */
	kPDEReader,

	/** PDEWriter object */
	kPDEWriter,


	/** PDETextItem object */
	kPDETextItem,


	/** PDEImageFlate object */
	kPDEImageFlate,


	/** PDEImageJPX object */
	kPDEImageJPX,


	/** JPXColorSpace object */
	kJPXColorSpace,


	/** JPXPalette object */
	kJPXPalette,
	
	/** PDESVGContent object */
	kPDESVGContent,
	
	/** */
	kPDELastType
} PDEType;

/* Handy defines for inserting at the beginning and end of lists */
#define kPDEBeforeFirst	((ASInt32) -1)
#define kPDEAfterLast	(MAXInt32 - 1)

/** A bit field for the PDEContentToCosObj() method, indicating the type of object to create
	and how it is created.
	@see PDEContentToCosObj  
*/
typedef enum {

	/** Create page contents. */
	kPDEContentToPage                = 0x0001,

	/** Create a form. */
	kPDEContentToForm                = 0x0002,

	/** Create charprocs. */
	kPDEContentToCharProc            = 0x0004,

	/** Currently unused. */
	kPDEContentRev1Compat            = 0x0008,

	/** Currently unused. */
	kPDEContentDoNotResolveForms     = 0x0010,

	/** Currently unused. */
	kPDEContentDoNotResolveType3     = 0x0020,

	/** Emit calibrated RGB and gray information using the PDF 1.0
		compatible mechanism. In this case, generate rg and k page operators and
		place DefaultGray and DefaultRGB color space arrays in the Resources
		dictionary, as described in Section 4.5.7 in the <i>PDF Reference</i>. 
	*/
	kPDEContentEmitDefaultRGBAndGray = 0x0040,

	/**  */
	kPDEContentInheritState          = 0x0080,
	
	/** Prevents the emission of the content compatibility operators, BX/EX, which
        cause issues for some versions of PDF/A or PDF/X.
    */
	kPDEContentDoNotEmitBXEX         = 0x0100,

	kPDEContentUseMaxPrecision		 = 0x0200,

	/** Use a space character as the EOL character in the content stream to make
	    the Flate compressor more effective.
	*/
	kPDEContentUseSpaceAsEOL		 = 0x0400,

	/** Emit a gstate or textstate parameter for any element, only if the corresponding WasSetFlag is set.
     */
    kPDEContentHonorWasSetFlags	 = 0x0800,
    
    /** Setting this flag will skip optimization of bounding box of form XObject.
     */
    kPDEContentSkipBBox = 0x1000,

	/** Setting this flag will skip reseting of ResTable in PDEContent
	and it also skips creating new resource dictionary in document and uses resource dictionary from input .
	*/
	kPDEContentSkipResReset = 0x2000


	} PDEContentToCosObjFlags;
	

/** A bit field for the PDEEnumElements() method.	
	@see PDEEnumElements
*/
typedef enum {

	/** Indicates whether Marked Content is ignored in the enumeration. This may
		be useful when generating elements purely for display purposes. 
	*/
	kPDEContentIgnoreMarkedContent = 0x0001
} PDEEnumElementsFlags;


/** A bit field for PDEContentAttrs.
	@see PDEContentGetResources
*/
typedef enum {

	/** Obtain font resources. */
	kPDEGetFonts,

	/** Obtain Xobject resources. */
	kPDEGetXObjects, 

	/**
		Obtain color space resources.
	*/
	kPDEGetColorSpaces
	} PDEContentGetResourceFlags;
	

/** A bit field for PDEContentAttrs.
	@see PDEContentGetAttrs
	@see PDEContentToCosObj
*/
typedef enum {

	/** If set, <code>cacheDevice</code> contains 6 cache device values. */
	kPDESetCacheDevice	= 0x0001,	

	/** If set, <code>cacheDevice</code> contains 2 charwidth values. */
	kPDESetCharWidth	= 0x0002,	

	/** If set, <code>formMatrix</code> contains a valid matrix. */
	kPDEFormMatrix		= 0x0004	
	} PDEContentFlags;
	

/** A bit field used in PDEText methods.
	@see PDETextAdd
	@see PDETextGetFont
	@see PDETextGetText
*/
typedef enum {

	/** Text run. */
	kPDETextRun = 0x0001, 

	/** Character (text run with only one character). */
	kPDETextChar = 0x0002,

	/** Obtain the advance width in page space. */
	kPDETextPageSpace = 0x0004,

	/** Fill in the left and right bounds of the text run's bounding box. */
	kPDETextGetBounds = 0x0008
	} PDETextFlags;


/** Flags indicating text rendering mode set by the Tr operator.
	@see PDETextCreate
*/	
typedef enum {

	/** Fill text. */ 
	kPDETextFill,

	/** Stroke text. */ 
	kPDETextStroke,

	/** Fill and stroke text. */ 
	kPDETextFillAndStroke,

	/**
		Text with no fill and no stroke (invisible).
	*/ 
	kPDETextInvisible
	} PDETextRenderMode;
	 

/** An enumerated data type for path segment operators in PDEPath elements.
	@see PDEPathAddSegment
	@see PDEPathCreate
	@see PDEPathSetData
*/
typedef enum {

	/** Designates the m (<code>moveto</code>) operator, which moves the current point. */
	kPDEMoveTo,

	/** Designates the l (<code>lineto</code>) operator, which appends a straight line
		segment from the current point. 
	*/
	kPDELineTo,

	/** Designates the c (<code>curveto</code>) operator, which appends a bezier curve
		to the path. 
	*/
	kPDECurveTo,

	/** Designates the v (<code>curveto</code>) operator, which appends a bezier curve
		to the current path when the first control point coincides with
		initial point on the curve. 
	*/
	kPDECurveToV,

	/** Designates the y (<code>curveto</code>) operator, which appends a bezier curve
		to the current path when the second control point coincides with
		final point on the curve. 
	*/
	kPDECurveToY,

	/** Designates the re operator, which adds a rectangle to the current path. */
	kPDERect,

	/** Designates the h (<code>closepath</code>) operator, which closes the current sub-path. */
	kPDEClosePath,

	/** */
	kPDEPathLastType
	} PDEPathElementType;
	

/** Flags for paint operators in a PDEPath.
	@see PDEPathGetPaintOp
	@see PDEPathSetPaintOp
*/
typedef enum {

	/** The path is neither stroked nor filled, so it is invisible. */
	kPDEInvisible			= 0x00,

	/** Stroke the path, as with the S (<code>stroke</code>) operator. */
	kPDEStroke				= 0x01,

	/** Fills the path, using the nonzero winding number rule to
		determine the region to fill, as with the f (<code>fill</code>) operator. 
	*/
	kPDEFill				= 0x02,

	/** Fills the path, using the even/odd rule to determine the region to
		fill, as with the f* (<code>eofill</code>) operator. 
	*/
	kPDEEoFill				= 0x04
	} PDEPathOpFlags;


/** Flags for PDEImageAttrs. See Section 4.8.4 in the <i>PDF Reference</i> for more
	information on image attributes.
	@see PDEImageCreate
	@see PDEImageGetAttrs
	@see PDEImageGetData
*/
typedef enum {

	/** The image is an XObject. */
	kPDEImageExternal		= 0x0001,	

	/** The image is an imagemask. */
	kPDEImageIsMask			= 0x0002,	

	/** <code>interpolate</code> is <code>true</code>. */
	kPDEImageInterpolate	= 0x0004,	

	/** The image has a decode array. */
	kPDEImageHaveDecode		= 0x0008,	

	/** The image uses an indexed color space. */
	kPDEImageIsIndexed		= 0x0010,	

	/** The image has a Mask key containing an ImageMask stream. */
	kPDEImageMaskedByPosition	= 0x0020,	

	/** The image has a Mask key containing an array of color values. */
	kPDEImageMaskedByColor		= 0x0040	
	} PDEImageAttrFlags;


/** Flags for <code>PDEImageGetData()</code>, <code>PDEImageGetDataStm()</code>, <code>PDEImageSetData()</code>, and
	<code>PDEImageSetDataStm()</code>.
	@see PDEImageGetData
	@see PDEImageGetDataStm
	@see PDEImageSetData
	@see PDEImageSetDataStm
	@see PDEImageCreate
*/
typedef enum {

	/** Indicates that the filter is active; data is encoded. */
	kPDEImageEncodedData = 0x0001,
	kPDEImageAllowDelayedRead = 0x0002,
	/** Indicates if the accompanying image data is 16-bit. Should be passed in 
		for 16-bit images to PDEImageGetData/PDEImageGetDataStm to prevent the 
		return of 8-bit converted data. 
	*/
	kPDEImage16bpcData  = 0x0004
	} PDEImageDataFlags;
	

/** Flags to enable <code>PDEImageGetColorSpaceEx()</code> to return a color space 
	with a particular bpc, depending on the image's bpc.
	@see PDEImageGetColorSpaceEx
	@see PDEImageGetColorSpace
*/
typedef enum {

	/** Indicates conversion of the color space of 16 bpc image to 8 bpc. */
	kPDEImageConvert16bpcColorSpace = 0x0001
	} PDEImageColorSpaceFlags;


/** Flags for PDEPSAttrs. */
typedef enum {

	/** PS is an XObject. */
	kPDEPSExternal			= 0x0001	
	} PDEPSFlags;


/** A bit field for <code>PDEElementCopy()</code>. 
	@see PDEElementCopy
*/
typedef enum {

	/** The copied element does not need <code>gstate</code> or <code>clip</code>. */
	kPDEElementCopyForClip = 0x0001,	

	/** Acquire the clip path and put it in the copied object. */
	kPDEElementCopyClipping = 0x0002	
	} PDEElementCopyFlags;


/** Flags for <code>PDEFontCreateFromSysFont()</code>.
	If you want to subset a font, set both the
	<code>kPDEFontCreateEmbedded</code> and <code>kPDEFontWillSubset</code> flags.
	@see PDEFontCreateFromSysFont
	@see PDEFontCreateFromSysFontAndEncoding
	@see PDEFontCreateFromSysFontWithParams
	@see kPDEFontWillSubset
 */
typedef enum {

	/** Embed the font. Create an embedded font. By itself, this will not subset the font.*/
	kPDEFontCreateEmbedded		= 0x0001,		

	/** Subset the font. If you want to subset a font, set both the <code>kPDEFontCreateEmbedded</code>
		and kPDEFontWillSubset flags. You must call <code>PDEFontSubsetNow()</code> to actually subset
		the font. Both embedding and subsetting a font creates a CFF font. 
	*/
	kPDEFontWillSubset			= 0x0002,		

	/** Do not embed the font. You cannot set both
		this and the <code>kPDEFontWillSubset</code> flags.
		Nor can you set <code>kPDEFontCreateEmbedded</code>.
	*/
	kPDEFontDoNotEmbed			= 0x0004,		

	/** Create a CIDFont with identity (GID) encoding. */
	kPDEFontEncodeByGID			= 0x0008,		

	/** Wait to get the widths until later (this affects Type0 fonts only). */
	kPDEFontDeferWidths			= 0x0010,		

	/** */
	kPDEFontCreateSubset		= kPDEFontWillSubset,

	/** PDFLib will convert <code>cp</code> to <code>gid</code> with identity embedded. */
	kPDEFontCreateGIDOverride	= 0x0020,		

	/** Create a ToUnicode CMap. */
	kPDEFontCreateToUnicode		= 0x0040,		

	/** Supply the entire widths table (this affects Type0 fonts only). */
	kPDEFontCreateAllWidths		= 0x0080,		

	/** Embed an OpenType style font subset, if appropriate. */
	kPDEFontCreateEmbedOpenType	= 0x0100,

	/** Reserved for internal usage */
	kPDEFontCreateReserved1		= 0x0200,

	/** Create CIDSet entry from all CIDs present in subsetted Type0 CID Fonts containing
	glyph descriptions based on identity encoded TrueType fonts (subtype CIDFontType2).
	Subsetted identity encoded Type0 fonts can have more glyphs than asked for to avoid
	glyph renumbering. Default behavior is to only include CIDs used in PDF page content
	stream in CIDset */
	kPDEFontCreateFullCIDSet = 0x0400,

	/** If kPDEFontThrowIfToUnicodeUsesPUA flag is passed along with kPDEFontCreateToUnicode
	and kPDEFontCreateSubset, throw error in PDEFont embedding APIs if the created ToUnicode
	table contains Private Use Area (Range: E000-F8FF in plane 0 and Supplemental Private 
	Use Area A & B) Unicode values */
	kPDEFontThrowIfToUnicodeUsesPUA = 0x0800
} PDEFontCreateFlags;


/** Flags for PDEFontGetCreateNeedFlags(). */
typedef enum {

	/** It is necessary to to create the width. */
	kPDEFontCreateNeedWidths	= 0x00010000,	

	/** It is necessary to to create the ToUnicode stream. */
	kPDEFontCreateNeedToUnicode	= 0x00020000,	

	/** It is necessary to to embed it. */
	kPDEFontCreateNeedEmbed		= 0x00040000	
} PDEFontCreateNeedFlags;


/** Flags for protection of PDEFontAttrs: embedding is not allowed. */
#define kPDEFontNoEmbedding				0x00000001	

/** Flags for protection of PDEFontAttrs: editable embedding is not allowed. */
#define	kPDEFontNoEditableEmbedding		0x00000002

/* Flags for <code>packageType</code> of PDEFontAttrs. */
typedef enum {
	kPDSysFontUnknown = 0,
	kPDSysFontType1,
	kPDSysFontTrueType,
	kPDSysFontCID,
	kPDSysFontATC,
	kPDSysFontOCF,
	kPDSysFontOpenTypeCFF,
	kPDSysFontOpenTypeCID,
	kPDSysFontOpenTypeTT
} PDSysFontPackageType;


/** Flags for use with PDESoftMaskCreate().
	@see PDESoftMaskCreate
*/
typedef enum {

	/** Specifies how the mask is to be computed. */
    kPDESoftMaskTypeLuminosity  = 0x0001,

	/** Specifies how the mask is to be computed. */
    kPDESoftMaskTypeAlpha       = 0x0002
	} PDESoftMaskCreateFlags;	


/** An enumerated data type used to specify the type of transparency group to create.
	@see PDESoftMaskCreate
*/
typedef enum {

	/** Creates a transparency XGroup object. */
    kPDEXGroupTypeTransparency  = 0x0001
	} PDEXGroupCreateFlags;	


/**
	A callback for PDEEnumElements(). It is called once for each 
	PDEElement in a page's Contents Stream or Resources dictionary. 
	
	@param elem The PDEElement. 
	@param clientData User-supplied data that was specified 
	in the call to PDEEnumElements().
	@return <code>true</code> to continue enumeration, <code>false</code> to halt enumeration. 
	
	@see PDEEnumElements 
*/
typedef ACCBPROTO1 ASBool (ACCBPROTO2 *PDEElementEnumProc)(
			IN	PDEElement elem,
			IN	void *clientData);

/**
	A callback for PDEClipFlattenedEnumElems(), which enumerates 
	all of a PDEClip object's PDEElement objects in a flattened manner. 
	@param elem IN/OUT The PDEElement currently being enumerated. 
	
	@param clientData IN/OUT User-supplied data that was passed in 
	the call to PDEClipFlattenedEnumElems(). 
	@return If <code>false</code>, enumeration halts. If <code>true</code>, enumeration continues. 
	
	@see PDEClipFlattenedEnumElems 
*/
typedef ACCBPROTO1 ASBool (ACCBPROTO2 *PDEClipEnumProc)(
			IN	PDEElement elem,
			IN	void *clientData);

/*------------------------------------------------------------------------
	PDFEdit Dump.
------------------------------------------------------------------------*/


/**
	A callback for PDELogDump() or PDEObjectDump(). It is called once 
	for each PDEObject, its children, and their attributes for 
	the specified number of levels. 
	@param obj IN/OUT The PDEObject. 
	@param dumpInfo IN/OUT Contains information about an object. Information 
	fields are delimited by tabs. There are no newline characters 
	in this string. 
	@param clientData IN/OUT User-supplied data that was specified 
	in the call to PDELogDump() or PDEObjectDump(). 
	@see PDELogDump 
	@see PDEObjectDump 
*/
typedef ACCBPROTO1 void (ACCBPROTO2 *PDEObjectDumpProc)(
			IN	PDEObject obj,
			IN	const char *dumpInfo,
			IN	void *clientData);

/**
	A callback for PDEAttrEnumTable(). It is called once for each 
	attribute in a table. 
	@param attrHdrP An opaque pointer to the attribute. The 
	actual attribute type is not specified in this function, 
	since the storage mechanism only knows the size of the attribute, 
	not its type. 
	@param refCount The reference count of the attribute. 
	@param size The size of <code>attrHdrP</code> in bytes. 
	@param clientData User-supplied data that was specified 
	in the call to PDEAttrEnumTable().
	@return <code>true</code> to continue enumeration, <code>false</code> to halt enumeration. 
	
	@see PDEAttrEnumTable 
*/
typedef ACCBPROTO1 ASBool (ACCBPROTO2 *PDEAttrEnumProc)(
			IN	void *attrHdrP,
			IN	ASUns32 refCount,
			IN	ASUns16 size,
			IN	void *clientData);
/*------------------------------------------------------------------------
	PDFEdit Colorspace Struct.
------------------------------------------------------------------------*/

/** */
typedef struct {
    float min;
    float max;
} PDEColorRangeFlt;
/** */
typedef struct {
    float x;
    float y;
    float z;
} PDEXYZColorFlt;
/**
	A structure describing a white point in a calibrated color 
	space. 
	@see PDEColorSpaceCreate
*/
typedef PDEXYZColorFlt PDEWhitePointFlt;
/**
	A structure describing a black point in a calibrated color 
	space. 
	@see PDEColorSpaceCreate
*/
typedef PDEXYZColorFlt PDEBlackPointFlt;


/**
	A structure describing a CalGray color space. 

	<p>Default value: <code>PDEGrayCalFlt calGray = {{0, 0, 0}, {0, 0, 0}, 1};</code></p>

	@see PDEColorSpaceCreate
*/
typedef struct _t_PDEGrayCalFlt {

	/** */
    PDEWhitePointFlt 	whitePoint;

	/** */
    PDEBlackPointFlt 	blackPoint;

	/** */
    float				gamma;
} PDEGrayCalFlt;


/**
	A structure describing a CalRGB color space. It is the same as AGMRGBCalFlt 
	(it is only available as part of the PDF Library SDK). 

	<p>Default value: <code>{{0, 0, 0}, {0, 0, 0}, 1, 1, 1, {1, 0, 0, 0, 1, 0, 0, 0, 1}}; </code></p>

	@see PDEColorSpaceCreate
*/
typedef struct _t_PDERGBCalFlt {

	/** */
	PDEWhitePointFlt	whitePoint;

	/** */
    PDEBlackPointFlt	blackPoint;

	/** */
    float 				redGamma;

	/** */
    float 				greenGamma;

	/** */
    float 				blueGamma;

	/** */
    float 				matrix[9];
} PDERGBCalFlt;


/**
	A structure describing a Lab color space. 

	<p>Default value: <code>{{0, 0, 0}, {0, 0, 0}, {-100, 100}, {-100, 100}}; </code></p>
	
	@see PDEColorSpaceCreate
*/
typedef struct _t_PDELabCalFlt {

	/** */
    PDEWhitePointFlt	whitePoint;

	/** */
    PDEBlackPointFlt	blackPoint;

	/** */
    PDEColorRangeFlt	rangeA, rangeB;
} PDELabCalFlt;

/* DeviceGray, DeviceRGB, DeviceCMYK - pass NULL for PDEColorSpaceStruct */



/**
	A PDEColorSpace that describes a Pattern color space. 
	@see PDEColorSpaceCreate
*/
typedef PDEColorSpace PDEPatternColorSpace;


/** An ICC-based color space. */
typedef struct _t_PDEICCBasedColorData {

	/** Set the size to <code>sizeof(PDEICCColorData)</code>. */
	ASSize_t		size;			

	/** A stream containing an ICC Profile. */
	ASStm			iccstream;		

	/** The number of color components (<code>1</code>, <code>3</code>, or <code>4</code>). */
	ASUns32			nComps;			

	/** (Optional) An alternate color space. */
	PDEColorSpace	altCs;			
} PDEICCBasedColorData;


/**
	A structure describing an indexed color space. 
	@see PDEColorSpaceCreate 
*/
typedef struct _t_PDEIndexedColorData {

	/** Set the size to <code>sizeof(PDEIndexedColorData)</code>. */
	ASSize_t		size;			

	/** The base color space. */
	PDEColorSpace	baseCs;			

	/** The highest color value. */
	ASUns16			hival;			

	/** Indexed color lookup data. */
	char			*lookup;		

	/** The number of bytes in the lookup data. */
	ASUns32			lookupLen;		
} PDEIndexedColorData;


/**
	A structure describing a separation color space. 
	@see PDEColorSpaceCreate 
*/
typedef struct _t_PDESeparationColorData {

	/** The size of the data structure. It must be set to 
		<code>sizeof(PDESeparationColorData)</code>. 
	*/
	ASSize_t		size;			

	/** The name of the separation or colorant. */
	ASAtom			name;			

	/** The alternate color space. */
	PDEColorSpace	alt;			

	/** The tintTransform dictionary or function. 
		See Section 4.5.5 in the <i>PDF Reference</i>. 
	*/
	CosObj			tintTransform;	
} PDESeparationColorData;


/**
	A structure describing a DeviceRGB or DeviceCMYK color space. 
	
	@see PDEColorSpaceCreate 
*/
typedef struct _t_PDEDeviceNColorData {

	/** The size of the data structure. It must be set to 
		<code>sizeof(PDEDeviceNColorData)</code>. 
	*/
	ASSize_t		size;			

	/** The names of the colorants. */
	ASAtom			*names;			

	/** The number of colorants. */
	ASUns32			nNames;			

	/** The alternate color space. */
	PDEColorSpace	alt;			

	/** The tintTransform dictionary or function. 
		See Section 4.5.5 in the <i>PDF Reference</i> for more information. 
	*/
	CosObj			tintTransform;	
} PDEDeviceNColorData;


/** A color space structure for PDEColorSpaceCreate(). See Section 4.5 in the <i>PDF Reference</i>
	for information on color spaces.
	@see PDEColorSpaceCreate
*/
typedef union {

	/** */
	PDEGrayCalFlt			*calGray;

	/** */
	PDERGBCalFlt			*calRGB;

	/** */
	PDELabCalFlt			*lab;

	/** */
	PDEICCBasedColorData	*icc;

	/** */
	PDEIndexedColorData		*indexed;

	/** */
	PDEPatternColorSpace	patternbase;

	/** */
	PDESeparationColorData	*sep;

	/** */
	PDEDeviceNColorData		*devn;
} PDEColorSpaceStruct;

/* codePage supported for PDSysEncodingCreateFromCodePage */
enum	{
		/* Windows code pages	*/
	kPDCodePageWinEastEuropeanRoman		=		1250,
	kPDCodePageWinCyrillic				=		1251,
	kPDCodePageWinGreek					=		1253,
	kPDCodePageWinTurkish				=		1254,
	kPDCodePageWinHebrew				=		1255,
	kPDCodePageWinArabic				=		1256,
	kPDCodePageWinBaltic				=		1257,
	
		/* Macintosh pseudo code pages	*/
	kPDCodePageMacCentralEuropean		=		-9994,
	kPDCodePageMacCroatian				=		-9993,
	kPDCodePageMacRomanian				=		-9992,
	kPDCodePageMacCyrillic				=		-9991,
	kPDCodePageMacUkrainian				=		-9990,
	kPDCodePageMacGreek					=		-9989,
	kPDCodePageMacTurkish				=		-9988,
	kPDCodePageMacHebrew				=		-9987,
	kPDCodePageMacArabic				=		-9986
};



/*
**   Attribute definitions for JPX objects
*/

/**
	Attributes of a JPX image.
*/
typedef struct _t_PDEImageJPXAttrs {
   ASUns32 flags;    /* PDEImageAttrFlags */
   ASInt32 width;
   ASInt32 height;
   ASInt32 tileWidth;
   ASInt32 tileHeight;
   ASInt32 nResolutions;
   ASInt32 nComponents;
   ASInt32 bpc[24];
   ASReal decode[24];
} PDEImageJPXAttrs, *PDEImageJPXAttrsP;

    

/**
	JPX Color Space types.
*/
typedef enum {
    kJPXCSUnknown              = 0x0000,
    kJPXCSEnumerated           = 0x0001,
    kJPXCSRestrictedICC        = 0x0002,
    kJPXCSAnyICC               = 0x0003,
    kJPXCSVenderColor          = 0x0004
} JPXColorSpaceType;	


/**
	Attributes of a JPXCSEnumerated JPXColorSpace.
*/
typedef struct _t_JPXCSEnumAttrs {
	/** The enumerated color space number. */
	ASUns32 enumNumber;        
	/** The standard illuminant is used.   */
	ASBool  stdIlluminant;     
	/** A 4-byte illuminant value.       */
	ASUns32 illuminant;        
	/** L, A, and B range values.      */
	ASUns32 labRange[3];       
	/** L, A, and B offset values.     */
	ASUns32 labOffset[3];      
} JPXCSEnumAttrs, *JPXCSEnumAttrsP;


#ifdef __cplusplus
}
#endif


#if defined( WINDOWS ) || defined( WIN_ENV )
#pragma pack (pop, peexpt )		/* reset to /Zp  */
#endif

#endif /* _H_PEExpT */


