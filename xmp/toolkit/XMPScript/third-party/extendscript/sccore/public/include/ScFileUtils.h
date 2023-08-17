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
#ifndef _ScFileUtils_h
#define _ScFileUtils_h

#include "ScString.h"

#if SC_NO_DISKIO == 1
#error FileUtils not supported in this static library configuration, please edit ScStaticLibDefs.h and recompile!
#endif

#include "ScPreConfig.h"

namespace ScCore
{

class FileSpec;

/**
The File Utilities class provides general support routines like routines
to determine the location of various standard folders. All methods are
static. For thread safety, the class relies on the C runtime library and 
the OS APIs to be thread safe. There are no own locks.
*/

class SC_API FileUtils
{
public:
	/**
	Retrieve the file system type as a string. This is currently one of "Windows"
	or "Macintosh".
	*/
	static	const char*			getFileSystem();
	/**
	Retrieve the system line feed sequence. This is one of the constants
	defined as arguments to String::convertLF().
	@see String::convertLF
	*/
	static	String::CRLFEncoding getSystemLF();
	/**
	Return the file system specific path separator character.
	*/
	static	char				getPathSeparator();
	/// Special directories that you can get:
	enum SpecialDirs  : scint32_t {
		/// The directory where the executable is located.
		kStartupDir			= 1,
		/// The directory where the application package is located (the bundle on Mac, the exe file elsewhere).
		kApplicationDir		= 2,
		/// The system directory.
		kSystemDir			= 3,
		/// The trash directory.
		kTrashDir			= 4,
		/// The dir for temporary files.
		kTempDir			= 5,
		/// The dir containing application data for all users.
		kAppDataAllUsersDir	= 6,
		/// The dir containing application data for the current user.
		kAppDataDir			= 7,
		/// The Common Files directory.
		kCommonFilesDir		= 8,
		/// The My Documents directory.
		kMyDocumentsDir		= 9,
		/// The user desktop. Unix: the home directory.
		kDesktopDir			= 10,
		/// The Common Files directory for 32-bit apps on a 64-bit system.
		kCommonFiles32Dir	= 11,
	};
	/**
	Get one of the special directories as defined in above enumeration.
	@param	what				the constant
	@param	spec				the file spec to be filled in
	@return						any error
	*/
	static	scerror_t			getSpecialDir (scint32_t what, FileSpec& r);
	/**
	Store the system boot directory into the given FileSpec instance. Deprecated.
	Return an error code on errors.
	*/
	static	scerror_t			getStartupDir (FileSpec& r);
	/**
	Store the system directory into the given FileSpec instance. Deprecated.
	Return an error code on errors.
	*/
	static	scerror_t			getSystemDir (FileSpec& r);
	/**
	Store the trash directory into the given FileSpec instance. Deprecated.
	Return an error code on errors.
	*/
	static	scerror_t			getTrashDir (FileSpec& r);
	/**
	Store the temporary directory into the given FileSpec instance. Deprecated.
	Return an error code on errors.
	*/
	static	scerror_t			getTempDir (FileSpec& r);
	/**
	Store the local user data directory into the given FileSpec instance. Deprecated.
	Return an error code on errors.
	*/
	static	scerror_t			getUserDataDir (FileSpec& r);
	/**
	Store the common user data directory into the given FileSpec instance. Deprecated.
	Return an error code on errors.
	*/
	static	scerror_t			getAppDataDir (FileSpec& r);
	/**
	Store the current directory into the given FileSpec instance.
	Return an error code on errors.
	*/
	static	scerror_t			getCurrentDir (FileSpec& r);
	/**
	Set the current directory to this file object.
	@return						an error code on errors
	*/
	static	scerror_t			setCurrentDir (const FileSpec& dir);
	/**
	Rename this file spec. This call renames an alias file if the spec points to an alias;
	it does not rename the file behind the alias. The path protion of the name is ignored.
	@param	name				the new name or URI
	@return						an error code on errors
	*/
	static	scerror_t			rename (FileSpec& spec, const String& newName);
	/**
	Move this file spec. This call moves an alias file if the spec points to an alias;
	it does not move the file behind the alias. The spec is changed accordingly.
	@param	newPath				the new path or URI
	@return						an error code on errors
	*/
	static	scerror_t			move (FileSpec& spec, const String& newPath);
	/**
	Delete this file or folder. It does not resolve an alias, but rather deletes
	the alias file. If the file spec refers to a folder, the folder must be empty.
	The spec is cleared.
	@return						false on errors
	*/
	static	scerror_t			remove (FileSpec& spec);
	/**
	Bring this application to the front and activate it. This call attempts to bypass the
	Windows XP limitation that causes the app to only flash the task bar.
	*/
	static	void				appToFront();
};

}	// end namespace

#include "ScPostConfig.h"
#endif

