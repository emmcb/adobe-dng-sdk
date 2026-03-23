/*********************************************************************************
	File: 		PDClassDefs.h
	Created:	June 16, 2003
	Purpose:	This class contains macros commonly used by all PDClasses
*
* ___________________
*
*  (c) Copyright 2002,2005,2006 Adobe Systems, Inc.
*  All Rights Reserved.
*
* NOTICE: Adobe permits you to use, modify, and distribute this file
* in accordance with the terms of the Adobe license agreement
* accompanying it. If you have received this file from a source other
* than Adobe, then your use, modification, or distribution of it
* requires the prior written permission of Adobe.
************************************************************************************/
#ifndef _PDCLASSDEFS_H
#define _PDCLASSDEFS_H

#if defined (__cplusplus)

/* 
   - The base class for the common C++ objects differs depending 
   on the place it is being used. The below macro defines a constant
   BASECLASS that will contain the required base class based on the context

   - INTERNAL_PLUGIN is for plugins internal to Acrobat. This define
   allows the class to use classes that are internal to Acrobat. This is a new
   define and needs to be added to internal plugins that use this class.
*/
#if (EXCHANGE || READER)
#define BASECLASS	AcroStdBase
#elif INTERNAL_PLUGIN
#define BASECLASS	CmiAlloc
#endif

/* Way for clients of this header to define their own exception handling mechanism
   for exception raised as a result of calls made by the class methods. A sample 
   CPPError.h file could contain the following lines
   #define ASTRY						DURING
   #define END_ASTRY					HANDLER { throw ERRORCODE; } END_HANDLER		
*/
#if CPP_EXCEPTIONS
#include "CPPError.h"
#else
#ifndef ASTRY
#define ASTRY
#define END_ASTRY
#endif
#endif	/* CPP_EXCEPTIONS */

#endif /* __cplusplus */

#endif /* _PDCLASSDEFS_H */
