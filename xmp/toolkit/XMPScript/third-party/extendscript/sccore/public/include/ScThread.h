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
#ifndef _ScThread_h
#define _ScThread_h

#include "ScRoot.h"
#include "ScPreConfig.h"

namespace ScCore
{

class Array;

/**
The Thread class implements a very simple thread. Its main purpose is to
help testing the multithreaded implementation of the ExtendScript/ScCore
components. For communication between threads, use Lock and Event instances.
There is no need to use this class in order to have ScCore multithreading;
ScCore uses native Windows functionality, Apple Multiprocessing APIs, or
pthreads for Unixes. Once created, the start() method runs its run() method and
terminates upon return from that method. The stack is the default stack the OS
allocates for threads.
*/

class SC_API Thread : public Root
{
	static scuint32_t SC_STDCALL go (void*);
	intptr_t				mData;
	scuint32_t				mID;
protected:
	/// The derived run() method may set an error code here.
	scerror_t				mError;
	/**
	Overload this method to implement the thread functionality.
	*/
	virtual	void			run() = 0;
private:
	bool					mActive;
public:
	/**
	The constructor creates an inactive thread using the default stack allocation
	mechanisms that the OS provides. The OS usually allocates a stack of about 1 MB.
	*/
							Thread();
	/**
	Delete this thread. Do not call the thread destructor before the thread
	actually has terminated. Use isActive() to check for this.
	*/
	virtual					~Thread();
	/**
	Get the ID of the current thread. Returns 0 for the main thread ID.
	*/
	static	scuint32_t		getCurrentID();
	/**
	Send the currently executing thread to sleep.
	*/
	static	void			sleep (scuint32_t msecs);
	/**
	Return the high-resolution timer. This call resets the timer, so the next call
	returns the elapsed time since the last call. The value is in microseconds.
	*/
	static	scint64_t		getHiResTimer();
	/**
	Get the ID of this thread. For info purposes only.
	*/
	inline	scuint32_t		getID() const { return mID; }
	/**
	Start this thread. Calling this method causes the run() method to be invoked as a separate
	thread.
	*/
			void			start();
	/**
	Check if this thread is still active.
	*/
	inline	bool			isActive() const { return mActive; }
	/**
	Retrieve the error code of the thread. This error code is kErrBadAction
	if the thread could not be created; otherwise, it may be an error code set
	by the derived run() method.
	*/
	inline	scerror_t		getError() const { return mError; }
};

}	// end namespace

#include "ScPostConfig.h"

#endif

