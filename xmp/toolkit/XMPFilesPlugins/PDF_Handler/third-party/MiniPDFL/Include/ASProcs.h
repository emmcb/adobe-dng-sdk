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

 ASProcs.h

 - Catalog of functions exported by AcroSupport.

*********************************************************************/

#if DEBUG

/**
	Allocates and returns a pointer to a memory block containing 
	the specified number of bytes. 
	@param nBytes IN/OUT The number of bytes for which space is allocated. 
	
	@return A pointer to the allocated memory. Returns <code>NULL</code> on failure. 
	
	@see ASrealloc 
	@see ASfree 
	@since PI_ACROSUPPORT_VERSION >= 0x00020000
*/
SPROC(void *, ASmalloc, (os_size_t nBytes), ASHFTClientASmalloc)

/**
	If possible, extends the given block and simply returns 
	<code>ptr</code>. Otherwise, it allocates a new block of <code>newNBytes</code> bytes, 
	copies the contents from the old pointer into the new block, 
	frees the old pointer, and returns the pointer to the new 
	block. If a new block cannot be allocated, the call fails 
	and <code>ptr</code> is not freed. Reallocating a block to a smaller 
	size will never fail. 
	@param ptr IN/OUT The existing memory block. 
	@param newNBytes IN/OUT The number of bytes the memory block must 
	be able to hold. 
	@return A pointer to memory block. 
	@see ASmalloc 
	@see ASfree 
	@since PI_ACROSUPPORT_VERSION >= 0x00020000
*/
SPROC(void *, ASrealloc, (void *ptr, os_size_t newNBytes), ASHFTClientASrealloc)

/**
	Frees the specified memory block. 
	@param ptr IN/OUT The block of memory to free. 
	@see ASmalloc 
	@see ASrealloc 
	@since PI_ACROSUPPORT_VERSION >= 0x00020000
	
*/
SPROC(void, ASfree, (void *ptr), ASHFTClientASfree)
#else

/**
	Allocates and returns a pointer to a memory block containing 
	the specified number of bytes. 
	@param nBytes IN/OUT The number of bytes for which space is allocated. 
	
	@return A pointer to the allocated memory, <code>NULL</code> on failure. 
	
	@see ASrealloc 
	@see ASfree 
	@since PI_ACROSUPPORT_VERSION >= 0x00020000
	
*/
NPROC(void *, ASmalloc, (os_size_t nBytes))

/**
	If possible, extends the given block and simply returns 
	<code>ptr</code>. Otherwise, it allocates a new block of <code>newNBytes</code> bytes, 
	copies the contents from the old pointer into the new block, 
	frees the old pointer, and returns the pointer to the new 
	block. If a new block cannot be allocated, the call fails 
	and <code>ptr</code> is not freed. Reallocating a block to a smaller 
	size will never fail. 
	@param ptr IN/OUT The existing memory block. 
	@param newNBytes IN/OUT The number of bytes the memory block must 
	be able to hold. 
	@return A pointer to memory block. 
	@see ASmalloc 
	@see ASfree 
	@since PI_ACROSUPPORT_VERSION >= 0x00020000
	
*/
NPROC(void *, ASrealloc, (void *ptr, os_size_t newNBytes))

/**
	Frees the specified memory block. 
	@param ptr IN/OUT The block of memory to free. 
	@see ASmalloc 
	@see ASrealloc 
	@since PI_ACROSUPPORT_VERSION >= 0x00020000
	
*/
NPROC(void, ASfree, (void *ptr))
#endif


/**
	Gets a string describing the specified error/exception. 
	
	@param errorCode The exception whose error string is obtained. 
	This must be a full error code, built with the ErrBuildCode() 
	macro or a user-defined exception returned from ASRegisterErrorString(). 
	See Errors for a list of predefined exceptions. 
	@param buffer (Filled by the method) A buffer into which 
	the string is written.  Make sure to <code>memset</code> the buffer to <code>0</code> before calling
	ASGetErrorString().
	@param lenBuffer The number of characters that buffer can hold.
	@return A useful pointer to <code>buffer</code>.  This does not mean that the function worked.
	You must call <code>strlen</code> on the returned buffer (as long as you <code>memset</code> the buffer to <code>0</code>) to
	determine whether the error code was valid.
	@see ASGetExceptionErrorCode 
	@see ASRegisterErrorString 
	@see ASRaise 
	@since PI_ACROSUPPORT_VERSION >= 0x00020000
*/
NPROC(const char *, ASGetErrorString, (ASErrorCode errorCode, char *buffer, ASTArraySize lenBuffer))

/**
	Registers a new error and string. The error can be used 
	to raise a plug-in-specific exception using ASRaise(). When 
	the exception is raised, its error string can be retrieved 
	using ASGetErrorString() and reported to the user using AVAlertNote(). 
	
	<p>The error system is automatically forced to be ErrSysXtn. 
	(See the list of Error Systems).</p> 

	<p>The error is automatically assigned an error code that is 
	not used by any other plug-in (in the current implementation, 
	the Acrobat viewer increments a counter each time any plug-in 
	requests an error code, and returns the value of the counter). 
	As a result, plug-ins cannot rely on being assigned the 
	same error code each time the Acrobat viewer is launched.</p> 
	
	@ref ErrorSystems

	@param severity The severity of the error being defined. 
	It must be one of the Severities. 
	@param errorString The string describing the exception. 
	This string is used by ASGetErrorString(), and is 
	copied by ASRegisterErrorString(); it may be freed by the 
	plug-in after registering the error.
	@return The newly created error code. Plug-ins should assign the 
	error code returned by this method to a variable if they 
	will use the error code later in the current session. 
	
	@see ASGetErrorString 
	@see ASRaise 
	@since PI_ACROSUPPORT_VERSION >= 0x00020000
*/
NPROC(ASErrorCode, ASRegisterErrorString, (ASErrSeverity severity, const char *errorString))

/**
	Creates a new Host Function Table (HFT) server. An HFT server 
	is responsible for creating an instance of an HFT with the 
	specified version number, and destroying the HFT. 
	@param name The new HFT server's name. 
	@param serverProc (Required) A user-supplied callback that 
	provides an HFT when given a version number. This procedure 
	is called by ASExtensionMgrGetHFT() when another plug-in imports 
	the HFT. 
	@param destroyProc (Optional) A user-supplied callback that 
	destroys the specified HFT (this generally means deallocating 
	the memory associated with the HFT). This procedure is called 
	by HFTDestroy(). 
	@param clientData A pointer to user-supplied data to pass to the HFT server.
	@return The newly created HFT server. 
	@see HFTServerDestroy 
	@see HFTNewEx
	@since PI_ACROSUPPORT_VERSION >= 0x00020000
*/
NPROC(HFTServer, HFTServerNew, (const char *name, HFTServerProvideHFTProc serverProc, HFTServerDestroyProc destroyProc, void *clientData))

/**
	Destroys an HFT server. Call this method only if the HFT 
	will not be used again. 
	@param hftServer IN/OUT The HFT server to destroy. 
	@see HFTServerNew 
	@since PI_ACROSUPPORT_VERSION >= 0x00020000
	
*/
NPROC(void, HFTServerDestroy, (HFTServer hftServer))


/**
	Obsolete.  See HFTNewEx(). Creates a new HFT by calling the specified HFT server's 
	HFTServerProvideHFTProc(). 
	@param hftServer The HFT server for the HFT being created. 
	The HFT server must have been created previously using HFTServerNew(). 
	
	@param numSelectors The number of entries in the new HFT. 
	This determines the number of methods that the HFT can contain; 
	each method occupies one entry.
	@return The newly created HFT. 
	@see ASExtensionMgrGetHFT 
	@see HFTDestroy
	@see HFTNewEx
	@since PI_ACROSUPPORT_VERSION >= 0x00020000
*/
NPROC(HFT, HFTNew, (HFTServer hftServer, ASTCount numSelectors))

/**
	Destroys an existing HFT by freeing all the HFT's memory. 
	Call this method only if you are absolutely sure that neither 
	your plug-in nor any other plug-in will use the HFT again. 
	Because this is usually impossible to know, plug-ins should 
	not destroy HFTs. It is even dangerous to destroy an HFT 
	at unload time, because the order in which plug-ins are 
	unloaded is not specified. 
	@param hft The HFT to destroy.
	@see HFTNew 
	@since PI_ACROSUPPORT_VERSION >= 0x00020000
*/
NPROC(void, HFTDestroy, (HFT hft))

/**
	Replaces the specified entry in the specified HFT. This 
	allows a plug-in to override and replace certain methods 
	in Acrobat's API. See Replaceable Methods for a list of 
	replaceable methods. This method can be used from anywhere 
	in the plug-in, but it makes the most sense for most plug-ins 
	to replace methods in the importReplaceAndRegisterCallback() 
	procedure. Plug-ins register their HFTs in the export callback, 
	but the code to populate the function table is only executed 
	when the first client requests the HFT. 

	<p>Plug-ins can use the <code>REPLACE</code> macro instead of calling HFTReplaceEntry() 
	directly.</p> 

	<p>All plug-ins, and Acrobat itself, share a single copy of 
	each HFT. As a result, when a plug-in replaces the implementation 
	of a method, all other plug-ins and Acrobat also use the 
	new implementation of that method. In addition, once a method's 
	implementation has been replaced, there is no way to remove 
	the new implementation without restarting Acrobat.</p> 

	@param hft The HFT in which a method is replaced. Use 
	ASExtensionMgrGetHFT() to get the HFT, given its name. For 
	the HFTs built into the Acrobat viewer, global variables 
	containing the HFTs have been defined, so you can skip calling 
	ASExtensionMgrGetHFT() for these HFTs.  
	@param sel The entry in the HFT to replace, derived from 
	the method's name by appending <code>SEL</code>. For example, to replace 
	AVAlert, <code>sel</code> must be <code>AVAlertSEL</code>. 
	@param newEntry The function to replace the current one. 
	The function pointer must be converted to an HFTEntry using 
	the ASCallbackCreateReplacement() macro. 
	@param flags The new entry's properties. Currently, only 
	HFTEntryReplaceable is defined.
	@exception xmErrCannotReplaceSelector
	@ref ReplaceableMethods
	@see ASExtensionMgrGetHFT 
	@see HFTGetReplacedEntry 
	@see CALL_REPLACED_PROC 
	@see REPLACE 
	@see ASCallbackCreateReplacement 
	@note The <code>CALL_REPLACED_PROC</code> macro is available to call 
	the previous HFT entry function that was replaced. 
	@since PI_ACROSUPPORT_VERSION >= 0x00020000
*/
NPROC(void, HFTReplaceEntry, (HFT hft, Selector sel, HFTEntry newEntry, ASFlagBits flags))

/**
	Gets the HFTEntry that was replaced by the specified HFTEntry 
	in the specified entry. Plug-ins should generally not use 
	this method directly, but use the <code>CALL_REPLACED_PROC</code> macro 
	instead. 

	<p>It is necessary to specify both a selector (the index of 
	an entry in the HFT's table of callback pointers) and an 
	HFTEntry (a callback pointer) because a method may be replaced 
	several times, and the various replacement methods are kept 
	in a linked list. The selector determines which linked list 
	is examined, and the HFTEntry determines the entry in the 
	linked list to return. </p>
	@param hft The HFT in which a replaced entry is retrieved. 
	See HFTReplaceEntry() for more information. 
	@param sel The selector whose previous value is obtained. 
	See HFTReplaceEntry() for more information. 
	@param oldEntry The HFTEntry for which the previous value 
	is obtained.
	@return The entry present prior to being replaced.  
	<code>NULL</code> is returned if the entry has not been replaced. 
	@see ASExtensionMgrGetHFT 
	@see CALL_REPLACED_PROC 
	@since PI_ACROSUPPORT_VERSION >= 0x00020000
*/
NPROC(HFTEntry, HFTGetReplacedEntry, (HFT hft, Selector sel, HFTEntry oldEntry))

#if ADD_ASFIXED //PKG
#if ACROBAT_LIBRARY

/**
	Multiplies two fixed numbers. 
	@param a The first number to multiply.
	@param b The second number to multiply.
	@return The product of <code>a</code> and <code>b</code>. 
	@see ASFixedDiv 
	@see Fixed Numbers 
	@see ASFixedRoundToInt16 
	@see ASFixedRoundToInt32 
	@see ASFixedTruncToInt16 
	@see ASFixedTruncToInt32 
	@see ASFixedToFloat 
	@see ASFloatToFixed 
	@see ASInt16ToFixed 
	@see ASInt32ToFixed 
	@since PI_ACROSUPPORT_VERSION >= 0x00020000
*/
NPROC(ASFixed, ASFixedMul, (ASFixed a, ASFixed b))

/**
	Divides two fixed numbers. 
	@param a The dividend. 
	@param b The divisor.
	@return The quotient <code>a / b</code>. 
	@see ASFixedMul 
	@see Fixed Numbers 
	@see ASFixedRoundToInt16 
	@see ASFixedRoundToInt32 
	@see ASFixedTruncToInt16 
	@see ASFixedTruncToInt32 
	@see ASFixedToFloat 
	@see ASFloatToFixed 
	@see ASInt16ToFixed 
	@see ASInt32ToFixed 
	@since PI_ACROSUPPORT_VERSION >= 0x00020000
*/
NPROC(ASFixed, ASFixedDiv, (ASFixed a, ASFixed b))

/**
	Converts a fixed number to a <code>CString</code>. 
	@param f The fixed number to convert. 
	@param s (Filled by the method) The string corresponding 
	to <code>f</code>. 
	@param maxLength The maximum number of characters that 
	<code>s</code> can contain. 
	@param precision The number of digits to retain in the 
	converted number. 
	@note The precision for Mac OS numbers 
	is valid to 9 significant digits.
	@see ASCStringToFixed 
	@see Fixed Numbers 
	@see ASFloatToFixed 
	@see ASInt16ToFixed 
	@see ASInt32ToFixed 
	@since PI_ACROSUPPORT_VERSION >= 0x00020000
*/
NPROC(void, ASFixedToCString, (ASFixed f, char *s, os_size_t maxLength, ASSmallCount precision))

/**
	Converts a <code>CString</code> to a fixed point number. It processes the 
	string from left to right only until the first invalid character 
	is located (for example, <code>a-z, A-Z</code>). 
	@param s A <code>CString</code> to convert.
	@return Fixed number corresponding to <code>s</code>. Returns <code>0</code> if the string 
	does not contain any valid number. 
	@see ASFixedToCString 
	@see ASFixedRoundToInt16 
	@see ASFixedRoundToInt32 
	@see ASFixedTruncToInt16 
	@see ASFixedTruncToInt32 
	@see ASFixedToFloat 
	@since PI_ACROSUPPORT_VERSION >= 0x00020000
*/
NPROC(ASFixed, ASCStringToFixed, (const char *s))

/**
	Multiplies two matrices. 
	@param result (Filled by the method) A pointer to matrix 
	<code>m2 x m1</code>. It is allowed for the result to point to the same location 
	as either <code>m1</code> or <code>m2</code>. 
	@param m1 A pointer to the ASFixedMatrix value for the first 
	matrix to multiply. 
	@param m2 A pointer to the ASFixedMatrix value for the second 
	matrix to multiply.
	@see ASFixedMatrixInvert 
	@see ASFixedMatrixTransform 
	@see ASFixedMatrixTransformRect 
	@see Fixed Numbers 
	@see ASFixedRoundToInt16 
	@see ASFixedRoundToInt32 
	@see ASFixedTruncToInt16 
	@see ASFixedTruncToInt32 
	@see ASFixedToFloat 
	@see ASFloatToFixed 
	@see ASInt16ToFixed 
	@see ASInt32ToFixed 
	@since PI_ACROSUPPORT_VERSION >= 0x00020000
*/
NPROC(void, ASFixedMatrixConcat, (ASFixedMatrixP result, const ASFixedMatrix *m1,const ASFixedMatrix *m2))

/**
	Inverts a matrix. 

	<p>If a matrix is nearly singular (which means that it has a determinant 
	that is nearly zero), inverting and re-inverting the matrix may 
	not yield the original matrix. </p>
	@param result (Filled by the method) A pointer to <code>m-1</code>. It 
	is allowed for the result to point to the same location as <code>m</code>. 
	@param m A pointer to the ASFixedMatrix to invert.
	@see ASFixedMatrixConcat 
	@see ASFixedMatrixTransform 
	@see ASFixedMatrixTransformRect 
	@see ASFixedRoundToInt16 
	@see ASFixedRoundToInt32 
	@see ASFixedTruncToInt16 
	@see ASFixedTruncToInt32 
	@see ASFixedToFloat 
	@since PI_ACROSUPPORT_VERSION >= 0x00020000
*/
NPROC(void, ASFixedMatrixInvert, (ASFixedMatrixP result, const ASFixedMatrixP m))

/**
	Transforms the point <code>p</code> through the matrix <code>m</code>, and puts the result 
	in <code>result</code>. <code>p</code> and <code>result</code> can point to the same location. 
	@param result (Filled by the method) A pointer to the ASFixedPoint 
	containing the result of transforming <code>p</code> through <code>m</code>. It is 
	allowed for the result to point to the same location as <code>m</code>. 
	@param m A pointer to the ASFixedMatrix through which <code>p</code> 
	is transformed. 
	@param p A pointer to the ASFixedPoint representing the 
	point to transform through <code>m</code>.
	@see ASFixedMatrixTransformRect 
	@see ASFixedMatrixConcat 
	@see ASFixedMatrixInvert 
	@see Fixed Numbers 
	@see ASFixedRoundToInt16 
	@see ASFixedRoundToInt32 
	@see ASFixedTruncToInt16 
	@see ASFixedTruncToInt32 
	@see ASFixedToFloat 
	@see ASFloatToFixed 
	@see ASInt16ToFixed 
	@see ASInt32ToFixed 
	@since PI_ACROSUPPORT_VERSION >= 0x00020000
*/
NPROC(void, ASFixedMatrixTransform, (ASFixedPointP result, const ASFixedMatrixP m,const ASFixedPointP p))

/**
	Transforms a rectangle through a matrix. 
	@param result (Filled by the method) A pointer to the ASFixedRect 
	containing the smallest bounding box for the transformed 
	rectangle. It is allowed for the result to point to the same location 
	as <code>m</code>. result will always have <code>bottom < top</code> and <code>left < right</code>. 
	
	@param m A pointer to the ASFixedMatrix containing the matrix 
	through which <code>r</code> is transformed. 
	@param r A pointer to the ASFixedRect containing the 
	rectangle to transform through <code>m</code>.
	@see ASFixedMatrixTransform 
	@see ASFixedMatrixConcat 
	@see ASFixedMatrixInvert 
	@see Fixed Numbers 
	@see ASFixedRoundToInt16 
	@see ASFixedRoundToInt32 
	@see ASFixedTruncToInt16 
	@see ASFixedTruncToInt32 
	@see ASFixedToFloat 
	@see ASFloatToFixed 
	@see ASInt16ToFixed 
	@see ASInt32ToFixed 
	@since PI_ACROSUPPORT_VERSION >= 0x00020000
*/
NPROC(void, ASFixedMatrixTransformRect, (ASFixedRectP result, const ASFixedMatrixP m,const ASFixedRectP r))
#else

/**
	Multiplies two fixed numbers. 
	@param a The first number to multiply.
	@param b The second number to multiply.
	@return The product of <code>a</code> and <code>b</code>. 
	@see ASFixedDiv 
	@see Fixed Numbers 
	@see ASFixedRoundToInt16 
	@see ASFixedRoundToInt32 
	@see ASFixedTruncToInt16 
	@see ASFixedTruncToInt32 
	@see ASFixedToFloat 
	@see ASFloatToFixed 
	@see ASInt16ToFixed 
	@see ASInt32ToFixed 
	@since PI_ACROSUPPORT_VERSION >= 0x00020000
*/
SPROC(ASFixed, ASFixedMul, (ASFixed a, ASFixed b), ASHFTClientASFixedMul)

/**
	Divides two fixed numbers. 
	@param a The dividend. 
	@param b The divisor.
	@return The quotient <code>a / b</code>. 
	@see ASFixedMul 
	@see Fixed Numbers 
	@see ASFixedRoundToInt16 
	@see ASFixedRoundToInt32 
	@see ASFixedTruncToInt16 
	@see ASFixedTruncToInt32 
	@see ASFixedToFloat 
	@see ASFloatToFixed 
	@see ASInt16ToFixed 
	@see ASInt32ToFixed 
	@since PI_ACROSUPPORT_VERSION >= 0x00020000
*/
SPROC(ASFixed, ASFixedDiv, (ASFixed a, ASFixed b), ASHFTClientASFixedDiv)

/**
	Converts a fixed number to a <code>CString</code>. 
	@param f The fixed number to convert. 
	@param s (Filled by the method) The string corresponding 
	to <code>f</code>. 
	@param maxLength The maximum number of characters that 
	<code>s</code> can contain. 
	@param precision The number of digits to retain in the 
	converted number. 
	@note The precision for Mac OS numbers 
	is valid to 9 significant digits.
	@see ASCStringToFixed 
	@see Fixed Numbers 
	@see ASFloatToFixed 
	@see ASInt16ToFixed 
	@see ASInt32ToFixed 
	@since PI_ACROSUPPORT_VERSION >= 0x00020000
*/
SPROC(void, ASFixedToCString, (ASFixed f, char *s, os_size_t maxLength, ASSmallCount precision), ASHFTClientASFixedToCString)

/**
	Converts a <code>CString</code> to a fixed point number. Processes the 
	string from left to right only until the first invalid character 
	is located (for example, <code>a-z, A-Z</code>). 
	@param s A <code>CString</code> to convert.
	@return A fixed number corresponding to <code>s</code>, <code>0</code> if the string 
	does not contain any valid number. 
	@see ASFixedToCString 
	@see ASFixedRoundToInt16 
	@see ASFixedRoundToInt32 
	@see ASFixedTruncToInt16 
	@see ASFixedTruncToInt32 
	@see ASFixedToFloat 
	@since PI_ACROSUPPORT_VERSION >= 0x00020000
*/
SPROC(ASFixed, ASCStringToFixed, (const char *s), ASHFTClientCStringToASFixed)

/**
	Multiplies two matrices. 
	@param result (Filled by the method) A pointer to matrix 
	<code>m2 x m1</code>. It is allowed for the result to point to the same location 
	as either <code>m1</code> or <code>m2</code>. 
	@param m1 A pointer to the ASFixedMatrix value for the first 
	matrix to multiply. 
	@param m2 A pointer to the ASFixedMatrix value for the second 
	matrix to multiply.
	@see ASFixedMatrixInvert 
	@see ASFixedMatrixTransform 
	@see ASFixedMatrixTransformRect 
	@see Fixed Numbers 
	@see ASFixedRoundToInt16 
	@see ASFixedRoundToInt32 
	@see ASFixedTruncToInt16 
	@see ASFixedTruncToInt32 
	@see ASFixedToFloat 
	@see ASFloatToFixed 
	@see ASInt16ToFixed 
	@see ASInt32ToFixed 
	@since PI_ACROSUPPORT_VERSION >= 0x00020000
*/
NPROC(void, ASFixedMatrixConcat, (ASFixedMatrixP result, const ASFixedMatrix *m1,const ASFixedMatrix *m2))

/**
	Inverts a matrix. 

	<p>If a matrix is nearly singular (meaning that it has a determinant of
	nearly zero), inverting and re-inverting the matrix may 
	not yield the original matrix. </p>

	@param result (Filled by the method) A pointer to <code>m-1</code>. It 
	is allowed for the result to point to the same location as <code>m</code>. 
	@param m A pointer to the ASFixedMatrix to invert.
	@see ASFixedMatrixConcat 
	@see ASFixedMatrixTransform 
	@see ASFixedMatrixTransformRect 
	@see ASFixedRoundToInt16 
	@see ASFixedRoundToInt32 
	@see ASFixedTruncToInt16 
	@see ASFixedTruncToInt32 
	@see ASFixedToFloat 
	@since PI_ACROSUPPORT_VERSION >= 0x00020000
*/
NPROC(void, ASFixedMatrixInvert, (ASFixedMatrixP result, const ASFixedMatrixP m))

/**
	Transforms the point <code>p</code> through the matrix <code>m</code>, puts result 
	in result. <code>p</code> and result can point to the same place. 
	@param result (Filled by the method) A pointer to the ASFixedPoint 
	containing the result of transforming <code>p</code> through <code>m</code>. It is 
	allowed for the result to point to the same location as <code>m</code>. 
	@param m A pointer to the ASFixedMatrix through which <code>p</code> 
	is transformed. 
	@param p A pointer to the ASFixedPoint representing the 
	point to transform through <code>m</code>.
	@see ASFixedMatrixTransformRect 
	@see ASFixedMatrixConcat 
	@see ASFixedMatrixInvert 
	@see Fixed Numbers 
	@see ASFixedRoundToInt16 
	@see ASFixedRoundToInt32 
	@see ASFixedTruncToInt16 
	@see ASFixedTruncToInt32 
	@see ASFixedToFloat 
	@see ASFloatToFixed 
	@see ASInt16ToFixed 
	@see ASInt32ToFixed 
	@since PI_ACROSUPPORT_VERSION >= 0x00020000
*/
NPROC(void, ASFixedMatrixTransform, (ASFixedPointP result, const ASFixedMatrixP m,const ASFixedPointP p))

/**
	Transforms a rectangle through a matrix. 
	@param result (Filled by the method) A pointer to the ASFixedRect 
	containing the smallest bounding box for the transformed 
	rectangle. It is allowed for the result to point to the same location 
	as <code>m</code>. The result will always have <code>bottom < top</code> and <code>left < right</code>. 
	
	@param m A pointer to the ASFixedMatrix containing the matrix 
	through which <code>r</code> is transformed. 
	@param rectIn A pointer to the ASFixedRect containing the 
	rectangle to transform through <code>m</code>.
	@see ASFixedMatrixTransform 
	@see ASFixedMatrixConcat 
	@see ASFixedMatrixInvert 
	@see Fixed Numbers 
	@see ASFixedRoundToInt16 
	@see ASFixedRoundToInt32 
	@see ASFixedTruncToInt16 
	@see ASFixedTruncToInt32 
	@see ASFixedToFloat 
	@see ASFloatToFixed 
	@see ASInt16ToFixed 
	@see ASInt32ToFixed 
	@since PI_ACROSUPPORT_VERSION >= 0x00020000
*/
NPROC(void, ASFixedMatrixTransformRect, (ASFixedRectP result, const ASFixedMatrixP m,const ASFixedRectP rectIn))
#endif
#endif //ADD_ASFIXED //PKG

/**
	This method was deprecated in Acrobat 5.0. Use ASFileSysCreatePathName() 
	instead. 

	<p>It converts a platform-specific path name to an ASPathName. 
	It can create an ASPathName from a file path where the file 
	does not already exist. It works for Windows UNC path names 
	as well. It is the caller's responsibility to release the returned 
	ASPathName.</p> 
	@param platformPath A pointer to a platform-specific path name. 
	On Windows and UNIX, it is a <code>NULL</code>-terminated string containing 
	the full path name with the appropriate path separators for 
	each platform. 
	@return The ASPathName corresponding to <code>platformPath</code>. 
	@see ASFileSysReleasePath 
	@since PI_ACROSUPPORT_VERSION >= 0x00020000
*/
NPROC(ASPathName, ASPathFromPlatformPath, (void *platformPath))

/**
	Gets the default standard file system implementation for 
	a platform. 
	@return The platform's default file system. 
	@see ASFileRegisterFileSys 
	@see ASPathFromPlatformPath 
	@since PI_ACROSUPPORT_VERSION >= 0x00020000
	
*/
NPROC(ASFileSys, ASGetDefaultFileSys, (void))

/**
	Converts a file name, specified as an ASPathName, to a device-independent 
	path name. It is the caller's responsibility to free the 
	memory associated with the returned string using ASfree(). 

	@note On Mac OS, if <code>pathName</code> and <code>relativeToThisPath</code> 
	refer to files that are on different volumes, the method returns an absolute path.
	
	@note This method can only be used to get host encoding. 
	For any other encoding, use ASFileSysDIPathFromPathEx() in 
	Acrobat 6.0 and later. 
	@param fileSys (May be <code>NULL</code>) The file system from which 
	<code>pathName</code> was obtained. Pass <code>NULL</code> to use the default file 
	system. 
	@param path The ASPathName to convert. 
	@param relativeToThisPath (May be <code>NULL</code>) The path name relative 
	to which the device-independent path name is specified. If 
	<code>NULL</code>, the device-independent path name will be an absolute, 
	not a relative, path name.
	@return A device-independent path name corresponding to the parameter 
	values supplied, or <code>NULL</code> if the operation is not supported 
	by the file system. 

	<p>See section 3.10 in the <i>PDF Reference</i> for a description 
	of the device-independent path name format. This path name 
	may not be understood on another platform since drive specifiers 
	may be prepended. </p>

	@see ASGetDefaultFileSys 
	@see ASFileSysPathFromDIPath 
	@since PI_ACROSUPPORT_VERSION >= 0x00020000
*/
NPROC(char *, ASFileSysDIPathFromPath, ( ASFileSys fileSys, ASPathName path, ASPathName relativeToThisPath))

/**
	Converts a device-independent path name to an ASPathName. 
	This method can only be used for files that already exist 
	(that is, it cannot be used to create a placeholder path name 
	for files that a plug-in intends to create in the future). 
	
	<p>It is the caller's responsibility to release the returned 
	ASPathName. </p>

	@param fileSys (May be <code>NULL</code>) The file system within which the 
	ASPathName will be created. Pass <code>NULL</code> to use the 
	default file system. 
	@param diPath The device-independent path name to convert. 
	See Section 3.10 in the <i>PDF Reference</i> for a description 
	of the device-independent path name format. This path name 
	may not be understood on another platform since drive specifiers 
	may be prepended. On Windows, you cannot specify a UNC path name. 
	You must have a file mounted on the file server. For example, 
	the following path is valid: <code>/f/dirname/file.pdf</code> where <code>f</code> 
	is <code>\\server\\people</code>. The following does not work: <code>/server/people/dirname/file.pdf</code>.
	
	@param relativeToThisPath The path name relative to which <code>diPath</code> 
	is interpreted. If it is <code>NULL</code>, <code>diPath</code> is interpreted as an absolute 
	path name, not a relative path name.
	@return An ASPathName corresponding to the parameter values supplied,  
	<code>NULL</code> if <code>diPath</code> cannot be converted to an ASPathName 
	or if the specified file does not already exist. 
	@exception genErrNoMemory 
	@see ASFileSysDIPathFromPath 
	@see ASFileSysReleasePath 
	@note In Acrobat 6.0 and later, use ASFileSysPathFromDIPathEx() instead for anything other than host encoding. 
	@since PI_ACROSUPPORT_VERSION >= 0x00020000
*/
NPROC(ASPathName, ASFileSysPathFromDIPath, (ASFileSys fileSys, const char * diPath, ASPathName relativeToThisPath))

/**
	Generates and copies the specified ASPathName (but does 
	not copy the file specified by the path name). The ASPathName 
	must have been obtained through the specified file system. 
	This method may be used regardless of whether the file specified 
	by path name is open. 

	<p>It is the caller's responsibility to release the ASPathName when it is no longer needed
	by using ASFileSysReleasePath().</p> 
	
	@param fileSys (May be <code>NULL</code>) The file system from which 
	<code>pathName</code> was obtained. Pass <code>NULL</code> to use the default file 
	system. 
	@param pathName The ASPathName to copy.
	@return A copy of <code>pathName</code>. 
	@see ASFileSysReleasePath 
	@since PI_ACROSUPPORT_VERSION >= 0x00020000
*/
SPROC(ASPathName, ASFileSysCopyPath, (ASFileSys fileSys, ASPathName pathName), ASFileSysCopyPathName)

/**
	Decrements the internal reference count for the path name and 
	disposes of the path name (but not the file itself) if the 
	reference count is zero. This does not result in any file-level 
	operations, and is unaffected by whether there is an open 
	file for this path name. 
	@param fileSys (May be <code>NULL</code>) The file system from which 
	<code>pathName</code> was obtained. Pass <code>NULL</code> to use the default file 
	system. 
	@param pathName The ASPathName to release.
	@since PI_ACROSUPPORT_VERSION >= 0x00020000
*/
SPROC(void, ASFileSysReleasePath, (ASFileSys fileSys, ASPathName pathName), ASFileSysReleasePathName)

/**
	Attempts to open a file in the specified file system, in 
	the specified read/write/create mode. If the file is already 
	open, the existing file handle is returned. The caller retains 
	ownership of <code>pathName</code>.
	
	<p>This call returns an error if a file over 2 GB in length is opened.
	ASFileSysOpenFile64() should be used instead of this call wherever
	possible, and must be used if files over 2 GB in length may be
	encountered.</p>

	<p>In Mac OS, when this method creates a file, the file's creator 
	is set to <code>'CARO'</code> and its type is set to <code>'PDF '</code> (with a space 
	after PDF).</p> 
	@param fileSys (May be <code>NULL</code>) The file system from which 
	<code>pathName</code> was obtained. Pass <code>NULL</code> to use the default file 
	system. 
	@param pathName The path name of the file to open. 
	@param mode An open-mode value as specified for ASFileMode. 
	
	@param fP (Filled by the method) The ASFile that was opened. 
	@return <code>0</code> if the operation was successful, a non-zero 
	error code otherwise. The error is platform and file-system specific:

	<TABLE rules="all" cellspacing="1">
	<TR><TH>Platform</TH><TH>Error</TH></TR>
	<TR><TD>Windows</TD><TD><ul>
		<li>Returns fileErrWrPerm if trying to open a read-only file with write permissions.</li>
		<li>Returns ErrSysXtnMgr (use GetLastError()) for platform-specific error conditions that CreateFile() may use.</li>
		</ul></TD>
	</TR>
	<TR><TD>Mac OS</TD><TD><ul>
		<li>Returns fileErrFNF if trying to open a file for reading that does not exist.</li>
		<li>Returns ErrSysMDSystem for platform-specific errors that <code>FSpCreate</code>, <code>FSpSetFInfo</code>, <code>FSpOpenRF</code>, <code>FSpOpenDF</code>, or <code>SetFPos</code> may use).</li>
		</ul></TD>
	</TR>
	</TABLE>

	@exception genErrNoError 
	@see ASFileClose 
	@see ASFileReopen 
	@see ASGetDefaultFileSys 
	@since PI_ACROSUPPORT_VERSION >= 0x00020000
*/
NPROC(ASErrorCode, ASFileSysOpenFile, (ASFileSys fileSys, ASPathName pathName, ASFileMode mode, ASFile *fP))

/**
	Attempts to delete the file referred to by <code>pathName</code>. 

	@param fileSys (May be <code>NULL</code>) The file system from which 
	<code>pathName</code> was obtained. Pass <code>NULL</code> to use the default file 
	system. 
	@param pathName The file to delete.
	@return <code>0</code> if the operation was successful, a non-zero 
	platform-dependent error code otherwise. 
	@note If a file is already open for this <code>pathName</code>, the semantics 
	of ASFileSysRemoveFile() are file system-dependent. Make sure 
	you have closed all ASFile objects for <code>pathName</code> before calling 
	ASFileSysRemoveFile(). 
	@since PI_ACROSUPPORT_VERSION >= 0x00020000
*/
SPROC(ASErrorCode, ASFileSysRemoveFile, (ASFileSys fileSys, ASPathName pathName), ASFileSysRemove)


/**
	Attempts to reopen a file using the specified read/write 
	mode. On some platforms, this may result in the file being 
	closed and then reopened, and some error conditions 
	may render the file invalid.
	@param aFile The file to reopen. 
	@param mode An open-mode value as specified for ASFileMode.
	@return <code>0</code> if the operation was successful; some file 
	system or platform-dependent error code is returned otherwise. 
	@see ASFileSysOpenFile 
	@see ASFileClose 
	@note The file mode and return types changed in 0x00060000.
	@since PI_ACROSUPPORT_VERSION >= 0x00020000
*/
NPROC(ASErrorCode, ASFileReopen, ( ASFile aFile, ASFileMode mode ))

/**
	Closes the specified file. After a call to ASFileClose(), 
	the file handle is no longer valid but may be reused as 
	the result of a subsequent call to ASFileSysOpenFile(). 
	@param aFile IN/OUT The file to close. The file must have been 
	opened previously using ASFileSysOpenFile(). 
	@return <code>0</code> if the operation was successful; some file 
	system or platform-dependent error code is returned otherwise. 
	@see ASFileFlush 
	@see ASFileReopen 
	@see ASFileStmRdOpen 
	@see ASFileStmWrOpen 
	@see ASFileSysOpenFile 
	@since PI_ACROSUPPORT_VERSION >= 0x00020000
	
*/
NPROC(ASErrorCode, ASFileClose, ( ASFile aFile ))

/**
	Seeks to the specified position in a file. This is the position 
	at which the next read or write will begin. This call only works
	when the desired file position is less than 2 GB.
	@param aFile IN/OUT The file in which to seek. 
	@param pos IN/OUT The position to seek. 
	@exception fileErrIO
	@see ASFileSetPos64
	@see ASFileGetPos 
	@see ASFileRead 
	@see ASFileWrite 
	@since PI_ACROSUPPORT_VERSION >= 0x00020000
	
*/
NPROC(void, ASFileSetPos, ( ASFile aFile, ASTFilePos pos ))

/**
	Gets the current seek position in a file. This is the position 
	at which the next read or write will begin. This call returns an
	error if the file position is greater than 2 GB.
	@param aFile IN/OUT The file in which to get the seek position. 
	
	@return The current seek position. 
	@exception fileErrIO
	@see ASFileGetPos64
	@see ASFileSetPos 
	@see ASFileRead 
	@see ASFileWrite 
	@since PI_ACROSUPPORT_VERSION >= 0x00020000
	
*/
NPROC(ASTFilePos, ASFileGetPos, ( ASFile aFile ))

/**
	Changes the size of a file. The new size may by larger or 
	smaller than the original size. Since this method does not return any values,
	the status can be assessed by examining the error code in the HANDLER clause.
	This method may raise file system or platform-specific exceptions.
	This call only works when the desired file size is less than 2 GB.
	@param aFile The file whose size is changed. 
	@param newFileSize The new size of file.
	@see ASFileSetEOF64
	@see ASFileCanSetEOF 
	@see ASFileGetEOF 
	@see ASFileGetPos 
	@exception fileErrIO
	@exception asGenErrMethodNotImplemented
	@exception asFileErrGeneral
	@since PI_ACROSUPPORT_VERSION >= 0x00020000
*/
NPROC(void, ASFileSetEOF, ( ASFile aFile, ASTFilePos newFileSize ))

/**
	Gets the current size of a file. It calls ASFileSysGetEofProc(). 
	This call returns an error if the file size is greater than 2 GB.
	@param aFile The ASFile whose size is obtained.
	@return The size of the file. 
	@exception fileErrIO
	@see ASFileGetEOF64
	@see ASFileSetEOF 
	@see ASFileSetPos 
	@since PI_ACROSUPPORT_VERSION >= 0x00020000
*/
NPROC(ASTFilePos, ASFileGetEOF, ( ASFile aFile ))

/**
	Reads data from a file, beginning at the current seek position. 
	
	@param aFile IN/OUT The file from which data is read. 
	@param p IN/OUT (Filled by the method) A buffer into which 
	data is written. The buffer must be able to hold at least 
	<code>count</code> bytes. 
	@param count IN/OUT The number of bytes to read. 
	@return The number of bytes actually read from the file. 
	@exception fileErrIO 
	@exception fileErrUserRequestedStop 
	@exception fileErrBytesNotReady 
	@exception fileErrIOTimeout 
	@exception fileErrGeneral 
	@see ASFileSetPos 
	@see ASFileWrite 
	@since PI_ACROSUPPORT_VERSION >= 0x00020000
	
*/
NPROC(ASTArraySize, ASFileRead, ( ASFile aFile, char *p, ASTArraySize count ))

/**
	Writes data to a file, beginning at the current seek position. 
	
	@param aFile IN/OUT The file to which data is written. 
	@param p IN/OUT A buffer holding the data that is to be written. 
	The buffer must be able to hold at least <code>count</code> bytes. 
	@param count IN/OUT The number of bytes to write. 
	@return The number of bytes actually written to the file. 
	@exception fileErrIO 
	@exception fileErrWrite 
	@see ASFileRead 
	@see ASFileSetPos 
	@since PI_ACROSUPPORT_VERSION >= 0x00020000
	
*/
NPROC(ASTArraySize, ASFileWrite, ( ASFile aFile, const char *p, ASTArraySize count ))

/**
	Flushes any buffered data to a file. This method may raise 
	file system or platform-specific exceptions. 
	@param aFile The file whose data is flushed.
	@exception fileErrIO 
	@see ASFileHardFlush 
	@since PI_ACROSUPPORT_VERSION >= 0x00020000
*/
NPROC(void, ASFileFlush, ( ASFile aFile ))

/**
	Gets the path name for a file and increments an internal reference 
	count. It is the caller's responsibility to release the 
	ASPathName when it is no longer needed by using ASFileSysReleasePath(). 
	@param aFile IN/OUT The file whose path name is acquired. 
	@return The ASPathName associated with <code>asFile</code>. You can use ASFileSysDIPathFromPath() 
	to convert this to a device-independent path name. 
	@see ASFileSysReleasePath 
	@since PI_ACROSUPPORT_VERSION >= 0x00020000
	
*/
NPROC(ASPathName, ASFileAcquirePathName, (ASFile aFile))

/**
	Gets the file system through which a file was opened. 
	@param aFile IN/OUT The open file whose file system is obtained. 
	
	@return The file's ASFileSys. 
	@see ASFileGetFileSysByName 
	@since PI_ACROSUPPORT_VERSION >= 0x00020000
	
*/
NPROC(ASFileSys, ASFileGetFileSys, (ASFile aFile))

/**
	For internal use only.
	@since PI_ACROSUPPORT_VERSION >= 0x00020000
*/
NPROC(void *, ASDebug, (ASInt32 op, void *parm, ASTArraySize parmLen, void *clientData))


/**
	Creates a read-only ASStm from a file. The stream supports seek operations. 
	
	@param afile The open file to associate with the stream. 
	The file must have been opened previously using ASFileSysOpenFile(). 
	Each open file has a unique ASFile. The ASFile value has 
	meaning only to the common ASFile implementation and bears 
	no relationship to platform-specific file objects. 
	@param bufSize The length in bytes of the data buffer. If <code>bufSize</code> 
	is <code>0</code>, the default buffer size (currently 4 K) will be used. 
	The default is generally sufficient. A larger buffer size 
	should be used only when data in the file will be accessed 
	in chunks larger than the default buffer. Although <code>bufSize</code> 
	is passed as an ASUns16, it is treated internally as an 
	ASInt16. As a result, buffer sizes above 32 K are not permitted.
	@return The newly created ASStm. 
	@see ASFileSysOpenFile 
	@see ASFileStmWrOpen 
	@see ASMemStmRdOpen 
	@see ASProcStmRdOpenEx 
	@see ASStmClose 
	@see ASStmRead 
	@see CosNewStream 
	@since PI_ACROSUPPORT_VERSION >= 0x00020000
*/
NPROC(ASStm, ASFileStmRdOpen, (ASFile afile, ASSmallBufferSize bufSize))

/**
	Creates a read-only ASStm from a memory-resident buffer. 
	The stream supports seek operations. 
	@param data A buffer containing the data to read into the 
	stream. This data buffer must not be disposed of until the 
	ASStm is closed. 
	@param len The length in bytes of <code>data</code>.
	@return The newly created ASStm. 
	@see ASStmRead 
	@see ASStmClose 
	@see CosNewStream 
	@see ASMemStmRdOpen 
	@see ASProcStmRdOpenEx 
	@since PI_ACROSUPPORT_VERSION >= 0x00020000
*/
NPROC(ASStm, ASMemStmRdOpen, (const char *data, ASArraySize len))

/**
	Creates a read-only ASStm from an arbitrary data-producing 
	procedure. The stream does not support seek operations. 

	<p><code>readProc</code> is called when the client of the stream attempts 
	to read data from it. </p>
	@param readProc A user-supplied callback that supplies the 
	stream's data. 
	@param clientData A pointer to user-supplied data to pass 
	to <code>readProc</code> each time it is called.
	@return The newly created ASStm. 
	@exception genErrNoMemory 
	@see ASStmRead 
	@see ASStmClose 
	@see CosNewStream 
	@see ASFileStmRdOpen 
	@see ASMemStmRdOpen 
	@since PI_ACROSUPPORT_VERSION >= 0x00020000
*/
NPROC(ASStm, ASProcStmRdOpen, (ASStmProc readProc, void *clientData))


/**
	Reads data from <code>stm</code> into memory. 
	@param ptr (Filled by the method) A buffer into which 
	data is written. 
	@param itemSize The number of bytes in a stream item. 
	See the description of <code>nItems</code> for further information. 
	@param nItems The number of items to read. The amount of data 
	read into the memory buffer will be <code>itemSize * nItems</code>, unless 
	an EOF is encountered first. The relative values of <code>itemSize</code> 
	and <code>nItems</code> really do not matter; the only thing that matters 
	is their product. It is often convenient to set <code>itemSize</code> 
	to <code>1</code>, so that <code>nItems</code> is the number of bytes to read. 
	@param stm The stream from which data is read.
	@return The number of items (not bytes) read. 
	@see ASStmWrite 
	@since PI_ACROSUPPORT_VERSION >= 0x00020000
*/
NPROC(ASTCount, ASStmRead, (char *ptr, ASTArraySize itemSize, ASTCount nItems, ASStm stm))

/**
	Writes data from a memory buffer into an ASStm. 

	<p>You cannot use this method to change a PDF page content 
	stream. It can only be used for a print stream. </p>.

	<p>Historically, this method was provided to allow plug-ins 
	to write data into the print stream when printing to a PostScript 
	printer (see the PDDocWillPrintPage() notification). However, 
	ASStm is a general purpose I/O mechanism in Acrobat even though 
	only limited open and read/write methods are provided in 
	the plug-in API. For instance, not all ASStm objects support 
	seek operations. </p>
	@param ptr A buffer from which data is read. 
	@param itemSize The number of bytes in a stream item. 
	See the description of <code>nItems</code> for additional information. 
	
	@param nItems The number of items to write. The amount of 
	data written into the stream will be <code>itemSize * nItems</code>. 
	The relative values of <code>itemSize</code> 
	and <code>nItems</code> really do not matter; the only thing that matters 
	is their product. It is often convenient to set <code>itemSize</code> 
	to <code>1</code>, so that <code>nItems</code> is the number of bytes to read. 
	@param stm The stream into which data is written.
	@return The number of items (not bytes) written. 
	@see ASStmRead 
	@since PI_ACROSUPPORT_VERSION >= 0x00020000
*/
NPROC(ASTCount, ASStmWrite, (const char *ptr, ASTArraySize itemSize, ASTCount nItems, ASStm stm))

/**
	Closes the specified stream. 
	@param stm The stream to close.
	@see ASFileStmRdOpen 
	@see ASFileStmWrOpen 
	@see ASMemStmRdOpen 
	@see ASProcStmRdOpenEx 
	@since PI_ACROSUPPORT_VERSION >= 0x00020000
*/
NPROC(void, ASStmClose, (ASStm stm))

/* Acrobat 2.2 additions */

/**
	Allows a <code>fileSys</code> to be unregistered. In general, a <code>fileSys</code> 
	is only unregistered by the extension that registered it. 
	
	@param extension IN/OUT The gExtensionID of the plug-in un-registering 
	<code>fileSys</code>. 
	@param fileSys IN/OUT The ASFileSys to un-register. 
	@return <code>true</code> if <code>fileSys</code> successfully unregistered, <code>false</code> if  
	there are any open files that were opened through <code>fileSys</code>. 
	
	@see ASFileRegisterFileSys 
	@since PI_ACROSUPPORT_VERSION >= 0x00020002
	
*/
NPROC(ASBool, ASFileUnregisterFileSys, (ASExtension extension, ASFileSys fileSys))

/**
	Sends data from a file system implementation to an ASFile. 
	The data may be for a multi-read request call, or may be 
	unsolicited. 

	<p>This method can only be called from within a file system implementation. 
	It must not be called by clients of the ASFile, such as a caller that 
	acquired the file with ASFileSysOpenFile(). </p>
	
	@param aFile IN/OUT The file to which data is sent. 
	@param p IN/OUT The data being pushed. 
	@param offset IN/OUT A byte offset into the file at which the data should 
	be written. 
	@param length IN/OUT The number of bytes held in the buffer. 
	@exception fileErrGeneral 
	@see ASFileRead 
	@see ASFileWrite 
	@since PI_ACROSUPPORT_VERSION >= 0x00020002
	
*/
NPROC(void, ASFilePushData, (ASFile aFile, const char *p, ASTFilePos offset, ASTArraySize length))

/**
	Allows an implementor to provide a file system for use by 
	external clients. An external client can locate the file 
	system using ASFileGetFileSysByName(). <code>fileSys</code> provides its 
	name via the ASFileSysGetFileSysNameProc() callback. This 
	method returns <code>false</code> if a file system with the same name 
	is already registered. 
	@param extension IN/OUT The gExtensionID of the plug-in registering 
	the <code>fileSys</code>. 
	@param fileSys IN/OUT The ASFileSys being registered. 
	@return <code>true</code> if <code>fileSys</code> is successfully registered, <code>false</code> otherwise. 
	
	@see ASFileUnregisterFileSys 
	@see ASFileGetFileSysByName 
	@since PI_ACROSUPPORT_VERSION >= 0x00020002
	
*/
NPROC(ASBool, ASFileRegisterFileSys, (ASExtension extension, ASFileSys fileSys))

/**
	Gets the file system that was registered with the specified 
	name. 
	@param name IN/OUT The ASAtom corresponding to the name of the 
	file system to obtain. It may be one of the following:

	<TABLE rules="all" cellspacing="1">
	<TR><TH>String</TH><TH>Description</TH></TR>
	<TR><TD><code>"Mac_K"</code></TD><TD>Mac OS file system</TD></TR>
	<TR><TD><code>"DOS_K"</code></TD><TD>Classic Windows file system (it only supports host-encoded paths)</TD></TR>
	<TR><TD><code>"Win_K"</code></TD><TD>Unicode Windows file system</TD></TR>
	<TR><TD><code>"Unix_K"</code></TD><TD>UNIX file system</TD></TR>
	<TR><TD><code>"CHTTP"</code></TD><TD>HTTP file system</TD></TR>
	<TR><TD><code>"CDocumentum"</code></TD><TD>Documentum file system</TD></TR>
	<TR><TD><code>"CODMA"</code></TD><TD>Open Document Management file system</TD></TR>
	</TABLE>

	@return The file system, otherwise <code>NULL</code> if no matching file system 
	was found. 
	@see ASFileGetFileSys 
	@since PI_ACROSUPPORT_VERSION >= 0x00020002
	
*/
NPROC(ASFileSys, ASFileGetFileSysByName, (ASAtom name))

/**
	Gets the ASFile associated with the specified ASMDFile and 
	ASFileSys. 
	@param mdFile IN/OUT The ASMDFile for which the information is 
	desired. 
	@param fileSys IN/OUT The ASFileSys through which <code>fileID</code> was opened. 
	
	@param pfN IN/OUT (Filled by the method, may be <code>NULL</code>) The ASFile 
	representing <code>fileID</code> within <code>fileSys</code>. 
	@return <code>true</code> if <code>fileID</code> is determined to be a valid file opened through 
	<code>fileSys</code>, <code>false</code> otherwise. 
	@see ASFileGetFileSys 
	@see ASFileGetMDFile 
	@since PI_ACROSUPPORT_VERSION >= 0x00020002
	
*/
NPROC(ASBool, ASFileFromMDFile, (ASMDFile mdFile, ASFileSys fileSys, ASFile *pfN))

/**
	Given an ASFile, returns the <code>fileSys</code> and the ASMDFile identification 
	in that <code>fileSys</code>. This call is needed for a file system in 
	a plug-in to be able to call the inner routines in another 
	file system. 
	@param fN IN/OUT The ASFile for which the information is desired. 
	
	@param pFileID IN/OUT (Filled by the method, may be <code>NULL</code>) The 
	ASMDFile identifier associated with file. 
	@param pFileSys IN/OUT (Filled by the method, may be <code>NULL</code>) The 
	file system through which this file was opened. 
	@return <code>true</code> if the file is an open file, <code>false</code> otherwise. 
	@see ASFileFromMDFile 
	@see ASFileGetFileSys 
	@since PI_ACROSUPPORT_VERSION >= 0x00020002
	
*/
NPROC(ASBool, ASFileGetMDFile, (ASFile fN, ASMDFile *pFileID, ASFileSys *pFileSys))


/**
	Creates an ASPathName based on the input type and <code>pathSpec</code>. 
	Each <code>fileSys</code> implementation must publish the input types 
	that it accepts. 

	<p>It is the caller's responsibility to release the ASPathName 
	when it is no longer needed by using ASFileSysReleasePath().</p> 
	
	<p>Developers should consider using the simpler helper macros 
	instead of using the call directly. </p>

	@note This method does not work for relative POSIX paths on Mac OS; only absolute POSIX paths will work.

	@param fileSys (May be <code>NULL</code>) The ASFileSys in which you 
	are trying to create an ASPathName. Pass <code>NULL</code> to use the 
	default file system. 
	@param pathSpecType An ASAtom specifying the data type 
	in <code>pathSpec</code>, as follows:

	<TABLE rules="all" cellspacing="1">
	<TR><TH>Data type</TH><TH>Description</TH></TR>
	<TR>
		<TD><code>"Cstring"</code></TD>
		<TD>Accepted by the 
			default file system on all platforms. <code>pathSpec</code> is a <code>NULL</code>-
			terminated <code>char*</code>. On Mac OS it must be an absolute path 
			separated by colons, as in <code>"VolumeName:Folder:file.pdf"</code>. 
			On Windows the path may be absolute, as in <code>"C:\\folder\\file.pdf"</code>
			or relative as in <code>"...\\folder\\file.pdf"</code>. On UNIX the 
			path may be absolute as in <code>"/folder/file.pdf"</code> or relative 
			as in <code>".../folder/file.pdf"</code>.
		</TD>
	</TR>
	<TR>
		<TD><code>"FSSpec"</code></TD>
		<TD>Accepted by the default file system on Mac OS. <code>pathSpec</code> is a pointer to a valid FSSpec.
			This type is deprecated in Acrobat 9.0.  Use FSRef, FSRefWithCFStringRef, CFURLRef, or POSIXPath instead.
		</TD>
	</TR>
	<TR>
		<TD><code>"FSRef"</code></TD>
		<TD>
			Accepted by the default file system on Mac OS.
			<code>pathSpec</code> is a valid FSRef.
		</TD>
	</TR>
	<TR>
		<TD><code>"FSRefWithCFStringRef"</code></TD>
		<TD>
			Accepted by the default file system on Mac OS. <code>pathSpec</code> is a pointer 
			to a valid FSRefWithCFStringRefRec.
		</TD>
	</TR>
	<TR>
		<TD><code>"CFURLRef"</code></TD>
		<TD>Accepted by the default file system on Mac OS. <code>pathSpec</code> is a valid CFURLRef.</TD>
	</TR>
	<TR>
		<TD><code>"POSIXPath"</code></TD>
		<TD>
			Accepted by the default file system on Mac OS. <code>pathSpec</code> is a 
			<code>NULL</code>-terminated <code>char*</code> containing a POSIX-style, UTF-8 encoded path string.
		</TD>
	</TR>
	<TR>
		<TD><code>"SFReply"</code></TD>
		<TD>
			In the past this was accepted by the default file system on Mac OS. 
			This type is deprecated and should not be used.
		</TD>
	</TR>
	<TR>
		<TD><code>"DIPath"</code></TD>
		<TD>
			Accepted by the default file system on Windows and Mac OS. <code>pathSpec</code> is a device-independent 
			path as documented in the <i>PDF Reference</i>. <code>pathSpec</code> can contain 
			an absolute or relative path. If a relative path is used, 
			the method will evaluate that path against an ASPathName 
			passed in the <code>mustBeZero</code> parameter.
		</TD>
	</TR>
	<TR>
		<TD><code>"DIPathWithASText"</code></TD>
		<TD>Accepted by the default file system on Windows and Mac OS. <code>pathSpec</code> is a device-independent 
			path, in the form of an ASText, as documented in the <i>PDF Reference</i>. <code>pathSpec</code> can contain 
			an absolute or relative path. If a relative path is used, 
			the method will evaluate that path against an ASPathName 
			passed in the <code>mustBeZero</code> parameter.
		</TD>
	</TR>
	<TR>
		<TD><code>"FolderPathName"</code></TD>
		<TD>
			Accepted by the default file system on Windows and Mac OS. 
			<code>pathSpec</code> is an ASPathName that contains the path of a folder. 
			<code>mustBeZero</code> is a C string containing the name of the file. 
			The returned ASPathName contains the result of appending 
			<code>mustBeZero</code> to <code>pathSpec</code>.
		</TD>
	</TR>
	<TR>
		<TD><code>"FolderPathNameWithASText"</code></TD>
		<TD>
			Accepted by the default file system on Windows and Mac OS. 
			<code>pathSpec</code> is an ASPathName that contains the path of a folder. 
			<code>mustBeZero</code> is an ASText containing the name of the file. 
			The returned ASPathName contains the result of appending 
			<code>mustBeZero</code> to <code>pathSpec</code>.
		</TD>
	</TR>
	</TABLE>

	@param pathSpec The file specification from which to create 
	an ASPathName. Relative paths are evaluated from the directory 
	containing the executable (if used with the PDF Library), 
	or the directory containing Acrobat (if used in a plug-in). 
	
	@param additionalData See <code>pathSpecType</code> parameter description.
	@return The newly created path name, or <code>NULL</code> on failure. 
	@exception genErrBadParm on Windows if the <code>pathSpecType</code> is not recognized. 
	
	@exception genErrMethodNotImplemented 
	@see ASFileSysCopyPath 
	@see ASFileSysCreatePathFromCString 
	@see ASFileSysCreatePathFromDIPath 
	@see ASFileSysCreatePathFromFSSpec 
	@see ASFileSysCreatePathWithFolderName 
	@since PI_ACROSUPPORT_VERSION >= 0x00020002
*/
NPROC(ASPathName, ASFileSysCreatePathName, (const ASFileSys fileSys, ASAtom pathSpecType, const void *pathSpec,
							const void *additionalData))

/**
	Converts an ASPathName from one file system to another. 
	It returns an ASPathName acquired through <code>newFileSys</code> that refers 
	to an image (which may possibly be cached) of the file in <code>oldfileSys</code>. 
	Use this call to get a local file that is an image of a 
	remote file (in a URL file system, for example). This is 
	needed by programs such as the QuickTime Movie Player, because they 
	can only work from local file-system files. The returned 
	ASPathName may be a reference to a cache, so the file should 
	be treated as read-only. 

	<p>Because of the possibility of cache flushing, you must hold 
	a copy of the remote file's ASPathName for the duration 
	of use of the local file. </p>

	<p>Do not remove the local file copy, since the <code>newFileSys</code> 
	system does not know about the linkage to the remote (<code>oldFileSys</code>) 
	file! </p>

	<p>The source file does not have to be open. </p>

	<p>This call is handled by <code>oldFileSys</code> if <code>oldFileSys</code> contains 
	the appropriate procedure. Otherwise it is handled by copying 
	the file. The source file is closed at the end of the copy 
	if it was not open prior to the call. </p>

	<p>It is the caller's responsibility to release the ASPathName 
	when it is no longer needed by using ASFileSysReleasePath().</p> 
	
	@param oldFileSys IN/OUT (May be <code>NULL</code>) The file system from which 
	<code>oldPathName</code> was obtained. Pass <code>NULL</code> to use the default file 
	system. 
	@param oldPathName IN/OUT The ASPathname in the current file system. 
	
	@param newFileSys IN/OUT (May be <code>NULL</code>) The file system to which 
	the <code>oldPathName</code> is converted. Pass <code>NULL</code> to use the default 
	file system. 
	@return The ASPathName in <code>newFileSys</code> or <code>NULL</code> if one cannot be made. 
	
	@exception ERR_NOMEMORY 
	@exception fileErrIO 
	@exception fileErrUserRequestedStop 
	@exception fileErrBytesNotReady 
	@exception fileErrIOTimeout 
	@exception fileErrGeneral 
	@exception fileErrWrite 
	@see ASFileSysCreatePathName 
	@since PI_ACROSUPPORT_VERSION >= 0x00020002
	
*/
NPROC(ASPathName, ASFileSysAcquireFileSysPath, (ASFileSys oldFileSys, ASPathName oldPathName, ASFileSys newFileSys))

/**
	Gets or sets the mode flags for a file. Pass <code>0</code> for <code>modeValue</code> 
	and <code>modeMask</code> to simply get the current mode flags. 
	@param fN The file for which to get or set the mode. 
	
	@param modeValue The mode flag values to get or set, which 
	are described in ASFileMode Flags. 
	@param modeMask The mask for the mode flags to get or set.
	@return The previous value of the mode, or <code>0</code> if the file system 
	does not support this operation. 

	@see ASFileRead 
	@see ASFileSysOpenFile 
	@note This operation is primarily intended for slow file 
	systems such as the Internet, where there can potentially 
	be an appreciable wait between requesting and retrieving 
	bytes. 
	@since PI_ACROSUPPORT_VERSION >= 0x00020002
*/
NPROC(ASFlagBits, ASFileSetMode, (ASFile fN, ASFlagBits modeValue, ASFlagBits modeMask))

/* Acrobat 4.0 additions */

/**
	Creates an ASStm from an arbitrary data-producing procedure. 
	The stream does not support seek operations. 
	@param writeProc A user-supplied callback that provides 
	the data for the stream. 
	@param destroyProc A user-supplied callback that destroys 
	the specified ASStm. (Generally, this means deallocating 
	the memory associated with the ASStm.) 
	@param clientData A pointer to user-supplied data to pass 
	to <code>writeProc</code> each time it is called.
	@return The newly created ASStm. 
	@exception genErrNoMemory 
	@see ASFileStmRdOpen 
	@see ASFileStmWrOpen 
	@see ASMemStmRdOpen 
	@see ASProcStmRdOpenEx 
	@see ASStmWrite 
	@see ASStmRead 
	@see ASStmClose 
	@see CosNewStream 
	@since PI_ACROSUPPORT_VERSION >= 0x00040000
*/
NPROC(ASStm, ASProcStmWrOpen, (ASStmProc writeProc, ASProcStmDestroyProc destroyProc, void *clientData))

/**
	Creates a writable ASStm from a file. The stream supports seek operations. 
	
	@param afile The open file to associate with the stream. 
	The file must have been opened previously using ASFileSysOpenFile(). 
	Each open file has a unique ASFile. The ASFile value has 
	meaning only to the common ASFile implementation and bears 
	no relationship to platform-specific file objects. 
	@param bufSize The length in bytes of a data buffer. If <code>bufSize</code> 
	is <code>0</code>, the default buffer size (currently 4kB) is used. The 
	default is generally sufficient. A larger buffer size should 
	be used only when data in the file will be accessed in chunks 
	larger than the default buffer. Although <code>bufSize</code> is passed 
	as an ASUns16, it is treated internally as an ASInt16. As 
	a result, buffer sizes above 32 K are not permitted.
	@return The newly created ASStm. 
	@exception genErrNoMemory 
	@see ASProcStmWrOpen 
	@see ASFileStmRdOpen 
	@see ASMemStmRdOpen 
	@see ASProcStmRdOpenEx 
	@see ASStmWrite 
	@see ASStmRead 
	@see ASStmClose 
	@see ASFileSysOpenFile 
	@see CosNewStream 
	@since PI_ACROSUPPORT_VERSION >= 0x00040000
*/
NPROC(ASStm, ASFileStmWrOpen, (ASFile afile, ASSmallBufferSize bufSize))

/**
	Tests whether an HFT is valid. 
	@param hft IN/OUT The HFT to test. 
	@return <code>true</code> if <code>hft</code> is valid, <code>false</code> otherwise. 
	@since PI_ACROSUPPORT_VERSION >= 0x00040000
	
*/
NPROC(ASBool, HFTIsValid, (HFT hft))


/* Acrobat 5.0 additions */

/**
	Populates an ASFileSysItemProps record with a full description 
	of the file system object associated with <code>pathName</code>. It calls 
	ASFileSysGetItemPropsProc(). 
	@param fileSys (May be <code>NULL</code>) The file system from which 
	<code>pathName</code> was obtained. Pass <code>NULL</code> to use the default file 
	system. 
	@param pathName The ASPathName associated with the object. 
	
	@param props (Filled by the method) A properties structure 
	describing the object. The size field must be set on input.
	@return <code>0</code> if no error was encountered; otherwise an error code is 
	returned. If an error code is returned, <code>props</code> will not be 
	filled with valid values. If no file system object is present, 
	an error will not be reported and the <code>props.isThere</code> 
	field will be <code>false</code>. 
	@exception genErrBadParm 
	@exception genErrMethodNotImplemented 
	@see ASFileSysConvertCabToItemProps 
	@see ASFileSysConvertItemPropsToCab 
	@see ASFileSysGetItemPropsAsCab 

	@note The method clears the memory associated with <code>itemProps</code>, 
	so the caller need not do so. However, the caller must explicitly 
	set the <code>props->size</code> field of the ASFileSysItemProps structure 
	before calling this method. 

	@since PI_ACROSUPPORT_VERSION >= 0x00050000
*/
NPROC(ASErrorCode,		ASFileSysGetItemProps,			(ASFileSys fileSys, ASPathName pathName, ASFileSysItemProps props))

/**
	Creates an iterator which can be used to enumerate all objects 
	inside the specified folder, and returns the properties 
	of the first item found in the folder. The iteration can 
	be continued by passing the returned ASFolderIterator to 
	ASFileSysNextFolderItem. 

	<p>Both <code>itemProps</code> and <code>itemPath</code> are optional, and may be <code>NULL</code> 
	if you are not interested in that information. </p>

	<p>The client is obligated to eventually free the resources 
	associated with ASFolderIterator by calling ASFileSysDestroyFolderIterator(). </p>

	@param fileSys IN/OUT (May be <code>NULL</code>) The file system from which 
	<code>folderPath</code> was obtained. Pass <code>NULL</code> to use the default file 
	system. 
	@param folderPath IN/OUT The path associated with the target folder. 
	
	@param props IN/OUT (Filled by the method, may be <code>NULL</code>) A properties 
	structure describing the first object iterated. 
	@param itemPath (Filled by the method, may be <code>NULL</code>) An ASPathName, allocated by
	ASFileSysFirstFolderItem(), which is associated with the object.
	The caller of ASFileSysFirstFolderItem() must free the ASPathName.
	This parameter contains an absolute path on Windows and UNIX.
	@return A valid ASFolderIterator object if <code>folderPath</code> contained 
	any files. <code>NULL</code> will be returned if the folder is empty 
	or the operation is not supported by the file system. 
	@exception genErrBadParm 
	@exception fileErrFNF (raised by the Windows default file system)
	@exception asFileErrNotADir (raised by the Windows default file system)
	@exception ERR_NOMEMORY 
	@see ASFileSysNextFolderItem 
	@see ASFileSysDestroyFolderIterator 
	
	@note The order in which items are enumerated is implementation-dependent. 
	Of particular importance is the fact that items will probably not be iterated in alphabetic order. 

	@note If items are added to or removed from a folder during 
	iteration, the results are implementation-dependent. 
	@since PI_ACROSUPPORT_VERSION >= 0x00050000
	
*/
NPROC(ASFolderIterator, ASFileSysFirstFolderItem,		(ASFileSys fileSys, ASPathName folderPath, ASFileSysItemProps props, ASPathName *itemPath))

/**
	Continues the iteration process associated with the ASFolderIterator 
	object. 

	<p>Both <code>itemPath</code> and <code>itemProps</code> are optional, and may be <code>NULL</code> 
	if you are not interested in that information. </p>

	@param fileSys (May be <code>NULL</code>) The file system with which 
	the iteration was started. Pass <code>NULL</code> to use the default 
	file system. 
	@param folderIter An ASFolderIterator object returned 
	from a previous call to ASFileSysFirstFolderItem(). 
	@param props (Filled by the method, may be <code>NULL</code>) A properties 
	structure describing the next object in the iteration. 
	@param itemPath (Filled by the method, may be <code>NULL</code>) An ASPathName, allocated by
	ASFileSysNextFolderItem(), which is associated with the object.
	The caller of ASFileSysNextFolderItem() must free the ASPathName.
	This parameter contains an absolute path on Windows and UNIX.
	@return <code>true</code> if another object was found, <code>false</code> otherwise. 
	@exception genErrBadParm 
	@exception fileErrGeneral 
	@exception ERR_NOMEMORY 
	@see ASFileSysFirstFolderItem 
	@see ASFileSysDestroyFolderIterator 
	@since PI_ACROSUPPORT_VERSION >= 0x00050000
*/
NPROC(ASBool,			ASFileSysNextFolderItem,		(ASFileSys fileSys, ASFolderIterator folderIter, ASFileSysItemProps props, ASPathName *itemPath))

/**
	Releases the resources associated with <code>folderIter</code>. 
	@param fileSys IN/OUT (May be <code>NULL</code>) The file system from which 
	the iteration was started. Pass <code>NULL</code> to use the default 
	file system. 
	@param folderIter IN/OUT An ASFolderIterator object returned from 
	a previous call to ASFileSysFirstFolderItem(). 
	@see ASFileSysFirstFolderItem 
	@see ASFileSysNextFolderItem 
	@since PI_ACROSUPPORT_VERSION >= 0x00050000
	
*/
NPROC(void,				ASFileSysDestroyFolderIterator, (ASFileSys fileSys, ASFolderIterator folderIter))

/**
	Returns the parent folder of the file system object associated 
	with <code>pathName</code>. The following rules apply in the default 
	file systems: 
	
	<ul>
		<li><code>pathName</code> may be associated with either a file or a folder.</li>
		<li>The file system object associated with <code>pathName</code> need not exist.</li>
	</ul>
	

	<p>It is the caller's responsibility to release the returned ASPathName. </p>

	@param fileSys IN/OUT (May be <code>NULL</code>) The file system from which 
	<code>pathName</code> was obtained. Pass <code>NULL</code> to use the default file 
	system. 
	@param pathName IN/OUT The ASPathName. 
	@return The ASPathName associated with the parent folder. The method will return 
	<code>NULL</code> if the parent could not be returned. 
	@exception genErrNoMemory 
	@exception fileErrIO 
	@exception fileErrUserRequestedStop 
	@exception fileErrBytesNotReady 
	@exception fileErrIOTimeout 
	@exception fileErrGeneral 
	@exception fileErrWrite 
	@see ASFileSysCreatePathName 
	@since PI_ACROSUPPORT_VERSION >= 0x00050000
	
*/
NPROC(ASPathName,		ASFileSysAcquireParent,			(ASFileSys fileSys, ASPathName pathName))


/**
	Performs a comparison between the file and path to determine 
	if they represent the same file. This method will return 
	<code>false</code> if the file was not opened through the <code>fileSys</code> file system. 
	
	@param fN IN/OUT The file in question. 
	@param pathName IN/OUT The ASPathName in question. 
	@param fileSys IN/OUT The file system from which the path was obtained. 
	
	@return <code>false</code> if the comparison fails, <code>true</code> otherwise. 
	@note This method is not guaranteed to work on all file systems. 
	@since PI_ACROSUPPORT_VERSION >= 0x00050000
	
*/
NPROC(ASBool,			ASFileIsSame,					(ASFile fN, ASPathName pathName, ASFileSys fileSys))

/**
	Extracts the file name (including extension) from the path. 
	
	@param fileSys (May be <code>NULL</code>) The file system from which 
	<code>pathName</code> was obtained. Pass <code>NULL</code> to use the default file 
	system. 
	@param pathName The ASPathName associated with the file in question. 
	@param name (Filled by the method) A buffer used to store the file name. 
	@param maxLength Maximum number of bytes that buffer can hold.
	@return <code>0</code> if the operation was successful, a non-zero 
	platform-dependent error code otherwise. The buffer is returned as 
	a host-encoded C string. 
	@exception fileErrGeneral 
	@see ASFileSysGetNameFromPathAsASText 

	@note In Acrobat 6.0 and later, this method can only be used to get 
	host encoding. For any other encoding, use ASFileSysGetNameFromPathAsASText(). 
	@since PI_ACROSUPPORT_VERSION >= 0x00050000
*/
NPROC(ASErrorCode,		ASFileSysGetNameFromPath,		(ASFileSys fileSys, ASPathName pathName, char *name, ASTArraySize maxLength))

/**
	Returns a unique path name suitable for use in creating 
	temporary files. It is the caller's responsibility to release 
	the returned object using ASFileSysReleasePath(). 

	<p>If <code>siblingPath</code> is non-<code>NULL</code>, the returned ASPathName is created 
	at the same folder level as this path. Otherwise the standard 
	temporary file location is used. </p>

	@param fileSys (May be <code>NULL</code>) The file system from which 
	<code>siblingPath</code> was obtained. Pass <code>NULL</code> to use the default file 
	system. 
	@param siblingPathName (May be <code>NULL</code>) An ASPathName indicating 
	the desired location of the temporary path name. The returned 
	ASPathName is created at the same folder level as this path.
	@return The ASPathName if the operation was successful, <code>NULL</code> otherwise. 
	
	@see ASFileSysReleasePath 
	@since PI_ACROSUPPORT_VERSION >= 0x00050000
*/
NPROC(ASPathName,		ASFileSysGetTempPathName,		(ASFileSys fileSys, ASPathName siblingPathName))

/**
	Gets the amount of free space on the volume containing <code>pathName</code>. 
	
	@param fileSys IN/OUT (May be <code>NULL</code>) The file system from which 
	<code>pathName</code> was obtained. Pass <code>NULL</code> to use the default file 
	system. 
	@param pathName IN/OUT The ASPathName in question. 
	@return The amount of free space in bytes, <code>0</code> otherwise. Because 
	the free space is returned as an ASUns32, it is limited to 4 GB. 
	@since PI_ACROSUPPORT_VERSION >= 0x00050000
	
*/
NPROC(ASDiskSpace,		ASFileSysGetStorageFreeSpace,	(ASFileSys fileSys, ASPathName pathName))

/**
	Flushes the volume on which the specified file resides. 
	This ensures that any data written to the system for the 
	volume containing <code>pathName</code> is flushed out to the physical 
	volume (equivalent to Mac OS FlushVol or to the UNIX 
	sync). 

	<p>Only the Mac OS default file system implements the callback 
	associated with this method. This is a no-op on Windows 
	and UNIX.</p> 

	@param fileSys IN/OUT (May be <code>NULL</code>) The file system from which 
	<code>pathName</code> was obtained. Pass <code>NULL</code> to use the default file 
	system. 
	@param pathName IN/OUT The ASPathName from which the volume information is obtained. 
	@return <code>0</code> if the operation was successful, a non-zero 
	platform-dependent error code otherwise. 
	@since PI_ACROSUPPORT_VERSION >= 0x00050000
	
*/
NPROC(ASErrorCode,		ASFileSysFlushVolume,			(ASFileSys fileSys, ASPathName pathName))

  
/**
	Initiates a byte range request for a given file, if the file is in the browser.
	@param fN The file for which you wish to make read requests.
	@param blockPairs The array of <code>ASInt32</code> pairs.  The first <code>ASInt32</code> in the pair 
	is the offset into the file to read, and the second <code>ASInt32</code> is the length of
	the range to request.
	@param nBlockPairs The number of block pairs to request.
	@see ASFileHasOutstandingMReads
	@since PI_ACROSUPPORT_VERSION >= 0x00050000
*/
NPROC(void,				ASFileMReadRequest,				(ASFile fN, ASInt32 *blockPairs, ASTCount nBlockPairs ))

/**
	Clears all outstanding <code>mreads</code> for the given file.
	@param fN The file to clear <code>mreads</code> for. 
	@see ASFileHasOutstandingMReads
	@since PI_ACROSUPPORT_VERSION >= 0x00060000
*/
NPROC(void,				ASFileClearOutstandingMReads,	(ASFile fN))

/**
	Returns the URL corresponding to <code>pathName</code>. It is the caller's 
	responsibility to free the memory associated with the returned 
	string using ASfree(). 
	@param fileSys IN/OUT The file system from which <code>path</code> was obtained. 
	Pass <code>NULL</code> to use the default file system. 
	@param path IN/OUT The ASPathName in question. 
	@return A buffer containing the URL, or <code>NULL</code> if some error occurred. 
	The URL is in the standard <code>'file://'</code> URL style. 
	@since PI_ACROSUPPORT_VERSION >= 0x00050000
	
*/
NPROC(char *,			ASFileSysURLFromPath,			(ASFileSys fileSys, ASPathName path))

/**
	Returns the URL associated with file. It is the caller's 
	responsibility to release the memory associated with the 
	returned string using ASfree(). 
	@param asf IN/OUT The file in question. 
	@return A buffer containing the URL, or <code>NULL</code> if it could not be determined. 
	On all file systems, the path will be URL-escaped.  On the default file system, encoding will be 
	platform-encoded.  On Unicode and browser file systems, encoding will be UTF8.
	
	@since PI_ACROSUPPORT_VERSION >= 0x00050000
	
*/
NPROC(char *,			ASFileGetURL,					(ASFile asf))

/**
	Creates an empty folder at the specified <code>pathName</code>. 
	@param fileSys (May be <code>NULL</code>) The file system from which 
	<code>pathName</code> was obtained. Pass <code>NULL</code> to use the default file 
	system. 
	@param path The path of the folder to create. 
	@param recurse Recursively create the parent folder if necessary. 
	@return <code>0</code> if the operation was successful, a non-zero 
	platform-dependent error code otherwise. 
	@exception genErrMethodNotImplemented 
	@exception fileErrFNF 
	@see ASFileSysRemoveFolder 
	@since PI_ACROSUPPORT_VERSION >= 0x00050000
*/
NPROC(ASErrorCode,		ASFileSysCreateFolder,			(ASFileSys fileSys, ASPathName path, ASBool recurse))

/**
	Deletes the folder at the specified <code>pathName</code> only if it is empty. 
	@param fileSys (May be <code>NULL</code>) The file system from which 
	<code>pathName</code> was obtained. Pass <code>NULL</code> to use the default file 
	system. 
	@param path The path of the folder to remove.
	@return <code>0</code> if the operation was successful, a non-zero 
	platform-dependent error code otherwise. 
	@exception genErrMethodNotImplemented 
	@see ASFileSysCreateFolder 
	@since PI_ACROSUPPORT_VERSION >= 0x00050000
*/
NPROC(ASErrorCode, 		ASFileSysRemoveFolder,			(ASFileSys fileSys, ASPathName path))

/**
	Gets the file access mode(s) specified for the file when it 
	was opened. 
	@param fN The file in question. 
	@return A value corresponding to one or more ASFileMode objects
	used to access or create the file, as shown in the table below. The 
	values that can be returned include combinations of the 
	following, OR'd with each other: 

	<p>Return value from ASFileGetOpenMode(): </p>
	<TABLE rules="all" cellspacing="1">
	<TR><TH>Return value</TH><TH>Meaning</TH></TR>
	<TR><TD><code>0</code></TD><TD>created</TD></TR>
	<TR><TD><code>1</code></TD><TD>readable</TD></TR>
	<TR><TD><code>2</code></TD><TD>readable and writable</TD></TR>
	<TR><TD><code>8</code></TD><TD>sequential access</TD></TR>
	<TR><TD><code>16</code></TD><TD>local</TD></TR>
	</TABLE>

	@since PI_ACROSUPPORT_VERSION >= 0x00050000
*/
NPROC(ASFileMode,		ASFileGetOpenMode,				(ASFile fN))

/**
	Returns a user-friendly representation of a path. It is 
	the caller's responsibility to release the memory associated 
	with the returned string using ASfree(). 
	
	@param fileSys (May be <code>NULL</code>) The file system from which 
	<code>pathName</code> was obtained. Pass <code>NULL</code> to use the default file 
	system. 
	@param path The ASPathName in question.
	@return A buffer containing the display string, or <code>NULL</code> if this 
	operation is not supported by the file system or some error 
	occurred. 

	@example
		<TABLE rules="all" cellspacing="1">
		<TR><TH>Operating system</TH><TH>Display string</TH></TR>
		<TR><TD>Windows</TD><TD><code>"C:\\Folder\\File"</code></TD></TR>
		<TR><TD>Mac OS</TD><TD><code>"Hard Disk:Folder:File"</code></TD></TR>
		<TR><TD>UNIX</TD><TD><code>"/Folder/File"</code></TD></TR>
		</TABLE>

	@see ASFileSysDisplayASTextFromPath 

	@note In Acrobat 6.0 and later, this method can only be used to get 
	host encoding. For any other encoding, use ASFileSysDisplayASTextFromPath(). 
	@since PI_ACROSUPPORT_VERSION >= 0x00050000
*/
NPROC(char *, 			ASFileSysDisplayStringFromPath,	(ASFileSys fileSys, ASPathName path))

/**
	Returns the number of seconds elapsed since midnight, January 
	1, 1970, coordinated universal time, up to the current time. 
	@return See above. 
	@since PI_ASEXTRA_VERSION >= 0x00050000
*/
SPROC(ASCount,			ASGetSecs,	(void), ASSecs)

/**
	Sets the type and creator of a file. See Type/Creator Codes. 
	
	@param fileSys IN/OUT The file system for which the type and creator 
	are needed. 
	@param path IN/OUT The path name of the file. 
	@param type IN/OUT The type of the file. 
	@param creator IN/OUT The creator of the file. 
	@return None. 
	@see ASFileSysGetTypeAndCreator 
	@note As is the case for ASFileSysGetTypeAndCreator(), this 
	method only applies to the Mac OS default file system. 
	Windows and UNIX make this a no-op. 
	@since PI_ACROSUPPORT_VERSION >= 0x00050000
	
*/
NPROC(void, 			ASFileSysSetTypeAndCreator,		(ASFileSys fileSys, ASPathName path, ASUns32 type, ASUns32 creator))

/**
	Gets the type and creator of the file specified by the path. 
	
	<p>See Creators and Acrobat Types. </p>
	
	@ref Creators
	@ref AcrobatTypes
	@param fileSys The file system containing the file for 
	which the type and creator are needed. 
	@param path The path name of the file. 
	@param type (Filled by method) The type of the file. 
	@param creator (Filled by method) The creator of the file.
	@see ASFileSysSetTypeAndCreator 

	@note This is only meaningful for the Mac OS default file system. 
	Windows and UNIX always return <code>0</code> for both <code>type</code> and <code>creator</code>. 
	@since PI_ACROSUPPORT_VERSION >= 0x00050000
*/
NPROC(void, 			ASFileSysGetTypeAndCreator,		(ASFileSys fileSys, ASPathName path, ASUns32 *type, ASUns32 *creator))

/**
	Determines whether the given byte is a lead byte of a multi-byte 
	character, and how many tail bytes follow. 

	<p>When parsing a string in a host encoding, you must keep in 
	mind that the string could be in a variable length multi-byte 
	encoding. In such an encoding (for example, Shift-JIS) the 
	number of bytes required to represent a character varies 
	on a character-by-character basis. To parse such a string 
	you must start at the beginning and, for each byte, determine 
	whether that byte represents a character or is the first 
	byte of a multi-byte character. If the byte is a lead byte 
	for a multi-byte character, you must also compute how many 
	bytes will follow the lead byte to make up the entire character. 
	Currently the API provides a call (PDHostMBLen()) that performs 
	these computations, but only if the encoding in question 
	is the operating system encoding (as returned by PDGetHostEncoding()). ASHostMBLen() 
	allows you to determine this for any byte in any host 
	encoding. </p>

	@param encoding The host encoding type. 
	@param byte The first byte of a multi-byte character.
	@return The number of additional bytes required to form the character. 
	For example, if the encoding is a double-byte encoding, the 
	return value will be <code>1</code> for a two-byte character and <code>0</code> for 
	a one-byte character. For Roman encodings, the return value 
	will always be <code>0</code>. 

	@note ASHostMBLen() cannot confirm whether the required number of 
	trailing bytes actually follow the first byte. If you are 
	parsing a multi-byte string, make sure your code will stop 
	at the first <code>NULL</code> (zero) byte even if it appears immediately 
	after the lead byte of a multi-byte character. 
	@see PDGetHostEncoding 
	@see PDHostMBLen 
	@since PI_ACROSUPPORT_VERSION >= 0x00050000
*/
//NPROC(ASInt32, ASHostMBLen, (ASHostEncoding encoding, ASUns8 byte))

/**
	Causes a hard flush on a file, which means that 
	the file is flushed to the physical destination. For example, 
	if a WebDAV-based file is opened, ASFileFlush() only flushes 
	changes to the local cached version of the file. This method 
	would flush changes all the way to the WebDAV server. 
	@param aFile The file that is flushed.
	@return <code>0</code> if the operation succeeded, <code>-1</code> if there was an error. 
	
	@see ASFileFlush 
	@since PI_ACROSUPPORT_VERSION >= 0x00050000
*/
NPROC(ASErrorCode, ASFileHardFlush, ( ASFile aFile ))

/**
	A new version of HFTReplaceEntry() that adds the extension argument. 
	
	<p>Plug-ins can use the REPLACE macro instead of calling HFTReplaceEntryEx 
	directly. </p>

	@param hft The HFT in which a method is replaced. Use 
	ASExtensionMgrGetHFT() to get the HFT, given its name. For 
	the HFTs built into the Acrobat viewer, global variables 
	containing the HFTs have been defined, so you can skip calling 
	ASExtensionMgrGetHFT() for these HFTs. 
	@param sel The entry in the HFT to replace, derived from 
	the method's name by appending <code>SEL</code>. For example, to replace 
	AVAlert, <code>sel</code> must be <code>AVAlertSEL</code>. 
	@param newEntry The function to replace the current one. 
	The function pointer must be converted to an HFTEntry using 
	the ASCallbackCreateReplacement() macro. 
	@param extension Plug-ins should pass in <code>gExtensionID</code> 
	for this parameter (see the code for the Acrobat 5.0 version 
	of the REPLACE macro). This parameter is stored by Acrobat 
	so that any entries that were replaced by a plug-in can 
	be unreplaced in the event that the plug-in unloads. 
	@param flags The new entry's properties. Currently, only 
	<code>HFTEntryReplaceable</code> is defined.
	@exception xmErrCannotReplaceSelector 
	@see ASExtensionMgrGetHFT 
	@see HFTGetReplacedEntry 
	@see HFTReplaceEntry 
	@see HFTUnreplaceEntry 
	@see CALL_REPLACED_PROC 
	@see REPLACE 
	@see ASCallbackCreateReplacement 
	@note The CALL_REPLACED_PROC macro is available to call 
	the previous HFT entry function that was replaced. 
	@since PI_ACROSUPPORT_VERSION >= 0x00050000
*/
NPROC(void, HFTReplaceEntryEx, (HFT hft, Selector sel, HFTEntry newEntry, ASExtension extension, ASFlagBits flags))

/**
	Removes the <code>oldEntry</code> item from <code>hft</code> at <code>sel</code> if the extension 
	fields match. It allows HFT replacements to be undone in cases 
	such as with the DigSig plug-in, which replaces a method 
	that Acrobat could use after DigSig unloads. 
	@param hft The HFT in which a method is un-replaced. Use 
	ASExtensionMgrGetHFT() to get the HFT, given its name. For 
	the HFTs built into the Acrobat viewer, global variables 
	containing the HFTs have been defined, so you can skip calling 
	ASExtensionMgrGetHFT() for these HFTs. 
	@param sel The entry in the HFT to un-replace, derived 
	from the method's name by appending <code>SEL</code>. For example, to 
	replace AVAlert, <code>sel</code> must be <code>AVAlertSEL</code>. 
	@param oldEntry The old function to be replaced. The function 
	pointer must be converted to an HFTEntry using the ASCallbackCreateReplacement() 
	macro. 
	@param extension An object of type ASExtension.
	@see ASExtensionMgrGetHFT 
	@see HFTGetReplacedEntry 
	@see HFTReplaceEntry 
	@see HFTReplaceEntryEx 
	@see REPLACE 
	@see ASCallbackCreateReplacement 
	@since PI_ACROSUPPORT_VERSION >= 0x00050000
*/
NPROC(void, HFTUnreplaceEntry, (HFT hft, Selector sel, HFTEntry oldEntry, ASExtension extension))

/* Acrobat 6.0 additions */

/**
	Extends ASProcStmRdOpen() and creates a read-only 
	ASStm from an arbitrary data-producing procedure. The stream 
	optionally supports seek operations, although external clients do not
	have the ability to initiate a seek operation.

	<p>The supplied handlers are called when the client of the 
	stream attempts to read data from it, seek it, or find it's
	length, as well as when the client closes it.</p> 

	@param handler A structure containing user-supplied callbacks 
	that supply the stream's data and destroy the stream. 
	@param clientData A pointer to user-supplied data to pass 
	to the procedures each time they are called.
	@return The newly created ASStm. 
	@exception genErrNoMemory 
	@see ASProcStmRdOpen 
	@see ASStmClose 
	@see ASStmRead 
	@since PI_ACROSUPPORT_VERSION >= 0x00060000
*/
NPROC(ASStm, ASProcStmRdOpenEx, (ASProcStmRdExHandler handler, void *clientData))

/**
	Generates a unique identifier (UUID). 
	@param dst (Filled by the method) The UUID created from the hash. 
	@return <code>true</code> if the UUID is successfully created, <code>false</code> otherwise. 
	
	@see ASUUIDFromCString 
	@see ASUUIDGenFromHash 
	@see ASUUIDGenFromName 
	@see ASUUIDToCString 
	@see AVAppGetUUID 
	@since PI_ASEXTRA_VERSION >= 0x00060000
*/
NPROC(ASBool, ASUUIDGenUnique, (ASUUID *dst))

/**
	Generates a universal unique identifier (UUID) for a block 
	of data (a name) in a context (a namespace). 
	@param dst (Filled by the method) The UUID created from the name. 
	@param ns A namespace or context meaningful to the client. 
	
	@param name A pointer to an arbitrary block of data to 
	be identified by the UUID. 
	@param bytes The number of bytes in <code>name</code>. 
	@return <code>true</code> if the UUID is successfully created, <code>false</code> otherwise. 
	
	@see ASUUIDFromCString 
	@see ASUUIDGenFromHash 
	@see ASUUIDGenUnique 
	@see ASUUIDToCString 
	@see AVAppGetUUID 
	@since PI_ASEXTRA_VERSION >= 0x00060000
*/
NPROC(ASBool, ASUUIDGenFromName, (ASUUID *dst, const ASUUID *ns, void *name, ASByteCount bytes))

/**
	Generates a unique identifier (UUID) from a hash value. 
	
	@param dst (Filled by the method) The UUID created from the hash. 
	@param hash A hash value, such as MD5.
	@return <code>true</code> if the UUID is successfully created, <code>false</code> otherwise. 
	
	@see ASUUIDFromCString 
	@see ASUUIDGenFromName 
	@see ASUUIDGenUnique 
	@see ASUUIDToCString 
	@see AVAppGetUUID 
	@since PI_ASEXTRA_VERSION >= 0x00060000
*/
NPROC(ASBool, ASUUIDGenFromHash, (ASUUID *dst, ASUns8 hash[16]))

/**
	Parses a C string, such as one generated by ASUUIDToCString(), 
	into a unique identifier (UUID). 
	@param dst (Filled by the method) The UUID created from the string. 
	@param str A <code>NULL</code>-terminated string from which to generate 
	the UUID, in the following form: <code>f81d4fae-7dec-11d0-a765-00a0c91e6bf6</code>.
	@return <code>true</code> if the UUID is successfully created, <code>false</code> otherwise. 
	
	@see ASUUIDGenFromHash 
	@see ASUUIDGenFromName 
	@see ASUUIDGenUnique 
	@see ASUUIDToCString 
	@see AVAppGetUUID 
	@since PI_ASEXTRA_VERSION >= 0x00060000
*/
NPROC(ASBool, ASUUIDFromCString, (ASUUID *dst, const char *str))

/**
	Generates a <code>NULL</code>-terminated C string from the unique identifier 
	(UUID) for a user or session. 
	@param dst (Filled by the method) A <code>NULL</code>-terminated string 
	from which to generate the UUID, in the following 
	form: <code>f81d4fae-7dec-11d0-a765-00a0c91e6bf6</code>. The string must 
	be at least the length specified by ASUUIDMaxStringLen(). 
	
	@param src The UUID from which to generate the string . 
	@see ASUUIDFromCString 
	@see ASUUIDGenFromHash 
	@see ASUUIDGenFromName 
	@see ASUUIDGenUnique 
	@see AVAppGetUUID 
	@since PI_ASEXTRA_VERSION >= 0x00060000
*/
NPROC(void, ASUUIDToCString, (char *dst, const ASUUID *src))

/**
	Deprecated API: always returns <code>NULL</code>.
	@since PI_ASEXTRA_VERSION >= 0x00060000
*/
NPROC(void *, ASFileSysGetPlatformThing, (ASFileSys fileSys, ASPathName path, ASAtom thing))

/**
	Returns a platform-specific file system representation of 
	the specified path, according to the specified type, wrapped 
	in an allocated ASPlatformPath object. It calls ASFileSysAcquirePlatformPathProc(). 
	
	<p>This method creates an equivalent platform-specific type 
	(such as FSRef on Mac OS) from an ASPathName. Use ASFileSysCreatePathName() 
	for the reverse situation to create an equivalent ASPathName from 
	a platform-specific type. </p>

	<p>In previous releases, you could cast an ASPathName to an 
	FSSpec, for example, but that does not work in the Acrobat 
	6.0 SDK and later (because of changes to accommodate long, Unicode 
	file names on Mac OS X). When developing for Mac OS, use 
	this call to get an FSSpec from an ASPathName safely on 
	Mac OS X, without casting. However, it is recommended that 
	you transition to using newer types such as FSRef to be 
	compatible with OS X filenames, or change to using all ASFileSys 
	methods. </p>

	@param fileSys (May be <code>NULL</code>) The file system from which 
	<code>pathName</code> was obtained. Pass <code>NULL</code> to use the default file 
	system. 
	@param path The ASPathName in the file system specified 
	by fileSys. 
	@param platformPathType The platform path type, one of 
	the following ASAtom values:
	
	<TABLE rules="all" cellspacing="1">
	<TR><TH>ASAtom value</TH><TH>Operating system</TH></TR>
	<TR><TD>FSRefWithCFStringRef</TD><TD>Mac OS</TD></TR>
	<TR><TD>FSSpec</TD><TD>Mac OS</TD></TR>
	<TR><TD>CFURLRef</TD><TD>Mac OS</TD></TR>
	<TR><TD>POSIXPath</TD><TD>Mac OS</TD></TR>
	<TR><TD>FSRef (<code>pathName</code> object must exist)</TD><TD>Mac OS</TD></TR>
	<TR><TD><code>CString</code></TD><TD>Windows/UNIX</TD></TR>
	</TABLE>

	@param platformPath (Filled by the method) The new platform 
	path object. Always free this object with ASFileSysReleasePlatformPath() 
	when done.
	@return <code>0</code> if the operation was successful, non-zero error 
	code otherwise. 
	@see ASFileSysReleasePlatformPath 
	@since PI_ACROSUPPORT_VERSION >= 0x00060000
*/
NPROC(ASInt32,	ASFileSysAcquirePlatformPath, (ASFileSys fileSys, ASPathName path, ASAtom platformPathType, ASPlatformPath *platformPath))

/**
	Releases the specified platform path object. Each call to 
	ASFileSysAcquirePlatformPath() should have a corresponding 
	call to this method. 
	@param fileSys (May be <code>NULL</code>) The file system from which 
	<code>pathName</code> was obtained. Pass <code>NULL</code> to use the default file 
	system. 
	@param platformPath The platform path object to release.
	@see ASFileSysAcquirePlatformPath 
	@since PI_ACROSUPPORT_VERSION >= 0x00060000
*/
NPROC(void, 	ASFileSysReleasePlatformPath, (ASFileSys fileSys, ASPlatformPath platformPath))

/**
	Gets a platform path object in the form of a C string for 
	Windows or UNIX, if the ASPlatformPath object 
	was acquired with this type in the <code>platformPathType</code> parameter 
	of ASFileSysAcquirePlatformPath(). 
	@param path The platform path.
	@return A pointer to a C string of a platform-specific path. 
	@see ASFileSysAcquirePlatformPath 
	@see ASFileSysReleasePlatformPath 

	@note Applications should use this as a read-only pointer; 
	modifying the returned buffer can corrupt the ASPlatformPath. 
	Do not free the pointer. 

	@note Do not release the returned value, or any member data 
	of an ASPlatformPath directly; use ASFileSysReleasePlatformPath() 
	when finished with the object. 
	@since PI_ACROSUPPORT_VERSION >= 0x00060000
*/
NPROC(Cstring_Ptr,					ASPlatformPathGetCstringPtr, (ASPlatformPath path))

#if  !MAC_PLATFORM || (MAC_PLATFORM && !AS_ARCH_64BIT)
/**
	This method was deprecated in Acrobat 9.0. Use ASPlatformPathGetFSRefPtr(),
	ASPlatformPathGetFSRefWithCFStringRefRecPtr(), ASPlatformPathGetCFURLRefRecPtr(),
	or ASPlatformPathGetPOSIXPathPtr() instead.

	Gets a platform path object in the form of an FSSpec for the 
	Mac OS, if the ASPlatformPath object was acquired with this 
	type in the <code>platformPathType</code> parameter of ASFileSysAcquirePlatformPath(). 
	@param path The platform path.
	@return A pointer to an FSSpec. 
	@see ASFileSysAcquirePlatformPath 
	@see ASPlatformPathGetFSRefPtr 
	@see ASFileSysReleasePlatformPath 
	
	@note Do not release the returned value, or any member data 
	of an ASPlatformPath directly; use ASFileSysReleasePlatformPath() 
	when finished with the object. 
	@since PI_ACROSUPPORT_VERSION >= 0x00060000
*/
NPROC(FSSpec_Ptr,					ASPlatformPathGetFSSpecPtr, (ASPlatformPath path))
#endif 

/**
	Gets a platform path object in the form of an FSRef for the 
	Mac OS, if the ASPlatformPath object was acquired with this 
	type in the <code>platformPathType</code> parameter of ASFileSysAcquirePlatformPath(). 
	@param path The platform path.
	@return A pointer to an FSRef. 
	@see ASFileSysAcquirePlatformPath 
	@see ASPlatformPathGetFSRefWithCFStringRefRecPtr 
	@see ASFileSysReleasePlatformPath 
	
	@note Do not release the returned value, or any member data 
	of an ASPlatformPath directly; use ASFileSysReleasePlatformPath() 
	when finished with the object. 
	@since PI_ACROSUPPORT_VERSION >= 0x00060000
*/
NPROC(FSRef_Ptr,					ASPlatformPathGetFSRefPtr, (ASPlatformPath path))

/**
	Gets a platform path object in the form of an FSRef and 
	CFStringRef for Mac OS, if the ASPlatformPath object was 
	acquired with this type in the <code>platformPathType</code> parameter 
	of ASFileSysAcquirePlatformPath(). 
	@param path The platform path.
	@return A pointer to a structure containing an FSRef and a CFStringRef. 
	
	@see ASFileSysAcquirePlatformPath 
	@see ASPlatformPathGetFSRefPtr 
	@see ASFileSysReleasePlatformPath 

	@note Do not release the returned value, or any member data 
	of an ASPlatformPath directly; use ASFileSysReleasePlatformPath() 
	when finished with the object. 
	@since PI_ACROSUPPORT_VERSION >= 0x00060000
*/
NPROC(FSRefWithCFStringRefRec_Ptr,	ASPlatformPathGetFSRefWithCFStringRefRecPtr, (ASPlatformPath path))

/**
	Gets a platform path object in the form of a CFURLRef for the
	Mac OS, if the ASPlatformPath object was acquired with this 
	type in the <code>platformPathType</code> parameter of ASFileSysAcquirePlatformPath(). 
	@param path The platform path.
	@return A pointer to a structure containing a CFURLRef. 
	@see ASFileSysAcquirePlatformPath 
	@see ASFileSysReleasePlatformPath 
	
	@note Do not release the returned value, or any member data 
	of an ASPlatformPath directly; use ASFileSysReleasePlatformPath() 
	when finished with the object. 
	@since PI_ACROSUPPORT_VERSION >= 0x00060000
*/
NPROC(CFURLRefRec_Ptr,				ASPlatformPathGetCFURLRefRecPtr, (ASPlatformPath path))

/**
	Gets a platform path object in the form of a POSIX path 
	C string, if the ASPlatformPath object was acquired with 
	this type in the <code>platformPathType</code> parameter of ASFileSysAcquirePlatformPath(). 
	@param path The platform path.
	@return A pointer to a POSIX path (UTF-8 encoding) as a C string. 
	
	@see ASFileSysAcquirePlatformPath 
	@see ASFileSysReleasePlatformPath 

	@note Do not release the returned value, or any member data 
	of an ASPlatformPath directly; use ASFileSysReleasePlatformPath() 
	when finished with the object. 
	@since PI_ACROSUPPORT_VERSION >= 0x00060000
*/
NPROC(POSIXPath_Ptr,				ASPlatformPathGetPOSIXPathPtr, (ASPlatformPath path))

/**
	Extracts the file name (including the extension) from the path as 
	an ASText object. This method supersedes ASFileSysGetNameFromPath() in 
	Acrobat 6.0 and later. 
	@param fileSys (May be <code>NULL</code>) The file system from which 
	<code>pathName</code> was obtained. Pass <code>NULL</code> to use the default file 
	system. 
	@param pathName The ASPathName associated with the file 
	in question. 
	@param name (Filled by the method) The text object containing 
	the file name.
	@return <code>0</code> if the operation was successful, a non-zero 
	platform-dependent error code otherwise. 
	@exception fileErrGeneral 
	@see ASFileSysGetNameFromPath 
	@since PI_ACROSUPPORT_VERSION >= 0x00060000
*/
NPROC(ASErrorCode,					ASFileSysGetNameFromPathAsASText, (ASFileSys fileSys, ASPathName pathName, ASText name))

/**
	Returns a user-friendly representation of a path as a text 
	object. It calls ASFileSysDisplayASTextFromPathProc(). 

	<p>This method supersedes ASFileSysDisplayStringFromPath() in Acrobat 6.0 and later.</p>

	@param fileSys (May be <code>NULL</code>) The file system from which 
	<code>pathName</code> was obtained. Pass <code>NULL</code> to use the default file 
	system. 
	@param path The ASPathName in question. 
	@param displayText (Filled by method) The text object 
	containing the display representation of the path. 
	@return <code>0</code> if the operation was successful, a non-zero 
	platform-dependent error code otherwise. 
	@see ASFileSysDisplayStringFromPath 
	@since PI_ACROSUPPORT_VERSION >= 0x00060000
*/
NPROC(ASErrorCode, 					ASFileSysDisplayASTextFromPath, (ASFileSys fileSys, ASPathName path, ASText displayText))


/**
	Flushes any buffered data to the specified stream. 
	@param stm The stream to flush.
	@return <code>0</code> if successful, non-zero otherwise. 
	@see ASStmClose 
	@since PI_ACROSUPPORT_VERSION >= 0x00060000
*/
NPROC(ASTCount, ASStmFlush, (ASStm stm))

/**
	Determines whether there are any outstanding multi-byte range 
	requests for a file. A document can have outstanding <code>mreads</code> 
	if it was opened in a browser, Acrobat requested some 
	byte ranges, and the byte ranges have not yet arrived. 
	@param fN The file in question.
	@return <code>true</code> if the file has outstanding <code>mreads</code>, <code>false</code> otherwise. 
	
	@since PI_ACROSUPPORT_VERSION >= 0x00060000
*/
NPROC(ASBool,						ASFileHasOutstandingMReads,(ASFile fN))

/**
	Checks if ASFileSetEOF() can be done for this file with a specified new file size.
	@param file The file in question.
	@param newFileSize The proposed new file size. This parameter will be treated as unsigned.
	@see ASFileSetEOF
	@see ASFileGetEOF
	@see ASFileGetPos
	@since PI_ACROSUPPORT_VERSION >= 0x00060000
*/
NPROC(ASBool, ASFileCanSetEOF, (ASFile file, ASInt32 newFileSize))

/**
	Returns the version of the HFT, if available. 
	@param hft The HFT whose version is obtained.
	@return The version number if the HFT is valid and the version is 
	available, <code>HFT_ERROR_NO_VERSION</code> otherwise. 
	@see HFTNewEx 
	@since PI_ACROSUPPORT_VERSION >= 0x00060000
*/
NPROC(ASVersion, HFTGetVersion, (HFT hft))


/**
	Extends HFTNew() with version information in Acrobat 6. Creates 
	a new HFT by calling the specified HFT server's HFTServerProvideHFTProc(). 
	
	@param hftServer The HFT server for the HFT being created. 
	The HFT server must have been created previously using HFTServerNew(). 
	
	@param data The data to pass to the server, which includes: 
	<ul>
		<li>
			The number of entries in the new HFT, which determines 
			the number of methods that the HFT can contain. Each method 
			occupies one entry.
		</li>
		<li>The HFT version.</li>
	</ul>
	@return The newly created HFT. 
	@see HFTNew 
	@see ASExtensionMgrGetHFT 
	@see HFTDestroy 
	@since PI_ACROSUPPORT_VERSION >= 0x00060000
*/
NPROC(HFT, HFTNewEx, (HFTServer hftServer, HFTData data))

/**
	Converts a file name, specified as an ASPathName, to a device-independent 
	path name, which is returned as an ASText object. It calls ASFileSysDIPathFromPathExProc(). 
	
	<p>This method supersedes ASFileSysDIPathFromPath() in Acrobat 6.0 and later. </p>
	
	@param fileSys (May be <code>NULL</code>) The file system from which 
	<code>pathName</code> was obtained. Pass <code>NULL</code> to use the default file 
	system. 
	@param path The ASPathName to convert. 
	@param relativeToThisPath (May be <code>NULL</code>) The path name relative 
	to which the device-independent path name is specified. If it is
	<code>NULL</code>, the device-independent path name will be an absolute, 
	not a relative, path name. 
	@param diPathText (Filled by the method) The ASText object 
	to contain the device-independent path. It must be allocated 
	and freed by the client.
	@return <code>0</code> if the operation was successful, a non-zero 
	platform-dependent error code otherwise. 
	@see ASGetDefaultFileSys 
	@see ASFileSysPathFromDIPathEx 
	@since PI_ACROSUPPORT_VERSION >= 0x00060000
*/
NPROC(ASErrorCode, ASFileSysDIPathFromPathEx, ( ASFileSys fileSys, ASPathName path, ASPathName relativeToThisPath, ASText diPathText))


/**
	Converts a device-independent path name in an ASText object 
	to an ASPathName. This method can only be used for files 
	that already exist (that is, it cannot be used to create 
	a placeholder path name for files that a plug-in intends 
	to create in the future). 

	<p>It is the caller's responsibility to release the returned ASPathName.</p> 

	<p>This method supersedes ASFileSysPathFromDIPath() in Acrobat 6.0 and later.</p> 
	
	@param fileSys (May be <code>NULL</code>) The file system within which the 
	ASPathName will be created. Pass <code>NULL</code> to use the 
	default file system. 
	@param diPathText The device-independent path name to convert, 
	as an ASText object. See Section 3.10 in the <i>PDF Reference</i> 
	for a description of the device-independent path name format. 
	This path name may not be understood on another platform 
	since drive specifiers may be prepended. On Windows, you 
	cannot specify a UNC path name. You must have a file mounted 
	on the file server. For example, the following path is valid: 
	<code>/f/dirname/file.pdf</code> where <code>f</code> is <code>\\server\\people</code>. The following 
	does not work: <code>/server/people/dirname/file.pdf</code>.
	@param relativeToThisPath A path name relative to which <code>diPath</code> 
	is interpreted. If <code>NULL</code>, <code>diPath</code> is interpreted as an absolute 
	path name, not a relative path name.
	@return An ASPathName corresponding to the parameter values supplied. 
	Returns <code>NULL</code> if <code>diPath</code> cannot be converted to an ASPathName. This would occur, 
	for example, if the specified file does not already exist. 
	
	@exception genErrNoMemory 
	@see ASFileSysDIPathFromPathEx 
	@see ASFileSysReleasePath 
	@since PI_ACROSUPPORT_VERSION >= 0x00060000
*/
NPROC(ASPathName, ASFileSysPathFromDIPathEx, (ASFileSys fileSys, ASConstText diPathText, ASPathName relativeToThisPath))


/**
	Gets the temporary file system implementation for 
	a platform. 
	@return The platform's default file system. 
	@see ASSetTempFileSys 
	@since PI_ACROSUPPORT_VERSION >= 0x00060000
*/
NPROC(ASFileSys, ASGetTempFileSys, (void)) /* JGS */

/**
	Sets the temporary file system implementation for 
	a platform. 
	@return none 
	@see ASGetDefaultFileSys 
	@see ASGetTempFileSys 
	@see ASGetRamFileSys 
	@since PI_ACROSUPPORT_VERSION >= 0x00060000
*/
NPROC(void, ASSetTempFileSys, (ASFileSys fileSys)) /* JGS */

/**
	Gets the in-memory file system implementation for 
	a platform. 
	@return The platform's in-memory file system. 
	@see ASSetDefaultFileSys 
	@see ASSetTempFileSys 
	@since PI_ACROSUPPORT_VERSION >= 0x00060000
*/
NPROC(ASFileSys, ASGetRamFileSys, (void)) /* JGS */

#if ADD_ASFIXED //PKG
/**
	Converts an ASFixed to a <code>float</code>.
	
	@param inASFixed IN The ASFixed value to convert.
	@return The <code>float</code> representation of the ASFixed value.
	
*/
NPROC(float, ASFixedToFloat, (ASFixed inASFixed))

/**
	Converts a <code>float</code> to an ASFixed value.
	
	@param inFloat IN The <code>float</code> value to convert.
	@return The ASFixed representation of the <code>float</code> value.
	
*/
NPROC(ASFixed, FloatToASFixed, (double inFloat))
#endif //ADD_ASFIXED //PKG

/**
	Attempts to open a file in the specified file system, in 
	the specified read/write/create mode. If the file is already 
	open, the existing file handle is returned. The caller retains 
	ownership of <code>pathName</code>.
	
	<p>This call can open files over 2 GB in length and should be used
	instead of ASFileSysOpenFile() whenever possible.</p>

	<p>On Mac OS, when this method creates a file, the file's creator 
	is set to <code>'CARO'</code> and its type is set to <code>'PDF '</code> (with a 
	space after PDF). </p>

	@param fileSys IN/OUT (May be <code>NULL</code>) The file system from which the 
	path name was obtained. Pass <code>NULL</code> to use the default file 
	system. 
	@param pathName IN/OUT The path name of the file to open. 
	@param mode IN/OUT An <code>OR</code> of the ASFile Open Modes. 
	@param fP IN/OUT (Filled by the method) The ASFile that was opened. 

	@ref ASFileOpenModes

	@return <code>0</code> if the operation was successful, a non-zero 
	error code otherwise. The error is platform and file-system specific:  

	<TABLE rules="all" cellspacing="1">
	<TR><TH>Platform</TH><TH>Error</TH></TR>
	<TR><TD>Windows</TD><TD><ul>
		<li>Returns fileErrWrPerm if trying to open a read-only file with write permissions.</li>
		<li>Returns ErrSysXtnMgr (use GetLastError()) for platform-specific error conditions that CreateFile() may use.</li>
		<li>Returns <code>fileErrGeneral</code> if the developer passed in an invalid ASPathName.</li>
		</ul></TD>
	</TR>
	<TR><TD>Mac OS</TD><TD><ul>
		<li>Returns fileErrFNF if trying to open a file for reading that does not exist.</li>
		<li>Returns ErrSysMDSystem for platform-specific errors that <code>FSpCreate</code>, <code>FSpSetFInfo</code>, <code>FSpOpenRF</code>, <code>FSpOpenDF</code>, or <code>SetFPos</code> may use).</li>
		<li>Returns <code>fileErrGeneral</code> if the developer passed in an invalid ASPathName.</li>
		</ul></TD>
	</TR>
	</TABLE>

	@exception genErrNoError 
	@see ASFileClose 
	@see ASFileReopen 
	@see ASGetDefaultFileSys 
	@since PI_ACROSUPPORT_VERSION >= 0x00070000
	
*/
NPROC(ASErrorCode, ASFileSysOpenFile64, (ASFileSys fileSys, ASPathName pathName, ASFileMode mode, ASFile *fP))

/**
	Returns the maximum file position that can be processed by this file
	system. This is not the maximum size file that can be created or the
	amount of room left in the file system, but the maximum file position
	that can be handled by the arithmetic in the file system implementation.
	This will typically be <code>(2 ^ 31) - 1</code> or <code>(2 ^ 63) - 1</code>.
	@return The maximum file position that can be processed.
	@param fileSys IN/OUT (May be <code>NULL</code>) The file system from which 
	the path name was obtained. Pass <code>NULL</code> to use the default file 
	system. 
	@see ASFileSysOpenFile64 
	@since PI_ACROSUPPORT_VERSION >= 0x00070000
	
*/
NPROC(ASFilePos64, ASFileSysGetFilePosLimit, (ASFileSys fileSys))

/**
	Seeks to the specified position in a file. This is the position 
	at which the next read or write will begin. This call will
	work with files over 2 GB in length.
	@param aFile IN/OUT The file in which to seek. 
	@param pos IN/OUT The position to seek. 
	@exception fileErrIO 
	@see ASFileGetPos64 
	@see ASFileRead 
	@see ASFileWrite 
	@since PI_ACROSUPPORT_VERSION >= 0x00070000
	
*/
NPROC(void, ASFileSetPos64, ( ASFile aFile, ASFilePos64 pos ))

/**
	Gets the current seek position in a file. This is the position 
	at which the next read or write will begin. This call will
	work with files over 2 GB in length.
	@param aFile IN/OUT The file in which to get the seek position. 
	
	@return The current seek position. 
	@exception fileErrIO 
	@see ASFileSetPos64
	@see ASFileRead 
	@see ASFileWrite 
	@since PI_ACROSUPPORT_VERSION >= 0x00070000
	
*/
NPROC(ASFilePos64, ASFileGetPos64, ( ASFile aFile ))

/**
	Changes the size of a file. The new size may by larger or 
	smaller than the original size. This method may raise file 
	system or platform-specific exceptions. This call will
	work with files over 2 GB in length.
	@param aFile IN/OUT The file whose size is changed. 
	@param newFileSize IN/OUT The new size of the file. 
	@exception fileErrIO 
	@see ASFileGetEOF64 
	@see ASFileGetPos 
	@since PI_ACROSUPPORT_VERSION >= 0x00070000
	
*/
NPROC(void, ASFileSetEOF64, ( ASFile aFile, ASFilePos64 newFileSize ))

/**
	Gets the current size of a file. 
	@param aFile IN/OUT The ASFile whose size is obtained. This
	call will work with files over 2 GB in length.
	@return The size of the file. 
	@exception fileErrIO 
	@see ASFileSetEOF64 
	@see ASFileSetPos 
	@since PI_ACROSUPPORT_VERSION >= 0x00070000
	
*/
NPROC(ASFilePos64, ASFileGetEOF64, ( ASFile aFile ))

/**
	Sets the temporary file system implementation for 
	a platform. 
	@see ASGetRamFileSys 
	@since PI_ACROSUPPORT_VERSION >= 0x00070000
*/
NPROC(void, ASRamFileSysSetLimitKB, (ASInt32 limit))

/**
	This method writes into <code>nameForDisplay</code>
	the representation of that item as it would be shown in Windows Explorer
	or Mac OS Finder. For example, it will provide the localized string for
	<code>"My Documents"</code> even though, on disk, <code>"My Documents"</code> is always in English.
	It will also strip the extension if that is what Windows Explorer or the
	Mac Finder would do for that file.
	@param fileSys (May be <code>NULL</code>) The file system from which 
	<code>pathName</code> was obtained. Pass <code>NULL</code> to use the default file 
	system. 
	@param pathName The ASPathName associated with the file 
	in question. 
	@param nameForDisplay (Filled by the method) The text object containing 
	the name used for display.
	@return <code>0</code> if the operation was successful, a non-zero 
	platform-dependent error code otherwise. 
	@exception fileErrGeneral 
	@since PI_ACROSUPPORT_VERSION >= 0x00070000
*/
NPROC(ASErrorCode, ASFileSysGetNameFromPathForDisplay, (ASFileSys fileSys, ASPathName pathName, ASText nameForDisplay))

/**
	Gets the file system implementation that supports 
	Unicode file path names. If a platform does not
	have a file system that supports Unicode, then <code>NULL</code>
	will be returned.
	@return The platform's Unicode file system. 
	@see ASGetDefaultFileSys 
	@since PI_ACROSUPPORT_VERSION >= 0x00080000
	
*/
NPROC(ASFileSys, ASGetDefaultUnicodeFileSys, (void))

/**
	Gets an ASText object containing a string describing the specified exception. 
	
	@param errorCode The exception whose error string is obtained. 
	This must be a full error code, built with the ErrBuildCode 
	macro or a user-defined exception returned from ASRegisterErrorString(). 
	See Error Systems for a list of predefined exceptions.
	@param errorString (Filled by the method) The text object containing the error string. 
	The client must pass a valid ASText object. The routine does not allocate it.

    @see ASGetErrorString
	@see ASRegisterErrorStringASText
	@see ASRegisterErrorString
	@see ASGetExceptionErrorCode
	@see ASRaise
	@ref ErrorSystems
	@since PI_ACROSUPPORT_VERSION >= 0x00080000
*/
NPROC(void, ASGetErrorStringASText, (ASErrorCode errorCode, ASText errorString))

/**
	Registers a new error and string. 
	
	@param severity The severity of the error being defined. 
	It must be one of the Error Severities. 
	@param errorString The text object containing the error string to be set.
	@return The newly created error code. Plug-ins should assign the 
	error code returned by this method to a variable if they 
	wish to use the error code later in the current session. 
	
    @see ASRegisterErrorString
	@see ASGetErrorStringASText
	@see ASGetErrorString
	@see ASRaise
	@ref ErrorSeverities
	@since PI_ACROSUPPORT_VERSION >= 0x00080000
*/
NPROC(ASErrorCode, ASRegisterErrorStringASText, (ASErrSeverity severity, const ASText errorString))

/**
	Gets the best file system implementation that supports 
	the passed in path.  If the path requires the
	Unicode file system then the default Unicode file system
	is returned, otherwise the default file system is returned.
	@return The platform's default or Unicode file system. 
	@see ASGetDefaultFileSys 
	@see ASGetDefaultUnicodeFileSys 
	@since PI_ACROSUPPORT_VERSION >= 0x00080000
	
*/
NPROC(ASFileSys, ASGetDefaultFileSysForPath, (ASAtom pathSpecType, const void *pathSpec))

/**
	Returns <code>true</code> if <code>fileSys</code> is <code>NULL</code>,
	the default file system or the default Unicode file system.
	@return <code>true</code> if <code>fileSys</code> is <code>NULL</code>
	or a local file system. 
	@see ASGetDefaultFileSys 
	@see ASGetDefaultUnicodeFileSys 
	@since PI_ACROSUPPORT_VERSION >= 0x00080000
	
*/
NPROC(ASBool, ASFileSysIsLocal, (ASFileSys fileSys))

/**
	Gets the amount of free space on the volume containing <code>pathName</code>. 
	This is the same as ASFileSysGetStorageFreeSpace() without the 4 GB limit.
	
	@param fileSys IN/OUT (May be <code>NULL</code>) The file system from which 
	<code>pathName</code> was obtained. Pass <code>NULL</code> to use the default file 
	system. 
	@param pathName IN/OUT The ASPathName. 
	@return The amount of free space in bytes, <code>0</code> otherwise. 
	@since PI_ACROSUPPORT_VERSION >= 0x00080000
	
*/
NPROC(ASDiskSpace64,		ASFileSysGetStorageFreeSpace64,	(ASFileSys fileSys, ASPathName pathName))

#if ACROBAT_LIBRARY
/**
	Multiplies two matrices. 
	@param result (Filled by the method) A pointer to matrix 
	<code>m2 x m1</code>. It is allowed for the result to point to the same location 
	as either <code>m1</code> or <code>m2</code>. 
	@param m1 A pointer to the ASDoubleMatrix value for the first 
	matrix to multiply. 
	@param m2 A pointer to the ASDoubleMatrix value for the second 
	matrix to multiply.
	@see ASDoubleMatrixInvert 
	@see ASDoubleMatrixTransform 
	@see ASDoubleMatrixTransformRect
	@since PI_ACROSUPPORT_VERSION >= 0x000A0000
*/
NPROC(void, ASDoubleMatrixConcat, (ASDoubleMatrix *result, const ASDoubleMatrix *m1,const ASDoubleMatrix *m2))

/**
	Inverts a matrix. 

	<p>If a matrix is nearly singular (which means that it has a determinant 
	that is nearly zero), inverting and re-inverting the matrix may 
	not yield the original matrix. </p>
	@param result (Filled by the method) A pointer to <code>m-1</code>. It 
	is allowed for the result to point to the same location as <code>m</code>. 
	@param m A pointer to the ASDoubleMatrix to invert.
	@see ASDoubleMatrixConcat 
	@see ASDoubleMatrixTransform 
	@see ASDoubleMatrixTransformRect
	@since PI_ACROSUPPORT_VERSION >= 0x000A0000
*/
NPROC(void, ASDoubleMatrixInvert, (ASDoubleMatrix *result, const ASDoubleMatrix *m))

/**
	Transforms the point <code>p</code> through the matrix <code>m</code>, and puts the result 
	in <code>result</code>. <code>p</code> and <code>result</code> can point to the same location. 
	@param result (Filled by the method) A pointer to the ASDoublePoint 
	containing the result of transforming <code>p</code> through <code>m</code>. It is 
	allowed for the result to point to the same location as <code>m</code>. 
	@param m A pointer to the ASDoubleMatrix through which <code>p</code> 
	is transformed. 
	@param p A pointer to the ASDoublePoint representing the 
	point to transform through <code>m</code>.
	@see ASDoubleMatrixConcat 
	@see ASDoubleMatrixInvert 
	@see ASDoubleMatrixTransformRect
	@since PI_ACROSUPPORT_VERSION >= 0x000A0000
*/
NPROC(void, ASDoubleMatrixTransform, (ASDoublePoint *result, const ASDoubleMatrix *m,const ASDoublePoint *p))

/**
	Transforms a rectangle through a matrix. 
	@param result (Filled by the method) A pointer to the ASDoubleRect 
	containing the smallest bounding box for the transformed 
	rectangle. It is allowed for the result to point to the same location 
	as <code>m</code>. result will always have <code>bottom < top</code> and <code>left < right</code>. 
	
	@param m A pointer to the ASDoubleMatrix containing the matrix 
	through which <code>r</code> is transformed. 
	@param r A pointer to the ASDoubleRect containing the 
	rectangle to transform through <code>m</code>.
	@see ASDoubleMatrixTransform 
	@see ASDoubleMatrixConcat 
	@see ASDoubleMatrixInvert 
	@since PI_ACROSUPPORT_VERSION >= 0x000A0000
*/
NPROC(void, ASDoubleMatrixTransformRect, (ASDoubleRect* result, const ASDoubleMatrix* m,const ASDoubleRect* r))
#else

/**
	Multiplies two matrices. 
	@param result (Filled by the method) A pointer to matrix 
	<code>m2 x m1</code>. It is allowed for the result to point to the same location 
	as either <code>m1</code> or <code>m2</code>. 
	@param m1 A pointer to the ASDoubleMatrix value for the first 
	matrix to multiply. 
	@param m2 A pointer to the ASDoubleMatrix value for the second 
	matrix to multiply.
	@see ASDoubleMatrixInvert 
	@see ASDoubleMatrixTransform 
	@see ASDoubleMatrixTransformRect
	@since PI_ACROSUPPORT_VERSION >= 0x000A0000
*/
NPROC(void, ASDoubleMatrixConcat, (ASDoubleMatrix *result, const ASDoubleMatrix *m1,const ASDoubleMatrix *m2))

/**
	Inverts a matrix. 

	<p>If a matrix is nearly singular (which means that it has a determinant 
	that is nearly zero), inverting and re-inverting the matrix may 
	not yield the original matrix. </p>
	@param result (Filled by the method) A pointer to <code>m-1</code>. It 
	is allowed for the result to point to the same location as <code>m</code>. 
	@param m A pointer to the ASDoubleMatrix to invert.
	@see ASDoubleMatrixConcat 
	@see ASDoubleMatrixTransform 
	@see ASDoubleMatrixTransformRect
	@since PI_ACROSUPPORT_VERSION >= 0x000A0000
*/
NPROC(void, ASDoubleMatrixInvert, (ASDoubleMatrix *result, const ASDoubleMatrix *m))

/**
	Transforms the point <code>p</code> through the matrix <code>m</code>, and puts the result 
	in <code>result</code>. <code>p</code> and <code>result</code> can point to the same location. 
	@param result (Filled by the method) A pointer to the ASDoublePoint 
	containing the result of transforming <code>p</code> through <code>m</code>. It is 
	allowed for the result to point to the same location as <code>m</code>. 
	@param m A pointer to the ASDoubleMatrix through which <code>p</code> 
	is transformed. 
	@param p A pointer to the ASDoublePoint representing the 
	point to transform through <code>m</code>.
	@see ASDoubleMatrixConcat 
	@see ASDoubleMatrixInvert 
	@see ASDoubleMatrixTransformRect
	@since PI_ACROSUPPORT_VERSION >= 0x000A0000
*/
NPROC(void, ASDoubleMatrixTransform, (ASDoublePoint *result, const ASDoubleMatrix *m,const ASDoublePoint *p))

/**
	Transforms a rectangle through a matrix. 
	@param result (Filled by the method) A pointer to the ASDoubleRect 
	containing the smallest bounding box for the transformed 
	rectangle. It is allowed for the result to point to the same location 
	as <code>m</code>. result will always have <code>bottom < top</code> and <code>left < right</code>. 
	
	@param m A pointer to the ASDoubleMatrix containing the matrix 
	through which <code>r</code> is transformed. 
	@param r A pointer to the ASDoubleRect containing the 
	rectangle to transform through <code>m</code>.
	@see ASDoubleMatrixTransform 
	@see ASDoubleMatrixConcat 
	@see ASDoubleMatrixInvert 
	@since PI_ACROSUPPORT_VERSION >= 0x000A0000
*/
NPROC(void, ASDoubleMatrixTransformRect, (ASDoubleRect* result, const ASDoubleMatrix* m,const ASDoubleRect* rectIn))
#endif

/**
	ASFileOpenWithVirtualEOF attempts to create a second ASFile instance
	to a file that is already open. Both the current instance fN and the
	new instance must be read only. The new instance shall set a virtual 
	end of file. This virtual EOF and no effect on the first instance or
	on the physical file. It only effect the ASFile calls where newFile
	is passed in has the file. Each instance maintains it's own file 
	position marker. The original instance of the file should be close
	after all other instances have been closed. This routine does not
	raise, but returns an error code. 
 
	@param fN IN The ASFile to base the new file on 
	@param virtualEOF IN The new EOF. 
	@parem newFile OUT the new ASFile.
	@return Error code if the newFile could not be created. 
	@since PI_ACROSUPPORT_VERSION >= 0x000A0000
 
 */
NPROC(ASInt32,		ASFileOpenWithVirtualEOF,	(ASFile fN, ASFilePos64 virtualEOF, ASFile *newFile))
