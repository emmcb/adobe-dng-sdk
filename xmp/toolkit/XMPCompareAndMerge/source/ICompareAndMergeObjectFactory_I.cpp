// =================================================================================================
// Copyright Adobe
// Copyright 2015 Adobe
// All Rights Reserved
//
// NOTICE:  Adobe permits you to use, modify, and distribute this file in accordance with the terms
// of the Adobe license agreement accompanying it. If you have received this file from a source other 
// than Adobe, then your use, modification, or distribution of it requires the prior written permission
// of Adobe.
// =================================================================================================


#include "XMPCompareAndMerge/Interfaces/ICompareAndMergeObjectFactory_I.h"
#include "XMPCommon/Utilities/TWrapperFunctions_I.h"
#include "XMPCommon/Interfaces/IError_I.h"

namespace AdobeXMPCompareAndMerge_Int {

	pvoid APICALL ICompareAndMergeObjectFactory_I::GetInterfacePointer( uint64 interfaceID, uint32 interfaceVersion ) {
		return GetInterfacePointerInternal( interfaceID, interfaceVersion, true );
	}

	pvoid APICALL ICompareAndMergeObjectFactory_I::getInterfacePointer( uint64 interfaceID, uint32 interfaceVersion, pcIError_base & error ) __NOTHROW__ {
		return CallUnSafeFunction< ICompareAndMergeObjectFactory_I, pvoid, pvoid, uint64, uint32 >(
			error, this, NULL, &ICompareAndMergeObjectFactory_I::GetInterfacePointer, __FILE__, __LINE__, interfaceID, interfaceVersion );
	}

	pvoid APICALL ICompareAndMergeObjectFactory_I::GetInterfacePointerInternal( uint64 interfaceID, uint32 interfaceVersion, bool isTopLevel ) {
		if ( interfaceID == kICompareAndMergeObjectFactoryID ) {
			switch ( interfaceVersion ) {
			case 1:
				return static_cast< ICompareAndMergeObjectFactory_v1 * >( this );
				break;

			case kInternalInterfaceVersionNumber:
				return this;
				break;

			default:
				throw IError_I::CreateInterfaceVersionNotAvailableError(
					IError_v1::kESOperationFatal, interfaceID, interfaceVersion, __FILE__, __LINE__ );
				break;
			}
		} else if ( interfaceID == kIObjectFactoryID ) {
			switch ( interfaceVersion ) {
			case 1:
				return static_cast< IObjectFactory_v1 * >( this );
				break;

			default:
				throw IError_I::CreateInterfaceVersionNotAvailableError(
					IError_v1::kESOperationFatal, interfaceID, interfaceVersion, __FILE__, __LINE__ );
				break;

			}
		}
		if ( isTopLevel )
			throw IError_I::CreateInterfaceNotAvailableError(
				IError_v1::kESOperationFatal, kICompareAndMergeObjectFactoryID, interfaceID, __FILE__, __LINE__ );
		return NULL;
	}

}
