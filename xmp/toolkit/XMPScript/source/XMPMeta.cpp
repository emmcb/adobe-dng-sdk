// =================================================================================================
// Copyright 2006-2007 Adobe
// All Rights Reserved.
//
// NOTICE:	Adobe permits you to use, modify, and distribute this file in accordance with the terms
// of the Adobe license agreement accompanying it. If you have received this file from a source other 
// than Adobe, then your use, modification, or distribution of it requires the prior written permission
// of Adobe.
// =================================================================================================

#include "XMPMeta.h"
#include "XMPIterator.h"
#include "XMPDateTime.h"

using namespace ScCore;
using namespace std;


// This section defines the properties and uses the LivePropertyManager
// to do the usual handling of the properties.

#define kGetProperty			100
#define kGetArrayItem			101
#define kGetStructField			102
#define kGetQualifier			103
#define kSetProperty			104
#define kSetArrayItem			105
#define kInsertArrayItem		106
#define kAppendArrayItem		107
#define kCountArrayItems		108
#define kSetStructField			109
#define kSetQualifier			110
#define kDeleteProperty			111
#define kDeleteArrayItem		112
#define kDeleteStructField		113
#define kDeleteQualifier		114
#define kDoesPropertyExist		115
#define kDoesArrayItemExist		116
#define kDoesStructFieldExist	117
#define kDoesQualifierExist		118
#define kGetLocalizedText		119
#define kSetLocalizedText		120
#define kIterator				140
#define kSerialize				150
#define kSerializeToArray		151
#define kDumpObject				152
#define kSort					153


static const ScCore::LivePropertyInfo INSTANCE_PROPERTIES[] = {
	{ "getProperty",			kGetProperty,			METHOD, "XMPProperty,schemaNS:string,propName:string" },
	{ "getArrayItem",			kGetArrayItem,			METHOD, "XMPProperty,schemaNS:string,arrayName:string,itemIndex:number" },
	{ "getStructField",			kGetStructField,		METHOD, "XMPProperty,schemaNS:string,structName:string,fieldNS:string,fieldName:string" },
	{ "getQualifier",			kGetQualifier,			METHOD, "XMPProperty,schemaNS:string,propName:string,qualNS:string,qualName:string" },
	{ "setProperty",			kSetProperty,			METHOD, "schemaNS:string,propName:string,propValue:string,setOptions:number" },
	{ "setArrayItem",			kSetArrayItem,			METHOD, "schemaNS:string,arrayName:string,itemIndex:number,itemValue:string,itemOptions:number" },
	{ "insertArrayItem",		kInsertArrayItem,		METHOD, "schemaNS:string,arrayName:string,itemIndex:number,itemValue:string,itemOptions:number" },
	{ "appendArrayItem",		kAppendArrayItem,		METHOD, "schemaNS:string,arrayName:string,itemValue:string,itemOptions:number,arrayOptions:number" },
	{ "countArrayItems",		kCountArrayItems,		METHOD, "number,schemaNS:string,arrayName:string" },
	{ "setStructField",			kSetStructField,		METHOD, "schemaNS:string,structName:string,fieldNS:string,fieldName:string,fieldValue:string,setOptions:number" },
	{ "setQualifier",			kSetQualifier,			METHOD, "schemaNS:string,propName:string,qualNS:string,qualName:string,qualValue:string,setOptions:number" },
	{ "deleteProperty",			kDeleteProperty,		METHOD, "schemaNS:string,propName:string"},
	{ "deleteArrayItem",		kDeleteArrayItem,		METHOD, "schemaNS:string,arrayName:string,arrayIndex:number"},
	{ "deleteStructField",		kDeleteStructField,		METHOD, "schemaNS:string,structName:string,fieldNS:string,fieldName:string"},
	{ "deleteQualifier",		kDeleteQualifier,		METHOD, "schemaNS:string,propName:string,qualNS:string,qualName:string"},
	{ "doesPropertyExist",		kDoesPropertyExist,		METHOD, "boolean,schemaNS:string,propName:string"},
	{ "doesArrayItemExist",		kDoesArrayItemExist,	METHOD, "boolean,schemaNS:string,arrayName:string,itemIndex:number"},
	{ "doesStructFieldExist",	kDoesStructFieldExist,	METHOD, "boolean,schemaNS:string,structName:string,fieldNS:string,fieldName:string"},
	{ "doesQualifierExist",		kDoesQualifierExist,	METHOD, "boolean,schemaNS:string,propName:string,qualNS:string,qualName:string"},
	{ "getLocalizedText",		kGetLocalizedText,		METHOD,	"XMPProperty,schemaNS:string,altTextName:string,genericLang:string,specificLang:string"},
	{ "setLocalizedText",		kSetLocalizedText,		METHOD,	"XMPProperty,schemaNS:string,altTextName:string,genericLang:string,specificLang:string,propValue:string,setOptions:number"},
	{ "iterator",				kIterator,				METHOD, "XMPIterator,iteratorOptions:number,schemaNS:string,propName:string" },
	{ "serialize",				kSerialize,				METHOD, "string" },
	{ "serializeToArray",		kSerializeToArray,		METHOD, "Array" },
	{ "dumpObject",				kDumpObject,			METHOD, "string" },
	{ "sort",					kSort,					METHOD, "" },
	{ null }
};



//////////////////////////////////////////////////////////////////////////////////////

XMPMeta::XMPMeta() : XMPBase ("XMPMeta"), properties (INSTANCE_PROPERTIES)
{
	setPropertyManager (&properties);
}


XMPMeta::XMPMeta(SXMPMeta _meta) : XMPBase ("XMPMeta"), properties (INSTANCE_PROPERTIES)
{
	this->meta = _meta;
	setPropertyManager (&properties);
}


scerror_t XMPMeta::initialize (const Array& args, Error * errors)
{
	scerror_t errorCode = kErrOK;

	try
	{
		if (args.length() == 0)
		{
			// create empty xmp object
			meta = SXMPMeta();
		}
		else if (args.length() > 1)
		{
			errorCode = kErrBadArgumentList;
			errors->push(errorCode, kErrMsgBadArgList + String("'new XMPMeta(xmpPacket: String/Array = null)'"), NULL, false);
		}
		else if (args[0].getType() == kString)
		{
			// create xmp object and parse it from a string
			string buffer = getStr(args[0]);
			meta = SXMPMeta(buffer.c_str(), (XMP_StringLen)buffer.length());
		}
		else if (args[0].getType() == kJSObject)
		{
			Array byteArray;
			args[0].getArray(byteArray);

			string buffer;
			buffer.reserve ( byteArray.length() );
			
			for (uint i = 0; i < byteArray.length(); i++)
			{
				buffer.append ( static_cast<char> (byteArray[i].getInteger()  &  0xff), 1 );
			}

			meta = SXMPMeta(buffer.c_str(), (XMP_StringLen)buffer.size());
		}
		else
		{
			// error: Parameter has wrong type.
			errorCode = kErrBadArgument;
			errors->push(errorCode, args[0]);
		}
	}
	catch ( XMP_Error & e )
	{
		errorCode = kErrXMPException;
		errors->push(errorCode, String(e.GetErrMsg()));
	}
	
	return errorCode;
}



//////////////////////////////////////////////////////////////////////////////////////


scerror_t XMPMeta::get (int32_t id, ScCore::Variant& result, ScCore::Error* errors) const
{
	scerror_t errorCode = LiveObject::get (id, result, errors);
	return setError (errorCode, id, errors);
}


scerror_t XMPMeta::put (int32_t id, const ScCore::Variant& value, ScCore::Error* errors)
{
	scerror_t errorCode = LiveObject::put (id, value, errors);
	return setError (errorCode, id, errors);
}


/**
 * Callback-function for the XMP dump...()-methods.
 */
XMP_Status dumpToString2( void * refCon, XMP_StringPtr outStr, XMP_StringLen outLen );
XMP_Status dumpToString2( void * refCon, XMP_StringPtr outStr, XMP_StringLen outLen )
{
	XMP_Status	status	= 0;
	std::string * dumpString = (std::string*) refCon; 
	
	try {
		dumpString->append( outStr, outLen );
	} catch ( ... ) {
		status = -1;
	}
	return status;
}


scerror_t XMPMeta::call (int32_t id, const ScCore::Array& args, ScCore::Variant& result, ScCore::Error* errors)
{
	scerror_t errorCode = ScCore::kErrOK;
	
	try
	{
		switch (id)
		{
			case kGetProperty:
				if (2 <= args.length()  &&  args.length() <= 3)
				{
					string schemaNS = getStr(args[0]);
					string propName = getStr(args[1]);
					// if the property is not found, an undefined is returned
					bool found = false;
					Variant propValue;
					XMP_OptionBits options = 0;

					string type;
					if (args.length() >= 3)
					{
						type = getStr(args[2]);
					}
					
					if (type.empty()  ||  type == "string")
					{
						// default case if type is not provided
						string stringValue;
						found = meta.GetProperty(
							schemaNS.c_str(),
							propName.c_str(),
							&stringValue,
							&options);
						if (found)
						{
							propValue = Variant(stringValue.c_str(), const_cast<Encoder*>(&Encoder::getUtf8()));
						}
					}
					else if (type == "integer")
					{
						XMP_Int32 intValue;
						found = meta.GetProperty_Int(
							schemaNS.c_str(),
							propName.c_str(),
							&intValue,
							&options);
						if (found)
						{
							propValue = Variant(static_cast<int32_t> (intValue));
						}
					}
					else if (type == "number")
					{
						double floatValue;
						found = meta.GetProperty_Float(
							schemaNS.c_str(),
							propName.c_str(),
							&floatValue,
							&options);
						if (found)
						{
							propValue = Variant(floatValue);
						}
					}
					else if (type == "boolean")
					{
						bool booleanValue;
						found = meta.GetProperty_Bool(
							schemaNS.c_str(),
							propName.c_str(),
							&booleanValue,
							&options);
						if (found)
						{
							propValue = Variant(booleanValue);
						}
					}
					else if (type == "xmpdate")
					{
						XMP_DateTime dateValue;
						found = meta.GetProperty_Date(
							schemaNS.c_str(),
							propName.c_str(),
							&dateValue,
							&options);
						if (found)
						{
							XMPDateTime * xmpDateTime = new XMPDateTime(dateValue);
							propValue = Variant(xmpDateTime);
							xmpDateTime->release();
						}
					}
					else
					{
						errorCode = kErrBadArgument;
						errors->push(errorCode, String("- The (optional) value type can only be one of 'string', 'integer', 'number', 'boolean' or 'xmpdate'!"));
					}

					if (found)
					{
						// only if the result is found, a prop is returned
						XMPProperty * prop = new XMPProperty(String(schemaNS.c_str()), propName.c_str(), propValue, options);
						result.setLiveObject(prop);
						prop->release();
					}
				}
				else
				{
					errorCode = kErrBadArgumentList;
					errors->push(errorCode, kErrMsgBadArgList + String("'getProperty(schemaNS: String, propName: String, valueType: String = null): XMPProperty'"), NULL, false);
				}
				break;

			case kGetArrayItem:
				if (args.length() == 3)
				{
					string schemaNS = getStr(args[0]);
					string arrayName = getStr(args[1]);
					int itemIndex = args[2].getInteger();
					string propValue;
					XMP_OptionBits options;
					
					meta.GetArrayItem(
						schemaNS.c_str(),
						arrayName.c_str(),
						itemIndex,
						&propValue,
						&options);

					XMPProperty * prop = new XMPProperty(String(schemaNS.c_str()), Variant(propValue.c_str(), const_cast<Encoder*>(&Encoder::getUtf8())), options);
					result.setLiveObject(prop);
					prop->release();
				}
				else
				{
					errorCode = kErrBadArgumentList;
					errors->push(errorCode, kErrMsgBadArgList + String("'getArrayItem(schemaNS: String, arrayName: String, itemIndex: Number): XMPProperty'"), NULL, false);
				}
				break;

			case kGetStructField:
				if (args.length() == 4)
				{
					string schemaNS = getStr(args[0]);
					string structName = getStr(args[1]);
					string fieldNS = getStr(args[2]);
					string fieldName = getStr(args[3]);
					string propValue;
					XMP_OptionBits options;
					
					meta.GetStructField(
						schemaNS.c_str(),
						structName.c_str(),
						fieldNS.c_str(),
						fieldName.c_str(),
						&propValue,
						&options);

					XMPProperty * prop = new XMPProperty(String(schemaNS.c_str()), Variant(propValue.c_str(), const_cast<Encoder*>(&Encoder::getUtf8())), options);
					result.setLiveObject(prop);
					prop->release();
				}
				else
				{
					errorCode = kErrBadArgumentList;
					errors->push(errorCode, kErrMsgBadArgList + String("'getStructField(schemaNS: String, structName: String, fieldNS: String, fieldName: String): XMPProperty'"), NULL, false);
				}
				break;

			case kGetQualifier:
				if (args.length() == 4)
				{
					string schemaNS = getStr(args[0]);
					string propName = getStr(args[1]);
					string qualNS = getStr(args[2]);
					string qualName = getStr(args[3]);
					string propValue;
					XMP_OptionBits options;
					
					meta.GetQualifier(
						schemaNS.c_str(),
						propName.c_str(),
						qualNS.c_str(),
						qualName.c_str(),
						&propValue,
						&options);

					XMPProperty * prop = new XMPProperty(String(schemaNS.c_str()), Variant(propValue.c_str(), const_cast<Encoder*>(&Encoder::getUtf8())), options);
					result.setLiveObject(prop);
					prop->release();
				}
				else
				{
					errorCode = kErrBadArgumentList;
					errors->push(errorCode, kErrMsgBadArgList + String("'getQualifier(schemaNS: String, structName: String, qualNS: String, qualName: String): XMPProperty'"), NULL, false);
				}
				break;

			case kSetProperty:
				if (args.length() >= 3  &&  args.length() <= 5)
				{
					string schemaNS = getStr(args[0]);
					string propName = getStr(args[1]);
					int options = args.length() >= 4 ? args[3].getInteger() : 0;	
					
					string type;
					if (args.length() >= 5  &&  args[4].isDefined())
					{
						type = getStr(args[4]);
					}
					
					if (type == "integer" || type == "number"  ||
						(type.empty()  &&  args[2].getType() == kNumber))
					{
						if (type == "number"  ||  !args[2].isInteger())
						{
							// double
							double floatValue = args[2].getDouble();
							
							meta.SetProperty_Float(
								schemaNS.c_str(),
								propName.c_str(),
								floatValue,
								options);
						}
						else
						{
							XMP_Int32 intValue = args[2].getInteger();
							
							meta.SetProperty_Int(
								schemaNS.c_str(),
								propName.c_str(),
								intValue,
								options);
						}
					}
					else if (type == "boolean"  ||
							 (type.empty()  &&  args[2].getType() == kBool))
					{
						bool booleanValue = args[2].getBool();
						
						meta.SetProperty_Bool(
							schemaNS.c_str(),
							propName.c_str(),
							booleanValue,
							options);
					}
					// XMPDateTime cannot be converted from string -> real type is evaluated
					else if (args[2].getType() == kLiveObject  &&
							 args[2].getLiveObject()->getClassName() == "XMPDateTime")
					{
						XMPDateTime * dateValue = static_cast<XMPDateTime *> (args[2].getLiveObject());
						
						meta.SetProperty_Date(
							schemaNS.c_str(),
							propName.c_str(),
							dateValue->getDateTime(),
							options);
					}
					else if (type == "string"  ||  type.empty())
					{
						// default case if type is not provided
						string stringValue = "";
						if (args[2].isDefined()  &&  !args[2].isNull())
						{
							stringValue = getStr(args[2]);
						}
						meta.SetProperty(
							schemaNS.c_str(),
							propName.c_str(),
							stringValue != "" ? stringValue.c_str() : null,
							options);
					}
					else
					{
						errorCode = kErrBadArgument;
						errors->push(errorCode, String("- The (optional) value type can only be one of 'string', 'integer', 'number', 'boolean' or 'xmpdate' used with a corresponding value type!"));
					}
				}
				else
				{
					errorCode = kErrBadArgumentList;
					errors->push(errorCode, kErrMsgBadArgList + String("'setProperty(schemaNS: String, propName: String, propValue: Object, setOptions: Number = 0, valueType: String = null)'"), NULL, false);
				}
				break;

			case kSetArrayItem:
				if (4 <= args.length()  &&  args.length() <= 5)
				{
					string schemaNS = getStr(args[0]);
					string arrayName = getStr(args[1]);
					int itemIndex = args[2].getInteger();
					string itemValue = ""; 
					if (args[3].isDefined()  &&  !args[3].isNull())
					{
						itemValue = getStr(args[3]);
					}
					int itemOptions = args.length() >= 5 ? args[4].getInteger() : 0;
										
					meta.SetArrayItem(
						schemaNS.c_str(),
						arrayName.c_str(),
						itemIndex,
						itemValue != "" ? itemValue.c_str() : null,
						itemOptions);
				}
				else
				{
					errorCode = kErrBadArgumentList;
					errors->push(errorCode, kErrMsgBadArgList + String("'setArrayItem(schemaNS: String, arrayName: String, itemIndex: Number, itemValue: String, itemOptions: Number = 0)'"), NULL, false);
				}
				break;

			case kInsertArrayItem:
				// TOTRACK: insertArrayItem is wrapped now by setArrayItem(), but will be added to C++, too
				if (4 <= args.length()  &&  args.length() <= 5)
				{
					string schemaNS = getStr(args[0]);
					string arrayName = getStr(args[1]);
					int itemIndex = args[2].getInteger();
					string itemValue = ""; 
					if (args[3].isDefined()  &&  !args[3].isNull())
					{
						itemValue = getStr(args[3]);
					}
					// the kXMP_insertBeforeItem flag makes the call an "insert" instead of "replace".
					int itemOptions = args.length() >= 5 ?
						args[4].getInteger() | kXMP_InsertBeforeItem :
						kXMP_InsertBeforeItem;
					
					meta.SetArrayItem(
						schemaNS.c_str(),
						arrayName.c_str(),
						itemIndex,
						itemValue != "" ? itemValue.c_str() : null,
						itemOptions);
				}
				else
				{
					errorCode = kErrBadArgumentList;
					errors->push(errorCode, kErrMsgBadArgList + String("'insertArrayItem(schemaNS: String, arrayName: String, itemIndex: Number, itemValue: String, itemOptions: Number = 0)'"), NULL, false);
				}
				break;

			case kAppendArrayItem:
				// TOTRACK: arrayOptions could be replaced with createArraySeq, 
				//      createArrayAlt and createArrayBag
				if (3 <= args.length()  &&  args.length() <= 5)
				{
					string schemaNS = getStr(args[0]);
					string arrayName = getStr(args[1]);
					string itemValue = ""; 
					if (args[2].isDefined()  &&  !args[2].isNull())
					{
						itemValue = getStr(args[2]);
					}
					int itemOptions = args.length() >= 4 ? args[3].getInteger() : 0;
					int arrayOptions = args.length() >= 5 ? args[4].getInteger() : 0;
					
					meta.AppendArrayItem(
						schemaNS.c_str(),
						arrayName.c_str(),
						arrayOptions,
						itemValue != "" ? itemValue.c_str() : null,
						itemOptions);
				}
				else
				{
					errorCode = kErrBadArgumentList;
					errors->push(errorCode, kErrMsgBadArgList + String("'appendArrayItem(schemaNS: String, arrayName: String, itemValue: String, itemOptions: Number = 0, arrayOptions: Number = 0)'"), NULL, false);
				}
				break;

			case kCountArrayItems:
				if (args.length() == 2)
				{
					string schemaNS = getStr(args[0]);
					string arrayName = getStr(args[1]);
					
					XMP_Index count = meta.CountArrayItems(
						schemaNS.c_str(),
						arrayName.c_str());

					result.setInteger(count);
				}
				else
				{
					errorCode = kErrBadArgumentList;
					errors->push(errorCode, kErrMsgBadArgList + String("'countArrayItems(schemaNS: String, arrayName: String): Number'"), NULL, false);
				}
				break;

			case kSetStructField:
				if (5 <= args.length()  &&  args.length() <= 6)
				{
					string schemaNS = getStr(args[0]);
					string structName = getStr(args[1]);
					string fieldNS = getStr(args[2]);
					string fieldName = getStr(args[3]);
					string fieldValue = getStr(args[4]);
					int options = args.length() == 6 ? args[5].getInteger() : 0;
					
					meta.SetStructField(
						schemaNS.c_str(),
						structName.c_str(),
						fieldNS.c_str(),
						fieldName.c_str(),
						fieldValue.c_str(),
						options);
				}
				else
				{
					errorCode = kErrBadArgumentList;
					errors->push(errorCode, kErrMsgBadArgList + String("'setStructField(schemaNS: String, structName: String, fieldNS: String, fieldName: String, fieldValue: String, options: Number = 0)'"), NULL, false);
				}
				break;

			case kSetQualifier:
				if (5 <= args.length()  &&  args.length() <= 6)
				{
					string schemaNS = getStr(args[0]);
					string propName = getStr(args[1]);
					string qualNS = getStr(args[2]);
					string qualName = getStr(args[3]);
					string qualValue = getStr(args[4]);
					int options = args.length() == 6 ? args[5].getInteger() : 0;
					
					meta.SetQualifier(
						schemaNS.c_str(),
						propName.c_str(),
						qualNS.c_str(),
						qualName.c_str(),
						qualValue.c_str(),
						options);
				}
				else
				{
					errorCode = kErrBadArgumentList;
					errors->push(errorCode, kErrMsgBadArgList + String("'setQualifier(schemaNS: String, propName: String, qualNS: String, qualName: String, qualValue: String, options: Number)'"), NULL, false);
				}
				break;

			case kDeleteProperty:
				if (args.length() == 2)
				{
					string schemaNS = getStr(args[0]);
					string propName = getStr(args[1]);
					
					meta.DeleteProperty(
						schemaNS.c_str(),
						propName.c_str());
				}
				else
				{
					errorCode = kErrBadArgumentList;
					errors->push(errorCode, kErrMsgBadArgList + String("'deleteStructField(schemaNS: String, propName: String)'"), NULL, false);
				}
				break;

			case kDeleteArrayItem:
				if (args.length() == 3)
				{
					string schemaNS = getStr(args[0]);
					string arrayName = getStr(args[1]);
					int itemIndex = args[2].getInteger();
					
					meta.DeleteArrayItem(
						schemaNS.c_str(),
						arrayName.c_str(),
						itemIndex);
				}
				else
				{
					errorCode = kErrBadArgumentList;
					errors->push(errorCode, kErrMsgBadArgList + String("'deleteArrayItem(schemaNS: String, arrayName: String, itemIndex: Number)'"), NULL, false);
				}
				break;

			case kDeleteStructField:
				if (args.length() == 4)
				{
					string schemaNS = getStr(args[0]);
					string structName = getStr(args[1]);
					string fieldNS = getStr(args[2]);
					string fieldName = getStr(args[3]);
					
					meta.DeleteStructField(
						schemaNS.c_str(),
						structName.c_str(),
						fieldNS.c_str(),
						fieldName.c_str());
				}
				else
				{
					errorCode = kErrBadArgumentList;
					errors->push(errorCode, kErrMsgBadArgList + String("'deleteStructField(schemaNS: String, structName: String, fieldNS: String, fieldName: String)'"), NULL, false);
				}
				break;

			case kDeleteQualifier:
				if (args.length() == 4)
				{
					string schemaNS = getStr(args[0]);
					string structName = getStr(args[1]);
					string qualNS = getStr(args[2]);
					string qualName = getStr(args[3]);
					
					meta.DeleteQualifier(
						schemaNS.c_str(),
						structName.c_str(),
						qualNS.c_str(),
						qualName.c_str());
				}
				else
				{
					errorCode = kErrBadArgumentList;
					errors->push(errorCode, kErrMsgBadArgList + String("'deleteQualifier(schemaNS: String, structName: String, qualNS: String, qualName: String)'"), NULL, false);
				}
				break;

			case kDoesPropertyExist:
				if (args.length() == 2)
				{
					string schemaNS = getStr(args[0]);
					string propName = getStr(args[1]);
					
					bool exists = meta.DoesPropertyExist(
						schemaNS.c_str(),
						propName.c_str());

					result.setBool(exists);
				}
				else
				{
					errorCode = kErrBadArgumentList;
					errors->push(errorCode, kErrMsgBadArgList + String("'doesPropertyExist(schemaNS: String, propName: String): Boolean'"), NULL, false);
				}
				break;

			case kDoesArrayItemExist:
				if (args.length() == 3)
				{
					string schemaNS = getStr(args[0]);
					string arrayName = getStr(args[1]);
					int itemIndex = args[2].getInteger();
					
					bool exists = meta.DoesArrayItemExist(
						schemaNS.c_str(),
						arrayName.c_str(),
						itemIndex);

					result.setBool(exists);
				}
				else
				{
					errorCode = kErrBadArgumentList;
					errors->push(errorCode, kErrMsgBadArgList + String("'doesArrayItemExist(schemaNS: String, arrayName: String, itemIndex: Number): Boolean'"), NULL, false);
				}
				break;
		
			case kDoesStructFieldExist:
				if (args.length() == 4)
				{
					string schemaNS = getStr(args[0]);
					string structName = getStr(args[1]);
					string fieldNS = getStr(args[2]);
					string fieldName = getStr(args[3]);
					
					bool exists = meta.DoesStructFieldExist(
						schemaNS.c_str(),
						structName.c_str(),
						fieldNS.c_str(),
						fieldName.c_str());

					result.setBool(exists);
				}
				else
				{
					errorCode = kErrBadArgumentList;
					errors->push(errorCode, kErrMsgBadArgList + String("'doesStructFieldExist(schemaNS: String, structName: String, fieldNS: String, fieldName: String): Boolean'"), NULL, false);
				}
				break;

			case kDoesQualifierExist:
				if (args.length() == 4)
				{
					string schemaNS = getStr(args[0]);
					string structName = getStr(args[1]);
					string qualNS = getStr(args[2]);
					string qualName = getStr(args[3]);
					
					bool exists = meta.DoesQualifierExist(
						schemaNS.c_str(),
						structName.c_str(),
						qualNS.c_str(),
						qualName.c_str());

					result.setBool(exists);
				}
				else
				{
					errorCode = kErrBadArgumentList;
					errors->push(errorCode, kErrMsgBadArgList + String("'doesQualifierExist(schemaNS: String, arrayName: String, qualNS: String, qualName: String): Boolean'"), NULL, false);
				}
				break;

			case kGetLocalizedText:
				if (args.length() == 4)
				{
					string schemaNS = getStr(args[0]);
					string altTextName = getStr(args[1]);
					string genericLang = getStr(args[2]);
					string specificLang = getStr(args[3]);
					string actualLang;
					string propValue;
					XMP_OptionBits options;
					
					if ( meta.GetLocalizedText(
						schemaNS.c_str(),
						altTextName.c_str(),
						genericLang.c_str(),
						specificLang.c_str(),
						&actualLang,
						&propValue,
						&options) ) 
					{

						XMPProperty * prop = new XMPProperty(String(actualLang.c_str()), Variant(propValue.c_str(), const_cast<Encoder*>(&Encoder::getUtf8())), options);
						result.setLiveObject(prop);
						prop->release();

					}
					else
					{
						errorCode = kErrBadArgumentList;
						errors->push(errorCode, kErrMsgBadArgList + String("'getLocalizedText(schemaNS: String, altTextName: String, genericLang: String, specificLang: String): XMPProperty'"), NULL, false);
					}
				}
				else
				{
					errorCode = kErrBadArgumentList;
					errors->push(errorCode, kErrMsgBadArgList + String("'getLocalizedText(schemaNS: String, altTextName: String, genericLang: String, specificLang: String): XMPProperty'"), NULL, false);
				}
				break;

			case kSetLocalizedText:
				if (5 <= args.length()  &&  args.length() <= 6)
				{
					string schemaNS = getStr(args[0]);
					string altTextName = getStr(args[1]);
					string genericLang = getStr(args[2]);
					string specificLang = getStr(args[3]);
					string propValue = getStr(args[4]);
					XMP_OptionBits options = args.length() == 6 ? args[5].getInteger() : 0;
					
					meta.SetLocalizedText(
						schemaNS.c_str(),
						altTextName.c_str(),
						genericLang.c_str(),
						specificLang.c_str(),
						propValue.c_str(),
						options);
				}
				else
				{
					errorCode = kErrBadArgumentList;
					errors->push(errorCode, kErrMsgBadArgList + String("'XMPMetaObj.setLocalizedText(schemaNS: String, altTextName: String, genericLang: String, specificLang: String, itemValue: String, setOptions: Number = 0)'"), NULL, false);
				}
				break;

			case kIterator:
				if (args.length() <= 3)
				{
					XMP_OptionBits options = args.length() >= 1 ? args[0].getInteger() : 0;
					string schemaNS = "";
					string propName = "";
					if (args.length() >= 2  &&  !args[1].isNull())
					{
						schemaNS = getStr(args[1]);
					}
					if (args.length() >= 3  &&  !args[2].isNull())
					{
						propName = getStr(args[2]);
					}

					XMPIterator * iterator = new XMPIterator(meta, options, String(schemaNS.c_str()), String(propName.c_str()));
					result.setLiveObject(iterator);
					iterator->release();
				}
				else
				{
					errorCode = kErrBadArgumentList;
					errors->push(errorCode, kErrMsgBadArgList + String("'iterator(options: Number = 0, schemaNS: String = null, propName: String = null): XMPIterator'"), NULL, false);
				}
				break;

			case kSerialize:
				if (args.length() <= 5)
				{
					int options =  args.length() >= 1 ? args[0].getInteger() : 0;
					int padding = args.length() >= 2 ? args[1].getInteger() : 0;	
					string newline;
					if (args.length() >= 3  &&  args[2].isDefined())
					{
						newline = getStr(args[2]);
					}
					string indent;
					if (args.length() >= 4  &&  args[3].isDefined())
					{
						indent = getStr(args[3]);
					}
					int baseIndent = args.length() >= 5 ? args[4].getInteger() : 0;
					
					string serializedXMP = "";
					
					meta.SerializeToBuffer(&serializedXMP, options, padding, newline.c_str(), indent.c_str(), baseIndent);
						
					String s (serializedXMP.c_str(), &Encoder::getUtf8());
					result.setString (s);
				}
				else
				{
					errorCode = kErrBadArgumentList;
					errors->push(errorCode, kErrMsgBadArgList + String("'serialize(options: Number = 0, padding: Number = 2048, indent: String = \"  \", newline: String = \"\\n\", baseIndent: Number = 0): String'"), NULL, false);
				}
				break;

			case kSerializeToArray:
				if (args.length() <= 5)
				{
					int options =  args.length() >= 1 ? args[0].getInteger() : 0;
					int padding = args.length() >= 2 ? args[1].getInteger() : 0;	
					string newline;
					if (args.length() >= 3)
					{
						newline = getStr(args[2]);
					}
					string indent;
					if (args.length() >= 4)
					{
						indent = getStr(args[3]);
					}
					int baseIndent = args.length() >= 5 ? args[4].getInteger() : 0;

					string serializedXMP = "";
					
					meta.SerializeToBuffer(&serializedXMP, options, padding, newline.c_str(), indent.c_str(), baseIndent);
						
					Array byteArray;
					byteArray.setLength((uint32_t)serializedXMP.length());
					for (uint i = 0; i < serializedXMP.length(); i++)
					{
						byteArray[i] = serializedXMP[i];
					}
					result.setArray (byteArray);
					break;
				}
				else
				{
					errorCode = kErrBadArgumentList;
					errors->push(errorCode, kErrMsgBadArgList + String("'serializeToArray(options: Number = 0, padding: Number = 2048, indent: String = \"  \", newline: String = \"\\n\", baseIndent: Number = 0): Array'"), NULL, false);
				}
				break;

			case kDumpObject:
				if (args.length() == 0)
				{
					string str;
					meta.DumpObject( dumpToString2, &str );
					result.setString( str.c_str() );
				}
				else
				{
					errorCode = kErrBadArgumentList;
					errors->push(errorCode, String(kErrMsgNoArgsExpected), NULL, false);
				}
				break;

			case kSort:
				if (args.length() == 0)
				{
					meta.Sort();
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


SXMPMeta XMPMeta::getXMPMeta()
{
	return meta;
}


void XMPMeta::setName(ScCore::String _name) const
{
    //Fixing warning of shadowing the variable. this->name = _name should be done and const should be removed. Not changing the public API for now.
	//_name = _name;
}
