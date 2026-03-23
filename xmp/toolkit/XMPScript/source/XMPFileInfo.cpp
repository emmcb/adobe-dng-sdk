// =================================================================================================
// Copyright 2006-2007 Adobe
// All Rights Reserved.
//
// NOTICE:	Adobe permits you to use, modify, and distribute this file in accordance with the terms
// of the Adobe license agreement accompanying it. If you have received this file from a source other 
// than Adobe, then your use, modification, or distribution of it requires the prior written permission
// of Adobe.
// =================================================================================================

#include "XMPFileInfo.h"


// This section defines the properties and uses the LivePropertyManager
// to do the usual handling of the properties.

#define	kFilePath		100
#define	kOpenFlags		101
#define kFormat			102
#define kHandlerFlags	103


static const ScCore::LivePropertyInfo INSTANCE_PROPERTIES[] = {
	{ "filePath",		kFilePath,		ROPROP, "string" },
	{ "openFlags",		kOpenFlags,		ROPROP, "number" },
	{ "format",			kFormat,		ROPROP, "number" },
	{ "handlerFlags",	kHandlerFlags,	ROPROP, "number" },
	{ null }
};


XMPFileInfo::XMPFileInfo(std::string _filePath, XMP_OptionBits _openFlags, int _format, XMP_OptionBits _handlerFlags) :
	XMPBase ("XMPFileInfo"), properties (INSTANCE_PROPERTIES)
{
	this->filePath = _filePath;
	this->openFlags = _openFlags;
	this->format = _format;
	this->handlerFlags = _handlerFlags;
		
	setPropertyManager (&properties);
}


scerror_t XMPFileInfo::get (int32_t id, ScCore::Variant& result, ScCore::Error* errs) const
{
	scerror_t errorCode = ScCore::kErrOK;
	switch (id)
	{
		case kFilePath:
			result.setString(filePath.c_str());
			break;

		case kOpenFlags:
			result.setInteger((uint32_t)openFlags);
			break;

		case kFormat:
			result.setInteger(format);
			break;

		case kHandlerFlags:
			result.setInteger((uint32_t)handlerFlags);
			break;

		default: errorCode = LiveObject::get (id, result, errs);
	}
	return setError (errorCode, id, errs);
}


void XMPFileInfo::toString (ScCore::String& result) const
{
	result = filePath.c_str(); 
}
