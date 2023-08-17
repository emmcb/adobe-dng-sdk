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
//	AWL Alan Lillich
//	FNO Frank Nocke
//	PKA Patrick Kamin
//	ERJ Eric Juteau
//	JRS Jerry Scoggins
//	SDS Sankar Dey Sarkar
//	JEH Joerg Ehrlich
//	PW  Patrick Wollek
//	PKG Praveen Kumar Goyal
//	JKR Jens Krueger
//	IJS Inder Jeet Singh
//	ADC Amandeep Chawla
//	SKP Sunil Kishor Pathak
//	HK	Honey Kansal
//	ANS	Ankita Sharma
//	AB  Amit Bhatti
//
// mm-dd-yy who Description of changes, most recent on top
//
// 02-02-15 ADC 5.6-f128 Porting XMPFiles to gcc 4.8.x on Linux Platform.
// 01-30-15 SKP 5.6-f126  Fixed linux build failure on releng ( warning: variable �a� set but not used [-Wunused-but-set-variable]
//						  #define XMP_FILES_STATIC_START try { int a;  & similar erros) on Linux gcc4.8.2 compiler.
// 01-05-15	AB	5.6-f122 Provide more functionalities to Plugin( Existing XMP packet, PacketInfo, OpenFlags, Error Callback and progress notification),
//						 more standard handler access API getFileModDate,IsMetadataWritable,putXMP,getAssociatedResources.
//						 New plugin handler for MPEG4 with Exif support.
// 10-21-14 AB  5.6.f120 [3836549] Making kXMPFiles_OptimizeFileLayout flag public for XMP SDK.
// 08-26-14 AB  5.6-f116 [3812072] Generic Handler Bugfix.
// 05-06-14 ANS 5.6-f106 [3745465] Re-setting handler to 0 in exception scenario in CloseFile() to avoid crash in disk full conditions
// 02-06-14 IJS 5.6-f094 Excluded Files will not open with  GetAssociatedResources/IsMetadataWritable/GetFileModDate APIs
// 02-06-14 IJS 5.6-f093 Change the behavior of flag combination kXMPFiles_ForceGivenHandler and 
//                       kXMPFiles_OpenUseGenericHandler for GetAssociatedResources/IsMetadataWritable/GetFileModDate APIs
// 01-31-14 IJS 5.6-f091 [3696309, 3693612,3700229]Fix bugs for Generic Handling
//							Reverting Changes Done in 5.6-f089
// 01-24-14 HK  5.6-f090 [3696826] Generic Handler: If use GenericHandler flag is present in GetAssociatedResources/IsMetadataWritable/GetFileModDate APIs, 
//						then Generic Handler is always used even if Smart Handler is present for that format.
// 01-20-14 ADC 5.6-f089 [3696245] Generic Handler: Generic handler returns kXMP_Unknown type for handler->format.	
// 01-16-14 IJS 5.6-f088 Generic Handling Feature - Adding support for kXMPFiles_OpenUseGenericHandler OpenFile() flag.
// 09-05-13 IJS 5.6-f077 [3611478] MP4/QT files- Differentiate incomplete box data from Garbage and on repair, truncate only if it is Garbage.
// 07-22-13 ADC 5.6-f074 [3560723] For Folder based formats XMPFiles : OpenFile should throw an error when called on a file
//						 where the metadata files are read only and the option bit "OpenForUpdate" is used.
// 07-05-13 ADC 5.6-f073 In Terminate reset static variables.
// 05-13-13 ADC 5.6-f060 Removing usage of bool in APIs exposed at DLL boundaries.
// 05-06-13 IJS 5.6-f058 Fix Checkformat and GetXMPStandard  Host  implementation issues
// 04-26-13 ADC 5.6-f055 [3532883] XMP libraries provide mechanisms to clients for registration of memory allocation and 
//						 de-allocation function pointers which causes problems with the static std::string obejcts in the code base.
// 02-08-13 HK  5.6-f031 [3492357, 3492355] Provide client callback when GetAssociatedResources/IsMetadataWritable() API's called for "unsupported" format.
// 02-05-13 HK  5.6-f030 Fixed bugs#3477099, #3495118, #3490341, #3492358.
// 01-30-13 IJS 5.6-f029 Removed LocateMetadataFiles API from XMPFiles
// 01-15-13 IJS 5.6-f021 Change GetFileModDate to use GetAssociatedResources for all handlers.
// 01-11-13 IJS 5.6-f020 Reworked GetFileModDate, GetAssociatedResources and IsMetadataWritable APIs.
//                       Fixed Bugs 3469857 and 3473168.
// 01-07-13 IJS 5.6-f018 Changed name of API IsWritable to IsMetadataWritable.
//                       Added root Path folder as associated Resource for folder based Formats.
// 01-02-13 ADC 5.6-f012 Catching error from FillMetadataFiles and returning false.
// 12-21-12 IJS 5.6-f008 Added GetAssociatedResources and IsWritable APIs 
//
// 10-18-12 ADC 5.5-f051 [3347406] XMPFiles::OpenFile should throw an error when it is given a path to a file that is of an
//						 unsupported extension type along with the flag kXMPFiles_OpenUseSmartHandler.
// 10-16-12 ADC 5.5-f050 [3347167] XMPFiles::OpenFile() does not throw any exception when a file does not exist at the specified
//						 location and the flag 'kXMP_UnknownFile' is passed to the function.
// 10-12-12 ADC 5.5-f049 [3345417] XMPFiles::OpenFile() does not throw any error when the path to a folder is given to the function.
//						 Added support for new Error code kXMPErr_UnsupportedFileExtension.
// 10-10-12 ADC 5.5-f045 Implement the internal infrastructure for XMPFiles error notifications.
// 10-09-12 SKP 5.5-f044 Refactored LocateMetadataFiles API.
// 09-28-12 AWL 5.5-f039 Add LocateMetadataFiles.
// 09-28-12 ADC 5.5-f038 Implement public API and glue layer for XMPFiles error notifications.
// 09-20-12 IJS 5.5-f036 [3333897]Progress Notifications change to add some work ( for File Handlers
//						  incapable of kXMPFiles_CanRewrite ) inside XMPFiles::CloseFile.
// 06-22-12 AWL 5.5-f019 Add file update progress tracking to the MPEG-4 handler.
// 06-20-12 AWL 5.5-f018 Add outer layers for XMPFiles progress notifications, no handlers use it yet.
// 06-08-12 JKR 5.5-f016 Introduced EnablePluginManager preprocessor define to disable plugins for iOS
//
// 10-27-11 AWL 5.4-f030 [3007461] Improve performance of GetFileModDate.
// 09-25-11 PW  5.4-f013 Add support to access replaced file handler.
// 09-23-11 AWL 5.4-f012 Add GetFileModDate.
// 06-27-11 PKG 5.4-f001 Initial checkin of plugin architecture.
//
// 10-26-10 PW  5.3-f015 Integration from winwood.
// 09-30-10 AWL 5.3-f010 Add Canon XF handler from Peter Lee.
// 09-07-10 AWL 5.3-f008 Extend support of OpenFile with XMP_IO to all appropriate formats.
// 08-26-10 AWL 5.3-f006 Move the folder info functions into the Host_IO namespace.
// 08-19-10 AWL 5.3-f003 Remove all use of the LFA_* names.
// 08-17-10 AWL 5.3-f001 Integrate I/O revamp to main.
//
// 07-28-10	JEH	5.2-f001 Adding AIFF handler to XMPFiles.
//
// 10-21-09 AWL 5.0-f091 [1776893] Make sure temp files are deleted when exceptions get propagated.
// 10-07-09 AWL 5.0-f085 Minor tweaks for file layout optimization.
// 10-06-09 AWL 5.0-f083 Add MPEG-4 support for kXMPFiles_OptimizeFileLayout.
// 08-24-09 AWL 5.0-f070 Use Erase/ApplyTemplate instead of RemoveProperties/AppendProperties in XMPFiles::GetXMP.
// 07-10-09 FNO 5.0-f052 Move RIFF (AVI/WAV) handler from NewHandlers to regular Handlers.
// 06-23-09 AWL 5.0-c040-f049 Add hack to revert to global DLL locking.
// 06-11-09 AWL 5.0-f043 Finish threading revamp, implement friendly reader/writer locking.
// 05-21-09 AWL 5.0-f040 Revamp glue again to pass SetClientString with each function.
// 05-19-09 AWL 5.0-f039 First part of threading improvements, revamp the client glue.
// 05-14-09 AWL 5.0-f038 Improve the call tracing mechanism.
// 05-12-09 AWL 5.0-f037 Finish deprecated function removal.
// 05-07-09 FNO 5.0-f036 Start combined RIFF(AVI/WAV) Handler rewrite.
// 04-03-09 AWL 5.0-f032 Fix Terminate to be tolerant of too many calls (count going negative).
// 03-26-09 AWL 5.0-f029 More QT related fixes. Properly handle OpenStrictly for MPEG-4 and MOV.
//				FIll in remaining Mac langXyz to XMP (ISO 639) mappings. Add 0xA9 to set of OK chars
//				for unexpected top level boxes in CheckFormat. Fix byte order of ISO 'cprt' language.
//				Output iTunes metadata if it did not yet exist.
// 03-10-09 AWL 5.0-f027 Initial integration of QuickTime support into MPEG-4 handler.
// 02-06-09 FNO 5.0-c007f026 Add scannable ADOBE_INFO to VersionInfo (mostly for Linux/Unix).
// 01-29-09 AWL 5.0-f023 [1699977] Additional fix for XDCAM-FAM, don't shift leafName to uppercase.
// 01-29-09 AWL 5.0-f022 [1699977] Fixes for UNIX file case sensitivity.
// 01-21-09 AWL 5.0-f021 Fix AVI and WAV handlers for 64-bit builds.
// 01-15-09 AWL 5.0-f020 Split handlerTemp into tempPtr and tempUI32.
// 01-12-09 AWL 5.0-f019 Fix SelectSmartHandler to close file if necessary at "middle" default handler check.
// 11-17-08 FNO 5.0-f006 Move MP3 handler CS5 rewrite from NewHandlers to regular handlers.
// 11-13-08 AWL 5.0-f004 Add server mode support that ignores local text. Enable all handlers except
//				MOV for generic UNIX - that will be handled as part of the rewrite.
//
// 10-13-08 AWL 4.4-f013 Remove internals of GetThumbnail.
// 07-20-08 FNO 4.4-f009 Enabling ASF handler for SDK (no functional impact on private builds).
//
// 06-04-08 AWL 4.2.2-f122 [1813875] Fix AVI_MetaHandler::CacheFileData to delete bad forms of
//				xmpDM:startTimecode and xmpDM:altTimecode. Fix XMPFiles::OpenFile to delete the
//				handler and close the file if <handler>::CacheFileData throws an exception.
//
// 03-06-08 AWL 4.2-f093 Change the XDCAM file type constants to be explicit about all 3: FAM/SAM/EX.
// 03-03-08 AWL 4.2-f089 [1705112] Follow on hack to clean up the f087 changes to SelectSmartHandler.
// 02-28-08 AWL 4.2-f087 [1704032] Fix P2 and XDCAM code to check for NULL pointers from XML tree methods.
//				Fix SelectSmartHandler to differentiate file-oriented and folder-oriented cases, the
//				check procs have different parameters.
// 02-27-08 AWL 4.2-f086 [1699986] XMPFiles::OpenFile is sometimes opening the file more than once,
//				then closing only the last open, leaving the file open on exit.
// 02-22-08 AWL 4.2-f082 Fix XDCAM_CheckFormat to be case insensitive for PROAV. Remove the CUEUP.XML
//				check from XDCAMEX_CheckFormat. Fix handerTemp leaks for all folder-oriented handlers.
// 02-20-08 AWL 4.2-f079 Disable all handlers except JPEG for generic UNIX, even packet scanning.
//				Temporary tactic to control risk for Ozzy with Photoshop Express, to be revisited.
// 02-18-08 AWL 4.2-f077 More changes to generic UNIX builds for XMPFiles.
// 02-15-08 AWL 4.2-f075 Integrate more folder-oriented handler updates. Initial changes to create
//				generic UNIX builds for XMPFiles.
// 02-11-08 AWL 4.2-f071 Enable SXMPFiles::CheckFileFormat, the leaks were not real.
// 02-07-08 AWL 4.2-f070 Test, fix bugs, and move the advanced video handlers out of NewHandlers.
// 02-05-08 AWL 4.2-f069 Integrate latest advanced video handlers.
// 02-01-08 JEH 4.2-f067 Moved MPEG4 and FLV to regular handlers.
// 01-28-08 AWL 4.2-f062 Add public CheckFileFormat and CheckPackageFormat functions.
// 01-22-08 FNO 4.2-f061 [1580732] XMPFiles should not write XMP into Camera Raw files.
// 01-21-08 AWL 4.2-f058 Global tweak to have the handler's packetInfo always refer to the file.
// 01-08-08 FNO 4.2-f055 Add MOV_NewHandler to NewHandlers (release builds unchanged).
// 12-31-07 AWL 4.2-f051 Add initial Sony HDV handler under NewHandlers.
// 12-21-07 AWL 4.2-f050 Add initial skeleton FLV handler under NewHandlers.
// 12-11-07 FNO 4.2-f045 Moved UCF to regular handlers.
// 12-06-07 AWL 4.2-f043 Add basic XDCAM handler.
// 11-29-07 AWL 4.2-f041 First working version P2 handler in NewHandlers, XMP-only, no legacy.
// 11-28-07 AWL 4.2-f040 Introduce empty P2 handler in NewHandlers.
// 11-28-07 AWL 4.2-f039 Add folder utils and implement TryFolderHandlers.
// 11-28-07 FNO 4.2-f038 Made MOV-handler and other exclusions/changes needed for x64 windows.
// 11-16-07 AWL 4.2-f037 Introduce basic infrastructure for "directory oriented" handlers.
// 11-14-07 PKA 4.2-f036 Add multiple metadata tag support to SWF Handler. Write XMP tag at the
//				beginning of the file. SWF handler moved out of NewHandler section.
// 11-13-07 AWL 4.2-f035 Add MPEG-4 skeleton handler. Rename MPEG handler to MPEG-2.
// 11-05-07 AWL 4.2-f032 [1596344] Fix the MOV handler to presume QuickTime is available when
//				kXMPFiles_NoQuickTimeInit is passed to XMPFiles::Initialize.
// 10-10-07 AWL 4.2-f027 Add handler method GetSerializeOptions. Fix GetPacketRWMode to tolerate a
//				missing packet wrapper.
// 10-09-07 FNO 4.2-f026 Added UCF draft to NewHandlers.
// 02-13-07 AWL 4.2-f017 Centralize the Adobe IP constant.
// 01-30-07 AWL 4.2-f014 [1471832] Change the JPEG/PSD/TIFF handlers to do a forced legacy import if
//				the XMP portion won't parse.
// 01-26-07 AWL 4.2-f013 [1471566] Repair the lost use of XMPMeta::SendAssertNotify for XMPFiles asserts.
// 01-23-07 AWL 4.2-f010 Finish changing the copyright to 2007.
// 01-11-07 AWL 4.2-f008 Change copyright to 2007.
// 01-11-07 AWL 4.2-f007 [1454747] Change QuickTime init/term calls to handle main and background thread issues.
//
// 11-28-06 AWL 4.1-f077 [1378220] Add conditional code to gather top level performance data.
// 11-27-06 AWL 4.1-f074 [1428151] Fix XMPFiles::PutXMP to check for a handler before dereferencing.
// 11-08-06 AWL 4.1-f063 Delete PSIR_Support.cpp, IFF_Handler.*, disable MP4 handler.
// 11-01-06 AWL 4.1-f057 Move include of XMPFiles_ChangeLog.hpp from XMPFiles_Impl.hpp to XMPFiles.cpp.
//				This stops the full rebuilds from each update to the change log.
// 10-27-06 AWL 4.1-f050 [1408360] Have XMPFiles call the XMPCore init/term routines. The changes in
//				build 4.1-f041 added an init-time dependency. Not to mention the general dependency
//				that was always there and would break if the client didn't init both.
// 10-20-06 AWL 4.1-f047 [1403500] Add support to call Mac QuickTime properly from background threads.
// 10-12-06 AWL 4.1-f041 [1235816] Use the new/delete from XMPCore in DLL builds.
// 10-12-06 AWL 4.1-f040 [1394229] Make sure all handlers accept only "proper" files. Preferably
//				only by file content. Where we must, by "known" file extension.
// 10-10-06 AWL 4.1-f039 Adapt the MP3 handler to run on Mac.
// 10-08-06 AWL 4.1-f038 Adapt the RIFF-based handlers (AVI and WAV) to run on Mac.
// 09-15-06 AWL 4.1-f035 Revamp the MPEG handler to be platform neutral.
// 09-14-06 AWL 4.1-f034 Finish the support for XMP-only in-place updates for JPEG/TIFF/PSD.
// 09-01-06 AWL 4.1-f033 Move the new JPEG/TIFF/PSD handlers out of NewHandlers.
// 08-21-06 AWL 4.1-f028 [0946377] Add more file extensions for MPEG files.
//
// 07-31-06 AWL 4.0-f019 Make sure the source file is closed before calling LFA_Rename. MoveFileW on
//				Windows complains if the source file is open.
// 07-13-06 AWL 4.0-f016 Add a handler flag for file formats that need to have read-only packets.
// 07-13-06 AWL 4.0-f015 Fix CloseFile to properly decide if a safe update can be done. Fix
//				GetThumbnail to check the handler's kXMPFiles_ReturnsTNail flag.
// 06-01-06 AWL 4.0-f008 Activate new TIFF and Photoshop file handlers.
// 05-19-06 AWL 4.0-f006 Change XMPFiles::OpenFile to return bool, don't throw if smart handler not
//				found when kXMPFiles_OpenUseSmartHandler is passed.
// 05-18-06 AWL 4.0-f005 Add support for JPEG/Exif thumbnails.
// 05-15-06 AWL 4.0-f004 Working, but minimally tested read-only smart JPEG handler under NewHandlers.
// 05-01-06 AWL 4.0-f003 Revamp the embedded, Mac PList, and Windows Property info.
// 03-24-06 AWL 4.0-f001 Adapt for move to ham-perforce, integrate XMPFiles, bump version to 4.
//
// 01-25-06 AWL 1.3-013 Replace null iterator notion, VC8 complains about compares to default iterator.
// 01-24-06 AWL 1.3-012 Adapt for VC8.
// 09-02-08 AWL 1.3-005 Call BasicASInit and BasicASTerm for the MDKit handler.
// 09-02-05 AWL 1.3-004 Restore the MDKit handler, now using the notBIB XMP.
// 09-01-05 AWL 1.3-003 [1225858] Don't implicitly set the SafeUpdate flag for non-owning handlers.
// 08-22-05 AWL 1.3-002 Use XMP's mem procs.
// 07-21-05 AWL 1.3-001 Remove BIB, same as XMP. Create Xcode project. Bump version to 1.3.
//
// 06-03-05 AWL 1.2-060 [1157869] Make sure destructors deallocate everything and clear pointers.
//
// 04-15-05 SDS 1.1-055 Added MPEG handler.
// 04-13-05 AWL 1.1-052 Add tweaks to the CheckProc needed by the MDKit-based JPEG/TIFF/PSD handler.
//
// 02-07-05 AWL 1.0-046 [1137383] Add LFA_Create, LFA_Rename, and LFA_Delete. Don't use C stdio routines,
//				they don't handle UTF-8 properly on Windows!
// 01-26-05 AWL 1.0-038 [1137684] Add .ffx to the list of file extensions for limited packet scanning.
// 01-21-05 AWL 1.0-037 Move the "restricted by extension" test for MP3 to the right place.
// 01-21-05 AWL 1.0-036 Remove WAV from the "restricted by extension" handlers.
// 01-20-05 AWL 1.0-033 Remove AVI from the "restricted by extension" handlers, go ahead and use
//				AVI CheckFormat function.
// 01-20-05 ERJ 1.0-031 Adding MP3 support.
// 01-18-05 AWL 1.0-030 [1136246] Add ai and ait to the set of file extensions used for limited packet scanning.
// 01-14-05 AWL 1.0-029 [1134197] Add gif, png, inx, inds, inpk, incd, inct, and incx to the set of
//				file extensions used for limited packet scanning.
// 01-06-05 AWL 1.0-027 [1014854] Cleanup handling for UTF-16 and UTF-32. This involves some general
//				changes to the separation of work between the common code and handlers, making the
//				storage of the SXMPMeta object and packet string more visible. Move the AVI and WAV
//				handler installs to the "does not own file" section.
// 12-22-04 ERJ 1.0-026 Removing the same Debug_Assert(), but for the WAV_Handler.
// 12-21-04 JRS 1.0-024 Remove Debug_Assert() to verify AVI handler owns the file, as it no longer does.
// 12-21-04 AWL 1.0-022 [1067086] Add option to only packet scan files "known" to need scanning.
// 12-15-04 AWL 1.0-021 [1088863] Improve crash-safe update.
// 12-13-04 ERJ 1.0-020 Add WAV handler.
// 11-03-04 AWL 1.0-017 Add target for "new" handlers, make AVI handler be Windows-only.
// 11-01-04 AWL 1.0-016 [1100873] Fix CloseFile to close original before rename in safe save.
// 10-19-04 AWL 1.0-012 [1094742] Fix CanPutXMP to call GetFileInfo properly, and to return the
//				result from the wrapper.
// 10-08-04 AWL 1.0-011 Check can-inject flag in PutXMP.
// 10-08-04 AWL 1.0-010 Add thread locking.
// 10-07-04 AWL 1.0-009 Add first cut of support for crash safe file update.
// 09-23-04 AWL 1.0-007 Use the XMP toolkit's build number and timestamp. Add current position
//				result to LFA_Seek. Add SetAbortProc and abort checking.
// 09-22-04 AWL 1.0-006 Fix bugs in InDesign handler.
// 08-26-04 AWL 1.0-005 Fix OpenFile to use format hint or file extension.
//
// 08-25-04 AWL Add MOV and AVI handlers.
// 08-24-04 AWL Disable JPEG handler.
// 08-23-04 AWL Add GetVersion.
// 08-20-04 AWL Add handlerFlags output to GetFileInfo.
// 08-13-04 AWL Checkpoint, almost full implementation, compiles.
// 07-02-04 AWL First draft.
//
// =================================================================================================
#endif // AdobePrivate

#include "public/include/XMP_Environment.h"	// ! Must be the first #include!
#include "public/include/XMP_Const.h"
#include "public/include/XMP_IO.hpp"

#include <vector>
#include <string.h>

#include "source/UnicodeConversions.hpp"
#include "source/XMPFiles_IO.hpp"
#include "source/XIO.hpp"

#include "XMPFiles/source/XMPFiles_Impl.hpp"
#include "XMPFiles/source/HandlerRegistry.h"

#if EnablePluginManager
	#include "XMPFiles/source/PluginHandler/PluginManager.h"
#endif

#include "XMPFiles/source/FormatSupport/ID3_Support.hpp"
#include "XMPFiles/source/FormatSupport/ISOBaseMedia_Support.hpp"

#if AdobePrivate
	#include "XMPFiles/source/XMPFiles_ChangeLog.hpp"
#endif

#if EnablePacketScanning
	#include "XMPFiles/source/FileHandlers/Scanner_Handler.hpp"
#endif

#if EnableGenericHandling
	#include "XMPFiles/source/FileHandlers/Generic_Handler.hpp"
#endif

#include "XMPCore/XMPCoreDefines.h"
#if ENABLE_CPP_DOM_MODEL
	#include "XMPCore/Interfaces/IMetadata.h"
	#include "XMPCore/Interfaces/INodeIterator.h"
	#include "XMPCommon/Interfaces/IUTF8String.h"
	#include "XMPCore/Interfaces/ICoreObjectFactory.h"
#endif

// =================================================================================================
/// \file XMPFiles.cpp
/// \brief High level support to access metadata in files of interest to Adobe applications.
///
/// This header ...
///
// =================================================================================================

using namespace Common;

#if EnablePluginManager
	using namespace XMP_PLUGIN;
#endif

// =================================================================================================

XMP_Int32 sXMPFilesInitCount = 0;

static XMP_ProgressTracker::CallbackInfo sProgressDefault;
static XMPFiles::ErrorCallbackInfo sDefaultErrorCallback;


#if GatherPerformanceData
	APIPerfCollection* sAPIPerf = 0;
#endif

// These are embedded version strings.

#if XMP_DebugBuild
	#define kXMPFiles_DebugFlag 1
#else
	#define kXMPFiles_DebugFlag 0
#endif

#define kXMPFiles_VersionNumber	( (kXMPFiles_DebugFlag << 31)    |	\
                                  (XMP_API_VERSION_MAJOR << 24) |	\
						          (XMP_API_VERSION_MINOR << 16) |	\
						          (XMP_API_VERSION_MICRO << 8) )

#if AdobePrivate
	#define kXMPFilesName "Adobe XMP Files"
	#define kXMPFiles_BasicVersion   XMPFILES_API_VERSION_STRING "-f" kXMPFiles_EngString " " BUILDVERSIONSTR
	#define kXMPFiles_VersionString  kXMPFilesName " " kXMPFiles_BasicVersion
	#define kXMPFiles_VersionMessage kXMPFiles_VersionString ", " BUILDDATESTR kXMPFiles_DebugString
#else
	#define kXMPFilesName "XMP Files"
	#define kXMPFiles_VersionMessage kXMPFilesName " " XMPFILES_API_VERSION_STRING
#endif

const char * kXMPFiles_EmbeddedVersion   = kXMPFiles_VersionMessage;
const char * kXMPFiles_EmbeddedCopyright = kXMPFilesName " " kXMP_CopyrightStr;

#if AdobePrivate

	const char * kXMPFiles_EmbeddedAdobeIP = kXMP_AdobeIPStr;

	// Ugly strings wanted by CoreTech tools.
	const char * uglyFilesBuildVersion = "ADOBE_INFO BUILDVERSION : " BUILDVERSIONSTR;
	const char * uglyFilesBuildDate    = "ADOBE_INFO BUILDDATE : " BUILDDATESTR;
	const char * uglyFilesFileVersion  = "ADOBE_INFO FILEVERSION : " XMPFILES_API_VERSION_STRING "-f" kXMPFiles_EngString;
	const char * uglyFilesCopyright    = "ADOBE_INFO COPYRIGHT : " kXMP_CopyrightStr;
	const char * uglyFilesBuildId      = "ADOBE_INFO BUILDID : " BUILDIDSTR;

	#if XMP_DebugBuild
		const char * uglyFilesBuildVariant = "ADOBE_INFO BUILDVARIANT : Debug";
	#else
		const char * uglyFilesBuildVariant = "ADOBE_INFO BUILDVARIANT : Release";
	#endif

#endif

#define EMPTY_FILE_PATH ""
#define XMP_FILES_STATIC_START try { /*int a;*/
#define XMP_FILES_STATIC_END1(severity) /*a = 1;*/ } catch ( XMP_Error & error ) { sDefaultErrorCallback.NotifyClient ( (severity), error, EMPTY_FILE_PATH ); }
#define XMP_FILES_STATIC_END2(filePath, severity) /*a = 1;*/ } catch ( XMP_Error & error ) { sDefaultErrorCallback.NotifyClient ( (severity), error, (filePath) ); }
#define XMP_FILES_START try { /*int b;*/
#define XMP_FILES_END1(severity) /*b = 1;*/ } catch ( XMP_Error & error ) { errorCallback.NotifyClient ( (severity), error, this->filePath.c_str() ); }
#define XMP_FILES_END2(filePath, severity) /*b = 1;*/ } catch ( XMP_Error & error ) { errorCallback.NotifyClient ( (severity), error, (filePath) ); }
#define XMP_FILES_STATIC_NOTIFY_ERROR(errorCallbackPtr, filePath, severity, error)							\
	if ( (errorCallbackPtr) != NULL ) (errorCallbackPtr)->NotifyClient ( (severity), (error), (filePath) );


// =================================================================================================

#if EnablePacketScanning
	static XMPFileHandlerInfo kScannerHandlerInfo ( kXMP_UnknownFile, kScanner_HandlerFlags,
													(CheckFileFormatProc)0, Scanner_MetaHandlerCTor );
#endif

#if EnableGenericHandling
	static XMPFileHandlerInfo kGenericHandlerInfo ( kXMP_UnknownFile, kGeneric_HandlerFlags,
		(CheckFileFormatProc)0, Generic_MetaHandlerCTor );
#endif
// =================================================================================================

/* class-static */
void
XMPFiles::GetVersionInfo ( XMP_VersionInfo * info )
{
	XMP_FILES_STATIC_START
	memset ( info, 0, sizeof(XMP_VersionInfo) );

	info->major   = XMPFILES_API_VERSION_MAJOR;
	info->minor   = XMPFILES_API_VERSION_MINOR;
	info->micro   = 0; //no longer used
	info->isDebug = kXMPFiles_DebugFlag;
#if AdobePrivate
	info->build   = kXMPFiles_EngBuild;
#endif
	info->flags   = 0;	// ! None defined yet.
	info->message = kXMPFiles_VersionMessage;
	XMP_FILES_STATIC_END1 ( kXMPErrSev_OperationFatal )
}	// XMPFiles::GetVersionInfo

// =================================================================================================

#if XMP_TraceFilesCalls
	FILE * xmpFilesLog = stderr;
#endif

#if UseGlobalLibraryLock & (! XMP_StaticBuild )
	XMP_BasicMutex sLibraryLock;	// ! Handled in XMPMeta for static builds.
#endif

/* class static */
bool
XMPFiles::Initialize( XMP_OptionBits options, const char* pluginFolder, const char* plugins /* = NULL */ )
{
	XMP_FILES_STATIC_START
	++sXMPFilesInitCount;
	if ( sXMPFilesInitCount > 1 ) return true;

	#if XMP_TraceFilesCallsToFile
		xmpFilesLog = fopen ( "XMPFilesLog.txt", "w" );
		if ( xmpFilesLog == 0 ) xmpFilesLog = stderr;
	#endif

	#if UseGlobalLibraryLock & (! XMP_StaticBuild )
		InitializeBasicMutex ( sLibraryLock );	// ! Handled in XMPMeta for static builds.
	#endif

	SXMPMeta::Initialize();	// Just in case the client does not.

	#if AdobePrivate
		#if ! XMP_StaticBuild
			SXMPMeta::GetMemProcs ( &sXMP_MemAlloc, &sXMP_MemFree );
		#endif
	#endif

	if ( ! Initialize_LibUtils() ) return false;
	if ( ! ID3_Support::InitializeGlobals() ) return false;

	#if GatherPerformanceData
		sAPIPerf = new APIPerfCollection;
	#endif

	XMP_Uns16 endianInt  = 0x00FF;
	XMP_Uns8  endianByte = *((XMP_Uns8*)&endianInt);
	if ( kBigEndianHost ) {
		if ( endianByte != 0 ) XMP_Throw ( "Big endian flag mismatch", kXMPErr_InternalFailure );
	} else {
		if ( endianByte != 0xFF ) XMP_Throw ( "Little endian flag mismatch", kXMPErr_InternalFailure );
	}

	XMP_Assert ( kUTF8_PacketHeaderLen == strlen ( "<?xpacket begin='xxx' id='W5M0MpCehiHzreSzNTczkc9d'" ) );
	XMP_Assert ( kUTF8_PacketTrailerLen == strlen ( (const char *) kUTF8_PacketTrailer ) );

	HandlerRegistry::getInstance().initialize();

	InitializeUnicodeConversions();

	ignoreLocalText = XMP_OptionIsSet ( options, kXMPFiles_IgnoreLocalText );
	#if XMP_UNIXBuild
		if ( ! ignoreLocalText ) XMP_Throw ( "Generic UNIX clients must pass kXMPFiles_IgnoreLocalText", kXMPErr_EnforceFailure );
	#endif

	#if EnablePluginManager
		if ( pluginFolder != 0 ) {
			std::string pluginList;
			if ( plugins != 0 ) pluginList.assign ( plugins );
			PluginManager::initialize ( pluginFolder, pluginList );  // Load file handler plugins.
		}
	#endif

	#if AdobePrivate
		#if EnableNewHandlers
			// These are in-development handlers that need to own the file.
		#endif
	#endif

	// Make sure the embedded info strings are referenced and kept.
	if ( (kXMPFiles_EmbeddedVersion[0] == 0) || (kXMPFiles_EmbeddedCopyright[0] == 0) ) return false;
	#if AdobePrivate
		if ( kXMPFiles_EmbeddedAdobeIP[0] == 0 ) return false;
		if ( (uglyFilesBuildVersion[0] == 0) || (uglyFilesBuildDate[0] == 0) ||
			 (uglyFilesFileVersion[0] == 0)  || (uglyFilesCopyright[0] == 0) ||
			 (uglyFilesBuildVariant[0] == 0) || (uglyFilesBuildId[0] == 0)) return false;
	#endif

	// Verify critical type sizes.
	XMP_Assert ( sizeof(XMP_Int8) == 1 );
	XMP_Assert ( sizeof(XMP_Int16) == 2 );
	XMP_Assert ( sizeof(XMP_Int32) == 4 );
	XMP_Assert ( sizeof(XMP_Int64) == 8 );
	XMP_Assert ( sizeof(XMP_Uns8) == 1 );
	XMP_Assert ( sizeof(XMP_Uns16) == 2 );
	XMP_Assert ( sizeof(XMP_Uns32) == 4 );
	XMP_Assert ( sizeof(XMP_Uns64) == 8 );
	XMP_Assert ( sizeof(XMP_Bool) == 1 );
	XMP_FILES_STATIC_END1 ( kXMPErrSev_ProcessFatal )
	return true;

}	// XMPFiles::Initialize

// =================================================================================================

#if GatherPerformanceData

#if XMP_WinBuild
	#pragma warning ( disable : 4996 )	// '...' was declared deprecated
#endif

#include "PerfUtils.cpp"

static void ReportPerformanceData()
{
	struct SummaryInfo {
		size_t callCount;
		double totalTime;
		SummaryInfo() : callCount(0), totalTime(0.0) {};
	};

	SummaryInfo perfSummary [kAPIPerfProcCount];

	XMP_DateTime now;
	SXMPUtils::CurrentDateTime ( &now );
	std::string nowStr;
	SXMPUtils::ConvertFromDate ( now, &nowStr );

	#if XMP_WinBuild
		#define kPerfLogPath "C:\\XMPFilesPerformanceLog.txt"
	#else
		#define kPerfLogPath "/XMPFilesPerformanceLog.txt"
	#endif
	FILE * perfLog = fopen ( kPerfLogPath, "ab" );
	if ( perfLog == 0 ) return;

	fprintf ( perfLog, "\n\n// =================================================================================================\n\n" );
	fprintf ( perfLog, "XMPFiles performance data\n" );
	fprintf ( perfLog, "   %s\n", kXMPFiles_VersionMessage );
	fprintf ( perfLog, "   Reported at %s\n", nowStr.c_str() );
	fprintf ( perfLog, "   %s\n", PerfUtils::GetTimerInfo() );

	// Gather and report the summary info.

	for ( size_t i = 0; i < sAPIPerf->size(); ++i ) {
		SummaryInfo& summaryItem = perfSummary [(*sAPIPerf)[i].whichProc];
		++summaryItem.callCount;
		summaryItem.totalTime += (*sAPIPerf)[i].elapsedTime;
	}

	fprintf ( perfLog, "\nSummary data:\n" );

	for ( size_t i = 0; i < kAPIPerfProcCount; ++i ) {
		long averageTime = 0;
		if ( perfSummary[i].callCount != 0 ) {
			averageTime = (long) (((perfSummary[i].totalTime/perfSummary[i].callCount) * 1000.0*1000.0) + 0.5);
		}
		fprintf ( perfLog, "   %s, %d total calls, %d average microseconds per call\n",
				  kAPIPerfNames[i], perfSummary[i].callCount, averageTime );
	}

	fprintf ( perfLog, "\nPer-call data:\n" );

	// Report the info for each call.

	for ( size_t i = 0; i < sAPIPerf->size(); ++i ) {
		long time = (long) (((*sAPIPerf)[i].elapsedTime * 1000.0*1000.0) + 0.5);
		fprintf ( perfLog, "   %s, %d microSec, ref %.8X, \"%s\"\n",
				  kAPIPerfNames[(*sAPIPerf)[i].whichProc], time,
				  (*sAPIPerf)[i].xmpFilesRef, (*sAPIPerf)[i].extraInfo.c_str() );
	}

	fclose ( perfLog );

}	// ReportAReportPerformanceDataPIPerformance

#endif

// =================================================================================================

/* class static */
void
XMPFiles::Terminate()
{
	XMP_FILES_STATIC_START
	--sXMPFilesInitCount;
	if ( sXMPFilesInitCount != 0 ) return;	// Not ready to terminate, or already terminated.

	#if GatherPerformanceData
		ReportPerformanceData();
		EliminateGlobal ( sAPIPerf );
	#endif

	#if EnablePluginManager
		PluginManager::terminate();
	#endif

	HandlerRegistry::terminate();

	SXMPMeta::Terminate();	// Just in case the client does not.

	ID3_Support::TerminateGlobals();
	ISOMedia::TerminateGlobals();
	Terminate_LibUtils();

	#if UseGlobalLibraryLock & (! XMP_StaticBuild )
		TerminateBasicMutex ( sLibraryLock );	// ! Handled in XMPMeta for static builds.
	#endif

	#if XMP_TraceFilesCallsToFile
		if ( xmpFilesLog != stderr ) fclose ( xmpFilesLog );
		xmpFilesLog = stderr;
	#endif

	// reset static variables
	sDefaultErrorCallback.Clear();
	sProgressDefault.Clear();
	#if AdobePrivate
		#if ! XMP_StaticBuild
			//reset memory pointers to default values
			sXMP_MemAlloc = malloc;
			sXMP_MemFree = free;
		#endif
	#endif

	XMP_FILES_STATIC_END1 ( kXMPErrSev_ProcessFatal )
}	// XMPFiles::Terminate

// =================================================================================================

XMPFiles::XMPFiles()
	: clientRefs(0)
	, format(kXMP_UnknownFile)
	, ioRef(0)
	, openFlags(0)
	, handler(0)
	, tempPtr(0)
	, tempUI32(0)
	, abortProc(0)
	, abortArg(0)
	, progressTracker(0)
{
	XMP_FILES_START
	if ( sProgressDefault.clientProc != 0 ) {
		this->progressTracker = new XMP_ProgressTracker ( sProgressDefault );
		if (this->progressTracker == 0) XMP_Throw ( "XMPFiles: Unable to allocate memory for Progress Tracker", kXMPErr_NoMemory );
	}

	if ( sDefaultErrorCallback.clientProc != 0 ) {
		this->errorCallback.wrapperProc = sDefaultErrorCallback.wrapperProc;
		this->errorCallback.clientProc = sDefaultErrorCallback.clientProc;
		this->errorCallback.context = sDefaultErrorCallback.context;
		this->errorCallback.limit = sDefaultErrorCallback.limit;
	}
	XMP_FILES_END1 ( kXMPErrSev_OperationFatal );
}	// XMPFiles::XMPFiles

// =================================================================================================

static inline void CloseLocalFile ( XMPFiles* thiz )
{
	if ( thiz->UsesLocalIO() ) {

		XMPFiles_IO* localFile = (XMPFiles_IO*)thiz->ioRef;

		if ( localFile != 0 ) {
			localFile->Close();
			delete localFile;
			thiz->ioRef = 0;
		}

	}

}	// CloseLocalFile

// =================================================================================================

XMPFiles::~XMPFiles() NO_EXCEPT_FALSE
{
	XMP_FILES_START
	XMP_Assert ( this->clientRefs <= 0 );

	if ( this->handler != 0 ) {
		delete this->handler;
		this->handler = 0;
	}

	CloseLocalFile ( this );

	if ( this->progressTracker != 0 ) delete this->progressTracker;
	if ( this->tempPtr != 0 ) free ( this->tempPtr );	// ! Must have been malloc-ed!
	XMP_FILES_END1 ( kXMPErrSev_OperationFatal )

}	// XMPFiles::~XMPFiles

// =================================================================================================

/* class static */
bool
XMPFiles::GetFormatInfo ( XMP_FileFormat   format,
                          XMP_OptionBits * flags /* = 0 */ )
{
	XMP_FILES_STATIC_START
		return HandlerRegistry::getInstance().getFormatInfo ( format, flags );
	XMP_FILES_STATIC_END1 ( kXMPErrSev_OperationFatal )
		return false;

}	// XMPFiles::GetFormatInfo

// =================================================================================================

/* class static */
XMP_FileFormat
XMPFiles::CheckFileFormat ( XMP_StringPtr clientPath )
{
	XMP_FILES_STATIC_START
	if ( (clientPath == 0) || (*clientPath == 0) ) return kXMP_UnknownFile;

	XMPFiles bogus;	// Needed to provide context to SelectSmartHandler.
	bogus.SetFilePath ( clientPath ); // So that XMPFiles destructor cleans up the XMPFiles_IO object.
	XMPFileHandlerInfo * handlerInfo = HandlerRegistry::getInstance().selectSmartHandler(&bogus, clientPath, kXMP_UnknownFile, kXMPFiles_OpenForRead);

	if ( handlerInfo == 0 ) {
		if ( !Host_IO::Exists ( clientPath ) ) {
			XMP_Error error ( kXMPErr_NoFile, "XMPFiles: file does not exist" );
			XMP_FILES_STATIC_NOTIFY_ERROR ( &sDefaultErrorCallback, clientPath, kXMPErrSev_Recoverable, error );
		}
		return kXMP_UnknownFile;
	}
	return handlerInfo->format;
	XMP_FILES_STATIC_END2 ( clientPath, kXMPErrSev_OperationFatal )
	return kXMP_UnknownFile;

}	// XMPFiles::CheckFileFormat

// =================================================================================================

/* class static */
XMP_FileFormat
XMPFiles::CheckPackageFormat ( XMP_StringPtr folderPath )
{
	XMP_FILES_STATIC_START
	// This is called with a path to a folder, and checks to see if that folder is the top level of
	// a "package" that should be recognized by one of the folder-oriented handlers. The checks here
	// are not overly extensive, but hopefully enough to weed out false positives.
	//
	// Since we don't have many folder handlers, this is simple hardwired code.

	#if ! EnableDynamicMediaHandlers
		return kXMP_UnknownFile;
	#else
		Host_IO::FileMode folderMode = Host_IO::GetFileMode ( folderPath );
		if ( folderMode != Host_IO::kFMode_IsFolder ) return kXMP_UnknownFile;
		return HandlerRegistry::checkTopFolderName ( std::string ( folderPath ) );
	#endif
	XMP_FILES_STATIC_END2 ( folderPath, kXMPErrSev_OperationFatal )
	return kXMP_UnknownFile;

}	// XMPFiles::CheckPackageFormat

// =================================================================================================

static bool FileIsExcluded (
	XMP_StringPtr clientPath,
	std::string * fileExt,
	Host_IO::FileMode * clientMode,
	const XMPFiles::ErrorCallbackInfo * _errorCallbackInfoPtr = NULL )
{
	// ! Return true for excluded files, false for OK files.
	
	*clientMode = Host_IO::GetFileMode ( clientPath );

	if ( (*clientMode == Host_IO::kFMode_IsFolder) || (*clientMode == Host_IO::kFMode_IsOther) ) {
		XMP_Error error ( kXMPErr_FilePathNotAFile, "XMPFiles: path specified is not a file" );
		XMP_FILES_STATIC_NOTIFY_ERROR ( _errorCallbackInfoPtr, clientPath, kXMPErrSev_Recoverable, error );
		return true;
	}

	XMP_Assert ( (*clientMode == Host_IO::kFMode_IsFile) || (*clientMode == Host_IO::kFMode_DoesNotExist) );

	if ( *clientMode == Host_IO::kFMode_IsFile ) {

		// Find the file extension. OK to be "wrong" for something like "C:\My.dir\file". Any
		// filtering looks for matches with real extensions, "dir\file" won't match any of these.
		XMP_StringPtr extPos = clientPath + strlen ( clientPath );
		for ( ; (extPos != clientPath) && (*extPos != '.'); --extPos ) {}
		if ( *extPos == '.' ) {
			fileExt->assign ( extPos+1 );
			MakeLowerCase ( fileExt );
		}

		// See if this file is one that XMPFiles should never process.
		for ( size_t i = 0; kKnownRejectedFiles[i] != 0; ++i ) {
			if ( *fileExt == kKnownRejectedFiles[i] ) {
				XMP_Error error ( kXMPErr_RejectedFileExtension, "XMPFiles: rejected file extension specified" );
				XMP_FILES_STATIC_NOTIFY_ERROR ( _errorCallbackInfoPtr, clientPath, kXMPErrSev_Recoverable, error );
				return true;
			}
		}

	}
	
	return false;

}	// FileIsExcluded

static XMPFileHandlerInfo* CreateFileHandlerInfo (
	XMPFiles* dummyParent,
	XMP_FileFormat * format,
	XMP_OptionBits   options,
	XMP_Bool&  excluded,
	const XMPFiles::ErrorCallbackInfo * _errorCallbackInfoPtr = NULL )
{
	Host_IO::FileMode clientMode;
	std::string fileExt;	// Used to check for excluded files.
	excluded = FileIsExcluded ( dummyParent->GetFilePath().c_str(), &fileExt, &clientMode, &sDefaultErrorCallback );	// ! Fills in fileExt and clientMode.
	if ( excluded ) return 0;

	XMPFileHandlerInfo * handlerInfo  = 0;

	XMP_FileFormat dummyFormat = kXMP_UnknownFile;
	if ( format == 0 ) format = &dummyFormat;

	options |= kXMPFiles_OpenForRead;
	handlerInfo = HandlerRegistry::getInstance().selectSmartHandler ( dummyParent, dummyParent->GetFilePath().c_str(), *format, options );

	if ( handlerInfo == 0 ) {
		if ( clientMode == Host_IO::kFMode_DoesNotExist ) {
			XMP_Error error ( kXMPErr_NoFile, "XMPFiles: file does not exist" );
			XMP_FILES_STATIC_NOTIFY_ERROR ( _errorCallbackInfoPtr, dummyParent->GetFilePath().c_str(), kXMPErrSev_Recoverable, error );
		}else{
			XMP_Error error ( kXMPErr_NoFileHandler, "XMPFiles: No smart file handler available to handle file" );
			XMP_FILES_STATIC_NOTIFY_ERROR ( _errorCallbackInfoPtr, dummyParent->GetFilePath().c_str(), kXMPErrSev_Recoverable, error );
		}
		return 0;
	}
	return handlerInfo;
}

// =================================================================================================

/* class static */
bool
XMPFiles::GetFileModDate ( XMP_StringPtr    clientPath,
						   XMP_DateTime *   modDate,
						   XMP_FileFormat * format, /* = 0 */
						   XMP_OptionBits   options /* = 0 */ )
{
	XMP_FILES_STATIC_START
	// ---------------------------------------------------------------
	// First try to select a smart handler. Return false if not found.
	
	XMPFiles dummyParent;	// GetFileModDate is static, but the handler needs a parent.
	dummyParent.SetFilePath ( clientPath );
	
	
	XMPFileHandlerInfo * handlerInfo  = 0;
	XMP_Bool excluded=false;
	handlerInfo = CreateFileHandlerInfo ( &dummyParent, format, options, excluded, &sDefaultErrorCallback );
#if EnableGenericHandling
#if GenericHandlingAlwaysOn
	XMP_OptionBits oldOptions = options;
	options |= kXMPFiles_OpenUseGenericHandler;
#endif
	if (handlerInfo == 0 && !excluded
		&& (options & kXMPFiles_OpenUseGenericHandler)  )
	{
		Host_IO::FileMode fileMode = Host_IO::GetFileMode( clientPath );
		if ( fileMode == Host_IO::kFMode_DoesNotExist )
			return false;

		handlerInfo = &kGenericHandlerInfo;
	}
#if GenericHandlingAlwaysOn
	options = oldOptions;
#endif
#endif
	if ( handlerInfo == 0 ) return false;
	
	// -------------------------------------------------------------------------
	// Fill in the format output. Call the handler to get the modification date.

	dummyParent.format = handlerInfo->format;
	if ( format ) *format = handlerInfo->format;
	dummyParent.openFlags = handlerInfo->flags;

	dummyParent.handler = handlerInfo->handlerCTor ( &dummyParent );

	bool ok = false;

	std::vector <std::string> resourceList;
	try{
		XMP_DateTime lastModDate;
		XMP_DateTime junkDate;
		if ( modDate == 0 ) modDate = &junkDate;
		dummyParent.handler->FillAssociatedResources ( &resourceList );
		size_t countRes = resourceList.size();
		for ( size_t index = 0; index < countRes ; ++index ){
			XMP_StringPtr curFilePath = resourceList[index].c_str();
			if( Host_IO::GetFileMode ( curFilePath ) != Host_IO::kFMode_IsFile ) continue;// only interested in files
			if (!Host_IO::GetModifyDate ( curFilePath, &lastModDate ) ) continue;
			if ( ! ok || ( SXMPUtils::CompareDateTime ( *modDate , lastModDate ) < 0 ) ) 
			{
				*modDate = lastModDate;
				ok = true;
			}
		}
	}
	catch(...){
		// Fallback to the old way 
		// eventually this method would go away as 
		// soon as the implementation for
		// GetAssociatedResources is added to all 
		// the file/Plugin Handlers
		ok = dummyParent.handler->GetFileModDate ( modDate );
	}
	delete dummyParent.handler;
	dummyParent.handler = 0;

	return ok;
	XMP_FILES_STATIC_END2 ( clientPath, kXMPErrSev_OperationFatal )
	return false;

}	// XMPFiles::GetFileModDate

// =================================================================================================

/* class static */
bool
XMPFiles::GetFileModDate ( const Common::XMPFileHandlerInfo& hdlInfo,
						   XMP_StringPtr    clientPath,
						   XMP_DateTime *   modDate,
						   XMP_OptionBits   options /* = 0 */ )
{
	XMP_FILES_STATIC_START
	Host_IO::FileMode clientMode;
	std::string fileExt;	// Used to check for excluded files.
	bool excluded = FileIsExcluded ( clientPath, &fileExt, &clientMode, &sDefaultErrorCallback );	// ! Fills in fileExt and clientMode.
	if ( excluded ) return false;

	XMPFiles dummyParent;	// GetFileModDate is static, but the handler needs a parent.
	dummyParent.SetFilePath ( clientPath );
	dummyParent.format = hdlInfo.format;
	dummyParent.openFlags = hdlInfo.flags;
	dummyParent.handler = hdlInfo.handlerCTor ( &dummyParent );

	bool ok = false;

	std::vector <std::string> resourceList;
	try{
		XMP_DateTime lastModDate;
		XMP_DateTime junkDate;
		if ( modDate == 0 ) modDate = &junkDate;
		dummyParent.handler->FillAssociatedResources ( &resourceList );
		size_t countRes = resourceList.size();
		for ( size_t index = 0; index < countRes ; ++index ){
			XMP_StringPtr curFilePath = resourceList[index].c_str();
			if( Host_IO::GetFileMode ( curFilePath ) != Host_IO::kFMode_IsFile ) continue;// only interested in files
			if (!Host_IO::GetModifyDate ( curFilePath, &lastModDate ) ) continue;
			if ( ! ok || ( SXMPUtils::CompareDateTime ( *modDate , lastModDate ) < 0 ) ) 
			{
				*modDate = lastModDate;
				ok = true;
			}
		}
	}
	catch(...){
		// Fallback to the old way 
		// eventually this method would go away as 
		// soon as the implementation for
		// GetAssociatedResources is added to all 
		// the file/Plugin Handlers
		ok = dummyParent.handler->GetFileModDate ( modDate );
	}
	delete dummyParent.handler;
	dummyParent.handler = 0;

	return ok;
	XMP_FILES_STATIC_END2 ( clientPath, kXMPErrSev_OperationFatal )
	return false;

}	// XMPFiles::GetFileModDate

// =================================================================================================

/* class static */
bool
XMPFiles::GetAssociatedResources ( 
		XMP_StringPtr              filePath,
        std::vector<std::string> * resourceList,
        XMP_FileFormat             format  /* = kXMP_UnknownFile */, 
        XMP_OptionBits             options /*  = 0 */ )
{
	XMP_FILES_STATIC_START
	XMP_Assert ( (resourceList != 0) && resourceList->empty() );	// Ensure that the glue passes in an empty local.

	// Try to select a handler.

	if ( (filePath == 0) || (*filePath == 0) ) return false;

	XMPFiles dummyParent;	// GetAssociatedResources is static, but the handler needs a parent.
	dummyParent.SetFilePath ( filePath );

	XMPFileHandlerInfo * handlerInfo  = 0;
	XMP_Bool excluded=false;
	handlerInfo = CreateFileHandlerInfo ( &dummyParent, &format, options, excluded, &sDefaultErrorCallback );
#if EnableGenericHandling
#if GenericHandlingAlwaysOn
	XMP_OptionBits oldOptions = options;
	options |= kXMPFiles_OpenUseGenericHandler;
#endif	
	if (handlerInfo == 0 && !excluded
		&& (options & kXMPFiles_OpenUseGenericHandler)  )
	{
		Host_IO::FileMode fileMode = Host_IO::GetFileMode( filePath );
		if ( fileMode == Host_IO::kFMode_DoesNotExist )
			return false;

		handlerInfo = &kGenericHandlerInfo;
	}
#if GenericHandlingAlwaysOn
	options = oldOptions;
#endif
#endif
	if ( handlerInfo == 0 ) return false;

	// -------------------------------------------------------------------------
	// Fill in the format output. Call the handler to get the Associated Resources.

	dummyParent.format = handlerInfo->format;
	dummyParent.openFlags = handlerInfo->flags;

	dummyParent.handler = handlerInfo->handlerCTor ( &dummyParent );
	
	try {
		dummyParent.handler->FillAssociatedResources ( resourceList );
	} catch ( XMP_Error& error ) {
		if ( error.GetID() == kXMPErr_Unimplemented ) {
			XMP_FILES_STATIC_NOTIFY_ERROR ( &sDefaultErrorCallback, filePath, kXMPErrSev_Recoverable, error );
			return false;
		} else {
			throw;
		}
	}
	XMP_Assert ( ! resourceList->empty() );

	delete dummyParent.handler;
	dummyParent.handler = 0;
	
	return true;

	XMP_FILES_STATIC_END2 ( filePath, kXMPErrSev_OperationFatal )
	return false;

}	// XMPFiles::GetAssociatedResources

// =================================================================================================

/* class static */
bool
XMPFiles::GetAssociatedResources (
		const Common::XMPFileHandlerInfo& hdlInfo,
		XMP_StringPtr              filePath,
        std::vector<std::string> * resourceList,
        XMP_OptionBits             options /*  = 0 */ )
{
	XMP_FILES_STATIC_START
	Host_IO::FileMode clientMode;
	std::string fileExt;	// Used to check for excluded files.
	bool excluded = FileIsExcluded ( filePath, &fileExt, &clientMode, &sDefaultErrorCallback );	// ! Fills in fileExt and clientMode.
	if ( excluded ) return false;

	XMPFiles dummyParent;	// GetFileModDate is static, but the handler needs a parent.
	dummyParent.SetFilePath ( filePath );
	dummyParent.format = hdlInfo.format;
	dummyParent.openFlags = hdlInfo.flags;
	dummyParent.handler = hdlInfo.handlerCTor ( &dummyParent );

	try {
		dummyParent.handler->FillAssociatedResources ( resourceList );
	} catch ( XMP_Error& error ) {
		if ( error.GetID() == kXMPErr_Unimplemented ) {
			XMP_FILES_STATIC_NOTIFY_ERROR ( &sDefaultErrorCallback, filePath, kXMPErrSev_Recoverable, error );
			return false;
		} else {
			throw;
		}
	}
	XMP_Assert ( ! resourceList->empty() );

	delete dummyParent.handler;
	dummyParent.handler = 0;
	
	return true;

	XMP_FILES_STATIC_END2 ( filePath, kXMPErrSev_OperationFatal )
	return false;

}	// XMPFiles::GetAssociatedResources

// =================================================================================================

/* class static */
bool 
XMPFiles::IsMetadataWritable (
		XMP_StringPtr  filePath,
        XMP_Bool *     writable,
        XMP_FileFormat format  /* = kXMP_UnknownFile */,
        XMP_OptionBits options  /* = 0 */ )
{
	XMP_FILES_STATIC_START
	// Try to select a handler.
	
	if ( (filePath == 0) || (*filePath == 0) ) return false;
	
	XMPFiles dummyParent;	// IsMetadataWritable is static, but the handler needs a parent.
	dummyParent.SetFilePath ( filePath );

	XMPFileHandlerInfo * handlerInfo  = 0;
	XMP_Bool excluded=false;
	handlerInfo = CreateFileHandlerInfo ( &dummyParent, &format, options, excluded, &sDefaultErrorCallback );
#if EnableGenericHandling
#if GenericHandlingAlwaysOn
	XMP_OptionBits oldOptions = options;
	options |= kXMPFiles_OpenUseGenericHandler;
#endif
	if (handlerInfo == 0 && !excluded
		&& (options & kXMPFiles_OpenUseGenericHandler))	
	{
		Host_IO::FileMode fileMode = Host_IO::GetFileMode( filePath );
		if ( fileMode == Host_IO::kFMode_DoesNotExist )
			return false;

		handlerInfo = &kGenericHandlerInfo;
	}
#if GenericHandlingAlwaysOn
	options = oldOptions;
#endif
#endif
	if ( handlerInfo == 0 ) return false;

	if ( writable == 0 ) {
		XMP_Throw("Boolean parameter is required for IsMetadataWritable() API.", kXMPErr_BadParam);
	} else {
		*writable = kXMP_Bool_False;
	}


	dummyParent.format = handlerInfo->format;
	dummyParent.openFlags = handlerInfo->flags;
	dummyParent.handler = handlerInfo->handlerCTor ( &dummyParent );
	
	// We don't require any of the files to be opened at this point.
	// Also, if We don't close them then this will be a problem for embedded handlers because we will be checking
	// write permission on the same file which could be open (in some mode) already.
	CloseLocalFile(&dummyParent);
	
	try {
		*writable = ConvertBoolToXMP_Bool( dummyParent.handler->IsMetadataWritable() );
	} catch ( XMP_Error& error ) {
		delete dummyParent.handler;
		dummyParent.handler = 0;
		if ( error.GetID() == kXMPErr_Unimplemented ) {
			XMP_FILES_STATIC_NOTIFY_ERROR ( &sDefaultErrorCallback, filePath, kXMPErrSev_Recoverable, error );
			return false;
		} else {
			throw;
		}
	}
	if ( dummyParent.handler ) {
		delete dummyParent.handler;
		dummyParent.handler = 0;
	}
	XMP_FILES_STATIC_END2 ( filePath, kXMPErrSev_OperationFatal )
	return true;
} // XMPFiles::IsMetadataWritable 

// =================================================================================================

/* class static */
bool 
XMPFiles::IsMetadataWritable (
		const Common::XMPFileHandlerInfo& hdlInfo,
		XMP_StringPtr  filePath,
        XMP_Bool *     writable,
        XMP_OptionBits options  /* = 0 */ )
{
	XMP_FILES_STATIC_START
	Host_IO::FileMode clientMode;
	std::string fileExt;	// Used to check for excluded files.
	bool excluded = FileIsExcluded ( filePath, &fileExt, &clientMode, &sDefaultErrorCallback );	// ! Fills in fileExt and clientMode.
	if ( excluded ) return false;

	if ( writable == 0 ) {
		XMP_Throw("Boolean parameter is required for IsMetadataWritable() API.", kXMPErr_BadParam);
	} else {
		*writable = kXMP_Bool_False;
	}

	XMPFiles dummyParent;	// GetFileModDate is static, but the handler needs a parent.
	dummyParent.SetFilePath ( filePath );
	dummyParent.format = hdlInfo.format;
	dummyParent.openFlags = hdlInfo.flags;
	dummyParent.handler = hdlInfo.handlerCTor ( &dummyParent );

	// We don't require any of the files to be opened at this point.
	// Also, if We don't close them then this will be a problem for embedded handlers because we will be checking
	// write permission on the same file which could be open (in some mode) already.
	CloseLocalFile(&dummyParent);
	
	try {
		*writable = ConvertBoolToXMP_Bool( dummyParent.handler->IsMetadataWritable() );
	} catch ( XMP_Error& error ) {
		delete dummyParent.handler;
		dummyParent.handler = 0;
		if ( error.GetID() == kXMPErr_Unimplemented ) {
			XMP_FILES_STATIC_NOTIFY_ERROR ( &sDefaultErrorCallback, filePath, kXMPErrSev_Recoverable, error );
			return false;
		} else {
			throw;
		}
	}
	if ( dummyParent.handler ) {
		delete dummyParent.handler;
		dummyParent.handler = 0;
	}
	XMP_FILES_STATIC_END2 ( filePath, kXMPErrSev_OperationFatal )
	return true;
}  // XMPFiles::IsMetadataWritable 

// =================================================================================================

#if AdobePrivate
bool
XMPFiles::GetAlbumArts(
		std::vector<AlbumArt> * albumArtVector )
{
	XMP_FILES_START
	XMP_Assert( ( albumArtVector != 0 ) && albumArtVector->empty() );	// Ensure that the glue passes in an empty local.

	if ( this->handler == 0 )
		XMP_Throw( "XMPFiles::GetAlbumArts - No open file", kXMPErr_BadObject );
	if ( ( this->handler->handlerFlags & kXMPFiles_SupportsAlbumArt ) == 0 )
		XMP_Throw( "XMPFiles::GetAlbumArts - Handler doesn't support processing on Album Arts", kXMPErr_BadObject );

	// Album art has been already processed
	if ( this->handler->needsArtUpdate )
	{
		albumArtVector->insert( albumArtVector->begin(), this->handler->albumArtVector.begin(), this->handler->albumArtVector.end() );
		return true;
	}

	bool isArtworkExists = false;
	isArtworkExists = this->handler->GetAlbumArtworks();
	if ( isArtworkExists )
	{
		albumArtVector->insert( albumArtVector->begin(), this->handler->albumArtVector.begin(), this->handler->albumArtVector.end() );
		this->handler->albumArtVector.clear();
		return true;
	}
	this->handler->albumArtVector.clear();
	XMP_FILES_END2( filePath.c_str(), kXMPErrSev_OperationFatal )
	return false;

}	// XMPFiles::GetAlbumArts

// =================================================================================================
#endif

static bool
DoOpenFile ( XMPFiles *     thiz,
			 XMP_IO *       clientIO,
			 XMP_StringPtr  clientPath,
	         XMP_FileFormat format /* = kXMP_UnknownFile */,
	         XMP_OptionBits openFlags /* = 0 */ )
{
	XMP_Assert ( (clientIO == 0) ? (clientPath[0] != 0) : (clientPath[0] == 0) );
	
	openFlags &= ~kXMPFiles_ForceGivenHandler;	// Don't allow this flag for OpenFile.

	if ( (openFlags & kXMPFiles_OptimizeFileLayout) && (! (openFlags & kXMPFiles_OpenForUpdate)) ) {
			XMP_Throw ( "OptimizeFileLayout requires OpenForUpdate", kXMPErr_BadParam );
	}

	if ( thiz->handler != 0 ) XMP_Throw ( "File already open", kXMPErr_BadParam );
	CloseLocalFile ( thiz );	// Sanity checks if prior call failed.

	thiz->ioRef = clientIO;
	thiz->SetFilePath ( clientPath );

	thiz->format = kXMP_UnknownFile;	// Make sure it is preset for later check.
	thiz->openFlags = openFlags;

	bool readOnly = XMP_OptionIsClear ( openFlags, kXMPFiles_OpenForUpdate );

	Host_IO::FileMode clientMode;
	std::string fileExt;	// Used to check for camera raw files and OK to scan files.

	if ( thiz->UsesClientIO() ) {
		clientMode = Host_IO::kFMode_IsFile;
	} else {
		bool excluded = FileIsExcluded ( clientPath, &fileExt, &clientMode, &thiz->errorCallback );	// ! Fills in fileExt and clientMode.
		if ( excluded ) return false;
	}

	// Find the handler, fill in the XMPFiles member variables, cache the desired file data.

	XMPFileHandlerInfo * handlerInfo  = 0;
	XMPFileHandlerCTor   handlerCTor  = 0;
	XMP_OptionBits       handlerFlags = 0;

	if ( ! (openFlags & kXMPFiles_OpenUsePacketScanning) ) {
		handlerInfo = HandlerRegistry::getInstance().selectSmartHandler( thiz, clientPath, format, openFlags );
#if EnableGenericHandling
#if GenericHandlingAlwaysOn
		XMP_OptionBits oldOpenFlags = openFlags;
		openFlags |= kXMPFiles_OpenUseGenericHandler;
#endif
	if (handlerInfo==0 
		&& !(openFlags & kXMPFiles_OpenStrictly) 
		&& (openFlags & kXMPFiles_OpenUseGenericHandler)) 
		{
			if ( clientMode == Host_IO::kFMode_DoesNotExist )
				return false;

			handlerInfo = &kGenericHandlerInfo;
		}
#if GenericHandlingAlwaysOn
		openFlags = oldOpenFlags;
#endif
#endif
	}

	#if ! EnablePacketScanning

		if ( handlerInfo == 0 ) {
			if ( clientMode == Host_IO::kFMode_DoesNotExist ) {
				XMP_Error error ( kXMPErr_NoFile, "XMPFiles: file does not exist" );
				XMP_FILES_STATIC_NOTIFY_ERROR ( &thiz->errorCallback, clientPath, kXMPErrSev_Recoverable, error );
			}
			return false;
		}

	#else

		if ( handlerInfo == 0 ) {

			// No smart handler, packet scan if appropriate.
			if ( clientMode == Host_IO::kFMode_DoesNotExist ) {
				XMP_Error error ( kXMPErr_NoFile, "XMPFiles: file does not exist" );
				XMP_FILES_STATIC_NOTIFY_ERROR ( &thiz->errorCallback, clientPath, kXMPErrSev_Recoverable, error );
				return false;
			} else if ( clientMode != Host_IO::kFMode_IsFile ) {
				return false;
			}

			if ( openFlags & kXMPFiles_OpenUseSmartHandler ) {
				CloseLocalFile ( thiz );
				XMP_Error error ( kXMPErr_NoFileHandler, "XMPFiles: No smart file handler available to handle file" );
				XMP_FILES_STATIC_NOTIFY_ERROR ( &thiz->errorCallback, clientPath, kXMPErrSev_Recoverable, error );
				return false;
			}

#if EnableGenericHandling
			if ( openFlags & kXMPFiles_OpenUseGenericHandler ) {
				CloseLocalFile ( thiz );
				XMP_Error error ( kXMPErr_NoFileHandler, "XMPFiles: Generic handler not available to handle file" );
				XMP_FILES_STATIC_NOTIFY_ERROR ( &thiz->errorCallback, clientPath, kXMPErrSev_Recoverable, error );
				return false;
			}
#endif

			if ( openFlags & kXMPFiles_OpenLimitedScanning ) {
				bool scanningOK = false;
				for ( size_t i = 0; kKnownScannedFiles[i] != 0; ++i ) {
					if ( fileExt == kKnownScannedFiles[i] ) { scanningOK = true; break; }
				}
				if ( ! scanningOK ) return false;
			}

			handlerInfo = &kScannerHandlerInfo;
			if ( thiz->ioRef == 0 ) {	// Normally opened in SelectSmartHandler, but might not be open yet.
				thiz->ioRef = XMPFiles_IO::New_XMPFiles_IO ( clientPath, readOnly );
				if ( thiz->ioRef == 0 ) return false;
			}

		}

	#endif

	XMP_Assert ( handlerInfo != 0 );
	handlerCTor  = handlerInfo->handlerCTor;
	handlerFlags = handlerInfo->flags;

	XMP_Assert ( (thiz->ioRef != 0) ||
				 (handlerFlags & kXMPFiles_UsesSidecarXMP) ||
				 (handlerFlags & kXMPFiles_HandlerOwnsFile) ||
				 (handlerFlags & kXMPFiles_FolderBasedFormat) );

	if ( thiz->format == kXMP_UnknownFile ) thiz->format = handlerInfo->format;	// ! The CheckProc might have set it.
	XMPFileHandler* handler = (*handlerCTor) ( thiz );
	XMP_Assert ( handlerFlags == handler->handlerFlags );

	thiz->handler = handler;

	try {
		if ( !readOnly && handlerFlags & kXMPFiles_FolderBasedFormat ) {
			bool isMetadataWritable = handler->IsMetadataWritable();
			if ( !isMetadataWritable ) {
				XMP_Throw ( "Open, file permission error", kXMPErr_FilePermission );
			}
		}
		handler->CacheFileData();
	} catch ( ... ) {
		delete thiz->handler;
		thiz->handler = 0;
		if ( ! (handlerFlags & kXMPFiles_HandlerOwnsFile) ) CloseLocalFile ( thiz );
		throw;
	}

	if ( handler->containsXMP ) FillPacketInfo ( handler->xmpPacket, &handler->packetInfo );

	if ( (! (openFlags & kXMPFiles_OpenForUpdate)) && (! (handlerFlags & kXMPFiles_HandlerOwnsFile)) && (!(handlerFlags &  kXMPFiles_NeedsLocalFileOpened)) ){
		// Close the disk file now if opened for read-only access.
		CloseLocalFile ( thiz );
	}

	return true;

}	// DoOpenFile

// =================================================================================================

static bool DoOpenFile( XMPFiles* thiz, 
						const Common::XMPFileHandlerInfo& hdlInfo, 
						XMP_IO* clientIO, 
						XMP_StringPtr clientPath, 
						XMP_OptionBits openFlags )
{
	XMP_Assert ( (clientIO == 0) ? (clientPath[0] != 0) : (clientPath[0] == 0) );
	
	openFlags &= ~kXMPFiles_ForceGivenHandler;	// Don't allow this flag for OpenFile.

	if ( (openFlags & kXMPFiles_OptimizeFileLayout) && (! (openFlags & kXMPFiles_OpenForUpdate)) ) {
			XMP_Throw ( "OptimizeFileLayout requires OpenForUpdate", kXMPErr_BadParam );
	}

	if ( thiz->handler != 0 ) XMP_Throw ( "File already open", kXMPErr_BadParam );

	//
	// setup members
	//
	thiz->ioRef		= clientIO;
	thiz->SetFilePath ( clientPath );
	thiz->format	= hdlInfo.format;
	thiz->openFlags = openFlags;

	//
	// create file handler instance
	//
	XMPFileHandlerCTor handlerCTor	= hdlInfo.handlerCTor;
	XMP_OptionBits handlerFlags		= hdlInfo.flags;

	XMPFileHandler* handler			= (*handlerCTor) ( thiz );
	XMP_Assert ( handlerFlags == handler->handlerFlags );

	thiz->handler = handler;
	bool readOnly = XMP_OptionIsClear ( openFlags, kXMPFiles_OpenForUpdate );

	if ( thiz->ioRef == 0 ) {	//Need to open the file if not done already
		thiz->ioRef = XMPFiles_IO::New_XMPFiles_IO ( clientPath, readOnly );
		if ( thiz->ioRef == 0 ) return false;
	}
	//
	// try to read metadata
	//
	try 
	{
		handler->CacheFileData();

		if( handler->containsXMP ) 
		{
			FillPacketInfo( handler->xmpPacket, &handler->packetInfo );
		}

		if( (! (openFlags & kXMPFiles_OpenForUpdate)) && (! (handlerFlags & kXMPFiles_HandlerOwnsFile)) && (!(handlerFlags &  kXMPFiles_NeedsLocalFileOpened)) )
		{
			// Close the disk file now if opened for read-only access.
			CloseLocalFile ( thiz );
		}
	} 
	catch( ... ) 
	{
		delete thiz->handler;
		thiz->handler = NULL;

		if( ! (handlerFlags & kXMPFiles_HandlerOwnsFile) ) 
		{
			CloseLocalFile( thiz );
		}

		throw;
	}

	return true;
}

// =================================================================================================

bool
XMPFiles::OpenFile ( XMP_StringPtr  clientPath,
	                 XMP_FileFormat _format /* = kXMP_UnknownFile */,
	                 XMP_OptionBits _openFlags /* = 0 */ )
{
	XMP_FILES_START
	return DoOpenFile ( this, 0, clientPath, _format, _openFlags );
	XMP_FILES_END2 ( clientPath, kXMPErrSev_FileFatal )
	return false;
}

// =================================================================================================

#if XMP_StaticBuild	// ! Client XMP_IO objects can only be used in static builds.

bool
XMPFiles::OpenFile ( XMP_IO*        clientIO,
	                 XMP_FileFormat _format /* = kXMP_UnknownFile */,
	                 XMP_OptionBits _openFlags /* = 0 */ )
{
	XMP_FILES_START
	this->progressTracker = 0;	// Progress tracking is not supported for client-managed I/O.
	return DoOpenFile ( this, clientIO, "", _format, _openFlags );
	XMP_FILES_END1 ( kXMPErrSev_FileFatal )
	return false;

}	// XMPFiles::OpenFile

bool XMPFiles::OpenFile ( const Common::XMPFileHandlerInfo&	hdlInfo,
						 XMP_IO*							clientIO,
						 XMP_OptionBits						_openFlags /*= 0*/ )
{
	XMP_FILES_START
	this->progressTracker = 0;	// Progress tracking is not supported for client-managed I/O.
	return DoOpenFile ( this, hdlInfo, clientIO, NULL, _openFlags );
	XMP_FILES_END1 ( kXMPErrSev_FileFatal )
	return false;

}

#endif	// XMP_StaticBuild

// =================================================================================================

bool XMPFiles::OpenFile ( const Common::XMPFileHandlerInfo&	hdlInfo,
						 XMP_StringPtr						_filePath,
						 XMP_OptionBits						_openFlags /*= 0*/ )
{
	XMP_FILES_START
	return DoOpenFile ( this, hdlInfo, NULL, _filePath, _openFlags );
	XMP_FILES_END2 (_filePath, kXMPErrSev_FileFatal )
	return false;
}

// =================================================================================================

void
XMPFiles::CloseFile ( XMP_OptionBits closeFlags /* = 0 */ )
{
	XMP_FILES_START
	if ( this->handler == 0 ) return;	// Return if there is no open file (not an error).

	bool needsUpdate = this->handler->needsUpdate;
	bool optimizeFileLayout = XMP_OptionIsSet ( this->openFlags, kXMPFiles_OptimizeFileLayout );

	XMP_OptionBits handlerFlags = this->handler->handlerFlags;

	// Decide if we're doing a safe update. If so, make sure the handler supports it. All handlers
	// that don't own the file tolerate safe update using common code below.

	bool doSafeUpdate = XMP_OptionIsSet ( closeFlags, kXMPFiles_UpdateSafely );
	#if GatherPerformanceData
		if ( doSafeUpdate ) sAPIPerf->back().extraInfo += ", safe update";	// Client wants safe update.
	#endif

	if ( ! (this->openFlags & kXMPFiles_OpenForUpdate) ) doSafeUpdate = false;
	if ( ! needsUpdate ) doSafeUpdate = false;

	bool safeUpdateOK = ( (handlerFlags & kXMPFiles_AllowsSafeUpdate) ||
						  (! (handlerFlags & kXMPFiles_HandlerOwnsFile)) );
	if ( doSafeUpdate && (! safeUpdateOK) ) {
		XMP_Throw ( "XMPFiles::CloseFile - Safe update not supported", kXMPErr_Unavailable );
	}

	if ( (this->progressTracker != 0) && this->UsesLocalIO() && this->ioRef != NULL ) {
		XMPFiles_IO * localFile = (XMPFiles_IO*)this->ioRef;
		localFile->SetProgressTracker ( this->progressTracker );
	}

	// Try really hard to make sure the file is closed and the handler is deleted.

	try {

		if ( (! doSafeUpdate) || (handlerFlags & kXMPFiles_HandlerOwnsFile) ) {	// ! Includes no update case.

			// Close the file without doing common crash-safe writing. The handler might do it.

			needsUpdate |= optimizeFileLayout;

			if ( needsUpdate ) {
				#if GatherPerformanceData
					sAPIPerf->back().extraInfo += ", direct update";
				#endif
				this->handler->UpdateFile ( doSafeUpdate );
			}

			delete this->handler;
			this->handler = 0;
			CloseLocalFile ( this );

		} else {

			// Update the file in a crash-safe manner using common control of a temp file.

			XMP_IO* tempFileRef = this->ioRef->DeriveTemp();
			if ( tempFileRef == 0 ) XMP_Throw ( "XMPFiles::CloseFile, cannot create temp", kXMPErr_InternalFailure );

			if ( handlerFlags & kXMPFiles_CanRewrite ) {

				// The handler can rewrite an entire file based on the original.

				#if GatherPerformanceData
					sAPIPerf->back().extraInfo += ", file rewrite";
				#endif

				this->handler->WriteTempFile ( tempFileRef );

			} else {

				// The handler can only update an existing file. Copy to the temp then update.


				#if GatherPerformanceData
					sAPIPerf->back().extraInfo += ", copy update";
				#endif


				XMP_IO* origFileRef = this->ioRef;

				origFileRef->Rewind();
				if ( this->progressTracker != 0 && (this->handler->handlerFlags & kXMPFiles_CanNotifyProgress) ) progressTracker->BeginWork ( (float) origFileRef->Length() );
				XIO::Copy ( origFileRef, tempFileRef, origFileRef->Length(), abortProc, abortArg );

				try {

					this->ioRef = tempFileRef;
					this->handler->UpdateFile ( false );	// We're doing the safe update, not the handler.
					this->ioRef = origFileRef;

				} catch ( ... ) {

					this->ioRef->DeleteTemp();
					this->ioRef = origFileRef;
					throw;

				}

				if ( progressTracker != 0 && (this->handler->handlerFlags & kXMPFiles_CanNotifyProgress)) progressTracker->WorkComplete();

			}

			this->ioRef->AbsorbTemp();
			CloseLocalFile ( this );

			delete this->handler;
			this->handler = 0;

		}

	} catch ( ... ) {

		// *** Don't delete the temp or copy files, not sure which is best.

		try {
			if ( this->handler != 0 ) {
				delete this->handler;
				this->handler = 0;
			}
		} catch ( ... ) { /* Do nothing, throw the outer exception later. */ }

		if ( this->ioRef ) this->ioRef->DeleteTemp();
		CloseLocalFile ( this );
		this->ClearFilePath();

		this->handler   = 0;
		this->format    = kXMP_UnknownFile;
		this->ioRef     = 0;
		this->openFlags = 0;

		if ( this->tempPtr != 0 ) free ( this->tempPtr );	// ! Must have been malloc-ed!
		this->tempPtr  = 0;
		this->tempUI32 = 0;

		throw;

	}

	// Clear the XMPFiles member variables.

	CloseLocalFile ( this );
	this->ClearFilePath();

	this->handler   = 0;
	this->format    = kXMP_UnknownFile;
	this->ioRef     = 0;
	this->openFlags = 0;

	if ( this->tempPtr != 0 ) free ( this->tempPtr );	// ! Must have been malloc-ed!
	this->tempPtr  = 0;
	this->tempUI32 = 0;
	XMP_FILES_END1 ( kXMPErrSev_FileFatal )

}	// XMPFiles::CloseFile

// =================================================================================================

bool
XMPFiles::GetFileInfo ( XMP_StringPtr *  _filePath /* = 0 */,
                        XMP_StringLen *  pathLen /* = 0 */,
	                    XMP_OptionBits * _openFlags /* = 0 */,
	                    XMP_FileFormat * _format /* = 0 */,
	                    XMP_OptionBits * handlerFlags /* = 0 */ ) const
{
	XMP_FILES_START
	if ( this->handler == 0 ) return false;
	//XMPFileHandler * handler = this->handler;

	XMP_StringPtr  voidStringPtr = 0;
	XMP_StringLen  voidStringLen = 0;
	XMP_OptionBits voidOptionBits = 0;
	XMP_FileFormat voidFileFormat = 0;

	if ( _filePath == 0 ) _filePath = &voidStringPtr;
	if ( pathLen == 0 ) pathLen = &voidStringLen;
	if ( _openFlags == 0 ) _openFlags = &voidOptionBits;
	if ( _format == 0 ) _format = &voidFileFormat;
	if ( handlerFlags == 0 ) handlerFlags = &voidOptionBits;

	*_filePath     = this->filePath.c_str();
	*pathLen      = (XMP_StringLen) this->filePath.size();
	*_openFlags    = this->openFlags;
	*_format       = this->format;
	*handlerFlags = this->handler->handlerFlags;
	XMP_FILES_END1 ( kXMPErrSev_FileFatal )
	return true;

}	// XMPFiles::GetFileInfo

// =================================================================================================

void
XMPFiles::SetAbortProc ( XMP_AbortProc _abortProc,
						 void *        _abortArg )
{
	XMP_FILES_START
	this->abortProc = _abortProc;
	this->abortArg  = _abortArg;

	XMP_Assert ( (_abortProc != (XMP_AbortProc)0) || (_abortArg != (void*)(unsigned long long)0xDEADBEEFULL) );	// Hack to test the assert callback.
	XMP_FILES_END1 ( kXMPErrSev_OperationFatal )
}	// XMPFiles::SetAbortProc

// =================================================================================================
// SetClientPacketInfo
// ===================
//
// Set the packet info returned to the client. This is the internal packet info at first, which
// tells what is in the file. But once the file needs update (PutXMP has been called), we return
// info about the latest XMP. The internal packet info is left unchanged since it is needed when
// the file is updated to locate the old packet in the file.

static void
SetClientPacketInfo ( XMP_PacketInfo * clientInfo, const XMP_PacketInfo & handlerInfo,
					  const std::string & xmpPacket, bool needsUpdate )
{

	if ( clientInfo == 0 ) return;

	if ( ! needsUpdate ) {
		*clientInfo = handlerInfo;
	} else {
		clientInfo->offset = kXMPFiles_UnknownOffset;
		clientInfo->length = (XMP_Int32) xmpPacket.size();
		FillPacketInfo ( xmpPacket, clientInfo );
	}

}	// SetClientPacketInfo

// =================================================================================================

bool
XMPFiles::GetXMP ( SXMPMeta *       xmpObj /* = 0 */,
                   XMP_StringPtr *  xmpPacket /* = 0 */,
                   XMP_StringLen *  xmpPacketLen /* = 0 */,
                   XMP_PacketInfo * packetInfo /* = 0 */ )
{
	XMP_FILES_START
	if ( this->handler == 0 ) XMP_Throw ( "XMPFiles::GetXMP - No open file", kXMPErr_BadObject );

	/*
	 Sometimes XMP can be very huge See : CTECHXMP-4170126
	 Client needs to know the length of XMP packet in file, so they can
	 decide whether they want to process XMP packet or not.
	
	 This is possible, if format knows the length of XMP packet in file during CacheFileData.
	 So, if client pass only XMP_PacketInfo and does not pass meta object and xmp packet length is
	 known during CacheFileData, we can provide this info to client without processing xmp.
	 */
	
	if(! this->handler->processedXMP && !xmpObj && !(this->handler->xmpPacket.empty()) && packetInfo)
	{
		/*CTECHXMP-4170329: xmppacket and xmpPacketLen will only be populated if client has explicitly requested for xmp packet*/
		if ( xmpPacket != 0 ) *xmpPacket = this->handler->xmpPacket.c_str();
		if ( xmpPacketLen != 0 ) *xmpPacketLen = (XMP_StringLen)this->handler->xmpPacket.size();
		SetClientPacketInfo(packetInfo, this->handler->packetInfo,
			this->handler->xmpPacket, this->handler->needsUpdate );
		return true;
	}
	
	XMP_OptionBits applyTemplateFlags = kXMPTemplate_AddNewProperties | kXMPTemplate_IncludeInternalProperties;

	if ( ! this->handler->processedXMP ) {
		try {
			this->handler->ProcessXMP();
		} catch ( ... ) {
			// Return the outputs then rethrow the exception.
			if ( xmpObj != 0 ) {
				// ! Don't use Clone, that replaces the internal ref in the local xmpObj, leaving the client unchanged!
				xmpObj->Erase();
				#if AdobePrivate
					#if ENABLE_CPP_DOM_MODEL
						try{
							AdobeXMPCore::spIMetadata sp1 = xmpObj->GetIMetadata();
							sp1->Clear();
							AdobeXMPCore::spIMetadata toBeCloned = this->handler->xmpObj.GetIMetadata ( );
							AdobeXMPCore::spINodeIterator it = toBeCloned->Iterator ( );
							while (it) {
								sp1->AppendNode(it->GetNode()->Clone());
								it = it->Next();
							}
							AdobeXMPCore::spINodeIterator qualIter = ( toBeCloned->HasQualifiers ( ) ) ? toBeCloned->QualifiersIterator ( ) : AdobeXMPCore::spINodeIterator ( );
							while (qualIter) {
								sp1->InsertQualifier(qualIter->GetNode()->Clone());
								qualIter = qualIter->Next();
							}
							sp1->SetAboutURI ( toBeCloned->GetAboutURI ( )->c_str ( ), toBeCloned->GetAboutURI ( )->size ( ) );
						}

						catch(...) {
						
							SXMPUtils::ApplyTemplate(xmpObj, this->handler->xmpObj, applyTemplateFlags);
						}
			    
					#else
							SXMPUtils::ApplyTemplate(xmpObj, this->handler->xmpObj, applyTemplateFlags);
					#endif
				#else
					SXMPUtils::ApplyTemplate(xmpObj, this->handler->xmpObj, applyTemplateFlags);
				#endif
			}
			if ( xmpPacket != 0 ) *xmpPacket = this->handler->xmpPacket.c_str();
			if ( xmpPacketLen != 0 ) *xmpPacketLen = (XMP_StringLen) this->handler->xmpPacket.size();
			SetClientPacketInfo ( packetInfo, this->handler->packetInfo,
								  this->handler->xmpPacket, this->handler->needsUpdate );
			throw;
		}
		if ((this->handler->handlerFlags &  kXMPFiles_NeedsLocalFileOpened) && !(this->openFlags & kXMPFiles_OpenForUpdate)) {
			CloseLocalFile(this);
		}
	}

	if ( ! this->handler->containsXMP ) return false;

	#if 0	// *** See bug 1131815. A better way might be to pass the ref up from here.
		if ( xmpObj != 0 ) *xmpObj = this->handler->xmpObj.Clone();
	#else
		if ( xmpObj != 0 ) {
		#if AdobePrivate
			#if ENABLE_CPP_DOM_MODEL
		
				try{
					AdobeXMPCore::spIMetadata sp2 = xmpObj->GetIMetadata();
					sp2->Clear();
					AdobeXMPCore::spIMetadata toBeCloned = this->handler->xmpObj.GetIMetadata();
					AdobeXMPCore::spINodeIterator it = toBeCloned->Iterator();
					while (it) {
						sp2->AppendNode(it->GetNode()->Clone(true));
						it = it->Next();
					}
					AdobeXMPCore::spINodeIterator qualIter = ( toBeCloned->HasQualifiers ( ) ) ? toBeCloned->QualifiersIterator ( ): AdobeXMPCore::spINodeIterator ( );
					while (qualIter) {
						sp2->InsertQualifier(qualIter->GetNode()->Clone(true));
						qualIter = qualIter->Next();
					}

					sp2->SetAboutURI ( toBeCloned->GetAboutURI ( )->c_str ( ), toBeCloned->GetAboutURI ( )->size ( ) );
				}
				catch(...) {
					// ! Don't use Clone, that replaces the internal ref in the local xmpObj, leaving the client unchanged!
					xmpObj->Erase();
					SXMPUtils::ApplyTemplate(xmpObj, this->handler->xmpObj, applyTemplateFlags);
				}
			#else
					xmpObj->Erase();
					SXMPUtils::ApplyTemplate(xmpObj, this->handler->xmpObj, applyTemplateFlags);
			#endif
		#else
			xmpObj->Erase();
			SXMPUtils::ApplyTemplate(xmpObj, this->handler->xmpObj, applyTemplateFlags);
        #endif	
		}
	#endif

	if ( xmpPacket != 0 ) *xmpPacket = this->handler->xmpPacket.c_str();
	if ( xmpPacketLen != 0 ) *xmpPacketLen = (XMP_StringLen) this->handler->xmpPacket.size();
	SetClientPacketInfo ( packetInfo, this->handler->packetInfo,
						  this->handler->xmpPacket, this->handler->needsUpdate );
	XMP_FILES_END1 ( kXMPErrSev_FileFatal )
	return true;

}	// XMPFiles::GetXMP

// =================================================================================================

static bool
DoPutXMP ( XMPFiles * thiz, const SXMPMeta & xmpObj, const bool doIt )
{
	// Check some basic conditions to see if the Put should be attempted.

	if ( thiz->handler == 0 ) XMP_Throw ( "XMPFiles::PutXMP - No open file", kXMPErr_BadObject );
	if ( ! (thiz->openFlags & kXMPFiles_OpenForUpdate) ) {
		XMP_Throw ( "XMPFiles::PutXMP - Not open for update", kXMPErr_BadObject );
	}

	XMPFileHandler * handler      = thiz->handler;
	XMP_OptionBits   handlerFlags = handler->handlerFlags;
	XMP_PacketInfo & packetInfo   = handler->packetInfo;
	std::string &    xmpPacket    = handler->xmpPacket;

	if ( ! handler->processedXMP ) handler->ProcessXMP();	// Might have Open/Put with no GetXMP.

	size_t oldPacketOffset = (size_t)packetInfo.offset;
	size_t oldPacketLength = packetInfo.length;

	if ( oldPacketOffset == (size_t)kXMPFiles_UnknownOffset ) oldPacketOffset = 0;	// ! Simplify checks.
	if ( oldPacketLength == (size_t)kXMPFiles_UnknownLength ) oldPacketLength = 0;

	bool fileHasPacket = (oldPacketOffset != 0) && (oldPacketLength != 0);

	if ( ! fileHasPacket ) {
		if ( ! (handlerFlags & kXMPFiles_CanInjectXMP) ) {
			XMP_Throw ( "XMPFiles::PutXMP - Can't inject XMP", kXMPErr_Unavailable );
		}
		if ( handler->stdCharForm == kXMP_CharUnknown ) {
			XMP_Throw ( "XMPFiles::PutXMP - No standard character form", kXMPErr_InternalFailure );
		}
	}

	// Serialize the XMP and update the handler's info.

	XMP_Uns8 charForm = handler->stdCharForm;
	if ( charForm == kXMP_CharUnknown ) charForm = packetInfo.charForm;

	XMP_OptionBits options = handler->GetSerializeOptions() | XMP_CharToSerializeForm ( charForm );
	if ( handlerFlags & kXMPFiles_NeedsReadOnlyPacket ) options |= kXMP_ReadOnlyPacket;
	if ( fileHasPacket && (thiz->format == kXMP_UnknownFile) && (! packetInfo.writeable) ) options |= kXMP_ReadOnlyPacket;

	bool preferInPlace = ((handlerFlags & kXMPFiles_PrefersInPlace) != 0);
	bool tryInPlace    = (fileHasPacket & preferInPlace) || (! (handlerFlags & kXMPFiles_CanExpand));

	if ( handlerFlags & kXMPFiles_UsesSidecarXMP ) tryInPlace = false;

	if ( tryInPlace ) {
		try {
			xmpObj.SerializeToBuffer ( &xmpPacket, (options | kXMP_ExactPacketLength), (XMP_StringLen) oldPacketLength );
			XMP_Assert ( xmpPacket.size() == oldPacketLength );
		} catch ( ... ) {
			if ( preferInPlace ) {
				tryInPlace = false;	// ! Try again, out of place this time.
			} else {
				if ( ! doIt ) return false;
				throw;
			}
		}
	}

	if ( ! tryInPlace ) {
		try {
			xmpObj.SerializeToBuffer ( &xmpPacket, options );
		} catch ( ... ) {
			if ( ! doIt ) return false;
			throw;
		}
	}

	if ( doIt ) {
		handler->xmpObj = xmpObj.Clone();
		handler->containsXMP = true;
		handler->processedXMP = true;
		handler->needsUpdate = true;
	}

	return true;

}	// DoPutXMP

// =================================================================================================

void
XMPFiles::PutXMP ( const SXMPMeta & xmpObj )
{
	XMP_FILES_START
	(void) DoPutXMP ( this, xmpObj, true );
	XMP_FILES_END1 ( kXMPErrSev_FileFatal )

}	// XMPFiles::PutXMP

// =================================================================================================

void
XMPFiles::PutXMP ( XMP_StringPtr xmpPacket,
                   XMP_StringLen xmpPacketLen /* = kXMP_UseNullTermination */ )
{
	XMP_FILES_START
	SXMPMeta xmpObj;
	xmpObj.SetErrorCallback ( ErrorCallbackForXMPMeta, &errorCallback );
	xmpObj.ParseFromBuffer ( xmpPacket, xmpPacketLen );
	this->PutXMP ( xmpObj );
	XMP_FILES_END1 ( kXMPErrSev_FileFatal )
}	// XMPFiles::PutXMP

// =================================================================================================

bool
XMPFiles::CanPutXMP ( const SXMPMeta & xmpObj )
{
	XMP_FILES_START
	if ( this->handler == 0 ) XMP_Throw ( "XMPFiles::CanPutXMP - No open file", kXMPErr_BadObject );

	if ( ! (this->openFlags & kXMPFiles_OpenForUpdate) ) return false;

	if ( this->handler->handlerFlags & kXMPFiles_CanInjectXMP ) return true;
	if ( ! this->handler->containsXMP ) return false;
	if ( this->handler->handlerFlags & kXMPFiles_CanExpand ) return true;

	return DoPutXMP ( this, xmpObj, false );
	XMP_FILES_END1 ( kXMPErrSev_FileFatal )
	return false;

}	// XMPFiles::CanPutXMP

// =================================================================================================

bool
XMPFiles::CanPutXMP ( XMP_StringPtr xmpPacket,
                      XMP_StringLen xmpPacketLen /* = kXMP_UseNullTermination */ )
{
	XMP_FILES_START
	SXMPMeta xmpObj;
	xmpObj.SetErrorCallback ( ErrorCallbackForXMPMeta, &errorCallback );
	xmpObj.ParseFromBuffer ( xmpPacket, xmpPacketLen );
	return this->CanPutXMP ( xmpObj );
	XMP_FILES_END1 ( kXMPErrSev_FileFatal )
	return false;

}	// XMPFiles::CanPutXMP


// =================================================================================================

#if AdobePrivate
bool
XMPFiles::PutAlbumArts( const std::vector<AlbumArt> & albumArtVector )
{
	XMP_FILES_START
	
	if ( this->handler == 0 ) XMP_Throw( "XMPFiles::PutAlbumArts - No open file", kXMPErr_BadObject );
	if ( !( this->openFlags & kXMPFiles_OpenForUpdate ) )
		XMP_Throw( "XMPFiles::PutAlbumArts - Not open for update", kXMPErr_BadObject );
	if ( ( this->handler->handlerFlags & kXMPFiles_SupportsAlbumArt ) == 0 )
		XMP_Throw( "XMPFiles::PutAlbumArts - Handler doesn't support processing on Album Arts", kXMPErr_BadObject );

	XMP_Assert( this->handler->needsArtUpdate || this->handler->albumArtVector.size() == 0 );
	
	// Need to set vector used while updating the file
	this->handler->albumArtVector = albumArtVector;
	this->handler->needsArtUpdate = true;
	this->handler->needsUpdate = true;
	return true;
	XMP_FILES_END1( kXMPErrSev_FileFatal )
	return false;

}	// XMPFiles::PutAlbumArts

// =================================================================================================
#endif

/* class-static */
void
XMPFiles::SetDefaultProgressCallback ( const XMP_ProgressTracker::CallbackInfo & cbInfo )
{
	XMP_FILES_STATIC_START
	XMP_Assert ( cbInfo.wrapperProc != 0 );	// ! Should be provided by the glue code.

	sProgressDefault = cbInfo;
	XMP_FILES_STATIC_END1 ( kXMPErrSev_OperationFatal )

}	// XMPFiles::SetDefaultProgressCallback

// =================================================================================================

void
XMPFiles::SetProgressCallback ( const XMP_ProgressTracker::CallbackInfo & cbInfo )
{
	XMP_FILES_START
	XMP_Assert ( cbInfo.wrapperProc != 0 );	// ! Should be provided by the glue code.

	if ( (this->handler != 0) && this->UsesClientIO() ) return;	// Can't use progress tracking.

	if ( this->progressTracker != 0 ) {
		delete this->progressTracker;	// ! Delete any existing tracker.
		this->progressTracker = 0;
	}
	
	if ( cbInfo.clientProc != 0 ) {
		this->progressTracker = new XMP_ProgressTracker ( cbInfo );
		if ( this->handler != 0 ) {		 // Provided to support ProgressTracker in Plugins
			XMP_ProgressTracker::CallbackInfo * callbackInfo = new XMP_ProgressTracker::CallbackInfo(cbInfo);
			this->handler->SetProgressCallback( callbackInfo ) ;
			delete callbackInfo;
		}
	}
	XMP_FILES_END1 ( kXMPErrSev_OperationFatal )

}	// XMPFiles::SetProgressCallback

// =================================================================================================
// Error notifications
// ===================

// -------------------------------------------------------------------------------------------------
// SetDefaultErrorCallback
// -----------------------

/* class-static */ 
void XMPFiles::SetDefaultErrorCallback ( XMPFiles_ErrorCallbackWrapper	wrapperProc,
										 XMPFiles_ErrorCallbackProc		clientProc,
										 void *							context,
										 XMP_Uns32						limit )
{
	XMP_FILES_STATIC_START
	XMP_Assert ( wrapperProc != 0 );	// Must always be set by the glue;

	sDefaultErrorCallback.wrapperProc = wrapperProc;
	sDefaultErrorCallback.clientProc = clientProc;
	sDefaultErrorCallback.context = context;
	sDefaultErrorCallback.limit = limit;
	XMP_FILES_STATIC_END1 ( kXMPErrSev_OperationFatal )
}	// SetDefaultErrorCallback

// -------------------------------------------------------------------------------------------------
// SetErrorCallback
// ----------------

void XMPFiles::SetErrorCallback ( XMPFiles_ErrorCallbackWrapper	wrapperProc,
								  XMPFiles_ErrorCallbackProc	clientProc,
								  void *						context,
								  XMP_Uns32						limit )
{
	XMP_FILES_START
	XMP_Assert ( wrapperProc != 0 );	// Must always be set by the glue;

	this->errorCallback.Clear();
	this->errorCallback.wrapperProc = wrapperProc;
	this->errorCallback.clientProc = clientProc;
	this->errorCallback.context = context;
	this->errorCallback.limit = limit;
	if ( this->handler != 0 )	 // Provided to support ErrroCallback in Plugins
		this->handler->SetErrorCallback( ErrorCallbackBox( errorCallback.wrapperProc, errorCallback.clientProc, errorCallback.context, errorCallback.limit ) );
	XMP_FILES_END1 ( kXMPErrSev_OperationFatal )
}	// SetErrorCallback

// -------------------------------------------------------------------------------------------------
// ResetErrorCallbackLimit
// -----------------------

void XMPFiles::ResetErrorCallbackLimit ( XMP_Uns32 limit ) {
	XMP_FILES_START
	this->errorCallback.limit = limit;
	this->errorCallback.notifications = 0;
	this->errorCallback.topSeverity = kXMPErrSev_Recoverable;
	if ( this->handler != 0 )		// Provided to support ErrroCallback in Plugins
		this->handler->SetErrorCallback( ErrorCallbackBox( errorCallback.wrapperProc, errorCallback.clientProc, errorCallback.context, errorCallback.limit ) );
	XMP_FILES_END1 ( kXMPErrSev_OperationFatal )
}	// ResetErrorCallbackLimit

// -------------------------------------------------------------------------------------------------
// ErrorCallbackInfo::CanNotify
// -------------------------------
//
// This is const just to be usable from const XMPMeta functions.

bool 
	XMPFiles::ErrorCallbackInfo::CanNotify() const
{
	XMP_Assert ( (this->clientProc == 0) || (this->wrapperProc != 0) );
	return ( this->clientProc != 0 );
}

// -------------------------------------------------------------------------------------------------
// ErrorCallbackInfo::ClientCallbackWrapper
// -------------------------------
//
// This is const just to be usable from const XMPMeta functions.

bool
	XMPFiles::ErrorCallbackInfo::ClientCallbackWrapper ( XMP_StringPtr _filePath,
														 XMP_ErrorSeverity severity,
														 XMP_Int32 cause,
														 XMP_StringPtr messsage ) const
{
	
	XMP_StringPtr filePathPtr = _filePath;
	if ( filePathPtr == 0 ) {
		filePathPtr = this->filePath.c_str();
	}

	XMP_Bool retValue = (*this->wrapperProc) ( this->clientProc, this->context, filePathPtr, severity, cause, messsage );
	return ConvertXMP_BoolToBool(retValue);
}

// -------------------------------------------------------------------------------------------------
// ErrorCallbackForXMPMeta
// -------------------------------

bool
	ErrorCallbackForXMPMeta ( void * context, XMP_ErrorSeverity severity, 
							   XMP_Int32 cause,
							   XMP_StringPtr message)
{
	//typecast context to GenericErrorCallback
	GenericErrorCallback * callback = ( GenericErrorCallback * ) context;
	XMP_Error error ( cause, message );
	callback->NotifyClient ( severity, error );
	return !kXMP_Bool_False;
}

// =================================================================================================
