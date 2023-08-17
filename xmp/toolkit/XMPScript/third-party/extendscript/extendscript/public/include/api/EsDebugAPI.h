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
#ifndef _EsDebugAPI_h
#define _EsDebugAPI_h

#include "ExtendScript.h"
#include "ScPreConfig.h"

namespace ScCore
{
	class File;
}

namespace ScScript
{

class RealEngine;
class Property;

/// The name of this API

const scint32_t kDebugAPI = 0x64656267; // 'debg'

/**
A profiler entry contains information about one line of one function.
*/

class ES_API ProfilerData : public ScCore::Heap
{
public:
			intptr_t		mReserved;
	/// The total duration time in microseconds.
			scint64_t		mTime;
	/**
	The script ID as passed in to Engine::eval() as file name. Use the DataPool class
	to retrieve the string value.
	*/
			sID				mScriptID;
	/**
	The function name ID. Scriptlets have the value sID_EMPTY.  Use the DataPool class
	to retrieve the string value.
	*/
			sID				mFunctionID;
	/***
	The line number, zero based.
	*/
			scuint32_t		mLine;
	/// The hit count.
			scuint32_t		mHits;
	
	inline ProfilerData() : mReserved(0), mTime(0), mScriptID(), mFunctionID(), mLine(0), mHits(0) { }
};

/**
The breakpoint info contains all information about a breakpoint.
BreakpointInfo instances remain the property of the caller, and the
engine uses/updates them as needed.
*/

class ES_API BreakpointInfo : public ScCore::Root
{
public:
	/// The ctor sets file to sID_EMPTY, the line to 0, the hits to 1, the count to 0, and enabled.
								BreakpointInfo();
								BreakpointInfo (const BreakpointInfo& r);
			BreakpointInfo&		operator= (const BreakpointInfo& r);
	virtual						~BreakpointInfo();
	/// The break condition; user-changeable.
			ScCore::String		mCondition;
	/// The sID of the file name.
			sID					mFile;
	/// The line number, 0-based. Note that the first line numer, if given, is not added here.
			scint32_t			mLine;
	/// The hit count; user-changeable.
			scint32_t			mHits;
	/// The actual hit count, user-changeable; set to a negative value to keep the count when setting.
			scint32_t			mCount;
	/// The enabled flag. A breakpoint may be disabled, but the hit count is still maintained.
			bool				mEnabled;
};

/**
The Debugger API provides debugger access to an engine.
*/

class ES_API DebugAPI : public API
{
	friend class RealEngine;
	friend class Property;

	/// Private copy ctor, because this class has pointer data members.
						DebugAPI (const DebugAPI&);
	/// Private assignment operator, because this class has pointer data members.
	DebugAPI&			operator= (const DebugAPI&);

	Property*			mWatchedProp;		// watched property
	scint32_t			mContextLevel;		// the context level
	ScCore::File*		mOutputStream;		// the output stream
						DebugAPI (Engine&);
public:
	enum Cmd
	{
		/**
		Break a currently running program. Equivalent to the debugBreak() method with an empty argument.
		*/
		kBreakCmd		= 'b',
		/**
		Continue running the program as if it not was debugged. All breakpoints
		remain active.
		*/
		kRunCmd			= 'r',
		/**
		End the program. This commands set the error kErrHalted and causes the program
		to be terminated.
		*/
		kStopCmd		= '!',
		/**
		Execute the next line. Step into any function calls.
		*/
		kStepCmd		= 's',
		/**
		Execute the next line. Do not step into any function calls, but execute them
		in realtime.
		*/
		kStepOverCmd	= 'o',
		/**
		Continue running, but break when the current function exits.
		*/
		kStepOutCmd		= 'i',
		kStepIntoCmd	= kStepOutCmd,	// fix of a typo
		__Cmd__32bit__	= 0x7FFFFFFF
	};

	/// The possible watchpoint commands.

	enum WatchAction
	{
		/// Set a watchpoint.
		kSetWatch		= 0,
		/// Clear a watchpoint.
		kClearWatch		= 1,
		/// Check whether a watchpoint is set.
		kTestWatch		= 2,
		__WatchAction__32bit__	= 0x7FFFFFFF
	};

	/// Possible engine states.

	enum State
	{
		/**
		The program is running. The debugger should leave the break and stop
		buttons enabled and possibly clear the stack trace and source panes.
		*/
		kRunState		= 'R',
		/**
		The program has encountered a breakpoint. The debugger should activate
		its UI if not yet visible. It should load the current stack trace and
		source code location. All actions should be possible.
		*/
		kBreakState		= 'B',
		/**
		A catchable runtime error has been encountered. This state is like the
		BREAK state. Aditionally, the runtime error should be displayed.
		*/
		kErrorState		= 'E',
		/**
		A non-catchable fatal runtime error has been encountered. The state is like
		the kError state, but the program cannot be continued. The only command
		possible is to stop the debugger.
		*/
		kFatalErrorState = 'F',
		__State__32bit__ = 0x7FFFFFFF
	};

	/**
	Set an output stream. This is a File pointer that the engine uses to output
	low-level debugging info (in the debug version) like bytecode tokens. If this
	stream is set and no debugger is active, this stream is used. The Shell app
	calls this method to set stdout for bytecode output. Setting it to NULL (which
	is the default) causes no output at all if no debugger is active.
	@param	stream		the File* pointer or NULL
	*/
	inline	void			setOutputStream (ScCore::File* stream) { mOutputStream = stream; }
	/**
	Retrieve the file stream used for low-level output of opcodes. This
	is a File* pointer or NULL.
	*/
	inline	ScCore::File*	getOutputStream() const { return mOutputStream; }
	/**
	Break the currently running scriptlet. Optionally, execute the given
	scriptlet and break only if the result of the execution is true.
	@param	scriptlet		scriptlet to run
	*/
			void			debugBreak (const ScCore::String& scriptlet) const;
	/**
	Retrieve the debugging state of the given engine. This state can be determined
	by inspecting the error info provided by the engine. If there is no error, it is a
	simple breakpoint/watchpoint. An error code > 0 signals a runtime error, and an
	error code < 0 signals a fatal runtime error.
	*/
			State			getDebugState() const;
	/**
	Get the current nesting depth of the stack.
	*/
			scint32_t			getStackDepth() const;
	/**
	Get the stack trace as a LF separated string. Use the start frame and size
	as supplied. The current stack frame is frame 0.
	*/
			ScCore::String	getStackTrace (scint32_t start = 0, scint32_t size = 0x7FFFFFFF) const;
	/**
	Get the frame info for the given execution context.
	@param	level			the context level (0 = current)
	@param	name			takes the function name
	@param	args			takes the actual function arguments
	@return					the number of formal arguments, or -1 for an invalid frame
	*/
			scint32_t			getContextInfo (scint32_t level, ScCore::String& name, ScCore::Array& args) const;
	/**
	Set the execution context to display. This is a positive number starting with 0. Level 0
	is the current execution context, 1 is the caller's context and so on.
	@return					the context level (0 is the current context, positive numbers only)
	*/
	inline	int				getContextLevel() const { return mContextLevel; }
	/**
	Set the execution context to display. This is a positive number starting with 0. Level 0
	is the current execution context, 1 is the caller's context and so on.
	@param	level		the context level (0 is the current context, positive numbers only)
	*/
			void			setContextLevel (scint32_t level);
	/**
	Get the Script instance corresponding to the current stack level. 
	Call the script's release() method when you are done with it.
	*/
			Script*			getScript() const;
	/**
	Get a list of breakpoints defined in this engine. This is a copy of the entries
	in the engine, and they should be deleted after use.
	*/
			void			getBreakpoints (ScCore::TSimpleArray<BreakpointInfo>& bpList) const;
	/**
	Set the list of given breakpoints. Use NULL as argument to remove all breakpoints.
	The breakpoints in this list are copied into the engine, thus deleting previous
	elements.
	*/
			void			setBreakpoints (const ScCore::TSimpleArray<BreakpointInfo>* bpList);
	/**
	Retrieve the file sID of the current stack frame. If there is no stack frame,
	return sID_EMPTY.
	*/
			sID				getSourceFileID() const;
	/**
	Retrieve the source code for the current stack frame. This method may attempt
	to load a disk file if the source code is an include file. If there is no source
	code, the returned string is empty.
	*/
			ScCore::String	getSource() const;
	/**
	Retrieve the line number of the current stack frame. If there is no stack frame,
	return -1. The number is zero based and does not include the starting line number 
	supplied to compile() or eval().
	*/
			scint32_t			getSourceLine() const;
	/**
	Check whether a certain debugging command should be enabled according to the current
	engine state.
	@param	cmd				the command to check
	@return					true to enable the command
	*/
			bool			isCommandEnabled (Cmd cmd);
	/**
	Execute a debugging command. This call is ignored if the debugger is inactive.
	Otherwise, the command is executed and the debug mode is terminated. Most
	commands simply set some sort of temporary breakpoint. the RUN command just
	exits the debug mode, and the STOP command sets the runtime error kErrHalted.
	All other command clear the current error before the are executed so the engine
	can continue running.
	@param	cmd				the command to execute
	@param	clearError		set to false to leave any error condition active
	*/
			void			command (Cmd cmd, bool clearError = true);
	/**
	Evaluate a script. This is similar to the Engine::eval() call, but never 
	causes the debugger to be activated. Use this method to eval user input
	without disturbing the current state of the debugger. This method waits
	for one second and then sets a timeout error. The text is evaluated within 
	the current context. The result is an Error object on runtime errors.
	@param	text			the text to evaluate
	@param	result			the result
	@return					the error code on runtime errors
	*/
			scerror_t		eval (const ScCore::String& text, ScCore::Variant& result);
	/**
	Execute a watchpoint command on a property. The property must exist.
	@param	objRef			the object containing the property; use undefined for the local workspace
	@param	name			the property name
	@param	action			the action to perform
	@return					true if set or cleared successfully
	*/
			bool			watchpoint (const ScCore::Variant& objRef, const ScCore::Variant& name, WatchAction action) const;
	/**
	Get the watchpoint info. If this method returns false, the debugger activation was due to a breakpoint,
	if the result is true, it was due to a watchpoint.
	@param	objRef			the object containing the property; use undefined for the local workspace
	@param	name			receives the property name
	@param	value			receives the property value
	@return					true if the info could be retrieved, false if the activation was a breakpoint
	*/
			bool			getWatchpointInfo (ScCore::Variant& objRef, ScCore::Variant& name, ScCore::Variant& value);
	/**
	Get a list of properties into the supplied hash table. The hash table contains name/value
	pairs. The list needs to be cleared by calling deleteObjects() after use. Deleted variables
	do not have a value; their value entry is NULL.
	This API is obsolete. It is superceded by getProperties (flags) and will be removed in
	version 4.
	@param	objRef			the object containing the properties; use undefined for the local workspace
	@param	list			the list to be filled
	@param	builtin			true to also fetch built-in data
	@param	walkChain		true if to walk the prototype chain
	@return					false for an invalid object reference
	*/
			bool			getProperties (const ScCore::Variant& objRef, 
										   ScCore::THashTable<ScCore::Variant>& list, 
										   bool builtin = false, bool walkPrototypes = false);
	/**
	Get a list of properties into the supplied hash table. The hash table contains name/value
	pairs. The list needs to be cleared by calling deleteObjects() after use. Deleted variables
	do not have a value; their value entry is NULL.
	@param	objRef			the object containing the properties; use undefined for the local workspace
	@param	list			the list to be filled
	@param	flags			a flag set as defined in the ClassInfo class; kJavaScriptLanguage is added.
	@return					false for an invalid object reference
	*/
			bool			getProperties (const ScCore::Variant& objRef, 
										   ScCore::THashTable<ScCore::Variant>& list, 
										   scint32_t flags);
	/**
	Check for a property being read only.
	@param	objRef			the object containing the properties; use undefined for the local workspace
	@param	name			the property name
	@return					true if the property is read only
	*/
			bool			isReadOnly (const ScCore::Variant& objRef, const ScCore::String& propName);
	/**
	Get the profiler information. This information is kept around until it is erased
	by setting the second argument to true. You need to call deleteObjects() on the 
	array to release the profiler entries. The data is sorted by ScriptID, function 
	ID and line number.
	@param	data			this array takes the profiler data (optional)
	@param	erase			true to erase the profiler data
	*/
			void			getProfilerData (ScCore::TSimpleArray<ProfilerData>* data, bool erase);
	/**
	This is a high-level XML command interface to the engine. See a (to be written)
	tech note for details. Currently, this API is intended for use by the ESTK.
	@param	xml				the XML to execute
	@param	error			takes any errors
	@return					any XML generated by the command or NULL; XML needs to be released
	*/
			ScCore::XML*	executeXML (const ScCore::XML& xml, ScCore::Error& error);
	/**
	The static version of the call above executes XML commands that do not require an engine.
	Currently, these are the get-info, get_dictionaries, get_map, get_package, and tofront
	commands.
	*/
	static	ScCore::XML*	executeStaticXML (const ScCore::XML& xml, ScCore::Error& error);
};

}	// end namespace

#include "ScPostConfig.h"

#endif

