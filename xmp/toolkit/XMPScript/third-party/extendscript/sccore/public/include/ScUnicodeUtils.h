/**************************************************************************
*
* ADOBE CONFIDENTIAL
* ___________________
*
*  Copyright 1998 - 2005 Adobe Systems Incorporated
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
#ifndef _ScUnicodeUtils_h
#define _ScUnicodeUtils_h

#include "ScRoot.h"
#include "ScPreConfig.h"

namespace ScCore
{

#    if defined(__GNUC__)
#        undef iswupper
#        undef iswlower
#        undef iswalpha
#        undef iswalnum
#    endif

extern "C" {
/// Some standard C library functions coded for scutf16_t pointers.
SC_API const scutf16_t* memchr16 (const scutf16_t* chars, scutf16_t ch, int size);
///
SC_API int strcmp16 (const scutf16_t* a, const scutf16_t* b);
///
SC_API int stricmp16 (const scutf16_t* a, const scutf16_t* b);
///
SC_API int strncmp16 (const scutf16_t* a, const scutf16_t* b, scint32_t n);
///
SC_API int strnicmp16 (const scutf16_t* a, const scutf16_t* b, scint32_t n);
///
SC_API const scutf16_t* strstr16 (const scutf16_t* a, const scutf16_t* b);
///
SC_API const scutf16_t* strchr16 (const scutf16_t* p, scutf16_t ch);
///
SC_API int strlen16 (const scutf16_t* p);
///
SC_API void strcpy16 (scutf16_t* dst, const scutf16_t* src);
}

/**
A collection of Unicode (UTF-16) utility functions. Use as needed.
*/

class SC_API UnicodeUtils
{
public:
	/// atol().
	static scint32_t watol (const scutf16_t* p);
	/// Check for a char being Unicode whitespace.
	static bool isWS (scutf16_t ch);
	/// Check for a char being a format control character.
	static bool isCf (scutf16_t ch);
	/// Check for a char being a hyphenation character.
	static bool isPd (scutf16_t ch);
	/// Compare a Unicode string with a plain ASCII string (no conversion), case sensitive.
	static int wcscmpmb (const scutf16_t* a, const char* b);
	/// Compare a Unicode string with a plain ASCII string (no conversion), case insensitive.
	static int wcsicmpmb (const scutf16_t* a, const char* b);
	/// Compare a Unicode string with a plain ASCII string (no conversion) for n bytes, case sensitive.
	static int wcsncmpmb (const scutf16_t* a, const char* b, scint32_t n);
	/// Compare a Unicode string with a plain ASCII string (no conversion) for n bytes, case insensitive.
	static int wcsnicmpmb (const scutf16_t* a, const char* b, scint32_t n);
	/// Case conversion to lower case. Only one-to-one mappings are supported.
	static scutf16_t towlower (scutf16_t ch);
	/// Case conversion to upper case. Only one-to-one mappings are supported.
	static scutf16_t towupper (scutf16_t ch);
	///@name Extended case tests over the entire Unicode range.
	//@{
	///
	static bool iswupper (scutf16_t ch);
	///
	static bool iswlower (scutf16_t ch);
	///
	static bool iswalpha (scutf16_t ch);
	///
	static bool iswalnum (scutf16_t ch);
	//@}
	/**
	Check for an identifier character. This is the dollar sign, 
	the underline, or one of the following Unicode categories:
	<ul>
	<li>Uppercase letter (Lu)
	<li>Lowercase letter (Ll)
	<li>Titlecase letter (Lt)
	<li>Modifier letter (Lm)
	<li>Other letter (Lo)
	<li>Letter number (Nl)
	<li>Non-spacing mark (Mn)
	<li>Combining spacing mark (Mc)
	<li>Decimal number (Nd)
	<li>Connector punctuation (Pc)
	</ul>
	*/
	static bool isSymbolChar (scutf16_t ch);
	/**
	Check for an identifier starting character. This is the dollar sign, 
	the underline, or one of the following Unicode categories:
	<ul>
	<li>Uppercase letter (Lu)
	<li>Lowercase letter (Ll)
	<li>Titlecase letter (Lt)
	<li>Modifier letter (Lm)
	<li>Other letter (Lo)
	<li>Letter number (Nl)
	</ul>
	*/
	static bool isSymbolStart (scutf16_t ch);
};

}	// end Namespace

#include "ScPostConfig.h"
#endif

