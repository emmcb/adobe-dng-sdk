#ifndef AssetTimePartImpl_h__
#define AssetTimePartImpl_h__ 1

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
// 12-07-14 ADC 1.0-a041 Refactored Part Classes and Adding support for IAssetArtboardPart.
// 11-26-14 SKP 1.0-a011 Implemented SerializeAsRDFSyntax() to get serialized content of part
// 11-21-14 ADC 1.0-a002 Support for IAssetPart and IAssetTimePart interfaces.
//
// =================================================================================================
#endif  // AdobePrivate

#if !(IMPLEMENTATION_HEADERS_CAN_BE_INCLUDED)
	#error "Not adhering to design constraints"
	// this file should only be included from its own cpp file
#else
	#include "XMPAssetManagement/ImplHeaders/AssetPartImpl.h"
#endif

#include "XMPAssetManagement/Interfaces/IAssetTimePart_I.h"
#include <utility>

#if XMP_WinBuild
	#pragma warning( push )
	#pragma warning( disable : 4250 )
#endif

namespace AdobeXMPAM_Int {
	using namespace AdobeXMPAM_Int;
	using namespace AdobeXMPCommon;
	using namespace AdobeXMPCommon_Int;
	
	class AssetTimePartImpl
		: public virtual IAssetTimePart_I
		, public virtual AssetPartImpl
	{
	public:
		AssetTimePartImpl( eAssetPartComponent component);
		AssetTimePartImpl( const char * component, sizet componentLength );

		virtual void APICALL SetFrameCountDuration( uint64 startFrameCount = 0, uint64 countOfFrames = Max_XMP_Uns64 ) __NOTHROW__;
		virtual void APICALL GetFrameCountDuration(uint64 & startFrameCount, uint64 & countOfFrames) const __NOTHROW__;
		virtual void APICALL GetFrameRateForStartFrameCount(uint64 & numerator, uint64 & denominator) const __NOTHROW__;
		virtual void APICALL GetFrameRateForCountOfFrames(uint64 & numerator, uint64 & denominator) const __NOTHROW__;
		virtual void APICALL SetFrameRateForStartFrameCount(uint64 numerator, uint64 denominator = 1);
		virtual void APICALL SetFrameRateForCountOfFrames(uint64 numerator, uint64 denominator = 1);
		virtual void APICALL SetBothFrameRates(uint64 numerator, uint64 denominator = 1);
		virtual eAssetPartType APICALL GetType() const;
		virtual bool Equals( const spcIAssetPart & other ) const;

		virtual spIAssetPart APICALL Clone() const;

	protected:
		AssetTimePartImpl();
		virtual ~AssetTimePartImpl() __NOTHROW__;

		virtual void AppendSpecificContentToRDFSyntax( const spIUTF8String & str ) const;
		virtual spIAssetPart_I CreateAndPopulateSpecificContent() const;

	protected:
		std::pair<uint64, uint64>				mFrameDuration;
		std::pair<uint64, uint64>				mFrameRateForStartFrameCount;
		std::pair<uint64, uint64>				mFrameRateForCountOfFrames;

	#ifdef FRIEND_CLASS_DECLARATION
			FRIEND_CLASS_DECLARATION();
	#endif
			REQ_FRIEND_CLASS_DECLARATION();
		

	};
	
	inline AssetTimePartImpl::~AssetTimePartImpl() __NOTHROW__ { }
}

#if XMP_WinBuild
	#pragma warning( pop )
#endif

#endif  // AssetTimePartImpl_h__
