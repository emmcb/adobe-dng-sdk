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
#ifndef _EsScriptContainer_h
#define	_EsScriptContainer_h

#include "ExtendScript.h"
#include "ScPreConfig.h"

namespace ScScript
{

/**
The ZString constant for the error text. %1 is the file name or the ID, %2
is the line number. %3 is the source code line, and %4 is the error message.
*/

#define ES_ERROR_ZSTRING	"$$$/CT/ExtendScript/UI/ScriptError=Error in %1^nLine %2: %3^n%4"

class ScriptContainerData;

/**
The ScriptContainer class is a container of scripts. It has a compile() method
that returns any compilation errors. In order to compile stuff, the ScriptContainer
class contains its own engine instance. The method adds the newly compiled script
to the list of compiled scripts. There are methods to compile strings as well as
source files.Furthermore, there is a execute() method that executes all of the 
script in compilation order.
<p>
Any compilation or execution errors are stored in a local Error instance. If this
instance contains any error, additional scripts cannot be compiled, nor can the
compiled scripts be executed. To display the error, use the errorAlert() method.
The text displayed by this alert box is defined by the ZString constant ES_ERROR_ZSTRING.
<p>
Sometimes, a script attempts to call a function that is defined in a script which has been 
loaded after that script. This forward reference would normally result in a runtime error.
If you call the load() method before calling execute(), all static function declarations
(of the form "function f()", not the form "f = function()") are preloaded into the engine.
<p>
For security reasons, the class will not load hidden files or folders, nor will it load files
or folders whose name begins with a dot character (like e.g. ".illustrator.jsx"), because
these files also may be considered as hidden on the Unix-based filesystem on the Mac.
*/

class ES_API ScriptContainer : public ScCore::Root
{
	/// Private copy ctor, because this class has pointer data members.
							ScriptContainer (const ScriptContainer&);
	/// Private assignment operator, because this class has pointer data members.
	ScriptContainer&		operator= (const ScriptContainer&);

public:
	/**
	This callback function is an optional rgument to the compile() method. It is called
	on every file and folder that is about to be loaded and compiled, and if it returns 
	false, the file or folder is not compiled. This makes it possible to exclude scripts 
	or entire folders from compilation.
	*/

	typedef	bool			compileCallback (const ScCore::FileSpec& file);

	/// The default ctor stops compilation or execution on errors.
							ScriptContainer();
	/// Specify how to handle errors. If true, continue to load and/or to execute on errors.
							ScriptContainer (bool continueOnErrors);
							~ScriptContainer();
	/// Get the number of scripts.
			long			length() const;
	/// Get a compiled script.
			Script*			getScript (scint32_t index) const;
	/// Get the embedded Error instance.
			ScCore::Error&	getError() const;
	/// Does this container continue on errors?
			bool			isContinueOnError() const;
	/** 
	Read and compile a file or folder. For each file, the current directory is set to the
	parent directory of the file so #include statements can be relative to the file being
	compiled. If the spec is a folder, all files with the extensions .jsx and .jsxbin are
	loaded. A file can either contain source code, or a precomiled script.
	@param	file			the descriptor of the file or folder to compile. Aliases are resolved.
	@param	recurse			true to load folders recursively
	@param	cb				an optional callback to suppress compilation of files and folders
	@return					an error code
	*/
			scerror_t		compile (const ScCore::FileSpec& spec, bool recurse = false, compileCallback cb = NULL);
	/** 
	Compile a string.
	@param	src				the source code to compile
	@param	id				the ID to associate with the source code (for debugging with the Toolkit)
	@return					an error code
	*/
			scerror_t		compile (const ScCore::String& src, const ScCore::String& id);
	/**
	Load the static function definitions of all compiled scripts in order of compilation.
	This call can be used before running execute() to resolve forward references to functions.
	Only globally declared function of the form "function f()" are declared.
	@param	engine			the engine to use
	@return					an error code
	*/
			scerror_t		load (ScScript::Engine& engine);
	/**
	Execute all compiled scripts in order of compilation.
	@param	engine			the engine to use
	@param	timeout			an optional timeout for each script (default is ten seconds)
	@return					an error code
	*/
			scerror_t		execute (ScScript::Engine& engine, scint32_t timeout = 10000);
	/**
	Insert a script.
	@param	script			the script to insert
	@param	index			the insertion position, -1 to append
	*/
			void			insert (Script& script, scint32_t index = -1);
	/**
	Add a second script container. If prepend is true, the container is inserted
	before the current scripts.
	@param	container		the container to add
	@param	prepend			true to prepend, false to append
	*/
			void			add (const ScriptContainer& container, bool prepend = false);
	/**
	Erase the contents of this container.
	*/
			void			erase();
	/**
	Return a standardized error message based on the embedded error.
	*/
			ScCore::String	errorMessage() const;
	/**
	Display the embedded error as an error alert box.
	*/
			void			errorAlert() const;
	/**
	Static helper method: generate a standardized error message. If the supplied Error instance
	does not contain any error, the returned string is empty. Also, the error is logged into 
	{appdata}/Adobe/Scripting.log as "SCRIPT EXECUTION ERROR" with a time stamp.
	@param	error		the error to display
	*/
	static	ScCore::String	errorMessage (const ScCore::Error& error);
	/**
	Static helper method: display a standardized error alert. If the supplied Error instance
	does not contain any error, nothing happens.
	@param	error			the error to display
	*/
	static	void			errorAlert (const ScCore::Error& error);

private:
	ScriptContainerData*	mData;
};

}	// end namespace

#include "ScPostConfig.h"

#endif

