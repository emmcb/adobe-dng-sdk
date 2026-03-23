#ifndef __XMPFiles_Impl_hpp__
#define __XMPFiles_Impl_hpp__	1

// =================================================================================================
// Copyright Adobe
// Copyright 2004 Adobe
// All Rights Reserved
//
// NOTICE: Adobe permits you to use, modify, and distribute this file in accordance with the terms
// of the Adobe license agreement accompanying it. If you have received this file from a source other 
// than Adobe, then your use, modification, or distribution of it requires the prior written permission
// of Adobe.
// =================================================================================================

#if AdobePrivate
// =================================================================================================
// Change history
// ==============
//
// Writers:
//  AWL Alan Lillich
//  FNO Frank Nocke
//  PKG Praveen Kumar Goyal
//  JKR Jens Krueger
//  SKP Sunil Kishor Pathak
//  ADC Amandeep Chawla
//  IJS Inder Jeet Singh
//	AB  Amit Bhatti
//
// mm-dd-yy who Description of changes, most recent on top
//
// 01-05-15	AB	5.6-f122 Provide more functionalities to Plugin( Existing XMP packet, PacketInfo, OpenFlags, Error Callback and progress notification),
//						 more standard handler access API getFileModDate,IsMetadataWritable,putXMP,getAssociatedResources.
//						 New plugin handler for MPEG4 with Exif support.
// 01-16-14 IJS 5.6-f088 Generic Handling Feature - Adding support for kXMPFiles_OpenUseGenericHandler OpenFile() flag.
// 08-13-13 IJS 5.6-f076 [3206367,3206363] Enabling Error Notifications for Bad file format exceptions in MPEG4_Handler.
// 01-11-13 IJS 5.6-f020 Added IsMetadataWritable Plugin API in the Plugin Architechture.Bumped the Plugin API to version 3.
// 12-21-12 IJS 5.6-f008 Added GetAssociatedResources and IsWritable APIs 
//
// 10-10-12 ADC 5.5-f045 Implement the internal infrastructure for XMPFiles error notifications.
// 10-09-12 SKP 5.5-f044 Refactored LocateMetadataFiles API.
// 06-08-12 JKR	5.5-f016 Introduced EnablePluginManager preprocessor define to disable plugins for iOS
// 07-05-12 AWL 5.5-f015 [2999627] Improve ID3 genre handling.

// 09-23-11 AWL 5.4-f012 Add GetFileModDate.
// 06-27-11 PKG 5.4-f001 Initial checkin of plugin architecture.
//
// 08-26-10 AWL 5.3-f006 Move the folder info functions into the Host_IO namespace.
// 08-19-10 AWL 5.3-f003 Remove all use of the LFA_* names.
// 08-17-10 AWL 5.3-f001 Integrate I/O revamp to main.
//
// 06-18-09 AWL 5.0-f046 Minor tweaks from code review.
// 06-11-09 AWL 5.0-f043 Finish threading revamp, implement friendly reader/writer locking.
// 05-27-09 AWL 5.0-f041 Remove XMPMeta::SendAssertNotify.
// 05-21-09 AWL 5.0-f040 Revamp glue again to pass SetClientString with each function.
// 05-19-09 AWL 5.0-f039 First part of threading improvements, revamp the client glue.
// 05-14-09 AWL 5.0-f038 Improve the call tracing mechanism.
// 04-03-09 AWL 5.0-f032 Fix Terminate to be tolerant of too many calls (count going negative).
// 11-13-08 AWL 5.0-f004 Add server mode support that ignores local text. Enable all handlers except
//				MOV for generic UNIX - that will be handled as part of the rewrite.
//
// 10-xx-08 AWL 4.4-f0xx MWG compliance changes, remove native thumbnail support.
//
// 04-11-08 FNO 4.2.2-f104 [1720135] Integrate large ucf support. LFA_Routines to source/LargeFileAccess.
//
// 02-15-08 AWL 4.2-f075 Integrate more folder-oriented handler updates. Initial changes to create
//				generic UNIX builds for XMPFiles.
// 02-05-08 AWL 4.2-f068 Use client memory routines for malloc/free also, so that leak checking works.
// 01-22-08 FNO 4.2-f061 [1580732] XMPFiles should not write XMP into Camera Raw files.
// 01-22-08 AWL 4.2-f060 Fix SplitLeafName to tolerate partial paths.
// 01-21-08 AWL 4.2-f058 Global tweak to have the handler's packetInfo always refer to the file.
// 12-06-07 AWL 4.2-f043 Add basic XDCAM handler.
// 11-28-07 AWL 4.2-f039 Add folder utils and implement TryFolderHandlers.
// 11-16-07 AWL 4.2-f037 Introduce basic infrastructure for "directory oriented" handlers.
// 11-13-07 FNO 4.2-f034 Made XIO::Move() more flexible to support same and different file as target.
// 11-08-07 FNO 4.2-f033 Added XMP_Validate() macro.
// 10-25-07 FNO 4.2-f031 Added XIO::Move() to move data within a file.
// 10-16-07 FNO 4.2-f028 UCF read-only in NewHandlers(not final), added LFA_Read* convenience routines to XMPFiles
// 10-10-07 AWL 4.2-f027 Add handler method GetSerializeOptions. Introduce FillPacketInfo to replace
//				GetPacketCharForm, GetPacketRWMode, and GetPacketPadSize. And tolerate no wrapper.
// 07-17-07 AWL 4.2-f025 Change Mac to use pthread synchronization, needed for 64-bit builds.
// 01-26-07 AWL 4.2-f013 [1471566] Repair the lost use of XMPMeta::SendAssertNotify for XMPFiles asserts.
//
// 11-28-06 AWL 4.1-f077 [1378220] Add conditional code to gather top level performance data.
// 11-09-06 AWL 4.1-f064 [1417587] Fix memory leaks associated with IPTC_Support's DisposeLooseValue.
//				It was being called for zero length values whose address was past the allocation.
//				Add hooks to find sources of leaks again in the future.
// 11-08-06 AWL 4.1-f062 Consolidate the endian conversion routines in EndianUtils.hpp.
// 11-01-06 AWL 4.1-f057 Move include of XMPFiles_ChangeLog.hpp from XMPFiles_Impl.hpp to XMPFiles.cpp.
//				This stops the full rebuilds from each update to the change log.
// 10-12-06 AWL 4.1-f041 [1235816] Use the new/delete from XMPCore in DLL builds.
// 09-15-06 AWL 4.1-f035 Revamp the MPEG handler to be platform neutral.
// 09-14-06 AWL Add MakeUns64XX routines. (No build number increment.)
//
// 07-10-06 AWL 4.0-f014 Initial version of new read-write JPEG handler and underpinnings. Reasonably
//				but not thoroughly tested, still within NewHandlers conditional.
// 06-16-06 AWL 4.0-f012 Make XMP_ENTER_WRAPPER check sXMPFilesInitCount in debug builds.
// 05-19-06 AWL 4.0-f007 Package I/O buffer info into a struct, change I/O macros to functions.
//				This will make it reasonable to pass the buffer state among layered functions.
// 05-18-06 AWL 4.0-f005 Add support for JPEG/Exif thumbnails.
// 05-01-06 AWL 4.0-f003 Revamp the embedded, Mac PList, and Windows Property info.
// 03-24-06 AWL 4.0-f001 Adapt for move to ham-perforce, integrate XMPFiles, bump version to 4.
//
// 11-07-05 AWL 1.3-008 Fix converting form of GetUns32LE and GetUns16LE.
// 08-22-05 AWL 1.3-002 Use XMP's mem procs.
//
// 04-13-05 AWL 1.1-052 Add tweaks to the CheckProc needed by the MDKit-based JPEG/TIFF/PSD handler.
//
// 02-07-05 AWL 1.0-046 [1137383] Add LFA_Create, LFA_Rename, and LFA_Delete. Don't use C stdio routines,
//				they don't handle UTF-8 properly on Windows!
// 01-26-05 AWL 1.0-040 [1142011] Bump copyright to 2005.
// 01-26-05 AWL 1.0-039 [1141684] Use XMPMeta::SendAssertNotify for XMPFiles asserts.
// 01-06-05 AWL 1.0-027 [1014854] Cleanup handling for UTF-16 and UTF-32. This involves some general
//				changes to the separation of work between the common code and handlers, making the
//				storage of the SXMPMeta object and packet string more visible.
// 12-15-04 AWL 1.0-021 [1088863] Improve crash-safe update.
// 10-08-04 AWL 1.0-010 Add thread locking.
// 10-07-04 AWL 1.0-009 Add first cut of support for crash safe file update.
// 09-23-04 AWL 1.0-007 Use the XMP toolkit's build number and timestamp. Add current position
//				result to LFA_Seek. Add SetAbortProc and abort checking.
//
// 08-13-04 AWL Checkpoint, almost full implementation, compiles.
// 07-09-04 AWL First draft.
//
// =================================================================================================
#endif // AdobePrivate

#include "public/include/XMP_Environment.h"	// ! Must be the first #include!
#include "public/include/XMP_Const.h"
#include "build/XMP_BuildInfo.h"
#include "source/XMP_LibUtils.hpp"
#include "source/EndianUtils.hpp"

#include <string>
#define TXMP_STRING_TYPE std::string
#define XMP_INCLUDE_XMPFILES 1
#include "public/include/XMP.hpp"

#include "XMPFiles/source/XMPFiles.hpp"
#include "public/include/XMP_IO.hpp"
#include "source/Host_IO.hpp"

#include <vector>
#include <map>
#include <cassert>
#include <cstring>
#include <cstdlib>
#include <cstdio>

#if XMP_WinBuild

	#define snprintf _snprintf

#else

	#if XMP_MacBuild
		#include <CoreServices/CoreServices.h>
	#endif
	
	// POSIX headers for both Mac and generic UNIX.
	#include <fcntl.h>
	#include <unistd.h>
	#include <dirent.h>
	#include <sys/stat.h>
	#include <sys/types.h>

#endif

// =================================================================================================
// General global variables and macros
// ===================================

typedef std::vector<XMP_Uns8> RawDataBlock;

extern bool ignoreLocalText;

#if AdobePrivate

	#ifndef XMP_StaticBuild
		#define XMP_StaticBuild 0
	#endif
	
	// This can not be enables as it is for cr_sdk/Imagecore clients as of current
	// (15.3) release. This has to do with the fact that 'EnableMiscHandlers'
	// controls PNG support which is required by cr_sdk/Imagecore clients.
	// 'EnablePacketScanning' requires 'EnablePacketScanning` flag and hence both
	// need to be enabled.
	// Moreover, 'EnableDynamicMediaHandlers' controls HEIF metadata support which
	// again is required by current cr_sdk/Imagecore clients.
	// Ideally, it would help to expose finer control over the file format support in
	// XMPFiles but then the gains in terms of final cr_sdk/Imagecore library size
	// is few hundred KBs which is interesting only for some of our mobile clients
	// and cr_web.
	// One of the workflows to test format specific metadata support through XMPFiles
	// is in-place metadata updates in ACR e.g. open a PNG file, make a change, and then
	// run 'Export Settings to XMP' from the three-dot menu.
	// krishnas - Feb 20, 2023
	
	#ifndef XMPFilesForACR	// ACR should change this to define XMPFilesForACR as 1.
		#define XMPFilesForACR 0
	#endif

	#if XMPFilesForACR
		#define EnablePhotoHandlers 1
		#define EnableDynamicMediaHandlers 0
		#define EnableMiscHandlers 0
		#define EnablePacketScanning 0
	#endif

	#ifndef EnableGenericHandling
		#define EnableGenericHandling 1
	#endif
	#define GenericHandlingAlwaysOn 0
#endif

#ifndef EnablePhotoHandlers
	#define EnablePhotoHandlers 1
#endif

#ifndef EnableDynamicMediaHandlers
	#define EnableDynamicMediaHandlers 1
#endif

#ifndef EnableMiscHandlers
	#define EnableMiscHandlers 1
#endif

#ifndef EnablePacketScanning
	#define EnablePacketScanning 1
#endif

#ifndef EnablePluginManager
	#if (XMP_iOSBuild) || (XMP_UWP)
		#define EnablePluginManager 0
	#else
		#define EnablePluginManager 1
	#endif
#endif

extern XMP_Int32 sXMPFilesInitCount;

#if AdobePrivate
	#if ! XMP_StaticBuild
		extern XMP_AllocateProc sXMP_MemAlloc;
		extern XMP_DeleteProc   sXMP_MemFree;
		#define malloc(size) (*sXMP_MemAlloc) ( size )
		#define free(addr)   (*sXMP_MemFree) ( addr )
	#endif
#endif

#ifndef GatherPerformanceData
	#define GatherPerformanceData 0
#endif

#if ! GatherPerformanceData

	#define StartPerfCheck(proc,info)	/* do nothing */
	#define EndPerfCheck(proc)	/* do nothing */

#else

	#include "PerfUtils.hpp"
	
	enum {
		kAPIPerf_OpenFile,
		kAPIPerf_CloseFile,
		kAPIPerf_GetXMP,
		kAPIPerf_PutXMP,
		kAPIPerf_CanPutXMP,
		kAPIPerfProcCount	// Last, count of the procs.
	};
	
	static const char* kAPIPerfNames[] =
	{ "OpenFile", "CloseFile", "GetXMP", "PutXMP", "CanPutXMP", 0 };
	
	struct APIPerfItem {
		XMP_Uns8    whichProc;
		double      elapsedTime;
		XMPFilesRef xmpFilesRef;
		std::string extraInfo;
		APIPerfItem ( XMP_Uns8 proc, double time, XMPFilesRef ref, const char * info )
			: whichProc(proc), elapsedTime(time), xmpFilesRef(ref), extraInfo(info) {};
	};
	
	typedef std::vector<APIPerfItem> APIPerfCollection;
	
	extern APIPerfCollection* sAPIPerf;
	
	#define StartPerfCheck(proc,info)	\
		sAPIPerf->push_back ( APIPerfItem ( proc, 0.0, xmpFilesRef, info ) );	\
		APIPerfItem & thisPerf = sAPIPerf->back();								\
		PerfUtils::MomentValue startTime, endTime;								\
		try {																	\
		startTime = PerfUtils::NoteThisMoment();
	
	#define EndPerfCheck(proc)	\
		endTime = PerfUtils::NoteThisMoment();										\
		thisPerf.elapsedTime = PerfUtils::GetElapsedSeconds ( startTime, endTime );	\
	} catch ( ... ) {																\
		endTime = PerfUtils::NoteThisMoment();										\
		thisPerf.elapsedTime = PerfUtils::GetElapsedSeconds ( startTime, endTime );	\
		thisPerf.extraInfo += "  ** THROW **";										\
		throw;																		\
	}

#endif

// **** See CTECHXMP-4169947 *****

//extern XMP_FileFormat voidFileFormat;	// Used as sink for unwanted output parameters.
//extern XMP_PacketInfo voidPacketInfo;
//extern void *         voidVoidPtr;
//extern XMP_StringPtr  voidStringPtr;
//extern XMP_StringLen  voidStringLen;
//extern XMP_OptionBits voidOptionBits;


static const XMP_Uns8 * kUTF8_PacketStart = (const XMP_Uns8 *) "<?xpacket begin=";
static const XMP_Uns8 * kUTF8_PacketID    = (const XMP_Uns8 *) "W5M0MpCehiHzreSzNTczkc9d";
static const size_t     kUTF8_PacketHeaderLen = 51;	// ! strlen ( "<?xpacket begin='xxx' id='W5M0MpCehiHzreSzNTczkc9d'" )

static const XMP_Uns8 * kUTF8_PacketTrailer    = (const XMP_Uns8 *) "<?xpacket end=\"w\"?>";
static const size_t     kUTF8_PacketTrailerLen = 19;	// ! strlen ( kUTF8_PacketTrailer )

struct FileExtMapping {
	XMP_StringPtr  ext;
	XMP_FileFormat format;
};

extern const FileExtMapping kFileExtMap[];
extern const char * kKnownScannedFiles[];
extern const char * kKnownRejectedFiles[];

class CharStarLess {	// Comparison object for the genre code maps.
public:
	bool operator() ( const char * left, const char * right ) const {
		int order = strcmp ( left, right );
		return order < 0;
	}
};

typedef std::map < const char *, const char *, CharStarLess >		ID3GenreMap;
extern ID3GenreMap* kMapID3GenreCodeToName;	// Storage defined in ID3_Support.cpp.
extern ID3GenreMap* kMapID3GenreNameToCode;

#define Uns8Ptr(p) ((XMP_Uns8 *) (p))

#define IsNewline( ch )    ( ((ch) == kLF) || ((ch) == kCR) )
#define IsSpaceOrTab( ch ) ( ((ch) == ' ') || ((ch) == kTab) )
#define IsWhitespace( ch ) ( IsSpaceOrTab ( ch ) || IsNewline ( ch ) )

static inline void MakeLowerCase ( std::string * str )
{
	for ( size_t i = 0, limit = str->size(); i < limit; ++i ) {
		char ch = (*str)[i];
		if ( ('A' <= ch) && (ch <= 'Z') ) (*str)[i] += 0x20;
	}
}

static inline void MakeUpperCase ( std::string * str )
{
	for ( size_t i = 0, limit = str->size(); i < limit; ++i ) {
		char ch = (*str)[i];
		if ( ('a' <= ch) && (ch <= 'z') ) (*str)[i] -= 0x20;
	}
}

#define XMP_LitMatch(s,l)		(strcmp((s),(l)) == 0)
#define XMP_LitNMatch(s,l,n)	(strncmp((s),(l),(n)) == 0)

#if AdobePrivate
struct AlbumArt
{
	XMP_BinaryData imageData;

	XMP_Uns32 imageDataLength;

	XMP_Uns8 usageType;

	XMP_Uns8 formatType;

	XMP_StringPtr description;

	XMP_StringLen descLength;

	XMP_Uns8 encodingType;

	AlbumArt() : imageData( NULL ), imageDataLength( 0 ), usageType( kXMPFiles_AlbumArt_Use_None ), formatType( kXMPFiles_AlbumArt_NoFormat ),
		description( NULL ), descLength( 0 ), encodingType( kXMPFiles_AlbumArt_Enc_Unknown ) {}
};
#endif

// =================================================================================================
// Support for call tracing
// ========================

#ifndef XMP_TraceFilesCalls
	#define XMP_TraceFilesCalls			0
	#define XMP_TraceFilesCallsToFile	0
#endif

#if XMP_TraceFilesCalls

	#undef AnnounceThrow
	#undef AnnounceCatch
	
	#undef AnnounceEntry
	#undef AnnounceNoLock
	#undef AnnounceExit
	
	extern FILE * xmpFilesLog;
	
	#define AnnounceThrow(msg)	\
		fprintf ( xmpFilesLog, "XMP_Throw: %s\n", msg ); fflush ( xmpFilesLog )
	#define AnnounceCatch(msg)	\
		fprintf ( xmpFilesLog, "Catch in %s: %s\n", procName, msg ); fflush ( xmpFilesLog )
	
	#define AnnounceEntry(proc)			\
		const char * procName = proc;	\
		fprintf ( xmpFilesLog, "Entering %s\n", procName ); fflush ( xmpFilesLog )
	#define AnnounceNoLock(proc)		\
		const char * procName = proc;	\
		fprintf ( xmpFilesLog, "Entering %s (no lock)\n", procName ); fflush ( xmpFilesLog )
	#define AnnounceExit()	\
		fprintf ( xmpFilesLog, "Exiting %s\n", procName ); fflush ( xmpFilesLog )

#endif

// =================================================================================================
// Support for memory leak tracking
// ================================

#ifndef TrackMallocAndFree
	#define TrackMallocAndFree 0
#endif

#if TrackMallocAndFree

	static void* ChattyMalloc ( size_t size )
	{
		void* ptr = malloc ( size );
		fprintf ( stderr, "Malloc %d bytes @ %.8X\n", size, ptr );
		return ptr;
	}
	
	static void ChattyFree ( void* ptr )
	{
		fprintf ( stderr, "Free @ %.8X\n", ptr );
		free ( ptr );
	}
	
	#define malloc(s) ChattyMalloc ( s )
	#define free(p) ChattyFree ( p )

#endif

// =================================================================================================
// FileHandler declarations
// ========================

extern void ReadXMPPacket ( XMPFileHandler * handler );

extern void FillPacketInfo ( const XMP_VarString & packet, XMP_PacketInfo * info );

class XMPFileHandler {	// See XMPFiles.hpp for usage notes.
public:

#define DefaultCTorPresets							\
	handlerFlags(0), stdCharForm(kXMP_CharUnknown),	\
	containsXMP(false), processedXMP(false), needsUpdate(false), needsArtUpdate (false)

	XMPFileHandler() : parent(0), DefaultCTorPresets {};
	XMPFileHandler (XMPFiles * _parent) : parent(_parent), DefaultCTorPresets
	{
		xmpObj.SetErrorCallback(ErrorCallbackForXMPMeta, &parent->errorCallback);
	};

	virtual ~XMPFileHandler() {};	// ! The specific handler is responsible for tnailInfo.tnailImage or AlbumArt.
	
	virtual bool GetFileModDate ( XMP_DateTime * modDate );	// The default implementation is for embedding handlers.
	virtual void FillMetadataFiles ( std::vector<std::string> * metadataFiles );
	virtual void FillAssociatedResources ( std::vector<std::string> * resourceList );
	virtual bool IsMetadataWritable ( );

	virtual void CacheFileData() = 0;
	virtual void ProcessXMP();		// The default implementation just parses the XMP.
#if AdobePrivate
	virtual bool GetAlbumArtworks();	// The default implementation will return false without exception
#endif
	virtual XMP_OptionBits GetSerializeOptions();	// The default is compact.

	virtual void UpdateFile ( bool doSafeUpdate ) = 0;
	virtual void WriteTempFile ( XMP_IO* tempRef ) = 0;

	// Currently, FileHandleInstance needs to implement Error and progress callback because of plugins,
	// Rest handlers need not to implement them because handlers can access them parent
	virtual void SetErrorCallback ( ErrorCallbackBox errorCallbackBox ) {}		
	virtual void SetProgressCallback ( XMP_ProgressTracker::CallbackInfo * progCBInfoPtr ) {}


	static void NotifyClient(GenericErrorCallback * errCBptr, XMP_ErrorSeverity severity, XMP_Error & error);

	// ! Leave the data members public so common code can see them.

	XMPFiles *     parent;			// Let's the handler see the file info.
	XMP_OptionBits handlerFlags;	// Capabilities of this handler.
	XMP_Uns8       stdCharForm;		// The standard character form for output.

	bool containsXMP;		// True if the file has XMP or PutXMP has been called.
	bool processedXMP;		// True if the XMP is parsed and reconciled.
	bool needsUpdate;		// True if the file needs to be updated.
	
	bool needsArtUpdate;	// True if Album arts need to be updated.

	XMP_PacketInfo			packetInfo;	// ! This is always info about the packet in the file, if any!
	std::string				xmpPacket;	// ! This is the current XMP, updated by XMPFiles::PutXMP.
	SXMPMeta				xmpObj;
#if AdobePrivate
	std::vector<AlbumArt>	albumArtVector;	// Vector to get and update album arts in a file. Could be embedded into the specific handler.
											// Will be taken care later.
#endif
};	// XMPFileHandler

typedef XMPFileHandler * (* XMPFileHandlerCTor) ( XMPFiles * parent );

typedef bool (* CheckFileFormatProc ) ( XMP_FileFormat format,
									   XMP_StringPtr  filePath,
									   XMP_IO *       fileRef,
									   XMPFiles *     parent );

typedef bool (*CheckFolderFormatProc ) ( XMP_FileFormat format,
										const std::string & rootPath,
										const std::string & gpName,
										const std::string & parentName,
										const std::string & leafName,
										XMPFiles * parent );

// =================================================================================================

// -------------------------------------------------------------------------------------------------

static inline bool CheckBytes ( const void * left, const void * right, size_t length )
{
	return (memcmp ( left, right, length ) == 0);
}

// -------------------------------------------------------------------------------------------------

static inline bool CheckCString ( const void * left, const void * right )
{
	return (strcmp ( (char*)left, (char*)right ) == 0);
}

#endif /* __XMPFiles_Impl_hpp__ */
