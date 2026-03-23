#ifndef __TwoWayValueComparisonResultImpl_h__
#define __TwoWayValueComparisonResultImpl_h__ 1

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

#include "XMPCompareAndMerge/Interfaces/ITwoWayValueComparisonResult.h"
#include "XMPCommon/BaseClasses/MemoryManagedObject.h"
#include "XMPCommon/ImplHeaders/SharedObjectImpl.h"

#if XMP_WinBuild
	#pragma warning( push )
	#pragma warning( disable : 4250 )
#endif

namespace AdobeXMPCompareAndMerge_Int {

	class TwoWayValueComparisonResultImpl
		: public virtual ITwoWayValueComparisonResult
		, public virtual SharedObjectImpl
		, public virtual MemoryManagedObject
	{
	public:
		TwoWayValueComparisonResultImpl();

		virtual double APICALL GetMatchingPercetage() const;
		virtual bool APICALL IsExactMatch() const;
		virtual void APICALL SetMatchedResultOfChild( const spcITwoWayValueComparisonResult & childResult );
		virtual void APICALL setExactlyMatched();
		virtual void APICALL setTotallyDifferent();

	protected:
		virtual ~TwoWayValueComparisonResultImpl() __NOTHROW__ {}

		void computePercentage() const;

		spITwoWayValueComparisonResults				mResultListOfChildren;
		mutable double								mPercentage;
		mutable bool								mComputed;

	#ifdef FRIEND_CLASS_DECLARATION
		FRIEND_CLASS_DECLARATION();
	#endif
		REQ_FRIEND_CLASS_DECLARATION();
	};
}

#if XMP_WinBuild
	#pragma warning( pop )
#endif

#endif  // __TwoWayValueComparisonResultImpl_h__
