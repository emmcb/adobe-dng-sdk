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
#ifndef _EsInitTerm_h
#define _EsInitTerm_h

#include "ScCore.h"
#include "ScPreConfig.h"

namespace ScScript
{

/**
Global class to define initialization and termination code entry points.
The init() method must be called before the scripting subsystem can be used.
init() and exit() are reference counted; you must call exit() as often as
you called init(). You can set multithreaded mode for your app by calling the
method ScCore::InitTerm::setMT().
*/

class ES_API InitTerm
{
public:
	/**
	Initialize ExtendScript. Call this before using any ExtendScript code. This
	call This call does not initialize the ScCore library.
	*/
	static void init();
	/**
	Deinitialize ExtendScript. Call this after deleting any ExtendScript data.
	This call does not deinitialize the ScCore library, since other components 
	may still use it.
	*/
	static void exit();
	/**
	Return true if both ExtendScript and ScCore have been initialized.
	*/
	static bool isInitialized();
	/**
	Add a procedure to be called upon termination. ExtendScript calls these
	procedures in the order they have been registered before it shuts
	down ExtendScript. If you want toregister an exit procedure that needs
	to be called after ExtendScript has terminated, but before ScCore terminates,
	use ScCore::InitTerm::atExit(). The ExitProc is a procedure taking no
	arguments and returning nothing.
	*/
	static void	atExit (ScCore::ExitProc proc);
};

}	// end namespace

#include "ScPostConfig.h"
#endif

