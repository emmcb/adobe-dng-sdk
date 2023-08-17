#ifndef IMarker_I_h__
#define IMarker_I_h__ 1

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
//  AJ	Abhishek Jindal
//
// mm-dd-yy who Description of changes, most recent first.
//
//
//
// =================================================================================================
#endif  // AdobePrivate

#include "XMPMarker/XMPMarkerFwdDeclarations_I.h"
#include "XMPMarker/Interfaces/IMarker.h"
#include "XMPCore/XMPCoreFwdDeclarations_I.h"

#include "XMPCommon/Interfaces/BaseInterfaces/ISharedObject_I.h"

namespace AdobeXMPAM_Int {
	using namespace AdobeXMPCommon;
	using namespace AdobeXMPCommon_Int;
	using namespace AdobeXMPAM;
	using namespace AdobeXMPCore_Int;
	/*using namespace NS_INT_XMPCORE;*/

#if XMP_WinBuild
	#pragma warning( push )
	#pragma warning( disable : 4250 )
#endif



	class IMarker_I
		: public virtual IMarker,
		public virtual ISharedObject_I
	{
	public:

		virtual spcIUTF8String APICALL GetName_I() const = 0;

		

		static spIMarker CreateMarker(const char * markerName, uint64 markerTypes, const char * markerTypeName = NULL, const char * guid = NULL);
		
		static spIMarker_I CreateMarker_I(const char * markerName, sizet markerNameLen, uint64 markerTypes, const char * markerTypeName = NULL, sizet markerTypeNameLen = 0, const char * guid = NULL, sizet guidLen = 0);

		virtual void APICALL SetName_I(const char * name, sizet nameLen) = 0;

		virtual uint64 APICALL SetTypes_I(uint64 inputMask) = 0;

		virtual uint64 APICALL RemoveTypes_I(uint64 inputMask) = 0;

		virtual uint64 APICALL GetTypes_I() const = 0;

		virtual bool APICALL AppendCustomType_I(const char * type, sizet typeLen) = 0;

		virtual bool APICALL RemoveCustomType_I(const char * userDefinedType, sizet userDefinedTypeLen) = 0;
		
		virtual sizet APICALL GetCustomTypeCount_I() const = 0;

		virtual spcIUTF8String APICALL GetCustomType_I(sizet index) const = 0;

		virtual spcIUTF8StringList_I APICALL GetCustomTypes_I() const = 0;

		virtual spcIUTF8StringList_I APICALL GetCuePointParams_I(const char * inputKey, sizet inputKeyLen) const = 0;

		virtual sizet APICALL RemoveCuePointParams_I(const char * inputKey, sizet inputKeyLen, const char * inputValue = NULL, sizet inputValueLen = 0) = 0;

		virtual sizet APICALL AddCuePointParam_I(const char * inputKey, sizet inputKeyLen, const char * inputValue, sizet inputValueLen) = 0;

		virtual spcICuePointParamList_I APICALL GetAllCuePointParams_I() const = 0;

		virtual spIStructureNode APICALL AddExtension_I(const char * extensionNameSpace, const char * extensionName) = 0;

		virtual spIStructureNode APICALL GetExtension_I(const char * extensionNameSpace, const char * extensionName) = 0;

		virtual spIStructureNode APICALL RemoveExtension_I(const char * extensionNameSpace, const char * extensionName) = 0;

		virtual spIMarker APICALL Clone_I() const = 0;

		virtual void APICALL SetFrameCountSpan_I(uint64 startFrameCount = 0, uint64 countOfFrames = Max_XMP_Uns64) = 0;

		virtual void APICALL SetTimeSpan_I(double startTimeInSeconds, double durationInSeconds) = 0;

		virtual void APICALL SetFrameRateForStartFrameCount_I(uint64 numerator, uint64 denominator) = 0;

		virtual void APICALL SetFrameRateForCountOfFrames_I(uint64 numerator, uint64 denominator) = 0;

		virtual void APICALL SetFrameRates_I(uint64 numerator, uint64 denominator) = 0;

		virtual void APICALL SetProbability_I(double markerProbability) = 0;

		virtual spcIUTF8String APICALL GetLocation_I() const = 0;

		virtual spcIUTF8String APICALL GetSpeaker_I() const = 0;

		virtual spcIUTF8String APICALL GetTarget_I() const = 0;

		virtual spcIUTF8String APICALL GetComment_I() const = 0;

		virtual spcIUTF8String APICALL GetGUID_I() const = 0;

		virtual void APICALL SetLocation_I(const char * location, sizet locationLen) = 0 ;

		virtual void APICALL SetSpeaker_I(const char * speaker, sizet speakerLen) = 0;

		virtual void APICALL SetTarget_I(const char * target, sizet targetLen) = 0;

		virtual void APICALL SetComment_I(const char * name, sizet nameLen) = 0;

		virtual void APICALL SetGUID_I(const char *guid) = 0;

		virtual void APICALL SetExtension_I(spINode extension) = 0;

		virtual void Serialize_I(spIStructureNode & node) = 0;


		virtual weak_ptr<ITrack_I> GetAssociatedTrack_I() const  =0;

		virtual void SetAssociatedTrack_I(spITrack_I track) = 0;



		virtual bool APICALL HasTypes(uint64 markerTypeMask) const {

			uint64 presentMask = GetTypes_I();
			if (presentMask & markerTypeMask) {
				return true;
			}
			return false;
		}

		virtual bool APICALL AddExtension_I(const char * extensionNameSpace, sizet extensionNameSpaceLen, const char * extensionName, sizet extensionNameLen, pIExtension extension) = 0;

		virtual pIExtension GetExtension_I(const char * extensionNameSpace, sizet extensionNameSpaceLen, const char * extensionName, sizet extensionNameLen) const = 0;

		virtual sizet APICALL RemoveAllExtensions() __NOTHROW__ = 0;

		virtual sizet APICALL GetExtensionCount() const __NOTHROW__ = 0;


		virtual bool HasExtension_I(const char * extensionNameSpace, sizet extensionNameSpaceLen, const char * extensionName, sizet extensionNameLen) const __NOTHROW__ = 0;


		virtual sizet APICALL RemoveExtension_I(const char  * extensionNameSpace, sizet extensionNameSpaceLen, const char * extensionName, sizet extensionNameLen) = 0;

		virtual sizet RemoveAllCuePointParams_I() = 0;

		virtual bool HasCustomType_I(const char * customType, sizet customTypeLen) const = 0;

		static uint32 GetInterfaceVersion() { return kInternalInterfaceVersionNumber; }

		virtual pvoid APICALL GetInterfacePointer(uint64 interfaceID, uint32 interfaceVersion);


		virtual pIMarker APICALL GetActualIMarker() __NOTHROW__{ return this; }
		virtual pIMarker_I APICALL GetIMarker_I() __NOTHROW__{ return this; }
		
	protected:

		virtual pcIUTF8String_base APICALL  getName(pcIError_base & error) const __NOTHROW__;

		virtual spcIUTF8String  APICALL GetName() const;
	
		virtual void APICALL  SetName(const char * markerName, sizet markerNameLen);

		virtual void APICALL  setName(const char * name, sizet nameLen, pcIError_base & error);

		virtual uint64  APICALL setTypes(uint64 inputMask, pcIError_base & error);

		virtual uint64  APICALL SetTypes(uint64 inputMask);

		virtual uint64  APICALL removeTypes(uint64 inputMask, pcIError_base & error);

		virtual uint64  APICALL RemoveTypes(uint64 inputMask);

		virtual uint64 APICALL  getTypes(pcIError_base & error) const;

		virtual uint64  APICALL GetTypes() const;

		virtual bool  APICALL appendCustomType(const char * userDefinedType, sizet userDefinedTypeLen, pcIError_base & error);

		virtual bool APICALL  AppendCustomType(const char * userDefinedType, sizet userDefinedTypeLen);

		virtual bool  APICALL RemoveCustomType(const char  * userDefinedType, sizet userDefinedTypeLen);

		virtual bool  APICALL removeCustomType(const char  * userDefinedType, sizet userDefinedTypeLen, pcIError_base & error);

		virtual sizet  APICALL GetCustomTypeCount() const;

		virtual sizet APICALL  getCustomTypeCount(pcIError_base & error) const;

		virtual spcIUTF8String  APICALL GetCustomType(sizet index) const;

		virtual pcIUTF8String_base APICALL  getCustomType(sizet index, pcIError_base & error) const;

		virtual spcIUTF8StringList APICALL  GetCustomTypes() const;

		virtual sizet  APICALL getCustomTypes(pcIUTF8String_base *& array, MemAllocatorFunc allocFunc, pcIError_base & error) const;

		virtual spcIUTF8StringList  APICALL GetCuePointParams(const char * inputKey, sizet inputKeyLen) const;

		virtual sizet  APICALL getCuePointParams(const char * inputKey, sizet inputKeyLen, pcIUTF8String_base *& array, MemAllocatorFunc allocFunc, pcIError_base & error) const;

		virtual sizet APICALL  RemoveCuePointParams(const char * inputKey, sizet inputKeyLen, const char * inputValue, sizet inputValueLen);

		virtual sizet  APICALL removeCuePointParams(const char * inputKey, sizet inputKeyLen, const char * inputValue, sizet inputValueLen, pcIError_base & error);

		virtual sizet APICALL AddCuePointParam(const char * inputKey, sizet inputKeyLen, const char *inputValue, sizet inputValueLen);

		virtual sizet APICALL addCuePointParam(const char * inputKey, sizet inputKeyLen, const char *inputValue, sizet inputValueLen, pcIError_base & error);

		virtual pIStructureNode_base  APICALL addExtension(const char * extensionNameSpace, const char * extensionName, pcIError_base & error);

		virtual spIStructureNode  APICALL AddExtension(const char * extensionNameSpace, const char * extensionName);

		virtual pIStructureNode_base  APICALL getExtension(const char * extensionNameSpace, const char * extensionName, pcIError_base & error);

		virtual spIStructureNode  APICALL GetExtension(const char * extensionNameSpace, const char * extensionName);

		virtual pIStructureNode_base  APICALL removeExtension(const char * extensionNameSpace, const char * extensionName, pcIError_base & error);

		virtual spIStructureNode  APICALL RemoveExtension(const char * extensionNameSpace, const char * extensionName);

		virtual spIMarker  APICALL Clone() const;

		virtual pIMarker_base  APICALL clone(pcIError_base & error) const;

		virtual void  APICALL SetFrameCountSpan(uint64 startFrameCount = 0, uint64 countOfFrames = Max_XMP_Uns64);

		virtual void  APICALL setFrameCountSpan(uint64 startFrameCount, uint64 countOfFrames, pcIError_base& error);

		virtual void  APICALL setTimeSpan(double startTimeInSeconds, double durationInSeconds, pcIError_base & error);

		virtual void  APICALL SetTimeSpan(double startTimeInSeconds = 0, double durationInSeconds = DBL_MAX);

		//using IMarker_v1::GetAllCuePointParams;
		virtual spcICuePointParamList APICALL GetAllCuePointParams() const;


		virtual sizet  APICALL getAllCuePointParams(pcIUTF8String_base *& array, MemAllocatorFunc allocFunc, pcIError_base & error) const;

		virtual void  APICALL setFrameRateForStartFrameCount(uint64 numerator, uint64 denominator, pcIError_base & error);

		virtual void  APICALL setFrameRateForCountOfFrames(uint64 numerator, uint64 denominator, pcIError_base & error);

		virtual void  APICALL setFrameRates(uint64 numerator, uint64 denominator, pcIError_base & error);

		virtual void  APICALL SetFrameRateForStartFrameCount(uint64 numerator, uint64 denominator);

		virtual void  APICALL SetFrameRateForCountOfFrames(uint64 numerator, uint64 denominator);

		virtual void  APICALL SetFrameRates(uint64 numerator, uint64 denominator);

		virtual void  APICALL setProbability(double markerProbability, pcIError_base & error);

		virtual void  APICALL SetProbability(double markerProbability);

		virtual pcIUTF8String_base  APICALL getComment(pcIError_base &error) const;

		virtual pcIUTF8String_base APICALL  getLocation(pcIError_base &error) const;

		virtual pcIUTF8String_base APICALL  getSpeaker(pcIError_base &error) const;

		virtual pcIUTF8String_base APICALL  getTarget(pcIError_base &error) const;

		virtual spcIUTF8String  APICALL GetComment() const;

		virtual spcIUTF8String APICALL  GetLocation() const;

		virtual spcIUTF8String  APICALL GetSpeaker() const;

		virtual spcIUTF8String  APICALL GetTarget() const;

		virtual pcIUTF8String_base  APICALL getGUID(pcIError_base &error) const;

		virtual spcIUTF8String  APICALL GetGUID() const;

		virtual void  APICALL setLocation(const char * location, sizet locationLen, pcIError_base & error);

		virtual void  APICALL setSpeaker(const char * speaker, sizet speakerLen, pcIError_base & error);

		virtual void  APICALL setTarget(const char * target, sizet targetLen, pcIError_base & error);

		virtual void  APICALL setComment(const char * comment, sizet commentLen, pcIError_base & error);

		virtual void   APICALL SetLocation(const char * location, sizet locationLen);

		virtual void  APICALL  SetSpeaker(const char * speaker, sizet speakerLen);

		virtual void   APICALL SetTarget(const char * target, sizet targetLen);

		virtual void  APICALL  SetComment(const char * markerComment, sizet markerCommentLen);

		virtual bool  APICALL  AddExtension(const char * extensionNameSpace, sizet extensionNameSpaceLen, const char * extensionName, sizet extensionNameLen, pIExtension extension);

		virtual uint32  APICALL  addExtension(const char * extensionNameSpace, sizet extensionNameSpaceLen, const char * extensionName, sizet extensionNameLen, pIExtension extension, pcIError_base & error);


		virtual pIExtension  APICALL getExtension(const char * extensionNameSpace, sizet extensionNameSpaceLen, const char * extensionName, sizet extensionNameLen, pcIError_base & error) const;

		virtual pIExtension  APICALL GetExtension(const char * extensionNameSpace, sizet extensionNameSpaceLen, const char * extensionName, sizet extensionNameLen) const;

		
		virtual bool  APICALL HasExtension(const char * extensionNameSpace, sizet extensionNameSpaceLen, const char * extensionName, sizet extensionNameLen) const __NOTHROW__;

		virtual uint32  APICALL hasExtension(const char * extensionNameSpace, sizet extensionNameSpaceLen, const char * extensionName, sizet extensionNameLen, pcIError_base & error) const;


		virtual sizet  APICALL removeExtension(const char  * extensionNameSpace, sizet extensionNameSpaceLen, const char * extensionName, sizet extensionNameLen, pcIError_base & error);

		virtual sizet  APICALL RemoveExtension(const char  * extensionNameSpace, sizet extensionNameSpaceLen, const char * extensionName, sizet extensionNameLen);

		virtual sizet  APICALL RemoveAllCuePointParams();

		virtual uint32  APICALL hasCustomType(const char * customType, sizet customTypeLen, pcIError_base & error) const;

		virtual bool  APICALL HasCustomType(const char * customType, sizet customTypeLen) const;

		pvoid APICALL GetInterfacePointerInternal(uint64 interfaceID, uint32 interfaceVersion, bool isTopLevel);

		virtual pvoid APICALL getInterfacePointer(uint64 interfaceID, uint32 interfaceVersion, pcIError_base & error) __NOTHROW__;

		virtual ~IMarker_I() __NOTHROW__;

#ifndef FRIEND_CLASS_DECLARATION
#define FRIEND_CLASS_DECLARATION() 
#endif
		FRIEND_CLASS_DECLARATION();
	};

	inline IMarker_I::~IMarker_I() __NOTHROW__{}
}

#endif  // IMarker_I_h__
