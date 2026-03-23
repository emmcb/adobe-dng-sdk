#ifndef __IGenericStrategyDatabase_I_h__
#define __IGenericStrategyDatabase_I_h__ 1

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
#include "XMPCompareAndMerge/Interfaces/IGenericStrategyDatabase.h"
#include "XMPCommon/Interfaces/BaseInterfaces/ISharedObject_I.h"

namespace AdobeXMPCompareAndMerge_Int {

#if XMP_WinBuild
	#pragma warning( push )
	#pragma warning( disable : 4250 )
#endif

	//!
	//! \brief Internal interface that represents a database of generic strategies.
	//! Provides functions to 
	//!		- Retrieve generic strategy objects from the database.
	//!
	class IGenericStrategyDatabase_I
		: public virtual IGenericStrategyDatabase
		, public virtual ISharedObject_I
	{
	public:

		//!
		//! return the version of the interface.
		//! \return 32 bit unsigned integer representing the version of the interface.
		//!
		static uint32 GetInterfaceVersion() { return kInternalInterfaceVersionNumber; }

		virtual pIGenericStrategyDatabase APICALL GetActualIGenericStrategyDatabase() __NOTHROW__ { return this; }
		virtual pIGenericStrategyDatabase_I APICALL GetIGenericStrategyDatabase_I() __NOTHROW__ { return this; }

		virtual pvoid APICALL GetInterfacePointer( uint64 interfaceID, uint32 interfaceVersion );

		// static factory functions

		//!
		//! Creates an IGenericStrategyDatabase object.
		//! \return a shared pointer to an IGenericStrategyDatabase object
		//!
		XMP_PRIVATE static spIGenericStrategyDatabase CreateGenericStrategyDatabase();

		//!
		//! Destroys the IGenericStrategyDatabase object.
		//!
		XMP_PRIVATE static void DestoryGenericStrategyDatabase();

	protected:
		virtual ~IGenericStrategyDatabase_I() __NOTHROW__ {}
		pvoid APICALL GetInterfacePointerInternal( uint64 interfaceID, uint32 interfaceVersion, bool isTopLevel );

		virtual pvoid APICALL getInterfacePointer( uint64 interfaceID, uint32 interfaceVersion, pcIError_base & error ) __NOTHROW__;

		virtual pIThreeWayGenericStrategy_base APICALL getThreeWayGenericStrategy( uint32 genericStrategyType, pcIError_base & error ) const __NOTHROW__;

	#ifdef FRIEND_CLASS_DECLARATION
		FRIEND_CLASS_DECLARATION();
	#endif
		REQ_FRIEND_CLASS_DECLARATION();
	};

#if XMP_WinBuild
	#pragma warning( pop )
#endif

}

#endif  // __IConflictResolutionStrategyDatabase_I_h__
