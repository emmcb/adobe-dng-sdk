#ifndef ITemporalAssetManager_I_h__
#define ITemporalAssetManager_I_h__ 1

// =================================================================================================
// ADOBE SYSTEMS INCORPORATED
// Copyright 2014 Adobe Systems Incorporated
// All Rights Reserved
//
// NOTICE:  Adobe permits you to use, modify, and distribute this file in accordance with the terms
// of the Adobe license agreement accompanying it.
// =================================================================================================

#include "XMPMarker/XMPMarkerFwdDeclarations_I.h"
#include "XMPMarker/Interfaces/ITemporalAssetManager.h"
#include "XMPMarker/Interfaces/ITrack_I.h"
#include "XMPCore/XMPCoreFwdDeclarations_I.h"

#include "XMPCommon/Interfaces/BaseInterfaces/ISharedObject_I.h"

namespace AdobeXMPAM_Int {
	using namespace AdobeXMPCommon;
	using namespace AdobeXMPCommon_Int;
	using namespace AdobeXMPAM;
	/*using namespace NS_INT_XMPCORE;*/

#if XMP_WinBuild
	#pragma warning( push )
	#pragma warning( disable : 4250 )
#endif
	class ITemporalAssetManager_I
		: public virtual ITemporalAssetManager,
		 public virtual ISharedObject_I
	{
	public:

		
		static spITemporalAssetManager_I CreateTemporalAssetManager_I(const spIMetadata & metadata);

		virtual spITrack APICALL AddTrack_I( spITrack  marker) = 0;

		virtual spITrackList_I APICALL GetAllTracks_I() const = 0;

		virtual spITrackList_I APICALL RemoveAllTracks_I() = 0;

		virtual spIMarkerList_I APICALL GetMarkersByName_I(const char * markerName, sizet markerNameLen) const = 0;

		virtual spIMarkerList_I APICALL RemoveMarkers_I(uint64 markerTypes = UINT64_MAX, bool matchAllCriteria = false, const std::vector<std::pair<const char *, sizet> > &markerTypeName = std::vector<std::pair< const char *, sizet> >()) = 0;

		virtual spIMarkerList_I APICALL GetMarkers_I(uint64 markerTypes = UINT64_MAX, bool matchAllCriteria = false, const std::vector<std::pair<const char *, sizet> > &markerTypeName = std::vector<std::pair< const char *, sizet> >()) const = 0;

		virtual spITrackList_I APICALL RemoveTracks_I(uint64 markerTypes = UINT64_MAX, bool matchAllCriteria = false, const std::vector<std::pair<const char *, sizet> > &markerTypeName = std::vector<std::pair< const char *, sizet> >()) = 0;

		virtual spITrackList_I APICALL GetTracks_I(uint64 markerTypes = UINT64_MAX, bool matchAllCriteria = false, const std::vector<std::pair<const char *, sizet> > &markerTypeName = std::vector<std::pair< const char *, sizet> >()) const = 0;

		virtual spIMarkerList_I APICALL GetMarkersInTimeRange_I(uint64 startFrameCount = 0, uint64 endFrameCount = UINT64_MAX, ITrack::eTrackTimeBoundary timeBoundary = ITrack::kTTBothStartTimeAndEndTimeIncluded, uint64 markerTypes = UINT64_MAX, bool matchAllCriteria = false, const std::vector<std::pair<const char *, sizet> > &markerTypeName = std::vector<std::pair<const char *, sizet> >()) const = 0;

		static uint32 GetInterfaceVersion() { return kInternalInterfaceVersionNumber; }

		virtual pvoid APICALL GetInterfacePointer(uint64 interfaceID, uint32 interfaceVersion);

		virtual pITemporalAssetManager APICALL GetActualITemporalAssetManager() __NOTHROW__{ return this; }

		virtual pITemporalAssetManager_I APICALL GetITemporalAssetManager_I() __NOTHROW__{ return this; }

	protected:


		virtual spITrack  APICALL AddTrack(spITrack  trackToBeAdded);
		
		virtual pITrack_base  APICALL addTrack(pITrack_base  trackToBeAdded, pcIError_base & error);

		virtual sizet  APICALL getAllTracks(pITrack_base *& array, MemAllocatorFunc allocFunc, pcIError_base & error) const;

		virtual sizet  APICALL removeAllTracks(pITrack_base *& array, MemAllocatorFunc allocFunc, pcIError_base & error);

		virtual spITrackList APICALL  GetAllTracks() const;

		virtual spITrackList APICALL  RemoveAllTracks();

		virtual sizet APICALL  getMarkersByName(const char * markerName, sizet markerNameLen, pIMarker_base *& array, MemAllocatorFunc allocFunc, pcIError_base & error) const;

		virtual spIMarkerList APICALL  GetMarkersByName(const char * markerName, sizet markerNameLen) const;

		virtual spIMarkerList APICALL  RemoveMarkers(uint64 markerTypes = UINT64_MAX, bool matchAllCriteria = false, const std::vector<std::pair< const char *, sizet> > &markerTypeName = std::vector<std::pair<const char *, sizet> >());

		virtual sizet APICALL  removeMarkers(uint64 markerTypes, uint32 matchAllCriteria, void * rawptr, sizet count, pIMarker_base *&array, MemAllocatorFunc allocFunc, pcIError_base & error);

		virtual spIMarkerList  APICALL GetMarkers(uint64 markerTypes = UINT64_MAX, bool matchAllCriteria = false, const std::vector<std::pair< const char *, sizet> > &markerTypeName = std::vector<std::pair<const char *, sizet> >()) const;

		virtual sizet  APICALL getMarkers(uint64 markerTypes, uint32 matchAllCriteria, void * rawptr, sizet count, pIMarker_base *&array, MemAllocatorFunc allocFunc, pcIError_base & error) const;

		virtual spITrackList  APICALL RemoveTracks(uint64 markerTypes = UINT64_MAX, bool matchAllCriteria = false, const std::vector<std::pair< const char *, sizet> > &markerTypeName = std::vector<std::pair<const char *, sizet> >());

		virtual sizet  APICALL removeTracks(uint64 markerTypes, uint32 matchAllCriteria, void * rawptr, sizet count, pITrack_base *&array, MemAllocatorFunc allocFunc, pcIError_base & error);

		virtual spITrackList  APICALL GetTracks(uint64 markerTypes = UINT64_MAX, bool matchAllCriteria = false, const std::vector<std::pair< const char *, sizet> > &markerTypeName = std::vector<std::pair<const char *, sizet> >()) const;

		virtual sizet  APICALL getTracks(uint64 markerTypes, uint32 matchAllCriteria, void * rawptr, sizet count, pITrack_base *&array, MemAllocatorFunc allocFunc, pcIError_base & error) const;

		virtual sizet  APICALL getMarkersInTimeRange(uint64 startFrameCount, uint64 endFrameCount, uint32 timeBoundary, uint64 markerTypes, uint32 matchAllCriteria, void * rawptr, sizet count, pIMarker_base *&array, MemAllocatorFunc allocFunc, pcIError_base & error) const;

		virtual spIMarkerList  APICALL GetMarkersInTimeRange(uint64 startFrameCount = 0, uint64 endFrameCount = UINT64_MAX, ITrack::eTrackTimeBoundary timeBoundary = ITrack::kTTBothStartTimeAndEndTimeIncluded, uint64 markerTypes = UINT64_MAX, bool matchAllCriteria = false, const std::vector<std::pair< const char *, sizet> > &markerTypeName = std::vector<std::pair<const char *, sizet> >()) const;

		pvoid APICALL GetInterfacePointerInternal(uint64 interfaceID, uint32 interfaceVersion, bool isTopLevel);

		virtual pvoid APICALL getInterfacePointer(uint64 interfaceID, uint32 interfaceVersion, pcIError_base & error) __NOTHROW__;

		virtual ~ITemporalAssetManager_I() __NOTHROW__;

#ifndef FRIEND_CLASS_DECLARATION
#define FRIEND_CLASS_DECLARATION() 
#endif
		FRIEND_CLASS_DECLARATION();
	};

	inline ITemporalAssetManager_I::~ITemporalAssetManager_I() __NOTHROW__{}
}

#endif  // ITemporalAssetManager_I_h__
