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
*  Copyright 1998 - 2003 Adobe Systems Incorporated
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
**************************************************************************/

#if SC_ONCE
#pragma once
#endif
#ifndef _ScEncoder_h
#define _ScEncoder_h

#include "ScRoot.h"
#include "ScPreConfig.h"

namespace ScCore
{

class String;

/// Use this character for characters that cannot be converted to UTF-16.

#define REPLACEMENT_CHARACTER (scutf16_t (0xFFFD))

/**
This is the base class for all encoders used inside the ExtendScript environment. 
To add an encoder, derive from this class, create a new encoder instance and add it
to the table of encoders. An encoder has a name and two methods: toUnicode()
and toMBCS() that do the actual encoding for a given string. Both accept
a source buffer, a target buffer, and their lengths. A last boolean determines
whether the call is a follow-up call to a previous call or whether it is an 
initial call. The encoder uses this flag to reset its state to initial if needed.
<p>
When converting, the source buffer may be too small to hold a complete character
when converting from an MBCS encoding. In this case, the encoder stores the Unicode 
Replacement Character 0xFFFD and return kErrCharPartial. If the target buffer 
becomes full, the encoder converts as much as it can and return kErrOK. This 
allows the caller to convert a string without actually knowing the size of the 
target buffer. The caller can convert in chunks and allocate more memory as needed. 
If the encoder cannot convert a character, it stores a Replacement Character and 
returns the error kErrCharConversion; the encoder does not necessarily stop at
this error. If, therefore, an input buffer contains one or more invalid or nonconvertable
characters, the target buffer will contain one or more Replacement Characters.
<p>
Upon return, the arguments to toUnicode() and toMBCS() contain the number of source 
and target characters that are left to convert. For toUnicode(), the source count 
is in bytes and the target count is in scutf16_t units; for toMBCS(), the source count 
is in scutf16_t units and the target count is in bytes.
<p>
The encoder may work in either one of two ways. The usual way is to fill the output
buffer until it is full, and then stop. The caller is responsible for converting
more chunks of the source text until the entire text is converted. Another approach
is first to determine the required buffer length and then to convert the entire text
in one large chunk. Actually, Mac encoders work chunk by chunk (The Mac OS does not
support determining the length of a text to be converted), while Windows encoders
convert the text in one big chunk (Windows does not support partial conversions).
The encoder may choose to support the query methods, but does not need to. If these
query methods return a value less than 0, the encoder cannot determine the actual
length of the converted text.
<p>
The String class uses the default encoder when converting strings and no encoder was 
set. This is preset to the system encoder, which mirrors the current system encoding
settings. This encoder may be redefined at any time to a different encoder. This makes
it possible for an application to set the default encoding of its string to, say,
UTF-8 or any other encoding without caring about the system encoding. 
<p>
Don't be surprised when you find that the library already contains a stunning amount
of encoders; all encodings that the OS can handle are set up already. Mac OS knows
many encodings by name; on Windows, you can ask the OS for an encoder by using the encoding
name "CPnnnn" or "WINDOWS-nnnn", where "nnnn" is the code page number. You also do 
not need to care about dashes or underlines in a name; the encoder converts dashes to 
underlines, and it remvoes any leading "X-". Thus, "UTF-8", "UTF_8", "X-UTF-8" and 
"X-UTF_8" are the same. If the get() method cannot find a suitable encoder,
it walks the list of registered encoders and calls each encoder's find() method. 
If an encoder returns an encoder pointer, that encoder is used for the given encoding.
You can, therefore, write a fallback encoder that can handle encodings not available
otherwise, implement the getEncodings() method to return an empty string and register it.
This encoder would then return itself in its find() method if it can handle an otherwise 
unknown encodig; or it could do an heuristic search on the other encoders to find an encoder
that could handle it, or it could create an entirely new encoder for this encoding and return
that one.
<p>
Thread safety: Encoders do usually not contain private data; they are, therefore, thread safe.
All registered encoders are thread global. The pointer to the default encoder is thread local,
however.
*/

class SC_API Encoder : public Root
{
protected:
	/// This is normally -1; set this to the number of bytes required to map one char if constant.
			int				iMapping;
public:
							Encoder();
			virtual			~Encoder();
	/**
	To add a encoder to the base of encoders, create a new converter and call
	this method. The encoder then belongs to the library and is deleted when the 
	app terminates. All encoders are thread global.
	@param	converter		the encoder to add. Must be allocated on the heap.
	*/
	static	void			add (const Encoder& encoder);
	/**
	Add an alias name for an existing encoder. Use this method to define additional
	names for an encoding which the given encoder can handle. All aliases are thread global.
	@param	encoder			the encoder that can handle the alias name
	@param	aliasName		the alias name for the encoding
	@return					true if OK, false for an invalid alias name
	*/
	static	bool			addAlias (const Encoder& encoder, const char* aliasName);
	/**
	Add an alias name for an existing encoding. Use this method to define additional
	names for an encoding which the given encoding can handle. All aliases are thread global.
	@param	encoding		the encoding that is the same as the alias name
	@param	aliasName		the alias name for the encoding
	@return					true if OK, false for an invalid alias name or unknown encoding
	*/
	static	bool			addAlias (const char* encoding, const char* aliasName);
	/**
	Find the encoder for a given encoding. Alias values are supported. The compare
	is case insensitive. Underlines are converted to dashes. All encoders are thread global.
	@param	charset			the character set name
	*/
	static	const Encoder*	get (const scutf16_t* charSet);
	/// ASCII only.
	static	const Encoder*	get (const char* charSet);
	/**
	Return the ASCII encoder. This encoder is used for 7-bit ASCII; it simply expands
	the ASCII string into Unicode chars without any lookup; it is, therefore, very
	quick. Used internally to translate Dispatcher strings.
	*/
	static	const Encoder&	getAscii();
	/**
	Return the binary encoder. This encoder is named BINARY and does a simply 1:1
	translation between 8 bits and 16 bits. On Converting from Unicode, the upper 8 bits
	of Unicode characters are ignored.
	*/
	static	const Encoder&	getBinary();
	/**
	Return the UTF-8 encoder. This encoder is named UTF-8 and converts from and to UTF-8
	using surrogate character pairs as needed.
	*/
	static	const Encoder&	getUtf8();
	/**
	Return the UTF-32 encoder. This encoder is named UTF-32 and converts from and to UTF-32
	using surrogate character pairs as needed.
	*/
	static	const Encoder&	getUtf32();
	/**
	Return the system encoder. This encoder uses the same charset as the current
	settings of the operating system.
	*/
	static	const Encoder&	getSystem();
	/**
	Return the default encoder. This encoder is used implicitly by the String class
	when a null decoder is supplied as parameter. It is initalized to the system encoder.
	This reference is thread local.
	@see	String
	*/
	static	const Encoder&	getDefault();
	/**
	Set the default encoder. This encoder is used implicitly by the String class
	when a null decoder is supplied as parameter. This reference is thread local.
	@see	String
	*/
	static	void			setDefault (const Encoder& e);
	/**
	Check whether this encoder supports a fixed mapping of bytes to Unicode characters.
	Return this number. If <= 0, there is no direct mapping; if > 0, the encoder needs
	exactly iMapping bytes for one Unicode character.
	*/
	inline	int				getMapping() const { return iMapping; }

	// ///////////////////// Overloadable Methods ///////////////////////////

	/**
	Return a list of valid names for this encoder. The list is comma-separated.
	The names should all be in upper case; underlines should show up as dashes, 
	and leading "X-" sequences should be removed. The given string is to be filled 
	with the data.
	@return					the name list
	*/
	virtual	void			getEncodings (String& encodings) const = 0;
	/**
	Check the given encoding. If the encoder is able to process this encoding, or if it
	knows an encoder capable of handling the encoding, return that encoder. If not,
	return null. Use this method to implement additional encodings; if the get() method 
	does not find an encoder in the first run, it walks the list of known encoders and 
	calls this method for each encoder to maybe find another encoder which can do the encoding.
	@param	encoding		the encoding to test for; uppercase, and underlines replaced by dashes
	@return					this encoder, another encoder, a new encoder or null
	*/
	virtual	const Encoder*	find (const String& encoding) const;
	/**
	Determine the required length of the target buffer when converting to Unicode.
	If the encoder is unable to determine the length, return a value less than zero.
	The base class method returns -1.
	@param	srcBuf			source buffer
	@param	srcLen			length of source buffer in bytes
	@param	dstLen			takes the required length (UTF-16 characters) of the target buffer or -1 if unable to determine
	@return					an error code; unable to determine is not an error
	*/
	virtual	scerror_t		getUnicodeLength (const char* srcBuf, scint32_t srcLen, scint32_t& dstLen) const;
	/**
	Determine the required length of the target buffer when converting from Unicode.
	If the encoder is unable to determine the length, return a value less than zero.
	The base class method returns -1. This method should not raise any errors.
	@param	srcBuf			source buffer
	@param	srcLen			length of source buffer in UTF-16 characters
	@param	dstLen			takes the required length (bytes) of the target buffer or -1 if unable to determine
	@return					an error code; unable to determine is not an error
	*/
	virtual	scerror_t		getMBCSLength (const scutf16_t* srcBuf, scint32_t srcLen, scint32_t& dstLen) const;
	/**
	Convert a source buffer to Unicode. If the target buffer is too small, 
	this call can be repeated with the parameter followUp set to true. 
	The method updates srcLen to reflect how many bytes were processed
	in the source buffer and dstLen to reflect how many Unicode characters
	were stored.
	<p>
	This method is thread safe. 
	@param	srcBuf			source buffer
	@param	srcLen			length of source buffer in bytes; the instance updates this variable
	@param	dstBuf			destination buffer
	@param	dstLen			length of destination buffer in UTF-16 characters; the instance updates this variable
	@return					any error code
	@param	followUp		true if this is a follow-up call
	*/
	virtual	scerror_t		toUnicode (const char* srcBuf, scint32_t& srcLen, scutf16_t* dstBuf, scint32_t& dstLen, 
									   bool followUp = false) const;
	/**
	Convert a source buffer from Unicode. If the target buffer is too small, 
	this call can be repeated with the parameter followUp set to true. 
	The method updates srcLen to reflect how many Unicode chars were processed
	in the source buffer and dstLen to reflect how many bytes were stored.
	<p>
	This method is thread safe. 
	@param	srcBuf			source buffer
	@param	srcLen			length of source buffer in UTF-16 characters; the instance updates this variable
	@param	dstBuf			destination buffer
	@param	dstLen			length of destination buffer in bytes; the instance updates this variable
	@param	followUp		true if this is a follow-up call
	@return					any error code
	*/
	virtual	scerror_t		toMBCS (const scutf16_t* srcBuf, scint32_t& srcLen, char* dstBuf, scint32_t& dstLen, 
								    bool followUp = false) const;
};

}	// end Namespace

#include "ScPostConfig.h"

#endif

