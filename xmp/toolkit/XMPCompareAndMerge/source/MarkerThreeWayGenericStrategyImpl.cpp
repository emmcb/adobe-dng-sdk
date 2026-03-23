// =================================================================================================
// Copyright Adobe
// Copyright 2014 Adobe
// All Rights Reserved
//
// NOTICE:  Adobe permits you to use, modify, and distribute this file in accordance with the terms
// of the Adobe license agreement accompanying it. If you have received this file from a source other 
// than Adobe, then your use, modification, or distribution of it requires the prior written permission
// of Adobe.
// =================================================================================================



// XMPCore interfaces
#include <string>
#define TXMP_STRING_TYPE std::string
#include "XMP.hpp"

#include <algorithm>
#include <map>
#include <string>
#include <iostream>
#include <fstream>
#include <iostream>
#include "XMPCompareAndMerge/ImplHeaders/MarkerThreeWayGenericStrategyImpl.h"
#include "XMPCore/XMPCoreFwdDeclarations.h"
#include "XMPCore/Interfaces/ICoreObjectFactory.h"
#include "XMPCore/Interfaces/INode.h"
#include "XMPCore/Interfaces/IPath.h"
#include "XMPCore/Interfaces/IPathSegment.h"
#include "XMPCore/Interfaces/INodeIterator.h"
#include "XMPCore/Interfaces/IMetadata.h"
#include "XMPCore/Interfaces/ISimpleNode.h"
#include "XMPCore/Interfaces/IArrayNode.h"
#include "XMPCore/XMPCoreErrorCodes.h"

#include "XMPCommon/Interfaces/IUTF8String_I.h"
#include "XMPCommon/Interfaces/IErrorNotifier_I.h"
#include "XMPCommon/Utilities/TSmartPointers_I.h"
#define  STATIC_SAFE_API
#include "source/SafeStringAPIs.h"


namespace AdobeXMPCompareAndMerge_Int {

	using namespace AdobeXMPCommon;
	using namespace AdobeXMPCore;
	using namespace AdobeXMPCompareAndMerge;

	static const char markersPath[] = kXMP_NS_DM ":Tracks";

	spIThreeWayGenericStrategy MarkerThreeWayGenericStrategyImpl::CreateMarkerThreeWayGenericStrategy() {
		return MakeUncheckedSharedPointer(new MarkerThreeWayGenericStrategyImpl(), __FILE__, __LINE__, true);
	}

	ThreeWayGenericStrategyImpl * APICALL MarkerThreeWayGenericStrategyImpl::createAndCloneContents() const {
		return new MarkerThreeWayGenericStrategyImpl();
	}

	typedef struct
	{
		std::string trackType;
		std::string trackName;
		std::string frameRate;
	} Track; //currently only assuming these there will be present except Markers

	typedef struct
	{
		Track trackInfo;
		XMPDMO_MarkerInfo marker;
	} XMPDMO_MarkerInfo_ex; // Track info of the marker and the info of marker itself

	typedef struct
	{
		Track trackInfo;
		std::vector<XMPDMO_MarkerInfo> markers;
	} Markers_In_Track;

	class MergeMarkers
	{
	public:
		MergeMarkers(spcINode base, spcINode latest, spcINode current);
		bool Merge();
		spIArrayNode getMergedNode();
	private:
		typedef std::map<std::string, XMPDMO_MarkerInfo_ex > marker_map; // <guid -> marker extended map>
		typedef std::map<std::string, std::vector<XMPDMO_MarkerInfo> > resolved_marker_map; // <trackType_trackname_frameRate -> vector of marker info>
		typedef std::map<std::string, Markers_In_Track > trackToMarkersMap;

		bool isInitialized;
		marker_map m_baseMap, m_headMap, m_currentMap;
		resolved_marker_map resolvedMap;
		spIArrayNode m_MergedNode;

		trackToMarkersMap m_resolvedMap;

		void getMarkers(spcINode node, std::vector<XMPDMO_MarkerInfo_ex> &marker);
		std::string getGUIDFromMarkerInfo(XMPDMO_MarkerInfo_ex marker);
		bool createMarkerMap(std::map<std::string, XMPDMO_MarkerInfo_ex > *map, spcINode node);
		bool compareMarkers(XMPDMO_MarkerInfo_ex, XMPDMO_MarkerInfo_ex);
		XMPDMO_MarkerInfo_ex mergeSingleMarker(XMPDMO_MarkerInfo_ex, XMPDMO_MarkerInfo_ex, XMPDMO_MarkerInfo_ex);
		const char* mergeMarkerField(const char*, const char*, const char*);
		bool compareTrackData(XMPDMO_MarkerInfo_ex, XMPDMO_MarkerInfo_ex);
		bool compareMarkerData(XMPDMO_MarkerInfo_ex, XMPDMO_MarkerInfo_ex);
		void addtoResolve(XMPDMO_MarkerInfo_ex markerToAdd);
		SXMPMeta loadPacket(std::string fileName, SXMPMeta *meta);
		std::string generateTrackKey(XMPDMO_MarkerInfo_ex marker);
		spIArrayNode createFinalNode();
		bool checkTracks(spcINode node);
		void addMarkerToStruct(XMPDMO_MarkerInfo markerItemStruct, spIStructureNode & markerStruct);

		/*
		** function name
		**	[Description] .....
		**
		** [in]
		**
		** []
		*/
		void fillMarkerVector(XMPDMO_MarkerInfo_ex *markerVector, spcINode markerNode);
		void fillCuePointArray(XMPDMO_CuePointInfo *cuePoint, spcINode cuePointArrayItem);

	};

	bool MergeMarkers::Merge()
	{
		//iterater through all the current markers, see corresponding headMarkers
		for (marker_map::iterator iter = m_currentMap.begin(); iter != m_currentMap.end(); iter++)
		{
			//see if a marker with this guid is there in the head
			marker_map::iterator head_iter = m_headMap.find(iter->first);
			if (head_iter != m_headMap.end()) //the marker is there in both head and current
			{
				//compare these two markers. If same then resolved else need to check the base
				if (compareMarkers(iter->second, head_iter->second))
				{
					//both head and current have same value of marker. So this is resolved				
					addtoResolve(iter->second);
					//knock this marker from all the head maps
					m_headMap.erase(iter->first);
				}
				else //head and current have different value
				{
					//check this in base. If not there in base then conflict. Else need to check the value in base
					marker_map::iterator base_iter = m_baseMap.find(iter->first);
					if (base_iter != m_baseMap.end())
					{
						//now check the value in base. If base==current keep head
						//                             If base==head keep current
						//							   else possible conflict	
						if (compareMarkers(iter->second, base_iter->second))
						{
							//base and current are equal. Resolved. Keep head						
							addtoResolve(head_iter->second);
							//knock of this marker from head map
							m_headMap.erase(head_iter->first);
						}
						else if (compareMarkers(head_iter->second, base_iter->second))
						{
							//base and head are equal. Resolved. keep current						
							addtoResolve(iter->second);
							//knock of this marker from head map
							m_headMap.erase(head_iter->first);
						}
						else //all three are different
						{
							//conflict possibility, try merging the markers.
							try{
								XMPDMO_MarkerInfo_ex mergedMarker = mergeSingleMarker(base_iter->second, head_iter->second, iter->second);
								//Markers can be merged. conflict resolved.
								addtoResolve(mergedMarker);
								//knock of this marker from head map
								m_headMap.erase(head_iter->first);
							}
							catch (...){
								//Merging of markers is not possible. this will go as conflict.
								return 0;
							}
						}
					}
					else
					{
						//Conflict
						return 0;
					}
				}
			}
			else //marker is not there in head but its there in current
			{
				//check if this marker is there in base. If no then resolved, else check the value in base
				marker_map::iterator base_iter = m_baseMap.find(iter->first);
				if (base_iter != m_baseMap.end())
				{
					//this marker is there in base also. Compare with base. If value is same as base then kock it off, else conflict
					if (compareMarkers(iter->second, base_iter->second))
					{
						//Resolved. Knock this marker off					
					}
					else
					{
						//conflict. Base and current are diff and missing in head.	
						return 0;
					}
				}
				else
				{
					//its not there in base also. So resolved
					addtoResolve(iter->second);
				}
			}
		}
		//now lets see whats left in the head map
		//the entries left are gauranteed to be not there in the current map for sure.
		//We will just compare these left markers with base map.
		//if they exist in base and are equal then resolved
		//if they exist in base and is different, conflict
		//if its not there in base, resolved, add this to resolved list.
		for (marker_map::iterator iter = m_headMap.begin(); iter != m_headMap.end(); iter++)
		{
			marker_map::iterator base_iter = m_baseMap.find(iter->first);
			if (base_iter != m_baseMap.end()) //the marker is there in both head and base
			{
				//compare these two markers. If same then resolved else need to check the base
				if (compareMarkers(iter->second, base_iter->second))
				{
					//both head and base have same value of marker. So this is resolved. This has been delete in current. Nothing to do				
				}
				else //base and head are different, and its been delete from current. So conflict
				{
					return 0;
				}
			}
			else //this marker is not there in base (or current) so its only added in head. Resolved. Add this in the resolved Map 
			{
				addtoResolve(iter->second);
			}
		}

		m_MergedNode = createFinalNode();
		return 1;
	}

	bool MergeMarkers::checkTracks(spcINode node)
	{
		spcIStructureNode trackItem;

		//1. Check if Tracks is there and is an array 
		//2. Check is markers are there in tracks they only have white listed entries
		//3. All markers must have GUID	

		spcIArrayNode tnode;
		tnode = node->ConvertToArrayNode();
		if (!tnode)
			return false;
		if ((strcmp(node->GetNameSpace()->c_str(), kXMP_NS_DM) != 0) && (strcmp(node->GetName()->c_str(), "Tracks") != 0) && (node->ConvertToArrayNode()->GetArrayForm() != IArrayNode::kAFUnordered))
			return false;

		//spcINodeIterator track_Iter = std::dynamic_pointer_cast<const IArrayNode>(node)->Iterator();
		spcINodeIterator track_Iter;
		track_Iter = tnode->Iterator();
		

		while (track_Iter)
		{
			spcINode tempTrackNode = track_Iter->GetNode();
			trackItem = (tempTrackNode)->ConvertToStructureNode();
			if (!trackItem)
				return false;
			if (trackItem->ChildCount() > 4)		//Every track item can have a maximum of 4 properties
				return false;
			if (!trackItem->GetNode(kXMP_NS_DM, AdobeXMPCommon::npos, "frameRate", AdobeXMPCommon::npos))	//Must exist
				return false;
			if (!trackItem->GetNode(kXMP_NS_DM, AdobeXMPCommon::npos, "trackType", AdobeXMPCommon::npos))	//Must exist
				return false;
			if (!trackItem->GetNode(kXMP_NS_DM, AdobeXMPCommon::npos, "markers", AdobeXMPCommon::npos))		//Must exist
				return false;
			tnode = trackItem->GetArrayNode(kXMP_NS_DM, AdobeXMPCommon::npos, "markers", AdobeXMPCommon::npos);
			if (!tnode)
				return false;
			if (tnode->GetArrayForm() != IArrayNode::kAFOrdered)	//Must be a seq and contain some marker		
				return false;
			if (tnode->ChildCount() <= 0)
				return false;	// Must contain at least 1 marker
			spcINodeIterator marker_Iter = tnode->Iterator();
			while (marker_Iter)
			{
				spcIStructureNode markerItem = marker_Iter->GetStructureNode();
				if (!markerItem)
					return false;
				spcIArrayNode cuePointNode = markerItem->GetArrayNode(kXMP_NS_DM, AdobeXMPCommon::npos, "cuePointParams", AdobeXMPCommon::npos);

				spIPath path = IPath::CreatePath();
				spcIPathSegment segment = IPathSegment::CreatePropertyPathSegment(kXMP_NS_DM, AdobeXMPCommon::npos, "key", AdobeXMPCommon::npos);
				path->AppendPathSegment(segment);

				if (!cuePointNode)
					return false;
				spcINodeIterator cuePoint_Iter = cuePointNode->Iterator();
				int flag = 0;

				while (cuePoint_Iter)
				{
					spcIStructureNode cuePointItem = cuePoint_Iter->GetStructureNode();
					if (!cuePointItem)
						return false;
					spcISimpleNode tSimpleNode;
					tSimpleNode = cuePointItem->GetSimpleNode(kXMP_NS_DM, AdobeXMPCommon::npos, "key", AdobeXMPCommon::npos);
					if (!tSimpleNode)
						return false;
					if (strcmp(tSimpleNode->GetValue()->c_str(), "marker_guid") == 0)
					{
						flag = 1;
						break;
					}
					cuePoint_Iter = cuePoint_Iter->Next();
				}

				if (flag == 0)		//Must contain guid for every marker
					return false;

				marker_Iter = marker_Iter->Next();
			}

			track_Iter = track_Iter->Next();
		}

		return true;
	}

	void MergeMarkers::addMarkerToStruct(XMPDMO_MarkerInfo markerItemStruct, spIStructureNode & markerStruct)
	{
		spISimpleNode newSimpleNode;

		if (markerItemStruct.comment.len != 0)
		{
			newSimpleNode = ISimpleNode::CreateSimpleNode(kXMP_NS_DM, AdobeXMPCommon::npos, "comment", AdobeXMPCommon::npos, markerItemStruct.comment.ptr, markerItemStruct.comment.len);
			markerStruct->InsertNode(newSimpleNode);
		}
		if (markerItemStruct.cuePointType.len != 0)
		{
			newSimpleNode = ISimpleNode::CreateSimpleNode(kXMP_NS_DM, AdobeXMPCommon::npos, "cuePointType", AdobeXMPCommon::npos, markerItemStruct.cuePointType.ptr, markerItemStruct.cuePointType.len);
			markerStruct->InsertNode(newSimpleNode);
		}
		if (markerItemStruct.duration.len != 0)
		{
			newSimpleNode = ISimpleNode::CreateSimpleNode(kXMP_NS_DM, AdobeXMPCommon::npos, "duration", AdobeXMPCommon::npos, markerItemStruct.duration.ptr, markerItemStruct.duration.len);
			markerStruct->InsertNode(newSimpleNode);
		}
		if (markerItemStruct.location.len != 0)
		{
			newSimpleNode = ISimpleNode::CreateSimpleNode(kXMP_NS_DM, AdobeXMPCommon::npos, "location", AdobeXMPCommon::npos, markerItemStruct.location.ptr, markerItemStruct.location.len);
			markerStruct->InsertNode(newSimpleNode);
		}
		if (markerItemStruct.name.len != 0)
		{
			newSimpleNode = ISimpleNode::CreateSimpleNode(kXMP_NS_DM, AdobeXMPCommon::npos, "name", AdobeXMPCommon::npos, markerItemStruct.name.ptr, markerItemStruct.name.len);
			markerStruct->InsertNode(newSimpleNode);
		}
		if (markerItemStruct.probability.len != 0)
		{
			newSimpleNode = ISimpleNode::CreateSimpleNode(kXMP_NS_DM, AdobeXMPCommon::npos, "probability", AdobeXMPCommon::npos, markerItemStruct.probability.ptr, markerItemStruct.probability.len);
			markerStruct->InsertNode(newSimpleNode);
		}
		if (markerItemStruct.speaker.len != 0)
		{
			newSimpleNode = ISimpleNode::CreateSimpleNode(kXMP_NS_DM, AdobeXMPCommon::npos, "speaker", AdobeXMPCommon::npos, markerItemStruct.speaker.ptr, markerItemStruct.speaker.len);
			markerStruct->InsertNode(newSimpleNode);
		}
		if (markerItemStruct.startTime.len != 0)
		{
			newSimpleNode = ISimpleNode::CreateSimpleNode(kXMP_NS_DM, AdobeXMPCommon::npos, "startTime", AdobeXMPCommon::npos, markerItemStruct.startTime.ptr, markerItemStruct.startTime.len);
			markerStruct->InsertNode(newSimpleNode);
		}
		if (markerItemStruct.target.len != 0)
		{
			newSimpleNode = ISimpleNode::CreateSimpleNode(kXMP_NS_DM, AdobeXMPCommon::npos, "target", AdobeXMPCommon::npos,  markerItemStruct.target.ptr, markerItemStruct.target.len);
			markerStruct->InsertNode(newSimpleNode);
		}
		if (markerItemStruct.type.len != 0)
		{
			newSimpleNode = ISimpleNode::CreateSimpleNode(kXMP_NS_DM, AdobeXMPCommon::npos, "type", AdobeXMPCommon::npos, markerItemStruct.type.ptr, markerItemStruct.type.len);
			markerStruct->InsertNode(newSimpleNode);
		}

		if (markerItemStruct.cuePointParams.size() > 0)
		{
			spIArrayNode cuePointArray = IArrayNode::CreateOrderedArrayNode(kXMP_NS_DM, AdobeXMPCommon::npos, "cuePointParams", AdobeXMPCommon::npos);

			for (size_t j = 0; j < markerItemStruct.cuePointParams.size(); j++)
			{
				spIStructureNode cuePointStructureNode = IStructureNode::CreateStructureNode(kXMP_NS_DM, AdobeXMPCommon::npos, "cuePointParams", AdobeXMPCommon::npos);

				spISimpleNode keyNode = ISimpleNode::CreateSimpleNode(kXMP_NS_DM, AdobeXMPCommon::npos, "key", AdobeXMPCommon::npos, markerItemStruct.cuePointParams[j].key.ptr, markerItemStruct.cuePointParams[j].key.len);
				cuePointStructureNode->InsertNode(keyNode);
				spISimpleNode valueNode = ISimpleNode::CreateSimpleNode(kXMP_NS_DM, AdobeXMPCommon::npos, "value", AdobeXMPCommon::npos, markerItemStruct.cuePointParams[j].value.ptr, markerItemStruct.cuePointParams[j].value.len);
				cuePointStructureNode->InsertNode(valueNode);

				cuePointArray->AppendNode(cuePointStructureNode);
			}

			markerStruct->InsertNode(cuePointArray);
		}

	}

	spIArrayNode MergeMarkers::createFinalNode()
	{
		spIArrayNode mergedNode;
		mergedNode = IArrayNode::CreateUnorderedArrayNode(kXMP_NS_DM, AdobeXMPCommon::npos, "Tracks", AdobeXMPCommon::npos);

		std::string nodeName;
		spIStructureNode trackItem;
		spISimpleNode trackInfoItem;
		int markerIndex = 0;
		size_t i, k;

		for (trackToMarkersMap::iterator map_iter = m_resolvedMap.begin(); map_iter != m_resolvedMap.end(); map_iter++, markerIndex++)
		{
			trackItem = IStructureNode::CreateStructureNode(mergedNode->GetNameSpace()->c_str(), mergedNode->GetNameSpace()->size(), mergedNode->GetName()->c_str(), mergedNode->GetName()->size());

			//Set trackName, trackType and frameRate
			if (map_iter->second.trackInfo.trackName.length() != 0)
			{
				trackInfoItem = ISimpleNode::CreateSimpleNode(kXMP_NS_DM, AdobeXMPCommon::npos, "trackName", AdobeXMPCommon::npos, map_iter->second.trackInfo.trackName.c_str(), map_iter->second.trackInfo.trackName.size());
				trackItem->InsertNode(trackInfoItem);
			}
			if (map_iter->second.trackInfo.trackType.length() != 0)
			{
				trackInfoItem = ISimpleNode::CreateSimpleNode(kXMP_NS_DM, AdobeXMPCommon::npos, "trackType", AdobeXMPCommon::npos, map_iter->second.trackInfo.trackType.c_str(), map_iter->second.trackInfo.trackType.size());
				trackItem->InsertNode(trackInfoItem);
			}
			if (map_iter->second.trackInfo.frameRate.length() != 0)
			{
				trackInfoItem = ISimpleNode::CreateSimpleNode(kXMP_NS_DM, AdobeXMPCommon::npos, "frameRate", AdobeXMPCommon::npos, map_iter->second.trackInfo.frameRate.c_str(), map_iter->second.trackInfo.frameRate.size());
				trackItem->InsertNode(trackInfoItem);
			}

			//Set the Markers
			spIArrayNode markerItem = IArrayNode::CreateOrderedArrayNode(kXMP_NS_DM, AdobeXMPCommon::npos, "markers", AdobeXMPCommon::npos);

			for (i = 0; i < map_iter->second.markers.size(); i++)
			{
				for (k = 1; k <= markerItem->ChildCount(); k++)
				{
					int startTimeNewItem = std::atoi(map_iter->second.markers[i].startTime.ptr);
					spcIStructureNode tnode;
					tnode = markerItem->GetStructureNodeAtIndex(k);
					if (!tnode)
						return spIArrayNode();
					spcINode startTimeNode = tnode->GetNode(kXMP_NS_DM, AdobeXMPCommon::npos, "startTime", AdobeXMPCommon::npos);
					spcISimpleNode tSimpleNode;
					tSimpleNode = startTimeNode->ConvertToSimpleNode();
					if (!tSimpleNode)
						return spIArrayNode();
					int startTimeCurrentItem = std::atoi(tSimpleNode->GetValue()->c_str());
					if (startTimeNewItem <= startTimeCurrentItem) break;
				}

				spIStructureNode markerStruct = IStructureNode::CreateStructureNode(markerItem->GetNameSpace()->c_str(), markerItem->GetNameSpace()->size(), markerItem->GetName()->c_str(), markerItem->GetName()->size());
				//Insert the marker item from the vector into the marker struct Node
				addMarkerToStruct(map_iter->second.markers[i], markerStruct);

				markerItem->InsertNodeAtIndex(markerStruct, k);
			}

			trackItem->InsertNode(markerItem);

			mergedNode->AppendNode(trackItem);

		}
		return mergedNode;
	}

	spIArrayNode MergeMarkers::getMergedNode()
	{
		return m_MergedNode;
	}

	std::string MergeMarkers::generateTrackKey(XMPDMO_MarkerInfo_ex marker)
	{
		//generate key string = trkType_trkFrameRate_trkName
		std::string key, tmp;
		tmp = marker.trackInfo.trackType;
		!tmp.empty() ? key += tmp : key += "##";
		tmp.clear();
		tmp = marker.trackInfo.frameRate;
		!tmp.empty() ? key += "_" + tmp : key += tmp + "_##";
		tmp.clear();
		tmp = marker.trackInfo.trackName;
		!tmp.empty() ? key += "_" + tmp : key += tmp + "_##";
		tmp.clear();
		return key;
	}

	void MergeMarkers::addtoResolve(XMPDMO_MarkerInfo_ex markerToAdd)
	{
		//generate key string = trkType_trkFrameRate_trkName
		std::string trackKey = generateTrackKey(markerToAdd);

		trackToMarkersMap::iterator res_iter = m_resolvedMap.find(trackKey);
		if (res_iter == m_resolvedMap.end()) //this key is not there in the map add it
		{
			Markers_In_Track tmpMarker_ex;

			tmpMarker_ex.trackInfo.frameRate = markerToAdd.trackInfo.frameRate;
			tmpMarker_ex.trackInfo.trackName = markerToAdd.trackInfo.trackName;
			tmpMarker_ex.trackInfo.trackType = markerToAdd.trackInfo.trackType;
			tmpMarker_ex.markers.push_back(markerToAdd.marker);

			m_resolvedMap.insert(std::make_pair(trackKey, tmpMarker_ex));
		}
		else //append the vector of the trackType
		{
			res_iter->second.markers.push_back(markerToAdd.marker);
		}
	}

	bool MergeMarkers::compareMarkers(XMPDMO_MarkerInfo_ex first, XMPDMO_MarkerInfo_ex second)
	{
		return (compareTrackData(first, second) &&
			compareMarkerData(first, second)
			);
	}

	const char* MergeMarkers::mergeMarkerField(const char* base, const char* head, const char* current)
	{
		std::string baseString = "", headString = "", currentString = "";

		if (base != NULL)
			baseString = base;

		if (head != NULL)
			headString = head;

		if (current != NULL)
			currentString = current;

		if (baseString.compare(headString) == 0)
		{
			// base and head are same that means current will have latest modifications.
			return current;
		}
		else
		{
			// head contains new changes.
			if (baseString.compare(currentString) == 0)
			{
				// base and current are same that means current has nothing to do with this field. 
				// head will have latest modifications.
				return head;
			}
			else
			{
				if (baseString.compare(currentString) == 0)
				{
					// head and current both are doing same change. both have latest modifications.
					return current;
				}
				else
				{
					// All three are different. Conflict state.
					throw std::string("Conflict");
				}
			}
		}
	}
    
 

    static XMPDMO_StringInfo getXMPString(const char* value)
	{
		XMPDMO_StringInfo newXMPString;
		memset(&newXMPString, 0, sizeof(newXMPString));

		newXMPString.ptr = value;

		if (value == NULL)
			newXMPString.len = 0;
		else
			newXMPString.len = (XMP_StringLen)strnlen_safe(value, AdobeXMPCommon::npos);
		return newXMPString;
	}

	XMPDMO_MarkerInfo_ex MergeMarkers::mergeSingleMarker(XMPDMO_MarkerInfo_ex base, XMPDMO_MarkerInfo_ex head, XMPDMO_MarkerInfo_ex current)
	{
		XMPDMO_MarkerInfo_ex mergedMarker;

		// Merge track data.
		mergedMarker.trackInfo.trackType = mergeMarkerField(base.trackInfo.trackType.c_str(), head.trackInfo.trackType.c_str(), current.trackInfo.trackType.c_str());
		mergedMarker.trackInfo.trackName = mergeMarkerField(base.trackInfo.trackName.c_str(), head.trackInfo.trackName.c_str(), current.trackInfo.trackName.c_str());
		mergedMarker.trackInfo.frameRate = mergeMarkerField(base.trackInfo.frameRate.c_str(), head.trackInfo.frameRate.c_str(), current.trackInfo.frameRate.c_str());

		// Merge marker data.
		mergedMarker.marker.startTime = getXMPString(mergeMarkerField(base.marker.startTime.ptr, head.marker.startTime.ptr, current.marker.startTime.ptr));
		mergedMarker.marker.duration = getXMPString(mergeMarkerField(base.marker.duration.ptr, head.marker.duration.ptr, current.marker.duration.ptr));
		mergedMarker.marker.comment = getXMPString(mergeMarkerField(base.marker.comment.ptr, head.marker.comment.ptr, current.marker.comment.ptr));
		mergedMarker.marker.name = getXMPString(mergeMarkerField(base.marker.name.ptr, head.marker.name.ptr, current.marker.name.ptr));
		mergedMarker.marker.location = getXMPString(mergeMarkerField(base.marker.location.ptr, head.marker.location.ptr, current.marker.location.ptr));
		mergedMarker.marker.target = getXMPString(mergeMarkerField(base.marker.target.ptr, head.marker.target.ptr, current.marker.target.ptr));
		mergedMarker.marker.type = getXMPString(mergeMarkerField(base.marker.type.ptr, head.marker.type.ptr, current.marker.type.ptr));
		mergedMarker.marker.speaker = getXMPString(mergeMarkerField(base.marker.speaker.ptr, head.marker.speaker.ptr, current.marker.speaker.ptr));
		mergedMarker.marker.cuePointType = getXMPString(mergeMarkerField(base.marker.cuePointType.ptr, head.marker.cuePointType.ptr, current.marker.cuePointType.ptr));
		mergedMarker.marker.probability = getXMPString(mergeMarkerField(base.marker.probability.ptr, head.marker.probability.ptr, current.marker.probability.ptr));

		std::vector<XMPDMO_CuePointInfo>::iterator iter1, iter2, iter3;
		for (iter1 = base.marker.cuePointParams.begin(), iter2 = head.marker.cuePointParams.begin(), iter3 = current.marker.cuePointParams.begin();
			iter1 != base.marker.cuePointParams.end() && iter2 != head.marker.cuePointParams.end() && iter3 != current.marker.cuePointParams.end();
			iter1++, iter2++, iter3++
			)
		{
			XMPDMO_CuePointInfo mergedCuePoint;
			mergedCuePoint.key = getXMPString(mergeMarkerField(iter1->key.ptr, iter2->key.ptr, iter3->key.ptr));
			mergedCuePoint.value = getXMPString(mergeMarkerField(iter1->value.ptr, iter2->value.ptr, iter3->value.ptr));

			mergedMarker.marker.cuePointParams.push_back(mergedCuePoint);
		}

		//TODO : Above code will merge markers till all the iterators are valid. If number of cuePointParams are diffrent in three markers, marged packet will 
		// miss extra cuePointParams because loop will end as soon as one iterator goes out of bound. Need to take care this scenerio, if this is a leagel condition. 

		return mergedMarker;
	}

	bool MergeMarkers::compareTrackData(XMPDMO_MarkerInfo_ex marker1, XMPDMO_MarkerInfo_ex marker2)
	{
		if (std::string(marker1.trackInfo.trackType).compare(marker2.trackInfo.trackType)) return 0;
		if (std::string(marker1.trackInfo.trackName).compare(marker2.trackInfo.trackName)) return 0;
		if (std::string(marker1.trackInfo.frameRate).compare(marker2.trackInfo.frameRate)) return 0;
		return 1;
	}

	bool MergeMarkers::compareMarkerData(XMPDMO_MarkerInfo_ex marker1, XMPDMO_MarkerInfo_ex marker2)
	{
		if ((marker1.marker.startTime.len != marker2.marker.startTime.len) || ((marker1.marker.startTime.ptr != 0) && (std::string(marker1.marker.startTime.ptr).compare(marker2.marker.startTime.ptr)))) return 0;
		if ((marker1.marker.duration.len != marker2.marker.duration.len) || ((marker1.marker.duration.ptr != 0) && (std::string(marker1.marker.duration.ptr).compare(marker2.marker.duration.ptr)))) return 0;
		if ((marker1.marker.comment.len != marker2.marker.comment.len) || ((marker1.marker.comment.ptr != 0) && (std::string(marker1.marker.comment.ptr).compare(marker2.marker.comment.ptr)))) return 0;
		if ((marker1.marker.name.len != marker2.marker.name.len) || ((marker1.marker.name.ptr != 0) && (std::string(marker1.marker.name.ptr).compare(marker2.marker.name.ptr)))) return 0;
		if ((marker1.marker.location.len != marker2.marker.location.len) || ((marker1.marker.location.ptr != 0) && (std::string(marker1.marker.location.ptr).compare(marker2.marker.location.ptr)))) return 0;
		if ((marker1.marker.target.len != marker2.marker.target.len) || ((marker1.marker.target.ptr != 0) && (std::string(marker1.marker.target.ptr).compare(marker2.marker.target.ptr)))) return 0;
		if ((marker1.marker.type.len != marker2.marker.type.len) || ((marker1.marker.type.ptr != 0) && (std::string(marker1.marker.type.ptr).compare(marker2.marker.type.ptr)))) return 0;
		if ((marker1.marker.speaker.len != marker2.marker.speaker.len) || ((marker1.marker.speaker.ptr != 0) && (std::string(marker1.marker.speaker.ptr).compare(marker2.marker.speaker.ptr)))) return 0;
		if ((marker1.marker.cuePointType.len != marker2.marker.cuePointType.len) || ((marker1.marker.cuePointType.ptr != 0) && (std::string(marker1.marker.cuePointType.ptr).compare(marker2.marker.cuePointType.ptr)))) return 0;
		if ((marker1.marker.probability.len != marker2.marker.probability.len) || ((marker1.marker.probability.ptr != 0) && (std::string(marker1.marker.probability.ptr).compare(marker2.marker.probability.ptr)))) return 0;

		std::vector<XMPDMO_CuePointInfo> marker1_cuePointParams = marker1.marker.cuePointParams;
		std::vector<XMPDMO_CuePointInfo> marker2_cuePointParams = marker1.marker.cuePointParams;
		if (marker1_cuePointParams.size() != marker2_cuePointParams.size()) return 0;

		std::vector<XMPDMO_CuePointInfo>::iterator iter1, iter2;
		for (iter1 = marker1_cuePointParams.begin(), iter2 = marker2_cuePointParams.begin();
			iter1 != marker1_cuePointParams.end();
			iter1++, iter2++
			)
		{
			if (iter1->key.len != iter2->key.len) return 0;
			if (iter1->value.len != iter2->value.len) return 0;

			if ((iter1->key.ptr != 0) && std::string(iter1->key.ptr).compare(iter2->key.ptr)) return 0;
			if ((iter2->key.ptr != 0) && std::string(iter1->value.ptr).compare(iter2->value.ptr)) return 0;
		}
		return 1;
	}

	MergeMarkers::MergeMarkers(spcINode base, spcINode latest, spcINode current)
	{
		isInitialized = false;
		isInitialized = (createMarkerMap(&m_baseMap, base) &&
			createMarkerMap(&m_headMap, latest) &&
			createMarkerMap(&m_currentMap, current)
			);
	}

	bool MergeMarkers::createMarkerMap(std::map<std::string, XMPDMO_MarkerInfo_ex > *map, spcINode node)
	{
		if (node == 0)
		{
			//Provided node is NULL means markers are absent, so map should be empty.
			return true;
		}

		//Check sanity of Marker
		if (!checkTracks(node))
			return false;

		//Valid Marker, create map.
		std::vector<XMPDMO_MarkerInfo_ex> markers;
		getMarkers(node, markers);

		for (std::vector<XMPDMO_MarkerInfo_ex>::iterator iter = markers.begin(); iter != markers.end(); iter++)
		{
			std::string GUID = getGUIDFromMarkerInfo(*iter);
			map->insert(std::make_pair(GUID, *iter));
		}

		return true;
	}

	std::string MergeMarkers::getGUIDFromMarkerInfo(XMPDMO_MarkerInfo_ex markerInfo)
	{
		std::string marker_key = "marker_guid";
		std::vector<XMPDMO_CuePointInfo> cuePointParams;
		std::vector<XMPDMO_CuePointInfo>::iterator cue_iter;
		cuePointParams = markerInfo.marker.cuePointParams;
		for (cue_iter = cuePointParams.begin(); cue_iter != cuePointParams.end(); cue_iter++)
		{
			if (!marker_key.compare(cue_iter->key.ptr))
			{
				return (std::string(cue_iter->value.ptr));
			}
		}
        return (std::string(""));
	}


	void MergeMarkers::fillMarkerVector(XMPDMO_MarkerInfo_ex *markerVector, spcINode markerItem)
	{
		spcINode markerNode;
		spcISimpleNode tSimpleNode;
		spcIStructureNode tnode;
		tnode = markerItem->ConvertToStructureNode();
		if (!tnode)
		{
			NOTIFY_ERROR(IError_base::kEDDataModel, kDMECDifferentNodeTypePresent,
				"Different type of node present than the one expected by the client ", IError_base::kESOperationFatal, false, false);
		}
		markerNode = tnode->GetNode(kXMP_NS_DM, AdobeXMPCommon::npos, "startTime", AdobeXMPCommon::npos);
		if (markerNode)
		{
			tSimpleNode = markerNode->ConvertToSimpleNode();
			if (!tSimpleNode)
				NOTIFY_ERROR(IError_base::kEDDataModel, kDMECDifferentNodeTypePresent,
				"Different type of node present than the one expected by the client ", IError_base::kESOperationFatal, false, false);
			markerVector->marker.startTime.ptr = tSimpleNode->GetValue()->c_str();
			markerVector->marker.startTime.len = (XMP_StringLen)strnlen_safe(markerVector->marker.startTime.ptr, AdobeXMPCommon::npos);
		}

		markerNode = tnode->GetNode(kXMP_NS_DM, AdobeXMPCommon::npos, "comment", AdobeXMPCommon::npos);
		if (markerNode)
		{
			tSimpleNode = markerNode->ConvertToSimpleNode();
			if (!tSimpleNode)
				NOTIFY_ERROR(IError_base::kEDDataModel, kDMECDifferentNodeTypePresent,
				"Different type of node present than the one expected by the client ", IError_base::kESOperationFatal, false, false);
			markerVector->marker.comment.ptr = tSimpleNode->GetValue()->c_str();
			markerVector->marker.comment.len = (XMP_StringLen)strnlen_safe(markerVector->marker.comment.ptr, AdobeXMPCommon::npos);
		}

		markerNode = tnode->GetNode(kXMP_NS_DM, AdobeXMPCommon::npos, "cuePointType", AdobeXMPCommon::npos);
		if (markerNode)
		{
			tSimpleNode = markerNode->ConvertToSimpleNode();
			if (!tSimpleNode)
				NOTIFY_ERROR(IError_base::kEDDataModel, kDMECDifferentNodeTypePresent,
				"Different type of node present than the one expected by the client ", IError_base::kESOperationFatal, false, false);
			markerVector->marker.cuePointType.ptr = tSimpleNode->GetValue()->c_str();
			markerVector->marker.cuePointType.len = (XMP_StringLen)strnlen_safe(markerVector->marker.cuePointType.ptr, AdobeXMPCommon::npos);
		}

		markerNode = tnode->GetNode(kXMP_NS_DM, AdobeXMPCommon::npos, "duration", AdobeXMPCommon::npos);
		if (markerNode)
		{
			tSimpleNode = markerNode->ConvertToSimpleNode();
			if (!tSimpleNode)
				NOTIFY_ERROR(IError_base::kEDDataModel, kDMECDifferentNodeTypePresent,
				"Different type of node present than the one expected by the client ", IError_base::kESOperationFatal, false, false);
			markerVector->marker.duration.ptr = tSimpleNode->GetValue()->c_str();
			markerVector->marker.duration.len = (XMP_StringLen)strnlen_safe(markerVector->marker.duration.ptr, AdobeXMPCommon::npos);
		}

		markerNode = tnode->GetNode(kXMP_NS_DM, AdobeXMPCommon::npos, "location", AdobeXMPCommon::npos);
		if (markerNode)
		{
			tSimpleNode = markerNode->ConvertToSimpleNode();
			if (!tSimpleNode)
				NOTIFY_ERROR(IError_base::kEDDataModel, kDMECDifferentNodeTypePresent,
				"Different type of node present than the one expected by the client ", IError_base::kESOperationFatal, false, false);
			markerVector->marker.location.ptr = tSimpleNode->GetValue()->c_str();
			markerVector->marker.location.len = (XMP_StringLen)strnlen_safe(markerVector->marker.location.ptr, AdobeXMPCommon::npos);
		}

		markerNode = tnode->GetNode(kXMP_NS_DM, AdobeXMPCommon::npos, "name", AdobeXMPCommon::npos);
		if (markerNode)
		{
			tSimpleNode = markerNode->ConvertToSimpleNode();
			if (!tSimpleNode)
				NOTIFY_ERROR(IError_base::kEDDataModel, kDMECDifferentNodeTypePresent,
				"Different type of node present than the one expected by the client ", IError_base::kESOperationFatal, false, false);
			markerVector->marker.name.ptr = tSimpleNode->GetValue()->c_str();
			markerVector->marker.name.len = (XMP_StringLen)strnlen_safe(markerVector->marker.name.ptr, AdobeXMPCommon::npos);
		}

		markerNode = tnode->GetNode(kXMP_NS_DM, AdobeXMPCommon::npos, "probability", AdobeXMPCommon::npos);
		if (markerNode)
		{
			tSimpleNode = markerNode->ConvertToSimpleNode();
			if (!tSimpleNode)
				NOTIFY_ERROR(IError_base::kEDDataModel, kDMECDifferentNodeTypePresent,
				"Different type of node present than the one expected by the client ", IError_base::kESOperationFatal, false, false);
			markerVector->marker.probability.ptr = tSimpleNode->GetValue()->c_str();
			markerVector->marker.probability.len = (XMP_StringLen)strnlen_safe(markerVector->marker.probability.ptr, AdobeXMPCommon::npos);
		}

		markerNode = tnode->GetNode(kXMP_NS_DM, AdobeXMPCommon::npos, "speaker", AdobeXMPCommon::npos);
		if (markerNode)
		{
			tSimpleNode = markerNode->ConvertToSimpleNode();
			if (!tSimpleNode)
				NOTIFY_ERROR(IError_base::kEDDataModel, kDMECDifferentNodeTypePresent,
				"Different type of node present than the one expected by the client ", IError_base::kESOperationFatal, false, false);
			markerVector->marker.speaker.ptr = tSimpleNode->GetValue()->c_str();
			markerVector->marker.speaker.len = (XMP_StringLen)strnlen_safe(markerVector->marker.speaker.ptr, AdobeXMPCommon::npos);
		}

		markerNode = tnode->GetNode(kXMP_NS_DM, AdobeXMPCommon::npos, "target", AdobeXMPCommon::npos);
		if (markerNode)
		{
			tSimpleNode = markerNode->ConvertToSimpleNode();
			if (!tSimpleNode)
				NOTIFY_ERROR(IError_base::kEDDataModel, kDMECDifferentNodeTypePresent,
				"Different type of node present than the one expected by the client ", IError_base::kESOperationFatal, false, false);
			markerVector->marker.target.ptr = tSimpleNode->GetValue()->c_str();
			markerVector->marker.target.len = (XMP_StringLen)strnlen_safe(markerVector->marker.target.ptr, AdobeXMPCommon::npos);
		}

		markerNode = tnode->GetNode(kXMP_NS_DM, AdobeXMPCommon::npos, "type", AdobeXMPCommon::npos);
		if (markerNode)
		{
			tSimpleNode = markerNode->ConvertToSimpleNode();
			if (!tSimpleNode)
				NOTIFY_ERROR(IError_base::kEDDataModel, kDMECDifferentNodeTypePresent,
				"Different type of node present than the one expected by the client ", IError_base::kESOperationFatal, false, false);
			markerVector->marker.type.ptr = tSimpleNode->GetValue()->c_str();
			markerVector->marker.type.len = (XMP_StringLen)strnlen_safe(markerVector->marker.type.ptr, AdobeXMPCommon::npos);
		}
	}

	void MergeMarkers::fillCuePointArray(XMPDMO_CuePointInfo *cuePoint, spcINode cuePointArrayItem)
	{
		spcINode cuePointNode;
		spcISimpleNode tSimpleNode;
		spcIStructureNode tnode;
		tnode = cuePointArrayItem->ConvertToStructureNode();
		if (!tnode)
		{
			//throw error
			NOTIFY_ERROR(IError_base::kEDDataModel, kDMECDifferentNodeTypePresent,
				"Different type of node present than the one expected by the client ", IError_base::kESOperationFatal, false, false);
		}

		cuePointNode = tnode->GetNode(kXMP_NS_DM, AdobeXMPCommon::npos, "key", AdobeXMPCommon::npos);
		if (cuePointNode)
		{
			tSimpleNode = cuePointNode->ConvertToSimpleNode();
			if (!tSimpleNode)
				NOTIFY_ERROR(IError_base::kEDDataModel, kDMECDifferentNodeTypePresent,
				"Different type of node present than the one expected by the client ", IError_base::kESOperationFatal, false, false);
			cuePoint->key.ptr = tSimpleNode->GetValue()->c_str();
			cuePoint->key.len = (XMP_StringLen)strnlen_safe(cuePoint->key.ptr, AdobeXMPCommon::npos);
		}

		cuePointNode = tnode->GetNode(kXMP_NS_DM, AdobeXMPCommon::npos, "value",  AdobeXMPCommon::npos);
		if (cuePointNode)
		{
			tSimpleNode = cuePointNode->ConvertToSimpleNode();
			if (!tSimpleNode)
				NOTIFY_ERROR(IError_base::kEDDataModel, kDMECDifferentNodeTypePresent,
				"Different type of node present than the one expected by the client ", IError_base::kESOperationFatal, false, false);
			cuePoint->value.ptr = tSimpleNode->GetValue()->c_str();
			cuePoint->value.len = (XMP_StringLen)strnlen_safe(cuePoint->value.ptr, AdobeXMPCommon::npos);
		}
	}

	void MergeMarkers::getMarkers(spcINode node, std::vector<XMPDMO_MarkerInfo_ex> &marker)
	{
		spcIStructureNode tStructNode;
		spcISimpleNode tSimpleNode;
		spcIArrayNode tnode;
		tnode = node->ConvertToArrayNode();
		if (!tnode)
			return;
		if (tnode->ChildCount() == 0) return;

		spcINodeIterator iter_Track, iter_Markers, iter_cuePoint;
		spcINode trackItem, cuePointArrayItem, markerItem;
		spcIStructureNode trackInfoStruct;
		spcIArrayNode markerArray;
		spcISimpleNode simpleProp;
		spcINode tempNode;

		//Iterate over all items of the bag "Tracks"
		iter_Track = tnode->Iterator();

		while (iter_Track)
		{
			trackItem = iter_Track->GetNode();

			std::string trackName, trackType, frameRate;

			//Fetch values for the struct
			tStructNode = trackItem->ConvertToStructureNode();
			if (!tStructNode)
				NOTIFY_ERROR(IError_base::kEDDataModel, kDMECDifferentNodeTypePresent,
				"Different type of node present than the one expected by the client ", IError_base::kESOperationFatal, false, false);
			
			tempNode = tStructNode->GetNode(kXMP_NS_DM, AdobeXMPCommon::npos, "trackName", AdobeXMPCommon::npos);
			if (tempNode) {
				tSimpleNode = tempNode->ConvertToSimpleNode();
				if (tSimpleNode)
					trackName = tSimpleNode->GetValue()->c_str();
			}
			
			
			tempNode = tStructNode->GetNode(kXMP_NS_DM, AdobeXMPCommon::npos, "trackType", AdobeXMPCommon::npos);
			if (tempNode) {
				tSimpleNode = tempNode->ConvertToSimpleNode();
				if (tSimpleNode)
					trackType = tSimpleNode->GetValue()->c_str();
			}

			
			tempNode = tStructNode->GetNode(kXMP_NS_DM, AdobeXMPCommon::npos, "frameRate", AdobeXMPCommon::npos);
			if (tempNode) {
				tSimpleNode = tempNode->ConvertToSimpleNode();
				if (tSimpleNode)
					frameRate = tSimpleNode->GetValue()->c_str();
			}


			// Now get all the markers		
			
			tempNode = tStructNode->GetNode(kXMP_NS_DM, AdobeXMPCommon::npos, "markers", AdobeXMPCommon::npos);
			markerArray = tempNode->ConvertToArrayNode();
			if (!markerArray)
				NOTIFY_ERROR(IError_base::kEDDataModel, kDMECDifferentNodeTypePresent,
				"Different type of node present than the one expected by the client ", IError_base::kESOperationFatal, false, false);
			iter_Markers = markerArray->Iterator();

			while (iter_Markers)
			{
				XMPDMO_MarkerInfo_ex markerVector;
				markerItem = iter_Markers->GetNode();
				if (trackName.length() != 0) markerVector.trackInfo.trackName = trackName;
				if (trackType.length() != 0) markerVector.trackInfo.trackType = trackType;
				if (frameRate.length() != 0) markerVector.trackInfo.frameRate = frameRate;

				fillMarkerVector(&markerVector, markerItem);

				tempNode = markerItem->ConvertToStructureNode()->GetNode(kXMP_NS_DM, AdobeXMPCommon::npos, "cuePointParams", AdobeXMPCommon::npos);
				spcIArrayNode cuePointNode = tempNode->ConvertToArrayNode();
				if (!cuePointNode)
					NOTIFY_ERROR(IError_base::kEDDataModel, kDMECDifferentNodeTypePresent,
					"Different type of node present than the one expected by the client ", IError_base::kESOperationFatal, false, false);
				iter_cuePoint = cuePointNode->Iterator();

				while (iter_cuePoint)
				{
					XMPDMO_CuePointInfo cuePoint;
					cuePointArrayItem = iter_cuePoint->GetNode();
					fillCuePointArray(&cuePoint, cuePointArrayItem);

					markerVector.marker.cuePointParams.push_back(cuePoint);
					iter_cuePoint = iter_cuePoint->Next();
				}
				marker.push_back(markerVector);
				iter_Markers = iter_Markers->Next();
			}

			iter_Track = iter_Track->Next();
		}
	}

	// *********************************
	// Strategy header functions

		bool APICALL MarkerThreeWayGenericStrategyImpl::InterestedInResolvingConflict(const spcIUTF8String & nodePath, INode::eNodeType nodeType, IThreeWayUnResolvedConflict::eConflictReason reason, bool alreadyResolved) {

		if (alreadyResolved)
			return false;

		if ((nodeType != INode::kNTArray) || (reason != IThreeWayUnResolvedConflict::kCRValuesMismatch &&
			reason != IThreeWayUnResolvedConflict::kCRDeletedInLatestEditedInUser &&
			reason != IThreeWayUnResolvedConflict::kCRDeletedInUserEditedInLatest))
				return true;
		
		if (strncmp(nodePath->c_str(), markersPath, nodePath->size()) == 0) {
			return true;
		}

		sizet pathSize = nodePath->size();
		for (auto it = mPaths->begin(); it != mPaths->end(); it++)
		{
			sizet registeredPathSize = (*it)->size();
			if ((pathSize == registeredPathSize) && (strncmp(nodePath->c_str(), (*it)->c_str(), registeredPathSize) == 0)){
				return true;
			}
		}
		return false;
	}

	IThreeWayUnResolvedConflict::eConflictReason APICALL MarkerThreeWayGenericStrategyImpl::ResolveConflict(IThreeWayUnResolvedConflict::eConflictReason reasonsForConflict,
		const spcINode & commonBaseNode, const spcINode & latestVersionNode, const spcINode & userVersionNode,
		spcINode & resolvedNode, IThreeWayResolvedConflict::eConflictResolutionReason& reasonForConflictResolution, IThreeWayResolvedConflict::eResolvedSource & resolvedNodeSource)
	{
		
		const spcIUTF8String  nodePath = latestVersionNode ? latestVersionNode->GetPath()->Serialize() : userVersionNode->GetPath()->Serialize();
		if (strncmp(nodePath->c_str(), markersPath, nodePath->size()) != 0){
			return reasonsForConflict;
		}

		MergeMarkers obj(commonBaseNode, latestVersionNode, userVersionNode);

		if (obj.Merge())
			resolvedNode = obj.getMergedNode();

		if (resolvedNode)
		{
			resolvedNodeSource = IThreeWayResolvedConflict::kRSFieldsMerged;
			reasonForConflictResolution = IThreeWayResolvedConflict::kCRRStrategyResolved;
			return IThreeWayUnResolvedConflict::kCRResolved;
		}
		else
			return reasonsForConflict;
	}
}
