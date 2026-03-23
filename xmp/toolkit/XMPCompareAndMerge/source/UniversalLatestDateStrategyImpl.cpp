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

#include "XMPCompareAndMerge/ImplHeaders/UniversalLatestDateStrategyImpl.h"
#include "XMPCompareAndMerge/ImplHeaders/AutoMergeUtilities.h"


#include <string>
#define TXMP_STRING_TYPE std::string 
#include "public/include/XMP.hpp"  
#include "public/include/XMP.incl_cpp"

#include "XMPCore/Interfaces/ISimpleNode.h"
#include "XMPCore/Interfaces/IPath.h"
#include "XMPCore/Interfaces/ICompositeNode.h"
#include "XMPCore/Interfaces/IArrayNode.h"
#include "XMPCore/Interfaces/IPathSegment.h"

namespace AdobeXMPCompareAndMerge_Int {

	static const char instanceIDPath[] = kXMP_NS_XMP_MM ":InstanceID";
	static const char documentIDPath[] = kXMP_NS_XMP_MM ":DocumentID";
	static const char originalDocumentIDPath[] = kXMP_NS_XMP_MM ":OriginalDocumentID";
	static const char versionIDPath[] = kXMP_NS_XMP_MM ":VersionID";
	static const char pdfVersionPath[] = kXMP_NS_PDF ":PDFVersion";

	spIThreeWayGenericStrategy UniversalLatestDateStrategyImpl::CreateUniversalLatestDateStrategyImpl() {
		return MakeUncheckedSharedPointer(new UniversalLatestDateStrategyImpl(), __FILE__, __LINE__, true);
	}

	ThreeWayGenericStrategyImpl * APICALL UniversalLatestDateStrategyImpl::createAndCloneContents() const {
		return new UniversalLatestDateStrategyImpl();
	}



	bool APICALL UniversalLatestDateStrategyImpl::InterestedInResolvingConflict(const spcIUTF8String & nodePath, INode::eNodeType nodeType, IThreeWayUnResolvedConflict::eConflictReason reason, bool alreadyResolved) {
		if (alreadyResolved)
			return false;
		if (mPaths->size() == 0)
			return false;
		sizet nodePathSize = nodePath->size();
		for (auto it = mPaths->begin(); it != mPaths->end(); it++)
		{
			sizet registeredPathSize = (*it)->size();
			if ((nodePathSize == registeredPathSize) && (strncmp(nodePath->c_str(), (*it)->c_str(), registeredPathSize) == 0)) {
				return true;
			}
		}
		return false;
	}

	static XMP_DateTime ConvertToDate(const spcISimpleNode & node) {
		XMP_DateTime date;
		SXMPUtils::ConvertToDate(node->GetValue()->c_str(), &date);
		return date;
	}

	static bool operator > (const XMP_DateTime & lhs, const XMP_DateTime & rhs) {
		if (SXMPUtils::CompareDateTime(lhs, rhs) > 0)
			return true;
		return false;
	}

	static bool verifyNode(spcINode node)
	{
		//This would verify if this is the correct strategy for this node.
		if (!node)
			return false;
		INode::eNodeType nodeType = node->GetNodeType();
		XMP_DateTime date;
		if (nodeType != INode::kNTSimple)
			return false;
		const spcIUTF8String  nodePath = node->GetPath()->Serialize();
		if (strncmp(nodePath->c_str(), instanceIDPath, nodePath->size()) != 0 &&
			strncmp(nodePath->c_str(), versionIDPath, nodePath->size()) != 0 &&
			strncmp(nodePath->c_str(), pdfVersionPath, nodePath->size()) != 0 &&
			strncmp(nodePath->c_str(), documentIDPath, nodePath->size()) != 0 &&
			strncmp(nodePath->c_str(), originalDocumentIDPath, nodePath->size()) != 0)
		{
			try {
				date = ConvertToDate(node->ConvertToSimpleNode());
			}
			catch (...)
			{
				return false;
			}
		}
		return true;
	}

	static spcICompositeNode convertToCompositeNode(spcINode node){
		INode::eNodeType nodetype=node->GetNodeType();
		switch(nodetype){

			case INode::kNTArray: return node->ConvertToArrayNode();
			case INode::kNTStructure: return node->ConvertToStructureNode();
			default: return spcICompositeNode();
		}

	}
	IThreeWayUnResolvedConflict::eConflictReason APICALL UniversalLatestDateStrategyImpl::ResolveConflict(IThreeWayUnResolvedConflict::eConflictReason conflictReason,
		const spcINode & baseVersionNode, const spcINode & latestVersionNode, const spcINode & userVersionNode,
		spcINode & resolvedNode, IThreeWayResolvedConflict::eConflictResolutionReason & conflictResolutionReason, IThreeWayResolvedConflict::eResolvedSource & resolvedSource)
	{

		switch (conflictReason) {
		case IThreeWayUnResolvedConflict::kCRURIMismatch:
			return conflictReason;
			break;


		case IThreeWayUnResolvedConflict::kCRTypesMismatch:
			NOTIFY_ERROR(IError_base::kEDDataModel, kDMECDifferentNodeTypePresent,
				"Node type is incompatible with the registered strategy", IError_base::kESOperationFatal, false, false);
			break;

		case IThreeWayUnResolvedConflict::kCRQualifiersMismatch:
		{
			if (!verifyNode(latestVersionNode) || !verifyNode(userVersionNode))
				NOTIFY_ERROR(IError_base::kEDDataModel, kDMECDifferentNodeTypePresent,
					"Node type is incompatible with the registered strategy", IError_base::kESOperationFatal, false, false);
			AdobeXMPCore::spISimpleNode mergedNode = latestVersionNode->Clone()->ConvertToSimpleNode();
			if (verifyNode(baseVersionNode))
				AutoMergeUtilities::resolveQualifier(baseVersionNode, latestVersionNode, userVersionNode, mergedNode);
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
			XMP_DateTime latestDate;
			bool resolved(false);
			bool dependentProperties = false;
			bool baseUsed = false;
			bool latestUsed = false;

			const spcIUTF8String  nodePath = latestVersionNode ? latestVersionNode->GetPath()->Serialize() : userVersionNode->GetPath()->Serialize();
			spIPath path;

			if (strncmp(nodePath->c_str(), originalDocumentIDPath, nodePath->size()) == 0)
				NOTIFY_ERROR(IError_base::kEDDataModel, kDMECDifferentNodeTypePresent,
					"xmpMM:OriginalDocumentID should not be edited", IError_base::kESOperationFatal, false, false);

			if (strncmp(nodePath->c_str(), instanceIDPath, nodePath->size()) == 0 ||
				strncmp(nodePath->c_str(), versionIDPath, nodePath->size()) == 0 ||
				strncmp(nodePath->c_str(), pdfVersionPath, nodePath->size()) == 0 ||
				strncmp(nodePath->c_str(), documentIDPath, nodePath->size()) == 0) {
				path = IPath::CreatePath();
				spcIPathSegment segment = IPathSegment::CreatePropertyPathSegment(kXMP_NS_XMP, AdobeXMPCommon::npos, "MetadataDate", AdobeXMPCommon::npos);
				path->AppendPathSegment(segment);
				dependentProperties = true;
			}

			if (baseVersionNode && baseVersionNode->GetParent()) {
				if (verifyNode(baseVersionNode)) {
					baseUsed = true;
					spcINode node = baseVersionNode;
					if (dependentProperties) {
						auto parentNode=convertToCompositeNode(baseVersionNode->GetParent());
						if(parentNode)
							node = parentNode -> GetNodeAtPath(path);
						//node = dynamic_cast<pcICompositeNode>(baseVersionNode->GetParent()->GetActualINode())->GetNodeAtPath(path);
						if (!node)
						{
							path = IPath::CreatePath();
							spcIPathSegment segment = IPathSegment::CreatePropertyPathSegment(kXMP_NS_XMP, AdobeXMPCommon::npos, "ModifyDate", AdobeXMPCommon::npos);
							path->AppendPathSegment(segment);
							parentNode=convertToCompositeNode(baseVersionNode->GetParent());
							if(parentNode)
								node = parentNode -> GetNodeAtPath(path);
							//node = dynamic_cast<pcICompositeNode>(baseVersionNode->GetParent()->GetActualINode())->GetNodeAtPath(path);
						}
					}
					resolved = true;
					if (!node)
						return conflictReason;
					try {
						latestDate = ConvertToDate(node->ConvertToSimpleNode());
					}
					catch (...)
					{
						return conflictReason;
					}
					resolvedNode = baseVersionNode;
					resolvedSource = IThreeWayResolvedConflict::kRSBaseVersion;
					conflictResolutionReason = IThreeWayResolvedConflict::kCRRStrategyResolved;
				}
			}

			if (latestVersionNode && latestVersionNode->GetParent()) {
				if (!verifyNode(latestVersionNode))
					NOTIFY_ERROR(IError_base::kEDDataModel, kDMECDifferentNodeTypePresent,
						"Node type is incompatible with the registered strategy", IError_base::kESOperationFatal, false, false);
				latestUsed = true;
				spcINode node = latestVersionNode;
				if (dependentProperties) {
					auto parentNode=convertToCompositeNode(latestVersionNode->GetParent());
					if(parentNode)
						node = parentNode -> GetNodeAtPath(path);
					//node = dynamic_cast<pcICompositeNode>(latestVersionNode->GetParent()->GetActualINode())->GetNodeAtPath(path);
					if (!node && !baseUsed) {
						path = IPath::CreatePath();
						spcIPathSegment segment = IPathSegment::CreatePropertyPathSegment(kXMP_NS_XMP, AdobeXMPCommon::npos, "ModifyDate", AdobeXMPCommon::npos);
						path->AppendPathSegment(segment);
						parentNode=convertToCompositeNode(baseVersionNode->GetParent());
						if(parentNode)
							node = parentNode -> GetNodeAtPath(path);
						//node = dynamic_cast<pcICompositeNode>(baseVersionNode->GetParent()->GetActualINode())->GetNodeAtPath(path);
					}
				}
				resolved = true;
				XMP_DateTime date;
				if (!node)
					return conflictReason;
				try {
					date = ConvertToDate(node->ConvertToSimpleNode());
				}
				catch (...)
				{
					return conflictReason;
				}
				if (date > latestDate) {
					latestDate = date;
					resolvedNode = latestVersionNode;
					resolvedSource = IThreeWayResolvedConflict::kRSLatestVersion;
					conflictResolutionReason = IThreeWayResolvedConflict::kCRRStrategyResolved;
				}
			}

			if (userVersionNode && userVersionNode->GetParent()) {
				if (!verifyNode(userVersionNode))
					NOTIFY_ERROR(IError_base::kEDDataModel, kDMECDifferentNodeTypePresent,
						"Node type is incompatible with the registered strategy", IError_base::kESOperationFatal, false, false);
				spcINode node = userVersionNode;
				if (dependentProperties) {
					auto parentNode=convertToCompositeNode(userVersionNode->GetParent());
					if(parentNode)
						node = parentNode -> GetNodeAtPath(path);
					//node = dynamic_cast<pcICompositeNode>(userVersionNode->GetParent()->GetActualINode())->GetNodeAtPath(path);
					if (!node && !baseUsed && !latestUsed) {
						path = IPath::CreatePath();
						spcIPathSegment segment = IPathSegment::CreatePropertyPathSegment(kXMP_NS_XMP, AdobeXMPCommon::npos, "ModifyDate", AdobeXMPCommon::npos);
						path->AppendPathSegment(segment);
						parentNode=convertToCompositeNode(baseVersionNode->GetParent());
						if(parentNode)
							node = parentNode -> GetNodeAtPath(path);
						//node = dynamic_cast<pcICompositeNode>(baseVersionNode->GetParent()->GetActualINode())->GetNodeAtPath(path);
					}
				}
				resolved = true;
				if (!node)
					return conflictReason;
				XMP_DateTime date;
				try {
					date = ConvertToDate(node->ConvertToSimpleNode());
				}
				catch (...)
				{
					return conflictReason;
				}
				if (date > latestDate) {
					latestDate = date;
					resolvedNode = userVersionNode;
					resolvedSource = IThreeWayResolvedConflict::kRSUserVersion;
					conflictResolutionReason = IThreeWayResolvedConflict::kCRRStrategyResolved;
				}

			}

			if (resolved)
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
