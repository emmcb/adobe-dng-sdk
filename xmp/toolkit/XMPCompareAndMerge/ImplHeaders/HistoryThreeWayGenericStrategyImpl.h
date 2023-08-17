
#ifndef __HistoryThreeWayGenericStrategyImpl_h__
#define __HistoryThreeWayGenericStrategyImpl_h__ 1

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

#include "XMPCompareAndMerge/ImplHeaders/ThreeWayGenericStrategyImpl.h"

#if XMP_WinBuild
#pragma warning( push )
#pragma warning( disable : 4250 )
#endif

namespace AdobeXMPCompareAndMerge_Int {

	class HistoryThreeWayGenericStrategyImpl
		:public virtual ThreeWayGenericStrategyImpl
	{
	public:
		HistoryThreeWayGenericStrategyImpl() {}
		virtual bool APICALL InterestedInResolvingConflict(const spcIUTF8String & nodePath, INode_base::eNodeType nodeType,
			IThreeWayUnResolvedConflict_base::eConflictReason reason, bool alreadyResolved);

		virtual IThreeWayUnResolvedConflict_base::eConflictReason APICALL ResolveConflict(
			IThreeWayUnResolvedConflict_base::eConflictReason reasonsForConflict, const spcINode & commonBaseNode,
			const spcINode & latestVersionNode, const spcINode & userVersionNode, spcINode & resolvedNode,
			IThreeWayResolvedConflict_base::eConflictResolutionReason & reasonForConflictResolution,
			IThreeWayResolvedConflict_base::eResolvedSource & resolvedNodeSource);

		static spIThreeWayGenericStrategy CreateHistoryThreeWayGenericStrategy();
		
	protected:
		virtual ~HistoryThreeWayGenericStrategyImpl() __NOTHROW__ {}
		virtual ThreeWayGenericStrategyImpl * APICALL createAndCloneContents() const;

	#ifdef FRIEND_CLASS_DECLARATION
			FRIEND_CLASS_DECLARATION();
	#endif
			REQ_FRIEND_CLASS_DECLARATION();
	};

}

#if XMP_WinBuild
#pragma warning( pop )
#endif

#endif 

