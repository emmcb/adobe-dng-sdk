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
#ifndef _ScFile_h
#define _ScFile_h

#include "ScString.h"

#if SC_NO_DISKIO == 1
#error File not supported in this static library configuration, please edit ScStaticLibDefs.h and recompile!
#endif

#include "ScPreConfig.h"

namespace ScCore
{

class FileSpec;
class FileData;
class Encoder;

/**
The file class provides system independent file I/O. It is able to read data 
in various encodings and translate the data into Unicode. A few encodings are
detected automatically by looking at the first few bytes in the file. It can 
also write data in several different encodings. Many methods return an error 
code with kErrOK (0) for no error. The constructor attempts to resolve any alias; 
if this fails, all subsequent calls return that error code.
<p>
For thread safety, the class relies on the C runtime library and the OS APIs
to be thread safe. There are no own locks. If the same file is used in multiple
File instances, they are treated as separate instances; no file or record locking.
*/

class SC_API File : public Root
{
			FileData*			mData;
public:
	/**@name construction */
	//@{
	/**
	The constructor takes the file spec. The encoding is set to the default system encoding.
	The constructor attempts to resolve any alias; if this fails, all subsequent calls 
	return that error code.
	@param	name				the file spec
	*/
	explicit					File (const FileSpec& spec);
	/**
	The copy constructor creates a closed file if the source file
	was open. The source file remains open.
	*/
								File (const File&);
	/**
	The assignment operator creates a closed file if the source file
	was open. The source file remains open.
	*/
			File&				operator= (const File&);
	/**
	The destructor closes the file if it is open.
	*/
	virtual						~File();
	//@}
	/**@name general access */
	//@{
	/**
	Get the line feed character sequence used when writing text data.
	This is one of the constants used in String::convertLF().
	@see String::convertLF
	*/
			String::CRLFEncoding getLF() const;
	/**
	Set the line feed character sequence used when writing text data.
	This is one of the constants used in String::convertLF().
	@see String::convertLF
	*/
			void				setLF (String::CRLFEncoding lf);
	/**
	Retrieve the file size. On errors, return an error code.
	*/
	virtual	scerror_t			getSize (scint64_t& size) const;
	/**
	Set the file size. On errors, return an error code. If the file was open, it will be closed.
	*/
	virtual	scerror_t			setSize (scint64_t n);
	/**
	Get the encoding that is used for I/O. When opening a file for reading,
	some encodings are detected automatically (UCS-2, UCS-4 and UTF-8).
	*/
			const String&		getEncoding() const;
	/**
	Set the encoding to use for subsequent I/O.
	@param	encoding			the encoding to use; if not recognized, set to the default encoding.
	*/
			void				setEncoding (const String& encoding);
	/**
	Open a file for I/O. The open mode is case insensitive; it is one of "r" (read),
	"w" (write) or "e" (edit). The write mode starts with an empty file; any existing 
	content is erased. The edit mode opens the file in read/write mode, leaving the
	contents. Internally, the file is opened in binary mode. Optionally, a Macintosh 
	type and creator may be supplied as four-byte IDs (ignored on Windows). 
	Returns an error code on errors.
	*/
	virtual	scerror_t			open (const String& mode, scuint32_t type = 0, scuint32_t creator = 0);
	/**
	Close any open file. Returns an error code on errors.
	*/
	virtual	scerror_t			close();
	/**
	Return true if the file is open.
	*/
	virtual	bool				isOpen() const;
	/**
	Read a chunk of characters and return the data read. If the file is encoded, multiple
	characters may be read to form a single Unicode character. This call ensures that
	always full Unicode characters are read. If you want to read the entire file, use
	a negative value for the number of characters. CRLF sequences are converted to \n.
	@param	len					the number of Unicode characters to read
	@param	text				takes the text read
	@return						an error code on errors
	*/
	virtual	scerror_t			read (scint64_t len, String& text);
	/**
	Read a single line of text up to a CRLF sequence. The \n character is not included.
	@param	text				takes the line text
	@return						an error code on errors
	*/
	virtual	scerror_t			readln (String& text);
	/**
	Get the entire contents of the file as a Unicode string. If the file is larger
	than the given maximum size, the returned string is empty and an error code is
	returned. If the file is not open, it is opened and closed automatically.
	*/
	virtual	scerror_t			getContent (String& target, scint32_t maximum = 0x7FFFFFFF);
	/**
	Check for the EOF condition.
	*/
	virtual	bool				eof() const;
	/**
	Write the given string. Translate a CRLF sequence into the CRLF sequence set
	if the second arg is true.
	@param	text				the text to write
	@param	translate			if true, translate CRLFs
	@return						an error code on errors
	*/
	virtual	scerror_t			write (const String& text, bool translate = true);
	/**
	Seek to a byte position within the file. In encoded files, this position may
	be in the middle of an encoded character.
	@param	pos					where to seek to
	@param	mode				0 - absolute, 1 - relative, 2 - from end
	@return						an error code on errors
	*/
	virtual	scerror_t			seek (scint64_t pos, int mode);
	/**
	Return the current byte position. Return -1 on errors.
	*/
	virtual	scint64_t				tell() const;
	/**
	Flush the contents of any temporary buffer.
	@return						an error code on errors
	*/
	virtual	scerror_t			flush();
	/**
	Create a copy of this file object. If this file or the other file is open, they
	will be closed first.
	@param	to					the name of the file to copy the data to
	@return						false on errors
	*/
	virtual	scerror_t			copyTo (const FileSpec& to) const;
	/**
	Execute this file within the operating system. This method attempts to find
	the app associated with this file, to launch the app and have the app run that file.
	If successful, the call returns immediately without waiting for the termination
	of the app. If the file is open, it will be closed.
	@return						an error code on errors
	*/
	virtual	scerror_t			execute();
};

}	// end namespace

#include "ScPostConfig.h"
#endif

