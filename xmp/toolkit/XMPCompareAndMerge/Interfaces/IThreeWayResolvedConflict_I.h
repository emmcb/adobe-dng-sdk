#ifndef __IThreeWayResolvedConflict_I_h__
#define __IThreeWayResolvedConflict_I_h__ 1

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
#include "XMPCompareAndMerge/Interfaces/IThreeWayResolvedConflict.h"
#include "XMPCommon/Interfaces/BaseInterfaces/ISharedObject_I.h"

namespace AdobeXMPCompareAndMerge_Int {

#if XMP_WinBuild
	#pragma warning( push )
	#pragma warning( disable : 4250 )
#endif

	//!
	//! \brief Internal interface that represents a conflict that has been resolved by applying one of the strategies.
	//! Provides all the functions to get all the information regarding the initial conflict and final resolution of the conflict.
	//! \attention: It is not thread safe and anyways since the client is provided with a constant object its not a need.
	//!
	class IThreeWayResolvedConflict_I
		: public virtual IThreeWayResolvedConflict
		, public virtual ISharedObject_I
	{
	public:

		//!
		//! return the version of the interface.
		//! \return 32 bit unsigned integer representing the version of the interface.
		//!
		static uint32 GetInterfaceVersion() { return kInternalInterfaceVersionNumber; }

		virtual pIThreeWayResolvedConflict APICALL GetActualIThreeWayResolvedConflict() __NOTHROW__ { return this; }
		virtual pIThreeWayResolvedConflict_I APICALL GetIThreeWayResolvedConflict_I() __NOTHROW__ { return this; }

		virtual pvoid APICALL GetInterfacePointer( uint64 interfaceID, uint32 interfaceVersion );

		// static factory functions

		//!
		//! Creates an IThreeWayResolvedConflict object.
		//! \param[in] resolutionReason a value of type #eConflictResolutionReason indicating the reason for conflict resolution.
		//! \param[in] baseVersionNode a shared pointer to const #AdobeXMPCore::INode object pointing to node in base metadata object.
		//! \param[in] latestVersionNode a shared pointer to const #AdobeXMPCore::INode object pointing to node in latest metadata object.
		//! \param[in] userVersionNode a shared pointer to const #AdobeXMPCore::INode object pointing to node in user metadata object.
		//! \param[in] resolvedNode a shared pointer to const #AdobeXMPCore::INode object pointing to the resolved node.
		//! \param[in] resolvedSource a value of type #eResolvedSource indicating the source of the resolved node.
		//! \return a shared pointer to an IThreeWayResolvedConflict object
		//!
		XMP_PRIVATE static spIThreeWayResolvedConflict CreateThreeWayResolvedConflict( eConflictResolutionReason resolutionReason, const spcINode & baseVersionNode,
			const spcINode & latestVersionNode, const spcINode & userVersionNode, const spcINode & resolvedNode, eResolvedSource resolvedSource );

		//!
		//! Creates an empty list of Resolved conflicts for a 3 way merge.
		//! \returns a shared pointer to #IThreeWayResolvedConflicts object.
		//!
		XMP_PRIVATE static spIThreeWayResolvedConflicts CreateThreeWayResolvedConflicts();

		//!
		//! Creates a list of Resolved Conflicts from a 2 way resolved conflict list.
		//! \param[in] twoWayResolvedConflictList a shared pointer to a list of two way resolved conflicts.
		//! \param[in] baseMetadata a shared pointer to const #AdobeXMPCore::IMetadata object pointing to the base metadata object.
		//! \returns a shared pointer to #IThreeWayResolvedConflicts object.
		//!
		XMP_PRIVATE static spIThreeWayResolvedConflicts CreateThreeWayResolvedConflicts( const spITwoWayResolvedConflicts & twoWayResolvedConflicts,
			const spcIMetadata & baseMetadata );

	protected:
		virtual ~IThreeWayResolvedConflict_I() __NOTHROW__ {}
		pvoid APICALL GetInterfacePointerInternal( uint64 interfaceID, uint32 interfaceVersion, bool isTopLevel );

		virtual uint32 APICALL getConflictResolutionReason( pcIError_base & error ) const __NOTHROW__;
		virtual pcINode_base APICALL getNode( sizet whichNode, pcIError_base & error ) const __NOTHROW__;
		virtual uint32 APICALL getResolvedSource( pcIError_base & error ) const __NOTHROW__;
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

#endif  // __IThreeWayResolvedConflict_I_h__
