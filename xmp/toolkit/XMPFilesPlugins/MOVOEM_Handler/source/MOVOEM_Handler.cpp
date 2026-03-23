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
//	This plugin is provided to support Exif metadata in the quicktime videos.
//	We currently supports only such videos which are captured by cameras of Canon, fujifilm and Panasonic.
//  The specifications of embeding Exif metadata in such videos were being given by Canon, fujifilm and Panasonic.
// =================================================================================================
#endif // AdobePrivate

#include "XMPFilesPlugins/api/source/PluginBase.h"
#include "XMPFilesPlugins/api/source/PluginRegistry.h"
#include "XMPFilesPlugins/api/source/HostAPIAccess.h"
#include "PluginUtils.h"

#include "source/XMPFiles_IO.hpp"
#include "source/XIO.hpp"
#include "source/XMP_LibUtils.hpp"

#include "XMPFiles/source/FormatSupport/ISOBaseMedia_Support.hpp"
#include "XMPFiles/source/FormatSupport/MOOV_Support.hpp"
#include "XMPFiles/source/FormatSupport/QuickTime_Support.hpp"
#include "XMPFilesPlugins/FormatSupport/OEM_Support.hpp"

// MOVOEM_MetaHandler is a handler that supports Exif meta data in QT videos
namespace XMP_PLUGIN
{
	typedef XMP_Uns8 QTErrorMode;

// =================================================================================================
// Class MOVOEM_MetaHandler
//

class MOVOEM_MetaHandler : public PluginBase
{

public:
	MOVOEM_MetaHandler( const std::string& filePath, XMP_Uns32 openFlags, XMP_Uns32 format, XMP_Uns32 handlerFlags )
		: PluginBase( filePath, openFlags, format, handlerFlags ), progressCBInfoPtr(NULL), exifManager()  {}
	~MOVOEM_MetaHandler() {}
	
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
	// Exporting need not to do 
	virtual void exportFromXMP( XMP_StringPtr xmpStr ) {};

private:
	static int init;

	// To store Mode of QT i.e, Modern or Classic which is dependent upon presence of 'ftyp' box
	XMP_Uns8 mFileMode;
	// To store moov atom 
	MOOV_Manager moovMgr;
	
	//To check presence of panasonic xml
	TradQT_Manager tradQTMgr;

	// To support Progress callback in Plug-in
	XMP_ProgressTracker::CallbackInfo * progressCBInfoPtr;

	// To Get Exif Metadata from file
	OEM_Support::ExifManager exifManager;

	// =================================================================================================
	// Below code doesn't relate with Exif issue but provided to support same as MPEG4_Handler
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
// Class MOVOEM_MetaHandler
// =================================================================================================

int MOVOEM_MetaHandler::init = 0;
#define IsTolerableBoxChar(ch)	( ((0x20 <= (ch)) && ((ch) <= 0x7E)) || ((ch) == 0xA9) )

//============================================================================
// registerHandler MOVOEM_MetaHandler
//

// Return module identifier string. This string should be same as the string present in 
// the resource file otherwise plug-in won't be loaded.
const char* GetModuleIdentifier()
{
	static const char* kModuleIdentifier = "com.adobe.XMP.plugins.MOVOEM";
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
	const char* sXMPTemplate = "adobe.coretech.xmp.MOVOEM"; 
	PluginRegistry::registerHandler( new PluginCreator<MOVOEM_MetaHandler>(sXMPTemplate) );
}

//============================================================================
// Static Functions to support compatibility with standard MPEG4_Handler
//
static bool IsClassicQuickTimeBox ( XMP_Uns32 boxType )
{
	if ( (boxType == ISOMedia::k_moov) || (boxType == ISOMedia::k_mdat) || (boxType == ISOMedia::k_pnot) ||
		 (boxType == ISOMedia::k_free) || (boxType == ISOMedia::k_skip) || (boxType == ISOMedia::k_wide) ) return true;
	return false;
}

static bool IsTolerableBox ( XMP_Uns32 boxType )
{
	XMP_Uns8 b1 = (XMP_Uns8) (boxType >> 24);
	XMP_Uns8 b2 = (XMP_Uns8) ((boxType >> 16) & 0xFF);
	XMP_Uns8 b3 = (XMP_Uns8) ((boxType >> 8) & 0xFF);
	XMP_Uns8 b4 = (XMP_Uns8) (boxType & 0xFF);
	bool ok = IsTolerableBoxChar(b1) && IsTolerableBoxChar(b2) &&
		IsTolerableBoxChar(b3) && IsTolerableBoxChar(b4);
	return ok;
}
//
// 
// ============================================================================

// =================================================================================================
// MOVOEM_MetaHandler::initialize
// ====================================
bool MOVOEM_MetaHandler::initialize()
{
	init++;
	if(init != 1) return true;
	{
		SXMPMeta::Initialize();
	}
	return true;
}	// MOVOEM_MetaHandler::initialize

// =================================================================================================
// MOVOEM_MetaHandler::terminate
// ====================================
bool MOVOEM_MetaHandler::terminate()
{
	init--;
	if(init != 0) return true;
	SXMPMeta::Terminate();	
	return true;
}	// MOVOEM_MetaHandler::terminate

// =================================================================================================
// MOVOEM_MetaHandler::checkFileFormat
// ====================================
// It is like CheckFileFormat in MPEG4 Handler
bool MOVOEM_MetaHandler::checkFileFormat( const std::string& filePathStr, const IOAdapter& file )
{
	const char * filePath = filePathStr.c_str();

	XMP_Uns64 fileSize, nextOffset;

	ISOMedia::BoxInfo currBox;

	if ( Host_IO::GetFileMode( filePath ) != Host_IO::kFMode_IsFile ) return false;

	Host_IO::FileRef hostRef = Host_IO::Open( filePath, Host_IO::openReadOnly );
	if ( hostRef == Host_IO::noFileRef ) return false;	// The open failed.
	XMPFiles_IO fileIO( hostRef, filePath, Host_IO::openReadOnly );	// Destructor will close.

	fileSize = fileIO.Length();

	if ( fileSize < 8 ) return false;

	nextOffset = ISOMedia::GetBoxInfo( &fileIO, 0, fileSize, &currBox );
	if ( currBox.headerSize < 8 ) return false;	// Can't be an ISO file.

	if ( currBox.boxType == ISOMedia::k_ftyp )
	{
		// Have an 'ftyp' box, look through the compatible brands. If 'qt  ' is present then this is
		// a modern QuickTime file, regardless of what else is found.

		XMP_Uns32 ioCount, brandCount, brandOffset;
		XMP_Uns8  buffer[ 4 * 1024 ];

		ioCount = brandOffset = 0;

		if ( currBox.contentSize < 12 ) return false;	// No compatible brands at all.
		if ( currBox.contentSize > 1024 * 1024 ) return false;	// Sanity check and make sure count fits in 32 bits.
		brandCount = ( ( XMP_Uns32 ) currBox.contentSize - 8 ) >> 2;

		fileIO.Seek( 8, kXMP_SeekFromCurrent );	// Skip the major and minor brands.
		
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
			if ( brand == ISOMedia::k_qt ) // Don't need to look further, We just need QT brand.
				return true;
		}
	}
	else
	{
#if AdobePrivate
		// Don't waste time in searching for classic QuickTime boxes in the file if it is a 
		// R3D RAW 
		if ( currBox.boxType == ISOMedia::k_RED1 || currBox.boxType == ISOMedia::k_REDV || currBox.boxType == ISOMedia::k_RED2 )
			return false;
#endif // AdobePrivate
		// No 'ftyp', look for classic QuickTime: 'moov', 'mdat', 'pnot', 'free', 'skip', and 'wide'.
		// As an expedient, quit when a 'moov' box is found. Tolerate other boxes, they are in the
		// wild for ill-formed files, e.g. seen when 'moov'/'udta' children get left at top level.

		while ( currBox.boxType != ISOMedia::k_moov )
		{
			if ( !IsClassicQuickTimeBox( currBox.boxType ) )
				if ( !IsTolerableBox( currBox.boxType ) )
					return false;
			if ( nextOffset >= fileSize ) return false;
			nextOffset = ISOMedia::GetBoxInfo( &fileIO, nextOffset, fileSize, &currBox );
		}
		return true;
	}

	return false;

}	// MOVOEM_MetaHandler::checkFileFormat

// =================================================================================================
// MOVOEM_MetaHandler::cacheFileData
// ====================================
// It is almost like cacheFileData as in MPEG4_Handler
void MOVOEM_MetaHandler::cacheFileData( const IOAdapter& file, std::string& xmpStr )
{
	bool readOnly = true;
	if ( getOpenFlags() & kXMPFiles_OpenForUpdate )
		readOnly = false;

	bool doRepair = XMP_OptionIsSet( getOpenFlags(), kXMPFiles_OpenRepairFile );
	
	XMP_StringPtr filePath = this->getPath().c_str();
	if( !readOnly )
		CheckQTFileStructure ( filePath, doRepair);

	XMPFiles_IO fileIO ( Host_IO::Open ( filePath, readOnly ), filePath, readOnly );
	XMP_Uns64 fileSize = fileIO.Length();
	
	XMP_Uns64 boxPos, boxNext;
	ISOMedia::BoxInfo currBox;

	for ( boxPos = 0; boxPos < fileSize; boxPos = boxNext ) {

		boxNext = ISOMedia::GetBoxInfo ( &fileIO, boxPos, fileSize, &currBox );

		if ( boxPos == 0  )
		{
			// Modern QT if first atom is ftyp atom,else Classic QT file
			if( currBox.boxType == ISOMedia::k_ftyp )
				this->mFileMode = MOOV_Manager::kFileIsModernQT ;
			else 
				this->mFileMode = MOOV_Manager::kFileIsTraditionalQT ;
		}
		if ( currBox.boxType == ISOMedia::k_moov ) {

			XMP_Uns64 fullMoovSize = currBox.headerSize + currBox.contentSize;
			if ( fullMoovSize > TopBoxSizeLimit) {
				XMP_Throw ( "Oversize 'moov' box", kXMPErr_EnforceFailure );
			}
			// Storing moov atom but not parsing it now
			this->moovMgr.fullSubtree.assign ( (XMP_Uns32)fullMoovSize, 0 );
			fileIO.Seek ( boxPos, kXMP_SeekFromStart );
			fileIO.Read ( &this->moovMgr.fullSubtree[0], (XMP_Uns32)fullMoovSize );
			fileIO.Close();
			return;
		}
	}
	fileIO.Close();
	XMP_Error error ( kXMPErr_BadFileFormat,"No 'moov' box" );
	ErrorNotifyClient( getErrorCallbackInfo() , kXMPErrSev_FileFatal, error);
}	// MOVOEM_MetaHandler::cacheFileData

// =================================================================================================
// MOVOEM_MetaHandler::importToXMP
// ====================================
void MOVOEM_MetaHandler::importToXMP( XMP_StringPtr* str, XMP_StringPtr* packetPtr, XMP_PacketInfo * packetInfo )
{
	bool containsXMP;
	std::string xmpStr;
	std::string oldXMP;

	// Getting XMP packet from standard handler
	bool isSuccess = PluginBase::getXMPStandard( xmpStr, getOpenFlags(), NULL, &containsXMP, &oldXMP, packetInfo, getErrorCallbackInfo(), progressCBInfoPtr );
	if( isSuccess == false )
	{
		XMP_Error error( kXMPErr_PluginImportToXMP, "MOVOEM: Import to XMP from standard handler failed." );
		ErrorNotifyClient( getErrorCallbackInfo(), kXMPErrSev_OperationFatal, error );
	}
	
	// Parsing of 'MOOV' atom
	this->moovMgr.ParseMemoryTree( this->mFileMode );

	MOOV_Manager::BoxRef UDTARef = this->moovMgr.GetBox ( "moov/udta", 0 );
	if ( UDTARef != 0 )
	{
		// Importing Canon Exif metadata
		MOOV_Manager::BoxInfo CNTHInfo;
		MOOV_Manager::BoxRef  CNTHRef = this->moovMgr.GetTypeChild( UDTARef, ISOMedia::k_CNTH, &CNTHInfo );
		if( CNTHRef != 0 )
		{
			MOOV_Manager::BoxInfo CNDAInfo;
			MOOV_Manager::BoxRef  CNDARef = this->moovMgr.GetTypeChild( CNTHRef, ISOMedia::k_CNDA, &CNDAInfo );
			if( CNDARef != 0 )
				exifManager.ImportExif( CNDAInfo.content, CNDAInfo.contentSize, xmpStr, OEM_Support::kExifMedia_Canon );
		}

		// Importing FujiFilm Exif metadata
		MOOV_Manager::BoxInfo MVTGInfo;
		MOOV_Manager::BoxRef  MVTGRef = this->moovMgr.GetTypeChild ( UDTARef, ISOMedia::k_MVTG, &MVTGInfo );
		if( MVTGRef != 0 )
			exifManager.ImportExif( MVTGInfo.content, MVTGInfo.contentSize, xmpStr, OEM_Support::kExifMedia_Fujifilm );

		// Importing Panasonic Exif metadata
		MOOV_Manager::BoxInfo PANAInfo;
		MOOV_Manager::BoxRef  PANARef = this->moovMgr.GetTypeChild( UDTARef, ISOMedia::k_PANA, &PANAInfo );
		if (PANARef != 0)
		{
			bool notReadExif = this->tradQTMgr.ParseMetaBox(this->moovMgr); //to parse metadata present in moov/meta box
			if (notReadExif)
				notReadExif = this->ParseAndValidateSemiProXML();
			if(!notReadExif)
				exifManager.ImportExif(PANAInfo.content, PANAInfo.contentSize, xmpStr, OEM_Support::kExifMedia_Panasonic);

		}
		// Can't have any two atoms among the CNTH, MVTG and PANA atoms in a single file
		if ( CNTHRef ? !( !MVTGRef && !PANARef ) : ( MVTGRef && PANARef ) )
		{
			XMP_Error error( kXMPErr_PluginImportToXMP, "MOVOEM: A file can't have more than one vendor atom");
			ErrorNotifyClient( getErrorCallbackInfo(), kXMPErrSev_FileFatal, error );
		}
	}

	size_t strLength;
	
	// Storing XMP Packet into str
	if ( containsXMP )
	{
		// Currently supporting only XMP packet upto 4GB
		strLength = xmpStr.length();
		if ( strLength > Max_XMP_Uns32 - 1 )
		{
			XMP_Error error( kXMPErr_PluginImportToXMP, "XMP currently supports XMP packet upto 4 GB" );
			ErrorNotifyClient( getErrorCallbackInfo(), kXMPErrSev_OperationFatal, error );
		}
		// Memory allocation is done using Host APIs, So memory free must be done by Host library
		StringPtr bufferPtr = HostStringCreateBuffer( static_cast< XMP_Uns32 >( strLength + 1 ) );
		memcpy( bufferPtr, xmpStr.c_str(), strLength );
		bufferPtr[ strLength ] = '\0';
		*str = bufferPtr;
	}

	// Storing existing XMP Packet into packetPtr
	if ( oldXMP.length() != 0 )
	{
		// Currently supporting only XMP packet upto 4GB
		strLength = oldXMP.length();
		if ( strLength > Max_XMP_Uns32 - 1 )
		{
			XMP_Error error( kXMPErr_PluginImportToXMP, "XMP currently supports XMP packet upto 4 GB" );
			ErrorNotifyClient( getErrorCallbackInfo(), kXMPErrSev_OperationFatal, error );
		}
		// Memory allocation is done using Host APIs, So memory free must be done by Host library
		StringPtr bufferPtr = HostStringCreateBuffer( static_cast< XMP_Uns32 >( strLength + 1 ) );
		memcpy( bufferPtr, oldXMP.c_str(), strLength );
		bufferPtr[ strLength ] = '\0';
		*packetPtr = bufferPtr;
	}

}	// MOVOEM_MetaHandler::importToXMP

// =================================================================================================
// MOVOEM_MetaHandler::updateFile
// ====================================
void MOVOEM_MetaHandler::updateFile( const IOAdapter& file, bool doSafeUpdate, const std::string& xmpStr )
{
	//
	// Need to do processing over xmp packet before embedding it into the file
	// This needs to be done without breaking any other existing workflows
	// Ideally, we should update native EXIF block

	// Embedding xmp packet in file
	( void ) PluginBase::putXMPStandard( xmpStr.c_str(), getOpenFlags(), NULL, getErrorCallbackInfo(), progressCBInfoPtr );

}	// MOVOEM_MetaHandler::updateFile

// =================================================================================================
// MOVOEM_MetaHandler::IsMetadataWritable
// ====================================
bool MOVOEM_MetaHandler::IsMetadataWritable()
{
	XMP_Bool isWritable;
	if( PluginBase::isMetadataWritableStandardHandler( &isWritable, getOpenFlags() ) )
		return ConvertXMP_BoolToBool( isWritable );
	return false;

}	// MOVOEM_MetaHandler::IsMetadataWritable

// =================================================================================================
// MOVOEM_MetaHandler::getFileModDate
// ====================================
bool MOVOEM_MetaHandler::getFileModDate ( XMP_DateTime * modDate )
{
	XMP_Bool isSuccess;
	if( PluginBase::getFileModDateStandardHandler( modDate, &isSuccess, getOpenFlags() ) )
		return ConvertXMP_BoolToBool( isSuccess );
	return false;

}	// MOVOEM_MetaHandler::getFileModDate

// =================================================================================================
// MOVOEM_MetaHandler::FillAssociatedResources
// ====================================
void MOVOEM_MetaHandler::FillAssociatedResources( std::vector<std::string> * resourceList )
{
	if( ! PluginBase::getAssociatedResourcesStandardHandler( resourceList, getOpenFlags()) )
	{
		XMP_Error error(kXMPErr_PluginFillAssociatedResources,"MOVOEM: Not able to get associated resource");
		ErrorNotifyClient( getErrorCallbackInfo(), kXMPErrSev_OperationFatal, error );
	}

}	// MOVOEM_MetaHandler::FillAssociatedResources

// =================================================================================================
// MOVOEM_MetaHandler::SetProgressCallback
// ====================================
void MOVOEM_MetaHandler::SetProgressCallback( XMP_ProgressTracker::CallbackInfo * progCBInfoPtr )
{
	if ( progCBInfoPtr != NULL )
		progressCBInfoPtr = new XMP_ProgressTracker::CallbackInfo ( *progCBInfoPtr );

}	// MOVOEM_MetaHandler::SetProgressCallback

// =================================================================================================
// Below functions are added just to handle case excatly as handled by standard handler   
//

// =================================================================================================
// MOVOEM_MetaHandler::IsXMPUUID
// ====================================
bool MOVOEM_MetaHandler::IsXMPUUID ( XMP_IO * fileRef, XMP_Uns64 contentSize, bool unmovedFilePtr /*=false*/ )
{
	if ( contentSize < 16 ) return false;
	XMP_Uns8 uuid [16];
	fileRef->ReadAll ( uuid, 16 );
	if ( unmovedFilePtr ) fileRef->Seek ( -16, kXMP_SeekFromCurrent );
	if ( memcmp ( uuid, ISOMedia::k_xmpUUID, 16 ) != 0 ) return false;	// Check for the XMP GUID.
	return true;
}	// MOVOEM_MetaHandler::IsXMPUUID

// =================================================================================================
// MOVOEM_MetaHandler::GetAtomInfo
// ====================================
QTErrorMode MOVOEM_MetaHandler::GetAtomInfo ( XMP_IO* qtFile, XMP_Int64 spanSize, int nesting, AtomInfo * info )
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

}	// MOVOEM_MetaHandler::GetAtomInfo

// =================================================================================================
// MOVOEM_MetaHandler::CheckAtomList
// ====================================
QTErrorMode MOVOEM_MetaHandler::CheckAtomList ( XMP_IO* qtFile, XMP_Int64 spanSize, int nesting )
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

}	// MOVOEM_MetaHandler::CheckAtomList

// =================================================================================================
// MOVOEM_MetaHandler::AttemptFileRepair
// ====================================
void MOVOEM_MetaHandler::AttemptFileRepair ( XMP_IO* qtFile, XMP_Int64 fileSpace, QTErrorMode status)
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
}	// MOVOEM_MetaHandler::AttemptFileRepair

// =================================================================================================
// MOVOEM_MetaHandler::CheckQTFileStructure
// ====================================
void MOVOEM_MetaHandler::CheckQTFileStructure ( const std::string filePath ,bool doRepair )
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
}	// MOVOEM_MetaHandler::CheckQTFileStructure

 bool MOVOEM_MetaHandler::ParseAndValidateSemiProXML()
{
		std::string data;
		XMP_Uns32 index;
		bool found = tradQTMgr.FindMetaAtom("com.panasonic.Semi-Pro.metadata.xml", data, index);
		if (found)
		{
			ISOMedia::SemiProXMLMeta semiProXml;
			return semiProXml.ValidateXML(data.c_str(),(XMP_Uns32) data.size());
		}
		else
			return false;
}

} //namespace XMP_PLUGIN
//============================================================================