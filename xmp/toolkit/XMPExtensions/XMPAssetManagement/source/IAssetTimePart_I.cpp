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
// 11-21-14 ADC 1.0-a004 Support for IAssetLayerPart interface.
// 11-21-14 ADC 1.0-a002 Support for IAssetPart and IAssetTimePart interfaces.
//
// =================================================================================================
#endif  // AdobePrivate

#include "XMPAssetManagement/Interfaces/IAssetTimePart_I.h"
#include "XMPCommon/Interfaces/IError_I.h"
#include "XMPCommon/XMPCommonErrorCodes.h"
#include "XMPCommon/Utilities/TWrapperFunctions_I.h"

namespace AdobeXMPAM_Int {

	pvoid APICALL IAssetTimePart_I::GetInterfacePointer(uint64 interfaceID, uint32 interfaceVersion) {
		return GetInterfacePointerInternal(interfaceID, interfaceVersion, true);
	}

	pvoid APICALL IAssetTimePart_I::getInterfacePointer(uint64 interfaceID, uint32 interfaceVersion, pcIError_base & error) __NOTHROW__{
		return CallUnSafeFunction< IAssetTimePart_I, pvoid, pvoid, uint64, uint32 >(
		error, this, NULL, &IAssetTimePart_I::GetInterfacePointer, __FILE__, __LINE__, interfaceID, interfaceVersion);
	}

		pvoid APICALL IAssetTimePart_I::GetInterfacePointerInternal(uint64 interfaceID, uint32 interfaceVersion, bool isTopLevel) {
		pvoid returnValue(NULL);
		if (interfaceID == kIAssetTimePartID) {
			switch (interfaceVersion) {
			case 1:
				return static_cast< IAssetTimePart_v1 * >(this);
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
		else {
			returnValue = IAssetPart_I::GetInterfacePointerInternal(interfaceID, interfaceVersion, false);
			if (returnValue) return returnValue;
		}
		if (isTopLevel)
			throw IError_I::CreateInterfaceNotAvailableError(
			IError_v1::kESOperationFatal, kIAssetTimePartID, interfaceID, __FILE__, __LINE__);
		return NULL;
	}



	void APICALL IAssetTimePart_I::setFrameRate(uint32 typeOfFrameRate, uint64 numerator, uint64 denominator, pcIError_base & error) __NOTHROW__{
		try {
			switch ( typeOfFrameRate ) {
				case 1:
					SetFrameRateForStartFrameCount( numerator, denominator );
					break;

				case 2:
					SetFrameRateForCountOfFrames( numerator, denominator );
					break;

				default:
					SetBothFrameRates( numerator, denominator );
					break;
			} 
		} catch ( spcIError & err ) {                                       //refactor as per CoreObjectFactoryImpl
			error = err->GetActualIError();
			error->GetISharedObject_I()->AcquireInternal();
		}
		/*catch( pIError & err) {
			error = err;
		}*/ catch ( ... ) {
				/*pIError_I pErr = IError_I::CreateError(
				IError_base::kEDGeneral,
				kGECUnknownExceptionCaught,
				IError_base::kESOperationFatal);
				pErr->SetLocation( __FILE__, __LINE__ );
				pErr->SetMessage( "Unknown Exception Caught" );
				error = pErr;*/
			pIError_I err = IError_I::CreateUnknownExceptionCaughtError(IError_v1::kESOperationFatal, __FILE__, __LINE__).get();
			err->AcquireInternal();
			error = err;
		}
	}
		
	pIAssetPart_base APICALL IAssetTimePart_I::clone(pcIError_base & error) const __NOTHROW__{
		return CallConstUnSafeFunctionReturningSharedPointer< IAssetPart_v1, pIAssetPart_base, IAssetPart >(
		error, this, &IAssetPart_v1::Clone, __FILE__, __LINE__);
	}
/*	spIAssetPart APICALL IAssetTimePart_I::Clone() const {
#if !GENERATE_XMPEXTENSIONS_CLIENT_LAYER_CODE
		return Clone_I();
#else
		return spIAssetTimePart();
#endif
	}*/
}

namespace AdobeXMPAM {
	using namespace AdobeXMPAM_Int;

#if SOURCE_COMPILING_XMPEXTENSIONS_LIB
	spIAssetTimePart IAssetTimePart_v1::CreateStandardTimePart(eAssetPartComponent component)
	{
		return IAssetTimePart_I::CreateStandardTimePart( component );
	}

	spIAssetTimePart IAssetTimePart_v1::CreateUserDefinedTimePart(
		const char * component, sizet componentLength )
	{
		return IAssetTimePart_I::CreateUserDefinedTimePart( component, componentLength );
	}
#endif
}
