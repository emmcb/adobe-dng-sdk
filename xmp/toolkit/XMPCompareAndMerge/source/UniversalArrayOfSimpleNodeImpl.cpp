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

#include "XMPCompareAndMerge/ImplHeaders/UniversalArrayOfSimpleNodeImpl.h"
#include "XMPCompareAndMerge/ImplHeaders/AutoMergeUtilities.h"

namespace AdobeXMPCompareAndMerge_Int {

	static bool verifyNode ( spcINode node )
	{
		//This would verify if this is the correct strategy for this node.
		if ( !node )
			return false;
		INode::eNodeType nodeType = node->GetNodeType ( );
		if ( nodeType != INode::kNTArray )
			return false;
		spcIArrayNode arrayNode = node->ConvertToArrayNode ( );
		IArrayNode::eArrayForm arrayForm = arrayNode->GetArrayForm ( );

		if ( arrayForm != IArrayNode::kAFOrdered && arrayForm != IArrayNode::kAFUnordered && arrayForm != IArrayNode::kAFAlternative )
			return false;
		INode::eNodeType childType = arrayNode->GetChildNodeType ( );
		if ( childType != INode::kNTSimple )
			return false;
		return true;
	}

	bool APICALL UniversalArrayOfSimpleNodeImpl::InterestedInResolvingConflict ( const spcIUTF8String & nodePath , INode::eNodeType nodeType , IThreeWayUnResolvedConflict::eConflictReason reason , bool alreadyResolved )
	{
		if ( alreadyResolved )
			return false;

		if ( nodeType != INode::kNTArray )
			return false;
		return true;
	}

	IThreeWayUnResolvedConflict::eConflictReason APICALL UniversalArrayOfSimpleNodeImpl::ResolveConflict ( IThreeWayUnResolvedConflict::eConflictReason conflictReason ,
		const spcINode & baseVersionNode , const spcINode & latestVersionNode , const spcINode & userVersionNode ,
		spcINode & resolvedNode , IThreeWayResolvedConflict::eConflictResolutionReason & conflictResolutionReason , IThreeWayResolvedConflict::eResolvedSource & resolvedSource )
	{
		spcIArrayNode baseArrayNode = baseVersionNode ? baseVersionNode->ConvertToArrayNode ( ) : spcIArrayNode ( );
		spcIArrayNode latestArrayNode = spcIArrayNode();
		spcIArrayNode userArrayNode = spcIArrayNode();
		spIArrayNode mergedNode = spIArrayNode();
		IArrayNode::eArrayForm latestArrayForm = IArrayNode::kAFNone;
		IArrayNode::eArrayForm userArrayForm = IArrayNode::kAFNone;
		IArrayNode::eArrayForm baseArrayForm = IArrayNode::kAFNone;
		bool useBase ( false );

		if ( verifyNode ( baseVersionNode ) && verifyNode ( latestVersionNode ) )
		{
			baseArrayForm = baseVersionNode->ConvertToArrayNode ( )->GetArrayForm ( );
			latestArrayForm = latestVersionNode->ConvertToArrayNode ( )->GetArrayForm ( );
			if ( baseArrayForm == latestArrayForm )
			{
				useBase = true;
			}
		}

		switch ( conflictReason ) {
		case IThreeWayUnResolvedConflict::kCRURIMismatch:
			return conflictReason;
			break;

		case IThreeWayUnResolvedConflict::kCRDeletedInLatestEditedInUser:
			if ( !verifyNode ( userVersionNode ) )
				return conflictReason;
			resolvedNode = latestVersionNode;
			resolvedSource = IThreeWayResolvedConflict::kRSLatestVersion;
			conflictResolutionReason = IThreeWayResolvedConflict::kCRRStrategyResolved;
			return IThreeWayUnResolvedConflict::kCRResolved;
			break;

		case IThreeWayUnResolvedConflict::kCRDeletedInUserEditedInLatest:
			if ( !verifyNode ( latestVersionNode ) )
				return conflictReason;
			resolvedNode = latestVersionNode;
			resolvedSource = IThreeWayResolvedConflict::kRSLatestVersion;
			conflictResolutionReason = IThreeWayResolvedConflict::kCRRStrategyResolved;
			return IThreeWayUnResolvedConflict::kCRResolved;
			break;

		case IThreeWayUnResolvedConflict::kCRTypesMismatch:
			return conflictReason;
			break;

		case IThreeWayUnResolvedConflict::kCRQualifiersMismatch:
			if ( !verifyNode ( userVersionNode ) || !verifyNode ( latestVersionNode ) )
				return conflictReason;
			latestArrayNode = latestVersionNode->ConvertToArrayNode ( );
			userArrayNode = userVersionNode->ConvertToArrayNode ( );
			latestArrayForm = latestArrayNode->GetArrayForm ( );
			userArrayForm = userArrayNode->GetArrayForm ( );
			if ( latestArrayForm != userArrayForm )
				NOTIFY_ERROR ( IError_base::kEDDataModel , kDMECDifferentNodeTypePresent ,
					"Different type of array present in packets " , IError_base::kESOperationFatal , false , false );
			mergedNode = latestVersionNode->Clone ( )->ConvertToArrayNode ( );
			if ( useBase )
				AutoMergeUtilities::resolveQualifier ( baseArrayNode , latestArrayNode , userArrayNode , mergedNode );
			resolvedNode = mergedNode;
			break;

		case IThreeWayUnResolvedConflict::kCRValuesMismatch:
			if ( !verifyNode ( userVersionNode ) || !verifyNode ( latestVersionNode ) )
				return conflictReason;
			latestArrayNode = latestVersionNode->ConvertToArrayNode ( );
			userArrayNode = userVersionNode->ConvertToArrayNode ( );

			latestArrayForm = latestArrayNode->GetArrayForm ( );
			userArrayForm = userArrayNode->GetArrayForm ( );
			if ( latestArrayForm != userArrayForm )
				NOTIFY_ERROR ( IError_base::kEDDataModel , kDMECDifferentNodeTypePresent ,
					"Different type of array present in packets " , IError_base::kESOperationFatal , false , false );

			mergedNode = latestArrayNode->Clone ( )->ConvertToArrayNode ( );
			mergedNode->Clear ( true , false );

			AutoMergeUtilities::makeUnion ( baseArrayNode , latestArrayNode , userArrayNode , mergedNode , useBase );

			if ( useBase )
				AutoMergeUtilities::resolveQualifier (/* useBase,*/ baseArrayNode , latestArrayNode , userArrayNode , mergedNode );

			resolvedNode = mergedNode;
			break;

		default:
			return conflictReason;
			break;
		}

		if ( resolvedNode )
		{
			resolvedSource = IThreeWayResolvedConflict::kRSFieldsMerged;
			conflictResolutionReason = IThreeWayResolvedConflict::kCRRStrategyResolved;
			return IThreeWayUnResolvedConflict::kCRResolved;
		}
		return conflictReason;
	}
}
