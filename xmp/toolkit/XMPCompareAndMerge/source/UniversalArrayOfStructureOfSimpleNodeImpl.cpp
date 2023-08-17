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

#include "XMPCompareAndMerge/ImplHeaders/UniversalArrayOfStructureOfSimpleNodeImpl.h"
#include "XMPCompareAndMerge/ImplHeaders/AutoMergeUtilities.h"
#include <set>

namespace AdobeXMPCompareAndMerge_Int {

	static spcIStructureNode findNode(std::vector<spcIStructureNode> nodes, spcIStructureNode node) {
		for (std::vector<spcIStructureNode>::iterator it = nodes.begin(); it != nodes.end(); it++) {
			if (AutoMergeUtilities::compareStructs(*it, node, false))
				return *it;
		}
		return spcIStructureNode();
	}

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
		if ( !AutoMergeUtilities::verifyChildType ( arrayNode ) )
			return false;
		return true;

	}

	static spIArrayNode ResolveValueMismatch ( spcIArrayNode baseArrayNode , spcIArrayNode latestArrayNode , spcIArrayNode userArrayNode , bool useBase ) {
		std::vector<spcIStructureNode> baseArray;
		if ( useBase ) {
			sizet basechildCount = baseArrayNode->ChildCount ( );
			for ( sizet i = 1; i <= basechildCount; i++ )
			{
				spcIStructureNode child = baseArrayNode->GetStructureNodeAtIndex ( i );
				if (child && !findNode(baseArray, child)) {
					baseArray.push_back(child);
				}
			}
		}

		AdobeXMPCore::spIArrayNode mergedNode = latestArrayNode->Clone ( )->ConvertToArrayNode ( );
		mergedNode->Clear ( true , false );

		std::vector<spcIStructureNode> userArray;

		sizet userchildCount = userArrayNode->ChildCount ( );
		for ( sizet i = 1; i <= userchildCount; i++ )
		{
			spcIStructureNode child = userArrayNode->GetStructureNodeAtIndex ( i );
			if ( child && !findNode(userArray, child))
				userArray.push_back( child );
		}

		std::vector<spcIStructureNode> latestArray;

		sizet latestchildCount = latestArrayNode->ChildCount ( );
		for ( sizet i = 1; i <= latestchildCount; i++ )
		{
			spcIStructureNode child = latestArrayNode->GetStructureNodeAtIndex ( i );

			if ( !child )
				continue;

			spIStructureNode clonedChild = child->Clone ( )->ConvertToStructureNode ( );
			if ( !clonedChild )
				continue;

			if ( !useBase )
			{
				if (!findNode(latestArray, clonedChild)) {
					latestArray.push_back(clonedChild);
					mergedNode->AppendNode(clonedChild);
				}
			}
			else
			{
				spcIStructureNode userNodeInSet = findNode(userArray, clonedChild);
				spcIStructureNode baseNodeInSet = findNode(baseArray, clonedChild);
				if ( !userNodeInSet)
				{
					if (!baseNodeInSet)
					{
						if (!findNode(latestArray, clonedChild)) {
							latestArray.push_back(clonedChild);
							mergedNode->AppendNode(clonedChild);
						}
					}
				}
				else
				{
					if (findNode(baseArray, clonedChild))
					{
						spcINodeIterator it = baseNodeInSet->Iterator ( );
						while ( it ) {
							spcISimpleNode field = it->GetSimpleNode ( );
							spISimpleNode latestField = clonedChild->GetSimpleNode ( field->GetNameSpace ( )->c_str ( ) , AdobeXMPCommon::npos , field->GetName ( )->c_str ( ) , AdobeXMPCommon::npos );
							spcISimpleNode userField = userNodeInSet->GetSimpleNode ( field->GetNameSpace ( )->c_str ( ) , AdobeXMPCommon::npos , field->GetName ( )->c_str ( ) , AdobeXMPCommon::npos );
							AutoMergeUtilities::resolveQualifier ( field , latestField , userField , latestField );
							it = it->Next ( );
						}
						AutoMergeUtilities::resolveQualifier ( baseNodeInSet , clonedChild , userNodeInSet , clonedChild );
					}
					if (!findNode(latestArray, clonedChild)) {
						latestArray.push_back(clonedChild);
						mergedNode->AppendNode(clonedChild);
					}
				}

			}
		}
		spcINodeIterator itUser = userArrayNode->Iterator ( );
		while ( itUser )
		{
			spcIStructureNode child = itUser->GetStructureNode ( );

			if ( !child )
				continue;
			spIStructureNode clonedChild = child->Clone ( )->ConvertToStructureNode ( );
			if ( !clonedChild )
				continue;
			if ( !useBase )
			{
				if (!findNode(latestArray, clonedChild)) {
					latestArray.push_back(clonedChild);
					mergedNode->AppendNode(clonedChild);
				}
			}
			else
			{
				if (!findNode(latestArray, clonedChild))
				{
					if (!findNode(baseArray, clonedChild))
					{
						latestArray.push_back(clonedChild);
						mergedNode->AppendNode(clonedChild);				
					}
				}
			}
			itUser = itUser->Next ( );
		}

		if ( useBase )
			AutoMergeUtilities::resolveQualifier ( baseArrayNode , latestArrayNode , userArrayNode , mergedNode );
		return mergedNode;
	}

	bool APICALL UniversalArrayOfStructureOfSimpleNodeImpl::InterestedInResolvingConflict ( const spcIUTF8String & nodePath , INode::eNodeType nodeType , IThreeWayUnResolvedConflict::eConflictReason reason , bool alreadyResolved )
	{
		if ( alreadyResolved )
			return false;

		if ( nodeType != INode::kNTArray )
			return false;
		return true;
	}

	IThreeWayUnResolvedConflict::eConflictReason APICALL UniversalArrayOfStructureOfSimpleNodeImpl::ResolveConflict ( IThreeWayUnResolvedConflict::eConflictReason conflictReason ,
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
			resolvedNode = ResolveValueMismatch ( baseArrayNode , latestArrayNode , userArrayNode , useBase );
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
