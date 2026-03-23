#ifndef ThreeWayGenericStrategyImpl_h__
#define ThreeWayGenericStrategyImpl_h__ 1

// =================================================================================================
// Copyright Adobe
// Copyright 2015 Adobe
// All Rights Reserved
//
// NOTICE:  Adobe permits you to use, modify, and distribute this file in accordance with the terms
// of the Adobe license agreement accompanying it. If you have received this file from a source other 
// than Adobe, then your use, modification, or distribution of it requires the prior written permission
// of Adobe.
// =================================================================================================

#include "XMPCompareAndMerge/Interfaces/IThreeWayGenericStrategy_I.h"
#include "XMPCommon/BaseClasses/MemoryManagedObject.h"
#include "XMPCommon/ImplHeaders/SharedObjectImpl.h"

#if XMP_WinBuild
	#pragma warning( push )
	#pragma warning( disable : 4250 )
#endif

namespace AdobeXMPCompareAndMerge_Int {

	class ThreeWayGenericStrategyImpl
		: public virtual IThreeWayGenericStrategy_I
		, public virtual SharedObjectImpl
		, public virtual MemoryManagedObject
	{
	public:
		ThreeWayGenericStrategyImpl();

		virtual bool APICALL AppendPath( const char * path, sizet pathSize );
		virtual spcIUTF8String APICALL GetPath( sizet index ) const;
		virtual const spcIUTF8Strings_const APICALL GetPaths() const;
		virtual sizet APICALL PathCount() const __NOTHROW__;
		virtual spIUTF8String APICALL RemovePath( const char * path, sizet pathSize );
		virtual spIThreeWayGenericStrategy APICALL Clone() const;

	protected:
		virtual ~ThreeWayGenericStrategyImpl() __NOTHROW__ {}
		virtual ThreeWayGenericStrategyImpl * APICALL createAndCloneContents() const = 0;

		spIUTF8Strings					mPaths;
	#ifdef FRIEND_CLASS_DECLARATION
		FRIEND_CLASS_DECLARATION();
	#endif
		REQ_FRIEND_CLASS_DECLARATION();
	};
}

#if XMP_WinBuild
	#pragma warning( pop )
#endif
#endif  // ThreeWayGenericStrategyImpl_h__

