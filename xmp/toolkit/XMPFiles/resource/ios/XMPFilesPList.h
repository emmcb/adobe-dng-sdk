#include "../../../public/include/XMP_Version.h"
#include "../../../build/XMP_BuildInfo.h"
#include "../../source/XMPFiles_ChangeLog.hpp"

#if NDEBUG	// Can't use XMP_Environment.h, it seems to mess up the PList compiler.
	#define kConfig Release
	#define kDebugSuffix
	#define kBasicVersion XMPFILES_API_VERSION-f kXMPFiles_EngBuild
#else
	#define kConfig Debug
	#if IncludeNewHandlers
		#define kDebugSuffix (debug/NewHandlers)
	#else
		#define kDebugSuffix (debug)
	#endif
	#define kBasicVersion XMPFILES_API_VERSION-f kXMPFiles_EngBuild kDebugSuffix
#endif

#define kBuildInfo BUILDVERSION, BUILDDATE
#define kShortVersion MACRO_PASTE(XMPFILES_API_VERSION, ., BUILDID)

#define kShortFileVersion MACRO_PASTE(XMPFILES_API_VERSION, ., kXMPFiles_EngBuild)
#define kFileVersion MACRO_PASTE(kShortFileVersion,.,BUILDID)
