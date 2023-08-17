#ifndef __MOOV_Support_hpp__
#define __MOOV_Support_hpp__	1

// =================================================================================================
// Copyright Adobe
// Copyright 2009 Adobe
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
//  IJS Inder Jeet Singh
//  ADC Amandeep Chawla
//  HK  Honey Kansal
//	AB  Amit Bhatti
//
// mm-dd-yy who Description of changes, most recent on top
//
// 09-03-14 AB  5.6-f119 Adding support of UUID atom in MPEG4 Handler.
// 01-03-14 HK  5.6-f087 [3688857] Fixing data alignment issues on ARM processor.
// 05-09-13 ADC 5.6-f059 Fixing a potential problem due to not resetting pragma pack directive.
//
// 10-18-12 ADC 5.5-f052 Support for iTunes metadata.
// 09-21-12 IJS 5.5-f037 [3300999] Ignore timecode Sample if the Data references in timecode track of a QT file are external.
//
// 11-11-11 AWL 5.4-f035 [2945829] Include QuickTime starting offset in the timecode string.
//
// 08-17-10 AWL 5.2-f003 Integrate I/O revamp to main.
//
// 01-09-10 SAM 5.0-f126 [2527779] Disable QT and iTunes metadata mapping
// 09-24-09 AWL 5.0-f081 Revamp MPEG-4 handler to keep the entire 'moov' subtree in memory.
//
// =================================================================================================
#endif // AdobePrivate

#include "public/include/XMP_Environment.h"	// ! This must be the first include.
#include "public/include/XMP_Const.h"

#include "source/EndianUtils.hpp"

#include "XMPFiles/source/XMPFiles_Impl.hpp"
#include "XMPFiles/source/FormatSupport/ISOBaseMedia_Support.hpp"
#include <vector>

#if AdobePrivate
	#ifndef EnableQuickTimeMetadata
		#define EnableQuickTimeMetadata	0
	#endif
	#ifndef EnableITunesMetadata
		#define EnableITunesMetadata	1
	#endif
#endif

#if AdobePrivate
// =================================================================================================
// While the entire 'moov' subtree is cached in memory, it is not fully parsed. We only parse into
// the boxes that we care about. Boxes that we don't care about are in their parent's list of children,
// but they just have a blob of raw data for content. The boxes that we care about are:
//
// 'moov' (exactly 1, simple container)
//   'mvhd' (exactly 1, FullBox)
//   'udta' (0 or 1, simple container)
//     'XMP_' (0 or 1) - only in QuickTime files
//     'cprt' (0 or more, FullBox)
//     '©...' (0 or 1 of each) - QuickTime localized text boxes
//     'meta' (0 or 1, FullBox) - The only non-leaf FullBox, a PITA
//       'hdlr' (exactly 1, FullBox)
//       'ilst' (0 or 1, simple container) - Apple defined for iTunes metadata
//         '----' (0 or more, simple container) - The box type is 4 ASCII hyphens
//           'mean' (exactly 1, FullBox)
//           'name' (0 or 1, FullBox)
//           'data' (1 or more)
//         other (0 or more, simple container) - The box type is anything other than 4 ASCII hyphens
//           'data' (1 or more)
//   'trak' (1 or more, simple container)
//     'tkhd' (exactly 1, FullBox)
//     'edts' (at most 1, simple container)
//       'elst' (at most 1, FullBox)
//     'mdia' (exactly 1, simple container)
//       'hdlr' (exactly 1, FullBox)
//       'minf' (exactly 1, simple container)
//         'stbl' (exactly 1, simple container)
//           'stsd' (exactly 1, FullBox)
//           'stsc' (exactly 1, FullBox)
//           'stco' (0 or 1, FullBox) - Exactly one of 'stco' or 'co64' must be present
//           'co64' (0 or 1, FullBox)
//         'dinf' (0 or 1, simple container)
//           'dref' (0 or 1, FullBox)
//     'udta' (0 or 1, simple container)
//       'cprt' (0 or more, FullBox)
//       '©...' (0 or 1 of each) - QuickTime localized text boxes
//
// =================================================================================================
#endif

#define TopBoxSizeLimit 100*1024*1024

// =================================================================================================
// MOOV_Manager
// ============

class MOOV_Manager : public ISOBaseMedia_Manager {
public:

	// ---------------------------------------------------------------------------------------------
	// Types and constants

	enum {	// Values for fileMode.
		kFileIsNormalISO = 0,		// A "normal" MPEG-4 file, no 'qt  ' compatible brand.
		kFileIsModernQT  = 1,		// Has an 'ftyp' box and 'qt  ' compatible brand.
		kFileIsTraditionalQT = 2	// Old QuickTime, no 'ftyp' box.
	};

	

	// ---------------------------------------------------------------------------------------------
	// GetBox - Pick a box given a '/' separated list of box types. Picks the 1st of each type.

	BoxRef GetBox ( const char * boxPath, BoxInfo * info ) const;
	

	// ---------------------------------------------------------------------------------------------
	// NoteChange - Note overall change, value was directly replaced.
	// SetBox(path) - Like above, but creating path to the box if necessary.

	void SetBox ( const char * boxPath, const void* dataPtr, XMP_Uns32 size , const XMP_Uns8 * idUUID = 0 );


	// ---------------------------------------------------------------------------------------------


	void ParseMemoryTree ( XMP_Uns8 fileMode );
	void UpdateMemoryTree();

	// ---------------------------------------------------------------------------------------------

	#pragma pack (push, 1)	// ! These must match the file layout!

	struct Content_mvhd_0 {
		XMP_Uns32 vFlags;			//   0
		XMP_Uns32 creationTime;		//   4
		XMP_Uns32 modificationTime;	//   8
		XMP_Uns32 timescale;		//  12
		XMP_Uns32 duration;			//  16
		XMP_Int32 rate;				//  20
		XMP_Int16 volume;			//  24
		XMP_Uns16 pad_1;			//  26
		XMP_Uns32 pad_2, pad_3;		//  28
		XMP_Int32 matrix [9];		//  36
		XMP_Uns32 preDef [6];		//  72
		XMP_Uns32 nextTrackID;		//  96
	};								// 100

	struct Content_mvhd_1 {
		XMP_Uns32 vFlags;			//   0
		XMP_Uns64 creationTime;		//   4
		XMP_Uns64 modificationTime;	//  12
		XMP_Uns32 timescale;		//  20
		XMP_Uns64 duration;			//  24
		XMP_Int32 rate;				//  32
		XMP_Int16 volume;			//  36
		XMP_Uns16 pad_1;			//  38
		XMP_Uns32 pad_2, pad_3;		//  40
		XMP_Int32 matrix [9];		//  48
		XMP_Uns32 preDef [6];		//  84
		XMP_Uns32 nextTrackID;		// 108
	};								// 112


#if AdobePrivate
	struct Content_tkhd_0 {
		XMP_Uns32 vFlags;			//   0
		XMP_Uns32 creationTime;		//   4
		XMP_Uns32 modificationTime;	//   8
		XMP_Uns32 trackID	;		//  12
		XMP_Uns32 reserved_1;		//  16
		XMP_Uns32 duration;			//  20
		XMP_Uns32 reserved_2 [2];	//	24
		XMP_Int16 layer;			//  32
		XMP_Int16 altGroup;			//  34
		XMP_Int16 volume;			//  36
		XMP_Uns16 reserved_3;		//  38
		XMP_Int32 matrix[9];		//  40
		XMP_Uns32 width;			//  76
		XMP_Uns32 height;			//  80
	};								//  84

	struct Content_tkhd_1 {
		XMP_Uns32 vFlags;			//   0
		XMP_Uns64 creationTime;		//   4
		XMP_Uns64 modificationTime;	//  12
		XMP_Uns32 trackID;			//  20
		XMP_Uns32 reserved_1;		//  24
		XMP_Uns64 duration;			//  28
		XMP_Uns32 reserved_2[2];	//	36
		XMP_Int16 layer;			//  44
		XMP_Int16 altGroup;			//  46
		XMP_Int16 volume;			//  48
		XMP_Uns16 reserved_3;		//  50
		XMP_Int32 matrix[9];		//  52
		XMP_Uns32 width;			//  88
		XMP_Uns32 height;			//  92
	};								//  96
#endif

	struct Content_stsd_entry {
		XMP_Uns32 entrySize;		//  0
		XMP_Uns32 format;			//  4
		XMP_Uns8  reserved_1 [6];	//  8
		XMP_Uns16 dataRefIndex;		// 14
		XMP_Uns32 reserved_2;		// 16
		XMP_Uns32 flags;			// 20
		XMP_Uns32 timeScale;		// 24
		XMP_Uns32 frameDuration;	// 28
		XMP_Uns8  frameCount;		// 32
		XMP_Uns8  reserved_3;		// 33
		// Plus optional trailing ISO boxes.
	};								// 34

	struct Content_stsc_entry {
		XMP_Uns32 firstChunkNumber;	//  0
		XMP_Uns32 samplesPerChunk;	//  4
		XMP_Uns32 sampleDescrID;	//  8
	};								// 12

	#pragma pack( pop )

#if SUNOS_SPARC || XMP_IOS_ARM || XMP_ANDROID_ARM
	#pragma pack( )
#endif //#if SUNOS_SPARC || XMP_IOS_ARM || XMP_ANDROID_ARM

	// ---------------------------------------------------------------------------------------------

	MOOV_Manager() : fileMode(0)
	{
		XMP_Assert ( sizeof ( Content_mvhd_0 ) == 100 );	// Make sure the structs really are packed.
		XMP_Assert ( sizeof ( Content_mvhd_1 ) == 112 );
		XMP_Assert ( sizeof ( Content_hdlr ) == 24 );
		XMP_Assert ( sizeof ( Content_stsd_entry ) == 34 );
		XMP_Assert ( sizeof ( Content_stsc_entry ) == 12 );
		#if AdobePrivate
			XMP_Assert ( sizeof ( Content_tkhd_0 ) == 84 );
			XMP_Assert ( sizeof ( Content_tkhd_1 ) == 96 );
		#endif
	};

	virtual ~MOOV_Manager() {};

private:

	
	XMP_Uns8 fileMode;
	
	void ParseNestedBoxes ( BoxNode * parentNode, const std::string & parentPath, bool ignoreMetaBoxes );

	XMP_Uns32  NewSubtreeSize ( const BoxNode & node, const std::string & parentPath );
	XMP_Uns8 * AppendNewSubtree ( const BoxNode & node, const std::string & parentPath,
										 XMP_Uns8 * newPtr, XMP_Uns8 * newEnd );

};	// MOOV_Manager

#endif	// __MOOV_Support_hpp__
