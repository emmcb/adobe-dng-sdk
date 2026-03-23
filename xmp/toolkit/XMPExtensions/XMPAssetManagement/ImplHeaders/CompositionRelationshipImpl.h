#ifndef CompositionRelationshipImpl_h__
#define CompositionRelationshipImpl_h__ 1

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
// 12-01-14 SKP 1.0-a025 Added functionality to add stRef on relationship.
// 11-26-14 ADC 1.0-a013 Added the full skeleton for IComposedAssetManager interface.
// 11-25-14 ADC 1.0-a009 Fixing compile and linking issues.
// 11-24-14 ADC 1.0-a008 Support for ICompositionRelationship interface.
//
// =================================================================================================
#endif  // AdobePrivate

#if !(IMPLEMENTATION_HEADERS_CAN_BE_INCLUDED)
	#error "Not adhering to design constraints"
	// this file should only be included from its own cpp file
#endif

#include "XMPAssetManagement/Interfaces/ICompositionRelationship_I.h"

#include "XMPCommon/BaseClasses/MemoryManagedObject.h"
#include "XMPCommon/ImplHeaders/SharedObjectImpl.h"
//#include "XMPCommon/Interfaces/ISharedMutex.h"
#include "XMPCommon/ImplHeaders/ThreadSafeImpl.h"


#if XMP_WinBuild
	#pragma warning( push )
	#pragma warning( disable : 4250 )
#endif

namespace AdobeXMPAM_Int{
	using namespace AdobeXMPAM_Int;
	using namespace AdobeXMPCommon;
	using namespace AdobeXMPCommon_Int;
	
	class CompositionRelationshipImpl
		: public virtual ICompositionRelationship_I
		, public virtual SharedObjectImpl
		, public virtual MemoryManagedObject
		, public virtual ThreadSafeImpl
	{
	public:
		CompositionRelationshipImpl( const spIMetadata & composedMetadata,
			const spcIAssetPart & composedPart, const spcIStructureNode & componentMetadata,
			const spcIAssetPart & componentPart );

		virtual spcIAssetPart APICALL GetComponentPart() const;
		virtual spcIAssetPart APICALL GetComposedPart() const;
		virtual spcIUTF8Strings GetComponentAlternatePaths_I() const;
		virtual spcIStructureNode APICALL GetComponentMetadata() const;
		virtual spcIStructureNode APICALL GetComposedMetadata() const;

		virtual eCompositionMaskMarkers APICALL GetMaskMarkersFlag() const;
		virtual void APICALL SetMaskMarkersFlag(eCompositionMaskMarkers flag);

		virtual void APICALL SetMappingFunctionName(const char * name, sizet nameLength);
		virtual spcIUTF8String APICALL GetMappingFunctionName() const;

		virtual spcIUTF8String APICALL GetComponentDocumentID() const;
		virtual spcIUTF8String APICALL GetComponentInstanceID() const;
		virtual spcIUTF8String APICALL GetComposedDocumentID() const;
		virtual spcIUTF8String APICALL GetComposedInstanceID() const;

		virtual void APICALL SetComponentFilePath(const char * filePath, sizet filePathLength);
		virtual spcIUTF8String APICALL GetComponentFilePath() const;

		virtual void APICALL AppendComponentAlternatePath(const char * alternatePath, sizet altPathLength);
		virtual void APICALL ClearComponentAlternatePaths();

		virtual void SetIngredientNode(spIStructureNode node);
		virtual spIStructureNode GetIngredientNode(void);

	protected:
		virtual ~CompositionRelationshipImpl() __NOTHROW__;

		virtual sizet  APICALL getComponentAlternatePathsSize() __NOTHROW__;
		virtual pcIUTF8String_base APICALL  getComponentAlternatePathAtIndex(sizet index, pcIError_base & error) __NOTHROW__;

	protected:
		//spISharedMutex_I			mSharedMutex;
		spIStructureNode			mComposedMetadata;
		spcIStructureNode		    mComponentMetadata;
		spcIAssetPart				mComposedPart;
		spcIAssetPart				mComponentPart;
		spIUTF8String				mMappingFunctionName;
		eCompositionMaskMarkers		mMaskMarkersFlag;
		spIUTF8String				mComponentFilePath;
		spcIUTF8Strings		    mComponentAlternatePaths;
		spIStructureNode			mIngredientEntry;

	#ifdef FRIEND_CLASS_DECLARATION
			FRIEND_CLASS_DECLARATION();
	#endif
			REQ_FRIEND_CLASS_DECLARATION();

	};
	
	inline CompositionRelationshipImpl::~CompositionRelationshipImpl() __NOTHROW__ { }
}

#if XMP_WinBuild
	#pragma warning( pop )
#endif

#endif  // CompositionRelationshipImpl_h__
