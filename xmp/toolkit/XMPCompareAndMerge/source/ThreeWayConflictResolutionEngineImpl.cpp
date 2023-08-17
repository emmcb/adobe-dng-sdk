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
	#include "XMPCompareAndMerge/ImplHeaders/ThreeWayConflictResolutionEngineImpl.h"
#undef IMPLEMENTATION_HEADERS_CAN_BE_INCLUDED

#include "XMPCommon/Interfaces/IError_I.h"
#include "XMPCompareAndMerge/XMPCompareAndMergeErrorCodes.h"
#include "XMPCompareAndMerge/Interfaces/IThreeWayResolvedConflict_I.h"
#include "XMPCompareAndMerge/Interfaces/IThreeWayUnResolvedConflict_I.h"
#include "XMPCompareAndMerge/Interfaces/IThreeWayStrategy_I.h"

#include "XMPCommon/Utilities/TSmartPointers_I.h"

#include "XMPCore/Interfaces/IPath.h"
#include "XMPCore/Interfaces/INameSpacePrefixMap.h"

namespace AdobeXMPCompareAndMerge_Int {


	spIThreeWayConflictResolutionEngine IThreeWayConflictResolutionEngine::CreateThreeWayConflictResolutionEngine() {
		return MakeUncheckedSharedPointer( new ThreeWayConflictResolutionEngineImpl(), __FILE__, __LINE__, true );
	}

	TAllocator< spIThreeWayStrategy > gTAllocatorThreeWayStrategies;

	ThreeWayConflictResolutionEngineImpl::ThreeWayConflictResolutionEngineImpl()
		: mStrategies( MakeUncheckedSharedPointerWithDefaultDelete( new IThreeWayStrategies( gTAllocatorThreeWayStrategies ), __FILE__, __LINE__, true ) ){}

	bool ThreeWayConflictResolutionEngineImpl::RegisterResolutionStrategy( const spIThreeWayStrategy & strategy ) {
		mStrategies->push_back( strategy );
		return true;
	}

	static void MarkEntryAsResolved( const spIThreeWayUnResolvedConflicts & conflictList, const spIThreeWayResolvedConflicts & resolvedList, sizet index,
		const spcINode & resolvedNode, IThreeWayResolvedConflict::eConflictResolutionReason reason, IThreeWayResolvedConflict::eResolvedSource & resolvedSource,
		const spIThreeWayStrategy & strategyUsed )
	{
		spIThreeWayUnResolvedConflict & currentConflict = ( *conflictList )[ index ];
		spIThreeWayResolvedConflict resolvedConflict = IThreeWayResolvedConflict_I::CreateThreeWayResolvedConflict( reason,
			currentConflict->GetBaseVersionNode(), currentConflict->GetLatestVersionNode(), currentConflict->GetUserVersionNode(), resolvedNode, resolvedSource /*, strategyUsed */ );
		resolvedList->push_back( resolvedConflict );
		conflictList->erase( conflictList->begin() + index );
	}

	sizet ThreeWayConflictResolutionEngineImpl::ResolveConflicts( const spIThreeWayUnResolvedConflicts & conflictList, const spIThreeWayResolvedConflicts & resolvedList ) const {
		sizet nStrategies = mStrategies->size();
		sizet nUnResolvedConflicts = conflictList->size();
		sizet resolvedCount = 0;

		for ( sizet j = 0; j < nUnResolvedConflicts; j++ ) {
			spIThreeWayUnResolvedConflict currentConflict = ( *conflictList )[ j ];
			bool alreadyResolved = false;
			sizet strategyUsed = 0;
			spcINode resolvedNode;
			IThreeWayResolvedConflict::eResolvedSource resolvedNodeSource = IThreeWayResolvedConflict::kRSMaxValue;
			IThreeWayResolvedConflict::eConflictResolutionReason resolvedConflictReason = IThreeWayResolvedConflict::kCRRUnResolved;

			for ( sizet i = 0; i < nStrategies; i++ ) {
				spIThreeWayStrategy & currentStrategy = ( *mStrategies )[ i ];
				const spcINode & commonBaseNode = currentConflict->GetBaseVersionNode();
				const spcINode & latestVersionNode = currentConflict->GetLatestVersionNode();
				const spcINode & userVersionNode = currentConflict->GetUserVersionNode();
				IThreeWayUnResolvedConflict::eConflictReason unResolvedConflictReason = currentConflict->GetConflictReason();

				if ( currentStrategy->InterestedInResolvingConflict(
					latestVersionNode ? latestVersionNode->GetPath()->Serialize(/*INameSpacePrefixMap::GetDefaultNameSpacePrefixMap()*/) : userVersionNode->GetPath()->Serialize(),
					latestVersionNode ? latestVersionNode->GetNodeType() : userVersionNode->GetNodeType(),
					unResolvedConflictReason, alreadyResolved ) )
				{
					IThreeWayUnResolvedConflict::eConflictReason result = currentStrategy->ResolveConflict(
						unResolvedConflictReason, commonBaseNode, latestVersionNode, userVersionNode,
						resolvedNode, resolvedConflictReason, resolvedNodeSource );
					if ( result == IThreeWayUnResolvedConflict::kCRResolved ) {
						alreadyResolved = true;
						strategyUsed = i;
					} else {
						currentConflict->GetIThreeWayUnResolvedConflict_I()->SetConflictReason( result );
					}
				}
			}

			if ( alreadyResolved ) {
				MarkEntryAsResolved( conflictList, resolvedList, j, resolvedNode, resolvedConflictReason, resolvedNodeSource, ( *mStrategies )[ strategyUsed ] );
				resolvedCount++;
				j--;
				nUnResolvedConflicts--;
			}
		}

		return resolvedCount;
	}
}
