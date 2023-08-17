// =================================================================================================
// Copyright 2006-2007 Adobe
// All Rights Reserved.
//
// NOTICE:	Adobe permits you to use, modify, and distribute this file in accordance with the terms
// of the Adobe license agreement accompanying it. If you have received this file from a source other 
// than Adobe, then your use, modification, or distribution of it requires the prior written permission
// of Adobe.
// =================================================================================================

#include "XMPFileFactory.h"
#include "XMPFile.h"

using namespace ScCore;
using namespace std;


// This section defines the properties and uses the LivePropertyManager
// to do the usual handling of the properties.

// properties
#define	kVersion				100

// methods
#define	kGetFormatInfo			200



static const LivePropertyInfo STATIC_PROPERTIES[] = {
	{ "version",			kVersion,				ROPROP, "string" },
	{ "getFormatInfo",		kGetFormatInfo,			METHOD, "number,format:number" },
	{ null }
};


/**
Initializes the native XMPFiles component and adds a new JavaScript Error for XMP
*/
XMPFileFactory::XMPFileFactory() : XMPBase("XMPFile"), properties (STATIC_PROPERTIES)
{
	setPropertyManager (&properties);
	
	try
	{
		// Adds a new JavaScript Error for XMP
		Error::defineError(kErrXMPException, "XMP Exception: %1");
		SXMPFiles::Initialize();
	}
	catch ( XMP_Error & e )
	{
		(void)e;
		// TODO where put the error?
	}
}


/** Support for the JavaScript "new" operator */
scerror_t XMPFileFactory::clone (LiveObject** newObj, bool) const
{
	*newObj = new XMPFile();
	return kErrOK;
}


scerror_t XMPFileFactory::get (int32_t id, ScCore::Variant& result, ScCore::Error* errs) const
{
	scerror_t err = ScCore::kErrOK;
	switch (id)
	{
		// global properties
		case kVersion:				
			XMP_VersionInfo version;
			SXMPFiles::GetVersionInfo ( &version );
			result.setString( version.message );
			break;
		default:
			err = LiveObject::get (id, result, errs);
	}
	return setError (err, id, errs);
}


scerror_t XMPFileFactory::call (int32_t id, const ScCore::Array& args, ScCore::Variant& result, ScCore::Error * errors)
{
	scerror_t errorCode = ScCore::kErrOK;
	
	try
	{
		switch (id)
		{
			case kGetFormatInfo:
				if (args.length() == 1)
				{
					int format = args[0].getInteger();
					XMP_OptionBits handlerFlags;

					SXMPFiles::GetFormatInfo(format, &handlerFlags);
					result.setInteger((uint32_t)handlerFlags);
				}
				else
				{
					errorCode = kErrBadArgumentList;
				}
				break;

			default:
				errorCode = LiveObject::call (id, args, result, errors);
		}
	}
	catch ( XMP_Error & e )
	{
		errorCode = kErrGeneral;
		if (errors)
		{
			String s ("XMP Exception: ");
			s += e.GetErrMsg();
			errors->push(errorCode, s);
		}
	}
	return setError (errorCode, id, errors);
}

/**
Terminates the native XMPFiles component
*/
XMPFileFactory::~XMPFileFactory()
{
	try
	{
		SXMPFiles::Terminate();
	}
	catch ( XMP_Error & e )
	{
		(void)e;
		// TODO where put the error?
	}
}
