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

#include "XMPAssetManagement/Interfaces/IAssetConfigurationManager_I.h"
#include "XMPCommon/Utilities/TWrapperFunctions_I.h"

namespace NS_INT_XMPASSETMANAGEMENT {
	using namespace NS_XMPCOMMON;
	using namespace NS_INT_XMPCOMMON;

	UInt32 IAssetConfigurationManager_I::RegisterAppName(const char * appName, pcIError_base & error) __NOTHROW__{
		return NS_INT_XMPCOMMON::CallUnSafeFunction< IAssetConfigurationManager_I, UInt32, bool, const char * >(
		error, this, false, &IAssetConfigurationManager_I::RegisterAppName, appName);
	}

	UInt32 IAssetConfigurationManager_I::RegisterPrefix(const char * prefix, const eXMPIDPrefix prefixType, pcIError_base & error) __NOTHROW__{
		return NS_INT_XMPCOMMON::CallUnSafeFunction< IAssetConfigurationManager_I, UInt32, bool, const char *, const eXMPIDPrefix >(
		error, this, false, &IAssetConfigurationManager_I::RegisterPrefix, prefix, prefixType);
	}

	UInt32 IAssetConfigurationManager_I::RegisterHistoryUpdatePolicy(const eHistoryUpdatePolicy historyUpdatePolicy, const eHistoryUpdateAction HistoryUpdateAction, pcIError_base & error) __NOTHROW__{
		return NS_INT_XMPCOMMON::CallUnSafeFunction< IAssetConfigurationManager_I, UInt32, bool, const eHistoryUpdatePolicy, const eHistoryUpdateAction>(
		error, this, false, &IAssetConfigurationManager_I::RegisterHistoryUpdatePolicy, historyUpdatePolicy, HistoryUpdateAction);
	}

	UInt32 IAssetConfigurationManager_I::RegisterGetTimeCallback(GetDateTimeCallbackPtr datetimeCallbackptr, pcIError_base & error) __NOTHROW__{
		return NS_INT_XMPCOMMON::CallUnSafeFunction< IAssetConfigurationManager_I, UInt32, bool, GetDateTimeCallbackPtr >(
		error, this, false, &IAssetConfigurationManager_I::RegisterGetTimeCallback, datetimeCallbackptr);
	}

}

