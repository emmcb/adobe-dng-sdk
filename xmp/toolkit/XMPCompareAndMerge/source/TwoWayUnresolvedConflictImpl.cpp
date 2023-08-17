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
	#include "XMPCompareAndMerge/ImplHeaders/TwoWayUnResolvedConflictImpl.h"
#undef IMPLEMENTATION_HEADERS_CAN_BE_INCLUDED

#include "XMPCommon/Interfaces/IError_I.h"
#include "XMPCommon/Utilities/TSmartPointers_I.h"

namespace AdobeXMPCompareAndMerge_Int {


	TwoWayUnResolvedConflictImpl::TwoWayUnResolvedConflictImpl( eTwoWayConflictReason conflictReason, const spcINode & baseVersionNode, const spcINode & latestVersionNode )
		: mTwoWayConflictReason( conflictReason )
		, mBaseVersionNode( baseVersionNode )
		, mLatestVersionNode( latestVersionNode )
		, mConflictReason( IThreeWayUnResolvedConflict::kCRMaxValue ) { }

	spcINode APICALL TwoWayUnResolvedConflictImpl::GetBaseVersionNode() const {
		return mBaseVersionNode;
	}

	IThreeWayUnResolvedConflict::eConflictReason APICALL TwoWayUnResolvedConflictImpl::GetConflictReason() const {
		return mConflictReason;
	}

	spcINode APICALL TwoWayUnResolvedConflictImpl::GetLatestVersionNode() const {
		return mLatestVersionNode;
	}

	ITwoWayUnResolvedConflict::eTwoWayConflictReason APICALL TwoWayUnResolvedConflictImpl::GetTwoWayConflictReason() const {
		return mTwoWayConflictReason;
	}

	void APICALL TwoWayUnResolvedConflictImpl::SetConflictReason( IThreeWayUnResolvedConflict::eConflictReason reason ) {
		mConflictReason = reason;
	}

	spITwoWayUnResolvedConflict ITwoWayUnResolvedConflict::CreateTwoWayUnResolvedConflict( eTwoWayConflictReason conflictReason,
		const spcINode & baseVersionNode, const spcINode & latestVersionNode )
	{
		return MakeUncheckedSharedPointer( new TwoWayUnResolvedConflictImpl( conflictReason, baseVersionNode, latestVersionNode ), __FILE__, __LINE__, true );
	}

	TAllocator< spITwoWayUnResolvedConflict > gTAllocatorTwoWayUnResolvedConflict;

	spITwoWayUnResolvedConflicts ITwoWayUnResolvedConflict::CreateTwoWayUnResolvedConflicts() {
		return MakeUncheckedSharedPointerWithDefaultDelete( new ITwoWayUnResolvedConflicts( gTAllocatorTwoWayUnResolvedConflict ), __FILE__, __LINE__, true );
	}

}
