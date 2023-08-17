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

#ifndef REZ
	#if SC_ONCE
	#pragma once
	#endif
#endif

#ifndef _ScUnitStdTypes_h_
#define _ScUnitStdTypes_h_

#ifndef REZ
namespace ScCore
{

#endif

/**
Definitions of unit type id's used by the ScCore standard unit system.
The header can be consumed by a C++ compiler as well as the Metrowerks version of
the Rez tool.
<p>
IMPORTANT: None of the Unit definitions shown below may be changed to an other type id.
Changing any of the values would result in breaking scripting compatibility for existing
scripting implementations.
*/

enum
{
	//	These are defined with the same values as used in AERegistry.h (Mac OS)
	///	Base unit for length values
	kTypeMeters					= 0x6d657472, // 'metr'
	/// 2.54 cm per inch
	kTypeInches					= 0x696e6368, // 'inch'
	/// 12 inches per foot
	kTypeFeet					= 0x66656574, // 'feet'
	/// 3 feet per yard
	kTypeYards					= 0x79617264, // 'yard'
	/// 1760 yzrds per mile
	kTypeMiles					= 0x6d696c65, // 'mile'
	///
	kTypeCentimeters			= 0x636D7472, // 'cmtr'
	///
	kTypeKilometers				= 0x6b6d7472, // 'kmtr'

	//	Other basic length unit types.
	///
	kTypeMillimeters			= 0x814D4D8C,
	///	72.27 per inch
	kTypeTraditionalPoints		= 0x8150548C,
	/// 72 per inch
	kTypePSPoints				= 0x8150538C,
	///	12 traditional points
	kTypeTraditionalPicas		= 0x8150438C,
	///	12 points
	kTypePSPicas				= 0x8153508C,
	///	12 didot points
	kTypeCiceros				= 0x8143528C
};

///	Relational units.
enum
{
	///
	kTypePercent				= 0x81504E8C
};

///	Count / Size units.
enum
{
	///
	kTypePixels					= 0x8150588C
};


///	Base unit value types.
enum
{
	/// The base length type is meters.
	kBaseLengthUnitType			= kTypeMeters,
	/// The base relational type is percent.
	kBaseRelationalUnitType		= kTypePercent,
	/// The base count/size unit is pixels.
	kBaseCountUnitType			= kTypePixels
};

#ifndef REZ
}		// namespace
#endif

#ifndef REZ
	#endif		// _ScUnitStdTypes_h_
#endif

