// =================================================================================================
// Copyright Adobe
// Copyright 2014 Adobe
// All Rights Reserved
//
// NOTICE:  Adobe permits you to use, modify, and distribute this file in accordance with the terms
// of the Adobe license agreement accompanying it. If you have received this file from a source other 
// than Adobe, then your use, modification, or distribution of it requires the prior written permission
// of Adobe.
// =================================================================================================

#define IMPLEMENTATION_HEADERS_CAN_BE_INCLUDED 1
	#include "XMPCompareAndMerge/ImplHeaders/TwoWayConflictResolutionStrategyForThreeWayConflictIdentification.h"
#undef IMPLEMENTATION_HEADERS_CAN_BE_INCLUDED

#include "XMPCompareAndMerge/Interfaces/ITwoWayComparisonEngine.h"
#include "XMPCompareAndMerge/Interfaces/ITwoWayBasicComparisonResult.h"
#include "XMPCompareAndMerge/Interfaces/ITwoWayValueComparisonResult.h"

#include "XMPCommon/Interfaces/IError_I.h"
#include "XMPCommon/Interfaces/IUTF8String.h"
#include "XMPCommon/Utilities/TSmartPointers_I.h"
#include "XMPCommon/XMPCommonErrorCodes.h"

#include "XMPCore/Interfaces/IArrayNode.h"
#include "XMPCore/Interfaces/ISimpleNode.h"
#include "XMPCore/Interfaces/IMetadata.h"

namespace AdobeXMPCompareAndMerge_Int {

	
	spITwoWayConflictResolutionStrategy TwoWayConflictResolutionStrategyForThreeWayConflictIdentification::CreateStrategy( const spcIMetadata & commonBaseMetadata ) {
		return MakeUncheckedSharedPointer( new TwoWayConflictResolutionStrategyForThreeWayConflictIdentification( commonBaseMetadata ), __FILE__, __LINE__, true );
	}

	TwoWayConflictResolutionStrategyForThreeWayConflictIdentification::TwoWayConflictResolutionStrategyForThreeWayConflictIdentification( const spcIMetadata & commonBaseMetadata )
		: mCommonBaseMetadata( commonBaseMetadata )
		, mComparisonEngine( ITwoWayComparisonEngine::CreateTwoWayComparisonEngine() ) { }

	bool TwoWayConflictResolutionStrategyForThreeWayConflictIdentification::compareValues( const spcINode & first, const spcINode & second, bool doBasicComparison,
		bool doQualifiersComparison, bool doValueComparison ) const
	{
		if ( doBasicComparison ) {
			spcITwoWayBasicComparisonResult basicComparisonResult = mComparisonEngine->CompareBasicProperties( first, second );
			if ( !basicComparisonResult->IsEqual() )
				return false;

			switch ( first->GetNodeType() ) {
			case INode::kNTSimple:
				basicComparisonResult = mComparisonEngine->CompareBasicProperties( first->ConvertToSimpleNode(), second->ConvertToSimpleNode() );
				break;

			case INode::kNTArray:
				basicComparisonResult = mComparisonEngine->CompareBasicProperties( first->ConvertToArrayNode(), second->ConvertToArrayNode() );
				break;

			case INode::kNTStructure:
				basicComparisonResult = mComparisonEngine->CompareBasicProperties( first->ConvertToStructureNode(), second->ConvertToStructureNode() );
				break;

			default:
				NOTIFY_ERROR( IError::kEDGeneral, kGECLogicalError, "Unhandled Situation Occured", IError::kESOperationFatal, false, false );
				break;
			}

			if ( !basicComparisonResult->IsEqual() )
				return false;
		}

		if ( doQualifiersComparison ) {
			bool firstHasQualifiers = first->HasQualifiers(), secondHasQualifiers = second->HasQualifiers();

			if ( ( firstHasQualifiers && !secondHasQualifiers ) || ( secondHasQualifiers && !firstHasQualifiers ) ) {
				return false;
			} else if ( firstHasQualifiers && secondHasQualifiers ) {
				spcITwoWayValueComparisonResult qualifiersComparisonResult = mComparisonEngine->CompareQualifiers( first, second );
				if ( !qualifiersComparisonResult->IsExactMatch() )
					return false;
			}
		}

		if ( doValueComparison ) {
			spcITwoWayValueComparisonResult valueComparisonResult = mComparisonEngine->CompareValues( first, second );
			if ( !valueComparisonResult->IsExactMatch() )
				return false;
		}

		return true;
	}

	IThreeWayUnResolvedConflict::eConflictReason TwoWayConflictResolutionStrategyForThreeWayConflictIdentification::handleOnlyPresentInOneMetadataTree(
		bool presentInLatest, const spcINode & node, spcINode & resolvedNode, IThreeWayResolvedConflict::eConflictResolutionReason & reasonForConflictResolution,
		IThreeWayResolvedConflict::eResolvedSource & resolvedNodeSource ) const
	{
		spcINode nodeInCommonBaseTree;
		bool presentInCommonBaseTree = mComparisonEngine->IsEquivalentNodePresentInOtherTree( node, mCommonBaseMetadata, nodeInCommonBaseTree );
		if ( presentInCommonBaseTree ) {
			bool bothEqual = compareValues( node, nodeInCommonBaseTree );
			resolvedNode = spcINode();
			resolvedNodeSource = IThreeWayResolvedConflict::kRSNone;
			if ( bothEqual ) {
				reasonForConflictResolution = presentInLatest
					? IThreeWayResolvedConflict::kCRRDeletionOnlyInUserVersion
					: IThreeWayResolvedConflict::kCRRDeletionOnlyInLatestVersion;
				return IThreeWayUnResolvedConflict::kCRResolved;
			} else {
				reasonForConflictResolution = IThreeWayResolvedConflict::kCRRUnResolved;
				return presentInLatest
					? IThreeWayUnResolvedConflict::kCRDeletedInUserEditedInLatest
					: IThreeWayUnResolvedConflict::kCRDeletedInLatestEditedInUser;
			}
		} else {
			resolvedNode = node;
			if ( presentInLatest ) {
				resolvedNodeSource = IThreeWayResolvedConflict::kRSLatestVersion;
				reasonForConflictResolution = IThreeWayResolvedConflict::kCRRPresentOnlyInLatestVersion;
			} else {
				resolvedNodeSource = IThreeWayResolvedConflict::kRSUserVersion;
				reasonForConflictResolution = IThreeWayResolvedConflict::kCRRPresentOnlyInUserVersion;
			}
			return IThreeWayUnResolvedConflict::kCRResolved;
		}
	}

	IThreeWayUnResolvedConflict::eConflictReason TwoWayConflictResolutionStrategyForThreeWayConflictIdentification::handlePresentInBothButDifferent(
		bool doBasicComparison, const ITwoWayUnResolvedConflict::eTwoWayConflictReason reasonsForConflict, const spcINode & latestVersionNode,
		const spcINode & userVersionNode, spcINode & resolvedNode, IThreeWayResolvedConflict::eConflictResolutionReason & reasonForConflictResolution,
		IThreeWayResolvedConflict::eResolvedSource & resolvedNodeSource ) const
	{
		spcINode baseVersionNode;
		bool baseVersionNodePresent = mComparisonEngine->IsEquivalentNodePresentInOtherTree( latestVersionNode, mCommonBaseMetadata, baseVersionNode );

		if ( baseVersionNodePresent ) {
			// compare base with latest
			bool equalToLatest = compareValues( baseVersionNode, latestVersionNode );
			if ( equalToLatest ) {
				resolvedNode = userVersionNode;
				resolvedNodeSource = IThreeWayResolvedConflict::kRSUserVersion;
				reasonForConflictResolution = IThreeWayResolvedConflict::kCRRBaseAndLatestVersionEqual;
				return IThreeWayUnResolvedConflict::kCRResolved;
			} else {
				// compare with the user
				bool equalToUser = compareValues( baseVersionNode, userVersionNode );
				if ( equalToUser ) {
					resolvedNode = latestVersionNode;
					resolvedNodeSource = IThreeWayResolvedConflict::kRSLatestVersion;
					reasonForConflictResolution = IThreeWayResolvedConflict::kCRRBaseAndUserVersionEqual;
					return IThreeWayUnResolvedConflict::kCRResolved;
				}
			}
		} else { // scenarios where base node is not present and one of the entries is empty
			if ( latestVersionNode->GetNodeType() == INode::kNTArray ) {
				if ( latestVersionNode->ConvertToArrayNode()->ChildCount() == 0 ) {
					resolvedNode = userVersionNode;
					resolvedNodeSource = IThreeWayResolvedConflict::kRSUserVersion;
					reasonForConflictResolution = IThreeWayResolvedConflict::kCRRPresentOnlyInUserVersion;
					return IThreeWayUnResolvedConflict::kCRResolved;
				} else if ( userVersionNode->ConvertToArrayNode()->ChildCount() == 0 ) {
					resolvedNode = latestVersionNode;
					resolvedNodeSource = IThreeWayResolvedConflict::kRSLatestVersion;
					reasonForConflictResolution = IThreeWayResolvedConflict::kCRRPresentOnlyInLatestVersion;
					return IThreeWayUnResolvedConflict::kCRResolved;
				}
			}
		}

		// no such node in the base dom or base not equal to either latest or user
		resolvedNode = spcINode();
		resolvedNodeSource = IThreeWayResolvedConflict::kRSNone;
		reasonForConflictResolution = IThreeWayResolvedConflict::kCRRUnResolved;

		if ( ( reasonsForConflict & ITwoWayUnResolvedConflict::kTWCRTypesMismatch ) || 
			(reasonsForConflict & ITwoWayUnResolvedConflict::kTWCRArrayTypesMismatch) ||
			(reasonsForConflict & ITwoWayUnResolvedConflict::kTWCRArrayElementTypesMismatch) ) {
			return IThreeWayUnResolvedConflict::kCRTypesMismatch;
		} else {
			return IThreeWayUnResolvedConflict::kCRValuesMismatch;
		}
	}

	IThreeWayUnResolvedConflict::eConflictReason TwoWayConflictResolutionStrategyForThreeWayConflictIdentification::handleQualifiersPresentInOneNode(
		bool presentInLatest, const spcINode & node, spcINode & resolvedNode, IThreeWayResolvedConflict::eConflictResolutionReason & reasonForConflictResolution,
		IThreeWayResolvedConflict::eResolvedSource & resolvedNodeSource ) const
	{
		spcINode nodeInCommonBaseTree;
		bool presentInCommonBaseTree = mComparisonEngine->IsEquivalentNodePresentInOtherTree( node, mCommonBaseMetadata, nodeInCommonBaseTree );

		if ( presentInCommonBaseTree && nodeInCommonBaseTree->HasQualifiers() ) {
			bool bothEqual = compareValues( nodeInCommonBaseTree, node, false, true, false );
			resolvedNode = spcINode();
			resolvedNodeSource = IThreeWayResolvedConflict::kRSNone;
			if ( bothEqual ) {
				reasonForConflictResolution = presentInLatest
					? IThreeWayResolvedConflict::kCRRDeletionOfQualifierOnlyInUserVersion
					: IThreeWayResolvedConflict::kCRRDeletionOfQualifierOnlyInLatestVersion;
				return IThreeWayUnResolvedConflict::kCRResolved;
			} else {
				reasonForConflictResolution = IThreeWayResolvedConflict::kCRRUnResolved;
				return IThreeWayUnResolvedConflict::kCRQualifiersMismatch;
			}
		} else {
			resolvedNode = node;
			if ( presentInLatest ) {
				resolvedNodeSource = IThreeWayResolvedConflict::kRSLatestVersion;
				reasonForConflictResolution = IThreeWayResolvedConflict::kCRRQualifierPresentOnlyInLatestVersion;
			} else {
				resolvedNodeSource = IThreeWayResolvedConflict::kRSUserVersion;
				reasonForConflictResolution = IThreeWayResolvedConflict::kCRRQualifierPresentOnlyInUserVersion;
			}
			return IThreeWayUnResolvedConflict::kCRResolved;
		}
	}

	IThreeWayUnResolvedConflict::eConflictReason TwoWayConflictResolutionStrategyForThreeWayConflictIdentification::handleQualifiersPresentInBothButDifferent(
		const spcINode & latestVersionNode, const spcINode & userVersionNode, spcINode & resolvedNode,
		IThreeWayResolvedConflict::eConflictResolutionReason & reasonForConflictResolution, IThreeWayResolvedConflict::eResolvedSource & resolvedNodeSource ) const
	{
		spcINode baseVersionNode;
		bool baseVersionNodePresent = mComparisonEngine->IsEquivalentNodePresentInOtherTree( latestVersionNode, mCommonBaseMetadata, baseVersionNode );

		if ( baseVersionNodePresent && baseVersionNode->HasQualifiers() ) {
			// compare base with latest
			bool equalToLatest = compareValues( baseVersionNode, latestVersionNode, false, true, false );
			if ( equalToLatest ) {
				resolvedNode = userVersionNode;
				resolvedNodeSource = IThreeWayResolvedConflict::kRSUserVersion;
				reasonForConflictResolution = IThreeWayResolvedConflict::kCRRBaseAndLatestVersionEqual;
				return IThreeWayUnResolvedConflict::kCRResolved;
			} else {
				// compare with the user
				bool equalToUser = compareValues( baseVersionNode, userVersionNode );
				if ( equalToUser ) {
					resolvedNode = latestVersionNode;
					resolvedNodeSource = IThreeWayResolvedConflict::kRSLatestVersion;
					reasonForConflictResolution = IThreeWayResolvedConflict::kCRRBaseAndUserVersionEqual;
					return IThreeWayUnResolvedConflict::kCRResolved;
				} else {
					resolvedNode = spcINode();
					resolvedNodeSource = IThreeWayResolvedConflict::kRSNone;
					reasonForConflictResolution = IThreeWayResolvedConflict::kCRRUnResolved;
					return IThreeWayUnResolvedConflict::kCRQualifiersMismatch;
				}
			}
		} else { // no such node in the base dom
			resolvedNode = spcINode();
			resolvedNodeSource = IThreeWayResolvedConflict::kRSNone;
			reasonForConflictResolution = IThreeWayResolvedConflict::kCRRUnResolved;
			return IThreeWayUnResolvedConflict::kCRQualifiersMismatch;
		}
	}

	IThreeWayUnResolvedConflict::eConflictReason TwoWayConflictResolutionStrategyForThreeWayConflictIdentification::handleSimpleIsUriFlagMismatch(
		bool latestNodeIsURI, const spcINode & latestVersionNode, const spcINode & userVersionNode, spcINode & resolvedNode,
		IThreeWayResolvedConflict::eConflictResolutionReason & reasonForConflictResolution, IThreeWayResolvedConflict::eResolvedSource & resolvedNodeSource ) const
	{
		spcINode nodeInBaseTree;
		bool nodeInBaseTreePresent = mComparisonEngine->IsEquivalentNodePresentInOtherTree( latestVersionNode, mCommonBaseMetadata, nodeInBaseTree );

		if ( nodeInBaseTreePresent ) {
			bool baseNodeIsURI = nodeInBaseTree->ConvertToSimpleNode()->IsURIType();
			if ( baseNodeIsURI ) {
				if ( latestNodeIsURI ) {
					resolvedNode = userVersionNode;
					resolvedNodeSource = IThreeWayResolvedConflict::kRSUserVersion;
					reasonForConflictResolution = IThreeWayResolvedConflict::kCRRBaseAndLatestVersionEqual;
					return IThreeWayUnResolvedConflict::kCRResolved;
				} else {
					resolvedNode = latestVersionNode;
					resolvedNodeSource = IThreeWayResolvedConflict::kRSLatestVersion;
					reasonForConflictResolution = IThreeWayResolvedConflict::kCRRBaseAndUserVersionEqual;
					return IThreeWayUnResolvedConflict::kCRResolved;
				}
			} else {
				if ( !latestNodeIsURI ) {
					resolvedNode = userVersionNode;
					resolvedNodeSource = IThreeWayResolvedConflict::kRSUserVersion;
					reasonForConflictResolution = IThreeWayResolvedConflict::kCRRBaseAndLatestVersionEqual;
					return IThreeWayUnResolvedConflict::kCRResolved;
				} else {
					resolvedNode = latestVersionNode;
					resolvedNodeSource = IThreeWayResolvedConflict::kRSLatestVersion;
					reasonForConflictResolution = IThreeWayResolvedConflict::kCRRBaseAndUserVersionEqual;
					return IThreeWayUnResolvedConflict::kCRResolved;
				}
			}
		} else {
			resolvedNode = spcINode();
			resolvedNodeSource = IThreeWayResolvedConflict::kRSNone;
			reasonForConflictResolution = IThreeWayResolvedConflict::kCRRUnResolved;
			return IThreeWayUnResolvedConflict::kCRURIMismatch;
		}

	}

	bool APICALL TwoWayConflictResolutionStrategyForThreeWayConflictIdentification::InterestedInResolvingConflict( const spcIUTF8String & nameSpace,
		const spcIUTF8String & name, INode::eNodeType nodeType, ITwoWayUnResolvedConflict::eTwoWayConflictReason reason, bool alreadyResolved ) const
	{
		if ( alreadyResolved )
			return false;
		return true;
	}

	IThreeWayUnResolvedConflict::eConflictReason APICALL TwoWayConflictResolutionStrategyForThreeWayConflictIdentification::ResolveConflict(
		ITwoWayUnResolvedConflict::eTwoWayConflictReason reasonsForConflict, const spcINode & baseNode, const spcINode & currentNode, spcINode & resolvedNode,
		IThreeWayResolvedConflict::eConflictResolutionReason & reasonForConflictResolution, IThreeWayResolvedConflict::eResolvedSource & resolvedNodeSource ) const
	{

		for ( int i = 0, flag = 1; i < 32; i++, flag *= 2 ) {
			if ( ( flag & reasonsForConflict ) != 0 ) {
				switch ( static_cast< ITwoWayUnResolvedConflict::eTwoWayConflictReason > ( flag ) ) {
				case ITwoWayUnResolvedConflict::kTWCROnlyPresentInBase:
					return handleOnlyPresentInOneMetadataTree( true, baseNode, resolvedNode, reasonForConflictResolution, resolvedNodeSource );
					break;

				case ITwoWayUnResolvedConflict::kTWCROnlyPresentInLatest:
					return handleOnlyPresentInOneMetadataTree( false, currentNode, resolvedNode, reasonForConflictResolution, resolvedNodeSource );
					break;

				case ITwoWayUnResolvedConflict::kTWCRTypesMismatch:
					return handlePresentInBothButDifferent( true, reasonsForConflict, baseNode, currentNode, resolvedNode,
						reasonForConflictResolution, resolvedNodeSource );
					break;

				case ITwoWayUnResolvedConflict::kTWCRValuesMismatch:
					return handlePresentInBothButDifferent( false, reasonsForConflict, baseNode, currentNode, resolvedNode,
						reasonForConflictResolution, resolvedNodeSource );
					break;

				case ITwoWayUnResolvedConflict::kTWCQualifiersValueMismatch:
					return handleQualifiersPresentInBothButDifferent( baseNode, currentNode, resolvedNode, reasonForConflictResolution, resolvedNodeSource );
					break;

				case ITwoWayUnResolvedConflict::kTWCRQualifiersOnlyPresentInBase:
					return handleQualifiersPresentInOneNode( true, baseNode, resolvedNode, reasonForConflictResolution, resolvedNodeSource );
					break;

				case ITwoWayUnResolvedConflict::kTWCRQualifiersOnlyPresentInLatest:
					return handleQualifiersPresentInOneNode( false, currentNode, resolvedNode, reasonForConflictResolution, resolvedNodeSource );
					break;

				case ITwoWayUnResolvedConflict::kTWCRSimpleNodeIsURIOnlyInBase:
					return handleSimpleIsUriFlagMismatch( true, baseNode, currentNode, resolvedNode, reasonForConflictResolution, resolvedNodeSource );
					break;

				case ITwoWayUnResolvedConflict::kTWCRSimpleNodeIsURIOnlyInLatest:
					return handleSimpleIsUriFlagMismatch( false, baseNode, currentNode, resolvedNode, reasonForConflictResolution, resolvedNodeSource );
					break;

				case ITwoWayUnResolvedConflict::kTWCRArrayTypesMismatch:
				case ITwoWayUnResolvedConflict::kTWCRArraySizeMismatch:
				case ITwoWayUnResolvedConflict::kTWCRArrayElementTypesMismatch:
					return handlePresentInBothButDifferent( true, reasonsForConflict, baseNode, currentNode, resolvedNode, reasonForConflictResolution,
						resolvedNodeSource );
					break;

				case ITwoWayUnResolvedConflict::kTWCRAboutURIPresentOnlyInBase:
				case ITwoWayUnResolvedConflict::kTWCRAboutURIPresentOnlyInLatest:
				case ITwoWayUnResolvedConflict::kTWCRAboutURIValueMismatch:
					//TODO: HandleMetadataAboutURIFlagMismatch();
				case ITwoWayUnResolvedConflict::kTWCRStructureSizeMismatch:
				default:
					NOTIFY_ERROR( IError::kEDGeneral, kGECLogicalError, "Unhandled Situation Occured", IError::kESOperationFatal, false, false );
					break;
				}
			}
		}
		return IThreeWayUnResolvedConflict::kCRMaxValue;
	}

}
