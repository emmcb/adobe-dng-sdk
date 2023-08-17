#ifndef IAssetConfigurationManager_I_h__
#define IAssetConfigurationManager_I_h__ 1

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
//  SKP	Sunil Kishor Pathak
//
// mm-dd-yy who Description of changes, most recent first.
//
// 02-19-15 SKP 1.0-a056 Fixed Allocator, ErrorNotification and threading handling in AssetConfigMgr
// 02-11-15 SKP 1.0-a049 Added initial support for client strategies. Using these strategies client
//						 can set prefix, AppName, history update policy and it's own datetime
//						 function which will be used to get time for ModifyDate, Metadata date etc.
//
// =================================================================================================
#endif	// AdobePrivate

#include "XMPAssetManagement/XMPAssetManagementFwdDeclarations_I.h"
#include "XMPCore/XMPCoreFwdDeclarations.h"
//#include "XMPCommon/Interfaces/BaseInterfaces/ISharedOwnership.h"
#include "XMPAssetManagement/Interfaces/IAssetConfigurationManager.h"
#include "XMPCommon/Interfaces/BaseInterfaces/ITSingleton.h"
#include "XMPCommon/Interfaces/IConfigurationManager_I.h"
#include <string>


namespace NS_INT_XMPASSETMANAGEMENT {
	using namespace NS_XMPCOMMON;
	using namespace NS_INT_XMPCOMMON;
	using namespace NS_XMPASSETMANAGEMENT;


	#if XMP_WinBuild
		#pragma warning( push )
		#pragma warning( disable : 4250 )
	#endif

	//!
	//! \brief internal interface that represents configuration settings controllable by the client.
	//! \details Provides functions through which client can plug in its own memory allocators, error notifiers.
	//! \attention Not Thread Safe as this functionality is generally used at the initialization phase.
	//!
	class IAssetConfigurationManager_I
		: public virtual IAssetConfigurationManager_latest
		, public virtual IConfigurationManager_I
		, public virtual ITSingleton< IAssetConfigurationManager_I >
	{
	public:
		//!
		//! Allowed the client to set the app name which will be used for xmp:creator and stEvt:SoftwareAgent
		//! \param const char *[in]	AppName in const char *
		//! \return true if success otherwise false.
		//!
		virtual bool RegisterAppName(const char * appName) = 0;

		//!
		//! Allows the client to set prefix which will be used as prefix for xmpMM:OriginalDocumentID and xmpMM:DocumentID
		//! \param const char*	[in]	DocumentID prefix in const char*
		//! \param eXMPIDPrefix [in]	Type of the prefix
		//! \return true if success otherwise false.
		//! \attention Error is thrown in case
		//!		-# Prefix is not supported
		//!
		virtual bool RegisterPrefix(const char * docIDPrefix, const eXMPIDPrefix) = 0;
		
		//!
		//! Allows the client to register HistoryUpdatePolicy. By default for every composition a history entry will be made
		//! \param eHistoryUpdatePolicy Policy to make event history
		//! \param eHistoryUpdateAction action for which history entry will make (based on the eHistoryUpdatePolicy)
		//! \return a value of bool type; true means successful and false otherwise.
		//!
		virtual bool RegisterHistoryUpdatePolicy(const eHistoryUpdatePolicy eHistoryUpdatePolicy, const eHistoryUpdateAction HistoryUpdateAction) = 0;
		
		//!
		//! Allows the client to set set call back for time which will be used for stEvt:When, xmp:ModifyDate etc. Default is current time
		//! \param GetTimeCallbackPtr
		//! \return true if success otherwise false.
		//!
		virtual bool RegisterGetTimeCallback( GetDateTimeCallbackPtr ) = 0;

	protected:
		//! \cond XMP_INTERNAL_DOCUMENTATION
		virtual UInt32 RegisterAppName(const char * appName, pcIError_base & error) __NOTHROW__;
		virtual UInt32 RegisterPrefix(const char * docIDPrefix, const eXMPIDPrefix, pcIError_base & error) __NOTHROW__;
		virtual UInt32 RegisterHistoryUpdatePolicy(const eHistoryUpdatePolicy historyUpdatePolicy, const eHistoryUpdateAction HistoryUpdateAction, pcIError_base & error) __NOTHROW__;
		virtual UInt32 RegisterGetTimeCallback(GetDateTimeCallbackPtr, pcIError_base & error) __NOTHROW__;
		//! \endcond

	protected:
		//!
		//! protected virtual destructor
		//!
		virtual ~IAssetConfigurationManager_I() __NOTHROW__ = 0;


	public:
		virtual std::string GetAppName(void) const = 0;
		virtual std::string GetPrefix(const eXMPIDPrefix) const = 0;
		virtual GetDateTimeCallbackPtr GetTimeCallbackPtr(void) = 0;
		virtual std::string GetDateTime(std::string& datetime) = 0;
		virtual void GetHistoryUpdatePolicyAndAction(eHistoryUpdatePolicy& historyUpdatePolicy, eHistoryUpdateAction& historyUpdateAction) = 0;
		virtual bool UpdateHistoryEntry(NS_XMPCOMMON::spIXMPArrayNode historyNode, std::string action = "", std::string changed = "", std::string swagent = "", std::string instanceid = "", std::string when = "") = 0;

    #ifndef FRIEND_CLASS_DECLARATION
        #define FRIEND_CLASS_DECLARATION() 
    #endif
		FRIEND_CLASS_DECLARATION();
	};

	#if XMP_WinBuild
		#pragma warning( pop )
	#endif
	
	inline IAssetConfigurationManager_I::~IAssetConfigurationManager_I() __NOTHROW__{}
}

#endif // IAssetConfigurationManager_I_h__
