#ifndef __ITwoWayConflictResolutionStrategy_h__
#define __ITwoWayConflictResolutionStrategy_h__ 1

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

#include "XMPCompareAndMerge/Interfaces/ITwoWayUnResolvedConflict.h"
#include "XMPCompareAndMerge/Interfaces/IThreeWayResolvedConflict.h"
#include "XMPCompareAndMerge/Interfaces/IThreeWayUnResolvedConflict.h"

#include "XMPCore/Interfaces/INode.h"

namespace AdobeXMPCompareAndMerge_Int {

#if XMP_WinBuild
	#pragma warning( push )
	#pragma warning( disable : 4250 )
#endif

	//!
	//! \brief Internal interface that represents a two way strategy object.
	//!	\details Provides all the functions to
	//!		- let the conflict resolution engine know whether a strategy is interested in resolving.
	//!		- resolve the conflict if possible when invoked by the resolution engine.
	//!
	class ITwoWayConflictResolutionStrategy
		:public virtual ISharedObject_I
	{
	public:

		virtual bool APICALL InterestedInResolvingConflict( const spcIUTF8String & nameSpace, const spcIUTF8String & name, INode::eNodeType nodeType,
			ITwoWayUnResolvedConflict::eTwoWayConflictReason reason, bool alreadyResolved ) const = 0;

		virtual IThreeWayUnResolvedConflict::eConflictReason APICALL ResolveConflict( ITwoWayUnResolvedConflict::eTwoWayConflictReason reasonsForConflict,
			const spcINode & baseNode, const spcINode & currentNode, spcINode & resolvedNode, IThreeWayResolvedConflict::eConflictResolutionReason & reasonForConflictResolution,
			IThreeWayResolvedConflict::eResolvedSource & resolvedNodeSource ) const = 0;

	protected:
		virtual ~ITwoWayConflictResolutionStrategy() __NOTHROW__ {}

	#ifdef FRIEND_CLASS_DECLARATION
		FRIEND_CLASS_DECLARATION();
	#endif
		REQ_FRIEND_CLASS_DECLARATION();
	};

#if XMP_WinBuild
	#pragma warning( pop )
#endif

}

#endif  // __ITwoWayConflictResolutionStrategy_h__
