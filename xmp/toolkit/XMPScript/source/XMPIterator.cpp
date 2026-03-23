// =================================================================================================
// Copyright 2006-2007 Adobe
// All Rights Reserved.
//
// NOTICE:	Adobe permits you to use, modify, and distribute this file in accordance with the terms
// of the Adobe license agreement accompanying it. If you have received this file from a source other 
// than Adobe, then your use, modification, or distribution of it requires the prior written permission
// of Adobe.
// =================================================================================================


#include "XMPIterator.h"

using namespace ScCore;
using namespace std;


// This section defines the properties and uses the LivePropertyManager
// to do the usual handling of the properties.

#define kHasNext			100
#define kNext				101
#define kSkipSubtree		102
#define kSkipSiblings		103


static const ScCore::LivePropertyInfo INSTANCE_PROPERTIES[] = {
	{ "hasNext",		kHasNext,		METHOD, "boolean" },
	{ "next",			kNext,			METHOD, "XMPProperty" },
	{ "skipSubtree",	kSkipSubtree,	METHOD, "void" }, // TODO is void possible?
	{ "skipSiblings",	kSkipSiblings,	METHOD, "void" },
	{ null }
};


XMPIterator::XMPIterator(SXMPMeta meta, int options, String schemaNS, String propName) : XMPBase ("XMPIterator"), 
	properties (INSTANCE_PROPERTIES), iterator(meta, getStr(schemaNS).c_str(), getStr(propName).c_str(), options)
{
	setPropertyManager (&properties);
}


scerror_t XMPIterator::call (int32_t id, const ScCore::Array& args, ScCore::Variant& result, ScCore::Error* errors)
{
	scerror_t errorCode = ScCore::kErrOK;
	
	try
	{
		switch (id)
		{
			case kHasNext:
				if (args.length() == 0)
				{
					// TODO not implemented
				}
				else
				{
					errorCode = kErrBadArgumentList;
					errors->push(errorCode, String(kErrMsgNoArgsExpected), NULL, false);
				}
				break;

			case kNext:
				if (args.length() == 0)
				{
					string schemaNS;
					string propPath;
					string propValue;
					XMP_OptionBits options;

					bool hasNext = iterator.Next(
						&schemaNS,
						&propPath,
						&propValue,
						&options);

					if (hasNext)
					{
						XMPProperty * prop = new XMPProperty(String(schemaNS.c_str(), &Encoder::getUtf8()), String(propPath.c_str(), &Encoder::getUtf8()), Variant(propValue.c_str()), options);
						result.setLiveObject(prop);
						prop->release();
					}
				}
				else
				{
					errorCode = kErrBadArgumentList;
					errors->push(errorCode, String(kErrMsgNoArgsExpected), NULL, false);
				}
				break;

			case kSkipSubtree:
				if (args.length() == 0)
				{
					iterator.Skip(kXMP_IterSkipSubtree);
				}
				else
				{
					errorCode = kErrBadArgumentList;
					errors->push(errorCode, String(kErrMsgNoArgsExpected), NULL, false);
				}
				break;

			case kSkipSiblings:
				if (args.length() == 0)
				{
					iterator.Skip(kXMP_IterSkipSiblings);
				}
				else
				{
					errorCode = kErrBadArgumentList;
					errors->push(errorCode, String(kErrMsgNoArgsExpected), NULL, false);
				}
				break;

			default:
				errorCode = LiveObject::call (id, args, result, errors);
		}
	}
	catch ( XMP_Error & e )
	{
		errorCode = kErrXMPException;
		errors->push(errorCode, String(e.GetErrMsg()));
	}
	return setError (errorCode, id, errors);
}
