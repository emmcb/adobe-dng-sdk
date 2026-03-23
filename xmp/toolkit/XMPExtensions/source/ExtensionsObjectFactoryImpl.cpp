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
#include "XMPExtensions/include//ExtensionsObjectFactoryImpl.h"
#undef IMPLEMENTATION_HEADERS_CAN_BE_INCLUDED

#if GENERATE_XMPEXTENSIONS_DLL_LAYER_CODE

#include "XMPCommon/Interfaces/IErrorNotifier_I.h"
#include "XMPCommon/Interfaces/IError_I.h"
#include "XMPCore/Interfaces/IMetadata.h"
#include "XMPAssetManagement/Interfaces/IAssetUtilities_I.h"
#include "XMPAssetManagement/Interfaces/IAssetPart_I.h"
#include "XMPAssetManagement/Interfaces/IAssetTimePart_I.h"
#include "XMPAssetManagement/Interfaces/IAssetLayerPart_I.h"
#include "XMPAssetManagement/Interfaces/IAssetPagePart_I.h"
#include "XMPAssetManagement/Interfaces/IAssetArtboardPart_I.h"
#include "XMPAssetManagement/Interfaces/IComposedAssetManager_I.h"
#include "XMPCommon/Utilities/TWrapperFunctions_I.h"
#include "XMPCommon/Interfaces/IUTF8String_I.h"
#include "XMPMarker/Interfaces/ITemporalAssetManager_I.h"
#include "XMPMarker/Interfaces/ITrack_I.h"

namespace AdobeXMPAM_Int {

	pvoid APICALL ExtensionsObjectFactoryImpl::GetInterfacePointer(uint64 interfaceID, uint32 interfaceVersion) {
		return GetInterfacePointerInternal(interfaceID, interfaceVersion, true);
	}

	pvoid APICALL ExtensionsObjectFactoryImpl::getInterfacePointer(uint64 interfaceID, uint32 interfaceVersion, pcIError_base & error) __NOTHROW__{
		return CallUnSafeFunction< ExtensionsObjectFactoryImpl, pvoid, pvoid, uint64, uint32 >(
		error, this, NULL, &ExtensionsObjectFactoryImpl::GetInterfacePointer, __FILE__, __LINE__, interfaceID, interfaceVersion);
	}

		pvoid APICALL ExtensionsObjectFactoryImpl::GetInterfacePointerInternal(uint64 interfaceID, uint32 interfaceVersion, bool isTopLevel) {
		if (interfaceID == kIExtensionsObjectFactoryID) {
			switch (interfaceVersion) {
			case 1:
				return static_cast< IExtensionsObjectFactory_v1 * >(this);
				break;

			case kInternalInterfaceVersionNumber:
				return this;
				break;

			default:
				throw IError_I::CreateInterfaceVersionNotAvailableError(
					IError_v1::kESOperationFatal, interfaceID, interfaceVersion, __FILE__, __LINE__);
				break;
			}
		}
		else if (interfaceID == kIObjectFactoryID) {
			switch (interfaceVersion) {
			case 1:
				return static_cast< IObjectFactory_v1 * >(this);
				break;

			default:
				throw IError_I::CreateInterfaceVersionNotAvailableError(
					IError_v1::kESOperationFatal, interfaceID, interfaceVersion, __FILE__, __LINE__);
				break;

			}
		}

		if (isTopLevel)
			throw IError_I::CreateInterfaceNotAvailableError(
			IError_v1::kESOperationFatal, kIExtensionsObjectFactoryID, interfaceID, __FILE__, __LINE__);
		return NULL;
	}


	/*	template < typename returnTypeRequired, typename internalSharedPointerType, typename ... Ts >
	returnTypeRequired CreateSharedPointerAndReturnRawPointer( pcIError & error, shared_ptr< internalSharedPointerType >( *FuncPtr )( Ts ... ), Ts ... Vs ) {
	error = NULL;
	try {
	auto returnValue = FuncPtr( Vs ... );
	if (returnValue) returnValue->GetISharedObject_I()->AcquireInternal();
	return returnValue.get();
	} catch ( spcIError & err ) {
	error = err->GetActualIError();
	error->GetISharedObject_I()->AcquireInternal();
	} /*catch ( spIError_I & err ) {

	} *//*catch ( ... ) {
	/*spIError_I _error = IError_I::CreateError(
	IError_base::kDomainGeneral,
	IError_base::kGeneralCodeUnknownExceptionCaught,
	IError_base::kSeverityOperationFatal );
	_error->SetLocation( __FILE__, __LINE__ );
	_error->SetMessage( "Unknown Exception Caught" );
	_error->AcquireSharedOwnershipForRawPointer();
	error = _error.get();*/
	/*pIError_I err = IError_I::CreateUnknownExceptionCaughtError(IError_v1::kESOperationFatal, __FILE__, __LINE__).get();
	err->AcquireInternal();
	error = err;
	}
	return NULL;
	}*/

	//definition of class methods
	pIAssetPart_base APICALL ExtensionsObjectFactoryImpl::CreateStandardGenericPart(uint32 component, pcIError_base & error) __NOTHROW__{
		return ReturnRawPointerFromSharedPointer< IAssetPart_I, pIAssetPart_base, IAssetPart_v1::eAssetPartComponent>(
		&IAssetPart_I::CreateStandardGenericPart, error, __FILE__, __LINE__, static_cast< IAssetPart_v1::eAssetPartComponent >(component));

	}

		pIAssetPart_base APICALL ExtensionsObjectFactoryImpl::CreateUserDefinedGenericPart(const char * component, sizet componentLength, pcIError_base & error) __NOTHROW__{
		return ReturnRawPointerFromSharedPointer< IAssetPart_I, pIAssetPart_base, const char *, sizet >(
		&IAssetPart_I::CreateUserDefinedGenericPart, error, __FILE__, __LINE__, component, componentLength);
	}

		pIAssetUtilities_base APICALL ExtensionsObjectFactoryImpl::GetAssetUtilities(pcIError_base & error) __NOTHROW__{
		error = NULL;
		try {
			return IAssetUtilities_I::GetInstance();
		}
		catch (spcIError & err) {
			error = err->GetActualIError();
			error->GetISharedObject_I()->AcquireInternal();
		} /*catch ( spIError_I & err ) {
		  err->AcquireInternal();
		  error = err.get();
		  } */catch (...) {
			/*spIError_I _error; /* = AdobeXMPAM_Int::IError_I::CreateError(
			IError_base::kDomainGeneral,
			IError_base::kGeneralCodeUnknownExceptionCaught,
			IError_base::kSeverityOperationFatal );
			_error->SetLocation( __FILE__, __LINE__ );
			_error->SetMessage( "Unknown Exception Caught" );
			_error->AcquireSharedOwnershipForRawPointer();
			error = _error.get();*/
			/*_error = IError_I::CreateUnknownExceptionCaughtError(IError::kESOperationFatal, __FILE__, __LINE__);
			error = _error.get();*/

			pIError_I err = IError_I::CreateUnknownExceptionCaughtError(IError_v1::kESOperationFatal, __FILE__, __LINE__).get();
			err->AcquireInternal();
			error = err;
		}
		return NULL;
	}

		pIAssetTimePart_base APICALL ExtensionsObjectFactoryImpl::CreateStandardTimePart(uint32 component, pcIError_base & error) __NOTHROW__
	{
		return ReturnRawPointerFromSharedPointer< IAssetTimePart_I, pIAssetTimePart_base, IAssetPart_v1::eAssetPartComponent >(
		&IAssetTimePart_I::CreateStandardTimePart, error, __FILE__, __LINE__, static_cast< IAssetPart_v1::eAssetPartComponent >(component));
	}

		pIAssetTimePart_base APICALL ExtensionsObjectFactoryImpl::CreateUserDefinedTimePart(const char * component, sizet componentLength,
		pcIError_base & error) __NOTHROW__
	{
		return ReturnRawPointerFromSharedPointer< IAssetTimePart_I, pIAssetTimePart_base, const char *, sizet>(
		&IAssetTimePart_I::CreateUserDefinedTimePart, error, __FILE__, __LINE__, component, componentLength);
	}

		pIAssetLayerPart_base APICALL ExtensionsObjectFactoryImpl::CreateStandardLayerPart(uint32 component, const char * id,
		sizet idLength, pcIError_base & error) __NOTHROW__
	{
		return ReturnRawPointerFromSharedPointer<
		IAssetLayerPart_I, pIAssetLayerPart_base, IAssetPart_v1::eAssetPartComponent, const char *, sizet>(
		&IAssetLayerPart_I::CreateStandardLayerPart, error, __FILE__, __LINE__, static_cast< IAssetPart_v1::eAssetPartComponent >(component),
		id, idLength
		);
	}

		pIAssetLayerPart_base APICALL ExtensionsObjectFactoryImpl::CreateUserDefinedLayerPart(const char * component, sizet componentLength, const char * id, sizet idLength, pcIError_base & error) __NOTHROW__{
		return ReturnRawPointerFromSharedPointer<
		IAssetLayerPart_I, pIAssetLayerPart_base, const char *, sizet, const char *, sizet>(
		&IAssetLayerPart_I::CreateUserDefinedLayerPart, error, __FILE__, __LINE__, component, componentLength, id, idLength
		);
	}

		pIAssetPagePart_base APICALL ExtensionsObjectFactoryImpl::CreateStandardPagePart(uint32 component, sizet pageNumber, pcIError_base & error) __NOTHROW__
	{
		return ReturnRawPointerFromSharedPointer<
		IAssetPagePart_I, pIAssetPagePart_base, IAssetPart_v1::eAssetPartComponent, sizet>(
		&IAssetPagePart_I::CreateStandardPagePart, error, __FILE__, __LINE__, static_cast< IAssetPart_v1::eAssetPartComponent >(component),
		pageNumber
		);
	}

		pIAssetPagePart_base APICALL ExtensionsObjectFactoryImpl::CreateUserDefinedPagePart(const char * component, sizet componentLength, sizet pageNumber, pcIError_base & error) __NOTHROW__
	{
		return ReturnRawPointerFromSharedPointer<
		IAssetPagePart_I, pIAssetPagePart_base, const char *, sizet, sizet
		>(
		&IAssetPagePart_I::CreateUserDefinedPagePart, error, __FILE__, __LINE__, component, componentLength, pageNumber);
	}

		pIComposedAssetManager_base APICALL ExtensionsObjectFactoryImpl::CreateComposedAssetManager(
		pIMetadata_base composedAssetMetadata, pcIError_base & error) __NOTHROW__
	{
		auto sp = IMetadata::MakeShared(composedAssetMetadata);
		return ReturnRawPointerFromSharedPointer<
			IComposedAssetManager_I, pIComposedAssetManager_base, const spIMetadata &
		>(
		&IComposedAssetManager_I::CreateComposedAssetManager, error, __FILE__, __LINE__, sp);
	}

		pIAssetArtboardPart_base APICALL ExtensionsObjectFactoryImpl::CreateStandardArtboardPart(uint32 component, const char * id, sizet idLength, pcIError_base & error) __NOTHROW__{
		return ReturnRawPointerFromSharedPointer<
		IAssetArtboardPart_I, pIAssetArtboardPart_base, IAssetPart_v1::eAssetPartComponent, const char *, sizet>(
		&IAssetArtboardPart_I::CreateStandardArtboardPart, error, __FILE__, __LINE__, static_cast< IAssetPart_v1::eAssetPartComponent >(component),
		id, idLength
		);
	}

		pIAssetArtboardPart_base APICALL ExtensionsObjectFactoryImpl::CreateUserDefinedArtboardPart(const char * component, sizet componentLength, const char * id, sizet idLength, pcIError_base & error) __NOTHROW__{
		return ReturnRawPointerFromSharedPointer<
		IAssetArtboardPart_I, pIAssetArtboardPart_base, const char *, sizet, const char *, sizet>(
		&IAssetArtboardPart_I::CreateUserDefinedArtboardPart, error, __FILE__, __LINE__, component, componentLength, id, idLength
		);
	}

		pIError_base APICALL ExtensionsObjectFactoryImpl::CreateError(uint32 domain, uint32 code, uint32 severity, pcIError_base & error) {
		return IError_I::CreateError(static_cast< IError::eErrorDomain >(domain), static_cast< IError::eErrorCode >(code), static_cast< IError::eErrorSeverity >(severity)).get();
		/*return ReturnRawPointerFromSharedPointer< IError_I, pIError_base, IError::eErrorDomain, IError::eErrorCode, IError::eErrorSeverity >(
			&IError_I::CreateError, error, __FILE__, __LINE__, static_cast< IError::eErrorDomain >(domain), static_cast< IError::eErrorCode >(code), static_cast< IError::eErrorSeverity >(severity));*/
	}

		pIUTF8String_base APICALL ExtensionsObjectFactoryImpl::CreateUTF8String(const char * buf, sizet count, pcIError_base & error) {
		return ReturnRawPointerFromSharedPointer<  IUTF8String, pIUTF8String_base, const char *, sizet>(
			&IUTF8String_I::CreateUTF8String, error, __FILE__, __LINE__, buf, count);
	}

		pITrack_base APICALL ExtensionsObjectFactoryImpl::CreateTrack(const char * trackName, sizet trackNameLen, uint64 trackTypes, const char * trackTypeName, sizet trackTypeNameLen, pcIError_base & error) __NOTHROW__{

		return ReturnRawPointerFromSharedPointer <
		ITrack_I, pITrack_base, const char *, sizet, uint64, const char *, sizet>(
		&ITrack_I::CreateTrack_I
		, error, __FILE__, __LINE__, trackName, trackNameLen, trackTypes, trackTypeName, trackTypeNameLen
		);

	}

		pITemporalAssetManager_base APICALL ExtensionsObjectFactoryImpl::CreateTemporalAssetManager( pIMetadata_base temporalAssetMetadata, pcIError_base & error) __NOTHROW__{

		auto sp = IMetadata::MakeShared(temporalAssetMetadata);
		return ReturnRawPointerFromSharedPointer< 
			ITemporalAssetManager_I, pITemporalAssetManager_base, const spIMetadata &>(
		&ITemporalAssetManager_I::CreateTemporalAssetManager_I, error, __FILE__, __LINE__ ,sp
		);
	}

}

#include "XMPAssetManagement/Interfaces/TSingletonExtension.h"

namespace AdobeXMPAM {
	using namespace AdobeXMPCommon_Int;
	using namespace AdobeXMPAM_Int;

	template< >
	pIExtensionsObjectFactory_I ITSingleton< IExtensionsObjectFactory_I >::GetInstance() {
		return &TSingleton< ExtensionsObjectFactoryImpl >::Instance();
	}

	template< >
	void ITSingleton< IExtensionsObjectFactory_I >::CreateInstance() {
		GetInstance();
	}

	template< >
	void ITSingleton< IExtensionsObjectFactory_I >::DestroyInstance() {
		TSingleton< ExtensionsObjectFactoryImpl >::Destroy();
	}
}

#endif  // GENERATE_XMPEXTENSIONS_DLL_LAYER_CODE

