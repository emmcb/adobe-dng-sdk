#include "../../../public/include/XMP_Version.h"
#include "../../source/XMPCore_ChangeLog.hpp"
#include "../../../build/XMP_BuildInfo.h"

#if NDEBUG	// Can't use XMP_Environment.h, it seems to mess up the PList compiler.
	#define kConfig Release
	#define kDebugSuffix
	#define kBasicVersion XMPCORE_API_VERSION-c kXMPCore_EngBuild
#else
	#define kConfig Debug
	#define kDebugSuffix (debug)
	#define kBasicVersion XMPCORE_API_VERSION-c kXMPCore_EngBuild kDebugSuffix
#endif

#define kBuildInfo BUILDVERSION, BUILDDATE
#define kShortVersion MACRO_PASTE(XMPCORE_API_VERSION, ., BUILDID)

#define kShortFileVersion MACRO_PASTE(XMPCORE_API_VERSION, ., kXMPCore_EngBuild)
#define kFileVersion MACRO_PASTE(kShortFileVersion,.,BUILDID)
