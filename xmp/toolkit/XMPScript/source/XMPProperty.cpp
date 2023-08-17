// =================================================================================================
// Copyright 2006-2007 Adobe
// All Rights Reserved.
//
// NOTICE:	Adobe permits you to use, modify, and distribute this file in accordance with the terms
// of the Adobe license agreement accompanying it. If you have received this file from a source other 
// than Adobe, then your use, modification, or distribution of it requires the prior written permission
// of Adobe.
// =================================================================================================

#include "XMPProperty.h"

using namespace ScCore;


// This section defines the properties and uses the LivePropertyManager
// to do the usual handling of the properties.

#define	kNamespace	100
#define	kPath		101
#define	kLocale		102
#define	kValue		103
#define	kOptions	104


static const ScCore::LivePropertyInfo INSTANCE_PROPERTIES[] = {
	{ "namespace",	kNamespace,	ROPROP, "string" },
	{ "path",		kPath,		ROPROP, "string" },
	{ "locale",		kLocale,	ROPROP, "string" },
	{ "value",		kValue,		ROPROP, "variant" },
	{ "options",	kOptions,	ROPROP, "number" }, // number
	{ null }
};


XMPProperty::XMPProperty(Variant _value, int _options) : XMPBase ("XMPProperty"), properties (INSTANCE_PROPERTIES)
{
	this->value = _value;
	this->options = _options;

	setPropertyManager (&properties);
}


XMPProperty::XMPProperty(String _locale, Variant _value, int _options) : XMPBase ("XMPProperty"), properties (INSTANCE_PROPERTIES)
{
	this->locale = _locale;
	
	// TODO: call other constructor?
	this->value = _value;
	this->options = _options;

	setPropertyManager (&properties);
}


XMPProperty::XMPProperty(String _schemaNS, String _propPath, Variant _value, int _options) : XMPBase ("XMPProperty"), properties (INSTANCE_PROPERTIES)
{
	this->schemaNS = _schemaNS;
	this->propPath = _propPath;

	// TODO: call other constructor?
	this->value = _value;
	this->options = _options;

	setPropertyManager (&properties);
}



//////////////////////////////////////////////////////////////////////////////////////


scerror_t XMPProperty::get (int32_t id, ScCore::Variant& result, ScCore::Error* errs) const
{
	scerror_t errorCode = ScCore::kErrOK;
	switch (id)
	{
		case kNamespace:	result = schemaNS; break;
		case kPath:			result = propPath; break;
		case kLocale:		result = locale; break;
		case kValue:		result = value; break;
		case kOptions:		result.setInteger(options); break;

		default: errorCode = LiveObject::get (id, result, errs);
	}
	return setError (errorCode, id, errs);
}


/**
@return Returns only the value of the property, converted to a string.
*/
void XMPProperty::toString (ScCore::String& result) const
{
	result = value.toString();
}
