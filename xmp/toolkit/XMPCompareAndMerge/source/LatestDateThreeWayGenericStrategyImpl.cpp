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

#include "XMPCompareAndMerge/ImplHeaders/LatestDateThreeWayGenericStrategyImpl.h"

#include "XMPCommon/Interfaces/IError_I.h"
#include "XMPCompareAndMerge/XMPCompareAndMergeErrorCodes.h"

#include <string>
#define TXMP_STRING_TYPE std::string 
#include "public/include/XMP.hpp"  
#include "public/include/XMP.incl_cpp"

#include "XMPCommon/Utilities/TSmartPointers_I.h"
#include "XMPCommon/Interfaces/IUTF8String.h"

#include "XMPCore/Interfaces/ISimpleNode.h"
#include "XMPCore/Interfaces/IPath.h"
#include "XMPCore/Interfaces/ICompositeNode.h"
#include "XMPCore/Interfaces/IArrayNode.h"
#include "XMPCore/Interfaces/IPathSegment.h"
#include "XMPCore/XMPCoreFwdDeclarations.h"
#include "XMPCore/Interfaces/INameSpacePrefixMap.h"
#include <algorithm>

namespace AdobeXMPCompareAndMerge_Int {

	spIThreeWayGenericStrategy LatestDateThreeWayGenericStrategyImpl::CreateLatestDateThreeWayGenericStrategy() {
		return MakeUncheckedSharedPointer( new LatestDateThreeWayGenericStrategyImpl(), __FILE__, __LINE__, true );
	}

	ThreeWayGenericStrategyImpl * APICALL LatestDateThreeWayGenericStrategyImpl::createAndCloneContents() const {
		return new LatestDateThreeWayGenericStrategyImpl();
	}

	bool APICALL LatestDateThreeWayGenericStrategyImpl::InterestedInResolvingConflict( const spcIUTF8String & nodePath, INode::eNodeType nodeType, IThreeWayUnResolvedConflict::eConflictReason reason, bool alreadyResolved ) {
		if ( alreadyResolved )
			return false;
		if ( mPaths->size() == 0 )
			return false;
		if ( nodeType != INode::kNTSimple )
			return false;

		sizet nodePathSize = nodePath->size();
		for (auto it = mPaths->begin(); it != mPaths->end(); it++)
		{
			sizet registeredPathSize = (*it)->size();
			if ((nodePathSize == registeredPathSize) && (strncmp(nodePath->c_str(), (*it)->c_str(), registeredPathSize) == 0)){
				return true;
			}
		}
		return false;
	}

	static XMP_DateTime ConvertToDate( const spcISimpleNode & node ) {
		XMP_DateTime date;
		SXMPUtils::ConvertToDate( node->GetValue()->c_str(), &date );
		return date;
	}

	static bool operator > ( const XMP_DateTime & lhs, const XMP_DateTime & rhs ) {
		if ( SXMPUtils::CompareDateTime( lhs, rhs ) > 0 )
			return true;
		return false;
	}

	static spcICompositeNode convertToCompositeNode(spcINode node){
		INode::eNodeType nodetype=node->GetNodeType();
		switch(nodetype){

			case INode::kNTArray: return node->ConvertToArrayNode();
			case INode::kNTStructure: return node->ConvertToStructureNode();
            default: return spcICompositeNode();
		}

	}

	IThreeWayUnResolvedConflict::eConflictReason APICALL LatestDateThreeWayGenericStrategyImpl::ResolveConflict( IThreeWayUnResolvedConflict::eConflictReason conflictReason,
		const spcINode & baseVersionNode, const spcINode & latestVersionNode, const spcINode & userVersionNode,
		spcINode & resolvedNode, IThreeWayResolvedConflict::eConflictResolutionReason & conflictResolutionReason, IThreeWayResolvedConflict::eResolvedSource & resolvedSource )
	{
		XMP_DateTime latestDate;
		bool resolved(false);
		bool insID = false;

		const spcIUTF8String  nodePath = latestVersionNode ? latestVersionNode->GetPath()->Serialize() : userVersionNode->GetPath()->Serialize();
		static const char instanceIDPath[] = kXMP_NS_XMP_MM ":InstanceID";
		spIPath path;
		if (strncmp(nodePath->c_str(), instanceIDPath, nodePath->size()) == 0){
			path = IPath::CreatePath();
			spcIPathSegment segment = IPathSegment::CreatePropertyPathSegment(kXMP_NS_XMP, AdobeXMPCommon::npos, "MetadataDate", AdobeXMPCommon::npos);
			path->AppendPathSegment(segment);
			insID = true;
		}

		if (baseVersionNode && baseVersionNode->GetParent()) {
			spcINode node = baseVersionNode;
			if (insID){
				auto parentNode=convertToCompositeNode(baseVersionNode->GetParent());
				if(parentNode)
					node = parentNode -> GetNodeAtPath(path);
				//node = dynamic_cast<pcICompositeNode>(baseVersionNode->GetParent()->GetActualINode())->GetNodeAtPath(path);
			}
			resolved = true;
			if (!node->ConvertToSimpleNode())
				return conflictReason;
			try{
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

		if (latestVersionNode && latestVersionNode->GetParent()) {
			spcINode node = latestVersionNode;
			if (insID){
                auto parentNode=convertToCompositeNode(latestVersionNode->GetParent());
                if(parentNode)
                    node = parentNode -> GetNodeAtPath(path);
				//node = dynamic_cast<pcICompositeNode>(latestVersionNode->GetParent()->GetActualINode())->GetNodeAtPath(path);
			}
			resolved = true;
			XMP_DateTime date;
			if (!node->ConvertToSimpleNode())
				return conflictReason;
			try{
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
			spcINode node = userVersionNode;
			if (insID){
                auto parentNode=convertToCompositeNode(userVersionNode->GetParent());
                if(parentNode)
                    node = parentNode -> GetNodeAtPath(path);
				//node = dynamic_cast<pcICompositeNode>(userVersionNode->GetParent()->GetActualINode())->GetNodeAtPath(path);
			}
			resolved = true;
			if (!node->ConvertToSimpleNode())
				return conflictReason;
			XMP_DateTime date;
			try{
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


}
