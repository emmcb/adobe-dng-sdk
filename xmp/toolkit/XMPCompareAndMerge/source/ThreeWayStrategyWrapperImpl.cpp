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

namespace AdobeXMPCompareAndMerge_Int {
	class ThreeWayStrategyWrapperImpl;
}

#define FRIEND_CLASS_DECLARATION() friend class AdobeXMPCompareAndMerge_Int::ThreeWayStrategyWrapperImpl;



#define IMPLEMENTATION_HEADERS_CAN_BE_INCLUDED 1
	#include "XMPCompareAndMerge/ImplHeaders/ThreeWayStrategyWrapperImpl.h"
#undef IMPLEMENTATION_HEADERS_CAN_BE_INCLUDED

#include "XMPCommon/Interfaces/IError_I.h"
#include "XMPCommon/Utilities/TSmartPointers_I.h"
#include "XMPCommon/Interfaces/IUTF8String.h"

namespace AdobeXMPCompareAndMerge_Int {

	ThreeWayStrategyWrapperImpl::ThreeWayStrategyWrapperImpl( pIThreeWayStrategy_base strategy )
		: mStrategy( strategy ) {}

	bool APICALL ThreeWayStrategyWrapperImpl::InterestedInResolvingConflict( const spcIUTF8String & nodePath, INode::eNodeType nodeType,
		IThreeWayUnResolvedConflict::eConflictReason reason, bool alreadyResolved )
	{
		pcIError_base pError( NULL );
		uint32 unknownExceptionCaught( 0 );
		auto result = mStrategy->interestedInResolvingConflict( nodePath->GetActualIUTF8String(), static_cast< uint32 >( nodeType ),
			static_cast< uint32 >( reason ), alreadyResolved ? 1 : 0, pError, unknownExceptionCaught );
		if ( pError ) {
			auto spError = IError::MakeShared( pError );
			pError->Release();
			throw spError;
		}
		if ( unknownExceptionCaught )
			NOTIFY_ERROR( IError::kEDGeneral, kGECUnknownExceptionCaught, "Unknown Exception caught in the client code", IError::kESOperationFatal, false, false );
		return result != 0 ? true : false;
	}

	IThreeWayUnResolvedConflict::eConflictReason APICALL ThreeWayStrategyWrapperImpl::ResolveConflict( IThreeWayUnResolvedConflict::eConflictReason conflictReason,
		const spcINode & baseVersionNode, const spcINode & latestVersionNode, const spcINode & userVersionNode, spcINode & resolvedNode,
		IThreeWayResolvedConflict::eConflictResolutionReason & conflictResolutionReason, IThreeWayResolvedConflict::eResolvedSource & resolvedSource )
	{
		pcIError_base pError( NULL );
		uint32 unknownExceptionCaught( 0 );
		pcINode _resolvedNode( NULL );
		uint32 _conflictResolutionReason;
		uint32 _resolvedSource;
		auto result = mStrategy->resolveConflict( static_cast< uint32 >( conflictReason ), baseVersionNode?baseVersionNode->GetActualINode():NULL, latestVersionNode?latestVersionNode->GetActualINode():NULL,
			userVersionNode?userVersionNode->GetActualINode():NULL, _resolvedNode, _conflictResolutionReason, _resolvedSource, pError, unknownExceptionCaught );
		if ( pError ) {
			auto spError = IError::MakeShared( pError );
			pError->Release();
			throw spError;
		}
		if ( unknownExceptionCaught )
			NOTIFY_ERROR( IError::kEDGeneral, kGECUnknownExceptionCaught, "Unknown Exception caught in the client code", IError::kESOperationFatal, false, false );
		if (_resolvedNode){
			resolvedNode = INode::MakeShared(_resolvedNode);
			resolvedNode->GetActualINode()->Release();
		}
		conflictResolutionReason = static_cast< IThreeWayResolvedConflict::eConflictResolutionReason > ( _conflictResolutionReason );
		resolvedSource = static_cast< IThreeWayResolvedConflict::eResolvedSource >( _resolvedSource );
		return static_cast< IThreeWayUnResolvedConflict::eConflictReason >( result );
	}

	ThreeWayStrategyWrapperImpl::~ThreeWayStrategyWrapperImpl() __NOTHROW__ {}


	spIThreeWayStrategy IThreeWayStrategy_I::CreateThreeWayStrategy( pIThreeWayStrategy_base strategy ) {
		return MakeUncheckedSharedPointer( new ThreeWayStrategyWrapperImpl( strategy ), __FILE__, __LINE__, true );
	}

}
