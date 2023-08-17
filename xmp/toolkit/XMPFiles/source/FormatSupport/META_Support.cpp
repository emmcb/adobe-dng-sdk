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
#include "XMPFiles/source/FormatSupport/META_Support.hpp"

#include "XMPFiles/source/FormatSupport/ISOBaseMedia_Support.hpp"
#include<algorithm>

// =================================================================================================
// Meta_Manager::ParseMetaMemoryTree
// ==============================
void Meta_Manager::ParseMetaMemoryTree() {
	//this->fileMode = _fileMode;
	this->subtreeRootNode.offset = this->subtreeRootNode.boxType = 0;
	this->subtreeRootNode.headerSize = this->subtreeRootNode.contentSize = 0;
	this->subtreeRootNode.children.clear();
	this->subtreeRootNode.changedContent.clear();
	this->subtreeRootNode.changed = false;

	if (this->fullSubtree.empty()) return;

	ISOMedia::BoxInfo metaInfo;
	const XMP_Uns8 * metaOrigin = &this->fullSubtree[0];
	const XMP_Uns8 * metaLimit = metaOrigin + this->fullSubtree.size();

	(void)ISOMedia::GetBoxInfo(metaOrigin, metaLimit, &metaInfo);
	XMP_Enforce(metaInfo.boxType == ISOMedia::k_meta);

	XMP_Uns64 fullMetaSize = metaInfo.headerSize + metaInfo.contentSize;

	if (fullMetaSize > TopBoxSizeLimit) {	// From here on we know 32-bit offsets are safe.  
		XMP_Throw("Oversize 'meta' box", kXMPErr_EnforceFailure);
	}

	this->subtreeRootNode.boxType = ISOMedia::k_meta;
	this->subtreeRootNode.headerSize = metaInfo.headerSize;
	this->subtreeRootNode.contentSize = (XMP_Uns32)metaInfo.contentSize;

	///this->subtreeRootNode = BoxNode(0, metaInfo.boxType, metaInfo.headerSize, (XMP_Uns32)metaInfo.contentSize);
	ParseNestedMetaBoxes(& (this->subtreeRootNode), "meta");
	
}

// =================================================================================================
// Meta_Manager::ParseNestedMetaBoxes
// ==============================
void Meta_Manager::ParseNestedMetaBoxes(BoxNode * parentNode, const std::string & parentPath) {


	const XMP_Uns8 * metaOrigin = &this->fullSubtree[0];
	ISOMedia::BoxInfo isoInfo;
	BoxInfo mmgrInfo; 
	
	const XMP_Uns8 * childOrigin = metaOrigin + parentNode->offset + parentNode->headerSize;
	const XMP_Uns8 * childLimit = childOrigin + parentNode->contentSize;

	const XMP_Uns8 * nextChild;

	parentNode->contentSize = 0;

	//meta box, iinf, dref is a full box
	if (parentPath == "meta" || parentPath == "iref"){
		childOrigin += 4; parentNode->contentSize = 4;
	}
	else if (parentPath == "dref"){
		childOrigin += 8; // Adding size(4 bytes) of entry count
		parentNode->contentSize = 8;
	}
	else if (parentPath == "iinf") {
		if(childOrigin[0] == 0){	
			childOrigin += 6; // 2 bytes in iinf box are for entry count 
			parentNode->contentSize = 6;
		}
		else {
			childOrigin += 8; // 4 bytes in iinf box are for entry count
			parentNode->contentSize = 8;
		}
			 
	}

	for (const XMP_Uns8 * currChild = childOrigin; currChild < childLimit; currChild = nextChild) {
		nextChild = ISOMedia::GetBoxInfo(currChild, childLimit, &isoInfo);
		if ((isoInfo.headerSize < 8) && 
			(isoInfo.contentSize == 0)) continue;
		
		XMP_Uns32 childOffset = (XMP_Uns32)(currChild - metaOrigin);

		//currently no uuid box is used in HEIF files
		if (isoInfo.boxType == ISOMedia::k_uuid)
			parentNode->children.push_back(BoxNode(childOffset, isoInfo.boxType, isoInfo.headerSize, (XMP_Uns8 *)isoInfo.idUUID, (XMP_Uns32)isoInfo.contentSize));
		else
			parentNode->children.push_back(BoxNode(childOffset, isoInfo.boxType, isoInfo.headerSize, (XMP_Uns32)isoInfo.contentSize));
		BoxNode * newChild = &parentNode->children.back();
		
		switch (isoInfo.boxType) {

			case ISOMedia::k_iinf: this->ParseNestedMetaBoxes(newChild, "iinf"); break;
			case ISOMedia::k_dinf: this->ParseNestedMetaBoxes(newChild, "dinf"); break;
			case ISOMedia::k_dref: this->ParseNestedMetaBoxes(newChild, "dref"); break;
			case ISOMedia::k_iref: this->ParseNestedMetaBoxes(newChild, "iref"); break;
		}
		
		
	}
}

#if AdobePrivate
#define CheckPointerLimitReturnFalse(ptr,count,limit)	{ if(ptr + count > limit) return false;  }

// =================================================================================================
// Meta_Manager::ImportItemInformation
// ==============================
bool Meta_Manager::ImportItemInformation() {

	ISOBaseMedia_Manager::BoxInfo iinfInfo;
	ISOBaseMedia_Manager::BoxRef  iinfRef = this->GetTypeChild(&subtreeRootNode, ISOMedia::k_iinf, &iinfInfo);

	itemInfoEntryList.clear();

	if (iinfInfo.content == nullptr || iinfInfo.contentSize <= 4)
		return false;

	XMP_Uns8 iinfVersion = iinfInfo.content[0];
	XMP_Uns32 nItems = 0;
	if (iinfVersion == 0)
		nItems = (XMP_Uns32)GetUns16BE(iinfInfo.content + 4);
	else if (iinfVersion == 1)
		nItems = GetUns32BE(iinfInfo.content + 4);
	else
		return false;

	
	ISOBaseMedia_Manager::BoxNode * iinfNode = (ISOBaseMedia_Manager::BoxNode*)iinfRef;

	//If nItems > number of children of iinfNode, do not process
	if (nItems > (iinfNode->children).size()) return false;
	
	itemInfoEntryList.reserve(nItems);

	for (XMP_Int64 i = 0; i < nItems; i++) {
		ISOBaseMedia_Manager::BoxInfo infeInfo;
		ISOBaseMedia_Manager::BoxRef  infeRef = GetNthChild(iinfRef, i, &infeInfo);
		const XMP_Uns8 * limitPtr = infeInfo.content + infeInfo.contentSize;
		if (!(infeInfo.content != nullptr && infeInfo.boxType == ISOMedia::k_infe && infeInfo.contentSize > 4)) {	// An extra check for type of box
																													//iinf is a full box
			itemInfoEntryList.clear();
			//ItemInfoEntryList.resize(0);
			break;
		}

		XMP_Uns8 version = infeInfo.content[0];
		if (version != 2 && version != 3)
			continue;

		const XMP_Uns8 *ptr = infeInfo.content + 4;
		itemInfoEntry item;
		item.version = version;
		if (version == 2) {
			CheckPointerLimitReturnFalse(ptr, 2, limitPtr);
			item.itemID = GetUns16BE(ptr);
			ptr += 2;
		}
		else {
			CheckPointerLimitReturnFalse(ptr, 4, limitPtr);
			item.itemID = GetUns32BE(ptr);
			ptr += 4;
		}

		CheckPointerLimitReturnFalse(ptr, 2, limitPtr);
		item.itemProtectionIndex = GetUns16BE(ptr);
		ptr += 2;

		CheckPointerLimitReturnFalse(ptr, 4, limitPtr);
		item.itemType = GetUns32BE(ptr);
		ptr += 4;

		//const XMP_Uns8 *offset = ptr;

		size_t limit = infeInfo.content + infeInfo.contentSize - ptr;
		XMP_Uns8 * posNullChar = (XMP_Uns8 *)memchr(ptr, '\0', limit);
		size_t len = 0;
		if (posNullChar != nullptr) 
			len = (size_t)(posNullChar - ptr);
		else
			//If null char not found, conisder it till end of content
			len = (size_t)(infeInfo.content + infeInfo.contentSize - ptr);
		
		item.itemName.assign((char*)ptr, len);
		ptr = ptr + len + 1;
		limit = infeInfo.content + infeInfo.contentSize - ptr;
		if (item.itemType == ISOMedia::k_mime && limit > 0) {

			posNullChar = (XMP_Uns8 *)memchr(ptr, '\0', limit);
			if (posNullChar != nullptr) 
				len = posNullChar - ptr;
			else 
				len = (size_t)(infeInfo.content + infeInfo.contentSize - ptr);
			item.contentType.assign((char*)ptr, len);
			ptr = ptr + len + 1;

			limit = infeInfo.content + infeInfo.contentSize - ptr;
			posNullChar = (XMP_Uns8 *)memchr(ptr, '\0', limit);

			if (posNullChar != nullptr && ptr < infeInfo.content + infeInfo.contentSize && limit > 0)
			{
				len = posNullChar - ptr;
				item.contenEncoding.assign((char*)ptr, len);
				ptr = ptr + len + 1;
			}
		}
		else if (item.itemType == ISOMedia::k_uri) {
			if (posNullChar != nullptr) {
				posNullChar = (XMP_Uns8 *)memchr(ptr, '\0', limit);
				len = posNullChar - ptr;
				item.itemUriType.assign((char*)ptr, len);
			}	
		}

		itemInfoEntryList.push_back(item);
	}

	return (itemInfoEntryList.size() >= 1);

}

// =================================================================================================
// Meta_Manager::ImportItemLocationInformation
// ==============================
bool Meta_Manager::ImportItemLocationInformation() {
	
	ISOBaseMedia_Manager::BoxInfo ilocInfo;
	ISOBaseMedia_Manager::BoxRef  ilocRef = this->GetTypeChild(&subtreeRootNode, ISOMedia::k_iloc, &ilocInfo);
	
	if (ilocInfo.content == nullptr && ilocInfo.contentSize < 4)
		return false;
	
	itemLocationMap.clear();

	//Allowed Versions are 0,1,2
	XMP_Uns8 version = ilocInfo.content[0];
	if (version > 2)
		return false; 

	const XMP_Uns8 * limitPtr = ilocInfo.content + ilocInfo.contentSize;
	const XMP_Uns8 * ptr = ilocInfo.content + 4; // condition contentSize < 4 already checked
	
	CheckPointerLimitReturnFalse(ptr, 2, limitPtr);
	XMP_Uns16 sizeValue = GetUns16BE(ptr);
	ptr += 2;

	ilocByteSizesStruct.baseOffsetSize = ((sizeValue & 0x00F0) >> 4) * 8;
	ilocByteSizesStruct.lengthSize = ((sizeValue & 0x0F00) >> 8) * 8;
	ilocByteSizesStruct.offsetSize = ((sizeValue & 0xF000) >> 12) * 8;
	ilocByteSizesStruct.indexSize = (sizeValue & 0x000F) * 8;


	XMP_Uns32 itemCount = 0;
	if (version < 2) {
		CheckPointerLimitReturnFalse(ptr, 2, limitPtr);
		itemCount = GetUns16BE(ptr);
		ptr += 2;
		
	}
	else if (version == 2) {
		CheckPointerLimitReturnFalse(ptr, 4, limitPtr);
		itemCount = GetUns32BE(ptr);
		ptr += 4;
	}
	
	const XMP_Uns8 * initialptr; 

	for (XMP_Uns32 i = 0; i < itemCount ; i++) {
		initialptr = ptr;
		XMP_Uns32 itemID = 0;
		ilocItem ilm;
		if (version < 2) {
			CheckPointerLimitReturnFalse(ptr, 2, limitPtr);
			itemID = GetUns16BE(ptr);
			ptr += 2;
		}
		else if (version == 2) {
			CheckPointerLimitReturnFalse(ptr, 4, limitPtr);
			itemID = GetUns32BE(ptr);
			ptr += 4;
		}

		if (version == 1 || version == 2) {
			CheckPointerLimitReturnFalse(ptr, 2, limitPtr);
			XMP_Uns16 value1 = GetUns16BE(ptr);
			ilm.constructionMethod = (value1 & 0x000F);
			ptr += 2;
		}
		
		CheckPointerLimitReturnFalse(ptr, 2, limitPtr);
		ilm.dataReferenceIndex = GetUns16BE(ptr);
		ptr += 2;

		switch (ilocByteSizesStruct.baseOffsetSize) {
		case 0: /*ptr = ptr + 2;*/ break; 
		case 32: CheckPointerLimitReturnFalse(ptr, 4, limitPtr); GetUns32BE(ptr); ptr += 4; break;
		case 64: CheckPointerLimitReturnFalse(ptr, 8, limitPtr); GetUns64BE(ptr); ptr += 8;  break;
		}

		CheckPointerLimitReturnFalse(ptr, 2, limitPtr); 
		XMP_Uns16 extentCount = GetUns16BE(ptr);
		ptr += 2;

		XMP_Uns64 sumPrevExtentsLength = 0;
		//No need to throw exception in this case, it'll be thrown by ProcessXMP once false is returned
		if (extentCount < 1) {
			ilm.sizeOfItem = (XMP_Uns32)(ptr - initialptr);
			itemLocationMap[itemID] = ilm;
			continue; // tolerate if any item is corrupted as in having no extent
		}
			
		for (size_t j = 0; j < extentCount; j++) {
			extent ex;
			if ((version == 1 || version == 2)) { 
				// part4 = index_size // && byteSizes.part4 > 0 
				switch (ilocByteSizesStruct.indexSize) {
				case 0:  ex.extent_index = 1; break; // 0 is reserved
				case 32: CheckPointerLimitReturnFalse(ptr, 4, limitPtr); ex.extent_index = GetUns32BE(ptr);  ptr += 4; break;
				case 64: CheckPointerLimitReturnFalse(ptr, 8, limitPtr); ex.extent_index = GetUns64BE(ptr);  ptr += 8; 
				}
			}

			//offset_size
			switch (ilocByteSizesStruct.offsetSize) {
			case 0:  ex.extent_offset = 0;  break;
			case 32: CheckPointerLimitReturnFalse(ptr, 4, limitPtr); ex.extent_offset = GetUns32BE(ptr); ptr += 4; break;
			case 64: CheckPointerLimitReturnFalse(ptr, 8, limitPtr); ex.extent_offset = GetUns64BE(ptr); ptr += 8; 
			}

			//length_size
			//According to spec, If length is not specified, or specified as zero, then entire length of source is implied
			switch (ilocByteSizesStruct.lengthSize) {
			case 0:  ex.extent_length = 0; /*skip += 2;  length = fileLength;*/ break; //length of entire source 
			case 32: CheckPointerLimitReturnFalse(ptr, 4, limitPtr); ex.extent_length = GetUns32BE(ptr); ptr += 4; break;
			case 64: CheckPointerLimitReturnFalse(ptr, 8, limitPtr); ex.extent_length = GetUns64BE(ptr); ptr += 8; 
			}

			ex.relativeOffsetInData = sumPrevExtentsLength;
			sumPrevExtentsLength += ex.extent_length;
			ilm.iExtents.push_back(ex);
		}
		
		ilm.sizeOfItem = (XMP_Uns32)(ptr - initialptr);
		itemLocationMap[itemID] = ilm;
	}

	return (itemLocationMap.size() >= 1);
}

// =================================================================================================
// Meta_Manager::ImportDataInformationBox
// ==============================
bool  Meta_Manager::ImportDataInformationBox() {
	
	ISOBaseMedia_Manager::BoxRef  dinfRef, drefRef;
	ISOBaseMedia_Manager::BoxInfo dinfInfo, drefInfo;
	dinfRef = this->GetTypeChild(&subtreeRootNode, ISOMedia::k_dinf, &dinfInfo);

	if (dinfRef == 0 || dinfInfo.content == nullptr || dinfInfo.contentSize == 0)
		return false;
	
	drefRef = this->GetTypeChild(dinfRef, ISOMedia::k_dref, &drefInfo);

	if (drefInfo.content == nullptr || drefInfo.contentSize != 8)
		return false;

	XMP_Uns32  entry_count = GetUns32BE(drefInfo.content + 4);
	if (entry_count != drefInfo.childCount)
		// because other boxes use indexes to refer here. So if it doesn't match here that would mean we may be refrencing wrong index
		return false;

	for (XMP_Uns32 i = 0; i < entry_count; i++) {
		ISOBaseMedia_Manager::BoxInfo deInfo;
		ISOBaseMedia_Manager::BoxRef  deRef = GetNthChild(drefRef, i, &deInfo);

		// It may happen there is box can end on version flags
		if (!(deInfo.content != nullptr &&  deInfo.contentSize >= 4 &&
			(deInfo.boxType == ISOMedia::k_url || deInfo.boxType == ISOMedia::k_urn)))
			return false;

		XMP_Uns32 flag = (GetUns32BE(deInfo.content) & 0x0FFF);
		dinfFlags.push_back(flag);

	}
	
	return true;
}

// =================================================================================================
// Meta_Manager::ImportItemReferenceBox
// ==============================
bool Meta_Manager::ImportItemReferenceBox(){
	
	ISOBaseMedia_Manager::BoxInfo irefInfo;
	ISOBaseMedia_Manager::BoxRef  irefRef = this->GetTypeChild(&subtreeRootNode, ISOMedia::k_iref, &irefInfo);

	//iref is a fullbox, and it's content excluding child boxes are version and flags
	if (!irefRef || irefInfo.content == nullptr || irefInfo.contentSize != 4) 
		return false;

	XMP_Uns8 version = irefInfo.content[0];

	if(version != 0 && version != 1)
		XMP_Throw("HEIF_ProcessXMP - Essential boxes for metadata processing missing/malformed", kXMPErr_BadValue);

	XMP_Uns32 minSize;
	(version == 0) ? minSize = 6 : minSize = 10;
	XMP_Uns32 i;
	for (i= 0; i < irefInfo.childCount; i++) {
		ISOBaseMedia_Manager::BoxInfo childInfo;
		ISOBaseMedia_Manager::BoxRef  childRef = GetNthChild(irefRef, i, &childInfo);

		// child is a box
		if (childRef == 0 || childInfo.content == nullptr || childInfo.contentSize < minSize)
			return false; 
		
		XMP_Uns32 referenceType = childInfo.boxType, fromItemID;
		std::unordered_map<XMP_Uns32, std::vector<XMP_Uns32>> temp;
		ItemReferenceTypeMap::iterator irefIterator = irefBox.find(referenceType);
		if(irefIterator == irefBox.end())
			irefBox[referenceType] = temp;	
		
		if (version == 0) 
			fromItemID = (XMP_Uns32)GetUns16BE(childInfo.content);
		else
			fromItemID = GetUns32BE(childInfo.content);

		irefBox[referenceType][fromItemID] = std::vector<XMP_Uns32>();
		std::vector<XMP_Uns32> &toItemIDs = irefBox[referenceType][fromItemID];
		XMP_Uns16 referenceCount = GetUns16BE(childInfo.content + 2);
		toItemIDs.assign(referenceCount, 0);
        const XMP_Uns8 * limitPtr = childInfo.content + childInfo.contentSize;

		if (version == 0) {
            CheckPointerLimitReturnFalse(childInfo.content,  4 + (2 * referenceCount), limitPtr);
			for (int j = 0; j < referenceCount; j++)
				toItemIDs[j] = (GetUns16BE(childInfo.content + 4 + (2 * j)));

		}
		else{
            CheckPointerLimitReturnFalse(childInfo.content, 6 + (4 * referenceCount), limitPtr);
			for (int j = 0; j < referenceCount; j++)
				toItemIDs[j] = (GetUns32BE(childInfo.content + 6 + (4 * j)));
			
		}
	}

	return (i == irefInfo.childCount);
}
#endif //AdobePrivate

// Find a box given the type path. Pick the first child of each type.
ISOBaseMedia_Manager::BoxRef Meta_Manager::GetBox(const char * boxPath, BoxInfo * info) const
{
	size_t pathLen = strlen(boxPath);
	XMP_Assert((pathLen >= 4) && XMP_LitNMatch(boxPath, "meta", 4));
	if (info != 0) memset(info, 0, sizeof(BoxInfo));

	const char * pathPtr = boxPath + 5;	// Skip the "meta/" portion.
	const char * pathEnd = boxPath + pathLen;

	BoxRef currRef = &this->subtreeRootNode;

	while (pathPtr < pathEnd) {

		XMP_Assert((pathEnd - pathPtr) >= 4);
		XMP_Uns32 boxType = GetUns32BE(pathPtr);
		pathPtr += 5;	// ! Don't care that the last step goes 1 too far.

		currRef = this->GetTypeChild(currRef, boxType, 0);
		if (currRef == 0) return 0;

	}

	this->FillBoxInfo(*((BoxNode*)currRef), info);
	return currRef;

}

#if AdobePrivate
bool Meta_Manager::AddItemInfoEntry(XMP_Uns32 itemID, itemInfoEntry iItem) {

	ISOBaseMedia_Manager::BoxInfo ilocInfo;
	ISOBaseMedia_Manager::BoxRef  ilocRef = this->GetTypeChild(&subtreeRootNode, ISOMedia::k_iloc, &ilocInfo);

	if (ilocInfo.content == nullptr)
		return false;

	XMP_Uns8 ilocVersion = ilocInfo.content[0];

	ISOBaseMedia_Manager::BoxInfo iinfInfo;
	ISOBaseMedia_Manager::BoxRef  iinfRef = this->GetTypeChild(&subtreeRootNode, ISOMedia::k_iinf, &iinfInfo);

	//content limit check already performed during Import

	XMP_Uns8 iinfVersion = iinfInfo.content[0];

	if (iinfVersion > 1)
		return false;

	std::string newData;
	iItem.version = (ilocVersion < 2) ? 2 : 3;
	iItem.itemID = itemID;
	itemInfoEntryList.push_back(iItem);

	size_t size = sizeof(iItem);
	newData.assign((XMP_Uns32)size, 0);

	XMP_Uns32 vecIndex = 0;

	if (iItem.version == 2)
	{
		PutUns32BE(0x02000000, &newData[vecIndex]); vecIndex += 4; //version (2) + flags
		PutUns16BE((XMP_Uns16)iItem.itemID, &newData[vecIndex]); vecIndex += 2;
	}
	else
	{
		PutUns32BE(0x03000000, &newData[vecIndex]); vecIndex += 4; //version (3) + flags
		PutUns32BE(iItem.itemID, &newData[vecIndex]); vecIndex += 4;
	}

	PutUns16BE(iItem.itemProtectionIndex, &newData[vecIndex]); vecIndex += 2;
	PutUns32BE(iItem.itemType, &newData[vecIndex]); vecIndex += 4;
	memcpy(&newData[vecIndex], "\0", 1); vecIndex += 1;
	memcpy(&newData[vecIndex], iItem.contentType.c_str(), iItem.contentType.size());
	vecIndex += (XMP_Uns32)iItem.contentType.size();

	// 1 added in vecIndex for empty string to indicate absence of content_encoding
	this->AddChildBox(iinfRef, ISOMedia::k_infe, newData.c_str(), vecIndex + 1);


	//updating entry_count in iinfbox
	if (iinfVersion == 0) {
		XMP_Uns16 entryCount = GetUns16BE(iinfInfo.content + 4) + 1;
		if (entryCount == this->itemInfoEntryList.size())
			PutUns16BE((XMP_Uns16)this->itemInfoEntryList.size(), (void **)(iinfInfo.content + 4));
	}
	else {
		XMP_Uns32 entryCount = GetUns32BE(iinfInfo.content + 4) + 1;
		if (entryCount == this->itemInfoEntryList.size())
			PutUns32BE((XMP_Uns32)this->itemInfoEntryList.size(), (void **)(iinfInfo.content + 4));
	}

	this->SetBox(iinfRef, iinfInfo.content, iinfInfo.contentSize);

	NoteChange();
	return true;
		
		
}


bool Meta_Manager::AddReferenceToPrimaryItem(XMP_Uns32 primaryItemID, XMP_Uns32 itemID) {

	ISOBaseMedia_Manager::BoxInfo irefInfo;
	ISOBaseMedia_Manager::BoxRef  irefRef = this->GetTypeChild(&subtreeRootNode, ISOMedia::k_iref, &irefInfo);

	XMP_Uns8 version;
	if (irefRef)
		version = irefInfo.content[0];
	else {
		ISOBaseMedia_Manager::BoxInfo ilocInfo;
		ISOBaseMedia_Manager::BoxRef  ilocRef = this->GetTypeChild(&subtreeRootNode, ISOMedia::k_iloc, &ilocInfo);

		XMP_Uns8 ilocVersion = ilocInfo.content[0];

		//Matching version of iloc to refrerence box becuase ilocVersion==2 has 32 bits item_id & 16 bits otherwise
		ilocVersion != 2 ? version = 0 : version = 1;

		//Create & Add iref box to meta box
		RawDataBlock buffer;
		buffer.assign(4, 0);
		PutUns32BE(0, &buffer[0]);
		this->AddChildBox(&subtreeRootNode, ISOMedia::k_iref, &buffer[0], 4);
		irefRef = this->GetTypeChild(&subtreeRootNode, ISOMedia::k_iref, &irefInfo);
		
		XMP_Assert(irefRef != 0);
	}
	
	RawDataBlock buffer;
	if (version == 0) {
		buffer.assign(6, 0);
		PutUns16BE((XMP_Uns16)itemID,&buffer[0]);
		PutUns16BE(1, &buffer[0]+2); //only one reference from new item to the primary item
		PutUns16BE((XMP_Uns16)primaryItemID, &buffer[0] +4);
		this->AddChildBox(irefRef, ISOMedia::k_cdsc, &buffer[0], 6);
	}
	else if (version == 1) {
	
		buffer.assign(10, 0);
		PutUns32BE(itemID, &buffer[0]);
		PutUns16BE(1, &buffer[0] + 4); //only one reference from new item to the primary item
		PutUns32BE(primaryItemID, &buffer[0] + 6);
		this->AddChildBox(irefRef, ISOMedia::k_cdsc, &buffer[0], 10);
	}

	NoteChange(); 
	
	return true;

}

void Meta_Manager::RemoveReferenceToPrimaryItem(XMP_Uns32 primaryItemID, XMP_Uns32 itemID) {
	ISOBaseMedia_Manager::BoxInfo irefInfo;
	ISOBaseMedia_Manager::BoxRef  irefRef = this->GetTypeChild(&subtreeRootNode, ISOMedia::k_iref, &irefInfo);

	//No need to check version or existence of box, if this point is reached then these would have been checked earlier
	XMP_Assert(irefRef);
	XMP_Uns8 version = irefInfo.content[0];
	ItemReferenceTypeMap::const_iterator it = irefBox.find(ISOMedia::k_cdsc);
	
	XMP_Assert(it != irefBox.end());
		
	RelatedItemsMap itemsMap = it->second;
	RelatedItemsMap::iterator vecItr = itemsMap.find(itemID); //item id of metadata item

	std::vector<XMP_Uns32> &toItems = vecItr->second;
	toItems.erase(std::remove(toItems.begin(), toItems.end(), primaryItemID), toItems.end());//remove primary itemid from the refrences
	
	// Check size of vector
	// if size == 0, delete the box. else change the contents
	bool deletebox = false;

	if (toItems.size() == 0)
		deletebox = true;
	
	for (XMP_Uns32 i = 0; i < irefInfo.childCount; i++) {
		ISOBaseMedia_Manager::BoxInfo childInfo;
		ISOBaseMedia_Manager::BoxRef  childRef = GetNthChild(irefRef, i, &childInfo);
		XMP_Uns32 referenceType = childInfo.boxType, fromItemID;
		XMP_Uns32 newSize;
		if (referenceType != ISOMedia::k_cdsc)
			continue;

		if (version == 0)
			fromItemID = (XMP_Uns32)GetUns16BE(childInfo.content);
		else
			fromItemID = GetUns32BE(childInfo.content);

		if (fromItemID != itemID)
			continue;

		//Now we have the reqd iref box

		if (deletebox) {
			this->DeleteNthChild(irefRef, i);
			break;
		}
		ISOBaseMedia_Manager::BoxNode * node = (ISOBaseMedia_Manager::BoxNode*)childRef;
		XMP_Uns16 referenceCount = (XMP_Uns16)toItems.size();
		newSize = (version == 0) ? (2 + 2 + referenceCount * 2) : (4 + 2 + referenceCount * 4);
		XMP_Uns32 vecIndex = 0; //only a box and not Fullbox
		node->changedContent.assign(newSize, 0);
		if (version == 0) {
			PutUns16BE((XMP_Uns16)itemID, &(node->changedContent[vecIndex]));
			vecIndex += 2;
		}
		else {
			PutUns32BE(itemID, &(node->changedContent[vecIndex]));
			vecIndex += 4;
		}
		//update refrence count
		PutUns16BE(referenceCount, &(node->changedContent[vecIndex])); vecIndex += 2;

		for (int j = 0; j < referenceCount; j++) {
			//put contents of toItems
			if (version == 0) {
				PutUns16BE((XMP_Uns16)toItems[j], &(node->changedContent[vecIndex]));
				vecIndex += 2;
			}
			else {
				PutUns32BE(toItems[j], &(node->changedContent[vecIndex]));
				vecIndex += 4;
			}

		}
		node->changed = true;
		break;
		

	}
	NoteChange();

}
#endif //AdobePrivate

// =================================================================================================
// Meta_Manager::NewSubtreeSize
// ============================
//
// Determine the new (changed) size of a subtree. Ignore 'free' and 'wide' boxes.
XMP_Uns32 Meta_Manager::NewSubtreeSize(const BoxNode & node, const std::string & parentPath)
{
	XMP_Uns32 subtreeSize;
	if (node.contentSize != 0)
		subtreeSize = 8 + node.contentSize;	// All boxes will have 8 byte headers.
	else
		subtreeSize = 8;

	if (node.boxType == ISOMedia::k_uuid)
		subtreeSize += 16;				// id of uuid is 16 bytes long
	

		for (size_t i = 0, limit = node.children.size(); i < limit; ++i) {

			char suffix[6];
			suffix[0] = '/';
			PutUns32BE(node.boxType, &suffix[1]);
			suffix[5] = 0;
			std::string nodePath = parentPath + suffix;

			subtreeSize += this->NewSubtreeSize(node.children[i], nodePath);
			XMP_Enforce(subtreeSize < TopBoxSizeLimit);

		}

		return subtreeSize;

}// Meta_Manager::NewSubtreeSize

// =================================================================================================
// Meta_Manager::UpdateMemoryTree
// ==============================
void Meta_Manager::UpdateMemoryTree()
{
	if (!this->IsChanged()) return;

	XMP_Uns32 newSize = this->NewSubtreeSize(this->subtreeRootNode, "");
	XMP_Enforce(newSize < TopBoxSizeLimit);

	RawDataBlock newData;
	newData.assign(newSize, 0);	// Prefill with zeroes, can't append multiple items to a vector.

	XMP_Uns8 * newPtr = &newData[0];
	XMP_Uns8 * newEnd = newPtr + newSize;
	
	XMP_Uns8 * trueEnd = this->AppendNewSubtree(this->subtreeRootNode, "", newPtr, newEnd); //meta is a fullbox
	XMP_Enforce(trueEnd == newEnd);

	this->fullSubtree.swap(newData);
	this->ParseMetaMemoryTree();

}	// Meta_Manager::UpdateMemoryTree

#define IncrNewPtr(count)	{ newPtr += count; XMP_Enforce ( newPtr <= newEnd ); }

XMP_Uns8 * Meta_Manager::AppendNewSubtree(const BoxNode & node, const std::string & parentPath,
	XMP_Uns8 * newPtr, XMP_Uns8 * newEnd)
{
	
	/*XMP_Assert ( (node.boxType != ISOMedia::k_meta) ? (node.children.empty() || (node.contentSize == 0)) :
	(node.children.empty() || (node.contentSize == 4)) );*/

	XMP_Enforce((XMP_Uns32)(newEnd - newPtr) >= (8 + node.contentSize));

	//XMP_Uns8 * ilocOrigin;
	
	// Leave the size as 0 for now, append the type and content.
	XMP_Uns8 * boxOrigin = newPtr;	// Save origin to fill in the final size.
	PutUns32BE(node.boxType, (newPtr + 4));
	IncrNewPtr(8); 
	if (node.boxType == ISOMedia::k_uuid)		// For uuid, additional 16 bytes is stored for ID 
	{
		XMP_Enforce((XMP_Uns32)(newEnd - newPtr) >= (16 + node.contentSize));
		memcpy(newPtr, node.idUUID, 16);
		IncrNewPtr(16);
	}
	/*if (node.boxType == ISOMedia::k_iloc)
		ilocOrigin = newPtr;*/

	if (node.contentSize != 0 ) { 
		const XMP_Uns8 * content = PickContentPtr(node);
		memcpy(newPtr, content, node.contentSize);
		IncrNewPtr(node.contentSize);
	}

	// Append the nested boxes.

	if (!node.children.empty()) {

		char suffix[6];
		suffix[0] = '/';
		PutUns32BE(node.boxType, &suffix[1]);
		suffix[5] = 0;
		std::string nodePath = parentPath + suffix;

		for (size_t i = 0, limit = node.children.size(); i < limit; ++i) {
			newPtr = this->AppendNewSubtree(node.children[i], nodePath, newPtr, newEnd);
		}

	}

	// Fill in the final size.

	PutUns32BE((XMP_Uns32)(newPtr - boxOrigin), boxOrigin);
	//ilocOfset = (XMP_Uns32)(ilocOrigin - boxOrigin);
	return newPtr;

}	// MOOV_Manager::AppendNewSubtree

// =================================================================================================
// Meta_Manager::UpdateIlocBoxContent
// ==============================
void Meta_Manager::UpdateIlocBoxContent() {
	ISOBaseMedia_Manager::BoxInfo ilocInfo;
	ISOBaseMedia_Manager::BoxRef  ilocRef = this->GetTypeChild(&subtreeRootNode, ISOMedia::k_iloc, &ilocInfo);

	ISOBaseMedia_Manager::BoxNode * ilocNode = (ISOBaseMedia_Manager::BoxNode*)ilocRef;

	XMP_Uns8 ilocVersion = ilocInfo.content[0];
	if (ilocVersion > 2) //other versions not allowed
		return;
	
	// To know the size of changed content
	XMP_Uns32 newSize = 0;
	if (ilocVersion < 2)
		newSize += 8;
	else 
		newSize += 10;

	for (auto const& itr : itemLocationMap)
	{
		newSize += itr.second.sizeOfItem;
	}
	ilocNode->changedContent.assign(newSize, 0);
	memcpy(&(ilocNode->changedContent[0]), ilocInfo.content, ilocInfo.contentSize);

	XMP_Uns32 vecIndex = 4;//iloc is a FullBox
	//XMP_Uns16 sizeValue = GetUns16BE(ilocInfo.content + 4);
	vecIndex += 2; //sizeValue bytes

	if (ilocVersion < 2) {
		PutUns16BE((XMP_Uns16)itemLocationMap.size(),&( ilocNode->changedContent[vecIndex]));
		vecIndex += 2;
		
	}
	else {
		PutUns32BE((XMP_Uns32)itemLocationMap.size(), &(ilocNode->changedContent[vecIndex]));
		vecIndex += 4;
	}
	
	bool flag = 0;
	for (auto const& itr : itemLocationMap)
	{
		ilocItem currItem = itr.second;
		if (!currItem.changed &&  flag == 0) {
			//memcpy(&(ilocNode->changedContent[vecIndex]), ilocInfo.content + vec, currItem.sizeOfItem);
			vecIndex += currItem.sizeOfItem;
			continue;
		}
		flag = 1;
		if (ilocVersion < 2) {
			XMP_Uns16 itemId = (XMP_Uns16)itr.first;
			PutUns16BE(itemId, &(ilocNode->changedContent[vecIndex]));
			vecIndex += 2;
		}
		else if (ilocVersion == 2) {
			XMP_Uns32 itemId = itr.first;
			PutUns32BE(itemId, &(ilocNode->changedContent[vecIndex]));
			vecIndex += 4;
		}
		if (ilocVersion == 1 || ilocVersion == 2) {
			XMP_Uns16 res = 0;
			res &= 0xFFF0;
			res |= (currItem.constructionMethod & 0x000F);
			PutUns16BE(res, &(ilocNode->changedContent[vecIndex]));
			vecIndex += 2;// reserved + construction method = 0
		}
		PutUns16BE(0, &(ilocNode->changedContent[vecIndex])); //data refrence index
		vecIndex += 2;


		if (ilocByteSizesStruct.baseOffsetSize == 32) { PutUns32BE(0, &(ilocNode->changedContent[vecIndex])); vecIndex += 4; }
		else if (ilocByteSizesStruct.baseOffsetSize == 64) { PutUns64BE(0, &(ilocNode->changedContent[vecIndex])); vecIndex += 8; }
		XMP_Uns16 nExtents = (XMP_Uns16)currItem.iExtents.size();
		PutUns16BE(nExtents , &(ilocNode->changedContent[vecIndex]));
		vecIndex += 2;

		for (size_t j = 0; j < nExtents; j++) { 
			extent ex = currItem.iExtents[j];
	
			if ((ilocVersion == 1 || ilocVersion == 2) && ilocByteSizesStruct.indexSize > 0) { // part4 = index_size // && byteSizes.part4 > 0 	
				if (ilocByteSizesStruct.indexSize == 32) { PutUns32BE((XMP_Uns32)ex.extent_index, &(ilocNode->changedContent[vecIndex])); vecIndex += 4; }
				else if (ilocByteSizesStruct.indexSize == 64) { PutUns64BE(ex.extent_index, &(ilocNode->changedContent[vecIndex])); vecIndex += 8; }
			}

			if (ilocByteSizesStruct.offsetSize == 32) { PutUns32BE((XMP_Uns32)ex.extent_offset, &(ilocNode->changedContent[vecIndex])); vecIndex += 4; }
			else if (ilocByteSizesStruct.offsetSize == 64) { PutUns64BE(ex.extent_offset, &(ilocNode->changedContent[vecIndex])); vecIndex += 8; }

			if (ilocByteSizesStruct.lengthSize == 32) { PutUns32BE((XMP_Uns32)ex.extent_length, &(ilocNode->changedContent[vecIndex])); vecIndex += 4; }
			else if (ilocByteSizesStruct.lengthSize == 64) { PutUns64BE(ex.extent_length, &(ilocNode->changedContent[vecIndex])); vecIndex += 8; }
		}
	}

	ilocNode->changed = true;
	ilocNode->contentSize = newSize;
	NoteChange();
}

#if AdobePrivate
// =================================================================================================
// Meta_Manager::AddIlocItem
// ==============================
void Meta_Manager::AddIlocItem(XMP_Uns32 &id, XMP_Uns32 length, ExtentIDMap &emap) {

	ISOBaseMedia_Manager::BoxInfo ilocInfo;
	ISOBaseMedia_Manager::BoxRef  ilocRef = this->GetTypeChild(&subtreeRootNode, ISOMedia::k_iloc, &ilocInfo);

	if (ilocInfo.content == nullptr)
		XMP_Throw("HEIF_ProcessXMP - Essential boxes for metadata processing missing/malformed", kXMPErr_BadValue);

	//decison: To not to create new iloc box here, if not present
	ilocItem ilocItem;
	ilocItem.constructionMethod = 0;
	ilocItem.dataReferenceIndex = 0;
	extent ex;
	ex.extent_length = length;
	ex.extent_offset = 0;
	ex.relativeOffsetInData = 0;
	ilocItem.iExtents.push_back(ex);
	//ilocItem.sizeOfItem = ;
	XMP_Uns32 maxIlocID = 0,maxInfeID=0,maxID;
	for (auto const& itr : itemLocationMap)
	{
		maxIlocID = std::max(maxIlocID, itr.first);
	
	}

	for (auto const& itr : itemInfoEntryList)
	{
		maxInfeID = std::max(maxInfeID, itr.itemID);

	}
	
	maxID = std::max(maxInfeID, maxIlocID);
	
	XMP_Uns32 itemId = maxID + 1; //By spec, item id's should be in increasing order

	XMP_Uns8 version = ilocInfo.content[0];

	XMP_Uns32 len = 0;

	//Calculate size 
	if (version < 2) len += 2;
	else if (version == 2) len += 4;
	if (version == 1 || version == 2) len += 2;
	len += 2;//dataReferenceIndex

	XMP_Uns16 sizeValue = GetUns16BE(ilocInfo.content + 4);
	//base_offset_size
	len += (ilocByteSizesStruct.baseOffsetSize / 8);
	len += 2; //no. of extents
	//for (size_t j = 0; j < extentCount; j++) { // In future if required very large data to be kept in more than 1 extent
	if ((version == 1 || version == 2)) {
		len += (ilocByteSizesStruct.indexSize / 8);
	}
	//offset_size
	len += (ilocByteSizesStruct.offsetSize / 8);

	len += (ilocByteSizesStruct.lengthSize / 8);
	//}

	ilocItem.sizeOfItem = len;
	ilocItem.changed = true;
	itemLocationMap[itemId] = ilocItem;
	id = itemId;
	//To add this new data
	emap[itemId] = std::make_pair(0, ex);
	//return true;

}
#endif
