#ifndef __AutoMergeUtilities_h__
#define __AutoMergeUtilities_h__ 1

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

#include "XMPCompareAndMerge/XMPCompareAndMergeFwdDeclarations_I.h"
#include "XMPCore/XMPCoreFwdDeclarations.h"

#include "XMPCommon/Interfaces/IError_I.h"
#include "XMPCompareAndMerge/XMPCompareAndMergeErrorCodes.h"
#include "XMPCore/XMPCoreErrorCodes.h"
#include "XMPCommon/Interfaces/IErrorNotifier_I.h"

#include "XMPCommon/Utilities/TSmartPointers_I.h"
#include "XMPCommon/Utilities/IUTF8StringComparator.h"
#include "XMPCommon/Interfaces/IUTF8String.h"

#include "XMPCore/Interfaces/ISimpleNode.h"
#include "XMPCore/Interfaces/INode.h"
#include "XMPCore/Interfaces/IArrayNode.h"
#include "XMPCore/Interfaces/IStructureNode.h"
#include "XMPCore/Interfaces/INodeIterator.h"
#include <string>
#include <algorithm>
#include <map>
#include <vector>

namespace AdobeXMPCompareAndMerge_Int {
	class AutoMergeUtilities
	{
	public:
		static bool verifyChildType(const spcIArrayNode arrayNode);
		static bool verifyChildTypeArrOfStructOfSimpleAndOrArr(const spcIArrayNode arrayNode);
		static bool verifyChildTypeArrOfStructOfSimpleAndStruct ( const spcIArrayNode arrayNode );
		static bool compareOrderedArrayOfSimple ( const spcIArrayNode arrayNode , const spcIArrayNode arrayNode1, bool compareQualifiers = true);
		static bool compareArrayOfSimple ( const spcIArrayNode arrayNode1 , const spcIArrayNode arrayNode2, bool compareQualifiers = true);
		static void resolveQualifier( const spcINode baseNode, const spcINode latestNode, const spcINode userNode, const spINode & nodeForQualifier);
		static bool compareStructs( const spcIStructureNode latestNode, const spcIStructureNode userNode , bool compareQualifiers = true );
		static bool compareStructOfSimpleAndStruct ( const spcIStructureNode struct1, const spcIStructureNode struct2, bool compareQualifiers = true);
		static bool compareStructOfSimpleAndArr(const spcIStructureNode struct1, const spcIStructureNode struct2, bool checkOrder = false, bool compareQualifiers = true);
		static void makeUnion( const spcIArrayNode baseNode, const spcIArrayNode latestNode, const spcIArrayNode userNode, const spIArrayNode& mergedNode, bool useBase);
		static void makeUnion(const spcIStructureNode baseNode, const spcIStructureNode latestNode, const spcIStructureNode userNode, const spIStructureNode& mergedNode , bool useBase);
		static bool isQualEqual(spcINodeIterator it1, spcINodeIterator it2);
		static bool compareQualifier(spcISimpleNode node1, spcISimpleNode node2);
	};
}


#endif  // __AutoMergeUtilities_h__
