#ifndef ComposedAssetManagerImpl_h__
#define ComposedAssetManagerImpl_h__ 1

// =================================================================================================
// ADOBE SYSTEMS INCORPORATED
// Copyright 2014 Adobe Systems Incorporated
// All Rights Reserved
//
// NOTICE:  Adobe permits you to use, modify, and distribute this file in accordance with the terms
// of the Adobe license agreement accompanying it.
// =================================================================================================

#if !(IMPLEMENTATION_HEADERS_CAN_BE_INCLUDED)
	#error "Not adhering to design constraints"
	// this file should only be included from its own cpp file
#endif

#include "XMPAssetManagement/Interfaces/IComposedAssetManager_I.h"
#include "XMPCommon/ImplHeaders/SharedObjectImpl.h"
#include "XMPCommon/ImplHeaders/ThreadSafeImpl.h"
#include "XMPCommon/BaseClasses/MemoryManagedObject.h"

//#include "XMPCommon/Interfaces/ISharedMutex_I.h"

#include <list>
#include <map>

#if XMP_WinBuild
	#pragma warning( push )
	#pragma warning( disable : 4250 )
#endif

namespace AdobeXMPAM_Int {
	using namespace AdobeXMPAM_Int;
	using namespace AdobeXMPCommon;
	using namespace AdobeXMPCommon_Int;
	
	class ComposedAssetManagerImpl
		: public virtual IComposedAssetManager_I
		, public virtual SharedObjectImpl
		, public virtual MemoryManagedObject
		, public virtual ThreadSafeImpl
	{
	public:

		ComposedAssetManagerImpl( const spIMetadata & composedAssetMetadata );

		virtual spICompositionRelationship APICALL AddComponent(const spcIAssetPart & composedPart, const spcIStructureNode & component, const spcIAssetPart & componentPart);

		virtual sizet APICALL RemoveAllComponents() __NOTHROW__;

		//using IComposedAssetManager_I::RemoveComponents;
		virtual sizet APICALL RemoveComponents( const spcIStructureNode & component, const spcIAssetPart & componentPart = spcIAssetPart() );
		virtual sizet APICALL RemoveComponents(const char * documentID, sizet docIDLength, const char * instanceID = NULL, sizet insIDLength = 0, const spcIAssetPart & componentPart = spcIAssetPart());
		virtual sizet APICALL RemoveComponents(const spcIAssetPart & composedPart);
		virtual bool APICALL RemoveComponent(const spcICompositionRelationship& relationship);
		//virtual bool RemoveComponent_I(const spcICompositionRelationship_I& relationship);

		//virtual spcICompositionRelationshipList_I GetAllRelationships_I() const;
		virtual spcICompositionRelationshipList APICALL GetAllRelationships() const;
		virtual spcICompositionRelationshipList APICALL GetRelationships(const spcIStructureNode & component, const spcIAssetPart & componentPart = spcIAssetPart()) const;
		virtual spcICompositionRelationshipList APICALL GetRelationships(const char * documentID, sizet docIDLength, const char * instanceID = NULL, sizet insIDLength = 0, const spcIAssetPart & componentPart = spcIAssetPart()) const;
		virtual spcICompositionRelationshipList APICALL GetRelationships(const spcIAssetPart & composedPart) const;

		//using IComposedAssetManager_I::IsDirectlyComposedOf;
		virtual bool APICALL IsDirectlyComposedOf( const spcIStructureNode & component, const spcIAssetPart & componentPart = spcIAssetPart() ) const;
		virtual bool APICALL IsDirectlyComposedOf(const char * documentID, sizet docIDLength, const char * instanceID = NULL, sizet insIDLength = 0, const spcIAssetPart & componentPart = spcIAssetPart()) const;

		//using IComposedAssetManager_I::IsComposedOf;
		virtual bool APICALL IsComposedOf(const spcIStructureNode & component, const spcIAssetPart & componentPart = spcIAssetPart()) const;
		virtual bool APICALL IsComposedOf(const char * documentID, sizet docIDLength, const char * instanceID = NULL, sizet insIDLength = 0, const spcIAssetPart & componentPart = spcIAssetPart()) const;

	private:
		/*Get IXMPMeta Ingredients node; if not present create and add to ixmpmeta*/
		spIArrayNode GetIXMPMetaIngredientNode();

		/*Get IXMPMeta Pantry node; if not present create and add to ixmpmeta*/
		spIArrayNode GetIXMPMetaPantryNode();

		void ResetIngredientAndPantryEntry();
	protected:
		virtual ~ComposedAssetManagerImpl() __NOTHROW__;

	protected:
		//spISharedMutex_I			mSharedMutex;
		
		spIMetadata				   mComposedAssetMetadata; //ixmpmetadata of composed metadata

		//Keep ingredient & Pantry pointer handy to optimize delete an entry
		spIArrayNode				mComposedIngredientArray;
		spIArrayNode				mComposedPantryArray;

		spICompositionRelationshipList mRelationshipList;
		std::list<std::pair<spICompositionRelationship, spIStructureNode> > mRelationshipAndIngredientPtrList;
		//std::map < std::string, std::list<spICompositionRelationship> > mComposedPartRelationshipList;
		
		std::map<std::string, spIStructureNode > mComposedPantryList;
		std::map<std::string, uint32> mComponentCountPantryList;

	#ifdef FRIEND_CLASS_DECLARATION
			FRIEND_CLASS_DECLARATION();
	#endif
			REQ_FRIEND_CLASS_DECLARATION();


	};
	
	inline ComposedAssetManagerImpl::~ComposedAssetManagerImpl() __NOTHROW__ { }
}

#if XMP_WinBuild
	#pragma warning( pop )
#endif

#endif  // ComposedAssetManagerImpl_h__
