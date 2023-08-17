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

 PSFProcs.h

 - Catalog of functions exported by the PDSysFont HFT.

*********************************************************************/


/**
	Enumerates all of the system fonts with a user-supplied 
	procedure. 

	<p>The PDSysFont must be acquired during the enumeration if 
	the font is needed beyond the <code>enumProc</code>. </p>

	<p>Developers should not assume that the <code>enumProc</code> will be 
	called. If no system fonts are found (for example, if the 
	<code>PSRESOURCEPATH</code> environment variable is not set on UNIX platforms), 
	<code>enumProc</code> is never called, and PDEnumSysFonts() does not raise 
	an exception. </p>

	@note The font names that are returned from the methods 
	PDEnumSysFonts() and PDSysFontsGetAttrs() are different in 5.0 
	(compared to 4.05). The differences are shown in the table 
	below:

	<TABLE rules="all" cellspacing="1">
	<TR><TH>Acrobat 4.05 Name</TH><TH>Acrobat 4.05 PSname</TH><TH>Acrobat 5.0 Name</TH><TH>Acrobat 5.0 Psname</TH></TR>
	<TR><TD><code>MS-Mincho</code></TD><TD><code>NULL</code></TD><TD><code>MSMincho</code></TD><TD><code>MS-Mincho</code></TD></TR>
	<TR><TD><code>MS-Gothic</code></TD><TD><code>NULL</code></TD><TD><code>MSGothic</code></TD><TD><code>MS-Gothic</code></TD></TR>
	<TR><TD><code>MS-PMincho</code></TD><TD><code>NULL</code></TD><TD><code>MSPMincho</code></TD><TD><code>MS-PMincho</code></TD></TR>
	<TR><TD><code>MS-PGothic</code></TD><TD><code>NULL</code></TD><TD><code>MSPGothic</code></TD><TD><code>MS-PGothic</code></TD></TR>
	<TR><TD><code>MS-UIGothic</code></TD><TD><code>NULL</code></TD><TD><code>MSUIGothic</code></TD><TD><code>MS-UIGothic</code></TD></TR>
	</TABLE>

	@param enumProc IN/OUT A user-supplied callback to call once for 
	each system font. Enumeration continues until all fonts 
	have been enumerated, or until <code>enumProc</code> returns <code>false</code>. 
	@param clientData IN/OUT A pointer to user-supplied data to pass 
	to <code>enumProc</code> each time it is called. 
	@see PDFindSysFont 
	@see PDFindSysFontForPDEFont 
	@ingroup Enumerators
	@since PI_PDSYSFONT_VERSION >= 0x00040000
*/
UNPROC (void, PDEnumSysFonts, (
		IN		PDSysFontEnumProc enumProc, 
		IN		void *clientData)
	)

/**
	Finds a system font that matches the requested attributes. 
	
	<p>The method gets the PDSysFont rather than acquiring it, so 
	do not call PDERelease() on the returned PDSysFont when you are done 
	with it. </p>

	@param attrs IN/OUT A pointer to a PDEFontAttrs structure with the 
	attributes of the font you are searching for. 
	@param attrsSize IN/OUT The size of the <code>attrs</code> buffer in bytes. 
	@param flags IN/OUT Flags from PDSysFontMatchFlags. 
	@return The desired system font. 
	@see PDEnumSysFonts 
	@see PDFindSysFontForPDEFont 
	@see PDFindSysFontEx 
	@since PI_PDSYSFONT_VERSION >= 0x00040000

*/
UNPROC (PDSysFont, PDFindSysFont, (
		IN		PDEFontAttrsP attrs,
		IN		ASUns32 attrsSize,
		IN		ASUns32 flags)
	)


/**
	Finds a system font that matches the requested attributes. 
	
	<p>If the requested font is a multiple master font instance, 
	the base font is returned, and the specified design vector 
	is decoded and returned in <code>mmDesignVector</code>. </p>

	<p>The method gets the PDSysFont rather than acquiring it, so 
	do not call PDERelease() on the returned PDSysFont when done 
	with it. </p>

	@param attrs IN/OUT A pointer to a PDEFontAttrs structure with the 
	attributes of the font you are searching for. 
	@param attrsSize IN/OUT The size of the <code>attrs</code> buffer in bytes. 
	@param flags IN/OUT Flags from PDSysFontMatchFlags. 
	@param mmDesignVector IN/OUT (Filled by the method) If the requested 
	font is a multiple master font instance, the specified design 
	vector is decoded and returned in <code>mmDesignVector</code>. 
	@param designVecLength IN/OUT (Filled by the method) Pass the 
	length of <code>mmDesignVector</code>. This parameter also returns the 
	number of elements filled in <code>mmDesignVector</code> (the maximum is <code>4</code>). 
	
	@return The desired system font. 
	@see PDEnumSysFonts 
	@see PDFindSysFont 
	@see PDFindSysFontForPDEFont 
	@since PI_PDSYSFONT_VERSION >= 0x00040000

*/
UNPROC (PDSysFont, PDFindSysFontEx, (
		IN		PDEFontAttrsP attrs,
		IN		ASUns32 attrsSize,
		IN		ASUns32 flags,
		OUT		ASFixed *mmDesignVector,
		OUT		ASInt32 *designVecLength)
	)

/**
	Find a system font that matches the requested PDEFont. 

	<p>The method gets the PDSysFont rather than acquiring it, so 
	do not call PDERelease() on the returned PDSysFont when done 
	with it. </p>

	@param font A PDEFont whose matching system font is found. 
	
	@param flags A bit field comprised of PDSysFontMatchFlags 
	values.
	<ul>
	<li> kPDSysFontMatchNameAndCharSet</li>
	<li> kPDSysFontMatchFontType</li>
	<li> PDSysFontMatchFlags</li>
	<li> Passing zero matches the font by name only.</li>
	</ul>	

	@return The system font corresponding to <code>font</code>. 
	@exception peErrCantGetAttrs 
	@exception genErrBadParm 
	@exception genErrResourceLoadFailed 
	@see PDFindSysFont 
	@see PDEFontGetSysFont 
	@since PI_PDSYSFONT_VERSION >= 0x00050000
*/
UNPROC (PDSysFont, PDFindSysFontForPDEFont, (
		IN		PDEFont font, 
		IN		ASUns32 flags)
	)

/**
	Gets the attributes of a system font. 

	<p>The attributes will be returned in the buffer pointed to 
	by <code>attrsP</code>. </p>

	<p>No more than <code>attrsSize</code> bytes will be written to the buffer. </p>
	
	<p>This call can be expensive to execute, as it may involve 
	parsing the font in order to determine attributes. </p>

	@note The font names that are returned from the methods 
	PDEnumSysFonts and PDSysFontsGetAttrs are different in 5.0 
	(compared to 4.05). The differences are shown in the table 
	below:


	<TABLE rules="all" cellspacing="1">
	<TR><TH>Acrobat 4.05 Name</TH><TH>Acrobat 4.05 PSname</TH><TH>Acrobat 5.0 Name</TH><TH>Acrobat 5.0 Psname</TH></TR>
	<TR><TD><code>MS-Mincho</code></TD><TD><code>NULL</code></TD><TD><code>MSMincho</code></TD><TD><code>MS-Mincho</code></TD></TR>
	<TR><TD><code>MS-Gothic</code></TD><TD><code>NULL</code></TD><TD><code>MSGothic</code></TD><TD><code>MS-Gothic</code></TD></TR>
	<TR><TD><code>MS-PMincho</code></TD><TD><code>NULL</code></TD><TD><code>MSPMincho</code></TD><TD><code>MS-PMincho</code></TD></TR>
	<TR><TD><code>MS-PGothic</code></TD><TD><code>NULL</code></TD><TD><code>MSPGothic</code></TD><TD><code>MS-PGothic</code></TD></TR>
	<TR><TD><code>MS-UIGothic</code></TD><TD><code>NULL</code></TD><TD><code>MSUIGothic</code></TD><TD><code>MS-UIGothic</code></TD></TR>
	</TABLE>

	@param sysFont IN/OUT A PDSysFont object referencing a system 
	font whose attributes are obtained. 
	@param attrsP IN/OUT (Filled by the method) A pointer to a PDEFontAttrs 
	structure with the attributes of a system font. 
	@param attrsSize IN/OUT The size of the <code>attrsP</code> buffer in bytes. 
	@exception peErrCantGetAttrs 
	@see PDSysFontGetEncoding 
	@see PDSysFontGetInfo 
	@see PDSysFontGetName 
	@see PDSysFontGetType0Widths 
	@since PI_PDSYSFONT_VERSION >= 0x00040000

*/
UNPROC (void, PDSysFontGetAttrs, (
		IN		PDSysFont sysFont, 
		OUT		PDEFontAttrsP attrsP, 
		IN		ASUns32 attrsSize)
	)

/**
	Gets the widths of a single byte encoded system font. 
	@param sysFont IN/OUT A PDSysFont object referencing a system 
	font whose widths are obtained. 
	@param widthsP IN/OUT (Filled by the method) A pointer to the widths 
	array. <code>widthsP</code> must have room for 256 entries. 
	@exception peErrCantGetWidths 
	@see PDSysFontGetType0Widths 
	@see PDSysFontGetWidthsEx 
	@see PDFontGetWidths 
	@since PI_PDSYSFONT_VERSION >= 0x00040000

*/
UNPROC (void, PDSysFontGetWidths, (
		IN		PDSysFont sysFont, 
		OUT		ASInt16 *widthsP)
	)

/**
	Gets the widths of a single-byte encoded system font. 
	@param sysFont IN/OUT A PDSysFont object referencing a system 
	font whose widths are obtained. 
	@param widthsP IN/OUT (Filled by the method) A pointer to the widths 
	array. <code>widthsP</code> must have room for 256 entries. 
	@param mmDesignVector IN/OUT If <code>sysFont</code> is a multiple master font, 
	it points to the design vector, whose length must equal the 
	number of design axes of <code>sysFont</code>. 
	@exception peErrCantGetWidths 
	@see PDSysFontGetType0Widths 
	@see PDSysFontGetWidths 
	@see PDFontGetWidths 
	@since PI_PDSYSFONT_VERSION >= 0x00040000

*/
UNPROC (void, PDSysFontGetWidthsEx, (
		IN		PDSysFont sysFont, 
		OUT		ASInt16 *widthsP,
		IN		ASFixed *mmDesignVector)
	)

/**
	Gets the encoding of a single-byte encoded system font. 
	
	@param sysFont A PDSysFont object referencing a system 
	font whose encoding is obtained. 
	@param encodingNameP (Filled by the method) An encoding 
	name if the standard encoding is used. For Windows, it is <code>WinAnsiEncoding</code>; for Mac OS, this 
	is <code>MacRomanEncoding</code>.
	@return If the return value is non-<code>NULL</code>, it is a pointer to an encoding 
	array of 256 C strings. Each entry in the array either contains 
	a glyph name or <code>NULL</code>; if it is <code>NULL</code>, the corresponding entry uses 
	the font's built in encoding value. 

	<p>The returned encoding must be freed via a call to ASfree(). </p>

	@see PDSysFontAcquirePlatformData 
	@see PDSysFontGetInfo 
	@see PDSysFontGetName 
	@see PDSysFontGetType0Widths 
	
	@note This encoding array is returned only on Mac 
	platforms; on all others the function returns <code>NULL</code>. 
	@since PI_PDSYSFONT_VERSION >= 0x00040000
*/
UNPROC (ASUns8 **, PDSysFontGetEncoding, (
		IN		PDSysFont sysFont, 
		OUT		ASAtom *encodingNameP)
	)

/**
	Gets high-level information about a system font. 
	@param sysFont A PDSysFont object referencing a system 
	font whose information is obtained. 
	@param infoP (Filled by the method) A pointer to PDEFontInfoP 
	structure to fill with font information for <code>sysFont</code>. No 
	more than <code>infoSize</code> bytes are written to this buffer. 
	@param infoSize The size of the <code>infoP</code> buffer in bytes.
	@see PDSysFontAcquirePlatformData 
	@see PDSysFontGetEncoding 
	@see PDSysFontGetName 
	@see PDSysFontGetType0Widths 
	@since PI_PDSYSFONT_VERSION >= 0x00040000
*/
UNPROC (void, PDSysFontGetInfo, (
		IN		PDSysFont sysFont, 
		OUT		PDEFontInfoP infoP, 
		IN		ASUns32 infoSize)
	)

/**
	Gets the PostScript or TrueType styled name for a system 
	font. 
	@param sysFont IN/OUT A PDSysFont object referencing a system 
	font whose name is obtained. 
	@return The ASAtom for the system font's name. 
	@see PDSysFontAcquirePlatformData 
	@see PDSysFontGetEncoding 
	@see PDSysFontGetInfo 
	@see PDSysFontGetType0Widths 
	@since PI_PDSYSFONT_VERSION >= 0x00050000

*/
UNPROC (ASAtom, PDSysFontGetName, (
		IN		PDSysFont sysFont)
	)

/**
	Acquires platform-specific data for use by user interface 
	code. It must be released when finished by PDSysFontReleasePlatformData(). 
	
	@param sysFont IN/OUT A PDSysFont object referencing a system 
	font returned by either PDFindSysFont() or PDFindSysFontForPDEFont(). 
	
	@return A pointer to a platform-dependent structure PDSysFontPlatData 
	containing information relating to a system font. It returns 
	<code>NULL</code> if it is out of memory. 
	@see PDSysFontReleasePlatformData 
	@since PI_PDSYSFONT_VERSION >= 0x00040000

*/
UNPROC (PDSysFontPlatDataP, PDSysFontAcquirePlatformData, (
        IN      PDSysFont sysFont)
	)

/**
	Releases platform-specific data for the specified PDSysFont. 
	
	@param platDataP A pointer to a PDSysFontPlatDataP structure 
	containing platform-specific data.
	@see PDSysFontAcquirePlatformData 
	@since PI_PDSYSFONT_VERSION >= 0x00040000
*/
UNPROC (void, PDSysFontReleasePlatformData, (
        IN PDSysFontPlatDataP platDataP)
	)

/* PDSysFontGetScript
** Returns a PDScript value for the specified PDSysFont

UNPROC (PDScript, PDSysFontGetScript, (
        IN  PDSysFont sysFont)
	)
*/


/**
	If there is a font on the system that matches this PDEFont, 
	embed the full font, regardless of whether it was subsetted 
	or not embedded at all in the first place. This will not 
	work for CID fonts, because they must be subsetted. 

	<p>The matching is based on the PDSysFontMatchFlags. </p>

	<p>Only the font object itself is modified: no content streams 
	are changed. </p>
	@param font IN/OUT A PDEFont object returned from one of the PDEFontCreate 
	methods. 
	@param flags IN/OUT Flags from PDSysFontMatchFlags that determine 
	matches. 
	@param cosDoc IN/OUT Currently unused. 
	@exception peErrFontToEmbedNotOnSys is raised if there is no system font 
	that matches this. 
	@exception PDEFont. 
	@exception genErrBadParm is raised if the PDEFont is a CID font. 
	@exception peErrCantCreateFontSubset 
	@exception peErrCantGetAttrs 
	@exception peErrCantGetWidths 
	@see PDEFontCreateFromSysFont 
	@see PDFindSysFontForPDEFont 
	@since PI_PDSYSFONT_VERSION >= 0x00040000

	@note This method does not change the reference count of 
	the font. 
*/
UNPROC (void, PDEmbedSysFontForPDEFont, (
		IN	PDEFont	font,
		IN	ASUns32	flags,
		IN	CosDoc	cosDoc)
)

/**
	Derives the registry, ordering, and supplement information 
	of a multi-byte system font. This information can be used 
	to create a PDEFont from a system font. For more information 
	on CID fonts, see PDFontGetCIDSystemInfo(). 
	@param sysFont IN/OUT A PDSysFont object referencing a multi-byte 
	system font. 
	@param registry IN/OUT (Filled by the method) The ASAtom representing 
	the CIDFont's Registry information (for example, <code>"Adobe</code>). 
	@param ordering IN/OUT (Filled by the method) The ASAtom representing 
	the CIDFont's Ordering information (for example, <code>"Japan1"</code>). 
	
	@param supplement IN/OUT (Filled by the method) The SystemSupplement 
	field from the CIDFont. 
	@see PDFontGetCIDSystemInfo 
	@see PDFontGetCIDSystemSupplement 
	@since PI_PDSYSFONT_VERSION >= 0x00040000

*/
UNPROC (void, PDSysFontGetCIDSystemInfo, (
  		IN		PDSysFont sysFont, 
		OUT		ASAtom* registry, 
		OUT		ASAtom* ordering, 
		OUT		ASInt32* supplement)
	)

/**
	Gets width information from a Type 0 system font. This information 
	can be used to create a PDEFont from a system font. 
	
	@param sysFont IN/OUT A PDSysFont object referencing a multibyte 
	system font. 
	@param ordering IN/OUT An ASAtom representing the CIDFont's Ordering 
	information. It is used to get a CMap object for <code>sysFont</code>. 
	@param hasDW IN/OUT (Filled by the method) <code>true</code> if <code>sysFont</code> has 
	a valid <code>dw</code> value, <code>false</code> otherwise. 
	@param dw IN/OUT (Filled by the method) Default width for glyphs 
	in a CIDFont. Currently, always 1000. See Section 5.6 on 
	CIDFontType 0 in the <i>PDF Reference</i> for more information. 
	
	@param w IN/OUT (Filled by the method) A Cos array of a set of 
	lists that define the widths for the glyphs in the CIDFont. 
	Each list can specify individual widths for consecutive 
	CIDs, or one width for a range of CIDs. See Section 5.6.3 
	on character widths in CIDFonts in the <i>PDF Reference</i> for 
	information on the format of this array. 
	@param hasDW2 IN/OUT (Filled by the method) <code>true</code> if <code>sysFont</code> has 
	a valid <code>dw2</code> value. The default is <code>false</code>. 
	@param dw2 IN/OUT (Filled by the method) The default metrics for 
	writing mode <code>1</code>. This entry is an array of two ASInt32 numbers: 
	the y component of the position vector and the y component 
	of the displacement vector for writing mode <code>1</code>. The x component 
	of the position vector is always half the width of the character. 
	The x component of the displacement vector is always <code>0</code>. 
	The default value is <code>[880-1000]</code>. For information on writing 
	mode <code>1</code>, see Section 5.6.3 on vertical writing in the <i>PDF 
	Reference</i>. 
	@param w2 IN/OUT (Filled by the method) A Cos array defining the 
	metrics for vertical writing. Its format is similar to the 
	format of the array in w. It defines the x and y components 
	of the position vector, and the y component of the displacement 
	vector. The x component of the displacement vector is always 
	<code>0</code>. See Section 5.6.3 on character widths in CIDFonts in 
	the <i>PDF Reference</i> for information on the format of this 
	array. 
	@see PDSysFontGetWidths 
	@see PDSysFontGetWidthsEx 
	@see PDFontGetWidths 

	@note In general, you are discouraged from using this method. 
	Instead, use PDEFontCreateFromSysFontAndEncoding() followed 
	by PDEFontCreateWidthsNow() to create the W entry in a font. 
	@since PI_PDSYSFONT_VERSION >= 0x00040000

*/
UNPROC (void, PDSysFontGetType0Widths, (
  		IN		PDSysFont sysFont,
		IN		ASAtom ordering,
		OUT		ASBool* hasDW,
		OUT		ASInt32* dw, 
		OUT		CosObj* w, 
		OUT		ASBool* hasDW2,
		OUT		ASInt32* dw2, 
		OUT		CosObj* w2)
	)

