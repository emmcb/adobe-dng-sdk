#ifndef __XMPTimePart_Impl_h__
#define __XMPTimePart_Impl_h__ 1

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
#include "XMPAssetManagement/Interfaces/IXMPPart.h"

namespace AdobeXMPAM { 
	class XMPTimePart_Impl : public IXMPTimePart {
	public:
		XMPTimePart_Impl::XMPTimePart_Impl();
		XMPTimePart_Impl::XMPTimePart_Impl(TimePartFormat kTimePartFormat, uint64_t start, uint64_t startNumerator, uint64_t startDenominator, uint64_t duration, uint64_t durationNumerator, uint64_t durationDenominator, std::string qualifier);

		virtual XMPPartType GetType(void);
		
		virtual void Set(std::string);
		virtual std::string Get(void);

		virtual void SetDescription(std::string);
		virtual void RemoveDescription(void);
		virtual std::string GetDescription();

		virtual void SetStartTimeInFrameCount(uint64_t frames, uint64_t Numerator, uint64_t Denominator);
		virtual void SetDurationInFrameCount(uint64_t frames, uint64_t Numerator, uint64_t Denominator);

		virtual spIXMPTime GetXMPTime(void);
	private:
		std::string _time;
		uint64_t mStartFrameCount, mStartNumeratorFrameCount, mStartDenominator;
		uint64_t mDurationFrameCount, mDurationNumeratorFrameCount, mDurationDenominator;
		//std::string sQualifier;
		IXMPPart* basePart;
	}; // class XMPTimePart_Impl
};

#endif	// __XMPTimePart_Impl_h__
