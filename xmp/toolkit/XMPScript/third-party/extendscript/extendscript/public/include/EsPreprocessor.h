/**************************************************************************
*
* ADOBE CONFIDENTIAL
* ___________________
*
*  Copyright 1998 - 2006 Adobe Systems Incorporated
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
#ifndef _EsPreprocessor_h
#define	_EsPreprocessor_h

#include "ScCore.h"

namespace ScScript
{

/**
The ExtendScript preprocessor scans a JavaScript source file for preprocessor 
directives. Such a directive is either a line that begins with a hash sign
and a directive name (like #include), or a @word right after a single-line comment
(like e.g. // @include). The following directives are given special treatment:
<ul>
<li>#include     - the file is included in the output
<li>#includepath - add the path name(s) to the list of includes
</ul>
All other preprocessor directives are collected into a hash table of strings.
*/

class ES_API Preprocessor : public ScCore::Root
{
public:
									Preprocessor();
	virtual							~Preprocessor();
	/**
	Get the include path. This path is initialized to the contents
	of the JSINCLUDE environment variable.
	*/
			const ScCore::String&	getIncludes() const;
	/// Set the include path.
			void					setIncludes (const ScCore::String& path);
	/**
	Preprocess the source; the optional Error instance takes errors.
	Return the error code.
	@param	src						the source code
	@param	file					the source file name; the parent directory is
									temporarily appended to the include paths
	@param	out						takes the source code with included files and
									preprocessor directives removed
	@param	errors					takes an error description if supplied
	@returns						an error code
	*/
			scerror_t				process (const ScCore::String& src, const ScCore::String& file, 
											 ScCore::String& out, ScCore::Error* error = NULL);
	/**
	Return the string associated with a preprocessor directive.
	If the directive was not found, return NULL.
	@param	name					the directive name
	@return							a string pointer to the line remainder or NULL
	*/
			const ScCore::String*	getDirective (const ScCore::String& name) const;
private:
			struct					ProcData;
			class					Data;
			Data*					mData;
			bool					doProcess (ProcData&);		// process a source string
			bool					directive (ProcData&);		// process a directive
};

}	// end namespace

#endif
