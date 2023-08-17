#ifndef __XMPStream_IO_hpp__
#define __XMPStream_IO_hpp__ 1

// =================================================================================================
// Copyright Adobe
// Copyright 2010 Adobe
// All Rights Reserved
//
// NOTICE:  Adobe permits you to use, modify, and distribute this file in accordance with the terms
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
//  Yash Arora
//
// =================================================================================================
#endif	// AdobePrivate

#include "public/include/XMP_Environment.h"	// ! XMP_Environment.h must be the first included header.

#include "public/include/XMP_Const.h"
#include "public/include/XMP_IO.hpp"

#include "source/Host_IO.hpp"
#include "source/XMP_ProgressTracker.hpp"
#include "XMP_LibUtils.hpp"

#include <string>

// =================================================================================================


class XMPStream_IO : public XMP_IO {

public:
	XMP_Uns32 Read(void* buffer, XMP_Uns32 count, bool readAll = false);
	void Write(const void * buffer, XMP_Uns32 count);
	XMP_Int64 Seek(XMP_Int64 offset, SeekMode mode);
	XMP_Int64 Length();
	void Truncate(XMP_Int64 length);
	XMP_IO * DeriveTemp();
	void AbsorbTemp();
	void DeleteTemp();
	//for this API to work please call GetCurrentStreamLength first to know the size of buffer for memory allocation of output buffer
	void GetCurrentStream(char *outputBuffer, int outputLength);
	int GetCurrentStreamLength();
	XMPStream_IO(void * buffer, int length);
	~XMPStream_IO();
private:
	bool readOnly=false;
	char *Globalbuffer=NULL;
	int length = 0;
	int capacity = 0;
	int curr_offset=0;
	XMPStream_IO * derivedTemp=NULL;
	bool isTemp=false;
	void *outputBuffer=NULL;
	int outputLength=0;

	void ExpandBuffer(int bufferLength, int newLength);
	void CopyBuffer(char* source, char* destination, int length);
};


#endif	// __XMPStream_IO_hpp__
