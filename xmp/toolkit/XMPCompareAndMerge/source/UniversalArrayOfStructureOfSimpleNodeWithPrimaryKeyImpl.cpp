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

#include "XMPCompareAndMerge/ImplHeaders/UniversalArrayOfStructureOfSimpleNodeWithPrimaryKeyImpl.h"
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

	UniversalArrayOfStructureOfSimpleNodeWithPrimaryKeyImpl::UniversalArrayOfStructureOfSimpleNodeWithPrimaryKeyImpl ( std::map <std::pair<std::string , std::string> , std::pair<std::string , std::string> > _mprimaryKeyMap )
	{
		//making a copy since this strategy is instantiated only once
		this->mprimaryKeyMap = _mprimaryKeyMap;
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

	bool APICALL UniversalArrayOfStructureOfSimpleNodeWithPrimaryKeyImpl::InterestedInResolvingConflict ( const spcIUTF8String & nodePath , INode::eNodeType nodeType , IThreeWayUnResolvedConflict::eConflictReason reason , bool alreadyResolved )
	{
		if ( alreadyResolved )
			return false;

		if ( nodeType != INode::kNTArray )
			return false;
		return true;
	}

	IThreeWayUnResolvedConflict::eConflictReason APICALL UniversalArrayOfStructureOfSimpleNodeWithPrimaryKeyImpl::ResolveConflict ( IThreeWayUnResolvedConflict::eConflictReason conflictReason ,
		const spcINode & baseVersionNode , const spcINode & latestVersionNode , const spcINode & userVersionNode ,
		spcINode & resolvedNode , IThreeWayResolvedConflict::eConflictResolutionReason & conflictResolutionReason , IThreeWayResolvedConflict::eResolvedSource & resolvedSource )
	{

		//find in primary key map, if primary key is not registered then look for other strategy
		auto it = mprimaryKeyMap.find ( std::make_pair ( (latestVersionNode?latestVersionNode:userVersionNode)->GetNameSpace ( )->c_str ( ) , ( latestVersionNode ? latestVersionNode : userVersionNode )->GetName ( )->c_str ( )  ) );
		if ( it == mprimaryKeyMap.end ( ) )
			return conflictReason;

		mprimaryKeyNamespace = it->second.first;
		mprimaryKeyName = it->second.second;

		spcIArrayNode baseArrayNode = baseVersionNode ? baseVersionNode->ConvertToArrayNode ( ) : spcIArrayNode ( );
        spcIArrayNode latestArrayNode = spcIArrayNode();
        spcIArrayNode userArrayNode = spcIArrayNode();
        spIArrayNode mergedNode = spIArrayNode();
		IArrayNode::eArrayForm latestArrayForm = IArrayNode::kAFNone;
		IArrayNode::eArrayForm userArrayForm = IArrayNode::kAFNone;
		IArrayNode::eArrayForm baseArrayForm = IArrayNode::kAFNone;
		bool useBase ( false );
		bool duplicateEntries ( false );

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

		case  IThreeWayUnResolvedConflict::kCRTypesMismatch:
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
		{
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

			std::map <spcIUTF8String , spcIStructureNode , IUTF8StringComparator> userArrayChildMap;
			std::map <spcIUTF8String , spcIStructureNode , IUTF8StringComparator> baseArrayChildMap;
			std::map <spcIUTF8String , spIStructureNode , IUTF8StringComparator> latestArrayChildMap;
			
			sizet userchildCount = userArrayNode->ChildCount ( );
			for ( sizet i = 1; i <= userchildCount; i++ )
			{
				spcIStructureNode child = userArrayNode->GetStructureNodeAtIndex ( i );
				if ( child )
				{
					spcISimpleNode primaryKeyNode = child->GetSimpleNode ( mprimaryKeyNamespace.c_str ( ) , AdobeXMPCommon::npos , mprimaryKeyName.c_str ( ) , AdobeXMPCommon::npos );
					if ( primaryKeyNode )
					{
						if (!userArrayChildMap.insert(std::make_pair(primaryKeyNode->GetValue(), child)).second )
						{
							duplicateEntries = true;
							break;
						}
					}
				}
			}

			if ( duplicateEntries == true )
			{
				//notify_warning
				NOTIFY_WARNING ( IError_v1::kEDConflictResolution , kGECParametersNotAsExpected , "Duplicate Key" , false , false );
				resolvedNode = latestArrayNode->Clone ( );
				resolvedSource = IThreeWayResolvedConflict::kRSFieldsMerged;
				conflictResolutionReason = IThreeWayResolvedConflict::kCRRStrategyResolved;
				return IThreeWayUnResolvedConflict::kCRResolved;
			}

			if ( useBase )
			{
				sizet basechildCount = baseArrayNode->ChildCount ( );
				for ( sizet i = 1; i <= basechildCount; i++ )
				{
					spcIStructureNode child = baseArrayNode->GetStructureNodeAtIndex ( i );
					if ( child )
					{
						spcISimpleNode primaryKeyNode = child->GetSimpleNode ( mprimaryKeyNamespace.c_str ( ) , AdobeXMPCommon::npos , mprimaryKeyName.c_str ( ) , AdobeXMPCommon::npos );
						if ( primaryKeyNode )
							baseArrayChildMap.insert ( std::make_pair ( primaryKeyNode->GetValue ( ) , child ) );
					}
				}
			}

			std::vector<spcIStructureNode> missingPrimaryKeyStructs;

			sizet latestchildCount = latestArrayNode->ChildCount ( );
			for ( sizet i = 1; i <= latestchildCount; i++ )
			{
				spcIStructureNode child = latestArrayNode->GetStructureNodeAtIndex ( i );
				if ( !child )
					continue;
				
				spIStructureNode clonedChild = child->Clone ( )->ConvertToStructureNode ( );
				
				if ( !clonedChild )
					continue;
				
				spcISimpleNode primaryKeyNode = child->GetSimpleNode ( mprimaryKeyNamespace.c_str ( ) , AdobeXMPCommon::npos , mprimaryKeyName.c_str ( ) , AdobeXMPCommon::npos );				
				if ( !primaryKeyNode )
				{
					if (findNode(missingPrimaryKeyStructs, clonedChild))
					{
						duplicateEntries = true;
						break;
					}
					else
					{
						missingPrimaryKeyStructs.push_back(clonedChild);
						mergedNode->AppendNode(clonedChild);
					}
				}
				else
				{
					if ( latestArrayChildMap.find ( primaryKeyNode->GetValue ( ) ) != latestArrayChildMap.end ( ) )
					{
						duplicateEntries = true;
						break;
					}

					if ( !useBase )
					{
						latestArrayChildMap.insert ( std::make_pair ( primaryKeyNode->GetValue ( ) , clonedChild ) );
						mergedNode->AppendNode ( clonedChild );
					}
					else
					{
						if ( userArrayChildMap.find ( primaryKeyNode->GetValue ( ) ) == userArrayChildMap.end ( ) )
						{
							if ( baseArrayChildMap.find ( primaryKeyNode->GetValue ( ) ) == baseArrayChildMap.end ( ) ||
								!AutoMergeUtilities::compareStructs ( baseArrayChildMap.find ( primaryKeyNode->GetValue ( ) )->second , clonedChild ) )
							{
								latestArrayChildMap.insert ( std::make_pair ( primaryKeyNode->GetValue ( ) , clonedChild ) );
								mergedNode->AppendNode ( clonedChild );
							}

							//else :: deleted in  user so no need to insert in map
						}

						else
						{
							//base present
							if ( baseArrayChildMap.find ( primaryKeyNode->GetValue ( ) ) != baseArrayChildMap.end ( ) )
							{
								//compare user and latest
								if ( !AutoMergeUtilities::compareStructs ( userArrayChildMap.find ( primaryKeyNode->GetValue ( ) )->second , clonedChild ) )
								{
									//compare user and base
									if ( AutoMergeUtilities::compareStructs ( userArrayChildMap.find ( primaryKeyNode->GetValue ( ) )->second , baseArrayChildMap.find ( primaryKeyNode->GetValue ( ) )->second ) )
									{
										latestArrayChildMap.insert ( std::make_pair ( primaryKeyNode->GetValue ( ) , clonedChild ) );
										mergedNode->AppendNode ( clonedChild );
									}
									else
									{
										//compare base and latest
										if ( AutoMergeUtilities::compareStructs ( baseArrayChildMap.find ( primaryKeyNode->GetValue ( ) )->second , clonedChild ) )
										{
											//insert user structure
											latestArrayChildMap.insert ( std::make_pair ( primaryKeyNode->GetValue ( ) , userArrayChildMap.find ( primaryKeyNode->GetValue ( ) )->second->Clone ( )->ConvertToStructureNode ( ) ) );
											mergedNode->AppendNode ( userArrayChildMap.find ( primaryKeyNode->GetValue ( ) )->second->Clone ( )->ConvertToStructureNode ( ) );
										}
										else
										{
											//merge structure of all three
											clonedChild->Clear ( true , false );
											AutoMergeUtilities::makeUnion ( baseArrayChildMap.find ( primaryKeyNode->GetValue ( ) )->second , child , userArrayChildMap.find ( primaryKeyNode->GetValue ( ) )->second , clonedChild , true );
											AutoMergeUtilities::resolveQualifier ( baseArrayChildMap.find ( primaryKeyNode->GetValue ( ) )->second , clonedChild , userArrayChildMap.find ( primaryKeyNode->GetValue ( ) )->second , clonedChild );
											latestArrayChildMap.insert ( std::make_pair ( primaryKeyNode->GetValue ( ) , clonedChild ) );
											mergedNode->AppendNode ( clonedChild );
										}
									}
								}
								else
								{
									AutoMergeUtilities:: resolveQualifier ( baseArrayChildMap.find ( primaryKeyNode->GetValue ( ) )->second , clonedChild , userArrayChildMap.find ( primaryKeyNode->GetValue ( ) )->second , clonedChild );
									latestArrayChildMap.insert ( std::make_pair ( primaryKeyNode->GetValue ( ) , clonedChild ) );
									mergedNode->AppendNode ( clonedChild );
								}
							}
							else
							{
								//if in base then compare whether base and latest are same if same then take user
								//otherwise latest 
								//if none are same then take union
								latestArrayChildMap.insert ( std::make_pair ( primaryKeyNode->GetValue ( ) , clonedChild ) );
								mergedNode->AppendNode ( clonedChild );
							}
						}
					}
				}
			}

			if ( duplicateEntries == true )
			{
				//notify_warning
				NOTIFY_WARNING ( IError_v1::kEDConflictResolution , kGECParametersNotAsExpected , "Duplicate Key" , false , false );

				resolvedNode = latestArrayNode->Clone ( );
				resolvedSource = IThreeWayResolvedConflict::kRSFieldsMerged;
				conflictResolutionReason = IThreeWayResolvedConflict::kCRRStrategyResolved;
				return IThreeWayUnResolvedConflict::kCRResolved;
			}

			std::vector<spcIStructureNode> missingPrimaryKeyTemp;

			for ( sizet i = 1; i <= userchildCount; i++ )
			{
				spcIStructureNode child = userArrayNode->GetStructureNodeAtIndex ( i );
				if ( !child )
					continue;
				spIStructureNode clonedChild = child->Clone ( )->ConvertToStructureNode ( );
				if ( !clonedChild )
					continue;
				spcISimpleNode primaryKeyNode = child->GetSimpleNode ( mprimaryKeyNamespace.c_str ( ) , AdobeXMPCommon::npos , mprimaryKeyName.c_str ( ) , AdobeXMPCommon::npos );
				
				if ( !primaryKeyNode ) {
					if ( findNode(missingPrimaryKeyTemp,clonedChild) )
					{
						duplicateEntries = true;
						break;
					}
					else {
						missingPrimaryKeyTemp.push_back(clonedChild);
						if (!findNode(missingPrimaryKeyStructs, clonedChild)) {
							missingPrimaryKeyStructs.push_back(clonedChild);
							mergedNode->AppendNode(clonedChild);
						}
						
					}
				}
				else
				{
					if ( !useBase )
					{
						if ( primaryKeyNode && latestArrayChildMap.find ( primaryKeyNode->GetValue ( ) ) == latestArrayChildMap.end ( ) )
						{
							latestArrayChildMap.insert ( std::make_pair ( primaryKeyNode->GetValue ( ) , clonedChild ) );
							mergedNode->AppendNode ( clonedChild );
						}
					}

					else
					{
						if ( primaryKeyNode && latestArrayChildMap.find ( primaryKeyNode->GetValue ( ) ) == latestArrayChildMap.end ( ) &&
							baseArrayChildMap.find ( primaryKeyNode->GetValue ( ) ) == baseArrayChildMap.end ( ) )
						{
							latestArrayChildMap.insert ( std::make_pair ( primaryKeyNode->GetValue ( ) , clonedChild ) );
							mergedNode->AppendNode ( clonedChild );
						}
						/*else
						{
						//base =user or base!=user, don't take user.

						}*/
					}
				}
			}
			if (duplicateEntries == true)
			{
				//notify_warning
				NOTIFY_WARNING(IError_v1::kEDConflictResolution, kGECParametersNotAsExpected, "Duplicate Key", false, false);

				resolvedNode = latestArrayNode->Clone();
				resolvedSource = IThreeWayResolvedConflict::kRSFieldsMerged;
				conflictResolutionReason = IThreeWayResolvedConflict::kCRRStrategyResolved;
				return IThreeWayUnResolvedConflict::kCRResolved;
			}
			if ( useBase )
				AutoMergeUtilities::resolveQualifier ( baseArrayNode , latestArrayNode , userArrayNode , mergedNode );

			resolvedNode = mergedNode;
		}
		break;

		default:
			return conflictReason;
			break;
		}
		//logicEnd
		if ( resolvedNode )
		{
			resolvedSource = IThreeWayResolvedConflict::kRSFieldsMerged;
			conflictResolutionReason = IThreeWayResolvedConflict::kCRRStrategyResolved;
			return IThreeWayUnResolvedConflict::kCRResolved;
		}
		return conflictReason;
	}
}
