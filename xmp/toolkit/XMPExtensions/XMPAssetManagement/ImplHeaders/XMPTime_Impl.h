#ifndef __XMPTime_Impl_h__
#define __XMPTime_Impl_h__ 1

// =================================================================================================
// ADOBE SYSTEMS INCORPORATED
// Copyright 2014 Adobe Systems Incorporated
// All Rights Reserved
//
// NOTICE: Adobe permits you to use, modify, and distribute this file in accordance with the terms
// of the Adobe license agreement accompanying it.
// =================================================================================================

#if AdobePrivate
// =================================================================================================
// Change history
// ==============
//
// Writers:
//  SKP Sunil Kishor Pathak
//
// mm-dd-yy who Description of changes, most recent on top
//
// 09-18-14 SKP 1.0 First draft for Asset Relationships API.
//
// =================================================================================================
#endif // AdobePrivate

#include <cstdio>
#include <vector>
#include <string>
#include <cstring>
#include <algorithm>

// Ensure XMP templates are instantiated
#define TXMP_STRING_TYPE std::string 

// Provide access to the API
#include "public/include/XMP.hpp"
#include "XMPAssetManagement/XMPAssetManagementFwdDeclarations.h"
#include "XMPAssetManagement/Interfaces/IXMPTime.h"

namespace AdobeXMPAM { 
		/*
		**	Time is in second but represented as frameCount.
		**		FrameCount : No of frames at a given "FrameRate" default is 1
		**		FrameRate : No of frames per "number of seconds	" (also called rate basis)
		**
		**	Framecount can be in format ##f##s## (first ## is number of frames : if there is nothing more it is equal to second
		**								if second ## is present but third ## is not there, then it is frame-rate with default rate basis == 1
		**								if third ## is present that is second.
		**
		**				calculating time : fr = (get frame rate : second ## / third ##)
		**									no of second  S = (first ## / fr)
 		*/
	class XMPTime_Impl : public IXMPTime {
	public:
		//XMPTime_Impl::XMPTime_Impl();
		XMPTime_Impl::XMPTime_Impl(uint64_t start=0, uint64_t startNumerator=1, uint64_t startDenominator=1, uint64_t duration=0, uint64_t durationNumerator=1, uint64_t durationDenominator=1);

		virtual std::string GetStartTime() const;
		virtual std::string GetDuration() const;
		virtual std::string GetTime() const;

	private:
		uint64_t mStartFrameCount, mStartNumeratorFrameCount, mStartDenominator;
		uint64_t mDurationFrameCount, mDurationNumeratorFrameCount, mDurationDenominator;
	}; // class XMPTime_Impl
}

#endif	// __XMPTime_Impl_h__
