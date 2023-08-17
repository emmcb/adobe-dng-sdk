#ifndef XMPCompareAndMergeFwdDeclarations_I_h__
#define XMPCompareAndMergeFwdDeclarations_I_h__ 1

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

#include "XMPCompareAndMerge/XMPCompareAndMergeDefines_I.h"
#include "XMPCompareAndMerge/XMPCompareAndMergeFwdDeclarations.h"
#include "XMPCommon/XMPCommonFwdDeclarations_I.h"

#include <vector>

#include "XMPCommon/Utilities/TAllocator.h"

namespace AdobeXMPCompareAndMerge_Int {
	using namespace AdobeXMPCompareAndMerge;
	using namespace AdobeXMPCommon;
	using namespace AdobeXMPCommon_Int;

	//ITwoWayConflictIdentificationEngine
	class ITwoWayConflictIdentificationEngine;
	typedef ITwoWayConflictIdentificationEngine *						pITwoWayConflictIdentificationEngine;
	typedef const ITwoWayConflictIdentificationEngine*					pcITwoWayConflictIdentificationEngine;
	typedef shared_ptr< ITwoWayConflictIdentificationEngine >			spITwoWayConflictIdentificationEngine;
	typedef shared_ptr< const ITwoWayConflictIdentificationEngine >		spcITwoWayConflictIdentificationEngine;

	//ITwoWayUnResolvedConflict
	class ITwoWayUnResolvedConflict;
	typedef ITwoWayUnResolvedConflict *									pITwoWayUnResolvedConflict;
	typedef const ITwoWayUnResolvedConflict *							pcITwoWayUnResolvedConflict;
	typedef shared_ptr< ITwoWayUnResolvedConflict >						spITwoWayUnResolvedConflict;
	typedef shared_ptr< const ITwoWayUnResolvedConflict >				spcITwoWayUnResolvedConflict;

	//ITwoWayResolvedConflict
	class ITwoWayResolvedConflict;	
	typedef ITwoWayResolvedConflict *									pITwoWayResolvedConflict;
	typedef const ITwoWayResolvedConflict *								pcITwoWayResolvedConflict;
	typedef shared_ptr< ITwoWayResolvedConflict >						spITwoWayResolvedConflict;
	typedef shared_ptr< const ITwoWayResolvedConflict >					spcITwoWayResolvedConflict;

	//ITwoWayConflictResolutionStrategy
	class ITwoWayConflictResolutionStrategy;
	typedef ITwoWayConflictResolutionStrategy *							pITwoWayConflictResolutionStrategy;
	typedef const ITwoWayConflictResolutionStrategy *					pcITwoWayConflictResolutionStrategy;
	typedef shared_ptr< ITwoWayConflictResolutionStrategy >				spITwoWayConflictResolutionStrategy;
	typedef shared_ptr< const ITwoWayConflictResolutionStrategy >		spcITwoWayConflictResolutionStrategy;

	//ITwoWayConflictResolutionEngine
	class ITwoWayConflictResolutionEngine;
	typedef ITwoWayConflictResolutionEngine *							pITwoWayConflictResolutionEngine;
	typedef const ITwoWayConflictResolutionEngine *						pcITwoWayConflictResolutionEngine;
	typedef shared_ptr< ITwoWayConflictResolutionEngine >				spITwoWayConflictResolutionEngine;
	typedef shared_ptr< const ITwoWayConflictResolutionEngine >			spcITwoWayConflictResolutionEngine;

	//ITwoWayValueComparisonResult
	class ITwoWayValueComparisonResult;
	typedef ITwoWayValueComparisonResult *								pITwoWayValueComparisonResult;
	typedef const ITwoWayValueComparisonResult *						pcITwoWayValueComparisonResult;
	typedef shared_ptr< ITwoWayValueComparisonResult >					spITwoWayValueComparisonResult;
	typedef shared_ptr< const ITwoWayValueComparisonResult >			spcITwoWayValueComparisonResult;

	//ITwoWayBasicComparisonResult
	class ITwoWayBasicComparisonResult;
	typedef ITwoWayBasicComparisonResult *								pITwoWayBasicComparisonResult;
	typedef const ITwoWayBasicComparisonResult *						pcITwoWayBasicComparisonResult;
	typedef shared_ptr< ITwoWayBasicComparisonResult >					spITwoWayBasicComparisonResult;
	typedef shared_ptr< const ITwoWayBasicComparisonResult >			spcITwoWayBasicComparisonResult;

	//ITwoWayComparisonEngine
	class ITwoWayComparisonEngine;
	typedef ITwoWayComparisonEngine *									pITwoWayComparisonEngine;
	typedef const ITwoWayComparisonEngine *								pcITwoWayComparisonEngine;
	typedef shared_ptr< ITwoWayComparisonEngine >						spITwoWayComparisonEngine;
	typedef shared_ptr< const ITwoWayComparisonEngine >					spcITwoWayComparisonEngine;

	//IThreeWayConflictIdentificationEngine
	class IThreeWayConflictIdentificationEngine;
	typedef IThreeWayConflictIdentificationEngine *						pIThreeWayConflictIdentificationEngine;
	typedef const IThreeWayConflictIdentificationEngine *				pcIThreeWayConflictIdentificationEngine;
	typedef shared_ptr< IThreeWayConflictIdentificationEngine >			spIThreeWayConflictIdentificationEngine;
	typedef shared_ptr< const IThreeWayConflictIdentificationEngine >	spcIThreeWayConflictIdentificationEngine;

	//IThreeWayConflictResolutionEngine
	class IThreeWayConflictResolutionEngine;
	typedef IThreeWayConflictResolutionEngine *							pIThreeWayConflictResolutionEngine;
	typedef const IThreeWayConflictResolutionEngine *					pcIThreeWayConflictResolutionEngine;
	typedef shared_ptr< IThreeWayConflictResolutionEngine >				spIThreeWayConflictResolutionEngine;
	typedef shared_ptr< const IThreeWayConflictResolutionEngine >		spcIThreeWayConflictResolutionEngine;

	//IThreeWayStrategy
	typedef shared_ptr< IThreeWayStrategy >								spIThreeWayStrategy;
	typedef shared_ptr< const IThreeWayStrategy >						spcIThreeWayStrategy;

	// typdefs for vectors and their corresponding shared pointers
	typedef std::vector< spITwoWayUnResolvedConflict >									ITwoWayUnResolvedConflicts;
	typedef shared_ptr< ITwoWayUnResolvedConflicts >									spITwoWayUnResolvedConflicts;

	typedef std::vector< spcITwoWayUnResolvedConflict >									cITwoWayUnResolvedConflicts;
	typedef shared_ptr< cITwoWayUnResolvedConflicts >									spcITwoWayUnResolvedConflicts;

	typedef std::vector< spITwoWayResolvedConflict >									ITwoWayResolvedConflicts;
	typedef shared_ptr< ITwoWayResolvedConflicts >										spITwoWayResolvedConflicts;

	typedef std::vector< spcITwoWayResolvedConflict >									cITwoWayResolvedConflicts;
	typedef shared_ptr< cITwoWayResolvedConflicts >										spcITwoWayResolvedConflicts;

	typedef std::vector< spcITwoWayConflictResolutionStrategy >							ITwoWayConflictResolutionStrategies;
	typedef shared_ptr< ITwoWayConflictResolutionStrategies >							spITwoWayConflictResolutionStrategies;

	typedef std::vector< spcITwoWayValueComparisonResult >								ITwoWayValueComparisonResults;
	typedef shared_ptr< ITwoWayValueComparisonResults >									spITwoWayValueComparisonResults;

	typedef std::vector< spITwoWayValueComparisonResults >								ITwoWayValueComparisonResultMatrix;
	typedef shared_ptr< ITwoWayValueComparisonResultMatrix >							spITwoWayValueComparisonResultMatrix;

	typedef std::vector< spIThreeWayStrategy >											IThreeWayStrategies;
	typedef shared_ptr< IThreeWayStrategies >											spIThreeWayStrategies;

	typedef std::pair< spIThreeWayUnResolvedConflicts, spIThreeWayResolvedConflicts >	ThreeWayUnResolvedResolvedConflicts;
	typedef std::pair< spcIThreeWayUnResolvedConflicts, spcIThreeWayResolvedConflicts >	cThreeWayUnResolvedResolvedConflicts;

	extern TAllocator< spITwoWayResolvedConflict >										gTAllocatorTwoWayResolvedConflict;
	extern TAllocator< spITwoWayUnResolvedConflict >									gTAllocatorTwoWayUnResolvedConflict;
	extern TAllocator< spITwoWayValueComparisonResult >									gTAllocatorTwoWayValueComparisonResult;
	extern TAllocator< spITwoWayValueComparisonResults >								gTAllocatorTwoWayValueComparisonResults;
	extern TAllocator< spITwoWayConflictResolutionStrategy >							gTAllocatorTwoWayConlictResolutionStrategy;
	extern TAllocator< spIThreeWayResolvedConflict >									gTAllocatorThreeWayResolvedConflict;
	extern TAllocator< spIThreeWayUnResolvedConflict >									gTAllocatorThreeWayUnResolvedConflict;
	extern TAllocator< spIThreeWayStrategy >											gTAllocatorThreeWayStrategies;

}

#endif // XMPCompareAndMergeFwdDeclarations_I_h__
