#include "../../../public/include/XMP_Version.h"
#include "../../source/XMPAssetManagement_ChangeLog.hpp"
#include "../../../build/XMP_BuildInfo.h"

#if NDEBUG	// Can't use XMP_Environment.h, it seems to mess up the PList compiler.
	#define kConfig Release
	#define kDebugSuffix
	#define kBasicVersion XMPASSETMANAGEMENT_API_VERSION-c kXMPAssetManagement_EngBuild
#else
	#define kConfig Debug
	#define kDebugSuffix (debug)
	#define kBasicVersion XMPASSETMANAGEMENT_API_VERSION-c kXMPAssetManagement_EngBuild kDebugSuffix
#endif

#define kBuildInfo BUILDVERSION, BUILDDATE
