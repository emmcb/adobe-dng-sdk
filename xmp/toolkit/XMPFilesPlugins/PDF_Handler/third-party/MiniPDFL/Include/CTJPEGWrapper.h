///////////////////////////////////////////////////////////////////////////////////////////////////
// CTJPEGWrapper.h
//           Copyright (c) 2007. Adobe Systems, Incorporated. All rights reserved.
//
////////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef __CTJPEGWrapper_H__
#define __CTJPEGWrapper_H__

#include "CTJPEG.h"
#include <exception>

// This file provides C++ wrapper to JPEG C APIs
namespace CTJPEG
{

	/** If error is returned by CTJPEG C APIs, then it is wrapped and thrown as C++ exception 
	 * Note that since CTJPEGWrapper is part of client code and not CTJPEG library, 
	 * no exception is being thrown across DLL boundaries.*/

	class Exception : public std::exception
	{
	public:
		Exception					(CTJPEGError inErrorCode);
		CTJPEGError GetErrorCode		();
	private:
		CTJPEGError fErrorCode;
	};

	/** Base class for CTJPEG class wrappers, below  */
	template<class RefType>
	class BaseRef
	{
	public:
		RefType		fRef;

		// Kill some XCode warnings:
		BaseRef () : fRef () {};
		BaseRef (const BaseRef &);
		BaseRef &operator= (const BaseRef &);
		virtual ~BaseRef () {};
	};

	/** C++ wrapper for CTJPEGReader.
	 *  Client can extend this class by implementing virtual methods.
	 */
	class Reader : public BaseRef<CTJPEGReader>
	{
	public:
		Reader();
		virtual ~Reader();

		/**
		* This method is used to skip forward by "offset" bytes given as parameter.
		* Next Read() call should return data by "offset" byes ahead of current data.
		* \param offset [in] number of bytes to skip by.
		* \return false to indicate some error happened while skipping data otherwise true
		*/
		virtual CTJPEGBool8 Skip(
			CTJPEGUint32 offset
		) = 0;

		/** 
		* This method is used to read atmost "length" bytes from the CTJPEG stream.
		* \param data [in] Pointer to buffer in which data needs to be filled.
		*                      Note that this buffer is pre-allocated by CTJPEG library.
		*                      Client should only copy data in this buffer. Client should
		*                      not try to delete/free this buffer.
		* \param bytesToRead [in] Maximum length of data to be read.
		* \param bytesRead [out] Actual number of bytes read.
		* \return false to indicate some error happened while reading data otherwise true
		*/
		virtual CTJPEGBool8 Read(
			CTJPEGUint8 * data,
			CTJPEGUint32 bytesToRead,
			CTJPEGUint32 *bytesRead
		) = 0; 

		/**
		* This method is used to indicate that this is the last call to CTJPEGReader.
		* Client should cleanup CTJPEGReader releated resources in this callback method.
		*/
		virtual void Close() = 0;
	};

	/** C++ wrapper for CTJPEGWriter.
	 *  Client can extend this class by implementing virtual methods.
	 */
	class Writer : public BaseRef<CTJPEGWriter>
	{
	public:
		Writer();
		virtual ~Writer();

		/** 
		* This method is used to write "length" bytes to the CTJPEG stream.
		* \param data [in] Pointer to buffer in which data needs to be written.
		*                  Note that this buffer is pre-allocated by CTJPEG library.
		*                  Client should only copy data in this buffer. Client should
		*                  not try to delete/free this buffer.
		* \param bytesToWrite [in] Maximum length of data to be written.
		* \param bytesWritten [out] Actual number of bytes written. 
		* \return false to indicate some error happened while reading data otherwise true
		*/
		virtual CTJPEGBool8 Write(
			CTJPEGUint8 * data,
			CTJPEGUint32 bytesToWrite,
			CTJPEGUint32 *bytesWritten
		) = 0; 

		/**
		* This method is used to indicate that this is the last call to CTJPEGWriter.
		* Client should cleanup CTJPEGWriter releated resources in this callback method.
		*/
		virtual void Close() = 0; 
	};

	/** C++ wrapper for CTJPEGWarningHandler.
	 *  Client can extend this class by implementing virtual methods.
	 */
	class WarningHandler : public BaseRef<CTJPEGWarningHandler>
	{
	public:
		WarningHandler();
		virtual ~WarningHandler();

	/** If the CTJPEG image is not well-formed but still can be decoded, 
	* then decoder calls these warning callbacks to ask whether to continue with decoding or not.
	* \param warning CTJPEG warning that occurred while decoding image. 
	* \return if false then decoder stops with warning as CTJPEGError, otherwise continues with decoding.
	*/
		virtual CTJPEGBool8 ProcessWarning(CTJPEGError warning) = 0;
	};

	/** C++ wrapper for CTJPEGMetaDataHandler.
	 *  Client can extend this class by implementing virtual methods.
	 */
	class MetaDataHandler : public BaseRef<CTJPEGMetaDataHandler>
	{
	public:
		MetaDataHandler();

		virtual ~ MetaDataHandler();

		/**
		* This method is called by decoder when start of meta data is found.
		* \param tag [in] APPn tag of metaData found 
		* \param length [in] Length of metaData found
		* \param dataToBeFilled [out] Pointer to data buffer. Client needs to allocate space for 
		* data field of metadata of size not less than the length given as parameter. 
		* If on return, *(dataToBeFilled) points to NULL, then decoder will skip this marker and continue decoding.
		* Otherwise decoder will fill metaData information in this buffer.
		*/
		virtual void InitMetaData(
			CTJPEGUint8 tag,
			CTJPEGUint16 length,
			CTJPEGUint8 **dataToBeFilled
		) = 0;

		/**
		* This method is called by decoder after decoder has filled the metaData object 
		* provided by client in InitMetaData call. Client can process this metaData to 
		* extract relevant information and free data field of metaData.
		* \param tag [in] APPn tag of metaData found 
		* \param length [in] Length of metaData found
		* \param dataFilled [in] Pointer to data buffer allocated by client in last call to InitMetaData().
		* This data buffer is filled by decoder for client use. Client can free this data buffer in this call.
		* \param bSuccess This parameter indicates if metaData was successfully decoded or not. 
		* The value in metaData provided in this call should be used by client only when bSuccess is true.
		* If isDataValid is false, then client should just free the buffer allocated for metadata.
		*/
		virtual void ProcessMetaData(
			CTJPEGUint8 tag,
			CTJPEGUint16 length,
			CTJPEGUint8 *dataFilled,
			CTJPEGBool8 isDataValid
		) = 0;
	};

	/** C++ wrapper for CTJPEGTiledContentReader.
	 *  Client can extend this class by implementing virtual methods.
	 */
	class TiledContentReader : public BaseRef<CTJPEGTiledContentReader>
	{
	public:
		TiledContentReader();
		virtual ~TiledContentReader();

		/**
		* CTJPEG library processes raw image in image parts (called as tiles). In this callback, 
		* the maximum size of tile/image part per request asked by library is passed as parameters.
		* Client can use these parameters to allocate space for fututre requests.
		* \param nMaxRowsPerRequest [in] Maximum number of rows that can be asked by library in any InitContent() request.
		* \param nMaxColsPerRequest [in] Maximum number of columns that can be asked by library in any InitContent() request.
		* \return If true then continue with operation else stop.
		*/
		virtual CTJPEGBool8 Init (
			CTJPEGUint16 nMaxRowsPerRequest,
			CTJPEGUint16 nMaxColsPerRequest
		) = 0;

		/**
		* In this callback, client is asked to allocate and initialize buffers pointing to raw image data
		* for the given region.
		* \param tileArea [in] Region of image for which imageContent needs to be initialized.
		* \param content [out] Image content structure that has buffer pointers initialized 
		*                              to start of the given region. Note that client must allocate space
		*                              for imageContent->componentData buffers. During decoding, decoder 
		*	                           fills this buffer with the decoded raw image data. During encoding, 
		*                              encoder reads from this buffer to to get raw image data for that region
		*                              and in turn uses it to create encoded CTJPEG stream.
		* \return If true then continue with operation else stop.
		*/
		virtual CTJPEGBool8 InitContent (
			CTJPEGRect tileArea,
			CTJPEGImageContent *content
		) = 0;

		/**
		* This callback method is used to indicate that this is the last call to CTJPEGTiledContentReader.
		* Client should cleanup CTJPEGTiledContentReader releated resources in this callback method.
		*/
		virtual void Close () = 0;

	};

	/** C++ wrapper for CTJPEGTiledContentWriter.
	 *  Client can extend this class by implementing virtual methods.
	 */
	class TiledContentWriter : public BaseRef<CTJPEGTiledContentWriter>
	{
	public:
		TiledContentWriter();
		virtual ~TiledContentWriter();

		/**
		* CTJPEG library processes raw image in image parts (called as tiles). In this method, 
		* the maximum size of tile/image part per request asked by library is passed as parameters.
		* Client can use these parameters to allocate space for fututre requests.
		* \param nMaxRowsPerRequest [in] Maximum number of rows that can be asked by library in any InitContent() request.
		* \param nMaxColsPerRequest [in] Maximum number of columns that can be asked by library in any InitContent() request.
		* \return If true then continue with operation else stop.
		*/
		virtual CTJPEGBool8 Init (
			CTJPEGUint16 nMaxRowsPerRequest,
			CTJPEGUint16 nMaxColsPerRequest
		) = 0;


		/**
		* In this method, client is asked to allocate and initialize buffers pointing to raw image data
		* for the given region.
		* \param tileArea [in] Region of image for which imageContent needs to be initialized.
		* \param content [out] Image content structure that has buffer pointers initialized 
		*                              to start of the given region. Note that client must allocate space
		*                              for imageContent->componentData buffers. During decoding, decoder 
		*	                           fills this buffer with the decoded raw image data. During encoding, 
		*                              encoder reads from this buffer to to get raw image data for that region
		*                              and in turn uses it to create encoded CTJPEG stream.
		* \return If true then continue with operation else stop.
		*/
		virtual CTJPEGBool8 InitContent (
			CTJPEGRect tileArea,
			CTJPEGImageContent *content
		) = 0;

		/**
		* CTJPEG Decoder calls this method to indicate if image content has been successfully filled or not.
		* If yes, then client can process this content e.g. display it. 
		* \param tileArea [in] Region of image for which imageContent is initialized.
		* \param contentFilled [in] Image content structure that has buffer pointers initialized 
		*                         to start of the given region. Note that space for data buffers of
		*                         imageConent were allocated by client. This space is used by decoder
		*                         to fill raw image data and pass it on to client. This space should not be
		*                         freed by client before Close() call.
		* \return If true then continue with operation else stop.
		*/
		virtual CTJPEGBool8 ProcessContent (
			CTJPEGRect tileArea,
			CTJPEGImageContent contentFilled
		) = 0;

		/**
		* This method is used to indicate that this is the last call to CTJPEGTiledContentWriter.
		* Client should cleanup CTJPEGTiledContentWriter releated resources in this method.
		*/
		virtual void Close () = 0;
	};

	/** C++ wrapper for CTJPEGDecoderRef. */
	class Decoder : public BaseRef<CTJPEGDecoderRef>
	{
	public:
		/** This method is used to create CTJPEG decoder to decode from buffer containg encoded jpeg image.
		* \param streamData [in] Buffer containing encoded jpeg image data.
		* \param decodeOptions [in] Decode options to be used for decoding
		* \param bSkipWarning [in] Flag for skipping warnings. If the CTJPEG image is not well-formed 
		*                     but still can be decoded then decoder raises warning. If this flag is true 
		*                     then decoder will ignore warnings and continue with decoding. Otherwise, it
		*                     will stop decoding and return with warning as error.
		* \param warningHandler [in] Handler to handle decode warnings. If handler is not NULL and bSkipWarning is
		*                     false then callbacks of warningHandler are called by decoder on warning. Otherwise
		*                     warningHandler is not used.
		*/
		Decoder(
			CTJPEGData streamData,
			CTJPEGDecodeOptions decodeOptions,
			CTJPEGBool8 bSkipWarning,
			WarningHandler* warningHandler = 0);

		/** This method is used to create CTJPEG decoder. Note that after use, CTJPEGDecoderRelease() needs be called to 
		* avoid memory leaks.
		* \param reader [in] structure to provide callbacks to read encoded jpeg data.
		* \param decodeOptions [in] Decode options to be used for decoding
		* \param bSkipWarning [in] Flag for skipping warnings. If the CTJPEG image is not well-formed 
		*                     but still can be decoded then decoder raises warning. If this flag is true 
		*                     then decoder will ignore warnings and continue with decoding. Otherwise, it
		*                     will stop decoding and return with warning as error.
		* \param warningHandler [in] Handler to handle decode warnings. If handler is not NULL and bSkipWarning is
		*                     false then callbacks of warningHandler are called by decoder on warning. Otherwise
		*                     warningHandler is not used.
		*/
		Decoder(
			Reader& reader,
			CTJPEGDecodeOptions decodeOptions,
			CTJPEGBool8 bSkipWarning,
			WarningHandler* warningHandler = 0
		);

		~Decoder();

		/** This method is used to parse and get header information from the encoded image.
		* \param metaDataHandler [in] Handler to handle metaData callbacks. If metaDataHandler is not NULL, 
		*                             then the callbacks specified in metaDataHandler are called by decoder
		*                             at the time of parsing metadata.
		* \return				 structure to hold header information. Decoder fills header with information 
		*						 parsed from input jpeg image.
		*/
		CTJPEGHeader ReadHeader(MetaDataHandler* metaDataHandler = 0);

		/** This method is used to parse and get header information from the encoded image and the encoded params
		* like quantization tables, sampling factors etc.
		* \param metaDataHandler [in] Handler to handle metaData callbacks. If metaDataHandler is not NULL, 
		*                             then the callbacks specified in metaDataHandler are called by decoder
		*                             at the time of parsing metadata.
		* \param encodedParams [out] Pointer to structure to hold the JPEG Encoded Params. If encodedParams is not
		*                             NULL, then the decoder fills encodedParams with information parsed from input jpeg image.
		* \return				 structure to hold header information. Decoder fills header with information 
		*						 parsed from input jpeg image.
		*/
		CTJPEGHeader ReadHeaderAndEncodedParams(MetaDataHandler* metaDataHandler = 0,
												CTJPEGEncodedParams * encodedParams = NULL);

		/**
		* This method is used to decode raw image content from the encoded image.
		* \param colorSpace [in] Desired colorSpace of decoded raw image 
		* \param contentWriter [in] Handler to handle content write callbacks. If contentWriter is not NULL, 
		*                             then the callbacks specified in contentWriter are called by decoder
		*                             at the time of decoding image content.
		* \return CTJPEGErrorNone on success.
		*/
		void ReadTiledContent(
				CTJPEGColorSpace colorSpace,
				TiledContentWriter& contentWriter
		);

		/**
		* This method is used to decode raw image content from the encoded image and store it in 
		* the given imageContent structure.
		* \param colorSpace [in] Desired colorSpace of decoded raw image 
		* \param imageContentToBeFilled [in] ImageContent that will be filled by decoder.
		*                                    Client should allocate space and initialize componentData buffers
		*                                    of this structure to hold decoded raw data for complete image.
		* \return CTJPEGErrorNone on success.
		*/
		void ReadContent(
				CTJPEGColorSpace colorSpace,
				CTJPEGImageContent imageContentToBeFilled
		);

		/**
		* This method is used to set color space of output raw data 
		* \param outColorSpace [in] Desired colorSpace of decoded raw image 
		*/
		void SetOutColorSpace(
			CTJPEGColorSpace outColorSpace
		);

		/**
		* This method is check if there is some tile left to for decoding.
		* \return true if there are more tiles to be decoded, false otherwise.
		*/
		CTJPEGBool8 HasMoreTiles();

		/**
		* This method is used to get information about next tile of raw image content to be decoded.
		* \return tileArea   Area of next tile that will be decoded
		*/
		CTJPEGRect GetNextTileInfo();

		/**
		* This method is used to decode next tile of raw image content from the encoded image and store it in 
		* the given imageContent structure.
		* \param imageContentToBeFilled [in] ImageContent that will be filled by decoder.
		*                                    Client should allocate space and initialize componentData buffers
		*                                    of this structure to hold decoded raw data for next tile.
		*/
		void ReadNextTile(
			CTJPEGImageContent contentToBeFilled
		);
	};

	/** C++ wrapper for CTJPEGEncoderRef. */
	class Encoder : public BaseRef<CTJPEGEncoderRef>
	{
	public:
		/** 
		* This method is used to create CTJPEG encoder to encode image.
		* \param writer [in] Structure to provide callbacks to write encoded jpeg data.
		* \param encodeOptions [in] Options to be used for encoding.
		*/
		Encoder(
			Writer& writer,
			CTJPEGEncodeOptions encodeOptions
		);

		~Encoder();

		/** 
		* This method is used to write meta data in encoded jpeg.
		* This method should be called before writing content. Otherwise eCTJPEGErrorCannotWriteMetaDataAfterContent
		* error will be returned.
		* \param metaData [in] Structure containing MetaData information.
		* \return CTJPEGErrorNone on success.
		*/
		void WriteMetaData(
			CTJPEGMetaData metaData
		);

		/**
		* This method is used to write more than one meta data tags in bulk in encoded jpeg.
		* This method should be called before writing content. Otherwise exception with error code
		* eCTJPEGErrorCannotWriteMetaDataAfterContent will be thrown.
		* <BR>As per CTJPEG specification, maximum length of one metadata marker is 2^16 - 3. If metaData
		* to be written is more than the max metadata length, then client can split metaData into buffers
		* of size less than max metadata length  and make sepearate calls to WriteMetadata().
		* Other option with client is to use CTJPEGEncoderBulkWriteMetaData(). This API allows user to 
		* specify data of more than 2^16 bytes.
		* <BR>Using this API, client can write more than one metadata markers in one go. In this case, 
		* the client should add APPn tag and APPn length appropriately for each metadata marker to the buffer. CTJPEG Encoder 
		* will blindly write the buffer passed into CTJPEG stream. 
		* error will be returned.
		* \param encoderRef [in] Reference to encoder object
		* \param metaDataAsBlob [in] buffer containing MetaData information. This buffer is wriiten to CTJPEG stream as it is.
		* \param sizeOfMetaDataAsBlob [in] size of metaData buffer
		*/
		void BulkWriteMetaData(
			CTJPEGUint8*	metaDataAsBlob,
			CTJPEGUint32	sizeOfMetaDataAsBlob
		);

		/** 
		* This method is used to write image content in encoded jpeg.
		* \param header [in] Structure containing header information.
		* \param contentReader [in] Structure containing content read related callbacks.
		*/
		void WriteTiledContent(
			CTJPEGHeader header,
			TiledContentReader& contentReader
		);

		/** 
		* This method is used to write image content in encoded jpeg.
		* \param header [in] Structure containing header information.
		* \param imageContent [in] Structure containing raw content for complete image.
		*/
		void WriteContent(
			CTJPEGHeader header,
			CTJPEGImageContent imageContent
		);


		/**
		* This method is used to write image header in encoded jpeg.
		* \param header [in] Structure containing header information.
		*/
		void WriteHeader(
			CTJPEGHeader header
		);

		/**
		* This method is used to encode next tile of raw image content given in imageContent
		* \param tileArea   [out] Area of next tile that will be encoded
		* \param contentToBeEncoded [in] ImageContent that will be filled by decoder.
		*                                    Client should allocate space and initialize componentData buffers
		*                                    of this structure to hold decoded raw data for next tile.
		*/
		void WriteNextTileContent(
			CTJPEGRect			tileArea,
			CTJPEGImageContent	contentToBeEncoded
		);

	};

	/** C++ wrapper for CTJPEGRotateImage. */
	void RotateImage(
		  Reader& reader,
		  Writer& outputStream,
		  CTJPEGRotateOptions rotateOptions);
};
#endif
