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
#ifndef _ScSimpleArray_h
#define _ScSimpleArray_h

#include "ScRoot.h"
#include "ScPreConfig.h"

namespace ScCore
{

struct SimpleArrayData
{
	scint32_t		mRefCount;			// the reference count
	scint32_t 		mElementCount;		// the number of elements
	scint32_t 		mSize;				// the array size (0x80000000 set if unique is active)
	scint32_t		mIncrement;			// the array increment
	intptr_t		mOORElement;		// dummy element to be returned for out-of-range references
	union {
	intptr_t 		mElements[16];		// the elements as integers; 16 is used for debugging purposes
	void*			mData[16];			// the elements as raw data; 16 is used for debugging purposes
	Root*			mObjects[16];		// the elements as based objects; 16 is used for debugging purposes
	};
};

/**
The general-purpose array class contains intptr_t values which are casted to pointers
in derived classes. This data can be inserted, deleted or searched for. It supports 
a default mIncrement of 8 elements to conserve memory space. This mIncrement can be 
set on a per-instance basis. The elements may also be null. 
The templatized TISimpleArray and TSimpleArray classes work with pointers instead 
of intptr_t data. Array data is reference counted, so assigment and copying is very fast.
When using an array in multiple threads, you should call makeUnique(). 
This method turns off data refcounting for the affected instance, making it 
easier to work on that array with multiple threads. When modifying the array, 
you should always use Critical Sections anyway.
*/

class SC_API SimpleArray : public Heap
{
	void				unique();
protected:
	SimpleArrayData*	mData;
public:
	/// The constructor does not allocate memory.
						SimpleArray();
	/**
	The copy constructor increases the refcount. The objects stored in deriveds of this
	array are not copied, so be careful not to delete the objects twice! If makeUnique()
	has been called on 
	*/
						SimpleArray (const SimpleArray&);
	/// The destructor does not delete the values inside the array.
						~SimpleArray();
	/**
	Make the array data unique. Calling this method ensures that the data in this
	array is not refcounted, and that the data will be copied instead of refcounted 
	when used in a copy constructor or assignment operator.
	*/
			void		makeUnique();
	/**
	The assignment operator increases the refcount. The objects stored in deriveds of this
	array are not copied, so be careful not to delete the objects twice!
	*/
			SimpleArray& operator= (const SimpleArray&);
	/// Compare the contents of this hash table to the given hash table.
			bool		operator== (const SimpleArray& r) const;
	///
	inline	bool		operator!= (const SimpleArray& r) const { return !operator== (r); }
	/// Retrieve the current mIncrement.
	inline	scint32_t		getIncrement() const { return mData->mIncrement; }
	/// Set the current increment.
			void		setIncrement (scint32_t inc);
	/// Erase the array.
	inline	void		erase() { setSize (0); }
	/// Zero out the contents of the array without changing its size.
			void		clear();
	/**
	Expand or shrink the array to a certain size using the current mIncrement.
	@param	elements	the size
	*/
			void		setSize (scint32_t elements);
	/**
	Find the index of a value.
	@param	val			the value
	@return				the index or -1 if not found
	*/
   			scint32_t 	indexOf (intptr_t val) const;
   	/**
   	Copy a part of this array to another array.
   	@param	start		the first element to copy
   	@param	size		the number of elements (<0: copy all)
   	@return				the new array.
   	*/
   			SimpleArray	copy (scint32_t start, scint32_t size = -1) const;
	/**
	Insert null values. If the index is < 0, the value is
	inserted at the beginning; if it is > the size, the value
	is appended to the array.
	@param	index		the position
	@param	gapSize		the gap size
	*/
			void 		insertGap (scint32_t index, scint32_t gapSize);
	/**
	Insert a value. If the index is < 0, the value is
	inserted at the beginning; if it is > the size, the value
	is appended to the array.
	@param	val			the value to insert
	@param	index		the position
	*/
			void 		insert (intptr_t val, scint32_t index);
	/**
	Append a value.
	@param	val			the value to append
	*/
			void 		append (intptr_t val);
	/**
	Append the contents of the given array to the array.
	@param	obj			the array whose contents to append
	*/
	inline	void 		append (const SimpleArray& obj) { *this += obj; }
	/**
	Remove a range of data. If the index is < 0, the values are
	removed at the beginning; if it is > the size, nothing happens.
	@param	index		the position
	@param	gapSize		the gap size
	*/
			void 		removeGap (scint32_t index, scint32_t gapSize);
	/**
	Remove the value and shrink the array.
	@param	val			the value to be removed
	*/
			void 		remove (intptr_t val);
	/**
	Remove the value at the given index and shrink the array.
	@param	index		the index where to remove the value
	@return				the value removed
	*/
			intptr_t	removeAt (scint32_t index);
	/**
	Merge the contents of the given array with this array. If
	an element of the given array exists, it is not copied into this array.
	@param	array		the array to merge
	@param	mergeAll	also merge 0 or null values
	*/
			void		merge (const SimpleArray& array, bool mergeAll = true);
	/**
	Retrieve the element count.
	@return				the element count
	*/
	inline	scint32_t 	length() const { return mData->mElementCount; }
	/**
	Access the raw data of the array
	@return				the data pointer (may be null)
	*/
	inline	const intptr_t* getData() const { return (const intptr_t*) mData->mElements; }
	/**
	Access an array element (lvalue).
	@param	index		the index
	@return				the element reference
	*/
	inline	intptr_t&	operator[] (scint32_t index);
	/**
	Access an array element.
	@param	index		the index
	@return				the element reference
	*/
	inline	intptr_t	operator[] (scint32_t index) const;
	/**
	Append the given value to the array.
	@param	val			the value to append
	*/
	inline	SimpleArray& operator+= (intptr_t val) { append (val); return *this; }
	/**
	Append the contents of the given array to the array.
	@param	obj			the array whose contents to append
	*/
			SimpleArray& operator+= (const SimpleArray& obj);
	/**
	Remove the value at the given index and shrink the array.
	Invalid indexes are ignored.
	@param	index		the index where to remove the value
	@return				the value removed
	*/
	inline	SimpleArray& operator-= (intptr_t val) { remove (val); return *this; }
	/**
	Iterate through every element in the array and call the supplied
	callback function with each index and data element. The second argument
	to the iterator fucntion points to the data location. 
	*/
			void		forEach (void (*callback) (scint32_t index, intptr_t* data, intptr_t someInfo), intptr_t someInfo = 0) const;
};

inline intptr_t& SimpleArray::operator[] (scint32_t index) 
{
	sc_assert (index >= 0 && index < length(), "Index out of range");
	if (mData->mRefCount > 1)
		unique();
	return (index >= 0 && index < length()) ? mData->mElements [index] : mData->mOORElement;
}

inline intptr_t SimpleArray::operator[] (scint32_t index) const
{
	sc_assert (index >= 0 && index < length(), "Index out of range");
	return (index >= 0 && index < length()) ? mData->mElements [index] : 0; 
}


/**
The templatized array class casts the data to a class derived from Refcountable.
It provides a releaseObjects() method to release the array contents. This template 
works with all classes that have addRef() and release() methods. Note that this
class does not do automatic refcounting. It only provides a releaseObjects()
method for this purpose.
*/

template <class T>
class TISimpleArray : public SimpleArray
{
public:
	inline				TISimpleArray() : SimpleArray() {}
	inline				~TISimpleArray() {}
	inline				TISimpleArray (const TISimpleArray<T>& r) : SimpleArray ((const SimpleArray&) r) {}
   	inline scint32_t		indexOf (const T* data) const { return SimpleArray::indexOf ((intptr_t) data); }
   	inline void			insert (const T* data, scint32_t i) { SimpleArray::insert ((intptr_t) data, i); }
   	inline void			append (const T* data) { SimpleArray::append ((intptr_t) data); }
   	inline void			remove (const T* data) { SimpleArray::remove ((intptr_t) data); }
	// does not decrement the refcount!
   	inline T*			removeAt (scint32_t i) { return (T*) SimpleArray::removeAt (i); }
	inline void			merge (const T& array, bool ma) { SimpleArray::merge ((const SimpleArray&) array, ma); }
    inline T**			getData() const { return (T**) mData->mElements; }
	inline T*			operator[] (scint32_t index) const { return (T*) SimpleArray::operator[] (index); }
	inline T*&			operator[] (scint32_t index) { return (T*&) SimpleArray::operator[] (index); }
	inline TISimpleArray<T>& operator=  (const TISimpleArray<T>& data) { SimpleArray::operator= ((const SimpleArray&) data); return *this; }
	inline TISimpleArray<T>& operator+= (const TISimpleArray<T>& data) { SimpleArray::operator+= ((const SimpleArray&) data); return *this; }
	inline TISimpleArray<T>& operator+= (const T* data) { append (data); return *this; }
	inline TISimpleArray<T>& operator-= (const T* data) { remove (data); return *this; }
	inline void			forEach (void (*callback) (scint32_t index, T** data, intptr_t i), intptr_t info = 0) const 
						{ SimpleArray::forEach ((void (*) (scint32_t, intptr_t*, intptr_t)) callback, info); }
	inline	void		releaseObjects() { for (int i = 0; i < length(); i++) (*this) [i]->release(); }
};

/**
The templatized array class casts the data to a special class.
It also provides a deleteObjects() method to delete the array 
contents. deleteObjects() does not check for duplicate pointers.
Remember that the copy ctor and the assignment operator cause
pointers to appear twice, so be careful about deleting objects
stored in this array.
*/
template <class T>
class TSimpleArray : public SimpleArray
{
public:
	inline				TSimpleArray() : SimpleArray() {}
	inline				TSimpleArray (const TSimpleArray<T>& r) : SimpleArray ((const SimpleArray&) r) {}
   	inline scint32_t		indexOf (const T* data) const { return SimpleArray::indexOf ((intptr_t) data); }
   	inline void			insert (const T* data, scint32_t i) { SimpleArray::insert ((intptr_t) data, i); }
   	inline void			append (const T* data) { SimpleArray::append ((intptr_t) data); }
   	inline void			remove (const T* data) { SimpleArray::remove ((intptr_t) data); }
   	inline T*			removeAt (scint32_t i) { return (T*) SimpleArray::removeAt (i); }
	inline void			merge (const T& array, bool ma) { SimpleArray::merge ((const SimpleArray&) array, ma); }
    inline T**			getData() const { return (T**) mData->mElements; }
	inline T*			operator[] (scint32_t index) const { return (T*) SimpleArray::operator[] (index); }
	inline T*&			operator[] (scint32_t index) { return (T*&) SimpleArray::operator[] (index); }
	inline TSimpleArray<T>&	operator= (const TSimpleArray<T>& data) { SimpleArray::operator= ((const SimpleArray&) data); return *this; }
	inline TSimpleArray<T>&	operator+= (const TSimpleArray<T>& data) { SimpleArray::operator+= ((const SimpleArray&) data); return *this; }
	inline TSimpleArray<T>&	operator+= (const T* data) { SimpleArray::append ((intptr_t) data); return *this; }
	inline TSimpleArray<T>&	operator-= (const T* data) { SimpleArray::remove ((intptr_t) data); return *this; }
	inline void			forEach (void (*callback) (scint32_t index, T** data, intptr_t i), intptr_t info = 0) 
						{ SimpleArray::forEach ((void (*) (scint32_t, intptr_t*, intptr_t)) callback, info); }
	inline	void		deleteObjects() { for (int i = 0; i < length(); i++) delete (*this) [i]; }
	inline	void		quickDeleteObjects() { deleteObjects(); }
};

}		// end Namespace

#include "ScPostConfig.h"

#endif

