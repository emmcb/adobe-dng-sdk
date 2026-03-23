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
#ifndef _EsParserAPI_h
#define _EsParserAPI_h

#include "ExtendScript.h"
#include "ScPreConfig.h"

namespace ScScript
{

/// The name of this API

const scint32_t kParserAPI = 0x70617273; // 'pars'

class ScopeNode;

/**
The parser API provides access to the internal JavaScript parser. When
feeding a Unistring source, the parser compiles this source into a well-
formed tree. This tree is already partially optimized. Constant expressions
are folded, and unreachable code (like "while (false) dead-code") has been
removed. The tree is guaranteed to have a defined structure and to be
syntactically and semantically correct. See the separate documentation
about the structure of the tree. The tree must be deleted after use by
deleting the returned scope node.
*/

class ES_API ParserAPI : public API
{
	friend class RealEngine;
							ParserAPI (Engine&);
public:
	/**
	Enable or disable the preprocessor. The return value is the previous
	state of the preprocessor enabled flag. The preprocessor is by default
	enabled.
	*/
	bool					enablePreProcessor (bool enable);
	/**
	Retrieve the include path used for preprocessor includes. This string
	contains all include directories, separated by the semicolon character.
	*/
	const ScCore::String&	getIncludePath() const;
	/**
	Define the include path used for preprocessor includes. This string
	contains all include directories, separated by the colon or the
	semicolon character. This path takes precedence over the JSINCLUDE 
	environment variable, which may contain additional include directories. 
	*/
	void					setIncludePath (const ScCore::String& path) const;
	/**
	Parse the given source code and return a scope node. This scope node
	is not named, and its array of locals contain the "var" declarations
	found in this scriptlet. The method always returns a parsed tree. On
	errors, it registers a runtime error with the engine and returns a
	partially parsed tree. The return tree must be deleted after use by
	deleting the returned scope node.
	@param	source			the source code
	@return					the parsed tree; must be deleted after use
	*/
	ScopeNode*				parse (const ScCore::String& source) const;
	/**
	Decompile a parsed tree. This method attempts to decompile the given
	tree into a scriptlet. Currently, this method just returns the source
	string stored into the supplied scope node.
	@param	node			the scope node to decompile
	@return					the decompiled source code
	@see					ScopeNode
	*/
	ScCore::String			decompile (const ScopeNode& node) const;
};

}	// end namespace

#include "ScPostConfig.h"
#endif

