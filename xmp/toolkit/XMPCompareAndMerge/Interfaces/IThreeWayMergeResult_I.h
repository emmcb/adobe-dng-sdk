#ifndef __IThreeWayMergeResult_I_h__
#define __IThreeWayMergeResult_I_h__ 1

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
#include "XMPCompareAndMerge/Interfaces/IThreeWayMergeResult.h"
#include "XMPCommon/Interfaces/BaseInterfaces/ISharedObject_I.h"

namespace AdobeXMPCompareAndMerge_Int {

#if XMP_WinBuild
	#pragma warning( push )
	#pragma warning( disable : 4250 )
#endif

	//!
	//! \brief Internal interface that represents an ThreeWayMergeResult.
	//!	\details Provides all the functions to access
	//!
	class IThreeWayMergeResult_I
		: public virtual IThreeWayMergeResult
		, public virtual ISharedObject_I
	{
	public:
        using IThreeWayMergeResult::GetResolvedConflicts;
        using IThreeWayMergeResult::GetUnResolvedConflicts;
		virtual spIThreeWayResolvedConflicts APICALL GetResolvedConflicts() = 0;
		virtual spIThreeWayUnResolvedConflicts APICALL GetUnResolvedConflicts() = 0;

		//!
		//! return the version of the interface.
		//! \return 32 bit unsigned integer representing the version of the interface.
		//!
		static uint32 GetInterfaceVersion() { return kInternalInterfaceVersionNumber; }

		virtual pIThreeWayMergeResult APICALL GetActualIThreeWayMergeResult() __NOTHROW__ { return this; }
		virtual pIThreeWayMergeResult_I APICALL GetIThreeWayMergeResult_I() __NOTHROW__ { return this; }

		virtual pvoid APICALL GetInterfacePointer( uint64 interfaceID, uint32 interfaceVersion );

		// static factory functions

		//!
		//! Creates an IThreeWayMergeResult object.
		//! \return a shared pointer to an IThreeWayMergeResult object
		//!
		XMP_PRIVATE static spIThreeWayMergeResult CreateThreeWayMergeResult( const spcIMetadata & latestVersionMetadata, const spcIMetadata & userVersionMetadata,
			const spIThreeWayResolvedConflicts & resolvedConflicts, const spIThreeWayUnResolvedConflicts & unResolvedConflictList );

	protected:
		virtual ~IThreeWayMergeResult_I() __NOTHROW__ {}
		pvoid APICALL GetInterfacePointerInternal( uint64 interfaceID, uint32 interfaceVersion, bool isTopLevel );

		virtual pIMetadata_base APICALL getMergedMetadata( pcIError_base & err ) const __NOTHROW__;
		virtual uint32 APICALL getMergedStatus( pcIError_base & error ) const __NOTHROW__;
		virtual pcIThreeWayResolvedConflict_base APICALL getResolvedConflict( sizet index, pcIError_base & error ) const __NOTHROW__;
		virtual pcIThreeWayUnResolvedConflict_base APICALL getUnResolvedConflict( sizet index, pcIError_base & error ) const __NOTHROW__;
		virtual pvoid APICALL getInterfacePointer( uint64 interfaceID, uint32 interfaceVersion, pcIError_base & error ) __NOTHROW__;

	#ifdef FRIEND_CLASS_DECLARATION
		FRIEND_CLASS_DECLARATION();
	#endif
		REQ_FRIEND_CLASS_DECLARATION();
	};

#if XMP_WinBuild
	#pragma warning( pop )
#endif

}

#endif  // __IThreeWayMergeResult_I_h__
