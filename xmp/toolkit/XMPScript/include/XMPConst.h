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
Wraps all constants needed for the use of the XMP ExtendScript API.
*/
class XMPConst : public XMPBase
{
	/** the properties manager */
	ScCore::LivePropertyManager	properties;

public:
	/** Constructor */
	XMPConst();


	/**
	The overridden function prevents the new operator.
	@param	newObj				takes the newly created object
	@param	cloneAll			if false, do not clone dynamic properties
	@return						an error code (0 for no error)
	*/
	virtual	scerror_t			clone (ScCore::LiveObject** newObj, bool deepClone = true) const;


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
	virtual	scerror_t			enumerate (ScCore::SimpleArray& propertyIDs, int32_t langAndBits = ScCore::kJavaScriptLanguage) const;
};

#endif
