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

#include "XMPCompareAndMerge/ImplHeaders/UniversalVersionsStrategyImpl.h"
#include "XMPCompareAndMerge/ImplHeaders/AutoMergeUtilities.h"

#define TXMP_STRING_TYPE std::string 
#include "public/include/XMP.hpp"  

namespace AdobeXMPCompareAndMerge_Int {

	static bool verifyNode(spcINode node)
	{
		if (!node)
			return false;
		INode::eNodeType nodeType = node->GetNodeType();
		if (nodeType != INode::kNTArray)
			return false;
		
		spcIArrayNode arrayNode = node->ConvertToArrayNode();
		IArrayNode::eArrayForm arrayForm = arrayNode->GetArrayForm();
		if (arrayForm != IArrayNode::kAFOrdered)
			return false;

		if (!AutoMergeUtilities::verifyChildTypeArrOfStructOfSimpleAndStruct(arrayNode))
			return false;
		return true;
	}

	static spINode MergeEntries(const spcINode & source1, const spcINode & source2) {
		//This will not take care of qualifiers on source1 and source 2. will have to be resolved in the calling function
		//source1 and source2 are both array types
		std::set<std::string> versionNumber;
		spcIArrayNode array1 = source1->ConvertToArrayNode();
		spcIArrayNode array2 = source2->ConvertToArrayNode();

		sizet indexInArray1 = 1, indexInArray2 = 1, sizeOfArray1 = array1->ChildCount(), sizeOfArray2 = array2->ChildCount();
		// array index is 1 based.
		sizeOfArray1++;
		sizeOfArray2++;

		spIArrayNode mergedArray = array2->Clone()->ConvertToArrayNode();
		mergedArray->Clear(true,false);
		std::vector<spIStructureNode> nodesInArray1, nodesInArray2;
		XMP_DateTime dateInNode1, dateInNode2;
		bool useNodeInArray2 = false;
		bool useBoth = true;
		bool incorrectNodeInArray1, incorrectNodeInArray2;
		while (indexInArray1 < sizeOfArray1 && indexInArray2 < sizeOfArray2) {
			incorrectNodeInArray1 = incorrectNodeInArray2 = false;
			    
				spcISimpleNode versionModifyDate1 = spcISimpleNode();
				if (useBoth || !useNodeInArray2) {
					while (indexInArray1 < sizeOfArray1 && !versionModifyDate1) //if in = size, then think
					{
						spcIStructureNode nodeInArray1 = array1->GetStructureNodeAtIndex(indexInArray1);
						versionModifyDate1 = nodeInArray1->GetSimpleNode(kXMP_NS_XMP_ST_Version, AdobeXMPCommon::npos, "modifyDate", AdobeXMPCommon::npos);
						nodesInArray1.push_back(nodeInArray1->Clone()->ConvertToStructureNode());
						indexInArray1++;
					}
					//if (indexInArray1 != sizeOfArray1)
						indexInArray1--;
					if (versionModifyDate1) {
						try {
							SXMPUtils::ConvertToDate(versionModifyDate1->GetValue()->c_str(), &dateInNode1); // catch and think of exception
						}
						catch (...)
						{
							incorrectNodeInArray1 = true;
						}
					}
					else
						incorrectNodeInArray1 = true;
				}
				
				spcISimpleNode versionModifyDate2 = spcISimpleNode();
				if (useBoth || useNodeInArray2) {
					while (indexInArray2 < sizeOfArray2 && !versionModifyDate2) //if in = size, then think
					{
						spcIStructureNode nodeInArray2 = array2->GetStructureNodeAtIndex(indexInArray2);
						versionModifyDate2 = nodeInArray2->GetSimpleNode(kXMP_NS_XMP_ST_Version, AdobeXMPCommon::npos, "modifyDate", AdobeXMPCommon::npos);
						nodesInArray2.push_back(nodeInArray2->Clone()->ConvertToStructureNode());
						indexInArray2++;
					}
					//if (indexInArray2 != sizeOfArray2)
						indexInArray2--;
					if (versionModifyDate2) {
						try {
							SXMPUtils::ConvertToDate(versionModifyDate2->GetValue()->c_str(), &dateInNode2);
						}
						catch (...)
						{
							incorrectNodeInArray2 = true;
						}
					}
					else
						incorrectNodeInArray2 = true;
				}
				
				if (incorrectNodeInArray1)
				{
					if (incorrectNodeInArray2) {
						indexInArray1++;
						indexInArray2++;
						useBoth = true;
						nodesInArray1.clear();
						nodesInArray2.clear();
						continue;
					}
					else {
						useBoth = false;
						useNodeInArray2 = false;
						indexInArray1++;
						nodesInArray1.clear();
						continue;
					}
				}
				else if (incorrectNodeInArray2) {
					useBoth = false;
					useNodeInArray2 = true;
					indexInArray2++;
					nodesInArray2.clear();
					continue;
				}

				// compare two if equal pick any one and add into the main 
				std::vector<spIStructureNode>::iterator it1 = nodesInArray1.begin();
				std::vector<spIStructureNode>::iterator it2 = nodesInArray2.begin();
				for (; it1 != nodesInArray1.end() && it2 != nodesInArray2.end();) {
					if (AutoMergeUtilities::compareStructOfSimpleAndStruct(*it1, *it2, false)) {
						spcISimpleNode versionNo = (*it2)->GetSimpleNode(kXMP_NS_XMP_ST_Version, AdobeXMPCommon::npos, "version", AdobeXMPCommon::npos);
						if(!versionNo || versionNumber.insert(versionNo->GetValue()->c_str()).second)
							mergedArray->AppendNode(*it2);
						it1 = nodesInArray1.erase(it1);
						it2 = nodesInArray2.erase(it2);
					}
					else
						break;
				}
				//Since both the vectors end in date, then either both will have remaing values or none
				if (it1 == nodesInArray1.end() || it2 == nodesInArray2.end()) {
					indexInArray1++;
					indexInArray2++;
					useBoth = true;
					nodesInArray1.clear();
					nodesInArray2.clear();
					continue;
				}
				else {

					if (SXMPUtils::CompareDateTime(dateInNode1, dateInNode2) >= 0) {
						useNodeInArray2 = true;
						useBoth = false;
						indexInArray2++;
					}
					else {
						useNodeInArray2 = false;
						useBoth = false;
						indexInArray1++;
					}

				}

				if (useNodeInArray2)
				{
					std::vector<spIStructureNode>::iterator iter2 = nodesInArray2.begin();
					for (; iter2 != nodesInArray2.end(); iter2++)
					{
						spcISimpleNode versionNo = (*iter2)->GetSimpleNode(kXMP_NS_XMP_ST_Version, AdobeXMPCommon::npos, "version", AdobeXMPCommon::npos);
						if (!versionNo || versionNumber.insert(versionNo->GetValue()->c_str()).second)
							mergedArray->AppendNode(*iter2);
					}
					nodesInArray2.clear();
				}
				else {
					std::vector<spIStructureNode>::iterator iter1 = nodesInArray1.begin();
					for (; iter1 != nodesInArray1.end(); iter1++)
					{
						spcISimpleNode versionNo = (*iter1)->GetSimpleNode(kXMP_NS_XMP_ST_Version, AdobeXMPCommon::npos, "version", AdobeXMPCommon::npos);
						if (!versionNo || versionNumber.insert(versionNo->GetValue()->c_str()).second)
							mergedArray->AppendNode(*iter1);
					}
					nodesInArray1.clear();
				}
		
		}
	
		if (indexInArray1 == sizeOfArray1) 
		{ // we have exhausted all the nodes in array1

			if (!nodesInArray2.empty())
			{
				std::vector<spIStructureNode>::iterator it2;
				bool valid = false;
				XMP_DateTime date;
				spcISimpleNode dateNode = (nodesInArray2.back())->GetSimpleNode(kXMP_NS_XMP_ST_Version, AdobeXMPCommon::npos, "modifyDate", AdobeXMPCommon::npos);
				if (dateNode)
				{
					try {
						SXMPUtils::ConvertToDate(dateNode->GetValue()->c_str(), &date);
						valid = true;
					}
					catch (...)
					{
						//do nothing
					}
				}
				if (valid)
				{
					it2 = nodesInArray2.begin();
					for (; it2 != nodesInArray2.end(); it2++)
					{
						spcISimpleNode versionNo = (*it2)->GetSimpleNode(kXMP_NS_XMP_ST_Version, AdobeXMPCommon::npos, "version", AdobeXMPCommon::npos);
						if (!versionNo || versionNumber.insert(versionNo->GetValue()->c_str()).second)
							mergedArray->AppendNode(*it2);
					}
				}
				indexInArray2++;
				nodesInArray2.clear();
			}

			for (; indexInArray2 < sizeOfArray2; indexInArray2++) {
				nodesInArray2.clear();
				spcISimpleNode whenNodeInArray2 = spcISimpleNode();
				XMP_DateTime date;
				while (indexInArray2 < sizeOfArray2 && !whenNodeInArray2) //if in = size, then think
				{
					spcIStructureNode nodeInArray2 = array2->GetStructureNodeAtIndex(indexInArray2);
					whenNodeInArray2 = nodeInArray2->GetSimpleNode(kXMP_NS_XMP_ST_Version, AdobeXMPCommon::npos, "modifyDate", AdobeXMPCommon::npos);
					nodesInArray2.push_back(nodeInArray2->Clone()->ConvertToStructureNode());
					indexInArray2++;
				}
				indexInArray2--;
				if (whenNodeInArray2)
				{
					try {
						SXMPUtils::ConvertToDate(whenNodeInArray2->GetValue()->c_str(), &date);
						for (std::vector<spIStructureNode>::iterator it2 = nodesInArray2.begin(); it2 != nodesInArray2.end(); it2++)
						{
							spcISimpleNode versionNo = (*it2)->GetSimpleNode(kXMP_NS_XMP_ST_Version, AdobeXMPCommon::npos, "version", AdobeXMPCommon::npos);
							if (!versionNo || versionNumber.insert(versionNo->GetValue()->c_str()).second)
								mergedArray->AppendNode(*it2);
						}
					}
					catch (...)
					{
						//do nothing
					}
				}
			}
		}
		else if (indexInArray2 == sizeOfArray2) { // we have exhausted all the nodes in array2
			if (!nodesInArray1.empty())
			{
				std::vector<spIStructureNode>::iterator it1;
				bool valid = false;
				XMP_DateTime date;
				spcISimpleNode dateNode = (nodesInArray1.back())->GetSimpleNode(kXMP_NS_XMP_ST_Version, AdobeXMPCommon::npos, "modifyDate", AdobeXMPCommon::npos);
				if (dateNode)
				{
					try {
						SXMPUtils::ConvertToDate(dateNode->GetValue()->c_str(), &date);
						valid = true;
					}
					catch (...)
					{
						//do nothing
					}
				}
				if (valid)
				{
					it1 = nodesInArray1.begin();
					for (; it1 != nodesInArray1.end(); it1++)
					{
						spcISimpleNode versionNo = (*it1)->GetSimpleNode(kXMP_NS_XMP_ST_Version, AdobeXMPCommon::npos, "version", AdobeXMPCommon::npos);
						if (!versionNo || versionNumber.insert(versionNo->GetValue()->c_str()).second)
							mergedArray->AppendNode(*it1);
					}
				}
				indexInArray1++;
				nodesInArray1.clear();
			}


			for (; indexInArray1 < sizeOfArray1; indexInArray1++) {
				nodesInArray1.clear();
				spcISimpleNode whenNodeInArray1 = spcISimpleNode();
				XMP_DateTime date;
				while (indexInArray1 < sizeOfArray1 && !whenNodeInArray1) //if in = size, then think
				{
					spcIStructureNode nodeInArray1 = array1->GetStructureNodeAtIndex(indexInArray1);
					whenNodeInArray1 = nodeInArray1->GetSimpleNode(kXMP_NS_XMP_ST_Version, AdobeXMPCommon::npos, "modifyDate", AdobeXMPCommon::npos);
					nodesInArray1.push_back(nodeInArray1->Clone()->ConvertToStructureNode());
					indexInArray1++;
				}
				indexInArray1--;
				if (whenNodeInArray1)
				{
					try {
						SXMPUtils::ConvertToDate(whenNodeInArray1->GetValue()->c_str(), &date);
						for (std::vector<spIStructureNode>::iterator it1 = nodesInArray1.begin(); it1 != nodesInArray1.end(); it1++)
						{
							spcISimpleNode versionNo = (*it1)->GetSimpleNode(kXMP_NS_XMP_ST_Version, AdobeXMPCommon::npos, "version", AdobeXMPCommon::npos);
							if (!versionNo || versionNumber.insert(versionNo->GetValue()->c_str()).second)
								mergedArray->AppendNode(*it1);
						}
					}
					catch (...)
					{
						//do nothing
					}
				}
			}
		}
		
		return mergedArray;
	}

	bool APICALL UniversalVersionsStrategyImpl::InterestedInResolvingConflict(const spcIUTF8String & nodePath, INode::eNodeType nodeType, IThreeWayUnResolvedConflict::eConflictReason reason, bool alreadyResolved)
	{
		static const char versionsPath[] = kXMP_NS_XMP_MM ":Versions";
		
		if (alreadyResolved)
			return false;
	    
		if (strncmp(nodePath->c_str(), versionsPath, nodePath->size()) == 0) {
			return true;
		}
		
		return false;
	}

	IThreeWayUnResolvedConflict::eConflictReason APICALL UniversalVersionsStrategyImpl::ResolveConflict(IThreeWayUnResolvedConflict::eConflictReason conflictReason,
		const spcINode & baseVersionNode, const spcINode & latestVersionNode, const spcINode & userVersionNode,
		spcINode & resolvedNode, IThreeWayResolvedConflict::eConflictResolutionReason & conflictResolutionReason, IThreeWayResolvedConflict::eResolvedSource & resolvedSource)
	{
		//To-Do what to do for kCRQualifiersMismatch and kCRURIMismatch
	
		bool useBase(false);
		if (verifyNode(baseVersionNode))
			useBase = true;
		AdobeXMPCore::spINode mergedNode;
		
		switch (conflictReason) {
		case IThreeWayUnResolvedConflict::kCRURIMismatch:
			return conflictReason;
			break;

		case IThreeWayUnResolvedConflict::kCRTypesMismatch:
			NOTIFY_ERROR(IError_base::kEDDataModel, kDMECDifferentNodeTypePresent,
				"Different type of node present in all three packets ", IError_base::kESOperationFatal, false, false);
			break;

			//kCRQualifiersMismatch would come only when values are same and only qualifiers differ.
		case IThreeWayUnResolvedConflict::kCRQualifiersMismatch:
			if(!verifyNode(latestVersionNode) || !verifyNode(userVersionNode))
				NOTIFY_ERROR(IError_base::kEDDataModel, kDMECDifferentNodeTypePresent,
					"Versions must be of type Unordered Array of Structure of xmpMM:Version", IError_base::kESOperationFatal, false, false);

			mergedNode = latestVersionNode->Clone();
			if(useBase)
		 	  AutoMergeUtilities::resolveQualifier(baseVersionNode, latestVersionNode, userVersionNode, mergedNode);
			resolvedNode = mergedNode;
			resolvedSource = IThreeWayResolvedConflict::kRSFieldsMerged;
			conflictResolutionReason = IThreeWayResolvedConflict::kCRRStrategyResolved;
			break;
			
		case IThreeWayUnResolvedConflict::kCRDeletedInLatestEditedInUser:
			if(!verifyNode ( userVersionNode))
				NOTIFY_ERROR(IError_base::kEDDataModel, kDMECDifferentNodeTypePresent,
					"Versions must be of type Unordered Array of Structure of xmpMM:Version", IError_base::kESOperationFatal, false, false);
			if (useBase) 
			{
				resolvedNode = MergeEntries(baseVersionNode, userVersionNode);
				resolvedSource = IThreeWayResolvedConflict::kRSFieldsMerged;
				conflictResolutionReason = IThreeWayResolvedConflict::kCRRStrategyResolved;
			}
			else
			{  
				resolvedNode = userVersionNode;
				resolvedSource = IThreeWayResolvedConflict::kRSUserVersion;
				conflictResolutionReason = IThreeWayResolvedConflict::kCRRStrategyResolved;
			}
			return IThreeWayUnResolvedConflict::kCRResolved;
			break;

		case IThreeWayUnResolvedConflict::kCRDeletedInUserEditedInLatest:
			if (!verifyNode(latestVersionNode))
				NOTIFY_ERROR(IError_base::kEDDataModel, kDMECDifferentNodeTypePresent,
					"Versions must be of type Unordered Array of Structure of xmpMM:Version", IError_base::kESOperationFatal, false, false);
			if (useBase) 
			{
				resolvedNode = MergeEntries(baseVersionNode, latestVersionNode);
				resolvedSource = IThreeWayResolvedConflict::kRSFieldsMerged;
				conflictResolutionReason = IThreeWayResolvedConflict::kCRRStrategyResolved;
			}
			else
			{ 
				resolvedNode = latestVersionNode;
				resolvedSource = IThreeWayResolvedConflict::kRSLatestVersion;
				conflictResolutionReason = IThreeWayResolvedConflict::kCRRStrategyResolved;
			}
			return IThreeWayUnResolvedConflict::kCRResolved;
			break;

		case IThreeWayUnResolvedConflict::kCRValuesMismatch:
			if (!verifyNode(latestVersionNode) || !verifyNode(userVersionNode))
				NOTIFY_ERROR(IError_base::kEDDataModel, kDMECDifferentNodeTypePresent,
					"Versions must be of type Unordered Array of Structure of xmpMM:Version", IError_base::kESOperationFatal, false, false);
			if (useBase) 
				mergedNode = MergeEntries(baseVersionNode, latestVersionNode);
			else 
				mergedNode = latestVersionNode->Clone();
			mergedNode = MergeEntries(userVersionNode, mergedNode);
			if (useBase)
				AutoMergeUtilities::resolveQualifier(baseVersionNode, latestVersionNode, userVersionNode, mergedNode);
			resolvedNode = mergedNode;
			resolvedSource = IThreeWayResolvedConflict::kRSFieldsMerged;
			conflictResolutionReason = IThreeWayResolvedConflict::kCRRStrategyResolved;
			break;

		default:
			return conflictReason;
			break;
		}

		if (resolvedNode)
		{
			if (resolvedNode->IsEmpty())
				resolvedNode = spcINode();
			return IThreeWayUnResolvedConflict::kCRResolved;
		}
		else
			return conflictReason;

	}
}
