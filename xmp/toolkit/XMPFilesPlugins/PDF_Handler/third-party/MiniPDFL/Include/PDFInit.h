/*
 * PDFInit.h - PDF Toolkit and library initialization & shutdown declarations
 * Copyright (c) 1996-2006 Adobe Systems Inc.
 * All Rights Reserved.
 */
 
#ifndef _H_PDFInit
#define _H_PDFInit

#include "Environ.h"

#if ACROBAT_LIBRARY || PLUGIN
#include "PIExcept.h"
#include "CorCalls.h"
#include "ASExpT.h"
#else 
#include "PubTypes.h"
#include "ASAllocC.h"
#include "ASTypes.h"
#include "ASStream.h"
#include "ASExtend.h"
#include "ASHFT.h"
#endif

#if WIN_PLATFORM
#include <Windows.h>
#elif MAC_PLATFORM
#if defined (__MWERKS__)
#include <CarbonCore/Multiprocessing.h>
#else
//#include <CoreServices/CoreServices.h>
#endif
#include <Carbon/Carbon.h>
#else
#include <pthread.h>
#endif

#if __cplusplus
extern "C" {
#endif

#define kPDFLVersion	0x00090000 /*obsolete */

/** Flags for PDFLInit */
enum {

	/** Do not search through default font directories.
		@see PDFLGetFlags
	*/
	kPDFLInitIgnoreDefaultDirectories = 0x001,

	/** (obsolete) If it is not necessary to write PDF, there is no need to initialize the
	 font table (which would be much faster!). 
	*/
	kReadOnlyInit = 	0x0002,		

	/** Check for substitution fonts at toolkit initialization.
	If they are not found, return an error.
	*/
	kCheckSubFonts =	0x0004,		

	/** Do not load plug-ins if this is set. */
	kDontLoadPlugIns =	0x0008,	

	/** TEST ONLY ** Create the same PDF file for every run. This forces
	dates to January 1, 1999 at 1:01:01 and file IDs to
	"[&lt;11111111111111111111111111111111&gt;&lt;11111111111111111111111111111111&gt;]".
	The resulting PDF files are invalid and should only be used for testing
	purposes. */
	kTestUniform =		0x0010,

	/** Initialize PDFEdit if this is <code>true</code>. */
	kInitPDFEdit =		0x0020

	/** Reserved */
	/** 0x1000 */
	/** 0x2000 */
};

#if ACROBAT_LIBRARY || PLUGIN
/* typedefs for Memory Allocator */

/**
	Called when the PDF Library needs to allocate memory. The callback 
	should allocate the requested amount of memory and return 
	it as a <code>void*</code>. 
	@param clientData The <code>clientData</code> specified in the TKAllocatorProcs 
	structure. 
	@param size The amount of memory to be allocated.
	@return A pointer to the allocated block of memory. 
	@see ASClientMemAvailProc 
	@see ASClientFreeProc 
	@see ASClientReallocProc 
	@see PDFLInit 
*/
typedef void * (*ASClientAllocProc)(void *clientData, ASSize_t size);
typedef ASClientAllocProc ASMemAllocProc;

/**
	Called when the PDF Library needs to reallocate memory. 
	The callback should reallocate the requested amount of memory 
	and return it as a <code>void*</code>. 
	@param clientData A pointer to the <code>clientData</code> specified 
	in the TKAllocatorProcs structure. 
	@param p A pointer to memory to reallocate. 
	@param size The amount of memory requested.
	@return A pointer to the allocated block of memory. 
	@see ASClientAllocProc 
	@see ASClientMemAvailProc 
	@see ASClientFreeProc 
	@see PDFLInit 
*/
typedef void* (*ASClientReallocProc)(void *clientData, void* p, ASSize_t size);
typedef ASClientReallocProc ASMemReallocProc;

/**
	Called when the PDF Library needs to free memory. The callback 
	should free the requested block of memory. 
	@param clientData A pointer to the <code>clientData</code> specified 
	in the TKAllocatorProcs structure. 
	@param p A pointer to memory to free.
	@see ASClientAllocProc 
	@see ASClientMemAvailProc 
	@see ASClientReallocProc 
	@see PDFLInit 
*/
typedef void (*ASClientFreeProc)(void *clientData, void *p);
typedef ASClientFreeProc ASMemFreeProc;

/**
	Called when the PDF Library needs to determine the amount 
	of available free memory. 
	@param clientData A pointer to the <code>clientData</code> specified 
	in the TKAllocatorProcs structure.
	@return The amount of available free memory. 
	@see ASClientAllocProc 
	@see ASClientFreeProc 
	@see ASClientReallocProc 
	@see PDFLInit 
*/
typedef ASSize_t (*ASClientMemAvailProc)(void *clientData);
typedef ASClientMemAvailProc ASMemAvailProc;
#endif /* ACROBAT_LIBRARY || PLUGIN */


/**
	A structure for memory allocation callbacks used by the PDF 
	Library. 
	@see PDFLInit 
*/
typedef struct _t_TKAllocatorProcs {

	/** Called when the PDF
		Library needs to allocate memory.
	*/
	ASClientAllocProc		allocProc;

	/** Called when the PDF Library
		needs to reallocate a block of memory.
	*/
	ASClientReallocProc	reallocProc;

	/** Called when the PDF Library
		needs to free a block of memory.
	*/
	ASClientFreeProc		freeProc;

	/** Called when the PDF Library
		needs to determine the amount of memory available.
	*/
	ASClientMemAvailProc		memAvailProc;

	/** 
		A pointer to data to pass into each of the memory management
		callbacks.
	*/
	void *clientData;
} TKAllocatorProcs, *TKAllocatorProcsP;


/**
	A callback in the TKResourceProcs structure in the Adobe PDF Library. 
	It acquires the specified resource and uses it to fill in the 
	<code>rdStm</code> parameter. 
	@param resourceName The name of the resource to acquire. 
	@param resType One of the following from the <code>resType</code> enum: 
		<ul>
			<li>resCMapData</li>
			<li>resCharTypeData</li>
			<li>resCharRangeData</li>
			<li>resFontPropData</li>
			<li>resCJKNoteFontNormal</li>
			<li>resCJKNoteFontBold</li>
			<li>resCJKNoteFontOblique</li>
			<li>resFontData</li>
			<li>resCMapLastID</li>
		</ul>
	    
	@param registry One of the following:
		<TABLE rules="all" cellspacing="1">
		<TR><TH>Value</TH><TH>Description</TH></TR>
		<TR><TD><code>Adobe-Japan1</code></TD><TD>Japanese</TD></TR>
		<TR><TD><code>Adobe-Korea1</code></TD><TD>Korean</TD></TR>
		<TR><TD><code>Adobe-CNS1</code></TD><TD>Traditional Chinese</TD></TR>
		<TR><TD><code>Adobe-GB1</code></TD><TD>Simplified Chinese</TD></TR>
		</TABLE>

	@param size (Filled by the callback) The size of the ASStm 
	returned. 
	@param clientData A pointer to the <code>clientData</code> specified 
	in the TKResourceProcs structure. 
	@param rdStm (Filled by the callback) An ASStm for the 
	data requested.
	@return A <code>void*</code> of data that you want returned to you in TKResourceReleaseProc(). 
	This data can be used to determine what Resource to release. 
	You can return <code>NULL</code> from TKResourceAcquireProc(). 
	@see TKResourceReleaseProc 
	@see PDFLInit
*/
typedef ACCBPROTO1 void* (ACCBPROTO2 *TKResourceAcquireProc)(char *resourceName, ASInt32 resType, void *registry, ASInt32 *size, void *clientData, ASStm *rdStm);


/**
	A callback in the TKResourceProcs structure in the Adobe PDF Library. 
	It releases the resources previously acquired and closes the 
	ASStm passed in as <code>rdStm</code>. 
	@param rdStm An ASStm for the resource to release and 
	close with ASStmClose(). 
	@param data The <code>clientData</code> returned from PMSetTextProc(). 
	
	@param clientData A pointer to the clientData specified 
	in the TKResourceProcs structure.
	@see PMSetTextProc 
	@see PDFLInit 
*/
typedef ACCBPROTO1 void (ACCBPROTO2 *TKResourceReleaseProc)(ASStm rdStm, void * data, void *clientData);



/**
	Structure for resource acquisition callbacks used by the 
	PDF Library. 
	@see PDFLInit 
*/
typedef struct _t_TKResourceProcs {

	/** Called when the PDF Library
		needs to acquire resources.
	*/
	TKResourceAcquireProc acquireProc;

	/** Called when the PDF
		Library needs to release resources.
	*/
	TKResourceReleaseProc releaseProc;

	/** A pointer to data to pass into every 
		call to the PMSetTextProc() or TKResourceReleaseProc() as the 
		<code>clientData</code> argument.
	*/
	void * clientData;
} TKResourceProcs, *TKResourceProcsP;


/** 
	Called periodically during plug-in loading with the ASExtension
	currently being loaded, and a selector indicating what phase of
	loading is happening.
*/
#if ACROBAT_LIBRARY || PLUGIN
enum
{

	/** Loading  */
	ASExtensionPhase_LOADING,

	/** Exporting */
	ASExtensionPhase_EXPORTING,

	/** Importing */
	ASExtensionPhase_IMPORTING,

	/** Initializing */
	ASExtensionPhase_INITIALIZING,

	/** Unloading */
	ASExtensionPhase_UNLOADING
};

typedef ASInt16 ASExtensionPhase;


/**
	Called periodically during plug-in loading with the ASExtension 
	currently being loaded. 

	@note Extensions to the PDF Library are very similar to 
	plugins to the Acrobat application. However, Adobe 
	does not currently supply any extensions or support their 
	creation. 
	@param extension The plug-in. 
	@param phase The context of the load, which determines 
	the sequence of operations during the load operation. It is one 
	of these constants: 
	
	<ul>
		<li>ASExtensionPhase_LOADING</li>
		<li>ASExtensionPhase_EXPORTING</li>
		<li>ASExtensionPhase_IMPORTING</li>
		<li>ASExtensionPhase_INITIALIZING</li>
		<li>ASExtensionPhase_UNLOADING</li>
	</ul>
	
	@param clientData A pointer to the client data.
	@see ASExtensionReportErrorProc 
*/
typedef ACCBPROTO1 void (ACCBPROTO2 *ASExtensionProgressProc)(
	ASExtension extension,
	ASExtensionPhase phase,
	void *clientData);


/**
	Called during ASExtension initialization whenever an error 
	occurs. 
	@param extension The plug-in. 
	@param phase Phase. 
	@param error The error code. 
	@param clientData A pointer to the client data.
	@see ASExtensionProgressProc 
*/
typedef ACCBPROTO1 void (ACCBPROTO2 *ASExtensionReportErrorProc)(
	ASExtension extension,
	ASExtensionPhase phase,
	ASInt32 error,
	void *clientData);


#endif /* ACROBAT_LIBRARY || PLUGIN */


/**
	A structure containing function pointers for ASExtension callbacks 
	used by the PDF Library. 
	@see PDFLInit 
*/
typedef struct _t_TKExtensionProcs {

	/** Called when the
		PDF Library needs to allocate memory.
	*/
	ASExtensionProgressProc		progressProc;

	/** Called when
		the PDF Library needs to reallocate a block of memory.
	*/
	ASExtensionReportErrorProc	reportErrProc;

	/** A <code>void*</code> of data to pass into each of the memory
		management callbacks.
	*/
	void						*clientData;
} TKExtensionProcs, *TKExtensionProcsP;


/** PDFLData structure for PDFLInit. */
#if WIN_PLATFORM
typedef DWORD ThreadLocalKey;
typedef struct _t_PDFLData {

	/** The size of the data structure. It must be set to <code>sizeof(PDFLDataRec)</code>. */
	ASSize_t size;	

	/** Currently <code>kPDFLInitIgnoreDefaultDirectories</code> and <code>kDontLoadPlugIns</code> flags are supported.
		When <code>kPDFLInitIgnoreDefaultDirectories</code> flag is set, the initialization process does not
		search through the default font directories(currently Adobe font directories installed by some Adobe
		applications), but only in those specified in <code>dirList</code>.
		When <code>kDontLoadPlugIns</code> flag is set, plug-ins are ignored during initialization process.
	*/
	ASUns32 flags;

	/** (Windows only)
		<p>The location of resources for the Library. For a static library, you link them
		into your application and use <code>NULL</code> for <code>inst</code>. For a DLL version of the
		library, call <code>LoadLibrary</code> on the DLL and set <code>inst</code> equal to the
		<code>HINSTANCE</code> returned:</p>

		<p><code>libData.inst = LoadLibrary("PDFL50.DLL");</code></p>

		<p>If you are linking the static library into a DLL, pass the <code>HINSTANCE</code>
		passed into <code>DllMain</code> or <code>WinMain</code> for <code>inst</code>, and link the resources into
		the DLL.</p>

		<p>Free the <code>HINSTANCE</code> with <code>FreeLibrary</code> after you call PDFLTerm.</p>
	*/
	HINSTANCE inst;

	/** List of directories with fonts. 
	
		@example For Windows:
		
		<p><code>libData.listLen=3L;</code></p>
		<p><code>libData.dirList=(ASUTF16Val**)malloc(sizeof(ASUTF16Val*)*3);</code></p>
		<p><code>libData.dirList[0]=(ASUTF16Val*)myFontDir0;</code></p>
		<p><code>libData.dirList[1]=(ASUTF16Val*)myFontDir1;</code></p>
		<p><code>libData.dirList[2]=(ASUTF16Val*)myFontDir2;</code></p>
		
	 */
	ASUTF16Val **dirList;

	/** The number of directories listed in <code>dirList</code>. */
	ASInt32 listLen;

	/** The structure containing function pointers for memory allocation
		callbacks. The library manages its own memory. Most applications
		linking with the library should pass <code>NULL</code> for this member. Applications
		wishing more control over memory allocation can fill out this structure
		and will be responsible for providing and freeing memory, as well as
		reporting available memory as requested by the library.
	*/
	TKAllocatorProcsP allocator;

	/** The structure containing function pointers for resource management
		callbacks. The library manages its own resources. Most applications
		linking with the library should pass <code>NULL</code> for this member. Applications
		wishing more control over resource allocation can fill out this structure
		and will be responsible for providing and freeing resources requested
		by the library.
	*/
	TKResourceProcsP resProcs;

	/** The structure containing function pointers for ASExtension callbacks
		used by the PDF Library. 
	*/
	TKExtensionProcsP extProcs;

	/** A list of font cache file names. It is parallel to <code>dirList</code>, but contains
		separately configurable target files for CoolType cache files for
		use in cases where the specified font directory may not be writable
		by the client. Either <code>cacheFileList</code> or any/all of its subsidiary
		pointers may be <code>NULL</code>, in which case CoolType will default to the
		font directory passed in <code>dirList</code>.
	*/
	ASUTF16Val **cacheFileList;

	/** A separately configurable target file where CoolType should store the
		system font cache file. It may be <code>NULL</code>, in which case CoolType will
		choose a default path.
	*/
	ASUTF16Val *systemCacheFile;

	/** Separately configurable target file where CoolType should store the
		common font cache file. It may be <code>NULL</code>, in which case CoolType will
		choose a default path.
	*/
	ASUTF16Val *commonCacheFile;

	/** The complete <code>NULL</code>-terminated path of the directory where CoolType can
		find CMap files. This directory will be the preferred location for
		CMaps that are associated with the system fontset. If CMaps with the
		same name are found in this directory and common fonts
		directory, the copy in this directory will be preferred. It may be 
		<code>NULL</code>, in which case CoolType will only associate CMaps present in the
		common fonts directory with the system font set.
	*/
	ASUTF16Val *cMapDirectory;

	/** The complete, <code>NULL</code>-terminated path of the directory in which the Unicode mapping files 
		can be found.
	*/
	ASUTF16Val *unicodeDirectory;

	/**
		Specifies the number of directories listed in <code>pluginDirList</code>.
		With regards to loading plug-ins, PDF Library behavior is now as follows: 
		
		<ul>
		<li>If <code>pluginDirListLen > 0</code>, then PDF Library will only look for plug-ins in the paths passed in <code>pluginDirList</code>.
		   It will not search the earlier locations, (for example, <code>"pdfplug_ins"</code> folder on Windows, <code>"PDFPlug-Ins"</code> on Mac 
		   OS, and the list of directories passed in <code>dirList</code> on Unix.</li>
		<li>If <code>pluginDirListLen <= 0</code> or is not a part of the <code>PDFLDataRec</code> structure passed by the PDF Library client,
		   then PDF Library will search the earlier locations for plug-ins.</li>
		</ul>

	*/	ASInt32	   pluginDirListLen;

	/**
		<p>An array of plug-in directories.</p>

		<p>If a PDF Library client wants PDFL to load headless PDF Library plug-ins from desired locations, the client can pass a list
		of paths in <code>pluginDirList</code>. The 2 fields <code>pluginDirList</code> and <code>pluginDirListLen</code> are similar 
		to the existing fields <code>dirList</code> and <code>listLen</code>.</p>

		<p>Clients can pass an array of plug-in directories in the field <code>pluginDirList</code>:</p>
		<ul>
			<li>On Windows, <code>pluginDirList</code> is an array of <code>ASUTF16Val</code> paths. Each path should be <code>NULL</code>-terminated.</li>
			<li>On Mac OS, the paths passed in <code>pluginDirList</code> should be <code>NULL</code>-terminated UTF-8 POSIX strings. Hence,
			  the type of <code>pluginDirList</code> on Mac is <code>char **</code>.</li>
			<li>On Unix, the paths passed in <code>pluginDirList</code> should be simple <code>NULL</code>-terminated <code>char</code> strings.</li>
	    </ul>

	*/	ASUTF16Val **pluginDirList;

	/**
		Specifies the number of directories listed in <code>colorProfileDirList</code>.

	*/	
	ASInt32	   colorProfileDirListLen;

	/**
		<p>An array of color profile directories.</p>

		<p>If a PDF Library client wants to search for color profiles from custom
		directories, the client can pass a list	of paths in <code>colorProfileDirList</code>.
		The 2 fields <code>colorProfileDirList</code> and <code>colorProfileDirListLen</code>
		are similar	to the existing fields <code>dirList</code> and <code>listLen</code>.</p>

		<p>If the client does not set this parameter then PDF Library will search
		for the color profiles from default locations such as Creative Suites and
		Acrobat	installation directories. If the clients sets this parameter then
		PDF Library will search for the color profiles from it's default directory
		as	well as from the custom directories specified by the client through
		parameter <code>colorProfileDirList</code>. However the order of searching for
		these profiles from default and custom directories is not fixed.</p>
		<ul>
			<li>On Windows, <code>colorProfileDirList</code> is an array of <code>ASUTF16Val</code> paths.
			  Each path should be <code>NULL</code>-terminated.</li>
			<li>On Mac OS, the paths passed in <code>colorProfileDirList</code> should be <code>NULL</code>-terminated
			  UTF-8 POSIX strings. Hence, the type of <code>colorProfileDirList</code> on Mac is <code>char **</code>.</li>
			<li>On Unix, the paths passed in <code>colorProfileDirList</code> should be simple <code>NULL</code>-terminated
			  <code>char</code> strings.</li>
	    </ul>

	*/	
	ASUTF16Val **colorProfileDirList;

} PDFLDataRec, *PDFLData;
#elif MAC_PLATFORM && !IOS
typedef TaskStorageIndex ThreadLocalKey;

/** PDFLData structure. */
typedef struct _t_PDFLData {

	/** The size of the data structure. It must be set to <code>sizeof(PDFLDataRec)</code>. */
	ASSize_t size;	

	/** Currently <code>kPDFLInitIgnoreDefaultDirectories</code> and <code>kDontLoadPlugIns</code> flags are supported.
		When <code>kPDFLInitIgnoreDefaultDirectories</code> flag is set, the initialization process does not
		search through the default font directories(currently Adobe font directories installed by some Adobe
		applications), but only in those specified in <code>dirList</code>.
		When <code>kDontLoadPlugIns</code> flag is set, plug-ins are ignored during initialization process.
	*/
	ASUns32 flags;

	/** A list of directories with fonts. 
	
		@example For Windows:
		<p><code>libData.listLen=3L;</code></p>
		<p><code>libData.dirList=(ASPathName *)malloc(sizeof(ASPathName)*3);</code></p>
		<p><code>libData.dirList[0]=(ASPathName)"c:\\PSFonts";</code></p>
		<p><code>libData.dirList[1]=(ASPathName)"c:\\Fonts";</code></p>
		<p><code>libData.dirList[2]=(ASPathName)"c:\\CMap";</code></p>
		
	*/
	char **dirList;

	/** The number of directories listed in <code>dirList</code>.
	*/
	ASInt32 listLen;

	/** The structure containing function pointers for memory allocation
		callbacks. The library manages its own memory. Most applications
		linking with the library should pass <code>NULL</code> for this member. Applications
		wishing more control over memory allocation can fill out this structure
		and will be responsible for providing and freeing memory, as well as
		reporting available memory as requested by the library.
	*/
	TKAllocatorProcsP allocator;

	/** (Mac only)
		<p>The resources location, if they are not in the application itself. Set them
		using OpenResFile:</p>

		@example <code>libData.resFile = OpenResFile("\\pMyResFileGoesHere")</code>
	*/
	short resFile;

	/** The structure containing function pointers for resource management
		callbacks. The library manages its own resources. Most applications
		linking with the library should pass <code>NULL</code> for this member. Applications
		wishing more control over resource allocation can fill out this structure
		and will be responsible for providing and freeing resources requested
		by the library.
	*/
	TKResourceProcsP resProcs;

	/** The structure containing function pointers for ASExtension callbacks
		used by the PDF Library.
	*/
	TKExtensionProcsP extProcs;

	/** A list of font cache file names. It is parallel to <code>dirList</code>, but contains
		separately configurable target files for CoolType cache files for
		use in cases where the specified font directory may not be writable
		by the client. Either <code>cacheFileList</code> or any/all of its subsidiary
		pointers may be <code>NULL</code>, in which case CoolType will default to the
		font directory passed in <code>dirList</code>.
	*/
	char **cacheFileList;

	/** Separately configurable target file where CoolType should store the
		system font cache file. It may be <code>NULL</code>, in which case CoolType will
		choose a default path.
	*/
	char *systemCacheFile;

	/** Separately configurable target file where CoolType should store the
		common font cache file. It may be <code>NULL</code>, in which case CoolType will
		choose a default path.
	*/
	char *commonCacheFile;

	/** The complete <code>NULL</code>-terminated path of the directory where CoolType can
		find CMap files. This directory will be the preferred location for
		CMaps that are associated with the system fontset. If CMaps with the
		same name are found in this directory and common fonts
		directory, the copy in this directory will be preferred. May be 
		<code>NULL</code>, in which case CoolType will only associate CMaps present in
		common fonts directory with the system font set.
	*/
	char *cMapDirectory;

	/** The complete, <code>NULL</code>-terminated path of the directory in which the Unicode mapping files 
		can be found.
	*/
	char *unicodeDirectory;

	/**
		Specifies the number of directories listed in <code>pluginDirList</code>.
		With regards to loading plug-ins, PDF Library behavior is now as follows: 
		
		<ul>
		<li>If <code>pluginDirListLen > 0</code>, then PDF Library will only look for plug-ins in the paths passed in <code>pluginDirList</code>.
		   It will not search the earlier locations, (for example, <code>"pdfplug_ins"</code> folder on Windows, <code>"PDFPlug-Ins"</code> on Mac 
		   OS, and the list of directories passed in <code>dirList</code> on Unix.</li>
		<li>If <code>pluginDirListLen <= 0</code> or is not a part of the <code>PDFLDataRec</code> structure passed by the PDF Library client,
		   then PDF Library will search the earlier locations for plug-ins.</li>
		</ul>

	*/
	ASInt32	   pluginDirListLen;

	/**
		<p>An array of plug-in directories.</p>

		<p>If a PDF Library client wants PDFL to load headless PDF Library plug-ins from desired locations, the client can pass a list
		of paths in <code>pluginDirList</code>. The 2 fields <code>pluginDirList</code> and <code>pluginDirListLen</code> are similar 
		to the existing fields <code>dirList</code> and <code>listLen</code>.</p>

		<p>Clients can pass an array of plug-in directories in the field <code>pluginDirList</code>:</p>
		<ul>
			<li>On Windows, <code>pluginDirList</code> is an array of <code>ASUTF16Val</code> paths. Each path should be <code>NULL</code>-terminated.</li>
			<li>On Mac OS, the paths passed in <code>pluginDirList</code> should be <code>NULL</code>-terminated UTF-8 POSIX strings. Hence,
			  the type of <code>pluginDirList</code> on Mac is <code>char **</code>.</li>
			<li>On Unix, the paths passed in <code>pluginDirList</code> should be simple <code>NULL</code>-terminated <code>char</code> strings.</li>
	    </ul>
	*/
	char **pluginDirList;

	/**
		Specifies the number of directories listed in <code>colorProfileDirList</code>.
	*/	
	ASInt32	   colorProfileDirListLen;

	/**
		<p>An array of color profile directories.</p>

		<p>If a PDF Library client wants to search for color profiles from custom
		directories, the client can pass a list	of paths in <code>colorProfileDirList</code>.
		The 2 fields <code>colorProfileDirList</code> and <code>colorProfileDirListLen</code>
		are similar	to the existing fields <code>dirList</code> and <code>listLen</code>.</p>

		<p>If the client does not set this parameter then PDF Library will search
		for the color profiles from default locations such as Creative Suites and
		Acrobat	installation directories. If the clients sets this parameter then
		PDF Library will search for the color profiles from it's default directory
		as	well as from the custom directories specified by the client through
		parameter <code>colorProfileDirList</code>. However the order of searching for
		these profiles from default and custom directories is not fixed.</p>
		<ul>
			<li>On Windows, <code>colorProfileDirList</code> is an array of <code>ASUTF16Val</code> paths.
			  Each path should be <code>NULL</code>-terminated.</li>
			<li>On Mac OS, the paths passed in <code>colorProfileDirList</code> should be <code>NULL</code>-terminated
			  UTF-8 POSIX strings. Hence, the type of <code>colorProfileDirList</code> on Mac is <code>char **</code>.</li>
			<li>On Unix, the paths passed in <code>colorProfileDirList</code> should be simple <code>NULL</code>-terminated
			  <code>char</code> strings.</li>
	    </ul>

	*/	
	char **colorProfileDirList;

} PDFLDataRec, *PDFLData;
#else
typedef pthread_key_t ThreadLocalKey;

/** PDFLData structure. */
typedef struct _t_PDFLData {

	/** The size of the data structure. It must be set to <code>sizeof(PDFLDataRec)</code>. */
	ASSize_t size;	

	/** Currently <code>kPDFLInitIgnoreDefaultDirectories</code> and <code>kDontLoadPlugIns</code> flags are supported.
		When <code>kPDFLInitIgnoreDefaultDirectories</code> flag is set, the initialization process does not
		search through the default font directories(currently Adobe font directories installed by some Adobe
		applications), but only in those specified in <code>dirList</code>.
		When <code>kDontLoadPlugIns</code> flag is set, plug-ins are ignored during initialization process.
	*/
	ASUns32 flags;

	/** A list of directories with fonts. 
	
		@example For Windows:
		<p><code>libData.listLen=3L;</code></p>
		<p><code>libData.dirList=(ASPathName *)malloc(sizeof(ASPathName)*3);</code></p>
		<p><code>libData.dirList[0]=(ASPathName)"c:\\PSFonts";</code></p>
		<p><code>libData.dirList[1]=(ASPathName)"c:\\Fonts";</code></p>
		<p><code>libData.dirList[2]=(ASPathName)"c:\\CMap";</code></p>
		
	*/
	char **dirList;

	/** The number of directories listed in <code>dirList</code>.
	*/
	ASInt32 listLen;

	/** The structure containing function pointers for memory allocation
		callbacks. The library manages its own memory. Most applications
		linking with the library should pass <code>NULL</code> for this member. Applications
		wishing more control over memory allocation can fill out this structure
		and will be responsible for providing and freeing memory, as well as
		reporting available memory as requested by the library.
	*/
	TKAllocatorProcsP allocator;

	/** The structure containing function pointers for resource management
		callbacks. The library manages its own resources. Most applications
		linking with the library should pass <code>NULL</code> for this member. Applications
		wishing more control over resource allocation can fill out this structure
		and will be responsible for providing and freeing resources requested
		by the library.
	*/
	TKResourceProcsP resProcs;

	/** The structure containing function pointers for ASExtension callbacks
		used by the PDF Library.
	*/
	TKExtensionProcsP extProcs;

	/** A list of font cache file names. It is parallel to <code>dirList</code>, but contains
		separately configurable target files for CoolType cache files for
		use in cases where the specified font directory may not be writable
		by the client. Either <code>cacheFileList</code> or any/all of its subsidiary
		pointers may be <code>NULL</code>, in which case CoolType will default to the
		font directory passed in <code>dirList</code>.
	*/
	char **cacheFileList;

	/** Separately configurable target file where CoolType should store the
		system font cache file. It may be <code>NULL</code>, in which case CoolType will
		choose a default path.
	*/
	char *systemCacheFile;

	/** Separately configurable target file where CoolType should store the
		common font cache file. It may be <code>NULL</code>, in which case CoolType will
		choose a default path.
	*/
	char *commonCacheFile;

	/** The complete, <code>NULL</code>-terminated path of the directory in which the Unicode mapping files
		can be found.
	*/
	char *unicodeDirectory;

	/** The complete <code>NULL</code>-terminated path of the directory where CoolType can
	find CMap files. This directory will be the preferred location for
	CMaps that are associated with the system fontset. If CMaps with the
	same name are found in this directory and common fonts
	directory, the copy in this directory will be preferred. May be 
	<code>NULL</code>, in which case CoolType will only associate CMaps present in
	common fonts directory with the system font set.
	*/
	char *cMapDirectory;

	/*

	If a PDFL client wants PDFL to load headless PDFL plug-ins from desired locations, then it can pass a list
	of paths in pluginDirList. The 2 fields pluginDirList and pluginDirListLen are similar to existing fields
	dirList and listLen.

		pluginDirList - Clients can pass an array of plug-in directories in the field pluginDirList. 		        
		  1. On Win, pluginDirList is an array of ASUTF16Val paths. Each path should be NULL terminated.
		  2. On Mac, the paths passed in pluginDirList should be NULL terminated UTF-8 POSIX strings. Hence,
		  the type of pluginDirList on Mac is char **.
		  3. On Unix, the paths passed in pluginDirList should be simple NULL terminated char strings.

		pluginDirListLen - specifies the number of directories listed in pluginDirList.

    With regards to loading plug-ins, PDFL behavior now will be as follows: 
	
	1. If pluginDirListLen > 0, then PDFL will only look for plug-ins in the paths passed in pluginDirList.
	   It will not search the earlier locations, i.e., "pdfplug_ins" folder on Win, "PDFPlug-Ins" on Mac 
	   ,and the list of directories passed in dirList on Unix.  
	2. If pluginDirListLen <= 0 or is not a part of the PDFLDataRec structure passed by PDFL client,
	   then PDFL will search the earlier locations for plug-ins.

	*/

	ASInt32	   pluginDirListLen;

	/**
		<p>An array of plug-in directories.</p>

		<p>If a PDF Library client wants PDFL to load headless PDF Library plug-ins from desired locations, the client can pass a list
		of paths in <code>pluginDirList</code>. The 2 fields <code>pluginDirList</code> and <code>pluginDirListLen</code> are similar 
		to the existing fields <code>dirList</code> and <code>listLen</code>.</p>

		<p>Clients can pass an array of plug-in directories in the field <code>pluginDirList</code>:</p>
		<ul>
			<li>On Windows, <code>pluginDirList</code> is an array of <code>ASUTF16Val</code> paths. Each path should be <code>NULL</code>-terminated.</li>
			<li>On Mac OS, the paths passed in <code>pluginDirList</code> should be <code>NULL</code>-terminated UTF-8 POSIX strings. Hence,
			  the type of <code>pluginDirList</code> on Mac is <code>char **</code>.</li>
			<li>On Unix, the paths passed in <code>pluginDirList</code> should be simple <code>NULL</code>-terminated <code>char</code> strings.</li>
	    </ul>
	*/
	char **pluginDirList;

	/**
		Specifies the number of directories listed in <code>colorProfileDirList</code>.
	*/	
	ASInt32	   colorProfileDirListLen;

	/**
		<p>An array of color profile directories.</p>

		<p>If a PDF Library client wants to search for color profiles from custom
		directories, the client can pass a list	of paths in <code>colorProfileDirList</code>.
		The 2 fields <code>colorProfileDirList</code> and <code>colorProfileDirListLen</code>
		are similar	to the existing fields <code>dirList</code> and <code>listLen</code>.</p>

		<p>If the client does not set this parameter then PDF Library will search
		for the color profiles from default locations such as Creative Suites and
		Acrobat	installation directories. If the clients sets this parameter then
		PDF Library will search for the color profiles from it's default directory
		as	well as from the custom directories specified by the client through
		parameter <code>colorProfileDirList</code>. However the order of searching for
		these profiles from default and custom directories is not fixed.</p>
		<ul>
			<li>On Windows, <code>colorProfileDirList</code> is an array of <code>ASUTF16Val</code> paths.
			  Each path should be <code>NULL</code>-terminated.</li>
			<li>On Mac OS, the paths passed in <code>colorProfileDirList</code> should be <code>NULL</code>-terminated
			  UTF-8 POSIX strings. Hence, the type of <code>colorProfileDirList</code> on Mac is <code>char **</code>.</li>
			<li>On Unix, the paths passed in <code>colorProfileDirList</code> should be simple <code>NULL</code>-terminated
			  <code>char</code> strings.</li>
	    </ul>

	*/	
	char **colorProfileDirList;
} PDFLDataRec, *PDFLData;
#endif


/**
	Initializes the Adobe PDF Library. This method must be called 
	before any other Library calls can be made, printing or 
	otherwise. 
	@param data Initialization data for Adobe PDF Library.
	@return <code>0</code> if initialization was successful, or an error code if 
	it was not. Use <code>ASGetErrorString()</code> to convert any error code 
	to a string. 
	@see PDFLGetFlags 
	@see PDFLGetInitCount 
	@see PDFLTerm 
*/
ACEX1 ASInt32 ACEX2 PDFLInit(PDFLData data);

/**
	Terminates the Adobe PDF Library. Call this method after
	you are completely done using the library. Call this once
	to terminate and release memory used by the library. After
	the library has been shut down, the process should terminate.

	@see PDFLTerm 
*/
ACEX1 void ACEX2 PDFLTerm();

/* <code>PDFLInitHFT</code> and <code>PDFLTermHFT</code> function identically with <code>PDFLInit</code> and <code>PDFLTerm</code>
   respectively except they also initialize the HFT mechanism to allow indirect
   calls into the library. These functions are implemented in client side code.
   <code>PDFLTermHFT</code> must be used to terminate the PDF Library if it was initialized
   with <code>PDFLInitHFT</code>.*/
ASInt32 PDFLInitHFT(PDFLData data);

void PDFLTermHFT(void);

/* <code>PDFLInitThreadLocalData</code> is for the exclusive use of the Adobe supplied code
   that is compiled on the clinet when using HFTs to access the library. This
   function provides access to a block of thread specific data where the HFT
   locations are kept. */
void *PDFLInitThreadLocalData(ThreadLocalKey *key, ASUns32 size);

/**
	Gets the Core HFT. 
	@return The Core HFT object. 
	@see PDFLInit
*/
ACEX1 HFT ACEX2 PDFLGetCoreHFT(void);

#if __cplusplus
}
#endif

#endif /* _H_PDFInit */

