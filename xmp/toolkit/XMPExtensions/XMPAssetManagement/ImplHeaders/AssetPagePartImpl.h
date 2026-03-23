#ifndef AssetPagePartImpl_h__
#define AssetPagePartImpl_h__ 1

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
//  SKP Sunil Kishor Pathak
//	ANS Ankita Sharma
//
// mm-dd-yy who Description of changes, most recent first.
//
// 12-07-14 ADC 1.0-a041 Refactored Part Classes and Adding support for IAssetArtboardPart.
// 11-26-14 SKP 1.0-a014 Fixed mac build break.
// 11-26-14 SKP 1.0-a012 Made changed to build AML on Mac cl:157148 
// 11-26-14 SKP 1.0-a011 Implemented SerializeAsRDFSyntax() to get serialized content of part
// 11-24-14 ANS 1.0-a007 Fixing build errors
// 11-23-14 SKP 1.0-a006 Support for IAssetPagePart interface.
//
// =================================================================================================
#endif  // AdobePrivate

#if !(IMPLEMENTATION_HEADERS_CAN_BE_INCLUDED)
	#error "Not adhering to design constraints"
	// this file should only be included from its own cpp file
#else
	#include "XMPAssetManagement/ImplHeaders/AssetPartImpl.h"
#endif

#include "XMPAssetManagement/Interfaces/IAssetPagePart_I.h"

#if XMP_WinBuild
	#pragma warning( push )
	#pragma warning( disable : 4250 )
#endif

namespace AdobeXMPAM_Int {
	using namespace AdobeXMPAM_Int;
	using namespace AdobeXMPCommon;
	using namespace AdobeXMPCommon_Int;
	using AdobeXMPCommon::sizet;
	
	class AssetPagePartImpl
		: public virtual IAssetPagePart_I
		, public virtual AssetPartImpl
	{
	public:
		AssetPagePartImpl( eAssetPartComponent component, sizet pageNumber );
		AssetPagePartImpl( const char * component, sizet length, sizet pageNumber );

		virtual sizet APICALL GetNumber() const __NOTHROW__;
		virtual void APICALL SetDescriptivePageNumber(const char * name, sizet length) __NOTHROW__;
		virtual spcIUTF8String APICALL GetDescriptivePageNumber() const;
		
		virtual eAssetPartType APICALL GetType() const;
		virtual spIAssetPart APICALL Clone() const;

		virtual bool Equals( const spcIAssetPart & other ) const;

	protected:
		virtual ~AssetPagePartImpl() __NOTHROW__;

		virtual spIAssetPart_I CreateAndPopulateSpecificContent() const;
		virtual void AppendSpecificContentToRDFSyntax( const spIUTF8String & str ) const;

	protected:
		sizet					mPageNumber;

	#ifdef FRIEND_CLASS_DECLARATION
			FRIEND_CLASS_DECLARATION();
	#endif
			REQ_FRIEND_CLASS_DECLARATION();
		

	};
	
	inline AssetPagePartImpl::~AssetPagePartImpl() __NOTHROW__ { }
}

#if XMP_WinBuild
	#pragma warning( pop )
#endif

#endif  // AssetPagePartImpl_h__
