#ifndef IComposedAssetManager_I_h__
#define IComposedAssetManager_I_h__ 1

// =================================================================================================
// ADOBE SYSTEMS INCORPORATED
// Copyright 2014 Adobe Systems Incorporated
// All Rights Reserved
//
// NOTICE:  Adobe permits you to use, modify, and distribute this file in accordance with the terms
// of the Adobe license agreement accompanying it.
// =================================================================================================

#include "XMPAssetManagement/XMPAssetManagementFwdDeclarations_I.h"
#include "XMPAssetManagement/Interfaces/IComposedAssetManager.h"
#include "XMPCommon/Interfaces/BaseInterfaces/ISharedObject_I.h"
#include "XMPCommon/Interfaces/BaseInterfaces/IThreadSafe_I.h"

#if XMP_WinBuild
#pragma warning( push )
#pragma warning( disable : 4250 )
#endif //#if XMP_WinBuild
namespace AdobeXMPAM_Int {
	using namespace AdobeXMPCommon;
	using namespace AdobeXMPCommon_Int;
	using namespace AdobeXMPAM;

	class IComposedAssetManager_I
		: public virtual IComposedAssetManager
		, public virtual ISharedObject_I
		, public virtual IThreadSafe_I
	{
	public:

		//!
		//! Add a component asset to a composite asset mentioning the part of component that is
		//! being used along with the part of the composite asset where it will be placed.
		//! \param[in] composedPart a shared pointer to a const #NS_INT_XMPASSETMANAGEMENT::IAssetPart_I
		//! mentioning the part in the composite asset where the component asset's part will be
		//! placed.
		//! \param[in] component a shared pointer of const #NS_XMPCORE::IStructureNode containing
		//! the metadata of the component asset.
		//! \param[in] componentPart a shared pointer of const #NS_INT_XMPASSETMANAGEMENT::IAssetPart_I
		//! mentioning the part of the component asset that is being used.
		//! \return a shared pointer of #NS_INT_XMPASSETMANAGEMENT::ICompositionRelationship_I
		//! containing all the aspects of the relationship between the composite asset's part with
		//! the component asset's part.
		//! \attention Error is thrown in case
		//!		-# any of the shared pointer is invalid.
		//!		-# component is non track able asset.
		//!
		/*virtual spICompositionRelationship_I APICALL AddComponent_I( const spcIAssetPart_I & composedPart,
			const spcIStructureNode & component, const spcIAssetPart_I & componentPart ) = 0;

		//!
		//! Remove all the composition relationships maintained in the metadata of the composite document
		//! for the given component whose metadata is provided and an optional component part.
		//! \param[in] component a shared pointer of const #NS_XMPCORE::IStructureNode containing
		//! the metadata of the component asset.
		//! \param[in] componentPart a shared pointer of const #NS_INT_XMPASSETMANAGEMENT::IAssetPart_I
		//! mentioning the part of the component asset that is being used.
		//! \return a value of type #NS_XMPCOMMON::sizet containing the number of relationships removed.
		//! \attention Error is thrown in case
		//!		-# component is an invalid shared pointer.
		//!		-# component is non track able asset.
		//! \note setting componentPart as invalid pointer means ignore the part.
		//!
		virtual sizet APICALL RemoveComponents(const spcIStructureNode & component, const spcIAssetPart_I & componentPart = spcIAssetPart_I()) = 0;

		//!
		//! Remove all the composition relationships maintained in the metadata of the composite document
		//! for the component whose documentID and instanceID (optional ) are provided and an optional
		//! component part.
		//! \param[in] documentID a pointer to a const NULL terminated char buffer containing the document
		//! ID of the component's asset.
		//! \param[in] instanceID a pointer to a const NULL terminated char buffer containing the instance
		//! ID of the component's asset.
		//! \param[in] componentPart a shared pointer of const #NS_INT_XMPASSETMANAGEMENT::IAssetPart_I
		//! mentioning the part of the component asset that is being used.
		//! \return a value of type #NS_XMPCOMMON::sizet containing the number of relationships removed.
		//! \attention Error is thrown in case
		//!		-# documentID is a null pointer or it contents are empty.
		//!		-# instanceID is non null pointer and its contents are empty.
		//! \note setting instanceID as NULL means ignore the instanceIDs.
		//! \note setting componentPart as invalid pointer means ignore the part.
		//!
		virtual sizet APICALL RemoveComponents(const char * documentID, const char * instanceID = NULL, const spcIAssetPart_I & componentPart = spcIAssetPart_I()) = 0;

		//!
		//! Remove all the composition relationships maintained in the metadata of the composite document
		//! for a given composite asset part.
		//! \param[in] composedPart a shared pointer of const #NS_INT_XMPASSETMANAGEMENT::IAssetPart_I mentioning
		//! the part of the composite asset whose relationships are no longer required.
		//! \return a value of type #NS_XMPCOMMON::sizet containing the number of relationships removed.
		//! \attention Error is thrown in case
		//!		-# composedPart is an invalid shared pointer.
		//! 
		virtual sizet APICALL RemoveComponents(const spcIAssetPart_I & composedPart) = 0;

		//!
		//! Remove the composition relationship maintained in the metadata of the composite document for a
		//! given composite asset part.
		//! \param[in] relationship of a shared pointer of const #AdobeXMPAM::ICompositionRelationship
		//!  mentioning the relationship which is no longer required.
		//! \return a value of type true if relationship has been removed otherwise false.
		//! \attention Error is thrown in case
		//!		-# relationship is an invalid shared pointer.
		//! 
		virtual bool APICALL RemoveComponent_I(const spcICompositionRelationship_I& relationship) = 0;

		//!
		//! Get the list of all the composition relationships for a composite asset.
		//! \return a vector consisting of shared pointers of const
		//! #NS_INT_XMPASSETMANAGEMENT::ICompositionRelationships_I.
		//
		virtual spcICompositionRelationshipList_I APICALL GetAllRelationships_I() const = 0;

		//!
		//! Get the list of all the composition relationships for a composite asset for a given component
		//! and an optional componentPart.
		//! \param[in] component a shared pointer of const #NS_XMPCORE::IStructureNode containing
		//! the metadata of the component asset.
		//! \param[in] componentPart a shared pointer of const #NS_INT_XMPASSETMANAGEMENT::IAssetPart_I
		//! mentioning the part of the component asset that is being used.
		//! \return a a vector consisting of shared pointers of const
		//! #NS_INT_XMPASSETMANAGEMENT::ICompositionRelationships_I which meets the given criteria.
		//! \attention Error is thrown in case
		//!		-# component is an invalid shared pointer.
		//!		-# component is non track able asset.
		//! \note setting componentPart as invalid pointer means ignore the part.
		//!
		virtual spcICompositionRelationshipList_I APICALL GetRelationships(const spcIStructureNode & component,
			const spcIAssetPart_I & componentPart = spcIAssetPart_I() ) const = 0;

		//!
		//! Get the list of all the composition relationships for a composite asset whose documentID and
		//! instanceID( optional ) are provided and an optional component part.
		//! \param[in] documentID a pointer to a const NULL terminated char buffer containing the document
		//! ID of the component's asset.
		//! \param[in] instanceID a pointer to a const NULL terminated char buffer containing the instance
		//! ID of the component's asset.
		//! \param[in] componentPart a shared pointer of const #NS_INT_XMPASSETMANAGEMENT::IAssetPart_I
		//! mentioning the part of the component asset that is being used.
		//! \return a a vector consisting of shared pointers of const
		//! #NS_INT_XMPASSETMANAGEMENT::ICompositionRelationships_I which meets the given criteria.
		//! \attention Error is thrown in case
		//!		-# documentID is a null pointer or it contents are empty.
		//!		-# instanceID is non null pointer and its contents are empty.
		//! \note setting instanceID as NULL means ignore the instanceIDs.
		//! \note setting componentPart as invalid pointer means ignore the part.for a given component
		//! and an optional componentPart.
		//!
		virtual spcICompositionRelationshipList_I APICALL GetRelationships(const char * documentID, const char * instanceID = NULL,
			const spcIAssetPart_I & componentPart = spcIAssetPart_I() ) const = 0;

		//!
		//! Get the list of all the composition relationships for a composite asset for a given composite
		//! asset part.
		//! \param[in] composedPart a shared pointer of const #NS_INT_XMPASSETMANAGEMENT::IAssetPart_I mentioning
		//! the part of the composite asset whose relationships are no longer required.
		//! \return a a vector consisting of shared pointers of const
		//! #NS_INT_XMPASSETMANAGEMENT::ICompositionRelationships_I which meets the given criteria.
		//! \attention Error is thrown in case composedPart is an invalid shared pointer.
		//!
		virtual spcICompositionRelationshipList_I APICALL GetRelationships(const spcIAssetPart_I & composedPart) const = 0;

		//!
		//! Indicates whether a given component whose metadata is provided is directly used in the composite.
		//! \param[in] component a shared pointer of const #NS_XMPCORE::IStructureNode containing
		//! the metadata of the component asset.
		//! \param[in] componentPart a shared pointer of const #NS_INT_XMPASSETMANAGEMENT::IAssetPart_I
		//! mentioning the part of the component asset that is being used.
		//! \return a bool value returning true in case a given component is directly used in the composite.
		//! \attention Error is thrown in case
		//!		-# component is an invalid shared pointer.
		//!		-# component is non track able asset.
		//! \note setting componentPart as invalid pointer means ignore the part.
		//!
		virtual bool APICALL IsDirectlyComposedOf(const spcIStructureNode & component, const spcIAssetPart_I & componentPart = spcIAssetPart_I()) const = 0;

		//!
		//! Indicates whether a given component whose metadata is provided is directly used in the composite.
		//! \param[in] documentID a pointer to a const NULL terminated char buffer containing the document
		//! ID of the component's asset.
		//! \param[in] instanceID a pointer to a const NULL terminated char buffer containing the instance
		//! ID of the component's asset.
		//! \param[in] componentPart a shared pointer of const #NS_INT_XMPASSETMANAGEMENT::IAssetPart_I
		//! mentioning the part of the component asset that is being used.
		//! \return a bool value returning true in case a given component is directly used in the composite.
		//! \attention Error is thrown in case
		//!		-# component is an invalid shared pointer.
		//!		-# component is non track able asset.
		//! \note setting instanceID as NULL means ignore the instanceIDs.
		//! \note setting componentPart as invalid pointer means ignore the part.
		//!
		virtual bool APICALL IsDirectlyComposedOf(const char * documentID, const char * instanceID = NULL,
			const spcIAssetPart_I & componentPart = spcIAssetPart_I() ) const = 0;

		//!
		//! Indicates whether a given component whose metadata is provided is [in]directly used in the composite.
		//! \param[in] component a shared pointer of const #NS_XMPCORE::IStructureNode containing
		//! the metadata of the component asset.
		//! \param[in] componentPart a shared pointer of const #NS_INT_XMPASSETMANAGEMENT::IAssetPart_I
		//! mentioning the part of the component asset that is being used.
		//! \return a bool value returning true in case a given component is [in]directly used in the composite.
		//! \attention Error is thrown in case
		//!		-# component is an invalid shared pointer.
		//!		-# component is non track able asset.
		//! \note setting componentPart as invalid pointer means ignore the part.
		//!
		virtual bool APICALL IsComposedOf(const spcIStructureNode & component, const spcIAssetPart_I & componentPart = spcIAssetPart_I()) const = 0;

		//!
		//! Indicates whether a given component whose metadata is provided is [in]directly used in the composite.
		//! \param[in] documentID a pointer to a const NULL terminated char buffer containing the document
		//! ID of the component's asset.
		//! \param[in] instanceID a pointer to a const NULL terminated char buffer containing the instance
		//! ID of the component's asset.
		//! \param[in] componentPart a shared pointer of const #NS_INT_XMPASSETMANAGEMENT::IAssetPart_I
		//! mentioning the part of the component asset that is being used.
		//! \return a bool value returning true in case a given component is [in]directly used in the composite.
		//! \attention Error is thrown in case
		//!		-# component is an invalid shared pointer.
		//!		-# component is non track able asset.
		//! \note setting instanceID as NULL means ignore the instanceIDs.
		//! \note setting componentPart as invalid pointer means ignore the part.
		//!
		virtual bool APICALL IsComposedOf(const char * documentID, const char * instanceID = NULL,
			const spcIAssetPart_I & componentPart = spcIAssetPart_I() ) const = 0;
		*/

		// Factories functions to create various kind of objects

		static uint32 GetInterfaceVersion() { return kInternalInterfaceVersionNumber; }
		virtual pIComposedAssetManager APICALL GetActualIComposedAssetManager() __NOTHROW__{ return this; }
		virtual pIComposedAssetManager_I APICALL GetIComposedAssetManager_I() __NOTHROW__{ return this; }

		virtual pvoid APICALL GetInterfacePointer(uint64 interfaceID, uint32 interfaceVersion);
		
		static spIComposedAssetManager_I CreateComposedAssetManager( const spIMetadata & metadata);

	protected:
		virtual pICompositionRelationship_base APICALL addComponent(pcIAssetPart_base composedPart,
			pcIStructureNode_base component, pcIAssetPart_base componentPart, pcIError_base & error ) __NOTHROW__;

		virtual sizet APICALL removeComponents(pcIStructureNode_base component, pcIAssetPart_base componentPart, pcIError_base & error) __NOTHROW__;
		virtual sizet APICALL removeComponents(const char * documentID, sizet docIDLength, const char * instanceID, sizet insIDLength, pcIAssetPart_base componentPart, pcIError_base & error) __NOTHROW__;
		virtual sizet APICALL removeComponents(pcIAssetPart_base composedPart, pcIError_base & error) __NOTHROW__;
		virtual bool APICALL  removeComponent(pcICompositionRelationship relationship, pcIError_base & error) __NOTHROW__;

		virtual sizet APICALL  getAllRelationships(pcICompositionRelationship_base *& array, MemAllocateProc allocFunc, pcIError_base & error) const __NOTHROW__;
		virtual sizet APICALL  getRelationships(pcIStructureNode_base component, pcIAssetPart_base componentPart, pcICompositionRelationship_base *& array, MemAllocateProc allocFunc, pcIError_base & error) const __NOTHROW__;
		virtual sizet APICALL  getRelationships(const char * documentID, sizet docIDLength, const char * instanceID, sizet insIDLength, pcIAssetPart_base componentPart, pcICompositionRelationship_base *& array, MemAllocateProc allocFunc, pcIError_base & error) const __NOTHROW__;
		virtual sizet APICALL  getRelationships(pcIAssetPart_base composedPart, pcICompositionRelationship_base *& array, MemAllocateProc allocFunc, pcIError_base & error) const __NOTHROW__;

		virtual uint32 APICALL  isDirectlyComposedOf(pcIStructureNode_base component, pcIAssetPart_base componentPart, pcIError_base & error) const __NOTHROW__;
		virtual uint32 APICALL  isDirectlyComposedOf(const char * documentID, sizet docIDLength, const char * instanceID, sizet insIDLength, pcIAssetPart_base, pcIError_base & error) const __NOTHROW__;

		virtual uint32 APICALL  isComposedOf(pcIStructureNode_base component, pcIAssetPart_base componentPart, pcIError_base & error) const __NOTHROW__;
		virtual uint32 APICALL  isComposedOf(const char * documentID, sizet docIDLength, const char * instanceID, sizet insIDLength, pcIAssetPart_base componentPart, pcIError_base & error) const __NOTHROW__;

		pvoid APICALL GetInterfacePointerInternal(uint64 interfaceID, uint32 interfaceVersion, bool isTopLevel);
		virtual pvoid APICALL getInterfacePointer(uint64 interfaceID, uint32 interfaceVersion, pcIError_base & error) __NOTHROW__;

	/*	virtual spICompositionRelationship APICALL  AddComponent(const spcIAssetPart & composedPart,
			const spcIStructureNode & component, const spcIAssetPart & componentPart );

		virtual sizet APICALL  RemoveComponents(const spcIStructureNode & component, const spcIAssetPart & componentPart = spcIAssetPart());
		virtual sizet APICALL  RemoveComponents(const char * documentID, const char * instanceID = NULL, const spcIAssetPart & componentPart = spcIAssetPart());
		virtual sizet APICALL  RemoveComponents(const spcIAssetPart & composedPart);
		virtual bool APICALL  RemoveComponent(const spcICompositionRelationship & relationship);

		virtual spcICompositionRelationshipList APICALL  GetAllRelationships() const;

		virtual spcICompositionRelationshipList APICALL  GetRelationships(const spcIAssetPart & composedPart) const;
		virtual spcICompositionRelationshipList APICALL  GetRelationships(const spcIStructureNode & component, const spcIAssetPart & componentPart = spcIAssetPart()) const;
		virtual spcICompositionRelationshipList APICALL  GetRelationships(const char * documentID, const char * instanceID = NULL, const spcIAssetPart & componentPart = spcIAssetPart()) const;

		virtual bool APICALL  IsDirectlyComposedOf(const spcIStructureNode & component, const spcIAssetPart & componentPart = spcIAssetPart()) const;
		virtual bool APICALL  IsDirectlyComposedOf(const char * documentID, const char * instanceID = NULL, const spcIAssetPart & componentPart = spcIAssetPart()) const;

		virtual bool APICALL  IsComposedOf(const spcIStructureNode & component, const spcIAssetPart & componentPart = spcIAssetPart()) const;
		virtual bool APICALL  IsComposedOf(const char * documentID, const char * instanceID = NULL, const spcIAssetPart & componentPart = spcIAssetPart()) const;
		*/
		//!
		//! protected Virtual Destructor
		//!
		virtual ~IComposedAssetManager_I() {}
	#ifdef FRIEND_CLASS_DECLARATION
			FRIEND_CLASS_DECLARATION();
	#endif
			REQ_FRIEND_CLASS_DECLARATION();
	};
}
#if XMP_WinBuild
#pragma warning( pop )
#endif //#if XMP_WinBuild
#endif  // IComposedAssetManager_I_h__
