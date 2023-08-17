#ifndef __OEM_Support_hpp__
#define __OEM_Support_hpp__	1

// =================================================================================================
// Copyright Adobe
// Copyright 2015 Adobe
// All Rights Reserved
//
// NOTICE: Adobe permits you to use, modify, and distribute this file in accordance with the terms
// of the Adobe license agreement accompanying it. If you have received this file from a source other 
// than Adobe, then your use, modification, or distribution of it requires the prior written permission
// of Adobe.
// =================================================================================================

#include "public/include/XMP_Environment.h"	// ! This must be the first include.

#include "public/include/XMP_Const.h"
#include "public/include/XMP_IO.hpp"

#include "XMPFiles/source/FormatSupport/TIFF_Support.hpp"

// =================================================================================================
/// \file OEM_Support.hpp
/// \brief MPEG4 support for getting Exif metadata from QT/MP4 file.
///
// =================================================================================================

namespace OEM_Support
{
	enum
	{
		kExifMedia_Canon = 0,
		kExifMedia_Fujifilm = 1,
		kExifMedia_Panasonic = 2,
		kExifMedia_Unknown = 3
	};

	class ExifManager
	{
	public:

		/** @brief Importing Exif data
		*
		* It imports the Exif metadata from the Cameras generated video file and reconciles it
		*
		* @param data			block of data that contains Exif metadata
		* @param length		contains the length of Exif block
		* @param xmpPacketStr	a string that represents input serialized xmp packet and will also store resultant reconciled packet
		* @param mediaType		denotes which type of video Exists ( could be Canon or FujiFilm or Panasonic )
		*/
		void ImportExif(const XMP_Uns8 * data, const XMP_Uns32 length, std::string &xmpPacketStr, const XMP_Uns32 & mediaType);

		/** @brief Updating/Processing the Exif data block using xmp packet before embedding the xmp metadata into the file
		*
		* It can be used to update the Exif block or to remove exif properties from the XMP packet before embedding the packet into file
		*
		* @param xmpPacketStr		a string denotes the input as well as output processed xmp packet
		*/
		// Not yet implemented
		void UpdateExif(std::string &xmpPacketStr);

		ExifManager() : mExifData(0), mMediaType(kExifMedia_Unknown) {}
		~ExifManager();

	private:

		/** @brief Reconcile Exif data where the data have full Exif support with APP markers
		*
		* It imports the Exif metadata from the Cameras generated video file and reconciles it
		*
		* @param data			block of data that contains Exif metadata
		* @param length			contains the length of Exif block
		*/
		bool ImportFullExif(const XMP_Uns8 * data);

		/** @brief It will be used to reconcile specific Exif data
		*
		* It reconciles the xmp metadata with Exif block present inside the video file
		*
		* @param xmpPacketStr	a string that represents input serialized xmp packet and will also store resultant reconciled packet
		*/
		void ReconcileReqExif(std::string &xmpPacketStr);

		// Variable that stores exif datablock
		TIFF_Manager * mExifData;

		// As RemoveExif() has been skipped, So following variable is no longer useful but provided
		// for handling operations while updating Exif properties from xmp packet
		XMP_Uns32  mMediaType;

		//Variable that stores tiff stream
		const XMP_Uns8* mTiffStream;

	};		// Class ExifManager

	struct sExifTagToXMP {
		XMP_Uns16    id;
		XMP_Uns16    valueType;
		XMP_Uns8	 ifdType;
		XMP_StringPtr ns;	// The namespace of the mapped XMP property.
		XMP_StringPtr name;	// The name of the mapped XMP property
		sExifTagToXMP(XMP_Uns16 tempId, XMP_Uns16 tempType, XMP_Uns8 tempIfdType, XMP_StringPtr tempNs, XMP_StringPtr tempName)
		{
			this->id = tempId;
			this->valueType = tempType;
			this->ifdType = tempIfdType;
			this->ns = tempNs;
			this->name = tempName;
		}
	};

	static const sExifTagToXMP ExifXMPReqMappings[] =
	{
		{ /*   271 */ kTIFF_Make, kTIFF_ASCIIType, kTIFF_PrimaryIFD, kXMP_NS_TIFF, "Make" },
		{ /*   272 */ kTIFF_Model, kTIFF_ASCIIType, kTIFF_PrimaryIFD, kXMP_NS_TIFF, "Model" },
		{ /* 33434 */ kTIFF_ExposureTime, kTIFF_RationalType, kTIFF_ExifIFD, kXMP_NS_EXIF, "ExposureTime" },
		{ /* 33437 */ kTIFF_FNumber, kTIFF_RationalType, kTIFF_ExifIFD, kXMP_NS_EXIF, "FNumber" },
		{ /* 37377 */ kTIFF_ShutterSpeedValue, kTIFF_SRationalType, kTIFF_ExifIFD, kXMP_NS_EXIF, "ShutterSpeedValue" },
		{ /* 34850 */ kTIFF_ExposureProgram, kTIFF_ShortType, kTIFF_ExifIFD, kXMP_NS_EXIF, "ExposureProgram" },
		{ /* 37378 */ kTIFF_ApertureValue, kTIFF_RationalType, kTIFF_ExifIFD, kXMP_NS_EXIF, "ApertureValue" },
		// ISO speed ratings
		// Sensitvity type
		// ISO speed
		{ /* 37380 */ kTIFF_ExposureBiasValue, kTIFF_SRationalType, kTIFF_ExifIFD, kXMP_NS_EXIF, "ExposureBiasValue" },
		{ /* 37381 */ kTIFF_MaxApertureValue, kTIFF_RationalType, kTIFF_ExifIFD, kXMP_NS_EXIF, "MaxApertureValue" },
		{ /* 37382 */ kTIFF_SubjectDistance, kTIFF_RationalType, kTIFF_ExifIFD, kXMP_NS_EXIF, "SubjectDistance" },
		{ /* 37386 */ kTIFF_FocalLength, kTIFF_RationalType, kTIFF_ExifIFD, kXMP_NS_EXIF, "FocalLength" },
		{ /* 41989 */ kTIFF_FocalLengthIn35mmFilm, kTIFF_ShortType, kTIFF_ExifIFD, kXMP_NS_EXIF, "FocalLengthIn35mmFilm" },
		{ /* 41486 */ kTIFF_FocalPlaneXResolution, kTIFF_RationalType, kTIFF_ExifIFD, kXMP_NS_EXIF, "FocalPlaneXResolution" },
		{ /* 41487 */ kTIFF_FocalPlaneYResolution, kTIFF_RationalType, kTIFF_ExifIFD, kXMP_NS_EXIF, "FocalPlaneYResolution" },
		{ /* 41488 */ kTIFF_FocalPlaneResolutionUnit, kTIFF_ShortType, kTIFF_ExifIFD, kXMP_NS_EXIF, "FocalPlaneResolutionUnit" },
		// TIFF Artist
		// Copyright
		// LensSpecification
		{ /* 42036 */ kTIFF_LensModel, kTIFF_ASCIIType, kTIFF_ExifIFD, kXMP_NS_ExifEX, "LensModel" },
		{ /* 42037 */ kTIFF_LensSerialNumber, kTIFF_ASCIIType, kTIFF_ExifIFD, kXMP_NS_ExifEX, "LensSerialNumber" },
		{ /* 42035 */ kTIFF_LensMake, kTIFF_ASCIIType, kTIFF_ExifIFD, kXMP_NS_ExifEX, "LensMake" },
		// GPSLatitude
		// GPSLongitude
		{ /*     5 */ kTIFF_GPSAltitudeRef, kTIFF_ByteType, kTIFF_GPSInfoIFD, kXMP_NS_EXIF, "GPSAltitudeRef" },
		{ /*     6 */ kTIFF_GPSAltitude, kTIFF_RationalType, kTIFF_GPSInfoIFD, kXMP_NS_EXIF, "GPSAltitude" },
		{ 0xFFFF, 0, 0, 0, 0 }	// ! Must end with sentinel.
	};

	// Helping functions used to import tag from files
	// Based on Tiff Support
	static void ImportSingleTIFF(const TIFF_Manager::TagInfo & tagInfo, const bool nativeEndian, SXMPMeta * xmp, const char * xmpNS, const char * xmpProp);
	static void ImportSingleTIFF_Short(const TIFF_Manager::TagInfo & tagInfo, const bool nativeEndian, SXMPMeta * xmp, const char * xmpNS, const char * xmpProp);
	static void ImportSingleTIFF_Long(const TIFF_Manager::TagInfo & tagInfo, const bool nativeEndian, SXMPMeta * xmp, const char * xmpNS, const char * xmpProp);
	static void ImportSingleTIFF_Rational(const TIFF_Manager::TagInfo & tagInfo, const bool nativeEndian, SXMPMeta * xmp, const char * xmpNS, const char * xmpProp);
	static void ImportSingleTIFF_SRational(const TIFF_Manager::TagInfo & tagInfo, const bool nativeEndian, SXMPMeta * xmp, const char * xmpNS, const char * xmpProp);
	static void ImportSingleTIFF_ASCII(const TIFF_Manager::TagInfo & tagInfo, SXMPMeta * xmp, const char * xmpNS, const char * xmpProp);
	static void ImportSingleTIFF_Byte(const TIFF_Manager::TagInfo & tagInfo, SXMPMeta * xmp, const char * xmpNS, const char * xmpProp);
	static void ImportSingleTIFF_SByte(const TIFF_Manager::TagInfo & tagInfo, SXMPMeta * xmp, const char * xmpNS, const char * xmpProp);
	static void ImportSingleTIFF_SShort(const TIFF_Manager::TagInfo & tagInfo, const bool nativeEndian, SXMPMeta * xmp, const char * xmpNS, const char * xmpProp);
	static void ImportSingleTIFF_SLong(const TIFF_Manager::TagInfo & tagInfo, const bool nativeEndian, SXMPMeta * xmp, const char * xmpNS, const char * xmpProp);
	static void ImportSingleTIFF_Float(const TIFF_Manager::TagInfo & tagInfo, const bool nativeEndian, SXMPMeta * xmp, const char * xmpNS, const char * xmpProp);
	static void ImportSingleTIFF_Double(const TIFF_Manager::TagInfo & tagInfo, const bool nativeEndian, SXMPMeta * xmp, const char * xmpNS, const char * xmpProp);
	static size_t TrimTrailingSpaces(char * firstChar, size_t origLen);
	static void TrimTrailingSpaces(TIFF_Manager::TagInfo * info);
	static void TrimTrailingSpaces(std::string * stdstr);
	static void ImportTIFF_Date(const TIFF_Manager & tiff, const TIFF_Manager::TagInfo & dateInfo, SXMPMeta * xmp, const char * xmpNS, const char * xmpProp);
	static XMP_Uns32 GatherInt(const char * strPtr, size_t count);
	static bool GetTag_Date(const TIFF_Manager & tiff, const TIFF_Manager::TagInfo & dateInfo, XMP_DateTime &dateTime);
	static void ImportTIFF_PhotographicSensitivity(const TIFF_Manager & exif, SXMPMeta * xmp);
	static void	ImportTIFF_GPSCoordinate(const TIFF_Manager & tiff, const TIFF_Manager::TagInfo & posInfo,
		SXMPMeta * xmp, const char * xmpNS, const char * xmpProp);
	static void ImportArrayTIFF_Rational(const TIFF_Manager::TagInfo & tagInfo, const bool nativeEndian,
		SXMPMeta * xmp, const char * xmpNS, const char * xmpProp);

#if AdobePrivate
	//Support of Canon makernote IFD for [CTECHXMP-4169687]
	class Canon_MakerNote_Support {
	public:
		enum {
			kEXIF_MakerNote_Tag = 0x927C,
			kCanon_MakerNote_TimeZone_Tag = 0x0035
		};

		static bool ParseCanonMakerNoteTag(TIFF_Manager * mExifData, const XMP_Uns8* tiffSTream, SXMPMeta& xmpMetaObject);
	private:
		struct TweakedIFDEntry {	// ! Most fields are in native byte order, dataOrPos is for offsets only.
			XMP_Uns16 id;
			XMP_Uns16 type;
			XMP_Uns32 bytes;
			XMP_Uns32 dataOrPos;
			TweakedIFDEntry() : id(0), type(0), bytes(0), dataOrPos(0) {};
		};

		bool ProcessMakerNoteIFD(TIFF_Manager * mExifData, const TIFF_Manager::TagInfo& ifdOffset);
		bool GetTag(XMP_Uns16 id, TIFF_Manager::TagInfo* info) const;
		Canon_MakerNote_Support(const XMP_Uns8* tiffSTream) :count(0), entries(NULL), mTiff_stream(tiffSTream), isBigEndian(false) {}

		XMP_Uns16 count;
		TweakedIFDEntry* entries;
		const XMP_Uns8* mTiff_stream;
		bool isBigEndian;
	};
#endif //AdobePrivate

}      // namespace OEM_Support

	   // =================================================================================================
#endif // __OEM_Support_hpp__