 //Copyright 2007 Adobe
 //All Rights Reserved.

 //NOTICE:  Adobe permits you to use, modify, and distribute this file in accordance with the terms
 //of the Adobe license agreement accompanying it. If you have received this file from a source other 
// than Adobe, then your use, modification, or distribution of it requires the prior written permission
// of Adobe.
 //=================================================================================================

#if AdobePrivate
 //=================================================================================================
 //Change history
 //==============

 //Writers:
 //AJ Abhishek Jindal

//mm-dd-yy who Description of changes, most recent on top.
// 02-06-15 AJ  5.6-c037 Fixing warnings due to implicit typecasting
// 12-21-14 AJ  5.6-c031 Routed XMPDocOps to new DOM APIs.
//
//=================================================================================================
#endif // AdobePrivate

#include "XMPCore/source/XMPDocOps-Utils.hpp"
#include "XMPCore/source/XMPUtils.hpp"
#if AdobePrivate
#include "XMPCore/XMPCoreDefines_I.h"
#if ENABLE_CPP_DOM_MODEL
#include "XMPCore/Interfaces/IMetadata_I.h"
#include "XMPCore/Interfaces/IArrayNode_I.h"
#include "XMPCore/Interfaces/ISimpleNode_I.h"
#include "XMPCommon/Interfaces/IUTF8String_I.h"


using namespace AdobeXMPCore_Int;

XMP_Int32 XMPDocOpsUtils::PruneOneRedundantRun_v2(const spIArrayNode & historyNode, XMP_Int32 endIndex) {
	spINode finalAction = XMPUtils::FindChildNode(historyNode->GetNodeAtIndex(endIndex + 1), "action", kXMP_NS_XMP_ResourceEvent, false, 0);
	if ( !finalAction || finalAction->GetNodeType() != INode::kNTSimple || strcmp( "saved", finalAction->ConvertToSimpleNode()->GetValue()->c_str() ) )
		return endIndex - 1;
	spINode finalParts = XMPUtils::FindChildNode(historyNode->GetNodeAtIndex(endIndex + 1), "changed", kXMP_NS_XMP_ResourceEvent, false, 0);
	if ( !finalParts || finalParts->GetNodeType() != INode::kNTSimple || finalParts->ConvertToSimpleNode()->GetValue()->empty() )
		return endIndex - 1;

	XMP_Int32 runLength = 1;

	for (XMP_Int32 item = endIndex - 1 ; item >= 0; --item) {	// ! Need a signed loop index!
		spINode currAction = XMPUtils::FindChildNode(historyNode->GetNodeAtIndex(item + 1), "action", kXMP_NS_XMP_ResourceEvent, false, 0);
		if (!currAction || currAction->GetNodeType() != INode::kNTSimple || strcmp("saved", finalAction->ConvertToSimpleNode()->GetValue()->c_str())) break;
		spINode currParts = XMPUtils::FindChildNode(historyNode->GetNodeAtIndex( item + 1), "changed", kXMP_NS_XMP_ResourceEvent, false, 0);
		if (!currParts || currParts->GetNodeType() != INode::kNTSimple || strcmp( currParts->ConvertToSimpleNode()->GetValue()->c_str(), finalParts->ConvertToSimpleNode()->GetValue()->c_str())) break;

		++runLength;
	}

	if (runLength >= 3) {

		XMP_Int32 deleteStart = endIndex - (runLength - 2);
		XMP_Int32 deleteEnd = endIndex - 1;

		for (XMP_Int32 item = deleteEnd; item >= deleteStart; --item) {

			historyNode->RemoveNodeAtIndex(item + 1);
		}
	}

	return endIndex - runLength;

}	// PruneOneRedundantRun

bool XMPDocOpsUtils::IsPartInList_v2(XMP_StringPtr * newPartsPtr, XMP_Index newPartsCount, const spINode & oldPartsNode)
{
	XMP_Assert((newPartsPtr != 0) || (newPartsCount == 0));

	if (!newPartsCount || oldPartsNode->GetNodeType() != INode::kNTSimple) return false;
	spcIUTF8String oldPartNodeValue = oldPartsNode->ConvertToSimpleNode()->GetValue();
	if (XMP_LitMatch(oldPartNodeValue->c_str(), "/")) return true;	// Common special case of "all changed".

	std::string newPart, oldPart;

	for (; newPartsCount > 0; --newPartsCount, ++newPartsPtr) {

		newPart = *newPartsPtr;
			NormalizePartPath ( newPart );
		size_t newLen = newPart.size();
		XMP_Assert(newPart[0] == '/');

		if (newLen == 1) return true;	// The newPart must be "/".

		XMP_StringPtr nextPart = oldPartNodeValue->c_str();
		XMP_StringPtr limit = nextPart + oldPartNodeValue->size();

		while (nextPart < limit) {

			XMP_StringPtr oldPtr = nextPart;
			while ((nextPart < limit) && (*nextPart != ';')) ++nextPart;
			size_t oldLen = nextPart - oldPtr;	// ! Do this before following increment.
			if (nextPart < limit) ++nextPart;			// Move past the ';'.
			if (oldLen == 0) continue;				// ! Do this after the above increment.

			oldPart.assign(oldPtr, oldLen);
				NormalizePartPath ( oldPart );
			oldLen = oldPart.size();

			if (newLen == oldLen) {
				// Do the parts match?
				if (newPart == oldPart) return true;
			}
			else if (newLen > oldLen) {
			//	 Is newPart a descendant of oldPart?  (Must use bigger.compare!)
				if ((newPart[oldLen] == '/') && (newPart.compare(0, oldLen, oldPart) == 0)) return true;
			}
			else {	// newLen < oldLen
			//	 Is newPart an ancestor of oldPart?  (Must use bigger.compare!)
				if ((oldPart[newLen] == '/') && (oldPart.compare(0, newLen, newPart) == 0)) return true;
			}

		}

	}

	return false;

}	// IsPartInList

void XMPDocOpsUtils::NormalizePartsList_v2(const spINode & stEvt_changed) {

	//TODO :Implemented under the assumption that stEvt_changed muust be a simplenode
//	 Change a struct (should never happen) to an empty simple value.

	XMP_OptionBits stEvtChangedOptions = 0;
	XMP_Assert(stEvt_changed->GetNodeType() == INode::kNTSimple);
	XMP_VarString value = stEvt_changed->ConvertToSimpleNode()->GetValue()->c_str();

//	 Reduce a value containing all and other parts to just "/".

	size_t valueSize = value.length();

	if (valueSize >= 2) {

		if (((value[0] == '/') && (value[1] == ';')) ||
			((value[valueSize - 1] == ';') && (value[valueSize - 2] == '/')) ||
			((valueSize >= 3) && (value.find(";/;") != std::string::npos))) {
			value = '/';
		}

	}
	stEvt_changed->ConvertToSimpleNode()->SetValue( value.c_str(), value.size() );
}	// NormalizePartsList

// -----------// -------------------------------------------------------------------------------------------------
// GetLastModTime
// --------------
//
// Look for the last "saved" or "created" xmpMM:History entry and extract its timestamp.

// ! The implementation takes advantage of being part of XMPCore, directly processing the tree nodes.

bool XMPDocOpsUtils::GetLastModTime_v2(const XMPMeta & xmpMeta, XMP_DateTime * modTime)
{
	XMP_ExpandedXPath	expPath;
	ExpandXPath(kXMP_NS_XMP_MM, "History", &expPath);
	const XMPMeta2  &xmpMeta2Ptr = dynamic_cast<const XMPMeta2 &> (xmpMeta);
	spIArrayNode arrayNode;
	XMP_OptionBits arrayNodeOptions = 0;
	spINode destNode;
	if (!XMPUtils::FindCnstNode(xmpMeta2Ptr.mDOM, expPath, destNode, &arrayNodeOptions))
		return false;
	arrayNode = destNode->ConvertToArrayNode();
	if (!XMP_PropIsArray(arrayNodeOptions)) return false;	// Ignore bad history.

	for (size_t item = arrayNode->ChildCount(); item >= 1; --item) {

		spINode historyNode = arrayNode->GetNodeAtIndex(item);
		if (!XMP_PropIsStruct(XMPUtils::GetIXMPOptions(historyNode))) continue;	// Ignore bad history.

		// *** Depends on stEvt namespace prefix:
		spINode fieldNode = XMPUtils::FindChildNode(historyNode, "action", kXMP_NS_XMP_ResourceEvent, kXMP_ExistingOnly, 0);
		if (!fieldNode) continue;	// Ignore bad history.
		spcIUTF8String fieldNodeValue = fieldNode->ConvertToSimpleNode()->GetValue();
		if (!XMP_LitMatch(fieldNodeValue->c_str(), "saved") && !XMP_LitMatch(fieldNodeValue->c_str(), "created")) continue;

		// *** Depends on stEvt namespace prefix:
		fieldNode = XMPUtils::FindChildNode(historyNode, "when", kXMP_NS_XMP_ResourceEvent, kXMP_ExistingOnly, 0);
		if (!fieldNode) return false;	// ! Return, don't look for earlier saved/created events.
		fieldNodeValue = fieldNode->ConvertToSimpleNode()->GetValue();
		try {
			XMPUtils::ConvertToDate(fieldNodeValue->c_str(), modTime);
			return true;
		}
		catch (...) {
			return false;	// Ignore bad history.
		}

	}

	return false;

}	// GetLastModTime--------------------------------------------------------------------------------------

#endif
#endif
