/*************************************************************************
 *
 *  Copyright 2003-2006 Adobe Systems Incorporated
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
** PDFLPrint.h
** 
** Describes PDFLPrintDoc() API and PDFLPrintUserParams structure
**
*/

#ifndef _H_PDFLPrint
#define _H_PDFLPrint

#include "PDFLExpT.h"
#if ACROBAT_LIBRARY || PDFL_EXTENSION
#include "PDFLCalls.h"
#endif

#include <stdio.h>
#include <stdlib.h>

#if MAC_PLATFORM && !defined(IOS_ENV)
#include "Carbon/Carbon.h"
#endif

#if WIN_PLATFORM
#include <Windows.h>
#endif

#ifdef __cplusplus
extern "C" {
#endif

#if defined( WINDOWS ) || defined( WIN_ENV )
#pragma pack(push, peexpt, 8)
#endif


struct _t_PPDFeatures;
typedef struct _t_PPDFeatures PPDFeaturesRec, *PPDFeaturesP;

/**
	This is called once per page of a document being printed. 
	

	In addition to giving the ability to cancel the print job, 
	a developer can use this callback to return control briefly 
	to an application to handle events, update UI elements, 
	and so on. The library pauses printing until the return 
	from this procedure, because it is single-threaded. 
	@param pdDoc The document being printed. 
	@param clientData A pointer to the client data that was 
	passed in the PDFLPrintUserParamsRec with the call to PDFLPrintDoc.
	@return true to cancel the print job, false otherwise. 
	@see ASCancelProc 
	@see CancelProc 
	@see PDFLPrintUserParamsRec
*/
typedef ACCBPROTO1 ASBool (ACCBPROTO2 *PDFLPrintCancelProc)(PDDoc pdDoc, void *clientData);

#if UNIX_PLATFORM


/** 
	<p>Used to control printing with PDFLPrintDoc. </p>
	
	<p>To print to a PostScript file, set <code>emitToFile</code> to <code>true</code>, fill in
    <code>printParams</code>, and create a writeable stream.</p>

    <p>To print to a printer, set <code>emitToPrinter</code> to <code>true</code>, fill in <code>printParams</code>,
    and optionally fill in the command.</p>
	
	@note UNIX supports only PostScript to a file or printer, as controlled by 
	the (mutually exclusive) <code>emitToFile</code> and <code>emitToPrinter</code> parameters.

	@see PDFLPrintDoc
	@see PDFLPrintCancelProc
*/

typedef struct _t_PDFLPrintUserParams {

	/** Size of the data structure. Must be set to
		sizeof(PDFLPrintUserParamsRec).
	*/
	ASInt32 size; 
	

	/** PDPrintParams structure. Used for both creating a
		PostScript file and printing to a printer.
	*/
	PDPrintParams printParams; 


	/** Create a PostScript file. You set either this parameter or
		emitToPrinter, but not both.
	*/
	ASBool emitToFile;
	

	/** Writable ASStm that points to a file stream or proc stream. It applies to
		both a PostScript printer and a file.
	*/
	ASStm printStm; 


	/** Width of paper in points. The default value is 612. Applies to a
		PostScript printer.
	*/
	ASUns16 paperWidth;	


	/** Height of paper in points. The default value is 792. Applies to a
		PostScript printer.
	*/
	ASUns16 paperHeight; 


	/** Number of fonts that should not be downloaded. Applies to
		both a PostScript printer and a file.
	*/
	ASInt32 dontEmitListLen; 


	/** List of fonts (T1, TrueType, CID) that should not be downloaded.
		Applies to both a PostScript printer and a file.
	*/
	char ** dontEmitList;


	/** Output PDF file to a PostScript printer. Set either this
		parameter or <code>emitToFile</code>, but not both.
	*/
	ASBool emitToPrinter;


	/** 
		Optional command line arguments. It is used only if
		<code>emitToPrinter</code> is <code>true</code>. For example, <code>"lp"</code> or <code>"lpr"</code> applies to
		a PostScript printer.
	*/
	char * command; 


	/** 
		Optional PDFLPrintCancelProc. It applies to both a
		a PostScript printer and a file.
	*/
	PDFLPrintCancelProc cancelProc;


	/** 
		Optional pointer to data passed to <code>cancelProc</code>. Applies to
		both a PostScript printer and a file.
	*/
	void * clientData;
	

	/** Spooler ID from StartDoc */
	int startResult;

	/** Optional pointer to structure containing pointers to user callback functions */
	PDFLPrintUserCallbacks userCallbacks;

	/** (Optional) Number of copies to print. It applies to a PostScript file. */
	int   nCopies;

	/** Optional pointer to a structure where you can add additional PPD features like staple, input tray, 
	output tray, duplex mode etc. for output to PostScript file. For more details,
	see <code>_t_PPDFeatures</code>*/
	PPDFeaturesP PPDFeatures;

	/** (Optional)The name and path of the PPD file from which the PPD features will be emitted to a 
	PostScript file. Use this field alongwith <code>PPDFeaturesP PPDFeatures</code>. For more details, 
	see <code>_t_PPDFeatures</code>.*/
	ASText ppdFileName;

} PDFLPrintUserParamsRec;

#elif WIN_PLATFORM

/** 
	<p>Used to control printing with PDFLPrintDoc. </p>
	
	<p>See PDFLPrint.h for details about printing on specific platforms.</p>

	<p>To print to a PostScript file, set <code>emitToFile</code> to <code>true</code>, fill in
    <code>printParams</code>, and create a writeable stream.</p>

    <p>To print to a printer, set <code>emitToPrinter</code> to <code>true</code>, fill in <code>printParams</code>,
    and optionally fill in the command.</p>
	
	@note UNIX supports only PostScript to a file or printer, as controlled by 
	the mutually exclusive <code>emitToFile</code> and <code>emitToPrinter</code> parameters.

	@see PDFLPrintDoc
	@see PDFLPrintCancelProc
*/

typedef struct _t_PDFLPrintUserParams {

	/** Size of the data structure. Must be set to
		sizeof(PDFLPrintUserParamsRec).
	*/
	ASInt32 size; 


	/** Create a PostScript file. You set either this parameter or
		emitToPrinter, but not both.
	*/
	ASBool emitToFile; 


	/** PDPrintParams structure. It is used for both creating a
		PostScript file and printing to a printer.
	*/
	PDPrintParams printParams;


	/** Writable ASStm that points to file stm or proc stm. It applies to
		both a PostScript printer and a file.
	*/
	ASStm printStm;
	

	/** Width of paper in points. The default value is 612. It applies to a
		PostScript printer.
	*/
	ASUns16 paperWidth;	


	/** Height of paper in points. The default value is 792. It applies to a
		PostScript printer.
	*/
	ASUns16 paperHeight;
	

	/** Number of fonts that should not be downloaded. It applies to
		both a PostScript printer and a file.
	*/
	ASInt32 dontEmitListLen; 


	/** List of fonts (T1, TT, CID) that should not be downloaded.
		It applies to both a PostScript printer and a file.
	*/
	char ** dontEmitList; 


	/** Output PDF file to a PostScript printer. Set either this
		parameter or <code>emitToFile</code>, but not both.
	*/
	ASBool emitToPrinter; 


	/** 
		<p>Deprecated. Use the Unicode version <code>inFileNameW</code>.</p>

		<p>Used for the Windows DOCINFO structure (<code>lpszDocName</code>)
		that points to a <code>NULL</code>-terminated string that specifying the name of
		the document. In case of printing/exporting to Postscript, this field sets
        the Title DSC comment in the generated Postscript file. inFileNameW has precedence over the
        deprecated inFileName. If neither is provided, the Title DSC comment is filled with the name of
        pdf document.</p>
    */	
	char *inFileName;
	

	/** 
		<p>Deprecated. Use the Unicode version <code>outFileNameW</code>.</p>

		</p>Used for the Windows DOCINFO structure (<code>lpszOutput</code>). It
		points to a <code>NULL</code>-terminated string that specifies the name of an
		output file. If this pointer is <code>NULL</code>, the output will be sent to the
		device identified by <code>deviceName</code>, <code>driverName</code>, and
		<code>portName</code>. It applies to a printer.</p>
	*/
	char *outFileName;	


	/**
		<p>Deprecated. Use the Unicode version <code>deviceNameW</code>.</p>

		<p>The name of the device on which to print (for example, <code>"Distiller Assistant
		v3.01"</code>). Available devices can be found in the Windows Registry at
		<code>HKEY_CURRENT_USER\\Software\\Microsoft\\Windows NT\\CurrentVersion\\Devices</code>. It applies to a printer.</p>
	*/
	char *deviceName;		


	/** 
		<p>Deprecated. Use the Unicode version <code>driverNameW</code>.</p>

		<p>The name of the driver (for example, <code>"winspool"</code>). See the registry key for
		<code>deviceName</code>. It applies to the printer.</p>
	*/
	char *driverName;		


	/** 
		<p>Deprecated. Use the Unicode version <code>portNameW</code>.</p>

		<p>The name of the port (for example, <code>"Ne00"</code>). See the registry key for
		<code>deviceName</code>. It applies to the printer.</p>
	*/
	char *portName;	
	

	/** 
		<p>Deprecated. Use the Unicode version <code>pDevModeW</code>.</p>

		<p>(Optional) Allows you to pass a DEVMODE structure in if you have one. </p>
		
		<p>If you do not pass in your own DEVMODE structure, 
		PDFL creates its own by calling OpenPrinter(), 
		DocumentProperties(), and ClosePrinter(). </p>
		
		<p>PDFL never calls SetPrinter() and has no facility 
		to offer the features provided by SetPrinter() 
		(for example, pause/resume, deleting jobs, changing printer 
		status, etc.). DocumentProperties() is called only
		in the case where the client does not provide a DEVMODE.</p>

		@note If you provide your own DEVMODE, you will certainly
		want to call DocumentProperties() to populate it
		and/or display the print dialog box.
	*/
	DEVMODEA * pDevMode;		


	/** Page to start printing with, using zero-based numbering. It applies to a printer. */
	int   startPage;
	

	/** Page on which to finish printing. It applies to a printer. */
	int   endPage;
	

	/** <code>1</code> turns on shrink to fit, <code>0</code> turns it off. It applies to a PostScript
		printer.
	*/
	int   shrinkToFit;
	

	/** <code>1</code> turns on printing annotations, <code>0</code> turns it off. It applies to a printer.
	*/
	int   printAnnots;
	

	/** PostScript level. It applies to a PostScript printer. */
	int   psLevel;
	

	/** Number of copies to print. It applies to a printer and to a PostScript file. */
	int   nCopies;
	

	/** <code>1</code> turns on binary data, <code>0</code> turns it off. It applies to a PostScript printer. */
	int   binaryOK;
	

	/** <code>1</code> turns on emit half-tones, <code>0</code> turns it off. It applies to a PostScript
		printer.
	*/
	int   emitHalftones;
	

	/** <code>1</code> reverses print order, <code>0</code> does regular order. It applies to a printer. */
	int   reverse;
	

	/** 
		One of PDFarEastFont.The default value is
		PDPDFarEastFont_Download_All. It applies to a PostScript
		printer. 
		<code>PDPDFarEastFont_Print_As_Image</code> will not be honored.
		@see PDFarEastFont enum in PDPrint.h.
		
	*/
	int	  farEastFontOpt;	
							

	/**
		Transparency flattening quality control. <code>1</code> is fastest; <code>5</code> is best.
		It applies to a printer.
		The default is <code>3</code>.
	*/
	int   transQuality;		/* Transparency flattening quality - 1 is fastest, 5 is best */

	/**
		<code>1</code> uses OverPrint Preview, <code>0</code> does not. It applies to a printer.
		
		The default is <code>0</code>.
	*/
	int   doOPP;

							
	/** 
		Optional PDFLPrintCancelProc. It applies to both a
		PostScript printer and a file.
	*/
	PDFLPrintCancelProc cancelProc;


	/** Optional pointer to data passed to <code>cancelProc</code>. It applies to
		both a PostScript printer and a file.
	*/
	void * clientData;


	/** Spooler ID from StartDoc(). */
    int   startResult;  

	/** 
	Used for the Windows DOCINFO structure (<code>lpszDocName</code>)
	that points to a <code>NULL</code>-terminated string that specifying the name of
	the document. In case of printing/exporting to Postscript, this field sets
    the Title DSC comment in the generated Postscript file. inFileNameW has precedence over the
    deprecated inFileName. If neither is provided, the Title DSC comment is filled with the name of
    pdf document.
	*/
	ASUns16 *inFileNameW;


	/** 
	Used for the Windows DOCINFO structure (<code>lpszOutput</code>). It
	points to a <code>NULL</code>-terminated string that specifies the name of an
	output file. If this pointer is <code>NULL</code>, the output will be sent to the
	device identified by <code>deviceName</code>, <code>driverName</code>, and
	<code>portName</code>. It applies to a printer.
	*/
	ASUns16 *outFileNameW;	

	/**
	The name of the device to print to (for example, <code>"Distiller Assistant
	v3.01"</code>). Available devices can be found in the Windows Registry at
	<code>HKEY_CURRENT_USER\\Software\\Microsoft\\Windows NT\\CurrentVersion\\Devices</code>. It applies to a printer.
	*/
	ASUns16 *deviceNameW;		


	/** 
	The name of the driver (for example, <code>"winspool"</code>). See the registry key for
	<code>deviceName</code>. It applies to the printer.
	*/
	ASUns16 *driverNameW;		


	/** 
	The name of the port (for example, <code>"Ne00"</code>). See the registry key for
	<code>deviceName</code>. It applies to the printer.
	*/
	ASUns16 *portNameW;	


	/** 
	<p>(Optional) Allows you to pass a DEVMODE structure in if you have one. </p>

	<p>If you do not pass in your own DEVMODE structure, 
	PDFL creates its own by calling <code>OpenPrinter()</code>, 
	<code>DocumentProperties()</code>, and <code>ClosePrinter()</code>. </p>

	<p>PDFL never calls <code>SetPrinter()</code> and has no facility 
	to offer the features provided by <code>SetPrinter()</code> 
	(for example, pause/resume, deleting jobs, changing printer 
	status, etc.). <code>DocumentProperties()</code> is called only
	in the case where the client does not provide a DEVMODE.</p>

	@note If you provide your own DEVMODE, call <code>DocumentProperties()</code> to populate it
	or display the print dialog box.

	*/
	DEVMODEW * pDevModeW;
		
	/**
	(Optional) Allows you to pass a DeviceContext structure.

	<p>If you do not pass in your own DeviceContext structure
	PDFL creates its own.</p>

	@note you should provide your own DeviceContext only 
	if you register to the PDPageDirectDrawToPlatform notification 
	and	wish to draw extra content to a page after PDFL has finished
	printing it.

	*/
	HDC pDC;

	/** Optional pointer to structure containing pointers to user callback functions */
	PDFLPrintUserCallbacks userCallbacks;

	/** Optional pointer to a structure where you can add additional PPD features like staple, input tray, 
	output tray, duplex mode etc. for output to PostScript file. For more details,
	see <code>_t_PPDFeatures</code>*/
	PPDFeaturesP PPDFeatures;

	/** (Optional)The name and path of the PPD file from which the PPD features will be emitted to a 
	PostScript file. Use this field alongwith <code>PPDFeaturesP PPDFeatures</code>. For more details, 
	see <code>_t_PPDFeatures</code>.*/
	ASText ppdFileName;

} PDFLPrintUserParamsRec;

#elif MAC_PLATFORM

/** 
	<p>Used to control printing with PDFLPrintDoc. </p>

	<p>To print to a PostScript file, set <code>emitToFile</code> to true, fill in
    <code>printParams</code>, and create a writeable stream.</p>

    <p>To print to a printer, set <code>emitToPrinter</code> to <code>true</code>, fill in <code>printParams</code>,
    and optionally fill in the command.</p>
	
	@note UNIX supports only PostScript to a file or printer, as controlled by 
	the (mutually exclusive) <code>emitToFile</code> and <code>emitToPrinter</code> parameters.

	@see PDFLPrintDoc
	@see PDFLPrintCancelProc
*/

typedef struct _t_PDFLPrintUserParams {

	/** Size of the data structure. It must be set to
		<code>sizeof(PDFLPrintUserParamsRec)</code>.
	*/
	ASInt32 size; 
	

	/** Create a PostScript file. Set either this parameter or
		<code>emitToPrinter</code>, but not both.
	*/
	ASBool emitToFile; 


	/** PDPrintParams structure. It is used both for creating a
		PostScript file and printing to a printer.
	*/
	PDPrintParams printParams;


	/** Writable ASStm that points to a file stream or proc stream. It applies to
		both a PostScript printer and a file.
	*/
	ASStm printStm; 


	/** Width of paper in points. The default value is <code>612</code>. It applies to a
		PostScript printer.
	*/
	ASUns16 paperWidth;	


	/** Height of paper in points. The default value is <code>792</code>. It applies to a
		PostScript printer.
	*/
	ASUns16 paperHeight;


	/** Number of fonts that should not be downloaded. It applies to
		both a PostScript printer and a file.
	*/
	ASInt32 dontEmitListLen; 


	/** List of fonts (T1, TrueType, CID) that should not be downloaded.
		It applies to both a PostScript printer and a file.
	*/
	char ** dontEmitList; 


	/** Output a PDF file to a PostScript printer. Set either this
		parameter or <code>emitToFile</code>, but not both.
	*/
	ASBool emitToPrinter; 
	
	
	/** DO NOT USE: Classic (non-Carbon) printing is no longer supported. */
	void * printRecord;
	
#ifndef IOS_ENV
	/** Carbon print session - may be <code>NULL</code>. It applies to a printer. */
	PMPrintSession printSession;
	
	
	/** Carbon print settings - may be <code>NULL</code>. It applies to a printer. */
	PMPrintSettings printSettings;
	
	
	/** Carbon page format - may be <code>NULL</code>. It applies to a printer. */
	PMPageFormat pageFormat;
#endif

	/** 
		Optional PDFLPrintCancelProc. It applies to both a
		PostScript printer and a file.
	*/
	PDFLPrintCancelProc cancelProc;


	/** Optional pointer to data passed to <code>cancelProc</code>. It applies to
		both a PostScript printer and a file.
	*/
	void * clientData;
	
	
	/** The page on which to start printing, using zero-based numbering. It applies to a printer. */
	int   startPage;
	

	/** The page on which to finish printing. It applies to a printer. */
	int   endPage;
	

	/** <code>1</code> turns on shrink to fit, <code>0</code> turns it off. It applies to a PostScript
		printer.
	*/
	int   shrinkToFit;
	

	/** <code>1</code> turns on printing annots, <code>0</code> turns it off. It applies to a printer. */
	int   printAnnots;
	

	/** PostScript level. It applies to a PostScript printer. */
	int   psLevel;
	

	/** The number of copies to print. It applies to a printer and to a PostScript file. */
	int   nCopies;
	

	/** <code>1</code> turns on binary data, <code>0</code> turns it off. It applies to a PostScript printer. */
	int   binaryOK;

	/** <code>1</code> turns on emit half-tones, <code>0</code> turns them off. It applies to a PostScript
		printer.
	*/
	int   emitHalftones;
	
	/** <code>1</code> reverses print order, <code>0</code> does regular order. It applies to a printer. */
	int   reverse;

	/**
		<code>1</code> uses OverPrint Preview, <code>0</code> does not. It applies to a printer.
		
		The default is <code>0</code>.
	*/
	int   doOPP;

	/** Optional pointer to structure containing pointers to user callback functions */
	PDFLPrintUserCallbacks userCallbacks;

	/** Optional pointer to a structure where you can add additional PPD features like staple, input tray, 
	output tray, duplex mode etc. for output to PostScript file. For more details,
	see <code>_t_PPDFeatures</code>*/
	PPDFeaturesP PPDFeatures;

	/** (Optional)The name and path of the PPD file from which the PPD features will be emitted to a 
	PostScript file. Use this field alongwith <code>PPDFeaturesP PPDFeatures</code>. For more details, 
	see <code>_t_PPDFeatures</code>.*/
	ASText ppdFileName;
	
} PDFLPrintUserParamsRec;
#endif

/** 
	<p>Used to control the emitting of PPD dependent features to a PostScript file
	when printing with PDFLPrintDoc. These features are emitted only if <code>emitToFile
	</code> is set to <code>true</code>. Please see PDFLPrintDoc for more details.</p>
	
	<p>This can be used to generate PPD dependent PostScript file on all platforms.</p>

	<p>All PPD features in the structure are optional. Set only the ones you want to be 
	emit to the output PostScript file.</p>

	<p>The PPD file and its path should be passed in as <code>ppdFileName</code> inside 
	<code>PDFLPrintUserParamsRec</code>.</p>

    <p>If the PPD feature specified here does not exist in the passed PPD file, then it
	will not be outputted in the generated PostScript file. For eg. if Upper is passed
	as <code>InputSlot</code> and there is no tray as Upper in the PPD file, then the
	generated PostScript file will not contain an entry setting InputSlot to Upper.</p>

	<p>Note: If no PPD file is passed then this structure would be ignored.</p>
	
*/

struct _t_PPDFeatures{

	/** Size of the data structure. It must be set to
	<code>sizeof(PPDFeaturesRec)</code>.
	*/
	ASUns32 size;

	/** Optional. Specifies the Input Tray of the printer. For eg. Upper*/ 
	ASText InputSlot;

	/** Optional. Specifies the Onput Tray of the printer. For eg. Left*/ 
	ASText OutputBin;

	/** Optional. Specifies the Duplex mode. For eg. DuplexTumble*/ 
	ASText Duplex;

	/** Optional. Specifies the location for stapling. For eg. SinglePortrait. Either 
	<code>StapleLocation</code> should be used or both <code>StapleX</code> and <code>StapleY</code>
	to specify the location for stapling.*/ 
	ASText StapleLocation;

	/** Optional. Specifies the StapleX. For eg. Left. Also read the comment
	on <code>StapleLocation</code>*/ 
	ASText StapleX;

	/** Optional. Specifies the StapleY. For eg. Top. Also read the comment
	on <code>StapleLocation</code>*/ 
	ASText StapleY;

	/** Optional. Specifies the orientation of the stapling. For eg. 45*/ 
	ASText StapleOrientation;

	/** Optional. Specifies when to staple. For eg. EndOfPage*/ 
	ASText StapleWhen;

};



/* The definitions of PDFLPrintDoc and PDFLPrintPDF have been moved to
   PDFLCalls.h and PDFLProcs.h. */



#ifdef __cplusplus
}
#endif

#if defined( WINDOWS ) || defined( WIN_ENV )
#pragma pack (pop, peexpt )		/* reset to /Zp  */
#endif

#endif /* _H_PDFLPrint */

