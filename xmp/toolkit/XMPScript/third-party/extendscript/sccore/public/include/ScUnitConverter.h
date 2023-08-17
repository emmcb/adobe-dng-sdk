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
#ifndef _ScUnitConverter_h
#define _ScUnitConverter_h

#include "ScDefs.h"
#include "ScRefcountable.h"
#include "ScPreConfig.h"

namespace ScCore {

//	--------------------------------------------------------------------------------

/**
Core unit value types. Always accessible.
*/
enum
{
	/// Wild card value (for converters).
	kTypeWildCard				= 0x2A2A2A2a, // '****',
	/// No specific type.
	kTypeNoType					= 0x4E6F5470 // 'NoTp'
};

/// Definitions of standard Unit type id's are located in the ScUnitStdTypes header


typedef scuint32_t	UnitType;

/**
The conversion tuple is used to describe the conversion capabilities of a single
UnitConverter object.
<p>
NOTE:	If mBase is true (a type-to-based tuple) then mTypeTwo will be 
		interpreted as a base type.  When false, the tuple is assumed to describes 
		a direct conversion between two non-base types.
<p>
IMPORTANT:	mTypeOne must _never_ be a base type.
			mTypeOne may ONLY be kWildCard if mTypeTwo is also wildcard
*/

class SC_API ConversionTuple : public Heap
{
public:
	/// The first type.
	UnitType		mTypeOne;
	/// The second type.
	UnitType		mTypeTwo;
	/// True if the second type is a base type.
	bool			mBase;
	///
	inline ConversionTuple() :
		Heap(),
		mTypeOne( kTypeNoType ),
		mTypeTwo( kTypeNoType ),
		mBase( false )
	{ }
	///
	inline ConversionTuple( UnitType t1, UnitType t2, bool b ) :
		Heap(),
		mTypeOne( t1 ),
		mTypeTwo( t2 ),
		mBase( b )
	{ }
	inline ConversionTuple( const ConversionTuple& r ) :
		Heap(),
		mTypeOne( r.mTypeOne ),
		mTypeTwo( r.mTypeTwo ),
		mBase( r.mBase )
	{ }
	inline ConversionTuple& operator= (const ConversionTuple& r) {
		Heap();
		mTypeOne = r.mTypeOne;
		mTypeTwo = r.mTypeTwo;
		mBase    = r.mBase;
		return *this;
	}
		
};

/**
The TypeNameForm enumeration is used when converting a Unit-id to a string.
*/
enum TypeNameForm : scint32_t
{
	///
	kSingularForm			= 1,
	///
	kPluralForm				= 2,
	///
	kAbbreviationForm		= 3,
};

class Error;
class String;

/** The UnitConverter is the base class that all Converter classes must implement.
All conversions must be bi-directional so if a meter to inches conversion is provided,
then the converter object must also provide an inches to meter conversion
<p>
The objects are refcounted and subclasses may override addRef/release to use stack based /
static objects.
When a UnitConverter is added to the internal converter table; an addRef is performed; when the
Converter tables are cleared; release will be called.
<p>
The UnitConverter functionality allows for converting UnitValues between various
UnitTypes. UnitConverters must be bi-directional.
UnitConverters are used in one of two ways: they can be activated from the convert
value in the UnitValue class; alternatively UnitConverters are used to translate from
a string representation of a unit, such as "3pt" to a UnitValue.
<p>
The ScCore frame work installs a number of standard converters. These are listed
below. ScCore clients can add custom converters by calling the add method. All
converters superceeds previously installed converters, so a client can override
a standard converter by installing a new converter.
<p>
A unit converter provides a conversion between 2 specified types. These types are
described in the ConversionTuple. Clients that are interested in exposing Apple event
interfaces should use Apple type id's for unit types.
<p>
When the ScCore frame work tries to convert between two different types (T and S), the
following algorithm is used.
<p>
Identify a converter related to a direct conversion; meaning find any converter described by
a ConversionTuple, where mTypeOne is T and mTypeTwo is S; or (the reverse) mTypeOne is S and
mTypeTwo is T.
<p>
If a direct conversion was not found; then identify 2 UnitConverters: a converter that converts
T to a base unit (mTypeOne is T and mBaseConversion is true); and an other converter that
converts S to a base unit (mTypeOne is S and mBaseConversion is true). If the base unit type of
these two converter objects is the same, then the following conversion will happen: T -> base -> S.
<p>
if a base conversion was not found, then try to find a partial wildcard converter for T1
(mTypeOne is T and mTypeTwo is kTypeWildCard). If such a converter was not found try the same for S
<p>
if no partial Wildcard converters were found; then use the full Wildcard converter (mTypeOne and mTypeTwo
are both kTypeWildcard)
<p>
if none of the above searches were successful, then the conversion will fail with a "kIncompatibleTypes" error.
<p>
A conversion to or from type kTypeNoType to any other type is always possible. Such a conversion will not alter
the value.
<p>
Requirements: A base type must always be specified in the mTypeTwo field.
A partial wildcard converter must specify the wildcard type in mTypeTwo.
*/

class SC_API UnitConverter : public Refcountable
{
protected:
	virtual ~UnitConverter() {}

public:
	/**
	The unit convertValue method should convert between two types. This method is only called when the fromType and the toType
	match the types that were specified in the ConversionTuple when adding the ConversionObject.
	@param	fromType		id of the type that is related to the "value" argument
	@param	value			incoming value
	@param	toType			id of type to convert the value to
	@param	result			the converted value
	@param	err				an optional error ptr that can be used to return extended error information
	@return					The convert method should return kErrOK if the conversion succeeded. If the conversion
							failed then the convert method should return a relevant error, and if the err ptr is not null
							optionally create an extended error information record
	*/
	virtual	scerror_t		convertValue( UnitType fromType, double value, UnitType toType, double& result, Error * err = NULL ) const = 0;

	/**
	parse should attempt to create a unit-value from an incoming string representation of a unit value.
	Pre-parsing of the unit expressionis performed by the UnitConverter frame work.
	@param	unitName		string value of the unit. (if the string is "12pt4" then unitName will be "pt" )
	@param	left			digits found to the left of the unitName. (if the string is "12pt4" then left will be "12" )
	@param	right			digits found to the right of the unitName. (if the string is "12pt4" then right will be "4" )
	@param	type			[out]. If the method was able to parse the unit expression; then type should be set to the type-id of the UnitValue.
	@param	value			[out]. If the method was able to parse the unit expression; then value should be set to the number part of the UnitValue.
	@return					kErrOK if the method successfully parsed the incoming string.
							kErrConversion is the converter did not handle the provided unit.
							kErrSyntax if the type name is recognized but the provided left and right values are illegal
	*/
	virtual scerror_t		parse( const String& unitName, double leftValue, double rightValue, UnitType& type, double& value ) const = 0;

	/**
	A UnitConverter should be able to provide a textual representation of a given UnitValue using its types
	The returned string should be symmetrical in the sense that the following expression should be true "parse( getUnitStr( aValue ) ) == aValue"
	If the UnitConverter is able to create a textual representation from the unit value it should return kErrOK
	A converter object will only be asked to name types that it has registered for.
	@param	type			string value of the unit. (if the string is "12pt4" then unitName will be "pt" )
	@param	value			digits found to the left of the unitName. (if the string is "12pt4" then left will be "12" )
	@param	typeStr			[out]. If the method was able to parse the unit expression; then type should be set to the type-id of the UnitValue.
	@return					kErrOK if the method successfully parsed the incoming string; otherwise return kErrConversion or any other appropriate error.
	*/
	virtual scerror_t		getUnitExpr( UnitType type, double value, String& typeStr ) const = 0;


	/**
	Return the type ID for a text representation of a unit. The CuniConverter should handle all relevant forms of the type. For example
	if kTypeMeters is supported, then the converter should recognize: "meter", "meters" and "m"
	@param	unitStr			text representation of the unit string
	@param	type			[out]. If the method returns kErrOK, then type will be set to the type id corresponding to the provided string
	@return					kErrOK if the method handled the unit string.
							kErrConversion is the converter did not handle the unit string
	*/
	virtual scerror_t		getTypeID( const String& unitStr, UnitType& type ) const = 0;

	/**
	Return the type ID for a text representation of a unit. The UniConverter should handle all relevant forms of the type. For example
	if kTypeMeters is supported, then the converter should recognize: "meter", "meters" and "m". The UnitConverter will only be asked to
	provide names for types that it has registered for
	@param	type			ID of unit type
	@param	form			desired form for the returned string. Should be one of the values defined in: TypeNameForm (singular, plural or abbreviation)
	@param	unitStr			[out]. If the method returns kErrOK, then type will be set to the string corresponding to the "type" argument
	@return					kErrOK if the method handled the unit string.
							kErrConversion is the converter does not handle providing string names for the unit-id
	*/
	virtual scerror_t		getTypeName( UnitType type, scint32_t form, String& unitStr ) const = 0;
};

class UnitValue;

/**
The UnitConverterTable class handles the global table of all registered UnitConverters.
The class provides for functinality related to adding converters or to query all installed
UnitConverters for parsing a unit expression.
The class will install standard UnitConverters in a lazily fashion. When the first UnitConverter related method is
called all standard UnitConverters are installed.
If you call "clear" before any UnitConverter related method is called then the class will not attempt to install
standard converters.
*/

class SC_API UnitConverterTable : public Heap
{
public:
	/**
	Add all UnitConversions described by the provided ConversionTuple list to the internal table and associate them
	with the converter object.
	If you install a conversion between types T and S, you superceed any already installed T-S converters.
	One or more addRefs will be called on the converter object
	@param	conversions		list of conversions supported by the "converter" object
	@param	num				number of entries in the conversion list
	@param	converter		a UnitConverter object
	@return					kErrOK if the the conversions were successfully added to the list. Will return kErrSyntax
							if the entries in the ConverterTuples are not valid
	*/
	static scerror_t		add( const ConversionTuple* conversions[], scuint32_t num, const UnitConverter& convertion );

	/**
	Ask all installed converter objects to parse the incoming unit-value expression.
	@param	unitExp			a textual UnitValue expression such as "12 meters"
	@param	value			[out] if the method returns kErrOK, then value is set to the UnitValue corresponding to the unitExp string
	@return					kErrOK if any of the installed unit converters were able to parse unit expression
	*/
	static scerror_t		parse( const String& unitExp, UnitValue& value );

	/**
	Attempts to create a textual representation of a given UnitValue.
	The converter table will find the first unit converter associated with the type contained in the incoming value, and
	ask that conversion object to provide a textual representation of the unit.
	@param	value			the UnitValue to create a string representation for
	@param	valueStr		[out] if the method returns true, then this variable will be set to the textual representation of value
	@return					kErrOK if a textual representation was created
	*/
	static bool				getUnitExpr( const UnitValue& value, String& valueStr );

	// Look in the ConverterList for any possible conversion
	// Any conversion involving either kTypeWildCard or kTypeNoType will simply return the incoing value
	// to the outgoing variable

	/**
	Look in the ConverterList for any possible conversion between the incoming unitValue and toType. See comments above
	for information about the conversion algorithm.
	@param	value			the UnitValue to convert
	@param	toType			the type to attempt to convert to
	@param	result			[out] if the method returns kErrOK, then result will hold the converted value
	@param	err				if provided, and UnitConverter may use this variable to provide extended error information
	@return					kErrOK if the conversion was successfull
	*/
	static scerror_t		convert( const UnitValue& value, UnitType toType, double& result, Error * err = NULL );

	/**
	Look ConverterList for any possible conversion between the incoming unitValue and toType. See comments above
	for information about the conversion algorithm.
	@param	value			the UnitValue to convert
	@param	toType			the type to attempt to convert to
	@param	result			[out] if the method returns kErrOK, then result will hold the resulting UnitValue
	@param	err				if provided, and UnitConverter may use this variable to provide extended error information
	@return					kErrOK if the conversion was successfull
	*/
	static scerror_t		convert( const UnitValue& value, UnitType toType, UnitValue& result, Error * err = NULL );

	/**
	Convert the incoming value to it's base type
	@param	value			the UnitValue to convert
	@param	toType			[out] if the method returns kErrOK, then toType is set to the base type
	@param	result			[out] if the method returns kErrOK, then result will hold the converterd value
	@param	err				if provided, and UnitConverter may use this variable to provide extended error information
	@return					kErrOK if the conversion was successfull
	*/
	static scerror_t		convertToBase( const UnitValue& value, UnitValue& result, Error * err = NULL );

	/// remove all registered converters. If you call this before any other  UnitConverterTable calls, then the standard
	// converters will not be installed
	static void				erase();

	/**
	return the base type of the provided unit. If no base type is found or if type is is kTypeWildCard or kTypeNoType
	then kErrConversion is returned
	@param	type			the type to find the base type for
	@param	baseType		[out] if the method returns kErrOK, then baseType is set to the base type related to type
	@return					kErrOK if a base type was found; otherwise kErrConversion is returned.
	*/
	static scerror_t		findBase( UnitType type, UnitType& baseType );

	/**
	Return the unit type id for a given string
	@param	unitStr			The string representation of the unit
	@param	type			[out] if the method returns kErrOK, then type is set to the unit type id
	@return					kErrOK if a base type was found
	*/
	static scerror_t		getTypeID( const String& unitStr, UnitType& type );

	/**
	Return the name for a unit type id
	@param	type			The id of the unit type to obtain the string representation for
	@param	form			The requested form of the returned string. Should be one of the values defined in: TypeNameForm (singular, plural or abbreviation)
	@param	unitStr			[out] if the method returns kErrOK, then unitStr is set to the string representation of the unit
	@return					kErrOK if a base type was found. Returns kErrConversion if type is kTypeNoType or kTypeWildCard or if a string representation
							was not found
	*/
	static scerror_t		getTypeName( UnitType type, scint32_t form, String& unitStr );
};


} // namspace ScCore

#include "ScPostConfig.h"

#endif // _ScUnitConverter_h

