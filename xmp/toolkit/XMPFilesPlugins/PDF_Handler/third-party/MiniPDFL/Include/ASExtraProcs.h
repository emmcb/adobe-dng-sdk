/*************************************************************************
* WARNING: All additions to this file must go at the end of the file.
* This file is #included to build a table of function pointers (an HFT).
* All components which make use of this HFT rely on the order and
* positioning of these functions (e.g. plug-ins). Adding entries in the 
* middle of this file or modifying existing entries will cause these 
* components to break.
**************************************************************************/

/*********************************************************************

 ADOBE SYSTEMS INCORPORATED
 Copyright (C) 1999-2006 Adobe Systems Incorporated
 All rights reserved.

 NOTICE: Adobe permits you to use, modify, and distribute this file
 in accordance with the terms of the Adobe license agreement
 accompanying it. If you have received this file from a source other
 than Adobe, then your use, modification, or distribution of it
 requires the prior written permission of Adobe.

 ---------------------------------------------------------------------

 ASExtraProcs.h

 - Catalog of functions exported by the ASExtra HFT.

  WARNING: All additions to this file must go to the end of the
file. ASExtraProcs.h is #included to make a table. Existing
plug-ins were compiled expecting the order of entries in that
table to remain fixed. Adding entries in the middle of this
file will break those plug-ins which use entries in the HFT.

*********************************************************************/


/**
	Converts from an ASScript code to a host encoding type. 
	On Windows, the host encoding is a <code>CHARSET id</code>. On Mac OS, 
	the host encoding is a script code. 
	@param asScript The script value.
	@return The new host encoding type. 
	@see ASScriptFromHostEncoding 
	@since PI_ASEXTRA_VERSION >= 0x00050000
*/
NPROC(ASHostEncoding, ASScriptToHostEncoding, (ASScript asScript))

/**
	Converts from a host encoding type to an ASScript value. 
	On Windows, the host encoding is a <code>CHARSET id</code>. On Mac OS, 
	the host encoding is a script code. 
	@param osScript The host encoding type.
	@return The new ASScript value. 
	@see ASScriptToHostEncoding 
	@since PI_ASEXTRA_VERSION >= 0x00050000
*/
NPROC(ASScript, ASScriptFromHostEncoding, (ASHostEncoding osScript))

/********************************************************************************
 * ASText API calls
 *
 * General Notes
 *
 *   When creating a new ASText object, or putting new data in an
 *   existing object, the implementation will always *copy* the
 *   supplied data into the ASText object. The original data is yours
 *   to do with as you will (and release if necessary).
 *
 *   The size of ASText data is always specified in bytes. For example, the
 *   'len' argument to ASTextFromSizedUnicode specifies the number of
 *   bytes in the string, not the number of Unicode characters.
 *
 *   Host encoding and Unicode strings are always terminated with a
 *   NULL *character* (which consists of one NULL byte for host
 *   encoded strings and two NULL bytes for Unicode strings). You
 *   cannot create a string with an embedded NULL character even using
 *   the calls which take an explicit length parameter.
 *
 *   The 'GetXXX' calls return pointers to data held by the ASText
 *   object. You cannot free or manipulate this data directly. The
 *   'GetXXXCopy' calls return data you can manipulate at will and
 *   that you're responsible for freeing.
 *
 *   The ASText object returned by the 'ASTextFromXXX' calls must be
 *   freed by the client by calling ASTextDestroy.
 *
 ********************************************************************************/


/**
	Creates a new text object containing no text. 
	@return An ASText object. 
	@exception genErrNoMemory 
	@since PI_ASEXTRA_VERSION >= 0x00050000
*/
NPROC(ASText, ASTextNew, (void))

/**
	Creates a new string from a <code>NULL</code>-terminated Unicode string. 
	This string is not expected to have <code>0xFE 0xFF</code> prepended, 
	or country/language identifiers. 
	@param ucs A Unicode string. 
	@param format The Unicode format used by <code>ucs</code>.
	@return An ASText object. 
	@see ASTextFromSizedUnicode 
	@since PI_ASEXTRA_VERSION >= 0x00050000
*/
NPROC(ASText, ASTextFromUnicode, (const ASUTF16Val *ucs, ASUnicodeFormat format))

/**
	Creates a new text object from the specified Unicode string. 
	This string is not expected to have <code>0xFE 0xFF</code> prepended, 
	or country/language identifiers. 

	<p>The string cannot contain an embedded <code>NULL</code> character. </p>

	@param ucs The Unicode string 
	@param format The Unicode format of <code>ucs</code>. 
	@param len The length of <code>ucs</code> in bytes.
	@return An ASText object. 
	@exception genErrBadParm is raised if <code>len < 0</code>.
	@see ASTextFromUnicode 
	@since PI_ASEXTRA_VERSION >= 0x00050000
*/
NPROC(ASText, ASTextFromSizedUnicode, (const ASUTF16Val *ucs, ASUnicodeFormat format, ASTArraySize len))

/**
	Creates a new text object from a <code>NULL</code>-terminated multi-byte 
	string in the specified host encoding. 
	@param str The input string. 
	@param encoding The host encoding.
	@return An ASText object. 
	@see ASTextFromSizedEncoded 
	@since PI_ASEXTRA_VERSION >= 0x00050000
*/
NPROC(ASText, ASTextFromEncoded, (const char *str, ASHostEncoding encoding))

/**
	Creates a new text object from a multi-byte string of the specified length in the 
	specified host encoding. 
	@param str A string. 
	@param len The length in bytes. 
	@param encoding The specified host encoding.
	@return An ASText object. 
	@exception genErrBadParm is raised if <code>len < 0</code>. 
	@see ASTextFromEncoded 
	@since PI_ASEXTRA_VERSION >= 0x00050000
*/
NPROC(ASText, ASTextFromSizedEncoded, (const char *str, ASTArraySize len, ASHostEncoding encoding))

/**
	Creates a new string from a <code>NULL</code>-terminated multi-byte string 
	of the specified script. This is a wrapper around ASTextFromEncoded(); 
	the script is converted to a host encoding using ASScriptToHostEncoding(). 
	
	@param str A string. 
	@param script The specified script.
	@return An ASText object. 
	@see ASTextFromSizedScriptText 
	@since PI_ASEXTRA_VERSION >= 0x00050000
*/
NPROC(ASText, ASTextFromScriptText, (const char *str, ASScript script))

/**
	Creates a new text object from the specified multi-byte 
	string of the specified script. This is a wrapper around 
	ASTextFromEncoded(); the script is converted to a host encoding 
	using ASScriptToHostEncoding(). 
	@param str A string. 
	@param len The length in bytes. 
	@param script The specified script.
	@return An ASText object. 
	@see ASTextFromScriptText 
	@since PI_ASEXTRA_VERSION >= 0x00050000
*/
NPROC(ASText, ASTextFromSizedScriptText, (const char *str, ASTArraySize len, ASScript script))

/**
	Creates a new string from some PDF text taken out of a PDF 
	file. This is either a UTF-16 string with the <code>0xFEFF</code> prepended 
	to the front or a PDFDocEncoding string. In either case 
	the string is expected to have the appropriate <code>NULL</code> termination. 
	If the PDText is in UTF-16, it may have embedded language 
	and country information; this will cause the ASText object 
	to have its language and country codes set to the values 
	found in the string. 
	@param str A string.
	@return An ASText object. 
	@see ASTextFromSizedPDText 
	@since PI_ASEXTRA_VERSION >= 0x00050000
*/
NPROC(ASText, ASTextFromPDText, (const char *str))

/**
	Creates a new string from some PDF text taken out of a PDF 
	file. This is either a UTF-16 string with the <code>0xFEFF</code> prepended 
	to the front or a PDFDocEncoding string. If the PDText is 
	in UTF-16, it may have embedded language and country information; 
	this will cause the ASText object to have its language and 
	country codes set to the values found in the string. The 
	<code>length</code> parameter specifies the size, in bytes, of the string. 
	The string must not contain embedded <code>NULL</code> characters. 
	@param str A string. 
	@param length The length in bytes.
	@return An ASText object. 
	@see ASTextFromPDText 
	@since PI_ASEXTRA_VERSION >= 0x00050000
*/
NPROC(ASText, ASTextFromSizedPDText, (const char *str, ASTArraySize length))

/**
	Frees all memory associated with the text object. 
	@param str IN/OUT A text object. 
	@since PI_ASEXTRA_VERSION >= 0x00050000
*/
NPROC(void, ASTextDestroy, (ASText str))


/**
	Alters an existing string from a <code>NULL</code>-terminated Unicode 
	string. This string is not expected to have <code>0xFE 0xFF</code> prepended 
	or embedded country/language identifiers. 
	@param str (Filled by the method) A string. 
	@param ucsValue A Unicode string. 
	@param format The Unicode format.
	@see ASTextSetSizedUnicode 
	@since PI_ASEXTRA_VERSION >= 0x00050000
*/
NPROC(void, ASTextSetUnicode, (ASText str, const ASUTF16Val *ucsValue, ASUnicodeFormat format))

/**
	Replaces the contents of an existing ASText object with 
	the specified Unicode string. This string is not expected 
	to have <code>0xFE 0xFF</code> prepended or embedded country/language 
	identifiers. 

	<p>The string cannot contain a <code>NULL</code> character. </p>

	@param str (Filled by the method) A string. 
	@param ucsValue A Unicode string. 
	@param format The Unicode format. 
	@param len The length of the string in bytes.
	@see ASTextSetUnicode 
	@since PI_ASEXTRA_VERSION >= 0x00050000
*/
NPROC(void, ASTextSetSizedUnicode, (ASText str, const ASUTF16Val *ucsValue, ASUnicodeFormat format, ASTArraySize len))

/**
	Replaces the contents of an existing ASText object with 
	a <code>NULL</code>-terminated multi-byte string in the specified host 
	encoding. 
	@param str IN/OUT An ASText object to hold the string. 
	@param text IN/OUT A pointer to the text string. 
	@param encoding IN/OUT The type of encoding. 
	@exception genErrBadParm is raised if <code>text</code> is <code>NULL</code>.
	@see ASTextSetSizedEncoded 
	@since PI_ASEXTRA_VERSION >= 0x00050000
*/
NPROC(void, ASTextSetEncoded, (ASText str, const char *text, ASHostEncoding encoding))

/**
	Alters an existing string from a multi-byte string in the 
	specified host encoding and of the specified length. This 
	text does not need to be <code>NULL</code>-terminated, and no <code>NULL</code> (zero) 
	bytes should appear in the characters passed in. 
	@param str IN/OUT A string. 
	@param text IN/OUT A pointer to the text string. 
	@param len IN/OUT The length of the text string. 
	@param encoding IN/OUT The host encoding type. 
	@exception genErrBadParm is raised if <code>text</code> is <code>NULL</code>. 
	@see ASTextSetEncoded 
	@since PI_ASEXTRA_VERSION >= 0x00050000
*/
NPROC(void, ASTextSetSizedEncoded, (ASText str, const char *text, ASTArraySize len, ASHostEncoding encoding))

/**
	Alters an existing string from a <code>NULL</code>-terminated multi-byte 
	string of the specified script. This is a wrapper around 
	ASTextFromEncoded(); the script is converted to a host encoding 
	using ASScriptToHostEncoding(). 
	@param str IN/OUT A string. 
	@param text IN/OUT A pointer to the text string. 
	@param script IN/OUT The writing script. 
	@see ASTextSetSizedScriptText 
	@since PI_ASEXTRA_VERSION >= 0x00050000
*/
NPROC(void, ASTextSetScriptText, (ASText str, const char *text, ASScript script))

/**
	Replaces the contents of an existing ASText object with 
	the specified multi-byte string of the specified script. 
	This is a wrapper around ASTextFromSizedEncoded(); the script 
	is converted to a host encoding using ASScriptToHostEncoding(). 
	
	@param str IN/OUT A string. 
	@param text IN/OUT A pointer to the text string. 
	@param len IN/OUT The length of the text string. 
	@param script IN/OUT The writing script. 
	@exception genErrBadParm is raised if <code>text</code> is <code>NULL</code>. 
	@since PI_ASEXTRA_VERSION >= 0x00050000
*/
NPROC(void, ASTextSetSizedScriptText, (ASText str, const char *text, ASTArraySize len, ASScript script))

/**
	Alters an existing string from some PDF text taken out of 
	a PDF file. This is either a big-endian UTF-16 string with 
	the <code>0xFEFF</code> prepended to the front or a PDFDocEncoding string. 
	In either case the string is expected to have the appropriate 
	<code>NULL</code> termination. If the PDText is in UTF-16, it may have 
	embedded language and country information; this will cause 
	the ASText object to have its language and country codes 
	set to the values found in the string. 
	@param str A string. 
	@param text A text string.
	@see ASTextSetSizedPDText 
	@since PI_ASEXTRA_VERSION >= 0x00050000
*/
NPROC(void, ASTextSetPDText, (ASText str, const char *text))

/**
	Replaces the contents of an existing ASText object with 
	PDF text taken out of a PDF file. This is either a big-endian 
	UTF-16 string with the <code>0xFEFF</code> prepended to the front or 
	a PDFDocEncoding string. In either case the <code>length</code> parameter 
	indicates the number of bytes in the string. The string 
	should not be <code>NULL</code>-terminated and must not contain any <code>NULL</code> 
	characters. If the PDText is in UTF-16, it may have embedded 
	language and country information; this will cause the ASText 
	object to have its language and country codes set to the 
	values found in the string. 
	@param str A string. 
	@param text A pointer to a text string. 
	@param length The length of the text string.
	@see ASTextSetPDText 
	@since PI_ASEXTRA_VERSION >= 0x00050000
*/
NPROC(void, ASTextSetSizedPDText, (ASText str, const char *text, ASTArraySize length))

/**
	Returns a pointer to a string in kUTF16HostEndian format 
	(see ASUnicodeFormat). The memory to which this string points
	is owned by the ASText object, and may not be valid after 
	additional operations are performed on the object. 

	<p>The Unicode text returned will not have <code>0xFE 0xFF</code> prepended 
	or any language or country codes. </p>

	@param str A string.
	@return See above. 
	@see ASTextGetUnicodeCopy 
	@since PI_ASEXTRA_VERSION >= 0x00050000
*/
NPROC(const ASUTF16Val *, ASTextGetUnicode, (ASConstText str))

/**
	Returns a pointer to a <code>NULL</code>-terminated string in the specified 
	Unicode format. The memory to which this string points is 
	owned by the client, which can modify it at will and is responsible 
	for destroying it using ASfree. 

	<p>The Unicode text returned will not have <code>0xFE 0xFF</code> prepended 
	or any language or country codes. </p>

	@param str A string. 
	@param format The Unicode format.
	@return A string copy. The client owns the resulting information. 
	
	@exception genErrNoMemory is raised if memory could not be allocated for the 
	copy. 
	@see ASTextGetUnicode 
	@since PI_ASEXTRA_VERSION >= 0x00050000
*/
NPROC(ASUTF16Val *, ASTextGetUnicodeCopy, (ASConstText str, ASUnicodeFormat format))

/**
	Returns a <code>NULL</code>-terminated string in the given encoding. 
	The memory to which this string points is owned by the ASText 
	object and may not be valid after additional operations 
	are performed on the object. 

	<p>Various exceptions may be raised.</p>

	@param str IN/OUT An ASText object. 
	@param encoding IN/OUT The specified host encoding. 
	@return A pointer to a <code>NULL</code>-terminated string corresponding to the 
	text in <code>str</code>. 
	@see ASTextGetEncodedCopy 
	@since PI_ASEXTRA_VERSION >= 0x00050000
*/
NPROC(const char *, ASTextGetEncoded, (ASConstText str, ASHostEncoding encoding))

/**
	Returns a copy of a string in a specified encoding. 
	@param str An ASText object. 
	@param encoding The specified encoding.
	@return A copy of the text in <code>str</code>. The client owns the resulting 
	information and is responsible for freeing it using ASfree(). 
	
	@exception genErrNoMemory is raised if memory could not be allocated for the 
	copy. 
	@see ASTextGetEncoded 
	@since PI_ASEXTRA_VERSION >= 0x00050000
*/
NPROC(char *, ASTextGetEncodedCopy, (ASConstText str, ASHostEncoding encoding))

/**
	Converts the Unicode string in the ASText object to the 
	appropriate script, and returns a pointer to the converted 
	text. The memory to which it points is owned by the ASText object 
	and must not be altered or destroyed by the client. The 
	memory may also become invalid after subsequent operations 
	are applied to the ASText object. 

	<p>Various exceptions may be raised.</p>

	@param str IN/OUT A string. 
	@param script IN/OUT The writing script. 
	@return A string. 
	@see ASTextGetScriptTextCopy 
	@since PI_ASEXTRA_VERSION >= 0x00050000
*/
NPROC(const char *, ASTextGetScriptText, (ASConstText str, ASScript script))

/**
	Converts the Unicode string in the ASText object to the 
	appropriate script and returns a pointer to the converted 
	text. The memory to which it points is owned by the client, which is 
	responsible for freeing it using ASfree(). 
	@param str A string. 
	@param script A writing script.
	@return A string copy. The client owns the resulting information. 
	
	@exception genErrNoMemory is raised if memory could not be allocated for the 
	copy. 
	@see ASTextGetEncodedCopy 
	@see ASTextGetScriptText 
	@since PI_ASEXTRA_VERSION >= 0x00050000
*/
NPROC(char *, ASTextGetScriptTextCopy, (ASConstText str, ASScript script))

/**
	Returns the text in a form suitable for storage in a PDF 
	file. If the text can be represented using PDFDocEncoding, 
	it is; otherwise it is represented in big-endian UTF-16 
	format with <code>0xFE 0xFF</code> prepended to the front and any country/language 
	codes embedded in an escape sequence right after <code>0xFE 0xFF</code>. 
	
	<p>You can determine if the string is Unicode by inspecting 
	the first two bytes. The Unicode case is used if the string 
	has a language and country code set. The resulting string 
	is <code>NULL</code>-terminated as appropriate. That is, one <code>NULL</code> byte 
	is used for PDFDocEncoding, two are used for UTF-16. </p>

	<p>Various exceptions may be raised.</p>

	@param str A string. 
	@param len The length in bytes of the resulting string, 
	not counting the <code>NULL</code> bytes at the end.
	@return A string copy. The client owns the resulting information 
	and is responsible for freeing it with ASfree(). 
	@since PI_ASEXTRA_VERSION >= 0x00050000
*/
NPROC(char *, ASTextGetPDTextCopy, (ASConstText str, ASTArraySize *len))

/**
	Returns the best host encoding for representing the text. 
	The best host encoding is the one that is least likely to 
	lose characters during the conversion from Unicode to host. 
	If the string can be represented accurately in multiple 
	encodings (for example, it is low-ASCII text that can be 
	correctly represented in any host encoding), ASTextGetBestEncoding() 
	returns the preferred encoding based on the <code>preferredEncoding</code> 
	parameter. 

	<p>Various exceptions may be raised.</p>

	@param str An ASText string. 
	@param preferredEncoding The preferred encoding. There 
	is no default.

	@example 
	<p><code>// If you prefer to use the application's language encoding: </code></p>
	<p><code>ASHostEncoding bestEncoding = ASTextGetBestEncoding(text, AVAppGetLanguageEncoding()); </code></p>
	<p><code>// If you prefer to use the operating system encoding: </code></p>
	<p><code>ASHostEncoding bestEncoding = ASTextGetBestEncoding(text, (ASHostEncoding)PDGetHostEncoding()); </code></p>
	<p><code>// If you want to favor Roman encodings: </code></p>
	<p><code>ASHostEncoding hostRoman = ASScriptToHostEncoding(kASRomanScript);</code></p>
	<p><code>ASHostEncoding bestEncoding = ASTextGetBestEncoding(text, hostRoman);</code></p> 

	@return The text encoding. 
	@see ASTextGetBestScript 
	@since PI_ASEXTRA_VERSION >= 0x00050000
*/
//NPROC(ASHostEncoding, ASTextGetBestEncoding, (ASConstText str, ASHostEncoding preferredEncoding))

/**
	Returns the best host script for representing the text. 
	The functionality is similar to ASTextGetBestEncoding(), with resulting 
	host encoding converted to a script code using ASScriptFromHostEncoding(). 
	
	@param str IN/OUT An ASText string. 
	@param preferredScript IN/OUT The preferred host script. There 
	is no default. 
	@return The best host script. 
	@see ASTextGetBestEncoding 
	@since PI_ASEXTRA_VERSION >= 0x00050000
*/
//NPROC(ASScript, ASTextGetBestScript, (ASConstText str, ASScript preferredScript))

/**
	Retrieves the country associated with an ASText object. 
	
	@param text IN/OUT An ASText object. 
	@return The country code. 
	@see ASTextSetCountry 
	@since PI_ASEXTRA_VERSION >= 0x00050000
*/
NPROC(ASCountryCode, ASTextGetCountry, (ASConstText text))

/**
	Sets the language codes associated with a piece of text. 
	ASText objects can have country and language codes associated 
	with them. These can be explicitly set or parsed from the 
	Unicode form of PDText strings.  
	
	@param text IN/OUT An ASText object. 
	@param country IN/OUT Country code. 
	@see ASTextGetCountry 
	@since PI_ASEXTRA_VERSION >= 0x00050000
*/
NPROC(void, ASTextSetCountry, (ASText text, ASCountryCode country))

/**
	Retrieves the language code associated with an ASText object. 
	@param text An ASText object.
	@return The language code. 
	@see ASTextSetLanguage 
	@since PI_ASEXTRA_VERSION >= 0x00050000
*/
NPROC(ASLanguageCode, ASTextGetLanguage, (ASConstText text))

/**
	Sets the language codes associated with a piece of text. 
	
	@param text IN/OUT An ASText object. 
	@param language IN/OUT The language code. 
	@see ASTextGetLanguage 
	@since PI_ASEXTRA_VERSION >= 0x00050000
*/
NPROC(void, ASTextSetLanguage, (ASText text, ASLanguageCode language))

/**
	Concatenates the <code>from</code> text to the end of the <code>to</code> text, altering 
	<code>to</code> but not <code>from</code>. It does not change the language or country 
	of <code>to</code> unless it has no language or country, in which case 
	it acquires the language and country of <code>from</code>. 
	@param to IN/OUT The encoded text to which <code>from</code> is appended. 
	@param from IN/OUT The encoded text to be appended to <code>to</code>. 
	@since PI_ASEXTRA_VERSION >= 0x00050000
*/
NPROC(void, ASTextCat, (ASText to, ASConstText from))

/**
	Concatenates a series of ASText objects to the end of the 
	<code>to</code> object. Be sure to provide <code>NULL</code> as the last argument 
	to the call. 

	<p>Various exceptions may be raised.</p>

	@param to IN/OUT The ASText object to which the subsequent ASText 
	arguments are concatenated. 
	@since PI_ASEXTRA_VERSION >= 0x00050000
*/
NPROC(void, ASTextCatMany, (ASText to, ...))

/**
	Copies the text in <code>from</code> to <code>to</code>, along with the country and 
	language. 
	@param to IN/OUT The destination text object. 
	@param from IN/OUT The source text object. 
	@since PI_ASEXTRA_VERSION >= 0x00050000
*/
NPROC(void, ASTextCopy, (ASText to, ASConstText from))

/**
	Creates a new ASText object that contains the same text/country/language 
	as the one passed in. 
	@param str A text object.
	@return An ASText object. 
	@exception genErrBadParm is raised if <code>str</code> is <code>NULL</code>. 
	@since PI_ASEXTRA_VERSION >= 0x00050000
*/
NPROC(ASText, ASTextDup, (ASConstText str))

/**
	Compares two ASText objects. This routine can be used to 
	sort text objects using the default collating rules of the 
	underlying operating system before presenting them to the user. The comparison 
	is case-sensitive. The results are suitable for displaying a sorted
	list of strings to the user in his chosen language and according
	to the rules of the platform on which the application is running. The
	results can vary based on the platform and user locale. If you want
	to compare strings in a way that is consistent across locales and
	platforms (but not suitable for displaying sorted strings to a user)
	see ASTextCaseSensitiveCmp().

	<p>Various exceptions may be raised.</p>

	@param str1 The first text object. 
	@param str2 The second text object.
	@return A negative number if <code>str1 < str2</code>, a positive number 
	if <code>str1 > str2</code>, and <code>0</code> if they are equal. 
	@see ASTextCaseSensitiveCmp 
	@since PI_ASEXTRA_VERSION >= 0x00050000
*/
NPROC(ASInt32, ASTextCmp, (ASConstText str1, ASConstText str2))

/**
	Replaces all occurrences of <code>toReplace</code> in <code>src</code> with the text 
	specified in <code>replacement</code>. This uses an ASText string 
	to indicate the <code>toReplace</code> string; ASTextReplaceASCII() uses 
	a low ASCII Roman string to indicate the text to replace. 

	<p>Various exceptions may be raised.</p>

	@param src Source text. 
	@param toReplace Text in source text to replace. 
	@param replacement Text used in replacement.
	@see ASTextReplaceASCII 
	@see ASTextReplaceBadChars 
	@since PI_ASEXTRA_VERSION >= 0x00050000
*/
NPROC(void, ASTextReplace, (ASText src, ASConstText toReplace, ASConstText replacement))

/**
	Replaces all occurrences of <code>toReplace</code> in <code>src</code> with the text 
	specified in <code>replacement</code>. ASTextReplace() uses an ASText string 
	to indicate the toReplace string; this uses 
	a low-ASCII Roman string to indicate the text to replace. 
	
	<p>This call is intended for formatting strings for the user 
	interface. For example, it can be used for replacing a known sequence such as 
	<code>'%1'</code> with other text. Be sure to use only low ASCII characters, 
	which are safe on all platforms. Avoid using backslash and 
	currency symbols.</p> 

	<p>Various exceptions may be raised.</p>

	@param src The ASText object containing the text. 
	@param toReplace The text to replace. 
	@param replacement The replacement text.
	@see ASTextReplace 
	@see ASTextReplaceBadChars 
	@since PI_ASEXTRA_VERSION >= 0x00050000
*/
NPROC(void, ASTextReplaceASCII, (ASText src, const char *toReplace, ASConstText replacement))

#if ADD_ASCAB //PKG
/**
	Creates a new, empty cabinet. 
	@return The newly created cabinet. 
	@exception genErrNoMemory 
	@since PI_ASEXTRA_VERSION >= 0x00050000
*/
NPROC(ASCab, ASCabNew, (void))

/**
	Builds a cabinet based on a constant array of ASCabDescriptor 
	records (see ASCabEntryRec). The first entry in each descriptor 
	specifies the name of the key; subsequent fields contain 
	the value. The entry list must end with a descriptor containing 
	<code>NULL</code> for the key name. See ASExtraExpT.h for more info. 
	
	@param entryList A constant array of ASCabDescriptor records 
	(see ASCabEntryRec). Passing <code>NULL</code> generates an empty ASCab.
	@return The newly created ASCab. 
	@exception genErrBadParm 
	@since PI_ASEXTRA_VERSION >= 0x00050000
*/
NPROC(ASCab, ASCabFromEntryList, (const ASCabEntryRec *entryList))

/**
	Destroys the cabinet and all its key/value pairs. This method 
	raises an exception if the cabinet is the value for some key in another 
	cabinet. 
	@param theCab The cabinet.
	@exception genErrBadParm 
	@since PI_ASEXTRA_VERSION >= 0x00050000
*/
NPROC(void, ASCabDestroy, (ASCab theCab))

/**
	Returns the number of key/value pairs in <code>theCab</code>. 
	@param theCab The cabinet.
	@return See above. 
	@exception genErrBadParm 
	@since PI_ASEXTRA_VERSION >= 0x00050000
*/
NPROC(ASTArraySize, ASCabNumEntries, (ASConstCab theCab))

/**
	Returns <code>true</code> if <code>theKey</code> is present in <code>theCab</code>. 
	@param theCab IN/OUT The cabinet. 
	@param theKey IN/OUT The key name. 
	@return See above. 
	@exception genErrBadParm 
	@since PI_ASEXTRA_VERSION >= 0x00050000
*/
NPROC(ASBool, ASCabKnown, (ASConstCab theCab, const char *theKey))

/**
	Returns the type of value stored under <code>theKey</code> in <code>theCab</code>. 
	
	@param theCab IN/OUT The cabinet. 
	@param theKey IN/OUT The key name. 
	@return The type of value stored under <code>theKey</code>, or kASValueUnknown 
	if the key is not found. 
	@exception genErrBadParm 
	@since PI_ASEXTRA_VERSION >= 0x00050000
*/
NPROC(ASCabValueType, ASCabGetType, (ASConstCab theCab, const char *theKey))

/**
	Enumerates all the keys in the cabinet. 

	<p>Keys consisting solely of digits are enumerated first, in 
	numeric order (assuming they are not padded with zeros at 
	the front, which will confuse matters). Non-numeric keys 
	are then enumerated in <code>strcmp</code> order. </p>

	<p>It is safe to add, delete, and modify items in <code>theCab</code> during 
	the enumeration. Items that are added during the enumeration 
	will not be enumerated. Modified items that have been enumerated 
	already will not be enumerated again. Deleted items that 
	have not yet been enumerated will not be enumerated. </p>

	@param theCab The cabinet. 
	@param enumProc A user-supplied callback that is called 
	for each entry found in <code>theCab</code>. 
	@param clientData A pointer to user-supplied data to pass 
	to <code>enumProc</code> each time it is called.
	@exception genErrBadParm 
	@note This will <code>RERAISE</code> any exceptions thrown by <code>enumProc</code>. 
	@see ASConstCabEnum 
	@ingroup Enumerators
	@since PI_ASEXTRA_VERSION >= 0x00050000
*/
NPROC(void, ASCabEnum, (ASCab theCab, ASCabEnumProc enumProc, void *clientData))

/* ==================================================== */
/* Routines for storing and accessing key/value pairs.
 *
 * The ASCabinet "owns" all the data inside it (except for
 * pointers, which are handled separately). When a key is removed from
 * the cabinet the ASCabinet destroys the associated value.
 * Similarly, every time you place a value inside a cabinet you are
 * handing that value to the ASCabinet implementation to manage;
 * putting a value in a cabinet is always a hand-off operation. For
 * example, if you create an ASText object and add it as a value in an
 * ASCabinet the ASText object is no longer "owned" by you; it's
 * owned by the ASCabinet. The ASCabinet will destroy the ASText
 * object when it's associated key is removed or its value is
 * over-written.
 *
 * When adding a new value to an ASCabinet, the client may specify the
 * key to be <code>NULL</code>, in which case the ASCabinet will create a unique
 * key. The generated key will be a string representation of an
 * integer that is guaranteed to be greater than the number of items
 * in the ASCabinet prior to adding the new value.
 *
 * The routine naming convention is as follows:
 *
 * "Get" routines return a value. These objects are owned by the
 * ASCabinet and should not be altered or destroyed by the caller
 * of "Get".
 *
 * "GetCopy" routines make a copy of the data; the "GetCopy" client
 * owns the resulting information and can do whatever they want with
 * it.
 *
 * "Detach" routines work the same way as "Get" routines but the key
 * is removed from the ASCabinet without destroying the associated
 * value which is passed back to the client of "Detach".
 *
 * Pointers are handled separately. Normally pointers are treated the
 * same way as integers; the ASCabinet passes the pointer value
 * back and forth but doesn't "own" the data pointed to. This all
 * changes if the pointer has an associated 'destroyProc'. If the
 * 'destroyProc' is set the ASCabinet will reference count the
 * pointer to track how many times the pointer is referenced from any
 * ASCab (e.g. the reference count will be bumped up whenever the
 * pointer is copied via ASCabCopy) and will destroy the data
 * associated with the pointer when the ref count goes to 0. The data
 * is destroyed by calling the 'destroyProc' to destroy it. This makes
 * detaching a pointer somewhat more complicated. Detaching a pointer
 * removes one reference to the pointer (without every destroying the
 * information pointed to). ASCabDetachPointer returns a separate
 * value indicating whether the pointer can safely be destroyed or is
 * still referred to by other key/value pairs inside ASCabinets.
 *
 */


/**
	Removes <code>theKey</code> entry from <code>theCab</code>, destroying the associated 
	value. 
	@param theCab IN/OUT The cabinet. 
	@param theKey IN/OUT The key name. 
	@exception genErrBadParm 
	@since PI_ASEXTRA_VERSION >= 0x00050000
*/
NPROC(void, ASCabRemove, (ASCab theCab, const char *theKey))


/**
	Returns the ASBool value stored under <code>theKey</code> in <code>theCab</code>. 
	
	@param theCab IN/OUT The cabinet. 
	@param theKey IN/OUT The key name. 
	@param defValue IN/OUT The default value. 
	@return The ASBool value stored under <code>theKey</code> if the key is found 
	and the value stored under it is of type kASValueBool; otherwise 
	<code>defValue</code> is returned. 
	@exception genErrBadParm 
	@see ASCabPutBool 
	@since PI_ASEXTRA_VERSION >= 0x00050000
*/
NPROC(ASBool, ASCabGetBool, (ASConstCab theCab, const char *theKey, ASBool defValue))

/**
	Stores an ASBool value in theCab under theKey. 
	@param theCab IN/OUT The cabinet. 
	@param theKey IN (May be <code>NULL</code>) The key name. 
	@param theBool IN The value to be stored.  Non-zero values are stored as <code>true</code>.
	@exception genErrBadParm 
	@see ASCabGetBool 
	@since PI_ASEXTRA_VERSION >= 0x00050000
*/
NPROC(void, ASCabPutBool, (ASCab theCab, const char *theKey, ASBool theBool))


/**
	Returns the ASInt32 value stored under <code>theKey</code> in <code>theCab</code>. 
	
	@param theCab IN/OUT The cabinet. 
	@param theKey IN/OUT The key name. 
	@param defValue IN/OUT The default value. 
	@return The ASInt32 value stored under <code>theKey</code> if the key is found 
	and the value stored under it is of type kASValueInteger; 
	otherwise <code>defValue</code> is returned. 
	@exception genErrBadParm 
	@see ASCabPutInt 
	@since PI_ASEXTRA_VERSION >= 0x00050000
*/
NPROC(ASInt32, ASCabGetInt, (ASConstCab theCab, const char *theKey, ASInt32 defValue))

/**
	Stores an ASInt32 value in <code>theCab</code> under <code>theKey</code>. 
	@param theCab IN/OUT The cabinet. 
	@param theKey IN/OUT (May be <code>NULL</code>) The key name. 
	@param theInt IN/OUT The value to be stored. 
	@exception genErrBadParm 
	@see ASCabGetInt 
	@since PI_ASEXTRA_VERSION >= 0x00050000
*/
NPROC(void , ASCabPutInt, (ASCab theCab, const char *theKey, ASInt32 theInt))


/**
	Returns the ASAtom value stored under <code>theKey</code> in <code>theCab</code>. 
	
	@param theCab IN/OUT The cabinet. 
	@param theKey IN/OUT The key name. 
	@param defValue IN/OUT The default value. 
	@return The ASAtom value stored under <code>theKey</code> if the key is found 
	and the value stored under it is of type kASValueAtom; otherwise 
	<code>defValue</code> is returned. 
	@exception genErrBadParm 
	@see ASCabPutAtom 
	@since PI_ASEXTRA_VERSION >= 0x00050000
*/
NPROC(ASAtom, ASCabGetAtom, (ASConstCab theCab, const char *theKey, ASAtom defValue))

/**
	Stores an ASAtom value in <code>theCab</code> under <code>theKey</code>. 
	@param theCab IN/OUT The cabinet. 
	@param theKey IN/OUT (May be <code>NULL</code>) The key name. 
	@param atomValue IN/OUT The value to be stored. 
	@exception genErrBadParm 
	@see ASCabGetAtom 
	@since PI_ASEXTRA_VERSION >= 0x00050000
*/
NPROC(void, ASCabPutAtom, (ASCab theCab, const char *theKey, ASAtom atomValue))


/**
	Returns the <code>double</code> value stored under <code>theKey</code> in <code>theCab</code>. 
	If the value stored under <code>theKey</code> is of type kASValueInteger, 
	this value will be cast to a <code>double</code> and returned to the 
	client. 
	@param theCab IN/OUT The cabinet. 
	@param theKey IN/OUT The key name. 
	@param defValue IN/OUT The default value. 
	@return The <code>double</code> value stored under <code>theKey</code> if the key is found 
	and the value stored under it is of type kASValueDouble	or kASValueInteger; otherwise <code>defValue</code> is returned. 
	@exception genErrBadParm 
	@see ASCabPutDouble 
	@since PI_ASEXTRA_VERSION >= 0x00050000
*/
NPROC(double, ASCabGetDouble, (ASConstCab theCab, const char *theKey, double defValue))

/**
	Stores a <code>double</code> value in <code>theCab</code> under <code>theKey</code>. 
	@param theCab IN/OUT The cabinet. 
	@param theKey IN/OUT (May be <code>NULL</code>) The key name. 
	@param floatValue IN/OUT The value to be stored. 
	@exception genErrBadParm 
	@see ASCabGetDouble 
	@since PI_ASEXTRA_VERSION >= 0x00050000
*/
NPROC(void, ASCabPutDouble, (ASCab theCab, const char *theKey, double floatValue))

/* ASCabGetPointer
** ASCabDetachPointer
** ASCabPutPointer
** ASCabGetPointerDestroyProc
**
** Pointers are always 'typed', in that they always have associated with them
** a string indicating what type they are supposed to point to. For example,
** to place a pointer to an AVDoc in a cabinet use:
**
**	ASCabPutPointer(theCab, "BlahBlah", AVDoc, theDoc);
**
** and to retrieve it use:
**
**  theDoc = ASCabGetPointer(theCab, "BlahBlah", AVDoc);
**
** In both cases the AVDoc type passed in is used to explicitly denote
** the pointer's type (and to correctly cast the return result of
** ASCabGetPointer).
**
** You can also associate a destroyProc with a pointer. When all
** copies of the pointer contained in ASCabs are removed the
** destroyProc will be called to free the object it points to. When
** detaching a pointer using ASCabDetachPointer the 'noRefs' param
** will be set to true if you just removed the last copy of the
** pointer and 'false' if some ASCab still contains a copy of the
** pointer.
**
** Note: it is valid to pass <code>NULL</code> in for 'noRefs'.
**
** Note: calling ASCabPutPointer with a <code>NULL</code> pointer removes the key
** from the cabinet.
*/

/* !! Don't call the RAW forms of these routines. Use
   the macros at the top of this file.
*/

/**
	Returns the pointer value stored under <code>theKey</code> in <code>theCab</code>.
	@param theCab The cabinet. 
	@param theKey The key name.
	@param expectedType The data type referenced by the pointer. Since
	ASCabGetPointer() is actually a macro, you should pass the type
	as a literal name, not a string. For example, use <code>PDDoc</code> instead of <code>"PDDoc"</code>.
	Pointers are always <i>typed</i>, in that they always have associated with them
	a string indicating the type to which they point.
	@return The pointer value stored under <code>theKey</code> if the key is found, 
	the value stored under <code>theKey</code> is of type kASValuePointer, and the type
	of the pointer matches <code>expectedType</code>; otherwise <code>NULL</code> is returned. The
	object referenced by this pointer is owned by <code>theCab</code> and should not be
	destroyed by the client.
	@exception genErrBadParm 
	@see ASCabDetachPointer
	@see ASCabPutPointer
	@since PI_ASEXTRA_VERSION >= 0x00050000
*/
NPROC(void *, ASCabGetPointerRaw, (
	ASConstCab theCab, const char *theKey, const char *expectedType))

/**
	Retrieves the pointer stored under <code>theKey</code> in <code>theCab</code> and removes the
	key from <code>theCab</code>. If <code>noRefs</code> is set to <code>true</code>, the client assumes ownership
	of the data referenced by the pointer and is responsible for deallocating
	any resources associated with it.
	@param theCab The cabinet. 
	@param theKey The key name.
	@param expectedType The data type referenced by the pointer. Since
	ASCabGetPointer() is actually a macro, you should pass the type
	as a literal name, not a string. For example, use <code>PDDoc</code> instead of <code>"PDDoc"</code>.
	Pointers are always <i>typed</i>, in that they always have associated with
	them a string indicating the type to which they point.
	@param noRefs (Filled by the method, may be <code>NULL</code>) If non-<code>NULL</code>, a value
	of <code>true</code> indicates that there are no other ASCab objects that reference this
	pointer, and a value of <code>false</code> indicates that some ASCab object still contains
	a copy of the pointer.
	@return The pointer value stored under <code>theKey</code>. It will be <code>NULL</code> if <code>theKey</code> is
	not present in <code>theCab</code>, the value stored under <code>theKey</code> is not of type
	kASValuePointer, or the type of the pointer does not match <code>expectedType</code>.
	@exception genErrBadParm 
	@see ASCabGetPointer
	@see ASCabPutPointer
	@since PI_ASEXTRA_VERSION >= 0x00050000
*/

NPROC(void *, ASCabDetachPointerRaw, (
	ASCab theCab, const char *theKey, const char *expectedType, ASBool *noRefs))

/**
	Stores a pointer in <code>theCab</code> under <code>theKey</code>. See the ASCab description for more information.
	@param theCab The cabinet. 
	@param theKey (May be <code>NULL</code>) The key name.
	@param theType The data type referenced by the pointer. Since
	ASCabGetPointer() is actually a macro, you should pass the type
	as a literal name, not a string. For example, use <code>PDDoc</code> instead of <code>"PDDoc"</code>.
	Pointers are always <i>typed</i>, in that they always have associated with
	them a string indicating the type to which they point.
	@param thePtr The value to be stored. 
	@param destroy (May be <code>NULL</code>) A user-supplied callback which is
	called when the reference count associated with <code>thePtr</code> is zero.
	@exception genErrBadParm
	@exception genErrNoMemory
	@see ASCabGetPointer
	@see ASCabDetachPointer
	@since PI_ASEXTRA_VERSION >= 0x00050000
*/	
NPROC(void, ASCabPutPointerRaw, (
	ASCab theCab, const char *theKey, const char *theType,
	void *thePtr, ASCabPointerDestroyProc destroy))

/**
	Obtains the resource deallocation callback associated with 
	the pointer stored under <code>theKey</code> in <code>theCab</code>. When the reference 
	count of the pointer falls to zero, the callback is called 
	to free the resources associated with the object it references. 
	
	@param theCab IN/OUT The cabinet. 
	@param theKey IN/OUT The key name. 
	@return The callback (if any) associated with the pointer if the 
	key is found and the value stored under it is of type kASValuePointer; 
	otherwise <code>NULL</code> is returned. 
	@exception genErrBadParm 
	@see ASCabDetachPointer 
	@since PI_ASEXTRA_VERSION >= 0x00050000
*/
NPROC(ASCabPointerDestroyProc, ASCabGetPointerDestroyProc, (
	ASConstCab theCab, const char *theKey))

/**
	Returns a string representation of the data type referenced 
	by the pointer stored under <code>theKey</code> in <code>theCab</code>. 
	@param theCab IN/OUT The cabinet. 
	@param theKey IN/OUT The key name. 
	@return The string if the key is found and the value stored under 
	it is of type kASValuePointer; otherwise <code>NULL</code> is returned. 
	
	@exception genErrBadParm 
	@since PI_ASEXTRA_VERSION >= 0x00050000
*/
NPROC(const char *, ASCabGetPointerType, (
	ASConstCab theCab, const char *theKey))


/**
	Stores an ASCab in <code>theCab</code> under <code>theKey</code>. If the cabinet is 
	already a value for some other ASCab, ASCabPutCab() will raise an exception, 
	since any cabinet can be contained by at most one other 
	cabinet. 

	<p><code>theCab</code> assumes ownership of the cabinet, so the client must 
	not destroy it.</p> 
	@param theCab IN/OUT The cabinet. 
	@param keyName IN/OUT (May be <code>NULL</code>) The key name. 
	@param putCab IN/OUT (May be <code>NULL</code>) The ASCab to be stored in <code>theCab</code>. 
	If <code>cabVal</code> is <code>NULL</code>, then any value under <code>theKey</code> is destroyed 
	and <code>theKey</code> is removed from <code>theCab</code>. 
	@exception genErrBadParm 
	@see ASCabGetCab 
	@see ASCabDetachCab 
	@since PI_ASEXTRA_VERSION >= 0x00050000
*/
NPROC(void, ASCabPutCab, (ASCab theCab, const char *keyName, ASCab putCab))

/**
	Returns the ASCab stored under <code>theKey</code> in <code>theCab</code>. 
	@param theCab IN/OUT The cabinet. 
	@param theKey IN/OUT The key name. 
	@return The ASCab stored under <code>theKey</code> if the key is found and the 
	value stored under it is of type kASValueCabinet; otherwise 
	<code>NULL</code> is returned. This object is owned by <code>theCab</code> and should 
	not be destroyed by the client. 
	@exception genErrBadParm 
	@see ASCabPutCab 
	@see ASCabDetachCab 
	@since PI_ASEXTRA_VERSION >= 0x00050000
*/
NPROC(ASCab, ASCabGetCab, (ASConstCab theCab, const char *theKey))

/**
	Retrieves the ASCab stored under <code>theKey</code> in <code>theCab</code> and removes 
	the key from <code>theCab</code>. 

	<p>The client assumes ownership of the ASCab returned and is 
	responsible for destroying it. </p>
	@param theCab IN/OUT The cabinet. 
	@param theKey IN/OUT The key name. 
	@return The cabinet. Will be <code>NULL</code> if <code>theKey</code> is not present in <code>theCab</code>, 
	or if the value stored under <code>theKey</code> is not of type kASValueCabinet. 
	
	@exception genErrBadParm 
	@see ASCabPutCab 
	@see ASCabGetCab 
	@since PI_ASEXTRA_VERSION >= 0x00050000
*/
NPROC(ASCab, ASCabDetachCab, (ASCab theCab, const char *theKey))


/**
	Returns the string stored under <code>theKey</code> in <code>theCab</code>. 
	@param theCab IN/OUT The cabinet. 
	@param theKey IN/OUT The key name. 
	@return The string stored under <code>theKey</code> if the key is found and the 
	value stored under it is of type kASValueString; otherwise 
	<code>NULL</code> is returned. The object referenced by this pointer 
	is owned by <code>theCab</code> and should not be destroyed by the client. 
	
	@exception genErrBadParm 
	@exception genErrNoMemory 
	@see ASCabGetStringCopy 
	@see ASCabDetachString 
	@see ASCabPutString 
	@since PI_ASEXTRA_VERSION >= 0x00050000
*/
NPROC(const char *, ASCabGetString, (ASConstCab theCab, const char *theKey))

/**
	Returns a copy of the string stored under <code>theKey</code> in <code>theCab</code>. 
	
	<p>It is the client's responsibility to release the memory 
	allocated for the string using ASfree(). </p>
	@param theCab IN/OUT The cabinet. 
	@param theKey IN/OUT The key name. 
	@return A copy of the string stored under <code>theKey</code> if the key is found 
	and the value stored under it is of type kASValueString; 
	otherwise <code>NULL</code> is returned. 
	@exception genErrBadParm 
	@exception genErrNoMemory 
	@see ASCabGetString 
	@see ASCabDetachString 
	@see ASCabPutString 
	@since PI_ASEXTRA_VERSION >= 0x00050000
*/
NPROC(char *, ASCabGetStringCopy, (ASConstCab theCab, const char *theKey))

/**
	Retrieves the string stored under <code>theKey</code> in <code>theCab</code> and removes 
	the key from <code>theCab</code>. 

	<p>The client assumes ownership of the string and is responsible 
	for deallocating any resources associated with it. </p>
	@param theCab IN/OUT The cabinet. 
	@param theKey IN/OUT The key name. 
	@return The string stored under <code>theKey</code>. Will be <code>NULL</code> if <code>theKey</code> is 
	not present in <code>theCab</code>, or if the value stored under <code>theKey</code> 
	is not of type kASValueString. 
	@exception genErrBadParm 
	@see ASCabGetString 
	@see ASCabGetStringCopy 
	@see ASCabPutString 
	@since PI_ASEXTRA_VERSION >= 0x00050000
*/
NPROC(char *, ASCabDetachString, (ASCab theCab, const char *theKey))

/**
	Stores a string in <code>theCab</code> under <code>theKey</code>. A string consists 
	of some number of bytes followed by a single <code>NULL</code> (zero) 
	byte. The string must have been allocated using ASmalloc(). 

	<p><code>theCab</code> assumes ownership of the string, so the client should 
	not attempt to free the memory associated with it. </p>
	@param theCab IN/OUT The cabinet. 
	@param theKey IN/OUT (May be <code>NULL</code>) The key name. 
	@param theStr IN/OUT (May be <code>NULL</code>) The string to be stored. 
	If <code>NULL</code>, the value (if any) stored under <code>theKey</code> in <code>theCab</code> 
	is destroyed and <code>theKey</code> is removed from <code>theCab</code>. 
	@exception genErrBadParm 
	@see ASCabGetString 
	@see ASCabGetStringCopy 
	@see ASCabDetachString 
	@since PI_ASEXTRA_VERSION >= 0x00050000
*/
NPROC(void, ASCabPutString, (ASCab theCab, const char *theKey, const char *theStr))


/**
	Returns the ASText object stored under <code>theKey</code> in <code>theCab</code>. 
	
	@param theCab IN/OUT The cabinet. 
	@param theKey IN/OUT The key name. 
	@return The ASText object stored under <code>theKey</code> if the key is found 
	and the value stored under it is of type kASValueText; otherwise 
	<code>NULL</code> is returned. This object is owned by <code>theCab</code> and should 
	not be destroyed by the client. 
	@exception genErrBadParm 
	@see ASCabDetachText 
	@see ASCabPutText 
	@since PI_ASEXTRA_VERSION >= 0x00050000
*/
NPROC(ASText, ASCabGetText, (ASConstCab theCab, const char *theKey))

/**
	Retrieves the ASText object stored under <code>theKey</code> in <code>theCab</code> 
	and removes the key from <code>theCab</code>. 

	<p>The client assumes ownership of the ASText object and is 
	responsible for deallocating it using ASTextDestroy(). </p>
	@param theCab The cabinet. 
	@param theKey The key name.
	@return The ASText object stored under <code>theKey</code>. It will be <code>NULL</code> if <code>theKey</code> 
	is not present in <code>theCab</code>, or if the value stored under <code>theKey</code> 
	is not of type kASValueText. 
	@exception genErrBadParm 
	@see ASCabGetText 
	@see ASCabPutText 
	@since PI_ASEXTRA_VERSION >= 0x00050000
*/
NPROC(ASText, ASCabDetachText, (ASCab theCab, const char *theKey))

/**
	Stores an ASText object in <code>theCab</code> under <code>theKey</code>. 

	<p><code>theCab</code> assumes ownership of the object, so the client should 
	not attempt to free the memory associated with it. </p>
	@param theCab IN/OUT The cabinet. 
	@param theKey IN/OUT (May be <code>NULL</code>) The key name. 
	@param theText IN/OUT (May be <code>NULL</code>) The object to be stored. If its value is 
	<code>NULL</code>, the value (if any) stored under <code>theKey</code> in <code>theCab</code> is 
	destroyed and <code>theKey</code> is removed from <code>theCab</code>. 
	@exception genErrBadParm 
	@see ASCabGetText 
	@see ASCabDetachText 
	@since PI_ASEXTRA_VERSION >= 0x00050000
*/
NPROC(void, ASCabPutText, (ASCab theCab, const char *theKey, ASText theText))


/**
	Returns the binary object stored under <code>theKey</code> in <code>theCab</code>. 
	
	@param theCab IN/OUT The cabinet. 
	@param theKey IN/OUT The key name. 
	@param numBytes IN/OUT (Filled by the method, may be <code>NULL</code>) If it is 
	not <code>NULL</code>, it contains the size (in bytes) of the object returned. 
	
	@return The binary object stored under <code>theKey</code> if the key is found 
	and the value stored under it is of type kASValueBinary; 
	otherwise <code>NULL</code> is returned. This object is owned by the 
	ASCab and should not be destroyed by the caller. 
	@exception genErrBadParm 
	@see ASCabGetBinaryCopy 
	@see ASCabDetachBinary 
	@see ASCabPutBinary 
	@since PI_ASEXTRA_VERSION >= 0x00050000
*/
NPROC(const void *, ASCabGetBinary, (ASConstCab theCab, const char *theKey, ASTArraySize *numBytes))

/**
	Returns a copy of the binary object stored under <code>theKey</code> 
	in <code>theCab</code>. 
	
	<p>It is the client's responsibility to release 
	the memory associated with the object using ASfree(). </p>
	@param theCab IN/OUT The cabinet. 
	@param theKey IN/OUT The key name. 
	@param numBytes IN/OUT (Filled by the method, may be <code>NULL</code>) If it is
	not <code>NULL</code>, it contains the size of the object returned. 
	@return The binary object stored under <code>theKey</code> if the key is found 
	and the value stored under it is of type kASValueBinary; 
	otherwise <code>NULL</code> is returned. 
	@exception genErrBadParm 
	@exception genErrNoMemory 
	@see ASCabGetBinary 
	@see ASCabDetachBinary 
	@see ASCabPutBinary 
	@since PI_ASEXTRA_VERSION >= 0x00050000
*/
NPROC(void *, ASCabGetBinaryCopy, (ASConstCab theCab, const char *theKey, ASTArraySize *numBytes))

/**
	Retrieves the binary object stored under <code>theKey</code> in <code>theCab</code> 
	and removes the key from <code>theCab</code>. 

	<p>The client assumes ownership of the object and is responsible 
	for deallocating any resources associated with it. </p>

	@param theCab IN/OUT The cabinet. 
	@param theKey IN/OUT The key name. 
	@param numBytes IN/OUT (Filled by the method, may be <code>NULL</code>) If it is 
	not <code>NULL</code>, it contains the size (in bytes) of the object retrieved. 
	@return A pointer to the binary object. It will be <code>NULL</code> if <code>theKey</code> is 
	not present in <code>theCab</code> or if the value stored under <code>theKey</code> 
	is not of type kASTypeBinary. 
	@exception genErrBadParm 
	@see ASCabGetBinary 
	@see ASCabGetBinaryCopy 
	@see ASCabPutBinary 
	@since PI_ASEXTRA_VERSION >= 0x00050000
*/
NPROC(void *, ASCabDetachBinary, (ASCab theCab, const char *theKey, ASTArraySize *numBytes))

/**
	Stores a binary object in <code>theCab</code> under <code>theKey</code>. The ASCab 
	assumes ownership of the binary object, so the client should 
	not attempt to free the memory associated with it. The binary 
	object must have been allocated using ASmalloc(). 
	@param theCab IN/OUT The cabinet. 
	@param theKey IN/OUT (May be <code>NULL</code>) The key name. 
	@param theBlob IN/OUT (May be <code>NULL</code>) A pointer to the binary object 
	to be stored. If it is <code>NULL</code>, the value (if any) stored under <code>theKey</code> 
	in <code>theCab</code> is destroyed and <code>theKey</code> removed from <code>theCab</code>. 
	@param blobSize IN/OUT The size of the binary object. 
	@exception genErrBadParm 
	@see ASCabGetBinary 
	@see ASCabGetBinaryCopy 
	@see ASCabDetachBinary 
	@since PI_ASEXTRA_VERSION >= 0x00050000
*/
NPROC(void, ASCabPutBinary, (
	ASCab theCab, const char *theKey, void *theBlob, ASTArraySize blobSize))


/**
	Stores a value with a type of kASValueNull in <code>theCab</code> under 
	<code>theKey</code>. <code>NULL</code> cabinet entries are used as placeholders or 
	to removed other cabinet entries during an ASCabMunge operation. 
	
	@param theCab IN/OUT The cabinet. 
	@param theKey IN/OUT The key name. 
	@exception genErrBadParm 
	@since PI_ASEXTRA_VERSION >= 0x00050000
*/
NPROC(void, ASCabPutNull, (ASCab theCab, const char *theKey))


/**
	Removes all keys from <code>theCab</code> and destroys all values they 
	point to. 
	@param theCab IN/OUT The cabinet. 
	@exception genErrBadParm 
	@since PI_ASEXTRA_VERSION >= 0x00050000
*/
NPROC(void, ASCabMakeEmpty, (ASCab theCab))


/**
	Finds any empty cabinets in <code>theCab</code>, removes their corresponding 
	keys, and destroys them. 
	@param theCab The cabinet. 
	@param recurse <code>true</code> to recurse through all sub-cabinets 
	inside <code>theCab</code>; <code>false</code> to limit enumeration to key/value pairs 
	directly inside <code>theCab</code>.
	@exception genErrBadParm 
	@since PI_ASEXTRA_VERSION >= 0x00050000
*/
NPROC(void, ASCabDestroyEmpties, (ASCab theCab, ASBool recurse))


/**
	For each key/value pair in <code>srcCab</code> a copy of the key/value 
	pair will be placed into <code>dstCab</code>, possibly overwriting any 
	identically named key/value pair in <code>dstCab</code>. If the value 
	being copied is a pointer with an associated <code>destroyProc</code>, 
	the pointer and its type string, but not the data it points 
	to, will be copied and an internal reference count will be incremented. 
	
	@param srcCab The source cabinet. 
	@param dstCab The destination cabinet.
	@exception genErrBadParm 
	@exception genErrNoMemory 
	@see ASCabDup 
	@since PI_ASEXTRA_VERSION >= 0x00050000
*/
NPROC(void, ASCabCopy, (ASConstCab srcCab, ASCab dstCab))

/**
	Creates a new ASCab and populates it with copies of the 
	key/value pairs in <code>srcCab</code>. It is equivalent to <code>ASCabCopy( srcCab, ASCabNew () )</code>. 
	@param srcCab The source cabinet.
	@return The newly created ASCab. 
	@exception genErrBadParm 
	@exception genErrNoMemory 
	@see ASCabCopy 
	@since PI_ASEXTRA_VERSION >= 0x00050000
*/
NPROC(ASCab, ASCabDup, (ASConstCab srcCab))


/**
	Compares two cabinet values and returns <code>true</code> only 
	if they are equal (meaning that they have the same type and value). 
	Cabinets are compared using ASCabEqual(). ASText values are 
	compared by using ASTextCmp() and testing for a return value 
	of <code>0</code> (zero). Strings and binary values must have the same 
	lengths and byte-for-byte contents. Booleans, atoms, doubles, 
	and integers must have equal values. Pointer values must 
	point to the same location in memory but may have different 
	<i>destroyProcs</i> and type strings. 
	@param cab1 IN/OUT The first cabinet. 
	@param keyName1 IN/OUT The key name. 
	@param cab2 IN/OUT The second cabinet. 
	@param keyName2 IN/OUT The key name. 
	@return See above. 
	@exception genErrBadParm 
	@see ASCabEqual 
	@since PI_ASEXTRA_VERSION >= 0x00050000
*/
NPROC(ASBool, ASCabValueEqual, (ASConstCab cab1, const char *keyName1,
								ASConstCab cab2, const char *keyName2))


/**
	Compares two cabinets and verifies that they have a matching 
	set of keys and that all key values are equal as reported by 
	ASCabValueEqual(). 
	@param cab1 The first cabinet. 
	@param cab2 The second cabinet.
	@return <code>true</code> if the cabinets are equal, <code>false</code> otherwise. 
	@exception genErrBadParm 
	@see ASCabValueEqual 
	@since PI_ASEXTRA_VERSION >= 0x00050000
*/
NPROC(ASBool, ASCabEqual, (ASConstCab cab1, ASConstCab cab2))


/**
	Munges the keys and the corresponding values in <code>theCab</code> based 
	on the keys in <code>keyCab</code> and the munge action. Note that <code>keyCab</code> 
	is never altered, but <code>theCab</code> is. 
	@param theCab IN/OUT The cabinet to be modified. 
	@param keyCab IN/OUT The cabinet used to modify <code>theCab</code>. 
	@param action IN/OUT The type of action to be taken. 
	@exception genErrBadParm 
	@since PI_ASEXTRA_VERSION >= 0x00050000
*/
NPROC(void, ASCabMunge, (ASCab theCab, ASConstCab keyCab, ASCabMungeAction action))


/**
	Stores an ASPathName in <code>theCab</code> under <code>theKey</code>. 

	<p><code>theCab</code> assumes ownership of the ASPathName, so the client 
	need not call ASFileSysReleasePath(). </p>
	@param theCab IN/OUT The cabinet. 
	@param keyName IN/OUT (May be <code>NULL</code>) The key name. 
	@param fileSys IN/OUT The ASFileSys from which the path was obtained. 
	
	@param pathName IN/OUT (May be <code>NULL</code>) The ASPathName to be stored. 
	If <code>NULL</code>, the value (if any) stored under <code>theKey</code> in <code>theCab</code> 
	is destroyed and <code>theKey</code> is removed from <code>theCab</code>. 
	@exception genErrBadParm 
	@see ASCabGetPathNameCopy 
	@see ASCabDetachPathName 
	@since PI_ASEXTRA_VERSION >= 0x00050000
*/
NPROC(void, ASCabPutPathName,
	  (ASCab theCab, const char *keyName, ASFileSys fileSys, ASPathName pathName))

/**
	Returns a copy of ASPathName stored under <code>theKey</code> in <code>theCab</code>. 

	<p>It is the client's responsibility to release the ASPathName 
	using ASFileSysReleasePath(). </p>

	<p>Both <code>fileSys</code> and <code>pathName</code> will be <code>NULL</code> if <code>theKey</code> was not 
	found, there was no valid ASPathName stored under the 
	key, or if <code>pathName</code> does not point to an existing file. </p>
	
	@param theCab IN/OUT The cabinet. 
	@param keyName IN/OUT The key name. 
	@param fileSys IN/OUT (Filled by the method) The ASFileSys that 
	<code>pathName</code> was opened through. 
	@param pathName IN/OUT (Filled by the method) The path name. 
	@exception genErrNoMemory 
	@exception Any exception raised by ASFileSysPathFromDIPath. 
	@see ASCabPutPathName 
	@see ASCabDetachPathName 
	@since PI_ASEXTRA_VERSION >= 0x00050000
*/
NPROC(void, ASCabGetPathNameCopy,
	  (ASConstCab theCab, const char *keyName, ASFileSys *fileSys, ASPathName *pathName))

/**
	Retrieves the ASPathName stored under <code>theKey</code> in <code>theCab</code> and 
	removes the key from <code>theCab</code>. 

	<p>Both <code>fileSys</code> and <code>pathName</code> will be <code>NULL</code> if <code>theKey</code> was not 
	found, there was no valid ASPathName stored under the 
	key, or if the ASPathName does not point to an existing 
	file.</p> 

	<p>It is the client's responsibility to release the memory 
	associated with the ASPathName using ASFileSysReleasePath().</p> 
	
	@param theCab IN/OUT The cabinet. 
	@param keyName IN/OUT The key name. 
	@param fileSys IN/OUT (Filled by the method) The ASFileSys that 
	pathName was opened through. 
	@param pathName IN/OUT (Filled by the method) The path name. 
	@exception genErrNoMemory 
	@exception Any exceptions raised by ASFileSysPathFromDIPath. 
	@see ASCabPutPathName 
	@see ASCabGetPathNameCopy 
	@since PI_ASEXTRA_VERSION >= 0x00050000
*/
NPROC(void, ASCabDetachPathName,
	  (ASCab theCab, const char *keyName, ASFileSys *fileSys, ASPathName *pathName))


/**
	Writes <code>theCab</code> out to a stream. The caller retains ownership 
	of the cabinet. The stream will not be closed or flushed. 
	
	@param theCab IN/OUT The cabinet. 
	@param theStm IN/OUT Must be a stream opened through ASFileStmWrOpen() 
	or ASProcStmWrOpen(). 
	@exception genErrBadParm 
	@exception fileErrWrite 
	@see ASCabReadFromStream 
	@since PI_ASEXTRA_VERSION >= 0x00050000
*/
NPROC(void, ASCabWriteToStream, (ASConstCab theCab, ASStm theStm))

/**
	Reads a previously written cabinet from a stream. 
	@param stm Must be a stream opened through ASFileStmRdOpen(), 
	ASMemStmRdOpen(), or ASProcStmRdOpenEx().
	@return The ASCab, or <code>NULL</code> if it could not be constructed. 
	@see ASCabWriteToStream 
	@since PI_ASEXTRA_VERSION >= 0x00050000
*/
NPROC(ASCab, ASCabReadFromStream, (ASStm stm))


/**
	Renames a key within <code>theCab</code> while preserving the value associated 
	with it. If there is already a key equal to <code>newKeyName</code> in 
	<code>theCab</code>, its value will be destroyed and replaced with the 
	value of oldKeyName. 

	<p>Any attempt to move the item from one sub-cabinet to another 
	will cause ASCabRename() to raise an exception. For example, 
	<code>ASCabRename(theCab, "SubCab1:Key1", "SubCab2:Key1")</code> will 
	raise an exception. If this routine raises an exception, <code>theCab</code> will 
	be untouched. </p>
	@param theCab The cabinet. 
	@param oldKeyName The key name to be changed. 
	@param newKeyName The new name.
	@exception genErrBadParm 
	@since PI_ASEXTRA_VERSION >= 0x00050000
*/
NPROC(void, ASCabRename, (ASCab theCab, const char *oldKeyName, const char *newKeyName))
#endif //ADD_ASCAB //PKG

/**
	Used to determine whether the ASText object contains no 
	text. For example, it determines if retrieving Unicode text would yield 
	a <code>0</code>-length string. 
	@param str A string.
	@return Returns <code>true</code> if the ASText object contains no text. 
	@since PI_ASEXTRA_VERSION >= 0x00050000
*/
NPROC(ASBool, ASTextIsEmpty, (ASConstText str))


/**
	Replaces all end-of-line characters within the ASText object 
	with the correct end-of-line character for the current platform. For 
	example, on Windows, <code>\\r</code> and <code>\\n</code> are replaced with <code>\\r\\n</code>. 
	
	@param text An object of type ASText.
	@since PI_ASEXTRA_VERSION >= 0x00050000
*/
NPROC(void, ASTextNormalizeEndOfLine, (ASText text))


/**
	Creates a new string from an ASInt32 by converting the number 
	to its decimal representation without punctuation or leading 
	zeros. 
	@param num A number of type ASInt32.
	@return An ASText object. 
	@see ASTextFromUns32 
	@since PI_ASEXTRA_VERSION >= 0x00050000
*/
NPROC(ASText, ASTextFromInt32, (ASInt32 num))

/**
	Creates a new string from an ASUns32 by converting it to 
	a decimal representation without punctuation or leading 
	zeros. 
	@param num IN/OUT A value of type ASUns32. 
	@return An ASText object. 
	@see ASTextFromInt32 
	@since PI_ASEXTRA_VERSION >= 0x00050000
*/
NPROC(ASText, ASTextFromUns32, (ASUns32 num))

/**
	Removes the contents of an ASText (turns it into an empty 
	string). 
	@since PI_ASEXTRA_VERSION >= 0x00050000
*/
NPROC(void, ASTextMakeEmpty, (ASText str))

/* Acrobat 6.0 additions */
/**
	Replaces all occurrences of characters contained in the 
	list <code>pszBadCharList</code> in the text with the specified replacement 
	character. 

	<p>Various exceptions may be raised.</p>

	@param str The text in which to replace characters. 
	@param pszBadCharList A list of characters to replace, 
	in sorted order with no duplicates. 
	@param replaceChar The character with which to replace 
	any character appearing in the list.
	@see ASTextReplace 
	@see ASTextReplaceASCII 
	@since PI_ASEXTRA_VERSION >= 0x00060000
*/
NPROC(void, ASTextReplaceBadChars, (ASText str, const char *pszBadCharList, char replaceChar))

#if ADD_ASCAB //PKG
/**
	Returns the ASUns32 value stored under <code>theKey</code> in <code>theCab</code>.
	@param theCab The cabinet. 
	@param theKey The key name. 
	@param defValue The default value.
	@return The ASUns32 value stored under <code>theKey</code> if the key is found and the value stored under
	it is of type kASValueUns; otherwise <code>defValue</code> is returned.
	@exception genErrBadParm 
	@see ASCabPutUns 
	@since PI_ASEXTRA_VERSION >= 0x00060000
*/
NPROC(ASUns32, ASCabGetUns, (ASConstCab theCab, const char *theKey, ASUns32 defValue))

/**
	Stores the ASUns32 value under <code>theKey</code> in <code>theCab</code>.
	@param theCab The cabinet. 
	@param theKey The key name. 
	@param theUns The value to be stored.
	@exception genErrBadParm 
	@see ASCabGetUns 
	@since PI_ASEXTRA_VERSION >= 0x00060000
*/
NPROC(void , ASCabPutUns, (ASCab theCab, const char *theKey, ASUns32 theUns))
#endif //ADD_ASCAB //PKG

/********************************************************************************
 * ASDate API calls
 *
 * General Notes
 *	
 *		Some of the ASDate methods will raise exceptions when they fail to allocate memory.
 *		
 *		
 ********************************************************************************/
#if ADD_ASDATE //PKG
/**
	Creates a date object. The newly allocated object reflects 
	the epoch time: Jan 1 1970 00:00:00 UTC. 

	<p>Raises an exception if there is not enough memory for the 
	operation.</p> 
	@return The newly created date object. 
	@see ASDateClear 
	@see ASDateCopy 
	@see ASDateDestroy 
	@see ASDateDup 
	@since PI_ACROSUPPORT_VERSION >= 0x00060000
*/
NPROC(ASDate, ASDateNew, (void))

/**
	Creates a new date object containing the same data as an existing date object.
	@param date The date to duplicate.
	@return The new date object.
	@see ASDateCopy 
	@see ASDateDestroy 
	@see ASDateNew 
	@since PI_ACROSUPPORT_VERSION >= 0x00060000
*/
NPROC(ASDate, ASDateDup, (const ASDate date))

/**
	Reinitializes a date object to the newly-allocated state, 
	as returned by ASDateNew(). 
	@param retVal The date object.
	@see ASDateDestroy 
	@see ASDateNew 
	@since PI_ACROSUPPORT_VERSION >= 0x00060000
*/
NPROC(void, ASDateClear, (ASDate retVal))

/**
	Copies date and time data from one date object to another. 
	
	@param original The date to be copied. 
	@param copy The date into which the data is copied.
	@see ASDateClear 
	@see ASDateDup 
	@since PI_ACROSUPPORT_VERSION >= 0x00060000
*/
NPROC(void, ASDateCopy, (const ASDate original, ASDate copy))

/**
	Releases and destroys a date object. 
	@param date The date.
	@see ASDateClear 
	@see ASDateDup 
	@see ASDateNew 
	@since PI_ACROSUPPORT_VERSION >= 0x00060000
*/
NPROC(void, ASDateDestroy, (ASDate date))  

/**
	Creates a time span object. It raises an exception if there 
	is not enough memory for the operation. 
	@return The newly created time span object. 
	@see ASTimeSpanCopy 
	@see ASTimeSpanDup 
	@see ASTimeSpanDestroy 
	@since PI_ACROSUPPORT_VERSION >= 0x00060000
*/
NPROC(ASTimeSpan, ASTimeSpanNew,(void)) 

/**
	Creates a new time span object containing the same data 
	as an existing time span object. It raises an exception if 
	there is not enough memory. 
	@param timeSpan The time span to duplicate.
	@return The new time span object. 
	@see ASTimeSpanCopy 
	@see ASTimeSpanDestroy 
	@see ASTimeSpanNew 
	@since PI_ACROSUPPORT_VERSION >= 0x00060000
*/
NPROC(ASTimeSpan, ASTimeSpanDup, (const ASTimeSpan timeSpan))

/**
	Copies data from one time span object to another. 
	@param original The time span to be copied. 
	@param copy The time span into which the data is copied.
	@see ASTimeSpanDup 
	@since PI_ACROSUPPORT_VERSION >= 0x00060000
*/
NPROC(void, ASTimeSpanCopy, (const ASTimeSpan original, ASTimeSpan copy))

/**
	Releases and destroys a time span object. 
	@param timeSpan The time span.
	@see ASTimeSpanDup 
	@see ASTimeSpanNew 
	@since PI_ACROSUPPORT_VERSION >= 0x00060000
*/
NPROC(void, ASTimeSpanDestroy, (ASTimeSpan timeSpan)) 

/**
	Sets a date object to the current UTC time with no time 
	zone information. 
	@param retVal The date.
	@see ASDateClear 
	@see ASDateGetUTCTime 
	@see ASDateSetToCurrentLocalTime 
	@see ASDateSetLocalTimeOffset 
	@since PI_ACROSUPPORT_VERSION >= 0x00060000
*/
NPROC(void, ASDateSetToCurrentUTCTime, (ASDate retVal))

/**
	Sets a date object to the current local time, using the 
	time zone information from the operating system. 
	@param date The date.
	@see ASDateClear 
	@see ASDateGetLocalTime 
	@see ASDateSetToCurrentUTCTime 
	@see ASDateSetLocalTimeOffset 
	@since PI_ACROSUPPORT_VERSION >= 0x00060000
*/
NPROC(void, ASDateSetToCurrentLocalTime, (ASDate date))

/**
	Sets a date object's local time offset according to the 
	operating system's current time zone information. Different 
	operating systems handle daylight savings differently. This 
	method causes the date object to always use the same daylight savings time 
	offset that the operating system is currently using, even 
	if the date object is modified. 
	@param date The date.
	@see ASDateSetToCurrentLocalTime 
	@see ASDateSetToCurrentUTCTime 
	@since PI_ACROSUPPORT_VERSION >= 0x00060000
*/
NPROC(void, ASDateSetLocalTimeOffset, (ASDate date))

/**
	Initializes a date object from a time string. It raises an 
	exception if there is not enough memory, if the format is 
	unrecognized, or if the time string is not formatted according 
	to the supplied format. 
	@param date The date object. 
	@param timeString The time string, in the specified format. 
	
	@param format The format of the time string. kASTimeNone and kASTimeUniversalH are not supported.
	@see ASDateClear 
	@see ASDateSetToCurrentLocalTime 
	@see ASDateSetToCurrentUTCTime 
	@see ASDateSetTimeFromString 
	@since PI_ACROSUPPORT_VERSION >= 0x00060000
*/
NPROC(void, ASDateSetTimeFromString, (ASDate date, const char* timeString, ASDateTimeFormat format))

/**
	Initializes a date object from a time record. It raises an 
	exception if the time structure represents an invalid time, 
	such as January 32nd 1999 or Feb 29th 2001. It assumes that 
	the parameters for the day and month in the time record are 
	<code>1</code>-based. 
	@param date The date object. 
	@param timeRec The time record.
	@see ASDateClear 
	@see ASDateSetToCurrentLocalTime 
	@see ASDateSetToCurrentUTCTime 
	@see ASDateSetTimeFromString 
	@since PI_ACROSUPPORT_VERSION >= 0x00060000
*/
NPROC(void, ASDateSetTimeFromRec, (ASDate date, const ASTimeRec* timeRec))

/*These functions modify the date by the length of time provided by the ASCalendarTimeSpan/ASTimeSpan object. 
  Note the built in ambiguity in ASCalendarTimeSpan. If you want to subtract and add exact timeSpans (2592000
  seconds as opposed to 1 month, use ASDateAdd/SubtractTimeSpan instead of their CalendarTimeSpan counterparts.*/
  
/**
	Subtracts a calendar time span from a date. It modifies the 
	date by the length of time provided in the ASCalendarTimeSpan 
	object. 
	
	@param date The date. 
	@param timeSpan The calendar time span to subtract.
	@see ASDateAddCalendarTimeSpan 
	@see ASDateSubtractTimeSpan 
	@since PI_ACROSUPPORT_VERSION >= 0x00060000
	
	@note There is some ambiguity in a calendar 
	time span; to subtract an exact time span (for example, 
	2592000 seconds rather than one month), use ASDateSubtractTimeSpan(). 
*/
NPROC(void, ASDateSubtractCalendarTimeSpan, (ASDate date, const ASCalendarTimeSpan timeSpan))
  
/**
	Adds a calendar time span to a date. It modifies the date by 
	the length of time provided in the ASCalendarTimeSpan object. 
	@param date The date. 
	@param timeSpan The calendar time span to add.
	@see ASDateAddTimeSpan 
	@see ASDateSubtractCalendarTimeSpan 
	@since PI_ACROSUPPORT_VERSION >= 0x00060000

	@note There is some ambiguity in a calendar time span; 
	to add an exact time span (for example, 2592000 seconds 
	rather than one month), use ASDateAddTimeSpan(). 
*/
NPROC(void, ASDateAddCalendarTimeSpan, (ASDate date, const ASCalendarTimeSpan timeSpan))
  
/**
	Subtracts a time span (an exact number of seconds) from 
	a date. It modifies the date by the length of time provided 
	in the ASTimeSpan object. 
	@param date The date. 
	@param timeSpan The time span to subtract.
	@see ASDateAddTimeSpan 
	@see ASDateSubtractCalendarTimeSpan 
	@since PI_ACROSUPPORT_VERSION >= 0x00060000
*/
NPROC(void, ASDateSubtractTimeSpan, (ASDate date, const ASTimeSpan timeSpan))
  
/**
	Adds a time span (an exact number of seconds) to a date. 
	It modifies the date by the length of time provided in the 
	ASTimeSpan object. 
	@param date The date. 
	@param timeSpan The time span to add.
	@see ASDateAddCalendarTimeSpan 
	@see ASDateSubtractTimeSpan 
	@see ASTimeSpanAdd 
	@since PI_ACROSUPPORT_VERSION >= 0x00060000
*/
NPROC(void, ASDateAddTimeSpan, (ASDate date, const ASTimeSpan timeSpan))


/**
	Calculates the difference between two ASDate objects and stores 
	the result in the provided ASCalendarTimeSpan object. The 
	result is broken down into years, months, and so on, in 
	the highest denomination possible. For example, a difference 
	of 13 months is reported as 1 year and 1 month. 
	@param date1 The first date. 
	@param date2 The second date. 
	@param result The calendar time span structure in which 
	to store the difference.
	@see ASDateExactDiff 
	@see ASDateCompare 
	@since PI_ACROSUPPORT_VERSION >= 0x00060000
*/
NPROC(void, ASDateCalendarDiff, (const ASDate date1, const ASDate date2, ASCalendarTimeSpan result)) 

/**
	Calculates the exact difference in seconds between two date 
	objects and stores the result in the provided ASTimeSpan 
	object. If <code>date1</code> is earlier than <code>date2</code>, the result is negative. 
	
	@param date1 The first date. 
	@param date2 The second date. 
	@param result The time span structure in which to store 
	the difference.
	@see ASDateCalendarDiff 
	@see ASDateCompare 
	@see ASTimeSpanDiff 
	@since PI_ACROSUPPORT_VERSION >= 0x00060000
*/
NPROC(void, ASDateExactDiff, (const ASDate date1, const ASDate date2, ASTimeSpan result)) 

/**
	Creates a time string from a date object according to a specified format. If time zone
	information is available in the date object, the string contains the local time along with the
	time zone adjustment, if that is supported by the requested format.	
	It raises an exception if there is not enough memory.

	<p>It is the client's responsibility to release the memory associated with the returned string using ASfree().</p>
	
	@param date The date object.
	@param format The format of the time string.
	@return The time string in the specified format. 
	@see ASDateClear
	@see ASDateGetLocalTime 
	@see ASDateGetUTCTime 
	@see ASDateSetTimeFromString 
	@since PI_ACROSUPPORT_VERSION >= 0x00060000
*/
NPROC(char*, ASDateGetTimeString, (const ASDate date, ASDateTimeFormat format))

/**
	Creates a time record that represents the UTC time represented 
	by the date object. It raises an exception if there is not 
	enough memory. 
	@param date The date object.
	@return The newly created time record. 
	@see ASDateClear 
	@see ASDateGetLocalTime 
	@see ASDateGetUTCTime 
	@see ASDateSetTimeFromString 
	@since PI_ACROSUPPORT_VERSION >= 0x00060000
*/
NPROC(ASTimeRec, ASDateGetUTCTime, (const ASDate date))

/**
	Creates a time record that represents the local time represented 
	by the date object. The resulting local time might not account 
	for daylight savings time correctly if the date object has 
	been modified by adding or substracting a time span or calendar 
	time span. It raises an exception if there is not enough memory. 
	@param date The date.
	@return The newly created time record. 
	@see ASDateGetTimeString 
	@see ASDateGetUTCTime 
	@see ASDateSetTimeFromRec 
	@since PI_ACROSUPPORT_VERSION >= 0x00060000
*/
NPROC(ASTimeRec, ASDateGetLocalTime, (const ASDate date))

/**
	Compares two calendar time spans with respect to a base 
	date. Because the values in a calendar time span are not 
	absolute (for example, a leap year has a different number 
	of days), they are resolved with respect to the base date 
	before the comparison is made. 
	@param timeSpan1 The first calendar time span. 
	@param timeSpan2 The second calendar time span. 
	@param baseDate The base date, or <code>NULL</code> to use Jan 1 1970 
	00:00:00, the epoch time.
	@return <code>1</code> if <code>timeSpan1 > timeSpan2</code>, <code>0</code> if they are equal, and <code>-1</code> if <code>timeSpan1 < timeSpan2</code>. 
	
	@see ASDateCompare 
	@see ASTimeSpanCompare 
	@since PI_ACROSUPPORT_VERSION >= 0x00060000
*/
NPROC(ASInt32, ASCalendarTimeSpanCompare, (const ASCalendarTimeSpan timeSpan1, const ASCalendarTimeSpan timeSpan2, const ASDate baseDate))

/**
	Compares two time spans to determine if they are equal or if 
	one represents fewer seconds than the other. 
	@param timeSpan1 The first time span. 
	@param timeSpan2 The second time span.
	@return <code>1</code> if <code>timeSpan1 > timeSpan2</code>, <code>0</code> if they are equal, and <code>-1</code> if <code>timeSpan1 < timeSpan2</code>. 
	
	@see ASDateCompare 
	@see ASCalendarTimeSpanCompare 
	@since PI_ACROSUPPORT_VERSION >= 0x00060000
*/
NPROC(ASInt32, ASTimeSpanCompare, (const ASTimeSpan timeSpan1, const ASTimeSpan timeSpan2))

/**
	Adds two calendar time spans, storing the result in another 
	calendar time span object. Because the values in a calendar 
	time span are not absolute (for example, a leap year has 
	a different number of days), they are resolved with respect 
	to the base date before the addition is done. The result 
	is broken down into years, months, and so on, in the highest 
	denomination possible. For example, a difference of 13 months 
	is reported as 1 year and 1 month. 
	@param timeSpan1 The first calendar time span to add. 
	
	@param timeSpan2 The calendar time span to add. 
	@param baseDate The base date, or <code>NULL</code> to use Jan 1 1970 
	00:00:00, the epoch time. 
	@param result The calendar time span structure in which 
	to store the result.
	@see ASCalendarTimeSpanCompare 
	@see ASTimeSpanAdd 
	@see ASDateAddCalendarTimeSpan 
	@since PI_ACROSUPPORT_VERSION >= 0x00060000
*/
NPROC(void, ASCalendarTimeSpanAddWithBase, (const ASCalendarTimeSpan timeSpan1, const ASCalendarTimeSpan timeSpan2, const ASDate baseDate, ASCalendarTimeSpan result))

/**
	Adds two time spans, storing the result (an exact number 
	of seconds) in another time span object. 
	@param timeSpan1 The first time span to add. 
	@param timeSpan2 The second time span to add. 
	@param result The time span object in which to store the 
	result.
	@see ASCalendarTimeSpanAddWithBase 
	@see ASDateAddTimeSpan 
	@since PI_ACROSUPPORT_VERSION >= 0x00060000
*/
NPROC(void, ASTimeSpanAdd, (const ASTimeSpan timeSpan1, const ASTimeSpan timeSpan2, ASTimeSpan result))

/**
	Calculates the difference between calendar time span objects 
	and stores the result in the provided ASCalendarTimeSpan 
	object. If <code>timeSpan2</code> is less than <code>timeSpan1</code>, the result 
	is negative. Because the values in a calendar time span 
	are not absolute (for example, a leap year has a different 
	number of days), they are resolved with respect to the base 
	date before the addition is done. The result is broken down 
	into years, months, and so on, in the highest denomination 
	possible. For example, a difference of 13 months is reported 
	as 1 year and 1 month. 
	@param timeSpan1 The first calendar time span. 
	@param timeSpan2 The second calendar time span. 
	@param baseDate The base date, or <code>NULL</code> to use Jan 1 1970 
	00:00:00, the epoch time. 
	@param result The calendar time span object in which to 
	store the difference.
	@see ASDateCalendarDiff 
	@see ASTimeSpanDiff 
	@since PI_ACROSUPPORT_VERSION >= 0x00060000
*/
NPROC(void, ASCalendarTimeSpanDiff, (const ASCalendarTimeSpan timeSpan1, const ASCalendarTimeSpan timeSpan2, const ASDate baseDate, ASCalendarTimeSpan result))

/**
	Calculates the exact difference in seconds between time 
	span objects and stores the result in the provided ASTimeSpan 
	object. If <code>timeSpan2</code> is less than <code>timeSpan1</code>, the result 
	is negative. 
	@param timeSpan1 The first time span. 
	@param timeSpan2 The second time span. 
	@param result The time span object in which to store the 
	difference.
	@see ASDateExactDiff 
	@see ASCalendarTimeSpanDiff 
	@since PI_ACROSUPPORT_VERSION >= 0x00060000
*/
NPROC(void, ASTimeSpanDiff, (const ASTimeSpan timeSpan1, const ASTimeSpan timeSpan2, ASTimeSpan result))

/**
	Tests whether one date is earlier or later than another. 
	
	@param date1 The first date. 
	@param date2 The second date.
	@return <code>1</code> if <code>date1 > date2</code>, <code>0</code> if they are equal, <code>-1</code> if <code>date1 < date2</code>. 
	
	@see ASDateExactDiff 
	@see ASTimeSpanCompare 
	@see ASCalendarTimeSpanCompare 
	@since PI_ACROSUPPORT_VERSION >= 0x00060000
*/
NPROC(ASInt32, ASDateCompare,(const ASDate date1, const ASDate date2))

/**
	Initializes a time span object to represent a time span 
	of a specific number of seconds. 
	@param timeSpan The time span object. 
	@param numSeconds The number of seconds.
	@see ASTimeSpanGetASInt32 
	@see ASTimeSpanSet 
	@see ASTimeSpanSetFromString 
	@since PI_ACROSUPPORT_VERSION >= 0x00060000
*/
NPROC(void, ASTimeSpanSetFromASInt32,(ASTimeSpan timeSpan, ASInt32 numSeconds))

/**
	Converts a string to a number of seconds, and initializes 
	a time span object to represent a time span of that number 
	of seconds. This is useful for time spans that are too long 
	to represent with an ASInt32 value. 
	@param timeSpan The time span object. 
	@param numSecondsString The string containing the number 
	of seconds. The string must consist of an optional minus 
	sign (for negative numbers) followed by decimal digits. 
	No white spaces are allowed anywhere in the string.
	@see ASTimeSpanSet 
	@see ASTimeSpanSetFromASInt32 
	@since PI_ACROSUPPORT_VERSION >= 0x00060000
*/
NPROC(void, ASTimeSpanSetFromString, (ASTimeSpan timeSpan, const char* numSecondsString))

/**
	The internal representation of a time span uses 64-bit signed 
	integers (to avoid the year 2038 problem caused by 32-bit representation). 
	This method initializes a time span object to represent 
	a time span of <code>x</code> seconds, where <code>x</code> is the 64-bit 
	signed integer obtained from concatenating <code>highBits</code> and 
	<code>lowBits</code>. 
	@param timeSpan The time span object. 
	@param highBits The most significant word in the desired 
	64-bit signed integer value. 
	@param lowBits The least significant word in the desired 
	64-bit signed integer value.
	@see ASTimeSpanSetFromASInt32 
	@see ASTimeSpanSetFromString 
	@since PI_ACROSUPPORT_VERSION >= 0x00060000
*/
NPROC(void, ASTimeSpanSet, (ASTimeSpan timeSpan, ASInt32 highBits, ASUns32 lowBits))
#endif //ADD_ASDATE //PKG

#if ADD_ASCAB //PKG
/**
	Replaces percent-quoted expressions in the text object with 
	the result of their evaluation, using key/value pairs in 
	the ASCab. For example, for a text value containing the 
	string <code>"%keyone%%keytwo%"</code>, the value is replaced with the 
	concatenation of the values of the keys <code>keyone</code> and <code>keytwo</code> 
	in the ASCab passed in. 
	@param theText A text object containing percent-quoted 
	expressions to replace. 
	@param params The ASCab containing the key/value pairs 
	to use for text replacement.
	@return None. 
	@exception genErrBadParm if <code>theText</code> is <code>NULL</code>. 
	@since PI_ASEXTRA_VERSION >= 0x00060000
*/
//NPROC(void, ASTextEval, (ASText theText, ASCab params))

/**
	Gets a full description of the file system object associated 
	with <code>pathName</code>, returning the item properties in the ASCab 
	format. Calls ASFileSysGetItemPropsAsCabProc(). 

	<p>If the ASCab has no keys on entry, every property known 
	is filled in. If it is not empty, only properties corresponding 
	to keys in the ASCab are filled in. Keys that do not map 
	to a property of the object are removed.</p> 
	
	<p>The ASCab has the following potential entries:</p>

	<TABLE rules="all" cellspacing="1">
	<TR><TH>Key Name</TH><TH>Type</TH></TR>
	<TR><TD><code>isThere</code></TD><TD>ASBool</TD></TR>
	<TR><TD><code>type</code></TD><TD>ASInt32</TD></TR>
	<TR><TD><code>isHidden</code></TD><TD>ASBool</TD></TR>
	<TR><TD><code>isReadOnly</code></TD><TD>ASBool</TD></TR>
	<TR><TD><code>creationDate</code></TD><TD><code>char*</code> (PDF style date string)</TD></TR>
	<TR><TD><code>modDate</code></TD><TD><code>char*</code> (PDF style date string)</TD></TR>
	<TR><TD><code>fileSizeHigh</code></TD><TD>ASUns32</TD></TR>
	<TR><TD><code>fileSizeLow</code></TD><TD>ASUns32</TD></TR>
	<TR><TD><code>folderSize</code></TD><TD>ASInt32</TD></TR>
	<TR><TD><code>creatorCode</code></TD><TD>ASUns32</TD></TR>
	<TR><TD><code>typeCode</code></TD><TD>ASUns32</TD></TR>
	<TR><TD><code>versionMajor</code></TD><TD>ASUns32</TD></TR>
	<TR><TD><code>versionMinor</code></TD><TD>ASUns32</TD></TR>
	<TR><TD><code>isCheckedOut</code></TD><TD>ASBool</TD></TR>
	<TR><TD><code>isPublished</code></TD><TD>ASBool</TD></TR>
	</TABLE>


	@param fileSys (May be <code>NULL</code>) The file system from which 
	<code>pathName</code> was obtained. Pass <code>NULL</code> to use the default file 
	system. 
	@param pathName The ASPathName associated with the object. 
	
	@param theCab (Filled by the method) Properties describing 
	the object, in cabinet format. 
	@return <code>0</code> if no error was encountered; otherwise an error code is 
	returned. If an error code is returned, <code>theCab</code> is not filled 
	with valid values. If the path name does not point to an 
	object on the file system, returns <code>asFileErrFNF</code> and a valid 
	ASCab with <code>isThere</code> set to <code>false</code>. 
	@exception genErrBadParm 
	@exception genErrMethodNotImplemented 
	@see ASFileSysConvertCabToItemProps 
	@see ASFileSysConvertItemPropsToCab 
	@see ASFileSysGetItemProps 
	@since PI_ACROSUPPORT_VERSION >= 
*/
NPROC(ASInt32, ASFileSysGetItemPropsAsCab, (ASFileSys fileSys, ASPathName pathName, ASCab theCab))

/**
	Converts a set of item properties from the ASCab format 
	to the ASFileSysItemPropsRec format. 
	@param props (Filled by the method) The item properties 
	structure. 
	@param theCab Properties describing the object, in cabinet 
	format. 
	@return <code>0</code> if no error was encountered; otherwise an error code is 
	returned. 
	@exception genErrBadParm 
	@exception genErrMethodNotImplemented 
	@see ASFileSysConvertItemPropsToCab 
	@see ASFileSysGetItemProps 
	@see ASFileSysGetItemPropsAsCab 
	@since PI_ACROSUPPORT_VERSION >= 
*/
NPROC(ASInt32, ASFileSysConvertCabToItemProps, (ASFileSysItemProps props, ASCab theCab))

/**
	Converts a set of item properties from the ASFileSysItemPropsRec 
	format to the ASCab format.
	
	<p>The ASCab has the following potential entries:</p>

	<TABLE rules="all" cellspacing="1">
	<TR><TH>Key Name</TH><TH>Type</TH></TR>
	<TR><TD><code>isThere</code></TD><TD>ASBool</TD></TR>
	<TR><TD><code>type</code></TD><TD>ASInt32</TD></TR>
	<TR><TD><code>isHidden</code></TD><TD>ASBool</TD></TR>
	<TR><TD><code>isReadOnly</code></TD><TD>ASBool</TD></TR>
	<TR><TD><code>creationDate</code></TD><TD><code>char*</code> (PDF style date string)</TD></TR>
	<TR><TD><code>modDate</code></TD><TD><code>char*</code> (PDF style date string)</TD></TR>
	<TR><TD><code>fileSizeHigh</code></TD><TD>ASUns32</TD></TR>
	<TR><TD><code>fileSizeLow</code></TD><TD>ASUns32</TD></TR>
	<TR><TD><code>folderSize</code></TD><TD>ASInt32</TD></TR>
	<TR><TD><code>creatorCode</code></TD><TD>ASUns32</TD></TR>
	<TR><TD><code>typeCode</code></TD><TD>ASUns32</TD></TR>
	<TR><TD><code>versionMajor</code></TD><TD>ASUns32</TD></TR>
	<TR><TD><code>versionMinor</code></TD><TD>ASUns32</TD></TR>
	<TR><TD><code>isCheckedOut</code></TD><TD>ASBool</TD></TR>
	<TR><TD><code>isPublished</code></TD><TD>ASBool</TD></TR>
	</TABLE>

	@param theCab (Filled by the method) Properties describing 
	the object, in cabinet format. 
	@param props The item properties structure.
	@return <code>0</code> if no error was encountered; otherwise an error code is 
	returned. 
	@exception genErrBadParm 
	@exception genErrMethodNotImplemented 
	@see ASFileSysConvertCabToItemProps 
	@see ASFileSysGetItemProps 
	@see ASFileSysGetItemPropsAsCab 
	@since PI_ACROSUPPORT_VERSION >= 
*/
NPROC(ASInt32, ASFileSysConvertItemPropsToCab, (ASCab theCab, const ASFileSysItemPropsRec *props))

/**
	Tests whether a given operation can be performed on a particular 
	file. It calls the canPerformOpOnItem() procedure registered 
	for the ASFileSysRec, which determines whether the operation 
	is one of the file system-defined operation strings for which 
	there is a handler. 
	@param fileSys (May be <code>NULL</code>) The file system from which 
	<code>pathName</code> was obtained. Pass <code>NULL</code> to use the default file 
	system. 
	@param pathName The ASPathName of the file. 
	@param op The name of the operation to test. A file system-defined 
	string handled by ASFileSysCanPerformOpOnItemProc().
	@return <code>asGenErrNoError</code> if the operation can be performed 
	on the item, or an error indicating why the oepration would 
	fail. 
	@see ASFileSysPerformOpOnItem 
	@since PI_ACROSUPPORT_VERSION >= 0x00060000
*/
NPROC(ASInt32, ASFileSysCanPerformOpOnItem, (ASFileSys fileSys, ASPathName pathName, const char *op))

/**
	Performs a specified operation on a particular file, passing 
	specified parameters. It calls the performOpOnItem() procedure 
	registered for the ASFileSysRec. 
	@param fileSys (May be <code>NULL</code>) The file system from which 
	<code>pathName</code> was obtained. Pass <code>NULL</code> to use the default file 
	system. 
	@param pathName The ASPathName of the file. 
	@param op The name of the operation to perform. A file system-defined 
	string handled by ASFileSysPerformOpOnItemProc(). 
	@param params An ASCab object containing parameters to 
	pass to the operation.
	@return <code>0</code> if the operation was successful,  
	a nonzero platform-dependent error code otherwise. 
	@see ASFileSysCanPerformOpOnItem 
	@since PI_ACROSUPPORT_VERSION >= 0x00060000
*/
NPROC(ASInt32, ASFileSysPerformOpOnItem, (ASFileSys fileSys, ASPathName pathName, const char *op, ASCab params))

#if ADD_ASDATE //PKG
/**
	Negates the time span value of a time span object. 
	@param timeSpan The time span.
	@see ASTimeSpanSet 
	@see ASTimeSpanSetFromASInt32 
	@see ASTimeSpanSetFromString 
	@since PI_ACROSUPPORT_VERSION >= 0x00060000
*/
NPROC(void,ASTimeSpanNegate, (ASTimeSpan timeSpan))
#endif //ADD_ASDATE //PKG

/**
	Tests whether the bytes in the string conform to 
	the Unicode UTF-8 encoding form. The method does not test 
	whether the string is <code>NULL</code>-terminated. 
	@param cIn The string. 
	@param cInLen The length of the string in bytes, not including 
	the <code>NULL</code> byte at the end.
	@return <code>true</code> if the bytes in the string conform to the Unicode UTF-8 
	encoding form, <code>false</code> otherwise. 
	@since PI_ASEXTRA_VERSION >= 0x00060000
*/
NPROC(ASBool, ASIsValidUTF8, ( const ASUns8* cIn, ASCount cInLen))

/**
	Compares two ASConstText objects, ignoring language and 
	country information. The comparison is case-sensitive. 

	<p>Various exceptions may be raised.</p>

	@param str1 First text object. 
	@param str2 Second text object.
	@return Returns a negative number if <code>str1 < str2</code>, a positive number 
	if <code>str1 > str2</code>, and <code>0</code> if they are equal. 
	@see ASTextCmp 
	@since PI_ASEXTRA_VERSION >= 0x00060000
*/
NPROC(ASInt32, ASTextCaseSensitiveCmp, (ASConstText str1, ASConstText str2))

/**
	Enumerates all the keys in the constant cabinet. 

	<p>Keys consisting solely of digits are enumerated first, in 
	numeric order (assuming they are not padded with zeros at 
	the front, which will confuse matters). Non-numeric keys 
	are then enumerated in <code>strcmp</code> order. </p>

	<p>The callback procedure must not add, delete, or modify items 
	in <code>theCab</code> during the enumeration. </p>

	<p>It will <code>RERAISE</code> any exceptions thrown by <code>enumProc</code>.</p>

	@param theCab The cabinet. 
	@param enumProc User-supplied callback that is called 
	for each entry found in <code>theCab</code>. This callback cannot modify 
	the ASConstCab object. 
	@param clientData A pointer to user-supplied data to pass 
	to <code>enumProc</code> each time it is called.
	@exception genErrBadParm 
	@see ASCabEnum 
	@since PI_ASEXTRA_VERSION >= 0x00060000
*/
NPROC(void, ASConstCabEnum, (ASConstCab theCab, ASConstCabEnumProc enumProc, void *clientData))
#endif //ADD_ASCAB //PKG

/**
	Runs the specified filter on a text object, modifying the 
	text as specified. 
	@param text A text object modified by the method. 
	@param filter The filter to run on the text object.
	@return None. 
	@exception genErrBadParm if <code>text</code> is <code>NULL</code> or if an invalid filter is 
	specified. 
	@since PI_ASEXTRA_VERSION >= 0x00060000
*/
NPROC(void, ASTextFilter, (ASText text, ASTextFilterType filter))

#if ADD_ASDATE //PKG
/**
	Gets the number of seconds from a time span object. 
	@param timeSpan The time span object. 
	@param outOverflow (Filled by the method) <code>true</code> if the 
	number of seconds was too large to be represented by an 
	ASInt32 value, <code>false</code> otherwise.
	@return The number of seconds. 
	@see ASTimeSpanSet 
	@see ASTimeSpanSetFromASInt32 
	@see ASTimeSpanSetFromString 
	@since PI_ACROSUPPORT_VERSION >= 0x00060000
*/
NPROC(ASInt32, ASTimeSpanGetASInt32, (ASTimeSpan timeSpan, ASBool* outOverflow))
#endif //ADD_ASDATE //PKG

#if ADD_ASCAB //PKG
/* Acrobat 7.0 additions */
/**
	Returns the ASInt64 value stored under <code>theKey</code> in <code>theCab</code>. 
	
	@param theCab IN/OUT The cabinet. 
	@param theKey IN/OUT The key name. 
	@param defValue IN/OUT The default value. 
	@return The ASInt64 value stored under <code>theKey</code> if the key is found 
	and the value stored under it is of type kASValueInt64; 
	otherwise <code>defValue</code> is returned. 
	@exception genErrBadParm 
	@see ASCabPutInt64 
	@since PI_ASEXTRA_VERSION >= 0x00070000
*/
NPROC(ASInt64, ASCabGetInt64, (ASConstCab theCab, const char *theKey, ASInt64 defValue))

/**
	Stores an ASInt64 value in <code>theCab</code> under <code>theKey</code>. 
	@param theCab IN/OUT The cabinet. 
	@param theKey IN/OUT (May be <code>NULL</code>) The key name. 
	@param theInt IN/OUT The value to be stored. 
	@exception genErrBadParm 
	@see ASCabGetInt64 
	@since PI_ASEXTRA_VERSION >= 0x00070000
*/
NPROC(void , ASCabPutInt64, (ASCab theCab, const char *theKey, ASInt64 theInt))

/**
	Returns the ASUns64 value stored under <code>theKey</code> in <code>theCab</code>. 
	
	@param theCab IN/OUT The cabinet. 
	@param theKey IN/OUT The key name. 
	@param defValue IN/OUT The default value. 
	@return The ASUns64 value stored under <code>theKey</code> if the key is found 
	and the value stored under it is of type kASValueUns64; 
	otherwise <code>defValue</code> is returned. 
	@exception genErrBadParm 
	@see ASCabPutUns64 
	@since PI_ASEXTRA_VERSION >= 0x00070000
*/
NPROC(ASUns64, ASCabGetUns64, (ASConstCab theCab, const char *theKey, ASUns64 defValue))

/**
	Stores an ASUns64 value in <code>theCab</code> under <code>theKey</code>. 
	@param theCab IN/OUT The cabinet. 
	@param theKey IN/OUT (May be <code>NULL</code>) The key name. 
	@param theInt IN/OUT The value to be stored. 
	@exception genErrBadParm 
	@see ASCabGetUns64 
	@since PI_ASEXTRA_VERSION >= 0x00070000
*/
NPROC(void , ASCabPutUns64, (ASCab theCab, const char *theKey, ASUns64 theInt))

/* Acrobat 9.0 additions */

/**
	Removes the contents of an <code>ASText</code> object (converts it into an empty 
	string). It clears the released storage (for security strings).
	@since PI_ASEXTRA_VERSION >= 0x00090000
*/
NPROC(void, ASTextMakeEmptyClear, (ASText str))

/** 
	Converts user input of a password to a form that can be used by Acrobat to open a file.
	@param inPassword  IN  A host-endian, 16-bit <code>NULL</code>-terminated Unicode string. 
	@param outPassword OUT A location to store a pointer to an allocated <code>char*</code> <code>NULL</code>-terminated string. 
	@param useUTF      IN  A flag for controlling the conversion. Prior to Acrobat 9.0, passwords were converted from 
	host code-page encoding (8-bit mode) to <code>PDFDocEncoding</code>.
	If <code>useUTF == false</code>, this routine does the same, starting from 16-bit Unicode.
	With encryption, Acrobat 9.0 and later allows Unicode passwords, normalized and converted to UTF-8 encoding.
	If <code>useUTF == true</code>, such a Unicode password is what is returned.
*/
 //NPROC(void, ASUCS_GetPasswordFromUnicode, (ASUTF16Val* inPassword, void** outPassword, ASBool useUTF))
#endif //ADD_ASCAB //PKG
