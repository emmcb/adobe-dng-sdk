// =================================================================================================
// Copyright 2006-2007 Adobe
// All Rights Reserved.
//
// NOTICE:	Adobe permits you to use, modify, and distribute this file in accordance with the terms
// of the Adobe license agreement accompanying it. If you have received this file from a source other 
// than Adobe, then your use, modification, or distribution of it requires the prior written permission
// of Adobe.
// =================================================================================================
   
#include "XMPBase.h"

using namespace ScCore;


XMPBase::XMPBase(const char * objectName) : LiveObject(objectName) 
{
	// EMPTY
}


// inherited methods

scerror_t XMPBase::getIDForName (const ScCore::String& name, int32_t& id, ScCore::InfoType& type, int32_t lang) const
{
	// add property name lookup for runtime-determined properties here.
	return LiveObject::getIDForName (name, id, type, lang);
}


ScCore::InfoType XMPBase::getType (int32_t id) const
{
	// add property type information for runtime-determined properties here.
	return LiveObject::getType (id);
}


scerror_t XMPBase::getNameForID (int32_t id, ScCore::String& name, int32_t lang) const
{
	// add property name reverse lookup for runtime-determined properties here.
	return LiveObject::getNameForID (id, name, lang);
}


// protected methods

std::string XMPBase::getStr(ScCore::String scStr)
{
	std::string std = scStr.encode(Encoder::get("UTF-8"));
	return std;
}


std::string XMPBase::getStr(ScCore::Variant scVariant)
{
	std::string std = scVariant.toString().encode(Encoder::get("UTF-8"));
	return std;
}


void XMPBase::toString (ScCore::String& result) const
{
	LiveObject::toString(result);
}
