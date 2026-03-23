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
#ifndef _ScFolder_h
#define _ScFolder_h

#include "ScRoot.h"

#if SC_NO_DISKIO == 1
#error Folder not supported in this static library configuration, please edit ScStaticLibDefs.h and recompile!
#endif

#include "ScPreConfig.h"

namespace ScCore
{

class FileSpec;
class FolderData;
class FileFilter;

/**
The folder class contains the functionaliy that you would expect from a folder.
Many methods return an error error code with kErrOK (0) for no error.
<p>
For thread safety, the class relies on the C runtime library and the OS APIs
to be thread safe. There are no own locks. If the same file is used in multiple
Folder instances, they are treated as separate instances; no file or record locking.
*/

class SC_API Folder : public Heap
{
			FolderData*		mData;
			Folder&			operator= (const Folder&);
public:
	/**
	The constructor takes the file spec to operate upon. If the spec
	points to an existing disk file, any operation on the folder object
	results in an error.
	*/
	explicit				Folder (const FileSpec& spec);
	///
	explicit				Folder (const Folder&);

							~Folder();
	/**
	Create a folder based on this file name. If part of the path is a link,
	that link is followed. If any path elements are missing, they are created.
	This call can be used to create complete paths.
	@return					an error code on errors
	*/
			scerror_t		create();
	/**
	Iterate through all files in this folder. The method recurses through the directory tree
	until the given level is reached. Negative levels cause the method to resurse the 
	complete tree. The result is true if the iteration was completed normally, or false.
	The given filter object is used to actually determine the files and folder to be processed
	and to actually process these elements.
	@param	filter			the filter object
	@param	levels			the number of levels to recurse through
	@return					an error code on errors
	*/
			scerror_t		iterate (FileFilter& filter, int levels = -1);
	/**
	Open this folder within the operating system. This method acts if the folder was double-clicked.
	On Windows, it opens an Explorer view with the folder activated, on the Mac, the Finder opens
	the folder.
	@return					an error code on errors
	*/
			scerror_t		open();
};

}	// end namespace

#include "ScPostConfig.h"
#endif

