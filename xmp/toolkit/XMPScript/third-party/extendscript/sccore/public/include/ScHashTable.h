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
#ifndef _ScHashTable_h
#define _ScHashTable_h

#include "ScSimpleArray.h"
#include "ScString.h"
#include "ScPreConfig.h"

namespace ScCore
{

class HashItem;

/**
The hash table class allows the storage of objects under a string key.
You can store any data into the table. The data does not depend
on being derived from Root. The templatized THashTable class works 
with pointers instead of intptr_t data.
<p>
All methods that accept a string key also accept a second boolean which, 
if set to true, causes the hash table to do a case insensitive lookup.
*/

class SC_API HashTable : public Heap
{
protected:
	scint32_t     		mElementCount;
	SimpleArray			mElements;

    HashItem*			findItem (const String& key, bool nc) const;
	void      			smartShrink();
	void      			smartGrow();
public:
	/// The ctor does not allocate any memory yet.
					   	HashTable();
	/// The copy ctor creates an independent copy.
						HashTable (const HashTable&);
   						~HashTable();
	/// The assignment operator creates an independent copy.
			HashTable& operator= (const HashTable&);
   	/// merge another table into this table.
   			HashTable& operator+= (const HashTable& r);
	/// Compare the contents of this hash table to the given hash table.
			bool		operator== (const HashTable& r) const;
	///
	inline	bool		operator!= (const HashTable& r) const { return !operator== (r); }
	/// Return the number of elements
   	inline	scint32_t		length() const { return mElementCount; }
	/// Erase all keys and empty the table. The data is not deleted.
   			void		erase();
	/// Retrieve a data element by key.
	inline	intptr_t	operator[] (const String& key) const { return find (key); }
	/**
	Retrieve an object by key
	@param	key			the key
	@param	nc			set to true for case-insensitive lookup
	@return				the object or null
	*/
			intptr_t	find (const String& key, bool nc = false) const;
	/**
	Check whether a key is present.
	@param	key			the key
	@param	nc			set to true for case-insensitive lookup
	@return				true if present
	*/
   			bool		hasKey (const String& key, bool nc = false) const;
	/**
	Find the key for a data element. Seqential scan.
	@param	data		the data
	@return				the key or null
	*/
   	const String*		findKey (intptr_t data) const;
	/**
	Add a new data element to the table.
	@param	key			the key
	@param	data		the data
	@param	nc			set to true for case-insensitive lookup
	@return				the reference to the internal string key
	*/
	const String&		set	(const String& key, intptr_t data, bool nc = false);
	/**
	Remove all keys for a data element.
	@param	data		the data to remove the keys for
	*/
			void		removeData (intptr_t data);
	/**
	Remove a key and return the data stored under this key.
	@param	key			the key
	@param	nc			set to true for case-insensitive lookup
	@return				the data
	*/
			intptr_t	remove (const String& key, bool nc = false);
	/**
	Fill an array with all data contained in this table.
	If a data element is stored under multiple keys, it 
	will only appear once.
	@param	array		the array to fill
	*/
			void		getData (SimpleArray& array) const;
	/**
	Fill an array with all keys contained in this table.
	The keys belong to the hash table, so a key may become invalid 
	when the entry is deleted.
	@param	array		the array to fill
	*/
			void		getKeys (TSimpleArray<const String>& array) const;
	/**
	Iterate through every element in the hash table and call the supplied
	callback function with each key and data element. The second argument
	to the iterator fucntion points to the data location. During the iteration,
	elementes must not be deleted directly. Instead, return false inside
	the callback function to have the current key deleted. The object that
	is stored in the key is not deleted.
	*/
			void		forEach (bool (*callback) (const String& key, intptr_t* data, intptr_t someInfo), intptr_t someInfo = 0);
};

/**
The templatized TIHashTable class works with pointers instead of intptr_t data. It also features
a releaseObjects() method that assumes the existence of a release() method at the contained 
elements. Therefore, this class is suitable to contains LiveBase derivates, although it
does not implement refcounting by itself.
*/

template <class T>
class TIHashTable : public HashTable
{
	static  bool			release (const String&, T** p, intptr_t) { (*p)->release(); return true; }
public:
							TIHashTable() : HashTable() {}
							TIHashTable (const TIHashTable<T>& r) : HashTable ((const HashTable&) r) {}
   	inline TIHashTable<T>&	operator= (const TIHashTable<T>& r) { HashTable::operator= ((const HashTable&) r); return *this; }
   	inline TIHashTable<T>&	operator+= (const TIHashTable<T>& r) { HashTable::operator+= ((const HashTable&) r); return *this; }
	inline T*				operator[] (const String& key) const { return (T*) HashTable::find (key); }
    inline T*				find (const String& key, bool nc = false) const { return (T*) HashTable::find (key, nc); }
   	inline const String* findKey (const T* obj) const { return HashTable::findKey ((intptr_t) obj); }
    inline const String& set (const String& key, const T* obj, bool nc = false) { return HashTable::set (key, (intptr_t) obj, nc); }
	inline void				removeData (const T* obj) { HashTable::removeData ((intptr_t) obj); }
    inline T*				remove (const String& key, bool nc = false) { return (T*) HashTable::remove (key, nc); }
	inline void				getObjects (TISimpleArray<T>& data) const { HashTable::getData ((SimpleArray&) data); }
	inline void				forEach (bool (*callback) (const String& key, T** data, intptr_t i), intptr_t info = 0) 
							{ HashTable::forEach ((bool (*) (const String&, intptr_t*, intptr_t)) callback, info); }
	inline void				releaseObjects() { forEach (release); }
};

/**
The templatized THashTable class works with pointers instead of intptr_t data. It also features
a deleteObjects() method that destroys all objects inside this array. It cannot be used for
LiveBase derivates, because these objects are refcounted.
*/

template <class T>
class THashTable : public HashTable
{
	static  bool			destroy (const String&, T** p, intptr_t) { delete *p; *p = NULL; return true; }
public:
							THashTable() : HashTable() {}
							THashTable (const THashTable<T>& r) : HashTable ((const HashTable&) r) {}
   	inline THashTable<T>&	operator= (const THashTable<T>& r) { HashTable::operator= ((const HashTable&) r); return *this; }
   	inline THashTable<T>&	operator+= (const THashTable<T>& r) { HashTable::operator+= ((const HashTable&) r); return *this; }
	inline T*				operator[] (const String& key) const { return (T*) HashTable::find (key); }
    inline T*				find (const String& key, bool nc = false) const { return (T*) HashTable::find (key, nc); }
   	inline const String* findKey (const T* obj) const { return HashTable::findKey ((intptr_t) obj); }
    inline const String& set (const String& key, const T* obj, bool nc = false) { return HashTable::set (key, (intptr_t) obj, nc); }
	inline void				removeData (const T* obj) { HashTable::removeData ((intptr_t) obj); }
    inline T*				remove (const String& key, bool nc = false) { return (T*) HashTable::remove (key, nc); }
	inline void				getObjects (TSimpleArray<T>& data) const { HashTable::getData ((SimpleArray&) data); }
	inline void				forEach (bool (*callback) (const String& key, T** data, intptr_t i), intptr_t info = 0) 
							{ HashTable::forEach ((bool (*) (const String&, intptr_t*, intptr_t)) callback, info); }
	inline void				deleteObjects() { forEach (destroy); }
};

}	// end Namespace

#include "ScPostConfig.h"

#endif

