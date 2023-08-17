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
//  AWL Alan Lillich
//  SKP Sunil Kishor Pathak
//  ADC Amandeep Chawla
//  IJS Inder Jeet Singh
//	HK	Honey Kansal
//	AB  Amit Bhatti
//
// mm-dd-yy who Description of changes, most recent on top
//
// 02-06-15 AB  5.6-f130 Redesign XDCAM Handler by dividing it into 2 child handlers XDCAMFAM_Handler and XDCAMSAM_Handler and making it more consistent as per latest spec.
// 03-13-14 IJS 5.6-f097 Fix the break after 5.6-f096.
// 03-12-14 IJS 5.6-f096 [3714412] Proxy and High Res Clip in XDCAM FAM should open the same XMP.
// 04-26-13 ADC 5.6-f054 [3526891] Read-only checks in IsMetadataWritable() API try creating temporary files if no metadata exists, instead of checking for file permissions.
//						 [3525961] IsMetaDataWritable checks for Plugins and embedded handlers are different.
// 04-04-13 IJS 5.6-f051 Move common I/O methods to IOUtils.
//						 [3534631] Optimize GetAssociated Resources for RED_Handler
// 02-26-13 HK  5.6-f042 Support old sidecar naming convention for XMPilot and SxS formats to maintain backward compatibility.
// 02-13-13 ADC 5.6-f035 [3498686, 3497304] XMP GetFileModDate() is extremely slow.
// 02-13-13 HK  5.6-f034 [3498318] In XDCAM_FAM handler, if an old side car of XMPilot can't be renamed, XMPFiles should at least allow to read it.
// 02-05-13 HK  5.6-f030 Fixed bugs#3477099, #3495118, #3490341, #3492358.
// 01-30-13 IJS 5.6-f028 [3477008] Modified XDCAM FAM(XMPilot) so as to preserve metadata from an old mxf Sidecar.
// 01-18-13 IJS 5.6-f024 Implemented GetAssociatedResources and IsMetadataWritable APIs for XDCAM Family.
// 12-13-12 IJS 5.6-f002 [3143977] Tweaks to identify the variant XDCAM Memory SxS of XDCAM FAM format.
//
// 10-10-12 ADC 5.5-f045 Implement the internal infrastructure for XMPFiles error notifications.
// 10-09-12 SKP 5.5-f044 Refactored LocateMetadataFiles API.
// 09-19-12 AWL 5.5-f033 [3211021] Fix the folder handlers complain about read-only XMP files when opening for update.
// 09-17-12	AWL	5.5-f032 Integrate tweaks to support XMPilot variant of XDCAM FAM.
// 01-16-12	AWL	5.5-f006 [2980767] Improve sidecar file update for disk full situations.
//
// 10-27-11 AWL 5.4-f030 [3007461] Improve performance of GetFileModDate.
// 09-30-11 AWL 5.4-f017 Add XDCAM (EX/FAM/SAM) support for GetFileModDate.
//
// 11-24-10	AWL	5.3-f020 [2755623,2755632] Add XDCAM fixes for non-XMP metadata problems.
// 08-27-10 AWL 5.3-f007 Add comments to Host_IO.hpp, fix semantic irregularities in the code.
// 08-26-10 AWL 5.3-f006 Move the folder info functions into the Host_IO namespace.
// 08-19-10 AWL 5.3-f004 Move the seek mode constants to XMP_Const.
// 08-17-10 AWL 5.3-f001 Integrate I/O revamp to main.
//
// 06-11-09 AWL 5.0-f043 Finish threading revamp, implement friendly reader/writer locking.
// 01-29-09 AWL 5.0-f022 [1699977] Fixes for UNIX file case sensitivity.
// 01-15-09 AWL 5.0-f020 Split handlerTemp into tempPtr and tempUI32.
//
// 10-13-08 AWL 4.4-f013 Remove internals of GetThumbnail.
//
// 03-06-08 AWL 4.2-f093 Change the XDCAM file type constants to be explicit about all 3: FAM/SAM/EX.
// 03-03-08 AWL 4.2-f088 [1704032] Fix references to legacy .XML file in the XDCAM handlers.
// 02-22-08 AWL 4.2-f082 Fix XDCAM_CheckFormat to be case insensitive for PROAV. Remove the CUEUP.XML
//				check from XDCAMEX_CheckFormat. Fix handerTemp leaks for all folder-oriented handlers.
// 02-15-08 AWL 4.2-f075 Integrate more folder-oriented handler updates. Initial changes to create
//				generic UNIX builds for XMPFiles.
// 02-07-08 AWL 4.2-f070 Test, fix bugs, and move the advanced video handlers out of NewHandlers.
// 02-05-08 AWL 4.2-f069 Integrate latest advanced video handlers.
// 01-16-08 AWL 4.2-f057 Remove private parent pointer from MPEG-4, P2, SonyHDV, and XDCAM. Like all
//				handlers they must use the parent pointer in the XMPFileHandler base class.
// 01-07-08 AWL 4.2-f054 Fix some of the easy 64-bit compile warnings.
// 01-01-08 AWL 4.2-f053 Move the legacy digests for P2 and XDCAM to the xmp:NativeDigests struct.
// 12-31-07 AWL 4.2-f052 Integrate P2 and XDCAM handler changes from Peter Lee and Gerry Miller.
// 12-06-07 AWL 4.2-f043 Add basic XDCAM handler.
//
// =================================================================================================
#endif // AdobePrivate

#include "public/include/XMP_Environment.h"	// ! XMP_Environment.h must be the first included header.

#include "public/include/XMP_Const.h"
#include "public/include/XMP_IO.hpp"

#include "XMPFiles/source/XMPFiles_Impl.hpp"
#include "source/XMPFiles_IO.hpp"
#include "source/XIO.hpp"
#include "source/IOUtils.hpp"

#include "XMPFiles/source/FileHandlers/XDCAM_Handler.hpp"
#include "XMPFiles/source/FormatSupport/XDCAM_Support.hpp"
#include "XMPFiles/source/FormatSupport/PackageFormat_Support.hpp"
#include "third-party/zuid/interfaces/MD5.h"

using namespace std;

// =================================================================================================
/// \file XDCAM_Handler.cpp
/// \brief Folder format handler for XDCAM.
///
/// This handler is for the XDCAM video format. This is a pseudo-package, visible files but with a very
/// well-defined layout and naming rules. There are 2 different layouts for XDCAM, called FAM and SAM.
/// The FAM layout is used by "normal" XDCAM devices. The SAM layout is used by XDCAM-EX devices.
///
/// A typical FAM layout looks like (note mixed case for the nested folders):
///
/// .../MyMovie/
/// 	INDEX.XML
/// 	DISCMETA.XML
/// 	MEDIAPRO.XML
/// 	General/
/// 		unknown files
/// 	Clip/
/// 		C0001.MXF
/// 		C0001M01.XML
/// 		C0001M01.XMP
/// 		C0002.MXF
/// 		C0002M01.XML
/// 		C0002M01.XMP
/// 	Sub/
/// 		C0001S01.MXF
/// 		C0002S01.MXF
/// 	Edit/
/// 		E0001E01.SMI
/// 		E0001M01.XML
/// 		E0002E01.SMI
/// 		E0002M01.XML
///
/// A typical FAM XMPilot layout looks like (note mixed case for the nested folders):
///
/// .../MyMovie/
/// 	DISCMETA.XML
/// 	MEDIAPRO.XML
/// 	General/
/// 	Clip/
/// 		Office_0001.MXF
/// 		Office_0001M01.XML
/// 		Office_0001M01.XMP
/// 		Office_0002.MXF
/// 		Office_0002M01.XML
/// 		Office_0002M01.XMP
/// 	Sub/
/// 		Office_0001S01.MXF
/// 		Office_0002S01.MXF
/// 	Edit/
///     UserData/
///         unknown files
///
/// A typical FAM XDCAM Memory SxS layout looks like (note mixed case for the nested folders):
///
/// .../MyMovie/
/// 	DISCMETA.XML
/// 	MEDIAPRO.XML
///     CUEUP.XML
/// 	General/
/// 	Clip/
/// 		C0001.MXF
/// 		C0001M01.XML
/// 		C0001M01.XMP
/// 		C0001R01.BIM
/// 		C0002.MXF
/// 		C0002M01.XML
/// 		C0002M01.XMP
/// 		C0001R01.BIM
/// 	Sub/
/// 		C0001S01.MXF
/// 		C0002S01.MXF
/// 	Edit/
///		Take/
/// 		T0001.SMI
/// 		T0001M01.XML
///     UserData/
///
/// A typical SAM layout looks like:
///
/// .../MyMovie/
/// 	GENERAL/
/// 		unknown files
/// 	PROAV/
/// 		INDEX.XML
/// 		INDEX.BUP
/// 		DISCMETA.XML
/// 		DISCINFO.XML
/// 		DISCINFO.BUP
/// 		CLPR/
/// 			C0001/
/// 				C0001C01.SMI
/// 				C0001V01.MXF
/// 				C0001A01.MXF
/// 				C0001A02.MXF
/// 				C0001R01.BIM
/// 				C0001I01.PPN
/// 				C0001M01.XML
/// 				C0001M01.XMP
/// 				C0001S01.MXF
/// 			C0002/
/// 				...
/// 		EDTR/
/// 			E0001/
/// 				E0001E01.SMI
/// 				E0001M01.XML
/// 			E0002/
/// 				...
///
/// Note that the Sony documentation uses the folder names "General", "Clip", "Sub", and "Edit". We
/// use all caps here. Common code has already shifted the names, we want to be case insensitive.
///
/// From the user's point of view, .../MyMovie contains XDCAM stuff, in this case 2 clips whose raw
/// names are C0001 and C0002. There may be mapping information for nicer clip names to the raw
/// names, but that can be ignored for now. Each clip is stored as a collection of files, each file
/// holding some specific aspect of the clip's data.
///
/// The XDCAM handler operates on clips. The path from the client of XMPFiles can be either a logical
/// clip path, like ".../MyMovie/C0001", or a full path to one of the files. In the latter case the
/// handler must figure out the intended clip, it must not blindly use the named file.
///
/// Once the XDCAM structure and intended clip are identified, the handler only deals with the .XMP
/// and .XML files in the CLIP or CLPR/<clip> folders. The .XMP file, if present, contains the XMP
/// for the clip. The .XML file must be present to define the existance of the clip. It contains a
/// variety of information about the clip, including some legacy metadata.
///
// =================================================================================================

// =================================================================================================
// XDCAM_CheckFormat
// =================
//
// This version does fairly simple checks. The top level folder (.../MyMovie) must have exactly 1
// child, a folder called CONTENTS. This must have a subfolder called CLIP. It may also have
// subfolders called VIDEO, AUDIO, ICON, VOICE, and PROXY. Any mixture of these additional folders
// is allowed, but no other children are allowed in CONTENTS. The CLIP folder must contain a .XML
// file for the desired clip. The name checks are case insensitive.
//
// The state of the string parameters depends on the form of the path passed by the client. If the
// client passed a logical clip path, like ".../MyMovie/C0001", the parameters are:
//   rootPath   - ".../MyMovie"
//   gpName     - empty
//   parentName - empty
//   leafName   - "C0001"
//
// If the client passed a FAM file path, like ".../MyMovie/Edit/E0001E01.SMI", they are:
//   rootPath   - "..."
//   gpName     - "MyMovie"
//   parentName - "EDIT"	(common code has shifted the case)
//   leafName   - "E0001E01"
//
// If the client passed a SAM file path, like ".../MyMovie/PROAV/CLPR/C0001/C0001A02.MXF", they are:
//   rootPath   - ".../MyMovie/PROAV"
//   gpName     - "CLPR"
//   parentName - "C0001"
//   leafName   - "C0001A02"
//
// For both FAM and SAM the leading character of the leafName for an existing file might be coerced
// to 'C' to form the logical clip name. And suffix such as "M01" must be removed for FAM. We don't
// need to worry about that for SAM, that uses the <clip> folder name.

// ! The FAM format supports general clip file names through an ALIAS.XML mapping file. The simple
// ! existence check has an edge case bug, left to be fixed later. If the ALIAS.XML file exists, but
// ! some of the clips still have "raw" names, and we're passed an existing file path in the EDIT
// ! folder, we will fail to do the leading 'E' to 'C' coercion. We might also erroneously remove a
// ! suffix from a mapped essence file with a name like ClipX01.MXF.

// ! The common code has shifted the gpName, parentName, and leafName strings to uppercase. It has
// ! also made sure that for a logical clip path the rootPath is an existing folder, and that the
// ! file exists for a full file path.

// =================================================================================================
// XDCAM_MetaHandler::XDCAM_MetaHandler
// ====================================
XDCAM_MetaHandler::XDCAM_MetaHandler ( XMPFiles * _parent ) : expat(0), clipMetadata(NULL)
{

	this->parent = _parent;	// Inherited, can't set in the prefix.
	this->stdCharForm  = kXMP_Char8Bit;

}	// XDCAM_MetaHandler::XDCAM_MetaHandler

// =================================================================================================
// XDCAM_MetaHandler::~XDCAM_MetaHandler
// =====================================

XDCAM_MetaHandler::~XDCAM_MetaHandler()
{

	this->CleanupLegacyXML();
	if ( this->parent->tempPtr != 0 ) {
		free ( this->parent->tempPtr );
		this->parent->tempPtr = 0;
	}

}	// XDCAM_MetaHandler::~XDCAM_MetaHandler

// =================================================================================================
// XDCAM_MetaHandler::MakeMediaproPath
// ===================================

bool XDCAM_MetaHandler::MakeMediaproPath ( std::string * path, bool checkFile /* = false */ )
{

	*path = this->rootPath;
	*path += kDirChar;
	*path += "MEDIAPRO.XML";
	
	if ( ! checkFile ) return true;
	return Host_IO::Exists ( path->c_str() );

}	// XDCAM_MetaHandler::MakeMediaproPath

// =================================================================================================
// XDCAM_MetaHandler::MakeLegacyDigest
// ===================================

// *** Early hack version.

#define kHexDigits "0123456789ABCDEF"

// =================================================================================================
// XDCAM_MetaHandler::MakeLegacyDigest
// ================================

void XDCAM_MetaHandler::MakeLegacyDigest ( std::string * digestStr )
{
	digestStr->erase();
	if ( this->clipMetadata == 0 ) return;	// Bail if we don't have any legacy XML.
	XMP_Assert ( this->expat != 0 );

	XMP_StringPtr xdcNSptr = this->xdcNS.c_str();
	XML_NodePtr legacyContext, legacyProp;

	legacyContext = this->clipMetadata->GetNamedElement ( xdcNSptr, "Access" );
	if ( legacyContext == 0 ) return;

	MD5_CTX    context;
	unsigned char digestBin [16];
	MD5Init ( &context );

	legacyProp = legacyContext->GetNamedElement ( xdcNSptr, "Creator" );
	if ( (legacyProp != 0) && legacyProp->IsLeafContentNode() && (! legacyProp->content.empty()) ) {
		const XML_Node * xmlValue = legacyProp->content[0];
		MD5Update ( &context, (XMP_Uns8*)xmlValue->value.c_str(), (unsigned int)xmlValue->value.size() );
	}

	legacyProp = legacyContext->GetNamedElement ( xdcNSptr, "CreationDate" );
	if ( (legacyProp != 0) && legacyProp->IsLeafContentNode() && (! legacyProp->content.empty()) ) {
		const XML_Node * xmlValue = legacyProp->content[0];
		MD5Update ( &context, (XMP_Uns8*)xmlValue->value.c_str(), (unsigned int)xmlValue->value.size() );
	}

	legacyProp = legacyContext->GetNamedElement ( xdcNSptr, "LastUpdateDate" );
	if ( (legacyProp != 0) && legacyProp->IsLeafContentNode() && (! legacyProp->content.empty()) ) {
		const XML_Node * xmlValue = legacyProp->content[0];
		MD5Update ( &context, (XMP_Uns8*)xmlValue->value.c_str(), (unsigned int)xmlValue->value.size() );
	}

	MD5Final ( digestBin, &context );

	char buffer [40];
	for ( int in = 0, out = 0; in < 16; in += 1, out += 2 ) {
		XMP_Uns8 byte = digestBin[in];
		buffer[out]   = kHexDigits [ byte >> 4 ];
		buffer[out+1] = kHexDigits [ byte & 0xF ];
	}
	buffer[32] = 0;
	digestStr->append ( buffer );

}	// XDCAM_MetaHandler::MakeLegacyDigest

// =================================================================================================
// XDCAM_MetaHandler::CleanupLegacyXML
// ================================

void XDCAM_MetaHandler::CleanupLegacyXML()
{

	if ( this->expat != 0 ) { delete ( this->expat ); this->expat = 0; }

	clipMetadata = 0;	// ! Was a pointer into the expat tree.

}	// XDCAM_MetaHandler::CleanupLegacyXML

// =================================================================================================
// XDCAM_MetaHandler::readXMLFile
// ================================

void  XDCAM_MetaHandler::readXMLFile( XMP_StringPtr filePath, ExpatAdapter* &_expat )
{
	Host_IO::FileRef hostRef = Host_IO::Open ( filePath, Host_IO::openReadOnly );
	if ( hostRef == Host_IO::noFileRef ) return;	// The open failed.
	XMPFiles_IO xmlFile ( hostRef, filePath, Host_IO::openReadOnly );

	_expat = XMP_NewExpatAdapter ( ExpatAdapter::kUseLocalNamespaces );
	if ( _expat == 0 ) XMP_Throw ( "XDCAM_MetaHandler: Can't create Expat adapter", kXMPErr_NoMemory );

	XMP_Uns8 buffer [64*1024];
	while ( true ) {
		XMP_Int32 ioCount = xmlFile.Read ( buffer, sizeof(buffer) );
		if ( ioCount == 0 ) break;
		_expat->ParseBuffer ( buffer, ioCount, false /* not the end */ );
	}
	_expat->ParseBuffer ( 0, 0, true );	// End the parse.

	xmlFile.Close();
}

// =================================================================================================
// XDCAM_MetaHandler::GetFileModDate
// =================================

static inline bool operator< ( const XMP_DateTime & left, const XMP_DateTime & right ) {
	int compare = SXMPUtils::CompareDateTime ( left, right );
	return (compare < 0);
}

// =================================================================================================
// XDCAM_MetaHandler::GetFileModDate
// ================================

bool XDCAM_MetaHandler::GetFileModDate ( XMP_DateTime * modDate )
{

	// Modify date is found in the increasing priority order
	//
	// MEDIAPRO.XML
	// Non-Real time metadata file
	// XMP file

	bool ok, haveDate = false;
	std::string fullPath;
	XMP_DateTime oneDate, junkDate;
	if ( modDate == 0 ) modDate = &junkDate;

	// MEDIAPRO.XML	
	std::string mediaproPath;
	ok = MakeMediaproPath ( &mediaproPath, true /* checkFile */ );
	if ( ok ) ok = Host_IO::GetModifyDate ( mediaproPath.c_str(), &oneDate );
	if ( ok ) {
		if ( (! haveDate) ) *modDate = oneDate;
		haveDate = true;
	}

	// Non-Real time metadata file
	ok = Host_IO::Exists( this->mNRTFilePath.c_str() );
	//ok = this->MakeClipFilePath ( &fullPath, "M01.XML", true /* checkFile */ );
	if ( ok ) ok = Host_IO::GetModifyDate ( this->mNRTFilePath.c_str(), &oneDate );
	if ( ok ) {
		if ( (! haveDate) || (*modDate < oneDate) ) *modDate = oneDate;
		haveDate = true;
	}

	// XMP file
	ok = Host_IO::Exists( this->sidecarPath.c_str() );
	//ok = this->MakeClipFilePath ( &fullPath, "M01.XMP", true /* checkFile */ );
	if ( ok ) ok = Host_IO::GetModifyDate ( this->sidecarPath.c_str(), &oneDate );
	if ( ok ) {
		if ( (! haveDate) || (*modDate < oneDate) ) *modDate = oneDate;
		haveDate = true;
	}

	return haveDate;

}	// XDCAM_MetaHandler::GetFileModDate

// =================================================================================================
// XDCAM_MetaHandler::RefersClipUmid
// ==================================
bool XDCAM_MetaHandler::RefersClipUmid ( std::string clipUmid , XMP_StringPtr editInfoPath ) 
{
	ExpatAdapter* editInfoExpat = 0 ;
	XMP_StringPtr nameSpace = 0 ;
	try {
		readXMLFile( editInfoPath, editInfoExpat );
		if ( editInfoExpat != 0 )
		{	
			XML_Node & xmlTree = editInfoExpat->tree;
			XML_NodePtr rootElem = 0;

			for ( size_t i = 0, limit = xmlTree.content.size(); i < limit; ++i ) {
				if ( xmlTree.content[i]->kind == kElemNode ) {
					rootElem = xmlTree.content[i];
				}
			}
			if ( rootElem != 0 )
			{
				XMP_StringPtr rootLocalName = rootElem->name.c_str() + rootElem->nsPrefixLen;
				
				if ( XMP_LitMatch ( rootLocalName, "smil" ) ) 
				{
					nameSpace = rootElem->ns.c_str() ;
					size_t noOfBodyElements = rootElem->CountNamedElements ( nameSpace, "body" ) ;
					while( noOfBodyElements-- )
					{
						XML_NodePtr bodyNode = rootElem->GetNamedElement(  nameSpace, "body" );
						size_t noOfParElements = bodyNode->CountNamedElements ( nameSpace, "par" ) ;
						while( noOfParElements-- )
						{
							XML_NodePtr parNode = bodyNode->GetNamedElement(  nameSpace, "par" );
							size_t noOfRefElements = parNode->CountNamedElements ( nameSpace, "ref" ) ;
							size_t whichElem = 0;
							while( noOfRefElements-- )
							{
								XML_NodePtr refNode = parNode->GetNamedElement(  nameSpace, "ref" ,whichElem++ );
								XMP_StringPtr umidValue = refNode->GetAttrValue ( "src" );
								if ( umidValue != 0 && 
										( XMP_LitMatch( umidValue , clipUmid.c_str() ) || 
											( strlen(umidValue) > 15 && XMP_LitMatch( &umidValue[15] , clipUmid.c_str() ) ) 
										)
									)
									{
									delete ( editInfoExpat ) ;
									return true;
								}
							}
						}
					}
				}
			}
		}
		
	} catch ( ... ) {
	}
	delete ( editInfoExpat ) ;
	return false;
} // XDCAM_MetaHandler::RefersClipUmid

// =================================================================================================
// XDCAM_MetaHandler::IsMetadataWritable
// =======================================
bool XDCAM_MetaHandler::IsMetadataWritable ( )
{
	std::vector<std::string> metadataFiles;
	FillMetadataFiles(&metadataFiles);
	std::vector<std::string>::iterator itr = metadataFiles.begin();
	// Check whether sidecar is writable, if not then check if it can be created.
	bool xmpWritable = Host_IO::Writable( itr->c_str(), true );
	// Check for legacy metadata file.
	bool xmlWritable = Host_IO::Writable( (++itr)->c_str(), false );
	return ( xmlWritable && xmpWritable );
}// XDCAM_MetaHandler::IsMetadataWritable

// =================================================================================================
// XDCAM_MetaHandler::FillMetadataFiles
// ====================================
void XDCAM_MetaHandler::FillMetadataFiles ( std::vector<std::string> * metadataFiles )
{
	metadataFiles->push_back( this->sidecarPath );
	metadataFiles->push_back( this->mNRTFilePath );

}	// XDCAM_MetaHandler::FillMetadataFiles

// =================================================================================================
// XDCAM_MetaHandler::CacheFileData
// ================================

void XDCAM_MetaHandler::CacheFileData()
{
	XMP_Assert ( ! this->containsXMP );

	if ( this->parent->UsesClientIO() ) {
		XMP_Throw ( "XDCAM cannot be used with client-managed I/O", kXMPErr_InternalFailure );
	}

	// See if the clip's .XMP file exists.
	if ( ! Host_IO::Exists ( this->sidecarPath.c_str() ) ) return;	// No XMP.

	// Read the entire .XMP file. We know the XMP exists, New_XMPFiles_IO is supposed to return 0
	// only if the file does not exist.

	bool readOnly = XMP_OptionIsClear ( this->parent->openFlags, kXMPFiles_OpenForUpdate );

	XMP_Assert ( this->parent->ioRef == 0 );

	XMPFiles_IO* xmpFile =  XMPFiles_IO::New_XMPFiles_IO ( this->sidecarPath.c_str(), readOnly );
	if ( xmpFile == 0 ) XMP_Throw ( "XDCAM XMP file open failure", kXMPErr_InternalFailure );
	this->parent->ioRef = xmpFile;

	XMP_Int64 xmpLen = xmpFile->Length();
	if ( xmpLen > 100*1024*1024 ) {
		XMP_Throw ( "XDCAM XMP is outrageously large", kXMPErr_InternalFailure );	// Sanity check.
	}

	this->xmpPacket.erase();
	this->xmpPacket.append ( (size_t)xmpLen, ' ' );

	XMP_Int32 ioCount = xmpFile->ReadAll ( (void*)this->xmpPacket.data(), (XMP_Int32)xmpLen );

	this->packetInfo.offset = 0;
	this->packetInfo.length = (XMP_Int32)xmpLen;
	FillPacketInfo ( this->xmpPacket, &this->packetInfo );

	this->containsXMP = true;

}	// XDCAM_MetaHandler::CacheFileData

// =================================================================================================
// XDCAM_MetaHandler::GetMediaProMetadata
// ======================================

// =================================================================================================
// XDCAM_MetaHandler::ProcessXMP
// =============================

void XDCAM_MetaHandler::ProcessXMP()
{

	// Some versions of gcc can't tolerate goto's across declarations.
	// *** Better yet, avoid this cruft with self-cleaning objects.
	#define CleanupAndExit	\
		{																								\
			bool openForUpdate = XMP_OptionIsSet ( this->parent->openFlags, kXMPFiles_OpenForUpdate );	\
			if ( ! openForUpdate ) this->CleanupLegacyXML();											\
			return;																						\
		}

	if ( this->processedXMP ) return;
	this->processedXMP = true;	// Make sure only called once.

	if ( this->containsXMP ) {
		this->xmpObj.ParseFromBuffer ( this->xmpPacket.c_str(), (XMP_StringLen)this->xmpPacket.size() );
	}

	// NonRealTimeMeta -> XMP by schema
	std::string xmlPath = this->mNRTFilePath;
	std::string umid;

	readXMLFile( xmlPath.c_str(), this->expat );
	if ( this->expat == 0 ) return;

	// The root element should be NonRealTimeMeta in some namespace. Take whatever this file uses.

	XML_Node & xmlTree = this->expat->tree;
	XML_NodePtr rootElem = 0;

	for ( size_t i = 0, limit = xmlTree.content.size(); i < limit; ++i ) {
		if ( xmlTree.content[i]->kind == kElemNode ) {
			rootElem = xmlTree.content[i];
		}
	}

	if ( rootElem == 0 ) CleanupAndExit
	XMP_StringPtr rootLocalName = rootElem->name.c_str() + rootElem->nsPrefixLen;
	if ( ! XMP_LitMatch ( rootLocalName, "NonRealTimeMeta" ) ) CleanupAndExit

	this->legacyNS = rootElem->ns;

	// Check the legacy digest.

	XMP_StringPtr legacyNSPtr = this->legacyNS.c_str();

	this->clipMetadata = rootElem;	// ! Save the NonRealTimeMeta pointer for other use.

	std::string oldDigest, newDigest;
	bool digestFound = this->xmpObj.GetStructField ( kXMP_NS_XMP, "NativeDigests", kXMP_NS_XMP, "XDCAM", &oldDigest, 0 );
	if ( digestFound ) {
		this->MakeLegacyDigest ( &newDigest );
		if ( oldDigest == newDigest ) CleanupAndExit
	}

	// If we get here we need find and import the actual legacy elements using the current namespace.
	// Either there is no old digest in the XMP, or the digests differ. In the former case keep any
	// existing XMP, in the latter case take new legacy values.

	this->containsXMP = XDCAM_Support::GetLegacyMetadata ( &this->xmpObj, rootElem, legacyNSPtr, digestFound, umid );
	this->containsXMP |= GetMediaProMetadata ( &this->xmpObj, umid, digestFound );

	CleanupAndExit
	#undef CleanupAndExit

}	// XDCAM_MetaHandler::ProcessXMP

// =================================================================================================
// XDCAM_MetaHandler::UpdateFile
// =============================
//
// Note that UpdateFile is only called from XMPFiles::CloseFile, so it is OK to close the file here.

void XDCAM_MetaHandler::UpdateFile ( bool doSafeUpdate )
{
	if ( ! this->needsUpdate ) return;
	this->needsUpdate = false;	// Make sure only called once.

	XMP_Assert ( this->parent->UsesLocalIO() );

	// Update the internal legacy XML tree if we have one, and set the digest in the XMP.

	bool updateLegacyXML = false;

	if ( this->clipMetadata != 0 ) {
		updateLegacyXML = XDCAM_Support::SetLegacyMetadata ( this->clipMetadata, &this->xmpObj, this->legacyNS.c_str());
	}

	std::string newDigest;
	this->MakeLegacyDigest ( &newDigest );
	this->xmpObj.SetStructField ( kXMP_NS_XMP, "NativeDigests", kXMP_NS_XMP, "XDCAM", newDigest.c_str(), kXMP_DeleteExisting );
	this->xmpObj.SerializeToBuffer ( &this->xmpPacket, this->GetSerializeOptions() );

	// -----------------------------------------------------------------------
	// Update the XMP file first, don't let legacy XML failures block the XMP.

	

	bool haveXMP = Host_IO::Exists ( this->sidecarPath.c_str() );
	if ( ! haveXMP ) {
		XMP_Assert ( this->parent->ioRef == 0 );
		Host_IO::Create ( this->sidecarPath.c_str() );
		this->parent->ioRef = XMPFiles_IO::New_XMPFiles_IO ( this->sidecarPath.c_str(), Host_IO::openReadWrite );
		if ( this->parent->ioRef == 0 ) XMP_Throw ( "Failure opening XDCAM XMP file", kXMPErr_ExternalFailure );
	}

	XMP_IO* xmpFile = this->parent->ioRef;
	XMP_Assert ( xmpFile != 0 );
	XIO::ReplaceTextFile ( xmpFile, this->xmpPacket, (haveXMP & doSafeUpdate) );

	// --------------------------------------------
	// Now update the legacy XML file if necessary.

	if ( updateLegacyXML ) {

		std::string legacyXML, xmlPath;
		this->expat->tree.Serialize ( &legacyXML );
		xmlPath = this->mNRTFilePath;

		bool haveXML = Host_IO::Exists ( xmlPath.c_str() );
		if ( ! haveXML ) Host_IO::Create ( xmlPath.c_str() );

		Host_IO::FileRef hostRef = Host_IO::Open ( xmlPath.c_str(), Host_IO::openReadWrite );
		if ( hostRef == Host_IO::noFileRef ) XMP_Throw ( "Failure opening XDCAM XML file", kXMPErr_ExternalFailure );
		XMPFiles_IO origXML ( hostRef, xmlPath.c_str(), Host_IO::openReadWrite );
		XIO::ReplaceTextFile ( &origXML, legacyXML, (haveXML & doSafeUpdate) );
		origXML.Close();

	}

}	// XDCAM_MetaHandler::UpdateFile

// =================================================================================================
// XDCAM_MetaHandler::WriteTempFile
// ================================

void XDCAM_MetaHandler::WriteTempFile ( XMP_IO* tempRef )
{

	// ! WriteTempFile is not supposed to be called for handlers that own the file.
	XMP_Throw ( "XDCAM_MetaHandler::WriteTempFile should not be called", kXMPErr_InternalFailure );

}	// XDCAM_MetaHandler::WriteTempFile

// =================================================================================================
