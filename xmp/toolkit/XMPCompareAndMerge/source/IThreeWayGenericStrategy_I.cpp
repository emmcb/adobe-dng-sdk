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


#include "XMPCompareAndMerge/Interfaces/IThreeWayGenericStrategy_I.h"
#include "XMPCommon/Utilities/TWrapperFunctions_I.h"
#include "XMPCommon/Interfaces/IError_I.h"
#include "XMPCommon/Interfaces/IUTF8String.h"

namespace AdobeXMPCompareAndMerge_Int {

	pvoid APICALL IThreeWayGenericStrategy_I::GetInterfacePointer( uint64 interfaceID, uint32 interfaceVersion ) {
		return GetInterfacePointerInternal( interfaceID, interfaceVersion, true );
	}

	pvoid APICALL IThreeWayGenericStrategy_I::getInterfacePointer( uint64 interfaceID, uint32 interfaceVersion, pcIError_base & error ) __NOTHROW__ {
		return CallUnSafeFunction< IThreeWayGenericStrategy_I, pvoid, pvoid, uint64, uint32 >(
			error, this, NULL, &IThreeWayGenericStrategy_I::GetInterfacePointer, __FILE__, __LINE__, interfaceID, interfaceVersion );
	}

	uint32 APICALL IThreeWayGenericStrategy_I::appendPath( const char * path, sizet pathSize, pcIError_base & error ) __NOTHROW__ {
		return CallUnSafeFunction< IThreeWayGenericStrategy_v1, uint32, bool, const char *, sizet >(
			error, this, static_cast< uint32 >( 0 ), &IThreeWayGenericStrategy_v1::AppendPath, __FILE__, __LINE__, path, pathSize );
	}

	pcIUTF8String_base APICALL IThreeWayGenericStrategy_I::getPath( sizet index, pcIError_base & error ) const __NOTHROW__ {
		return CallConstUnSafeFunctionReturningSharedPointer< IThreeWayGenericStrategy_v1, pcIUTF8String_base, const IUTF8String, sizet >(
			error, this, &IThreeWayGenericStrategy_v1::GetPath, __FILE__, __LINE__, index );
	}

	pIUTF8String_base APICALL IThreeWayGenericStrategy_I::removePath( const char * path, sizet pathSize, pcIError_base & error ) __NOTHROW__ {
		return CallUnSafeFunctionReturningSharedPointer< IThreeWayGenericStrategy_v1, pIUTF8String_base, IUTF8String, const char *, sizet >(
			error, this, &IThreeWayGenericStrategy_v1::RemovePath, __FILE__, __LINE__, path, pathSize );
	}

	pIThreeWayGenericStrategy APICALL IThreeWayGenericStrategy_I::GetGenericStrategy() {
		return this;
	}

	pvoid APICALL IThreeWayGenericStrategy_I::GetInterfacePointerInternal( uint64 interfaceID, uint32 interfaceVersion, bool isTopLevel ) {
		if ( interfaceID == kIThreeWayGenericStrategyID ) {
			switch ( interfaceVersion ) {
			case 1:
				return static_cast< IThreeWayGenericStrategy_v1 * >( this );
				break;

			case kInternalInterfaceVersionNumber:
				return this;
				break;

			default:
				throw IError_I::CreateInterfaceVersionNotAvailableError(
					IError_v1::kESOperationFatal, interfaceID, interfaceVersion, __FILE__, __LINE__ );
				break;
			}
		}
		if ( isTopLevel )
			throw IError_I::CreateInterfaceNotAvailableError(
				IError_v1::kESOperationFatal, kIThreeWayGenericStrategyID, interfaceID, __FILE__, __LINE__ );
		return NULL;
	}

}
