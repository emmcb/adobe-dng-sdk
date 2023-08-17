#include "../../../public/include/XMP_Version.h"
#include "../../source/XMPExtensions_ChangeLog.hpp"
#include "../../../build/XMP_BuildInfo.h"

#if NDEBUG	// Can't use XMP_Environment.h, it seems to mess up the PList compiler.
	#define kConfig Release
	#define kDebugSuffix
	#define kBasicVersion XMPEXTENSIONS_API_VERSION-c kXMPExtensions_EngBuild
#else
	#define kConfig Debug
	#define kDebugSuffix (debug)
	#define kBasicVersion XMPEXTENSIONS_API_VERSION-c kXMPExtensions_EngBuild kDebugSuffix
#endif

#define kBuildInfo BUILDVERSION, BUILDDATE

#define kShortVersion MACRO_PASTE(XMPEXTENSIONS_API_VERSION, ., kXMPExtensions_EngBuild)
#define kFileVersion MACRO_PASTE(kShortVersion,.,BUILDID)
