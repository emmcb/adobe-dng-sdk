// =================================================================================================
// Copyright 2006-2007 Adobe
// All Rights Reserved.
//
// NOTICE:	Adobe permits you to use, modify, and distribute this file in accordance with the terms
// of the Adobe license agreement accompanying it. If you have received this file from a source other 
// than Adobe, then your use, modification, or distribution of it requires the prior written permission
// of Adobe.
// =================================================================================================

#ifndef _XMPDateTime_h
#define _XMPDateTime_h

#include "XMPBase.h"


/**
Wrapper for the XMP_DateTime struct.
*/
class XMPDateTime : public XMPBase
{
	/** the properties manager */
	ScCore::LivePropertyManager	properties;

public:
	/** Constructor which initializes with the current date */
	XMPDateTime();


	/** Constructor with initial values */
	XMPDateTime(XMP_DateTime dateTime);


	/**
	Initialize this object. ExtendScript calls this method after creating a new 
	object by calling the factory object's clone() method. Its arguments are the same
	as passed in to the "new Object()" call.
	*/
	virtual	scerror_t			initialize (const ScCore::Array& args, ScCore::Error* error = null);


	/**
	Read the value of a property. Return 0 if the property could
	be read, an error code otherwise.
	@param	id					the property ID
	@param	value				the location to take the value
	@param	errors				an optional pointer to extended error information
	@return						kErrUndefined
	*/
	virtual	scerror_t			get (int32_t id, ScCore::Variant& value, ScCore::Error* errors = null) const;


	/**
	Store the value of a property. Return 0 if the property could
	be stored, an error code otherwise.
	@param	id					the property ID
	@param	value				the value to store
	@param	errors				an optional pointer to extended error information
	@return						kErrUndefined
	*/
	virtual	scerror_t			put (int32_t id, const ScCore::Variant& value, ScCore::Error* errors = null);


	/**
	Call a method. Return 0 if the method could be called, an error code otherwise.
	@param	id					the property ID
	@param	args				the method arguments
	@param	result				the return value.
	@param	errors				an optional pointer to extended error information
	@return						kErrUndefined
	*/
	virtual	scerror_t			call (int32_t id, const ScCore::Array& args, 
									  ScCore::Variant& result, ScCore::Error* errors = null);
									  

	/**
	Return the string "[object XMPDateTime]".
	*/
	virtual	void				toString (ScCore::String& result) const;


	virtual XMP_DateTime		getDateTime();

private:
	/** Contains the wrapped instance of XMP_DateTime. */
	XMP_DateTime dateTime;
};

#endif
