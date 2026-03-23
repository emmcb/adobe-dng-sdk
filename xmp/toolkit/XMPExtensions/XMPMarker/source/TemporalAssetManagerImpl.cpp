// =================================================================================================
// ADOBE SYSTEMS INCORPORATED
// Copyright 2014 Adobe Systems Incorporated
// All Rights Reserved
//
// NOTICE:  Adobe permits you to use, modify, and distribute this file in accordance with the terms
// of the Adobe license agreement accompanying it.
// =================================================================================================


#define IMPLEMENTATION_HEADERS_CAN_BE_INCLUDED 1
#include "XMPMarker/ImplHeaders/TemporalAssetManagerImpl.h"
#include "XMPMarker/ImplHeaders/MarkerImpl.h"
#include "XMPMarker/ImplHeaders/TrackImpl.h"
#undef IMPLEMENTATION_HEADERS_CAN_BE_INCLUDED

#include "XMPMarker/Interfaces/IMarker.h"
#include "XMPMarker/Interfaces/ITrack.h"
#include "XMPMarker/Interfaces/IMarker_I.h"
#include "XMPMarker/Interfaces/ITrack_I.h"
#include "XMPCommon/Interfaces/IErrorNotifier_I.h"
#include "XMPCommon/Utilities/AutoSharedLock.h"
#include "XMPCommon/Interfaces/IUTF8String_I.h"
#include "XMPCore/Interfaces/INode.h"
#include "XMPMarker/Interfaces/ITrack_I.h"
#include "XMPMarker/ImplHeaders/TemporalAssetUtilitiesImpl.h"
#include "XMPCore/Interfaces/IArrayNode.h"
#include <iostream>
#include <string>
#include <sstream>
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
#include "source/UnicodeInlines.incl_cpp"
#include "XMPCommon/Utilities/TSmartPointers_I.h"
//#include "XMPAssetManagement/Interfaces/ITrack_I.h"
#include "XMPCommon/Interfaces/IUTF8String_I.h"


#define TXMP_STRING_TYPE std::string 
// Ensure XMP templates are instantiated
#include "public/include/XMP.incl_cpp"

// Provide access to the API
#include "public/include/XMP.hpp"
#include "XMPCore/source/XMPDocOps-Utils.hpp"
#include "XMPCore/Interfaces/IStructureNode_I.h"
#include "source/UnicodeInlines.incl_cpp"
#include "TXMPDocOps.hpp"
#include "TXMPUtils.hpp"

namespace AdobeXMPAM_Int {

	using namespace AdobeXMPCore;
	using namespace AdobeXMPCommon;
	using namespace AdobeXMPCore_Int;
	const sizet kXMP_NS_DM_LEN = strlen(kXMP_NS_DM);
	static const XMP_VarString markerGUID = "marker_guid";

	static XMP_StringPtr kExtension = "extension";
	static XMP_StringPtr kExtensions = "extensions";
	static XMP_StringPtr kGUID = "guid";
	static XMP_StringPtr kTarget = "target";
	static XMP_StringPtr kSpeaker = "speaker";
	static XMP_StringPtr kProbability = "probability";
	static XMP_StringPtr kMarkerName = "name";
	static XMP_StringPtr kDuration = "duration";
	static XMP_StringPtr kCuePointParams = "cuePointParams";
	static XMP_StringPtr kStartTime = "startTime";
	static XMP_StringPtr kLocation = "location";
	static XMP_StringPtr kMarker = "marker";
	static XMP_StringPtr kComment = "comment";
	static XMP_StringPtr kType = "type";
	static XMP_StringPtr kCuePointType = "cuePointType";
	static XMP_StringPtr kKey = "key";
	static XMP_StringPtr kValue = "value";
	static XMP_StringPtr kTrackName = "trackName";
	static XMP_StringPtr kMarkers = "markers";
	static XMP_StringPtr kFrameRate = "frameRate";
	static XMP_StringPtr kTrackType = "trackType";
	static XMP_StringPtr kTracks = "tracks";


	bool IsStringPresent(const char * str, const spcIUTF8StringList_I  & strList);
	bool DoesMarkerMatchTimeCriteria(const spIMarker_I & marker, uint64 start, uint64 count, ITrack::eTrackTimeBoundary timeBoundary);

	bool GetSimpleValue(spINode &node, std::string &value) {

		if (!node || node->GetNodeType() != INode::kNTSimple) return false;
		spcIUTF8String nodeValue = node->ConvertToSimpleNode()->GetValue();
		if (nodeValue) {
			value = nodeValue->c_str();
			return true;
		}
		return false;


	}
	void PopulateCuePointParams(spINode cuePointParams, spIMarker_I & inmarker) {

		if (cuePointParams && cuePointParams->GetNodeType() == INode::kNTArray) {

			spIArrayNode cuePointParamsNode = (cuePointParams)->ConvertToArrayNode();
			for (auto cuePointIter = cuePointParamsNode->Iterator(); cuePointIter; cuePointIter = cuePointIter->Next()) {

				spINode singleCuePointParam = cuePointIter->GetNode();
				if (singleCuePointParam && singleCuePointParam->GetNodeType() == INode::kNTStructure) {

					spIStructureNode singleCuePointParamStructNode = singleCuePointParam->ConvertToStructureNode();
					spINode keyNode = singleCuePointParamStructNode->GetSimpleNode(kXMP_NS_DM, kXMP_NS_DM_LEN,kKey, 3);
					spINode valueNode = singleCuePointParamStructNode->GetSimpleNode(kXMP_NS_DM, kXMP_NS_DM_LEN, kValue, 5);
					std::string keyNodeValue, valueNodeValue;
					if (GetSimpleValue(keyNode, keyNodeValue) && GetSimpleValue(valueNode, valueNodeValue)) {

						inmarker->AddCuePointParam_I(keyNodeValue.c_str(), keyNodeValue.length(), valueNodeValue.c_str(), valueNodeValue.length());
					}

				}
			}
		}
	}
	
	


	void TemporalAssetManagerImpl::PopulateSingleMarker(spIStructureNode &markerNode, spITrack_I & parentTrack ) {
	
		spIMarker_I marker = IMarker_I::CreateMarker_I("", 0, 0);		 
		std::string simpleValue  ;
		spINode name = markerNode->GetSimpleNode(kXMP_NS_DM, kXMP_NS_DM_LEN, kMarkerName, strlen(kMarkerName));
		if (GetSimpleValue(name, simpleValue)) {
			marker->SetName_I(simpleValue.c_str(), simpleValue.length());
		}
		
		spINode markerType = markerNode->GetSimpleNode(kXMP_NS_DM, kXMP_NS_DM_LEN, kType, strlen(kType));
		if ( GetSimpleValue(markerType, simpleValue) ) {
			std::vector<IMarker::eMarkerType> markerTypes;
			std::vector<std::string> markerTypeNames;
			if ( TemporalAssetUtilities::CovertCommaSeparatedType(simpleValue, markerTypes, markerTypeNames)) {
				size_t typeNameIter = 0;
				for (size_t typeIter = 0, typeIterLim = markerTypes.size(); typeIter < typeIterLim && typeNameIter < typeIterLim; ++typeIter) {

					if (markerTypes[typeIter] == IMarker::kMTUserDefinedMarker && typeNameIter < markerTypeNames.size()) {
                        
						marker->AppendCustomType_I(markerTypeNames[typeNameIter].c_str(), markerTypeNames[typeNameIter].length());
                        ++typeNameIter;
					}
					else {
						
						marker->SetTypes_I(markerTypes[typeIter]);
					}
				}
			}
		}
		//3
		spINode comment = markerNode->GetSimpleNode(kXMP_NS_DM, kXMP_NS_DM_LEN, kComment, strlen(kComment));
		if (GetSimpleValue(comment, simpleValue)) {
			marker->SetComment_I(simpleValue.c_str(), simpleValue.length());
		}
		//4
		/*TODO read cue point params Array node*/

		
		spINode cuePointType = markerNode->GetSimpleNode(kXMP_NS_DM, kXMP_NS_DM_LEN, kCuePointType, strlen(kCuePointType));
		if (GetSimpleValue(cuePointType, simpleValue)) {
			
			if ( !strcmp(simpleValue.c_str(), "event") ) {
				marker->SetTypes_I(IMarker::kMTFLVCuePoint_Event);
			}
			else if(!strcmp(simpleValue.c_str(), "navigation")) {

				marker->SetTypes_I(IMarker::kMTFLVCuePoint_Navigation);
			}
		}
		/*TODO read duration*/
		//7
		spINode target = markerNode->GetSimpleNode(kXMP_NS_DM, kXMP_NS_DM_LEN, kTarget, strlen(kTarget));
		if (GetSimpleValue(cuePointType, simpleValue)) {
			marker->SetTarget_I(simpleValue.c_str(), simpleValue.length());
		}
		//6
		spINode speaker = markerNode->GetNode(kXMP_NS_DM, kXMP_NS_DM_LEN, kSpeaker, strlen(kSpeaker));
		if (GetSimpleValue(cuePointType, simpleValue)) {
			marker->SetSpeaker_I(simpleValue.c_str(), simpleValue.length());
		}
		//5
		spINode probability = markerNode->GetNode(kXMP_NS_DM, kXMP_NS_DM_LEN, kProbability, strlen(kProbability));
		if (GetSimpleValue(cuePointType, simpleValue)) {
            marker->SetProbability_I(TemporalAssetUtilities::ConvertStringToDouble(simpleValue));
		}

		spINode location = markerNode->GetNode(kXMP_NS_DM, kXMP_NS_DM_LEN, kLocation, strlen(kLocation));
		if (GetSimpleValue(location, simpleValue)) {
			marker->SetLocation_I(simpleValue.c_str(), simpleValue.length());
		}

		uint64 startFrame = 0 , countFrames = UINT64_MAX;
		spINode time = markerNode->GetNode(kXMP_NS_DM, kXMP_NS_DM_LEN, kStartTime, strlen(kStartTime));
		if (GetSimpleValue(time, simpleValue)) {
			uint64 numerator = 1, denominator = 1, count = 0;
			bool isMax = false;
			TemporalAssetUtilities::ParseFrameCountInStr(simpleValue.c_str(), simpleValue.length(), count, numerator, denominator, isMax);
			if (isMax) count = UINT64_MAX;
			else {
				marker->SetFrameRateForStartFrameCount_I(numerator, denominator);
				startFrame = count;
				//todo ADD frame coutn
			}

		}
		spINode duration = markerNode->GetNode(kXMP_NS_DM, kXMP_NS_DM_LEN, kDuration, strlen(kDuration));
		if (GetSimpleValue(duration, simpleValue)) {
			uint64 numerator = 1, denominator = 1, count = 0;
			bool isMax = false;
		
			TemporalAssetUtilities::ParseFrameCountInStr(simpleValue.c_str(), simpleValue.length(), count, numerator, denominator, isMax);
			if (isMax) count = UINT64_MAX;
			else {
				marker->SetFrameRateForCountOfFrames_I(numerator, denominator);
				countFrames = count;
				//marker->SetFrameCountDuration_I(numerator, denominator);
				//todo add frame count
			}

		}
		marker->SetFrameCountSpan_I(startFrame, countFrames);
		spINode cuePointParams = markerNode->GetNode(kXMP_NS_DM, kXMP_NS_DM_LEN, kCuePointParams, strlen(kCuePointParams));
		PopulateCuePointParams(cuePointParams, marker);

		spINode guid = markerNode->GetNode(kXMP_NS_DM, kXMP_NS_DM_LEN, kGUID, strlen(kGUID));
		if (GetSimpleValue(guid, simpleValue)) {
			marker->SetGUID_I(simpleValue.c_str());
		}
		spINode extensions = markerNode->GetNode(kXMP_NS_DM, kXMP_NS_DM_LEN, kExtensions, strlen(kExtensions));
		if (extensions && extensions->GetNodeType() == INode::kNTStructure) {
			marker->SetExtension_I(extensions);
		}
		parentTrack->AddMarker_I(marker);
	}

	void TemporalAssetManagerImpl::PopulateSingleTrack(spIStructureNode & trackNode) {
		// 4 things
		// name, type, framerate, markers
		
		spINode trackType = trackNode->GetNode(kXMP_NS_DM, kXMP_NS_DM_LEN, kTrackType, strlen(kTrackType));
		spINode trackFrameRate = trackNode->GetNode(kXMP_NS_DM, kXMP_NS_DM_LEN, kFrameRate, strlen(kFrameRate));
		std::string simpleValue;
		

		spITrack_I newTrack = ITrack_I::CreateTrack_I("", 0, 0);

		if (GetSimpleValue(trackType, simpleValue)) {
			std::vector<IMarker::eMarkerType> markerTypes;
			std::vector<std::string> markerTypeNames;
			if (TemporalAssetUtilities::CovertCommaSeparatedType(simpleValue, markerTypes, markerTypeNames)) {
				size_t typeNameIter = 0;
				for (size_t typeIter = 0, typeIterLim = markerTypes.size(); typeIter < typeIterLim; ++typeIter) {

					if (markerTypes[typeIter] == IMarker::kMTUserDefinedMarker && typeNameIter < markerTypeNames.size()) {

						newTrack->AppendCustomType_I(markerTypeNames[typeNameIter].c_str(), markerTypeNames[typeNameIter].length());

						typeNameIter++;
					}
					else {

						newTrack->SetTypes_I(markerTypes[typeIter]);
					}
				}
			}
		}
		// GetTrackType from string and populate it into the marker
		spINode markers = trackNode->GetNode(kXMP_NS_DM, kXMP_NS_DM_LEN, kMarkers, strlen(kMarkers));
		if (markers && markers->GetNodeType() == INode::kNTArray) {
			spIArrayNode markerArray = markers->ConvertToArrayNode();
			for (size_t markerIter = (size_t)1, markerIterLim = markerArray->ChildCount(); markerIter <= markerIterLim; markerIter++) {

				spINode singleMarker = markerArray->GetNodeAtIndex(markerIter);
				if (singleMarker && singleMarker->GetNodeType() == INode::kNTStructure) {

					spIStructureNode singleMarkerStructure = singleMarker->ConvertToStructureNode();
					PopulateSingleMarker(singleMarkerStructure, newTrack);					
				}
			}
		}

		if (GetSimpleValue(trackFrameRate, simpleValue)) {
			uint64 numerator = 1, denominator = 1;
			TemporalAssetUtilities::ParseFrameRateInStr(simpleValue.c_str(), simpleValue.length(), numerator, denominator);

			newTrack->SetFrameRate_I(numerator, denominator);
			//todo ADD frame coutn
		}

		mTracks.push_back(dynamic_pointer_cast<ITrack_I>(newTrack));
	}

	void TemporalAssetManagerImpl::PopulateTracks() {
		spINode trackRootNode = mTemporalMetdadata->GetNode(kXMP_NS_DM, kXMP_NS_DM_LEN, kTracks, strlen(kTracks));
		if (!trackRootNode || trackRootNode->GetNodeType() != INode::kNTArray) return;
		spIArrayNode trackRootArrayNode = trackRootNode->ConvertToArrayNode();
		// TODO whether this check should be this strict
		if (trackRootArrayNode->GetArrayForm() != IArrayNode::kAFOrdered) return;
		for (size_t trackIdx = 1, trackIdxLim = trackRootArrayNode->ChildCount(); trackIdx <= trackIdxLim; ++trackIdx) {
			spINode singleTrackNode = trackRootArrayNode->GetNodeAtIndex(trackIdx);
			if (!singleTrackNode || singleTrackNode->GetNodeType() != INode::kNTStructure) continue;
			spIStructureNode singleTrackStuctNode = (singleTrackNode)->ConvertToStructureNode();
			PopulateSingleTrack(singleTrackStuctNode);
		}
	}

	void TemporalAssetManagerImpl::InitializeContents() {

		if (!mTemporalMetdadata) return;
		mRootNode = mTemporalMetdadata->GetArrayNode(kXMP_NS_DM, kXMP_NS_DM_LEN, kTracks, strlen(kTracks));
		if (!mRootNode) {
			mRootNode = IArrayNode::CreateOrderedArrayNode(kXMP_NS_DM, kXMP_NS_DM_LEN, kTracks, strlen(kTracks));
			mTemporalMetdadata->AppendNode(mRootNode);
		}
		PopulateTracks();
	}


	TemporalAssetManagerImpl::TemporalAssetManagerImpl(const spIMetadata & temporalAssetMetadata)
		try
	{
		mTemporalMetdadata = temporalAssetMetadata;
		InitializeContents();
		
	}

	

	catch (...) {
		/*NOTIFY_ERROR(IError_v1::kDomainGeneral, IError_v1::kGeneralCodeParametersNotAsExpected,
			"Parameters not as expected by TemporalAssetManagerImpl::TemporalAssetImpl",
			IError_v1::kSeverityOperationFatal, true, (void *)temporalAssetMetadata.get(),
			true, static_cast<uint32>(multiThreadingSupport));*/
	}


	AdobeXMPAM_Int::spITrack TemporalAssetManagerImpl::AddTrack_I(spITrack track) {
	
		spIStructureNode serializedMarker;
		track->GetITrack_I()->Serialize_I(serializedMarker);
		try {


			if (serializedMarker) {
				mRootNode->AppendNode(serializedMarker);
				spITrack_I trackInternal = MakeUncheckedSharedPointer <ITrack_I>(track->GetITrack_I() , __FILE__, __LINE__, false);
				mTracks.push_back(trackInternal);
			}
		}
		catch (...) {

			return spITrack();
		}
		//TO DO pack it as a structure node
	//	mRootNode->AppendNode(track-)
		return track;
	}

	AdobeXMPAM_Int::spITrackList_I TemporalAssetManagerImpl::GetAllTracks_I() const {

		spITrackList_I matchedTrackList(new ITrackList_I());

		for ( auto cIt = mTracks.begin(); cIt != mTracks.end(); ++cIt) {
			matchedTrackList->push_back(*cIt);
		}
		return matchedTrackList;
	}

	AdobeXMPAM_Int::spITrackList_I TemporalAssetManagerImpl::RemoveAllTracks_I() {

		spITrackList_I matchedTrackList(new ITrackList_I());

		for (auto cIt = mTracks.begin(); cIt != mTracks.end(); ++cIt) {
			matchedTrackList->push_back(*cIt);
		}

		mTracks.clear();
		mRootNode->Clear();
		return matchedTrackList;
	}

	AdobeXMPAM_Int::spIMarkerList_I TemporalAssetManagerImpl::GetMarkersByName_I(const char * markerName, sizet markerNameLen) const {

		if (!markerName)  {

			/*	NOTIFY_WARNING(IError_base::kDomainGeneral, IError_base::kGeneralCodeParametersNotAsExpected,
					"Parameters to IXMPNameSpacePrefixMap::AddOrReplacePair() are not as expected",
					true, (void *)markerName,
					markerName, markerName);*/

			return spIMarkerList_I();
		}
			
		spIMarkerList_I matchedMarkerList(new IMarkerList_I());

		for (auto cIt = mTracks.begin(); cIt != mTracks.end(); ++cIt) {
			auto track = *cIt;

			spIMarkerList_I markers = track->GetAllMarkers_I();

			for ( auto mIt = markers->begin(); mIt != markers->end(); mIt++) {

				auto marker = *mIt;
				if (!strncmp(marker->GetName_I()->c_str(), markerName, markerNameLen)) {

					matchedMarkerList->push_back(marker);
				}
			}			
		}

		return matchedMarkerList;
	}


	spITemporalAssetManager_I ITemporalAssetManager_I::CreateTemporalAssetManager_I(const spIMetadata & metadata)
	{
		return MakeUncheckedSharedPointer(
			new TemporalAssetManagerImpl(metadata)
			, __FILE__, __LINE__, true);
	}

	AdobeXMPAM_Int::spIMarkerList_I TemporalAssetManagerImpl::RemoveMarkers_I(uint64 markerTypes /*= UINT64_MAX*/, bool matchAllCriteria /*= false*/, const std::vector<std::pair<const char *, sizet> > &markerTypeName /*= std::vector<const char *>()*/) {


		spIMarkerList_I matchedMarkerList(new IMarkerList_I());

		if (matchAllCriteria) {

			for (auto track : mTracks) {

				auto markerList = track->GetAllMarkers_I();
				size_t markerCount = markerList->size();
				for (int idx = markerCount - 1; idx >= 0; idx--) {

					spIMarker_I marker = markerList->at(idx);
					if (!marker->HasTypes(markerTypes) && markerTypes != UINT64_MAX) continue;
					auto customTypes = marker->GetCustomTypes_I();

					bool notFound = false;
					for (size_t idx2 = 0; idx2 < markerTypeName.size(); ++idx2) {
						//TODO length
						if (!IsStringPresent(markerTypeName[idx2].first, customTypes)) {

							notFound = true;
							break;
						}
					}
					if (!notFound) {
						matchedMarkerList->push_back(marker);
						track->RemoveMarker_I(idx + 1);
						
					}

				}
			}
			
		}
		else {

			for (auto track : mTracks) {

				auto markerList = track->GetAllMarkers_I();
				size_t markerCount = markerList->size();
				for (int idx = markerCount - 1; idx >= 0; idx--) {
					spIMarker_I marker = markerList->at(idx);
					bool found = false;
					uint64 markerType = marker->GetTypes_I();
					if (markerType | markerTypes || (markerTypes == UINT64_MAX)) {
						found = true;

					}
					if (!found) {

						auto customTypes = marker->GetCustomTypes_I();
						for (size_t idx2 = 0; idx2 < markerTypeName.size(); ++idx2) {
							//TODO length
							if (IsStringPresent(markerTypeName[idx2].first, customTypes)) {

								found = true;
								break;
							}
						}
					}


					if (found) {

						matchedMarkerList->push_back(marker);
						track->RemoveMarker_I(idx + 1);
					}

				}

			}
		}
		return matchedMarkerList;
	}

	AdobeXMPAM_Int::spIMarkerList_I TemporalAssetManagerImpl::GetMarkers_I(uint64 markerTypes /*= UINT64_MAX*/, bool matchAllCriteria /*= false*/, const std::vector<std::pair<const char *, sizet> > &markerTypeName /*= std::vector<const char *>()*/) const {

		spIMarkerList_I matchedMarkerList(new IMarkerList_I());
		if (matchAllCriteria) {

			
			for (auto track : mTracks) {

				auto markerList = track->GetAllMarkers_I();
				size_t markerCount = markerList->size();
				for (size_t idx = 0; idx < markerCount; ++idx) {

					auto marker = markerList->at(idx);
					if (!marker->HasTypes(markerTypes) && markerTypes != UINT64_MAX) continue;
					auto customTypes = marker->GetCustomTypes_I();

					bool notFound = false;
					for (size_t idx = 0; idx < markerTypeName.size(); ++idx) {
						//TODO length
						if (!IsStringPresent(markerTypeName[idx].first, customTypes)) {

							notFound = true;
							break;
						}
					}
					if (!notFound) {

						matchedMarkerList->push_back(marker);
					}

				}
			}
			
		}
		else {

			for (auto track : mTracks) {
				auto markerList = track->GetAllMarkers_I();
				size_t markerCount = markerList->size();
				for (size_t idx = 0; idx < markerCount; ++idx) {
					auto marker = markerList->at(idx);
					bool found = false;
					uint64 markerType = marker->GetTypes_I();
					if ((markerType | markerTypes) || (markerTypes == UINT64_MAX)) {
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


					if (found) {

						matchedMarkerList->push_back(marker);
					}

				}
			}
		
		}
		return matchedMarkerList;
	}

	AdobeXMPAM_Int::spITrackList_I TemporalAssetManagerImpl::RemoveTracks_I(uint64 markerTypes /*= UINT64_MAX*/, bool matchAllCriteria /*= false*/, const std::vector<std::pair<const char *, sizet> > &markerTypeName /*= std::vector<const char *>()*/) {

		spITrackList_I matchedMarkerList(new ITrackList_I());
		if (matchAllCriteria) {


			for (int idx = mTracks.size() - 1; idx >=0;idx--) {

				spITrack_I track = mTracks[idx];
				if (!track->HasTypes(markerTypes) && markerTypes != UINT64_MAX) continue;

				auto customTypes = track->GetCustomTypes_I();

				bool notFound = false;
				for (size_t idx2 = 0; idx2 < markerTypeName.size(); ++idx2) {
					//TODO length
					if (!IsStringPresent(markerTypeName[idx2].first, customTypes)) {

						notFound = true;
						break;
					}
				}
				if (!notFound) {
					matchedMarkerList->push_back(track);
					mTracks.erase(mTracks.begin(), mTracks.begin() + idx);
					mRootNode->RemoveNodeAtIndex(idx + 1);
				}
	
			}

		}
		else {


			for (int idx = mTracks.size() - 1; idx >= 0; idx--) {

				spITrack_I track = mTracks[idx];
				uint64 markerType = track->GetTypes_I();
				bool found = false;
				if (( markerType | markerTypes) || (markerTypes == UINT64_MAX)) {
					found = true;
				}

				auto customTypes = track->GetCustomTypes_I();

				
				for (size_t idx2 = 0; idx2 < markerTypeName.size(); ++idx2) {
					//TODO length
					if (IsStringPresent(markerTypeName[idx2].first, customTypes)) {

						found = true;
						break;
					}
				}
				if (found) {
					matchedMarkerList->push_back(track);
					mTracks.erase(mTracks.begin(), mTracks.begin() + idx);
					mRootNode->RemoveNodeAtIndex(idx + 1);
				}

			}
		

		}
		return matchedMarkerList;
		
	}

	AdobeXMPAM_Int::spITrackList_I TemporalAssetManagerImpl::GetTracks_I(uint64 markerTypes /*= UINT64_MAX*/, bool matchAllCriteria /*= false*/, const std::vector<std::pair<const char *, sizet> > &markerTypeName /*= std::vector<const char *>()*/) const {

		spITrackList_I matchedMarkerList(new ITrackList_I());
		if (matchAllCriteria) {


			for (int idx = mTracks.size() - 1; idx >=0;idx--) {

				spITrack_I track = mTracks[idx];
				if (!track->HasTypes(markerTypes) && markerTypes != UINT64_MAX) continue;

				auto customTypes = track->GetCustomTypes_I();

				bool notFound = false;
				for (size_t idx2 = 0; idx2 < markerTypeName.size(); ++idx2) {
					//TODO ength
					if (!IsStringPresent(markerTypeName[idx2].first, customTypes)) {

						notFound = true;
						break;
					}
				}
				if (!notFound) {
					matchedMarkerList->push_back(track);
				}
	
			}

		}
		else {


			for (int idx = mTracks.size() - 1; idx >= 0; idx--) {

				spITrack_I track = mTracks[idx];
				uint64 markerType = track->GetTypes_I();
				bool found = false;
				if (( markerType | markerTypes) || (markerTypes == UINT64_MAX)) {
					found = true;
				}

				auto customTypes = track->GetCustomTypes_I();

				
				for (size_t idx2 = 0; idx2 < markerTypeName.size(); ++idx2) {
					//TODO length
					if (IsStringPresent(markerTypeName[idx2].first, customTypes)) {

						found = true;
						break;
					}
				}
				if (found) {
					matchedMarkerList->push_back(track);
				}

			}
	

		}
		return matchedMarkerList;
	}

	spIMarkerList_I TemporalAssetManagerImpl::GetMarkersInTimeRange_I(uint64 startFrameCount /*= 0*/, uint64 endFrameCount /*= UINT64_MAX*/, ITrack::eTrackTimeBoundary timeBoundary /*= ITrack::kTTBothStartTimeAndEndTimeIncluded*/, uint64 markerTypes /*= UINT64_MAX*/, bool matchAllCriteria /*= false*/, const std::vector<std::pair<const char *, sizet> > &markerTypeName /*= std::vector<const char *>()*/) const {

		spIMarkerList_I matchedMarkerList(new IMarkerList_I());
		if (matchAllCriteria) {


			for (auto track : mTracks) {

				auto markerList = track->GetAllMarkers_I();
				size_t markerCount = markerList->size();
				for (size_t idx = 0; idx < markerCount; ++idx) {

					auto marker = markerList->at(idx);
					if (!marker->HasTypes(markerTypes) && markerTypes != UINT64_MAX) continue;
					if (!DoesMarkerMatchTimeCriteria(marker, startFrameCount, endFrameCount, timeBoundary)) continue;
					auto customTypes = marker->GetCustomTypes_I();

					bool notFound = false;
					for (size_t idx = 0; idx < markerTypeName.size(); ++idx) {
						//TODO
						if (!IsStringPresent(markerTypeName[idx].first, customTypes)) {

							notFound = true;
							break;
						}
					}
					if (!notFound) {

						matchedMarkerList->push_back(marker);
					}

				}
			}

		}
		else {

			for (auto track : mTracks) {
				auto markerList = track->GetAllMarkers_I();
				size_t markerCount = markerList->size();
				for (size_t idx = 0; idx < markerCount; ++idx) {
					auto marker = markerList->at(idx);
					bool found = false;
					uint64 markerType = marker->GetTypes_I();
					if ((markerType & markerTypes) || (markerTypes == UINT64_MAX)) {
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
			}

		}
		return matchedMarkerList;
	}


	
	
}


namespace AdobeXMPAM {
	using namespace AdobeXMPAM_Int;
	spITemporalAssetManager ITemporalAssetManager_v1::MakeShared(pITemporalAssetManager_base ptr) {
		pITemporalAssetManager p = ITemporalAssetManager::GetInterfaceVersion() > 1 ? ptr->GetInterfacePointer< ITemporalAssetManager >() : ptr;
		return MakeUncheckedSharedPointer(p, __FILE__, __LINE__, false);
	}
}
