#include "XMPMarker/ImplHeaders/TemporalAssetUtilitiesImpl.h"
#include "XMPCommon/Interfaces/IUTF8String.h"

namespace AdobeXMPAM {

	namespace TemporalAssetUtilities {

		void InitMaps() {

			if (!markerTypeMap.size()) {
				markerTypeMap.insert(std::make_pair(IMarker_v1::kMTComment, "Comment"));
				markerTypeMap.insert(std::make_pair(IMarker_v1::kMTChapter, "Chapter"));
				markerTypeMap.insert(std::make_pair(IMarker_v1::kMTFLVCuePoint, "FLVCuePoint"));
				markerTypeMap.insert(std::make_pair(IMarker_v1::kMTWebLink, "WebLink"));
				markerTypeMap.insert(std::make_pair(IMarker_v1::kMTCue, "Cue"));
				markerTypeMap.insert(std::make_pair(IMarker_v1::kMTBeat, "Beat"));
				markerTypeMap.insert(std::make_pair(IMarker_v1::kMTTrack, "Track"));
				markerTypeMap.insert(std::make_pair(IMarker_v1::kMTIndex, "Index"));
				markerTypeMap.insert(std::make_pair(IMarker_v1::kMTSpeech, "Speech"));		// Text created by speech recognition technology, manually entered or other means.
				markerTypeMap.insert(std::make_pair(IMarker_v1::kMTSubClip, "InOut"));				// Only written by Prelude (SilkRoad))
				markerTypeMap.insert(std::make_pair(IMarker_v1::kMTSegmentation, "Segmentation"));
				markerTypeMap.insert(std::make_pair(IMarker_v1::kMTAlertVideoTooLow, "AlertVideoTooLow"));
				markerTypeMap.insert(std::make_pair(IMarker_v1::kMTAlertVideoTooHigh, "AlertVideoTooHigh"));
				markerTypeMap.insert(std::make_pair(IMarker_v1::kMTAlertAudioClippingLeft, "AlertAudioClippingLeft"));
				markerTypeMap.insert(std::make_pair(IMarker_v1::kMTAlertAudioClippingRight, "AlertAudioClippingRight"));
				markerTypeMap.insert(std::make_pair(IMarker_v1::kMTAlertAudioLowVolumeLeft, "AlertAudioLowVolumeLeft"));
				markerTypeMap.insert(std::make_pair(IMarker_v1::kMTAlertAudioLowVolumeRight, "AlertAudioLowVolumeRight"));
				markerTypeMap.insert(std::make_pair(IMarker_v1::kMTAlertAudioLoudNoiseLeft, "AlertAudioLoudNoiseLeft"));
				markerTypeMap.insert(std::make_pair(IMarker_v1::kMTAlertAudioLoudNoiseRight, "AlertAudioLoudNoiseRight"));
				markerTypeMap.insert(std::make_pair(IMarker_v1::kMTAlertAudioHummingLeft, "AlertAudioHummingLeft"));
				markerTypeMap.insert(std::make_pair(IMarker_v1::kMTAlertAudioHummingRight, "AlertAudioHummingRight"));
				markerTypeMap.insert(std::make_pair(IMarker_v1::kMTAlertAudioNoSignalLeft, "AlertAudioNoSignalLeft"));
				markerTypeMap.insert(std::make_pair(IMarker_v1::kMTAlertAudioNoSignalRight, "AlertAudioNoSignalRight"));
				markerTypeMap.insert(std::make_pair(IMarker_v1::kMTFLVCuePoint_Navigation, "Navigation"));
				markerTypeMap.insert(std::make_pair(IMarker_v1::kMTFLVCuePoint_Event, "Event"));

				inverseMarkerTypeMap.insert(std::make_pair("Comment", IMarker_v1::kMTComment));
				inverseMarkerTypeMap.insert(std::make_pair("Chapter", IMarker_v1::kMTChapter));
				inverseMarkerTypeMap.insert(std::make_pair("FLVCuePoint", IMarker_v1::kMTFLVCuePoint));
				inverseMarkerTypeMap.insert(std::make_pair("WebLink", IMarker_v1::kMTWebLink));
				inverseMarkerTypeMap.insert(std::make_pair("Cue", IMarker_v1::kMTCue));
				inverseMarkerTypeMap.insert(std::make_pair("Beat", IMarker_v1::kMTBeat));
				inverseMarkerTypeMap.insert(std::make_pair("Track", IMarker_v1::kMTTrack));
				inverseMarkerTypeMap.insert(std::make_pair("Index", IMarker_v1::kMTIndex));
				inverseMarkerTypeMap.insert(std::make_pair("Speech", IMarker_v1::kMTSpeech));		// Text created by speech recognition technology, manually entered or other means.
				inverseMarkerTypeMap.insert(std::make_pair("InOut", IMarker_v1::kMTSubClip));				// Only written by Prelude (SilkRoad))
				inverseMarkerTypeMap.insert(std::make_pair("Segmentation", IMarker_v1::kMTSegmentation));
				inverseMarkerTypeMap.insert(std::make_pair("AlertVideoTooLow", IMarker_v1::kMTAlertVideoTooLow));
				inverseMarkerTypeMap.insert(std::make_pair("AlertVideoTooHigh", IMarker_v1::kMTAlertVideoTooHigh));
				inverseMarkerTypeMap.insert(std::make_pair("AlertAudioClippingLeft", IMarker_v1::kMTAlertAudioClippingLeft));
				inverseMarkerTypeMap.insert(std::make_pair("AlertAudioClippingRight", IMarker_v1::kMTAlertAudioClippingRight));
				inverseMarkerTypeMap.insert(std::make_pair("AlertAudioLowVolumeLeft", IMarker_v1::kMTAlertAudioLowVolumeLeft));
				inverseMarkerTypeMap.insert(std::make_pair("AlertAudioLowVolumeRight", IMarker_v1::kMTAlertAudioLowVolumeRight));
				inverseMarkerTypeMap.insert(std::make_pair("AlertAudioLoudNoiseLeft", IMarker_v1::kMTAlertAudioLoudNoiseLeft));
				inverseMarkerTypeMap.insert(std::make_pair("AlertAudioLoudNoiseRight", IMarker_v1::kMTAlertAudioLoudNoiseRight));
				inverseMarkerTypeMap.insert(std::make_pair("AlertAudioHummingLeft", IMarker_v1::kMTAlertAudioHummingLeft));
				inverseMarkerTypeMap.insert(std::make_pair("AlertAudioHummingRight", IMarker_v1::kMTAlertAudioHummingRight));
				inverseMarkerTypeMap.insert(std::make_pair("AlertAudioNoSignalLeft", IMarker_v1::kMTAlertAudioNoSignalLeft));
				inverseMarkerTypeMap.insert(std::make_pair("AlertAudioNoSignalRight", IMarker_v1::kMTAlertAudioNoSignalRight));
				inverseMarkerTypeMap.insert(std::make_pair("Navigation", IMarker_v1::kMTFLVCuePoint_Navigation));
				inverseMarkerTypeMap.insert(std::make_pair("Event", IMarker_v1::kMTFLVCuePoint_Event));
			}
		}


		bool IsBitSet(uint64 inputMask, uint64 checkMask) {

			if (inputMask & checkMask) {
				return true;
			}
			return false;
		}

		void ConvertBitMaskToCommaSeparatedString(const uint64 & mask, std::string & str) {

			InitMaps();
			bool isFirst = true;
			for (uint64 i = 0; i < 32; i++) {

				if (IsBitSet(mask, 1 << i) && markerTypeMap.find(1 << i) != markerTypeMap.end()) {

					if (isFirst) {

						str += markerTypeMap[1 << i];
						isFirst = false;
					}
					else {

						str += "," + markerTypeMap[1 << i];
					}
				}
			}
		}

		void ConvertCustomTypesToCommaSeparatedString(const std::vector<spIUTF8String> & listOfTypes, std::string & str) {

			InitMaps();
			bool isFirst = (str.length() == 0);
			for (size_t idx = 0, count = listOfTypes.size(); idx < count; ++idx) {

				if (isFirst) {

					str += listOfTypes[idx]->c_str();
					isFirst = false;
				}
				else {

					str += ",";
					str += listOfTypes[idx]->c_str();
				}
			}
		}

		bool CovertCommaSeparatedType(const std::string & str, std::vector<IMarker::eMarkerType> &type, std::vector<std::string> & typeName) {
			InitMaps();
			std::stringstream sstream(str);

			std::string singleType;

			try {
				/*	while (sstream >> singleType)
				{
				if (inverseMarkerTypeMap.count(singleType)) {
				type.push_back(inverseMarkerTypeMap[singleType]);
				}
				else {
				type.push_back(IMarker::kMTUserDefinedMarker);
				typeName.push_back(singleType);
				}

				if (sstream.peek() == ',') {
				sstream.ignore();
				}
				}*/

				while (std::getline(sstream, singleType, ',')) {
					if (inverseMarkerTypeMap.count(singleType)) {
						type.push_back(inverseMarkerTypeMap[singleType]);
					}
					else {
						type.push_back(IMarker::kMTUserDefinedMarker);
						typeName.push_back(singleType);
					}

				}
			}

			catch (...) {
				return false;
			}

			return true;
		}

		void ConvertToList(std::string & commaSeparatedList, const uint64 & typeMask, std::vector < spIUTF8String_I > & customTypes) {


		}

		void CreateCommaSeparatedString(const uint64 & mask, const std::vector < spIUTF8String > & listOfTypes, std::string & str) {
			InitMaps();
			ConvertBitMaskToCommaSeparatedString(mask, str);
			ConvertCustomTypesToCommaSeparatedString(listOfTypes, str);
		}

		uint64 Parseuint64InStr(const char * strValue, sizet length) {



			if (length == 0) {
				/*NOTIFY_ERROR(IError_v1::kDomainGeneral, IError_v1::kGeneralCodeParametersNotAsExpected,
				"Parameter is not an unsinged integer", IError_v1::kSeverityOperationFatal, true, strValue);*/
			}
			UTF8String utf8Value;
			UTF8StringStream ss;

			utf8Value.assign(strValue, length);
			if (utf8Value.find_first_not_of("0123456789") == std::string::npos) {
				ss.str(utf8Value);
				uint64 number;
				ss >> number;
				return number;
			}
			else {
				/*NOTIFY_ERROR(IError_v1::kDomainGeneral, IError_v1::kGeneralCodeParametersNotAsExpected,
				"Parameter is not an unsinged integer", IError_v1::kSeverityOperationFatal, true, utf8Value.c_str());*/
			}
			return Max_XMP_Uns64;
		}

	sizet FindCharacterInStr(const char * string, sizet length, char a) {
			for (sizet i = 0; i < length; i++) {
				if (string[i] == a)
					return i;
			}
			return std::string::npos;
		}

		sizet RFindCharacterInStr(const char * string, sizet length, char a) {
			for (sizet i = length; i > 0; i--) {
				if (string[i - 1] == a)
					return i - 1;
			}
			return std::string::npos;
		}

		void ParseFrameRateInStr(const char * value, sizet length, uint64 & numerator, uint64 & denominator) {

			numerator = 1; denominator = 1;

			sizet fPos = FindCharacterInStr(value, length, 'f');

			if (std::string::npos != fPos) {
				sizet fPos2 = RFindCharacterInStr(value, length, 'f');
				if (fPos != fPos2) {
					/*NOTIFY_ERROR(IError_v1::kDomainGeneral, IError_v1::kGeneralCodeParametersNotAsExpected,
					"Parameter is not a frame rate value", IError_v1::kSeverityOperationFatal, true, value);*/
				}
				else {

					const char * newValue = value + fPos + 1;
					sizet newLength = length - fPos - 1;
					sizet sPos = FindCharacterInStr(newValue, newLength, 's');
					if (std::string::npos != sPos) {
						sizet sPos2 = RFindCharacterInStr(newValue, newLength, 's');
						if (sPos != sPos2) {
							/*NOTIFY_ERROR(IError_v1::kDomainGeneral, IError_v1::kGeneralCodeParametersNotAsExpected,
							"Parameter is not a frame rate value", IError_v1::kSeverityOperationFatal, true, value);*/
						}
						else {
							numerator = Parseuint64InStr(value + fPos + 1, sPos - fPos - 1);
							denominator = Parseuint64InStr(value + sPos + 2, length - sPos - 2);
						}
					}
					else { // new string only contains numerator
						numerator = Parseuint64InStr(value + fPos + 1, newLength);
					}
				}
			}
			else { // it simply contains frameCount

				/*NOTIFY_ERROR(IError_v1::kDomainGeneral, IError_v1::kGeneralCodeParametersNotAsExpected,
				"Parameter is not a frame rate value", IError_v1::kSeverityOperationFatal, true, value);*/
			}
		}

		void ParseFrameCountInStr(const char * value, sizet length, uint64 & frameCount, uint64 & numerator, uint64 & denominator, bool & isMax) {
			frameCount = 0; numerator = 1; denominator = 1;
			static const std::string maximumStr = "maximum";
			if (length == maximumStr.length()) {

				if (!strcmp(value, maximumStr.c_str())) {

					isMax = true;
					return;
				}
			}
			isMax = false;
			sizet fPos = FindCharacterInStr(value, length, 'f');

			if (std::string::npos != fPos) {
				sizet fPos2 = RFindCharacterInStr(value, length, 'f');
				if (fPos != fPos2) {
					/*NOTIFY_ERROR(IError_v1::kDomainGeneral, IError_v1::kGeneralCodeParametersNotAsExpected,
					"Parameter is not a frame count value", IError_v1::kSeverityOperationFatal, true, value);*/
				}
				else {
					frameCount = Parseuint64InStr(value, fPos);
					const char * newValue = value + fPos + 1;
					sizet newLength = length - fPos - 1;
					sizet sPos = FindCharacterInStr(newValue, newLength, 's');
					if (std::string::npos != sPos) {
						sizet sPos2 = RFindCharacterInStr(newValue, newLength, 's');
						if (sPos != sPos2) {
							/*NOTIFY_ERROR(IError_v1::kDomainGeneral, IError_v1::kGeneralCodeParametersNotAsExpected,
							"Parameter is not a frame count value", IError_v1::kSeverityOperationFatal, true, value);*/
						}
						else {
							numerator = Parseuint64InStr(newValue, sPos);
							denominator = Parseuint64InStr(newValue + sPos + 1, newLength - sPos - 1);
						}
					}
					else { // new string only contains numerator
						numerator = Parseuint64InStr(newValue, newLength);
					}
				}
			}
			else { // it simply contains frameCount
				frameCount = Parseuint64InStr(value, length);
			}
		}

		void ConvertFrameCountToTime(uint64 inFrameCount, double &outTime, uint64 inFrameRate, uint64 inFrameRateBasis) {

			//TODO ERROR NOTIFICATION
			if (!inFrameRate) return;
			outTime = (1.0 * inFrameCount * inFrameRateBasis) / inFrameRate;
		}

		void ConvertTimeToFrameCount(double inTime, uint64 &outFrameCount, bool isStart, uint64 inFrameRate, uint64 inFrameRateBasis) {

			if (!inFrameRateBasis) return;

			//TODO ERROR NOTIFICATION
			double calculatedFrame = (inTime * inFrameRate) / inFrameRateBasis;

			if (isStart) {
				outFrameCount = static_cast<uint64>(floor(calculatedFrame));
			}
			else {
				outFrameCount = static_cast<uint64>(ceil(calculatedFrame));
			}
		}

		void ConvertFrameCountToString(std::string & str, uint64 frameCount, uint64 frameRate, uint64 frameRateBasis, bool isDuration) {

			std::ostringstream os;

			if (isDuration && frameCount == UINT64_MAX) {

				os << "maximum";
				str = os.str();
				return;
			}
			os << frameCount;
			if (frameRateBasis != 1 || frameRate != 1)
			{
				os << "f" << frameRate;
			}
			if (frameRateBasis != 1)
			{
				os << "s" << frameRateBasis;
			}

			str = os.str();

		}

		void ConvertFrameRateToString(std::string & str, uint64 frameRate, uint64 frameRateBasis) {

			std::ostringstream os;

			os << "f" << frameRate;

			if (frameRateBasis != 1)
			{
				os << "s" << frameRateBasis;
			}

			str = os.str();

		}

		void ConvertFrameCountToAnotherFrameRate(uint64 inFrameCount, uint64 &outFrameCount, bool isStart, uint64 inFrameRate, uint64 inFrameRateBasis, uint64 outFrameRate, uint64 outFrameRateBasis) {
			//TODO ERROR NOTIFICATION
			double calcTime;
			ConvertFrameCountToTime(inFrameCount, calcTime, inFrameRate, inFrameRateBasis);
			ConvertTimeToFrameCount(calcTime, outFrameCount, isStart, outFrameRate, outFrameRateBasis);
		}
        
        double ConvertStringToDouble(const std::string & str) {
            
            double number;
            std::stringstream strstream;
            strstream << str;
            strstream >> number;
            return number;
        }
        
	}

}