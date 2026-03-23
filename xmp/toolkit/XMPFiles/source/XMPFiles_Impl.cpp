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
//  JEH Joerg Ehrlich
//  SKP Sunil Kishor Pathak
//  ADC Amandeep Chawla
//  IJS Inder Jeet Singh
//	AB  Amit Bhatti
//
// mm-dd-yy who Description of changes, most recent on top
//
// 03-31-15 AB  5.6-f147 Adding support for 3gp and 3g2 formats as well as adding more brands for mp4.
// 03-05-15 AB  5.6-f138 Removing SVG format from packet scanner as smart handler has been provided for handling SVG files.
// 03-04-15 AB  5.5-f136 First checkin of SVG Handler.
// 08-13-13 IJS 5.6-f076 [3206367,3206363] Enabling Error Notifications for Bad file format exceptions in MPEG4_Handler.
// 04-26-13 ADC 5.6-f054 [3526891] Read-only checks in IsMetadataWritable() API try creating temporary files if no metadata exists, instead of checking for file permissions.
//						 [3525961] IsMetaDataWritable checks for Plugins and embedded handlers are different.
// 01-11-13 IJS 5.6-f020 Reworked GetFileModDate, GetAssociatedResources and IsMetadataWritable APIs.
// 12-21-12 IJS 5.6-f008 Added GetAssociatedResources and IsWritable APIs 
//
// 11-01-12 IJS 5.5-f055 Changes done to support R3D Plugin Handler
// 10-10-12 ADC 5.5-f046 Fixing compile time issues which creeped into while doing merging of my changelist.
// 10-10-12 ADC 5.5-f045 Implement the internal infrastructure for XMPFiles error notifications.
// 10-09-12 SKP 5.5-f044 Refactored LocateMetadataFiles API.
//
// 09-23-11 AWL 5.4-f012 Add GetFileModeDate.
//
// 08-19-10 AWL 5.3-f004 Move the seek mode constants to XMP_Const.
// 08-19-10 AWL 5.3-f003 Remove all use of the LFA_* names.
// 08-17-10 AWL 5.3-f001 Integrate I/O revamp to main.
//
// 02-05-10 AWL 5.1-f002 Fix build warnings from Xcode 3.2.
//
// 08-11-09 AWL 5.0-f061 Add "r3d" (Red) to rejected file list.
// 06-12-09 FNO 5.0-f044 [2277195] Add XMP Support for UCF(based) .idap and .icap CS5 file formats.
// 06-11-09 AWL 5.0-f043 Finish threading revamp, implement friendly reader/writer locking.
// 05-07-09 FNO 5.0-f036 Start combined RIFF(AVI/WAV) Handler rewrite.
// 12-19-08 FNO 5.0-f011 [1913500] Add seven more known camera raw extensions to list.
// 11-13-08 AWL 5.0-f004 Add server mode support that ignores local text. Enable all handlers except
//				MOV for generic UNIX - that will be handled as part of the rewrite.
// 11-05-08 FNO 5.0-f003 [1913500] Add Lecia's .RWL extension to hard coded lists of RAW file extensions.
//
// 10-xx-08 AWL 4.4-f0xx MWG compliance changes, remove native thumbnail support.
// 07-17-08 FNO 4.4-f007 [1841019] better fix to also detect rsrcFork-free files under 10.4.
// 07-09-08 FNO 4.4-f005 [1841019] better fix. leniency on CatalogInfo for non-MacOS file systems.
// 07-04-08 FNO 4.4-f004 [1841019] further refinement of fix. affects mac only.
// 07-04-08 FNO 4.4-f003 [1841019] leniency on CatalogInfo for non-MacOS file systems. affects mac only.
// 07-03-08 FNO 4.4-f002 [1832465] enabling CS4 InCopy extensions (UCF+XML).
// 05-23-08 FNO 4.2.2-f118 [1799808] Added raw extension .3fr to kKnownRejectedFiles.
// 05-19-08 JEH 4.2.2-f114 [1786791] Activating packet scanner for AEP and AEPX files
// 04-11-08 FNO 4.2.2-f104 [1720135] Integrate large ucf (zip-64) support. LFA_Routines to source/LargeFileAccess.cpp|hpp.
// 03-05-08 FNO 4.2-f091 Fix temp file user rights on Linux.
// 02-22-08 FNO 4.2-f081 UCF-handler: Add accepted xfl, pdfxml extensions, rejected air.
// 02-19-08 AWL 4.2-f078 Don't try to muck with ownership when creating temp files.
// 02-11-08 AWL 4.2-f072 Add 3 more MPEG-4 standard file extensions.
// 02-05-08 AWL 4.2-f068 Use client memory routines for malloc/free also, so that leak checking works.
// 01-22-08 FNO 4.2-f061 [1580732] XMPFiles should not write XMP into Camera Raw files.
// 11-29-07 AWL 4.2-f041 First working version P2 handler in NewHandlers, XMP-only, no legacy.
// 11-28-07 AWL Fix Windows compile problems in f039.
// 11-28-07 AWL 4.2-f039 Add folder utils and implement TryFolderHandlers.
// 11-13-07 FNO 4.2-f034 Made XIO::Move() more flexible to support same and different file as target.
// 10-25-07 FNO 4.2-f031 Added XIO::Move() to move data within a file
// 10-16-07 FNO 4.2-f028 UCF read-only in NewHandlers(not final), added LFA_Read* convenience routines to XMPFiles
// 10-10-07 AWL 4.2-f027 Add handler method GetSerializeOptions. Introduce FillPacketInfo to replace
//				GetPacketCharForm, GetPacketRWMode, and GetPacketPadSize. And tolerate no wrapper.
// 10-09-07 FNO 4.2-f026 Added UCF draft to NewHandlers.
// 07-17-07 AWL 4.2-f025 Change Mac to use pthread synchronization, needed for 64-bit builds.
// 05-30-07 AWL 4.2-f024 Change operator new override to not throw for 0 length request, gcc on AIX
//				returns a null pointer for 0 length.
// 04-18-07 AWL 4.2-f022 Add dng to kFileExtMap, mapping to the TIFF handler.
// 12-20-06 AWL 4.2-f004 [1446159] Change the Mac version of LFA_Seek to support seeks past EOF with
//				implicit file growth. Fix an MP3 growth problem introduced with ID3_Support.cpp #5.
//				LFA_Write got changed to LFA_Seek in the loop of OffsetAudioData.
//
// 11-10-06 AWL 4.1-f067 [1419025] Change the form of temp file names so that Bridge does not try to
//				process them. Use a prefix of "._nn_" instead of "_nn" between the base and extension.
// 11-09-06 AWL 4.1-f064 [1417587] Fix memory leaks associated with IPTC_Support's DisposeLooseValue.
//				It was being called for zero length values whose address was past the allocation.
//				Add hooks to find sources of leaks again in the future.
// 09-15-06 AWL 4.1-f035 Revamp the MPEG handler to be platform neutral.
//
// 05-18-06 AWL 4.0-f005 Add support for JPEG/Exif thumbnails.
// 05-01-06 AWL 4.0-f003 Revamp the embedded, Mac PList, and Windows Property info.
// 03-24-06 AWL 4.0-f001 Adapt for move to ham-perforce, integrate XMPFiles, bump version to 4.
//
// 08-22-05 AWL 1.3-002 Use XMP's mem procs.
//
// 02-07-05 AWL 1.0-046 [1137383] Add LFA_Create, LFA_Rename, and LFA_Delete. Don't use C stdio routines,
//				they don't handle UTF-8 properly on Windows!
// 01-28-05 AWL 1.0-043 [1137383] Fix the Windows version of LFA_Open to properly handle UTF-8.
// 01-06-05 AWL 1.0-027 [1014854] Cleanup handling for UTF-16 and UTF-32. This involves some general
//				changes to the separation of work between the common code and handlers, making the
//				storage of the SXMPMeta object and packet string more visible.
// 12-15-04 AWL 1.0-021 [1088863] Improve crash-safe update.
// 10-08-04 AWL 1.0-010 Add thread locking.
// 10-07-04 AWL 1.0-009 Add first cut of support for crash safe file update.
// 09-23-04 AWL 1.0-007 Use the XMP toolkit's build number and timestamp. Add current position
//				result to LFA_Seek. Add SetAbortProc and abort checking.
// 08-13-04 AWL Checkpoint, almost full implementation, compiles.
// 07-09-04 AWL First draft.
//
// =================================================================================================
#endif // AdobePrivate

#include "public/include/XMP_Environment.h"	// ! XMP_Environment.h must be the first included header.

#include "public/include/XMP_Const.h"
#include "public/include/XMP_IO.hpp"

#include "XMPFiles/source/XMPFiles_Impl.hpp"
#include "source/XIO.hpp"

#include "source/UnicodeConversions.hpp"

using namespace std;

// Internal code should be using #if with XMP_MacBuild, XMP_WinBuild, XMP_AndroidBuild, or XMP_UNIXBuild.
// This is a sanity check in case of accidental use of *_ENV. Some clients use the poor
// practice of defining the *_ENV macro with an empty value.
#if defined ( MAC_ENV )
	#if ! MAC_ENV
		#error "MAC_ENV must be defined so that \"#if MAC_ENV\" is true"
	#endif
#elif defined ( WIN_ENV )
	#if ! WIN_ENV
		#error "WIN_ENV must be defined so that \"#if WIN_ENV\" is true"
	#endif
#elif defined ( UNIX_ENV )
	#if ! UNIX_ENV
		#error "UNIX_ENV must be defined so that \"#if UNIX_ENV\" is true"
	#endif
#elif defined ( ANDROID_ENV )
	#if ! ANDROID_ENV
		#error "ANDROID_ENV must be defined so that \"#if ANDROID_ENV\" is true"
	#endif
#elif defined ( WEB_ENV )
    #if ! WEB_ENV
        #error "WEB_ENV must be defined so that \"#if WEB_ENV\" is true"
    #endif
#endif

// =================================================================================================
/// \file XMPFiles_Impl.cpp
/// \brief ...
///
/// This file ...
///
// =================================================================================================
#include "public/include/XMP.incl_cpp"

#if XMP_WinBuild
	#pragma warning ( disable : 4290 )	// C++ exception specification ignored except to indicate a function is not __declspec(nothrow)
	#pragma warning ( disable : 4800 )	// forcing value to bool 'true' or 'false' (performance warning)
#endif

bool ignoreLocalText = false;

XMP_FileFormat voidFileFormat = 0;	// Used as sink for unwanted output parameters.

//***** see CTECHXMP-4169947 *****

//#if ! XMP_StaticBuild
//	XMP_PacketInfo voidPacketInfo;
//	void *         voidVoidPtr    = 0;
//	XMP_StringPtr  voidStringPtr  = 0;
//	XMP_StringLen  voidStringLen  = 0;
//	XMP_OptionBits voidOptionBits = 0;
//#endif

// =================================================================================================

// Add all known mappings, multiple mappings (tif, tiff) are OK.
const FileExtMapping kFileExtMap[] =
	{ { "pdf",  kXMP_PDFFile },
	  { "ps",   kXMP_PostScriptFile },
	  { "eps",  kXMP_EPSFile },

	  { "jpg",  kXMP_JPEGFile },
	  { "jpeg", kXMP_JPEGFile },
	  { "jpx",  kXMP_JPEG2KFile },
	  { "tif",  kXMP_TIFFFile },
	  { "tiff", kXMP_TIFFFile },
	  { "dng",  kXMP_TIFFFile },	// DNG files are well behaved TIFF.
	  { "gif",  kXMP_GIFFile },
	  { "giff", kXMP_GIFFile },
	  { "png",  kXMP_PNGFile },

	  { "swf",  kXMP_SWFFile },
	  { "flv",  kXMP_FLVFile },

	  { "aif",  kXMP_AIFFFile },

	  { "mov",  kXMP_MOVFile },
	  { "avi",  kXMP_AVIFile },
	  { "cin",  kXMP_CINFile },
	  { "wav",  kXMP_WAVFile },
	  { "mp3",  kXMP_MP3File },
	  { "mp4",  kXMP_MPEG4File },
	  { "m4v",  kXMP_MPEG4File },
	  { "m4a",  kXMP_MPEG4File },
	  { "f4v",  kXMP_MPEG4File },
	  { "3gp",  kXMP_MPEG4File },
	  { "3g2",  kXMP_MPEG4File },
	  { "crm",  kXMP_MPEG4File },
	  { "ses",  kXMP_SESFile },
	  { "cel",  kXMP_CELFile },
	  { "wma",  kXMP_WMAVFile },
	  { "wmv",  kXMP_WMAVFile },
	  { "mxf",  kXMP_MXFFile },
	  { "r3d",  kXMP_REDFile },

	  { "mpg",  kXMP_MPEGFile },
	  { "mpeg", kXMP_MPEGFile },
	  { "mp2",  kXMP_MPEGFile },
	  { "mod",  kXMP_MPEGFile },
	  { "m2v",  kXMP_MPEGFile },
	  { "mpa",  kXMP_MPEGFile },
	  { "mpv",  kXMP_MPEGFile },
	  { "m2p",  kXMP_MPEGFile },
	  { "m2a",  kXMP_MPEGFile },
	  { "m2t",  kXMP_MPEGFile },
	  { "mpe",  kXMP_MPEGFile },
	  { "vob",  kXMP_MPEGFile },
	  { "ms-pvr", kXMP_MPEGFile },
	  { "dvr-ms", kXMP_MPEGFile },
	  { "heic", kXMP_HEIFFile }, 
	  { "heif", kXMP_HEIFFile },
	  { "avc1", kXMP_HEIFFile },

	  { "html", kXMP_HTMLFile },
	  { "xml",  kXMP_XMLFile },
	  { "txt",  kXMP_TextFile },
	  { "text", kXMP_TextFile },
	  { "svg",  kXMP_SVGFile },

	  { "psd",  kXMP_PhotoshopFile },
	  { "ai",   kXMP_IllustratorFile },
	  { "indd", kXMP_InDesignFile },
	  { "indt", kXMP_InDesignFile },
	  { "aep",  kXMP_AEProjectFile },
	  { "aepx", kXMP_AEProjectFile },
	  { "aet",  kXMP_AEProjTemplateFile },
	  { "ffx",  kXMP_AEFilterPresetFile },
	  { "ncor", kXMP_EncoreProjectFile },
	  { "prproj", kXMP_PremiereProjectFile },
	  { "prtl", kXMP_PremiereTitleFile },
	  { "ucf", kXMP_UCFFile },
	  { "xfl", kXMP_UCFFile },
	  { "pdfxml", kXMP_UCFFile },
	  { "mars", kXMP_UCFFile },
	  { "idml", kXMP_UCFFile },
	  { "idap", kXMP_UCFFile },
	  { "icap", kXMP_UCFFile },
	  { "webp", kXMP_WebPFile },
	  { "", 0 } };	// ! Must be last as a sentinel.

// Files known to contain XMP but have no smart handling, here or elsewhere.
const char * kKnownScannedFiles[] =
	{ "ai",		// Illustrator, actually a PDF file.
	  "ait",	// Illustrator template, actually a PDF file.
	  "aet",	// After Effects template project file.
	  "ffx",	// After Effects filter preset file.
	  "aep",	// After Effects project file in proprietary format
	  "aepx",	// After Effects project file in XML format
	  "inx",	// InDesign interchange, an XML file.
	  "inds",	// InDesign snippet, an XML file.
	  "inpk",	// InDesign package for GoLive, a text file (not XML).
	  "incd",	// InCopy story, an XML file.
	  "inct",	// InCopy template, an XML file.
	  "incx",	// InCopy interchange, an XML file.
	  "fm",		// FrameMaker file, proprietary format.
	  "book",	// FrameMaker book, proprietary format.
	  "icml",	// an inCopy (inDesign) format
	  "icmt",	// an inCopy (inDesign) format
	  "idms",	// an inCopy (inDesign) format
	  0 };		// ! Keep a 0 sentinel at the end.


// Extensions that XMPFiles never handles.
const char * kKnownRejectedFiles[] =
	{
	#if AdobePrivate
	// See http://sol.corp.adobe.com/twiki/bin/view/Photoshop/CRExtensions for reference.
	#endif
		// RAW files
		"cr2", "erf", "fff", "dcr", "kdc", "mos", "mfw", "mef",
		"raw", "nef", "orf", "pef", "arw", "sr2", "srf", "sti",
		"3fr", "rwl", "crw", "sraw", "mos", "mrw", "nrw", "rw2",
		"c3f",
		// UCF subformats
		"air",
		// Others
	  0 };	// ! Keep a 0 sentinel at the end.

// =================================================================================================

#if AdobePrivate
	#if ! XMP_StaticBuild

		#undef malloc
		#undef free

		XMP_AllocateProc sXMP_MemAlloc = malloc;
		XMP_DeleteProc   sXMP_MemFree  = free;

//******* SEE : CTECHXMP-4169971 *************//
#if 0
		#define malloc(size) (*sXMP_MemAlloc) ( size )
		#define free(addr)   (*sXMP_MemFree) ( addr )

		void * operator new ( size_t len ) throw ( std::bad_alloc )
		{
			void * mem = (*sXMP_MemAlloc) ( len );
			if ( (mem == 0) && (len != 0) ) throw std::bad_alloc();
			return mem;
		}

		void * operator new[] ( size_t len ) throw ( std::bad_alloc )
		{
			void * mem = (*sXMP_MemAlloc) ( len );
			if ( (mem == 0) && (len != 0) ) throw std::bad_alloc();
			return mem;
		}

		void operator delete ( void * ptr ) throw()
		{
			if ( ptr != 0 ) (*sXMP_MemFree) ( ptr );
		}

		void operator delete[] ( void * ptr ) throw()
		{
			if ( ptr != 0 ) (*sXMP_MemFree) ( ptr );
		}
#endif

	#endif
#endif

// =================================================================================================

// =================================================================================================
// GetPacketCharForm
// =================
//
// The first character must be U+FEFF or ASCII, typically '<' for an outermost element, initial
// processing instruction, or XML declaration. The second character can't be U+0000.
// The possible input sequences are:
//   Cases with U+FEFF
//      EF BB BF -- - UTF-8
//      FE FF -- -- - Big endian UTF-16
//      00 00 FE FF - Big endian UTF 32
//      FF FE 00 00 - Little endian UTF-32
//      FF FE -- -- - Little endian UTF-16
//   Cases with ASCII
//      nn mm -- -- - UTF-8 -
//      00 00 00 nn - Big endian UTF-32
//      00 nn -- -- - Big endian UTF-16
//      nn 00 00 00 - Little endian UTF-32
//      nn 00 -- -- - Little endian UTF-16

static XMP_Uns8 GetPacketCharForm ( XMP_StringPtr packetStr, XMP_StringLen packetLen )
{
	XMP_Uns8   charForm = kXMP_CharUnknown;
	XMP_Uns8 * unsBytes = (XMP_Uns8*)packetStr;	// ! Make sure comparisons are unsigned.

	if ( packetLen < 2 ) return kXMP_Char8Bit;

	if ( packetLen < 4 ) {

		// These cases are based on the first 2 bytes:
		//   00 nn Big endian UTF-16
		//   nn 00 Little endian UTF-16
		//   FE FF Big endian UTF-16
		//   FF FE Little endian UTF-16
		//   Otherwise UTF-8

		if ( packetStr[0] == 0 ) return kXMP_Char16BitBig;
		if ( packetStr[1] == 0 ) return kXMP_Char16BitLittle;
		if ( CheckBytes ( packetStr, "\xFE\xFF", 2 ) ) return kXMP_Char16BitBig;
		if ( CheckBytes ( packetStr, "\xFF\xFE", 2 ) ) return kXMP_Char16BitLittle;
		return kXMP_Char8Bit;

	}

	// If we get here the packet is at least 4 bytes, could be any form.

	if ( unsBytes[0] == 0 ) {

		// These cases are:
		//   00 nn -- -- - Big endian UTF-16
		//   00 00 00 nn - Big endian UTF-32
		//   00 00 FE FF - Big endian UTF 32

		if ( unsBytes[1] != 0 ) {
			charForm = kXMP_Char16BitBig;			// 00 nn
		} else {
			if ( (unsBytes[2] == 0) && (unsBytes[3] != 0) ) {
				charForm = kXMP_Char32BitBig;		// 00 00 00 nn
			} else if ( (unsBytes[2] == 0xFE) && (unsBytes[3] == 0xFF) ) {
				charForm = kXMP_Char32BitBig;		// 00 00 FE FF
			}
		}

	} else {

		// These cases are:
		//   FE FF -- -- - Big endian UTF-16, FE isn't valid UTF-8
		//   FF FE 00 00 - Little endian UTF-32, FF isn't valid UTF-8
		//   FF FE -- -- - Little endian UTF-16
		//   nn mm -- -- - UTF-8, includes EF BB BF case
		//   nn 00 00 00 - Little endian UTF-32
		//   nn 00 -- -- - Little endian UTF-16

		if ( unsBytes[0] == 0xFE ) {
			if ( unsBytes[1] == 0xFF ) charForm = kXMP_Char16BitBig;	// FE FF
		} else if ( unsBytes[0] == 0xFF ) {
			if ( unsBytes[1] == 0xFE ) {
				if ( (unsBytes[2] == 0) && (unsBytes[3] == 0) ) {
					charForm = kXMP_Char32BitLittle;	// FF FE 00 00
				} else {
					charForm = kXMP_Char16BitLittle;	// FF FE
				}
			}
		} else if ( unsBytes[1] != 0 ) {
			charForm = kXMP_Char8Bit;					// nn mm
		} else {
			if ( (unsBytes[2] == 0) && (unsBytes[3] == 0) ) {
				charForm = kXMP_Char32BitLittle;		// nn 00 00 00
			} else {
				charForm = kXMP_Char16BitLittle;		// nn 00
			}
		}

	}

	//	XMP_Assert ( charForm != kXMP_CharUnknown );
	return charForm;

}	// GetPacketCharForm

// =================================================================================================
// FillPacketInfo
// ==============
//
// If a packet wrapper is present, the the packet string is roughly:
//   <?xpacket begin= ...?>
//   <outer-XML-element>
//     ... more XML ...
//   </outer-XML-element>
//   ... whitespace padding ...
//   <?xpacket end='.'?>

// The 8-bit form is 14 bytes, the 16-bit form is 28 bytes, the 32-bit form is 56 bytes.
#define k8BitTrailer  "<?xpacket end="
#define k16BitTrailer "<\0?\0x\0p\0a\0c\0k\0e\0t\0 \0e\0n\0d\0=\0"
#define k32BitTrailer "<\0\0\0?\0\0\0x\0\0\0p\0\0\0a\0\0\0c\0\0\0k\0\0\0e\0\0\0t\0\0\0 \0\0\0e\0\0\0n\0\0\0d\0\0\0=\0\0\0"
static XMP_StringPtr kPacketTrailiers[3] = { k8BitTrailer, k16BitTrailer, k32BitTrailer };

void FillPacketInfo ( const std::string & packet, XMP_PacketInfo * info )
{
	XMP_StringPtr packetStr = packet.c_str();
	XMP_StringLen packetLen = (XMP_StringLen) packet.size();
	if ( packetLen == 0 ) return;

	info->charForm = GetPacketCharForm ( packetStr, packetLen );
	XMP_StringLen charSize = XMP_GetCharSize ( info->charForm );

	// Look for a packet wrapper. For our purposes, we can be lazy and just look for the trailer PI.
	// If that is present we'll assume that a recognizable header is present. First do a bytewise
	// search for '<', then a char sized comparison for the start of the trailer. We don't really
	// care about big or little endian here. We're looking for ASCII bytes with zeroes between.
	// Shorten the range comparisons (n*charSize) by 1 to easily tolerate both big and little endian.

	XMP_StringLen padStart, padEnd;
	XMP_StringPtr packetTrailer = kPacketTrailiers [ charSize>>1 ];

	padEnd = packetLen - 1;
	for ( ; padEnd > 0; --padEnd ) if ( packetStr[padEnd] == '<' ) break;
	if ( (packetStr[padEnd] != '<') || ((packetLen - padEnd) < (18*charSize)) ) return;
	if ( ! CheckBytes ( &packetStr[padEnd], packetTrailer, (13*charSize) ) ) return;

	info->hasWrapper = true;

	char rwFlag = packetStr [padEnd + 15*charSize];
	if ( rwFlag == 'w' ) info->writeable = true;

	// Look for the start of the padding, right after the last XML end tag.

	padStart = padEnd;	// Don't do the -charSize here, might wrap below zero.
	for ( ; padStart >= charSize; padStart -= charSize ) if ( packetStr[padStart] == '>' ) break;
	if ( padStart < charSize ) return;
	padStart += charSize;	// The padding starts after the '>'.

	info->padSize = padEnd - padStart;	// We want bytes of padding, not character units.

}	// FillPacketInfo

// =================================================================================================
// ReadXMPPacket
// =============

void ReadXMPPacket ( XMPFileHandler * handler )
{
	XMP_IO* fileRef   = handler->parent->ioRef;
	std::string & xmpPacket = handler->xmpPacket;
	XMP_StringLen packetLen = handler->packetInfo.length;

	if ( packetLen == 0 ) XMP_Throw ( "ReadXMPPacket - No XMP packet", kXMPErr_BadXMP );

	xmpPacket.erase();
	xmpPacket.reserve ( packetLen );
	xmpPacket.append ( packetLen, ' ' );

	XMP_StringPtr packetStr = XMP_StringPtr ( xmpPacket.c_str() );	// Don't set until after reserving the space!

	fileRef->Seek ( handler->packetInfo.offset, kXMP_SeekFromStart );
	fileRef->ReadAll ( (char*)packetStr, packetLen );

}	// ReadXMPPacket

// =================================================================================================
// XMPFileHandler::GetFileModDate
// ==============================
//
// The base implementation is only for typical embedding handlers. It returns the modification date
// of the named file.

bool XMPFileHandler::GetFileModDate ( XMP_DateTime * modDate )
{

	XMP_OptionBits flags = this->handlerFlags;
	if ( (flags & kXMPFiles_HandlerOwnsFile) ||
		 (flags & kXMPFiles_UsesSidecarXMP) ||
		 (flags & kXMPFiles_FolderBasedFormat) ) {
		XMP_Throw ( "Base implementation of GetFileModDate only for typical embedding handlers", kXMPErr_InternalFailure );
	}
	
	if ( this->parent->GetFilePath().empty() ) {
		XMP_Throw ( "GetFileModDate cannot be used with client-provided I/O", kXMPErr_InternalFailure );
	}
	
	return Host_IO::GetModifyDate ( this->parent->GetFilePath().c_str(), modDate );

}	// XMPFileHandler::GetFileModDate

// =================================================================================================
// XMPFileHandler::FillMetadataFiles
// ==============================
//
// The base implementation is only for files having embedded metadata for which the same file will
// be returned.

void XMPFileHandler::FillMetadataFiles ( std::vector<std::string> * metadataFiles )
{
	XMP_OptionBits flags = this->handlerFlags;
	if ( (flags & kXMPFiles_HandlerOwnsFile) ||
		 (flags & kXMPFiles_UsesSidecarXMP) ||
		 (flags & kXMPFiles_FolderBasedFormat) ) {
		XMP_Throw ( "Base implementation of FillMetadataFiles only for typical embedding handlers", kXMPErr_InternalFailure );
	}
	
	if ( this->parent->GetFilePath().empty() ) {
		XMP_Throw ( "FillMetadataFiles cannot be used with client-provided I/O", kXMPErr_InternalFailure );
	}
	
	metadataFiles->push_back ( std::string ( this->parent->GetFilePath().c_str() ) );
}	// XMPFileHandler::FillMetadataFiles

// =================================================================================================
// XMPFileHandler::FillAssociatedResources
// =======================================
//
// The base implementation is only for files having embedded metadata for which the same file will
// be returned as the associated resource.
// 

void XMPFileHandler::FillAssociatedResources ( std::vector<std::string> * metadataFiles )
{
	XMP_OptionBits flags = this->handlerFlags;
	if ( (flags & kXMPFiles_HandlerOwnsFile) ||
		 (flags & kXMPFiles_UsesSidecarXMP) ||
		 (flags & kXMPFiles_FolderBasedFormat) ) {
		XMP_Throw ( "GetAssociatedResources is not implemented for this file format", kXMPErr_InternalFailure );
	}
	
	if ( this->parent->GetFilePath().empty() ) {
		XMP_Throw ( "GetAssociatedResources cannot be used with client-provided I/O", kXMPErr_InternalFailure );
	}
	
	metadataFiles->push_back ( std::string ( this->parent->GetFilePath().c_str() ) );
}	// XMPFileHandler::FillAssociatedResources

// =================================================================================================
// XMPFileHandler::IsMetadataWritable
// =======================================
//
// The base implementation is only for files having embedded metadata and it checks whether that 
// file is writable or no.Returns true if the file is writable. 
// 
bool XMPFileHandler::IsMetadataWritable ( )
{
	XMP_OptionBits flags = this->handlerFlags;
	if ( (flags & kXMPFiles_HandlerOwnsFile) ||
		 (flags & kXMPFiles_UsesSidecarXMP)  ||
		 (flags & kXMPFiles_FolderBasedFormat) ) {
		XMP_Throw ( "IsMetadataWritable is not implemented for this file format", kXMPErr_InternalFailure );
	}

	if ( this->parent->GetFilePath().empty() ) {
		XMP_Throw ( "IsMetadataWritable cannot be used with client-provided I/O", kXMPErr_InternalFailure );
	}

	try {
		return Host_IO::Writable( this->parent->GetFilePath().c_str() );
	} catch ( ... ) {

	}
	return false;
}

#if AdobePrivate
// =================================================================================================
// XMPFileHandler::GetAlbumArtworks
// ==========================
//
// This base implementation will just return false. If the derived handler does support the album art then it
// must have its own implementation of GetAlbumArtworks.

bool XMPFileHandler::GetAlbumArtworks()
{
	return false;
}
#endif

// =================================================================================================
// XMPFileHandler::ProcessXMP
// ==========================
//
// This base implementation just parses the XMP. If the derived handler does reconciliation then it
// must have its own implementation of ProcessXMP.

void XMPFileHandler::ProcessXMP()
{

	if ( (!this->containsXMP) || this->processedXMP ) return;

	if ( this->handlerFlags & kXMPFiles_CanReconcile ) {
		XMP_Throw ( "Reconciling file handlers must implement ProcessXMP", kXMPErr_InternalFailure );
	}

	SXMPUtils::RemoveProperties ( &this->xmpObj, 0, 0, kXMPUtil_DoAllProperties );
	this->xmpObj.ParseFromBuffer ( this->xmpPacket.c_str(), (XMP_StringLen)this->xmpPacket.size() );
	this->processedXMP = true;

}	// XMPFileHandler::ProcessXMP

// =================================================================================================
// XMPFileHandler::GetSerializeOptions
// ===================================
//
// This base implementation just selects compact serialization. The character form and padding/in-place
// settings are added in the common code before calling SerializeToBuffer.

XMP_OptionBits XMPFileHandler::GetSerializeOptions()
{

	return kXMP_UseCompactFormat;

}	// XMPFileHandler::GetSerializeOptions

// =================================================================================================
// XMPFileHandler::NotifyClient
// ===================================
//
// Generic function for all the file handlers to replace existing exception throws
//
void XMPFileHandler::NotifyClient(GenericErrorCallback * errCBptr, XMP_ErrorSeverity severity, XMP_Error & error)
{	
	if (errCBptr)
		errCBptr->NotifyClient( severity, error );
	else {
		if ( severity != kXMPErrSev_Recoverable )
			throw error;
	}
}
// =================================================================================================
