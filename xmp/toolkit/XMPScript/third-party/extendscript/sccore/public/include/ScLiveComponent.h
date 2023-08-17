/**************************************************************************
* $File$
* $Author$
* $DateTime$
* $Revision$
* $Change$
*
* ADOBE CONFIDENTIAL
* ___________________
*
*  Copyright 1998 - 2003 Adobe Systems Incorporated
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
**************************************************************************/

#if SC_ONCE
#pragma once
#endif
#ifndef _ScLiveComponent_h
#define _ScLiveComponent_h

#include "ScLiveBase.h"
#include "ScClassInfo.h"
#include "ScPreConfig.h"

namespace ScCore
{

class LiveObject;
class LivePropertyManager;

/**
A LiveComponent encapsulates one or more object properties. It has a get, a put
and a call interface. It contains an inspection interface so object browsers 
may display information about a property. If you want to set or return an object, 
you may, of course, return or set any object reference if you use the property 
in an ExtendScript context. Without ExtendScript, your use of objects is restricted
to LiveObjects.
<p>
Since newly added properties of a LiveObject hide the properties of previously 
added property objects, you can override default implementations of specific 
properties by adding a customized property object to a LiveObject that redefines 
the property. You can even remove present properties by letting getType()
return kRemove for a given property ID. You should never use any IDs in the
range 0x1F800000 to 0x1FFFFFFF, since these IDs are reserved as dynamic IDs.
<p>
LiveComponents contains support for indexed access. If you want to implement accesses
by index, override the getLength/setLength/getByIndex/setByIndex methods. The default
implementations return kErrUndefined.
<p>
When implementing a LiveComponent, make sure that the component does not throw any
exceptions unless you know that your application handles the throwing of exceptions
across component boundaries. The default implementation is exception safe. This class
also inherits the createLock(), lock() and unlock() methods from LiveBase. LiveObjects
call lock() and unlock() when accessing a LiveComponent.
<p>
Unlike LiveObjects, LiveComponents are never invalidated.
@see sRuntimeErrors
@see ClassInfo
*/

class SC_API LiveComponent : public LiveBase
{
protected:
								LiveComponent();
	virtual						~LiveComponent();
public:
	/**
	Clear all dynamic data and reset the component to its initial state.
	*/
	virtual	void				reset();
	/**
	Retrieve the ID of a property. If the property exists, fill in its ID 
	and type data and return 0 for no error. Return kErrUndefined if the
	property does not exist. Use the returned ID for subsequent property 
	accesses. The type field is set to one of the enum values kReadOnly, 
	kReadWrite or kMethod. For unrecognized names, set the ID to zero
	and the type to kInvalid and return kErrUndefined. 
	Must be implemented for scripting languages.
	@param	thisObj				the object to act upon
	@param	id					the property ID
	@param	name				the property name
	@param	id					takes the ID
	@param	type				takes the type of the property
	@param	lang				the language ID
	@return						an error code (0 for no error)
	*/
	virtual	scerror_t			getIDForName (const LiveObject& thisObj, const String& name, 
											  scint32_t& id, InfoType& type, scint32_t lang = kJavaScriptLanguage) const;
	/**
	Retrieve the name of a property. Use this call to convert an ID to
	a name. If the ID cannot be matched to a name, clear the string and return 
	kErrUndefined. Implement this method to support scripting languages. 
	If not implemented, the method returns kErrUndefined.
	@param	thisObj				the object to act upon
	@param	id					the property ID to match
	@param	name				takes the name
	@param	lang				the language ID
	@return						an error code (0 for no error)
	*/
	virtual	scerror_t			getNameForID (const LiveObject& thisObj, scint32_t id, String& name,
											  scint32_t lang = kJavaScriptLanguage) const;
	/**
	Retrieve the type of a property. This call returns one of the enum
	values kReadOnly, kReadWrite or kMethod. For unrecognized
	IDs, return kInvalid. Must be implemented.
	@param	thisObj				the object to act upon
	@param	id					the property ID
	@return						the type of the property
	*/
	virtual	InfoType			getType (const LiveObject& thisObj, scint32_t id) const;
	/**
	Check whether this instance has operator overloads defined. These overloads have special IDs
	that range between LiveBase::kJSFirstOverload and LiveBase::kJSLastOverload. The default
	implementation checks the property manager if present.
	*/
	virtual	bool				hasOperators() const;
	/**
	Fill the supplied array with the IDs of all properties that this object 
	supports. Use the given language specifier to determine which properties
	need to be returned. You can supply additional browse bits to override the
	default behavior for a scripting language. The default implementation simply 
	returns kErrOK.
	@param	thisObj				the object to act upon
	@param	propertyIDs			the array to fill
	@param	lang				the language ID plus browse bits
	@return						an error code (0 for no error)
	*/
	virtual	scerror_t			enumerate (const LiveObject& thisObj, SimpleArray& propertyIDs, 
										   scint32_t langAndBits = kJavaScriptLanguage) const;
	/**
	Get information about the given property ID. The method returns 
	kErrUndefined if there is no property information.
	@param	thisObj				the object to act upon
	@param	id					the property ID to query
	@param	lang				the langugage to look for (optional)
	@return						a pointer to a ServiceInfo instance
	*/
	virtual	const ServiceInfo*	getServiceInfo (const LiveObject& thisObj, scint32_t id, scint32_t lang = kJavaScriptLanguage) const;
	/**
	Read the value of a property using the supplied LiveObject. Return 0 if the property could
	be read, an error code otherwise.
	@param	thisObj				the object to act upon
	@param	id					the property ID
	@param	value				the location to take the value
	@param	errors				an optional pointer to extended error information
	@return						kErrUndefined
	*/
	virtual	scerror_t			get (const LiveObject& thisObj, scint32_t id, Variant& value, Error* errors = NULL) const;
	/**
	Store the value of a property using the supplied LiveObject. Return 0 if the property could
	be stored, an error code otherwise.
	@param	thisObj				the object to act upon
	@param	id					the property ID
	@param	value				the value to store
	@param	errors				an optional pointer to extended error information
	@return						kErrUndefined
	*/
	virtual	scerror_t			put (LiveObject& thisObj, scint32_t id, const Variant& value, Error* errors = NULL);
	/**
	Call a method using the supplied LiveObject. Return 0 if the method could
	be called, an error code otherwise.
	@param	thisObj				the object to act upon
	@param	id					the property ID
	@param	args				the method arguments
	@param	result				the return value.
	@param	errors				an optional pointer to extended error information
	@return						kErrUndefined
	*/
	virtual	scerror_t			call (LiveObject& thisObj, scint32_t id, const Array& args, Variant& result, Error* errors = NULL);
	/**
	Does the supplied method ID work with any LiveObject as the "this" object, or does the ID
	care about the "this" object at all? If so, return true. The default implementation
	return false. This makes LiveObject::apply() return sERR_WRONG CLASS if the supplied
	"this" object's class name is not equal to the class name of the LiveObject that was called.
	*/
	virtual	bool				isGeneric (scint32_t id) const;
	/**
	Check whether this component supports indexing. You need to overload this method to
	return true if you implement the index access methods below. This implementation
	returns false to indicate that the index access methods are not implemented.
	*/
	virtual	bool				isIndexable() const;
	/**
	Retrieve the length of a collection using the supplied LiveObject. This default 
	implementation sets the length to 0 and returns kErrUndefined.
	@param	thisObj				the object to act upon
	@param	length				takes the length
	@return						an error code (0 for success, kErrUndefined if not implemented)
	*/
	virtual	scerror_t			getLength (const LiveObject& thisObj, scuint32_t& length) const;
	/**
	Set the length of a collection using the supplied LiveObject. This default implementation does nothing 
	and returns kErrUndefined.
	@param	thisObj				the object to act upon
	@param	length				the length
	@return						an error code (0 for success, kErrUndefined if not implemented)
	*/
	virtual	scerror_t			putLength (LiveObject& thisObj, scuint32_t length);
	/**
	Read the value of an index property using the supplied LiveObject. This default implementation 
	clears the value and returns kErrRange.
	@param	thisObj				the object to act upon
	@param	index				the index
	@param	value				the location to take the value
	@param	errors				an optional pointer to extended error information
	@return						an error code (0 for no error)
	*/
	virtual	scerror_t			getByIndex (const LiveObject& thisObj, scuint32_t index, Variant& value, Error* errors = NULL) const;
	/**
	Set the value of an index property using the supplied LiveObject. This default implementation 
	clears the value and returns kErrRange.
	@param	thisObj				the object to act upon
	@param	index				the index
	@param	value				the location to take the value
	@param	errors				an optional pointer to extended error information
	@return						an error code (0 for no error)
	*/
	virtual	scerror_t			putByIndex (LiveObject& thisObj, scuint32_t index, const Variant& value, Error* errors = NULL);
	/**
	Delete an element by index.
	@param	thisObj				the object to act upon
	@param	index				the index
	@return						an error code
	*/
	virtual	scerror_t			deleteByIndex (LiveObject& thisObj, scuint32_t index);
};

}	// end Namespace

#include "ScPostConfig.h"
#endif

