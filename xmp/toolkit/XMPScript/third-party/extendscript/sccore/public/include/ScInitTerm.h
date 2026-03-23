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
#ifndef _ScInitTerm_h
#define _ScInitTerm_h

#include "ScRoot.h"
#include "ScPreConfig.h"

namespace ScCore
{

/// The call format of an exit procedure which ScCore calls during termination.
typedef void (*ExitProc)();

/**
Global class to define initialization and termination code entry points
for the ScCore library. InitTerm::exit() shuts down ScCore including the
memory management; any attempt to allocate memory after ScCore has been
shut down results in a C++ exception. init() and exit() have a reference
counter; exit() must be called as often as init().
<p>
If you want the multithreaded version of the DLL, call InitTerm::setMT().
This creates all internal data structures and locks for both the ScCore and
ExtendScript DLLs.
This class is NOT thread safe. All methods must be called during app setup
and shut down only, and only from the main thread.
*/

class SC_API InitTerm
{
public:
	/**
	Initialize ScCore. Call this before using any ScCore code.
	*/
	static	void	init();
	/**
	Deinitialize ScCore. Call this after deleting any ScCore data.
	You must call exit() as often as you called init().
	*/
	static	void	exit();
	/**
	Return true if ScCore has been initialized.
	*/
	static	bool	isInitialized();
	/**
	Add a procedure to be called upon termination. ScCore calls these
	procedures in the order they have been registered before it shuts
	down ScCore.
	*/
	static	void	atExit (ExitProc proc);
	/**
	Define the startup folder. This call is important for Unix systems,
	where the only way to determine the startup folder of the app is
	to inspect argv [0]. On non-Unix systems, this call redefines the
	startup folder already set by the OS. Note: The call strips the last
	element of the path, assuming that it is the name of the application!
	@param	arg0	the content of argv[0]
	*/
	static	void	setStartupSpec (const char* arg0);
	/**
	Enable multithreading in the ScCore DLL. Call this before using any ScCore code.
	*/
	static	void	setMT();
	/**
	Check ehwther multithreading is enabled.
	*/
	static	bool	isMT();
	/**
	Enable or disable any UI. Use this method to inhibit any dialogs. If disabled,
	dialogs return as if the user had clicked Cancel or No. Useful for servers.
	This flag is application global.
	*/
	static	void	enableUI (bool enable);
	/**
	Is UI enabled?
	*/
	static	bool	isUIEnabled();
};

}	// end Namespace

#include "ScPostConfig.h"
#endif

