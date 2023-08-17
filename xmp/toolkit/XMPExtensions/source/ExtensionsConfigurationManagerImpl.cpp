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

#include <string>
#define TXMP_STRING_TYPE std::string 
// Ensure XMP templates are instantiated

// Provide access to the API
#include "public/include/XMP.hpp"  
#include "public/include/XMP.incl_cpp"

#include "XMPCore/Interfaces/ISimpleNode.h"
#include "XMPCore/Interfaces/IArrayNode.h"
#include "XMPCore/Interfaces/IStructureNode.h"

#define IMPLEMENTATION_HEADERS_CAN_BE_INCLUDED 1
#include "XMPExtensions/include//ExtensionsConfigurationManagerImpl.h"
#undef IMPLEMENTATION_HEADERS_CAN_BE_INCLUDED

#include "XMPCommon/Interfaces/IErrorNotifier_I.h"
#include "XMPAssetManagement/Interfaces/TSingletonExtension.h"
#include "XMPCommon/Interfaces/IError_I.h"
#include "XMPCommon/XMPCommonErrorCodes.h"
#include "XMPCommon/Interfaces/IError_I.h"
#include "XMPCommon/Utilities/TSmartPointers_I.h"
#include "XMPCommon/Interfaces/IErrorNotifier_I.h"
#include "XMPExtensions/XMPMarker/Interfaces/IExtensionHandlerRegistry_I.h"
#include "XMPCommon/BaseClasses/TSingleton.h"

namespace AdobeXMPAM {
	using namespace AdobeXMPCommon_Int;
	using namespace AdobeXMPAM_Int;

	template<>
	pIExtensionsConfigurationManager_I ITSingleton<IExtensionsConfigurationManager_I>::GetInstance() {
		return &TSingleton< ExtensionsConfigurationManagerImpl >::Instance();
	}

	template< >
	void ITSingleton< IExtensionsConfigurationManager_I >::CreateInstance() {
		GetInstance();
	}

	template< >
	void ITSingleton< IExtensionsConfigurationManager_I >::DestroyInstance() {
		TSingleton< ExtensionsConfigurationManagerImpl >::Destroy();
	}
}

namespace AdobeXMPAM_Int{
	//definition of class methods
	using namespace AdobeXMPAM_Int;
	using namespace AdobeXMPCommon;
	using namespace AdobeXMPCommon_Int;

	bool APICALL ExtensionsConfigurationManagerImpl::RegisterAppName(const char * appName, sizet appNameLength) {
		if (appName)
			_appName = appName;
		else
			_appName = "";

		return true;
	}

	std::string ExtensionsConfigurationManagerImpl::GetAppName() const
	{
		return _appName;
	}

	bool APICALL ExtensionsConfigurationManagerImpl::RegisterPrefix(const char * prefixPtr, sizet prefixLength, const eXMPIDPrefix prefixType) {
		std::string prefix;
		if (prefixPtr)
			prefix = prefixPtr;

		switch (prefixType) {
		case IExtensionsConfigurationManager_v1::kAllIDPrefix:
			_prefixPolicy = IExtensionsConfigurationManager_v1::kAllIDPrefix;
			_prefix = prefix;

			//reset other id prefixes
			_docIDPrefix.erase();
			_instanceIDPrefix.erase();
			_orgDocIDPrefix.erase();
			_prefixPolicy = IExtensionsConfigurationManager_v1::kAllIDPrefix;
			break;

		case IExtensionsConfigurationManager_v1::kDocumentIDPrefix:
			_prefixPolicy |= IExtensionsConfigurationManager_v1::kDocumentIDPrefix;
			_docIDPrefix = prefix;
			break;

		case IExtensionsConfigurationManager_v1::kInstanceIDPrefix:
			_prefixPolicy |= IExtensionsConfigurationManager_v1::kInstanceIDPrefix;
			_instanceIDPrefix = prefix;
			break;

		case IExtensionsConfigurationManager_v1::kOriginalDocumentIDPrefix:
			_prefixPolicy |= IExtensionsConfigurationManager_v1::kOriginalDocumentIDPrefix;
			_orgDocIDPrefix = prefix;
			break;

		case IExtensionsConfigurationManager_v1::kIDPrefixMaxValue:
			NOTIFY_ERROR(IError::kEDGeneral, kGECParametersNotAsExpected,
				"Invalid prefix value", IError::kESOperationFatal, false, false);
			;
		default:
			;
		}
		return true;
	}

	std::string ExtensionsConfigurationManagerImpl::GetPrefix(const eXMPIDPrefix prefixType) const
	{
		std::string prefix;
		switch (prefixType) {
		case IExtensionsConfigurationManager_v1::kOriginalDocumentIDPrefix:
			prefix = _orgDocIDPrefix;
			break;
		case IExtensionsConfigurationManager_v1::kInstanceIDPrefix:
			prefix = _instanceIDPrefix;
			break;
		case IExtensionsConfigurationManager_v1::kDocumentIDPrefix:
			prefix = _docIDPrefix;
			break;
			prefix = _prefix;
			break;
		default:
			;
		}
		return prefix.empty() ? _prefix : prefix;
	}
	bool APICALL ExtensionsConfigurationManagerImpl::RegisterHistoryUpdatePolicy(const eHistoryUpdatePolicy historyUpdatePolicy, const eHistoryUpdateAction historyUpdateAction) {
		_historyUpdatePolicy = historyUpdatePolicy;
		_historyUpdateAction = historyUpdateAction;

		return true;
	}

	void ExtensionsConfigurationManagerImpl::GetHistoryUpdatePolicyAndAction(eHistoryUpdatePolicy& historyUpdatePolicy, eHistoryUpdateAction& historyUpdateAction)
	{
		historyUpdatePolicy = _historyUpdatePolicy;
		historyUpdateAction = _historyUpdateAction;
	}

	std::string ExtensionsConfigurationManagerImpl::GetDateTime(std::string& dateStr)
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
	bool APICALL ExtensionsConfigurationManagerImpl::RegisterGetTimeCallback(GetDateTimeCallbackPtr getDateTimeCallbackPtr) {
		_getDateTimeCallbackPtr = getDateTimeCallbackPtr;
		return true;
	}

	IExtensionsConfigurationManager_v1::GetDateTimeCallbackPtr ExtensionsConfigurationManagerImpl::GetTimeCallbackPtr()
	{
		return _getDateTimeCallbackPtr;
	}

	bool ExtensionsConfigurationManagerImpl::UpdateHistoryEntry(spIArrayNode historyArrayNode, std::string ationStr, std::string changedStr, std::string swagent, std::string NewInstanceID, std::string dateStr)
	{
		if (historyArrayNode.get() == NULL)
			return false; //TODO@skpathak raise exception
		spIStructureNode historyEntryNewNode;
		spISimpleNode node;

		if (dateStr.empty()) {
			ITSingleton<IExtensionsConfigurationManager_I>::GetInstance()->GetDateTime(dateStr);
		}

		switch (_historyUpdatePolicy) {
		case IExtensionsConfigurationManager_v1::kUpdateSkip:
			break;
		case IExtensionsConfigurationManager_v1::kUpdateOneTime:
			break;
		case IExtensionsConfigurationManager_v1::kUpdateEveryTime:
			if (NewInstanceID.empty()) {
				std::string NewInstanceIDPrefix = ITSingleton< IExtensionsConfigurationManager_I >::GetInstance()->GetPrefix(IExtensionsConfigurationManager_v1::kInstanceIDPrefix);
				SXMPDocOps::CreateID(&NewInstanceID, NewInstanceIDPrefix.c_str() /*"xmp.iid:"*/);  //   
			}

			if (swagent.empty()) {
				swagent = ITSingleton< IExtensionsConfigurationManager_I >::GetInstance()->GetAppName();
			}

			historyEntryNewNode = IStructureNode::CreateStructureNode(kXMP_NS_XMP_MM, AdobeXMPCommon::npos, "History", AdobeXMPCommon::npos); // (refactor) length as per XMPMetaGet-Set.cpp
			node = ISimpleNode::CreateSimpleNode(kXMP_NS_XMP_ResourceEvent, AdobeXMPCommon::npos, "action", AdobeXMPCommon::npos, ationStr.empty() ? "saved" : ationStr.c_str(), ationStr.empty() ? AdobeXMPCommon::npos : ationStr.size());
			historyEntryNewNode->InsertNode(node);
			node = ISimpleNode::CreateSimpleNode(kXMP_NS_XMP_ResourceEvent, AdobeXMPCommon::npos, "changed", AdobeXMPCommon::npos, changedStr.empty() ? "/metadata" : changedStr.c_str(), changedStr.empty() ? AdobeXMPCommon::npos : changedStr.size());
			historyEntryNewNode->InsertNode(node);
			if (!swagent.empty()) {
				node = ISimpleNode::CreateSimpleNode(kXMP_NS_XMP_ResourceEvent, AdobeXMPCommon::npos, "softwareAgent", AdobeXMPCommon::npos, swagent.c_str(), swagent.size());
				historyEntryNewNode->InsertNode(node);
			}
			node = ISimpleNode::CreateSimpleNode(kXMP_NS_XMP_ResourceEvent, AdobeXMPCommon::npos, "instanceID", AdobeXMPCommon::npos, NewInstanceID.c_str(), NewInstanceID.size());
			historyEntryNewNode->InsertNode(node);

			node = ISimpleNode::CreateSimpleNode(kXMP_NS_XMP_ResourceEvent, AdobeXMPCommon::npos, "when", AdobeXMPCommon::npos, dateStr.c_str(), dateStr.size());
			historyEntryNewNode->InsertNode(node);

			historyArrayNode->AppendNode(historyEntryNewNode);

			break;
		default:
			;
		}

		return true;
	}

	bool ExtensionsConfigurationManagerImpl::RegisterExtensionHandler_I(const char * extensionNameSpace, sizet extensionNameSpaceLen, const char * extensionName, sizet extensionNameLen, pIExtensionHandler customHandler) {

		return AdobeXMPAM_Int::IExtensionHandlerRegistry::GetInstance()->RegisterHandler(extensionNameSpace, extensionName, customHandler);
	}

	bool ExtensionsConfigurationManagerImpl::RegisterExtensionHandler(const char * extensionNameSpace, sizet extensionNameSpaceLen, const char * extensionName, sizet extensionNameLen, pIExtensionHandler customHandler) {

		return RegisterExtensionHandler_I(extensionNameSpace, extensionNameSpaceLen, extensionName, extensionNameLen, customHandler);
	}
}
#if BUILDING_XMPEXTENSIONS_LIB || SOURCE_COMPILING_XMPEXTENSIONS_LIB
namespace AdobeXMPAM {
	spIExtensionsConfigurationManager IExtensionsConfigurationManager_v1::MakeShared(pIExtensionsConfigurationManager_base ptr) {
		if (!ptr) return spIExtensionsConfigurationManager();
		pIExtensionsConfigurationManager p = IExtensionsConfigurationManager::GetInterfaceVersion() > 1 ? ptr->GetInterfacePointer< IExtensionsConfigurationManager >() : ptr;
		return MakeUncheckedSharedPointer(p, __FILE__, __LINE__, false);
	}
}
#endif

