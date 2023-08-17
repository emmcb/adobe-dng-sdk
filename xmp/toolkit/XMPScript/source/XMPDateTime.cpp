// =================================================================================================
// Copyright 2006-2007 Adobe
// All Rights Reserved.
//
// NOTICE:	Adobe permits you to use, modify, and distribute this file in accordance with the terms
// of the Adobe license agreement accompanying it. If you have received this file from a source other 
// than Adobe, then your use, modification, or distribution of it requires the prior written permission
// of Adobe.
// =================================================================================================

#include "XMPDateTime.h"
#include "EsEngine.h"
#include <string>
#include <stdio.h>	// For snprintf.

using namespace ScCore;
using namespace std;

#if XMP_WinBuild
	#define snprintf _snprintf
#endif


// This section defines the properties and uses the LivePropertyManager
// to do the usual handling of the properties.

#define	kYear				100
#define	kMonth				101
#define kDay				102
#define kHour				103
#define kMinute				104
#define kSecond				105
#define kNanoSecond			106
#define kTzSign				107
#define kTzHour				108
#define kTzMinute			109
#define kGetDate			110
#define kSetLocalTimeZone	111
#define kConvertToUTCTime	112
#define kConvertToLocalTime	113
#define kCompareTo			114
#define kHasDate			115
#define kHasTime			116
#define kHasTimeZone		117


static const ScCore::LivePropertyInfo INSTANCE_PROPERTIES[] = {
	// Properties
	{ "year",			kYear,			RWPROP, "number" },
	{ "month",			kMonth,			RWPROP, "number" },
	{ "day",			kDay,			RWPROP, "number" },
	{ "hour",			kHour,			RWPROP, "number" },
	{ "minute",			kMinute,		RWPROP, "number" },
	{ "second",			kSecond,		RWPROP, "number" },
	{ "nanoSecond",		kNanoSecond,	RWPROP, "number" },
	{ "tzSign",			kTzSign,		RWPROP, "number" },
	{ "tzHour",			kTzHour,		RWPROP, "number" },
	{ "tzMinute",		kTzMinute,		RWPROP, "number" },
	
	// Methods
	{ "getDate",			kGetDate,				METHOD, "Date" },
	{ "setLocalTimeZone",	kSetLocalTimeZone,		METHOD, "" },
	{ "convertToUTCTime",	kConvertToUTCTime,		METHOD, "" },
	{ "convertToLocalTime",	kConvertToLocalTime,	METHOD, "" },
	{ "compareTo",			kCompareTo,				METHOD, "Number,date:XMPDateTime" },
	{ "hasDate",			kHasDate,				METHOD, "boolean" },
	{ "hasTime",			kHasTime,				METHOD, "boolean" },
	{ "hasTimeZone",		kHasTimeZone,			METHOD, "boolean" },
	{ null }
};


XMPDateTime::XMPDateTime() : XMPBase ("XMPDateTime"), properties (INSTANCE_PROPERTIES)
{
	setPropertyManager (&properties);
}


XMPDateTime::XMPDateTime(XMP_DateTime _dateTime) : XMPBase ("XMPDateTime"), properties (INSTANCE_PROPERTIES)
{
	this->dateTime = _dateTime;
	setPropertyManager (&properties);
}


scerror_t XMPDateTime::initialize (const Array& args, Error * errors)
{
	scerror_t errorCode = kErrOK;

	try
	{
		// init dateTime

		//dateTime = { 0 };
		dateTime.year = 0;
		dateTime.month = 0;
		dateTime.day = 0;
		dateTime.hour = 0;
		dateTime.minute = 0;
		dateTime.second = 0;
		dateTime.nanoSecond = 0;
		dateTime.tzSign = 0;
		dateTime.tzHour = 0;
		dateTime.tzMinute = 0;

		if (args.length() > 1)
		{
			// error: Only one "Date" argument allowed.
			errorCode = kErrBadArgumentList;
			errors->push(errorCode, kErrMsgBadArgList + String("'new XMPDateTime(date: Date = null)'"), NULL, false);
		}
		else if (args.length() == 1)
		{
			if (args[0].getType() == kJSObject)   // TODO check if arg[0] is a Date
			{
				ScScript::Engine* engine = ScScript::Engine::getCurrent();
				String fctName( "getFullYear" );
				ScCore::Array    emptyArgs;
				ScCore::Variant    result;

				// the JS Date is always full qualified (no "Date-only" state possible)
				dateTime.hasDate = true;
				dateTime.hasTime = true;
				dateTime.hasTimeZone = true;

				if( engine->call( args[0], "getFullYear", emptyArgs, &result ) )
				{
					dateTime.year = result.getInteger();
				} 			
				if( engine->call( args[0], "getMonth", emptyArgs, &result ) )
				{
					dateTime.month = result.getInteger() + 1;
				} 			
				if( engine->call( args[0], "getDate", emptyArgs, &result ) )
				{
					dateTime.day = result.getInteger();
				} 			
				if( engine->call( args[0], "getHours", emptyArgs, &result ) )
				{
					dateTime.hour = result.getInteger();
				} 			
				if( engine->call( args[0], "getMinutes", emptyArgs, &result ) )
				{
					dateTime.minute = result.getInteger();
				} 			
				if( engine->call( args[0], "getSeconds", emptyArgs, &result ) )
				{
					dateTime.second = result.getInteger();
				} 			
				if( engine->call( args[0], "getMilliseconds", emptyArgs, &result ) )
				{
					dateTime.nanoSecond = result.getInteger() * 1000000;
				}
				if( engine->call( args[0], "getTimezoneOffset", emptyArgs, &result ) )
				{
					int timeZoneOffset = result.getInteger();
					if (timeZoneOffset < 0)
					{
						dateTime.tzSign = +1;
					}
					else if (timeZoneOffset > 0)
					{
						dateTime.tzSign = -1;
					}
					else // timeZoneOffset == 0
					{
						dateTime.tzSign = 0;
					}
					dateTime.tzHour = abs(result.getInteger() / 60);
					dateTime.tzMinute = abs(result.getInteger() % 60);
				}
			}
			else if (args[0].getType() == kString)
			{
				SXMPUtils::ConvertToDate(getStr(args[0]), &dateTime);				
			}
			else
			{
				// error: Parameter has wrong type.
				errorCode = kErrBadArgument;
				errors->push(errorCode, args[0]);
			}
		}
	}
	catch ( XMP_Error & e )
	{
		errorCode = kErrXMPException;
		errors->push(errorCode, String(e.GetErrMsg()));
	}
	
	return errorCode;
}


scerror_t XMPDateTime::get (int32_t id, ScCore::Variant& result, ScCore::Error* errs) const
{
	scerror_t errorCode = ScCore::kErrOK;
	switch (id)
	{
		case kYear:			result.setInteger(dateTime.year); break;
		case kMonth:		result.setInteger(dateTime.month); break;
		case kDay:			result.setInteger(dateTime.day); break;
		case kHour:			result.setInteger(dateTime.hour); break;
		case kMinute:		result.setInteger(dateTime.minute); break;
		case kSecond:		result.setInteger(dateTime.second); break;
		case kNanoSecond:	result.setInteger(dateTime.nanoSecond); break;
		case kTzSign:		result.setInteger(dateTime.tzSign); break;
		case kTzHour:		result.setInteger(dateTime.tzHour); break;
		case kTzMinute:		result.setInteger(dateTime.tzMinute); break;

		default: errorCode = LiveObject::get (id, result, errs);
	}
	return setError (errorCode, id, errs);
}


scerror_t XMPDateTime::put (int32_t id, const ScCore::Variant& value, ScCore::Error* errors)
{
	scerror_t errorCode = ScCore::kErrOK;

	try
	{
		switch (id)
		{
			case kYear:			dateTime.year = value.getInteger();
								dateTime.hasDate = true;
								break;
			case kMonth:		dateTime.month = value.getInteger();
								dateTime.hasDate = true;
								break;
			case kDay:			dateTime.day = value.getInteger();
								dateTime.hasDate = true;
								break;

			case kHour:			dateTime.hour = value.getInteger();
								dateTime.hasTime = true;
								break;
			case kMinute:		dateTime.minute = value.getInteger();
								dateTime.hasTime = true;
								break;
			case kSecond:		dateTime.second = value.getInteger();
								dateTime.hasTime = true;
								break;
			case kNanoSecond:	dateTime.nanoSecond = value.getInteger();
								dateTime.hasTime = true;
								break;

			case kTzSign:		dateTime.tzSign = (XMP_Int8)value.getInteger();
								dateTime.hasTimeZone = true;
								break;
			case kTzHour:		dateTime.tzHour = value.getInteger();
								dateTime.hasTimeZone = true;
								break;
			case kTzMinute:		dateTime.tzMinute = value.getInteger();
								dateTime.hasTimeZone = true;
								break;

			default: errorCode = LiveObject::put (id, value, errors);
		}
	}
	catch ( XMP_Error & e )
	{
		errorCode = kErrXMPException;
		errors->push(errorCode, String(e.GetErrMsg()));
	}

	return setError (errorCode, id, errors);
}

scerror_t XMPDateTime::call (int32_t id, const ScCore::Array& args, ScCore::Variant& result, ScCore::Error* errors)
{
	scerror_t errorCode = ScCore::kErrOK;
	
	try
	{
		switch (id)
		{
			case kGetDate:
				if (args.length() == 0)
				{
					// TODO test and incorporate time zone of dateTime or use UTC

					string tzSign = "+";
					if (dateTime.tzSign < 0)
					{
						tzSign = "-";
					}

					// Convert to local time.
					XMP_DateTime UTCDateTime = dateTime;
					SXMPUtils::ConvertToLocalTime ( &UTCDateTime );

					char str[256];
					//  AUDIT: Using sizeof for snprintf length is safe. 
					snprintf (str, sizeof(str), "new Date(%d, %d, %d, %d, %d, %d, %d);", 
						UTCDateTime.year,
						UTCDateTime.month - 1,
						UTCDateTime.day,
						UTCDateTime.hour,
						UTCDateTime.minute,
						UTCDateTime.second,
						UTCDateTime.nanoSecond / 1000000);

					ScScript::Engine* engine = ScScript::Engine::getCurrent();
					engine->eval(str, &result);
				}
				else
				{
					errorCode = kErrBadArgumentList;
					errors->push(errorCode, kErrMsgNoArgsExpected, NULL, false);
				}
				break;

			case kSetLocalTimeZone:
				if (args.length() == 0)
				{
					SXMPUtils::SetTimeZone(&dateTime);
				}
				else
				{
					errorCode = kErrBadArgumentList;
					errors->push(errorCode, kErrMsgNoArgsExpected, NULL, false);
				}
				break;

			case kConvertToUTCTime:
				if (args.length() == 0)
				{
					SXMPUtils::ConvertToUTCTime(&dateTime);
				}
				else
				{
					errorCode = kErrBadArgumentList;
					errors->push(errorCode, String(kErrMsgNoArgsExpected), NULL, false);
				}
				break;

			case kConvertToLocalTime:
				if (args.length() == 0)
				{
					SXMPUtils::ConvertToLocalTime(&dateTime);
				}
				else
				{
					errorCode = kErrBadArgumentList;
					errors->push(errorCode, String(kErrMsgNoArgsExpected), NULL, false);
				}
				break;

			case kCompareTo:
				if (args.length() == 1 &&
					args[0].getType() == kLiveObject  &&
					args[0].getLiveObject()->getClassName() == "XMPDateTime")
				{
					XMPDateTime * xmpDate = static_cast<XMPDateTime *> (args[0].getLiveObject());
					int comp = SXMPUtils::CompareDateTime(dateTime, xmpDate->getDateTime());
					result.setInteger(comp);
				}
				else
				{
					errorCode = kErrBadArgumentList;
					errors->push(errorCode, kErrMsgBadArgList + String("'compareTo(date: XMPDateTime)'"), NULL, false);
				}
				break;

			case kHasDate:
				if (args.length() == 0)
				{
					result.setBool(dateTime.hasDate > 0);
				}
				else
				{
					errorCode = kErrBadArgumentList;
					errors->push(errorCode, String(kErrMsgNoArgsExpected), NULL, false);
				}
				break;

			case kHasTime:
				if (args.length() == 0)
				{
					result.setBool(dateTime.hasTime > 0);
				}
				else
				{
					errorCode = kErrBadArgumentList;
					errors->push(errorCode, String(kErrMsgNoArgsExpected), NULL, false);
				}
				break;

			case kHasTimeZone:
				if (args.length() == 0)
				{
					result.setBool(dateTime.hasTimeZone > 0);
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


XMP_DateTime XMPDateTime::getDateTime()
{
	return dateTime;
}


void XMPDateTime::toString (ScCore::String& result) const
{
	try
	{
		string stringValue;
		SXMPUtils::ConvertFromDate ( dateTime, &stringValue );
		result = String(stringValue.c_str());
	}
	catch ( XMP_Error & e )
	{
		result = e.GetErrMsg();
	}
}
