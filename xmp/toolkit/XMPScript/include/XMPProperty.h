// =================================================================================================
// Copyright 2006-2007 Adobe
// All Rights Reserved.
//
// NOTICE:	Adobe permits you to use, modify, and distribute this file in accordance with the terms
// of the Adobe license agreement accompanying it. If you have received this file from a source other 
// than Adobe, then your use, modification, or distribution of it requires the prior written permission
// of Adobe.
// =================================================================================================

#ifndef _XMPConst_h
#define _XMPConst_h

#include "XMPBase.h"


/**
Information about a property that has been requested by getProperty().
*/
class XMPProperty : public XMPBase
{

private:
	/** the properties manager */
	ScCore::LivePropertyManager	properties;
	/** Contains the namespace of the property */
	ScCore::String schemaNS;
	/** Contains the namespace of the property */
	ScCore::String propPath;
	/** the actual locale of alternative language properties, null otherwise. */
	ScCore::String locale;
	/** Contains the value of the property */
	ScCore::Variant value;
	/** Contains the options of the property */
	int options;


public:
	/** Constructor with initial values */
	XMPProperty(ScCore::Variant value, int options);

	/** Constructor with initial values and locale */
	XMPProperty(ScCore::String locale, ScCore::Variant value, int options);

	/** Constructor with initial values and schemaNS and property path*/
	XMPProperty(ScCore::String schemaNS, ScCore::String propPath, ScCore::Variant value, int options);


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
	@return Returns the property value coverted as String.
	*/
	virtual	void				toString (ScCore::String& result) const;
};

#endif
