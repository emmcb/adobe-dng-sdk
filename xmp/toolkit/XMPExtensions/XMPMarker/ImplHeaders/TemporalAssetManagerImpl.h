#ifndef TemporalAssetManagerImpl_h__
#define TemporalAssetManagerImpl_h__ 1

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

#include "XMPMarker/Interfaces/ITemporalAssetManager_I.h"
#include "XMPCommon/ImplHeaders/SharedObjectImpl.h"
#include "XMPCommon/BaseClasses/MemoryManagedObject.h"
#include "XMPCore/XMPCoreFwdDeclarations_I.h"
#include "XMPCore/XMPCoreFwdDeclarations_I.h"
#include "XMPCore/Interfaces/IArrayNode.h"

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

	class TemporalAssetManagerImpl
		: public virtual ITemporalAssetManager_I
		, public virtual SharedObjectImpl
		, public virtual MemoryManagedObject
		, public virtual enable_shared_from_this< TemporalAssetManagerImpl >
	{
	public:
	
		TemporalAssetManagerImpl(const spIMetadata & temporalAssetMetadata);
	
		
		virtual spITrack APICALL AddTrack_I( spITrack  track);

		virtual spITrackList_I APICALL GetAllTracks_I() const;

		virtual spITrackList_I APICALL RemoveAllTracks_I();

		virtual spIMarkerList_I APICALL GetMarkersByName_I(const char * markerName, sizet markerNameLen) const;

		virtual spIMarkerList_I APICALL RemoveMarkers_I(uint64 markerTypes = UINT64_MAX, bool matchAllCriteria = false, const std::vector<std::pair<const char *, sizet> > &markerTypeName = std::vector<std::pair<const char *, sizet> >());

		virtual spIMarkerList_I APICALL GetMarkers_I(uint64 markerTypes = UINT64_MAX, bool matchAllCriteria = false, const std::vector<std::pair<const char *, sizet> > &markerTypeName = std::vector<std::pair<const char *, sizet> >()) const;

		virtual spITrackList_I APICALL RemoveTracks_I(uint64 markerTypes = UINT64_MAX, bool matchAllCriteria = false, const std::vector<std::pair<const char *, sizet> > &markerTypeName = std::vector<std::pair<const char *, sizet> >());

		virtual spITrackList_I APICALL GetTracks_I(uint64 markerTypes = UINT64_MAX, bool matchAllCriteria = false, const std::vector<std::pair<const char *, sizet> > &markerTypeName = std::vector<std::pair< const char *, sizet> >()) const;

		virtual spIMarkerList_I APICALL GetMarkersInTimeRange_I(uint64 startFrameCount = 0, uint64 endFrameCount = UINT64_MAX, ITrack::eTrackTimeBoundary timeBoundary = ITrack::kTTBothStartTimeAndEndTimeIncluded, uint64 markerTypes = UINT64_MAX, bool matchAllCriteria = false, const std::vector<std::pair<const char *, sizet> > &markerTypeName = std::vector<std::pair< const char *, sizet> >())const;


	//	virtual void AddTrack(const spcITrack & markerToBeAdded);

	//	virtual void ProcessAllChanges();

	//	virtual spcITrackList_I GetAllTracks_I() const;

	//	virtual spITemporalAsset_I Clone_I(eMultiThreadingSupport multiThreadingSupport = kMultiThreadingUseClonedObjectValue) const;

	//	virtual void SaveAllChanges_I();

	//	virtual void RemoveAllTracks_I();

	//	virtual void RemoveTrack_I(const char * trackName, IMarker::eMarkerType trackType);

	//	virtual spcIMarkerList_I GetMarkersByType_I( IMarker::eMarkerType markerType ) const ;

	//	virtual spcIMarkerList_I GetMarkersInRange_I( uint64 startFrameCount, uint64 frameCount, uint64 frameRate, uint64 frameRateBasis ) const;
	//	/*
	//	get tracks by name, type
	//	*/

	//protected:
	virtual ~TemporalAssetManagerImpl() __NOTHROW__;
		void PopulateTracks();
		void PopulateSingleTrack(spIStructureNode &trackNode);
		void PopulateSingleMarker(spIStructureNode &markerNode, spITrack_I & parentTrack);
		void InitializeContents();

	protected:

	
	
	private:
//		spISharedMutex_I				mSharedMutex;
		std::vector<spITrack_I>															mTracks;
		spIUTF8String_I																	mName;
		//uint64																			mFrameRate;
		//uint64																			mFrameRateBasis;
		spIUTF8String_I																	mType;

		mutable spIMetadata																mTemporalMetdadata;
		//TODO check actual name and values instead of shared_ptr
		std::map<std::pair<spcIUTF8String, spcIUTF8String>, spcITrack_I >				mTrackMap;

		spIArrayNode																	mRootNode;
			
		

#ifndef FRIEND_CLASS_DECLARATION
#define FRIEND_CLASS_DECLARATION() 
#endif
		FRIEND_CLASS_DECLARATION();

	};

	inline TemporalAssetManagerImpl::~TemporalAssetManagerImpl() __NOTHROW__{}
}

#if XMP_WinBuild
#pragma warning( pop )
#endif

#endif  // TemporalAssetManagerImpl_h__
