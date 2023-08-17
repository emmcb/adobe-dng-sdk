// =================================================================================================
// ADOBE SYSTEMS INCORPORATED
// Copyright 2014 Adobe Systems Incorporated
// All Rights Reserved
//
// NOTICE:  Adobe permits you to use, modify, and distribute this file in accordance with the terms
// of the Adobe license agreement accompanying it.
// =================================================================================================
#include "XMPMarker/XMPMarkerFwdDeclarations_I.h"
#include "XMPCommon/Interfaces/IUTF8String_I.h"
#include "XMPMarker/Interfaces/ITrack_I.h"
#include "XMPCommon/Utilities/TWrapperFunctions_I.h"

#include "XMPCore/XMPCoreFwdDeclarations_I.h"
#include "XMPMarker/Interfaces/IMarker_I.h"

#include "XMPCommon/Utilities/TSmartPointers_I.h"
#include "XMPCommon/XMPCommonErrorCodes.h"


namespace AdobeXMPAM_Int {

//	void ITrack_I::AddMarker(const pcIMarker_base & marker, pcIError_base &error) {
//
//		return NS_INT_XMPCOMMON::CallUnSafeFunctionReturningVoid<ITrack_I, const spcIMarker_I &>(error, this, &ITrack_I::AddMarker_I, NS_INT_XMPCOMMON::MakeSharedPointer<const IMarker_I> (marker));
//	}
//	
	pvoid APICALL ITrack_I::GetInterfacePointer(uint64 interfaceID, uint32 interfaceVersion) {
		return GetInterfacePointerInternal(interfaceID, interfaceVersion, true);
	}

	pvoid APICALL ITrack_I::getInterfacePointer(uint64 interfaceID, uint32 interfaceVersion, pcIError_base & error) __NOTHROW__{
		return CallUnSafeFunction< ITrack_I, pvoid, pvoid, uint64, uint32 >(
		error, this, NULL, &ITrack_I::GetInterfacePointer, __FILE__, __LINE__, interfaceID, interfaceVersion);
	}

	pvoid APICALL ITrack_I::GetInterfacePointerInternal(uint64 interfaceID, uint32 interfaceVersion, bool isTopLevel) {
		pvoid returnValue(NULL);
		if (interfaceID == kITrackID) { 
			switch (interfaceVersion) {
			case 1:
				return static_cast<ITrack_v1 *>(this);
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
			IError_v1::kESOperationFatal, kITrackID, interfaceID, __FILE__, __LINE__);
		return NULL;
	}

	static sizet CopyInnerListAsListOfRawPointers(spIMarkerList_I & list,
		pIMarker_base *& array, MemAllocatorFunc allocFunc, pcIError_base & error)
	{
		sizet nElements = list->size();
		array = (pIMarker_base *)allocFunc(nElements * sizeof(pIMarker_base));
		if (array) {
			for (sizet i = 0; i < nElements; i++) {
				spIMarker_I & ref = list->at(i);
				//ref->AcquireSharedObjectForRawPointer();
				ref->Acquire();
				array[i] = ref.get();
			}
		}
		else {
		/*	AdobeXMPCommon_Int::spIError_I _error = AdobeXMPCommon_Int::IError_I::CreateError(
				IError_base::kDomainMemoryManagement,
				IError_base::kMemoryManagementCodeAllocationFailure,
				IError_base::kSeverityOperationFatal);
			_error->AcquireSharedObjectForRawPointer();
			error = _error.get();*/
		}
		return nElements;
	}

	static spIMarkerList CopyInnerListAsPublicList(const spIMarkerList_I & list) {
		sizet nElements = list->size();
		spIMarkerList newList = MakeUncheckedSharedPointerWithDefaultDelete< IMarkerList>(new IMarkerList(), __FILE__, __LINE__);
			//new IMarkerList());
		newList->reserve(nElements);
		for (sizet i = 0; i < nElements; i++) {
			newList->push_back(list->at(i));
		}
		return newList;
	}

	sizet ITrack_I::getAllMarkers(pIMarker_base *& array, MemAllocatorFunc allocFunc,
		pcIError_base & error) const
	{
		typedef spIMarkerList_I(APICALL ITrack_I::*func)() const ;
		func f = &ITrack_I::GetAllMarkers_I;
		auto list = CallConstUnSafeFunction<
		ITrack_I, spIMarkerList_I, spIMarkerList_I
		>(error, this, spIMarkerList_I(), f, __FILE__, __LINE__);

		return CopyInnerListAsListOfRawPointers(list, array, allocFunc, error);
	}

	spIMarkerList ITrack_I::GetAllMarkers() const {

		spIMarkerList_I list = GetAllMarkers_I();
		return CopyInnerListAsPublicList(list);
	}

spITrack ITrack_I::Clone() const{
#if !GENERATE_XMPEXTENSIONS_CLIENT_LAYER_CODE
	return Clone_I();
#else
	return spITrack();
#endif
}

pITrack_base ITrack_I::clone(pcIError_base & error) const{

	typedef spITrack(APICALL ITrack_I::*func)() const;
	func f = &ITrack_I::Clone_I;

	return CallConstUnSafeFunctionReturningSharedPointer< ITrack_I, pITrack_base, ITrack>(
	error, this, f, __FILE__, __LINE__);
}


spcIUTF8String ITrack_I::GetName() const {

	return this->GetName_I();
}
pcIUTF8String_base ITrack_I::getName(pcIError_base & error) const  __NOTHROW__{

	typedef spcIUTF8String(APICALL ITrack_I::*func)() const;
	func f = &ITrack_I::GetName_I;
	return CallConstUnSafeFunctionReturningSharedPointer<
	ITrack_I, pcIUTF8String_base, const IUTF8String>(error, this, f, __FILE__, __LINE__);
}

void ITrack_I::setName(const char * name, sizet nameLen, pcIError_base & error)  {

	typedef void(APICALL IMarker_I::*func)(const char *, sizet);
	func f = &IMarker_I::SetName_I;
	CallUnSafeFunctionReturningVoid<ITrack_I>(error, this, &ITrack_I::SetName_I, __FILE__, __LINE__, name, nameLen);
}

void ITrack_I::SetName(const char * name, sizet nameLen) {

	SetName_I(name, nameLen);
}

uint64 ITrack_I::setTypes(uint64 inputMask, pcIError_base & error)  {

	typedef uint64(APICALL ITrack_I::*func)(uint64);
	func f = &ITrack_I::SetTypes_I;
	return CallUnSafeFunction<
		ITrack_I, uint64, uint64, uint64
	>(error, this, 0, &ITrack_I::SetTypes_I, __FILE__, __LINE__, inputMask);
}

uint64 ITrack_I::SetTypes(uint64 inputMask) {

	return SetTypes_I(inputMask);
}

uint64 ITrack_I::removeTypes(uint64 inputMask, pcIError_base & error)  {

	typedef uint64(APICALL ITrack_I::*func)(uint64);
	func f = &ITrack_I::RemoveTypes_I;
	return CallUnSafeFunction<
		ITrack_I, uint64, uint64, uint64
	>(error, this, 0, f, __FILE__, __LINE__, inputMask);
}

uint64 ITrack_I::RemoveTypes(uint64 inputMask) {

	return RemoveTypes_I(inputMask);
}

uint64 ITrack_I::getTypes(pcIError_base & error) const  {

	typedef uint64(APICALL ITrack_I::*func)()const;
	func f = &ITrack_I::GetTypes_I;
	return CallConstUnSafeFunction<
		ITrack_I, uint64, uint64
	>(error, this, 0, f, __FILE__,__LINE__);
}

uint64 ITrack_I::GetTypes() const {

	return GetTypes_I();
}

bool ITrack_I::appendCustomType(const char * userDefinedType, sizet userDefinedTypeLen, pcIError_base & error) {

	typedef bool(APICALL ITrack_I::*func)(const char *, sizet);
	func f = &ITrack_I::AppendCustomType_I;

	return CallUnSafeFunction<
		ITrack_I, bool, bool, const char *, sizet
	>(error, this, 0, &ITrack_I::AppendCustomType_I, __FILE__, __LINE__, userDefinedType, userDefinedTypeLen);
}

bool ITrack_I::AppendCustomType(const char * userDefinedType, sizet userDefinedTypeLen) {

	return AppendCustomType_I(userDefinedType, userDefinedTypeLen);
}

bool ITrack_I::removeCustomType(const char * userDefinedType, sizet userDefinedTypeLen, pcIError_base & error) {

	typedef bool (APICALL ITrack_I::*func)(const char *, sizet);
	func f = &ITrack_I::RemoveCustomType_I;
	return CallUnSafeFunction<
		ITrack_I, bool, bool, const char *, sizet
	>(error, this, 0, &ITrack_I::RemoveCustomType_I, __FILE__, __LINE__, userDefinedType, userDefinedTypeLen);
}

bool ITrack_I::RemoveCustomType(const char * userDefinedType, sizet userDefinedTypeLen) {

	return RemoveCustomType_I(userDefinedType, userDefinedTypeLen);
}

sizet ITrack_I::GetCustomTypeCount() const {

	return GetCustomTypeCount_I();
}

sizet ITrack_I::getCustomTypeCount(pcIError_base & error) const {

	typedef sizet(APICALL ITrack_I::*func)()const;
	func f = &ITrack_I::GetCustomTypeCount_I;
	return CallConstUnSafeFunction<
		ITrack_I, sizet, sizet
	>(error, this, 0, f, __FILE__, __LINE__);
}

AdobeXMPCommon::spcIUTF8String ITrack_I::GetCustomType(sizet index) const {

	return this->GetCustomType_I(index);
}

pcIUTF8String_base ITrack_I::getCustomType(sizet index, pcIError_base & error) const {

	typedef spcIUTF8String (APICALL ITrack_I::*func)(sizet) const;
	func f = &ITrack_I::GetCustomType_I;
	return CallConstUnSafeFunctionReturningSharedPointer< ITrack_I, pcIUTF8String_base, const IUTF8String, sizet >(error, this, f, __FILE__, __LINE__, index);
}


static sizet CopyInnerListAsListOfRawPointers(const spcIUTF8StringList_I & list,
	pcIUTF8String_base *& array, MemAllocatorFunc allocFunc, pcIError_base & error)
{
	sizet nElements = list->size();
	array = (pcIUTF8String_base *)allocFunc(nElements * sizeof(pcIUTF8String_base));
	if (array) {
		for (sizet i = 0; i < nElements; i++) {
			const spcIUTF8String & ref = list->at(i);
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

static spcIUTF8StringList CopyInnerListAsPublicList(const spcIUTF8StringList_I & list) {
	sizet nElements = list->size();
	spcIUTF8StringList newList = MakeUncheckedSharedPointerWithDefaultDelete(
		new cIUTF8StringList(), __FILE__, __LINE__ );
	newList->reserve(nElements);
	for (sizet i = 0; i < nElements; i++) {
		newList->push_back(list->at(i));
	}
	return newList;
}

spcIUTF8StringList ITrack_I::GetCustomTypes() const {
	auto list = GetCustomTypes_I();
	return CopyInnerListAsPublicList(list);
}

sizet ITrack_I::getCustomTypes(pcIUTF8String_base *& array, MemAllocatorFunc allocFunc,
	pcIError_base & error) const
{
	typedef spcIUTF8StringList_I(APICALL ITrack_I::*func)()const;
	func f = &ITrack_I::GetCustomTypes_I;
	auto list = CallConstUnSafeFunction<
		ITrack_I, spcIUTF8StringList_I, spcIUTF8StringList_I
	>(error, this, spcIUTF8StringList_I(), f, __FILE__, __LINE__);

	return CopyInnerListAsListOfRawPointers(list, array, allocFunc, error);
}

	
	spIMarker ITrack_I::AddMarker(const char * markerName, sizet markerNameLen, uint64 markerTypes, const char * markerTypeName, sizet markerTypeNameLen, const char * guid, sizet guidLen) {

		return AddMarker_I(markerName, markerNameLen, markerTypes, markerTypeName, markerTypeNameLen, guid, guidLen);
	}

	pIMarker_base ITrack_I::addMarker(const char * markerName, sizet markerNameLen, uint64 markerTypes, const char * markerTypeName, sizet markerTypeNameLen, const char * guid, sizet guidLen, pcIError_base & error) {

		
		typedef spIMarker(APICALL ITrack_I::*func)(const char*, sizet, uint64, const char *, sizet, const char *, sizet);
		func f = &ITrack_I::AddMarker_I;
		return CallUnSafeFunctionReturningSharedPointer < ITrack_I, pIMarker_base, IMarker, const char*, sizet, uint64, const char *, sizet, const char *,sizet>(
			error, this, &ITrack_I::AddMarker_I, __FILE__, __LINE__, markerName, markerNameLen, markerTypes, markerTypeName, markerTypeNameLen, guid, guidLen);
	

	}

	AdobeXMPAM::pIMarker_base ITrack_I::addMarker(pIMarker_base inMarker, pcIError_base & error) {

		if (error) return 0;
		pIMarker_I ptr = dynamic_cast<pIMarker_I> (inMarker);
		auto sp = IMarker::MakeShared(inMarker);
		if (error) return 0;
		typedef spIMarker(APICALL ITrack_I::*func)(spIMarker_I );
		func f = &ITrack_I::AddMarker_I;
		return CallUnSafeFunctionReturningSharedPointer < ITrack_I, pIMarker_base, IMarker, spIMarker_I>(
			error, this, &ITrack_I::AddMarker_I, __FILE__, __LINE__, dynamic_pointer_cast<IMarker_I>(sp));
		
	}

	AdobeXMPAM::spIMarker ITrack_I::AddMarker(spIMarker inMarker) {
		auto sp = IMarker::MakeShared(inMarker.get());
		return AddMarker_I(dynamic_pointer_cast<IMarker_I>(sp));
	}

	void ITrack_I::setFrameRate(uint64 numerator, uint64 denominator, pcIError_base & error) {

		typedef void (APICALL ITrack_I::*func)(uint64, uint64);
		func f = &ITrack_I::SetFrameRate_I;
		return CallUnSafeFunctionReturningVoid<ITrack_I>(error, this, &ITrack_I::SetFrameRate_I, __FILE__, __LINE__, numerator, denominator);
	}

	void ITrack_I::SetFrameRate(uint64 numerator /*= 1*/, uint64 denominator /*= 1*/) {

		return SetFrameRate_I(numerator, denominator);
	}

	AdobeXMPAM::pIMarker_base ITrack_I::removeMarker(const sizet & markerIdx, pcIError_base & error) {

		typedef spIMarker (APICALL ITrack_I::*func)(const sizet &);
		func f = &ITrack_I::RemoveMarker_I;

		return CallUnSafeFunctionReturningSharedPointer < ITrack_I, pIMarker_base, IMarker, const sizet &>(
			error, this, &ITrack_I::RemoveMarker_I, __FILE__, __LINE__, markerIdx);
	}

	AdobeXMPAM::spIMarker ITrack_I::RemoveMarker(const sizet & markerIdx) {

		return RemoveMarker_I(markerIdx);
	}

	AdobeXMPAM::pIMarker_base ITrack_I::removeMarker(const char * guid, sizet guidLen, pcIError_base & error) {

		typedef spIMarker (APICALL ITrack_I::*func)(const char *, sizet);
		func f = &ITrack_I::RemoveMarker_I;
		return CallUnSafeFunctionReturningSharedPointer < ITrack_I, pIMarker_base, IMarker, const char *, sizet>(
			error, this, &ITrack_I::RemoveMarker_I, __FILE__, __LINE__, guid, guidLen);
	}

	AdobeXMPAM::spIMarker ITrack_I::RemoveMarker(const char *guid, sizet guidLen) {

		return RemoveMarker_I(guid, guidLen); 
	}

	AdobeXMPAM::spIMarker ITrack_I::GetMarker(const sizet & markerIdx) const{

		return GetMarker_I(markerIdx);
	}

	AdobeXMPAM::spIMarker ITrack_I::GetMarker(const char * guid, sizet guidLen) const {

		return GetMarker_I(guid, guidLen);
	}

	AdobeXMPAM::pIMarker_base ITrack_I::getMarker(const char * guid, sizet guidLen, pcIError_base & error) const {

		typedef spIMarker (APICALL ITrack_I::*func)(const char *, sizet) const;
		func f = &ITrack_I::GetMarker_I;
		return CallConstUnSafeFunctionReturningSharedPointer < ITrack_I, pIMarker_base, IMarker, const char *, sizet>(
			error, this, &ITrack_I::GetMarker_I, __FILE__, __LINE__, guid, guidLen);
	}

	AdobeXMPAM::pIMarker_base ITrack_I::getMarker(const sizet & markerIdx, pcIError_base & error) const {

		typedef spIMarker (APICALL ITrack_I::*func)(const sizet &) const;
		func f = &ITrack_I::GetMarker_I;
		return CallConstUnSafeFunctionReturningSharedPointer < ITrack_I, pIMarker_base, IMarker, const sizet &>(
			error, this, &ITrack_I::GetMarker_I, __FILE__, __LINE__, markerIdx);
	}

	void ITrack_I::foo(std::vector<std::pair<const char *, sizet> > & v) {

	}

	static void CopyInnerListAsListOfRawPointers(std::vector<std::pair<const char *, sizet> > &vec, void *v, sizet size, pcIError_base & error)
	{
		
		std::vector<std::pair< const char *, sizet> > *ptr = (std::vector<std::pair<const char *, sizet> > *) v;
		vec.reserve(size);
		if (v) {
			for (sizet i = 0; i < size; i++) {
				vec.push_back({ ptr->at(i).first, ptr->at(i).second });
			}
		}
		else {
		/*	AdobeXMPCommon_Int::spIError_I _error = AdobeXMPCommon_Int::IError_I::CreateError(
				IError_base::kDomainMemoryManagement,
				IError_base::kMemoryManagementCodeAllocationFailure,
				IError_base::kSeverityOperationFatal);
			_error->SetLocation(__FILE__, __LINE__);
			_error->SetMessage("Memory allocation failure");
			_error->AcquireSharedObjectForRawPointer();
			error = _error.get();*/
		}
		
	}

	void ITrack_I::foo(void * v, sizet size, pcIError_base & error) {

		std::vector<std::pair<const char *, sizet> > vec;
		CopyInnerListAsListOfRawPointers(vec, v, size, error);
		return foo_I(vec);
	}

	AdobeXMPAM::spIMarkerList ITrack_I::RemoveMarkers(uint64 markerTypes /*= UINT64_MAX*/, bool matchAllCriteria /*= false*/, const std::vector<std::pair< const char *, sizet> > &markerTypeName /*= std::vector<const char *>()*/) {

		spIMarkerList_I  list = RemoveMarkers_I(markerTypes, matchAllCriteria, markerTypeName);
		return CopyInnerListAsPublicList(list);

	}

	AdobeXMPCommon_Int::sizet ITrack_I::removeMarkers(uint64 markerTypes, uint32 matchAllCriteria, void * rawptr, sizet count, pIMarker_base *&array, MemAllocatorFunc allocFunc, pcIError_base & error) {


		std::vector<std::pair<const char *, sizet> > vec;
		CopyInnerListAsListOfRawPointers(vec, rawptr, count, error);
		typedef spIMarkerList_I(APICALL ITrack_I::*func)(uint64, bool, const std::vector<std::pair<const char *, sizet> > &);
		func f = &ITrack_I::RemoveMarkers_I;
		auto list = CallUnSafeFunction<
			ITrack_I, spIMarkerList_I, spIMarkerList_I, uint64, bool, const std::vector<std::pair<const char *, sizet> > &
		>(error, this, spIMarkerList_I(), f, __FILE__, __LINE__, markerTypes, static_cast<bool>(matchAllCriteria), vec);

		return CopyInnerListAsListOfRawPointers(list, array, allocFunc, error);

	}

	AdobeXMPAM::spIMarkerList ITrack_I::GetMarkers(uint64 markerTypes /*= UINT64_MAX*/, bool matchAllCriteria /*= false*/, const std::vector<std::pair<const char *, sizet> > &markerTypeName /*= std::vector<const char *>()*/) const {

		spIMarkerList_I  list = GetMarkers_I(markerTypes, matchAllCriteria, markerTypeName);
		return CopyInnerListAsPublicList(list);
	}

	AdobeXMPCommon_Int::sizet ITrack_I::getMarkers(uint64 markerTypes, uint32 matchAllCriteria, void * rawptr, sizet count, pIMarker_base *&array, MemAllocatorFunc allocFunc, pcIError_base & error) const {

		typedef spIMarkerList_I(APICALL ITrack_I::*func)(uint64 a, bool b, const std::vector<std::pair<const char *, sizet> > &c /*= std::vector<const char *>()*/)const;
		func f = &ITrack_I::GetMarkers_I;
		std::vector<std::pair<const char *, sizet> > vec;
		CopyInnerListAsListOfRawPointers(vec, rawptr, count, error);
		auto list = CallConstUnSafeFunction<
			ITrack_I, spIMarkerList_I, spIMarkerList_I, uint64, bool, const std::vector<std::pair<const char *, sizet> > &
		>(error, this, spIMarkerList_I(), &ITrack_I::GetMarkers_I, __FILE__, __LINE__, markerTypes, static_cast<uint32>(matchAllCriteria), vec);

		return CopyInnerListAsListOfRawPointers(list, array, allocFunc, error);
	}

	AdobeXMPCommon::sizet ITrack_I::getMarkersByName(const char * name, sizet nameLen, pIMarker_base *&array, MemAllocatorFunc allocFunc, pcIError_base & error) const {
	
		typedef spIMarkerList_I(APICALL ITrack_I::*func)(const char *, sizet)const;
		func f = &ITrack_I::GetMarkersByName_I;
		auto list = CallConstUnSafeFunction<
			ITrack_I, spIMarkerList_I, spIMarkerList_I,  const char *, sizet
		>(error, this, spIMarkerList_I(), f, __FILE__, __LINE__, name, nameLen);

		return CopyInnerListAsListOfRawPointers(list, array, allocFunc, error);
	}

	AdobeXMPAM::spIMarkerList ITrack_I::GetMarkersByName(const char * markerName, sizet markerNameLen) const {
		
		spIMarkerList_I  list = GetMarkersByName_I(markerName, markerNameLen);
		return CopyInnerListAsPublicList(list);
	}

	AdobeXMPCommon_Int::sizet ITrack_I::getMarkersInTimeRange(uint64 startFrameCount, uint64 endFrameCount, uint32 timeBoundary, uint64 markerTypes, uint32 matchAllCriteria, void * rawptr, sizet count, pIMarker_base *&array, MemAllocatorFunc allocFunc, pcIError_base & error) const {
		std::vector<std::pair<const char *, sizet> > vec;
		CopyInnerListAsListOfRawPointers(vec, rawptr, count, error);
		typedef spIMarkerList_I(APICALL ITrack_I::*func)(uint64, uint64, eTrackTimeBoundary, uint64, bool, const std::vector<std::pair<const char *, sizet> > &)const;
		func f = &ITrack_I::GetMarkersInTimeRange_I;
		auto list = CallConstUnSafeFunction<
			ITrack_I, spIMarkerList_I, spIMarkerList_I, uint64, uint64, eTrackTimeBoundary, uint64, bool, const std::vector<std::pair<const char *, sizet> > &
		>(error, this, spIMarkerList_I(), &ITrack_I::GetMarkersInTimeRange_I, __FILE__, __LINE__, startFrameCount, endFrameCount, static_cast<ITrack::eTrackTimeBoundary>(timeBoundary),  markerTypes, static_cast<uint32>(matchAllCriteria), vec);
		return CopyInnerListAsListOfRawPointers(list, array, allocFunc, error);
	}

	AdobeXMPAM::spIMarkerList ITrack_I::GetMarkersInTimeRange(uint64 startFrameCount /*= 0*/, uint64 endFrameCount /*= UINT64_MAX*/, eTrackTimeBoundary timeBoundary /*= kTTBothStartTimeAndEndTimeIncluded*/, uint64 markerTypes /*= UINT64_MAX*/, bool matchAllCriteria /*= false*/, const std::vector<std::pair< const char *, sizet> > &markerTypeName /*= std::vector<const char *>()*/) const {

		spIMarkerList_I list = GetMarkersInTimeRange_I(startFrameCount, endFrameCount, timeBoundary, markerTypes, matchAllCriteria, markerTypeName);
		return CopyInnerListAsPublicList(list);
	}

	bool ITrack_I::HasCustomType(const char * customType, sizet customTypeLen) const {

		return HasCustomType_I(customType, customTypeLen);
	}

	AdobeXMPCommon::uint32 ITrack_I::hasCustomType(const char * customType, sizet customTypeLen, pcIError_base & error) const {

		typedef bool(APICALL ITrack_I::*func)(const char *, sizet)const;
		func f = &ITrack_I::HasCustomType_I;
		auto returnValue = CallConstUnSafeFunction <
			ITrack_I, uint32, bool, const char *, sizet
		>(error, this, 0, f, __FILE__, __LINE__, customType, customTypeLen);

		return static_cast<uint32>(returnValue);
	}

	
}




namespace AdobeXMPAM {
	using namespace AdobeXMPAM_Int;
	
#if SOURCE_COMPILING_XMPEXTENSIONS_LIB

	spITrack  ITrack_v1::CreateTrack(const char * trackName, sizet trackNameLen, uint64 trackTypes, const char * trackTypeName /* = NULL */, sizet trackTypeNameLen) {
	
		return ITrack_I::CreateTrack_I(trackName, trackNameLen, trackTypes, trackTypeName, trackTypeNameLen);
	}

#endif
}
