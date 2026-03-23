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

#ifndef _ScFileSpec_h
#define _ScFileSpec_h

#include "ScString.h"

#if SC_NO_DISKIO == 1
#error FileSpec not supported in this static library configuration, please edit ScStaticLibDefs.h and recompile!
#endif

#if SC_MAC
	#include <CoreServices/CoreServices.h>
#endif

#include "ScPreConfig.h"

namespace ScCore
{

class FileSpec;
class FileUtils;
class File;
class Folder;
class FileOSSpecific;

/**
The file filter is used as an argument to the iterate() method of
the Folder object as well as to the chooseFile() and chooseFolder() dialogs. 
It is used to process the contents of a folder.
*/

class SC_API FileFilter : public Root
{
			String			mMask;
public:
	/**
	The default constructor causes all files to be processed regardless
	of their name.
	*/
							FileFilter();
	/**
	The constructor takes a search mask. This mask may contain the
	special character * and ?, where * stands for and characters up
	to either the extension separator character "." or the end of the
	name.
	*/
							FileFilter (const String& mask);
	///
	virtual					~FileFilter();
	/**
	Get the current filter mask.
	*/
	inline	const String&	getMask() const { return mMask; }
	/**
	The test procedure determines if the given file or folder should
	be handled at all. This implementation returns true to indicate
	that the given file spec should be processed.
	@param	fileSpec		the file spec to test
	@return					false to ignore this spec
	*/
	virtual	bool			test (const FileSpec& spec) const;
	/**
	This method is called by the Folder::iterate() method if the test() 
	method returned true and the file spec refers to a disk file. 
	Return en error code to halt processing. chooseFile() does not call this method.
	@param	fileSpec		the file spec to test
	@return					an error code on errors
	*/
	virtual	scerror_t		fileAction (const FileSpec& spec) const;
	/**
	This method is called by Folder::iterate() method if the test()
	method returned true and the file spec refers to a disk folder. 
	Return an error code to halt processing. chooseFolder() does not call this method.
	@param	fileSpec		the file spec to test
	@return					an error code on errors
	*/
	virtual	scerror_t		folderAction (const FileSpec& spec) const;
};


/// The filespec data contains a full path name.

class SpecData : public Root
{
public:
	/// The path name.
	String				mPath;
	/// -Weffc++: 'mPath' should be initialized in the member initialization list
	inline	SpecData() : mPath() { }
};

/**
The file specifier refers to a disk file or a directory. The referred location
does not need to exists, but at least the parent path must exist. File and
path names may be UT8 encoded with percent escapes.
<p>
Path names can either be an operating system specific name or a URI-style name, 
including an UTF-8 encoded name with percent escapes (which is supposed to be a string
containing only characters with values < 128, where characters between 128 and 255 are 
encoded into %xx escapes). Paths may be relative; they are converted into absolute
path names using the current working directory as a base. Encoding the character '/' 
is mandatory, since that character is the path separator for the URI-like notation.
The URI-like notation follows RFC2396 pretty closely, with a few extensions. If the
path begins with a double slash, it meant that the path originates from the root of
the current drive. The first element of the path may be a drive name, like /c/ or
/Macintosh HD/. If the first element is not recognized as a drive name, it is treated
as an entry in the root directory of the current drive. Any file::// prefix is stripped.
<p>
The Macintosh file spec offers Mac OS specific constructors, setters and getters
that support Mac OS specific data structures. Internally, the filespec holds the
file path and, if the file/folder is not present, a leaf name.
<p>
The multihreading, the class relies on the C std library and the OS APIs being thread safe.
There are no own locks.
*/

class SC_API FileSpec : public Heap
{
	friend class ScCore::FileUtils;
	friend class ScCore::File;
	friend class ScCore::Folder;
	friend class ScCore::FileOSSpecific;

private:
			void				initData();
			void				releaseData();
protected:
			SpecData*			mData;
								FileSpec (SpecData*);
public:
	/**
	The empty constructor creates an empty reference.
	*/
	inline						FileSpec() : mData(NULL) { initData(); }
	/**
	The constructor takes the path to the file or folder. This can either be an 
	operating system specific name or a URI-style name, including an UTF-8 encoded
	name with percent escapes (which is supposed to be a String instance containing
	only characters with values < 128, where characters between 128 and 255 are encoded
	into %xx escapes). The path may be relative; it is converted into an absolute
	path name using the current working directory as a base.
	@param	name				the name of the file or folder
	@param	isOSPath			if true, treat this path as an OS specific path, never as an URI
	*/
	explicit					FileSpec (const String& name, bool isOSPath = false);
	/// This constructor takes the parent path plus a name of a leaf file or folder.
								FileSpec (const FileSpec& parent, const String& leaf);
	///
								FileSpec (const FileSpec&);
	///
			FileSpec&			operator= (const FileSpec&);
								~FileSpec ();
	/**
	Compare this file spec to another file spec and return true if both
	specs point to the same disk location.
	*/
			bool				operator== (const FileSpec& otherSpec) const;
	///
	inline	bool				operator!= (const FileSpec& otherSpec) const { return !(*this == otherSpec); }
	/**
	Comparison operator for sorts.
	*/
	inline	bool				operator< (const FileSpec& otherSpec) const { return getName() < otherSpec.getName(); }
	/**
	Set the path.  The path name is encoded as described in the constructor. 
	The file or folder may or may not exist. Returns false on errors.
	*/
			bool				setPath (const String& path, bool isOSPath = false);
	/**
	Change the path by applying the relative pathspec to the current path.
	Note that on the Mac, the path must exist except for the last element of the path.
	@param	path				the path to apply
	@return						true if the change was successful
	*/
			bool				changePath (const String& path);
	/**
	Create a temporary file name that is guaranteed to refer to a nonexisting file
	within the current spec (which must be a spec to an existing folder). Returns
	false if this file spec does not refer to an existing folder. 
	*/
			bool				tempName();
	/**
	Retrieve the file name as URI. This URI is relative to the current folder.
	@return						the path name or an empty string on errors
	*/
			String				getRelativeURI() const;
	/**
	Retrieve the file name as URI. This URI is relative to the given folder.
	You need to make sure that the given base path points to a folder.
	@return						the path name or an empty string on errors
	*/
			String				getRelativeURI (const FileSpec& basePath) const;
	/**
	Retrieve the file name as an absolute URI. Specific characters are encoded
	as UTF-8 with percent escapes.
	@param	home				if true, generate "~" for the home dir
	@return						the path name or an empty string on errors
	*/
			String				getAbsoluteURI (bool home = true) const;
	/**
	Retrieve the file name as an OS-formatted absolute path name. This may be the path
	to a file system alias.
	@return						the path name or an empty string on errors
	*/
			String				getPath() const;
	/**
	Retrieve the file name as an OS-formatted absolute path name.
	@return						the path name or an empty string on errors
	*/
			String				getResolvedPath() const;
	/**
	Get the name part of a file spec without the parent specification 
	(non-URI format, no UTF-8 encodings, no escapes).
	*/
			String				getName() const;
	/**
	Get the display name of a file spec without the parent specification.
	On the Mac, the display name may be different from the real name. On
	all other OSes, getDisplayName() is identical to getName().
	*/
			String				getDisplayName() const;
	/**
	Get the extension part of file spec (the part of the file spec that follows the dot)
	(non-URI format, no UTF-8 encodings, no escapes).
	*/
			String				getExtension() const;
	/**
	Get the name part of file spec without the parent specification and the extension
	(non-URI format, no UTF-8 encodings, no escapes).
	*/
			String				getRootName() const;
	/**
	Get the name of the parent folder of this file spec. If there is no parent folder, 
	return an empty string. The string is formatted in OS dependent form.
	*/
			String				getParent () const;
	/**
	Get the parent folder of this file spec. If there is no parent folder, return false.
	*/
			bool				getParentSpec (FileSpec& parentSpec) const;

	/// The possible flags bits for file attributes.
	enum
	{
		/// A volume
		kAttrVolume	= 1,
		/// A directory
		kAttrDirectory	= 2,
		/// A file
		kAttrFile		= 4,
		/// An alias
		kAttrAlias		= 8,
		/// A hidden entry
		kAttrHidden		= 16,
		/// A read-only entry
		kAttrProtected	= 32,
		/// A Mac OS 10 package
		kAttrPackage	= 64
	};
	/**
	Get the attributes of this object. On errors, return 0. Return the file alias attributes
	if the file points to an alias.
	@param	name				the file name
	@param	created				if present, takes the creation date in Unix format
	@param	modified			if present, takes the modification date in Unix format
	@return						the attribute bits or 0 on errors
	*/
			scint16_t				getAttributes (scint32_t* created = NULL, scint32_t* modified = NULL) const;
	/**
	Set the attributes of the given file. Only the attributs kAttrHidden and kAttrProtected
	are supported. This call sets the attributes for the file alias if the object is a file alias.
	@param	attrs				the attribute bits
	@return						true on success
	*/
			bool				setAttributes (scint16_t attrs);
	/**
	Does the file or folder exist?
	*/
	inline	bool				exists() const { return (0 != getAttributes()); }
	/**
	Is this an existing file?
	*/
	inline	bool				isFile() const { return ((kAttrFile & getAttributes()) != 0); }
	/**
	Is this an existing folder?
	*/
	inline	bool				isFolder() const { return ((kAttrDirectory & getAttributes()) != 0); }
	/**
	Is this a file alias?
	*/
	inline	bool				isAlias() const { return ((kAttrAlias & getAttributes()) != 0); }
	/**
	Is this an existing Mac OS 10 package?
	*/
	inline	bool				isPackage() const { return ((kAttrPackage & getAttributes()) != 0); }
	/**
	Is the file or folder visible?
	*/
	inline	bool				isVisible() const { return ((kAttrHidden & getAttributes()) == 0); }
	/**
	Is the file or folder protected?
	*/
	inline	bool				isProtected() const { return ((kAttrProtected & getAttributes()) != 0); }
	/**
	Attempt to resolve any aliases that this file spec points to and set the given file spec
	to the actually existing file. If the file spec is not an alias, the given file spec is
	a copy of this file spec. Returns an error code on errors.
	*/
			scerror_t			resolve (FileSpec& resolvedSpec) const;
	/**
	Attempt to resolve any aliases that this file spec points to and set the this file spec
	to the actually existing file. Returns false on errors. (IS THAT OK, OR AN ERROR CODE?)
	*/
	inline	scerror_t			resolve() { return resolve (*this); }
	/**
	Get the Macintosh file type. On Windows, a few default file types are returned
	according to their extensions; the default type returned on Windows is 'TEXT'.
	Aliases are resolved.
	*/
			scuint32_t			getType() const;
	/**
	Get the Macintosh file creator. On Windows, "????" is returned.
	Aliases are resolved.
	*/
			scuint32_t			getCreator() const;
	/**
	Create a file system alias. This instance is assumed to be the alias file to be created.
	Supply the absolute or relative path to the file that you want this filespec to point to.
	On Windows, aliases are converted to absolute paths.
	@param	path				the path which this alias will point to
	@param	reserved			reserved; do not use	
	@return						an error code
	*/
			scerror_t			createAlias (const String& which, bool = false);
	/**
	Execute a standard Open or Save File dialog. The types argument contains a comma-separated list 
	of file types.  This argument is ignored on the Mac. Each file type is a descriptive text,
	followed by a colon and a file mask. Multiple file masks  may be used; separate them with 
	semicolons, like e.g. "Text files:*.TXT;*.DOC, All files:*".  The star is a placeholder for 
	all files. Optionally, a filter object can be supplied to filter the files in case the dialog 
	is an open dialog. On Windows, the dialog starts in the current directory. If the FileSpec
	is preset with a directory, the dialog changes to the directory; if the FileSpec is preset
	with a file, the dialog changes to the parent directory of that file and presets the input
	field with the file name if applicable. If list is non-NULL, the Open dialog allows for the
	selection of multiple files. In this case, the FileSpec instance remains unmodified.
	Returns true or false; if true is returned, the FileSpec contains the selected file.
	@param	prompt				an optional prompt
	@param	save				true for a save dialog, false for an open dialog
	@param	typeList			the type list; if empty, a default list is used (Windows only)
	@param	filter				an optional filter method (Macintosh + open only)
	@param	list				an optional list that takes all selected files
	*/
			bool				chooseFile (const String& prompt, bool save, const String* typeList = NULL, 
											FileFilter* filter = NULL, Array* list = NULL);
	/**
	This special form of the Save dialog prompts the user if the file that has been selected
	already exists and ask for permission to replace the file.
	*/
			bool				chooseSaveFile (const String& prompt, const String* typeList = NULL, 
											FileFilter* filter = NULL);
	/**
	Execute a standard Choose Folder dialog. On Windows, the dialog starts in the current directory.
	Returns true or false; if true is returned, the FileSpec contains the selected folder.
	@param	prompt				an optional prompt
	*/
			bool				chooseFolder (const String& prompt);

#if SC_MAC

	// ///////////////// MAC SPECIFIC /////////////////////

	enum PackageTreatment : scint32_t
	{
		kPackageAsFolder = 1,
		kPackageAsFile   = 2,
	};
    
	/**
	Get a full path, in HFS format (':' delimiters).
	*/
			OSErr				getHFSPath( String & hfsPath, PackageTreatment packageTreatment ) const;
#endif
};

}	// end namespace

#include "ScPostConfig.h"
#endif

