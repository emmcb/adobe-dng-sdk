#ifndef __CompareAndMergeObjectFactoryImpl_h__
#define __CompareAndMergeObjectFactoryImpl_h__ 1

// =================================================================================================
// Copyright Adobe
// Copyright 2014 Adobe
// All Rights Reserved
//
// NOTICE:  Adobe permits you to use, modify, and distribute this file in accordance with the terms
// of the Adobe license agreement accompanying it. If you have received this file from a source other 
// than Adobe, then your use, modification, or distribution of it requires the prior written permission
// of Adobe.
// =================================================================================================

#if !(IMPLEMENTATION_HEADERS_CAN_BE_INCLUDED)
	#error "Not adhering to design constraints"
	// this file should only be included from its own cpp file
#endif

#include "XMPCompareAndMerge/Interfaces/ICompareAndMergeObjectFactory_I.h"

#if XMP_WinBuild
	#pragma warning( push )
	#pragma warning( disable : 4250 )
#endif

namespace AdobeXMPCompareAndMerge_Int {

	class CompareAndMergeObjectFactoryImpl
		: public virtual ICompareAndMergeObjectFactory_I
	{
	public:
		CompareAndMergeObjectFactoryImpl() {}
		virtual ~CompareAndMergeObjectFactoryImpl() __NOTHROW__ {}

		virtual pIThreeWayMerge_base APICALL CreateThreeWayMerge( pcIError_base & error ) __NOTHROW__;
		virtual pcIGenericStrategyDatabase_base APICALL GetGenericStrategyDatabase( pcIError_base & error ) __NOTHROW__;
		virtual pIError_base APICALL CreateError( uint32 domain, uint32 code, uint32 severity, pcIError_base & error ) __NOTHROW__;
		virtual pIUTF8String_base APICALL CreateUTF8String( const char * buf, sizet count, pcIError_base & error ) __NOTHROW__;

	protected:

	#ifdef FRIEND_CLASS_DECLARATION
		FRIEND_CLASS_DECLARATION();
	#endif
		REQ_FRIEND_CLASS_DECLARATION();
	};
}

#if XMP_WinBuild
	#pragma warning( pop )
#endif

#endif  // __CompareAndMergeObjectFactoryImpl_h__
