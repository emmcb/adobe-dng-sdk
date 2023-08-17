#ifndef __ITemporalAssetUtility_h__
#define __ITemporalAssetUtility_h__ 1

// =================================================================================================
// ADOBE SYSTEMS INCORPORATED
// Copyright 2014 Adobe Systems Incorporated
// All Rights Reserved
//
// NOTICE:  Adobe permits you to use, modify, and distribute this file in accordance with the terms
// of the Adobe license agreement accompanying it.
// =================================================================================================

#if AdobePrivate

#endif	// AdobePrivate

#include "XMPMarker/XMPMarkerFwdDeclarations.h"
#include "XMPCore/XMPCoreFwdDeclarations.h"
#include "XMPCommon/Interfaces/BaseInterfaces/ISharedObject_I.h"
#include "XMPMarker/Interfaces/IMarker.h"
#include <map>
#include <vector>
#include <sstream>
#include "XMPCommon/Utilities/UTF8String.h"
#include "XMPCommon/XMPCommonFwdDeclarations_I.h"
#include "XMPCommon/Interfaces/IUTF8String_I.h"
#include <math.h>

namespace AdobeXMPAM {

		using namespace AdobeXMPCommon;
		using namespace AdobeXMPCommon_Int;
		using namespace AdobeXMPAM_Int;

	
		static std::map<IMarker::eMarkerType, std::string> markerTypeMap;
		static std::map<std::string, uint64> inverseMarkerTypeMap;


		namespace TemporalAssetUtilities {

			void InitMaps();

			bool IsBitSet(uint64 inputMask, uint64 checkMask);

			void ConvertBitMaskToCommaSeparatedString(const uint64 & mask, std::string & str);

			void ConvertCustomTypesToCommaSeparatedString(const std::vector<spIUTF8String> & listOfTypes, std::string & str);

			void CreateCommaSeparatedString(const uint64 & mask, const std::vector < spIUTF8String > & listOfTypes, std::string & str); 

			bool CovertCommaSeparatedType(const std::string & str, std::vector<IMarker::eMarkerType> &type, std::vector<std::string> & typeName);

			void ConvertToList(std::string & commaSeparatedList, const uint64 & typeMask, std::vector < spIUTF8String_I > & customTypes);

			uint64 Parseuint64InStr(const char * strValue, sizet length);

			sizet FindCharacterInStr(const char * string, sizet length, char a);

			sizet RFindCharacterInStr(const char * string, sizet length, char a);

			void ParseFrameRateInStr(const char * value, sizet length, uint64 & numerator, uint64 & denominator);

			void ParseFrameCountInStr(const char * value, sizet length, uint64 & frameCount, uint64 & numerator, uint64 & denominator, bool & isMax);

			void ConvertFrameCountToTime(uint64 inFrameCount, double &outTime, uint64 inFrameRate = 1, uint64 inFrameRateBasis = 1);

			void ConvertTimeToFrameCount(double inTime, uint64 &outFrameCount, bool isStart = 1, uint64 inFrameRate = 1, uint64 inFrameRateBasis = 1);

			void ConvertFrameCountToString(std::string & str, uint64 frameCount = 0, uint64 frameRate = 1, uint64 frameRateBasis = 1, bool isDuration = false);

			void ConvertFrameRateToString(std::string & str, uint64 frameRate = 1, uint64 frameRateBasis = 1);

			void ConvertFrameCountToAnotherFrameRate(uint64 inFrameCount, uint64 &outFrameCount, bool isStart = 1, uint64 inFrameRate = 1, uint64 inFrameRateBasis = 1, uint64 outFrameRate = 1, uint64 outFrameRateBasis = 1);
            
            double ConvertStringToDouble(const std::string & str);
		};
};

#endif // __ITemporalAssetUtility_h__