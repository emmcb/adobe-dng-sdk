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
//
// =================================================================================================
#endif  // AdobePrivate

#include "XMPAssetManagement/Interfaces/IAssetArtboardPart_I.h"
#include "XMPCommon/Utilities/TWrapperFunctions_I.h"
#include "XMPCommon/Interfaces/IUTF8String_I.h"

namespace AdobeXMPAM_Int {

	pvoid APICALL IAssetArtboardPart_I::GetInterfacePointer(uint64 interfaceID, uint32 interfaceVersion) {
		return GetInterfacePointerInternal(interfaceID, interfaceVersion, true);
	}

	pvoid APICALL IAssetArtboardPart_I::getInterfacePointer(uint64 interfaceID, uint32 interfaceVersion, pcIError_base & error) __NOTHROW__{
		return CallUnSafeFunction< IAssetArtboardPart_I, pvoid, pvoid, uint64, uint32 >(
		error, this, NULL, &IAssetArtboardPart_I::GetInterfacePointer, __FILE__, __LINE__, interfaceID, interfaceVersion);
	}

		pvoid APICALL IAssetArtboardPart_I::GetInterfacePointerInternal(uint64 interfaceID, uint32 interfaceVersion, bool isTopLevel) {
		pvoid returnValue(NULL);
		if (interfaceID == kIAssetArtboardPartID) {
			switch (interfaceVersion) {
			case 1:
				return static_cast< IAssetArtboardPart_v1 * >(this);
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
			IError_v1::kESOperationFatal, kIAssetArtboardPartID, interfaceID, __FILE__, __LINE__);
		return NULL;
	}
	pcIUTF8String_base APICALL IAssetArtboardPart_I::getID(pcIError_base & error) const __NOTHROW__{
		return CallConstUnSafeFunctionReturningSharedPointer< IAssetArtboardPart_v1, pcIUTF8String_base, const IUTF8String >(
			error, this, &IAssetArtboardPart_v1::GetID, __FILE__, __LINE__ );
	}

	/*spcIUTF8String APICALL IAssetArtboardPart_I::GetID() const {
		return GetID_I();
	}*/

	void APICALL IAssetArtboardPart_I::setName(const char * name, sizet length, pcIError_base & error) __NOTHROW__{
		CallUnSafeFunctionReturningVoid< IAssetArtboardPart_v1, const char *, sizet >(
		error, this, &IAssetArtboardPart_v1::SetName, __FILE__, __LINE__, name, length);
	}

	pcIUTF8String_base APICALL IAssetArtboardPart_I::getName(pcIError_base & error) const __NOTHROW__{
		return CallConstUnSafeFunctionReturningSharedPointer< IAssetArtboardPart_v1, pcIUTF8String_base, const IUTF8String >(
		error, this, &IAssetArtboardPart_v1::GetName, __FILE__, __LINE__);
	}

	/*spcIUTF8String APICALL IAssetArtboardPart_I::GetName() const {
		return GetName_I();
	}*/

	pIAssetPart_base APICALL IAssetArtboardPart_I::clone(pcIError_base & error) const __NOTHROW__{
		return CallConstUnSafeFunctionReturningSharedPointer< IAssetArtboardPart_v1, pIAssetPart_base, IAssetPart >(
		error, this, &IAssetArtboardPart_v1::Clone, __FILE__, __LINE__);
	}

	/*spIAssetPart APICALL IAssetArtboardPart_I::Clone() const {
#if !GENERATE_XMPEXTENSIONS_CLIENT_LAYER_CODE
		return Clone_I();
#else
		return spIAssetArtboardPart();
#endif
	}*/
}

namespace AdobeXMPAM {
	using namespace AdobeXMPAM_Int;

#if SOURCE_COMPILING_XMPEXTENSIONS_LIB
	spIAssetArtboardPart IAssetArtboardPart_v1::CreateStandardArtboardPart( eAssetPartComponent component, const char * id, sizet idLength)
	{
		return IAssetArtboardPart_I::CreateStandardArtboardPart( component, id, idLength);
	}

	spIAssetArtboardPart IAssetArtboardPart_v1::CreateUserDefinedArtboardPart( const char * component, sizet componentLength, const char * id, sizet idLength)
	{
		return IAssetArtboardPart_I::CreateUserDefinedArtboardPart( component, componentLength, id, idLength );
	}
#endif
}
