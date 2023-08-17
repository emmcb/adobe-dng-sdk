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
#ifndef _ScVariant_h
#define _ScVariant_h

// The VariantType enum has been moved to ScVariantType.h
#include "ScVariantType.h"
#include "ScString.h"
#include "ScHashTable.h"
#include "ScClassInfo.h"
#include "ScLiveBase.h"
#include "ScInterfacePtr.h"
#include "ScPreConfig.h"

namespace ScScript
{
	class Engine;
	class Object;
}

namespace ScCore
{

class LiveObject;
class LiveCollection;
class Point;
class Rect;
class Cloneable;
class FileSpec;
class Array;
class RegExp;
class UnitValue;
class Error;

/**
Enumeration flags for numeric Variant attributes.
*/
enum
{
	/// normal number (except for -0)
	kNumNormal		= 0x0001,
	/// Signed integer number
	kNumInteger		= 0x0002,
	/// Unsigned integer number
	kNumUInteger	= 0x0004,
	/// NaN
	kNumNaN			= 0x0008,
	/// Positive Infinity
	kNumPosInfinity	= 0x0010,
	/// Negative Infinity
	kNumNegInfinity	= 0x0020,
	/// Minus zero
	kNumMinus0		= 0x0040,
	/// Number has been checked for being integer
	kNumChecked		= 0x0080,
	/// Number has been printed
	kNumToText		= 0x0100,
	/// This is part of a property; set by the ES engine.
	kJSProperty		= 0x0200,
	/// This flags a Variant inside an engine to be a const.
	kJSConst		= 0x0400,
	/// Finite number mask
	kNumFinite		= (kNumNormal | kNumInteger | kNumUInteger | kNumMinus0),
	/// Infinite number mask
	kNumInfinite 	= (kNumNaN | kNumPosInfinity | kNumNegInfinity),
	/// Mask for all number bits
	kNumValidMask	= (kNumFinite | kNumInfinite),
	/// Mask for all number bits
	kNumBitMask		= 0x01FF,
	/// Mask for all numeric attribute bits
	kNumAttrBitMask	= 0x00FF
};


/**
The Variant class is a wrapper for all kinds of scalar data. Numerous methods
exist to extract the data in a desired format or to convert the data into a
different format. For objects, a value may hold an object ID. This ID is mapped
to an object inside the engine. The type sOBJECT consists of two parts: an object 
ID and a class name. The class name can be retrieved with getString(), and the object 
ID can be retrieved with getInteger(). Optionally, a data object pointer may be added 
that contains your private data. This data pointer belongs the object as scint32_t as
the Javascript object that the data pointer is attached to does not physically exists.
When the JavaScript object is present, the data pointer belongs to that object and will
be destroyed when the JavaScript object is destroyed. This may cause unwanted memory
leaks when a JavaScript object is not created as expected. Remember also that the copy
constructors as well as the assignment operator of this class simply copy the data pointer.
<p>
Variant instances may be kept around as scint32_t as desired, except for values referencing
objects. If the value has been filled in by the engine (maybe it was the iThis part of
the dispatcher data block, or it has been filled in by findObject), the value contains
an object pointer that may become invalid when the object is garbage-collected. You
should, therefore, never keep Variants containing object references!
<P>
Conversion rules apply to the following types according to ECMA-262. The conversion
rules for objects cannot be used on object IDs outside the engine. There is, therefore,
no way to convert an object to a string or a  number. These conversions return the
object class name or the object ID. 
<p>
<b>bool</b>: undefined and null is false. Numbers are false if they are 0 or NaN.
Strings are false if they are empty. Objects are always true.
<p>
<b>number</b>: undefined is NaN. Null is 0. Bools are 0 or 1. 
Strings are converted, while the special values "Infinity" and "-Inifinity" are 
recognized. If the string does not contain a valid number, the result is NaN. Empty 
strings are 0. Octal values (values starting with 0) are treated as decimals. 
Objects return their ID; if the object is a LiveObject, it returns the result of
the method valueOf() if present.
<p>
<b>string</b>: Undefined is "undefined", null is "null", bools are "false" or "true". 
Numbers are converted to scientific format if they are < 10e-6 or > 10e21. Objects 
return their class name; if the object is a LiveObject, it returns the result of the 
method toString() if present.
<p>
<b>Point</b>: a two-element array can be converted to a coordinate pair and back.
<p>
<b>Rect</b>: a four-element array can be converted to a Rect and back.
<p>
<b>Array</b>: an array can be converted to a coordinate pair, a Rect,
a LiveArray object and back. All other data types are converted to a string and 
parsed into the array.
<p>
<b>RegExp</b>: All data types are converted to string before being converted to a
regular expression object, and conversion back is via the regular expression's
string representation.
<p>
<b>Unit values</b>: Strings can be parsed into unit values, and unit values can be 
converted to strings; for numeric conversions and comparisons, the unit value is 
converted to its base unit. Numbers are converted to units with kTypeNoType as unit.
<p><b>other core objects</b>: currently, these cannot be converted, except for a string;
otherwise, the result is kNull.
*/

class SC_API Variant : public Heap
{
	struct ObjRef {
		Root*				data;
		scint32_t			objID;
	};
private:
		// disallow promotion of pointers to Booleans
							Variant (void*);
							Variant (const void*);
		Variant&			operator= (void*);
		Variant&			operator= (const void*);
protected:
#if defined(SC_HPUX) && !defined(HPUX_ACC)
                //  Some architectures (HP) require aligned values and the MSC specific __unalign 
                // keyword doesn't have a comparable definition in GCC, so must force the 
                // alignment of the union below.
                double j_force_align;
#endif	        
	union {
		double				d;
		LiveObject*			liveObj;
		Point*				point;
		Rect*				rect;
		Array*				array;
		RegExp*				regexp;
		FileSpec*			fileSpec;
		UnitValue*			unit;
		bool				b;
		ObjRef				ref;
	};
	ScScript::Object*		obj;
	mutable String*			text;
	VariantType				mType;
	mutable scint16_t		attrs;

			void			doErase();
			void			doSetNumAttrs() const;
			double			doGetDouble() const;
			void			doToString (scint32_t base) const;
			bool			doIsFinite() const;
			bool			doIsNaN() const;
			bool			doIsMinus0() const;
	// Some ctors we do not want to see called
							Variant (VariantType);
public:
	/// Flags to control the scanning of numbers.
	enum { 
		/// Scan all of the string. Returns NaN if there are characters behind the number.
		kAll			 = 0,
		/// Scan up to white space behind the number. Returns NaN if there is not white space behind.
		kWhitespace		 = 1,
		/// Scan up to the first non-number character. Never returns an error.
		kNoErrors		 = 2,
		/// Do not treat a 0 as prefix as an octal value. Add this value to other values.
		kNoOctals		 = 0x100,
		/// Do not attempt to scan octal or hex values. Add this value to other values.
		kNoHex			 = 0x200,
		__Flags_32bit__	 = 0x7FFFFFFF
	};
	/// Possible arguments to the getString() method with two arguments:
	enum {
		/// Convert the number to a string according to ECMA rules.
		kECMA			= 0,
		/// Convert the number to a string forcing decimal point notation
		kFixed			= 1,
		/// Convert the number to a string forcing scientific notation
		kScientific		= 2,
		__Args_32bit__	= 0x7FFFFFFF
	};
	/**
	Scan an integer value. 
	This method recognizes octal and hex prefixes. If there is any base supplied,
	the conversion is forced to that base, ignoring prefixes; otherwise, the base is 10
	and prefixes (0 for octal and 0x for hex) are recognized. Conversion skips
	white space and stops at the first character behind the number. In case of errors, the
	return value is NaN. Any remaining text is returned in remainder if supplied.
	*/
	static	double			scanInteger (const String& text, scint32_t base = 0, scint32_t mode = kAll, String* remainder = NULL);
	/**
	Scan a floating-point value. Conversion skips white space and stops at the first character 
	behind the number. In case of errors, the return value is NaN. Any remaining text is 
	returned in remainder if supplied.
	*/
	static	double			scanDouble (const String& text, scint32_t mode = kAll, String* remainder = NULL);
	/// An undefined value
	explicit	inline		Variant() : obj(NULL), text(NULL), attrs(0), mType(kUndefined) {}
	/// A boolean value
	explicit				Variant (bool);
	/// An integer number
	explicit				Variant (scint32_t);
	/// A floating point number
	explicit				Variant (double);
	/// A string
	explicit				Variant (const String&);
	/// A MBCS string with a mandatory encoder.
	explicit				Variant (const char*, Encoder*);
	/// An ASCII string.
	explicit				Variant (const char*);
	/// An UTF-16 string.
	explicit				Variant (const scutf16_t*);
	/// A LiveObject. LiveObjects are refcounted inside the Variant object.
	explicit				Variant (LiveObject& obj, scint32_t id = 0);
	/// A LiveObject. LiveObjects are refcounted inside the Variant object. Null pointers lead to kNull.
	explicit				Variant (LiveObject* obj, scint32_t id = 0);
	/// An object reference with optional object data.
	explicit				Variant (scint32_t objID, const String& clsName, Root* data = NULL);
	/// A coordinate pair.
	explicit				Variant (const Point& r);
	/// A Rect.
	explicit				Variant (const Rect& r);
	/// A FileSpec object.
	explicit				Variant (const FileSpec& r);
	/// A regular expression.
	explicit				Variant (const RegExp&);
	/// An Array object.
	explicit				Variant (const Array& r);
	/// An UnitValue object.
	explicit				Variant (const UnitValue& r);
	/// The copy constructor.
							Variant (const Variant&);
	/// The copy constructor for a pointer.
	explicit				Variant (const Variant*);
							~Variant();
	/// Retrieve the type of the value
	inline	VariantType		getType() const { return mType; }
	/**
	Get the type of the data that is exctractable. If the variant contains
	an object, try to extract the data type of the data that this object
	contains. The method calls the various getters for the data types RegExp, 
	Point, Rect, Array and FileSpec to determine the presence of that 
	data, calling the getters in the given order. If a JavaScript object wraps
	a LiveObject that is unable to supply any know data, the returned type
	is kLiveObject. There is no attempt to determine whether an object is able
	to return simple data types such as numbers or strings. Objects are not
	evaluated (by e.g. calling their valueOf() method); they are just scanned
	for wrapping the simpler data types mentioned above.
	*/
			VariantType		getDataType() const;
	/// Is the value defined?
	inline	bool			isDefined() const { return mType != kUndefined; }
	/// Is the value NULL?
	inline	bool			isNull() const { return mType == kNull; }
	/// Set the value to NULL
			void			setNull();
	/**
	Set an object reference. Optionally, you may supply a user-defined data block
	which must be derived from Root so the engine can delete it when it deletes
	the object. Do not supply user-defined data for mutable properties unless you
	make sure that you supply the same pointer for the same object! This method creates
	the appropriate LiveObject and assigns the ID as the JavaScript ID. The LiveObject
	is created using the JavaScript language specifier.
	@param	id				the object ID. May be any positive or any negative value < -10.
							If 0, the engine would assign a new ID if you ever access it again.
	@param	className		the object class name.
	@param	data			the optional user defined data block.
	*/
			void			setObject (scint32_t id, const String& className, Root* data = NULL);
	/**
	Set a LiveObject. LiveObjects are refcounted inside the Variant object.
	*/
			void			setLiveObject (LiveObject& r, scint32_t jsID = 0);
	/**
	Set a LiveObject. LiveObjects are refcounted inside the Variant object. Null pointers lead to sNULL.
	*/
			void			setLiveObject (LiveObject* p, scint32_t jsID = 0);
	/**
	Set the numeric attributes. Call this method after setDouble() to increase performance
	if you expect multiple read accesses to this value. The method is called internally
	if needed, so this call is optional.
	*/
			void			setNumAttrs() const;
	/// Is the value a signed integer value?
	inline	bool			isInteger () const;
	/// Is the value a unsigned integer value?
	inline	bool			isUInteger () const;
	/// Is the value a finite value?
	inline	bool			isFinite() const;
	/// Is the value an invalid number?
	inline	bool			isNaN() const;
	/// is the value the special value -0?
	inline	bool			isMinus0() const;
	/// Store the special value -0
			void			setMinus0();
	/// Store NaN
			void			setNaN();
	/// Store positive Infinity
			void			setPosInfinity();
	/// Store negative Infinity
			void			setNegInfinity();
	/// Erase the value
	inline	void			erase();
	/**
	Convert the data inside the value. *X*
	Conversion to kFileSpec, kArray results in kNull.
	Conversion to kPoint or kRectangle cause an empty Point or Rect to be created. A
	conversion to kJSObject or kLiveObject is currently not possible.
	*/
			void			convert (VariantType newType);
	/// Retrieve the value as boolean
			bool			getBool() const;
	/**
	Retrieve the value as a signed integer (the lower 32 bits of the double value as integer).
	This operation may not be very fast, so make sure you save the value instead of retrieving
	it repeatedly.
	*/
			scint32_t		getInteger() const;
	/**
	Retrieve the value as an unsigned integer (the lower 32 bits of the double value as integer).
	This operation may not be very fast, so make sure you save the value instead of retrieving
	it repeatedly.
	*/
	inline	scuint32_t		getUInteger() const { return (scuint32_t) getInteger(); }
	/**
	Retrieve the value as an integer limited to the given range. This operation is not 
	very fast, so make sure you save the value instead of retrieving it repeatedly.
	*/
			scint32_t		getInteger (scint32_t min, scint32_t max) const;
	/// Retrieve the value as a floating point value. The prefix 0x is recognized.
	inline	double			getDouble() const { return (mType == kNumber) ? d : doGetDouble(); }
	/// Retrieve the value as a string, defererencing an object to a string.
			void			toString (String& target) const;
	///
			String			toString() const;
	/// Retrieve the value as a string. FOr kJSObject, the class name is returned.
	inline	const String&	getString () const { if (mType != kString) doToString (10); return *text; }
	/// Retrieve the value as a string. Apply the given base if necessary.
	inline	const String&	getString (scint32_t base) const { if (mType != kString || base != 10) doToString (base); return *text; }
	/**
	Retrieve the value as a string, applying the given rule and the given decimals.
	The rule is 0 for auto, 1 for fixed point and 2 for exponential notation.
	*/
			const String&	getString (scint32_t rule, scint32_t decimals) const;
	/// Retrieve a LiveObject if the Variant contains one.
			LiveObject*		getLiveObject() const;
	/// Retrieve a LiveCollection if the Variant contains one.
			LiveCollection* getLiveCollection() const;
	/// Retrieve a LiveObject if the Variant contains one or if one can be generated.
			bool			getAsLiveObject (InterfacePtr<LiveObject>& ref) const;
	/// Retrieve a LiveCollection if the Variant contains one or if one can be generated.
			bool			getAsLiveCollection (InterfacePtr<LiveCollection>& ref) const;
	/**
	Retrieve the ScScript engine which the object belongs to. This call returns
	a pointer to the engine only if the data type is sOBJECT and the value contains
	an actual stub object instead of a object reference.
	*/
			ScScript::Engine* getExtendScriptEngine() const;
	/// Retrieve the private object data if this is an object reference.
			Root*			getObjectData() const;
	/**
	Check whether this value contains object data that is still unassigned. After a copy
	constructor or assignment operator call, both instances contain the same object data pointer!
	*/
			bool			hasUnassignedObjectData() const;
	/// Retrieve the coordinate pair if the value contains a Rect or a LiveObject wrapping a pair.
	const	Point*			getPoint() const;
	/// Retrieve the Rect if the value contains a pair or a LiveObject wrapping a Rect.
	const	Rect*			getRect() const;
	/// Retrieve the FileSpec object if the value contains a FileSpec or a LiveObject wrapping a FileSpec.
	const	FileSpec*		getFileSpec() const;
	/// Retrieve the regular expression if the value contains a RegExp or a LiveObject wrapping a RegExp.
	const	RegExp*			getRegExp() const;
	/**
	Retrieve the array if the value contains an array, or a LiveObject that supports the kLiveIDArray
	constant. The latter is also true for ActionScript wrapper objects that represent an ActionScript
	array (Mac/Win only). Note that the call may be slow for ActionScript Array objects because the
	array elements need to be retrieved, wrapped, and copied into an internal Array object. If errors
	occur during the retrieval of the array elements, the return value is NULL. 
	Future versions of ExtendScript will return a BasicArray instead of an Array, permitting internally 
	optimized access to ActionScript arrays.
	@return					a pointer to an Array or NULL on errors
	*/
	const	Array*			getArray() const;
	/**
	This alternative to getArray() fills in an existing array. In addition to the getArray() method
	that returns an array pointer, it is also able to fill an array with the contents of a JavaScript
	array, or the contents of a LiveCollection.
	@param	array			the array to fill
	@return					true if the type was kArray, there was a LiveObject containing an array, or
							a JavaScript Array object, or a LiveCollection.
	*/
			bool			getArray (Array& array) const;
	/// Retrieve an unit value if the value contains an unit value or a LiveObject wrapping such a value.
	const	UnitValue*		getUnitValue() const;
	/**
	Set the private object data if this is an object reference. If not, the call is ignored.
	If you set the data multiple times, any previously set data will NOT be deleted if the
	variant does not contain a valid object reference. Call hasUnassignedObjectData() to find out.
	*/
			void			setObjectData (Root* data);
	/// Set the object ID if this is an object reference and the id is not reserved.
			void			setObjectID (scint32_t id);
	/// Assign a boolean value
			 void			setBool (bool n);
	/// Assign an signed integer number
		 	void			setInteger (scint32_t n);
	/// Assign an unsigned integer number
		 	void			setInteger (scuint32_t n);
	/// Assign a floating point number
		 	void			setDouble (double n);
	/// Assign a MBCS string with a mandatory encoder
		 	void			setString (const char* p, Encoder*);
	/// Assign an ASCII string
		 	void			setString (const char* p);
	/// Assign a UTF-16 string
		 	void			setString (const scutf16_t* p);
	/// Assign a string
		 	void			setString (const String& r);
	/// Assign a pair.
			void			setPoint (const Point& r);
	/// Assign a Rect.
			void			setRect (const Rect& r);
	/// Assign a FileSpec object.
			void			setFileSpec (const FileSpec& r);
	/// Assign a regular expression.
			void			setRegExp (const RegExp&);
	/// Assign an Array object.
			void			setArray (const Array& r);
	/// Assign an UnitValue object.
			void			setUnitValue (const UnitValue& r);
	///	The assignment operators.
			Variant&		operator= (const Variant&);
	///
	inline	Variant&		operator= (bool n)				{ setBool (n); return *this;   }
	///
	inline	Variant&		operator= (scint32_t n)			{ setInteger (n); return *this; }
	///
	inline	Variant&		operator= (scuint32_t n)			{ setInteger (n); return *this; }
	///
	inline	Variant&		operator= (double n)			{ setDouble (n); return *this; }
	/// ASCII only
	inline	Variant&		operator= (const char* p)		{ setString (p); return *this; }
	///
	inline	Variant&		operator= (const scutf16_t* p)	{ setString (p); return *this; }
	///
	inline	Variant&		operator= (const String& r)		{ setString (r); return *this; }
	///
	inline	Variant&		operator= (LiveObject* p)		{ setLiveObject (p); return *this; }
	///
	inline	Variant&		operator= (LiveObject& r)		{ setLiveObject (r); return *this; }
	///
	inline	Variant&		operator= (const Point& r)		{ setPoint (r); return *this; }
	///
	inline	Variant&		operator= (const Rect& r)		{ setRect (r); return *this; }
	///
	inline	Variant&		operator= (const FileSpec& r)	{ setFileSpec (r); return *this; }
	///
	inline	Variant&		operator= (const RegExp& r)		{ setRegExp (r); return *this; }
	///
	inline	Variant&		operator= (const Array& r)		{ setArray (r); return *this; }
	///
	inline	Variant&		operator= (const UnitValue& r)	{ setUnitValue (r); return *this; }
	/**
	Compare two variables. For objects, do a pure pointer comparison.
	*/
			bool			operator== (const Variant& r) const;
	///
	inline	bool			operator!= (const Variant& r) const { return ! (*this == r); }
	/**
	Compare two variables. Returns < 0 for this < r, = 0 for ==, and > 0 for this > r.
	If there is a numeric compare and any of the two operands is NaN, or if the values
	cannot be compared, the result is 0x7FFFFFFF. If any of the values is undefined, 
	it is smaller than the other value. For LiveObjects, compare their string 
	representation.
	*/
			scint32_t		compare (const Variant& r) const;
	/**
	Compare tow variables. This operator makes variants useable insiade a STL container.
	*/
	inline	int				operator< (const Variant& r) const { return int (compare (r)); }
	/**
	Validate the contents of this instance to be of a certain type.
	Returns kErrTypeMismatch on errors, 0 if the type matches
	*/
			scerror_t		validate (VariantType expectedType) const;
	/**
	Validate the contents of this instance to be numeric and within the given range.
	Returns kErrTypeMismatch or kErrRange on errors, 0 if type and range are ok.
	*/
			scerror_t		validate (double min, double max) const;
	/**
	Validate the contents of this instance to be numeric and within the given range.
	Returns kErrTypeMismatch or kErrRange on errors, 0 if type and range are ok
	and the number actually is an integer.
	*/
			scerror_t		validate (scint32_t min, scint32_t max) const;
	/**
	Validate the contents of this instance to be numeric and within the given range.
	Returns kErrTypeMismatch or kErrRange on errors, 0 if type and range are ok
	and the number actually is an unsigned integer.
	*/
			scerror_t		validate (scuint32_t min, scuint32_t max) const;
	/**
	Check whether this instance contains a valid object.
	*/
			bool			isValidObject() const;
	/**
	Read the property values for an object. This method attempts to enumerate the
	properties of an object and to fill the supplied hash table with name/value
	pairs. Use the same language/enumerations bits as defined for the LiveObject
	or ClassInfo enumerate() method. If the variant does not contain a Javascript 
	object or a LiveObject, the return value is kErrObjectExpected. The data of 
	the supplied hash table needs to be destroyed by calling its deleteObjects() 
	method when you are done.
	THIS METHOD IS BUGGY. It will not return LiveObject methods. Therefore, this
	method is deprecated and will be removed in version 4.0. Use the method
	ScScript::DebugAPI::getProperties() instead.
	@param	data		the hash table to fill with values
	@param	bits		language and enumeration bits
	@param	error		an optional Error instance that is filled in on errors
	*/
			scerror_t		getObjectProperties (THashTable<Variant>& data, 
							scint32_t bits = kJavaScriptLanguage
										 | ClassInfo::kEnumerateDontWalkProtoChain 
										 | ClassInfo::kEnumerateProperties, Error* error = NULL) const;
	/**
	Set the property values for an object. This method walks the supplied hash
	table and attempts to set each property value at the object stored in the
	variant. If a put operation fails, the method aborts and returns the error.
	If a property does not exist, it is created as a normal read-write property.
	THIS METHOD IS BUGGY. Therefore, this method is deprecated and will be 
	removed in version 4.0. Use ScScript::Engine::put() instead.
	@param	data		the hash table containing the values
	@param	error		an optional Error instance that is filled in on errors
	*/
			scerror_t		setObjectProperties (const THashTable<Variant>& data, Error* error = NULL);
	// Set a core JavaScript object. The ExtendScript engine uses this method.
			void			setJSObject (ScScript::Object&);
			void			setJSObject (ScScript::Object*);
	// Get a core JavaScript object. The ExtendScript engine uses this method.
			ScScript::Object* getJSObject() const;
};

inline void Variant::erase()
{
	if (mType != kUndefined)
		doErase();
}

inline bool Variant::isInteger () const 
{ 
	if (mType == kNumber)
	{
		if (!(attrs & kNumChecked))
			setNumAttrs();
		return ((attrs & kNumInteger) != 0);
	}
	else
		return false;
}

inline bool Variant::isUInteger () const 
{ 
	if (mType == kNumber)
	{
		if (!(attrs & kNumChecked))
			setNumAttrs();
		return ((attrs & kNumUInteger) != 0);
	}
	else
		return false;
}

inline bool Variant::isFinite () const 
{ 
	if (mType == kNumber)
	{
		if (!(attrs & kNumValidMask))
			doSetNumAttrs();
		return (attrs & kNumFinite) != 0;
	}
	else
		return doIsFinite(); 
}

inline bool Variant::isNaN () const 
{ 
	if (mType == kNumber)
	{
		if (!(attrs & kNumValidMask))
			doSetNumAttrs();
		return (attrs & kNumNaN) != 0;
	}
	else
		return doIsNaN(); 
}

inline bool Variant::isMinus0 () const 
{ 
	if (mType == kNumber)
	{
		if (!(attrs & kNumValidMask))
			doSetNumAttrs();
		return (attrs & kNumMinus0) != 0;
	}
	else
		return doIsMinus0(); 
}

}	// end Namespace

#include "ScPostConfig.h"

#endif

