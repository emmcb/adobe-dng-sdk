#ifndef __ThreeWayUnResolvedConflictImpl_h__
#define __ThreeWayUnResolvedConflictImpl_h__ 1

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

#include "XMPCompareAndMerge/Interfaces/IThreeWayUnResolvedConflict_I.h"
#include "XMPCommon/BaseClasses/MemoryManagedObject.h"
#include "XMPCommon/ImplHeaders/SharedObjectImpl.h"

#if XMP_WinBuild
	#pragma warning( push )
	#pragma warning( disable : 4250 )
#endif

namespace AdobeXMPCompareAndMerge_Int {

	class ThreeWayUnResolvedConflictImpl
		: public virtual IThreeWayUnResolvedConflict_I
		, public virtual SharedObjectImpl
		, public virtual MemoryManagedObject
	{
	public:
		ThreeWayUnResolvedConflictImpl( eConflictReason conflictReason, const spcINode & baseVersionNode,
			const spcINode & latestVersionNode, const spcINode & userVersionNode );

		virtual spcINode APICALL GetBaseVersionNode() const;
		virtual eConflictReason APICALL GetConflictReason() const;
		virtual spcINode APICALL GetLatestVersionNode() const;
		virtual spcINode APICALL GetUserVersionNode() const;

		virtual void APICALL SetConflictReason( eConflictReason reason );

	protected:
		virtual ~ThreeWayUnResolvedConflictImpl() __NOTHROW__ {}

		void setConflictReason( IThreeWayUnResolvedConflict::eConflictReason reason );

		spcINode					mBaseVersionNode;
		spcINode					mLatestVersionNode;
		spcINode					mUserVersionNode;
		eConflictReason				mConflictReason;

	#ifdef FRIEND_CLASS_DECLARATION
		FRIEND_CLASS_DECLARATION();
	#endif
		REQ_FRIEND_CLASS_DECLARATION();
	};
}

#endif  // __ThreeWayUnResolvedConflictImpl_h__
