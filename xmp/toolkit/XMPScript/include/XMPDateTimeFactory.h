// =================================================================================================
// Copyright 2006-2007 Adobe
// All Rights Reserved.
//
// NOTICE:	Adobe permits you to use, modify, and distribute this file in accordance with the terms
// of the Adobe license agreement accompanying it. If you have received this file from a source other 
// than Adobe, then your use, modification, or distribution of it requires the prior written permission
// of Adobe.
// =================================================================================================

#ifndef _XMPDateTimeFactory_h
#define _XMPDateTimeFactory_h

#include "XMPBase.h"


/**
Factory for the XMPDateTime-object.
*/
class XMPDateTimeFactory : public XMPBase
{
	/** the properties manager */
	ScCore::LivePropertyManager	properties;

public:
	/** constructor */
	XMPDateTimeFactory();


	/**
	Factory support: clone this object.
	@param	newObj				takes the newly created object
	@param	cloneAll			if false, do not clone dynamic properties
	@return						an error code (0 for no error)
	*/
	virtual	scerror_t			clone (ScCore::LiveObject** newObj, bool deepClone = true) const;
};

#endif
