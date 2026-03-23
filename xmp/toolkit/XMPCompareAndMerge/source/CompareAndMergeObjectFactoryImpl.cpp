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

#define IMPLEMENTATION_HEADERS_CAN_BE_INCLUDED 1
	#include "XMPCompareAndMerge/ImplHeaders/CompareAndMergeObjectFactoryImpl.h"
#undef IMPLEMENTATION_HEADERS_CAN_BE_INCLUDED

#include "XMPCommon/Interfaces/IError_I.h"
#include "XMPCompareAndMerge/XMPCompareAndMergeErrorCodes.h"

#include "XMPCompareAndMerge/Interfaces/IThreeWayMerge_I.h"
#include "XMPCompareAndMerge/Interfaces/IGenericStrategyDatabase_I.h"

#include "XMPCommon/Interfaces/IUTF8String_I.h"
#include "XMPCommon/Utilities/TWrapperFunctions_I.h"
#include "XMPCompareAndMerge/XMPCompareAndMerge.h"


namespace AdobeXMPCompareAndMerge_Int {


	AdobeXMPCompareAndMerge::pIThreeWayMerge_base APICALL CompareAndMergeObjectFactoryImpl::CreateThreeWayMerge( pcIError_base & error ) __NOTHROW__ {
		return ReturnRawPointerFromSharedPointer< IThreeWayMerge, pIThreeWayMerge_base >(
			&IThreeWayMerge::CreateThreeWayMerge, error, __FILE__, __LINE__ );
	}

	AdobeXMPCompareAndMerge::pcIGenericStrategyDatabase_base APICALL CompareAndMergeObjectFactoryImpl::GetGenericStrategyDatabase( pcIError_base & error ) __NOTHROW__ {
		return ReturnRawPointerFromSharedPointer< const IGenericStrategyDatabase, pcIGenericStrategyDatabase_base >(
			&IGenericStrategyDatabase::GetGenericStrategyDatabase, error, __FILE__, __LINE__ );
	}

	pIError_base APICALL CompareAndMergeObjectFactoryImpl::CreateError( uint32 domain, uint32 code, uint32 severity, pcIError_base & error ) __NOTHROW__ {
		return ReturnRawPointerFromSharedPointer< IError_I, pIError_base, IError::eErrorDomain, IError::eErrorCode, IError::eErrorSeverity >(
			&IError_I::CreateError, error, __FILE__, __LINE__, static_cast< IError::eErrorDomain >( domain ), static_cast< IError::eErrorCode >( code ), static_cast< IError::eErrorSeverity >( severity ) );
	}

	pIUTF8String_base APICALL CompareAndMergeObjectFactoryImpl::CreateUTF8String( const char * buf, sizet count, pcIError_base & error ) __NOTHROW__ {
		return ReturnRawPointerFromSharedPointer< IUTF8String, pIUTF8String_base >(
			&IUTF8String_I::CreateUTF8String, error, __FILE__, ( sizet ) __LINE__, buf, count );
	}

	static pICompareAndMergeObjectFactory ManageCompareAndMergeObjectFactory( bool destroy = false ) {
		static CompareAndMergeObjectFactoryImpl coreObjectFactoryImplObj;
		return &coreObjectFactoryImplObj;
	}

}

namespace AdobeXMPCompareAndMerge {

	void ICompareAndMergeObjectFactory_v1::SetupCompareAndMergeObjectFactory() {
		AdobeXMPCompareAndMerge_Int::ManageCompareAndMergeObjectFactory();
	}

	pICompareAndMergeObjectFactory ICompareAndMergeObjectFactory_v1::GetCompareAndMergeObjectFactory() {
		return AdobeXMPCompareAndMerge_Int::ManageCompareAndMergeObjectFactory();
	}

	void ICompareAndMergeObjectFactory_v1::DestroyCompareAndMergeObjectFactory() {
		AdobeXMPCompareAndMerge_Int::ManageCompareAndMergeObjectFactory( true );
	}
}

AdobeXMPCompareAndMerge::pICompareAndMergeObjectFactory_base XMPCM_GetObjectFactoryInstance() {
	return AdobeXMPCompareAndMerge_Int::ManageCompareAndMergeObjectFactory();
}
