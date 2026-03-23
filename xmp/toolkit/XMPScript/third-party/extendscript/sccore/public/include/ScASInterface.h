/**************************************************************************
*
* ADOBE CONFIDENTIAL
* ___________________
*
*  Copyright 2008 Adobe Systems Incorporated
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
#ifndef _ScASInterface_h
#define _ScASInterface_h

#include "ScDefs.h"
#if !defined( SC_UNIX )
// Unix not supported - adbeape lib missing.

#include "ScLiveCollection.h"

namespace ScScript
{
	class Engine;
}

namespace ScCore
{

class ASInterfaceData;

enum
{
	/**
	Flash event handling invokes two mechanisms. The first is a broadcast of 
	a kLiveMsgFlashEvent DataMessage containing a LiveObject that wraps the 
	AS object that sent the event. If that broadcast returned false because
	it was not processed, a JavaScript method "onFlashEvent (eventObject)" 
	is called at the interface object (if defined) to handle the message. 
	The JavaScript method may return true to indicate that the event has 
	been handled.

	<p>If the ASInterface wrapper object for the Flash movie did not handle
	the event, it is passed on to the next registered object model extension.
	*/
	kLiveMsgFlashEvent = 0x61734645,	// 'asFE'
	/**
	4.1.8: The ASInterface instance broadcasts this message when ActionScript
	code enters one of the host callback interfaces. This lets the host do pre-
	processing work before the code accesses its DOM. The message is a DataMessage
	with the Variant inside the message containing the ID of the callback (see
	the definition of IDs in the ASInterface class).
	*/
	kLiveMsgASEnterHost = 0x61734548,	// 'asEH'
	/**
	4.1.8: The ASInterface instance broadcasts this message before ActionScript
	code returns from one of the host callback interfaces. This lets the host do
	post-processing work after the code accessed its DOM. The message is a 
	DataMessage with the Variant inside the message containing the ID of the 
	callback (see the definition of IDs in the ASInterface class).
	*/
	kLiveMsgASLeaveHost = 0x61734C48,	// 'asEH'
};

/**
The ASInterface is the only interface to the ActionScript system of a Flex movie.
It needs to be created once per Flash movie. In the ScriptUI world, the FlashPlayer
widget automatically creates an "actionscript" property containing this LiveObject.
In an environment without ScriptUI, the host is responsible for creating this object.
The object should be created after the Flash player instance has been created, but
before the movie is loaded, since movie initialization code may already wish to talk
to the host.

<p>In a non-ScriptUI world, this LiveObject can still be used, even without an
ExtendScript engine. You need to create the component on your own and supply
a global LiveObject if desired. This activates the ActionScript HostObject, and you
can use the LiveObject to talk to ActionScript code and vice versa.

<p>The first argument is the APEPlayerRef of the Flash player instance, which is
cast to a intptr_t to avoid having to include the APE headers. The second
argument is the fully qualified package name of the application as Flex knows it, 
like "com.adobe.myapp". The ActionScript glue code uses this package name to look 
up SWC wrappers for host objects. If, for example, a returned host object is a 
Document object, the wrapper code would try to instantiate a com.adobe.myapp.Document
object.

<p>There are two constructors; the first constructor assumes that the code is called
from a executing ExtendScript program. In that case, it extracts and uses the currently
active engine's global object as the visible root object to ActionScript. The second
constructor expects the root object to use as a third argument, if any. Both constructors
have an optional fourth argument that, if set to true, registers the extension as an
additional extension. There can only ne one main extension, but many additional extensions.
Objects of additional extensions must always be accessed via their full package name.

<p>It is important to invalidate this object before unloading the Flash Player movie
to break internal connections and to invalidate pointers.
*/

class SC_API ASInterface : public LiveObject
{
public:
	/**
	These constants are used as part of the data messages broadcast
	during a kLiveMsgASEnterHost or kLiveMsgASLeaveHost message.
	*/
	enum
	{
		/// Get root host object
		kIDGetRootObject			= 1,
		/// Create host object
		kIDCreateObject				= 2,
		/// Get a host object's class name
		kIDGetClassName				= 3,
		/// Disconnect a hoist object from its ActionScript wrapper
		kIDDisconnectObject			= 4,
		/// Retrieve the nature of a host object property (r/o? r/w? method?)
		kIDGetPropertyNature		= 5,
		/// Get the value of a host object property
		kIDGetProperty				= 6,
		/// Set the value of a host object property
		kIDSetProperty				= 7,
		/// Invoke a hsot object method
		kIDInvokeMethod				= 8,
		/// Delete a host object property
		kIDDeleteProperty			= 9,
		/// Dispatch a Flash event to the host
		kIDDispatchFlashEvent		= 10,
		/// Get the host class object for a certain class
		kIDGetClassObject			= 11,
		/// Iterate through the properties of a host object
		kIDIterateObject			= 12,
		__HostCallback_32bit__		= 0x7FFFFFFF
	};

	/**
	Before using ASInterface objects, the system must be initialized, and the APE
	DLL must be attached correctly. Call this static method to attach the APE DLL.
	Returns 0 if the ASInterface cannot be used due to a missing or wrong APE DLL, 
	the version of the APE HostObject interface otherwise. The method may be called 
	any number of times. Note that this method does not initialize the APE DLL; it
	is assumed that the calling app already loaded and initialized the APE DLL. The
	ASInterface constructors call this method internally.
	*/
	static	scint32_t		getVersion();
	/**
	Retrieve the currently executing ExtendScript engine. This call returns the engine
	if the currently executing code is invoked from withing the execution of an engine,
	either because a script is being executed, or a get/put/call operation is executed.
	This call is handy to retrienve the engine pointer if the DOM is to be connected
	with an engine.
	*/
	static	ScScript::Engine* getCurrentEngine();
	/**
	The constructor takes an intptr_t which is actually the APEPlayerRef of the
	Flash Player instance, as well as the fully qualified package name of the extension.
	This method extracts and uses the global object of the supplied ExtendScript engine.
	If the registration of the DOM extension fails, the object's isValid() method returns 
	false; it is a good idea to check for that state after constructing the object.
	@param	playerRef		the APEPlayerRef of the Flash Player
	@param	pkgName			the fully qualified package name
	@param	engine			the ExtendScript engine whose global object to use
	@param	isAdditonal		false for the main extension, true for additional extensions
	*/
							ASInterface (intptr_t playerRef, const String& pkgName, 
										 ScScript::Engine& engine, bool isAdditonal = false);
	/**
	This constructor passes in the root object to use. If the root object is NULL, there 
	will not be a root object to use from ActionScript, effectively disabling accesses 
	from ActionScript to the host; access from the host to ActionScript is still possible, 
	though.	If the registration of the DOM extension fails, the object's isValid() method 
	returns false; it is a good idea to check for that state after constructing the object.

	<p>Plugins can use this constructor to register their own object model extension.
	In that case, set the isPlugin argument to true. There can only be one main
	object model extension, but there may be any number of plugin extensions. The
	package name of every extension must be unique.

	@param	playerRef		the APEPlayerRef of the Flash Player
	@param	pkgName			the fully qualified package name
	@param	rootObj			the root object that ActionScript can see for the app name
	@param	isAdditonal		false for the main extension, true for additional extensions
	*/
							ASInterface (intptr_t playerRef, const String& pkgName, 
										 LiveObject* rootObj, bool isAdditonal = false);

	virtual					~ASInterface();
	/**
	Disconnect this and all connected objects from the Player. Use this method if
	early disconnection is desired. This method also makes the object unusable for 
	scripts.
	*/
			void			invalidate();
	/**
	Return the APEPlayerRef that is the Flash Player instance. This value is 0
	if the object is invalid.
	*/
			intptr_t		getAPEPlayerRef() const;
	/**
	Retrieve the package name as passed in to the constructor.
	*/
	const	String&			getPackageName() const;
	/**
	 * Get the host's root object. This can either be a LiveObject, or an
	 * ExtendScript object wrapper (kJSObject).
	 */
			void			getRoot (Variant& root) const;
	/**
	Retrieve the ActionScript root object. Store any errors into the
	supplied Error instance if present. The returned LiveObject must
	NOT be released after use.
	@param	errors			if present, take any errors
	@return					the root object or NULL on errors or missing app object
	*/
			LiveObject*		getASRoot (Error* errors = NULL) const;
	/**
	Retrieve the ActionScript Application object. This object contains the public
	declarations in the MXML file. It can be used to invoke functions defined in
	the MXML file. Note that the object is NULL for Flash SWFs. Store any 
	errors into the supplied Error instance if present. The returned LiveObject
	must NOT be rleased after use.
	@param	errors			if present, take any errors
	@return					the application object or NULL
	*/
			LiveObject*		getASApplicationObject (Error* errors = NULL) const;
	/**
	Create an ActionScript object and return its wrapper LiveObject.
	A weak reference does not lock the AS object to the LiveObject;
	if the AS object is ganbage collected, the wrapping LiveObject is 
	automatically invalidated.
	@param	clsName			the fully qualified ActionScript class name
	@param	args			constructor arguments
	@param	weakRef			if true, create a weak reference
	@param	errors			optionally, takes any errors
	@return					the wrapper object, or NULL on errors
	*/
			LiveObject*		createASObject (const String& clsName, const Array& args, bool weakRef=false, Error* errors=NULL);
	/**
	Retrieve an ActionScript class object. This class object is needed to access
	the static properties and methods of a class. To create instances of a class,
	use the LiveObject's call() method with the predefined ID kJSDirectInvoke.
	@param	clsName			the fully qualified ActionScript class name
	@param	errors			optionally, takes any errors
	@return					the wrapper object, or NULL on errors
	*/
			LiveObject*		getASClassObject (const String& clsName, Error* errors=NULL);

	virtual	scerror_t		getIDForName (const String& name, scint32_t& id, InfoType& type,
										  scint32_t lang = kJavaScriptLanguage) const;
	virtual	scerror_t		enumerate (SimpleArray& propertyIDs, scint32_t langAndBits = kJavaScriptLanguage) const;
	virtual	scerror_t		get (scint32_t id, Variant& value, Error* errors = NULL) const;
private:
	friend	class			APE;
			ASInterfaceData* mData;
	LivePropertyManager		mMgr;
};

}

#endif
#endif
