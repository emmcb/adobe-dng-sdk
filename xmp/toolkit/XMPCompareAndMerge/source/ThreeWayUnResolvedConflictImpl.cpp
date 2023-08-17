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
	#include "XMPCompareAndMerge/ImplHeaders/ThreeWayUnResolvedConflictImpl.h"
#undef IMPLEMENTATION_HEADERS_CAN_BE_INCLUDED

#include "XMPCompareAndMerge/Interfaces/ITwoWayUnResolvedConflict.h"

#include "XMPCommon/Interfaces/IError_I.h"
#include "XMPCommon/Utilities/TSmartPointers_I.h"

#include "XMPCore/Interfaces/INode.h"
#include "XMPCore/Interfaces/IMetadata.h"

namespace AdobeXMPCompareAndMerge_Int {

	// All virtual functions
	
	// All static functions of _I class.

	ThreeWayUnResolvedConflictImpl::ThreeWayUnResolvedConflictImpl( eConflictReason conflictReason, const spcINode & baseVersionNode,
		const spcINode & latestVersionNode, const spcINode & userVersionNode )
		: mConflictReason( conflictReason )
		, mBaseVersionNode( baseVersionNode )
		, mLatestVersionNode( latestVersionNode )
		, mUserVersionNode( userVersionNode ) { }

	void ThreeWayUnResolvedConflictImpl::setConflictReason( IThreeWayUnResolvedConflict::eConflictReason reason ) {
		mConflictReason = reason;
	}

	spcINode APICALL ThreeWayUnResolvedConflictImpl::GetBaseVersionNode() const {
		return mBaseVersionNode;
	}

	IThreeWayUnResolvedConflict::eConflictReason APICALL ThreeWayUnResolvedConflictImpl::GetConflictReason() const {
		return mConflictReason;
	}

	spcINode APICALL ThreeWayUnResolvedConflictImpl::GetLatestVersionNode() const {
		return mLatestVersionNode;
	}

	spcINode APICALL ThreeWayUnResolvedConflictImpl::GetUserVersionNode() const {
		return mUserVersionNode;
	}

	void APICALL ThreeWayUnResolvedConflictImpl::SetConflictReason( eConflictReason reason ) {
		mConflictReason = reason;
	}

	spIThreeWayUnResolvedConflict IThreeWayUnResolvedConflict_I::CreateThreeWayUnResolvedConflict( eConflictReason conflictReason, const spcINode & baseVersionNode,
		const spcINode & latestVersionNode, const spcINode & userVersionNode )
	{
		return MakeUncheckedSharedPointer( new ThreeWayUnResolvedConflictImpl( conflictReason, baseVersionNode, latestVersionNode, userVersionNode ), __FILE__, __LINE__, true );
	}

	TAllocator< spIThreeWayUnResolvedConflict > gTAllocatorThreeWayUnResolvedConflict;

	spIThreeWayUnResolvedConflicts IThreeWayUnResolvedConflict_I::CreateThreeWayUnResolvedConflicts() {
		return MakeUncheckedSharedPointerWithDefaultDelete( new IThreeWayUnResolvedConflicts( gTAllocatorThreeWayUnResolvedConflict ), __FILE__, __LINE__, true );
	}

	spIThreeWayUnResolvedConflicts IThreeWayUnResolvedConflict_I::CreateThreeWayUnResolvedConflicts( const spITwoWayUnResolvedConflicts & twoWayUnResolvedConflicts, const spcIMetadata & baseMetadata ) {
		spIThreeWayUnResolvedConflicts list = IThreeWayUnResolvedConflict_I::CreateThreeWayUnResolvedConflicts();

		for ( sizet i = 0, twoWayListSize = twoWayUnResolvedConflicts->size(); i < twoWayListSize; ++i ) {
			const spcITwoWayUnResolvedConflict & twoWayUnResolvedConflict = ( *twoWayUnResolvedConflicts )[ i ];
			spcINode latestVersionNode = twoWayUnResolvedConflict->GetBaseVersionNode();
			spcINode userVersionNode = twoWayUnResolvedConflict->GetLatestVersionNode();

			spcIPath pathOfNodeFromRoot = latestVersionNode ? latestVersionNode->GetPath() : userVersionNode->GetPath();
			spcINode commonBaseNode = baseMetadata->GetNodeAtPath( pathOfNodeFromRoot );

			list->push_back( IThreeWayUnResolvedConflict_I::CreateThreeWayUnResolvedConflict( twoWayUnResolvedConflict->GetConflictReason(),
				commonBaseNode, latestVersionNode, userVersionNode ) );
		}

		return list;
	}
}
