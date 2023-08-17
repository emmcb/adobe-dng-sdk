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
// 11-21-14 ADC 1.0-a002 Support for IAssetPart and IAssetTimePart interfaces.
// 11-19-14 ADC 1.0-a001 Framework in place for Asset Management Library.
//
// =================================================================================================
#endif  // AdobePrivate

#include "XMPAssetManagement/Interfaces/IAssetUtilities_I.h"
#include "XMPCommon/Utilities/TWrapperFunctions_I.h"
#include "XMPCore/Interfaces/IMetadata.h"

namespace AdobeXMPAM_Int {
	pvoid APICALL IAssetUtilities_I::GetInterfacePointer(uint64 interfaceID, uint32 interfaceVersion) {
		return GetInterfacePointerInternal(interfaceID, interfaceVersion, true);
	}

	pvoid APICALL IAssetUtilities_I::getInterfacePointer(uint64 interfaceID, uint32 interfaceVersion, pcIError_base & error) __NOTHROW__{
		return CallUnSafeFunction< IAssetUtilities_I, pvoid, pvoid, uint64, uint32 >(
		error, this, NULL, &IAssetUtilities_I::GetInterfacePointer, __FILE__, __LINE__, interfaceID, interfaceVersion);
	}

	pvoid APICALL IAssetUtilities_I::GetInterfacePointerInternal(uint64 interfaceID, uint32 interfaceVersion, bool isTopLevel) {
		if (interfaceID == kIAssetUtilitiesID) {
			switch (interfaceVersion) {
			case 1:
				return static_cast< IAssetUtilities_v1 * >(this);
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
		if (isTopLevel)
			throw IError_I::CreateInterfaceNotAvailableError(
			IError_v1::kESOperationFatal, kIAssetUtilitiesID, interfaceID, __FILE__, __LINE__);
		return NULL;
	}



	uint32 APICALL IAssetUtilities_I::isTrackable( const pcIMetadata_base metadata, pcIError_base & error ) __NOTHROW__ {
		return CallUnSafeFunction< IAssetUtilities_v1, uint32, bool, const spcIMetadata & >(
		error, this, false, &IAssetUtilities_v1::IsTrackable, __FILE__, __LINE__, IMetadata::MakeShared(metadata));
	}

		uint32 APICALL IAssetUtilities_I::makeTrackable(const pIMetadata_base metadata, pcIError_base & error) __NOTHROW__{
		return CallUnSafeFunction< IAssetUtilities_v1, uint32, bool, const spIMetadata & >(
		error, this, false, &IAssetUtilities_v1::MakeTrackable, __FILE__, __LINE__, IMetadata::MakeShared(metadata));
	}

}

#if !SOURCE_COMPILING_XMPEXTENSIONS_LIB

namespace AdobeXMPAM{
	using namespace AdobeXMPAM_Int;
	pIAssetUtilities IAssetUtilities_v1::GetAssetUtilities() {
		return IAssetUtilities_I::GetInstance();
	}
}

#endif  // SOURCE_COMPILING_XMPEXTENSIONS_LIB

