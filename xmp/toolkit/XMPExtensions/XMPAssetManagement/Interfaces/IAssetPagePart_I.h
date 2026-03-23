#ifndef IAssetPagePart_I_h__
#define IAssetPagePart_I_h__ 1

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
//  SKP Sunil Kishor Pathak
//	ANS	Ankita Sharma
//
// mm-dd-yy who Description of changes, most recent first.
//
// 12-07-14 ADC 1.0-a041 Refactored Part Classes and Adding support for IAssetArtboardPart.
// 11-24-14 ANS 1.0-a007 Fixing build errors
// 11-23-14 SKP 1.0-a006 Support for IAssetPagePart interface.
//
// =================================================================================================
#endif  // AdobePrivate

#include "XMPAssetManagement/XMPAssetManagementFwdDeclarations_I.h"
#include "XMPAssetManagement/Interfaces/IAssetPagePart.h"

#include "XMPAssetManagement/Interfaces/IAssetPart_I.h"

#if XMP_WinBuild
	#pragma warning( push )
	#pragma warning( disable : 4250 )
#endif

namespace AdobeXMPAM_Int {
	using namespace AdobeXMPCommon;
	using namespace AdobeXMPCommon_Int;
	using namespace AdobeXMPAM;

	class IAssetPagePart_I
		: public virtual IAssetPagePart
		, public virtual IAssetPart_I
	{
	public:

		static uint32 GetInterfaceVersion() { return kInternalInterfaceVersionNumber; }

		virtual pIAssetPagePart APICALL GetActualIAssetPagePart() __NOTHROW__{ return this; }
		virtual pIAssetPagePart_I APICALL GetIAssetPagePart_I() __NOTHROW__{ return this; }

		virtual pvoid APICALL GetInterfacePointer(uint64 interfaceID, uint32 interfaceVersion);

		/*//!
		//! Get the name of the page.
		//! \return a shared pointer of a const #NS_INT_XMPCOMMON::IUTF8String_I object containing the name of the page.
		//! \note an invalid shared pointer is returned if no name is set or it is explicitly removed.
		//!
		virtual spcIUTF8String APICALL GetDescriptivePageNumber_I() const = 0;

		//!
		//! Virtual copy constructor
		//! Clones the object creating an exact replica of the asset's page part.
		//! \param[in] multiThreadingSupport a value of type #NS_XMPCOMMON::eMultiThreadingSupport controlling whether
		//! the new object created should be thread safe or not. By default new object created will inherit its setting from
		//! the object begin cloned.
		//! \return a shared pointer to a #AdobeXMPAM::IAssetPart object.
		//!
		*/
		//virtual spIAssetPart_I APICALL Clone_I() const = 0;
		
		// Factories functions to create various kind of objects

		//!
		//! Creates a Page based Asset Part.
		//! \param[in] component a value of type #eAssetPartComponent indicating the component of the
		//! asset this object is referring to.
		//! \param[in] pageNumber containing page number.
		//! \param[in] multiThreadingSupport a value of type #NS_XMPCOMMON::eMultiThreadingSupport controlling
		//! whether the new object created should be thread safe or not. By default new object created will
		//! inherit its setting from the #NS_XMPCOMMON::IConfigurationManager.
		//! \return a shared pointer to a #NS_INT_XMPASSETMANAGEMENT::IAssetPagePart_I object.
		//! \attention Error is thrown in case
		//!		-# the component value is one which is not supported by the library.
		//!		-# the pageNumber is less than or equal to zero.
		//! \note the number of the page is considered as unique and its client's responsibility to make sure the no it
		//! provides while creating a page part is unique.
		//!
		
		static spIAssetPagePart_I CreateStandardPagePart( eAssetPartComponent component, sizet pageNumber );

		//!
		//! Creates a Page based Asset Part.
		//! \param[ in ] component pointer to a const NULL terminated char buffer containing component string of
		//! the asset this string is referring to.
		//! \param[in] pageNumber containing page number.
		//! \param[in] multiThreadingSupport a value of type #NS_XMPCOMMON::eMultiThreadingSupport controlling
		//! whether the new object created should be thread safe or not. By default new object created will
		//! inherit its setting from the #NS_XMPCOMMON::IConfigurationManager.
		//! \return a shared pointer to a #NS_INT_XMPASSETMANAGEMENT::IAssetPagePart_I object.
		//! \attention Error is thrown in case
		//!		-# component is a NULL pointer.
		//!		-# the pageNumber is less than or equal to zero.
		//! \note the number of the page is considered as unique and its client's responsibility to make sure the no it
		//! provides while creating a page part is unique.
		//!
		static spIAssetPagePart_I CreateUserDefinedPagePart( const char * component, sizet length, sizet pageNumber );

		//!
		//! Creates a page based Asset Part from RDF syntax
		//! \param[in] component pointer to a const NULL terminated char buffer containing component string of
		//! the asset this string is referring to.
		//! \param[in] value pointer to a const NULL terminated char buffer containing value of the page part
		//! in rdf syntax.
		//! \param[in] multiThreadingSupport a value of type #NS_XMPCOMMON::eMultiThreadingSupport controlling
		//! whether the new object created should be thread safe or not.
		//! \return a shared pointer to a #NS_INT_XMPASSETMANAGEMENT::IAssetTimePart_I object.
		//! \attention Error is thrown in case the 
		//!		- In case the component is a NULL pointer,
		//!		- In case value is a null pointer or its contents are not supported.
		//!
		static spIAssetPagePart_I CreatePartFromRDFSyntax( const char * component, sizet componentLength, const char * value, sizet valueLength );

	protected:
		// REMOVE: copy all the safe functions from the external interface and mark them as non pure.
		pvoid APICALL GetInterfacePointerInternal(uint64 interfaceID, uint32 interfaceVersion, bool isTopLevel);
		virtual pvoid APICALL getInterfacePointer(uint64 interfaceID, uint32 interfaceVersion, pcIError_base & error) __NOTHROW__;

		virtual pcIUTF8String_base APICALL getDescriptivePageNumber(pcIError_base & error) const __NOTHROW__;
		virtual pIAssetPart_base APICALL clone(pcIError_base & error) const __NOTHROW__;
		//virtual spIAssetPart Clone() const;

		//!
		//! protected Virtual Destructor
		//!
		virtual ~IAssetPagePart_I() {}
	#ifdef FRIEND_CLASS_DECLARATION
			FRIEND_CLASS_DECLARATION();
	#endif
			REQ_FRIEND_CLASS_DECLARATION();
		
	};

}

#if XMP_WinBuild
	#pragma warning( pop )
#endif

#endif  // IAssetPagePart_I_h__
