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
*  Copyright 1998 - 2002 Adobe Systems Incorporated
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
#ifndef _ScLockable_h
#define _ScLockable_h

#include "ScLock.h"
#include "ScPreConfig.h"

namespace ScCore
{

/**
The Lockable class is a base class that is used as a mix-in class for objects
that support external locking. Objects like LiveObject, LiveComponent or
LivePropertyManager use this class. Usually, there is no lock; calling the
lock() or unlock() methods do not have any effect. This is the default for
the single-threaded version. In the multi-threaded version, you can call the
createLock() method to explicitly create a lock. After this lock has been
created, lock() locks the object and unlock() releases the object. Both lock()
and unlock() are reference counted (at least for the same thread).
<p>
Note that Lockable is not derived from Root and does not implement a virtual destructor for performance reasons.
Please be aware that some compilers (gcc -Weffc++ ) will flag this as a problem because destructors of derived classes
might not be called in situations like this:
	Lockable* ptrToLivePropertyManager = new LivePropertyManager();
	delete ptrToLivePropertyManager;	// ~LivePropertyManager() won't be called.
*/

class SC_API Lockable 
{
	Lock*				mLock;

public:
	///
	inline				Lockable() : mLock (NULL) {}
	///
	inline				Lockable (const Lockable&) : mLock (NULL) {}
	///
	inline				~Lockable() { delete mLock; }
	/// Do Not Touch the lock in the assignment operator.
	inline	Lockable&	operator= (const Lockable&) { return *this; }
	/**
	Create a lock and attach it to this object. If a lock has been created, the lock() 
	and unlock() methods lock the object against other threads. In the single-threaded
	version, this method does nothing.
	*/
			void		createLock();
	/**
	Lock this object against other concurrent threads. You should call it if you
	want the LiveObject/LiveComponent to be protected against other threads. The
	ExtendScript engine locks a LiveObject whenever it accesses its contents.
	Both lock() and unlock() are recursive. You need to call unlock() as many
	times as you called lock() to actually unlock the object. If createLock()
	has not been called, lock() does nothing.
	*/
	inline	void		lock() const { if (mLock) mLock->acquire(); }
	/**
	Unlock this object and make it available to other threads. You must call unlock()
	as often as you called lock() in order to actually release the object to other
	threads. If createLock() has not been called, unlock() does nothing.
	*/
	inline	void		unlock() const { if (mLock) mLock->release(); }
};

/**
The object lock object locks a Lockable object. You should use this class
on the stack to lock an object, since the usage is exception safe.
*/

class ObjectLock : public ScCore::Heap
{
	const Lockable&		mLockable;
public:
						ObjectLock (const Lockable& r) : mLockable (r) { r.lock(); }
						~ObjectLock() { mLockable.unlock(); }
};

}	// end Namespace

#include "ScPostConfig.h"
#endif

