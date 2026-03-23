// =================================================================================================
// ADOBE SYSTEMS INCORPORATED
// Copyright 2014 Adobe Systems Incorporated
// All Rights Reserved
//
// NOTICE:  Adobe permits you to use, modify, and distribute this file in accordance with the terms
// of the Adobe license agreement accompanying it.
// =================================================================================================

#define IMPLEMENTATION_HEADERS_CAN_BE_INCLUDED 1
	#include "XMPAssetManagement/ImplHeaders/AssetTimePartImpl.h"
#undef IMPLEMENTATION_HEADERS_CAN_BE_INCLUDED

#include "XMPCommon/Interfaces/IErrorNotifier_I.h"
#include "XMPCommon/Utilities/AutoSharedLock.h"
#include "XMPCommon/Interfaces/IUTF8String_I.h"
#include "XMPCommon/Utilities/UTF8String.h"
#include "XMPCommon/XMPCommonErrorCodes.h"
#include "XMPCommon/Interfaces/IError_I.h"
#include "XMPCommon/Utilities/TSmartPointers_I.h"

#include "XMPAssetManagement/XMPAssetManagementErrorCodes.h"

namespace AdobeXMPAM_Int {

	//definition of static factory methods of internal interface

	spIAssetTimePart_I IAssetTimePart_I::CreateStandardTimePart( eAssetPartComponent component)
	{
		return MakeUncheckedSharedPointer( new AssetTimePartImpl( component ),__FILE__,__LINE__,true );
	}

	spIAssetTimePart_I IAssetTimePart_I::CreateUserDefinedTimePart( const char * component, sizet componentLength )
	{
		return MakeUncheckedSharedPointer( new AssetTimePartImpl( component, componentLength ),__FILE__,__LINE__,true );
	}

	static uint64 ParseUInt64( const char * strValue, sizet length ) {
		if ( length == 0 ) {
			NOTIFY_ERROR( IError_v1::kEDGeneral, kGECParametersNotAsExpected,
				"Parameter is not an unsinged integer", IError_v1::kESOperationFatal, true, strValue );
		}
		UTF8String utf8Value;
		UTF8StringStream ss;

		utf8Value.assign( strValue, length );
		if ( utf8Value.find_first_not_of( "0123456789" ) == std::string::npos ) {
			ss.str( utf8Value );
			uint64 number;
			ss >> number;
			return number;
		} else {
			NOTIFY_ERROR( IError_v1::kEDGeneral, kGECParametersNotAsExpected,
				"Parameter is not an unsinged integer", IError_v1::kESOperationFatal, true, utf8Value.c_str() );
		}
		return Max_XMP_Uns64;
	}

	static sizet FindCharacter( const char * string, sizet length, char a ) {
		for ( sizet i = 0; i < length; i++ ) {
			if ( string[i] == a )
				return i;
		}
		return std::string::npos;
	}

	static sizet RFindCharacter( const char * string, sizet length, char a ) {
		for ( sizet i = length; i > 0; i-- ) {
			if ( string[ i - 1 ] == a )
				return i - 1;
		}
		return std::string::npos;
	}

	static void ParseFrameCount( const char * value, sizet length, uint64 & frameCount, uint64 & numerator, uint64 & denominator ) {
		frameCount = 0; numerator = 1; denominator = 1;
		
		sizet fPos = FindCharacter( value, length, 'f' );

		if ( std::string::npos != fPos ) {
			sizet fPos2 = RFindCharacter( value, length, 'f' );
			if ( fPos != fPos2 ) {
				NOTIFY_ERROR( IError_v1::kEDGeneral, kGECParametersNotAsExpected,
					"Parameter is not a frame count value", IError_v1::kESOperationFatal, true, value );
			} else {
				frameCount = ParseUInt64( value, fPos );
				const char * newValue = value + fPos + 1;
				sizet newLength = length - fPos - 1;
				sizet sPos = FindCharacter( newValue, newLength, 's' );
				if ( std::string::npos != sPos ) {
					sizet sPos2 = RFindCharacter( newValue, newLength, 's' );
					if ( sPos != sPos2 ) {
						NOTIFY_ERROR( IError_v1::kEDGeneral, kGECParametersNotAsExpected,
							"Parameter is not a frame count value", IError_v1::kESOperationFatal, true, value );
					} else {
						numerator = ParseUInt64( newValue, sPos );
						denominator = ParseUInt64( newValue + sPos + 1, newLength - sPos - 1 );
					}
				} else { // new string only contains numerator
					numerator = ParseUInt64( newValue, newLength );
				}
			}
		} else { // it simply contains frameCount
			frameCount = ParseUInt64( value, length );
		}
	}

	spIAssetTimePart_I IAssetTimePart_I::CreatePartFromRDFSyntax( const char * component, sizet componentLength, const char * value, sizet valueLength )
	{
		// parse the string
		//sizet valueLen = strlen( value );
		sizet valueLen = valueLength;
		bool usesDurationFormat = false;
		char seperatorChar = 'd';
		auto sp = IAssetTimePart_I::CreateUserDefinedTimePart( component,componentLength );
		if ( valueLen > 0 ) {
			sizet seperatorPos = FindCharacter( value, valueLen, seperatorChar );
			if ( std::string::npos != seperatorPos ) {
				usesDurationFormat = true;
				sizet seperatorPos2 = RFindCharacter( value, valueLen, seperatorChar );
				if ( seperatorPos != seperatorPos2 ) {
					NOTIFY_ERROR( IError_v1::kEDAssetManagement, IErrorAssetManagement::kTimePartValueNotWellFormed,
						"Parameter is not a valid time part value", IError_v1::kESOperationFatal, true, value );
				}
			} else {
				usesDurationFormat = false;
				seperatorChar = 'r';
				seperatorPos = FindCharacter( value, valueLen, seperatorChar );
				if ( std::string::npos != seperatorPos ) {
					usesDurationFormat = true;
					sizet seperatorPos2 = RFindCharacter( value, valueLen, seperatorChar );
					if ( seperatorPos != seperatorPos2 ) {
						NOTIFY_ERROR( IError_v1::kEDAssetManagement, IErrorAssetManagement::kTimePartValueNotWellFormed,
							"Parameter is not a valid time part value", IError_v1::kESOperationFatal, true, value );
					}
				}
			}

			uint64 startFrameCount = 0, numerator = 1, denominator = Max_XMP_Uns64, durationFrameCount = Max_XMP_Uns64;
			if ( std::string::npos != seperatorPos ) {
				ParseFrameCount( value, seperatorPos, startFrameCount, numerator, denominator );
				sp->SetFrameRateForStartFrameCount( numerator, denominator );
				ParseFrameCount( value + seperatorPos + 1, valueLen - seperatorPos - 1, durationFrameCount, numerator, denominator );
				sp->SetFrameRateForCountOfFrames( numerator, denominator );
				if ( !usesDurationFormat ) {
					if ( durationFrameCount < startFrameCount ) {
						NOTIFY_ERROR( IError_v1::kEDAssetManagement, IErrorAssetManagement::kTimePartValueNotWellFormed,
							"Parameter is not a valid time part value", IError_v1::kESOperationFatal, true, value );
					}
					durationFrameCount -= startFrameCount;
				}
				sp->SetFrameCountDuration( startFrameCount, durationFrameCount );
			} else {
				ParseFrameCount( value, valueLen, startFrameCount, numerator, denominator );
				sp->SetFrameCountDuration( startFrameCount );
				sp->SetFrameRateForStartFrameCount( numerator, denominator );
			}
		}
		return sp;
	}

	//definition of class methods

	void APICALL AssetTimePartImpl::SetFrameCountDuration( uint64 startFrameCount /*= 0*/, uint64 countOfFrames /*= Max_XMP_Uns64 */ ) __NOTHROW__ {
		AutoSharedLock lock( mSharedMutex, true );
		mFrameDuration.first = startFrameCount;
		mFrameDuration.second = countOfFrames;
	}

	void APICALL AssetTimePartImpl::GetFrameCountDuration(uint64 & startFrameCount, uint64 & countOfFrames) const __NOTHROW__{
		AutoSharedLock lock( mSharedMutex );
		startFrameCount = mFrameDuration.first;
		countOfFrames = mFrameDuration.second;
	}

	void APICALL AssetTimePartImpl::GetFrameRateForStartFrameCount(uint64 & numerator, uint64 & denominator) const __NOTHROW__{
		AutoSharedLock lock( mSharedMutex );
		numerator = mFrameRateForStartFrameCount.first;
		denominator = mFrameRateForStartFrameCount.second;
	}

	void APICALL AssetTimePartImpl::GetFrameRateForCountOfFrames(uint64 & numerator, uint64 & denominator) const __NOTHROW__{
		AutoSharedLock lock( mSharedMutex );
		numerator = mFrameRateForCountOfFrames.first;
		denominator = mFrameRateForCountOfFrames.second;
	}

	void APICALL AssetTimePartImpl::SetFrameRateForStartFrameCount(uint64 numerator, uint64 denominator /*= 1 */) {
		if ( numerator == 0 || denominator == 0 ) {
			NOTIFY_ERROR( IError_v1::kEDGeneral, kGECParametersNotAsExpected,
				"numerator, denominator or both can't be zero", IError_v1::kESOperationFatal, 
				true, numerator, true, denominator );
		}
		AutoSharedLock lock( mSharedMutex, true );
		mFrameRateForStartFrameCount.first = numerator;
		mFrameRateForStartFrameCount.second = denominator;
	}

	void APICALL AssetTimePartImpl::SetFrameRateForCountOfFrames(uint64 numerator, uint64 denominator /*= 1 */) {
		if ( numerator == 0 || denominator == 0 ) {
			NOTIFY_ERROR( IError_v1::kEDGeneral, kGECParametersNotAsExpected,
				"numerator, denominator or both can't be zero", IError_v1::kESOperationFatal,
				true, numerator, true, denominator );
		}
		AutoSharedLock lock( mSharedMutex, true );
		mFrameRateForCountOfFrames.first = numerator;
		mFrameRateForCountOfFrames.second = denominator;
	}

	void APICALL AssetTimePartImpl::SetBothFrameRates(uint64 numerator, uint64 denominator /*= 1 */) {
		if ( numerator == 0 || denominator == 0 ) {
			NOTIFY_ERROR( IError_v1::kEDGeneral, kGECParametersNotAsExpected,
				"numerator, denominator or both can't be zero", IError_v1::kESOperationFatal,
				true, numerator, true, denominator );
		}
		AutoSharedLock lock( mSharedMutex, true );
		mFrameRateForCountOfFrames.first = numerator;
		mFrameRateForCountOfFrames.second = denominator;
		mFrameRateForStartFrameCount.first = numerator;
		mFrameRateForStartFrameCount.second = denominator;
	}

	IAssetPart_v1::eAssetPartType APICALL AssetTimePartImpl::GetType() const  {
		return IAssetPart_v1::kAssetPartTypeTime;
	}

	AssetTimePartImpl::AssetTimePartImpl( eAssetPartComponent component ) 
		: AssetPartImpl( component )
		, mFrameDuration( 0, Max_XMP_Uns64 )
		, mFrameRateForStartFrameCount( 1, 1 )
		, mFrameRateForCountOfFrames( 1, 1 ) { }

	AssetTimePartImpl::AssetTimePartImpl( const char * component, sizet componentLength ) 
		: AssetPartImpl( component, componentLength )
		, mFrameDuration( 0, Max_XMP_Uns64 )
		, mFrameRateForStartFrameCount( 1, 1 )
		, mFrameRateForCountOfFrames( 1, 1 ) { }

	void AssetTimePartImpl::AppendSpecificContentToRDFSyntax( const spIUTF8String & str ) const {
		UTF8StringStream ss;
		ss << mFrameDuration.first;
		
		if ( mFrameRateForStartFrameCount.first != 1 || mFrameRateForStartFrameCount.second != 1 ) {
			if ( mFrameRateForStartFrameCount.second != 1 ) {
				ss << "f" << mFrameRateForStartFrameCount.first << "s" << mFrameRateForStartFrameCount.second;
			} else {
				ss << "f" << mFrameRateForStartFrameCount.first;
			}
		}

		if ( mFrameDuration.second != Max_XMP_Uns64 ) {
			ss << "d" << mFrameDuration.second;

			if ( mFrameRateForCountOfFrames.first != 1 || mFrameRateForCountOfFrames.second != 1 ) {
				if ( mFrameRateForCountOfFrames.second != 1 ) {
					ss << "f" << mFrameRateForCountOfFrames.first << "s" << mFrameRateForCountOfFrames.second;
				} else {
					ss << "f" << mFrameRateForCountOfFrames.first;
				}
			}
		}

		str->append( ":", 1 )->append( ss.str().c_str(),ss.str().size() );
	}

	spIAssetPart_I AssetTimePartImpl::CreateAndPopulateSpecificContent() const {
		spIAssetTimePart_I sp;
		if ( mAssetPartComponent == kAssetPartComponentUserDefined ) {
			sp = IAssetTimePart_I::CreateUserDefinedTimePart( mUserDefinedComponent->c_str(), mUserDefinedComponent->size());
		} else {
			sp = IAssetTimePart_I::CreateStandardTimePart( mAssetPartComponent );
		}
		sp->SetFrameCountDuration( mFrameDuration.first, mFrameDuration.second );
		sp->SetFrameRateForStartFrameCount( mFrameRateForStartFrameCount.first, mFrameRateForStartFrameCount.second );
		sp->SetFrameRateForCountOfFrames( mFrameRateForCountOfFrames.first, mFrameRateForCountOfFrames.second );
		return sp;
	}

	bool AssetTimePartImpl::Equals( const spcIAssetPart & other ) const {
		bool equal = false;
		if ( other ) {
			const AssetTimePartImpl * otherImpl = dynamic_cast< const AssetTimePartImpl * >( other.get() );
			if ( otherImpl ) {
				if ( otherImpl == this )
					return true;

				AutoSharedLock lock( mSharedMutex );
				AutoSharedLock otherLock( otherImpl->mSharedMutex );
				equal = AssetPartImpl::Equals( otherImpl );
				if ( !equal ) return equal;
				equal = mFrameDuration == otherImpl->mFrameDuration;
				if ( !equal ) return equal;
				equal = mFrameRateForStartFrameCount == otherImpl->mFrameRateForStartFrameCount;
				if ( !equal ) return equal;
				equal = mFrameRateForCountOfFrames == otherImpl->mFrameRateForCountOfFrames;
				return equal;
			}
		}
		return equal;
	}


	spIAssetPart APICALL AssetTimePartImpl::Clone() const {
		#if !GENERATE_XMPEXTENSIONS_CLIENT_LAYER_CODE
			AutoSharedLock lock(mSharedMutex);
			/*if (multiThreadingSupport == kMultiThreadingUseClonedObjectValue)
				multiThreadingSupport = mSharedMutex ? kMulthiThreading : kNoMultiThreading;*/
			spIAssetPart_I sp = CreateAndPopulateSpecificContent();
			sp->SetDescription(mDescription ? mDescription->c_str() : NULL, mDescription ? mDescription->size() : AdobeXMPCommon::npos);
		return sp;
		#else
				return spIAssetTimePart();
		#endif
	}

}
namespace AdobeXMPAM{
	using namespace AdobeXMPAM_Int;
	spIAssetTimePart IAssetTimePart_v1::MakeShared(pIAssetPart_base ptr) {
		if (!ptr) return spIAssetTimePart();
		pIAssetPart p = IAssetPart::GetInterfaceVersion() > 1 ? ptr->GetInterfacePointer< IAssetPart >() : ptr;
		pIAssetTimePart pt = dynamic_cast<pIAssetTimePart>(p);
		return MakeUncheckedSharedPointer(pt, __FILE__, __LINE__, false);
	}
}