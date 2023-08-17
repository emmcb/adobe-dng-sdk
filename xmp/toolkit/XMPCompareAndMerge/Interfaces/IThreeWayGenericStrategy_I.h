#ifndef __IThreeWayGenericStrategy_I_h__
#define __IThreeWayGenericStrategy_I_h__ 1

// =================================================================================================
// Copyright Adobe
// Copyright 2014 Adobe
// All Rights Reserved
//
// NOTICE:  Adobe permits you to use, modify, and distribute this file in accordance with the terms
// of the Adobe license agreement accompanying it. If you have received this file from a source other 
// than Adobe, then your use, modification, or distribution of it requires the prior written permission
// of Adobe.
// =================================================================================================


#include "XMPCompareAndMerge/XMPCompareAndMergeFwdDeclarations_I.h"
#include "XMPCompareAndMerge/Interfaces/IThreeWayGenericStrategy.h"
#include "XMPCompareAndMerge/Interfaces/IThreeWayStrategy_I.h"

namespace AdobeXMPCompareAndMerge_Int {

#if XMP_WinBuild
	#pragma warning( push )
	#pragma warning( disable : 4250 )
#endif

	//!
	//! \brief Internal interface that represents an ThreeWayGenericStrategy.
	//!	\details Provides all the functions to access
	//!
	class IThreeWayGenericStrategy_I
		: public virtual IThreeWayGenericStrategy
		, public virtual IThreeWayStrategy_I
	{
	public:

		virtual pIThreeWayGenericStrategy APICALL GetGenericStrategy();

		virtual spIThreeWayGenericStrategy APICALL Clone() const = 0;

		//!
		//! return the version of the interface.
		//! \return 32 bit unsigned integer representing the version of the interface.
		//!
		static uint32 GetInterfaceVersion() { return kInternalInterfaceVersionNumber; }

		virtual pIThreeWayGenericStrategy APICALL GetActualIThreeWayGenericStrategy() __NOTHROW__ { return this; }
		virtual pIThreeWayGenericStrategy_I APICALL GetIThreeWayGenericStrategy_I() __NOTHROW__ { return this; }

		//!
		//! list all functions to be defined in this interface
		//!


		virtual pvoid APICALL GetInterfacePointer( uint64 interfaceID, uint32 interfaceVersion );

	protected:
		virtual ~IThreeWayGenericStrategy_I() __NOTHROW__ {}
		pvoid APICALL GetInterfacePointerInternal( uint64 interfaceID, uint32 interfaceVersion, bool isTopLevel );

		virtual pvoid APICALL getInterfacePointer( uint64 interfaceID, uint32 interfaceVersion, pcIError_base & error ) __NOTHROW__;

		virtual uint32 APICALL appendPath( const char * path, sizet pathSize, pcIError_base & error ) __NOTHROW__;
		virtual pcIUTF8String_base APICALL getPath( sizet index, pcIError_base & error ) const __NOTHROW__;
		virtual pIUTF8String_base APICALL removePath( const char * path, sizet pathSize, pcIError_base & error ) __NOTHROW__;


	#ifdef FRIEND_CLASS_DECLARATION
		FRIEND_CLASS_DECLARATION();
	#endif
		REQ_FRIEND_CLASS_DECLARATION();
	};

#if XMP_WinBuild
	#pragma warning( pop )
#endif

}

#endif  // __IThreeWayGenericStrategy_I_h__
