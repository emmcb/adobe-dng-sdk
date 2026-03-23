#ifndef IThreeWayStrategy_I_h__
#define IThreeWayStrategy_I_h__ 1

// =================================================================================================
// Copyright Adobe
// Copyright 2015 Adobe
// All Rights Reserved
//
// NOTICE:  Adobe permits you to use, modify, and distribute this file in accordance with the terms
// of the Adobe license agreement accompanying it. If you have received this file from a source other 
// than Adobe, then your use, modification, or distribution of it requires the prior written permission
// of Adobe.
// =================================================================================================


#include "XMPCompareAndMerge/XMPCompareAndMergeFwdDeclarations_I.h"
#include "XMPCompareAndMerge/Interfaces/IThreeWayStrategy.h"
#include "XMPCommon/Interfaces/BaseInterfaces/ISharedObject_I.h"

namespace AdobeXMPCompareAndMerge_Int {

#if XMP_WinBuild
	#pragma warning( push )
	#pragma warning( disable : 4250 )
#endif

	//!
	//! \brief Internal interface that represents an interface that allows clients to resolve conflicts during 3 way merge operation.
	//! Provides all the functions
	//!		- show inclination for resolving the conflict
	//!		- try to solve the conflict
	//!
	class IThreeWayStrategy_I
		: public virtual IThreeWayStrategy
		, public virtual ISharedObject_I
	{
	public:
		//!
		// @{
		//! Returns the underlying Generic strategy if possible.
		//! \return a pointer to generic strategy object in case the strategy is a generic strategy object, null otherwise.
		//!
		virtual pIThreeWayGenericStrategy APICALL GetGenericStrategy() { return NULL; }
		XMP_PRIVATE pcIThreeWayGenericStrategy GetGenericStrategy() const {
			return const_cast< IThreeWayStrategy_I * >( this )->GetGenericStrategy();
		}

		// static factory functions

		//!
		//! Creates a shared pointer to an IThreeWayStrategy object from external strategy pointer.
		//! \param[in] strategy a pointer to an external strategy object.
		//! \return a shared pointer to an IThreeWayStrategy object.
		//!
		XMP_PRIVATE static spIThreeWayStrategy CreateThreeWayStrategy( pIThreeWayStrategy_base strategy );

	protected:
		virtual ~IThreeWayStrategy_I() __NOTHROW__ {}

	#ifdef FRIEND_CLASS_DECLARATION
		FRIEND_CLASS_DECLARATION();
	#endif
		REQ_FRIEND_CLASS_DECLARATION();
	};

#if XMP_WinBuild
	#pragma warning( pop )
#endif

}
#endif  // IThreeWayStrategy_I_h__

