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

 AcroErr.h

 - Error codes are used in the ASRaise/DURING/HANDLER mechanism
   established in Except.h and also as error code return values from
   certain lower-level routines that do not raise. Error codes are
   32-bit integers (ASInt32). An error code can be built using the
   ErrBuildCode macro.

 - An error flag has three components:

   Severity: none, warning, severe			; 4 bits 	=> 16 severities
   System: Cos, PDDoc, etc.					; 8 bits 	=> 256 systems
   Error: FileOpen, Syntax, etc.			; 16 bits   => 65535 errors

*********************************************************************/

#ifndef _H_AcroErr
#define _H_AcroErr

#include "CoreExpT.h"

/* These are used so frequently they have their own macro */
#define ERR_GENERAL			1
#define ERR_NOMEMORY		2

#define ERR_SEVERITY_MASK	0xf0000000	/* upper four bits */
#define ERR_UNUSED_MASK		0x0f000000	/* reserved */
#define ERR_SYSTEM_MASK		0x00ff0000
#define ERR_ERROR_MASK		0x0000ffff	/* put this last to access it fastest */

#define ERR_SEVERITY_SHIFT	28
#define ERR_SYSTEM_SHIFT	16
#define ERR_ERROR_SHIFT		0

/* Errors but not severities or systems may be negative */
/**
	Builds an error code for the specified severity, system, and error number. Error codes are used in exception handling. 
	The ASRaise() method takes an error code for its argument; ASRegisterErrorString() returns an error code.

	An error code has three components:

	<TABLE rules="all" cellspacing="1">
	<TR><TH>Component</TH><TH>Description</TH><TH>Size</TH></TR>
	<TR><TD>Severity</TD><TD>none, warning, severe</TD><TD>4 bits</TD></TR>
	<TR><TD>System</TD><TD>Cos, PDDoc, and so on</TD><TD>8 bits</TD></TR>
	<TR><TD>Error</TD><TD>FileOpen, Syntax, and so on</TD><TD>16 bits</TD></TR>
	</TABLE>

	@param xseverity	The severity of the error. It must be one of ErrorSeverities.
	@param xsys			The error system. It must be one of ErrorSystems.
	@param xerror		The error number in the error system.

	@example	<code>myErrorCode = ErrBuildCode(ErrAlways, ErrSysAcroView, avErrActionRestricted);</code>

	@see ErrGetCode
	@see ErrGetSeverity
	@see ErrGetSignedCode
	@see ErrGetSystem
	@see ASRaise
	@see ASRegisterErrorString
	@ref ErrorSeverities
	@ref ErrorSystems
*/
#define ErrBuildCode(xseverity,xsys,xerror) \
			( \
			 (((ASUns32) (xseverity))	<< ERR_SEVERITY_SHIFT)	| \
			 (((ASUns32) (xsys))		<< ERR_SYSTEM_SHIFT)	| \
			((((ASUns32) (xerror))	<< ERR_ERROR_SHIFT) & ERR_ERROR_MASK) \
			)

/* ErrGetCode will return a value between 0 & 65535 inclusive.
 * If the platform considers errors to be signed, use ErrGetSignedCode.
 */
/**
	Gets the error severity from an error code. It returns one of ErrorSeverities.

	@param xcode	The error code.

	@example	<code>errorSeverity = ErrGetSeverity(errorCode);</code>

	@see ErrBuildCode
	@see ErrGetCode
	@see ErrGetSignedCode
	@see ErrGetSystem
	@see ASGetErrorString
	@see ASGetExceptionErrorCode
	@see ASRegisterErrorString
	@ref ErrorSeverities
*/
#define ErrGetSeverity(xcode)	((((ASUns32) (xcode)) & ERR_SEVERITY_MASK)	>> ERR_SEVERITY_SHIFT)
/**
	Gets the error system from an error code. It returns one of ErrorSystems.

	@param xcode	The error code.

	@example	<code>errorSystem = ErrGetSystem(errorCode);</code>

	@see ErrBuildCode
	@see ErrGetCode
	@see ErrGetSeverity
	@see ErrGetSignedCode
	@see ASGetErrorString
	@see ASGetExceptionErrorCode
	@see ASRegisterErrorString
	@ref ErrorSystems
*/
#define ErrGetSystem(xcode)		((((ASUns32) (xcode)) & ERR_SYSTEM_MASK)		>> ERR_SYSTEM_SHIFT)
/**
	Gets the error number from an error code.

	@param xcode	The error code.

	@example	<code>errorNumber = ErrGetCode(errorCode);</code>

	@see ErrBuildCode
	@see ErrGetSeverity
	@see ErrGetSignedCode
	@see ErrGetSystem
	@see ASGetErrorString
	@see ASGetExceptionErrorCode
	@see ASRegisterErrorString
*/
#define ErrGetCode(xcode)		((((ASUns32) (xcode)) & ERR_ERROR_MASK)		>> ERR_ERROR_SHIFT)
/**
	Gets a signed error number from an error code.

	@param xcode	The error code.

	@example	<code>errorSignedNumber = ErrGetSignedCode(errorCode);</code>

	@see ErrBuildCode
	@see ErrGetCode
	@see ErrGetSeverity
	@see ErrGetSystem
	@see ASGetErrorString
	@see ASGetExceptionErrorCode
	@see ASRegisterErrorString
*/
#define ErrGetSignedCode(xcode)	((ASInt16) ErrGetCode(xcode))

/* Error severity */
enum {
	/**
		No error occurred.
		@ingroup ErrorSeverities
	*/
	ErrNoError = 0,
	/**
		Display a warning.
		@ingroup ErrorSeverities
	*/
	ErrWarning,
	/**
		Display a message if the user has not suppresed errors.
		@ingroup ErrorSeverities
	*/
	ErrSuppressable,	
	/**
		Never display a message.
		@ingroup ErrorSeverities
	*/
	ErrSilent,			
	/**
		Always display a message, even if others are suppressed.
		@ingroup ErrorSeverities
	*/
	ErrAlways			
};

typedef ASEnum8 ErrSeverity;

/* Systems */
enum {
	/**
		General error and out of memory errors.
		@ingroup ErrorSystems
	*/
	ErrSysNone = 0,	
	/**
		CosStore, filters errors.
		@ingroup ErrorSystems
	*/
	ErrSysCos,		
	/**
		Cos syntax errors.
		@ingroup ErrorSystems
	*/
	ErrSysCosSyntax,
	/**
		PDDoc and family, Page tree, outlines errors.
		@ingroup ErrorSystems
	*/
	ErrSysPDDoc,	
	/**
		PDPage and family, thumbs, annotations errors.
		@ingroup ErrorSystems
	*/
	ErrSysPDPage,	
	/**
		Global PD errors.
		@ingroup ErrorSystems
	*/
	ErrSysPDModel,	
	/**
		AcroView errors.
		@ingroup ErrorSystems
	*/
	ErrSysAcroView,	
	/**
		Page parsing and RIPping errors.
		@ingroup ErrorSystems
	*/
	ErrSysPage,		
	/**
		Font Server errors.
		@ingroup ErrorSystems
	*/
	ErrSysFontSvr,	
	/**
		Rasterizer errors.
		@ingroup ErrorSystems
	*/
	ErrSysRaster,	
	/**
		ASFile I/O errors.
		@ingroup ErrorSystems
	*/
	ErrSysASFile,	
	/**
		Extension Manager errors.
		@ingroup ErrorSystems
	*/
	ErrSysXtnMgr,	
	/**
		<i>New</i> error codes added by extensions.
		@ingroup ErrorSystems
	*/
	ErrSysXtn,		
	/**
		Platform-specific system errors.
		@ingroup ErrorSystems
	*/
	ErrSysMDSystem,	
	/**
		Platform-specific application errors.
		@ingroup ErrorSystems
	*/
	ErrSysMDApp,	
	/**
		PDFX-specific errors.
		@ingroup ErrorSystems
	*/
	ErrSysPDFX,		
	/**
		PDFEdit errors.
		@ingroup ErrorSystems
	*/
	ErrSysPDFEdit,	
	/**
		PDSEdit (structure) errors.
		@ingroup ErrorSystems
	*/
	ErrSysPDSEdit,	
	/**
		XAP Metadata errors.
		@ingroup ErrorSystems
	*/
    ErrSysPDMetadata, 

	ErrSysLast		/* no more than 256 systems allowed */
};

typedef ASEnum8 ErrSystem;

/* Macros for building system specific codes.
** Use these macros with the errors defined here.
** For example: ASRaise(PDDocError(pdErrTooManyPagesForOpen));
*/

#define GenError(e)			ErrBuildCode(ErrAlways, ErrSysNone, e)
#define CosError(e)			ErrBuildCode(ErrSuppressable, ErrSysCos, e)
#define CosErrorAlways(e)	ErrBuildCode(ErrAlways, ErrSysCos, e)
#define CosSyntaxError(e)	ErrBuildCode(ErrSuppressable, ErrSysCosSyntax, e)
#define PDDocError(e)		ErrBuildCode(ErrSuppressable, ErrSysPDDoc, e)
#define PDDocErrorAlways(e)	ErrBuildCode(ErrAlways, ErrSysPDDoc, e)
#define PDPageError(e)		ErrBuildCode(ErrSuppressable, ErrSysPDPage, e)
#define PDPageErrorAlways(e) ErrBuildCode(ErrAlways, ErrSysPDPage, e)
#define PDPageErrorSilent(e) ErrBuildCode(ErrSilent, ErrSysPDPage, e)
#define PDModelError(e)		ErrBuildCode(ErrAlways, ErrSysPDModel, e)
#define AcroViewError(e)	ErrBuildCode(ErrAlways, ErrSysAcroView, e)
#define PageError(e)		ErrBuildCode(ErrSuppressable, ErrSysPage, e)
#define PageErrorSilent(e)	ErrBuildCode(ErrSilent, ErrSysPage, e)
#define FontSvrError(e)		ErrBuildCode(ErrAlways, ErrSysFontSvr, e)
#define RasterError(e)		ErrBuildCode(ErrAlways, ErrSysRaster, e)
#if !defined(WEBBUY_LIB)
#define ASFileError(e)		ErrBuildCode(ErrAlways, ErrSysASFile, e)
#endif
#define XtnMgrError(e)		ErrBuildCode(ErrAlways, ErrSysXtnMgr, e)
#define XtnError(e)			ErrBuildCode(ErrAlways, ErrSysXtn, e)
#define MDSysError(e)		ErrBuildCode(ErrAlways, ErrSysMDSystem, e)
#define MDAppError(e)		ErrBuildCode(ErrAlways, ErrSysMDApp, e)
#define PDFXError(e)		ErrBuildCode(ErrAlways, ErrSysPDFX, e)
#define PDFEditError(e)		ErrBuildCode(ErrAlways, ErrSysPDFEdit, e)
#define PDSEditError(e)		ErrBuildCode(ErrAlways, ErrSysPDSEdit, e)
#define PDMetadataError(e)	ErrBuildCode(ErrAlways, ErrSysPDMetadata, e)


/* General Errors */
enum
{
	/**
		No error.
		@ingroup GeneralErrors
	*/
	genErrNoError,              
	/**
		An internal error occurred.
		@ingroup GeneralErrors
	*/
	genErrGeneral,                
	/**
		Out of memory.
		@ingroup GeneralErrors
	*/
	genErrNoMemory,              
	/**
		Bad parameter.
		@ingroup GeneralErrors
	*/
	genErrBadParm,                
	/**
		Operation or data is too complex.
		@ingroup GeneralErrors
	*/
	genErrListOverflow,           
	/**
		Attempt to release an unlocked object.
		@ingroup GeneralErrors
	*/
	genErrBadUnlock,              
	/**
		Exception stack overflow.
		@ingroup GeneralErrors
	*/
	genErrExceptionStackOverflow, 
	/**
		Failed to load an application resource (internal error).
		@ingroup GeneralErrors
	*/
	genErrResourceLoadFailed,     
	/**
		Attempt to register an object with a name already in use.
		@ingroup GeneralErrors
	*/
	genErrNameAlreadyRegistered,  
	/**
		Attempt to call a method that has not been implemented.
		@ingroup GeneralErrors
	*/
	genErrMethodNotImplemented,   
	/**
		The user cancelled the operation.
		@ingroup GeneralErrors
	*/
	genErrCanceled,               
	/**
		No valid Acrobat serial number was found. Acrobat will now quit.
		@ingroup GeneralErrors
	*/
	genErrNoValidSerialNoFound,
	/**
		The parameter was out of Valid range.
		@ingroup GeneralErrors
	*/
	genErrOutOfRange
};

/* General Cos Errors */
enum
{
	/**
		No error.
		@ingroup GeneralCosErrors
	*/
	cosErrNoError,                
	/**
		Read error.
		@ingroup GeneralCosErrors
	*/
	cosErrReadError,              
	/**
		Write error.
		@ingroup GeneralCosErrors
	*/
	cosErrWriteError,             
	/**
		Syntax error.
		@ingroup GeneralCosErrors
	*/
	cosErrBadSyntax,              
	/**
		The file needs to be repaired.
		@ingroup GeneralCosErrors
	*/
	cosErrNeedRebuild,            
	/**
		Could not repair file.
		@ingroup GeneralCosErrors
	*/
	cosErrRebuildFailed,          
	/**
		A temporary file could not be opened.
		@ingroup GeneralCosErrors
	*/
	cosErrCantOpenTempFile,       
	/**
		The temporary file is full or nearly full. Close or save any modified documents.
		@ingroup GeneralCosErrors
	*/
	cosErrTempFileFull,           
	/**
		The stream source is shorter than specified length.
		@ingroup GeneralCosErrors
	*/
	cosErrStreamTooShort,         
	/**
		A stream specifies an unknown filter.
		@ingroup GeneralCosErrors
	*/
	cosErrBadFilterName,          
	/**
		The operation or data is too complex.
		@ingroup GeneralCosErrors
	*/
	cosErrListOverflow,          
	/**
		The Cos document table is full.
		@ingroup GeneralCosErrors
	*/
	cosErrDocTableFull,           
	/**
		A number is out of range.
		@ingroup GeneralCosErrors
	*/
	cosErrInt16OutOfRange,        
	/**
		A <code>NULL</code> object was expected.
		@ingroup GeneralCosErrors
	*/
	cosErrExpectedNull,           
	/**
		A dictionary object was expected.
		@ingroup GeneralCosErrors
	*/
	cosErrExpectedDict,           
	/**
		An array object was expected.
		@ingroup GeneralCosErrors
	*/
	cosErrExpectedArray,          
	/**
		A number object was expected.
		@ingroup GeneralCosErrors
	*/
	cosErrExpectedNumber,         
	/**
		A boolean object was expected.
		@ingroup GeneralCosErrors
	*/
	cosErrExpectedBoolean,        
	/**
		A name object was expected.
		@ingroup GeneralCosErrors
	*/
	cosErrExpectedName,           
	/**
		A string object was expected.
		@ingroup GeneralCosErrors
	*/
	cosErrExpectedString,         
	/**
		A stream object was expected.
		@ingroup GeneralCosErrors
	*/
	cosErrExpectedStream,         
	/**
		This direct object already has a container.
		@ingroup GeneralCosErrors
	*/
	cosErrInvalidAssignment,      
	/**
		Implementation failure: this document is now invalid.
		@ingroup GeneralCosErrors
	*/
	cosErrAfterSave,              
	/**
		The desired operation cannot be performed on this object.
		@ingroup GeneralCosErrors
	*/
	cosErrInvalidObj,             
	/**
		Array out-of-bounds error.
		@ingroup GeneralCosErrors
	*/
	cosErrArrayBounds,            
	/**
		The dictionary key must be a name object.
		@ingroup GeneralCosErrors
	*/
	cosErrDictKeyNotName,         
	/**
		This file must be saved with a full save.
		@ingroup GeneralCosErrors
	*/
	cosErrNeedFullSave,           
	/**
		Error in the encryption filter.
		@ingroup GeneralCosErrors
	*/
	cosErrEncryptionErr,         
	/**
		Error in the JPEG data filter.
		@ingroup GeneralCosErrors
	*/
	cosErrDCTError,               
	/**
		Error in the CCITT fax data filter.
		@ingroup GeneralCosErrors
	*/
	cosErrCCFError,               
	/**
		Error in the LZW data filter.
		@ingroup GeneralCosErrors
	*/
	cosErrLZWError,               
	/**
		A direct object was expected.
		@ingroup GeneralCosErrors
	*/
	cosErrExpectedDirect,         
	/**
		Obsolete linearized (optimized for the Web) format.
		@ingroup GeneralCosErrors
	*/
	cosErrOldLinFormat,           
	/**
		The temporary file was unexpectedly short.
		@ingroup GeneralCosErrors
	*/
	cosErrTempTooShort,			  
	/**
		The Save operation was cancelled.
		@ingroup GeneralCosErrors
	*/
	cosErrCancelSave,			  
	/**
		Encryption and decryption are not supported.
		@ingroup GeneralCosErrors
	*/
	cosErrEncryptionNotSupported, 
	/**
		An encryption key is not supplied for a stream.
		@ingroup GeneralCosErrors
	*/
	cosErrNoEncryptionKeySupplied,

	/* Defined for Acrobat 6.0 */
	/**
		A filter with the same name is already registered.
		@ingroup GeneralCosErrors
	*/
	cosErrDuplicateFilterName,    
	/**
		Attempted to decode without the associated filter.
		@ingroup GeneralCosErrors
	*/
	cosErrNoDecodeFilter,         
	/**
		Attempted to encode without the associated filter.
		@ingroup GeneralCosErrors
	*/
	cosErrNoEncodeFilter,         
	/**
		Decryption authorization failed during data access.
		@ingroup GeneralCosErrors
	*/
	cosErrCryptAuthFailed,        
	/**
		An object stream was expected.
		@ingroup GeneralCosErrors
	*/
	cosErrExpectedObjectStream,   
	/**
		An indirect object was expected.
		@ingroup GeneralCosErrors
	*/
	cosErrExpectedIndirect,       
	/**
		A procedure was expected.
		@ingroup GeneralCosErrors
	*/
	cosErrExpectedProc,           
	/**
		An object collection was expected.
		@ingroup GeneralCosErrors
	*/
	cosErrExpectedCollection,     
	/**
		A compressed object was expected.
		@ingroup GeneralCosErrors
	*/
	cosErrNotCompressed,          
	/**
		An object that was not compressed was expected.
		@ingroup GeneralCosErrors
	*/
	cosErrCompressed,             

	/* Skip four "message" codes. */
	/**
		An acquired object was already released.
		@ingroup GeneralCosErrors
	*/
	cosErrBadRefcount = 51,		 
	/**
		An object has been replaced or destroyed.
		@ingroup GeneralCosErrors
	*/
	cosErrObjFreed,				  
	/**
		Internal error in the memory manager. 
		@ingroup GeneralCosErrors
	*/
    cosErrMemMgrError,			  
	/**
		This file requires a cross-reference stream (PDF 1.5).
		@ingroup GeneralCosErrors
	*/
    cosErrNeedXrefStm,			 

	/* Defined for Acrobat 10.0 */
	/**
		General Encryption System error (failed to load or initialize).
		@ingroup GeneralCosErrors
	*/
    cosErrCryptSystem			 
};

/* Cos Syntax Errors */
enum
{
	/**
		No syntax error.
		@ingroup CosSyntaxErrors
	*/
	cosSynErrNoError,             
	/**
		File does not begin with <code>'%PDF-'</code>.
		@ingroup CosSyntaxErrors
	*/
	cosSynErrNoHeader,            
	/**
		Missing <code>%%EOF</code>.
		@ingroup CosSyntaxErrors
	*/
	cosSynErrNoEOF,               
	/**
		Could not find <code>startxref</code> address.
		@ingroup CosSyntaxErrors
	*/
	cosSynErrNoStartXRef,         
	/**
		The value of <code>startxref</code> address is not an integer. 
		@ingroup CosSyntaxErrors
	*/
	cosSynErrNoStartAddress,      
	/**
		Missing <code>'xref'</code>.
		@ingroup CosSyntaxErrors
	*/
	cosSynErrBadXref,             
	/**
		<code>Xref</code> header should be two integers.
		@ingroup CosSyntaxErrors
	*/
	cosSynErrBadXrefHeader,       
	/**
		Error reading <code>xref</code> entry.
		@ingroup CosSyntaxErrors
	*/
	cosSynErrBadXrefEntry,        
	/**
		The trailer dictionary start is missing <code>'<<'</code>. 
		@ingroup CosSyntaxErrors
	*/
	cosSynErrBadTrailerStart,     
	/**
		The object label is badly formatted.
		@ingroup CosSyntaxErrors
	*/
	cosSynErrBadObjectLabel,     
	/**
		Unrecognized object name.
		@ingroup CosSyntaxErrors
	*/
	cosSynErrUnknownName,         
	/**
		Unrecognized token type.
		@ingroup CosSyntaxErrors
	*/
	cosSynErrUnknownTokenType,    
	/**
		Missing endstream.
		@ingroup CosSyntaxErrors
	*/
	cosSynErrNoEndStream,         
	/**
		Unexpected endstream.
		@ingroup CosSyntaxErrors
	*/
	cosSynErrExtraEndStream,      
	/**
		Unterminated string.
		@ingroup CosSyntaxErrors
	*/
	cosSynErrUnterminatedString,  
	/**
		The string is too long.
		@ingroup CosSyntaxErrors
	*/
	cosSynErrStringTooLong,       
	/**
		The token is too long.
		@ingroup CosSyntaxErrors
	*/
	cosSynErrTokenTooLong,        
	/**
		There is a non-hex character in a hex string.
		@ingroup CosSyntaxErrors
	*/
	cosSynErrBadCharInHexString,  
	/**
		Unexpected token type.
		@ingroup CosSyntaxErrors
	*/
	cosSynErrUnexpectedType,      
	/**
		End of image was not found.
		@ingroup CosSyntaxErrors
	*/
	cosSynErrImageNeverEnded,     
	/**
		Unexpected end of dictionary.
		@ingroup CosSyntaxErrors
	*/
	cosSynErrUnexpectedDict,      
	/**
		Unexpected end of array.
		@ingroup CosSyntaxErrors
	*/
	cosSynErrUnexpectedArray,     
	/**
		There was an error reading the dictionary.
		@ingroup CosSyntaxErrors
	*/
	cosSynErrBadDict,             
	/**
		There was an error reading the object.
		@ingroup CosSyntaxErrors
	*/
	cosSynErrBadObject,           
	/**
		Expected a dictionary or array. 
		@ingroup CosSyntaxErrors
	*/
	cosSynErrBadArrayDict,        
	/**
		Bad foreign object reference.
		@ingroup CosSyntaxErrors
	*/
	cosSynErrBadFRef,             
	/**
		There was a parse stack underflow while reading the object.
		@ingroup CosSyntaxErrors
	*/
	cosSynErrPStackUnderflow,     
	/**
		There was an error reading the linearized hint data.
		@ingroup CosSyntaxErrors
	*/
	cosSynErrBadLinearized,       
	/**
		There was a non-hex character after <code>#</code> in a name.
		@ingroup CosSyntaxErrors
	*/
	cosSynErrBadHexCharInName,	  
	/**
		There are illegal characters in a name.
		@ingroup CosSyntaxErrors
	*/
	cosSynErrBadName,			  
	/**
		An object reference is invalid.
		@ingroup CosSyntaxErrors
	*/
	cosSynErrBadObjectRef,		  

	/* Defined for Acrobat 6.0 */
	/**
		There is an error in the <code>XRef</code> stream.
		@ingroup CosSyntaxErrors
	*/
	cosSynErrBadXrefStream,       
	/**
		Unexpected end of file.
		@ingroup CosSyntaxErrors
	*/
	cosSynErrPrematureEOF,	      
	/**
		Expected <code>CR</code> and/or <code>LF</code> after <code>'stream'</code>.
		@ingroup CosSyntaxErrors
	*/
	cosSynErrBadStreamStart,      
	/**
		There is an error in the object stream.
		@ingroup CosSyntaxErrors
	*/
	cosSynErrBadObjStream,	      
	/**
		Dictionary keys must be direct name objects.
		@ingroup CosSyntaxErrors
	*/
	cosSynErrDictKeyNotName,      
	/**
		The <code>NULL</code> object was expected.
		@ingroup CosSyntaxErrors
	*/
	cosSynErrExpectedNull,        
	/**
		A number was expected.
		@ingroup CosSyntaxErrors
	*/
	cosSynErrExpectedNumber,      
	/**
		An integer was expected.
		@ingroup CosSyntaxErrors
	*/
	cosSynErrExpectedInteger,     
	/**
		A real number was expected.
		@ingroup CosSyntaxErrors
	*/
	cosSynErrExpectedReal,        
	/**
		A non-negative integer was expected.
		@ingroup CosSyntaxErrors
	*/
	cosSynErrExpectedUnsigned,    
	/**
		<code>true</code> or <code>false</code> was expected.
		@ingroup CosSyntaxErrors
	*/
	cosSynErrExpectedBoolean,     
	/**
		A name integer was expected.
		@ingroup CosSyntaxErrors
	*/
	cosSynErrExpectedName,        
	/**
		A string was expected.
		@ingroup CosSyntaxErrors
	*/
	cosSynErrExpectedString,      
	/**
		A dictionary was expected.
		@ingroup CosSyntaxErrors
	*/
	cosSynErrExpectedDict,        
	/**
		An array was expected.
		@ingroup CosSyntaxErrors
	*/
	cosSynErrExpectedArray,       
	/**
		A stream was expected.
		@ingroup CosSyntaxErrors
	*/
	cosSynErrExpectedStream,      
	/**
		A stream was found in an illegal context.
		@ingroup CosSyntaxErrors
	*/
	cosSynErrIllegalStream,       
	/**
		The stream is missing a <code>Length</code> key.
		@ingroup CosSyntaxErrors
	*/
	cosSynErrNoLength,            
	/**
		An indirect object was expected.
		@ingroup CosSyntaxErrors
	*/
	cosSynErrExpectedIndirect,    
	/**
		A direct object was expected.
		@ingroup CosSyntaxErrors
	*/
	cosSynErrExpectedDirect,      
	/**
		Illegal indirect reference.
		@ingroup CosSyntaxErrors
	*/
	cosSynErrIllegalIndRef        
};

/* PDDoc Errors */
enum
{
	/**
		No error.
		@ingroup PDDocErrors
	*/
	pdErrNoError,                 
	/**
		Bad font object or font descriptor object.
		@ingroup PDDocErrors
	*/
	pdErrBadFont,                 
	/**
		An error occurred while trying to embed a font.
		@ingroup PDDocErrors
	*/
	pdErrEmbeddingFont,          
	/**
		The root object is missing or invalid.
		@ingroup PDDocErrors
	*/
	pdErrBadRootObj,              
	/**
		The base pages object is missing or invalid.
		@ingroup PDDocErrors
	*/
	pdErrBadBaseObj,              
	/**
		The outlines object is missing or invalid.
		@ingroup PDDocErrors
	*/
	pdErrBadOutlineObj,           
	/**
		There are invalid or corrupt font metrics in the resource file.
		@ingroup PDDocErrors
	*/
	pdErrBadResMetrics,           
	/**
		A page object is missing or invalid.
		@ingroup PDDocErrors
	*/
	pdErrBadPageObj,              
	/**
		An error occurred while processing a thumbnail.
		@ingroup PDDocErrors
	*/
	pdErrThumbError,              
	/**
		Invalid annotation object.
		@ingroup PDDocErrors
	*/
	pdErrBadAnnotation,           
	/**
		The document's page tree contains an invalid node.
		@ingroup PDDocErrors
	*/
	pdErrBadPageTree,             
	/**
		The information needed to print a page is unavailable.
		@ingroup PDDocErrors
	*/
	pdErrUnknownProcsets,         
	/**
		This file could not be opened.
		@ingroup PDDocErrors
	*/
	pdErrUnableToOpenDoc,         
	/**
		Unable to open the file for writing. It may be locked or unavailable.
		@ingroup PDDocErrors
	*/
	pdErrIsFileLocked,            
	/**
		Unable to write the file. 
		@ingroup PDDocErrors
	*/
	pdErrUnableToWrite,           
	/**
		Unable to rename the temporary file to the Save As name.
		@ingroup PDDocErrors
	*/
	pdErrUnableToRenameTemp,      
	/**
		Unable to recover the original file.
		@ingroup PDDocErrors
	*/
	pdErrUnableToRecover,         
	/**
		Unable to read the file.
		@ingroup PDDocErrors
	*/
	pdErrUnableToRead,            
	/**
		This is not a valid PDF document. It cannot be opened.
		@ingroup PDDocErrors
	*/
	pdErrUnknownFileType,         
	/**
		This file is already open.
		@ingroup PDDocErrors
	*/
	pdErrAlreadyOpen,             
	/**
		This file cannot be opened because it contains too many pages.
		@ingroup PDDocErrors
	*/
	pdErrTooManyPagesForOpen,     
	/**
		There is not enough temporary disk space for this operation.
		@ingroup PDDocErrors
	*/
	pdErrNotEnoughSpaceForTempFile, 
	/**
		Inserting this file would result in a document with too many pages.
		@ingroup PDDocErrors
	*/
	pdErrTooManyPagesForInsert,   
	/**
		There is an error in the bookmarks.
		@ingroup PDDocErrors
	*/
	pdErrBookmarksError,          
	/**
		Cannot open more bookmarks.
		@ingroup PDDocErrors
	*/
	pdErrCannotOpenMoreBkMark,    
	/**
		Unable to extract embedded font.
		@ingroup PDDocErrors
	*/
	pdErrUnableToExtractFontErr,  
	/**
		An error occurred while creating the document notes file. 
		@ingroup PDDocErrors
	*/
	pdErrCannotOpenNotes,        
	/**
		This document has no notes.
		@ingroup PDDocErrors
	*/
	pdErrNoNotes,                 
	/**
		The copy of a page failed.
		@ingroup PDDocErrors
	*/
	pdErrCopyPageFailed,          
	/**
		This file is damaged. 
		@ingroup PDDocErrors
	*/
	pdErrNeedRebuild,             
	/**
		The font contains bad /Flags.
		@ingroup PDDocErrors
	*/
	pdErrBadFontFlags,            
	/**
		The font contains a bad /BBox.
		@ingroup PDDocErrors
	*/
	pdErrBadFontBBox,            
	/**
		The font contains bad /Widths.
		@ingroup PDDocErrors
	*/
	pdErrBadFontWidths,           
	/*
		OBSOLETE.
	*/
	pdErrOldCosFileOBSOLETE,      
	/**
		This file can only be saved using Save As.
		@ingroup PDDocErrors
	*/
	pdErrTrySaveAs,              
	/**
		Creation of the notes file was cancelled. 
		@ingroup PDDocErrors
	*/
	pdErrAbortNotes,              
	/**
		One or more pages are in use and could not be deleted.
		@ingroup PDDocErrors
	*/
	pdErrPagesLockedNotDeleted,   
	/**
		There is not enough memory available to open the document.
		@ingroup PDDocErrors
	*/
	pdErrNotEnoughMemoryToOpenDoc, 
	/**
		Unable to close the document due to outstanding references.
		@ingroup PDDocErrors
	*/
	pdErrUnableToCloseDueToRefs,  
	/**
		This document requires a password. 
		@ingroup PDDocErrors
	*/
	pdErrNeedPassword,           
	/**
		This operation is not permitted.
		@ingroup PDDocErrors
	*/
	pdErrOpNotPermitted,         
	/**
		The security plug-in required by this command is unavailable.
		@ingroup PDDocErrors
	*/
	pdErrNoCryptHandler,          
	/**
		Invalid article object.
		@ingroup PDDocErrors
	*/
	pdErrBadThread,               
	/**
		Invalid article element.
		@ingroup PDDocErrors
	*/
	pdErrBadBead,                
	/**
		An error occurred while processing articles.
		@ingroup PDDocErrors
	*/
	pdErrThreadProcessing,        
	/**
		Unknown action type.
		@ingroup PDDocErrors
	*/
	pdErrUnknownAction,           
	/**
		Invalid action object.
		@ingroup PDDocErrors
	*/
	pdErrBadAction,               
	/**
		This file contains information not understood by the viewer. It cannot be used for this operation.
		@ingroup PDDocErrors
	*/
	pdErrCantUseNewVersion,      
	/**
		 This viewer cannot decrypt this document. 
		@ingroup PDDocErrors
	*/
	pdErrOldEncryption,          
	/**
		Unable to extract the embedded font. Some characters may not display or print correctly.
		@ingroup PDDocErrors
	*/
	pdErrUnableToExtractFont,    
	/**	
		Unable to find or create the font. Some characters may not display or print correctly.
		@ingroup PDDocErrors
	*/
	pdErrUnableToFindFont,        
	/**
		Invalid annotation color (only RGB colors are allowed).
		@ingroup PDDocErrors
	*/
	pdErrBadAnnotColor,           
	/**
		Cannot execute this command on an unsecured document. 
		@ingroup PDDocErrors
	*/
	pdErrNeedCryptHandler,        
	/**
		The font contains bad /FontDescriptor metrics.
		@ingroup PDDocErrors
	*/
	pdErrBadFontDescMetrics,      
	/**
		There was an error while inserting or extracting pages and another error while trying to recover.
		@ingroup PDDocErrors
	*/
	pdErrWhileRecoverInsertPages, 
	/**
		Invalid bookmark object.
		@ingroup PDDocErrors
	*/
	pdErrBadBookmark,             
	/**
		Invalid file specification object.
		@ingroup PDDocErrors
	*/
	pdErrBadFileSpec,             
	/**
		This document was successfully saved, but an error occurred after saving the document. Please close and reopen the document.
		@ingroup PDDocErrors
	*/
	pdErrAfterSave,               
	/**
		Some text in the font and character could not be displayed or printed correctly. The font could not be reencoded.
		@ingroup PDDocErrors
	*/
	pdErrUnableToXlateText,       

	/* Defined after Acrobat 2.1 */
	/**
		Not enough bytes are in the text string for the multibyte character code.
		@ingroup PDDocErrors
	*/
	pdErrTextStringTooShort,		
	/**
		A font contains a bad CMap /Encoding.
		@ingroup PDDocErrors
	*/
	pdErrBadCMap,					
	/**
		The font cannot be displayed with the installed version of Adobe Type Manager.
		@ingroup PDDocErrors
	*/
	pdErrOldATMVersion,				
	/**
		This file cannot be opened because it has no pages.
		@ingroup PDDocErrors
	*/
	pdErrZeroPageFile,				
	/**
		Adobe Type Manager is running out of memory. The text in the font may not render properly.
		@ingroup PDDocErrors
	*/
	pdErrATMMemory,					
	/**
		There is not enough memory to optimize this file.
		@ingroup PDDocErrors
	*/
	pdErrOptMemory,					
	/**
		The Save operation was cancelled.
		@ingroup PDDocErrors
	*/
	pdErrCancelSave, 				

	/* vvv previously missing vvv */
	/**
		These documents contain subset fonts that have the same name and cannot be merged.
		@ingroup PDDocErrors
	*/
	pdErrCannotMergeWithSubsetFonts, 
	/**
		This document was successfully saved, but an error occurred after saving the document. Please close and reopen the document.
		@ingroup PDDocErrors
	*/
	pdErrCannotReopenDoc,	
	/**
		 No PDDoc is associated with the CosDoc. 
		@ingroup PDDocErrors
	*/
	pdErrNoPDDocForCosDoc,	
	/**
		The application has not set the host encoding.
		@ingroup PDDocErrors
	*/
	pdErrHostEncodingNotSet,	
	/**
		An invalid font was removed from the document.
		@ingroup PDDocErrors
	*/
	pdErrInvalidEmbeddedFont,	

	/* Defined for and after Acrobat 5.0 */
	/**
		You cannot delete all pages.  At least one page must remain.
		@ingroup PDDocErrors
	*/
	pdErrCannotDeleteAllPages,       
	/**
		The starting page number must be less than or the same as the ending page number.
		@ingroup PDDocErrors
	*/
	pdErrStartLessThanEnd,          
	/**
		There is no page with such a number in this document.
		@ingroup PDDocErrors
	*/
	pdErrNotValidPage,               
	/**
		The page number cannot be left blank.
		@ingroup PDDocErrors
	*/
	pdErrCannotBeBlankPage,          
	/**
		Invalid page number.
		@ingroup PDDocErrors
	*/
	pdErrInvalidPageNumber,         
	/**
		Exceeds support encryption key length.
		@ingroup PDDocErrors
	*/
	pdErrExceedEncryptionLength,     
	/**
		This version of encryption is not supported.
		@ingroup PDDocErrors
	*/
	pdErrExceedEncryptionVersion,    
	/**
		Only Adobe certified Acrobat plug-ins are allowed while viewing this document.
		@ingroup PDDocErrors
	*/
	pdErrRequireTrustedMode,         
	/**
		Unable to find a substitution font with all the characters used by the font. Some characters may not be displayed or printed.
		@ingroup PDDocErrors
	*/
	pdErrMissingGlyphs,              
	/**
		An error has occurred that may be fixed by installing the latest version of the Traditional Chinese Language Support package.
		@ingroup PDDocErrors
	*/
	pdErrNeedTradChinese,            
	/**
		An error has occurred that may be fixed by installing the latest version of the Traditional Chinese Language Support package.
		@ingroup PDDocErrors
	*/
	pdErrNeedSimpChinese,            
	/**
		An error has occurred that may be fixed by installing the latest version of the Korean Language Support package.
		@ingroup PDDocErrors
	*/
	pdErrNeedKorean,                 
	/**
		An error has occurred that may be fixed by installing the latest version of the Japanese Language Support package.
		@ingroup PDDocErrors
	*/
	pdErrNeedJapanese,               
	/**
		A font required for font substitution is missing. 
		@ingroup PDDocErrors
	*/
	pdErrMissingSubsetFont,          
	/**
		The encoding (CMap) specified by a font is missing.
		@ingroup PDDocErrors
	*/
	pdErrCMapNotFound,               
	/**
		The implementation limit was exceeded.
		@ingroup PDDocErrors
	*/
	pdErrLimitcheck = 92,           
	/**
		There is insufficient disk space for the print job spool file to hold the entire print job. Try freeing up disk space on the startup volume and print the remaining pages of the document again.
		@ingroup PDDocErrors
	*/
	pdErrPrintAsImageSpoolFileFull, 
	/**
		Invalid MediaBox.
		@ingroup PDDocErrors
	*/
	pdErrInvalidMediaBox,            

	/* Defined for Acrobat 6.0 */
	/**
		The enumeration process was cancelled by the callback function.
		@ingroup PDDocErrors
	*/
	pdEnumCanceled = 97,             
	/**
		Could not embed the font.
		@ingroup PDDocErrors
	*/
	pdErrFontEmbeddingFailed,        
	/**
		Font embedding was cancelled.
		@ingroup PDDocErrors
	*/
	pdErrFontEmbeddingCanceled = 101,
	/**
		The operation could not be performed because the objects belong to different documents.
		@ingroup PDDocErrors
	*/
	pdErrMultipleDocuments,          
	/**
		Invalid type or value in an Optional Content object.
		@ingroup PDDocErrors
	*/
	pdErrBadOCObject,                
	/**
		The operation could not be performed because the image is inline. 
		@ingroup PDDocErrors
	*/
	pdErrNoInlineImage,              
	/**
		The required Crypt Filter is not registered by the security handler plug-in.
		@ingroup PDDocErrors
	*/
	pdErrNoCryptFilterHandler,       
	/**
		Bad encryption dictionary.
		@ingroup PDDocErrors
	*/
	pdErrBadEncryptDict,             
	/**
		The permission handler required by this command is unavailable.
		@ingroup PDDocErrors
	*/
	pdErrNoPermHandler = 108,        
	/**
		The permission handler has already been added for this document.
		@ingroup PDDocErrors
	*/
	pdErrDuplicatePermHandler,       
	/**
		The maximum number of permission handlers has been reached for this document.
		@ingroup PDDocErrors
	*/
	pdErrExceedMaxPermHandlers,      
	/**
		The font contains an invalid encoding. Some characters may not display.
		@ingroup PDDocErrors
	*/
	pdErrBadEncoding = 112,          
	/**
		The implementation limit was exceeded. Try decreasing the magnification level.
		@ingroup PDDocErrors
	*/
	pdErrMatrixTooBig,               

	/* Defined for Acrobat 7.0 */
	/**
		An error has occurred that may be fixed by installing the latest version of the Extended Language Support package.
		@ingroup PDDocErrors
	*/
	pdErrNeedExtendedLang = 120,     
	/**
		This file is encrypted with an unsupported cryptograhpic algorithm. A later version of Acrobat may be needed in order to open this document.
		@ingroup PDDocErrors
	*/
	pdErrUnknownCryptFilter,         
	/**
		An error has occurred that may be fixed by installing the latest version of the 3D Support package.
		@ingroup PDDocErrors
	*/
	pdErrNeed3D                      
};

/* PDPage Errors */
enum
{
	/**
		No error.
		@ingroup PDPageErrors
	*/
	pdPErrNoError,                 
	/**
		The dimensions of this page are out of range.
		@ingroup PDPageErrors
	*/
	pdPErrPageDimOutOfRange,       
	/**
		Invalid Type 3 font.
		@ingroup PDPageErrors
	*/
	pdPErrBadType3Font,            
	/**
		The Type 3 font is too complex to print.
		@ingroup PDPageErrors
	*/
	pdPErrType3TooComplex,         
	/**
		The form is too complex to print. 
		@ingroup PDPageErrors
	*/
	pdPErrFormTooComplex,         
	/**
		The creation of a raster port failed.
		@ingroup PDPageErrors
	*/
	pdPErrUnableToCreateRasterPort 
};

/* Other PDModel errors */
enum
{
	/**
		No error.
		@ingroup OtherPDModelErrors
	*/
	pdModErrNoError,              
	/**
		Unable to initialize the font encoding tables.
		@ingroup OtherPDModelErrors
	*/
	pdModErrEncTablesFailed,      
	/**
		A security plug-in with this name is already registered.
		@ingroup OtherPDModelErrors
	*/
	pdModErrDuplicateCryptName,   
	/**
		A permission handler with this name is already registered.
		@ingroup OtherPDModelErrors
	*/
	pdModErrDuplicatePermName,
	/**
		This navigator file cannot be read.  Ensure that the mimetype file exists and is the first file in the package.
		@ingroup OtherPDModelErrors
	*/
	pdModErrBadNavigator,
	/**
		This navigator requires a new version of Acrobat.
		@ingroup OtherPDModelErrors
	*/
	pdModErrNewNavigator,
	/**
		This navigator does not have a valid file extension. The only valid file extension for a navigator is either \"nav\" or \"swf\".
		@ingroup OtherPDModelErrors
	*/
	pdModErrNavUnknownExtension,
	/**
		This navigator has an invalid mimetype file. The navigator mimetype must be \"application/vnd.adobe.pdf-navigator\".
		@ingroup OtherPDModelErrors
	*/
	pdModErrNavInvalidMimeType,
	/**
		This navigator is missing its swf file. Check the spelling of the file name (it is case sensitive).
		@ingroup OtherPDModelErrors
	*/
	pdModErrNavMissingSwf,
	/**
		This navigator is missing its navigator xml file.
		@ingroup OtherPDModelErrors
	*/
	pdModErrNavMissingNavigatorXML,
	/**
		This navigator is missing its locales xml file. Check the spelling of the file name (it is case sensitive).
		@ingroup OtherPDModelErrors
	*/
	pdModErrNavMissingLocalesXML,
	/**
		This navigator is missing one or more of its localization xml files. Check the spelling of the file name (it is case sensitive).
		@ingroup OtherPDModelErrors
	*/
	pdModErrNavMissingStringsXML,
	/**
		This navigator's navigator xml file is malformed. Verify that your xml file is valid per the navigator schema.
		@ingroup OtherPDModelErrors
	*/
	pdModErrNavInvalidNavigatorXML,
	/**
		This navigator's locales xml file is malformed. Verify that your xml file is valid per the navigator schema.
		@ingroup OtherPDModelErrors
	*/
	pdModErrNavInvalidLocalesXML,
	/**
		One or more of this navigator's localization xml files is malformed. Verify that your xml file is valid per the navigator schema.
		@ingroup OtherPDModelErrors
	*/
	pdModErrNavInvalidStringsXML
};

/* AcroView errors */
enum
{
	/**
		No error.
		@ingroup AcroViewErrors
	*/
	avErrNoError,                
	/**
		No more than ten documents can be opened at a time. 
		@ingroup AcroViewErrors
	*/
	avErrCantOpenMoreThanTenDocs, 
	/**
		There are too many pages to print.
		@ingroup AcroViewErrors
	*/
	avErrPrintJobTooBig,          
	/**
		There is too much text to display. No more than 32,000 characters can be displayed.
		@ingroup AcroViewErrors
	*/
	avErrTooManyChars,           
	/**
		There is no text.
		@ingroup AcroViewErrors
	*/
	avErrNoText,                  

	/* previously missing */
	/**
		Acrobat cannot open this file. There is a modal dialog box open.
		@ingroup AcroViewErrors
	*/
	avErrCantOpenDialog,		
	/**
		This action cannot be performed from within an external window.
		@ingroup AcroViewErrors
	*/
	avErrActionExternal,		
	/**
		This action cannot be performed during full screen mode.
		@ingroup AcroViewErrors
	*/
	avErrActionFullScreen,		
	/**
		This action cannot be performed.
		@ingroup AcroViewErrors
	*/
	avErrActionRestricted,		
	/**
		Acrobat cannot open this file while printing another document.
		@ingroup AcroViewErrors
	*/
	avErrCantOpenPrinting,		

	/* new for 3.1 */
	/**
		Unregistered copy proc for the annotation object.
		@ingroup AcroViewErrors
	*/
	avErrBadAnnotationCopy,    
	/**
		Unregistered copy proc for the action object.
		@ingroup AcroViewErrors
	*/
	avErrBadActionCopy,         
	/**
		Unable to determine if a new update for this product exists.
		@ingroup AcroViewErrors
	*/
	avErrUpdateInternalError,   
	/**
		Unable to access the Internet.
		@ingroup AcroViewErrors
	*/
	avErrUpdateInternetError,   
	/**
		Web services are currently not available.
		@ingroup AcroViewErrors
	*/
	avErrUpdateNoWebServices,   
	/**
		Unable to read the article because it is damaged or missing.
		@ingroup AcroViewErrors
	*/
	avErrBadThreadLinkError,    
	/**
		Invalid page range.
		@ingroup AcroViewErrors
	*/
	avInvalidPageRange,         

	/* Defined for Acrobat 6.0 */
	/**
		Unable to download the Adobe Reader Help file.
		@ingroup AcroViewErrors
	*/
	avErrDownloadHelpError,     
	/**
		Insufficient permission for this operation.
		@ingroup AcroViewErrors
	*/
	avSAInsufficientPermission, 
	/**
		The operation failed due to the presence of 128-bit encryption in the PDF file.
		@ingroup AcroViewErrors
	*/
	avSA128EncryptionPresent    
};

/* Page Contents errors */
enum
{
	/**
		No error.
		@ingroup PageContentsErrors
	*/
	pageErrNoError,               
	/**
		Too few operands.
		@ingroup PageContentsErrors
	*/
	pageErrTooFewOps,             
	/**
		Wrong operand type.
		@ingroup PageContentsErrors
	*/
	pageErrWrongOpType,          
	/**
		The operand is too large.
		@ingroup PageContentsErrors
	*/
	pageErrOpTooLarge,            
	/**
		The page contents object has the wrong type.
		@ingroup PageContentsErrors
	*/
	pageErrBadContents,          
	/**
		Expected a number while parsing an image.
		@ingroup PageContentsErrors
	*/
	pageErrImageExpectedNumber,   
	/**
		Expected the end of color space.
		@ingroup PageContentsErrors
	*/
	pageErrExpectedEndOfColor,    
	/**
		Expected an <code>AsciiHex</code> or <code>Ascii85</code> string. 
		@ingroup PageContentsErrors
	*/
	pageErrExpectedHexOrASC85,    
	/**
		There was an error while trying to parse an image.
		@ingroup PageContentsErrors
	*/
	pageErrErrorParsingImage,     
	/**
		Bad object type within a text operator array. 
		@ingroup PageContentsErrors
	*/
	pageErrBadTypeInXTextArray,   
	/**
		An unexpected operator was found in the display list. 
		@ingroup PageContentsErrors
	*/
	pageErrUnexpectedOpInDisplay, 
	/**
		Invalid restore.
		@ingroup PageContentsErrors
	*/
	pageErrInvalidGRestore,       
	/**
		The font has not been set.
		@ingroup PageContentsErrors
	*/
	pageErrFontNotSet,            
	/**
		There are too few operands in the path.
		@ingroup PageContentsErrors
	*/
	pageErrTooFewPathOps,         
	/**
		The image in the form or Type 3 font is too big.
		@ingroup PageContentsErrors
	*/
	pageErrImageTooBig,           
	/**
		An error occurred while parsing a form or Type 3 font.
		@ingroup PageContentsErrors
	*/
	pageErrParseContextError,     
	/**
		Invalid Type 3 font.
		@ingroup PageContentsErrors
	*/
	pageErrBadType3Font,          
	/**
		A font is not in the Resources dictionary.
		@ingroup PageContentsErrors
	*/
	pageErrFontNotInResources,    
	/**
		Dash arguments are invalid. 
		@ingroup PageContentsErrors
	*/
	pageErrInvalidDash,           
	/**
		The array length is out of range.
		@ingroup PageContentsErrors
	*/
	pageErrArrayLenWrong,         
	/**
		A number value is out of range.
		@ingroup PageContentsErrors
	*/
	pageErrNumberOutOfRange,      
	/**
		A color value is out of range.
		@ingroup PageContentsErrors
	*/
	pageErrColorOutOfRange,       
	/**
		There is an illegal operator inside a text outline object.
		@ingroup PageContentsErrors
	*/
	pageErrIllegalOpInTextOutline, 
	/**
		A curve operator has the wrong number of operands.
		@ingroup PageContentsErrors
	*/
	pageErrWrongNumOpsInCurve,    
	/**
		There were several parsing errors on this page.
		@ingroup PageContentsErrors
	*/
	pageErrSeveralParsingErrors,  
	/**
		Wrong operand type. Another type was expected.
		@ingroup PageContentsErrors
	*/
	pageErrWrongOperand,          
	/**
		Could not find a font in the Resources dictionary; using Helvetica instead.
		@ingroup PageContentsErrors
	*/
	pageErrFontNotInResDict,     
	/**
		Could not find the XObject.
		@ingroup PageContentsErrors
	*/
	pageErrXObjectNotFound,       
	/**
		Could not find the form.
		@ingroup PageContentsErrors
	*/
	pageErrFormNotFound,          
	/**
		Unknown XObject type.
		@ingroup PageContentsErrors
	*/
	pageErrUnknownXObjectType,   
	/**
		Less image data was read than expected.
		@ingroup PageContentsErrors
	*/
	pageErrReadLessImageData,     
	/**
		An unrecognized token was found.
		@ingroup PageContentsErrors
	*/
	pageErrUnrecognizedToken,     
	/**
		The token type was not recognized.
		@ingroup PageContentsErrors
	*/
	pageErrTokenTypeNotRec,       
	/**
		There were too few arguments.
		@ingroup PageContentsErrors
	*/
	pageErrTooFewArgs,            
	/**
		There were too many arguments.
		@ingroup PageContentsErrors
	*/
	pageErrTooManyArgs,           
	/**
		An operand is too large.
		@ingroup PageContentsErrors
	*/
	pageErrOperandTooLarge,       
	/**
		There was an error reading the page near the contents.
		@ingroup PageContentsErrors
	*/
	pageErrErrorReadingPage,      
	/**
		Expected <code>'EI'</code> while parsing an image.
		@ingroup PageContentsErrors
	*/
	pageErrImageExpectedEI,       
	/**
		Unknown filter name.
		@ingroup PageContentsErrors
	*/
	pageErrUnknownFilterName,     
	/**
		Bad decode array.
		@ingroup PageContentsErrors
	*/
	pageErrBadDecodeArray,        
	/**
		Illegal operation inside a path. 
		@ingroup PageContentsErrors
	*/
	pageErrIllegalOpInPath,      
	/**
		Illegal operation inside a text object.
		@ingroup PageContentsErrors
	*/
	pageErrIllegalOpInTextObj,    
	/**
		Less image color data was read than expected.
		@ingroup PageContentsErrors
	*/
	pageErrReadLessImageColor,   
	/**
		The wrong number of arguments were used for a <code>setcolor</code> operator.
		@ingroup PageContentsErrors
	*/
	pageErrWrongArgsForSetColor,  
	/**
		Unknown ColorSpace.
		@ingroup PageContentsErrors
	*/
	pageErrUnknownColorSpace,    
	/**
		Could not find the ColorSpace.
		@ingroup PageContentsErrors
	*/
	pageErrColorSpaceNotFound,  
	/**
		Invalid form.
		@ingroup PageContentsErrors
	*/
	pageErrBadForm,               
	/**
		Illegal operation outside the text object.
		@ingroup PageContentsErrors
	*/
	pageErrIllegalTextOp,        
	/**
		The form type is not supported.
		@ingroup PageContentsErrors
	*/
	pageErrFormTypeNotAvailable,  
	/*
		OBSOLETE.
	*/
	pageErrOBSOLETE,             
	/**
		Internal error: the machine was called recursively.
		@ingroup PageContentsErrors
	*/
	pageErrRecursiveMachine,      

	/**
		An image is specified as an image mask with more than 1 bit per pixel.
		@ingroup PageContentsErrors
	*/
	pageErrInvalidImageMaskDepth,	

	/* Defined after Acrobat 2.1 */
	/**
		Invalid Pattern.
		@ingroup PageContentsErrors
	*/
	pageErrBadPattern,			
	/**
		The Pattern type is not supported.
		@ingroup PageContentsErrors
	*/
	pageErrPatternTypeNotAvailable,	
	/**
		Could not find the Pattern.
		@ingroup PageContentsErrors
	*/
	pageErrPatternNotFound,	
	/**
		Invalid color space.
		@ingroup PageContentsErrors
	*/
	pageErrBadColorSpace,		
	/**
		A resource is missing.
		@ingroup PageContentsErrors
	*/
	pageErrMissingResource,		
	/**
		The dictionary is missing the key.
		@ingroup PageContentsErrors
	*/
	pageErrMissingKey,		
	/**
		Could not find the Extended Graphics State.
		@ingroup PageContentsErrors
	*/
	pageErrEGStateNotFound,	
	/**
		Invalid Extended Graphics State. 
		@ingroup PageContentsErrors
	*/
	pageErrBadEGS,			
	/**
		Invalid Function resource.
		@ingroup PageContentsErrors
	*/
	pageErrBadFunction,			

	/* Defined after Acrobat 3.0 */
	/**
		An image uses a color space which will not separate correctly in some applications.
		@ingroup PageContentsErrors
	*/
	pageErrBadEPSColorSpace,	
	/**
		There is an error in the Shading dictionary.
		@ingroup PageContentsErrors
	*/
	pageErrBadShading,			
	/**
		There is an error in the Masked Image.
		@ingroup PageContentsErrors
	*/
	pageErrBadMaskImage,		
	/**
		There were too many color components.
		@ingroup PageContentsErrors
	*/
	pageErrTooManyComps,	
	/**
		A feature requires PostScript 3.
		@ingroup PageContentsErrors
	*/
	pageErrNotLevel3,			
	/**
		Invalid alternate image for the XObject.
		@ingroup PageContentsErrors
	*/
	pageErrBadAltXObject,	

	/* Defined after Acrobat 5.0 */
	/**
		Invalid transparency group.
		@ingroup PageContentsErrors
	*/
	pageErrBadTGroup,      
	/**
		Invalid soft mask.
		@ingroup PageContentsErrors
	*/
	pageErrBadSoftMask,      
	/**
		Invalid halftone.
		@ingroup PageContentsErrors
	*/
	pageErrBadHalftone,    
	/**
		A color operator was used where it is not permitted.
		@ingroup PageContentsErrors
	*/
	pageErrIllegalColorOp,
	/**
		Reference XObject target not found
		@ingroup PageContentsErrors
	*/
	pageErrRefXObjTargetNotFound,
	/**
		Invalid page number for target document
		@ingroup PageContentsErrors
	*/
	pageErrRefXObjPageNum,
	/**
		Missing or Invalid ID in Reference Dictionary
		@ingroup PageContentsErrors
	*/
	pageErrRefXObjMissingInvalidID,
	/**
		Xobject ID does not match with Document ID
		@ingroup PageContentsErrors
	*/
	pageErrRefXObjUnmatchedID,
	/**
		Missing or invalid metadata in X-5 compliant file
		@ingroup PageContentsErrors
	*/
	pageErrRefXObjMissingInvalidMetadata,
	/**
		Reference XObject points to itself
		@ingroup PageContentsErrors
	*/
	pageErrRefXObjRecursive
};

/* Font Server Errors */
enum
{
	/**
		No error.
		@ingroup FontServerErrors
	*/
	fsErrNoError,                
	/**
		Initialization of the font server module failed.
		@ingroup FontServerErrors
	*/
	fsErrInitFailed,          
	/**
		No Multiple Master fonts were found.
		@ingroup FontServerErrors
	*/
	fsErrNoMMFonts,            
	/**
		Adobe Type Manager was not found.
		@ingroup FontServerErrors
	*/
	fsErrNoATM,                   
	/**
		A new version of Adobe Type Manager is required.
		@ingroup FontServerErrors
	*/
	fsErrNeedNewATM,           
	/**
		The Type 1 font <i>'ZapfDingbats'</i> must be installed.
		@ingroup FontServerErrors
	*/
	fsErrNoT1ZapfDingbats,       
	/**
		The font download failed.
		@ingroup FontServerErrors
	*/
	fsErrDownloadFailed, 		 
	/**
		The font download aborted.
		@ingroup FontServerErrors
	*/
	fsErrDownloadAborted,		
	/**
		A bad parameter was passed to the font server. 
		@ingroup FontServerErrors
	*/
	fsErrBadParameter, 			  
	/**
		No <i>ZapfDingbats</i> or Multiple Master fonts were found.
		@ingroup FontServerErrors
	*/
	fsErrMissingFont 			
};

/* Rasterizer errors */
enum
{
	/**
		No error.
		@ingroup RasterizerErrors
	*/
	rasErrNoError,          
	/**
		Initialization of the rasterizer module failed. 
		@ingroup RasterizerErrors
	*/
	rasErrInitFailed,          
	/**
		Creation of the rasterizer port failed.
		@ingroup RasterizerErrors
	*/
	rasErrCreatePort,           
	/**
		A rasterizer error occurred. 
		@ingroup RasterizerErrors
	*/
	rasErrDraw               
};

/* ASFile errors */
enum
{
	/**
		No error.
		@ingroup ASFileErrors
	*/
	fileErrNoErr,        
	/**
		A file error has occurred.
		@ingroup ASFileErrors
	*/
	fileErrGeneral,            
	/*
		OBSOLETE.
	*/
	fileErrObsolete,          
	/**
		The directory is full.
		@ingroup ASFileErrors
	*/
	fileErrDirFull,           
	/**
		The document's disk or the disk used for temporary files is full.
		@ingroup ASFileErrors
	*/
	fileErrDiskFull,            
	/**
		The disk containing this file is not available.
		@ingroup ASFileErrors
	*/
	fileErrNSV,                
	/**
		A file I/O error has occurred.
		@ingroup ASFileErrors
	*/
	fileErrIO,  
	/**
		A file read error has occurred. 
		@ingroup ASFileErrors
	*/
	fileErrRead,             
	/**
		A file write error has occurred.
		@ingroup ASFileErrors
	*/
	fileErrWrite,               
	/**
		The end of file was reached unexpectedly.
		@ingroup ASFileErrors
	*/
	fileErrEOF,                 
	/**
		This file is locked.
		@ingroup ASFileErrors
	*/
	fileErrLocked,              
	/**
		This disk is locked and cannot be written to. 
		@ingroup ASFileErrors
	*/
	fileErrVLocked,             
	/**
		This file is busy and cannot be deleted.
		@ingroup ASFileErrors
	*/
	fileErrBusy,             
	/**
		Another file already exists under the same name.
		@ingroup ASFileErrors
	*/
	fileErrExists,            
	/**
		This file is already open or in use by another application.
		@ingroup ASFileErrors
	*/
	fileErrAlreadyOpen,         
	/**
		You do not have access to this file.
		@ingroup ASFileErrors
	*/
	fileErrPerm,                
	/**
		You do not have permission to write to this file.
		@ingroup ASFileErrors
	*/
	fileErrWrPerm,                
	/**
		This file cannot be found.
		@ingroup ASFileErrors
	*/
	fileErrFNF,                 
	/**
		File open failed.
		@ingroup ASFileErrors
	*/
	fileErrOpenFailed,         

	/* The following are new in 2.2 */
	/**
		The bytes are not ready.
		@ingroup ASFileErrors
	*/
	fileErrBytesNotReady,		 
	/**
		The user requested a stop.
		@ingroup ASFileErrors
	*/
	fileErrUserRequestedStop, 	 
	/**
		A file I/O error has occurred. The file connection timed out.
		@ingroup ASFileErrors
	*/
	fileErrIOTimeout, 			
	/**
		A file I/O error has occurred. The file is blocked while reading.
		@ingroup ASFileErrors
	*/
	fileErrReadBlocked,			
	/**
		This operation can only be performed on a folder. 
		@ingroup ASFileErrors
	*/
	fileErrNotADir,             
	/**
		A uniquely named temporary file could not be created. Please restart the application and try again.
		@ingroup ASFileErrors
	*/
	fileErrTempCreate,        

	/* Acrobat 7.0 */
	/**
		This file is too big for the current operation.
		@ingroup ASFileErrors
	*/
	fileErrTooBig			
};

/* Extension-Manager errors */
enum
{
	/**
		No error.
		@ingroup ExtensionManagerErrors
	*/
	xmErrNoError,           
	/**
		The plug-in was compiled with an out-of-date HFT.
		@ingroup ExtensionManagerErrors
	*/
	xmErrOutOfDateHFT,           
	/**
		The plug-in lacks a <code>PLUG</code> resource of <code>ID 1</code>.
		@ingroup ExtensionManagerErrors
	*/
	xmErrNoPLUGResource,     
	/**
		The plug-in is incompatible with this version of the viewer.
		@ingroup ExtensionManagerErrors
	*/
	xmErrPluginIncompatible,   
	/**
		The plug-in failed to initialize.
		@ingroup ExtensionManagerErrors
	*/
	xmErrInitializationFailed,  
	/**
		Two plug-ins are attempting to register with the same name.
		@ingroup ExtensionManagerErrors
	*/
	xmErrDuplicatePluginName,   
	/**
		There was an attempt to replace an unreplaceable selector. 
		@ingroup ExtensionManagerErrors
	*/
	xmErrCannotReplaceSelector,  
	/**
		An unimplemented function was called.
		@ingroup ExtensionManagerErrors
	*/
	xmErrCalledObsoleteProc,     
	/**
		The plug-in failed to load.
		@ingroup ExtensionManagerErrors
	*/
	xmErrPluginLoadFailed,      
	/**
		Adobe Reader cannot load this plug-in.
		@ingroup ExtensionManagerErrors
	*/
	xmErrNotPrivileged,          
	/**
		Only the 68K Viewer can load this plug-in.
		@ingroup ExtensionManagerErrors
	*/
	xmErr68KOnly,                
	/**
		Only the PowerPC Viewer can load this plug-in.
		@ingroup ExtensionManagerErrors
	*/
	xmErrPPCOnly,               
	/**
		This is not a trusted plug-in.
		@ingroup ExtensionManagerErrors
	*/
	xmErrPlugInNotTrusted,      
	/**
		This plugin cannot load on Mac OS X.
		@ingroup ExtensionManagerErrors
	*/
	xmErrPlugInNotCarbonized,    
	/**
		The currently selected language resources are not supported by this plugin.
		@ingroup ExtensionManagerErrors
	*/
	xmErrPluginResourceMismatch	  
};

/* PDFX specific errors */
enum
{
	/**
		No error.
		@ingroup PDFXSpecificErrors
	*/
	pdfxErrNoError,            
	/**
		The size of the passed callbacks <code>struct</code> is wrong (version).
		@ingroup PDFXSpecificErrors
	*/
	pdfxErrWrongCallbacks,		
	/**
		There was an attempt to call a <code>PDFXInstance</code> function during a callback proc.
		@ingroup PDFXSpecificErrors
	*/
	pdfxErrDuringCallback,			
	/**
		PDFX could not launch Acrobat.
		@ingroup PDFXSpecificErrors
	*/
	pdfxErrCannotLaunchAcrobat,	
	/**
		Could not find the External Window Handler plug-in.
		@ingroup PDFXSpecificErrors
	*/
	pdfxErrCannotFindEWH		
};

/* PDFEdit specific errors */
enum
{
	/**
		No error.
		@ingroup PDFEditSpecificErrors
	*/
	peErrNoError,				
	/**
		Unknown PDEColorSpace value.
		@ingroup PDFEditSpecificErrors
	*/
	peErrUnknownPDEColorSpace,	
	/**
		Incorrect PDEObject type.
		@ingroup PDFEditSpecificErrors
	*/
	peErrWrongPDEObjectType,		
	/**
		Unknown PDEObject resource type.
		@ingroup PDFEditSpecificErrors
	*/
	peErrUnknownResType,			
	/**
		PDFEdit parse stack underflow while reading object.
		@ingroup PDFEditSpecificErrors
	*/
	peErrPStackUnderflow,			
	/**
		Unable to create embedded font subset.
		@ingroup PDFEditSpecificErrors
	*/
	peErrCantCreateFontSubset,		
	/**
		Bad block header for Type 1 embedded stream.
		@ingroup PDFEditSpecificErrors
	*/
	peErrBadBlockHeader, 			
	/**
		Unable to get attributes for the font.
		@ingroup PDFEditSpecificErrors
	*/
	peErrCantGetAttrs,				
	/**
		Unable to get widths for the font.
		@ingroup PDFEditSpecificErrors
	*/
	peErrCantGetWidths,				
	/**
		Unable to find the font to embed on this system.
		@ingroup PDFEditSpecificErrors
	*/
	peErrFontToEmbedNotOnSys,		
	/**
		This font is licensed and cannot be embedded.
		@ingroup PDFEditSpecificErrors
	*/
	peErrCantEmbedFont,			
	/**
		Unable to get the image dictionary.
		@ingroup PDFEditSpecificErrors
	*/
	peErrCantGetImageDict,			
	/**
		Unable to read the image data.
		@ingroup PDFEditSpecificErrors
	*/
	peErrCantReadImage,				
	/**
		Unable to get the shading resource.
		@ingroup PDFEditSpecificErrors
	*/
	peErrCantGetShading,			
	/**
		Wrong operand type.
		@ingroup PDFEditSpecificErrors
	*/
	peErrWrongOpType,				
	/**
		There were too few operands in the path.
		@ingroup PDFEditSpecificErrors
	*/
	peErrTooFewPathOps,				
	/**
		There was an error while trying to parse an image.
		@ingroup PDFEditSpecificErrors
	*/
	peErrErrorParsingImage,			
	/**
		Less image color data was read than expected.
		@ingroup PDFEditSpecificErrors
	*/
	peErrReadLessImageColor,		
	/**
		Less image data was read than expected.
		@ingroup PDFEditSpecificErrors
	*/
	peErrReadLessImageData,			
	/**
		There are invalid or corrupt font metrics in the resource file.
		@ingroup PDFEditSpecificErrors
	*/
	peErrBadResMetrics,				
	/**
		Invalid Type 3 font.
		@ingroup PDFEditSpecificErrors
	*/
	peErrBadType3Font			
};

/* PDSEdit (structure) specific errors */
enum
{
	/**
		A required field was missing from a dictionary.
		@ingroup PDSEditSpecificErrors
	*/
	pdsErrRequiredMissing,		
	/**
		An incorrect structure was found in the PDF file.
		@ingroup PDSEditSpecificErrors
	*/
	pdsErrBadPDF,					
	/**
		The dictionary entry has the wrong Cos type.
		@ingroup PDSEditSpecificErrors
	*/
	pdsErrWrongTypeEntry,		
	/**
		The wrong type parameter was supplied to a <code>PDS</code> procedure.
		@ingroup PDSEditSpecificErrors
	*/
	pdsErrWrongTypeParameter,		
	/**
		There is already a table entry with the same name.
		@ingroup PDSEditSpecificErrors
	*/
	pdsErrAlreadyExists,			
	/**
		Some software required to perform this operation is not present in this version of Acrobat.
		@ingroup PDSEditSpecificErrors
	*/
	pdsErrCantDo					
};

/* PDMetadata (XAP Metadata API) specific errors */
enum
{
	/**
		The given metadata was not in the XAP format.
		@ingroup PDMetadataSpecificErrors
	*/
    pdMetadataErrBadXAP,           
	/**
		XAP metadata processing found a syntax error in the PDF.
		@ingroup PDMetadataSpecificErrors
	*/
    pdMetadataErrBadPDF,       
	/**
		An internal representation of the XAP metadata could not be created.
		@ingroup PDMetadataSpecificErrors
	*/
    pdMetadataErrCouldntCreateMetaXAP, 
	/**
		An internal error occurred while processing the XAP metadata.
		@ingroup PDMetadataSpecificErrors
	*/
    pdMetadataErrInternalError   
};

#if MAC_PLATFORM
/* Macintosh System errors */
enum
{
	/**
		No error (<code>noErr</code>).
		@ingroup MacintoshSystemErrors
	*/
	cfMacNoErr = 0,                
	/**
		The directory is full (<code>dirFulErr</code>).
		@ingroup MacintoshSystemErrors
	*/
	cfMacdirFulErr = -33,    
	/**
		The document's disk or the disk used for temporary files is full (<code>dskFulErr</code>).
		@ingroup MacintoshSystemErrors
	*/
	cfMacdskFulErr = -34,      
	/**
		There is no such volume available (<code>nsvErr</code>).
		@ingroup MacintoshSystemErrors
	*/
	cfMacnsvErr = -35,             
	/**
		A file I/O error has occurred (<code>ioErr</code>).
		@ingroup MacintoshSystemErrors
	*/
	cfMacioErr = -36,       
	/**
		The end of file was reached unexpectedly (<code>eofErr</code>).
		@ingroup MacintoshSystemErrors
	*/
	cfMaceofErr = -39,          
	/**
		This file is locked (<code>fLckdErr</code>).
		@ingroup MacintoshSystemErrors
	*/
	cfMacfLckdErr = -45,       
	/**
		This volume is locked and cannot be written to (<code>vLckdErr</code>).
		@ingroup MacintoshSystemErrors
	*/
	cfMacvLckdErr = -46,         
	/**
		This file is busy and cannot be deleted (<code>fBsyErr</code>).
		@ingroup MacintoshSystemErrors
	*/
	cfMacfBsyErr = -47,         
	/**
		Another file already exists under the same name (<code>dupFNErr</code>).
		@ingroup MacintoshSystemErrors
	*/
	cfMacdupFNErr = -48,       
	/**
		This file is already open or in use by another application (<code>opWrErr</code>).
		@ingroup MacintoshSystemErrors
	*/
	cfMacopWrErr = -49,       
	/**
		This file's volume is not available (<code>volOffLinErr</code>).
		@ingroup MacintoshSystemErrors
	*/
	cfMacvolOffLinErr = -53,   
	/**
		You do not have permission to open this file (<code>permErr</code>).
		@ingroup MacintoshSystemErrors
	*/
	cfMacpermErr = -54,        
	/**
		This disk is not a Macintosh disk (<code>noMacDskErr</code>).
		@ingroup MacintoshSystemErrors
	*/
	cfMacnoMacDskErr = -57,     
	/**
		You do not have permission to write to this file (<code>wrPermErr</code>).
		@ingroup MacintoshSystemErrors
	*/
	cfMacwrPermErr = -61,       
	/**
		Out of memory (<code>-26</code>). (<code>dsMemFullErr</code>).
		@ingroup MacintoshSystemErrors
	*/
	cfMacdsMemFullErr = -26,     
	/**
		Out of memory (<code>-108</code>). (<code>memFullErr</code>).
		@ingroup MacintoshSystemErrors
	*/
	cfMacmemFullErr = -108,      
	/**
		Tried to get a nonexistent resource (<code>-192</code>). (<code>resNotFound</code>).
		@ingroup MacintoshSystemErrors
	*/
	cfMacresNotFound = -192,    
	/**
		PostScript error (<code>-8133</code>).
		@ingroup MacintoshSystemErrors
	*/
	cfMacGenPSErr = -8133,		
	/**
		An I/O error has occurred (<code>-27</code>). (<code>iIOAbort</code>).
		@ingroup MacintoshSystemErrors
	*/
	cfMaciIOAbort = -27 ,      
	/**
		Error saving print file (<code>-1</code>).
		@ingroup MacintoshSystemErrors
	*/
	cfMaciPrSavPFil = -1,		
	/**
		This file's server connection has closed down (<code>aspParamErr</code>).
		@ingroup MacintoshSystemErrors
	*/
	cfMacServerLostConnection = -1070	/* This file's server connection has closed down. (<code>aspParamErr</code>). */
};

/* Macintosh Application errors */
enum
{
	/**
		No error.
		@ingroup MacintoshApplicationErrors
	*/
	mdAppErrNoError,            
	/**
		Cannot print to Acrobat PDFWriter.
		@ingroup MacintoshApplicationErrors
	*/
	mdAppCantPrintToPDFWriter, 
	/**
		Please close all Desk Accessories before printing.
		@ingroup MacintoshApplicationErrors
	*/
	mdAppNoDAsWhilePrint,        
	/**
		Printing is not possible until you have chosen a Printer using the Chooser.
		@ingroup MacintoshApplicationErrors
	*/
	mdAppNoPrinter,            
	/**
		Background printing is not possible with the Assistant Toolbox system extension and loadable ATM.
		@ingroup MacintoshApplicationErrors
	*/
	mdAppAsstToolboxActive,		  
	/**
		Some data in the embedded font was invalid. Some characters may not display or print correctly.
		@ingroup MacintoshApplicationErrors
	*/
	mdAppIncorrectTTEmbed		 
};
#endif

#if WIN_PLATFORM
enum
{
	/**
		The file does not exist.
		@ingroup WindowsErrors
	*/
	WinBadFileErr = 2,       
	/**
		The path does not exist.
		@ingroup WindowsErrors
	*/
	WinBadPathErr = 3,           
	/**
		There are too many open files.
		@ingroup WindowsErrors
	*/
	WinTooManyErr = 4,     
	/**
		Access denied.
		@ingroup WindowsErrors
	*/
	WinAccessErr = 5,           
	/**
		Bad file handle.
		@ingroup WindowsErrors
	*/
	WinBadHdlErr = 6,            
	/**
		Not enough memory.
		@ingroup WindowsErrors
	*/
	WinMemErr = 8,                
	/**
		Badly formatted disk.
		@ingroup WindowsErrors
	*/
	WinBadDiskErr = 11,          
	/**
		Invalid drive.
		@ingroup WindowsErrors
	*/
	WinBadDriveErr = 15,          
	/**
		You do not have write permission.
		@ingroup WindowsErrors
	*/
	WinWrPermErr = 19,             
	/**
		Not an MS-DOS disk.
		@ingroup WindowsErrors
	*/
	WinNotDosErr = 26,           
	/**
		General failure.
		@ingroup WindowsErrors
	*/
	WinGeneralErr = 31,           
	/**
		Sharing violation.
		@ingroup WindowsErrors
	*/
	WinShareErr = 32,              
	/**
		Lock violation.
		@ingroup WindowsErrors
	*/
	WinLockErr = 33,               
	/**
		Device does not exist.
		@ingroup WindowsErrors
	*/
	WinDeviceErr = 39,           
	/**
		File already exists.
		@ingroup WindowsErrors
	*/
	WinExistsErr = 80           
};
#endif

#if OS2_PLATFORM
enum
{
	/**
		The file does not exist.
		@ingroup OS2Errors
	*/
	OS2BadFileErr = 2,       
	/**
		The path does not exist.
		@ingroup OS2Errors
	*/
	OS2BadPathErr = 3,         
	/**
		Too many open files.
		@ingroup OS2Errors
	*/
	OS2TooManyErr = 4,           
	/**
		Access denied.
		@ingroup OS2Errors
	*/
	OS2AccessErr = 5,            
	/**
		Bad file handle.
		@ingroup OS2Errors
	*/
	OS2BadHdlErr = 6,          
	/**
		Not enough memory.
		@ingroup OS2Errors
	*/
	OS2MemErr = 8,               
	/**
		Badly formatted disk.
		@ingroup OS2Errors
	*/
	OS2BadDiskErr = 11,          
	/**
		Invalid drive.
		@ingroup OS2Errors
	*/
	OS2BadDriveErr = 15,         
	/**
		You do not have write permission.
		@ingroup OS2Errors
	*/
	OS2WrPermErr = 19,           
	/**
		Not an MS-DOS disk.
		@ingroup OS2Errors
	*/
	OS2NotDosErr = 26,            
	/**
		General failure.
		@ingroup OS2Errors
	*/
	OS2GeneralErr = 31,          
	/**
		Sharing violation.
		@ingroup OS2Errors
	*/
	OS2ShareErr = 32,             
	/**
		Lock violation. 
		@ingroup OS2Errors
	*/
	OS2LockErr = 33,            
	/**
		The device does not exist.
		@ingroup OS2Errors
	*/
	OS2DeviceErr = 39,            
	/**
		The file already exists.
		@ingroup OS2Errors
	*/
	OS2ExistsErr = 80              
};
#endif

#if UNIX_PLATFORM

#include <errno.h>

/* Unix System errors */
enum
{
	/**
		No error.
		@ingroup UnixSystemErrors
	*/
	mdSysNoErr		= 0,		
	/**
		The math argument is out of the domain of the function.
		@ingroup UnixSystemErrors
	*/
	mdSysEDOM		= EDOM,		
	/**
		The math result cannot be represented.
		@ingroup UnixSystemErrors
	*/
	mdSysERANGE		= ERANGE,	
	/**
		Not the super user.
		@ingroup UnixSystemErrors
	*/
	mdSysEPERM		= EPERM,	
	/**
		No such file or directory.
		@ingroup UnixSystemErrors
	*/
	mdSysENOENT		= ENOENT,	
	/**
		No such process.
		@ingroup UnixSystemErrors
	*/
	mdSysESRCH		= ESRCH,	
	/**
		The system call was interrupted.
		@ingroup UnixSystemErrors
	*/
	mdSysEINTR		= EINTR,	
	/**
		I/O error.
		@ingroup UnixSystemErrors
	*/
	mdSysEIO		= EIO,		
	/**
		No such device or address.
		@ingroup UnixSystemErrors
	*/
	mdSysENXIO		= ENXIO,	
	/**
		Bad file number. 
		@ingroup UnixSystemErrors
	*/
	mdSysEBADF		= EBADF,	
	/**
		No more processes allowed.
		@ingroup UnixSystemErrors
	*/
	mdSysEAGAIN		= EAGAIN,	
	/**
		Not enough core memory.
		@ingroup UnixSystemErrors
	*/
	mdSysENOMEM		= ENOMEM,	
	/**
		Permission is denied.
		@ingroup UnixSystemErrors
	*/
	mdSysEACCES		= EACCES,	
	/**
		The file exists.
		@ingroup UnixSystemErrors
	*/
	mdSysEEXIST		= EEXIST,	
	/**
		Not a directory.
		@ingroup UnixSystemErrors
	*/
	mdSysENOTDIR	= ENOTDIR,	
	/**
		Is a directory.
		@ingroup UnixSystemErrors
	*/
	mdSysEISDIR		= EISDIR,	
	/**
		File table overflow.
		@ingroup UnixSystemErrors
	*/
	mdSysENFILE		= ENFILE,	
	/**
		Too many open files.
		@ingroup UnixSystemErrors
	*/
	mdSysEMFILE		= EMFILE,	
	/**
		No space is left on the device.
		@ingroup UnixSystemErrors
	*/
	mdSysENOSPC		= ENOSPC,	
	/**
		Read-only file system.
		@ingroup UnixSystemErrors
	*/
	mdSysEROFS		= EROFS,	
	/**
		Too many links.
		@ingroup UnixSystemErrors
	*/
	mdSysEMLINK		= EMLINK,	
#ifdef EDQUOT
	/**
		The disk quota was exceeded.
		@ingroup UnixSystemErrors
	*/
	mdSysEDQUOT		= EDQUOT,	
#endif
#ifdef ELOOP
	/**
		There are too many levels of symbolic links.
		@ingroup UnixSystemErrors
	*/
	mdSysELOOP		= ELOOP,	
#endif
#ifdef EMULTIHOP
	/**
		A multihop was attempted.
		@ingroup UnixSystemErrors
	*/
	mdSysEMULTIHOP	= EMULTIHOP,
#endif
#ifdef ENOLINK
	/**
		Inactive link to a remote machine.
		@ingroup UnixSystemErrors
	*/
	mdSysNOLINK		= ENOLINK,	
#endif
#ifdef EOVERFLOW
	/**
		Stat buffer overflow.
		@ingroup UnixSystemErrors
	*/
	mdSysOVERFLOW	= EOVERFLOW,
#endif /* EOVERFLOW */
	/**
		The file name was too long.
		@ingroup UnixSystemErrors
	*/
	mdSysENAMETOOLONG = ENAMETOOLONG,	
	/**
		Illegal address.
		@ingroup UnixSystemErrors
	*/
	mdSysEFAULT		= EFAULT	
};

/* Unix Application errors */
enum
{
	/**
		No error.
		@ingroup UnixApplicationErrors
	*/
	mdAppErrNoError,		
	/**
		Open Message partially failed.
		@ingroup UnixApplicationErrors
	*/
	mdAppOpenMsgPartFailed,	
	/**
		Open Message failed.
		@ingroup UnixApplicationErrors
	*/
	mdAppOpenMsgFailed,		
	/**
		Modal window is open.
		@ingroup UnixApplicationErrors
	*/
	mdAppModalWindowOpen,
	/**
		Bad <code>Property</code> format.
		@ingroup UnixApplicationErrors
	*/
	mdAppBadPropertyFormat,	
	/**
		<code>GetProperty</code> error.
		@ingroup UnixApplicationErrors
	*/
	mdAppGetPropertyError,	
	/**
		Bad AVWindow Platform Thing.
		@ingroup UnixApplicationErrors
	*/
	mdAppBadPlatformThing,	
	/**
		A bus error was caught.
		@ingroup UnixApplicationErrors
	*/
	mdAppSIGBUS,			
	/**
		An illegal instruction was caught.
		@ingroup UnixApplicationErrors
	*/
	mdAppSIGILL,			
	/**
		A segmentation violation was caught.
		@ingroup UnixApplicationErrors
	*/
	mdAppSIGSEGV,			
	/**
		An unknown signal was caught.
		@ingroup UnixApplicationErrors
	*/
	mdAppSIGUnknown,		
	/**
		The print job terminated.
		@ingroup UnixApplicationErrors
	*/
	mdAppLPTERM,			
	/**
		The print job stopped.
		@ingroup UnixApplicationErrors
	*/
	mdAppLPSTOP,			
	/**
		Bad temporary directory resource.
		@ingroup UnixApplicationErrors
	*/
	mdAppBadTmpDir			
};
#endif /* UNIX_PLATFORM */

#endif /* _H_AcroErr */
