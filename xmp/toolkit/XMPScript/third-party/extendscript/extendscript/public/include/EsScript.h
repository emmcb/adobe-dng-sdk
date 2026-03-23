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
#ifndef _EsScript_h
#define	_EsScript_h

#include "ScCore.h"
#include "ScPreConfig.h"

namespace ScScript
{

class BreakpointInfo;		// see below

/**
The Script class is an object containing precompiled, executable code.
You can retrieve the original information like the source code, the file
name or the the starting line number. The Script object is reference counted, so
once you are done with it, call its release() method. Script objects are not
engine dependent. A script compiled inside one engine may be executed on a
different engine. Compilation erorrs can be found in the compiling engine.
*/

class ES_API Script : public ScCore::Refcountable
{
protected:
	/// use release() to delete this object.
	virtual				~Script();
public:
	/**
	To avoid binary incompatibility, this method is static, although it should be virtual.
	Return true if the given line points to executable code.
	*/
	static	bool		isValidLine (const Script& r, scint32_t line);
	/// Return true if this script is compiled code that can not be debugged.
	virtual	bool		isBinary() const = 0;
	/// Retrieve the source code of the script.
	virtual	const ScCore::String& getSource() const = 0;
	/// Retrieve the file name that was supplied to compile() or eval(). This may be the empty string.
	virtual	const ScCore::String& getFileName() const = 0;
	/**
	Get the main file name as a sID; this may be sID_EMPTY.
	*/
	virtual	sID			getFileNameID() const = 0;
	/**
	Fill a list of file name sIDs that this script contains. The first returned element
	is the main file sID as returned by getFileNameID(). Other elements are the IDs of
	files included during compilation with the #include statement.
	*/
	virtual	void		getFileNameIDs (ScCore::SimpleArray& ids) const = 0;
	/**
	Store the script in the given string.
	*/
	virtual	scerror_t	store (ScCore::String& r) const = 0;
};

}	// end namespace

#include "ScPostConfig.h"

#endif


