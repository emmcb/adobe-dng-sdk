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
// 11-27-14 SKP 1.0-a017 Implemented Asset composition related functionality for basic part addition.
// 11-25-14 ADC 1.0-a009 Fixing compile and linking issues.
// 11-24-14 ADC 1.0-a008 Support for ICompositionRelationship interface.
//
// =================================================================================================
#endif  // AdobePrivate

#include "XMPAssetManagement/Interfaces/ICompositionRelationship_I.h"
#include "XMPAssetManagement/Interfaces/IAssetPart_I.h"
#include "XMPCommon/Interfaces/IUTF8String_I.h"
#include "XMPCommon/Utilities/TWrapperFunctions_I.h"
#include "XMPCore/Interfaces/IStructureNode.h"

namespace AdobeXMPAM_Int {

	pvoid APICALL ICompositionRelationship_I::GetInterfacePointer(uint64 interfaceID, uint32 interfaceVersion) {
		return GetInterfacePointerInternal(interfaceID, interfaceVersion, true);
	}

	pvoid APICALL ICompositionRelationship_I::getInterfacePointer(uint64 interfaceID, uint32 interfaceVersion, pcIError_base & error) __NOTHROW__{
		return CallUnSafeFunction< ICompositionRelationship_I, pvoid, pvoid, uint64, uint32 >(
		error, this, NULL, &ICompositionRelationship_I::GetInterfacePointer, __FILE__, __LINE__, interfaceID, interfaceVersion);
	}

		pvoid APICALL ICompositionRelationship_I::GetInterfacePointerInternal(uint64 interfaceID, uint32 interfaceVersion, bool isTopLevel) {
		if (interfaceID == kICompositionRelationshipID) {
			switch (interfaceVersion) {
			case 1:
				return static_cast< ICompositionRelationship_v1 * >(this);
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
			IError_v1::kESOperationFatal, kICompositionRelationshipID, interfaceID, __FILE__, __LINE__);
		return NULL;
	}

	/*spcIAssetPart ICompositionRelationship_I::GetComponentPart() const {
		return GetComponentPart_I();
	}*/

	pcIAssetPart_base APICALL ICompositionRelationship_I::getComponentPart( pcIError_base & error ) const __NOTHROW__ {
		return CallConstUnSafeFunctionReturningSharedPointer<
		ICompositionRelationship_v1, pcIAssetPart_base, const IAssetPart
		>(error, this, &ICompositionRelationship_v1::GetComponentPart, __FILE__, __LINE__);
	}

	/*spcIAssetPart ICompositionRelationship_I::GetComposedPart() const {
		return GetComposedPart_I();
	}*/

	pcIAssetPart_base  APICALL ICompositionRelationship_I::getComposedPart(pcIError_base & error) const __NOTHROW__{
		return CallConstUnSafeFunctionReturningSharedPointer<
		ICompositionRelationship_v1, pcIAssetPart_base, const IAssetPart
		>(error, this, &ICompositionRelationship_v1::GetComposedPart, __FILE__, __LINE__);
	}

	spcIUTF8Strings APICALL ICompositionRelationship_I::GetComponentAlternatePaths() const {
		spcIUTF8Strings list = GetComponentAlternatePaths_I();
		sizet nElements = list->size();
		spcIUTF8Strings newList( new cIUTF8Strings( GetComponentAlternatePaths_I()->size() ) );
		for ( sizet i = 0; i < nElements; i++ ) {
			newList->push_back( list->at(i) );
		}
		return newList;
	}

	pcIStructureNode_base APICALL ICompositionRelationship_I::getComponentMetadata(pcIError_base & error) const __NOTHROW__{
		return CallConstUnSafeFunctionReturningSharedPointer<
		ICompositionRelationship_base, pcIStructureNode_base, const IStructureNode
		>(error, this, &ICompositionRelationship_base::GetComponentMetadata, __FILE__, __LINE__);
	}

	pcIStructureNode_base APICALL ICompositionRelationship_I::getComposedMetadata(pcIError_base & error) const __NOTHROW__{
		return CallConstUnSafeFunctionReturningSharedPointer<
			ICompositionRelationship_v1, pcIStructureNode_base, const IStructureNode
		>(error, this, &ICompositionRelationship_v1::GetComposedMetadata, __FILE__, __LINE__);
	}

	pcIUTF8String_base APICALL ICompositionRelationship_I::getMappingFunctionName(pcIError_base & error) const __NOTHROW__{
		return CallConstUnSafeFunctionReturningSharedPointer<
		ICompositionRelationship_v1, pcIUTF8String_base, const IUTF8String
		>(error, this, &ICompositionRelationship_v1::GetMappingFunctionName, __FILE__, __LINE__);
	}

	/*spcIUTF8String ICompositionRelationship_I::GetMappingFunctionName() const {
		return GetMappingFunctionName_I();
	}*/

	uint32 APICALL ICompositionRelationship_I::getMaskMarkersFlag(pcIError_base & error) const __NOTHROW__{
		return CallConstUnSafeFunction<
		ICompositionRelationship_v1, uint32, eCompositionMaskMarkers
		>(error, this, kCompositionMaskMardersMaxValue, &ICompositionRelationship_v1::GetMaskMarkersFlag, __FILE__, __LINE__);
	}

	void APICALL ICompositionRelationship_I::setMaskMarkersFlag(uint32 flag, pcIError_base & error) __NOTHROW__{
		return CallUnSafeFunctionReturningVoid<
		ICompositionRelationship_v1, eCompositionMaskMarkers
		>(error, this, &ICompositionRelationship_v1::SetMaskMarkersFlag, __FILE__, __LINE__, static_cast< eCompositionMaskMarkers>(flag));
	}

	pcIUTF8String_base APICALL ICompositionRelationship_I::getComponentDocumentID(pcIError_base & error) const __NOTHROW__{
		return CallConstUnSafeFunctionReturningSharedPointer<
		ICompositionRelationship_v1, pcIUTF8String_base, const IUTF8String
		>(error, this, &ICompositionRelationship_v1::GetComponentDocumentID, __FILE__, __LINE__);
	}

	/*spcIUTF8String ICompositionRelationship_I::GetComponentDocumentID() const {
		return GetComponentDocumentID_I();
	}*/

	pcIUTF8String_base APICALL ICompositionRelationship_I::getComponentInstanceID(pcIError_base & error) const __NOTHROW__{
		return CallConstUnSafeFunctionReturningSharedPointer<
		ICompositionRelationship_v1, pcIUTF8String_base, const IUTF8String
		>(error, this, &ICompositionRelationship_v1::GetComponentInstanceID, __FILE__, __LINE__);
	}

	/*spcIUTF8String ICompositionRelationship_I::GetComponentInstanceID() const {
		return GetComponentInstanceID_I();
	}*/

	pcIUTF8String_base APICALL ICompositionRelationship_I::getComposedDocumentID(pcIError_base & error) const __NOTHROW__{
		return CallConstUnSafeFunctionReturningSharedPointer<
		ICompositionRelationship_v1, pcIUTF8String_base, const IUTF8String
		>(error, this, &ICompositionRelationship_v1::GetComposedDocumentID, __FILE__, __LINE__);
	}

	/*spcIUTF8String ICompositionRelationship_I::GetComposedDocumentID() const {
		return GetComposedDocumentID_I();
	}*/

	pcIUTF8String_base APICALL ICompositionRelationship_I::getComposedInstanceID(pcIError_base & error) const __NOTHROW__{
		return CallConstUnSafeFunctionReturningSharedPointer<
		ICompositionRelationship_v1, pcIUTF8String_base, const IUTF8String
		>(error, this, &ICompositionRelationship_v1::GetComposedInstanceID, __FILE__, __LINE__);
	}

	/*spcIUTF8String ICompositionRelationship_I::GetComposedInstanceID() const {
		return GetComposedInstanceID_I();
	}*/

	pcIUTF8String_base APICALL ICompositionRelationship_I::getComponentFilePath(pcIError_base & error) const __NOTHROW__{
		return CallConstUnSafeFunctionReturningSharedPointer<
		ICompositionRelationship_v1, pcIUTF8String_base, const IUTF8String
		>(error, this, &ICompositionRelationship_v1::GetComponentFilePath, __FILE__, __LINE__);
	}

	/*spcIUTF8String ICompositionRelationship_I::GetComponentFilePath() const {
		return GetComponentFilePath_I();
	}*/

	void APICALL ICompositionRelationship_I::appendComponentAlternatePath(const char * alternatePath, sizet altPathlength, pcIError_base & error) __NOTHROW__{
		return CallUnSafeFunctionReturningVoid<
		ICompositionRelationship_v1, const char *, sizet
		>(error, this, &ICompositionRelationship_v1::AppendComponentAlternatePath, __FILE__, __LINE__, alternatePath, altPathlength);
	}

}

namespace AdobeXMPAM {
	using namespace AdobeXMPAM_Int;

#if SOURCE_COMPILING_XMPEXTENSIONS_LIB
	
#endif
}