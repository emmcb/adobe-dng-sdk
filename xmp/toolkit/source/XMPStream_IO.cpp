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
//
// Writers:
//Yash Arora

#endif	// AdobePrivate

#include "public/include/XMP_Environment.h"	// ! XMP_Environment.h must be the first included header.

#include "public/include/XMP_Const.h"
#include "public/include/XMP_IO.hpp"

#include "source/XMP_LibUtils.hpp"
#include "source/XMPStream_IO.hpp"
#include "source/XIO.hpp"
#include "source/XMP_LibUtils.hpp"

#define TwoGB (XMP_Uns32)(2*1024*1024*1024UL)

#define XMP_STREAMIO_START try { /* int a;*/
#define XMP_STREAMIO_END1(severity)																				\
		/*b = 1;*/																									\
	} catch ( XMP_Error & error ) {																				\
		 throw;																								\
	}
void XMPStream_IO::CopyBuffer(char* src, char* dest,int length)
{
	/*for (int i = 0; i < length; i++)
		dest[i] = src[i];*/
	memcpy(dest, src, length);
}

XMPStream_IO::XMPStream_IO(void * buffer, int length)
{
	this->Globalbuffer = new char[length];
	CopyBuffer((char*) buffer, this->Globalbuffer,length);
	this->length = length;
	this->capacity = length;

}

XMPStream_IO::~XMPStream_IO()
{
	if(this->Globalbuffer !=NULL)
		delete[] this->Globalbuffer;
	this->Globalbuffer = NULL;

}

void XMPStream_IO::ExpandBuffer(int currLength, int reqLength)
{
	XMP_STREAMIO_START
	char* tempBuffer = new char[currLength];
	CopyBuffer(this->Globalbuffer, tempBuffer, currLength);
	
	if(this->Globalbuffer!=NULL)
		delete[] this->Globalbuffer;

	this->Globalbuffer = NULL;
	int optimumNewLength = (currLength + (2 * reqLength));
	this->Globalbuffer = new char[optimumNewLength];
	
	for (int i = 0; i < currLength; i++)
	{
		this->Globalbuffer[i] = tempBuffer[i];
	}
	this->length = optimumNewLength;

	if(tempBuffer!=NULL)
		delete[] tempBuffer;
	tempBuffer = NULL;
	XMP_STREAMIO_END1(kXMPErrSev_StreamFatal)
	
}

XMP_Uns32 XMPStream_IO::Read(void* buffer, XMP_Uns32 count, bool readAll)
{
	XMP_STREAMIO_START
	if (count >= TwoGB) 
		XMP_Throw("Stream_IO::Read, request too large", kXMPErr_EnforceFailure);

	if (count > (this->length - this->curr_offset)) {
		if (readAll) 
			XMP_Throw("XMPStream_IO::Read, not enough data", kXMPErr_EnforceFailure);
		count = (XMP_Uns32)(this->length - this->curr_offset);
	}

	char *charBuffer;
	charBuffer = (char*)buffer;

	for (int i = 0; i < count; i++)
	{
		charBuffer[i] = this->Globalbuffer[this->curr_offset + i];
	}
	buffer = (void *)charBuffer;

	this->curr_offset += count;

	return count;
	XMP_STREAMIO_END1(kXMPErrSev_StreamFatal)
	return 0;


}

void XMPStream_IO::Write(const void * buffer, XMP_Uns32 count) {
	XMP_STREAMIO_START
	int init_offset = this->curr_offset;
	int init_length = this->length;
	try {
		if (this->readOnly)
			XMP_Throw("New_XMPStream_IO, write not permitted on read only stream", kXMPErr_FilePermission);
		//if(this->length - this->curr_offset < count || buffer == NULL)
		if (buffer == NULL) 
			XMP_Throw("New_XMPStream_IO, Bad input Parameters", kXMPErr_BadStreamParameters);
		
		//check if Global buffer's length is zero meaning buffer has been cleared using truncate(0)
		if (this->length == 0)
		{
			this->Globalbuffer = new char[count*2];
			this->curr_offset = 0;
			this->length = count*2;
		}
		if ((this->curr_offset + count) > this->length) 
		{
			ExpandBuffer(this->length, count);
		}
		int byteswritten=0;
		char *charBuffer;
		charBuffer = (char*)buffer;

		for (int i = 0; i < count; i++)
		{
			this->Globalbuffer[this->curr_offset + i] = charBuffer[i];
			byteswritten++;
		}
		if(byteswritten!=count)
			XMP_Throw("New_XMPStream_IO, Bad input Parameters", kXMPErr_BadStreamParameters);
		
	}
	catch (...) {
		try {
			// we should try to maintain the state as best as possible
			// but no exception should escape from this backup plan.
			// Make sure the internal state reflects partial writes.
			this->curr_offset = init_offset;
			this->length = init_length;
		}
		catch (...) {
			// don't do anything
		}
		throw;
	}

	this->curr_offset += count;
	if (this->curr_offset > this->length) this->length = this->curr_offset;
	XMP_STREAMIO_END1(kXMPErrSev_StreamFatal)
}

XMP_Int64 XMPStream_IO::Seek(XMP_Int64 offset, SeekMode mode) 
{ 
	XMP_Int64 newOffset = offset;
	if (mode == kXMP_SeekFromCurrent) {
		newOffset += this->curr_offset;
	}
	else if (mode == kXMP_SeekFromEnd) {
		newOffset = this->length - newOffset;
	}

	this->curr_offset = newOffset; //as seekfromstart is equal to newoffset itself

	return this->curr_offset;
}

XMP_Int64 XMPStream_IO::Length()
{ 
	return this->length; 
}

void XMPStream_IO::Truncate(XMP_Int64 length) {
	XMP_STREAMIO_START
	if (this->readOnly)
		XMP_Throw("New_Stream_IO, truncate not permitted on read only stream", kXMPErr_FilePermission);

	XMP_Enforce(length <= this->length);
	try {
		if (length > 0)
		{
			//To-Do create a function to deep copy object
			char* tempBuffer = new char[length];
			CopyBuffer(this->Globalbuffer, tempBuffer, length);
			char *temp = this->Globalbuffer;
			if (temp != NULL)
				delete[] temp;
			temp = NULL;
			this->Globalbuffer = NULL;
			this->Globalbuffer = new char[length];
			CopyBuffer(tempBuffer, this->Globalbuffer, length);
		}

		else if (length == 0)
		{
			try
			{
				char *temp = this->Globalbuffer;
				if (temp != NULL)
					delete[] temp;
				temp = NULL;
				this->Globalbuffer = NULL;
			}
			catch (...)
			{
				throw;
			}

			this->curr_offset = 0;
		}
		else {
			XMP_Throw("New_Stream_IO, truncate not possible for negative length", kXMPErr_BadStreamParameters);
		}
		this->length = length;
		//if offset has become invalid now then make offset at end of stream
		if (this->curr_offset > this->length) this->curr_offset = this->length - 1;
	}
	catch (...)
	{
		throw;
	}
	XMP_STREAMIO_END1(kXMPErrSev_StreamFatal)
	
}

XMP_IO * XMPStream_IO::DeriveTemp()
{
	XMP_STREAMIO_START
	if (this->derivedTemp != NULL) return this->derivedTemp;
	if (this->readOnly) {
		XMP_Throw("XMPStream_IO::DeriveTemp, can't derive from read-only", kXMPErr_InternalFailure);
	}
	try {

		XMPStream_IO* newTemp = new XMPStream_IO((void *)this->Globalbuffer, this->length);
		if (newTemp == 0) {
			newTemp = nullptr;
			XMP_Throw("XMPStream_IO::DeriveTemp, can't open temp stream", kXMPErr_InternalFailure);
		}
		newTemp->isTemp = true;
		this->derivedTemp = newTemp;
	}
	catch (...)
	{
		this->derivedTemp = NULL;
		throw;
	}
	
		
	return this->derivedTemp;
	XMP_STREAMIO_END1(kXMPErrSev_StreamFatal)
	return NULL;
}

void XMPStream_IO::AbsorbTemp() 
{
	XMP_STREAMIO_START
	XMPStream_IO * temp = this->derivedTemp;
	if (temp == NULL) {
		XMP_Throw("XMPStream_IO::AbsorbTemp, no temp to absorb", kXMPErr_InternalFailure);
	}
	XMP_Assert(temp->isTemp);

	if (this->Globalbuffer != NULL)
		delete[] this->Globalbuffer;
	this->Globalbuffer = NULL;

	this->Globalbuffer = new char[temp->length];
	CopyBuffer(temp->Globalbuffer, this->Globalbuffer, temp->length);
	this->length = temp->length;

	this->DeleteTemp();
	this->curr_offset = 0;
	XMP_STREAMIO_END1(kXMPErrSev_StreamFatal)
}

void XMPStream_IO::DeleteTemp() 
{
	XMP_STREAMIO_START
	XMPStream_IO * temp = this->derivedTemp;

	if (temp != 0) {

		if (temp->Globalbuffer != NULL) {
			delete[] temp->Globalbuffer;
		}

		//delete temp;
		this->derivedTemp = 0;

	}
	XMP_STREAMIO_END1(kXMPErrSev_StreamFatal)
}

void XMPStream_IO::GetCurrentStream(char *outputBuffer, int outputLength)
{
	XMP_STREAMIO_START

	XMP_Enforce(outputLength >= this->length);
	CopyBuffer(this->Globalbuffer, outputBuffer, outputLength);

	XMP_STREAMIO_END1(kXMPErrSev_StreamFatal)
}
int XMPStream_IO::GetCurrentStreamLength()
{
	return this->length;
}