// =================================================================================================
// Copyright 2006-2007 Adobe
// All Rights Reserved.
//
// NOTICE:	Adobe permits you to use, modify, and distribute this file in accordance with the terms
// of the Adobe license agreement accompanying it. If you have received this file from a source other 
// than Adobe, then your use, modification, or distribution of it requires the prior written permission
// of Adobe.
// =================================================================================================

#include "XMPFile.h"
#include "XMPMeta.h"
#include "XMPPacketInfo.h"
#include "XMPFileInfo.h"

using namespace ScCore;
using namespace std;


// This section defines the properties and uses the LivePropertyManager
// to do the usual handling of the properties.

#define kGetXMP			100
#define kGetPacketInfo	101
#define kGetFileInfo	102
#define kPutXMP			103
#define kCanPutXMP		104
#define kCloseFile		105

static const ScCore::LivePropertyInfo INSTANCE_PROPERTIES[] = {
	{ "getXMP",			kGetXMP,		METHOD, "XMPMeta" },
	{ "getPacketInfo",	kGetPacketInfo,	METHOD, "XMPPacketInfo" },
	{ "getFileInfo",	kGetFileInfo,	METHOD, "XMPFileInfo" },
	{ "putXMP",			kPutXMP,		METHOD, "XMPProperty,schemaNS:string,propName:string,qualNS:string,qualName:string" },
	{ "canPutXMP",		kCanPutXMP,		METHOD, "schemaNS:string,propName:string,propValue:string,setOptions:number" },
	{ "closeFile",		kCloseFile,		METHOD, "schemaNS:string,arrayName:string,itemIndex:number,itemValue:string,setOptions:number" },
	{ null }
};


XMPFile::XMPFile() : XMPBase ("XMPFile"), properties (INSTANCE_PROPERTIES)
{
	setPropertyManager (&properties);
}


scerror_t XMPFile::initialize (const Array& args, Error * errors)
{
	scerror_t errorCode = kErrOK;

	try
	{
		if (args.length() == 3)
		{
			file = SXMPFiles(getStr(args[0]).c_str(), args[1].getInteger(), args[2].getInteger());
		}
		else 
		{
			errorCode = kErrBadArgumentList;
			errors->push(errorCode, kErrMsgBadArgList + String("'new XMPFile(filePath: String, format: Number, openFlags: Number)'"), NULL, false);
		}
	}
	catch ( XMP_Error & e )
	{
		errorCode = kErrXMPException;
		errors->push(errorCode, String(e.GetErrMsg()));
	}
	
	return errorCode;
}


scerror_t XMPFile::call (int32_t id, const ScCore::Array& args, ScCore::Variant& result, ScCore::Error* errors)
{
	scerror_t errorCode = ScCore::kErrOK;
	
	try
	{
		switch (id)
		{
			case kGetXMP:
				if (args.length() == 0)
				{
					SXMPMeta xmpMeta;
					file.GetXMP(&xmpMeta, null, null);

					XMPMeta * meta = new XMPMeta(xmpMeta);
					result.setLiveObject(meta);
					meta->release();
				}
				else
				{
					errorCode = kErrBadArgumentList;
					errors->push(errorCode, kErrMsgNoArgsExpected, NULL, false);
				}
				break;

			case kGetPacketInfo:
				if (args.length() == 0)
				{
					string xmpStr;
					XMP_PacketInfo packetInfo;
					
					file.GetXMP(null, &xmpStr, &packetInfo);

					XMPPacketInfo * xmpPacketInfo = new XMPPacketInfo(xmpStr, packetInfo);
					result.setLiveObject(xmpPacketInfo);
					xmpPacketInfo->release();
				}
				else
				{
					errorCode = kErrBadArgumentList;
					errors->push(errorCode, kErrMsgNoArgsExpected, NULL, false);
				}
				break;

			case kGetFileInfo:
				if (args.length() == 0)
				{
					string filePath;
					XMP_OptionBits openFlags;
					XMP_FileFormat format;
					XMP_OptionBits handlerFlags;
					
					file.GetFileInfo(&filePath, &openFlags, &format, &handlerFlags);

					XMPFileInfo * fileInfo = new XMPFileInfo(filePath, openFlags, format, handlerFlags);
					result.setLiveObject(fileInfo);
					fileInfo->release();
				}
				else
				{
					errorCode = kErrBadArgumentList;
					errors->push(errorCode, kErrMsgNoArgsExpected, NULL, false);
				}
				break;

			case kPutXMP:
				if (args.length() == 1)
				{
					if (args[0].getType() == kLiveObject  &&
						args[0].getLiveObject()->getClassName() == "XMPMeta")
					{
						XMPMeta * meta = static_cast<XMPMeta *> (args[0].getLiveObject());
						file.PutXMP(meta->getXMPMeta());
					}
					else if (args[0].getType() == kString)
					{
						string buffer = getStr(args[0]);
						file.PutXMP(buffer);
					}
					else if (args[0].getType() == kJSObject)  // TODO check if arg[0] is an Array
					{
						Array byteArray;
						args[0].getArray(byteArray);

						string buffer;
						buffer.reserve ( byteArray.length() );
						
						for (uint i = 0; i < byteArray.length(); i++)
						{
							buffer.append ( static_cast<char> (byteArray[i].getInteger()  &  0xff), 1 );
						}

						file.PutXMP(buffer.c_str(), (XMP_StringLen)buffer.size());
					}
					else
					{
						errorCode = kErrBadArgument;
					}
				}
				else
				{
					errorCode = kErrBadArgumentList;
					errors->push(errorCode, kErrMsgBadArgList + String("'putXMP(xmpObj: XMPMeta/String/Array)"), NULL, false);
				}
				break;

			case kCanPutXMP:
				if (args.length() == 1)
				{
					bool canPutXMP = false;
					
					if (args[0].getType() == kLiveObject  &&
						args[0].getLiveObject()->getClassName() == "XMPMeta")
					{
						XMPMeta * meta = static_cast<XMPMeta *> (args[0].getLiveObject());
						canPutXMP = file.CanPutXMP(meta->getXMPMeta());
					}
					else if (args[0].getType() == kString)
					{
						string buffer = getStr(args[0]);
						canPutXMP = file.CanPutXMP(buffer);
					}
					else if (args[0].getType() == kJSObject)  // TODO check if arg[0] is an Array
					{
						Array byteArray;
						args[0].getArray(byteArray);

						string buffer;
						buffer.reserve ( byteArray.length() );
						
						for (uint i = 0; i < byteArray.length(); i++)
						{
							buffer.append ( static_cast<char> (byteArray[i].getInteger()  &  0xff), 1 );
						}

						canPutXMP = file.CanPutXMP(buffer.c_str(), (XMP_StringLen)buffer.size());
					}
					else
					{
						errorCode = kErrBadArgument;
					}

					result.setBool(canPutXMP);
				}
				else
				{
					errorCode = kErrBadArgumentList;
					errors->push(errorCode, kErrMsgBadArgList + String("'canPutXMP(xmpObj: XMPMeta/String/Array): Boolean"), NULL, false);
				}
				break;

			case kCloseFile:
				if (args.length() <= 1)
				{
					int closeFlags = args.length() >= 1 ? args[0].getInteger() : 0;
					file.CloseFile(closeFlags);
				}
				else
				{
					errorCode = kErrBadArgumentList;
					errors->push(errorCode, kErrMsgBadArgList + String("'closeFile(closeFlags: Number = 0)"), NULL, false);
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


void XMPFile::setName(ScCore::String _name) const
{
	 //Fixing warning of shadowing the variable. this->name = _name should be done and const should be removed. Not changing the public API for now.
	//_name = _name;
}
