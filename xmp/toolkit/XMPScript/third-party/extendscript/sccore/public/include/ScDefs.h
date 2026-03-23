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
#ifndef _ScDefs_h
#define _ScDefs_h

// ////////////////////////////////////////////////////////////////////

// Standard definitions

//#include <stddef.h>
#if defined (SC_LIB) || defined (ES_LIB)
#include "ScStaticLibDefs.h"
#endif

// Define the following constant to avoid the definition of uchar, uint, ushort, ulong
// #define SC_NO_LEGACY_DATATYPES 1

#undef SC_WCHAR_WIDTH
#undef SC_ONCE
#undef SC_NOTHROW
#undef SC_PURE
#undef SC_STDCALL
#undef SC_API
#undef ES_API

// Visual C++, Windows

/*
The file stdint.h is missing on Windows. Therefore, we did some research and found the
following definitions:

1) OWL: (OWLConditionals.h and OWLIntTypes.h):

	#define OWL_INT8_TYPE						__int8
	#define OWL_INT16_TYPE						__int16
	#define OWL_INT32_TYPE						__int32
	#define OWL_INT64_TYPE						__int64

	typedef signed OWL_INT8_TYPE				scint8_t;
	typedef signed OWL_INT16_TYPE				scint16_t;
	typedef signed OWL_INT32_TYPE				scint32_t;
	typedef signed OWL_INT64_TYPE				scint64_t;

	typedef unsigned OWL_INT8_TYPE				scuint8_t;
	typedef unsigned OWL_INT16_TYPE				scuint16_t;
	typedef unsigned OWL_INT32_TYPE				scuint32_t;
	typedef unsigned OWL_INT64_TYPE				scuint64_t;

2) Bravo: ASBasicTypes.h

    typedef __int8 scint8_t;
    typedef unsigned __int8 scuint8_t;
    typedef __int16 scint16_t;
    typedef unsigned __int16 scuint16_t;
    typedef __int32 scint32_t;
    typedef unsigned __int32 scuint32_t;
    typedef __int64 scint64_t;
    typedef unsigned __int64 scuint64_t;

3) Boost: cstdint.hpp

     typedef signed char     scint8_t;
     typedef signed char     int_least8_t;
     typedef signed char     int_fast8_t;
     typedef unsigned char   scuint8_t;
     typedef unsigned char   uint_least8_t;
     typedef unsigned char   uint_fast8_t;
     typedef short           scint16_t;
     typedef short           int_least16_t;
     typedef short           int_fast16_t;
     typedef unsigned short  scuint16_t;
     typedef unsigned short  uint_least16_t;
     typedef unsigned short  uint_fast16_t;
     typedef int             scint32_t;
     typedef int             int_least32_t;
     typedef int             int_fast32_t;
     typedef unsigned int    scuint32_t;
     typedef unsigned int    uint_least32_t;
     typedef unsigned int    uint_fast32_t;

All definitions are compatible, because this is what thew MS doc says about the MS 
specific __intXX types:

The types __int8, __int16, and __int32 are synonyms for the ANSI types that have the 
same size, and are useful for writing portable code that behaves identically across 
multiple platforms. The __int8 data type is synonymous with type char, __int16 is 
synonymous with type short, and __int32 is synonymous with type int. The __int64 type 
has no ANSI equivalent.

*/

// [khopps 2015-12-18]
// To correct the comment above, stdint.h is NOT missing on Windows.
// Plus, the #define _STDINT does not prevent later #include <stdint.h>
// because the vs2015 stdint.h uses #pragma once instead of
// #ifndef _STDINT. So, we just include stdint.h right now.
//
#if defined(_MSC_VER)
#if _MSC_VER >= 1900
#include <stdint.h>
#endif
#ifdef _WIN64
	#define SC_64 1
#else
	#define SC_32 1
#endif
	#define SC_WIN 1
	typedef   signed __int8	 scint8_t;
	typedef unsigned __int8	 scuint8_t;
	typedef   signed __int16 scint16_t;
	typedef unsigned __int16 scuint16_t;
	typedef	  signed __int32 scint32_t;
	typedef unsigned __int32 scuint32_t;
	typedef   signed __int64 scint64_t;
	typedef unsigned __int64 scuint64_t;
	#if !defined( SC_NO_WIN_STDINT_H ) && !defined(_STDINT)
		#define _ADOBE_STDINT_H_
		#define _STDINT		// Gaurd condition for uint_fast16_t while VS 2012 upgrade
		/// stddef.h supports intptr_t and uintptr_t
		typedef   scint8_t	int8_t;
		typedef   scint8_t	int_least8_t;
		typedef   scint8_t	int_fast8_t;
		typedef  scuint8_t	uint8_t;
		typedef  scuint8_t  uint_least8_t;
		typedef  scuint8_t  uint_fast8_t;
		typedef  scint16_t	int16_t;
		typedef  scint16_t	int_least16_t;
		typedef  scint16_t	int_fast16_t;
		typedef  scuint16_t	uint16_t;
		typedef  scuint16_t	uint_least16_t;
		typedef  scuint16_t	uint_fast16_t;
		typedef	 scint32_t	int32_t;
		typedef	 scint32_t	int_least32_t;
		typedef	 scint32_t	int_fast32_t;
		typedef  scuint32_t	uint32_t;
		typedef  scuint32_t	uint_least32_t;
		typedef  scuint32_t	uint_fast32_t;
		typedef  scint64_t	int64_t;
		typedef  scint64_t	int_least64_t;
		typedef  scint64_t	int_fast64_t;
		typedef  scuint64_t	uint64_t;
		typedef  scuint64_t	uint_least64_t;
		typedef  scuint64_t	uint_fast64_t;

		typedef scint64_t	intmax_t;
		typedef scuint64_t	uintmax_t;

	#endif
	#if !defined(SC_NO_LEGACY_DATATYPES)
		// legacy data types.
		typedef scuint8_t  uchar;
		typedef unsigned int uint;
		typedef scuint16_t ushort;
		typedef scuint32_t ulong;
	#endif
	/// A wchar_t is 16 bits
	#define SC_WCHAR_WIDTH 16
	/// Macro: usage of #pragma once is OK
	#define SC_ONCE 1
	/// Macro: do not generate exception code for this method
	#define SC_NOTHROW __declspec(nothrow)
	/// Macro to define a class being a pure base class
	#define SC_PURE __declspec(novtable)
	/// Calling convention for Windows API calls
	#define SC_STDCALL __stdcall
	#if SC_DLL
	// defines for building the ScCore shared lib
	/// Define the type of class declaration (exported for DLLs, imported for stub libraries).
		#define SC_API __declspec(dllexport)
	#elif SC_LIB
		// defines for building the ScCore static lib
		#define SC_API
	#else
		// defines for accessing the ScCore DLL
		#define SC_API __declspec(dllimport)
	#endif
	#if ES_DLL
		#define ES_API __declspec(dllexport)
	#elif ES_LIB
		#define ES_API
	#else
		#define ES_API __declspec(dllimport)
    #endif

// Mac XCode

#elif defined(__APPLE__)
	#define SC_MAC 1
	#define SC_MAC_XCODE 1
#if defined(__POWERPC__)
	#define SC_MAC_XCODE_PPC 1
#else
	#define SC_MAC_XCODE_INTEL 1
#endif
#if defined(_LP64)
	#define SC_64 1
#else
	#define SC_32 1
#endif
	#define SC_XCODE 1
	#if __APPLE_CC__ >= 5442
	#define SC_XCODE3 1
	#else
	#define SC_XCODE2 1
	#endif
	#ifndef ADOBE_STDINT_H
		#define ADOBE_STDINT_H
		#include <stdint.h>
	#endif
	#if !defined(SC_NO_LEGACY_DATATYPES)
		// legacy data types.
		typedef uint8_t  uchar;
		typedef unsigned int uint;
		typedef uint16_t ushort;
		typedef uint32_t ulong;
	#endif
	#define SC_MACHO 1
	#define SC_WCHAR_WIDTH 32
	/// Macro: usage of #pragma once is not OK
	#define SC_ONCE 0
	#define SC_NOTHROW
	#define SC_PURE
	#define SC_STDCALL
	#define SC_API __attribute__((visibility("default")))
	#define ES_API __attribute__((visibility("default")))

// AIX with Visual Age CPP v6

#elif defined(__EMSCRIPTEN__)
    #define SC_WEB 1
    #define SC_64 1
    #ifndef ADOBE_STDINT_H
        #define ADOBE_STDINT_H
        #include <sys/types.h>
    #endif
    /// a wchar_t is 16 bits
    #define SC_WCHAR_WIDTH 32
    /// Macro: usage of #pragma once is not OK
    #define SC_ONCE 0
    #define SC_NOTHROW
    #define SC_PURE
    #define SC_STDCALL
    #define SC_API
    #define ES_API
#elif defined(AIX_VACPP6)
	#define SC_UNIX 1
	#define SC_AIX 1
	#define SC_32 1
	#ifndef ADOBE_STDINT_H
		#define ADOBE_STDINT_H
		#include <sys/types.h>
	#endif
	/// a wchar_t is 16 bits
	#define SC_WCHAR_WIDTH 16
	/// Macro: usage of #pragma once is not OK
	#define SC_ONCE 0
	#define SC_NOTHROW
	#define SC_PURE
	#define SC_STDCALL
	#define SC_API
	#define ES_API

// HP/UX with the HP ANSI C++ compiler A.03.33

#elif defined(HPUX_ACC)
	#define SC_UNIX 1
	#define SC_HPUX 1
	#define SC_32 1
	// enable this for HP/UX 11.1 and up
	// #define SC_HPUX_IPV6 1
	#ifndef ADOBE_STDINT_H
		#define ADOBE_STDINT_H
		#include <sys/types.h>
	#endif
	/// a wchar_t is 32 bits
	#define SC_WCHAR_WIDTH 32
	/// Macro: usage of #pragma once is not OK
	#define SC_ONCE 0
	#define SC_NOTHROW
	#define SC_PURE
	#define SC_STDCALL
	#if SC_DLL
		#define SC_API __declspec(dllexport)
	#elif SC_LIB
		#define SC_API
	#else
		#define SC_API
	#endif
	#if ES_DLL
		#define ES_API __declspec(dllexport)
	#elif ES_LIB
		#define ES_API
	#else
		#define ES_API
        #endif

// Sparc Solaris 8.0 with gcc 3.2

#elif defined(SOLARIS)
	#define SC_UNIX 1
	#define SC_32 1
	#define SC_SOLARIS 1
	// Solaris supports stdint.h as inttypes.h
	#ifndef ADOBE_STDINT_H
		#define ADOBE_STDINT_H
		#include <inttypes.h>
	#endif
	/// a wchar_t is 32 bits
	#define SC_WCHAR_WIDTH 32
	/// Macro: usage of #pragma once is not OK
	#define SC_ONCE 0
	#define SC_NOTHROW
	#define SC_PURE
	#define SC_STDCALL
	#define SC_STDCALL
	#define SC_API __attribute__((visibility("default")))
	#define ES_API __attribute__((visibility("default")))

// GCC 4.1 on all Unix flavors

#else

	#define SC_UNIX 1
	#define SC_ONCE 0
	#define SC_NOTHROW
	#define SC_PURE
	#define SC_STDCALL
	#define SC_STDCALL
	#define SC_API __attribute__((visibility("default")))
	#define ES_API __attribute__((visibility("default")))

    #if defined(LINUX)

        // Red Hat Linux 9 with gcc 4
		#define SC_LINUX 1
		#define SC_32 1
		#ifndef ADOBE_STDINT_H
		#define ADOBE_STDINT_H
			#include <stdint.h>
			#include <sys/types.h>
		#endif
		/// a wchar_t is 32 bits
		#define SC_WCHAR_WIDTH 32
		/// Macro: usage of #pragma once is not OK

	// Sparc Solaris 9.0

	#elif defined(SOLARIS)
		#define SC_SOLARIS 1
		#define SC_32 1
		/// a wchar_t is 32 bits
		#define SC_WCHAR_WIDTH 32
		// Solaris supports stdint.h as inttypes.h
		#ifndef ADOBE_STDINT_H
			#define ADOBE_STDINT_H
			#include <inttypes.h>
		#endif
	// AIX

	#elif defined(AIX)
		#define SC_AIX 1
		#ifndef ADOBE_STDINT_H
			#define ADOBE_STDINT_H
			#include <sys/types.h>
		#endif
		/// a wchar_t is 16 bits
		#define SC_WCHAR_WIDTH 16

	// HP/UX

	#elif defined(HPUX)
		#define SC_HPUX 1
		#define SC_32 1
                // Provide separate declarations for each HPUX influenced type of feature
                // in order to understand which settings need correction when compiling for 
                // HP with GCC.
		#define SC_HPUX_ENDIAN BIG
		#define SC_HPUX_MATH   math.h
                #define SC_HPUX_SOCKET yes
                #define SC_HPUX_STACK yes
		/// a wchar_t is 32 bits
		#define SC_WCHAR_WIDTH 32
		// enable this for HP/UX 11.1 and up 
        // (gcc is used for HPUX v11.11 and later)
		#define SC_HPUX_IPV6 1
		#ifndef ADOBE_STDINT_H
			#define ADOBE_STDINT_H
			#include <sys/types.h>
		#endif
	#else
		#error Unknown compiler environment
	#endif

#endif

/// This defines SO_API as a NOP (some environments defines this differently)
#ifndef SO_API
#define SO_API
#endif

/// The ScCore framework uses an all-lower case synonym for the null pointer.
#ifdef __cplusplus
#define null NULL
#endif

#if !defined( SC_WIN )
	typedef    int8_t	scint8_t;
	typedef   uint8_t	scuint8_t;
	typedef   int16_t	scint16_t;
	typedef  uint16_t	scuint16_t;
	typedef	  int32_t	scint32_t;
	typedef  uint32_t	scuint32_t;
	typedef   int64_t	scint64_t;
	typedef  uint64_t	scuint64_t;
#endif

/// An error code is 32 bits.
typedef scint32_t  scerror_t;
/// This data type is an alias for an UTF-16 encoded character.
typedef scuint8_t scutf8_t;
/// This system is strictly UTF-16. This data type takes an UTF-16 encoded character.
typedef scuint16_t scutf16_t;
/// Datatype taking an UTF-32 encoded character for systems where a wchar_t is 32 bits.
typedef scuint32_t scutf32_t;
/// A data type that shows that this value actually is an enum value.
typedef scint32_t scenum_t;

typedef	scutf8_t	utf8_t;
typedef	scutf16_t	utf16_t;
typedef	scutf32_t	utf32_t;

// assert definitions

#undef sc_assert
#undef sc_break

namespace ScCore { 
	extern void SC_API DebugBreak(); 
	extern void SC_API DebugAlert (const char* msg, const char* file, int line); 
}

#ifdef _DEBUG
	#ifdef __cplusplus
		/**
		The sc_assert macro takes two arguments, the condition to test and an error message.
		If the condition is false, a dialog pops up, letting the programmer enter the debugger.
		*/
		#define sc_assert( cond, msg ) if (!bool (cond)) { ScCore::DebugAlert (msg, __FILE__, __LINE__); }
		/**
		The sc_message macro prints a debug message in a pop-up dialog.
		*/
		#define sc_message( msg ) { ScCore::DebugAlert (msg, __FILE__, __LINE__); }
		/**
		This macro causes the procrssor to break here.
		*/
		#define	sc_break { ScCore::DebugBreak(); }
	#else
		#define sc_assert( nothing, nomsg )
		#define sc_message( nomsg )
		#define sc_break
	#endif
#else
	#define sc_assert( nothing, nomsg )
	#define sc_message( nomsg )
	#define sc_break
#endif

// Stringizing
#ifndef ADOBE_STR
	#define ADOBE_STR( x ) _ADOBE_STR( x )
	#define _ADOBE_STR( x ) #x
#endif

#endif

