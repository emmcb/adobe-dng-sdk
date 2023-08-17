// =================================================================================================
// Copyright 2006-2007 Adobe
// All Rights Reserved.
//
// NOTICE:	Adobe permits you to use, modify, and distribute this file in accordance with the terms
// of the Adobe license agreement accompanying it. If you have received this file from a source other 
// than Adobe, then your use, modification, or distribution of it requires the prior written permission
// of Adobe.
// =================================================================================================

#ifndef _XMPAliasInfo_h
#define _XMPAliasInfo_h

#include "XMPBase.h"


/**
Represents an XMP Alias: a namespace, property name and an array form.
*/
class XMPAliasInfo : public XMPBase
{
	/** the LiveObject properties manager */
	ScCore::LivePropertyManager	properties;

public:
	/** Constructor with initial values */
	XMPAliasInfo(std::string propNS, std::string propName, int arrayForm);


	/**
	Read the value of a property. Return 0 if the property could
	be read, an error code otherwise.
	@param	id					the property ID
	@param	value				the location to take the value
	@param	errors				an optional pointer to extended error information
	@return						kErrUndefined
	*/
	virtual	scerror_t			get (int32_t id, ScCore::Variant& value, ScCore::Error* errors = null) const;

private:
	/** Contains the namespace of the aliased property */
	std::string propNS;
	/** Contains the name of the aliased property */
	std::string propName;
	/** Contains the array form (seq, bag, alt) of the aliased property */
	int arrayForm;
};

#endif
