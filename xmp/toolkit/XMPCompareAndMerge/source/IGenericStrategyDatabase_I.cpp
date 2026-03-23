// =================================================================================================
// Copyright Adobe
// Copyright 2014 Adobe
// All Rights Reserved
//
// NOTICE:  Adobe permits you to use, modify, and distribute this file in accordance with the terms
// of the Adobe license agreement accompanying it. If you have received this file from a source other 
// than Adobe, then your use, modification, or distribution of it requires the prior written permission
// of Adobe.
// =================================================================================================


#include "XMPCompareAndMerge/Interfaces/IGenericStrategyDatabase_I.h"
#include "XMPCommon/Utilities/TWrapperFunctions_I.h"
#include "XMPCommon/Interfaces/IError_I.h"
#include "XMPCompareAndMerge/Interfaces/IThreeWayGenericStrategy.h"

namespace AdobeXMPCompareAndMerge_Int {

	pvoid APICALL IGenericStrategyDatabase_I::GetInterfacePointer( uint64 interfaceID, uint32 interfaceVersion ) {
		return GetInterfacePointerInternal( interfaceID, interfaceVersion, true );
	}

	pvoid APICALL IGenericStrategyDatabase_I::getInterfacePointer( uint64 interfaceID, uint32 interfaceVersion, pcIError_base & error ) __NOTHROW__ {
		return CallUnSafeFunction< IGenericStrategyDatabase_I, pvoid, pvoid, uint64, uint32 >(
			error, this, NULL, &IGenericStrategyDatabase_I::GetInterfacePointer, __FILE__, __LINE__, interfaceID, interfaceVersion );
	}

	pIThreeWayGenericStrategy_base APICALL IGenericStrategyDatabase_I::getThreeWayGenericStrategy( uint32 genericStrategyType, pcIError_base & error ) const __NOTHROW__ {
		return CallConstUnSafeFunctionReturningSharedPointer< IGenericStrategyDatabase_v1, pIThreeWayGenericStrategy_base, IThreeWayGenericStrategy, eGenericStrategyType >(
			error, this, &IGenericStrategyDatabase_v1::GetThreeWayGenericStrategy, __FILE__, __LINE__, static_cast< eGenericStrategyType >( genericStrategyType ) );
	}

	pvoid APICALL IGenericStrategyDatabase_I::GetInterfacePointerInternal( uint64 interfaceID, uint32 interfaceVersion, bool isTopLevel ) {
		if ( interfaceID == kIGenericStrategyDatabaseID ) {
			switch ( interfaceVersion ) {
			case 1:
				return static_cast< IGenericStrategyDatabase_v1 * >( this );
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
				IError_v1::kESOperationFatal, kIGenericStrategyDatabaseID, interfaceID, __FILE__, __LINE__ );
		return NULL;
	}

}
