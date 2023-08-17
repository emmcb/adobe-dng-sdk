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
#ifndef _EsDebugger_h
#define _EsDebugger_h

#include "ScDefs.h"
#include "ScCore.h"
#include "ScPreConfig.h"

namespace ScScript
{

class Engine;

/**
The debugger class is the basic interface class between the engine and a debugger.
There are very few methods that are called by the engine when it needs to activate
or to deactivate the debugger, to output text, or to signal a breakpoint/watchpoint.
The debugger in turn calls the methods of the DebugAPI class to set breakpoints,
to execute single steps or to continue execution. The debugger should reflect its
state before executing a debugging command. If the user chose to continue running,
f.ex. all input elements should be disabled except for the ability to break or to
to terminate a script.
<p>
When the engine starts running, poll() is called repeatedly until the engine stops. 
When a breakpoint has been hit, start() is called; the debugger should return from 
that call until is has finished its task. After the engine stops execution, stop() 
is called if the debugger has been actived.
<p>
In a multithreaded environment, the implementer of the debugger needs to decide if his
debugger is capable of handling multiple threads. If not, the debugger's createLock()
method should be called to the engine can lock the debugger for exclusive use. The
built-in debugger is single-threaded and thus lockable.
*/

class ES_API Debugger : public ScCore::Lockable
{
public:
						Debugger();
	virtual				~Debugger();
	/**
	Start the debugger. This method sets up the debugging UI according to the state
	(runtime error, fatal runtime error, or breakpoint). The method should return 
	after displaying and activating the debugger UI, and handling the breakpoint.
	@param	engine		the calling engine
	*/
	virtual	void		start (Engine& engine) = 0;
	/**
	Stop the debugger. Called when the engine terminates running if the debugger
	had been activated at least once. The UI should be deactivated, and the debugger
	should be hidden if there is no more active debugging.
	@param	engine		the calling engine
	@param	soft		true if soft exit
	*/
	virtual	void		stop (Engine& engine) = 0;
	/**
	Poll the debugger UI. This method polls the event loop of the debugger UI.
	It is called during the execution of the engine.
	@param	engine		the calling engine
	*/
	virtual	void		poll (Engine& engine) = 0;
	/**
	Write the given string to some output view. Called by the engine when it wants
	to display debugging information.
	@param	engine		the calling engine
	@param	text		the text to write, may contain LFs
	*/
	virtual	void		write (Engine& engine, const ScCore::String& text) = 0;
	/**
	Hide the debugger if visible. Issued by Javascripts when running "$.hideDebugger()".
	@param	engine		the calling engine
	*/
	virtual	void		hide (Engine& engine) = 0;
};

}	// end namespace

#include "ScPostConfig.h"

#endif

