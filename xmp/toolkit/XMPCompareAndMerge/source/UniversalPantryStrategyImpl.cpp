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

#include "XMPCompareAndMerge/ImplHeaders/UniversalPantryStrategyImpl.h"
#include "XMPCompareAndMerge/ImplHeaders/AutoMergeUtilities.h"

namespace AdobeXMPCompareAndMerge_Int {

	static bool createHash ( spcIStructureNode  node , std::string& hash ) {
		spcISimpleNode insId = node->GetSimpleNode ( kXMP_NS_XMP_MM , AdobeXMPCommon::npos , "InstanceID" , AdobeXMPCommon::npos );
		std::string insIdStr = "";

		if ( insId )
			insIdStr = insId->GetValue ( )->c_str ( );

		if ( insIdStr.empty ( ) )
			return false;
		else
		{
			hash = hash.append ( insIdStr );
			return true;
		}
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

		if ( arrayForm != IArrayNode::kAFUnordered )
			return false;

		INode::eNodeType childType = arrayNode->GetChildNodeType ( );

		if ( childType != INode::kNTStructure )
			return false;
		return true;

	}

	bool APICALL UniversalPantryStrategyImpl::InterestedInResolvingConflict ( const spcIUTF8String & nodePath , INode::eNodeType nodeType , IThreeWayUnResolvedConflict::eConflictReason reason , bool alreadyResolved )
	{
		static const char pantryPath[] = kXMP_NS_XMP_MM ":Pantry";
		if ( alreadyResolved )
			return false;

		if ( strncmp ( nodePath->c_str ( ) , pantryPath , nodePath->size ( ) ) == 0 ) {
			return true;
		}

		return false;
	}

	IThreeWayUnResolvedConflict::eConflictReason APICALL UniversalPantryStrategyImpl::ResolveConflict ( IThreeWayUnResolvedConflict::eConflictReason conflictReason ,
		const spcINode & baseVersionNode , const spcINode & latestVersionNode , const spcINode & userVersionNode ,
		spcINode & resolvedNode , IThreeWayResolvedConflict::eConflictResolutionReason & conflictResolutionReason , IThreeWayResolvedConflict::eResolvedSource & resolvedSource )
	{
		spcIArrayNode baseArrayNode = baseVersionNode ? baseVersionNode->ConvertToArrayNode ( ) : spcIArrayNode ( );
		spcIArrayNode latestArrayNode = spcIArrayNode();
        spcIArrayNode userArrayNode = spcIArrayNode();
		spIArrayNode mergedNode = spIArrayNode();
		bool useBase ( false );
		bool duplicateEntries ( false );
		if ( verifyNode ( baseVersionNode ) )
			useBase = true;

		switch ( conflictReason ) {
		case IThreeWayUnResolvedConflict::kCRURIMismatch:
			return conflictReason;
			break;

		case IThreeWayUnResolvedConflict::kCRDeletedInLatestEditedInUser:
			resolvedNode = latestVersionNode;
			resolvedSource = IThreeWayResolvedConflict::kRSLatestVersion;
			conflictResolutionReason = IThreeWayResolvedConflict::kCRRStrategyResolved;
			return IThreeWayUnResolvedConflict::kCRResolved;
			break;

		case IThreeWayUnResolvedConflict::kCRDeletedInUserEditedInLatest:
			if ( !verifyNode ( latestVersionNode ) )
				NOTIFY_ERROR ( IError_base::kEDDataModel , kDMECDifferentNodeTypePresent ,
					"Pantry must be of type Unordered Array of Structure" , IError_base::kESOperationFatal , false , false );
			resolvedNode = latestVersionNode;
			resolvedSource = IThreeWayResolvedConflict::kRSLatestVersion;
			conflictResolutionReason = IThreeWayResolvedConflict::kCRRStrategyResolved;
			return IThreeWayUnResolvedConflict::kCRResolved;
			break;

		case  IThreeWayUnResolvedConflict::kCRTypesMismatch:
			NOTIFY_ERROR ( IError_base::kEDDataModel , kDMECDifferentNodeTypePresent ,
				"Different type of node present in all three packets " , IError_base::kESOperationFatal , false , false );
			break;

		case IThreeWayUnResolvedConflict::kCRQualifiersMismatch:
			if ( !verifyNode ( latestVersionNode ) || !verifyNode ( userVersionNode ) )
				NOTIFY_ERROR ( IError_base::kEDDataModel , kDMECDifferentNodeTypePresent ,
					"Pantry must be of type Unordered Array of Structure" , IError_base::kESOperationFatal , false , false );
			mergedNode = latestVersionNode->Clone ( )->ConvertToArrayNode ( );
			if ( useBase )
				AutoMergeUtilities::resolveQualifier ( baseArrayNode , latestArrayNode , userArrayNode , mergedNode );
			resolvedNode = mergedNode;
			break;

		case IThreeWayUnResolvedConflict::kCRValuesMismatch:
		{
			if ( !verifyNode ( latestVersionNode ) || !verifyNode ( userVersionNode ) )
				NOTIFY_ERROR ( IError_base::kEDDataModel , kDMECDifferentNodeTypePresent ,
					"Pantry must be of type Unordered Array of Structure" , IError_base::kESOperationFatal , false , false );
			latestArrayNode = latestVersionNode->ConvertToArrayNode ( );
			userArrayNode = userVersionNode->ConvertToArrayNode ( );
			std::map <std::string , spcIStructureNode> baseChildMap;
			std::map <std::string , spIStructureNode> latestChildMap;
			std::map <std::string , spcIStructureNode> userChildMap;
			std::vector<spIStructureNode> missingPrimaryKeyStructs;

			if ( useBase ) {
				spcINodeIterator itBase = baseArrayNode->Iterator ( );
				while ( itBase )
				{
					spcIStructureNode child = itBase->GetStructureNode ( );
					if ( child ) {
						std::string hashBase = "";
						if ( createHash ( child , hashBase ) )
							baseChildMap.insert ( std::make_pair ( hashBase , child ) );
					}
					itBase = itBase->Next ( );
				}
			}

			spcINodeIterator itUser = userArrayNode->Iterator ( );
			while ( itUser )
			{
				spcIStructureNode child = itUser->GetStructureNode ( );
				if ( child ) {
					std::string hashUser = "";
					if ( createHash ( child , hashUser ) ) {
						sizet mapSize = userChildMap.size ( );
						userChildMap.insert ( std::make_pair ( hashUser , child ) );
						if ( mapSize == userChildMap.size ( ) )
						{
							duplicateEntries = true;
							break;
						}
					}
					else
						missingPrimaryKeyStructs.push_back ( child->Clone ( )->ConvertToStructureNode ( ) );
				}
				itUser = itUser->Next ( );
			}
			if ( duplicateEntries == true )
			{
				//notify_warning
				NOTIFY_WARNING ( IError_v1::kEDConflictResolution , kGECParametersNotAsExpected , "Duplicate Key in Pantry" , false , false );

				resolvedNode = latestArrayNode->Clone ( );
				resolvedSource = IThreeWayResolvedConflict::kRSFieldsMerged;
				conflictResolutionReason = IThreeWayResolvedConflict::kCRRStrategyResolved;
				return IThreeWayUnResolvedConflict::kCRResolved;
			}

			//in below for loop check for elements in user map , i.e case of deleteion in user
			spcINodeIterator itLatest = latestArrayNode->Iterator ( );
			while ( itLatest )
			{
				spcIStructureNode child = itLatest->GetStructureNode ( );
				if ( !child )
					continue;
				spIStructureNode clonedChild = child->Clone ( )->ConvertToStructureNode ( );
				if ( !clonedChild )
					continue;
				std::string hashLatest = "";
				if ( !createHash ( clonedChild , hashLatest ) )
					missingPrimaryKeyStructs.push_back ( clonedChild );
				else {
					//since in all cases (presence of qualifiers, absence, value diff, name diff: qualifier will be chosen on priority)
					if ( latestChildMap.find ( hashLatest ) != latestChildMap.end ( ) )
					{
						duplicateEntries = true;
						break;
					}

					if ( !useBase )
						latestChildMap.insert ( std::make_pair ( hashLatest , clonedChild ) );
					else
					{
						if ( userChildMap.find ( hashLatest ) == userChildMap.end ( ) )
						{
							if ( baseChildMap.find ( hashLatest ) == baseChildMap.end ( ) )
								latestChildMap.insert ( std::make_pair ( hashLatest , clonedChild ) );
						}
						else
						{
							spcIStructureNode userNodeInMap = userChildMap.find ( hashLatest )->second;
							if ( baseChildMap.find ( hashLatest ) != baseChildMap.end ( ) )
							{
								spcIStructureNode baseNodeInMap = baseChildMap.find ( hashLatest )->second;
								AutoMergeUtilities::resolveQualifier ( baseNodeInMap , clonedChild , userNodeInMap , clonedChild );
							}
							latestChildMap.insert ( std::make_pair ( hashLatest , clonedChild ) );
						}
					}
				}
				itLatest = itLatest->Next ( );
			}


			if ( duplicateEntries == true )
			{
				//notify_warning
				NOTIFY_WARNING ( IError_v1::kEDConflictResolution , kGECParametersNotAsExpected , "Duplicate Key in Pantry" , false , false );

				resolvedNode = latestArrayNode->Clone ( );
				resolvedSource = IThreeWayResolvedConflict::kRSFieldsMerged;
				conflictResolutionReason = IThreeWayResolvedConflict::kCRRStrategyResolved;
				return IThreeWayUnResolvedConflict::kCRResolved;
			}

			for ( std::map <std::string , spcIStructureNode>::iterator userChildMapIt = userChildMap.begin ( ); userChildMapIt != userChildMap.end ( ); userChildMapIt++ )
			{
				if ( !useBase )
				{
					if ( latestChildMap.find ( userChildMapIt->first ) == latestChildMap.end ( ) )
					{
						latestChildMap.insert ( std::make_pair ( userChildMapIt->first , userChildMapIt->second->Clone ( )->ConvertToStructureNode ( ) ) );
					}
				}

				else
				{

					if ( latestChildMap.find ( userChildMapIt->first ) == latestChildMap.end ( ) )
					{
						if ( baseChildMap.find ( userChildMapIt->first ) == baseChildMap.end ( ) )
						{
							latestChildMap.insert ( std::make_pair ( userChildMapIt->first , userChildMapIt->second->Clone ( )->ConvertToStructureNode ( ) ) );
						}
					}
				}
			}

			mergedNode = latestArrayNode->Clone ( )->ConvertToArrayNode ( );
			mergedNode->Clear ( true , false );

			std::map <std::string , spIStructureNode>::iterator itLatestArrayChildMap = latestChildMap.begin ( );

			for ( ; itLatestArrayChildMap != latestChildMap.end ( ); itLatestArrayChildMap++ )
			{
				mergedNode->AppendNode ( itLatestArrayChildMap->second );
			}

			std::vector<spIStructureNode>::iterator itMissingPrimaryKeyStructs = missingPrimaryKeyStructs.begin ( );

			for ( ; itMissingPrimaryKeyStructs != missingPrimaryKeyStructs.end ( ); itMissingPrimaryKeyStructs++ )
			{
				mergedNode->AppendNode ( *itMissingPrimaryKeyStructs );
			}

			if ( useBase )
				AutoMergeUtilities::resolveQualifier (/* useBase,*/ baseArrayNode , latestArrayNode , userArrayNode , mergedNode );

			resolvedNode = mergedNode;
		}
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
