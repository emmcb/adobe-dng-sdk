#ifndef AssetPartImpl_h__
#define AssetPartImpl_h__ 1

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
// 11-19-14 ADC 1.0-a001 Framework in place for Asset Management Library.
//
// =================================================================================================
#endif  // AdobePrivate

#if !(IMPLEMENTATION_HEADERS_CAN_BE_INCLUDED)
	#error "Not adhering to design constraints"
	// this file should only be included from its own cpp file
#endif

#include "XMPAssetManagement/Interfaces/IAssetPart_I.h"

#include "XMPCommon/BaseClasses/MemoryManagedObject.h"
#include "XMPCommon/ImplHeaders/SharedObjectImpl.h"
#include "XMPCommon/ImplHeaders/ThreadSafeImpl.h"

#if XMP_WinBuild
	#pragma warning( push )
	#pragma warning( disable : 4250 )
#endif

namespace AdobeXMPAM_Int {
	using namespace AdobeXMPAM_Int;
	using namespace AdobeXMPCommon;
	using namespace AdobeXMPCommon_Int;

	class AssetPartImpl
		: public virtual IAssetPart_I
		, public virtual SharedObjectImpl
		, public virtual MemoryManagedObject
		, public virtual ThreadSafeImpl
	{
	public:
		AssetPartImpl( eAssetPartComponent component);
		AssetPartImpl( const char * component, sizet length);

		virtual ~AssetPartImpl() __NOTHROW__;
		virtual eAssetPartType APICALL GetType() const;
		virtual eAssetPartComponent APICALL GetComponent() const;
		virtual spcIUTF8String APICALL GetUserDefinedComponent() const;
		virtual spIAssetPart APICALL Clone() const;
		virtual void SetDescription( const char * description, sizet descriptionLength );
		virtual spcIUTF8String GetDescription() const;
		spIUTF8String SerializeAsRDFSyntax() const;
		virtual bool Equals ( const spcIAssetPart & other ) const;

	protected:
		virtual spIAssetPart_I CreateAndPopulateSpecificContent() const;
		virtual void AppendSpecificContentToRDFSyntax( const spIUTF8String & str ) const;
		bool Equals( const AssetPartImpl * other ) const;

	protected:
		eAssetPartComponent			mAssetPartComponent;
		spIUTF8String				mUserDefinedComponent;
		spIUTF8String				mDescription;
	#ifdef FRIEND_CLASS_DECLARATION
			FRIEND_CLASS_DECLARATION();
	#endif
			REQ_FRIEND_CLASS_DECLARATION();
	};

	inline AssetPartImpl::~AssetPartImpl() __NOTHROW__ { }
}

#if XMP_WinBuild
	#pragma warning( pop )
#endif

#endif  // AssetPartImpl_h__
