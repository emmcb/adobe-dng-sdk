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
#ifndef _EsFileDisp_h
#define _EsFileDisp_h

#include "ExtendScript.h"
#include "ScPreConfig.h"

namespace ScScript
{

/**
The FileDisp dispatcher wraps the standard File and Folder objects. Refer to the separate 
PDF documentation in LiveLink for details.
<p>
IMPORTANT: If you are going to return FileSpecs to ExtendScript, ExtendScript will create
corresponding File and Folder JavaScript objects. Without registering this dispatcher,
these objects will be unusable.
<p>
In a multithreading environment, you may want to use different FIleDispatcher objects for 
different engines to set different sets of security flags.
*/

class ES_API FileDisp : public Dispatcher
{
	scint32_t		iFlags;
public:
					FileDisp();
	virtual			~FileDisp();
	/**
	File dispatcher security flags.
	If desired, an app can set one or more security flags. These flags provide various levels
	of sandbox functionality. The flags operate on a specific FileDisp instance, so if desired,
	an app can even have different levels of sandbox functionality in different engine instances.
	An attempt to execute a locked method or property causes the error kErrBadAction to be set.
	*/

	enum
	{
		/// getFiles() returns NULL.
		kNoFileList		= 0x00000001,
		/// Setting the file size, write(), writeln() and copy() do not work.
		kNoWrite		= 0x00000002,
		/// Opening files does not work.
		kNoOpen			= 0x00000004,
		/// Modifications of a dir (rename, remove, create) do not work.
		kNoDirWrite		= 0x00000008,
		/// Setting Folder.current does not work.
		kNoChdir		= 0x00000010,
		/// Resolving aliases does not work.
		kNoResolve		= 0x00000020,
		/// File.execute() is disallowed.
		kNoExecute		= 0x00000040,
		__CFlags__32bit__ = 0x7FFFFFFF
	};

	/// Set security flags.
	inline	void	setSecurityFlags (scint32_t f) { iFlags = f; }
	/// Get security flags.
	inline	scint32_t	getSecurityFlags() const { return iFlags; }
	/**
	Create a Folder object. If the given file system exists, and it it a file, this
	method creates a File object instead.
	@param	objRef	takes the created object reference
	@param	path	the path name
	@return			true if the object was created
	*/
			bool	createFolder (ScCore::Variant& objRef, const ScCore::String& path);
	/**
	Create a File object. If the given file system exists, and it it a folder, this
	method creates a Folder object instead.
	@param	objRef	takes the created object reference
	@param	path	the path name
	@return			true if the object was created
	*/
			bool	createFile (ScCore::Variant& objRef, const ScCore::String& path);
};

}	// end namespace

#include "ScPostConfig.h"

#endif

