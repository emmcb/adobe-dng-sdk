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
#ifndef _EsEngine_h
#define	_EsEngine_h

#include "ScCore.h"
#include "ScPreConfig.h"

namespace ScScript
{

class EngineData;
class Callback;
class Dispatcher;
class Debugger;
class Engine;
class Script;

/**
The base class for additional APIs contains the engine that it belongs to.
The class is insofar thread safe that the engine it operates upon must be
in a single thread.
*/

class ES_API API : public ScCore::Root
{
protected:
	scint32_t		iName;
	Engine&			iEngine;
					API (Engine& eng, scint32_t name) : iName (name), iEngine (eng) {}
public:
	/// Retrieve the name of this API.
	inline scint32_t	getName() const { return iName; }
	/// Retrieve the engine associated with this API.
	inline Engine&	getEngine() const { return iEngine; }
};

/**
The error information is part of the ExtendScript engine and can be retrieved 
with Engine::getErrorInfo(). For start column and length, only values up 
to and including 65535 are valid. For larger values, the value remains 
65535. iOffset, iStart and iLength are supported for compile errors
only.
<p>
This structure is deprecated. You should use ScCore::Error instead, which
provides more detailed error information if applicable.
<p>
This class is insofar thread safe that it belongs to a specific engine.
*/

class ES_API RuntimeError : public ScCore::Root
{
public:
	/// The explanatory error text
	ScCore::String		iText;
	/// The source code of the scriptlet the line number refers to.
	ScCore::String		iSource;
	/// The line where the error occured, 0-based, relative to the source code.
	scint32_t			iLine;
	/// The character offset of the erroneous symbol.
	scint32_t			iOffset;
	/// The numeric error code.
	scerror_t			iCode;
	/// The starting column of the erroneous symbol.
	scuint16_t			iStart;
	/// The length of the erroneous symbol.
	scuint16_t			iLength;
	///
						RuntimeError();
	///
						RuntimeError (const RuntimeError&);
	virtual				~RuntimeError();
	///
	RuntimeError&		operator= (const RuntimeError&);
	/// Store the given extended error into this info.
	void				set (const ScCore::Error& r);
};

/** Debugging flags.
To see any debugging output, you need to define a File object to take the
output, and to define that File in the DebugAPI instance of this engine.
*/
enum DebugFlags
{
	/// Trace tokens as they are being executed.
	kTraceToken				= 0x00000001,
	/// Trace line numbers.
	kTraceLines				= 0x00000002,
	/// Dump generated code.
	kDumpCode				= 0x00000004,
	/// Break into C++ debugger on errors.
	kBreakOnErrors			= 0x00000008,
	/// Do not optimize constant expressions (compiler).
	kDontOptimizeExprs		= 0x00000010,
	/// Cause the lexer to dump tokens.
	kDumpScanner			= 0x00000020,
	/// Do a GC as often as possible (debugging).
	kMoreGC					= 0x00000040,
	/// Trace function calls.
	kTraceCalls				= 0x00000080,
	/// Generate extended errors: kErrReadOnly and kErrUnsupportedElement.
	kExtendedErrors			= 0x00000100,
	/// Localize on. Same as setting $.localize.
	kLocalize				= 0x00000200,
	/// Do not break if a runtime error is recorded in a try block.
	kNoBreakOnException		= 0x00000400,
	/// Display an alert on setting $.level.
	kAlertOnSetDebugLevel	= 0x00000800,
	/// OBSOLETE - do not use.
	kDebugJava				= 0x00001000,
	/// RESERVED - DO NOT USE.
	kReserved				= 0x70000000,
	/// The following flags are settable by the ESTK via remote command.
	kESTKFlags				= kTraceToken | kTraceLines | kNoBreakOnException,
	__DebugFlags__32bit__	= 0x7FFFFFFF
};

/// The message ID broadcast before an engine shuts down.
#define kMsgEngineShutdown 0x656E5368	// 'enSh'

/**
This is the base class for a scripting engine. All scripting engines
derive from that class. Also, this class is the official interface to the
scripting system. Setting up a scripting system is as easy as creating an
instance of a given class by using createEngine(), adding scriptlets to
the engine and starting one or every scriptlet.
<p>
Optionally, you can append a user data object. It will be deleted by the 
destructor if the autodelete flag is set to true. Also, a Callback object 
may be speficied which the engine uses to communicate back to the application. 
The engine uses this interface to control the display of the debugger
and to notify the app about certain states of its execution.
<p>
Objects can be created on the fly by calling findObject() and setting the second parameter
to true. As long as you keep an object in a Variant, it cannot be garbage collected.
<p>
Normally, the engine runs forever when activated. A dead loop in the script causes the
application to hang. You can avoid this situation in two ways. First, the engine calls
Callback::running() at regular intervals. Here, you may set a runtime error or to break
into the debugger. Also, a default timeout may be set that is activated on all calls that
cause the engine to start running. For the eval() methods, individual timeouts
may also be set. On a timeout, a runtime error is generated.
<p>
The engine is guaranteed to handle out-of-memory conditions gracefully. All public methods
catch the exception std::bad_alloc and convert it to a runtime error kErrNoMemory. Note,
however, that this class is the only class with this guaranteed behavior. All other classes
pass std::bad_alloc to the caller.
<p>
ExtendScript engines may have their own localizer. If this is so, the engine switches the
current localizer to the "local" localizer before executing a script. After the call, the 
engine switches back to the previous localizer.
<p>
In a multithreading environment, engines are not reentrant by multiple threads. Only one
thread at a time can execute a script or access the engine APIs. If you need to share an
engine between threads, use the locking methods inherited from the Lockable class to
create and maintain a lock, and from Broadcaster so it broadcasts a kMsgDeleted message. 
The list of all engines as well as the pointer to the current engine are thread local. 
The pointer to the current engine is public and part of the ScCore::Context object.
<p>
On shutdown, the engine broadcast a kMsgEngineShutdown message before actually shutting
down. This broadcast can e.g. be used to erase Variants that contain kJSObject pointers,
since the engine will tear down these objects, leaving dangling pointers in these Variants.
*/

class ES_API Engine : public ScCore::Lockable, public ScCore::Broadcaster
{
	/// Private copy ctor, because this class has pointer data members.
							Engine (const Engine&);
	/// Private assignment operator, because this class has pointer data members.
	Engine&					operator= (const Engine&);

	ScCore::Root*			mUserData;			// user data object
	ScCore::Error			mError;				// any runtime error
	EngineData*				mData;				// pointer to misc data
	scint32_t				mDebugFlags;		// the debugging flags
	scint32_t				mDebugLevel;		// the debugging and profiling levels
	bool					mUserDel;			// Whether to delete the user data or not.
protected:
							Engine();
public:
	/// Engine types.
	enum Type
	{
		/// A JavaScript engine.
		kJavaScript		= ScCore::kJavaScriptLanguage,
		__Type__32bit__	= 0x7FFFFFFF
	};

	/**
	Flags to control the way eval(), get(), and call() work. Use these flags as arguments
	to the respective Engine methods.
	*/

	enum
	{
		/// Coerce any resulting object reference to a string by calling ref.toString().
		kEvalCoerceToString		= 0x0001,
		/// Coerce any resulting object reference to a scalar by calling ref.valueOf() and possibly ref.toString().
		kEvalCoerceToValue		= 0x0002,
		/// obsolete.
		kEvalLockObject			= 0,
		/// obsolete.
		kEvalBreakOnEntry		= 0,
		/// Eval flag: force the scriptlet ro execute in the global scope (important for var declarations).
		kEvalInGlobalScope		= 0x0010,
		/// Obsolete.
		kEvalCheckForReference	= 0,
		/** Force the return of an error object in case of errors. Make sure that you erase the
		result before deleting the engine. */
		kEvalWantErrorObject	= 0x0040,
		/// Do not execute the code; just load the static function definitions.
		kEvalLoadFunctionsOnly	= 0x0080,
		/// Construct a new object; this is for call() (from 4.0.2 on).
		kEvalConstruct			= 0x0100,
		__Eval__32bit__			= 0x7FFFFFFF
	};

	/**
	The ExtendScript version number is a 32 bit integer with four sub-fields.  It consists
	of a "debug vs. release" field, a "major version" field, a "minor version" field,
	and an "engineering" or "build number" field.  The highest bit (0x80000000) of
	the integer is set to 1 for debug builds of ExtendScript and is set to 0 for release
	builds.  The next highest 15 bits (0x7fff0000) are the major version.  The next
	highest 8 bits (0x0000ff00) are the minor version.  The lowest 8 bits (0x000000ff) are 
	the engineering version.
	<p>
	On Windows the ExtendScript version number number is displayed in the ExtendScript.dll file
	by looking in the Properties dialog (right-click on the ExtendScript.dll file
	and choose "Properties" from the pop-up menu).  In this dialog, look at the "Version" panel. 
	The version number is displayed as the "File version:"
	<p>
	On Macintosh the version number can be seen by selecting the ExtendScriptLib file in the
	finder and choosing "Get Info" from the "File" menu.  In that dialog (in
	the "General Information" panel) the version number is displayed at the
	beginning of the "Version:" field.
	<p>
	The versioning scheme is the same as for all CoreTech components.
	@return				the version
	*/
	static	scint32_t	getVersion();
	/**
	Create an engine of a given type.
	@param	type		the engine type
	@return				the new engine or NULL for unknown types
	*/
	static	Engine*		createEngine (Type type = kJavaScript);
	/**
	Fill an array with all engines that are known to this thread.
	*/
	static	void		getAll (ScCore::TSimpleArray<Engine>& engines);
	/**
	Return the currently active engine. If this pointer is NULL, no engine
	is running. Use this method to find the active engine e.g. within a
	LiveObject.
	*/
	static	Engine*		getCurrent();
	/**
	Invalidate all objects for the given class name. This method walks the
	list of engines and calls invalidateClass() on each engine. The call can 
	be used to remove any reference to any object of the given class when 
	unloading a DLL that contains LiveObject code. Note that this call is
	thread local, not global for all threads.
	*/
	static	void		invalidateClassAll (const ScCore::String& clsName);
	/**
	Create a new engine containing the same dispatchers and other characteristics
	as this engine. The engine name is not cloned.
	*/
	virtual scerror_t	clone (Engine** targetLocation) const;
	/**
	Issue a garbage collection call on all engines known to the thread.
	*/
	static void			gcAll();
	/**
	Destroy the engine. It is an absolute error to destroy the engine while
	it is busy; this is a safe way to crash your system. No dispatchers are
	destroyed.
	*/
	virtual				~Engine();
	/**
	Get the engine ID. This is a application unique 32-bit ID.
	*/
	scuint32_t			getID() const;
	/**
	Retrieve the user data object.
	@return				the user data object if set
	*/
	inline	ScCore::Root* getUserData() const { return mUserData; }
	/**
	Set the user data object.
	@param	date		the data object
	@param	autoDel		set to true if the dtor should delete the data as well
	*/
	inline	void		setUserData (ScCore::Root* obj, bool autoDel) { mUserData = obj; mUserDel = autoDel; }
	/** 
	Retrieve the name of the engine. This should be a script compatible name.
	*/
	const	ScCore::String&	getName() const;
	/** 
	Set the name of the engine. This should be a script compatible name.
	@param	name		the new name
	@return				false if the name already is taken
	*/
			bool		setName (const ScCore::String& name);
	/**
	Retrieve an engine owned by the calling thread by name.
	@param	name		the name
	@return				the engine or NULL
	*/
	static	Engine*		findEngine (const ScCore::String& name);
	/**
	Return the current callback object.
	@return				the callback object
	*/
			Callback*	getCallback() const;
	/**
	Define a callback object which the engine uses to communicate a number of events
	back to the application. The object does not belong to the engine and thus may be
	installed into a number of engines. Setting the callback to NULL is OK; this is
	actually the default.
	@param	obj			the callback object or NULL
	*/
			void		setCallback (Callback* obj);
	/**
	Get the engine-specific localizer object. This localizer object overrides the global
	localizer object. This value is initially NULL.
	*/
			const ScCore::Localizer* getLocalizer() const;
	/**
	Set an engine-specific localizer object. This localizer object overrides the global
	localizer object. The object does not belong to the engine and thus may be
	installed into a number of engines. Setting the localizer to NULL is OK; this is
	actually the default. The engine never deletes this localizer.
	*/
			void		setLocalizer (const ScCore::Localizer* p);
	/**
	Not functional - please use Dictionary::get (engine->getName()).
	Returns NULL.
	*/
	virtual	ScCore::Dictionary* getDictionary() const;
	/**
	Not functional - please name the dictionary and use Dictionary::set (dict)
	*/
	virtual	void		setDictionary (ScCore::Dictionary*);
	/**
	Get the stack limit. This is the maximum stack size in bytes. The default value is 256 K.
	@return				the limit
	*/
	virtual	scint32_t	getStackLimit() const = 0;
	/**
	Set the stack limit. This is the maximum stack size in bytes. The default value is 256 K,
	but you may want to reduce that limit if stack space is precious.
	@param	limit		the stack size; sizes < 32 K are set to 32 K.
	*/
	virtual	void		setStackLimit (scint32_t limit) = 0;
	/// Clear an error condition
	virtual	void		clearError();
	/**
	Raise an error condition. If line < 0, leave the original line. If fullText is true,
	assume that moreInfo already contains the full, localized text.
	@param	code		the error code
	@param	moreInfo	additional info or the full error message
	@param	line		the line number (-1 to leave as is)
	@param	fullText	if true, moreInfo contains the full message
	*/
	virtual	void		setError (scerror_t code, const ScCore::String& moreInfo, scint32_t line=-1, bool fullText=false);
	/**
	Raise an error condition returned by a LiveObject. If line < 0, leave the original line. 
	@param	code		the error code
	@param	liveObj		the LiveObject
	@param	id			the LiveObject property ID
	@param	isIndex		true if the propertyID is an index ID
	@param	line		the line number (-1 to leave as is)
	*/
	virtual	void		setError (scerror_t code, const ScCore::LiveObject& liveObj, scint32_t id, bool isIndex = false, scint32_t line=-1);
	/// Raise an error condition.
	virtual	void		setError (scerror_t code);
	///
	virtual	void		setError (const RuntimeError& info);
	/// The new interface with more complex error information.
	virtual	void		setError (const ScCore::Error& error);
	/// Retrieve the error info block
	virtual	const RuntimeError& getErrorInfo() const;
	/// Retrieve the extended error information.
	virtual	const ScCore::Error& getError() const;
	/// Check for any errors.
	virtual	bool		good() = 0;
	/**
	Reset this engine. The errors are cleared, and the global object is reinitialized.
	All variables and objects are erased. Scriptlets are not affected. Variables not reset are
	<ul>
	<li>the garbage collection lock
	<li>the global timeout
	</ul>
	*/
	virtual	void		reset() = 0;
	/**
	Find the object for the given object reference. The reference is set up with the ID and
	class name. If the object exists, the object reference is updated and the method returns true.
	Afterwards, the object data pointer may be extracted if present. Object references with the ID
	StdIDs::kDontCare receive a special treatment. If the object reference contains a data pointer,
	the object is searched for by data pointer rather than by ID. If the ref does not contain a data
	pointer and no ID, the lookup fails. If the create flag is set, the object is created if not found.
	@param	objRef		the object reference
	@param	create		creates the object if not found
	@return				true if the object was found or created
	*/
	virtual bool		findObject (ScCore::Variant& val, bool create = false) = 0;
	/**
	Invalidate the given object. Call this method when a host object dies, and you want the
	ExtendScript proxy objects to become invalid. All further accesses to an invalidated
	object will generate the runtime error kErrInvalidObject. Any attached LiveObject is 
	invalidated and disconnected.
	@param	objRef		the object to invalidate
	*/
	virtual	void		invalidateObject (const ScCore::Variant& objRef) = 0;
	/**
	Delete all properties that contain the given object reference. This call effectively
	destroys all occurrances of a given object and may be used when a host object goes away.
	Object with a standard ID cannot be deleted. Any attached LiveObject is disconnected.
	@param	objRef		the object to delete
	*/
	virtual	void		deleteObject (const ScCore::Variant& objRef) = 0;
	/**
	Delete the properties of an object. This method removes all properties of an object unless
	they are declared as non-deletable or aas a constructor function property. Not resursive.
	Use a level of 0 to erase all variables. A level of 1 also erases variables declared with the
	"var" keyword (this affects the global namespace in Netscape flavor only). Level 2 also erases
	any function definitions.
	@param	objRef		the object reference
	@param	level		the level of deletion
	*/
	virtual	void		deleteProperties (const ScCore::Variant& objRef, int level = 0) = 0;
	/**
	Lock this engine against garbage collection. This method maintains an internal lock count,
	so you can call it multiple times. If the garbage collection is disabled, you need to call
	gc() manually on order to force a garbage collection.
	@param	lock		if true, lock, else unlock
	@return				true if the engine is locked, false otherwise
	*/
	virtual	bool		lockGC (bool lock) = 0;
	/**
	Do a garbage collection.
	*/
	virtual	void		gc() = 0;
	/**
	Get a list of all enumerable properties as an array of sIDs. Use the DataPool
	class to get the string values for the returned sIDs.
	@param	objRef		the object to enumerate
	@param	props		the array of sIDs to fill
	*/
	virtual	void		enumProperties (const ScCore::Variant& objRef, ScCore::SimpleArray& props) = 0;
	/**
	Get a property. This method waits forever without any timeout, so it may hang eventually, unless the
	default timeout has been set with setTimeout(). Returns undefined if the property does not exist.
	@param	objRef		the reference to the object being queried. The object must exist.
	@param	name		the property name
	@param	value		the location of the value
	@param	flags		optional eval flags (kEvalInGlobalScope is ignored)
	@return				true if the object was found
	*/
	virtual bool		get (const ScCore::Variant& objRef, const ScCore::Variant& name, ScCore::Variant& value, int flags=0) = 0;
	/**
	Set a property. This method waits forever without any timeout, so it may hang eventually, unless the
	default timeout has been set with setTimeout(). 
	@param	objRef		the reference to the object being accessed. This object must exist.
	@param	name		the property name
	@param	value		the value to store
	@param	flags		optional eval flags (obsolete, ignored)
	@return				true if the object was found
	*/
	virtual bool		put (const ScCore::Variant& objRef, const ScCore::Variant& name, const ScCore::Variant& value, int flags=0) = 0;
	/**
	Call a method. The method is identified by the object reference and the method name.
	This method waits forever without any timeout, so it may hang eventually, unless the
	default timeout has been set with setTimeout(). The method returns false without setting 
	an error code when the function does not exist.
	@param	objID		the object reference
	@param	name		the method name
	@param	args		the argument list
	@param	flags		optional eval flags (kEvalInGlobalScope is ignored)
	@param	result		the value which takes the result.
	@return				false if the method does not exist or on other errors
	*/
	virtual	bool		call (const ScCore::Variant& objRef, const ScCore::String& name, 
							  const ScCore::BasicArray& args, ScCore::Variant* result=NULL, int flags=0) = 0;
	/**
	Call a method. The method is assumed to be the object reference. This method can be
	used to call a supplied callback method. The call is executed within the current context.
	This method waits forever without any timeout, so it may hang eventually, unless the
	default timeout has been set with setTimeout(). The method returns false without setting 
	an error code when the function does not exist. This method accepts the kEvalConstruct
	flag; in that case, the methodRef must contain the constructor function.
	@param	objID		the object reference (the method)
	@param	args		the argument list
	@param	result		the value which takes the result.
	@param	flags		optional eval flags (kEvalInGlobalScope is ignored)
	@param	thisRef		an optional object ref which "this" is set to
	@return				false if the method does not exist or on other errors
	*/
	virtual	bool		call (const ScCore::Variant& methodRef, const ScCore::BasicArray& args, 
							  ScCore::Variant* result=NULL, int flags=0,
							  const ScCore::Variant* thisRef = NULL) = 0;
	/**
	Get class information about an object. Return NULL if there is no info. 
	The default implementation fills in basic information about the object.
	@param	objRef		the object reference
	@return				a pointer to the class info or NULL if there is none
	*/
	virtual	const ScCore::ClassInfo* getClassInfo (const ScCore::Variant& objRef) = 0;
	/**
	Convert the given reference to a scalar value. If the reference is a LiveObject or an object,
	first call valueOf() if present. If either valueOf() is not present, or valueOf() returns
	itself (which is the default), call toString() to convert the object. If both calls fail,
	generate a kErrNoValue error.
	@param	toConvert	the value to convert
	@return				true if the engine is good
	*/
	virtual	bool		convertToScalar (ScCore::Variant& toConvert) = 0;
	/**
	Precompile a script to be run. The precompiled script is returned as
	a Script instance. If there were any compile errors, the engine carries 
	the error information. The script is not tied to a particular engine; 
	you can run a compiled script on any engine. On errors, you may want to
	call clearError() after dealing with the error. The supplied string may
	also be a string returned by Script::store(), containing a compiled script.
	<p>
	Optionally, you can supply a file name and a line number. The line number
	(zero-based) causes the debugger and the Error objects to report the current
	line as the sum of the line number supplied here and the offset relative
	to the beginning of the script.
	@param	source		the source code to be compiled.
	@param	file		an optional file name
	@param	line		obsolete; ignored
	@return				a Script pointer or NULL
	*/
	virtual	Script*		compile (const ScCore::String& source, const ScCore::String* file = NULL, scint32_t line = 0) = 0;
	/**
	Run a script. The script is compiled and run in the current execution context. If the source code
	string contains the name of a precompiled scriptlet, this scriptlet is run instead.	The optional 
	result pointer must be valid while the code is being executed. The method has an optional 
	timeout. If the result is an object reference, it is often desirable to have it converted to 
	a scalar value. If the kEvalCoerceToValue flag is set, the engine first calls objectRef.valueOf(), 
	and if the method again returns an object reference (which is the default for most objects), calls
	objectRef.toString() to dereference an object reference to a scalar value. Optionally, a "this"
	object reference may be supplied. The call fails if a debugging session is active. The supplied 
	timeout overrides the default timeout set with setTimeout() unless it is zero. If the timeout is zero, 
	the default timeout is used. The supplied string may also be a string returned by Script::store(), 
	containing a compiled script.
	<p>
	Optionally, you can supply a file name and a line number. The line number
	(zero-based) causes the debugger and the Error objects to report the current
	line as the sum of the line number supplied here and the offset relative
	to the beginning of the script.
	@param	script		the script source or the compiled script to execute
	@param	result		an optional value to store any result into. May be preset to true or false.
	@param	timeout		the number of msecs to wait for the result, less or equal to 0 means to wait forever
	@param	flags		optional eval flags
	@param	thisRef		the object reference to "this". Use NULL for the global object.
	@param	file		an optional file name
	@param	line		obsolete; ignored
	@return				true if executed
	*/
	virtual	bool		eval (const ScCore::String& script, ScCore::Variant* result = NULL, 
							  scint32_t timeout = 0, int flags = 0, const ScCore::Variant* thisRef = NULL,
							  const ScCore::String* file = NULL, scint32_t line = 0) = 0;
	/// this version runs a precompiled script instead of source code.
	virtual	bool		eval (Script& code, ScCore::Variant* result = NULL, 
							  scint32_t timeout = 0, int flags = 0, const ScCore::Variant* thisRef = NULL) = 0;
	/**
	From time to time, you may want to create a populated JavaScript array of some data. This method accepts
	an initialized Array instance containing the values and creates an Array object populated with these 
	values. The reference to this array is returned.
	@param	values		the array of values to populate the array with
	@param	array		the variable containing the array reference on return
	*/
	virtual	void		makeArray (const ScCore::Array& values, ScCore::Variant& array) = 0;
	/**
	This method provides a quick and convenient way to access the contents of a JavaScript array. 
	It fills the supplied Array instance with the array data from given index range. Undefined
	index entries create Array entries set to kUndefined.	If the array is shorter than the requested 
	length, the method stops.
	@param	arrayRef	the object reference returned from eval()
	@param	values		the Array array to fill
	@param	start		the first index to return
	@param	length		the number of entries to return
	*/
	virtual	void		fillArray (const ScCore::Variant& arrayRef, ScCore::Array& toFill, scuint32_t start=0, scuint32_t length=1000) = 0;
	/**
	Schedule a string of source code for later execution. This code is appended to a list of
	tasks that is checked by the poll() method. When the code is ready ro tun, it is compiled and
	executed if the engine is not busy. The code is executed in the context of the global object.
	To run a task, the engine must be initialized and not be busy.
	@param	source		the source code to execute
	@param	delay		the delay in milliseconds
	@param	repeat		set to true for repeated execution
	@return				a task ID which may be used for cancelTask()
	*/
	virtual	scint32_t	scheduleTask (const ScCore::String& source, double delay, bool repeat) = 0;
	/**
	Cancel a scheduled task.
	@param	id			the task ID
	@param	repeat		IGNORED, will be removed in 4.0
	*/
	virtual	void		cancelTask (scint32_t id, bool) = 0;
	/**
	Cancel all scheduled tasks.
	*/
	virtual	void		cancelTasks() = 0;
	/**
	Add a dispatcher object. This object is not deleted when the engine is deleted. If defining
	a global dispatcher, keep in mind that there can only be one global dispatcher. Attempting
	to define more than one global dispatcher results in the redefinition of the global dispatcher.
	All of the built-in dispatchers are engine specific. Never attempt to add a built-in dispatcher
	of one engine to another engine and then delete the engine where the dispatcher originated from!
	@param	dispatcher	the dispatcher to add
	@param	isGlobal	set this to true when you add the dispatcher for the global object class
	*/
	virtual	void		addDispatcher (Dispatcher& dispatcher, bool isGlobal = false) = 0;
	/**
	Retrieve a dispatcher object. An empty string as the class name returns the built-in dispatcher
	for the global object. The built-in dispatcher are engine specific and can be freely modified.
	Never attempt to add a built-in dispatcher of one engine to another engine and then delete 
	the engine where the dispatcher originated from!
	@param	className	the class name of the dispatcher
	@return				the dispatcher or NULL
	*/
	virtual	Dispatcher* getDispatcher (const ScCore::String& className) = 0;

	/**
	Retrieve the debugging level.
	*/
	inline	scint32_t	getDebugLevel() const { return scint32_t (mDebugLevel & 0x00FF); }
	/**
	Set the debugging level:
	<ul>
	<li>0 - no debugging (default)
	<li>1 - debug on runtime errors
	<li>2 - stop at the beginning of the program
	</ul>
	*/
			void		setDebugLevel (int level);
	/**
	Retrieve the profiling level.
	*/
	inline	scint32_t	getProfilingLevel() const { return scint32_t ((mDebugLevel >> 8) & 0x00FF); }
	/**
	Set the profiling level. The higher the level, the slower the engine runs.
	<ul>
	<li>0 - no profiling (default)
	<li>1 - function level profiling
	<li>2 - function level profiling with timing information
	<li>3 - line level profiling
	<li>4 - line level profiling with timing information.
	</ul>
	*/
			void		setProfilingLevel (int level);
	/**
	Stop the execution of this engine.
	*/
	virtual	void		stop() = 0;
	/**
	Check the state of this engine. When a program is running, the result is true.
	*/
	virtual	bool		isBusy() const = 0;
	/**
	Set the default timeout set for all all engine runs. This value default to 0, which
	stands for no timeout at all. Setting the timeout to 0 or a negative value disables
	the timeout altogether. The timeout value supplied in eval() overrides 
	the default timeout when nonzero. If zero, the default timeout takes precedence.
	@param	timeout		the new default timeout in milliseconds
	@return				the old default timeout in milliseconds
	*/
	virtual	scint32_t	setTimeout (scint32_t timeout) = 0;
	/**
	Temporarily suspend the timeout for waiting calls. This method affects the current timeout.
	Calls to this method can be nested.
	@param	wait		set to true to increment the suspend flag, false otherwise
	*/
	virtual	void		suspendTimeout (bool suspend) = 0;
	/**
	Poll the engine. This call checks for timed tasks and executes them when they are due.
	Call this method regularly in your idle or message loop.
	*/
	virtual	void		poll() = 0;
	/**
	Get the debugger object. This object may be common to all engines. 
	@return				the debugger object. May be NULL (which is the default).
	*/
	virtual	Debugger*	getDebugger() const = 0;
	/**
	Set the debugger object. This object may be common to all engines. 
	A NULL value resets the debugger to the default built-in debugger object.
	@param	debugger	the new debugger object. May be NULL.
	*/
	virtual	void		setDebugger (Debugger* debugger) = 0;
	/**
	Retrieve the debugging flags. These are the flags documented in the $.flags
	property. You shuld normally not use this call, since some flags are undocumented
	and may cause the engine to malfunction if used incorrectly.
	*/
	inline	scint32_t	getDebugFlags() const { return mDebugFlags; }
	/**
	Set the debugging flags. These are the flags documented in the $.flags
	property. You shuld normally not use this call, since some flags are undocumented
	and may cause the engine to malfunction if used incorrectly.
	*/
	inline	void		setDebugFlags (scint32_t flags) { mDebugFlags = flags; }
	/**
	Retrieve a specific API. This method returns a pointer to an object containing
	additional, customer-specific API calls. The API object belongs to the engine.
	@param	name		the name of the requested API as a four-byte constant
	@return				the API object or NULL if it is not present
	*/
	virtual	API*		getAPI (scint32_t name) = 0;
	/**
	Invalidate all objects for the given class name. This method walks the entire tree
	of objects. If an object matches the class name, its contents are cleared and it is
	set to invalid. All further accesses to that object result in a runtime error.
	The call can be used to remove any reference to any object of the given class when 
	unloading a DLL that contains LiveObject code. Any attached LiveObject is invalidated
	and disconnected.
	*/
	virtual	void		invalidateClass (const ScCore::String& clsName) = 0;
	/**
	Protect an object against garbage collection. Call this method if you create an object
	that you do not want to keep a reference to, but which you want to stay around, like
	e.g. a button which you attached an onClick() callback to. You need to call unprotectObject() 
	as often as you called protectObject().
	@param	objref		the object reference to protect
	@return				true if successful
	*/
	virtual	bool		protectObject (const ScCore::Variant& objRef) = 0;
	/**
	Unprotect a protected object reference. Use this call when you are done with an object 
	reference This call is very dangerous if you attempt to unlock an object which not has 
	been protected with protectObject(). It decrements an internal reference count. If used on
	an unprotected, this would probably crash the engine, because the object may be
	deleted too early!!!
	@param	objref		the protected object reference
	@return				true if successful
	*/
	virtual	bool		unprotectObject (const ScCore::Variant& objRef) = 0;
	/**
	Restore an error condition. Use this call instead of setError() if you just want the
	engine ot restore an error without the usual error handling, like e.g. break into
	the debugger.
	@param	error		the error to restore
	*/
	virtual	void		restoreError (const ScCore::Error& error);
	// obsolete methods.
	inline	void		lockRef (const ScCore::Variant&) {}
	inline	void		unlockRef (const ScCore::Variant&) {}
};

}	// end namespace

#include "ScPostConfig.h"

#endif


