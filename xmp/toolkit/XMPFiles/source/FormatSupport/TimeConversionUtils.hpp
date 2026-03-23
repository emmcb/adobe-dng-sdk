#ifndef TimeConversionUtils_h__
#define TimeConversionUtils_h__ 1

// =================================================================================================
// Copyright Adobe
// Copyright 2014 Adobe
// All Rights Reserved
//
// NOTICE:  Adobe permits you to use, modify, and distribute this file in accordance with the terms
// of the Adobe license agreement accompanying it. If you have received this file from a source other 
// than Adobe, then your use, modification, or distribution of it requires the prior written permission
// of Adobe.
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
// 08-14-14 ADC 5.6-f114 Timecode computation from iXML information for wave file.
//
// =================================================================================================
#endif  // AdobePrivate

#include "public/include/XMP_Environment.h"	// ! XMP_Environment.h must be the first included header.
#include "public/include/XMP_Const.h"
#include <string>
#include <sstream>
#include <iomanip>
#include <cmath>
#include "XMPFiles/source/XMPFiles_Impl.hpp"

namespace TimeConversionUtils {
	bool ConvertSamplesToSMPTETimecode(
		std::string &		outTimecode,
		XMP_Int64			inSamples,
		XMP_Uns64			inSampleRate,
		const std::string & inTimecodeFormat );

	bool ConvertSMPTETimecodeToSamples(
		XMP_Int64 &			outSamples,
		const std::string & inTimecode,
		XMP_Uns64			inSampleRate,
		const std::string & inTimecodeFormat
		);
   
};

#endif // TimeConversionUtils_h__
