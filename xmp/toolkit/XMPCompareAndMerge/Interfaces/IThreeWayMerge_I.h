#ifndef __IThreeWayMerge_I_h__
#define __IThreeWayMerge_I_h__ 1

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
#include "XMPCompareAndMerge/Interfaces/IThreeWayMerge.h"
#include "XMPCommon/Interfaces/BaseInterfaces/ISharedObject_I.h"

namespace AdobeXMPCompareAndMerge_Int {

#if XMP_WinBuild
	#pragma warning( push )
	#pragma warning( disable : 4250 )
#endif

	//!
	//! \brief Internal interface that represents an object which performs 3 way Merge.
	//! Provides all the functions
	//!		- to perform 3 way merge.
	//!		- to register 3 way conflict resolution strategies for custom conflict resolution.
	//!
	class IThreeWayMerge_I
		: public virtual IThreeWayMerge
		, public virtual ISharedObject_I
	{
	public:

		//!
		//! Allows clients to plug in their own conflict resolution strategies.
		//! \param[in] strategy a raw pointer to #IThreeWayStrategy object which will be used by the conflict resolution engine
		//! to resolve the conflict.
		//!
		using IThreeWayMerge::RegisterThreeWayStrategy;
		virtual bool APICALL RegisterThreeWayStrategy( const spIThreeWayStrategy & strategy ) = 0;

		virtual bool APICALL RegisterThreeWayStrategy( pIThreeWayStrategy_base strategy );
		virtual bool APICALL RegisterThreeWayGenericStrategy( const spIThreeWayGenericStrategy & strategy );

		//!
		//! return the version of the interface.
		//! \return 32 bit unsigned integer representing the version of the interface.
		//!
		static uint32 GetInterfaceVersion() { return kInternalInterfaceVersionNumber; }

		virtual pIThreeWayMerge APICALL GetActualIThreeWayMerge() __NOTHROW__ { return this; }
		virtual pIThreeWayMerge_I APICALL GetIThreeWayMerge_I() __NOTHROW__ { return this; }

		virtual pvoid APICALL GetInterfacePointer( uint64 interfaceID, uint32 interfaceVersion );

	protected:
		virtual ~IThreeWayMerge_I() __NOTHROW__ {}
		pvoid APICALL GetInterfacePointerInternal( uint64 interfaceID, uint32 interfaceVersion, bool isTopLevel );

		virtual pvoid APICALL getInterfacePointer( uint64 interfaceID, uint32 interfaceVersion, pcIError_base & error ) __NOTHROW__;
		virtual pcIThreeWayMergeResult_base APICALL merge( pcIMetadata_base baseVersionMetadata, pcIMetadata_base latestVersionMedata, pcIMetadata_base userVersionMetadata, pcIError_base & error ) const __NOTHROW__;
		virtual pcIThreeWayMergeResult_base APICALL autoMerge ( pcIMetadata_base baseVersionMetadata, pcIMetadata_base latestVersionMedata, pcIMetadata_base userVersionMetadata, uint32 skipAutoMergeStrategy, pcIError_base & error ) const __NOTHROW__;
		virtual pcIUTF8String_base APICALL autoMerge ( const char * baseVersionMetadata, sizet baseVersionMetadataLength, const char * latestVersionMetadata, sizet latestVersionMetadataLength, const char * userVersionMetadata, sizet userVersionMetadataLength, uint32 & isMerged, uint32 skipAutoMergeStrategy, pcIError_base & error ) const __NOTHROW__;

        virtual XMPMetaRef APICALL autoMerge ( XMPMetaRef baseVersionMetadata, XMPMetaRef latestVersionMetadata, XMPMetaRef userVersionMetadata, uint32 & isMerged, uint32 skipAutoMergeStrategy, pcIError_base & error ) const __NOTHROW__;

		virtual uint32 APICALL registerThreeWayGenericStrategy( pIThreeWayGenericStrategy_base strategy, pcIError_base & error ) __NOTHROW__;
		virtual uint32 APICALL registerThreeWayStrategy( pIThreeWayStrategy_base strategy, pcIError_base & error ) __NOTHROW__;

	#ifdef FRIEND_CLASS_DECLARATION
		FRIEND_CLASS_DECLARATION();
	#endif
		REQ_FRIEND_CLASS_DECLARATION();
	};

#if XMP_WinBuild
	#pragma warning( pop )
#endif

}

#endif  // __IThreeWayMerge_I_h__
