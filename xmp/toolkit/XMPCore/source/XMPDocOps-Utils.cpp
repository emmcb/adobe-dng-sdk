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
// JEH Joerg Ehrlich
//
// mm-dd-yy who Description of changes, most recent on top.
//
// 02-06-15 AJ  5.6-c037 Fixing warnings due to implicit typecasting
// 06-08-12 AWL 5.5-c002 Use standard O/S utilities for GUID creation.
//
// 11-25-11	JEH	5.3-c009 [3055349] fix recognition of redundant entries in sequential NoteChange calls.
// 11-22-11	JEH	5.3-c008 Moved DocOps utilities to this class, to be able to write unit tests.
//
// =================================================================================================
#endif // AdobePrivate

#include "XMPCore/source/XMPDocOps-Utils.hpp"
#include "XMPCore/source/XMPUtils.hpp"
#if  XMP_AndroidBuild
	#include "XMPCore/source/Android_Utils.hpp"
#endif
// =================================================================================================

static XMP_StringLen  voidLen;		// For "don't want" parameters to GetProperty.
static XMP_OptionBits voidOptions;

// =================================================================================================

typedef XMP_Uns8 BinaryID [16];
typedef char StringID [50];   // Need 45: 8+36+1, "xmp.*id:12345678-1234-1234-1234-123456789012"

static const char * Hex = "0123456789abcdef";	// Use lower case a-f to be consistent with IETF RFC 4122.

static inline void FormatByte ( XMP_Uns8 b, char * ch ) {
   ch[0] = Hex[b>>4];
   ch[1] = Hex[b&0xF];
}

// =================================================================================================

#if XMP_MacBuild | XMP_iOSBuild

	#include <CoreFoundation/CoreFoundation.h> 

	static void CreateBinaryID ( BinaryID * binID ) {
		XMP_Assert ( sizeof ( BinaryID ) == sizeof ( CFUUIDBytes ) );
		CFUUIDRef cfID = CFUUIDCreate ( kCFAllocatorDefault );
		*((CFUUIDBytes*)binID) = CFUUIDGetUUIDBytes ( cfID );
		CFRelease ( cfID );
	}

#elif XMP_WinBuild

	#include <Windows.h>
	#include "Objbase.h"
	#include "rpcdce.h"

	static void CreateBinaryID ( BinaryID * binID ) {
		XMP_Assert ( sizeof ( BinaryID ) == sizeof ( UUID ) );
		HRESULT err = CoCreateGuid( (UUID*)binID );
		if ( err != S_OK ) {
			XMP_Throw ( "Failure from UuidCreate", kXMPErr_ExternalFailure );
		}
		

	}

#elif XMP_UNIXBuild

#include <boost/uuid/uuid.hpp>            
#include <boost/uuid/uuid_generators.hpp> 
  

	static void CreateBinaryID(BinaryID * binID) {
		boost::uuids::random_generator generator;
		boost::uuids::uuid uuid = generator(); 
	
		XMP_Index index = 0;
		for (boost::uuids::uuid::const_iterator it = uuid.begin(); it != uuid.end(); ++it) {
			(*binID)[index] = *it;
			index++;
			if (index > 15)
				break;
			
		}
		
	}

#elif  XMP_AndroidBuild
	static void CreateBinaryID(BinaryID * binID) {
		GuidGenerator(*binID);
	}

#endif

// =================================================================================================
// =================================================================================================

// -------------------------------------------------------------------------------------------------
// ConjureUURI
// -----------
//
// Conjure a UUID and format it as a URI.

// *** Currently use Photoshop's format : uuid:<uuid-in-hex>
// *** Should define a standard form based on a registered URI scheme.

void XMPDocOpsUtils::ConjureUURI ( XMP_StringPtr prefix, XMP_VarString * idStr, XMP_ReadWriteLock * threadLock )
{
	XMP_Assert ( (prefix != 0) && (prefix[0] != 0) );

	BinaryID binID;
	idStr->erase();

	// Create the binary ID, with thread locking if desired.
	
	if ( threadLock == 0 ) {
		CreateBinaryID ( &binID );
	} else {
		XMP_AutoLock autoLock ( threadLock, kXMP_WriteLock );
		CreateBinaryID ( &binID );
		autoLock.Release();
	}
	
	// Initialize the output string, reserving space, setting the prefix, and setting a suffix template.
	
	size_t prefixLen = strlen ( prefix );
	idStr->reserve ( prefixLen + 1 + 36 );	// ...:12345678-1234-1234-1234-123456789012
	*idStr = prefix;
	if ( (*idStr)[prefixLen-1] != ':' ) *idStr += ':';
	prefixLen = idStr->size();
	*idStr += "12345678-1234-1234-1234-123456789012";

	// Replace the suffix template with the actual ID.
	
	XMP_Uns8 * b = &binID[0];
	char * ch = &((*idStr)[prefixLen]);	// Should point to the first character of the suffix.
	XMP_Assert ( (*(ch-1) == ':') && (*ch == '1') );
	
	for ( int i = 0; i < 4; ++i, ++b, ch += 2 ) FormatByte ( *b, ch );
	++ch;	// Skip the '-'.
	for ( int i = 0; i < 2; ++i, ++b, ch += 2 ) FormatByte ( *b, ch );
	++ch;	// Skip the '-'.
	for ( int i = 0; i < 2; ++i, ++b, ch += 2 ) FormatByte ( *b, ch );
	++ch;	// Skip the '-'.
	for ( int i = 0; i < 2; ++i, ++b, ch += 2 ) FormatByte ( *b, ch );
	++ch;	// Skip the '-'.
	for ( int i = 0; i < 6; ++i, ++b, ch += 2 ) FormatByte ( *b, ch );

}	// ConjureUURI

// -------------------------------------------------------------------------------------------------
// GetLastModTime
// --------------
//
// Look for the last "saved" or "created" xmpMM:History entry and extract its timestamp.

// ! The implementation takes advantage of being part of XMPCore, directly processing the tree nodes.

bool XMPDocOpsUtils::GetLastModTime ( const XMPMeta & xmpMeta, XMP_DateTime * modTime )
{

#if AdobePrivate
	#if ENABLE_CPP_DOM_MODEL
	
		if(sUseNewCoreAPIs) {
			
			const XMPMeta2 & xmpMeta2Ptr = dynamic_cast<const XMPMeta2 &>(xmpMeta);
			return XMPDocOpsUtils::GetLastModTime_v2(xmpMeta, modTime);
		}

	#endif
#endif
	
	XMP_ExpandedXPath	expPath;
	ExpandXPath(kXMP_NS_XMP_MM, "History", &expPath);
	const XMP_Node * arrayNode = FindConstNode(&xmpMeta.tree, expPath);
	if (arrayNode == 0) return false;
	if (!XMP_PropIsArray(arrayNode->options)) return false;	// Ignore bad history.

	for (size_t item = (int)arrayNode->children.size(); item > 0; --item) {

		XMP_Node * historyNode = arrayNode->children[item - 1];
		if (!XMP_PropIsStruct(historyNode->options)) continue;	// Ignore bad history.

		// *** Depends on stEvt namespace prefix:
		XMP_Node * fieldNode = FindChildNode(historyNode, "stEvt:action", kXMP_ExistingOnly);
		if (fieldNode == 0) continue;	// Ignore bad history.
		if ((fieldNode->value != "saved") && (fieldNode->value != "created")) continue;
		// *** Depends on stEvt namespace prefix:
		fieldNode = FindChildNode(historyNode, "stEvt:when", kXMP_ExistingOnly);
		if (fieldNode == 0) return false;	// ! Return, don't look for earlier saved/created events.
		try {
			XMPUtils::ConvertToDate(fieldNode->value.c_str(), modTime);
			return true;
		}
		catch (...) {
			return false;	// Ignore bad history.
		}

	}

	return false;

}	// GetLastModTime

// -------------------------------------------------------------------------------------------------
// EnsureHistoryForm
// -----------------
//
// Make sure any existing xmpMM:History is an array, delete it if not.

void XMPDocOpsUtils::EnsureHistoryForm ( XMPMeta * xmpMeta )
{
	XMP_StringPtr voidPtr;
	XMP_OptionBits historyFlags;

	bool historyExists = xmpMeta->GetProperty ( kXMP_NS_XMP_MM, "History", &voidPtr, &voidLen, &historyFlags );

	if ( historyExists && (! XMP_PropIsArray (historyFlags)) ) {
		xmpMeta->DeleteProperty ( kXMP_NS_XMP_MM, "History" );
	}

}	// EnsureHistoryForm

// -------------------------------------------------------------------------------------------------
// FillResourceRef
// ---------------

void XMPDocOpsUtils::FillResourceRef ( const XMPMeta & srcXMP, XMPMeta * destXMP, XMP_StringPtr destNS, XMP_StringPtr destProp )
{
	bool found;
	XMP_StringPtr value;

	found = srcXMP.GetProperty ( kXMP_NS_XMP_MM, "InstanceID", &value, &voidLen, &voidOptions );
	if ( found ) {
		destXMP->SetStructField ( destNS, destProp, kXMP_NS_XMP_ResourceRef, "instanceID", value, kXMP_DeleteExisting );
	}

	found = srcXMP.GetProperty ( kXMP_NS_XMP_MM, "DocumentID", &value, &voidLen, &voidOptions );
	if ( found ) {
		destXMP->SetStructField ( destNS, destProp, kXMP_NS_XMP_ResourceRef, "documentID", value, kXMP_DeleteExisting );
	}

	found = srcXMP.GetProperty ( kXMP_NS_XMP_MM, "OriginalDocumentID", &value, &voidLen, &voidOptions );
	if ( found ) {
		destXMP->SetStructField ( destNS, destProp, kXMP_NS_XMP_ResourceRef, "originalDocumentID", value, kXMP_DeleteExisting );
	}

	found = srcXMP.GetProperty ( kXMP_NS_XMP_MM, "VersionID", &value, &voidLen, &voidOptions );
	if ( found ) {
		destXMP->SetStructField ( destNS, destProp, kXMP_NS_XMP_ResourceRef, "versionID", value, kXMP_DeleteExisting );
	}

	found = srcXMP.GetProperty ( kXMP_NS_XMP_MM, "RenditionClass", &value, &voidLen, &voidOptions );
	if ( found ) {
		destXMP->SetStructField ( destNS, destProp, kXMP_NS_XMP_ResourceRef, "renditionClass", value, kXMP_DeleteExisting );
	}

	found = srcXMP.GetProperty ( kXMP_NS_XMP_MM, "RenditionParams", &value, &voidLen, &voidOptions );
	if ( found ) {
		destXMP->SetStructField ( destNS, destProp, kXMP_NS_XMP_ResourceRef, "renditionParams", value, kXMP_DeleteExisting );
	}

	found = srcXMP.GetProperty ( kXMP_NS_XMP_MM, "Manager", &value, &voidLen, &voidOptions );
	if ( found ) {
		destXMP->SetStructField ( destNS, destProp, kXMP_NS_XMP_ResourceRef, "manager", value, kXMP_DeleteExisting );
	}

	found = srcXMP.GetProperty ( kXMP_NS_XMP_MM, "ManagerVariant", &value, &voidLen, &voidOptions );
	if ( found ) {
		destXMP->SetStructField ( destNS, destProp, kXMP_NS_XMP_ResourceRef, "managerVariant", value, kXMP_DeleteExisting );
	}

	found = srcXMP.GetProperty ( kXMP_NS_XMP_MM, "ManageTo", &value, &voidLen, &voidOptions );
	if ( found ) {
		destXMP->SetStructField ( destNS, destProp, kXMP_NS_XMP_ResourceRef, "manageTo", value, kXMP_DeleteExisting );
	}

	found = srcXMP.GetProperty ( kXMP_NS_XMP_MM, "ManageUI", &value, &voidLen, &voidOptions );
	if ( found ) {
		destXMP->SetStructField ( destNS, destProp, kXMP_NS_XMP_ResourceRef, "manageUI", value, kXMP_DeleteExisting );
	}

}	// FillResourceRef

// -------------------------------------------------------------------------------------------------

void XMPDocOpsUtils::NormalizePartPath ( std::string & part )
{
	// Enforce a leading '/', strip the tail of the path if it has any unknown operators.
	
	if ( part[0] != '/' ) part.insert ( 0, 1, '/' );
	
	size_t i, limit;
	for ( i = 0, limit = part.size(); i < limit; ++i ) {
	
		XMP_Uns8 ch = part[i];
		if ( ch > 0x7F ) continue;
		if ( ('a' <= ch) && (ch <= 'z') ) continue;
		if ( ('A' <= ch) && (ch <= 'Z') ) continue;
		if ( ('0' <= ch) && (ch <= '9') ) continue;
		if ( (ch == '/') || (ch == ':') || (ch == '.') || (ch == '_') || (ch == '-') ) continue;
		break;
	
	}
	
	if ( i != limit ) {
		XMP_Assert ( (i > 0) && (part[0] == '/') );
		for ( ; part[i] != '/'; --i ) {}
		if ( i > 0 ) {
			part.erase ( i );
		} else {
			part = "/";
		}
	}

	// strip potential trailing /
	if( part.size() > 1 && part[part.size() - 1] == '/')
	{
		part.erase(part.size() - 1);
	}
	
}	// NormalizePartPath

// -------------------------------------------------------------------------------------------------
// NormalizePartsList
// ------------------
//
// Go forward through the history and normalize the stEvt:changed fields. There are known cases of
// files containing history entries with the old array form of stEvt:changed. These need to be
// changed to the simple string form. There are also known cases of all plus other usage, e.g.
// "/;/metadata". These need to be changed to simply "/".

void XMPDocOpsUtils::NormalizePartsList ( XMP_NodePtr stEvt_changed ) {
	
	// Change a struct (should never happen) to an empty simple value.
	
	if ( XMP_PropIsStruct ( stEvt_changed->options ) ) {
		stEvt_changed->RemoveChildren();
		stEvt_changed->options &= ~kXMP_PropCompositeMask;
		XMP_Assert ( XMP_PropIsSimple ( stEvt_changed->options ) && stEvt_changed->value.empty() );
		return;
	}
	
	// Catenate array item values into a single string.
	
	if ( XMP_PropIsArray ( stEvt_changed->options ) ) {

		XMP_Assert ( stEvt_changed->value.empty() );

		for ( size_t part = 0; part < stEvt_changed->children.size(); ++part ) {
			XMP_NodePtr onePart = stEvt_changed->children[part];
			if ( ! onePart->value.empty() ) {
				stEvt_changed->value.append ( onePart->value );
				stEvt_changed->value.append ( ";" );
			}
		}

		if ( ! stEvt_changed->value.empty() ) {
			stEvt_changed->value.erase ( stEvt_changed->value.size() - 1 );	// Remove the final ';'.
		}

		stEvt_changed->RemoveChildren();
		stEvt_changed->options &= ~kXMP_PropCompositeMask;
		XMP_Assert ( XMP_PropIsSimple ( stEvt_changed->options ) );

	}
	
	// Reduce a value containing all and other parts to just "/".
	
	size_t valueSize = stEvt_changed->value.size();
	XMP_Assert ( XMP_PropIsSimple ( stEvt_changed->options ) );
	
	if ( valueSize >= 2 ) {
	
		const char * firstTwo = stEvt_changed->value.c_str();
		const char * lastTwo  = firstTwo + valueSize - 2;
		
		if ( ((firstTwo[0] == '/') && (firstTwo[1] == ';')) ||
			 ((lastTwo[0] == ';') && (lastTwo[1] == '/')) ||
			 ((valueSize >= 3) && (stEvt_changed->value.find(";/;") != std::string::npos)) ) {
			stEvt_changed->value = '/';
		}
	
	}

}	// NormalizePartsList

// -------------------------------------------------------------------------------------------------
// PruneOneRedundantRun
// --------------------
//
// Prune redundant "saved" entries from the history, looking for a run ending at the given index. To
// keep the array indices straight, suppose there are 4 redundant entries. These have vector indices
// of end-3 through end, and we want to remove end-2 through end-1. Returns the index of the next
// place to look for a run, either the start-1 or the endIndex-1 if there is no run.

XMP_Int32 XMPDocOpsUtils::PruneOneRedundantRun ( XMP_NodePtr historyNode, XMP_Int32 endIndex )
{

	XMP_NodePtr finalAction = FindChildNode ( historyNode->children[endIndex], "stEvt:action", false, 0 );
	if ( (finalAction == 0) || finalAction->value != "saved" ) return endIndex - 1;

	XMP_NodePtr finalParts = FindChildNode ( historyNode->children[endIndex], "stEvt:changed", false, 0 );
	if ( finalParts == 0 || finalParts->value.empty() ) return endIndex - 1;

	XMP_Int32 runLength = 1;
	
	for ( XMP_Int32 item = endIndex-1; item >= 0; --item ) {	// ! Need a signed loop index!
		XMP_NodePtr currAction = FindChildNode ( historyNode->children[item], "stEvt:action", false, 0 );
		if ( (currAction == 0) || (currAction->value != "saved") ) break;
		XMP_NodePtr currParts = FindChildNode ( historyNode->children[item], "stEvt:changed", false, 0 );
		if ( (currParts == 0) || (currParts->value != finalParts->value) ) break;
		++runLength;
	}

	if ( runLength >= 3 ) {

		XMP_Int32 deleteStart = endIndex - (runLength-2);
		XMP_Int32 deleteEnd   = endIndex - 1;
	
		for ( XMP_Int32 item = deleteStart; item <= deleteEnd; ++item ) {
			delete historyNode->children[item];	// Delete the XMP node, the vector holds pointers.
			historyNode->children[item] = 0;
		}
		
		XMP_NodePtrPos iterStart = historyNode->children.begin() + deleteStart;
		XMP_NodePtrPos iterEnd   = historyNode->children.begin() + deleteEnd + 1;
		historyNode->children.erase ( iterStart, iterEnd );	// Removes deleteStart through and including deleteEnd.
	
	}
	
	return endIndex - runLength;

}	// PruneOneRedundantRun

// -------------------------------------------------------------------------------------------------
// IsPartInList
// ------------
//
// The partsNode contains a semicolon separated list of parts. An input part might be explicitly in
// the list, might be a descendant of some part in the list, or might be an ancestor of some part in
// the list. In all three cases the part is considered found. Siblings do not affect each other.
//
// In other words, a part is found if:
//	1. The input and list values match.
//	2. The shorter+'/' matches the start of the longer.
//
// Examples:
//	input part	list part	result
//	/foo		/foo		in
//	/foo		/foo/bar	in
//	/foo/bar	/foo		in
//	/foo/b1		/foo/b2		not in

bool XMPDocOpsUtils::IsPartInList ( XMP_StringPtr * newPartsPtr, XMP_Index newPartsCount, const XMP_Node & oldPartsNode )
{
	XMP_Assert ( (newPartsPtr != 0) || (newPartsCount == 0) );
	
	if ( (newPartsCount == 0) || oldPartsNode.value.empty() ) return false;
	if ( oldPartsNode.value == "/" ) return true;	// Common special case of "all changed".
	
	std::string newPart, oldPart;

	for ( ; newPartsCount > 0; --newPartsCount, ++newPartsPtr ) {
	
		newPart = *newPartsPtr;
		NormalizePartPath ( newPart );
		size_t newLen = newPart.size();
		XMP_Assert ( newPart[0] == '/' );
		
		if ( newLen == 1 ) return true;	// The newPart must be "/".

		XMP_StringPtr nextPart = oldPartsNode.value.c_str();
		XMP_StringPtr limit    = nextPart + oldPartsNode.value.size();
		
		while ( nextPart < limit ) {
		
			XMP_StringPtr oldPtr = nextPart;
			while ( (nextPart < limit) && (*nextPart != ';') ) ++nextPart;
			size_t oldLen  = nextPart - oldPtr;	// ! Do this before following increment.
			if ( nextPart < limit ) ++nextPart;			// Move past the ';'.
			if ( oldLen == 0 ) continue;				// ! Do this after the above increment.

			oldPart.assign ( oldPtr, oldLen );
			NormalizePartPath ( oldPart );
			oldLen = oldPart.size();

			if ( newLen == oldLen ) {
				// Do the parts match?
				if ( newPart == oldPart ) return true;
			} else if ( newLen > oldLen ) {
				// Is newPart a descendant of oldPart?  (Must use bigger.compare!)
				if ( (newPart[oldLen] == '/') && (newPart.compare ( 0, oldLen, oldPart ) == 0) ) return true;
			} else {	// newLen < oldLen
				// Is newPart an ancestor of oldPart?  (Must use bigger.compare!)
				if ( (oldPart[newLen] == '/') && (oldPart.compare ( 0, newLen, newPart ) == 0) ) return true;
			}
		
		}
		
	}

	return false;

}	// IsPartInList

// =================================================================================================
