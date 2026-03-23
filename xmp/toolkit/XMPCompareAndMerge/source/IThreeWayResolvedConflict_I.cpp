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



#include "XMPCompareAndMerge/Interfaces/IThreeWayResolvedConflict_I.h"
#include "XMPCommon/Utilities/TWrapperFunctions_I.h"
#include "XMPCommon/Interfaces/IError_I.h"
#include "XMPCore/Interfaces/INode.h"

namespace AdobeXMPCompareAndMerge_Int {

	pvoid APICALL IThreeWayResolvedConflict_I::GetInterfacePointer( uint64 interfaceID, uint32 interfaceVersion ) {
		return GetInterfacePointerInternal( interfaceID, interfaceVersion, true );
	}

	pvoid APICALL IThreeWayResolvedConflict_I::getInterfacePointer( uint64 interfaceID, uint32 interfaceVersion, pcIError_base & error ) __NOTHROW__ {
		return CallUnSafeFunction< IThreeWayResolvedConflict_I, pvoid, pvoid, uint64, uint32 >(
			error, this, NULL, &IThreeWayResolvedConflict_I::GetInterfacePointer, __FILE__, __LINE__, interfaceID, interfaceVersion );
	}

	uint32 APICALL IThreeWayResolvedConflict_I::getConflictResolutionReason( pcIError_base & error ) const __NOTHROW__ {
		return CallConstUnSafeFunction< IThreeWayResolvedConflict, uint32, eConflictResolutionReason >(
			error, this, 0, &IThreeWayResolvedConflict::GetConflictResolutionReason, __FILE__, __LINE__ );
	}

	pcINode_base APICALL IThreeWayResolvedConflict_I::getNode( sizet whichNode, pcIError_base & error ) const __NOTHROW__ {
		typedef spcINode( APICALL IThreeWayResolvedConflict::*FnPtr )( ) const;
		FnPtr fnPtr;
		switch ( whichNode ) {
		case 1:
			fnPtr = &IThreeWayResolvedConflict::GetBaseVersionNode;
			break;
		case 2:
			fnPtr = &IThreeWayResolvedConflict::GetLatestVersionNode;
			break;
		case 3:
			fnPtr = &IThreeWayResolvedConflict::GetUserVersionNode;
			break;

		default:
			fnPtr = &IThreeWayResolvedConflict::GetResolvedNode;
		}
		return CallConstUnSafeFunctionReturningSharedPointer< IThreeWayResolvedConflict, pcINode_base, const INode >(
			error, this, fnPtr, __FILE__, __LINE__ );
		/*pcINode_base pn = CallConstUnSafeFunctionReturningSharedPointer< IThreeWayResolvedConflict, pcINode_base, const INode >(
			error, this, fnPtr, __FILE__, __LINE__);
		return pn?pn->GetActualINode():NULL;*/
	}

	uint32 APICALL IThreeWayResolvedConflict_I::getResolvedSource( pcIError_base & error ) const __NOTHROW__ {
		return CallConstUnSafeFunction< IThreeWayResolvedConflict, uint32, eResolvedSource >(
			error, this, 0, &IThreeWayResolvedConflict::GetResolvedSource, __FILE__, __LINE__ );
	}

	pvoid APICALL IThreeWayResolvedConflict_I::GetInterfacePointerInternal( uint64 interfaceID, uint32 interfaceVersion, bool isTopLevel ) {
		if ( interfaceID == kIThreeWayResolvedConflictID ) {
			switch ( interfaceVersion ) {
			case 1:
				return static_cast< IThreeWayResolvedConflict_v1 * >( this );
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
				IError_v1::kESOperationFatal, kIThreeWayResolvedConflictID, interfaceID, __FILE__, __LINE__ );
		return NULL;
	}

}
