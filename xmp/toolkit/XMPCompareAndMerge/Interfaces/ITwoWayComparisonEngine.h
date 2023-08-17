#ifndef __ITwoWayComparisonEngine_h__
#define __ITwoWayComparisonEngine_h__ 1

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


#include "XMPCompareAndMerge/XMPCompareAndMergeFwdDeclarations_I.h"
#include "XMPCommon/Interfaces/BaseInterfaces/ISharedObject_I.h"

namespace AdobeXMPCompareAndMerge_Int {

#if XMP_WinBuild
	#pragma warning( push )
	#pragma warning( disable : 4250 )
#endif

	//!
	//! \brief Internal interface that represents a two way comparison engine.
	//! It provides all functions to
	//!		- check for existence of nodes.
	//!		- compare basic properties of nodes.
	//!		- compare values of nodes.
	//!		- compare qualifiers.
	//!		- compare unsorted arrays.
	//!
	class ITwoWayComparisonEngine
		:public virtual ISharedObject_I
	{
	public:

		virtual bool APICALL IsEquivalentNodePresentInOtherTree( const spcINode & nodeInBaseTree, const spcICompositeNode & otherTreeParentNode,
			spcINode & equivalentNodeInOtherTree ) const = 0;
		virtual bool APICALL IsEquivalentNodePresentInOtherTree(const spcINode & nodeInBaseTree, const spcIMetadata & otherTreeDOM,
			spcINode & equivalentNodeInOtherTree) const = 0;
		virtual bool APICALL IsEquivalentQualifierPresentInOtherTree( const spcINode & qualifierInBaseTree, const spcINode & otherTreeHostNode,
			spcINode & equivalentQualifierInOtherTree ) const = 0;
		virtual spITwoWayBasicComparisonResult APICALL CompareBasicProperties( const spcINode & base, const spcINode & latest ) const = 0;
		virtual spITwoWayBasicComparisonResult APICALL CompareBasicProperties( const spcISimpleNode & base, const spcISimpleNode & latest ) const = 0;
		virtual spITwoWayBasicComparisonResult APICALL CompareBasicProperties( const spcIArrayNode & base, const spcIArrayNode & latest ) const = 0;
		virtual spITwoWayBasicComparisonResult APICALL CompareBasicProperties( const spcIStructureNode & base, const spcIStructureNode & latest ) const = 0;

		virtual spITwoWayValueComparisonResult APICALL CompareValues( const spcINode & base, const spcINode & latest ) const = 0;
		virtual spITwoWayValueComparisonResult APICALL CompareQualifiers( const spcINode & base, const spcINode & latest ) const = 0;

		virtual spITwoWayValueComparisonResultMatrix APICALL CreateAllCombinationsOfCompareValuesForArrayElements( const spcIArrayNode & base,
			const spcIArrayNode & latest ) const = 0;

		virtual void APICALL SetStopOnFirstUniqueNodeEncountered( bool value ) = 0;
		virtual void APICALL SetStopOnFirstDifferentValueEncountered( bool value ) = 0;
		virtual void APICALL SetStopCreatingAllCombinationsOfCompareValuesForAnArrayElementAfterExactMatchEncountered( bool value ) = 0;

		// static factory functions
		XMP_PRIVATE static spITwoWayComparisonEngine CreateTwoWayComparisonEngine();

	protected:
		virtual ~ITwoWayComparisonEngine() __NOTHROW__ {}

	#ifdef FRIEND_CLASS_DECLARATION
		FRIEND_CLASS_DECLARATION();
	#endif
		REQ_FRIEND_CLASS_DECLARATION();
	};

#if XMP_WinBuild
	#pragma warning( pop )
#endif

}

#endif  // __ITwoWayComparisonEngine_h__
