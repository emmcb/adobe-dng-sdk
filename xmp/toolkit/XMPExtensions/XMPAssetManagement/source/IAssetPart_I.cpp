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
// ADC	Amandeep Chawla
// SKP	Sunil Kishor Pathak
//
// mm-dd-yy who Description of changes, most recent first.
//
// 12-07-14 ADC 1.0-a041 Refactored Part Classes and Adding support for IAssetArtboardPart.
// 12-03-14 SKP 1.0-a034 Fixed issues in creating relationship from existing ingredient.
// 12-02-14 SKP 1.0-a028 1. Moved standardComponentStrings in IAssetPart_I.cpp so that it can be
//							used in other cpp files, 2. fixed issues in SerializeAsRDFSyntax(),
//							3. Fixed issue in GetAssetPartComponentIndex() and 4. fixed issues
//							while composing from composed component.
// 11-28-14 SKP 1.0-a018 Fixed issues in genericpart addition & history entry
// 11-27-14 ADC 1.0-a016 Submitting changes related to iOS build.
// 11-26-14 SKP 1.0-a010 Implemented function for parsing string and creating parts for relation
// 11-21-14 ADC 1.0-a002 Support for IAssetPart and IAssetTimePart interfaces.
// 11-19-14 ADC 1.0-a001 Framework in place for Asset Management Library.
//
// =================================================================================================
#endif  // AdobePrivate

#include "XMPAssetManagement/Interfaces/IAssetPart_I.h"
#include "XMPAssetManagement/Interfaces/IAssetPagePart_I.h"
#include "XMPAssetManagement/Interfaces/IAssetLayerPart_I.h"
#include "XMPAssetManagement/Interfaces/IAssetTimePart_I.h"
#include "XMPAssetManagement/Interfaces/IAssetArtboardPart_I.h"

#include "XMPCommon/XMPCommonErrorCodes.h"
#include "XMPCommon/Utilities/TWrapperFunctions_I.h"
#include "XMPCommon/Interfaces/IUTF8String_I.h"
#include "XMPCommon/Utilities/UTF8String.h"
#include "XMPCommon/Interfaces/IErrorNotifier_I.h"
#include "XMPAssetManagement/XMPAssetManagementErrorCodes.h"

#include <algorithm>

namespace AdobeXMPAM_Int {

	pvoid APICALL IAssetPart_I::GetInterfacePointer(uint64 interfaceID, uint32 interfaceVersion) {
		return GetInterfacePointerInternal(interfaceID, interfaceVersion, true);
	}

	pvoid APICALL IAssetPart_I::getInterfacePointer(uint64 interfaceID, uint32 interfaceVersion, pcIError_base & error) __NOTHROW__{
		return CallUnSafeFunction< IAssetPart_I, pvoid, pvoid, uint64, uint32 >(
		error, this, NULL, &IAssetPart_I::GetInterfacePointer, __FILE__, __LINE__, interfaceID, interfaceVersion);
	}

		pvoid APICALL IAssetPart_I::GetInterfacePointerInternal(uint64 interfaceID, uint32 interfaceVersion, bool isTopLevel) {
		if (interfaceID == kIAssetPartID) {
			switch (interfaceVersion) {
			case 1:
				return static_cast< IAssetPart_v1 * >(this);
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
			IError_v1::kESOperationFatal, kIAssetPartID, interfaceID, __FILE__, __LINE__);
		return NULL;
	}


	spIAssetPart_I IAssetPart_I::ParseAndCreatePartFromRDFSyntax( const char * str1, sizet length )
	{
		UTF8String str( str1 );

		if ( str.length() < 2 ) {
			return IAssetPart_I::CreateUserDefinedGenericPart( str1 ,length);
		}

		sizet colonPos = str.rfind(":");

		if ( colonPos != std::string::npos ) {
			sizet colonPos2 = str.find( ":" );
			if ( colonPos2 != colonPos )
				NOTIFY_ERROR( IError_v1::kEDAssetManagement, IErrorAssetManagement::kPartNotWellFormed,
					"colon is present more than once", IError_v1::kESOperationFatal,
					true, str1 );
		}
		
		UTF8String value;
		if ( std::string::npos == colonPos ) {
			// It's a part without value so should be treated as generic part
			return IAssetPart_I::CreateUserDefinedGenericPart( str1, length);
		} else {
			// It can be one specialized part which is supported by the library.
			UTF8String value = str.substr( colonPos + 1 );

			// Find the pos of last "/" before ":"
			sizet slashPos = str.rfind( '/', colonPos );

			UTF8String partType;
			UTF8String component;
			if ( std::string::npos == slashPos ) {
				partType = str.substr( 0, colonPos );
			} else {
				partType = str.substr( slashPos + 1, colonPos - slashPos - 1 );
				component = str.substr( 0, slashPos );
			}
			std::transform( partType.begin(), partType.end(), partType.begin(), ::tolower );
			static sizet nEntries = sizeof( partTypeStrings ) / sizeof ( const char * ), i = 0;

			for ( i = 0; i < nEntries; i++ ) {
				if ( partType.compare( partTypeStrings[i] ) == 0 )
					break;
			}

			if ( i < nEntries ) {
				switch( static_cast< IAssetPart_v1::eAssetPartType >( i ) ) {
					case IAssetPart_v1::kAssetPartTypePage:
						return IAssetPagePart_I::CreatePartFromRDFSyntax(component.c_str(), component.size(), value.c_str(), value.size());
						break;

					case IAssetPart_v1::kAssetPartTypeLayer:
						return IAssetLayerPart_I::CreateUserDefinedLayerPart( component.c_str(), component.size(), value.c_str(), value.size());
						break;

					case IAssetPart_v1::kAssetPartTypeArtboard:
						return IAssetArtboardPart_I::CreateUserDefinedArtboardPart( component.c_str(), component.size(), value.c_str(), value.size());
						break;

					case IAssetPart_v1::kAssetPartTypeTime:
						return IAssetTimePart_I::CreatePartFromRDFSyntax( component.c_str(),component.size(), value.c_str(), value.size());
						break;

					default:
						return IAssetPart_I::CreateUserDefinedGenericPart( str1, length);
						break;
				}
			} else {
				return IAssetPart_I::CreateUserDefinedGenericPart( str1, length);
			}
		}
	}

	uint32 APICALL IAssetPart_I::getType(pcIError_base & error) const __NOTHROW__{
		return CallConstUnSafeFunction< IAssetPart_v1, uint32, eAssetPartType >(
		error, this, kAssetPartTypeMaxValue, &IAssetPart_v1::GetType, __FILE__, __LINE__);
	}

	uint32 APICALL IAssetPart_I::getComponent(pcIError_base & error) const __NOTHROW__{
		return CallConstUnSafeFunction< IAssetPart_v1, uint32, eAssetPartComponent >(
		error, this, kAssetPartComponentMaxValue, &IAssetPart_v1::GetComponent, __FILE__, __LINE__);
	}

	pcIUTF8String_base APICALL IAssetPart_I::getUserDefinedComponent(pcIError_base & error) const __NOTHROW__{
		return CallConstUnSafeFunctionReturningSharedPointer< IAssetPart_v1, pcIUTF8String_base, const IUTF8String >(
		error, this, &IAssetPart_v1::GetUserDefinedComponent, __FILE__, __LINE__);
	}

	pIAssetPart_base APICALL IAssetPart_I::clone(pcIError_base & error ) const __NOTHROW__ {
		return CallConstUnSafeFunctionReturningSharedPointer< IAssetPart_v1, pIAssetPart_base, IAssetPart>(
		error, this, &IAssetPart_v1::Clone, __FILE__, __LINE__);
	}

	/*spcIUTF8String APICALL IAssetPart_I::GetUserDefinedComponent() const {
		return GetUserDefinedComponent_I();
	}*/

	/*spIAssetPart APICALL IAssetPart_I::Clone() const {
#if !GENERATE_XMPEXTENSIONS_CLIENT_LAYER_CODE
		return Clone_I();
#else
		return spIAssetPart();
#endif
	}*/

}

#if SOURCE_COMPILING_XMPEXTENSIONS_LIB

namespace AdobeXMPAM{
	using namespace AdobeXMPAM_Int;

	spIAssetPart IAssetPart_v1::CreateStandardGenericPart( eAssetPartComponent component)
	{
		return IAssetPart_I::CreateStandardGenericPart( component);
	}

	spIAssetPart IAssetPart_v1::CreateUserDefinedGenericPart( const char * component, sizet length)
	{
		return IAssetPart_I::CreateUserDefinedGenericPart( component , length);
	}

}

#endif  // SOURCE_COMPILING_XMPEXTENSIONS_LIB

