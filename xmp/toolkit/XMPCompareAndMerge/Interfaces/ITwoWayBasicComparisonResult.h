#ifndef __ITwoWayBasicComparisonResult_h__
#define __ITwoWayBasicComparisonResult_h__ 1

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

#include "XMPCompareAndMerge/Interfaces/ITwoWayUnResolvedConflict.h"

#include "XMPCore/Interfaces/INode.h"
#include "XMPCore/Interfaces/IArrayNode.h"

namespace AdobeXMPCompareAndMerge_Int {

#if XMP_WinBuild
	#pragma warning( push )
	#pragma warning( disable : 4250 )
#endif

	//!
	//! \brief Internal interface that represents an object which takes care of basic comparison of the two nodes.
	//!	\details Provides all the functions to
	//!		- get access to various basic properties of nodes from base and latest metadata objects.
	//!		- Check if two nodes are exactly equal or equal ignoring certain conditions.
	//!		- Set various conflict reasons on the basic properties.
	//!
	class ITwoWayBasicComparisonResult
		:public virtual ISharedObject_I
	{
	public:

		virtual ITwoWayUnResolvedConflict::eTwoWayConflictReason APICALL GetConflictReason() const = 0;


		virtual INode::eNodeType APICALL GetBaseNodeType() const = 0;
		virtual INode::eNodeType APICALL GetLatestNodeType() const = 0;

		virtual sizet APICALL GetBaseChildCount() const = 0;
		virtual sizet APICALL GetLatestChildCount() const = 0;

		virtual IArrayNode::eArrayForm APICALL GetBaseArrayType() const = 0;
		virtual IArrayNode::eArrayForm APICALL GetLatestArrayType() const = 0;

		virtual INode::eNodeType APICALL GetBaseArrayElementType() const = 0;
		virtual INode::eNodeType APICALL GetLatestArrayElementType() const = 0;


		virtual bool APICALL IsEqual( ITwoWayUnResolvedConflict::eTwoWayConflictReason mask = ITwoWayUnResolvedConflict::kTWCRNone ) const = 0;

		virtual bool APICALL IsFollowingConflictPresent( ITwoWayUnResolvedConflict::eTwoWayConflictReason conflictReason ) const = 0;

		virtual void APICALL AddConflictType( ITwoWayUnResolvedConflict::eTwoWayConflictReason conflictReason ) = 0;

		static spITwoWayBasicComparisonResult CreateNodeTypesMismatchTwoWayBasicComparisonResult( INode::eNodeType baseNodeType,
			INode::eNodeType latestNodeType, sizet baseNodeSize, sizet latestNodeSize );

		static spITwoWayBasicComparisonResult CreateSimpleNodeMismatchTwoWayBasicComparisonResult( bool IsBaseURIType, bool IsLatestURIType );
		static spITwoWayBasicComparisonResult CreateArrayNodeMismatchTwoWayBasicComparisonResult( sizet baseArrayNodeCount, sizet latestArrayNodeCount,
			IArrayNode::eArrayForm baseArrayType, IArrayNode::eArrayForm latestArrayType,
			INode::eNodeType baseArrayNodeType, INode::eNodeType latesetArrayNodeType );
		static spITwoWayBasicComparisonResult CreateStructNodeMismatchTwoWayBasicComparisonResult( sizet baseStructNodeCount, sizet latestStructNodeCount );

	protected:
		virtual ~ITwoWayBasicComparisonResult() __NOTHROW__ {}

	#ifdef FRIEND_CLASS_DECLARATION
		FRIEND_CLASS_DECLARATION();
	#endif
		REQ_FRIEND_CLASS_DECLARATION();
	};

#if XMP_WinBuild
	#pragma warning( pop )
#endif

}

#endif  // __ITwoWayBasicComparisonResult_h__
