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
#ifndef _ScVariantType_h
#define _ScVariantType_h

#include "ScPreConfig.h"

namespace ScCore
{

/**
The possible value types of a Variant. Defined in a separate file to prevent
circular inclusion.
*/

enum VariantType 
{
	/**
	This data type is not a real data type that you would find in a Variant. Instead,
	is is used as a return value in the TypeInfo class to indicate that the data type
	of a property or the return data type of a function can be any type.
	*/
	kAnyType		= 126,
	/**
	This data type is not a real data type that you would find in a Variant. Instead,
	is is used as a return value in the TypeInfo class to indicate that the data type
	of a property or the return data type of a function is not known.
	*/
	kUnknownType	= 127,

	/// Undefined. This is the initial value of a new Variant instance.
	kUndefined		= 0, 
	/// A null pointer.
	kNull			= 1, 
	/// A boolean value.
	kBool			= 2, 
	/// An numeric value.
	kNumber			= 3, 
	/// A string value.
	kString			= 4,
	/// A JavaScript object reference.
	kJSObject		= 5,
	///	A LiveObject reference.  LiveObjects are refcounted when assigned to a Variant.
	kLiveObject		= 6,
	///	A Point.
	kPoint			= 7,
	/// A Rect.
	kRect			= 8,
	///	An Array.
	kArray			= 9,
	/// A FileSpec.
	kFileSpec		= 10,
	/// A regular expression.
	kRegExp			= 11,
	/// An unit value.
	kUnitValue		= 12,
	__VariantType_32bit__	= 0x7FFFFFFF
};

}	// end namespace

#include "ScPostConfig.h"

#endif

