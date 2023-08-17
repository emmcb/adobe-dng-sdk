#ifndef AssetArtboardPartImpl_h__
#define AssetArtboardPartImpl_h__ 1

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
//
// mm-dd-yy who Description of changes, most recent first.
//
// 12-07-14 ADC 1.0-a041 Refactored Part Classes and Adding support for IAssetArtboardPart.
//
// =================================================================================================
#endif  // AdobePrivate

#if !(IMPLEMENTATION_HEADERS_CAN_BE_INCLUDED)
	#error "Not adhering to design constraints"
	// this file should only be included from its own cpp file
#else
	#include "XMPAssetManagement/ImplHeaders/AssetPartImpl.h"
#endif

#include "XMPAssetManagement/Interfaces/IAssetArtboardPart_I.h"

#if XMP_WinBuild
	#pragma warning( push )
	#pragma warning( disable : 4250 )
#endif

namespace AdobeXMPAM_Int {
	using namespace AdobeXMPAM_Int;
	using namespace AdobeXMPCommon;
	using namespace AdobeXMPCommon_Int;
	
	class AssetArtboardPartImpl
		: public virtual IAssetArtboardPart_I
		, public virtual AssetPartImpl
	{
	public:
		AssetArtboardPartImpl( eAssetPartComponent component, const char * id, sizet idLength);
		AssetArtboardPartImpl( const char * component, sizet componentLength, const char * id, sizet idLength );

		virtual spcIUTF8String APICALL GetID() const;
		virtual void APICALL SetName(const char * name, sizet length);
		virtual spcIUTF8String APICALL GetName() const;             //refactor return type was _I

		virtual eAssetPartType APICALL GetType() const;
	
		virtual bool Equals( const spcIAssetPart & other ) const;

		virtual spIAssetPart APICALL Clone() const;

	protected:
		virtual ~AssetArtboardPartImpl() __NOTHROW__;

		virtual spIAssetPart_I CreateAndPopulateSpecificContent() const;
		virtual void AppendSpecificContentToRDFSyntax( const spIUTF8String & str ) const;

	protected:
		//spcIUTF8String_I				mID;                       //refactor
		spcIUTF8String					mID;
	
	#ifdef FRIEND_CLASS_DECLARATION
			FRIEND_CLASS_DECLARATION();
	#endif
			REQ_FRIEND_CLASS_DECLARATION();
	

	};
	
	inline AssetArtboardPartImpl::~AssetArtboardPartImpl() __NOTHROW__ { }
}

#if XMP_WinBuild
	#pragma warning( pop )
#endif

#endif  // AssetArtboardPartImpl_h__
