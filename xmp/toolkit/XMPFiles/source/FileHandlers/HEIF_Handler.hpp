#if AdobePrivate	// ! This entire handler is private, it must not go in the public SDK.

#ifndef __HEIF_Handler_hpp__
#define __HEIF_Handler_hpp__	1
// =================================================================================================
// Copyright Adobe
// Copyright 2018 Adobe
// All Rights Reserved
//
// NOTICE: Adobe permits you to use, modify, and distribute this file in accordance with the terms
// of the Adobe license agreement accompanying it. If you have received this file from a source other 
// than Adobe, then your use, modification, or distribution of it requires the prior written permission
// of Adobe.
// =================================================================================================


#include "XMPFiles/source/XMPFiles_Impl.hpp"

#include "XMPFiles/source/FormatSupport/META_Support.hpp"
#include "XMPFiles/source/FormatSupport/TIFF_Support.hpp"


extern XMPFileHandler * HEIF_MetaHandlerCTor(XMPFiles * parent);

extern bool HEIF_CheckFormat(XMP_FileFormat format,
	XMP_StringPtr  filePath,
	XMP_IO*    fileRef,
	XMPFiles *     parent);


static const XMP_OptionBits kHEIF_HandlerFlags = (kXMPFiles_CanInjectXMP |
	kXMPFiles_CanExpand |
	kXMPFiles_CanRewrite | // If WriteTempFile() implemented
	//kXMPFiles_PrefersInPlace |
	kXMPFiles_CanReconcile |
	kXMPFiles_AllowsOnlyXMP |	
	kXMPFiles_ReturnsRawPacket |
	kXMPFiles_AllowsSafeUpdate |
	//kXMPFiles_CanNotifyProgress |  //todo if implement progresstracker 
	kXMPFiles_NeedsLocalFileOpened
	);
	

class HEIF_MetaHandler : public XMPFileHandler
{
public:
	void CacheFileData();
	void ProcessXMP();

	void UpdateFile(bool doSafeUpdate);
	void WriteTempFile(XMP_IO* tempRef);

	HEIF_MetaHandler(XMPFiles * _parent);
	virtual ~HEIF_MetaHandler();
	
	

private:
	Meta_Manager metaMgr;
	TIFF_Manager * exifMgr;
	XMP_Uns32 metaBoxSize;
	XMP_Uns64 metaBoxPos; // The file offset of the 'meta' box (the size field, not the content).
	XMP_Uns32 primaryItemID;

	bool isProcessXMPFailed;
	bool validDataInfoBoxPresent;
	//XMP_Uns8 fileMode // TODO stage 2 use this to differentiate between differnt heif filetypes like image vs image seq vs video

	XMP_Uns32 xmpItemID;
	XMP_Uns32 exifItemID;
	Meta_Manager::ExtentIDMap extentIDMap;
	RawDataBlock exifData;

	struct MdatInfo {
		XMP_Uns64 offset, size;
		XMP_Uns32 headerSize;
		MdatInfo() : offset(0), size(0), headerSize(0) {};
		MdatInfo(XMP_Uns64 _offset, XMP_Uns64 _size, XMP_Uns32 _headersize) : offset(_offset), size(_size), headerSize(_headersize) {};
	};
	std::vector<MdatInfo> mdatList;

	bool CheckItemExistsForItemType(XMP_Uns32 itemType, XMP_Uns32 & itemID, std::string contenttype = "") const;
	bool CheckValidExistanceInIlocBox(XMP_Uns32 itemID, Meta_Manager::ilocItem &) const;
	//Try to import both XMP and EXIF metadata in this function
	bool ImportMetaData();
	bool ImportExifData();
	bool ImportXMPData();
	XMP_Uns32 GetItemData(XMP_Uns32 itemID, XMP_Uns32 sLimit, RawDataBlock &contents, XMP_Int64 &fileOffset);
	bool SetMdatBoxFree(XMP_IO* fileRef, XMP_Uns32 dataSize, XMP_Uns64 curExtentOffset);
	void CreateMdatBoxList(XMP_IO* fileRef, XMP_Uns64 fileSize);
	bool GetParentMdatInfo(XMP_Uns64 dataOffset, MdatInfo &mdatInfo);
	XMP_Uns32 GetDataByConstructionMethodOne(Meta_Manager::ilocItem& ilocItem, XMP_Uns32 sizeLimit, RawDataBlock &contents, XMP_Int64 &fileOffset);
	XMP_Uns32 GetDataByConstructionMethodZero(Meta_Manager::ilocItem& ilocItem, XMP_Uns32 sizeLimit, RawDataBlock &contents, XMP_Int64 &fileOffset);
	XMP_Uns32 GetDataByConstructionMethodTwo(Meta_Manager::ilocItem& ilocItem, XMP_Uns32 sizeLimit, RawDataBlock &contents, XMP_Int64 &fileOffset, XMP_Uns32 itemID);
	void ModifyDataByConstructionMethodZero(Meta_Manager::ilocItem &ilocItem, XMP_Uns32 itemID, XMP_Uns8 * packet, XMP_Uns32 size);
	void AddFileLevelMdatBox();
	bool ModifyItemData(XMP_Uns32 itemID, XMP_Uns32 size, XMP_Uns8 * packet);
	void UpdateItemExtentOffset(XMP_Int64 xmpoffset, XMP_Uns32 itemID, size_t extentIndex);
	bool AddNewResourceEntries(XMP_Uns32 length, XMP_Uns32 itemType, XMP_Uns32 &itemID);
	void UpdateTopLevelBox(XMP_Uns64 oldOffset, XMP_Uns32 oldSize, const XMP_Uns8 * newBox, XMP_Uns32 newSize);
	XMP_Int64 AddTopLevelBox(const XMP_Uns8 * newBox, XMP_Uns32 newSize);
	void UpdateXMPOffset(XMP_Uns32);
	void UpdateOperation(bool fileHadXMP);
	void UpdateTempFile(XMP_IO* tempRef, bool fileHadXMP);

}; // HEIF_MetaHandler

// =================================================================================================

#endif // __HEIF_Handler_hpp__

#endif	// AdobePrivate	// ! This entire handler is private, it must not go in the public SDK.
