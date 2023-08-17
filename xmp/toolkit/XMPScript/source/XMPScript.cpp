// =================================================================================================
// Copyright 2006-2007 Adobe
// All Rights Reserved.
//
// NOTICE:	Adobe permits you to use, modify, and distribute this file in accordance with the terms
// of the Adobe license agreement accompanying it. If you have received this file from a source other 
// than Adobe, then your use, modification, or distribution of it requires the prior written permission
// of Adobe.
// =================================================================================================

#include "XMPMetaFactory.h"
#include "XMPFileFactory.h"
#include "XMPUtilsFactory.h"
#include "XMPConst.h"
#include "XMPDateTimeFactory.h"
#include "ExtendScript.h"
#include "SoSharedLibDefs.h"
#include "XMPScript_ChangeLog.hpp"
#include "XMP_BuildInfo.h"

// only place to include XMP.incl_cpp
#include "XMP.incl_cpp"


/** Version info */

#if AdobePrivate
#define kXMPScriptName				"Adobe XMP Script"
	#define kXMPScript_BasicVersion		XMPSCRIPT_API_VERSION_STRING "-s" kXMPScript_EngString " " BUILDVERSIONSTR
	#define kXMPScript_VersionString	kXMPScriptName " " kXMPScript_BasicVersion
	#define kXMPScript_VersionMessage	kXMPScript_VersionString ", " BUILDDATESTR kXMPScript_DebugString
#else
	#define kXMPScriptName "XMP Script"
	#define kXMPScript_VersionMessage kXMPScriptName " " XMPSCRIPT_API_VERSION_STRING
#endif

const char * kXMPScript_EmbeddedVersion   = kXMPScript_VersionMessage;
const char * kXMPScript_EmbeddedCopyright = kXMPScriptName " " kXMP_CopyrightStr;

#if AdobePrivate

	const char * kXMPScript_EmbeddedAdobeIP = kXMP_AdobeIPStr;

	// Ugly strings wanted by CoreTech tools.
	const char * uglyScriptBuildVersion = "BUILDVERSION : " BUILDVERSIONSTR;
	const char * uglyScriptBuildDate    = "BUILDDATE : " BUILDDATESTR;
	const char * uglyScriptFileVersion  = "FILEVERSION : " XMPSCRIPT_API_VERSION_STRING "-s" kXMPScript_EngString;
	const char * uglyScriptCopyright    = "COPYRIGHT : " kXMP_CopyrightStr;
	#if XMP_DebugBuild
		const char * uglyScriptBuildVariant = "BUILDVARIANT : Debug";
	#else
		const char * uglyScriptBuildVariant = "BUILDVARIANT : Release";
	#endif

#endif

/** Exported functions */
extern "C" {
	char*	ESInitialize();
	void		ESTerminate();
	long		ESGetVersion();
	void		ESFreeMem (void*);
	long		getVersion (const TaggedData **, long, TaggedData * result);
};


/**
Initialize the external object, 
register the object factories.
*/
char* ESInitialize()
{
	// Make sure the embedded info strings are referenced and kept.
	if ( (kXMPScript_EmbeddedVersion[0] == 0) || (kXMPScript_EmbeddedCopyright[0] == 0) ) return null;
	#if AdobePrivate
		if ( kXMPScript_EmbeddedAdobeIP[0] == 0 ) return null;
		if ( (uglyScriptBuildVersion[0] == 0) || (uglyScriptBuildDate[0] == 0) ||
			 (uglyScriptFileVersion[0] == 0)  || (uglyScriptCopyright[0] == 0) || (uglyScriptBuildVariant[0] == 0) ) return null;
	#endif
	
	// Register XMPMetadataFactory in the global namespace.
	ScCore::LiveObject* xmpMetaFactory = new XMPMetaFactory;
	ScCore::LiveObject::addFactory (*xmpMetaFactory, ScCore::kJavaScriptLanguage);
	xmpMetaFactory->release();  // JS Engine took over the object

	// Register XMPFileFactory in the global namespace.
	ScCore::LiveObject* xmpFileFactory = new XMPFileFactory;
	ScCore::LiveObject::addFactory (*xmpFileFactory, ScCore::kJavaScriptLanguage);
	xmpFileFactory->release();  // JS Engine took over the object

	// Register XMPUtilsFactory in the global namespace.
	ScCore::LiveObject* xmpUtilsFactory = new XMPUtilsFactory;
	ScCore::LiveObject::addFactory (*xmpUtilsFactory, ScCore::kJavaScriptLanguage);
	xmpUtilsFactory->release();  // JS Engine took over the object

	// Register XMPConst in the global namespace.
	ScCore::LiveObject* xmpConst = new XMPConst;
	ScCore::LiveObject::addFactory (*xmpConst, ScCore::kJavaScriptLanguage);
	xmpConst->release();  // JS Engine took over the object

	// Register XMPDateTimeFactory in the global namespace.
	ScCore::LiveObject* xmpDateTimeFactory = new XMPDateTimeFactory;
	ScCore::LiveObject::addFactory (*xmpDateTimeFactory, ScCore::kJavaScriptLanguage);
	xmpDateTimeFactory->release();  // JS Engine took over the object

	return null;
}



/**
On the termination of XMP Scripting,
all factories and objects are removed from memory.
*/
void ESTerminate()
{
	// remove factories
	ScCore::LiveObject::removeFactory ("XMPMeta");
	ScCore::LiveObject::removeFactory ("XMPFile");
	ScCore::LiveObject::removeFactory ("XMPUtils");
	ScCore::LiveObject::removeFactory ("XMPDateTime");
	ScCore::LiveObject::removeFactory ("XMPConst");

	// remove instances
	ScScript::Engine::invalidateClassAll("XMPMeta");
	ScScript::Engine::invalidateClassAll("XMPFile");
	ScScript::Engine::invalidateClassAll("XMPIterator");
	ScScript::Engine::invalidateClassAll("XMPDateTime");
	ScScript::Engine::invalidateClassAll("XMPAliasInfo");
	ScScript::Engine::invalidateClassAll("XMPProperty");
	ScScript::Engine::invalidateClassAll("XMPFileInfo");
	ScScript::Engine::invalidateClassAll("XMPPacketInfo");
	ScScript::Engine::invalidateClassAll("XMPUtils");
	ScScript::Engine::invalidateClassAll("XMPConst");
}


/**
@return Returns the ExtendScript version.
*/
long ESGetVersion()
{
	return 1;
}


/**
Frees the memory used by getVersion()
*/
void ESFreeMem (void* p)
{
	free (p);
}

#if defined(WIN_ENV)
/**
@return	Returns the version of the ExtendScript wrapper.
*/
long getVersion (const TaggedData **, long, TaggedData * result)
{
	result->type = kTypeString;
	result->data.string = _strdup (kXMPScript_VersionMessage);
	return kESErrOK;
}
#endif
