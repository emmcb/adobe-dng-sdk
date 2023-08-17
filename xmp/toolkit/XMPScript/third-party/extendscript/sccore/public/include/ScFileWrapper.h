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
#ifndef _ScFileWrapper_h
#define _ScFileWrapper_h

#include "ScString.h"

#if SC_NO_DISKIO == 1
#error FileWrapper not supported in this static library configuration, please edit ScStaticLibDefs.h and recompile!
#endif

#include "ScCloneable.h"
#include "ScSimpleArray.h"
#include "ScPreConfig.h"

namespace ScCore
{

class FileWrapperData;
class Encoder;
class FileSpec;

/**
The common file class provides system independent file and folder I/O.
It is able to read data in various encodings and translate the data into
Unicode. It is able to detect a few encodings automatically by looking
at the first few bytes in the file. It can also write data in several
different encodings. Once a FileWrapper object is created, it points to an
absolute location on disk, even if the file was created with a relative URI.
The class is cloneable.
<p>
This class is deprecated; you should use the FileSpec, File and Folder classes
instead.
<p>
For thread safety, the class relies on the C runtime library and the OS APIs
to be thread safe. There are no own locks. If the same file is used in multiple
FileWrapper instances, they are treated as separate instances; no file or record locking.
*/

class SC_API FileWrapper : public Cloneable
{
			FileWrapperData*	mData;
			void				toFile() const;
			void				toFolder() const;
			bool				checkError (scerror_t err) const;
public:
	/**@name static methods */
	//@{
	/**
	Helper function: encode a string to UTF-8 with %xx escapes.
	@param	src					the text to encode
	@param	dst					the encoded text
	*/
	static	void				encode (const String& src, String& dst);
	/**
	Helper function: decode a string from UTF-8 with %xx escapes.
	@param	src					the text to decode
	@param	dst					the decoded text
	*/
	static	void				decode (const String& src, String& dst);
	/**
	Retrieve the file system type as a string. This is currently one of "Windows"
	or "Macintosh".
	*/
	static	const char*			getFileSystem();
	/**
	Retrieve the current directory as a new FileWrapper object.
	*/
	static	FileWrapper*		getCurrentDir();
	/**
	Retrieve a special directory using a constant as defined in the FileUtils class.
	*/
	static	FileWrapper*		getSpecialDir (scint32_t what);
	/**
	Retrieve the system boot directory as a new FileWrapper object. Deprecated.
	*/
	static	FileWrapper*		getStartupDir();
	/**
	Retrieve the system directory as a new FileWrapper object. Deprecated.
	*/
	static	FileWrapper*		getSystemDir();
	/**
	Retrieve the trash directory as a new FileWrapper object. Deprecated.
	*/
	static	FileWrapper*		getTrashDir();
	/**
	Retrieve the temporary directory as a new FileWrapper object. Deprecated.
	*/
	static	FileWrapper*		getTempDir();
	/**
	Retrieve the system line feed sequence. This is one of the constants
	defined as arguments to String::convertLF().
	@see String::convertLF
	*/
	static	int					getSystemLF();
	/**
	Test the path for "/cloud-content/" on mac and "D:\\cloud-content\\" on win
	Where the D can be any drive letter.
	*/
	static	bool				getCloudContent(const String& inString);
	//@}
	/**@name general methods */
	//@{
	/**
	The constructor takes the file name. This can either be an operating system
	specific name or a URI-style name. If the name is empty, a temporary name
	is generated. The encoding is set to the default system encoding.
	@param	isOSPath			if true, the path name is not a URI path, but an OS path
	*/
	explicit					FileWrapper (const String& name, bool isOSPath = false);
	/**
	The constructor takes the file spec. If the name is empty, a temporary name
	is generated. The encoding is set to the default system encoding. The spec is
	copied and not altered.
	@param	name				the file name
	*/
	explicit					FileWrapper (const FileSpec& spec);
	/**
	The empty constructor operates as an ctor with a temp file.
	*/
								FileWrapper ();
	///
								FileWrapper (const FileWrapper& r);
	/**
	The operator= copies the file reference from the provided file. If the current
	object refers to an open file, the file is closed before th copy.
	*/
			FileWrapper&		operator=( const FileWrapper& rhs );
	/**
	The destructor closes the file if it is open.
	*/
	virtual						~FileWrapper();
	/**
	Get the spec the file is working upon. This may be a different spec than the supplied
	one if a temp file has been created, or the file has been renamed.
	*/
			const FileSpec&		getFileSpec() const;
	/**
	Create a copy of this instance. The new instance never contains an open disk file.
	*/
	virtual	scerror_t			clone (Cloneable** targetLocation, bool isIgnored = true) const;
	/**
	Retrieve the last error message. If this string is empty, there was no error.
	The string is cleared before any operation.
	*/
			const String&		getError() const;
	/**
	Set an error message. If the string is empty, any I/O error is cleared.
	*/
			void				setError (const String& msg);
	/**
	Get the cloud document flag.
	*/
			bool				getCloudDoc() const;
	/**
	Set the cloud document flag. The document parent is now "/cloud-content/"
	*/
			void				setCloudDoc (bool to);
	/**
	Retrieve the file name as URI. This URI is relative to the current folder.
	*/
			String				getRelativeURI() const;
	/**
	Retrieve the file name as URI. This URI is relative to the given folder.
	*/
			String				getRelativeURI (const FileWrapper& basePath) const;
	/**
	Retrieve the file name as an absolute URI.
	@param	tilde				if true, generate "~" for th home dir
	@return						the path name or an empty string on errors
	*/
			String				getAbsoluteURI (bool tilde = true) const;
	/**
	Retrieve the file name as an OS-formatted absolute path name. This may be the path
	to a file system alias.
	@return						the path name or an empty string on errors
	*/
			String				getPath() const;
	/**
	Retrieve the file name as an OS-formatted absolute path name. This is always the resolved path..
	@return						the path name or an empty string on errors
	*/
			String				getResolvedPath() const;
	/**
	Get the display name of a file spec without the parent specification.
	On the Mac, the display name may be different from the real name. On
	all other OSes, getDisplayName() is identical to getName().
	*/
			String				getDisplayName() const;
	/**
	Change the path by applying the relative pathspec to the current path.
	Note that on the Mac, the path must exist except for the last element of the path.
	If this wrapper wraps an open file, that file is closed.
	@param	path				the path to apply
	@return						true if the change was successful
	*/
			bool				changePath (const String& path);
	/**
	Create a file system alias. This instance is assumed to be the alias file to be created.
	Supply the absolute or relative path to the file that you want this filespec to point to.
	On Windows, aliases are converted to absolute paths.
	@param	path				the path which this alias will point to
	@param	reserved			reserved; do not use	
	@return						false on errors
	*/
			bool				createAlias (const String& which, bool = false);
	//@}
	/**@name file attributes */
	//@{
	/// The possible flags bits for file attributes.
	enum
	{
		/// A volume
		kAttrVolume		= 1,
		/// A directory
		kAttrDirectory	= 2,
		/// A file
		kAttrFile		= 4,
		/// An alias
		kAttrAlias		= 8,
		/// A hidden entry
		kAttrHidden		= 16,
		/// A read-only entry
		kAttrProtected	= 32
	};
	/**
	Get the attributes of this object. On errors, return 0. Return the file alias attributes
	if the file points to an alias.
	@param	name				the file name
	@param	created				if present, takes the creation date in Unix format
	@param	modified			if present, takes the modification date in Unix format
	@return						the attribute bits or 0 on errors
	*/
			int					getAttributes (scint32_t* created = NULL, scint32_t* modified = NULL) const;
	/**
	Set the attributes of the given file. Only the attributs kAttrHidden and kAttrProtected
	are supported. This call sets the attributes for the file alias if the object is a file alias.
	@param	attrs				the attribute bits
	@return						true on success
	*/
			bool				setAttributes (int attrs);
	//@}
	/**@name directory access */
	//@{
	/**
	Set the current directory to this file object.
	@return						false on errors
	*/
			bool				setCurrentDir();
	/**
	Retrieve a list of files contained in this directory. If this is not a directory,
	return false. The array contains FileWrapper objects after the call. The optional mask
	may contain the characters "*" and "?". Aliases are resolved.
	@param	array				the array to fill
	@param	mask				the optional mask
	@return						false on errors
	*/
			bool				getFiles (TSimpleArray<FileWrapper>& array, const String* mask = NULL);
	/**
	Create a folder based on this file name.
	@return						false on errors
	*/
			bool				makeDir();
	/**
	Rename this file. This call renames an alias file; it does not rename
	the file behind the alias. If the new name is a path name, attempt to
	move the file to the new location. If not, the file is just renamed.
	@param	name				the new name or URI
	@return						false on errors
	*/
			bool				rename (const String& name);
	/**
	Delete this file. It does not resolve an alias, but rather deletes
	the alias file.
	@return						false on errors
	*/
			bool				remove();
	//@}
	/**@name I/O methods */
	//@{
	/**
	Get the Macintosh file type. On Windows, a few default file types are returned
	according to their extensions; the default type returned on Windows is "TEXT".
	Aliases are resolved.
	*/
			String				getType() const;
	/**
	Get the Macintosh file creator. On Windows, "????" is returned.
	Aliases are resolved.
	*/
			String				getCreator() const;
	/**
	Get the line feed character sequence used when writing text data.
	This is one of the constants used in String::convertLF().
	@see String::convertLF
	*/
			int					getLF() const;
	/**
	Set the line feed character sequence used when writing text data.
	This is one of the constants used in String::convertLF().
	@see String::convertLF
	*/
			void				setLF (int lf);
	/**
	Retrieve the file size. On errors, return -1.
	*/
			scint64_t				getSize() const;
	/**
	Set the file size. On errors, return false.
	*/
			bool				setSize (scint64_t n);
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
	"w" (write) or "e" (edit). The write mode starts with an empty file; any extsting 
	content is erased. The edit mode opens the file in read/write mode, leaving the
	contents. Internally, the file is opened in binary mode. Optionally, a Macintosh 
	type and creator may be supplied; these strings must be at least 4 characters 
	scint32_t so a four-byte ID can be created. Returns false on errors.
	*/
			bool				open (const String& mode, const String* type = NULL, 
								const String* creator = NULL);
	/**
	Close any open file. Returns false on errors.
	*/
			bool				close();
	/**
	Read a chunk of characters and return the data read. If the file is encoded, multiple
	characters may be read to form a single Unicode character. This call ensures that
	always full Unicode characters are read. If you want to read the entire file, use
	a negative value for the number of characters. Any encoder other than ASCII or BINARY
	will slow down the read considerably. CRLF sequences are converted to \n.
	@param	len					the number of Unicode characters to read
	@param	text				takes the text read
	@return						false on errors
	*/
			bool				read (scint64_t len, String& text);
	/**
	Read a single Unicode character. Returns -1 on errors or EOF.
	*/
			int					readch();
	/**
	Read a single line of text up to a CRLF sequence. The \n character is not included.
	@param	text				takes the line text
	@return						false on errors
	*/
			bool				readln (String& text);
	/**
	Check for the EOF condition.
	*/
			bool				eof() const;
	/**
	Check if the file is open
	*/
			bool				isOpen() const;
	/**
	Write the given string. Translate a CRLF sequence into the CRLF sequence set
	if the second arg is true.
	@param	text				the text to write
	@param	translate			if true, translate CRLFs
	@return						false on errors
	*/
			bool				write (const String& text, bool translate = true);
	/**
	Seek to a byte position within the file. In encoded files, this position may
	be in the middle of an encoded character.
	@param	pos					where to seek to
	@param	mode				0 - absolute, 1 - relative, 2 - from end
	@return						false on errors
	*/
			bool				seek (scint64_t pos, int mode);
	/**
	Return the current byte position. Return -1 on errors.
	*/
			scint64_t				tell() const;
	/**
	Flush the contents of any temporary buffer.
	*/
			bool				flush();
	/**
	Create a copy of this file object. If this file or the other file is open, they
	will be closed first.
	@param	to					the name of the file to copy the data to
	@return						false on errors
	*/
			bool				copy (const String& to) const;
	//@}
	/**
	Execute this file within the operating system. This method attempts to find
	the app associated with this file, to launch the app and have the app run that file.
	If successfull, the call returns immediately without wating for the termination
	of the app. If the file is open, it will be closed. For folders, this method acts 
	if the folder was double-clicked. On Windows, it opens an Explorer view with the 
	folder activated, on the Mac, the Finder opens the folder.
	@return						false on errors
	*/
			bool				execute();
};

}	// end namespace

#include "ScPostConfig.h"
#endif

