#ifndef __TwoWayConflictResolutionEngine_Impl_h__
#define __TwoWayConflictResolutionEngine_Impl_h__ 1

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

#include "XMPCompareAndMerge/Interfaces/ITwoWayConflictResolutionEngine.h"
#include "XMPCommon/BaseClasses/MemoryManagedObject.h"
#include "XMPCommon/ImplHeaders/SharedObjectImpl.h"

#if XMP_WinBuild
	#pragma warning( push )
	#pragma warning( disable : 4250 )
#endif

namespace AdobeXMPCompareAndMerge_Int {

	class TwoWayConflictResolutionEngineImpl
		: public virtual ITwoWayConflictResolutionEngine
		, public virtual SharedObjectImpl
		, public virtual MemoryManagedObject
	{
	public:
		TwoWayConflictResolutionEngineImpl();

		virtual bool APICALL RegisterResolutionStrategy( const spcITwoWayConflictResolutionStrategy & strategy );
		virtual sizet ResolveConflicts( spITwoWayUnResolvedConflicts & conflictList, spITwoWayResolvedConflicts & resolvedList ) const;

	protected:
		virtual ~TwoWayConflictResolutionEngineImpl() __NOTHROW__ {}

		spITwoWayConflictResolutionStrategies			mStrategies;

	#ifdef FRIEND_CLASS_DECLARATION
		FRIEND_CLASS_DECLARATION();
	#endif
		REQ_FRIEND_CLASS_DECLARATION();
	};
}

#endif  // __TwoWayConflictResolutionEngine_Impl_h__
