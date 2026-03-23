#if AdobePrivate	// ! This entire handler is private, it must not go in the public SDK.

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

#include "public/include/XMP_Environment.h"	// ! XMP_Environment.h must be the first included header.

#include <iterator>
#include <algorithm>

#include "public/include/XMP_Const.h"
#include "public/include/XMP_IO.hpp"

#include "XMPFiles/source/XMPFiles_Impl.hpp"
#include "source/XMPFiles_IO.hpp"
#include "source/XIO.hpp"
#include "XMPFiles/source/FileHandlers/HEIF_Handler.hpp"
#include "XMPFiles/source/FormatSupport/ISOBaseMedia_Support.hpp"
#include "XMPFiles/source/FormatSupport/TIFF_Support.hpp"
#include "XMPFiles/source/FormatSupport/Reconcile_Impl.hpp"

#define DataSizeLimit 1024*1024*1024 
#define CheckInt64LimitThrow(offset) if (offset > Max_XMP_Int64) { XMP_Error error( kXMPErr_EnforceFailure, "Offset out of Int64 bound"); \
XMPFileHandler::NotifyClient(&parent->errorCallback, kXMPErrSev_FileFatal, error); } \

// =================================================================================================
// HEIF_CheckFormat
// ====================
bool HEIF_CheckFormat(XMP_FileFormat format,
	XMP_StringPtr  filePath,
	XMP_IO*    fileRef,
	XMPFiles *     parent) 
{
	XMP_Uns8  buffer[4 * 1024];
	XMP_Uns32 ioCount, brandCount, brandOffset;
	XMP_Uns64 fileSize, nextOffset;
	ISOMedia::BoxInfo currBox;

	XMP_AbortProc abortProc = parent->abortProc;
	void *        abortArg = parent->abortArg;
	const bool    checkAbort = (abortProc != 0);

	bool openStrictly = XMP_OptionIsSet(parent->openFlags, kXMPFiles_OpenStrictly);

	XMP_Assert((parent->tempPtr == 0) && (parent->tempUI32 == 0));

	fileSize = fileRef->Length();
	if (fileSize < 8) return false;

	nextOffset = ISOMedia::GetBoxInfo(fileRef, 0, fileSize, &currBox);
	if (currBox.headerSize < 8) return false;

	if (currBox.boxType != ISOMedia::k_ftyp) return false;

	// No compatible brands at all. ||  Sanity check and make sure count fits in 32 bits.
	if (currBox.contentSize < 12 || currBox.contentSize > 1024 * 1024) return false;	

	brandCount = ((XMP_Uns32)currBox.contentSize - 8) >> 2;

	fileRef->Seek(8, kXMP_SeekFromCurrent);	// Skip major brand minor version.
	ioCount = brandOffset = 0;

	bool haveCompatibleBrand = false, haveCompulsaryBrand = false;
	for (; brandCount > 0; --brandCount, brandOffset += 4) {
		if (brandOffset >= ioCount) {
			if (checkAbort && abortProc(abortArg)) {
				XMP_Throw("HEIF_CheckFormat - User abort", kXMPErr_UserAbort);
			}
			ioCount = 4 * brandCount;
			if (ioCount > sizeof(buffer)) ioCount = sizeof(buffer);
			ioCount = fileRef->ReadAll(buffer, ioCount);
			brandOffset = 0;
		}
		XMP_Uns32 brand = GetUns32BE(&buffer[brandOffset]);
		if (brand == ISOMedia::k_mif1)
			haveCompulsaryBrand = true;
		if (brand == ISOMedia::k_mif1 || brand == ISOMedia::k_heic || brand == ISOMedia::k_jpeg || brand == ISOMedia::k_heix || brand == ISOMedia::k_avc1) {
			haveCompatibleBrand = true;
		}

	}

	if (!(haveCompatibleBrand && haveCompulsaryBrand)) return false;
	if (openStrictly && (format != kXMP_HEIFFile)) return false;
	parent->format = kXMP_HEIFFile;
	return true;

	
}

// =================================================================================================
// HEIF_MetaHandlerCTor
// ====================
XMPFileHandler * HEIF_MetaHandlerCTor(XMPFiles * parent)
{

	return new HEIF_MetaHandler(parent);

}	// HEIF_MetaHandlerCTor

// =================================================================================================
// HEIF_MetaHandler::HEIF_MetaHandler
// =============================
HEIF_MetaHandler::HEIF_MetaHandler(XMPFiles * _parent)
	: metaBoxPos(0), metaBoxSize(0), exifMgr(0)
	{
	this->parent = _parent;	// Inherited, can't set in the prefix.
	this->handlerFlags = kHEIF_HandlerFlags;
	this->stdCharForm = kXMP_Char8Bit;
	//this->fileMode = (XMP_Uns8)_parent->tempUI32;
	_parent->tempUI32 = 0;
	isProcessXMPFailed = false;

}	// HEIF_MetaHandler::HEIF_MetaHandler

// =================================================================================================
// HEIF_MetaHandler::~HEIF_MetaHandler
// =============================
HEIF_MetaHandler::~HEIF_MetaHandler()
{

	if (exifMgr != 0) delete (exifMgr);

}	// HEIF_MetaHandler::~HEIF_MetaHandler

// =================================================================================================
// HEIF_MetaHandler::CacheFileData
// =============================
void HEIF_MetaHandler::CacheFileData() 
{
	XMPFiles * parent = this->parent;
	XMP_OptionBits openFlags = parent->openFlags;

	XMP_IO* fileRef = parent->ioRef;

	XMP_AbortProc abortProc = parent->abortProc;
	void *        abortArg = parent->abortArg;
	const bool    checkAbort = (abortProc != 0);


	XMP_Int64 fileLength = fileRef->Length();

	XMP_Int64 boxPos; XMP_Uns64 boxNext;
	ISOMedia::BoxInfo currBox;

	bool metaPresent = 0;
	for (boxPos = 0; boxPos < fileLength; boxPos = boxNext) {

		if (checkAbort && abortProc(abortArg)) {
			XMP_Throw("HEIF_MetaHandler::CacheFileData - User abort", kXMPErr_UserAbort);
		}

		boxNext = ISOMedia::GetBoxInfo(fileRef, boxPos, fileLength, &currBox);
		if (currBox.boxType != ISOMedia::k_meta)
			continue;

		metaPresent = 1;
		
		if (currBox.contentSize < 4) //meta box is a full box, so size should not be less than 4
		{
			XMP_Error err(kXMPErr_BadValue, "Essential boxes for metadata processing missing/malformed");
			XMPFileHandler::NotifyClient(&parent->errorCallback, kXMPErrSev_FileFatal, err);
		}
			

		XMP_Uns64 fullMetaSize = currBox.headerSize + currBox.contentSize;
		if (fullMetaSize > TopBoxSizeLimit) {	// From here on we know 32-bit offsets are safe.
			XMP_Error err(kXMPErr_BadValue, "Oversize 'meta' box");
			XMPFileHandler::NotifyClient(&parent->errorCallback, kXMPErrSev_FileFatal, err);
			
		}

		this->metaMgr.fullSubtree.assign((XMP_Uns32)fullMetaSize, 0);
		fileRef->Seek(boxPos, kXMP_SeekFromStart);
		fileRef->Read(this->metaMgr.fullSubtree.data(), (XMP_Uns32)fullMetaSize);
		metaBoxPos = boxPos;
		metaBoxSize = (XMP_Uns32)fullMetaSize;
		
		XMP_Uns8  metaVersion = *(this->metaMgr.fullSubtree.data() + 8);

		if(metaVersion != 0)
			metaPresent = 0;
		break;
	}
	if (!metaPresent) {
		XMP_Error err(kXMPErr_BadValue, "Essential boxes for metadata processing missing/malformed");
		XMPFileHandler::NotifyClient(&parent->errorCallback, kXMPErrSev_FileFatal, err);
	}
	
	
}

// =================================================================================================
// HEIF_MetaHandler::ProcessXMP
// =============================
void HEIF_MetaHandler::ProcessXMP()
{
	
	try {

		if (isProcessXMPFailed)
		{
			XMP_Error err(kXMPErr_BadValue, "Essential boxes for metadata processing missing/malformed");
			XMPFileHandler::NotifyClient(&parent->errorCallback, kXMPErrSev_FileFatal, err);
		}

		if (this->processedXMP) return;
		this->processedXMP = true;	// Make sure only called once.

		XMPFiles * parent = this->parent;
		XMP_OptionBits openFlags = parent->openFlags;
		bool readOnly = (!(openFlags & kXMPFiles_OpenForUpdate));
		if (exifMgr == 0) {
			if (readOnly)
				exifMgr = new TIFF_MemoryReader();
			else
				exifMgr = new TIFF_FileWriter();
		}

		

		// Parse the cached 'meta' subtree
		this->metaMgr.ParseMetaMemoryTree();

		//Check for mandatory hdlr box in meta box
		ISOBaseMedia_Manager::BoxInfo hdlrInfo;
		ISOBaseMedia_Manager::BoxRef  hdlrRef = this->metaMgr.GetTypeChild(&(this->metaMgr.subtreeRootNode), ISOMedia::k_hdlr, &hdlrInfo);
		if (hdlrInfo.contentSize <= 8 || hdlrInfo.content == 0) {
			XMP_Error err(kXMPErr_BadValue, "Essential boxes for metadata processing missing/malformed");
			XMPFileHandler::NotifyClient(&parent->errorCallback, kXMPErrSev_FileFatal, err);
		}
			
		const ISOBaseMedia_Manager::Content_hdlr * hdlr = (ISOBaseMedia_Manager::Content_hdlr*)hdlrInfo.content;

		// Throw exception to inform user. Mandatory to have handler type pict
		if (hdlr->versionFlags != 0 || GetUns32BE(&hdlr->handlerType) != ISOMedia::k_pict) {
			XMP_Error err(kXMPErr_BadValue, "Essential boxes for metadata processing missing/malformed");
			XMPFileHandler::NotifyClient(&parent->errorCallback, kXMPErrSev_FileFatal, err);
		}

		bool present = this->metaMgr.ImportItemInformation();
		present &= this->metaMgr.ImportItemLocationInformation();

		/* We'll not be able to put XMP as well if iinf and illoc not prsent, or corrupted.
		** As we wouldn't want to delete and items that may be present or add metadata even if no image info is present
		** So throw exception in this scenario
		*/
		if (!present) {
			XMP_Error err(kXMPErr_BadValue, "Essential boxes for metadata processing missing/malformed");
			XMPFileHandler::NotifyClient(&parent->errorCallback, kXMPErrSev_FileFatal, err);
		}

		present &= this->metaMgr.ImportItemReferenceBox();

		//Get primary ItemId
		ISOBaseMedia_Manager::BoxInfo pitmInfo;
		ISOBaseMedia_Manager::BoxRef  pitmRef = this->metaMgr.GetTypeChild(&(this->metaMgr.subtreeRootNode), ISOMedia::k_pitm, &pitmInfo);

		// Not processing further, Throw exception to inform user of this case. We get metadata of only primary item
		if (pitmInfo.content == nullptr) {
			XMP_Error err(kXMPErr_BadValue, "Essential boxes for metadata processing missing/malformed");
			XMPFileHandler::NotifyClient(&parent->errorCallback, kXMPErrSev_FileFatal, err);
		}
			
		XMP_Uns8 pitmversion = pitmInfo.content[0];

		if (pitmversion > 1) {
			XMP_Error err(kXMPErr_BadValue, "Essential boxes for metadata processing missing/malformed");
			XMPFileHandler::NotifyClient(&parent->errorCallback, kXMPErrSev_FileFatal, err);
		}
		if (pitmversion == 0 && pitmInfo.contentSize == 6)
			primaryItemID = GetUns16BE(pitmInfo.content + 4);
		else if (pitmversion == 1 && pitmInfo.contentSize == 8)
			primaryItemID = GetUns32BE(pitmInfo.content + 4);
		else {
			XMP_Error err(kXMPErr_BadValue, "Essential boxes for metadata processing missing/malformed");
			XMPFileHandler::NotifyClient(&parent->errorCallback, kXMPErrSev_FileFatal, err);
		}

		XMP_Assert(parent->ioRef != nullptr);

		if (present) {
			this->containsXMP = ImportMetaData();
		}
	}
	catch (...)
	{
		isProcessXMPFailed = true;
		this->processedXMP = false;
		throw;
	}
}

// =================================================================================================
// HEIF_MetaHandler::CheckValidExistanceInIlocBox
// =============================
bool HEIF_MetaHandler::CheckValidExistanceInIlocBox(XMP_Uns32 itemID, Meta_Manager::ilocItem & ilocItem) const{

	std::map<XMP_Uns32, Meta_Manager::ilocItem>::const_iterator itr = this->metaMgr.itemLocationMap.find(itemID);

	if (itr == this->metaMgr.itemLocationMap.end())
		return false;
	
	ilocItem = itr->second;
	
	//Protection scheme already checked in CheckItemExistsForItemType API

	if (ilocItem.iExtents.size() < 1)
		return false; //mandatory to have atleast 1 extent
	
	if (ilocItem.dataReferenceIndex != 0 ) {
		if ((!this->validDataInfoBoxPresent) || (this->validDataInfoBoxPresent && this->metaMgr.dinfFlags[ilocItem.dataReferenceIndex - 1] != 1))
			return false;
	}

	return true;
}

// =================================================================================================
// HEIF_MetaHandler::CheckItemExistsForItemType
// =============================
//
// Here itemID is filled with id of item which is linked with primary item, in case item_type is Exif 
bool HEIF_MetaHandler::CheckItemExistsForItemType(XMP_Uns32 itemType, XMP_Uns32 & itemID, std::string contenttype ) const{
	bool found = false;
	for (Meta_Manager::itemInfoEntry item : this->metaMgr.itemInfoEntryList) {
		if (item.itemType != itemType)
			continue;

		if (item.itemProtectionIndex != 0) //not supporting protection schemes 
			continue;// continue, to check next items if there exists another item with given item type

		Meta_Manager::ItemReferenceTypeMap::const_iterator it = this->metaMgr.irefBox.find(ISOMedia::k_cdsc);
		if (it == this->metaMgr.irefBox.end())
			return found;
		Meta_Manager::RelatedItemsMap itemsMap = it->second;
		Meta_Manager::RelatedItemsMap::iterator vecItr= itemsMap.find(item.itemID); //item id of metadata item
		if (vecItr == itemsMap.end())
			continue;

		std::vector<XMP_Uns32> &toItems = vecItr->second;
		if (std::find(toItems.begin(), toItems.end(), primaryItemID) != toItems.end()) {
			if (contenttype.size() == 0 || (contenttype.size() != 0 && item.contentType == contenttype)) {
				itemID = item.itemID;
				found = true;
			}
		}
		if (found)  //If not found, continue to check others
			break;
	}
	return found;
}

// =================================================================================================
// HEIF_MetaHandler::GetItemData
// =============================
XMP_Uns32 HEIF_MetaHandler::GetItemData(XMP_Uns32 itemID, XMP_Uns32 sizeLimit, RawDataBlock &contents,XMP_Int64 &fileOffset) {

	contents.clear(); //clearing already there contents, if any // sanity
	
	Meta_Manager::ilocItem ilocItem;
	if (!CheckValidExistanceInIlocBox(itemID, ilocItem))
		return 0;
	
	
	const size_t numExtents = ilocItem.iExtents.size();
	size_t constructionMethod = ilocItem.constructionMethod;
    
    XMP_Uns32 dataSize = 0;

	switch (constructionMethod) {

		case 0 : dataSize = GetDataByConstructionMethodZero(ilocItem, sizeLimit, contents, fileOffset); break;
		case 1 : dataSize = GetDataByConstructionMethodOne(ilocItem, sizeLimit, contents, fileOffset); break;
		case 2 : dataSize = GetDataByConstructionMethodTwo(ilocItem, sizeLimit, contents, fileOffset, itemID); break;
	}
	
	return dataSize;
}

// =================================================================================================
// HEIF_MetaHandler::GetDataForConstructionMethodZero
// =============================
XMP_Uns32 HEIF_MetaHandler::GetDataByConstructionMethodZero(Meta_Manager::ilocItem& ilocItem, XMP_Uns32 sizeLimit, RawDataBlock &contents, XMP_Int64 &fileOffset) {
	
	XMP_IO* fileRef = this->parent->ioRef;
	const size_t numExtents = ilocItem.iExtents.size();
	size_t vecIndex = 0;
	XMP_Uns32 totalExtentSize = 0;
	XMP_Uns64 extentOffset = 0;
	XMP_Uns64 extentLength = 0;

	for (size_t ext = 0; ext < numExtents; ext++) {
		if (ilocItem.iExtents[ext].extent_length == 0)
			ilocItem.iExtents[ext].extent_length = fileRef->Length();	// Should not happen in our case, since we are not considering anyother file at data refrence Index, but only the current file
		totalExtentSize += (XMP_Uns32)ilocItem.iExtents[ext].extent_length;
	}

	//Decision: Throwing exception in case data is greater than 2GB
	if (!(totalExtentSize < sizeLimit)) {
		XMP_Error error(kXMPErr_EnforceFailure, "Data size greater than the limit.");
		XMPFileHandler::NotifyClient(&parent->errorCallback, kXMPErrSev_FileFatal, error);
	}
		

	contents.assign(totalExtentSize, 0);

	for (size_t ext = 0; ext < numExtents; ext++) {
		extentOffset = ilocItem.iExtents[ext].extent_offset;
		extentLength = ilocItem.iExtents[ext].extent_length;
		fileRef->Seek(ilocItem.iExtents[ext].extent_offset, kXMP_SeekFromStart);
		if (ext == 0)
			fileOffset = fileRef->Offset(); //Decison: Consider fileoffset as the offset of first extent

		fileRef->Read(contents.data() + vecIndex, (XMP_Uns32)ilocItem.iExtents[ext].extent_length);

		//Not allowing sizes which are greater than 2^32 (So can be casted to XMP_Uns32)
		vecIndex += (XMP_Uns32)ilocItem.iExtents[ext].extent_length;
	}

	if (vecIndex != totalExtentSize)
		return 0;

	return totalExtentSize;
}

// =================================================================================================
// HEIF_MetaHandler::GetDataForConstructionMethodOne
// =============================
XMP_Uns32 HEIF_MetaHandler::GetDataByConstructionMethodOne(Meta_Manager::ilocItem& ilocItem, XMP_Uns32 sizeLimit, RawDataBlock &contents, XMP_Int64 &fileOffset) {

	XMP_IO* fileRef = this->parent->ioRef;
	const size_t numExtents = ilocItem.iExtents.size();
	size_t vecIndex = 0;
	XMP_Uns32 totalExtentSize = 0;
	XMP_Uns64 extentOffset = 0;
	XMP_Uns64 extentLength = 0;

	ISOBaseMedia_Manager::BoxInfo idatInfo;
	ISOBaseMedia_Manager::BoxRef  idatRef = this->metaMgr.GetTypeChild(&(this->metaMgr.subtreeRootNode), ISOMedia::k_idat, &idatInfo);

	for (size_t ext = 0; ext < numExtents; ext++) {
		if (ilocItem.iExtents[ext].extent_length == 0)
			ilocItem.iExtents[ext].extent_length = idatInfo.contentSize;

		totalExtentSize += (XMP_Uns32)ilocItem.iExtents[ext].extent_length;
	}

	if (!(totalExtentSize < sizeLimit)) {
		XMP_Error error(kXMPErr_EnforceFailure, "Data size greater than the limit.");
		XMPFileHandler::NotifyClient(&parent->errorCallback, kXMPErrSev_FileFatal, error);
	}

	contents.assign(totalExtentSize, 0);

	// idat is a box
	if (idatInfo.content == nullptr)
		return 0;

	const XMP_Uns8 * idatLimit = idatInfo.content + idatInfo.contentSize;
	ISOBaseMedia_Manager::BoxNode * idatNode = (ISOBaseMedia_Manager::BoxNode*)idatRef;
	for (size_t ext = 0; ext < numExtents; ext++) {
		extentOffset = ilocItem.iExtents[ext].extent_offset;
		extentLength = ilocItem.iExtents[ext].extent_length;

		if (ext == 0)
			fileOffset = metaBoxPos + idatNode->offset + idatNode->headerSize + extentOffset;

		const XMP_Uns8 *endpart = idatInfo.content + extentOffset + (XMP_Uns32)extentLength;
		if (endpart > idatLimit)
			return 0; // Not processing any further as extent data exceeding idat content
		std::copy(idatInfo.content + extentOffset, idatInfo.content + extentOffset + (XMP_Uns32)extentLength, contents.begin() + vecIndex);
		vecIndex += (XMP_Uns32)extentLength;
	}

	if (vecIndex != totalExtentSize)
		return 0;

	return totalExtentSize;
}

// =================================================================================================
// HEIF_MetaHandler::GetDataForConstructionMethodTwo
// =============================
XMP_Uns32 HEIF_MetaHandler::GetDataByConstructionMethodTwo(Meta_Manager::ilocItem& ilocItem, XMP_Uns32 sizeLimit, RawDataBlock &contents, XMP_Int64 &fileOffset, XMP_Uns32 itemID){

	XMP_IO* fileRef = this->parent->ioRef;
	const size_t numExtents = ilocItem.iExtents.size();
	size_t vecIndex = 0;
	XMP_Uns32 totalExtentSize = 0;
	XMP_Uns64 extentOffset = 0;
	XMP_Uns64 extentLength = 0;

	Meta_Manager::ItemReferenceTypeMap::iterator itr = this->metaMgr.irefBox.find(ISOMedia::k_iloc);
	if (itr == this->metaMgr.irefBox.end())	return 0;

	Meta_Manager::RelatedItemsMap::iterator itrItemId = itr->second.find(itemID);
	if (itrItemId == itr->second.end())	return 0;

	std::vector<XMP_Uns32> &toItems = itrItemId->second;
	XMP_Int64 storeOffset, tempOffset; //To be set later when all is processed

	for (size_t ext = 0; ext < numExtents; ext++) {
		Meta_Manager::extent cExtent = ilocItem.iExtents[ext];
		XMP_Uns32 refitemID = toItems[cExtent.extent_index - 1];
		RawDataBlock refItemContents;

		//try to insert refitemID in set
		if (this->metaMgr.traversed.insert(refitemID).second == false) //check to prevent infinite recursion, if present do not process further
			return 0;

		//this->metaMgr.traversed.insert(refitemID); //We have processed this item

		XMP_Uns32 dataLength = this->GetItemData(refitemID, sizeLimit, refItemContents, tempOffset);//  check logic again
		if (ext == 0)
			storeOffset = tempOffset + cExtent.extent_offset; //Ofset of otherItem's data + this extent's offset

		if (cExtent.extent_length == 0)
			cExtent.extent_length = dataLength; // Length of entire source

		totalExtentSize += (XMP_Uns32)cExtent.extent_length;

		if (!(totalExtentSize < sizeLimit)) {
			XMP_Error error(kXMPErr_EnforceFailure, "Data size greater than the limit.");
			XMPFileHandler::NotifyClient(&parent->errorCallback, kXMPErrSev_FileFatal, error);
		}

		if (dataLength < 0)
			return 0;

		std::copy(refItemContents.begin() + cExtent.extent_offset, refItemContents.begin() + cExtent.extent_offset + cExtent.extent_length, std::back_inserter(contents));
		vecIndex += (XMP_Uns32)cExtent.extent_length;
	}

	if (vecIndex != totalExtentSize)
		return 0;

	fileOffset = storeOffset; 

	return totalExtentSize;
}

// =================================================================================================
// HEIF_MetaHandler::ImportMetaData
// =============================
bool HEIF_MetaHandler::ImportMetaData() {

	this->validDataInfoBoxPresent = this->metaMgr.ImportDataInformationBox();
	bool exifPresent = false, xmpPresent = false;

	xmpPresent = ImportXMPData();

	exifPresent = ImportExifData();
	
	
	IPTC_Reader iptc;
	PSIR_MemoryReader psir;
	XMP_OptionBits options = 0;
	if (xmpPresent) options |= k2XMP_FileHadXMP;
	if(exifPresent)	options |= k2XMP_FileHadExif;

	ImportPhotoData(*exifMgr, iptc, psir, kDigestDiffers, &this->xmpObj, options);

	return xmpPresent | exifPresent;
}

// =================================================================================================
// HEIF_MetaHandler::ImportExifData
// =============================
bool HEIF_MetaHandler::ImportExifData() {
	XMP_Uns32 exif_id = 0;
	bool found = CheckItemExistsForItemType(ISOMedia::k_Exif, exif_id); //this will mainly check in infe or iref box
	
	if (!found)
		return false;

	this->metaMgr.traversed.clear();   // at a time ony one type of metadata will be processsed, so clearing before it
	this->metaMgr.traversed.insert(exif_id);
	XMP_Int64 fileoffset = 0;
	XMP_Uns32 dataSize = this->GetItemData(exif_id, DataSizeLimit, this->exifData, fileoffset);
	if (dataSize == 0) {
		this->exifData.clear();
		fileoffset = 0;
		return false;
	}
		
	const XMP_Uns8 * exifOrigin = this->exifData.data();
	XMP_Uns32 exifHeaderOffset = GetUns32BE(exifOrigin);
	exifOrigin = exifOrigin + 4 + exifHeaderOffset;
	XMP_Uns32 exifSize = dataSize - 4 - exifHeaderOffset;
	try {
		exifMgr->ParseMemoryStream(exifOrigin, exifSize);
		exifItemID = exif_id;
		return true;
	}
	catch (...) {
		//skip parsing failure and try to get XMP if prrsent
		return false;
	}
	
}

// =================================================================================================
// HEIF_MetaHandler::ImportXMPData
// =============================
bool HEIF_MetaHandler::ImportXMPData() {
	bool  found = CheckItemExistsForItemType(ISOMedia::k_mime, xmpItemID, "application/rdf+xml");
	if (!found)
		return false;

	this->metaMgr.traversed.clear();
	this->metaMgr.traversed.insert(xmpItemID);
	RawDataBlock xmpData;
	XMP_Int64 fileOffset;
	XMP_Uns32 dataSize = this->GetItemData(xmpItemID, DataSizeLimit, xmpData, fileOffset);
	if (dataSize == 0) {
		xmpData.clear();
		fileOffset = 0;
		return false;
	}
		
	XMP_StringPtr xmpOrigin = (XMP_StringPtr)xmpData.data();
	xmpPacket.assign((char*)xmpOrigin, dataSize);
	//packetInfo.offset = contentOrigin + kMainXMPSignatureLength; 
	packetInfo.length = (XMP_Int32)dataSize;
	packetInfo.padSize = 0;	// Assume the rest for now, set later in ProcessXMP.
	packetInfo.charForm = kXMP_CharUnknown;
	packetInfo.writeable = true;
	//Following is as done in JPEG_Handler
	try {
		xmpObj.ParseFromBuffer(xmpOrigin, dataSize);
	}
	catch (...) { 
		/* Ignore parsing failures, Try to process Exif block */
		return false;
	}
	packetInfo.offset = fileOffset; //only if all is returned well should this offset be set.
	return true;
	
}

// =================================================================================================
// CheckFinalBox
// =============
//
// Before appending anything new, check if the final top level box has a "to EoF" length. If so, fix
// it to have an explicit length.

static void CheckFinalBox(XMP_IO* fileRef, XMP_AbortProc abortProc, void * abortArg)
{
	const bool checkAbort = (abortProc != 0);

	XMP_Uns64 fileSize = fileRef->Length();

	// Find the last 2 boxes in the file. Need the previous to last in case it is an Apple 'wide' box.

	XMP_Uns64 prevPos, lastPos, nextPos;
	ISOMedia::BoxInfo prevBox, lastBox;
	XMP_Uns8 buffer[16];	// Enough to create an extended header.

	memset(&prevBox, 0, sizeof(prevBox));	// AUDIT: Using sizeof(prevBox) is safe.
	memset(&lastBox, 0, sizeof(lastBox));	// AUDIT: Using sizeof(lastBox) is safe.
	prevPos = lastPos = nextPos = 0;
	while (nextPos != fileSize) {
		if (checkAbort && abortProc(abortArg)) {
			XMP_Throw("CheckFinalBox - User abort", kXMPErr_UserAbort);
		}
		prevBox = lastBox;
		prevPos = lastPos;
		lastPos = nextPos;
		nextPos = ISOMedia::GetBoxInfo(fileRef, lastPos, fileSize, &lastBox, true /* throw errors */);
	}

	// See if the last box is valid and has a "to EoF" size.

	if (lastBox.headerSize < 8) XMP_Throw("HEIF Final box is invalid", kXMPErr_EnforceFailure);
	fileRef->Seek(lastPos, kXMP_SeekFromStart);
	fileRef->Read(buffer, 4);
	XMP_Uns64 lastSize = GetUns32BE(&buffer[0]);	// ! Yes, the file has a 32-bit value.
	if (lastSize != 0) return;

	// Have a final "to EoF" box, try to write the explicit size.

	lastSize = lastBox.headerSize + lastBox.contentSize;
	if (lastSize <= 0xFFFFFFFFUL) {

		// Fill in the 32-bit exact size.
		PutUns32BE((XMP_Uns32)lastSize, &buffer[0]);
		fileRef->Seek(lastPos, kXMP_SeekFromStart);
		fileRef->Write(buffer, 4);

	}
	else {

		// Try to convert to using an extended header.

		if ((prevBox.boxType != ISOMedia::k_wide) || (prevBox.headerSize != 8) || (prevBox.contentSize != 0)) {
			XMP_Throw("Can't expand final box header", kXMPErr_EnforceFailure);
		}
		XMP_Assert(prevPos == (lastPos - 8));

		PutUns32BE(1, &buffer[0]);
		PutUns32BE(lastBox.boxType, &buffer[4]);
		PutUns64BE(lastSize, &buffer[8]);
		fileRef->Seek(prevPos, kXMP_SeekFromStart);
		fileRef->Write(buffer, 16);

	}

}	// CheckFinalBox

static void setBytesNull(XMP_IO *fileRef, XMP_Uns64 offset, XMP_Uns32 length) {
	if (offset > Max_XMP_Int64)
		XMP_Throw("Offset out of Int64 bound", kXMPErr_EnforceFailure);

	fileRef->Seek(offset, kXMP_SeekFromStart);

	std::string nullChars(length,'\0');
	fileRef->Write(nullChars.c_str(), length);
}

// =================================================================================================
// HEIF_MetaHandler::CreateMdatBoxList
// ===================
void HEIF_MetaHandler::CreateMdatBoxList(XMP_IO* fileRef, XMP_Uns64 fileSize)
{
	XMP_Uns64 boxPos = 0, boxNext = 0;
	ISOMedia::BoxInfo currBox;

	fileRef->Rewind();
	mdatList.clear();

	for (boxPos = 0; boxPos < fileSize; boxPos = boxNext) {

		boxNext = ISOMedia::GetBoxInfo(fileRef, boxPos, fileSize, &currBox, true /* throw errors */);
		XMP_Uns64 currSize = currBox.headerSize + currBox.contentSize;
		if (currBox.boxType != ISOMedia::k_mdat)
			continue;

		mdatList.push_back(MdatInfo(boxPos, currSize, currBox.headerSize));
	}

}	//CreateMdatBoxList

// =================================================================================================
// HEIF_MetaHandler::SetMdatBoxFree
// ===================
bool HEIF_MetaHandler::SetMdatBoxFree(XMP_IO* fileRef, XMP_Uns32 dataSize, XMP_Uns64 curExtentOffset) {
	MdatInfo currMdatInfo;
	bool inMdat = this->GetParentMdatInfo(curExtentOffset, currMdatInfo);

	if (!inMdat)
		return false;

	//this case not greater than 2 GB size, beacuse of our own limit
	if (dataSize + currMdatInfo.headerSize == currMdatInfo.size) //able to free whole box
		this->metaMgr.WipeBoxFree(fileRef, currMdatInfo.offset, (XMP_Uns32)currMdatInfo.size);
	else {
		if (currMdatInfo.offset + currMdatInfo.size != curExtentOffset + dataSize)
			return false;

		//	the only time this would be executed is when our old unused data space is at the end of mdat box, 
		//	this means its size will be less than 2GB limit,so downcasting in XMP_Uns32 bits safe
		this->metaMgr.WipeBoxFree(fileRef, curExtentOffset, dataSize);
		//update header of parent mdat // reduce its size
		fileRef->Seek(currMdatInfo.offset, kXMP_SeekFromStart); //seek to mdat box start
		this->metaMgr.WriteBoxHeader(fileRef, ISOMedia::k_mdat, currMdatInfo.size - dataSize);
	}

	return true;
}

// =================================================================================================
// HEIF_MetaHandler::GetParentMdatInfo
// ===================
bool HEIF_MetaHandler::GetParentMdatInfo(XMP_Uns64 dataOffset, MdatInfo &mdatInfo) {
	for (auto minfo : mdatList) {
		if (dataOffset > minfo.offset && dataOffset < minfo.offset + minfo.size) {
			mdatInfo = minfo;
			return true;
		}
	}
	return false;
}

// =================================================================================================
// HEIF_MetaHandler::UpdateFile
// =============================
void HEIF_MetaHandler::UpdateFile(bool doSafeUpdate) {

	/*bool optimizeFileLayout = false;
	if (this->parent)
	{
		optimizeFileLayout = XMP_OptionIsSet(this->parent->openFlags, kXMPFiles_OptimizeFileLayout);
	}
*/
	if (!this->needsUpdate) {	// If needsUpdate is set then at least the XMP changed.
		//if (optimizeFileLayout) this->OptimizeFileLayout();
		return;
	}

	XMP_Assert(!doSafeUpdate);	// This should only be called for "unsafe" updates.

	this->needsUpdate = false;	// Make sure only called once.

	XMP_Int32 oldPacketLength = this->packetInfo.length;
	XMP_Int64 oldPacketOffset = this->packetInfo.offset;

	if (oldPacketOffset == kXMPFiles_UnknownOffset) oldPacketOffset = 0;
	if (oldPacketLength == kXMPFiles_UnknownLength) oldPacketLength = 0;

	bool fileHadXMP = (oldPacketOffset != 0) && ((oldPacketLength != 0));
	// Update native TIFF/Exif metadata. ExportPhotoData also trips the tiff: and
	// exif: copies from the XMP, so reserialize the now final XMP packet.

	ExportPhotoData(kXMP_JPEGFile, &this->xmpObj, exifMgr, 0, 0);
	try {
		XMP_OptionBits options = kXMP_UseCompactFormat;
		if (fileHadXMP) options |= kXMP_ExactPacketLength;
		this->xmpObj.SerializeToBuffer(&this->xmpPacket, options, oldPacketLength);
	}
	catch (...) {
		this->xmpObj.SerializeToBuffer(&this->xmpPacket, kXMP_UseCompactFormat);
	}

	bool doInPlace = (fileHadXMP && (this->xmpPacket.size() == (size_t)oldPacketLength));
	if ((this->exifMgr != 0) && (this->exifMgr->IsLegacyChanged())) doInPlace = false;

	if (doInPlace) {
		//Only update XMP!!!
		ModifyItemData(xmpItemID, (XMP_Uns32) this->xmpPacket.size(), (XMP_Uns8 *)this->xmpPacket.c_str());
		return;
	}

	// !doInPlace
	XMP_IO* origRef = this->parent->ioRef;
	XMP_IO* tempRef = origRef->DeriveTemp();
	
	UpdateTempFile(tempRef, fileHadXMP);

	origRef->AbsorbTemp();
}

// =================================================================================================
// HEIF_MetaHandler::WriteTempFile
// =============================
void HEIF_MetaHandler::WriteTempFile(XMP_IO* tempRef)
{
	XMP_Assert(this->needsUpdate);

	this->needsUpdate = false;	// Make sure only called once.

	XMP_Int32 oldPacketLength = this->packetInfo.length;
	XMP_Int64 oldPacketOffset = this->packetInfo.offset;

	if (oldPacketOffset == kXMPFiles_UnknownOffset) oldPacketOffset = 0;
	if (oldPacketLength == kXMPFiles_UnknownLength) oldPacketLength = 0;

	bool fileHadXMP = (oldPacketOffset != 0) && ((oldPacketLength != 0));

	ExportPhotoData(kXMP_JPEGFile, &this->xmpObj, exifMgr, 0, 0);
	try {
		XMP_OptionBits options = kXMP_UseCompactFormat;
		if (fileHadXMP) options |= kXMP_ExactPacketLength;
		this->xmpObj.SerializeToBuffer(&this->xmpPacket, options, oldPacketLength);
	}
	catch (...) {
		this->xmpObj.SerializeToBuffer(&this->xmpPacket, kXMP_UseCompactFormat);
	}

	UpdateTempFile(tempRef, fileHadXMP);


}	// HEIF_MetaHandler::WriteTempFile

void HEIF_MetaHandler::UpdateTempFile(XMP_IO* tempRef, bool fileHadXMP) {
	XMP_IO* originalRef = this->parent->ioRef;

	tempRef->Rewind();
	originalRef->Rewind();

	XIO::Copy(originalRef, tempRef, originalRef->Length(),
		this->parent->abortProc, this->parent->abortArg);

	try {
		this->parent->ioRef = tempRef;	
		this->UpdateOperation(fileHadXMP);
		this->parent->ioRef = originalRef;
	}
	catch (...) {
		this->parent->ioRef = originalRef;
		throw;
	}
}

void HEIF_MetaHandler::UpdateOperation(bool fileHadXMP) {

	XMP_IO* fileRef = this->parent->ioRef;

	XMP_Uns32 length = (XMP_Uns32)this->xmpPacket.length();
	bool performUpdate = false;
	CreateMdatBoxList(fileRef, fileRef->Length());
	

	if (!fileHadXMP)
	{
		bool resourceAdded = AddNewResourceEntries(length, ISOMedia::k_mime, xmpItemID);
		if (resourceAdded)
			this->metaMgr.AddReferenceToPrimaryItem(primaryItemID, xmpItemID);
		performUpdate |= resourceAdded;
	}
	else {
		performUpdate = ModifyItemData(xmpItemID, (XMP_Uns32) this->xmpPacket.size(), (XMP_Uns8 *)this->xmpPacket.c_str()); //Update if we are able to modify
																															//performUpdate = true;
	}
	XMP_Uns32 exifLen = 0;
	void* exifPtr = NULL;
	if (exifMgr != 0)
		exifLen = exifMgr->UpdateMemoryStream(&exifPtr);

	if (exifLen > 0) {
		if (this->exifData.size() == 0) {

			static const char * kExifSignatureString = "Exif\0\x00"; //Length = 6
			bool resourceAdded = AddNewResourceEntries(exifLen + 6 + 4, ISOMedia::k_Exif, exifItemID);

			if (resourceAdded) {
				this->metaMgr.AddReferenceToPrimaryItem(primaryItemID, exifItemID);
				this->exifData.assign(exifLen + 6 + 4, 0);
				PutUns32BE(6, this->exifData.data());
				memcpy(this->exifData.data() + 4, kExifSignatureString, 6);
				memcpy(this->exifData.data() + 10, exifPtr, exifLen);

			}
			performUpdate |= resourceAdded;
		}
		else {
			const XMP_Uns8 * exifOrigin = this->exifData.data();
			if (this->exifData.size() > 4) {
				XMP_Uns32 exifHeaderOffset = GetUns32BE(exifOrigin);
				size_t headerSize = exifHeaderOffset + 4;
				RawDataBlock newData;
				newData.assign(exifLen + headerSize, 0);
				memcpy(newData.data(), exifOrigin, headerSize);
				memcpy(newData.data() + headerSize, exifPtr, exifLen);
				this->exifData.swap(newData);
				performUpdate |= ModifyItemData(exifItemID, (XMP_Uns32)(exifLen + headerSize), this->exifData.data());

				//performUpdate = true;
			}

		}
	}


	if (performUpdate) {
		this->metaMgr.UpdateIlocBoxContent();
		if (this->metaMgr.IsChanged()) {
			this->metaMgr.UpdateMemoryTree();
			UpdateTopLevelBox(metaBoxPos, metaBoxSize, &this->metaMgr.fullSubtree[0],
				(XMP_Uns32)this->metaMgr.fullSubtree.size());
		}
		AddFileLevelMdatBox();
	}
}

// =================================================================================================
// HEIF_MetaHandler::AddNewResourceEntries
// =============================
bool HEIF_MetaHandler::AddNewResourceEntries(XMP_Uns32 length, XMP_Uns32 itemType, XMP_Uns32& itemID) {
	
	std::string contentType;
	contentType = (itemType == ISOMedia::k_mime) ? "application/rdf+xml" : "";  // Only handling mime type item XMP
	
	//Check if an itemInfoEntry item does not already exists. And corresponding iloc item is not corrupted
	XMP_Uns32 checkItemID;
	if (CheckItemExistsForItemType(itemType, checkItemID, contentType)) {
		Meta_Manager::ilocItem ilocitem;
		
		//We have reached this point, so there'll be some issue in reading from earlier entry. 
		//That's why remove it's refrence regardless if it's valid iloc item or not. 
		this->metaMgr.RemoveReferenceToPrimaryItem(primaryItemID, checkItemID); 

	}
	this->metaMgr.AddIlocItem(itemID, length,extentIDMap);
	Meta_Manager::itemInfoEntry iItem;
	iItem.itemProtectionIndex = 0;
	iItem.contenEncoding = "";
	iItem.itemName = "";
	iItem.itemType = itemType;
	iItem.contentType = contentType;

	bool infeItemAdded = this->metaMgr.AddItemInfoEntry(itemID, iItem);

	if (!infeItemAdded)
		return false;
	
	return true;
}

// =================================================================================================
// HEIF_MetaHandler::UpdateXMPOffset
// =============================
void HEIF_MetaHandler::UpdateXMPOffset(XMP_Uns32 xmpoffset) {
	ISOBaseMedia_Manager::BoxInfo ilocInfo;
	ISOBaseMedia_Manager::BoxRef  ilocRef = this->metaMgr.GetTypeChild(&this->metaMgr.subtreeRootNode, ISOMedia::k_iloc, &ilocInfo);
	PutUns32BE(xmpoffset + 8, (void **)(ilocInfo.content + ilocInfo.contentSize - 8));
	
	ISOBaseMedia_Manager::BoxNode * ilocNode = (ISOBaseMedia_Manager::BoxNode*)ilocRef;
	ilocNode->changedContent.assign(ilocInfo.contentSize, 0);	// Fill with 0's first to get the storage.
	
	memcpy(&this->metaMgr.fullSubtree[0] + ilocNode->offset + ilocNode->headerSize, ilocInfo.content, ilocInfo.contentSize);
	
	UpdateTopLevelBox(metaBoxPos, metaBoxSize, &this->metaMgr.fullSubtree[0],
		(XMP_Uns32)this->metaMgr.fullSubtree.size());
} 

static XMP_Uns8 kZeroes[64 * 1024];
//different from Mpeg handler, so don't combine
void HEIF_MetaHandler::UpdateTopLevelBox(XMP_Uns64 oldOffset, XMP_Uns32 oldSize,
	const XMP_Uns8 * newBox, XMP_Uns32 newSize)
{
	XMP_Int64 newoffset;
	if ((oldSize == 0) && (newSize == 0)) return;	// Sanity check, should not happen.

	XMP_IO* fileRef = this->parent->ioRef;
	XMP_Uns64 oldFileSize = fileRef->Length();

	XMP_AbortProc abortProc = this->parent->abortProc;
	void *        abortArg = this->parent->abortArg;

	if (newSize == oldSize) {

		// Trivial case, update the existing box in-place.
		fileRef->Seek(oldOffset, kXMP_SeekFromStart);
		newoffset = fileRef->Offset();
		fileRef->Write(newBox, oldSize);

	}
	else if ((oldOffset + oldSize) == oldFileSize) {

		// The old box was at the end, write the new and truncate the file if necessary.
		fileRef->Seek(oldOffset, kXMP_SeekFromStart);
		fileRef->Write(newBox, newSize);
		newoffset = fileRef->Offset();
		fileRef->Truncate((oldOffset + newSize));	// Does nothing if new size is bigger.

	}
	else if ((newSize < oldSize) && ((oldSize - newSize) >= 8)) {

		// The new size is smaller and there is enough room to create a free box.
		fileRef->Seek(oldOffset, kXMP_SeekFromStart);
		newoffset = fileRef->Offset();
		fileRef->Write(newBox, newSize);
		this->metaMgr.WipeBoxFree(fileRef, (oldOffset + newSize), (oldSize - newSize));

	}
	else {

		// Look for a trailing free box with enough space. If not found, consider any free space.
		// If still not found, append the new box and make the old one free.

		ISOMedia::BoxInfo nextBoxInfo;
		(void)ISOMedia::GetBoxInfo(fileRef, (oldOffset + oldSize), oldFileSize, &nextBoxInfo, true /* throw errors */);

		XMP_Uns64 totalRoom = oldSize + nextBoxInfo.headerSize + nextBoxInfo.contentSize;

		bool nextIsFree = (nextBoxInfo.boxType == ISOMedia::k_free) || (nextBoxInfo.boxType == ISOMedia::k_skip);
		bool haveEnoughRoom = (newSize == totalRoom) ||
			((newSize < totalRoom) && ((totalRoom - newSize) >= 8));

		if (nextIsFree & haveEnoughRoom) {

			fileRef->Seek(oldOffset, kXMP_SeekFromStart);
			newoffset = fileRef->Offset();
			fileRef->Write(newBox, newSize);

			if (newSize < totalRoom) {
				// Don't wipe, at most 7 old bytes left, it will be covered by the free header.
				this->metaMgr.WriteBoxHeader(fileRef, ISOMedia::k_free, (totalRoom - newSize));
			}

		}
		else {

			// Create a list of all top level free space, including the old space as free. Use the
			// earliest space that fits. If none, append.

			Meta_Manager::SpaceList spaceList;
			this->metaMgr.CreateFreeSpaceList(fileRef, oldFileSize, oldOffset, oldSize, &spaceList);

			size_t freeSlot, limit;
			for (freeSlot = 0, limit = spaceList.size(); freeSlot < limit; ++freeSlot) {
				XMP_Uns64 freeSize = spaceList[freeSlot].size;
				if ((newSize == freeSize) || ((newSize < freeSize) && ((freeSize - newSize) >= 8))) break;
			}

			if (freeSlot == spaceList.size()) {

				// No available free space, append the new box.
				CheckFinalBox(fileRef, abortProc, abortArg);
				fileRef->ToEOF();
				newoffset = fileRef->Offset();
				fileRef->Write(newBox, newSize);
				this->metaMgr.WipeBoxFree(fileRef, oldOffset, oldSize);

			}
			else {

				// Use the available free space. Wipe non-overlapping parts of the old box. The old
				// box is either included in the new space, or is fully disjoint.

				Meta_Manager::SpaceInfo & newSpace = spaceList[freeSlot];

				bool oldIsDisjoint = ((oldOffset + oldSize) <= newSpace.offset) ||		// Old is in front.
					((newSpace.offset + newSpace.size) <= oldOffset);	// Old is behind.

				XMP_Assert((newSize == newSpace.size) ||
					((newSize < newSpace.size) && ((newSpace.size - newSize) >= 8)));

				XMP_Assert(oldIsDisjoint ||
					((newSpace.offset <= oldOffset) &&
					((oldOffset + oldSize) <= (newSpace.offset + newSpace.size))) /* old is included */);

				XMP_Uns64 newFreeOffset = newSpace.offset + newSize;
				XMP_Uns64 newFreeSize = newSpace.size - newSize;

				fileRef->Seek(newSpace.offset, kXMP_SeekFromStart);
				newoffset = fileRef->Offset();
				fileRef->Write(newBox, newSize);

				if (newFreeSize > 0) this->metaMgr.WriteBoxHeader(fileRef, ISOMedia::k_free, newFreeSize);

				if (oldIsDisjoint) {

					this->metaMgr.WipeBoxFree(fileRef, oldOffset, oldSize);

				}
				else {

					// Clear the exposed portion of the old box.

					XMP_Uns64 zeroStart = newFreeOffset + 8;
					if (newFreeSize > 0xFFFFFFFF) zeroStart += 8;
					if (oldOffset > zeroStart) zeroStart = oldOffset;
					XMP_Uns64 zeroEnd = newFreeOffset + newFreeSize;
					if ((oldOffset + oldSize) < zeroEnd) zeroEnd = oldOffset + oldSize;

					if (zeroStart < zeroEnd) {	// The new box might cover the old.
						XMP_Assert((zeroEnd - zeroStart) <= (XMP_Uns64)oldSize);
						XMP_Uns32 zeroSize = (XMP_Uns32)(zeroEnd - zeroStart);
						fileRef->Seek(zeroStart, kXMP_SeekFromStart);
						for (XMP_Uns32 ioCount = sizeof(kZeroes); zeroSize > 0; zeroSize -= ioCount) {
							if (ioCount > zeroSize) ioCount = zeroSize;
							fileRef->Write(&kZeroes[0], ioCount);
						}
					}

				}

			}

		}

	}
	metaBoxPos = newoffset;
	metaBoxSize = newSize;

}	// MPEG4_MetaHandler::UpdateTopLevelBox

XMP_Int64 HEIF_MetaHandler::AddTopLevelBox(const XMP_Uns8 * box, XMP_Uns32 size) {
	if (size == 0) return 0;	// Sanity check, should not happen.
	XMP_Int64 offset; 
	XMP_IO* fileRef = this->parent->ioRef;
	XMP_Uns64 oldFileSize = fileRef->Length();

	XMP_AbortProc abortProc = this->parent->abortProc;
	void *        abortArg = this->parent->abortArg;

	Meta_Manager::SpaceList spaceList;
	this->metaMgr.CreateFreeSpaceList(fileRef, oldFileSize, 0, 0, &spaceList);

	size_t freeSlot, limit;
	for (freeSlot = 0, limit = spaceList.size(); freeSlot < limit; ++freeSlot) {
		XMP_Uns64 freeSize = spaceList[freeSlot].size;
		if ((size == freeSize) || ((size < freeSize) && ((freeSize - size) >= 8))) break;
	}

	if (freeSlot == spaceList.size()) {

		// No available free space, append the new box.
		CheckFinalBox(fileRef, abortProc, abortArg);
		fileRef->ToEOF();
		offset = fileRef->Offset();
		CheckInt64LimitThrow(offset);
		fileRef->Write(box, size);
		//WipeBoxFree(fileRef, oldOffset, oldSize);

	}
	else {
		// Use the available free space. Wipe non-overlapping parts of the old box. The old
		// box is either included in the new space, or is fully disjoint.

		Meta_Manager::SpaceInfo & newSpace = spaceList[freeSlot];

		XMP_Assert((size == newSpace.size) ||
			((size < newSpace.size) && ((newSpace.size - size) >= 8)));

		XMP_Uns64 newFreeOffset = newSpace.offset + size;
		XMP_Uns64 newFreeSize = newSpace.size - size;

		CheckInt64LimitThrow(newFreeOffset);
		
		fileRef->Seek(newSpace.offset, kXMP_SeekFromStart);
		offset = fileRef->Offset();
		fileRef->Write(box, size);

		if (newFreeSize > 0) this->metaMgr.WriteBoxHeader(fileRef, ISOMedia::k_free, newFreeSize);
	
	}
	return offset;
}

// =================================================================================================
// HEIF_MetaHandler::ModifyItemData
// =============================
bool HEIF_MetaHandler::ModifyItemData(XMP_Uns32 itemID, XMP_Uns32 size, XMP_Uns8 * packet) {
	// we don't need to check protection & data info
	// as we would have done it during ProcessXMP() 
	
	Meta_Manager::ilocItem ilocItem;
	std::map<XMP_Uns32, Meta_Manager::ilocItem>::iterator itr = this->metaMgr.itemLocationMap.find(itemID);
	if (itr != this->metaMgr.itemLocationMap.end())
		ilocItem = itr->second;
	else
		return 0;
	
	size_t constructionMethod = ilocItem.constructionMethod;

	switch (constructionMethod) {
		case 0:	ModifyDataByConstructionMethodZero(ilocItem, itemID, packet, size); break;
		case 1:
		case 2:	XMP_Error error(kXMPErr_HEIFConstructionMethodNotSupported, "Unsupported Construction method found for the data");
				XMPFileHandler::NotifyClient(&parent->errorCallback, kXMPErrSev_FileFatal, error);
				
	}

	itr->second = ilocItem;
	return true;
}

// =================================================================================================
// HEIF_MetaHandler::ModifyDataByConstructionMethodZero
// =============================
void HEIF_MetaHandler::ModifyDataByConstructionMethodZero(Meta_Manager::ilocItem &ilocItem, XMP_Uns32 itemID,XMP_Uns8 * packet, XMP_Uns32 size) {

	XMP_IO* fileRef = this->parent->ioRef;
	size_t index = 0;
	size_t numExtents = ilocItem.iExtents.size();
	XMP_Int64 fileLength = fileRef->Length();
	XMP_Uns32 totalExtentSize = 0;

	for (size_t ext = 0; ext < numExtents; ext++) {
		if (ilocItem.iExtents[ext].extent_length == 0)
			ilocItem.iExtents[ext].extent_length = fileLength;
		totalExtentSize += (XMP_Uns32)ilocItem.iExtents[ext].extent_length;
	}

	if (totalExtentSize == size) {
		for (size_t ext = 0; ext < numExtents; ext++) {
			//Since Seek Api has parameteer XMP_Int64
			CheckInt64LimitThrow(ilocItem.iExtents[ext].extent_offset);

			fileRef->Seek(ilocItem.iExtents[ext].extent_offset, kXMP_SeekFromStart);
			fileRef->Write(packet + index, (XMP_Uns32)ilocItem.iExtents[ext].extent_length);
			//Not allowing sizes which are greater than 2^32 (So can be casted to XMP_Uns32)
			index += (XMP_Uns32)ilocItem.iExtents[ext].extent_length;
		}
	}
	else if (totalExtentSize > size) { // So we can fit the data in current location
		XMP_Uns32 remainingSize = size;
		size_t numExtentsRemoved = 0, ext = 0;
		for (ext = 0; ext < numExtents; ext++) {
			//Not allowing sizes which are greater than 2^32 (So can be casted to XMP_Uns32)
			XMP_Uns64 curExtentOffset = ilocItem.iExtents[ext].extent_offset;
			XMP_Uns32 curExtentLength = (XMP_Uns32)ilocItem.iExtents[ext].extent_length;

			CheckInt64LimitThrow(curExtentOffset);

			fileRef->Seek(curExtentOffset, kXMP_SeekFromStart);
			// Case if (numExtents == 1) is also covered
			if (!(remainingSize > 0)) {
				break;
			}

			// remainingSize > 0
			if (curExtentLength <= remainingSize) {
				fileRef->Write(packet + index, curExtentLength);
				remainingSize -= curExtentLength;
				index += curExtentLength;
				continue;
			}
			// Following code for condition curExtentLength > remainingSize
			MdatInfo mdatInfo;
			bool inMdat = this->GetParentMdatInfo(curExtentOffset + remainingSize, mdatInfo);
			if (!inMdat) {
				setBytesNull(fileRef, curExtentOffset + remainingSize, curExtentLength - remainingSize);
				// add new extent for this data
				Meta_Manager::extent newExt;
				newExt.extent_length = size;
				newExt.extent_offset = 0;
				newExt.relativeOffsetInData = index;
				extentIDMap[itemID] = std::make_pair(ext, newExt); //keeping no. of extent = 1
				ilocItem.iExtents[ext] = newExt;
				ilocItem.changed = true;

				continue;
			}
			//data is in some mdat box
			if (curExtentLength - remainingSize < 8) {
				//Not reducing mdat size in this case
				setBytesNull(fileRef, curExtentOffset + remainingSize, curExtentLength - remainingSize);
				ilocItem.iExtents[ext].extent_length -= remainingSize;
				remainingSize -= remainingSize;
				index += remainingSize;

				continue;
			}
			//Below case curExtentLength - remainingSize >= 8
			if (mdatInfo.size > 0) {
				fileRef->Write(packet + index, remainingSize);
				bool success = this->SetMdatBoxFree(fileRef, (XMP_Uns32)curExtentLength - remainingSize, curExtentOffset + remainingSize);
				if (!success)	// data is not at the end of mdat box
					setBytesNull(fileRef, curExtentOffset + remainingSize, (XMP_Uns32)curExtentLength - remainingSize);
			}
			else
				setBytesNull(fileRef, curExtentOffset + remainingSize, (XMP_Uns32)curExtentLength - remainingSize);

			ilocItem.iExtents[ext].extent_length -= remainingSize;
			remainingSize -= remainingSize;
			index += remainingSize;
			XMP_Assert(remainingSize == 0);

			ilocItem.changed = true;
			// in caseof less we need to can free all other remainig boxes and just create 1 other box
			// if one box, not fit than free and other place write. 

		}

		if (!(remainingSize <= 0 && ext < numExtents))
			return;

		//If all new data is updated and old data space is left, so try to free it
		size_t remExt = ext;
		//free other extents left
		while (ext < numExtents) {
			XMP_Uns64 curExtentOffset = ilocItem.iExtents[ext].extent_offset;
			XMP_Uns32 curExtentLength = (XMP_Uns32)ilocItem.iExtents[ext].extent_length;

			MdatInfo mdatInfo;

			if (mdatInfo.size > 0) {
				bool success = this->SetMdatBoxFree(fileRef, curExtentLength, curExtentOffset);
				if (!success)	// data is not at the end of mdat box
					setBytesNull(fileRef, curExtentOffset, curExtentLength); //curExtentLength not cross 32bit size boundary sice we only dealing with size limit till 2GB
			}
			else
				setBytesNull(fileRef, curExtentOffset + remainingSize, curExtentLength - remainingSize);

			ext++;
		}
		ilocItem.iExtents.erase(ilocItem.iExtents.begin() + remExt, ilocItem.iExtents.end());

	}
	else { //totalExtentSize < size
		if (numExtents == 1) {

			bool success = SetMdatBoxFree(fileRef, (XMP_Uns32)ilocItem.iExtents[0].extent_length, ilocItem.iExtents[0].extent_offset);
			if (!success)
				setBytesNull(fileRef, ilocItem.iExtents[0].extent_offset, (XMP_Uns32)ilocItem.iExtents[0].extent_length);

			//Add new mdat box
			Meta_Manager::extent newExt;
			newExt.extent_length = size;
			newExt.extent_offset = 0;
			newExt.relativeOffsetInData = 0;
			extentIDMap[itemID] = std::make_pair(0, newExt); //keeping no. of extent = 1
			ilocItem.iExtents[0] = newExt;
			ilocItem.changed = true;

		}
		else {
			//Nothing to be free here, as we'll add remaining data in one extent
			for (size_t ext = 0; ext < numExtents; ext++) {

				CheckInt64LimitThrow(ilocItem.iExtents[ext].extent_offset);

				fileRef->Seek(ilocItem.iExtents[ext].extent_offset, kXMP_SeekFromStart);
				fileRef->Write(packet + index, (XMP_Uns32)ilocItem.iExtents[ext].extent_length);

				//Not allowing sizes which are greater than 2^32 (So can be casted to XMP_Uns32)
				index += (XMP_Uns32)ilocItem.iExtents[ext].extent_length;
			}

			Meta_Manager::extent newExt;
			newExt.extent_length = size - totalExtentSize;
			newExt.extent_offset = 0;
			newExt.relativeOffsetInData = totalExtentSize; //this is where extent would start, recheck!!!???
			ilocItem.iExtents.push_back(newExt);
			extentIDMap[itemID] = std::make_pair(numExtents, newExt);
			ilocItem.changed = true;
		}
	}
	
}

// =================================================================================================
// HEIF_MetaHandler::AddFileLevelMdatBox
// =============================
void HEIF_MetaHandler::AddFileLevelMdatBox() {

	ISOBaseMedia_Manager::BoxInfo ilocInfo;
	ISOBaseMedia_Manager::BoxRef  ilocRef = this->metaMgr.GetTypeChild(&this->metaMgr.subtreeRootNode, ISOMedia::k_iloc, &ilocInfo);
	ISOBaseMedia_Manager::BoxNode * ilocNode = (ISOBaseMedia_Manager::BoxNode*)ilocRef;
	ilocNode->changedContent.assign(ilocInfo.contentSize, 0);	// Fill with 0's first to get the storage.
	for (auto const& itr : extentIDMap) {
		auto extPair = itr.second;
		size_t extentIndex = extPair.first;
		Meta_Manager::extent ext = extPair.second;
		if (itr.first == xmpItemID) {
			XMP_Uns32 contentSize = (XMP_Uns32)ext.extent_length + 8;// (XMP_Uns32) this->xmpPacket.length() + 8;
			XMP_Uns8 * packet = new XMP_Uns8[contentSize];
			PutUns32BE(contentSize, packet);
			PutUns32BE(ISOMedia::k_mdat, packet + 4);
			memcpy(packet + 8, (this->xmpPacket.c_str()+ext.relativeOffsetInData), contentSize - 8);
			XMP_Int64 offset = AddTopLevelBox(packet, contentSize) + 8;
			delete[] packet;
			if (offset <= 0) {
				XMP_Error error(kXMPErr_WriteError, "Data offset out of 32 bit bounds");
				XMPFileHandler::NotifyClient(&parent->errorCallback, kXMPErrSev_FileFatal, error);
			}

			UpdateItemExtentOffset(offset, xmpItemID,extentIndex);
			
		}
		else if(itr.first == exifItemID){
			XMP_Uns32 contentSize = (XMP_Uns32)ext.extent_length + 8;
			XMP_Uns8 * packet = new XMP_Uns8[contentSize];
			PutUns32BE(contentSize, packet);
			PutUns32BE(ISOMedia::k_mdat, packet + 4);
			memcpy(packet + 8, (this->exifData.data() + ext.relativeOffsetInData), contentSize - 8);
			XMP_Int64 offset = AddTopLevelBox(packet, contentSize) + 8; // what if it's 64???? 
			delete[] packet;
			if (offset <= 0) {
				XMP_Error error(kXMPErr_WriteError, "Data offset out of 32 bit bounds");
				XMPFileHandler::NotifyClient(&parent->errorCallback, kXMPErrSev_FileFatal, error);
			}
			UpdateItemExtentOffset(offset, exifItemID, extentIndex);

		}
	}

	memcpy(&this->metaMgr.fullSubtree[0] + ilocNode->offset + ilocNode->headerSize, ilocInfo.content, ilocInfo.contentSize);

	UpdateTopLevelBox(metaBoxPos, metaBoxSize, &this->metaMgr.fullSubtree[0],
		(XMP_Uns32)this->metaMgr.fullSubtree.size());

}

// =================================================================================================
// HEIF_MetaHandler::UpdateItemExtentOffset
// =============================
void HEIF_MetaHandler::UpdateItemExtentOffset(XMP_Int64 offset,XMP_Uns32 itemID, size_t extentIndex) {

	ISOBaseMedia_Manager::BoxInfo ilocInfo;
	ISOBaseMedia_Manager::BoxRef  ilocRef = this->metaMgr.GetTypeChild(&this->metaMgr.subtreeRootNode, ISOMedia::k_iloc, &ilocInfo);
	XMP_Uns8 ilocVersion = ilocInfo.content[0]; //valid version already checked during Import
	XMP_Uns8 const * ptr = ilocInfo.content + 6;
	if(ilocVersion < 2) 
		ptr += 2;
	else
		ptr += 4;
	
	for (auto const& itr : this->metaMgr.itemLocationMap){
		Meta_Manager::ilocItem currItem = itr.second;
		if (itr.first != itemID) {
			ptr += currItem.sizeOfItem;
			continue;
		}

		if (ilocVersion < 2) ptr += 2;
		else if (ilocVersion == 2) ptr += 4;
		if (ilocVersion == 1 || ilocVersion == 2) ptr += 2;
		//base_offset_size
		ptr += (this->metaMgr.ilocByteSizesStruct.baseOffsetSize / 8);
		ptr += 2;//dataReferenceIndex
		ptr += 2; //num extents
		for (size_t i = 0; i < currItem.iExtents.size(); i++) {

			if (i == extentIndex) {
				if ((ilocVersion == 1 || ilocVersion == 2)) {

					ptr += (this->metaMgr.ilocByteSizesStruct.indexSize / 8);
				}
				
				if (ilocVersion < 2) {
					//Downcasting so need to verify if the offset is in 32 bit range
					if(offset <= Max_XMP_Uns32)
						PutUns32BE((XMP_Uns32)offset, (void **)(ptr)); 
					else {
						XMP_Error error(kXMPErr_WriteError, "Data offset out of 32 bit bounds");
						XMPFileHandler::NotifyClient(&parent->errorCallback, kXMPErrSev_FileFatal, error);
					}
				}
				else {
					PutUns64BE(offset, (void **)(ptr));
				}
				break;
			}
			else
				ptr += currItem.iExtents[i].extent_offset;
		}

	}
}

// =================================================================================================

#endif	// AdobePrivate	// ! This entire handler is private, it must not go in the public SDK.
