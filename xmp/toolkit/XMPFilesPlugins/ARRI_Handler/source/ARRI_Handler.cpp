// =================================================================================================
// Copyright Adobe
// Copyright 2018 Adobe
// All Rights Reserved
//
// NOTICE: Adobe permits you to use, modify, and distribute this file in accordance with the terms
// of the Adobe license agreement accompanying it. If you have received this file from a source other 
// than Adobe, then your use, modification, or distribution of it requires the prior written permission
// of Adobe.
// =================================================================================================
#include "XMPFilesPlugins/api/source/PluginBase.h"
#include "XMPFilesPlugins/api/source/PluginRegistry.h"
#include "XMPFilesPlugins/api/source/HostAPIAccess.h"

#include "XMPMetadataMapper.h"
#include "ArriMetadataContainer.h"

#include "source/XMPFiles_IO.hpp"
#include "source/XIO.hpp"
#include "source/IOUtils.hpp"

typedef Amc::ArriMetadataContainer::ColorSpace ColorSpace;
#define ARRI_SDK_VERSION 3

namespace XMP_PLUGIN
{
    class ARRI_MetaHandler : public PluginBase
    {
        Amc::ArriMetadataContainer arriMetadataContainer;
        public:
        
        ARRI_MetaHandler( const std::string& filePath, XMP_Uns32 openFlags, XMP_Uns32 format, XMP_Uns32 handlerFlags )
        : PluginBase( filePath, openFlags, format, handlerFlags ) {}
        ~ARRI_MetaHandler() {}
        
        
        //Functions implemented by ARRI handler
        virtual void cacheFileData( const IOAdapter& file, std::string& xmpStr );
        virtual bool getFileModDate ( XMP_DateTime * modDate );
        virtual void updateFile( const IOAdapter& file, bool doSafeUpdate, const std::string& xmpStr );
        //virtual void FillAssociatedResources ( std::vector<std::string> * resourceList );
        bool IsMetadataWritable ( );
        
        //Static functions
        
        static bool initialize();
        static bool terminate();
        
        //@return true on success otherwise false.
        static bool checkFileFormat( const std::string& filePath, const IOAdapter& file );
        
        // This function is not needed by a normal handler but an "owning" handler needs to implement it.
        static inline bool checkFolderFormat(	const std::string& rootPath,
                                             const std::string& gpName,
                                             const std::string& parentName, 
                                             const std::string& leafName ) { return false; }

		
    private:
        static int init;
        ColorSpace getColorSpace(XMP_OptionBits optionFlags);
        XMP_Uns32 getbitsPerComponent(XMP_OptionBits optionFlags);
		static const XMP_Int64 ArriFileHeader_SIZE;
    };
    
    int ARRI_MetaHandler::init = 0;
	XMP_Int64 const ARRI_MetaHandler::ArriFileHeader_SIZE = sizeof(ArriRawMeta::ArriFileHeaderV3);

    //@return true on success otherwise false.
    bool ARRI_MetaHandler::initialize(){
        init++;
        if(init != 1)
            return true;
        return SXMPMeta::Initialize();
        
    }
    
    //This function is called to terminate the file handler.
    //@return true on success otherwise false.
    bool ARRI_MetaHandler::terminate(){
        init--;
        if(init != 0) return true;
        SXMPMeta::Terminate();
        return true;
    }
    
    //============================================================================
    // registerHandler ARRI_MetaHandler
    //============================================================================
    void RegisterFileHandlers()
    {
        //UID of the file handler. It's different from the module identifier.
        const char* sXMPTemplate = "com.adobe.xmp.plugins.ARRI.handler";
        PluginRegistry::registerHandler( new PluginCreator<ARRI_MetaHandler>(sXMPTemplate) );
    }
    
    const char* GetModuleIdentifier()
    {
        static const char* kModuleIdentifier = "com.adobe.xmp.plugins.ARRI";
        return kModuleIdentifier;
    }
    
    bool SetupPlugin()
    {
        return true;
    }
    
    bool ARRI_MetaHandler::checkFileFormat( const std::string& filePath, const IOAdapter& file )
    {
        const char* const DATA = "\x41\x52\x52\x49\x12\x34\x56\x78";
        
        if ( file.Length() <= ArriFileHeader_SIZE)
            return false;
        
		XMP_Int64 offset = 0; const XMP_Int64 sizeToRead = 16;
        XMP_Uns8 buffer[ sizeToRead ];
        file.Seek ( offset, kXMP_SeekFromStart );
        file.Read( buffer, sizeToRead ,true);
        
        if(memcmp(buffer, DATA, 8) != 0)
            return false;
       
        XMP_Uns32 length= GetUns32LE(buffer+8);
        XMP_Uns32 version= GetUns32LE(buffer+12);
        
        if(version != ARRI_SDK_VERSION && length != ArriFileHeader_SIZE)
            return false;
        
        
        return true;
    }
    
   
    void ARRI_MetaHandler::cacheFileData( const IOAdapter& file, std::string& xmpStr ){
      
        xmpStr.clear();
        XMP_Uns8 buffer[ArriFileHeader_SIZE];
        XMP_OptionBits optionFlags = this->getOpenFlags();
        bool readOnly = ( ! ( optionFlags & kXMPFiles_OpenForUpdate ) );
        if(!readOnly)
            XMP_Throw ( "Host_IO::Open error, file should be opened as ReadOnly", kXMPErr_BadParam );
        ColorSpace colorSpace = getColorSpace(optionFlags);
        SXMPMeta meta;
        
        XMP_Int64 offset=0;
        file.Seek ( offset, kXMP_SeekFromStart );
        XMP_Uns32 readBytes=file.Read(buffer, ArriFileHeader_SIZE,false);
        
        ArriRawMeta::ArriFileHeaderV3 header;
        memcpy(&header, buffer, ArriFileHeader_SIZE);
        Amc::ErrorValue err=arriMetadataContainer.initFromHeader(&header,colorSpace);
        const char* packet = NULL;
		
		err = Amxi::mapInternal(&packet, arriMetadataContainer, 1, ARRI_MetaHandler::getbitsPerComponent(optionFlags));

		if (err.isError())
			XMP_Throw(err.getMsg(), kXMPErr_ExternalFailure);
		
		xmpStr.assign(packet);
     
    }
    
    
    bool ARRI_MetaHandler::getFileModDate ( XMP_DateTime * modDate ){
		if (modDate != nullptr) {
			std::string arriFilePath = this->getPath();
			if (arriFilePath.empty()) return false;
			bool ok = Host_IO::Exists(arriFilePath.c_str());

			if (ok)
				ok = Host_IO::GetModifyDate(arriFilePath.c_str(), modDate);

			return ok;
		}
		else
			return false;
        
    }
    
    void ARRI_MetaHandler::updateFile( const IOAdapter& file, bool doSafeUpdate, const std::string& xmpStr ){
		//Should not be called. Throw Error for precaution
		XMP_Throw("Can't update .ari files", kXMPErr_Unimplemented);

    
    }
    
    
    bool ARRI_MetaHandler::IsMetadataWritable ( ){
        //can't write XMP to ARRI files
        return false;
    }
    
    ColorSpace ARRI_MetaHandler::getColorSpace(XMP_OptionBits optionFlags){
        
        if(optionFlags & kXMPFiles_ARRI_ColorSpace_ITU709)
            return ColorSpace::ITU709;
        
        if(optionFlags & kXMPFiles_ARRI_ColorSpace_CameraNative)
            return ColorSpace::CameraNative;
        
        if(optionFlags & kXMPFiles_ARRI_ColorSpace_P3)
            return ColorSpace::P3;
        
        if(optionFlags & kXMPFiles_ARRI_ColorSpace_ACES)
            return ColorSpace::ACES;
        
        if(optionFlags & kXMPFiles_ARRI_ColorSpace_Film)
            return ColorSpace::Film;
        
        if(optionFlags & kXMPFiles_ARRI_ColorSpace_WideGamut)
            return ColorSpace::WideGamut;
        
        if(optionFlags & kXMPFiles_ARRI_ColorSpace_LogC_Monochrome)
            return ColorSpace::LogC_Monochrome;
        
        if(optionFlags & kXMPFiles_ARRI_ColorSpace_Video_Monochrome)
            return ColorSpace::Video_Monochrome;
        
        if(optionFlags & kXMPFiles_ARRI_ColorSpace_SceneLinear_CameraNative)
            return ColorSpace::SceneLinear_CameraNative;
        
        if(optionFlags & kXMPFiles_ARRI_ColorSpace_Video_ITU2020)
            return ColorSpace::Video_ITU2020;
        
        if(optionFlags & kXMPFiles_ARRI_ColorSpace_Video_DCI_D60)
            return ColorSpace::Video_DCI_D60;
        
        if(optionFlags & kXMPFiles_ARRI_ColorSpace_Video_DCI_D65)
            return ColorSpace::Video_DCI_D65;
        
        //Default value
        return Amc::ArriMetadataContainer::ColorSpace::SceneLinear_WideGamut;
    }
    
    XMP_Uns32 ARRI_MetaHandler::getbitsPerComponent(XMP_OptionBits optionFlags){
        if(optionFlags & kXMPFiles_ARRI_10_bits)
            return 10;
        
        if(optionFlags & kXMPFiles_ARRI_12_bits)
            return 12;
        
        if(optionFlags & kXMPFiles_ARRI_14_bits)
            return 14;
        
        if(optionFlags & kXMPFiles_ARRI_16_bits)
            return 16;
        
        return 12;
        
    }
    

}
