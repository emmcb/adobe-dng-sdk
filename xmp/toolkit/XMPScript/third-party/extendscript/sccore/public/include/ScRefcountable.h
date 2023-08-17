/**************************************************************************
*
* ADOBE CONFIDENTIAL
* ___________________
*
*  Copyright 1998 - 2007 Adobe Systems Incorporated
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
#ifndef _ScRefcountable_h
#define _ScRefcountable_h

#include "ScLock.h"
#include "ScPreConfig.h"

namespace ScCore
{

/**
The Refcountable class is the base class for all objects that are refcounted.
Classes that derive from this class should make their destructors protected
so instances cannot be allocated on the stack, and release() must be called.
*/

class SC_API Refcountable : public Root
{
protected:
	/// The refcount is preset to 1, and decremented by release().
	mutable	 scint32_t			iRefCount;
	/// Dont construct a Refcountable directly. The refcount is initially 1.
								Refcountable() : iRefCount (1) {}
	/// And never delete a Refcountable directly! You should also make your derived destructors protected.
	virtual						~Refcountable();
public:
	/// Increment the ref count of this object.
	inline	void				addRef() const { Lock::increment (iRefCount); }
	/// Get the ref count.
	inline	scint32_t			getRefCount() const { return iRefCount; }
	/**
	Release the object. The object is deleted when the refcount reaches zero. 
	The "this" pointer may, therefore, be invalid after calling this method.
	*/
	virtual	void				release() const;
};

/**
The WeakRef class is a weak reference to a refcounted instance. A weak reference
becomes NULL when the referenced instance is deleted.
*/

class SC_API WeakRefBase : public Heap
{
protected:
	Refcountable*				mRef;
								WeakRefBase();
								~WeakRefBase();
	/// Assign the weak reference
			Refcountable*		assign (Refcountable* p);
public:
	/// Release all references to the given instance.
	static	void				release (const Refcountable* p);
};

/**
The templatized class WeakRef allows to create direct weak references to classes
derived from Refcountable, like e.g. LiveObjects.
*/

template <class T>
class WeakRef : public WeakRefBase
{
public:
	inline						WeakRef() : WeakRefBase() {}
	inline						~WeakRef() {}
	inline	T*					operator= (T* p) { return (T*) WeakRefBase::assign (p); }
	inline	T&					operator= (T& r) { WeakRefBase::assign (&r); return (T&) r; }
	inline	T*					get() const { return (T*) mRef; }
	inline	T*					operator->() const { return (T*) mRef; }
	inline						operator T*() const { return (T*) mRef; }
	};

}	// end Namespace

#include "ScPostConfig.h"
#endif

