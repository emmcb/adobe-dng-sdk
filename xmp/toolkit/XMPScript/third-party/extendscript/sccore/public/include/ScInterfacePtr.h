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
*  Copyright 1998 - 2002 Adobe Systems Incorporated
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
#ifndef _ScInterfacePtr_h
#define _ScInterfacePtr_h

#include "ScRoot.h"

namespace ScCore
{

/**
A smart pointer class for Interface pointers.

A smart pointer class to encapsulate reference-counted MultiScript interface pointers.
The primary focus is LiveObjects (and derived classes), but it can be used with any
class that supports "addRef" and "release". It handles the reference-counting semantics, 
including clean-up when exceptions are encountered.
<p>
The smart pointer functionality includes the following:
<ol>
<li>If the InterfacePtr instance was allocated on the stack, its
	destructor automatically calls release on its encapsulated interface
    pointer when the InterfacePtr instance goes out of scope. This ensures
    that reference-counting semantics are preserved when exceptions occur,
    without requiring additional exception-handling code.
<li>If the InterfacePtr instance was created with the new operator, its
	destructor automatically calls release on its encapsulated interface
    pointer when delete is called.
<li>The InterfacePtr class automatically calls release on its encapsulated
    interface pointer before overwriting it during assignment.
<li>Assignment of a NULL interface pointer to a InterfacePtr instance
   is allowed.
<li>The InterfacePtr class automatically calls addRef on any non-NULL
    interface pointer it receives in an assignment operation.
<li>To avoid conflicts with some STL implementations operator& is NOT defined
	for the class.
<li>The InterfacePtr class provides the appropriate comparison operators
    to support STL collections of smart pointer instances.
</ol>
*/

template <class Interface>
class InterfacePtr : public Heap
{
private:    
    /**
     The encapsulated interface pointer.
     */
			Interface*		mInterfacePtr;

public:
    /**
    Default constructor. Initializes the encapsulated interface pointer
    to NULL.
    */
							InterfacePtr() : mInterfacePtr( NULL ) {}

    /**
    Construct from a pointer of type Interface. addRef is called on the
    source interface pointer, if non-NULL.
    @param	sourcePtr		The interface pointer to encapsulate
    */
							InterfacePtr (Interface* sourcePtr);

    /**
    Construct from another InterfacePtr instance. addRef is called on
    the source instance's encapsulated interface pointer, if non-NULL.
    
	@param		sourcePtr	The smart pointer to copy
    */
							InterfacePtr(const InterfacePtr<Interface>& sourcePtr);
    
    /**
    Releases the encapsulated interface pointer, if non-NULL.
    */
	inline					~InterfacePtr();
    
	/**
    Assign a pointer of type Interface. Any existing encapsulated interface
    is released, and addRef is called on the source interface pointer,
    if non-NULL.
    
	@param	sourcePtr		The interface pointer to encapsulate
    @return					The encapsulated interface pointer, with addRef.
    */
	inline	InterfacePtr<Interface>& operator=( const Interface* sourcePtr );
    
    /**
    Assign another InterfacePtr instance. Any existing encapsulated
    interface is released, and addRef is called on the source instance's
    encapsulated interface pointer, if non-NULL.

    @param	sourcePtr		The InterfacePtr instance to copy
    @return					The new InterfacePtr instance, with addRef.
     */
	inline	InterfacePtr<Interface>& operator=( const InterfacePtr<Interface>& sourcePtr );

	/**
    Access the encapsulated pointer. To preserve the smart pointer's
    reference-counting semantics, access to the addRef and release methods
    of the encapsulated interface is not allowed through the -> operator.
     
    @return					The encapsulated interface pointer
    */
	inline					operator Interface*() const	{ return mInterfacePtr; }
	///
	inline					operator const Interface*() const { return mInterfacePtr; }
	///
	inline	Interface*		getInterface() const { return mInterfacePtr; }
 	///
	inline Interface*		operator->() const	{ sc_assert (NULL != mInterfacePtr, "No interface"); return mInterfacePtr; }
   
    /**
    Determine if the binary value of the encapsulated pointer is equal to
    the binary value of a raw interface pointer.
     
    @param	testPtr			The raw interface pointer to test
    @return					True if the binary values of the pointers are equal
    */
    inline	bool			operator==(const Interface* testPtr) const { return ( mInterfacePtr == testPtr ); }

    /**
    Determine if the binary value of the encapsulated pointer is not equal to
    the binary value of a raw interface pointer.
     
    @param	testPtr			The raw interface pointer to test
    @return					True if the binary values of the pointers are not equal
    */
    inline	bool			operator!=(const Interface* testPtr) const { return ( mInterfacePtr != testPtr ); }
    
    /**
    Determine if the binary value of the encapsulated pointer is less than
    the binary value of a raw interface pointer. This method is provided to
    support STL collections of smart pointer instances.
     
    @param	testPtr			The raw interface pointer to test
    @return					True if the binary value of the encapsulated pointer is less
    */
	inline	bool			operator < (Interface* testPtr) const { return ( mInterfacePtr < testPtr ); }
    
    /**
    Aquire ownership of a raw interface pointer, without addRef. Any existing
    encapsulated interface is released.
     
    @param	sourcePtr		The smart pointer to acquire
    */
	inline	void			attach(Interface* newPtr);
    
    /**
    Transfer ownership of the encapsulated pointer to a raw interface pointer,
    without addRef. The encapsulated interface pointer is set to NULL, so
    release will not be called in the smart pointer's destructor. The caller
    becomes responsible for calling release on the interface.
     
    @return					The encapsulated interface pointer, without addRef
    */
	inline	Interface*		detach();
};

// ctor from Interface*
template <class Interface>
InterfacePtr<Interface>::InterfacePtr(Interface* sourcePtr)
{
    if ((mInterfacePtr = sourcePtr) != NULL)
    {
        mInterfacePtr->addRef();
    }
}

// ctor from InterfacePtr
template <class Interface>
InterfacePtr<Interface>::InterfacePtr(const InterfacePtr<Interface>& sourcePtr)
{
    if ( (mInterfacePtr = sourcePtr.getInterface() ) != NULL)
    {
        mInterfacePtr->addRef();
    }
}

// dtor
template <class Interface>
InterfacePtr<Interface>::~InterfacePtr()
{
	try {
	    if (mInterfacePtr != NULL)
	    {
	        mInterfacePtr->release();
	    }
	}
	catch (...)
	{
		sc_message( "unhandled exception caught in ~InterfacePtr" );
	}
}

// assign an Interface*
template <class Interface>
InterfacePtr<Interface>& InterfacePtr<Interface>::operator=(const Interface* sourcePtr)
{
	if ( NULL != sourcePtr )
	{
		sourcePtr->addRef();
	}
	if (mInterfacePtr != NULL)
    {
        mInterfacePtr->release();
    }

    mInterfacePtr = const_cast<Interface*>( sourcePtr );
    
    return *this;
}

// assign a InterfacePtr
template <class Interface>
InterfacePtr<Interface>& InterfacePtr<Interface>::operator=(const InterfacePtr<Interface>& sourcePtr)
{
	if ( NULL != sourcePtr.mInterfacePtr )
	{
		sourcePtr.mInterfacePtr->addRef();
	}
	if (mInterfacePtr != NULL)
	{
	    mInterfacePtr->release();
	}
    mInterfacePtr = sourcePtr.mInterfacePtr;
    
    return *this;
}

// attach to an Interface*
template <class Interface>
void InterfacePtr<Interface>::attach(Interface* newPtr)
{
    if (mInterfacePtr != NULL)
    {
        mInterfacePtr->release();
    }
    mInterfacePtr = newPtr;
}

// detach the encapsulated pointer
template <class Interface>
Interface* InterfacePtr<Interface>::detach()
{
    Interface * const savedPtr = mInterfacePtr;
    mInterfacePtr = NULL;
    return savedPtr;
}

}		// end Namespace

/*
		( afb )		Andy Bachorski
		(jsbache)	Jesper Storm Bache

	Change History ( most recent first ):
		<>		 6/13/02	jsbache	Disabled the "AddRef_Or_Release_Not_Allowed" wrapper class as it resulted in
									(correct) warnings using VC7, and we don't really need the safe-guard mechanism
									since we need to call AddReff/Release in very few & defined places
		<>		05/03/02	jsbache	Port to MultiScript
		<>		12/04/01	afb		Don't include SPTypes.h.
		<>		 8/29/01	jsbache Integrated MCOM change
		<>		 6/08/01	jsbache	Added a specific method for extracting the interface (GetInterface)
									This makes code easier to read. Instead of haing to write:
										dynamic_cast<foo*>( static_cast<bar*>( pInterface ) );
									you can now do the following instead:
										dynamic_cast<foo*>( pInterface.GetInterface() );
	   <9>		 6/04/01	afb		Replace #include of McErrors.h with CMcError.h.
		<>		 2/12/01	jsbache	QueryInterface / AcquireInterface changes
		<>		12/19/00	jsbache	Header added & 'verify' replaced by require

*/

#endif

