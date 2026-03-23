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

#include "XMPCompareAndMerge/ImplHeaders/UniversalIngredientsStrategyImpl.h"
#include "XMPCompareAndMerge/ImplHeaders/AutoMergeUtilities.h"
#include <set>

namespace AdobeXMPCompareAndMerge_Int {

	static spcIStructureNode findNode ( std::vector<spcIStructureNode> nodes, spcIStructureNode node ) {
		for ( std::vector<spcIStructureNode>::iterator it = nodes.begin (); it != nodes.end (); it++ ) {
			if ( AutoMergeUtilities::compareStructOfSimpleAndArr ( *it, node, false, false ) )
				return *it;
		}
		return spcIStructureNode ();
	}

	static bool verifyChildTypeOfIngredients ( const spcIArrayNode arrayNode )
	{
		INode::eNodeType childType = arrayNode->GetChildNodeType ( );

		if ( childType == INode::kNTStructure ) {
			sizet arrayChildCount = arrayNode->ChildCount ( );
			for ( XMP_Index i = 1; i <= (XMP_Index)arrayChildCount; i++ )
			{
				int cnt = 0;
				spcIStructureNode childStruct = arrayNode->GetStructureNodeAtIndex ( i );
				spcINodeIterator it = childStruct->Iterator ( );
				while ( it )
				{
					if ( it->GetNodeType ( ) != INode::kNTSimple )
					{
						if ( cnt == 0 && it->GetNodeType ( ) == INode::kNTArray && it->GetArrayNode ( )->GetArrayForm ( ) == IArrayNode::kAFOrdered &&
							it->GetArrayNode ( )->GetName ( )->compare ( "alternatePaths" ) == 0 &&
							it->GetArrayNode ( )->GetNameSpace ( )->compare ( kXMP_NS_XMP_ResourceRef ) == 0 &&
							it->GetArrayNode ( )->GetChildNodeType ( ) == INode::kNTSimple )
							cnt = 1;
						else
							return false;
					}
					it = it->Next ( );
				}
			}
		}
		else
			return false;
		return true;
	}

	static std::pair<UniversalIngredientsStrategyImpl::eIngPrimaryKey , std::string> hashFunc ( const spcIStructureNode ingredient )
	{
		spcISimpleNode docID = ingredient->GetSimpleNode ( kXMP_NS_XMP_ResourceRef , AdobeXMPCommon::npos , "documentID" , AdobeXMPCommon::npos );
		spcISimpleNode insID = ingredient->GetSimpleNode ( kXMP_NS_XMP_ResourceRef , AdobeXMPCommon::npos , "instanceID" , AdobeXMPCommon::npos );
		spcISimpleNode fromPart = ingredient->GetSimpleNode ( kXMP_NS_XMP_ResourceRef , AdobeXMPCommon::npos , "fromPart" , AdobeXMPCommon::npos );
		spcISimpleNode toPart = ingredient->GetSimpleNode ( kXMP_NS_XMP_ResourceRef , AdobeXMPCommon::npos , "toPart" , AdobeXMPCommon::npos );

		std::string docStr = "" , insStr = "";
		std::string toPartStr = "" , fromPartStr = "" , hash = "";

		if ( docID )
			docStr = docID->GetValue ( )->c_str ( );
		if ( insID )
			insStr = insID->GetValue ( )->c_str ( );
		if ( toPart )
			toPartStr = toPart->GetValue ( )->c_str ( );
		if ( fromPart )
			fromPartStr = fromPart->GetValue ( )->c_str ( );

		if ( docStr.empty ( ) && insStr.empty ( ) && toPartStr.empty ( ) && fromPartStr.empty ( ) )
			return std::make_pair ( UniversalIngredientsStrategyImpl::kIPKNone , "Empty" );
		else
		{
			hash.append ( docStr ).append ( insStr ).append ( fromPartStr ).append ( toPartStr );
			if ( !docStr.empty ( ) && !insStr.empty ( ) && !toPartStr.empty ( ) && !fromPartStr.empty ( ) )
				return std::make_pair ( UniversalIngredientsStrategyImpl::kIPKAll , hash );

			return std::make_pair ( UniversalIngredientsStrategyImpl::kIPKSome , hash );
		}
	}

	static spIStructureNode MakeUnionIngredients ( const spIStructureNode baseNode , const spIStructureNode latestNode , const spIStructureNode userNode )
	{
		spINode altNodeBase = baseNode->RemoveNode ( kXMP_NS_XMP_ResourceRef , AdobeXMPCommon::npos , "alternatePaths" , AdobeXMPCommon::npos );
		spINode altNodeLatest = latestNode->RemoveNode ( kXMP_NS_XMP_ResourceRef , AdobeXMPCommon::npos , "alternatePaths" , AdobeXMPCommon::npos );
		spINode altNodeUser = userNode->RemoveNode ( kXMP_NS_XMP_ResourceRef , AdobeXMPCommon::npos , "alternatePaths" , AdobeXMPCommon::npos );
		spIArrayNode altBase = spIArrayNode() , altLatest = spIArrayNode() , altUser = spIArrayNode();

		if ( altNodeBase )
			altBase = altNodeBase->ConvertToArrayNode ( );
		if ( altNodeUser )
			altUser = altNodeUser->ConvertToArrayNode ( );
		if ( altNodeLatest )
			altLatest = altNodeLatest->ConvertToArrayNode ( );

		spIStructureNode ing = latestNode->Clone ( )->ConvertToStructureNode ( );
		ing->Clear ( true , false );

		AutoMergeUtilities::makeUnion ( baseNode , latestNode , userNode , ing , true );
		spIArrayNode finalAlt = spIArrayNode();
		if ( !altBase )
			finalAlt = altLatest;
		else
		{
			if ( !altUser && altLatest )
			{
				if ( ! AutoMergeUtilities::compareOrderedArrayOfSimple ( altBase , altLatest ) )
					finalAlt = altLatest;
			}
			else
			{
				if ( altUser && altLatest )
                {
                    finalAlt = IArrayNode::CreateOrderedArrayNode ( kXMP_NS_XMP_ResourceRef , AdobeXMPCommon::npos , "alternatePaths" , AdobeXMPCommon::npos );
                    AutoMergeUtilities::makeUnion ( altBase , altLatest , altUser , finalAlt , true );
                    AutoMergeUtilities::resolveQualifier ( altBase , altLatest , altUser , finalAlt );
                }
			}
		}
    
		if ( finalAlt )
			ing->AppendNode ( finalAlt );
		AutoMergeUtilities::resolveQualifier ( baseNode , latestNode , userNode , ing );

		return ing;
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
		if ( !verifyChildTypeOfIngredients ( arrayNode ) )
			return false;
		return true;

	}

	bool APICALL UniversalIngredientsStrategyImpl::InterestedInResolvingConflict ( const spcIUTF8String & nodePath , INode::eNodeType nodeType , IThreeWayUnResolvedConflict::eConflictReason reason , bool alreadyResolved )
	{
		static const char ingredientsPath[] = kXMP_NS_XMP_MM ":Ingredients";

		if ( alreadyResolved )
			return false;

		if ( strncmp ( nodePath->c_str ( ) , ingredientsPath , nodePath->size ( ) ) == 0 )
			return true;

		return false;
	}

	IThreeWayUnResolvedConflict::eConflictReason APICALL UniversalIngredientsStrategyImpl::ResolveConflict ( IThreeWayUnResolvedConflict::eConflictReason conflictReason ,
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
					"Ingredients must be of type Unordered Array of Structure" , IError_base::kESOperationFatal , false , false );
			resolvedNode = latestVersionNode;
			resolvedSource = IThreeWayResolvedConflict::kRSLatestVersion;
			conflictResolutionReason = IThreeWayResolvedConflict::kCRRStrategyResolved;
			return IThreeWayUnResolvedConflict::kCRResolved;
			break;

		case IThreeWayUnResolvedConflict::kCRTypesMismatch:
			NOTIFY_ERROR ( IError_base::kEDDataModel , kDMECDifferentNodeTypePresent ,
				"Ingredients must be of type Unordered Array " , IError_base::kESOperationFatal , false , false );
			break;

		case IThreeWayUnResolvedConflict::kCRQualifiersMismatch:
			if ( !verifyNode ( latestVersionNode ) || !verifyNode ( userVersionNode ) )
				NOTIFY_ERROR ( IError_base::kEDDataModel , kDMECDifferentNodeTypePresent ,
					"Ingredients must be of type Unordered Array of Structure" , IError_base::kESOperationFatal , false , false );
			mergedNode = latestVersionNode->Clone ( )->ConvertToArrayNode ( );
			if ( useBase )
				AutoMergeUtilities::resolveQualifier ( baseArrayNode , latestArrayNode , userArrayNode , mergedNode );
			resolvedNode = mergedNode;
			break;

		case IThreeWayUnResolvedConflict::kCRValuesMismatch:
		{
			if ( !verifyNode ( latestVersionNode ) || !verifyNode ( userVersionNode ) )
				NOTIFY_ERROR ( IError_base::kEDDataModel , kDMECDifferentNodeTypePresent ,
					"Ingredients must be of type Unordered Array of Structure" , IError_base::kESOperationFatal , false , false );
			latestArrayNode = latestVersionNode->ConvertToArrayNode ( );
			userArrayNode = userVersionNode->ConvertToArrayNode ( );
			
			std::vector<spcIStructureNode> missingPrimaryKeyStructs;

			std::map <std::pair<UniversalIngredientsStrategyImpl::eIngPrimaryKey , std::string> , spcIStructureNode > userArrayChildMap;
			std::map <std::pair<UniversalIngredientsStrategyImpl::eIngPrimaryKey , std::string> , spcIStructureNode > baseArrayChildMap;
			std::map <std::pair<UniversalIngredientsStrategyImpl::eIngPrimaryKey , std::string> , spIStructureNode > latestArrayChildMap;

			sizet userchildCount = userArrayNode->ChildCount ( );
			for ( XMP_Index i = 1; i <= (XMP_Index)userchildCount; i++ )
			{
				spcIStructureNode child = userArrayNode->GetStructureNodeAtIndex ( i );
				if ( child ) {
					std::pair<UniversalIngredientsStrategyImpl::eIngPrimaryKey , std::string>  hashUser = hashFunc ( child );

					if ( hashUser.first == UniversalIngredientsStrategyImpl::kIPKNone )
					{
						if ( findNode(missingPrimaryKeyStructs, child) )
						{
							duplicateEntries = true;
							break;
						}
						else
							missingPrimaryKeyStructs.push_back ( child );
					}
					else
					{
						if ( !userArrayChildMap.insert ( make_pair ( hashUser , child ) ).second )
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
				NOTIFY_WARNING ( IError_v1::kEDConflictResolution , kGECParametersNotAsExpected , "Duplicate Key in Ingredients" , false , false );
				resolvedNode = latestArrayNode->Clone ( );
				resolvedSource = IThreeWayResolvedConflict::kRSFieldsMerged;
				conflictResolutionReason = IThreeWayResolvedConflict::kCRRStrategyResolved;
				return IThreeWayUnResolvedConflict::kCRResolved;
			}

			if ( useBase )
			{
				sizet basechildCount = baseArrayNode->ChildCount ( );
				for ( XMP_Index i = 1; i <= (XMP_Index)basechildCount; i++ )
				{
					spcIStructureNode child = baseArrayNode->GetStructureNodeAtIndex ( i );
					if ( child )
						baseArrayChildMap.insert ( make_pair ( hashFunc ( child ) , child ) );
				}
			}

			std::vector<spcIStructureNode> missingPrimaryKeyTemp;

			sizet latestchildCount = latestArrayNode->ChildCount ( );
			for ( XMP_Index i = 1; i <= (XMP_Index)latestchildCount; i++ )
			{
				spcIStructureNode child = latestArrayNode->GetStructureNodeAtIndex ( i );
				if ( !child )
					continue;
				spIStructureNode clonedChild = child->Clone ( )->ConvertToStructureNode ( );
				if ( !clonedChild )
					continue;

				std::pair<UniversalIngredientsStrategyImpl::eIngPrimaryKey , std::string>  hashLatest = hashFunc ( clonedChild );

				if ( hashLatest.first == UniversalIngredientsStrategyImpl::kIPKNone )
				{
					if ( findNode ( missingPrimaryKeyTemp, child ) )
					{
						duplicateEntries = true;
						break;
					}
					else {
						missingPrimaryKeyTemp.push_back ( child );
						if(!findNode( missingPrimaryKeyStructs , child))
							missingPrimaryKeyStructs.push_back ( child );
					}
				}
				else
				{
					if ( latestArrayChildMap.find ( hashLatest ) != latestArrayChildMap.end ( ) )
					{
						duplicateEntries = true;
						break;
					}
					if ( !useBase )
					{
						latestArrayChildMap.insert ( make_pair ( hashLatest , clonedChild ) );
						if ( hashLatest.first == UniversalIngredientsStrategyImpl::kIPKSome )
						{
							std::map <std::pair<UniversalIngredientsStrategyImpl::eIngPrimaryKey , std::string> , spcIStructureNode >::iterator iter = userArrayChildMap.find ( hashLatest );
							if ( iter != userArrayChildMap.end ( ) )
							{
								//hack changing the key
								if ( !AutoMergeUtilities::compareStructOfSimpleAndArr ( iter->second , clonedChild , false , false ) )
								{
									hashLatest.second += ((char)i) ;
									latestArrayChildMap.insert ( make_pair ( hashLatest , iter->second->Clone ( )->ConvertToStructureNode ( ) ) );
								}
							}
						}

					}
					else
					{
						if ( userArrayChildMap.find ( hashLatest ) == userArrayChildMap.end ( ) )
						{
							if ( baseArrayChildMap.find ( hashLatest ) == baseArrayChildMap.end ( ) ||
								!AutoMergeUtilities::compareStructOfSimpleAndArr ( baseArrayChildMap.find ( hashLatest )->second , clonedChild , true ) )
								latestArrayChildMap.insert ( make_pair ( hashLatest , clonedChild ) );

							//else :: deleted in  user so no need to insert in map
						}

						else
						{
							//base present
							if ( baseArrayChildMap.find ( hashLatest ) != baseArrayChildMap.end ( ) )
							{
								//compare user and latest
								if ( !AutoMergeUtilities::compareStructOfSimpleAndArr ( userArrayChildMap.find ( hashLatest )->second , clonedChild , true ) )
								{
									//compare user and base
									if ( AutoMergeUtilities::compareStructOfSimpleAndArr ( userArrayChildMap.find ( hashLatest )->second , baseArrayChildMap.find ( hashLatest )->second , true ) )
									{
										latestArrayChildMap.insert ( make_pair ( hashLatest , clonedChild ) );
									}
									else
									{
										//compare base and latest
										if ( AutoMergeUtilities::compareStructOfSimpleAndArr ( baseArrayChildMap.find ( hashLatest )->second , clonedChild , true ) )
										{
											//insert user structure
											latestArrayChildMap.insert ( make_pair ( hashLatest , userArrayChildMap.find ( hashLatest )->second->Clone ( )->ConvertToStructureNode ( ) ) );
										}
										else
										{
											//merge structure of all three
											if ( hashLatest.first == UniversalIngredientsStrategyImpl::kIPKAll ) {
												spIStructureNode mergedStructure = MakeUnionIngredients ( baseArrayChildMap.find ( hashLatest )->second->Clone ( )->ConvertToStructureNode ( ) , clonedChild , userArrayChildMap.find ( hashLatest )->second->Clone ( )->ConvertToStructureNode ( ) );
												AutoMergeUtilities::resolveQualifier ( baseArrayChildMap.find ( hashLatest )->second , clonedChild , userArrayChildMap.find ( hashLatest )->second , mergedStructure );
												latestArrayChildMap.insert ( make_pair ( hashLatest , mergedStructure ) );

											}
											else if ( hashLatest.first == UniversalIngredientsStrategyImpl::kIPKSome )
											{
												//insert both as some keys are missing and complete ing are not same .
												latestArrayChildMap.insert ( make_pair ( hashLatest , clonedChild ) );
												hashLatest.second += ((char)i);
												latestArrayChildMap.insert ( make_pair ( hashLatest , userArrayChildMap.find ( hashLatest )->second->Clone ( )->ConvertToStructureNode ( ) ) );
											}
											//clonedChild->Clear ( true , false ); do in make union

										}
									}
								}
								else
								{
									AutoMergeUtilities::resolveQualifier ( baseArrayChildMap.find ( hashLatest )->second , clonedChild , userArrayChildMap.find ( hashLatest )->second , clonedChild );
									latestArrayChildMap.insert ( make_pair ( hashLatest , clonedChild ) );
								}
							}
							else
							{
								//if in base then compare whether base and latest are same if same then take user
								//otherwise latest 
								//if none are same then take union
								latestArrayChildMap.insert ( make_pair ( hashLatest , clonedChild ) );
								if (  hashLatest.first == UniversalIngredientsStrategyImpl::kIPKSome )
								{
									std::map <std::pair<UniversalIngredientsStrategyImpl::eIngPrimaryKey, std::string>, spcIStructureNode >::iterator iter = userArrayChildMap.find ( hashLatest );
									if ( iter != userArrayChildMap.end ( ) )
									{
										if ( !AutoMergeUtilities::compareStructOfSimpleAndArr ( iter->second , clonedChild , false , false ) )
										{
											hashLatest.second += ((char)i);
											latestArrayChildMap.insert ( make_pair ( hashLatest , iter->second->Clone ( )->ConvertToStructureNode ( ) ) );
										}
									}
								}

							}

						}
					}
				}
			}

			if ( duplicateEntries == true )
			{
				//notify_warning
				NOTIFY_WARNING ( IError_v1::kEDConflictResolution , kGECParametersNotAsExpected , "Duplicate Key in Ingredients" , false , false );

				resolvedNode = latestArrayNode->Clone ( );
				resolvedSource = IThreeWayResolvedConflict::kRSFieldsMerged;
				conflictResolutionReason = IThreeWayResolvedConflict::kCRRStrategyResolved;
				return IThreeWayUnResolvedConflict::kCRResolved;
			}

			for ( std::map <std::pair<UniversalIngredientsStrategyImpl::eIngPrimaryKey , std::string> , spcIStructureNode > ::iterator itUser = userArrayChildMap.begin ( ); itUser != userArrayChildMap.end ( ); itUser++ )
			{
				if ( !useBase )
				{
					if ( latestArrayChildMap.find ( itUser->first ) == latestArrayChildMap.end ( ) )
						latestArrayChildMap.insert ( std::make_pair ( itUser->first , itUser->second->Clone ( )->ConvertToStructureNode ( ) ) );
				}

				else
				{

					if ( latestArrayChildMap.find ( itUser->first ) == latestArrayChildMap.end ( ) &&
						baseArrayChildMap.find ( itUser->first ) == baseArrayChildMap.end ( ) )
						latestArrayChildMap.insert ( std::make_pair ( itUser->first , itUser->second->Clone ( )->ConvertToStructureNode ( ) ) );
					/*else
					{
					//base =user or base!=user, don't take user.

					}*/
				}
			}

			mergedNode = latestArrayNode->Clone ( )->ConvertToArrayNode ( );
			mergedNode->Clear ( true , false );

			std::map <std::pair<UniversalIngredientsStrategyImpl::eIngPrimaryKey , std::string> , spIStructureNode >::iterator itMergeStructChildMap = latestArrayChildMap.begin ( );

			for ( ; itMergeStructChildMap != latestArrayChildMap.end ( ); itMergeStructChildMap++ )
			{
				mergedNode->AppendNode ( itMergeStructChildMap->second );
			}

			std::vector<spcIStructureNode>::iterator itMissingPrimaryKeyStructs = missingPrimaryKeyStructs.begin ( );

			for ( ; itMissingPrimaryKeyStructs != missingPrimaryKeyStructs.end ( ); itMissingPrimaryKeyStructs++ )
			{
				mergedNode->AppendNode ( (*itMissingPrimaryKeyStructs)->Clone() );
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

		if ( resolvedNode )
		{
			resolvedSource = IThreeWayResolvedConflict::kRSFieldsMerged;
			conflictResolutionReason = IThreeWayResolvedConflict::kCRRStrategyResolved;
			return IThreeWayUnResolvedConflict::kCRResolved;
		}
		return conflictReason;
	}

}
