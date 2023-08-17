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

#include "XMPCompareAndMerge/ImplHeaders/AutoMergeUtilities.h"

namespace AdobeXMPCompareAndMerge_Int {
	bool AutoMergeUtilities::verifyChildType ( const spcIArrayNode arrayNode ) {
		INode::eNodeType childType = arrayNode->GetChildNodeType ( );

		if ( childType == INode::kNTStructure ) {
			sizet arrayChildCount = arrayNode->ChildCount ( );
			for ( sizet i = 1; i <= arrayChildCount; i++ )
			{
				spcIStructureNode childStruct = arrayNode->GetStructureNodeAtIndex ( i );
				spcINodeIterator it = childStruct->Iterator ( );
				while ( it )
				{
					if ( it->GetNodeType ( ) != INode::kNTSimple )
						return false;
					it = it->Next ( );
				}
			}
		}
		else
			return false;
		return true;
	}

	bool AutoMergeUtilities::verifyChildTypeArrOfStructOfSimpleAndOrArr ( const spcIArrayNode arrayNode ) {
		INode::eNodeType childType = arrayNode->GetChildNodeType ( );

		if ( childType == INode::kNTStructure ) {
			sizet arrayChildCount = arrayNode->ChildCount ( );
			for ( sizet i = 1; i <= arrayChildCount; i++ )
			{
				spcIStructureNode childStruct = arrayNode->GetStructureNodeAtIndex ( i );
				spcINodeIterator it = childStruct->Iterator ( );
				while ( it )
				{
					if ( it->GetNodeType ( ) != INode::kNTSimple )
					{
						if ( !(it->GetNode ( )->GetNodeType ( ) == INode::kNTArray &&
							it->GetArrayNode ( )->GetArrayForm ( ) == IArrayNode::kAFOrdered
							&& it->GetArrayNode ( )->GetChildNodeType ( ) == INode::kNTSimple) )
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

	bool AutoMergeUtilities::verifyChildTypeArrOfStructOfSimpleAndStruct ( const spcIArrayNode arrayNode ) {
		INode::eNodeType childType = arrayNode->GetChildNodeType ( );

		if ( childType == INode::kNTStructure ) {
			sizet arrayChildCount = arrayNode->ChildCount ( );
			for ( sizet i = 1; i <= arrayChildCount; i++ )
			{
				spcIStructureNode childStruct = arrayNode->GetStructureNodeAtIndex ( i );
				spcINodeIterator it = childStruct->Iterator ( );
				while ( it )
				{
					if ( it->GetNodeType ( ) != INode::kNTSimple )
					{
						if ( !(it->GetNode ( )->GetNodeType ( ) == INode::kNTStructure) )
							return false;
						spcIStructureNode spChildStruct = it->GetNode ( )->ConvertToStructureNode ( );
						spcINodeIterator it1 = spChildStruct->Iterator ( );
						while ( it1 )
						{
							if ( it1->GetNodeType ( ) != INode::kNTSimple )
								return false;
							it1 = it1->Next ( );
						}
					}
					it = it->Next ( );
				}
			}
		}
		else
			return false;
		return true;
	}

	bool AutoMergeUtilities::compareOrderedArrayOfSimple ( const spcIArrayNode arrayNode1 , const spcIArrayNode arrayNode2 , bool compareQualifiers )
	{
		if ( arrayNode1->ChildCount ( ) != arrayNode2->ChildCount ( ) )
			return false;

		for ( sizet i = 1; i <= arrayNode1->ChildCount ( ); i++ )
		{
			spcISimpleNode child1 = arrayNode1->GetSimpleNodeAtIndex ( i );
			spcISimpleNode child2 = arrayNode2->GetSimpleNodeAtIndex ( i );

			if ( child1->GetValue ( )->compare ( child2->GetValue ( ) ) != 0 )
				return false;
			if ( compareQualifiers )
			{
				if ( !compareQualifier ( child1 , child2 ) )
					return false;
			}
		}
		return true;
	}

	bool AutoMergeUtilities::compareArrayOfSimple ( const spcIArrayNode arrayNode1 , const spcIArrayNode arrayNode2 , bool compareQualifiers )
	{
		if ( arrayNode1->ChildCount ( ) != arrayNode2->ChildCount ( ) )
			return false;
				
		for ( sizet i = 1; i <= arrayNode1->ChildCount ( ); i++ )
		{
			spcISimpleNode child1 = arrayNode1->GetSimpleNodeAtIndex ( i );
			spcIUTF8String value1 = child1->GetValue ( );
			sizet j;
			for (j = 1; j <= arrayNode1->ChildCount ( ); j++ ) {
				spcISimpleNode child2 = arrayNode2->GetSimpleNodeAtIndex ( j );
				spcIUTF8String value2 = child2->GetValue ( );
				if ( value1->compare ( value2 ) == 0 )
				{
					if (compareQualifiers)
					{
						if (compareQualifier(child1, child2))
							break;
					}
					else
						 break;
				}
			}

			if ( j  == arrayNode1->ChildCount ( ) + 1 )
				return false;
		}
		return true;
	}

	bool AutoMergeUtilities::isQualEqual ( spcINodeIterator it1 , spcINodeIterator it2 )
	{
		if ( (it1->GetSimpleNode ( )->GetName ( )->compare ( it2->GetSimpleNode ( )->GetName ( ) ) == 0) &&
			(it1->GetSimpleNode ( )->GetNameSpace ( )->compare ( it2->GetSimpleNode ( )->GetNameSpace ( ) ) == 0) &&
			(it1->GetSimpleNode ( )->GetValue ( )->compare ( it2->GetSimpleNode ( )->GetValue ( ) ) == 0)
			)
			return true;
		else
			return false;
	}

	void AutoMergeUtilities::resolveQualifier ( const spcINode baseNode , const spcINode latestNode , const spcINode userNode , const spINode & nodeForQualifier ) {
		//Assumption: nodeforQualifier has the qulaifier of latest node already present. If resolveQualifier determines that the qulaifier to be used are of 
		//latest node then they are not appended based on the assumption.
		//If count of qualifier is greater than one or is not of simple type then qualifier of latest is taken. 3 qual on base, 0 on user and same 3 on latest are also not handled.

		//For code readability point of view some conditions are not mergered, they can be done after testing (if required), otherwise code will mess up

		if ( baseNode->HasQualifiers ( ) && baseNode->QualifiersCount ( ) == 1 )
		{
			spcINodeIterator itBase = baseNode->QualifiersIterator ( );
			if ( latestNode->HasQualifiers ( ) && latestNode->QualifiersCount ( ) == 1 && userNode->HasQualifiers ( ) && userNode->QualifiersCount ( ) == 1 )
			{
				spcINodeIterator itLatest = latestNode->QualifiersIterator ( );
				spcINodeIterator itUser = userNode->QualifiersIterator ( );

				if ( itLatest->GetNodeType ( ) == INode::kNTSimple && itUser->GetNodeType ( ) == INode::kNTSimple && itBase->GetNodeType ( ) == INode::kNTSimple )
				{
					if ( isQualEqual ( itLatest , itUser ) == false )
					{
						if ( isQualEqual ( itLatest , itBase ) == true )
						{
							//quali of user
							nodeForQualifier->Clear ( false , true );
							nodeForQualifier->InsertQualifier ( itUser->GetSimpleNode ( )->Clone ( ) );
						}
					}
				}
			}
			else if ( latestNode->HasQualifiers ( ) && latestNode->QualifiersCount ( ) == 1 && !userNode->HasQualifiers ( ) ) {
				spcINodeIterator itLatest = latestNode->QualifiersIterator ( );
				if ( itLatest->GetNodeType ( ) == INode::kNTSimple &&  itBase->GetNodeType ( ) == INode::kNTSimple && isQualEqual ( itLatest , itBase ) )
					nodeForQualifier->Clear ( false , true ); //as qualifier is deleted by user
			}
		}
		else if ( !baseNode->HasQualifiers ( ) ) {
			//base does not have qualifiers
			if ( !latestNode->HasQualifiers ( ) && userNode->HasQualifiers ( ) && userNode->QualifiersCount ( ) ) {
				//quali of user
				spcINodeIterator itUser = userNode->QualifiersIterator ( );
				if ( itUser->GetNodeType ( ) == INode::kNTSimple )
				{
					nodeForQualifier->Clear ( false , true );
					nodeForQualifier->InsertQualifier ( itUser->GetSimpleNode ( )->Clone ( ) );
				}
			}
		}
	}

	void AutoMergeUtilities::makeUnion ( const spcIArrayNode baseNode , const spcIArrayNode latestNode , const spcIArrayNode userNode , const spIArrayNode& mergedNode , bool useBase )
	{
		//Assumption: latest and base will not be null
		std::map <std::string , spcISimpleNode> baseChildMap;
		std::map <std::string , spISimpleNode> latestChildMap;
		std::map <std::string , spcISimpleNode> userChildMap;

		if ( useBase ) {
			spcINodeIterator itBase = baseNode->Iterator ( );
			while ( itBase )
			{
				spcISimpleNode child = itBase->GetSimpleNode ( );
				if ( child )
					baseChildMap.insert ( std::make_pair ( child->GetValue ( )->c_str ( ) , child ) );
				itBase = itBase->Next ( );
			}
		}

		spcINodeIterator itUser = userNode->Iterator ( );
		while ( itUser )
		{
			spcISimpleNode child = itUser->GetSimpleNode ( );
			if ( child )
				userChildMap.insert ( std::make_pair ( child->GetValue ( )->c_str ( ) , child ) );
			itUser = itUser->Next ( );
		}

		//in below for loop check for elements in user map , i.e case of deleteion in user
		spcINodeIterator itLatest = latestNode->Iterator ( );
		while ( itLatest )
		{
			spcISimpleNode child = itLatest->GetSimpleNode ( );
			if ( !child )
				continue;
			spISimpleNode clonedChild = child->Clone ( )->ConvertToSimpleNode ( );
			if ( !clonedChild )
				continue;

			//since in all cases (presence of qualifiers, absence, value diff, name diff: qualifier will be chosen on priority)
			if ( !useBase ) {
				if ( latestChildMap.insert ( std::make_pair ( child->GetValue ( )->c_str ( ) , clonedChild ) ).second )
					mergedNode->AppendNode ( clonedChild );
			}
			else
			{
				if ( userChildMap.find ( child->GetValue ( )->c_str ( ) ) == userChildMap.end ( ) )
				{
					if ( baseChildMap.find ( child->GetValue ( )->c_str ( ) )  == baseChildMap.end ( ) ) {
						if ( latestChildMap.insert ( std::make_pair ( child->GetValue ( )->c_str ( ), clonedChild ) ).second )
							mergedNode->AppendNode ( clonedChild );
					}
				}
				else
				{
					spcISimpleNode userNodeInMap = userChildMap.find ( child->GetValue ( )->c_str ( ))->second;
					if ( baseChildMap.find ( child->GetValue ( )->c_str ( ) ) != baseChildMap.end ( ) )
					{
						spcISimpleNode baseNodeInMap = baseChildMap.find (child->GetValue ( )->c_str ( ) )->second;
						resolveQualifier ( baseNodeInMap , clonedChild , userNodeInMap , clonedChild );
					}
					if ( latestChildMap.insert ( std::make_pair ( child->GetValue ( )->c_str ( ), clonedChild ) ).second )
						mergedNode->AppendNode ( clonedChild );
				}
			}
			itLatest = itLatest->Next ( );
		}

		itUser = userNode->Iterator ( );
		while ( itUser )
		{
			spcISimpleNode child = itUser->GetSimpleNode ( );

			if ( !child )
				continue;
			spISimpleNode clonedChild = child->Clone ( )->ConvertToSimpleNode ( );
			if ( !clonedChild )
				continue;
			if ( !useBase )
			{
				if ( latestChildMap.find ( child->GetValue ( )->c_str ( ) ) == latestChildMap.end ( ) )
				{
					if ( latestChildMap.insert ( std::make_pair ( child->GetValue ( )->c_str ( ) , clonedChild ) ).second )
						mergedNode->AppendNode ( clonedChild );
				}
			}
			else
			{

				if ( latestChildMap.find ( child->GetValue ( )->c_str ( )) == latestChildMap.end ( ) )
				{
					if ( baseChildMap.find (child->GetValue ( )->c_str ( )) == baseChildMap.end ( ) )
					{
						if ( latestChildMap.insert ( std::make_pair (child->GetValue ( )->c_str ( ), clonedChild ) ).second )
							mergedNode->AppendNode ( clonedChild );
					}
				}
			}
			itUser = itUser->Next ( );
		}
	}

	void AutoMergeUtilities::makeUnion ( const spcIStructureNode baseNode , const spcIStructureNode latestNode , const spcIStructureNode userNode , const spIStructureNode& mergedNode , bool useBase )
	{
		//latest and base cannot be null
		std::map <std::pair<std::string , std::string> , spcISimpleNode> baseChildMap;
		std::map <std::pair<std::string , std::string> , spISimpleNode> latestChildMap;
		std::map <std::pair<std::string , std::string> , spcISimpleNode> userChildMap;

		if ( useBase ) {
			spcINodeIterator itBase = baseNode->Iterator ( );
			while ( itBase )
			{
				spcISimpleNode child = itBase->GetSimpleNode ( );
				if ( child )
					baseChildMap.insert ( std::make_pair ( std::make_pair ( child->GetNameSpace ( )->c_str ( ) , child->GetName ( )->c_str ( ) ) , child ) );
				itBase = itBase->Next ( );
			}
		}

		spcINodeIterator itUser = userNode->Iterator ( );
		while ( itUser )
		{
			spcISimpleNode child = itUser->GetSimpleNode ( );
			if ( child )
				userChildMap.insert ( std::make_pair ( std::make_pair ( child->GetNameSpace ( )->c_str ( ) , child->GetName ( )->c_str ( ) ) , child ) );
			itUser = itUser->Next ( );
		}

		//in below for loop check for elements in user map , i.e case of deleteion in user
		spcINodeIterator itLatest = latestNode->Iterator ( );
		while ( itLatest )
		{
			spcISimpleNode child = itLatest->GetSimpleNode ( );
			if ( !child )
				continue;
			spISimpleNode clonedChild = child->Clone ( )->ConvertToSimpleNode ( );
			if ( !clonedChild )
				continue;

			//since in all cases (presence of qualifiers, absence, value diff, name diff: qualifier will be chosen on priority)
			if ( !useBase ) {
				if ( latestChildMap.insert ( std::make_pair ( std::make_pair ( child->GetNameSpace ( )->c_str ( ) , child->GetName ( )->c_str ( ) ) , clonedChild ) ).second )
					mergedNode->AppendNode ( clonedChild );
			}
			else
			{
				if ( userChildMap.find ( std::make_pair ( child->GetNameSpace ( )->c_str ( ) , child->GetName ( )->c_str ( ) ) ) == userChildMap.end ( ) )
				{
					if ( baseChildMap.find ( std::make_pair ( child->GetNameSpace ( )->c_str ( ) , child->GetName ( )->c_str ( ) ) ) == baseChildMap.end ( ) ) {
						if ( latestChildMap.insert ( std::make_pair ( std::make_pair ( child->GetNameSpace ( )->c_str ( ) , child->GetName ( )->c_str ( ) ) , clonedChild ) ).second )
							mergedNode->AppendNode ( clonedChild );
					}
					else {
						if ( child->GetValue ( )->compare ( baseChildMap.find ( std::make_pair ( child->GetNameSpace ( )->c_str ( ) , child->GetName ( )->c_str ( ) ) )->second->GetValue ( ) ) != 0 ) {
							if ( latestChildMap.insert ( std::make_pair ( std::make_pair ( child->GetNameSpace ( )->c_str ( ) , child->GetName ( )->c_str ( ) ) , clonedChild ) ).second )
								mergedNode->AppendNode ( clonedChild );
						}

					}
				}
				else
				{
					spcISimpleNode userNodeInMap = userChildMap.find ( std::make_pair ( child->GetNameSpace ( )->c_str ( ) , child->GetName ( )->c_str ( ) ) )->second;
					if ( baseChildMap.find ( std::make_pair ( child->GetNameSpace ( )->c_str ( ) , child->GetName ( )->c_str ( ) ) ) != baseChildMap.end ( ) )
					{
						spcISimpleNode baseNodeInMap = baseChildMap.find ( std::make_pair ( child->GetNameSpace ( )->c_str ( ) , child->GetName ( )->c_str ( ) ) )->second;
						if ( userNodeInMap->GetValue ( )->compare ( child->GetValue ( ) ) != 0 )
						{
							//compare user and base
							if ( userNodeInMap->GetValue ( )->compare ( baseNodeInMap->GetValue ( ) ) == 0 )
							{
								if ( latestChildMap.insert ( std::make_pair ( std::make_pair ( child->GetNameSpace ( )->c_str ( ) , child->GetName ( )->c_str ( ) ) , clonedChild ) ).second )
									mergedNode->AppendNode ( clonedChild );
							}
							else
							{
								//compare base and latest
								if ( child->GetValue ( )->compare ( baseNodeInMap->GetValue ( ) ) == 0 )
								{
									//insert user structure
									if ( latestChildMap.insert ( std::make_pair ( std::make_pair ( userNodeInMap->GetNameSpace ( )->c_str ( ) , userNodeInMap->GetName ( )->c_str ( ) ) , userNodeInMap->Clone ( )->ConvertToSimpleNode ( ) ) ).second )
										mergedNode->AppendNode ( userNodeInMap->Clone ( )->ConvertToSimpleNode ( ) );
								}
								else
								{
									//merge structure of all three

									if ( latestChildMap.insert ( std::make_pair ( std::make_pair ( child->GetNameSpace ( )->c_str ( ) , child->GetName ( )->c_str ( ) ) , clonedChild ) ).second )
										mergedNode->AppendNode ( clonedChild );
								}
							}
						}
						else
						{
							if ( child->GetValue ( )->compare ( baseNodeInMap->GetValue ( ) ) == 0 ) {
								resolveQualifier ( baseNodeInMap , clonedChild , userNodeInMap , clonedChild );
							}
							if ( latestChildMap.insert ( std::make_pair ( std::make_pair ( child->GetNameSpace ( )->c_str ( ) , child->GetName ( )->c_str ( ) ) , clonedChild ) ).second )
								mergedNode->AppendNode ( clonedChild );
						}
						//resolveQualifier(baseNodeInMap, clonedChild, userNodeInMap, clonedChild);
					}
					else
					{
						if ( latestChildMap.insert ( std::make_pair ( std::make_pair ( child->GetNameSpace ( )->c_str ( ) , child->GetName ( )->c_str ( ) ) , clonedChild ) ).second )
							mergedNode->AppendNode ( clonedChild );
					}
				}
			}
			itLatest = itLatest->Next ( );
		}

		itUser = userNode->Iterator ( );
		while ( itUser )
		{
			spcISimpleNode child = itUser->GetSimpleNode ( );

			if ( !child )
				continue;
			spISimpleNode clonedChild = child->Clone ( )->ConvertToSimpleNode ( );
			if ( !clonedChild )
				continue;

			if ( !useBase )
			{
				if ( latestChildMap.find ( std::make_pair ( child->GetNameSpace ( )->c_str ( ) , child->GetName ( )->c_str ( ) ) ) == latestChildMap.end ( ) )
				{
					if ( latestChildMap.insert ( std::make_pair ( std::make_pair ( child->GetNameSpace ( )->c_str ( ) , child->GetName ( )->c_str ( ) ) , clonedChild ) ).second )
						mergedNode->AppendNode ( clonedChild );
				}
			}
			else
			{
				if ( latestChildMap.find ( std::make_pair ( child->GetNameSpace ( )->c_str ( ) , child->GetName ( )->c_str ( ) ) ) == latestChildMap.end ( ) )
				{
					if ( baseChildMap.find ( std::make_pair ( child->GetNameSpace ( )->c_str ( ) , child->GetName ( )->c_str ( ) ) ) == baseChildMap.end ( ) )
					{
						if ( latestChildMap.insert ( std::make_pair ( std::make_pair ( child->GetNameSpace ( )->c_str ( ) , child->GetName ( )->c_str ( ) ) , clonedChild ) ).second )
							mergedNode->AppendNode ( clonedChild );
					}
				}
			}
			itUser = itUser->Next ( );
		}
	}

	//Comparison on the basis of values, TBD: on basis of qualifiers
	bool AutoMergeUtilities::compareStructs ( const spcIStructureNode latestNode , const spcIStructureNode userNode , bool compareQualifiers )
	{
		if ( latestNode->ChildCount ( ) != userNode->ChildCount ( ) )
			return false;
		spcINodeIterator itFirst = latestNode->Iterator ( );
		while ( itFirst )
		{
			spcISimpleNode child = itFirst->GetSimpleNode ( );
			spcISimpleNode field = userNode->GetSimpleNode ( child->GetNameSpace ( )->c_str ( ) , AdobeXMPCommon::npos , child->GetName ( )->c_str ( ) , AdobeXMPCommon::npos );
			if ( !field || field->GetValue ( )->compare ( child->GetValue ( ) ) != 0 )
				return false;
			if ( compareQualifiers )
			{
				if ( !compareQualifier ( child , field ) )
					return false;
			}
			itFirst = itFirst->Next ( );
		}

		return true;
	}

	bool AutoMergeUtilities::compareStructOfSimpleAndStruct ( const spcIStructureNode struct1 , const spcIStructureNode struct2 , bool compareQualifiers )
	{
		if ( struct1->ChildCount ( ) != struct2->ChildCount ( ) )
			return false;
		spcINodeIterator it1 = struct1->Iterator ( );
		while ( it1 )
		{
			spcINode child1 = it1->GetNode ( );
			spcINode child2 = struct2->GetNode ( child1->GetNameSpace ( )->c_str ( ) , AdobeXMPCommon::npos , child1->GetName ( )->c_str ( ) , AdobeXMPCommon::npos );

			if ( !child2 )
				return false;
			if ( child1->ConvertToSimpleNode ( ) && child2->ConvertToSimpleNode ( ) )
			{
				if ( child1->ConvertToSimpleNode ( )->GetValue ( )->compare ( child2->ConvertToSimpleNode ( )->GetValue ( ) ) != 0 )
					return false;
				if ( compareQualifiers )
					if ( !compareQualifier ( child1->ConvertToSimpleNode ( ) , child2->ConvertToSimpleNode ( ) ) )
						return false;
			}
			else
			{
				if ( !(child1->ConvertToStructureNode ( ) && child2->ConvertToStructureNode ( )) )
					return false;
				if ( !AutoMergeUtilities::compareStructs ( child1->ConvertToStructureNode ( ) , child2->ConvertToStructureNode ( ) , compareQualifiers ) )
					return false;
			}
			it1 = it1->Next ( );
		}
		return true;
	}

	bool AutoMergeUtilities::compareStructOfSimpleAndArr ( const spcIStructureNode struct1 , const spcIStructureNode struct2 , bool checkOrder , bool compareQualifiers )
	{
		if ( struct1->ChildCount ( ) != struct2->ChildCount ( ) )
			return false;
		spcINodeIterator it1 = struct1->Iterator ( );
		while ( it1 )
		{
			spcINode child1 = it1->GetNode ( );
			spcINode child2 = struct2->GetNode ( child1->GetNameSpace ( )->c_str ( ) , AdobeXMPCommon::npos , child1->GetName ( )->c_str ( ) , AdobeXMPCommon::npos );

			if ( !child2 )
				return false;
			if ( child1->ConvertToSimpleNode ( ) && child2->ConvertToSimpleNode ( ) )
			{
				if ( child1->ConvertToSimpleNode ( )->GetValue ( )->compare ( child2->ConvertToSimpleNode ( )->GetValue ( ) ) != 0 )
					return false;
				if ( compareQualifiers )
					if ( !compareQualifier ( child1->ConvertToSimpleNode ( ) , child2->ConvertToSimpleNode ( ) ) )
						return false;
			}
			else
			{
				if ( !(child1->ConvertToArrayNode ( ) && child2->ConvertToArrayNode ( )) )
					return false;
				if ( !(checkOrder ? AutoMergeUtilities::compareOrderedArrayOfSimple ( child1->ConvertToArrayNode ( ) , child2->ConvertToArrayNode ( ) , compareQualifiers ) : AutoMergeUtilities::compareArrayOfSimple ( child1->ConvertToArrayNode ( ) , child2->ConvertToArrayNode ( ) , compareQualifiers )) )
					return false;
			}
			it1 = it1->Next ( );
		}
		return true;
	}

	bool AutoMergeUtilities::compareQualifier ( spcISimpleNode node1 , spcISimpleNode node2 )
	{
		spcINodeIterator node1It;
		spcINodeIterator node2It;
		if ( node1->HasQualifiers ( ) && node2->HasQualifiers ( ) ) {
			if ( node1->QualifiersCount ( ) == 1 && node2->QualifiersCount ( ) == 1 && (node1It = node1->QualifiersIterator ( ))->GetNodeType ( ) == INode::kNTSimple
				&& (node2It = node2->QualifiersIterator ( ))->GetNodeType ( ) == INode::kNTSimple )
			{
				if ( !isQualEqual ( node1It , node2It ) )
					return false;
			}
			else
			{
				return false;
			}
		}
		else if ( (!node1->HasQualifiers ( ) && node2->HasQualifiers ( )) || (node1->HasQualifiers ( ) && !node2->HasQualifiers ( )) )
			return false;
		return true;
	}
}
