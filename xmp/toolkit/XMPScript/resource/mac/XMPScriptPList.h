#include "../../../public/include/XMP_Version.h"
#include "../../source/XMPScript_ChangeLog.hpp"
#include "../../../build/XMP_BuildInfo.h"

#if NDEBUG	// Can't use XMP_Environment.h, it seems to mess up the PList compiler.
	#define kConfig Release
	#define kDebugSuffix
	#define kBasicVersion XMPSCRIPT_API_VERSION-s kXMPScript_EngBuild
#else
	#define kConfig Debug
	#define kDebugSuffix (debug)
	#define kBasicVersion XMPSCRIPT_API_VERSION-s kXMPScript_EngBuild kDebugSuffix
#endif

#define kBuildInfo BUILDVERSION, BUILDDATE

#define kShortVersion MACRO_PASTE(XMPSCRIPT_API_VERSION, ., kXMPScript_EngBuild)
#define kFileVersion MACRO_PASTE(kShortVersion,.,BUILDID)
