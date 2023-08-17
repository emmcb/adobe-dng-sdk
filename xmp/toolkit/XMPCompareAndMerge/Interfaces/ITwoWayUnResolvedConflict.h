#ifndef __ITwoWayUnResolvedConflict_h__
#define __ITwoWayUnResolvedConflict_h__ 1

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

#include "XMPCompareAndMerge/Interfaces/IThreeWayUnResolvedConflict.h"

namespace AdobeXMPCompareAndMerge_Int {

#if XMP_WinBuild
	#pragma warning( push )
	#pragma warning( disable : 4250 )
#endif

	//!
	//! \brief Internal interface that represents a conflict that has not been resolved by applying one of the strategies.
	//! Provides all the functions to get all the information regarding the conflict.
	//!
	class ITwoWayUnResolvedConflict
		:public virtual ISharedObject_I
	{
	public:

		//!
		//! An enum to indicate the reason for mismatch in the two nodes.
		//!
		typedef enum {
			//! No conflict, nodes in the both base and latest metadata objects are same.
			kTWCRNone									= 0x0,

			//! Node only present in the base metadata object.
			kTWCROnlyPresentInBase						= 0x1,

			//! Node only present in the latest metadata object.
			kTWCROnlyPresentInLatest					= 0x2,

			//! Node type in the base and the latest metadata objects are different.
			kTWCRTypesMismatch							= 0x4,

			//! Values of the node in the base and latest metadata objects are different.
			kTWCRValuesMismatch							= 0x08,


			//! Qualifiers of the node are only present in the base metadata object.
			kTWCRQualifiersOnlyPresentInBase			= 0x10,

			//! Qualifiers of the node are only present in the latest metadata object.
			kTWCRQualifiersOnlyPresentInLatest			= 0x20,

			//! Values of the qualifier node are not same in the base and the latest metadata object.
			kTWCQualifiersValueMismatch					= 0x40,

			
			//! Simple Node in the base metadata object is only marked as URI.
			kTWCRSimpleNodeIsURIOnlyInBase				= 0x100,

			//! Simple Node in the latest metadata object is only marked as URI.
			kTWCRSimpleNodeIsURIOnlyInLatest			 = 0x200,


			//! Types of the array node in the base and the latest metadata objects are different.
			kTWCRArrayTypesMismatch						= 0x1000,

			//! Size of the array nodes in the base and the latest metadata objects are different.
			kTWCRArraySizeMismatch						= 0x2000,

			//! Types of elements stored in the array nodes in the base and the latest metadata objects are different.
			kTWCRArrayElementTypesMismatch				= 0x4000,


			//! Size of the structure nodes in the base and the latest metadata objects are different.
			kTWCRStructureSizeMismatch					= 0x10000,


			//! About URI only present in the base metadata object.
			kTWCRAboutURIPresentOnlyInBase				= 0x100000,

			//! About URI only present in the latest metadata object.
			kTWCRAboutURIPresentOnlyInLatest			= 0x200000,

			//! About URI values in the base and the latest metadata object are different.
			kTWCRAboutURIValueMismatch					= 0x400000,

			kTWCRMaxSize								= kMaxEnumValue
		} eTwoWayConflictReason;

		//!
		//! Get the reason for the two conflict.
		//! \return a value of type #eTwoWayConflictReason indicating the reason for the conflict in two way comparison.
		//!
		virtual eTwoWayConflictReason APICALL GetTwoWayConflictReason() const = 0;

		//!
		//! Get the equivalent reason for the conflict in case of 3 way comparison.
		//! \return a value of #IThreeWayUnResolvedConflict::eConflictReason indicating the reason for the conflict.
		//!
		virtual IThreeWayUnResolvedConflict::eConflictReason APICALL GetConflictReason() const = 0;

		//!
		//! Get the conflicting node as it is present in the base metadata object.
		//! \return a shared pointer to const #AdobeXMPCore::INode interface.
		//!
		virtual spcINode APICALL GetBaseVersionNode() const = 0;

		//!
		//! Get the conflicting node as it is present in the latest metadata object.
		//! \return a shared pointer to const #AdobeXMPCore::INode interface.
		//!
		virtual spcINode APICALL GetLatestVersionNode() const = 0;
		
		//!
		//! Allows to set the conflict reason in case of 3 way comparison.
		//! \param[in] reason a value of #IThreeWayUnResolvedConflict::eConflictReason indicating the reason for the conflict.
		//!
		virtual void APICALL SetConflictReason( IThreeWayUnResolvedConflict::eConflictReason reason ) = 0;

		// static factory functions

		//!
		//! Creates an ITwoWayUnResolvedConflict object.
		//! \param[in] conflictReason a value of #IThreeWayUnResolvedConflict::eConflictReason indicating the reason for the conflict.
		//! \param[in] baseVersionNode a shared pointer to const #AdobeXMPCore::INode object pointing to node in base metadata object.
		//! \param[in] latestVersionNode a shared pointer to const #AdobeXMPCore::INode object pointing to node in latest metadata object.
		//! \return a shared pointer to an ITwoWayUnResolvedConflict object
		//!
		XMP_PRIVATE static spITwoWayUnResolvedConflict CreateTwoWayUnResolvedConflict( eTwoWayConflictReason conflictReason, const spcINode & baseVersionNode, 
			const spcINode & latestVersionNode );

		//!
		//! Creates an empty list of UnResolved conflicts for a 2 way merge.
		//! \returns a shared pointer to #ITwoWayUnResolvedConflicts object.
		//!
		XMP_PRIVATE static spITwoWayUnResolvedConflicts CreateTwoWayUnResolvedConflicts();

	protected:
		virtual ~ITwoWayUnResolvedConflict() __NOTHROW__ {}

	#ifdef FRIEND_CLASS_DECLARATION
		FRIEND_CLASS_DECLARATION();
	#endif
		REQ_FRIEND_CLASS_DECLARATION();
	};

#if XMP_WinBuild
	#pragma warning( pop )
#endif

}

#endif  // __ITwoWayUnResolvedConflict_h__
