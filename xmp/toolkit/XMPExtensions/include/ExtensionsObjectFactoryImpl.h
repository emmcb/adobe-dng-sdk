#ifndef ExtensionsObjectFactoryImpl_h__
#define ExtensionsObjectFactoryImpl_h__ 1

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

#include "XMPExtensions/include/IExtensionsObjectFactory_I.h"

#if GENERATE_XMPEXTENSIONS_DLL_LAYER_CODE

//#include "XMPCommon/Interfaces/ISharedMutex_I.h"
#include "XMPCommon/BaseClasses/MemoryManagedObject.h"
#include "XMPCommon/ImplHeaders/SharedObjectImpl.h"

#if XMP_WinBuild
#pragma warning( push )
#pragma warning( disable : 4250 )
#endif

namespace AdobeXMPAM_Int {
	using namespace AdobeXMPAM_Int;
	using namespace AdobeXMPCommon;
	using namespace AdobeXMPCommon_Int;

	class ExtensionsObjectFactoryImpl
		: public virtual IExtensionsObjectFactory_I
		, public virtual SharedObjectImpl
		, public virtual MemoryManagedObject
	{
	public:
		ExtensionsObjectFactoryImpl() {};
		virtual ~ExtensionsObjectFactoryImpl() __NOTHROW__;

		virtual pvoid APICALL GetInterfacePointer(uint64 interfaceID, uint32 interfaceVersion);
		virtual pIError_base APICALL CreateError(uint32 domain, uint32 code, uint32 severity, pcIError_base & error) __NOTHROW__; 
		virtual pIUTF8String_base APICALL CreateUTF8String(const char * buf, sizet count, pcIError_base & error) __NOTHROW__;

		virtual pIAssetUtilities_base APICALL GetAssetUtilities(pcIError_base & error) __NOTHROW__;
		virtual pIAssetPart_base APICALL CreateStandardGenericPart(uint32 component, pcIError_base & error) __NOTHROW__;
		virtual pIAssetPart_base APICALL CreateUserDefinedGenericPart(const char * component, sizet componentLength, pcIError_base & error) __NOTHROW__;
		virtual pIAssetTimePart_base APICALL CreateStandardTimePart(uint32 component, pcIError_base & error) __NOTHROW__;
		virtual pIAssetTimePart_base APICALL CreateUserDefinedTimePart(const char * component, sizet componentLength, pcIError_base & error) __NOTHROW__;
		virtual pIAssetLayerPart_base APICALL CreateStandardLayerPart(uint32 component, const char * id, sizet idLength, pcIError_base & error) __NOTHROW__;
		virtual pIAssetLayerPart_base APICALL CreateUserDefinedLayerPart(const char * component, sizet componentLength, const char * id, sizet idLength, pcIError_base & error) __NOTHROW__;
		virtual pIAssetPagePart_base APICALL CreateStandardPagePart(uint32 component, sizet pageNumber, pcIError_base & error) __NOTHROW__;
		virtual pIAssetPagePart_base APICALL CreateUserDefinedPagePart(const char * component, sizet componentLength, sizet pageNumber, pcIError_base & error) __NOTHROW__;
		virtual pIAssetArtboardPart_base APICALL CreateStandardArtboardPart(uint32 component, const char * id, sizet idLength, pcIError_base & error) __NOTHROW__;
		virtual pIAssetArtboardPart_base APICALL CreateUserDefinedArtboardPart(const char * component, sizet componentLength, const char * id, sizet idLength, pcIError_base & error) __NOTHROW__;
		virtual pIComposedAssetManager_base APICALL CreateComposedAssetManager(pIMetadata_base composedAssetMetadata, pcIError_base & error) __NOTHROW__;
		virtual pITrack_base APICALL CreateTrack(const char * trackName, sizet trackNameLen, uint64 trackTypes, const char * trackTypeName, sizet trackTypeNameLen, pcIError_base & error) __NOTHROW__;
		virtual pITemporalAssetManager_base APICALL CreateTemporalAssetManager(pIMetadata_base temporalAssetMetadata, pcIError_base & error) __NOTHROW__;

	protected:

		pvoid APICALL GetInterfacePointerInternal(uint64 interfaceID, uint32 interfaceVersion, bool isTopLevel);
		virtual pvoid APICALL getInterfacePointer(uint64 interfaceID, uint32 interfaceVersion, pcIError_base & error) __NOTHROW__;

#ifdef FRIEND_CLASS_DECLARATION
		FRIEND_CLASS_DECLARATION();
#endif
		REQ_FRIEND_CLASS_DECLARATION();

	};

	inline ExtensionsObjectFactoryImpl::~ExtensionsObjectFactoryImpl() __NOTHROW__{}
}

#if XMP_WinBuild
#pragma warning( pop )
#endif

#endif  // GENERATE_XMPEXTENSIONS_DLL_LAYER_CODE

#endif  // ExtensionsObjectFactoryImpl_h__
