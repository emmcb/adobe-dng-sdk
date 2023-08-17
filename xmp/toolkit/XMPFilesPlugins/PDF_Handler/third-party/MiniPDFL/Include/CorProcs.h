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
 Copyright (C) 1994-2006 Adobe Systems Incorporated
 All rights reserved.

 NOTICE: Adobe permits you to use, modify, and distribute this file
 in accordance with the terms of the Adobe license agreement
 accompanying it. If you have received this file from a source other
 than Adobe, then your use, modification, or distribution of it
 requires the prior written permission of Adobe.

 ---------------------------------------------------------------------

 CorProcs.h

 - Catalog of the "core" exported functions; this table is handed off
   to the plug-in at initialization time,

*********************************************************************/


/**
	Raises an exception. Plug-ins can raise any exception defined 
	in the AcroErr.h header file using the <code>ErrBuildCode</code> macro, 
	or can define their own exceptions using ASRegisterErrorString(). 
	See Errors for a list of predefined exceptions. 

	<p>If the code that calls ASRaise() gets control as a result 
	of a non-Acrobat event (such as a drag and drop event on 
	some platforms), this method fails since there is no Acrobat 
	viewer code in the stack to handle the exception. </p>
	@param error An error code for the exception to raise. Error 
	codes have three parts: severity, system, and error number. 
	Use <code>ErrBuildCode</code> to build an error code for an existing 
	error.
	@see ASGetErrorString 
	@see ASRegisterErrorString 
	@see RERAISE 
	@since PI_ACROSUPPORT_VERSION >= 0x00020000
*/
NPROC(void, ASRaise, (ASErrorCode error))
#if !ACROBAT_LIBRARY

/**
	Pushes an exception frame buffer and a frame-restoration 
	callback onto the stack. The <code>restoreFunc</code> should be a function 
	matching the following prototype. 
	@param asEnviron IN/OUT Represents a stack environment that is 
	restored if an exception occurs. On Windows and Mac OS, 
	this is a <code>jmp_buf</code>, which is an array of integers. 
	@param restoreFunc IN/OUT Should be a function matching the following 
	prototype: <code>ACCB1 void ACCB2 RestorePlugInFrame( void* asEnviron)</code> 
	
	@note You will probably never call ASPushExceptionFrame() 
	directly; use the <code>DURING</code> macro instead. 

	@since PI_ACROSUPPORT_VERSION >= 0x00020000
*/
SPROC(void, ASPushExceptionFrame, (void *asEnviron, ACRestoreEnvironProc restoreFunc), ACPushExceptionFrame)

/**
	Pops an exception frame off the stack. 

	@note You will probably never call ASPopExceptionFrame() directly; 
	it is called for you as appropriate from within the <code>HANDLER</code>, 
	<code>E_RETURN</code> and <code>E_RTRN_VOID</code> macros. 
	@since PI_ACROSUPPORT_VERSION >= 0x00020000
*/
SPROC(void, ASPopExceptionFrame, (void), ACPopExceptionFrame)

/**
	Gets the error code for the most recently raised exception. 
	See Error Systems for a list of predefined exceptions. 
	@return Exception error code. 
	@see ASRaise 
	@see ASGetErrorString 
	@see ASRegisterErrorString
	@ref ErrorSystems
	@since PI_ACROSUPPORT_VERSION >= 0x00020000
*/
SPROC(ASErrorCode, ASGetExceptionErrorCode, (void), ACGetExceptionErrorCode)
#endif


/**
	Gets the ASAtom for the specified string. You can also use 
	this method to create an ASAtom, since it creates one for 
	the string if one does not already exist. 

	<p>If an ASAtom already exists for <code>nameStr</code>, the existing ASAtom 
	is returned. Thus, ASAtom objects may be compared for equality of 
	the underlying string.</p> 

	<p>Because ASAtom objects cannot be deleted, they are useful for strings 
	that are used many times in an Acrobat viewer session, but 
	are not recommended for strings that have a short lifetime. 
	For the same reason, it is not a good idea to create large 
	numbers of ASAtom objects. </p>
	@param nameStr The string for which an ASAtom is created. 
	@return The ASAtom corresponding to <code>nameStr</code>. 
	@see ASAtomExistsForString 
	@see ASAtomGetCount (Only available with the PDF Library SDK) 
	@see ASAtomGetString 
	@since PI_ACROSUPPORT_VERSION >= 0x00020000
*/
NPROC(ASAtom, ASAtomFromString, (const char *nameStr))

/**
	Tests whether an ASAtom exists for the specified string. 
	
	@param nameStr The string to test. 
	@param atom (Filled by the method, may be <code>NULL</code>) If the ASAtom corresponding 
	to <code>nameStr</code> already exists, it is returned in atom. Pass 
	<code>NULL</code> to simply check whether the ASAtom already exists.
	@return <code>true</code> if an ASAtom already exists for <code>nameStr</code>, <code>false</code> otherwise. 
	
	@see ASAtomFromString 
	@see ASAtomGetCount (Only available with PDF Library SDK) 
	@see ASAtomGetString 
	@since PI_ACROSUPPORT_VERSION >= 0x00020000
*/
NPROC(ASBool, ASAtomExistsForString, (const char *nameStr, ASAtom *atom))

/**
	Gets the string associated with the specified ASAtom. 
	@param atm The ASAtom whose string is obtained. 
	@return The string corresponding to <code>atom</code>. It returns an empty string 
	if <code>atom == ASAtomNull</code>, or <code>NULL</code> if the atom has not been defined. 
	
	@see ASAtomExistsForString 
	@see ASAtomFromString 
	@since PI_ACROSUPPORT_VERSION >= 0x00020000
*/
NPROC(const char *, ASAtomGetString, (ASAtom atm))


/**
	<p>Deprecated as of Acrobat 8.0.</p>

	<p>Creates a callback that allows the Acrobat viewer to call 
	a function in a plug-in. All plug-in functions that are 
	called by the Acrobat viewer must be converted to callbacks 
	before being passed to the viewer. </p>

	<p>Whenever possible, plug-ins should not call ASCallbackCreate() 
	directly, but should use the macros ASCallbackCreateProto(), 
	ASCallbackCreateNotification(), and ASCallbackCreateReplacement(). 
	These macros (which eventually call ASCallbackCreate()) have 
	two advantages: </p>
	<ul>
	<li> They allow compilers to perform type checking, eliminating 
	one extremely common source of plug-in bugs. </li>
	<li> They handle <code>extensionID</code> automatically. </li>
	</ul>

	<p>Plug-ins must use ASCallbackCreate() directly, for example, 
	when calling a Mac toolbox routine that expects a 
	<code>ProcPtr</code>. </p>
	@param extensionID IN/OUT The <code>gExtensionID</code> extension that calls 
	<code>proc</code>. 
	@param proc IN/OUT The user-supplied procedure for which a callback 
	is created. 
	@return The newly-created callback. 
	@see ASCallbackDestroy 
	@see AVAppRegisterNotification 
	@see AVAppUnregisterNotification 
	@see ASCallbackDestroy 
	@see ASCallbackCreateReplacement 
	@see ASCallbackCreateProto 
	@see ACCB1 
	@see ACCB2 
	@see DEBUG 
	@see ASCallbackCreate 

	@note If you call ASCallbackCreate() directly, you are actually 
	invoking the ASCallbackCreate() macro, not this HFT routine. 
	The ASCallbackCreate() macro takes only one parameter, the 
	<code>proc</code>, and passes that information into this underlying HFT 
	routine as the second argument. The first argument is always 
	set to <code>gExtensionID</code>, which should be the extension identifier of 
	your plug-in. 
	@since PI_ACROSUPPORT_VERSION >= 0x00020000
*/
ANPROC(ASCallback, ASCallbackCreate, (ASExtension extensionID, void *proc))

/**
	<p>Deprecated as of Acrobat 8.0.</p>

	<p>Destroys a callback. </p>
	@param callback IN/OUT The callback to destroy. 
	@see ASCallbackCreate 
	@since PI_ACROSUPPORT_VERSION >= 0x00020000
*/
ANPROC(void, ASCallbackDestroy, (ASCallback callback))

/**
	Gets the specified version of the Host Function Table (HFT) 
	that has the specified name. If you want to get one of the 
	Acrobat viewer's built-in HFTs, use the predefined global 
	variables for the HFT Values instead of this method. 
	@param name The name of the HFT to obtain. 
	@param version The version number of the HFT to obtain.
	@return The specified HFT, or <code>NULL</code> if the HFT does not exist. 
	@see HFTReplaceEntry 
	@see HFTReplaceEntryEx 
	@since PI_ACROSUPPORT_VERSION >= 0x00020000
*/
SPROC(HFT, ASExtensionMgrGetHFT, (ASAtom name, ASVersion version), ASGetHFTByNameAndVersion)

/**
	<p>Gets information about the Acrobat viewer application under 
	which the plug-in is running. Use this method if your plug-in's 
	functionality depends on the Acrobat viewer that is running.</p> 

	<p>The method can return a product name, or check whether the 
	current product allows editing. Do not rely on the product 
	name to determine whether the product can edit files, as 
	product names and feature sets may vary; use the <code>CanEdit</code> 
	selector to do this. </p>
	@param key The key determines whether the method tests 
	editability, or finds which product configuration is running. 
	Its values are:

	<TABLE rules="all" cellspacing="1">
	<TR><TH>Value</TH><TH>Description</TH></TR>
	<TR><TD><code>CanEdit</code></TD><TD>Checks whether editing is allowed in the current environment (regardless of the product name).</TD></TR>
	<TR><TD><code>Product</code></TD><TD>Checks which Acrobat application is running.</TD></TR>
	</TABLE>

	@return The return value's type depends on the request key. Cast 
	the return value to the type you are expecting, based on 
	the key you pass in: 

	<TABLE rules="all" cellspacing="1">
	<TR><TH>Value</TH><TH>Return type</TH></TR>
	<TR><TD><code>CanEdit</code></TD><TD>An ASBool value: <code>true</code> if the current application allows editing, <code>false</code> otherwise.</TD></TR>
	<TR><TD><code>Product</code></TD><TD>A <code>const char*</code> value, one of the following strings:
	<ul>
	<li><code>"Reader"</code>: Adobe Reader</li>
	<li><code>"Exchange"</code>: Acrobat Standard</li>
	<li><code>"Exchange-Pro"</code>: Acrobat Professional</li>
	<li><code>"Acrobat PDF LIbrary"</code>: Acrobat PDF Library</li>
	</ul></TD></TR>
	</TABLE>

	@exception UNDEFINED_CONFIGURATION_SELECTOR is returned if an unknown value is passed as <code>key</code> (see CoreExpT.h). 
	
	@since PI_ACROSUPPORT_VERSION >= 0x00020000
*/
ANPROC(void *, ASGetConfiguration, (ASAtom key))

/* Acrobat 4.0 additions */

/**
	Enumerates all ASExtension objects (valid plug-ins). 
	@param proc A user-supplied callback to call for each plug-in. 
	Enumeration halts if <code>proc</code> returns <code>false</code>. 
	@param clientData A pointer to user-supplied data to pass 
	to <code>proc</code> each time it is called. 
	@param onlyLivingExtensions If <code>true</code>, ASExtension objects that 
	have been unloaded or otherwise deactivated are not enumerated. 
	If <code>false</code>, all ASExtension objects are enumerated.
	@return If <code>proc</code> returned <code>false</code>, the last ASExtension that was enumerated is returned,	<code>NULL</code> otherwise. 
	@see ASExtensionGetFileName 
	@see ASExtensionGetRegisteredName 
	@ingroup Enumerators
	@since PI_ACROSUPPORT_VERSION >= 0x00040000
*/
NPROC(ASExtension, ASEnumExtensions, (ASExtensionEnumProc proc, void *clientData,
									  ASBool onlyLivingExtensions))

/**
	Gets the file name of an ASExtension. 
	@param extension IN/OUT The ASExtension whose file name is obtained. 
	
	@param buffer IN/OUT (Filled by the method, may be <code>NULL</code>) A pointer 
	to a buffer for the file name. Pass <code>NULL</code> to have this method 
	return the length of the file name (excluding a terminating 
	<code>NULL</code> character). 
	@param bufSize IN/OUT The number of bytes in <code>buffer</code>. It is ignored if <code>buffer</code> 
	is <code>NULL</code>. 
	@return The number of characters written into <code>buffer</code>, excluding 
	the <code>NULL</code> character. 
	@see ASExtensionGetRegisteredName 
	@since PI_ACROSUPPORT_VERSION >= 0x00040000
*/
NPROC(ASTArraySize, ASExtensionGetFileName, (ASExtension extension, char *buffer, ASTArraySize bufSize))

/**
	Gets the registered name associated with a plug-in. 
	@param extension IN/OUT The ASExtension whose name is obtained. 
	
	@return An ASAtom representing the plug-in name, or <code>NULL</code> if the 
	name could not be identified. 
	@see ASExtensionGetFileName 
	@since PI_ACROSUPPORT_VERSION >= 0x00040000
*/
NPROC(ASAtom, ASExtensionGetRegisteredName, (ASExtension extension))
