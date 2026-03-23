// =================================================================================================
// ADOBE SYSTEMS INCORPORATED
// Copyright 2014 Adobe Systems Incorporated
// All Rights Reserved
//
// NOTICE:  Adobe permits you to use, modify, and distribute this file in accordance with the terms
// of the Adobe license agreement accompanying it.
// =================================================================================================

#include <string>
#include <iostream>
#include <sstream>
#include <map>
#include <float.h>
#include <math.h>
#define TXMP_STRING_TYPE std::string

#include "public/include/XMP.hpp"
//#include "public/include/XMP.incl_cpp"

#include "TXMPDocOps.hpp"

#define IMPLEMENTATION_HEADERS_CAN_BE_INCLUDED 1
#include "XMPMarker/ImplHeaders/MarkerImpl.h"
#undef IMPLEMENTATION_HEADERS_CAN_BE_INCLUDED

#include "XMPCommon/Interfaces/IErrorNotifier_I.h"
#include "XMPCommon/Utilities/AutoSharedLock.h"
#include "XMPCommon/Interfaces/IUTF8String_I.h"
#include "XMPCore/Interfaces/INode.h"
#include "XMPMarker/ImplHeaders/TemporalAssetUtilitiesImpl.h"
#include "include/IExtensionsConfigurationManager_I.h"
#include "XMPMarker/Interfaces/IExtensionHandler.h"
#include "XMPMarker/Interfaces/IExtensionHandlerRegistry_I.h"
#include "XMPMarker/Interfaces/IExtensionHandler_I.h"




// Provide access to the API

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
#include "XMPCommon/Interfaces/IUTF8String.h"

namespace AdobeXMPAM_Int {


	using namespace AdobeXMPAM_Int;
	using namespace AdobeXMPCore;
	using namespace AdobeXMPCommon;
	using namespace AdobeXMPAM;
	const std::string kKeywordExtDVAPrefixV1 = "keywordExtDVAv1_";

	const std::string quoteString = "&quot;";
	const char startBrace = '{';
	const char closeBrace = '}';
	
	static const XMP_VarString extensionPrefix = "Extensions_";
	static XMP_VarString doubleQuotes = "&quot;";
	static const XMP_VarString openingBrace = "{";
	static const XMP_VarString closingBrace = "}";
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
	const sizet kXMP_NS_DM_LEN = strlen(kXMP_NS_DM);

	static void GenerateGUID(std::string &uuid) {
		
		SXMPDocOps::CreateID(&uuid, "");
		
	}

	static bool HandleSimpleNode(const pIExtensionHandler & handler, const spcISimpleNode & node);
	static bool HandleStructureNode(const pIExtensionHandler & handler, const spcIStructureNode & node);
	static bool HandleArrayNode(const pIExtensionHandler & handler, const spcIArrayNode & node);


	static bool HandleNode(const pIExtensionHandler & handler, const spcINode & node) {
		bool retValue(false);
		if (!node) return false;
		switch (node->GetNodeType()) {
		case INode::kNTSimple:
			retValue = HandleSimpleNode(handler, node->ConvertToSimpleNode());
			break;
		case INode::kNTStructure:
			retValue = HandleStructureNode(handler, node->ConvertToStructureNode());
			break;
		case INode::kNTArray:
			retValue = HandleArrayNode(handler, node->ConvertToArrayNode());
			break;
        default:
            return retValue;
            break;
		}
		return retValue;
    }

	static bool HandleExtensionNode(const pIExtensionHandler & handler, const spcIStructureNode & node) {
		auto it = node->Iterator();
		while (it) {
			bool retValue = HandleNode(handler, it->GetNode());
			if (!retValue)
				return false;
			it = it->Next();
		}
		return true;
	}

	static pIExtension ParseNode(const pIExtensionHandler & handler, const spcIStructureNode & node) {
		if (handler->BeginExtension(node->GetNameSpace()->c_str(), node->GetNameSpace()->size(), node->GetName()->c_str(), node->GetName()->size())) {
			if (HandleExtensionNode(handler, node))
				return handler->EndExtension(node->GetNameSpace()->c_str(), node->GetNameSpace()->size(), node->GetName()->c_str(), node->GetName()->size());
		}
		return NULL;
	}

	static bool HandleCustomDataNode(const pIExtensionHandler & handler, const spcIStructureNode & node) {
		auto it = node->Iterator();
		while (it) {
			bool retValue = HandleNode(handler, it->GetNode());
			if (!retValue)
				return false;
			it = it->Next();
		}
		return true;
	}

	static bool HandleSimpleNode(const pIExtensionHandler & handler, const spcISimpleNode & node) {

		return handler->AddKeyValuePair(node->GetNameSpace()->c_str(), node->GetNameSpace()->size(), node->GetName()->c_str(), node->GetName()->size(), node->GetValue()->c_str(), node->GetValue()->size());
	}

	static bool HandleStructureNode(const pIExtensionHandler & handler, const spcIStructureNode & node) {
		const char * name = node->GetName()->c_str();
		if (handler->BeginStructure(node->GetNameSpace()->c_str(), node->GetNameSpace()->size(), node->GetName()->c_str(), node->GetName()->size())) {
			auto it = node->Iterator();
			while (it) {
				bool retValue = HandleNode(handler, it->GetNode());
				if (!retValue)
					return false;
				it = it->Next();
			}
			return handler->EndStructure(node->GetNameSpace()->c_str(), node->GetNameSpace()->size(), node->GetName()->c_str(), node->GetName()->size());
		}
		return false;
	}

	static bool HandleArrayNode(const pIExtensionHandler & handler, const spcIArrayNode & node) {
		const char * name = node->GetName()->c_str();
		if (handler->BeginArray(node->GetNameSpace()->c_str(), node->GetNameSpace()->size(), node->GetName()->c_str(), node->GetName()->size())) {
			auto it = node->Iterator();
			while (it) {
				bool retValue = HandleNode(handler, it->GetNode());
				if (!retValue)
					return false;
				it = it->Next();
			}
			return handler->EndArray(node->GetNameSpace()->c_str(), node->GetNameSpace()->size(), node->GetName()->c_str(), node->GetName()->size());
		}
		return false;
	}


//	// TODO ERROR MECHANISMS
	spcIUTF8String MarkerImpl::GetName_I() const {
		if (!mNameNode) return spcIUTF8String();
		return ( mNameNode->GetValue() );
	}

	bool IsBitSet(uint64 inputMask, uint64 checkMask) {

		if (inputMask & checkMask) {
			return true;
		}
		return false;
	}

	

	void MarkerImpl::MergeNodes(const AdobeXMPCore::spIStructureNode & sourceRootNode, const AdobeXMPCore::spIStructureNode & destNode) {

		using namespace AdobeXMPCore;
		if (!sourceRootNode || !destNode) return;
		spINode sourceNode = sourceRootNode->GetNode(destNode->GetNameSpace()->c_str(), AdobeXMPCommon::npos, destNode->GetName()->c_str(), AdobeXMPCommon::npos);
		if (!sourceNode) {
			// destNode is not present in sourceRootNode
			sourceRootNode->AppendNode(destNode);
		}
		else {

			if (!IsExtensionValidForBackwardCompatibility(sourceNode)) {

			}
			else {
				spIStructureNode sourceStructureNode = sourceNode->ConvertToStructureNode();
				for (auto destNodeChildIter = destNode->Iterator(); destNodeChildIter; destNodeChildIter = destNodeChildIter->Next()) {

					spISimpleNode destChildNode = destNodeChildIter->GetNode()->ConvertToSimpleNode();
					if (!destChildNode) continue;
					if (!sourceStructureNode->GetNode(destChildNode->GetNameSpace()->c_str(), AdobeXMPCommon::npos, destChildNode->GetName()->c_str(), AdobeXMPCommon::npos)) {
						spINode cloneOfDestChildNode = destChildNode->Clone();
						sourceStructureNode->AppendNode(cloneOfDestChildNode);
					}
				}
			}
			destNode->Clear();
		}
	}

	std::string GetCombinedString(const std::string & nameSpace, const std::string & name)  {
		std::string combinedString(nameSpace);
		combinedString.append(":").append(name);
		return combinedString;
	}

	void MarkerImpl::RemoveSpeakerNode() {

		if (mSpeakerNode) {

			mRootNode->RemoveNode(kXMP_NS_DM, kXMP_NS_DM_LEN, kSpeaker, strlen(kSpeaker));
			mSpeakerNode = spISimpleNode();
		}
	}

	void MarkerImpl::RemoveLocationNode() {

		if (mLocationNode) {

			mRootNode->RemoveNode(kXMP_NS_DM, kXMP_NS_DM_LEN, kLocation, strlen(kLocation));
			mLocationNode = spISimpleNode();
		}
	}


	void MarkerImpl::RemoveProbabilityNode() {

		if (mLocationNode) {

			mRootNode->RemoveNode(kXMP_NS_DM, kXMP_NS_DM_LEN, kProbability, strlen(kProbability));
			mProbabilityNode = spISimpleNode();
		}
	}

	void MarkerImpl::RemoveCommentNode() {

		if (mCommentNode) {

			mRootNode->RemoveNode(kXMP_NS_DM, kXMP_NS_DM_LEN,  kComment, strlen(kComment));
			mCommentNode = spISimpleNode();
		}
	}

	void MarkerImpl::RemoveCuePointParamsNode() {

		if (mCuePointParamsNode) {

			mRootNode->RemoveNode(kXMP_NS_DM, kXMP_NS_DM_LEN, kCuePointParams, strlen(kCuePointParams));
			mCuePointParamsNode = spIArrayNode();
		}
	}

	void MarkerImpl::RemoveStartTimeNode() {

		if (mStartTimeNode) {

			mRootNode->RemoveNode(kXMP_NS_DM, kXMP_NS_DM_LEN, kStartTime, strlen(kStartTime));
			mStartTimeNode = spISimpleNode();
		}
	}

	void MarkerImpl::RemoveDurationNode() {

		if (mDurationNode) {

			mRootNode->RemoveNode(kXMP_NS_DM, kXMP_NS_DM_LEN, kDuration, strlen(kDuration));
			mDurationNode = spISimpleNode();
		}
	}

	void MarkerImpl::RemoveTargetNode() {

		if (mTargetNode) {

			mRootNode->RemoveNode(kXMP_NS_DM, kXMP_NS_DM_LEN, kTarget, strlen(kTarget));
			mTargetNode = spISimpleNode();
		}
	}

	void MarkerImpl::RemoveExtensionNode() {

		if (mExtension) {

			mRootNode->RemoveNode(kXMP_NS_DM, kXMP_NS_DM_LEN, kExtensions, strlen(kExtensions));
			mExtension = spIStructureNode();
		}
	}

	void MarkerImpl::RemoveNameNode() {

		if (mNameNode) {

			mRootNode->RemoveNode(kXMP_NS_DM, kXMP_NS_DM_LEN, kMarkerName, strlen(kMarkerName));
			mNameNode = spISimpleNode();
		}
	}



	void MarkerImpl::CreateExtensionNodeIfRequired() {
		if (!mExtension) {
			mExtension = IStructureNode::CreateStructureNode(kXMP_NS_XMP, strlen(kXMP_NS_XMP), kExtensions, strlen(kExtensions));
			mRootNode->AppendNode(mExtension);
		}
	}

	void MarkerImpl::UpdateFrameInformationInDOM() {

		std::string startTime, duration;
		TemporalAssetUtilities::ConvertFrameCountToString(duration, mFrameDuration.second, mFrameRateForCountOfFrames.first, mFrameRateForCountOfFrames.second, true);
		TemporalAssetUtilities::ConvertFrameCountToString(startTime, mFrameDuration.first, mFrameRateForStartFrameCount.first, mFrameRateForStartFrameCount.second, false);
		if (!mStartTimeNode) {

			mStartTimeNode = ISimpleNode::CreateSimpleNode(kXMP_NS_DM, kXMP_NS_DM_LEN, kStartTime, strlen(kStartTime), startTime.c_str(), startTime.length());
			mRootNode->AppendNode(mStartTimeNode);
		}
		else {

			mStartTimeNode->SetValue(startTime.c_str(), startTime.length());
		}
		if (!mDurationNode) {

			mDurationNode = ISimpleNode::CreateSimpleNode(kXMP_NS_DM, kXMP_NS_DM_LEN, kDuration, strlen(kDuration), duration.c_str(), duration.length());
			mRootNode->AppendNode(mDurationNode);
		}
		else {
			mDurationNode->SetValue(duration.c_str(), duration.length());
		}
	}


	bool MarkerImpl::SerializeExtensionAsJSON(const AdobeXMPCore::spINode & extensionNode, std::string & key, std::string& value) {

		using namespace AdobeXMPCore;

		if (!IsExtensionValidForBackwardCompatibility(extensionNode)) {

			return false;
		}
		key = extensionPrefix;
		std::string extensionQualifiedName;

		GetNodeQualifiedName(extensionNode, extensionQualifiedName);
		key.append(extensionQualifiedName);
		value.clear();
		value.append(std::string(1, '{'));
		spINodeIterator extensionNodeChildIter;
		if (extensionNode->GetNodeType() == INode::kNTStructure) {
			extensionNodeChildIter = extensionNode->ConvertToStructureNode()->Iterator();
		}
		else if (extensionNode->GetNodeType() == INode::kNTArray) {
			extensionNodeChildIter = extensionNode->ConvertToArrayNode()->Iterator();
		}
		bool isNotFirstChild = false;
		for (; extensionNodeChildIter; extensionNodeChildIter = extensionNodeChildIter->Next()) {

			if (isNotFirstChild) {
				value.append(",");
			}
			else {
				isNotFirstChild = true;
			}

			spcINode childNode = extensionNodeChildIter->GetNode();
			std::string childQualifiedName;
			GetNodeQualifiedName(childNode, childQualifiedName);
			CreateStringForSerialization(childQualifiedName);
			value.append(childQualifiedName);
			value.append(":");
			std::string childNodeValue = childNode->ConvertToSimpleNode()->GetValue()->c_str();
			CreateStringForSerialization(childNodeValue);
			value.append(childNodeValue);
		}
		value.append(std::string(1, '}'));
		return true;
	}

	MarkerImpl::MarkerImpl(const char * name, sizet markerNameLen, uint64 markerTypes, const char * markerTypeName, sizet markerTypeNameLen, const char * markerGUID, sizet guidLen){

		if (!name)  {

			/*NOTIFY_WARNING(IError_base::kDomainGeneral, IError_base::kGeneralCodeParametersNotAsExpected,
				"Parameters to CreateMarker() are not as expected",
				true, (void *)name,
				name, name);*/
			return;
		}

		if (markerTypes & kMTUserDefinedMarker) {
			if (!markerTypeName) {

				/*NOTIFY_WARNING(IError_base::kDomainGeneral, IError_base::kGeneralCodeParametersNotAsExpected,
					"Parameters to CreateMarker() are not as expected",
					true, (void *)markerTypeName,
					markerTypeName, markerTypeName);*/
				return;
			}
			
		}
		mRootNode = IStructureNode::CreateStructureNode(kXMP_NS_DM, kXMP_NS_DM_LEN, kMarker, strlen(kMarker));
		mNameNode = ISimpleNode::CreateSimpleNode(kXMP_NS_DM, kXMP_NS_DM_LEN, kMarkerName, 4, name, markerNameLen);
		
		spIUTF8String mGUID;
		if (markerGUID && guidLen) {

			mGUID = IUTF8String_I::CreateUTF8String(markerGUID, guidLen);
		}
		else {
			std::string guidStr;
			GenerateGUID(guidStr);
			mGUID = IUTF8String_I::CreateUTF8String(guidStr.c_str(), guidStr.length());
		}
		mGUIDNode = ISimpleNode::CreateSimpleNode(kXMP_NS_DM, kXMP_NS_DM_LEN, kGUID, strlen(kGUID), mGUID->c_str(), mGUID->size());
		mRootNode->AppendNode(mGUIDNode);
		mRootNode->AppendNode(mNameNode);
		mTypeMask = markerTypes;
		if (markerTypeName) {

			mTypeMask |= kMTUserDefinedMarker;
			spIUTF8String customType = IUTF8String_I::CreateUTF8String(markerTypeName, strlen(markerTypeName));
			mCustomTypes.push_back((customType));
			mCustomTypeMap[markerTypeName] = true;
			
		}

		std::string typeString;
		TemporalAssetUtilities::CreateCommaSeparatedString(mTypeMask, mCustomTypes, typeString);
		if (typeString.length()) {

			if (!mTypeNode) {

				mTypeNode = ISimpleNode::CreateSimpleNode(kXMP_NS_DM, kXMP_NS_DM_LEN, kType, strlen(kType), typeString.c_str(), typeString.length());
				mRootNode->AppendNode(mTypeNode);
			}
			else {

				mTypeNode->SetValue(typeString.c_str(), typeString.length());
			}
		}
		mFrameDuration.first = 0;
		mFrameDuration.second = UINT64_MAX;
		mFrameRateForCountOfFrames.first = mFrameRateForCountOfFrames.second = mFrameRateForStartFrameCount.first = mFrameRateForStartFrameCount.second = 1;
		UpdateFrameInformationInDOM();

	}

	spIMarker_I IMarker_I::CreateMarker_I(const char * markerName, sizet markerNameLen, uint64 markerTypes, const char * markerTypeName, sizet markerTypeNameLen, const char * guid, sizet guidLen) {

		return MakeUncheckedSharedPointer(new MarkerImpl(markerName, markerNameLen, markerTypes, markerTypeName, markerTypeNameLen,  guid, guidLen), __FILE__, __LINE__, true);
	}

	void MarkerImpl::SetName_I(const char * name, sizet nameLen) {

		if ( !name )  {

			/*NOTIFY_WARNING(IError_base::kDomainGeneral, IError_base::kGeneralCodeParametersNotAsExpected,
				"Parameters to IXMPNameSpacePrefixMap::AddOrReplacePair() are not as expected",
				true, (void *)name,
				name, name);*/
			RemoveNameNode();
			return;

		}

		
		mNameNode->SetValue(name, nameLen);
		return;
	}


	uint64 MarkerImpl::SetTypes_I(uint64 inputMask) {

		//TODO validate inputMask

		if (!IsBitSet(mTypeMask, kMTUserDefinedMarker) && IsBitSet(inputMask, kMTUserDefinedMarker ) ) {

			if ( ! mCustomTypes.size() ) {

				inputMask &= ~kMTUserDefinedMarker;
			}
		}

		mTypeMask |= inputMask;
		std::string typeString;
		TemporalAssetUtilities::CreateCommaSeparatedString(mTypeMask, mCustomTypes, typeString);
		if (typeString.length()) {

			if (!mTypeNode) {

				mTypeNode = ISimpleNode::CreateSimpleNode(kXMP_NS_DM, kXMP_NS_DM_LEN, kType, strlen(kType), typeString.c_str(), typeString.length());
				mRootNode->AppendNode(mTypeNode);
			}
			else {

				mTypeNode->SetValue(typeString.c_str(), typeString.length());
			}
		}

		return mTypeMask;
	}

	uint64 MarkerImpl::RemoveTypes_I(uint64 inputMask) {

		//TODO validate inputMask
		if (!IsBitSet(mTypeMask, IMarker::kMTUserDefinedMarker) && IsBitSet(inputMask, IMarker::kMTUserDefinedMarker)) {

			mCustomTypes.clear();
			mCustomTypeMap.clear();
		}
		uint64 currentBit = 1ull << 63;
		while (currentBit) {

			if ((inputMask & currentBit) && (mTypeMask & currentBit)) {
				

				mTypeMask ^= currentBit;
				 
			}
			currentBit >>= 1;
		}

		std::string typeString;
		TemporalAssetUtilities::CreateCommaSeparatedString(mTypeMask, mCustomTypes, typeString);
		if (typeString.length()) {

			if (!mTypeNode) {

				mTypeNode = ISimpleNode::CreateSimpleNode(kXMP_NS_DM, kXMP_NS_DM_LEN, kType, strlen(kType), typeString.c_str(), typeString.length());
				mRootNode->AppendNode(mTypeNode);
			}
			else {

				mTypeNode->SetValue(typeString.c_str(), typeString.length());
			}
		}
		return mTypeMask;
	}

	uint64 APICALL MarkerImpl::GetTypes_I() const {

		return mTypeMask;
	}

	bool MarkerImpl::AppendCustomType_I(const char * type, sizet typeLen) {

		if (!type) {

			//NOTIFY_WARNING(IError_base::kDomainGeneral, IError_base::kGeneralCodeParametersNotAsExpected,
			//	"Parameters to CreateMarker() are not as expected",
			//	true, (void *)type,
			//	type, type);
			return false;
		}

		if (mCustomTypeMap[type] ) {
			return false;
		}
		mTypeMask |= kMTUserDefinedMarker;
		spIUTF8String customType = IUTF8String_I::CreateUTF8String(type, typeLen);
		mCustomTypes.push_back((customType));
		mCustomTypeMap[type] = true;
		std::string typeString;
		TemporalAssetUtilities::CreateCommaSeparatedString(mTypeMask, mCustomTypes, typeString);
		if (typeString.length()) {

			if (!mTypeNode) {

				mTypeNode = ISimpleNode::CreateSimpleNode(kXMP_NS_DM, kXMP_NS_DM_LEN,kType, strlen(kType), typeString.c_str(), typeString.length());
				mRootNode->AppendNode(mTypeNode);
			}
			else {

				mTypeNode->SetValue(typeString.c_str(), typeString.length());
			}
		}
		return true;
	}
	
	bool MarkerImpl::RemoveCustomType_I(const char * type, sizet typeLen) {

		if (!type) {

			/*NOTIFY_WARNING(IError_base::kDomainGeneral, IError_base::kGeneralCodeParametersNotAsExpected,
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
		for (int idx = mCustomTypes.size() - 1; idx >=0 ; --idx) {

			if (!strcmp(type, mCustomTypes[idx]->c_str())) {

				operationsPerformed = true;
				mCustomTypes.erase(mCustomTypes.begin() + idx);
			}
		}

		if (!mCustomTypes.size()) {

			mTypeMask &= ~kMTUserDefinedMarker;
		}
		std::string typeString;
		TemporalAssetUtilities::CreateCommaSeparatedString(mTypeMask, mCustomTypes, typeString);
		if (typeString.length()) {

			if (!mTypeNode) {

				mTypeNode = ISimpleNode::CreateSimpleNode(kXMP_NS_DM, kXMP_NS_DM_LEN, kType, strlen(kType), typeString.c_str(), typeString.length());
				mRootNode->AppendNode(mTypeNode);
			}
			else {

				mTypeNode->SetValue(typeString.c_str(), typeString.length());
			}
		}
		return operationsPerformed;
	}

	sizet MarkerImpl::GetCustomTypeCount_I() const {

		return mCustomTypes.size();
	}

	spcIUTF8String MarkerImpl::GetCustomType_I(sizet index) const {
		

		if (index <= 0 && index > mCustomTypes.size()) {

			/*NOTIFY_WARNING(IError_base::kDomainGeneral, IError_base::kGeneralCodeParametersNotAsExpected,
				"Parameters to Remove custom type are not as expected",
				true, (void *)index,
				index, index);*/
			return spcIUTF8String();
		}
		
		return mCustomTypes[ index ];
	}

	spcIUTF8StringList_I MarkerImpl::GetCustomTypes_I() const {

		spcIUTF8StringList_I list(new cIUTF8StringList_I());
		for (size_t idx = 0; idx < mCustomTypes.size(); ++idx) {
			list->push_back((mCustomTypes[idx]));
		}

		return list;
	}

	spcIUTF8StringList_I MarkerImpl::GetCuePointParams_I(const char * inputKey, sizet inputKeyLen) const {

		
		spcIUTF8StringList_I list(new cIUTF8StringList_I());
		if (!mCuePointParamsNode) {

			return list;
		}
		for (size_t count = mCuePointParamsNode->ChildCount(), idx = 1; idx <= count; ++idx) {
			spIStructureNode cuePointParam = mCuePointParamsNode->GetNodeAtIndex(idx)->ConvertToStructureNode();
			spISimpleNode keyNode = cuePointParam->GetSimpleNode(kXMP_NS_DM, kXMP_NS_DM_LEN, kKey, strlen(kKey));
			if (!strcmp(keyNode->GetValue()->c_str(), inputKey)) {
				spISimpleNode valueNode = cuePointParam->GetSimpleNode(kXMP_NS_DM, kXMP_NS_DM_LEN, kValue, strlen(kValue));
				list->push_back(valueNode->GetValue());
			}
		}

		return list;
	}

	AdobeXMPAM_Int::spcICuePointParamList_I MarkerImpl::GetAllCuePointParams_I() const {

		spcICuePointParamList_I cuePointList(new cICuePointParamList_I());
		if (!mCuePointParamsNode) {
			return cuePointList;
		}

		for (size_t count = mCuePointParamsNode->ChildCount(), idx = 1; idx <= count; ++idx) {
			spIStructureNode cuePointParam = mCuePointParamsNode->GetNodeAtIndex(idx)->ConvertToStructureNode();
			spcISimpleNode keyNode = cuePointParam->GetSimpleNode(kXMP_NS_DM, kXMP_NS_DM_LEN, kKey, strlen(kKey));
			spcISimpleNode valueNode = cuePointParam->GetSimpleNode(kXMP_NS_DM, kXMP_NS_DM_LEN, kValue, strlen(kValue));
			spcIUTF8String keyNodeValue = (keyNode->GetValue());
			spcIUTF8String valueNodeValue = (valueNode->GetValue());
			cuePointList->push_back(std::make_pair(keyNodeValue, valueNodeValue));
		}
		return cuePointList;
	}

	sizet MarkerImpl::RemoveCuePointParams_I(const char * inputKey /* = NULL */, sizet inputKeyLen, const char * inputValue /* = NULL */, sizet inputValueLen) {

		if (!mCuePointParamsNode) {

			return 0;
		}
		sizet count = 0;
		bool inputKeyNonNull = inputKey ? true : false;
		bool inputValueNonNull = inputValue ? true : false;
		for (size_t count = mCuePointParamsNode->ChildCount(), idx = 1; idx <= count; ++idx) {
			if (!mCuePointParamsNode->GetNodeAtIndex(idx)) continue;
			spcIStructureNode cuePointParam = mCuePointParamsNode->GetNodeAtIndex(idx)->ConvertToStructureNode();
			spcISimpleNode keyNode = cuePointParam->GetSimpleNode(kXMP_NS_DM, kXMP_NS_DM_LEN, kKey, strlen(kKey));
			spcISimpleNode valueNode = cuePointParam->GetSimpleNode(kXMP_NS_DM, kXMP_NS_DM_LEN, kValue, strlen(kValue));
			bool eraseIndex = false;
			if ( !inputKeyNonNull ){

				mCuePointParamsNode->RemoveNodeAtIndex(idx);
				eraseIndex = true;
			}
			else {
				if (strcmp(keyNode->GetValue()->c_str(), inputKey)) 
					continue;
				}
				if (!inputValueNonNull)  {
					mCuePointParamsNode->RemoveNodeAtIndex(idx);
					eraseIndex = true;
				}
				else if (!strcmp(valueNode->GetValue()->c_str(), inputValue)) {
					mCuePointParamsNode->RemoveNodeAtIndex(idx);
					eraseIndex = true;
				}
				
				count += eraseIndex;
			}

		return count;
	}


	sizet MarkerImpl::AddCuePointParam_I(const char * inputKey /* = NULL */, sizet inputKeyLen, const char * inputValue /* = NULL */, sizet inputValueLen) {

		auto defaultMap = INameSpacePrefixMap::GetDefaultNameSpacePrefixMap();
		if (!inputKey) {
			/*	NOTIFY_WARNING(IError_base::kDomainGeneral, IError_base::kGeneralCodeParametersNotAsExpected,
					"Parameters to Remove custom type are not as expected",
					true, (void *)inputKey,
					inputKey, inputKey);*/
			if (mCuePointParamsNode)
			return mCuePointParamsNode->ChildCount();
			return 0;
		}
		if (!inputValue) {

			//NOTIFY_WARNING(IError_base::kDomainGeneral, IError_base::kGeneralCodeParametersNotAsExpected,
			//	"Parameters to Remove custom type are not as expected",
			//	true, (void *)inputValue,
			//	inputValue, inputValue);
			return mCuePointParamsNode->ChildCount();
		}


		if (!mCuePointParamsNode) {

			mCuePointParamsNode = IArrayNode::CreateOrderedArrayNode(kXMP_NS_DM, kXMP_NS_DM_LEN, kCuePointParams, strlen(kCuePointParams));
			mRootNode->AppendNode(mCuePointParamsNode);
		}
		SetTypes_I(kMTCue);
		spIStructureNode cuePointParam = IStructureNode::CreateStructureNode(kXMP_NS_DM, kXMP_NS_DM_LEN, kCuePointParams, strlen(kCuePointParams));
		spISimpleNode keyNode = ISimpleNode::CreateSimpleNode(kXMP_NS_DM, kXMP_NS_DM_LEN, kKey, strlen(kKey), inputKey, inputKeyLen);
		spISimpleNode valueNode = ISimpleNode::CreateSimpleNode(kXMP_NS_DM, kXMP_NS_DM_LEN, kValue, strlen(kValue), inputValue, inputValueLen);
		cuePointParam->AppendNode(keyNode);
		cuePointParam->AppendNode(valueNode);

		mCuePointParamsNode->AppendNode(cuePointParam);
		sizet returnValue =  mCuePointParamsNode->ChildCount();
		if (IsKeyValidExtension(inputKey)) {
			

			XMP_VarString extensionName;
			if (!GetExtensionNameFromCPPKey(inputKey, extensionName)) return returnValue;
			size_t colonPos = extensionName.find_last_of(':');
			XMP_VarString  extensionURI = extensionName.substr(0, colonPos);

			if (defaultMap->GetNameSpace(extensionURI.c_str(), AdobeXMPCommon::npos)) {
				extensionURI = defaultMap->GetNameSpace(extensionURI.c_str(), AdobeXMPCommon::npos)->c_str();
			}

			XMP_VarString  extensionPartName = extensionName.substr(colonPos + 1);
			if (mExtension && !mExtension->GetNode(extensionURI.c_str(), extensionURI.length(), extensionPartName.c_str(), extensionPartName.length())) return returnValue;
			mCuePointParamsMap[inputKey].push_back(mCuePointParamsNode->ChildCount());
			CreateExtensionNodeIfRequired();
			spIStructureNode extensionNodeToBeAdded = IStructureNode::CreateStructureNode(extensionURI.c_str(), AdobeXMPCommon::npos, extensionPartName.c_str(), AdobeXMPCommon::npos);
			if (CreateExtensionNode(extensionNodeToBeAdded, inputValue, "&quot;") || CreateExtensionNode(extensionNodeToBeAdded, inputValue, "\"")) {

				if (extensionNodeToBeAdded) {
					MergeNodes(mExtension, extensionNodeToBeAdded);
				}
			}
		}
		return mCuePointParamsNode->ChildCount();
	}

	AdobeXMPCore::spIStructureNode MarkerImpl::AddExtension_I(const char * extensionNameSpace, const char * extensionName) {

		if (!extensionNameSpace) {
		/*	NOTIFY_WARNING(IError_base::kDomainGeneral, IError_base::kGeneralCodeParametersNotAsExpected,
				"Parameters to AddExtension are not as expected",
				true, (void *)extensionNameSpace,
				extensionNameSpace, extensionNameSpace);*/

			return spIStructureNode();
		}
		if (!extensionName) {

			/*NOTIFY_WARNING(IError_base::kDomainGeneral, IError_base::kGeneralCodeParametersNotAsExpected,
				"Parameters to AddExtension are not as expected",
				true, (void *)extensionName,
				extensionName, extensionName);*/
			
			return spIStructureNode();
		}
		if (!mExtension) {

			mExtension = IStructureNode::CreateStructureNode(kXMP_NS_DM, kXMP_NS_DM_LEN, kExtensions, strlen(kExtensions));
			mRootNode->AppendNode(mExtension);
		}
		spIStructureNode extension = IStructureNode::CreateStructureNode(extensionNameSpace, strlen(extensionNameSpace), extensionName, strlen(extensionName));

		mExtension->AppendNode(extension);
		return mExtension;
	}

	bool MarkerImpl::AddExtension_I(const char * extensionNameSpace, sizet extensionNameSpaceLen, const char * extensionName, sizet extensionNameLen, pIExtension extension) {

		if (extension) {
			pIExtensionHandler handler = IExtensionHandlerRegistry::GetInstance()->GetHandler( extensionNameSpace, extensionName);
			if (handler) {
				CreateExtensionNodeIfRequired();
				std::string combinedString = GetCombinedString( extensionNameSpace, extensionName);
				mExtensionMap[combinedString] = extension;
	/*			customData->SetParentNode(mwpParentNode.lock());*/
				mExtension->RemoveNode( extensionNameSpace, extensionNameSpaceLen, extensionName, extensionNameLen);
				pIExtensionHandler_I serializerHandler = IExtensionHandler_I::CreateSerializerHandler(extension, extensionNameSpace, extensionName, mExtension);
				handler->Serialize(extension, serializerHandler);

				spINode extensionChildNode = mExtension->GetNode(extensionNameSpace, extensionNameSpaceLen, extensionName, extensionNameLen);


				XMP_VarString expectedKey, serializedJSON;
				bool isSerializable = SerializeExtensionAsJSON(extensionChildNode, expectedKey, serializedJSON);
				if (!isSerializable) return true;
				RemoveCuePointParams_I(expectedKey.c_str(), expectedKey.length());
				AddCuePointParam_I(expectedKey.c_str(), expectedKey.length(), serializedJSON.c_str(), serializedJSON.length());
                /*fix to Coverity CT-4170441 to avoid resource leak*/
                serializerHandler=NULL;
				return true;
			}
		}
		return false;
	}

	AdobeXMPCore::spIStructureNode MarkerImpl::GetExtension_I(const char * extensionNameSpace, const char * extensionName) {

		if (!extensionNameSpace) {
			/*NOTIFY_WARNING(IError_base::kDomainGeneral, IError_base::kGeneralCodeParametersNotAsExpected,
				"Parameters to GetExtension are not as expected",
				true, (void *)extensionNameSpace,
				extensionNameSpace, extensionNameSpace);*/

			return spIStructureNode();
		}
		if (!extensionName) {

			/*NOTIFY_WARNING(IError_base::kDomainGeneral, IError_base::kGeneralCodeParametersNotAsExpected,
				"Parameters to GetExtension are not as expected",
				true, (void *)extensionName,
				extensionName, extensionName);*/

			return spIStructureNode();
		}

		spIStructureNode extension = mExtension->GetNode(extensionNameSpace, strlen(extensionNameSpace), extensionName, strlen(extensionName))->ConvertToStructureNode();
		return (extension);
	}

	AdobeXMPAM::pIExtension MarkerImpl::GetExtension_I(const char * extensionNameSpace, sizet extensionNameSpaceLen, const char * extensionName, sizet extensionNameLen) const {

		std::string combinedString = GetCombinedString(extensionNameSpace, extensionName);
		auto it = mExtensionMap.find(combinedString);
		if (it != mExtensionMap.end())
			return it->second;
		else {
			auto handler = IExtensionHandlerRegistry::GetInstance()->GetHandler(extensionNameSpace, extensionName);
			auto node = mExtension->GetStructureNode(extensionNameSpace, extensionNameSpaceLen, extensionName, extensionNameLen);
			if (handler && node) {
				pIExtension returnValue = ParseNode(handler, node);
				mExtensionMap[combinedString] = returnValue;
				/*returnValue->SetParentNode(mwpParentNode.lock());*/
				return returnValue;
			}
		}
		return NULL;
	}


	AdobeXMPCore::spIStructureNode MarkerImpl::RemoveExtension_I(const char * extensionNameSpace, const char * extensionName) {


		if (!extensionNameSpace) {
		/*	NOTIFY_WARNING(IError_base::kDomainGeneral, IError_base::kGeneralCodeParametersNotAsExpected,
				"Parameters to GetExtension are not as expected",
				true, (void *)extensionNameSpace,
				extensionNameSpace, extensionNameSpace);*/

			return spIStructureNode();
		}
		if (!extensionName) {

			/*NOTIFY_WARNING(IError_base::kDomainGeneral, IError_base::kGeneralCodeParametersNotAsExpected,
				"Parameters to GetExtension are not as expected",
				true, (void *)extensionName,
				extensionName, extensionName);*/

			return spIStructureNode();
		}

		spIStructureNode extension = mExtension->GetNode(extensionNameSpace, strlen(extensionNameSpace), extensionName, strlen(extensionName))->ConvertToStructureNode();
		mExtension->RemoveNode(extensionNameSpace, strlen(extensionNameSpace), extensionName, strlen(extensionName));
		return extension;
	}

	AdobeXMPCommon::sizet MarkerImpl::RemoveExtension_I(const char * extensionNameSpace, sizet extensionNameSpaceLen, const char * extensionName, sizet extensionNameLen) {

		sizet returnValue = (mExtension) ? mExtension->ChildCount() : 0;
		if (!returnValue) return returnValue;
		std::string combinedString = GetCombinedString(extensionNameSpace, extensionName);
		if (mExtensionMap.count(combinedString))
			mExtensionMap.erase(mExtensionMap.find(combinedString));
		spINode extensionChildNode = mExtension->GetNode(extensionNameSpace, extensionNameSpaceLen, extensionName, extensionNameLen);
		if (mExtension) mExtension->RemoveNode(extensionNameSpace, extensionNameSpaceLen, extensionName, extensionNameLen);
		XMP_VarString expectedKey, serializedJSON;
		bool isSerializable = SerializeExtensionAsJSON(extensionChildNode, expectedKey, serializedJSON);
		if (!isSerializable) return returnValue;
		RemoveCuePointParams_I(expectedKey.c_str(), expectedKey.length());
		return returnValue;
	}



void MarkerImpl::GetNodeQualifiedName(const spcINode & node, std::string & qualifiedName) {

	using namespace AdobeXMPCore_Int;
	qualifiedName.clear();
	

	qualifiedName.append(node->GetNameSpace()->c_str());
	qualifiedName.append(":");
	qualifiedName.append(node->GetName()->c_str());

}

bool MarkerImpl::IsKeyValidExtension(const std::string & cppKey) {

	XMP_StringLen cppKeyLen = cppKey.length();
	if (cppKey.find_first_of(extensionPrefix) != 0) return false;
	return true;

}

bool SplitNameSpaceAndName(const std::string & qualifiedName, std::string & nameSpacePrefix, std::string & name, bool needPrefix = true) {

	size_t colonPos = qualifiedName.find_last_of(":");
	if (colonPos == std::string::npos) return false;
	if (needPrefix) {
		spcINameSpacePrefixMap defaultMap = INameSpacePrefixMap::GetDefaultNameSpacePrefixMap();
//		XMP_StringPtr nsPrefix;
		spcIUTF8String nsPrefix = defaultMap->GetPrefix(qualifiedName.substr(0, colonPos).c_str(), qualifiedName.substr(0, colonPos).length());
		bool nsFound = nsPrefix ? true : false;// sRegisteredNamespaces->GetPrefix(qualifiedName.substr(0, colonPos).c_str(), &nsPrefix, 0);
		if (!nsFound) return false;
		nameSpacePrefix = nsPrefix->c_str();
	}
	else {
		nameSpacePrefix = qualifiedName.substr(0, colonPos);
	}
	name = qualifiedName.substr(colonPos + 1);
	return true;
}

bool MarkerImpl::GetExtensionNameFromCPPKey(const std::string &extensionAsKey, std::string &extensionName) {


	XMP_Index colonPos = extensionAsKey.find_last_of(":");
	if (colonPos == XMP_VarString::npos) {
		return false;
	}
	extensionName = extensionAsKey.substr(extensionPrefix.length(), colonPos - extensionPrefix.length());
	spcINameSpacePrefixMap defaultMap = INameSpacePrefixMap::GetDefaultNameSpacePrefixMap();
	spcIUTF8String nsPrefix = defaultMap->GetPrefix(extensionName.c_str(), extensionName.length());
	bool nsFound = nsPrefix ? true : false;// sRegisteredNamespaces->GetPrefix(extensionName.c_str(), &nsPrefix, 0);
	if (!nsFound) return false;
	extensionName = nsPrefix->c_str();
	extensionName.append(":"+extensionAsKey.substr(colonPos + 1));
	return true;
}

bool MarkerImpl::CreateExtensionNode(spIStructureNode xmpNode, const std::string & serializedJSON, const std::string & doubleQuotesStr) {

	// TODO  handle quotes within URI, quotes within value
	
	try {

		doubleQuotes = doubleQuotesStr;
		XMP_Index jsonIter = 1;
		size_t jsonLen = serializedJSON.length();
		if (!(jsonLen > 1)) throw 1;
		if (!(serializedJSON[0] == '{' && serializedJSON[jsonLen - 1] == '}')) throw 1;

		while (jsonIter + 1 < jsonLen) {

			if (!(serializedJSON.find(doubleQuotes, jsonIter) == jsonIter)) throw 1;
			jsonIter += doubleQuotes.length();
			size_t qualifiedNameEnd = serializedJSON.find(doubleQuotes, jsonIter);
			if (!(qualifiedNameEnd != std::string::npos)) throw 1;
			XMP_VarString childNameSpace, childName;
			bool isSplit = SplitNameSpaceAndName(serializedJSON.substr(jsonIter, qualifiedNameEnd - jsonIter), childNameSpace, childName, false);
			if (!isSplit) throw 1;
			XMP_VarString childPrefixName = childNameSpace + childName;
			jsonIter = qualifiedNameEnd + doubleQuotes.length();

			if (!(serializedJSON[jsonIter] == ':')) throw 1;

			jsonIter++;
			if (!(serializedJSON.find(doubleQuotes, jsonIter) == jsonIter)) throw 1;
			jsonIter += doubleQuotes.length();

			size_t valueEnd = serializedJSON.find(doubleQuotes, jsonIter);

			if (!(qualifiedNameEnd != std::string::npos)) throw 1;

			spISimpleNode newChildNode = ISimpleNode::CreateSimpleNode(childNameSpace.c_str(), childNameSpace.length(), childName.c_str(), childName.length());

			newChildNode->SetValue(serializedJSON.substr(jsonIter, valueEnd - jsonIter).c_str(), serializedJSON.substr(jsonIter, valueEnd - jsonIter).length());

			jsonIter = valueEnd + doubleQuotes.length();
			xmpNode->AppendNode(newChildNode);
			if (!(jsonIter == jsonLen - 1 || serializedJSON[jsonIter] == ',')) throw 1;
			jsonIter++;
		}

	}
	catch (...) {
		xmpNode->Clear();

		return false;
	}

	return true;

}

bool MarkerImpl::IsExtensionValidForBackwardCompatibility(spINode extensionNode) {

	if (!extensionNode) return false;
	if (extensionNode->HasQualifiers()) return false;
	if (extensionNode->GetNodeType() != INode_I::kNTStructure) return false;

	spINodeIterator childIter = extensionNode->ConvertToStructureNode()->Iterator();

	for (; childIter; childIter = childIter->Next()) {

		spcINode childNode = childIter->GetNode();
		if (childNode->GetNodeType() != INode::kNTSimple) return false;
		if (childNode->HasQualifiers()) return false;
	}

	return true;
}

void MarkerImpl::CreateStringForSerialization(std::string & inputStr) {

	inputStr = "&quot;" + inputStr + "&quot;";
}



	void MarkerImpl::CloneContents(spIMarker_I & marker) const {

		if (!mRootNode) return;


		spINode clonedNode = mRootNode->Clone(); 
		MarkerImpl * that = dynamic_cast<MarkerImpl *>(marker.get());
		(that)->mRootNode = clonedNode->ConvertToStructureNode();
		if (!that->mRootNode) return;
		spIStructureNode mThatRootNode = that->mRootNode;
		(that)->mTypeNode = mThatRootNode->GetSimpleNode(kXMP_NS_DM, kXMP_NS_DM_LEN, kType, strlen(kType));
		(that)->mCommentNode = mThatRootNode->GetSimpleNode(kXMP_NS_DM, kXMP_NS_DM_LEN, kComment, strlen(kComment));
		(that)->mLocationNode = mThatRootNode->GetSimpleNode(kXMP_NS_DM, kXMP_NS_DM_LEN, kLocation, strlen(kLocation));
		(that)->mSpeakerNode = mThatRootNode->GetSimpleNode(kXMP_NS_DM, kXMP_NS_DM_LEN, kSpeaker, strlen(kSpeaker));
		(that)->mTargetNode = mThatRootNode->GetSimpleNode(kXMP_NS_DM, kXMP_NS_DM_LEN, kTarget, strlen(kTarget));
		(that)->mProbabilityNode = mThatRootNode->GetSimpleNode(kXMP_NS_DM, kXMP_NS_DM_LEN, kProbability, strlen(kProbability));
		(that)->mCuePointTypeNode = mThatRootNode->GetSimpleNode(kXMP_NS_DM, kXMP_NS_DM_LEN, kCuePointType, strlen(kCuePointType));
		(that)->mCuePointParamsNode = mThatRootNode->GetArrayNode(kXMP_NS_DM, kXMP_NS_DM_LEN, kCuePointParams, strlen(kCuePointParams));
		(that)->mExtension = mThatRootNode->GetStructureNode(kXMP_NS_XMP, kXMP_NS_DM_LEN, kExtensions, strlen(kExtensions));
		(that)->mStartTimeNode = mThatRootNode->GetSimpleNode(kXMP_NS_DM, kXMP_NS_DM_LEN, kStartTime, strlen(kStartTime));
		(that)->mDurationNode = mThatRootNode->GetSimpleNode(kXMP_NS_DM, kXMP_NS_DM_LEN, kDuration, strlen(kDuration));
		(that)->mGUIDNode = mThatRootNode->GetSimpleNode(kXMP_NS_DM, kXMP_NS_DM_LEN, kGUID, strlen(kGUID));
		(that)->mNameNode = mThatRootNode->GetSimpleNode(kXMP_NS_DM, kXMP_NS_DM_LEN, kMarkerName, strlen(kMarkerName));
		if (mGUIDNode) {
			std::string guidStr;
			GenerateGUID(guidStr);
			(that)->mGUIDNode->SetValue(guidStr.c_str(), guidStr.length());
		}

		(that)->mTypeMask = mTypeMask;
		for (size_t idx = 0; idx < mCustomTypes.size(); ++idx) {
			spIUTF8String clonedString = IUTF8String_I::CreateUTF8String(mCustomTypes[idx]->c_str(), mCustomTypes[idx]->size());
			(that)->mCustomTypes.push_back((clonedString));
		}
		(that)->mFrameRateForCountOfFrames = mFrameRateForCountOfFrames;
		(that)->mFrameRateForStartFrameCount = mFrameRateForStartFrameCount;
		(that)->mFrameDuration = mFrameDuration;
		(that)->mExtensionMap = mExtensionMap;
	}

	spIMarker MarkerImpl::Clone_I() const {
		
	  //TODO
	  // use mutex
		spIMarker_I sp = spIMarker_I();
		if (mCustomTypes.size() > 0)
			sp = IMarker_I::CreateMarker_I(mNameNode->GetValue()->c_str(), mNameNode->GetValue()->size(), mTypeMask, mCustomTypes[0]->c_str(), mCustomTypes[0]->size());
		else
			sp = IMarker_I::CreateMarker_I(mNameNode->GetValue()->c_str(), mNameNode->GetValue()->size(), mTypeMask);
		auto implPointer = dynamic_cast<MarkerImpl *>( sp.get());
		CloneContents(sp);
		//	implPointer->InitializeContents();
		
		return sp;
	}

	void MarkerImpl::SetFrameCountSpan_I(uint64 startFrameCount, uint64 countOfFrames) {

		mFrameDuration.first = startFrameCount;
		mFrameDuration.second = countOfFrames;
		UpdateFrameInformationInDOM();

	}

	void MarkerImpl::GetTimeSpan(double & startTimeInSeconds, double & durationInSeconds) const {
		uint64 numerator = 1, denominator = 1, startFrameNumber = 0, countOfFrames = 1;
		GetFrameCountSpan(startFrameNumber, countOfFrames);
		GetFrameRateForStartFrameCount(numerator, denominator);
		double dStartFrameNumber = static_cast< double >(startFrameNumber), dCountOfFrames = static_cast< double >(countOfFrames);
		startTimeInSeconds = dStartFrameNumber * denominator / numerator;
		if (countOfFrames == Max_XMP_Uns64) {
			durationInSeconds = DBL_MAX;
		}
		else {
			GetFrameRateForCountOfFrames(numerator, denominator);
			durationInSeconds = dCountOfFrames * denominator / numerator;
		}
	}

	void MarkerImpl::GetFrameCountSpan(uint64 & startFrameCount, uint64 & countOfFrames) const {
		startFrameCount = mFrameDuration.first;
		countOfFrames = mFrameDuration.second;
	}

	void MarkerImpl::GetFrameRateForStartFrameCount(uint64 & numerator, uint64 & denominator) const {
		numerator = mFrameRateForStartFrameCount.first;
		denominator = mFrameRateForStartFrameCount.second;
	}

	void MarkerImpl::GetFrameRateForCountOfFrames(uint64 & numerator, uint64 & denominator) const {
		numerator = mFrameRateForCountOfFrames.first;
		denominator = mFrameRateForCountOfFrames.second;
	}

	void MarkerImpl::SetTimeSpan_I(double startTimeInSeconds, double durationInSeconds) {
		
		if (startTimeInSeconds < 0.0 || durationInSeconds <= 0.0)
			return;
		static const double kDoubleEpsilon = 0.00000000001;

		uint64 numerator = 1, denominator = 1;
		uint64 startFrameCount = 0;
		if (fabs(startTimeInSeconds) >= kDoubleEpsilon) {
			GetFrameRateForStartFrameCount(numerator, denominator);
			double dStartFrameCount = startTimeInSeconds * numerator / denominator;
			startFrameCount = static_cast<uint64>(floor(dStartFrameCount));
		}

		uint64 countOfFrames = Max_XMP_Uns64;
		if (fabs(durationInSeconds - DBL_MAX) >= kDoubleEpsilon) {
			GetFrameRateForCountOfFrames(numerator, denominator);
			double dCountOfFrames = durationInSeconds * numerator / denominator;
			countOfFrames = static_cast<uint64>(ceil(dCountOfFrames));
		}

		SetFrameCountSpan_I(startFrameCount, countOfFrames);
		return;
	}

	void MarkerImpl::SetFrameRateForStartFrameCount_I(uint64 numerator, uint64 denominator) {
		if (numerator == 0 || denominator == 0) {
			/*NOTIFY_ERROR(IError_v1::kDomainGeneral, IError_v1::kGeneralCodeParametersNotAsExpected,
				"numerator, denominator or both can't be zero", IError_v1::kSeverityOperationFatal,
				true, numerator, true, denominator);*/
			return;
		}
		mFrameRateForStartFrameCount.first = numerator;
		mFrameRateForStartFrameCount.second = denominator;
		UpdateFrameInformationInDOM();
		return;
	}

	void MarkerImpl::SetFrameRateForCountOfFrames_I(uint64 numerator, uint64 denominator) {
		if (numerator == 0 || denominator == 0) {
		/*	NOTIFY_ERROR(IError_v1::kDomainGeneral, IError_v1::kGeneralCodeParametersNotAsExpected,
				"numerator, denominator or both can't be zero", IError_v1::kSeverityOperationFatal,
				true, numerator, true, denominator);*/
			return;
		}
		mFrameRateForCountOfFrames.first = numerator;
		mFrameRateForCountOfFrames.second = denominator;
		UpdateFrameInformationInDOM();
		return;
	}

	void MarkerImpl::SetFrameRates_I(uint64 numerator, uint64 denominator) {
			if (numerator == 0 || denominator == 0) {
			/*	NOTIFY_ERROR(IError_v1::kDomainGeneral, IError_v1::kGeneralCodeParametersNotAsExpected,
					"numerator, denominator or both can't be zero", IError_v1::kSeverityOperationFatal,
					true, numerator, true, denominator);*/
				return;
				}
		
			mFrameRateForCountOfFrames.first = numerator;
			mFrameRateForCountOfFrames.second = denominator;
			mFrameRateForStartFrameCount.first = numerator;
			mFrameRateForStartFrameCount.second = denominator;
			UpdateFrameInformationInDOM();
			return ;
	}

	double MarkerImpl::GetProbability() const {

		if (mProbabilityNode) {

			std::string returnValue = mProbabilityNode->GetValue()->c_str();
            return TemporalAssetUtilities::ConvertStringToDouble(returnValue);
		}

		return 0.0;
	}

	void MarkerImpl::SetProbability_I(double markerProbability) {

		double epsilon = 1e-10;
		if  ( !( markerProbability <= 1.0 + epsilon && markerProbability >= 0 - epsilon )) {
		/*	NOTIFY_WARNING(IError_base::kDomainGeneral, IError_base::kGeneralCodeParametersNotAsExpected,
				"Parameters to GetExtension are not as expected",
				true, markerProbability,
				markerProbability, markerProbability);*/
			if (mProbabilityNode) {
				mProbabilityNode->Clear();
				mProbabilityNode = spISimpleNode();
				mRootNode->RemoveNode(kXMP_NS_DM, kXMP_NS_DM_LEN, kProbability, strlen(kProbability));
				
			}

			return;
			
		}
		std::ostringstream strs;
		strs << markerProbability;
		std::string str = strs.str();
		SetTypes_I(kMTCue);
		if (!mProbabilityNode) {
			
			mProbabilityNode = ISimpleNode::CreateSimpleNode(kXMP_NS_DM, kXMP_NS_DM_LEN, kProbability, strlen(kProbability), str.c_str(), str.length());
			mRootNode->AppendNode(mProbabilityNode);

		}
		else {

			mProbabilityNode->SetValue(str.c_str(), str.length());
		}
		return;
	}

	spcIUTF8String MarkerImpl::GetLocation_I() const {

		if (!mLocationNode) {

			return spcIUTF8String();
		}
		else {

			return  (mLocationNode->GetValue());
		}
	}

	spcIUTF8String MarkerImpl::GetSpeaker_I() const {

		if (!mSpeakerNode) {

			return spcIUTF8String();
		}
		else {

			return (mSpeakerNode->GetValue());
		}
		
	}

	spcIUTF8String MarkerImpl::GetTarget_I() const {
		
		if (!mTargetNode) {

			return spcIUTF8String();
		}
		else {

			return (mTargetNode->GetValue());
		}
	}

	spcIUTF8String MarkerImpl::GetComment_I() const {
		
		if (!mCommentNode) {

			return spcIUTF8String();
		}
		else {

			return  (mCommentNode->GetValue());
		}
		
	}

	spcIUTF8String MarkerImpl::GetGUID_I() const {
		
		return  (mGUIDNode->GetValue());
	}

	void MarkerImpl::SetLocation_I(const char * name, sizet locationLen) {
		if (!name) {
		/*	NOTIFY_WARNING(IError_base::kDomainGeneral, IError_base::kGeneralCodeParametersNotAsExpected,
				"Parameters to SetComment are not as expected",
				true, (void *)name,
				name, name);*/
			RemoveLocationNode();
			return;
		}
		if (!mLocationNode) {
			mLocationNode = ISimpleNode::CreateSimpleNode(kXMP_NS_DM, kXMP_NS_DM_LEN, kLocation, strlen(kLocation));
			mRootNode->AppendNode(mLocationNode);
		}

		mLocationNode->SetValue(name, locationLen);

		return;
	}

	void MarkerImpl::SetSpeaker_I(const char * speaker, sizet speakerLen) {
		
		if (!speaker) {
			/*NOTIFY_WARNING(IError_base::kDomainGeneral, IError_base::kGeneralCodeParametersNotAsExpected,
				"Parameters to SetSpeaker are not as expected",
				true, (void *)speaker,
				speaker, speaker);*/

			RemoveSpeakerNode();
			return;
		}
		SetTypes_I(kMTSpeech);
		if (!mSpeakerNode) {
			mSpeakerNode = ISimpleNode::CreateSimpleNode(kXMP_NS_DM, kXMP_NS_DM_LEN, kSpeaker, strlen(speaker));
			mRootNode->AppendNode(mSpeakerNode);
		}
		mSpeakerNode->SetValue(speaker, speakerLen);

		return;
	}

	void MarkerImpl::SetComment_I(const char * name, sizet commentLen) {
		if (!name) {
			/*NOTIFY_WARNING(IError_base::kDomainGeneral, IError_base::kGeneralCodeParametersNotAsExpected,
				"Parameters to SetComment are not as expected",
				true, (void *)name,
				name, name);*/
			RemoveCommentNode();
			return;
		}
		SetTypes_I(kMTComment);
		if (!mCommentNode) {
			mCommentNode = ISimpleNode::CreateSimpleNode(kXMP_NS_DM, kXMP_NS_DM_LEN, kComment, strlen(kComment));
			mRootNode->AppendNode(mCommentNode);
		}
		mCommentNode->SetValue(name, commentLen);

		return;
	}

	void MarkerImpl::SetTarget_I(const char * name, sizet targetLen) {
		

		if (!name) {
			/*NOTIFY_WARNING(IError_base::kDomainGeneral, IError_base::kGeneralCodeParametersNotAsExpected,
				"Parameters to SetTarget are not as expected",
				true, (void *)name,
				name, name);*/
			RemoveTargetNode();
			return;
		}
		if (!mTargetNode) {
			mTargetNode = ISimpleNode::CreateSimpleNode(kXMP_NS_DM, kXMP_NS_DM_LEN, kTarget, strlen(kTarget));
			mRootNode->AppendNode(mTargetNode);
		}
		mTargetNode->SetValue(name, targetLen);

		return;
	}

	void MarkerImpl::SetGUID_I(const char *guid) {

		mGUIDNode->SetValue(guid, strlen(guid));
	}

	void MarkerImpl::SetExtension_I(spINode extension) {

		if (!extension) mExtension = spIStructureNode();
		mExtension = extension->ConvertToStructureNode();
	}


	void MarkerImpl::Serialize_I(spIStructureNode & structNode) {

		structNode = mRootNode;
		return;
	}

	weak_ptr<ITrack_I> MarkerImpl::GetAssociatedTrack_I() const {

		return mTrack;
	}

	void MarkerImpl::SetAssociatedTrack_I(spITrack_I track) {

		mTrack = track;
	}

	AdobeXMPCommon::sizet MarkerImpl::RemoveAllExtensions() __NOTHROW__ {

		sizet returnValue = (mExtension) ? mExtension->ChildCount() : 0;
		mExtensionMap.clear();
		mExtension->Clear();
		RemoveExtensionNode();
		return returnValue;
	}

	AdobeXMPCommon::sizet MarkerImpl::GetExtensionCount() const __NOTHROW__{

		return mExtension ? mExtension->ChildCount() : 0;
	}

	bool MarkerImpl::HasExtension_I(const char * extensionNameSpace, sizet extensionNameSpaceLen, const char * extensionName, sizet extensionNameLen) const __NOTHROW__{
		
		spINode extensionNode = mExtension->GetNode(extensionNameSpace, extensionNameSpaceLen, extensionName, extensionNameLen);
		
		return (mExtension) ? true : false;

	}

	AdobeXMPCommon::sizet MarkerImpl::RemoveAllCuePointParams_I() {

		//TODO remove type if required
		sizet returnValue = mCuePointParamsNode ? mCuePointParamsNode->ChildCount() : 0;

		if (!mCuePointParamsNode) {

			return returnValue;
		}
		mCuePointParamsNode->Clear();
		RemoveCuePointParamsNode();
		return returnValue;
	}

	bool MarkerImpl::HasCustomType_I(const char * customType, sizet customTypeLen) const {

		if (!customType) return false;
		if (mCustomTypeMap.find(customType) != mCustomTypeMap.end()) {
			return true;
		}
		return false;
	}
}

namespace AdobeXMPAM {
	using namespace AdobeXMPAM_Int;
	spIMarker IMarker_v1::MakeShared(pIMarker_base ptr) {
		pIMarker p = IMarker::GetInterfaceVersion() > 1 ? ptr->GetInterfacePointer< IMarker >() : ptr;
		return MakeUncheckedSharedPointer(p, __FILE__, __LINE__, false);
	}
}
