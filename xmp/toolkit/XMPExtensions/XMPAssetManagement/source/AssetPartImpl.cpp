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
// 03-05-15 SKP 1.0-a078 Bug: 3924437 Crash in application on calling clone method of AssetPart.
// 12-07-14 ADC 1.0-a041 Refactored Part Classes and Adding support for IAssetArtboardPart.
// 12-05-14 SKP 1.0-a039 Fixed issue in SetDescription()(). Fixed argment in CreateFromCStr() call.
// 12-02-14 SKP 1.0-a028 1. Moved standardComponentStrings in IAssetPart_I.cpp so that it can be
//							used in other cpp files, 2. fixed issues in SerializeAsRDFSyntax(),
//							3. Fixed issue in GetAssetPartComponentIndex() and 4. fixed issues
//							while composing from composed component.
// 11-26-14 SKP 1.0-a012 Made changed to build AML on Mac cl:157148 
// 11-26-14 SKP 1.0-a011 Implemented SerializeAsRDFSyntax() to get serialized content of part
// 11-21-14 ADC 1.0-a004 Support for IAssetLayerPart interface.
// 11-21-14 ADC 1.0-a002 Support for IAssetPart and IAssetTimePart interfaces.
// 11-19-14 ADC 1.0-a001 Framework in place for Asset Management Library.
//
// =================================================================================================
#endif  // AdobePrivate


#define IMPLEMENTATION_HEADERS_CAN_BE_INCLUDED 1
	#include "XMPAssetManagement/ImplHeaders/AssetPartImpl.h"
#undef IMPLEMENTATION_HEADERS_CAN_BE_INCLUDED

#include "XMPCommon/Interfaces/IError_I.h"
#include "XMPCommon/XMPCommonErrorCodes.h"
#include "XMPCommon/Interfaces/IErrorNotifier_I.h"
#include "XMPCommon/Utilities/AutoSharedLock.h"
#include "XMPCommon/Interfaces/IUTF8String_I.h"
#include "XMPCommon/Utilities/UTF8String.h"
#include "XMPCommon/Utilities/TSmartPointers_I.h"

#include <algorithm>

namespace AdobeXMPAM_Int {
	IAssetPart_v1::eAssetPartType APICALL AssetPartImpl::GetType() const {
		return kAssetPartTypeGeneric;
	}

	IAssetPart_v1::eAssetPartComponent APICALL AssetPartImpl::GetComponent() const {
		AutoSharedLock lock ( mSharedMutex );
		return mAssetPartComponent;
	}

	spcIUTF8String APICALL AssetPartImpl::GetUserDefinedComponent() const {
		AutoSharedLock lock( mSharedMutex );
		return mUserDefinedComponent;
	}

	spIAssetPart APICALL AssetPartImpl::Clone() const {
	#if !GENERATE_XMPEXTENSIONS_CLIENT_LAYER_CODE
		AutoSharedLock lock( mSharedMutex );
		/*if ( multiThreadingSupport == kMultiThreadingUseClonedObjectValue )
			multiThreadingSupport = mSharedMutex ? kMulthiThreading : kNoMultiThreading;*/
		spIAssetPart_I sp = CreateAndPopulateSpecificContent();         //arg=multithreadingsupport
		//sp->SetDescription(mDescription ? mDescription->c_str() : NULL);
		sp->SetDescription(mDescription ? mDescription->c_str() : NULL, mDescription ? mDescription->size(): AdobeXMPCommon::npos);
		return sp;

	#else
		return spIAssetPart();
	#endif
	}

	void AssetPartImpl::SetDescription( const char * description, sizet descriptionLength ) {
		AutoSharedLock lock( mSharedMutex, true );
		if ( description ) {
			if ( mDescription )
				mDescription->assign( description, descriptionLength );
			else
				mDescription = IUTF8String_I::CreateUTF8String( description, descriptionLength);
		} else {
			mDescription.reset();
		}
	}

	spcIUTF8String AssetPartImpl::GetDescription() const {
		AutoSharedLock lock( mSharedMutex );
		return mDescription;
	}

	AssetPartImpl::AssetPartImpl( eAssetPartComponent component)
		: mAssetPartComponent( component )
	{
		static auto maxLimit = sizeof( standardComponentStrings ) / sizeof( const char * );
		if ( component < 0 || component >= maxLimit ) {
			NOTIFY_ERROR( IError_v1::kEDGeneral, kGECParametersNotAsExpected,
				"component Value specified is not supported", IError_v1::kESOperationFatal,
				true, static_cast< uint32 >( component ) );
		}
	}

	AssetPartImpl::AssetPartImpl( const char * component, sizet length)
		:mAssetPartComponent( kAssetPartComponentUserDefined )
	{
		if ( component == NULL ) {
			NOTIFY_ERROR(IError_v1::kEDGeneral, kGECParametersNotAsExpected,
				"component can't be null", IError_v1::kESOperationFatal,
				true, ( void * ) component
				);
		}
		static sizet maxLimit = sizeof( standardComponentStrings ) / sizeof( const char * );

		UTF8String utf8String( component );
		std::transform( utf8String.begin(), utf8String.end(), utf8String.begin(), ::tolower );

		for ( size_t i = 0; i < maxLimit; i++ ) {
			if ( utf8String.compare( standardComponentStrings[ i ] ) == 0 ) {
				mAssetPartComponent = static_cast< eAssetPartComponent >( i );
				break;
			}
		}

		if ( mAssetPartComponent == kAssetPartComponentUserDefined ) {
			mUserDefinedComponent = IUTF8String_I::CreateUTF8String( component, length );
		}
	}

	spIUTF8String AssetPartImpl::SerializeAsRDFSyntax() const {
		spIUTF8String rdfSerializedString;
		AutoSharedLock lock ( mSharedMutex );

		if ( mAssetPartComponent == kAssetPartComponentUserDefined )
			rdfSerializedString = IUTF8String_I::CreateUTF8String( mUserDefinedComponent->c_str(), mUserDefinedComponent->size());
		else {
			rdfSerializedString = IUTF8String_I::CreateUTF8String( standardComponentStrings[mAssetPartComponent], std::string::npos );
		}

		const char * partType = partTypeStrings[ GetType() ];
		sizet partTypeLen = strlen( partType );
		sizet serializedStrLen = rdfSerializedString->size();

		if ( serializedStrLen > 0 && 
			 partTypeLen > 0 && 
			 rdfSerializedString->c_str()[ serializedStrLen - 1 ] != '/' )
		{
			rdfSerializedString->append( "/",1 );
		}

		if ( partTypeLen > 0 ) {
			rdfSerializedString->append( partType,partTypeLen);
			AppendSpecificContentToRDFSyntax( rdfSerializedString );
		}
		return rdfSerializedString;
	}

	spIAssetPart_I AssetPartImpl::CreateAndPopulateSpecificContent() const {
		spIAssetPart_I sp;
		if ( mAssetPartComponent == kAssetPartComponentUserDefined ) {
			sp = IAssetPart_I::CreateUserDefinedGenericPart( mUserDefinedComponent->c_str(), mUserDefinedComponent->size());
		} else {
			sp = IAssetPart_I::CreateStandardGenericPart( mAssetPartComponent);
		}
		return sp;
	}

	void AssetPartImpl::AppendSpecificContentToRDFSyntax( const spIUTF8String & str ) const { }

	spIAssetPart_I IAssetPart_I::CreateStandardGenericPart( eAssetPartComponent component ) {
		return MakeUncheckedSharedPointer(new AssetPartImpl(component), __FILE__, __LINE__);
	}

	spIAssetPart_I IAssetPart_I::CreateUserDefinedGenericPart( const char * component, sizet length) {
		return MakeUncheckedSharedPointer(new AssetPartImpl(component, length), __FILE__, __LINE__);
	}

	bool AssetPartImpl::Equals( const AssetPartImpl * other ) const {
		bool equal = false;
		equal = mAssetPartComponent == other->mAssetPartComponent;
		if ( !equal ) return equal;
		equal = ( mUserDefinedComponent ? true : false ) == ( other->mUserDefinedComponent ? true : false );
		if ( !equal ) return equal;
		if ( mUserDefinedComponent )
			equal = strcmp( mUserDefinedComponent->c_str(), other->mUserDefinedComponent->c_str() ) == 0;
		if ( !equal ) return equal;
		equal = ( mDescription ? true : false ) == ( other->mDescription ? true : false );
		if ( !equal ) return equal;
		if ( mDescription )
			equal = strcmp( mDescription->c_str(), other->mDescription->c_str() ) == 0;
		return equal;
	}

	bool AssetPartImpl::Equals ( const spcIAssetPart & other ) const {
		bool equal = false;
		if ( other ) {
			const AssetPartImpl * otherImpl = dynamic_cast< const AssetPartImpl * >( other.get() );
			if ( otherImpl ) {
				if ( otherImpl == this )
					return true;

				AutoSharedLock lock( mSharedMutex );
				AutoSharedLock otherLock( otherImpl->mSharedMutex );
				return Equals( otherImpl );
			}
		}
		return equal;
	}
}

namespace AdobeXMPAM {
	using namespace AdobeXMPAM_Int;
	spIAssetPart IAssetPart_v1::MakeShared(pIAssetPart_base ptr) {
		if (!ptr) return spIAssetPart();
		pIAssetPart p = IAssetPart::GetInterfaceVersion() > 1 ? ptr->GetInterfacePointer< IAssetPart >() : ptr;
		return MakeUncheckedSharedPointer(p, __FILE__, __LINE__, false);
	}
}
