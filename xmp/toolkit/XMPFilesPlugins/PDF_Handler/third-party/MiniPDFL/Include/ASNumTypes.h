/*********************************************************************

ADOBE SYSTEMS INCORPORATED
Copyright (C) 2009 Adobe Systems Incorporated
All rights reserved.

NOTICE: Adobe permits you to use, modify, and distribute this file
in accordance with the terms of the Adobe license agreement
accompanying it. If you have received this file from a source other
than Adobe, then your use, modification, or distribution of it
requires the prior written permission of Adobe.

*********************************************************************/

#ifndef _H_ASNUMTYPES
#define _H_ASNUMTYPES

/**
* ASNumTypes.h defines basic integer types.
*/

#if defined (__GNUC__)
#   include <inttypes.h>
#endif

#include <stddef.h>

#ifndef AS_ARCH_64BIT
#   if __LP64__ || _LP64 || defined(__arch64__) || defined(_WIN64)
#       define AS_ARCH_64BIT 1
#   else
#       define AS_ARCH_64BIT 0
#   endif
#endif //AS_ARCH_64BIT

/* integer types */
/**
	1-byte <code>signed char</code> value. 
*/
typedef signed char			ASInt8, *ASInt8P;
/**
	2-byte <code>signed short</code> numeric value. 
*/
typedef short int			ASInt16, *ASInt16P;

/**
	4-byte <code>signed long</code> numeric value. 
*/
typedef int					ASInt32, *ASInt32P;

/**
	8-byte <code>signed long</code> numeric value. 
*/
typedef signed long long int		ASInt64;

/** */
#define ASMAXInt8			((ASInt8)0x7F)
/** */
#define ASMINInt8			((ASInt8)0x80)
/** */
#define ASMAXInt16			((ASInt16)0x7FFF)
/** */
#define ASMINInt16			((ASInt16)0x8000)
/** */
#define ASMAXInt32			((ASInt32)0x7FFFFFFF)
/** */
#define ASMINInt32			((ASInt32)0x80000000)
/** */
#define ASMAXInt64			((ASInt64)0x7FFFFFFFFFFFFFFFLL)
/** */
#define ASMINInt64			((ASInt64)0x8000000000000000LL)


/* cardinal types */
/**
	1-byte <code>unsigned char</code> value. 
*/
typedef unsigned char		ASUns8, *ASUns8P;
/**
	2-byte unsigned short numeric value. 
*/
typedef unsigned short int	ASUns16, *ASUns16P;

/**
	4-byte <code>unsigned long</code> numeric value. 
*/
typedef unsigned int		ASUns32, *ASUns32P;

/**
	8-byte <code>unsigned long</code> numeric value. 
*/
typedef unsigned long long int		ASUns64;

/** */
typedef intptr_t			ASIntOrPtr;
/** */
typedef uintptr_t			ASUnsOrPtr;

/** */
#define ASMAXUns8			((ASUns8)0xFF)
/** */
#define ASMINUns8			((ASUns8)0x00)
/** */
#define ASMAXUns16			((ASUns16)0xFFFF)
/** */
#define ASMINUns16			((ASUns16)0x0000)
/** */
#define ASMAXUns32			((ASUns32)0xFFFFFFFF)
/** */
#define ASMINUns32			((ASUns32)0x00000000)
/** */
#define ASMAXUns64			((ASUns64)0xFFFFFFFFFFFFFFFFLL)
/** */
#define ASMINUns64			((ASUns64)0x0000000000000000LL)


/** ASBool */
typedef ASUns16 ASBool;




#if defined(__cplusplus)                        
#  define HAS_BOOL_SUPPORT 1
#else 
#  define HAS_BOOL_SUPPORT 0
#endif

#if !HAS_BOOL_SUPPORT
#  ifndef true
#     define true    1
#  endif
#  ifndef false
#     define false    0
#  endif
#endif

#ifndef TRUE
#  define TRUE 1
#endif

#ifndef FALSE
#  define FALSE 0
#endif

/* ASSize_t - canonical type for sizes of things in bytes. */
/** */
typedef size_t				ASSize_t;

#endif // _H_ASNUMTYPES
