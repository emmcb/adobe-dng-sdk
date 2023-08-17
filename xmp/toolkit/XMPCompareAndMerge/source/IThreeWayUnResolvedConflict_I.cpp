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


#include "XMPCompareAndMerge/Interfaces/IThreeWayUnResolvedConflict_I.h"
#include "XMPCommon/Utilities/TWrapperFunctions_I.h"
#include "XMPCommon/Interfaces/IError_I.h"
#include "XMPCore/Interfaces/INode.h"

namespace AdobeXMPCompareAndMerge_Int {

	pvoid APICALL IThreeWayUnResolvedConflict_I::GetInterfacePointer( uint64 interfaceID, uint32 interfaceVersion ) {
		return GetInterfacePointerInternal( interfaceID, interfaceVersion, true );
	}

	pvoid APICALL IThreeWayUnResolvedConflict_I::getInterfacePointer( uint64 interfaceID, uint32 interfaceVersion, pcIError_base & error ) __NOTHROW__ {
		return CallUnSafeFunction< IThreeWayUnResolvedConflict_I, pvoid, pvoid, uint64, uint32 >(
			error, this, NULL, &IThreeWayUnResolvedConflict_I::GetInterfacePointer, __FILE__, __LINE__, interfaceID, interfaceVersion );
	}

	uint32 APICALL IThreeWayUnResolvedConflict_I::getConflictReason( pcIError_base & error ) const __NOTHROW__ {
		return CallConstUnSafeFunction< IThreeWayUnResolvedConflict, uint32, eConflictReason >(
			error, this, 0, &IThreeWayUnResolvedConflict::GetConflictReason, __FILE__, __LINE__ );
	}

	pcINode_base APICALL IThreeWayUnResolvedConflict_I::getNode( sizet whichNode, pcIError_base & error ) const __NOTHROW__ {
		typedef spcINode( APICALL IThreeWayUnResolvedConflict::*FnPtr )( ) const;
		FnPtr fnPtr;
		switch ( whichNode ) {
		case 1:
			fnPtr = &IThreeWayUnResolvedConflict::GetBaseVersionNode;
			break;
		case 2:
			fnPtr = &IThreeWayUnResolvedConflict::GetLatestVersionNode;
			break;
		default:
			fnPtr = &IThreeWayUnResolvedConflict::GetUserVersionNode;
			break;
		}
		return CallConstUnSafeFunctionReturningSharedPointer< IThreeWayUnResolvedConflict, pcINode_base, const INode >(
			error, this, fnPtr, __FILE__, __LINE__ );
		/*pcINode_base pn = CallConstUnSafeFunctionReturningSharedPointer< IThreeWayUnResolvedConflict, pcINode_base, const INode >(
			error, this, fnPtr, __FILE__, __LINE__);
		return pn ? pn->GetActualINode() : NULL;*/
	}

	pvoid APICALL IThreeWayUnResolvedConflict_I::GetInterfacePointerInternal( uint64 interfaceID, uint32 interfaceVersion, bool isTopLevel ) {
		if ( interfaceID == kIThreeWayUnResolvedConflictID ) {
			switch ( interfaceVersion ) {
			case 1:
				return static_cast< IThreeWayUnResolvedConflict_v1 * >( this );
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
				IError_v1::kESOperationFatal, kIThreeWayUnResolvedConflictID, interfaceID, __FILE__, __LINE__ );
		return NULL;
	}

}
