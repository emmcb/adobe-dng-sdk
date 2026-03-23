// =================================================================================================
// ADOBE SYSTEMS INCORPORATED
// Copyright 2014 Adobe Systems Incorporated
// All Rights Reserved
//
// NOTICE:  Adobe permits you to use, modify, and distribute this file in accordance with the terms
// of the Adobe license agreement accompanying it.
// =================================================================================================

#if AdobePrivate
// =================================================================================================
// Change history
// ==============
//
// Writers:
//  ADC	Amandeep Chawla
//	SKP Sunil Kishor Pathak
//
// mm-dd-yy who Description of changes, most recent first.
//
// 12-11-14 SKP 1.0-a044 Fixed return value in IsTrackable, fixed issues in RemoveComponent,
//						 Added GetIngredientNode() to return the reference of node within the
//						 the relationship to the ingredient entry.
// 12-04-14 SKP 1.0-a036 Fixed issue in SetMappingFunctionName().
// 12-02-14 SKP 1.0-a027 Fixed copy-paste error, replaced filePath with maskMarkers.
// 12-01-14 SKP 1.0-a025 Added functionality to add stRef on relationship.
// 11-27-14 SKP 1.0-a017 Implemented Asset composition related functionality for basic part addition.
// 11-25-14 ADC 1.0-a009 Fixing compile and linking issues.
// 11-24-14 ADC 1.0-a008 Support for ICompositionRelationship interface.
//
// =================================================================================================
#endif  // AdobePrivate


#define IMPLEMENTATION_HEADERS_CAN_BE_INCLUDED 1
	#include "XMPAssetManagement/ImplHeaders/CompositionRelationshipImpl.h"
#undef IMPLEMENTATION_HEADERS_CAN_BE_INCLUDED

#include "XMPCommon/Interfaces/IErrorNotifier_I.h"
#include "XMPCommon/Utilities/AutoSharedLock.h"
#include "XMPCommon/Interfaces/IUTF8String_I.h"
#include "XMPCommon/Utilities/TSmartPointers_I.h"
#include "XMPCore/Interfaces/IMetadata.h"
#include "XMPCore/Interfaces/ISimpleNode.h"
#include "XMPCore/Interfaces/IArrayNode.h"
#include "XMPCommon/Interfaces/IError_I.h"
#include "XMPCommon/XMPCommonErrorCodes.h"

namespace AdobeXMPAM_Int {

	//definition of static factory methods of internal interface
	spICompositionRelationship_I ICompositionRelationship_I::CreateCompositionRelationship(
		const spIMetadata & composedMetadata,
		const spcIAssetPart & composedPart, const spcIStructureNode & componentMetadata,
		const spcIAssetPart & componentPart)
	{
		return MakeUncheckedSharedPointer( new CompositionRelationshipImpl(composedMetadata,
			composedPart, componentMetadata, componentPart), __FILE__, __LINE__);
	}

	//definition of class methods

	spcIAssetPart APICALL CompositionRelationshipImpl::GetComponentPart() const {
		AutoSharedLock lock( mSharedMutex );
		return mComponentPart;
	}

	spcIAssetPart APICALL CompositionRelationshipImpl::GetComposedPart() const {
		AutoSharedLock lock( mSharedMutex );
		return mComposedPart;
	}

	spcIUTF8Strings CompositionRelationshipImpl::GetComponentAlternatePaths_I() const {
		AutoSharedLock lock( mSharedMutex );
		return mComponentAlternatePaths;
	}

	spcIStructureNode APICALL CompositionRelationshipImpl::GetComponentMetadata() const {
		AutoSharedLock lock( mSharedMutex );
		return mComponentMetadata;
	}

	spcIStructureNode APICALL CompositionRelationshipImpl::GetComposedMetadata() const {
		AutoSharedLock lock( mSharedMutex );
		return mComposedMetadata;
	}

	spcIUTF8String APICALL CompositionRelationshipImpl::GetMappingFunctionName() const {
		AutoSharedLock lock( mSharedMutex );
		return mMappingFunctionName;
	}

	void APICALL CompositionRelationshipImpl::SetMappingFunctionName(const char * name, sizet nameLength) {
		if ( name ) {
			AutoSharedLock( mSharedMutex, true );
			if ( !mMappingFunctionName )
				mMappingFunctionName = IUTF8String_I::CreateUTF8String( name, nameLength );
			else
				mMappingFunctionName->assign( name, nameLength );
		} else {
			AutoSharedLock lock( mSharedMutex, true );
			mMappingFunctionName.reset();
		}

		//update the ingredient entry node
		spISimpleNode mappingFunctionNode = (mIngredientEntry->GetNode(kXMP_NS_XMP_ResourceRef, AdobeXMPCommon::npos, "partMapping", AdobeXMPCommon::npos))->ConvertToSimpleNode();
		if (mappingFunctionNode) {
			mappingFunctionNode->SetValue(mMappingFunctionName->c_str(), mMappingFunctionName->size());
		}
		else {
			mappingFunctionNode = ISimpleNode::CreateSimpleNode(kXMP_NS_XMP_ResourceRef, AdobeXMPCommon::npos, "partMapping", AdobeXMPCommon::npos, mMappingFunctionName->c_str(), mMappingFunctionName->size());
			this->mIngredientEntry->InsertNode(mappingFunctionNode);
		}
	}

	ICompositionRelationship_v1::eCompositionMaskMarkers APICALL CompositionRelationshipImpl::GetMaskMarkersFlag() const {
		AutoSharedLock lock( mSharedMutex );
		return mMaskMarkersFlag;
	}

	void APICALL CompositionRelationshipImpl::SetMaskMarkersFlag(eCompositionMaskMarkers flag) {
		if ( flag < ICompositionRelationship_v1::kCompositionMaskMarkersAll ||
			 flag > ICompositionRelationship_v1::kCompositionMaskMarkersNotSpecified )
		{
			NOTIFY_ERROR( IError_v1::kEDGeneral, kGECParametersNotAsExpected,
				"Unsupported value for eCompositionMaskMarkers specified", IError_v1::kESOperationFatal,
				true, static_cast< uint32 >( flag ) );
		}
		AutoSharedLock lock( mSharedMutex, true );
		mMaskMarkersFlag = flag;

		//update the ingredient entry node
		spIUTF8String maskMarker;
		if (flag == ICompositionRelationship_v1::kCompositionMaskMarkersAll)
			maskMarker = IUTF8String_I::CreateUTF8String("All", AdobeXMPCommon::npos);
		else {
			if (flag == ICompositionRelationship_v1::kCompositionMaskMarkersNone)
				maskMarker = IUTF8String_I::CreateUTF8String("None", AdobeXMPCommon::npos);
		}

		spISimpleNode mappingFunctionNode = (mIngredientEntry->GetNode(kXMP_NS_XMP_ResourceRef, AdobeXMPCommon::npos, "maskMarkers", AdobeXMPCommon::npos))->ConvertToSimpleNode();
		if (mappingFunctionNode) {
			mappingFunctionNode->SetValue(maskMarker->c_str(), maskMarker->size());
		}
		else {
			mappingFunctionNode = ISimpleNode::CreateSimpleNode(kXMP_NS_XMP_ResourceRef, AdobeXMPCommon::npos, "maskMarkers", AdobeXMPCommon::npos, maskMarker->c_str(), maskMarker->size());
			this->mIngredientEntry->InsertNode(mappingFunctionNode);
		}
	}

	spcIUTF8String APICALL CompositionRelationshipImpl::GetComponentDocumentID() const {
		AutoSharedLock lock( mSharedMutex );
		/*auto utf8String = IXMPNode_v1::AdaptConstNodeTo< const IXMPSimpleNode >(
			mComponentMetadata->GetNode( kXMP_NS_XMP_MM, "DocumentID" ), IXMPNode_v1::kXMPNodeTypeSimple, true )->GetValue();*/
		spcINode compMetadata;
		compMetadata = mComponentMetadata->GetNode(kXMP_NS_XMP_MM, AdobeXMPCommon::npos, "DocumentID", AdobeXMPCommon::npos);
		if (compMetadata->GetNodeType() == INode::kNTSimple){
			auto utf8String = compMetadata->ConvertToSimpleNode()->GetValue();
			return dynamic_pointer_cast<const IUTF8String>(utf8String);
		}
		else
		{
			// reafactor ToDo throw error 
			/*static const char * nodeTypeNames[] = { "kXMPNodeTypeSimple", "kXMPNodeTypeArray", "kXMPNodeTypeStructure" };
			static const char * params[2];
			params[0] = nodeTypeNames[static_cast< uint32 >(INode::kNTSimple)];
			params[1] = nodeTypeNames[static_cast< uint32 >(compMetadata->GetNodeType())];
			ThrowError(IError_base::kDomainXMPDataModel, kGE,
				false, "Different type of node present than the one expected by the client", __FILE__, __LINE__,
				2, params);*/
			return shared_ptr<const IUTF8String>();
		}
	}

	spcIUTF8String APICALL CompositionRelationshipImpl::GetComponentInstanceID() const {
		AutoSharedLock lock( mSharedMutex );
		spcINode compMetadata;
		compMetadata =  mComponentMetadata->GetNode(kXMP_NS_XMP_MM, AdobeXMPCommon::npos,"InstanceID", AdobeXMPCommon::npos);
		/*auto utf8String = IXMPNode_v1::AdaptConstNodeTo< const IXMPSimpleNode >(
			mComponentMetadata->GetNode( kXMP_NS_XMP_MM, "InstanceID" ), IXMPNode_v1::kXMPNodeTypeSimple, true )->GetValue();
		return dynamic_pointer_cast< const IUTF8String >( utf8String );*/
		if (compMetadata->GetNodeType() == INode::kNTSimple){
			auto utf8String = compMetadata->ConvertToSimpleNode()->GetValue();
			return dynamic_pointer_cast<const IUTF8String>(utf8String);
		}
		else
		{
			// reafactor ToDo throw error 
			/*static const char * nodeTypeNames[] = { "kXMPNodeTypeSimple", "kXMPNodeTypeArray", "kXMPNodeTypeStructure" };
			static const char * params[2];
			params[0] = nodeTypeNames[static_cast< uint32 >(INode::kNTSimple)];
			params[1] = nodeTypeNames[static_cast< uint32 >(compMetadata->GetNodeType())];
			ThrowError(IError_base::kDomainXMPDataModel, kGE,
			false, "Different type of node present than the one expected by the client", __FILE__, __LINE__,
			2, params);*/
			return shared_ptr<const IUTF8String>();
		}
	}

	spcIUTF8String APICALL CompositionRelationshipImpl::GetComposedDocumentID() const {
		AutoSharedLock lock( mSharedMutex );
		spcINode compMetadata;
		compMetadata = mComposedMetadata->GetNode(kXMP_NS_XMP_MM, AdobeXMPCommon::npos, "DocumentID", AdobeXMPCommon::npos);
		if (compMetadata->GetNodeType() == INode::kNTSimple){
			auto utf8String = compMetadata->ConvertToSimpleNode()->GetValue();
			return utf8String;
		}
		else
		{
			// refactor ToDo throw error 
			/*static const char * nodeTypeNames[] = { "kXMPNodeTypeSimple", "kXMPNodeTypeArray", "kXMPNodeTypeStructure" };
			static const char * params[2];
			params[0] = nodeTypeNames[static_cast< uint32 >(INode::kNTSimple)];
			params[1] = nodeTypeNames[static_cast< uint32 >(compMetadata->GetNodeType())];
			ThrowError(IError_base::kDomainXMPDataModel, kGE,
			false, "Different type of node present than the one expected by the client", __FILE__, __LINE__,
			2, params);*/
			return shared_ptr<const IUTF8String>();
		}
		/*auto utf8String = IXMPNode_v1::AdaptConstNodeTo< const IXMPSimpleNode >(
			mComposedMetadata->GetNode( kXMP_NS_XMP_MM, "DocumentID" ), IXMPNode_v1::kXMPNodeTypeSimple, true )->GetValue();
        return utf8String; //dynamic_pointer_cast< const IUTF8String >( utf8String );*/
	}

	spcIUTF8String APICALL CompositionRelationshipImpl::GetComposedInstanceID() const {
		AutoSharedLock lock( mSharedMutex );
		spcINode compMetadata;
		compMetadata = mComposedMetadata->GetNode(kXMP_NS_XMP_MM, AdobeXMPCommon::npos, "InstanceID", AdobeXMPCommon::npos);
		if (compMetadata->GetNodeType() == INode::kNTSimple){
			auto utf8String = compMetadata->ConvertToSimpleNode()->GetValue();
			return dynamic_pointer_cast<const IUTF8String>(utf8String);
		}
		else
		{
			// reafactor ToDo throw error 
			/*static const char * nodeTypeNames[] = { "kXMPNodeTypeSimple", "kXMPNodeTypeArray", "kXMPNodeTypeStructure" };
			static const char * params[2];
			params[0] = nodeTypeNames[static_cast< uint32 >(INode::kNTSimple)];
			params[1] = nodeTypeNames[static_cast< uint32 >(compMetadata->GetNodeType())];
			ThrowError(IError_base::kDomainXMPDataModel, kGE,
			false, "Different type of node present than the one expected by the client", __FILE__, __LINE__,
			2, params);*/
			return shared_ptr<const IUTF8String>();
		}
		/*auto utf8String = IXMPNode_v1::AdaptConstNodeTo< const IXMPSimpleNode >(
			mComposedMetadata->GetNode( kXMP_NS_XMP_MM, "InstanceID" ), IXMPNode_v1::kXMPNodeTypeSimple, true )->GetValue();
		return dynamic_pointer_cast< const IUTF8String >( utf8String );*/
	}

	void APICALL CompositionRelationshipImpl::SetComponentFilePath(const char * filePath, sizet filePathLength) {
		if ( filePath ) {
			AutoSharedLock( mSharedMutex, true );
			if ( !mComponentFilePath )
				mComponentFilePath = IUTF8String_I::CreateUTF8String( filePath, filePathLength );
			else
				mComponentFilePath->assign( filePath, filePathLength );
		} else {
			AutoSharedLock lock( mSharedMutex, true );
			mComponentFilePath.reset();
		}

		spISimpleNode filePathNode = (mIngredientEntry->GetNode(kXMP_NS_XMP_ResourceRef, AdobeXMPCommon::npos, "filePath", AdobeXMPCommon::npos))->ConvertToSimpleNode();
		if (filePathNode) {
			filePathNode->SetValue(mComponentFilePath->c_str(), mComponentFilePath->size());
		}
		else {
			filePathNode = ISimpleNode::CreateSimpleNode(kXMP_NS_XMP_ResourceRef, AdobeXMPCommon::npos, "filePath", AdobeXMPCommon::npos, mComponentFilePath->c_str(), mComponentFilePath->size());
			this->mIngredientEntry->InsertNode(filePathNode);
		}
	}

	spcIUTF8String APICALL CompositionRelationshipImpl::GetComponentFilePath() const {
		AutoSharedLock lock( mSharedMutex );
		return mComponentFilePath;
	}

	void APICALL CompositionRelationshipImpl::AppendComponentAlternatePath(const char * alternatePath, sizet altPathLength) {
		if ( alternatePath == NULL || alternatePath[0] == '\0' ) {
			NOTIFY_ERROR( IError_v1::kEDGeneral, kGECParametersNotAsExpected,
				"Invalid value for alternate path", IError_v1::kESOperationFatal,
				true,  ( void * ) alternatePath, alternatePath, alternatePath );
		}

		AutoSharedLock lock( mSharedMutex, true );
		if ( !mComponentAlternatePaths )
			mComponentAlternatePaths = shared_ptr<cIUTF8Strings>( new cIUTF8Strings() );
		mComponentAlternatePaths->push_back( IUTF8String_I::CreateUTF8String( alternatePath, altPathLength ) );
	}

	void APICALL CompositionRelationshipImpl::ClearComponentAlternatePaths() {
		AutoSharedLock lock( mSharedMutex, true );
		if ( mComponentAlternatePaths ) mComponentAlternatePaths->clear();
	}

	sizet CompositionRelationshipImpl::getComponentAlternatePathsSize() __NOTHROW__ {
		AutoSharedLock lock( mSharedMutex );
		if ( mComponentAlternatePaths )
			return mComponentAlternatePaths->size();
		else
			return 0;
	}

	void CompositionRelationshipImpl::SetIngredientNode(spIStructureNode node)
	{
		mIngredientEntry = node;
	}

	spIStructureNode CompositionRelationshipImpl::GetIngredientNode()
	{
		return mIngredientEntry;
	}

	pcIUTF8String_base CompositionRelationshipImpl::getComponentAlternatePathAtIndex( sizet index, pcIError_base & error ) __NOTHROW__ {
		AutoSharedLock lock( mSharedMutex );
		if ( mComponentAlternatePaths && index < mComponentAlternatePaths->size() ) {
			auto ptr = mComponentAlternatePaths->at(index).get();
			ptr->GetISharedObject_I()->AcquireInternal();
			return ptr;
		} else {
			try {
				NOTIFY_ERROR( IError_v1::kEDGeneral, kGECIndexOutOfBounds,
					"Requested element is not present in the Alternate Path Size List", IError_v1::kESOperationFatal,
					true, getComponentAlternatePathsSize(), true, index );
			} catch ( spcIError err ) {
				/*//err->AcquireSharedOwnershipForRawPointer();
				error = err; //.get();*/
				error = err->GetActualIError();
				error->GetISharedObject_I()->AcquireInternal();
			}
		}
		return NULL;
	}

	CompositionRelationshipImpl::CompositionRelationshipImpl( const spIMetadata & composedMetadata,
		const spcIAssetPart & composedPart, const spcIStructureNode & componentMetadata,
		const spcIAssetPart & componentPart )
	try
		: mComposedMetadata( composedMetadata )
		, mComposedPart( composedPart )
		, mComponentMetadata( componentMetadata )
		, mComponentPart( componentPart )
		, mMaskMarkersFlag( ICompositionRelationship_v1::kCompositionMaskMarkersNotSpecified )
	{
		
	} catch ( ... ) {
		NOTIFY_ERROR( IError_v1::kEDGeneral, kGECParametersNotAsExpected,
			"Parameters not as expected by CompositionRelationshipImpl::CompositionRelationshipImpl",
			IError_v1::kESOperationFatal, true, ( void * ) composedMetadata.get(),
			true, ( void * ) composedPart.get(), true, ( void * ) componentMetadata.get(),
			true, ( void * ) componentPart.get() );
	}

}

namespace AdobeXMPAM {
	using namespace AdobeXMPAM_Int;
	spICompositionRelationship ICompositionRelationship_v1::MakeShared(pICompositionRelationship_base ptr) {
		if (!ptr) return spICompositionRelationship();
		pICompositionRelationship p = ICompositionRelationship::GetInterfaceVersion() > 1 ? ptr->GetInterfacePointer< ICompositionRelationship >() : ptr;
		return MakeUncheckedSharedPointer(p, __FILE__, __LINE__, false);
	}
}

