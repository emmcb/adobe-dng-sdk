////////////////////////////////////////////////////////////////////////////////////////////////////
// CTJPEGTypes.h
//           Copyright (c) 2006-2007. Adobe Systems, Incorporated. All rights reserved.
//
////////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef __H_CTJPEG_TYPES__
#define __H_CTJPEG_TYPES__

#ifdef _MSC_VER
#include <stddef.h>
#elif (__sun__ || __hpux__)
#include <inttypes.h>
#else
#include <stdint.h>
#endif

#ifdef __cplusplus
extern "C" {
#endif

#define CTJPEGTRUE 1
#define CTJPEGFALSE 0
#define CT_JPEG_MAX_INT 0x7FFFFFFF
#define CT_JPEG_MIN_INT 0x80000000
#define CT_JPEG_MAX_COMPONENTS 4

#ifdef _MSC_VER
typedef __int8				CTJPEGInt8,  *CTJPEGInt8P;
typedef unsigned __int8		CTJPEGUint8,  *CTJPEGUint8P;
typedef __int16				CTJPEGInt16, *CTJPEGInt16P;
typedef unsigned __int16	CTJPEGUint16, *CTJPEGUint16P;
typedef __int32				CTJPEGInt32, *CTJPEGInt32P;
typedef unsigned __int32	CTJPEGUint32, *CTJPEGUint32P;
typedef long long			CTJPEGInt64, *CTJPEGInt64P;
typedef unsigned long long	CTJPEGUint64, *CTJPEGUint64P;
#else
typedef int8_t		CTJPEGInt8,  *CTJPEGInt8P;
typedef uint8_t		CTJPEGUint8,  *CTJPEGUint8P;
typedef int16_t		CTJPEGInt16, *CTJPEGInt16P;
typedef uint16_t	CTJPEGUint16, *CTJPEGUint16P;
typedef int32_t		CTJPEGInt32, *CTJPEGInt32P;
typedef uint32_t	CTJPEGUint32, *CTJPEGUint32P;
typedef int64_t		CTJPEGInt64, *CTJPEGInt64P;
typedef uint64_t	CTJPEGUint64, *CTJPEGUint64P;

#endif

typedef CTJPEGUint8  CTJPEGBool8;
/**	*/
typedef CTJPEGUint16 CTJPEGBool16;
/**	*/
typedef CTJPEGUint32 CTJPEGBool32;

#ifdef __cplusplus
}
#endif

#endif //__H_CTJPEG_TYPES__
