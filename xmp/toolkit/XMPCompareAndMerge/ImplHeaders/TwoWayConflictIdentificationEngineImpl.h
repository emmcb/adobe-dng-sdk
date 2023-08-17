#ifndef __TwoWayConflictIdentificationEngineImpl_h__
#define __TwoWayConflictIdentificationEngineImpl_h__ 1

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

#include "XMPCompareAndMerge/Interfaces/ITwoWayConflictIdentificationEngine.h"
#include "XMPCommon/BaseClasses/MemoryManagedObject.h"
#include "XMPCommon/ImplHeaders/SharedObjectImpl.h"

#include "XMPCompareAndMerge/Interfaces/ITwoWayUnResolvedConflict.h"

#if XMP_WinBuild
	#pragma warning( push )
	#pragma warning( disable : 4250 )
#endif

namespace AdobeXMPCompareAndMerge_Int {

	class TwoWayConflictIdentificationEngineImpl
		: public virtual ITwoWayConflictIdentificationEngine
		, public virtual SharedObjectImpl
		, public virtual MemoryManagedObject
	{
	public:
		TwoWayConflictIdentificationEngineImpl();

		virtual spITwoWayUnResolvedConflicts IdentifyConflicts( const spcIMetadata & baseMetadata, const spcIMetadata & latestMetadata );
		virtual spITwoWayUnResolvedConflicts IdentifyConflicts( const spcINode & baseNode, const spcINode & latestNode );

	protected:
		virtual ~TwoWayConflictIdentificationEngineImpl() __NOTHROW__ {}

		void identifyConflicts( const spcIArrayNode & baseArrayNode, const spcIArrayNode & latestArrayNode,
			ITwoWayUnResolvedConflict::eTwoWayConflictReason flagsToBeAppended, spITwoWayUnResolvedConflicts & conflictList );
		void identifyConflicts( const spcISimpleNode & baseSimpleNode, const spcISimpleNode & latestSimpleNode,
			ITwoWayUnResolvedConflict::eTwoWayConflictReason flagsToBeAppended, spITwoWayUnResolvedConflicts & conflictList );
		void identifyConflicts( const spcIStructureNode & baseStructNode, const spcIStructureNode & latestStructNode,
			ITwoWayUnResolvedConflict::eTwoWayConflictReason flagsToBeAppended, spITwoWayUnResolvedConflicts & conflictList );
		void identifyConflicts( spcINodeIterator & baseIterator, const spcINode & latestNode, spITwoWayUnResolvedConflicts & conflictList );
		void identifyConflictsForStructNode( spcINodeIterator & baseIterator, const spcIStructureNode & latestStructureNode, spITwoWayUnResolvedConflicts & conflictList );

		spITwoWayComparisonEngine				mComparisonEngine;

	#ifdef FRIEND_CLASS_DECLARATION
		FRIEND_CLASS_DECLARATION();
	#endif
		REQ_FRIEND_CLASS_DECLARATION();
	};
}

#endif  // __TwoWayConflictIdentificationEngineImpl_h__
