    //
    //  XMPFileOps.cpp
    //  XMPFiles
    //
    //  Created by Lovlesh Malik on 07/01/19.
    //

    #include "XMPFileOps.hpp"
    

    XMPFilesOps::XMPFilesOps():streamIO(NULL){}

    XMPFilesOps::~XMPFilesOps()
	{
		delete streamIO;
		streamIO = NULL;
	}

    bool XMPFilesOps::OpenFile(XMP_StringPtr  stream, XMP_Uns32 length, XMP_FileFormat format, XMP_OptionBits openFlags)
	{
        bool ok=false;
        try {
            ok = xmpFile.OpenFile(stream, format, openFlags);

			#if 0
			//in case previous instance remains
			delete streamIO;
			streamIO = NULL;
			streamIO = new XMPStream_IO((void*)stream, length, NULL, 0);
			ok = xmpFile.OpenFile(streamIO, format, openFlags);
			#endif
        } catch (XMP_Error &exception) {
            throw XMPOpsException(kXMPOpsErr_XMPException, exception.GetErrMsg() );
        }
        return ok;
    }

    bool XMPFilesOps::GetXMP(XMPMetaOps* metaops)
    {
        try {
            SXMPMeta meta;
            bool xmpPresent = xmpFile.GetXMP(&meta);
            metaops->SetMetadataFromMeta(meta);
			return xmpPresent;
        } catch (XMP_Error &exception) {
            throw XMPOpsException(kXMPOpsErr_XMPException, exception.GetErrMsg() );
        }
    }

    void XMPFilesOps::PutXMP(XMPMetaOps* metaops)
    {
        try {
            SXMPMeta meta = metaops->GetMetadata();
            xmpFile.PutXMP(meta);
        } catch (XMP_Error &exception) {
            throw XMPOpsException(kXMPOpsErr_XMPException, exception.GetErrMsg() );
        }
    }

    void XMPFilesOps::CloseFile()
    {
        try {
            xmpFile.CloseFile();
        } catch (XMP_Error &exception) {
            throw XMPOpsException(kXMPOpsErr_XMPException, exception.GetErrMsg() );
        }
    }

	void XMPFilesOps::GetStream(char* buffer)
	{
		try {
			//streamIO->GetStreamAfterUpdate(buffer);  //this api should return a null terminated string 
		}
		catch (XMP_Error &exception) {
			throw XMPOpsException(kXMPOpsErr_XMPException, exception.GetErrMsg());
		}
	}

	XMP_Int32 XMPFilesOps::GetStreamSize()
	{
		return 0;
		//return streamIO->GetStreamSize(); 
	}



