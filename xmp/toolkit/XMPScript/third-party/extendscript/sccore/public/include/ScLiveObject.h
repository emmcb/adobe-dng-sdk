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
#ifndef _ScLiveObject_h
#define _ScLiveObject_h

#include "ScLiveBase.h"
#include "ScClassInfo.h"
#include "ScLiveProperty.h"
#include "ScMessage.h"

#include "ScPreConfig.h"

namespace ScCore
{

class LiveComponent;
class LiveObjectData;
class ClassInfo;
class LivePropertyManager;

/// the LiveID for a LiveObject.
const scint32_t kLiveIDLiveObject = 0x206F626A;	// ' obj'
/// The LiveID for the value of a LiveObject (every LiveObject can have a value).
const scint32_t kLiveIDValue		= 0x2076616c;	// ' val'

/**
A LiveObject is an opaque object containing an arbitrary set of properties.
The object acts just as a container for properties; it does not define any
way the properties are implemented. When looking for a property, you ask
for the ID of a property name. If the return value is 0, the ID of a 
property stored within the object is returned. Use this property ID to issue
property read, write or call operations. Implementations may choose to 
predefine property IDs for easier access; if the object is to be used 
within ExtendScript, you will need to implement getIDForName() so property 
names can be matched to property IDs. LiveObjects support the enumeration 
of properties, so simple object browsers may be implemented. LiveObjects 
support full inspection of objects and their elements.
<p>
You can choose one of several methods to implement the functionality of a LiveObject:
<ul>
<li>Aggregation - write LiveComponents containing your code and collect them
into LiveObjects.
<li>Derivation - override methods like get(), put() or call().
<li>Delegation - add an object as listener to receive messages that the
LiveObject broadcasts.
</ul>
Of course, you can mix and match all of these ways to implement your code.
<p>
The implementer determines the lifetime of a LiveObject. He can choose
whether he wants the object to be deleted, or to remain persistent. LiveObjects 
should only be deleted by calling its release() method, which may be overloaded 
in any way the programmer would like. The base class implementation provides a 
reference counter. The release() method deletes the object if the reference count 
reaches zero.
<p>
When creating properties on the fly, a LiveObject uses the ID range from kFirstDynamicLiveID
up to kLastDynamicLiveID. This range is pretty unlikely to collide with existing property
IDs, and potential collisions are detected. By default, you can set arbitrary
dynamic properties. If you do not want the object to contain additional
properties, set the "dynamic" argument to the constructor to false. This will
effectively disallow the creation of any property unless hardcoded or added as
a LiveComponent. It will also inhibit messaging. For LiveObjects whose IDs may change,
there is a special kLiveMsgRemapPropertyIDs broadcast message that forces listeners
to forget about any cached property IDs and to remap them by calling getIDForName()
once again.
<p>
Every object may hold an arbitrary data block. This data block must be derived from
Root, because once you set it, it belongs to the LiveObject, and the LiveObject
deletes it after use.
<p>
LiveObjects can be invalidated and revalidated. A typical usage of this feature is 
when the host object that the LiveObject actually wraps goes away, e.g. if the user 
closes a document. Revalidation comes in handy if a LiveObject is part of an Undo 
operation.
<p>
When implementing a LiveObject, make sure that the component does not throw any
exceptions unless you know that your application handles the throwing of exceptions
across component boundaries. The default implementation is exception safe. LiveObjects
also inherit the multithreading methods createLock(), lock() and unlock() from LiveBase.
You should use the LiveObjectLock class to lock a LiveObject,since that class is
exception safe.
<p>
The LiveObject factory is a set of a few static methods that register, unregister and clone
a LiveObject via calling the clone() method of the registered LiveObject. The factory
data is app global and thread safe. During the creation of new LiveObjects, the create()
method locks the factory object.
@see sRuntimeErrors
@see ClassInfo
*/

class SC_API LiveObject : public LiveBase, public Broadcaster, public Listener
{
	/// Private copy ctor, because LiveObject has pointer data members.
								LiveObject (const LiveObject&);
	/// Private assignment operator, because LiveObject has pointer data members.
	LiveObject&					operator= (const LiveObject&);
private:
	friend class				LODataAccsssor;
	LiveObjectData*				mData;
	mutable scuint32_t			mFlags;
protected:
	/// May be used as storage for the class name.
	mutable	String				iClassName;
	/// Never delete a LiveObject directly; calls its release() method instead.
	virtual						~LiveObject();
	/**
	Clone the content of this LiveObject. The argument "cloneAll" controls 
	the copying of dynamic properties created by createIDForName(). If set 
	to false, these properties are not copied. All LiveComponents are copied
	to the new LiveObject, and the class name of the LiveObject is set to
	whatever text is stored in the iClassName member. This is a helper method
	for derived implementations of clone().
	@param	target				which object to fill
	@param	cloneAll			also clone dynamic properties
	@return						an error code (0 for no error)
	@see clone()
	*/
			scerror_t			cloneContent (LiveObject& target, bool cloneAll) const;
	/**
	Search for a LiveComponent that supports the given property ID. The method walks the
	list of LiveComponents to find the component that supports the ID.
	@param	id					the ID to lookup
	@return						the component found or null
	*/
			LiveComponent*		lookupComponent (scint32_t id) const;
	/**
	Find a LiveComponent that supports indexing. The method walks the list of 
	LiveComponents to find a component that supports indexing (i.e. whose 
	isIndexable() method returns true).
	@return						the component found or null
	*/
			LiveComponent*		lookupIndexableComponent() const;
	/**
	Invoke the autostore functionality if an autostore object is registered.
	This method causes a put() to be issued on the given autostore object with
	the registered property ID, using this object as value. Call this method 
	after a put operation if you overload put() or putByIndex() completely.
	The method returns any error code returned be the called put().
	*/
			scerror_t			invokeAutostore (Error* error = NULL);
	/**
	Create a new dynamic property. Overload this method if you want to customize a LiveProperty
	and return that property. This implementation creates and returns a new LiveProperty instance.
	@param	name				the property name
	@param	id					the property ID
	@param	flags				creation flags as supplied to defineIDForName()/createIDForName()
	@return						the new property object
	*/
	virtual	LiveProperty*		createProperty (const String& name, scint32_t id, scuint32_t flags);
public:
	/**
	Properties may be read-only or read-write. Also, they might not want to
	be part of an enumeration, and they might not want to be deleted. Feel free to
	combine these bits during calls to defineIDForName() or createIDForName().
	These bits are also used by the property manager class. Implementation
	note: the bits 0-3 are reserved for type information if used with the property
	manager. They take a TypeInfo constant. For dynamic properties, it is possible
	to use the kMethod type. This causes the LiveObject to invoke the property by
	calling its call() method. This makes only sense if you overload the LiveProperty
	class as well as the LiveObject method createProperty() in order to implement
	the call() method. You can also use the types kReadOnly or kReadWrite (which is
	the default if not being used). The attribute kAttrReadOnly also sets the type
	to kReadOnly. For TypeInfo constants, see ScClassInfo.h.
	@see LivePropertyManager
	*/
	enum {
		/// The property is read only.
		kAttrReadOnly		= 0x0010,
		/// The property is not enumerated.
		kAttrDontEnum		= 0x0020,
		/// The property cannot be deleted.
		kAttrDontDelete		= 0x0040,
		///	Hidden properties don't show up in a normal enumeration (may be deprecated, for instance).
		kAttrHidden			= 0x0080,
		/// Immutable property. JavaScript will not keep it from being gc'ed.
		kAttrImmutable		= 0x0100,
		/// Reserved. Do Not Use.
		kAttrReserved		= 0xF800,
		__Attr_32bit__		= 0x7FFFFFFF
	};
	/**
	The selection bits for the enumerate() method allow you to fine-tune the
	behavior of the enumerate() method in combination with a language identifier.
	The lanaguage ID selects the properties valid for a specific language or environment.
	The mask bits select what you want enumerate() to return. The masks kEnumerateMethods,
	kEnumerateAllProperties and kEnumerateReadWriteProperties determine what you want to 
	have returned in general. The default implementation of enumerate() interprets these 
	mask bits directly. The mask kEnumerateHiddenProperties adds hidden elements to the list, and the
	mask kEnumerateOverrideDontEnum also reports properties marked explicitly as not enumerable.
	@see ScCore::ClassInfo
	*/
	enum {
		kEnumerateMethodsBit				= 31,
		kEnumerateReadOnlyPropertiesBit		= 30,
		kEnumerateReadWritePropertiesBit	= 29,
		kEnumerateHiddenItemsBit			= 28,
		kEnumerateOverrideDontEnumBit		= 27,
		kEnumerateDontWalkProtoChainBit		= 26,
		kEnumerateIgnorePredefinedBit		= 25,
		/// In ClassInfo, this is kEnumerateStaticElementsBit.
		kReservedBit						= 24,
		/// Enumerate only methods.
		kEnumerateMethods					= (1 << kEnumerateMethodsBit),
		/// Enumerate read-only properties.
		kEnumerateReadOnlyProperties		= (1 << kEnumerateReadOnlyPropertiesBit),
		/// Enumerate read-write properties.
		kEnumerateReadWriteProperties		= (1 << kEnumerateReadWritePropertiesBit),
		/// Enumerate also hidden items. Use with one or more of the above masks.
		kEnumerateHiddenItems				= (1 << kEnumerateHiddenItemsBit),
		kEnumerateHiddenProperties			= kEnumerateHiddenItems,
		/// Also force kAttrDontEnum properties to show up.
		kEnumerateOverrideDontEnum			= (1 << kEnumerateOverrideDontEnumBit),
		/// Dont walk the prototype chain for JavaScript objects; include only the immediate prototype.
		kEnumerateDontWalkProtoChain		= (1 << kEnumerateDontWalkProtoChainBit),
		/// Ignore predefined properties and methods.
		kEnumerateIgnorePredefined			= (1 << kEnumerateIgnorePredefinedBit),
		//// Enumerate all non-hidden properties.
		kEnumerateProperties				= (kEnumerateReadOnlyProperties 
											|  kEnumerateReadWriteProperties ),
		//// Enumerate all non-hidden properties and methods.
		kEnumeratePropertiesAndMethods		= (kEnumerateMethods 
											|  kEnumerateProperties ),
		//// Enumerate everything you can find.
		kEnumerateEverything				= (kEnumeratePropertiesAndMethods 
											|  kEnumerateHiddenItems
											|  kEnumerateOverrideDontEnum ),
		__Enum_32bit__						= 0x7FFFFFFF
	};
	/**
	The configuration flags control some general behavior aspects fo the LiveObject.
	All flags can be changed at any time unless noted otherwise.
	*/
	enum {
		/** This LiveObject allows the creation of dynamic properties. The flag is set
		by the "dynamic" argument of the constructor.
		*/
		kAllowDynamicProperties				= 0x00000001,
		/** This LiveObject may alter the IDs and/or types for a given property name. The getIDForName()
		method may return different property IDs for the same string. Id you cache the property ID,
		you must call getType() before using get(), put() or call() to make sure that the meaning of
		the ID has not changed. Predefined ExtendScript IDs cannot be remapped; therefore, ExtendScript
		does not call getType() for these IDs. See ScLiveBase.h for a list of these IDs.
		*/
		kPropertiesMayRemap					= 0x00000002,
		/**
		This LiveObject is actually an enumeration object. The flag controls the way
		the LiveObject's ClassInfo and ServiceInfo spit out their XML. If set, the
		ClassInfo generates an <enumeration> element instead of a <classdef> element,
		and the ServiceInfo toXML() generates <enum> elements instead of <property>
		elements. Note that if this flag is set, the LiveObject should not contain
		any methods, and the properties should all be readonly.
		*/
		kIsEnumeration						= 0x00000004,
		/**
		This LiveObject can act as a constructor. Setting this bit causes ExtendScript
		to call the LiveObject's clone() method when attempting to use the JavaScript
		wrapper object as a constructor. This example is from the Java wrapper:
		<p><pre>
		var jstring = java.lang.String; // returns a LiveObject with kIsConstructor set
		var s = new jstring ('hi');     // calls the LiveObject as a constructor
		<pre>
		*/
		kIsConstructor						= 0x00000008,
		/**
		Mark all native properties of this LiveObject as being DontEnum. This does also
		cover collection elements indexed by number.
		*/
		kPropertiesAreDontEnum				= 0x00000010,
		/**
		This flag affects the display in the ESTK Data Browser. If set, the LiveObject
		and its elements are considered to be part of the built-in JavaScript classes.
		This flag, if used, should also be set for factory objects.
		*/
		kIsCoreJS							= 0x00000020,
		/** The following flags are for internal use only.
		*/
		kInternalFlags						= 0x00FF0000,
		/** The upper 8 flags are user-configurable. The LiveObject ignores these flags.
		*/
		kUserFlags							= 0xFF000000,
		__Flags_32bit__						= 0x7FFFFFFF
	};
	/**@name Factory */
	//@{
	/**
	Register a LiveObject as a factory object. The create() method looks up the
	LiveObject by its class name and calls its clone() method to create a new object.
	The LiveObject's addRef() method will be called. If you set the object ID to any
	four-byte value, the version of create() with that ID can be called as well.
	The method is thread safe; the factory is global to all threads.
	@param	obj					the LiveObject to be used as a factory object
	@param	languages			a combination of language and environment bits that are valid for this class
	@return						an error code (0 for no error)
	*/
	static	scerror_t			addFactory (LiveObject& factory, scint32_t languages);
	/**
	Unregister a factory object. The LiveObject's release() method will be called.
	The method is thread safe; the factory is global to all threads.
	@param	clsName				the class name to unregister
	*/
	static	void				removeFactory (const String& className);
	/**
	Retrieve a factory object by class name.
	@param	clsName				the class name
	@param	language			a language or environment bit
	@return						the factory object or null
	*/
	static	LiveObject*			getFactory (const String& clsName, scint32_t language);
	/**
	Retrieve a factory object by class ID.
	@param	clsID				the class ID
	@param	language			a language or environment bit
	@return						the factory object or null
	*/
	static	LiveObject*			getFactory (scint32_t clsID, scint32_t language);
	/**
	Create a new LiveObject. Look up the factory object by the given class name
	and call its clone() method to create a new object if the given language bits match.
	During the call of the factory object's clone() method, the factory object is locked.
	@param	newObj				takes the new object with the refcount set to 1
	@param	clsName				the class name of the object to create
	@param	language			a language or environment bit
	@return						an error code (0 for no error)
	*/
	static	scerror_t			create (LiveObject** newObj, const String& clsName, scint32_t language);
	/**
	Create a new LiveObject. Look up the factory object by the given object ID
	and call its clone() method to create a new object if the given language bits match.
	During the call of the factory object's clone() method, the factory object is locked.
	@param	newObj				takes the new object with the refcount set to 1
	@param	clsID				the object ID to look up
	@param	language			a language or environment bit
	@return						an error code (0 for no error)
	*/
	static	scerror_t			create (LiveObject** newObj, scint32_t clsID, scint32_t language);
	/**
	Ask this instance to create a new LiveObject of a certain class. Often, you would not
	want child elements to appear in the public factory, since they are usually not creatable
	on their own. The parse() and parseNew() methods call this method to instantiate new
	LiveObjects as they go along. A typical example is a Dialog object containing Button
	objects. You will not want a JavaScript to be able to do a "new Button", so you want
	to keep the Button factory out of the global factory, but rather inside the Dialog
	object. The default implementation actually asks the factory to create the requested 
	object, but this behavior is overloadable. The returned LiveObject should have a refcount
	of 1.
	*/
	virtual	scerror_t			createLocal (LiveObject** newObj, const String& clsName, scint32_t language);
	//@}
	/**@name Construction */
	//@{
	/**
	The class name will be used in Javascript. If you do not supply it initially, 
	overload getClassName() to return a class name. The ref count is 1. If you
	set the dynamic bit to false, the object does not broadcast during a getIDForName()
	call, since the creation of properties at runtime is inhibited.
	@param	clsName				the optional class name; if not supplied, overload getClassName() (ASCII only)
	@param	dynamic				if true, allow dynamic properties to be created
	*/
								LiveObject (const char* clsName = NULL, bool dynamic = true);
	///
	explicit					LiveObject (const String& clsName, bool dynamic = true);
	/**
	Clone this LiveObject by creating an exact copy. The optional argument 
	"deepClone" controls the copying of dynamic properties created by 
	createIDForName(). If set to false, these properties are not copied. 
	All LiveComponents are copied to the new LiveObject, and the class 
	name of the LiveObject is set to whatever text is stored in the iClassName 
	member. The reference count of the returned object is one. If the method 
	succeeds, the new object is stored in the newObj pointer and 0 is returned. 
	On errors, the 	newObj pointer is null and an error code is returned.
	To create a new copy of this object, the methos attempts to call the
	method kLiveIDCreate. If that method is undfined, a new LiveObject
	instance is created. If it is defined, the returned LiveObject should
	have a refcount of one.
	@param	newObj				takes the newly created object
	@param	cloneAll			if false, do not clone dynamic properties
	@return						an error code (0 for no error)
	*/
	virtual	scerror_t			clone (LiveObject** newObj, bool deepClone = true) const;
	/**
	Initialize a LiveObject with an argument list. This method initially returns kErrOK,
	but you may provide an implementation so scripting environments can construct a
	LiveObject using an argument list. ExtendScript calls this method after retrieving
	a new LiveObject from the factory to pass on any constructor call arguments. 
	@param	args				the argument list
	@param	error				an optional Error object
	@return						an error code (0 for no errors)
	*/
	virtual	scerror_t			initialize (const Array& args, Error* error = NULL);
	//@}
	/**@name Class name and casting */
	//@{
	/**
	Retrieve the class name; if not set, set it to "Object". May be overloaded
	to implement delayed lookup of the class name. The member iClassName can be used
	to store the class name.
	*/
	virtual	const String&		getClassName() const;
	/// Get the getAs() interfaceID of this class. Useful for template programming.
	static	scint32_t			getInterfaceID() { return kLiveIDLiveObject; }
	/**
	Retrieve either a downcast version of this instance or a pointer to 
	object-specific data according to the given four-byte ID. This method 
	lets you easily determine which kind of LiveObject this is, or to 
	access data that this instance wraps, like a FileSPec pointer for an object 
	wrapping a file. The default implementation returns null.
	These IDs are called LiveIDs, and they are global to all LiveObjects.
	You should, therefore, be sure that the LiveIDs you create do not collide
	with other existing LiveIDs. The file sLiveIDs.h contains a number of
	predefined LiveIDs for a lot of classes. Also, this file contains a list
	of pre-registered characters for Adobe apps. If you want to publish your
	LiveIDs, please have a character registered for you.
	@param	liveID				the liveID
	@return						the object or null
	*/
	virtual void*				getAs (scint32_t liveID);
	///
	virtual const void*			getAs (scint32_t liveID) const;
	//@}
	/**@name Comparison */
	//@{
	/**
	Compare this instance to another LiveObject. The method does an in-depth compare
	of its contents. All data must match exactly, like the number and sequence of
	components, or the data stored in the LiveObject. ExtendScript calls this method
	in order to check for object identity.
	*/
	virtual	bool				operator== (const LiveObject& r) const;
	//@}
	/**@name User-defined data */
	//@{
	/**
	Set an arbitrary ID for this LiveObject. There is no fixed format for this ID.
	The object ID is used by the MultiScript environemnt.
	*/
			void				setObjectID (scint32_t id);
	/**
	Retrieve the object ID. This value is initially Zero.
	*/
			scint32_t			getObjectID() const;
	/**
	Set a user-defined data block. After setting the data, the data belongs to
	the LiveObject, and it is deleted when the LiveObject is deleted. For this reason,
	the data block must be derived from Root. Calling this method multiple times
	causes any existing data to be deleted.
	<p>
	NOTE: If the data is derived from Refcountable, its refcount is maintained
	instead of the data being deleted.
	*/
	virtual	void				setData (Root* data);
	/**
	Get the user-defined data. This data defaults to null.
	*/
	virtual	Root*				getData () const;	
	/**
	Invalidate a LiveObject. The LiveObject should, if appropriate, take care to 
	invalidate its contents and/or to ignore any access to it. The LiveObject 
	is guaranteed not to call its enclosed LiveComponents if it is invalidated.
	A LiveMessage kLiveMsgInvalidated is broadcast to inform listeners.
	*/
	virtual	void				invalidate();
	/**
	Revalidate a LiveObject. The LiveObject should, if appropriate, take care to 
	revalidate its contents. A LiveMessage kLiveMsgRevalidated is broadcast to 
	inform listeners.
	*/
	virtual	void				revalidate();
	/**
	Retrieve the configuration flags for this LiveObject.
	*/
	inline	scuint32_t			getFlags() const { return mFlags; }
	/**
	Set the configuration flags for this LiveObject. Some flags may not be settable,
	and this method ignores these flags. Note that flags also can be set on a const object.
	*/
	inline	void				setFlags (scuint32_t flags) const { mFlags = flags; }
	//@}
	/**@name Component maintenance */
	//@{
	/**
	Add a new LiveComponent. This object is appended to the list of already 
	existing LiveComponents. When a property ID is searched for, this list
	is scanned in reverse order so newly added LiveComponents override the
	implementation of any older property ID. The LiveComponent belongs to
	the LiveObject.  When adding, its addRef() method is called. Its 
	release() method will be called when the parent LiveObject is deleted.
	@param	component			the component to add
	@return						an error code (0 for no error)
	*/
	virtual	scerror_t			addComponent (LiveComponent& component);
	/**
	Remove a component. If present, the component is removed, and its reference
	count is decreased.
	@param	component			the component to remove
	@return						0 for no error
	*/
	virtual	scerror_t			removeComponent (LiveComponent& component);
	/**
	Search for a LiveComponent object. This method is intended to be used by
	LiveComponent objects, so they can implement subclassing of methods and
	properties. The method takes an optional LiveComponent pointer that can
	be excluded from the search so the calling LiveComponent object can make
	sure that the method does not find itself. The method can, of course, not
	find a LiveComponent object for methods and properties that the LiveObject
	implements itself. The refcount of the returned object is not changed.
	@param	id					the ID to lookup
	@param	exclude				the LiveComponent to be excluded from the search (optional)
	@return						the found LiveComponent object or null
	*/
			LiveComponent*		getComponent (scint32_t id, const LiveComponent* exclude = NULL) const;
	//@}
	/**@name Name lookup */
	//@{
	/**
	Retrieve the ID of a property. If the property exists, fill in its ID 
	and type data and return 0 for no error. Return kErrUndefined if the
	property does not exist. Use the returned ID for subsequent property 
	accesses. The type field is set to one of the enum values kReadOnly, 
	kReadWrite or kMethod. For unrecognized names, set the ID to zero
	and the type to kInvalid and return kErrUndefined. 
	Must be implemented for scripting languages. The default implementation 
	first attempts to find a matching dynamic property. If this fails, it
	searches the LiveComponents for a matching name by calling their getIDForName() 
	method. The search is backwards so newly added property objects override 
	previous property objects. If this fails, a LiveMessage kLiveMsgGetID is broadcast
	to obtain information about the name. If this succeeds, an internal dynamic
	property entry is created that marshals any accesses to listeners by broadcasting
	LiveMessages (see the other LiveObject methods below for message IDs). If the 
	dynamic flag was set to false during construction, the object does not broadcast 
	a message for a name/ID mapping.
	<p>
	Besides of the language bits, the lang argument may contain additional lookup flags.
	The kAttrLookupLHS flag is set during a put operation. Some objects may choose to
	pre-create a property during lookup. The XML object, for example, supports deep put,
	meaning that if e.g. xml.a.b.c = 5 is executed, and b does not exist, b and c are
	created during lookup if that flag is set. The kAttrLookupMethod flag is set if
	ExtendScript wants to specifically lookup a method.
	@see	LiveMessage
	@param	name				the property name
	@param	id					takes the ID
	@param	type				takes the type of the property
	@param	lang				the language ID plus lookup flags
	@return						an error code (0 for no error)
	*/
	virtual	scerror_t			getIDForName (const String& name, scint32_t& id, InfoType& type,
											  scint32_t lang = kJavaScriptLanguage) const;
	/**
	Create a new property. This method does not check for the existence of
	the given name! If you are not sure whether the property already exists,
	call getIDForName() first to check. The method creates a new property and 
	assigns the next available ID from the ID pool. If all possible dynamic IDs have
	been used (which is almost impossible), the method fails. When creating a
	read-only property, the property will accept exactly one put operation, so
	you should store its value immediately after creating the property. The initial
	value of a new property is Undefined. The type is the four lowest bits of the flags.
	@param	name				the property name
	@param	id					takes the ID
	@param	flags				a combination of type, language, kAttrReadOnly, kAttrDontEnum and kAttrDontDelete
	@return						an error code (0 for no error)
	*/
	virtual	scerror_t			createIDForName (const String& name, scint32_t& id, scint32_t flags=kReadWrite);
	/**
	Create a new property. This method does not check for the existence of
	the given name or ID! If you are not sure whether the property already exists,
	call getIDForName() first to check. The method creates a new property and 
	assigns the given ID to it. When creating a read-only property, the property 
	will accept exactly one put operation, so you should store its value 
	immediately after creating the property. The initial value of a new property 
	is Undefined. If the flag does not contain any type, the type is set to
	kReadWrite. The type is the four lowest bits of the flags.
	@param	name				the property name
	@param	id					the ID
	@param	flags				a combination of type, language, kAttrReadOnly, kAttrDontEnum and kAttrDontDelete
	@return						an error code (0 for no error)
	*/
	virtual	scerror_t			defineIDForName (const String& name, scint32_t id, scint32_t flags=kReadWrite);
	/**
	Lookup a dynamic property. This method does not work for non-dynamic properties.
	It may also return properties that were generated internally because of the result
	of a broadcast operation. Use this method to access your own, overloaded properties.
	*/
			LiveProperty*		findProperty (scint32_t id) const;
	/**
	Retrieve the name of a property. Use this call to convert an ID to
	a name. The method should be implemented if the LiveObject supports
	object browsers. If the ID cannot be matched to a name, clear the
	string and return false. The default implementation searches first
	the dynamic properties, then it searches the property objects for a 
	matching name by calling their getNameForID() method. The search is 
	backwards so newly added property objects override previous property 
	objects.
	@param	id					the property ID to match
	@param	name				takes the name
	@param	lang				the language ID
	@return						an error code (0 for no error)
	*/
	virtual	scerror_t			getNameForID (scint32_t id, String& name, scint32_t lang = kJavaScriptLanguage) const;
	/**
	Delete all dynamic properties. If forceAll is false, delete only properties not marked with kAttrDontDelete.
	*/
	virtual	void				deleteDynamicProperties (bool forceAll);
	/**
	Check whether this instance has operator overloads defined. These overloads have special IDs
	that range between LiveBase::kJSFirstOverload and LiveBase::kJSLastOverload. The default
	implementation checks the dynamic properties as well as the components and the property
	manager if present.
	*/
	virtual	bool				hasOperators() const;
	//@}
	/**@name Deletion */
	//@{
	/**
	Delete a property. This method deletes only properties created by the
	createIDForName() method. Returns false if the property was not deleted.
	@return						0 if deleted, kErrUndefined if not found
	*/
	virtual	scerror_t			deleteID (scint32_t id);
	/**
	Clear all dynamic data and reset the object to its initial state. This call
	calls the reset() method of all LiveComponents and deletes all dynamic data.
	*/
	virtual	void				reset();
	//@}
	/**@name Enumeration */
	//@{
	/**
	Fill the supplied array with the IDs of all properties that this object 
	supports. Use the given language specifier to determine which properties
	need to be returned. You can supply additional browse bits to override the
	default behavior for a scripting language. The default implementation
	enumerates all elements, but ignores the kEnumerateIgnorePredefined bit
	- after all, people would want to see what is inside a LiveObject. That
	bit should be used only for objects that are part of the scripting language 
	standard.
	@param	propertyIDs			the array to fill
	@param	lang				the language ID plus any browse bits
	@return						an error code (0 for no error)
	@see	LiveBase
	*/
	virtual	scerror_t			enumerate (SimpleArray& propertyIDs, scint32_t langAndBits = kJavaScriptLanguage) const;
	//@}
	/**@name Inspection */
	//@{
	/**
	Retrieve the type of a property. This call returns one of the enum
	values kReadOnly, kReadWrite or kMethod. For unrecognized
	IDs, return kInvalid.
	@param	id					the property ID
	@return						the type of the property
	*/
	virtual	InfoType			getType (scint32_t id) const;
	/**
	Get information about the object. The default implementation fills in
	the class name of this instance. A LiveMessage kLiveMsgGetInfo
	with an ID of zero is broadcast to obtain the information.
	@param	info				a pointer to the info block to be filled
	@param	lang				the langugage to look for (optional)
	@return						an error code; return kErrUndefined if there is no info
	*/
	virtual	const ClassInfo*	getClassInfo (scint32_t lang = kJavaScriptLanguage) const;
	//@}
	/**@name Property interface */
	//@{
	/**
	Read the value of a property. Return 0 if the property could be read, 
	an error code otherwise. This implementation first attemptz to read
	a dynamic property; if this fails, it forwards the call to one 
	of the LiveComponents stored inside this object by walking backwards 
	the components array. Finally, a LiveMessage kLiveMsgGet is broadcast to retrieve 
	the value.
	@param	id					the property ID
	@param	value				the location to take the value
	@param	errors				an optional pointer to extended error information
	@return						an error code (kErrUndefined if no match, 0 for no error)
	*/
	virtual	scerror_t			get (scint32_t id, Variant& value, Error* errors = NULL) const;
	/**
	Store a value into a property. Return kErrUndefined if the property does not exist,
	kErrReadOnly if it is read-only, or 0 if everything is fine. This implementation 
	first attempts to store the value into an existing dynamic property. If this fails,
	it forwards the call to one of the LiveComponents stored inside this object by walking 
	backwards the components array. Finally, a LiveMessage kLiveMsgPut is broadcast to retrieve 
	the value.
	@param	id					the property ID
	@param	value				the value to store
	@param	errors				an optional pointer to extended error information
	@return						an error code (kErrUndefined if no match, 0 for no error)
	*/
	virtual	scerror_t			put (scint32_t id, const Variant& value, Error* errors = NULL);
	/**
	Call a method. This implementation forwards the call to one 
	of the LiveComponents stored inside this object by walking backwards the 
	components array. If no component accepted the ID, a LiveMessage kLiveMsgCall
	is broadcast to retrieve the value. If the called method is JavaScript, no runtime 
	error will be generated; instead, the error code is returned to the caller.

	@param	id					the property ID
	@param	args				the method arguments
	@param	result				the return value.
	@param	errors				an optional pointer to extended error information
	@return						an error code (kErrNoFunction if no match, 0 for no error)
	*/
	virtual	scerror_t			call (scint32_t id, const Array& args, Variant& result, Error* errors = NULL);
	///
			scerror_t			call (scint32_t id, const Array& args);
	///
			scerror_t			call (scint32_t id, Variant& result);
	///
			scerror_t			call (scint32_t id);
	/**
	Apply a function call to a given LiveObject. This method is a more generic implementation
	of call(). Instead uf calling the requested method with this instance as the "this" object,
	the "this" object is supplied. This implementation forwards the call to one of the 
	LiveComponents stored inside this object by walking backwards the components array. If 
	no component accepted the ID, a LiveMessage kLiveMsgCall is broadcast to execute the call. 
	<p>
	If the class nameof the supplied "this" object is different from the class name of this instance,
	the call usually fails with the error kErrWrongClass. All LiveObject methods are by default 
	defined to be strict, which means that they can only work on instances of the same class as the 
	class which they are defined in. You can override this behavior for selected methods. These methods
	are either "static" in the C++ way because they ignore the "this" object, or they can deal with
	every basic LiveObject. To declare such a function, implement LiveComponent::isGeneric() to
	return true fo the respective property ID. This implicates that only methods defined in a 
	LiveComponent can be declared as generic - sorry.
	<p>
	If the called method is JavaScript, no runtime error will be generated; instead, the error code
	is returned to the caller.
	@param	thisObj				the object to use as "this"
	@param	id					the property ID
	@param	args				the method arguments
	@param	result				takes the method result
	@param	errors				an optional pointer to extended error information
	@return						an error code (kErrNoFunction if no match, 0 for no error)
	*/
	virtual	scerror_t			apply (LiveObject& thisObj, scint32_t id, const Array& args, Variant& result, Error* errors = NULL) const;
	//@}
	/**@name Parsing */
	//@{
	/**
	Parse the contents of the string and set the properties accordingly.
	Use this call to setup the object with multiple properties at once.
	The string contains name:value pairs. The value may be a number,
	a quoted string following JavaScript guidelines (including backslash
	escape sequences), "true", "false" or "NULL". The name may be quoted 
	to define properties that do not follow JavaScript syntax. The value may 
	also be a nested object. Use curly brackets to declare a nested object and to
	group its properties. If the string itself is surrounded by curly
	brackets, these brackets are ignored. Setting new properties only
	works if the object is dynamic (which is the default). Inline array
	declarations are supported (use square brackets and separate the values
	by commas). 
	<p>
	To create an object of a certain class, use the syntax "propertyName : 
	className { prop1:value1, ... }". The LiveObject uses the createLocal() method
	(which defaults to the factory create() method) to create objects of a certain 
	class; if this creation fails, it returns a runtime error kErrNoCtor, except 
	for objects without a class name or the class name "Object"; in this case, 
	it will just create standard LiveObject objects of the class "Object". 
	The factory will be called with the supplied language. This syntax is not
	supported on the top level. Use parseNew() for this purpose.
	specifier.
	@param	text				the text to parse
	@param	lang				the language ID to use for the create() method
	@return						an error code (0 for no error)
	*/
			scerror_t			parse (const String& text, scint32_t lang = kJavaScriptLanguage);
	/// The same call with a more detailed error reporting.
			scerror_t			parse (const String& text, Error& error, scint32_t lang = kJavaScriptLanguage);
	/**
	Parse a new LiveObject. This static method allows for the creation of a new
	LiveObject entirely out of the supplied string. The string starts with an 
	optional class name followed by curly braces, like "classname { prop1: xx...}". 
	If there is no class name or no outer curly braces, the method uses "Object"
	as the class name. The LiveObject uses the createLocal() method (which defaults 
	to the factory create() method) to create objects of a certain class; if this 
	creation fails, it returns a runtime error kErrNoCtor, except for objects 
	without a class name or the class name "Object"; in this case, it will just 
	create standard LiveObject objects of the class "Object". 
	The factory will be called with the supplied language.
	@param	text				the text to parse
	@param	obj					takes the newly created object
	@param	lang				the language ID to use for the create() method
	@return						an error code (0 for no error)
	*/
	static	scerror_t			parseNew (const String& text, LiveObject*& obj, scint32_t lang = kJavaScriptLanguage);
	//@}
	/**@name Autostore */
	//@{
	/**
	Set an autostore ID. Setting a LiveObject and an ID causes the LiveObject's
	put() method to be called with the given ID and this object as data to signal
	that object that the content of this object has been changed. ExtendScript
	clears the autostore target if a LiveObject is assigned to a variable. This lets
	the autostore feature work fine for items like "color.rgb.red = 100", but not
	for "c = color.rgb; c.red = 100". The ref count of the target is not increased
	so the target object may be deleted. Instead, this LiveObjects listens to the
	deletion broadcast of the target and clears the pointer if the target gets deleted.
	@param	target				the autostore target
	@param	id					the ID to be used on that target
	*/
			void				setAutostore (LiveObject* target, scint32_t id) const;
	//@}
	/**@name Other */
	//@{
	/**
	Convert this object into a scalar value. The base implementation attempts
	to call the "valueOf" method. If the method does not exist or the call fails,
	the value is set to the value stored inside the LiveObject, which initially
	is Undefined.
	*/
	virtual	void				valueOf (Variant& result) const;
	/**
	Convert this object into a string. The base implementation attempts to call
	the method "toSTring". If the method does not exist or the call fails, it
	sets the return value to "[object className]", where className is the actual 
	class name.
	*/
	virtual	void				toString (String& result) const;
	/**
	Listen to whatever message this LiveObject has registered for. By default, 
	a LiveObject listens to an Autostore object and clears the pointer if 
	the Autostore object gets deleted.
	@return						the result of the processing of the message
	*/
	virtual	bool				notify (Message& msg);
	/**
	Short version of get() that takes the property name. The language constant used
	kJavaScriptLanguage.
	@param	name				the property name
	@param	result				the result
	@param	error				an optional Error instance that takes any errors
	@return						an error code
	*/
			scerror_t			get (const String& name, Variant& result, Error* err = NULL);
	/**
	Short version of put() that takes the property name. The language constant used
	kJavaScriptLanguage.
	@param	name				the property name
	@param	value				the value
	@param	flags				creation flags if not present (kAttrReadOnly etc as above, -1 = don't create)
	@param	error				an optional Error instance that takes any errors
	@return						an error code
	*/
			scerror_t			put (const String& name, const Variant& value, scint32_t flags = -1, Error* err = NULL);
	/**
	Short version of call() that takes the property name. The language constant used
	kJavaScriptLanguage.
	@param	name				the property name
	@param	args				the arguments
	@param	result				the result
	@param	error				an optional Error instance that takes any errors
	@return						an error code
	*/
			scerror_t			call (const String& name, const Array& args, Variant& result, Error* err = NULL);
	///
			scerror_t			call (const String& name, const Array& args, Error* err = NULL);
	///
			scerror_t			call (const String& name, Variant& result, Error* err = NULL);
	///
			scerror_t			call (const String& name, Error* err = NULL);
	/**
	Check whether there is a method to invoke on whatever object listens to this 
	LiveObject. This method creates a message kLiveMsgCanInvoke, stores the supplied 
	method name in the data part of the message and broadcasts the message in order 
	to find out about the method. 
	@param	name				the name of the method to invoke
	@return						true if there is an invokable method
	*/
			bool				canInvoke (const String& name) const;
	/**
	Invoke a method on whatever object listens to this LiveObject. This method 
	creates a message kLiveMsgInvoke, stores the supplied method name in the data
	part of the message and broadcasts the message in order to invoke this method. 
	How the method is invoked depends solely on the listener object. This method 
	is useful for invoking methods on objects that are not LiveObjects. If the called
	method is JavaScript, no runtime error will be generated; instead, the error code
	is returned to the caller.
	@param	name				the name of the method to invoke
	@param	args				the arguments
	@param	result				the result
	@param	errors				an optional pointer to extended error information
	@return						an error code (kErrNoFunction if no match, 0 for no error)

	@return						an error code; kErrNoFunction if noone responded
	*/
			scerror_t			invoke (const String& name, const Array& args, Variant& result, Error* errors = NULL) const;
	///
			scerror_t			invoke (const String& name, Variant& result, Error* errors = NULL) const;
	///
			scerror_t			invoke (const String& name, const Array& args, Error* errors = NULL) const;
	///
			scerror_t			invoke (const String& name, Error* errors = NULL) const;
	/**
	Issue a broadcast of the kLiveMsgRemapPropertyIDs message to all listeners so they
	are informed that the property IDs and/or the property types of this LiveObject
	have changed. This gives listeners the chance to forget about any cached property 
	ID values and types.
	*/
			void				propertyDefsChanged() const;
};

/**
The LiveObjectLock class locks the LiveObject against deletion by releasing
as well as against multhreaded access. This class is important to use in
a multithreaded environment, because the thread lock needs to be removed
before the object is released (which may cause it to go away).
*/

class LiveObjectLock : public Heap
{
public:
	inline			LiveObjectLock (LiveObject& r) : mLiveObj (r)
					{
						r.addRef();
						r.lock();
					}
	inline			~LiveObjectLock()
					{
						mLiveObj.unlock();
						mLiveObj.release();
					}
private:
	LiveObject&		mLiveObj;
};

}	// end Namespace


#include "ScPostConfig.h"
#endif

