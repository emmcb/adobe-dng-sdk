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
#ifndef _ScLiveArray_h
#define _ScLiveArray_h

#include "ScLiveCollection.h"
#include "ScArray.h"
#include "ScPreConfig.h"

namespace ScCore
{

class String;

/// the LiveID for a LiveArray.
const scint32_t kLiveIDArrayObject	= 0x20617272;	// 'arr'
/// The LiveID for the Array object inside a LiveArray.
const scint32_t kLiveIDArray			= 0x20617264;	// ' ard'

/**
A LiveArray is a live collection containing an Array object which it maintains.
The array object is accessible from the outside, where it can be manipulated;
the LiveArray is just a wrapper around this array. The LiveArray does not
support messaging or components for index methods.
<p>
LiveArrays are exception safe. They also inherit the createLock(), lock()
and unlock() methods from LiveBase.
*/

class SC_API LiveArray : public LiveCollection
{
		Array				mArray;
public:
	/**@name Constructors */
	//@{
	/**
	The default ctor sets up an empty object that may be filled later. You can
	supply a class name and/or the ID of the length property. Set readonly to 
	true to have immutable elements.
	@param	className			if not set, default to "Collection" (ASCII only)
	@param	lengthID			the ID of the length property (0: use dynamic ID)
	@param	readOnly			true for a read only collection
	*/
								LiveArray 
								(const char* className = NULL, scint32_t lengthID = 0, bool readOnly = false);
	/**
	The constructor populates the internal data array with the contents of the
	supplied data array. Set readOnly to true to have immutable elements.
	@param	data				the initial values for the collection
	@param	className			if not set, default to "Collection" (ASCII only)
	@param	lengthID			the ID of the length property (0: use dynamic ID)
	@param	readOnly			true for a read only collection
	*/
								LiveArray 
								(const Array& data, const char* className = NULL, 
								scint32_t lengthID = 0, bool readOnly = false);

	virtual						~LiveArray();
	//@}
	/**@name Class name and casting */
	//@{
	/**
	Retrieve this instance if the ID is kLiveIDCollection. Also retrieve the internal
	array (a Array instance with the constant kLiveIDArray. Do not modify
	this array directly, since changes to the array are not forwarded to a LiveComponent
	or a message listener.
	@param	typeID				the type ID of the LiveObject
	@return						this or null
	*/
	virtual void*				getAs (scint32_t typeID);
	///
	virtual const void*			getAs (scint32_t typeID) const;
	/// Helpers: get the enclosed array
	inline	Array&				getArray() { return mArray; }
	///
	inline	const Array&		getArray() const { return mArray; }
	/// Get the getAs() interfaceID of this class. Useful for template programming.
	static	scint32_t				getInterfaceID() { return kLiveIDArrayObject; }
	/**
	Clear the array and reset the object to its initial state. This call
	calls the reset() method of all LiveComponents.
	*/
	virtual	void				reset();
	//@{
	/**@name Indexed accesses */
	//@{
	/**
	Retrieve the length of the embedded array.
	@param	length				takes the length
	@return						kErrOK
	*/
	virtual	scerror_t			getLength (scuint32_t& length) const;
	/**
	Set the length of the embedded array and delete excess elements if necessary.
	@param	length				the length to set
	@return						an error code
	*/
	virtual	scerror_t			putLength (scuint32_t length);
	/**
	Read the value of an index property. The LiveArray sets the return value to Undefined
	and returns kErrOK if the index is out of range or the index points to a non-existing
	element.
	@param	index				the index
	@param	value				the location to take the value
	@param	errors				an optional pointer to extended error information
	@return						an error code (0 for no error)
	*/
	virtual	scerror_t			getByIndex (scuint32_t index, Variant& value, Error* errors = NULL) const;
	/**
	Set the value of an index property. The LiveArray stores the value regardless of the 
	index. If the index is beyond the length, the array is expanded automatically.
	@param	index				the index
	@param	value				the location of the value
	@param	errors				an optional pointer to extended error information
	@return						an error code (0 for no error)
	*/
	virtual	scerror_t			putByIndex (scuint32_t index, const Variant& value, Error* errors = NULL);
	//@}
	/**@name Other */
	//@{
	/**
	Create a string representation of this object. The method first
	looks for a defined "toString" method. If not found, it creates
	an array representation like "1,2,3..." of up to 1000 elements.
	*/
	virtual	void				toString (String& result) const;
	//@}
};

}	// end Namespace

#include "ScPostConfig.h"
#endif

