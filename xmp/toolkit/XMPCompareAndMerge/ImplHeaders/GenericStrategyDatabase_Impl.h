#ifndef __GenericStrategyDatabase_Impl_h__
#define __GenericStrategyDatabase_Impl_h__ 1

// =================================================================================================
// ADOBE SYSTEMS INCORPORATED
// Copyright 2014 Adobe Systems Incorporated
// All Rights Reserved
//
// NOTICE:  Adobe permits you to use, modify, and distribute this file in accordance with the terms
// of the Adobe license agreement accompanying it.
// =================================================================================================

#if !(IMPLEMENTATION_HEADERS_CAN_BE_INCLUDED)
	#error "Not adhering to design constraints"
	// this file should only be included from its own cpp file
#endif 

#include "XMPCompareAndMerge/XMPCompareAndMergeFwdDeclarations_I.h"
#include "XMPCommon/BaseClasses/IntrusiveRefCountSharedOwnership.h"
#include "XMPCommon/Interfaces/BaseInterfaces/ISharedOwnership.h"
#include "XMPCommon/BaseClasses/Object.h"

#include "XMPCompareAndMerge/Interfaces/IGenericStrategyDatabase_I.h"


#if XMP_WinBuild
	#pragma warning( push )
	#pragma warning( disable : 4250 )
#endif

namespace NS_INT_XMPCOMPAREANDMERGE {

	using namespace NS_XMPCOMMON;
	using namespace NS_XMPCORE;
	using namespace NS_XMPCOMPAREANDMERGE;
	
	class GenericStrategyDatabaseImpl
		:public virtual IGenericStrategyDatabase_I
		,public virtual Object
	{
	public:
		virtual spIThreeWayGenericConflictResolutionStrategy_I GetStrategy_I(eStrategyType strategyType);
		virtual void AddStrategy_I(eStrategyType key, spIThreeWayGenericConflictResolutionStrategy_I strategy);

		//virtual std::list<eStrategyType> GetStrategyTypeList()const __NOTHROW__;
		virtual ~GenericStrategyDatabaseImpl() __NOTHROW__;
	
	protected:
		//std::list<eStrategyType> keys;
		

	#ifndef FRIEND_CLASS_DECLARATION            
		#define FRIEND_CLASS_DECLARATION() 
	#endif
		FRIEND_CLASS_DECLARATION();

	};
	inline GenericStrategyDatabaseImpl::~GenericStrategyDatabaseImpl() __NOTHROW__{};




}
#if XMP_WinBuild
	#pragma warning( pop )
#endif
#endif