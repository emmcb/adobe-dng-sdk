#ifndef __TwoWayConflictResolutionStrategyForThreeWayConflictIdentification_h__
#define __TwoWayConflictResolutionStrategyForThreeWayConflictIdentification_h__ 1

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

#include "XMPCompareAndMerge/Interfaces/ITwoWayConflictResolutionStrategy.h"
#include "XMPCommon/BaseClasses/MemoryManagedObject.h"
#include "XMPCommon/ImplHeaders/SharedObjectImpl.h"

#if XMP_WinBuild
	#pragma warning( push )
	#pragma warning( disable : 4250 )
#endif

namespace AdobeXMPCompareAndMerge_Int {

	class TwoWayConflictResolutionStrategyForThreeWayConflictIdentification
		: public virtual ITwoWayConflictResolutionStrategy
		, public virtual SharedObjectImpl
		, public virtual MemoryManagedObject
	{
	public:
		

		static spITwoWayConflictResolutionStrategy CreateStrategy( const spcIMetadata & commonBaseMetadata );

	protected:
		TwoWayConflictResolutionStrategyForThreeWayConflictIdentification( const spcIMetadata & commonBaseMetadata );
		virtual ~TwoWayConflictResolutionStrategyForThreeWayConflictIdentification() __NOTHROW__{}

		bool compareValues( const spcINode & first, const spcINode & second, bool doBasicComparison = true,
			bool doQualifiersComparison = true, bool doValueComparison = true ) const;

		IThreeWayUnResolvedConflict::eConflictReason handleOnlyPresentInOneMetadataTree( bool presentInLatest,
			const spcINode & node, spcINode & resolvedNode, IThreeWayResolvedConflict::eConflictResolutionReason & reasonForConflictResolution,
			IThreeWayResolvedConflict::eResolvedSource & resolvedNodeSource ) const;

		IThreeWayUnResolvedConflict::eConflictReason handlePresentInBothButDifferent( bool doBasicComparison,
			const ITwoWayUnResolvedConflict::eTwoWayConflictReason reasonsForConflict, const spcINode & latestVersionNode,
			const spcINode & userVersion, spcINode & resolvedNode, IThreeWayResolvedConflict::eConflictResolutionReason & reasonForConflictResolution,
			IThreeWayResolvedConflict::eResolvedSource & resolvedNodeSource ) const;

		IThreeWayUnResolvedConflict::eConflictReason handleQualifiersPresentInOneNode( bool presentInLatest,
			const spcINode & node, spcINode & resolvedNode, IThreeWayResolvedConflict::eConflictResolutionReason & reasonForConflictResolution,
			IThreeWayResolvedConflict::eResolvedSource & resolvedNodeSource ) const;

		IThreeWayUnResolvedConflict::eConflictReason handleQualifiersPresentInBothButDifferent(
			const spcINode & latestVersionNode, const spcINode & userVersion, spcINode & resolvedNode,
			IThreeWayResolvedConflict::eConflictResolutionReason & reasonForConflictResolution,
			IThreeWayResolvedConflict::eResolvedSource & resolvedNodeSource ) const;

		IThreeWayUnResolvedConflict::eConflictReason handleSimpleIsUriFlagMismatch( bool latestNodeIsURI,
			const spcINode & latestVersionNode, const spcINode & userVersionNode, spcINode & resolvedNode,
			IThreeWayResolvedConflict::eConflictResolutionReason & reasonForConflictResolution,
			IThreeWayResolvedConflict::eResolvedSource & resolvedNodeSource ) const;

		virtual bool APICALL InterestedInResolvingConflict( const spcIUTF8String & nameSpace, const spcIUTF8String & name, INode::eNodeType nodeType, ITwoWayUnResolvedConflict::eTwoWayConflictReason reason, bool alreadyResolved ) const;

		virtual IThreeWayUnResolvedConflict::eConflictReason APICALL ResolveConflict( ITwoWayUnResolvedConflict::eTwoWayConflictReason reasonsForConflict, const spcINode & baseNode, const spcINode & currentNode, spcINode & resolvedNode, IThreeWayResolvedConflict::eConflictResolutionReason & reasonForConflictResolution, IThreeWayResolvedConflict::eResolvedSource & resolvedNodeSource ) const;

		spcIMetadata							mCommonBaseMetadata;
		spcITwoWayComparisonEngine				mComparisonEngine;

	#ifdef FRIEND_CLASS_DECLARATION
		FRIEND_CLASS_DECLARATION();
	#endif
		REQ_FRIEND_CLASS_DECLARATION();
	};
}

#endif  // __TwoWayConflictResolutionStrategyForThreeWayConflictIdentification_h__
