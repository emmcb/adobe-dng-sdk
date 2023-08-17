/**************************************************************************
*
* ADOBE CONFIDENTIAL
* ___________________
*
*  Copyright 1998 Adobe Systems Incorporated
*  All Rights Reserved.
*
* NOTICE:  All information contained herein is, and remains the property of
* Adobe Systems Incorporated  and its suppliers,  if any.  The intellectual 
* and technical concepts contained herein are proprietary to  Adobe Systems 
* Incorporated  and its suppliers  and may be  covered by U.S.  and Foreign 
* Patents,patents in process,and are protected by trade secret or copyright 
* law.  Dissemination of this  information or reproduction of this material
* is strictly  forbidden  unless prior written permission is  obtained from 
* Adobe Systems Incorporated.
* $File$
* $Author$
* $DateTime$
* $Revision$
* $Change$
**************************************************************************/

#if SC_ONCE
#pragma once
#endif
#ifndef _ScError_h
#define _ScError_h

#include "ScString.h"
#include "ScVariant.h"
#include "ScPreConfig.h"

extern void ScErrorSetup (bool);

namespace ScCore
{

class LiveObject;
class Cloneable;

/**
The list of runtime errors. Errors with negative values are considered fatal
and cannot be caught by Javascript. When defining own error messages, you
can use %ls or %1 as a placeholder for own strings.
<p>
In the JavaScript world, errors have a code, a descriptive message and a class. 
This error class is thrown by the runtime system if a runtime error occurs, and 
if there is an active try clause. Fatal errors are never thrown; they are always 
forwarded to the caller as a runtime error. The error kErrNoMemory is a 
runtime error unless you want ExtendScript to generate a C++ exception.
*/

enum RuntimeErrors
{
	/// No error
	kErrOK												= 0,
	/// %1 (Error)
	kErrGeneral											= 1,
	/// %1 is undefined (ReferenceError)
	kErrUndefined										= 2,
	/// Cannot assign value (ReferenceError)
	kErrNoLvalue										= 3,
	/// Unterminated string constant (SyntaxError)
	kErrOpenString										= 4,
	/// Unterminated comment (SyntaxError)
	kErrOpenComment										= 5,
	/// Bad digit in number (SyntaxError)
	kErrBadDigit										= 6,
	/// Language feature %1 is not supported (SyntaxError)
	kErrUnsupported										= 7,
	/// Syntax error (SyntaxError)
	kErrSyntax											= 8,
	/// Illegal use of reserved word '%1' (SyntaxError)
	kErrKeyword											= 9,
	/// Break or Continue outside a loop (SyntaxError)
	kErrBadBreakContinue								= 10,
	/// Label not found (SyntaxError)
	kErrBadLabel										= 11,
	/// Deprecated.
	kErrExpressionNotConst								= 12,
	/// Too many closing braces (SyntaxError)
	kErrClosedBlock										= 13,
	/// No matching closing brace found (SyntaxError)
	kErrOpenBlock										= 14,
	/// try without catch/finally (SyntaxError)
	kErrNoCatch											= 15,
	/// catch/finally without try (SyntaxError)
	kErrNoTry											= 16,
	/// Variable expected (TypeError)
	kErrVarExpected										= 17,
	/// Deprecated.
	kErrScalarExpected									= 18,
	/// Bad argument %1 (TypeError)
	kErrBadArgument										= 19,
	/// Bad argument list (TypeError)
	kErrBadArgumentList									= 20,
	/// %1 is not an object (TypeError)
	kErrObjectExpected									= 21,
	/// %1 does not have a constructor (ReferenceError)
	kErrNoCtor											= 22,
	/// %1 does not have a value (ReferenceError)
	kErrNoValue											= 23,
	/// %1 is not a function (ReferenceError)
	kErrNoFunction										= 24,
	/// Expected: %1 (SyntaxError)
	kErrExpected										= 25,
	/// %1 cannot work with this class (Error)
	kErrWrongClass										= 26,
	/// Stack overrun
	kErrStackOverrun									= 27,
	/// Out of memory
	kErrNoMemory										= -28,
	/// Uncaught exception %1
	kErrException										= -29,
	/// Illegal 'return' outside of a function body (SyntaxError)
	kErrBadReturn										= 30,
	/// Bad URI (URIError)
	kErrBadURI											= 31,
	/// Cannot perform requested action (Error)
	kErrBadAction										= 32,
	/// Internal error
	kErrInternal										= -33,
	/// Execution halted. Does not activate the debugger.
	kErrHalted											= -34,
	/// Timeout while waiting for the engine
	kErrTimeout											= -35,
	/// Not yet implemented
	kErrNotImplemented									= -36,
	/// Conversion error (SyntaxError)
	kErrCharConversion									= 37,
	/// Partial multibyte character found (SyntaxError)
	kErrCharPartial										= 38,
	/// More than one switch default (SyntaxError)
	kErrDuplicateDefault								= 39,
	/// %1 redeclared (TypeError)
	kErrRedeclared										= 40,
	/// %1 is out of range (RangeError)
	kErrRange											= 41,
	/// Catch after unconditional catch (SyntaxError)
	kErrCatchAfterCatch									= 42,
	/// Evaluation error (EvalError)
	kErrEval											= 43,
	/// Cannot convert (TypeError)
	kErrConversion										= 44,
	/// Object is invalid (ReferenceError)
	kErrInvalidObject									= 45,
	/// %1 is read only (ReferenceError)
	kErrReadOnly										= 46,
	/// Type mismatch (TypeError)
	kErrTypeMismatch									= 47,
	/// File or folder does not exist (IOError)
	kErrNoFile											= 48,
	/// File or folder already exists (IOError)
	kErrFileExists										= 49,
	/// I/O device is not open (IOError)
	kErrNotOpen											= 50,
	kErrFileNotOpen = kErrNotOpen,
	/// Read past EOF (IOError)
	kErrEOF												= 51,
	/// I/O error (IOError)
	kErrIO												= 52,
	/// Permission denied (ReferenceError)
	kErrNoPermission									= 53,
	/// JavaScript exception (Error)
	kErrJSException										= 54,
	/// Object does not support the property or method '%1' (ReferenceError)
	kErrUnsupportedElement								= 55,
	/// Cannot connect to %1 (IOError)
	kErrCannotConnect									= 56,
	/// Cannot resolve reference (IOError)
	kErrCannotResolve									= 57,
	/// I/O Timeout (IOError)
	kErrIOTimeout										= 58,
	/// No response (IOError)
	kErrNoResponse										= 59,
	/// Not well-formed (invalid token)
	kErrInvalidToken									= 60,
 	/// Unclosed token
	kErrUnclosedToken									= 61,
	/// Mismatched tag 
	kErrTagMismatch										= 62,
	/// Duplicate attribute
	kErrDuplicateAttribute								= 63,
	/// Junk after document element
	kErrJunkAfterDocElement								= 64, 
	/// Entity parsing error 
	kErrBadEntity										= 65,
	/// XML declaration not at start of external entity 
	kErrMisplacedProcessing								= 66,
	/// Unknown encoding
	kErrUnknownEncoding									= 67, 
	/// Encoding specified in XML declaration is incorrect 
	kErrIncorrectEncoding								= 68,
	/// Unclosed CDATA section 
	kErrUnclosedCdataSection							= 69,
	/// Requested feature requires XML_DTD support 
	kErrFeatureRequiresDTD								= 70,
	/// Unexpected DOCTYPE name
	kErrUnexpectedDoctypeName							= 71,
	/// Unbound namespace prefix
	kErrUnboundPrefix									= 72,
	/// Unknown axis name: %1
	kErrUnknownAxis										= 73,
	/// Unsupported axis
	kErrUnsupportedAxis									= 74,	
	/// Operand expected, but found %1
	kErrOperandExpected									= 75,
	/// Node set expected
	kErrNodeSetExpected									= 76,
	/// Node test expected, but found %1
	kErrNodeTestExpected								= 77,
	/// Cannot set property %1
	kErrCannotSet										= 78,
	/// ActionScript error: %1
	kErrActionScript									= 79,
	/// The last hardwired error code
	kErrLastError										= 79,
	/// The first error code for ScriptUI
	kErrFirstScriptUIError								= 501,
	/// The last error code for ScriptUI
	kErrLastScriptUIError								= 550,
	/// The first available error code for apps
	kErrFirstAppError									= 1000,
	/// The last available error code for apps
	kErrLastAppError									= 9999,
	__RuntimeErrors_32bit__								= 0x7FFFFFFF
};

/**
A runtime error is an error code plus an additional data element. In addition,
there is a pointer to arbitrary Cloneable-based error information. This pointer
may be used by some specific error codes. Errors may be chained as a stack.
This class is part of the <a href="Error.html">Error</a> class, which owns all errors.
@see Cloneable
*/

class SC_API ErrorInfo : public Heap
{
	/// Private assignment operator, because ErrorInfo has pointer data members.
	ErrorInfo& 		operator= (const ErrorInfo&);

	friend class Error;
	friend class ErrorSetup;

	mutable const ErrorInfo*	mNext;		// link to the next error
	String						mText;		// the error text
	String						mFile;		// the file name of the script (only set if supplied to Engine::eval())
	String						mSource;	// The source code of the script the line number refers to.
	mutable Variant*			mVariant;	// optional error data, created on demand
	const Cloneable*			mData;		// any additional data
	scint32_t						mLine;		// The line where the error occured, 0-based.
	scint32_t						mOffset;	// The character offset of the erroneous symbol.
	scint32_t						mColumn;	// The starting column of the erroneous symbol.
	scuint16_t					mLength;	// The length of the erroneous symbol.
	scerror_t					mCode;		// the error code

								ErrorInfo (const ErrorInfo&);
								ErrorInfo (scerror_t, const String&, const Cloneable*, bool);
								ErrorInfo (scerror_t, const Variant&, const Cloneable*);
								~ErrorInfo();	// only the error stack may delete errors
public:
	/// Get the error code.
	inline	scerror_t			getCode() const { return mCode; }
	/// Get the error data. Set to any data thrown in case of the kErrJSException error.
			const	Variant&	getErrorData() const;
	/// Get the error data, usually this is the localized error text.
	inline	const String&		getText() const { return mText; }
	/// Get the file name of the script the line number refers to. Only set if supplied to the eval() call.
	inline	const String&		getFile() const { return mFile; };
	/// Get the source code of the script the line number refers to.
	inline	const String&		getSource() const { return mSource; };
	/// Get the line offset relative the the source code where the error occured, 0-based. The default is -1.
	inline	scint32_t				getLine() const { return mLine; };
	/// Get the character offset of the erroneous symbol. The default is -1.
	inline	scint32_t				getOffset() const { return mOffset; }
	/// Get the starting column of the erroneous symbol. The default is -1.
	inline	scint32_t				getColumn() const { return mColumn; }
	/// Get the length of the erroneous symbol.
	inline	scuint16_t			getLength() const { return mLength; }
	/// Get the additional data pointer if present.
	inline	const Cloneable*	getData() const { return mData; }
	/// Get the next error in the chain if present.
	inline	const ErrorInfo*	getNext() const { return mNext; }
};

/// The error data structure refcounts the error chain.

class ErrorData : public Heap
{
	/// Private copy ctor, because this class has pointer data members.
						ErrorData (const ErrorData&);
	/// Private assignment operator, because this class has pointer data members.
	ErrorData& 			operator= (const ErrorData&);

public:
	const ErrorInfo*	mError;
	scint32_t				mRefs;
	
	inline				ErrorData() : mError(NULL), mRefs(0) { }
};

/**
The Error class is the container for all errors. Its push() methods
insert a new error at the beginning of the error stack, so multiple
components may add error information. The push methods are exception
safe; if there is an exception during the pushing of an error, the entire
current stack is deleted and replaced with a special, static error
containing either an kErrNoMemory or kErrException. As long as the
stack contains this special data, you cannot push any other errors. 
The data of the stack is refcounted so it can be copy-constructed 
and assigned very fast. It follows a copy-on-write schema; if you
copy-construct an instance, only the refcount is increased. If you 
then push a new error, the instance creates a new copy of the stack
before pushing the new error information.
<p>
JavaScript exceptions are forwarded as the runtime error kErrJSException
with the variant instance of the error data set to whatever Javascript
threw. If a call returned the error kErrJSException, examine the attached
variant. If it is undefined, the exception is unknown and cannot be caught
by JavaScript. You can cause a calling JavaScript to throw a JavaScript
exception by storing the runtime error code kErrJSException along with the
data that JavaScript should throw before returning to JavaScript.
*/

class SC_API Error : public Heap
{
private:
			ErrorData*			mData;
			void				unique();
public:
	/**
	Define an error code. Supply the explanatory text and, if applicable, the
	JavaScript error class. If you do not supply an error text, an empty text
	is assumed. The text may contain the character sequence "%1", ,where the
	system inserts any additional text that has been supplied to the error.
	If no class is supplied, the class is "Error". While it is possible to
	redefine predefined errors, you should not do this.
	@param	code				the error code
	@param	msg					the error message (with an optional %1 embedded)
	@param	cls					the JavaScript class name (ASCII only)
	*/
	static	void				defineError (scerror_t code, const String& msg, const char* cls = NULL);
	/**
	Undefine an error. COmponents that unload dynamically may want to undefine
	their error codes on termination. Standard error codes cannt be undefined,
	but if a standard error code has been redefined, this call can be used to
	uncover the original error definition again.
	@param	code				the error code
	*/
	static	void				undefineError (scerror_t code);
	/**
	Retrieve the definition of a runtime error. If there is no error defined,
	the methods returns false and does not fill in any error string. Otherwise,
	it fills in the two error strings and returns true.
	@param	code				the error code
	@param	msg					takes the error message
	@param	cls					takes the error class name
	*/
	static	bool				getErrorDefinition (scerror_t code, String& msg, String& cls);
	/**
	Check whether the given symbol is an error class.
	*/
	static	bool				isErrorClass (const String& errClass);

								Error();
								Error( const Error& );
								Error& operator= ( const Error& );
								~Error();

	/**
	Conveniency ctor used to create an initial error state. The ctor is
	equivalent to: "default ctor" followed by push. The constructor uses 
	whatever string is passed in as error message if "localize" is false.
	If "localize" is true, it first calls the Localizer method getLocalizedText() 
	to localize the string. Then, the localizer's getMessage() method is called 
	to retrieve the error message, and all occurrances of "%1" are replaced with 
	the localized version of the supplied string.
	*/
								Error( scerror_t code, const String& text, bool localize=true );
	/**
	Push the complete error info contained in the source Error class on top of this instance.
	@param	error				the Error instance to push
	*/
			void				push (const Error& r);
	/**
	Push the given error info on top of this instance.
	@param	error				the ErrorInfo instance to push
	*/
			void				push (const ErrorInfo& r);
	/**
	The code-only push() method fetches the error message from the Localizer
	instance by calling its getMessage() method. The optional data pointer may 
	contain additional, error-specific data. This data must be allocated on the 
	heap; the ErrorInfo instances destroy the data in their destructor.
	@param	code				the error code
	@param	data				optional additional data
	*/
			void				push (scerror_t code, const Cloneable* data = NULL);
	/**
	This method uses the passed LiveObject property ID to generate the error message. It
	calls the LiveObject's getErrorText() method to create the error text.
	The optional data pointer may contain additional, error-specific
	data. This data must be allocated on the heap; the ErrorInfo instances destroy
	the data in their destructor.
	@param	code				the error code
	@param	r					the affected LiveObject
	@param	id					the property ID
	@param	isIndex				if true, the property ID is nan unsigned scint32_t array index
	@param	data				optional additional data
	*/
			void				push (scerror_t code, const LiveObject& r, scint32_t id, bool isIndex = false, const Cloneable* data = NULL);
	/**
	Push a new error containing arbitrary data. Usually, this method is used to push
	a JavaScript exception. The optional data pointer may contain additional, error-specific
	data. This data must be allocated on the heap; the Error instances destroy
	the data in their destructor. 
	@param	code				the error code
	@param	errData				the error data
	@param	data				optional additional data
	*/
			void				push (scerror_t code, const Variant& errData, const Cloneable* data = NULL);
	/**
	This method uses whatever string is passed in as error message if "localize" is false.
	If "localize" is true, it first calls the Localizer method getLocalizedText() to localize 
	the string. Then, the localizer's getMessage() method is called to retrieve the error message,
	and all occurrences of "%1" are replaced with the localized version of the supplied string.
	The optional data pointer may contain additional, error-specific data. This data must 
	be allocated on the heap; the Error instances destroy the data in their destructor. 
	@param	code				the error code
	@param	text				the error text
	@param	data				optional additional data
	@param	localize			use the localizer to localize the message if true; use the string as-is if false
	*/
			void				push (scerror_t code, const String& text, const Cloneable* data=NULL, bool localize=true);
	/**
	Set more info about the error. This method appends source, line etc info to the error being
	on top of the error stack. Useful for scripts that wish to specify the exact location of the
	error.
	@param	source				the source code
	@param	line				the line number offset where the error occured, 0-based, relative to source code
	@param	offset				the offset of the error location relative to the source code
	@param	col					the starting column in the error line
	@param	length				the length of the offending symbol if applicable
	*/
			void				setMoreInfo (const String& src, scint32_t line=-1, scint32_t off=-1, scint32_t col=-1, scuint16_t len=0);
	/**
	Erase all errors.
	*/
			void				erase();
	/**
	Get the top of the chained errors. If there is no error, this method
	returns null.
	*/
	inline	const ErrorInfo*	getError() const { return mData->mError; }
	/**
	Get the data for the first error if there is one.
	*/
	inline	const Variant*		getErrorData() const { return (NULL == mData->mError) ? (const Variant*) NULL : &mData->mError->getErrorData(); }
	/**
	Get the error code of the first entry if there is one.
	*/
	inline	scerror_t			getCode() const { return (NULL == mData->mError) ? scerror_t (kErrOK) : mData->mError->getCode(); }
	/**
	Combine all error messages into a single message by appending them with LF
	characters and prepending the 2nd and following lines with a dash. The call
	walks the chain of ErrorInfo classes.
	@param	text				takes the full text
	*/
			void				getFullText (String& text) const;
};

}	// end Namespace

#include "ScPostConfig.h"

#endif


