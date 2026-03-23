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


#include<algorithm>
#include <string>
#define TXMP_STRING_TYPE std::string 
#include "public/include/XMP.hpp"  
#include "public/include/XMP.incl_cpp"

#include "XMPCompareAndMerge/ImplHeaders/HistoryThreeWayGenericStrategyImpl.h"
#include "XMPCore/Interfaces/IArrayNode.h"
#include "XMPCore/Interfaces/ICoreObjectFactory.h"
#include "XMPCore/Interfaces/IStructureNode.h"
#include "XMPCore/Interfaces/ISimpleNode.h"
#include "XMPCore/Interfaces/IPath.h"
#include "XMPCore/Interfaces/INodeIterator.h"
#include "XMPCommon/Interfaces/IUTF8String.h"
#include "XMPCommon/Interfaces/IErrorNotifier_I.h"
#include "XMPCommon/XMPCommonErrorCodes.h"
#include "XMPCommon/Utilities/TSmartPointers_I.h"

namespace AdobeXMPCompareAndMerge_Int {

	using namespace AdobeXMPCommon;
	using namespace AdobeXMPCore;
	using namespace AdobeXMPCompareAndMerge;

	static const char historyPath[] = kXMP_NS_XMP_MM ":History";

	spIThreeWayGenericStrategy HistoryThreeWayGenericStrategyImpl::CreateHistoryThreeWayGenericStrategy() {
		return MakeUncheckedSharedPointer(new HistoryThreeWayGenericStrategyImpl(), __FILE__, __LINE__, true);
	}

	ThreeWayGenericStrategyImpl * APICALL HistoryThreeWayGenericStrategyImpl::createAndCloneContents() const {
		return new HistoryThreeWayGenericStrategyImpl();
	}

	static bool CompareTwoHistoryEntries(const spcIStructureNode & node1, const spcIStructureNode & node2);

	static spcINode MergeEntries(const spcINode & source1, const spcINode & source2);

	static void CopyOneHistoryEntryIntoOther(const spcIStructureNode & sourceNode, spIStructureNode & destNode);

	bool APICALL HistoryThreeWayGenericStrategyImpl::InterestedInResolvingConflict(const spcIUTF8String & nodePath, INode::eNodeType nodeType,
		IThreeWayUnResolvedConflict::eConflictReason reason, bool alreadyResolved)
	{
		if (alreadyResolved)
			return false;

		if ((nodeType != INode::kNTArray) || (reason != IThreeWayUnResolvedConflict::kCRValuesMismatch &&
			reason != IThreeWayUnResolvedConflict::kCRDeletedInLatestEditedInUser &&
			reason != IThreeWayUnResolvedConflict::kCRDeletedInUserEditedInLatest))
			return false;

		if (strncmp(nodePath->c_str(), historyPath, nodePath->size()) == 0){
			return true;
		}

		sizet pathSize = nodePath->size();
		for (auto it = mPaths->begin(); it != mPaths->end(); it++)
		{
			sizet registeredPathSize = (*it)->size();
			if ((pathSize == registeredPathSize) && (strncmp(nodePath->c_str(), (*it)->c_str(), registeredPathSize) == 0)){
				return true;
			}
		}
		return false;
	}

	IThreeWayUnResolvedConflict::eConflictReason APICALL HistoryThreeWayGenericStrategyImpl::ResolveConflict(
		IThreeWayUnResolvedConflict::eConflictReason reasonsForConflict,
		const spcINode & commonBaseNode, const spcINode & latestVersionNode, const spcINode & userVersionNode,
		spcINode & resolvedNode, IThreeWayResolvedConflict::eConflictResolutionReason & reasonForConflictResolution,
		IThreeWayResolvedConflict::eResolvedSource & resolvedNodeSource)
	{
		const spcIUTF8String  nodePath = latestVersionNode ? latestVersionNode->GetPath()->Serialize() : userVersionNode->GetPath()->Serialize();
		if (strncmp(nodePath->c_str(), historyPath, nodePath->size()) != 0){
			return reasonsForConflict;
		}

		switch (reasonsForConflict) {
		case IThreeWayUnResolvedConflict::kCRDeletedInLatestEditedInUser:
			resolvedNode = MergeEntries(commonBaseNode, userVersionNode);
			resolvedNodeSource = IThreeWayResolvedConflict::kRSFieldsMerged;
			reasonForConflictResolution = IThreeWayResolvedConflict::kCRRStrategyResolved;
			break;

		case IThreeWayUnResolvedConflict::kCRDeletedInUserEditedInLatest:
			resolvedNode = MergeEntries(commonBaseNode, latestVersionNode);
			resolvedNodeSource = IThreeWayResolvedConflict::kRSFieldsMerged;
			reasonForConflictResolution = IThreeWayResolvedConflict::kCRRStrategyResolved;
			break;

		case IThreeWayUnResolvedConflict::kCRValuesMismatch:
			if (commonBaseNode) resolvedNode = MergeEntries(commonBaseNode, latestVersionNode);
			else resolvedNode = latestVersionNode;
			resolvedNode = MergeEntries(resolvedNode, userVersionNode);
			resolvedNodeSource = IThreeWayResolvedConflict::kRSFieldsMerged;
			reasonForConflictResolution = IThreeWayResolvedConflict::kCRRStrategyResolved;
			break;

		default:
		case IThreeWayUnResolvedConflict::kCRQualifiersMismatch:
			return reasonsForConflict;
			break;
		}

		if (resolvedNode)
			return IThreeWayUnResolvedConflict::kCRResolved;
		else
			return reasonsForConflict;
	}

	spcINode MergeEntries(const spcINode & source1, const spcINode & source2) {
		//source1 and source2 are both array types
		spcIArrayNode array1 = source1->ConvertToArrayNode();
		spcIArrayNode array2 = source2->ConvertToArrayNode();

		if (!array1 || !array2)
			return spcINode();

		if (array1->GetArrayForm() != IArrayNode::kAFOrdered
			|| array2->GetArrayForm() != IArrayNode::kAFOrdered)
			return spcINode();

		sizet indexInArray1 = 1, indexInArray2 = 1, sizeOfArray1 = array1->ChildCount(), sizeOfArray2 = array2->ChildCount();
		// array index is 1 based.
		sizeOfArray1++;
		sizeOfArray2++;

		spIArrayNode mergedArray = IArrayNode::CreateOrderedArrayNode(array1->GetNameSpace()->c_str(), array1->GetNameSpace()->size(), array1->GetName()->c_str(), array1->GetName()->size());

		while (indexInArray1 < sizeOfArray1 || indexInArray2 < sizeOfArray2) {
			spcIStructureNode nodeInArray1 = array1->GetStructureNodeAtIndex(indexInArray1);
			spcIStructureNode nodeInArray2 = array2->GetStructureNodeAtIndex(indexInArray2);

			if (!nodeInArray1 || !nodeInArray2)
				return spcINode();

			spIStructureNode structNode = IStructureNode::CreateStructureNode(mergedArray->GetNameSpace()->c_str(), mergedArray->GetNameSpace()->size(), mergedArray->GetName()->c_str(), mergedArray->GetName()->size());
			mergedArray->AppendNode(structNode);

			// compare two if equal pick any one and add into the main node
			if (CompareTwoHistoryEntries(nodeInArray1, nodeInArray2)) {
				CopyOneHistoryEntryIntoOther(nodeInArray1, structNode);
				if (!structNode)
					return spcINode();
				indexInArray1++;
				indexInArray2++;
			}
			else {
				// compare dates and chose the one with the smallest date
				XMP_DateTime dateInNode1, dateInNode2;
				// Get the "when" node
				spcISimpleNode whenNodeInArray1 = nodeInArray1->GetSimpleNode(kXMP_NS_XMP_ResourceEvent, AdobeXMPCommon::npos, "when", AdobeXMPCommon::npos);
				if (!whenNodeInArray1)
					return spcINode();

				spcISimpleNode whenNodeInArray2 = nodeInArray2->GetSimpleNode(kXMP_NS_XMP_ResourceEvent, AdobeXMPCommon::npos, "when", AdobeXMPCommon::npos);
				if (!whenNodeInArray2)
					return spcINode();

				try {
					SXMPUtils::ConvertToDate(whenNodeInArray1->GetValue()->c_str(), &dateInNode1);
					SXMPUtils::ConvertToDate(whenNodeInArray2->GetValue()->c_str(), &dateInNode2);
				}
				catch (...) {
					return spcINode();
				}

				if (SXMPUtils::CompareDateTime(dateInNode1, dateInNode2) > 0) {
					CopyOneHistoryEntryIntoOther(nodeInArray2, structNode);
					if (!structNode)
						return spcINode();
					indexInArray2++;
				}
				else {
					CopyOneHistoryEntryIntoOther(nodeInArray1, structNode);
					if (!structNode)
						return spcINode();
					indexInArray1++;
				}

			}

			if (indexInArray1 == sizeOfArray1) { // we have exhausted all the nodes in array1
				for (; indexInArray2 < sizeOfArray2; indexInArray2++) {
					spIStructureNode spStructNode = IStructureNode::CreateStructureNode(mergedArray->GetNameSpace()->c_str(), mergedArray->GetNameSpace()->size(), mergedArray->GetName()->c_str(), mergedArray->GetName()->size());
					mergedArray->AppendNode(spStructNode);
					spcIStructureNode spNodeInArray2 = array2->GetStructureNodeAtIndex(indexInArray2);
					CopyOneHistoryEntryIntoOther(spNodeInArray2, spStructNode);
					if (!spStructNode)
						return spcINode();
				}
			}
			else if (indexInArray2 == sizeOfArray2) { // we have exhausted all the nodes in array2
				for (; indexInArray1 < sizeOfArray1; indexInArray1++) {
					spIStructureNode spStructNode = IStructureNode::CreateStructureNode(mergedArray->GetNameSpace()->c_str(), mergedArray->GetNameSpace()->size(), mergedArray->GetName()->c_str(), mergedArray->GetName()->size());
					mergedArray->AppendNode(spStructNode);
					spcIStructureNode spNodeInArray1 = array1->GetStructureNodeAtIndex(indexInArray1);
					CopyOneHistoryEntryIntoOther(spNodeInArray1, spStructNode);
					if (!spStructNode)
						return spcINode();
				}
			}
		}
		return mergedArray;
	}

	bool CompareTwoHistoryEntries(const spcIStructureNode & node1, const spcIStructureNode & node2) {
		// check basic properties
		if (node1->ChildCount() != node2->ChildCount())
			return false;

		spcINodeIterator iterator(node1->Iterator());

		while (iterator) {
			spcISimpleNode simpleNodeInStruct1 = (iterator->GetSimpleNode());

			if (!simpleNodeInStruct1)
				return false;

			spcISimpleNode simpleNodeInStruct2 = node2->GetSimpleNode(simpleNodeInStruct1->GetNameSpace()->c_str(), simpleNodeInStruct1->GetNameSpace()->size(), simpleNodeInStruct1->GetName()->c_str(), simpleNodeInStruct1->GetName()->size());
			if (!simpleNodeInStruct2)
				return false;

			spcIUTF8String utf8String = simpleNodeInStruct1->GetValue();

			if (strncmp(simpleNodeInStruct1->GetValue()->c_str(), simpleNodeInStruct2->GetValue()->c_str(), simpleNodeInStruct2->GetValue()->size()) != 0)
				return false;

			iterator = iterator->Next();
		}

		return true;
	}

	void CopyOneHistoryEntryIntoOther(const spcIStructureNode & sourceNode, spIStructureNode & destNode) {
		spcINodeIterator iterator(sourceNode->Iterator());
		while (iterator) {
			spcISimpleNode simpleNodeInSourceNode = iterator->GetSimpleNode();

			if (!simpleNodeInSourceNode) {
				destNode.reset();
				return;
			}

			spISimpleNode simpleNodeForDestNode = ISimpleNode::CreateSimpleNode(simpleNodeInSourceNode->GetNameSpace()->c_str(), simpleNodeInSourceNode->GetNameSpace()->size(), simpleNodeInSourceNode->GetName()->c_str(), simpleNodeInSourceNode->GetName()->size(), simpleNodeInSourceNode->GetValue()->c_str(), simpleNodeInSourceNode->GetValue()->size());
			destNode->InsertNode(simpleNodeForDestNode);
			iterator = iterator->Next();
		}
	}

}
