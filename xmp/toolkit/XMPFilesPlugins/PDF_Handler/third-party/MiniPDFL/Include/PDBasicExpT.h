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

 PDBasicExpT.h

 - Types required to use the PDModel HFT. ONLY handles to exported
   types are defined in this file.

*********************************************************************/

#ifndef _BASIC_EXP_T_
#define _BASIC_EXP_T_ 1

#ifdef __cplusplus
extern "C" {
#endif

#ifndef _T_PDDOC_
#define _T_PDDOC_

/** The underlying PDF representation of a document. There is a correspondence
	between a PDDoc and an ASFile; the PDDoc object is the hidden object behind
	every AVDoc. An ASFile may have zero or more underlying files, so a PDF file
	does not always correspond to a single disk file. For example, an ASFile may
	provide access to PDF data in a database.

	<p>Through PDDoc objects, your application can perform most of the menu items for pages
	from Acrobat (delete, replace, and so on). Thumbnails can be created and deleted
	through this object. You can set and retrieve document information fields through this
	object as well. The first page in a PDDoc is page <code>0</code>.</p>
	@see AVDocGetPDDoc
	@see PDDocFromCosDoc
	@see PDDocOpen
	@see PDDocOpenFromASFile
	@see PDDocOpenWithParams
	@see PDDocCreate
	@see PDPageGetDoc
	@see PDFileSpecGetDoc
	@see PDEnumDocs
	@see PDDocClose
	@see PDDocRelease
	@see PDDocEnumFonts
	@see PDDocEnumLoadedFonts
	@see PDEnumDocs
*/
typedef struct _t_PDDoc *PDDoc;
#endif

#ifndef _T_PDPAGE_
#define _T_PDPAGE_

/** A single page in the PDF representation of a document. Just as PDF files are partially
	composed of their pages, PDDoc objects are composed of PDPage objects. A page contains a
	series of objects representing the objects drawn on the page (PDGraphic), a list of
	resources used in drawing the page, annotations (PDAnnot), an optional thumbnail
	image of the page, and the beads used in any articles that occur on the page. The first
	page in a PDDoc is page <code>0</code>.

	@see PDDocCreatePage
	@see PDBeadAcquirePage
	@see PDDocAcquirePage
	@see AVPageViewGetPage
	@see PDDocDeletePages
	@see PDPageRelease
*/
typedef struct _t_PDPage *PDPage;
#endif

#ifndef _T_PDFONT_
#define _T_PDFONT_

/** A font that is used to draw text on a page. It corresponds to a Font Resource in a PDF
	file. Applications can get a list of PDFont objects used on a PDPage or a range of PDPage objects.
	More than one PDPage may reference the same PDFont object.
	A PDFont has a number of attributes whose values can be read or set, including an
	array of widths, the character encoding, and the font's resource name.

	@see PDDocEnumFonts
	@see PDDocEnumLoadedFonts
	@see PDFontGetDescendant
	@see PDStyleGetFont
	@see PDDocEnumFonts
	@see PDFontEnumCharProcs
*/
typedef struct _t_PDFont *PDFont;
#endif

#ifndef _T_PDCONTENT_
#define _T_PDCONTENT_

/** A pointer to a PDContent <code>struct</code>. */
typedef struct _t_PDContent *PDContent;
#endif

#ifndef _T_PDTHUMB_
#define _T_PDTHUMB__

/** A thumbnail preview image of a page.
	@see PDDocCreateThumbs
	@see PDDocDeleteThumbs
*/
typedef struct _t_PDThumb *PDThumb;
#endif

#ifndef _T_PDTEXTSELECT_
#define _T_PDTEXTSELECT_

/** A pointer to a PDTextSelect <code>struct</code>. */
typedef struct _t_PDTextSelect *PDTextSelect;
#endif /* _T_PDTEXTSELECT_ */

#ifndef _T_PDRESTRE_
#define _T_PDRESTRE_

/** A selection of text on a single page that may contain more than one disjoint group of
words. A text selection is specified by one or more ranges of text, with each range
containing the word numbers of the selected words. Each range specifies a start and
end word, where <i>"start"</i> is the first of a series of selected words and <i>"end"</i> is the first
word not in the series.
	@see AVDocGetSelection
	@see AVPageViewTrackText
	@see PDDocCreateTextSelect
	@see PDTextSelectCreatePageHilite
	@see PDTextSelectCreatePageHiliteEx
	@see PDTextSelectCreateWordHilite
	@see PDTextSelectCreateWordHiliteEx
	@see PDTextSelectCreateRanges
	@see PDTextSelectCreateRangesEx
	@see PDTextSelectDestroy
	@see PDTextSelectEnumQuads
	@see PDTextSelectEnumText
*/
typedef struct _t_PDResTree *PDResTree;
#endif /* _T_PDRESTRE_ */

#ifdef __cplusplus
}
#endif

#endif /* _POQUITO_EXP_T_ */

