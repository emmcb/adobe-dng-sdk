#include "XMPFilesPlugins/api/source/PluginBase.h"
#include "XMPFilesPlugins/api/source/PluginRegistry.h"
#include "XMPFilesPlugins/api/source/HostAPIAccess.h"

#include "source/Host_IO.hpp"

#include "CorCalls.h"
#include "CosCalls.h"
#include "ASCalls.h"
#include "PDCalls.h"
#include "ASExtraCalls.h"
#include "MyPDFLibUtils.h"
#include <stdio.h>


#if WIN_ENV
	typedef DWORD ThreadLocalKey;
#elif UNIX_PLATFORM || MAC_PLATFORM
	#include <pthread.h>
	typedef pthread_key_t ThreadLocalKey;
#else
	#error "Platform not supported!"
#endif

#if XMP_64
	typedef XMP_Uns64 FunctionCalls;
#else
	typedef XMP_Uns32 FunctionCalls;
#endif

using namespace XMP_PLUGIN;
namespace PDF_Handler
{

class ThreadLocalStorage
{
public:
	static XMP_Bool Init();
	static XMP_Bool Term();
	static XMP_Bool GetValue ( FunctionCalls & value );
	static XMP_Bool SetValue ( FunctionCalls value );

private:
	static ThreadLocalKey sThreadLocalKey;
};

#if WIN_ENV

ThreadLocalKey ThreadLocalStorage::sThreadLocalKey = TLS_OUT_OF_INDEXES;
XMP_Bool ThreadLocalStorage::Init()
{
	sThreadLocalKey = TlsAlloc();
	if ( sThreadLocalKey == TLS_OUT_OF_INDEXES )
		return false;
	SetValue (0);
	return true;
}

XMP_Bool ThreadLocalStorage::Term()
{
	if ( sThreadLocalKey != TLS_OUT_OF_INDEXES )
	{
		BOOL retValue = TlsFree ( sThreadLocalKey );
		if ( retValue )
			sThreadLocalKey = TLS_OUT_OF_INDEXES;
		return retValue;
	}
	return true;
}

XMP_Bool ThreadLocalStorage::GetValue ( FunctionCalls & value )
{
	if ( sThreadLocalKey != TLS_OUT_OF_INDEXES )
	{
		void * retValue = TlsGetValue ( sThreadLocalKey );
		if ( retValue )
		{
			value = reinterpret_cast  <FunctionCalls > ( retValue );
			return true;
		}
		else
		{
			if ( GetLastError() == ERROR_SUCCESS )
			{
				value = 0;
				return true;
			}
		}
	}
	return false;
}

XMP_Bool ThreadLocalStorage::SetValue ( FunctionCalls value )
{
	if ( sThreadLocalKey != TLS_OUT_OF_INDEXES )
	{
		return TlsSetValue ( sThreadLocalKey, reinterpret_cast < void * > ( value ) );
	}
	return false;
}
#elif UNIX_PLATFORM || MAC_PLATFORM

ThreadLocalKey ThreadLocalStorage::sThreadLocalKey;
XMP_Bool ThreadLocalStorage::Init()
{
	if ( pthread_key_create (&sThreadLocalKey, NULL ) != 0 )
		return false;
	SetValue (0);
	return true;
}

XMP_Bool ThreadLocalStorage::Term()
{
	if ( pthread_key_delete ( sThreadLocalKey ) != 0 )
		return false;
	return true;
}

XMP_Bool ThreadLocalStorage::GetValue ( FunctionCalls & value )
{
	void * tempValue = pthread_getspecific ( sThreadLocalKey );
	value = reinterpret_cast < FunctionCalls > ( tempValue );
	return true;
}

XMP_Bool ThreadLocalStorage::SetValue ( FunctionCalls value )
{
	if ( pthread_setspecific ( sThreadLocalKey, reinterpret_cast < void * > ( value ) ) != 0 )
		return false;
	return true;
}

#endif

/*****************************************************************************
	Static functions declarations
*****************************************************************************/
static CosObj CosDocGetXAPStream(CosDoc cosDoc);
static void CosStreamGetString(CosObj cosStream, std::string & result);
static void CosDocSerializeMetaXAPToStream(CosDoc cosDoc, const std::string& xmpStr );
static void UpdateMetaXAPFromDocInfo(CosDoc cosDoc, SXMPMeta * metaXAP);
static void ClearDocInfo( CosDoc cosDoc );

extern void XAPDateTimeToPDFDate(std::string & pdfDate, XMP_DateTime & xapDateTime);
extern void CosDocUpdateDocInfoFromXAP(CosDoc cosDoc, const SXMPMeta& metaXAP);
extern bool IsDateProperty(ASAtom nameAtom);
extern void SetDateItemInXAP( SXMPMeta * metaXAP, const std::string & nmspace, const char * key, const std::string & value );
extern void SetInfoItemInXAP(SXMPMeta * metaXAP, const char * key, const std::string& value);
extern void SetupMetaXAP(SXMPMeta * metaXAP);
extern ASBool PDFDateToXAPDateTime(const char *buf, XMP_DateTime & xapDateTime);

// =================================================================================================
/// \file PDF_Handler.hpp
/// \brief File format handler for PDF.
// =================================================================================================

class PDF_MetaHandler : public PluginBase
{
public:
	PDF_MetaHandler( const std::string& filePath, XMP_Uns32 openFlags, XMP_Uns32 format, XMP_Uns32 handlerFlags );
#if WIN_ENV	
	~PDF_MetaHandler() NO_EXCEPT_FALSE;
#else
	~PDF_MetaHandler();
#endif
	virtual bool getFileModDate ( XMP_DateTime * modDate );
	virtual void cacheFileData( const IOAdapter& file, std::string& xmpStr );
	virtual void updateFile( const IOAdapter& file, bool doSafeUpdate, const std::string& xmpStr );
	
	static bool initialize();
	static bool terminate();
	static bool checkFileFormat( const std::string& filePath, const IOAdapter& file );
	
	// It's a dummy function. The design doesn't look good. Plugin need to implement both checkFileFormat and 
	// checkFolderFormat, and one of them should be just a dummy function.
	static inline bool checkFolderFormat( const std::string& rootPath, const std::string& gpName, const std::string& parentName, const std::string& leafName ) { return false; }

	bool openPDF( long inPermission );
	void savePDF();
	void closePDF();

private:
	PDDoc			mPDDoc;
	CosDoc			mCosDoc;
	std::string     xmpPacket;
};


/*****************************************************************************
 PDF_MetaHandler specific functions
*****************************************************************************/

PDF_MetaHandler::PDF_MetaHandler( const std::string& filePath, XMP_Uns32 openFlags, XMP_Uns32 format, XMP_Uns32 handlerFlags ): 
	PluginBase( filePath, openFlags, format, handlerFlags ), 
	mPDDoc( NULL ), 
	mCosDoc( NULL )
{
	FunctionCalls calls = 0;
	XMP_Bool success =  ThreadLocalStorage::GetValue ( calls );
	if ( !success )
	{
		throw XMP_Error ( kXMPErr_PluginInternal, "PDFHandler Thread Local Storage problem" );
	}
	if ( calls == 0 ) {
		int result = MyPDFLInit();
		if ( result != 0 )
		{
			throw XMP_Error ( kXMPErr_PluginInternal, "MyPDFLInit Failed" );
		}
	}

	calls++;
	success = ThreadLocalStorage::SetValue ( calls );
	if ( !success )
	{
		throw XMP_Error ( kXMPErr_PluginInternal, "PDFHandler Thread Local Storage problem" );
	}
}

#if WIN_ENV	
PDF_MetaHandler::~PDF_MetaHandler() NO_EXCEPT_FALSE
#else
PDF_MetaHandler::~PDF_MetaHandler()
#endif
{
	this->closePDF();
	FunctionCalls calls = 0; 
	XMP_Bool success = ThreadLocalStorage::GetValue(calls);
	if ( ! success )
	{
		throw XMP_Error ( kXMPErr_PluginInternal, "PDFHandler Thread Local Storage problem" );
	}
	if ( calls ) calls--;
	success = ThreadLocalStorage::SetValue ( calls );
	if ( ! success )
	{
		throw XMP_Error ( kXMPErr_PluginInternal, "PDFHandler Thread Local Storage problem" );
	}
	if (calls == 0)
		MyPDFLTerm();
}
bool PDF_MetaHandler::getFileModDate ( XMP_DateTime * modDate )
{
	const std::string & filePath = this->getPath();
	if ( filePath.empty() ) return false;
	return Host_IO::GetModifyDate ( filePath.c_str(), modDate );
}

void PDF_MetaHandler::cacheFileData( const IOAdapter& file, std::string& xmpStr )
{
	this->checkAbort( true );
	
	if( this->openPDF( 0 ) )
    {
        CosObj xapStream = CosDocGetXAPStream(this->mCosDoc);
        if( CosObjGetType(xapStream) != CosNull )
		{	
			CosStreamGetString(xapStream, xmpStr);
		}
	}

	// Reconcile non-XMP metadata
	if( this->mCosDoc )
	{
		SXMPMeta xmp;
		xmp.ParseFromBuffer(xmpStr.c_str(), static_cast<XMP_StringLen>(xmpStr.length()), kXMP_NoOptions);

		SetupMetaXAP( &xmp );
		UpdateMetaXAPFromDocInfo( this->mCosDoc, &xmp );
		xmp.SerializeToBuffer(&xmpStr, kXMP_NoOptions, 0);
	}
}


void PDF_MetaHandler::updateFile( const IOAdapter& file, bool doSafeUpdate, const std::string& xmpStr )
{
	this->checkAbort( true );
	
	if( !mCosDoc || PDDocPermRequest(mPDDoc, PDPermReqObjDoc, PDPermReqOprModify, 0) )
	{
		throw XMP_Error( kXMPErr_PluginInternal, "File was not opened" );
	}
	
	// Reconcile non-XMP metadata
	if( this->mCosDoc )
	{
		SXMPMeta xmp;
		xmp.ParseFromBuffer(xmpStr.c_str(),static_cast<XMP_StringLen>(xmpStr.length()), kXMP_NoOptions);

		CosObj infoDict = CosDocGetInfoDict( this->mCosDoc );
		if( CosObjGetType(infoDict) != CosNull )
		{
			ClearDocInfo( this->mCosDoc );
			CosDocUpdateDocInfoFromXAP( this->mCosDoc, xmp );
		}

		//To make PDF PDF-A compliance 
		xmp.SerializeToBuffer( &this->xmpPacket, 0, 2048, "\n", NULL, 0  );
	}

	// To make PDF PDF-A compliance
	//CosDocSerializeMetaXAPToStream(this->mCosDoc, xmpStr);
	CosDocSerializeMetaXAPToStream(this->mCosDoc, this->xmpPacket);
	this->savePDF();
}

static FunctionCalls init = 0;

bool PDF_MetaHandler::initialize()
{
	init++;

	if(init != 1) return true;

#if XMP_StaticBuild
    SXMPMeta::Initialize();
#endif
	XMP_Bool success = ThreadLocalStorage::Init();

	if ( ! success )
	{
		fprintf ( stderr, "PDFHandler Thread Local Storage problem" );
		return false;
	}

	int err = MyPDFLInit();	/* initialize the PDFLib */
	if (err != 0)			/* check for error after initialization */
	{
		fprintf(stderr, "Initialization error\n");
		return false;
	}
	success = ThreadLocalStorage::SetValue ( init );
	if ( ! success )
	{
		fprintf ( stderr, "PDFHandler Thread Local Storage problem" );
		return false;
	}
	return true;
}

bool PDF_MetaHandler::terminate()
{
	init--;

	if(init != 0) return true;
#if XMP_StaticBuild
    SXMPMeta::Terminate();
#endif
	MyPDFLTerm();	/* terminate the PDFLib */
	XMP_Bool success = ThreadLocalStorage::Term();
	if ( ! success )
	{
		fprintf ( stderr, "PDFHandler Thread Local Storage problem" );
		return false;
	}
	return true;
}

bool PDF_MetaHandler::checkFileFormat( const std::string& filePath, const IOAdapter& file )
{
	bool result = false;
	FILE* pFile = fopen( filePath.c_str(), "r" );
	if( pFile != NULL )
	{
		char buffer[5];
		fgets(buffer, 5, pFile);
		fclose( pFile );
		if( strcmp(buffer, "%PDF") == 0 )
			result = true;
	}
	return result;
}

bool PDF_MetaHandler::openPDF( long inPermission )
{
	this->checkAbort( true );
	
	if( !mCosDoc )
	{
		ASPathName path = NULL;
		ASBool fileOpenSuccess = false;
		DURING
			path = ASFileSysCreatePathFromDIPath( NULL, this->getPath().c_str() , NULL );
			
			//Check file permission.
			ASFileMode pdfOpenFlag = ASFILE_READ;
			if( this->getOpenFlags() & kXMPFiles_OpenForUpdate )
				pdfOpenFlag |= ASFILE_WRITE;
			
			ASFile file = NULL;
			ASFileSysOpenFile64( NULL, path, pdfOpenFlag, &file);
			
			if( file )
			{
				fileOpenSuccess = true;
				ASFileClose( file );
				mPDDoc = PDDocOpen( path, NULL, 0, 0 );
				mCosDoc = PDDocGetCosDoc( mPDDoc );
			}
		HANDLER
		END_HANDLER
		
		ASFileSysReleasePath( NULL, path);
		if( !fileOpenSuccess )
		{
			throw XMP_Error( kXMPErr_PluginInternal, "File could not be opened in desire mode." );
		}
	}
	
	return (mCosDoc != NULL);
}

static ASBool Static_CancelProc(void *clientData)
{
	return CheckAbort( clientData );
}

void PDF_MetaHandler::savePDF()
{
	this->checkAbort( true );
	
	if( mCosDoc )
	{
		DURING
			PDDocSaveParamsRec	params;			
			memset( &params, 0, sizeof(PDDocSaveParamsRec) );
			params.size = sizeof( PDDocSaveParamsRec );
			
			if( this->getOpenFlags() & kXMPFiles_PreservePDFState )
			{
				//Check whether the PDF was previously linearized or not
				ASInt32	flags = PDDocGetFlags( mPDDoc );
				
				if( flags & PDDocIsLinearized )
					params.saveFlags = PDSaveFull | PDSaveLinearized | PDSaveCollectGarbage;
				else
					params.saveFlags = PDSaveFull | PDSaveCollectGarbage;
			}
			else
			{
				params.saveFlags = PDSaveFull | PDSaveLinearized | PDSaveCollectGarbage;
			}
			
			//
			// Pass cancelProc
			//
			params.cancelProc = Static_CancelProc;
			params.cancelProcClientData = this;

			PDDocGetVersion( mPDDoc, &params.major, &params.minor );
			PDDocSaveWithParams( mPDDoc, &params );
		HANDLER
			throw XMP_Error( kXMPErr_PluginInternal, "File could not be saved" );
		END_HANDLER
	}
}


void PDF_MetaHandler::closePDF()
{
	if( mCosDoc )
	{
		DURING
			PDDocClose( mPDDoc );
		HANDLER
			throw XMP_Error( kXMPErr_PluginInternal, "File could not be closed" );
		END_HANDLER
		
		mPDDoc = NULL;
		mCosDoc = NULL;
	}
}

/*****************************************************************************
	Static functions implementation
*****************************************************************************/

#define Metadata_K            ASAtomFromString("Metadata")
#define Type_K                ASAtomFromString("Type") 
#define Subtype_K             ASAtomFromString("Subtype") 
#define Trapped_K             ASAtomFromString("Trapped") 

static CosObj CosDocGetXAPStream(CosDoc cosDoc)
{
    CosObj catalogDict = CosDocGetRoot((cosDoc));
    if(CosObjGetType(catalogDict) != CosDict)
		throw XMP_Error( kXMPErr_PluginInternal, "Invalid cos object type" );
    CosObj xapStream = CosDictGet(catalogDict, Metadata_K);
    switch (CosObjGetType(xapStream)) {
    case CosNull: // Fall through.
    case CosStream:
        return xapStream;
    default:
		throw XMP_Error( kXMPErr_PluginInternal, "Invalid cos object" );
        return xapStream;
    }
}

static void CosStreamGetString(CosObj cosStream, std::string & result)
{
	const size_t bufsiz = 1024;
	char buffer[bufsiz];
	ASStm asStm = NULL;
	DURING
	asStm = CosStreamOpenStm(cosStream, cosOpenFiltered);
	ASUns32 nItemsRead = 0;
		do {
			nItemsRead = ASStmRead(buffer,
								   1,
								   bufsiz,
								   asStm);
			result.append(buffer, nItemsRead);
		} while( nItemsRead == bufsiz );
	HANDLER
	END_HANDLER
	if(asStm != NULL)
		ASStmClose(asStm);
}

static char* GetPDStringFromUTF8( std::string& text, ASInt32& textNBytes )
{
	char * newText = NULL;
	textNBytes  = 0;

    const ASUns16 * bogusWideBuffer = reinterpret_cast<const ASUns16 *>(text.c_str());
	ASText asText = ASTextFromUnicode(const_cast<ASUns16 *>(bogusWideBuffer), kUTF8);
	if( asText )
	{
		newText = ASTextGetPDTextCopy(asText, &textNBytes);
		ASTextDestroy(asText);
	}
	return newText;
}

static bool IsTrappedProperty(ASAtom infoKey)
{
	return infoKey == ASAtomFromString("Trapped");
}

void SetTextPropertyInInfo(CosDoc cosDoc, ASAtom key, std::string & value);
void SetTextPropertyInInfo(CosDoc cosDoc, ASAtom key, std::string & value)
{
	CosObj cosValue;

	if( IsTrappedProperty(key) )
	{
		ASAtom valueAtom = ASAtomFromString( value.c_str() );
		cosValue = CosNewName(cosDoc, false, valueAtom);
	}
	else
	{
		ASInt32 textNBytes;
		char* ptr = GetPDStringFromUTF8( value, textNBytes );
		cosValue = CosNewString(cosDoc, false, ptr , textNBytes ); // casting won't be needed in A10 version. leaving for now
		ASfree( ptr );
	}

	CosObj infoDict = CosDocGetInfoDict(cosDoc);
	CosDictPut(infoDict, key, cosValue);
}

static void CosDocSerializeMetaXAPToStream(CosDoc cosDoc, const std::string& xmpStr )
{
	CosObj catalog = CosDocGetRoot(cosDoc);

	ASStm asStm = ASMemStmRdOpen( xmpStr.c_str(), (ASUns32) xmpStr.size() ); // TODO : remove castness 
	CosObj attributesDict = CosNewDict(cosDoc, false, 3);
	CosDictPut(attributesDict, Type_K, CosNewName(cosDoc, false, Metadata_K));
	CosDictPut(attributesDict, Subtype_K, CosNewName(cosDoc, false, ASAtomFromString("XML")));
	CosObj streamObj = CosNewStream(cosDoc,
						     true,
						     asStm,
						     0,
						     true,
						     attributesDict,
						     CosNewNull(),
						     (ASUns32) xmpStr.size() );
	ASStmClose(asStm);
	CosDictPut(catalog, Metadata_K, streamObj);
}


void GetUTF8FromFromPDString( const std::string& value, std::string& utf8Value );
void GetUTF8FromFromPDString( const std::string& value, std::string& utf8Value )
{
	// Have to convert to whatever character encoding xaptk accepts.
    ASText asText = ASTextFromSizedPDText(value.data(), (ASUns32) value.size());
    char* utf8 = reinterpret_cast<char*>( ASTextGetUnicodeCopy(asText, kUTF8) );
    utf8Value.assign( utf8 );
	ASfree(utf8);
    ASTextDestroy(asText);
}

static ASBool UpdateMetaXAPFromDocInfoEnumProc(CosObj name, CosObj value, void * clientData)
{
	if( CosObjGetType(value) == CosNull )
		return true;

    SXMPMeta * metaXAP = reinterpret_cast<SXMPMeta *>(clientData);

    const ASAtom nameAtom = CosNameValue(name);
    const char * nameString = ASAtomGetString(nameAtom);
    switch( CosObjGetType(value) ) 
	{
    case CosString:
        {
            ASInt32 nbytes;
            char * stringData = CosCopyStringValue(value, &nbytes);
            std::string valueString(stringData, nbytes);
            ASfree(stringData);
            // The normal case: it's a string containing PDF text.  If
            // it's a known item, put its value in the SXMPMeta under the
            // PDF schema; otherwise, put in in the PDFX schema.
            // Change encoding to UTF-8 first.
            SetInfoItemInXAP( metaXAP, nameString, valueString );
        }
    break;
        
    case CosName:
        // It's a name, so we had better be dealing with the /Trapped key.
        // Put its value in the SXMPMeta under the PDF schema.
        if (nameAtom == Trapped_K) 
		{
			ASAtom valueAtom = CosNameValue(value);
			const char * valueString = ASAtomGetString(valueAtom);
			SetInfoItemInXAP(metaXAP, ASAtomGetString(Trapped_K), valueString);
		}
        break;
    default:		
        break;
    }
    return true;
}

static ASBool DocInfoIsNewerThanXAP( CosObj docInfoDict, SXMPMeta * metaXAP )
{
	ASBool result = false;

	if( CosObjGetType(docInfoDict) != CosNull )
	{
		XMP_DateTime xmpUpdatedTime;
		XMP_OptionBits features;
		if( metaXAP->GetProperty_Date(kXMP_NS_XMP, "MetadataDate", &xmpUpdatedTime, &features) )
		{
			CosObj value = CosDictGet( docInfoDict, ASAtomFromString("ModDate") );
			if( CosObjGetType(value) != CosNull )
			{
				XMP_DateTime docInfoModTime;
				ASInt32 nbytes;
				char * stringData = CosCopyStringValue(value, &nbytes);
				if( PDFDateToXAPDateTime( stringData, docInfoModTime ) )
				{
					// if docInfoModTime is later than xmpUpdatedTime then docInfo entry will
					// overwrite the xmp entries.
					if( SXMPUtils::CompareDateTime( docInfoModTime, xmpUpdatedTime ) > 0 )
					{
						result = true;
					}
				}
			}
		}
		else
		{
			// XAP might not have xap:MetadataDate.  If it doesn't, assume DocInfo is newer.
			result = true;
		}
	}

	return result;
}


static void UpdateMetaXAPFromDocInfo(CosDoc cosDoc, SXMPMeta * metaXAP)
{
	DURING		
		CosObj docInfoDict = CosDocGetInfoDict(cosDoc);
		if (CosObjGetType(docInfoDict) != CosNull) 
		{
			CosObj xapStream = CosDocGetXAPStream(cosDoc);
        	if( CosObjGetType(xapStream) == CosNull || DocInfoIsNewerThanXAP(docInfoDict, metaXAP) )
			{
				CosObjEnum(docInfoDict, UpdateMetaXAPFromDocInfoEnumProc, metaXAP); /* can raise */
			}
		}
	HANDLER
		throw XMP_Error( kXMPErr_PluginInternal, "MetaData could not be imported from DocInfoi" );
	END_HANDLER
}

static ASBool ClearDocInfoEnumProc(CosObj name, CosObj value, void * clientData)
{
	if( CosObjGetType(name) != CosNull )
	{
		CosObj * docInfo = reinterpret_cast<CosObj *>(clientData);
		const ASAtom nameAtom = CosNameValue(name);
		CosDictRemove( *docInfo, nameAtom);
	}
	return true;
}

static void ClearDocInfo( CosDoc cosDoc )
{
	DURING
		CosObj docInfoDict = CosDocGetInfoDict(cosDoc);
		if( CosObjGetType(docInfoDict) == CosDict ) 
		{
			CosObjEnum( docInfoDict, ClearDocInfoEnumProc, &docInfoDict ); /* can raise */
		}
	HANDLER
		throw XMP_Error( kXMPErr_PluginInternal, "DocInfo could not be cleared" );
	END_HANDLER
}

} //namespace PDF_Handler

/*****************************************************************************
 PDF_MetaHandler Register
*****************************************************************************/
namespace XMP_PLUGIN
{

const char* GetModuleIdentifier()
{
	static const char* kModuleIdentifier = "com.adobe.XMP.plugins.MiniPDFL";
	return kModuleIdentifier;
}

bool SetupPlugin()
{
	return true;
}

void RegisterFileHandlers()
{
	//Register all the handlers provided by the plugin.
	const char* sXMPPDFL = "adobe.coretech.xmp.minipdfl";
	PluginRegistry::registerHandler(
		new PluginCreator<PDF_Handler::PDF_MetaHandler>(sXMPPDFL) );
}

} //namespace XMP_PLUGIN
