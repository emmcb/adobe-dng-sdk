// =================================================================================================
// Copyright 2014 Adobe
// All Rights Reserved.
//
// NOTICE:  Adobe permits you to use, modify, and distribute this file in accordance with the terms
// of the Adobe license agreement accompanying it. If you have received this file from a source other 
// than Adobe, then your use, modification, or distribution of it requires the prior written permission
// of Adobe.
// =================================================================================================


/*#define OLD_LINKING_XMPCORE_LIB LINKING_XMPCORE_LIB
#undef LINKING_XMPCORE_LIB
#define LINKING_XMPCORE_LIB 0*/

#include "XMPCompareAndMerge/XMPCompareAndMerge.h"
#include "build/XMP_BuildInfo.h"
#include "XMPCompareAndMerge/source/XMPCompareAndMerge_ChangeLog.hpp"
#include "XMPCommon/XMPCommon_I.h"
#include "XMPCommon/Interfaces/IConfigurationManager_I.h"
#include "XMPCompareAndMerge/Interfaces/ICompareAndMergeObjectFactory_I.h"
#include "XMPCore/Interfaces/ICoreObjectFactory.h"

/*#undef LINKING_XMPCORE_LIB
	#define LINKING_XMPCORE_LIB OLD_LINKING_XMPCORE_LIB
#undef OLD_LINKING_XMPCORE_LIB*/


#define IMPLEMENTATION_HEADERS_CAN_BE_INCLUDED 1
	#include "XMPCommon/ImplHeaders/ConfigurationManagerImpl.h"
#undef IMPLEMENTATION_HEADERS_CAN_BE_INCLUDED

#include "XMPCompareAndMerge/Interfaces/IGenericStrategyDatabase_I.h"

#if XMP_DebugBuild
	#define kXMPCompareAndMerge_DebugFlag 1
#else
	#define kXMPCompareAndMerge_DebugFlag 0
#endif

#if AdobePrivate
	#define kXMPCompareAndMergeName "Adobe XMP Compare And Merge"
	#define kXMPCompareAndMerge_BasicVersion   XMPCOMPAREANDMERGE_API_VERSION_STRING "-f" kXMPCompareAndMerge_EngString " " BUILDVERSIONSTR
	#define kXMPCompareAndMerge_VersionString  kXMPCompareAndMergeName " " kXMPCompareAndMerge_BasicVersion
	#define kXMPCompareAndMerge_VersionMessage kXMPCompareAndMerge_VersionString ", " BUILDDATESTR kXMPCompareAndMerge_DebugString
#else
	#define kXMPCompareAndMergeName "XMP Compare And Merge"
	#define kXMPCompareAndMerge_VersionMessage kXMPCompareAndMergeName " " XMPCOMPAREANDMERGE_API_VERSION_STRING
#endif

const char * kXMPCompareAndMerge_EmbeddedVersion   = kXMPCompareAndMerge_VersionMessage;
const char * kXMPCompareAndMerge_EmbeddedCopyright = kXMPCompareAndMergeName " " kXMP_CopyrightStr;

#if AdobePrivate

	const char * kXMPCompareAndMerge_EmbeddedAdobeIP = kXMP_AdobeIPStr;

	// Ugly strings wanted by CoreTech tools.
	const char * uglyFilesBuildVersion = "ADOBE_INFO BUILDVERSION : " BUILDVERSIONSTR;
	const char * uglyFilesBuildDate    = "ADOBE_INFO BUILDDATE : " BUILDDATESTR;
	const char * uglyFilesFileVersion  = "ADOBE_INFO FILEVERSION : " XMPCOMPAREANDMERGE_API_VERSION_STRING "-f" kXMPCompareAndMerge_EngString;
	const char * uglyFilesCopyright    = "ADOBE_INFO COPYRIGHT : " kXMP_CopyrightStr;
	const char * uglyFilesBuildId      = "ADOBE_INFO BUILDID : " BUILDIDSTR;

	#if XMP_DebugBuild
		const char * uglyFilesBuildVariant = "ADOBE_INFO BUILDVARIANT : Debug";
	#else
		const char * uglyFilesBuildVariant = "ADOBE_INFO BUILDVARIANT : Release";
	#endif

#endif

void XMPCM_GetVersionInfo ( XMP_VersionInfo * info ) {
	memset ( info, 0, sizeof(XMP_VersionInfo) );

	info->major   = XMPCOMPAREANDMERGE_API_VERSION_MAJOR;
	info->minor   = XMPCOMPAREANDMERGE_API_VERSION_MINOR;
	info->micro   = 0; //no longer used
	info->isDebug = kXMPCompareAndMerge_DebugFlag;
#if AdobePrivate
	info->build   = kXMPCompareAndMerge_EngBuild;
#endif
	info->flags   = 0;	// ! None defined yet.
	info->message = kXMPCompareAndMerge_VersionMessage;
}

namespace AdobeXMPCompareAndMerge_Int {
    
	 static pIConfigurationManager ManageConfigurationManager( bool destroy = false ) {
		static pIConfigurationManager sConfigurationManagerPtr( NULL );
		if ( destroy ) {
			if ( sConfigurationManagerPtr ) {
				delete sConfigurationManagerPtr;
				sConfigurationManagerPtr = NULL;
			}
			return sConfigurationManagerPtr;
		}

		if ( sConfigurationManagerPtr == NULL ) {
			sConfigurationManagerPtr = new ConfigurationManagerImpl();
		}
		return sConfigurationManagerPtr;
	}
}

AdobeXMPCommon::pIConfigurationManager XMPCM_Initialize( AdobeXMPCore::pICoreObjectFactory_base xmpCoreObjFactoryPtr) {
	using namespace AdobeXMPCommon;
	using namespace AdobeXMPCompareAndMerge_Int;

	if ( xmpCoreObjFactoryPtr == NULL ) return NULL;
#if LINKING_XMPCORE_LIB
	AdobeXMPCore::ICoreObjectFactory::SetupCoreObjectFactory();
#else
	AdobeXMPCore::ICoreObjectFactory::SetupCoreObjectFactory(xmpCoreObjFactoryPtr);
#endif
	try {
		InitializeXMPCommonFramework();
		ICompareAndMergeObjectFactory::SetupCompareAndMergeObjectFactory();
		IGenericStrategyDatabase_I::CreateGenericStrategyDatabase();
		return ManageConfigurationManager();
	} catch( ... ) {
		return NULL;
	}
}

void XMPCM_Terminate() {
	using namespace AdobeXMPCommon;
	using namespace AdobeXMPCompareAndMerge_Int;

	AdobeXMPCore::ICoreObjectFactory::DestroyCoreObjectFactory();
	try {
		IGenericStrategyDatabase_I::DestoryGenericStrategyDatabase();
		ICompareAndMergeObjectFactory::DestroyCompareAndMergeObjectFactory();
		TerminateXMPCommonFramework();
	} catch( ... ) {

	}
}

