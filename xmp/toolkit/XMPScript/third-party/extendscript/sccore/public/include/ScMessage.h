/**************************************************************************
* $File$
* $Author$
* $DateTime$
* $Revision$
* $Change$
*
* ADOBE CONFIDENTIAL
* ___________________
*
*  Copyright 1998 - 2003 Adobe Systems Incorporated
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
**************************************************************************/

#if SC_ONCE
#pragma once
#endif
#ifndef _ScMessage_h
#define _ScMessage_h

#include "ScArray.h"
#include "ScVariant.h"
#include "ScPreConfig.h"

namespace ScCore
{

class Message;
class BroadcasterData;

/**
A message listener is able to receive a Message object from a message broadcaster.
Message listeners register themselves with a message broadcaster. The messages
are broadcast to each listener registered for a particular type of message until
one of the listeners returns true to indicate that the message has been processed.
This class is intended to be used as a mix-in class. Do not derive from this class
unless your class also derives from Heap to avoid confusing the memory allocation
algorithms.
<p>
When the listener is destroyed, it must remove itself from all associated broadcasters.
If you want to use this class on the heap without mixing it into another class,
please note that the allocator for this class is the generic operator new,
not the operator new defined in the Heap base class, which means that the memory
needed for this class will not go through your memory allocator.
*/

class SC_API Listener
{

public:
	virtual				~Listener();

	/**
	Overload this method to receive messages.
	*/
	virtual	bool		notify (Message& msg) = 0;
};

/**
The message broadcaster implements message broadcasting. It has method for
listeners to register and unregister themselves. On calling the broadcast()
method, the notify() method of all registered listeners is called until
one of the listeners returns true to indicate that the message has been 
processed successfully. The broadcast is synchronous.
This class is intended to be used as a mix-in class. Do not derive from this class
unless your class also derives from Heap to avoid confusing the memory allocation
algorithms.
<p>
When a broadcaster instance is deleted, it broadcasts a message kMsgDeleted to
all listeners listening either to all message or to the kMsgDeleted message.
<p>
The Broadcaster class is thread safe. All methods lock an internal lock.
If you want to use this class on the heap without mixing it into another class,
please note that the allocator for this class is the generic operator new,
not the operator new defined in the Heap base class, which means that the memory
needed for this class will not go through your memory allocator.
<p>
Note that Broadcaster is not derived from Root and does not implement a virtual destructor for performance reasons.
Please be aware that some compilers (gcc -Weffc++ ) will flag this as a problem because destructors of derived classes
might not be called in situations like this:
	Broadcaster* ptrToLiveObject = new LiveObject();
	delete ptrToLiveObject;	// ~LiveObject won't be called.
*/

class SC_API Broadcaster 
{
	BroadcasterData*		mData;
public:
							Broadcaster();
	/**
	When Broadcaster is deleted, it broadcasts a kMsgDeleted message to
	interested listeners. On receipt of the kMsgDeleted message, listeners
	should no longer expect messages from that broadcaster.
	*/
							~Broadcaster();
	/// Is a listener registered for a specific message ID?
			bool			isListening (scint32_t msgid) const;
	///	Register a message listener. Use kMsgAll to listen to all messages.
			void			addListener (Listener& listener, scint32_t what);
	/// Unregister a message listener. Use kMsgAll to completely remove the listener.
			void			removeListener (Listener& listener, scint32_t what);
	/// Broadcast a message. Return true if a listener replied with true.
			bool			broadcast (Message& msg) const;
	/// Broadcast a simple message. Return true if a listener replied with true.
			bool			broadcast (scint32_t msgID) const;
	
	/// Prevent transfer of listener list when copying a broadcaster (sub)class.
	inline					Broadcaster( const Broadcaster& ) : mData( NULL ) {}
	///
	inline	Broadcaster& 	operator= (const Broadcaster&) { return *this; }
};

/**
The basic message class contains a four-byte message ID and a pointer to the sender.
Additionally, it contains a virtual getData() method that returns null. In the
derived class DataMessage, the getData() method returns a pointer
to a Variant instance.
*/

class SC_API Message : public Root
{
	/// Private copy ctor, because Message has pointer data members.
					Message (const Message&);
	/// Private assignment operator, because Message has pointer data members.
	Message& 		operator= (const Message&);

private:
	friend class				Broadcaster;
	const Broadcaster*			mSender;	// set during broadcast()
	scint32_t						mID;
public:
	/// The ctor accepts the message ID
								Message (scint32_t id) : mSender (NULL), mID (id) {}
	/// Retrieve the sender. This variable is set during broadcasting. It is null if not broadcast.
	inline	const Broadcaster*	getSender() const { return mSender; }
	/// Get the message ID.
	inline	scint32_t				getID() const { return mID; }
	/// Get the message data. This default method returns null.
	virtual	Variant*			getData() const;
};

/**
The DataMessage class contains a Variant instance that is used to transport
data between bradcaster and listener. This data may be modified by the
listener in order to return some value.
*/

class SC_API DataMessage : public Message
{
	mutable	Variant		mData;
public:
	/// A message with an undefined value.
						DataMessage (scint32_t id);
	/// A message with a Boolean value.
						DataMessage (scint32_t id, bool val);
	/// A message with an integer value.
						DataMessage (scint32_t id, scint32_t val);
	/// A message with a floating-point value.
						DataMessage (scint32_t id, double val);
	/// A message with a string literal value (ASCII only).
						DataMessage (scint32_t id, const char* val);
	/// A message with a UTF-16 string literal value.
						DataMessage (scint32_t id, const scutf16_t* val);
	/// A message with a string instance value (a bit faster than a literal).
						DataMessage (scint32_t id, const String& val);
	/// A message with a LiveObject value or a null value.
						DataMessage (scint32_t id, LiveObject* val);
	/// A message with a Variant value.
						DataMessage (scint32_t id, const Variant& val);
	/// Retrieve the value.
	virtual	Variant*	getData() const;
};

/**
The ArgumentsMessage adds an Array instance to the DataMessage. Use this
message to e.g. call a method with an argument list and a return value.
*/

class SC_API ArgumentsMessage : public DataMessage
{
	const	Array		mArgs;
public:
	/// The ctor takes the ID and the argument list.
						ArgumentsMessage (const Variant& args);
	/// Get the arguments.
	inline	const Array& getArgs() const { return mArgs; }
};

/**
Enumeration of standard message ID values.
*/

enum
{
	/** When a broadcaster is deleted, it broadcasts a kMsgDeleted message to
	interested listeners. On receipt of the kMsgDeleted message, listeners
	should no longer expect messages from that broadcaster.
	*/
	kMsgDeleted		= 0x44454C45,	// 'DELE'
	/// Special message identifier to add a listener to every message.
	kMsgAll			= 0x414C4C21	// 'ALL!'
};

}	// end Namespace

#include "ScPostConfig.h"

#endif

