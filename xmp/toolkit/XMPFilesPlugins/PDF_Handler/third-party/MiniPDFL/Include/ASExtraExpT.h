/*********************************************************************

 ADOBE SYSTEMS INCORPORATED
 Copyright (C) 1999-2006 Adobe Systems Incorporated
 All rights reserved.

 NOTICE: Adobe permits you to use, modify, and distribute this file
 in accordance with the terms of the Adobe license agreement
 accompanying it. If you have received this file from a source other
 than Adobe, then your use, modification, or distribution of it
 requires the prior written permission of Adobe.

 ---------------------------------------------------------------------

 ASExtraExpT.h

 - Types, macros, structures, etc. required to use the ASExtra HFT.

*********************************************************************/

#ifndef _H_ASExtraExpT
#define _H_ASExtraExpT

#include "Environ.h"

#if PLUGIN || ACROBAT_LIBRARY
#include "CoreExpT.h"
#include "ASExpT.h"
#else
#include "ASBasic.h"
#include "ASTypes.h"
#endif

#ifdef __cplusplus
extern "C" {
#endif

/** */
typedef ASUTF16Val ASUniChar;
/** */
typedef ASUns16 ASCountryCode;
/** */
typedef ASUns16 ASLanguageCode;


/** 
*/
enum {

	/** Does nothing. */
	kASTextFilterIdentity,					

	/** Normalizes line endings (equivalent to ASTextNormalizeEndOfLine()). */
	kASTextFilterLineEndings,

	/** Makes all text upper case. DEPRECATED: Case is not a reliably localizable concept. Do not use this. */
	kASTextFilterUpperCase,

	/** Makes all text lower case. DEPRECATED: Case is not a reliably localizable concept. Do not use this. */
	kASTextFilterLowerCase,			

	/** Changes any ASText to "XXX" (for debugging). */
	kASTextFilterXXXDebug,			

	/** Makes all text except <code>scanf</code> format strings upper case. */
	kASTextFilterUpperCaseDebug,

	/** Makes all text except <code>scanf</code> format strings lower case. */
	kASTextFilterLowerCaseDebug,

	/** Removes stand-alone ampersands, and turns <code>&&</code> into <code>&</code> */
	kASTextFilterRemoveAmpersands,			

	/** Changes any full width ASCII variants to their lower-ASCII version. */
	/* For example, <code>0xFF21</code> (full width <code>'A'</code>) becomes <code>0x0041</code> (ASCII <code>'A'</code>) */
	kASTextFilterNormalizeFullWidthASCIIVariants,			

	/** Removes line endings and replaces them with spaces. */
	kASTextRemoveLineEndings,

	/** Reserved. Do not use. */
	kASTextFilterRsvd1 = 1000,				

	/** An invalid filter type. */
	kASTextFilterUnknown = -1				
};
/**
	Constants that specify filter types used to modify text	objects. 
	@see ASTextFilter 
*/
typedef ASEnum16 ASTextFilterType;


/** 
	ASCabinet objects can be used to store arbitrary key/value pairs.  The
	keys are always <code>NULL</code>-terminated strings containing only low ASCII
	alphanumeric characters. The various types of values are enumerated here.
*/
enum {

	/** An ASBool. */
	kASValueBool,		

	/** An ASInt32. */
	kASValueInteger,	

	/** An ASAtom. */
	kASValueAtom,		

	/** A double precision floating-point number. */
	kASValueDouble,		

	/** A <code>NULL</code>-terminated, unencoded string. */
	kASValueString,		

	/** An ASText object. */
	kASValueText,		

	/** A binary blob of any size. */
	kASValueBinary,		

	/** A pointer to something outside the cabinet. */
	kASValuePointer,	

	/** Another cabinet. */
	kASValueCabinet,	

	/** The key exists but has no useful value. For example, the key may be a placeholder. */
	kASValueNull,		

	/** An ASUns32. */
	kASValueUns,		

	/** An ASInt64. */
	kASValueInt64,	

	/** An ASUns64. */
	kASValueUns64,
			
	/** An invalid type. */
	kASValueUnknown	= -1	
};
/**
	A constant that specifies the various types of values in ASCab objects. 
	ASCab objects can be used to store arbitrary key/value pairs. The 
	keys are always <code>NULL</code>-terminated strings containing only 
	low ASCII alphanumeric characters. 
	@see ASCabFromEntryList 
	@see ASCabGetType 
*/
typedef ASEnum16 ASCabValueType;


/** Cabinet keys are <code>NULL</code>-terminated C strings. This constant
	declares the maximum length of one component of that string.

	<p>The characters in the key string must all be low ASCII
	alphanumeric characters, such as <code>'0' - '9'</code>, <code>'a' - 'z'</code>, <code>'A' - 'Z'</code>.</p>

	<p>You can burrow through multiple levels of a cabinet heirarchy by
	passing in a string of individual key names separated by
	colons.</p> 
	
	<p>For example,	<code>ASCabGetInt(cab, "Hello:World", -1);</code> is equivalent to
	<code>ASCabGetInt(ASCabGetCab(cab, "Hello"), "World", -1);</code>.</p>
	
	<p>Similarly, <code>ASCabPutInt(theCab, "Hello:World", 33);	</code>	will create an 
	integer key named <code>"World"</code> inside the <code>"Hello"</code> cabinet
	inside theCab, creating the <code>"Hello"</code> key and cabinet if
	necessary.</p>
*/
#define MAX_ASCAB_KEY 1024


/**
	A data structure representing a cabinet entry. The first entry 
	in each descriptor specifies the name of the key, the second 
	field contains the type, and the following fields contain the 
	values. The entry list must end with a descriptor containing 
	<code>NULL</code> for the key name. It can be used as shown below to 
	construct an ASCab: 

	<p><code>ASCabEntryRec cabData[] = {{"key1", kASValueInteger, 1}, 
	{"key2", kASValueInteger, -1}, {"key3", kASValueBool, false}, 
	{NULL}};</code> </p>

	<p><code>ASCab CreateDefaultCab() { return ASCabFromEntryList (cabData); }</code></p> 

	<p>This example uses just three values for each record. However, 
	more may be required, such as a <code>double</code>: </p>

	<p><code>{"keyDouble", kASValueDouble, 0, NULL, double}</code></p>

	<p>For a <code>string</code>, the following code could be used: </p>

	<p><code>{"keyString", kASValueString, 0, (void*)string}</code></p>
	@see ASCabFromEntryList 
*/
typedef struct _t_ASCabEntryRec {

	/** The name of the key.
	*/
	const char *	keyName;

	/** The supported ASCabValueTypes are:
		<TABLE rules="all" cellspacing="1">
		<TR><TH>Type</TH><TH>Description</TH></TR>
		<TR><TD>kASValueBool</TD><TD>A boolean value. <code>intVal</code> contains the value.</TD></TR>
		<TR><TD>kASValueInteger</TD><TD>An integer value. <code>intVal</code> contains the value.</TD></TR>
		<TR><TD>kASValueAtom</TD><TD>An atom. <code>intVal</code> contains the value.</TD></TR>
		<TR><TD>kASValueDouble</TD><TD>A double value. <code>doubleVal</code> contains the value.</TD></TR>
		<TR><TD>kASValueString</TD><TD><code>ptrVal</code> points to a <code>NULL</code>-terminated C string.</TD></TR>
		<TR><TD>kASValueText</TD><TD><code>ptrVal</code> points to a <code>NULL</code>-terminated string containing script text, 
		and <code>intVal</code> specifies the ASScript code for the text.</TD></TR>
		<TR><TD>kASValueBinary</TD><TD><code>ptrVal</code> points to the binary data, and <code>intVal</code> specifies the size of the data.</TD></TR>
		<TR><TD>kASValueNull</TD><TD>Creates an entry with a NULL value.</TD></TR>
		</TABLE>

		<p>No other types are supported (specifically kASValueCabinet and kASValuePointer). You can build 
		nested cabinets using the <code>"key: key"</code> syntax for the <code>keyNames</code>.</p>
	*/
	ASCabValueType	type;

	/** See above.
	*/
	ASInt32			intVal;
	/** See above.
	*/
	const void *	ptrVal;
	/** See above.
	*/
	double			doubleVal;
} ASCabEntryRec;


/**
	Used when enumerating the values inside a cabinet. 
	@param theCab The cabinet being enumerated. 
	@param theKey The key name of an entry in the cabinet. 
	@param itsType The type of the value associated with <code>theKey</code>. 
	@param clientData User-supplied data passed into ASCabEnum.
	
	@return <code>true</code> to continue enumeration, <code>false</code> to halt enumeration. 
	
	@see ASCabEnum 
*/
typedef ACCBPROTO1 ASBool (ACCBPROTO2 *ASCabEnumProc)
	(ASCab theCab, const char *theKey, ASCabValueType itsType, void *clientData);


/**
	Used when enumerating the values inside a constant cabinet. 
	The callback procedure must not add, delete, or modify items 
	in <code>theCab</code> during the enumeration. 
	@param theCab The cabinet being enumerated. 
	@param theKey The key name of an entry in the cabinet. 
	@param itsType The type of the value associated with <code>theKey</code>. 
	@param clientData User-supplied data passed into ASCabEnum.

	@return <code>true</code> to continue enumeration, <code>false</code> to halt enumeration. 
	
	@see ASConstCabEnum 
*/
typedef ACCBPROTO1 ASBool (ACCBPROTO2 *ASConstCabEnumProc)
	(ASConstCab theCab, const char *theKey, ASCabValueType itsType, void *clientData);


/** A value that determines the actions to be taken when ASCabMunge() is called. <code>keyCab</code>
	is the ASCab that provides the keys determining how <code>theCab</code> is to be changed.
	During an ASCabMunge() operation, the key cab will not be altered.
*/
enum {

	/** Any keys in <code>keyCab</code> are removed from <code>theCab</code>. */
	kASMungeRemove,

	/** Any keys in <code>theCab</code> which are not also in <code>keyCab</code> are removed.	*/
	kASMungeRemoveUnknown,

	/** Any keys in <code>keyCab</code> which are also in <code>theCab</code> and have the same value in <code>theCab</code> are removed. */
	kASMungeRemoveDefaults,

	/** Any keys in <code>theCab</code> which are also in <code>keyCab</code> but have different values are removed.	*/
	kASMungeRemoveBadValues,

	/** All key/value pairs in <code>keyCab</code> are copied into <code>theCab</code>. */
	kASMungeCopy,

	/** Any keys in <code>theCab</code> which are also in <code>keyCab</code> are replaced with the values in <code>keyCab</code>.	*/
	kASMungeReplace,

	/** Any keys in <code>keyCab</code> which are not in <code>theCab</code> are copied over to <code>theCab</code>. */
	kASMungeCopyMissing,

	/** Any keys in <code>keyCab</code> with a value of <code>NULL</code> are removed from <code>theCab</code>. */
	kASMungeRemoveNulls
};
typedef ASEnum16 ASCabMungeAction;


/**
	A deallocation callback that can be associated with a pointer 
	in an ASCab. When the reference count of the pointer falls 
	to zero, this callback is called to free the resources associated 
	with the object the pointer references. 
	@param ptr IN/OUT The value stored in an ASCab. 
	@see ASCabPutPointer 
	@see ASCabGetPointerDestroyProc 
*/
typedef ACCBPROTO1 void (ACCBPROTO2 *ASCabPointerDestroyProc)
	(void *ptr);

/* ASReportProc
**
** Used to report errors, warnings, and other messages to the user.
**
** The reportType indicates what sort of information is being
** reported. Perhaps the best way to think of this is to consider what
** sort of icon the user should see when they read the message.
**
** The errorCode argument should be set to one of the defined error
** codes (i.e. the same ones you would use when raising errors). It
** can also be set to 0 (indicating no error).
**
** The 'message' field specifies the text the user should read. If the
** 'message' field is NULL the system will retrieve the message
** associated with the errorCode. If both 'message' and 'errorCode'
** are specified the 'message' argument will be used.
**
** If the 'replacementText' is not NULL the system will attempt to
** replace the string "%s" in the message with the replacement
** text. This applies whether the text is specified via the 'message'
** argument or retrieved from the system using the 'errorCode'
** argument.
**
** The moreInfo field is not used as yet.
**
** The reportProcData is a pointer to whatever data is associated with
** the reportProc (which should be passed to you with the report
** proc).
**
** All of these arguments are handed off to the reportProc. It's the
** reportProc's responsibility to destroy all objects passed to it,
** and it may destroy them at any time.
*/


/** Used in an ASReportProc to indicate what kind of information is being reported.
	@see ASReportProc 
*/
enum {

	/** A note. */
	kASReportNote,

	/** A warning. */
	kASReportWarning,

	/** An error. */
	kASReportError
};
typedef ASEnum16 ASReportType;

#define kMoreTextKey	"MoreText"


/**
	<p>A report proc can be used to report errors, warnings, and 
	other messages to the user. Normally a report proc will use a dialog
	to notify the user of an error, but in some 
	contexts (such as during batch processing) it may either log the 
	error or warning to a file or ignore it. </p>

	<p>It is this callback's responsibility to destroy all objects 
	passed to it, and it may do so at any time. </p>

	@param reportType The type of information that is reported. 
	
	@param errorCode An error code defined by the system or 
	by ASRegisterErrorString(). If message is not <code>NULL</code>, the error code 
	can be <code>0</code>. 
	@param message Specifies the text the user should read. 
	If the message field is <code>NULL</code>, the system will retrieve the 
	message associated with the <code>errorCode</code>. If both <code>message</code> and 
	<code>errorCode</code> are specified, the message argument is used. 
	@param replacementText If this replacement text is not <code>NULL</code>, 
	the system will attempt to replace the string <code>'%s'</code> in the 
	message with the replacement text. This applies whether 
	the text is specified via the <code>message</code> argument or retrieved 
	from the system using the <code>errorCode</code> argument. 
	@param moreInfo Not currently used. The report proc will 
	destroy this cabinet immediately. 
	@param reportProcData A pointer to the data associated 
	with the report proc (which should be passed in when 
	the report proc is acquired).
	@see AVAppGetReportProc 
	@see AVCommandGetReportProc 
*/
typedef ACCBPROTO1 void (ACCBPROTO2 *ASReportProc)
	(ASReportType reportType, ASInt32 errorCode, ASText message, ASText replacementText,
	 ASCab moreInfo, void *reportProcData);

/** */
typedef ACCBPROTO1 ASText (ACCBPROTO2 *ASTextEvalProc)(ASCab params);


typedef struct _t_ASStatusMonitorProcs {

	/** The size of the structure. */
	ASSize_t			size;

	/** A pointer to the progress monitor. */
	ASProgressMonitor	progMon;

	/** A pointer to client data, which can be of any type. */
	void				*progMonClientData;

	/**
		This call has been replaced by ASCancelProc(). 

		<p>A callback to check for cancelling operations. An ASCancelProc 
		is typically passed to some method that takes a long time 
		to complete. At frequent intervals, the method calls the 
		ASCancelProc. If it returns <code>true</code>, the method cancels 
		its operation; if <code>false</code>, it continues. </p>

		@param clientData User-supplied data passed to the ASCancelProc.
		@return <code>true</code> if the processing is cancelled, <code>false</code> otherwise. 
		@see PDFLPrintCancelProc (Only available with PDF Library SDK) 
		
		@see AVAppGetCancelProc 
	*/
	ASCancelProc		cancelProc;

	/** A pointer to client data for the cancel procedure. */
	void				*cancelProcClientData;

	/** The report procedure. */
	ASReportProc		reportProc;

	/** A pointer to client data for the report procedure. */
	void				*reportProcClientData;
} ASStatusMonitorProcsRec, *ASStatusMonitorProcs;

#ifdef __cplusplus
}
#endif

#endif /* _H_ASExtraExpT */
