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
	#include "XMPCompareAndMerge/ImplHeaders/TwoWayConflictIdentificationEngineImpl.h"
#undef IMPLEMENTATION_HEADERS_CAN_BE_INCLUDED

#include "XMPCompareAndMerge/Interfaces/ITwoWayComparisonEngine.h"
#include "XMPCompareAndMerge/Interfaces/ITwoWayBasicComparisonResult.h"
#include "XMPCompareAndMerge/Interfaces/ITwoWayValueComparisonResult.h"

#include "XMPCommon/Interfaces/IError_I.h"
#include "XMPCommon/Interfaces/IUTF8String.h"
#include "XMPCommon/Utilities/TSmartPointers_I.h"

#include "XMPCore/Interfaces/IArrayNode.h"
#include "XMPCore/Interfaces/ISimpleNode.h"
#include "XMPCore/Interfaces/IMetadata.h"
#include "XMPCore/Interfaces/INodeIterator.h"
#include "XMPCommon/Utilities/UTF8String.h"

#include <algorithm>

namespace AdobeXMPCompareAndMerge_Int {

	TwoWayConflictIdentificationEngineImpl::TwoWayConflictIdentificationEngineImpl()
		: mComparisonEngine( ITwoWayComparisonEngine::CreateTwoWayComparisonEngine() ) { }

	void TwoWayConflictIdentificationEngineImpl::identifyConflicts( const spcIArrayNode & baseArrayNode, const spcIArrayNode & latestArrayNode, ITwoWayUnResolvedConflict::eTwoWayConflictReason flagsToBeAppended, spITwoWayUnResolvedConflicts & conflictList ) {
		// perform basic comparison first
		spITwoWayBasicComparisonResult basicComparisonResult = mComparisonEngine->CompareBasicProperties( baseArrayNode, latestArrayNode );
		if ( basicComparisonResult->IsEqual() ) {
			// no difference in basic properties so perform Value comparison
			spITwoWayValueComparisonResult valueComparisonResult = mComparisonEngine->CompareValues( baseArrayNode, latestArrayNode );

			if ( valueComparisonResult->IsExactMatch() ) {
				// perform qualifiers identify conflict
				if ( flagsToBeAppended == 0 && baseArrayNode->HasQualifiers() ) {
					auto it = baseArrayNode->QualifiersIterator();
					identifyConflicts( it, latestArrayNode, conflictList );
				}
				if ( flagsToBeAppended != 0 ) {
					conflictList->push_back( ITwoWayUnResolvedConflict::CreateTwoWayUnResolvedConflict( flagsToBeAppended, baseArrayNode, latestArrayNode ) );
				}
			} else {
				uint32 flags = ITwoWayUnResolvedConflict::kTWCRValuesMismatch;
				flags |= flagsToBeAppended;

				// do qualifiers comparison also
				if ( flagsToBeAppended == 0 && baseArrayNode->HasQualifiers() ) {
					spITwoWayValueComparisonResult qualifiersComparisonResult = mComparisonEngine->CompareQualifiers( baseArrayNode, latestArrayNode );
					if ( !qualifiersComparisonResult->IsExactMatch() )
						flags |= ITwoWayUnResolvedConflict::kTWCQualifiersValueMismatch;
				}
				conflictList->push_back( ITwoWayUnResolvedConflict::CreateTwoWayUnResolvedConflict(
					static_cast< ITwoWayUnResolvedConflict::eTwoWayConflictReason >( flags ), baseArrayNode, latestArrayNode ) );
			}
		} else {
			basicComparisonResult->AddConflictType( flagsToBeAppended );
			conflictList->push_back( ITwoWayUnResolvedConflict::CreateTwoWayUnResolvedConflict(
				basicComparisonResult->GetConflictReason(), baseArrayNode, latestArrayNode ) );
		}
	}

	void TwoWayConflictIdentificationEngineImpl::identifyConflicts( const spcISimpleNode & baseSimpleNode, const spcISimpleNode & latestSimpleNode, ITwoWayUnResolvedConflict::eTwoWayConflictReason flagsToBeAppended, spITwoWayUnResolvedConflicts & conflictList ) {
		// perform basic comparison first
		spITwoWayBasicComparisonResult basicComparisonResult = mComparisonEngine->CompareBasicProperties( baseSimpleNode, latestSimpleNode );
		if ( basicComparisonResult->IsEqual() ) {
			// no difference in basic properties so perform Value comparison
			spITwoWayValueComparisonResult valueComparisonResult = mComparisonEngine->CompareValues( baseSimpleNode, latestSimpleNode );

			if ( valueComparisonResult->IsExactMatch() ) {
				// perform qualifiers identify conflict
				if (flagsToBeAppended == 0 && baseSimpleNode->HasQualifiers() ) {
					auto it = baseSimpleNode->QualifiersIterator();
					identifyConflicts( it, latestSimpleNode, conflictList );
				}
				if ( flagsToBeAppended != 0 ) {
					conflictList->push_back( ITwoWayUnResolvedConflict::CreateTwoWayUnResolvedConflict( flagsToBeAppended, baseSimpleNode, latestSimpleNode ) );
				}
			} else {
				uint32 flags = ITwoWayUnResolvedConflict::kTWCRValuesMismatch;
				flags |= flagsToBeAppended;

				// do qualifiers comparison also
				if (flagsToBeAppended == 0 && baseSimpleNode->HasQualifiers() ) {
					spITwoWayValueComparisonResult qualifiersComparisonResult = mComparisonEngine->CompareQualifiers( baseSimpleNode, latestSimpleNode );
					if ( !qualifiersComparisonResult->IsExactMatch() )
						flags |= ITwoWayUnResolvedConflict::kTWCQualifiersValueMismatch;
				}
				conflictList->push_back( ITwoWayUnResolvedConflict::CreateTwoWayUnResolvedConflict(
					static_cast< ITwoWayUnResolvedConflict::eTwoWayConflictReason >( flags ), baseSimpleNode, latestSimpleNode ) );
			}
		} else {
			basicComparisonResult->AddConflictType( flagsToBeAppended );
			conflictList->push_back( ITwoWayUnResolvedConflict::CreateTwoWayUnResolvedConflict(
				basicComparisonResult->GetConflictReason(), baseSimpleNode, latestSimpleNode ) );
		}
	}

	void TwoWayConflictIdentificationEngineImpl::identifyConflicts( const spcIStructureNode & baseStructNode, const spcIStructureNode & latestStructNode, ITwoWayUnResolvedConflict::eTwoWayConflictReason flagsToBeAppended, spITwoWayUnResolvedConflicts & conflictList ) {
		// do qualifiers identify conflict
		auto it = baseStructNode->QualifiersIterator();
		identifyConflicts( it, latestStructNode, conflictList );
		it = baseStructNode->Iterator();
		identifyConflictsForStructNode( it, latestStructNode, conflictList );
		if ( flagsToBeAppended != 0 ) {
			conflictList->push_back( ITwoWayUnResolvedConflict::CreateTwoWayUnResolvedConflict( flagsToBeAppended, baseStructNode, latestStructNode ) );
		}
	}

	void TwoWayConflictIdentificationEngineImpl::identifyConflicts( spcINodeIterator & baseIterator, const spcINode & latestNode, spITwoWayUnResolvedConflicts & conflictList ) {
		//typedef std::pair< spcIUTF8String, spcIUTF8String > NameSpaceNamePair;
		typedef std::pair< UTF8String, UTF8String > NameSpaceNamePair;
		typedef std::vector< NameSpaceNamePair, TAllocator< NameSpaceNamePair > > NameSpaceNamePairList;

		NameSpaceNamePairList nameSpaceNamePairList;

		while ( baseIterator ) {
			spcINode baseChildNode = baseIterator->GetNode();
			spcINode latestChildNode;

			if ( mComparisonEngine->IsEquivalentQualifierPresentInOtherTree( baseChildNode, latestNode, latestChildNode ) ) {
				nameSpaceNamePairList.push_back( std::make_pair( baseChildNode->GetNameSpace()->c_str(), baseChildNode->GetName()->c_str() ) );
				spITwoWayUnResolvedConflicts childConflicts = IdentifyConflicts( baseChildNode, latestChildNode );

				// append all child conflicts to the top conflict list
				for ( sizet i = 0, childConflictsCount = childConflicts->size(); i < childConflictsCount; ++i ) {
					conflictList->push_back( ( *childConflicts )[ i ] );
				}
			} else {
				if ( baseChildNode->GetNodeType() == INode::kNTStructure ) {
					spcIStructureNode baseChildStructNode = baseChildNode->ConvertToStructureNode();
					spcINodeIterator it( baseChildStructNode->Iterator() );
					while ( it ) {
						conflictList->push_back( ITwoWayUnResolvedConflict::CreateTwoWayUnResolvedConflict(
							ITwoWayUnResolvedConflict::kTWCROnlyPresentInBase, it->GetNode(), latestChildNode ) );
						it = it->Next();
					}
				} else {
					conflictList->push_back( ITwoWayUnResolvedConflict::CreateTwoWayUnResolvedConflict(
						ITwoWayUnResolvedConflict::kTWCROnlyPresentInBase, baseChildNode, latestChildNode ) );
				}
			}
			baseIterator = baseIterator->Next();
		}

		auto it = latestNode->QualifiersIterator();

		while ( it ) {
			// if nameSpace name pair is not present in the list then add to conflict list
			spcINode node = it->GetNode();
			NameSpaceNamePair pair = std::make_pair( node->GetNameSpace()->c_str(), node->GetName()->c_str() );

			NameSpaceNamePairList::iterator endIterator = nameSpaceNamePairList.end();
			NameSpaceNamePairList::iterator foundIterator = std::find( nameSpaceNamePairList.begin(), endIterator, pair );
			if ( foundIterator == endIterator ) {
				if ( node->GetNodeType() == INode::kNTStructure ) {
					spcIStructureNode childStructNode = node->ConvertToStructureNode();
					spcINodeIterator iter( childStructNode->Iterator() );
					while ( iter ) {
						conflictList->push_back( ITwoWayUnResolvedConflict::CreateTwoWayUnResolvedConflict(
							ITwoWayUnResolvedConflict::kTWCROnlyPresentInLatest, spcINode(), iter->GetNode() ) );
						iter = iter->Next();
					}
				} else {
					conflictList->push_back( ITwoWayUnResolvedConflict::CreateTwoWayUnResolvedConflict(
						ITwoWayUnResolvedConflict::kTWCROnlyPresentInLatest, spcINode(), node ) );
				}
			}
			it = it->Next();
		}
	}

	void TwoWayConflictIdentificationEngineImpl::identifyConflictsForStructNode( spcINodeIterator & baseIterator, const spcIStructureNode & latestStructNode, spITwoWayUnResolvedConflicts & conflictList ) {
		//typedef std::pair< spcIUTF8String, spcIUTF8String > NameSpaceNamePair;
		typedef std::pair< UTF8String, UTF8String > NameSpaceNamePair;
		typedef std::vector< NameSpaceNamePair, TAllocator< NameSpaceNamePair > > NameSpaceNamePairList;

		NameSpaceNamePairList nameSpaceNamePairList;

		while ( baseIterator ) {
			spcINode baseChildNode = baseIterator->GetNode();
			spcINode latestChildNode;

			if ( mComparisonEngine->IsEquivalentNodePresentInOtherTree( baseChildNode, static_cast<const spcICompositeNode>(latestStructNode), latestChildNode ) ) {
				//nameSpaceNamePairList.push_back( std::make_pair( baseChildNode->GetNameSpace(), baseChildNode->GetName() ) );
				nameSpaceNamePairList.push_back(std::make_pair(baseChildNode->GetNameSpace()->c_str(), baseChildNode->GetName()->c_str()));
				spITwoWayUnResolvedConflicts childConflicts = IdentifyConflicts( baseChildNode, latestChildNode );

				// append all child conflicts to the top conflict list
				for ( sizet i = 0, childConflictsCount = childConflicts->size(); i < childConflictsCount; ++i ) {
					conflictList->push_back( ( *childConflicts )[ i ] );
				}
			} else {
				if ( baseChildNode->GetNodeType() == INode::kNTStructure ) {
					spcIStructureNode baseChildStructNode = baseChildNode->ConvertToStructureNode();
					spcINodeIterator it( baseChildStructNode->Iterator() );
					while( it ) {
						conflictList->push_back( ITwoWayUnResolvedConflict::CreateTwoWayUnResolvedConflict(
							ITwoWayUnResolvedConflict::kTWCROnlyPresentInBase, it->GetNode(), latestChildNode ) );
						it = it->Next();
					}
				} else {
					conflictList->push_back( ITwoWayUnResolvedConflict::CreateTwoWayUnResolvedConflict(
						ITwoWayUnResolvedConflict::kTWCROnlyPresentInBase, baseChildNode, latestChildNode ) );
				}
			}
			baseIterator = baseIterator->Next();
		}

		auto it = latestStructNode->Iterator();

		while ( it ) {
			// if nameSpace name pair is not present in the list then add to conflict list
			spcINode node = it->GetNode();
			//NameSpaceNamePair pair = std::make_pair( node->GetNameSpace(), node->GetName() );
			NameSpaceNamePair pair = std::make_pair(node->GetNameSpace()->c_str(), node->GetName()->c_str());
			NameSpaceNamePairList::iterator endIterator = nameSpaceNamePairList.end();
			NameSpaceNamePairList::iterator foundIterator = std::find( nameSpaceNamePairList.begin(), endIterator, pair );
			if ( foundIterator == endIterator ) {
				if ( node->GetNodeType() == INode::kNTStructure ) {
					spcIStructureNode childStructNode = node->ConvertToStructureNode();
					spcINodeIterator iter( childStructNode->Iterator() );
					while ( iter) {
						/*conflictList->push_back( ITwoWayUnResolvedConflict::CreateTwoWayUnResolvedConflict(
							ITwoWayUnResolvedConflict::kTWCRAboutURIPresentOnlyInLatest, spcINode(), iter->GetNode() ) );*/
						conflictList->push_back(ITwoWayUnResolvedConflict::CreateTwoWayUnResolvedConflict(
							ITwoWayUnResolvedConflict::kTWCROnlyPresentInLatest, spcINode(), iter->GetNode()));
						iter = iter->Next();
					}
				} else {
					/*conflictList->push_back( ITwoWayUnResolvedConflict::CreateTwoWayUnResolvedConflict(
						ITwoWayUnResolvedConflict::kTWCRAboutURIPresentOnlyInLatest, spcINode(), node ) );*/
					conflictList->push_back(ITwoWayUnResolvedConflict::CreateTwoWayUnResolvedConflict(
						ITwoWayUnResolvedConflict::kTWCROnlyPresentInLatest, spcINode(), node));
				}
			}
			it = it->Next();
		}
	}

	AdobeXMPCompareAndMerge_Int::spITwoWayUnResolvedConflicts TwoWayConflictIdentificationEngineImpl::IdentifyConflicts( const spcIMetadata & baseMetadata, const spcIMetadata & latestMetadata ) {
		spcINode baseNode = baseMetadata;
		spcINode latestNode = latestMetadata;

		spITwoWayUnResolvedConflicts conflictList = IdentifyConflicts( baseNode, latestNode );

		spcIUTF8String baseAboutURI = baseMetadata->GetAboutURI();
		spcIUTF8String latestAboutURI = latestMetadata->GetAboutURI();

		if ( baseAboutURI && latestAboutURI ) {
			if ( baseAboutURI->compare( latestAboutURI ) != 0 ) {
				conflictList->push_back( ITwoWayUnResolvedConflict::CreateTwoWayUnResolvedConflict(
					ITwoWayUnResolvedConflict::kTWCRAboutURIValueMismatch, baseMetadata, latestMetadata ) );
			}
		} else if ( baseAboutURI ) {
			conflictList->push_back( ITwoWayUnResolvedConflict::CreateTwoWayUnResolvedConflict(
				ITwoWayUnResolvedConflict::kTWCRAboutURIPresentOnlyInBase, baseMetadata, latestMetadata ) );
		} else if ( latestAboutURI ) {
			conflictList->push_back( ITwoWayUnResolvedConflict::CreateTwoWayUnResolvedConflict(
				ITwoWayUnResolvedConflict::kTWCRAboutURIPresentOnlyInLatest, baseMetadata, latestMetadata ) );
		}

		return conflictList;
	}

	AdobeXMPCompareAndMerge_Int::spITwoWayUnResolvedConflicts TwoWayConflictIdentificationEngineImpl::IdentifyConflicts( const spcINode & baseNode, const spcINode & latestNode ) {
		// Perform Basic Comparison of the nodes
		spITwoWayUnResolvedConflicts conflictList = ITwoWayUnResolvedConflict::CreateTwoWayUnResolvedConflicts();

		spITwoWayBasicComparisonResult basicComparisonResult = mComparisonEngine->CompareBasicProperties( baseNode, latestNode );

		bool stopAfterBasicComparison = false;
		uint32 flagsToBeCarriedForward = 0;

		for ( int i = 0, flag = 1; i < 32; i++, flag *= 2 ) {
			ITwoWayUnResolvedConflict::eTwoWayConflictReason conflictType = static_cast< ITwoWayUnResolvedConflict::eTwoWayConflictReason >( flag );
			if ( basicComparisonResult->IsFollowingConflictPresent( conflictType ) ) {
				switch ( conflictType ) {
				case ITwoWayUnResolvedConflict::kTWCROnlyPresentInBase:
				case ITwoWayUnResolvedConflict::kTWCROnlyPresentInLatest:
				case ITwoWayUnResolvedConflict::kTWCRTypesMismatch:
					stopAfterBasicComparison = true;
					break;

				case ITwoWayUnResolvedConflict::kTWCRQualifiersOnlyPresentInBase:
				case ITwoWayUnResolvedConflict::kTWCRQualifiersOnlyPresentInLatest:
					flagsToBeCarriedForward |= conflictType;
					break;

				default:
					break;
				}
			}
		}

		if ( stopAfterBasicComparison ) {
			conflictList->push_back( ITwoWayUnResolvedConflict::CreateTwoWayUnResolvedConflict( basicComparisonResult->GetConflictReason(), baseNode, latestNode ) );
			return conflictList;
		}

		switch ( baseNode->GetNodeType() ) {
		case INode::kNTSimple:
			identifyConflicts( baseNode->ConvertToSimpleNode(), latestNode->ConvertToSimpleNode(),
				static_cast< ITwoWayUnResolvedConflict::eTwoWayConflictReason >( flagsToBeCarriedForward ), conflictList );
			break;

		case INode::kNTArray:
			identifyConflicts( baseNode->ConvertToArrayNode(), latestNode->ConvertToArrayNode(),
				static_cast< ITwoWayUnResolvedConflict::eTwoWayConflictReason >( flagsToBeCarriedForward ), conflictList );
			break;

		case INode::kNTStructure:
			identifyConflicts( baseNode->ConvertToStructureNode(), latestNode->ConvertToStructureNode(),
				static_cast< ITwoWayUnResolvedConflict::eTwoWayConflictReason >( flagsToBeCarriedForward ), conflictList );
			break;

		default:
			break;
		}

		return conflictList;
	}

	spITwoWayConflictIdentificationEngine ITwoWayConflictIdentificationEngine::CreateTwoWayConflictIdentificationEngine() {
		return MakeUncheckedSharedPointer( new TwoWayConflictIdentificationEngineImpl(), __FILE__, __LINE__, true );
	}

}
