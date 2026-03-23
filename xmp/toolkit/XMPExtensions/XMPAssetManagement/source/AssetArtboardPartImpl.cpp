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


#define IMPLEMENTATION_HEADERS_CAN_BE_INCLUDED 1
	#include "XMPAssetManagement/ImplHeaders/AssetArtboardPartImpl.h"
#undef IMPLEMENTATION_HEADERS_CAN_BE_INCLUDED

#include "XMPCommon/Interfaces/IErrorNotifier_I.h"
#include "XMPCommon/Utilities/AutoSharedLock.h"
#include "XMPCommon/Interfaces/IUTF8String_I.h"
#include "XMPCommon/XMPCommonErrorCodes.h"
#include "XMPCommon/Interfaces/IError_I.h"
#include "XMPCommon/Utilities/TSmartPointers_I.h"

namespace AdobeXMPAM_Int {

	//definition of static factory methods of internal interface
	spIAssetArtboardPart_I IAssetArtboardPart_I::CreateStandardArtboardPart( eAssetPartComponent component, const char * id, sizet idLength)
	{
		return MakeUncheckedSharedPointer(new AssetArtboardPartImpl(component, id, idLength), __FILE__, __LINE__, true);
	}

	spIAssetArtboardPart_I IAssetArtboardPart_I::CreateUserDefinedArtboardPart( const char * component, sizet componentLength, const char * id, sizet idLength)
	{
		return MakeUncheckedSharedPointer(new AssetArtboardPartImpl(component, componentLength, id, idLength), __FILE__, __LINE__, true);
	}

	//definition of class methods
	spcIUTF8String APICALL AssetArtboardPartImpl::GetID() const {
		AutoSharedLock lock( mSharedMutex );
		return mID;
	}

	void APICALL AssetArtboardPartImpl::SetName(const char * name, sizet length) {
		SetDescription( name, length );
	}

	spcIUTF8String APICALL AssetArtboardPartImpl::GetName() const {
		return GetDescription();
	}

	IAssetPart_v1::eAssetPartType APICALL AssetArtboardPartImpl::GetType() const {
		return IAssetPart_v1::kAssetPartTypeArtboard;
	}

	spIAssetPart_I AssetArtboardPartImpl::CreateAndPopulateSpecificContent( ) const {
		spIAssetArtboardPart_I sp;
		if ( mAssetPartComponent == kAssetPartComponentUserDefined ) {
			sp = IAssetArtboardPart_I::CreateUserDefinedArtboardPart( mUserDefinedComponent->c_str(), mUserDefinedComponent->size(), mID->c_str(), mID->size());
		} else {
			sp = IAssetArtboardPart_I::CreateStandardArtboardPart( mAssetPartComponent, mID->c_str(), mID->size());
		}
		return sp;
	}

	AssetArtboardPartImpl::AssetArtboardPartImpl( eAssetPartComponent component, const char * id, sizet idLength )
		: AssetPartImpl( component )
	{
		if ( id == NULL || *id == '\0' ) {
			NOTIFY_ERROR( IError_v1::kEDGeneral, kGECParametersNotAsExpected,
				"Id is a must for creating artboard part", IError_v1::kESOperationFatal,
				true, ( void * ) id, id, id
				);
		}

		mID = IUTF8String_I::CreateUTF8String( id, idLength );
	}

	AssetArtboardPartImpl::AssetArtboardPartImpl( const char * component, sizet componentLength, const char * id, sizet idLength  )
		: AssetPartImpl( component, componentLength)
	{
		if ( id == NULL || *id == '\0' ) {
			NOTIFY_ERROR( IError_v1::kEDGeneral, kGECParametersNotAsExpected,
				"Id is a must for creating artboard part", IError_v1::kESOperationFatal,
				true, ( void * ) id, id, id
			);
		}

		mID = IUTF8String_I::CreateUTF8String( id, idLength );
	}

	void AssetArtboardPartImpl::AppendSpecificContentToRDFSyntax( const spIUTF8String & str ) const {
		str->append(":", 1)->append(mID);
	}

	bool AssetArtboardPartImpl::Equals( const spcIAssetPart & other ) const {
		bool equal = false;
		if ( other ) {
			const AssetArtboardPartImpl * otherImpl = dynamic_cast< const AssetArtboardPartImpl * >( other.get() );
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

	spIAssetPart APICALL AssetArtboardPartImpl::Clone() const {
		
	#if !GENERATE_XMPEXTENSIONS_CLIENT_LAYER_CODE
		AutoSharedLock lock(mSharedMutex);
		/*if (multiThreadingSupport == kMultiThreadingUseClonedObjectValue)
			multiThreadingSupport = mSharedMutex ? kMulthiThreading : kNoMultiThreading;*/
		spIAssetPart_I sp = CreateAndPopulateSpecificContent();
		sp->SetDescription(mDescription ? mDescription->c_str() : NULL, mDescription ? mDescription->size() : AdobeXMPCommon::npos);
		return sp;
	#else
			return spIAssetArtboardPart();
	#endif
	}
}
namespace AdobeXMPAM{
	using namespace AdobeXMPAM_Int;
	spIAssetArtboardPart IAssetArtboardPart_v1::MakeShared(pIAssetPart_base ptr) {
		if (!ptr) return spIAssetArtboardPart();
		pIAssetPart p = IAssetPart::GetInterfaceVersion() > 1 ? ptr->GetInterfacePointer< IAssetPart >() : ptr;
		pIAssetArtboardPart pt = dynamic_cast<pIAssetArtboardPart>(p);
		return MakeUncheckedSharedPointer(pt, __FILE__, __LINE__, false);
	}
}