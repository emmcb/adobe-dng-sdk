// =================================================================================================
// Copyright Adobe
// Copyright 2015 Adobe
// All Rights Reserved
//
// NOTICE:  Adobe permits you to use, modify, and distribute this file in accordance with the terms
// of the Adobe license agreement accompanying it. If you have received this file from a source other 
// than Adobe, then your use, modification, or distribution of it requires the prior written permission
// of Adobe.
// =================================================================================================

#include "XMPCompareAndMerge/ImplHeaders/UniversalBlindStrategyImpl.h"
#include "XMPCompareAndMerge/ImplHeaders/AutoMergeUtilities.h"

namespace AdobeXMPCompareAndMerge_Int {

	bool APICALL UniversalBlindStrategyImpl::InterestedInResolvingConflict(const spcIUTF8String & nodePath, INode::eNodeType nodeType, IThreeWayUnResolvedConflict::eConflictReason reason, bool alreadyResolved)
	{
		if (alreadyResolved)
			return false;
		return true;
	}

	IThreeWayUnResolvedConflict::eConflictReason APICALL UniversalBlindStrategyImpl::ResolveConflict(IThreeWayUnResolvedConflict::eConflictReason conflictReason,
		const spcINode & baseVersionNode, const spcINode & latestVersionNode, const spcINode & userVersionNode,
		spcINode & resolvedNode, IThreeWayResolvedConflict::eConflictResolutionReason & conflictResolutionReason, IThreeWayResolvedConflict::eResolvedSource & resolvedSource)
	{

		if (conflictReason == IThreeWayUnResolvedConflict::kCRTypesMismatch)
			NOTIFY_ERROR(IError_base::kEDDataModel, kDMECDifferentNodeTypePresent,
				"Different type of node present in all three packets ", IError_base::kESOperationFatal, false, false);

		INode::eNodeType latestNodeType = latestVersionNode->GetNodeType();
		INode::eNodeType userNodeType = userVersionNode->GetNodeType();

		if (userNodeType != latestNodeType)
		{
			NOTIFY_ERROR(IError_base::kEDDataModel, kDMECDifferentNodeTypePresent,
				"Different type of node present in all three packets ", IError_base::kESOperationFatal, false, false);
		}

		resolvedNode = latestVersionNode;
		resolvedSource = IThreeWayResolvedConflict::kRSLatestVersion;
		conflictResolutionReason = IThreeWayResolvedConflict::kCRRStrategyResolved;
		return IThreeWayUnResolvedConflict::kCRResolved;
	}
}
