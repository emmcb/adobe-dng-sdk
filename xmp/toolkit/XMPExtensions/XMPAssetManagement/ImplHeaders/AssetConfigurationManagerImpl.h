#ifndef AssetConfigurationManagerImpl_h__
#define AssetConfigurationManagerImpl_h__ 1

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

#include "XMPAssetManagement/Interfaces/IAssetConfigurationManager_I.h"

#include "XMPCommon/BaseClasses/Object.h"
#include "XMPCommon/ImplHeaders/ConfigurationManagerImpl.h"
#include <string>

#if XMP_WinBuild
#pragma warning( push )
#pragma warning( disable : 4250 )
#endif


namespace NS_INT_XMPASSETMANAGEMENT {
	using namespace NS_INT_XMPASSETMANAGEMENT;
	using namespace NS_XMPCOMMON;
	using namespace NS_INT_XMPCOMMON;
	//!
	//! \brief internal interface that represents configuration settings controllable by the client.
	//! \details Provides functions through which client can plug in its own memory allocators, error notifiers.
	//! \attention Not Thread Safe as this functionality is generally used at the initialization phase.
	//!
	class AssetConfigurationManagerImpl
		: public virtual IAssetConfigurationManager_I
		, public virtual ConfigurationManagerImpl
	{
	public:
		AssetConfigurationManagerImpl() { }
		virtual ~AssetConfigurationManagerImpl() __NOTHROW__;

		virtual bool RegisterAppName(const char * appName);
		virtual bool RegisterPrefix(const char * docIDPrefix, const eXMPIDPrefix);
		virtual bool RegisterHistoryUpdatePolicy( const eHistoryUpdatePolicy historyUpdatePolicy, const eHistoryUpdateAction HistoryUpdateAction);
		virtual bool RegisterGetTimeCallback( GetDateTimeCallbackPtr );

		std::string GetAppName(void) const;
		std::string GetPrefix(const eXMPIDPrefix) const;
		GetDateTimeCallbackPtr GetTimeCallbackPtr(void);
		std::string GetDateTime(std::string& datetime);
		void GetHistoryUpdatePolicyAndAction(eHistoryUpdatePolicy& historyUpdatePolicy, eHistoryUpdateAction& historyUpdateAction);
		bool UpdateHistoryEntry(NS_XMPCOMMON::spIXMPArrayNode historyNode, std::string action = "", std::string changed = "", std::string swagent = "", std::string instanceid = "", std::string when = "");

	protected:
		//virtual UInt32 RegisterClientAllocator(pIClientAllocator_base clientAllocator, pcIError_base & error) __NOTHROW__;
		//virtual UInt32 RegisterClientErrorNotifier(pIClientErrorNotifier_base clientErrorNotifier, pcIError_base & error) __NOTHROW__;
		//virtual UInt32 DisableMultiThreading(pcIError_base & error) __NOTHROW__;
		//virtual void AcquireSharedOwnership() const __NOTHROW__{};
		//virtual void ReleaseSharedOwnership() const __NOTHROW__{};
		
	private:
		std::string _appName;
		std::string _prefix;
		std::string _docIDPrefix;
		std::string _orgDocIDPrefix;
		std::string _instanceIDPrefix;
		UInt32		_prefixPolicy;
		eHistoryUpdatePolicy _historyUpdatePolicy;
		eHistoryUpdateAction _historyUpdateAction;
		eXMPIDPrefix		 _IDPrefix;
		GetDateTimeCallbackPtr _getDateTimeCallbackPtr;
	protected:
        #ifndef FRIEND_CLASS_DECLARATION
            #define FRIEND_CLASS_DECLARATION() 
        #endif
		    FRIEND_CLASS_DECLARATION();
	};

	inline AssetConfigurationManagerImpl::~AssetConfigurationManagerImpl() __NOTHROW__{}
}

#endif // AssetConfigurationManagerImpl_h__
