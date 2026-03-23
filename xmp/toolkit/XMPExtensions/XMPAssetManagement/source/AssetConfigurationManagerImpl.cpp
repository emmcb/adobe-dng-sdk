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
// SKP	Sunil Kishor Pathak
//
// mm-dd-yy who Description of changes, most recent first.
//
// 03-04-15 SKP 1.0-a075 Bug: 3947384, 3947379
// 02-19-15 SKP 1.0-a056 Fixed Allocator, ErrorNotification and threading handling in AssetConfigMgr
// 02-16-15 SKP 1.0-a053 Fixed XMPToolkit build break
// 02-12-15 SKP 1.0-a052 Fixed mac build break
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
#endif  // AdobePrivate


#define TXMP_STRING_TYPE std::string 
//#include "public\include\TXMPUtils.hpp"
// Ensure XMP templates are instantiated
#include "public/include/XMP.incl_cpp"

// Provide access to the API
#include "public/include/XMP.hpp"
#include "XMPCore/source/XMPDocOps-Utils.hpp"

#define IMPLEMENTATION_HEADERS_CAN_BE_INCLUDED 1
	#include "XMPAssetManagement/ImplHeaders/AssetConfigurationManagerImpl.h"
#undef IMPLEMENTATION_HEADERS_CAN_BE_INCLUDED

#include "XMPCommon/Interfaces/IErrorNotificationManager_I.h"

#include "XMPCommon/BaseClasses/TSingleton.h"


namespace NS_XMPCOMMON {
	using namespace NS_INT_XMPCOMMON;
	using namespace NS_INT_XMPASSETMANAGEMENT;

	template<>
	pIAssetConfigurationManager_I ITSingleton<IAssetConfigurationManager_I>::GetInstance() {
		return &TSingleton< AssetConfigurationManagerImpl >::Instance();
	}

	template< >
	void ITSingleton< IAssetConfigurationManager_I >::CreateInstance() {
		GetInstance();
	}

	template< >
	void ITSingleton< IAssetConfigurationManager_I >::DestroyInstance() {
		TSingleton< AssetConfigurationManagerImpl >::Destroy();
	}
}

namespace NS_INT_XMPASSETMANAGEMENT {
	//definition of class methods
	using namespace NS_INT_XMPASSETMANAGEMENT;
	using namespace NS_XMPCOMMON;
	using namespace NS_INT_XMPCOMMON;

	bool AssetConfigurationManagerImpl::RegisterAppName(const char * appName) {
		if (appName)
			_appName = appName;
		else
			_appName = "";

		return true;
	}

	std::string AssetConfigurationManagerImpl::GetAppName() const
	{
		return _appName;
	}

	bool AssetConfigurationManagerImpl::RegisterPrefix(const char * prefixPtr, const eXMPIDPrefix prefixType) {
		std::string prefix;
		if (prefixPtr)
			prefix = prefixPtr;

		switch (prefixType) {
		case IAssetConfigurationManager_v1::kAllIDPrefix:
			_prefixPolicy = IAssetConfigurationManager_v1::kAllIDPrefix;
			_prefix = prefix;

			//reset other id prefixes
			_docIDPrefix.erase();
			_instanceIDPrefix.erase();
			_orgDocIDPrefix.erase();
			_prefixPolicy = IAssetConfigurationManager_v1::kAllIDPrefix;
			break;

		case IAssetConfigurationManager_v1::kDocumentIDPrefix:
			_prefixPolicy |= IAssetConfigurationManager_v1::kDocumentIDPrefix;
			_docIDPrefix = prefix;
			break;

		case IAssetConfigurationManager_v1::kInstanceIDPrefix:
			_prefixPolicy |= IAssetConfigurationManager_v1::kInstanceIDPrefix;
			_instanceIDPrefix = prefix;
			break;

		case IAssetConfigurationManager_v1::kOriginalDocumentIDPrefix:
			_prefixPolicy |= IAssetConfigurationManager_v1::kOriginalDocumentIDPrefix;
			_orgDocIDPrefix = prefix;
			break;

		case IAssetConfigurationManager_v1::kIDPrefixMaxValue:
			NOTIFY_ERROR(IError_base::kDomainGeneral, IError_base::kGeneralCodeParametersNotAsExpected,
				"Invalid prefix value", IError_base::kSeverityOperationFatal, false, false);
			;
		default:
			;
		}
		return true;
	}

	std::string AssetConfigurationManagerImpl::GetPrefix(const eXMPIDPrefix prefixType) const
	{
		std::string prefix;
		switch (prefixType) {
		case IAssetConfigurationManager_v1::kOriginalDocumentIDPrefix:
			prefix = _orgDocIDPrefix;
			break;
		case IAssetConfigurationManager_v1::kInstanceIDPrefix:
			prefix = _instanceIDPrefix;
			break;
		case IAssetConfigurationManager_v1::kDocumentIDPrefix:
			prefix = _docIDPrefix;
			break;
			prefix = _prefix;
			break;
		default:
			;
		}
		return prefix.empty() ? _prefix : prefix;
	}
	bool AssetConfigurationManagerImpl::RegisterHistoryUpdatePolicy(const eHistoryUpdatePolicy historyUpdatePolicy, const eHistoryUpdateAction historyUpdateAction) {
		_historyUpdatePolicy = historyUpdatePolicy;
		_historyUpdateAction = historyUpdateAction;

		return true;
	}

	void AssetConfigurationManagerImpl::GetHistoryUpdatePolicyAndAction(eHistoryUpdatePolicy& historyUpdatePolicy, eHistoryUpdateAction& historyUpdateAction)
	{
		historyUpdatePolicy = _historyUpdatePolicy;
		historyUpdateAction = _historyUpdateAction;
	}

	std::string AssetConfigurationManagerImpl::GetDateTime(std::string& dateStr)
	{
		XMP_DateTime now;
		if ((_getDateTimeCallbackPtr != NULL) && (_getDateTimeCallbackPtr(&now) == true)) {
		}
		else {
			SXMPUtils::CurrentDateTime(&now);
			SXMPUtils::ConvertToLocalTime(&now);
		}

		SXMPUtils::ConvertFromDate(now, &dateStr);

		return dateStr;
	}
	bool AssetConfigurationManagerImpl::RegisterGetTimeCallback(GetDateTimeCallbackPtr getDateTimeCallbackPtr) {
		_getDateTimeCallbackPtr = getDateTimeCallbackPtr;
		return true;
	}
	 
	IAssetConfigurationManager_v1::GetDateTimeCallbackPtr AssetConfigurationManagerImpl::GetTimeCallbackPtr()
	{
		return _getDateTimeCallbackPtr;
	}

	bool AssetConfigurationManagerImpl::UpdateHistoryEntry(NS_XMPCOMMON::spIXMPArrayNode historyArrayNode, std::string ationStr, std::string changedStr, std::string swagent, std::string NewInstanceID, std::string dateStr)
	{
		if (historyArrayNode.get() == NULL)
			return false; //TODO@skpathak raise exception
		spIXMPStructureNode historyEntryNewNode;
		spIXMPSimpleNode node;

		if (dateStr.empty()) {
			ITSingleton<IAssetConfigurationManager_I>::GetInstance()->GetDateTime(dateStr);
		}

		switch (_historyUpdatePolicy) {
		case IAssetConfigurationManager_v1::kUpdateSkip:
			break;
		case IAssetConfigurationManager_v1::kUpdateOneTime:
			break;
		case IAssetConfigurationManager_v1::kUpdateEveryTime:
			if (NewInstanceID.empty()) {
				std::string NewInstanceIDPrefix = ITSingleton< IAssetConfigurationManager_I >::GetInstance()->GetPrefix(IAssetConfigurationManager_v1::kInstanceIDPrefix);
				SXMPDocOps::CreateID(&NewInstanceID, NewInstanceIDPrefix.c_str() /*"xmp.iid:"*/);
			}

			if (swagent.empty()) {
				swagent = ITSingleton< IAssetConfigurationManager_I >::GetInstance()->GetAppName();
			}

			historyEntryNewNode = NS_XMPCOMMON::IXMPStructureNode::CreateStructureNode(kXMP_NS_XMP_MM, "History");
			node = NS_XMPCOMMON::IXMPSimpleNode::CreateSimpleNode(kXMP_NS_XMP_ResourceEvent, "action", ationStr.empty() ? "saved" : ationStr.c_str());
			historyEntryNewNode->InsertNode(node);
			node = NS_XMPCOMMON::IXMPSimpleNode::CreateSimpleNode(kXMP_NS_XMP_ResourceEvent, "changed", changedStr.empty() ? "/metadata" : changedStr.c_str());
			historyEntryNewNode->InsertNode(node);
			if (!swagent.empty()) {
				node = NS_XMPCOMMON::IXMPSimpleNode::CreateSimpleNode(kXMP_NS_XMP_ResourceEvent, "softwareAgent", swagent.c_str());
				historyEntryNewNode->InsertNode(node);
			}
			node = NS_XMPCOMMON::IXMPSimpleNode::CreateSimpleNode(kXMP_NS_XMP_ResourceEvent, "instanceID", NewInstanceID.c_str());
			historyEntryNewNode->InsertNode(node);
			
			node = NS_XMPCOMMON::IXMPSimpleNode::CreateSimpleNode(kXMP_NS_XMP_ResourceEvent, "when", dateStr.c_str());
			historyEntryNewNode->InsertNode(node);

			historyArrayNode->AppendNode(historyEntryNewNode);

			break;
		default:
			;
		}
		
		return true;
	}
}

