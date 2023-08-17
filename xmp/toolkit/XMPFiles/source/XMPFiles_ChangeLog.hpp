#ifndef __XMPFiles_ChangeLog_hpp__
#define __XMPFiles_ChangeLog_hpp__ 1

// =================================================================================================
// Copyright 2004 Adobe
// All Rights Reserved.
//
// NOTICE:  Adobe permits you to use, modify, and distribute this file in accordance with the terms
// of the Adobe license agreement accompanying it. If you have received this file from a source other 
// than Adobe, then your use, modification, or distribution of it requires the prior written permission
// of Adobe.
// =================================================================================================

// =================================================================================================
// This file has two purposes, to provide the current engineering build number and to maintain a log
// of source changes going into each build. All but the most trivial changes should be noted here.
// This is the primary source for release notes. It is not necessary to change the build number for
// every change, but it should be updated prior to weekly builds if there are changes. The build
// number is maintained by hand. It reflects the state of the source. Official builds also include
// a Perforce changelist number and timestamp, so that separate builds from identical source can be
// differentiated.
// =================================================================================================

#if 0
	// ! Can't use this because Xcode does not do includes for plist processing.
	#include "public/include/XMP_Environment.h"	// ! Must be the first include.
#else
	#ifndef XMP_DebugBuild
		#if NDEBUG
			#define XMP_DebugBuild 0
		#else
			#define XMP_DebugBuild 1
		#endif
	#endif
#endif

#define kXMPFiles_EngBuild	 1
#define kXMPFiles_EngString	"001"

#if ! XMP_DebugBuild
	#define kXMPFiles_DebugSuffix
	#define kXMPFiles_DebugString ""
#elif ! IncludeNewHandlers
	#define kXMPFiles_DebugSuffix (debug)
	#define kXMPFiles_DebugString " (debug)"
#else
	#define kXMPFiles_DebugSuffix (debug/NewHandlers)
	#define kXMPFiles_DebugString " (debug/NewHandlers)"
#endif

#if AdobePrivate
// =================================================================================================
// Change history
// ==============
//
// Writers:
//	AWL Alan Lillich
//	JRS Jerry Scoggins
//	ERJ Eric Juteau
//	Rys Cris Rys
//	SDS Sanakar Dey Sarkar
//	ALB Alex Born
//	FNO Frank Nocke
//	PKA Patrick Kamin
//	JEH Joerg Ehrlich
//	GEM Gerry Miller
//	SAM Samy Makki
//	PW  Patrick Wollek
//	PKG Praveen Kumar Goyal
//	JKR Jens Krueger
//	IJS Inder Jeet Singh
//	ADC Amandeep Chawla
//	HK  Honey Kansal
//	SKP Sunil Kishor Pathak
//	AB  Amit Bhatti
//	ANS Ankita Sharma
//  AJ  Abhishek Jindal
//
// mm-dd-yy who M.m-bbb Description of changes, most recent on top.
//
// 04-02-15 XMP	It has been decided in internal meeting that Changelist description will no longer be written in this file but can be tracked using Perforce. Only version number will be bumped here. 
// 03-31-15 AB  5.6-f147 Adding support for 3gp and 3g2 formats as well as adding more brands for mp4.
// 03-24-15 AB  5.6-f146 Doing changes in SVG_Adapter to support for complex workflows of SVG and checking in unit test cases of SVG.
// 03-24-15 ADC 5.6-f145 Changing the mapping of iXML->TRACK to xmpDM:shotNumber in place of xmpDM:trackNumber.
// 03-24-15 ADC 5.6-f144 Adding std::stringsupport for reconciliation of iXML's TRACK_LIST in Wave files.
// 03-23-15 AB  5.6-f143 Restructuring safe update code for SVG_handler.
// 03-23-15 ADC 5.6-f142 Adding support for parsing and serializing TRACK_LIST tag in iXML block.
// 03-19-15 AB  5.6-f141 [3675137] Fixing memory leak due to zero length data in IPTC_Support.
// 03-13-15 AB  5.6-f140 Making SVG support only for UTF-8 SVG files.
// 03-12-15 AB  5.6-f139 Moving SVG_Adapter from XMPCore to XMPFiles. Expat parser source code is now available to XMPFiles.
// 03-05-15 AB  5.6-f138 Removing SVG format from packet scanner as smart handler has been provided for handling SVG files.
// 03-04-15 AB  5.6-f137 Improvement in SVG_Handler.
// 03-04-15 AB  5.6-f136 Initial creation of SVG_Handler.
// 02-17-15 AB  5.6-f135 Fixing XDCAM issue related with release configuration.
// 02-11-15 AB  5.6-f134 Making Exif properties as Read-Write properties for videos.
// 02-09-15 AB  5.6-f133 Restructuring the Exif Metadata support in MP4OEM_Handler and MOVOEM_Handler.
// 02-09-15 AJ  5.6-f132 Fixing some more warnings due to implicit typecasting
// 02-06-15 AJ  5.6-f131 Fixing warnings due to implicit typecasting
// 02-06-15 AB  5.6-f130 Redesign XDCAM Handler by dividing it into 2 child handlers XDCAMFAM_Handler and XDCAMSAM_Handler and making it more consistent as per latest spec.
// 02-05-15 AB  5.6-f129 For 50P and 59.94P, The Frame number for starttimecode should be written half to actual frame number in the XML file because NRT XML can store frame number value only between 00-29.
// 02-02-15 ADC 5.6-f128 Porting XMPFiles to gcc 4.8.x on Linux Platform.
// 01-31-15 SKP 5.6-f127 Fixing build break on UNIX
// 01-30-15 SKP 5.6-f126 Fixed linux build failure on releng ( warning: variable a set but not used [-Wunused-but-set-variable]
//						  #define XMP_FILES_STATIC_START try { int a;  & similar erros) on Linux gcc4.8.2 compiler.
// 01-28-15 SKP 5.6-f125 Ported XMPToolkit on gcc 4.8.2
// 01-08-15 AB  5.6-f124 Moving plugins MP4OEM_Handler and MOVOEM_Handler output binaries to optional folder parallel to public folder in XMPFilesPlugins.
// 01-06-15	AB	5.6-f123 Correcting the check while reconciling the metadata in Exif for Canon and FujiFilm's MPEG4 videos.
// 01-05-15	AB	5.6-f122 Provide more functionalities to Plugin( Existing XMP packet, PacketInfo, OpenFlags, Error Callback and progress notification),
//						 more standard handler access API getFileModDate,IsMetadataWritable,putXMP,getAssociatedResources.
//						 New plugin handler for MPEG4 with Exif support.
// 11-05-14 SKP 5.6-f121 Implemented MWG spec to read IPTC irrespective of IIM digest matching or not.
// 10-21-14 AB  5.6.f120 [3836549] Making kXMPFiles_OptimizeFileLayout flag public for XMP SDK.
// 09-03-14 AB  5.6-f119 Adding support of UUID atom in MPEG4 Handler.
// 08-27-14 ADC 5.6-f118 [3813039] [iXML] : Parse and map iXML <CIRCLED> property to xmpDM:good.
// 08-27-14 ADC 5.6-f117 [3812401] [iXML] : Fix issue with mapping of timecode rate from 24000/1001 to 23976Timecode.
// 08-26-14 AB  5.6-f116 [3812072] Generic Handler Bugfix.
// 08-19-14 ADC 5.6-f115 Fixing multiple issues with iXML support.
// 08-14-14 ADC 5.6-f114 Timecode computation from iXML information for wave file.
// 08-08-14 ADC 5.6-f113 Fixing test cases failure after last checkin.
// 08-07-14 ADC 5.6-f112 [3800239] [iXML] : Retain xmpDM mappings for iXML node properties to ensure backward compatibility.
// 08-04-14 ANS 5.6-f111 Ignoring non-integral values for nodes in iXML chunk in WAV files
// 08-04-14 AB  5.6-f110 [3747608] Invalid frame Count in tmcd sample in stsd entry.
// 08-03-14 ADC 5.6-f109 Fixing test case failure on Mac.
// 08-03-14 ADC 5.6-f108 Fixing debug asserts in case of any node is empty in iXML packet.
// 07-31-14 ADC 5.6-f107 iXML Support for Wave Files.
// 04-30-14 IJS 5.6-f106 [3735490] Modified  GetPreservedPathName to get the correct preserved folder and file names 
//												if  unicode characters are present in the path.
// 04-28-14 IJS 5.6-f105 [3751378] Initializing ClipTitle to NULL to fix a uninitialized dereferencing crash.
// 04-24-14 ADC 5.6-f104 Reverting back to the old strategy of creating a file in the folder in case we need to check writable.
// 04-23-14 ADC 5.6-f103 Fixing issue with OpenFolder function in case of Unicode Characters.
// 04-22-14 ADC 5.6-f102 Fixing issue with non NTFS shared folders on Windows.
// 04-22-14 IJS 5.6-f101 [3742456] Fixing P2 Handler to pick the clipName from clipFileName instead of P2 content XML.
// 03-26-14 IJS 5.6-f100 [3699475] Writing Old Digest in CanonXF XMP results in re-import of dc:title from .CIF file in the next read.
// 03-20-14 IJS 5.6-f099 Change filename filter for searching Clip Metadata XML.
// 03-19-14 IJS 5.6-f098 [3709896] P2 Handler Modified to support spanned clip.
// 03-13-14 IJS 5.6-f097 Fix the break after 5.6-f096.
// 03-12-14 IJS 5.6-f096 [3714412] Proxy and High Res Clip in XDCAM FAM should open the same XMP.
// 03-06-14 ADC 5.6-f095 Reverting changes done via CL # 155650 (MWG specification related).
// 02-06-14 IJS 5.6-f094 Excluded Files will not open with  GetAssociatedResources/IsMetadataWritable/GetFileModDate APIs
// 02-06-14 IJS 5.6-f093 Change the behavior of flag combination kXMPFiles_ForceGivenHandler and 
//                       kXMPFiles_OpenUseGenericHandler for GetAssociatedResources/IsMetadataWritable/GetFileModDate APIs
// 01-31-14 IJS 5.6-f092 Modified Generic Handler Serialize Options by adding kXMP_IncludeRDFHash as default flag
// 01-31-14 IJS 5.6-f091 [3696309, 3693612,3700229]Fix bugs for Generic Handling
//							Reverting Changes Done in 5.6-f089
// 01-24-14 HK  5.6-f090 [3696826] Generic Handler: If use GenericHandler flag is present in GetAssociatedResources/IsMetadataWritable/GetFileModDate APIs, 
//						then Generic Handler is always used even if Smart Handler is present for that format.
// 01-20-14 ADC 5.6-f089 [3696245] Generic Handler: Generic handler returns kXMP_Unknown type for handler->format.
// 01-16-14 IJS 5.6-f088 Generic Handling Feature - Adding support for kXMPFiles_OpenUseGenericHandler OpenFile() flag.
// 01-03-14 HK  5.6-f087 [3688857] Fixing data alignment issues on ARM processor.
// 12-11-13 HK  5.6-f086 [3682902] Defining mapping of Mac language to available encodings on IOS.
// 12-04-13 HK  5.6-f085 [3677602] Fixing Unicode conversion issues on IOS.
// 11-25-13 HK  5.6-f084 [3672894] Toolkit crashes while reconciling JPEG file. Fix:Reverting CL#155205 and a particular change in CL#153536.
// 11-22-13 HK  5.6-f083 [3672894] Toolkit crashes while reconciling JPEG file.
// 11-06-13 ADC 5.6-f082 [3658683] Error processing a large JPEG with multiple EXIF APP1 markers.
// 09-11-13 ADC 5.6-f081 Wrongly used trackNumber string in place of tempo.
// 09-11-13 ADC 5.6-f080 Tweaking the logic of reconciliation of tempo field with iTunes native data.
// 09-06-13 ADC 5.6-f079 Ignoring iTunes ReleaseDate in case it is same as that of stored in releaseDate property of DM namespace.
// 09-05-13 ADC 5.6-f078 [3594877] Cannot write property Dynamic Media / Disc Number into Quicktime files.
// 09-05-13 IJS 5.6-f077 [3611478] MP4/QT files- Differentiate incomplete box data from Garbage and on repair, truncate only if it is Garbage.
// 08-13-13 IJS 5.6-f076 [3206367,3206363] Enabling  repair of ill-formed structure in all MPEG4 Files.
//						 Enabling Error Notifications for Bad file format exceptions in MPEG4_Handler.
// 08-08-13 IJS 5.6-f075 [3557717]  Enabling  timecode import for ISO MP4 files.
// 07-22-13 ADC 5.6-f074 [3560723] For Folder based formats XMPFiles : OpenFile should throw an error when called on a file
//						 where the metadata files are read only and the option bit "OpenForUpdate" is used.
// 07-05-13 ADC 5.6-f073 In Terminate reset static variables.
// 07-04-13 ADC 5.6-f072 [3584418] MXF Handler : GetAssociatedresources API when called for MXF file returns the path
//						 to the sidecar even when it is not present.
// 07-04-13 ADC 5.6-f071 Fixing memory leaks in IPTC_Support.
// 06-21-13 ADC 5.6-f069 [1572860] Support PSIR in JPEG larger than 64K.
// 06-18-13 ADC 5.6-f068 [3556901] Save Metadata for some JPGs fails.
// 06-12-13 ADC 5.6-f067 Support for GoPro files in MPEG4 file handler.
// 06-11-13 IJS 5.5-f066 Moving #include vector out of AdobePrivate as now non-AdobePrivate function APIs in XMPFiles also 
//						 need declaration for vector to compile successfully
// 05-29-13 ADC 5.6-f065 [3565147] Plugin Architecture: If any unknown node is present in the Plugin Manifest then the Plugin is not loaded.
// 05-29-13 IJS 5.6-f064 [3533210] Added check for "RED2" in  CheckFormat() to detect and ignore it quickly.
// 05-28-13 IJS 5.6-f063 Fixed issues found creating XMP SDK - PDF_Handler now uses new version of SerializeToBuffer
//								[3565164] Fixed the Plug-in version incompatibility with CS6
//								Improved error handling in ImportToXMPString call chain.
// 05-24-13 ADC 5.6-f062 [3564457] Cannot write property Dynamic Media / Disc Number into Quicktime files.
// 05-15-13 ADC 5.6-f061 Integrating changes from DVA to MXF_Handler code base, Re factored class declaration and definition into separate files,
//						 and Added MXF_Handler into top level CMAKE.
// 05-13-13 ADC 5.6-f060 Removing usage of bool in APIs exposed at DLL boundaries.
// 05-09-13 ADC 5.6-f059 Fixing a potential problem due to not resetting pragma pack directive.
// 05-06-13 IJS 5.6-f058 Fix Checkformat and GetXMPStandard  Host  implementation issues
// 04-26-13 IJS 5.6-f057 Fix a typo in import code of ASF handler
// 04-26-13 IJS 5.6-f056 Make API to access overridden file handler available for public SDK
// 04-26-13 ADC 5.6-f055 [3532883] XMP libraries provide mechanisms to clients for registration of memory allocation and 
//						 de-allocation function pointers which causes problems with the static std::string obejcts in the code base.
// 04-26-13 ADC 5.6-f054 [3526891] Read-only checks in IsMetadataWritable() API try creating temporary files if no metadata exists, instead of checking for file permissions.
//						 [3525961] IsMetaDataWritable checks for Plugins and embedded handlers are different.
// 04-26-13 IJS 5.6-f053 [3533210] Optimize the code in the CheckFormat() API in MPEG handler if the first box is "REDV" or "RED1"
// 04-10-13 ADC 5.6-f052 [3432113] Garbage error message is returned if the XMP_Error thrown by Plugins are caught by any dll.
// 04-04-13 IJS 5.6-f051 Move common I/O methods to IOUtils.
//						 [3534631] Optimize GetAssociated Resources for RED_Handler
// 03-28-13 SAM 5.6-f050 [3528952] Fixed crash for some special SxS clips.
// 03-25-13 IJS 5.6-f049 Fixed the code to initailize the AVCHD_LegacyMetadata structure when calling from FillMetadataResources.
//						 Fixed code to ignore maker's private data if it is not that of panasonic when reading Playlist Extension Data.
// 03-25-13 IJS 5.6-f048 Update the R3D SDK to the latest version that includes GetRMDPath API.
//                       Use the GetRMDPath API to get the RMD Path inside RED_Handler. 
// 03-21-13 ADC 5.6-f047 [3526905] In MAC and Linux, progress notifications are not returned to the registered callbacks as per the set time-interval.
// 03-13-13 ADC 5.6-f046 Remove deprecated OSX APIs from XMP toolkit code.
// 03-02-13 ADC 5.6-f045 Incorporating Review comments for [3496361] Plugin Multi-threading.
// 03-02-13 ADC 5.6-f044 Incorporating Review comments for support for preloading plugins based on handler flags.
// 02-27-13 ADC 5.6-f043 Adding support for preloading plugins based on handler flags. PDF Handler will support preloading.
// 02-26-13 HK  5.6-f042 Support old sidecar naming convention for XMPilot and SxS formats to maintain backward compatibility.
// 02-21-13 HK  5.6-f041 [3503922] Support GetAssociatedResources and IsMetadataWritable API's for MPEG2.
// 02-20-13 JKR 5.6-f040 [3503209] Added try/catch-block in InitializePlugin2().
// 02-19-13 ADC 5.6-f039 [3496361] Plugin Multi-threading: Either crash or memory leak is there when multiple plugins are tried to be loaded simultaneously.
//                       Fixed crash on Linux.
// 02-18-13 JKR 5.6-f038 New plugin entry point InitializePlugin2() which call SetupPlugin().
// 02-18-13 ADC 5.6-f037 [3496361] Plugin Multi-threading: Either crash or memory leak is there when multiple plugins are tried to be loaded simultaneously.
//                       Fixed memory leaks on all platforms.
// 02-18-13 JKR 5.6-f036 [3473194] Handler version in plugin manifest file is used to resolve plugin handler conflicts
// 02-13-13 ADC 5.6-f035 [3498686, 3497304] XMP GetFileModDate() is extremely slow.
// 02-13-13 HK  5.6-f034 [3498318] In XDCAM_FAM handler, if an old side car of XMPilot can't be renamed, XMPFiles should at least allow to read it.
// 02-11-13 JKR 5.6-f033 Reworked plugin versioning to be able to load all supported older and newer plugins.
// 02-08-13 HK  5.6-f032 Rework for SonyHDV and XDCAM-EX spanning support in GetAssociatedResources() API.
// 02-08-13 HK  5.6-f031 [3492357, 3492355] Provide client callback when GetAssociatedResources/IsMetadataWritable() API's called for "unsupported" format.
// 02-05-13 HK  5.6-f030 Fixed bugs#3477099, #3495118, #3490341, #3492358.
// 01-30-13 IJS 5.6-f029 Removed LocateMetadataFiles API from XMPFiles
// 01-30-13 IJS 5.6-f028 [3477008] Modified XDCAM FAM(XMPilot) so as to preserve metadata from an old mxf Sidecar.
// 01-29-13 HK  5.6-f027 [3491927] Support spanning for XDCAM-EX GetAssociatedResources() API.
// 01-29-13 HK  5.6-f026 [3491777] Support spanning for SonyHDV GetAssociatedResources() API.
// 01-22-13 HK  5.6-f025 Rework of GetAssociatedResources() API done for XDACM-EX and P2 formats.
// 01-18-13 IJS 5.6-f024 Implemented GetAssociatedResources and IsMetadataWritable APIs for XDCAM Family.
// 01-17-13 HK  5.6-f023 Implemented GetAssociatedResources and IsMetadataWritable APIs for Sony-HDV format.
// 01-16-13 HK  5.6-f022 Implemented IsMetadataWritable API for XDCAM-EX, P2, CanonXF and AVCHD formats.
// 01-15-13 IJS 5.6-f021 Change GetFileModDate to use GetAssociatedResources for all handlers.
// 01-11-13 IJS 5.6-f020 Reworked GetFileModDate, GetAssociatedResources and IsMetadataWritable APIs.
//                       Added IsMetadataWritable Plugin API in the Plugin Architechture.Bumped the Plugin API to version 3.
//                       Fixed Bugs 3469857 and 3473168.
// 01-10-13 HK  5.6-f019 Worked on comments in review#1094601:"Added GetAssociatedResources API for XDCAM-EX format."
// 01-07-13 IJS 5.6-f018 Changed name of API IsWritable to IsMetadataWritable.
//                       Added root Path folder as associated Resource for folder based Formats.
// 01-04-13 HK  5.6-f017 [3435502] For XDCAM_EX folder format, GetAssociatedResources API should also return the path to the folder "ClipName/TAKR".
// 01-03-13 HK  5.6-f016 [3435383] Added GetAssociatedResources API for P2 format.
// 01-03-13 IJS 5.6-f015 [3429812] Added GetAssociatedResources API for CononXF format.
// 01-03-13 IJS 5.6-f014 [3429815] Added GetAssociatedResources API for AVCHD format.
// 01-02-13 HK  5.6-f013 Create PackageFormat_Support namespace to have common code for folder based handlers.
// 01-02-13 ADC 5.6-f012 Catching error from FillMetadataFiles and returning false.
// 01-02-13 ADC 5.6-f011 Allowing multiple different plugins to be loaded simultaneously.
// 01-02-13 HK  5.6-f010 [3429813] Added GetAssociatedResources API for XDCAM-EX format.
// 12-21-12 IJS 5.6-f009 Added FillAssociatedResources support in plugins 
// 12-21-12 IJS 5.6-f008 Added GetAssociatedResources and IsWritable APIs 
// 12-19-12 IJS 5.6-f007 Removing #if  DISABLE_SERIALIZED_IMPORT_EXPORT 
// 12-19-12 ADC 5.6-f006 [3427733] PluginRegistry isn't thread-safe.
// 12-19-12 IJS 5.6-f005 Making the importToXMP and exportFromXMP as public Plug-in methods
// 12-17-12 ADC 5.6-f004 Bumping version of XMPFiles Plug-in API to 2.
// 12-13-12 IJS 5.6-f003 [3424244] Fixed initialization of XDCAMEX Handler which was causing a crash.
// 12-13-12 IJS 5.6-f002 [3143977] Tweaks to identify the variant XDCAM Memory SxS of XDCAM FAM format.
// 12-11-12 ADC 5.6-f001 Bumping version to 5.6, reseting Eng build number.
//
// 12-10-12 IJS 5.5-f059 Adding Serialized versions for importToXMP and exportFromXMP Plug-in APIs.
// 12-10-12 ADC 5.5-f058 Incorporating review Comments for iTunes Support.
// 12-10-12 ADC 5.5-f057 Incorporating Review Comments for XMP Plug-ins support for LocateMetadataFiles API.
// 11-05-12 ADC 5.5-f056 XMP Plug-ins support for LocateMetadataFiles API.
// 11-01-12 IJS 5.5-f055 Changes done to support R3D Plugin Handler
// 11-01-12 ADC 5.5-f054 [3352773] XMPFiles::CloseFile() does not throw a WriteError when the file being updated is no longer available.
// 10-30-12 ADC 5.5-f053 Supporting long file paths on windows
// 10-18-12 ADC 5.5-f052 Support for iTunes metadata.
// 10-18-12 ADC 5.5-f051 [3347406] XMPFiles::OpenFile should throw an error when it is given a path to a file that is of an
//						 unsupported extension type along with the flag kXMPFiles_OpenUseSmartHandler.
// 10-16-12 ADC 5.5-f050 [3347167] XMPFiles::OpenFile() does not throw any exception when a file does not exist at the specified 
//						 location and the flag 'kXMP_UnknownFile' is passed to the function.
// 10-12-12 ADC 5.5-f049 [3345417] XMPFiles::OpenFile() does not throw any error when the path to a folder is given to the function.
//						 Added support for new Error code kXMPErr_UnsupportedFileExtension.
// 10-11-12 ADC 5.5-f048 Write and Truncate function calls on read only files should throw file permission errors.
// 10-10-12 ADC 5.5-f047 Fixing compile time issues on Mac and Linux where references to temporary object are not allowed.
// 10-10-12 ADC 5.5-f046 Fixing compile time issues which creeped into while doing merging of my changelist.
// 10-10-12 ADC 5.5-f045 Implement the internal infrastructure for XMPFiles error notifications.
// 10-09-12 SKP 5.5-f044 Refactored LocateMetadataFiles API.
// 10-09-12 IJS 5.5-f043 Reverting IOBuffer Changes because of perf issues in large PS files.
// 10-03-12 HK  5.5-f042 [3217688] A red X appears on the thumbnail in Panasonic P2 Viewer after updating the P2 media's metadata.
// 10-03-12 IJS 5.5-f041 [3170225] No reconciliation for Date fields if the native date is invalid in PS.
// 10-03-12 IJS 5.5-f040 Remove IOBuffer from PostScript Handler.
// 09-28-12 AWL 5.5-f039 Add LocateMetadataFiles.
// 09-28-12 ADC 5.5-f038 Implement public API and glue layer for XMPFiles error notifications.
// 09-21-12 IJS 5.5-f037 [3300999] Ignore timecode Sample if the Data references in timecode track of a QT file are external.
// 09-20-12 IJS 5.5-f036 [3333897]Progress Notifications change to add some work ( for File Handlers
//						  incapable of kXMPFiles_CanRewrite ) inside XMPFiles::CloseFile.
// 09-20-12 IJS 5.5-f035 Add Progress Notification flag to MPEG-4,PSD,AIFF,WAVE,ASF,TIFF,FLV and PS Handlers.
// 09-20-12 IJS 5.5-f034 Add file update progress tracking to the PostScript handler.
// 09-19-12 AWL 5.5-f033 [3211021] Fix the folder handlers complain about read-only XMP files when opening for update.
// 09-17-12	AWL	5.5-f032 Integrate tweaks to support XMPilot variant of XDCAM FAM.
// 09-17-12 AWL 5.5-f031 Refine the Host_IO exceptions for permission errors and disk full writes.
// 09-14-11	AWL	5.5-f030 Remove IOBuffer from the JPEG, TIFF, and PSD handlers.
// 09-13-12 AWL 5.5-f029 [3213337] Fix two SWF handler bugs: update full length in file header, and
//				recompress on output if the input was compressed.
// 09-04-12 IJS 5.5-f028 Add file update progress tracking to the TIFF handler.
// 09-04-12 IJS 5.5-f027 Add file update progress tracking to the FLV handler.
// 09-04-12 IJS 5.5-f026 Add file update progress tracking to the ASF handler.
// 09-04-12 IJS 5.5-f025 Add file update progress tracking to the AIFF and WAVE handler.
// 09-03-12 IJS 5.5-f024 Add file update progress tracking to the PSD handler.
// 08-28-12 IJS 5.5-f023 Added support to write(expand), inject and reconcile metadata in PS/EPS files.
// 08-21-12 AWL 5.5-f022 Remove use of IOBuffer from the ASF, GIF, and PNG handlers.
// 08-21-12 AWL 5.5-f021 Remove use of IOBuffer from the InDesign handler.
// 06-28-12 JEH	5.5-f020 Adding acceptance for mimetypes of Adobe Touch Apps: Collage, Ideas, Proto.
// 06-22-12 AWL 5.5-f019 Add file update progress tracking to the MPEG-4 handler.
// 06-20-12 AWL 5.5-f018 Add outer layers for XMPFiles progress notifications, no handlers use it yet.
// 06-14-12 AWL 5.5-f017 [2943495] Fix the TIFFFileWriter code to compact Exif that grows beyond 64KB.
// 06-08-12 JKR	5.5-f016 Introduced EnablePluginManager preprocessor define to disable plugins for iOS.
// 06-05-12 AWL 5.5-f015 [2999627] Improve ID3 genre handling.
// 05-09-12	AWL	5.5-f014 Make the JPEG, TIFF, and PSD handlers ignore XMP parsing errors.
// 05-03-12 JKR	5.5-f013 [3180369] added locks to fix thread safety issue when accessing mSessions in PluginManager
// 04-03-12 JKR	5.5-f012 [3138855] Fixed plugin manager to call checkFileFormat() in plugin if no CheckFormat 
//						 entry is provided in plugin manifest.
// 03-29-12	AWL	5.5-f011 [3151551] Fix timecode problem with Canon 5D Mark 3 video.
// 03-13-12	AWL	5.5-f010 Improve redundant PSIR handling.
// 02-28-12	JEH	5.5-f009 Fix writeTempFile plugin method to pass serialized XMP packet instead of file path.
// 02-02-12	AWL	5.5-f008 [3093755] Fix ASF handler truncation when updating with an almost full disk.
// 01-16-12	JEH	5.5-f007 [3092566] Fix GetFileModDate for CanonXF.
// 01-16-12	AWL	5.5-f006 [2980767] Improve sidecar file update for disk full situations.
// 01-16-12	JEH	5.5-f005 [3081446] Fix OpenFolder utility on win to cope with trailing slashes.
// 01-16-12	JEH	5.5-f004 [3081444] Fix GetFileModDate for XDCAM_EX.
// 01-12-12	AWL	5.5-f003 [3082003] Add support for the PluginResource Architecture attribute.
// 01-04-12	JEH	5.5-f002 [3073402] Make Cart chunk parsing tolerant if chunk size is too small.
// 01-03-12	JEH	5.5-f001 Bumping version to 5.5, reset build number.
//
// 12-19-11 AWL 5.4-f043 [1274437] Make the Exif processing tolerant of numeric tags with unexpected type.
// 12-02-11 AWL 5.4-f042 Separate ID3_Support header and code.
// 12-01-11 AWL 5.4-f041 [2715712] Ignore a bad IFD offset for the GPS and Interoperability IFDs.
// 11-29-11 AWL 5.4-f040 [3053308] Fix infinite loop in SWF decompression.
// 11-22-11 SAM 5.4-f039 [3014738] RIFF: Always read/write the last chunk if there are duplicates.
// 11-17-11 SAM 5.4-f038 [2981712] WAVE: Fixed bug in LIST:INFO parser to read empty LIST chunks.
// 11-16-11 JEH 5.4-f037 [2747840] CanonXF handler maps native value directly to xmp:CreatorTool now
// 11-16-11 SAM 5.4-f036 [2954004] Lurking bugs in WAV handler
// 11-11-11 AWL 5.4-f035 [2945829] Include QuickTime starting offset in the timecode string.
// 11-04-11 AWL 5.4-f034 [3015824,2731561] Be tolerant of ill-formed but understandable GPS values.
// 11-02-11 SAM 5.4-f033 Fix size calculcation in SWF decompress.
// 11-01-11 AWL 5.4-f032 [2935215] Make the InDesign handler tolerant of endian mistakes when reading.
// 10-31-11 AWL 5.4-f031 [2757711] Have Mac Host_IO::SwapData use a 3-way rename if fsexchangeobjects fails.
// 10-27-11 AWL 5.4-f030 [3007461] Improve performance of GetFileModDate.
// 10-21-11 AWL 5.4-f029 [3006421] Have existing XMP win over the Cr8r and PrmL boxes.
// 10-21-11 AWL 5.4-f028 [2774374] Integrate DMO edits for AVCHD problem with the .MPL and .CPI files.
// 10-21-11 AWL 5.4-f027 [2891842] Fix case sensitivity in Canon XF and XDCAM EX handlers.
// 10-19-11 SAM 5.4-f026 Integrated missing bugfix from winwood branch:
//						 [2977170,2810640] Audition 4 is overwriting the "Name" field from files created in previous versions of Audition
// 10-19-11 SAM 5.4-f025 [2984517] Adding mapping from/to Cart PostTimer Array for WAVE
// 10-18-11 AWL 5.4-f024 [3002257] Make the IPTC Manager tolerant of out of order IIM records.
// 10-12-11 AWL 5.4-f023 [2763298] Fix SelectSmartHandler to only extract the file extension for existing files.
// 10-12-11 AWL 5.4-f022 [2932182] Fix corruption of FLV files that have no OnMetadata tag.
// 10-12-11 SAM 5.4-f021 [2787609] MP3: Make Genre 0 in ID3 show as "Blues" in XMP (instead of "Other").
// 10-11-11 AWL 5.4-f020 [2918493] Make the TIFF support ignore tags with bad info instead of throwing an exception.
// 10-10-11 AWL 5.4-f019 [2831943] Fix SWF handler bugs causing it to further corrupt ill-formed files.
// 10-04-11 AWL 5.4-f018 [2958689] Fix the MPEG-4 handler to set all necessary parts of the non-XMP timecode.
// 09-30-11 AWL 5.4-f017 Add XDCAM (EX/FAM/SAM) support for GetFileModDate.
// 09-30-11 AWL 5.4-f016 Add P2 and SonyHDV support for GetFileModDate.
// 09-29-11 AWL 5.4-f015 Add AVCHD and CanonXF support for GetFileModDate.
// 09-27-11 AWL 5.4-f014 Add MPEG-2 support for GetFileModDate.
// 09-25-11 PW  5.4-f013 Add support to access replaced file handler.
// 09-23-11 AWL 5.4-f012 Add GetFileModDate.
// 09-20-11 AWL 5.4-f011 Fix bugs left in ID3 v2.2 support.
// 09-14-11 AWL 5.4-f010 Finish adding support for ID3 v2.2.
// 09-06-11 AWL 5.4-f009 Add support for ID3 v2.2 to ID3_Support.hpp.
// 08-29-11 JEH 5.4-f008 Remove file extension check from PluginManager Checkfileformat function
// 08-25-11 AWL 5.4-f007 Fix bugs in the handler registration and replacement logic.
// 08-15-11 AWL 5.4-f006 [2874815] Improve handling of Nikon JPEG files with full Exif APP1 segments.
// 08-11-11 AWL 5.4-f005 Fix regression in WAV import and export caused by cart chunk typo.
// 08-03-11 AWL 5.4-f004 Add support for the AES cart chunk in WAV files for real.
// 08-02-11 AWL 5.4-f003 Add support for the AES cart chunk in WAV files.
// 07-27-11 AWL 5.4-f002 Add support for new and modified Exif 2.3 tags.
// 06-27-11 PKG 5.4-f001 Initial checkin of plugin architecture.
//
// 05-19-11	AWL	5.3-f029 Fix sloppy but not buggy IOBuffer seek/fill actions in InDesign and PostScript handlers.
// 05-18-11	AWL	5.3-f028 [2875433] Fix IO_Buffer bug when refilling after skipping past the end of the current data.
// 04-29-11	AWL	5.3-f027 [2837254] Fix IPTC Manager to handle edge case IIM that no 2:xx portion.
// 						 [2860903] Fix TIFF import code to tolerate a too short UserComment.
// 02-02-11	SAM	5.3-f026 Adding XMP_Throw_Verbose macro for Debug only in order to add internal error
//						 codes to the message. Activating verbose PK Run for mac debug testrunner.
// 02-01-11	AWL	5.3-f025 [2800994] Apply fixes to Canon XF handler from Vivek Neelamegam.
// 01-28-11	AWL	5.3-f024 [2796677] Fix export to PSIR 1034 to follow missing/false/true model of xmpRights:Marked.
// 01-13-11	AWL	5.3-f023 [2691680] Fix box counting bug in MPEG-4 handler.
// 12-15-10	AWL	5.3-f022 Change Host_IO::GetNextChild to recognize AIX termination of readdir_r iteration.
// 12-07-10 PW  5.3-f021 export to local encoding: [2753033] BWF: On Wave format only: Double byte characters such as Japanese in BWF fields becomes ? marks when reopens the file after save
// 11-24-10	AWL	5.3-f020 [2755623,2755632] Add XDCAM fixes for non-XMP metadata problems.
// 11-24-10 PW  5.3-f019 [2753033] BWF: On Wave format only: Double byte characters such as Japanese in BWF fields becomes ? marks when reopens the file after save
// 11-23-10 JEH 5.3-f018 Fix of out of bounds access in ASF_LegacyManager::ComputeDigest
// 11-15-10	JEH	5.3-f017 [2757114] Negative or non-number XMP values are not written to unsigned number non-XMP properties anymore
// 11-02-10	PW	5.3-f016 [2551447] MP3 handler accepts files with non-standard Frame IDs now
// 10-26-10 PW  5.3-f015 Integration from winwood:
// 							Adjusting legacy mappings for ISRC and ITCH to transient listinfo namespace.
// 							Adding support for ordered arrays in new reconcile code.
// 							[2732321] WAV/AIFF: An update causes the file to grow unproportional to the added data.
// 							Adding new WAVE handler based on new IFF common code.
// 10-21-10	AWL	5.3-f014 Host_IO-UNIX and XMPFiles_IO tweaks as a result of unit testing.
// 10-21-10	AWL	5.3-f013 Host_IO tweaks as a result of unit testing, for real this time.
// 10-19-10	AWL	5.3-f012 Canon XF handler fixes from Peter Lee for improper property modeling.
// 10-08-10	AWL	5.3-f011 Host_IO tweaks as a result of unit testing.
// 09-30-10	AWL	5.3-f010 Add Canon XF handler from Peter Lee.
// 09-30-10 SAM 5.3-f009 Separate version numbers for XMPCore, XMPFiles and XMPScript.
// 09-07-10 AWL 5.3-f008 Extend support of OpenFile with XMP_IO to all appropriate formats.
// 08-27-10 AWL 5.3-f007 Add comments to Host_IO.hpp, fix semantic irregularities in the code.
// 08-26-10 AWL 5.3-f006 Move the folder info functions into the Host_IO namespace.
// 08-20-10 AWL 5.3-f005 Fix XMPFiles_IO::Truncate to make sure the I/O pointer is unmoved.
// 08-19-10 AWL 5.3-f004 Move the seek mode constants to XMP_Const.
// 08-19-10 AWL 5.3-f003 Remove all use of the LFA_* names.
// 08-18-10 AWL 5.3-f002 Don't include XIO.hpp in any headers, only .cpp files.
// 08-17-10 AWL 5.3-f001 Integrate I/O revamp to main.
//
// 08-09-10	JEH	5.2-f002 Making Endian classes Singletons; included minor tweaks from code review
//				and removed warnings from AIFF code.
// 07-28-10	JEH	5.2-f001 Bumping version to 5.2, reset build number;
//						 initial creation of new AIFF handler and adding it to XMPFiles.
//
// 07-01-10 AWL 5.1-f013 [2646065] Fix import and export of Exif GPSVersionID.
// 06-29-10 AWL 5.1-f012 [2447801] Make sure TrimTrailingSpaces is used for all Exif text imports.
// 06-28-10 AWL 5.1-f011 [2610127] Fix TIFF parsing length checks to be more accurate and robust.
// 06-24-10 AWL 5.1-f010 [2589499] Fix OECF and SFR import to not leave partial table on errors.
// 06-18-10 AWL 5.1-f009 [2539648] Fix Exif Artist handling of embedded commas and semicolons.
// 06-08-10 AWL 5.1-f008 [2581817] Fix erroneous release of name for memory-based PSIR resource.
// 05-26-10 SAM 5.1-f007 Include tweak for MPEG-4 update in order to workaround bug#2604185.
//						 Change order of processing to become (moov, XMP, free, mdat).
// 05-20-10 JEH 5.1-f006 Fix import of exif:GPSVersionID, import of exif:GPSTimeStamp also tolerates hyphens and
//				import of GPS coordinates tolerates zero denominators.
// 04-30-10 AWL 5.1-f005 Add AVCHD MakerModelCode constants from Peer Lee.
// 02-25-10 AWL 5.1-f004 [2558846] Fix ImportPhotoData to not trip over new alias for exif:DateTimeDigitized to xmp:CreateDate.
// 02-05-10 AWL 5.1-f003 Fix build warnings from Visual Studio.
// 02-05-10 AWL 5.1-f002 Fix build warnings from Xcode 3.2.
// 01-28-10 SAM 5.1-f001 Bump version to 5.1, reset build number.
//
// 12-22-09 AWL 5.0-f127 [2537954] Force the XMP chunk in RIFF to always have an even size.
// 01-09-10 SAM 5.0-f126 [2527779] Disable QT and iTunes metadata mapping.
// 12-22-09 AWL 5.0-f125 [2508484] Don't create Exif DateTimeDigitized or IIM DigitalDateCreated to
//				avoid unnecessary PSD full rewrite due to new mapping with xmp:CreateDate.
// 12-18-09 AWL 5.0-f124 [2505903] Redo the date/time fix to separate Exif DateTimeOriginal and IPTC DateCreated.
// 12-15-09 AWL 5.0-f123 [2505903] Add special handling for date-only photoshop:DateCreated to preserve Exif.
// 12-15-09 AWL 5.0-f122 [2500563] Force WAV text chunks to have even size.
// 12-14-09 AWL 5.0-f121 [2508013] Change the JPEG handler to look for Exif, PSIR, and XMP up to the first SOS marker.
// 12-03-09 AWL 5.0-f120 [2499192] Add special case support for exif:ISOSpeedRatings over 65535.
// 12-02-09 JEH 5.0-f119 [2497657] Change size variables from 32 to 64bit types in PSD handler to store sizes above 4GB.
// 11-20-09 AWL 5.0-f118 [2418287] Trim padding from Nikon full Exif APP1.
// 11-19-09 AWL 5.0-f117 [2480909] Fix AVI problems with LIST:INFO placement.
// 11-18-09 AWL 5.0-f116 Change loops in ContainerChunk::changesAndSize to avoid excessive MS iterator checks.
// 11-18-09 AWL 5.0-f115 Rewrite ContainerChunk::changesAndSize for better placement and simpler code.
// 11-17-09 AWL 5.0-f114 [2480954] Fix placement of LIST:Tdat chunk in RIFF files.
// 11-17-09 SAM 5.0-f113 [2457482] Ignore reading cr8r and prml chunks with wrong size.
// 11-16-09 AWL 5.0-f112 [2448942] Tolerate limited amounts of trailing garbage in RIFF files.
// 11-16-09 AWL 5.0-f111 [2457482] Finish support to read and write Cr8r chunk in AVI and WAV.
// 11-16-09 SAM 5.0-f110 [2471511] Fixing tolerance of missing pad byte at very end of file.
// 11-13-09 AWL 5.0-f109 [2471511] Restore file repair for some ill-formed RIFF files.
// 11-12-09 AWL 5.0-f108 [2476274] Remove internal limit of chunk size, accept any well-formed chunk.
// 11-12-09 AWL 5.0-f107 [2471511] Workaround issue of bad RIFF chunk size, tolerate on read and throw on write.
// 11-11-09 SAM 5.0-f106 [2473381] Fixed iter corruption after deletion of child in vector.
// 11-06-09 FNO 5.0-f105 [2473381] Fixed Cr8r,PrmL,TDat last-chunk-creation.
// 11-06-09 FNO 5.0-f104 [2473381] Assert 2/4GB border, prevent main growth, remove Index Adjustments.
// 11-06-09 FNO 5.0-f103 [2457482] Cr8r,PrmL read support.
// 11-05-09 FNO 5.0-f102 JUNK-Optimization II (64k buffer).
// 11-05-09 FNO 5.0-f101 JUNK-Optimization (in response to code review).
// 11-04-09 FNO 5.0-f100 [2473381] RIFF: deal with growth beyond 1,2 GB boundaries.
// 11-04-09 FNO 5.0-f099 RIFF: Fix beyond 4GB check. Applies to WAV-only.
// 11-04-09 FNO 5.0-f098 [2473303] RIFF: Fixed stomp-on-feet effect on rewrite.
// 11-03-09 FNO 5.0-f097 RIFF: Minor functional optimizations, comments, indentation, cleanup.
// 10-30-09 FNO 5.0-f096 [2465689] Fix avi indices. Metadata export re-enabled.
// 10-29-09 SAM 5.0-f095 [2430362] MP3: fix import/export of date and time.
// 10-26-09 FNO 5.0-f094 [2435625] RIFF: Prevent breaking index chunks.
// 10-26-09 AWL 5.0-f093 [1572075] Minor suggested changes from Hubert Figure.
// 10-23-09 FNO 5.0-f092 RIFF: Substantial rewrite of chunk logic to prepare for index adjustments.
// 10-21-09 AWL 5.0-f091 [1776893] Make sure temp files are deleted when exceptions get propagated.
// 10-20-09 AWL 5.0-f090 [2457482] Restore lost support for 'Cr8r' box in QuickTime files.
// 10-14-09 SAM 5.0-f089 [2338569] MP3: fix truncation of file length.
// 10-14-09 FNO 5.0-f088 Fix x64 compiler warnings in RIFF handler.
// 10-13-09 FNO 5.0-f087 [2414649] Fix "Bad Assert error".
// 10-08-09 AWL 5.0-f086 [2410350] Tweak MWG policy for better backwards compatibility.
// 10-07-09 AWL 5.0-f085 Minor tweaks for file layout optimization.
// 10-06-09 AWL 5.0-f084 [2426060,2434421,2424514] Fix TrimTrailingSpaces in ReconcileTIFF.cpp to handle all
//				blank input. Fix Mac script conversions to use the right script. Fix MPEG-4 imports
//				to not let one failure abort all. Fix TIFF handler to keep the XMP the same size if possible.
// 10-05-09 AWL 5.0-f083 Add MPEG-4 support for kXMPFiles_OptimizeFileLayout.
// 09-30-09 SAM 5.0-f082 [2424996,2443226,2424740] Integrate P2 and AVCHD fixes from Peter Lee.
// 09-24-09 AWL 5.0-f081 Revamp MPEG-4 handler to keep the entire 'moov' subtree in memory.
// 09-16-09 FNO 5.0-f080 [2430362] xmp:CreateDate import from ID3v2.
// 09-10-09 FNO 5.0-f079 Add ID3v1 and ID3v1.1 support to MP3 handler.
// 09-03-09 FNO 5.0-f078 [2419851] Enhance XDCAM metadata support (Fix provided by GEM).
// 08-31-09 AWL 5.0-f077 [2416566] Add special case fixup of negative GPSAltitude in Exif.
// 08-28-09 AWL 5.0-f076 [2417142] Fix export of timecode-derived QT 'udta' items.
// 08-27-09 AWL 5.0-f075 [2336123] Make TIFF importing be lenient about expected array sizes.
// 08-26-09 AWL 5.0-f074 [2398293] Fix MPEG-4 handler to not always think the iTunes metadata changed.
// 08-22-09 FNO 5.0-f073 [2389942] ServerMode: XMP props are not preserved if nothing is imported.
// 08-25-09 AWL 5.0-f072 [2369357] Fix TIFF_MemoryReader to be more robust for bad files.
// 08-25-09 AWL 5.0-f071 [2412960] Fix P2, AVCHD, and ASF handlers to create dc:creator as an ordered array.
// 08-24-09 AWL 5.0-f070 Use Erase/ApplyTemplate instead of RemoveProperties/AppendProperties in XMPFiles::GetXMP.
// 08-24-09 AWL 5.0-f069 Add support for QuickTime timecode information.
// 08-21-09 FNO 5.0-f068 RIFF-Handler: Fix leak un invalid odd-byte umid.
// 08-21-09 FNO 5.0-f067 RIFF-Handler: Fix leak when encountering invalid umid.
// 08-20-09 FNO 5.0-f066 RIFF-Handler: Edge case: First chunk is JUNK.
// 08-20-09 FNO 5.0-f065 RIFF-Handler: Fix three minor bugs found in code review.
// 08-19-09 FNO 5.0-f064 RIFF-Handler: Add bext chunk write support.
// 08-18-09 FNO 5.0-c050f63 RIFF-Handler: Add full bext chunk read support.
// 08-14-09 FNO 5.0-c49f62i14s002 [2387633] Add missing CFBundleVersion in Info.plist.
// 08-11-09 AWL 5.0-f061 Integrate P2 and AVCHD changes from Peter Lee. Add "r3d" (Red) to rejected file list.
// 08-10-09 AWL 5.0-f060 Assume denominator of 1 if XMP for rational Exif has just an integer.
// 08-07-09 AWL 5.0-f059 Add additional TIFF and Exif "inject-only" tags for Bridge.
// 07-29-09 FNO 5.0-f058 [2389942] XMP props not properly preserved in ServerMode.
// 07-27-09 FNO 5.0-f057 Implement ServerMode, reconciliation precedence, prior digests.
// 07-16-09 FNO 5.0-f056 [2376832] JUNK padding chunks... now also fixed for WAV(DISP chunk). Code simplified.
// 07-15-09 FNO 5.0-f055 [2376832] JUNK padding chunks are not used effectivly. File grows.
// 07-13-09 FNO 5.0-f054 Store XMP and Legacy redundantly (instead of non-redundantly).
// 07-13-09 FNO 5.0-f053 [1521093] Proper handling of RIFF files with no last pad byte.
// 07-10-09 FNO 5.0-f052 Move RIFF (AVI/WAV) handler from NewHandlers to regular Handlers.
// 07-01-09 JEH 5.0-f051 Fix wrong deallocation of variable mimetype in CheckFormat of UCF handler
// 06-25-09 FNO 5.0-f050 Add LFA_Write functions for numeric data types. Completing Peek functions.
// 06-23-09 AWL 5.0-c040-f049 Add hack to revert to global DLL locking.
// 06-23-09 AWL 5.0-c039-f048 Fix atomic increment and decrement to be optional, not on Solaris.
// 06-22-09 AWL 5.0-f047 Fix locking code to use atomic increment and decrement.
// 06-18-09 AWL 5.0-f046 Minor tweaks from code review.
// 06-12-09 AWL 5.0-f045 Simplify the multiple lock permutations.
// 06-12-09 FNO 5.0-f044 [2277195] Add XMP Support for UCF(based) .idap and .icap CS5 file formats.
// 06-11-09 AWL 5.0-f043 Finish threading revamp, implement friendly reader/writer locking.
// 05-27-09 FNO 5.0-f042 [2352188] MP3: map ID3v2:TCOP to dc:rights['x-default'].
// 05-27-09 AWL 5.0-f041 Remove XMPMeta::SendAssertNotify.
// 05-21-09 AWL 5.0-f040 Revamp glue again to pass SetClientString with each function.
// 05-19-09 AWL 5.0-f039 First part of threading improvements, revamp the client glue.
// 05-14-09 AWL 5.0-f038 Improve the call tracing mechanism.
// 05-12-09 AWL 5.0-f037 Finish deprecated function removal.
// 05-07-09 FNO 5.0-f036 Start combined RIFF(AVI/WAV) Handler rewrite.
// 05-06-09 AWL 5.0-f035 [2307195] Fix Mac text conversions to work better on Windows.
// 04-21-09 AWL 5.0-f034 [2271936] Import and export Exif date/time values with spaces for missing parts.
// 04-20-09 AWL 5.0-f033 [2307195] Add language parameter for Mac text conversions.
// 04-03-09 AWL 5.0-f032 Fix Terminate to be tolerant of too many calls (count going negative).
// 03-30-09 AWL 5.0-f031 [1830967,1891113,1891119] Minor P2 fixes for native tag imports.
// 03-30-09 AWL 5.0-f030 [2306833] Fix first loop in ExportISOCopyrights to cover index 0.
// 03-26-09 AWL 5.0-f029 More QT related fixes. Properly handle OpenStrictly for MPEG-4 and MOV.
//				FIll in remaining Mac langXyz to XMP (ISO 639) mappings. Add 0xA9 to set of OK chars
//				for unexpected top level boxes in CheckFormat. Fix byte order of ISO 'cprt' language.
//				Output iTunes metadata if it did not yet exist.
// 03-16-09 AWL 5.0-f028 Incorporate MOV file repair into new QuickTime support.
// 03-10-09 AWL 5.0-f027 Initial integration of QuickTime support into MPEG-4 handler.
// 02-06-09 FNO 5.0-c007c026 Add scannable ADOBE_INFO to VersionInfo (mostly for Linux/Unix).
// 02-03-09 FNO 5.0-f025 Setting XMP_IO* to XMP_Int32 (before: void*) for UNIX_ENV (Linux/Unix).
// 01-30-09 AWL 5.0-f024 [2268451] Fix PostScript checks of DSC version numbers.
// 01-29-09 AWL 5.0-f023 [1699977] Additional fix for XDCAM-FAM, don't shift leafName to uppercase.
// 01-29-09 AWL 5.0-f022 [1699977] Fixes for UNIX file case sensitivity.
// 01-21-09 AWL 5.0-f021 Fix AVI and WAV handlers for 64-bit builds.
// 01-15-09 AWL 5.0-f020 Split handlerTemp into tempPtr and tempUI32.
// 01-12-09 AWL 5.0-f019 Fix SelectSmartHandler to close file if necessary at "middle" default handler check.
// 01-08-09 AWL 5.0-f018 Fix regression introduced in f017.
// 01-07-09 AWL 5.0-f017 [1937288] Fix import and export of exif:UserComment. Fix deletion of entire IFD.
// 01-07-09 FNO 5.0-f016 Undoing f014 change (affects UNIX/Linux only).
// 01-06-09 AWL 5.0-f015 [1937288] Do delete all of tiff: and exif: on import, saving new writebacks.
//				Change import of Exif RelatedSoundFile to accept any length.
// 01-06-09 FNO 5.0-f014 Proper conversion of POSIX file pointers to descriptors (affects UNIX/Linux only).
// 01-05-09 AWL 5.0-f013 [1937288] Don't delete all of tiff: and exif: on import, just the digests.
// 01-05-09 AWL 5.0-f012 [1937288] Export Exif GPSTimeStamp (plus date), GPSAltitude (plus ref).
// 12-19-08 FNO 5.0-f011 [1913500] Add seven more known camera raw extensions to list.
// 12-18-08 AWL 5.0-f010 [1932925] Fix TIFF handler to ignore trailing zero bytes for LONG tag 33723.
//				Don't parse read-only IPTC if the digest matches, always parse for update.
// 11-21-08 AWL 5.0-f009 Fix TIFF imports to check for empty string after calling TrimTrailingSpaces.
// 11-18-08 AWL 5.0-f008 Fix server mode to not delete XMP for non-ASCII input that is ignored.
// 11-17-08 AWL 5.0-f007 Fix IIM output to write 1:90 instead of 1:190.
// 11-17-08 FNO 5.0-f006 Move MP3 handler CS5 rewrite from NewHandlers to regular handlers.
// 11-13-08 AWL 5.0-f005 Fix a couple of missed cases of UNIX build hacks.
// 11-13-08 AWL 5.0-f004 Add server mode support that ignores local text. Enable all handlers except
//				MOV for generic UNIX - that will be handled as part of the rewrite.
// 11-05-08 FNO 5.0-f003 [1913500] Add Lecia's .RWL extension to hard coded lists of RAW file extensions.
// 10-31-08 AWL 5.0-f002 MWG compliance changes: Fix Exif Artist and date/time mapping bugs.
// 10-30-08 AWL 5.0-f001 Bump version to 5.0, reset build number.
//
// 10-30-08 AWL 4.4-f016 MWG compliance changes: Always write IIM using UTF-8.
// 10-23-08 AWL 4.4-f015 MWG compliance changes: Don't keep device properties in the file's XMP;
//				mapping changes for 3-way properties, especially description and date/time.
// 10-14-08 AWL 4.4-f014 MWG compliance changes: Simplified block selection.
// 10-13-08 AWL 4.4-f013 Remove internals of GetThumbnail.
// 10-13-08 AWL 4.4-f012 Add support for zone-less times.
// 09-30-08 AWL 4.4-f011 [1899539] Fix ASF handler to not create empty XMP values for missing native items.
// 09-08-08 FNO 4.4-f010 [1817149] Show company names in file explorer properties tab.
// 07-20-08 FNO 4.4-f009 Enabling ASF handler for SDK (no functional impact on private builds).
// 07-18-08 AWL 4.4-f008 [1854413] Add check and repair for bad size in outermost RIFF chunk.
// 07-17-08 FNO 4.4-f007 [1841019] Better fix to also detect rsrcFork-free files under 10.4.
// 07-15-08 AWL 4.4-f006 [1847764] Change FLV to use onXMPData instead of onXMP.
// 07-09-08 FNO 4.4-f005 [1841019] Better fix, eniency on CatalogInfo for non-MacOS file systems.
// 07-04-08 FNO 4.4-f004 [1841019] Further refinement of fix. affects mac only.
// 07-04-08 FNO 4.4-f003 [1841019] Leniency on CatalogInfo for non-MacOS file systems. affects mac only.
// 07-03-08 FNO 4.4-f002 [1832465] Enabling CS4 InCopy extensions (UCF+XML).
// 07-02-08 FNO 4.4-f001 Bumping build number to 4.4, reset Eng Builder Numbers to 1.
//
// 06-24-08 AWL 4.3-f125 [1830156] Make sure exif:UserComment is a LangAlt.
//
// 06-06-08 AWL 4.2.2-f124 [1817488] Fix MOV copy of Cr8r code to properly copy bounded strings.
//				Fix MOV, AVI, and WAV to ensure that native Cr8r strings are terminated.
// 06-05-08 AWL 4.2.2-f123 [1816329] Remove code setting xmpDM:videoPixelAspectRatio.
// 06-04-08 AWL 4.2.2-f122 [1813875] Fix AVI_MetaHandler::CacheFileData to delete bad forms of
//				xmpDM:startTimecode and xmpDM:altTimecode. Fix XMPFiles::OpenFile to delete the
//				handler and close the file if <handler>::CacheFileData throws an exception.
// 06-04-08 AWL 4.2.2-f121 [1781226] Fix RIFF_Support to detect bad tag lengths.
// 05-29-08 FNO 4.2.2-f120 [1806526] Report image width and pixel aspect ratio correctly for P2 HD formats.
// 05-28-08 FNO 4.2.2-f119 [1799268] Be lenient on files/folders in P2.
// 05-23-08 FNO 4.2.2-f118 [1799808] Added raw extension .3fr to kKnownRejectedFiles.
// 05-23-08 FNO 4.2.2-f117 [1780960] Adding fix provided by Dale Weaver.
// 05-22-08 FNO 4.2.2-f116 [1781657] Fixing WAV part of bug (fix now complete).
// 05-21-08 FNO 4.2.2-f115 [1781657] Fixing AVI part, and common RIFF portion of bug.
// 05-19-08 JEH 4.2.2-f114 [1786791] Activating packet scanner for AEP and AEPX files
// 05-09-08 AWL 4.2.2-f113 [1780690] Tolerate ID3 padding, and avoid buffer overflow from bad tag lengths.
// 05-06-08 AWL 4.2.2-f112 [1780077] Fix InDesign handler to use the proper "live" XMP length when updating.
// 05-06-08 AWL 4.2.2-f111 [1779236] Fix endian problems in Cr8r info in MOV and RIFF files.
// 04-23-08 AWL 4.2.2-f110 Integrate Sony HDV handler update from Jim Mork.
// 04-22-08 AWL 4.2.2-f109 [1740071] Use Exif DateTimeOriginal to set a missing xmp:CreateDate.
// 04-17-08 AWL 4.2.2-f108 [1521093] Fix RIFF_Support::MakeChunk to add missing pad byte.
// 04-16-08 AWL 4.2.2-f107 [1741260] More fixes to AVI and WAV.
// 04-14-08 AWL 4.2.2-f106 [1741260] Integrate updates to AVCHD, AVI, MOV, Sony HDV, and WAV.
// 04-10-08 AWL 4.2.2-f105 [1741255] Fix double negative in AVCHD file checks.
// 04-09-08 FNO 4.2.2-f104 [1720135] Integrate large ucf (zip-64) support. LFA_Routines to source/LargeFileAccess.cpp|hpp.
// 03-31-08 AWL 4.2.2-f103 [1741470] Integrate P2 bug fixes.
// 03-31-08 AWL 4.2.2-f102 [1741260] Integrate additional native metadata for AVI, WAV, and MOV.
// 03-31-08 AWL 4.2.2-f101 [1741255] Fix AVCHD handler to use MovieObj.bdm.
// 03-31-08 AWL 4.2.2-f100 [1736560] Fix P2 handler to read dc:title using GetLocalizedText.
// 03-31-08 AWL 4.2.2-f099 [1732028] Don't add empty legacy items to AVI.
// 03-28-08 PKA 4.2.2-f098 [1707700] SWF handler: Fix scaling down to smaller XMP
// 03-27-08 FNO 4.2.2-f097 UCF handler: tolerate extranous \0xA on mimetype [16980028].
// 03-21-08 AWL 4.2.2-f096 [1726253] Fix backwards test for RIFF comment chunk in AVI_MetaHandler::UpdateFile.
//
// 03-07-08 AWL 4.2-f095 [1708927] Fix SWF_Support's FileInfo::Inf to properly track the source position.
// 03-06-08 AWL 4.2-f094 Integrate changes to the Sony HDV handler.
// 03-06-08 AWL 4.2-f093 Change the XDCAM file type constants to be explicit about all 3: FAM/SAM/EX.
// 03-05-08 AWL 4.2-f092 [1708630] Fix ASF bug in updating the File Properties object's file size field.
// 03-05-08 FNO 4.2-f091 Fix temp file user rights on Linux.
// 03-03-08 AWL 4.2-f090 [1706551] Fix empty XMP and legacy handling for JPEG, PSD, and TIFF.
// 03-03-08 AWL 4.2-f089 [1705112] Follow on hack to clean up the f087 changes to SelectSmartHandler.
// 03-03-08 AWL 4.2-f088 [1704032] Fix references to legacy .XML file in the XDCAM handlers.
// 02-28-08 AWL 4.2-f087 [1704032] Fix P2 and XDCAM code to check for NULL pointers from XML tree methods.
//				Fix SelectSmartHandler to differentiate file-oriented and folder-oriented cases, the
//				check procs have different parameters.
// 02-27-08 AWL 4.2-f086 [1699986] XMPFiles::OpenFile is sometimes opening the file more than once,
//				then closing only the last open, leaving the file open on exit.
// 02-27-08 AWL 4.2-f085 [1700696] Fix in-place update logic for JPEG, PSD, and TIFF handlers.
// 02-25-08 FNO 4.2-f084 Integrate minor tweaks for Solaris
// 02-25-08 AWL 4.2-f083 [1692876] Fix XDCAMEX_CheckFormat to be case insensitive for BPAV.
// 02-22-08 AWL 4.2-f082 Fix XDCAM_CheckFormat to be case insensitive for PROAV. Remove the CUEUP.XML
//				check from XDCAMEX_CheckFormat. Fix handerTemp leaks for all folder-oriented handlers.
// 02-22-08 FNO 4.2-f081 UCF-handler: Add accepted xfl, pdfxml extensions, rejected air.
// 02-20-08 AWL 4.2-f080 [1692876] Fix XDCAMEX_CheckFormat to handle both logical and full file paths
//				properly. It was appending an extra "/BPAV" component to full file paths.
// 02-20-08 AWL 4.2-f079 Disable all handlers except JPEG for generic UNIX, even packet scanning.
//				Temporary tactic to control risk for Ozzy with Photoshop Express, to be revisited.
// 02-19-08 AWL 4.2-f078 Don't try to muck with ownership when creating temp files.
// 02-18-08 AWL 4.2-f077 More changes to generic UNIX builds for XMPFiles.
// 02-15-08 AWL 4.2-f076 More changes to generic UNIX builds for XMPFiles.
// 02-15-08 AWL 4.2-f075 Integrate more folder-oriented handler updates. Initial changes to create
//				generic UNIX builds for XMPFiles.
// 02-13-08 PKA 4.2-f074 [1668559] Added consideration of file size to compressed SWF handling.
// 02-13-08 AWL 4.2-f073 [1538320] Add GPS Latitude and Longitude to the Exif writeback set.
// 02-11-08 AWL 4.2-f072 Add 3 more MPEG-4 standard file extensions.
// 02-11-08 AWL 4.2-f071 Enable SXMPFiles::CheckFileFormat, the leaks were not real.
// 02-07-08 AWL 4.2-f070 Test, fix bugs, and move the advanced video handlers out of NewHandlers.
// 02-05-08 AWL 4.2-f069 Integrate latest advanced video handlers.
// 02-05-08 AWL 4.2-f068 Use client memory routines for malloc/free also, so that leak checking works.
// 02-01-08 JEH 4.2-f067 Moved MPEG4 and FLV to regular handlers.
// 01-30-08 AWL 4.2-f066 [1648922] Fix TIFF_MemoryReader to not assume pointers are 32-bit.
// 01-30-08 AWL 4.2-f065 [1643402] Always strip IIM and PSIR from DNG. Have TIFF handler reject DNG
//				if DNGBackwardVersion has a high order byte that is not 1 (>= 2.0.0.0).
// 01-29-08 AWL 4.2-f064 [1643402] Don't write IIM or PSIR blocks into DNG files.
// 01-29-08 AWL 4.2-f063 [1652570] Preserve the name of PSIRs.
// 01-28-08 AWL 4.2-f062 Add public CheckFileFormat and CheckPackageFormat functions.
// 01-22-08 FNO 4.2-f061 [1580732] XMPFiles should not write XMP into Camera Raw files.
// 01-22-08 AWL 4.2-f060 Fix SplitLeafName to tolerate partial paths.
// 01-22-08 AWL 4.2-f059 First working, minimally tested FLV handler.
// 01-21-08 AWL 4.2-f058 Global tweak to have the handler's packetInfo always refer to the file.
// 01-16-08 AWL 4.2-f057 Remove private parent pointer from MPEG-4, P2, SonyHDV, and XDCAM. Like all
//				handlers they must use the parent pointer in the XMPFileHandler base class.
// 01-15-08 AWL 4.2-f056 [1665011] Fix assertion failures in MPEG-4.
// 01-08-08 FNO 4.2-f055 Add MOV_NewHandler to NewHandlers (release builds unchanged).
// 01-07-08 AWL 4.2-f054 Fix some of the easy 64-bit compile warnings.
// 01-01-08 AWL 4.2-f053 Move the legacy digests for P2 and XDCAM to the xmp:NativeDigests struct.
// 12-31-07 AWL 4.2-f052 Integrate P2 and XDCAM handler changes from Peter Lee and Gerry Miller.
// 12-31-07 AWL 4.2-f051 Add initial Sony HDV handler under NewHandlers.
// 12-21-07 AWL 4.2-f050 Add initial skeleton FLV handler under NewHandlers.
// 12-21-07 AWL 4.2-f049 First working version of MPEG-4 handler, still under NewHandlers.
// 12-14-07 AWL 4.2-f048 [1645489] Fix 64-bit failure for InDesign databases.
// 12-13-07 AWL 4.2-f047 Fix P2 handler to set containsXMP when legacy is imported.
// 12-13-07 PKA 4.2-f046 Fixed mishandling of existing metadata tags in SWF files without file attribute tag.
// 12-11-07 FNO 4.2-f045 Moved UCF to regular handlers.
// 12-10-07 AWL 4.2-f044 Add P2 legacy digest and legacy .XML file update. Fix namespace handling.
// 12-06-07 AWL 4.2-f043 Add basic XDCAM handler.
// 11-30-07 AWL 4.2-f042 Add skeleton of legacy reconciliation with .XML file.
// 11-29-07 AWL 4.2-f041 First working version P2 handler in NewHandlers, XMP-only, no legacy.
// 11-28-07 AWL 4.2-f040 Introduce empty P2 handler in NewHandlers.
// 11-28-07 AWL 4.2-f039 Add folder utils and implement TryFolderHandlers.
// 11-28-07 FNO 4.2-f038 Made MOV-handler and other exclusions/changes needed for x64 windows.
// 11-16-07 AWL 4.2-f037 Introduce basic infrastructure for "directory oriented" handlers.
// 11-14-07 PKA 4.2-f036 Add multiple metadata tag support to SWF Handler. Write XMP tag at the
//				beginning of the file. SWF handler moved out of NewHandler section.
// 11-13-07 AWL 4.2-f035 Add MPEG-4 skeleton handler. Rename MPEG handler to MPEG-2.
// 11-13-07 FNO 4.2-f034 Made XIO::Move() more flexible to support same and different file as target.
// 11-08-07 FNO 4.2-f033 Added XMP_Validate() macro.
// 11-05-07 AWL 4.2-f032 [1596344] Fix the MOV handler to presume QuickTime is available when
//				kXMPFiles_NoQuickTimeInit is passed to XMPFiles::Initialize.
// 10-25-07 FNO 4.2-f031 Added XIO::Move() to move data within a file.
// 10-23-07 FNO 4.2-c22f30s11 added 'acrobat' linker flags and buffer checks
// 10-16-07 PKA	4.2-f029 Added compressed swf support to SWF handler. Considering of HasMetadata flag.
// 10-16-07 FNO 4.2-f028 UCF read-only in NewHandlers(not final), added LFA_Read* convenience routines to XMPFiles
// 10-10-07 AWL 4.2-f027 Add handler method GetSerializeOptions. Introduce FillPacketInfo to replace
//				GetPacketCharForm, GetPacketRWMode, and GetPacketPadSize. And tolerate no wrapper.
// 10-09-07 FNO 4.2-f026 Added UCF draft to NewHandlers.
// 07-17-07 AWL 4.2-f025 Change Mac to use pthread synchronization, needed for 64-bit builds.
// 05-30-07 AWL 4.2-f024 Change operator new override to not throw for 0 length request, gcc on AIX
//				returns a null pointer for 0 length.
// 05-15-07 ALB 4.2-f023 New version #defines (SRM).
// 04-18-07 AWL 4.2-f022 Add dng to kFileExtMap, mapping to the TIFF handler.
// 03-27-07 AWL 4.2-f021 [1512496] Add a legacy digest for WAV.
// 02-23-07 AWL 4.2-f020 [1493588] Add support for IPTC that has a 1:90 DataSet specifying UTF-8.
// 02-22-07 AWL 4.2-f019 [1491614] Fix the MOV handler to use Unicode aware QT file APIs.
// 02-21-07 AWL 4.2-f018 [1491825] Another round of nascent MP3 bugs coming out of the ground. The
//				MP3 handler was not reporting the proper offset and length of the original (in file)
//				XMP packet. This made the common code make wrong decisions about in-place updates.
//				If the ID3 had to grow, the ID3 support failed to seek back to the file start after
//				shifting the audio data and before writing the revised ID3.
// 02-13-07 AWL 4.2-f017 Centralize the Adobe IP constant.
// 02-06-07 AWL 4.2-f016 Version info tweaks.
// 02-02-07 AWL 4.2-f015 [1477631] Fix ID3_Support to properly inject an ID3 section into an MP3 file.
// 01-30-07 AWL 4.2-f014 [1471832] Change the JPEG/PSD/TIFF handlers to do a forced legacy import if
//				the XMP portion won't parse.
// 01-26-07 AWL 4.2-f013 [1471566] Repair the lost use of XMPMeta::SendAssertNotify for XMPFiles asserts.
// 01-26-07 ALB 4.2-f012 [1406374] ASF: padding optimizations.
// 01-25-07 AWL 4.2-f011 [1465694] Change legacy import logic to be a little closer to Photoshop in
//				an edge case where the IPTC digest is missing.
// 01-23-07 AWL 4.2-f010 Finish changing the copyright to 2007.
// 01-16-07 AWL 4.2-f009 [1457497] Fix PSD handler to do in-place update of XMP at the right offset.
//				Improve ExportIPTC_DateCreated to not create the IPTC time (2:60) unless necessary.
// 01-15-07 AWL 4.2-f008 Change copyright to 2007.
// 01-11-07 AWL 4.2-f007 [1454747] Change QuickTime init/term calls to handle main and background thread issues.
// 01-10-07 AWL 4.2-f006 [1454756] Change PSIR_Support to handle any resource type, not just "8BIM".
//          ALB          [1406374] Fixed nasty placement of parenthesis in ASF handler (ASF_Support::ReadHeaderExtensionObject).
// 12-20-06 AWL 4.2-f005 [1440767] Work around fsmakefsspec failure to handle leading "./" on Windows.
// 12-20-06 AWL 4.2-f004 [1446159] Change the Mac version of LFA_Seek to support seeks past EOF with
//				implicit file growth. Fix an MP3 growth problem introduced with ID3_Support.cpp #5.
//				LFA_Write got changed to LFA_Seek in the loop of OffsetAudioData.
// 12-07-06 AWL 4.2-f003 [1435660] Disable the lossless IPTC support (IPTC as UTF-8).
// 12-06-06 AWL 4.2-f002 [1425907] Move build info (branch, changelist, date) to the FileVersion tag.
// 12-04-06 AWL 4.2-f001 Bump version to 4.2.
//
// 11-30-06 AWL 4.1-f081 [1432641] The fixes for bug 1429154 (XMPFiles 4.1-f075) broke the handling
//				of TIFF files larger than 128K. A header consistency check is using an I/O buffer
//				size for the total file size.
// 11-30-06 AWL 4.1-f080 [1432298] Preserve UTF-8 characters when truncating values for the IIM limits.
//				If the truncation would be in the middle of a multi-byte character, back up to the
//				beginning of that character.
// 11-30-06 ALB 4.1-f079 [1426158] ASF: Native metadata added through XMP is not seen or modifiable
//				in Windows Media File Editor. Set updated filesize in File Properties Object.
// 11-30-06 AWL 4.1-f078 [1432498] Fix the JPEG thumbnail recognition broken in 4.1-f075.
// 11-28-06 AWL 4.1-f077 [1378220] Add conditional code to gather top level performance data.
// 11-28-06 AWL 4.1-f076 [1430326] Fix the JPEG handler to create all forms of legacy metadata if
//				necessary. It was failing to create PSIR and IPTC for empty (no legacy) input.
// 11-27-06 AWL 4.1-f075 [1429154] Make the TIFF parser tolerate no IFDs and empty IFDs. Both are
//				formally invalid, but there are real world instances that we need to accept.
// 11-27-06 AWL 4.1-f074 [1428151] Fix XMPFiles::PutXMP to check for a handler before dereferencing.
// 11-23-06 ALB 4.1-f073 [1396247] Use "official" numeric types (PNG, SWF, ASF:GUID on non-Win platforms).
// 11-20-06 ALB 4.1-f072 [1424235] ASF: limit legacy data to maximum possible field sizes.
// 11-17-06 ALB 4.1-f071 [1424223] ASF: be more strict when reading files.
// 11-16-06 AWL 4.1-f070 [1423378] Fix legacy import/export to catch per-property exceptions and keep going.
// 11-16-06 ALB 4.1-f069 [1406374] ASF: optimize use of padding objects.
// 11-15-06 AWL 4.1-f068 [1418029] Update the IPTC legacy support to match the IPTC4XMP specification.
// 11-10-06 AWL 4.1-f067 [1419025] Change the form of temp file names so that Bridge does not try to
//				process them. Use a prefix of "._nn_" instead of "_nn" between the base and extension.
// 11-10-06 AWL 4.1-f066 [1417221] Tweak the "legacy digest missing" logic to import legacy values
//				that do not have corresponding XMP.
// 11-09-06 AWL 4.1-f065 [1409584] Add support for storing IPTC using UTF-8. To simplify this, the
//				only recognized DataSets are now 1:90, 2:0, and the "classic Protoshop" items that
//				are mapped to XMP.
// 11-09-06 AWL 4.1-f064 [1417587] Fix memory leaks associated with IPTC_Support's DisposeLooseValue.
//				It was being called for zero length values whose address was past the allocation.
//				Add hooks to find sources of leaks again in the future.
// 11-08-06 AWL 4.1-f063 Delete PSIR_Support.cpp, IFF_Handler.*, disable MP4 handler.
// 11-08-06 AWL 4.1-f062 Consolidate the endian conversion routines in EndianUtils.hpp.
// 11-07-06 AWL 4.1-f061 [1415778] Fix assert in creation of Exif legacy in JPEG. The Exif IFD "pointer"
//				(tag 34665) in the primary IFD isn't being created early enough.
// 11-01-06 AWL 4.1-f060 [1409613] Be more lenient about ill-formed IPTC dates.
// 11-01-06 AWL 4.1-f059 [1409577] Fix JPEG and PSD legacy import to better match Photoshop. There
//				are different JPEG/PSD/TIFF file policies for TIFF tags 270 (dc:description), 315
//				(dc:creator), and 33432 (dc:rights).
// 11-01-06 ALB 4.1-f058 [1405330] Fix: xmp:Date reconciliation / conversion of MS-flavour timestamps.
// 11-01-06 AWL 4.1-f057 Move include of XMPFiles_ChangeLog.hpp from XMPFiles_Impl.hpp to XMPFiles.cpp.
//				This stops the full rebuilds from each update to the change log.
// 10-31-06 AWL 4.1-f056 [1410712] Fix a bug in TIFF_FileWriter::UpdateMemoryStream - it was wrongly
//				trying to create an empty TIFF stream when one didn't exist before and still didn't.
//				Also fix JPEG_MetaHandler::WriteFile to not write empty TIFF or PSIR sections. And
//				fix IPTC_Writer::DeleteDataSet to not try to dispose of empty values.
// 10-31-06 ALB 4.1-f055 [1407442] ASF: URL conversion incorrect at end of string (don't treat zero as
//				non-display ASCII character.)
// 10-31-06 ALB 4.1-f054 ASF handler fixes:
//				[1406668] Don't create Content Encryption Objects in ASF files (WMA/WMV)
//				[1405330] xmp:Date reconciliation / conversion of MS-flavour timestamps
// 10-30-06 AWL 4.1-f053 [1409572] Be more lenient about recognizing the Exif section of JPEG.
// 10-30-06 AWL 4.1-f052 [1333769] Add DateTimeOriginal and DateTimeDigitized to the Exif writeback tags.
//				Also fix a bug where fractional Exif seconds were not output correctly. This affects the
//				TIFF DateTime/SubSecTime (tags 306/37520, xmp:ModifyDate) in addition to the new ones.
// 10-27-06 AWL 4.1-f051 [1408636] Fix the ID3 support to handle a UTF-16 'COMM' frame properly, it
//				has 2 substrings each needing a BOM. Also only use UTF-16 if really needed, use a
//				full Latin-1 check not just an ASCII check.
// 10-27-06 AWL 4.1-f050 [1408360] Have XMPFiles call the XMPCore init/term routines. The changes in
//				build 4.1-f041 added an init-time dependency. Not to mention the general dependency
//				that was always there and would break if the client didn't init both.
// 10-25-06 ALB 4.1-f049 ASF handler fixes (typo in object size, in-memory digest, handle
//				legacy<->dc:creator as array).
// 10-24-06 ALB 4.1-f048 Move the new ASF handler out of NewHandlers (including reconciliation).
// 10-20-06 AWL 4.1-f047 [1403500] Add support to call Mac QuickTime properly from background threads.
// 10-18-06 AWL 4.1-f046 [1395599] Add ISO Latin-1 conversions, change ID3 support to use it.
// 10-17-06 AWL 4.1-f045 [1380056] Fix output of Photoshop image resource names.
// 10-16-06 AWL 4.1-f044 [1388170] Fix a major regression in the previous f042 changes. That caused
//				TIFF and Exif imports to stop for plain camera files - ones with TIFF and Exif but
//				no PSIR or XMP.
// 10-13-06 AWL 4.1-f043 [1386343] Change the WAV handler to use dc:rights instead of xmpDM:copyright.
// 10-13-06 AWL 4.1-f042 [1388170] Behave the same as Photoshop when the legacy digest is missing.
//				In this case take the XMP as-is. This accomodates XMP-only editors.
// 10-12-06 AWL 4.1-f041 [1235816] Use the new/delete from XMPCore in DLL builds.
// 10-12-06 AWL 4.1-f040 [1394229] Make sure all handlers accept only "proper" files. Preferably
//				only by file content. Where we must, by "known" file extension.
// 10-10-06 AWL 4.1-f039 Adapt the MP3 handler to run on Mac.
// 10-08-06 AWL 4.1-f038 Adapt the RIFF-based handlers (AVI and WAV) to run on Mac.
// 09-21-06 AWL 4.1-f037 [1378220,1367149] Fix the legacy metadata output to allow proper detection
//				of XMP-only changes and thus take advantage of an in-place XMP update for unsafe
//				saves. Fix the PSIR 1034 copyright flag handling to match Photoshop. Add CR<->LF
//				normalization hackery to match Photoshop.
// 09-19-06 AWL 4.1-f036 [1379782] Fix the MPEG handler to truncate an existing sidecar before rewriting.
// 09-15-06 AWL 4.1-f035 Revamp the MPEG handler to be platform neutral.
// 09-14-06 AWL 4.1-f034 Finish the support for XMP-only in-place updates for JPEG/TIFF/PSD.
// 09-01-06 AWL 4.1-f033 Move the new JPEG/TIFF/PSD handlers out of NewHandlers.
// 08-31-06 AWL 4.1-f032 More work on in-place updates. Have ReconcileXMPtoJTP only update the IPTC
//				and PSIR blocks if they have changed. Move the XMP updating out to the file handler.
// 08-29-06 AWL 4.1-f031 Have the SetXyz functions for TIFF, PSIR, and IPTC simply return if the
//				existing value matches the new value. This will be needed to implement the minimal
//				in-place update logic in the handlers. Undo the similar checks made in ReconcileTIFF.
// 08-24-06 AWL 4.1-f030 Fix problems with the TIFF condensed rewrite code. This lets the JPEG save
//				logic work properly when the TIFF appends cause it to exceed 64K.
// 08-22-06 AWL 4.1-f029 Fix final length check in ID3_Support::GetFrameData.
// 08-21-06 AWL 4.1-f028 [0946377] Add more file extensions for MPEG files.
// 08-21-06 AWL 4.1-f027 [1346203] Fix encoding conversion problems in ID3_Support.cpp.
// 08-18-06 AWL 4.1-f026 [1352603] Add support for rewriting a full TIFF memory stream. This handles
//				edit-by-append overflow of the Exif in JPEG files. And as a side effect adds support
//				for wack-o JPEG files that have no initial Exif section. Fix related bugs in the
//				JPEG handler for writing files that have no Exif/PSIR/IPTC.
// 08-18-06 AWL 4.1-f025 Fix problems in ReconcileTIFF.cpp that caused the TIFF to always grow. Fix
//				bugs in TIFF_FileWriter.cpp with setting proper offsets for changed tags.
// 08-17-06 AWL 4.1-f024 [1352917] Fix a map iterator bug in JPEG_MetaHandler::CacheFileData when
//				processing an extended XMP block.
// 08-15-06 AWL 4.1-f023 Fix partial conversion bugs in the Mac code for ReconcileUtils::UTF8ToLocal
//				and ReconcileUtils::LocalToUTF8. It turns out that Apple's ConvertFromUnicodeToText
//				and ConvertFromTextToUnicode return kTECOutputBufferFullStatus instead of noErr.
// 08-02-06 AWL 4.1-f022 Fix uninitialized "ok" variable in TIFF_FileWriter::ProcessPShop6IFD for case
//				of flipping special tables.
// 08-01-06 AWL 4.1-f021 [1342734] Fix bug in locating XMP in MP3 files introduced when cleaning up
//				formatting as part of the 4.0-f013 fixes for the Acrobat security audit. The bug was
//				a change in the seek offset, going to the end of the XMP instead of the beginning.
// 08-01-06 AWL 4.1-f020 Bump API version to 4.1.
//
// 07-31-06 AWL 4.0-f019 Make sure the source file is closed before calling LFA_Rename. MoveFileW on
//				Windows complains if the source file is open.
// 07-31-06 AWL 4.0-f018 [1337834] Fix I/O regression in the PostScript/EPS handler. Need to make the
//				buffer look empty when doing LFA_Seek and RefillBuffer.
// 07-13-06 AWL 4.0-f017 Fix I/O regression in the InDesign handler. Need to make the buffer look
//				empty when doing LFA_Seek and RefillBuffer.
// 07-13-06 AWL 4.0-f016 Add a handler flag for file formats that need to have read-only packets.
// 07-13-06 AWL 4.0-f015 Fix CloseFile to properly decide if a safe update can be done. Fix
//				GetThumbnail to check the handler's kXMPFiles_ReturnsTNail flag.
// 07-10-06 AWL 4.0-f014 Initial version of new read-write JPEG handler and underpinnings. Reasonably
//				but not thoroughly tested, still within NewHandlers conditional.
// 06-27-06 AWL 4.0-f013 Changes for Acrobat security audit.
// 06-16-06 AWL 4.0-f012 Make XMP_ENTER_WRAPPER check sXMPFilesInitCount in debug builds.
// 06-09-06 AWL 4.0-f011 Add hackery to support TIFF files written by Photoshop 6.
// 06-07-06 AWL 4.0-f010 Improve the Unicode conversions. Implement the Unicode to/from local encoding conversions.
// 06-02-06 AWL 4.0-f009 [0949032] Fix Windows problems with calling QuickTime. The fsspec parameter
//				to OpenMovieFile wasn't robustly setup, and the resource ID (3rd) parameter to
//				NewMovieFromFile looked to be uninitialized on input.
// 06-01-06 AWL 4.0-f008 Activate new TIFF and Photoshop file handlers.
// 05-19-06 AWL 4.0-f007 Package I/O buffer info into a struct, change I/O macros to functions.
//				This will make it reasonable to pass the buffer state among layered functions.
// 05-19-06 AWL 4.0-f006 Change XMPFiles::OpenFile to return bool, don't throw if smart handler not
//				found when kXMPFiles_OpenUseSmartHandler is passed.
// 05-18-06 AWL 4.0-f005 Add support for JPEG/Exif thumbnails.
// 05-15-06 AWL 4.0-f004 Working, but minimally tested read-only smart JPEG handler under NewHandlers.
// 05-01-06 AWL 4.0-f003 Revamp the embedded, Mac PList, and Windows Property info.
// 04-07-06 AWL 4.0-f002 Add XMPFiles::GetThumbnail. Change XMPFiles::OpenFile to close the disk file
//				early for read-only access. Change XMPFileHandler::ExtractXMP to CacheFileData.
// 03-24-06 AWL 4.0-f001 Adapt for move to ham-perforce, integrate XMPFiles, bump version to 4.
//
// 01-25-06 AWL 1.3-014 Don't try to decrement iter.begin, even if result is unused, VC8 debug checks complain.
// 01-25-06 AWL 1.3-013 Replace null iterator notion, VC8 complains about compares to default iterator.
// 01-24-06 AWL 1.3-012 Adapt for VC8.
// 01-10-06 AWL 1.3-011 [1264593] Bump version to make use of latest MDKit more visible.
// 01-03-06 AWL 1.3-010 [1266255] Bump version to make use of latest MDKit more visible.
// 12-21-05 AWL 1.3-009 Bump version to make use of latest MDKit more visible.
// 11-07-05 AWL 1.3-008 Fix converting form of GetUns32LE and GetUns16LE.
// 10-27-05 JRS 1.3-007 Merging a few more bug fixes that were done in hazel-didv.
//				- JRS [1235472] Add .ms-pvr to list of MPEG file extensions.
//				- JRS Fix for MPEG handler grabbing .ffx files as its own.  Instead use packet scanner.
//				- JRS Fixed bug 1215542 Added .aet to files scanned for XMP with packet scanner.
//				- JRS Changed file extension for MPEG sidecar from <root>.<extension>.XMP to <root>.xmp.
//				- JRS Fixed bug 1209729.  Two problems with MPEG handler.  Registered in incorrect location,
//				with handlers that own file.  Also CheckFormat() had problems. Added additional file
//				extensions for MPEG files, and removed distinction for MPEG2.
// 09-28-05 ERJ 1.3-006 Merged all the bug fixes that were done in hazel-didv for: WAV, MP3 and AVI handlers.
//				- JRS Fixed bug 1209508 The reconciliation code wasn't updating the legacy log comment field on update.
//				- JRS Fixed bug 1209926 Tape and Alt tape names got truncated due to legacy reconciliation.
//				- ERJ [1219125] Fix MP3 handler to identify files it can't currently support.
//		        - ERJ Multiple Bugs: Fix MP3 handler and WAV handler to support <= 1000 char tags
//		 		(independently of language). Also really fixing 1219125 by supporting those special
//				mp3 files in the mp3 handler. Finally, getting the mp3 handler to not reconcile when
//				reconciliation is turned off (was done half-way, which was resulting in data loss).
// 09-02-05 AWL 1.3-005 Call BasicASInit and BasicASTerm for the MDKit handler.
// 09-02-05 AWL 1.3-004 Restore the MDKit handler, now using the notBIB XMP.
// 09-01-05 AWL 1.3-003 [1225858] Don't implicitly set the SafeUpdate flag for non-owning handlers.
// 08-22-05 AWL 1.3-002 Use XMP's mem procs.
// 07-21-05 AWL 1.3-001 Remove BIB, same as XMP. Create Xcode project. Bump version to 1.3.
//
// 06-03-05 AWL 1.2-060 [1157869] Make sure destructors deallocate everything and clear pointers.
// 05-26-05 AWL 1.2-059 Bump version to be ahead of Hazel-DIDV.
//
// 04-20-05 ERJ 1.1-058 Fixed the potential build break introduced in 1.1-057 and added the NewHandlers target.
// 04-20-05 ERJ 1.1-057 Merged the vc target to include the NewHandlers. Removed unwanted registrations.
// 04-15-05 Rys 1.1-056 Add MDKit handler writing ability (for JPEG, TIFF, PSD).
// 04-15-05 SDS 1.1-055 Added MPEG handler.
// 04-14-05 Rys 1.1-054 Add MDKit handler JPEG/TIFF/PSD reading of metadata. (may have first appeared in 52)
// 04-14-05 ERJ 1.1-053 Fix numerous reconciliation bugs on WAV and MP3 file handlers.
// 04-13-05 AWL 1.1-052 Add tweaks to the CheckProc needed by the MDKit-based JPEG/TIFF/PSD handler.
// 03-21-05 ERJ 1.1-051 Fix a tag truncation problem in the WAV Handler.
// 03-07-05 AWL 1.1-050 [1165582] Fix Basic_MetaHandler::WriteFile to follow its own protocol.
//				It was not dealing with the "trailing file content" and "back of the file" properly.
// 02-16-05 ERJ 1.1-049 Fix language encoding issues and now persisting unknown "INFO" tags in WAV.
//				Also making reconciliation optional for WAV and MP3 handlers.
// 02-16-05 AWL 1.1-048 [1153053] Fix inconsistent handling of PostScript that lacks a marker comment.
// 02-11-05 ERJ	1.1-047 Added reconciliation for WAV and MP3 files, also added some functionality in the AVI_Support.
// 02-07-05 AWL 1.1-046 [1137383] Add LFA_Create, LFA_Rename, and LFA_Delete. Don't use C stdio routines,
//				they don't handle UTF-8 properly on Windows!
// 01-30-05 JRS 1.1-045 Four bug fixes:  Always register name spaces; cut/paste error; legacy can
//				add metadata to packet, so set containsXMP flag correctly; update xmpPacket and length
//				after updating xmpObj with legacy data.
// 01-28-05 AWL 1.1-044 Roll main version to 1.1, DVA changes are no longer propagated to Hazel.
//
// 01-28-05 AWL 1.0-043 [1137383] Fix the Windows version of LFA_Open to properly handle UTF-8.
// 01-27-05 JRS 1.0-042 Added reconciliation for AVI files with Premiere's existing metadata format, and a bit of cleanup.
// 01-27-05 ERJ 1.0-041 Fixing a versioning problem in the mp3 handler (now using id3v2.3 as default).
// 01-26-05 AWL 1.0-040 [1142011] Bump copyright to 2005.
// 01-26-05 AWL 1.0-039 [1141684] Use XMPMeta::SendAssertNotify for XMPFiles asserts.
// 01-26-05 AWL 1.0-038 [1137684] Add .ffx to the list of file extensions for limited packet scanning.
// 01-21-05 AWL 1.0-037 Move the "restricted by extension" test for MP3 to the right place.
// 01-21-05 AWL 1.0-036 Remove WAV from the "restricted by extension" handlers.
// 01-21-05 JRS 1.0-035 Use this->xmpPacket in place of stack based string in ExtractXMP()
// 01-20-05 ERJ 1.0-034 Fixed string alloc (like 1.0-032) for MP3 handler.
// 01-20-05 AWL 1.0-033 Remove AVI from the "restricted by extension" handlers, go ahead and use
//				AVI CheckFormat function.
// 01-20-05 JRS 1.0-032 Fixed string allocation to use std::string for exception safety.
// 01-20-05 ERJ 1.0-031 Adding MP3 support.
// 01-18-05 AWL 1.0-030 [1136246] Add ai and ait to the set of file extensions used for limited packet scanning.
// 01-14-05 AWL 1.0-029 [1134197] Add gif, png, inx, inds, inpk, incd, inct, and incx to the set of
//				file extensions used for limited packet scanning.
// 01-07-05 AWL 1.0-028 [1014854] Set stdCharForm in handlers that require UTF-8.
// 01-06-05 AWL 1.0-027 [1014854] Cleanup handling for UTF-16 and UTF-32. This involves some general
//				changes to the separation of work between the common code and handlers, making the
//				storage of the SXMPMeta object and packet string more visible.
// 12-22-04 ERJ 1.0-026 Modify the WAV_Handler to be on par with the AVI_Handler (see changes 24 and 25)
// 12-22-04 JRS 1.0-025 [1108584] Modified the serializing to buffer of the XMP so small changes to the XMP don't
//				cause the packet to grow, and require a new addition to the movie, forever growing.  Also made
//				the change for AVI, in addition to the report for MOV.
// 12-21-04 JRS 1.0-024 Remove Debug_Assert() to verify AVI handler owns the file, as it no longer does.
// 12-21-04 JRS 1.0-023 Modified the AVI handler to no longer own the file handle.  Fixed a bug(AVI/MOV handlers)
//				that caused the XMP packet's to grow with padding when calling SerializeToBuffer().
// 12-21-04 AWL 1.0-022 [1067086] Add option to only packet scan files "known" to need scanning.
// 12-15-04 AWL 1.0-021 [1088863] Improve crash-safe update.
// 12-13-04 ERJ 1.0-020 Add WAV handler.
// 12-06-04 JRS 1.0-019 Fixed a precedence warning.
// 11-05-04 JRS 1.0-018 Fixed AVI handler, to round trip properly.  Updated xmp length on PutXMP
// 11-03-04 AWL 1.0-017 Add target for "new" handlers, make AVI handler be Windows-only.
// 11-01-04 AWL 1.0-016 [1100873] Fix CloseFile to close original before rename in safe save.
// 10-28-04 JRS 1.0-015 [1094297] Set the handler flags correctly to fix CanPutXMP().  Fix string handling in LocateXMP,
//				by using the string length, and some cleanup.
// 10-20-04 JRS 1.0-014 [1094476] Change 85079: Set the handlerFlags during construction for
//				AVI and MOV smart handlers.
// 10-20-04 AWL 1.0-013 [1084185] Bump version to mark rebuild with XMP_InternalRef changes.
// 10-19-04 AWL 1.0-012 [1094742] Fix CanPutXMP to call GetFileInfo properly, and to return the
//				result from the wrapper.
// 10-08-04 AWL 1.0-011 Check can-inject flag in PutXMP.
// 10-08-04 AWL 1.0-010 Add thread locking.
// 10-07-04 AWL 1.0-009 Add first cut of support for crash safe file update.
// 10-07-04 AWL 1.0-008 Add missing SetAbortProc in WXMPFiles.cpp and export lists.
// 09-23-04 AWL 1.0-007 Use the XMP toolkit's build number and timestamp. Add current position
//				result to LFA_Seek. Add SetAbortProc and abort checking.
// 09-22-04 AWL 1.0-006 Fix bugs in InDesign handler.
// 08-26-04 AWL 1.0-005 Fix OpenFile to use format hint or file extension.
// 08-25-04 AWL 1.0-004 Add MOV and AVI handlers.
// 08-25-04 AWL 1.0-003 Change InDesign handler to a Basic_Handler. Add tweaks for MOV and AVI.
// 08-24-04 AWL 1.0-002 Disable JPEG handler.
// 08-23-04 AWL 1.0-001 First created.
//
// =================================================================================================
#endif /* AdobePrivate */

#endif	/* __XMPFiles_ChangeLog_hpp__ */
