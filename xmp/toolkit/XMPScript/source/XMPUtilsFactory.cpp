// =================================================================================================
// Copyright 2006-2007 Adobe
// All Rights Reserved.
//
// NOTICE:	Adobe permits you to use, modify, and distribute this file in accordance with the terms
// of the Adobe license agreement accompanying it. If you have received this file from a source other 
// than Adobe, then your use, modification, or distribution of it requires the prior written permission
// of Adobe.
// =================================================================================================

#include "XMPUtilsFactory.h"
#include "XMPMeta.h"

using namespace ScCore;
using namespace std;


// This section defines the properties and uses the LivePropertyManager
// to do the usual handling of the properties and methods.

#define	kComposeArrayItemPath	100
#define	kComposeStructFieldPath	101
#define	kComposeQualifierPath	102
#define	kComposeLangSelector	103
#define	kComposeFieldSelector	104
#define	kCatenateArrayItems		105
#define	kSeparateArrayItems		106
#define	kRemoveProperties		107
#define	kAppendProperties		108
#define	kDuplicateSubtree		109


static const LivePropertyInfo STATIC_PROPERTIES[] = {
	{ "composeArrayItemPath",	kComposeArrayItemPath,		METHOD, "string,schemaNS:string,arrayName:string,itemIndex:number" },
	{ "composeStructFieldPath",	kComposeStructFieldPath,	METHOD, "string,schemaNS:string,structName:string,fieldNS:string,fieldName:string" },
	{ "composeQualifierPath",	kComposeQualifierPath,		METHOD, "string,schemaNS:string,propName:string,qualNS:string,qualName:string" },
	{ "composeLangSelector",	kComposeLangSelector,		METHOD, "string,schemaNS:string,arrayName:string,langName:string" },
	{ "composeFieldSelector",	kComposeFieldSelector,		METHOD, "string,schemaNS:string,arrayName:string,fieldNS:string,fieldName:string" },
	
	{ "catenateArrayItems",		kCatenateArrayItems,		METHOD, "xmpObj:XMPMeta,schemaNS:string,arrayName:string,separator:string,quotes:string,arrayOptions:number" },
	{ "separateArrayItems",		kSeparateArrayItems,		METHOD, "xmpObj:XMPMeta,schemaNS:string,arrayName:string,arrayOptions:number,catedString:string" },

	{ "removeProperties",		kRemoveProperties,			METHOD, "xmpObj:XMPMeta,schemaNS:string,propName:string,options:number" },
	{ "appendProperties",		kAppendProperties,			METHOD, "source:XMPMeta,dest:XMPMeta,options:number" },
	{ "duplicateSubtree",		kDuplicateSubtree,			METHOD, "source:XMPMeta,dest:XMPMeta,sourceNS:string,sourceRoot:string,destNS:string,destRoot:string,options:number" },
	{ null }
};


/**
Adds a new JavaScript Error for XMP for the case  it has not been done yet.
*/
XMPUtilsFactory::XMPUtilsFactory() : XMPBase("XMPUtils"), properties (STATIC_PROPERTIES)
{
	setPropertyManager (&properties);
	Error::defineError(kErrXMPException, "XMP Exception: %1");
}


/** The overridden function prevents the new operator. */
scerror_t XMPUtilsFactory::clone (LiveObject** newObj, bool) const
{
	(void)newObj;
	return kErrObjectExpected;
}


scerror_t XMPUtilsFactory::call (int32_t id, const ScCore::Array& args, ScCore::Variant& result, ScCore::Error * errors)
{
	scerror_t errorCode = ScCore::kErrOK;
	
	try
	{
		switch (id)
		{
			case kComposeArrayItemPath:
				if (args.length() == 3)
				{
					string path;
					string schemaNS = getStr(args[0]);
					string arrayName = getStr(args[1]);
					int itemIndex = args[2].getInteger();

					SXMPUtils::ComposeArrayItemPath(schemaNS.c_str(), arrayName.c_str(), itemIndex, &path);

					result.setString (path.c_str());
				}
				else
				{
					errorCode = kErrBadArgumentList;
					errors->push(errorCode, kErrMsgBadArgList + String("'composeArrayItemPath(schemaNS: String, arrayName: String, itemIndex: Number): String'"), NULL, false);
				}
				break;

			case kComposeStructFieldPath:
				if (args.length() == 4)
				{
					string path;
					string schemaNS = getStr(args[0]);
					string structName = getStr(args[1]);
					string fieldNS = getStr(args[2]);
					string fieldName = getStr(args[3]);

					SXMPUtils::ComposeStructFieldPath(schemaNS.c_str(), structName.c_str(), fieldNS.c_str(), fieldName.c_str(), &path);

					result.setString (path.c_str());
				}
				else
				{
					errorCode = kErrBadArgumentList;
					errors->push(errorCode, kErrMsgBadArgList + String("'composeStructFieldPath(schemaNS: String, structName: String, fieldNS: String, fieldName: String): String'"), NULL, false);
				}
				break;

			case kComposeQualifierPath:
				if (args.length() == 4)
				{
					string path;
					string schemaNS = getStr(args[0]);
					string propName = getStr(args[1]);
					string qualNS = getStr(args[2]);
					string qualName = getStr(args[3]);

					SXMPUtils::ComposeQualifierPath(schemaNS.c_str(), propName.c_str(), qualNS.c_str(), qualName.c_str(), &path);

					result.setString (path.c_str());
				}
				else
				{
					errorCode = kErrBadArgumentList;
					errors->push(errorCode, kErrMsgBadArgList + String("'composeQualifierPath(schemaNS: String, propName: String, qualNS: String, qualName: String): String'"), NULL, false);
				}
				break;

			case kComposeLangSelector:
				if (args.length() == 3)
				{
					string path;
					string schemaNS = getStr(args[0]);
					string arrayName = getStr(args[1]);
					string locale = getStr(args[2]);

					SXMPUtils::ComposeLangSelector(schemaNS.c_str(), arrayName.c_str(), locale.c_str(), &path);

					result.setString (path.c_str());
				}
				else
				{
					errorCode = kErrBadArgumentList;
					errors->push(errorCode, kErrMsgBadArgList + String("'composeLangSelector(schemaNS: String, arrayName: String, locale: String): String'"), NULL, false);
				}
				break;

			case kComposeFieldSelector:
				if (args.length() == 5)
				{
					string path;
					string schemaNS = getStr(args[0]);
					string arrayName = getStr(args[1]);
					string fieldNS = getStr(args[2]);
					string fieldName = getStr(args[3]);
					string fieldValue = getStr(args[4]);

					SXMPUtils::ComposeFieldSelector(schemaNS.c_str(), arrayName.c_str(), fieldNS.c_str(), fieldName.c_str(), fieldValue.c_str(), &path);

					result.setString (path.c_str());
				}
				else
				{
					errorCode = kErrBadArgumentList;
					errors->push(errorCode, kErrMsgBadArgList + String("'composeFieldSelector(schemaNS: String, structName: String, fieldNS: String, fieldName: String, fieldValue: String): String'"), NULL, false);
				}
				break;


			case kCatenateArrayItems:
				if ((args.length() == 5  ||  args.length() == 6)  &&
					args[0].getType() == kLiveObject  &&
					args[0].getLiveObject()->getClassName() == "XMPMeta")
				{
					string catedString;
					XMPMeta * meta = static_cast<XMPMeta *> (args[0].getLiveObject());
					string schemaNS = getStr(args[1]);
					string arrayName = getStr(args[2]);
					string separator = getStr(args[3]);
					string quotes = getStr(args[4]);
					int arrayOptions = args.length() >= 6 ? args[5].getInteger() : 0;

					SXMPUtils::CatenateArrayItems(meta->getXMPMeta(), schemaNS.c_str(), arrayName.c_str(), separator.c_str(), quotes.c_str(), arrayOptions, &catedString);

					result.setString (catedString.c_str());
				}
				else
				{
					errorCode = kErrBadArgumentList;
					errors->push(errorCode, kErrMsgBadArgList + String("'catenateArrayItems(xmpObj: XMPMeta, schemaNS: String, arrayName: String, separator: String = '; ', quotes: String = '\"', options: Number = 0): String'"), NULL, false);
				}
				break;
			
			case kSeparateArrayItems:
				if (args.length() == 5  &&
					args[0].getType() == kLiveObject  &&
					args[0].getLiveObject()->getClassName() == "XMPMeta")
				{
					XMPMeta * meta = static_cast<XMPMeta *> (args[0].getLiveObject());
					SXMPMeta xmpMeta = meta->getXMPMeta();
					string schemaNS = getStr(args[1]);
					string arrayName = getStr(args[2]);
					int arrayOptions = args[3].getInteger();
					string catedString = getStr(args[4]);

					SXMPUtils::SeparateArrayItems(&xmpMeta, schemaNS.c_str(), arrayName.c_str(), arrayOptions, catedString.c_str());
				}
				else
				{
					errorCode = kErrBadArgumentList;
					errors->push(errorCode, kErrMsgBadArgList + String("'separateArrayItems(xmpObj: XMPMeta, schemaNS: String, arrayName: String, arrayOptions: Number, concatString: String)'"), NULL, false);
				}
				break;

			case kRemoveProperties:
				// TOTRACK make arguments 2 and 3 optional
				if ((args.length() == 3  ||  args.length() == 4)  &&
					args[0].getType() == kLiveObject  &&
					args[0].getLiveObject()->getClassName() == "XMPMeta")
				{
					XMPMeta * meta = static_cast<XMPMeta *> (args[0].getLiveObject());
					SXMPMeta xmpMeta = meta->getXMPMeta();
					string schemaNS = getStr(args[1]);
					string propName = getStr(args[2]);
					int options = args.length() == 4 ? args[3].getInteger() : 0;

					SXMPUtils::RemoveProperties(&xmpMeta, schemaNS.c_str(), propName.c_str(), options);
				}
				else
				{
					errorCode = kErrBadArgumentList;
					errors->push(errorCode, kErrMsgBadArgList + String("'removeProperties(xmpObj: XMPMeta, schemaNS: String, propName: String, options: Number = 0)'"), NULL, false);
				}
				break;

			case kAppendProperties:
				if ((args.length() == 2  ||  args.length() == 3)  &&
					args[0].getType() == kLiveObject  &&
					args[0].getLiveObject()->getClassName() == "XMPMeta"  &&
					args[1].getType() == kLiveObject  &&
					args[1].getLiveObject()->getClassName() == "XMPMeta")
				{
					XMPMeta * source = static_cast<XMPMeta *> (args[0].getLiveObject());
					SXMPMeta sourceMeta = source->getXMPMeta();
					XMPMeta * dest = static_cast<XMPMeta *> (args[1].getLiveObject());
					SXMPMeta destMeta = dest->getXMPMeta();
					int options = args.length() == 3 ? args[2].getInteger() : 0;

					SXMPUtils::AppendProperties(sourceMeta, &destMeta, options);
				}
				else
				{
					errorCode = kErrBadArgumentList;
					errors->push(errorCode, kErrMsgBadArgList + String("'appendProperties(source: XMPMeta, dest: XMPMeta, options: Number = 0)'"), NULL, false);
				}
				break;

			case kDuplicateSubtree:
				// TOTRACK: make arguments 3 - 6 optional
				if ((args.length() == 6  ||  args.length() == 7)  &&
					args[0].getType() == kLiveObject  &&
					args[0].getLiveObject()->getClassName() == "XMPMeta"  &&
					args[1].getType() == kLiveObject  &&
					args[1].getLiveObject()->getClassName() == "XMPMeta")
				{
					XMPMeta * source = static_cast<XMPMeta *> (args[0].getLiveObject());
					SXMPMeta sourceMeta = source->getXMPMeta();
					XMPMeta * dest = static_cast<XMPMeta *> (args[1].getLiveObject());
					SXMPMeta destMeta = dest->getXMPMeta();
					string sourceNS = getStr(args[2]);
					string sourceRoot = getStr(args[3]);
					string destNS = getStr(args[4]);
					string destRoot = getStr(args[5]);
					int options = args.length() == 7 ? args[6].getInteger() : 0;

					SXMPUtils::DuplicateSubtree(sourceMeta, &destMeta, sourceNS.c_str(), sourceRoot.c_str(), destNS.c_str(), destRoot.c_str(), options);
				}
				else
				{
					errorCode = kErrBadArgumentList;
					errors->push(errorCode, kErrMsgBadArgList + String("'duplicateSubtree(source: XMPMeta, dest: XMPMeta, sourceNS: String, sourceRoot: String, destNS: String, destRoot: String, options: Number = 0)'"), NULL, false);
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
