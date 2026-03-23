///////////////////////////////////////////////////////////////////////////////////////////////////
// CTJPEG.h
//           Copyright (c) 2006-2007. Adobe Systems, Incorporated. All rights reserved.
//
////////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef __H_CTJPEG__
#define __H_CTJPEG__

#include <stdio.h>

#include "CTJPEGTypes.h"

#ifdef __cplusplus
extern "C" {
#endif

#define CT_JPEGLIB_EXPORT
/*
#if WIN32
#define CT_JPEGLIB_EXPORT __declspec(dllexport)
#elif (__GNUC__) && (__GNUC__ > 3)
#define CT_JPEGLIB_EXPORT __attribute__((visibility("default")))
#else
#define CT_JPEGLIB_EXPORT
#endif
*/
/**
* Error codes for CTJPEG Library
*/
enum CTJPEGError
{
	eCTJPEGErrorNone					=	0,
	eCTJPEGErrorInvalidDQT				=	-1,
	eCTJPEGErrorInvalidDQTIndexInHeader	=	-2,
	eCTJPEGErrorInvalidDHT				=	-3,
	eCTJPEGErrorInvalidSOS				=	-4,
	eCTJPEGErrorInvalidBlock			=	-5,
	eCTJPEGErrorInvalidSOF				=	-6,
	eCTJPEGErrorInvalidImage			=	-7,

	// if precision is any thing other than 8 bit then this error code is used.
	// we support only 8 bit jpegs (just like EdFiala library)
	eCTJPEGErrorInvalidPrecision		=	-8,
	eCTJPEGErrorInvalidCompCountInFrame	=	-9,
	eCTJPEGErrorComponentIDInScanNotUnique	=	-10,
	eCTJPEGErrorImageHeightOrWidthZero	=	-11,

	eCTJPEGErrorImproperRotateImageSize	=	-101,
	eCTJPEGErrorInvalidArgument			=	-102,
	eCTJPEGErrorInsufficientMemory		=	-103,
	eCTJPEGErrorCannotWriteMetaDataAfterContent	= -104,
	eCTJPEGErrorWriteFailed				=	-105,
	eCTJPEGErrorReadFailed				=	-106,
	eCTJPEGErrorSkipFailed				=	-107,
	eCTJPEGErrorUnknown					=	-108,
	eCTJPEGErrorNotFoundSOF				=	-109,
	eCTJPEGErrorNotFoundDQT				=	-110,
	eCTJPEGErrorNotFoundDHT				=	-111,
	eCTJPEGErrorInvalidSamplingRatio	=	-112,

	eCTJPEGErrorPreviewTaskNotSet		=	-120,
	eCTJPEGErrorInvalidEncodeTask		=	-121,

	eCTJPEGWarnBadHuffmanCode			=	-300,
	eCTJPEGWarnPrematureEOF				=	-301,
	eCTJPEGErrorStoppedOnClientRequest	=	-350,
	eCTJPEGErrorThreadCreationFailed	=	-351,

	// internal
	eCTJPEGErrorInvalidDecoderState		= -400,
	eCTJPEGErrorMaxVal					=	CT_JPEG_MIN_INT
};



/**
* Color spaces supported by CTJPEG library.
*/
enum CTJPEGColorSpace
{
	/**
	* Default color space. 
	* Based on the jpeg image info, CTJPEG Decoder tries to guess the color space of original image.
	* If no info is present then eRGB24 is used as default colorspace.
	* One should not pass this to the Encoder
	*/
	eCTJPEGDefaultColorSpace = 0,
	/**
	* RGB (24 bits) color space
	*/
	eCTJPEGRGB24		=	1,
	/**
	* Gray (8 bits) color space
	*/
	eCTJPEGGray8		=	2,

	/**
	* CMYK color space
	*/
	eCTJPEGCMYK		=	4,

	/**
	* Lab color space
	*/
	eCTJPEGLabColor	=	5,

	/**
	* ARGB color space
	*/
	eCTJPEGARGB	=	6,

	/**
	* If the image color-space is none of the above, then the client has 
	* to set the color-space as native color space(eCTJPEGNativeColorSpace). 
	* In this case no color transform is applied.
	*/
	eCTJPEGNativeColorSpace = 7,

	eCTJPEGColorSpaceMaxValue	= CT_JPEG_MAX_INT
};


/**
* Type memory allocated by CTJPEG library.
*/
enum CTJPEGMemoryType
{
	/**
	* Default type of memory.
	* Normal memory used for regular JPEG structures and class instances
	*/
	eCTJPEGNormalMemory = 0,
	/**
	* Bulk memory used for output and temporary image buffers
	*/
	eCTJPEGBulkMemory	=	1
};

#pragma pack(8)
/**
* This structure provides various options needed for decoding image.
* \see CTJPEGDecodeOptionsInitDefault
*/
struct CT_JPEGLIB_EXPORT CTJPEGDecodeOptions
{
	/** Final ImageWidth = ImageWidth / resizeWidthRatio.
	* If resizeWidthRatio is 1, the image of original width is returned.
	* If resizeWidthRatio is 8, the a tumbnail of image with 1/8th width is returned.
	* Default is 1. Currently 1 and 8 are the only supported ratio.
	*/
	CTJPEGUint8		resizeWidthRatio;

	/** Final ImageHeight = ImageHeight / resizeHeightRatio.
	* If resizeHeightRatio is 1, the image of original height is returned.
	* If resizeHeightRatio is 8, the a tumbnail of image with 1/8th height is returned.
	* Default is 1. Currently 1 and 8 are the only supported ratio.
	*/
	CTJPEGUint8		resizeHeightRatio;
};
typedef struct CTJPEGDecodeOptions CTJPEGDecodeOptions;
#pragma pack()

/** Function to initialize CTJPEGDecodeOptions to default values.
* \param options CTJPEGDecodeOptions structure that needs to be filled.
* \return If options is NULL, then eCTJPEGErrorInvalidArgument is returned. 
* On Success, eCTJPEGErrorNone is returned.
*/
CTJPEGError CT_JPEGLIB_EXPORT CTJPEGDecodeOptionsInitDefault(CTJPEGDecodeOptions * options);

/**
* Encoding Mode e.g. Baseline, Extended, Progressive etc
*/
enum CTJPEGEncodeMode
{
	/**
	* Default mode for encoding in CTJPEG library. Baseline is used as default.
	*/	
	eCTJPEGDefaultEncodeMode	= 0,

	/**
	* Baseline. Most used mode of encoding.
	*/
	eCTJPEGEncodeBaseline		= 1,

	/**
	* Extended. Gives better file size than baseline.
	*/
	eCTJPEGEncodeExtended	= 2,

	/**
	* Progressive. Suited for creating jpegs for low speed internet connection.
	* Generic option. While endcoding, it this option is set then it will be 
	* treated as eCTJPEGEncodeProgressive_3Scans
	*/
	eCTJPEGEncodeProgressive	= 3,

	/**
	* Progressive. Suited for creating jpegs for low speed internet connection.
	* Special type of progressive.
	* Scans: DC, [1..5], [6..63]
	* Number of Scans: 3
	*/
	eCTJPEGEncodeProgressive_3Scans	= 4,

	/**
	* Progressive. Suited for creating jpegs for low speed internet connection.
	* Special type of progressive.
	* Scans: DC, [1..5]{Al=1}, [6..63]{Al=1}, [1..63]{Al=0}
	* Number of Scans: 4
	*/
	eCTJPEGEncodeProgressive_4Scans	= 5,

		/**
	* Progressive. Suited for creating jpegs for low speed internet connection.
	* Special type of progressive.
	* Scans: DC, [1..2], [3..5], [6..14], [15..63]
	* Number of Scans: 5
	*/
	eCTJPEGEncodeProgressive_5Scans	= 6,

		/**
	* Progressive. Suited for creating jpegs for low speed internet connection.
	* Special type of progressive.
	* Scans: DC{Al=1}, [1..5]{AL=2}, [6..63]{Al=2}, [1..63]{Al=1}, DC{AL=0}, [1..63]{Al=0}
	* Number of Scans: 6
	*/
	eCTJPEGEncodeProgressive_6Scans	= 7,

	eCTJPEGEncodeModeMaxValue = CT_JPEG_MAX_INT
};

/**
* up-sampling method to be used in CTJPEG decoder.
*/

enum CTJPEGUpsamplingMethod
{
	eCTJPEG_UPSAMPLING_AVG = 211,
	eCTJPEG_UPSAMPLING_WEIGHT_AVG = 212,
	eCTJPEG_UPSAMPLING_REPLICATE = 0
};
/**
* Encoding method to be used. CTJPEG encoder provides methods of encoding jpeg.
*/
enum CTJPEGEncodeMethod
{
	/**
	* Default method used. eCTJPEGEncodeFastHuffman is used as default.
	*/
	eCTJPEGDefaultEncodeMethod	= 0,
	/**
	* This option yields faster encoding but images of less optimal size.
	* Note that quality remains same.
	*/
	eCTJPEGEncodeFastHuffman		= 1,
	/**
	* This option yields images of optimal size but is a bit slower.
	*/
	eCTJPEGEncodeOptimalHuffman	= 2,
	/**
	* This option uses pre-computed huffman table and is used in encoding tile by tile.
	*/
	eCTJPEGEncodePreComputedHuffman = 3,
	
	eCTJPEGEncodeMethodMaxValue	= CT_JPEG_MAX_INT
};

/**
* Quality of image to be encoded
*/
enum CTJPEGQuality
{
	eCTJPEGQuality0		= 0,
	eCTJPEGQuality1		= 1,
	eCTJPEGQuality2		= 2,
	eCTJPEGQuality3		= 3,
	eCTJPEGQuality4		= 4,
	eCTJPEGQuality5		= 5,
	eCTJPEGQuality6		= 6,
	eCTJPEGQuality7		= 7,
	eCTJPEGQuality8		= 8,
	eCTJPEGQuality9		= 9,
	eCTJPEGQuality10		= 10,
	eCTJPEGQuality11		= 11,
	eCTJPEGQuality12		= 12,

	/**
	* Custom Quality. In this case, quality is determined by
	* the quantization tables and sampling factors provided by user.
	*/
	eCTJPEGCustomQuality	= 101,
	/**
	* Minimum Quality
	*/
	eCTJPEGMinQuality		= eCTJPEGQuality0,
	/**
	* Default Quality. Quality number 8 is used as default.
	*/
	eCTJPEGDefaultQuality	= eCTJPEGQuality8,
	/**
	* Maximum Quality.
	*/
	eCTJPEGMaximumQuality	= eCTJPEGQuality12,

	eCTJPEGQualityMaxValue	= CT_JPEG_MAX_INT
};


#pragma pack(8)
/**
* This structure provides various options needed for defining custom quality.
* \see CTJPEGEncodeOptionsInitDefault
*/
struct CT_JPEGLIB_EXPORT CTJPEGQualityParams
{
	/** Quantization table for each component. Each quantization table contains 64 entries */
	CTJPEGUint16	quantizationTables[CT_JPEG_MAX_COMPONENTS][64];

	/** Horizontal sampling factor for each component. Valid values are from 1 to 4. */
	CTJPEGUint8		horzSamplingFactors[CT_JPEG_MAX_COMPONENTS];

	/** Vertical sampling factor for each component. Valid values are from 1 to 4. */
	CTJPEGUint8		vertSamplingFactors[CT_JPEG_MAX_COMPONENTS];

	/** Which DQT is for which component */
	CTJPEGUint8		dqtIndex[CT_JPEG_MAX_COMPONENTS];

	/** Number of valid components */
	CTJPEGUint8		numberOfComponents;
};
typedef struct CTJPEGQualityParams CTJPEGQualityParams;
#pragma pack()


/*
* This enum CTJPEGPreEncodeTasks is used to set various tasks at the Encoder
* the Encoder can be used for 
	1. estimating the jpeg file size before the actual encode,
	2. generating preview of the jpeg before the actual encode, and 
	3. the actual encode
*/
enum CTJPEGEncodeTasks
{
	/* eCTJPEGEstimateSize - to estimate the size of the jpeg encoded*/
	eCTJPEGEstimateSize = 0x0001,
	/* eCTJPEGEncode - for actual encode*/
	eCTJPEGEncode = 0x0002,
	/* eCTJPEGGeneratePreview - to generate a raster preview of the jpeg encoded*/
	eCTJPEGGeneratePreview = 0x0004

	/*NOTE */
	/* if set both eCTJPEGEncode and eCTJPEGEstimateSize, it ignores eCTJPEGEncode*/
};

#pragma pack(8)
/**
* This structure provides various options needed for encoding image.
* \see CTJPEGEncodeOptionsInitDefault
*/
struct CT_JPEGLIB_EXPORT CTJPEGEncodeOptions
{
	/** Quality */
	CTJPEGQuality		quality;

	/** Quality Params. If quality is set to eCTJPEGCustomQuality, 
	 *  then these custom parameters will be used. */
	CTJPEGQualityParams customQualityParams;

	/** Encode Mode */
	CTJPEGEncodeMode	mode;	

	/** Implemenation method */
	CTJPEGEncodeMethod	method;
};
typedef struct CTJPEGEncodeOptions CTJPEGEncodeOptions;
#pragma pack()

/** Function to initialize CTJPEGEncodeOptions to default values.
* \param options CTJPEGEncodeOptions structure that needs to be filled.
* \return If options is NULL, then eCTJPEGErrorInvalidArgument is returned. 
* On Success, eCTJPEGErrorNone is returned.
*/
CTJPEGError CT_JPEGLIB_EXPORT CTJPEGEncodeOptionsInitDefault(CTJPEGEncodeOptions * options);

#pragma pack(8)
/**
* \This structure provides information about the encoded params, retrieved by parsing the JPEG image.
* \Encoded params give you information like the quantizer table, sampling factors and also the encoding mode ( progressive or baseline etc.) 
* \of the image being decoded.
* \
*/
struct CT_JPEGLIB_EXPORT CTJPEGEncodedParams
{
	/** Quality Params. */
	CTJPEGQualityParams qualityParams;

	/** Encode Mode */
	CTJPEGEncodeMode	mode;	

};
typedef struct CTJPEGEncodedParams CTJPEGEncodedParams;
#pragma pack()

#pragma pack(8)
/**
* This structure is used to specify a rectangular area of Image.
*/
struct CT_JPEGLIB_EXPORT CTJPEGRect
{
	/** Starting row number for image area*/
	CTJPEGUint16 rowStart;
	/** Starting column number for image area*/
	CTJPEGUint16 colStart;
	/** Number of rows in the area*/
	CTJPEGUint16 nRows;
	/** Number of columns in the area */
	CTJPEGUint16 nCols;

};
typedef struct CTJPEGRect CTJPEGRect;
#pragma pack()

#pragma pack(8)
/**
* Header part of CTJPEG image. It contains information like height, width, colorspace of the image.
*/
struct CT_JPEGLIB_EXPORT CTJPEGHeader
{
	/**
	* Width of image
	*/
	CTJPEGUint16		imageWidth;
	/**
	* Height of image
	*/
	CTJPEGUint16		imageHeight;
	/**
	* Number of components e.g. for RGB, it is 3.
	*/
	CTJPEGInt8		numberOfComponents;
	/**
	* ColorSpace
	*/
	CTJPEGColorSpace	colorSpace;
};
typedef struct CTJPEGHeader CTJPEGHeader;
#pragma pack()

#pragma pack(8)
/**
This structure holds CTJPEG data.
*/
struct CT_JPEGLIB_EXPORT CTJPEGData
{
	/**
	* Buffer containing data
	*/
	CTJPEGUint8 *data;
	/**
	* length of the buffer
	*/
	CTJPEGUint32 dataLen;
};
typedef struct CTJPEGData CTJPEGData;
#pragma pack()

#pragma pack(8)
/** Metadata embedded in CTJPEG image. Each 'App' tag in CTJPEG image corresponds to separate Metadata struct.*/
struct CT_JPEGLIB_EXPORT CTJPEGMetaData
{
	/** Buffer containing application meta data. */
	CTJPEGUint8 * data;
	/** Length of the application data */
	CTJPEGUint16 length;
	/** Application Tag. This tag indicates the type of metadata. */
	CTJPEGUint8 tag;
};
#pragma pack()

//struct CTJPEGMetaDataList
//{
//	CTJPEGMetaData metaData;
//	CTJPEGMetaDataList *next;
//};

#pragma pack(8)
/**
* This structure holds the raw image content.
*/
struct CT_JPEGLIB_EXPORT CTJPEGImageContent
{
	/** It is array of pointers pointing to buffer data for each component in the decoded image. 
	 *  For GrayScale color, componentData[0] refers to Gray component. <br>
	 *  For RGB color, componentData[0], componentData[1] and componentData[2] refer to R, G and B respectively.<br>
	 *  For LAB color, componentData[0], componentData[1] and componentData[2] refer to L, A and B respectively.<br>
	 *  For CMYK color, componentData[0], componentData[1], componentData[2] and componentData[3] refer to C,M,Y,K respectively.<br>
	 *  In other words,
	 *  componentData[0] refers to Gray, R, L or C component based on output colorspace.<br>
	 *  componentData[1] refers to       G, A or M component based on output colorspace.<br>
	 *  componentData[2] refers to       B, B or Y component based on output colorspace.<br>
	 *  componentData[3] refers to		         K component based on output colorspace.<br>
	 */
	CTJPEGUint8 *componentData[CT_JPEG_MAX_COMPONENTS];

	/** Offset that needs to be added to get to next column of the same component.
	 * For planar data, this value would be 1.
	 * For interleaved data, this value would be numberOfComponents.
	 */
	CTJPEGUint32 colOffset;

	/** Offset that needs to be added to get to next row of the same component.
	 * For planar data, this value could be imageWidth.
	 * For interleaved data, this value could be (imageWidth * numberOfComponents)
	 */
	CTJPEGUint32 rowOffset;

	/** Number of components in the image e.g. for RGB image, it wil be 3. */
	CTJPEGUint8 numberOfComponents;
};
typedef struct CTJPEGImageContent CTJPEGImageContent;
#pragma pack()

///**
//* Initialize CTJPEGImageContent structure for interleaved image data
//* \param imageContent [out] imageContent structure that needs to be intiailized by this method call.
//* \param header [in] Image header to be used to get information like colorspace, imageWidth etc.
//* \param imageData [in]  Pointer to buffer containing interleaved raw image data.
//* \return If imageContent is NULL, then eCTJPEGErrorInvalidArgument is returned.
//* On success, eCTJPEGErrorNone is returned.
//*/
//CT_JPEGLIB_EXPORT CTJPEGError CTJPEGImageContentInitInterleaved(
//	CTJPEGImageContent *imageContent,
//	CTJPEGHeader header,
//	CTJPEGUint8 *imageData
//); 
//
///**
//* Initialize CTJPEGImageContent structure for planar image data
//* \param imageContent [out] imageContent structure that needs to be intiailized by this method call.
//* \param header [in] Image header to be used to get information like colorspace, imageWidth etc.
//* \param imageData [in] Pointer to buffer containing planar raw image data. 
//*                       ImageData[i] refers to raw image data for plane[i].
//* \return If imageContent is NULL, then eCTJPEGErrorInvalidArgument is returned.
//* On success, eCTJPEGErrorNone is returned.
//*/
//CT_JPEGLIB_EXPORT CTJPEGError CTJPEGImageContentInitPlanar(
//	CTJPEGImageContent *imageContent,
//	CTJPEGHeader header,
//	CTJPEGUint8 **imageData
//); 

#pragma pack(8)
/**
This structure holds CTJPEG task.
*/
struct CT_JPEGLIB_EXPORT CTJPEGTask
{
	/**
	* parameter to be passed to DoTask function
	*/
	void* params;
	/**
	* Function that needs to be executed by task manager
	*/
	CTJPEGBool8 (*DoTask) (void* context);
};

typedef struct CTJPEGTask CTJPEGTask;
#pragma pack()



#pragma pack(8)
/**
* This structure contains context object and callbacks needed for memory allocation 
* and deallocation by CTJPEG library. Context object provided in this structure by client 
* is passed as parameter to these callbacks.
*/
struct CT_JPEGLIB_EXPORT CTJPEGMemoryManager
{
	/** 
	* Context object provided in this structure by client is passed as parameter to these callbacks.
	*/
	void * context;

	/**
	* This callback method is used to allocate memory of given size.
	* \param context [in] Context object provided by client along with callbacks.
	* \param size [in] number of bytes to allocate.
	* \param memoryType [in] type of memory to allocate.
	* \return pointer of memory buffer allocated.
	*/
	void* (*Malloc)(
		void * context,
		size_t size,
		CTJPEGMemoryType memoryType
	);

	/** 
	* This callback method is used to free the allocated memory buffer.
	* \param context [in] Context object provided by client along with callbacks.
	* \param data [in] Pointer of memory buffer that needs to be freed.
	* \param memoryType [in] type of memory to allocate.
	*/
	void (*Free)(
		void * context,
		void * data,
		CTJPEGMemoryType memoryType
	); 
};
typedef struct CTJPEGMemoryManager CTJPEGMemoryManager;
#pragma pack()


#pragma pack(8)
/**
* This structure contains context object and callbacks needed for executing JPEG sub tasks. 
* Clients can provide their own thread pool implementation.
*/
struct CT_JPEGLIB_EXPORT CTJPEGTaskManager
{
	/** 
	* Context object provided in this structure by client is passed as parameter to these callbacks.
	*/
	void * context;

	/**
	* This callback method is used to initialize task manager
	* \param context [in] Context object provided by client along with callbacks.
	* \return true on success, false otherwise.
	*/
	CTJPEGBool8 (*Init)(
		void * context
	);

	/** 
	* This callback method is used to terminate task manager
	* \param context [in] Context object provided by client along with callbacks.
	* \return none.
	*/
	void (*Terminate)(
		void * context
	); 


	/**
	* This callback method is used to schedule tasks provided by CT JPEG library.
	* These tasks can be executed in parallel. This call can return immediately after 
	* scheduling the tasks (e.g. adding it in a queue)
	* \param context [in] Context object provided by client along with callbacks.
	* \param task [in] task to be executed
	* \return true on success, false otherwise.
	*/
	CTJPEGBool8 (*Schedule)(
		void * context,
		CTJPEGTask task
	);


	/**
	* This callback method is used to block the calling thread till the time all scheduled tasks
	* have been executed.
	* \param context [in] Context object provided by client along with callbacks.
	* \return true on success, false otherwise.
	*/
	CTJPEGBool8 (*Wait)(
		void * context
	);


	/**
	* This callback method is used to give the number of task queues present i.e. number of tasks that
	* could be executed in parallel. This could be same as number of threads in thread pool.
	* This information helps library to decide on how to best divide the tasks.
	* \param context [in] Context object provided by client along with callbacks.
\	* \return number of task queuese.
	*/
	CTJPEGUint32 (*GetTaskQueuesCount)(
		void * context
	);
};
typedef struct CTJPEGTaskManager CTJPEGTaskManager;
#pragma pack()

#pragma pack(8)
/**
* This structure contains context object and callbacks function to create the task manager 
* Clients can provide their own task manager implementation.
*/
struct CT_JPEGLIB_EXPORT CTJPEGTaskManagerBuilder
{
	/**
	* Context object provided in this structure by client is passed as parameter to these callbacks.
	*/
	void *context;
	
	/**
	* Function that creates the task manager
	*/
	CTJPEGTaskManager (*CreateTaskManager) (void *context);
};
typedef struct CTJPEGTaskManagerBuilder CTJPEGTaskManagerBuilder;
#pragma pack()

#pragma pack(8)
/**
* This structure contains context object and callbacks needed to read encoded CTJPEG stream. 
* Context object provided in this structure by client is passed as parameter to these callbacks.
* CTJPEG Decoder calls these callbacks to read more CTJPEG data on need basis.
*/
struct CT_JPEGLIB_EXPORT CTJPEGReader
{
	/** 
	* Context object provided in this structure by client is passed as parameter to these callbacks.
	*/
	void * context;

	/**
	* This callback method is used to skip forward by "offset" bytes given as parameter.
	* Next Read() call should return data by "offset" byes ahead of current data.
	* \param context [in] Context object provided by client along with callbacks.
	* \param offset [in] number of bytes to skip by.
	* \return false to indicate some error happened while skipping data otherwise true
	*/
	CTJPEGBool8 (*Skip)(
		void * context,
		CTJPEGUint32 offset
	);

	/** 
	* This callback method is used to read atmost "length" bytes from the CTJPEG stream.
	* \param context [in] Context object provided by client along with callbacks.
	* \param data [in] Pointer to buffer in which data needs to be filled.
	*                      Note that this buffer is pre-allocated by CTJPEG library.
	*                      Client should only copy data in this buffer. Client should
	*                      not try to delete/free this buffer.
	* \param bytesToRead [in] Maximum length of data to be read.
	* \param bytesRead [out] Actual number of bytes read.
	* \return false to indicate some error happened while reading data otherwise true
	*/
	CTJPEGBool8 (*Read)(
		void * context,
		CTJPEGUint8 * data,
		CTJPEGUint32 bytesToRead,
		CTJPEGUint32 *bytesRead
	); 

	/**
	* This callback method is used to indicate that this is the last call to CTJPEGReader.
	* Client should cleanup CTJPEGReader releated resources in this callback method.
	* \param context [in] Context object provided by client along with callbacks.
	*/
	void (*Close)(
		void * context
	); 
};
typedef struct CTJPEGReader CTJPEGReader;
#pragma pack()

#pragma pack(8)
/**
* This structure contains context object and callbacks needed to write encoded CTJPEG stream. 
* Context object provided in this structure by client is passed as parameter to these callbacks.
* CTJPEG Encoder calls these callbacks to write CTJPEG data on need basis as encoding continues.
*/
struct CT_JPEGLIB_EXPORT CTJPEGWriter
{
	/** 
	* Context object provided in this structure by client is passed as parameter to these callbacks.
	*/
	void * context;

	/** 
	* This callback method is used to write "length" bytes to the CTJPEG stream.
	* \param context [in] Context object provided by client along with callbacks.
	* \param data [in] Pointer to buffer in which data needs to be written.
	*                  Note that this buffer is pre-allocated by CTJPEG library.
	*                  Client should only copy data in this buffer. Client should
	*                  not try to delete/free this buffer.
	* \param bytesToWrite [in] Maximum length of data to be written.
	* \param bytesWritten [out] Actual number of bytes written. 
	* \return false to indicate some error happened while reading data otherwise true
	*/
	CTJPEGBool8 (*Write)(
		void * context,
		CTJPEGUint8 * data,
		CTJPEGUint32 bytesToWrite,
		CTJPEGUint32 *bytesWritten
	); 

	/**
	* This callback method is used to indicate that this is the last call to CTJPEGWriter.
	* Client should cleanup CTJPEGWriter releated resources in this callback method.
	* \param context [in] Context object provided by client along with callbacks.
	*/
	void (*Close)(
		void * context
	); 
};
typedef struct CTJPEGWriter CTJPEGWriter;
#pragma pack()

#pragma pack(8)
/**
* This structure contains warning related callbacks and context object. 
* If the CTJPEG image is not well-formed but still can be decoded, 
* then decoder calls these warning callbacks to ask whether to continue with decoding or not.
* Client can put in code to handle warning e.g. show warning message to users etc.
* Context object provided in this structure by client is passed as parameter to these callbacks.
*/
struct CT_JPEGLIB_EXPORT CTJPEGWarningHandler
{
	/** Context object provided in this structure by client is passed as parameter to these callbacks.
	*/
	void * context;

	/** If the CTJPEG image is not well-formed but still can be decoded, 
	* then decoder calls these warning callbacks to ask whether to continue with decoding or not.
	* \param context Context object provided by client along with callbacks
	* \param warning CTJPEG warning that occurred while decoding image. 
	* \return if false then decoder stops with warning as CTJPEGError, otherwise continues with decoding.
	*/
	CTJPEGBool8 (*ProcessWarning)(
		void * context,
		CTJPEGError warning
	);
};
#pragma pack()

#pragma pack(8)
/**
* This structure contains context object and metadata related callbacks. 
* Context object provided in this structure by client is passed as parameter to these callbacks.
* These callbacks are provided by client and are called by CTJPEG Decoder at the time of decoding metadata. 
*/
struct CT_JPEGLIB_EXPORT CTJPEGMetaDataHandler
{
	/**
	* This value is provided by client and is passed to all the metadata related callbacks by CTJPEG library.
	*/
	void * context;

	/**
	* This method is called by decoder when start of meta data is found.
	* \param context [in] Context value provided by client along with this callback method
	* \param tag [in] APPn tag of metaData found 
	* \param length [in] Length of metaData found
	* \param dataToBeFilled [out] Pointer to data buffer. Client needs to allocate space for 
	* data field of metadata of size not less than the length given as parameter. 
	* If on return, *(dataToBeFilled) points to NULL, then decoder will skip this marker and continue decoding.
	* Otherwise decoder will fill metaData information in this buffer.
	*/
	void (*InitMetaData)(
		void * context,
		CTJPEGUint8 tag,
		CTJPEGUint16 length,
		CTJPEGUint8 **dataToBeFilled
	);

	/**
	* This method is called by decoder after decoder has filled the metaData object 
	* provided by client in InitMetaData call. Client can process this metaData to 
	* extract relevant information and free data field of metaData.
	* \param context [in] Context value provided by client along with this callback method
	* \param tag [in] APPn tag of metaData found 
	* \param length [in] Length of metaData found
	* \param dataFilled [in] Pointer to data buffer allocated by client in last call to InitMetaData().
	* This data buffer is filled by decoder for client use. Client can free this data buffer in this call.
	* \param bSuccess This parameter indicates if metaData was successfully decoded or not. 
	* The value in metaData provided in this call should be used by client only when bSuccess is true.
	* If isDataValid is false, then client should just free the buffer allocated for metadata.
	*/
	void (*ProcessMetaData)(
		void * context,
		CTJPEGUint8 tag,
		CTJPEGUint16 length,
		CTJPEGUint8 *dataFilled,
		CTJPEGBool8 isDataValid
	);
};
typedef struct CTJPEGMetaDataHandler CTJPEGMetaDataHandler;
#pragma pack()

#pragma pack(8)
/**
* This structure contains context object and callbacks to give raw image content to decoder in chunks/tiles.
* Context object provided in this structure by client is passed as parameter to these callbacks.
* CTJPEG library processes raw image in image parts (called as tiles). It asks client to provide the buffer 
* for a particular region of image (tile). During encoding, encoder reads from this buffer to to get raw image data for that region
* and in turn uses it to create encoded CTJPEG stream.
*/
struct CT_JPEGLIB_EXPORT CTJPEGTiledContentReader
{
	/**
	* This value is provided by client and is passed to all the metadata related callbacks by CTJPEG library.
	*/
	void * context;

	/**
	* CTJPEG library processes raw image in image parts (called as tiles). In this callback, 
	* the maximum size of tile/image part per request asked by library is passed as parameters.
	* Client can use these parameters to allocate space for fututre requests.
	* \param context [in] Context value provided by client along with this callback method
	* \param nMaxRowsPerRequest [in] Maximum number of rows that can be asked by library in any InitContent() request.
	* \param nMaxColsPerRequest [in] Maximum number of columns that can be asked by library in any InitContent() request.
	* \return If true then continue with operation else stop.
	*/
	CTJPEGBool8 (*Init) (
		void * context,
		CTJPEGUint16 nMaxRowsPerRequest,
		CTJPEGUint16 nMaxColsPerRequest
	);

	/**
	* In this callback, client is asked to allocate and initialize buffers pointing to raw image data
	* for the given region.
	* \param context [in] Context value provided by client along with this callback method.
	* \param tileArea [in] Region of image for which imageContent needs to be initialized.
	* \param content [out] Image content structure that has buffer pointers initialized 
	*                              to start of the given region. Note that client must allocate space
	*                              for imageContent->componentData buffers. During decoding, decoder 
	*	                           fills this buffer with the decoded raw image data. During encoding, 
	*                              encoder reads from this buffer to to get raw image data for that region
	*                              and in turn uses it to create encoded CTJPEG stream.
	* \return If true then continue with operation else stop.
	*/
	CTJPEGBool8 (*InitContent) (
		void * context,
		CTJPEGRect tileArea,
		CTJPEGImageContent *content
	);

	/**
	* This callback method is used to indicate that this is the last call to CTJPEGTiledContentReader.
	* Client should cleanup CTJPEGTiledContentReader releated resources in this callback method.
	* \param context [in] Context object provided by client along with callbacks.
	*/
	void (*Close) (
		void * context
	);
};
typedef struct CTJPEGTiledContentReader CTJPEGTiledContentReader;
#pragma pack()


#pragma pack(8)
/**
* This structure contains context object and callbacks to write decoded raw image content.
* Context object provided in this structure by client is passed as parameter to these callbacks.
* CTJPEG library processes raw image in image parts (called as tiles). It asks client to provide the buffer 
* for a particular region of image (tile). During decoding, decoder fills this buffer with the decoded 
* raw image data.
*/
struct CT_JPEGLIB_EXPORT CTJPEGTiledContentWriter
{
	/**
	* This value is provided by client and is passed to all the metadata related callbacks by CTJPEG library.
	*/
	void * context;

	/**
	* CTJPEG library processes raw image in image parts (called as tiles). In this callback, 
	* the maximum size of tile/image part per request asked by library is passed as parameters.
	* Client can use these parameters to allocate space for fututre requests.
	* \param context [in] Context value provided by client along with this callback method
	* \param nMaxRowsPerRequest [in] Maximum number of rows that can be asked by library in any InitContent() request.
	* \param nMaxColsPerRequest [in] Maximum number of columns that can be asked by library in any InitContent() request.
	* \return If true then continue with operation else stop.
	*/
	CTJPEGBool8 (*Init) (
		void * context,
		CTJPEGUint16 nMaxRowsPerRequest,
		CTJPEGUint16 nMaxColsPerRequest
	);


	/**
	* In this callback, client is asked to allocate and initialize buffers pointing to raw image data
	* for the given region.
	* \param context [in] Context value provided by client along with this callback method.
	* \param tileArea [in] Region of image for which imageContent needs to be initialized.
	* \param content [out] Image content structure that has buffer pointers initialized 
	*                              to start of the given region. Note that client must allocate space
	*                              for imageContent->componentData buffers. During decoding, decoder 
	*	                           fills this buffer with the decoded raw image data. During encoding, 
	*                              encoder reads from this buffer to to get raw image data for that region
	*                              and in turn uses it to create encoded CTJPEG stream.
	* \return If true then continue with operation else stop.
	*/
	CTJPEGBool8 (*InitContent) (
		void * context,
		CTJPEGRect tileArea,
		CTJPEGImageContent *content
	);

	/**
	* CTJPEG Decoder calls this method to indicate if image content has been successfully filled or not.
	* If yes, then client can process this content e.g. display it. 
	* \param context [in] Context value provided by client along with this callback method.
	* \param tileArea [in] Region of image for which imageContent is initialized.
	* \param contentFilled [in] Image content structure that has buffer pointers initialized 
	*                         to start of the given region. Note that space for data buffers of
	*                         imageConent were allocated by client. This space is used by decoder
	*                         to fill raw image data and pass it on to client. This space should not be
	*                         freed by client before Close() call.
	* \return If true then continue with operation else stop.
	*/
	CTJPEGBool8 (*ProcessContent) (
		void * context,
		CTJPEGRect tileArea,
		CTJPEGImageContent contentFilled
	);

	/**
	* This callback method is used to indicate that this is the last call to CTJPEGTiledContentWriter.
	* Client should cleanup CTJPEGTiledContentWriter releated resources in this callback method.
	* \param context [in] Context object provided by client along with callbacks.
	*/
	void (*Close) (
		void * context
	);
};
typedef struct CTJPEGTiledContentWriter CTJPEGTiledContentWriter;
#pragma pack()


/** \defgroup CTJPEGDecoder
*/

/** \ingroup CTJPEGDecoder
*/
typedef struct CTJPEGDecoder * CTJPEGDecoderRef;

/** \ingroup CTJPEGDecoder
* This method is used to create CTJPEG decoder to decode from buffer containg encoded jpeg image.
* \param streamData [in] Buffer containing encoded jpeg image data.
* \param decodeOptions [in] Decode options to be used for decoding
* \param bSkipWarning [in] Flag for skipping warnings. If the CTJPEG image is not well-formed 
*                     but still can be decoded then decoder raises warning. If this flag is true 
*                     then decoder will ignore warnings and continue with decoding. Otherwise, it
*                     will stop decoding and return with warning as error.
* \param warningHandler [in] Handler to handle decode warnings. If handler is not NULL and bSkipWarning is
*                     false then callbacks of warningHandler are called by decoder on warning. Otherwise
*                     warningHandler is not used.
* \param decoderRef [out] Reference to decoder object. On successful return, this value is initialized with 
*                      reference to decoder object. This reference can be used for other decoder operations.
* \return CTJPEGErrorNone on success.
*/
CTJPEGError CT_JPEGLIB_EXPORT CTJPEGDecoderCreateUsingData(
	CTJPEGData streamData,
	CTJPEGDecodeOptions decodeOptions,
	CTJPEGBool8 bSkipWarning,
	CTJPEGWarningHandler *warningHandler,
	CTJPEGDecoderRef *decoderRef
);

/** \ingroup CTJPEGDecoder
* This method is used to create CTJPEG decoder. Note that after use, CTJPEGDecoderRelease() needs be called to 
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
* \param decoderRef [out] Reference to decoder object. On successful return, this value is initialized with 
*                      reference to decoder object. This reference can be used for other decoder operations.
* \return CTJPEGErrorNone on success.
* \see CTJPEGDecoderRelease
*/
CTJPEGError CT_JPEGLIB_EXPORT CTJPEGDecoderCreate(
	CTJPEGReader reader,
	CTJPEGDecodeOptions decodeOptions,
	CTJPEGBool8 bSkipWarning,
	CTJPEGWarningHandler *warningHandler,
	CTJPEGDecoderRef *decoderRef
);

/** \ingroup CTJPEGDecoder
* This method is used to create CTJPEG decoder. A decoder created using this API uses an uncached reader, thus 
* could be slower. Hence, only create a decoder using this API if you need to precisely read the number of bytes
* in a compressed JPEG image.
* Note that after use, CTJPEGDecoderRelease() needs be called to 
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
* \param decoderRef [out] Reference to decoder object. On successful return, this value is initialized with 
*                      reference to decoder object. This reference can be used for other decoder operations.
* \return CTJPEGErrorNone on success.
* \see CTJPEGDecoderRelease
*/

CTJPEGError CT_JPEGLIB_EXPORT CTJPEGDecoderCreateUsingUncachedReader(
	CTJPEGReader reader,
	CTJPEGDecodeOptions decodeOptions,
	CTJPEGBool8 bSkipWarning,
	CTJPEGWarningHandler *warningHandler,
	CTJPEGDecoderRef *decoderRef
);

/** \ingroup CTJPEGDecoder
* The CTJPEGReader passed by the client while creating a decoder is cached by CTJPEG internally. 
* The cache size is pre-defined. CTJPEG requests the client for data in multiples of this cache size. If the length of
* JPEG file/stream is less than the amount of data requested by CTJPEG ( which is equal to cache size ) then the client 
* should only provide data equal to the length of the JPEG stream. If the client has no means to determine the length of 
* the JPEG stream and provides more data (say equal to the JPEG cache size) to the JPEG library, calling 
* CTJPEGGetUnusedCacheBytesCount tells the client how much data read by the library was NOT used for decoding the JPEG. 
* The client can then use this information to reset his/her stream correctly.
* \param decoderRef [in] Reference to decoder object
* \return number of unused bytes
*/

CTJPEGUint32 CT_JPEGLIB_EXPORT CTJPEGGetUnusedCacheBytesCount(
	CTJPEGDecoderRef decoderRef
);

/** \ingroup CTJPEGDecoder
* This method is called to release resources held by CTJPEG decoder. This needs to be called for
* every CTJPEGDecoderCreate...() call. Otherwise it will cause memory leak.
* \see CTJPEGDecoderCreate, CTJPEGDecoderCreateUsingData
*/
void CT_JPEGLIB_EXPORT CTJPEGDecoderRelease(
	CTJPEGDecoderRef decoderRef
);

/** \ingroup CTJPEGDecoder
* This method is used to parse and get header information from the encoded image.
* \param decoderRef [in] Reference to decoder object
* \param metaDataHandler [in] Handler to handle metaData callbacks. If metaDataHandler is not NULL, 
*                             then the callbacks specified in metaDataHandler are called by decoder
*                             at the time of parsing metadata.
* \param header [out] Pointer to structure to hold header information. If header is not NULL, 
*                        then decoder fills header with information parsed from input jpeg image.
* \return CTJPEGErrorNone on success.
*/
CTJPEGError CT_JPEGLIB_EXPORT CTJPEGDecoderReadHeader(
	CTJPEGDecoderRef decoderRef,
	CTJPEGMetaDataHandler * metaDataHandler,
	CTJPEGHeader * header);

/** \ingroup CTJPEGDecoder
* This method is used to parse and get header information from the encoded image the encoded params like 
* quantization tables, sampling factors etc.
* \param decoderRef [in] Reference to decoder object
* \param metaDataHandler [in] Handler to handle metaData callbacks. If metaDataHandler is not NULL, 
*                             then the callbacks specified in metaDataHandler are called by decoder
*                             at the time of parsing metadata.
* \param header [out] Pointer to structure to hold header information. If header is not NULL, 
*                        then decoder fills header with information parsed from input jpeg image.
* \param encodedParams [out] Pointer to structure to hold the JPEG Encoded Params. If encodedParams is not
                         NULL, then the decoder fills encodedParams with information parsed from input jpeg image.
* \return CTJPEGErrorNone on success.
*/
CTJPEGError CT_JPEGLIB_EXPORT CTJPEGDecoderReadHeaderAndEncodedParams(
	CTJPEGDecoderRef decoderRef,
	CTJPEGMetaDataHandler * metaDataHandler,
	CTJPEGHeader * header,
	CTJPEGEncodedParams * encodedParams
);

/** \ingroup CTJPEGDecoder
* This method is used to decode raw image content from the encoded image.
* \param decoderRef [in] Reference to decoder object
* \param colorSpace [in] Desired colorSpace of decoded raw image 
* \param contentWriter [in] Handler to handle content write callbacks. If contentWriter is not NULL, 
*                             then the callbacks specified in contentWriter are called by decoder
*                             at the time of decoding image content.
* \return CTJPEGErrorNone on success.
*/
CTJPEGError CT_JPEGLIB_EXPORT CTJPEGDecoderReadTiledContent(
	CTJPEGDecoderRef decoderRef,
	CTJPEGColorSpace colorSpace,
	CTJPEGTiledContentWriter contentWriter
);

/** \ingroup CTJPEGDecoder
* This method is used to decode raw image content from the encoded image and store it in 
* the given imageContent structure.
* \param decoderRef [in] Reference to decoder object
* \param colorSpace [in] Desired colorSpace of decoded raw image 
* \param imageContentToBeFilled [in] ImageContent that will be filled by decoder.
*                                    Client should allocate space and initialize componentData buffers
*                                    of this structure to hold decoded raw data for complete image.
* \return CTJPEGErrorNone on success.
*/
CTJPEGError CT_JPEGLIB_EXPORT CTJPEGDecoderReadContent(
	CTJPEGDecoderRef decoderRef,
	CTJPEGColorSpace colorSpace,
	CTJPEGImageContent imageContentToBeFilled
);

/** \ingroup CTJPEGDecoder
* This method is used to set color space of output raw data 
* \param decoderRef [in] Reference to decoder object
* \param outColorSpace [in] Desired colorSpace of decoded raw image 
* \return CTJPEGErrorNone on success.
*/
CTJPEGError CT_JPEGLIB_EXPORT CTJPEGDecoderSetOutColorSpace(
	CTJPEGDecoderRef decoderRef,
	CTJPEGColorSpace outColorSpace
);

/** \ingroup CTJPEGDecoder
* This method is used to set up-sampling method of output raw data 
* \param decoderRef [in] Reference to decoder object
* \param upSamplingMethod [in] desired up-sampling method of decoded raw image 
* \return CTJPEGErrorNone on success.
*/
CTJPEGError CT_JPEGLIB_EXPORT CTJPEGDecoderSetUpsamplingMethod(
	CTJPEGDecoderRef decoderRef,
	CTJPEGUpsamplingMethod upSamplingMethod
);


/** \ingroup CTJPEGDecoder
* This method is used to get information about next tile of raw image content to be decoded.
* \param decoderRef [in]  Reference to decoder object
* \param tileArea   [out] Area of next tile that will be decoded
* \return CTJPEGErrorNone on success.
*/
CTJPEGError CT_JPEGLIB_EXPORT CTJPEGDecoderGetNextTileInfo(
	CTJPEGDecoderRef	decoderRef,
	CTJPEGRect*			tileArea
);

/** \ingroup CTJPEGDecoder
* This method is check if there is some tile left to for decoding.
* \param decoderRef [in] Reference to decoder object
* \param hasMoreTiles [in] true if there are more tiles to be decoded, false otherwise.
* \return CTJPEGErrorNone on success.
*/
CTJPEGError CT_JPEGLIB_EXPORT CTJPEGDecoderHasMoreTiles(
	CTJPEGDecoderRef	decoderRef,
	CTJPEGBool8*		hasMoreTiles
);

/** \ingroup CTJPEGDecoder
* This method is used to decode next tile of raw image content from the encoded image and store it in 
* the given imageContent structure.
* \param decoderRef [in] Reference to decoder object
* \param imageContentToBeFilled [in] ImageContent that will be filled by decoder.
*                                    Client should allocate space and initialize componentData buffers
*                                    of this structure to hold decoded raw data for next tile.
* \return CTJPEGErrorNone on success.
*/
CTJPEGError CT_JPEGLIB_EXPORT CTJPEGDecoderReadNextTile(
	CTJPEGDecoderRef decoderRef,
	CTJPEGImageContent contentToBeFilled
);

/** \ingroup CTJPEGEncoder
* Reference to CTJPEGEncoder.
*/
typedef struct CTJPEGEncoder * CTJPEGEncoderRef;

/** \defgroup CTJPEGEncoder
*/

/** \ingroup CTJPEGEncoder
* This method is used to create CTJPEG encoder to encode image.
* \param writer [in] Structure to provide callbacks to write encoded jpeg data.
* \param encodeOptions [in] Options to be used for encoding.
* \param encoderRef [out] Reference to encoder object. On successful return, 
                    this value is initialized with reference to encoder object. 
					This reference can be used for other encoder operations.
* \return CTJPEGErrorNone on success.
*/

/**
Depending up on the API used the encode options supported can vary.  
The following combinations are supported.

													Tiling API (if you use CTJPEGEncoderWriteNextTileContent)		
				
			eCTJPEGEncodePreComputedHuffman	eCTJPEGEncodeOptimalHuffman	eCTJPEGEncodeFastHuffman	default
				
Baseline				Yes								Yes						No					eCTJPEGEncodePreComputedHuffman
Progressive				No								Yes						No					eCTJPEGEncodeOptimalHuffman
				
				
													Non-tiling API (if you use CTJPEGEncoderWriteContent)		
				
			eCTJPEGEncodePreComputedHuffman	eCTJPEGEncodeOptimalHuffman	eCTJPEGEncodeFastHuffman	default
				
Baseline				Yes								Yes						Yes					eCTJPEGEncodeFastHuffman
Progressive				No								Yes						No					eCTJPEGEncodeOptimalHuffman


The "Extended" encoding is not yet tested and may not function properly.

*/

CTJPEGError CTJPEGEncoderCreate(
	CTJPEGWriter writer,
	CTJPEGEncodeOptions encodeOptions,
    CTJPEGEncoderRef *encoderRef
);

/** \ingroup CTJPEGEncoder
* This method is called to release resources held by CTJPEG encoder. 
* This needs to be called for every CTJPEGEncoderCreate...() call. Otherwise it will cause memory leak. 
* \param encoderRef [in] Reference to encoder object. On successful return, 
                    this value is initialized with reference to encoder object. 
					This reference can be used for other encoder operations.
* See also:
* CTJPEGEncoderCreate, CTJPEGEncoderCreateUsingFile
*/
void CT_JPEGLIB_EXPORT CTJPEGEncoderRelease(
	CTJPEGEncoderRef encoderRef
);

/** \ingroup CTJPEGEncoder
* This method is used to write meta data in encoded jpeg.
* This method should be called before writing content. Otherwise eCTJPEGErrorCannotWriteMetaDataAfterContent
* error will be returned.
* \param encoderRef [in] Reference to encoder object
* \param metaData [in] Structure containing MetaData information.
* \return CTJPEGErrorNone on success.
*/
CTJPEGError CT_JPEGLIB_EXPORT CTJPEGEncoderWriteMetaData(
	CTJPEGEncoderRef encoderRef,
	CTJPEGMetaData metaData
);


/** \ingroup CTJPEGEncoder
* This method is used to write more than one meta data tags in bulk in encoded jpeg.
* This method should be called before writing content. Otherwise eCTJPEGErrorCannotWriteMetaDataAfterContent.
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
* \return CTJPEGErrorNone on success.
*/
CTJPEGError CT_JPEGLIB_EXPORT CTJPEGEncoderBulkWriteMetaData(
	CTJPEGEncoderRef encoderRef,
	CTJPEGUint8	*metaDataAsBlob,
	CTJPEGUint32	sizeOfMetaDataAsBlob
);

/** \ingroup CTJPEGEncoder
* This method is used to write image content in encoded jpeg.
* \param encoderRef [in] Reference to encoder object
* \param header [in] Structure containing header information.
* \param contentReader [in] Structure containing content read related callbacks.
* \return CTJPEGErrorNone on success.
*/
CTJPEGError CT_JPEGLIB_EXPORT CTJPEGEncoderWriteTiledContent(
	CTJPEGEncoderRef encoderRef,
	CTJPEGHeader header,
	CTJPEGTiledContentReader contentReader
);

/** \ingroup CTJPEGEncoder
* This method is used to write image content in encoded jpeg.
* \param encoderRef [in] Reference to encoder object
* \param header [in] Structure containing header information.
* \param imageContent [in] Structure containing raw content for complete image.
* \return CTJPEGErrorNone on success.
*/
CTJPEGError CT_JPEGLIB_EXPORT CTJPEGEncoderWriteContent(
	CTJPEGEncoderRef encoderRef,
	CTJPEGHeader header,
	CTJPEGImageContent imageContent
);


/** \ingroup CTJPEGEncoder
* This method is used to write image header in encoded jpeg.
* \param encoderRef [in] Reference to encoder object
* \param header [in] Structure containing header information.
* \return CTJPEGErrorNone on success.
*/
CTJPEGError CT_JPEGLIB_EXPORT CTJPEGEncoderWriteHeader(
	CTJPEGEncoderRef encoderRef,
	CTJPEGHeader header
);

/** \ingroup CTJPEGEncoder
* This method is used to encode next tile of raw image content given in imageContent
* \param encoderRef [in] Reference to decoder object
* \param tileArea   [out] Area of next tile that will be encoded
* \param contentToBeEncoded [in] ImageContent that will be filled by decoder.
*                                    Client should allocate space and initialize componentData buffers
*                                    of this structure to hold decoded raw data for next tile.
* \return CTJPEGErrorNone on success.
*/
CTJPEGError CT_JPEGLIB_EXPORT CTJPEGEncoderWriteNextTileContent(
	CTJPEGEncoderRef			encoderRef,
	CTJPEGRect			tileArea,
	CTJPEGImageContent	contentToBeEncoded
	);


/**
* Angle of rotation
* \ingroup CTJPEGRotater
*/
enum CTJPEGRotateAngle
{
	/**
	* Do not rotate
	*/
	eCTJPEGNoRotate			= 0,
	/**
	* Rotate clockwise by 90 degrees
	*/
	eCTJPEGClockwise90		= 1,
	/**
	* Rotate clockwise by 180 degrees
	*/
	eCTJPEGFlip				= 2,
	/**
	* Rotate anti-clockwise by 90 degrees
	*/
	eCTJPEGAntiClockwise90	= 3,

	eCTJPEGRotateAngleMaxValue	= CT_JPEG_MAX_INT
};

#pragma pack(8)
/**
* Rotation options
* \ingroup CTJPEGRotater
*/
struct CT_JPEGLIB_EXPORT CTJPEGRotateOptions
{
	/**
	* Input jpeg image is rotated by the specified angle.
	* Default is eCTJPEGNoRotate.
	*/
	CTJPEGRotateAngle angle;

	/**
	* Encode options to use for encoding rotated image.
	*/
	CTJPEGEncodeOptions encodeOptions;

	/**
	* To add metadata from input image to output image or not.
	*/
	CTJPEGBool8 retainMetaData;
};
#pragma pack()


/** Function to initialize CTJPEGRotateOptions to default values.
* \param options CTJPEGRotateOptions structure that needs to be filled.
* \return If options is NULL, then eCTJPEGErrorInvalidArgument is returned. 
* On Success, eCTJPEGErrorNone is returned.
*/
CTJPEGError CT_JPEGLIB_EXPORT CTJPEGRotateOptionsInitDefault(CTJPEGRotateOptions * options);

/** \ingroup CTJPEGRotater
*/
CTJPEGError CT_JPEGLIB_EXPORT CTJPEGRotateImage(
  CTJPEGReader reader,
  CTJPEGWriter outputStream,
  CTJPEGRotateOptions rotateOptions
);

/* To set the encode tasks*/
/* The task coulde be any of the following */
/* Encode - eCTJPEGEncode */
/* Generate preview - eCTJPEGGeneratePreview*/
/* Estimate size - eCTJPEGEstimateSize*/
/* Encode and Generate preview - (eCTJPEGEncode|eCTJPEGGeneratePreview)*/
/* Estimate size and Generate preview - (eCTJPEGEstimateSize|eCTJPEGGeneratePreview)*/
CTJPEGError CT_JPEGLIB_EXPORT CTJPEGEncoderSetTask(
	CTJPEGEncoderRef	encoderRef, 
	CTJPEGEncodeTasks encodeTask
);

/* To find the estimate of the size of the jpeg*/
/* The estimate of the size is returned in the variable jpegSize*/
/* This function should be called after writing all the tiles to the Encoder*/
CTJPEGError CT_JPEGLIB_EXPORT CTJPEGEncoderGetJPEGSizeEstimate(
	CTJPEGEncoderRef	encoderRef, 
	CTJPEGUint32 *jpegSize
);

/** \ingroup CTJPEGEncoder
* This method is used to get information about next tile of raw image content to be previewed.
* \param encoderRef [in]  Reference to encoder object
* \param nextencodeTileArea  [in] Area of next tile that will be encoded
* \param nextPreviewTileArea [out] Area of the next preview tile
* \return CTJPEGErrorNone on success.
*/
CTJPEGError CT_JPEGLIB_EXPORT CTJPEGEncoderGetNextPreviewTileInfo(
	CTJPEGEncoderRef	encoderRef,
	CTJPEGRect			nextEncodeTileArea,
	CTJPEGRect*			nextPreviewTileArea
);

/** \ingroup CTJPEGEncoder
* This method is used to set the imageContent for the next preview tile 
* \param encoderRef [in] Reference to decoder object
* \param contentForPreview [in] the imageContent for the next preview tile 
*/
CTJPEGError CT_JPEGLIB_EXPORT CTJPEGEncoderSetNextPreviewTileContent(
	CTJPEGEncoderRef	encoderRef,
	CTJPEGImageContent	contentForPreview
);

extern CT_JPEGLIB_EXPORT const CTJPEGMemoryManager defaultCTJPEGMemoryManager;

extern CT_JPEGLIB_EXPORT const CTJPEGTaskManagerBuilder  defaultTaskManagerBuilder;
extern CT_JPEGLIB_EXPORT const CTJPEGTaskManagerBuilder parallelTaskManagerBuilder;

CTJPEGError CT_JPEGLIB_EXPORT CTJPEGLibInit(const CTJPEGMemoryManager memoryManager = defaultCTJPEGMemoryManager,
											const CTJPEGTaskManagerBuilder taskManagerBuilder = defaultTaskManagerBuilder);
void CT_JPEGLIB_EXPORT CTJPEGLibTerminate();

#ifdef __cplusplus
}
#endif

#endif

