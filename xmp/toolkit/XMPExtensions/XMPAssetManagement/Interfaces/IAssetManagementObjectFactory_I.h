#ifndef IAssetManagementObjectFactory_I_h__
#define IAssetManagementObjectFactory_I_h__ 1

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

#include "XMPAssetManagement/Interfaces/IAssetManagementObjectFactory.h"
#include "XMPCommon/Interfaces/BaseInterfaces/ITSingleton.h"

namespace NS_INT_XMPASSETMANAGEMENT {
	using namespace NS_XMPCOMMON;
	using namespace NS_INT_XMPCOMMON;
	using namespace NS_XMPASSETMANAGEMENT;

	//!
	//! \brief Internal interface that represents a factory interface which allows to create
	//! various artifacts for Asset Management.
	//!
	//! \details Provides all the functions to create instances of various artifacts for Asset
	//! Management and return them as raw pointers to the clients. This is the interface through
	//! which client glue code of the library actually get access to all other interfaces.
	//! \attention Supports multi threading.
	//!
	class IAssetManagementObjectFactory_I
		: public virtual IAssetManagementObjectFactory_latest
		, public virtual ITSingleton< IAssetManagementObjectFactory_I >
	{
	public:

	protected:

		//!
		//! protected Virtual Destructor
		//!
		virtual ~IAssetManagementObjectFactory_I() __NOTHROW__ = 0;

	#ifndef FRIEND_CLASS_DECLARATION
		#define FRIEND_CLASS_DECLARATION() 
	#endif
		FRIEND_CLASS_DECLARATION();
	};

	inline IAssetManagementObjectFactory_I::~IAssetManagementObjectFactory_I() __NOTHROW__ { }

}

#endif  // GENERATE_XMPEXTENSIONS_DLL_LAYER_CODE

#endif  // IAssetManagementObjectFactory_I_h__
