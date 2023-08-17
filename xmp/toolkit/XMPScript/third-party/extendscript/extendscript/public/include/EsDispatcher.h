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
#ifndef _EsDispatcher_h
#define _EsDispatcher_h

#include "ScCore.h"
#include "EsDataPool.h"
#include "ScPreConfig.h"

namespace ScScript
{

class Dispatcher;
class Engine;
class TableEntry;
class DispatcherData;
class PropInfo;

/// IDs for standard objects.

enum StdIDs
{
	///	The ID for objects that never need an ID from your side. These objects cannot be searched for.
	kDontCare		= 0,
	/// The global object. This is predefined inside the engine, but has external properties and methods.
	kGlobal			= -1,
	/// The ID for any prototype object.
	kPrototype 		= -2,
	/// The last reserved ID for the standard or internal IDs.
	kLastReserved	= -10,
	__StdIDs__32bit__ = 0x7FFFFFFF
};

/**
Pseudo property IDs.
The pseudo property IDs are set by the engine when it calls a DispatcherMethod.
*/
enum PseudoIDs
{
	/// Property ID when checking for the existence of a property after using .name
	kTest		= 0x74657374, // 'test'
	/// Property ID when calling into a method to get a property value
	kGet		= 0x67657420, // 'get '
	/// Property ID when calling into a method to put a property value
	kPut		= 0x70757420, // 'put '
	/// Property ID when calling into a method to call a method
	kCall		= 0x63616C6C, // 'call'
	/// Property ID when calling into a method to call a constructor
	kConstruct	= 0x63746F72,  // 'ctor'
	__PseudoIDs__32bit__ = 0x7FFFFFFF
};

/**
Common function call and property paratemer structure.
All methods and properties receive their parameters in a structure. This structure
contains info about the engine and dispatcher, the "this" object, the parameters
and a location for a return value. For properties, its ID is supplied. For properties,
the first argument is always the property name, which may be a number for numeric 
properties. On a property get, the value is returned in iValue as usual; for a property 
put, the iValue member contains the data to put.
<p>
A scripting method must be defined as a void function. All arguments and auxiliary values
are stored into a Arguments structure:
<p><tt>void myFunction(Arguments& data);</tt>
<p>
This struct is intended to be used as-is. Do not derive from this struct.
*/

struct Arguments
{
private:
	Arguments (const Arguments&);
	Arguments& operator= (const Arguments&);
public:
	/// The dispatcher that actually defined the function/property.
	Dispatcher&					iDispatcher;
	/// The engine that called the function.
	Engine&						iEngine;
	/// The "this" object as an object reference (ID and class name)
	ScCore::Variant&			iThis;
	/// The name of the property or method called.
	const ScCore::Variant&		iName;
	/// The property ID. For methods calls, this is one of the PseudoID constants.
	scint32_t					iPropID;
	/// The argument list.
	const ScCore::BasicArray& 	iArgs;
	/**
	The location where a value may be stored. Preset to undefined or the value to put().
	@see Dispatcher::put()
	*/
	ScCore::Variant&			iValue;
};

/**
Dispatcher callback method pointer.
A scripting method must be defined as a void function. All arguments and auxiliary values
are stored into a Arguments structure.
@see Arguments
*/

typedef void (*DispatcherMethod) (Arguments& data);

/**
Property entry definition structure.
Set up a table of these entries to easily define own properties.
This struct is intended to be used as-is. Do not derive from this struct.
*/

struct PropEntry
{
	/// Property flags

	enum PropertyFlags
	{
		/// Property is read only
		kReadOnly			= 0x0001,
		/// Property is not usable in for...in
		kDontEnum			= 0x0002,
		/// Property cannot be deleted
		kDontDelete			= 0x0004,
		/// get() and put() called on every access
		kMutable			= 0x0008,
		/// Property will be defined on class constructors only (like Number.NaN)
		kCtorProperty		= 0x0010,
		/// A method (implies kReadOnly | kDontEnum | kDontDelete)
		kMethod				= 0x0020,
		/// A method wants no object references, so valueOf() is called for every object parameter
		kDeRefToValue		=(0x0040 | kMethod),
		/// A method wants no object references, so toString() is called for every object parameter
		kDeRefToString		=(0x0080 | kMethod),
		/// A method wants "this" dereferenced, so valueOf() is called for "this"
		kDeRefThisToValue	= 0x0100,
		/// A method wants "this" dereferenced, so toString() is called for "this"
		kDeRefThisToString	= 0x0200,
		/// The method can only work on objects of its own class
		kStrictMethod		=(0x0400 | kMethod),
		/// The entry removes the definition inside a subclassed dispatcher. It forces the return of NULL.
		kRemove				= 0x0800,
		/// Hide this entry. Used for property names that are inaccessible by Javascripts.
		kHidden				= 0x1000,
		/// The property value remains despite of the kMutable flag (not cleared automatically).
		kPersistent			= (0x2000 | kMutable),
		/// Activate the autostore feature. Objects created here autostore themselves back on changes.
		kAutoStore			= (0x4000 | kDontDelete | kMutable),
		__PropertyFlags__32bit__ = 0x7FFFFFFF
	};

	/// The name of the property. Not Unicode to save space.
	const char*				iName;
	/**
	The property ID. For methods, the field contains the default number of arguments, a value that
	Javascripts can access through the methodName.length property. If both the property ID and the
	method entry are null, the entry is ignored (earlier code used this way to define global objects,
	but the engine now always creates global objects, so this usage of a property entry has been
	deprecated. These entries are now ignored).
	*/
	scint32_t				iID;
	/**
	Property flags. The upper 8 bits may contain the data type of a property or the return
	data type of a method if the flag kIsTyped is set. The data type corresponds to the 
	VariantType values defined in ScVariant.h.
	*/
	scint32_t				iFlags;
	///	Function address for functions, optional for properties. Required for generic entries.
	DispatcherMethod		iMethod;
	/**
	The data type of the property, or return type and parameter info for methods. The data type
	should be one of "boolean","number","string" or "object" for generic objects. The data type
	can also be the class name of an object, like "Document" or "Array" (note the initial upper 
	case letter). If the data can be any type, use the word "any".
	If a method does not return anything, the return value should be "undefined" or the empty string.
	A parameter list is a list of comma separated entries of the form name:type, like "index:number".
	The name part is optional. The parameter list is separated from the return value with a comma 
	as well. An example: the data information for String.indexOf() would be "number,what:string",
	or shorter "number,string". This entry is optional.
	If not present, the data type for properties is assumed as "unknwon", the return value for
	methods is "unknown", and for parameters, the name is "argX" with X being a number and the type
	being "any".
	*/
	const char*				iTypeInfo;
};

/**
Some commonly used abbreviations for above property flags.
*/

enum StdPropertyFlags
{
	/// A read/write property, not deletable, not enumerable.
	kPropRW				= (PropEntry::kMutable | PropEntry::kDontDelete | PropEntry::kDontEnum),
	/// A readonly property, not deletable, not enumerable.
	kPropRO				= (kPropRW | PropEntry::kReadOnly),
	/// An read once read/write property, not deletable, not enumerable.
	kPropRWOnce			= (PropEntry::kDontDelete | PropEntry::kDontEnum),
	/// An read once readonly property, not deletable, not enumerable.
	kPropROOnce			= (kPropRWOnce | PropEntry::kReadOnly),
	/// A generic method that can work with any class.
	kPropGenericMethod	= (PropEntry::kMethod),
	/// A strict method that only works with this class.
	kPropMethod			= (PropEntry::kStrictMethod),
	/// A constructor function.
	kPropConstructor	= (kPropGenericMethod | PropEntry::kHidden),
	__StdPropertyFlags__32bit__	= 0x7FFFFFFF
};

/**
The Dispatcher classs is used by the engine to communicate with objects inside the
application. The application programmer creates one or more instances of a Dispatcher
class and registers them within the engine. The engine uses this list of dispatchers
to find properties and methods for outside objects. The dispatchers remain in the
ownership of the application and must be deleted there. Predefined Javascript dispatchers
are engine specific. Each engine has its own dispatcher.
<p>
A dispatcher stands always for a collection of objects of one or more Javascript classes. It 
contains one or more static tables of property entries which defines the properties and methods 
for that class. These properties are enumerable and may be queried. The properties of a class are
read and written through overloadable methods. The methods of a class are treated differently.
Since methods may be attached to any kind of a class, they are grouped into special method
libraries. These libraries consist of a collection of static functions which are called
with a description of the "this" object and the appropriate parameters. A dispatcher only
contains the function names, so the engine can call them if needed. You may define more than 
one class name, so the dispatcher is able to handle multiple classes if desired. Also, it is
possible to add classes to an dispatcher during runtime. The global dispatcher should only have a single
class name. If not, the Engine uses the first registered class name of the global dispatcher
to name the class of the global object.
<p>
Usually, the value of a property is evaluated only once. Later fetches of the property fetch
the value stored inside the engine. Often, it is possible for a script to set a property value
without affectiong the outside world. If this behavior is unwanted, the flag kMutable may
be set which causes the engine to ask the dispatcher for the value any time the property is accessed.
Another flag kPersistent controls the way how objects are stored. If you return an object reference,
this reference will usually NOT be stored within the prototype object if you declare the entry
as being mutable, since the engine thinks that each read access to the property returns a new
object reference. Therefore, the object may be garbage collected. This may cause unwanted side
effects. Consider e.g. a "document" property that is declared as mutable. If some user attaches
data to it, the data will get lost once the object is garbage collected. If you set the kPersistent
flag, the engine will keep the object in the "document" property even if it is mutable. This causes
the object not to be garbage collected unless the "document" property returns a different object.
<p>
The JavaScript engine creates a prototype object for each class. The prototype object will initially 
contain all property entries. New objects do not contain any properties. Instead, they fetch everything
from the prototype object. Properties are appended to the object when fetched, while functions remain
with the prototype object when called.
<p>
A dispatcher may be responsible for more than one class. The method addClass() adds a class name
to its list of supported classes.
<p>
Dispatchers may be subclassed. When creating a subclassing dispatcher, the dispatcher should
support exactly one class (the class to be subclassed). If the dispatcher supports more than one 
class, only the first registered class will be subclassed. Then, call subClass() with the dispatcher 
to be subclassed as argument. Now, the subclasssed dispatcher is called when a local search does
not succeed. In order to support the removal of entries, set the flag kRemove. This causes 
the dispatcher to return null for this entry without querying the subclassed dispatcher. When calling
a function or property that actually belongs to a subclassed dispatcher, the subclassing dispatcher
will be called with its get(), put() or call() methods, but the Arguments structure contains the
original, subclassed dispatcher. This allows subclassed dispatchers to easily forward any call to
its parent dispatcher when it cannot handle the call correctly.
<p>
The constructor function is a function with the same name as the dispatcher's class
name. It is either called as a constructor (as new MyObject()) or as a global function (MyObject()).
When implementing this function, you need to check whether it is called as function or as constructor.
When called as constructor, the iThis member contains a fresh object which you can populate. You can
also return a new object reference if you want to create your own object, but be aware! The engine
will call your constructor function again to initialize this new object, so you need to add checking
code to avoid a stack overflow! 
<p>
When the engine sees a property name which is a positive integer, it looks for a table entry ".index" 
and uses the flags and calling convention of that entry while calling the associated routine and 
creating the property. For other numeric properties, the engine looks for the property ".number". 
For non-numeric properties, the engine looks for a ".name" property for a generic name lookup. 
In all cases, the iName member of the Arguments structure contains the property name. When 
the engine finds a generic property, the engine makes a first call to the property handler 
(which must be a function pointer) with the iPropID member set to kTest. If the property is 
present, the result of this call should be anything else than undefined. Take care when implementing 
the ".name" property so it does not include other predefined properties like toString().
<p>
When a call returns an object reference, this reference may contain a user data pointer. The
engine ALWAYS stores this data pointer with the given object. If the object already contains
an user data object, this data object is deleted before the new data object is stored. The
data object belongs to the object; it will be deleted when the object is deleted.
Optionally, a user data object may be stored with the dispatcher. This object can be retrieved at any time.
It will be deleted by the destructor if the autodelete flag is set to true.
<p>
The new APIs use sIDs instead of strings. These sIDs are unique 32-bit identifiers that match a string.
sIDs are globally unique across the application, since there is only one instance of the DataPool class
that maintains these sIDs.
<p>
Dispatchers have a built-in lock. Engines that deal with dispatchers lock and unlock the dispatchers
before accessing them. You should use the DispatcherLock class to lock a dispatcher, since that class
is exception safe.
@see DataPool
*/

class ES_API Dispatcher : public ScCore::Root, public ScCore::Lockable
{
	friend class Object;
	friend class Function;
	friend class Property;
	friend class RealEngine;
	friend class DispatcherDictionary;

	/// Private copy ctor, because this class has pointer data members.
						Dispatcher (const Dispatcher&);
	/// Private assignment operator, because this class has pointer data members.
	Dispatcher&			operator= (const Dispatcher&);
	
	DispatcherData*		mData;
	/// Get a class ID
	scint32_t			getClassID (int index) const;
	/// Get the table for a class name.
	const TableEntry*	getTable (sID clsName) const;
	/// Does the dispatcher define operator overloads?
	bool				hasOperators (sID className) const;
	
	typedef	ScCore::Lockable	Inherited;
	
protected:
	/// User data.
	ScCore::Root*		mUserData;
	/// Whether to delete the user data or not when the dispatcher is deleted.
	bool				mUserDel;
public:
	/**
	Create a dispatcher. If the dispatcher should appear in a certain dictionary
	group, pass in a group name. If the group name is NULL, the dispatcher is not
	visible in the object dictionary. Built-in dispatchers for standard JavaScript
	objects appear in the group "JavaScript".
	*/
						Dispatcher (const char* dictGroupName = "All");
	/**
	The dtor is virtual so it can be overloaded.
	*/
	virtual				~Dispatcher();
	/**
	Return the parent dispatcher if this dispatcher subclasses another dispatcher.
	*/
			Dispatcher* getParent() const;
	/**
	Check if a given class is supported by this dispatcher.
	@param	clsName		the class name
	@return				true if so
	*/
			bool		isClass (sID clsName) const;
			bool		isClass (const char* clsName) const;
			bool		isClass (const ScCore::String& clsName) const;
	/**
	Add a class name to the array of classes. Do this before registering the dispatcher;
	classes added after registering the dispatcher are not recognized!
	@param	clsName		the class name to add
	*/
			void		addClass (const char* clsName);
			void		addClass (const ScCore::String& clsName);
	/// Retrieve the number of classes that this dispatcher supports
			scint32_t	getClassCount() const;
	/// Retrieve a class name for a given index
			const ScCore::String& getClass (int index) const;
	/// Check this dispatcher for containing .name, .number or .index properties for a class.
			bool		hasDotProperties (sID className) const;
	/**
	Subclass en existing dispatcher. A dispatcher must support exactly one class, which is supported
	by the dispatcher supplied. The dispatcher expands or limits toe functionality of the subclassed
	dispatcher. After the local entries have been searched, the entries of the subclassed dispatchers
	are searched unless the local dispatcher has an entry with the flag kRemove set. This entry 
	explicitly declares the name as invalid so the dispatcher can "erase" the entry of a subclassed
	dispatcher.
	@param	disp		the dispatcher to subclass
	*/
			void		subClass (Dispatcher& parent);
	/**
	Add a property table to this dispatcher. Use this method to extend or alter the functionality of
	a dispatcher. If the given class name does not exist, it is registered. Since some dispatchers 
	(line the global and Object dispatchers) preload their properties into the prototype objects, 
	you should call Engine::reset() after altering a dispatcher.
	@param	ignored		ignored
	@param	tbl			the table to define
	@param	className	the class name to register the table for. If there is none
						supplied, use the first registered class name (this is the default for
						most dispatchers)
	*/
	inline	void		addProperties (scint32_t, const PropEntry* table, const char* clsName = NULL)
										{ registerProperties (0, table, clsName); }
	/**
	Find the property entry for a given name. Entries with the flag kCtorProperty or
	kRemove set are not returned. If the dispatcher subclasses an existing dispatcher, 
	that dispatcher is called when the lookup fails. If this method is overloaded, the return
	value is not cached anywhere, so it may as well be temporary.
	@param	clsName		the class name
	@param	name		the property name
	@param	objData		optional object data of the object that started the lookup
	@param	special		leave false
	@param	ctor		set to true to find a ctor property
	@return				the property entry or NULL
	*/
	virtual const PropInfo* findProperty 
						(sID clsName, sID name, ScCore::Root* objData = NULL, bool special=false, bool ctor=false) const;
	/**
	Find the property entry for a given name. Entries with the flag kCtorProperty or
	kRemove set are not returned. The returned property entry is used temporarily and 
	thus may be static.	The supplied name may be used to return a static entry. If the dispatcher 
	subclasses an existing dispatcher, that dispatcher is called when the lookup fails.
	<p>
	THIS METHOD IS DEPRECATED. USE findProperty() INSTEAD. 
	
	@param	objRef		the object reference. Usually, only the class name is used.
	@param	name		the property name
	@param	hidden		if true, search the hidden property table
	@return				the property entry or NULL
	*/
	virtual const PropEntry* hasProperty (const ScCore::Variant& objRef, const ScCore::Variant& name, bool hidden=false) const;
	/**
	Get a list of all properties which are enumerable. Derived classes which implement
	arbitrary lookup over the .name property might want to overload this method. If the dispatcher
	subclasses another dispatcher, the entries of the subclassed dispatchers are added unless there
	is a corresponding entry in this dispatcher with the flag kRemove set. The contents of the
	array will be deleted after use.
	<p>
	THIS METHOD IS DEPRECATED. USE getEnumerableProperties() INSTEAD. 
	@param	objRef		the object reference. Usually, only the class name is used.
	@param	all			the array which contains the names afterwards
	*/
	virtual	void		getEnumNames (const ScCore::Variant& objRef, ScCore::TSimpleArray<ScCore::String>& all) const;
	/**
	Get a property. Interpret the iPropID field and return the result in iValue.
	@param	data		the parameter data block
	*/
	virtual	void		get (Arguments& data);
	/**
	Put a property. The value to put is both in the Variant member and as the first
	parameter of the iArgs member. Interpret the iPropID field.
	@param	data		the parameter data block
	*/
	virtual	void		put (Arguments& data);
	/**
	Call a method. The method address is supplied as a separate argument. This allows for the creation
	of dispatcher specific wrapper methods. The default implementation is to simply call the given method
	with its arguments.
	*/
	virtual	void		call (Arguments& data, DispatcherMethod methodAddr);
	/**
	Forward a call to another dispatcher. This call may be used to directly call a property or method
	inside a dispatcher. The property must have been implemented with a function pointer; get() and put()
	calls are not supported (they may be called directly if desired). The call looks for the property
	defined within the parameter block; optionally, another property name may be supplied. If the property
	entry cannot be found or if the proeprty entry does not have a funtion pointer, a runtime error
	kErrNoFunction is generated. When the class name of the subclassed dispatcher is not the same 
	as the dispatcher to be subclassed, calls to the parent call() method may fail when the subclassed 
	dispatcher checks for the class name of the iThis object.
	@param	data		the data block for the call
	@param	propName	the optional property name
	*/
			void		call (Arguments& data, const char* propName = NULL);
	/**
	Get all properties for a specified class. This method collects all property names, also for ones that
	are not enumerable. It also walks the subclass chain of all dispatchers. Properties that are removed
	with the kRemove flag are not returned.
	@param	clsName		the class name sID
	@param	ignored		ignored
	@param	all			the array that is filled with property info structures
	*/
	virtual	void		getProperties (sID clsName, scint32_t ignored, ScCore::TSimpleArray <const PropInfo>& all) const;
	/**
	Get the names of all enumerable properties for a specific object. This call walks the subclassed chain of 
	dispatchers to return the sID for each and every property that is enumerable. Properties that are removed
	with the kRemove flag are not returned.
	@param	objRef		the object reference
	@param	all			the vector that takes the property sIDs
	@param	flags		enumeration flags (see ClassInfo)
	@see ScCore::ClassInfo
	*/
	virtual	void		getEnumerableProperties (const ScCore::Variant& objRef, 
												 ScCore::SimpleArray& all, 
												 scint32_t flags = ScCore::kJavaScriptLanguage | ScCore::ClassInfo::kEnumerateProperties) const;
	/**
	Get class information about an object. Return NULL if there is no info. 
	The default implementation fills in basic information about the object.
	@param	clsName		the class name
	@return				a pointer to the class info or NULL if there is none
	*/
	virtual	const ScCore::ClassInfo* getClassInfo (const ScCore::String& clsName);
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
	Define a property table. The table must be static and the last element must have
	a null name. The class name must exist.
	@param	ignored		ignored
	@param	tbl			the table to define
	@param	className	the class name to register the table for. If there is none
						supplied, use the first registered class name (this is the default for
						most dispatchers)
	*/
			void		registerProperties (scint32_t, const PropEntry* table);
			void		registerProperties (scint32_t, const PropEntry* table, const char* clsName);
			void		registerProperties (scint32_t, const PropEntry* table, const ScCore::String& clsName);
};

}	// end namespace

#include "ScPostConfig.h"

#endif

