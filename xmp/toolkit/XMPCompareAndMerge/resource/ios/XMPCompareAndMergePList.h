#include "../../../public/include/XMP_Version.h"
#include "../../source/XMPCompareAndMerge_ChangeLog.hpp"
#include "../../../build/XMP_BuildInfo.h"

#if NDEBUG	// Can't use XMP_Environment.h, it seems to mess up the PList compiler.
	#define kConfig Release
	#define kDebugSuffix
	#define kBasicVersion XMPCOMPAREANDMERGE_API_VERSION-c kXMPCompareAndMerge_EngBuild
#else
	#define kConfig Debug
	#define kDebugSuffix (debug)
	#define kBasicVersion XMPCOMPAREANDMERGE_API_VERSION-c kXMPCompareAndMerge_EngBuild kDebugSuffix
#endif

#define kBuildInfo BUILDVERSION, BUILDDATE
#define kShortVersion MACRO_PASTE(XMPCOMPAREANDMERGE_API_VERSION, ., BUILDID)

#define kShortFileVersion MACRO_PASTE(XMPCOMPAREANDMERGE_API_VERSION, ., kXMPCompareAndMerge_EngBuild)
#define kFileVersion MACRO_PASTE(kShortFileVersion,.,BUILDID)
