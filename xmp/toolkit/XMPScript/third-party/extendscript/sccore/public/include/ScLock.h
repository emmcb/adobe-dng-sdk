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
#ifndef _ScLock_h
#define _ScLock_h

#include "ScRoot.h"
#include "ScPreConfig.h"

extern "C" {
	SC_API scint32_t ScAtomicInc (scint32_t* p);
	SC_API scint32_t ScAtomicDec (scint32_t* p);
}

namespace ScCore
{

class LockData;
class RWLockData;
class EventData;

/**
The Lock implements a simple mutex class to be used in a multithreading environment.
It has acquire() and release() methods that acquire and release the mutex, respectively.
The acquire() method blocks the calling thread if another thread owns the lock, while
the release() method releases the lock again. acquire() and release() are nestable.
When deleting a lock, the lock is released if it has been acquired. On Windows and Mac,
it uses CriticalSection objects; on Unixes, it uses pthread mutexes.
Locks can be used recursively; if a thread already owns a lock, re-acquiring the
lock does not block the thread, but the lock must be released as often as it has
been acquired.
*/

class SC_API Lock : public Heap
{
	/// Private copy ctor, because this class has pointer data members.
					Lock (const Lock&);
	/// Private assignment operator, because this class has pointer data members.
	Lock& 			operator= (const Lock&);

public:
	/// Do an atomic increment on the given variable and return the result.
	static	scint32_t	increment (scint32_t& var) { return ScAtomicInc ((scint32_t*)&var); }
	/// Do an atomic decrement on the given variable and return the result.
	static	scint32_t	decrement (scint32_t& var) { return ScAtomicDec ((scint32_t*)&var); }

	/// The ctor initializes the operating system dependent parts.
					Lock();
	/// The dtor releases the the lock and any operating system dependent parts.
					~Lock();
	/// Acquire the lock. This may block the thread. Recursive calls are OK.
			void	acquire();
	/// Release the lock. This may release a waiting thread. Recursive calls are OK.
			void	release();
private:
		LockData*	mData;
};

/**
The Event implements a simple event class to be used in a multithreading environment.
It has wait() and signal() methods that causes a thread to wait for an event or to
signal an event, respectively. The wait() method blocks the calling thread until
another thread signals an event. If the event already has been signaled, the thread
does not block. Multiple threads can wait for a single event. The run order of the
threads when the event is signaled cannot be predicted. On Windows, it uses
Event objects, on the Mac, it uses MPEvents, and on Unixes, it uses pthread conditional
variables. 
<p>
NOTE: An event does not remember a signal if no thread is waiting. Waiting for an event
always blocks the thread.
*/

class SC_API Event : public Heap
{
	/// Private copy ctor, because Event has pointer data members.
					Event (const Event&);
	/// Private assignment operator, because Event has pointer data members.
	Event& 			operator= (const Event&);
public:
	/// The ctor initializes the operating system dependent parts.
					Event();
	/// The dtor releases any operating system dependent parts.
					~Event();
	/// Wait for the event. This may block the thread.
			void	wait();
	/// Signal an event. This may release a lot of waiting threads.
			void	signal();
private:
		EventData*	mData;
};

/**
This class is a simple wrapper for a Lock that makes sure the lock is released
during the stack unwinding of an exception.
*/

class SC_API LockRef : public Heap
{
	/// Private copy ctor, because LockRef has pointer data members.
					LockRef (const LockRef&);
	/// Private assignment operator, because LockRef has pointer data members.
	LockRef& 		operator= (const LockRef&);

public:
					LockRef (Lock* p) : mLock (p) { if (p) p->acquire(); }
					~LockRef() { if (mLock) mLock->release(); }
private:
		Lock*		mLock;
};

}	// end namespace

#include "ScPostConfig.h"
#endif

