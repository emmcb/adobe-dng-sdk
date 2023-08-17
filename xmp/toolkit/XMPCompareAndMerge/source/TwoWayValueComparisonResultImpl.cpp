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
#if WIN_ENV
	#pragma warning( push )
	#pragma warning(disable:4503)
#endif

#define IMPLEMENTATION_HEADERS_CAN_BE_INCLUDED 1
	#include "XMPCompareAndMerge/ImplHeaders/TwoWayValueComparisonResultImpl.h"
#undef IMPLEMENTATION_HEADERS_CAN_BE_INCLUDED

#include "XMPCommon/Interfaces/IError_I.h"
#include "XMPCommon/Utilities/TSmartPointers_I.h"

namespace AdobeXMPCompareAndMerge_Int {

	TAllocator< spITwoWayValueComparisonResult > gTAllocatorTwoWayValueComparisonResult;
	TAllocator< spITwoWayValueComparisonResults > gTAllocatorTwoWayValueComparisonResults;

	TwoWayValueComparisonResultImpl::TwoWayValueComparisonResultImpl()
		: mComputed( true )
		, mPercentage( 0 ) { }

	void TwoWayValueComparisonResultImpl::computePercentage() const {
		if ( !mComputed ) {
			if ( mResultListOfChildren ) {
				mPercentage = 0;
				for ( size_t i = 0, nChild = mResultListOfChildren->size(); i < nChild; i++ ) {
					mPercentage += ( *mResultListOfChildren )[ i ]->GetMatchingPercetage() / nChild;
				}
			}
			mComputed = true;
		}
	}

	double APICALL TwoWayValueComparisonResultImpl::GetMatchingPercetage() const {
		if ( mComputed )
			return mPercentage;
		computePercentage();
		return mPercentage;
	}

	bool APICALL TwoWayValueComparisonResultImpl::IsExactMatch() const {
		if ( !mComputed )
			computePercentage();
		return ( ( mPercentage < 100.0001 ) && ( mPercentage > 99.9999 ) );
	}

	void APICALL TwoWayValueComparisonResultImpl::SetMatchedResultOfChild( const spcITwoWayValueComparisonResult & childResult ) {
		if ( !mResultListOfChildren )
			mResultListOfChildren = CreateTwoWayValueComparisonResults();
		mResultListOfChildren->push_back( childResult );
		mComputed = false;
	}

	void TwoWayValueComparisonResultImpl::setExactlyMatched() {
		mPercentage = 100.0;
		mComputed = true;
	}

	void TwoWayValueComparisonResultImpl::setTotallyDifferent() {
		mPercentage = 0.0;
		mComputed = true;
	}

	spITwoWayValueComparisonResult ITwoWayValueComparisonResult::CreateExactlyMatchedResult() {
		auto sp = MakeUncheckedSharedPointer( new TwoWayValueComparisonResultImpl(), __FILE__, __LINE__, true );
		sp->setExactlyMatched();
		return sp;
	}

	spITwoWayValueComparisonResult ITwoWayValueComparisonResult::CreateExactlyMisMatchedResult() {
		auto sp = MakeUncheckedSharedPointer( new TwoWayValueComparisonResultImpl(), __FILE__, __LINE__, true );
		sp->setTotallyDifferent();
		return sp;
	}

	spITwoWayValueComparisonResults ITwoWayValueComparisonResult::CreateTwoWayValueComparisonResults() {
		return MakeUncheckedSharedPointerWithDefaultDelete( new ITwoWayValueComparisonResults( gTAllocatorTwoWayValueComparisonResult ),
			__FILE__, __LINE__, true );
	}

	spITwoWayValueComparisonResultMatrix ITwoWayValueComparisonResult::CreateTwoWayValueComparisonResultMatrix() {
		return MakeUncheckedSharedPointerWithDefaultDelete( new ITwoWayValueComparisonResultMatrix(), __FILE__, __LINE__, true );
	}

}
