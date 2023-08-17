#ifndef ICompositionRelationship_I_h__
#define ICompositionRelationship_I_h__ 1
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
// 11-26-14 SKP 1.0-a012 Made changed to build AML on Mac cl:157148 
// 11-25-14 ADC 1.0-a009 Fixing compile and linking issues.
// 11-24-14 ADC 1.0-a008 Support for ICompositionRelationship interface.
//
// =================================================================================================
#endif  // AdobePrivate

#include "XMPAssetManagement/XMPAssetManagementFwdDeclarations_I.h"
#include "XMPAssetManagement/Interfaces/ICompositionRelationship.h"

#include "XMPCommon/Interfaces/BaseInterfaces/ISharedObject_I.h"
#include "XMPCommon/Interfaces/BaseInterfaces/IThreadSafe_I.h"
#include "XMPCommon/Interfaces/IUTF8String.h"

#if XMP_WinBuild
#pragma warning( push )
#pragma warning( disable : 4250 )
#endif //#if XMP_WinBuild
namespace AdobeXMPAM_Int {
	using namespace AdobeXMPCommon;
	using namespace AdobeXMPCommon_Int;
	using namespace AdobeXMPAM;

	class ICompositionRelationship_I
		: public virtual ICompositionRelationship
		, public virtual ISharedObject_I
		,public virtual IThreadSafe_I
	{
	public:
		/*
		//!
		//! Get the details about the part used from the component asset.
		//! \return a shared pointer of const #NS_INT_XMPASSETMANAGEMENT::IAssetPart_I object containing all the details
		//! about the part used from the component asset.
		//!
		virtual spcIAssetPart_I GetComponentPart_I() const = 0;

		//!
		//! Get the details about the part of the composed asset which uses the component part.
		//! \return a shared pointer of const #NS_INT_XMPASSETMANAGEMENT::IAssetPart_I object containing all the details
		//! about the part of the composed asset which uses the component part.
		virtual spcIAssetPart_I GetComposedPart_I() const = 0;

		//!
		//! Get the list of alternate paths to be used for locating the component asset.
		//! \return a vector of shared pointers of const #NS_INT_XMPCOMMON::IUTF8String_I objects containing
		//! the alternate path to be used for locating the component asset.
		//!
		*/
		virtual spcIUTF8Strings GetComponentAlternatePaths_I() const = 0;
		/*
		//!
		//! Get the document ID of the component asset.
		//! \return a shared pointer of const NS_INT_XMPCOMMON::IUTF8String_I object containing the document
		//! ID of the component asset.
		//!
		virtual spcIUTF8String GetComponentDocumentID_I() const = 0;

		//!
		//! Get the instance ID of the component asset.
		//! \return a shared pointer of const NS_INT_XMPCOMMON::IUTF8String_I object containing the instance
		//! ID of the component asset.
		//!
		virtual spcIUTF8String GetComponentInstanceID_I() const = 0;

		//!
		//! Get the document ID of the composed asset.
		//! \return a shared pointer of const NS_INT_XMPCOMMON::IUTF8String_I object containing the document
		//! ID of the composed asset.
		//!
		virtual spcIUTF8String GetComposedDocumentID_I() const = 0;

		//!
		//! Get the instance ID of the composed asset.
		//! \return a shared pointer of const NS_INT_XMPCOMMON::IUTF8String_I object containing the instance
		//! ID of the composed asset.
		//!
		virtual spcIUTF8String GetComposedInstanceID_I() const = 0;

		//!
		//! Get the file path of the component asset.
		//! \return a shared pointer of const #NS_XMPCOMMON::IUTF8String object containing the file path
		//! of the component asset.
		//! \note an invalid shared pointer is returned in case no information regarding file path of the
		//! component asset is available or has been explicitly removed by the client.
		//!
		virtual spcIUTF8String GetComponentFilePath_I() const = 0;

		//!
		//! Get the name of a mapping function used to map the part from the component asset to the part
		//! in composed asset.
		//! \return a shared pointer of const #NS_INT_XMPCOMMON::IUTF8String_I containing the name of a mapping
		//! function used to map the part from the component asset to the part in composed asset.
		//! \note return an invalid shared pointer in case no name is provided by the client.
		//!
		virtual spcIUTF8String GetMappingFunctionName_I() const = 0;

        //!
		//! Get the metadata of the component asset.
		//! \return a shared pointer of const #NS_XMPCORE::IStructureNode object containing the metadata of
		//! the component asset.
		//!
		virtual spcIStructureNode GetComponentMetadata() const = 0;
        
        //!
		//! Get the metadata of the composed asset.
		//! \return a shared pointer of const #NS_XMPCORE::IStructureNode object containing the metadata of
		//! the composed asset.
		
        virtual spcIStructureNode APICALL GetComposedMetadata() const = 0;
        
        //!
		//! Gets whether markers in the component should be ignored( masked ) or processed normally.
		//! \return a value of type #eCompositionMaskMarkers indicating whether markers in the component
		//! should be ignored or not.
		//! \note default value is kCompositionMaskMarkersNotSpecified if not set by the client.
		//!
		virtual eCompositionMaskMarkers GetMaskMarkersFlag() const = 0;
        
		//!
		//! Sets whether markers in the component should be ignored( masked ) or processed normally.
		//! \param[in] flag a value of type #eCompositionMaskMarkers indicating whether markers in
		//! the component should be ignored or not.
		//! \attention Error is thrown in case value of flag is not the one supported by the library.
		//!
		virtual void SetMaskMarkersFlag( eCompositionMaskMarkers flag ) = 0;
        
        //!
		//! Appends an alternate path of the component asset to the list of alternate paths to be used
		//! for locating the asset.
		//! \param[in] alternatePath a pointer to const NULL terminated char buffer containing an
		//! alternate path to the component asset.
		//! \attention Error is thrown in case alternatePath is a NULL pointer or its contents are empty.
		//!
		virtual void AppendComponentAlternatePath( const char * alternatePath ) = 0;
        
		//!
		//! Set structure node representing the relationship
		//! \param[in] node referring to the structure entry into Ingredient
		//!
		*/
		virtual void SetIngredientNode(spIStructureNode node) = 0;

		//!
		//! Get structure node representing the relationship
		//! \return a value of type #spIStructureNode structure node representing the
		//! structure relationship
		//!
		virtual spIStructureNode GetIngredientNode(void) = 0;

		static uint32 GetInterfaceVersion() { return kInternalInterfaceVersionNumber; }
		virtual pICompositionRelationship APICALL GetActualICompositionRelationship() __NOTHROW__{ return this; }
		virtual pICompositionRelationship_I APICALL GetICompositionRelationship_I() __NOTHROW__{ return this; }

		virtual pvoid APICALL GetInterfacePointer(uint64 interfaceID, uint32 interfaceVersion);

		using ICompositionRelationship::SetMappingFunctionName;
		using ICompositionRelationship::GetMaskMarkersFlag;
		using ICompositionRelationship::SetMaskMarkersFlag;
		using ICompositionRelationship::SetComponentFilePath;
		using ICompositionRelationship::AppendComponentAlternatePath;
		using ICompositionRelationship::ClearComponentAlternatePaths;

		// Factories functions to create various kind of objects
		static spICompositionRelationship_I CreateCompositionRelationship( const spIMetadata & composedMetadata,
			const spcIAssetPart & composedPart, const spcIStructureNode & componentMetadata,
			const spcIAssetPart & componentPart);

	protected:
		
		virtual pcIAssetPart_base APICALL getComponentPart( pcIError_base & error ) const __NOTHROW__;
		virtual pcIAssetPart_base APICALL getComposedPart(pcIError_base & error) const __NOTHROW__;
		virtual pcIStructureNode_base APICALL getComponentMetadata(pcIError_base & error) const __NOTHROW__;
		virtual pcIStructureNode_base APICALL getComposedMetadata(pcIError_base & error) const __NOTHROW__;
		virtual pcIUTF8String_base APICALL getMappingFunctionName(pcIError_base & error) const __NOTHROW__;
		virtual uint32 APICALL getMaskMarkersFlag(pcIError_base & error) const __NOTHROW__;
		virtual void APICALL setMaskMarkersFlag(uint32 flag, pcIError_base & error) __NOTHROW__;
		virtual pcIUTF8String APICALL getComponentDocumentID(pcIError_base & error) const __NOTHROW__;
		virtual pcIUTF8String APICALL getComponentInstanceID(pcIError_base & error) const __NOTHROW__;
		virtual pcIUTF8String APICALL getComposedDocumentID(pcIError_base & error) const __NOTHROW__;
		virtual pcIUTF8String APICALL getComposedInstanceID(pcIError_base & error) const __NOTHROW__;
		virtual pcIUTF8String APICALL getComponentFilePath(pcIError_base & error) const __NOTHROW__;
		virtual void APICALL appendComponentAlternatePath(const char * alternatePath, sizet altPathLength, pcIError_base & error) __NOTHROW__;

		pvoid APICALL GetInterfacePointerInternal(uint64 interfaceID, uint32 interfaceVersion, bool isTopLevel);
		virtual pvoid APICALL getInterfacePointer(uint64 interfaceID, uint32 interfaceVersion, pcIError_base & error) __NOTHROW__;

		/*virtual spcIUTF8String GetMappingFunctionName() const;
		virtual spcIUTF8String GetComponentDocumentID() const;
		virtual spcIUTF8String GetComponentInstanceID() const;
		virtual spcIUTF8String GetComposedDocumentID() const;
		virtual spcIUTF8String GetComposedInstanceID() const;
		virtual spcIAssetPart GetComponentPart() const;
		virtual spcIAssetPart GetComposedPart() const;*/
		virtual spcIUTF8Strings APICALL GetComponentAlternatePaths() const;
		//virtual spcIUTF8String GetComponentFilePath() const;

		//!
		//! protected Virtual Destructor
		//!
		virtual ~ICompositionRelationship_I() {};

	#ifdef FRIEND_CLASS_DECLARATION
			FRIEND_CLASS_DECLARATION();
	#endif
			REQ_FRIEND_CLASS_DECLARATION();
	};
}
#if XMP_WinBuild
#pragma warning( pop )
#endif //#if XMP_WinBuild
#endif //#ifndef ICompositionRelationship_I_h__