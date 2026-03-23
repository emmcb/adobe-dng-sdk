#ifndef IAssetUtilities_I_h__
#define IAssetUtilities_I_h__ 1

// =================================================================================================
// ADOBE SYSTEMS INCORPORATED
// Copyright 2014 Adobe Systems Incorporated
// All Rights Reserved
//
// NOTICE:  Adobe permits you to use, modify, and distribute this file in accordance with the terms
// of the Adobe license agreement accompanying it.
// =================================================================================================

#if AdobePrivate
// =================================================================================================
// Change history
// ==============
//
// Writers:
//  ADC	Amandeep Chawla
//	SKP Sunil Kishor Pathak
//
// mm-dd-yy who Description of changes, most recent first.
//
// 12-05-14 SKP 1.0-a038 Changed API name from IsTrackAble() to IsTrackable() and from 
//						 MakeTrackAble() to MakeTrackable()
// 11-19-14 ADC 1.0-a001 Framework in place for Asset Management Library.
//
// =================================================================================================
#endif  // AdobePrivate

#include "XMPAssetManagement/XMPAssetManagementFwdDeclarations_I.h"
#include "XMPAssetManagement/Interfaces/IAssetUtilities.h"
#include "XMPAssetManagement/Interfaces/ITSingletonExtension.h"

#include "XMPCommon/Interfaces/BaseInterfaces/ISharedObject_I.h"

#if XMP_WinBuild
#pragma warning( push )
#pragma warning( disable : 4250 )
#endif //#if XMP_WinBuild

namespace AdobeXMPAM_Int {
	using namespace AdobeXMPCommon;
	using namespace AdobeXMPCommon_Int;
	using namespace AdobeXMPAM;
	//!
	//! \brief Internal interface that provides some basic utilities on an Asset's metadata.
	//! \details Provides functions to 
	//!		-# check whether an asset is track able.
	//!		-# add sufficient information to an asset's metadata to make it track able.
	//! \attention Support Multi-threading at object level.
	//!
	class IAssetUtilities_I
		: public virtual IAssetUtilities
		, public virtual ISharedObject_I
		, public virtual ITSingleton< IAssetUtilities_I >
	{
	public:
		/*//!
		//! Indicates whether an asset's metadata has sufficient information so that it can be tracked.
		//! \param[in] metadata a shared pointer to const #NS_XMPCORE::IXMPMetadata object containing assets metadata.
		//! \return a bool value indicating whether asset can be tracked or not.
		//! \note In case metadata is invalid it returns false.
		//!
		virtual bool IsTrackable( const spcIMetadata & metadata ) = 0;

		//!
		//! Enrich an asset's metadata if required so that there is sufficient information available to track it.
		//! \param[in] metadata a shared pointer to #NS_XMPCORE::IXMPMetadata object containing assets metadata.
		//! \return a bool value indicating whether any enrichment is performed or not.
		//! \attention Error is thrown in case:
		//!		-# metadata is an invalid shared pointer.
		//!
		virtual bool MakeTrackable( const spIMetadata & metadata ) = 0;*/

		static uint32 GetInterfaceVersion() { return kInternalInterfaceVersionNumber; }
		virtual pIAssetUtilities APICALL GetActualIAssetUtilities() __NOTHROW__{ return this; }
		virtual pIAssetUtilities_I APICALL GetIAssetUtilities_I() __NOTHROW__{ return this; }

		virtual pvoid APICALL GetInterfacePointer(uint64 interfaceID, uint32 interfaceVersion);

	protected:
		// all safe functions
		virtual uint32 APICALL isTrackable( const pcIMetadata_base metadata, pcIError_base & error ) __NOTHROW__;
		virtual uint32 APICALL makeTrackable( const pIMetadata_base metadata, pcIError_base & error ) __NOTHROW__;
		pvoid APICALL GetInterfacePointerInternal(uint64 interfaceID, uint32 interfaceVersion, bool isTopLevel);
		virtual pvoid APICALL getInterfacePointer(uint64 interfaceID, uint32 interfaceVersion, pcIError_base & error) __NOTHROW__;


		//!
		//! protected Virtual Destructor
		//!
		virtual ~IAssetUtilities_I() {}

	#ifdef FRIEND_CLASS_DECLARATION
			FRIEND_CLASS_DECLARATION();
	#endif
			REQ_FRIEND_CLASS_DECLARATION();
	};
}

#if XMP_WinBuild
#pragma warning( pop )
#endif //#if XMP_WinBuild
#endif  // IAssetUtilities_I_h__
