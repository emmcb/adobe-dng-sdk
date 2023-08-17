#ifndef __UniversalArrayOfStructureOfSimpleNodeWithPrimaryKeyImpl_h__
#define __UniversalArrayOfStructureOfSimpleNodeWithPrimaryKeyImpl_h__ 1

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

#include "XMPCompareAndMerge/Interfaces/IThreeWayStrategy_I.h"
#include "XMPCommon/BaseClasses/MemoryManagedObject.h"
#include "XMPCommon/ImplHeaders/SharedObjectImpl.h"
#include <string>
#include <map>

#if XMP_WinBuild
#pragma warning( push )
#pragma warning( disable : 4250 )
#endif

namespace AdobeXMPCompareAndMerge_Int {

	class UniversalArrayOfStructureOfSimpleNodeWithPrimaryKeyImpl
		: public virtual IThreeWayStrategy_I
		, public virtual SharedObjectImpl
		, public virtual MemoryManagedObject
	{
	public:
		UniversalArrayOfStructureOfSimpleNodeWithPrimaryKeyImpl ( ) {}
		UniversalArrayOfStructureOfSimpleNodeWithPrimaryKeyImpl ( std::map < std::pair<std::string , std::string> , std::pair<std::string , std::string> > mprimaryKeyMap );

		virtual bool APICALL InterestedInResolvingConflict ( const spcIUTF8String & nodePath , INode::eNodeType nodeType , IThreeWayUnResolvedConflict::eConflictReason reason , bool alreadyResolved );
		virtual IThreeWayUnResolvedConflict::eConflictReason APICALL ResolveConflict ( IThreeWayUnResolvedConflict::eConflictReason conflictReason , const spcINode & baseVersionNode , const spcINode & latestVersionNode , const spcINode & userVersionNode , spcINode & resolvedNode , IThreeWayResolvedConflict::eConflictResolutionReason & conflictResolutionReason , IThreeWayResolvedConflict::eResolvedSource & resolvedSource );

	protected:
		virtual ~UniversalArrayOfStructureOfSimpleNodeWithPrimaryKeyImpl ( ) __NOTHROW__ {}

		std::map <std::pair<std::string , std::string> , std::pair<std::string , std::string> > mprimaryKeyMap;
		std::string mprimaryKeyNamespace;
		std::string mprimaryKeyName;

	};
}

#if XMP_WinBuild
#pragma warning( pop )
#endif

#endif  // __UniversalUnorderedArrayOfStructureOfSimpleNodeWithoutPrimaryKeyImpl_h__
