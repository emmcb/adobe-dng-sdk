// =================================================================================================
// Copyright Adobe
// Copyright 2014 Adobe
// All Rights Reserved
//
// NOTICE: Adobe permits you to use, modify, and distribute this file in accordance with the terms
// of the Adobe license agreement accompanying it. If you have received this file from a source other 
// than Adobe, then your use, modification, or distribution of it requires the prior written permission
// of Adobe.
// =================================================================================================

#if AdobePrivate
// =================================================================================================
//	This plugin is provided to support Exif metadata in the videos based on ISO Base Media.
//	We currently supports only those videos which were captured by cameras of Canon, Panasonic and FujiFilm.
//  The specifications for embedding Exif metadata in such videos were given by corresponding camera vendors.
// =================================================================================================
#endif // AdobePrivate

#include "XMPFilesPlugins/api/source/PluginBase.h"
#include "XMPFilesPlugins/api/source/PluginRegistry.h"
#include "XMPFilesPlugins/api/source/HostAPIAccess.h"

#include "source/XMPFiles_IO.hpp"
#include "source/XIO.hpp"
#include "source/XMP_LibUtils.hpp"
#include "PluginUtils.h"

#include "XMPFiles/source/FormatSupport/ISOBaseMedia_Support.hpp"
#include "XMPFiles/source/FormatSupport/MOOV_Support.hpp"
#include "XMPFiles/source/FormatSupport/META_Support.hpp"
#include "XMPFilesPlugins/FormatSupport/OEM_Support.hpp"

// MP4OEM_MetaHandler is a handler that supports Exif meta data in MP4 (Not MOV) videos
namespace XMP_PLUGIN
{
	typedef XMP_Uns8 QTErrorMode;

// =================================================================================================
// Class MP4OEM_MetaHandler
//

class MP4OEM_MetaHandler : public PluginBase
{

public:
	MP4OEM_MetaHandler( const std::string& filePath, XMP_Uns32 openFlags, XMP_Uns32 format, XMP_Uns32 handlerFlags )
		: PluginBase( filePath, openFlags, format, handlerFlags ), progressCBInfoPtr( NULL ), exifManager() {}
	~MP4OEM_MetaHandler() {}
	
	static bool initialize();
	static bool terminate();
	static bool checkFileFormat( const std::string& filePath, const IOAdapter& file );

	virtual void cacheFileData( const IOAdapter& file, std::string& xmpStr );
	virtual void updateFile( const IOAdapter& file, bool doSafeUpdate, const std::string& xmpStr );
	virtual void importToXMP( XMP_StringPtr* xmpStr, XMP_StringPtr* packetPtr, XMP_PacketInfo * packetInfo );
	virtual void SetProgressCallback( XMP_ProgressTracker::CallbackInfo * progCBInfoPtr );

	virtual bool getFileModDate ( XMP_DateTime * modDate );
	virtual bool IsMetadataWritable ();
	virtual void FillAssociatedResources( std::vector<std::string> * resourceList );
	
	// =================================================================================================
	// Unused functions in this handler
	virtual void writeTempFile( const IOAdapter& srcFile, const IOAdapter& tmpFile, const std::string& xmpStr ) {};
	static inline bool checkFolderFormat(	const std::string& rootPath, 
											const std::string& gpName, 
											const std::string& parentName, 
											const std::string& leafName ) { return false; }
	// Currently, nothing is done while exporting
	virtual void exportFromXMP( XMP_StringPtr xmpStr ) {};

private:
	static int init;

	// To store moov atom 
	MOOV_Manager moovMgr;
	
	Meta_Manager metaMgr;

	// To support Progress callback in Plug-in
	XMP_ProgressTracker::CallbackInfo * progressCBInfoPtr;

	// To Get Exif Metadata from file
	OEM_Support::ExifManager exifManager;

	// =================================================================================================
	// Below code doesn't relate with Exif issue but is provided to support just like MPEG4_Handler
	// 
	struct AtomInfo {
		XMP_Int64 atomSize;
		XMP_Uns32 atomType;
		bool hasLargeSize;
	};

	enum {
		kBadQT_NoError		= 0,
		kBadQT_SmallInner	= 1,
		kBadQT_LargeInner	= 2,
		kBadQT_SmallOuter	= 3,
		kBadQT_LargeOuter	= 4	
	};

	bool IsXMPUUID ( XMP_IO * fileRef, XMP_Uns64 contentSize, bool unmovedFilePtr = false );
	QTErrorMode GetAtomInfo ( XMP_IO* qtFile, XMP_Int64 spanSize, int nesting, AtomInfo * info );
	QTErrorMode CheckAtomList ( XMP_IO* qtFile, XMP_Int64 spanSize, int nesting );
	void AttemptFileRepair ( XMP_IO* qtFile, XMP_Int64 fileSpace, QTErrorMode status );
	void CheckQTFileStructure ( const std::string filePath, bool doRepair );
	bool ParseAndValidateSemiProXML();
};
// MP4OEM_MetaHandler
// =================================================================================================

int MP4OEM_MetaHandler::init = 0;

//============================================================================
// registerHandler MP4OEM_MetaHandler
//

// Return module identifier string. This string should be same as the string present in 
// the resource file otherwise plug-in won't be loaded.
const char* GetModuleIdentifier()
{
	static const char* kModuleIdentifier = "com.adobe.XMP.plugins.MP4OEM";
	return kModuleIdentifier;
}

// The initialization will be aborted if false is returned.
bool SetupPlugin()
{
	return true;
}

// All the file handlers present in this module will be registered.
// @param sXMPTemplate uid of the file handler, 
void RegisterFileHandlers()
{
	//uid of the file handler. It's different from the module identifier.
	const char* sXMPTemplate = "adobe.coretech.xmp.MP4OEM"; 
	PluginRegistry::registerHandler( new PluginCreator<MP4OEM_MetaHandler>(sXMPTemplate) );
}

// =================================================================================================
// MP4OEM_MetaHandler::initialize
// ====================================
bool MP4OEM_MetaHandler::initialize()
{
	init++;
	if(init != 1)
		return true;
	return SXMPMeta::Initialize();

}	// MP4OEM_MetaHandler::initialize

// =================================================================================================
// MP4OEM_MetaHandler::terminate
// ====================================
bool MP4OEM_MetaHandler::terminate()
{
	init--;
	if(init != 0) return true;
	SXMPMeta::Terminate();	
	return true;
}	// MP4OEM_MetaHandler::terminate

// =================================================================================================
// MP4OEM_MetaHandler::checkFileFormat
// ====================================
// It is derived from CheckFileFormat in MPEG4 Handler
bool MP4OEM_MetaHandler::checkFileFormat( const std::string& filePathStr, const IOAdapter& file )
{
	const char * filePath = filePathStr.c_str();
	Host_IO::FileRef hostRef;

	XMP_Uns8  buffer[ 4 * 1024 ];
	XMP_Uns32 ioCount, brandCount, brandOffset;
	XMP_Uns64 fileSize, nextOffset;
	ISOMedia::BoxInfo currBox;

	if ( Host_IO::GetFileMode( filePath ) != Host_IO::kFMode_IsFile ) return false;

	hostRef = Host_IO::Open( filePath, Host_IO::openReadOnly );
	if ( hostRef == Host_IO::noFileRef ) return false;	// The open failed.
	XMPFiles_IO fileIO( hostRef, filePath, Host_IO::openReadOnly );	// Destructor will close.

	fileSize = fileIO.Length();

	if ( fileSize < 8 ) return false;

	nextOffset = ISOMedia::GetBoxInfo( &fileIO, 0, fileSize, &currBox );
	if ( currBox.headerSize < 8 ) return false;	// Can't be an ISO fileIO.

	if ( currBox.boxType == ISOMedia::k_ftyp )
	{

		// Have an 'ftyp' box, look through the compatible brands. If 'qt  ' is present then this is
		// a modern QuickTime fileIO, regardless of what else is found. Otherwise this is plain ISO if
		// any of the other recognized brands are found.

		if ( currBox.contentSize < 12 ) return false;	// No compatible brands at all.
		if ( currBox.contentSize > 1024 * 1024 ) return false;	// Sanity check and make sure count fits in 32 bits.
		brandCount = ( ( XMP_Uns32 ) currBox.contentSize - 8 ) >> 2;

		fileIO.Seek( 8, kXMP_SeekFromCurrent );	// Skip the major and minor brands.
		ioCount = brandOffset = 0;

		for ( ; brandCount > 0; --brandCount, brandOffset += 4 )
		{
			if ( brandOffset >= ioCount )
			{
				ioCount = 4 * brandCount;
				if ( ioCount > sizeof( buffer ) ) ioCount = sizeof( buffer );
				ioCount = fileIO.ReadAll( buffer, ioCount );
				brandOffset = 0;
			}

			XMP_Uns32 brand = GetUns32BE( &buffer[ brandOffset ] );
			// This handler needs to handle only MPEG4 files
			if ( brand == ISOMedia::k_qt )
				return false;
			else if ( ( brand == ISOMedia::k_mp41 ) || ( brand == ISOMedia::k_mp42 ) ||
				( brand == ISOMedia::k_f4v ) || ( brand == ISOMedia::k_avc1 ) || ( brand == ISOMedia::k_isom ) ||
				( brand == ISOMedia::k_3gp4 ) || ( brand == ISOMedia::k_3g2a ) || ( brand == ISOMedia::k_3g2b ) ||
				( brand == ISOMedia::k_3g2c ) )
				return true;
		}
	}
	return false;
}	// MP4OEM_MetaHandler::checkFileFormat

// =================================================================================================
// MP4OEM_MetaHandler::cacheFileData
// ====================================
// It is almost like cacheFileData as in MPEG4_Handler
void MP4OEM_MetaHandler::cacheFileData( const IOAdapter& file, std::string& xmpStr )
{
	bool readOnly = true;
	if ( getOpenFlags() & kXMPFiles_OpenForUpdate ) 
		readOnly = false;

	bool doRepair = XMP_OptionIsSet( getOpenFlags(), kXMPFiles_OpenRepairFile );

	XMP_StringPtr filePath = this->getPath().c_str();
	if( !readOnly )
		CheckQTFileStructure ( filePath, doRepair );

	XMPFiles_IO fileIO ( Host_IO::Open ( filePath, readOnly ), filePath, readOnly );
	XMP_Uns64 fileSize = fileIO.Length();

	XMP_Uns64 boxPos, boxNext;
	ISOMedia::BoxInfo currBox;
	bool moovFound = false;
	bool metaFound = false;

	for ( boxPos = 0; boxPos < fileSize; boxPos = boxNext )
	{
		boxNext = ISOMedia::GetBoxInfo ( &fileIO, boxPos, fileSize, &currBox );

		if (!moovFound && (currBox.boxType == ISOMedia::k_moov)) {

			XMP_Uns64 fullMoovSize = currBox.headerSize + currBox.contentSize;
			if ( fullMoovSize > TopBoxSizeLimit) {	// From here on we know 32-bit offsets are safe.
				fileIO.Close();
				XMP_Throw ( "Oversize 'moov' box", kXMPErr_EnforceFailure );
			}
			// Storing moov atom but not parsing it now
			this->moovMgr.fullSubtree.assign ( (XMP_Uns32)fullMoovSize, 0 );
			fileIO.Seek ( boxPos, kXMP_SeekFromStart );
			fileIO.Read ( &this->moovMgr.fullSubtree[0], (XMP_Uns32)fullMoovSize );
			moovFound = true;
			if (metaFound)
				break;
		}
		else if (!metaFound && (currBox.boxType == ISOMedia::k_meta))
		{
			XMP_Uns64 fullMetaSize = currBox.headerSize + currBox.contentSize;
			if (fullMetaSize > TopBoxSizeLimit) {	// From here on we know 32-bit offsets are safe.
				fileIO.Close();
				XMP_Throw("Oversize 'moov' box", kXMPErr_EnforceFailure);
			}
			// Storing moov atom but not parsing it now
			this->metaMgr.fullSubtree.assign((XMP_Uns32)fullMetaSize, 0);
			fileIO.Seek(boxPos, kXMP_SeekFromStart);
			fileIO.Read(&this->metaMgr.fullSubtree[0], (XMP_Uns32)fullMetaSize);
			metaFound = true;
			if (moovFound)
				break;
		}
	}
	fileIO.Close();
	if (!moovFound)
	{
		XMP_Error error(kXMPErr_BadFileFormat, "No 'moov' box");
		ErrorNotifyClient(getErrorCallbackInfo(), kXMPErrSev_FileFatal, error);
	}

}	// MP4OEM_MetaHandler::cacheFileData

// =================================================================================================
// MP4OEM_MetaHandler::importToXMP
// ====================================
void MP4OEM_MetaHandler::importToXMP( XMP_StringPtr* str, XMP_StringPtr* packetPtr , XMP_PacketInfo * packetInfo )
{
	bool containsXMP;
	std::string xmpStr;
	std::string oldXMP;

	// Getting XMP packet from standard handler
	bool isSuccess = PluginBase::getXMPStandard( xmpStr, getOpenFlags(), NULL, &containsXMP, &oldXMP, packetInfo, getErrorCallbackInfo(), progressCBInfoPtr );
	if( isSuccess == false )
	{
		XMP_Error error( kXMPErr_PluginImportToXMP, "MP4OEM: Import to XMP from standard handler failed.");
		ErrorNotifyClient( getErrorCallbackInfo(), kXMPErrSev_OperationFatal, error );
	}

	// Parsing of 'MOOV' atom
	this->moovMgr.ParseMemoryTree( MOOV_Manager:: kFileIsNormalISO );

	MOOV_Manager::BoxInfo uuidInfo;
	MOOV_Manager::BoxRef uuidRef = this->moovMgr.GetBox ( "moov/uuid", &uuidInfo );
	MOOV_Manager::BoxRef UDTARef = this->moovMgr.GetBox( "moov/udta", 0 );
	MOOV_Manager::BoxRef CNDARef = NULL;
	MOOV_Manager::BoxRef PANARef = NULL;

	// Importing Canon Exif metadata
	if( uuidRef != 0 && ( memcmp( uuidInfo.idUUID, ISOMedia::k_canonUUID, 16 ) == 0 ) )
	{
		MOOV_Manager::BoxRef CNTHRef = moovMgr.GetTypeChild( uuidRef, ISOMedia::k_CNTH, 0 );
		if( CNTHRef != 0 )
		{
			MOOV_Manager::BoxInfo CNDAInfo;
			CNDARef = this->moovMgr.GetTypeChild( CNTHRef, ISOMedia::k_CNDA, &CNDAInfo );
			if( CNDARef != 0 )
				exifManager.ImportExif( CNDAInfo.content, CNDAInfo.contentSize, xmpStr, OEM_Support::kExifMedia_Canon );
		}
	}

	// Importing Exif data from Panasonic cameras
	if ( UDTARef != 0 )
	{
		MOOV_Manager::BoxInfo PANAInfo;
		PANARef = this->moovMgr.GetTypeChild( UDTARef, ISOMedia::k_PANA, &PANAInfo );
		if (PANARef != 0)
		{
			//Parsing file level meta box.
			this->metaMgr.ParseMetaMemoryTree();
			bool notReadExif = this->ParseAndValidateSemiProXML();
			if (!notReadExif)
				exifManager.ImportExif(PANAInfo.content, PANAInfo.contentSize, xmpStr, OEM_Support::kExifMedia_Panasonic);
		}
	}

	// Can't have both atoms CNTH and PANA in a single file
	if ( CNDARef && PANARef )
	{
		XMP_Error error( kXMPErr_PluginImportToXMP, "MP4OEM: A file can't have more than one vendor specifc atom." );
		ErrorNotifyClient( getErrorCallbackInfo(), kXMPErrSev_FileFatal, error );
	}

	size_t strlength;

	// Storing XMP Packet into str
	if ( containsXMP )
	{
		// Currently supporting only XMP packet upto 4GB
		strlength = xmpStr.length();
		if ( strlength > Max_XMP_Uns32 - 1 )
		{
			XMP_Error error( kXMPErr_PluginImportToXMP, "XMP currently supports XMP packet upto 4 GB" );
			ErrorNotifyClient( getErrorCallbackInfo(), kXMPErrSev_OperationFatal, error );
		}
		StringPtr bufferPtr = HostStringCreateBuffer( static_cast< XMP_Uns32 >( strlength + 1 ) );
		memcpy( bufferPtr, xmpStr.c_str(), strlength );
		bufferPtr[strlength] = '\0';
		*str = bufferPtr;
	}

	// Storing existing XMP Packet into packetPtr
	if ( oldXMP.length() != 0 )
	{
		// Currently supporting only XMP packet upto 4GB
		strlength = oldXMP.length();
		if ( strlength > Max_XMP_Uns32 - 1 )
		{
			XMP_Error error( kXMPErr_PluginImportToXMP, "XMP currently supports XMP packet upto 4 GB" );
			ErrorNotifyClient( getErrorCallbackInfo(), kXMPErrSev_OperationFatal, error );
		}
		StringPtr bufferPtr = HostStringCreateBuffer( static_cast< XMP_Uns32 >( strlength ) +1 );
		memcpy( bufferPtr, oldXMP.c_str(), strlength );
		bufferPtr[strlength] = '\0';
		*packetPtr = bufferPtr;
	}

}	// MP4OEM_MetaHandler::importToXMP

// =================================================================================================
// MP4OEM_MetaHandler::updateFile
// ====================================
void MP4OEM_MetaHandler::updateFile( const IOAdapter& file, bool doSafeUpdate, const std::string& xmpStr )
{
	//
	// Need to do processing over xmp packet before embedding it into the file
	// This needs to be done without breaking any other existing workflows
	// Ideally, we should update native EXIF block

	// Embedding xmp packet in file
	( void ) PluginBase::putXMPStandard( xmpStr.c_str(), getOpenFlags(), NULL, getErrorCallbackInfo(), progressCBInfoPtr );

}	// MP4OEM_MetaHandler::updateFile

// =================================================================================================
// MP4OEM_MetaHandler::IsMetadataWritable
// ====================================
bool MP4OEM_MetaHandler::IsMetadataWritable()
{
	XMP_Bool isWritable;
	if( PluginBase::isMetadataWritableStandardHandler( &isWritable, getOpenFlags() ) )
		return ConvertXMP_BoolToBool( isWritable );
	return false;

}	// MP4OEM_MetaHandler::IsMetadataWritable

// =================================================================================================
// MP4OEM_MetaHandler::getFileModDate
// ====================================
bool MP4OEM_MetaHandler::getFileModDate ( XMP_DateTime * modDate )
{
	XMP_Bool isSuccess;
	if( PluginBase::getFileModDateStandardHandler( modDate, &isSuccess, getOpenFlags() ) )
		return ConvertXMP_BoolToBool( isSuccess );
	return false;

}	// MP4OEM_MetaHandler::getFileModDate

// =================================================================================================
// MP4OEM_MetaHandler::FillAssociatedResources
// ====================================
void MP4OEM_MetaHandler::FillAssociatedResources( std::vector<std::string> * resourceList )
{
	if( ! PluginBase::getAssociatedResourcesStandardHandler( resourceList, getOpenFlags()) )
	{
		XMP_Error error( kXMPErr_PluginFillAssociatedResources, "MP4OEM: Not able to get associated resource" );
		ErrorNotifyClient( getErrorCallbackInfo(), kXMPErrSev_OperationFatal, error );
	}

}	// MP4OEM_MetaHandler::FillAssociatedResources

// =================================================================================================
// MP4OEM_MetaHandler::SetProgressCallback
// ====================================
void MP4OEM_MetaHandler::SetProgressCallback( XMP_ProgressTracker::CallbackInfo * progCBInfoPtr )
{
	if ( progCBInfoPtr != NULL )
		progressCBInfoPtr = new XMP_ProgressTracker::CallbackInfo ( *progCBInfoPtr );

}	// MP4OEM_MetaHandler::SetProgressCallback

// =================================================================================================
// Below functions are added just to handle case excatly as handled by standard handler   
//

// =================================================================================================
// MP4OEM_MetaHandler::IsXMPUUID
// ====================================
bool MP4OEM_MetaHandler::IsXMPUUID ( XMP_IO * fileRef, XMP_Uns64 contentSize, bool unmovedFilePtr /*=false*/ )
{
	if ( contentSize < 16 ) return false;
	XMP_Uns8 uuid [16];
	fileRef->ReadAll ( uuid, 16 );
	if ( unmovedFilePtr ) fileRef->Seek ( -16, kXMP_SeekFromCurrent );
	if ( memcmp ( uuid, ISOMedia::k_xmpUUID, 16 ) != 0 ) return false;	// Check for the XMP GUID.
	return true;
}	// MP4OEM_MetaHandler::IsXMPUUID

// =================================================================================================
// MP4OEM_MetaHandler::GetAtomInfo
// ====================================
QTErrorMode MP4OEM_MetaHandler::GetAtomInfo ( XMP_IO* qtFile, XMP_Int64 spanSize, int nesting, AtomInfo * info )
{
	QTErrorMode status = kBadQT_NoError;
	XMP_Uns8 buffer [8];

	info->hasLargeSize = false;

	qtFile->ReadAll ( buffer, 8 );	// Will throw if 8 bytes aren't available.
	info->atomSize = GetUns32BE ( &buffer[0] );	// ! Yes, the initial size is big endian UInt32.
	info->atomType = GetUns32BE ( &buffer[4] );

	if ( info->atomSize == 0 ) {	// Does the atom extend to EOF?

		if ( nesting != 0 ) return kBadQT_LargeInner;
		info->atomSize = spanSize;	// This outer atom goes to EOF.

	} else if ( info->atomSize == 1 ) {	// Does the atom have a 64-bit size?

		if ( spanSize < 16 ) {	// Is there room in the span for the 16 byte header?
			status = kBadQT_LargeInner;
			if ( nesting == 0 ) status += 2;	// Convert to "outer".
			return status;
		}

		qtFile->ReadAll ( buffer, 8 );
		info->atomSize = (XMP_Int64) GetUns64BE ( &buffer[0] );
		info->hasLargeSize = true;

	}

	XMP_Assert ( status == kBadQT_NoError );
	return status;

}	// MP4OEM_MetaHandler::GetAtomInfo

// =================================================================================================
// MP4OEM_MetaHandler::CheckAtomList
// ====================================
QTErrorMode MP4OEM_MetaHandler::CheckAtomList ( XMP_IO* qtFile, XMP_Int64 spanSize, int nesting )
{
	QTErrorMode status = kBadQT_NoError;
	AtomInfo    info;

	const static XMP_Uns32 moovAtomType = 0x6D6F6F76;	// ! Don't use MakeUns32BE, already big endian.
	const static XMP_Uns32 udtaAtomType = 0x75647461;

	for ( ; spanSize >= 8; spanSize -= info.atomSize ) {

		QTErrorMode atomStatus = GetAtomInfo ( qtFile, spanSize, nesting, &info );
		if ( atomStatus != kBadQT_NoError ) return atomStatus;

		XMP_Int64 headerSize = 8;
		if ( info.hasLargeSize ) headerSize = 16;

		if ( (info.atomSize < headerSize) || (info.atomSize > spanSize) ) {
			status = kBadQT_LargeInner;
			if ( nesting == 0 ) status += 2;	// Convert to "outer".
			return status;
		}

		bool doChildren = false;
		if ( (nesting == 0) && (info.atomType == moovAtomType) ) doChildren = true;
		if ( (nesting == 1) && (info.atomType == udtaAtomType) ) doChildren = true;

		XMP_Int64 dataSize = info.atomSize - headerSize;

		if ( ! doChildren ) {
			qtFile->Seek ( dataSize, kXMP_SeekFromCurrent );
		} else {
			QTErrorMode innerStatus = CheckAtomList ( qtFile, dataSize, nesting+1 );
			if ( innerStatus > kBadQT_SmallInner ) return innerStatus;	// Quit for serious errors.
			if ( status == kBadQT_NoError ) status = innerStatus;	// Remember small inner errors.
		}

	}

	XMP_Assert ( status <= kBadQT_SmallInner );	// Else already returned.
	// ! Make sure inner kBadQT_SmallInner is propagated if this span is OK.

	if ( spanSize != 0 ) {
		qtFile->Seek ( spanSize, kXMP_SeekFromCurrent );	// ! Skip the trailing garbage of this span.
		status = kBadQT_SmallInner;
		if ( nesting == 0 ) status += 2;	// Convert to "outer".
	}

	return status;

}	// MP4OEM_MetaHandler::CheckAtomList

// =================================================================================================
// MP4OEM_MetaHandler::AttemptFileRepair
// ====================================
void MP4OEM_MetaHandler::AttemptFileRepair ( XMP_IO* qtFile, XMP_Int64 fileSpace, QTErrorMode status)
{

	switch ( status ) {
		case kBadQT_NoError    : return;	// Sanity check.
		case kBadQT_SmallInner : return;	// Fixed in normal update code for the 'udta' box.
		case kBadQT_LargeInner : 
			{
				XMP_Error error ( kXMPErr_BadFileFormat, "Can't repair QuickTime file" );
				ErrorNotifyClient( getErrorCallbackInfo(), kXMPErrSev_FileFatal, error);
				break;// will never be here
			}
		case kBadQT_SmallOuter :	// Truncate file below.
		case kBadQT_LargeOuter : 	// Truncate file below.
			{
				break;
			}
		default                : 
			{
				XMP_Error error ( kXMPErr_InternalFailure, "Invalid QuickTime error mode" );
				ErrorNotifyClient( getErrorCallbackInfo(), kXMPErrSev_FileFatal, error);
			}
	}

	AtomInfo info;
	XMP_Int64 headerSize;

	// Process the top level atoms until an error is found.

	qtFile->Rewind();

	for ( ; fileSpace >= 8; fileSpace -= info.atomSize ) {

		QTErrorMode atomStatus = GetAtomInfo ( qtFile, fileSpace, 0, &info );

		headerSize = 8;	// ! Set this before checking atomStatus, used after the loop.
		if ( info.hasLargeSize ) headerSize = 16;

		if ( atomStatus != kBadQT_NoError ) break;
		// If the atom size is less than header -case of kBadQT_SmallOuter
		// If the atom size is more than left filespace -case of kBadQT_LargeOuter
		if ( (info.atomSize < headerSize) || (info.atomSize > fileSpace ) ) break; 

		XMP_Int64 dataSize = info.atomSize - headerSize;
		qtFile->Seek ( dataSize, kXMP_SeekFromCurrent );

	}
	// Truncate only if the last box type was XMP boxes
	// Refrain from truncating a known box as it 
	// might have some useful data which is incomplete
	if ( fileSpace < 8 ||
		 ! ISOMedia::IsKnownBoxType ( info.atomType ) || 
		 ((info.atomType == ISOMedia::k_uuid) && IsXMPUUID(qtFile,info.atomSize-headerSize,true)) ||
		  (info.atomType == ISOMedia::k_XMP_)
		 ){
		
		XMP_Error error ( kXMPErr_BadFileFormat,"Truncate outer EOF Garbage" );
		ErrorNotifyClient( getErrorCallbackInfo(), kXMPErrSev_Recoverable, error);

		// Truncate the file. If fileSpace >= 8 then the loop exited early due to a bad atom, seek back
		// to the atom's start. Otherwise, the loop exited because no more atoms are possible, no seek.

		if ( fileSpace >= 8 ) qtFile->Seek ( -headerSize, kXMP_SeekFromCurrent );
		XMP_Int64 currPos = qtFile->Offset();
		qtFile->Truncate ( currPos );
	}
	else{
		
		XMP_Error error ( kXMPErr_BadFileFormat, "Missing box Data at EOF" );
		ErrorNotifyClient( getErrorCallbackInfo(), kXMPErrSev_FileFatal, error);

	}
}	// MP4OEM_MetaHandler::AttemptFileRepair

// =================================================================================================
// MP4OEM_MetaHandler::CheckQTFileStructure
// ====================================
void MP4OEM_MetaHandler::CheckQTFileStructure ( const std::string filePath ,bool doRepair )
{
	XMPFiles_IO fileIO ( Host_IO::Open ( filePath.c_str(), false ), filePath.c_str(), false );
	XMP_Int64 fileSize = fileIO.Length();

	// Check the basic file structure and try to repair if asked.

	fileIO.Rewind();
	QTErrorMode status = CheckAtomList ( &fileIO, fileSize, 0 );

	if ( status != kBadQT_NoError ) {
		if ( doRepair || (status == kBadQT_SmallInner) || (status == kBadQT_SmallOuter) ) {
			AttemptFileRepair ( &fileIO, fileSize, status );	// Will throw if the attempt fails.
		} else if ( status != kBadQT_SmallInner ) {
			//don't truncate Large Outer EOF garbage unless the client wants it to
			// Clients can pass their intent by setting the flag kXMPFiles_OpenRepairFile
			XMP_Error error ( kXMPErr_BadFileFormat, "Ill-formed QuickTime file" );
			ErrorNotifyClient( getErrorCallbackInfo(), kXMPErrSev_FileFatal, error);
		} 
	}
}	// MP4OEM_MetaHandler::CheckQTFileStructure

bool MP4OEM_MetaHandler::ParseAndValidateSemiProXML()
{
	Meta_Manager::BoxInfo xmlInfo;
	Meta_Manager::BoxRef  xmlRef = this->metaMgr.GetBox("meta/xml ", &xmlInfo);  //space is required after xml to keep the size equal to 4
	if (xmlRef == 0)
		return false;
	XMP_Assert(xmlInfo.boxType == ISOMedia::k_xml);
	if (xmlInfo.contentSize <= 4) // Just enough to check the version/flags at first.
		return false;

	ISOMedia::SemiProXMLMeta semiProXml;
	return semiProXml.ValidateXML(xmlInfo.content + 4, xmlInfo.contentSize - 4);
}
} //namespace XMP_PLUGIN
//============================================================================