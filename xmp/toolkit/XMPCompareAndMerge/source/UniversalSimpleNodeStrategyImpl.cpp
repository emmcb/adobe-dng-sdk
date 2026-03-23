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

#include "XMPCompareAndMerge/ImplHeaders/UniversalSimpleNodeStrategyImpl.h"
#include "XMPCompareAndMerge/ImplHeaders/AutoMergeUtilities.h"

namespace AdobeXMPCompareAndMerge_Int {

	static bool verifyNode ( spcINode node )
	{
		//This would verify if this is the correct strategy for this node.
		if ( !node )
			return false;
		INode::eNodeType nodeType = node->GetNodeType ( );
		if ( nodeType != INode::kNTSimple )
			return false;
		return true;
	}

	bool APICALL UniversalSimpleNodeStrategyImpl::InterestedInResolvingConflict ( const spcIUTF8String & nodePath , INode::eNodeType nodeType , IThreeWayUnResolvedConflict::eConflictReason reason , bool alreadyResolved )
	{
		if ( alreadyResolved )
			return false;
		if ( nodeType != INode::kNTSimple )
			return false;
		return true;
	}

	IThreeWayUnResolvedConflict::eConflictReason APICALL UniversalSimpleNodeStrategyImpl::ResolveConflict ( IThreeWayUnResolvedConflict::eConflictReason conflictReason ,
		const spcINode & baseVersionNode , const spcINode & latestVersionNode , const spcINode & userVersionNode ,
		spcINode & resolvedNode , IThreeWayResolvedConflict::eConflictResolutionReason & conflictResolutionReason , IThreeWayResolvedConflict::eResolvedSource & resolvedSource )
	{
		bool resolved ( false );

		switch ( conflictReason ) {
		case IThreeWayUnResolvedConflict::kCRDeletedInLatestEditedInUser:
			if ( !verifyNode ( userVersionNode ) )
				return conflictReason;
			resolved = true;
			resolvedNode = latestVersionNode;
			resolvedSource = IThreeWayResolvedConflict::kRSLatestVersion;
			conflictResolutionReason = IThreeWayResolvedConflict::kCRRStrategyResolved;
			return IThreeWayUnResolvedConflict::kCRResolved;
			break;

		case IThreeWayUnResolvedConflict::kCRDeletedInUserEditedInLatest:
			if ( !verifyNode ( latestVersionNode ) )
				return conflictReason;
			resolved = true;
			resolvedNode = latestVersionNode;
			resolvedSource = IThreeWayResolvedConflict::kRSLatestVersion;
			conflictResolutionReason = IThreeWayResolvedConflict::kCRRStrategyResolved;
			return IThreeWayUnResolvedConflict::kCRResolved;
			break;

		case IThreeWayUnResolvedConflict::kCRTypesMismatch:
			return conflictReason;
			break;

		case IThreeWayUnResolvedConflict::kCRValuesMismatch:
		{
			if ( !verifyNode ( latestVersionNode ) || !verifyNode ( userVersionNode ) )
				return conflictReason;
			spcISimpleNode baseNode = spcISimpleNode ( );
			if ( verifyNode ( baseVersionNode ) )
				baseNode = baseVersionNode->ConvertToSimpleNode ( );
			spcISimpleNode latestNode = latestVersionNode->ConvertToSimpleNode ( );
			spcISimpleNode userNode = userVersionNode->ConvertToSimpleNode ( );
			spcIUTF8String  latestValue = latestNode->GetValue ( );
			spcIUTF8String  userValue = userNode->GetValue ( );

			//All three are different
			if ( baseNode && baseNode->GetValue ( )->compare ( latestValue ) != 0 && baseNode->GetValue ( )->compare ( userValue ) != 0 && latestValue->compare ( userValue ) != 0 )
			{
				resolvedNode = latestVersionNode;
				resolvedSource = IThreeWayResolvedConflict::kRSLatestVersion;
				conflictResolutionReason = IThreeWayResolvedConflict::kCRRStrategyResolved;
			}

			else
			{
				if ( baseNode && baseNode->GetValue ( )->compare ( latestValue ) == 0 )
				{
					resolvedNode = userVersionNode;
					resolvedSource = IThreeWayResolvedConflict::kRSUserVersion;
					conflictResolutionReason = IThreeWayResolvedConflict::kCRRStrategyResolved;
				}

				else //if ( baseValue == userValue )
				{
					resolvedNode = latestVersionNode;
					resolvedSource = IThreeWayResolvedConflict::kRSLatestVersion;
					conflictResolutionReason = IThreeWayResolvedConflict::kCRRStrategyResolved;
				}
			}
			resolved = true;
		}
		break;

		case IThreeWayUnResolvedConflict::kCRQualifiersMismatch:
		{
			if ( !verifyNode ( latestVersionNode ) || !verifyNode ( userVersionNode ) )
				return conflictReason;
			AdobeXMPCore::spISimpleNode mergedNode = latestVersionNode->Clone ( )->ConvertToSimpleNode ( );
			if ( verifyNode ( baseVersionNode ) )
				AutoMergeUtilities::resolveQualifier ( baseVersionNode , latestVersionNode , userVersionNode , mergedNode );
			resolvedNode = mergedNode;
			resolvedSource = IThreeWayResolvedConflict::kRSFieldsMerged;
			conflictResolutionReason = IThreeWayResolvedConflict::kCRRStrategyResolved;
			resolved = true;
		}
		break;

		case IThreeWayUnResolvedConflict::kCRURIMismatch:
		{
			resolvedNode = latestVersionNode;
			resolvedSource = IThreeWayResolvedConflict::kRSLatestVersion;
			conflictResolutionReason = IThreeWayResolvedConflict::kCRRStrategyResolved;
			resolved = true;

		}
		break;

		default:
			return conflictReason;
			break;
		}

		if ( resolved )
			return IThreeWayUnResolvedConflict::kCRResolved;
		return conflictReason;
	}
}
