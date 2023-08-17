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
	#include "XMPCompareAndMerge/ImplHeaders/TwoWayComparisonEngineImpl.h"
#undef IMPLEMENTATION_HEADERS_CAN_BE_INCLUDED

#include "XMPCompareAndMerge/Interfaces/ITwoWayBasicComparisonResult.h"
#include "XMPCompareAndMerge/Interfaces/ITwoWayValueComparisonResult.h"

#include "XMPCommon/Interfaces/IError_I.h"
#include "XMPCommon/XMPCommonErrorCodes.h"
#include "XMPCommon/Interfaces/IUTF8String.h"
#include "XMPCommon/Utilities/TSmartPointers_I.h"

#include "XMPCore/Interfaces/IPathSegment.h"
#include "XMPCore/Interfaces/IPath.h"
#include "XMPCore/Interfaces/ISimpleNode.h"
#include "XMPCore/Interfaces/IArrayNode.h"
#include "XMPCore/Interfaces/IMetadata.h"
#include "XMPCore/Interfaces/INodeIterator.h"
#include "XMPCommon/Utilities/UTF8String.h"
#include <algorithm>

namespace AdobeXMPCompareAndMerge_Int {

	class ResetingValueBackToOld {
	public:
		ResetingValueBackToOld( bool & flag, bool newValue )
			: mFlagReference( flag )
			, mOriginalFlagValue( flag ) {
			flag = newValue;
		}

		void Reset() {
			mFlagReference = mOriginalFlagValue;
		}

		~ResetingValueBackToOld() {
			Reset();
		}

	private:
		ResetingValueBackToOld & operator = ( const ResetingValueBackToOld & rhs );
		bool &	mFlagReference;
		bool	mOriginalFlagValue;
	};
	
	static spcIPath CreatePathFromNodeToParent( const spcINode & node ) {
		spcIPathSegment segment;
		spcINode parent = node->GetParent();
		spcIUTF8String nameSpace = node->GetNameSpace();
		spcIUTF8String name = node->GetName();

		if ( parent ) {
			if ( node->IsQualifierNode() ) {
				segment = IPathSegment::CreateQualifierPathSegment( nameSpace->c_str(), nameSpace->size(), name->c_str(), name->size() );
			} else if ( node->IsArrayItem() ) {
				segment = IPathSegment::CreateArrayIndexPathSegment( nameSpace->c_str(), nameSpace->size(), node->GetIndex() );
			} else {
				segment = IPathSegment::CreatePropertyPathSegment( nameSpace->c_str(), nameSpace->size(), name->c_str(), name->size() );
			}
		}
		spIPath path = IPath::CreatePath();
		if ( segment )
			path->AppendPathSegment( segment );
		return path;
	}

	static sizet GetChildCount( const spcINode & node ) {
		sizet childCount = 0;
		switch ( node->GetNodeType() ) {
		case INode::kNTArray:
			childCount = node->ConvertToArrayNode()->ChildCount();
			break;

		case INode::kNTStructure:
			childCount = node->ConvertToStructureNode()->ChildCount();
			break;
			
		default:
			childCount = 0;
		}
		return childCount;
	}

	TwoWayComparisonEngineImpl::TwoWayComparisonEngineImpl()
		: mStopOnFirstDifferentValueEncountered( true )
		, mStopOnFirstUniqueNodeEncountered( false )
		, mStopCreatingAllCombinationsOfCompareValuesForAnArrayElementAfterExactMatchEncountered( true ) { }

	spITwoWayBasicComparisonResult APICALL TwoWayComparisonEngineImpl::CompareBasicProperties( const spcIArrayNode & base, const spcIArrayNode & latest ) const {
		sizet baseNodeChildCount = base->ChildCount(), latestNodeChildCount = latest->ChildCount();
		return ITwoWayBasicComparisonResult::CreateArrayNodeMismatchTwoWayBasicComparisonResult( baseNodeChildCount, latestNodeChildCount,
			base->GetArrayForm(), latest->GetArrayForm(), base->GetChildNodeType(), latest->GetChildNodeType() );
	}

	spITwoWayBasicComparisonResult APICALL TwoWayComparisonEngineImpl::CompareBasicProperties( const spcINode & base, const spcINode & latest ) const {
		INode::eNodeType baseNodeType = base->GetNodeType(), latestNodeType = latest->GetNodeType();
		sizet baseNodeChildCount = GetChildCount( base );
		sizet latestNodeChildCount = GetChildCount( latest );
		spITwoWayBasicComparisonResult basicComparisonResult = ITwoWayBasicComparisonResult::CreateNodeTypesMismatchTwoWayBasicComparisonResult(
			baseNodeType, latestNodeType, baseNodeChildCount, latestNodeChildCount );

		if ( basicComparisonResult->IsEqual() ) {

			bool baseHasQualifiers = base->HasQualifiers();
			bool latestHasQualifers = latest->HasQualifiers();

			if ( baseHasQualifiers && !latestHasQualifers ) {
				basicComparisonResult->AddConflictType( ITwoWayUnResolvedConflict::kTWCRQualifiersOnlyPresentInBase );
			} else if ( !baseHasQualifiers && latestHasQualifers ) {
				basicComparisonResult->AddConflictType( ITwoWayUnResolvedConflict::kTWCRQualifiersOnlyPresentInLatest );
			}

		}
		return basicComparisonResult;
	}

	spITwoWayBasicComparisonResult APICALL TwoWayComparisonEngineImpl::CompareBasicProperties( const spcISimpleNode & base, const spcISimpleNode & latest ) const {
		return ITwoWayBasicComparisonResult::CreateSimpleNodeMismatchTwoWayBasicComparisonResult( base->IsURIType(), latest->IsURIType() );
	}

	spITwoWayBasicComparisonResult APICALL TwoWayComparisonEngineImpl::CompareBasicProperties( const spcIStructureNode & base, const spcIStructureNode & latest ) const {
		sizet baseNodeChildCount = base->ChildCount(), latestNodeChildCount = latest->ChildCount();
		return ITwoWayBasicComparisonResult::CreateStructNodeMismatchTwoWayBasicComparisonResult( baseNodeChildCount, latestNodeChildCount );
	}

	spITwoWayValueComparisonResult APICALL TwoWayComparisonEngineImpl::CompareValues( const spcINode & base, const spcINode & latest ) const {
		switch ( base->GetNodeType() ) {
		case INode::kNTSimple:
			return compareValues( base->ConvertToSimpleNode(), latest->ConvertToSimpleNode() );
			break;

		case INode::kNTStructure:
			return compareValues( base->ConvertToStructureNode(), latest->ConvertToStructureNode() );
			break;

		case INode::kNTArray:
			return compareValues( base->ConvertToArrayNode(), latest->ConvertToArrayNode() );
			break;

		default:
			NOTIFY_ERROR( IError::kEDGeneral, kGECLogicalError, "Unhandled Situtation Occured", IError::kESOperationFatal, false, false );
			break;
		}

		return ITwoWayValueComparisonResult::CreateExactlyMisMatchedResult();
	}

	spITwoWayValueComparisonResult APICALL TwoWayComparisonEngineImpl::CompareQualifiers( const spcINode & base, const spcINode & latest ) const {

		//typedef std::pair< spcIUTF8String, spcIUTF8String > NameSpaceNamePair;
		typedef std::pair< UTF8String, UTF8String > NameSpaceNamePair;
		typedef std::vector< NameSpaceNamePair, TAllocator< NameSpaceNamePair > > NameSpaceNamePairList;
		NameSpaceNamePairList nameSpaceNamePairList;
		spITwoWayValueComparisonResult result = ITwoWayValueComparisonResult::CreateExactlyMatchedResult();

		if ( mStopOnFirstUniqueNodeEncountered ) {
			spITwoWayBasicComparisonResult basicComparisonResult = ITwoWayBasicComparisonResult::CreateStructNodeMismatchTwoWayBasicComparisonResult(
				base->QualifiersCount(), latest->QualifiersCount() );
			if ( !basicComparisonResult->IsEqual() ) {
				result->SetMatchedResultOfChild( ITwoWayValueComparisonResult::CreateExactlyMisMatchedResult() );
				return result;
			}
		}

		spcINodeIterator it = base->QualifiersIterator();
		while ( it ) {
			auto baseNode = it->GetNode();
			spcINode latestNode;

			bool nodePresentInLatestTree = IsEquivalentQualifierPresentInOtherTree( baseNode, latest, latestNode );

			if ( nodePresentInLatestTree ) {
				spITwoWayBasicComparisonResult basicComparisonResult = CompareBasicProperties( baseNode, latestNode );
				if ( basicComparisonResult->IsEqual() ) {
					spITwoWayValueComparisonResult childComparisonResult = CompareValues( baseNode, latestNode );
					result->SetMatchedResultOfChild( childComparisonResult );
					if ( mStopOnFirstDifferentValueEncountered && !childComparisonResult->IsExactMatch() ) {
						return result;
					}
					//nameSpaceNamePairList.push_back( std::make_pair( baseNode->GetNameSpace(), latestNode->GetName() ) );
					nameSpaceNamePairList.push_back(std::make_pair(baseNode->GetNameSpace()->c_str(), latestNode->GetName()->c_str()));
				} else if ( mStopOnFirstDifferentValueEncountered ) {
					result->SetMatchedResultOfChild( ITwoWayValueComparisonResult::CreateExactlyMisMatchedResult() );
					return result;
				} else {
					result->SetMatchedResultOfChild( ITwoWayValueComparisonResult::CreateExactlyMatchedResult() );
					//nameSpaceNamePairList.push_back( std::make_pair( baseNode->GetNameSpace(), baseNode->GetName() ) );
					nameSpaceNamePairList.push_back(std::make_pair(baseNode->GetNameSpace()->c_str(), baseNode->GetName()->c_str()));
				}
			} else if ( mStopOnFirstUniqueNodeEncountered ) {
				result->SetMatchedResultOfChild( ITwoWayValueComparisonResult::CreateExactlyMisMatchedResult() );
				return result;
			} else { // Node is not present in the latest tree
				result->SetMatchedResultOfChild( ITwoWayValueComparisonResult::CreateExactlyMisMatchedResult () );
			}

			it = it->Next();
		}

		it = latest->QualifiersIterator();
		while ( it ) {
			// if nameSpace name pair is not present in the list then add to total
			auto node = it->GetNode();
			//NameSpaceNamePair pair = std::make_pair( node->GetNameSpace(), node->GetName() );
			NameSpaceNamePair pair = std::make_pair(node->GetNameSpace()->c_str(), node->GetName()->c_str());
			NameSpaceNamePairList::iterator endIterator = nameSpaceNamePairList.end();
			NameSpaceNamePairList::iterator foundIterator = std::find( nameSpaceNamePairList.begin(), endIterator, pair );
			if ( foundIterator == endIterator ) {
				if ( mStopOnFirstUniqueNodeEncountered ) {
					result->SetMatchedResultOfChild( ITwoWayValueComparisonResult::CreateExactlyMisMatchedResult() );
					return result;
				} else {
					result->SetMatchedResultOfChild( ITwoWayValueComparisonResult::CreateExactlyMisMatchedResult () );
				}
			}
			it = it->Next();
		}
		return result;

	}

	spITwoWayValueComparisonResultMatrix APICALL TwoWayComparisonEngineImpl::CreateAllCombinationsOfCompareValuesForArrayElements( const spcIArrayNode & base, const spcIArrayNode & latest ) const {
		sizet nElements = base->ChildCount();
		spITwoWayValueComparisonResultMatrix matrix = ITwoWayValueComparisonResult::CreateTwoWayValueComparisonResultMatrix();

		if ( nElements != 0 ) {
			matrix->reserve( nElements );
			for ( sizet i = 0; i < nElements; i++ )
				matrix->push_back( ITwoWayValueComparisonResult::CreateTwoWayValueComparisonResults() );

			// compare each element with every other
			std::vector< sizet, TAllocator< sizet > > innerIndices;
			innerIndices.reserve( nElements );
			for ( sizet i = 0; i < nElements; i++ )
				innerIndices.push_back( i );

			for ( sizet i = 0; i < nElements; i++ ) {
				bool exactMatchFound = false;
				spITwoWayValueComparisonResults & currentRow = ( *matrix )[ i ];

				currentRow->reserve( nElements );
				for ( sizet j = 0; j < nElements; j++ ) {
					currentRow->push_back( ITwoWayValueComparisonResult::CreateExactlyMisMatchedResult() );
				}

				auto baseNode = base->GetNodeAtIndex( i + 1 );
				sizet nInnerLoopCount = innerIndices.size();

				for ( sizet j = 0; j < nInnerLoopCount; j++ ) {
					spITwoWayValueComparisonResult matchingResult = CompareValues ( baseNode , latest->GetNodeAtIndex ( innerIndices[j] + 1 ) );
					spITwoWayValueComparisonResult qualifiersComparisonResult = matchingResult;

					if ( matchingResult->IsExactMatch ( ) )
						qualifiersComparisonResult = CompareQualifiers ( baseNode , latest->GetNodeAtIndex ( innerIndices[j] + 1 ) );

					(*currentRow)[innerIndices[j]] = qualifiersComparisonResult;

					if ( qualifiersComparisonResult->IsExactMatch ( ) ) {
						exactMatchFound = true;
						if ( mStopCreatingAllCombinationsOfCompareValuesForAnArrayElementAfterExactMatchEncountered ) {
							innerIndices.erase ( innerIndices.begin ( ) + j );
						}
						break;
					}
				}

				if ( mStopOnFirstDifferentValueEncountered && !exactMatchFound ) {
					return matrix;
				}
			}
		}
		return matrix;

	}

	bool APICALL TwoWayComparisonEngineImpl::IsEquivalentNodePresentInOtherTree( const spcINode & nodeInBaseTree, const spcICompositeNode & otherTreeParentNode,
		spcINode & equivalentNodeInOtherTree ) const
	{
		spcIPath pathOfNodeInBaseTreeFromParent = CreatePathFromNodeToParent( nodeInBaseTree );
		equivalentNodeInOtherTree = otherTreeParentNode->GetNodeAtPath( pathOfNodeInBaseTreeFromParent );
		if ( equivalentNodeInOtherTree )
			return true;
		return false;
	}
	
	//comment Added while executing API Tests.
	bool APICALL TwoWayComparisonEngineImpl::IsEquivalentNodePresentInOtherTree(const spcINode & nodeInBaseTree, const spcIMetadata & otherTreeDOM,
		spcINode & equivalentNodeInOtherTree) const
	{
		spcIPath pathOfNodeFromRoot = nodeInBaseTree->GetPath();
		equivalentNodeInOtherTree = otherTreeDOM->GetNodeAtPath(pathOfNodeFromRoot);
		if (equivalentNodeInOtherTree)
			return true;
		return false;
	}

	bool APICALL TwoWayComparisonEngineImpl::IsEquivalentQualifierPresentInOtherTree( const spcINode & qualifierInBaseTree, const spcINode & otherTreeHostNode, spcINode & equivalentQualifierInOtherTree ) const {
		spcIUTF8String nameSpace = qualifierInBaseTree->GetNameSpace();
		spcIUTF8String name = qualifierInBaseTree->GetName();
		equivalentQualifierInOtherTree = otherTreeHostNode->GetQualifier( nameSpace->c_str(), nameSpace->size(), name->c_str(), name->size() );
		if ( equivalentQualifierInOtherTree )
			return true;
		return false;
	}

	void APICALL TwoWayComparisonEngineImpl::SetStopCreatingAllCombinationsOfCompareValuesForAnArrayElementAfterExactMatchEncountered( bool value ) {
		mStopCreatingAllCombinationsOfCompareValuesForAnArrayElementAfterExactMatchEncountered = value;
	}

	void APICALL TwoWayComparisonEngineImpl::SetStopOnFirstDifferentValueEncountered( bool value ) {
		mStopOnFirstDifferentValueEncountered = value;
	}

	void APICALL TwoWayComparisonEngineImpl::SetStopOnFirstUniqueNodeEncountered( bool value ) {
		mStopOnFirstUniqueNodeEncountered = value;
	}

	spITwoWayValueComparisonResult TwoWayComparisonEngineImpl::compareValues( const spcISimpleNode & base, const spcISimpleNode & latest ) const {
		if ( base->GetValue()->compare( latest->GetValue() ) == 0 )
			return ITwoWayValueComparisonResult::CreateExactlyMatchedResult();
		return ITwoWayValueComparisonResult::CreateExactlyMisMatchedResult();
	}

	spITwoWayValueComparisonResult TwoWayComparisonEngineImpl::compareValues( const spcIArrayNode & base, const spcIArrayNode & latest ) const {
		ResetingValueBackToOld resetter( mStopOnFirstUniqueNodeEncountered, true );

		switch ( base->GetArrayForm() ) {

		case IArrayNode::kAFUnordered:
			return compareUnOrderedArrays( base, latest );
			break;

		case IArrayNode::kAFOrdered:
			return compareOrderedArrays( base, latest );
			break;

		case IArrayNode::kAFAlternative:
			return compareAlternateArrays( base, latest );
			break;

		default:
			NOTIFY_ERROR( IError::kEDGeneral, kGECLogicalError, "Unhandled Situation Occured", IError::kESOperationFatal, false, false );
			break;
		}

		resetter.Reset();

		return ITwoWayValueComparisonResult::CreateExactlyMisMatchedResult();
	}

	spITwoWayValueComparisonResult TwoWayComparisonEngineImpl::compareValues( const spcIStructureNode & base, const spcIStructureNode & latest ) const {
		//typedef std::pair< spcIUTF8String, spcIUTF8String > NameSpaceNamePair;
		typedef std::pair< UTF8String, UTF8String > NameSpaceNamePair;
		typedef std::vector< NameSpaceNamePair, TAllocator< NameSpaceNamePair > > NameSpaceNamePairList;
		NameSpaceNamePairList nameSpaceNamePairList;
		spITwoWayValueComparisonResult result = ITwoWayValueComparisonResult::CreateExactlyMatchedResult();

		if ( mStopOnFirstUniqueNodeEncountered ) {
			spITwoWayBasicComparisonResult basicComparisonResult = ITwoWayBasicComparisonResult::CreateStructNodeMismatchTwoWayBasicComparisonResult(
				base->ChildCount(), latest->ChildCount() );
			if ( !basicComparisonResult->IsEqual() ) {
				result->SetMatchedResultOfChild( ITwoWayValueComparisonResult::CreateExactlyMisMatchedResult() );
				return result;
			}
		}

		spcINodeIterator it = base->Iterator();
		while ( it ) {
			auto baseNode = it->GetNode();
			spcINode latestNode;

			bool nodePresentInLatestTree = IsEquivalentNodePresentInOtherTree( baseNode, static_cast<const spcICompositeNode>(latest), latestNode );

			if ( nodePresentInLatestTree ) {
				spITwoWayBasicComparisonResult basicComparisonResult = CompareBasicProperties( baseNode, latestNode );
				if ( basicComparisonResult->IsEqual() ) {
					bool exactMatchFound = false;

					spITwoWayValueComparisonResult childComparisonResult = CompareValues( baseNode, latestNode );
					if ( childComparisonResult->IsExactMatch ( ) )
					{
						spITwoWayValueComparisonResult qualifiersComparisonResult = CompareQualifiers ( baseNode , latestNode );
						result->SetMatchedResultOfChild ( qualifiersComparisonResult );
						if ( qualifiersComparisonResult->IsExactMatch ( ) )
							exactMatchFound = true;
					}
					else
						result->SetMatchedResultOfChild ( childComparisonResult );
					if ( mStopOnFirstDifferentValueEncountered && !exactMatchFound ) {
						return result;
					}
					//nameSpaceNamePairList.push_back( std::make_pair( baseNode->GetNameSpace(), latestNode->GetName() ) );
					nameSpaceNamePairList.push_back(std::make_pair(baseNode->GetNameSpace()->c_str(), latestNode->GetName()->c_str()));
				} else if ( mStopOnFirstDifferentValueEncountered ) {
					result->SetMatchedResultOfChild( ITwoWayValueComparisonResult::CreateExactlyMisMatchedResult() );
					return result;
				} else {
					result->SetMatchedResultOfChild( ITwoWayValueComparisonResult::CreateExactlyMatchedResult() );
					//nameSpaceNamePairList.push_back( std::make_pair( baseNode->GetNameSpace(), baseNode->GetName() ) );
					nameSpaceNamePairList.push_back(std::make_pair(baseNode->GetNameSpace()->c_str(), baseNode->GetName()->c_str()));
				}
			} else if ( mStopOnFirstUniqueNodeEncountered ) {
				result->SetMatchedResultOfChild( ITwoWayValueComparisonResult::CreateExactlyMisMatchedResult() );
				return result;
			} else { // Node is not present in the latest tree
				result->SetMatchedResultOfChild( ITwoWayValueComparisonResult::CreateExactlyMatchedResult() );
			}

			it = it->Next();
		}

		it = latest->Iterator();
		while ( it ) {
			// if nameSpace name pair is not present in the list then add to total
			auto node = it->GetNode();
			//NameSpaceNamePair pair = std::make_pair( node->GetNameSpace(), node->GetName() );
			NameSpaceNamePair pair = std::make_pair(node->GetNameSpace()->c_str(), node->GetName()->c_str());

			NameSpaceNamePairList::iterator endIterator = nameSpaceNamePairList.end();
			NameSpaceNamePairList::iterator foundIterator = std::find( nameSpaceNamePairList.begin(), endIterator, pair );
			if ( foundIterator == endIterator ) {
				if ( mStopOnFirstUniqueNodeEncountered ) {
					result->SetMatchedResultOfChild( ITwoWayValueComparisonResult::CreateExactlyMisMatchedResult() );
					return result;
				} else {
					result->SetMatchedResultOfChild( ITwoWayValueComparisonResult::CreateExactlyMatchedResult() );
				}
			}
			it = it->Next();
		}
		return result;
	}

	spITwoWayValueComparisonResult TwoWayComparisonEngineImpl::compareUnOrderedArrays( const spcIArrayNode & base, const spcIArrayNode & latest ) const {
		spITwoWayBasicComparisonResult basicResult = CompareBasicProperties ( base, latest );
		if ( !basicResult->IsEqual () )
			return ITwoWayValueComparisonResult::CreateExactlyMisMatchedResult ();

		spITwoWayValueComparisonResult result = ITwoWayValueComparisonResult::CreateExactlyMatchedResult();
		sizet nElements = base->ChildCount();

		if ( nElements > 0 ) {
			spITwoWayValueComparisonResultMatrix matrix = CreateAllCombinationsOfCompareValuesForArrayElements( base, latest );
			std::vector< sizet, TAllocator< sizet > > innerIndices;
			innerIndices.reserve( nElements );
			for ( sizet i = 0; i < nElements; i++ )
				innerIndices.push_back( i );

			for ( sizet i = 0; i < nElements; i++ ) {
				sizet loopCount = innerIndices.size();
				spITwoWayValueComparisonResults & row = ( *matrix )[ i ];
				double maxValueInTheRow = ( *row )[ innerIndices[ 0 ] ]->GetMatchingPercetage();
				sizet indexWithMaxValueInTheRow = 0;

				for ( sizet j = 1; j < loopCount; j++ ) {
					double currentValue = ( *row )[ innerIndices[ j ] ]->GetMatchingPercetage();
					if ( currentValue > maxValueInTheRow ) {
						maxValueInTheRow = currentValue;
						indexWithMaxValueInTheRow = j;
					}
				}

				spcITwoWayValueComparisonResult chosenResult = ( *row )[ innerIndices[ indexWithMaxValueInTheRow ] ];
				result->SetMatchedResultOfChild( chosenResult );

				if ( mStopOnFirstDifferentValueEncountered && !chosenResult->IsExactMatch() ) {
					return result;
				}

				innerIndices.erase( innerIndices.begin() + indexWithMaxValueInTheRow );
			}
		}
		return result;
	}

	spITwoWayValueComparisonResult TwoWayComparisonEngineImpl::compareOrderedArrays( const spcIArrayNode & base, const spcIArrayNode & latest ) const {
		spITwoWayBasicComparisonResult basicResult = CompareBasicProperties (base, latest);
		if ( !basicResult->IsEqual () )
			return ITwoWayValueComparisonResult::CreateExactlyMisMatchedResult () ;

		spITwoWayValueComparisonResult result = ITwoWayValueComparisonResult::CreateExactlyMatchedResult();
		sizet nElements = base->ChildCount();
		for ( sizet i = 0; i < nElements; i++ ) {
			bool exactMatchFound = false;
			spITwoWayValueComparisonResult childResult = CompareValues( base->GetNodeAtIndex( i + 1 ), latest->GetNodeAtIndex( i + 1 ) );
			if ( childResult->IsExactMatch ( ) )
			{
				spITwoWayValueComparisonResult qualifiersComparisonResult = CompareQualifiers ( base->GetNodeAtIndex ( i + 1 ) , latest->GetNodeAtIndex ( i + 1 ) );
				result->SetMatchedResultOfChild ( qualifiersComparisonResult );
				if(qualifiersComparisonResult->IsExactMatch())
					exactMatchFound = true;

			}
			else
				result->SetMatchedResultOfChild( childResult );

			if ( mStopOnFirstDifferentValueEncountered && !exactMatchFound )
				return result;
		}
		return result;

	}

	spITwoWayValueComparisonResult TwoWayComparisonEngineImpl::compareAlternateArrays( const spcIArrayNode & base, const spcIArrayNode & latest ) const {
		return compareOrderedArrays( base, latest );
	}



	spITwoWayComparisonEngine ITwoWayComparisonEngine::CreateTwoWayComparisonEngine() {
		return MakeUncheckedSharedPointer( new TwoWayComparisonEngineImpl(), __FILE__, __LINE__, true );
	}

}
