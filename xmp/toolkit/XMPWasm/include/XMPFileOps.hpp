//
//  XMPFileOps.hpp
//  XMPFiles
//
//  Created by Lovlesh Malik on 07/01/19.
//

#ifndef XMPFileOps_hpp
#define XMPFileOps_hpp

#include "MetaOps.hpp"
#include "source/XMPStream_IO.hpp"

class XMPFilesOps{
    
private:
    SXMPFiles xmpFile;
	XMP_IO* streamIO;
public:
    
    XMPFilesOps();
    
    ~XMPFilesOps();
    
    bool OpenFile(XMP_StringPtr stream, XMP_Uns32 length, XMP_FileFormat format, XMP_OptionBits openFlags);
    
    bool GetXMP(XMPMetaOps* metaops);
    
    void PutXMP(XMPMetaOps* metaops);
    
    void CloseFile();

	void GetStream(char* buffer);

	XMP_Int32 GetStreamSize();
};

#endif /* XMPFileOps_hpp */
