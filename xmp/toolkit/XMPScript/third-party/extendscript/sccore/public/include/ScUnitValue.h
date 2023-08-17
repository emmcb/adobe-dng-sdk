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
#ifndef _ScUnitValue_h
#define _ScUnitValue_h

#include "ScRoot.h"
#include "ScUnitConverter.h"

#if SC_NO_UNITVALUE == 1
#error UnitValue not supported in this static library configuration, please edit ScStaticLibDefs.h and recompile!
#endif

#include "ScPreConfig.h"

namespace ScCore {

class String;
class Variant;

/// Unit point array index: X
const scuint32_t			kXValue = 0;
/// Unit point array index: Y
const scuint32_t			kYValue = 1;

/// Unit rectangle array index: left
const scuint32_t			kLeftValue		= 0;
/// Unit rectangle array index: top
const scuint32_t			kTopValue		= 1;
/// Unit rectangle array index: right
const scuint32_t			kRightValue		= 2;
/// Unit rectangle array index: bottom
const scuint32_t			kBottomValue	= 3;

/**
	The UnitValue class describes a typed value. Most UnitValues can be converted from one
	unit to another as long as they have a common base unit. Some units, like pixels and 
	percentages, do not have a base unit. For these units, you can either set a global
	base unit/value, or set a local base unit/value at the instance. This base unit desribes
	the conversion base for on unit. The default is 1/72 inch, which applies well to pixels.

	In the ScCore model you can create Unit based point and rectangle structures
	by creating arrays of UnitValue objects.

	An array with 2 elements can be interpreted as a point by using the kXValue and kYValue
	consts.

	An array with 4 elements can be interpreted as a rectangle by using the kLeftValue, kTopValue,
	kBottomValue and kRightValue consts.
*/

class SC_API UnitValue : public Heap
{
private:
	double				mValue;
	double				mBaseValue;
	UnitType			mUnitType;
	UnitType			mBaseUnitType;

public:
	/// Get the default conversion UnitValue for baseless conversions.
	static	void		getBaseUnit (UnitValue& r);
	/// Set the default conversion UnitValue for baseless conversions.
	static	void		setBaseUnit (const UnitValue& r);
	///	An undefined value.
						UnitValue();
	
	/**
	Set type of the value and initialize to 0.0.
	This ctor can be used when creating a UnitValue to be passed into a LiveObject method.
	The semantics of a LiveObject::get or LiveObject::call is for the called method to look at the
	type of the return variable. If it is UnitType, then the called method can use getType on
	the return value to see which unit the caller prefers.
	*/
	explicit			UnitValue (UnitType requestedType);
	
						UnitValue (UnitType unitType, double value);

	///	Create a unit value from a unit expression such as "10 meters".
	explicit			UnitValue (const String& unitString);

	/// The copy constructor.
						UnitValue (const UnitValue& newVal);

	///
			UnitValue&	operator=  (const UnitValue& rhs);

	///
			UnitValue&	operator=  (const String& rhs);

	/**
	less-than is provided to enable UnitValues to be used in STL containers.
	NOTE: The sorting algorithm is NOT a user ordering. Use the "cmp" method if you want to
	provide value based sorting.
	Operator less than uses the following algorithm:
	If the unit-type differs then the smallest value of the scint32_t representation of the type-id
	determines the smaller value. If the unit-types are equal then the smaller value of
	the value determines the smaller value
	*/
			bool		operator<  (const UnitValue& rhs) const;
	
	/// Equality operator
			bool		operator== (const UnitValue& rhs) const;

	/// Not equality operator.
	inline	bool		operator!= (const UnitValue& rhs) const	{ return ! (*this == rhs); }
		

	/**@name value based comparison. The comparison will compare values converted to the base-type.
	The comparison fails if the type described by rhs does not have the same base type as this
	@param		rhs					the UnitValue to compare with
	@return							<0: smaller, 0: equal, >0: larger. Return 0x7FFFFFFF if the values
									could not be compared.
	*/
			int			cmp (const UnitValue& rhs) const;

	
	//	--------------------	Accessors	--------------------------------------------

	/// Is this unit empty?
	inline	bool		isEmpty() const							{ return  (mUnitType == kTypeNoType); }
	/// Get the type.
	inline	UnitType	getType() const							{ return mUnitType; }
	/// Get the value.
	inline	double		getValue() const						{ return mValue; }
	/// Get the type and return the value.
	inline	double		getValue (UnitType& unitType) const		{ unitType = mUnitType; return mValue; }
	/// Get the base type and return the base value.
	inline	double		getBaseValue (UnitType& unitType) const	{ unitType = mBaseUnitType; return mBaseValue; }
	/// Get the base type and value.
	inline	void		getBaseValue (UnitValue& r) const		{ r.mUnitType = mBaseUnitType; r.mValue = mBaseValue; }
	
	//	--------------------	Changers	--------------------------------------------

	///	Set the unit object's type and value.
	inline	void		setValue (UnitType unitType, double value)	{ mUnitType = unitType; mValue = value; }
	///	Set the unit object's value, and the type to noType.
	inline	void		setValue (double value)						{ mUnitType = kTypeNoType; mValue = value; }
	///	Set the unit object's base type and value.
	inline	void		setBaseValue (UnitType type, double value)	{ mBaseUnitType = type; mBaseValue = value; }
	/// Set the base type and value.
	inline	void		setBaseValue (const UnitValue& r)			{ mBaseUnitType = r.mUnitType; mBaseValue = r.mValue; }

	
	///	Convert the contained UnitValue to the specified type.
			bool		convert (UnitType toType);

	/// Return a UnitValue corresponding to the contained value converted to "toType".
			bool		convert (UnitType toType, UnitValue& unit) const;
	
	/// Convert the object to its base type. If baseless, apply the stored base conversion first.
			scerror_t	convertToBase (UnitValue& r) const;
	///
			scerror_t	convertToBase();

	/// Reset the unit value.
	inline	void		erase()	{ mUnitType = mBaseUnitType = kTypeNoType; mValue = mBaseValue = 0.0; }

	/// Create a textual representation.
			void		toString (String& value) const;
}; // class UnitValue


} // namspace ScCore

#include "ScPostConfig.h"

#endif // _ScUnitValue_h

