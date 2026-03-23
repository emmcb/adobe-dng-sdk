#ifndef IAssetTimePart_I_h__
#define IAssetTimePart_I_h__ 1

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
//
// mm-dd-yy who Description of changes, most recent first.
//
// 12-07-14 ADC 1.0-a041 Refactored Part Classes and Adding support for IAssetArtboardPart.
// 11-21-14 ADC 1.0-a004 Support for IAssetLayerPart interface.
// 11-21-14 ADC 1.0-a002 Support for IAssetPart and IAssetTimePart interfaces.
//
// =================================================================================================
#endif  // AdobePrivate

#include "XMPAssetManagement/XMPAssetManagementFwdDeclarations_I.h"
#include "XMPAssetManagement/Interfaces/IAssetTimePart.h"

#include "XMPAssetManagement/Interfaces/IAssetPart_I.h"

#if XMP_WinBuild
	#pragma warning( push )
	#pragma warning( disable : 4250 )
#endif

namespace AdobeXMPAM_Int {
	using namespace AdobeXMPCommon;
	using namespace AdobeXMPCommon_Int;
	using namespace AdobeXMPCommon;

	class IAssetTimePart_I
		: public virtual IAssetTimePart
		, public virtual IAssetPart_I
	{
	public:
		/*
		//!
		//! Virtual copy constructor
		//! Clones the object creating an exact replica of the asset's Time part.
		//! \param[in] multiThreadingSupport a value of type #NS_XMPCOMMON::eMultiThreadingSupport controlling whether
		//! the new object created should be thread safe or not. By default new object created will inherit its setting from
		//! the object begin cloned.
		//! \return a shared pointer to a #AdobeXMPAM::IAssetPart object.
		//!
		virtual spIAssetPart_I APICALL Clone_I() const = 0;
		*/
		//!
		//! Creates a standard component of a Time based Asset Part
		//! \param[in] component a value of type #eAssetPartComponent indicating the component of the
		//! asset this object is referring to.
		//! \param[in] multiThreadingSupport a value of type #NS_XMPCOMMON::eMultiThreadingSupport controlling
		//! whether the new object created should be thread safe or not. By default new object created will
		//! inherit its setting from the #NS_XMPCOMMON::IConfigurationManager.
		//! \return a shared pointer to a #NS_INT_XMPASSETMANAGEMENT::IAssetTimePart_I object.
		//! \attention  In case the component value is one which is not supported by the library then an error is thrown.
		//!
		
		static uint32 GetInterfaceVersion() { return kInternalInterfaceVersionNumber; }

		virtual pIAssetTimePart APICALL GetActualIAssetTimePart() __NOTHROW__{ return this; }
		virtual pIAssetTimePart_I APICALL GetIAssetTimePart_I() __NOTHROW__{ return this; }

		virtual pvoid APICALL GetInterfacePointer(uint64 interfaceID, uint32 interfaceVersion);
		
		static spIAssetTimePart_I CreateStandardTimePart(eAssetPartComponent component);

		//!
		//! Creates a user defined component of a Time based Asset Part
		//! \param[in] component pointer to a const NULL terminated char buffer containing component string of
		//! the asset this string is referring to.
		//! \param[in] multiThreadingSupport a value of type #NS_XMPCOMMON::eMultiThreadingSupport controlling
		//! whether the new object created should be thread safe or not. By default new object created will
		//! inherit its setting from the #NS_XMPCOMMON::IConfigurationManager.
		//! \return a shared pointer to a #NS_INT_XMPASSETMANAGEMENT::IAssetTimePart_I object.
		//! \attention In case the component is a NULL pointer then an error is thrown.
		//!
		static spIAssetTimePart_I CreateUserDefinedTimePart(const char * component, sizet componentLength);

		//!
		//! Creates a Time based Asset Part from RDF syntax
		//! \param[in] component pointer to a const NULL terminated char buffer containing component string of
		//! the asset this string is referring to.
		//! \param[in] value pointer to a const NULL terminated char buffer containing value of the time part
		//! in rdf syntax.
		//! \param[in] multiThreadingSupport a value of type #NS_XMPCOMMON::eMultiThreadingSupport controlling
		//! whether the new object created should be thread safe or not.
		//! \return a shared pointer to a #NS_INT_XMPASSETMANAGEMENT::IAssetTimePart_I object.
		//! \attention Error is thrown in case the 
		//!		- In case the component is a NULL pointer,
		//!		- In case value is a null pointer or its contents are not supported.
		//!
		static spIAssetTimePart_I CreatePartFromRDFSyntax( const char * component, sizet componentLength, const char * value, sizet valueLength );


	protected:
		pvoid APICALL GetInterfacePointerInternal(uint64 interfaceID, uint32 interfaceVersion, bool isTopLevel);
		virtual pvoid APICALL getInterfacePointer(uint64 interfaceID, uint32 interfaceVersion, pcIError_base & error) __NOTHROW__;
		virtual void APICALL setFrameRate(uint32 typeOfFrameRate, uint64 numerator, uint64 denominator, pcIError_base & error) __NOTHROW__;
		virtual pIAssetPart_base APICALL clone(pcIError_base & error) const __NOTHROW__;
		
		//virtual spIAssetPart APICALL Clone() const;
		//!
		//! protected Virtual Destructor
		//!
		virtual ~IAssetTimePart_I() {}

	#ifdef FRIEND_CLASS_DECLARATION
			FRIEND_CLASS_DECLARATION();
	#endif
			REQ_FRIEND_CLASS_DECLARATION();
	};

}

#if XMP_WinBuild
	#pragma warning( pop )
#endif

#endif  // IAssetTimePart_I_h__
