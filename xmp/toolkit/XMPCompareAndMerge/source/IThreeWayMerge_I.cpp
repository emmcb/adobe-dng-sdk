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


#include "XMPCompareAndMerge/Interfaces/IThreeWayMerge_I.h"
#include "XMPCommon/Utilities/TWrapperFunctions_I.h"
#include "XMPCommon/Interfaces/IError_I.h"
#include "XMPCompareAndMerge/Interfaces/IThreeWayGenericStrategy_I.h"
#include "XMPCompareAndMerge/Interfaces/IThreeWayMergeResult.h"
#include "XMPCommon/Utilities/TSmartPointers_I.h"
#include "XMPCommon/Interfaces/IUTF8String.h"
#include "XMPCore/Interfaces/IMetadata.h"
#include "public/include/client-glue/WXMPMeta.hpp"

#define IMPLEMENTATION_HEADERS_CAN_BE_INCLUDED 1
	#include "XMPCompareAndMerge/ImplHeaders/ThreeWayStrategyWrapperImpl.h"
#undef IMPLEMENTATION_HEADERS_CAN_BE_INCLUDED

namespace AdobeXMPCompareAndMerge_Int {

	bool APICALL IThreeWayMerge_I::RegisterThreeWayStrategy( pIThreeWayStrategy_base strategy ) {
		auto spStrategy = ThreeWayStrategyWrapperImpl::CreateThreeWayStrategy( strategy );
		return RegisterThreeWayStrategy( spStrategy );
	}

	pvoid APICALL IThreeWayMerge_I::GetInterfacePointer( uint64 interfaceID, uint32 interfaceVersion ) {
		return GetInterfacePointerInternal( interfaceID, interfaceVersion, true );
	}

	pvoid APICALL IThreeWayMerge_I::getInterfacePointer( uint64 interfaceID, uint32 interfaceVersion, pcIError_base & error ) __NOTHROW__ {
		return CallUnSafeFunction< IThreeWayMerge_I, pvoid, pvoid, uint64, uint32 >(
			error, this, NULL, &IThreeWayMerge_I::GetInterfacePointer, __FILE__, __LINE__, interfaceID, interfaceVersion );
	}

	bool APICALL IThreeWayMerge_I::RegisterThreeWayGenericStrategy( const spIThreeWayGenericStrategy & strategy ) {
		auto sp = MakeUncheckedSharedPointer( strategy->GetIThreeWayGenericStrategy_I(), __FILE__, __LINE__, true );
		return RegisterThreeWayStrategy( sp );
	}

	pcIThreeWayMergeResult_base APICALL IThreeWayMerge_I::merge( pcIMetadata_base baseVersionMetadata, pcIMetadata_base latestVersionMedata, pcIMetadata_base userVersionMetadata, pcIError_base & error ) const __NOTHROW__ {
		return CallConstUnSafeFunctionReturningSharedPointer< IThreeWayMerge_v1, pcIThreeWayMergeResult_base, const IThreeWayMergeResult, const spcIMetadata &, const spcIMetadata &, const spcIMetadata & >(
			error, this, &IThreeWayMerge_v1::Merge, __FILE__, __LINE__, IMetadata::MakeShared( baseVersionMetadata ),
			IMetadata::MakeShared( latestVersionMedata ), IMetadata::MakeShared( userVersionMetadata ) );
	}

	pcIThreeWayMergeResult_base APICALL IThreeWayMerge_I::autoMerge ( pcIMetadata_base baseVersionMetadata, pcIMetadata_base latestVersionMedata, pcIMetadata_base userVersionMetadata, uint32 skipAutoMergeStrategy, pcIError_base & error ) const __NOTHROW__ {
		typedef spcIThreeWayMergeResult ( APICALL IThreeWayMerge_v1::*Func ) ( const spcIMetadata &, const spcIMetadata &, const spcIMetadata &, eAutoMergeStrategy ) const;
		Func fnPtr = &IThreeWayMerge_v1::AutoMerge;
		return CallConstUnSafeFunctionReturningSharedPointer< IThreeWayMerge_v1, pcIThreeWayMergeResult_base, const IThreeWayMergeResult, const spcIMetadata &, const spcIMetadata &, const spcIMetadata &, eAutoMergeStrategy > (
			error, this, fnPtr, __FILE__, __LINE__, IMetadata::MakeShared ( baseVersionMetadata ),
			IMetadata::MakeShared ( latestVersionMedata ), IMetadata::MakeShared ( userVersionMetadata ), static_cast<eAutoMergeStrategy> ( skipAutoMergeStrategy ) );
	}

	pcIUTF8String_base APICALL IThreeWayMerge_I::autoMerge ( const char * baseVersionMetadata, sizet baseVersionMetadataLength, const char * latestVersionMetadata, sizet latestVersionMetadataLength, const char * userVersionMetadata, sizet userVersionMetadataLength, uint32 & isMerged, uint32 skipAutoMergeStrategy, pcIError_base & error ) const __NOTHROW__ {
		typedef spcIUTF8String ( APICALL IThreeWayMerge_v1::*Func ) ( const char *, sizet, const char *, sizet, const char *, sizet, bool& ,eAutoMergeStrategy ) const;
		Func fnPtr = &IThreeWayMerge_v1::AutoMerge;
		bool boolCast = false;
		auto returnValue = CallConstUnSafeFunctionReturningSharedPointer< IThreeWayMerge_v1, pcIUTF8String_base,const IUTF8String, const char *, sizet, const char *,sizet, const char *,sizet, bool & , eAutoMergeStrategy > (
			error, this, fnPtr, __FILE__, __LINE__, baseVersionMetadata, baseVersionMetadataLength, latestVersionMetadata, latestVersionMetadataLength,
			userVersionMetadata, userVersionMetadataLength, boolCast,static_cast<eAutoMergeStrategy> ( skipAutoMergeStrategy ) );
		isMerged = static_cast< uint32 >( boolCast) ;
		return returnValue;
	}
    
     XMPMetaRef APICALL IThreeWayMerge_I::autoMerge ( XMPMetaRef baseVersionMetadata, XMPMetaRef latestVersionMetadata, XMPMetaRef userVersionMetadata, uint32 & isMerged, uint32 skipAutoMergeStrategy, pcIError_base & error ) const __NOTHROW__
    {
        typedef SXMPMeta ( APICALL IThreeWayMerge_v1::*Func ) ( const SXMPMeta *, const SXMPMeta *, const SXMPMeta *, bool& ,eAutoMergeStrategy ) const;
        Func fnPtr = &IThreeWayMerge_v1::AutoMerge;
        bool boolCast = false;
        
        SXMPMeta baseVersion(baseVersionMetadata);
        SXMPMeta latestVersion(latestVersionMetadata);
        SXMPMeta userVersion(userVersionMetadata);
        
        error = NULL;
        try {
            SXMPMeta mergedMeta = AutoMerge(&baseVersion, &latestVersion, &userVersion, boolCast,(eAutoMergeStrategy)skipAutoMergeStrategy);
            isMerged = static_cast< uint32 >( boolCast) ;
            WXMPMeta_IncrementRefCount_1(mergedMeta.GetInternalRef());
            return mergedMeta.GetInternalRef();
        }
        catch (spcIError err) {
            error = err->GetActualIError();
            error->GetISharedObject_I()->AcquireInternal();
        }
        catch (...) {
            pIError_I err = IError_I::CreateUnknownExceptionCaughtError(IError_v1::kESOperationFatal, __FILE__, __LINE__).get();
            err->AcquireInternal();
            error = err;
        }
        isMerged = static_cast< uint32 >( boolCast) ;
        return NULL;
    }
	 

	uint32 APICALL IThreeWayMerge_I::registerThreeWayGenericStrategy( pIThreeWayGenericStrategy_base strategy, pcIError_base & error ) __NOTHROW__ {
		typedef bool ( APICALL IThreeWayMerge_I::*FnPtr )( const spIThreeWayStrategy & );
		FnPtr fnPtr = &IThreeWayMerge_I::RegisterThreeWayStrategy;
		return CallUnSafeFunction< IThreeWayMerge_I, uint32, bool, const spIThreeWayStrategy & >(
			error, this, 0, &IThreeWayMerge_I::RegisterThreeWayStrategy, __FILE__, __LINE__,
			MakeUncheckedSharedPointer( strategy->GetIThreeWayGenericStrategy_I(), __FILE__, __LINE__, true ) );
	}

	uint32 APICALL IThreeWayMerge_I::registerThreeWayStrategy( pIThreeWayStrategy_base strategy, pcIError_base & error ) __NOTHROW__ {
		typedef bool ( APICALL IThreeWayMerge_I::*FnPtr )( const spIThreeWayStrategy & );
		FnPtr fnPtr = &IThreeWayMerge_I::RegisterThreeWayStrategy;
		return CallUnSafeFunction< IThreeWayMerge_I, uint32, bool, const spIThreeWayStrategy & >(
			error, this, 0, fnPtr, __FILE__, __LINE__, ThreeWayStrategyWrapperImpl::CreateThreeWayStrategy( strategy ) );
	}

	pvoid APICALL IThreeWayMerge_I::GetInterfacePointerInternal( uint64 interfaceID, uint32 interfaceVersion, bool isTopLevel ) {
		if ( interfaceID == kIThreeWayMergeID ) {
			switch ( interfaceVersion ) {
			case 1:
				return static_cast< IThreeWayMerge_v1 * >( this );
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
				IError_v1::kESOperationFatal, kIThreeWayMergeID, interfaceID, __FILE__, __LINE__ );
		return NULL;
	}

}
