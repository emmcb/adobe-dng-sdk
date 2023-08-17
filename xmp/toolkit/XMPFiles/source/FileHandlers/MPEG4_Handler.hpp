#ifndef __MPEG4_Handler_hpp__
#define __MPEG4_Handler_hpp__	1

// =================================================================================================
// Copyright Adobe
// Copyright 2006 Adobe
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
//  IJS Inder Jeet Singh
//	AB  Amit Bhatti
//
// mm-dd-yy who Description of changes, most recent on top
//
// 10-21-14 AB  5.6.f120 [3836549] Making kXMPFiles_OptimizeFileLayout public for XMP SDK.
// 09-20-12 IJS 5.5-f035 Add Progress Notification flag to MPEG-4,PSD,AIFF,WAVE,ASF,TIFF,FLV and PS Handlers.
// 08-19-10 AWL 5.3-f003 Remove all use of the LFA_* names.
// 08-17-10 AWL 5.3-f001 Integrate I/O revamp to main.
//
// 10-06-09 AWL 5.0-f083 Add MPEG-4 support for kXMPFiles_OptimizeFileLayout.
// 09-24-09 AWL 5.0-f081 Revamp MPEG-4 handler to keep the entire 'moov' subtree in memory.
// 08-24-09 AWL 5.0-f069 Add support for QuickTime timecode information.
// 03-10-09 AWL 5.0-f027 Initial integration of QuickTime support into MPEG-4 handler.
//
// 01-21-08 AWL 4.2-f058 Global tweak to have the handler's packetInfo always refer to the file.
// 01-16-08 AWL 4.2-f057 Remove private parent pointer from MPEG-4, P2, SonyHDV, and XDCAM. Like all
//				handlers they must use the parent pointer in the XMPFileHandler base class.
// 01-15-08 AWL 4.2-f056 [1665011] Fix assertions failures in MPEG-4.
// 12-21-07 AWL 4.2-f049 First working version of MPEG-4 handler, still under NewHandlers.
// 11-13-07 AWL 4.2-f035 Add MPEG-4 skeleton handler. Rename MPEG handler to MPEG-2.
// 09-15-06 ALB Initial creation.
//
// =================================================================================================
#endif // AdobePrivate

#include "XMPFiles/source/XMPFiles_Impl.hpp"

#include "XMPFiles/source/FormatSupport/MOOV_Support.hpp"
#include "XMPFiles/source/FormatSupport/QuickTime_Support.hpp"
#include "XMPFiles/source/FormatSupport/META_Support.hpp"


//  ================================================================================================
/// \file MPEG4_Handler.hpp
/// \brief File format handler for MPEG-4.
///
/// This header ...
///
//  ================================================================================================

extern XMPFileHandler * MPEG4_MetaHandlerCTor ( XMPFiles * parent );

extern bool MPEG4_CheckFormat ( XMP_FileFormat format,
								XMP_StringPtr  filePath,
								XMP_IO*    fileRef,
								XMPFiles *     parent );

static const XMP_OptionBits kMPEG4_HandlerFlags = ( kXMPFiles_CanInjectXMP |
													kXMPFiles_CanExpand |
													kXMPFiles_CanRewrite |
													kXMPFiles_PrefersInPlace |
													kXMPFiles_CanReconcile |
													kXMPFiles_AllowsOnlyXMP |
													kXMPFiles_ReturnsRawPacket |
													kXMPFiles_AllowsSafeUpdate |
													kXMPFiles_CanNotifyProgress
												  );

class MPEG4_MetaHandler : public XMPFileHandler
{
public:

	void CacheFileData();
	void ProcessXMP();

	void UpdateFile ( bool doSafeUpdate );
    void WriteTempFile ( XMP_IO* tempRef );


	MPEG4_MetaHandler ( XMPFiles * _parent );
	virtual ~MPEG4_MetaHandler();

	struct TimecodeTrackInfo {	// Info about a QuickTime timecode track.
		bool stsdBoxFound, isDropFrame;
		XMP_Uns32 timeScale;
		XMP_Uns32 frameDuration;
		XMP_Uns32 timecodeSample;
		XMP_Uns64 sampleOffset;	// Absolute file offset of the timecode sample, 0 if none.
		XMP_Uns32 nameOffset;	// The offset of the 'name' box relative to the 'stsd' box content.
		XMP_Uns16   macLang;	// The Mac language code of the trailing 'name' box.
		std::string macName;	// The text part of the trailing 'name' box, in macLang encoding.
		TimecodeTrackInfo()
			: stsdBoxFound(false), isDropFrame(false), timeScale(0), frameDuration(0),
			  timecodeSample(0), sampleOffset(0), nameOffset(0), macLang(0) {};
	};

private:

	MPEG4_MetaHandler() : fileMode(0), havePreferredXMP(false),
						  xmpBoxPos(0), moovBoxPos(0), xmpBoxSize(0), moovBoxSize(0)
#if AdobePrivate
	, metaBoxSize(0)
#endif
	{};	// Hidden on purpose.

#if AdobePrivate
	bool ParseAndValidateSemiProXML();
#endif

	bool ParseTimecodeTrack();
#if AdobePrivate
	bool ParseVideoTrack ( XMP_Int32 * matrix  );
#endif
	void UpdateTopLevelBox ( XMP_Uns64 oldOffset, XMP_Uns32 oldSize, const XMP_Uns8 * newBox, XMP_Uns32 newSize );

	void OptimizeFileLayout();

	XMP_Uns8 fileMode;
	bool havePreferredXMP;
	XMP_Uns64 xmpBoxPos;	// The file offset of the XMP box (the size field, not the content).
	XMP_Uns64 moovBoxPos;	// The file offset of the 'moov' box (the size field, not the content).
	XMP_Uns32 xmpBoxSize, moovBoxSize;	// The full size of the boxes, not just the content.
	
#if AdobePrivate
	XMP_Uns32  metaBoxSize;
#endif

	MOOV_Manager moovMgr;

#if AdobePrivate
	Meta_Manager metaMgr;

#if EnableITunesMetadata
	iTunes_Manager iTunesMgr;
#endif
#endif
	TradQT_Manager tradQTMgr;

	TimecodeTrackInfo tmcdInfo;

#if AdobePrivate
	ISOMedia::SemiProXMLMeta	semiProXml;
#endif
};	// MPEG4_MetaHandler

// =================================================================================================

#endif // __MPEG4_Handler_hpp__
