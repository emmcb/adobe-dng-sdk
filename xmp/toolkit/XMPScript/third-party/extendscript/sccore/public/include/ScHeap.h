/**************************************************************************
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
* $File$
* $Author$
* $DateTime$
* $Revision$
* $Change$
**************************************************************************/

#if SC_ONCE
#pragma once
#endif
#ifndef _ScHeap_h
#define _ScHeap_h

#include "ScDefs.h"
#include <new>
#include "ScPreConfig.h"

namespace ScCore
{

/// Get the version number. @see SC_VERSION_STRING

SC_API scint32_t getVersion();

/**
The base class of the ScCore foundation objects contains a memory allocator 
that lets you control how the ScCore objects allocates memory. The memory 
allocator makes sure that every Core object is allocated on the same heap
so you can pass them freely between apps and DLLs. It also does and statistics 
in the debugging version. It uses whatever allocator you define. If the memory 
requirement cannot be fulfilled, a std::bad_alloc exception is thrown.
The debugging version of operator new takes the file and line numbers. 
@see Allocator
*/

class SC_API SC_PURE Heap
{
					Heap (const Heap&);
	Heap& 			operator= (const Heap&);
public:
	/** Inline constructor to keep some compilers happy.
		Note that Heap does not implement a virtual destructor for performance reasons.
		Please be aware that some compilers (gcc -Weffc++ ) will flag this as a problem because destructors of derived classes
		might not be called in situations like this:
			Heap* ptrToRoot = new Root();
			delete ptrToRoot;	// ~Root won't be called.
	*/
	inline			Heap() {}
	/// Get the cache size. This is  a thread local value and preset to 100 K.
	static scint32_t	getCacheSize();
	/// Set the cache size. This is a thread local value and preset to 100 K.
	static	void	setCacheSize (scint32_t sizeInBytes);
	/// Flush the thread local memory cache plus all allocators.
	static	void	flushCache();
	///
	static	void*	operator new (size_t size) throw (std::bad_alloc);
	///
	static	void*	operator new[] (size_t size) throw (std::bad_alloc);
	///
	static	void*	operator new (size_t size, const char* file, int line) throw (std::bad_alloc);
	///
	static	void*	operator new[] (size_t size, const char* file, int line) throw (std::bad_alloc);
	/// Dummy inline placement new to make STL containers happy.
	static	void*	operator new (size_t, void* memP) { return memP; }
	/**
	The memory deallocator makes sure that every ScCore object is deallocated by the
	same allocator that allocated it. It also does memory statistics in the debugging 
	version.
	*/
	static	void	operator delete (void* p) throw (std::bad_alloc);
	///
	static	void	operator delete[] (void *p) throw (std::bad_alloc);
	/// Dummy inline placement delete to make STL containers happy.
	static	void	operator delete (void*, void*) {}
	/// Matching operator delete for new (file, line).
	static	void	operator delete (void* p, const char*, int) throw (std::bad_alloc);
	///
	static	void	operator delete[] (void *p, const char*, int) throw (std::bad_alloc);
	/**
	Reallocate an already existing memory chunk. Use this method to
	shrink or expand a memory segment. If the segment size cannot be
	changed, create a new segment and copy the contents over. This method
	is not available for the static ScCore library.
	@param	p		the memory chunk to change
	@param	size	the new length
	@return			the new memory pointer; may be the same as supplied
	*/
	static	void*	reAlloc (void* p, size_t size) throw (std::bad_alloc);
	/**
	Return some memory statistics. This method returns the total allocated size 
	in debug mode only. This method is not available for the static ScCore library.
	@param	count	the total number of memory elements allocated
	@param	size	the total size of all memory elements allocated (debug version only)
	*/
	static	void	getMemoryStatistics (scint32_t& totalCount, scint32_t& totalSize);
};

}	// end Namespace

#include "ScPostConfig.h"

#endif


