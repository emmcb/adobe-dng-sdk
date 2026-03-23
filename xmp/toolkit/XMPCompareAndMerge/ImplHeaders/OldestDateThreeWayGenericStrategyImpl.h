#ifndef __OldestDateThreeWayGenericStrategyImpl_h__
#define __OldestDateThreeWayGenericStrategyImpl_h__ 1

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

	class OldestDateThreeWayGenericStrategyImpl
		: public virtual ThreeWayGenericStrategyImpl
	{
	public:
		OldestDateThreeWayGenericStrategyImpl() {}

		virtual bool APICALL InterestedInResolvingConflict( const spcIUTF8String & nodePath, INode::eNodeType nodeType, IThreeWayUnResolvedConflict::eConflictReason reason, bool alreadyResolved );
		virtual IThreeWayUnResolvedConflict::eConflictReason APICALL ResolveConflict( IThreeWayUnResolvedConflict::eConflictReason conflictReason, const spcINode & baseVersionNode, const spcINode & latestVersionNode, const spcINode & userVersionNode, spcINode & resolvedNode, IThreeWayResolvedConflict::eConflictResolutionReason & conflictResolutionReason, IThreeWayResolvedConflict::eResolvedSource & resolvedSource );

		static spIThreeWayGenericStrategy CreateOldestDateThreeWayGenericStrategy();

	protected:
		virtual ~OldestDateThreeWayGenericStrategyImpl() __NOTHROW__ {}

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

#endif  // __OldestDateThreeWayGenericStrategyImpl_h__
