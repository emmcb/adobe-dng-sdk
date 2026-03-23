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
//  SKP Sunil Kishor Pathak
//	ANS	Ankita Sharma
//
// mm-dd-yy who Description of changes, most recent first.
//
// 12-07-14 ADC 1.0-a041 Refactored Part Classes and Adding support for IAssetArtboardPart.
// 11-24-14 ANS 1.0-a007 Fixing build errors
// 11-23-14 SKP 1.0-a006 Support for IAssetPagePart interface.
//
// =================================================================================================
#endif  // AdobePrivate

#include "XMPAssetManagement/Interfaces/IAssetPagePart_I.h"
#include "XMPCommon/Utilities/TWrapperFunctions_I.h"
#include "XMPCommon/Interfaces/IUTF8String_I.h"

namespace AdobeXMPAM_Int {
	//REMOVE: Define all safe functions

	pvoid APICALL IAssetPagePart_I::GetInterfacePointer(uint64 interfaceID, uint32 interfaceVersion) {
		return GetInterfacePointerInternal(interfaceID, interfaceVersion, true);
	}

	pvoid APICALL IAssetPagePart_I::getInterfacePointer(uint64 interfaceID, uint32 interfaceVersion, pcIError_base & error) __NOTHROW__{
		return CallUnSafeFunction< IAssetPagePart_I, pvoid, pvoid, uint64, uint32 >(
		error, this, NULL, &IAssetPagePart_I::GetInterfacePointer, __FILE__, __LINE__, interfaceID, interfaceVersion);
	}

		pvoid APICALL IAssetPagePart_I::GetInterfacePointerInternal(uint64 interfaceID, uint32 interfaceVersion, bool isTopLevel) {
		pvoid returnValue(NULL);
		if (interfaceID == kIAssetPagePartID) {
			switch (interfaceVersion) {
			case 1:
				return static_cast< IAssetPagePart_v1 * >(this);
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
			IError_v1::kESOperationFatal, kIAssetPagePartID, interfaceID, __FILE__, __LINE__);
		return NULL;
	}

	pcIUTF8String_base APICALL IAssetPagePart_I::getDescriptivePageNumber( pcIError_base & error ) const __NOTHROW__ {
		return CallConstUnSafeFunctionReturningSharedPointer< IAssetPagePart_v1, pcIUTF8String_base, const IUTF8String >(
		error, this, &IAssetPagePart_v1::GetDescriptivePageNumber, __FILE__, __LINE__);
	}


	pIAssetPart_base APICALL IAssetPagePart_I::clone(pcIError_base & error) const __NOTHROW__{
		return CallConstUnSafeFunctionReturningSharedPointer< IAssetPagePart_v1, pIAssetPart_base, IAssetPart>(
		error, this, &IAssetPagePart_v1::Clone, __FILE__, __LINE__);
	}

	/*(spIAssetPart APICALL IAssetPagePart_I::Clone() const {
#if !GENERATE_XMPEXTENSIONS_CLIENT_LAYER_CODE
		return Clone_I();
#else
		return spIAssetPagePart();
#endif
	}*/
}

namespace AdobeXMPAM {
	using namespace AdobeXMPAM_Int;

#if SOURCE_COMPILING_XMPEXTENSIONS_LIB
	spIAssetPagePart IAssetPagePart_v1::CreateStandardPagePart( eAssetPartComponent component, sizet pageNumber)
	{
		return IAssetPagePart_I::CreateStandardPagePart( component, pageNumber );
	}

	spIAssetPagePart IAssetPagePart_v1::CreateUserDefinedPagePart( const char * component, sizet componentLength, sizet pageNumber )
	{
		return IAssetPagePart_I::CreateUserDefinedPagePart( component, componentLength, pageNumber );
	}
#endif  // SOURCE_COMPILING_XMPEXTENSIONS_LIB
}
