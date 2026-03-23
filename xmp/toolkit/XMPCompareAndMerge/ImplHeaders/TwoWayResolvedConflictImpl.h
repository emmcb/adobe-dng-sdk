#ifndef __TwoWayResolvedConflictImpl_h__
#define __TwoWayResolvedConflictImpl_h__ 1

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

#if !(IMPLEMENTATION_HEADERS_CAN_BE_INCLUDED)
	#error "Not adhering to design constraints"
	// this file should only be included from its own cpp file
#endif

#include "XMPCompareAndMerge/Interfaces/ITwoWayResolvedConflict.h"
#include "XMPCommon/BaseClasses/MemoryManagedObject.h"
#include "XMPCommon/ImplHeaders/SharedObjectImpl.h"

#if XMP_WinBuild
	#pragma warning( push )
	#pragma warning( disable : 4250 )
#endif

namespace AdobeXMPCompareAndMerge_Int {

	class TwoWayResolvedConflictImpl
		: public virtual ITwoWayResolvedConflict
		, public virtual SharedObjectImpl
		, public virtual MemoryManagedObject
	{
	public:
		TwoWayResolvedConflictImpl( IThreeWayResolvedConflict::eConflictResolutionReason reason, const spcINode & baseVersionNode,
			const spcINode & latestVersionNode, const spcINode & resolvedNode, IThreeWayResolvedConflict_base::eResolvedSource resolvedSource,
			const spcITwoWayConflictResolutionStrategy & strategyUsed );

		virtual spcINode APICALL GetBaseVersionNode() const;
		virtual IThreeWayResolvedConflict::eConflictResolutionReason APICALL GetConflictResolutionReason() const;
		virtual spcINode APICALL GetLatestVersionNode() const;
		virtual spcINode APICALL GetResolvedNode() const;
		virtual IThreeWayResolvedConflict::eResolvedSource APICALL GetResolvedSource() const;
		virtual spcITwoWayConflictResolutionStrategy APICALL GetStrategyUsed() const;
		virtual void APICALL ModifyResolvedNode( IThreeWayResolvedConflict::eResolvedSource resolvedSource, spcITwoWayConflictResolutionStrategy & strategyUsed, const spcINode & resolvedNode );


	protected:
		virtual ~TwoWayResolvedConflictImpl() __NOTHROW__ {}

		IThreeWayResolvedConflict::eConflictResolutionReason		mConflictResolutionReason;
		IThreeWayResolvedConflict::eResolvedSource					mResolvedSource;
		spcINode													mBaseVersionNode;
		spcINode													mLatestVersionNode;
		spcINode													mResolvedNode;
		spcITwoWayConflictResolutionStrategy						mStrategyUsed;

	#ifdef FRIEND_CLASS_DECLARATION
		FRIEND_CLASS_DECLARATION();
	#endif
		REQ_FRIEND_CLASS_DECLARATION();
	};
}

#endif  // __TwoWayResolvedConflictImpl_h__
