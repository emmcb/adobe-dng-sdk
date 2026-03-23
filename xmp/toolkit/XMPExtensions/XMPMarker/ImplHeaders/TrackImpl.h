#ifndef TrackImpl_h__
#define TrackImpl_h__ 1

// =================================================================================================
// ADOBE SYSTEMS INCORPORATED
// Copyright 2014 Adobe Systems Incorporated
// All Rights Reserved
//
// NOTICE:  Adobe permits you to use, modify, and distribute this file in accordance with the terms
// of the Adobe license agreement accompanying it.
// =================================================================================================
#if !(IMPLEMENTATION_HEADERS_CAN_BE_INCLUDED)
#error "Not adhering to design constraints"
// this file should only be included from its own cpp file
#endif

#include "XMPMarker/Interfaces/ITrack_I.h"

#include "XMPCommon/ImplHeaders/SharedObjectImpl.h"
#include "XMPCommon/BaseClasses/MemoryManagedObject.h"
#include "XMPCore/XMPCoreFwdDeclarations_I.h"

#include <list>
#include <map>


#if XMP_WinBuild
#pragma warning( push )
#pragma warning( disable : 4250 )
#endif

namespace AdobeXMPAM_Int {
	using namespace AdobeXMPAM_Int;
	using namespace AdobeXMPCommon;
	using namespace AdobeXMPCommon_Int;
	using namespace AdobeXMPCore_Int;

	class TrackImpl
		: public virtual ITrack_I
		, public virtual enable_shared_from_this< TrackImpl >
		, public virtual SharedObjectImpl
		, public virtual MemoryManagedObject
	{
	public:
		TrackImpl(const char * trackName, sizet trackNameLen, uint64 trackTypes, const char * trackTypeName, sizet trackTypeNameLen);

		virtual spIMarker APICALL AddMarker_I(const char * markerName, sizet markerNameLen, uint64 markerTypes, const char * markerTypeName, sizet markerTypeNameLen, const char * guid, sizet guidLen);

		virtual spIMarker APICALL AddMarker_I(spIMarker_I marker);

		virtual spITrack APICALL Clone_I() const;

		virtual spcIUTF8String APICALL GetName_I() const;

		virtual void APICALL SetName_I(const char * name, sizet nameLen);


		virtual uint64 APICALL SetTypes_I(uint64 inputMask);

		virtual uint64 APICALL RemoveTypes_I(uint64 inputMask);

		virtual uint64 APICALL GetTypes_I() const;

		virtual bool APICALL AppendCustomType_I(const char * type, sizet typeLen);

		virtual bool APICALL RemoveCustomType_I(const char * type, sizet typeLen);

		virtual sizet APICALL GetCustomTypeCount_I() const;


		virtual spcIUTF8String APICALL GetCustomType_I(sizet index) const;

		virtual spcIUTF8StringList_I APICALL GetCustomTypes_I() const;

		virtual void APICALL SetFrameRate_I(uint64 numerator = 1, uint64 denominator = 1);

		virtual spIMarker APICALL RemoveMarker_I(const sizet & markerIdx);

		virtual spIMarker APICALL RemoveMarker_I(const char * guid, sizet guidLen);

		virtual spIMarker APICALL GetMarker_I(const char * guid, sizet guidLen) const;

		virtual spIMarker APICALL GetMarker_I(const sizet & markerIdx) const;

		

		virtual spIMarkerList_I APICALL GetAllMarkers_I() const;


		virtual spIMarkerList_I APICALL GetMarkersByName_I(const char * markerName, sizet markerNameLen) const;

	

		void APICALL GetFrameRate(uint64 & numerator, uint64 & denominator) const;

		virtual sizet APICALL GetMarkerCount() const;

		void Serialize_I(spIStructureNode & structNode) const;
		
		virtual void APICALL foo_I(std::vector< std::pair< const char *, sizet> > & v);

		virtual void CloneContents(spITrack & sp) const;

		virtual spIMarkerList_I APICALL RemoveMarkers_I(uint64 markerTypes = UINT64_MAX, bool matchAllCriteria = false, const std::vector<std::pair<const char *, sizet> > &markerTypeName = std::vector<std::pair<const char *, sizet> >());

		virtual spIMarkerList_I APICALL GetMarkers_I(uint64 markerTypes = UINT64_MAX, bool matchAllCriteria = false, const std::vector<std::pair<const char *, sizet> > &markerTypeName = std::vector<std::pair<const char *, sizet> >()) const;

		virtual spIMarkerList_I APICALL GetMarkersInTimeRange_I(uint64 startFrameCount = 0, uint64 endFrameCount = UINT64_MAX, eTrackTimeBoundary timeBoundary = kTTBothStartTimeAndEndTimeIncluded, uint64 markerTypes = UINT64_MAX, bool matchAllCriteria = false, const std::vector<std::pair<const char *, sizet> > &markerTypeName = std::vector<std::pair<const char *, sizet> >()) const ;

		virtual bool APICALL HasCustomType_I(const char * customType, sizet customTypeLen) const;

		void UpdateFrameRateInformationInDOM();
		///*virtual void AddMarker_I(const spcIMarker_I & marker);

		//virtual void AddMarker(const spcIMarker & marker);

		//virtual void RemoveMarker_I(const sizet & markerIdx);

		//virtual spcIMarkerList_I GetMarkersInRange_I(uint64 startFrameCount, uint64 endFrameCount, uint64 frameRate, uint64 frameRateBasis, eTrackTimeBoundary timeBoundaries) const __NOTHROW__;

		//virtual spcIMarkerList_I GetMarkersByType_I(uint64 markerType) const __NOTHROW__ ;

		//

		//virtual void  AddTrackType_I(IMarker::eMarkerType markerType, const char * markerTypeName) ;

		//virtual spcIUTF8String_I GetTrackType_I() const;

		//virtual void RemoveAllMarkers_I();

		//virtual void RemoveMarker_I(const char * guid);

		//virtual spcITemporalAsset_I GetAssociatedTemporalAsset_I() const ;

		//virtual void SetDefaultFrameRate_I(uint64 numerator = 1, uint64 denominator = 1);

		//virtual void GetDefaultFrameRate_I(uint64 & numerator, uint64 & denominator) const __NOTHROW__;

		//virtual spcIUTF8String_I GetTrackName_I() const;

		//virtual void SetTrackName_I(const char * trackName);

		//void SerializeTrack_I(spIStructureNode & structNode) const; 

		//virtual uint64 GetTrackTypeAsBitMask_I() const ;

		//virtual spcIMarker_I GetMarker_I(const char * guid) const;*/

	protected:
		virtual ~TrackImpl() __NOTHROW__;
		bool CheckSuitability(const spIMarker_I & marker);



	protected:
	
		void RemoveMarkersNode();
		void RemoveNameNode();
		void RemoveTypeNode();
		void RemoveFrameRateNode();
		void CreateMarkersNode();
		void CreateNameNode();
		void CreateTypeNode();
		void CreateFrameRateNode();


	private:
		std::vector<std::pair<IMarker::eMarkerType, spIUTF8String>	>				mListofTypes;
		spIUTF8String																mCommaSeparatedTypes;
		spIUTF8String																mTrackName;
		spIUTF8String																trackType;
		std::vector<spIMarker_I>													markerList;
		std::map<const char *, spIMarker_I>											markerGUIDMap;
		//spITemporalAsset_I						mTemporalAsset;
		std::pair<uint64, uint64>													mDefaultFrameRate;
		spIStructureNode															mRootNode;
		spISimpleNode																mNameNode;
		spISimpleNode																mTypeNode;
		spISimpleNode																mDefaultFrameRateNode;
		spIArrayNode																mMarkerListNode;
		uint64																		mTypeMask;
		std::vector<spIUTF8String>													mCustomTypes;
		std::map< std::string, bool >												mCustomTypeMap;
		

#ifndef FRIEND_CLASS_DECLARATION
#define FRIEND_CLASS_DECLARATION() 
#endif
		FRIEND_CLASS_DECLARATION();

	};

	inline TrackImpl::~TrackImpl() __NOTHROW__{}
}

#if XMP_WinBuild
#pragma warning( pop )
#endif

#endif  // TrackImpl_h__
