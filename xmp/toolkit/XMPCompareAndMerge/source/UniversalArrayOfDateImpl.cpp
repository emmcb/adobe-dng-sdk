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

#include "XMPCompareAndMerge/ImplHeaders/UniversalArrayOfDateImpl.h"
#include "XMPCompareAndMerge/ImplHeaders/AutoMergeUtilities.h"

#include <string>
#define TXMP_STRING_TYPE std::string 
#include "public/include/XMP.hpp" 

namespace AdobeXMPCompareAndMerge_Int {

	struct DateComparator {
		bool operator()( const XMP_DateTime & left , const XMP_DateTime & right ) const {
			int order = SXMPUtils::CompareDateTime ( left , right );
			if ( order < 0 )
				return true;
			return false;
		}
	};

	static XMP_DateTime ConvertToDate ( const spcISimpleNode & node ) {
		XMP_DateTime date;
		SXMPUtils::ConvertToDate ( node->GetValue ( )->c_str ( ) , &date );
		return date;
	}

	static bool operator < ( const XMP_DateTime & lhs , const XMP_DateTime & rhs ) {
		if ( SXMPUtils::CompareDateTime ( lhs , rhs ) < 0 )
			return true;
		return false;
	}

	spIThreeWayGenericStrategy UniversalArrayOfDateImpl::CreateArrayOfDateThreeWayGenericStrategy ( ) {
		return MakeUncheckedSharedPointer ( new UniversalArrayOfDateImpl ( ) , __FILE__ , __LINE__ , true );
	}

	ThreeWayGenericStrategyImpl * APICALL UniversalArrayOfDateImpl::createAndCloneContents ( ) const {
		return new UniversalArrayOfDateImpl ( );
	}

	bool APICALL UniversalArrayOfDateImpl::InterestedInResolvingConflict ( const spcIUTF8String & nodePath , INode::eNodeType nodeType , IThreeWayUnResolvedConflict::eConflictReason reason , bool alreadyResolved )
	{
		if ( alreadyResolved )
			return false;
		if ( mPaths->size ( ) == 0 )
			return false;
		sizet nodePathSize = nodePath->size ( );
		for ( auto it = mPaths->begin ( ); it != mPaths->end ( ); it++ )
		{
			sizet registeredPathSize = (*it)->size ( );
			if ( (nodePathSize == registeredPathSize) && (strncmp ( nodePath->c_str ( ) , (*it)->c_str ( ) , registeredPathSize ) == 0) ) {
				return true;
			}
		}
		return false;
	}

	static bool verifyNode ( spcINode node )
	{
		//This would verify if this is the correct strategy for this node.
		
		if ( !node || ( node && node->GetNodeType ( ) != INode::kNTArray) )
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

	static spIArrayNode ResolveValueMismatch ( spcIArrayNode baseArrayNode , spcIArrayNode latestArrayNode , spcIArrayNode userArrayNode , bool useBase ) {
		//Assumption: latest and base will not be null
		std::map <XMP_DateTime , spcISimpleNode , DateComparator> baseArrayChildMap;
		std::map <XMP_DateTime , spISimpleNode , DateComparator> latestArrayChildMap;
		std::map <XMP_DateTime , spcISimpleNode , DateComparator> userArrayChildMap;

		if ( useBase ) {
			sizet basechildCount = baseArrayNode->ChildCount ( );
			for ( sizet i = 1; i <= basechildCount; i++ )
			{
				spcISimpleNode child = baseArrayNode->GetSimpleNodeAtIndex ( i );
				XMP_DateTime date;
				try {
					date = ConvertToDate ( child );
				}
				catch ( ... )
				{
					continue;
				}

				if ( child )
					baseArrayChildMap.insert ( std::make_pair ( date , child ) );
			}
		}

		sizet userchildCount = userArrayNode->ChildCount ( );
		for ( sizet i = 1; i <= userchildCount; i++ )
		{
			spcISimpleNode child = userArrayNode->GetSimpleNodeAtIndex ( i );
			XMP_DateTime date;
			try {
				date = ConvertToDate ( child );
			}
			catch ( ... )
			{
				continue;
			}
			if ( child )
				userArrayChildMap.insert ( std::make_pair ( date , child ) );
		}

		//in below for loop check for elements in user map , i.e case of deletion in user
		sizet latestchildCount = latestArrayNode->ChildCount ( );
		for ( sizet i = 1; i <= latestchildCount; i++ )
		{

			spcISimpleNode child = latestArrayNode->GetSimpleNodeAtIndex ( i );
			if ( !child )
				continue;
			spISimpleNode clonedChild = child->Clone ( )->ConvertToSimpleNode ( );
			if ( !clonedChild )
				continue;

			XMP_DateTime date;
			try {
				date = ConvertToDate ( child );
			}
			catch ( ... )
			{
				continue;
			}

			//since in all cases (presence of qualifiers, absence, value diff, name diff: qualifier will be chosen on priority)
			if ( !useBase )
				latestArrayChildMap.insert ( std::make_pair ( date , clonedChild ) );
			else
			{
				if ( userArrayChildMap.find ( date ) == userArrayChildMap.end ( ) )
				{
					if ( baseArrayChildMap.find ( date ) == baseArrayChildMap.end ( ) ) {
						latestArrayChildMap.insert ( std::make_pair ( date , clonedChild ) );
					}
				}
				else
				{
					spcISimpleNode userNodeInMap = userArrayChildMap.find ( date )->second;
					if ( baseArrayChildMap.find ( date ) != baseArrayChildMap.end ( ) )
					{
						spcISimpleNode baseNodeInMap = baseArrayChildMap.find ( date )->second;
						AutoMergeUtilities::resolveQualifier ( baseNodeInMap , clonedChild , userNodeInMap , clonedChild );
					}
					latestArrayChildMap.insert ( std::make_pair ( date , clonedChild ) );
				}
			}
		}

		for ( std::map <XMP_DateTime , spcISimpleNode>::iterator itUser = userArrayChildMap.begin ( ); itUser != userArrayChildMap.end ( ); itUser++ )
		{
			spISimpleNode clonedChild = itUser->second->Clone ( )->ConvertToSimpleNode ( );

			if ( !useBase )
			{
				if ( latestArrayChildMap.find ( itUser->first ) == latestArrayChildMap.end ( ) )
					latestArrayChildMap.insert ( std::make_pair ( itUser->first , clonedChild ) );

			}
			else
			{
				if ( latestArrayChildMap.find ( itUser->first ) == latestArrayChildMap.end ( ) )
				{
					if ( baseArrayChildMap.find ( itUser->first ) == baseArrayChildMap.end ( ) )
						latestArrayChildMap.insert ( std::make_pair ( itUser->first , clonedChild ) );
				}
			}
		}

		AdobeXMPCore::spIArrayNode mergedNode = latestArrayNode->Clone ( )->ConvertToArrayNode ( );
		mergedNode->Clear ( true , false );

		std::map <XMP_DateTime , spISimpleNode>::iterator itLatestArrayChildMap = latestArrayChildMap.begin ( );

		for ( ; itLatestArrayChildMap != latestArrayChildMap.end ( ); itLatestArrayChildMap++ )
		{
			mergedNode->AppendNode ( itLatestArrayChildMap->second );
		}

		if ( useBase )
			AutoMergeUtilities::resolveQualifier ( baseArrayNode , latestArrayNode , userArrayNode , mergedNode );
		return mergedNode;
	}

	IThreeWayUnResolvedConflict::eConflictReason APICALL UniversalArrayOfDateImpl::ResolveConflict ( IThreeWayUnResolvedConflict::eConflictReason conflictReason ,
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
			resolvedNode = latestVersionNode;
			resolvedSource = IThreeWayResolvedConflict::kRSLatestVersion;
			conflictResolutionReason = IThreeWayResolvedConflict::kCRRStrategyResolved;
			return IThreeWayUnResolvedConflict::kCRResolved;
			break;

		case  IThreeWayUnResolvedConflict::kCRDeletedInUserEditedInLatest:
		{
			std::set<std::string> dateValues;
			mergedNode = latestVersionNode->Clone ( )->ConvertToArrayNode ( );
			mergedNode->Clear ( true , false );
			latestArrayNode = latestVersionNode->ConvertToArrayNode ( );
			sizet latestchildCount = latestArrayNode->ChildCount ( );
			for ( sizet i = 1; i <= latestchildCount; i++ )
			{
				spcISimpleNode child = latestArrayNode->GetSimpleNodeAtIndex ( i );
				if ( !child )
					continue;
				spISimpleNode clonedChild = child->Clone ( )->ConvertToSimpleNode ( );
				if ( !clonedChild )
					continue;

				XMP_DateTime date;
				try {
					date = ConvertToDate ( child );
					if ( dateValues.insert ( child->GetValue ( )->c_str ( ) ).second )
						mergedNode->AppendNode ( clonedChild );
				}
				catch ( ... )
				{
					continue;
				}
			}
			resolvedNode = mergedNode;
			break;
		}

		case IThreeWayUnResolvedConflict::kCRTypesMismatch:
			NOTIFY_ERROR ( IError_base::kEDDataModel , kDMECDifferentNodeTypePresent ,
				"Different type of node present in all three packets " , IError_base::kESOperationFatal , false , false );
			break;

		case IThreeWayUnResolvedConflict::kCRQualifiersMismatch:
			// not validating latest or user node for date. Can be done.
			if ( !verifyNode ( latestVersionNode ) || !verifyNode ( userVersionNode ) )
				NOTIFY_ERROR ( IError_base::kEDDataModel , kDMECDifferentNodeTypePresent ,
					"Ingredients must be of type Unordered Array of Structure" , IError_base::kESOperationFatal , false , false );
		
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
				NOTIFY_ERROR ( IError_base::kEDDataModel , kDMECDifferentNodeTypePresent ,
					"Different type of array present in packets " , IError_base::kESOperationFatal , false , false );

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
		//logicEnd

		if ( resolvedNode )
		{
			if (resolvedNode->IsEmpty())
				resolvedNode = spcINode();
			resolvedSource = IThreeWayResolvedConflict::kRSFieldsMerged;
			conflictResolutionReason = IThreeWayResolvedConflict::kCRRStrategyResolved;
			return IThreeWayUnResolvedConflict::kCRResolved;
		}
		return conflictReason;
	}
}
