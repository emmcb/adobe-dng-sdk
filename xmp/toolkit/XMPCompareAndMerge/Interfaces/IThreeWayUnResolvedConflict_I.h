#ifndef __IThreeWayUnResolvedConflict_I_h__
#define __IThreeWayUnResolvedConflict_I_h__ 1

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
#include "XMPCompareAndMerge/Interfaces/IThreeWayUnResolvedConflict.h"
#include "XMPCommon/Interfaces/BaseInterfaces/ISharedObject_I.h"

namespace AdobeXMPCompareAndMerge_Int {

#if XMP_WinBuild
	#pragma warning( push )
	#pragma warning( disable : 4250 )
#endif

	//!
	//! \brief Internal interface that represents a conflict that has not been resolved by applying any of the strategies.
	//! Provides all the functions to get all the information regarding the conflict.
	//! \attention: It is not thread safe and anyways since the client is provided with a constant object its not a need.
	//!
	class IThreeWayUnResolvedConflict_I
		: public virtual IThreeWayUnResolvedConflict
		, public virtual ISharedObject_I
	{
	public:

		//!
		//! Set the conflict reason.
		//!
		virtual void APICALL SetConflictReason( eConflictReason reason ) = 0;

		//!
		//! return the version of the interface.
		//! \return 32 bit unsigned integer representing the version of the interface.
		//!
		static uint32 GetInterfaceVersion() { return kInternalInterfaceVersionNumber; }

		virtual pIThreeWayUnResolvedConflict APICALL GetActualIThreeWayUnResolvedConflict() __NOTHROW__ { return this; }
		virtual pIThreeWayUnResolvedConflict_I APICALL GetIThreeWayUnResolvedConflict_I() __NOTHROW__ { return this; }

		virtual pvoid APICALL GetInterfacePointer( uint64 interfaceID, uint32 interfaceVersion );

		// static factory functions

		//!
		//! Creates an IThreeWayUnResolvedConflict object.
		//! \param[in] conflictReason a value of type #eConflictReason indicating the reason for conflict.
		//! \param[in] baseVersionNode a shared pointer to const #AdobeXMPCore::INode object pointing to node in base metadata object.
		//! \param[in] latestVersionNode a shared pointer to const #AdobeXMPCore::INode object pointing to node in latest metadata object.
		//! \param[in] userVersionNode a shared pointer to const #AdobeXMPCore::INode object pointing to node in user metadata object.
		//! \return a shared pointer to an IThreeWayUnResolvedConflict object
		//!
		XMP_PRIVATE static spIThreeWayUnResolvedConflict CreateThreeWayUnResolvedConflict( eConflictReason conflictReason, const spcINode & baseVersionNode,
			const spcINode & latestVersionNode, const spcINode & userVersionNode );

		//!
		//! Creates an empty list of UnResolved conflicts for a 3 way merge.
		//! \returns a shared pointer to #IThreeWayUnResolvedConflicts object.
		//!
		XMP_PRIVATE static spIThreeWayUnResolvedConflicts CreateThreeWayUnResolvedConflicts();

		//!
		//! Creates a list of UnResolved Conflicts from a 2 way unresolved conflict list.
		//! \param[in] twoWayResolvedConflictList a shared pointer to a list of two way resolved conflicts.
		//! \param[in] baseMetadata a shared pointer to const #AdobeXMPCore::IMetadata object pointing to the base metadata object.
		//! \returns a shared pointer to #IThreeWayUnResolvedConflicts object.
		//!
		XMP_PRIVATE static spIThreeWayUnResolvedConflicts CreateThreeWayUnResolvedConflicts( const spITwoWayUnResolvedConflicts & twoWayUnResolvedConflicts,
			const spcIMetadata & baseMetadata );

	protected:
		virtual ~IThreeWayUnResolvedConflict_I() __NOTHROW__ {}
		pvoid APICALL GetInterfacePointerInternal( uint64 interfaceID, uint32 interfaceVersion, bool isTopLevel );

		virtual uint32 APICALL getConflictReason( pcIError_base & error ) const __NOTHROW__;
		virtual pcINode_base APICALL getNode( sizet whichNode, pcIError_base & error ) const __NOTHROW__;
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

#endif  // __IThreeWayUnResolvedConflict_I_h__
