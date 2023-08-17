#ifndef __P2_Handler_hpp__
#define __P2_Handler_hpp__	1

// =================================================================================================
// Copyright Adobe
// Copyright 2007 Adobe
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
//  SKP Sunil Kishor Pathak
//	HK  Honey Kansal
//  IJS Inder Jeet Singh
//	AB  Amit Bhatti
//
// mm-dd-yy who Description of changes, most recent on top
//
// 02-05-15 AB  5.6-f129 [3913583] For 50P and 59.94P, The Frame number for starttimecode should be written half to actual frame number in the XML file because NRT XML can store frame number value only between 00-29.
// 03-19-14 IJS 5.6-f098 [3709896] P2 Handler Modified to support spanned clip.
// 01-16-13 HK  5.6-f022 Implemented IsMetadataWritable API for XDCAM-EX, P2, CanonXF and AVCHD formats.
// 01-03-13 HK  5.6-f016 [3435383] Added GetAssociatedResources API for P2 format.
// 10-09-12 SKP 5.5-f044 Refactored LocateMetadataFiles API.
// 10-03-12 HK  5.5-f042 [3217688] A red X appears on the thumbnail in Panasonic P2 Viewer after updating the P2 media's metadata.
// 09-28-12 AWL 5.5-f039 Add LocateMetadataFiles, correct handler flags.
//
// 09-30-11 AWL 5.4-f016 Add P2 and SonyHDV support for GetFileModDate.
//
// 08-17-10 AWL 5.3-f001 Integrate I/O revamp to main.
//
// 08-11-09 AWL 5.0-f061 Integrate P2 and AVCHD changes from Peter Lee.
// 06-11-09 AWL 5.0-f043 Finish threading revamp, implement friendly reader/writer locking.
//
// 03-31-08 AWL 4.2.2-f103 [1741470] Integrate P2 bug fixes.
//
// 02-07-08 AWL 4.2-f070 Test, fix bugs, and move the advanced video handlers out of NewHandlers.
// 01-16-08 AWL 4.2-f057 Remove private parent pointer from MPEG-4, P2, SonyHDV, and XDCAM. Like all
//				handlers they must use the parent pointer in the XMPFileHandler base class.
// 12-31-07 AWL 4.2-f052 Integrate P2 and XDCAM handler changes from Peter Lee and Gerry Miller.
// 12-10-07 AWL 4.2-f044 Add P2 legacy digest and legacy .XML file update. Fix namespace handling.
// 11-30-07 AWL 4.2-f042 Add skeleton of legacy reconciliation with .XML file.
// 11-29-07 AWL 4.2-f041 First working version P2 handler in NewHandlers, XMP-only, no legacy.
// 11-28-07 AWL 4.2-f040 Introduce empty P2 handler in NewHandlers.
//
// =================================================================================================
#endif // AdobePrivate

#include "public/include/XMP_Environment.h"	// ! This must be the first include.

#include "XMPFiles/source/XMPFiles_Impl.hpp"

#include "source/ExpatAdapter.hpp"

#include "third-party/zuid/interfaces/MD5.h"
#include "XMPFiles/source/FormatSupport/P2_Support.hpp"

// =================================================================================================
/// \file P2_Handler.hpp
/// \brief Folder format handler for P2.
///
/// This header ...
///
// =================================================================================================

// *** Could derive from Basic_Handler - buffer file tail in a temp file.

extern XMPFileHandler * P2_MetaHandlerCTor ( XMPFiles * parent );

extern bool P2_CheckFormat ( XMP_FileFormat format,
							 const std::string & rootPath,
							 const std::string & gpName,
							 const std::string & parentName,
							 const std::string & leafName,
							 XMPFiles * parent );

static const XMP_OptionBits kP2_HandlerFlags = (kXMPFiles_CanInjectXMP |
												kXMPFiles_CanExpand |
												kXMPFiles_CanRewrite |
												kXMPFiles_PrefersInPlace |
												kXMPFiles_CanReconcile |
												kXMPFiles_AllowsOnlyXMP |
												kXMPFiles_ReturnsRawPacket |
												kXMPFiles_HandlerOwnsFile |
												kXMPFiles_AllowsSafeUpdate |
												kXMPFiles_FolderBasedFormat);

class P2_MetaHandler : public XMPFileHandler
{
public:

	void FillAssociatedResources ( std::vector<std::string> * resourceList );
	bool IsMetadataWritable ( );

	void CacheFileData();
	void ProcessXMP();

	void UpdateFile ( bool doSafeUpdate );
    void WriteTempFile ( XMP_IO* tempRef );

	XMP_OptionBits GetSerializeOptions()	// *** These should be standard for standalone XMP files.
		{ return (kXMP_UseCompactFormat | kXMP_OmitPacketWrapper); };

	P2_MetaHandler ( XMPFiles * _parent );
	virtual ~P2_MetaHandler();

private:

	P2_MetaHandler() {};	// Hidden on purpose.

	bool MakeClipFilePath ( std::string * path, XMP_StringPtr suffix, bool checkFile = false );
	void MakeLegacyDigest ( std::string * digestStr );

	void DigestLegacyItem ( MD5_CTX & md5Context, XML_NodePtr legacyContext, XMP_StringPtr legacyPropName );
	void DigestLegacyRelations ( MD5_CTX & md5Context );

	void SetXMPPropertyFromLegacyXML ( bool digestFound,
									   XML_NodePtr legacyContext,
									   XMP_StringPtr schemaNS,
									   XMP_StringPtr propName,
									   XMP_StringPtr legacyPropName,
									   bool isLocalized );
	void SetXMPPropertyFromLegacyXML ( bool digestFound,
										XMP_VarString* refContext,
										XMP_StringPtr schemaNS,
										XMP_StringPtr propName,
										bool isLocalized );

	void SetRelationsFromLegacyXML ( bool digestFound );
	void SetAudioInfoFromLegacyXML ( bool digestFound );
	void SetVideoInfoFromLegacyXML ( bool digestFound );
	void SetDurationFromLegacyXML  ( bool digestFound );

	void SetVideoFrameInfoFromLegacyXML ( XML_NodePtr legacyVideoContext, bool digestFound );
	void SetStartTimecodeFromLegacyXML  ( XML_NodePtr legacyVideoContext, bool digestFound );
	void SetGPSPropertyFromLegacyXML  ( XML_NodePtr legacyLocationContext, bool digestFound, XMP_StringPtr propName, XMP_StringPtr legacyPropName );
	void SetAltitudeFromLegacyXML  ( XML_NodePtr legacyLocationContext, bool digestFound );
	void AdjustTimeCode( std::string & p2Timecode, const XMP_Bool & isXMPtoXMLConversion );

	XML_Node * ForceChildElement ( XML_Node * parent, XMP_StringPtr localName, XMP_Int32 indent, XMP_Bool insertAtFront );

	std::string rootPath , clipName;

	P2_Manager p2ClipManager;

};	// P2_MetaHandler

// =================================================================================================

#endif /* __P2_Handler_hpp__ */
