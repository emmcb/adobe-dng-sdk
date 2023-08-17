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
// 12-02-14 SKP 1.0-a029 Implemented time serialization and fixed issues in layer implementation.
// 11-26-14 SKP 1.0-a012 Made changed to build AML on Mac cl:157148 
// 11-26-14 SKP 1.0-a011 Implemented SerializeAsRDFSyntax() to get serialized content of part
// 11-21-14 ADC 1.0-a004 Support for IAssetLayerPart interface.
//
// =================================================================================================
#endif  // AdobePrivate


#define IMPLEMENTATION_HEADERS_CAN_BE_INCLUDED 1
	#include "XMPAssetManagement/ImplHeaders/AssetLayerPartImpl.h"
#undef IMPLEMENTATION_HEADERS_CAN_BE_INCLUDED

#include "XMPCommon/Interfaces/IErrorNotifier_I.h"
#include "XMPCommon/Interfaces/IUTF8String_I.h"

#include "XMPCommon/Utilities/AutoSharedLock.h"
#include "XMPCommon/XMPCommonErrorCodes.h"
#include "XMPCommon/Interfaces/IError_I.h"
#include "XMPCommon/Utilities/TSmartPointers_I.h"

namespace AdobeXMPAM_Int {

	spIAssetLayerPart_I IAssetLayerPart_I::CreateStandardLayerPart( eAssetPartComponent component, const char * id, sizet idLength)
	{
		return MakeUncheckedSharedPointer( new AssetLayerPartImpl( component, id, idLength),__FILE__, __LINE__, true );
	}

	spIAssetLayerPart_I IAssetLayerPart_I::CreateUserDefinedLayerPart( const char * component, sizet componentLength, const char * id, sizet idLength )
	{
		return MakeUncheckedSharedPointer( new AssetLayerPartImpl( component, componentLength, id, idLength),__FILE__,  __LINE__, true );
	}

	//definition of class methods

	spcIUTF8String APICALL AssetLayerPartImpl::GetID() const {
		AutoSharedLock lock( mSharedMutex );
		return mID;
	}

	void APICALL AssetLayerPartImpl::SetName(const char * name, sizet length) {
		SetDescription( name, length );
	}

	spcIUTF8String APICALL AssetLayerPartImpl::GetName() const {
		return GetDescription();
	}

	IAssetPart_v1::eAssetPartType APICALL AssetLayerPartImpl::GetType() const {
		return IAssetPart_v1::kAssetPartTypeLayer;
	}

	spIAssetPart_I AssetLayerPartImpl::CreateAndPopulateSpecificContent() const {
		spIAssetLayerPart_I sp;
		if ( mAssetPartComponent == kAssetPartComponentUserDefined ) {
			sp = IAssetLayerPart_I::CreateUserDefinedLayerPart( mUserDefinedComponent->c_str(), mUserDefinedComponent->size(), mID->c_str(), mID->size());
		} else {
			sp = IAssetLayerPart_I::CreateStandardLayerPart( mAssetPartComponent, mID->c_str(), mID->size());
		}
		return sp;
	}

	AssetLayerPartImpl::AssetLayerPartImpl( eAssetPartComponent component, const char * id, sizet idLength)
		: AssetPartImpl( component)
	{
		if ( id == NULL || *id == '\0' ) {
			NOTIFY_ERROR( IError_v1::kEDGeneral, kGECParametersNotAsExpected,
				"Id is a must for creating layer part", IError_v1::kESOperationFatal,
				true, ( void * ) id, id, id
				);
		}

		mID = IUTF8String_I::CreateUTF8String( id, idLength );
	}

	AssetLayerPartImpl::AssetLayerPartImpl( const char * component, sizet componentLength, const char * id, sizet idLength )
		: AssetPartImpl( component, componentLength )
	{
		if ( id == NULL || *id == '\0' ) {
			NOTIFY_ERROR( IError_v1::kEDGeneral, kGECParametersNotAsExpected,
				"Id is a must for creating layer part", IError_v1::kESOperationFatal,
				true, ( void * ) id, id, id
			);
		}

		mID = IUTF8String_I::CreateUTF8String( id, idLength );
	}

	void AssetLayerPartImpl::AppendSpecificContentToRDFSyntax( const spIUTF8String & str ) const {
		str->append(":", 1)->append( mID );
	}

	bool AssetLayerPartImpl::Equals( const spcIAssetPart & other ) const {
		bool equal = false;
		if ( other ) {
			const AssetLayerPartImpl * otherImpl = dynamic_cast< const AssetLayerPartImpl * >( other.get() );
			if ( otherImpl ) {
				if ( otherImpl == this )
					return true;

				AutoSharedLock lock( mSharedMutex );
				AutoSharedLock otherLock( otherImpl->mSharedMutex );
				equal = AssetPartImpl::Equals( otherImpl );
				if ( !equal ) return equal;
				equal = strcmp( mID->c_str(), otherImpl->mID->c_str() ) == 0;
				return equal;
			}
		}
		return equal;
	}

	spIAssetPart APICALL AssetLayerPartImpl::Clone() const {
	
	#if !GENERATE_XMPEXTENSIONS_CLIENT_LAYER_CODE
		AutoSharedLock lock(mSharedMutex);
		/*if (multiThreadingSupport == kMultiThreadingUseClonedObjectValue)
			multiThreadingSupport = mSharedMutex ? kMulthiThreading : kNoMultiThreading;*/
		spIAssetPart_I sp = CreateAndPopulateSpecificContent();
		sp->SetDescription(mDescription ? mDescription->c_str() : NULL, mDescription ? mDescription->size() : AdobeXMPCommon::npos);
		return sp;
	#else
			return spIAssetLayerPart();
	#endif
	}
}
namespace AdobeXMPAM{
	using namespace AdobeXMPAM_Int;
	spIAssetLayerPart IAssetLayerPart_v1::MakeShared(pIAssetPart_base ptr) {
		if (!ptr) return spIAssetLayerPart();
		pIAssetPart p = IAssetPart::GetInterfaceVersion() > 1 ? ptr->GetInterfacePointer< IAssetPart >() : ptr;
		pIAssetLayerPart pt = dynamic_cast<pIAssetLayerPart>(p);
		return MakeUncheckedSharedPointer(pt, __FILE__, __LINE__, false);
	}
}