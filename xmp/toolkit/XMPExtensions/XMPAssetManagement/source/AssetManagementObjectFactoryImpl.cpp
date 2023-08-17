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
//	ANS Ankita Sharma
//
// mm-dd-yy who Description of changes, most recent first.
//
// 12-07-14 ADC 1.0-a041 Refactored Part Classes and Adding support for IAssetArtboardPart.
// 11-26-14 ADC 1.0-a013 Added the full skeleton for IComposedAssetManager interface.
// 11-25-14 ADC 1.0-a009 Fixing compile and linking issues.
// 11-24-14 ANS 1.0-a007 Fixing build errors
// 11-23-14 SKP 1.0-a006 Support for IAssetPagePart interface.
// 11-21-14 ADC 1.0-a004 Support for IAssetLayerPart interface.
// 11-21-14 ADC 1.0-a002 Support for IAssetPart and IAssetTimePart interfaces.
// 11-19-14 ADC 1.0-a001 Framework in place for Asset Management Library.
//
// =================================================================================================
#endif  // AdobePrivate


#define IMPLEMENTATION_HEADERS_CAN_BE_INCLUDED 1
	#include "XMPAssetManagement/ImplHeaders/AssetManagementObjectFactoryImpl.h"
#undef IMPLEMENTATION_HEADERS_CAN_BE_INCLUDED

#if GENERATE_XMPEXTENSIONS_DLL_LAYER_CODE

#include "XMPCommon/Interfaces/IErrorNotificationManager_I.h"

#include "XMPAssetManagement/Interfaces/IAssetUtilities_I.h"
#include "XMPAssetManagement/Interfaces/IAssetPart_I.h"
#include "XMPAssetManagement/Interfaces/IAssetTimePart_I.h"
#include "XMPAssetManagement/Interfaces/IAssetLayerPart_I.h"
#include "XMPAssetManagement/Interfaces/IAssetPagePart_I.h"
#include "XMPAssetManagement/Interfaces/IAssetArtboardPart_I.h"
#include "XMPAssetManagement/Interfaces/IComposedAssetManager_I.h"

namespace NS_INT_XMPASSETMANAGEMENT {

	template < typename returnTypeRequired, typename internalSharedPointerType, typename ... Ts >
	returnTypeRequired CreateSharedPointerAndReturnRawPointer( pcIError & error, shared_ptr< internalSharedPointerType >( *FuncPtr )( Ts ... ), Ts ... Vs ) {
		error = NULL;
		try {
			auto returnValue = FuncPtr( Vs ... );
			if ( returnValue ) returnValue->AcquireSharedOwnershipForRawPointer();
			return returnValue.get();
		} catch ( NS_INT_XMPCOMMON::spcIError_I & err ) {
			err->AcquireSharedOwnershipForRawPointer();
			error = err.get();
		} catch ( NS_INT_XMPCOMMON::spIError_I & err ) {
			err->AcquireSharedOwnershipForRawPointer();
			error = err.get();
		} catch ( ... ) {
			NS_INT_XMPCOMMON::spIError_I _error = NS_INT_XMPCOMMON::IError_I::CreateError(
				IError_base::kDomainGeneral,
				IError_base::kGeneralCodeUnknownExceptionCaught,
				IError_base::kSeverityOperationFatal );
			_error->SetLocation( __FILE__, __LINE__ );
			_error->SetMessage( "Unknown Exception Caught" );
			_error->AcquireSharedOwnershipForRawPointer();
			error = _error.get();
		}
		return NULL;
	}
	//definition of class methods
	pIAssetPart_base AssetManagementObjectFactoryImpl::CreateStandardGenericPart( UInt32 component, UInt32 multiThreadingSupport, pcIError_base & error ) __NOTHROW__ {
		return CreateSharedPointerAndReturnRawPointer< pIAssetPart_base, IAssetPart_I, IAssetPart_v1::eAssetPartComponent, eMultiThreadingSupport >(
			error, &IAssetPart_I::CreateStandardGenericPart_I, static_cast< IAssetPart_v1::eAssetPartComponent >( component ),
			static_cast< eMultiThreadingSupport >( multiThreadingSupport ) );
		
	}

	pIAssetPart_base AssetManagementObjectFactoryImpl::CreateUserDefinedGenericPart( const char * component, UInt32 multiThreadingSupport, pcIError_base & error ) __NOTHROW__ {
		return CreateSharedPointerAndReturnRawPointer< pIAssetPart_base, IAssetPart_I, const char *, eMultiThreadingSupport >(
			error, &IAssetPart_I::CreateUserDefinedGenericPart_I, component, static_cast< eMultiThreadingSupport >( multiThreadingSupport ) );
	}

	pIAssetUtilities_base AssetManagementObjectFactoryImpl::GetAssetUtilities( pcIError_base & error ) __NOTHROW__ {
		error = NULL;
		try {
			return IAssetUtilities_I::GetInstance();
		} catch ( NS_INT_XMPCOMMON::spcIError_I & err ) {
			err->AcquireSharedOwnershipForRawPointer();
			error = err.get();
		} catch ( NS_INT_XMPCOMMON::spIError_I & err ) {
			err->AcquireSharedOwnershipForRawPointer();
			error = err.get();
		} catch ( ... ) {
			NS_INT_XMPCOMMON::spIError_I _error = NS_INT_XMPCOMMON::IError_I::CreateError(
				IError_base::kDomainGeneral,
				IError_base::kGeneralCodeUnknownExceptionCaught,
				IError_base::kSeverityOperationFatal );
			_error->SetLocation( __FILE__, __LINE__ );
			_error->SetMessage( "Unknown Exception Caught" );
			_error->AcquireSharedOwnershipForRawPointer();
			error = _error.get();
		}
		return NULL;
	}

	pIAssetTimePart_base AssetManagementObjectFactoryImpl::CreateStandardTimePart( UInt32 component, UInt32 multiThreadingSupport,
		pcIError_base & error ) __NOTHROW__
	{
		return CreateSharedPointerAndReturnRawPointer< pIAssetTimePart_base, IAssetTimePart_I, IAssetPart_v1::eAssetPartComponent, eMultiThreadingSupport >(
			error, &IAssetTimePart_I::CreateStandardTimePart_I, static_cast< IAssetPart_v1::eAssetPartComponent >( component ),
			static_cast< eMultiThreadingSupport >( multiThreadingSupport )
		);
	}

	pIAssetTimePart_base AssetManagementObjectFactoryImpl::CreateUserDefinedTimePart( const char * component, UInt32 multiThreadingSupport,
		pcIError_base & error ) __NOTHROW__
	{
		return CreateSharedPointerAndReturnRawPointer< pIAssetTimePart_base, IAssetTimePart_I, const char *, eMultiThreadingSupport >(
			error, &IAssetTimePart_I::CreateUserDefinedTimePart_I, component, static_cast< eMultiThreadingSupport >( multiThreadingSupport )
		);
	}

	pIAssetLayerPart_base AssetManagementObjectFactoryImpl::CreateStandardLayerPart( UInt32 component, const char * id,
		UInt32 multiThreadingSupport, pcIError_base & error ) __NOTHROW__
	{
		return CreateSharedPointerAndReturnRawPointer<
			pIAssetLayerPart_base, IAssetLayerPart_I, IAssetPart_v1::eAssetPartComponent, const char * , eMultiThreadingSupport
		>(
			error, &IAssetLayerPart_I::CreateStandardLayerPart_I, static_cast< IAssetPart_v1::eAssetPartComponent >( component ),
			id, static_cast< eMultiThreadingSupport >( multiThreadingSupport )
		);
	}

	pIAssetLayerPart_base AssetManagementObjectFactoryImpl::CreateUserDefinedLayerPart( const char * component, const char * id, UInt32 multiThreadingSupport, pcIError_base & error ) __NOTHROW__{
		return CreateSharedPointerAndReturnRawPointer<
			pIAssetLayerPart_base, IAssetLayerPart_I, const char *, const char * , eMultiThreadingSupport
		>(
			error, &IAssetLayerPart_I::CreateUserDefinedLayerPart_I, component, id, static_cast< eMultiThreadingSupport >( multiThreadingSupport )
		);
	}

	pIAssetPagePart_base AssetManagementObjectFactoryImpl::CreateStandardPagePart(UInt32 component, SizeT pageNumber,
		UInt32 multiThreadingSupport, pcIError_base & error) __NOTHROW__
	{
		return CreateSharedPointerAndReturnRawPointer<
			pIAssetPagePart_base, IAssetPagePart_I, IAssetPart_v1::eAssetPartComponent, SizeT, eMultiThreadingSupport
		>(
			error, &IAssetPagePart_I::CreateStandardPagePart_I, static_cast< IAssetPart_v1::eAssetPartComponent >(component),
			pageNumber, static_cast< eMultiThreadingSupport >(multiThreadingSupport)
		);
	}

	pIAssetPagePart_base AssetManagementObjectFactoryImpl::CreateUserDefinedPagePart(const char * component, SizeT pageNumber,
		UInt32 multiThreadingSupport, pcIError_base & error) __NOTHROW__
	{
		return CreateSharedPointerAndReturnRawPointer<
		pIAssetPagePart_base, IAssetPagePart_I, const char *, SizeT, eMultiThreadingSupport
		>(
			error, &IAssetPagePart_I::CreateUserDefinedPagePart_I, component, pageNumber, static_cast< eMultiThreadingSupport >(multiThreadingSupport)
		);
	}

	pIComposedAssetManager_base AssetManagementObjectFactoryImpl::CreateComposedAssetManager(
		pIXMPMetadata_base composedAssetMetadata, UInt32 multiThreadingSupport, pcIError_base & error ) __NOTHROW__
	{
		auto sp = NS_INT_XMPCOMMON::MakeSharedPointer< IXMPMetadata >( composedAssetMetadata );
		return CreateSharedPointerAndReturnRawPointer<
			pIComposedAssetManager_base, IComposedAssetManager_I, const spIXMPMetadata &, eMultiThreadingSupport
		>(
			error, &IComposedAssetManager_I::CreateComposedAssetManager_I, sp, static_cast< eMultiThreadingSupport >( multiThreadingSupport )
		);
	}

	pIAssetArtboardPart_base AssetManagementObjectFactoryImpl::CreateStandardArtboardPart( UInt32 component, const char * id, UInt32 multiThreadingSupport, pcIError_base & error ) __NOTHROW__{
		return CreateSharedPointerAndReturnRawPointer<
			pIAssetArtboardPart_base, IAssetArtboardPart_I, IAssetPart_v1::eAssetPartComponent, const char *, eMultiThreadingSupport
		>(
			error, &IAssetArtboardPart_I::CreateStandardArtboardPart_I, static_cast< IAssetPart_v1::eAssetPartComponent >( component ),
			id, static_cast< eMultiThreadingSupport >( multiThreadingSupport )
		);
	}

	pIAssetArtboardPart_base AssetManagementObjectFactoryImpl::CreateUserDefinedArtboardPart( const char * component, const char * id, UInt32 multiThreadingSupport, pcIError_base & error ) __NOTHROW__{
		return CreateSharedPointerAndReturnRawPointer<
			pIAssetArtboardPart_base, IAssetArtboardPart_I, const char *, const char *, eMultiThreadingSupport
		>(
			error, &IAssetArtboardPart_I::CreateUserDefinedArtboardPart_I, component, id,
			static_cast< eMultiThreadingSupport >( multiThreadingSupport )
		);
	}

}

#include "XMPCommon/BaseClasses/TSingleton.h"

namespace NS_XMPCOMMON {
	using namespace NS_INT_XMPCOMMON;
	using namespace NS_INT_XMPASSETMANAGEMENT;

	template< >
	pIAssetManagementObjectFactory_I ITSingleton< IAssetManagementObjectFactory_I >::GetInstance() {
		return &TSingleton< AssetManagementObjectFactoryImpl >::Instance();
	}

	template< >
	void ITSingleton< IAssetManagementObjectFactory_I >::CreateInstance() {
		GetInstance();
	}

	template< >
	void ITSingleton< IAssetManagementObjectFactory_I >::DestroyInstance() {
		TSingleton< AssetManagementObjectFactoryImpl >::Destroy();
	}
}

#endif  // GENERATE_XMPEXTENSIONS_DLL_LAYER_CODE

