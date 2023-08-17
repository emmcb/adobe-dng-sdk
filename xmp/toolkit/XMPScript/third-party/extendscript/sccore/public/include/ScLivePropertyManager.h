/**************************************************************************
*
* ADOBE CONFIDENTIAL
* ___________________
*
*  Copyright 1998 - 2007 Adobe Systems Incorporated
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
#ifndef _ScLivePropertyManager_h
#define _ScLivePropertyManager_h

#include "ScRoot.h"
#include "ScLiveObject.h"
#include "ScPreConfig.h"

namespace ScCore
{

class ServiceInfo;
class SimpleArray;
class String;


/**
Many clients of LivePropertyManager implement a fixed set of properties that is known at 
component creation time, and does not change. The LivePropertyInfo structure is used
to represent the static information about a single property, and the LivePropertyInfoList
is an array (terminated by an entry with a null 'name' pointer) which represents the entire
set of properties for a component.
<p>
Constructors are LiveObject factories with the method initialize() implemented. To define
argument and return type information for a constructor, use an entry with an empty string.
This entry is only used to return information about constructor arguments. The return type
must be set the the class name of the class - it is the name returned by the constructor
service info.
*/

struct LivePropertyInfo {
	/// Additional attribute bits
	enum
	{
		/// The InfoType goes into the lowest 4 bits.
		kTypeMask			= 0x0000000F,
		/// The property is not enumerated.
		kAttrDontEnum		= LiveObject::kAttrDontEnum,
		/// The property cannot be deleted.
		kAttrDontDelete		= LiveObject::kAttrDontDelete,
		///	Hidden properties don't show up in a normal enumeration (may be deprecated, for instance).
		kAttrHidden			= LiveObject::kAttrHidden,
		__Type__32bit__		= 0x7FFFFFFF
	};
	/**	
	The name by which a property is known to the languages specified by 'langMask'.
	A zero-length name identifies an unnamed property. ASCII only. For a constructor
	definition, this string is empty.
	*/
	const	char*			name;
	/**
	The property ID: there can be more than one entry with the same 'id' for different
	languages. For a constructor definition, this entry is ignored.
	*/
			scint32_t			id;
	/**
	The property type; must be one of kInvalid (for properties that hide other like-named
	properties), kReadOnly, kReadWrite,	and kMethod. Other bits describe additional 
	attributes and the language. If no language bits are set, the entry is valid for all
	languages. For a constructor definition, this entry should be set to kMethod.
	*/
			scint32_t			typeInfo;
	/**
	The data type of the property, or return type and parameter info for methods.
	Please refer to TypeInfo::parse() for a detailed description.
	@see	ScCore::TypeInfo::parse
	*/
		const char*			dataTypeInfo;
};

typedef const LivePropertyInfo* LivePropertyInfoList;

class LivePropMgrData;

/**
LivePropertyManager: a LiveObject helper class.
<p>
LivePropertyManager is a helper class designed for use by implementations of LiveComponent and
LiveObject subclasses. Scripting language environments have different naming conventions for
the same properties: some languages are case insensitive, some use completely different spellings
of names for the same property. Also, it can be complex to correctly implement the LiveObject
enumerate() method, because of language differences and the numerous options for specifying
which properties to return.
<p>
Using LivePropertyManager relieves implementors of LiveObjects and/or LiveComponents of the
burden of implementing the different, per-language behaviors of property name-to-ID mapping
and property ID enumeration in each LiveObject or LiveComponent. Using LivePropertyManager is
optional, but in most cases, it can make implementing an LiveComponent or LiveObject subclass
simpler.
<p>
LivePropertyManager can be used directly as a member of a LiveObject/Component subclass, or as an
external object that is referenced by a LiveObject/Component. It can be used with LiveObjects
that implement all their properties directly, all their properties via LiveComponents, or a
mixture of these models. Typically, there is a single LivePropertyManager instance for the
set of properties implemented by a LiveObject/Component, in order to reduce the memory required
for property names and attributes.
<p>
LiveComponent subclasses typically implement a fixed set of properties, so LivePropertyManager
is optimized to work with a client-supplied static description of a set of properties. However,
it will dynamically adapt to managing properties that are registered by a component at any time
during its life. Various methods for registering property attributes are provided, allowing the
client to choose the model which is most appropriate.
<p>
Once a LiveObject/Component subclass has created an instance of LivePropertyManager and
registered property descriptions with it, the implementation of the LiveObject/Component's
getIDForName, getNameForID, enumerate, and getType methods can simply call the corresponding
LivePropertyManager methods. LivePropertyManager also provides a getServiceInfo method
which can assist with the creation of an ServiceInfo instance, filled in with whatever
information is available to the LivePropertyManager (i.e., the client must still do some
work to complete filling in the structure). The getIDForName() method supports the 
kAttrLookupMethod flag; if set, only method entries are returned.
<p>
If you want to write a LivePropertyManager that may change its content, you should call the
createLock() method. This method creates a multithreading lock and makes the usage of this
instance safe across threads. Any LiveObject that accesses a property manager automatically
calls its lock() and unlock() methods. If the contents of the manager are static, there is
no need to create a lock.
@see ServiceInfo
*/

class SC_API LivePropertyManager : public Root, public Lockable {
	/// Private copy ctor, because LivePropertyManager has pointer data members.
								LivePropertyManager (const LivePropertyManager&);
	/// Private assignment operator, because LivePropertyManager has pointer data members.
	LivePropertyManager&		operator= (const LivePropertyManager&);
public:
	/**@name Constructors and Destructors */
	//@{
	/**
	The default ctor creates a property manager with no known properties. The client must call
	registerProperty or registerProperties to give it properties to manage.
	*/
								LivePropertyManager ();
	/**
	This ctor creates a property manager with a static set of properties to manage. It should be
	used when the set of properties is known at initialization time, and is not likely to change.
	*/
								LivePropertyManager (LivePropertyInfoList properties);
	/**
	The default dtor releases any memory allocated for dynamically-registered properties. It does
	nothing with the static set of properties provided to a ctor or registered via registerProperties().
	*/
	virtual						~LivePropertyManager ();
	//@}
	/**@name Property Registration */
	//@{
	/**
	Register a <property name, ID, type, attributes, languages> tuple with the property manager.
	@param	name		the name this property is known to scripting languages by. A null name
						pointer or a zero-length name is used for unnamed properties: these are
						typically internal properties which are not visible to scripting environments.
	@param	id			the property ID
	@param	typeInfo	the property type plus one or more property attributes 'OR'ed together plus
						a mask specifying languages that know this property by the above 'name'. The
						default value is a mask specifying all supported scripting languages. A value of
						kAppleEventEnvironment would mean the name is known only to AppleScript, 
						(kJavaScriptLanguage | kActionScriptLanguage) would mean the name is known
						to JavaScript and ActionScript, etc. registerProperty() may be called multiple
						times with the same 'id' to register different names for the same property.
						In such cases, the 'attributes' should be the same each time registerProperty()
						is called with an id already registered.
	@param	dataTypeInfo the data type info as described in the LivePropertyInfo structure.
	*/
			void				registerProperty (const char* name, scint32_t id, scint32_t typeInfo, const char* dataTypeInfo = NULL);
	///	Another form of registration: semantics identical to other registerProperty method.
			void				registerProperty (const LivePropertyInfo& propInfo);
	/**
	Register a static set of properties to manage. This form should be used when the set of properties
	will not change. If called more than once, the new set of properties will replace any previous set.
	The caller must not delete or modify the contents of 'properties': registerProperties uses it directly,
	it does not make a copy.
	*/
			void				registerProperties (LivePropertyInfoList properties);
	//@}
	/**@name Accessors */
	//@{
	/**
	getIDForName, getNameForID, enumerate, and getType have the same semantics as the like-named
	methods in the LiveObject class.
	*/
	virtual	scerror_t			getIDForName (const String& name, scint32_t& id,
											  InfoType& type, scint32_t lang) const;
	virtual	scerror_t			getNameForID (scint32_t id, String& name, scint32_t lang) const;
	virtual	InfoType			getType (scint32_t id) const;
	virtual	scerror_t			enumerate (SimpleArray& propertyIDs, scint32_t langAndBits) const;
	/**
	Return the ServiceInfo that describes the constructor. The name of that service
	is the string that has been used as the return value in the description. If there
	is no constructor entry defined, the return value is null.
	*/
	virtual	const ServiceInfo*	getConstructor (scint32_t lang = kJavaScriptLanguage) const;
	/**
	Return a ServiceInfo structure with info about the property specified by 'id',
	for the language specified by 'lang'. Return null if the property is unknown.
	*/
	virtual	const ServiceInfo*	getServiceInfo (scint32_t id, scint32_t lang = kJavaScriptLanguage) const;
	///
	virtual	const ServiceInfo*	getServiceInfo (const String& name, scint32_t lang = kJavaScriptLanguage) const;
	/**
	Check whether this instance has operator overloads defined. These overloads have special IDs
	that range between LiveBase::kJSFirstOverload and LiveBase::kJSLastOverload.
	*/
			bool				hasOperators() const;
	//@}
private:
			LivePropMgrData*	mData;
};

}	// end namespace

#include "ScPostConfig.h"
#endif

