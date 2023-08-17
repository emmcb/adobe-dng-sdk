// =================================================================================================
// Copyright 2007 Adobe
// All Rights Reserved.
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
// AJ Abhishek Jindal
//
// mm-dd-yy who Description of changes, most recent on top.
//
// 02-06-15 AJ  5.6-c037 Fixing warnings due to implicit typecasting
// 12-21-14 AJ  5.6-c031 Routed XMPDocOps to new DOM APIs.
//

// =================================================================================================
#endif // AdobePrivate


#if AdobePrivate
#include "XMPCore/XMPCoreDefines_I.h"
#if ENABLE_CPP_DOM_MODEL
#include "XMPCore/source/XMPDocOps2.hpp"
#include "XMPCore/source/XMPDocOps.hpp"
#include "XMPCore/source/XMPDocOps-Utils.hpp"

#include "public/include/client-glue/WXMPMeta.hpp"
#include "XMPCore/source/XMPUtils.hpp"
#include "XMPCore/Interfaces/IMetadata_I.h"
#include "XMPCore/Interfaces/INode_I.h"
#include "XMPCore/Interfaces/IArrayNode_I.h"
#include "XMPCore/Interfaces/ISimpleNode_I.h"
#include "XMPCommon/Interfaces/IUTF8String_I.h"


// =================================================================================================

static XMP_VarString * sAppName = 0;
static XMP_ReadWriteLock * sGUIDCreationLock = 0;

static XMP_StringLen  voidLen;		// For "don't want" parameters to GetProperty.
static XMP_OptionBits voidOptions;
using namespace AdobeXMPCore_Int;
using namespace AdobeXMPCommon_Int;


// =================================================================================================
// =================================================================================================

// -------------------------------------------------------------------------------------------------
// XMPDocOps2
// ---------

XMPDocOps2::XMPDocOps2() 
{
	

}	// XMPDocOps2

// -------------------------------------------------------------------------------------------------
// ~XMPDocOps2
// ----------

XMPDocOps2::~XMPDocOps2() RELEASE_NO_THROW
{

	

}	// ~XMPDocOps2

// =================================================================================================
// =================================================================================================



// -------------------------------------------------------------------------------------------------
// GetPartChangeID
// ---------------
//
// Get the InstanceID of the most recent save that might have modified any of the given parts. This
// has to look at "converted", "copied", and "modified" events also as indications of change. Return
// true if an appropriate event is found. No ID is returned if the result is false.

bool
XMPDocOps2::GetPartChangeID ( XMP_StringPtr * partsPtr,
							 XMP_Index       partsCount,
							 XMP_StringPtr * idStr,
							 XMP_StringLen * idLen ) const
{
	XMPMeta2 * xmpMeta = dynamic_cast<XMPMeta2 *>(this->docXMP);
	if ( !xmpMeta ) XMP_Throw ( "Must call NewXMP or OpenXMP first", kXMPErr_BadObject );

	if ( !partsCount ) return false;

	XMP_ExpandedXPath	expPath;
	ExpandXPath ( kXMP_NS_XMP_MM, "History", &expPath );
	spIArrayNode arrayNode;
	XMP_OptionBits arrayNodeOptions = 0;
	spINode destNode;
	if(!XMPUtils::FindCnstNode( xmpMeta->mDOM, expPath, destNode , &arrayNodeOptions)) return false;
	
	arrayNode = destNode->ConvertToArrayNode();
	if ( ! XMP_PropIsArray ( arrayNodeOptions ) ) return false;

	spINode historyNode, actionNode, idNode, partsNode, prevSave;

	for ( size_t item = arrayNode->ChildCount(); item > 0; --item ) {
		
		historyNode = arrayNode->GetNodeAtIndex(item);
		if (historyNode->GetNodeType() == INode::kNTArray ) continue;	// Ignore bad history.

		// *** Depends on stEvt namespace prefix:
		actionNode = XMPUtils::FindChildNode(historyNode, "action", kXMP_NS_XMP_ResourceEvent, kXMP_ExistingOnly, 0);
		if (!actionNode) continue;	// Ignore bad history.
		XMP_VarString actionNodeValue = actionNode->ConvertToSimpleNode()->GetValue()->c_str();
		if ( actionNodeValue == "saved" ) {

			prevSave = historyNode;

			// *** Depends on stEvt namespace prefix:
			partsNode = XMPUtils::FindChildNode(historyNode, "changed", kXMP_NS_XMP_ResourceEvent, kXMP_ExistingOnly, 0);
			if ( !partsNode || actionNode->ConvertToSimpleNode()->GetValue()->empty() ) return false;
			if ( ! XMPDocOpsUtils::IsPartInList_v2 ( partsPtr, partsCount, partsNode ) ) continue;

			// *** Depends on stEvt namespace prefix:
			
			idNode = XMPUtils::FindChildNode(historyNode, "instanceID", kXMP_NS_XMP_ResourceEvent, kXMP_ExistingOnly, 0);
			spcIUTF8String idNodeValue = XMPUtils::GetNodeValue(idNode);
			if ( !idNode || idNodeValue->empty() ) return false;
			*idStr = idNodeValue->c_str();
			*idLen = static_cast<XMP_StringLen>( idNodeValue->size() );
			return true;

		} else if ( actionNodeValue == "created" ) {

			// *** Depends on stEvt namespace prefix:
			idNode = XMPUtils::FindChildNode(historyNode, "instanceID", kXMP_NS_XMP_ResourceEvent, kXMP_ExistingOnly, 0);
			spcIUTF8String idNodeValue = XMPUtils::GetNodeValue(idNode);
			if ( !idNode || idNodeValue->empty() ) return false;
			*idStr = idNodeValue->c_str();
			*idLen = static_cast<XMP_StringLen>( idNodeValue->size() );
			return true;
		
		} else if ( (actionNodeValue == "converted") ||
					(actionNodeValue == "copied") ||
					(actionNodeValue == "modified") ) {

			if ( prevSave == libcppNULL ) return false;
			idNode = XMPUtils::FindChildNode(historyNode, "instanceID", kXMP_NS_XMP_ResourceEvent, kXMP_ExistingOnly, 0);
			spcIUTF8String idNodeValue = XMPUtils::GetNodeValue(idNode);
			if ( !idNode || idNodeValue->empty() ) return false;
			*idStr = idNodeValue->c_str();
			*idLen = static_cast<XMP_StringLen>( idNodeValue->size() );
			return true;

		}

	}

	return false;

}	// GetPartChangeID


// =================================================================================================
// =================================================================================================

// -------------------------------------------------------------------------------------------------
// HasPartChanged
// --------------
//
// Determine if any of the given parts have changed since the save with the given InstanceID. We get
// a yes answer from "saved" or "converted" events, a maybe answer from "copied" or "modified"
// events, and a maybe answer if no end point is found ("created" or "saved" with the given InstanceID).

XMP_TriState
XMPDocOps2::HasPartChanged ( XMP_StringPtr   prevInstanceID,
							XMP_StringPtr * partsPtr,
							XMP_Index       partsCount ) const
{
	XMPMeta2 * xmpMeta = dynamic_cast<XMPMeta2 *>(this->docXMP);
	if ( xmpMeta == 0 ) XMP_Throw ( "Must call NewXMP or OpenXMP first", kXMPErr_BadObject );

	if ( partsCount == 0 ) return kXMPTS_No;

	XMP_ExpandedXPath	expPath;
	ExpandXPath ( kXMP_NS_XMP_MM, "History", &expPath );
	XMP_OptionBits arrayOptions = 0;
	spIArrayNode arrayNode;
	spINode destNode;
	XMPUtils::FindCnstNode(xmpMeta->mDOM, expPath, destNode , &arrayOptions, 0);
	arrayNode = destNode->ConvertToArrayNode();
	if ( !arrayNode) return kXMPTS_Maybe;
	if ( ! XMP_PropIsArray ( arrayOptions ) ) return kXMPTS_Maybe;

	bool haveMaybe = false;	// Used for "copied" or "modified" maybes.
	spINode historyNode, actionNode, idNode, partsNode;
	XMP_OptionBits historyNodeOptions = 0;
	XMP_VarString actionNodeValue, idNodeValue;
	for ( size_t item = (int)XMPUtils::GetNodeChildCount(arrayNode); item > 0; --item ) {

		historyNode = arrayNode->GetNodeAtIndex(item);
		
		if ( historyNode->GetNodeType() != INode::kNTStructure ) continue;	// Ignore bad history.

		// *** Depends on stEvt namespace prefix:
		actionNode = XMPUtils::FindChildNode(historyNode, "action", kXMP_NS_XMP_ResourceEvent, false, 0);
		
		if ( !actionNode ) continue;	// Ignore bad history.
		if( actionNode->GetNodeType() != INode::kNTSimple) continue;
		actionNodeValue = XMPUtils::GetNodeValue(actionNode)->c_str();
		if ( actionNodeValue == "saved" ) {

			// *** Depends on stEvt namespace prefix:
			idNode = XMPUtils::FindChildNode ( historyNode, "instanceID", kXMP_NS_XMP_ResourceEvent, kXMP_ExistingOnly, 0);
			idNodeValue = XMPUtils::GetNodeValue(idNode)->c_str();
			if ( (idNode != libcppNULL) && (idNodeValue == prevInstanceID) ) {

				// Found the "saved" event with the given InstanceID.
				if ( haveMaybe ) return kXMPTS_Maybe;
				return kXMPTS_No;

			} else {

				// Found some other "saved" event, see what changed.
				// *** Depends on stEvt namespace prefix:
				partsNode = XMPUtils::FindChildNode(historyNode, "changed", kXMP_NS_XMP_ResourceEvent, kXMP_ExistingOnly, 0);
				spcIUTF8String partsNodeValue = XMPUtils::GetNodeValue(partsNode);
				if ( !partsNode || !partsNodeValue || partsNodeValue->empty() ) {
					haveMaybe = true;
				} else {
					if ( XMPDocOpsUtils::IsPartInList_v2 ( partsPtr, partsCount, partsNode ) ) return kXMPTS_Yes;
				}

			}

		} else if ( actionNodeValue == "created" ) {

			// *** Depends on stEvt namespace prefix:
			idNode = XMPUtils::FindChildNode(historyNode, "instanceID", kXMP_NS_XMP_ResourceEvent, kXMP_ExistingOnly, 0);
			if ( !idNode  || !XMP_LitMatch(XMPUtils::GetNodeValue(idNode)->c_str(),prevInstanceID) ) haveMaybe = true;
			if ( haveMaybe ) return kXMPTS_Maybe;
			return kXMPTS_No;

		} else if ( actionNodeValue == "converted" ) {

			return kXMPTS_Yes;

		} else if ( (actionNodeValue == "copied") || (actionNodeValue == "modified") ) {

			haveMaybe = true;

		}

	}

	return kXMPTS_Maybe;	// The save for the given InstanceID was not found.

}	// HasPartChanged

void
XMPDocOps2::PruneRedundantHistory()
{

	
	// Find the xmpMM:History node.
	
	XMPMeta2 * ptr = dynamic_cast<XMPMeta2 *>(this->docXMP);

	spINode historyNode = ptr->mDOM->GetNode( kXMP_NS_XMP_MM, AdobeXMPCommon::npos, "History", AdobeXMPCommon::npos );
	
	XMP_Assert ( historyNode != libcppNULL );	// Should always exist, called from PrepareForSave after AppendHistory.
	
	XMP_Int32 historySize = ( XMP_Int32 ) XMPUtils::GetNodeChildCount( historyNode );
	if ( historySize < 3 ) return;
	
	// Prune redundant "saved" entries from the tail of the history.
	spIArrayNode arrayNode = historyNode->ConvertToArrayNode();
	
	XMPDocOpsUtils::PruneOneRedundantRun_v2 ( arrayNode , historySize - 1  );
	
	// If the history is still big do a global cleanup once for this XMPDocOps object. Use 1000
	// history entries as the general case limit. Use 100 as the limit for JPEG, trying to keep the
	// XMP under 64 KB. Each history entry is about 550 bytes, so 100 of them is about 55 KB.
	
	if ( this->allHistoryCleaned ) return;
	this->allHistoryCleaned = true;
	
	XMP_Int32 historyLimit = 1000;
	XMP_StringPtr dcFormat;
	bool found = ptr->GetProperty ( kXMP_NS_DC, "format", &dcFormat, &voidLen, &voidOptions );
	if ( found && XMP_LitMatch ( dcFormat, "image/jpeg" ) ) historyLimit = 100;
	
	historySize = (XMP_Int32)XMPUtils::GetNodeChildCount(historyNode);	// Set again, it got reduced above.
	if ( historySize < historyLimit ) return;
	
	// Go forward through the history and normalize the stEvt:changed fields.
	spINode currEntry, currParts;
	for ( XMP_Int32 item = 1; item <= historySize; ++item ) {
		currEntry = historyNode->ConvertToArrayNode()->GetNodeAtIndex( item );
		currParts = XMPUtils::FindChildNode(currEntry, "changed", kXMP_NS_XMP_ResourceRef, false, 0);
		if ( currParts) XMPDocOpsUtils::NormalizePartsList_v2 ( currParts );
	}
	
	// Repeat the redundant run pruning, now looking at the entire history array. Start at the end
//	 again, the stEvt:changed normalization might have exposed a run.
	
	XMP_Int32 endIndex = historySize - 1 ;
	while ( endIndex >= 2 ) {
		spIArrayNode tempNode = historyNode->ConvertToArrayNode();
		endIndex = XMPDocOpsUtils::PruneOneRedundantRun_v2(tempNode, endIndex );
		historyNode = tempNode;
	}

}	// PruneRedundantHistory

#endif 
#endif 
