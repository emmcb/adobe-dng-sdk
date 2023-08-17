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
	#include "XMPCompareAndMerge/ImplHeaders/TwoWayResolvedConflictImpl.h"
#undef IMPLEMENTATION_HEADERS_CAN_BE_INCLUDED

#include "XMPCommon/Interfaces/IError_I.h"
#include "XMPCommon/Utilities/TSmartPointers_I.h"

namespace AdobeXMPCompareAndMerge_Int {

	TwoWayResolvedConflictImpl::TwoWayResolvedConflictImpl( IThreeWayResolvedConflict::eConflictResolutionReason reason, const spcINode & baseVersionNode,
		const spcINode & latestVersionNode, const spcINode & resolvedNode, IThreeWayResolvedConflict_base::eResolvedSource resolvedSource,
		const spcITwoWayConflictResolutionStrategy & strategyUsed )
		: mConflictResolutionReason( reason )
		, mBaseVersionNode( baseVersionNode )
		, mLatestVersionNode( latestVersionNode )
		, mResolvedNode( resolvedNode )
		, mResolvedSource( resolvedSource )
		, mStrategyUsed( strategyUsed ) { }

	spcINode APICALL TwoWayResolvedConflictImpl::GetBaseVersionNode() const {
		return mBaseVersionNode;
	}

	IThreeWayResolvedConflict::eConflictResolutionReason APICALL TwoWayResolvedConflictImpl::GetConflictResolutionReason() const {
		return mConflictResolutionReason;
	}

	spcINode APICALL TwoWayResolvedConflictImpl::GetLatestVersionNode() const {
		return mLatestVersionNode;
	}

	spcINode APICALL TwoWayResolvedConflictImpl::GetResolvedNode() const {
		return mResolvedNode;
	}

	IThreeWayResolvedConflict::eResolvedSource APICALL TwoWayResolvedConflictImpl::GetResolvedSource() const {
		return mResolvedSource;
	}

	spcITwoWayConflictResolutionStrategy APICALL TwoWayResolvedConflictImpl::GetStrategyUsed() const {
		return mStrategyUsed;
	}

	void APICALL TwoWayResolvedConflictImpl::ModifyResolvedNode( IThreeWayResolvedConflict::eResolvedSource resolvedSource, spcITwoWayConflictResolutionStrategy & strategyUsed, const spcINode & resolvedNode ) {
		mResolvedNode = resolvedNode;
		mResolvedSource = resolvedSource;
		mStrategyUsed = strategyUsed;
	}

	spITwoWayResolvedConflict ITwoWayResolvedConflict::CreateTwoWayResolvedConflict( IThreeWayResolvedConflict::eConflictResolutionReason resolutionReason,
		const spcINode & baseVersionNode, const spcINode & latestVersionNode, const spcINode & resolvedNode,
		IThreeWayResolvedConflict_base::eResolvedSource resolvedSource, const spcITwoWayConflictResolutionStrategy & strategyUsed )
	{
		return MakeUncheckedSharedPointer( new TwoWayResolvedConflictImpl( resolutionReason, baseVersionNode, latestVersionNode, resolvedNode, resolvedSource, strategyUsed ),
			__FILE__, __LINE__, true );
	}

	TAllocator< spITwoWayResolvedConflict >gTAllocatorTwoWayResolvedConflict;

	spITwoWayResolvedConflicts ITwoWayResolvedConflict::CreateTwoWayResolvedConflicts() {
		return MakeUncheckedSharedPointerWithDefaultDelete( new ITwoWayResolvedConflicts( gTAllocatorTwoWayResolvedConflict ), __FILE__, __LINE__, true );
	}

}

