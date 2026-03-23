//
//  XMPFileOpsWrapper.hpp
//  XMPFiles
//
//  Created by Lovlesh Malik on 09/01/19.
//

#ifndef XMPFileOpsWrapper_hpp
#define XMPFileOpsWrapper_hpp

#include "XMPFileOps.hpp"

#if __cplusplus
extern "C"{
#endif
    
    XMPFilesOps* CreateFileOps( XMP_Int32* id, char* msg);
    
    bool InitializeXMPFile(XMP_Int32* id, char* msg);
     
    bool OpenFileStream(XMPFilesOps* xmpFileOps, XMP_StringPtr stream, XMP_Uns32 length, XMP_FileFormat format, XMP_OptionBits openFlags, XMP_Int32* id, char* msg);
    
    bool GetXMP(XMPFilesOps* xmpFileOps,XMPMetaOps* metaops,XMP_Int32* id, char* msg);
    
    void PutXMP(XMPFilesOps* xmpFileOps,XMPMetaOps* metaops,XMP_Int32* id, char* msg);
    
    void CloseFile(XMPFilesOps* xmpFileOps,XMP_Int32* id, char* msg);

	void GetStream(XMPFilesOps* xmpFileOps, char* buffstr, XMP_Int32* id, char* msg);

	XMP_Int32 GetStreamSize(XMPFilesOps* xmpFileOps);
    
    void TerminateXMPFile(XMP_Int32* id, char* msg);
    
    void DestroyXMPFiles(XMPFilesOps* xmpFileOps);
    
    /*bool OpenFile_lmalik ( string xmpString );
    SXMPMeta* GetXMP_lmalik(string xmpString);
    void PutXMP_lmalik(string xmpString,SXMPMeta* meta);*/
    
#if __cplusplus
}
#endif

#endif /* XMPFileOpsWrapper_hpp */
