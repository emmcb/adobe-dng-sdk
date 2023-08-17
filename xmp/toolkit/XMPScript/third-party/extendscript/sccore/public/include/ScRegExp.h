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
#ifndef _ScRegExp_h
#define _ScRegExp_h

#include "ScDefs.h"
#include "ScString.h"
#include "ScLock.h"
#include "ScPreConfig.h"

namespace ScCore
{

class RegExpData;

/**
A single capture contains start and end offsets. If there is no capture, start is -1.
The end points to the first character behind the match, and is undefine if there is
no match. The capture list is an array of these elements, with the overall match
being in the first element.
*/

class Capture : public Heap
{
public:
	scint32_t				mStart, mEnd;
	inline					Capture() : mStart(-1), mEnd(0) {}
	inline					Capture (const Capture& r)   { mStart = r.mStart; mEnd = r.mEnd; }
	inline	Capture&		operator= (const Capture& r) { mStart = r.mStart; mEnd = r.mEnd; return *this; }
};

/**
A full-featured regular expression class including zero-width lookaheads and more.
The RegExp class parses its pattern on construction. It can then be executed repeatedly 
with different texts and offsets. The matches are stored in an array of captures supplied
as an argument. Matches may be undefined; in this case, the starting position of a match 
is -1. The class is thread safe.
*/

class SC_API RegExp : public Heap
{
	RegExpData*				mData;			// the compiled regular expression
public:
	/// Possible flags for regular expressions.
	enum : scint32_t {
		/// This flag is ignored here, but stored for completeness.
		kGlobal				= 1,
		/// Ignore Case.
		kNoCase				= 2,
		/// Multiline.
		kMultiLine			= 4,
	};
	/// The default ctor creates an empty RegExp that alway finds its pattern.
							RegExp();
	/// The ctor takes the pattern.
							RegExp (const String& pattern, scint32_t flags);
	///
							RegExp (const RegExp& r);
	///
			RegExp&			operator= (const RegExp& r);
							~RegExp();
	/// Retrieve the original pattern.
			const String&	getPattern() const;
	/// Retrieve the flags.
			scint32_t		getFlags() const;
	/**
	Match the given text against the pattern. Start with the
	current position. If this position is out of range, return false.
	On a successful match, the matchPos points to the start of the match, 
	the current position points behind the match, and the captures array 
	is filled with all matches. Returns true on success.
	@param	text			the text to match
	@param	startPos		the starting position
	@param	captures		the list of captures to be filled in; must be at least getCaptureLength() elements long
	*/
			bool			match (const String& text, scint32_t startPos, Capture& captures) const;
	/**
	Check for a correctly compiled regular expression.
	*/
			bool			good() const;
	/**
	Get the total number of captures, including the empty ones.
	An empty capture is f.ex. (a)? when "a" was not found. Capture 0
	is always the matched string.
	*/
			scint32_t		getCaptureLength() const;
	/**
	Create a capture list of the size getCaptureLength(), initialize it
	with -1 and return it. Use this method to create a capture list of
	of the correct size. Delete the capture list after being finished with it,
    and use operator delete [] to delete the list!
	*/
			Capture*		createCaptureList() const;
};

}	// end Namespace

#include "ScPostConfig.h"
#endif

