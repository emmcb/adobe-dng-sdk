#ifndef __XDCAM_Handler_hpp__
#define __XDCAM_Handler_hpp__	1

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
//  IJS Inder Jeet Singh
//	HK  Honey Kansal
//	AB  Amit Bhatti
//
// mm-dd-yy who Description of changes, most recent on top
//
// 02-06-15 AB  5.6-f130 Redesign XDCAM Handler by dividing it into 2 child handlers XDCAMFAM_Handler and XDCAMSAM_Handler and making it more consistent as per latest spec.
// 02-26-13 HK  5.6-f042 Support old sidecar naming convention for XMPilot and SxS formats to maintain backward compatibility.
// 02-13-13 HK  5.6-f034 [3498318] In XDCAM_FAM handler, if an old side car of XMPilot can't be renamed, XMPFiles should at least allow to read it.
// 01-30-13 IJS 5.6-f028 [3477008] Modified XDCAM FAM(XMPilot) so as to preserve metadata from an old mxf Sidecar.
// 01-18-13 IJS 5.6-f024 Implemented GetAssociatedResources and IsMetadataWritable APIs for XDCAM Family.
//
// 10-09-12 SKP 5.5-f044 Refactored LocateMetadataFiles API.
// 09-30-11 AWL 5.4-f017 Add XDCAM (EX/FAM/SAM) support for GetFileModDate.
//
// 11-24-10	AWL	5.3-f020 [2755623,2755632] Add XDCAM fixes for non-XMP metadata problems.
// 08-17-10 AWL 5.3-f001 Integrate I/O revamp to main.
//
// 06-11-09 AWL 5.0-f043 Finish threading revamp, implement friendly reader/writer locking.
//
// 02-22-08 AWL 4.2-f082 Fix XDCAM_CheckFormat to be case insensitive for PROAV. Remove the CUEUP.XML
//				check from XDCAMEX_CheckFormat. Fix handerTemp leaks for all folder-oriented handlers.
// 02-07-08 AWL 4.2-f070 Test, fix bugs, and move the advanced video handlers out of NewHandlers.
// 01-16-08 AWL 4.2-f057 Remove private parent pointer from MPEG-4, P2, SonyHDV, and XDCAM. Like all
//				handlers they must use the parent pointer in the XMPFileHandler base class.
// 12-31-07 AWL 4.2-f052 Integrate P2 and XDCAM handler changes from Peter Lee and Gerry Miller.
// 12-06-07 AWL 4.2-f043 Add basic XDCAM handler.
//
// =================================================================================================
#endif // AdobePrivate

#include "public/include/XMP_Environment.h"	// ! This must be the first include.

#include "XMPFiles/source/XMPFiles_Impl.hpp"

#include "source/ExpatAdapter.hpp"

// =================================================================================================
/// \file XDCAM_Handler.hpp
/// \brief Folder format handler for XDCAM.
///
/// This header ...
///
// =================================================================================================

inline bool IsDigit( char c )
{
	return c >= '0' && c <= '9';
}

class XDCAM_MetaHandler : public XMPFileHandler
{
public:

	bool GetFileModDate ( XMP_DateTime * modDate );

	virtual void FillAssociatedResources ( std::vector<std::string> * resourceList ) {};
	bool IsMetadataWritable ( ) ;

	void CacheFileData();
	void ProcessXMP();

	void UpdateFile ( bool doSafeUpdate );
    void WriteTempFile ( XMP_IO* tempRef );

	XMP_OptionBits GetSerializeOptions()	// *** These should be standard for standalone XMP files.
		{ return (kXMP_UseCompactFormat | kXMP_OmitPacketWrapper); };

	XDCAM_MetaHandler ( XMPFiles * _parent );
	virtual ~XDCAM_MetaHandler();

protected:

	XDCAM_MetaHandler() : expat(0), clipMetadata(0) {};	// Hidden on purpose.

	virtual bool MakeClipFilePath ( std::string * path, XMP_StringPtr suffix, bool checkFile = false ) { return false; }
	virtual void SetPathVariables ( const std::string & clientPath )  { }
	virtual bool GetMediaProMetadata ( SXMPMeta * xmpObjPtr, const std::string& clipUMID, bool digestFound ) {
		return false;
	}
	bool MakeMediaproPath ( std::string * path, bool checkFile = false );
	virtual bool GetClipUmid ( std::string &clipUmid ) { return false; }
	void readXMLFile( XMP_StringPtr filePath,ExpatAdapter* &expat );
	bool RefersClipUmid ( std::string clipUmid , XMP_StringPtr editInfoPath )  ;
	std::string rootPath, clipName, sidecarPath;
	
	std::string mNRTFilePath;
	std::string oldSidecarPath;

private:

	void FillMetadataFiles ( std::vector<std::string> * metadataFiles );
	void MakeLegacyDigest ( std::string * digestStr );
	void CleanupLegacyXML();

	std::string xdcNS, legacyNS;

	ExpatAdapter * expat;
	XML_Node * clipMetadata;	// ! Don't delete, points into the Expat tree.

};	// XDCAM_MetaHandler

// =================================================================================================

#endif /* __XDCAM_Handler_hpp__ */
