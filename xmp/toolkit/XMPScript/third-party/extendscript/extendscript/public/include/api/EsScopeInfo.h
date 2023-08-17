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
#ifndef _EsScopeInfo_h
#define _EsScopeInfo_h

#include "ExtendScript.h"
#include "ScPreConfig.h"

namespace ScScript
{

/**
The scope info data block contains basic information about one function declaration
or scriptlet. This is the source code, the name of the function and a hashtable
containing all formal parameters, local variables and local constants. The values
stored in the hashtable are numeric ids as described below.
*/

class ES_API ScopeInfo : public ScCore::Root
{
	/// Private copy ctor, because this class has pointer data members.
						ScopeInfo (const ScopeInfo&);
	/// Private assignment operator, because this class has pointer data members.
	ScopeInfo& 			operator= (const ScopeInfo&);

	/// Scope info blocks are refcounted.
	mutable scint32_t	mRefs;
public:
	/**
	A collection of possible variable types in a scope info block.
	This collection contains several values to distinguish between 
	variable types in the hash table of a ScopeInfo structure. The lower bits 
	are index values, describing the order of appearance.
	*/
	enum
	{
		/// The mask used to distinguish between various variable types.
		kTypeMask			= 0xE0000000L,
		/// Formal parameters. The index is the parameter number.
		kFormalArgument		= 0x20000000L,
		/// Local variables. The index is the order of appearance of a var, zero based.
		kVar				= 0x40000000L,
		/// Local constants. The index is the order of appearance of a const, zero based.
		kConst				= 0x60000000L,
		__Type__32bit__		= 0x7FFFFFFF
	};
	/**
	The status flags contain some information about the scope.
	*/
	enum
	{
		/// The lower 16 bits contain debug flag information.
		kDebugFlagsMask		= 0x0000FFFFL,
		/// Indicates whether the info covers a scriptlet or a function.
		kScriptlet			= 0x00010000L,
		/// Set if the function declaration was part of an expression.
		kExpression			= 0x00020000L,
		/// Set if the function declaration was loaded in binary form.
		kBinary				= 0x00040000L,
		__Flags__32bit__	= 0x7FFFFFFF
	};
						ScopeInfo();
	virtual				~ScopeInfo();
	inline	void		incRef() const { mRefs++; }
	inline	void		decRef() const { if (!--mRefs) delete this; }

	///	The full, original source code of the scriptlet containing this scope.
	ScCore::String		iSource;
	///	The file name sID (see DataPool class) as given to eval() or compile() - may be sID_EMPTY.
	sID					iFile;
	/// The start of the source code for this scope inside the original source.
	scint32_t			iSrcBgn;
	/// The length of the source code for this scope inside the original source.
	scint32_t			iSrcLen;
	/// The pool of local variables. The values are variable types plus index values.
	ScCore::SparseArray	iVariables;
	/// The number of formal parameters.
	scint32_t			iFormals;
	/// The number of variables.
	scint32_t			iVars;
	/// The number of constants.
	scint32_t			iConsts;
	/// Obsolete - always zero.
	scint32_t			iLine1;
	/**	The name of the scope. This name is the name of the scriptlet or function. For
	anonymous functions or for scriptlets, the name is sID_EMPTY. */
	sID					iName;
	/// If this is a scriptlet, this field may contain an array of exported symbols.
	ScCore::SimpleArray* iExports;
	/// Status flags.
	scuint32_t			iFlags;
};

}	// end namespace

#include "ScPostConfig.h"
#endif


