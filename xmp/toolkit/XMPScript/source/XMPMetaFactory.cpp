// =================================================================================================
// Copyright 2006-2007 Adobe
// All Rights Reserved.
//
// NOTICE:	Adobe permits you to use, modify, and distribute this file in accordance with the terms
// of the Adobe license agreement accompanying it. If you have received this file from a source other 
// than Adobe, then your use, modification, or distribution of it requires the prior written permission
// of Adobe.
// =================================================================================================

#include "XMPMetaFactory.h"
#include "XMPMeta.h"

using namespace ScCore;
using namespace std;


// This section defines the properties and uses the LivePropertyManager
// to do the usual handling of the properties.

#define	kVersion				100

#define	kRegisterNamespace		200
#define kGetNamespacePrefix		201
#define kGetNamespaceURI		202
#define kDeleteNamespace		203

#define kDumpNamespaces			250


static const LivePropertyInfo STATIC_PROPERTIES[] = {
	{ "version",			kVersion,				ROPROP, "string" },

	{ "registerNamespace",	kRegisterNamespace,		METHOD, "string,namespace:string,prefix:string" },
	{ "getNamespacePrefix",	kGetNamespacePrefix,	METHOD, "string,namespace:string" },
	{ "getNamespaceURI",	kGetNamespaceURI,		METHOD, "string,namespace:string" },
	{ "deleteNamespace",	kDeleteNamespace,		METHOD, "namespace:string" },
	{ "dumpNamespaces",		kDumpNamespaces,		METHOD, "string" },
//	{ "dumpPropertyTraits",	kDumpPropertyTraits,	METHOD, "string" },
	{ null }
};


//////////////////////////////////////////////////////////////////////////////////////


/**
Initializes the native XMP Toolkit and adds a new JavaScript Error for XMP
*/
XMPMetaFactory::XMPMetaFactory() : XMPBase("XMPMeta"), properties (STATIC_PROPERTIES)
{
	setPropertyManager (&properties);
	// TOTRACK memory mgmt, find a better place for init
	
	try
	{
		// Adds a new JavaScript Error for XMP
		Error::defineError(kErrXMPException, "XMP Exception: %1");
		SXMPMeta::Initialize();
	}
	catch ( XMP_Error & e )
	{
		(void)e;
		// TOTRACK where put the error?
	}
}


/** Support for the JavaScript "new" operator */
scerror_t XMPMetaFactory::clone (LiveObject** newObj, bool) const
{
	*newObj = new XMPMeta();
	return kErrOK;
}


//////////////////////////////////////////////////////////////////////////////////////




scerror_t XMPMetaFactory::get (int32_t id, ScCore::Variant& result, ScCore::Error* errs) const
{
	scerror_t err = ScCore::kErrOK;
	switch (id)
	{
		// global properties
		case kVersion:				
			XMP_VersionInfo version;
			SXMPMeta::GetVersionInfo ( &version );
			result.setString( version.message );
			break;
		default:
			err = LiveObject::get (id, result, errs);
	}
	return setError (err, id, errs);
}



scerror_t XMPMetaFactory::put (int32_t id, const ScCore::Variant& value, ScCore::Error* errors)
{
	scerror_t errorCode = LiveObject::put (id, value, errors);
	return setError (errorCode, id, errors);
}


/**
 * Callback-function for the XMP dump...()-methods.
 */
XMP_Status dumpToString( void * refCon, XMP_StringPtr outStr, XMP_StringLen outLen );
XMP_Status dumpToString( void * refCon, XMP_StringPtr outStr, XMP_StringLen outLen )
{
	XMP_Status	status	= 0;
	std::string * dumpString = (std::string*) refCon; 
	
	try {
		dumpString->append( outStr, outLen );
	} catch ( ... ) {
		status = -1;
	}
	return status;
}	// DumpToString



scerror_t XMPMetaFactory::call (int32_t id, const ScCore::Array& args, ScCore::Variant& result, ScCore::Error * errors)
{
	scerror_t errorCode = ScCore::kErrOK;
	
	try
	{
		switch (id)
		{
			case kRegisterNamespace:
				if (args.length() == 2)
				{
					string regPrefix = "";
					string ns = getStr(args[0]);
					string prefix = getStr(args[1]);
					SXMPMeta::RegisterNamespace(
						ns.c_str(),
						prefix.c_str(),
						&regPrefix);
					result.setString (regPrefix.c_str());
				}
				else
				{
					errorCode = kErrBadArgumentList;
					errors->push(errorCode, kErrMsgBadArgList + String("'registerNamespaces(namespaceURI: String, suggestedPrefix: String): String'"), NULL, false);
				}
				break;

			case kGetNamespacePrefix:
				if (args.length() == 1)
				{
					string prefix = "";
					string ns = getStr(args[0]);
					
					SXMPMeta::GetNamespacePrefix(
						ns.c_str(),
						&prefix);
					result.setString (prefix.c_str());
				}
				else
				{
					errorCode = kErrBadArgumentList;
					errors->push(errorCode, kErrMsgBadArgList + String("'getNamespacePrefix(namespaceURI: String): String'"), NULL, false);
				}
				break;

			case kGetNamespaceURI:
				if (args.length() == 1)
				{
					string ns = "";
					string prefix = getStr(args[0]);
					
					SXMPMeta::GetNamespaceURI(
						prefix.c_str(),
						&ns);
					result.setString (ns.c_str());
				}
				else
				{
					errorCode = kErrBadArgumentList;
					errors->push(errorCode, kErrMsgBadArgList + String("'getNamespaceURI(namespacePrefix: String): String'"), NULL, false);
				}
				break;

			case kDeleteNamespace:
				if (args.length() == 1)
				{
					// TODO check types of args?
					string ns = getStr(args[0]);
					
					SXMPMeta::DeleteNamespace(ns.c_str());
				}
				else
				{
					errorCode = kErrBadArgumentList;
					errors->push(errorCode, kErrMsgBadArgList + String("'deleteNamespace(namespaceURI: String)'"), NULL, false);
				}
				break;

			case kDumpNamespaces:
				if (args.length() == 0)
				{
					string str;
					SXMPMeta::DumpNamespaces ( dumpToString, &str );
					result.setString( str.c_str() );
				}
				else
				{
					errorCode = kErrBadArgumentList;
					errors->push(errorCode, kErrMsgNoArgsExpected, NULL, false);
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
Terminates the native XMP Toolkit
*/
XMPMetaFactory::~XMPMetaFactory()
{
	try
	{
		SXMPMeta::Terminate();
	}
	catch ( XMP_Error & e )
	{
		(void)e;
		// TOTRACK where put the error?
	}
}
