// =================================================================================================
// ADOBE SYSTEMS INCORPORATED
// Copyright 2014 Adobe Systems Incorporated
// All Rights Reserved
//
// NOTICE:  Adobe permits you to use, modify, and distribute this file in accordance with the terms
// of the Adobe license agreement accompanying it.
// =================================================================================================

#define IMPLEMENTATION_HEADERS_CAN_BE_INCLUDED 1
	#include "XMPAssetManagement/ImplHeaders/AssetPagePartImpl.h"
#undef IMPLEMENTATION_HEADERS_CAN_BE_INCLUDED

#include "XMPCommon/Interfaces/IErrorNotifier_I.h"
#include "XMPCommon/Interfaces/IUTF8String_I.h"

#include "XMPCommon/Utilities/AutoSharedLock.h"
#include "XMPCommon/Utilities/UTF8String.h"

#include "XMPAssetManagement/XMPAssetManagementErrorCodes.h"
#include "XMPCommon/XMPCommonErrorCodes.h"
#include "XMPCommon/Interfaces/IError_I.h"
#include "XMPCommon/Utilities/TSmartPointers_I.h"


namespace AdobeXMPAM_Int {

	spIAssetPagePart_I IAssetPagePart_I::CreateStandardPagePart( eAssetPartComponent component, sizet pageNumber )
	{
		return MakeUncheckedSharedPointer( new AssetPagePartImpl( component, pageNumber),__FILE__,__LINE__,true );
	}

	spIAssetPagePart_I IAssetPagePart_I::CreateUserDefinedPagePart( const char * component, sizet length, sizet pageNumber)
	{
		return MakeUncheckedSharedPointer( new AssetPagePartImpl( component, length, pageNumber ),__FILE__,__LINE__,true );
	}

	spIAssetPagePart_I IAssetPagePart_I::CreatePartFromRDFSyntax( const char * component, sizet componentLength, const char * value, sizet valueLength ) {
		// check if value is totally positive digit
		UTF8String utf8Value( value );
		if ( utf8Value.find_first_not_of("0123456789") == std::string::npos ) {
			UTF8StringStream ss( value );
			sizet pageNumber( 0 );
			ss>>pageNumber;
			return IAssetPagePart_I::CreateUserDefinedPagePart( component, componentLength, pageNumber );
		} else {
			NOTIFY_ERROR( IError_v1::kEDAssetManagement, IErrorAssetManagement::kPagePartValueNotWellFormed,
				"Value of page part is not supported", IError_v1::kESOperationFatal, true, value );
		}
		return spIAssetPagePart_I();
	}


	//definition of class methods

	sizet APICALL AssetPagePartImpl::GetNumber() const __NOTHROW__{
		AutoSharedLock lock( mSharedMutex );
		return mPageNumber;
	}

	void APICALL AssetPagePartImpl::SetDescriptivePageNumber(const char * name, sizet length) __NOTHROW__{
		SetDescription( name, length );
	}

	spcIUTF8String APICALL AssetPagePartImpl::GetDescriptivePageNumber() const {
		return GetDescription();
	}

	IAssetPart_v1::eAssetPartType APICALL AssetPagePartImpl::GetType() const {
		return IAssetPart_v1::kAssetPartTypePage;
	}

	spIAssetPart_I AssetPagePartImpl::CreateAndPopulateSpecificContent( ) const  {
		spIAssetPagePart_I sp;
		if ( mAssetPartComponent == kAssetPartComponentUserDefined ) {
			sp = IAssetPagePart_I::CreateUserDefinedPagePart( mUserDefinedComponent->c_str(),mUserDefinedComponent->size(), mPageNumber);
		} else {
			sp = IAssetPagePart_I::CreateStandardPagePart( mAssetPartComponent, mPageNumber );
		}
		return sp;
	}

	AssetPagePartImpl::AssetPagePartImpl(eAssetPartComponent component, sizet pageNumber)
		: AssetPartImpl(component)
	{
		if (pageNumber == 0)
			NOTIFY_ERROR(IError_v1::kEDAssetManagement, IErrorAssetManagement::kPagePartValueNotWellFormed,
			"Value of page part is not supported", IError_v1::kESOperationFatal, true, pageNumber);
		
		mPageNumber = pageNumber;
	}
		
	AssetPagePartImpl::AssetPagePartImpl( const char * component, sizet length, sizet pageNumber )
		: AssetPartImpl( component, length )
	{ 
		if (pageNumber == 0)
			NOTIFY_ERROR(IError_v1::kEDAssetManagement, IErrorAssetManagement::kPagePartValueNotWellFormed,
			"Value of page part is not supported", IError_v1::kESOperationFatal, true, pageNumber);
		mPageNumber = pageNumber;
	}

	void AssetPagePartImpl::AppendSpecificContentToRDFSyntax( const spIUTF8String & str ) const {
		UTF8StringStream ss;
		ss << mPageNumber;
		str->append(":", 1)->append( ss.str().c_str(),ss.str().size() );
	}

	bool AssetPagePartImpl::Equals( const spcIAssetPart & other ) const {
		bool equal = false;
		if ( other ) {
			const AssetPagePartImpl * otherImpl = dynamic_cast< const AssetPagePartImpl * >( other.get() );
			if ( otherImpl ) {
				if ( otherImpl == this )
					return true;

				AutoSharedLock lock( mSharedMutex );
				AutoSharedLock otherLock( otherImpl->mSharedMutex );
				equal = AssetPartImpl::Equals( otherImpl );
				if ( !equal ) return equal;
				equal = mPageNumber == otherImpl->mPageNumber;
				return equal;
			}
		}
		return equal;

	}

	spIAssetPart APICALL AssetPagePartImpl::Clone() const {

#if !GENERATE_XMPEXTENSIONS_CLIENT_LAYER_CODE

		AutoSharedLock lock(mSharedMutex);
		/*if (multiThreadingSupport == kMultiThreadingUseClonedObjectValue)
			multiThreadingSupport = mSharedMutex ? kMulthiThreading : kNoMultiThreading;*/
		spIAssetPart_I sp = CreateAndPopulateSpecificContent();
		sp->SetDescription(mDescription ? mDescription->c_str() : NULL, mDescription ? mDescription->size() : AdobeXMPCommon::npos);
		return sp;

#else
		return spIAssetPagePart();
#endif
	}
}
namespace AdobeXMPAM{
	using namespace AdobeXMPAM_Int;
	spIAssetPagePart IAssetPagePart_v1::MakeShared(pIAssetPart_base ptr) {
		if (!ptr) return spIAssetPagePart();
		pIAssetPart p = IAssetPart::GetInterfaceVersion() > 1 ? ptr->GetInterfacePointer< IAssetPart >() : ptr;
		pIAssetPagePart pt = dynamic_cast<pIAssetPagePart>(p);
		return MakeUncheckedSharedPointer(pt, __FILE__, __LINE__, false);
	}
}

