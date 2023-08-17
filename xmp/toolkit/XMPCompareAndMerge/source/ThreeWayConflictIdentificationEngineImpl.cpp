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
	#include "XMPCompareAndMerge/ImplHeaders/ThreeWayConflictIdentificationEngineImpl.h"
	#include "XMPCompareAndMerge/ImplHeaders/TwoWayConflictResolutionStrategyForThreeWayConflictIdentification.h"
#undef IMPLEMENTATION_HEADERS_CAN_BE_INCLUDED

#include "XMPCommon/Interfaces/IError_I.h"
#include "XMPCompareAndMerge/XMPCompareAndMergeErrorCodes.h"
#include "XMPCompareAndMerge/Interfaces/ITwoWayConflictIdentificationEngine.h"
#include "XMPCompareAndMerge/Interfaces/ITwoWayConflictResolutionEngine.h"
#include "XMPCompareAndMerge/Interfaces/ITwoWayResolvedConflict.h"
#include "XMPCompareAndMerge/Interfaces/IThreeWayUnResolvedConflict_I.h"
#include "XMPCompareAndMerge/Interfaces/IThreeWayResolvedConflict_I.h"

#include "XMPCommon/Utilities/TSmartPointers_I.h"

namespace AdobeXMPCompareAndMerge_Int {


	spIThreeWayConflictIdentificationEngine IThreeWayConflictIdentificationEngine::CreateThreeWayConflictIdentificationEngine() {
		return MakeUncheckedSharedPointer( new ThreeWayConflictIdentificationEngineImpl(), __FILE__, __LINE__, true );
	}

	ThreeWayUnResolvedResolvedConflicts ThreeWayConflictIdentificationEngineImpl::IdentifyConflicts(
		const spcIMetadata & baseVersionMetadata, const spcIMetadata & latestVersionMetadata, const spcIMetadata & userVersionMetadata ) const
	{
		// Get the two way conflict identification engine running
		spITwoWayConflictIdentificationEngine twoWayConflictIdentificationEngine = ITwoWayConflictIdentificationEngine::CreateTwoWayConflictIdentificationEngine();

		//Get the list of conflicting changes between latestVersion and userVersion
		auto listOfConflictingChangesBetweenLatestAndUserVersions
			= twoWayConflictIdentificationEngine->IdentifyConflicts( latestVersionMetadata, userVersionMetadata );

		if ( listOfConflictingChangesBetweenLatestAndUserVersions->size() == 0 ) { // no conflicting changes
			return std::make_pair( IThreeWayUnResolvedConflict_I::CreateThreeWayUnResolvedConflicts(), IThreeWayResolvedConflict_I::CreateThreeWayResolvedConflicts() );
		} else {
			// there are some conflicting changes so run through TwoWayConflictResolutionEngine
			// Strategy applied TwoWayConflictResolutionStrategyForThreeWayConflictIndentification
			spcITwoWayConflictResolutionStrategy strategy
				= TwoWayConflictResolutionStrategyForThreeWayConflictIdentification::CreateStrategy( baseVersionMetadata );
			spITwoWayConflictResolutionEngine twoWayConflictResolutionEngine = ITwoWayConflictResolutionEngine::CreateTwoWayConflictResolutionEngine();
			auto listOfResolvedChangesBetweenLatestAndUserVersions = ITwoWayResolvedConflict::CreateTwoWayResolvedConflicts();

			//set up resolution engine
			twoWayConflictResolutionEngine->RegisterResolutionStrategy( strategy );
			twoWayConflictResolutionEngine->ResolveConflicts( listOfConflictingChangesBetweenLatestAndUserVersions,
				listOfResolvedChangesBetweenLatestAndUserVersions );

			// create three way lists from two lists
			auto resolvedlist = IThreeWayResolvedConflict_I::CreateThreeWayResolvedConflicts( listOfResolvedChangesBetweenLatestAndUserVersions, baseVersionMetadata );
			auto unResolvedList = IThreeWayUnResolvedConflict_I::CreateThreeWayUnResolvedConflicts(	listOfConflictingChangesBetweenLatestAndUserVersions, baseVersionMetadata );

			return std::make_pair( unResolvedList, resolvedlist );
		}
	}

}
