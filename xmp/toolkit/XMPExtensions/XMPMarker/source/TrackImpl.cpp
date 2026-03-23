// =================================================================================================
// ADOBE SYSTEMS INCORPORATED
// Copyright 2014 Adobe Systems Incorporated
// All Rights Reserved
//
// NOTICE:  Adobe permits you to use, modify, and distribute this file in accordance with the terms
// of the Adobe license agreement accompanying it.
// =================================================================================================


#define IMPLEMENTATION_HEADERS_CAN_BE_INCLUDED 1
#include "XMPMarker/ImplHeaders/TrackImpl.h"
#include "XMPMarker/ImplHeaders/MarkerImpl.h"
#undef IMPLEMENTATION_HEADERS_CAN_BE_INCLUDED

#include "XMPCommon/Interfaces/IErrorNotifier_I.h"
#include "XMPCommon/Utilities/AutoSharedLock.h"
#include "XMPCommon/Interfaces/IUTF8String_I.h"
#include "XMPCore/Interfaces/INode.h"

#include <iostream>
#include <string>
#include <sstream>

#define TXMP_STRING_TYPE std::string 
// Ensure XMP templates are instantiated
#include "public/include/XMP.incl_cpp"

// Provide access to the API
#include "public/include/XMP.hpp"
#include "XMPCore/source/XMPDocOps-Utils.hpp"
#include "XMPCore/Interfaces/IStructureNode_I.h"
#include "source/UnicodeInlines.incl_cpp"
#include "TXMPDocOps.hpp"
#include "XMPMarker/XMPMarkerFwdDeclarations_I.h"
#include "XMPMarker/Interfaces/IMarker_I.h"
#include "XMPMarker/ImplHeaders/TemporalAssetUtilitiesImpl.h"
#include "XMPCommon/XMPCommonFwdDeclarations_I.h"
#include "XMPCommon/Interfaces/IUTF8String_I.h"
#include "XMPCore/XMPCoreFwdDeclarations.h"
#include "XMPCore/Interfaces/IStructureNode.h"
#include "XMPCore/source/XMPDocOps-Utils.hpp"
#include "XMPCore/Interfaces//IArrayNode.h"
#include "XMPCore/Interfaces/ISimpleNode.h"
#include "XMPCore/Interfaces/IStructureNode_I.h"
#include "XMPCore/Interfaces/IStructureNode.h"
#include "XMPCore/Interfaces/ICoreObjectFactory_I.h"
#include "XMPCore/Interfaces/ICompositeNode.h"
#include "XMPCore/Interfaces/INodeIterator.h"
#include "XMPCore/Interfaces/INameSpacePrefixMap.h"
#include "XMPCommon/XMPCommonFwdDeclarations_I.h"
#include "XMPCommon/Utilities/TSmartPointers_I.h"

namespace AdobeXMPAM_Int {
	using namespace AdobeXMPAM_Int;
	using namespace AdobeXMPCore;
	using namespace AdobeXMPCommon;
	using namespace AdobeXMPAM;
	const sizet kXMP_NS_DM_LEN = strlen(kXMP_NS_DM);

	bool IsBitSet(uint64 inputMask, uint64 checkMask);

	bool IsStringPresent(const char * str, const spcIUTF8StringList_I  & strList) {
		
		if (!str) return false;
		for (size_t idx = 0; idx < strList->size(); ++idx) {
			if (strList->at(idx)) {

				if (!strcmp(str, strList->at(idx)->c_str())) {

					return true;
				}
			}
		}

		return false;
	}
	void ConvertBitMaskToCommaSeparatedStringTrack(const uint64 & mask, std::string & str) {

		TemporalAssetUtilities::InitMaps();
		str.clear();
		bool isFirst = true;
		for (uint64 i = 0; i < 32; i++) {

			if (IsBitSet(mask, 1 << i) && markerTypeMap.find(1 << i) != markerTypeMap.end()) {

				if (isFirst) {

					str += markerTypeMap[1 << i];
					isFirst = false;
				}
				else {

					str += "," + markerTypeMap[1 << i];
				}
			}
		}
	}

	void ConvertCustomTypesToCommaSeparatedStringTrack(const std::vector<spIUTF8String> & listOfTypes, std::string & str) {

		bool isFirst = (str.length() == 0);
		for (size_t idx = 0, count = listOfTypes.size(); idx < count; ++idx) {

			if (isFirst) {

				str += listOfTypes[idx]->c_str();
				isFirst = false;
			}
			else {

				str += ",";
				str += listOfTypes[idx]->c_str();
			}
		}
	}

	void CreateCommaSeparatedStringTrack(const uint64 & mask, const std::vector < spIUTF8String > & listOfTypes, std::string & str) {
		
		TemporalAssetUtilities::InitMaps();
		ConvertBitMaskToCommaSeparatedStringTrack(mask, str);
		ConvertCustomTypesToCommaSeparatedStringTrack(listOfTypes, str);
	}

	bool DoesPointLieInTheInterval(uint64 point, uint64 start, uint64 count) {

		return point >= start && point <= start + count;
	}

	bool DoesMarkerMatchTimeCriteria(const spIMarker_I & marker, uint64 start, uint64 count, ITrack::eTrackTimeBoundary timeBoundary) {

		uint64 markerStart, markerEnd;
		marker->GetFrameCountSpan(markerStart, markerEnd);
		markerEnd += markerStart;
		switch (timeBoundary) {

		case  ITrack_v1::kTTBothStartTimeAndEndTimeExcluded:
			return !DoesPointLieInTheInterval(markerStart, start, count) && !DoesPointLieInTheInterval(markerEnd, start, count);
			break;
		case  ITrack_v1::kTTBothStartTimeAndEndTimeIncluded:
			return DoesPointLieInTheInterval(markerStart, start, count) && DoesPointLieInTheInterval(markerEnd, start, count);
			break;
		case  ITrack_v1::kTTOnlyEndTimeIncluded:
			return !DoesPointLieInTheInterval(markerStart, start, count) && DoesPointLieInTheInterval(markerEnd, start, count);
			break;
		case  ITrack_v1::kTTOnlyStartTimeIncluded:
			return DoesPointLieInTheInterval(markerStart, start, count) && !DoesPointLieInTheInterval(markerEnd, start, count);
			break;
		case ITrack_v1::kTTMax:
			return true;
			break;
		}
		return false;
	}

	static XMP_StringPtr kTrackName = "trackName";
	static XMP_StringPtr kMarkers = "markers";
	static XMP_StringPtr kFrameRate = "frameRate";
	static XMP_StringPtr kTrackType = "trackType";


	void TrackImpl::RemoveMarkersNode() {

		if (mMarkerListNode) {

			mRootNode->RemoveNode(kXMP_NS_DM, kXMP_NS_DM_LEN,  kMarkers, strlen(kMarkers));
			mMarkerListNode = spIArrayNode();
		}
	}

	void TrackImpl::RemoveNameNode() {

		if (mNameNode) {

			mRootNode->RemoveNode(kXMP_NS_DM, kXMP_NS_DM_LEN, kTrackName, strlen(kTrackName));
			mNameNode = spISimpleNode();
		}
	}

	void TrackImpl::RemoveTypeNode() {

		if (mTypeNode) {

			mRootNode->RemoveNode(kXMP_NS_DM, kXMP_NS_DM_LEN, kTrackType, strlen(kTrackType));
			mTypeNode = spISimpleNode();
		}

	}

	void TrackImpl::RemoveFrameRateNode() {

		if (mDefaultFrameRateNode) {

			mRootNode->RemoveNode(kXMP_NS_DM, kXMP_NS_DM_LEN, kFrameRate, strlen(kFrameRate));
			mDefaultFrameRateNode = spISimpleNode();
		}
	}

	void TrackImpl::CreateMarkersNode() {

		if (!mMarkerListNode) {

			mMarkerListNode = IArrayNode::CreateOrderedArrayNode(kXMP_NS_DM, kXMP_NS_DM_LEN, kMarkers, strlen(kMarkers));
			mRootNode->AppendNode(mMarkerListNode);
		}
	}

	void TrackImpl::CreateNameNode() {

		if (!mNameNode) {

			mNameNode = ISimpleNode::CreateSimpleNode(kXMP_NS_DM, kXMP_NS_DM_LEN, kTrackName, strlen(kTrackName));
			mRootNode->AppendNode(mNameNode);
		}
	}

	void TrackImpl::CreateTypeNode() {

		if (!mTypeNode) {
			mTypeNode = ISimpleNode::CreateSimpleNode(kXMP_NS_DM, kXMP_NS_DM_LEN, kTrackType, strlen(kTrackType));
			mRootNode->AppendNode(mTypeNode);
		}

	}

	void TrackImpl::CreateFrameRateNode() {

		if (!mDefaultFrameRateNode) {

			mDefaultFrameRateNode = ISimpleNode::CreateSimpleNode(kXMP_NS_DM, kXMP_NS_DM_LEN, kFrameRate, strlen(kFrameRate));
			mRootNode->AppendNode(mDefaultFrameRateNode);

		}
	}

	spITrack_I ITrack_I::CreateTrack_I(const char * trackName, sizet trackNameLen, uint64 trackTypes, const char * trackTypeName, sizet trackTypeNameLen) {

		return MakeUncheckedSharedPointer(new TrackImpl(trackName, trackNameLen, trackTypes, trackTypeName, trackTypeNameLen), __FILE__, __LINE__, true);
	}

	TrackImpl::TrackImpl(const char * trackName, sizet trackNameLen, uint64 trackTypes, const char * trackTypeName, sizet trackTypeLen) {
		
		if (!trackName)  {

			/*	NOTIFY_WARNING(IError_base::kDomainGeneral, IError_base::kGeneralCodeParametersNotAsExpected,
					"Parameters to CreateMarker() are not as expected",
					true, (void *)trackName,
					trackName, trackName);*/
			return;
		}

		if (trackTypes & IMarker::kMTUserDefinedMarker) {
			if (!trackTypeName) {

				/*NOTIFY_WARNING(IError_base::kDomainGeneral, IError_base::kGeneralCodeParametersNotAsExpected,
					"Parameters to CreateMarker() are not as expected",
					true, (void *)trackTypeName,
					trackTypeName, trackTypeName);*/
				return;
			}

		}

		mRootNode = IStructureNode::CreateStructureNode(kXMP_NS_DM, kXMP_NS_DM_LEN, "track", strlen("track"));
		mNameNode = ISimpleNode::CreateSimpleNode(kXMP_NS_DM, kXMP_NS_DM_LEN, kTrackName, strlen(kTrackName), trackName, trackNameLen);
		mRootNode->AppendNode(mNameNode);
		mTypeMask = trackTypes;
		if (trackTypeName) {

			mTypeMask |= IMarker::kMTUserDefinedMarker;
			spIUTF8String customType = IUTF8String_I::CreateUTF8String(trackTypeName, trackTypeLen);
			mCustomTypes.push_back((customType));
			mCustomTypeMap[trackTypeName] = true;
		}
		std::string typeString;
		CreateCommaSeparatedStringTrack(mTypeMask, mCustomTypes, typeString);

		if (typeString.length()) {
			CreateTypeNode();
			mTypeNode->SetValue(typeString.c_str(), typeString.length());
		}

		mDefaultFrameRate.first = mDefaultFrameRate.second = 1;

	}

	bool TrackImpl::CheckSuitability(const spIMarker_I & marker ) {


		const char * guid = marker->GetGUID_I()->c_str();
		for (size_t idx = 0; idx < markerList.size(); ++idx) {

			if (!strcmp(guid, markerList[idx]->GetGUID_I()->c_str())) {
				return false;
			}
		}

		return true;
	}

	spIMarker TrackImpl::AddMarker_I(const char * markerName, sizet markerNameLen, uint64 markerTypes, const char * markerTypeName, sizet markerTypeNameLen, const char * guid, sizet guidLen) {


		if (!markerName) {
			/*	NOTIFY_WARNING(IError_base::kDomainGeneral, IError_base::kGeneralCodeParametersNotAsExpected,
					"Parameters to CreateMarker() are not as expected",
					true, (void *)markerName,
					markerName, markerName);*/
			return spIMarker_I();
		}
		auto returnType = IMarker_I::CreateMarker_I(markerName, markerNameLen, markerTypes, markerTypeName, markerTypeNameLen, guid, guidLen);
		
		if (!CheckSuitability(returnType)) {

			/*NOTIFY_WARNING(IError_base::kDomainGeneral, IError_base::kGeneralCodeParametersNotAsExpected,
				"Parameters to CreateMarker() are not as expected",
				true, (void *)guid,
				guid, guid);*/
			return spIMarker_I();
		}


		spIStructureNode serializedMarker;
		returnType->Serialize_I(serializedMarker);
		CreateMarkersNode();
		if (!mMarkerListNode) {

			mMarkerListNode = IArrayNode::CreateOrderedArrayNode(kXMP_NS_DM, kXMP_NS_DM_LEN, kMarkers, strlen(kMarkers));
			mRootNode->AppendNode(mMarkerListNode);
		}
		//auto sp = shared_from_this();
		//if (sp)
		//returnType->SetAssociatedTrack_I(static_pointer_cast<ITrack_I> (sp));
		try {
			mMarkerListNode->AppendNode(serializedMarker);
			markerList.push_back(returnType);
		}
		catch (...) {

			return spIMarker_I();
		}
		return returnType;
	}

	spIMarker TrackImpl::AddMarker_I(spIMarker_I marker) {

		if (!CheckSuitability(marker)) {
			//TODO
			//NOTIFY_WARNING(IError_base::kDomainAssetManagement, IError_base::kDomainAssetManagement, "Marker can not be added");

			return spIMarker_I();
		}

		weak_ptr <ITrack_I> associatedTrack = marker->GetAssociatedTrack_I();

		if (auto spt = associatedTrack.lock())  {

			return spIMarker_I();
		}
		
		CreateMarkersNode();

		spIStructureNode serializedMarker;
		marker->Serialize_I(serializedMarker);
		try {
			mMarkerListNode->AppendNode(serializedMarker);
			markerList.push_back(marker);
		}
		catch (...) {

			//TODO
			//NOTIFY_WARNING(IError_base::kDomainGeneral, IError_base::kGeneralCodeParametersNotAsExpected,
			//	"Parameters to AddMarker() are not as expected", marker );

			return spIMarker_I();
		}
		return marker;
	}

	void TrackImpl::SetName_I(const char * name, sizet nameLen) {

		if (!name)  {

			//NOTIFY_WARNING(IError_base::kDomainGeneral, IError_base::kGeneralCodeParametersNotAsExpected,
			//	"Parameters to IXMPNameSpacePrefixMap::AddOrReplacePair() are not as expected",
			//	true, (void *)name,
			//	name, name);
			RemoveNameNode();
			return;

		}
		CreateNameNode();
		mNameNode->SetValue(name, nameLen);
		
	}


	uint64 TrackImpl::SetTypes_I(uint64 inputMask) {

		if (!IsBitSet(mTypeMask, IMarker::kMTUserDefinedMarker) && IsBitSet(inputMask, IMarker::kMTUserDefinedMarker)) {

			if (!mCustomTypes.size()) {

				inputMask &= ~IMarker::kMTUserDefinedMarker;
			}
		}

		mTypeMask |= inputMask;
		std::string typeString;
		CreateCommaSeparatedStringTrack(mTypeMask, mCustomTypes, typeString);
		if (typeString.length()) {
			CreateTypeNode();
			mTypeNode->SetValue(typeString.c_str(), typeString.length());
		}

		return mTypeMask;
	}

	uint64 TrackImpl::RemoveTypes_I(uint64 inputMask) {

		if (!IsBitSet(mTypeMask, IMarker::kMTUserDefinedMarker) && IsBitSet(inputMask, IMarker::kMTUserDefinedMarker)) {

			mCustomTypes.clear();
			mCustomTypeMap.clear();
		}
		while (inputMask) {

				inputMask &= (inputMask - 1);
				mTypeMask |= mTypeMask;
		}

		std::string typeString;
		CreateCommaSeparatedStringTrack(mTypeMask, mCustomTypes, typeString);
		if (typeString.length()) {

			CreateTypeNode();
			mTypeNode->SetValue(typeString.c_str(), typeString.length());
			
		}
		return mTypeMask;
	}


	uint64 APICALL TrackImpl::GetTypes_I() const {

		return mTypeMask;
	}

	bool TrackImpl::AppendCustomType_I(const char * type, sizet typeLen) {

		if (!type) {

			/*NOTIFY_WARNING(IError_base::kDomainGeneral, IError_base::kGeneralCodeParametersNotAsExpected,
				"Parameters to CreateMarker() are not as expected",
				true, (void *)type,
				type, type);*/
			return false;
		}

		if (mCustomTypeMap[type]) {
			return false;
		}
		mTypeMask |= IMarker::kMTUserDefinedMarker;
		spIUTF8String customType = IUTF8String_I::CreateUTF8String(type, typeLen);
		mCustomTypes.push_back((customType));
		mCustomTypeMap[type] = true;
		std::string typeString;
		CreateCommaSeparatedStringTrack(mTypeMask, mCustomTypes, typeString);
		if (typeString.length()) {

			CreateTypeNode();
			mTypeNode->SetValue(typeString.c_str(), typeString.length());
		
		}
		return true;
	}

	bool TrackImpl::RemoveCustomType_I(const char * type, sizet typeLen) {

		if (!type) {

		/*	NOTIFY_WARNING(IError_base::kDomainGeneral, IError_base::kGeneralCodeParametersNotAsExpected,
				"Parameters to Remove custom type are not as expected",
				true, (void *)type,
				type, type);*/
			return false;
		}

		if (!mCustomTypeMap[type]) {
			return false;
		}
		mCustomTypeMap[type] = false;
		bool operationsPerformed = false;
		for (int idx = mCustomTypes.size() - 1; idx >= 0; --idx) {

			if (!strcmp(type, mCustomTypes[idx]->c_str())) {

				operationsPerformed = true;
				mCustomTypes.erase(mCustomTypes.begin() + idx);
			}
		}

		if (!mCustomTypes.size()) {

			mTypeMask &= ~IMarker::kMTUserDefinedMarker;
		}
		std::string typeString;
		CreateCommaSeparatedStringTrack(mTypeMask, mCustomTypes, typeString);
		if (typeString.length()) {

			CreateTypeNode();
			mTypeNode->SetValue(typeString.c_str(), typeString.length());
		}
		return operationsPerformed;
	}

	sizet TrackImpl::GetCustomTypeCount_I() const {

		return mCustomTypes.size();
	}

	spcIUTF8String TrackImpl::GetCustomType_I(sizet index) const {

		if (index <= 0 && index > mCustomTypes.size()) {

			/*NOTIFY_WARNING(IError_base::kDomainGeneral, IError_base::kGeneralCodeParametersNotAsExpected,
				"Parameters to Remove custom type are not as expected",
				true, (void *)index,
				index, index);*/
			return spcIUTF8String();
		}

		return mCustomTypes[index];
	}

	spcIUTF8StringList_I TrackImpl::GetCustomTypes_I() const {
		spcIUTF8StringList_I list(new cIUTF8StringList_I());
		for (size_t idx = 0; idx < mCustomTypes.size(); ++idx) {
			list->push_back( (mCustomTypes[idx]));
		}

		return list;
	}

	spcIUTF8String TrackImpl::GetName_I() const {

		if (!mNameNode) return spcIUTF8String();
		return (mNameNode->GetValue());
	}

	void TrackImpl::GetFrameRate(uint64 & numerator, uint64 & denominator) const {

		numerator = mDefaultFrameRate.first, denominator = mDefaultFrameRate.second;
		return;
	}

	sizet TrackImpl::GetMarkerCount() const {
		if (!mMarkerListNode) return 0;
		return mMarkerListNode->ChildCount();
	}
	//void TrackImpl::AddMarker_I(const spcIMarker_I & marker) {

	//	
	//	spIMarker_I markerToBeAdded = const_pointer_cast<IMarker_I>(marker);
	//	if (marker->GetAssociatedTrack_I()) {
	//		markerToBeAdded = marker->Clone_I();
	//		
	//	}

	//	if (markerGUIDMap.count(markerToBeAdded->GetMarkerGUID_I()->c_str()) ) {

	//		NOTIFY_ERROR(IError_v1::kDomainGeneral, IError_v1::kGeneralCodeParametersNotAsExpected,
	//			"Marker already present with the same guid", IError_v1::kSeverityOperationFatal, true, marker->GetMarkerGUID_I()->c_str());
	//	}

	//	if (!mMarkerListNode) {

	//		mMarkerListNode = IArrayNode::CreateOrderedArrayNode(kXMP_NS_DM, kMarkers);
	//		mRootNode->AppendNode(mMarkerListNode);
	//	}
	//	markerGUIDMap[markerToBeAdded->GetMarkerGUID_I()->c_str()] = markerToBeAdded;
	//	spIStructureNode markerNode;
	//	markerToBeAdded->SerializeMarker_I(markerNode);
	//	mMarkerListNode->AppendNode(markerNode);
	//	markerToBeAdded->SetAssociatedTrack_I(this);
	//	
	//}
	//
	//void TrackImpl::AddMarker(const spcIMarker & marker) {

	//	spcIMarker_I marker_I = dynamic_pointer_cast<const IMarker_I>(marker);
	//	return this->AddMarker_I(marker_I);
	//}

	spIMarkerList_I TrackImpl::GetAllMarkers_I() const{

		spIMarkerList_I matchedMarkerList(new IMarkerList_I());

		for (size_t idx = 0; idx < markerList.size(); ++idx) {

			matchedMarkerList->push_back(markerList[idx]);
		}
		return matchedMarkerList;
	}

	spITrack TrackImpl::Clone_I() const  {
		// TODO Add mutex

		// use mutex
		spITrack sp = ITrack_I::CreateTrack_I(mNameNode->GetValue()->c_str(), mNameNode->GetValue()->size(), 0);
		auto implPointer = dynamic_cast<TrackImpl *>(sp.get());
		CloneContents(sp);

		return sp;
	}
	void TrackImpl::UpdateFrameRateInformationInDOM() {

		std::string frameRate;
		TemporalAssetUtilities::ConvertFrameRateToString(frameRate, mDefaultFrameRate.first, mDefaultFrameRate.second);

		CreateFrameRateNode();
		mDefaultFrameRateNode->SetValue(frameRate.c_str(), frameRate.length());
	}

	void TrackImpl::SetFrameRate_I(uint64 numerator /*= 1*/, uint64 denominator /*= 1*/) {

		if (numerator == 0 || denominator == 0) {
			/*NOTIFY_ERROR(IError_v1::kDomainGeneral, IError_v1::kGeneralCodeParametersNotAsExpected,
				"numerator, denominator or both can't be zero", IError_v1::kSeverityOperationFatal,
				true, numerator, true, denominator);*/
			return;
		}
		mDefaultFrameRate.first = numerator, mDefaultFrameRate.second = denominator;

		return UpdateFrameRateInformationInDOM();
	}

	AdobeXMPAM_Int::spIMarker TrackImpl::RemoveMarker_I(const sizet & markerIdx) {
		if (markerIdx <= 0 || markerIdx > markerList.size()) {

			return spIMarker_I();
		}
		spIMarker_I marker = markerList[markerIdx - 1];
		markerList.erase(markerList.begin() + markerIdx - 1);
		mMarkerListNode->RemoveNodeAtIndex(markerIdx);
		return marker;
	}

	AdobeXMPAM_Int::spIMarker TrackImpl::RemoveMarker_I(const char *guid, sizet guidLen) {
		if (!guid){
			return spIMarker_I();
		}
		
		for (size_t idx = 0; idx < markerList.size(); idx++) {

			if (markerList[idx]->GetGUID_I() && !strncmp(markerList[idx]->GetGUID_I()->c_str(), guid, guidLen)) {
				spIMarker_I markerToBeReturned = markerList[idx];
				markerList.erase(markerList.begin() + idx);
				mMarkerListNode->RemoveNodeAtIndex(idx + 1);
				return markerToBeReturned;
			}
		}

		return spIMarker_I();
	}

	AdobeXMPAM_Int::spIMarker TrackImpl::GetMarker_I(const char * guid, sizet guidLen) const {

		if (!guid){
			return spIMarker_I();
		}

		for (size_t idx = 0; idx < markerList.size(); idx++) {

			if (markerList[idx]->GetGUID_I() && !strncmp(markerList[idx]->GetGUID_I()->c_str(), guid, guidLen)) {

				return markerList[idx];
			}
		}

		return spIMarker_I();
	}

	AdobeXMPAM_Int::spIMarker TrackImpl::GetMarker_I(const sizet & markerIdx) const{

		if (markerIdx <= 0 || markerIdx > markerList.size()) {

			return spIMarker_I();
		}
		spIMarker_I marker = markerList[markerIdx - 1];

		return marker;
		
	}

	void TrackImpl::Serialize_I(spIStructureNode & structNode) const {

		structNode = mRootNode;
		return;
	}

	/*
	Test function for checking  a parameter of type vector < pair < const char *, sizet> >
	*/
	void TrackImpl::foo_I(std::vector< std::pair<const char *, sizet> > & v) {

		for (sizet idx = 0; idx < v.size(); ++idx) {

			std::string str = std::string(v[idx].first, v[idx].second);
		}
	}

	void TrackImpl::CloneContents(spITrack & track) const {


		if (!mRootNode || mRootNode->GetNodeType() != INode::kNTStructure) return;
		auto that = dynamic_cast<TrackImpl *>(track.get());
		spINode clonedNode = mRootNode->Clone();
		(that)->mRootNode = clonedNode->ConvertToStructureNode();
		spIStructureNode mThatRootNode = that->mRootNode;
		if (!mThatRootNode) return;
		if (mTypeNode) (that)->mTypeNode = mThatRootNode->GetSimpleNode(kXMP_NS_DM, kXMP_NS_DM_LEN, kTrackType, strlen(kTrackType));

		if (mNameNode) (that)->mNameNode = mThatRootNode->GetSimpleNode(kXMP_NS_DM, kXMP_NS_DM_LEN, kTrackName, strlen(kTrackName));
		
		if (mDefaultFrameRateNode) (that)->mDefaultFrameRateNode = mThatRootNode->GetSimpleNode(kXMP_NS_DM, kXMP_NS_DM_LEN, kFrameRate, strlen(kFrameRate));
		(that)->mTypeMask = mTypeMask;
		(that)->mCustomTypeMap = mCustomTypeMap;
		(that)->mDefaultFrameRate = mDefaultFrameRate;

		for (size_t idx = 0; idx < mCustomTypes.size(); ++idx) {
			spIUTF8String clonedString = IUTF8String_I::CreateUTF8String(mCustomTypes[idx]->c_str(), mCustomTypes[idx]->size());
			(that)->mCustomTypes.push_back((mCustomTypes[idx]));
		}
		
		mThatRootNode->RemoveNode(kXMP_NS_DM, kXMP_NS_DM_LEN, kMarkers, strlen(kMarkers));
		for (size_t idx = 0, markerCount = markerList.size(); idx < markerCount; ++idx) {
			spIMarker marker = markerList[idx]->Clone_I();
			spIMarker_I markerInternal = MakeUncheckedSharedPointer<IMarker_I>( marker->GetIMarker_I(), __FILE__, __LINE__, false);
			that->AddMarker_I(markerInternal);
		}
		if (mMarkerListNode) (that)->mMarkerListNode = mThatRootNode->GetArrayNode(kXMP_NS_DM, kXMP_NS_DM_LEN, kMarkers, strlen(kMarkers));
		
		/*	(*that)->mFrameRateForCountOfFrames = mFrameRateForCountOfFrames;
			(*that)->mFrameRateForStartFrameCount = mFrameRateForStartFrameCount;
			(*that)->mFrameDuration = mFrameDuration;*/
	}

	AdobeXMPAM_Int::spIMarkerList_I TrackImpl::RemoveMarkers_I(uint64 markerTypes /*= UINT64_MAX*/, bool matchAllCriteria /*= false*/, const std::vector<std::pair< const char *, sizet> > &markerTypeName /*= std::vector<const char *>()*/) {

		if (matchAllCriteria) {

			spIMarkerList_I matchedMarkerList(new IMarkerList_I());

			for (int idx = markerList.size() - 1; idx >= 0; idx--) {

				spIMarker_I marker = markerList[idx];
				if (!marker->HasTypes(markerTypes) && markerTypes != IMarker::kMTMax) continue;
				auto customTypes = marker->GetCustomTypes_I();

				bool notFound = false;
				for (size_t idx2 = 0; idx2 < markerTypeName.size(); ++idx2) {

					if (!IsStringPresent(markerTypeName[idx2].first, customTypes)) {

						notFound = true;
						break;
					}
				}
				if (!notFound) {

					matchedMarkerList->push_back(marker);
					mMarkerListNode->RemoveNodeAtIndex(idx + 1);
					markerList.erase(markerList.begin() + idx);
				}

			}

			return matchedMarkerList;
		}
		else {
			spIMarkerList_I matchedMarkerList(new IMarkerList_I());


			for (int idx = markerList.size() - 1; idx >= 0; idx--) {
				spIMarker_I marker = markerList[idx];
				bool found = false;
				uint64 markerType = marker->GetTypes_I();
				if (markerType & markerTypes || (markerTypes == IMarker::kMTMax)) {
					found = true;

				}
				if (!found) {

					auto customTypes = marker->GetCustomTypes_I();
					for (size_t idx2 = 0; idx2 < markerTypeName.size(); ++idx2) {
						// Use length
						if (IsStringPresent(markerTypeName[idx2].first, customTypes)) {

							found = true;
							break;
						}
					}
				}


				if (found) {

					matchedMarkerList->push_back(marker);
					mMarkerListNode->RemoveNodeAtIndex(idx + 1);
					markerList.erase(markerList.begin() + idx);
				}

			}
			return matchedMarkerList;

		}
	}


	AdobeXMPAM_Int::spIMarkerList_I TrackImpl::GetMarkers_I(uint64 markerTypes /*= UINT64_MAX*/, bool matchAllCriteria /*= false*/, const std::vector<std::pair<const char *, sizet> > &markerTypeName /*= std::vector<const char *>()*/) const {

		if (matchAllCriteria) {

			spIMarkerList_I matchedMarkerList(new IMarkerList_I());

			for (auto marker : markerList) {

				if (!marker->HasTypes(markerTypes) && markerTypes != IMarker::kMTMax) continue;
				auto customTypes = marker->GetCustomTypes_I();
				
				bool notFound = false;
				for (size_t idx = 0; idx < markerTypeName.size(); ++idx) {
					//use length
					if (!IsStringPresent(markerTypeName[idx].first, customTypes)) {

						notFound = true;
						break;
					}
				}
				if (!notFound) {

					matchedMarkerList->push_back(marker);
				}
				
			}

			return matchedMarkerList;
		}
		else {
			spIMarkerList_I matchedMarkerList(new IMarkerList_I());

			
			for (auto marker : markerList) {

				bool found = false;
				uint64 markerType = marker->GetTypes_I();
				if ( (markerType & markerTypes) || (markerTypes == IMarker::kMTMax) ) {
					found = true;

				}
				if (!found) {

					auto customTypes = marker->GetCustomTypes_I();
					for (size_t idx = 0; idx < markerTypeName.size(); ++idx) {

						if (IsStringPresent(markerTypeName[idx].first, customTypes)) {

							found = true;
							break;
						}
					}
				}


				if (found) {

					matchedMarkerList->push_back(marker);
				}
				
			}
			return matchedMarkerList;

		}
	}

	AdobeXMPAM_Int::spIMarkerList_I TrackImpl::GetMarkersByName_I(const char * markerName, sizet markerNameLen) const {

		if (!markerName) {

		/*	NOTIFY_WARNING(IError_base::kDomainGeneral, IError_base::kGeneralCodeParametersNotAsExpected,
				"Parameters to GetMarkersByName are not as expected",
				true, markerName,
				markerName, markerName);
			return spIMarkerList_I();*/
		}

		spIMarkerList_I matchedMarkerList(new IMarkerList_I());

		for (auto marker : markerList) {

			spcIUTF8String name = marker->GetName_I();
			if (name) {

				if (!strcmp(name->c_str(), markerName)) {

					matchedMarkerList->push_back(marker);
				}
			}
		}

		return matchedMarkerList;
	}

	AdobeXMPAM_Int::spIMarkerList_I TrackImpl::GetMarkersInTimeRange_I(uint64 startFrameCount /*= 0*/, uint64 endFrameCount /*= UINT64_MAX*/, eTrackTimeBoundary timeBoundary /*= kTTBothStartTimeAndEndTimeIncluded*/, uint64 markerTypes /*= UINT64_MAX*/, bool matchAllCriteria /*= false*/, const std::vector<std::pair<const char *, sizet> > &markerTypeName /*= std::vector<const char *>()*/) const{
	
		if (matchAllCriteria) {

			spIMarkerList_I matchedMarkerList(new IMarkerList_I());

			for (auto marker : markerList) {

				if (!marker->HasTypes(markerTypes) && markerTypes != IMarker::kMTMax) continue;
				if (!DoesMarkerMatchTimeCriteria(marker, startFrameCount, endFrameCount, timeBoundary)) continue;
				auto customTypes = marker->GetCustomTypes_I();

				bool notFound = false;
				for (size_t idx = 0; idx < markerTypeName.size(); ++idx) {

					if (!IsStringPresent(markerTypeName[idx].first, customTypes)) {

						notFound = true;
						break;
					}
				}
				if (!notFound) {

					matchedMarkerList->push_back(marker);
				}

			}

			return matchedMarkerList;
		}
		else {
			spIMarkerList_I matchedMarkerList(new IMarkerList_I());


			for (auto marker : markerList) {

				bool found = false;
				uint64 markerType = marker->GetTypes_I();
				if ((markerType & markerTypes) || (markerTypes == IMarker::kMTMax)) {
					found = true;
				}
				if (!found) {

					auto customTypes = marker->GetCustomTypes_I();
					for (size_t idx = 0; idx < markerTypeName.size(); ++idx) {
						//TODO length
						if (IsStringPresent(markerTypeName[idx].first, customTypes)) {

							found = true;
							break;
						}
					}
				}
				if (!found) {

					if (DoesMarkerMatchTimeCriteria(marker, startFrameCount, endFrameCount, timeBoundary)) {
						found = true;
					}
				}

				if (found) {

					matchedMarkerList->push_back(marker);
				}

			}
			return matchedMarkerList;

		}

	}

	bool TrackImpl::HasCustomType_I(const char * customType, sizet customTypeLen) const {

		if (!customType) return false;
		auto customTypes = GetCustomTypes_I();
		for (size_t idx = 0; idx < customTypes->size(); ++idx) {
			//TODO length
			if (! strncmp(customTypes->at(idx)->c_str(), customType, customTypeLen ) ) {

				return true;
			}
		}
		return false;
	}

}
namespace AdobeXMPAM {
	using namespace AdobeXMPAM_Int;
	spITrack ITrack_v1::MakeShared(pITrack_base ptr) {
		pITrack p = ITrack::GetInterfaceVersion() > 1 ? ptr->GetInterfacePointer< ITrack >() : ptr;
		return MakeUncheckedSharedPointer(p, __FILE__, __LINE__, false);
	}
}
