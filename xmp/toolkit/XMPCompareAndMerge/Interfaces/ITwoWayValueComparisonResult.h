#ifndef __ITwoWayValueComparisonResult_h__
#define __ITwoWayValueComparisonResult_h__ 1

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
	//! \brief Internal interface that represents an object which takes care of value comparisons.
	//!	\details Provides all the functions to
	//!		- get matching percentage and other aspects of the value comparison.
	//!		- allows to set the various properties of the value comparison operations.
	//!
	class ITwoWayValueComparisonResult
		:public virtual ISharedObject_I
	{
	public:

		//!
		//! Get the present matching percentage of the value comparison.
		//! \return a double value to indicate the matching percentage.
		//!
		virtual double APICALL GetMatchingPercetage() const = 0;

		//!
		//! Informs whether there is exact match or not.
		//! \return a boolean value; true in case the values match exactly, false otherwise.
		//!
		virtual bool APICALL IsExactMatch() const = 0;

		//!
		//! Submits the value comparison result of the child node to be considered.
		//! \param[in] childResult a shared pointer to a const object of type #ITwoWayValueComparisonResult having
		//! the comparison result of the child node.
		//!
		virtual void APICALL SetMatchedResultOfChild( const spcITwoWayValueComparisonResult & childResult ) = 0;

		// static factory functions

		//!
		//! Creates an exactly matched value comparison object.
		//! \return a shared pointer to an object of type  #ITwoWayValueComparisonResult whose state is set to exactly matched.
		//!
		static spITwoWayValueComparisonResult CreateExactlyMatchedResult();

		//!
		//! Creates an exactly mismatched value comparison object.
		//! \return a shared pointer to an object of type  #ITwoWayValueComparisonResult whose state is set to exactly mis matched.
		//!
		static spITwoWayValueComparisonResult CreateExactlyMisMatchedResult();

		//!
		//! Creates an empty list of value comparison objects.
		//! \return a shared pointer to #ITwoWayValueComparisonResults object.
		//!
		static spITwoWayValueComparisonResults CreateTwoWayValueComparisonResults();

		//!
		//! Creates an empty list of list of value comparison objects.
		//! \return a shared pointer to #ITwoWayValueComparisonResultMatrix object.
		//!
		static spITwoWayValueComparisonResultMatrix CreateTwoWayValueComparisonResultMatrix();

	protected:
		virtual ~ITwoWayValueComparisonResult() __NOTHROW__ {}

		//!
		//! Reset the result to be exactly matched.
		//!
		virtual void APICALL setExactlyMatched() = 0;

		//!
		//! Reset the result to be exactly mismatched.
		virtual void APICALL setTotallyDifferent() = 0;


	#ifdef FRIEND_CLASS_DECLARATION
		FRIEND_CLASS_DECLARATION();
	#endif
		REQ_FRIEND_CLASS_DECLARATION();
	};

#if XMP_WinBuild
	#pragma warning( pop )
#endif

}

#endif  // __ITwoWayValueComparisonResult_h__
