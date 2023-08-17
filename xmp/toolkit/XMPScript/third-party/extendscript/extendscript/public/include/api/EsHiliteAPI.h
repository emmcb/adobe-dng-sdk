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
#ifndef _EsHiliteAPI_h
#define _EsHiliteAPI_h

#include "ExtendScript.h"
#include "ScPreConfig.h"

namespace ScScript
{

/// The name of this API

const scint32_t	kHiliteAPI = 0x68696C75; // 'hilt'

class HiliteAPIData;

/**
A range of character for syntax highlighting.
This struct is intended to be used as-is. Do not derive from this struct unless 
your struct also derives from Heap to avoid confusing the memory allocation
algorithms.
*/

struct HiliteRange
{
	/**
	The table of possible color values. The proposed colors correspond to
	the default color settings of the syntax highlighter in GoLive.
	*/
	enum Color
	{
		/// Urecognized token, bad string or the like. Proposed color: red.
		kBad,
		/// Normal text that should not be highlighted. Proposed color: black.
		kText,
		/// A Javascript keyword. Proposed color: bright blue.
		kKeyword,
		/// A number. Proposed color: black.
		kNumber,
		/// A string. Proposed color: brown.
		kString,
		/// A variable, function or other symbol. Proposed color: black.
		kSymbol,
		/// An operator. Proposed color: black.
		kOperator,
		/// A comment. Proposed color: magenta.
		kComment,
		__Color__32bit__	= 0x7FFFFFFF
	};
	/// The start of the range, relative to the start of the supplied string.
	scint32_t		iStart;
	/// The number of Unicode characters to colorize.
	scint32_t		iLen;
	/// The hiliting color.
	Color			iColor;
	/// The error code if this range is in error.
	scerror_t		iError;
};

/**
The hiliting API provides the interface to syntax highlighting. When a text
is to be syntax highlighted, it is passed into the API which in turn sets up
its internal structures and returns a data pointer. Repeated calls to next() 
retrieve the highlighting information. After use, dispose the data pointer
by calling stop().
The parsing begins at a supplied offset and extends through the ending position
defined by offset and length. The parsing stops exactly at the given position, 
even if the location is within a token like f.ex. a keyword.
<p>
<b>Recommended parsing strategy for JavaScript</b>
<p>
For JavaScript syntax highlighting, the following strategy is recommended
if you do not want to reparse the entire source after each change (this may work
well, though, if you only apply the actually changed ranges; the highlighter is
pretty fast). If the change is within a comment, you should reparse from the
changed location thru the end of the comment in case the user typed in a newline
in a single line comment or * and / in a multiline comment. Otherwise, reparsing
the current line will do unless the user typed in a linefeed. In this case, the
two lines split by the linefeed will need reparsing.
*/

class ES_API HiliteAPI : public API
{
	friend class RealEngine;
					HiliteAPI (Engine&);
public:
	/**
	Initiate the highlighting. Supply the string and the offset to start from, 
	and the number of Unicode characters to scan. The given string is parsed from the
	supplied offset for the number of Unicode characters given. If this value
	is < 0, the entire string starting from the offset is parsed. The returned
	data pointer must be deleted after use.
	@param	text			the string to parse
	@param	offset			starting offset (optional)
	@param	len				number of Unicode characters to parse (optional)
	@return					a data pointer
	*/
			HiliteAPIData*	start (const ScCore::String& text, scint32_t offset = 0, scint32_t length = -1);
	/**
	Hilite the next element and fill in the supplied data structure. Return false
	if the end of the input data is reached.
	@param	data			the data pointer returned by start()
	@param	range			the range to fill
	@return					false if no more data
	*/
			bool			next (HiliteAPIData* data, HiliteRange& range);
	/**
	Terminate the highlighting. Call this method to stop highlighting and to
	release the internal data structures.
	@param	data			the data pointer returned by start()
	*/
			void			stop (HiliteAPIData* data);
};

}	// end namespace

#include "ScPostConfig.h"

#endif

