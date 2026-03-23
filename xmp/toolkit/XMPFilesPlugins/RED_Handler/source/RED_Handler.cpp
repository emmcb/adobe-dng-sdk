// =================================================================================================
// Copyright Adobe
// Copyright 2012 Adobe
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
//	IJS Inder Jeet Singh
//	HK  Honey Kansal
//	JKR Jens Krueger
//	ADC Amandeep Chawla
//
// mm-dd-yy who Description of changes, most recent on top
//
// 04-26-13 ADC 5.6-f054 [3526891] Read-only checks in IsMetadataWritable() API try creating temporary files if no metadata exists, instead of checking for file permissions.
//						 [3525961] IsMetaDataWritable checks for Plugins and embedded handlers are different.
// 04-04-13 IJS 5.6-f051 Move common I/O methods to IOUtils.
//						 [3534631] Optimize GetAssociated Resources for RED_Handler
// 03-25-13 IJS Update the R3D SDK to the latest version that includes GetRMDPath API.
//              Use the GetRMDPath API to get the RMD Path inside RED_Handler. 
// 03-05-13 IJS Fixed Multi-threaded crash on Mac.
// 02-18-13 JKR 5.6-f037 New plugin entry point InitializePlugin2() which call SetupPlugin().
// 02-05-13 HK  Fixed bugs#3477099, #3495118, #3490341, #3492358.
// 01-31-13 IJS [3493515] Modified IsMetadataWritable to detect the permission error.Rework on review comments.
// 01-29-13 IJS Implemented  GetAssociatedResources and IsMetadataWritable for RED Format
// 11-06-12 IJS [3360627] Return .RMD file path in FillMetadataFiles even if the file doesn't exist
// 11-06-12 IJS Added implementation for FillMetadata.
// 11-01-12 IJS First draft of RED Plug-in handler.
//
// =================================================================================================
#endif // AdobePrivate

#include "XMPFilesPlugins/api/source/PluginBase.h"
#include "XMPFilesPlugins/api/source/PluginRegistry.h"
#include "XMPFilesPlugins/api/source/HostAPIAccess.h"

#include "source/XMPFiles_IO.hpp"
#include "source/XIO.hpp"
#include "source/IOUtils.hpp"
#include <string>

#if XMP_MacBuild
    #include <mach-o/dyld.h>
    #include <sys/syslimits.h>
#endif

//	R3DSDK Includes
#include "R3DSDK.h"

namespace XMP_PLUGIN
{

#define WriteGuard(x) { XMP_AutoLock guard ( &clipOpenCloseLock, kXMP_WriteLock ); x; }
#define ReadGuard(x) { XMP_AutoLock guard ( &clipOpenCloseLock, kXMP_ReadLock ); x; }


	class RED_MetaHandler : public PluginBase
	{
	public:
		RED_MetaHandler( const std::string& filePath, XMP_Uns32 openFlags, XMP_Uns32 format, XMP_Uns32 handlerFlags )
			: PluginBase( filePath, openFlags, format, handlerFlags ) {}
		~RED_MetaHandler() {}
		//Functions implemented by RED handler
		virtual void cacheFileData( const IOAdapter& file, std::string& xmpStr );
		virtual bool getFileModDate ( XMP_DateTime * modDate );
		virtual void FillMetadataFiles ( std::vector<std::string> * metadataFiles );
		virtual void updateFile( const IOAdapter& file, bool doSafeUpdate, const std::string& xmpStr );
		virtual void FillAssociatedResources ( std::vector<std::string> * resourceList );
		bool IsMetadataWritable ( ) ;


		//Static functions 

		//@return true on success otherwise false.
		static bool initialize();

		//This function is called to terminate the file handler.
		//@return true on success otherwise false.
		static bool terminate();

		//@return true on success otherwise false.
		static bool checkFileFormat( const std::string& filePath, const IOAdapter& file );

		// This function is not needed by a normal handler but an "owning" handler needs to implement it.
		static inline bool checkFolderFormat(	const std::string& rootPath, 
			const std::string& gpName, 
			const std::string& parentName, 
			const std::string& leafName ) { return false; }
		// Creating a ThreadSafe version of the R3DSDK::Clip
		// R3DSDK requires the R3DSDK developer to synchronize calls to LoadFrom() or Close()
		// ThreadSafeClip class makes sure that all the calls to close() and LoadFrom() are
		// synchronized.
		class ThreadSafeClip : public R3DSDK::Clip
		{
		public:
			ThreadSafeClip( ){ }
			ThreadSafeClip(const char * pathToFile){ this->LoadFrom(pathToFile); }
			R3DSDK::LoadStatus LoadFrom(const char * pathToFile){ WriteGuard( return R3DSDK::Clip::LoadFrom(pathToFile ) ) }
			void Close( ) { WriteGuard( R3DSDK::Clip::Close( ) ) }
			R3DSDK::LoadStatus Status() const { ReadGuard( return R3DSDK::Clip::Status( ) ) }
			bool CreateOrUpdateRmd(const std::string xmpInput) const { ReadGuard( return R3DSDK::Clip::CreateOrUpdateRmd( xmpInput ) ) }
			bool GetRmdXmp(std::string & xmpOutput) const { ReadGuard( return R3DSDK::Clip::GetRmdXmp( xmpOutput ) ) }
			const char* GetRmdPath() const { ReadGuard( return R3DSDK::Clip::GetRmdPath() ) }
			~ThreadSafeClip(){ this->Close(); }
		private:
			static XMP_ReadWriteLock clipOpenCloseLock;
		};

	private:
		bool GetRmdPath(std::string& rmdFilePath);
		ThreadSafeClip clip;
		static int init;
		static const std::string XPACKET_HEADER;
		static const std::string XMPMETA_HEADER;
	};

	XMP_ReadWriteLock RED_MetaHandler::ThreadSafeClip::clipOpenCloseLock;
	int RED_MetaHandler::init = 0;
	const std::string RED_MetaHandler::XPACKET_HEADER = "<?xpacket";
	const std::string RED_MetaHandler::XMPMETA_HEADER = "<x:xmpmeta";

	//============================================================================
	// registerHandler RED_MetaHandler
	//============================================================================
	const char* GetModuleIdentifier()
	{
		static const char* kModuleIdentifier = "com.adobe.xmp.plugins.RED";
		return kModuleIdentifier;
	}

	bool SetupPlugin()
	{
		return true;
	}

	void RegisterFileHandlers()
	{
		//UID of the file handler. It's different from the module identifier.
		const char* sXMPTemplate = "com.adobe.xmp.plugins.RED.handler"; 
		PluginRegistry::registerHandler( new PluginCreator<RED_MetaHandler>(sXMPTemplate) );
	}

	//============================================================================
	// RED_MetaHandler specific functions
	//============================================================================

	bool RED_MetaHandler::initialize()
	{
		init++;
		
		#if XMP_MacBuild
            char path[PATH_MAX];
			memset ( path, 0x00, sizeof ( path ) );
            uint32_t size = sizeof(path);
            if (_NSGetExecutablePath(path, &size) == 0)
            {
                char * ch = strrchr(path,'/');
                if(ch != NULL)
                    memset(ch,0x00, size - (ch-path));
                if ( sizeof(path) > strlen(path) + strlen( "/../Frameworks/" ) )
                    strcat(path ,"/../Frameworks/");
            }
            R3DSDK::InitializeStatus r3dSDKStatus = R3DSDK::InitializeSdk(path , 0);
        #else
            R3DSDK::InitializeStatus r3dSDKStatus = R3DSDK::InitializeSdk("./", 0);
        #endif
		
		if (r3dSDKStatus != R3DSDK::InitializeStatus::ISInitializeOK) return false;
		if(init != 1) return true;
		return true;
	}


	bool RED_MetaHandler::terminate()
	{
		init--;
		R3DSDK::FinalizeSdk();
		if(init != 0) return true;

		return true;
	}

	// =================================================================================================
	// RED_MetaHandler::checkFileFormat
	// ==================================
	//
	//  Method to determine whether the filePath is a R3D file or not
	bool RED_MetaHandler::checkFileFormat( const std::string& filePath, const IOAdapter& file )
	{
		try 
		{
			// Constructor loads clip and sets the class status to indicate success or failure.
			// Clean up is taken care of in the destructor
			ThreadSafeClip clip( filePath.c_str() );

			// check to see if the R3D file was loaded successfully
			if ( clip.Status() != R3DSDK::LSClipLoaded )
			{
				return false;
			}
			return true;
		} 
		catch ( ... ) 
		{
			//No cleanup as the Destructor of Clip takes care
		}
		return false;
	}

	// =================================================================================================
	// RED_MetaHandler::cacheFileData
	// ==================================
	//
	//  Implements R3D metadata caching, if present
	void RED_MetaHandler::cacheFileData( const IOAdapter& file, std::string& xmpStr )
	{
		xmpStr.erase();
		bool readOnly = ( ! ( this->getOpenFlags() & kXMPFiles_OpenForUpdate ) );

		clip.LoadFrom( this->getPath().c_str() ) ;

		if (clip.Status() != R3DSDK::LSClipLoaded)
		{
			return;
		}
		if ( ! clip.GetRmdXmp(xmpStr) ) return ;
		if ( readOnly )
		{
			clip.Close();
			XMP_Assert( R3DSDK::LSClipIsEmpty==clip.Status() || R3DSDK::LSNoClipOpen==clip.Status() );
		}
	}

	static inline bool operator< ( const XMP_DateTime & left, const XMP_DateTime & right ) {
		int compare = SXMPUtils::CompareDateTime ( left, right );
		return (compare < 0);
	}

	// =================================================================================================
	// RED_MetaHandler::getFileModDate
	// ==================================
	//
	//  Method to get the modification time of the metadata file
	//  In this case between the R3D raw and sidecar XMP, this is the more 
	//  recent of both files.
	bool RED_MetaHandler::getFileModDate ( XMP_DateTime * modDate )
	{
		std::string rootPath =  this->getPath();
		std::string r3dFilePath = rootPath;
		XMP_DateTime oneDate, junkDate;
		std::string fileExt;
		std::string leafName;
		std::string rmdSidecarPath;
		XIO::SplitLeafName ( &rootPath, &leafName );
		XIO::SplitFileExtension(&leafName,&fileExt);
		if ( modDate == 0 ) modDate = &junkDate;
		bool ok,haveDate=false ;
		ok = Host_IO::Exists ( r3dFilePath.c_str() );
		if ( ok ) ok = Host_IO::GetModifyDate ( r3dFilePath.c_str(), &oneDate );
		if ( ok ) 
		{
			*modDate = oneDate;
			haveDate = true;
		}
		if ( GetRmdPath( rmdSidecarPath ) )
		{
			ok = Host_IO::Exists ( rmdSidecarPath.c_str() );
			if ( ok ) ok = Host_IO::GetModifyDate ( rmdSidecarPath.c_str(), &oneDate );
			if ( ok ) 
			{
				if ( (! haveDate) || (*modDate < oneDate) ) *modDate = oneDate;
				haveDate = true;
			}
		}
		return haveDate;
	}

	inline static  bool IsDigit( char c )
	{
		return c >= '0' && c <= '9';
	}

	// =================================================================================================
	// RED_MetaHandler::FillMetadataFiles
	// ====================================

	void RED_MetaHandler::FillMetadataFiles ( std::vector<std::string> * metadataFiles )
	{
		std::string rmdFilePathStr;	
		if ( GetRmdPath( rmdFilePathStr ) )
		{
			metadataFiles->push_back ( rmdFilePathStr );
		}
	}	// RED_MetaHandler::FillMetadataFiles



	static inline void MakeLowerCase ( std::string * str )
	{
		for ( size_t i = 0, limit = str->size(); i < limit; ++i ) {
			char ch = (*str)[i];
			if ( ('A' <= ch) && (ch <= 'Z') ) (*str)[i] += 0x20;
		}
	}

	inline static void AddResourceIfExists( std::vector<std::string> * resourceList ,
		std::string filenameNoExt ,std::string  ext )
	{
		std::string filePath = filenameNoExt + ext ;
		if( Host_IO::Exists( filePath.c_str() ) )
		{
			resourceList->push_back(filePath);
		}
		else
		{
			MakeLowerCase ( &ext );
			filePath = filenameNoExt + ext ;
			if ( Host_IO::Exists( filePath.c_str() ) )
				resourceList->push_back( filePath );
		}
	}

	// =================================================================================================
	// RED_MetaHandler::GetRmdPath
	// =========================================
	bool RED_MetaHandler::GetRmdPath( std::string& rmdFilePathStr )
	{
		const char* rmdFilePath;
		if (clip.Status() != R3DSDK::LSClipLoaded)
		{
			clip.LoadFrom( this->getPath().c_str() ) ;
		}
		rmdFilePath=clip.GetRmdPath();
		if ( rmdFilePath )
		{
			rmdFilePathStr = rmdFilePath ;
			return true;
		}
		return false;
	}// RED_MetaHandler::GetRmdPath

	// =================================================================================================
	// RED_MetaHandler::FillAssociatedResources
	// =========================================

	void RED_MetaHandler::FillAssociatedResources ( std::vector<std::string> * resourceList )
	{
		std::string redFilePath = this->getPath();

		bool atLeastOneFileAdded = false;
		std::string fileExt, rmdFilePathStr ;	
		std::string redClipName, filenameNoExt, ext;
		XIO::SplitFileExtension ( &redFilePath, &fileExt );
		XIO::SplitLeafName ( &redFilePath, &redClipName );

		size_t clipNameLen = redClipName.length() ;
		if ( clipNameLen > 4 && redClipName[clipNameLen - 4] == '_' 
			&& IsDigit(redClipName[clipNameLen - 3])
			&& IsDigit(redClipName[clipNameLen - 2])
			&& IsDigit(redClipName[clipNameLen - 1]) )
		{   // probably the clip is spanned
			// change the red clip name
			redClipName.erase( clipNameLen - 4 ) ;
			// Find the other R3D files in the same folder

			XMP_StringVector regExp;
			regExp.push_back( "^" + redClipName + "_\\d\\d\\d.R3D$");
			regExp.push_back( "^" + redClipName + "_\\d\\d\\d.r3d$");
			regExp.push_back( "^" + redClipName + "_\\d\\d\\d.R3d$");
			regExp.push_back( "^" + redClipName + "_\\d\\d\\d.r3D$");
			IOUtils::GetMatchingChildren ( *resourceList, redFilePath, regExp, false, true, true );

		}
		else
		{
			resourceList->push_back ( this->getPath() );
		}

		// assuming the RMD ext to be either uppercase or lowercase 
		// if either of them exists return it otherwise return the 
		// path with an uppercase extension 
		if ( GetRmdPath( rmdFilePathStr ) )
		{
			ext = "";
			AddResourceIfExists( resourceList ,rmdFilePathStr ,ext );
		}

		//Add .RSX file
		filenameNoExt = redFilePath + kDirChar + redClipName ;
		ext = ".RSX";
		AddResourceIfExists( resourceList ,filenameNoExt ,ext );

		//Add .mov files
		filenameNoExt = redFilePath + kDirChar + redClipName + "_F" ;
		ext = ".MOV";
		AddResourceIfExists( resourceList ,filenameNoExt ,ext );

		filenameNoExt = redFilePath + kDirChar + redClipName + "_H" ; 
		AddResourceIfExists( resourceList ,filenameNoExt ,ext );

		filenameNoExt = redFilePath + kDirChar + redClipName + "_M" ;
		AddResourceIfExists( resourceList ,filenameNoExt ,ext );

		filenameNoExt = redFilePath + kDirChar + redClipName + "_P" ;
		AddResourceIfExists( resourceList ,filenameNoExt ,ext );

	}	// RED_MetaHandler::FillAssociatedResources


	bool RED_MetaHandler::IsMetadataWritable ( )
	{
		std::vector<std::string> metadataFiles;
		FillMetadataFiles(&metadataFiles);
		//only check for the RMD file as we only write the XMP to this file
		return Host_IO::Writable( metadataFiles[0].c_str(), true );
		
	}// RED_MetaHandler::IsMetadataWritable

	// =================================================================================================
	// RED_MetaHandler::UpdateFile
	// ==================================
	//
	//  Method to write the file with updated XMP metadata 
	void RED_MetaHandler::updateFile( const IOAdapter& file, bool doSafeUpdate, const std::string& xmpStr )
	{
		if ( clip.Status() != R3DSDK::LSClipLoaded )
		{
			throw XMP_Error( kXMPErr_PluginInternal, "R3D File was not opened" );
		}
		if ( xmpStr.empty() ) return ;
		if( ! clip.CreateOrUpdateRmd(xmpStr) )
		{
			throw XMP_Error( kXMPErr_PluginInternal, "Unable to update XMP in a R3D File" );
		}
	}

} //namespace XMP_PLUGIN