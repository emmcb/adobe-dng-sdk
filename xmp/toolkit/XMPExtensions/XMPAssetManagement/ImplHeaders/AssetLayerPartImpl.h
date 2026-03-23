#ifndef AssetLayerPartImpl_h__
#define AssetLayerPartImpl_h__ 1

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
// 11-21-14 ADC 1.0-a004 Support for IAssetLayerPart interface.
//
// =================================================================================================
#endif  // AdobePrivate

#if !(IMPLEMENTATION_HEADERS_CAN_BE_INCLUDED)
	#error "Not adhering to design constraints"
	// this file should only be included from its own cpp file
#else
	#include "XMPAssetManagement/ImplHeaders/AssetPartImpl.h"
#endif

#include "XMPAssetManagement/Interfaces/IAssetLayerPart_I.h"

#if XMP_WinBuild
	#pragma warning( push )
	#pragma warning( disable : 4250 )
#endif

namespace AdobeXMPAM_Int {
	using namespace AdobeXMPAM_Int;
	using namespace AdobeXMPCommon;
	using namespace AdobeXMPCommon_Int;
	
	class AssetLayerPartImpl
		: public virtual IAssetLayerPart_I
		, public virtual AssetPartImpl
	{
	public:
		AssetLayerPartImpl( eAssetPartComponent component, const char * id, sizet idLength );
		AssetLayerPartImpl( const char * component, sizet componentLength, const char * id, sizet idLength );

		virtual spcIUTF8String APICALL GetID() const;
		virtual void APICALL SetName(const char * name, sizet length);
		virtual spcIUTF8String APICALL GetName() const;

		virtual eAssetPartType APICALL GetType() const;
		virtual bool Equals( const spcIAssetPart & other ) const;

		virtual spIAssetPart APICALL Clone() const;

	protected:
		virtual ~AssetLayerPartImpl() __NOTHROW__;

		virtual spIAssetPart_I CreateAndPopulateSpecificContent() const;
		virtual void AppendSpecificContentToRDFSyntax( const spIUTF8String & str ) const;

	protected:
		spcIUTF8String				mID;

	#ifdef FRIEND_CLASS_DECLARATION
			FRIEND_CLASS_DECLARATION();
	#endif
			REQ_FRIEND_CLASS_DECLARATION();
		

	};
	
	inline AssetLayerPartImpl::~AssetLayerPartImpl() __NOTHROW__ { }
}

#if XMP_WinBuild
	#pragma warning( pop )
#endif

#endif  // AssetLayerPartImpl_h__
