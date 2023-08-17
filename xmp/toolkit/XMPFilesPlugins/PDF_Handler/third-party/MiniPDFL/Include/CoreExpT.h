/*********************************************************************

 ADOBE SYSTEMS INCORPORATED
 Copyright (C) 1994-2007 Adobe Systems Incorporated
 All rights reserved.

 NOTICE: Adobe permits you to use, modify, and distribute this file
 in accordance with the terms of the Adobe license agreement
 accompanying it. If you have received this file from a source other
 than Adobe, then your use, modification, or distribution of it
 requires the prior written permission of Adobe.

 ---------------------------------------------------------------------

 CoreExpT.h

 - Types, macros, structures, etc. required to use the Core HFT.

*********************************************************************/

#ifndef _H_CoreExpT
#define _H_CoreExpT

#include "Environ.h"
#include "ASNumTypes.h"
#include "MiniPDFLConfig.h"

#if UNIX_PLATFORM || MAC_PLATFORM
#include <sys/types.h>
#endif

#if ((MAC_PLATFORM || UNIX_PLATFORM) && __LP64__) || (WIN_PLATFORM && _WIN64)
#define POINTER_64_BITS 1
#endif

/* for converting from ASBool to bool */
#ifdef __cplusplus
inline bool ASBoolToBool(ASBool b)
{
#  if defined(ASDebugAssert)
	   ASDebugAssert((b == 1) || (b == 0));
#  elif defined(miAssert)
	   miAssert((b == 1) || (b == 0));
#  endif
   return b != 0;
}
#else
#  define ASBoolToBool(boolval) (boolval != FALSE)
#endif

/* These types have been introduced to give us explicit control over
** the sizes of various enums we exported as such through our 2.0 plug-in
** API.  Enums are not sized the same way on the two platforms.
*/
#if WIN_PLATFORM

/**
1-byte enumeration with values from <code>0</code> to <code>127</code>, used in data 
structures. 
*/
typedef ASInt16 ASEnum8;
/**
2-byte enumeration with values from <code>0</code> to <code>32,767</code>, used in 
data structures. 
*/
typedef ASInt16 ASEnum16;

/** */
#  define kASMAXEnum8 ASMAXInt16
/** */
#  define kASMAXEnum16 ASMAXInt16

#else //!WIN_PLATFORM

/** */
typedef ASInt8 ASEnum8;
/**
2-byte enumeration with values from <code>0</code> to <code>32,767</code>, used in 
data structures. 
*/
typedef ASInt16 ASEnum16;

/** */
#  define kASMAXEnum8 ASMAXInt8
/** */
#  define kASMAXEnum16 ASMAXInt16

#endif

#if PLUGIN || ACROBAT_LIBRARY
#  include "AcroErr.h"
#endif

/* Generic pointers */
#ifndef NULL
#  ifdef __cplusplus
#  define NULL    0
#else
#  define NULL    ((void *)0)
#  endif
#endif

/* Opaque basetypes */
typedef ASInt32				OPAQUE_32_BITS;
typedef struct OPAQUE_64_BITS
{
	OPAQUE_32_BITS a, b;
}
OPAQUE_64_BITS;

/* HugePtr definition
** This is required for Intel processors when 
** running in a segmented mode. The HUGETYPE specifies
** whatever keyword is required to make a pointer 
** valid when accessing blocks of memory greater than 64K.
** For non-Intel machines, it is defined as nothing.
*/
#ifndef HUGEPTRTYPE
#  define HUGEPTRTYPE
#endif
#define HugePtr char HUGEPTRTYPE *

/*------------------------------------------------------------------------
typedef of ASAtom object
------------------------------------------------------------------------*/
#ifndef HAS_32BIT_ATOMS
#   if TOOLKIT || ACROBAT_LIBRARY || PDFLPI || PDFL_EXTENSION
#     define HAS_32BIT_ATOMS 1
#   else
#     define HAS_32BIT_ATOMS 0
#   endif
#endif

#if HAS_32BIT_ATOMS
typedef ASUns32 ASAtom;			/* PDFLib uses 32bit ASAtom :-) */
#  define	ASAtomNull ASMAXUns32
#else
typedef ASUns16 ASAtom;			/* this could be an ASUns32. See RecLst.h */
#  define	ASAtomNull ASMAXUns16
#endif


/** 
	Uniquely identifies an entry within an HFT.
	It is simply the integer offset of the entry from the start of the HFT.
*/
typedef ASInt32 Selector;
#define BAD_SELECTOR		0


/** 
	An HFTEntry may be cast to a pointer to a function whose prototype must
	be provided by the HFT's description file.
*/
typedef void *HFTEntry;

/** 
	An object that describes a set of exported functions.
	It is an array of function pointers, where the first element is unused.

	@note An HFT object may be cast to an <code>(HFTEntry *)</code>; you may then
	index directly into this object by a selector to obtain a pointer to
	a function.
*/
#if !defined(HFT_IMPL) || !HFT_IMPL
typedef HFTEntry *HFT;
#endif
/* bit to set for beta versions */
#define kHFT_IN_BETA_FLAG 0x80000000


/**
	A flag that specifies whether an HFT entry is replaceable: 
	<ul>
	<li> If the flag is set, the new entry can be replaced. Clients should 
	generally use this value, allowing other clients to subsequently 
	replace the method again. </li>
	<li> If the flag is not set, the new entry cannot be replaced. </li>
	</ul>
	@see HFTReplaceEntry
*/
#define HFTEntryReplaceable			(0x00000001)


/**
	An opaque pointer to an object that identifies a specific loaded plug-in. A unique
	ASExtension object is created for each plug-in when it is loaded. If the plug-in fails
	to initialize, the ASExtension remains but is marked as inactive.
	@see ASEnumExtensions
*/
typedef struct _t_ASExtension *ASExtension;
/** */
typedef ASExtension ExtensionID ;
/** */
typedef void *ASCallback;


/** This constant is returned by ASGetConfiguration() when the selector
	passed in is unknown to the application.
*/
#define UNDEFINED_CONFIGURATION_SELECTOR ((void *)-1)


/**
	Environment-restoration functions are called when an exception is raised. 
	@param asEnviron The environment to restore.
*/
typedef ACCBPROTO1 void (ACCBPROTO2 *restoreEnvironProc)(void *asEnviron);

/* not documenting these as they are obsolete. */
#define ASUSE_OBSOLETE_TYPES 1	/* XXX - temp */
#if ASUSE_OBSOLETE_TYPES
#define Int8	ASInt8
#define Int8P	ASInt8P
#define Int16	ASInt16
#define Int16P	ASInt16P
#define Int32	ASInt32
#define Int32P	ASInt32P

#define Uns8	ASUns8
#define Uns8P	ASUns8P
#define Uns16	ASUns16
#define Uns16P	ASUns16P
#define Uns32	ASUns32
#define Uns32P	ASUns32P

#define MAXInt8 	ASMAXInt8
#define MINInt8 	ASMINInt8
#define MAXInt16	ASMAXInt16
#define MINInt16	ASMINInt16
#define MAXInt32	ASMAXInt32
#define MINInt32	ASMINInt32

#define MAXUns8		ASMAXUns8	
#define MINUns8		ASMINUns8	
#define MAXUns16	ASMAXUns16	
#define MINUns16	ASMINUns16	
#define MAXUns32	ASMAXUns32	
#define MINUns32	ASMINUns32	

#ifndef _WIN32    /* We cannot do this for WIN32 since it has a different size for boolean */
#define boolean ASBool
#endif

#define os_size_t			ASSize_t
#endif /* ASUSE_OBSOLETE_TYPES */

#define FixedPointP		..Use.ASFixedPointP.instead..
#define FixedRectP		..Use.ASFixedRectP.instead..
#define FixedQuad		..Use.ASFixedQuad.instead..
#define FixedQuadP		..Use.ASFixedQuadP.instead..
#define FixedMatrix		..Use.ASFixedMatrix.instead..
#define FixedMatrixP	..Use.ASFixedMatrixP.instead..

/**
	Enumeration function for ASEnumExtensions(). 
	@param extension The ASExtension for a client. 
	@param clientData User-supplied data that was passed in 
	the call to ASEnumExtensions().
	@return If <code>false</code>, enumeration halts and the ASExtension on which 
	enumeration halted is returned. If <code>true</code>, enumeration continues. 
	
	@see ASEnumExtensions 
*/
typedef ACCBPROTO1 ASBool (ACCBPROTO2 *ASExtensionEnumProc)(
	ASExtension extension,
	void *clientData);

#endif /* _H_CoreExpT */
