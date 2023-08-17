#ifndef AssetManagementObjectFactoryImpl_h__
#define AssetManagementObjectFactoryImpl_h__ 1

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
//
// mm-dd-yy who Description of changes, most recent first.
//
// 12-07-14 ADC 1.0-a041 Refactored Part Classes and Adding support for IAssetArtboardPart.
// 11-26-14 ADC 1.0-a013 Added the full skeleton for IComposedAssetManager interface.
// 11-21-14 ADC 1.0-a004 Support for IAssetLayerPart interface.
// 11-21-14 ADC 1.0-a002 Support for IAssetPart and IAssetTimePart interfaces.
// 11-19-14 ADC 1.0-a001 Framework in place for Asset Management Library.
//
// =================================================================================================
#endif  // AdobePrivate

#if !(IMPLEMENTATION_HEADERS_CAN_BE_INCLUDED)
	#error "Not adhering to design constraints"
	// this file should only be included from its own cpp file
#endif

#include "XMPAssetManagement/Interfaces/IAssetManagementObjectFactory_I.h"

#if GENERATE_XMPEXTENSIONS_DLL_LAYER_CODE

#include "XMPCommon/BaseClasses/IntrusiveRefCountSharedOwnership.h"
#include "XMPCommon/BaseClasses/Object.h"
#include "XMPCommon/Interfaces/ISharedMutex_I.h"

#if XMP_WinBuild
	#pragma warning( push )
	#pragma warning( disable : 4250 )
#endif

namespace NS_INT_XMPASSETMANAGEMENT {
	using namespace NS_INT_XMPASSETMANAGEMENT;
	using namespace NS_XMPCOMMON;
	using namespace NS_INT_XMPCOMMON;
	
	class AssetManagementObjectFactoryImpl
		: public virtual IAssetManagementObjectFactory_I
		, public virtual Object
	{
	public:
		AssetManagementObjectFactoryImpl() {};
		virtual ~AssetManagementObjectFactoryImpl() __NOTHROW__;

		virtual pIAssetUtilities_base GetAssetUtilities( pcIError_base & error ) __NOTHROW__;
		virtual pIAssetPart_base CreateStandardGenericPart( UInt32 component, UInt32 multiThreadingSupport, pcIError_base & error ) __NOTHROW__;
		virtual pIAssetPart_base CreateUserDefinedGenericPart( const char * component, UInt32 multiThreadingSupport, pcIError_base & error ) __NOTHROW__;
		virtual pIAssetTimePart_base CreateStandardTimePart( UInt32 component, UInt32 multiThreadingSupport, pcIError_base & error ) __NOTHROW__;
		virtual pIAssetTimePart_base CreateUserDefinedTimePart( const char * component, UInt32 multiThreadingSupport, pcIError_base & error ) __NOTHROW__;
		virtual pIAssetLayerPart_base CreateStandardLayerPart( UInt32 component, const char * id, UInt32 multiThreadingSupport, pcIError_base & error ) __NOTHROW__;
		virtual pIAssetLayerPart_base CreateUserDefinedLayerPart( const char * component, const char * id, UInt32 multiThreadingSupport, pcIError_base & error ) __NOTHROW__;
		virtual pIAssetPagePart_base CreateStandardPagePart(UInt32 component, SizeT pageNumber, UInt32 multiThreadingSupport, pcIError_base & error) __NOTHROW__;
		virtual pIAssetPagePart_base CreateUserDefinedPagePart(const char * component, SizeT pageNumber, UInt32 multiThreadingSupport, pcIError_base & error) __NOTHROW__;
		virtual pIAssetArtboardPart_base CreateStandardArtboardPart( UInt32 component, const char * id, UInt32 multiThreadingSupport, pcIError_base & error ) __NOTHROW__;
		virtual pIAssetArtboardPart_base CreateUserDefinedArtboardPart( const char * component, const char * id, UInt32 multiThreadingSupport, pcIError_base & error ) __NOTHROW__;
		virtual pIComposedAssetManager_base CreateComposedAssetManager( pIXMPMetadata_base composedAssetMetadata, UInt32 multiThreadingSupport, pcIError_base & error ) __NOTHROW__;

	protected:

		#ifndef FRIEND_CLASS_DECLARATION
			#define FRIEND_CLASS_DECLARATION() 
		#endif
		FRIEND_CLASS_DECLARATION();

	};
	
	inline AssetManagementObjectFactoryImpl::~AssetManagementObjectFactoryImpl() __NOTHROW__ { }
}

#if XMP_WinBuild
	#pragma warning( pop )
#endif

#endif  // GENERATE_XMPEXTENSIONS_DLL_LAYER_CODE

#endif  // AssetManagementObjectFactoryImpl_h__
