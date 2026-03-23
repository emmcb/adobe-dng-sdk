#ifndef __TwoWayComparisonEngineImpl_h__
#define __TwoWayComparisonEngineImpl_h__ 1

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

#include "XMPCompareAndMerge/Interfaces/ITwoWayComparisonEngine.h"
#include "XMPCommon/BaseClasses/MemoryManagedObject.h"
#include "XMPCommon/ImplHeaders/SharedObjectImpl.h"

#if XMP_WinBuild
	#pragma warning( push )
	#pragma warning( disable : 4250 )
#endif

namespace AdobeXMPCompareAndMerge_Int {

	class TwoWayComparisonEngineImpl
		: public virtual ITwoWayComparisonEngine
		, public virtual SharedObjectImpl
		, public virtual MemoryManagedObject
	{
	public:
		TwoWayComparisonEngineImpl();

		virtual spITwoWayBasicComparisonResult APICALL CompareBasicProperties( const spcIArrayNode & base, const spcIArrayNode & latest ) const;
		virtual spITwoWayBasicComparisonResult APICALL CompareBasicProperties( const spcINode & base, const spcINode & latest ) const;
		virtual spITwoWayBasicComparisonResult APICALL CompareBasicProperties( const spcISimpleNode & base, const spcISimpleNode & latest ) const;
		virtual spITwoWayBasicComparisonResult APICALL CompareBasicProperties( const spcIStructureNode & base, const spcIStructureNode & latest ) const;
		virtual spITwoWayValueComparisonResult APICALL CompareValues( const spcINode & base, const spcINode & latest ) const;
		virtual spITwoWayValueComparisonResult APICALL CompareQualifiers( const spcINode & base, const spcINode & latest ) const;
		virtual spITwoWayValueComparisonResultMatrix APICALL CreateAllCombinationsOfCompareValuesForArrayElements( const spcIArrayNode & base, const spcIArrayNode & latest ) const;
		virtual bool APICALL IsEquivalentNodePresentInOtherTree( const spcINode & nodeInBaseTree, const spcICompositeNode & otherTreeParentNode, spcINode & equivalentNodeInOtherTree ) const;
		//comment
		virtual bool APICALL IsEquivalentNodePresentInOtherTree(const spcINode & nodeInBaseTree, const spcIMetadata & otherTreeDOM,
			spcINode & equivalentNodeInOtherTree) const;
		virtual bool APICALL IsEquivalentQualifierPresentInOtherTree( const spcINode & qualifierInBaseTree, const spcINode & otherTreeHostNode, spcINode & equivalentQualifierInOtherTree ) const;
		virtual void APICALL SetStopCreatingAllCombinationsOfCompareValuesForAnArrayElementAfterExactMatchEncountered( bool value );
		virtual void APICALL SetStopOnFirstDifferentValueEncountered( bool value );
		virtual void APICALL SetStopOnFirstUniqueNodeEncountered( bool value );

	protected:
		virtual ~TwoWayComparisonEngineImpl() __NOTHROW__ {}

		spITwoWayValueComparisonResult compareValues( const spcISimpleNode & base, const spcISimpleNode & latest ) const;
		spITwoWayValueComparisonResult compareValues( const spcIArrayNode & base, const spcIArrayNode & latest ) const;
		spITwoWayValueComparisonResult compareValues( const spcIStructureNode & base, const spcIStructureNode & latest ) const;

		spITwoWayValueComparisonResult compareUnOrderedArrays( const spcIArrayNode & base, const spcIArrayNode & latest ) const;
		spITwoWayValueComparisonResult compareOrderedArrays( const spcIArrayNode & base, const spcIArrayNode & latest ) const;
		spITwoWayValueComparisonResult compareAlternateArrays( const spcIArrayNode & base, const spcIArrayNode & latest ) const;

		mutable bool			mStopOnFirstUniqueNodeEncountered;
		bool					mStopOnFirstDifferentValueEncountered;
		bool					mStopCreatingAllCombinationsOfCompareValuesForAnArrayElementAfterExactMatchEncountered;

	#ifdef FRIEND_CLASS_DECLARATION
		FRIEND_CLASS_DECLARATION();
	#endif
		REQ_FRIEND_CLASS_DECLARATION();
	};
}

#endif  // __TwoWayComparisonEngineImpl_h__
