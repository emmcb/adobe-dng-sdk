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

#include "XMPCompareAndMerge/ImplHeaders/UniversalOldestDateStrategyImpl.h"
#include "XMPCompareAndMerge/ImplHeaders/AutoMergeUtilities.h"

#include <string>
#define TXMP_STRING_TYPE std::string 
#include "public/include/XMP.hpp" 
#include "public/include/XMP.incl_cpp"

namespace AdobeXMPCompareAndMerge_Int {

	spIThreeWayGenericStrategy UniversalOldestDateStrategyImpl::CreateUniversalOldestDateStrategyImpl ( ) {
		return MakeUncheckedSharedPointer ( new UniversalOldestDateStrategyImpl ( ) , __FILE__ , __LINE__ , true );
	}

	ThreeWayGenericStrategyImpl * APICALL UniversalOldestDateStrategyImpl::createAndCloneContents ( ) const {
		return new UniversalOldestDateStrategyImpl ( );
	}

	bool APICALL UniversalOldestDateStrategyImpl::InterestedInResolvingConflict ( const spcIUTF8String & nodePath , INode::eNodeType nodeType , IThreeWayUnResolvedConflict::eConflictReason reason , bool alreadyResolved ) {
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

	static bool verifyNode ( spcINode node )
	{
		//This would verify if this is the correct strategy for this node.
		if ( !node )
			return false;
		INode::eNodeType nodeType = node->GetNodeType ( );
		XMP_DateTime date;
		if ( nodeType != INode::kNTSimple )
			return false;
		try {
			date = ConvertToDate ( node->ConvertToSimpleNode ( ) );
		}
		catch ( ... )
		{
			return false;
		}
		return true;
	}

	IThreeWayUnResolvedConflict::eConflictReason APICALL UniversalOldestDateStrategyImpl::ResolveConflict ( IThreeWayUnResolvedConflict::eConflictReason conflictReason ,
		const spcINode & baseVersionNode , const spcINode & latestVersionNode , const spcINode & userVersionNode ,
		spcINode & resolvedNode , IThreeWayResolvedConflict::eConflictResolutionReason & conflictResolutionReason , IThreeWayResolvedConflict::eResolvedSource & resolvedSource )
	{
		switch ( conflictReason ) {
		case IThreeWayUnResolvedConflict::kCRURIMismatch:
			return conflictReason;
			break;


		case IThreeWayUnResolvedConflict::kCRTypesMismatch:
			NOTIFY_ERROR ( IError_base::kEDDataModel , kDMECDifferentNodeTypePresent ,
				"Node type is incompatible with the registered strategy" , IError_base::kESOperationFatal , false , false );
			break;

		case IThreeWayUnResolvedConflict::kCRQualifiersMismatch:
		{
			if ( !verifyNode ( latestVersionNode ) || !verifyNode ( userVersionNode ) )
				NOTIFY_ERROR ( IError_base::kEDDataModel , kDMECDifferentNodeTypePresent ,
					"Node type is incompatible with the registered strategy" , IError_base::kESOperationFatal , false , false );
			AdobeXMPCore::spISimpleNode mergedNode = latestVersionNode->Clone ( )->ConvertToSimpleNode ( );
			if ( verifyNode ( baseVersionNode ) )
				AutoMergeUtilities::resolveQualifier ( baseVersionNode , latestVersionNode , userVersionNode , mergedNode );
			resolvedNode = mergedNode;
			resolvedSource = IThreeWayResolvedConflict::kRSFieldsMerged;
			conflictResolutionReason = IThreeWayResolvedConflict::kCRRStrategyResolved;
			return IThreeWayUnResolvedConflict::kCRResolved;
			break;
		}

		case IThreeWayUnResolvedConflict::kCRDeletedInLatestEditedInUser:
		case IThreeWayUnResolvedConflict::kCRDeletedInUserEditedInLatest:
		case IThreeWayUnResolvedConflict::kCRValuesMismatch:
		{
			XMP_DateTime oldestDate;
			bool resolved ( false );

			if ( baseVersionNode ) {
				if ( verifyNode ( baseVersionNode ) ) {
					resolved = true;
					if ( !baseVersionNode->ConvertToSimpleNode ( ) )
						return conflictReason;
					try {
						oldestDate = ConvertToDate ( baseVersionNode->ConvertToSimpleNode ( ) );
					}
					catch ( ... )
					{
						return conflictReason;
					}
					resolvedNode = baseVersionNode;
					resolvedSource = IThreeWayResolvedConflict::kRSBaseVersion;
					conflictResolutionReason = IThreeWayResolvedConflict::kCRRStrategyResolved;
				}
			}

			if ( latestVersionNode ) {
				if ( !verifyNode ( latestVersionNode ) )
					NOTIFY_ERROR ( IError_base::kEDDataModel , kDMECDifferentNodeTypePresent ,
						"Node type is incompatible with the registered strategy" , IError_base::kESOperationFatal , false , false );
				resolved = true;
				XMP_DateTime date;
				if ( !latestVersionNode->ConvertToSimpleNode ( ) )
					return conflictReason;
				try {
					date = ConvertToDate ( latestVersionNode->ConvertToSimpleNode ( ) );
				}
				catch ( ... )
				{
					return conflictReason;
				}
				if ( date < oldestDate ) {
					oldestDate = date;
					resolvedNode = latestVersionNode;
					resolvedSource = IThreeWayResolvedConflict::kRSLatestVersion;
					conflictResolutionReason = IThreeWayResolvedConflict::kCRRStrategyResolved;
				}
			}

			if ( userVersionNode ) {
				if ( !verifyNode ( userVersionNode ) )
					NOTIFY_ERROR ( IError_base::kEDDataModel , kDMECDifferentNodeTypePresent ,
						"Node type is incompatible with the registered strategy" , IError_base::kESOperationFatal , false , false );
				resolved = true;
				XMP_DateTime date;
				if ( !userVersionNode->ConvertToSimpleNode ( ) )
					return conflictReason;
				try {
					date = ConvertToDate ( userVersionNode->ConvertToSimpleNode ( ) );
				}
				catch ( ... )
				{
					return conflictReason;
				}
				if ( date < oldestDate ) {
					oldestDate = date;
					resolvedNode = userVersionNode;
					resolvedSource = IThreeWayResolvedConflict::kRSUserVersion;
					conflictResolutionReason = IThreeWayResolvedConflict::kCRRStrategyResolved;
				}
			}

			if ( resolved )
				return IThreeWayUnResolvedConflict::kCRResolved;
			return conflictReason;
		}
		break;

		default:
			return conflictReason;
			break;
		}
	}

}
