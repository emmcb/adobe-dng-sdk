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
#ifndef _EsDataPool_h
#define _EsDataPool_h

#include "ScCore.h"
#include "ScPreConfig.h"

namespace ScScript
{

/**
Symbolic ID.
The sID is a long data value describing a reference into the symbol pool.
These sIDs are used internally as replacements for strings. The format
of a sID is determined by its upper three bits.
*/

typedef scint32_t sID;

/**
Definitions of sID types. These types are encoded in the upper three bits of the sID.
Any sID type code not defined here is assumed to be reserved.
*/

enum
{
	/**
	A standard ID as predefined or as added to the data pool.
	Most of the IDs have this type.
	*/
	sIDT_STD = (scint32_t) 0x00000000,
	/**
	A standard ID as predefined or as added to the data pool
	with the first letter being uppercase. This is usually
	true for class names.
	*/
	sIDT_STDUC = (scint32_t) 0x20000000,
	/**
	A Unicode character constant. The scanner generates these sIDs.
	The parsed tree never contains these values, but the iID field
	in the HiliteRange structure contains these values.
	@see HiliteRange
	*/
	sIDT_CHAR = (scint32_t) 0x40000000,
	/**
	These IDs are unsigned integers in the range form 0 through 0x1FFFFFFF.
	*/
	sIDT_LONGINT = (scint32_t) 0x60000000,
	/**
	ASCII Characters. Same as sIDT_CHAR, but it may contain up to three
	ASCII characters.
	*/
	sIDT_ASCIICHAR = (scint32_t) 0x80000000,
	/**
	The top three bits are used to distinguish between groups of IDs.
	*/
	sIDT_TYPEMASK = (scint32_t) 0xE0000000,
	__sIDT__32bit__ = 0x7FFFFFFF
};

// Definition of standard expression operators.

enum
{
	sID_EMPTY = 0,					// the empty string or no ID at all
	// operators are ordered by weight, unary operators first
	sID_TYPEOFOP,					// typeof
	sID_DELETEOP,					// delete
	sID_VOIDOP,						// void
	sID_PREINC,						// ++x
	sID_PREDEC,						// --x
	sID_POSTINC,					// x++
	sID_POSTDEC,					// x--
	sID_NOT,						// ~
	sID_LOGNOT,						// !
	sID_UNPLUS,						// +x
	sID_NEG,						// -x
	sID_TIMES,						// *
	sID_DIV,						// /
	sID_MOD,						// %
	sID_PLUS,						// +
	sID_MINUS,						// -
	sID_SHL,						// <<
	sID_SHR,						// >>
	sID_SHR2,						// >>>
	sID_LESS,						// <
	sID_LESSEQ,						// <=
	sID_GREATER,					// >
	sID_GREATEREQ,					// >=
	sID_INOP,						// in
	sID_INSTANCEOFOP,				// instanceof
	sID_EQUAL,						// ==
	sID_NOTEQUAL,					// !=
	sID_EQUALITY,					// ===
	sID_NOTEQUALITY,				// !===
	sID_AND,						// &
	sID_OR,							// |
	sID_XOR,						// ^
	sID_LOGAND,						// &&
	sID_LOGOR,						// ||
	sID_TERNARY,					// ?:
	sID_COMMA,						// comma separated expressions
	sID_TIMESEQ,					// *=
	sID_DIVEQ,						// /=
	sID_MODEQ,						// %=
	sID_PLUSEQ,						// +=
	sID_MINUSEQ,					// -=
	sID_SHLEQ,						// <<=
	sID_SHREQ,						// >>=
	sID_SHR2EQ,						// >>>=
	sID_EQ,							// =
	sID_ANDEQ,						// &=
	sID_OREQ,						// |=
	sID_XOREQ,						// ^=
	sID_FIRSTOPERATOR				= sID_TYPEOFOP,
	sID_LASTOPERATOR				= sID_XOREQ,
	sID_FIRSTUNARYOP				= sID_TYPEOFOP,
	sID_LASTUNARYOP					= sID_NEG,
	sID_FIRSTBINARYOP				= sID_TIMES,
	sID_LASTBINARYOP				= sID_XOREQ,

	// JavaScript keywords

	sID_ABSTRACT,	sID_BOOLEAN,	sID_BREAK,		sID_BYTE,		sID_CASE,		sID_CATCH,		
	sID_CHAR,		sID_CLASS,		sID_CONST,		sID_CONTINUE,	sID_DEBUGGER,	sID_DEFAULT,
	sID_DELETE,		sID_DO,			sID_DOUBLE,		sID_ELSE,		sID_ENUM,		sID_EXPORT,
	sID_EXTENDS,	sID_FALSE,		sID_FINAL,		sID_FINALLY,	sID_FLOAT,		sID_FOR,
	sID_FUNCTION,	sID_GOTO,		sID_IF,			sID_IMPLEMENTS,	sID_IMPORT,		sID_IN,
	sID_INSTANCEOF,	sID_INT,		sID_INTERFACE,	sID_LONG,		sID_NATIVE,		sID_NEW,
	sID_NULL,		sID_PACKAGE,	sID_PRIVATE,	sID_PROTECTED,	sID_PUBLIC,		sID_RETURN,
	sID_SHORT,		sID_STATIC,		sID_SUPER,		sID_SWITCH,		sID_SYNCHRONIZED,sID_THIS,
	sID_THROW,		sID_THROWS,		sID_TRANSIENT,	sID_TRUE,		sID_TRY,		sID_TYPEOF,
	sID_VAR,		sID_VOID,		sID_VOLATILE,	sID_WHILE,		sID_WITH,
	sID_FIRSTKEYWORD = sID_ABSTRACT,
	sID_LASTKEYWORD = sID_WITH,

	// some wellknown values

	sID_DOTINDEX,								// .index
	sID_DOTNUMBER,								// .number
	sID_DOTNAME,								// .name
	sID_IDCALLEE,								// callee
	sID_IDARGUMENTS,							// arguments
	sID_IDPROTOTYPE,							// prototype
	sID_IDTOSTRING,								// toString
	sID_IDVALUEOF,								// valueOf
	sID_IDLENGTH,								// length
	sID_IDUNDEFINED,							// undefined
	sID_IDNUMBER2,								// number
	sID_IDSTRING2,								// string
	sID_IDOBJECT2,								// object
	sID_IDGET,									// get
	sID_IDGETTER,								// getter
	sID_IDSET,									// set
	sID_IDSETTER,								// setter
	sID_IDCONSTRUCTOR,							// constructor
	sID_IDPROTO,								// __proto__
	sID_IDEVALNAME,								// [eval]
	sID_IDTOPLEVELNAME,							// [toplevel]
	sID_IDANONYMOUS,							// anonymous
	sID_IDMINUS0,								// -0
	sID_IDDOLLAR,								// $

	sID_CAPAND = (sID_IDDOLLAR + 1 + sIDT_STDUC),// AND
	sID_CAPOR,									// OR
	sID_CAPXOR,									// XOR
	sID_IDINFINITY,								// Infinity
	sID_IDNAN,									// NaN
	sID_IDARRAY,								// Array
	sID_IDBOOLEAN,								// Boolean
	sID_IDDATE,									// Date
	sID_IDERROR,								// Error
	sID_IDFUNCTION,								// Function
	sID_IDMATH,									// Math
	sID_IDNUMBER,								// Number
	sID_IDOBJECT,								// Object
	sID_IDREGEXP,								// RegExp
	sID_IDSTRING,								// String
	sID_IDLIVEOBJECT,							// LiveObject
	sID_IDREFLECTION,							// Reflection
	sID_IDREFLECTIONINFO,						// ReflectionInfo
	sID_IDDICTIONARY,							// Dictionary
	sID_IDXML,									// XML
	sID_IDXMLLIST,								// XMLList

	sID_EACH = ((sID_IDXMLLIST + 1)&~sIDT_STDUC),// E4X: each
	sID_NAMESPACE,								// E4X: namespace
	sID_XML,									// E4X: xml

	sID_FIRSTWELLKNOWN = sID_DOTINDEX,
	sID_LASTWELLKNOWN = sID_XML,
	__sID__32bit__		= 0x7FFFFFFF
};

/// Utility macro to define character constants

#define sID_CHAR( n ) sID (sIDT_CHAR |(n & 0x0000FFFF))

/// Utility macro2 to define ASCII character constants

#define sID_ASCIICHAR2( c1, c2 ) sID (sIDT_ASCIICHAR | (c2 << 8) | c1)

/// Utility macro2 to define ASCII character constants

#define sID_ASCIICHAR3( c1, c2, c3 ) sID (sIDT_ASCIICHAR | (c2 << 16) | (c2 << 8) | c1)

// Definitions of special, common characters

enum {
	sID_EOL			= sID_CHAR('\n'),				// end-of-line
	sID_EOS			= sID_CHAR(';'),				// end-of-statement
	sID_COLON		= sID_CHAR(':'),				// colon
	sID_COMMENT		= sID_ASCIICHAR2( '/','/' ),	// comment '//'
	sID_DOTDOT		= sID_ASCIICHAR2( '.','.' ),	// E4X: ..
	sID_DOUBLECOLON	= sID_ASCIICHAR2( ':',':' ),	// E4X: ::
	sID_ATTRIBUTE	= sID_CHAR( '@' ),				// E4X: @
	sID_PREDICATE	= sID_ASCIICHAR2( '(',')' ),	// E4X: .(expr)
	sID_DFLTXMLNS	= sID_ASCIICHAR3( 'X','n','s' ),// E4X: default xml namespace
	__sIDCONST__32bit__	= 0x7FFFFFFF
};

class DataPoolData;
class Engine;

/**
The data pool is global to all threads. It contains all symbols (property names)
used by all ExtendScript engines. ExtendScript operates internally with sIDs,
which are far easier to compare and to lookup than strings. Also, sIDs are
shareable across all ExtendScript engines. The data pool is preset with a set 
of well defined IDs. Access to the pool is guarded.
*/

class ES_API DataPool : public ScCore::Heap
{
	/// Private copy ctor, because this class has pointer data members.
							DataPool (const DataPool&);
	/// Private assignment operator, because this class has pointer data members.
	DataPool&				operator= (const DataPool&);

	DataPoolData*			mData;
	sID						mLastPredefinedID;
	bool					_isInteger (sID id) const;
	bool					_isUInteger (sID id) const;
public:
							DataPool();
							~DataPool();

	/**
	Get the reference to the global data pool. There is only one data pool
	per application.
	*/
	static	DataPool&		get();
	/**
	Check whether the given symbol starts with an upper case letter.
	*/
	static	bool			isUpperCase (sID id) { return ((id & sIDT_TYPEMASK) == sIDT_STDUC); }
	/**
	Check whether the given sID refers to a number. This is the case if the
	sID is either an integer numeric sID (sIDT_NUMBER) or refers to a numeric
	Variant.
	@param	id				the sID to check
	@return					true if numeric
	*/
			bool			isNumber (sID id) const;
	/**
	Check whether the given sID refers to a signed integer number. This is the case if the
	sID is either an integer numeric sID (sIDT_NUMBER) or refers to a numeric
	Variant that contains an integer.
	@param	id				the sID to check
	@return					true if integer numeric
	*/
	inline	bool			isInteger (sID id) const;
	/**
	Check whether the given sID refers to an unsigned integer number. This is the case if the
	sID is either an integer numeric sID (sIDT_NUMBER) or refers to a numeric
	Variant that contains an unsigned integer.
	@param	id				the sID to check
	@return					true if integer numeric
	*/
	inline	bool			isUInteger (sID id) const;
	/**
	Get the signed integer number associated with this sID.
	@param	id				the sID
	@return					the numeric value
	*/
			scint32_t		getInteger (sID id) const;
	/**
	Get the unsigned integer number associated with this sID.
	@param	id				the sID
	@return					the numeric value
	*/
			scuint32_t		getUInteger (sID id) const;
	/**
	Retrieve the number asociated with this sID. If the sID refers to a Variant,
	its getDouble() method is called. Otherwise, return the numeric part of
	the sID.
	@param	id				the sID
	@return					the numeric value
	@see					Variant
	*/
			double			getNumber (sID id) const;

	/// Add an ASCII string to the pool.
			sID				add (const char* r);
	/// Add an UTF-16 string to the pool.
			sID				add (const scutf16_t* r);
   	/// Add a character to the pool.
	inline	sID				add (scutf16_t ch) { return ch | sIDT_CHAR; }
	/// Add an array index to the pool.
			sID				add (scuint32_t idx); 
	/// Add a number to the pool.
			sID				add (double n);
	/// Add a value instance to the pool.
			sID				add (const ScCore::Variant& r);
	/**
	Get the value associated with a sID. For sID_TRUE, sID_FALSE and sID_NULL,
	the corresponding values are returned , not the string. For character SIDs,
	a string value containing the character is returned.
	@param	id				the ID to find the value for
	@param	value			takes the value found
	*/
			void			getValue (sID id, ScCore::Variant& value) const;
	/**
	Get the symbol associated with a sID. For character sIDs,
	a string value containing the character is returned.
	@param	id				the ID to find the string for
	@return					the string found
	*/
			ScCore::String	getSymbol (sID id) const;
};

inline bool DataPool::isInteger (sID id) const
{
	if (((id & sIDT_TYPEMASK) == sIDT_LONGINT) || (id == sID_IDMINUS0))
		return true;
	else if (id <= mLastPredefinedID)
		return false;
	else
		return _isInteger (id);
}

inline bool DataPool::isUInteger (sID id) const
{
	if (((id & sIDT_TYPEMASK) == sIDT_LONGINT) || (id == sID_IDMINUS0))
		return true;
	else if (id <= mLastPredefinedID)
		return false;
	else
		return _isUInteger (id);
}

}	// end namespace

#include "ScPostConfig.h"
#endif

