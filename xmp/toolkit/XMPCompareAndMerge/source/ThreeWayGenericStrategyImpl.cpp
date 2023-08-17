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
	#include "XMPCompareAndMerge/ImplHeaders/ThreeWayGenericStrategyImpl.h"
#undef IMPLEMENTATION_HEADERS_CAN_BE_INCLUDED

#include "XMPCommon/Interfaces/IError_I.h"
#include "XMPCompareAndMerge/XMPCompareAndMergeErrorCodes.h"

#include "XMPCommon/Interfaces/IUTF8String_I.h"
#include "XMPCommon/Utilities/TSmartPointers_I.h"
#include <assert.h>
#include <algorithm>

#define  STATIC_SAFE_API
#include "source/SafeStringAPIs.h"

namespace AdobeXMPCompareAndMerge_Int {
	
	static bool ValidateInputPathParameters( const char * path, sizet pathSize ) {
		if ( !path || pathSize == 0 || ( pathSize == AdobeXMPCommon::npos && strnlen_safe( path, AdobeXMPCommon::npos ) == 0 ) ) {
			return false;
		}
		return true;
	}

	ThreeWayGenericStrategyImpl::ThreeWayGenericStrategyImpl()
		: mPaths( MakeUncheckedSharedPointerWithDefaultDelete( new IUTF8Strings( gTAllocatorUTF8String ), __FILE__, __LINE__, true ) ) { }

	bool APICALL ThreeWayGenericStrategyImpl::AppendPath( const char * path, sizet pathSize ) {
		if (!ValidateInputPathParameters(path, pathSize))
			return false;
		if (pathSize == AdobeXMPCommon::npos)
			pathSize = strnlen_safe(path, AdobeXMPCommon::npos);
		for (auto it = mPaths->begin(); it != mPaths->end(); it++)
		{
			sizet registeredPathSize = (*it)->size();
			if ((pathSize == registeredPathSize) && (strncmp(path, (*it)->c_str(), registeredPathSize) == 0)){
				return false;
			}
		}
		spIUTF8String pathStr = IUTF8String_I::CreateUTF8String(path, pathSize);
		mPaths->push_back(pathStr);
		return true;

	}

	spcIUTF8String APICALL ThreeWayGenericStrategyImpl::GetPath( sizet index ) const {
		if ( index >= mPaths->size() ) {
			NOTIFY_ERROR( IError::kEDGeneral, kGECIndexOutOfBounds, "index is out of bounds", IError::kESOperationFatal, true, index, true, (uint64) mPaths->size() );
		}
		return mPaths->at( index );
	}

	const spcIUTF8Strings_const APICALL ThreeWayGenericStrategyImpl::GetPaths() const {
		auto rawPtr = new cIUTF8Strings( mPaths->begin(), mPaths->end(), gTAllocatorUTF8String );
		return MakeUncheckedSharedPointerWithDefaultDelete( rawPtr, __FILE__, __LINE__, true );
	}

	sizet APICALL ThreeWayGenericStrategyImpl::PathCount() const __NOTHROW__ {
		return mPaths->size();
	}

	spIUTF8String APICALL ThreeWayGenericStrategyImpl::RemovePath( const char * path, sizet pathSize ) {
		if (!ValidateInputPathParameters(path, pathSize))
			return spIUTF8String();

		if (pathSize == AdobeXMPCommon::npos)
			pathSize = strnlen_safe(path, AdobeXMPCommon::npos);

		spIUTF8String pathStr;
		for (auto it = mPaths->begin(); it != mPaths->end(); it++)
		{
			sizet registeredPathSize = (*it)->size();
			if ((pathSize == registeredPathSize) && (strncmp(path, (*it)->c_str(), registeredPathSize) == 0)){
				pathStr = IUTF8String_I::CreateUTF8String(path, pathSize);
				it = mPaths->erase(it);
				return pathStr;
			}
		}
		return pathStr;
	}

	spIThreeWayGenericStrategy APICALL ThreeWayGenericStrategyImpl::Clone() const {
		auto newObject = createAndCloneContents();
		if ( newObject && newObject->mPaths->size() == 0 ) {
			for ( auto it = mPaths->begin(), itEnd = mPaths->end(); it != itEnd; ++it ) {
				newObject->mPaths->push_back( ( *it )->substr() );
			}
		}
		return MakeUncheckedSharedPointer( newObject, __FILE__, __LINE__, true );
	}

}
