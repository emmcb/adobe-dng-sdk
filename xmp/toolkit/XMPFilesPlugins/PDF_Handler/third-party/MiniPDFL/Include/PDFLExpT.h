/*************************************************************************
 *
 *  Copyright 1998, 2001-2007 Adobe Systems Incorporated
 *  All Rights Reserved.
 *
 * NOTICE: Adobe permits you to use, modify, and distribute this file
 * in accordance with the terms of the Adobe license agreement
 * accompanying it. If you have received this file from a source other
 * than Adobe, then your use, modification, or distribution of it
 * requires the prior written permission of Adobe.
 *
 **************************************************************************/
/*
** PDFLExpT.h
**
** Types for library specific API's
**
*/

#ifndef _H_PDFLExpT
#define _H_PDFLExpT

#include "Environ.h"
#include "ASExpT.h"
#include "CosExpT.h"
#include "PIExcept.h"
#include "PDExpT.h"

#ifdef __cplusplus
class AGMPrintInterface;
#endif

#ifdef WIN_ENV

#if AGM_STRICT
#ifndef OS2PM_ENV
#include <Windows.h>
#else
#define INCL_DOS
#define INCL_PM
#define INCL_GPI
#include <os2.h>
#endif
typedef HWND	PlatformWindowPtr;
#ifndef OS2PM_ENV
typedef HBITMAP	PlatformBitmapPtr;
#else
typedef HPS		PlatformBitmapPtr;
#endif
#else
typedef void*	PlatformWindowPtr;
typedef void*	PlatformBitmapPtr;
#endif	/* AGM_STRICT */
#define __defedPlatformTypes

#endif	/* WIN_ENV	*/

#if defined( WINDOWS ) || defined( WIN_ENV )
#pragma pack(push, peexpt, 8)
#endif

#ifdef __cplusplus
extern "C" {
#endif


/* Types for Notifications */

/* NSelector
** Integer constant by which notifications are identified.
*/
#ifndef _T_NSELECTOR
#define _T_NSELECTOR
typedef Int32 NSelector;
#define BAD_NSELECTOR		(-1)
#endif

typedef struct _t_PDFLMemStats {
	ASUns32 numAllocs;
	ASUns32 curUsage;
	ASUns32 highUsage;
} PDFLMemStats;

/* From Cos.h 
   Prototype for callback to control permissions for accessing
   external files referenced within .pdf.
*/
typedef ACCBPROTO1 ASBool (ACCBPROTO2 *ExternalFilePermissionProc)(
	CosDoc dP, ASFileSys *fileSys, ASPathName *path
);

/**
	A callback for PDFLibraryRegisterRNG().
	It is called once to provide random data for encryption.
	Normally an operating system supplied source of highly
	random numbers is used (<code>/dev/urandom</code> on Mac OS and Unix, 
	<code>CryptGenRandom()</code> on Windows).  For some supported Unix environments, the
	<code>/dev/urandom</code> device does not exist.  While it is
	preferable to install this device, the PDFLibraryRegisterRNG()
	function is provided for the client to register an
	alternate source for strong random data.
	@param len  The length (in bytes) of the request.
	@param buffer  The buffer for the client to fill in.
	@return The number of bytes actually filled in the buffer.
	
	@see PDFLibraryRegisterRNG
*/
typedef ACCBPROTO1 ASSize_t (ACCBPROTO2 *PDFLClientRNGProc)(
			ASSize_t len, ASUns8P buffer);

/* XXXXXXXXXXXXXXX Begin From PDPrint.h XXXXXXXXXXXXXXX */

/****************************************************************/
/*		POSTSCRIPT PRINTING										*/
/* The following types are specific to PostScript printing.		*/
/****************************************************************/

/* Choices for downloading fonts and other resources */

/**
	Specifies how to include a resource in a file. 
*/
typedef ASEnum8 PDInclusion;
enum {

	/** Include the resource only once per file. */
	kIncludeOncePerDoc,

	/** Include the resource on every page in the file. */
	kIncludeOnEveryPage,

	/** Never include the resource. */
	kIncludeNever,

	/** When needed. */
	kIncludeWhenNeeded,

	/**  */
	kIncludeByRange
};


/**
	Specifies what kind of file to emit. 
*/
typedef ASEnum8 PDOutputType;
enum {

	/** Emit a PostScript file. */
	PDOutput_PS,            

	/** Emit an EPS file with no preview. */
	PDOutput_EPSNoPrev,     

#if !MAC_PLATFORM || (MAC_PLATFORM && !AS_ARCH_64BIT)
	/** Emit an EPS file with standard preview. */
	PDOutput_EPSMacStdPrev, 

	/** Emit an EPS file with extended preview. */
	PDOutput_EPSMacExtPrev
#endif  
};


/**
	CJK font related option for PostScript printing. 
*/
typedef ASEnum8 PDFarEastFont;
enum {

	/** Download all CJK fonts to the printer. */
	PDPDFarEastFont_Download_All,	

	/** Download only embedded fonts to the printer. */
	PDPDFarEastFont_Download_None,	

	/** GDI/QuickDraw on PS option. PS Level 1 should use this to print CJK. */
	PDPDFarEastFont_Print_As_Image	
};


/** A structure for controlling the printing of glyphs.
	@see PDFontPSEmitGlyphsIncr
*/
typedef struct
{

	/** The string.*/
	char *str;

	/** Byte length. */
	ASUns32 len;  

	/** Displacements. */
	ASRealPoint *displacements;

	/** The number of glyphs. */
	ASUns32 numGlyphs; 

	/** Flags. Reserved for internal use. */
	ASUns32 flags;
} PDPrintStr, *PDPrintStrP;


/** Specifies a synthetic font style.
	@see PDPrintFontRec
 */
enum {

	/** */
	PDRoman = 0,

	/** */
	PDItalic,

	/** */
	PDBold,

	/** */
	PDBoldItalic
};
typedef ASUns8 PDFontStyle;


/**
	Font array.
	@see PDPrintFontArray
*/

enum
{
	/** A fLag to indicate that the font uses a single byte encoding. */
	kPDSingleByteFont = 0x0001  
};

typedef struct
{

	/** The font name. */
	const char  *name;

	/** The font encoding. */
	const char  *encoding;

	/** CID font name. */
	const char  *cidFontName;

	/** Gothic or Mincho. */
	ASInt16 style;					

	/** Roman, Italic, Bold, or BoldItalic. */
	PDFontStyle syntheticStyle;		

	/** Set to <code>true</code> if glyphs from this component are used. */
	ASBool used;					

	/** Set to <code>true</code> if this component is emmitted. */
	ASBool emitted;					

	/** Baseline adjustment. */
	ASInt32	baseLineAdj;	

	/** Flags bits. 
		@see kPDSingleByteFont 
	*/
	ASUns32 flags;

} PDPrintFontRec, *PDPrintFontP;



/**
	An array of font pointers for use in PDFontPSGetComponentFontList.
	@see PDFontPSGetComponentFontList
*/
typedef struct
{

	/** The number of fonts in <code>arr</code>. */
	ASUns32 count;

	/** Whether to use the Display CT Font. */
	ASBool usesDisplayCTFont;

	/** The font array. */
	PDPrintFontP arr;

} PDPrintFontArray, *PDPrintFontArrayP;


/** Support for tiled flattening */
enum {

	/** No tiling. */
	kPDNoTiling = 0,

	/** Constant tiling. */
	kPDConstantTiling,

	/** Adaptive tiling. */
	kPDAdaptiveTiling
};
typedef ASInt32 PDFlattenTilingMode;

/** */
enum {

	/** Print all pages normally */
	kPDNoPageTiling = 0,

	/** Use tiling settings for all pages */
	kPDTileAllPages,

	/** Use tiling only for pages larger than size indicated in PDTileRec of tileInfo */
	kPDTileLargePages
};
typedef ASInt16 PDPageTilingMode;


/** Controls tile flattening. It is primarily of interest to resource limited (embedded) systems
	developers.
	@see PDPrintParams
*/
typedef struct {

	/** The size of the structure. Set it to <code>sizeof(PDFlattenRec)</code>. */
	ASSize_t	size;

	/** <code>0</code> = no tiling; <code>1</code> = constant tiling; <code>2</code> = adaptive tiling */
	PDFlattenTilingMode	tilingMode;	

	/** Output text outlines instead of native text. */
	ASBool				useTextOutlines;	

	/** Allow shading ouput. */
	ASBool				allowShadingOutput;

	/** Allow level 3 shading output. */
	ASBool				allowLevel3ShadingOutput;

	/** For converting stroke to outline. */
	ASBool				strokeToFill;

	/** Clip complex checkbox. */
    ASBool				clipComplexRegions;

	/** Resolution for flattening the interior of an atomic region. */
	float				internalDPI;	

	/** Resolution for flattening edges of atomic regions. */
	float				externalDPI;	

	/** Flattener path resolution; the default is <code>800</code>. */
	float				pathDPI;		

	/** Target tile size in points. */
	ASUns32				tileSizePts;	

	/** Maximum image size when flattening. The default is <code>0</code>. */
	ASUns32				maxFltnrImageSize;	

	/** Adaptive flattening threshold. */
	ASUns32				adaptiveThreshold;	

	/** Attempt to preserve overprint. */
	ASBool				preserveOverprint;	
} PDFlattenRec, *PDFlatten;


/** Duplex values.
	@see PDPrintParams
*/
enum {

/** */
kPDDuplexOff,

/** */
kPDDuplexOnTumbleShort,

/** */
kPDDuplexOnTumbleLong
};
typedef ASEnum8 PDDuplexEnum;


/** */
typedef struct {

	/** Used for giving information to and from the user. */
	PDTileRec pubRec; 
	/* new fields not yet public */

	/** (pts)*/
	ASUns32 imageablePaperWidth; 

	/** (pts)*/
	ASUns32 imageablePaperHeight; 

	/** Unprintable left edge (pts). */
	ASUns32 unprintablePaperWidth; 

	/** Unprintable bottom edge (pts). */
	ASUns32 unprintablePaperHeight; 

	/** The amount tile indented within the imageable area (pts). */
	ASUns32 indent;	    

	/** The clockwise rotation angle in degrees for tile. */
	ASInt32 rotateAngle; 

	/** The optional label string template has 6 parameters it can fill in. 
		If it is <code>NULL</code>, the label template <code>"$DOC$ $DATE$ $TIME$ $PAGE$ ($ROW$, $COL$)"</code> is used.
		@example <code>"page $PAGE$, row=$ROW$, column=$COL$"</code>
	*/
	ASText labelTemplate;
	
	/** The scale selected by the user in the Print Setup or Printer Properties dialog box. */
	double driverScale;
	/** The scale for tiled pages. */
	double tileScale;
}PDTileRecEx, *PDTileEx;

/* XXXXXXXXXXXXXXX End From PDPrint.h XXXXXXXXXXXXXXX */

/** Bit flags indicating which page marks are emitted for color separations.
	@see PDPrintParams
	@see PDFLPrintDoc
	@see PDPageEmitPSOrient
	@see AVDocPrintSeparations
	@see PDPageMakeSeparations
	@note One of the enum member "kPDPageEmitSlurMarks" is deprecated and
	it has no effect on printing.
*/
enum {
	/** enum kPDPageEmitColorBars. */
	kPDPageEmitColorBars = 0x0001,
	/**  enum kPDPageEmitRegMarks. */
	kPDPageEmitRegMarks = 0x0002, 
	/**  enum kPDPageEmitCropMarks. */
	kPDPageEmitCropMarks = 0x0004, 
	/**  enum kPDPageEmitBleedMarks. */
	kPDPageEmitBleedMarks = 0x0008, 
	/**  enum kPDPageEmitPageInfo. */
	kPDPageEmitPageInfo = 0x0010, 
	/** enum kPDPageEmitTrimMarks. */
	kPDPageEmitTrimMarks = 0x0020, 
	/** Deprecated It has no effect on printing. */
	kPDPageEmitSlurMarks = 0x0040 
};
typedef ASUns32 PDPageMarkFlags;

/** */
enum {
	PDPrintWhatAnnot_NoExtras = 0x01,
	PDPrintWhatAnnot_TrapAnnots = 0x02,
	PDPrintWhatAnnot_PrinterMarks = 0x04	
};
typedef ASEnum8 PDPrintWhatAnnot;

/** */
enum {
	kPDPrintFlipNone = 0x01,
	kPDPrintFlipX = 0x02,
	kPDPrintFlipY = 0x04,
	kPDPrintFlipXY = 0x08
};
typedef ASEnum8 PDPrintWhatFlip;
enum {
kPDPrintTrapNone = 0x01,
kPDPrintTrapInRIP = 0x04
};
typedef ASEnum8 PDPrintTrapType;
						

/**
A data structure indicating how a document should be printed.
@note All fields in the PDPrintParams structure apply to PostScript file creation. PDPrintParams are ignored when printing to non-PostScript devices.
@see PDFLPrintDoc
*/
typedef struct {

	/** The size of the data structure. It must be set to <code>sizeof(PDPrintParams)</code>.
	*/
	ASSize_t	size;				


	/** Ranges of pages to print. Use <code>NULL</code> to print the
		entire document.
	*/
	PDPageRange	*ranges;			

	/** The number of ranges of pages to print in ranges.
		The default value is <code>0</code>. */
	ASInt32		numRanges;			

	/** <code>true</code> if the page is scaled to fit the printer page
		size, <code>false</code> otherwise. This field overrides the
		scale. The default value is <code>false</code>. */
	ASBool		shrinkToFit;		

	/** <code>true</code> if small pages are to be scaled up to fit the
		printer page size, <code>false</code> otherwise. It overrides the
		scale. The default value is <code>false</code>. */
	ASBool		expandToFit;		

	/** <code>true</code> if the page is to be rotated to fit the printer's
		orientation and centered in the printer's page size;
		<code>false</code> otherwise.The default value is <code>false</code>.
		Rotation and centering (<code>true</code>) only occur,
		however, if the page contents are too wide to fit
		on a narrow page (or vice versa) and the page
		contents are less than an inch smaller than the
		target page in one direction. */
	ASBool		rotateAndCenter;	

	/** Print the document, the document and comments, or document form fields only. */
	PDPrintWhat	printWhat;			

	/** Annotation flags which modify PDPrintWhat to enable Pro product behavior. */
	PDPrintWhatAnnot printWhatAnnot;

	/** If <code>true</code>, emit a PostScript file. The default value is
		<code>true</code>.*/
	ASBool		emitPS;				

	/** PostScript level: <code>1</code>, <code>2</code>, or <code>3</code>. The default value is <code>2</code>.*/
	ASInt32		psLevel;			

	/** Print PostScript or EPS with or without a preview.
		@note EPS preview generation is not currently available on UNIX. */
	PDOutputType	outputType;		

	/*	Options controlling how/whether resources should be included in the
		PostScript output file. NOTE: For fonts, the parameters are applied
		in the order in which they are written here. E.g., an embedded Type 1
		font follows the rule for embedded fonts, not the rule for Type 1
		fonts. ALSO NOTE: "kIncludeNever" is not allowed for Type 3 fonts. */

	/** Embed the base fonts. The default value is
		kIncludeNever. */
	PDInclusion	incBaseFonts;		

	/** Embed fonts that are embedded in the PDF file.
		This overrides the <code>incType1Fonts</code>,
		<code>incTrueTypeFonts</code>, and <code>incCIDFonts</code>
		fields. The default value is
		kIncludeOncePerDoc.
	*/
	PDInclusion	incEmbeddedFonts;	

	/** Embed Type 1 fonts. The default value is
		kIncludeOncePerDoc.
	*/
	PDInclusion	incType1Fonts;		

	/** Embed Type 3 fonts. The default value is
		kIncludeOnEveryPage.

		@note This parameter must always be set to
		kIncludeOnEveryPage. PDF files exist
		with Type 3 fonts that contain different
		encodings on different pages.
	*/
	PDInclusion	incType3Fonts;		

	/** Embed TrueType fonts. The default value is
		kIncludeOncePerDoc.
	*/
	PDInclusion	incTrueTypeFonts;	

	/** Embed CID fonts. The default value is
		kIncludeOncePerDoc.
	*/
	PDInclusion	incCIDFonts;		


	/** Include Procsets in the file. The default value is
		kIncludeOncePerDoc.
		@note This is not honored anymore: its value is always <code>false</code>. Procsets are always emitted at the document level. 

	*/
	PDInclusion	incProcsets;		

	/** Include all other types of resources in the file. The
		default value is kIncludeOncePerDoc.
	 */
	PDInclusion	incOtherResources;	


	/** The amount of VM available for font downloading at
		the document level. It is ignored if it is less than or equal to <code>0</code>.
		@note This must be set to <code>0</code> for the toolkit; it is only
		used by the viewer.
	*/
	ASInt32		fontPerDocVM;		

	/** Emit save and restore <code>showpage</code> in PostScript
		files. The default value is <code>true</code>.
	 */
	ASBool		emitShowpage;		

	/** Emit TrueType fonts before any other fonts. The
		default value is <code>false</code>.
	*/
	ASBool		emitTTFontsFirst;	

	/** (PostScript level 2 only) Set the page size on each
		page. Use the media box for outputting to
		PostScript files, use the crop box for EPS files.
		Default is <code>false</code>.
	*/
	ASBool		setPageSize;		

	/** Write DSC (Document Structuring Conventions)
		comments. The default value is <code>true</code>.
		@note This is not honored anymore: its value is always <code>true</code>.
	*/
	ASBool		emitDSC;			

	/** If procsets are included, also include init/term
		code. The default value is <code>true</code>.
		@note This must be set to <code>true</code> for the toolkit.
	*/
	ASBool		setupProcsets;		

	/** Emit images for Level-1 separations. The default
		value is <code>false</code>.
		@note This will be honored only when printing to a printer.
	*/
	ASBool		emitColorSeps;		

	/** <code>true</code> if binary data is permitted in the PostScript
		file, <code>false</code> otherwise. It is overridden by the printer's 
		"Output Protocol" device settings. The default value is 
		<code>true</code>.		
	*/
	ASBool		binaryOK;		
	
	/** Add <code>SubFileDecode</code> filter to work around stream problems [<code>false</code>].
		The <code>SubFileDecode</code> filter will always be emitted when you print as binary.
	*/
	ASBool		useSubFileDecode;

	/** <code>true</code> if adding no unnecessary filters when emitting
		image data, <code>false</code> otherwise. The default value
		is <code>true</code>.
	*/
	ASBool		emitRawData;		

	/** If including TrueType fonts, convert to Type 42
		fonts instead of Type 1 fonts. The default value is
		<code>false</code>. 
		This parameter is valid only for the Export to PS workflow and not for printing to a printer.
	*/
	ASBool		TTasT42;			

	/** The document-wide scale factor. <code>100.0</code> = full size. The
		default value is <code>100</code>.
	*/
	float		scale;				

	/**	If an Image resource uses an external stream, emit
		code that points to the external file. The default
		value is <code>false</code>.
		@note This must be set to <code>false</code>. 
	*/
	ASBool		emitExternalStreamRef;	

	/** Preserve any halftone screening in the PDF file.
		The default value is <code>false</code>.
	*/
	ASBool		emitHalftones;		

	/** Emit PostScript XObjects into the PostScript stream [<code>false</code>]. 
	*/
	ASBool		emitPSXObjects;		

	/** <code>true</code> if CropBox output is centered on the page
		when the <code>CropBox < MediaBox</code>, <code>false</code>
		otherwise. The default value is <code>true</code>.
	*/
	ASBool		centerCropBox;		


	/** If emitting EPS, include only CMYK and gray
		images.
	*/
	ASBool		emitSeparableImagesOnly; 

	/** When emitting the extended graphics state, include
		the device-dependent parameters (overprint, black
		generation, undercolor removal, transfer, halftone,
		halftone phase, smoothness, flatness, rendering
		intent) in addition to the device-independent
		parameters (font, line width, line cap, line join,
		miter limit, dash pattern). If this flag is <code>false</code>,
		only the device-independent parameters will be
		emitted. This flag overrides <code>emitHalftones</code>; if
		this is <code>false</code>, then halftones are not emitted.
		[true] 
	*/
	ASBool		emitDeviceExtGState;	

	/** If all zeroes, it is ignored. Otherwise, it is used for
		<code>%%BoundingBox</code> DSC comment and in
		<code>centerCropBox</code> calculations and for
		<code>setpagedevice</code>. The default value is <code>[0 0 0 0]</code>.
	*/
	ASFixedRect	boundingBox;		



	/** Used when printing with system fonts. The
		default value is <code>false</code>.
	*/
	ASBool		useFontAliasNames;		

	/** Emit a concat at the beginning of each page so
		that the page is properly rotated. Used when
		emitting EPS. The default value is <code>false</code>.
	*/
	ASBool		emitPageRotation;		


	/* New for PDF 1.3. */

	/** If set to <code>true</code>, reverse the order of page output. */
	ASBool		reverse;		

	/** Temporary crop box to represent the selected region. */
	ASFixedRect		*tCropBox;		

	/** Emit page clip. */
	ASBool		emitPageClip;		

	/** Emit transfer. */
	ASBool		emitTransfer;		

	/** Emit black generation. */
	ASBool		emitBG;		

	/** Emit undercolor removal. */
	ASBool		emitUCR;		

	/* CJK font related option */

	/** GDI to PS or download CJK fonts.
		<code>PDPDFarEastFont_Print_As_Image</code> will not be honored.
	*/
	PDFarEastFont	farEastFontOpt;	

	/** If <code>true</code>, do not perform CJK substitution on the printer.
		This will work only when <code>farEastFontOpt</code> is <code>PDPDFarEastFont_Download_None</code>. 
	*/
	ASBool			suppressCJKSubstitution; 
		/* This has an impact only when farEastFontOpt is PDPDFarEastFont_Download_None. */
	/** Do not emit CSAs for 4-component (CMYK) colors. This will take precedence over the <code>hostbasedCM</code> settings. */
	ASBool		suppressCSA;

	/** For separator, do host-based color management. */
	ASBool		hostBased;	

	/** The output color space when <code>hostBased</code> color
		management is <code>true</code>.
	*/
	ASAtom		hostBasedOutputCS;	

	/** Sets the duplex mode if the device supports duplex printing. The default value is <code>kPDDuplexOff</code>. */
	PDDuplexEnum duplex;	

	/** Determines whether to tile none, all, or only large pages. */
	PDPageTilingMode	doTiling; 

	/** If non-<code>NULL</code>, tiling is desired with these
		parameters.
	*/
	PDTileEx	tileInfo;	

	/** Enable the auto-rotating behavior from past
		versions of Acrobat. 
	*/
	ASBool		rotate;		

	/** Do <code>hostBased</code> color management using the <code>destination</code> profile. The default is <code>false</code>, which means doing CSA generation for profiles
	instead of converting all colors on the host.
	*/
	ASBool		hostBasedCM;	

	/** If <code>hostBase</code> color management is <code>true</code>, use this
		profile. It should be the name of the profile like "U.S. Web Coated (SWOP) v2"
		and not the name of the file.
	*/
	char        destProfile[256];

	/** An ASAtom representing the device color space
		(DeviceGray, DeviceRGB, and so on).
	*/
	ASAtom		destCSAtom;

	/** <code>true</code> means try to save VM when printing to
		PostScript. 
	*/
	ASBool		saveVM;			

	/** Do the overprint preview operation. */
	ASBool		doOPP;
	
	/** When <code>true</code>, suppress OPP for pages that do not contain spot colors */
	ASInt32		suppressOPPWhenNoSpots;

	/** If <code>true</code>, do it fast; <code>false</code> means PostScript code
		must be page-independent. If it is set to <code>true</code>, font
		downloads are forced from
		kIncludeOnEveryPage to
		kIncludeOncePerDoc. 
	*/
	ASBool		optimizeForSpeed; 

	/** If <code>true</code>, do not set rendering intent in PostScript
		stream due to broken non-default CRDs. 
		@note This is not honored anymore: its value is always <code>false</code>.
	*/
	ASBool      brokenCRDs;     

	/** If <code>true</code>, store all possible resources in VM. */
	ASBool		useMaxVM;		

	/** Used when <code>setPageSize</code> is <code>true</code> to prevent
		unneeded <code>setpagedevice</code> calls. 
	*/
	ASInt32		lastWidth;		

	/** Used when <code>setPageSize</code> is <code>true</code> to prevent
		unneeded <code>setpagedevice</code> calls. 
	*/
	ASInt32		lastHeight;

	/** DPI for bitmaps. The default is <code>300</code>. 
		This will only work when you set <code>useFullResolutionJP2KData</code> to <code>false</code>; it should not be less then <code>50</code>.
	*/
	ASUns32		bitmapResolution;

	/** DPI for gradients interior to the object (not
		edges). It can generally be lower than the
		<code>bitmapResolution</code>. The default is <code>150</code>.
	*/
	ASUns32		gradientResolution;

	/** The transparency level. The range is <code>0-100</code>. */
	ASUns32		transparencyQuality; 


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

	/** Determines whether to use the maximum available JPEG2000
		resolution. 
	*/
	ASBool		useFullResolutionJP2KData;	

	/** When <code>true</code>, it requests that separations, one sheet
		per ink, be generated in the RIP (printer). 
		It is available only in Acrobat Professional. 
		@product_exclude RDR
		@product_exclude STD
	*/
	ASBool		emitInRipSeps;	

	/** Page mark indication. It is a bit-wise <code>OR</code> of the
		PDPageMarkFlags values.
		
		<p>It applies to print separations, which are available
		only in Acrobat Professional.</p>
		@product_exclude RDR
		@product_exclude STD
	*/
	PDPageMarkFlags whichMarks;	

	/** When <code>true</code>, use western style for page marks.
		It applies to print separations, which are available
		only in Acrobat Professional. Obsoleted by
		<code>markStyle</code> below.
		@product_exclude RDR
		@product_exclude STD
	*/
	ASBool		westernMarksStyle; 

	/** When <code>true</code>, print using proofing settings. */
	ASBool      doProofing;     

    /** Description string for the proofing profile.    */
    char        proofProfile[256];

    /** Proofing settings: simulate ink black. */
    ASBool      inkBlack;
    /** Proofing settings: simulate paper white. */
    ASBool      paperWhite;
    
    /** When <code>true</code>, emit <code>execform</code> calls when emitting
		Form XObjects.
	*/
	ASBool		useExecForm;	

	/** A structure containing parameters that control
		tile flattening.
	*/
	PDFlatten	flattenInfo;

	/** When <code>true</code>, invert the plate. */
	ASBool		negative;

	/** One of the following constants:
		
		<p><code>kPDPrintFlipNone = 0x01</code></p>
		<p><code>kPDPrintFlipX = 0x02</code></p>
		<p><code>kPDPrintFlipY = 0x04</code></p>
		<p><code>kPDPrintFlipXY = 0x08</code></p>
		

		<p>Mirroring is done in the PostScript output stream.</p>
	*/
	ASEnum8		mirrorprint;

	/** Enables collation for the viewer. */
	ASUns32		numCollatedCopies;	/* Obsolete : host-collation is handled by AGMP now. */

	/** Honors the flatness settings over <code>gstate</code> flatness. */
	ASBool emitFlatness;

	/** 
		The trap type:
		<ul>
		<li><code>kPDPrintTrapNone = 0x01</code>.</li>
		<li><code>kPDPrintTrapOnHost = 0x02</code> is not supported in Acrobat 7.</li>
		<li><code>kPDPrintTrapInRIP = 0x04</code>.</li>
		</ul>
	*/
	ASInt32 trapType;	/* kPDPrintTrapNone = 0x01,
							kPDPrintTrapOnHost = 0x02 NOT SUPPORTED in Acrobat 7
							kPDPrintTrapInRIP = 0x04
						*/

	/**
		This parameter is obsolete from PDFL 10.0.
		When this is set, emit TrueType fonts as CIDType2 instead of as CIDFontType0. 
	*/
	ASBool TTasCIDT2;
	/** Specify the style to use for page marks.
		The <code>PDPrintMarkStyles</code> enum contains possible styles.
		It applies to print separations, which are available
		only in Acrobat Pro.
		@product_exclude RDR
		@product_exclude STD
	*/
	ASInt32		markStyle;

	/** The line weight to use for printer marks. */
	float		lineWidth;		/* line weight to use for printer marks */

	/** <code>true</code> if the printer is a Mac QuickDraw printer, <code>false</code> otherwise. */
	ASBool		macQDPrinter;	/* <code>true</code> if the printer is a Mac QuickDraw printer on Normalizer. */

	/** If <code>markStyle == -1</code>, this should be a valid file name pointing to a valid .mrk file for custom printer marks. */
	ASPathName	customMarksFileName;	/* if markStyle == -1, this should be a valid filename, pointing to a valid .mrk file for custom printer marks */
#ifdef __cplusplus
	/** The AGMP interface pointer. This should not be set by PDF Library SDK clients. */
	AGMPrintInterface *pAGMPI; /* The AGMP interface pointer. This should not be set by the PDFL SDK clients. */
#endif
/* Keep debug parameters at the end so that release libraries can be run with debug test apps */
#if DEBUG

	/** Provide a means for all 4 page rotations to be tiling-exercised at once. */
	ASInt32		testTilingMode;	
#endif

	/** convert Gray to K (any Gray to CMYK (0,0,0,K)) */
	ASBool grayToK;

} PDPrintParamsRec, *PDPrintParams;


/*------------------------------------------------------------------------
	PDPageDraw types
------------------------------------------------------------------------*/

/**
	Bit flags indicating how a page is rendered.
*/
enum {

	/**  Erase the page while rendering only as needed.*/
	kPDPageDoLazyErase 			   = 0x00000001,

	/** Ignore Isolated and Knockout transparency at page boundary.*/
	kPDPageIgnoreIsolatedAndKnockoutTransparencyGroup
								   = 0x00000010,

	/** Draw annotation appearances.*/
	kPDPageUseAnnotFaces		   = 0x00000040,

	/** The page is being printed.*/
	kPDPageIsPrinting			   = 0x00000080,

	/** Display overprint preview. */
	kPDPageDisplayOverPrintPreview = 0x00000100,

	/** Use trap network annotations. */
	kPDPageUseTrapAnnots		   = 0x00002000,	

	/** Directly imposed page. */
	kPDPageDirectlyImposed         = 0x00004000,

	/** PostScript printing. */
	kPDPageIsPSPrinting			   = 0x00008000,

	/** Emit a page group. */
	kPDPageEmitPageGroup		   = 0x00010000,

	/** User printer's mark annotations. */
	kPDPageUsePrinterMarkAnnots	   = 0x00020000,

	/** Pass open prepress interface (OPI) to AGM port. */
	kPDPagePassOPItoAGMPort		   = 0x00040000,

	/** Pass metadata to AGM port. */
	kPDPagePassMetadatatoAGMPort   = 0x00080000,

	/** Pass optional content to AGM port. */
	kPDPagePassOCtoAGMPort		   = 0x00100000,

    /** Do not substitute working spaces. */
    kPDPageDoNotSubstituteWorkingSpaces =
                                     0x00800000,

    /** Render colors in BGR order rather than RGB. It is only valid for calls to
	    PDPageDrawContentsToMemory() and only when outputing to an RGB colorspace. */
	kPDPageSwapComponents          = 0x01000000,

    /** Supress raster alpha. */
    kPDPageSuppressRasterAlpha     = 0x02000000,

    /** If this is set, only use a working space instead of a device space
        if the process color model of the target device is different than
        that of the source */
    kPDPageWorkingSpacesOnlyForChange = 0x04000000,

	/** If set, only consider Stamp annotations. This overrides kPDPageUseAnnotFaces. */
	kPDPageUseStampAnnotsOnly		= 0x08000000
};
typedef ASUns32 PDPageDrawFlags;

/**
Const strings used as ASCab keys when passing PDPageDrawFlags. These are valid only 
for calls to PDPageDrawContentsToMemoryEx and PDPageDrawContentsToWindowEx2.
@see PDPageDrawSmoothFlags
@see PDPageDrawContentsToMemoryEx
@see PDPageDrawContentsToWindowEx2
*/

/**
Erase the page while rendering only as needed.
@ingroup PDPageDrawFlagsStr
@see PDPageDrawContentsToMemoryEx
@see PDPageDrawFlags
@see PDPageDrawContentsToWindowEx2
*/
#define kPDPageDoLazyEraseStr "DoLazyErase"

/**
Ignore Isolated and Knockout transparency at page boundary.
@ingroup PDPageDrawFlagsStr
@see PDPageDrawContentsToMemoryEx
@see PDPageDrawFlags
@see PDPageDrawContentsToWindowEx2
*/
#define kPDPageIgnoreIsolatedAndKnockoutTransparencyGroupStr "IgnoreIsolatedAndKnockoutTransparencyGroup"

/**
Draw annotation appearances.
@ingroup PDPageDrawFlagsStr
@see PDPageDrawContentsToMemoryEx
@see PDPageDrawFlags
@see PDPageDrawContentsToWindowEx2
*/
#define kPDPageUseAnnotFacesStr "UseAnnotFaces"

/**
The page is being printed.
@ingroup PDPageDrawFlagsStr
@see PDPageDrawContentsToMemoryEx
@see PDPageDrawFlags
@see PDPageDrawContentsToWindowEx2
*/
#define kPDPageIsPrintingStr "IsPrinting"

/**
Display overprint preview.
@ingroup PDPageDrawFlagsStr
@see PDPageDrawContentsToMemoryEx
@see PDPageDrawFlags
@see PDPageDrawContentsToWindowEx2
*/
#define kPDPageDisplayOverPrintPreviewStr "DisplayOverPrintPreview"

/**
Use trap network annotations.
@ingroup PDPageDrawFlagsStr
@see PDPageDrawContentsToMemoryEx
@see PDPageDrawFlags
@see PDPageDrawContentsToWindowEx2
*/
#define kPDPageUseTrapAnnotsStr "UseTrapAnnots"

/**
Directly imposed page.
@ingroup PDPageDrawFlagsStr
@see PDPageDrawContentsToMemoryEx
@see PDPageDrawFlags
@see PDPageDrawContentsToWindowEx2
*/
#define kPDPageDirectlyImposedStr "DirectlyImposed"

/**
PostScript printing.
@ingroup PDPageDrawFlagsStr
@see PDPageDrawContentsToMemoryEx
@see PDPageDrawFlags
@see PDPageDrawContentsToWindowEx2
*/
#define kPDPageIsPSPrintingStr "IsPSPrinting"

/**
Emit a page group.
@ingroup PDPageDrawFlagsStr
@see PDPageDrawContentsToMemoryEx
@see PDPageDrawFlags
@see PDPageDrawContentsToWindowEx2
*/
#define kPDPageEmitPageGroupStr "EmitPageGroup"

/**
User printer's mark annotations.
@ingroup PDPageDrawFlagsStr
@see PDPageDrawContentsToMemoryEx
@see PDPageDrawFlags
@see PDPageDrawContentsToWindowEx2
*/
#define kPDPageUsePrinterMarkAnnotsStr "UsePrinterMarkAnnots"

/**
Pass open prepress interface (OPI) to AGM port.
@ingroup PDPageDrawFlagsStr
@see PDPageDrawContentsToMemoryEx
@see PDPageDrawFlags
@see PDPageDrawContentsToWindowEx2
*/
#define kPDPagePassOPItoAGMPortStr "PassOPItoAGMPort"

/**
Pass metadata to AGM port.
@ingroup PDPageDrawFlagsStr
@see PDPageDrawContentsToMemoryEx
@see PDPageDrawFlags
@see PDPageDrawContentsToWindowEx2
*/
#define kPDPagePassMetadatatoAGMPortStr "PassMetadatatoAGMPort"

/**
Pass optional content to AGM port.
@ingroup PDPageDrawFlagsStr
@see PDPageDrawContentsToMemoryEx
@see PDPageDrawFlags
@see PDPageDrawContentsToWindowEx2
*/
#define kPDPagePassOCtoAGMPortStr "PassOCtoAGMPort"

/**
Do not substitute working spaces.
@ingroup PDPageDrawFlagsStr
@see PDPageDrawContentsToMemoryEx
@see PDPageDrawFlags
@see PDPageDrawContentsToWindowEx2
*/
#define kPDPageDoNotSubstituteWorkingSpacesStr "DoNotSubstituteWorkingSpaces"

/**
Render colors in BGR order rather than RGB. It is only valid for calls to
PDPageDrawContentsToMemoryEx() and only when outputing to an RGB colorspace.
@ingroup PDPageDrawFlagsStr
@see PDPageDrawContentsToMemoryEx
@see PDPageDrawFlags
@see PDPageDrawContentsToWindowEx2
*/
#define kPDPageSwapComponentsStr "SwapComponents"

/**
Supress raster alpha.
@ingroup PDPageDrawFlagsStr
@see PDPageDrawContentsToMemoryEx
@see PDPageDrawFlags
*/
#define kPDPageSuppressRasterAlphaStr "SuppressRasterAlpha"

/**
If this is set, only use a working space instead of a device space if the process 
color model of the target device is different than that of the source.
@ingroup PDPageDrawFlagsStr
@see PDPageDrawContentsToMemoryEx
@see PDPageDrawFlags
@see PDPageDrawContentsToWindowEx2
*/
#define kPDPageWorkingSpacesOnlyForChangeStr "WorkingSpacesOnlyForChange"

/**
If set, only consider Stamp annotations. This overrides kPDPageUseAnnotFaces.
@ingroup PDPageDrawFlagsStr
@see PDPageDrawContentsToMemoryEx
@see PDPageDrawFlags
@see PDPageDrawContentsToWindowEx2
*/
#define kPDPageUseStampAnnotsOnlyStr "UseStampAnnotsOnly"

/**
Takes ASext as input. If set we use this profile as blending profile.
kPDPageDisplayOverPrintPreviewStr must be set for this to get honored.
@ingroup PDPageDrawFlagsStr
@see PDPageDrawContentsToMemoryEx
@see PDPageDrawFlags
@see PDPageDrawContentsToWindowEx2
*/
#define kPDPageBlendingProfileValueStr "BlendingProfileDesc"



/** 
	Modes used to control the display of reference XObjects.
*/
enum {
	/** Never display reference XObjects. If a file contains reference XObjects,
		their content will not be displayed, and only the proxy will be displayed if
		this mode is selected.
	*/
	kRefXObjNever = 0,

	/** Always display reference XObjects. All files containing reference
		XObjects will display content from the XObjects.
	*/
	kRefXObjAlways = 1,

	/** All files that are PDFX5 compliant and that contain reference XObjects
		will display their content.
	*/
	kRefXObjPDFX5 = 2,

	/** Always display reference XObjects, but do not display the proxy. The PDF
		parsing fails if the target is not found.
	*/
	kRefXObjAlwaysAssured = 3,

	/** Always display reference XObjects, but do not display the proxy. The PDF
		parsing fails if the target PDF/X-5 document is not found.
	*/
	kRefXObjPDFX5Assured = 4
};
typedef ASInt32 PDRefXObjMode;

/**
Bit flags indicating how a page is rendered.
@see PDPageDrawFlags
@see PDPageDrawContentsToMemory
*/
enum {

	/** Draw smooth text. */
	kPDPageDrawSmoothText		= 0x00000001,

	/** Draw smooth line art. */
	kPDPageDrawSmoothLineArt	= 0x00000002,

	/** Draw smooth image. */
	kPDPageDrawSmoothImage		= 0x00000004,

	/** Draw smooth image using bicubic resampling. This option can have performance overhead */
	kPDPageDrawSmoothBicubicImage = 0x00000008
};
typedef ASUns32 PDPageDrawSmoothFlags;

/**
Draw smooth text.
@ingroup PDPageDrawSmoothFlagsStr
@see PDPageDrawContentsToMemoryEx
@see PDPageDrawSmoothFlags
*/
#define kPDPageDrawSmoothTextStr "SmoothText"

/**
Draw smooth line art.
@ingroup PDPageDrawSmoothFlagsStr
@see PDPageDrawContentsToMemoryEx
@see PDPageDrawSmoothFlags
*/
#define kPDPageDrawSmoothLineArtStr "SmoothLineArt"

/**
Draw smooth image.
@ingroup PDPageDrawSmoothFlagsStr
@see PDPageDrawContentsToMemoryEx
@see PDPageDrawSmoothFlags
*/
#define kPDPageDrawSmoothImageStr "SmoothImage"

/**
Draw smooth image using bicubic resampling. This option can have performance overhead.
@ingroup PDPageDrawSmoothFlagsStr
@see PDPageDrawContentsToMemoryEx
@see PDPageDrawSmoothFlags
*/
#define kPDPageDrawSmoothBicubicImageStr "SmoothImageUsingBicubicResampling"


/**
Bit flags that set the default smooth text and smooth images global flags for subsequent rendering methods.
These flags are passed to the <code>PDPrefSetAntialiasLevel</code>, and are similar to <code>PDPageDrawSmoothFlags</code>.
@see PDPageDrawSmoothFlags
@see PDPrefSetAntialiasLevel
*/
enum {

	/** Draw smooth text. */
	kPDPrefAASmoothText		= 0x00000001,

	/** Draw smooth line art. */
	kPDPrefAASmoothLineArt	= 0x00000002,

	/** Draw smooth image. */
	kPDPrefAASmoothImage	= 0x00000010
};

/** Text server. */
typedef struct _t_TextServer *TextServer;

enum {
	/** InDesign style. */
	kPDInDesignStyle = 0,

	/** InDesign J1 style. */
	kPDInDesignJ1Style,

	/** InDesign J2 style. */
	kPDInDesignJ2Style,

	/** Illustrator style. */
	kPDIllustratorStyle,

	/** Illustrator J style. */
	kPDIllustratorJStyle,

	/** QuarkXPress style. */
	kPDQuarkXPressStyle
};

typedef ASInt32 PDPrintMarkStyles;

/*------------------------------------------------------------------------
	PDPrint types
------------------------------------------------------------------------*/

/********************* The client ********************************/
/* PDPrintClient -- passed to PDDocPrintPages
** Contains methods to be implemented by the client.
** Unless otherwise indicated, methods may be NULL, indicating that they're no-ops.
**
*/
/**
	A data structure used by PDDocPrintPages(). It contains methods to be implemented by
	the client. Unless otherwise indicated, methods may be <code>NULL</code>, indicating that they do
	nothing. The methods are called in the order listed in this structure.
*/
typedef struct _t_PDPrintClient *PDPrintClient;
typedef struct _t_PDPrintController *PDPrintController; /* Opaque pointer to print controller */


/** Font encoding methods. */
enum {

	/** The encoding method to be used for Type 1 fonts and substituted fonts. */
	kPDDoReencode,

	/** The encoding method to be used for TrueType Windows font or built-in encoding font. */
	kPDDoNothing,

	/** The encoding method to be used for a TrueType custom font or font with Mac OS encoding. */
	kPDDoXlate
};
typedef ASInt32 PDEncodingType;


/****************************************************************/
/*		OPI Types for Postscript Printing						*/
/* The following types are specific to PostScript printing.		*/
/****************************************************************/


/** OPI Types for PostScript printing. */
typedef enum {

	/** OPI version 1.3. */
	OPIv13,	

	/** OPI version 2.0. */
	OPIv20	
} OPIversion;

/** OPI 1.3 color type information. */
typedef enum {
  /** Spot color type. */
  ALDImageColorType_Spot,
  /** Process color type. */
  ALDImageColorType_Process,
  /** Separation color type. */
  ALDImageColorType_Separation,
  /** Intrinsic color type. */
  ALDImageColorType_Intrinsic
} ALDImageColorType;


/** List structure used in <code>TIFFASCIITagList</code>. */
typedef struct _t_textList {
	/** Tag name. */
	char				*name;
	/** Pointer to next list element. */
	struct _t_textList		*next;
} TextList;

/** TIFF ASCII tags. */
typedef struct _t_TIFFASCIITagList {
	/** The tag number */
	ASInt32				tagNumber;
	/** Tag text list. */
	TextList			*tagText;
	/** For OPI 1.3, the <code>TextList</code> contains only one element. */
	struct _t_TIFFASCIITagList	*next;
} TIFFASCIITagList;

/** OPI 1.3 dictionary. */
typedef struct OPI13dict {
	/** TIFF ASCII tags. */
	TIFFASCIITagList
				*aldImageAsciiTags;
	/** Image color. */
	struct { float c, m, y, k; char *name; }
				aldImageColor;
	/** Image color type. */
	ALDImageColorType aldImageColorType;
	/** The portion of the image to be used. */
	float		aldImageCropFixed[4];
	/** The portion of the image to be used (<code>[left top right bottom]</code>). REQUIRED. */
	ASUns32		aldImageCropRect[4];	
	/** Included image dimensions. <code>[width height]</code> REQUIRED. <code>/Size</code> in PDF. */
	ASUns32		aldImageDimensions[2];	
	/** REQUIRED. Image file name. */
	PDFileSpec	aldImageFileName;	
	/** Changes to the brightness or contrast of the image. */
	ASUns16		*aldImageGrayMap;
	/** Length of the GrayMap array (<code>2</code>, <code>16</code>, or <code>256</code>). */
	ASUns16		aldImageGrayMapLength;	
	/** Image identifier. */
	char		*aldImageID;
	/** Determines whether the image is to overprint (<code>true</code>) or knock out (<code>false</code>) underlying marks on other separations. */
	ASBool		aldImageOverprint;
	/** The location on the page of the cropped image (<code>[llx lly ulx uly urx ury lrx lry]</code>. REQUIRED. */
	float		aldImagePosition[8];
	/** Image resolution (<code>[horiz vert]</code>). */
	float		aldImageResolution[2];
	/** The concentration of the color in which the image is to be rendered. */
	float		aldImageTint;
	/** Specifies whether white pixels in the image are to be treated as transparent. */
	ASBool		aldImageTransparency;
	/** Specifies the number of samples per pixel and bits per sample in the image (<code>[samplesPerPixel bitsPerSample]</code>). */
	ASUns32		aldImageType[2];
	/** A human-readable comment, typically containing instructions or suggestions to the operator of the OPI server on how to handle the image. */
	char		*aldObjectComments;

	/**	Since most parameters are optional, we indicate which were specified.
		It is required that all unspecified pointer-fields be <code>NULL</code>, so that
		error-handlers know exactly which ones to free. */
	struct {
		unsigned
			aldimageasciitags	:1,
			aldimagecolor		:1,
			aldimagecolortype	:1,
			aldimagecropfixed	:1,
			aldimagecroprect	:1,
			aldimagedimensions	:1,
			aldimagefilename	:1,
			aldimagegraymap		:1,
			aldimageid			:1,
			aldimageoverprint	:1,
			aldimageposition	:1,
			aldimageresolution	:1,
			aldimagetint		:1,
			aldimagetransparency:1,
			aldimagetype		:1,
			aldobjectcomments	:1;
	}			defined;
} OPI13dict;

	/* OPI 2.0 */
/** OPI 2.0 image ink information. */
typedef enum {
  /** Image colorant registration value. */
  ImageInk_Registration,

  /** Image colorant full color value. */
  ImageInk_FullColor,

  /** Image colorant monochrome value*/
  ImageInk_Monochrome
} ImageInk;

/** Image ink monochrome list. */
typedef struct _t_monoInkList {
	/** Name of the colorant. */
	char					*name;
	/** Concentration of the colorant. */
	float					level;
	/** Pointer to next list element. */
	struct _t_monoInkList	*next;
} MonoInkList;

/** Image inks. */
typedef struct {
	/** Image ink type. */
	ImageInk		type;
	/** Image ink monochrome list. This is defined only if <code>type == ImageInk_Monochrome</code>. */
	MonoInkList		*mono;
} ImageInks;

/** OPI 2.0 dictionary. */
typedef struct OPI20dict {
	/** The portion of the image to be used (<code>[left top right bottom]</code>). OPTIONAL. */
	float		imageCropRect[4];	

	/** The dimensions of the included image (<code>[width height]</code>). OPTIONAL. */
	float		imageDimensions[2];	

	/**	The external file containing the image. 
	    The following values are required if <code>imageCropRect</code> and <code>imageDimensions</code> are defined:
		
			<p><code>0 <= left < right <= width</code></p>
			<p><code>0 <= top < bottom <= height </code></p>
		
		<p>REQUIRED.</p>
	*/
	PDFileSpec	imageFileName;

	/** The colorants to be applied to the image. */
	ImageInks	imageInks;

	/** Specifies whether the image is to overprint (<code>true</code>) or knock out (<code>false</code>) underlying marks on other separations. */
	ASBool		imageOverprint;

	/** Included image dimensions. */
	ASUns32		includedImageDimensions[2];

	/** The quality of the included image. */
	float		includedImageQuality;

	/** The path name of the file containing the full-resolution image. */
	char		*mainImage;
	TIFFASCIITagList *tiffAsciiTags;

	/*	Since most parameters are optional, we indicate which were specified.
		It is required that all unspecified pointer-fields be NULL, so that
		error-handlers know exactly which ones to free. */
	struct {
		unsigned
			imagecroprect		: 1,
			imagedimensions		: 1,
			imagefilename		: 1,
			imageinks			: 1,
			imageoverprint		: 1,
			includedimagedimensions	: 1,
			includedimagequality: 1,
			mainimage			:1,
			tiffasciitags		:1;
	}			defined;
} OPI20dict;

/** OPI dictionary. */
typedef struct OPIdict {
	/** OPI version (<code>1.3</code> or <code>2.0</code>).*/
	OPIversion		version;		/* 1.3 or 2.0 */

	/** OPI 1.3 or 2.0 dictionary. */
	union {
		OPI13dict	*dict13;
		OPI20dict	*dict20;
	} dict;
} OPIdict;

/****************************************************************/
/*		POSTSCRIPT PRINTING										*/
/* The following types are specific to PostScript printing.		*/
/****************************************************************/

/** ProcSet identifiers used to specify which procsets must be downloaded. */
enum  {
	/** PDF only operators. */
	pdfProcSet, 
	/** AI3 or PDF typography. */
	typeProcSet, 
	/** General image support. */
	imgProcSet,	
	/** Monochrome images. */
	imbProcSet,	
	/** Color images. */
	imcProcSet,		
	/** Indexed (color table) images. */
	imiProcSet,	
	/** Make Original Composite Font (OCF) support. */
	ocfProcSet,	
	/** How many are defined. */
	nProcSets	
};


/** Encoding identifiers (for TE). */
#define AVPS_MAC_ROMAN_ENC	101
/** */
#define AVPS_WIN_ANSI_ENC	102


/********************* The parameters ********************************/

/** 
	<p>A resource tree for a PDPage or other PDModel object.</p>
*/
#ifndef _T_PDRESTRE_
#define _T_PDRESTRE_
typedef struct _t_PDResTree *PDResTree;
#endif /* _T_PDRESTRE_ */



/**
	Maintains information about the current print job and what fonts have been
	downloaded.
	@see PDFontDownloadContextCreate
	@see PDFontDownloadContextDestroy
*/
typedef struct _t_PDFontDownloadContext *PDFontDownloadContext;


/**
	A callback method that is called when an ExtGState object
	is encountered. This method is called for each key/value pair
	in the ExtGState object. If this method returns <code>true</code>, then
	the key/value will be emitted into the print job. If this method
	returns <code>false</code>, then nothing will be emitted for this key/value.
	If <code>emitHalftones</code> is <code>false</code>, then this method will not be called
	for the HT key.
	
	@param egsKey		A key in ExtGState.
	@param egsValue		The ExtGState value for <code>egsKey</code>.
	@param printClient	The object passed to PDDocPrintPages().
	@return <code>true</code> to emit this key/value pair, <code>false</code> to ignore the key/value.

	@see PDDocPrintPages
*/
typedef ACCBPROTO1 ASBool (ACCBPROTO2 *PDDoExtGStateProc)(
			ASAtom egsKey,
			CosObj egsValue,
			PDPrintClient printClient);



/**
	(Required for PostScript printing) A callback for PDPrintClient. 
	It asks the client which encoding method should be used for the 
	font. 
	@param fontP The font whose encoding method is determined. 
	
	@param printClient The PDPrintClient from which this is 
	invoked.
	@return The encoding method, which must be one of the following: 

	<TABLE rules="all" cellspacing="1">
	<TR><TH>Method</TH><TH>Description</TH></TR>
	<TR><TD>kPDDoReencode</TD><TD>Use for Type 1 fonts and substituted fonts.</TD></TR>
	<TR><TD>kPDDoNothing</TD><TD>Use for TrueType Windows font or built-in encoding font.</TD></TR>
	<TR><TD>kPDDoXlate</TD><TD>Use for a TrueType custom font or font with Mac OS encoding.</TD></TR>
	</TABLE>

	@see PDDocPrintPages 
*/
typedef ACCBPROTO1 ASInt32 (ACCBPROTO2 *PDPrintGetFontEncodingMethodProc)(PDFont fontP, PDPrintClient printClient);


/**
	(Required) A callback for PDPrintClient. It emits a font. For 
	Type0 fonts that require font substition, this routine may 
	emit multiple font definitions. The caller can get the list 
	of fonts used by calling GetFontComponentList(). 
	@param stm The PostScript print stream for the current 
	page. 
	@param fontP The font to emit. 
	@param printClient The PDPrintClient from which this is 
	invoked.
	@param flags Print flags.
	@return <code>true</code> if the font is emitted; <code>false</code> otherwise. 
	@see PDDocPrintPages 
*/
typedef ACCBPROTO1 ASBool (ACCBPROTO2 *PDPrintEmitFontProc)(ASStm stm, PDFont fontP, PDPrintClient printClient, ASUns32 flags);


/**
	(Optional) A callback for PDPrintClient. It is called to determine 
	whether a font can be emitted into the print job. This is 
	used to determine whether a font is a document-included 
	resource. Only used for PostScript printing. 

	If it is <code>NULL</code>, the default is to assume that any font can be emitted. 
	
	@param fontP The font to check. 
	@param printClient The PDPrintClient from which this is 
	invoked.
	@return <code>true</code> if client can emit specified font, <code>false</code> otherwise. 
	
	@see PDDocPrintPages 
*/
typedef ACCBPROTO1 ASBool (ACCBPROTO2 *PDPrintCanEmitFontProc)(PDFont fontP, PDPrintClient printClient);



/********************* The client ********************************/
/* PDPrintClient -- passed to PDDocPrintPages
** Contains methods to be implemented by the client.
** Unless otherwise indicated, methods may be NULL, indicating that they're no-ops.
**
*/


/**
	A data structure used by PDDocPrintPages. It contains methods 
	to be implemented by the client. Unless otherwise indicated, 
	methods may be <code>NULL</code>, indicating that they do nothing. The 
	methods are called in the order listed in this structure. 
	
	@see PDDocPrintPages 
	@see PDFontPSEmitGlyphsIncr 
	@see PDFontPSFlushIncrGlyphList 
	@see PDFontPSGetComponentFontList 
*/
typedef struct _t_PDPrintClient {

	/** The size of the structure. */
	ASSize_t		size;

	/** The document to print. */
	PDDoc			pdDoc;

	/** A control structure describing how to print the document. */
	PDPrintParams	params;

	/**************		CONTROL STRUCTURE FOR PDDocPrintPages	*****************/
	/*	The following methods are listed in the order in which they are called. */


	/**
		(Optional) A callback for PDPrintClient. Begin processing
		a document for printing.
		@param ranges IN/OUT Ranges of pages in the document to print.
		@param numRanges IN/OUT The number of page ranges in the document to print.

		@param stm IN/OUT The PostScript print stream.
		@param docTree IN/OUT Must be <code>NULL</code>.
		@param printClient IN/OUT The PDPrintClient from which this is
		invoked.
		@see PDDocPrintPages
	*/
	ACCBPROTO1 void (ACCBPROTO2 *DocBegin)
		(const PDPageRange ranges[], ASInt32 numRanges, ASStm stm,
		 const PDResTree docTree, PDPrintClient printClient);


	/**
		(Optional) A callback for PDPrintClient. Set up a document
		for printing.
		@param ranges IN/OUT The ranges of pages in the document to print.
		@param numRanges IN/OUT The number of page ranges in the document to print.

		@param prologStm IN/OUT The prolog stream.
		@param printClient IN/OUT The PDPrintClient from which this is
		invoked.
		@return <code>false</code> if you do not want PDDocPrintPages to emit procsets
		and other resources.
		@see PDDocPrintPages
	*/
	ACCBPROTO1 ASBool (ACCBPROTO2 *DocSetup)
		(const PDPageRange ranges[], ASInt32 numRanges, ASStm prologStm,
		 PDPrintClient printClient);


	/**
		(Optional) A callback for PDPrintClient. End the setup for
		printing a document.

		<p>One task that might be performed here is to broadcast a
		notification for each range of pages that will be printed,
		as in this pseudo code:</p>

		<code>
		<p>for i = 0 to numRanges-1 ... { </p>
		<p>NOTIFY( PDDocWillPrintPages)(doc, fromPage, toPage, psLevel, binaryOK); </p>
		<p>for pageNum = ranges[ i]-> startPage to ranges[ i]-> lastPage ... </p>
		<p>}</p>
		</code>

		@param stm IN/OUT The PostScript print stream.
		@param printClient IN/OUT The PDPrintClient from which this is
		invoked.
		@see PDDocPrintPages
	*/
	ACCBPROTO1 void (ACCBPROTO2 *EndSetup)(ASStm stm, PDPrintClient client);

	/*	for i = 0 to numRanges-1 ...
		{
			NOTIFY(PDDocWillPrintPages)(doc, fromPage, toPage, psLevel, binaryOK);
			for pageNum = ranges[i]->startPage to ranges[i]->lastPage ...
			{
	*/


	/**
		(Optional) A callback for PDPrintClient. It is called once for each
		page printed.

		<p>A client can use this notification to update the page number
		in the message pane, for example.</p>
		@param pageNum IN/OUT The number of the page to print.
		@param printClient IN/OUT The PDPrintClient from which this is
		invoked.
		@see PDDocPrintPages
	*/
	ACCBPROTO1 void (ACCBPROTO2 *NotifyNewPage)	(ASInt32 pageNum, PDPrintClient printClient);



	/**
		(Optional) A callback for PDPrintClient. It is called before a page
		begins printing.
		@param stm IN/OUT The PostScript print stream for the current
		page.
		@param pageNum IN/OUT The number of the page being printed.
		@param pageTree IN/OUT Must be <code>NULL</code>.
		@param printClient IN/OUT The PDPrintClient from which this is
		invoked.
		@see PDDocPrintPages
	*/
	ACCBPROTO1 void (ACCBPROTO2 *PageBegin)		
		(ASStm stm, ASInt32 pageNum, const PDResTree pageTree, PDPrintClient printClient);


	/**
		(Optional) A callback for PDPrintClient. It is called before a page
		is printed to set it up.
		@param stm IN/OUT The PostScript print stream for the current
		page.
		@param pageNum IN/OUT The number of the page to set up.
		@param printClient IN/OUT The PDPrintClient from which this is
		invoked.
		@return <code>true</code> on success; otherwise an error is raised.
		@see PDDocPrintPages
	*/
	ACCBPROTO1 ASBool (ACCBPROTO2 *PageSetup) (ASStm stm, ASInt32 pageNum, PDPrintClient printClient);


	/**
		A callback for PDPrintClient. PageSetupPostResource() is called
		from PDPrintPageBegin() after the resource tree has been processed
		(all fonts and other resources emitted into the PostScript
		stream).
		@param stm IN/OUT The PostScript print stream for the current
		page.
		@param pageNum IN/OUT The number of the page to set up.
		@param printClient IN/OUT The PDPrintClient from which this is
		invoked.
		@see PDDocPrintPages
	*/
	ACCBPROTO1 void (ACCBPROTO2 *PageSetupPostResource) (ASStm stm, ASInt32 pageNum, PDPrintClient printClient);


	/**
		(Optional) A callback for PDPrintClient. It mirrors PageSetup;
		PageCleanup is called before we clean our dictionaries.
		@param stm IN/OUT The PostScript print stream for the current
		page.
		@param pageNum IN/OUT The number of the page being printed.
		@param printClient IN/OUT The PDPrintClient from which this is
		invoked.
		@see PDDocPrintPages
	*/
	ACCBPROTO1 void (ACCBPROTO2 *PageCleanup) (ASStm stm, ASInt32 pageNum, PDPrintClient printClient);


	/**
		(Optional) A callback for PDPrintClient. It emits the page contents
		for printing. It broadcasts a notification that the page has
		been printed:

		<p><code>NOTIFY( PDDocDidPrintPage) (doc, page, stm, error)</code></p>

		<p>There is a default callback for PostScript printing.</p>

		@param stm IN/OUT The PostScript print stream for this page.
		@param pageNum IN/OUT The number of the page whose contents is
		emitted.
		@param pdPage IN/OUT The PDPage to print.
		@param printClient IN/OUT The PDPrintClient from which this is
		invoked.
		@see PDDocPrintPages
	*/
	ACCBPROTO1 void (ACCBPROTO2 *EmitPageContents)
		(ASStm stm, ASInt32 pageNum, PDPage pdPage, PDPrintClient printClient);


	/**
		(Optional) A callback for PDPrintClient. It is called after a page
		has printed. It broadcasts a notification that the page has printed:

		<p><code>NOTIFY( PDDocDidPrintPages) (doc, fromPage, toPage, error);</code></p>

		@param stm IN/OUT The PostScript print stream for the current
		page.
		@param pageNum IN/OUT The number of the page that just printed.
		@param veryLastPage IN/OUT <code>true</code> only for the last page of the
		very last range, <code>false</code> otherwise.
		@param printClient IN/OUT The PDPrintClient from which this is
		invoked.
		@see PDDocPrintPages
	*/
	ACCBPROTO1 void (ACCBPROTO2 *PageEnd)
		(ASStm stm, ASInt32 pageNum, ASBool veryLastPage, PDPrintClient printClient);


	/**
		(Optional) A callback for PDPrintClient. It ends processing a
		document for printing.
		@param stm IN/OUT The PostScript print stream.
		@param printClient IN/OUT The PDPrintClient from which this is
		invoked.
		@see PDDocPrintPages
	*/
#if MAC_PLATFORM
/** In order for the Mac platform to emit color comments, the controller needs to be passed to
the <code>DocEnd</code> client callback. The client then calls PDPrintFillColorUsageBuf() to fill in the 
data for the injection point used by the print manager to emit the custom color comments.
*/
	ACCBPROTO1 void (ACCBPROTO2 *DocEnd) (ASStm stm, PDPrintClient printClient, PDPrintController controller);
#else
	ACCBPROTO1 void (ACCBPROTO2 *DocEnd) (ASStm stm, PDPrintClient printClient);
#endif
	/*******************	END OF CONTROL STRUCTURE	*************************/


	/** This routine is called to ask the client what encoding method should be used for the font.
		This cannot be <code>NULL</code> for PostScript printing.
	 */
	PDPrintGetFontEncodingMethodProc GetFontEncodingMethod;


	/**
		(Optional) A callback for PDPrintClient. It asks the client what
		the VM usage is for the font.
		@param stm IN/OUT The PostScript print stream.
		@param printClient IN/OUT The PDPrintClient from which this is
		invoked.
		@return VM usage, in bytes.
		@see PDDocPrintPages
	*/
	ACCBPROTO1 ASInt32 (ACCBPROTO2 *GetFontVMUsage)(PDFont fontP, PDPrintClient printClient);



	/**
		(Optional) A callback for PDPrintClient. It is called before a font
		is emitted (outside DSC comments).
		@param stm IN/OUT The PostScript print stream for the current
		page.
		@param fontP IN/OUT The font to emit.
		@param printClient IN/OUT The PDPrintClient from which this is
		invoked.
		@see PDDocPrintPages
	*/
	ACCBPROTO1 void (ACCBPROTO2 *EmitPSFontBegin)(ASStm stm, PDFont fontP, PDPrintClient printClient);


	/**
		(Optional) A callback for PDPrintClient. It is called after a font
		is emitted (outside DSC comments).
		@param stm IN/OUT The PostScript print stream for the current
		page.
		@param fontP IN/OUT The font that was emitted.
		@param printClient IN/OUT The PDPrintClient from which this is
		invoked.
		@see PDDocPrintPages
	*/
	ACCBPROTO1 void (ACCBPROTO2 *EmitPSFontEnd)(ASStm stm, PDFont fontP, PDPrintClient printClient);



	/**
		(Optional) A callback for PDPrintClient. It is called before a font
		encoding is emitted.
		@param stm IN/OUT The PostScript print stream for the current
		page.
		@param fontP IN/OUT The font whose encoding is emitted.
		@param printClient IN/OUT The PDPrintClient from which this is
		invoked.
		@return Must be <code>true</code>.
		@see PDDocPrintPages
	*/
	ACCBPROTO1 ASBool (ACCBPROTO2 *EmitPSFontEncodingBegin)(ASStm stm, PDFont fontP, PDPrintClient printClient);


	/**
		(Optional) A callback for PDPrintClient. It is called after a font
		encoding is emitted.
		@param stm IN/OUT The PostScript print stream for the current
		page.
		@param fontP IN/OUT The font whose encoding was emitted.
		@param printClient IN/OUT The PDPrintClient from which this is
		invoked.
		@see PDDocPrintPages
	*/
	ACCBPROTO1 void (ACCBPROTO2 *EmitPSFontEncodingEnd)(ASStm stm, PDFont fontP, PDPrintClient printClient);



	/**
		(Optional) A callback for PDPrintClient. It is called before a PostScript
		resource is emitted.
		@param stm IN/OUT The PostScript print stream for the current
		page.
		@param resType IN/OUT An ASAtom representing the name of the resource
		type, such as ColorSpace or XObject, as defined in section
		3.7.2 of the <i>PDF Reference</i>.
		@param resName IN/OUT Name of the resource in the current page.

		@param resObj IN/OUT The resource's Cos object.
		@param printClient IN/OUT The PDPrintClient from which this is
		invoked.
		@see PDDocPrintPages
	*/
	ACCBPROTO1 void (ACCBPROTO2 *EmitPSResourceBegin)(ASStm stm, ASAtom resType, const char *resName, CosObj resObj, PDPrintClient printClient);


	/**
		(Optional) A callback for PDPrintClient. It is called after a PostScript
		resource is emitted.
		@param stm IN/OUT The PostScript print stream for the current
		page.
		@param resType IN/OUT An ASAtom representing the name of the resource
		type, such as ColorSpace or XObject as defined in section
		3.7.2 of the <i>PDF Reference</i>.
		@param resName IN/OUT The name of the resource in the current page.

		@param resObj IN/OUT The resource's Cos object.
		@param printClient IN/OUT The PDPrintClient from which this is
		invoked.
		@see PDDocPrintPages
	*/
	ACCBPROTO1 void (ACCBPROTO2 *EmitPSResourceEnd)(ASStm stm, ASAtom resType, const char *resName, CosObj resObj, PDPrintClient printClient);


	/**
		A callback for PDPrintClient. <code>ShouldCancel</code> is called at least
		once per page. It should return <code>true</code> if the print job should
		be cancelled (typically, the user hit a key combination to
		cancel printing).
		@param printClient IN/OUT The PDPrintClient from which this is
		invoked.
		@return See above.
		@see PDDocPrintPages
	*/
	ACCBPROTO1 ASBool (ACCBPROTO2 *ShouldCancel)(PDPrintClient printClient);


	/**
		A callback for PDPrintClient. <code>CancelPrintJob</code> is called to
		actually cause the print job to be cancelled. It should
		stop further printing and preferably should remove the incomplete
		print job.
		@param printClient IN/OUT The PDPrintClient from which this is
		invoked.
		@return Returns <code>true</code> if the print job was successfully cancelled.

		@see PDDocPrintPages
	*/
	ACCBPROTO1 ASBool (ACCBPROTO2 *CancelPrintJob)(PDPrintClient printClient);


	/**
		(Required) A callback for PDPrintClient. It is called when the ASStm
		buffer is full. Buffered data should be sent to the printer,
		driver, file, or other destination. Not used for non-PostScript
		printing.
		@param data IN/OUT A pointer to data to send.
		@param nData IN/OUT The length of <code>data</code>, in bytes.
		@param printClient IN/OUT The PDPrintClient from which this is
		invoked.
		@see PDDocPrintPages
	*/
	ACCBPROTO1 ASInt32 (ACCBPROTO2 *FlushString) (char *data, ASInt32 nData, PDPrintClient printClient);


	/**
		(Optional) A callback for PDPrintClient. It emits a prolog string.

		<p>It defaults to a method that calls FlushString().</p>

		@param s IN/OUT A pointer to the prolog string to emit.
		@param nData IN/OUT The length of <code>s</code> in bytes.
		@param printClient IN/OUT The PDPrintClient from which this is
		invoked.
		@return If successful, the result is greater than zero. If it is unsuccessful,
		the result is negative.
		@see PDDocPrintPages
	*/
	ACCBPROTO1 ASInt32 (ACCBPROTO2 *EmitPrologString) (const char *s, ASInt32 nData, PDPrintClient printClient);

	/** This routine is called to determine whether a font can be emitted into the print job.
	 This is used to determine whether a font is a document-included resource.
	 It is only used for PostScript.
	 <p>OPTIONAL: The default is to assume that any font can be emitted.</p>
	 */
	PDPrintCanEmitFontProc CanEmitFont;


	/** REQUIRED. This routine is called to emit a font.  For Type0 fonts that require font substition, this
	   routine may emit multiple font definitions. The caller can get the list of fonts used by
	   calling GetFontComponentList().
	*/
	PDPrintEmitFontProc EmitFont;



	/**
		(Required) UndefineFont() is called to remove the font from
		the list of fonts that have been downloaded and to undefine
		the font from PostScript VM. It may be called during a page,
		at the end of the page, or between pages. After this routine
		is called, if the font is required again, the EmitFont() routine
		will be called.
		@param stm IN/OUT The stream to which the PostScript code to undefine
		the font will be written.
		@param fontP IN/OUT The PDFont for the font to be removed.
		@param vmUndefine IN/OUT If <code>true</code> and this is a level 2 or greater
		PostScript interpreter, PostScript code will be emitted
		to undefine the font. If <code>false</code>, then it will just be removed
		from the list of fonts that have already been downloaded.

		@param printClient IN/OUT The PDPrintClient from which this is
		invoked.
		@return <code>true</code> on success.
		@see PDDocPrintPages
	*/
	ACCBPROTO1 ASBool (ACCBPROTO2 *UndefineFont)(
		ASStm stm, PDFont fontP,  ASBool vmUndefine, PDPrintClient client);


	/**
		(Optional) A callback for PDPrintClient. EmitFontProcSet() is
		called while emitting the document prolog. It should emit
		any procsets required to download the font. The routine
		will be called once for each font in the document. However,
		each procset only needs to be downloaded once per document.

		@param stm IN/OUT The stream where the procset should be written.

		@param fontP IN/OUT The PDFont for the font.
		@param printClient IN/OUT The PDPrintClient from which this is
		invoked.
		@return <code>false</code> if the PDPrintClient <code>fontDownloadContext</code> is not specified;
		<code>true</code> otherwise.
		@see PDDocPrintPages
	*/
	ACCBPROTO1 ASBool (ACCBPROTO2 *EmitFontProcSet)(
		ASStm stm, PDFont fontP, PDPrintClient client);


	/**
		A callback for PDPrintClient. It emits glyphs incrementally.
		@param stm IN/OUT The PostScript print stream for this page.
		@param fontP IN/OUT The PDFont for the font.
		@param srcStr IN/OUT The source string.
		@param dstStr IN/OUT The destination string.
		@param srcBytesUsedP IN/OUT The source bytes used.
		@param dstBytesUsedP IN/OUT The destination bytes used.
		@param glyphCount IN/OUT The number of glyphs.
		@param fontIndexP IN/OUT The font index.
		@param printClient IN/OUT The PDPrintClient from which this is
		invoked.
		@return <code>true</code> if the glyphs successfully download, <code>false</code> otherwise.

		@see PDDocPrintPages
		@see PDFontPSEmitGlyphsIncr
	*/
	ACCBPROTO1 ASBool (ACCBPROTO2 *EmitGlyphsIncr)(
		ASStm stm, PDFont fontP, PDPrintStrP srcStr, PDPrintStrP dstStr,
		ASUns32 *srcBytesUsed, ASUns32 *dstBytesUsed, ASUns32 *glyphCount, ASUns16 *fontIndex, PDPrintClient printClient);


	/**
		(Required) A callback for PDPrintClient. GetComponentFontList()
		is called to get the list of component font names used for
		substitution. For fonts that do not require substitution
		or are substituted by only one font, the list returned should
		only have one element. For a Type 0 font that requires several
		fonts for font substitution, the list will have multiple
		fonts. The font names returned are the names used by the
		font definitions downloaded to PostScript. The font definitions
		themselves are downloaded by a call to the EmitFont() routine.

		@param fontP IN/OUT The font.
		@param fontList IN/OUT The font array.
		@param printClient IN/OUT The PDPrintClient from which this is
		invoked.
		@see PDDocPrintPages
	*/
	ACCBPROTO1 void (ACCBPROTO2 *GetComponentFontList)(PDFont fontP, PDPrintFontArrayP fontList, PDPrintClient printClient);



	/**
		(Required) A callback for PDPrintClient. FlushIncrGlyphList()
		is called to flush the list of glyphs that have been incrementally
		download since the calls to EmitFont or the last call of
		FlushIncrGlyphList. This routine will be called if the caller
		generates PostScript code that would flush glyphs procedures
		that have been downloaded to the PostScript interpreter.
		For example, if at the beginning of each page, a save is
		executed and then at the end of a page, a restore is executed,
		any glyph procedures downloaded after the beginning of the
		page will be removed when the restore is executed.

		<p>The call to FlushIncrGlyphList() indicates that all the glyphs
		need to be redownloaded, by calls to EmitGlyphsIncr().</p>
		@param stm IN/OUT The PostScript print stream.
		@param printClient IN/OUT The PDPrintClient from which this is
		invoked.
		@see PDDocPrintPages
	*/
	ACCBPROTO1 void (ACCBPROTO2 *FlushIncrGlyphList)(	ASStm stm,  PDPrintClient printClient);



	/**
		(Optional) A callback for PDPrintClient. If this method is
		supplied, then during PostScript printing, it will be called
		when a form or image containing an OPI dictionary is encountered.
		If it returns <code>true</code>, then the client is presumed to have
		taken care of the entire form or image, and PDFL will
		emit nothing. Otherwise, PDFL will generate OPI comments
		based on the dictionary.

		@note The method may not store the <code>OPIdict</code> pointer or any
		pointers within it.
		@param opi IN/OUT The OPI dictionary.
		@param stm IN/OUT The PostScript print stream.
		@param clientData IN/OUT Client data.
		@return See above.
		@see PDDocPrintPages
	*/
	ACCBPROTO1 ASBool (ACCBPROTO2 *OPIhandler)
		(OPIdict *opi, ASStm stm, void *clientData);


	/**
		(Optional) If this method is supplied, and the <code>emitExternalStreamRef</code>
		parameter in the PDPrintParamsRec is <code>true</code>, then during PostScript
		printing this method will be called when an external stream
		is encountered. If the method is not supplied, or if it
		returns <code>NULL</code>, then the simple path name will be written
		as the <code>DataSource</code> for the stream; otherwise, the returned
		string will be written as the <code>DataSource</code>.

		<p>The DataSource is used thus:</p>

		<p><code>/ DataSource (xxxxx) (r) file</code></p>

		<p>where <code>xxxxx</code> is the returned string or the simple path name.</p>


		@note The method may not store the <code>PDFileSpec</code> pointer or
		any pointers (strings) within it.
		@param f IN/OUT An object of type PDFileSpec.
		@param buf IN/OUT (Filled by the callback) The buffer.
		@param bufSize IN/OUT The buffer size.
		@param clientData IN/OUT Client data.
		@return The length of the string written into <code>buf</code>.
		@see PDDocPrintPages
	*/
	ACCBPROTO1 ASUns32 (ACCBPROTO2 *DataSourceString)
		(PDFileSpec f, char *buf, ASUns32 bufSize, void *clientData);


	/** The current page, starting at <code>1</code>. */
	ASInt32		sequentialPageNum;	

	/**	Data for the client's use. This data is passed to the
		<code>OPIhandler</code> and <code>DataSourceString</code> methods. 
	*/
	void	*clientData;			

	/** For the client's use to hold a font download context for the
		EmitFontProcSet.
	*/
	void			*fontDownloadContext;	


	/** The amount of virtual memory used. It is incremented by
		routines that download fonts and resources.
	*/
	ASInt32		vmUsed; 


	/**	Optional. If <code>ProgressMonitor</code>, then during PostScript printing,
		it will be called when page uses the flattener to print.
	*/
	ACCBPROTO1 ASBool (ACCBPROTO2 *ProgressMonitor)(ASInt32 pageNum, ASInt32 totalPages, float current, const char* name, ASInt32 stage, void *progMonClientData);

	/** */
	void *progMonClientData;

	/** For use in the Exchange product, when emitting the time/date of the print job (the <code>PageInfo</code> mark for separations),
		this callback takes an ASText object and the client side fills in the <code>timeDate</code> with a localized 
		representation of the current date and time. This could be called from the Tiling code as well.
	*/
	ACCBPROTO1 void (ACCBPROTO2 *GetTimeDate)(ASText timeDate);

	/** Pro product only.
		@product_exclude RDR
		@product_exclude STD
	*/
	PDHostSepsSpec	seps;

/*#ifdef TOOLKIT */
	/* For the PDF Library SDK, allow the client to specify a page size when creating a PostScript file.
	*/
	/** For internal use of PDFL. SDK clients must set this parameter to <code>NULL</code>.
	*/
	ACCBPROTO1 void (ACCBPROTO2 *GetClientPaperSize)(PDPrintClient printClient, ASFixed* height, ASFixed* width);
/*#endif */
	/** Output stream for creating postscript file.
	*/
	ASStm outStm;

} PDPrintClientRec;

/** The structure holding pointers to certain optional Callbacks available 
	with the PDFLPrintDoc() API. */

typedef struct _t_PDFLPrintUserCallbacks{
	ASSize_t  size;
	PDPrintCanEmitFontProc CanEmitFont;
}PDFLPrintUserCallbacksRec;

typedef struct _t_PDFLPrintUserCallbacks *PDFLPrintUserCallbacks;

/** Declare the the type PDFLPrintUserParams, which is a pointer to a structure and is
   passed into PDFLPrintDoc. This structure is defined in <code>PDFLPrint.h</code>, but it is complex
   and platform-specific. This declaration avoids the need to include the platform specific
   details into files that just need a decaration of this pointer. */
typedef struct _t_PDFLPrintUserParams *PDFLPrintUserParams;

/** Declare the type PDOCFindOutZoomProc, which is a callback that lets the client set the current user interface zoom level.*/
typedef ACCBPROTO1 float (ACCBPROTO2 *PDOCFindOutZoomProc)(PDDoc pdDoc);

/** Declare the type PDOCFindOutLanguageProc, which is a callback that lets the client set the current user interface language level.*/
typedef ACCBPROTO1 char *(ACCBPROTO2 *PDOCFindOutLanguageProc)();

/** Declare the type PDOCFindOutUserProc, which is a callback that lets the client set the current user interface user level.*/
typedef ACCBPROTO1 ASText (ACCBPROTO2 *PDOCFindOutUserProc)(ASAtom indTtlOrOrg);

/** Declare the type PDOCFindOutAutoStatePrefProc, which is a callback that lets the client set the AutoState preference.*/
typedef ACCBPROTO1 ASBool (ACCBPROTO2 *PDOCFindOutAutoStatePrefProc)();

/** Flags for CosDocSave saveFlags parameter.  This is specific to the PDF Library. */
#define cosSaveWriteXref	0x20	/* write the xref (for Forms) */

#ifdef __cplusplus
}
#endif

#if defined( WINDOWS ) || defined( WIN_ENV )
#pragma pack (pop, peexpt )		/* reset to /Zp  */
#endif

#endif /* _H_PDFLExpT */
