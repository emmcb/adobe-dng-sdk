#ifndef __ITwoWayConflictIdentificationEngine_h__
#define __ITwoWayConflictIdentificationEngine_h__ 1

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
	//! \brief Internal interface that represents a conflict identification engine for a 2 way comparison.
	//!	\details Provides all the functions to
	//!		- Identify all the conflicts between base and latest metadata objects.
	//!		- Identify all the conflicts between a node in the base and latest metadata objects.
	//!
	class ITwoWayConflictIdentificationEngine
		:public virtual ISharedObject_I
	{
	public:

		virtual spITwoWayUnResolvedConflicts IdentifyConflicts( const spcIMetadata & baseMetadata, const spcIMetadata & latestMetadata ) = 0;
		virtual spITwoWayUnResolvedConflicts IdentifyConflicts( const spcINode & baseNode, const spcINode & latestNode ) = 0;

		// static factory functions

		//!
		//! Creates an ITwoWayConflictIdentificationEngine object.
		//! \return a shared pointer to an ITwoWayConflictIdentificationEngine object
		//!
		XMP_PRIVATE static spITwoWayConflictIdentificationEngine CreateTwoWayConflictIdentificationEngine();

	protected:
		virtual ~ITwoWayConflictIdentificationEngine() __NOTHROW__ {}

	#ifdef FRIEND_CLASS_DECLARATION
		FRIEND_CLASS_DECLARATION();
	#endif
		REQ_FRIEND_CLASS_DECLARATION();
	};

#if XMP_WinBuild
	#pragma warning( pop )
#endif

}
#endif  // __ITwoWayConflictIdentificationEngine_h__
