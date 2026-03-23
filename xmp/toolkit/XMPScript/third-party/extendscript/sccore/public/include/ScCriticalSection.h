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
#ifndef _ScCriticalSection_h
#define _ScCriticalSection_h

#include "ScLock.h"
#include "ScPreConfig.h"

namespace ScCore
{

/// Use the ScCricitalSection macro if you do not want to invent names for your sections.

#define ScCriticalSection( lock ) CriticalSection section (lock)

/**
The CriticalSection object is intended to be used within a critical section
of code. It expects a pointer to a Lock instance. If this pointer is null,
the CriticalSection object simply does nothing. In a multithreaded environment,
you would guard a critical section of code with a Lock object stored as instance data.
In the single-threaded version, this lock would be null.
*/

class SC_API CriticalSection : public Heap
{
private:
	/// Private copy ctor, because this class has pointer data members.
						CriticalSection (const CriticalSection&);
	/// Private assignment operator, because this class has pointer data members.
	CriticalSection& 	operator= (const CriticalSection&);

	Lock*	mLock;
public:
	///
	inline	CriticalSection (Lock* lock) : mLock (lock) { if (lock) lock->acquire(); }
	///
	inline	~CriticalSection() { if (mLock) mLock->release(); }
};

}	// end Namespace

#include "ScPostConfig.h"
#endif

