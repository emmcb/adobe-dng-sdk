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

#define IMPLEMENTATION_HEADERS_CAN_BE_INCLUDED 1
	#include "XMPCompareAndMerge/ImplHeaders/TwoWayBasicComparisonResultImpl.h"
#undef IMPLEMENTATION_HEADERS_CAN_BE_INCLUDED

#include "XMPCommon/Interfaces/IError_I.h"
#include "XMPCommon/Utilities/TSmartPointers_I.h"

namespace AdobeXMPCompareAndMerge_Int {


	TwoWayBasicComparisonResultImpl::TwoWayBasicComparisonResultImpl( ITwoWayUnResolvedConflict::eTwoWayConflictReason reason, sizet baseSize, sizet latestSize,
		INode::eNodeType baseNodeType, INode::eNodeType latestNodeType, IArrayNode::eArrayForm baseArrayType, IArrayNode::eArrayForm latestArrayType,
		INode::eNodeType baseArrayNodeType, INode::eNodeType latestArrayNodeType )
		: mConflictReason( reason )
		, mBaseSize( baseSize )
		, mLatestSize( latestSize )
		, mBaseNodeType( baseNodeType )
		, mLatestNodeType( latestNodeType )
		, mBaseArrayType( baseArrayType )
		, mLatestArrayType( latestArrayType )
		, mBaseArrayNodeType( baseArrayNodeType )
		, mLatestArrayNodeType( latestArrayNodeType ) { }

	void APICALL TwoWayBasicComparisonResultImpl::AddConflictType( ITwoWayUnResolvedConflict::eTwoWayConflictReason conflictReason ) {
		uint32 temp = conflictReason;
		temp |= mConflictReason;
		mConflictReason = static_cast< ITwoWayUnResolvedConflict::eTwoWayConflictReason >( temp );
	}

	INode::eNodeType APICALL TwoWayBasicComparisonResultImpl::GetBaseArrayElementType() const {
		return mBaseArrayNodeType;
	}

	IArrayNode::eArrayForm APICALL TwoWayBasicComparisonResultImpl::GetBaseArrayType() const {
		return mBaseArrayType;
	}

	sizet APICALL TwoWayBasicComparisonResultImpl::GetBaseChildCount() const {
		return mBaseSize;
	}

	INode::eNodeType APICALL TwoWayBasicComparisonResultImpl::GetBaseNodeType() const {
		return mBaseNodeType;
	}

	ITwoWayUnResolvedConflict::eTwoWayConflictReason APICALL TwoWayBasicComparisonResultImpl::GetConflictReason() const {
		return mConflictReason;
	}

	INode::eNodeType APICALL TwoWayBasicComparisonResultImpl::GetLatestArrayElementType() const {
		return mLatestArrayNodeType;
	}

	IArrayNode::eArrayForm APICALL TwoWayBasicComparisonResultImpl::GetLatestArrayType() const {
		return mLatestArrayType;
	}

	sizet APICALL TwoWayBasicComparisonResultImpl::GetLatestChildCount() const {
		return mLatestSize;
	}

	INode::eNodeType APICALL TwoWayBasicComparisonResultImpl::GetLatestNodeType() const {
		return mLatestNodeType;
	}

	bool APICALL TwoWayBasicComparisonResultImpl::IsEqual( ITwoWayUnResolvedConflict::eTwoWayConflictReason mask ) const {
		uint32 result = ~mask;
		result &= mConflictReason;
		if ( result == 0 )
			return true;
		return false;
	}

	bool APICALL TwoWayBasicComparisonResultImpl::IsFollowingConflictPresent( ITwoWayUnResolvedConflict::eTwoWayConflictReason conflictReason ) const {
		uint32 result = mConflictReason & conflictReason;
		if ( result != 0 )
			return true;
		return false;
	}

	spITwoWayBasicComparisonResult ITwoWayBasicComparisonResult::CreateNodeTypesMismatchTwoWayBasicComparisonResult( INode::eNodeType baseNodeType,
		INode::eNodeType latestNodeType, sizet baseNodeSize, sizet latestNodeSize )
	{
		ITwoWayUnResolvedConflict::eTwoWayConflictReason reason( ITwoWayUnResolvedConflict::kTWCRNone );

		if ( baseNodeType != latestNodeType ) {
			reason = ITwoWayUnResolvedConflict::kTWCRTypesMismatch;
		}
		return MakeUncheckedSharedPointer( new TwoWayBasicComparisonResultImpl( reason, baseNodeSize, latestNodeSize, baseNodeType, latestNodeType,
			IArrayNode::kAFNone, IArrayNode::kAFNone, INode::kNTNone, INode::kNTNone ), __FILE__, __LINE__, true );
	}

	spITwoWayBasicComparisonResult ITwoWayBasicComparisonResult::CreateSimpleNodeMismatchTwoWayBasicComparisonResult( bool IsBaseURIType, bool IsLatestURIType ) {
		ITwoWayUnResolvedConflict::eTwoWayConflictReason reason( ITwoWayUnResolvedConflict::kTWCRNone );

		if ( IsBaseURIType != IsLatestURIType ) {
			reason = IsBaseURIType ? ITwoWayUnResolvedConflict::kTWCRSimpleNodeIsURIOnlyInBase : ITwoWayUnResolvedConflict::kTWCRSimpleNodeIsURIOnlyInLatest;
		}
		return MakeUncheckedSharedPointer( new TwoWayBasicComparisonResultImpl( reason, 1, 1, INode::kNTSimple, INode::kNTSimple,
			IArrayNode::kAFNone, IArrayNode::kAFNone, INode::kNTNone, INode::kNTNone ), __FILE__, __LINE__, true );
	}

	spITwoWayBasicComparisonResult ITwoWayBasicComparisonResult::CreateArrayNodeMismatchTwoWayBasicComparisonResult( sizet baseArrayNodeCount, sizet latestArrayNodeCount,
		IArrayNode::eArrayForm baseArrayType, IArrayNode::eArrayForm latestArrayType, INode::eNodeType baseArrayNodeType, INode::eNodeType latesetArrayNodeType )
	{
		uint32 reasons = ITwoWayUnResolvedConflict::kTWCRNone;

		if ( baseArrayType != latestArrayType ) {
			reasons |= ITwoWayUnResolvedConflict::kTWCRArrayTypesMismatch;;
		}

		if ( baseArrayNodeType != latesetArrayNodeType ) {
			reasons |= ITwoWayUnResolvedConflict::kTWCRArrayElementTypesMismatch;
		}

		if ( baseArrayNodeCount != latestArrayNodeCount ) {
			reasons |= ITwoWayUnResolvedConflict::kTWCRArraySizeMismatch;
		}

		return MakeUncheckedSharedPointer(
			new TwoWayBasicComparisonResultImpl( static_cast< ITwoWayUnResolvedConflict::eTwoWayConflictReason >( reasons ), baseArrayNodeCount, latestArrayNodeCount,
				INode::kNTArray, INode::kNTArray, baseArrayType, latestArrayType, baseArrayNodeType, latesetArrayNodeType ),
			__FILE__, __LINE__, true );
	}

	spITwoWayBasicComparisonResult ITwoWayBasicComparisonResult::CreateStructNodeMismatchTwoWayBasicComparisonResult( sizet baseStructNodeCount, sizet latestStructNodeCount ) {
		ITwoWayUnResolvedConflict::eTwoWayConflictReason reason( ITwoWayUnResolvedConflict::kTWCRNone );

		if ( baseStructNodeCount != latestStructNodeCount ) {
			reason = ITwoWayUnResolvedConflict::kTWCRStructureSizeMismatch;
		}

		return MakeUncheckedSharedPointer( new TwoWayBasicComparisonResultImpl( reason, 1, 1, INode::kNTStructure, INode::kNTStructure,
			IArrayNode::kAFNone, IArrayNode::kAFNone, INode::kNTNone, INode::kNTNone ), __FILE__, __LINE__, true );
	}
}
