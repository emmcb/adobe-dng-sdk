#if AdobePrivate	// *** All plugins are fully private for the time being.

// =================================================================================================
// Copyright Adobe
// Copyright 2011 Adobe
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
//	ADC Amandeep Chawla
//
// mm-dd-yy who Description of changes, most recent on top
//
// 07-04-13 ADC 5.6-f072 [3584418] MXF Handler : GetAssociatedresources API when called for MXF file returns the path
//						 to the sidecar even when it is not present.
// 05-15-13 ADC 5.6-f061 Integrating changes from DVA to MXF_Handler code base, Re factored class declaration and definition into separate files,
//						 and Added MXF_Handler into top level CMAKE.
//
// 01-16-12	AWL	5.5-f006 [2980767] Improve sidecar file update for disk full situations.
//
// 06-27-11	AWL	First draft of MXF handler plugin.
//
// =================================================================================================
#endif // AdobePrivate

#include "MXF_Handler.h"

namespace XMP_PLUGIN {

int MXF_MetaHandler::sInitCount = 0;

typedef char KeyValue[16];
const KeyValue kClosedPartitionFooterKey = { 0x06, 0x0e, 0x2b, 0x34, 0x02, 0x05, 0x01, 0x01, 0x0d, 0x01, 0x02, 0x01, 0x01, 0x04, 0x04, 0x00 };


const char* GetModuleIdentifier() {
	return "com.adobe.XMP.plugins.MXF";
}

// This function will be called during initialization of the plugin.
// Additional host API suite can be requested using RequestAPISuite().
// The initialization will be aborted if false is returned.
bool SetupPlugin() {
	/*
	ExampleSuite* exampleSuite =
	reinterpret_cast<ExampleSuite*>( RequestAPISuite( "exampleSuite", 1 ) );
 
	return (exampleSuite != NULL);
	*/
	return true;
}

// =================================================================================================

void RegisterFileHandlers() {
	PluginRegistry::registerHandler (
		new PluginCreator<MXF_MetaHandler> ( "adobe.coretech.xmp.MXF" )
	);
}

// =================================================================================================

bool MXF_MetaHandler::initialize() {

	++sInitCount;

	if ( sInitCount == 1 ) {
		// Nothing to do.
	}
	
	return true;
}

// =================================================================================================

bool MXF_MetaHandler::terminate() {

	--sInitCount;

	if ( sInitCount == 0 ) {
		// Nothing to do.
	}

	return true;
}

// =================================================================================================

// Even though a sidecar is used for the XMP, look at the MXF file's content to determine the format.
// The MXF file format is defined by SMPTE 377M-2004. The underlying KLV format is in SMPTE 336M-2007.
// Although the MXF specification talks about an optional run-in at the start of a file, that is
// really a means to wrap MXF in some other file format. For our purposes we treat that as the other
// format, so we don't allow any run-in. The file must start with a Header Partition Pack KLV triple.
// A KLV triple has a 16 byte Key, a variable size Length of the value, and a Value.
//
// The Header Partition Pack Key must be 060E2B34 02050101 0D010201 0102xx00, hex bytes in file
// order. The "xx" byte should be in the range 01..04, but we can ignore it.

// For XDCAM Proxy file, there may be 8 bytes for run-in. So to support Proxy file, 
// we need check the Header Partition Pack key after the run-in.

// NOTE: For the static pre-load check only the first 14 bytes are used. This is good enough.

bool MXF_MetaHandler::checkFileFormat ( const std::string& filePathStr, const IOAdapter& file ) {

	// IgnoreParam(file); // Avoid Windows compile warnings.

	const char * filePath = filePathStr.c_str();

	try {

		if ( Host_IO::GetFileMode ( filePath ) != Host_IO::kFMode_IsFile ) return false;

		Host_IO::FileRef hostRef = Host_IO::Open ( filePath, Host_IO::openReadOnly );
		if ( hostRef == Host_IO::noFileRef ) return false;	// The open failed.
		XMPFiles_IO mxfFile ( hostRef, filePath, Host_IO::openReadOnly );	// Destructor will close.

		//	XMPFiles_IO 's function do not function well for growing file 
		if ( Host_IO::Length(hostRef) < 16 ) return false;

		XMP_Uns8 buffer[16];
		Host_IO::Seek(hostRef, 0, kXMP_SeekFromStart);
		XMP_Uns32 readBytes = Host_IO::Read(hostRef, buffer, 16 );	

		if (readBytes == 16)
		{
			if ((GetUns32BE(&buffer[0]) == 0x060E2B34) &&
				(GetUns32BE(&buffer[4]) == 0x02050101) &&
				(GetUns32BE(&buffer[8]) == 0x0D010201) &&
				((GetUns32BE(&buffer[12]) & 0xFFFF00FF) == 0x01020000))
			{
				return true;
			}
		}

		{
			// Handle Proxy file with 8 bytes run-in.
			//	XMPFiles_IO 's function do not function well for growing file 
			if ( Host_IO::Length(hostRef) < 24 ) return false;

			XMP_Uns8 lBuffer[24];
			Host_IO::Seek(hostRef, 0, kXMP_SeekFromStart);
			XMP_Uns32 lReadBytes = Host_IO::Read(hostRef, lBuffer, 24 );

			if (lReadBytes == 24)
			{
				if ((GetUns32BE(&lBuffer[8]) == 0x060E2B34) &&
					(GetUns32BE(&lBuffer[12]) == 0x02050101) &&
					(GetUns32BE(&lBuffer[16]) == 0x0D010201) &&
					((GetUns32BE(&lBuffer[20]) & 0xFFFF00FF) == 0x01020000))
				{
					return true;
				}
			}
		}
	} catch ( ... ) {

		return false;

	}

	return false;
	
}

// =================================================================================================

bool MXF_MetaHandler::checkFolderFormat ( const std::string& rootPath,
										  const std::string& gpName,
										  const std::string& parentName,
										  const std::string& leafName ) {
	return false;	// *** Throw? Should never be called.
}

// =================================================================================================

MXF_MetaHandler::MXF_MetaHandler ( const std::string& filePath, XMP_OptionBits openFlags, XMP_Uns32 format, XMP_Uns32 handlerFlags )
	: PluginBase ( filePath, openFlags, format, handlerFlags ), ioRef ( 0 ) {

	// Nothing else to do yet.

}

// =================================================================================================

MXF_MetaHandler::~MXF_MetaHandler() {

	if ( this->ioRef != 0 ) delete this->ioRef;	// The destructor will close the file.

}

// =================================================================================================
// FindFileExtension
// =================

#define AppendFileExtension 1
// If set then append the .xmp sidecar file extension, do not substitute it for .mxf (or whatever).

#if ! AppendFileExtension
static inline XMP_StringPtr FindFileExtension ( XMP_StringPtr filePath )
{

	XMP_StringPtr pathEnd = filePath + strlen(filePath);
	XMP_StringPtr extPtr;

	for ( extPtr = pathEnd-1; extPtr > filePath; --extPtr ) {
		if ( (*extPtr == '.') || (*extPtr == '/') ) break;
		#if XMP_WinBuild
			if ( (*extPtr == '\\') || (*extPtr == ':') ) break;
		#endif
	}

	if ( (extPtr < filePath) || (*extPtr != '.') ) return pathEnd;
	return extPtr;

}
#endif

// =================================================================================================

void MXF_MetaHandler::SetSidecarPath() {

	const std::string& mxfPath = this->getPath();
	if ( mxfPath.empty() ) return;

	#if AppendFileExtension
		this->sidecarPath = mxfPath;
	#else
		XMP_StringPtr pathPtr = mxfPath.c_str();
		XMP_StringPtr extPtr = FindFileExtension ( mxfPath );
		this->sidecarPath.assign ( mxfPath, (extPtr - pathPtr) );
	#endif

	this->sidecarPath += ".xmp";

}

// =================================================================================================

bool MXF_MetaHandler::getFileModDate ( XMP_DateTime * modDate )
{
	this->SetSidecarPath();

	XMP_DateTime sideCarModDate;
	XMP_DateTime mediaModDate;

	bool sideCarModDateIsValid(false);
	bool mediaModDateIsValid(false);

	if (!this->sidecarPath.empty())
	{
		sideCarModDateIsValid = Host_IO::GetModifyDate(this->sidecarPath.c_str(), &sideCarModDate);
	}

	if (!this->getPath().empty())
	{
		mediaModDateIsValid = Host_IO::GetModifyDate(this->getPath().c_str(), &mediaModDate);
	}

	if (!sideCarModDateIsValid || !mediaModDateIsValid)
	{
		return false;
	}

	// now check for growing status - if the file doesn't have a footer partition, it's deemed incomplete
	const int kEndBufferSize = 64 * 1024;
	Host_IO::FileRef hostRef = Host_IO::Open(this->getPath().c_str(), Host_IO::openReadOnly);
	if (hostRef == Host_IO::noFileRef)
	{
		return false;	// The open failed.
	}
	XMPFiles_IO mxfFile(hostRef, this->getPath().c_str(), Host_IO::openReadOnly);
	bool fileIsClosed(true);

	//	XMPFiles_IO 's function do not function well for growing file 
	if (Host_IO::Length(hostRef)> (XMP_Int64)kEndBufferSize)
	{
		Host_IO::Seek(hostRef, -kEndBufferSize, kXMP_SeekFromEnd);

		std::string buffer;
		buffer.resize(kEndBufferSize);
		Host_IO::Read(hostRef, &buffer[0], kEndBufferSize);

		// reverse find on the footer partition key
		size_t offset = buffer.rfind(kClosedPartitionFooterKey);
		if (offset == std::string::npos)
		{
			fileIsClosed = false;
		}
	}

	if (fileIsClosed)
	{
		*modDate = SXMPUtils::CompareDateTime(mediaModDate, sideCarModDate) > 0 ? mediaModDate : sideCarModDate;
	}
	else
	{
		// file is not closed, report back the sidecar's last time stamp until the file is complete
		*modDate = sideCarModDate;
	}

	return true;
}

// =================================================================================================

void MXF_MetaHandler::cacheFileData ( const IOAdapter& file, std::string& xmpStr ) {

	xmpStr.erase();
	bool readOnly = (! (this->getOpenFlags() & kXMPFiles_OpenForUpdate));

	// Try to open the sidecar XMP file. Tolerate an open failure, there might not be any XMP.
	// Note that checkFileFormat can't save the sidecar path because the handler instance doesn't exist then.
	
	this->SetSidecarPath();
	if ( this->sidecarPath.empty() ) return;
	if ( ! Host_IO::Exists ( this->sidecarPath.c_str() ) ) return;	// OK to not have XMP.

	XMPFiles_IO * xmpFile = XMPFiles_IO::New_XMPFiles_IO ( this->sidecarPath.c_str(), readOnly );
	if ( xmpFile == 0 ) XMP_Throw ( "Failure opening MXF XMP file", kXMPErr_ExternalFailure );
	this->ioRef = xmpFile;

	// Extract the sidecar's contents.

	XMP_Int64 xmpLength = xmpFile->Length();
	if ( xmpLength > 1024*1024*1024 ) {	// Sanity check and to not overflow std::string.
		XMP_Throw ( "MXF XMP sidecar file is too large", kXMPErr_ExternalFailure );
	}

	if ( xmpLength > 0 ) {
		xmpStr.assign ( (size_t)xmpLength, ' ' );
		xmpFile->ReadAll ( (void*)xmpStr.c_str(), (XMP_Uns32)xmpLength );
	}

	if ( readOnly ) {
		xmpFile->Close();
		delete xmpFile;
		this->ioRef = 0;
	}

}

// =================================================================================================

void MXF_MetaHandler::updateFile ( const IOAdapter& file, bool doSafeUpdate, const std::string& xmpStr ) {

	if ( this->ioRef == 0 ) {
		XMP_Assert ( ! Host_IO::Exists ( this->sidecarPath.c_str() ) );
		Host_IO::Create ( this->sidecarPath.c_str() );
		this->ioRef = XMPFiles_IO::New_XMPFiles_IO ( this->sidecarPath.c_str(), Host_IO::openReadWrite );
		if ( this->ioRef == 0 ) XMP_Throw ( "Failure opening MXF XMP file", kXMPErr_ExternalFailure );
	}

	XMP_IO* fileRef = this->ioRef;
	XMP_Assert ( fileRef != 0 );
	XIO::ReplaceTextFile ( fileRef, xmpStr, doSafeUpdate );
	delete this->ioRef;	// The destructor will close the file.
	this->ioRef = 0;

}

// =================================================================================================

void MXF_MetaHandler::FillMetadataFiles( std::vector<std::string> * metadataFiles ) {

	this->SetSidecarPath();
	if ( this->sidecarPath.empty() ) return;
	metadataFiles->push_back(this->sidecarPath);

}

// =================================================================================================

void MXF_MetaHandler::FillAssociatedResources( std::vector<std::string> * resourceList ) 
{
	resourceList->push_back(this->getPath());
	this->SetSidecarPath();
	if ( !this->sidecarPath.empty() && Host_IO::Exists( (this->sidecarPath).c_str() ) )
		resourceList->push_back( this->sidecarPath );
}

// =================================================================================================

bool MXF_MetaHandler::IsMetadataWritable( )
{
	std::vector<std::string> metadataFiles;
	FillMetadataFiles(&metadataFiles);
	//only check for the sidecar file as we only write the XMP to this file
	return Host_IO::Writable( metadataFiles[0].c_str(), true );
}

// =================================================================================================
}
#endif 	// AdobePrivate	// *** All plugins are fully private for the time being.
