#ifndef ITrack_I_h__
#define ITrack_I_h__ 1

// =================================================================================================
// ADOBE SYSTEMS INCORPORATED
// Copyright 2014 Adobe Systems Incorporated
// All Rights Reserved
//
// NOTICE:  Adobe permits you to use, modify, and distribute this file in accordance with the terms
// of the Adobe license agreement accompanying it.
// =================================================================================================

#include "XMPMarker/XMPMarkerFwdDeclarations_I.h"
#include "XMPMarker/Interfaces/ITrack.h"
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

	class ITrack_I
		: public virtual ITrack,
		public virtual ISharedObject_I
	{
	public:

		
		//static spITrack_I CreateTrack_I(const char * trackName, IMarker::eMarkerType trackType, eMultiThreadingSupport multiThreadingSupport) __NOTHROW__;

		static spITrack_I CreateTrack_I(const char * trackName, sizet trackNameLen, uint64 trackTypes, const char * trackTypeName = NULL, sizet trackTypeNameLen = 0) __NOTHROW__;

		virtual spIMarker APICALL AddMarker_I(const char * markerName, sizet markerNameLen, uint64 markerTypes, const char * markerTypeName = NULL, sizet markerTypeNameLen = 0, const char * guid = NULL, sizet guidLen = 0) = 0;

		virtual spIMarker APICALL AddMarker(const char * markerName, sizet markerNameLen, uint64 markerTypes, const char * markerTypeName, sizet markerTypeNameLen, const char * guid, sizet guidLen);
		
		virtual spIMarker APICALL AddMarker_I(spIMarker_I  inMarker) = 0;

		virtual spITrack APICALL Clone_I() const = 0;

		virtual void APICALL SetName_I(const char * name, sizet nameLen) = 0;

		virtual uint64 APICALL SetTypes_I(uint64 inputMask) = 0;

		virtual uint64 APICALL RemoveTypes_I(uint64 inputMask) = 0;

		virtual uint64 APICALL GetTypes_I() const = 0;

		virtual bool APICALL AppendCustomType_I(const char * type, sizet typeLen) = 0;

		virtual bool APICALL RemoveCustomType_I(const char * userDefinedType, sizet userDefinedTypeLen) = 0;

		virtual sizet APICALL GetCustomTypeCount_I() const = 0;

		virtual spcIUTF8String APICALL GetCustomType_I(sizet index) const = 0;

		virtual spcIUTF8StringList_I APICALL GetCustomTypes_I() const = 0;

		virtual void APICALL SetFrameRate_I(uint64 numerator = 1 , uint64 denominator = 1 ) = 0;

		virtual spIMarkerList APICALL GetAllMarkers() const;
		
		virtual spIMarkerList_I APICALL GetAllMarkers_I()  const = 0;


		virtual spIMarker APICALL RemoveMarker_I(const sizet & markerIdx) = 0;

		virtual spIMarker APICALL RemoveMarker_I(const char * guid, sizet guidLen) = 0;

		virtual spIMarker APICALL GetMarker_I(const char * guid, sizet guidLen) const = 0;

		virtual spIMarker APICALL GetMarker_I(const sizet & markerIdx) const = 0; 

		virtual void APICALL foo_I(std::vector< std::pair<const char *, sizet> > & v) = 0;  

		virtual spIMarkerList_I APICALL RemoveMarkers_I(uint64 markerTypes = UINT64_MAX, bool matchAllCriteria = false, const std::vector<std::pair<const char *, sizet> > &markerTypeName = std::vector<std::pair< const char *, sizet> >()) = 0;


		virtual spIMarkerList_I APICALL GetMarkers_I(uint64 markerTypes = UINT64_MAX, bool matchAllCriteria = false, const std::vector<std::pair<const char *, sizet> > &markerTypeName = std::vector<std::pair< const char *, sizet> >()) const = 0;

		virtual spIMarkerList_I APICALL GetMarkersByName_I(const char * markerName, sizet markerNameLen) const = 0;

		virtual bool APICALL HasTypes(uint64 markerTypeMask) const {

			uint64 presentMask = GetTypes_I();
			if (presentMask & markerTypeMask) {
				return true;
			}
			return false;
		}
		
		virtual spIMarkerList_I APICALL GetMarkersInTimeRange_I(uint64 startFrameCount = 0, uint64 endFrameCount = UINT64_MAX, eTrackTimeBoundary timeBoundary = kTTBothStartTimeAndEndTimeIncluded, uint64 markerTypes = UINT64_MAX, bool matchAllCriteria = false, const std::vector<std::pair<const char *, sizet> > &markerTypeName = std::vector<std::pair<const char *, sizet> >()) const = 0;

		virtual bool APICALL HasCustomType_I(const char * customType, sizet customTypeLen) const = 0;

		virtual void Serialize_I(spIStructureNode & structNode) const = 0;

		static uint32 GetInterfaceVersion() { return kInternalInterfaceVersionNumber; }

		virtual pvoid APICALL GetInterfacePointer(uint64 interfaceID, uint32 interfaceVersion);

		virtual pITrack APICALL GetActualITrack() __NOTHROW__{ return this; }

		virtual pITrack_I APICALL GetITrack_I() __NOTHROW__{ return this; }

	protected:
		
		virtual pIMarker_base  APICALL addMarker(const char * markerName, sizet markerNameLen, uint64 markerTypes, const char * markerTypeName, sizet markerTypeNameLen, const char * guid, sizet guidLen, pcIError_base & error);

		virtual pIMarker_base APICALL  addMarker(pIMarker_base  inMarker, pcIError_base & error);

		virtual spIMarker  APICALL AddMarker(spIMarker  inMarker);

		virtual pITrack_base  APICALL clone(pcIError_base & error) const;

		virtual spITrack  APICALL Clone() const;

		virtual pcIUTF8String_base APICALL  getName(pcIError_base & error) const __NOTHROW__;

		virtual spcIUTF8String APICALL  GetName() const;

		virtual void APICALL  SetName(const char * markerName, sizet markerNameLen);

		virtual void  APICALL setName(const char * name, sizet nameLen, pcIError_base & error);

		virtual uint64 APICALL  setTypes(uint64 inputMask, pcIError_base & error);

		virtual uint64 APICALL  SetTypes(uint64 inputMask);

		virtual uint64  APICALL removeTypes(uint64 inputMask, pcIError_base & error);

		virtual uint64  APICALL RemoveTypes(uint64 inputMask);

		virtual uint64  APICALL getTypes(pcIError_base & error) const;

		virtual uint64  APICALL GetTypes() const;

		virtual bool  APICALL appendCustomType(const char * userDefinedType, sizet userDefinedTypeLen, pcIError_base & error);

		virtual bool  APICALL AppendCustomType(const char * userDefinedType, sizet userDefinedTypeLen);

		virtual bool  APICALL RemoveCustomType(const char  * userDefinedType, sizet userDefinedTypeLen);

		virtual bool  APICALL removeCustomType(const char  * userDefinedType, sizet userDefinedTypeLen, pcIError_base & error);

		virtual sizet  APICALL GetCustomTypeCount() const;

		virtual sizet  APICALL getCustomTypeCount(pcIError_base & error) const;

		virtual spcIUTF8String  APICALL GetCustomType(sizet index) const;

		virtual pcIUTF8String_base  APICALL getCustomType(sizet index, pcIError_base & error) const;

		virtual spcIUTF8StringList  APICALL GetCustomTypes() const;

		virtual sizet APICALL  getCustomTypes(pcIUTF8String_base *& array, MemAllocatorFunc allocFunc, pcIError_base & error) const;

		virtual spcIUTF8String APICALL GetName_I() const = 0;

		virtual void  APICALL setFrameRate(uint64 numerator, uint64 denominator, pcIError_base & error);

		virtual void  APICALL SetFrameRate(uint64 numerator = 1, uint64 denominator = 1);

		virtual sizet  APICALL getAllMarkers(pIMarker_base *& array, MemAllocatorFunc allocFunc, pcIError_base & error) const;

		virtual pIMarker_base  APICALL removeMarker(const sizet & markerIdx, pcIError_base & error);

		virtual spIMarker  APICALL RemoveMarker(const sizet & markerIdx);

		virtual pIMarker_base  APICALL removeMarker(const char * guid, sizet guidLen, pcIError_base & error);

		virtual spIMarker  APICALL RemoveMarker(const char *  guid, sizet gudLen);

		virtual spIMarker  APICALL GetMarker(const sizet & markerIdx) const;

		virtual spIMarker  APICALL GetMarker(const char * guid, sizet guidLen) const;

		virtual pIMarker_base  APICALL getMarker(const char * guid, sizet guidLen, pcIError_base & error) const;

		virtual pIMarker_base  APICALL getMarker(const sizet & markerIdx, pcIError_base & error) const;

		virtual void  APICALL foo(std::vector<std::pair<const char *, sizet> > & v);


		virtual void  APICALL foo(void * v, sizet size, pcIError_base & error);


		virtual spIMarkerList  APICALL RemoveMarkers(uint64 markerTypes = UINT64_MAX, bool matchAllCriteria = false, const std::vector<std::pair<const char *, sizet> > &markerTypeName = std::vector<std::pair<const char *, sizet> >());

		virtual sizet  APICALL removeMarkers(uint64 markerTypes, uint32 matchAllCriteria, void * rawptr, sizet count, pIMarker_base *&array, MemAllocatorFunc allocFunc, pcIError_base & error);

		virtual spIMarkerList  APICALL GetMarkers(uint64 markerTypes = UINT64_MAX, bool matchAllCriteria = false, const std::vector<std::pair< const char *, sizet> > &markerTypeName = std::vector<std::pair<const char *, sizet> >()) const;

		virtual sizet  APICALL getMarkers(uint64 markerTypes, uint32 matchAllCriteria, void * rawptr, sizet count, pIMarker_base *&array, MemAllocatorFunc allocFunc, pcIError_base & error) const;

		virtual sizet  APICALL getMarkersByName(const char * name, sizet nameLen, pIMarker_base *&array, MemAllocatorFunc allocFunc, pcIError_base & error) const;

		virtual spIMarkerList  APICALL GetMarkersByName(const char * markerName, sizet markerNameLen) const;

		virtual sizet  APICALL getMarkersInTimeRange(uint64 startFrameCount, uint64 endFrameCount, uint32 timeBoundary, uint64 markerTypes, uint32 matchAllCriteria, void * rawptr, sizet count, pIMarker_base *&array, MemAllocatorFunc allocFunc, pcIError_base & error) const;
		
		virtual spIMarkerList  APICALL GetMarkersInTimeRange(uint64 startFrameCount = 0, uint64 endFrameCount = UINT64_MAX, eTrackTimeBoundary timeBoundary = kTTBothStartTimeAndEndTimeIncluded, uint64 markerTypes = UINT64_MAX, bool matchAllCriteria = false, const std::vector<std::pair< const char *, sizet> > &markerTypeName = std::vector<std::pair< const char *, sizet> >()) const;

		
		virtual bool  APICALL HasCustomType(const char * customType, sizet customTypeLen) const;

		virtual uint32  APICALL hasCustomType(const char * customType, sizet customTypeLen, pcIError_base & error) const;

		pvoid APICALL GetInterfacePointerInternal(uint64 interfaceID, uint32 interfaceVersion, bool isTopLevel);

		virtual pvoid APICALL getInterfacePointer(uint64 interfaceID, uint32 interfaceVersion, pcIError_base & error) __NOTHROW__;

		virtual ~ITrack_I() __NOTHROW__;

#ifndef FRIEND_CLASS_DECLARATION
#define FRIEND_CLASS_DECLARATION() 
#endif
		FRIEND_CLASS_DECLARATION();
	};

	inline ITrack_I::~ITrack_I() __NOTHROW__{}
}

#endif  // ITrack_I_h__
