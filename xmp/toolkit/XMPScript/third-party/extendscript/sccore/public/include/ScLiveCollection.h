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
#ifndef _ScLiveCollection_h
#define _ScLiveCollection_h

#include "ScLiveObject.h"
#include "ScPreConfig.h"

namespace ScCore
{

/// the LiveID for a LiveCollection.
const scint32_t kLiveIDCollection	= 0x20636F6C;	// ' col'

/**
A LiveCollection is the base for any collection object. It supports the length
property and getByIndex() as well as putByIndex() methods. It supports messaging,
so you could write a client that does the actual functionality for you. It also
attempts to call the index xxxLength() and xxxByIndex() methods of any enclosed
LiveComponent it that component reports itself as being indexable.
<p>
LiveCollections may be read only. Many collections are in fact read only, 
like the documents collection of an application. Any attempt to alter the 
contents of such a collection should be ignored or rejected. Therefore, 
the constructors take an optional Boolean argument to indicate that the 
instance should be read only. If set to true, the type of the length property
is reported as read only, and putLength() and putByIndex() return kErrReadOnly.
<p>
LiveCollections are exception safe. They also inherit the createLock(), lock()
and unlock() methods from LiveBase.
*/

class SC_API LiveCollection : public LiveObject
{
	const scint32_t				mLengthID;		// the ID for the length property
	const bool					mReadOnly;
public:
	/**@name Constructors */
	//@{
	/**
	The default ctor sets up an empty object that may be filled later. You can
	supply a class name and/or the ID of the length property. Set readonly to 
	true to have readonly length and index properties.
	@param	className			if not set, default to "Collection" (ASCII only)
	@param	lengthID			the ID of the length property (0: use dynamic ID)
	@param	readOnly			true for a read only collection
	*/
								LiveCollection 
								(const char* className = NULL, scint32_t lengthID = 0, bool readOnly = false);

	virtual						~LiveCollection();
	/**
	Is the collection read only? This affects the length and index properties.
	*/
	inline	bool				isReadOnly() const { return mReadOnly; }
	/// Retrieve the property ID for the length property.
	inline	scint32_t				getLengthID() const { return mLengthID; }
	//@}
	/**@name Class name and casting */
	//@{
	/**
	Retrieve this instance if the ID is kLiveIDCollection. 
	@param	typeID				the type ID of the LiveObject
	@return						this or null
	*/
	virtual void*				getAs (scint32_t typeID);
	///
	virtual const void*			getAs (scint32_t typeID) const;
	/// Get the getAs() interfaceID of this class. Useful for template programming.
	static	scint32_t				getInterfaceID() { return kLiveIDCollection; }
	/**
	Retrieve the class name; if not set, set it to "Collection". May be overloaded
	to implement delayed lookup of the class name. The base member iClassName can be used
	to store the class name.
	*/
	virtual	const String&		getClassName() const;
	//@}
	/**@name Name lookup */
	//@{
	/**
	Retrieve the ID for a property. This call implements the lookup for 
	the length property; for all other properties, the base class 
	implementation is called. 
	@param	name				the property name
	@param	id					takes the ID
	@param	type				takes the type of the property
	@param	lang				the language ID
	@return						an error code (0 for no error)
	*/
	virtual	scerror_t			getIDForName (const String& name, scint32_t& id, InfoType& type,
											  scint32_t lang = kJavaScriptLanguage) const;
	/**
	Retrieve the name of a property. This call implements the lookup for 
	the length property; for all other properties, the base class 
	implementation is called. 
	@param	id					the property ID to match
	@param	name				takes the name
	@param	lang				the language ID
	@return						an error code (0 for no error)
	*/
	virtual	scerror_t			getNameForID (scint32_t id, String& name, scint32_t lang = kJavaScriptLanguage) const;
	/**
	Retrieve the type of a property. This call returns either kReadOnly
	or kReadWrite for the "length" property depending on whether the object
	is read only or not.
	@param	id					the property ID
	@return						the type of the property
	*/
	virtual	InfoType			getType (scint32_t id) const;
	/**
	Call the base class method and add  the property ID for the length property if
	the respective browser bits are set.
	@param	propertyIDs			the array to fill
	@param	lang				the language ID
	@return						an error code (0 for no error)
	*/
	virtual	scerror_t			enumerate (SimpleArray& propertyIDs, scint32_t langAndBits = kJavaScriptLanguage) const;
	//@}
	/**@name Property interface */
	//@{
	/**
	Read the value of the "length" property. Return 0 if the property could
	be read, an error code otherwise. Call getLength() to get the length and
	the base class implementation for all other properties.
	@param	id					the property ID
	@param	value				the location to take the value
	@param	errors				an optional pointer to extended error information
	@return						an error code (0 for no error)
	*/
	virtual	scerror_t			get (scint32_t id, Variant& value, Error* errors = NULL) const;
	/**
	Store the value of the "length" property. Return 0 if the property could
	be stored, an error code otherwise. Call putLength() to set the length and
	the base class implementation for all other properties. Return kErrReadOnly 
	if the object is read only.
	@param	id					the property ID
	@param	value				the value to store
	@param	errors				an optional pointer to extended error information
	@return						an error code (0 for no error)
	*/
	virtual	scerror_t			put (scint32_t id, const Variant& value, Error* errors = NULL);
	//@{
	/**@name Indexed accesses */
	//@{
	/**
	Retrieve the length of a collection. First, an attempt is made to call a LiveComponent
	to get the length. Then, the method broadcasts a kLiveMsgGetLength message to obtain 
	the length. If this fails, 0 array is returned.
	@param	length				takes the length
	@return						an error code (kErrUndefined if not implemented, 0 for success)
	*/
	virtual	scerror_t			getLength (scuint32_t& length) const;
	/// The usual "length" method. Returns 0 on errors.
	inline	scuint32_t				length() const;
	/**
	Set the length of a collection. First, an attempt is made to call a LiveComponent
	to store the length. Then, the method broadcasts a kLiveMsgPutLength message to set 
	the length.
	@param	length				the length to set
	@return						an error code (kErrUndefined if not implemented, 0 for success)
	*/
	virtual	scerror_t			putLength (scuint32_t length);
	/**
	Read the value of an index property. First, an attempt is made to call a LiveComponent
	to retrieve the data. Then, the method broadcasts a kLiveMsgGetByIndex message to get 
	the value. The method should return kErrRange if the index is out of range; note, however,
	that some implementations (like the LiveArray) choose to return an Undefined value and
	return kErrOK instead.
	@param	index				the index
	@param	value				the location to take the value
	@param	errors				an optional pointer to extended error information
	@return						an error code (0 for no error)
	*/
	virtual	scerror_t			getByIndex (scuint32_t index, Variant& value, Error* errors = NULL) const;
	/**
	Set the value of an index property. First, an attempt is made to call a LiveComponent
	to store the data. Then, the method broadcasts a kLiveMsgPutByIndex message to store
	the value. The method should return kErrRange if the index is out of range; note however,
	that some implementations (like the LiveArray) simply extend the array when an index
	higher than the current maximum is used.
	@param	index				the index
	@param	value				the location of the value
	@param	errors				an optional pointer to extended error information
	@return						an error code (0 for no error)
	*/
	virtual	scerror_t			putByIndex (scuint32_t index, const Variant& value, Error* errors = NULL);
	/**
	Delete an element by index.
	@param	index				the index
	@return						an error code
	*/
	virtual	scerror_t			deleteByIndex (scuint32_t index);
	//@}
};

inline scuint32_t LiveCollection::length() const
{
	scuint32_t myLength;
	if (0 != getLength (myLength))
		myLength = 0;
	return myLength;
}

}	// end Namespace

#include "ScPostConfig.h"
#endif

