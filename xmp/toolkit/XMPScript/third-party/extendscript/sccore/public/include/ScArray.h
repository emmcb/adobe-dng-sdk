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
#ifndef _ScArray_h
#define _ScArray_h

#include "ScSimpleArray.h"
#include "ScSparseArray.h"
#include "ScVariant.h"
#include "ScLiveBase.h"
#include "ScPreConfig.h"

extern void ScArraySetup (bool);

namespace ScCore
{

class LiveObject;
class FileSpec;
class Lock;

/**
The base class for arrays of Variants permits internal usage of different implementations.
For example, Dispatcher argument blocks contain instances of this class. The base class
is pure virtual.
*/

class SC_API BasicArray : public Root
{
protected:
							BasicArray();
	virtual					~BasicArray();
public:
	/**
	Retrieve the element count.
	@return					the element count
	*/
	virtual	scuint32_t 		length() const = 0;
	/**
	Is this array element defined at all?
	*/
	virtual		bool		isDefined (scuint32_t index) const = 0;
	/**
	Get the Variant stored at the given index. If the index is out of range,
	or if there is no Variant at the given position, return NULL. Note that
	the Variant that the returned pointer points to may be modified.
	@param	index			the index
	@return					the element reference, or NULL if it does not exist
	*/
	virtual	Variant*		get (scuint32_t index) const = 0;
	/**
	Access an array element (rvalue). For undefined or out-of-range values,
	a placeholder Variant with the value Undefined is returned which should 
	not be altered.
	@param	index			the index
	@return					the element reference
	*/
			const Variant&	operator[] (scuint32_t index) const;
	/**
	Create a string representation of this object. The method creates
	an array representation like "1,2,3..." of up to 1000 elements.
	*/
			void			toString (String& result) const;
};

/**
This class is an array of Variant instances. The first 1024 elements are stored in a linear
array, while indexes > 1024 are stored in a hash table. If an initial size is given that
exceeds 1024, the array is considered to be huge, and the linear part contains only 16 elements.
The const [] operator returns a Variant set to "undefined" for array indexes that are out of 
bounds, while the non-const [] operator creates a new element if the element does not exist, 
and adjusts the length of the array to reflect the new length. The setLength() method changes 
the length of the array; if the array is shrunk, excess elements are discarded. You can insert 
a gap or shrink the array at a certain index, and you can delete single array elements.
<p>
The class is written as copy-on-write; the data is reference counted, so copy-constructing an
array is very fast. When using an array within multiple threads, you should call makeUnique()
to make sure that the array is not refcounted. Of course, you will need to use Critical Sections 
if you want to modify an array shared among threads.
<p>
NOTE: This class is the only ScCore class that deals with unsigned lengths and indexes.
All other classes work with signed 32-bit values.
*/

class SC_API Array : public BasicArray
{
	friend void ::ScArraySetup (bool);
	class Data : public Heap
	{
		struct Vector
		{
			scuint32_t 			mLength;			// the number of elements
			scuint32_t 			mSize;				// the array size
			Variant				mData [1];			// the data
		};
								Data (const Data&);
	public:
		Vector*					mLinear;			// for the first mThreshold indexes
		TSparseArray<Variant>*	mSparse;			// indexes >= mThreshold go here
		scint32_t					mRefCount;			// the internal refcount
		scuint32_t				mLength;			// the logical length
		scuint32_t				mThreshold;			// index >= this one go into a sparse array
								Data (scuint32_t);
								Data (const Data&, scuint32_t);
								~Data();
		void					create (scuint32_t len);
		void					grow (scuint32_t len);
	};

	Data*					mData;
	void					unique (scuint32_t);
	// prevent automatic promotion of a ptr to an operator+=(bool)
	Array&					operator+= (const void* val);
	Array&					operator+= (void* val);
public:
	/**
	The constructor does not allocate memory if the length is 0.
	If the length is non-zero, the length is used to create a linear
	array of the given length, setting the bottom index of data stored
	in a sparse array to either that value or 1024, whichever is the
	higher. The default constructor assumes an initial length of 0.
	*/
							Array ();
	///
							Array (scuint32_t initLength);
	/// The copy constructor adds one to the refcount of the data of the source array.
							Array (const Array&);
	virtual					~Array();
	/**
	Make the array data unique. Calling this method ensures that the data in this
	array is not refcounted, and that the data will be copied instead of refcounted 
	when used in a copy constructor or assignment operator.
	*/
			void			makeUnique();
	/// The assignment operator adds one to the refcount of the data of the source array.
			Array&			operator= (const Array&);
	/// The comparison operator compares the array contents.
			bool			operator== (const Array&) const;
	///
	inline	bool			operator!= (const Array& r) const { return !operator== (r); }
	/**
	Retrieve the element count.
	@return					the element count
	*/
	virtual	scuint32_t 		length() const;
	/// Erase the array. The call decrements the data refcount and releases it if it reaches zero.
	inline	void			erase() { setLength (0); }
	/**
	Expand or shrink the array to a certain size. The call first makes sure that
	the refcount of the data is one before changing its size. Not thread safe.
	@param	elements		the size
	*/
			void			setLength (scuint32_t elements);
	/**
	Get the data vector of this array. This call returns a pointer only if there
	is no data in the sparse array section, i.e. no elements have been added
	beyond the initial size of the index of 1024, whichever is highest!
	*/
			Variant*		getData() const;
	/**
	Insert a range of values into the array. The array is not populated with
	values. The internal straight array may be converted to a sparse
	array. The length is updated accordingly. The call first makes sure that
	the refcount of the data is one before changing its size. Not thread safe.
	@param	start			the starting index
	@param	count			the number of elements to insert
	*/
			void			insertAt (scuint32_t at, scuint32_t count);
	/**
	Remove a range of values from the array. The length is updated accordingly.
	The call first makes sure that the refcount of the data is one before changing 
	its size. Not thread safe.
	@param	start			the starting index
	@param	count			the number of elements to remove
	*/
			void			removeAt (scuint32_t at, scuint32_t count);
	/**
	Delete the value at the given index and shrink the array. The call first makes sure that
	the refcount of the data is one before changing its size. Not thread safe.
	@param	index			the index where to remove the value
	*/
			void			deleteAt (scuint32_t index);
	/**
	Is this array element defined at all?
	*/
	virtual	bool			isDefined (scuint32_t index) const;
	/**
	Access an array element (lvalue). Not thread safe.
	To access the array with the array as rvalue, the same operator has been defined
	in the BasicArray class as being const, returning const.
	@param	index			the index
	@return					the element reference
	*/
			Variant&		operator[] (scuint32_t index);
	/**
	Access an array element (rvalue). Not thread safe.
	@param	index			the index
	@return					the element reference
	*/
	using					BasicArray::operator[];
	/**
	Get the Variant stored at the given index. If the index is out of range,
	or if there is no Variant at the given position, return NULL. Note that
	the Variant that the returned pointer points to may be modified.
	@param	index			the index
	@return					the element reference, or NULL if it does not exist
	*/
	virtual	Variant*		get (scuint32_t index) const;
	/**
	Get the Variant stored at the given index. If the index is out of range,
	or if there is no Variant at the given position, return NULL. Force creation 
	of the Variant if not present, and always return a pointer. This may alter 
	the Array's length.
	@param	index			the index
	@return					the element reference
	*/
			Variant&		safeGet (scuint32_t index);
	/**
	Parse the contents of the string and fill the array with the results.
	Use this call to setup the array with multiple values at once.
	The string contains values separated by commas. The value may be a number,
	a quoted string following JavaScript guidelines (including backslash
	escape sequences), "true", "false" or "null". The name may be quoted 
	to define properties that do not follow JavaScript syntax. The value may 
	also be a nested object. Use curly brackets to declare a nested object and to
	group its properties. Finally, you can use square brackets to create a nested 
	array. The values are appended to any existing data. The instance uses
	the LiveObject factory to create objects of a certain class; if this
	creation fails, it returns a runtime error kErrNoCtor, except for objects 
	without a class name; here, it will just create standard LiveObject objects 
	of the class "Object". The factory will be called with the supplied language 
	specifier. Not thread safe.
	@param	text			the text to parse
	@param	lang			the language ID to be forwarded to the LiveObject factory
	@return					false on syntax errors.
	*/
			bool			parse (const String& text, scint32_t lang = kJavaScriptLanguage);
	/// Add a boolean value. Not thread safe.
	inline	Array&	operator+= (bool val)				{ (*this) [length()].setBool (val); return *this; }
	/// Add an integer value. Not thread safe.
	inline	Array&	operator+= (scint32_t val)			{ (*this) [length()].setInteger (val); return *this; }
	/// Add a numeric value. Not thread safe.
	inline	Array&	operator+= (double val)				{ (*this) [length()].setDouble (val); return *this; }
	/// Add a MBCS string value using the default encoder. Not thread safe.
	inline	Array&	operator+= (const char* val)		{ (*this) [length()].setString (val); return *this; }
	/// Add a string value. Not thread safe.
	inline	Array&	operator+= (const String& val)		{ (*this) [length()].setString (val); return *this; }
	/// Add a LiveObject. Not thread safe.
	inline	Array&	operator+= (LiveObject* val)		{ (*this) [length()].setLiveObject (val); return *this; }
	/// Add a LiveObject. Not thread safe.
	inline	Array&	operator+= (LiveObject& val)		{ (*this) [length()].setLiveObject (val); return *this; }
	/// Add a value. Not thread safe.
	inline	Array&	operator+= (const Variant& val)		{ (*this) [length()] = val;	 return *this; }
	/// Add a file. Not thread safe.
	inline	Array&	operator+= (FileSpec& val)			{ (*this) [length()].setFileSpec (val); return *this; }
	/// Add an array. Not thread safe.
	inline	Array&	operator+= (Array& val)				{ (*this) [length()].setArray (val); return *this; }
	/// Add a rectangle. Not thread safe.
	inline	Array&	operator+= (const Rect& val)		{ (*this) [length()].setRect (val); return *this; }
	/// Add a coordinate pair. Not thread safe.
	inline	Array&	operator+= (const Point& val)		{ (*this) [length()].setPoint (val); return *this; }
#if defined (SC_MAC) || defined (SC_WIN)
	/// Needed for Win/Mac to implement value += integer constant. Not thread safe.
	inline	Array&	operator= (int val)					{ (*this) [length()].setInteger (scint32_t (val)); return *this; }
#endif
};

}	// end Namespace

#include "ScPostConfig.h"

#endif

