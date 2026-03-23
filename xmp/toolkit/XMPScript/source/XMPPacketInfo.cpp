// =================================================================================================
// Copyright 2006-2007 Adobe
// All Rights Reserved.
//
// NOTICE:	Adobe permits you to use, modify, and distribute this file in accordance with the terms
// of the Adobe license agreement accompanying it. If you have received this file from a source other 
// than Adobe, then your use, modification, or distribution of it requires the prior written permission
// of Adobe.
// =================================================================================================

#include "XMPPacketInfo.h"

using namespace ScCore;
using namespace std;


// This section defines the properties and uses the LivePropertyManager
// to do the usual handling of the properties.

#define	kPacket		100
#define	kOffset		101
#define kLength		102
#define kPadSize	103
#define kCharForm	104
#define kWriteable	105


static const ScCore::LivePropertyInfo INSTANCE_PROPERTIES[] = {
	{ "packet",		kPacket,	ROPROP, "string" },
	{ "offset",		kOffset,	ROPROP, "number" },
	{ "length",		kLength,	ROPROP, "number" },
	{ "padSize",	kPadSize,	ROPROP, "number" },
	{ "charForm",	kCharForm,	ROPROP, "number" },
	{ "writeable",	kWriteable,	ROPROP, "boolean" },
	{ null }
};


XMPPacketInfo::XMPPacketInfo(string _xmpStr, XMP_PacketInfo _packetInfo) : XMPBase ("XMPPacketInfo"), properties (INSTANCE_PROPERTIES)
{
	this->xmpStr = _xmpStr;
	this->packetInfo = _packetInfo;
	
	setPropertyManager (&properties);
}


scerror_t XMPPacketInfo::get (int32_t id, ScCore::Variant& result, ScCore::Error* errs) const
{
	scerror_t errorCode = ScCore::kErrOK;
	switch (id)
	{
		case kPacket:
			result.setString(String(xmpStr.c_str(), &Encoder::getUtf8()));
			break;

		case kOffset:
			result.setInteger(static_cast<int32_t>(packetInfo.offset));
			break;

		case kLength:
			result.setInteger(packetInfo.length);
			break;

		case kPadSize:
			result.setInteger(packetInfo.padSize);
			break;

		case kCharForm:
			result.setInteger(packetInfo.charForm);
			break;

		case kWriteable:
			result.setBool(packetInfo.writeable > 0);
			break;

		default: errorCode = LiveObject::get (id, result, errs);
	}
	return setError (errorCode, id, errs);
}
