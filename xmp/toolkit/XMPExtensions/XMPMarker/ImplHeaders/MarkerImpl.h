#ifndef MarkerImpl_h__
#define MarkerImpl_h__ 1

// =================================================================================================
// ADOBE SYSTEMS INCORPORATED
// Copyright 2014 Adobe Systems Incorporated
// All Rights Reserved
//
// NOTICE:  Adobe permits you to use, modify, and distribute this file in accordance with the terms
// of the Adobe license agreement accompanying it.
// =================================================================================================

#if AdobePrivate
// =================================================================================================
// Change history
// ==============
//
// Writers:
//  ADC	Amandeep Chawla
//	SKP Sunil Kishor Pathak
//
// mm-dd-yy who Description of changes, most recent first.
//
// 11-28-14 SKP 1.0-a019 Implemented Find & Remove Relationship functionality for composed Part
// 11-28-14 SKP 1.0-a018 Fixed issues in genericpart addition & history entry
// 11-27-14 SKP 1.0-a017 Implemented Asset composition related functionality for basic part addition.
// 11-26-14 ADC 1.0-a013 Added the full skeleton for IComposedAssetManager interface.
//
// =================================================================================================
#endif  // AdobePrivate

#if !(IMPLEMENTATION_HEADERS_CAN_BE_INCLUDED)
#error "Not adhering to design constraints"
// this file should only be included from its own cpp file
#endif

#include "XMPMarker/Interfaces/IMarker_I.h"

#include "XMPCommon/ImplHeaders/SharedObjectImpl.h"
#include "XMPCommon/ImplHeaders/ThreadSafeImpl.h"
#include "XMPCommon/BaseClasses/MemoryManagedObject.h"
#include "XMPCore/XMPCoreFwdDeclarations_I.h"
#include "XMPMarker/Interfaces/IExtensionHandler_I.h"
#include "XMPMarker/ImplHeaders/TemporalAssetUtilitiesImpl.h"
#include <list>
#include <map>

#include <string>
#include <iostream>
#include <sstream>
#include <map>
#include <float.h>
#include <math.h>
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


#if XMP_WinBuild
#pragma warning( push )
#pragma warning( disable : 4250 )
#endif

namespace AdobeXMPAM_Int {
	using namespace AdobeXMPAM_Int;
	using namespace AdobeXMPCommon;
	using namespace AdobeXMPCommon_Int;
	using namespace AdobeXMPCore_Int;


	class MarkerImpl
		: public virtual IMarker_I
		, public virtual SharedObjectImpl
		, public virtual MemoryManagedObject
	{
	public:

		MarkerImpl(const char * markerName, sizet, uint64 markerTypes, const char * markerTypeName = NULL, sizet = AdobeXMPCommon::npos, const char * markerGUID = NULL, sizet = AdobeXMPCommon::npos);

		virtual spcIUTF8String APICALL GetName_I() const;

		spIMarker_I CreateMarker_I(const char * markerName, uint64 markerTypes, const char * markerTypeName, const char * guid);

		virtual void APICALL SetName_I(const char * name, sizet nameLen);
		

		virtual uint64 APICALL SetTypes_I(uint64 inputMask);

		virtual uint64 APICALL RemoveTypes_I(uint64 inputMask);

		virtual uint64 APICALL GetTypes_I() const;

		virtual bool APICALL AppendCustomType_I(const char * type, sizet typeLen);

		virtual bool APICALL RemoveCustomType_I(const char * type, sizet typeLen);

		virtual sizet APICALL GetCustomTypeCount_I() const ;


		virtual spcIUTF8String APICALL GetCustomType_I(sizet index) const;

		virtual spcIUTF8StringList_I APICALL GetCustomTypes_I() const ;


		virtual spcIUTF8StringList_I APICALL GetCuePointParams_I(const char * inputKey, sizet inputKeyLen) const;

		virtual sizet APICALL RemoveCuePointParams_I(const char * inputKey, sizet inputKeyLen, const char * inputValue = NULL, sizet inputValueLen = 0);

		virtual sizet APICALL AddCuePointParam_I(const char * inputKey, sizet inputKeyLen, const char * inputValue, sizet inputValueLen);

		virtual spcICuePointParamList_I APICALL GetAllCuePointParams_I() const;

		virtual spIStructureNode APICALL AddExtension_I(const char * extensionNameSpace, const char * extensionName);

		virtual spIStructureNode APICALL GetExtension_I(const char * extensionNameSpace, const char * extensionName);

		virtual spIStructureNode  APICALL RemoveExtension_I(const char * extensionNameSpace, const char * extensionName);

		virtual spIMarker APICALL Clone_I() const;

		virtual void APICALL SetFrameCountSpan_I(uint64 startFrameCount = 0, uint64 countOfFrames = Max_XMP_Uns64);

		virtual void  APICALL GetTimeSpan(double & startTimeInSeconds, double & durationInSeconds) const;

		virtual void  APICALL GetFrameCountSpan(uint64 & startFrameCount, uint64 & countOfFrames) const;

		virtual void APICALL  GetFrameRateForStartFrameCount(uint64 & numerator, uint64 & denominator) const;

		virtual void  APICALL GetFrameRateForCountOfFrames(uint64 & numerator, uint64 & denominator) const;

		virtual void APICALL SetTimeSpan_I(double startTimeInSeconds, double durationInSeconds);

		virtual void APICALL SetFrameRateForStartFrameCount_I(uint64 numerator, uint64 denominator);

		virtual void APICALL SetFrameRateForCountOfFrames_I(uint64 numerator, uint64 denominator);

		virtual void APICALL SetFrameRates_I(uint64 numerator, uint64 denominator);

		virtual double APICALL GetProbability() const;

		virtual void APICALL SetProbability_I(double markerProbability);

		virtual spcIUTF8String APICALL GetLocation_I() const;

		virtual spcIUTF8String APICALL GetSpeaker_I() const;

		virtual spcIUTF8String APICALL GetTarget_I() const;

		virtual spcIUTF8String APICALL GetComment_I() const;

		virtual spcIUTF8String APICALL GetGUID_I() const;

		virtual void APICALL SetLocation_I(const char * location, sizet locationLen);

		virtual void APICALL SetSpeaker_I(const char * speaker, sizet speakerLen);

		virtual void APICALL SetComment_I(const char * comment, sizet nameLen);

		virtual void APICALL SetTarget_I(const char * target, sizet targetLen);

		virtual void APICALL SetGUID_I(const char *guid);

		virtual void APICALL SetExtension_I(spINode extension);

		virtual void Serialize_I(spIStructureNode & node);

		virtual weak_ptr<ITrack_I> GetAssociatedTrack_I() const;

		virtual void SetAssociatedTrack_I( spITrack_I track);

		virtual void CloneContents(spIMarker_I & marker) const;

		virtual void UpdateFrameInformationInDOM();

		virtual bool APICALL AddExtension_I(const char * extensionNameSpace, sizet extensionNameSpaceLen, const char * extensionName, sizet extensionNameLen, pIExtension extension);

		virtual pIExtension GetExtension_I(const char * extensionNameSpace, sizet extensionNameSpaceLen, const char * extensionName, sizet extensionNameLen) const;

		virtual sizet APICALL RemoveAllExtensions() __NOTHROW__;

		virtual sizet APICALL GetExtensionCount() const __NOTHROW__;

		virtual bool  HasExtension_I(const char * extensionNameSpace, sizet extensionNameSpaceLen, const char * extensionName, sizet extensionNameLen) const __NOTHROW__;

		virtual sizet APICALL RemoveExtension_I(const char  * extensionNameSpace, sizet extensionNameSpaceLen, const char * extensionName, sizet extensionNameLen);

		virtual sizet RemoveAllCuePointParams_I();

		virtual bool  HasCustomType_I(const char * customType, sizet customTypeLen) const;

	protected:
		virtual ~MarkerImpl() __NOTHROW__;

	protected:

	
		std::vector<std::pair<eMarkerType, spIUTF8String>	>							mListofTypes;
		spIUTF8String																	mCommaSeparatedTypes;

		mutable  spIStructureNode														mExtension;

		std::pair<uint64, uint64>														mFrameDuration;
		std::pair<uint64, uint64>														mFrameRateForStartFrameCount;
		std::pair<uint64, uint64>														mFrameRateForCountOfFrames;
		weak_ptr<ITrack_I>																mTrack;
	


		mutable spIStructureNode														mRootNode;
		mutable spISimpleNode															mNameNode;
		mutable spISimpleNode															mTypeNode;
		mutable spISimpleNode															mGUIDNode;
		mutable spISimpleNode															mCuePointTypeNode;
		mutable spISimpleNode															mProbabilityNode;
		mutable spISimpleNode															mLocationNode;
		mutable spISimpleNode															mSpeakerNode;
		mutable spISimpleNode															mTargetNode;
		mutable spISimpleNode															mCommentNode;
		mutable spIArrayNode															mCuePointParamsNode;
		mutable spISimpleNode															mStartTimeNode, mDurationNode;
		uint64																			mTypeMask;
		std::vector<spIUTF8String>														mCustomTypes;
		std::map< std::string, bool >													mCustomTypeMap;
		mutable std::map< std::string, pIExtension >									mExtensionMap;
		std::map< std::string, std::vector<sizet> >										mCuePointParamsMap;
	
	private:
		bool IsExtensionValidForBackwardCompatibility(spINode extensionNode);
		bool SerializeExtensionAsJSON(const AdobeXMPCore::spINode & extensionNode, std::string & key, std::string& value);
		void SerializeAllExtensionsToJSONAndInsertInCuePointParams();
		bool CreateExtensionFromSerializedJSONKeyAndValue(spIStructureNode & extensionNode, const std::string &key, const std::string &value);
		void MergeNodes(const AdobeXMPCore::spIStructureNode & sourceRootNode, const AdobeXMPCore::spIStructureNode & destNode);
		void CreateExtensionNodeIfRequired();
		void RemoveSpeakerNode();
		void RemoveLocationNode();
		void RemoveProbabilityNode();
		void RemoveCommentNode();
		void RemoveCuePointParamsNode();
		void RemoveStartTimeNode();
		void RemoveDurationNode();
		void RemoveTargetNode();
		void RemoveExtensionNode();
		void RemoveNameNode();
		bool CreateExtensionNode(AdobeXMPCore::spIStructureNode xmpNode, const std::string & serializedJSON, const std::string & doubleQuotesStr);
		void GetNodeQualifiedName(const spcINode & node, std::string & qualifiedName);
		bool IsKeyValidExtension(const std::string & cppKey);
		void CreateStringForSerialization(std::string & inputStr);
		bool GetExtensionNameFromCPPKey(const std::string &extensionAsKey, std::string &extensionName);
#ifndef FRIEND_CLASS_DECLARATION
#define FRIEND_CLASS_DECLARATION() 
#endif
		FRIEND_CLASS_DECLARATION();

	};

	inline MarkerImpl::~MarkerImpl() __NOTHROW__{}
}



#if XMP_WinBuild
#pragma warning( pop )
#endif

#endif  // MarkerImpl_h__
