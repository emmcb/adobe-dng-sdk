/**************************************************************************
*
* ADOBE CONFIDENTIAL
* ___________________
*
*  Copyright 1998 - 2007 Adobe Systems Incorporated
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
#ifndef _ScString_h
#define _ScString_h

#include <stdarg.h>

#include "ScRoot.h"
#include "ScPreConfig.h"

namespace ScCore
{

class Encoder;
class Array;

/*
The ref-counted string data is defined in the header file so debuggers
may inspect the contents of a string. The text member is preset to a size
of 32 characters for debugger inspection only; the size of the data structure
varies according to the length of the string.
*/

struct StringData
{
	/// actual string size
	scint32_t len;
	/// buffer size (0x80000000 set if this is a unique instance)
	scint32_t buflen;
	/// reference count
	scint32_t ref;
	/// actual string data (may vary)
	scutf16_t text [32];
};

/// Macro to cast a wchar_t to either UTF-16 or UTF-32 depending on the OS.

#if SC_WCHAR_WIDTH == 32
#define SC_WCHAR_TO_UTF( s ) ((const scutf32_t*) (s))
#else
#define SC_WCHAR_TO_UTF( s ) ((const scutf16_t*) (s))
#endif

/// Macro to cast a String instance to const wchar_t* (static buffer may be involved!)

#if SC_WCHAR_WIDTH == 32
#define SC_STRING_TO_WCHAR( s ) ((const wchar_t*) (s).encodeUtf32())
#else
#define SC_STRING_TO_WCHAR( s ) ((const wchar_t*) (s).wstr())
#endif


/**
The string class provides everything a string should be able to.
There are ctors for loading a string from a resource, and there
is even a print() method provided which has full printf() functionality. 
Use %lc to print an UTF-16 char, and %ls to print an UTF-16 string. 
<p>
This class is strictly UTF-16. There is a decode() method that takes a 
multibyte string and an encoder, and there is the encode() method that 
takes the encoder to use to obtain the multibyte representation of the 
current contents. In case the encoder is null, the default encoder for 
the current operating system is used. This may be the CP1252 encoder for 
Western Windows, or SHIFT-JIS for Japanese systems. In case where char*
sttrings are used without an encoder, the string is assumed to be zero-terminated
ASCII only. Non-ASCII characters throw an assert in debug mode. There
is an constructor that deals with UTF-32 characters, and an encodeUtf32()
method that encodes the string into a static (!) buffer like encode() does,
as well as an operator = that accepts UTF-32. Two macros, SC_STRING_TO_WCHAR 
(taking a String) and SC_WCHAR_TO_UTF (accepting a const wchar_t*) are portable
aides for wchar_t support.
<p>
The case conversion methods cover the entire range of UTF-16, but only
exact one-to-one conversions are supported. This affects mostly a few Greek 
and Armenian characters.
<p>
A string is reference counted, so assigment and copying is very fast.
But if you are going to use the string as a data buffer, start off with
an empty string and use setup() to initialize it with a defined content.
This will ensure an unique buffer. When using a string in multiple threads,
you should call makeUnique(). This method turns off data refcounting for the
affected instance, making it easier to work on that string with multiple
threads. When modifying the string, you should always use Critical Sections
anyway.
*/

class SC_API String : public Heap
{
protected:
public:
	/// An empty string.
	static	const String&	emptyString();
	/// default ctor (empty string)
							String();
	/// ASCII character constant.
							String (const char* p);
	/// Multibyte character constant (with encoder).
							String (const char* p, const Encoder* encoder);
	/// UTF-16 character constant (with optional length in characters, not bytes).
							String (const scutf16_t* p, int len = -1);
	/// UTF-32 character constant (with optional length in characters, not bytes).
							String (const scutf32_t* p, int len = -1);
	/// copy ctor
							String (const String&);
	/**
	The destructor decrements the refcount of the internal data by one. If if reaches
	zero, it deletes the data.
	*/
							~String();
	/**
	Make the string data unique. Calling this method ensures that the data in this
	string is not refcounted, and that the data will be copied instead of refcounted 
	when used in a copy constructor or assignment operator.
	*/
			void			makeUnique();
	/// The length of the string.
	inline	scint32_t		length() const						{ return data->len;  }
	/// The contents of the string.
	inline					operator const scutf16_t*() const	{ return data->text; }
	/// If you want to use a method to access the contents.
	inline	const scutf16_t* wstr() const						{ return data->text; }
	/**
	Retrieve the contents of this string as a multibyte string using the supplied encoder.
	If no encoder is given, the default system charset encoder is used. The returned
	pointer is a static buffer that gets overwritten whenever this method is called.
	It also returns the trailing zero character.
	@param	bufferP			returns the pointer to a static buffer containing the encoded stuff
	@param	length			returns the number of bytes in the buffer
	@param	encoder			the encoder to use
	@return					an error code
	*/
			scerror_t		encode (const char*& bufferP, scint32_t& lengthInBytes, const Encoder* encoder = NULL) const;
	/**
	Retrieve the contents of this string as a multibyte string using the supplied encoder.
	If no encoder is given, the default system charset encoder is used. The returned
	pointer is a static buffer that gets overwritten whenever this method is called.
	The pointer has a trailing zero character.
	@param	encoder			the encoder to use; null is the default encoder
	@return					the encoded string (static buffer!)
	*/
			const char*		encode (const Encoder* encoder = NULL) const;
	/**
	Retrieve the contents of this string as aUTF-32 encoded string. The returned
	pointer is a static buffer that gets overwritten whenever this method is called.
	The pointer has a trailing zero character.
	@return					the encoded string (static buffer!)
	*/
			const scutf32_t*	encodeUtf32() const;
	/**
	Return the length of the character buffer returned by the last call to encode()
	in bytes. This call is handy for determining the number of converted characters
	if the buffer contains null characters, so strlen() would return a wrong result.
	The length does not include the trailing null character. The method is static,
	since the returned buffer also is static.
	@return					the size of the buffer returned by the last call to encode()
	*/
	static scint32_t		getEncodedLength();
	/**
	Encode a UTF-32 string into UTF-16.
	@param	buffer			the character buffer
	@reurn					an error code
	*/
			scerror_t		decode (const scutf32_t* buffer);
	/**
	Convert the given string into UTF-16 by using the supplied encoder
	and store it into the string.
	@param	p				the buffer
	@param	len				the length in bytes; needed if the string contains null bytes.
	@param	enc				the encoder. If null, the system default is used.
	@return					an error code
	*/
			scerror_t		decode (const char* p, int lengthInBytes = -1, const Encoder* encoder = NULL);
	/**
	Create an UTF-8 ASCII encoded version of this string. The string is converted
	into UTF-8. Every non-alphanumeric character is encoded as a percent escape
	character of the form %xx, where xx is the hex value of the character. You
	cal supply a list of characters that should not be encoded; the default is
	the list of charactes that are defined in RFC2396 (encoding of URIs).
	After the conversion to UTF-8 encoding and escaping, it is guaranteed that
	the string does not contain characters > 127. The method returns false on errors. 
	It uses the method encode() internally.
	@param	dontEncode		list of character that are never encoded (ASCII only)
	*/
			bool			encodeURI(const char* dontEncode = "-_.!~*'();/?:@&=+$,#");
	///
			bool			encodeURI(const scutf16_t* dontEncode);
	/**
	Decode a UTF8-encoded ASCII string generated by encodeURI(). The string should only
	contain characters with values < 128. In the first pass, every character that is
	percent-encoded (in the form %xx, where xx is the hex value of the character).
	Then, the resulting string ts treated as a UTF-8 encoded string and converted
	into UTF-16. On errors, it returns false. It uses the method decode() internally.
	*/
			bool			decodeURI();
	/**
	Return a substring of this instance.
	@param	off				the starting offset
	@param	len				the length
	*/
			String			substr (scint32_t off, scint32_t len = -1) const;
	/**
	Create a buffer with a defined length and fill it with a character.
	@param	ch				the fill character
	@param	size			the buffer size
	@param	enc				optional encoder for single characters
	*/

			void			setup (scutf16_t ch, scint32_t size);
	/**
	This is the printf() equivalent. Use %lc to print an UTF-16 char, and %ls to print 
	an UTF-16 string. 
	@param	fmt				the format string
	*/
			void			print (const scutf16_t* fmt, ...);
	/// Format is in system encoding.
			void			print (const char* fmt, ...);
	/**
	This is the vprintf() equivalent. Use %lc to print an UTF-16 char, and %ls to print 
	an UTF-16 string.
	@param	fmt				the format string
	*/
			void			vprint (const scutf16_t* fmt, va_list args);
	/// Format is in system encoding.
			void			vprint (const char* fmt, va_list args);
	/**
	Insert a character or string at the given position.
	@param	s				the character or string (may be scutf16_t, const scutf16_t*, or const String&)
	@param	pos				the position
	*/
			void			insert (const scutf16_t* s, scint32_t pos = 0);
	/// ASCII only.
			void			insert (const char* s, scint32_t pos = 0);
	///
			void			insert (const String& s, scint32_t pos = 0);
	///
			void			insert (scutf16_t ch, scint32_t pos = 0);
	/**
	Append a character or string
	@param	s				the character or string (may be scutf16_t, const scutf16_t*, or const String&)
	*/
	inline	void			append (const String& s) 	 { *this += s; }
	/// ASCII only!
	inline	void			append (char c)				 { *this += c; }
	///
	inline	void			append (scutf16_t c)		 { *this += c; }
	/// Append a character buffer; compute the length if len < 0.
			void			append (const scutf16_t* src, scint32_t len = -1);
	/// Append a ASCII character buffer; compute the length if len < 0.
			void			append (const char* src, scint32_t len = -1);
	/**
	Replace a character/string with a different character/string. The arguments
	may either be two scutf16_t's, or a mixture of const scutf16_t* and const String&.
	@param	what			the character/string to replace
	@param	with			the character/string to replace with
	@param	off				the starting offset
	@return					the offset where the string was found
	*/
			scint32_t		replace (const String& what, const String& with, scint32_t off = 0);
	///
			scint32_t		replace (scutf16_t what, scutf16_t with, scint32_t off = 0);
	///
			scint32_t		replace (const scutf16_t* what, const scutf16_t* with, scint32_t off = 0);
	///
			scint32_t		replace (const scutf16_t* what, const String& with, scint32_t off = 0);
	///
			scint32_t		replace (const String& what, const scutf16_t* with, scint32_t off = 0);
	/// ASCII only.
			scint32_t		replace (const char* what, const char* with, scint32_t off = 0);
	/// ASCII only.
			scint32_t		replace (const char* what, const String& with, scint32_t off = 0);
	/// ASCII only.
			scint32_t		replace (const String& what, const char* with, scint32_t off = 0);
	/**
	Replace all occurrences of a character/string with a different character/string.
	The arguments may either be two scutf16_t's, or a mixture of const scutf16_t* and const String&.
	@param	what			the character/string to replace
	@param	with			the character/string to replace with
	@param	off				the starting offset
	@return					the number of replacements made
	*/
			scint32_t		replaceAll (const String& what, const String& with, scint32_t off = 0);
	///
			scint32_t		replaceAll (scutf16_t what, scutf16_t with, scint32_t = 0);
	///
			scint32_t		replaceAll (const scutf16_t* what, const scutf16_t* with, scint32_t off = 0);
	///
			scint32_t		replaceAll (const scutf16_t* what, const String& with, scint32_t off = 0);
	///
			scint32_t		replaceAll (const String& what, const scutf16_t* with, scint32_t off = 0);
	/// ASCII only.
			scint32_t		replaceAll (const char* what, const char* with, scint32_t off = 0);
	/// ASCII only.
			scint32_t		replaceAll (const char* what, const String& with, scint32_t off = 0);
	/// ASCII only.
			scint32_t		replaceAll (const String& what, const char* with, scint32_t off = 0);
	/**
	Find a substring. If found, remove everything up to the delimiter,
	remove the delimiter, string the blanks off and return that part. 
	@param	delim			the delimiter to find (may be scutf16_t, const scutf16_t*, or const String&)
	@return					the part found (may be empty)
	*/
			String			split (const String& delim);
	///
			String			split (char delim);
	///
			String			split (scutf16_t delim);
	///
			String			split (const scutf16_t* delim);
	/// ASCII only.
			String			split (const char* delim);
	/**
	Find a character or a string.
	@param	s				the character or string (may be scutf16_t, const scutf16_t*, or const String&)
	@param	off				the starting offset
	@param	backwards		set to true for a backwards scan
	@return					the starting index of the character or string or -1
	*/
			scint32_t		find (const scutf16_t* s, scint32_t off = 0, bool backwards = false) const;
	/// ASCII only.
			scint32_t		find (const char* s, scint32_t off = 0, bool backwards = false) const;
	///
			scint32_t		find (const String& s, scint32_t off = 0, bool backwards = false) const;
	///
			scint32_t		find (scutf16_t ch, scint32_t off = 0, bool backwards = false) const;
	/**
	Find one of any characters of a substring
	@param	s				the list of characters
	@param	off				the starting offset
	@return					the character index or -1
	*/
			scint32_t		findAny (const scutf16_t* s, scint32_t off = 0) const;
	/// ASCII only.
			scint32_t		findAny (const char* s, scint32_t off = 0) const;
	///
			scint32_t		findAny (const String& s, scint32_t off = 0) const;
	/**@name Case-sensitive comparison.
	@param	s				the string to compare with
	@return					<0: smaller, 0: equal, >0: larger
	*/
			int				cmp (const scutf16_t*) const;
	/// ASCII only.
			int				cmp (const char*) const;
	///
			int				cmp (const String&) const;
	/**@name Case-insensitive comparison.
	@param	s				the string to compare with
	@return					<0: smaller, 0: equal, >0: larger
	*/
			int				ucmp (const scutf16_t*) const;
	/// ASCII only.
			int				ucmp (const char*) const;
	///
			int				ucmp (const String&) const;
	/**
	Case-sensitive comparison up to a certain length.
	@param	s				the string to compare with
	@param	n				the maximum UTF-16 characters to compare
	@return					<0: smaller, 0: equal, >0: larger
	*/
			int				ncmp (const scutf16_t*, scint32_t n) const;
	/// ASCII only.
			int				ncmp (const char*, scint32_t n) const;
	///
			int				ncmp (const String&, scint32_t n) const;
	/**
	Case-insensitive comparison up to a certain length.
	@param	s				the string to compare with
	@param	n				the maximum UTF-16 characters to compare
	@return					<0: smaller, 0: equal, >0: larger
	*/
			int				nucmp (const scutf16_t*, scint32_t n) const;
	/// ASCII only.
			int				nucmp (const char*, scint32_t n) const;
	///
			int				nucmp (const String&, scint32_t n) const;
	//@}
	/**@name Comparison operators
	Comparison operators perform a case-sensitive comparison.*/
	//@{
			bool			operator== (const String & r) const;
	///
	inline	bool			operator== (const scutf16_t* p) const	{ return bool (cmp (p) == 0); }
	/// ASCII only.
	inline	bool			operator== (const char* p) const	{ return bool (cmp (p) == 0); }
	///
	inline	bool			operator!= (const scutf16_t* p) const { return bool (cmp (p) != 0); }
	/// ASCII only.
	inline	bool			operator!= (const char* p) const	{ return bool (cmp (p) != 0); }
	///
	inline	bool			operator!= (const String& r) const	{ return ! (*this == r);      }
	///
	inline	bool			operator< (const scutf16_t* p) const  { return bool (cmp (p) <  0); }
	/// ASCII only.
	inline	bool			operator<  (const char* p) const	{ return bool (cmp (p) <  0); }
	///
	inline	bool			operator< (const String& r) const	{ return bool (cmp (r) <  0); }
	///
	inline	bool			operator> (const scutf16_t* p) const  { return bool (cmp (p) >  0); }
	/// ASCII only.
	inline	bool			operator>  (const char* p) const	{ return bool (cmp (p) >  0); }
	///
	inline	bool			operator> (const String& r) const 	{ return bool (cmp (r) >  0); }
	///
	inline	bool			operator<= (const scutf16_t* p) const { return bool (cmp (p) <= 0); }
	/// ASCII only.
	inline	bool			operator<= (const char* p) const	{ return bool (cmp (p) <= 0); }
	///
	inline	bool			operator<= (const String& r) const	{ return bool (cmp (r) <= 0); }
	///
	inline	bool			operator>= (const scutf16_t* p) const { return bool (cmp (p) >= 0); }
	/// ASCII only.
	inline	bool			operator>= (const char* p) const	{ return bool (cmp (p) >= 0); }
	///
	inline	bool			operator>= (const String& r) const	{ return bool (cmp (r) >= 0); }
	//@}
	/**
	Erase parts of a string.
	@param	off				the starting offset
	@param	len				the length
	*/
			void			erase (scint32_t off, scint32_t len = -1);
	/**
	Erase a string.
	*/
			void			erase();
	/// Clear the contents. This sets the length to 0, but does erase the original buffer.
			void			clear();
	/// Assign an ASCII character
			String&			operator= (char c);
	/// Assign an UTF-16 character
			String&			operator= (scutf16_t c);
	/// Assign a ASCII string.
			String&			operator= (const char*);
	/// Assign a UTF-16 string.
			String&			operator= (const scutf16_t*);
	/// Assign a UTF-32 string.
			String&			operator= (const scutf32_t*);
	/// Assign a string instance
			String&			operator= (const String&);
	/// Append a character or a string (may be scutf16_t, const scutf16_t*, or const String&)
			String&			operator+= (char ch);
	///
			String&			operator+= (scutf16_t ch);
	///
			String&			operator+= (const char*);
	///
			String&			operator+= (const scutf16_t*);
	///
			String&			operator+= (const String&);
	/// Append a character or a string (may be scutf16_t, const scutf16_t*, or const String&)
			String			operator+ (char) const;
	///
			String			operator+ (scutf16_t) const;
	///
			String			operator+ (const char*) const;
	///
			String			operator+ (const scutf16_t*) const;
	///
			String			operator+ (const String&) const;
	/**
	Write access to single characters is provided by a method
	because compilers always tend to call the non-const indexing operator.
	@param	index			the index
	@param	value			the character
	*/
			void			set (scint32_t index, scutf16_t value);
	/**
	Convert to lower case; return true if the string changed.
	This method cover the entire range of UTF-16, but only	exact one-to-one conversions are 
	supported. This affects mostly a few Greek and Armenian characters.
	*/
			bool			toLower();
	/**
	Convert to upper case; return true if the string changed.
	This method cover the entire range of UTF-16, but only	exact one-to-one conversions are 
	supported. This affects mostly a few Greek and Armenian characters.
	*/
			bool			toUpper();
	/**
	Convert to mixed case (first char of every word is upper case); return true if the string changed.
	This method cover the entire range of UTF-16, but only	exact one-to-one conversions are 
	supported. This affects mostly a few Greek and Armenian characters.
	*/
			bool			toMixed();
	/// strip leading & trailing whitespace
			void			strip();
	/// the target system for CR/LF conversions
	enum CRLFEncoding : scint32_t
	{
		/// the current system
		eCURRENT,
		/// Windows 95 (CRLF)
		eWIN,
		/// Macintosh (CR only)
		eMAC,
		/// Unix (LF only)
		eUNIX,
	};
	/**
	Perform CRLF conversion according to the target system
	@param	type			the conversion type to apply
	@return					the number of LF sequences c onverted
	*/
			scint32_t		convertLF (CRLFEncoding type = eCURRENT);
	/**
	Parse the contents of the string and interpret backslash escape sequences.
	If the string starts with a single or double quote,treat the quote as a
	delimiter. Interpret \b, \f, \n, \r, \t characters, \nnn octals, \xnn hex
	\unnnn UTF-16 sequences, and \Unnnnnnnn UTF-32 sequences (which are converted
	into surrogate pairs). Treat a newline character as the end of the scan. 
	Returns false on malformed escapes or if a closing quote was not found.
	The parsed string is removed from this instance if successful. If a parsed
	string contains a ZString, it is converted by calling the localizer's
	getZString() method.
	@param	text			takes the scanned string
	@return					false on errors
	@see					Localizer
	*/
			bool			parse (String& result);
	/**
	The same API, but with a starting position. The string is not mofidied, and the
	1st position after the parse is returned in pos.
	*/
			bool			parse (String& result, scint32_t& pos) const;
	/**
	Escape this string. Replace all special characters with their escaped representation,
	like e.g. the newline with "\n", or the single backslash with double backslashes.
	Enclose the escaped result in double quotes. If doUnicode is false, do not escape
	characters >= 128.
	*/
			String			escape (bool doUnicode = true) const;
	/**
	Localize this string. The method uses the global localizer to localize its contents.
	Any occurrence of "%n", where n is a number from 1 on, is replaced by the corresponding
	element of the supplied array, where "%1" is the first element of the array. ZStrings
	are converted as well.
	@param	args			the array
	*/
			void			localize (const Array* args = NULL);
			StringData*		data;							// actual string data
			void			init (scint32_t);				// create buffer
			void			adjust (scint32_t);				// adjust buffer size
			void			unique();						// create unique data instance
};

}	// end Namespace

#include "ScPostConfig.h"

#endif

