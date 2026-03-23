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


#include "XMPCompareAndMerge/Interfaces/IThreeWayMergeResult_I.h"
#include "XMPCompareAndMerge/Interfaces/IThreeWayResolvedConflict.h"
#include "XMPCompareAndMerge/Interfaces/IThreeWayUnResolvedConflict.h"

#include "XMPCommon/Utilities/TWrapperFunctions_I.h"
#include "XMPCommon/Interfaces/IError_I.h"

#include "XMPCore/Interfaces/IMetadata.h"

namespace AdobeXMPCompareAndMerge_Int {

	pvoid APICALL IThreeWayMergeResult_I::GetInterfacePointer( uint64 interfaceID, uint32 interfaceVersion ) {
		return GetInterfacePointerInternal( interfaceID, interfaceVersion, true );
	}

	pvoid APICALL IThreeWayMergeResult_I::getInterfacePointer( uint64 interfaceID, uint32 interfaceVersion, pcIError_base & error ) __NOTHROW__ {
		return CallUnSafeFunction< IThreeWayMergeResult_I, pvoid, pvoid, uint64, uint32 >(
			error, this, NULL, &IThreeWayMergeResult_I::GetInterfacePointer, __FILE__, __LINE__, interfaceID, interfaceVersion );
	}

	pIMetadata_base APICALL IThreeWayMergeResult_I::getMergedMetadata( pcIError_base & err ) const __NOTHROW__ {
		return CallConstUnSafeFunctionReturningSharedPointer< IThreeWayMergeResult_v1, pIMetadata_base, IMetadata >(
			err, this, &IThreeWayMergeResult_v1::GetMergedMetadata, __FILE__, __LINE__ );
		/*pIMetadata_base mt = CallConstUnSafeFunctionReturningSharedPointer< IThreeWayMergeResult_v1, pIMetadata_base, IMetadata >(
		err, this, &IThreeWayMergeResult_v1::GetMergedMetadata, __FILE__, __LINE__);
		return mt ? mt->GetActualIMetadata() : NULL;
		*/
		/*spIMetadata spMetadata = CallConstUnSafeFunction< IThreeWayMergeResult_v1, spIMetadata, spIMetadata >(
			err, this, spIMetadata(), &IThreeWayMergeResult_v1::GetMergedMetadata,__FILE__,__LINE__);
		if (spMetadata) {
			spMetadata->GetActualIMetadata()->Acquire();
		}
		return spMetadata?spMetadata->GetActualIMetadata():NULL;*/

	}

	uint32 APICALL IThreeWayMergeResult_I::getMergedStatus( pcIError_base & error ) const __NOTHROW__ {
		return CallConstUnSafeFunction< IThreeWayMergeResult_v1, uint32, eMergeStatus >(
			error, this, static_cast< uint32 >( kMSMaxValue ), &IThreeWayMergeResult_v1::GetMergedStatus, __FILE__, __LINE__ );
	}

	pcIThreeWayResolvedConflict_base APICALL IThreeWayMergeResult_I::getResolvedConflict( sizet index, pcIError_base & error ) const __NOTHROW__ {
		return CallConstUnSafeFunctionReturningSharedPointer< IThreeWayMergeResult_v1, pcIThreeWayResolvedConflict_base, const IThreeWayResolvedConflict, sizet >(
			error, this, &IThreeWayMergeResult_v1::GetResolvedConflict, __FILE__, __LINE__, index );
	}

	pcIThreeWayUnResolvedConflict_base APICALL IThreeWayMergeResult_I::getUnResolvedConflict( sizet index, pcIError_base & error ) const __NOTHROW__ {
		return CallConstUnSafeFunctionReturningSharedPointer< IThreeWayMergeResult_v1, pcIThreeWayUnResolvedConflict_base, const IThreeWayUnResolvedConflict, sizet >(
			error, this, &IThreeWayMergeResult_v1::GetUnResolvedConflict, __FILE__, __LINE__, index );
	}

	pvoid APICALL IThreeWayMergeResult_I::GetInterfacePointerInternal( uint64 interfaceID, uint32 interfaceVersion, bool isTopLevel ) {
		if ( interfaceID == kIThreeWayMergeResultID ) {
			switch ( interfaceVersion ) {
			case 1:
				return static_cast< IThreeWayMergeResult_v1 * >( this );
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
				IError_v1::kESOperationFatal, kIThreeWayMergeResultID, interfaceID, __FILE__, __LINE__ );
		return NULL;
	}

}
