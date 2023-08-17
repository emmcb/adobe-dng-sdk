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

#if AdobePrivate
// =================================================================================================
// Change history
// ==============
//
// Writers:
//  AJ	Abhishek Jindal
//
// mm-dd-yy who Description of changes, most recent first.
//
// 03-27-15 AJ  5.6-c068 Checking in complete implementation of XMPUtils with the new Core APIs
//
// =================================================================================================
#endif


#include "public/include/XMP_Environment.h"	// ! This must be the first include!
#include "XMPCore/source/XMPCore_Impl.hpp"
#include "XMPCore/XMPCoreDefines.h"
#if ENABLE_CPP_DOM_MODEL

#include "XMPCore/source/XMPUtils.hpp"
#include "source/UnicodeInlines.incl_cpp"
#include "source/UnicodeConversions.hpp"
#include "source/ExpatAdapter.hpp"
#include "third-party/zuid/interfaces/MD5.h"
#include "XMPCore/Interfaces/IMetadata_I.h"
#include "XMPCore/Interfaces/IArrayNode_I.h"
#include "XMPCore/Interfaces/ISimpleNode_I.h"
#include "XMPCore/Interfaces/INodeIterator.h"
#include "XMPCore/Interfaces/IPathSegment_I.h"
#include "XMPCore/Interfaces/IPath_I.h"
#include "XMPCore/Interfaces/INameSpacePrefixMap_I.h"
#include "XMPCore/Interfaces/IDOMImplementationRegistry_I.h"

#include "XMPCommon/Interfaces/IUTF8String_I.h"

const XMP_VarString xmlNameSpace = "http://www.w3.org/XML/1998/namespace";
extern const char * sListProps[];
extern const char * sDateProps[];

using namespace AdobeXMPCommon;
using namespace AdobeXMPCore;

// =================================================================================================
// CloneSubtree
// ============



static void CloneIXMPSubtree(const spcINode & origRoot, const spINode & cloneParent, bool skipEmpty /* = false */)
{
	spINode clonedRoot = origRoot->Clone(skipEmpty, true);
	if (!clonedRoot) return;
	if (cloneParent->GetNodeType() == INode::kNTArray) {
		cloneParent->ConvertToArrayNode()->AppendNode( clonedRoot );
	} else if (cloneParent->GetNodeType() == INode::kNTStructure) {
		cloneParent->ConvertToStructureNode()->InsertNode( cloneParent );
	}

}	// CloneSubtree

// =================================================================================================
// LookupLangItem
// ==============
//
// ! Assumes that the language value is already normalized.

static XMP_Index LookupIXMPLangItem(const spcIArrayNode & arrayNode, XMP_VarString & lang)
{
	XMP_OptionBits arrayOptions = XMPUtils::GetIXMPOptions(arrayNode);
	if (!(arrayOptions & kXMP_PropValueIsArray)) {	// *** Check for alt-text?
		XMP_Throw("Language item must be used on array", kXMPErr_BadXPath);
	}

	XMP_Index index = 1;
	XMP_Index itemLim = (XMP_Index)arrayNode->ChildCount();

	for (; index <= itemLim; ++index) {
		spcINode currItem = arrayNode->GetNodeAtIndex(index);

		if (!currItem->HasQualifiers()) continue;
		spcINode langQual = currItem->GetQualifier(xmlNameSpace.c_str(), xmlNameSpace.size(), "lang", AdobeXMPCommon::npos );
		if (!langQual) continue;
		if (langQual->GetNodeType() != INode::kNTSimple)  continue;
		XMP_VarString langQualValue = langQual->ConvertToSimpleNode()->GetValue()->c_str();
		if (langQualValue == lang) break;
	}

	if (index == itemLim + 1) index = -1;
	return index;

}	// LookupLangItem
// =================================================================================================


// CompareSubtrees
// ===============
//
// Compare 2 subtrees for semantic equality. The comparison includes value, qualifiers, and form.
// Schemas, top level properties, struct fields, and qualifiers are allowed to have differing order,
// the appropriate right node is found from the left node's name. Alt-text arrays are allowed to be
// in differing language order, other arrays are compared in order.

// *** Might someday consider sorting unordered arrays.
// *** Should expose this through XMPUtils.

static bool CompareSubtrees(spcINode leftNode, spcINode rightNode)
{
	// Don't compare the names here, we want to allow the outermost roots to have different names.
	XMP_OptionBits leftNodeOptions = XMPUtils::GetIXMPOptions(leftNode), rightNodeOptions = XMPUtils::GetIXMPOptions(rightNode);
	if (leftNode->GetNodeType() != rightNode->GetNodeType()) return false;
	if (leftNode->GetNodeType() == INode::kNTSimple) {
		if ( strcmp( leftNode->ConvertToSimpleNode()->GetValue()->c_str(), rightNode->ConvertToSimpleNode()->GetValue()->c_str() ) ) {
			return false;
		}
	}
	if (XMPUtils::GetNodeChildCount(leftNode) != XMPUtils::GetNodeChildCount(rightNode)) return false;
	if (leftNode->HasQualifiers() != rightNode->HasQualifiers() ) return false;
	if (leftNode->HasQualifiers()) {
		for (auto leftQualIter = leftNode->QualifiersIterator(); leftQualIter; leftQualIter = leftQualIter->Next()) {
			spcINode leftQualNode = leftQualIter->GetNode();
			spcINode righQualNode = rightNode->GetINode_I()->GetQualifier( leftQualNode->GetNameSpace(), leftQualNode->GetName() );
			if (!righQualNode || !CompareSubtrees(leftQualNode, righQualNode)){
				return false;
			}

		}
	}

	if (leftNode->GetNodeType() == INode::kNTStructure ) {
		for ( auto leftChildIter = XMPUtils::GetNodeChildIterator(leftNode); leftChildIter; leftChildIter = leftChildIter->Next()) {
			spcINode leftChildNode = leftChildIter->GetNode();
			spcINode rightChildNode = XMPUtils::FindChildNode( AdobeXMPCore_Int::const_pointer_cast<INode>(rightNode), leftChildNode->GetName()->c_str(), leftChildNode->GetNameSpace()->c_str(), false, 0);
			if (!rightChildNode || !CompareSubtrees(leftChildNode, rightChildNode)) {
				return false;
			}
		}
	}


	else if (leftNodeOptions & kXMP_PropArrayIsAltText) {
		// The parent node is an alt-text array.
		auto leftNodeAsArray = leftNode->ConvertToArrayNode();
		auto rightNodeAsArray = rightNode->ConvertToArrayNode();

		size_t leftNodeChildCount = XMPUtils::GetNodeChildCount(leftNode);
		for (size_t idx = 1; idx <= leftNodeChildCount; ++idx) {
			spcINode leftChild = leftNodeAsArray->GetNodeAtIndex(idx);
			spcINode leftChildFirstQualifier = leftChild->GetQualifier(xmlNameSpace.c_str(), xmlNameSpace.size(), "lang", AdobeXMPCommon::npos );
			if (leftChildFirstQualifier) {
				XMP_VarString leftChildFirstQualifierValue = leftChildFirstQualifier->ConvertToSimpleNode()->GetValue()->c_str();
				XMP_Index rightIdx = LookupIXMPLangItem( rightNodeAsArray, leftChildFirstQualifierValue );
				if (rightIdx == -1) {
					return false;
				}
				spcINode rightChild = rightNodeAsArray->GetNodeAtIndex( rightIdx );
				if (!CompareSubtrees(leftChild, rightChild)) {
					return false;
				}
			}
			
		}


	}
	else {

		// The parent must be simple or some other (not alt-text) kind of array.
		XMP_Assert((!(leftNodeOptions & kXMP_PropCompositeMask)) || (leftNodeOptions & kXMP_PropValueIsArray));
		auto leftNodeAsArray = leftNode->ConvertToArrayNode();
		auto rightNodeAsArray = rightNode->ConvertToArrayNode();

		size_t leftNodeChildCount = XMPUtils::GetNodeChildCount(leftNode);
		for (size_t idx = 1; idx <= leftNodeChildCount; ++idx) {
			spcINode leftChild = leftNodeAsArray->GetNodeAtIndex(idx);
			spcINode rightChild = rightNodeAsArray->GetNodeAtIndex(idx);
			if (!CompareSubtrees(leftChild, rightChild)) {
				return false;
			}
		}

	}
	return true;
}// CompareSubtrees

// -------------------------------------------------------------------------------------------------
// MergeArrayItems
// ---------------

static void
MergeArrayItems(spINode newArray, spINode mergedArray)
{
	XMP_Assert(newArray->GetNodeType() == INode::kNTArray);
	XMP_Assert(mergedArray->GetNodeType() == INode::kNTArray);

	auto newArrayAsArrayNode = newArray->ConvertToArrayNode();
	auto mergedArrayAsArrayNode = mergedArray->ConvertToArrayNode();

	for (size_t newNum = 1, newLim = XMPUtils::GetNodeChildCount(newArray); newNum <= newLim; ++newNum) {

		spcINode newItem = newArrayAsArrayNode->GetNodeAtIndex(newNum);
		size_t mergedNum, mergedLim;
		for (mergedNum = 1, mergedLim = XMPUtils::GetNodeChildCount(mergedArray) + 1; mergedNum < mergedLim; ++mergedNum) {
			spcINode mergedItem = mergedArrayAsArrayNode->GetNodeAtIndex(mergedNum);
			if (CompareSubtrees(newItem, mergedItem)) break;
		}

		if (mergedNum == mergedLim) CloneIXMPSubtree(newItem, mergedArray, false);

	}

}	// MergeArrayItems
#if AdobePrivate
// -------------------------------------------------------------------------------------------------
// UpdateArrayDifference
// ---------------------

static void
UpdateArrayDifference(spINode dpItem, spINode propNode)
{
	XMP_Assert(propNode->GetNodeType() == INode::kNTArray);

	spINode mergedList = XMPUtils::FindChildNode(dpItem, "DiffMergedList", kXMP_NS_Transient,kXMP_ExistingOnly, 0);
	XMP_Assert(mergedList);

	MergeArrayItems(propNode, mergedList);

}	// UpdateArrayDifference
// -------------------------------------------------------------------------------------------------
// UpdateDateDifference
// --------------------

static void
UpdateDateDifference (spINode dpItem, spcINode propNode )
{
	spINode oldestNode = XMPUtils::FindChildNode ( dpItem, "DiffOldest", kXMP_NS_Transient, kXMP_ExistingOnly, 0);
	spINode newestNode = XMPUtils::FindChildNode ( dpItem, "DiffNewest",  kXMP_NS_Transient, kXMP_ExistingOnly, 0);
	XMP_Assert ( (oldestNode != libcppNULL) && (newestNode != libcppNULL) );
	
	XMP_DateTime propDate;
	XMP_DateTime rangeDate;
	
	if (propNode->GetNodeType() != INode::kNTSimple) return;
	
	spcIUTF8String propNodeValue = propNode->ConvertToSimpleNode()->GetValue();
	spcIUTF8String oldestNodeValue = oldestNode->ConvertToSimpleNode()->GetValue();
	if ( propNodeValue->empty() ) return;	// ! Ignore missing dates.
	XMPUtils::ConvertToDate ( propNodeValue->c_str(), &propDate );
	XMPUtils::ConvertToDate ( oldestNodeValue->c_str(), &rangeDate );

	if ( XMPUtils::CompareDateTime ( propDate, rangeDate ) == -1 ) {
		oldestNode->ConvertToSimpleNode()->SetValue( propNodeValue->c_str(), propNodeValue->size() );
	} else {
		XMPUtils::ConvertToDate( newestNode->ConvertToSimpleNode()->GetValue()->c_str(), &rangeDate);
		if ( XMPUtils::CompareDateTime ( propDate, rangeDate ) == +1 ) {
			newestNode->ConvertToSimpleNode()->SetValue(propNodeValue->c_str(), propNodeValue->size());
		}
	}
			
}	// UpdateDateDifference

static void AddNewDifference(spIArrayNode dpArray, spINode propNode, bool propIsList, bool propIsDate)
{
	auto defaultMap = INameSpacePrefixMap::GetDefaultNameSpacePrefixMap()->GetINameSpacePrefixMap_I();
	XMP_VarString currNameSpace = defaultMap->GetPrefix( propNode->GetNameSpace() )->c_str();
	XMP_VarString nodeFullName = currNameSpace + ":" + propNode->GetName()->c_str();
	XMP_Assert(XMPUtils::LookupFieldSelector_v2(dpArray, "DiffPath", nodeFullName) == -1);

	
	spIStructureNode dpItem = IStructureNode::CreateStructureNode(dpArray->GetNameSpace()->c_str(), dpArray->GetNameSpace()->size(),
		kXMP_ArrayItemName, AdobeXMPCommon::npos );
	dpArray->AppendNode(dpItem);

	spISimpleNode dpName = ISimpleNode::CreateSimpleNode(kXMP_NS_Transient, AdobeXMPCommon::npos, "DiffPath", AdobeXMPCommon::npos,
		nodeFullName.c_str(), nodeFullName.size() );
	dpItem->AppendNode(dpName);
	spcINode propSchema = propNode->GetParent();
	 
	
	spISimpleNode dpSchema = ISimpleNode::CreateSimpleNode(kXMP_NS_Transient, AdobeXMPCommon::npos, "DiffURI", AdobeXMPCommon::npos,
		propNode->GetNameSpace()->c_str(), propNode->GetNameSpace()->size() );
	dpItem->AppendNode(dpSchema);
	if (propIsList) {
		spIArrayNode mergedList = AdobeXMPCore_Int::IArrayNode_I::CreateArrayNode(
			AdobeXMPCore_Int::IUTF8String_I::CreateUTF8String( kXMP_NS_Transient, AdobeXMPCommon::npos ),
			AdobeXMPCore_Int::IUTF8String_I::CreateUTF8String( "DiffMergedList", AdobeXMPCommon::npos ),
			propNode->ConvertToArrayNode()->GetArrayForm());

		//TODO
		//XMP_Node * mergedList = new XMP_Node(dpItem, "xmpx:DiffMergedList", "", (propNode.options & kXMP_PropArrayFormMask));
		
		dpItem->AppendNode(mergedList);
		MergeArrayItems(propNode, mergedList);	// ! Remove duplicates from first part.
	}
	else if (propIsDate) {
		XMP_Assert(propNode->GetNodeType() == INode::kNTSimple); // ! Catch bool param reversal.
		spcIUTF8String propNodeValue = propNode->ConvertToSimpleNode()->GetValue();
		spISimpleNode dateNode1 = ISimpleNode::CreateSimpleNode( kXMP_NS_Transient, AdobeXMPCommon::npos,
			"DiffOldest", AdobeXMPCommon::npos, propNodeValue->c_str(), propNodeValue->size() );
		spISimpleNode dateNode2 = ISimpleNode::CreateSimpleNode(kXMP_NS_Transient, AdobeXMPCommon::npos,
			"DiffNewest", AdobeXMPCommon::npos, propNodeValue->c_str(), propNodeValue->size() );
		dpItem->AppendNode(dateNode1);
		dpItem->AppendNode(dateNode2);
	}

}	// AddNewDifference

static void
NoteDifferingProperty_v2 ( spINode xmpxNode,
						  spINode  multiProp,		// ! Might be null!
						  spINode inputProp )	// ! Might be null!
{
	XMP_Assert ( (multiProp != libcppNULL) || (inputProp != libcppNULL) ); // Can't have both prop pointers be null.

	// ---------------------------------------------------------------------------------------------
	// Do some common setup, find the xmpx:DifferingProperties array, see if the property is already
	// noted, decide if the property is a date or catenated list.
	
	spINode dpArrayNode = XMPUtils::FindChildNode(xmpxNode, "DifferingProperties", kXMP_NS_Transient, false, 0);
	XMP_Assert ( dpArrayNode );
	spIArrayNode dpArray = dpArrayNode->ConvertToArrayNode();
	spINode propNode = multiProp;
	if (!propNode) propNode = inputProp;
	XMP_Assert ( propNode);	// Can't have both prop pointers be null.
	auto defaultMap = INameSpacePrefixMap::GetDefaultNameSpacePrefixMap()->GetINameSpacePrefixMap_I();
	XMP_VarString currNameSpace = defaultMap->GetPrefix(propNode->GetNameSpace())->c_str();
	XMP_VarString nodeFullName = currNameSpace + ":" + propNode->GetName()->c_str();
	spINode dpItem;
	XMP_Index dpItemIndex = XMPUtils::LookupFieldSelector_v2(dpArray, "DiffPath", nodeFullName);
	if ( dpItemIndex != -1 ) dpItem = dpArray->GetNodeAtIndex(dpItemIndex);
	
	const bool propIsList = CheckSpecialProperty ( nodeFullName, sListProps );
	const bool propIsDate = CheckSpecialProperty ( nodeFullName, sDateProps );

	// -----------------------
	// Process the difference.
	
	if ( !inputProp ) {

		// All prior input matched, this input lacks the property.
		XMP_Assert ( !dpItem );
		AddNewDifference ( dpArray, multiProp, propIsList, propIsDate );
		dpItem = dpArray->GetNodeAtIndex(dpArray->ChildCount()); 
		#if TraceMultiFile
			printf ( "	  New input lacks %s\n", multiProp->name.c_str() ); fflush(stdout);
		#endif
					
	} else if ( multiProp) {
	
		// The aggregate and input both have the property. All prior input matched, this input differs.
		XMP_Assert ( dpItem == libcppNULL );
		AddNewDifference ( dpArray, multiProp, propIsList, propIsDate );
		dpItem = dpArray->GetNodeAtIndex(dpArray->ChildCount());
		if ( propIsDate ) {
			UpdateDateDifference ( dpItem, inputProp );
		} else if ( propIsList ) {
			UpdateArrayDifference ( dpItem, inputProp );
		}
		#if TraceMultiFile
			printf ( "	  New input differs for %s\n", inputProp->name.c_str() ); fflush(stdout);
		#endif
	
	} else if ( !dpItem ) {
	
		// The property is appearing for the first time in this input.
	
		AddNewDifference ( dpArray, inputProp, propIsList, propIsDate );
		dpItem = dpArray->GetNodeAtIndex(dpArray->ChildCount());
		#if TraceMultiFile
			printf ( "	  Prior input lacks %s\n", inputProp->name.c_str() ); fflush(stdout);
		#endif
	
	} else {
	
		// The property is already noted. Since it is present in the input, update a date
		// range and merge a catenated list. Nothing else to do for regular properties.
		XMP_Assert ( dpItem );
		if ( propIsDate ) {
			UpdateDateDifference ( dpItem, inputProp );
		} else if ( propIsList ) {
			UpdateArrayDifference ( dpItem, inputProp );
		}
	
	}

}	// NoteDifferingProperty
#endif //AdobePrivate

static bool
ItemValuesMatch(spcINode leftNode, spcINode rightNode)
{
	if (!leftNode && !rightNode) return true;
	if (!leftNode || !rightNode) return false;
	const XMP_OptionBits leftNodeOptions = XMPUtils::GetIXMPOptions(leftNode);
	const XMP_OptionBits rightNodeOptions = XMPUtils::GetIXMPOptions(rightNode);
	const XMP_OptionBits leftForm = leftNodeOptions & kXMP_PropCompositeMask;
	const XMP_OptionBits rightForm = rightNodeOptions & kXMP_PropCompositeMask;

	if (leftForm != rightForm) return false;

	if (leftForm == 0) {

		// Simple nodes, check the values and xml:lang qualifiers.
		XMP_VarString leftValue = leftNode->ConvertToSimpleNode()->GetValue()->c_str();
		XMP_VarString rightValue = rightNode->ConvertToSimpleNode()->GetValue()->c_str();
		if (leftValue != rightValue) return false;
		if ((leftNodeOptions & kXMP_PropHasLang) != (rightNodeOptions & kXMP_PropHasLang)) return false;
		if (leftNodeOptions & kXMP_PropHasLang) {
			spcINode leftDefaultQualifier = leftNode->GetQualifier( xmlNameSpace.c_str(), xmlNameSpace.size(), "lang", AdobeXMPCommon::npos );
			spcINode rightDefaultQualifier = rightNode->GetQualifier( xmlNameSpace.c_str(), xmlNameSpace.size(), "lang", AdobeXMPCommon::npos );
			if (!leftDefaultQualifier && !rightDefaultQualifier) return true;
			if (!leftDefaultQualifier || !rightDefaultQualifier) return false;

			XMP_VarString leftFirstQualValue = leftDefaultQualifier->ConvertToSimpleNode()->GetValue()->c_str();
			XMP_VarString rightFirstQualValue = rightDefaultQualifier->ConvertToSimpleNode()->GetValue()->c_str();
			if (leftFirstQualValue != rightFirstQualValue) {
				return false;
			}
		}

	}
	else if (leftForm == kXMP_PropValueIsStruct) {

		// Struct nodes, see if all fields match, ignoring order.
		size_t leftNodeChildCount = XMPUtils::GetNodeChildCount(leftNode);
		size_t rightNodeChildCount = XMPUtils::GetNodeChildCount(rightNode);
		if (leftNodeChildCount != rightNodeChildCount) return false;

		auto leftNodeChildIter = XMPUtils::GetNodeChildIterator(leftNode);
		auto rightNodeChildIter = XMPUtils::GetNodeChildIterator(rightNode);
		for (; leftNodeChildIter; leftNodeChildIter = leftNodeChildIter->Next(), rightNodeChildIter = rightNodeChildIter->Next()) {
			spcINode leftField = leftNodeChildIter->GetNode();
			spINode rightField = XMPUtils::FindChildNode( AdobeXMPCore_Int::const_pointer_cast<INode>(rightNode),
				leftField->GetNameSpace()->c_str(), leftField->GetName()->c_str(), kXMP_ExistingOnly, 0);
			if (!rightField || !ItemValuesMatch(leftField, rightField)) {
				return false;
			}

		}

	}
	else {

		// Array nodes, see if the "leftNode" values are present in the "rightNode", ignoring order, duplicates,
		// and extra values in the rightNode-> The rightNode is the destination for AppendProperties.

		XMP_Assert(leftForm & kXMP_PropValueIsArray);
		size_t leftNodeChildCount = XMPUtils::GetNodeChildCount(leftNode);
		size_t rightNodeChildCount = XMPUtils::GetNodeChildCount(rightNode);
		auto leftNodeAsArrayNode = leftNode->ConvertToArrayNode();
		auto rightNodeAsArrayNode = rightNode->ConvertToArrayNode();
		for (size_t leftNum = 1; leftNum <= leftNodeChildCount; ++leftNum) {

			spcINode leftItem = leftNodeAsArrayNode->GetNodeAtIndex(leftNum);
			bool leftItemFound = false;
			for (size_t rightNum = 1; rightNum <= rightNodeChildCount; ++rightNum) {

				spcINode rightItem = rightNodeAsArrayNode->GetNodeAtIndex(rightNum);

				if (ItemValuesMatch(leftItem, rightItem)) {
					leftItemFound = true;
					break;
				}
			}
			if (!leftItemFound) return false;

		}

	}

	return true;	// All of the checks passed.

}	// ItemValuesMatch


static void
AppendSubtree(spcINode sourceNode, spINode &destParent,
const bool mergeCompound, const bool replaceOld, const bool deleteEmpty)
{

	XMP_VarString sourceName = sourceNode->GetName()->c_str();
	XMP_VarString sourceNamespace = sourceNode->GetNameSpace()->c_str();
	
	XMP_VarString destName = destParent->GetName()->c_str();
	XMP_VarString destNamespace = destParent->GetNameSpace()->c_str();
	if (sourceName.find("UserComment") != XMP_VarString::npos) {
		int y = 1;
		y++;
	}
	// Need clone non empty only 
	// to do lang alt append
	// to do lang alt
	size_t destPos = 0;
	spINode destNode = XMPUtils::FindChildNode(destParent, sourceNode->GetName()->c_str(), sourceNode->GetNameSpace()->c_str(), kXMP_ExistingOnly, &destPos);

	bool valueIsEmpty = false;
	XMP_OptionBits sourceNodeOptions = XMPUtils::GetIXMPOptions(sourceNode);
	
	if (sourceNode->GetNodeType() == INode::kNTSimple) {
		valueIsEmpty = sourceNode->ConvertToSimpleNode()->GetValue()->empty();
	}
	else {
		
		valueIsEmpty = XMPUtils::GetNodeChildCount(sourceNode) == 0;
	}
	

	if (valueIsEmpty) {
		if (sourceNode && deleteEmpty) {
			destParent->ConvertToStructureNode()->GetIStructureNode_I()->RemoveNode( sourceNode->GetNameSpace(), sourceNode->GetName() );
		}
		return;	// ! Done, empty values are either ignored or cause deletions.
	}

	if (!destNode) {
		// The one easy case, the destination does not exist.
		destNode = sourceNode->Clone(true, true); // TO DO need to skip empty nodes
		if (!destNode) {
			if (destParent->GetNodeType() == INode::kNTStructure) {
				destParent->ConvertToStructureNode()->GetIStructureNode_I()->RemoveNode( sourceNode->GetNameSpace(), sourceNode->GetName() );
			}
			else if (destParent->GetNodeType() == INode::kNTArray) {
				destParent->ConvertToArrayNode()->RemoveNodeAtIndex( destPos );
			}
			return;
		}
		if (destParent->GetNodeType() == INode::kNTStructure) {
			destParent->ConvertToStructureNode()->InsertNode( destNode );
		}
		else if (destParent->GetNodeType() == INode::kNTArray) {
			destParent->ConvertToArrayNode()->InsertNodeAtIndex( destNode, destPos );
		}
		//	XMP_Assert ( (!destNode) || (! destNode->value.empty()) || (! destNode->children.empty()) );
		return;
	}

	// If we get here we're going to modify an existing property, either replacing or merging.

	XMP_Assert((!valueIsEmpty) && (destNode));

	XMP_OptionBits sourceForm = XMPUtils::GetIXMPOptions(sourceNode) & kXMP_PropCompositeMask;
	XMP_OptionBits destForm = XMPUtils::GetIXMPOptions(destNode) & kXMP_PropCompositeMask;

	bool replaceThis = replaceOld;	// ! Don't modify replaceOld, it gets passed to inner calls.
	if (mergeCompound && (!XMP_PropIsSimple(sourceForm))) replaceThis = false;

	if (replaceThis) {
		if (destNode) {
			destParent->ConvertToStructureNode()->GetIStructureNode_I()->RemoveNode( destNode->GetNameSpace(), destNode->GetName() );
		}
		destNode = sourceNode->Clone(true, true);
		if (!destNode) return;
		if ( destNode->GetParent() != destParent && destParent->GetNodeType() == INode::kNTStructure )
			destParent->ConvertToStructureNode()->AppendNode( destNode );
		else if (destNode->GetParent() != destParent && destParent->GetNodeType() == INode::kNTArray) {
			destParent->ConvertToArrayNode()->InsertNodeAtIndex( destNode, destPos );
		}
		if (!XMP_PropIsSimple(XMPUtils::GetIXMPOptions(destNode)) && !XMPUtils::GetNodeChildCount(destNode)) {
			// Don't keep an empty array or struct. The source might be implicitly empty due to
			// all children being empty. In this case CloneOffspring should skip them.
			if (destParent->GetNodeType() == INode::kNTStructure) {
				destParent->ConvertToStructureNode()->GetIStructureNode_I()->RemoveNode( destNode->GetNameSpace(), destNode->GetName() );
			}
			else if (destParent->GetNodeType() == INode::kNTArray) {
				destParent->ConvertToArrayNode()->RemoveNodeAtIndex( destPos );
			}
		}

		return;

	}

	// From here on are cases for merging arrays or structs.

	if (XMP_PropIsSimple(sourceForm) || (sourceForm != destForm)) return;

	if (sourceForm == kXMP_PropValueIsStruct) {

		auto sourceChildIter = XMPUtils::GetNodeChildIterator(sourceNode);
		for (; sourceChildIter; sourceChildIter = sourceChildIter->Next()) {
			spcINode sourceField = sourceChildIter->GetNode();
			AppendSubtree(sourceField, destNode, mergeCompound, replaceOld, deleteEmpty);
			if (deleteEmpty && !XMPUtils::GetNodeChildCount(destNode)) {
				destParent->ConvertToStructureNode()->GetIStructureNode_I()->RemoveNode( destNode->GetNameSpace(), destNode->GetName() );
			}
		}


	}
	else if (sourceForm & kXMP_PropArrayIsAltText) {

		XMP_Assert(mergeCompound);
		spcIArrayNode sourceArrayNode = sourceNode->ConvertToArrayNode();
		for (size_t sourceNum = 1, sourceLim = XMPUtils::GetNodeChildCount(sourceNode); sourceNum <= sourceLim && destNode; ++sourceNum) {

			spcINode sourceItem = sourceArrayNode->GetNodeAtIndex(sourceNum);
			spcIUTF8String sourceItemValue = sourceItem->ConvertToSimpleNode()->GetValue();
			if (!sourceItem->HasQualifiers()) continue;
			spcINode langQualNode = sourceItem->GetQualifier(xmlNameSpace.c_str(), xmlNameSpace.size(), "lang", AdobeXMPCommon::npos );
			if (!langQualNode || langQualNode->GetNodeType() != INode::kNTSimple) continue;
			XMP_VarString langValue = langQualNode->ConvertToSimpleNode()->GetValue()->c_str();
			XMP_Index destIndex = LookupIXMPLangItem( destNode->ConvertToArrayNode(), langValue );

			if (sourceItemValue->empty()) {

				if (deleteEmpty && (destIndex != -1)) {
					//delete ( destNode->children[destIndex] );
					//destNode->children.erase ( destNode->children.begin() + destIndex );
					destNode->ConvertToArrayNode()->RemoveNodeAtIndex(destIndex);
					if (!XMPUtils::GetNodeChildCount(destNode)) {

						if (destParent->GetNodeType() == INode::kNTStructure) {
							destParent->ConvertToStructureNode()->GetIStructureNode_I()->RemoveNode( destNode->GetNameSpace(), destNode->GetName() );
						}
						else if (destParent->GetNodeType() == INode::kNTArray) {
							destParent->ConvertToArrayNode()->RemoveNodeAtIndex( destPos );
						}
					}
				}

			}
			else {

				if (destIndex != -1) {

					// The source and dest arrays both have this language item.

					if (replaceOld) {
						auto temp = destNode->ConvertToArrayNode()->GetNodeAtIndex( destIndex );
						temp->ConvertToSimpleNode()->SetValue(sourceItemValue->c_str(), sourceItemValue->size() );
					}

				}
				else {

					
					spcISimpleNode firstQualifier = sourceItem->GetSimpleQualifier( xmlNameSpace.c_str(), xmlNameSpace.size(), "lang", AdobeXMPCommon::npos );
					if ((!XMP_LitMatch(firstQualifier->GetValue()->c_str(), "x-default")) || !XMPUtils::GetNodeChildCount(destNode)) {
						
						CloneIXMPSubtree(sourceItem, destNode, true);
					}
					else {
						
						spINode destItem = AdobeXMPCore_Int::ISimpleNode_I::CreateSimpleNode( sourceItem->GetNameSpace(), sourceItem->GetName(), sourceItemValue );
						destNode->ConvertToArrayNode()->InsertNodeAtIndex(destItem, 1);
					}

				}

			}

		}

	}
	
	else if (sourceForm & kXMP_PropValueIsArray)  {
		auto sourceNodeChildIter = XMPUtils::GetNodeChildIterator(sourceNode);
		for (; sourceNodeChildIter; sourceNodeChildIter = sourceNodeChildIter->Next()) {
			spcINode sourceItem = sourceNodeChildIter->GetNode();

			spIArrayNode arrayNode = destNode->ConvertToArrayNode();
			size_t arrayChildCount = arrayNode->ChildCount();
			size_t foundIndex = arrayChildCount + 1;
			for (size_t arrayIdx = 1; arrayIdx <= arrayChildCount; arrayIdx++) {
				spINode destItem = arrayNode->GetNodeAtIndex(arrayIdx);
				//XMP_VarString destValue = INode_I::AdaptConstNodeTo_I<ISimpleNode, INode::kNTSimple>(destItem)->GetValue_I()->c_str();
				if (ItemValuesMatch(destItem, sourceItem)) {
					foundIndex = arrayIdx;
					break;
				}

			}
			if (foundIndex == arrayChildCount + 1) {

				CloneIXMPSubtree(sourceItem, destNode, true);
				
			}

		}


	}

}	// AppendSubtree_v2

#if AdobePrivate
// -------------------------------------------------------------------------------------------------
// AppendProperties_v2
// ----------------

/* class static */ void
XMPUtils::AppendProperties_v2(	const XMPMeta & sourceptr,
								XMPMeta *		 destPtr,
								XMP_OptionBits	 options )
{
	
	if(sUseNewCoreAPIs) {
		const XMPMeta2 & source = dynamic_cast<const XMPMeta2 &> (sourceptr);
		XMPMeta2 * dest = dynamic_cast<XMPMeta2 *>(destPtr);
		XMP_Assert(dest != 0);	// ! Enforced by wrapper.
		auto defaultMap = INameSpacePrefixMap::GetDefaultNameSpacePrefixMap()->GetINameSpacePrefixMap_I();
		

		const bool doAll = ((options & kXMPUtil_DoAllProperties) != 0);
		const bool replaceOld = ((options & kXMPUtil_ReplaceOldValues) != 0);
		const bool deleteEmpty = ((options & kXMPUtil_DeleteEmptyValues) != 0);
		const bool mergeCompound = (!replaceOld);	// ! Old API had implicit Add, superceded by explicit Replace.

		auto topLevelIter = XMPUtils::GetNodeChildIterator(source.mDOM);
		for (; topLevelIter; topLevelIter = topLevelIter->Next()) {
			spcINode topLevelNode = topLevelIter->GetNode();
			XMP_VarString currNameSpace = defaultMap->GetPrefix(topLevelNode->GetNameSpace())->c_str();
			XMP_VarString nodeFullName = currNameSpace + ":" + topLevelNode->GetName()->c_str();
			if (doAll || !IsInternalProperty(topLevelNode->GetNameSpace()->c_str(), nodeFullName)) {
				spINode destRoot = dest->mDOM;
				AppendSubtree(topLevelNode, destRoot, mergeCompound, replaceOld, deleteEmpty);

			}
		}
	}

}	// AppendProperties_v2
#endif //AdobePrivate

// -------------------------------------------------------------------------------------------------
// ApplyTemplate
// -------------

/* class static */ void
XMPUtils::ApplyTemplate_v2( XMPMeta *	      workingXMPBasePtr,
						   const XMPMeta & templateXMPBasePtr,
                           XMP_OptionBits  actions)
{
	XMPMeta2 * workingXMP = dynamic_cast<XMPMeta2 *>(workingXMPBasePtr);
	if (!workingXMPBasePtr) return;
	const XMPMeta2 & templateXMP = dynamic_cast<const XMPMeta2 &> (templateXMPBasePtr); 
	bool doClear = XMP_OptionIsSet(actions, kXMPTemplate_ClearUnnamedProperties);
	bool doAdd = XMP_OptionIsSet(actions, kXMPTemplate_AddNewProperties);
	bool doReplace = XMP_OptionIsSet(actions, kXMPTemplate_ReplaceExistingProperties);

	bool deleteEmpty = XMP_OptionIsSet(actions, kXMPTemplate_ReplaceWithDeleteEmpty);
	doReplace |= deleteEmpty;	// Delete-empty implies Replace.
	deleteEmpty &= (!doClear);	// Clear implies not delete-empty, but keep the implicit Replace. 

	bool doAll = XMP_OptionIsSet(actions, kXMPTemplate_IncludeInternalProperties);

	auto defaultMap = INameSpacePrefixMap::GetDefaultNameSpacePrefixMap()->GetINameSpacePrefixMap_I();

	if (doClear) {

		// Visit the top level working properties, delete if not in the template.
		auto topLevelPropIter = XMPUtils::GetNodeChildIterator(workingXMP->mDOM);
		std::vector<spcINode> propsToBeDeleted; // needed to avoid problems related with deletion and invalid const iterator

		for (; topLevelPropIter; topLevelPropIter = topLevelPropIter->Next()) {
			spcINode topLevelProp = topLevelPropIter->GetNode();
			XMP_VarString currNameSpace = defaultMap->GetPrefix(topLevelProp->GetNameSpace())->c_str();
			XMP_VarString nodeFullName = currNameSpace + ":" + topLevelProp->GetName()->c_str();
			if (doAll || !IsInternalProperty(topLevelProp->GetNameSpace()->c_str(), nodeFullName)) {
				if (!templateXMP.mDOM->GetIStructureNode_I()->GetNode(topLevelProp->GetNameSpace(), topLevelProp->GetName())) {
					propsToBeDeleted.push_back(topLevelProp);
				}
			}

		}
		for (size_t idx = 0; idx < propsToBeDeleted.size(); idx++) {
			workingXMP->mDOM->GetIStructureNode_I()->RemoveNode(propsToBeDeleted[idx]->GetNameSpace(), propsToBeDeleted[idx]->GetName());
		}

	}

	if (doAdd | doReplace) {

		auto templateTopPropIter = XMPUtils::GetNodeChildIterator(templateXMP.mDOM);
		for (; templateTopPropIter; templateTopPropIter = templateTopPropIter->Next()) {
			spcINode currentTemplateTopProp = templateTopPropIter->GetNode();	
			XMP_VarString currNameSpace = defaultMap->GetPrefix(currentTemplateTopProp->GetNameSpace())->c_str();
			XMP_VarString nodeFullName = currNameSpace + ":" + currentTemplateTopProp->GetName()->c_str();
			XMP_ExpandedXPath	expPath;
			ExpandXPath(currentTemplateTopProp->GetNameSpace()->c_str(), nodeFullName.c_str(), &expPath);
			spINode destNode;
			spINode templateXMPRoot = workingXMP->mDOM;
			if (!currentTemplateTopProp) continue;
			if (doAll || !IsInternalProperty(currentTemplateTopProp->GetNameSpace()->c_str(), nodeFullName)){
					AppendSubtree ( currentTemplateTopProp, templateXMPRoot, doAdd, doReplace, deleteEmpty );
			}
		}
	}


}	// ApplyTemplate_v2

// -------------------------------------------------------------------------------------------------
// DuplicateSubtree
// ----------------

static void CloneContents(spINode sourceNode, spINode &destNode) {

	
	if (sourceNode->GetNodeType() == INode::kNTSimple) {

		spISimpleNode sourceSimpleNode = sourceNode->ConvertToSimpleNode();
		spISimpleNode destSimpleNode = AdobeXMPCore_Int::ISimpleNode_I::CreateSimpleNode(destNode->GetNameSpace(), destNode->GetName(), sourceSimpleNode->GetValue() );
		destNode = destSimpleNode;
	}
	else if (sourceNode->GetNodeType() == INode::kNTArray)
	{
		spIArrayNode arraySourceNode = sourceNode->ConvertToArrayNode();
		spIArrayNode destArrayNode = AdobeXMPCore_Int::IArrayNode_I::CreateArrayNode(destNode->GetNameSpace(), destNode->GetName(), arraySourceNode->GetArrayForm());
		destNode = destArrayNode;
		for (auto childIter = arraySourceNode->Iterator(); childIter; childIter = childIter->Next()) {

			spINode childCloned = childIter->GetNode()->Clone();
			destArrayNode->AppendNode(childCloned);
		}

		destNode = destArrayNode;

	}
	else {

		spIStructureNode arraySourceNode = sourceNode->ConvertToStructureNode();
		
		spIStructureNode destArrayNode = destNode->ConvertToStructureNode();
		for (auto childIter = arraySourceNode->Iterator(); childIter; childIter = childIter->Next()) {

			spINode childCloned = childIter->GetNode()->Clone();
			destArrayNode->AppendNode(childCloned);
		}

		destNode = destArrayNode;

	}

	if (sourceNode->HasQualifiers()) {


		for (auto qualIter = sourceNode->QualifiersIterator(); qualIter; qualIter = qualIter->Next()) {

			spINode clonedQual = qualIter->GetNode()->Clone();
			destNode->InsertQualifier(clonedQual);
		}
	}
}

/* class static */ void
XMPUtils::DuplicateSubtree_v2(const XMPMeta & sourceBasePtr,
XMPMeta *		 destBasePtr,
XMP_StringPtr	 sourceNS,
XMP_StringPtr	 sourceRoot,
XMP_StringPtr	 destNS,
XMP_StringPtr	 destRoot,
XMP_OptionBits	 options)
{

	XMPMeta2 * dest = dynamic_cast<XMPMeta2 *>(destBasePtr);

	if (!dest) return;
	const XMPMeta2 & source = dynamic_cast<const XMPMeta2 &> (sourceBasePtr);

	IgnoreParam(options);
	// TODO : Use of testnode == soucenode seems slightly dodgy, verify if it works in null case, simple case, subtree case
	bool fullSourceTree = false;
	bool fullDestTree = false;

	XMP_ExpandedXPath sourcePath, destPath;

	spINode sourceNode;
	spINode destNode;

	XMP_Assert((sourceNS != 0) && (*sourceNS != 0));
	XMP_Assert((sourceRoot != 0) && (*sourceRoot != 0));
	XMP_Assert((dest != 0) && (destNS != 0) && (destRoot != 0));

	if (*destNS == 0)	  destNS = sourceNS;
	if (*destRoot == 0) destRoot = sourceRoot;

	if (XMP_LitMatch(sourceNS, "*")) fullSourceTree = true;
	if (XMP_LitMatch(destNS, "*"))   fullDestTree = true;

	if ((&source == dest) && (fullSourceTree | fullDestTree)) {
		XMP_Throw("Can't duplicate tree onto itself", kXMPErr_BadParam);
	}

	if (fullSourceTree & fullDestTree) XMP_Throw("Use Clone for full tree to full tree", kXMPErr_BadParam);

	if (fullSourceTree) {

		// The destination must be an existing empty struct, copy all of the source top level as fields.

		ExpandXPath(destNS, destRoot, &destPath);

		XMP_OptionBits destOptions = 0;
		if (!XMPUtils::FindCnstNode(dest->mDOM, destPath, destNode, &destOptions)) {
			XMP_Throw("Destination must be an existing struct", kXMPErr_BadXPath);
		}
		if (!XMP_PropIsStruct(destOptions)) {
			XMP_Throw("Destination must be an existing struct", kXMPErr_BadXPath);
		}

		if (XMPUtils::GetNodeChildCount(destNode)) {
			if (options & kXMP_DeleteExisting) {
				destNode->Clear();
			}
			else {
				XMP_Throw("Destination must be an empty struct", kXMPErr_BadXPath);
			}
		}
		sourceNode = source.mDOM;
		CloneContents(sourceNode, destNode);

	}
	else if (fullDestTree) {

		// The source node must be an existing struct, copy all of the fields to the dest top level.

		XMP_ExpandedXPath srcPath;
		ExpandXPath(sourceNS, sourceRoot, &srcPath);
		spINode spSourceNode;
		XMP_OptionBits sourceNodeOptions = 0;
		XMPUtils::FindCnstNode(source.mDOM, srcPath, spSourceNode, &sourceNodeOptions);

		if ((!spSourceNode) || (!XMP_PropIsStruct(sourceNodeOptions))) {
			XMP_Throw("Source must be an existing struct", kXMPErr_BadXPath);
		}

		destNode = dest->mDOM;

		if (XMPUtils::GetNodeChildCount(destNode)) {
			if (options & kXMP_DeleteExisting) {
				destNode->Clear();
			}
			else {
				XMP_Throw("Destination tree must be empty", kXMPErr_BadXPath);
			}
		}


		for (auto sourceChildIter = XMPUtils::GetNodeChildIterator(spSourceNode); sourceChildIter; sourceChildIter = sourceChildIter->Next()) {

			spINode copyNode = sourceChildIter->GetNode()->Clone();
			if (destNode->GetNodeType() == INode::kNTStructure){
				destNode->ConvertToStructureNode()->AppendNode(copyNode);
			}

		}
	}
	else {

		// Find the root nodes for the source and destination subtrees.

		ExpandXPath(sourceNS, sourceRoot, &sourcePath);
		ExpandXPath(destNS, destRoot, &destPath);
		spINode destNodeCopy;
		if (!XMPUtils::FindCnstNode(source.mDOM, sourcePath, sourceNode)) {
			XMP_Throw("Can't find source subtree", kXMPErr_BadXPath);
		}
		if (XMPUtils::FindCnstNode(dest->mDOM, destPath, destNode)) {
			XMP_Throw("Destination subtree must not exist", kXMPErr_BadXPath);
		}


		if (!XMPUtils::FindNode(dest->mDOM, destPath, kXMP_CreateNodes, 0, destNode))  {	// Now create the dest.
			XMP_Throw("Can't create destination root node", kXMPErr_BadXPath);
		}

		// Make sure the destination is not within the source! The source can't be inside the destination
		// because the source already existed and the destination was just created.

		if (&source == dest) {
			for (spINode testNode = destNode; testNode; testNode = testNode->GetParent()){
				if (testNode.get() == sourceNode.get()) {

					XMP_Throw("Destination subtree is within the source subtree", kXMPErr_BadXPath);
				}
			}
		}

		// *** Could use a CloneTree util here and maybe elsewhere.
		//destNode = sourceNode->Clone();
		
		if (sourceNode->GetNodeType() == INode::kNTSimple) {

			spISimpleNode sourceSimpleNode = sourceNode->ConvertToSimpleNode();
			spISimpleNode destSimpleNode = AdobeXMPCore_Int::ISimpleNode_I::CreateSimpleNode(destNode->GetNameSpace(), destNode->GetName(),
				sourceSimpleNode->GetValue() );
			destNodeCopy = destSimpleNode;
		}
		else if (sourceNode->GetNodeType() == INode::kNTArray)
		{	
			spIArrayNode arraySourceNode = sourceNode->ConvertToArrayNode();
			spIArrayNode destArrayNode = AdobeXMPCore_Int::IArrayNode_I::CreateArrayNode(destNode->GetNameSpace(), destNode->GetName(), arraySourceNode->GetArrayForm());
			destNodeCopy = destArrayNode;
			for (auto childIter = arraySourceNode->Iterator(); childIter; childIter = childIter->Next()) {

				spINode childCloned = childIter->GetNode()->Clone();
				destArrayNode->AppendNode(childCloned);
			}

			destNodeCopy = destArrayNode;
			
		}
		else {

			spIStructureNode arraySourceNode = sourceNode->ConvertToStructureNode();
			spIStructureNode destArrayNode = AdobeXMPCore_Int::IStructureNode_I::CreateStructureNode(destNode->GetNameSpace(), destNode->GetName());
			destNodeCopy = destArrayNode;
			for (auto childIter = arraySourceNode->Iterator(); childIter; childIter = childIter->Next()) {

				spINode childCloned = childIter->GetNode()->Clone();
				destArrayNode->AppendNode(childCloned);
			}

			destNodeCopy = destArrayNode;

		}

		if (sourceNode->HasQualifiers()) {


			for (auto qualIter = sourceNode->QualifiersIterator(); qualIter; qualIter = qualIter->Next()) {

				spINode clonedQual = qualIter->GetNode()->Clone();
				destNodeCopy->InsertQualifier(clonedQual);
			}
		}
		
		dest->mDOM->ReplaceNode(destNodeCopy);
	}

}	// DuplicateSubtree_v2

#if AdobePrivate
// -------------------------------------------------------------------------------------------------
// CollectMultiFileXMP
// -------------------

/* class static */ void
XMPUtils::CollectMultiFileXMP_v2(const XMPMeta & inputXMPRef,
								XMPMeta *       multiXMPPtr,
								XMP_OptionBits  options)
{
	if(sUseNewCoreAPIs) {
		const XMPMeta2 & inputXMP = dynamic_cast<const XMPMeta2&>(inputXMPRef);
		XMPMeta2 * multiXMP = dynamic_cast<XMPMeta2 *>(multiXMPPtr);

		IgnoreParam(options);

		bool transientSchemaExists = false;
		for (auto propIter = multiXMP->mDOM->Iterator(); propIter; propIter = propIter->Next()) {
			spINode propNode = propIter->GetNode();
			if (XMP_LitMatch(propNode->GetNameSpace()->c_str(), kXMP_NS_Transient)) {
				//CloneIXMPSubtree( propNode, multiXMP->mDOM,false);
				transientSchemaExists = true;
				break;
			}
		}
		if (!transientSchemaExists) {

			if (XMPUtils::GetNodeChildCount(multiXMP->mDOM)) XMP_Throw("Multi XMP must be empty at first", kXMPErr_BadParam);
			spcIUTF8String aboutURI = multiXMP->mDOM->GetAboutURI();
			spINode clonedCopy = inputXMP.mDOM->Clone();
			multiXMP->mDOM = clonedCopy->ConvertToMetadata();
			multiXMP->mDOM->SetAboutURI(aboutURI->c_str(), aboutURI->size());
			multiXMP->SetProperty(kXMP_NS_Transient, "MultiFileCount", "1", kXMP_NoOptions);
			multiXMP->SetProperty(kXMP_NS_Transient, "DifferingProperties", 0, kXMP_PropValueIsArray);

		}
		else {

			// This is a later call, add the input XMP with checking for differing properties.

			XMP_Index fileCount;
			XMP_OptionBits voidOptions;
			bool found = multiXMP->GetProperty_Int(kXMP_NS_Transient, "MultiFileCount", &fileCount, &voidOptions);
			XMP_Assert(found);
			multiXMP->SetProperty_Int(kXMP_NS_Transient, "MultiFileCount", fileCount + 1, kXMP_NoOptions);
#if TraceMultiFile
			fflush(stdout); printf ( "\nCollectMultiFileXMP adding file #%d\n", fileCount+1 ); fflush(stdout);
#endif

			for (auto inputPropIter = XMPUtils::GetNodeChildIterator(inputXMP.mDOM); inputPropIter; inputPropIter = inputPropIter->Next()) {
				spINode inputPropNode = AdobeXMPCore_Int::const_pointer_cast<INode>(inputPropIter->GetNode());
				XMP_Assert(!XMP_LitMatch(inputPropNode->GetNameSpace()->c_str(), kXMP_NS_Transient));
				spINode multiPropNode = multiXMP->mDOM->GetIStructureNode_I()->GetNode(inputPropNode->GetNameSpace(), inputPropNode->GetName());
				if (!multiPropNode) {

					NoteDifferingProperty_v2(multiXMP->mDOM, spINode(), inputPropNode);
				}
				// if subtrees not equal
				else if (!CompareSubtrees(inputPropNode, multiPropNode)){
					NoteDifferingProperty_v2(multiXMP->mDOM, multiPropNode, inputPropNode);
					multiXMP->mDOM->GetIStructureNode_I()->RemoveNode(inputPropNode->GetNameSpace(), inputPropNode->GetName());
				}

			}
			std::vector<spINode> nodesToBeDeleted;

			for (auto multiPropIter = multiXMP->mDOM->Iterator(); multiPropIter; multiPropIter = multiPropIter->Next()) {

				spINode multiPropNode = multiPropIter->GetNode();
				if (XMP_LitMatch(multiPropNode->GetNameSpace()->c_str(), kXMP_NS_Transient)) continue;
				spINode inputPropNode = inputXMP.mDOM->GetIStructureNode_I()->RemoveNode(multiPropNode->GetNameSpace(), multiPropNode->GetName());
				if (!inputPropNode) {
					//NoteDifferingProperty ( xmpxSchema, multiProp, 0 );
					NoteDifferingProperty_v2(multiXMP->mDOM, multiPropNode, spINode());
					nodesToBeDeleted.push_back(multiPropNode);
				}

			}
			for (size_t idx = 0; idx < nodesToBeDeleted.size(); ++idx) {

				multiXMP->mDOM->GetIStructureNode_I()->RemoveNode(nodesToBeDeleted[idx]->GetNameSpace(), nodesToBeDeleted[idx]->GetName());

			}

		}
	}
	
}	// CollectMultiFileXMP

// -------------------------------------------------------------------------------------------------
// DistributeMultiFileXMP
// ----------------------

/* class static */ void
XMPUtils::DistributeMultiFileXMP_v2(const XMPMeta & multiXMPRef,
                                   XMPMeta *       outputXMPPtr,
                                   XMP_OptionBits  options)
{
	if(sUseNewCoreAPIs) {
		const XMPMeta2 & multiXMP = dynamic_cast<const XMPMeta2 &> (multiXMPRef);
		XMPMeta2 * outputXMP = dynamic_cast<XMPMeta2 *>(outputXMPPtr);
		IgnoreParam(options);

		// Delete the properties that, well, need deleting.
		spcINode delArray = multiXMP.mDOM->GetNode(kXMP_NS_Transient, AdobeXMPCommon::npos, "DeletedProperties", AdobeXMPCommon::npos );
		if (delArray) {
			for (size_t propNum = 1, propLim = XMPUtils::GetNodeChildCount(delArray); propNum <= propLim; ++propNum) {
				spcINode delItem = delArray->ConvertToArrayNode()->GetNodeAtIndex(propNum);
				spcINode delURI = delItem->ConvertToStructureNode()->GetNode( kXMP_NS_Transient, AdobeXMPCommon::npos, "DelURI", AdobeXMPCommon::npos );
				spcINode delPath = delItem->ConvertToStructureNode()->GetNode( kXMP_NS_Transient, AdobeXMPCommon::npos, "DelPath", AdobeXMPCommon::npos );
				if (!delURI || !delPath) XMP_Throw("Invalid DeletedProperties entry", kXMPErr_BadXMP);
				outputXMP->DeleteProperty(delURI->ConvertToSimpleNode()->GetValue()->c_str(), delPath->ConvertToSimpleNode()->GetValue()->c_str());

			}
		}

		for (auto propIter = multiXMP.mDOM->Iterator(); propIter; propIter = propIter->Next()) {

			spINode propNode = propIter->GetNode();
			if (XMP_LitMatch(propNode->GetNameSpace()->c_str(), kXMP_NS_Transient)) continue;
			spINode outputRoot = outputXMP->mDOM;
			AppendSubtree(propNode, outputRoot, false, true, false);
		}
		// Update the normal (non-transient) properties, replacing any existing value.
	}
	

}	// DistributeMultiFileXMP

// -------------------------------------------------------------------------------------------------
// IsPropertyMultiValued
// ---------------------

// *** Depends on the prefix for the transient namespace!

/* class static */ bool
XMPUtils::IsPropertyMultiValued_v2(const XMPMeta & multiXMPRef,
                                  XMP_StringPtr   propNS,
                                  XMP_StringPtr   propName)

{
	if(sUseNewCoreAPIs) {

		const XMPMeta2 & multiXMP = dynamic_cast<const XMPMeta2 &> (multiXMPRef);
		spINode dpArray = multiXMP.mDOM->GetNode( kXMP_NS_Transient, AdobeXMPCommon::npos, "DifferingProperties", AdobeXMPCommon::npos );
		if (!dpArray) return false;

		XMP_ExpandedXPath expPath;
		ExpandXPath(propNS, propName, &expPath);		// *** Should have LookupBase utility.
		XMP_StringPtr baseProp = expPath[kRootPropStep].step.c_str();
		if (expPath[kRootPropStep].options & kXMP_StepIsAlias) {
			XMP_AliasMapPos aliasPos = sRegisteredAliasMap->find(expPath[kRootPropStep].step);
			XMP_Assert(aliasPos != sRegisteredAliasMap->end());
			baseProp = aliasPos->second[kRootPropStep].step.c_str();
		}
		XMP_Index dpIndex = LookupFieldSelector_v2( dpArray->ConvertToArrayNode(), "DiffPath", baseProp);
		if (dpIndex == -1) return false;
		return true;
	}
	else {
		return false;
	}

}	// IsPropertyMultiValued

// -------------------------------------------------------------------------------------------------
// GetDateRange
// ------------

// *** Depends on the prefix for the transient namespace!

/* class static */ bool
XMPUtils::GetDateRange_v2(const XMPMeta & multiXMPRef,
                         XMP_StringPtr   propNS,
                         XMP_StringPtr   propName,
                         XMP_DateTime *  oldest,
                         XMP_DateTime *  newest)
{
	if(sUseNewCoreAPIs) {
		const XMPMeta2 & multiXMP = dynamic_cast<const XMPMeta2 &> (multiXMPRef);
		spINode dpArray = multiXMP.mDOM->GetNode( kXMP_NS_Transient, AdobeXMPCommon::npos, "DifferingProperties", AdobeXMPCommon::npos );
		if (!dpArray) return false;

		XMP_ExpandedXPath expPath;
		ExpandXPath(propNS, propName, &expPath);
		if (expPath.size() > 2) return false;	// Only top level properties are tracked.
		XMP_StringPtr baseProp = expPath[kRootPropStep].step.c_str();
		if (expPath[kRootPropStep].options & kXMP_StepIsAlias) {
			XMP_AliasMapPos aliasPos = sRegisteredAliasMap->find(expPath[kRootPropStep].step);
			XMP_Assert(aliasPos != sRegisteredAliasMap->end());
			baseProp = aliasPos->second[kRootPropStep].step.c_str();
		}
		XMP_Index propIndex = LookupFieldSelector_v2( dpArray->ConvertToArrayNode(), "DiffPath", baseProp);
		if (propIndex == -1) return false;

		spINode dpItem = dpArray->ConvertToArrayNode()->GetNodeAtIndex(propIndex);
		spINode oldestNode = FindChildNode(dpItem, "DiffOldest", kXMP_NS_Transient, false, 0);
		if (!oldestNode) return false;
		spINode newestNode = FindChildNode(dpItem, "DiffNewest", kXMP_NS_Transient, false, 0);
		if (!newestNode) XMP_Throw("Missing xmpx:DiffNewest property", kXMPErr_BadXMP);
		XMPUtils::ConvertToDate( oldestNode->ConvertToSimpleNode()->GetValue()->c_str(), oldest);
		XMPUtils::ConvertToDate( newestNode->ConvertToSimpleNode()->GetValue()->c_str(), newest);
		return true;
	}
	else  {
		return true;
	}
}	// GetDateRange

// -------------------------------------------------------------------------------------------------
// GetMergedListPath
// -----------------

// *** Depends on the prefix for the transient namespace!

/* class static */ bool
XMPUtils::GetMergedListPath_v2(const XMPMeta & multiXMPRef,
                              XMP_StringPtr   propNS,
                              XMP_StringPtr   propName,
                              XMP_VarString * pathStr)
{

	if(sUseNewCoreAPIs) {

		const XMPMeta2 & multiXMP = dynamic_cast<const XMPMeta2 &> (multiXMPRef);
		pathStr->erase();

		spINode dpArray = multiXMP.mDOM->GetNode( kXMP_NS_Transient, AdobeXMPCommon::npos, "DifferingProperties", AdobeXMPCommon::npos );
		if (!dpArray) return false;
		XMP_ExpandedXPath expPath;
		ExpandXPath(propNS, propName, &expPath);
		if (expPath.size() > 2) return false;	// Only top level properties are tracked.
		XMP_StringPtr baseProp = expPath[kRootPropStep].step.c_str();
		if (expPath[kRootPropStep].options & kXMP_StepIsAlias) {
			XMP_AliasMapPos aliasPos = sRegisteredAliasMap->find(expPath[kRootPropStep].step);
			XMP_Assert(aliasPos != sRegisteredAliasMap->end());
			baseProp = aliasPos->second[kRootPropStep].step.c_str();
		}
		spcINode   oldestNode, newestNode;
		XMP_Index propIndex = LookupFieldSelector_v2( dpArray->ConvertToArrayNode(), "DiffPath", baseProp);
		if (propIndex == -1) return false;
		spINode dpItem = dpArray->ConvertToArrayNode()->GetNodeAtIndex(propIndex);
		spINode mergedList = FindChildNode(dpItem, "DiffMergedList", kXMP_NS_Transient, false, 0);
		if (!mergedList) return false;
		
		XMPUtils::ComposeArrayItemPath(kXMP_NS_Transient, "DifferingProperties", propIndex, pathStr);
		XMPUtils::ComposeStructFieldPath(kXMP_NS_Transient, pathStr->c_str(), kXMP_NS_Transient, "DiffMergedList", pathStr);
		return true;
	}
	else {
		return false;
	}

}	// GetMergedListPath

// -------------------------------------------------------------------------------------------------
// RemoveMultiValueInfo
// --------------------

// *** Depends on the prefix for the transient namespace!

/* class static */ void
XMPUtils::RemoveMultiValueInfo_v2(XMPMeta *     multiXMPPtr,
                                 XMP_StringPtr propNS,
                                 XMP_StringPtr propName)
{

	XMPMeta2 *     multiXMP = dynamic_cast<XMPMeta2 *>(multiXMPPtr);
	if (multiXMP) {
		spINode diffArray = multiXMP->mDOM->GetNode( kXMP_NS_Transient, AdobeXMPCommon::npos, "DifferingProperties", AdobeXMPCommon::npos );
		spINode delArray = multiXMP->mDOM->GetNode(kXMP_NS_Transient, AdobeXMPCommon::npos, "DeletedProperties", AdobeXMPCommon::npos );


		if (*propName == 0) {

			// Remove the info for all properties in the given schema.

			if (diffArray) {
				for (size_t itemNum = 1; itemNum <= XMPUtils::GetNodeChildCount(diffArray); itemNum++){

					spINode  diffItem = diffArray->ConvertToArrayNode()->GetNodeAtIndex(itemNum);
					spINode diffSchema = diffItem->ConvertToStructureNode()->GetNode( kXMP_NS_Transient, AdobeXMPCommon::npos, "DiffURI", AdobeXMPCommon::npos );
					XMP_Assert(diffSchema != libcppNULL && diffSchema->GetNodeType() == INode::kNTSimple);
					if (!strcmp(propNS, diffSchema->ConvertToSimpleNode()->GetValue()->c_str())){

						diffArray->ConvertToArrayNode()->RemoveNodeAtIndex(itemNum);
						--itemNum;
					}
				}
			}

			if (delArray) {
				for (size_t itemNum = 1; itemNum <= XMPUtils::GetNodeChildCount(delArray); itemNum++) {

					spINode delItem = delArray->ConvertToArrayNode()->GetNodeAtIndex(itemNum);
					spINode delURI = delItem->ConvertToStructureNode()->GetNode( kXMP_NS_Transient, AdobeXMPCommon::npos, "DelURI", AdobeXMPCommon::npos );
					XMP_Assert(delURI && delURI->GetNodeType() == INode::kNTSimple);
					if (!strcmp( delURI->ConvertToSimpleNode()->GetValue()->c_str(), propNS)) {

						diffArray->ConvertToArrayNode()->RemoveNodeAtIndex(itemNum);
						--itemNum;
					}
				}
			}

		}
		else {

			// Remove the info for the named property.

			XMP_ExpandedXPath expPath;
			ExpandXPath(propNS, propName, &expPath);		// *** Should have LookupBase utility.
		
			XMP_StringPtr baseProp = expPath[kRootPropStep].step.c_str();

			if (expPath[kRootPropStep].options & kXMP_StepIsAlias) {
				XMP_AliasMapPos aliasPos = sRegisteredAliasMap->find(expPath[kRootPropStep].step);
				XMP_Assert(aliasPos != sRegisteredAliasMap->end());
				baseProp = aliasPos->second[kRootPropStep].step.c_str();
			}

			if (diffArray) {

				for (size_t itemNum = 1; itemNum <= XMPUtils::GetNodeChildCount(diffArray); itemNum++) {
					spINode  diffItem = diffArray->ConvertToArrayNode()->GetNodeAtIndex(itemNum);
					spINode diffPath = diffItem->ConvertToStructureNode()->GetNode( kXMP_NS_Transient, AdobeXMPCommon::npos, "DiffPath", AdobeXMPCommon::npos );
					XMP_Assert(diffPath);
					if (IsPathPrefix( diffPath->ConvertToSimpleNode()->GetValue()->c_str(), baseProp)) {
						diffArray->ConvertToArrayNode()->RemoveNodeAtIndex( itemNum );
					}
				}
			}
			if (delArray) {
				for (size_t itemNum = 1; itemNum <= XMPUtils::GetNodeChildCount(delArray); itemNum++) {

					spINode delItem = delArray->ConvertToArrayNode()->GetNodeAtIndex(itemNum);
					spINode delURI = delItem->ConvertToStructureNode()->GetNode( kXMP_NS_Transient, AdobeXMPCommon::npos, "DelPath", AdobeXMPCommon::npos );
					XMP_Assert(delURI && delURI->GetNodeType() == INode::kNTSimple);
					if (IsPathPrefix( delURI->ConvertToSimpleNode()->GetValue()->c_str(), baseProp)) {
						diffArray->ConvertToArrayNode()->RemoveNodeAtIndex(itemNum);
					}
				}
			}
		}
	}

}	// RemoveMultiValueInfo

#endif //AdobePrivate
// -------------------------------------------------------------------------------------------------
// RemoveProperties
// ----------------

/* class static */ void
XMPUtils::RemoveProperties_v2(XMPMeta *		xmpMetaPtr,
                             XMP_StringPtr	schemaNS,
                             XMP_StringPtr	propName,
                             XMP_OptionBits options)
{
	using namespace AdobeXMPCommon;
	using namespace AdobeXMPCore;
	XMPMeta2 * xmpObj = dynamic_cast<XMPMeta2 *> (xmpMetaPtr);
	if (!xmpObj) {
		return;
	}
	// Handle aliases in remove properties
	XMP_Assert((schemaNS != 0) && (propName != 0));	// ! Enforced by wrapper.

	const bool doAll = XMP_TestOption(options, kXMPUtil_DoAllProperties);
	const bool includeAliases = XMP_TestOption(options, kXMPUtil_IncludeAliases);

	if (*propName != 0) {

		// Remove just the one indicated property. This might be an alias, the named schema might
		// not actually exist. So don't lookup the schema node.

		if (*schemaNS == 0) XMP_Throw("Property name requires schema namespace", kXMPErr_BadParam);

		XMP_ExpandedXPath expPath;
		
		ExpandXPath(schemaNS, propName, &expPath);
		XMP_Index propIndex = 0;
		spINode propNode;
		if (XMPUtils::FindNode(xmpObj->mDOM, expPath, kXMP_ExistingOnly, kXMP_NoOptions, propNode, &propIndex)) {
			if (doAll || !IsInternalProperty(expPath[kSchemaStep].step, expPath[kRootPropStep].step)) {
				spINode parentNode = propNode->GetParent();	// *** Should have XMP_Node::RemoveChild(pos).

				if (parentNode->GetNodeType() == INode::kNTStructure) {
					parentNode->ConvertToStructureNode()->GetIStructureNode_I()->RemoveNode( propNode->GetNameSpace(), propNode->GetName() );
				}
				else if (parentNode->GetNodeType() == INode::kNTArray) {
					parentNode->ConvertToArrayNode()->RemoveNodeAtIndex(propIndex);
				}
			}
		}

	}
	else if (*schemaNS != 0) {
		std::vector<spINode> topLevelNodesToBeDeleted;
		for (auto topLevelIter = xmpObj->mDOM->Iterator(); topLevelIter; topLevelIter = topLevelIter->Next()) {
			spINode topLevelProp = topLevelIter->GetNode();
			if (XMP_LitMatch(topLevelProp->GetNameSpace()->c_str(), schemaNS)) {
				topLevelNodesToBeDeleted.push_back(topLevelProp);
			}
		}
		for (size_t propIdx = 0; propIdx < topLevelNodesToBeDeleted.size(); ++propIdx) {

			xmpObj->mDOM->GetIStructureNode_I()->RemoveNode(topLevelNodesToBeDeleted[propIdx]->GetNameSpace(), topLevelNodesToBeDeleted[propIdx]->GetName());
		}
		if (includeAliases) {
			// Removing Aliases
			XMP_StringPtr nsPrefix;
			XMP_StringLen nsLen;
			(void)XMPMeta::GetNamespacePrefix(schemaNS, &nsPrefix, &nsLen);

			XMP_AliasMapPos currAlias = sRegisteredAliasMap->begin();
			XMP_AliasMapPos endAlias = sRegisteredAliasMap->end();

			for (; currAlias != endAlias; ++currAlias) {
				if (strncmp(currAlias->first.c_str(), nsPrefix, nsLen) == 0) {
					spINode destNode;
					XMP_Index actualPos = 0;
					size_t colonPos = currAlias->first.find_first_of(":");

					xmpObj->mDOM->RemoveNode( schemaNS, AdobeXMPCommon::npos, currAlias->first.substr( colonPos + 1 ).c_str(), AdobeXMPCommon::npos );
					/*if (!XMPUtils::FindCnstNode(xmpObj->mDOM, currAlias->second, destNode, 0, &actualPos)) continue;
					if (!destNode) continue;
					spINode rootProp = destNode;
					while (rootProp && rootProp->GetParent() != xmpObj->mDOM) {
						rootProp = rootProp->GetParent();
					}
					if (doAll || !IsInternalProperty(rootProp->GetNameSpace()->c_str(), rootProp->GetName()->c_str())) {
						spINode parentNode = destNode->GetParent();
						if (destNode->IsArrayItem()){
							INode_I::AdaptNodeTo_I<IArrayNode, INode::kNTArray>(parentNode)->RemoveNode_I(actualPos);
						}
						else {
							INode_I::AdaptNodeTo_I<IStructureNode, INode::kNTStructure>(parentNode)->RemoveNode_I(destNode->GetNameSpace()->c_str(), destNode->GetName()->c_str());
						}
					}*/

				}
			}

		}

	}
	else {

		xmpObj->mDOM->Clear();
	}

}	// RemoveProperties_v2
#endif

