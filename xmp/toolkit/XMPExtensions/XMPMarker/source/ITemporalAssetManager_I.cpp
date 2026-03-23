// =================================================================================================
// ADOBE SYSTEMS INCORPORATED
// Copyright 2014 Adobe Systems Incorporated
// All Rights Reserved
//
// NOTICE:  Adobe permits you to use, modify, and distribute this file in accordance with the terms
// of the Adobe license agreement accompanying it.
// =================================================================================================
//#define SOURCE_COMPILING_XMPEXTENSIONS_LIB 1
#include "XMPMarker/Interfaces/ITemporalAssetManager_I.h"
#include "XMPMarker/Interfaces/ITrack_I.h"

#include "XMPCommon/Utilities/TWrapperFunctions_I.h"
#include "XMPCommon/Interfaces/IUTF8String_I.h"
#include "XMPCore/XMPCoreFwdDeclarations_I.h"
#include "XMPMarker/Interfaces/IMarker_I.h"
#include "XMPCommon/Utilities/TSmartPointers_I.h"
#include "XMPCommon/XMPCommonErrorCodes.h"


namespace AdobeXMPAM_Int {



	pvoid APICALL ITemporalAssetManager_I::GetInterfacePointer(uint64 interfaceID, uint32 interfaceVersion) {
		return GetInterfacePointerInternal(interfaceID, interfaceVersion, true);
	}

	pvoid APICALL ITemporalAssetManager_I::getInterfacePointer(uint64 interfaceID, uint32 interfaceVersion, pcIError_base & error) __NOTHROW__{
		return CallUnSafeFunction< ITemporalAssetManager_I, pvoid, pvoid, uint64, uint32 >(
		error, this, NULL, &ITemporalAssetManager_I::GetInterfacePointer, __FILE__, __LINE__, interfaceID, interfaceVersion);
	}

		pvoid APICALL ITemporalAssetManager_I::GetInterfacePointerInternal(uint64 interfaceID, uint32 interfaceVersion, bool isTopLevel) {
		pvoid returnValue(NULL);
		if (interfaceID == kITemporalAssetManagerID) {
			switch (interfaceVersion) {
			case 1:
				return static_cast<ITemporalAssetManager_v1 *>(this);
				break;

			case kInternalInterfaceVersionNumber:
				return this;
				break;

			default:
				throw IError_I::CreateInterfaceVersionNotAvailableError(
					IError_v1::kESOperationFatal, interfaceID, interfaceVersion, __FILE__, __LINE__);
				break;
			}
		}
		if (isTopLevel)
			throw IError_I::CreateInterfaceNotAvailableError(
			IError_v1::kESOperationFatal, kITemporalAssetManagerID, interfaceID, __FILE__, __LINE__);
		return NULL;
	}

static sizet CopyInnerListAsListOfRawPointers(spITrackList_I & list,
	pITrack_base *& array, MemAllocatorFunc allocFunc, pcIError_base & error)
{
	sizet nElements = list->size();
	array = (pITrack_base *)allocFunc(nElements * sizeof(pITrack_base));
	if (array) {
		for (sizet i = 0; i < nElements; i++) {
			const spITrack_I & ref = list->at(i);
			//ref->AcquireSharedObjectForRawPointer();
			ref->Acquire();
			array[i] = ref.get();
		}

	}
	else {
		/*AdobeXMPCommon_Int::spIError_I _error = AdobeXMPCommon_Int::IError_I::CreateError(
			IError_base::kDomainMemoryManagement,
			IError_base::kMemoryManagementCodeAllocationFailure,
			IError_base::kSeverityOperationFatal);
		_error->SetLocation(__FILE__, __LINE__);
		_error->SetMessage("Memory allocation failure");
		_error->AcquireSharedObjectForRawPointer();
		error = _error.get();*/
	}
	return nElements;
}


static sizet CopyInnerListAsListOfRawPointers(spIMarkerList_I & list,
	pIMarker_base *& array, MemAllocatorFunc allocFunc, pcIError_base & error)
{
	sizet nElements = list->size();
	array = (pIMarker_base *)allocFunc(nElements * sizeof(pIMarker_base));
	if (array) {
		for (sizet i = 0; i < nElements; i++) {
			const spIMarker_I & ref = list->at(i);
	//		ref->AcquireSharedObjectForRawPointer();
			ref->Acquire();
			array[i] = ref.get();
		}
	}
	else {
		/*AdobeXMPCommon_Int::spIError_I _error = AdobeXMPCommon_Int::IError_I::CreateError(
			IError_base::kDomainMemoryManagement,
			IError_base::kMemoryManagementCodeAllocationFailure,
			IError_base::kSeverityOperationFatal);
		_error->SetLocation(__FILE__, __LINE__);
		_error->SetMessage("Memory allocation failure");
		_error->AcquireSharedObjectForRawPointer();
		error = _error.get();*/
	}
	return nElements;
}

static spITrackList CopyInnerListAsPublicList(spITrackList_I & list) {
	sizet nElements = list->size();
	spITrackList newList = MakeUncheckedSharedPointerWithDefaultDelete(
		new ITrackList(), __FILE__, __LINE__);
	newList->reserve(nElements);
	for (sizet i = 0; i < nElements; i++) {
		newList->push_back(list->at(i));
	}
	return newList;
}

static spIMarkerList CopyInnerListAsPublicList(spIMarkerList_I & list) {
	sizet nElements = list->size();
	spIMarkerList newList = MakeUncheckedSharedPointerWithDefaultDelete(
		new IMarkerList(), __FILE__, __LINE__);
	newList->reserve(nElements);
	for (sizet i = 0; i < nElements; i++) {
		newList->push_back(list->at(i));
	}
	return newList;
}

static void CopyInnerListAsListOfRawPointers(std::vector<std::pair< const char *, sizet> > &vec, void *v, sizet size, pcIError_base & error)
{

	std::vector<std::pair<const char *, sizet> > *ptr = (std::vector<std::pair<const char *, sizet> > *) v;
	vec.reserve(size);
	if (v) {
		for (sizet i = 0; i < size; i++) {
	//TODO LENGTH
			vec.push_back({ ptr->at(i).first, ptr->at(i).second });;
		}
	}
	else {
		/*AdobeXMPCommon_Int::spIError_I _error = AdobeXMPCommon_Int::IError_I::CreateError(
			IError_base::kDomainMemoryManagement,
			IError_base::kMemoryManagementCodeAllocationFailure,
			IError_base::kSeverityOperationFatal);
		_error->SetLocation(__FILE__, __LINE__);
		_error->SetMessage("Memory allocation failure");
		_error->AcquireSharedObjectForRawPointer();
		error = _error.get();*/
	}

}

AdobeXMPAM::spITrack ITemporalAssetManager_I::AddTrack(spITrack trackToBeAdded) {

	auto sp = ITrack::MakeShared(trackToBeAdded.get());
	return AddTrack_I(dynamic_pointer_cast<ITrack>(sp));
}

AdobeXMPAM::pITrack_base ITemporalAssetManager_I::addTrack(pITrack_base trackToBeAdded, pcIError_base & error) {


	if (error) return 0;
	pITrack_I ptr = dynamic_cast<pITrack_I>(trackToBeAdded);
	auto sp = ITrack::MakeShared(trackToBeAdded);
	if (error) return 0;
	typedef spITrack (APICALL ITemporalAssetManager_I::*func)(spITrack);
	func f = &ITemporalAssetManager_I::AddTrack_I;
	//return pITrack_base();
	return CallUnSafeFunctionReturningSharedPointer < ITemporalAssetManager_I, pITrack_base, ITrack, spITrack>(
		error, this, &ITemporalAssetManager_I::AddTrack_I, __FILE__, __LINE__, dynamic_pointer_cast<ITrack>(sp));
}

sizet ITemporalAssetManager_I::getAllTracks(pITrack_base *& array, MemAllocatorFunc allocFunc, pcIError_base & error) const{

	typedef spITrackList_I(APICALL ITemporalAssetManager_I::*func)()const;
	func f = &ITemporalAssetManager_I::GetAllTracks_I;
	auto list = CallConstUnSafeFunction<
		ITemporalAssetManager_I, spITrackList_I, spITrackList_I
	>(error, this, spITrackList_I(), f, __FILE__, __LINE__);

	return CopyInnerListAsListOfRawPointers(list, array, allocFunc, error);
}

AdobeXMPAM::spITrackList ITemporalAssetManager_I::GetAllTracks() const {

	spITrackList_I list = GetAllTracks_I();
	return CopyInnerListAsPublicList(list);
}

sizet ITemporalAssetManager_I::removeAllTracks(pITrack_base *& array, MemAllocatorFunc allocFunc, pcIError_base & error) {

	typedef spITrackList_I(APICALL ITemporalAssetManager_I::*func)();
	func f = &ITemporalAssetManager_I::RemoveAllTracks_I;
	auto list = CallUnSafeFunction<
		ITemporalAssetManager_I, spITrackList_I, spITrackList_I
	>(error, this, spITrackList_I(), &ITemporalAssetManager_I::RemoveAllTracks_I, __FILE__, __LINE__);

	return CopyInnerListAsListOfRawPointers(list, array, allocFunc, error);
}

AdobeXMPAM::spITrackList ITemporalAssetManager_I::RemoveAllTracks() {

	spITrackList_I list = RemoveAllTracks_I();
	return CopyInnerListAsPublicList(list);
}

AdobeXMPCommon_Int::sizet ITemporalAssetManager_I::getMarkersByName(const char * markerName, sizet markerNameLen, pIMarker_base *& array, MemAllocatorFunc allocFunc, pcIError_base & error) const {

	typedef spIMarkerList_I(APICALL ITemporalAssetManager_I::*func)(const char *, sizet)const;
	func f = &ITemporalAssetManager_I::GetMarkersByName_I;
	auto list = CallConstUnSafeFunction<
		ITemporalAssetManager_I, spIMarkerList_I, spIMarkerList_I, const char *, sizet
	>(error, this, spIMarkerList_I(), f, __FILE__, __LINE__, markerName, markerNameLen);

	return CopyInnerListAsListOfRawPointers(list, array, allocFunc, error);
}

AdobeXMPAM::spIMarkerList ITemporalAssetManager_I::GetMarkersByName(const char * markerName, sizet makerNameLen) const {

	spIMarkerList_I list = GetMarkersByName_I(markerName, makerNameLen);
	return CopyInnerListAsPublicList(list);
}

AdobeXMPAM::spIMarkerList ITemporalAssetManager_I::RemoveMarkers(uint64 markerTypes /*= UINT64_MAX*/, bool matchAllCriteria /*= false*/, const std::vector<std::pair< const char *, sizet> > &markerTypeName /*= std::vector<const char *>()*/) {

	spIMarkerList_I  list = RemoveMarkers_I(markerTypes, matchAllCriteria, markerTypeName);
	return CopyInnerListAsPublicList(list);

}

AdobeXMPCommon_Int::sizet ITemporalAssetManager_I::removeMarkers(uint64 markerTypes, uint32 matchAllCriteria, void * rawptr, sizet count, pIMarker_base *&array, MemAllocatorFunc allocFunc, pcIError_base & error) {

	typedef spIMarkerList_I(APICALL ITemporalAssetManager_I::*func)(uint64, bool, const std::vector<std::pair<const char *, sizet> > &);
	func f = &ITemporalAssetManager_I::RemoveMarkers_I;
	std::vector<std::pair<const char *, sizet> > vec;
	CopyInnerListAsListOfRawPointers(vec, rawptr, count, error);
	auto list = CallUnSafeFunction<
		ITemporalAssetManager_I, spIMarkerList_I, spIMarkerList_I, uint64, bool, const std::vector<std::pair<const char *, sizet> > &
	>(error, this, spIMarkerList_I(), f, __FILE__, __LINE__, markerTypes, static_cast<bool>(matchAllCriteria), vec);

	return CopyInnerListAsListOfRawPointers(list, array, allocFunc, error);

}

AdobeXMPAM::spIMarkerList ITemporalAssetManager_I::GetMarkers(uint64 markerTypes /*= UINT64_MAX*/, bool matchAllCriteria /*= false*/, const std::vector<std::pair< const char *, sizet> > &markerTypeName /*= std::vector<const char *>()*/) const {

	spIMarkerList_I  list = GetMarkers_I(markerTypes, matchAllCriteria, markerTypeName);
	return CopyInnerListAsPublicList(list);
}

AdobeXMPCommon_Int::sizet ITemporalAssetManager_I::getMarkers(uint64 markerTypes, uint32 matchAllCriteria, void * rawptr, sizet count, pIMarker_base *&array, MemAllocatorFunc allocFunc, pcIError_base & error) const {

	typedef spIMarkerList_I(APICALL ITemporalAssetManager_I::*func)(uint64, bool, const std::vector<std::pair<const char *, sizet> > &)const;
	func f = &ITemporalAssetManager_I::GetMarkers_I;
	std::vector<std::pair<const char *, sizet> > vec;
	CopyInnerListAsListOfRawPointers(vec, rawptr, count, error);
	auto list = CallConstUnSafeFunction<
		ITemporalAssetManager_I, spIMarkerList_I, spIMarkerList_I, uint64, bool, const std::vector<std::pair<const char *, sizet> > &
	>(error, this, spIMarkerList_I(), f, __FILE__, __LINE__, markerTypes, static_cast<uint32>(matchAllCriteria), vec);

	return CopyInnerListAsListOfRawPointers(list, array, allocFunc, error);
}

AdobeXMPAM::spITrackList ITemporalAssetManager_I::RemoveTracks(uint64 markerTypes /*= UINT64_MAX*/, bool matchAllCriteria /*= false*/, const std::vector<std::pair< const char *, sizet> > &markerTypeName /*= std::vector<const char *>()*/) {

	spITrackList_I  list = RemoveTracks_I(markerTypes, matchAllCriteria, markerTypeName);
	return CopyInnerListAsPublicList(list);
}

AdobeXMPCommon_Int::sizet ITemporalAssetManager_I::removeTracks(uint64 markerTypes, uint32 matchAllCriteria, void * rawptr, sizet count, pITrack_base *&array, MemAllocatorFunc allocFunc, pcIError_base & error) {

	std::vector<std::pair<const char *, sizet> > vec;
	CopyInnerListAsListOfRawPointers(vec, rawptr, count, error);
	typedef spITrackList_I(APICALL ITemporalAssetManager_I::*func)(uint64, bool, const std::vector<std::pair<const char *, sizet> > &);
	func f = &ITemporalAssetManager_I::RemoveTracks_I;
	auto list = CallUnSafeFunction<
		ITemporalAssetManager_I, spITrackList_I, spITrackList_I, uint64, bool, const std::vector<std::pair<const char *, sizet> > &
	>(error, this, spITrackList_I(), f,  __FILE__, __LINE__, markerTypes, static_cast<uint32>(matchAllCriteria), vec);

	return CopyInnerListAsListOfRawPointers(list, array, allocFunc, error);
}

AdobeXMPAM::spITrackList ITemporalAssetManager_I::GetTracks(uint64 markerTypes /*= UINT64_MAX*/, bool matchAllCriteria /*= false*/, const std::vector<std::pair< const char *, sizet> > &markerTypeName /*= std::vector<const char *>()*/) const {

	spITrackList_I  list = GetTracks_I(markerTypes, matchAllCriteria, markerTypeName);
	return CopyInnerListAsPublicList(list);
}

AdobeXMPCommon_Int::sizet ITemporalAssetManager_I::getTracks(uint64 markerTypes, uint32 matchAllCriteria, void * rawptr, sizet count, pITrack_base *&array, MemAllocatorFunc allocFunc, pcIError_base & error) const {

	std::vector<std::pair<const char *,sizet> > vec;
	CopyInnerListAsListOfRawPointers(vec, rawptr, count, error);
	typedef spITrackList_I(APICALL ITemporalAssetManager_I::*func)(uint64, bool, const std::vector<std::pair<const char *, sizet> > &)const;
	func f = &ITemporalAssetManager_I::GetTracks_I;
	auto list = CallConstUnSafeFunction<
		ITemporalAssetManager_I, spITrackList_I, spITrackList_I, uint64, bool, const std::vector<std::pair< const char *, sizet> > &
	>(error, this, spITrackList_I(), &ITemporalAssetManager_I::GetTracks_I, __FILE__, __LINE__, markerTypes, static_cast<uint32>(matchAllCriteria), vec);

	return CopyInnerListAsListOfRawPointers(list, array, allocFunc, error);
}

AdobeXMPCommon_Int::sizet ITemporalAssetManager_I::getMarkersInTimeRange(uint64 startFrameCount, uint64 endFrameCount, uint32 timeBoundary, uint64 markerTypes, uint32 matchAllCriteria, void * rawptr, sizet count, pIMarker_base *&array, MemAllocatorFunc allocFunc, pcIError_base & error) const {
	
	typedef spIMarkerList_I(APICALL ITemporalAssetManager_I::*func)(uint64, uint64, ITrack::eTrackTimeBoundary, uint64, bool, const std::vector<std::pair<const char *, sizet> > &)const;
	func f = &ITemporalAssetManager_I::GetMarkersInTimeRange_I;
	std::vector<std::pair<const char *, sizet> > vec;
	CopyInnerListAsListOfRawPointers(vec, rawptr, count, error);
	auto list = CallConstUnSafeFunction<
		ITemporalAssetManager_I, spIMarkerList_I, spIMarkerList_I, uint64, uint64, ITrack::eTrackTimeBoundary, uint64, bool, const std::vector<std::pair<const char *, sizet> > &
	>(error, this, spIMarkerList_I(), f, __FILE__, __LINE__, startFrameCount, endFrameCount, static_cast<ITrack::eTrackTimeBoundary>(timeBoundary), markerTypes, static_cast<uint32>(matchAllCriteria), vec);
	return CopyInnerListAsListOfRawPointers(list, array, allocFunc, error);
}

AdobeXMPAM::spIMarkerList ITemporalAssetManager_I::GetMarkersInTimeRange(uint64 startFrameCount /*= 0*/, uint64 endFrameCount /*= UINT64_MAX*/, ITrack::eTrackTimeBoundary timeBoundary /*= ITrack::kTTBothStartTimeAndEndTimeIncluded*/, uint64 markerTypes /*= UINT64_MAX*/, bool matchAllCriteria /*= false*/, const std::vector<std::pair<const char *, sizet> > &markerTypeName /*= std::vector<const char *>()*/) const {
	spIMarkerList_I list = GetMarkersInTimeRange_I(startFrameCount, endFrameCount, timeBoundary, markerTypes, matchAllCriteria, markerTypeName);
	return CopyInnerListAsPublicList(list);
}




}


namespace AdobeXMPAM {
	using namespace AdobeXMPAM_Int;

#if SOURCE_COMPILING_XMPEXTENSIONS_LIB

	spITemporalAssetManager ITemporalAssetManager_v1::CreateTemporalAssetManager(const spIMetadata & metadata)
	{
		return ITemporalAssetManager_I::CreateTemporalAssetManager_I(metadata);
	}
	
#endif
}
