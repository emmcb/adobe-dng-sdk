//
//  XMPFileOpsWrapper.cpp
//  XMPFiles
//
//  Created by Lovlesh Malik on 09/01/19.
//

#include "XMPFileOpsWrapper.hpp"

#if __cplusplus
extern "C"{
#endif
    XMPFilesOps* CreateFileOps( XMP_Int32* id, char* msg)
    {
        
        XMPOps_Assert_2(id, msg);
		TRY
        return new XMPFilesOps();
        XMPOPS_CATCH_EXCEPTIONS
        return 0;
    }
    
    bool InitializeXMPFile(XMP_Int32* id, char* msg)
    {
        
        XMPOps_Assert_2(id, msg);
		TRY
        return SXMPFiles::Initialize(kXMPFiles_IgnoreLocalText);
		}
		catch (XMP_Error e) {
			*id = e.GetID();
			memcpy(msg, e.GetErrMsg(), strlen(e.GetErrMsg()));
			msg[strlen(e.GetErrMsg())] = '\0';

		}
		catch (...) {
			*id = kXMPOpsErr_InternalFailure;
			std::string message = "Internal Failure";
			memcpy(msg, message.c_str(), message.length());
			msg[message.length()] = '\0';
		}
        return false;
    }
    
    bool OpenFileStream(XMPFilesOps* xmpFileOps,XMP_StringPtr stream, XMP_Uns32 length, XMP_FileFormat format, XMP_OptionBits openFlags, XMP_Int32* id, char* msg)
    {
        
        XMPOps_Assert_3(xmpFileOps, id, msg);
		TRY
        return xmpFileOps->OpenFile(stream, length, format, openFlags );
        XMPOPS_CATCH_EXCEPTIONS
        return false;
    }
    
    bool GetXMP(XMPFilesOps* xmpFileOps,XMPMetaOps* metaops,XMP_Int32* id, char* msg)
    {
        
        XMPOps_Assert_4(xmpFileOps, metaops, id, msg);
		TRY
        return xmpFileOps->GetXMP(metaops);
		XMPOPS_CATCH_EXCEPTIONS
		return false;
    }
    
    void PutXMP(XMPFilesOps* xmpFileOps,XMPMetaOps* metaops,XMP_Int32* id, char* msg)
    {
        
        XMPOps_Assert_4(xmpFileOps, metaops, id, msg);
		TRY
        xmpFileOps->PutXMP(metaops);
        XMPOPS_CATCH_EXCEPTIONS
    }
    
    void CloseFile(XMPFilesOps* xmpFileOps,XMP_Int32* id, char* msg)
    {
        
        XMPOps_Assert_3(xmpFileOps, id, msg);
		TRY
        xmpFileOps->CloseFile();
        XMPOPS_CATCH_EXCEPTIONS
    }
    
	void GetStream(XMPFilesOps* xmpFileOps, char* buffstr, XMP_Int32* id, char* msg)
	{

		XMPOps_Assert_3(xmpFileOps, id, msg);
		TRY
			xmpFileOps->GetStream(buffstr);
		XMPOPS_CATCH_EXCEPTIONS
			
	}

	XMP_Int32 GetStreamSize(XMPFilesOps* xmpFileOps)
	{
		XMPOps_Assert_1(xmpFileOps);
		return xmpFileOps->GetStreamSize();
	}

    void TerminateXMPFile(XMP_Int32* id, char* msg)
    {
        
        XMPOps_Assert_2(id, msg);
		TRY
			SXMPFiles::Terminate();
		}
		catch (XMP_Error e) {
			*id = e.GetID();
			memcpy(msg, e.GetErrMsg(), strlen(e.GetErrMsg()));
			msg[strlen(e.GetErrMsg())] = '\0';

		}
		catch (...) {
			*id = kXMPOpsErr_InternalFailure;
			std::string message = "Internal Failure";
			memcpy(msg, message.c_str(), message.length());
			msg[message.length()] = '\0';
		}
    }
    
    void DestroyXMPFiles(XMPFilesOps* xmpFileOps)
    {
        XMPOps_Assert_1(xmpFileOps);
        delete xmpFileOps;
    }
    

  /*
    bool OpenFile_lmalik ( string xmpString1 )
    {
        SXMPFiles xmpFile;
        string xmpString="testfiles/straightFromCamera.jpg";
        FILE *file = fopen(xmpString.c_str(), "rb");
        if (!file) {
            // will add logging here
        }
        try {
            if (!SXMPFiles::Initialize(kXMPFiles_IgnoreLocalText))
            {
                // Log exception here
                //err.GetErrMsg()
                return false;
                
            }
            bool ok = xmpFile.OpenFile(xmpString.c_str(), kXMP_JPEGFile, kXMPFiles_OpenForRead | kXMPFiles_OpenUseSmartHandler);
            
            xmpFile.CloseFile();
            return ok;
        } catch (XMP_Error err) {
            // Log exception here
            //err.GetErrMsg()
            return false;
        }
    }
    
    SXMPMeta* GetXMP_lmalik(string xmpString1){
        SXMPFiles xmpFile;
        SXMPMeta xmpMeta;
        string metadata;
        string xmpString="testfiles/straightFromCamera.jpg";
        try {
            if (!SXMPFiles::Initialize(kXMPFiles_IgnoreLocalText))
            {
                return 0;
                
            }
            bool ok = xmpFile.OpenFile(xmpString.c_str(), kXMP_JPEGFile, kXMPFiles_OpenForRead | kXMPFiles_OpenUseSmartHandler);
            xmpFile.GetXMP(&xmpMeta);
            xmpMeta.SerializeToBuffer(&metadata);
            //Print metadata here
            xmpFile.CloseFile();
            return &xmpMeta;
            
        } catch (XMP_Error err) {
            // Log exception here
            //err.GetErrMsg()
            //return false;
            return 0;
        }
    }
    
    void PutXMP_lmalik(string xmpString1,SXMPMeta* meta){
        SXMPFiles xmpFile;
        SXMPMeta xmpMeta;
        string metadata;
        string xmpString="testfiles/straightFromCamera.jpg";
        try {
            if (!SXMPFiles::Initialize(kXMPFiles_IgnoreLocalText))
            {
                
            }
            bool ok = xmpFile.OpenFile(xmpString.c_str(), kXMP_JPEGFile, kXMPFiles_OpenForRead | kXMPFiles_OpenUseSmartHandler);
            meta->SetProperty(kXMP_NS_DM, "album", " album");
            xmpFile.PutXMP(*meta);
            //Print metadata here
            xmpFile.CloseFile();
            
        } catch (XMP_Error err) {
            
        }
    }
   */
    
#if __cplusplus
}
#endif
