#ifndef __ITwoWayResolvedConflict_h__
#define __ITwoWayResolvedConflict_h__ 1

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
#include "XMPCommon/Interfaces/BaseInterfaces/ISharedObject_I.h"

#include "XMPCompareAndMerge/Interfaces/IThreeWayResolvedConflict.h"

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
	class ITwoWayResolvedConflict
		:public virtual ISharedObject_I
	{
	public:

		//!
		//! Get the reason for the conflict resolution.
		//! \return a value of #IThreeWayResolvedConflict::eConflictResolutionReason indicating the reason for the conflict resolution.
		//!
		virtual IThreeWayResolvedConflict::eConflictResolutionReason APICALL GetConflictResolutionReason() const = 0;

		//!
		//! Get the conflicting node as it is present in the base metadata object.
		//! \return a shared pointer to const #AdobeXMPCore::INode interface.
		//!
		virtual spcINode APICALL GetBaseVersionNode() const = 0;

		//!
		//! Get the conflicting node as it is present in the latest metadata object.
		//! \return a shared pointer to const #AdobeXMPCore::INode interface.
		//!
		virtual spcINode APICALL GetLatestVersionNode() const = 0;

		//!
		//! Get the resolved node.
		//! \return a shared pointer to const #AdobeXMPCore::INode interface.
		//!
		virtual spcINode APICALL GetResolvedNode() const = 0;

		//!
		//! Get the source of the resolved node.
		//! \return a value of #IThreeWayResolvedConflict::eResolvedSource indicating the source of the resolved node.
		//!
		virtual IThreeWayResolvedConflict::eResolvedSource APICALL GetResolvedSource() const = 0;

		//!
		//! Get the two way strategy used to resolve the conflict.
		//! \return a shared pointer to const #ITwoWayConflictResolutionStartegy object used to resolve the conflict.
		//!
		virtual spcITwoWayConflictResolutionStrategy APICALL GetStrategyUsed() const = 0;

		//!
		//! Allows to change the resolved node and associated properties later.
		//! \param[in] resolvedSource a value of type #IThreeWayResolvedConflict::eResolvedSource indicating the source of the resolved node.
		//! \param[in] strategyUsed a shared pointer to const #ITwoWayConflictResolutionStartegy object used to resolve the conflict.
		//! \param[in] resolvedNode a shared pointer to const #AdobeXMPCore::INode object pointing to the resolved node.
		//!
		virtual void APICALL ModifyResolvedNode( IThreeWayResolvedConflict::eResolvedSource resolvedSource,
			spcITwoWayConflictResolutionStrategy & strategyUsed, const spcINode & resolvedNode ) = 0;

		//!
		//! Creates an ITwoWayResolvedConflict object.
		//! \param[in] resolutionReason a value of type #IThreeWayResolvedConflict::eConflictResolutionReason indicating the reason for conflict resolution.
		//! \param[in] baseVersionNode a shared pointer to const #AdobeXMPCore::INode object pointing to node in base metadata object.
		//! \param[in] latestVersionNode a shared pointer to const #AdobeXMPCore::INode object pointing to node in latest metadata object.
		//! \param[in] resolvedNode a shared pointer to const #AdobeXMPCore::INode object pointing to the resolved node.
		//! \param[in] resolvedSource a value of type #IThreeWayResolvedConflict::eResolvedSource indicating the source of the resolved node.
		//! \param[in] strategyUsed a shared pointer to const #ITwoWayConflictResolutionStartegy object used to resolve the conflict.
		//! \return a shared pointer to an IResolvedConflict3Way object
		//!
		XMP_PRIVATE static spITwoWayResolvedConflict CreateTwoWayResolvedConflict( IThreeWayResolvedConflict::eConflictResolutionReason resolutionReason,
			const spcINode & baseVersionNode, const spcINode & latestVersionNode, const spcINode & resolvedNode,
			IThreeWayResolvedConflict_base::eResolvedSource resolvedSource, const spcITwoWayConflictResolutionStrategy & strategyUsed );

		//!
		//! Creates an empty list of Resolved conflicts for a 2 way merge.
		//! \returns a shared pointer to #ITwoWayResolvedConflicts object.
		//!
		XMP_PRIVATE static spITwoWayResolvedConflicts CreateTwoWayResolvedConflicts();

	protected:
		virtual ~ITwoWayResolvedConflict() __NOTHROW__ {}

	#ifdef FRIEND_CLASS_DECLARATION
		FRIEND_CLASS_DECLARATION();
	#endif
		REQ_FRIEND_CLASS_DECLARATION();
	};

#if XMP_WinBuild
	#pragma warning( pop )
#endif

}

#endif  // __ITwoWayResolvedConflict_h__
