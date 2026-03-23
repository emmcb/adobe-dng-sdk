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
// 03-01-15 SKP 1.0-a071 Fixed PK build break. Stopped building XMPAM unit test project.
// 02-19-15 SKP 1.0-a056 Fixed Allocator, ErrorNotification and threading handling in AssetConfigMgr
// 02-11-15 SKP 1.0-a050 Implementing support for XMPDateTime, History update policy and action
// 02-11-15 SKP 1.0-a049 Added initial support for client strategies. Using these strategies client
//						 can set prefix, AppName, history update policy and it's own datetime
//						 function which will be used to get time for ModifyDate, Metadata date etc.
// 12-07-14 ADC 1.0-a041 Refactored Part Classes and Adding support for IAssetArtboardPart.
// 11-28-14 ADC 1.0-a017 Submitting changes for fixing issue with the Mac build.
// 11-21-14 ADC 1.0-a002 Support for IAssetPart and IAssetTimePart interfaces.
// 11-19-14 ADC 1.0-a001 Framework in place for Asset Management Library.
//
// =================================================================================================
#endif  // AdobePrivate

#define OLD_DEPENDS_ON_XMPCORE LINKING_XMPCORE_LIB
#undef LINKING_XMPCORE_LIB
#define LINKING_XMPCORE_LIB 0

#include "XMPAssetManagement/XMPAssetManagement.h"
#include "build/XMP_BuildInfo.h"
#include "XMPAssetManagement/source/XMPAssetManagement_ChangeLog.hpp"
#include "XMPCommon/XMPCommon_I.h"
#include "XMPCommon/Interfaces/IConfigurationManager_I.h"
#include "XMPCore/Interfaces/ICoreObjectFactory.h"
#undef LINKING_XMPCORE_LIB
#define LINKING_XMPCORE_LIB OLD_DEPENDS_ON_XMPCORE
#undef OLD_DEPENDS_ON_XMPCORE

#include "XMPAssetManagement/XMPAssetManagementDefines_I.h"
#include "XMPExtensions/include/IExtensionsObjectFactory_I.h"
#include "XMPAssetManagement/Interfaces/IAssetUtilities_I.h"
#include "XMPExtensions/include/IExtensionsConfigurationManager_I.h"
#include "XMPAssetManagement/Interfaces/ITSingletonExtension.h"
#include "XMPAssetManagement/Interfaces/TSingletonExtension.h"

extern "C" {

	XMP_PUBLIC AdobeXMPAM::pIExtensionsConfigurationManager_base XMPAM_Initialize(AdobeXMPCore::pICoreObjectFactory_base xmpCoreObjFactoryPtr);

	XMP_PUBLIC void XMPAM_Terminate();

#if !SOURCE_COMPILING_XMPEXTENSIONS_LIB
	XMP_PUBLIC AdobeXMPAM::pIExtensionsObjectFactory_base XMPAM_GetObjectFactoryInstance();
#endif

}


#if XMP_DebugBuild
	#define kXMPExtensions_DebugFlag 1
#else
	#define kXMPExtensions_DebugFlag 0
#endif

#if AdobePrivate
	#define kXMPExtensionsName "Adobe XMP Asset Management"
	#define kXMPExtensions_BasicVersion   XMPEXTENSIONS_API_VERSION_STRING "-f" kXMPExtensions_EngString " " BUILDVERSIONSTR
	#define kXMPExtensions_VersionString  kXMPExtensionsName " " kXMPExtensions_BasicVersion
	#define kXMPExtensions_VersionMessage kXMPExtensions_VersionString ", " BUILDDATESTR kXMPExtensions_DebugString
#else
	#define kXMPExtensinos "XMP Extensions"
	#define kXMPExtensions_VersionMessage kXMPExtensionsName " " XMPEXTENSIONS_API_VERSION_STRING
#endif

const char * kXMPExtensions_EmbeddedVersion = kXMPExtensions_VersionMessage;
const char * kXMPExtensions_EmbeddedCopyright = kXMPExtensionsName " " kXMP_CopyrightStr;

#if AdobePrivate

	const char * kXMPExtensions_EmbeddedAdobeIP = kXMP_AdobeIPStr;

	// Ugly strings wanted by CoreTech tools.
	const char * uglyExtensionsBuildVersion = "ADOBE_INFO BUILDVERSION : " BUILDVERSIONSTR;
	const char * uglyExtensionsBuildDate = "ADOBE_INFO BUILDDATE : " BUILDDATESTR;
	const char * uglyExtensionsFileVersion = "ADOBE_INFO FILEVERSION : " XMPEXTENSIONS_API_VERSION_STRING "-f" kXMPExtensions_EngString;
	const char * uglyExtensionsCopyright = "ADOBE_INFO COPYRIGHT : " kXMP_CopyrightStr;
	const char * uglyExtensionsBuildId = "ADOBE_INFO BUILDID : " BUILDIDSTR;

	#if XMP_DebugBuild
		const char * uglyAssetManagementBuildVariant = "ADOBE_INFO BUILDVARIANT : Debug";
	#else
		const char * uglyAssetManagementBuildVariant = "ADOBE_INFO BUILDVARIANT : Release";
	#endif

#endif

void XMPAM_GetVersionInfo( XMP_VersionInfo * info ) {
	memset( info, 0, sizeof( XMP_VersionInfo ) );

	info->major = XMPEXTENSIONS_API_VERSION_MAJOR;
	info->minor = XMPEXTENSIONS_API_VERSION_MINOR;
	info->micro = 0; //no longer used
	info->isDebug = kXMPExtensions_DebugFlag;
#if AdobePrivate
	info->build = kXMPExtensions_EngBuild;
#endif
	info->flags = 0;	// ! None defined yet.
	info->message = kXMPExtensions_VersionMessage;
}

AdobeXMPAM::pIExtensionsConfigurationManager_base XMPAM_Initialize(AdobeXMPCore::pICoreObjectFactory_base xmpCoreObjFactoryPtr) {
	using namespace AdobeXMPCommon;
	using namespace AdobeXMPAM;
	if ( xmpCoreObjFactoryPtr == NULL ) return NULL;

	//XMPCoreObjectFactoryBaseInstance( true, xmpCoreObjFactoryPtr );
	//AdobeXMPCore::ICoreObjectFactory_v1::SetupCoreObjectFactory(xmpCoreObjFactoryPtr);

	try {
		AdobeXMPAM_Int::InitializeXMPCommonFramework();
		AdobeXMPAM::pIExtensionsConfigurationManager_base pAssetConfManager = ITSingleton<AdobeXMPAM_Int::IExtensionsConfigurationManager_I>::GetInstance();
        
		//set the default prefix, APPName 
		pAssetConfManager->RegisterAppName("Adobe XMP AML Toolkit", AdobeXMPCommon::npos);
		pAssetConfManager->RegisterPrefix("xmp.iid", AdobeXMPCommon::npos, AdobeXMPAM::IExtensionsConfigurationManager_v1::kInstanceIDPrefix);
		pAssetConfManager->RegisterPrefix("xmp.did", AdobeXMPCommon::npos, AdobeXMPAM::IExtensionsConfigurationManager_v1::kDocumentIDPrefix);
		pAssetConfManager->RegisterPrefix("xmp.did", AdobeXMPCommon::npos, AdobeXMPAM::IExtensionsConfigurationManager_v1::kOriginalDocumentIDPrefix);
		pAssetConfManager->RegisterGetTimeCallback(NULL);
		pAssetConfManager->RegisterHistoryUpdatePolicy(AdobeXMPAM::IExtensionsConfigurationManager_v1::kUpdateEveryTime, AdobeXMPAM::IExtensionsConfigurationManager_v1::kHistoryActionAll);
	#if !SOURCE_COMPILING_XMPEXTENSIONS_LIB
		AdobeXMPAM_Int::IExtensionsObjectFactory_I::CreateInstance();
	#endif
		AdobeXMPAM_Int::IAssetUtilities_I::CreateInstance();
		//return confManager.get();
		return pAssetConfManager;
	} catch ( ... ) {
		return NULL;
	}
}

void XMPAM_Terminate() {
	using namespace AdobeXMPCommon;
	AdobeXMPCore::ICoreObjectFactory_v1::DestroyCoreObjectFactory();
	try {
		AdobeXMPAM_Int::IAssetUtilities_I::DestroyInstance();
	#if !SOURCE_COMPILING_XMPEXTENSIONS_LIB
		AdobeXMPAM_Int::IExtensionsObjectFactory_I::DestroyInstance();
	#endif
		AdobeXMPAM_Int::TerminateXMPCommonFramework();
	} catch ( ... ) {

	}
}

#if !SOURCE_COMPILING_XMPEXTENSIONS_LIB
AdobeXMPAM::pIExtensionsObjectFactory_base XMPAM_GetObjectFactoryInstance() {
	return AdobeXMPAM_Int::IExtensionsObjectFactory_I::GetInstance();
}
#endif

