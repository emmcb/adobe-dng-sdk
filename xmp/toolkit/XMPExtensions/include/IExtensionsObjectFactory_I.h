#ifndef IExtensionsObjectFactory_I_h__
#define IExtensionsObjectFactory_I_h__ 1

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
// 11-21-14 ADC 1.0-a002 Support for IAssetPart and IAssetTimePart interfaces.
// 11-19-14 ADC 1.0-a001 Framework in place for Asset Management Library.
//
// =================================================================================================
#endif  // AdobePrivate

#include "XMPAssetManagement/XMPAssetManagementFwdDeclarations_I.h"

#if GENERATE_XMPEXTENSIONS_DLL_LAYER_CODE

#include "XMPExtensions/include/IExtensionsObjectFactory.h"
#include "XMPAssetManagement/Interfaces/ITSingletonExtension.h"

namespace AdobeXMPAM_Int {
	using namespace AdobeXMPCommon;
	using namespace AdobeXMPCommon_Int;
	using namespace AdobeXMPAM;

	//!
	//! \brief Internal interface that represents a factory interface which allows to create
	//! various artifacts for Asset Management.
	//!
	//! \details Provides all the functions to create instances of various artifacts for Asset
	//! Management and return them as raw pointers to the clients. This is the interface through
	//! which client glue code of the library actually get access to all other interfaces.
	//! \attention Supports multi threading.
	//!
	class IExtensionsObjectFactory_I
		: public virtual IExtensionsObjectFactory
		, public virtual ITSingleton < IExtensionsObjectFactory_I >
	{
	public:
		static uint32 GetInterfaceVersion() { return kInternalInterfaceVersionNumber; }

		virtual pIExtensionsObjectFactory APICALL GetActualIExtensionsObjectFactory() __NOTHROW__{ return this; }
		virtual pIExtensionsObjectFactory_I APICALL GetIExtensionsObjectFactory_I() __NOTHROW__{ return this; }

			//virtual pvoid APICALL GetInterfacePointer(uint64 interfaceID, uint32 interfaceVersion);

	protected:

		//!
		//! protected Virtual Destructor
		//!
		//pvoid APICALL GetInterfacePointerInternal(uint64 interfaceID, uint32 interfaceVersion, bool isTopLevel);
		//virtual pvoid APICALL getInterfacePointer(uint64 interfaceID, uint32 interfaceVersion, pcIError_base & error) __NOTHROW__;
		virtual ~IExtensionsObjectFactory_I() {}


#ifdef FRIEND_CLASS_DECLARATION
		FRIEND_CLASS_DECLARATION();
#endif
		REQ_FRIEND_CLASS_DECLARATION();
	};



}

#endif  // GENERATE_XMPEXTENSIONS_DLL_LAYER_CODE

#endif  // IExtensionsObjectFactory_I_h__
