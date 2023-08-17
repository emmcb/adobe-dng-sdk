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
#if WIN_ENV
#pragma warning( push )
#pragma warning(disable:4503)
#endif
#define IMPLEMENTATION_HEADERS_CAN_BE_INCLUDED 1
	#include "XMPCompareAndMerge/ImplHeaders/ThreeWayMergeResultImpl.h"
#undef IMPLEMENTATION_HEADERS_CAN_BE_INCLUDED

#include "XMPCompareAndMerge/XMPCompareAndMergeErrorCodes.h"
#include "XMPCompareAndMerge/Interfaces/IThreeWayResolvedConflict.h"

#include "XMPCommon/Interfaces/IError_I.h"
#include "XMPCommon/Utilities/TSmartPointers_I.h"

#include "XMPCore/Interfaces/IArrayNode.h"
#include "XMPCore/Interfaces/INodeIterator.h"
#include "XMPCore/Interfaces/IMetadata.h"
#include "XMPCore/Interfaces/IPath.h"
#include "XMPCore/Interfaces/IPathSegment.h"

namespace AdobeXMPCompareAndMerge_Int {

	static void ReplaceNodeValue( const spIMetadata & destinatationMetadata, const spcINode & nodeToBeCopied, const spcIPath & pathOfNode ) {
		//clear the node
		destinatationMetadata->RemoveNodeAtPath( pathOfNode );

		spINode parentNode = destinatationMetadata;
		sizet pathSegmentCount = pathOfNode->Size();

		if ( pathSegmentCount > 1 ) {
			spIPath path = pathOfNode->Clone( 1, pathSegmentCount - 1 );
			parentNode = destinatationMetadata->GetNodeAtPath( path );
		}
		if (nodeToBeCopied->IsQualifierNode())
		{
			parentNode->InsertQualifier(nodeToBeCopied->Clone());
		}
		else {

			if (parentNode->GetNodeType() == INode::kNTArray)
				parentNode->ConvertToArrayNode()->AppendNode(nodeToBeCopied->Clone());
			else
				parentNode->ConvertToStructureNode()->AppendNode(nodeToBeCopied->Clone());
		}
	}

	ThreeWayMergeResultImpl::ThreeWayMergeResultImpl( const spcIMetadata & latestVersionMetadata, const spcIMetadata & userVersionMetadata,
		const spIThreeWayResolvedConflicts & resolvedConflicts, const spIThreeWayUnResolvedConflicts & unResolvedConflicts)
		: mLatestVersionMetadata(latestVersionMetadata)
		, mUserVersionMetadata(userVersionMetadata)
	{
		sizet resolvedConflictsCount = resolvedConflicts->size();
		if ( resolvedConflictsCount > 0 ) {
			mResolvedConflicts = MakeUncheckedSharedPointerWithDefaultDelete(
				new IThreeWayResolvedConflicts(resolvedConflicts->begin(), resolvedConflicts->end(), gTAllocatorThreeWayResolvedConflict ), __FILE__, __LINE__, true );
		} else {
			mResolvedConflicts = MakeUncheckedSharedPointerWithDefaultDelete( new IThreeWayResolvedConflicts( gTAllocatorThreeWayResolvedConflict ), __FILE__, __LINE__, true );
		}

		sizet unresolvedConflictsCount = unResolvedConflicts->size();
		if ( unresolvedConflictsCount > 0 ) {
			mUnResolvedConflicts = MakeUncheckedSharedPointerWithDefaultDelete(
				new IThreeWayUnResolvedConflicts(unResolvedConflicts->begin(), unResolvedConflicts->end(), gTAllocatorThreeWayUnResolvedConflict ), __FILE__, __LINE__, true );
		} else {
			mUnResolvedConflicts = MakeUncheckedSharedPointerWithDefaultDelete( new IThreeWayUnResolvedConflicts( gTAllocatorThreeWayUnResolvedConflict ), __FILE__, __LINE__, true );
		}
	}

	sizet APICALL ThreeWayMergeResultImpl::CountOfResolvedConflicts() const __NOTHROW__ {
		return mResolvedConflicts->size();
	}

	sizet APICALL ThreeWayMergeResultImpl::CountOfUnResolvedConflicts() const __NOTHROW__ {
		return mUnResolvedConflicts->size();
	}

	spIMetadata APICALL ThreeWayMergeResultImpl::GetMergedMetadata() const {
		eMergeStatus mergedStatus = GetMergedStatus();

		if ( mergedStatus == kMSConflictsPending ) {
			NOTIFY_ERROR( IError::kEDThreeWayMerge, kTWMECConflictsPending, "Can't Generate Merged Metadata object as there are pending conflicts",
				IError::kESOperationFatal, true, static_cast< uint64 >( mUnResolvedConflicts->size() ) );
		} else if ( mergedStatus == kMSNoConflictFound ) {
			return mLatestVersionMetadata->Clone()->ConvertToMetadata();
		}

		// find which one should be used as base 
		sizet nCountLatest = 0, nCountUser = 0;
		bool useLatest = true;

		for ( sizet i = 0, nLoopCount = mResolvedConflicts->size(); i < nLoopCount; ++i ) {
			IThreeWayResolvedConflict::eResolvedSource resolvedNodeSource = mResolvedConflicts->at( i )->GetResolvedSource();
			if ( resolvedNodeSource == IThreeWayResolvedConflict::kRSLatestVersion ) {
				nCountLatest++;
			} else if ( resolvedNodeSource == IThreeWayResolvedConflict::kRSUserVersion ) {
				nCountUser++;
			}
		}

		if ( nCountUser > nCountLatest ) {
			useLatest = false;
		}

		// make a clone of the appropriate metadata Object
		spINode node = useLatest ? mLatestVersionMetadata->Clone() : mUserVersionMetadata->Clone();
		spIMetadata mergedMetadata = node->ConvertToMetadata();

		// perform operations on the merged Metadata
		for ( sizet i = 0, nLoopCount = mResolvedConflicts->size(); i < nLoopCount; i++ ) {
			const spcIThreeWayResolvedConflict & currentConflict = mResolvedConflicts->at( i );
			const spcINode & resolvedNode = currentConflict->GetResolvedNode();

			switch ( currentConflict->GetResolvedSource() ) {

			case IThreeWayResolvedConflict::kRSLatestVersion:
				if ( !useLatest ) {
					if(resolvedNode.get() == NULL)
						mergedMetadata->RemoveNodeAtPath(currentConflict->GetUserVersionNode()->GetPath());
					else
						ReplaceNodeValue( mergedMetadata, resolvedNode, resolvedNode->GetPath() );
				}
				break;

			case IThreeWayResolvedConflict::kRSUserVersion:
				if ( useLatest ) {
					if(resolvedNode.get() == NULL)
						mergedMetadata->RemoveNodeAtPath(currentConflict->GetLatestVersionNode()->GetPath());
					else
						ReplaceNodeValue( mergedMetadata, resolvedNode, resolvedNode->GetPath() );
				}
				break;

			case IThreeWayResolvedConflict::kRSBaseVersion:
				ReplaceNodeValue( mergedMetadata, resolvedNode, currentConflict->GetBaseVersionNode()->GetPath() );
				break;

			case IThreeWayResolvedConflict::kRSFieldsMerged:
			case IThreeWayResolvedConflict::kRSCustomMerged:
			case IThreeWayResolvedConflict::kRSStrategyDefined:
				{
					spcIPath pathOfNode;
					spcINode latestVersionNode = currentConflict->GetLatestVersionNode();
					if ( latestVersionNode ) {
						pathOfNode = latestVersionNode->GetPath();
					} else {
						spcINode userVersionNode = currentConflict->GetUserVersionNode();
						if ( userVersionNode ) {
							pathOfNode = userVersionNode->GetPath();
						} else {
							pathOfNode = currentConflict->GetBaseVersionNode()->GetPath();
						}
					}
					if(resolvedNode.get() == NULL)
						mergedMetadata->RemoveNodeAtPath(pathOfNode);
					else
						ReplaceNodeValue( mergedMetadata, resolvedNode, pathOfNode );
				}
				break;

			case IThreeWayResolvedConflict::kRSNone:
				if ( useLatest ) {
					IThreeWayResolvedConflict::eConflictResolutionReason reason = currentConflict->GetConflictResolutionReason();
					if ( reason == IThreeWayResolvedConflict::kCRRDeletionOnlyInUserVersion ) {
						mergedMetadata->RemoveNodeAtPath( currentConflict->GetBaseVersionNode()->GetPath() );
					}
					else {
						if (reason == IThreeWayResolvedConflict::kCRRDeletionOfQualifierOnlyInUserVersion) {
							mergedMetadata->GetNodeAtPath(currentConflict->GetBaseVersionNode()->GetPath())->Clear(false, true);
						}
					}
				} else {
					IThreeWayResolvedConflict::eConflictResolutionReason reason = currentConflict->GetConflictResolutionReason();
					if ( reason == IThreeWayResolvedConflict_base::kCRRDeletionOnlyInLatestVersion ) {
						mergedMetadata->RemoveNodeAtPath( currentConflict->GetBaseVersionNode()->GetPath() );
					}
					else {
						if (reason == IThreeWayResolvedConflict_base::kCRRDeletionOfQualifierOnlyInLatestVersion) {
							mergedMetadata->GetNodeAtPath(currentConflict->GetBaseVersionNode()->GetPath())->Clear(false, true);
						}
					}
				}
				break;

			default:
				NOTIFY_ERROR( IError::kEDGeneral, kGECLogicalError, "Unhandled Situation Occured", IError::kESOperationFatal, false, false );
				break;
			}
		}
		return mergedMetadata;
	}

	IThreeWayMergeResult::eMergeStatus APICALL ThreeWayMergeResultImpl::GetMergedStatus() const {
		if ( mUnResolvedConflicts->size() == 0 && mResolvedConflicts->size() == 0 ) {
			return kMSNoConflictFound;
		} else if ( mUnResolvedConflicts->size() == 0 ) {
			return kMSAllConflictsResolved;
		}
		return kMSConflictsPending;
	}

	spcIThreeWayResolvedConflict APICALL ThreeWayMergeResultImpl::GetResolvedConflict( sizet index ) const {
		if ( index >= mResolvedConflicts->size() ) {
			NOTIFY_ERROR( IError::kEDGeneral, kGECIndexOutOfBounds, "Provided index in not within the valid bounds", IError::kESOperationFatal,
				true, static_cast< uint64 >( index ), true, static_cast< uint64 >( mResolvedConflicts->size() ) );
			return spcIThreeWayResolvedConflict();
		} else {
			return mResolvedConflicts->at( index );
		}
	}

	spIThreeWayResolvedConflicts APICALL ThreeWayMergeResultImpl::GetResolvedConflicts() {
		return mResolvedConflicts;
	}

	spcIThreeWayResolvedConflicts_const APICALL ThreeWayMergeResultImpl::GetResolvedConflicts() const {
		auto sp = MakeUncheckedSharedPointerWithDefaultDelete(
			new cIThreeWayResolvedConflicts( mResolvedConflicts->begin(), mResolvedConflicts->end(), gTAllocatorThreeWayResolvedConflict ), __FILE__, __LINE__, true );
		return sp;
	}

	spcIThreeWayUnResolvedConflict APICALL ThreeWayMergeResultImpl::GetUnResolvedConflict( sizet index ) const {
		if ( index >= mUnResolvedConflicts->size() ) {
			NOTIFY_ERROR( IError::kEDGeneral, kGECIndexOutOfBounds, "Provided index in not within the valid bounds", IError::kESOperationFatal,
				true, static_cast< uint64 >( index ), true, static_cast< uint64 >( mUnResolvedConflicts->size() ) );
			return spcIThreeWayUnResolvedConflict();
		} else {
			return mUnResolvedConflicts->at( index );
		}
	}

	spIThreeWayUnResolvedConflicts APICALL ThreeWayMergeResultImpl::GetUnResolvedConflicts() {
		return mUnResolvedConflicts;
	}

	spcIThreeWayUnResolvedConflicts_const APICALL ThreeWayMergeResultImpl::GetUnResolvedConflicts() const {
		auto sp = MakeUncheckedSharedPointerWithDefaultDelete(
			new cIThreeWayUnResolvedConflicts( mUnResolvedConflicts->begin(), mUnResolvedConflicts->end(), gTAllocatorThreeWayUnResolvedConflict ), __FILE__, __LINE__, true );
		return sp;
	}

	spIThreeWayMergeResult IThreeWayMergeResult_I::CreateThreeWayMergeResult( const spcIMetadata & latestVersionMetadata, const spcIMetadata & userVersionMetadata,
		const spIThreeWayResolvedConflicts & resolvedConflicts, const spIThreeWayUnResolvedConflicts & unResolvedConflictList )
	{
		return MakeUncheckedSharedPointer(
			new ThreeWayMergeResultImpl( latestVersionMetadata, userVersionMetadata, resolvedConflicts, unResolvedConflictList ), __FILE__, __LINE__, true );
	}

}

namespace AdobeXMPCompareAndMerge {
	using namespace AdobeXMPCompareAndMerge_Int;

	spIThreeWayMergeResult IThreeWayMergeResult_v1::MakeShared( pIThreeWayMergeResult_base ptr ) {
		if ( !ptr ) return spIThreeWayMergeResult();
		pIThreeWayMergeResult p = IThreeWayMergeResult::GetInterfaceVersion() > 1 ? ptr->GetInterfacePointer< IThreeWayMergeResult >() : ptr;
		return MakeUncheckedSharedPointer( p, __FILE__, __LINE__, false );
	}
}
