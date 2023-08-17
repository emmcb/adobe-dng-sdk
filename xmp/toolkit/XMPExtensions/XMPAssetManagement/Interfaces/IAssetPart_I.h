#ifndef IAssetPart_I_h__
#define IAssetPart_I_h__ 1

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
// ADC	Amandeep Chawla
// SKP	Sunil Kishor Pathak
//
// mm-dd-yy who Description of changes, most recent first.
//
// 12-07-14 ADC 1.0-a041 Refactored Part Classes and Adding support for IAssetArtboardPart.
// 12-02-14 SKP 1.0-a028 1. Moved standardComponentStrings in IAssetPart_I.cpp so that it can be
//							used in other cpp files, 2. fixed issues in SerializeAsRDFSyntax(),
//							3. Fixed issue in GetAssetPartComponentIndex() and 4. fixed issues
//							while composing from composed component.
// 11-26-14 SKP 1.0-a011 Implemented SerializeAsRDFSyntax() to get serialized content of part
// 11-26-14 SKP 1.0-a010 Implemented function for parsing string and creating parts for relation
// 11-21-14 ADC 1.0-a004 Support for IAssetLayerPart interface.
// 11-21-14 ADC 1.0-a002 Support for IAssetPart and IAssetTimePart interfaces.
// 11-19-14 ADC 1.0-a001 Framework in place for Asset Management Library.
//
// =================================================================================================
#endif  // AdobePrivate

#include "XMPAssetManagement/XMPAssetManagementFwdDeclarations_I.h"
#include "XMPAssetManagement/Interfaces/IAssetPart.h"

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
	
	static const char * standardComponentStrings[] = {
		""						/* kAssetPartComponentNone =			0	*/,
		kXMP_Part_All			/* kAssetPartComponentAll =				1	*/,
		kXMP_Part_Metadata		/* kAssetPartComponentMetadata =		2	*/,
		kXMP_Part_Content		/* kAssetPartComponentContent =			3	*/,
		kXMP_Part_Audio			/* kAssetPartComponentAudio =			4	*/,
		kXMP_Part_Visual		/* kAssetPartComponentVisual =			5	*/,
		kXMP_Part_Video			/* kAssetPartComponentVideo =			6	*/,
		kXMP_Part_Raster		/* kAssetPartComponentRaster =			7	*/,
		kXMP_Part_Vector		/* kAssetPartComponentVector =			8	*/,
		kXMP_Part_FormData		/* kAssetPartComponentFormData =		9	*/,
		kXMP_Part_FormTemplate	/* kAssetPartComponentFormTemplate =	10	*/,
		kXMP_Part_Annots		/* kAssetPartComponentFormAnnotations =	11	*/
	};

	static const char * partTypeStrings[] = {
		""						/* kAssetPartTypeGeneric =				0	*/,
		"time"					/* kAssetPartTypeTime =					1	*/,
		"artboard"				/* kAssetPartTypeArtboard =				2	*/,
		"layer"					/* kAssetPartTypeLayer =				3	*/,
		"page"					/* kAssetPartTypePage =					4	*/
	};

	class IAssetPart_I
		: public virtual IAssetPart
		, public virtual ISharedObject_I
		, public virtual IThreadSafe_I
	{
	public:
/*
		//!
		//! Get the type of the part.
		//! \return an object of type #eAssetPartType indicating the type of the part.
		//!
		virtual eAssetPartType APICALL GetType() const = 0;

		//!
		//! Get the component of the asset this part object is referring to.
		//! \return an object of type #eAssetPartComponent.
		//! \note In case the component is user defined then the value returned is
		//! kAssetPartComponentUserDefined.
		//!
		virtual eAssetPartComponent APICALL GetComponent() const = 0;

		//!
		//! Get the user defined component of the part.
		//! \return a shared pointer to const #NS_XMPCOMMON::IUTF8String object containing component string
		//! of the part of an asset.
		//! \note In case component is amongst one of the standard components recognized by XMP then an
		//! invalid shared pointer will be returned.
		//!
		virtual spcIUTF8String APICALL GetUserDefinedComponent_I() const = 0;

		//!
		//! Virtual copy constructor
		//! Clones the object creating an exact replica of the asset's part.
		//! \param[in] multiThreadingSupport a value of type #NS_XMPCOMMON::eMultiThreadingSupport controlling whether
		//! the new object created should be thread safe or not. By default new object created will inherit its setting from
		//! the object begin cloned.
		//! \return a shared pointer to a #AdobeXMPAM::IAssetPart object.
		//!
		virtual spIAssetPart_I APICALL Clone_I() const = 0;

		//!
		//! Add some descriptive information related to the asset type.
		//! \param[in] description pointer to a const NULL terminated char buffer containing description of
		//! the part of an asset.
		//! \note In case the description is a null pointer the description is removed.
		//!
		*/
		virtual void SetDescription( const char * description, sizet descriptionLength ) = 0;

		//!
		//! Get the description of the part of an asset.
		//! \return a shared pointer to const #NS_INT_XMPCOMMON::IUTF8String object containing description string
		//! of the part of an asset.
		//! \note In case no description was provided an invalid shared pointer is returned.
		//!
		virtual spcIUTF8String GetDescription() const = 0;

		//!
		//! Serializes the part value as a string to be used for RDF serialization
		//! \return a const object of type #NS_INT_XMPCOMMON::UTF8String_I.
		//!
		virtual spIUTF8String SerializeAsRDFSyntax() const = 0;

		virtual bool Equals ( const spcIAssetPart & other ) const = 0;

		// Factories to create the specific segments

		//!
		//! Creates a standard component of a Generic Asset Part
		//! \param[in] component a value of type #eAssetPartComponent indicating the component of the
		//! asset this object is referring to.
		//! \param[in] multiThreadingSupport a value of type #NS_XMPCOMMON::eMultiThreadingSupport controlling
		//! whether the new object created should be thread safe or not. By default new object created will
		//! inherit its setting from the #NS_XMPCOMMON::IConfigurationManager.
		//! \return a shared pointer to a #AdobeXMPAM::IAssetPart object.
		//! \attention  In case the component value is one which is not supported by the library then an error is thrown.
		//!
		static spIAssetPart_I CreateStandardGenericPart( eAssetPartComponent component);

		//!
		//! Creates a user defined component of a Generic Asset Part
		//! \param[in] component pointer to a const NULL terminated char buffer containing component string of
		//! the asset this string is referring to.
		//! \param[in] multiThreadingSupport a value of type #NS_XMPCOMMON::eMultiThreadingSupport controlling
		//! whether the new object created should be thread safe or not. By default new object created will
		//! inherit its setting from the #NS_XMPCOMMON::IConfigurationManager.
		//! \return a shared pointer to a #AdobeXMPAM::IAssetPart object.
		//! \attention In case the component is a NULL pointer then an error is thrown.
		//!
		static spIAssetPart_I CreateUserDefinedGenericPart( const char * component, sizet length);

		//!
		//! Parse the strings and creates corresponding asset.
		//! \param[in] rdfSyntax a pointer to NULL terminated char buffer containing the serialized
		//! representation of a part in RDF syntax.
		//! \param[in] multiThreadingSupport a value of type #NS_XMPCOMMON::eMultiThreadingSupport controlling
		//! whether the new object created should be thread safe or not.
		//! \return a shared pointer to a #NS_INT_XMPASSETMANAGEMENT::IAssetPart_I object.
		//! \attention  In case the str value is one which is not supported by the library than an error is thrown.
		//!
		static spIAssetPart_I ParseAndCreatePartFromRDFSyntax( const char * rdfSyntax, sizet rdfSyntaxLength);

		static uint32 GetInterfaceVersion() { return kInternalInterfaceVersionNumber; }
		virtual pIAssetPart APICALL GetActualIAssetPart() __NOTHROW__{ return this; }
		virtual pIAssetPart_I APICALL GetIAssetPart_I() __NOTHROW__{ return this; }

		virtual pvoid APICALL GetInterfacePointer(uint64 interfaceID, uint32 interfaceVersion);


	protected:
		virtual uint32 APICALL getType(pcIError_base & error) const __NOTHROW__;
		virtual uint32 APICALL getComponent(pcIError_base & error) const __NOTHROW__;
		virtual pcIUTF8String_base APICALL getUserDefinedComponent(pcIError_base & error) const __NOTHROW__;
		virtual pIAssetPart_base APICALL clone( pcIError_base & error ) const __NOTHROW__;
		pvoid APICALL GetInterfacePointerInternal(uint64 interfaceID, uint32 interfaceVersion, bool isTopLevel);
		virtual pvoid APICALL getInterfacePointer(uint64 interfaceID, uint32 interfaceVersion, pcIError_base & error) __NOTHROW__;



		//!
		//! protected Virtual Destructor
		//!
		virtual ~IAssetPart_I() {}



	#ifdef FRIEND_CLASS_DECLARATION
			FRIEND_CLASS_DECLARATION();
	#endif
			REQ_FRIEND_CLASS_DECLARATION();
	};
}
#if XMP_WinBuild
#pragma warning( pop )
#endif //#if XMP_WinBuild
#endif  // IAssetPart_I_h__
