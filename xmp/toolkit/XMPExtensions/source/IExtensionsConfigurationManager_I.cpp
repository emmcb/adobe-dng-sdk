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
// 02-11-15 SKP 1.0-a049 Added initial support for client strategies. Using these strategies client
//						 can set prefix, AppName, history update policy and it's own datetime
//						 function which will be used to get time for ModifyDate, Metadata date etc.
//
// =================================================================================================
#endif  // AdobePrivate

#include "XMPExtensions/include/IExtensionsConfigurationManager_I.h"
#include "XMPCommon/Utilities/TWrapperFunctions_I.h"

namespace AdobeXMPAM_Int {
	using namespace AdobeXMPCommon;
	using namespace AdobeXMPCommon_Int;

	pvoid APICALL IExtensionsConfigurationManager_I::GetInterfacePointer(uint64 interfaceID, uint32 interfaceVersion) {
		return GetInterfacePointerInternal(interfaceID, interfaceVersion, true);
	}

	pvoid APICALL IExtensionsConfigurationManager_I::getInterfacePointer(uint64 interfaceID, uint32 interfaceVersion, pcIError_base & error) __NOTHROW__{
		return CallUnSafeFunction< IExtensionsConfigurationManager_I, pvoid, pvoid, uint64, uint32 >(
		error, this, NULL, &IExtensionsConfigurationManager_I::GetInterfacePointer, __FILE__, __LINE__, interfaceID, interfaceVersion);
	}

		pvoid APICALL IExtensionsConfigurationManager_I::GetInterfacePointerInternal(uint64 interfaceID, uint32 interfaceVersion, bool isTopLevel) {
		if (interfaceID == kIExtensionsConfigurationManagerID) {
			switch (interfaceVersion) {
			case 1:
				return static_cast<IExtensionsConfigurationManager_v1 *>(this);
				break;

			case kInternalInterfaceVersionNumber:
				return this;
				break;

			default:
				throw IError_I::CreateInterfaceVersionNotAvailableError(
					IError_v1::kESOperationFatal, interfaceID, interfaceVersion, __FILE__, __LINE__);
				break;
			}
		}
		else {
			pvoid returnValue(NULL);
			returnValue = IConfigurationManager_I::GetInterfacePointerInternal(interfaceID, interfaceVersion, false);
			if (returnValue) return returnValue;
		}

		if (isTopLevel)
			throw IError_I::CreateInterfaceNotAvailableError(
			IError_v1::kESOperationFatal, kIExtensionsConfigurationManagerID, interfaceID, __FILE__, __LINE__);
		return NULL;
	}

	uint32 APICALL IExtensionsConfigurationManager_I::registerAppName(const char * appName, sizet appNameLength, pcIError_base & error) __NOTHROW__{
		
         return CallUnSafeFunction< IExtensionsConfigurationManager_v1, uint32, bool, const char *, sizet >(
        error, this, false, &IExtensionsConfigurationManager_v1::RegisterAppName, __FILE__, __LINE__, appName, appNameLength);
        //return RegisterAppName(appName, appNameLength);
	}

		uint32 APICALL IExtensionsConfigurationManager_I::registerPrefix(const char * prefix, sizet prefixLength, const eXMPIDPrefix prefixType, pcIError_base & error) __NOTHROW__{
		return CallUnSafeFunction< IExtensionsConfigurationManager_v1, uint32, bool, const char *, sizet, const eXMPIDPrefix >(
		error, this, false, &IExtensionsConfigurationManager_v1::RegisterPrefix, __FILE__, __LINE__, prefix, prefixLength, prefixType);
      //  return RegisterPrefix(prefix, prefixLength, prefixType);
	}

		uint32 APICALL IExtensionsConfigurationManager_I::registerHistoryUpdatePolicy(const eHistoryUpdatePolicy historyUpdatePolicy, const eHistoryUpdateAction HistoryUpdateAction, pcIError_base & error) __NOTHROW__{
		return CallUnSafeFunction< IExtensionsConfigurationManager_v1, uint32, bool, const eHistoryUpdatePolicy, const eHistoryUpdateAction>(
		error, this, false, &IExtensionsConfigurationManager_v1::RegisterHistoryUpdatePolicy, __FILE__, __LINE__, historyUpdatePolicy, HistoryUpdateAction);
            return RegisterHistoryUpdatePolicy( historyUpdatePolicy, HistoryUpdateAction);
	}

		uint32 APICALL IExtensionsConfigurationManager_I::registerGetTimeCallback(GetDateTimeCallbackPtr datetimeCallbackptr, pcIError_base & error) __NOTHROW__{
		return CallUnSafeFunction< IExtensionsConfigurationManager_v1, uint32, bool, GetDateTimeCallbackPtr >(
		error, this, false, &IExtensionsConfigurationManager_v1::RegisterGetTimeCallback, __FILE__, __LINE__, datetimeCallbackptr);
            return RegisterGetTimeCallback( datetimeCallbackptr);
	}

	uint32 APICALL IExtensionsConfigurationManager_I::registerExtensionHandler(const char * extensionNameSpace, sizet extensionNameSpaceLen, const char * extensionName, sizet extensionNameLen, pIExtensionHandler customHandler, pcIError_base & error) __NOTHROW__{

		return CallUnSafeFunction< IExtensionsConfigurationManager_I, uint32, bool, const char *, sizet, const char *, sizet, pIExtensionHandler >(
		error, this, false, &IExtensionsConfigurationManager_I::RegisterExtensionHandler_I, __FILE__, __LINE__, extensionNameSpace, extensionNameSpaceLen, extensionName, extensionNameLen, customHandler);
	}

}

