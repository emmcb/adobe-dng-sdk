#ifndef __IThreeWayConflictIdentificationEngine_h__
#define __IThreeWayConflictIdentificationEngine_h__ 1

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
	//! \brief Internal interface that represents a conflict identification engine for a 3 way comparison.
	//!	\details Provides all the functions to
	//!		- Identify all the conflicts between base and latest metadata objects.
	//!
	class IThreeWayConflictIdentificationEngine
		:public virtual ISharedObject_I
	{
	public:

		virtual ThreeWayUnResolvedResolvedConflicts IdentifyConflicts( const spcIMetadata & baseVersionMetadata, const spcIMetadata & latestVersionMetadata,
			const spcIMetadata & userVersionMetadata ) const = 0;

		// static factory functions

		//!
		//! Creates an IThreeWayConflictIdentificationEngine object.
		//! \return a shared pointer to an IThreeWayConflictIdentificationEngine object
		//!
		XMP_PRIVATE static spIThreeWayConflictIdentificationEngine CreateThreeWayConflictIdentificationEngine();

	protected:
		virtual ~IThreeWayConflictIdentificationEngine() __NOTHROW__ {}

	#ifdef FRIEND_CLASS_DECLARATION
		FRIEND_CLASS_DECLARATION();
	#endif
		REQ_FRIEND_CLASS_DECLARATION();
	};

#if XMP_WinBuild
	#pragma warning( pop )
#endif

}

#endif  // __IThreeWayConflictIdentificationEngine_h__
