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

#include "XMPCompareAndMerge/ImplHeaders/OldestDateThreeWayGenericStrategyImpl.h"

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
#include "XMPCore/Interfaces/INameSpacePrefixMap.h"
#include <algorithm>

namespace AdobeXMPCompareAndMerge_Int {

	spIThreeWayGenericStrategy OldestDateThreeWayGenericStrategyImpl::CreateOldestDateThreeWayGenericStrategy() {
		return MakeUncheckedSharedPointer( new OldestDateThreeWayGenericStrategyImpl(), __FILE__, __LINE__, true );
	}

	ThreeWayGenericStrategyImpl * APICALL OldestDateThreeWayGenericStrategyImpl::createAndCloneContents() const {
		return new OldestDateThreeWayGenericStrategyImpl();
	}

	bool APICALL OldestDateThreeWayGenericStrategyImpl::InterestedInResolvingConflict( const spcIUTF8String & nodePath, INode::eNodeType nodeType, IThreeWayUnResolvedConflict::eConflictReason reason, bool alreadyResolved ) {
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
		SXMPUtils::ConvertToDate(node->GetValue()->c_str(), &date);
		return date;

	}

	static bool operator < ( const XMP_DateTime & lhs, const XMP_DateTime & rhs ) {
		if ( SXMPUtils::CompareDateTime( lhs, rhs ) < 0 )
			return true;
		return false;
	}

	IThreeWayUnResolvedConflict::eConflictReason APICALL OldestDateThreeWayGenericStrategyImpl::ResolveConflict( IThreeWayUnResolvedConflict::eConflictReason conflictReason,
		const spcINode & baseVersionNode, const spcINode & latestVersionNode, const spcINode & userVersionNode,
		spcINode & resolvedNode, IThreeWayResolvedConflict::eConflictResolutionReason & conflictResolutionReason, IThreeWayResolvedConflict::eResolvedSource & resolvedSource )
	{
		XMP_DateTime oldestDate;
		bool resolved( false );

		if ( baseVersionNode ) {
			resolved = true;
			if (!baseVersionNode->ConvertToSimpleNode())
				return conflictReason;
			try{
				oldestDate = ConvertToDate(baseVersionNode->ConvertToSimpleNode());
			}
			catch (...)
			{
				return conflictReason;
			}
			resolvedNode = baseVersionNode;
			resolvedSource = IThreeWayResolvedConflict::kRSBaseVersion;
			conflictResolutionReason = IThreeWayResolvedConflict::kCRRStrategyResolved;
		}

		if ( latestVersionNode ) {
			resolved = true;
			XMP_DateTime date;
			if (!latestVersionNode->ConvertToSimpleNode())
				return conflictReason;
			try{
				date = ConvertToDate(latestVersionNode->ConvertToSimpleNode());
			}
			catch (...)
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
			resolved = true;
			XMP_DateTime date;
			if (!userVersionNode->ConvertToSimpleNode())
				return conflictReason;
			try{
				date = ConvertToDate(userVersionNode->ConvertToSimpleNode());
			}
			catch (...)
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

}
