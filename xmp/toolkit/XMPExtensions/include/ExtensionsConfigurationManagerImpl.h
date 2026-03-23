#ifndef ExtensionsConfigurationManagerImpl_h__
#define ExtensionsConfigurationManagerImpl_h__ 1

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
// 02-12-15 SKP 1.0-a051 Implemented history policy support. After this support during composition
//						 history entry will be updated based oneHistoryUpdatePolicy and 
//						 eHistoryUpdateAction (IAssetConfigManager.h). Currently eHistoryUpdatePolicy
//						 ::kUpdateEveryTime, eHistoryUpdatePolicy::kUpdateIgnore policy and action 
//						 eHistoryUpdateAction::kHistoryActionAll is supported.
// 02-11-15 SKP 1.0-a050 Implementing support for XMPDateTime, History update policy and action
// 02-11-15 SKP 1.0-a049 Added initial support for client strategies. Using these strategies client
//						 can set prefix, AppName, history update policy and it's own datetime
//						 function which will be used to get time for ModifyDate, Metadata date etc.
//
// =================================================================================================
#endif	// AdobePrivate

#include "XMPAssetManagement/XMPAssetManagementFwdDeclarations_I.h"
//#include "XMPCore/XMPCoreFwdDeclarations.h"
//#include "XMPCommon/Interfaces/BaseInterfaces/ISharedOwnership.h"
//#include "XMPCommon/Interfaces/IConfigurationManager.h"
#if !(IMPLEMENTATION_HEADERS_CAN_BE_INCLUDED)
#error "Not adhering to design constraints"
// this file should only be included from its own cpp file
#else
#include "XMPCommon/ImplHeaders/ConfigurationManagerImpl.h"
#endif

#include "XMPExtensions/include/IExtensionsConfigurationManager_I.h"
#include "XMPCommon/BaseClasses/MemoryManagedObject.h"
#include "XMPCommon/ImplHeaders/ConfigurationManagerImpl.h"
#include <string>

#if XMP_WinBuild
#pragma warning( push )
#pragma warning( disable : 4250 )
#endif


namespace AdobeXMPAM_Int {
	using namespace AdobeXMPAM_Int;
	using namespace AdobeXMPCommon;
	using namespace AdobeXMPCommon_Int;
	//!
	//! \brief internal interface that represents configuration settings controllable by the client.
	//! \details Provides functions through which client can plug in its own memory allocators, error notifiers.
	//! \attention Not Thread Safe as this functionality is generally used at the initialization phase.
	//!
	class ExtensionsConfigurationManagerImpl
		: public virtual IExtensionsConfigurationManager_I
		, public virtual ConfigurationManagerImpl
	{
	public:
		ExtensionsConfigurationManagerImpl() { }
		virtual ~ExtensionsConfigurationManagerImpl() __NOTHROW__;

		virtual bool APICALL RegisterAppName(const char * appName, sizet appNameLength);
		virtual bool APICALL RegisterPrefix(const char * docIDPrefix, sizet docIDLength, const eXMPIDPrefix);
		virtual bool APICALL RegisterHistoryUpdatePolicy(const eHistoryUpdatePolicy historyUpdatePolicy, const eHistoryUpdateAction HistoryUpdateAction);
		virtual bool APICALL RegisterGetTimeCallback(GetDateTimeCallbackPtr);
		virtual bool APICALL RegisterExtensionHandler_I(const char * extensionNameSpace, sizet extensionNameSpaceLen, const char * extensionName, sizet extensionNameLen, pIExtensionHandler customHandler);
		virtual bool APICALL RegisterExtensionHandler(const char * extensionNameSpace, sizet  extensionNameSpaceLen, const char * extensionName, sizet extensionNameLen, pIExtensionHandler customHandler);

		std::string GetAppName(void) const;
		std::string GetPrefix(const eXMPIDPrefix) const;
		GetDateTimeCallbackPtr GetTimeCallbackPtr(void);
		std::string GetDateTime(std::string& datetime);
		void GetHistoryUpdatePolicyAndAction(eHistoryUpdatePolicy& historyUpdatePolicy, eHistoryUpdateAction& historyUpdateAction);
		bool UpdateHistoryEntry(spIArrayNode historyNode, std::string action = "", std::string changed = "", std::string swagent = "", std::string instanceid = "", std::string when = "");

	protected:
		//virtual uint32 RegisterClientAllocator(pIClientAllocator_base clientAllocator, pcIError_base & error) __NOTHROW__;
		//virtual uint32 RegisterClientErrorNotifier(pIClientErrorNotifier_base clientErrorNotifier, pcIError_base & error) __NOTHROW__;
		//virtual uint32 DisableMultiThreading(pcIError_base & error) __NOTHROW__;
		//virtual void AcquireSharedOwnership() const __NOTHROW__{};
		//virtual void ReleaseSharedOwnership() const __NOTHROW__{};

	private:
		std::string _appName;
		std::string _prefix;
		std::string _docIDPrefix;
		std::string _orgDocIDPrefix;
		std::string _instanceIDPrefix;
		uint32		_prefixPolicy;
		eHistoryUpdatePolicy _historyUpdatePolicy;
		eHistoryUpdateAction _historyUpdateAction;
		//eXMPIDPrefix		 _IDPrefix;
		GetDateTimeCallbackPtr _getDateTimeCallbackPtr;
	protected:

#ifdef FRIEND_CLASS_DECLARATION
		FRIEND_CLASS_DECLARATION();
#endif
		REQ_FRIEND_CLASS_DECLARATION();
	};

	inline ExtensionsConfigurationManagerImpl::~ExtensionsConfigurationManagerImpl() __NOTHROW__{}
}

#endif // ExtensionsConfigurationManagerImpl_h__
