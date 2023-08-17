#ifndef IAssetArtboardPart_I_h__
#define IAssetArtboardPart_I_h__ 1

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
//
// =================================================================================================
#endif  // AdobePrivate

#include "XMPAssetManagement/XMPAssetManagementFwdDeclarations_I.h"
#include "XMPAssetManagement/Interfaces/IAssetArtboardPart.h"
#include "XMPAssetManagement/Interfaces/IAssetPart_I.h"

#if XMP_WinBuild
	#pragma warning( push )
	#pragma warning( disable : 4250 )
#endif

namespace AdobeXMPAM_Int {
	using namespace AdobeXMPCommon;
	using namespace AdobeXMPCommon_Int;
	using namespace AdobeXMPAM;

	class IAssetArtboardPart_I
		: public virtual IAssetArtboardPart
		, public virtual IAssetPart_I
	{
	public:
		/*
		//!
		//! Get the id of the art board.
		//! \return a shared pointer of a const #NS_INT_XMPCOMMON::IUTF8String_I object containing id of the art board.
		//!
		virtual spcIUTF8String APICALL GetID_I() const = 0;

		//!
		//! Set the name of the art board.
		//! \param[in] name a pointer to a const NULL terminated char buffer containing name of the art board.
		//! \note providing NULL means client wants to remove the name altogether from the art board part.
		//!
		virtual void APICALL SetName(const char * name, sizet length) = 0;

		//!
		//! Get the name of the art board.
		//! \return a shared pointer of a const #NS_INT_XMPCOMMON::IUTF8String_I object containing the name of the art board.
		//! \note an invalid shared pointer is returned if no name is set or it is explicitly removed.
		//!
		virtual spcIUTF8String APICALL GetName_I() const = 0;

		//!
		//! Virtual copy constructor
		//! Clones the object creating an exact replica of the asset's Artboard part.
		//! \param[in] multiThreadingSupport a value of type #NS_XMPCOMMON::eMultiThreadingSupport controlling whether
		//! the new object created should be thread safe or not. By default new object created will inherit its setting from
		//! the object begin cloned.
		//! \return a shared pointer to a #AdobeXMPAM::IAssetPart object.
		//!
		virtual spIAssetPart_I APICALL Clone_I() const = 0;

		// Factories functions to create various kind of objects

		//!
		//! Creates an Art board based Asset Part.
		//! \param[in] component a value of type #eAssetPartComponent indicating the component of the
		//! asset this object is referring to.
		//! \param[in] id pointer to a const NULL terminated char buffer containing containing id of the art board.
		//! \param[in] multiThreadingSupport a value of type #NS_XMPCOMMON::eMultiThreadingSupport controlling
		//! whether the new object created should be thread safe or not. By default new object created will
		//! inherit its setting from the #NS_XMPCOMMON::IConfigurationManager.
		//! \return a shared pointer to a #NS_INT_XMPASSETMANAGEMENT::IAssetArtboardPart_I object.
		//! \attention Error is thrown in case
		//!		-# the component value is one which is not supported by the library.
		//!		-# id is a NULL pointer or its contents are empty.
		//! \note the id of the art board is considered as unique and its client's responsibility to make sure the ids it
		//! provides while creating a art board part is unique.
		//!
		*/

		static uint32 GetInterfaceVersion() { return kInternalInterfaceVersionNumber; }

		virtual pIAssetArtboardPart APICALL GetActualIAssetArtboardPart() __NOTHROW__{ return this; }
		virtual pIAssetArtboardPart_I APICALL GetIAssetArtboardPart_I() __NOTHROW__{ return this; }

		virtual pvoid APICALL GetInterfacePointer(uint64 interfaceID, uint32 interfaceVersion);

		static spIAssetArtboardPart_I CreateStandardArtboardPart( eAssetPartComponent component, const char * id, sizet idLength );

		//!
		//! Creates an Art board based Asset Part.
		//! \param[ in ] component pointer to a const NULL terminated char buffer containing component string of
		//! the asset this string is referring to.
		//! \param[in] id pointer to a const NULL terminated char buffer containing containing id of the art board.
		//! \param[in] multiThreadingSupport a value of type #NS_XMPCOMMON::eMultiThreadingSupport controlling
		//! whether the new object created should be thread safe or not. By default new object created will
		//! inherit its setting from the #NS_XMPCOMMON::IConfigurationManager.
		//! \return a shared pointer to a #NS_INT_XMPASSETMANAGEMENT::IAssetArtboardPart_I object.
		//! \attention Error is thrown in case
		//!		-# component is a NULL pointer.
		//!		-# id is a NULL pointer or its contents are empty.
		//! \note the id of the art board is considered as unique and its client's responsibility to make sure the ids it
		//! provides while creating a art board part is unique.
		//!
		static spIAssetArtboardPart_I CreateUserDefinedArtboardPart( const char * component, sizet componentLength, const char * id, sizet idLength );

	protected:
		pvoid APICALL GetInterfacePointerInternal(uint64 interfaceID, uint32 interfaceVersion, bool isTopLevel);
		virtual pvoid APICALL getInterfacePointer(uint64 interfaceID, uint32 interfaceVersion, pcIError_base & error) __NOTHROW__;

		virtual pcIUTF8String_base APICALL getID(pcIError_base & error) const __NOTHROW__;
		virtual void APICALL setName(const char * name, sizet length, pcIError_base & error) __NOTHROW__;
		virtual pcIUTF8String_base APICALL getName(pcIError_base & error) const __NOTHROW__;
		virtual pIAssetPart_base APICALL clone(pcIError_base & error) const __NOTHROW__;


		/*virtual spcIUTF8String APICALL GetID() const;
		virtual spcIUTF8String APICALL GetName() const;
		virtual spIAssetPart APICALL Clone() const;
		*/
		//!
		//! protected Virtual Destructor
		//!
		virtual ~IAssetArtboardPart_I() {}
		#ifdef FRIEND_CLASS_DECLARATION
				FRIEND_CLASS_DECLARATION();
		#endif
				REQ_FRIEND_CLASS_DECLARATION();
	};
}

#if XMP_WinBuild
	#pragma warning( pop )
#endif

#endif  // IAssetArtboardPart_I_h__
