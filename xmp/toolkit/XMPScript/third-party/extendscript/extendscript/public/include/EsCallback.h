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
#ifndef _EsCallback_h
#define	_EsCallback_h

#include "ScCore.h"
#include "ScPreConfig.h"

namespace ScScript
{

/**
The callback interface is an optional part of the runtime system. It contains
a number of methods which the engine calls at various moments. Most of these
methods are notification methods which may be overloaded if the user is interested
in certain events, f.ex. when the engine stops running or a runtime error occurs.
Also, the localization interface to the ExtendScript engine is part of this class.
To implement callbacks, derive from this class and set the callback with Engine::setCallback().
<p>
If you use the same callback object for multiple engines in a multithreaded environment,
use locks to guard your code if necessary.
*/

class ES_API Callback : public ScCore::Root
{
public:
	/// Enumeration constants for engineNotify()
	enum {
		/** scheduleTask() has been called. Use this notification to implement a precisely
		timed call of Engine::poll() instead of having to poll the engine in an idle loop.
		the data is the delay in milliseconds as a long integer.
		*/
		kScheduleTask	= 0x7461736B, // 'task'
		/** This notification is used immeditately before the engine starts running JavaScript
		code. The user data is 1 if this is a recursive execution, which means that the call
		is executed from within an already running script.
		*/
		kEngineStart	= 0x73746172, // 'star'
		/** This notification is used immeditately after the engine stopped running JavaScript
		code. The user data is 1 if this is a recursive execution, which means that the call
		is executed from within an already running script.
		*/
		kEngineStop		= 0x73746F70,  // 'stop'
		/** This is sent to bracket the access of any properties within the engine via a get(),
		put() or call() API, or via the ESTK. It will not be sent if the engine is evaluating
		a script (within a kEngineStart/kEngineStop notification). The user data is 1 when
		the access is started, and 0 after teminating the access. There may be multiple property
		access between notifications. The notification is not nested - there will only be one
		notificaiton and one end notification. This is useful if the hosting app needs to do a 
		context switch before properties are accessed.
		*/
		kAccessProperties = 0x61636373,	// 'accs'
		__Enum__32_1__	= 0x7FFFFFFF
	};
	/// Bit flags for the outout flag valud of getClassObject()
	enum {
		/** The returned class object should be treated as a class constructor function
		instead of a singleton object like Math.
		*/
		kIsFunction		= 0x00000001,
		/** Ask again for the class object when the property where it is stored is accessed
		again. This lets the host return a new class object every time a program asks for
		a class object. The host needs to set the flag every time it returns a new class
		object in order to be asked again. The host may choose to return NULL if the current
		class object should be used again. If this bit is set, the host should always return
		the same setting of the kIsFunction flag to prevent multiple copies of the JavaScript
		object wrapper to be created.
		*/
		kAskAgain		= 0x00000002,
		__Enum__32_2__	= 0x7FFFFFFF
	};
	/**
	When the engine is running, it calls this method periodically (about twice per second).
	Apps may choose to update the screen and/or to check whether to stop the program or not.
	@param	engine		the engine that called the method
	*/
	virtual	void		running (Engine& engine);
	/**
	When the engine is about to garbage collect, it calls this method to indicate this state.
	Since garbage collection may occur at any time, do not call any Engine methods that may
	alter its status, like eval(), get(), put() or call().
	@param	engine		the engine that called the method
	@return				false if you do not want the engine to garbage collect at this time
	*/
	virtual	bool		garbageCollecting (Engine& engine);
	/**
	A runtime error has been encountered. This callback may be used to log error information.
	It should NOT be used to manipulate or to clear the error, since the engine may become
	unstable when the error is not handled correctly.
	<p>
	This callback is also called when an out-of-memory condition is encountered. In this case,
	keep in mind that there is NO MEMORY left, so do NOT attempt to allocate any memory!
	@param	engine		the engine that called the method
	*/
	virtual	void		runtimeError (Engine& engine);
	/**
	The engine enters debugging mode. It needs the debugger UI to be activated. The app
	should enter some modal state and the display the debugger UI. The debugger window
	may already be visible because it has been left open after a Run command. The default
	implementation does nothing. Immediately after this method has been called, the start()
	method of the Debugger interface is called to indicate the start of debugging mode.
	Apps may choose to only overload one of these methods.
	@param	engine		the engine that called the method
	@see				Debugger
	*/
	virtual void		enterDebugMode (Engine& engine);
	/**
	The engine has left debugging mode. This callback is called when the engine leaves
	debugging mode due to a RUN command from the debugging API or because it has terminated
	execution. The app may choose to leave the debugger UI active, disabling some controls
	to allow the user to press some Break button. Immediately before calling this method,
	the engine calls Debugger::stop() to signal the debugger that it should disable itself.
	Apps may choose to implement either of these methods. An additional boolean value tells
	you whether the leaving of the debug mode is a soft exit, which means that the user issued
	a stepping command rather than a RUN/STOP command.
	@param	engine		the engine that called the method
	@param	soft		set to true if soft exit (after a stepping command)
	@see				Debugger
	*/
	virtual void		leaveDebugMode (Engine& engine, bool soft);
	/**
	An undefined property has been found. This callback gives the option to handle the error differently
	from the default implementation, which signals an error on global properties, but not on object
	properties. The property identification is supplied along with the storage location, so an undefined
	value may very well be set inside this callback; alternatively, the method may set an error code of
	its own before returning. This method cannot determine the original name of the object, nor can it
	check whether the property should contain a function. It is simply called when a property is undefined,
	nothing else.
	@param	engine		the engine that called the method
	@param	objRef		the owner of the property
	@param	propName	the property name
	@param	propValue	the property value (preset to undefined)
	@return				true if the error has been handled completely, false if the engine should handle it
	*/
	virtual	bool		undefinedError (Engine& engine, const ScCore::Variant& objRef, 
										const ScCore::Variant& propName, ScCore::Variant& propValue);
	/**
	Ask to return a class object for the given class name. ExtendScript calls this method when
	the lookup for a global variable whose name starts with an upper case letter fails. The
	user may choose to return a LiveObject that acts as the class object for the given class.
	If the LiveObject can act as a constructor (its create() method is overloaded), or as a
	callable function (a kJSDirectInvoke calls is implemented), the kIsFunction bit must be
	be set in the returned flags. If the object is a static object like Math, do not set it.
	If the engine should call this callback every time it encounters the given symbol, set
	the kAskAgain bit. This method is called before isValidClassName() is called.
	@param	engine		the engine that called the method
	@param	className	the class name to return the class object for
	@param	ioFlags		flag bits to describe the call and return value
	@return				the class object to use, or NULL if no class object
	*/
	virtual	ScCore::LiveObject*	getClassObject (Engine& engine, const ScCore::String& className, scuint32_t& ioFlags);
	/**
	Check whether the given class name is known to the application. When looking up a global
	property, ExtendScript also calls this method. If the method returns true, it creates a
	dummy constructor function of that name in the global namespace. This makes the instanceof
	operator work correctly, even for classes that ExtendScript has not seen yet.
	Note that this method is only called for property names that start with a capital letter.
	@param	engine		the engine that called the method
	@param	clsName		the class name in question
	@return				true if the class name is a valid class name
	*/
	virtual	bool		isValidClassName (Engine& engine, const ScCore::String& clsName);
	/**
	General notification callback. The engine calls this API on various occasions. See the
	enumeration constants above.
	@param	engine		the calling engine
	@param	id			the event ID
	@param	data		event specific data
	*/
	virtual	void		engineNotify (Engine& engine, scint32_t eventID, intptr_t data);
};

}	// end namespace

#include "ScPostConfig.h"

#endif

