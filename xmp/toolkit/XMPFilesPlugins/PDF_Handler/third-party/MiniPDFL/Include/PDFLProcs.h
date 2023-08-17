/*************************************************************************
* WARNING: All additions to this file must go at the end of the file.
* This file is #included to build a table of function pointers (an HFT).
* All components which make use of this HFT rely on the order and
* positioning of these functions (e.g. plug-ins). Adding entries in the 
* middle of this file or modifying existing entries will cause these 
* components to break.
**************************************************************************/

/*
**      PDFLProcs.h
**
**      Toolkit-specific API calls
**
*/
/*********************************************************************

 ADOBE SYSTEMS INCORPORATED
 Copyright (C) 2003-2006 Adobe Systems Incorporated
 All rights reserved.

 NOTICE: Adobe permits you to use, modify, and distribute this file
 in accordance with the terms of the Adobe license agreement
 accompanying it. If you have received this file from a source other
 than Adobe, then your use, modification, or distribution of it
 requires the prior written permission of Adobe.

 ---------------------------------------------------------------------

 PDFLProcs.h

 - Catalog of the functions exported by the PDFL that are not in the 
   Acrobat plug-in interface.

*********************************************************************/

/**
	Registers a user-supplied procedure to call when the specified 
	event occurs. This is exactly the same as the AVAppRegisterNotification 
	method. All of the PD level notifications are available 
	with the Adobe PDF Library. 
	@param nsel Notification type. It must be one of the notification 
	selectors. The notification selector is the name of the 
	notification with the characters <code>NSEL</code> appended. For example, 
	the selector for PDDocDidPrintPage() is <code>PDDocDidPrintPageNSEL</code>. 
	Only the PD-level notifications are available with the Adobe 
	PDF Library. 
	@param owner Identifies the owner. For the Adobe PDF Library, 
	if there is only one owner of the PDFEdit subsystem, <code>owner</code> 
	should be zero. If there are multiple owners, each should 
	specify a nonzero, non-negative owner. (A negative owner 
	is reserved for the implementation). 
	@param proc A user-supplied callback to be called when the notification 
	occurs. Its declaration depends on the notification type. 
	
	@param clientData A pointer to user-supplied data to pass 
	to <code>proc</code> each time it is called.
	@see AVExtensionMgrRegisterNotification 
	@see AVAppRegisterNotification 
	@see PDFLibraryUnregisterNotification 
*/
NPROC(void, PDFLibraryRegisterNotification, (NSelector nsel,
	ASExtension owner, void *proc, void *clientData))

/**
	Unregisters a user-supplied procedure to call when the specified 
	event occurs. This is exactly the same as the AVAppUnregisterNotification() 
	method. 
	
	@param nsel Notification type. It must be one of the notification 
	selectors. The notification selector is the name of the 
	notification with the characters <code>NSEL</code> appended. For example, 
	the selector for PDDocDidOpen() is <code>PDDocDidOpenNSEL</code>. 
	@param owner Identifies the owner. For the PDF Library, 
	if there is only one owner of the PDFEdit subsystem, <code>owner</code> 
	should be zero. If there are multiple owners, each should 
	specify a nonzero, non-negative owner. (A negative owner 
	is reserved for the implementation). 
	@param proc A user-supplied callback to be called when the notification 
	occurs. Its declaration depends on the notification type. 
	You must use the same callback that you called AVExtensionMgrRegisterNotification() 
	with. 
	@param clientData A pointer to user-supplied data to pass 
	to <code>proc</code> each time it is called.
	@see PDFLibraryRegisterNotification 
	@see AVAppUnregisterNotification 
*/
NPROC(void, PDFLibraryUnregisterNotification, (NSelector nsel,
	ASExtension owner, void *proc, void *clientData))

/* PDFL 6.0 Additions */

/**
	Attempts to free memory from the PDF Library caches. 

	<p>The caches in the PDF Library can grow in complex and unexpected 
	ways. A client can manage memory use with the PDFLInit memory 
	callbacks, or by explicitly calling this function after 
	certain functions, such as PDDocClose(). </p>

	<p>To manage memory use with the memory callbacks, a client 
	can call ASPurgeMemory() during the allocation callback if 
	it is low on memory, or to limit the amount of memory used 
	by the library. Use this approach only with extreme caution 
	and extensive testing. The run-time memory requirements 
	are very document-specific.</p>

	@param amount The desired amount of memory to free.
	@return The approximate amount of memory freed. 
	@see PDFLInit 
	@see PDFLTerm 
*/
NPROC(ASSize_t, ASPurgeMemory, (ASSize_t amount))

/**
	Gets the number of ASAtom objects that have been allocated. The 
	maximum number of ASAtom objects is <code>0xFFFFFFFF</code>. (This was a 16-bit 
	value in Acrobat 4.x, and changed to a 32-bit value in Acrobat 
	5.0). 

	<p>ASAtom objects cannot be deleted or freed. Use this method to determine 
	if it is necessary to re-initialize the library before creating 
	more ASAtom objects. </p>

	@return The number of ASAtom objects currently allocated. 
	@see ASAtomExistsForString 
	@see PDFLInit 
	@see PDFLTerm
*/
NPROC(ASInt32, ASAtomGetCount, (void))


/**
	Sets the default temporary path for the specified file system 
	to the specified path name. The method copies the passed 
	<code>pathname</code> object on success; the client is responsible for 
	releasing the object when it is no longer needed, using 
	ASFileSysReleasePath(). 

	<p>Pass a pathname of <code>NULL</code> to reset the default temporary path 
	to the file system default. </p>

	@param fileSys (May be <code>NULL</code>) The file system in which 
	to set the default temporary path. 
	@param pathName The path name for the new default temporary 
	path, or <code>NULL</code> to reset to the file system default.
	@return <code>false</code> if the path provided is not writable, <code>true</code> otherwise. 
	
	@see ASFileSysGetDefaultTempPath 
*/
NPROC(ASBool, ASFileSysSetDefaultTempPath, (ASFileSys fileSys, ASPathName pathName))


/**
	Gets the default temporary path that was set by ASFileSysSetDefaultTempPath(). 
	
	@param fileSys The file system in which the ASPathname 
	is set.
	@return The ASPathName if the operation was successful, <code>NULL</code> otherwise. 
	
	@see ASFileSysSetDefaultTempPath 
*/
NPROC(ASPathName, ASFileSysGetDefaultTempPath, (ASFileSys fileSys))


/*------------------------------------------------------------------------
	New Placed PDF API methods
------------------------------------------------------------------------*/

#if !defined(IOS_ENV)
/**
	Prints a range of pages from a document, controlled by a 
	structure of data and callbacks. 

	@note This low-level method should be avoided; use the higher-level 
	PDFLPrintDoc() instead. 
	@param client Control structure for the operation, containing 
	data and callback procedures.
	@exception genErrBadParm 
	@notify PSPrintAfterBeginPageSetup 
	@notify PSPrintAfterBeginProlog 
	@notify PSPrintAfterBeginSetup 
	@notify PSPrintAfterEmitExtGState 
	@notify PSPrintAfterPageTrailer 
	@notify PSPrintAfterTrailer 
	@notify PSPrintBeforeEndComments 
	@notify PSPrintBeforeEndSetup 
	@see PDFLInit 
	@see PDFLPrintDoc 
*/

NPROC(void, PDDocPrintPages , (PDPrintClient client))

/**
	@note Obsolete in PDF Library 6.0. Do not use. 
	@see PDFLPrintDoc 
	@see PDDocPrintPages
*/
NPROC(void, PDPageEmitPSOrient, (PDPage pdPage, ASInt16 paperHeight, ASInt16 paperWidth, \
	  ASStm stm, PDPrintParams params))

#endif
#if !MAC_PLATFORM || !AS_ARCH_64BIT
/**
	Draws the page to the window or display context. The window 
	and display context are implementation-dependent. This API is 
    not available on Mac 64.
	This API will return error "Creation of a raster port failed." on Mac 32.

	<p>This method is the same as PDPageDrawContentsToWindow(), except 
	that it raises an exception if the <code>pdPermCopy</code> (see PDPerms) 
	permission is not set in the document. </p>

	@param page The page to draw into window. 
	@param window The platform window to which to render. 
	On Windows, the window is an <code>HWND</code>. 
	@param displayContext The platform display context to 
	which to render. On Windows, <code>displayContext</code> is an <code>HDC</code>.
	@param isDPS Currently unused. Always set it to <code>false</code>. 
	@param matrix A pointer to the matrix to concatenate onto 
	the default page matrix. It is useful for converting from 
	page to window coordinates and for scaling. 
	@param updateRect A pointer to the rectangle to draw, defined 
	in user space coordinates. Any objects outside of <code>updateRect</code> 
	will not be drawn. All objects are drawn if <code>updateRect</code> is 
	<code>NULL</code>. 
	@param cancelProc A method called to check whether drawing 
	should be cancelled. If the method returns <code>true</code>, drawing 
	is stopped, nothing is erased, and the window contains whatever 
	was drawn up to the current state. 
	@param cancelProcClientData A pointer to user-supplied data to 
	pass to <code>cancelProc</code> each time it is called. It should be <code>NULL</code> if <code>cancelProc</code> 
	is <code>NULL</code>.
	@exception pdErrOpNotPermitted is raised if the <code>pdPermCopy</code> permission 
	in PDPermReqObj for the document is not set. 
	@see PDPageDrawContentsToWindow
*/
NPROC(void, PDPageDrawContentsPlacedToWindow, (PDPage page,	void *window, void *displayContext, \
	  ASBool isDPS,	ASFixedMatrix *matrix, ASFixedRect *updateRect,	CancelProc cancelProc, \
	  void *cancelProcClientData))
#endif

#if !defined(IOS_ENV)
/**
	Emits a font into a specified stream. The font is in a format 
	suitable for downloading to a PostScript VM. For example, 
	a TrueType font is converted into a Type 1 or Type 42 font. 
	
	<p>It is meant for use in the <code>EmitFont</code> callback for PDDocPrintPages(). </p>

	@param fontP The font to emit. 
	@param stm The ASStm into which the font is emitted. 
	@param context A context created by PDFontDownloadContextCreate.
	@return <code>true</code> if successful, <code>false</code> otherwise. 
	@see PDDocPrintPages 
	@see PDFontDownloadContextCreate 
*/
NPROC(ASBool, PDFontStreamPS, (PDFont fontP, ASStm stm, PDFontDownloadContext context))
#endif

/**
	Creates a font download context object. This object keeps 
	track of the fonts downloaded during a print job and whether 
	substitution fonts have already been downloaded. 
	It also tracks the font download parameters, such as <code>binaryOK</code> 
	and <code>'emit TrueType as Type 42'</code>. It is meant for use in the PDFontStreamPS() 
	method. 
	@param client The client record to pass to PDDocPrintPages().
	@return The newly-created context. 
	@see PDDocPrintPages 
	@see PDFontDownloadContextDestroy 
	@see PDFontStreamPS 
*/
NPROC(PDFontDownloadContext, PDFontDownloadContextCreate, (PDPrintClient client))

/**
	Destroys a font download context object. Call this method 
	after PDDocPrintPages() returns. 
	@param context IN/OUT The context to destroy. 
	@see PDDocPrintPages 
	@see PDFontDownloadContextCreate 
*/
NPROC(void, PDFontDownloadContextDestroy, (PDFontDownloadContext context))

/**
	Returns the width and height of the page, which could be 
	rotated or defaulted. 
	@param page The page whose size is being obtained. 
	@param width (Filled by the method) The width of the page. 
	
	@param height (Filled by the method) The height of the page.
*/
NPROC(void, PDPageGetSize, (PDPage page, ASFixed *width, ASFixed *height))



/**
	Tests whether the specified font is embedded in the 
	PDF file and has already been extracted to display or print 
	the file. 
	@param fontP The font to test.
	@return <code>true</code> if the font is embedded in the PDF file and has been 
	extracted, <code>false</code> otherwise. 
	@see PDFontDownloadContextCreate 
	@see PDFontWasFauxed 
*/
NPROC(ASBool, PDFontWasExtracted, (PDFont fontP))



/**
	Tests whether the specified font is embedded in the 
	PDF file or is installed in the user's system. If this is 
	the case, the correct font can be used for display and printing. 
	
	<p>If the font is not embedded or installed, the Acrobat viewer 
	has used a Multiple Master font to create a substitute font, 
	called a <i>faux</i> font. </p>

	@param font The font to test.
	@return <code>true</code> if the font has been substituted, <code>false</code> otherwise. 
	
	@see PDFontWasExtracted 
*/
NPROC(ASBool, PDFontWasFauxed, (PDFont font))



/**
	Sets the <i>greek level</i>. The greek level is a text height below 
	which text characters are not rendered. Instead, text-like 
	glyphs that have no meaning but look good at very small 
	size are used. This is known as <i>greeking</i>. 
	@param greekPixelLevel The greek level, in pixels.
	@see PDPrefGetGreekLevel 
*/
NPROC(void, PDPrefSetGreekLevel, (ASInt16 greekPixelLevel))


/**
	Sets the default smooth text and smooth images global flags 
	for subsequent rendering methods. 

	<p>If the function PDPageDrawContentsToMemory() is used for drawing, 
	the <code>smoothFlags</code> value passed to that function supersedes 
	the preference value for the duration of the call. </p>

	@param antialiasPixelLevel The antialias level, in pixels. 
	It is an <code>OR</code> of the following flags:
	<ul>
	<li>kPDPrefAASmoothText</li>
	<li>kPDPrefAASmoothLineArt </li>
	<li>kPDPrefAASmoothImage </li>
	</ul>
	@see PDPrefGetAntialiasLevel 
*/
NPROC(void, PDPrefSetAntialiasLevel, (ASInt16 antialiasPixelLevel))


/**
	Returns the greek level. 
	@return The greek level set by PDPrefSetGreekLevel(). 
	@see PDPrefSetGreekLevel
*/
NPROC(ASInt16, PDPrefGetGreekLevel, (void))


/**
	Returns the antialias level, in pixels. 
	@return The antialias level set by PDPrefSetAntialiasLevel(). 
	@see PDPrefSetAntialiasLevel
*/
NPROC(ASInt16, PDPrefGetAntialiasLevel, (void))



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
	@return None. 
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
	Enables or disables use of local fonts. 
	@param useLocalFonts When <code>true</code>, use local fonts. When 
	<code>false</code>, use global fonts.
*/
NPROC(void, PDPrefSetUseLocalFonts, (ASBool useLocalFonts))


/*XXX For Internal Use Only.  Should These Even Be Declared For Outside World? */

#if !defined(IOS_ENV)
/**
	Emit glyphs incrementally. This is the default <code>EmitGlyphsIncr</code> 
	callback procedure for the PDPrintClient structure. 
	@param stm The stream. 
	@param fontP The font. 
	@param srcStr The source string. 
	@param dstStr The destination string. 
	@param srcBytesUsedP The source bytes used. 
	@param dstBytesUsedP The destination bytes used. 
	@param glyphCount The number of glyphs. 
	@param fontIndexP The font index. 
	@param printClient The control structure.
	@return <code>true</code> if successful, <code>false</code> otherwise. 
	@see PDDocPrintPages 
	@see PDFontPSFlushIncrGlyphList 
	@see PDFontPSGetComponentFontList 
*/
NPROC(ASBool, PDFontPSEmitGlyphsIncr, (ASStm stm, PDFont fontP, PDPrintStrP srcStr, \
	  PDPrintStrP dstStr, ASUns32 *srcBytesUsedP, ASUns32 *dstBytesUsedP, \
	  ASUns32 *glyphCount, ASUns16 *fontIndexP, PDPrintClient printClient ))


/**
	Get the component font list. This is the default <code>GetComponentFontList</code> 
	callback procedure for the PDPrintClient structure. 
	@param fontP The font. 
	@param pdFontArr The font array. 
	@param printClient The control structure.
	@see PDFontPSEmitGlyphsIncr 
	@see PDFontPSFlushIncrGlyphList 
*/
NPROC(void, PDFontPSGetComponentFontList, (PDFont fontP, PDPrintFontArrayP pdFontArr, PDPrintClient printClient))

/**
	Flush the incremental glyphs list from a stream. This is 
	the default <code>FlushIncrGlyphList</code> callback procedure for the 
	PDPrintClient structure. 
	@param stm The stream. 
	@param printClient The control structure.
	@see PDFontPSEmitGlyphsIncr 
	@see PDFontPSGetComponentFontList 
*/
NPROC(void , PDFontPSFlushIncrGlyphList, (ASStm stm, PDPrintClient printClient))
#endif
/**
	Superseded by PDPageDrawContentsToMemoryEx() in Acrobat 10.0.

	Renders a page to memory. The width of the image is calculated 
	as follows:

	<code>
	<p>width = abs(ASFixedRoundToInt16(destRect.right) - ASFixedRoundToInt16(destRect.left));</p>
	<p>width = ((((width * bpc * nComps)+31) / 32) * 4) * 8 / (bpc	* nComps);</p>
	<p>nComps = 1 for DeviceGray, 3 for DeviceRGB, 4 for DeviceCMYK</p>
	<p>bpc = bits per component </p>
	</code>
	@param page The page to render. 
	@param flags A bit field of PDPageDrawFlags(). It must be an 
	<code>OR</code> of the following flags:

	<TABLE rules="all" cellspacing="1">
	<TR><TH>Flag</TH><TH>Description</TH></TR>
	<TR><TD>kPDPageDoLazyErase</TD><TD>If set, it erases the bitmap if the first object drawn does not cover the page's entire crop box.</TD></TR>
	<TR><TD>kPDPageUseAnnotFaces</TD><TD>If set, it draws annotations that have a default face, such as the visible fields in an Acrobat form. Text and link annotations are not drawn.</TD></TR>
	<TR><TD>kPDPageIsPrinting</TD><TD>If set, then form annotations are rendered as if they are being printed. This means that form fields marked as 'Hidden but printable' are rendered, but fields marked as 'Visible but doesn't print' will not be rendered. If it is not set, then form annotations are rendered as if they are being viewed. This means that form fields marked as 'Hidden but printable' are not rendered, but fields marked as 'Visible but doesn't print' will be.</TD></TR>
	</TABLE>

	@param matrix A pointer to the matrix to be concatenated 
	onto the default page matrix. It must not be <code>NULL</code>.
	@param updateRect A pointer to the rectangle to draw, defined 
	in user space coordinates. Any objects outside of <code>updateRect</code> 
	will not be drawn. All objects are drawn if <code>updateRect</code> is 
	<code>NULL</code>. 
	@param smoothFlags A bit field of PDPageDrawSmoothFlags(). 
	It must be an <code>OR</code> of the following flags:
	<ul>
	<li>kPDPageDrawSmoothText </li>
	<li>kPDPageDrawSmoothLineArt </li>
	<li>kPDPageDrawSmoothImage </li>
	<li>kPDPageDrawSmoothBicubicImage </li>
	</ul>
	@param csAtom The color space in which the bitmap data 
	is represented. It must be one of DeviceGray, DeviceRGB, 
	or DeviceCMYK. 
	@param bpc The number of bits per color component in the 
	bitmap data. <code>8</code> is the only valid value for DeviceCMYK and 
	DeviceRGB color spaces. <code>1</code> and <code>8</code> are valid for DeviceGray. 
	
	@param destRect A pointer to the rectangle of the bitmap. 
	It is defined in device space coordinates. It must not be <code>NULL</code>.
	@param buffer A pointer to the bitmap data. If it is <code>NULL</code>, this 
	function returns the size of the buffer needed for the bitmap. 
	
	@param bufferSize The size of the buffer. 
	@param cancelProc A method to call to check whether drawing 
	should be cancelled. If the method returns <code>true</code>, drawing 
	is stopped, nothing is erased, and the buffer contains whatever 
	was drawn up to the current state. 
	@param cancelProcData User-supplied data to pass to <code>cancelProc</code> 
	each time it is called.
	@return The size of the bitmap in bytes. 
	@see PDPageDrawContentsPlacedWithParams 
	@see PDPageDrawContentsToMemoryEx
*/

NPROC(ASInt32, PDPageDrawContentsToMemory, (PDPage page, ASUns32 flags, ASFixedMatrix *matrix, \
	  ASFixedRect *updateRect, ASUns32 smoothFlags, ASAtom csAtom, ASInt32 bpc, \
	  ASFixedRect *destRect, char *buffer, ASInt32 bufferSize, CancelProc cancelProc, \
	  void *cancelProcData))


#if !MAC_PLATFORM || !AS_ARCH_64BIT
 /**
	Draws the page to the window or display context. The window 
	and display context are implementation-dependent.  This API 
    is not available on Mac 64.
	This API will return error "Creation of a raster port failed." on Mac 32.

	<p>This method is the same as PDPageDrawContentsToWindow(), except 
	that it raises an exception if the <code>pdPermCopy</code> (see PDPerms) 
	permission is not set in the document. 
	This method is also like PDPageDrawContentsPlacedToWindow(), only passing in
	a PDDrawParams structure which lets the clients specify their own PDOCContext.</p>

	@param page The page to render.
	@param params Allows clients to specify their own PDOCContext.
	@exception pdErrOpNotPermitted is raised if the <code>pdPermCopy</code> permission 
	in PDPermReqObj is not set.
	@see PDPageDrawContentsToWindow
*/
 NPROC(void, PDPageDrawContentsPlacedWithParams, (PDPage page, PDDrawParams params))
#endif

/* END OC Drawing calls */


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
	Specifies use of a default color space rather than an ICC-based 
	color space. 
	@param nComponents The number of ICC color space components. 
	
	@param value When <code>true</code>, use a DefaultCMYK, DefaultRGB, or 
	DefaultGray color space instead of an ICC-based color space 
	with the same number of components.
	@see PDPrefGetSuppressICCSpaces 
*/
NPROC(void, PDPrefSetSuppressICCSpaces, (ASUns32 nComponents, ASBool value))
    
/**
	Returns the value of the <code>suppress</code> flag for ICC-based spaces 
	with the specified number of components. 
	@param nComponents The number of ICC-based space components 
	that are suppressed or not, according to the flag value.
	@return The <code>suppress</code> flag value. 
	@see PDPrefSetSuppressICCSpaces 
*/
NPROC(ASBool, PDPrefGetSuppressICCSpaces, (ASUns32 nComponents))

/**
	Registers a user-supplied procedure to call when the event 
	of the specified type occurs. This is exactly the same as 
	the AVAppRegisterNotification() method. All of the PD level 
	notifications are available with the Adobe PDF Library. 
	
	@param nsel Notification type. It must be one of the notification 
	selectors. The notification selector is the name of the 
	notification with the characters <code>NSEL</code> appended. For example, 
	the selector for PDDocDidPrintPage() is <code>PDDocDidPrintPageNSEL</code>. 
	Only the PD-level notifications are available with the Adobe 
	PDF Library. 
	@param owner Identifies the owner. For the Adobe PDF Library, 
	if there is only one owner of the PDFEdit subsystem, <code>owner</code> 
	should be zero. If there are multiple owners, each should 
	specify a nonzero, non-negative owner. (A negative owner 
	is reserved for the implementation). 
	@param proc A user-supplied callback to be called when the notification 
	occurs. Its declaration depends on the notification type. 
	
	@param clientData A pointer to user-supplied data to pass 
	to <code>proc</code> each time it is called.
	@see AVExtensionMgrUnregisterNotification 
	@see AVAppRegisterNotification 
*/
NPROC(void, AVExtensionMgrRegisterNotification, (NSelector nsel, ASExtension owner, void *proc, void *clientData))

/**
	Unregisters a user-supplied procedure to call when the specified 
	event occurs. This is exactly the same as the AVAppUnregisterNotification() 
	method. 
	
	@param nsel Notification type. It must be one of the notification 
	selectors. The notification selector is the name of the 
	notification with the characters <code>NSEL</code> appended. For example, 
	the selector for PDDocDidOpen() is <code>PDDocDidOpenNSEL</code>. 
	@param owner Identifies the owner with which the notification 
	was registered. 
	@param proc A user-supplied callback with which the notification 
	was registered. 
	@param clientData A pointer to user-supplied data that was 
	used when the notification was registered.
	@see AVExtensionMgrRegisterNotification 
	@see AVAppUnregisterNotification 
*/
NPROC(void, AVExtensionMgrUnregisterNotification, (NSelector nsel, ASExtension owner, void *proc, void *clientData))

/**
	Set the current RGB working space to a given ICC profile. 
	An RGB working space in PDF is defined as a profile to substitute 
	for a corresponding /DeviceRGB space. 
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
	for a corresponding /DeviceCMYK space. 
	@param profile A pointer to a buffer containing the ICC 
	color profile. 
	@param profileLength The length in bytes of the profile.
	@see PDPrefSetWorkingGray 
	@see PDPrefSetWorkingRGB 
*/
NPROC(void, PDPrefSetWorkingCMYK, (void *profile, ASUns32 profileLength))

/**
	Sets the current gray working space to a given ICC profile. 
	A gray working space in PDF is defined as a profile to substitute 
	for a corresponding /DeviceGray space. When rendering with 
	overprint preview, the gray substitution is suppressed, 
	to avoid converting grayscale to "rich black." 
	@param profile A pointer to a buffer containing the ICC 
	color profile. 
	@param profileLength The length in bytes of the profile.
	@see PDPrefSetWorkingCMYK 
	@see PDPrefSetWorkingRGB 
*/
NPROC(void, PDPrefSetWorkingGray, (void *profile, ASUns32 profileLength))

/* PDFL Control functions. Were in PDFLInit.h */

/**
	Gets the value of the Adobe PDF Library version (kPDFLVersion). 
	The most significant 16 bits are the major version number; 
	the least significant 16 bits are the minor version number. 
	The major version number indicates whether any incompatible 
	API changes have been made. The minor version number indicates that
	the API has changed, but in a compatible fashion. 

	@note Obsolete in PDF Library 6.0. Use HFTGetVersion() instead. 

	@return Adobe PDF Library version (kPDFLVersion). 
	@see PDFLTerm
*/
NPROC(ASUns32, PDFLGetVersion, (void))

/**
	Terminates the Adobe PDF Library. Call this method after 
	you are completely done using the library. Call this once 
	to terminate and release memory used by the library. After 
	the library has been shut down, the process should terminate. 
	
	@see PDFLInit
*/
NPROC(void, PDFLTerm, (void))


/**
	Gets the number of times the PDF Library has been initialized. 
	
	@return The number of times the PDF Library has been initialized. 
	
	@see PDFLInit
*/
NPROC(ASUns32, PDFLGetInitCount, (void))

/**
	<p>Gets the flags set when the PDF Library was initialized. 
	Currently <code>kPDFLInitIgnoreDefaultDirectories</code> , <code>kPDFLInitIgnoreSystemFonts</code> and 
	<code>kDontLoadPlugIns</code> flags are supported. </p>
	
	<p>When <code>kPDFLInitIgnoreDefaultDirectories</code> flag is set, the initialization process does not 
	search through the default font directories (currently Adobe font 
	directories installed by some Adobe applications), but only searches 
	for fonts in those directories specified in <code>dirList</code>. A default directory could appear as follows:</p>

	<p>C:\\Program Files\\Common Files\\Adobe\\PDFL\\[<i>version number</i>]\\Fonts</p>
	<p>C:\\Program Files\\Common Files\\Adobe\\PDFL\\[<i>version number</i>]\\CMaps</p>
	<p>When <code>kPDFLInitIgnoreSystemFonts</code> flag is set, the initialization process does not
	search through default System fonts also.</p>
	<p>When the <code>kDontLoadPlugIns</code> flag is set, plug-ins are
	ignored during initialization process.</p>

	@return The initialization flags value. 
	@see PDFLInit
*/
NPROC(ASUns32, PDFLGetFlags, (void))

#if !defined(IOS_ENV)
/**
	Prints a PDF document or pages from a PDF document allowing 
	the caller to specify options such as page size, rotation, 
	and shrink-to-fit. 

	@note Users of the PDFLPrintDoc() method do not have to create 
	the PDPrintClient() callbacks. That detail is handled by the 
	library. 
	@param doc The PDDoc for the document to print. 
	@param userParams Parameters to control printing.
	@exception cosErrWriteError 
	@exception pdErrOpNotPermitted 
	@exception genErrBadParm 
	@notify PSPrintAfterBeginPageSetup 
	@notify PSPrintAfterBeginProlog 
	@notify PSPrintAfterBeginSetup 
	@notify PSPrintAfterEmitExtGState 
	@notify PSPrintAfterPageTrailer 
	@notify PSPrintAfterTrailer 
	@notify PSPrintBeforeEndComments 
	@notify PSPrintBeforeEndSetup 
*/
NPROC(void, PDFLPrintDoc, (PDDoc doc, PDFLPrintUserParams userParams))


/**
	Deprecated: use PDFLPrintDoc() instead.
*/

NPROC(void, PDFLPrintPDF, (PDDoc pdDoc, ASPathName pathName, PDPrintParams psParams))
#endif

#ifndef ANDROID_ENV
/**
	Registers a user-supplied procedure to call when the specified 
	event occurs. 

	<p>Many notifications appear in Will/Did pairs (for example, 
	AVDocWillPerformAction() and AVDocDidPerformAction()). It is 
	possible that an operation may fail after the Will notification 
	and before the Did notification. When this occurs, the Did 
	notification is still broadcast, but the <code>err</code> parameter in 
	the Did notification is nonzero, and represents the error 
	that occurred. When <code>err</code> is nonzero, the other parameters 
	are not necessarily valid. Always check <code>err</code> in a Did notification 
	before using the other parameters. </p>

	<p>When calling AVAppUnregisterNotification() to un-register 
	for a notification, you must pass the <code>proc</code>, <code>clientData</code>, 
	and <code>owner</code> that were used when the notification was registered 
	using AVAppRegisterNotification(). You must use the same callback 
	that was used to register; you cannot use a newly created 
	callback. To accomplish this, call ASCallbackCreateNotification() 
	once before registering, and use the value returned from 
	this call both to register and un-register; do not call 
	ASCallbackCreateNotification() a second time when un-registering. 
	You will then need to destroy the pointer to the callback 
	using the ASCallbackDestroy() method. </p>

	@param nsel The notification type. It must be one of the 
	notification selectors . The notification 
	selector is the name of the notification with the characters 
	<code>NSEL</code> appended. For example, the selector for AVDocDidOpen() 
	is <code>AVDocDidOpenNSEL</code>. 
	@param owner The gExtensionID of the client registering 
	the notification. 
	@param proc A user-supplied callback to call when the notification 
	occurs. Its declaration depends on the notification type. Remember to use ASCallbackCreateNotification() 
	to convert <code>proc</code> to a callback before passing it to AVAppRegisterNotification(). 
	@param clientData A pointer to user-supplied data to pass 
	to <code>proc</code> each time it is called.
	@param priority The callbacks are enumerated in priority order, starting 
	with the highest priority.
*/
NPROC(void, PDFLibraryRegisterNotificationEx, (NSelector nsel, \
	ASExtension owner, void *proc, void *clientData, ASInt32 priority))

NPROC(void, CosSetExternalFilePermissionProc, (ExternalFilePermissionProc proc))

/**
	Registers a user-supplied random number generator.  By default,
	the PDF Library obtains high-quality random values from the operating
	system (<code>CryptGenRandom()</code> on Windows, and <code>/dev/random</code> on Mac OS and many UNIX operating systems).  
	This method allows an alternate handler to be used.
	The random numbers returned should be of high quality, and it is
	the responsibility of the developer to ensure this.  A <code>NULL</code> parameter
	will reset to default behavior.
	@param clientRNG A client-supplied function that supplies the
	strong random values.
	@see PDFLClientRNGProc
*/
NPROC(void, PDFLibraryRegisterRNG, (PDFLClientRNGProc clientRNG))
#endif

/**
	Registers a callback to the client user interface that can tell the core what the current zoom level is. 
*/
NPROC(void, PDOCRegisterFindOutZoomProc, (PDOCFindOutZoomProc proc))

/**
	Registers a callback to the client user interface that can tell the core what the current language is. 
*/
NPROC(void, PDOCRegisterFindOutLanguageProc, (PDOCFindOutLanguageProc proc))

/**
	Registers a callback to the client user interface that can tell the core what the current user is. 
*/
NPROC(void, PDOCRegisterFindOutUserProc, (PDOCFindOutUserProc proc))

/**
	Registers a callback to the client user interface that can tell the core what the current AutoState preference is.
*/
NPROC(void, PDOCRegisterFindOutAutoStatePrefProc, (PDOCFindOutAutoStatePrefProc proc))


/** 
	Sets reference XObject parameters.
	@param refXObjMode The mode to view or print reference XObjects. Its value can be one of the following:

		<TABLE rules="all" cellspacing="1">
		<TR><TH>Reference XObject mode</TH><TH>Description</TH></TR>
		<TR><TD><code>kRefXObjNever</code></TD><TD>Does nothing. The function returns <code>false</code>.</TD></TR>
		<TR><TD><code>kRefXObjAlways</code></TD><TD>Find a substitution with minimal checking.</TD></TR>
		<TR><TD><code>kRefXObjPDFX5</code></TD><TD>Restrict substitutions to PDF/X-5 conforming ones.</TD></TR>
		<TR><TD><code>kRefXObjAlwaysAssured</code></TD><TD>Fail the parser if the target is not found. Do not display the proxy.</TD></TR>
		<TR><TD><code>kRefXObjPDFX5Assured</code></TD><TD>Fail the parser if the target PDF/X-5 document not found. Do not display the proxy.</TD></TR>
		</TABLE><BR/>

    @param fileSys Used with <code>pathname</code> to specify the location where the target files should be located. These are used to look for the target document.
	@param pathname Used with <code>pathname</code> to specify the location where the target files should be located. These are used to look for the target document.
					The order followed is that given in the <i>PDF Reference</i>.
	*/

NPROC(void, PDPrefSetRefXObj, (PDRefXObjMode refXObjMode, 
							   ASFileSys fileSys, 
							   ASPathName pathName))

/** 
	Gets reference XObject parameters.
    @param fileSys Used with <code>pathname</code> to specify the location where the target files should be located. These are used to look for the target document.
	@param pathname Used with <code>pathname</code> to specify the location where the target files should be located. These are used to look for the target document.
					The order followed is that given in the <i>PDF Reference</i>.
	*/
NPROC(PDRefXObjMode, PDPrefGetRefXObj, (ASFileSys* fileSys, 
								  ASPathName* pathName))


/**
	Sets whether thin lines will be fattened non-linearly or the stroke adjust will be applied to thin rectangles.
	@param doThinLineTricks <code>true</code> if thin line heuristics are applied, <code>false</code> otherwise.
*/
NPROC(void, PDPrefSetEnableThinLineHeuristics, (ASBool doThinLineTricks))

/**
	Determines whether thin lines will be fattened non-linearly or the stroke adjust will be applied to thin rectangles.
	@return <code>true</code> if thin line heuristics are applied, <code>false</code> otherwise.
*/
NPROC(ASBool, PDPrefGetEnableThinLineHeuristics, (void))

/**
	Supersedes PDPageDrawContentsToMemory() in Acrobat 10.0.

	Renders a page to memory. It uses double precision input parameters. 
	The width of the image is calculated as follows:

	<code>
	<p>width = abs(ROUND(destRect.right) - ROUND(destRect.left));</p>
	<p>width = ((((width * bpc * nComps)+31) / 32) * 4) * 8 / (bpc	* nComps);</p>
	<p>nComps = 1 for DeviceGray, 3 for DeviceRGB, 4 for DeviceCMYK</p>
	<p>bpc = bits per component </p>
	</code>	
	where, ROUND() rounds off double precision values to nearest Int32 value.

	@param page The page to render. 
	@param flags ASCab used for passing PDPageDrawFlagsStr and PDPageDrawSmoothFlagsStr.
	@param matrix A pointer to double matrix to be concatenated 
	onto the default page matrix. It must not be <code>NULL</code>.
	@param updateRect A pointer to the double rectangle to draw, defined 
	in user space coordinates. 
	@param csAtom The color space in which the bitmap data 
	is represented. 
	@param bpc The number of bits per color component in the 
	bitmap data. 
	@param destRect A pointer to double rectangle of the bitmap. 
	It is defined in device space coordinates. It must not be <code>NULL</code>.
	@param buffer A pointer to the bitmap data. If it is <code>NULL</code>, this 
	function returns the size of the buffer needed for the bitmap. 
	@param bufferSize The size of the buffer. 
	@param cancelProc A method to call to check whether drawing 
	should be cancelled. 
	@param cancelProcData User-supplied data to pass to <code>cancelProc</code> 
	each time it is called.
	@return The size of the bitmap in bytes. 
	@see PDPageDrawContentsToMemory
	@see PDPageDrawContentsPlacedWithParams 
*/

NPROC(ASUns32, PDPageDrawContentsToMemoryEx, (PDPage page, ASCab flags, ASDoubleMatrix *matrix, \
	  ASDoubleRect *updateRect, ASAtom csAtom, ASInt32 bpc, \
	  ASDoubleRect *destRect, char *buffer, ASUns32 bufferSize, CancelProc cancelProc, \
	  void *cancelProcData))

/**
	Sets dpi value for rasterization of content during translation of structure to HTML
	@param val DPI.
 */
NPROC(void, PDPrefSetTranslationRasterizationDPI, (ASUns32 val))

/**
	Returns dpi value for rasterization of content during translation of structure to HTML
	@return the value set by PDPrefSetTranslationRasterizationDPI.
 */
NPROC(ASUns32, PDPrefGetTranslationRasterizationDPI, (void))
