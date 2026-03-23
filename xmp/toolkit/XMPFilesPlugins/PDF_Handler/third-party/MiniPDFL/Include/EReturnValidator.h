/*********************************************************************************
	File: 		EReturnValidator.h
	Created:	October 10, 2004
	Purpose:	This file contains code that makes sure ASRaiseAware objects are
				not used inside of E_RETURN.

*
* ___________________
*
*  (c) Copyright 2004-2006 Adobe Systems, Inc.
*  All Rights Reserved.
*
* NOTICE: Adobe permits you to use, modify, and distribute this file
* in accordance with the terms of the Adobe license agreement
* accompanying it. If you have received this file from a source other
* than Adobe, then your use, modification, or distribution of it
* requires the prior written permission of Adobe.
************************************************************************************/
#ifndef _H_ERETURNVALIDATOR
#define _H_ERETURNVALIDATOR

#ifdef __cplusplus

/*  These are templates that make sure ASRaiseAware clases are not used 
	inside of a E_RETURN. Returning an ASRaiseAware class inside of 
	E_RETURN will leave cause the exception frame stack to go out of
	order leaving the wrong frame at the top of the stack. This could
	lead to a crash in the event an ASRaise is called
*/
#if (defined(DEBUG) && defined(__MWERKS__) && defined(MAC_ENV) && PLUGIN)
	#define DO_ADDITIONAL_RAISEAWARE_CHECKS 1
#else
	#define DO_ADDITIONAL_RAISEAWARE_CHECKS 0
#endif	

#if DO_ADDITIONAL_RAISEAWARE_CHECKS

template < class T >
struct IsRaiseAwareClass
{
	typedef int You_Can_Not_E_Return_Raise_Aware_Objects;
};

template < class T >
inline void IsRaiseAwareClassHelperFunction( T* pThis )
{
	typename IsRaiseAwareClass< T >::Raise_Aware_Classes_Must_Use_The_Macro_RAISEAWARECLASS tester = 0;
}

template < class T >
inline void IsNotRaiseAwareClassHelperFunction( T someInstance )
{
	typename IsRaiseAwareClass< T >::You_Can_Not_E_Return_Raise_Aware_Objects tester = 0;
}

#endif /* DO_ADDITIONAL_RAISEAWARE_CHECKS */

#endif /* __cplusplus */

#endif /* _H_ERETURNVALIDATOR */

