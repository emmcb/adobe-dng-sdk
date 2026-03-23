/**************************************************************************
*
* ADOBE CONFIDENTIAL
* ___________________
*
*  Copyright 1998 Adobe Systems Incorporated
*  All Rights Reserved.
*
* NOTICE:  All information contained herein is, and remains the property of
* Adobe Systems Incorporated  and its suppliers,  if any.  The intellectual 
* and technical concepts contained herein are proprietary to  Adobe Systems 
* Incorporated  and its suppliers  and may be  covered by U.S.  and Foreign 
* Patents,patents in process,and are protected by trade secret or copyright 
* law.  Dissemination of this  information or reproduction of this material
* is strictly  forbidden  unless prior written permission is  obtained from 
* Adobe Systems Incorporated.
* $File$
* $Author$
* $DateTime$
* $Revision$
* $Change$
**************************************************************************/

#if SC_ONCE
#pragma once
#endif
#ifndef _ScLiveMessage_h
#define _ScLiveMessage_h

#include "ScMessage.h"
#include "ScArray.h"
#include "ScPreConfig.h"

namespace ScCore
{

/**
Message sent to a listener object to check for the presence of a method. 
The value is the method name. On return, the value contains true if the
method exists.
*/
const scint32_t kLiveMsgCanInvoke			= 0x6C6F4369;	// 'loCi'

/// Message sent to a listener object upon a invoke() call. The value is the method name.

const scint32_t kLiveMsgInvoke				= 0x6C6F496E;	// 'loIn'

/// Message sent to a listener when the object becomes invalid
	
const scint32_t kLiveMsgInvalidated			= 0x6C6F4977;	// 'loIv'

/// Message sent to a listener when the object becomes valid again, e.g. after an undo operation
	
const scint32_t kLiveMsgRevalidated			= 0x6C6F5277;	// 'loRv'

/// Message sent to a listener to make it forget any cached property IDs because they may have changed

const scint32_t kLiveMsgRemapPropertyIDs	= 0x6C6F466F;	// 'loFo'
/**
Broadcast message ID for querying an ID for a name. The property ID field contains 0, and
you are expected to fill in the new property ID. The value field contains the name to query.
You will need to set the value field to either kReadOnly, kReadWrite,
or kMethod. This message is not broadcast for index properties.
*/

const scint32_t kLiveMsgGetID				= 0x6C6F4769;	// 'loGi'

/**
Broadcast message ID for getting a value. The ID field contains the ID, and you are
expected to set the value.
*/

const scint32_t kLiveMsgGet					= 0x6C6F4765;	// 'loGe'

/**
Broadcast message ID for putting a value. The ID field contains the ID, and the
value field the value.
*/

const scint32_t kLiveMsgPut					= 0x6C6F5075;	// 'loPt'

/**
Broadcast message ID for calling a method. The ID field contains the ID, and the
args field the argument list. The value field takes the result.
*/

const scint32_t kLiveMsgCall				= 0x6C6F4361;	//' loCa'

/**
Broadcast message ID for enumerating an object. The ID field contains a pointer
to an SimpleArray coerced to a scint32_t. This array is already filled with the IDs found
in the object so far, so you can add missing stuff.
*/

const scint32_t kLiveMsgEnumerate			= 0x6C6F456E;	// 'loEn'

/**
Broadcast message ID for getting the length of the collection. Set the data
value to the desired length.
*/

const scint32_t kLiveMsgGetLength			= 0x6C6F4C47;	// 'loLG'

/**
Broadcast message ID after setting the length of the collection. The data
value contains the new length.
*/

const scint32_t kLiveMsgPutLength			= 0x6C6F4C53;	// 'loLS'

/**
Broadcast message ID for getting a value by index. The ID field contains the 
index, and you are expected to set the value.
*/

const scint32_t kLiveMsgGetByIndex			= 0x6C6F4967;	// 'loIg'

/**
Broadcast message ID for putting a value by index. The ID field contains the 
index, and the value field the value.
*/

const scint32_t kLiveMsgPutByIndex			= 0x6C6F4970;	// 'loIp'

/**
Broadcast message ID for deleting a value by index. The ID field contains the 
index.
*/

const scint32_t kLiveMsgDeleteByIndex		= 0x6C6F4974;	// 'loId'

/**
Message sent to a listener when a message broadcaster dies. The destructor of
a Broadcaster always broadcasts this message.
*/

const scint32_t kLiveMsgDeleted				= kMsgDeleted;

/**
Message sent to the LiveObject from the JavaScript engine every time a local
JavaScript property has been created. The message is a simple Message instance
with the sender set to null. The global LiveObject is never notified due to
performance reasons.
NOTE: This message is temporary; it will most likely go away in ExtendScript 4.0.
*/

const scint32_t kLiveMsgJSPropertyCreated	= 0x6C6F4A50;	// 'loJP'

class Error;
class LiveObject;

/**
The LiveMessage message is broadcast during a get or put operation.
You may choose to listen to these messages in order to supply data on demand,
or to forward any stored data to somewhere else. A LiveMessage is not thread safe.
*/

class SC_API LiveMessage : public DataMessage
{
	/// Private copy ctor, because this class has pointer data members.
							LiveMessage (const LiveMessage&);
	/// Private assignment operator, because this class has pointer data members.
	LiveMessage& 			operator= (const LiveMessage&);

private:
	const LiveObject&		mThis;
	scint32_t				mPropID;
	const Array				mArgs;
	scint32_t				mLanguage;
	SimpleArray*			mEnumIDs;
	Error*					mError;
	scerror_t				mErrorCode;
public:
	/// The get ctor takes the ID and the property ID.
							LiveMessage (const LiveObject& thisObj, scint32_t msgid, scint32_t propid, Error* err = NULL);
	/// The put ctor takes the ID, the property ID and the data.
							LiveMessage (const LiveObject& thisObj, scint32_t msgid, scint32_t propid, const Variant& data, Error* err = NULL);
	/// The call ctor takes the ID, the property ID and the arguments.
							LiveMessage (const LiveObject& thisObj, scint32_t msgid, scint32_t propid, const Array& args, Error* err = NULL);
	/// The enumeration ctor takes the ID, the property ID and the pre-filled array of IDs.
							LiveMessage (const LiveObject& thisObj, scint32_t msgid, SimpleArray& enumIDs);
	/// Get the LiveObject to work upon.
	inline const LiveObject& getThis() const { return mThis; }
	/// Accessor to the ID. In some broadcasts, you need to store a value here.
	inline	scint32_t&		getPropertyID() { return mPropID; }
	/// Accessor to the script language identifier.
	inline	scint32_t&		getLanguage() { return mLanguage; }
	/// Accessor to the error code. Set a code here to have the LiveObject return that code.
	inline	scerror_t&		getError() { return mErrorCode; }
	/// Accessor to any extended error stored here. May be null.
	inline	Error*			getExtError() const { return mError; }
	/// Accessor to the argument list for function calls.
	inline	const Array&	getArgs() const { return mArgs; }
	/// Accessor to the enumeration ID array if present.
	inline	SimpleArray*	getEnumIDs() const { return mEnumIDs; }
};

}	// end Namespace

#include "ScPostConfig.h"
#endif

