// =================================================================================================
// Copyright 2006-2007 Adobe
// All Rights Reserved.
//
// NOTICE:	Adobe permits you to use, modify, and distribute this file in accordance with the terms
// of the Adobe license agreement accompanying it. If you have received this file from a source other 
// than Adobe, then your use, modification, or distribution of it requires the prior written permission
// of Adobe.
// =================================================================================================

#include "XMPAliasInfo.h"

using namespace std;


// This section defines the properties and uses the LivePropertyManager
// to do the usual handling of the properties.

#define	kNamespace		100
#define	kName			101
#define kArrayForm		102


static const ScCore::LivePropertyInfo INSTANCE_PROPERTIES[] = {
	{ "namespace",	kNamespace,	ROPROP, "string" },
	{ "name",		kName,		ROPROP, "string" },
	{ "arrayForm",	kArrayForm,	ROPROP, "number" },
	{ null }
};


XMPAliasInfo::XMPAliasInfo(string propertyNS, string propertyName, int _arrayForm) : XMPBase ("XMPAliasInfo"), properties (INSTANCE_PROPERTIES)
{
	this->propNS = propertyNS;
	this->propName = propertyName;
	this->arrayForm = _arrayForm;

	setPropertyManager (&properties);
}


scerror_t XMPAliasInfo::get (int32_t id, ScCore::Variant& result, ScCore::Error* errs) const
{
	scerror_t errorCode = ScCore::kErrOK;
	switch (id)
	{
		case kNamespace:	result.setString(propNS.c_str()); break;
		case kName:			result.setString(propName.c_str()); break;
		case kArrayForm:	result.setInteger(arrayForm); break;

		default: errorCode = LiveObject::get (id, result, errs);
	}
	return setError (errorCode, id, errs);
}
