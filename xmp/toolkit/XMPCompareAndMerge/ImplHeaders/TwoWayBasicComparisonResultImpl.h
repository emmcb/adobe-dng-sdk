#ifndef __TwoWayBasicComparisonResultImpl_h__
#define __TwoWayBasicComparisonResultImpl_h__ 1

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

#if !(IMPLEMENTATION_HEADERS_CAN_BE_INCLUDED)
	#error "Not adhering to design constraints"
	// this file should only be included from its own cpp file
#endif

#include "XMPCompareAndMerge/Interfaces/ITwoWayBasicComparisonResult.h"
#include "XMPCommon/BaseClasses/MemoryManagedObject.h"
#include "XMPCommon/ImplHeaders/SharedObjectImpl.h"

#if XMP_WinBuild
	#pragma warning( push )
	#pragma warning( disable : 4250 )
#endif

namespace AdobeXMPCompareAndMerge_Int {

	class TwoWayBasicComparisonResultImpl
		: public virtual ITwoWayBasicComparisonResult
		, public virtual SharedObjectImpl
		, public virtual MemoryManagedObject
	{
	public:
		TwoWayBasicComparisonResultImpl( ITwoWayUnResolvedConflict::eTwoWayConflictReason reason, sizet baseSize, sizet latestSize,
			INode::eNodeType baseNodeType, INode::eNodeType latestNodeType, IArrayNode::eArrayForm baseArrayType, IArrayNode::eArrayForm latestArrayType,
			INode::eNodeType baseArrayNodeType, INode::eNodeType latestArrayNodeType );


	protected:
		virtual ~TwoWayBasicComparisonResultImpl() __NOTHROW__ {}

		virtual void APICALL AddConflictType( ITwoWayUnResolvedConflict::eTwoWayConflictReason conflictReason );

		virtual INode::eNodeType APICALL GetBaseArrayElementType() const;

		virtual IArrayNode::eArrayForm APICALL GetBaseArrayType() const;

		virtual sizet APICALL GetBaseChildCount() const;

		virtual INode::eNodeType APICALL GetBaseNodeType() const;

		virtual ITwoWayUnResolvedConflict::eTwoWayConflictReason APICALL GetConflictReason() const;

		virtual INode::eNodeType APICALL GetLatestArrayElementType() const;

		virtual IArrayNode::eArrayForm APICALL GetLatestArrayType() const;

		virtual sizet APICALL GetLatestChildCount() const;

		virtual INode::eNodeType APICALL GetLatestNodeType() const;

		virtual bool APICALL IsEqual( ITwoWayUnResolvedConflict::eTwoWayConflictReason mask = ITwoWayUnResolvedConflict::kTWCRNone ) const;

		virtual bool APICALL IsFollowingConflictPresent( ITwoWayUnResolvedConflict::eTwoWayConflictReason conflictReason ) const;

		ITwoWayUnResolvedConflict::eTwoWayConflictReason	mConflictReason;
		sizet												mBaseSize;
		sizet												mLatestSize;
		IArrayNode::eArrayForm								mBaseArrayType;
		IArrayNode::eArrayForm								mLatestArrayType;
		INode::eNodeType									mBaseArrayNodeType;
		INode::eNodeType									mLatestArrayNodeType;
		INode::eNodeType									mBaseNodeType;
		INode::eNodeType									mLatestNodeType;


	#ifdef FRIEND_CLASS_DECLARATION
		FRIEND_CLASS_DECLARATION();
	#endif
		REQ_FRIEND_CLASS_DECLARATION();
	};
}

#endif  // __TwoWayBasicComparisonResultImpl_h__
