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
//
// =================================================================================================
#endif  // AdobePrivate

#include "XMPAssetManagement/Interfaces/IAssetLayerPart_I.h"
#include "XMPCommon/Utilities/TWrapperFunctions_I.h"
#include "XMPCommon/Interfaces/IUTF8String_I.h"

namespace AdobeXMPAM_Int {


	pvoid APICALL IAssetLayerPart_I::GetInterfacePointer(uint64 interfaceID, uint32 interfaceVersion) {
		return GetInterfacePointerInternal(interfaceID, interfaceVersion, true);
	}

	pvoid APICALL IAssetLayerPart_I::getInterfacePointer(uint64 interfaceID, uint32 interfaceVersion, pcIError_base & error) __NOTHROW__{
		return CallUnSafeFunction< IAssetLayerPart_I, pvoid, pvoid, uint64, uint32 >(
		error, this, NULL, &IAssetLayerPart_I::GetInterfacePointer, __FILE__, __LINE__, interfaceID, interfaceVersion);
	}

		pvoid APICALL IAssetLayerPart_I::GetInterfacePointerInternal(uint64 interfaceID, uint32 interfaceVersion, bool isTopLevel) {
		pvoid returnValue(NULL);
		if (interfaceID == kIAssetLayerPartID) {
			switch (interfaceVersion) {
			case 1:
				return static_cast< IAssetLayerPart_v1 * >(this);
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
			IError_v1::kESOperationFatal, kIAssetLayerPartID, interfaceID, __FILE__, __LINE__);
		return NULL;
	}

	pcIUTF8String_base APICALL IAssetLayerPart_I::getID(pcIError_base & error) const __NOTHROW__{
		return CallConstUnSafeFunctionReturningSharedPointer< IAssetLayerPart_v1, pcIUTF8String_base, const IUTF8String >(
			error, this, &IAssetLayerPart_I::GetID,__FILE__,__LINE__ );
	}

	/* APICALL IAssetLayerPart_I::GetID() const {
		return GetID_I();
	}*/

	void APICALL IAssetLayerPart_I::setName(const char * name, sizet length, pcIError_base & error) __NOTHROW__{
		CallUnSafeFunctionReturningVoid< IAssetLayerPart_v1, const char *,sizet >(
		error, this, &IAssetLayerPart_v1::SetName, __FILE__, __LINE__, name, length);
	}

	pcIUTF8String_base APICALL IAssetLayerPart_I::getName(pcIError_base & error) const __NOTHROW__{
		return CallConstUnSafeFunctionReturningSharedPointer< IAssetLayerPart_v1, pcIUTF8String_base, const IUTF8String >(
		error, this, &IAssetLayerPart_v1::GetName, __FILE__, __LINE__);
	}

	/*spcIUTF8String APICALL IAssetLayerPart_I::GetName() const {
		return GetName_I();
	}*/

	pIAssetPart_base APICALL IAssetLayerPart_I::clone(pcIError_base & error) const __NOTHROW__{
		return CallConstUnSafeFunctionReturningSharedPointer< IAssetLayerPart_v1, pIAssetPart_base, IAssetPart >(
		error, this, &IAssetLayerPart_v1::Clone, __FILE__, __LINE__);
	}

	/*spIAssetPart APICALL IAssetLayerPart_I::Clone() const {
#if !GENERATE_XMPEXTENSIONS_CLIENT_LAYER_CODE
		return Clone_I();
#else
		return spIAssetLayerPart();
#endif
	}*/
}

namespace AdobeXMPAM {
	using namespace AdobeXMPAM_Int;

#if SOURCE_COMPILING_XMPEXTENSIONS_LIB
	spIAssetLayerPart IAssetLayerPart_v1::CreateStandardLayerPart( eAssetPartComponent component, const char * id, sizet idLength )
	{
		return IAssetLayerPart_I::CreateStandardLayerPart( component, id, idLength );
	}

	spIAssetLayerPart IAssetLayerPart_v1::CreateUserDefinedLayerPart( const char * component, sizet componentLength, const char * id, sizet idLength )
	{
		return IAssetLayerPart_I::CreateUserDefinedLayerPart( component, componentLength, id, idLength );
	}
#endif  // SOURCE_COMPILING_XMPEXTENTIONS_LIB
}
