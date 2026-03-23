// =================================================================================================
// Copyright Adobe
// Copyright 2014 Adobe
// All Rights Reserved
//
// NOTICE: Adobe permits you to use, modify, and distribute this file in accordance with the terms
// of the Adobe license agreement accompanying it. If you have received this file from a source other 
// than Adobe, then your use, modification, or distribution of it requires the prior written permission
// of Adobe.
// =================================================================================================

#include "public/include/XMP_Environment.h"	// ! This must be the first include.

#include "source/XMPFiles_IO.hpp"
#include "source/XIO.hpp"
#include "source/XMP_LibUtils.hpp"

#include "OEM_Support.hpp"
#include "XMPFiles/source/FormatSupport/Reconcile_Impl.hpp"

namespace OEM_Support
{

	// =================================================================================================
	// ExifManager::ImportExif
	// ====================================
	void ExifManager::ImportExif(const XMP_Uns8 * exifData, const XMP_Uns32 exifLength, std::string &xmpPacket, const XMP_Uns32 & mediaType)
	{
		if (mediaType == kExifMedia_Canon)
		{
			if (exifLength < 12)						// Minimum length should be 12 ( APP1 Marker + length + Exif signature )
				return;
			mMediaType = mediaType;
			if (ImportFullExif(exifData))
				ReconcileReqExif(xmpPacket);
		}
		else if (mediaType == kExifMedia_Fujifilm)
		{
			const XMP_Uns32 offset = 16;				// Here tiff stream starts after 16 bytes from starting of MVTG atom's content 
			if (exifLength <= offset)
				return;
			mMediaType = mediaType;
			mTiffStream = exifData + offset;
			XMP_Uns32 tiffLength = exifLength - offset;
			mExifData = new TIFF_MemoryReader();
			mExifData->ParseMemoryStream(mTiffStream, tiffLength, true, true);

			ReconcileReqExif(xmpPacket);

		}
		else if (mediaType == kExifMedia_Panasonic)
		{
			XMP_Uns8 extHeaderVersion = *(exifData + 2);
			XMP_Uns32 offset = 0x0000405C;			// Private Info starts after 16476 ( 16388 + 84 + 4 ) bytes from starting of PANA atom's content
			if (extHeaderVersion != 0)
				offset += 24;						// Here, 24 ( 16 + 8 ) more bytes are added to get Private Info
			const XMP_Uns8 * privateInfo = exifData + offset;
			const XMP_Uns16 tagId = GetUns16BE(privateInfo + 4);
			const XMP_Uns16 tagVer = GetUns16BE(privateInfo + 6);
			if (tagId == tagVer == 1)
			{
				offset = 8;
				XMP_Uns32 tiffLength = GetUns32BE(privateInfo + offset);
				if (tiffLength < 12)										// Minimum length should be 12 ( APP1 Marker + length + Exif signature )
					return;
				mMediaType = mediaType;
				const XMP_Uns8 * tiffStart = privateInfo + offset + 4;
				if (ImportFullExif(tiffStart))
					ReconcileReqExif(xmpPacket);

			}
		}
		else
			mMediaType = kExifMedia_Unknown;

	}	// ExifManager::ImportExif

		// =================================================================================================
		// ExifManager::ImportFullExif
		// ====================================
	bool ExifManager::ImportFullExif(const XMP_Uns8 * data)
	{
		const XMP_Uns32 kAPP1Marker = 0xFFD8FFE1;				// APP 1 marker is FFD8 FFE1
		XMP_Uns32 APP1Marker = GetUns32BE(data);
		if (APP1Marker != kAPP1Marker)
			return false;
		XMP_Uns32 tiffLength = GetUns16BE(data + 4);			// 2 byte Length is present after APP1 marker(4 Bytes long)
		const XMP_Uns32 kExifSignatureString = 0x45786966;		// Exif signature is Exif(0x45786966)
		const size_t kExifSignatureOffset = 4 + 2;				// 4 bytes for APP1 Marker and 2 bytes for length of APP1   
		XMP_Uns32 EXIFMarker = GetUns32BE(data + kExifSignatureOffset);
		if (EXIFMarker != kExifSignatureString)
			return false;
		mTiffStream = data + 4 + 2 + 4 + 2;		// APP1 Marker + length + Exif Signature + 2 bytes reserved null 
		this->mExifData = new TIFF_MemoryReader();
		this->mExifData->ParseMemoryStream(mTiffStream, tiffLength);
		return true;

	}	// ExifManager::ImportFullExif

		// =================================================================================================
		// ExifManager::ReconcileReqExif
		// ====================================
	void ExifManager::ReconcileReqExif(std::string &xmpPacketStr)
	{
		// Currently we are giving priority to XMP packet not native EXIF block.
		// This should be chnaged when we will support writeback support to native exif block
		if (xmpPacketStr.length() > Max_XMP_Uns32)
			return;

		SXMPMeta xmpMetaObject(xmpPacketStr.c_str(), static_cast< XMP_Uns32 >(xmpPacketStr.length()));

		const bool nativeEndian = mExifData->IsNativeEndian();
		bool foundFromExif = false;
		bool isDataExists = false;
		TIFF_Manager::TagInfo tagInfo;

#if AdobePrivate

		//Support of Canon makernote IFD for [CTECHXMP-4169687]
		if (mMediaType == kExifMedia_Canon) {
			isDataExists = Canon_MakerNote_Support::ParseCanonMakerNoteTag(mExifData, mTiffStream, xmpMetaObject);
		}
#endif //AdobePrivate

		

		for (XMP_Uns32 index = 0; ExifXMPReqMappings[index].id != 0xFFFF; ++index)
		{
			const sExifTagToXMP & mapInfo = ExifXMPReqMappings[index];
			if (!xmpMetaObject.DoesPropertyExist(mapInfo.ns, mapInfo.name))
			{
				try
				{
					bool lFoundFromExif = mExifData->GetTag(mapInfo.ifdType, mapInfo.id, &tagInfo);
					if (lFoundFromExif && tagInfo.type == mapInfo.valueType && tagInfo.count > 0)
					{
						ImportSingleTIFF(tagInfo, nativeEndian, &xmpMetaObject, mapInfo.ns, mapInfo.name);
						isDataExists = true;
					}
				}
				catch (...)
				{
					// Do nothing, let other imports proceed.
				}
			}
		}



		// Special Mappings
		ImportTIFF_PhotographicSensitivity(*mExifData, &xmpMetaObject);

		// First priority would be given to xmp,
		// If xmp doesn't have creator field then import artist field
		if (!xmpMetaObject.DoesPropertyExist(kXMP_NS_DC, "creator"))
		{
			try
			{
				bool lFoundFromExif = mExifData->GetTag(kTIFF_PrimaryIFD, kTIFF_Artist, &tagInfo);
				if (lFoundFromExif && tagInfo.count > 0)
				{
					std::string creatorValue((char*)tagInfo.dataPtr, tagInfo.dataLen);
					SXMPUtils::SeparateArrayItems(&xmpMetaObject, kXMP_NS_DC, "creator",
						(kXMP_PropArrayIsOrdered | kXMPUtil_AllowCommas), creatorValue);
					isDataExists = true;
				}
			}
			catch (...)
			{
				// Do nothing, let other imports proceed.
			}
		}

		// First priority would be given to xmp,
		// If xmp doesn't have rights field then copyright would be imported
		if (!xmpMetaObject.DoesPropertyExist(kXMP_NS_DC, "rights"))
		{
			try
			{
				bool lFoundFromExif = mExifData->GetTag(kTIFF_PrimaryIFD, kTIFF_Copyright, &tagInfo);
				if (lFoundFromExif && tagInfo.count > 0)
				{
					std::string rightsValue((char*)tagInfo.dataPtr, tagInfo.dataLen);
					xmpMetaObject.SetLocalizedText(kXMP_NS_DC, "rights", "", "x-default", rightsValue.c_str());
					isDataExists = true;
				}
			}
			catch (...)
			{
				// Do nothing, let other imports proceed.
			}
		}

		if (!xmpMetaObject.DoesPropertyExist(kXMP_NS_ExifEX, "LensSpecification"))
		{
			try
			{
				bool lFoundFromExif = mExifData->GetTag(kTIFF_ExifIFD, kTIFF_LensSpecification, &tagInfo);
				if (lFoundFromExif && tagInfo.count > 0)
				{
					ImportArrayTIFF_Rational(tagInfo, nativeEndian, &xmpMetaObject, kXMP_NS_ExifEX, "LensSpecification");
					isDataExists = true;
				}
			}
			catch (...)
			{
				// Do nothing, let other imports proceed.
			}
		}

		// Import GPSLatitude
		if (!xmpMetaObject.DoesPropertyExist(kXMP_NS_EXIF, "GPSLatitude"))
		{
			try
			{
				bool lFoundFromExif = mExifData->GetTag(kTIFF_GPSInfoIFD, kTIFF_GPSLatitude, &tagInfo);
				if (lFoundFromExif && tagInfo.count > 0)
				{
					ImportSingleTIFF(tagInfo, nativeEndian, &xmpMetaObject, kXMP_NS_EXIF, "GPSLatitude");
					isDataExists = true;
				}
			}
			catch (...)
			{
				// Do nothing, let other imports proceed.
			}
		}

		// Import GPSLongitude
		if (!xmpMetaObject.DoesPropertyExist(kXMP_NS_EXIF, "GPSLongitude"))
		{
			try
			{
				bool lFoundFromExif = mExifData->GetTag(kTIFF_GPSInfoIFD, kTIFF_GPSLongitude, &tagInfo);
				if (lFoundFromExif && tagInfo.count > 0)
				{
					ImportSingleTIFF(tagInfo, nativeEndian, &xmpMetaObject, kXMP_NS_EXIF, "GPSLongitude");
					isDataExists = true;
				}
			}
			catch (...)
			{
				// Do nothing, let other imports proceed.
			}
		}

		if (isDataExists)
			xmpMetaObject.SerializeToBuffer(&xmpPacketStr);

	}	// ExifManager::ReconcileReqExif

		// =================================================================================================
		// ExifManager::UpdateExif
		// ====================================
	void ExifManager::UpdateExif(std::string &xmp)
	{
		/*
		// Skipping currently
		Processing over Exif metadata before embedding xmp metadata into file
		*/
	}	// ExifManager::UpdateExif

		// =================================================================================================
		// ExifManager::~ExifManager
		// ====================================
	ExifManager::~ExifManager()
	{
		if (mExifData != 0)
			delete (mExifData);

	}	// ExifManager::~ExifManager

		// =================================================================================================
		// ImportSingleTIFF
		// ====================================
	void ImportSingleTIFF(const TIFF_Manager::TagInfo & tagInfo, const bool nativeEndian, SXMPMeta * xmp, const char * xmpNS, const char * xmpProp)
	{
		switch (tagInfo.type) {

		case kTIFF_ShortType:
			ImportSingleTIFF_Short(tagInfo, nativeEndian, xmp, xmpNS, xmpProp);
			break;

		case kTIFF_LongType:
			ImportSingleTIFF_Long(tagInfo, nativeEndian, xmp, xmpNS, xmpProp);
			break;

		case kTIFF_RationalType:
			ImportSingleTIFF_Rational(tagInfo, nativeEndian, xmp, xmpNS, xmpProp);
			break;

		case kTIFF_SRationalType:
			ImportSingleTIFF_SRational(tagInfo, nativeEndian, xmp, xmpNS, xmpProp);
			break;

		case kTIFF_ASCIIType:
			ImportSingleTIFF_ASCII(tagInfo, xmp, xmpNS, xmpProp);
			break;

		case kTIFF_ByteType:
			ImportSingleTIFF_Byte(tagInfo, xmp, xmpNS, xmpProp);
			break;

		case kTIFF_SByteType:
			ImportSingleTIFF_SByte(tagInfo, xmp, xmpNS, xmpProp);
			break;

		case kTIFF_SShortType:
			ImportSingleTIFF_SShort(tagInfo, nativeEndian, xmp, xmpNS, xmpProp);
			break;

		case kTIFF_SLongType:
			ImportSingleTIFF_SLong(tagInfo, nativeEndian, xmp, xmpNS, xmpProp);
			break;

		case kTIFF_FloatType:
			ImportSingleTIFF_Float(tagInfo, nativeEndian, xmp, xmpNS, xmpProp);
			break;

		case kTIFF_DoubleType:
			ImportSingleTIFF_Double(tagInfo, nativeEndian, xmp, xmpNS, xmpProp);
			break;

		}

	}	// ImportSingleTIFF

		// =================================================================================================
		// ImportSingleTIFF_Short
		// ====================================
	void ImportSingleTIFF_Short(const TIFF_Manager::TagInfo & tagInfo, const bool nativeEndian,
		SXMPMeta * xmp, const char * xmpNS, const char * xmpProp)
	{
		try {	// Don't let errors with one stop the others.

			XMP_Uns16 binValue = GetUns16AsIs(tagInfo.dataPtr);
			if (!nativeEndian) binValue = Flip2(binValue);

			char strValue[20];
			snprintf(strValue, sizeof(strValue), "%hu", binValue);	// AUDIT: Using sizeof(strValue) is safe.

			xmp->SetProperty(xmpNS, xmpProp, strValue);

		}
		catch (...) {
			// Do nothing, let other imports proceed.
			// ? Notify client?
		}

	}	// ImportSingleTIFF_Short

		// =================================================================================================
		// ImportSingleTIFF_Long
		// ====================================
	void ImportSingleTIFF_Long(const TIFF_Manager::TagInfo & tagInfo, const bool nativeEndian,
		SXMPMeta * xmp, const char * xmpNS, const char * xmpProp)
	{
		try {	// Don't let errors with one stop the others.

			XMP_Uns32 binValue = GetUns32AsIs(tagInfo.dataPtr);
			if (!nativeEndian) binValue = Flip4(binValue);

			char strValue[20];
			snprintf(strValue, sizeof(strValue), "%lu", (unsigned long)binValue);	// AUDIT: Using sizeof(strValue) is safe.

			xmp->SetProperty(xmpNS, xmpProp, strValue);

		}
		catch (...) {
			// Do nothing, let other imports proceed.
			// ? Notify client?
		}

	}	// ImportSingleTIFF_Long

		// =================================================================================================
		// ImportSingleTIFF_Rational
		// ====================================
	void ImportSingleTIFF_Rational(const TIFF_Manager::TagInfo & tagInfo, const bool nativeEndian,
		SXMPMeta * xmp, const char * xmpNS, const char * xmpProp)
	{
		try {	// Don't let errors with one stop the others.

			XMP_Uns32 * binPtr = (XMP_Uns32*)tagInfo.dataPtr;
			XMP_Uns32 binNum = GetUns32AsIs(&binPtr[0]);
			XMP_Uns32 binDenom = GetUns32AsIs(&binPtr[1]);
			if (!nativeEndian) {
				binNum = Flip4(binNum);
				binDenom = Flip4(binDenom);
			}

			char strValue[40];
			snprintf(strValue, sizeof(strValue), "%lu/%lu", (unsigned long)binNum, (unsigned long)binDenom);	// AUDIT: Using sizeof(strValue) is safe.

			xmp->SetProperty(xmpNS, xmpProp, strValue);

		}
		catch (...) {
			// Do nothing, let other imports proceed.
			// ? Notify client?
		}

	}	// ImportSingleTIFF_Rational

		// =================================================================================================
		// ImportSingleTIFF_SRational
		// ====================================
	void ImportSingleTIFF_SRational(const TIFF_Manager::TagInfo & tagInfo, const bool nativeEndian,
		SXMPMeta * xmp, const char * xmpNS, const char * xmpProp)
	{
		try {	// Don't let errors with one stop the others.

#if SUNOS_SPARC || XMP_IOS_ARM
			XMP_Uns32  binPtr[2];
			memcpy(&binPtr, tagInfo.dataPtr, sizeof(XMP_Uns32) * 2);
#else
			XMP_Uns32 * binPtr = (XMP_Uns32*)tagInfo.dataPtr;
#endif //#if SUNOS_SPARC || XMP_IOS_ARM
			XMP_Int32 binNum = GetUns32AsIs(&binPtr[0]);
			XMP_Int32 binDenom = GetUns32AsIs(&binPtr[1]);
			if (!nativeEndian) {
				Flip4(&binNum);
				Flip4(&binDenom);
			}

			char strValue[40];
			snprintf(strValue, sizeof(strValue), "%ld/%ld", (unsigned long)binNum, (unsigned long)binDenom);	// AUDIT: Using sizeof(strValue) is safe.

			xmp->SetProperty(xmpNS, xmpProp, strValue);

		}
		catch (...) {
			// Do nothing, let other imports proceed.
			// ? Notify client?
		}

	}	// ImportSingleTIFF_SRational

		// =================================================================================================
		// ImportSingleTIFF_ASCII
		// ====================================
	void ImportSingleTIFF_ASCII(const TIFF_Manager::TagInfo & tagInfo,
		SXMPMeta * xmp, const char * xmpNS, const char * xmpProp)
	{
		try {	// Don't let errors with one stop the others.

			TrimTrailingSpaces((TIFF_Manager::TagInfo*) &tagInfo);
			if (tagInfo.dataLen == 0) return;	// Ignore empty tags.

			const char * chPtr = (const char *)tagInfo.dataPtr;
			const bool   hasNul = (chPtr[tagInfo.dataLen - 1] == 0);
			const bool   isUTF8 = ReconcileUtils::IsUTF8(chPtr, tagInfo.dataLen);

			if (isUTF8 && hasNul) {
				xmp->SetProperty(xmpNS, xmpProp, chPtr);
			}
			else {
				std::string strValue;
				if (isUTF8) {
					strValue.assign(chPtr, tagInfo.dataLen);
				}
				else {
					if (ignoreLocalText) return;
					ReconcileUtils::LocalToUTF8(chPtr, tagInfo.dataLen, &strValue);
				}
				xmp->SetProperty(xmpNS, xmpProp, strValue.c_str());
			}

		}
		catch (...) {
			// Do nothing, let other imports proceed.
			// ? Notify client?
		}

	}	// ImportSingleTIFF_ASCII

		// =================================================================================================
		// ImportSingleTIFF_Byte
		// ====================================
	void ImportSingleTIFF_Byte(const TIFF_Manager::TagInfo & tagInfo,
		SXMPMeta * xmp, const char * xmpNS, const char * xmpProp)
	{
		try {	// Don't let errors with one stop the others.

			XMP_Uns8 binValue = *((XMP_Uns8*)tagInfo.dataPtr);

			char strValue[20];
			snprintf(strValue, sizeof(strValue), "%hu", (XMP_Uns16)binValue);	// AUDIT: Using sizeof(strValue) is safe.

			xmp->SetProperty(xmpNS, xmpProp, strValue);

		}
		catch (...) {
			// Do nothing, let other imports proceed.
			// ? Notify client?
		}

	}	// ImportSingleTIFF_Byte

		// =================================================================================================
		// ImportSingleTIFF_SByte
		// ====================================
	void ImportSingleTIFF_SByte(const TIFF_Manager::TagInfo & tagInfo,
		SXMPMeta * xmp, const char * xmpNS, const char * xmpProp)
	{
		try {	// Don't let errors with one stop the others.

			XMP_Int8 binValue = *((XMP_Int8*)tagInfo.dataPtr);

			char strValue[20];
			snprintf(strValue, sizeof(strValue), "%hd", (short)binValue);	// AUDIT: Using sizeof(strValue) is safe.

			xmp->SetProperty(xmpNS, xmpProp, strValue);

		}
		catch (...) {
			// Do nothing, let other imports proceed.
			// ? Notify client?
		}

	}	// ImportSingleTIFF_SByte

		// =================================================================================================
		// ImportSingleTIFF_SShort
		// ====================================
	void ImportSingleTIFF_SShort(const TIFF_Manager::TagInfo & tagInfo, const bool nativeEndian,
		SXMPMeta * xmp, const char * xmpNS, const char * xmpProp)
	{
		try {	// Don't let errors with one stop the others.

			XMP_Int16 binValue = *((XMP_Int16*)tagInfo.dataPtr);
			if (!nativeEndian) Flip2(&binValue);

			char strValue[20];
			snprintf(strValue, sizeof(strValue), "%hd", binValue);	// AUDIT: Using sizeof(strValue) is safe.

			xmp->SetProperty(xmpNS, xmpProp, strValue);

		}
		catch (...) {
			// Do nothing, let other imports proceed.
			// ? Notify client?
		}

	}	// ImportSingleTIFF_SShort


		// =================================================================================================
		// ImportSingleTIFF_SLong
		// ====================================
	void ImportSingleTIFF_SLong(const TIFF_Manager::TagInfo & tagInfo, const bool nativeEndian,
		SXMPMeta * xmp, const char * xmpNS, const char * xmpProp)
	{
		try {	// Don't let errors with one stop the others.

			XMP_Int32 binValue = *((XMP_Int32*)tagInfo.dataPtr);
			if (!nativeEndian) Flip4(&binValue);

			char strValue[20];
			snprintf(strValue, sizeof(strValue), "%ld", (long)binValue);	// AUDIT: Using sizeof(strValue) is safe.

			xmp->SetProperty(xmpNS, xmpProp, strValue);

		}
		catch (...) {
			// Do nothing, let other imports proceed.
			// ? Notify client?
		}

	}	// ImportSingleTIFF_SLong

		// =================================================================================================
		// ImportSingleTIFF_Float
		// ====================================
	void ImportSingleTIFF_Float(const TIFF_Manager::TagInfo & tagInfo, const bool nativeEndian,
		SXMPMeta * xmp, const char * xmpNS, const char * xmpProp)
	{
		try {	// Don't let errors with one stop the others.

			float binValue = *((float*)tagInfo.dataPtr);
			if (!nativeEndian) Flip4(&binValue);

			xmp->SetProperty_Float(xmpNS, xmpProp, binValue);

		}
		catch (...) {
			// Do nothing, let other imports proceed.
			// ? Notify client?
		}

	}	// ImportSingleTIFF_Float

		// =================================================================================================
		// ImportSingleTIFF_Double
		// ====================================
	void ImportSingleTIFF_Double(const TIFF_Manager::TagInfo & tagInfo, const bool nativeEndian,
		SXMPMeta * xmp, const char * xmpNS, const char * xmpProp)
	{
		try {	// Don't let errors with one stop the others.

			double binValue = *((double*)tagInfo.dataPtr);
			if (!nativeEndian) Flip8(&binValue);

			xmp->SetProperty_Float(xmpNS, xmpProp, binValue);	// ! Yes, SetProperty_Float.

		}
		catch (...) {
			// Do nothing, let other imports proceed.
			// ? Notify client?
		}

	}	// ImportSingleTIFF_Double

		// =================================================================================================
		// TrimTrailingSpaces
		// ====================================
	size_t TrimTrailingSpaces(char * firstChar, size_t origLen)
	{
		if (origLen == 0) return 0;

		char * lastChar = firstChar + origLen - 1;
		if ((*lastChar != ' ') && (*lastChar != 0)) return origLen;	// Nothing to do.

		while ((firstChar <= lastChar) && ((*lastChar == ' ') || (*lastChar == 0))) --lastChar;

		XMP_Assert((lastChar == firstChar - 1) ||
			((lastChar >= firstChar) && (*lastChar != ' ') && (*lastChar != 0)));

		size_t newLen = (size_t)((lastChar + 1) - firstChar);
		XMP_Assert(newLen <= origLen);

		if (newLen < origLen) {
			++lastChar;
			*lastChar = 0;
		}

		return newLen;
	}	// TrimTrailingSpaces

		// =================================================================================================
		// TrimTrailingSpaces
		// ====================================
	void TrimTrailingSpaces(TIFF_Manager::TagInfo * info)
	{
		info->dataLen = (XMP_Uns32)TrimTrailingSpaces((char*)info->dataPtr, (size_t)info->dataLen);

	}	// TrimTrailingSpaces

		// =================================================================================================
		// TrimTrailingSpaces
		// ====================================
	void TrimTrailingSpaces(std::string * stdstr)
	{
		size_t origLen = stdstr->size();
		size_t newLen = TrimTrailingSpaces((char*)stdstr->c_str(), origLen);
		if (newLen != origLen) stdstr->erase(newLen);
	}	// TrimTrailingSpaces

	bool GetTag_Date(const TIFF_Manager & tiff, const TIFF_Manager::TagInfo & dateInfo, XMP_DateTime &dateTime)
	{
		XMP_Uns16 secID = 0;
		switch (dateInfo.id) {
		case kTIFF_DateTime: secID = kTIFF_SubSecTime;			break;
		case kTIFF_DateTimeOriginal: secID = kTIFF_SubSecTimeOriginal;	break;
		case kTIFF_DateTimeDigitized: secID = kTIFF_SubSecTimeDigitized;	break;
		}

		try {	// Don't let errors with one stop the others.

			if ((dateInfo.type != kTIFF_ASCIIType) || (dateInfo.count != 20)) return false;

			const char * dateStr = (const char *)dateInfo.dataPtr;
			if ((dateStr[4] != ':') || (dateStr[7] != ':') ||
				(dateStr[10] != ' ') || (dateStr[13] != ':') || (dateStr[16] != ':')) return false;

			dateTime.year = GatherInt(&dateStr[0], 4);
			dateTime.month = GatherInt(&dateStr[5], 2);
			dateTime.day = GatherInt(&dateStr[8], 2);
			if ((dateTime.year != 0) | (dateTime.month != 0) | (dateTime.day != 0)) dateTime.hasDate = true;

			dateTime.hour = GatherInt(&dateStr[11], 2);
			dateTime.minute = GatherInt(&dateStr[14], 2);
			dateTime.second = GatherInt(&dateStr[17], 2);
			dateTime.nanoSecond = 0;	// Get the fractional seconds later.
			if ((dateTime.hour != 0) | (dateTime.minute != 0) | (dateTime.second != 0)) dateTime.hasTime = true;

			dateTime.tzSign = 0;	// ! Separate assignment, avoid VS integer truncation warning.
			dateTime.tzHour = dateTime.tzMinute = 0;
			dateTime.hasTimeZone = false;	// Exif times have no zone.

											// *** Consider looking at the TIFF/EP TimeZoneOffset tag?

			TIFF_Manager::TagInfo secInfo;
			bool found = tiff.GetTag(kTIFF_ExifIFD, secID, &secInfo);	// ! Subseconds are all in the Exif IFD.

			if (found && (secInfo.type == kTIFF_ASCIIType)) {
				const char * fracPtr = (const char *)secInfo.dataPtr;
				dateTime.nanoSecond = GatherInt(fracPtr, secInfo.dataLen);
				size_t digits = 0;
				for (; (('0' <= *fracPtr) && (*fracPtr <= '9')); ++fracPtr) ++digits;
				for (; digits < 9; ++digits) dateTime.nanoSecond *= 10;
				if (dateTime.nanoSecond != 0) dateTime.hasTime = true;
			}
			return true;
		}
		catch (...) {
			return false;
			// Do nothing, let other imports proceed.
			// ? Notify client?
		}
	}

	// =================================================================================================
	// ImportTIFF_Date
	// ====================================
	void ImportTIFF_Date(const TIFF_Manager & tiff, const TIFF_Manager::TagInfo & dateInfo, SXMPMeta * xmp, const char * xmpNS, const char * xmpProp)
	{
		try
		{
			XMP_DateTime dateTime;
			if (GetTag_Date(tiff, dateInfo, dateTime))
				xmp->SetProperty_Date(xmpNS, xmpProp, dateTime);
		}
		catch (...)
		{
			// Do nothing, let other imports proceed.
		}
	}	// ImportTIFF_Date

		// =================================================================================================
		// GatherInt
		// ====================================
	XMP_Uns32 GatherInt(const char * strPtr, size_t count)
	{
		XMP_Uns32 value = 0;
		const char * strEnd = strPtr + count;

		while (strPtr < strEnd) {
			char ch = *strPtr;
			if ((ch < '0') || (ch > '9')) break;
			value = value * 10 + (ch - '0');
			++strPtr;
		}

		return value;
	}	// GatherInt

		// =================================================================================================
		// ImportTIFF_PhotographicSensitivity
		// ====================================
	static void ImportTIFF_PhotographicSensitivity(const TIFF_Manager & exif, SXMPMeta * xmp)
	{

		// PhotographicSensitivity has special cases for values over 65534 because the tag is SHORT. It
		// has a count of "any", but all known cameras used a count of 1. Exif 2.3 still allows a count
		// of "any" but says only 1 value should ever be recorded. We only process the first value if
		// the count is greater than 1.
		//
		// Prior to Exif 2.3 the tag was called ISOSpeedRatings. Some cameras omit the tag and some
		// write 65535. Most put the real ISO in MakerNote, which might be in the XMP from ACR.
		//
		// With Exif 2.3 five speed-related tags were added of type LONG: StandardOutputSensitivity,
		// RecommendedExposureIndex, ISOSpeed, ISOSpeedLatitudeyyy, and ISOSpeedLatitudezzz. The tag
		// SensitivityType was added to say which of these five is copied to PhotographicSensitivity.
		//
		// Import logic:
		//	Save exif:ISOSpeedRatings when cleaning namespaces (done in ImportPhotoData)
		//	If ExifVersion is less than "0230" (or missing)
		//		If PhotographicSensitivity[1] < 65535 or no exif:ISOSpeedRatings: set exif:ISOSpeedRatings from tag
		//	Else (ExifVersion is at least "0230")
		//		Import SensitivityType and related long tags
		//		If PhotographicSensitivity[1] < 65535: set exifEX:PhotographicSensitivity and exif:ISOSpeedRatings from tag
		//		Else (no PhotographicSensitivity tag or value is 65535)
		//			Set exifEX:PhotographicSensitivity from tag (missing or 65535)
		//			If have SensitivityType and indicated tag: set exif:ISOSpeedRatings from indicated tag

		try {

			bool found;
			TIFF_Manager::TagInfo tagInfo;

			bool haveOldExif = true;	// Default to old Exif if no version tag.
			bool haveTag34855 = false;
			bool haveLowISO = false;	// Set for real if haveTag34855 is true.

			XMP_Uns32 valueTag34855;	// ! Only care about the first value if more than 1.

			found = exif.GetTag(kTIFF_ExifIFD, kTIFF_ExifVersion, &tagInfo);
			if (found && (tagInfo.type == kTIFF_UndefinedType) && (tagInfo.count == 4)) {
				haveOldExif = (strncmp((char*)tagInfo.dataPtr, "0230", 4) < 0);
			}

			haveTag34855 = exif.GetTag_Integer(kTIFF_ExifIFD, kTIFF_PhotographicSensitivity, &valueTag34855);
			if (haveTag34855) haveLowISO = (valueTag34855 < 65535);

			if (haveOldExif) {	// Exif version is before 2.3, use just the old tag and property.

				if (haveTag34855) {
					if (haveLowISO || (!xmp->DoesPropertyExist(kXMP_NS_EXIF, "ISOSpeedRatings"))) {
						xmp->DeleteProperty(kXMP_NS_EXIF, "ISOSpeedRatings");
						xmp->AppendArrayItem(kXMP_NS_EXIF, "ISOSpeedRatings", kXMP_PropArrayIsOrdered, "");
						xmp->SetProperty_Int(kXMP_NS_EXIF, "ISOSpeedRatings[1]", valueTag34855);
					}
				}

			}
			else {	// Exif version is 2.3 or newer, use the Exif 2.3 tags and properties.

					// Import the SensitivityType and related long tags.

				XMP_Uns16 whichLongTag = 0;
				XMP_Uns32 sensitivityType, tagValue;

				bool haveSensitivityType = exif.GetTag_Integer(kTIFF_ExifIFD, kTIFF_SensitivityType, &sensitivityType);
				if (haveSensitivityType) {
					xmp->SetProperty_Int(kXMP_NS_ExifEX, "SensitivityType", sensitivityType);
					switch (sensitivityType) {
					case 1: // Use StandardOutputSensitivity for both 1 and 4.
					case 4: whichLongTag = kTIFF_StandardOutputSensitivity; break;
					case 2: whichLongTag = kTIFF_RecommendedExposureIndex; break;
					case 3: // Use ISOSpeed for all of 3, 5, 6, and 7.
					case 5:
					case 6:
					case 7: whichLongTag = kTIFF_ISOSpeed; break;
					}
				}

				found = exif.GetTag_Integer(kTIFF_ExifIFD, kTIFF_StandardOutputSensitivity, &tagValue);
				if (found) {
					xmp->SetProperty_Int64(kXMP_NS_ExifEX, "StandardOutputSensitivity", tagValue);
				}

				found = exif.GetTag_Integer(kTIFF_ExifIFD, kTIFF_RecommendedExposureIndex, &tagValue);
				if (found) {
					xmp->SetProperty_Int64(kXMP_NS_ExifEX, "RecommendedExposureIndex", tagValue);
				}

				found = exif.GetTag_Integer(kTIFF_ExifIFD, kTIFF_ISOSpeed, &tagValue);
				if (found) {
					xmp->SetProperty_Int64(kXMP_NS_ExifEX, "ISOSpeed", tagValue);
				}

				found = exif.GetTag_Integer(kTIFF_ExifIFD, kTIFF_ISOSpeedLatitudeyyy, &tagValue);
				if (found) {
					xmp->SetProperty_Int64(kXMP_NS_ExifEX, "ISOSpeedLatitudeyyy", tagValue);
				}

				found = exif.GetTag_Integer(kTIFF_ExifIFD, kTIFF_ISOSpeedLatitudezzz, &tagValue);
				if (found) {
					xmp->SetProperty_Int64(kXMP_NS_ExifEX, "ISOSpeedLatitudezzz", tagValue);
				}

				// Deal with the special cases for exifEX:PhotographicSensitivity and exif:ISOSpeedRatings.

				if (haveTag34855 & haveLowISO) {	// The easier low ISO case.

					xmp->DeleteProperty(kXMP_NS_EXIF, "ISOSpeedRatings");
					xmp->AppendArrayItem(kXMP_NS_EXIF, "ISOSpeedRatings", kXMP_PropArrayIsOrdered, "");
					xmp->SetProperty_Int(kXMP_NS_EXIF, "ISOSpeedRatings[1]", valueTag34855);
					xmp->SetProperty_Int(kXMP_NS_ExifEX, "PhotographicSensitivity", valueTag34855);

				}
				else {	// The more complex high ISO case, or no PhotographicSensitivity tag.

					if (haveTag34855) {
						XMP_Assert(valueTag34855 == 65535);
						xmp->SetProperty_Int(kXMP_NS_ExifEX, "PhotographicSensitivity", valueTag34855);
					}

					if (whichLongTag != 0) {
						found = exif.GetTag(kTIFF_ExifIFD, whichLongTag, &tagInfo);
						if (found && (tagInfo.type == kTIFF_LongType) && (tagInfo.count == 1)) {
							xmp->DeleteProperty(kXMP_NS_EXIF, "ISOSpeedRatings");
							xmp->AppendArrayItem(kXMP_NS_EXIF, "ISOSpeedRatings", kXMP_PropArrayIsOrdered, "");
							xmp->SetProperty_Int(kXMP_NS_EXIF, "ISOSpeedRatings[1]", exif.GetUns32(tagInfo.dataPtr));
						}
					}

				}

			}

		}
		catch (...) {

			// Do nothing, don't let failures here stop other exports.

		}
	}

	// =================================================================================================
	// ImportTIFF_GPSCoordinate
	// ========================

	static void
		ImportTIFF_GPSCoordinate(const TIFF_Manager & tiff, const TIFF_Manager::TagInfo & posInfo,
			SXMPMeta * xmp, const char * xmpNS, const char * xmpProp)
	{
		try {	// Don't let errors with one stop the others. Tolerate ill-formed values where reasonable.

			const bool nativeEndian = tiff.IsNativeEndian();

			if ((posInfo.type != kTIFF_RationalType) || (posInfo.count == 0)) return;

			XMP_Uns16 refID = posInfo.id - 1;	// ! The GPS refs and coordinates are all tag n and n+1.
			TIFF_Manager::TagInfo refInfo;
			bool found = tiff.GetTag(kTIFF_GPSInfoIFD, refID, &refInfo);
			if ((!found) || (refInfo.count == 0)) return;
			char ref = *((char*)refInfo.dataPtr);
			if ((ref != 'N') && (ref != 'S') && (ref != 'E') && (ref != 'W')) return;

			XMP_Uns32 * binPtr = (XMP_Uns32*)posInfo.dataPtr;
			XMP_Uns32 degNum = 0, degDenom = 1;	// Defaults for missing parts.
			XMP_Uns32 minNum = 0, minDenom = 1;
			XMP_Uns32 secNum = 0, secDenom = 1;
			if (!nativeEndian) {
				degDenom = Flip4(degDenom);	// So they can be flipped again below.
				minDenom = Flip4(minDenom);
				secDenom = Flip4(secDenom);
			}

			degNum = GetUns32AsIs(&binPtr[0]);
			degDenom = GetUns32AsIs(&binPtr[1]);

			if (posInfo.count >= 2) {
				minNum = GetUns32AsIs(&binPtr[2]);
				minDenom = GetUns32AsIs(&binPtr[3]);
				if (posInfo.count >= 3) {
					secNum = GetUns32AsIs(&binPtr[4]);
					secDenom = GetUns32AsIs(&binPtr[5]);
				}
			}

			if (!nativeEndian) {
				degNum = Flip4(degNum);
				degDenom = Flip4(degDenom);
				minNum = Flip4(minNum);
				minDenom = Flip4(minDenom);
				secNum = Flip4(secNum);
				secDenom = Flip4(secDenom);
			}

			// *** No check is made, whether the numerator exceed the maximum values,
			//	which is 90 for Latitude and 180 for Longitude
			// ! The Exif spec says denominator must not be zero, but in reality they can be

			char buffer[40];

			// Simplest case: all denominators are 1
			if ((degDenom == 1) && (minDenom == 1) && (secDenom == 1)) {

				snprintf(buffer, sizeof(buffer), "%lu,%lu,%lu%c", (unsigned long)degNum, (unsigned long)minNum, (unsigned long)secNum, ref);	// AUDIT: Using sizeof(buffer) is safe.

			}
			else if ((degDenom == 0 && degNum != 0) || (minDenom == 0 && minNum != 0) || (secDenom == 0 && secNum != 0)) {

				// Error case: a denominator is zero and the numerator is non-zero
				return; // Do not continue with import

			}
			else {

				// Determine precision
				// The code rounds up to the next power of 10 to get the number of fractional digits
				XMP_Uns32 maxDenom = degDenom;
				if (minDenom > maxDenom) maxDenom = minDenom;
				if (secDenom > maxDenom) maxDenom = secDenom;

				int fracDigits = 1;
				while (maxDenom > 10) { ++fracDigits; maxDenom = maxDenom / 10; }

				// Calculate the values
				// At this point we know that the fractions are either 0/0, 0/y or x/y

				double degrees, minutes;

				// Degrees
				if (degDenom == 0 && degNum == 0) {
					degrees = 0;
				}
				else {
					degrees = (double)((XMP_Uns32)((double)degNum / (double)degDenom));	// Just the integral number of degrees.
				}

				// Minutes
				if (minDenom == 0 && minNum == 0) {
					minutes = 0;
				}
				else {
					double temp = 0;
					if (degrees != 0) temp = ((double)degNum / (double)degDenom) - degrees;

					minutes = (temp * 60.0) + ((double)minNum / (double)minDenom);
				}

				// Seconds, are added to minutes
				if (secDenom != 0 && secNum != 0) {
					minutes += ((double)secNum / (double)secDenom) / 60.0;
				}

				snprintf(buffer, sizeof(buffer), "%.0f,%.*f%c", degrees, fracDigits, minutes, ref);	// AUDIT: Using sizeof(buffer) is safe.

			}

			xmp->SetProperty(xmpNS, xmpProp, buffer);

		}
		catch (...) {
			// Do nothing, let other imports proceed.
			// ? Notify client?
		}

	}	// ImportTIFF_GPSCoordinate

		// =================================================================================================
		// ImportArrayTIFF_Rational
		// ========================

	void ImportArrayTIFF_Rational(const TIFF_Manager::TagInfo & tagInfo, const bool nativeEndian,
		SXMPMeta * xmp, const char * xmpNS, const char * xmpProp)
	{
		try
		{	// Don't let errors with one stop the others.
			XMP_Uns32 * binPtr = (XMP_Uns32*)tagInfo.dataPtr;
			xmp->DeleteProperty(xmpNS, xmpProp);	// ! Don't keep appending, create a new array.
			for (size_t i = 0; i < tagInfo.count; ++i, binPtr += 2)
			{
				XMP_Uns32 binNum = GetUns32AsIs(&binPtr[0]);
				XMP_Uns32 binDenom = GetUns32AsIs(&binPtr[1]);
				if (!nativeEndian)
				{
					binNum = Flip4(binNum);
					binDenom = Flip4(binDenom);
				}
				char strValue[40];
				snprintf(strValue, sizeof(strValue), "%lu/%lu", (unsigned long)binNum, (unsigned long)binDenom);	// AUDIT: Using sizeof(strValue) is safe.
				xmp->AppendArrayItem(xmpNS, xmpProp, kXMP_PropArrayIsOrdered, strValue);
			}
		}
		catch (...)
		{
			// Do nothing, let other imports proceed.
			// ? Notify client?
		}

	}	// ImportArrayTIFF_Rational


#if AdobePrivate
		//Canon_MakerNote_Support Implementation
		//Currently only Timezone tag inside makernote tag is supported. For more details see [CTECHXMP-4169687]
	bool Canon_MakerNote_Support::ParseCanonMakerNoteTag(TIFF_Manager * mExifData, const XMP_Uns8* tiffSTream, SXMPMeta& xmpMetaObject) {

		TIFF_Manager::TagInfo tagInfo;
		bool makerInfo = mExifData->GetTag(kTIFF_ExifIFD, kEXIF_MakerNote_Tag, &tagInfo);
		if (makerInfo) {
			Canon_MakerNote_Support canonMakerTag(tiffSTream);

			bool isProcessed = canonMakerTag.ProcessMakerNoteIFD(mExifData, tagInfo);
			if (isProcessed == false)
				return false;

			TIFF_Manager::TagInfo TimeZonetagInfo;
			bool retValue = canonMakerTag.GetTag(kCanon_MakerNote_TimeZone_Tag, &TimeZonetagInfo);
			if (!retValue) return false;

			XMP_Int32 tz = 0;
			if (canonMakerTag.isBigEndian)
				tz = GetUns32BE((const XMP_Uns8*)TimeZonetagInfo.dataPtr + 4); //Currently not parsing city code and  daytime saving values
			else
				tz = GetUns32LE((const XMP_Uns8*)TimeZonetagInfo.dataPtr + 4);

			if (tz == 0) //no need to consider timezone info from EXIF block if timezone info is 0(i.e UTC)
				return false;

			XMP_Int8 sign = (tz < 0) ? -1 : 1;
			XMP_Int16 tzHr =(XMP_Int16)(abs(tz) / 60);
			XMP_Int16 tzMin = abs(tz) % 60;

			XMP_DateTime d;
			TIFF_Manager::TagInfo dateTimeTag;

			bool ret = mExifData->GetTag(kTIFF_ExifIFD, kTIFF_DateTimeOriginal, &dateTimeTag);
			
			if (ret) {
				GetTag_Date(*mExifData, dateTimeTag, d);
				d.hasTimeZone = true;
				d.tzSign = sign;
				d.tzHour = tzHr;
				d.tzMinute = tzMin;

				xmpMetaObject.SetProperty_Date(kXMP_NS_XMP, "CreateDate", d);
			}

			if (!ret) return ret;

			ret = mExifData->GetTag(kTIFF_PrimaryIFD, kTIFF_DateTime, &dateTimeTag);
			if (ret) {
				GetTag_Date(*mExifData, dateTimeTag, d);
				d.hasTimeZone = true;
				d.tzSign = sign;
				d.tzHour = tzHr;
				d.tzMinute = tzMin;

				xmpMetaObject.SetProperty_Date(kXMP_NS_XMP, "ModifyDate", d);
			}

			if (!ret) return ret;
		}

		return makerInfo;
	}

	bool Canon_MakerNote_Support::ProcessMakerNoteIFD(TIFF_Manager * mExifData, const TIFF_Manager::TagInfo& makerTag) {

		XMP_Uns8* ifdPtr = (XMP_Uns8*)makerTag.dataPtr;
		XMP_Uns16 ifdCount = 0;

		if (mExifData->IsBigEndian()) {
			ifdCount = GetUns16BE(ifdPtr);
			isBigEndian = true;
		}
		else
			ifdCount = GetUns16LE(ifdPtr);

		TweakedIFDEntry* ifdEntries = (TweakedIFDEntry*)(ifdPtr + 2);

		if (ifdCount >= 0x8000) {
			return false;
		}

		count = ifdCount;
		entries = ifdEntries;
		return true;
	}

	bool Canon_MakerNote_Support::GetTag(XMP_Uns16 id, TIFF_Manager::TagInfo* info) const {
		if (entries == NULL)
			return false;

		bool retValue = false;

		for (XMP_Uns16 index = 0; index < count;++index) {
			TweakedIFDEntry& ifdInfo = entries[index];
			if (ifdInfo.id == id && info != NULL) {
				XMP_Uns16 thisType = 0;
				XMP_Uns32 thisBytes = 0;
				XMP_Uns32 dataOrPos = 0;

				if (isBigEndian) {
					thisType = GetUns16BE(&(ifdInfo.type));
					thisBytes = GetUns32BE(&(ifdInfo.bytes));
					dataOrPos = GetUns32BE(&(ifdInfo.dataOrPos));
				}
				else {
					thisType = GetUns16LE(&(ifdInfo.type));
					thisBytes = GetUns32LE(&(ifdInfo.bytes));
					dataOrPos = GetUns32LE(&(ifdInfo.dataOrPos));
				}

				if ((thisType < kTIFF_ByteType) || (thisType > kTIFF_LastType)) return false;	// Bad type, skip this tag.
				if (isBigEndian)
					info->id = GetUns16BE(&(ifdInfo.id));
				else
					info->id = GetUns16BE(&(ifdInfo.id));

				info->type = thisType;
				info->count = thisBytes;
				info->dataLen = thisBytes * (XMP_Uns32)kTIFF_TypeSizes[thisType];

				if (info->dataLen <= 4) {
					info->dataPtr = &(ifdInfo.dataOrPos);
				}
				else {
					info->dataPtr = (mTiff_stream + dataOrPos);
				}
				retValue = true;
				break;
			}
		}

		return retValue;
	}
#endif //AdobePrivate

}      // OEM_Support

	   // ================================================================================================