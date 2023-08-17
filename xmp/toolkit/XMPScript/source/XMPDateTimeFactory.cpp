// =================================================================================================
// Copyright 2006-2007 Adobe
// All Rights Reserved.
//
// NOTICE:	Adobe permits you to use, modify, and distribute this file in accordance with the terms
// of the Adobe license agreement accompanying it. If you have received this file from a source other 
// than Adobe, then your use, modification, or distribution of it requires the prior written permission
// of Adobe.
// =================================================================================================

#include "XMPDateTimeFactory.h"
#include "XMPDateTime.h"

using namespace ScCore;


/**
Initializes the native XMP Toolkit and adds a new JavaScript Error for XMP
*/
XMPDateTimeFactory::XMPDateTimeFactory() : XMPBase("XMPDateTime")
{
	// EMPTY
}


/** Support for the JavaScript "new" operator */
scerror_t XMPDateTimeFactory::clone (LiveObject** newObj, bool) const
{
	*newObj = new XMPDateTime();
	return kErrOK;
}
