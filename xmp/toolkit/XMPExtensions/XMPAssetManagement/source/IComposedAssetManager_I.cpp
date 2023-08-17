// =================================================================================================
// ADOBE SYSTEMS INCORPORATED
// Copyright 2014 Adobe Systems Incorporated
// All Rights Reserved
//
// NOTICE:  Adobe permits you to use, modify, and distribute this file in accordance with the terms
// of the Adobe license agreement accompanying it.
// =================================================================================================

#include "XMPAssetManagement/Interfaces/IComposedAssetManager_I.h"
#include "XMPCommon/Utilities/TWrapperFunctions_I.h"
#include "XMPAssetManagement/Interfaces/ICompositionRelationship_I.h"
#include "XMPAssetManagement/Interfaces/IAssetPart_I.h"
#include "XMPCore/Interfaces/IStructureNode.h"
#include "XMPCommon/Utilities/TWrapperFunctions_I.h"
#include "XMPCommon/Utilities/TSmartPointers_I.h"
#include "XMPCommon/XMPCommonErrorCodes.h"

namespace AdobeXMPAM_Int {

	pvoid APICALL IComposedAssetManager_I::GetInterfacePointer(uint64 interfaceID, uint32 interfaceVersion) {
		return GetInterfacePointerInternal(interfaceID, interfaceVersion, true);
	}

	pvoid APICALL IComposedAssetManager_I::getInterfacePointer(uint64 interfaceID, uint32 interfaceVersion, pcIError_base & error) __NOTHROW__{
		return CallUnSafeFunction< IComposedAssetManager_I, pvoid, pvoid, uint64, uint32 >(
		error, this, NULL, &IComposedAssetManager_I::GetInterfacePointer, __FILE__, __LINE__, interfaceID, interfaceVersion);
	}

		pvoid APICALL IComposedAssetManager_I::GetInterfacePointerInternal(uint64 interfaceID, uint32 interfaceVersion, bool isTopLevel) {
		pvoid returnValue(NULL);
		if (interfaceID == kIComposedAssetManagerID) {
			switch (interfaceVersion) {
			case 1:
				return static_cast< IComposedAssetManager_v1 * >(this);
				break;

			case kInternalInterfaceVersionNumber:
				return this;
				break;

			default:
				throw IError_I::CreateInterfaceVersionNotAvailableError(
					IError_v1::kESOperationFatal, interfaceID, interfaceVersion, __FILE__, __LINE__);
				break;
			}
		}
		if (isTopLevel)
			throw IError_I::CreateInterfaceNotAvailableError(
			IError_v1::kESOperationFatal, kIComposedAssetManagerID, interfaceID, __FILE__, __LINE__);
		return NULL;
	}



	pICompositionRelationship_base APICALL IComposedAssetManager_I::addComponent( pcIAssetPart_base composedPart,
		pcIStructureNode_base component, pcIAssetPart_base componentPart, pcIError_base & error ) __NOTHROW__
	{
		/*auto spComposedPart = MakeSharedPointer< const IAssetPart_I >( composedPart, error );
		if ( error ) return NULL;
		auto spComponentPart = NS_INT_XMPCOMMON::MakeSharedPointer< const IAssetPart_I >( componentPart, error );
		if ( error ) return NULL;
		auto spComponentMetadata = NS_INT_XMPCOMMON::MakeSharedPointer< const IXMPStructureNode >( component, error );
		if ( error ) return NULL;
		*/
		//if (!composedPart) return NULL;
		auto spComposedPart = IAssetPart::MakeShared(composedPart);
		//if (error) return NULL;
		//if (!componentPart) return NULL;
		auto spComponentPart = IAssetPart::MakeShared(componentPart);
		//if (error) return NULL;
		//if (!component) return NULL;
		auto spComponentMetadata = IStructureNode::MakeShared(component);
		//if (error) return NULL;
		return CallUnSafeFunctionReturningSharedPointer<
			IComposedAssetManager_v1, pICompositionRelationship_base, ICompositionRelationship,
			const spcIAssetPart &, const spcIStructureNode &, const spcIAssetPart &
		>(error, this, &IComposedAssetManager_v1::AddComponent, __FILE__, __LINE__, spComposedPart, spComponentMetadata, spComponentPart);
	}

	/*spICompositionRelationship IComposedAssetManager_I::AddComponent( const spcIAssetPart & composedPart,
	const spcIXMPStructureNode & component, const spcIAssetPart & componentPart )
	{
		auto spComposedPart = NS_INT_XMPCOMMON::MakeSharedPointer< const IAssetPart_I >( composedPart.get() );
		auto spComponentPart = NS_INT_XMPCOMMON::MakeSharedPointer< const IAssetPart_I >( componentPart.get() );
		auto spComponentMetadata = NS_INT_XMPCOMMON::MakeSharedPointer< const IXMPStructureNode >( component.get() );
		return AddComponent( spComposedPart, spComponentMetadata, spComponentPart );
	}*/

	sizet APICALL IComposedAssetManager_I::removeComponents( pcIStructureNode_base component, pcIAssetPart_base componentPart,
		pcIError_base & error ) __NOTHROW__
	{
		//refactor overloaded function was ambiguous. So this soln. //but not sure
		typedef sizet(APICALL IComposedAssetManager_v1::*func)(const spcIStructureNode &, const spcIAssetPart &);
		func f = &IComposedAssetManager_v1::RemoveComponents;
		//if (!componentPart) return NULL;
		auto spComponentPart = IAssetPart::MakeShared( componentPart );
		//if ( error ) return NULL;
		//if (!component) return NULL;
		auto spComponentMetadata = IStructureNode::MakeShared( component );
		//if ( error ) return NULL;
		return CallUnSafeFunction<
			IComposedAssetManager_v1, sizet, sizet, const spcIStructureNode &, const spcIAssetPart &
		>( error, this, 0, f/*&IComposedAssetManager_I::RemoveComponents*/, __FILE__, __LINE__, spComponentMetadata, spComponentPart );
	}

	sizet APICALL IComposedAssetManager_I::removeComponents( const char * documentID, sizet docIDLength, const char * instanceID, sizet insIDLength,
	pcIAssetPart_base componentPart, pcIError_base & error ) __NOTHROW__ 
	{
		typedef sizet(APICALL IComposedAssetManager_v1::*func)(const char *, sizet, const char *, sizet, const spcIAssetPart &);
		func f = &IComposedAssetManager_v1::RemoveComponents;
		//if (!componentPart) return NULL;
		auto spComponentPart = IAssetPart::MakeShared( componentPart );
		//if ( error ) return NULL;
		return CallUnSafeFunction<
			IComposedAssetManager_v1, sizet, sizet, const char *, sizet, const char *, sizet, const spcIAssetPart &
		>( error, this, 0, f/*&IComposedAssetManager_I::RemoveComponents*/, __FILE__, __LINE__, documentID, docIDLength, instanceID, insIDLength, spComponentPart );
	}

	sizet APICALL IComposedAssetManager_I::removeComponents( pcIAssetPart_base composedPart, pcIError_base & error ) __NOTHROW__ {
		typedef sizet(APICALL IComposedAssetManager_v1::*func)(const spcIAssetPart &);
		func f = &IComposedAssetManager_v1::RemoveComponents;
		//if (!composedPart) return NULL;
		auto spComposedPart = IAssetPart::MakeShared( composedPart );
		//if ( error ) return NULL;
		return CallUnSafeFunction<
			IComposedAssetManager_v1, sizet, sizet, const spcIAssetPart &
		>( error, this, 0, f/*&IComposedAssetManager_I::RemoveComponents*/, __FILE__, __LINE__, spComposedPart );
	}

	bool APICALL IComposedAssetManager_I::removeComponent(pcICompositionRelationship relationship, pcIError_base & error) __NOTHROW__{
		//if (!relationship) return NULL;
		auto spRelationship = ICompositionRelationship::MakeShared(relationship);
		//if (error) return NULL;
		return CallUnSafeFunction<
			IComposedAssetManager_v1, bool, bool, const spcICompositionRelationship &
		>(error, this, 0, &IComposedAssetManager_v1::RemoveComponent, __FILE__, __LINE__, spRelationship);
	}

	/*sizet IComposedAssetManager_I::RemoveComponents( const spcIXMPStructureNode & component,
	const spcIAssetPart & componentPart )
	{
		auto spComponentPart = NS_INT_XMPCOMMON::MakeSharedPointer< const IAssetPart_I >( componentPart.get() );
		return RemoveComponents( component, spComponentPart );
	}

	sizet IComposedAssetManager_I::RemoveComponents( const char * documentID, const char * instanceID,
		const spcIAssetPart & componentPart )
	{
		auto spComponentPart = NS_INT_XMPCOMMON::MakeSharedPointer< const IAssetPart_I >( componentPart.get() );
		return RemoveComponents( documentID, instanceID, spComponentPart );
	}

	sizet IComposedAssetManager_I::RemoveComponents( const spcIAssetPart & composedPart ) {
		auto spComposedPart = NS_INT_XMPCOMMON::MakeSharedPointer< const IAssetPart_I >( composedPart.get() );
		return RemoveComponents( spComposedPart );
	}

	bool IComposedAssetManager_I::RemoveComponent(const spcICompositionRelationship & relationship) {
		auto spRelationship = NS_INT_XMPCOMMON::MakeSharedPointer< const ICompositionRelationship_I >(relationship.get());
		return RemoveComponent(spRelationship);
	}
	*/
	static sizet CopyInnerListAsListOfRawPointers( const spcICompositionRelationshipList & list,
	pcICompositionRelationship_base *& array, MemAllocateProc allocFunc, pcIError_base & error)
	{
		sizet nElements = list->size();
		array = ( pcICompositionRelationship_base * ) allocFunc( nElements * sizeof( pcICompositionRelationship_base ) );
		if ( array ) {
			for ( sizet i = 0; i < nElements; i++ ) {
				const spcICompositionRelationship & ref = list->at( i );
				ref->GetISharedObject_I()->AcquireInternal();
				array[ i ] = ref.get();
			}
		} else {
			    spIError_I _error = IError_I::CreateError(
				IError_base::kEDMemoryManagement,
				kMMECAllocationFailure,
				IError_base::kESOperationFatal);
			_error->SetLocation( __FILE__, __LINE__ );
			_error->SetMessage( "Memory allocation failure" );
			_error->AcquireInternal();
			error = _error->GetActualIError();
		}
		return nElements;
	}

	static spcICompositionRelationshipList CopyInnerListAsPublicList( const spcICompositionRelationshipList & list ) {
		sizet nElements = list->size();
		spcICompositionRelationshipList newList = MakeUncheckedSharedPointerWithDefaultDelete(
			new cICompositionRelationshipList(), __FILE__, __LINE__ );
		newList->reserve( nElements );
		for ( sizet i = 0; i < nElements; i++ ) {
			newList->push_back ( list->at( i ) );
		}
		return newList;
	}

	sizet APICALL IComposedAssetManager_I::getAllRelationships(pcICompositionRelationship_base *& array, MemAllocateProc allocFunc,
		pcIError_base & error ) const __NOTHROW__
	{
		auto list = CallConstUnSafeFunction<
		IComposedAssetManager_v1, spcICompositionRelationshipList, spcICompositionRelationshipList
		>(error, this, spcICompositionRelationshipList(), &IComposedAssetManager_v1::GetAllRelationships, __FILE__, __LINE__);

		return CopyInnerListAsListOfRawPointers( list, array, allocFunc, error );
	}

	/*spcICompositionRelationshipList IComposedAssetManager_I::GetAllRelationships() const {
		spcICompositionRelationshipList_I list = GetAllRelationships_I();
		return CopyInnerListAsPublicList( list );
	}*/

	sizet APICALL IComposedAssetManager_I::getRelationships( pcIStructureNode_base component,
		pcIAssetPart_base componentPart, pcICompositionRelationship_base *& array, MemAllocateProc allocFunc,
		pcIError_base & error ) const __NOTHROW__
	{
		//if (!component) return NULL;
		auto spComponentMetadata = IStructureNode::MakeShared( component );
		//if ( error ) return NULL;
		//if (!componentPart) return NULL;
		auto spComponentPart = IAssetPart::MakeShared( componentPart );
		//if ( error ) return NULL;
		typedef spcICompositionRelationshipList(APICALL IComposedAssetManager_v1::*func)(const spcIStructureNode &, const spcIAssetPart &)const;
		func f = &IComposedAssetManager_v1::GetRelationships;
		auto list = CallConstUnSafeFunction<
			IComposedAssetManager_v1, spcICompositionRelationshipList, spcICompositionRelationshipList,
			const spcIStructureNode &, const spcIAssetPart & 
		>( error, this, spcICompositionRelationshipList(),f /*&IComposedAssetManager_I::GetRelationships*/, __FILE__, __LINE__,
			spComponentMetadata, spComponentPart ); 
		return CopyInnerListAsListOfRawPointers( list, array, allocFunc, error );
	}

	sizet APICALL IComposedAssetManager_I::getRelationships( const char * documentID, sizet docIDLength, const char * instanceID, sizet insIDLength,
		pcIAssetPart_base componentPart, pcICompositionRelationship_base *& array, MemAllocateProc allocFunc,
		pcIError_base & error ) const __NOTHROW__
	{
		//if (!componentPart) return NULL;
		auto spComponentPart = IAssetPart::MakeShared( componentPart );
		//if ( error ) return NULL;
		typedef spcICompositionRelationshipList(APICALL IComposedAssetManager_v1::*func)(const char *, sizet, const char *, sizet, const spcIAssetPart &)const;
		func f = &IComposedAssetManager_v1::GetRelationships;
		auto list = CallConstUnSafeFunction<
			IComposedAssetManager_v1, spcICompositionRelationshipList, spcICompositionRelationshipList,
			const char *, sizet, const char *, sizet, const spcIAssetPart &
		>( error, this, spcICompositionRelationshipList(),f /*&IComposedAssetManager_I::GetRelationships*/, __FILE__, __LINE__,
			documentID, docIDLength, instanceID, insIDLength, spComponentPart );
		return CopyInnerListAsListOfRawPointers( list, array, allocFunc, error );
	}

	sizet APICALL IComposedAssetManager_I::getRelationships( pcIAssetPart_base composedPart,
		pcICompositionRelationship_base *& array, MemAllocateProc allocFunc, pcIError_base & error ) const __NOTHROW__
	{
		// (!composedPart) return NULL;
		auto spComposedPart = IAssetPart::MakeShared( composedPart );
		//if ( error ) return NULL;
		typedef spcICompositionRelationshipList(APICALL IComposedAssetManager_v1::*func)(const spcIAssetPart &)const;
		func f = &IComposedAssetManager_v1::GetRelationships;
		auto list = CallConstUnSafeFunction<
			IComposedAssetManager_v1, spcICompositionRelationshipList, spcICompositionRelationshipList,
			const spcIAssetPart &
		>( error, this, spcICompositionRelationshipList(),f /*&IComposedAssetManager_I::GetRelationships*/, __FILE__, __LINE__,
			spComposedPart );
		return CopyInnerListAsListOfRawPointers( list, array, allocFunc, error );
	}

/*	spcICompositionRelationshipList IComposedAssetManager_I::GetRelationships( const spcIXMPStructureNode & component,
	const spcIAssetPart & componentPart ) const
	{
		auto spComponentPart = NS_INT_XMPCOMMON::MakeSharedPointer< const IAssetPart_I >( componentPart.get() );
		auto list = GetRelationships( component, spComponentPart );
		return CopyInnerListAsPublicList( list );
	}

	spcICompositionRelationshipList IComposedAssetManager_I::GetRelationships( const char * documentID,
		const char * instanceID, const spcIAssetPart & componentPart ) const
	{
		auto spComponentPart = NS_INT_XMPCOMMON::MakeSharedPointer< const IAssetPart_I >( componentPart.get() );
		auto list = GetRelationships( documentID, instanceID, spComponentPart );
		return CopyInnerListAsPublicList( list );
	}

	spcICompositionRelationshipList IComposedAssetManager_I::GetRelationships( const spcIAssetPart & composedPart ) const {
		auto spComposedPart = NS_INT_XMPCOMMON::MakeSharedPointer< const IAssetPart_I >( composedPart.get() );
		auto list = GetRelationships( spComposedPart );
		return CopyInnerListAsPublicList( list );
	}
	*/
	uint32 APICALL IComposedAssetManager_I::isDirectlyComposedOf( pcIStructureNode_base component,
		pcIAssetPart_base componentPart, pcIError_base & error ) const __NOTHROW__
	{
		//if (!component) return NULL;
		auto spComponentMetadata = IStructureNode::MakeShared(component);
		//if (error) return 0;
		//if (!componentPart) return NULL;
		auto spComponentPart = IAssetPart::MakeShared( componentPart );
		//if ( error ) return 0;
		typedef bool(APICALL IComposedAssetManager_v1::*func)(const spcIStructureNode &, const spcIAssetPart &)const;
		func f = &IComposedAssetManager_v1::IsDirectlyComposedOf;
		return CallConstUnSafeFunction<
			IComposedAssetManager_v1, uint32, bool, const spcIStructureNode &, const spcIAssetPart &
		>( error, this, 0, f /*&IComposedAssetManager_I::IsDirectlyComposedOf*/, __FILE__, __LINE__, spComponentMetadata, spComponentPart );
	}

	uint32 APICALL IComposedAssetManager_I::isDirectlyComposedOf( const char * documentID, sizet docIDLength, const char * instanceID, sizet insIDLength,
		pcIAssetPart_base componentPart, pcIError_base & error ) const __NOTHROW__
	{
		//if (!componentPart) return NULL;
		auto spComponentPart = IAssetPart::MakeShared( componentPart );
		//if ( error ) return 0;
		typedef bool(APICALL IComposedAssetManager_v1::*func)(const char *, sizet, const char *, sizet, const spcIAssetPart &)const;
		func f = &IComposedAssetManager_v1::IsDirectlyComposedOf;
		return CallConstUnSafeFunction<
			IComposedAssetManager_v1, uint32, bool, const char *, sizet, const char *, sizet, const spcIAssetPart &
		>( error, this, 0, f /*&IComposedAssetManager_I::IsDirectlyComposedOf*/, __FILE__, __LINE__, documentID, docIDLength, instanceID, insIDLength, spComponentPart );
	}
		/*
	bool IComposedAssetManager_I::IsDirectlyComposedOf( const spcIXMPStructureNode & component, const spcIAssetPart & componentPart /*= spcIAssetPart() */ /*) const  {
		auto spComponentPart = NS_INT_XMPCOMMON::MakeSharedPointer< const IAssetPart_I >( componentPart.get() );
		return IsDirectlyComposedOf( component, spComponentPart );
	}

	bool IComposedAssetManager_I::IsDirectlyComposedOf( const char * documentID, const char * instanceID /*= NULL*//*, const spcIAssetPart & componentPart /*= spcIAssetPart() *//* ) const  {
		auto spComponentPart = NS_INT_XMPCOMMON::MakeSharedPointer< const IAssetPart_I >( componentPart.get() );
		return IsDirectlyComposedOf( documentID, instanceID, spComponentPart );
	}*/

	uint32 APICALL IComposedAssetManager_I::isComposedOf( pcIStructureNode_base component, pcIAssetPart_base componentPart, pcIError_base & error ) const __NOTHROW__{
		//if (!component) return NULL;
		auto spComponentMetadata = IStructureNode::MakeShared( component );
		//if ( error ) return 0;
		//if (!componentPart) return NULL;
		auto spComponentPart = IAssetPart::MakeShared( componentPart );
		//if ( error ) return 0;
		typedef bool(APICALL IComposedAssetManager_v1::*func)(const spcIStructureNode &, const spcIAssetPart &)const;
		func f = &IComposedAssetManager_v1::IsComposedOf;
		return CallConstUnSafeFunction<
			IComposedAssetManager_v1, uint32, bool, const spcIStructureNode &, const spcIAssetPart &
		>( error, this, 0, f /*&IComposedAssetManager_I::IsComposedOf*/, __FILE__, __LINE__, spComponentMetadata, spComponentPart );
	}

	uint32 APICALL IComposedAssetManager_I::isComposedOf( const char * documentID, sizet docIDLength, const char * instanceID, sizet insIDLength, pcIAssetPart_base componentPart, pcIError_base & error ) const __NOTHROW__{
		//if (!componentPart) return NULL;
		auto spComponentPart = IAssetPart::MakeShared( componentPart );
		//if ( error ) return 0;
		typedef bool(APICALL IComposedAssetManager_v1::*func)(const char *, sizet, const char *, sizet, const spcIAssetPart &)const;
		func f = &IComposedAssetManager_v1::IsComposedOf;
		return CallConstUnSafeFunction<
			IComposedAssetManager_v1, uint32, bool, const char *, sizet, const char *, sizet, const spcIAssetPart &
		>( error, this, 0, f /*&IComposedAssetManager_I::IsComposedOf*/, __FILE__, __LINE__, documentID, docIDLength, instanceID, insIDLength, spComponentPart );
	}
		/*
	bool IComposedAssetManager_I::IsComposedOf( const spcIXMPStructureNode & component, const spcIAssetPart & componentPart /*= spcIAssetPart() *//* ) const  {
		auto spComponentPart = NS_INT_XMPCOMMON::MakeSharedPointer< const IAssetPart_I >( componentPart.get() );
		return IsComposedOf( component, spComponentPart );
	}

	bool IComposedAssetManager_I::IsComposedOf( const char * documentID, const char * instanceID /*= NULL*//*, const spcIAssetPart & componentPart /*= spcIAssetPart() *//* ) const  {
		auto spComponentPart = NS_INT_XMPCOMMON::MakeSharedPointer< const IAssetPart_I >( componentPart.get() );
		return IsComposedOf( documentID, instanceID, spComponentPart );
	}*/

}

namespace AdobeXMPAM {
	using namespace AdobeXMPAM_Int;

#if SOURCE_COMPILING_XMPEXTENSIONS_LIB
	spIComposedAssetManager IComposedAssetManager_v1::CreateComposedAssetManager( const spIMetadata & metadata )
	{
		return IComposedAssetManager_I::CreateComposedAssetManager( metadata );
	}
#endif
}
