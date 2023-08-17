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

#define IMPLEMENTATION_HEADERS_CAN_BE_INCLUDED 1
	#include "XMPCompareAndMerge/ImplHeaders/GenericStrategyDatabaseImpl.h"
	#include "XMPCompareAndMerge/ImplHeaders/LatestDateThreeWayGenericStrategyImpl.h"
	#include "XMPCompareAndMerge/ImplHeaders/OldestDateThreeWayGenericStrategyImpl.h"
	#include "XMPCompareAndMerge/ImplHeaders/HistoryThreeWayGenericStrategyImpl.h"
	#include "XMPCompareAndMerge/ImplHeaders/MarkerThreeWayGenericStrategyImpl.h"
#undef IMPLEMENTATION_HEADERS_CAN_BE_INCLUDED

#include "XMPCommon/Interfaces/IError_I.h"
#include "XMPCompareAndMerge/XMPCompareAndMergeErrorCodes.h"
#include "XMPCommon/Utilities/TSmartPointers_I.h"

namespace AdobeXMPCompareAndMerge_Int {

	spIThreeWayGenericStrategy APICALL GenericStrategyDatabaseImpl::GetThreeWayGenericStrategy( eGenericStrategyType genericStrategyType ) const {
		switch ( genericStrategyType ) {
		case kGSTLatestDate:
			return LatestDateThreeWayGenericStrategyImpl::CreateLatestDateThreeWayGenericStrategy();
			break;

		case kGSTOldestDate:
			return OldestDateThreeWayGenericStrategyImpl::CreateOldestDateThreeWayGenericStrategy();
			break;

		case kGSTHistory:
			return HistoryThreeWayGenericStrategyImpl::CreateHistoryThreeWayGenericStrategy();
			break;

		case kGSTMarker:
			return MarkerThreeWayGenericStrategyImpl::CreateMarkerThreeWayGenericStrategy();
			break;

		default:
			NOTIFY_ERROR( IError::kEDGenericStrategyDatabase, kGSDbNotAvailable, "Requested Type is not available with the library", IError::kESOperationFatal,
				true, static_cast< uint64 >( genericStrategyType ) );
		}
	}
    
    static spIGenericStrategyDatabase ManageGenericStrategyDatabase( bool destroy = false ) {
		static spIGenericStrategyDatabase sGenericStrategyDatabase;
		if ( destroy ) {
			sGenericStrategyDatabase.reset();
		} else {
			if ( !sGenericStrategyDatabase )
				sGenericStrategyDatabase = MakeUncheckedSharedPointer( new GenericStrategyDatabaseImpl(), __FILE__, __LINE__, true );
		}
		return sGenericStrategyDatabase;
	}

	spIGenericStrategyDatabase IGenericStrategyDatabase_I::CreateGenericStrategyDatabase() {
		return ManageGenericStrategyDatabase();
	}

	void IGenericStrategyDatabase_I::DestoryGenericStrategyDatabase() {
		ManageGenericStrategyDatabase( true );
	}
}

namespace AdobeXMPCompareAndMerge {
	using namespace AdobeXMPCompareAndMerge_Int;

	spIGenericStrategyDatabase IGenericStrategyDatabase_v1::MakeShared( pIGenericStrategyDatabase_base ptr ) {
		if ( !ptr ) return spIGenericStrategyDatabase();
		pIGenericStrategyDatabase p = IGenericStrategyDatabase::GetInterfaceVersion() > 1 ? ptr->GetInterfacePointer< IGenericStrategyDatabase >() : ptr;
		return MakeUncheckedSharedPointer( p, __FILE__, __LINE__, false );
	}

	spcIGenericStrategyDatabase IGenericStrategyDatabase_v1::GetGenericStrategyDatabase() {
		return ManageGenericStrategyDatabase();
	}
}
