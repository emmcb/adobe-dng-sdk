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
#ifndef _ScAllocator_h
#define _ScAllocator_h

#include "ScRoot.h"
#include "ScPreConfig.h"

namespace ScCore
{

/**
If necessary, an application can change the way the Scripting core library
allocates memory. All scripting core objects use a default allocator by 
deriving from the Heap class which has overloaded operators for new and delete. 
These operators usually throw a std::bad_alloc exception on an out-of-memory 
condition. There are alternate implementations of the new operators with the 
addition std::nothrow arguments that instead return null. This exception is 
usually caught within an ExtendScript engine and converted to the runtime 
error kErrNoMemory. If you, however, call setRethrow (true), the exception 
is never caught by the engine, but rather passed up to the calling application, 
which has to deal with the condition. The default allocator does not attempt to free a 
panic buffer or do anything else when it runs out of memory.
<p>
An application wishing to change the memory allocator can override this
class, thus defining new allocate and free methods. As soon as a new
allocator is set, the core classes use this allocator to obtain and release
memory. On low memory conditions, the new allocator may try to release
buffer space within the application in order to satisfy the memory
request. The allocator should never throw any exception, but rather return
null if the memory request cannot be satisfied.
<p>
The number of allocators you can use is limited to 10. It is, essentially 
that no allocator is deleted while there still are memory blocks around 
that were allocated with this allocator!
*/

class SC_API Allocator : public Root
{
	bool				mRethrow;
public:
	inline				Allocator() :mRethrow(false) { }
	/**
	Retrieve the current memory allocator. Initially, this pointer
	is the default allocator.
	*/
	static	Allocator*	get();
	/**
	Set a new allocator to use. ScCore uses this allocator for
	all subsequent allocations. Supplying null reverts to the default
	allocator. You can set a maximum of 10 different allocators. An
	attempt to set more than ten allocators is ignored, and the method
	return false. Allocators belong to the caller. The caller must not
	delete any allocator while ScCore still is active.
	*/
	static	bool		set (Allocator* alloc);
	/**
	Allocate the given amount of memory. If this method returns null,
	the allocator throws a std::bad_alloc exception which is passed to
	the calling app.
	*/
	virtual void*		allocMem (size_t size) = 0;
	/**
	Allocate the given amount of memory. If this method returns null,
	the allocator throws a std::bad_alloc exception which is passed to
	the calling app. This is the debugging version, with file and
	line number information.
	*/
	virtual void*		allocMem (size_t size, const char* file, int line);
	/**
	Deallocate the given pointer. The code ensures that the pointer
	supplied has been allocated with this allocator.
	*/
	virtual void		freeMem (void* p) = 0;
	/**
	Reallocate an already existing memory chunk. Use this method to
	shrink or expand a memory segment. If the segment size cannot be
	changed, create a new segment and copy the contents over.
	@param	p			the memory chunk to change
	@param	size		the new length
	@return				the new memory pointer; may be the same as supplied
	*/
	virtual	void*		reallocMem (void* p, size_t size) = 0;
	/**
	Flush the cache of this allocator. The implementation is optional.
	*/
	virtual	void		flush();
	/**
	Control whether the engine should rethrow an out-of-memory condition
	instead of generating a runtime error. Call this method with true as
	argument if you are able and willing to handle std::bad_alloc exceptions
	in your program code.
	*/
	inline	void		setRethrow (bool yes) { mRethrow = yes; }
	/**
	Check whether to rethrow an out-of-memory condition.
	*/
	inline	bool		isRethrow() const { return mRethrow; }
};
}	// end Namespace

#include "ScPostConfig.h"
#endif

