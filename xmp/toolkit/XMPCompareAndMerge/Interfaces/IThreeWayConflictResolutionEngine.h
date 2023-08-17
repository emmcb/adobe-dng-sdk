#ifndef __IThreeWayConflictResolutionEngine_h__
#define __IThreeWayConflictResolutionEngine_h__ 1

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
	//! \brief Internal interface that represents a 3 way conflict resolution engine.
	//!	\details Provides all the functions to
	//!		- Register conflict resolution strategies.
	//!		- Resolve conflicts using the strategies registered with the object.
	//!
	//!
	class IThreeWayConflictResolutionEngine
		:public virtual ISharedObject_I
	{
	public:

		virtual bool RegisterResolutionStrategy( const spIThreeWayStrategy & strategy ) = 0;

		virtual sizet ResolveConflicts( const spIThreeWayUnResolvedConflicts & conflictList, const spIThreeWayResolvedConflicts & resolvedList ) const = 0;

		// static factory functions

		//!
		//! Creates an IThreeWayConflictResolutionEngine object.
		//! \return a shared pointer to an IThreeWayConflictResolutionEngine object
		//!
		XMP_PRIVATE static spIThreeWayConflictResolutionEngine CreateThreeWayConflictResolutionEngine();

	protected:
		virtual ~IThreeWayConflictResolutionEngine() __NOTHROW__ {}

	#ifdef FRIEND_CLASS_DECLARATION
		FRIEND_CLASS_DECLARATION();
	#endif
		REQ_FRIEND_CLASS_DECLARATION();
	};

#if XMP_WinBuild
	#pragma warning( pop )
#endif

}

#endif  // __IThreeWayConflictResolutionEngine_h__
