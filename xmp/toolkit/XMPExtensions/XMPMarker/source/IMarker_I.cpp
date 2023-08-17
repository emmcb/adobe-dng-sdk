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
//
// mm-dd-yy who Description of changes, most recent first.
//
// 12-01-14 ADC 1.0-a022 Fixed the issue in the Get[All]Relationships client glue code.
// 11-26-14 ADC 1.0-a013 Added the full skeleton for IComposedAssetManager interface.
//
// =================================================================================================
#endif  // AdobePrivate

#include "XMPMarker/Interfaces/IMarker_I.h"

#include "XMPCommon/Utilities/TWrapperFunctions_I.h"
#include "XMPCommon/Interfaces/IUTF8String_I.h"
#include "XMPCore/XMPCoreFwdDeclarations_I.h"
#include "XMPMarker/XMPMarkerFwdDeclarations.h"
//#include "XMPAssetManagement/Interfaces/ITrack_I.h"
#include "XMPCore/XMPCoreFwdDeclarations.h"
#include "XMPCore/XMPCoreFwdDeclarations_I.h"
#include "XMPCore/Interfaces/IStructureNode_I.h"
#include "XMPMarker/XMPMarkerFwdDeclarations_I.h"
#include "XMPCommon/Interfaces/IError_I.h"
#include "XMPCommon/XMPCommonFwdDeclarations_I.h"
#include "XMPCommon/Utilities/TSmartPointers_I.h"
#include "XMPCommon/XMPCommonErrorCodes.h"
#include "XMPCommon/Interfaces/IUTF8String.h"

namespace AdobeXMPAM_Int {

	using  namespace AdobeXMPCommon;
	using namespace AdobeXMPCommon_Int;
	using namespace AdobeXMPCore;
	using namespace AdobeXMPCore_Int;
	using namespace AdobeXMPAM;

	pvoid APICALL IMarker_I::GetInterfacePointer(uint64 interfaceID, uint32 interfaceVersion) {
		return GetInterfacePointerInternal(interfaceID, interfaceVersion, true);
	}

	pvoid APICALL IMarker_I::getInterfacePointer(uint64 interfaceID, uint32 interfaceVersion, pcIError_base & error) __NOTHROW__{
		return CallUnSafeFunction< IMarker_I, pvoid, pvoid, uint64, uint32 >(
		error, this, NULL, &IMarker_I::GetInterfacePointer, __FILE__, __LINE__, interfaceID, interfaceVersion);
	}

		pvoid APICALL IMarker_I::GetInterfacePointerInternal(uint64 interfaceID, uint32 interfaceVersion, bool isTopLevel) {
		pvoid returnValue(NULL);
		if (interfaceID == kIMarkerID) {
			switch (interfaceVersion) {
			case 1:
				return static_cast<IMarker_v1 *>(this);
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
			IError_v1::kESOperationFatal, kIMarkerID, interfaceID, __FILE__, __LINE__);
		return NULL;
	}

	spcIUTF8String IMarker_I::GetName() const {

		return this->GetName_I();
	}
	pcIUTF8String_base IMarker_I::getName(pcIError_base & error) const  __NOTHROW__{
		typedef spcIUTF8String(APICALL IMarker_I::*func)()const;
		func f = &IMarker_I::GetName_I;
		return CallConstUnSafeFunctionReturningSharedPointer<
		IMarker_I, pcIUTF8String_base, const IUTF8String>(error, this, f, __FILE__, __LINE__);
	}

	void IMarker_I::setName(const char * name, sizet nameLen, pcIError_base & error)  {

		typedef void(APICALL IMarker_I::*func)(const char *, sizet);
		func f = &IMarker_I::SetName_I;
		CallUnSafeFunctionReturningVoid<IMarker_I, const char *, sizet>(error, this, f, __FILE__, __LINE__, name, nameLen);

	}

	void IMarker_I::SetName(const char * name, sizet nameLen) {

		return SetName_I(name, nameLen);
	}

	uint64 IMarker_I::setTypes(uint64 inputMask, pcIError_base & error)  {

		typedef uint64(APICALL IMarker_I::*func)(uint64);
		func f = &IMarker_I::SetTypes_I;
		return CallUnSafeFunction<
			IMarker_I, uint64, uint64, uint64
		>(error, this, 0, f, __FILE__, __LINE__, inputMask);
	}

	uint64 IMarker_I::SetTypes(uint64 inputMask) {

		return SetTypes_I(inputMask);
	}

	uint64 IMarker_I::removeTypes(uint64 inputMask, pcIError_base & error)  {

		typedef uint64(APICALL IMarker_I::*func)(uint64);
		func f = &IMarker_I::RemoveTypes_I;
		return CallUnSafeFunction<
			IMarker_I, uint64, uint64, uint64
		>(error, this, 0, f, __FILE__, __LINE__, inputMask);
	}

	uint64 IMarker_I::RemoveTypes(uint64 inputMask) {

		return RemoveTypes_I(inputMask);
	}


	uint64 IMarker_I::getTypes(pcIError_base & error) const  {
		typedef uint64(APICALL IMarker_I::*func)()const;
		func f = &IMarker_I::GetTypes_I;
		return CallConstUnSafeFunction<
			IMarker_I, uint64, uint64
		>(error, this, 0, f, __FILE__, __LINE__);
	}

	uint64 IMarker_I::GetTypes() const {

		return GetTypes_I();
	}

	bool IMarker_I::appendCustomType(const char * userDefinedType, sizet userDefinedTypeLen, pcIError_base & error) {
		
		typedef bool(APICALL IMarker_I::*func)(const char *, sizet);
		func f = &IMarker_I::AppendCustomType_I;
		return CallUnSafeFunction<
			IMarker_I, bool, bool, const char *, sizet
		>(error, this, 0, f, __FILE__, __LINE__, userDefinedType, userDefinedTypeLen);
	}

	bool IMarker_I::AppendCustomType(const char * userDefinedType, sizet userDefinedTypeLen) {

		return AppendCustomType_I(userDefinedType, userDefinedTypeLen);
	}

	bool IMarker_I::removeCustomType(const char * userDefinedType, sizet userDefinedTypeLen, pcIError_base & error) {

		typedef bool(APICALL IMarker_I::*func)(const char *, sizet);
		func f = &IMarker_I::RemoveCustomType_I;
		return CallUnSafeFunction<
			IMarker_I, bool, bool, const char *, sizet
		>(error, this, 0, &IMarker_I::RemoveCustomType_I, __FILE__, __LINE__, userDefinedType, userDefinedTypeLen);
	}

	bool IMarker_I::RemoveCustomType(const char * userDefinedType, sizet userDefinedTypeLen) {

		return RemoveCustomType_I(userDefinedType, userDefinedTypeLen);
	}

	sizet IMarker_I::GetCustomTypeCount() const {

		return GetCustomTypeCount_I();
	}

	sizet IMarker_I::getCustomTypeCount(pcIError_base & error) const {

		typedef sizet(APICALL IMarker_I::*func)()const;
		func f = &IMarker_I::GetCustomTypeCount_I;
		return CallConstUnSafeFunction<
			IMarker_I, sizet, sizet
		>(error, this, 0, f, __FILE__, __LINE__);
	}

	AdobeXMPCommon::spcIUTF8String IMarker_I::GetCustomType(sizet index) const {

		return this->GetCustomType_I(index);
	}

	pcIUTF8String_base IMarker_I::getCustomType(sizet index, pcIError_base & error) const {

		typedef spcIUTF8String(APICALL IMarker_I::*func)(sizet) const;
		func f = &IMarker_I::GetCustomType_I;
		return CallConstUnSafeFunctionReturningSharedPointer< IMarker_I, pcIUTF8String_base, const IUTF8String, sizet >(error, this, f, __FILE__, __LINE__, index);
	}
	

	static sizet CopyInnerListAsListOfRawPointers(const spcIUTF8StringList_I & list,
		pcIUTF8String_base *& array, MemAllocatorFunc allocFunc, pcIError_base & error)
	{
		sizet nElements = list->size();
		array = (pcIUTF8String_base *)allocFunc(nElements * sizeof(pcIUTF8String_base));
		if (array) {
			for (sizet i = 0; i < nElements; i++) {
				const spcIUTF8String & ref = list->at(i);
				//ref->AcquireSharedOwnershipForRawPointer();
				ref->Acquire();
				array[i] = ref.get();
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
		return nElements;
	}

	static spcIUTF8StringList CopyInnerListAsPublicList(const spcIUTF8StringList_I & list) {
		sizet nElements = list->size();
		spcIUTF8StringList newList = MakeUncheckedSharedPointerWithDefaultDelete(
			new cIUTF8StringList(), __FILE__, __LINE__);
		newList->reserve(nElements);
		for (sizet i = 0; i < nElements; i++) {
			newList->push_back(list->at(i));
		}
		return newList;
	}

	static spcICuePointParamList CopyInnerListAsPublicCueList(const spcICuePointParamList_I & list) {
		sizet nElements = list->size();
		spcICuePointParamList newList = MakeUncheckedSharedPointerWithDefaultDelete(
			new cICuePointParamList(), __FILE__, __LINE__);
		newList->reserve(nElements);
		for (sizet i = 0; i < nElements; i++) {
			newList->push_back(std::make_pair(list->at(i).first, list->at(i).second));
		}
		return newList;
	}


	spcIUTF8StringList IMarker_I::GetCustomTypes() const {
		auto list = GetCustomTypes_I();
		return CopyInnerListAsPublicList(list);
	}

	sizet IMarker_I::getCustomTypes(pcIUTF8String_base *& array, MemAllocatorFunc allocFunc,
		pcIError_base & error) const 
	{
		typedef spcIUTF8StringList_I(APICALL IMarker_I::*func)()const;
		func f = &IMarker_I::GetCustomTypes_I;
		auto list = CallConstUnSafeFunction<
		IMarker_I, spcIUTF8StringList_I, spcIUTF8StringList_I
		>(error, this, spcIUTF8StringList_I(), f, __FILE__, __LINE__);

		return CopyInnerListAsListOfRawPointers(list, array, allocFunc, error);
	}

	spcIUTF8StringList IMarker_I::GetCuePointParams(const char * inputKey, sizet inputKeyLen) const {
		auto list = GetCuePointParams_I(inputKey, inputKeyLen);
		return CopyInnerListAsPublicList(list);
	}

	sizet IMarker_I::getCuePointParams(const char * inputKey, sizet inputKeyLen, pcIUTF8String_base *& array, MemAllocatorFunc allocFunc,
		pcIError_base & error) const
	{
		typedef spcIUTF8StringList_I(APICALL IMarker_I::*func)(const char *, sizet)const;
		func f = &IMarker_I::GetCuePointParams_I;
		auto list = CallConstUnSafeFunction<
			IMarker_I, spcIUTF8StringList_I, spcIUTF8StringList_I, const char *, sizet
		>(error, this, spcIUTF8StringList_I(),  f, __FILE__, __LINE__, inputKey, inputKeyLen);

		return CopyInnerListAsListOfRawPointers(list, array, allocFunc, error);
	}

	sizet IMarker_I::removeCuePointParams(const char * inputKey, sizet inputKeyLen, const char * inputValue, sizet inputValueLen, pcIError_base & error) {

		typedef sizet(APICALL IMarker_I::*func)(const char *, sizet, const char *, sizet);
		func f = &IMarker_I::RemoveCuePointParams_I;
		return CallUnSafeFunction<
			IMarker_I, sizet, sizet, const char *, sizet ,const char * ,sizet
		>(error, this, 0, f, __FILE__, __LINE__, inputKey, inputKeyLen, inputValue, inputValueLen);
	}


	sizet IMarker_I::RemoveCuePointParams(const char * inputKey, sizet inputKeyLen, const char * inputValue, sizet inputValueLen) {

		return RemoveCuePointParams_I(inputKey, inputKeyLen, inputValue, inputValueLen);

	}


	sizet IMarker_I::addCuePointParam(const char * inputKey, sizet inputKeyLen, const char * inputValue, sizet inputValueLen, pcIError_base & error) {

		typedef sizet(APICALL IMarker_I::*func)(const char *, sizet, const char *, sizet);
		func f = &IMarker_I::AddCuePointParam_I;
		return CallUnSafeFunction<
			IMarker_I, sizet, sizet, const char *, sizet, const char *, sizet
		>(error, this, 0, f, __FILE__, __LINE__, inputKey, inputKeyLen, inputValue, inputValueLen);
	}


	sizet IMarker_I::AddCuePointParam(const char * inputKey /* = NULL */, sizet inputKeyLen, const char * inputValue /* = NULL */, sizet inputValueLen) {

		return AddCuePointParam_I(inputKey, inputKeyLen, inputValue, inputValueLen);

	}

	sizet IMarker_I::getAllCuePointParams(pcIUTF8String_base *& array, MemAllocatorFunc allocFunc, pcIError_base & error) const  {
		spcICuePointParamList_I list = GetAllCuePointParams_I();
		sizet nElements = list->size() * 2;
		array = (pcIUTF8String_base *)allocFunc(nElements * sizeof(pcIUTF8String_base));
		if (array) {
			for (sizet i = 0; i < nElements; i = i + 2) {
				const spcIUTF8String & ref = list->at(i).first;
				//ref->AcquireSharedObjectForRawPointer();
				ref->Acquire();
				array[i] = ref.get();
				const spcIUTF8String & ref2 = list->at(i).second;
				//ref2->AcquireSharedObjectForRawPointer();
				ref2->Acquire();
				array[i + 1] = ref2.get();
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

	AdobeXMPCore::pIStructureNode_base IMarker_I::addExtension(const char * extensionNameSpace, const char * extensionName, pcIError_base & error) {

		typedef spIStructureNode(APICALL IMarker_I::*func)(const char *, const char *);
		func f = &IMarker_I::AddExtension_I;
		return CallUnSafeFunctionReturningSharedPointer<
			IMarker_I, pIStructureNode_base, IStructureNode, const char *, const char *
		>(error, this, f, __FILE__, __LINE__, extensionNameSpace, extensionName);
	}

	AdobeXMPCore::spIStructureNode IMarker_I::AddExtension(const char * extensionNameSpace, const char * extensionName) {

		return AddExtension(extensionNameSpace, extensionName);
	}

	bool IMarker_I::AddExtension(const char * extensionNameSpace, sizet extensionNameSpaceLen, const char * extensionName, sizet extensionNameLen, pIExtension extension) {

		return AddExtension_I(extensionNameSpace, extensionNameSpaceLen, extensionName, extensionNameLen, extension);
	}

	uint32 IMarker_I::addExtension(const char * extensionNameSpace, sizet extensionNameSpaceLen, const char * extensionName, sizet extensionNameLen, pIExtension extension, pcIError_base & error) {

		typedef bool(APICALL IMarker_I::*func)(const char *, sizet, const char *, sizet, pIExtension);
		func f = &IMarker_I::AddExtension_I;

		return CallUnSafeFunction<
			IMarker_I, uint32, bool, const char *, sizet, const char *, sizet, pIExtension
		>(error, this, 0, &IMarker_I::AddExtension_I, __FILE__, __LINE__, extensionNameSpace, extensionNameSpaceLen, extensionName, extensionNameLen, extension);
	}



	AdobeXMPCore::spIStructureNode IMarker_I::GetExtension(const char * extensionNameSpace, const char * extensionName) {

		return GetExtension_I(extensionNameSpace, extensionName);
	}

	AdobeXMPCore::pIStructureNode_base IMarker_I::removeExtension(const char * extensionNameSpace, const char * extensionName, pcIError_base & error) {

		typedef spIStructureNode(APICALL IMarker_I::*func)(const char *, const char *);
		func f = &IMarker_I::RemoveExtension_I;
		return CallUnSafeFunctionReturningSharedPointer<
			IMarker_I, pIStructureNode_base, IStructureNode, const char *, const char *
		>(error, this, f, __FILE__, __LINE__, extensionNameSpace, extensionName);
	}

	AdobeXMPCore::spIStructureNode IMarker_I::RemoveExtension(const char * extensionNameSpace, const char * extensionName) {

		return RemoveExtension(extensionNameSpace, extensionName);
	}

	AdobeXMPCommon::sizet IMarker_I::removeExtension(const char * extensionNameSpace, sizet extensionNameSpaceLen, const char * extensionName, sizet extensionNameLen, pcIError_base & error) {

		typedef sizet(APICALL IMarker_I::*func)(const char *, sizet, const char *, sizet);
		func f = &IMarker_I::RemoveExtension_I;
		return CallUnSafeFunction<
			IMarker_I, sizet, sizet, const char *, sizet, const char *, sizet
		>(error, this, 0, f, __FILE__, __LINE__, extensionNameSpace, extensionNameSpaceLen, extensionName, extensionNameLen);

	}

	AdobeXMPCommon::sizet IMarker_I::RemoveExtension(const char * extensionNameSpace, sizet extensionNameSpaceLen, const char * extensionName, sizet extensionNameLen) {

		return RemoveExtension_I(extensionNameSpace, extensionNameSpaceLen, extensionName, extensionNameLen);
	}

	spcICuePointParamList IMarker_I::GetAllCuePointParams() const {
		auto list = this->GetAllCuePointParams_I();
		return CopyInnerListAsPublicCueList(list);
	}

	AdobeXMPCore::pIStructureNode_base IMarker_I::getExtension(const char * extensionNameSpace, const char * extensionName, pcIError_base & error) {
	
		typedef spIStructureNode(APICALL IMarker_I::*func)(const char *, const char *);
		func f = &IMarker_I::GetExtension_I;
		return CallUnSafeFunctionReturningSharedPointer<
			IMarker_I, pIStructureNode_base, IStructureNode, const char *, const char *
		>(error, this, f, __FILE__, __LINE__, extensionNameSpace, extensionName);
	}

	AdobeXMPAM::pIExtension IMarker_I::getExtension(const char * extensionNameSpace, sizet extensionNameSpaceLen, const char * extensionName, sizet extensionNameLen, pcIError_base & error) const {

		try {
			return GetExtension_I(extensionNameSpace, extensionNameSpaceLen, extensionName, extensionNameLen);
		}
		catch (pcIError_I & err) {
			//err->AcquireSharedObjectForRawPointer();
			err->Acquire();
			error = err;
		}
		catch (...) {
			/*AdobeXMPCommon_Int::spIError_I _error = AdobeXMPCommon_Int::IError_I::CreateError(
				IError_base::kDomainGeneral,
				IError_base::kGeneralCodeUnknownExceptionCaught,
				IError_base::kSeverityOperationFatal);
			_error->SetLocation(__FILE__, __LINE__);
			_error->SetMessage("Unknown Exception Caught");
			_error->AcquireSharedObjectForRawPointer();
			error = _error.get();*/
		}
		return NULL;
	}

	AdobeXMPAM::pIExtension IMarker_I::GetExtension(const char * extensionNameSpace, sizet extensionNameSpaceLen, const char * extensionName, sizet extensionNameLen) const {

		return GetExtension_I(extensionNameSpace, extensionNameSpaceLen, extensionName, extensionNameLen);
	}

	pIMarker_base IMarker_I::clone(pcIError_base & error) const {

		typedef spIMarker(APICALL IMarker_I::*func)()const;
		func f = &IMarker_I::Clone_I;
		//return pIMarker_base();
		return CallConstUnSafeFunctionReturningSharedPointer< IMarker_I, pIMarker_base, IMarker>(
		error, this, &IMarker_I::Clone_I, __FILE__, __LINE__);
	}

		spIMarker IMarker_I::Clone() const{
#if !GENERATE_XMPEXTENSIONS_CLIENT_LAYER_CODE
		return Clone_I();
#else
		return spIMarker();
#endif
	}

	void IMarker_I::SetFrameCountSpan(uint64 startFrameCount, uint64 countOfFrames) {

		return SetFrameCountSpan_I(startFrameCount, countOfFrames);
	}

	void IMarker_I::setFrameCountSpan(uint64 startFrameCount, uint64 countOfFrames, pcIError_base& error) {

		typedef void(APICALL IMarker_I::*func)(uint64, uint64);
		func f = &IMarker_I::SetFrameCountSpan_I;

		return CallUnSafeFunctionReturningVoid <
			IMarker_I, uint64, uint64 > (error, this, f, __FILE__, __LINE__, startFrameCount, countOfFrames);
	}

	void IMarker_I::setTimeSpan(double startTimeInSeconds, double durationInSeconds, pcIError_base & error) {


		typedef void(APICALL IMarker_I::*func)(double, double);
		func f = &IMarker_I::SetTimeSpan_I;

		return CallUnSafeFunctionReturningVoid<
			IMarker_I, double, double
		>(error, this, f, __FILE__, __LINE__, startTimeInSeconds, durationInSeconds);
	}

	void IMarker_I::SetTimeSpan(double startTimeInSeconds /*= 0*/, double durationInSeconds /*= DBL_MAX*/) {

		return SetTimeSpan_I(startTimeInSeconds, durationInSeconds);
	}

	void IMarker_I::setFrameRateForStartFrameCount(uint64 numerator, uint64 denominator, pcIError_base & error) {

		typedef void(APICALL IMarker_I::*func)(uint64, uint64);
		func f = &IMarker_I::SetFrameRateForStartFrameCount_I;
		return CallUnSafeFunctionReturningVoid<
			IMarker_I, uint64, uint64
		>(error, this, f, __FILE__, __LINE__, numerator, denominator);
	}

	void IMarker_I::SetFrameRateForStartFrameCount(uint64 numerator, uint64 denominator) {

		return SetFrameRateForStartFrameCount_I(numerator, denominator);
	}

	void IMarker_I::setFrameRateForCountOfFrames(uint64 numerator, uint64 denominator, pcIError_base & error) {

		typedef void(APICALL IMarker_I::*func)(uint64, uint64);
		func f = &IMarker_I::SetFrameRateForCountOfFrames_I;
		return CallUnSafeFunctionReturningVoid<
			IMarker_I, uint64, uint64
		>(error, this, f, __FILE__, __LINE__, numerator, denominator);
	}

	void IMarker_I::SetFrameRateForCountOfFrames(uint64 numerator, uint64 denominator) {

		return SetFrameRateForCountOfFrames_I(numerator, denominator);
	}

	void IMarker_I::setFrameRates(uint64 numerator, uint64 denominator, pcIError_base & error) {

		typedef void(APICALL IMarker_I::*func)(uint64, uint64);
		func f = &IMarker_I::SetFrameRates_I;
		return CallUnSafeFunctionReturningVoid<
			IMarker_I, uint64, uint64
		>(error, this, &IMarker_I::SetFrameRates_I, __FILE__, __LINE__, numerator, denominator);
		
	}


	void IMarker_I::SetFrameRates(uint64 numerator, uint64 denominator) {

		return SetFrameRates_I(numerator, denominator);
	}

	void IMarker_I::setProbability(double markerProbability, pcIError_base & error) {

		typedef void(APICALL IMarker_I::*func)( double );
		func f = &IMarker_I::SetProbability_I;
		return CallUnSafeFunctionReturningVoid<
			IMarker_I, double
		>(error, this, f, __FILE__, __LINE__, markerProbability);
	}

	void IMarker_I::SetProbability(double markerProbability) {

		return SetProbability_I(markerProbability);
	}

	spcIUTF8String IMarker_I::GetLocation() const {

		return this->GetLocation_I();
	}
	pcIUTF8String_base IMarker_I::getLocation(pcIError_base & error) const {

		typedef spcIUTF8String(APICALL IMarker_I::*func)() const;
		func f = &IMarker_I::GetLocation_I;
		return CallConstUnSafeFunctionReturningSharedPointer<
		IMarker_I, pcIUTF8String_base, const IUTF8String>(error, this, f, __FILE__, __LINE__);
	}

	spcIUTF8String IMarker_I::GetComment() const {

		return this->GetComment_I();
	}
	pcIUTF8String_base IMarker_I::getComment(pcIError_base & error) const {

		typedef spcIUTF8String(APICALL IMarker_I::*func)() const;
		func f = &IMarker_I::GetComment_I;
		return CallConstUnSafeFunctionReturningSharedPointer<
		IMarker_I, pcIUTF8String_base, const IUTF8String>(error, this, f, __FILE__, __LINE__);
	}

	spcIUTF8String IMarker_I::GetSpeaker() const {

		return this->GetSpeaker_I();
	}

	pcIUTF8String_base IMarker_I::getSpeaker(pcIError_base & error) const {

		typedef spcIUTF8String(APICALL IMarker_I::*func)() const;
		func f = &IMarker_I::GetSpeaker_I;

		return CallConstUnSafeFunctionReturningSharedPointer<
		IMarker_I, pcIUTF8String_base, const IUTF8String>(error, this, f, __FILE__, __LINE__);
	}

	spcIUTF8String IMarker_I::GetTarget() const {

		return this->GetTarget_I();
	}

	pcIUTF8String_base IMarker_I::getTarget(pcIError_base & error) const {

		typedef spcIUTF8String(APICALL IMarker_I::*func)() const;
		func f = &IMarker_I::GetTarget_I;
		return CallConstUnSafeFunctionReturningSharedPointer<
		IMarker_I, pcIUTF8String_base, const IUTF8String>(error, this, f, __FILE__, __LINE__);
	}

	spcIUTF8String IMarker_I::GetGUID() const {

		return this->GetGUID_I();
	}

	pcIUTF8String_base IMarker_I::getGUID(pcIError_base & error) const {

		typedef spcIUTF8String(APICALL IMarker_I::*func)() const;
		func f = &IMarker_I::GetGUID_I;

		return CallConstUnSafeFunctionReturningSharedPointer<
			IMarker_I, pcIUTF8String_base, const IUTF8String>(error, this, f, __FILE__, __LINE__);
	}

	void IMarker_I::setComment(const char * name, sizet nameLen,pcIError_base & error)  {

		typedef void(APICALL IMarker_I::*func)(const char *, sizet);
		func f = &IMarker_I::SetComment_I;
		return CallUnSafeFunctionReturningVoid<
			IMarker_I, const char *, sizet
		>(error, this, f, __FILE__, __LINE__, name, nameLen);
	}

	void IMarker_I::SetComment(const char * name, sizet nameLen) {

		return SetComment_I(name, nameLen);
	}

	void IMarker_I::setSpeaker(const char * speaker, sizet speakerLen, pcIError_base & error)  {

		typedef void(APICALL IMarker_I::*func)( const char *, sizet);
		func f = &IMarker_I::SetSpeaker_I;

		return CallUnSafeFunctionReturningVoid<
			IMarker_I, const char *, sizet
		>(error, this, f, __FILE__, __LINE__, speaker, speakerLen);
	}

	void IMarker_I::SetSpeaker(const char * speaker, sizet speakerLen) {

		return SetSpeaker_I(speaker, speakerLen);
	}

	void IMarker_I::setLocation(const char * location, sizet locationLen, pcIError_base & error)  {

		typedef void(APICALL IMarker_I::*func)(const char*, sizet);
		func f = &IMarker_I::SetLocation_I;

		return CallUnSafeFunctionReturningVoid<
			IMarker_I, const char *, sizet
		>(error, this, f, __FILE__, __LINE__, location, locationLen);
	}

	void IMarker_I::SetLocation(const char * location, sizet locationLen) {

		return SetLocation_I( location, locationLen);
	}

	void IMarker_I::setTarget(const char * target, sizet targetLen, pcIError_base & error)  {

		typedef void(APICALL IMarker_I::*func)( const char *, sizet);
		func f = &IMarker_I::SetTarget_I;

		return CallUnSafeFunctionReturningVoid<
			IMarker_I, const char *, sizet
		>(error, this, f, __FILE__, __LINE__, target, targetLen);
	}

	void IMarker_I::SetTarget(const char * target, sizet targetLen) {

		return SetTarget_I(target, targetLen);
	}

	bool IMarker_I::HasExtension(const char * extensionNameSpace, sizet extensionNameSpaceLen, const char * extensionName, sizet extensionNameLen) const __NOTHROW__{

		return HasExtension_I(extensionNameSpace, extensionNameSpaceLen, extensionName, extensionNameLen);
	}

	uint32 IMarker_I::hasExtension(const char * extensionNameSpace, sizet extensionNameSpaceLen, const char * extensionName, sizet extensionNameLen, pcIError_base & error) const {

		try {
			uint32 returnValue = static_cast<uint32> (HasExtension_I(extensionNameSpace, extensionNameSpaceLen, extensionName, extensionNameLen));
			return returnValue;
		}
		catch (pcIError_base & err) {
			//err->AcquireSharedObjectForRawPointer();
			err->Acquire();
			error = err;
		}
		catch (...) {
		/*	AdobeXMPCommon_Int::spIError_I _error = AdobeXMPCommon_Int::IError_I::CreateError(
				IError_base::kDomainGeneral,
				IError_base::kGeneralCodeUnknownExceptionCaught,
				IError_base::kSeverityOperationFatal);
			_error->SetLocation(__FILE__, __LINE__);
			_error->SetMessage("Unknown Exception Caught");
			_error->AcquireSharedObjectForRawPointer();
			error = _error.get();*/
		}

		return 0;
	}

	AdobeXMPCommon::sizet IMarker_I::RemoveAllCuePointParams() {

		return RemoveAllCuePointParams_I();
	}

	AdobeXMPCommon::uint32 IMarker_I::hasCustomType(const char * customType, sizet customTypeLen, pcIError_base & error) const {

		try {
			uint32 returnValue = static_cast<uint32> (HasCustomType_I(customType, customTypeLen));
			return returnValue;
		}
		catch (pcIError_I & err) {
			//err->AcquireSharedObjectForRawPointer();
			error = err;
		}
		catch (...) {
		/*	AdobeXMPCommon_Int::spIError_I _error = AdobeXMPCommon_Int::IError_I::CreateError(
				IError_base::kDomainGeneral,
				IError_base::kGeneralCodeUnknownExceptionCaught,
				IError_base::kSeverityOperationFatal);
			_error->SetLocation(__FILE__, __LINE__);
			_error->SetMessage("Unknown Exception Caught");
			_error->AcquireSharedObjectForRawPointer();
			error = _error.get();*/
		}

		return 0;
	}

	bool IMarker_I::HasCustomType(const char * customType, sizet customTypeLen) const {

		return HasCustomType_I(customType, customTypeLen);
	}





}


namespace AdobeXMPAM {
	using namespace AdobeXMPAM_Int;

#if SOURCE_COMPILING_XMPEXTENSIONS_LIB

	

	//spIMarker IMarker_v1::CreateMarker(const char * markerName, uint64 markerTypes, const char * markerTypeName, const char * guid, eMultiThreadingSupport multiThreadingSupport = kMultiThreadingConfiguredAtInitialization){
	//	return IMarker_I::CreateMarker_I(markerName, markerTypes, markerTypeName, guid, multiThreadingSupport);
	//}
#endif
}
