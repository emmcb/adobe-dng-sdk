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
#if WIN_ENV
#pragma warning( push )
#pragma warning(disable:4503)
#endif
#define IMPLEMENTATION_HEADERS_CAN_BE_INCLUDED 1
	#include "XMPCompareAndMerge/ImplHeaders/TwoWayConflictResolutionEngineImpl.h"
#undef IMPLEMENTATION_HEADERS_CAN_BE_INCLUDED

#include "XMPCompareAndMerge/Interfaces/IThreeWayResolvedConflict.h"
#include "XMPCompareAndMerge/Interfaces/ITwoWayResolvedConflict.h"
#include "XMPCompareAndMerge/Interfaces/ITwoWayUnResolvedConflict.h"
#include "XMPCompareAndMerge/Interfaces/ITwoWayConflictResolutionStrategy.h"

#include "XMPCommon/Interfaces/IError_I.h"
#include "XMPCommon/XMPCommonErrorCodes.h"
#include "XMPCommon/Utilities/TSmartPointers_I.h"

namespace AdobeXMPCompareAndMerge_Int {

	static void MarkEntryAsResolved( spITwoWayUnResolvedConflicts & conflictList, spITwoWayResolvedConflicts & resolvedList, sizet index,
		spcINode resolvedNode, IThreeWayResolvedConflict::eConflictResolutionReason reason, IThreeWayResolvedConflict::eResolvedSource & resolvedSource,
		spcITwoWayConflictResolutionStrategy strategyUsed )
	{
		spITwoWayUnResolvedConflict & currentConflict = ( *conflictList )[ index ];
		spITwoWayResolvedConflict resolvedConflict = ITwoWayResolvedConflict::CreateTwoWayResolvedConflict( reason, currentConflict->GetBaseVersionNode(),
			currentConflict->GetLatestVersionNode(), resolvedNode, resolvedSource, strategyUsed );
		resolvedList->push_back( resolvedConflict );
		conflictList->erase( conflictList->begin() + index );
	}

	TAllocator< spITwoWayConflictResolutionStrategy > gTAllocatorTwoWayConlictResolutionStrategy;

	TwoWayConflictResolutionEngineImpl::TwoWayConflictResolutionEngineImpl()
		: mStrategies( MakeUncheckedSharedPointerWithDefaultDelete( new ITwoWayConflictResolutionStrategies( gTAllocatorTwoWayConlictResolutionStrategy ), __FILE__, __LINE__, true ) ) { }


	bool APICALL TwoWayConflictResolutionEngineImpl::RegisterResolutionStrategy( const spcITwoWayConflictResolutionStrategy & strategy ) {
		mStrategies->push_back( strategy );
		return true;
	}

	AdobeXMPCommon::sizet TwoWayConflictResolutionEngineImpl::ResolveConflicts( spITwoWayUnResolvedConflicts & conflictList, spITwoWayResolvedConflicts & resolvedList ) const {
		sizet nStrategies = mStrategies->size();
		sizet nUnResolvedConflicts = conflictList->size();
		sizet resolvedCount = 0;

		for ( sizet j = 0; j < nUnResolvedConflicts; j++ ) {
			spITwoWayUnResolvedConflict currentConflict = ( *conflictList )[ j ];
			const spcINode & baseNode = currentConflict->GetBaseVersionNode();
			const spcINode & latestNode = currentConflict->GetLatestVersionNode();
			ITwoWayUnResolvedConflict::eTwoWayConflictReason currentConflictReason = currentConflict->GetTwoWayConflictReason();
			bool alreadyResolved = false;
			sizet strategyUsed = 0;
			spcINode resolvedNode;
			IThreeWayResolvedConflict::eResolvedSource resolvedSource = IThreeWayResolvedConflict::kRSMaxValue;
			IThreeWayResolvedConflict::eConflictResolutionReason conflictResolutionReason = IThreeWayResolvedConflict::kCRRUnResolved;
			for ( sizet i = 0; i < nStrategies; i++ ) {
				spcITwoWayConflictResolutionStrategy & currentStrategy = ( *mStrategies )[ i ];
				if ( currentStrategy->InterestedInResolvingConflict( baseNode ? baseNode->GetNameSpace() : latestNode->GetNameSpace(),
					baseNode ? baseNode->GetName() : latestNode->GetName(), baseNode ? baseNode->GetNodeType() : latestNode->GetNodeType(),
					currentConflictReason, alreadyResolved ) )
				{
					IThreeWayUnResolvedConflict::eConflictReason result = currentStrategy->ResolveConflict( currentConflictReason, baseNode,
						latestNode, resolvedNode, conflictResolutionReason, resolvedSource );
					if ( result == IThreeWayUnResolvedConflict::kCRResolved ) {
						alreadyResolved = true;
						strategyUsed = i;
					} else {
						currentConflict->SetConflictReason( result );
					}
				}
			}

			if ( alreadyResolved ) {
				MarkEntryAsResolved( conflictList, resolvedList, j, resolvedNode, conflictResolutionReason, resolvedSource, ( *mStrategies )[ strategyUsed ] );
				resolvedCount++;
				j--;
				nUnResolvedConflicts--;
			}
		}

		return resolvedCount;
	}

	spITwoWayConflictResolutionEngine ITwoWayConflictResolutionEngine::CreateTwoWayConflictResolutionEngine() {
		return MakeUncheckedSharedPointer( new TwoWayConflictResolutionEngineImpl(), __FILE__, __LINE__, true );
	}


}
