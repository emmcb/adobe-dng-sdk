/**************************************************************************
*
* ADOBE CONFIDENTIAL
* ___________________
*
*  Copyright 1998 - 2006 Adobe Systems Incorporated
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
#ifndef _ScSparseArray_h
#define _ScSparseArray_h

#include "ScSimpleArray.h"
#include "ScPreConfig.h"

namespace ScCore
{

struct ArrayItem;

/**
An array with intptr_t indexes. Internally, this class is a hash table
with integers as the key values. It is very similar to the HashTable
class. You can store any data into the table. The data does not depend
on being derived from Root. The templatized classes TISparseArray and
TSparseArray work with pointers instead of intptr_t data.
@see HashTable
*/

class SC_API SparseArray : public Heap
{
protected:
	scint32_t     		mElementCount;
	SimpleArray			mElements;

    ArrayItem*			findItem (intptr_t key) const;
			void		smartShrink();
			void		smartGrow();
public:
	/// The ctor does not allocate any memory yet.
					   	SparseArray();
	/// The copy ctor creates an independent copy.
						SparseArray (const SparseArray&);
						~SparseArray();
	/// The assignment operator creates an independent copy.
			SparseArray& operator= (const SparseArray&);
   	/// merge another table into this table
   			SparseArray& operator+= (const SparseArray& r);
	/// Compare the contents of this hash table to the given hash table.
			bool		operator== (const SparseArray& r) const;
	///
	inline	bool		operator!= (const SparseArray& r) const { return !operator== (r); }
	/// Return the number of elements
   	inline	scint32_t		length() const { return mElementCount; }
	/// Erase all keys and clear the array. The objects are not erased.
   			void		erase();
	/// Retrieve an object by key.
	inline	intptr_t	operator[] (intptr_t key) const { return find (key); }
	/**
	Retrieve an object by key.
	@param		key		the key
	@return				the object or null
	*/
			intptr_t	find (intptr_t key) const;
	/**
	Check whether a key is present.
	@param		key		the key
	@return				true if present
	*/
   			bool		hasKey (intptr_t key) const;
	/**
	Find the key for an object. Seqential scan.
	@param	data		the data
	@return				the key or -1 (you should call hasKey (-1) to verify)
	*/
   			intptr_t	findKey (intptr_t data) const;
	/**
	Add a new data element to the table.
	@param		key		the key
	@param		data	the data
	*/
			void 		set (intptr_t key, intptr_t data);
	/**
	Remove all keys for a data element.
	@param	data		the data to remove the keys for
	*/
			void		removeData (intptr_t data);
	/**
	Remove a key and return the data stored under this key.
	@param		key		the key
	@return				the data found
	*/
			intptr_t 	remove (intptr_t key);
	/**
	Fill an array with all data contained in this table.
	If some data is stored under multiple keys, it will only appear once.
	@param	array		the array to fill
	*/
			void		getData (SimpleArray& array) const;
	/**
	Fill an array with all keys contained in this table.
	@param	array		the array to fill
	*/
			void		getKeys (SimpleArray& array) const;
	/**
	Iterate through every element in the array and call the supplied
	callback function with each key and data element. The second argument
	to the iterator fucntion points to the data location. During the iteration,
	elementes must not be deleted directly. Instead, return false inside
	the callback function to have the current key deleted. The object that
	is stored in the key is not deleted.
	*/
			void		forEach (bool (*callback) 
						(intptr_t key, intptr_t* data, intptr_t someInfo), intptr_t someInfo = 0);
};

/**
The templatized array class casts the data to a special class.
It also provides a releaseObjects() method to release the array 
contents. This template works with all classes that have addRef()
and release() methods.
*/

template <class T>
class TISparseArray : public SparseArray
{
	static  bool		release (intptr_t, T** p, intptr_t) { (*p)->release(); return true; }
public:
						TISparseArray() : SparseArray() {}
						TISparseArray (const TISparseArray<T>& r) : SparseArray ((const SparseArray&) r) {}
   	inline TISparseArray<T>& operator= (const TISparseArray<T>& r) { SparseArray::operator= ((const SparseArray&) r); return *this; }
   	inline TISparseArray<T>& operator+= (const TISparseArray<T>& r) { SparseArray::operator+= ((const SparseArray&) r); return *this; }
	inline T*			operator[] (intptr_t key) const { return (T*) SparseArray::find (key); }
    inline T*			find (intptr_t key) const { return (T*) SparseArray::find (key); }
	inline intptr_t		findKey (const T* obj) const { return SparseArray::findKey ((intptr_t) obj); }
    inline void			set (intptr_t key, const T* data) { SparseArray::set (key, (intptr_t) data); }
	inline void			removeData (const T* data) { SparseArray::removeData ((intptr_t) data); }
    inline T*			remove (intptr_t key) { return (T*) SparseArray::remove (key); }
	inline void			getObjects (TISimpleArray<T>& data) const { SparseArray::getData ((SimpleArray&) data); }
	inline void			forEach (bool (*callback) (intptr_t key, T** data, intptr_t), intptr_t i = 0) 
						{ SparseArray::forEach ((bool (*) (intptr_t, intptr_t*, intptr_t)) callback, i); }
	inline void			releaseObjects() { forEach (release); }
};

/**
The templatized array class casts the data to a special class.
It also provides a deleteObjects() method to delete the array 
contents. deleteObjects() does not check for duplicate pointers.
*/
template <class T>
class TSparseArray : public SparseArray
{
	static  bool		destroy (intptr_t, T** p, intptr_t) { delete *p; *p = NULL; return true; }
public:
						TSparseArray() : SparseArray() {}
						TSparseArray (const TSparseArray<T>& r) : SparseArray ((const SparseArray&) r) {}
   	inline TSparseArray<T>& operator= (const TSparseArray<T>& r) { SparseArray::operator= ((const SparseArray&) r); return *this; }
   	inline TSparseArray<T>& operator+= (const TSparseArray<T>& r) { SparseArray::operator+= ((const SparseArray&) r); return *this; }
	inline T*			operator[] (intptr_t key) const { return (T*) SparseArray::find (key); }
    inline T*			find (intptr_t key) const { return (T*) SparseArray::find (key); }
	inline intptr_t		findKey (const T* obj) const { return SparseArray::findKey ((intptr_t) obj); }
    inline void			set (intptr_t key, const T* data) { SparseArray::set (key, (intptr_t) data); }
	inline void			removeData (const T* data) { SparseArray::removeData ((intptr_t) data); }
    inline T*			remove (intptr_t key) { return (T*) SparseArray::remove (key); }
	inline void			getObjects (TSimpleArray<T>& data) const { SparseArray::getData ((SimpleArray&) data); }
	inline void			forEach (bool (*callback) (intptr_t key, T** data, intptr_t), intptr_t i = 0) 
						{ SparseArray::forEach ((bool (*) (intptr_t, intptr_t*, intptr_t)) callback, i); }
	inline void			deleteObjects() { forEach (destroy); }
};

}	// end Namespace

#include "ScPostConfig.h"

#endif

