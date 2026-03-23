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
#include "XMPCompareAndMerge/ImplHeaders/ThreeWayResolvedConflictImpl.h"
#undef IMPLEMENTATION_HEADERS_CAN_BE_INCLUDED

#include "XMPCompareAndMerge/Interfaces/ITwoWayResolvedConflict.h"

#include "XMPCommon/Interfaces/IError_I.h"
#include "XMPCommon/Utilities/TSmartPointers_I.h"

#include "XMPCore/Interfaces/INode.h"
#include "XMPCore/Interfaces/IMetadata.h"

namespace AdobeXMPCompareAndMerge_Int {

	ThreeWayResolvedConflictImpl::ThreeWayResolvedConflictImpl( eConflictResolutionReason resolutionReason, const spcINode & baseVersionNode,
		const spcINode & latestVersionNode, const spcINode & userVersionNode, const spcINode & resolvedNode, eResolvedSource resolvedSource )
		: mConflictResolutionReason( resolutionReason )
		, mResolvedSource( resolvedSource )
		, mBaseVersionNode( baseVersionNode )
		, mLatestVersionNode( latestVersionNode )
		, mUserVersionNode( userVersionNode )
		, mResolvedNode( resolvedNode ) { }

	// All static functions of _I class.

	spcINode APICALL ThreeWayResolvedConflictImpl::GetBaseVersionNode() const {
		return mBaseVersionNode;
	}

	IThreeWayResolvedConflict::eConflictResolutionReason APICALL ThreeWayResolvedConflictImpl::GetConflictResolutionReason() const {
		return mConflictResolutionReason;
	}

	spcINode APICALL ThreeWayResolvedConflictImpl::GetLatestVersionNode() const {
		return mLatestVersionNode;
	}

	spcINode APICALL ThreeWayResolvedConflictImpl::GetResolvedNode() const {
		return mResolvedNode;
	}

	IThreeWayResolvedConflict::eResolvedSource APICALL ThreeWayResolvedConflictImpl::GetResolvedSource() const {
		return mResolvedSource;
	}

	spcINode APICALL ThreeWayResolvedConflictImpl::GetUserVersionNode() const {
		return mUserVersionNode;
	}

	spIThreeWayResolvedConflict IThreeWayResolvedConflict_I::CreateThreeWayResolvedConflict( eConflictResolutionReason resolutionReason, const spcINode & baseVersionNode,
		const spcINode & latestVersionNode, const spcINode & userVersionNode, const spcINode & resolvedNode, eResolvedSource resolvedSource )
	{
		return MakeUncheckedSharedPointer(
			new ThreeWayResolvedConflictImpl( resolutionReason, baseVersionNode, latestVersionNode, userVersionNode, resolvedNode, resolvedSource ), __FILE__, __LINE__, true );
	}

	TAllocator< spIThreeWayResolvedConflict > gTAllocatorThreeWayResolvedConflict;

	spIThreeWayResolvedConflicts IThreeWayResolvedConflict_I::CreateThreeWayResolvedConflicts() {
		return MakeUncheckedSharedPointerWithDefaultDelete( new IThreeWayResolvedConflicts( gTAllocatorThreeWayResolvedConflict ), __FILE__, __LINE__, true );
	}

	spIThreeWayResolvedConflicts IThreeWayResolvedConflict_I::CreateThreeWayResolvedConflicts( const spITwoWayResolvedConflicts & twoWayResolvedConflictList,
		const spcIMetadata & baseMetadata )
	{
		auto list = CreateThreeWayResolvedConflicts();
		for (sizet i = 0, count = twoWayResolvedConflictList->size(); i < count; ++i) {
			const spcITwoWayResolvedConflict & twoWayResolvedConflict = ( *twoWayResolvedConflictList )[ i ];
			spcINode latestVersionNode = twoWayResolvedConflict->GetBaseVersionNode();
			spcINode userVersionNode = twoWayResolvedConflict->GetLatestVersionNode();
			spcIPath pathOfNodeFromRoot = latestVersionNode ? latestVersionNode->GetPath() : userVersionNode->GetPath();
			spcINode baseVersionNode = baseMetadata->GetNodeAtPath( pathOfNodeFromRoot );

			list->push_back( CreateThreeWayResolvedConflict( twoWayResolvedConflict->GetConflictResolutionReason(), baseVersionNode, latestVersionNode, userVersionNode,
				twoWayResolvedConflict->GetResolvedNode(), twoWayResolvedConflict->GetResolvedSource() ) );
		}
		return list;
	}

}

