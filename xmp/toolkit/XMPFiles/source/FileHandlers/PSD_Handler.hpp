#ifndef __PSD_Handler_hpp__
#define __PSD_Handler_hpp__	1

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
//	IJS Inder Jeet Singh 
//
// mm-dd-yy who Description of changes, most recent on top
//
// 09-20-12 IJS 5.5-f035 Add Progress Notification flag to MPEG-4,PSD,AIFF,WAVE,ASF,TIFF,FLV and PS Handlers.
// 08-19-10 AWL 5.3-f003 Remove all use of the LFA_* names.
// 08-17-10 AWL 5.3-f001 Integrate I/O revamp to main.
//
// 10-13-08 AWL 4.4-f013 Remove internals of GetThumbnail.
//
// 09-14-06 AWL 4.1-f034 Finish the support for XMP-only in-place updates for JPEG/TIFF/PSD.
//
// 05-26-06 AWL Initial creation.
//
// =================================================================================================
#endif // AdobePrivate

#include "XMPFiles/source/FormatSupport/TIFF_Support.hpp"
#include "XMPFiles/source/FormatSupport/PSIR_Support.hpp"
#include "XMPFiles/source/FormatSupport/IPTC_Support.hpp"

// =================================================================================================
/// \file PSD_Handler.hpp
/// \brief File format handler for PSD (Photoshop).
///
/// This header ...
///
// =================================================================================================

// *** Could derive from Basic_Handler - buffer file tail in a temp file.

extern XMPFileHandler * PSD_MetaHandlerCTor ( XMPFiles * parent );

extern bool PSD_CheckFormat ( XMP_FileFormat format,
							  XMP_StringPtr  filePath,
                              XMP_IO*    fileRef,
                              XMPFiles *     parent );

static const XMP_OptionBits kPSD_HandlerFlags = (kXMPFiles_CanInjectXMP |
                                                 kXMPFiles_CanExpand |
                                                 kXMPFiles_CanRewrite |
                                                 kXMPFiles_PrefersInPlace |
                                                 kXMPFiles_CanReconcile |
                                                 kXMPFiles_AllowsOnlyXMP |
                                                 kXMPFiles_ReturnsRawPacket |
                                                 kXMPFiles_AllowsSafeUpdate |
												 kXMPFiles_CanNotifyProgress);

class PSD_MetaHandler : public XMPFileHandler
{
public:

	void CacheFileData();
	void ProcessXMP();

	void UpdateFile    ( bool doSafeUpdate );
    void WriteTempFile ( XMP_IO* tempRef );

	bool skipReconcile;	// ! Used between UpdateFile and WriteFile.

	PSD_MetaHandler ( XMPFiles * parent );
	virtual ~PSD_MetaHandler();

private:

	PSD_MetaHandler() : iptcMgr(0), exifMgr(0), skipReconcile(false) {};	// Hidden on purpose.

	PSIR_FileWriter psirMgr;	// Don't need a pointer, the PSIR part is always file-based.
	IPTC_Manager *  iptcMgr;	// Need to use pointers so we can properly select between read-only
	TIFF_Manager *  exifMgr;	//	and read-write modes of usage.

	XMP_Uns32 imageWidth, imageHeight;	// Pixel dimensions, used with thumbnail info.

};	// PSD_MetaHandler

// =================================================================================================

#endif /* __PSD_Handler_hpp__ */
