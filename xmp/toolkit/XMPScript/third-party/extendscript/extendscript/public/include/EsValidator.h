/**************************************************************************
* $File$
* $Author$
* $DateTime$
* $Revision$
* $Change$
*
* ADOBE CONFIDENTIAL
* ___________________
*
*  Copyright 2005 Adobe Systems Incorporated
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
*
* Release Notes: http://ctwiki.corp.adobe.com/bin/view/Main/BridgeTalkReleaseNotes
*
**************************************************************************/

#if SC_ONCE
#pragma once
#endif

#ifndef _EsValidator_h
#define _EsValidator_h

#include "ScString.h" 
#include "ScHashTable.h" 

namespace ScScript
{

/** 
See also http://ctwiki.corp.adobe.com/bin/view/Main/ExtendScriptValidator 
<p>
The implementation of Validator is delivered with the separate static Validator*.lib 
and NOT with ExtendScript.
<p>
Validator tests the following cases:
<p>
1)	Downcast every derived class with virtual method, thus testing dynamic_cast, 
Make sure that every such class has at least one virtual method defined.
<p>
2)	Instantiate more complex objects, like LiveObject derivates, and test the 
	virtual methods to make sure that the vtable layout has not changed.
<p>
3)	Throw std::bad_alloc across the DLL boundary and try to catch it in the app.
	The executable would only be rebuilt when making binary incompatible changes.
<p>
LIMITATIONS:
<p>
The Validator's vtable validation does not work with projects that use the incremental 
linking (/INCREMENTAL) feature in Visual Studio on Windows.
<p>
For more details see Microsoft's documentation: 
http://msdn2.microsoft.com/en-us/library/4khtbfyf(VS.80).aspx
<p>
*/

class Validator 
{
public:
	// type definition for function parameter used for logging during validate()
	typedef	void (*Log)( const ScCore::String& inLogText );
	
	/**
	Validates ExtendScript's and ScCore's API by checking three areas: 
	downcasts, vtable changes, throwing std::bad_alloc across the DLL boundary.	
	@param	inLog		will be called for logging results during validate()
	@param	outErrors	arrays of error strings.
	@return				true if the API used by ValidatorLib is identical to the API 
						currently provided by ExtendScript and ScCore.
	*/
	static bool	validate( Log inLog = NULL, ScCore::Array* outErrors = NULL );
	/// Returns the ExtendScript version used by ValidatorLib.
	static ScCore::String	getVersion();
	/// Returns the build of the ExtendScript version used by ValidatorLib.
	static ScCore::String	getBuild();
	/// Returns the build date of the ExtendScript version used by ValidatorLib.
	static ScCore::String	getBuildDate();
	/// Creates a HashTable that maps method names to vtable offset numbers.
	static ScCore::HashTable* createValidateVTable();
};
	
}	// end namespace

#endif


