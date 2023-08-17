// =================================================================================================
// Copyright 2003 Adobe
// All Rights Reserved.
//
// NOTICE:  Adobe permits you to use, modify, and distribute this file in accordance with the terms
// of the Adobe license agreement accompanying it. If you have received this file from a source other 
// than Adobe, then your use, modification, or distribution of it requires the prior written permission
// of Adobe.
//
// Adobe patent application tracking #P435, entitled 'Unique markers to simplify embedding data of
// one format in a file with a different format', inventors: Sean Parent, Greg Gilley.
// =================================================================================================

#if AdobePrivate
// =================================================================================================
// Change history
// ==============
//
// Writers:
//  AWL Alan Lillich
//  FNO Frank Nocke
//  ADC Amandeep Chawla
//	PRI Priyanka Gupta
//
// mm-dd-yy who Description of changes, most recent on top.
//
// 03-27-15 ADC 5.5-c066 Porting changes to gcc 4.4.4.
// 03-26-15 AJ  5.6-c065 Disabling the code for linux for integrating into green as shared_ptr is not available on gcc 4.4.4
// 03-26-15 AJ  5.6-c064 Minimizing risk of shared_ptr usage across dlls by changing glue code
// 03-25-15 PRI 5.6-c063 Changing map<string,vector<>> .count to .size as it wasn't working as expected. 
// 03-15-15 AJ  5.6-c062 Changing method of acquiring ownership of the shared_ptr 
// 03-23-15	PRI	5.6-c060 Fixing some issues in Get/SetBulkMarker NewDOMImplementation and adding missing NULL check. 
// 03-23-15 AJ  5.6-c059 Fixing issue with a corner case of structure type extension
// 03-23-15 AJ  5.6-c058 Fixing issue with setting the parent node of the extension node
// 03-20-15 AJ  5.6-c057 Fixing minor issue with json parser exception handling
// 03-19-15 AJ  5.6-c056 Fixing bug 3953797
// 03-19-15 AJ  5.6-c054 Adding merging logic for extensions present in the extension node and the cue point params
// 03-18-15 AJ  5.6-c052 Adding shared ptr for the private data in the marker structure 
// 03-17-15 AJ  5.6-c049 Adding a missed null check in GetBulkMarkers()
// 03-16-15 AJ  5.6-c048 Fixing build break due to XMPCore.def
// 03-16-15 AJ  5.6-c047 Completing Rework of Get/SetBulkMarkers()
// 12-03-15 AJ  5.6-c045 Reworking glue code for Get/SetBulkMarkers()
// 09-03-15 AJ  5.6-c044 Get/SetBulkMarkers version2 with the new DOM APIs and some bug fixes for the same
// 03-04-15 AJ  5.6-c042 Fixing release build crash in Get/SetBulkMarkers()
// 03-02-15 AJ  5.6-c040 Fixing some build issues and bugs in SetBulkMarkers()
// 03-02-15 AJ  5.6-c039 Marker Extensibility - Backward compatibility for extensions in GetBulkMarkers() and SetBulkMarkers()
// 02-06-15 AJ  5.6-c037 Fixing warnings due to implicit typecasting
// 07-10-14 ADC 5.6-c015 Refactoring, partial documentation and bug fixes of XMPCommon and XMPCore C++ APIs.
//
// 06-26-13 ADC 5.5-c019 [3083403] Failure to open a PDF file.
//						 GetProperty_Int, GetProperty_Int64 and GetProperrty_Float will tolerate presence of
//						 whitespace characters at the beginning and end.
//
// 03-26-10 AWL 5.1-c004 [2589499] Don't modify node.value in SetNodeValue if the new value is invalid UTF-8.
//
// 10-22-09 AWL 5.0-c057 [2283529] Fix DeleteLocalizedText to handle matching x-default and actual values.
// 06-11-09 AWL 5.0-c034 Finish threading revamp, implement friendly reader/writer locking.
// 05-19-09 AWL 5.0-c031 First part of threading improvements, revamp the client glue.
// 05-05-09 AWL 5.0-c027 [1239830] Fix AppendLangItem to call SetNodeValue.
// 04-22-09 AWL 5.0-c024 [1239830] Reject use of U+FFFE or U+FFFF in values.
// 03-27-09 AWL 5.0-c020 [1519098] Moved the check, added a comment explaining why only GetArrayItem has it.
// 02-16-09 FNO 5.0-c008 [1647989] Adding 3rd patch by H. Figuiere: adding delete-localized-text function.
// 01-23-09 AWL 5.0-c005 [2261961] Fix Get/SetBulkMarkers to tolerate varying size of cuePointParams vector.
//
// 05-05-08 AWL 4.2.2-c055 [1768777] Add private callbacks for CuePointParams storage in Get/SetBulkMarkers.
// 04-29-08 AWL 4.2.2-c054 [1768777,1768782] Move Get/SetBulkMarkers to XMPMeta_GetSet.cpp, call SetNodeValue.
//
// 01-25-08 AWL 4.2-c037 Change all xap* namespace prefixes to xmp*.
//
// 08-16-06 AWL 4.1-c015 [1312441,1340116] Change the parsing code to tolerate ISO Latin-1 input. At
//				the same time, get rid of the ugly hackery that tries to deal with unescaped ASCII
//				control characters. Also get rid of the check and complaint about parsing into an
//				empty object. This is predominantly what people want, so clear the XMPMeta object.
// 08-04-06 AWL 4.1-c014 [1345152] Change XMPMeta::AppendArrayItem to not check the form of existing
//				arrays, just accept what is there. We need to generally revisit error checking.
//
// 05-01-06 AWL 4.0-c005 Revamp the embedded, Mac PList, and WIndows Property info.
// 03-24-06 AWL 4.0-c001 Adapt for move to ham-perforce, integrate XMPFiles, bump version to 4.
//
// 06-07-05 AWL 3.2-112 Serialize empty packets nicely.
// 06-06-05 AWL 3.2-111 [0536565] Fix the compact serialization of empty structs.
// 06-03-05 AWL 3.2-109 [0467370] Parse failures must leave the XMP object empty, ready for another parse.
// 06-03-05 AWL 3.2-108 Output empty element in RDF for an empty array, e.g. <rdf:Seq/>.
// 06-02-05 AWL 3.2-105 [1016912] Use empty element form when rdf:Description has only attribute content.
// 06-01-05 AWL 3.2-105 [1110051] Add delete-existing option for SetProperty.
// 06-01-05 AWL 3.2-103 [0509601,1204017] Fix SetLocalizedText to find and set a matching "real"
//				language value when the specific_lang parameter is "x-default".
// 05-27-05 AWL 3.2-102 Move the setting of the alloc/free procs to the top of XMPMeta::Initialize.
// 05-16-05 AWL 3.2-100 Complete the deBIBification, integrate the internal and SDK source. Bump the
//              version to 3.3 and build to 100, well ahead of main's latest 3.3-009.
//
// 05-02-05 AWL 3.2-019 Add the Dynamic Media namespace, kXMP_NS_DM, "http://ns.adobe.com/xmp/1.0/DynamicMedia/".
// 04-14-05 AWL 3.2-018 Move the padding param, add overloads to simplify use of SerializeToBuffer.
// 04-13-05 AWL 3.2-017 Improve the documentation and behavior of Get/SetLocalizedText.
// 04-11-05 AWL 3.2-016 Add AdobePrivate conditionals where appropriate.
// 04-08-05 AWL 3.2-015 Undo unnecessary constant changes in XMP_Const.h - keep compatibility over
//				"perfection" so that SDK and internal code are easier to merge.
// 04-07-05 AWL 3.2-014 Fix SetLocalizedText to set the alt-text bit for empty alt array. This became
//				broken in the changes for 3.2-010 (SDK) and 3.3-003 (main). Don't throw an exception
//				if x:xmpmeta is missing and kXMP_RequireXMPMeta is used. Old code ignored that
//				candidate. This changed in the previous build's root node lookup changes.
// 04-06-05 AWL 3.2-013 [0509601] Normalize "single value" alt-text arrays. Improve the way the root
//				XML node is found and extract the previous toolkit version number.
// 04-05-05 AWL 3.2-012 [1172565] Move instance ID from rdf:about to xmpMM:InstanceID.
// 04-05-05 AWL 3.2-011 [0532345] Normalize xml:lang values so that compares are in effect case
//				insensitive as required by RFC 3066. Change parsing and serializing to force the
//				x-default item to be first.
// 04-01-05 AWL 3.2-010 Add leafOptions parameter to FindNode, used when creating new nodes.
// 03-17-05 AWL 3.2-006 Revise Plain XMP parsing and serialization for latest proposal.
// 02-16-05 AWL 3.2-005 Add first cut of Plain XMP parsing and serialization.
// 02-14-05 AWL 3.2-003 Add thread locks.
// 02-11-05 AWL 3.2-002 Add client reference counting.
// 01-28-05 AWL 3.2-001 Remove BIB.
//
// 02-02-05 AWL 3.1.1-110 [1145657] Fix XMPMeta::SerializeToBuffer to do an exact size UTF-16 or UTF-32
//				packet correctly. It was doing UTF-8 of the exact size, then converting.
// 01-26-05 AWL 3.1.1-107 [1141684] Add XMPMeta::UnregisterAssertNotify and XMPMeta::SendAssertNotify.
// 01-25-05 AWL 3.1.1-106 [1141007] Add XMPMeta::RegisterAssertNotify.
// 01-17-05 AWL 3.1.1-105 [1135773] Delete empty schema in addition to removing the pointer from the
//				tree node's child vector. This bug was introduced in the -101 changes.
// 01-07-05 AWL 3.1.1-104 [1129616] Fix SetObjectName to verify that the string is UTF-8.
// 12-14-04 AWL 3.1.1-101 [1085740] Delete empty schema nodes in CleanupParsedAliases.
// 12-14-04 AWL 3.1.1-100 [1022350,1075328] Add more namespaces and internal/external properties.
// 12-09-04 AWl 3.1.1-099 [1041438] Add parsing and serialization support for UTF-16 and UTF-32.
// 11-05-04 AWL 3.1.1-093 [1037508] Touch up exif:GPSTimeStamp values that have zero for the date.
// 11-05-04 AWL 3.1.1-092 [1106408] Add new IPTC namespace.
// 10-29-04 AWL 3.1.1-089 [1099634] Fix ParseFromBuffer to properly handle a partial escape followed
//				by another escape. E.g. "blah blah &#" then "xA;&#xA; blah blah".
// 10-20-04 AWL 3.1.1-085 [1084185] Fix XMP_InternalRef to not depend on BIBContainerBase layout.
// 10-12-04 AWL 3.1.1-084 [0616293] Add value checking to SetNodeValue, prefix checking to RegisterNamespace.
// 10-06-04 AWL 3.1.1-083 [1061778] Add lock tracing under TraceXMPLocking.
// 10-05-04 AWL 3.1.1-082 [1061778] Use recursive form of BIB mutex.
//
// 08-27-04 AWL 3.1-079 Remove overly aggressive option checks in SerializeAsRDF.
// 08-27-04 AWL 3.1-078 Add EXIF_Aux and CameraRaw namespaces.
// 08-23-04 AWL 3.1-076 Add kXMP_NS_PSAlbum as a predefined namespace.
// 08-04-04 AWL 3.1-073 [1050719] Fix handling of an empty default namespace declaration.
// 07-28-04 AWL 3.1-069 [1043286] Preflight parse buffers for ASCII controls.
// 07-26-04 AWL 3.1-068 [1046381,1046384] Fix XMPUtils::ConvertToXyz to throw instead of assert for
//				null or empty string. Fix XMPMeta::GetProperty_Xyz to make sure property is simple.
// 07-21-04 AWL 3.1-067 [1044036] Increase fudge factor for output reserve in SerializeToBuffer.
// 07-15-04 AWL 3.1-066 [1015904] Fix Get/SetLocalizedText to allow empty alt arrays.
// 07-14-04 AWL 3.1-062 [1026373] Strip old iX:changes elements during parsing.
// 07-14-04 AWL 3.1-059 [0658992] Fix SetQualifier to throw if property does not exist.
// 07-13-04 AWL 3.1-058 [1014255] Remove empty schema nodes.
// 07-13-04 AWL 3.1-057 [1006568] Check all XPath components for valid namespace prefixes.
// 07-09-04 AWL 3.1-053 [1037918] Use double escaping for ASCII controls other than tab, lf, and cr.
// 06-10-04 AWL 3.1-047 Fix more HPUX and AIX compilation errors and warnings.
// 06-04-04 AWL 3.1-046 Fix HPUX compilation errors and warnings.
//
// 05-25-04 AWL [1018426] Hide all use of cin/cout streams in #if DEBUG or equivalent.
// 05-14-04 AWL [1016811,1018220] Fix escaping bugs in serialization.
// 04-30-04 AWL Add new & delete operators that call BIBMemory functions. Change static objects that
//				require allocation to explicit pointers. Properly delete partial tree when parse
//				errors throw an exception. Clean up memory leaks.
// 04-19-04 AWL [1010249] Don't write a final newline after the packet trailer, the packet must end
//			    at the '>' of the trailer.
// 04-16-04 AWL Fix major bogosity in IsAttrQualifier XMPMeta.cpp.
// 04-16-04 AWL Inflate serialization size estimate to accommodate anticipated character entities,
//              e.g. &#xA; for newlines. There can be a lot in the base 64 encoding of a thumbnail.
// 03-17-04 AWL Cleanup error exceptions, make sure all have a reasonable message. Have GetProperty
//				and friends throw an exception if the schema namespace is not registered.
// 03-04-04 AWL Add hacks to handle the DC singleton denormalization of the XMP from Acrobat 5.
// 02-19-04 AWL Add the new "transient" standard namespace.
// 02-06-04 AWL Register a couple of missing standard namespaces.
// 01-29-04 AWL Add AppendArrayItem.
// 01-26-04 AWL Revamp expanded XPath handling to work better for aliases.
// 04-24-03	AWL	Initial start on the new implementation.
//
// =================================================================================================
#endif /* AdobePrivate */

#include "public/include/XMP_Environment.h"	// ! This must be the first include!

#if XMP_DebugBuild
	#include <iostream>
#endif

#include "XMPCore/source/XMPCore_Impl.hpp"

#include "XMPCore/source/XMPMeta.hpp"
#include "XMPCore/source/XMPIterator.hpp"
#include "XMPCore/source/XMPUtils.hpp"

#include "public/include/XMP_Version.h"
#if AdobePrivate
	#include "XMPCore/source/XMPCore_ChangeLog.hpp"
	#include "build/XMP_BuildInfo.h"
	#include "XMPCore/source/XMPDocOps.hpp"
#endif

#include "source/UnicodeInlines.incl_cpp"
#include "source/UnicodeConversions.hpp"
#include "source/ExpatAdapter.hpp"


#include "XMPCore/XMPCoreDefines.h"
#if ENABLE_CPP_DOM_MODEL
#include "third-party/zuid/interfaces/MD5.h"
#include "XMPCore/Interfaces/IMetadata_I.h"
#include "XMPCore/Interfaces/IPathSegment_I.h"
#include "XMPCore/Interfaces/IPath_I.h"
#include "XMPCore/Interfaces/INameSpacePrefixMap_I.h"
#include "XMPCore/Interfaces/IDOMImplementationRegistry_I.h"
#include "XMPCore/XMPCoreFwdDeclarations_I.h"
#include "XMPCore/XMPCoreFwdDeclarations.h"
#include "XMPCore/Interfaces/IStructureNode_I.h"
#include "XMPCore/Interfaces/ISimpleNode_I.h"
#include "XMPCore/Interfaces/IArrayNode_I.h"
#include "XMPCore/Interfaces/INodeIterator.h"
#include "XMPCore/Interfaces/ICoreObjectFactory.h"
#include "XMPCommon/Interfaces/IUTF8String_I.h"
#include "XMPCore/Interfaces/INode_I.h"
#endif 

using namespace std;

#if XMP_WinBuild
	#pragma warning ( disable : 4533 )	// initialization of '...' is skipped by 'goto ...'
	#pragma warning ( disable : 4702 )	// unreachable code
	#pragma warning ( disable : 4800 )	// forcing value to bool 'true' or 'false' (performance warning)
#endif



// *** Use the XMP_PropIsXyz (Schema, Simple, Struct, Array, ...) macros
// *** Add debug codegen checks, e.g. that typical masking operations really work
// *** Change all uses of strcmp and strncmp to XMP_LitMatch and XMP_LitNMatch


// =================================================================================================
// Local Types and Constants
// =========================

typedef unsigned char XMP_CLTMatch;

#if XMP_MARKER_EXTENSIBILITY_BACKWARD_COMPATIBILITY
extern "C" {
    
	 static void ReleaseXMP_Node(void * node) {
		if (node) {
			XMP_Node * ptr = (XMP_Node *)node;
			delete ptr;
			ptr = NULL;
		}
	}
}

#if ENABLE_CPP_DOM_MODEL
extern "C" {
    
	static void ReleaseIStructureNode(void * node) {
		if (node) {
			AdobeXMPCore::pIStructureNode_base ptr = ( AdobeXMPCore::pIStructureNode_base )node;
			ptr->Release();
			node = NULL;
		}
	}
}
#endif
#endif

enum {	// Values for XMP_CLTMatch.
	kXMP_CLT_NoValues,
	kXMP_CLT_SpecificMatch,
	kXMP_CLT_SingleGeneric,
	kXMP_CLT_MultipleGeneric,
	kXMP_CLT_XDefault,
	kXMP_CLT_FirstItem
};


// =================================================================================================
// Static Variables
// ================


// =================================================================================================
// Local Utilities
// ===============


// -------------------------------------------------------------------------------------------------
// SetNodeValue
// ------------
static inline void
	SetNodeValue ( XMP_Node * node, XMP_StringPtr value )
{
	node->SetValue( value );
}	//SetNodeValue

void XMP_Node::SetValue( XMP_StringPtr _value )
{

	#if XMP_DebugBuild	// ! Hack to force an assert.
		if ( (this->name == "xmp:TestAssertNotify") && XMP_LitMatch ( _value, "DoIt!" ) ) {
			XMP_Assert ( this->name != "xmp:TestAssertNotify" );
		}
	#endif
	
	std::string newValue = _value;	// Need a local copy to tweak and not change node.value for errors.
	
	XMP_Uns8* chPtr = (XMP_Uns8*) newValue.c_str();	// Check for valid UTF-8, replace ASCII controls with a space.
	while ( *chPtr != 0 ) {

		while ( (*chPtr != 0) && (*chPtr < 0x80) ) {
			if ( *chPtr < 0x20 ) {
				if ( (*chPtr != kTab) && (*chPtr != kLF) && (*chPtr != kCR) ) *chPtr = 0x20;
			} else if (*chPtr == 0x7F ) {
				*chPtr = 0x20;
			}
			++chPtr;
		}

		XMP_Assert ( (*chPtr == 0) || (*chPtr >= 0x80) );

		if ( *chPtr != 0 ) {
			XMP_Uns32 cp = GetCodePoint ( (const XMP_Uns8 **) &chPtr );	// Throws for bad UTF-8.
			if ( (cp == 0xFFFE) || (cp == 0xFFFF) ) {
				XMP_Throw ( "U+FFFE and U+FFFF are not allowed in XML", kXMPErr_BadUnicode );
			}
		}

	}

	if ( XMP_PropIsQualifier(this->options) && (this->name == "xml:lang") ) NormalizeLangValue ( &newValue );

	this->value.swap ( newValue );

	#if 0	// *** XMP_DebugBuild
		this->_valuePtr = this->value.c_str();
	#endif
	
}	// XMP_Node::SetValue


// -------------------------------------------------------------------------------------------------
// SetNode
// -------
//
// The internals for SetProperty and related calls, used after the node is found or created.

static void
SetNode	( XMP_Node * node, XMP_StringPtr value, XMP_OptionBits options )
{
	if ( options & kXMP_DeleteExisting ) {
		XMP_ClearOption ( options, kXMP_DeleteExisting );
		node->options = options;
		node->value.erase();
		node->RemoveChildren();
		node->RemoveQualifiers();
	}
	
	node->options |= options;	// Keep options set by FindNode when creating a new node.

	if ( value != 0 ) {
	
		// This is setting the value of a leaf node.
		if ( node->options & kXMP_PropCompositeMask ) XMP_Throw ( "Composite nodes can't have values", kXMPErr_BadXPath );
		XMP_Assert ( node->children.empty() );
		SetNodeValue ( node, value );
	
	} else {
	
		// This is setting up an array or struct.
		if ( ! node->value.empty() ) XMP_Throw ( "Composite nodes can't have values", kXMPErr_BadXPath );
		if ( node->options & kXMP_PropCompositeMask ) {	// Can't change an array to a struct, or vice versa.
			if ( (options & kXMP_PropCompositeMask) != (node->options & kXMP_PropCompositeMask) ) {
				XMP_Throw ( "Requested and existing composite form mismatch", kXMPErr_BadXPath );
			}
		}
		node->RemoveChildren();
	
	}
	
}	// SetNode


// -------------------------------------------------------------------------------------------------
// DoSetArrayItem
// --------------

static void
DoSetArrayItem ( XMP_Node *		arrayNode,
				 XMP_Index		itemIndex,
				 XMP_StringPtr	itemValue,
				 XMP_OptionBits options )
{
	XMP_OptionBits itemLoc = options & kXMP_PropArrayLocationMask;
	XMP_Index      arraySize = static_cast<XMP_Index>( arrayNode->children.size() );
	
	options &= ~kXMP_PropArrayLocationMask;
	options = VerifySetOptions ( options, itemValue );
	
	// Now locate or create the item node and set the value. Note the index parameter is one-based!
	// The index can be in the range [0..size+1] or "last", normalize it and check the insert flags.
	// The order of the normalization checks is important. If the array is empty we end up with an
	// index and location to set item size+1.
	
	XMP_Node * itemNode = 0;
	
	if ( itemIndex == kXMP_ArrayLastItem ) itemIndex = arraySize;
	if ( (itemIndex == 0) && (itemLoc == kXMP_InsertAfterItem) ) {
		itemIndex = 1;
		itemLoc = kXMP_InsertBeforeItem;
	}
	if ( (itemIndex == arraySize) && (itemLoc == kXMP_InsertAfterItem) ) {
		itemIndex += 1;
		itemLoc = 0;
	}
	if ( (itemIndex == arraySize+1) && (itemLoc == kXMP_InsertBeforeItem) ) itemLoc = 0;
	
	if ( itemIndex == arraySize+1 ) {

		if ( itemLoc != 0 ) XMP_Throw ( "Can't insert before or after implicit new item", kXMPErr_BadIndex );
		itemNode = new XMP_Node ( arrayNode, kXMP_ArrayItemName, 0 );
		arrayNode->children.push_back ( itemNode );

	} else {

		if ( (itemIndex < 1) || (itemIndex > arraySize) ) XMP_Throw ( "Array index out of bounds", kXMPErr_BadIndex );
		--itemIndex;	// ! Convert the index to a C zero-based value!
		if ( itemLoc == 0 ) {
			itemNode = arrayNode->children[itemIndex];
		} else {
			XMP_NodePtrPos itemPos = arrayNode->children.begin() + itemIndex;
			if ( itemLoc == kXMP_InsertAfterItem ) ++itemPos;
			itemNode = new XMP_Node ( arrayNode, kXMP_ArrayItemName, 0 );
			itemPos = arrayNode->children.insert ( itemPos, itemNode );
		}

	}
	
	SetNode ( itemNode, itemValue, options );
	
}	// DoSetArrayItem


// -------------------------------------------------------------------------------------------------
// ChooseLocalizedText
// -------------------
//
// 1. Look for an exact match with the specific language.
// 2. If a generic language is given, look for partial matches.
// 3. Look for an "x-default" item.
// 4. Choose the first item.

static XMP_CLTMatch
ChooseLocalizedText ( const XMP_Node *	 arrayNode,
					  XMP_StringPtr		 genericLang,
					  XMP_StringPtr		 specificLang,
					  const XMP_Node * * itemNode )
{
	const XMP_Node * currItem = 0;
	const size_t itemLim = arrayNode->children.size();
	size_t itemNum;
	
	// See if the array has the right form. Allow empty alt arrays, that is what parsing returns.
	// *** Should check alt-text bit when that is reliably maintained.

	if ( ! ( XMP_ArrayIsAltText(arrayNode->options) ||
	         (arrayNode->children.empty() && XMP_ArrayIsAlternate(arrayNode->options)) ) ) {
		XMP_Throw ( "Localized text array is not alt-text", kXMPErr_BadXPath );
	}
	if ( arrayNode->children.empty() ) {
		*itemNode = 0;
		return kXMP_CLT_NoValues;
	}

	for ( itemNum = 0; itemNum < itemLim; ++itemNum ) {
		currItem = arrayNode->children[itemNum];
		if ( currItem->options & kXMP_PropCompositeMask ) {
			XMP_Throw ( "Alt-text array item is not simple", kXMPErr_BadXPath );
		}
		if ( currItem->qualifiers.empty() || (currItem->qualifiers[0]->name != "xml:lang") ) {
			XMP_Throw ( "Alt-text array item has no language qualifier", kXMPErr_BadXPath );
		}
	}

	// Look for an exact match with the specific language.
	for ( itemNum = 0; itemNum < itemLim; ++itemNum ) {
		currItem = arrayNode->children[itemNum];
		if ( currItem->qualifiers[0]->value == specificLang ) {
			*itemNode = currItem;
			return kXMP_CLT_SpecificMatch;
		}
	}
	
	if ( *genericLang != 0 ) {

		// Look for the first partial match with the generic language.
		const size_t genericLen = strlen ( genericLang );
		for ( itemNum = 0; itemNum < itemLim; ++itemNum ) {
			currItem = arrayNode->children[itemNum];
			XMP_StringPtr currLang = currItem->qualifiers[0]->value.c_str();
			const size_t currLangSize = currItem->qualifiers[0]->value.size();
			if ( (currLangSize >= genericLen) &&
				 XMP_LitNMatch ( currLang, genericLang, genericLen ) &&
				 ((currLangSize == genericLen) || (currLang[genericLen] == '-')) ) {
				*itemNode = currItem;
				break;	// ! Don't return, need to look for other matches.
			}
		}

		if ( itemNum < itemLim ) {
			
			// Look for a second partial match with the generic language.
			for ( ++itemNum; itemNum < itemLim; ++itemNum ) {
				currItem = arrayNode->children[itemNum];
				XMP_StringPtr currLang = currItem->qualifiers[0]->value.c_str();
				const size_t currLangSize = currItem->qualifiers[0]->value.size();
				if ( (currLangSize >= genericLen) &&
					 XMP_LitNMatch ( currLang, genericLang, genericLen ) &&
					 ((currLangSize == genericLen) || (currLang[genericLen] == '-')) ) {
					return kXMP_CLT_MultipleGeneric;	// ! Leave itemNode with the first partial match.
				}
			}
			return kXMP_CLT_SingleGeneric;	// No second partial match was found.

		}
		
	}
	
	// Look for an 'x-default' item.
	for ( itemNum = 0; itemNum < itemLim; ++itemNum ) {
		currItem = arrayNode->children[itemNum];
		if ( currItem->qualifiers[0]->value == "x-default" ) {
			*itemNode = currItem;
			return kXMP_CLT_XDefault;
		}
	}
	
	// Everything failed, choose the first item.
	*itemNode = arrayNode->children[0];
	return kXMP_CLT_FirstItem;
	
}	// ChooseLocalizedText


// -------------------------------------------------------------------------------------------------
// AppendLangItem
// --------------

static void
AppendLangItem ( XMP_Node * arrayNode, XMP_StringPtr itemLang, XMP_StringPtr itemValue )
{
	XMP_Node * newItem  = new XMP_Node ( arrayNode, kXMP_ArrayItemName, (kXMP_PropHasQualifiers | kXMP_PropHasLang) );
	XMP_Node * langQual = new XMP_Node ( newItem, "xml:lang", kXMP_PropIsQualifier );
	
	try {	// ! Use SetNodeValue, not constructors above, to get the character checks.
		SetNodeValue ( newItem, itemValue );
		SetNodeValue ( langQual, itemLang );
	} catch (...) {
		delete newItem;
		delete langQual;
		throw;
	}
	
	newItem->qualifiers.push_back ( langQual );

	if ( (arrayNode->children.empty()) || (langQual->value != "x-default") ) {
		arrayNode->children.push_back ( newItem );
	} else {
		arrayNode->children.insert ( arrayNode->children.begin(), newItem );
	}

}	// AppendLangItem


// =================================================================================================
// Class Methods
// =============
//
//
// =================================================================================================


// -------------------------------------------------------------------------------------------------
// GetProperty
// -----------

bool
XMPMeta::GetProperty ( XMP_StringPtr	schemaNS,
					   XMP_StringPtr	propName,
					   XMP_StringPtr *	propValue,
					   XMP_StringLen *	valueSize,
					   XMP_OptionBits *	options ) const
{
	XMP_Assert ( (schemaNS != 0) && (propName != 0) );	// Enforced by wrapper.
	XMP_Assert ( (propValue != 0) && (valueSize != 0) && (options != 0) );	// Enforced by wrapper.

	XMP_ExpandedXPath expPath;
	ExpandXPath ( schemaNS, propName, &expPath );
	
	XMP_Node * propNode = FindConstNode ( &tree, expPath );
	if ( propNode == 0 ) return false;
	
	*propValue = propNode->value.c_str();
	*valueSize = static_cast<XMP_StringLen>( propNode->value.size() );
	*options   = propNode->options;
	
	return true;
	
}	// GetProperty


// -------------------------------------------------------------------------------------------------
// GetArrayItem
// ------------

bool
XMPMeta::GetArrayItem ( XMP_StringPtr	 schemaNS,
						XMP_StringPtr	 arrayName,
						XMP_Index		 itemIndex,
						XMP_StringPtr *	 itemValue,
						XMP_StringLen *  valueSize,
						XMP_OptionBits * options ) const
{
	XMP_Assert ( (schemaNS != 0) && (arrayName != 0) );	// Enforced by wrapper.
	XMP_Assert ( (itemValue != 0) && (options != 0) );	// Enforced by wrapper.

	// ! Special case check to make errors consistent if the array does not exist. The other array
	// ! functions and existing array here (empty or not) already throw.
	if ( (itemIndex <= 0) && (itemIndex != kXMP_ArrayLastItem) ) XMP_Throw ( "Array index must be larger than zero", kXMPErr_BadXPath );

	XMP_VarString itemPath;
	XMPUtils::ComposeArrayItemPath ( schemaNS, arrayName, itemIndex, &itemPath );
	return GetProperty ( schemaNS, itemPath.c_str(), itemValue, valueSize, options );

}	// GetArrayItem


// -------------------------------------------------------------------------------------------------
// GetStructField
// --------------

bool
XMPMeta::GetStructField	( XMP_StringPtr	   schemaNS,
						  XMP_StringPtr	   structName,
						  XMP_StringPtr	   fieldNS,
						  XMP_StringPtr	   fieldName,
						  XMP_StringPtr *  fieldValue,
						  XMP_StringLen *  valueSize,
						  XMP_OptionBits * options ) const
{
	XMP_Assert ( (schemaNS != 0) && (structName != 0) && (fieldNS != 0) && (fieldName != 0) );	// Enforced by wrapper.
	XMP_Assert ( (fieldValue != 0) && (options != 0) );	// Enforced by wrapper.

	XMP_VarString fieldPath;
	XMPUtils::ComposeStructFieldPath ( schemaNS, structName, fieldNS, fieldName, &fieldPath );
	return GetProperty ( schemaNS, fieldPath.c_str(), fieldValue, valueSize, options );

}	// GetStructField


// -------------------------------------------------------------------------------------------------
// GetQualifier
// ------------

bool
XMPMeta::GetQualifier ( XMP_StringPtr	 schemaNS,
						XMP_StringPtr	 propName,
						XMP_StringPtr	 qualNS,
						XMP_StringPtr	 qualName,
						XMP_StringPtr *	 qualValue,
						XMP_StringLen *  valueSize,
						XMP_OptionBits * options ) const
{
	XMP_Assert ( (schemaNS != 0) && (propName != 0) && (qualNS != 0) && (qualName != 0) );	// Enforced by wrapper.
	XMP_Assert ( (qualValue != 0) && (options != 0) );	// Enforced by wrapper.

	XMP_VarString qualPath;
	XMPUtils::ComposeQualifierPath ( schemaNS, propName, qualNS, qualName, &qualPath );
	return GetProperty ( schemaNS, qualPath.c_str(), qualValue, valueSize, options );

}	// GetQualifier


// -------------------------------------------------------------------------------------------------
// SetProperty
// -----------

// *** Should handle array items specially, calling SetArrayItem.

void
XMPMeta::SetProperty ( XMP_StringPtr  schemaNS,
					   XMP_StringPtr  propName,
					   XMP_StringPtr  propValue,
					   XMP_OptionBits options )
{
	XMP_Assert ( (schemaNS != 0) && (propName != 0) );	// Enforced by wrapper.

	options = VerifySetOptions ( options, propValue );

	XMP_ExpandedXPath expPath;
	ExpandXPath ( schemaNS, propName, &expPath );

	XMP_Node * propNode = FindNode ( &tree, expPath, kXMP_CreateNodes, options );
	if ( propNode == 0 ) XMP_Throw ( "Specified property does not exist", kXMPErr_BadXPath );
	
	SetNode ( propNode, propValue, options );
	
}	// SetProperty


// -------------------------------------------------------------------------------------------------
// SetArrayItem
// ------------

void
XMPMeta::SetArrayItem ( XMP_StringPtr  schemaNS,
						XMP_StringPtr  arrayName,
						XMP_Index	   itemIndex,
						XMP_StringPtr  itemValue,
						XMP_OptionBits options )
{
	XMP_Assert ( (schemaNS != 0) && (arrayName != 0) );	// Enforced by wrapper.

	XMP_ExpandedXPath arrayPath;
	ExpandXPath ( schemaNS, arrayName, &arrayPath );
	XMP_Node * arrayNode = FindNode ( &tree, arrayPath, kXMP_ExistingOnly );	// Just lookup, don't try to create.
	if ( arrayNode == 0 ) XMP_Throw ( "Specified array does not exist", kXMPErr_BadXPath );
	
	DoSetArrayItem ( arrayNode, itemIndex, itemValue, options );
	
}	// SetArrayItem


// -------------------------------------------------------------------------------------------------
// AppendArrayItem
// ---------------

void
XMPMeta::AppendArrayItem ( XMP_StringPtr  schemaNS,
						   XMP_StringPtr  arrayName,
						   XMP_OptionBits arrayOptions,
						   XMP_StringPtr  itemValue,
						   XMP_OptionBits options )
{
	XMP_Assert ( (schemaNS != 0) && (arrayName != 0) );	// Enforced by wrapper.

	arrayOptions = VerifySetOptions ( arrayOptions, 0 );
	if ( (arrayOptions & ~kXMP_PropArrayFormMask) != 0 ) {
		XMP_Throw ( "Only array form flags allowed for arrayOptions", kXMPErr_BadOptions );
	}
	
	// Locate or create the array. If it already exists, make sure the array form from the options
	// parameter is compatible with the current state.
	
	XMP_ExpandedXPath arrayPath;
	ExpandXPath ( schemaNS, arrayName, &arrayPath );
	XMP_Node * arrayNode = FindNode ( &tree, arrayPath, kXMP_ExistingOnly );	// Just lookup, don't try to create.
	
	if ( arrayNode != 0 ) {
		// The array exists, make sure the form is compatible. Zero arrayForm means take what exists.
		if ( ! (arrayNode->options & kXMP_PropValueIsArray) ) {
			XMP_Throw ( "The named property is not an array", kXMPErr_BadXPath );
		}
		#if 0
			// *** Disable for now. Need to do some general rethinking of semantic checks.
			if ( (arrayOptions != 0) && (arrayOptions != (arrayNode->options & kXMP_PropArrayFormMask)) ) {
				XMP_Throw ( "Mismatch of existing and specified array form", kXMPErr_BadOptions );
			}
		#endif
	} else {
		// The array does not exist, try to create it.
		if ( arrayOptions == 0 ) XMP_Throw ( "Explicit arrayOptions required to create new array", kXMPErr_BadOptions );
		arrayNode = FindNode ( &tree, arrayPath, kXMP_CreateNodes, arrayOptions );
		if ( arrayNode == 0 ) XMP_Throw ( "Failure creating array node", kXMPErr_BadXPath );
	}
	
	DoSetArrayItem ( arrayNode, kXMP_ArrayLastItem, itemValue, (options | kXMP_InsertAfterItem) );
	
}	// AppendArrayItem


// -------------------------------------------------------------------------------------------------
// SetStructField
// --------------

void
XMPMeta::SetStructField	( XMP_StringPtr	 schemaNS,
						  XMP_StringPtr	 structName,
						  XMP_StringPtr	 fieldNS,
						  XMP_StringPtr	 fieldName,
						  XMP_StringPtr	 fieldValue,
						  XMP_OptionBits options )
{
	XMP_Assert ( (schemaNS != 0) && (structName != 0) && (fieldNS != 0) && (fieldName != 0) );	// Enforced by wrapper.

	XMP_VarString fieldPath;
	XMPUtils::ComposeStructFieldPath ( schemaNS, structName, fieldNS, fieldName, &fieldPath );
	SetProperty ( schemaNS, fieldPath.c_str(), fieldValue, options );

}	// SetStructField


// -------------------------------------------------------------------------------------------------
// SetQualifier
// ------------

void
XMPMeta::SetQualifier ( XMP_StringPtr  schemaNS,
						XMP_StringPtr  propName,
						XMP_StringPtr  qualNS,
						XMP_StringPtr  qualName,
						XMP_StringPtr  qualValue,
						XMP_OptionBits options )
{
	XMP_Assert ( (schemaNS != 0) && (propName != 0) && (qualNS != 0) && (qualName != 0) );	// Enforced by wrapper.

	XMP_ExpandedXPath expPath;
	ExpandXPath ( schemaNS, propName, &expPath );
	XMP_Node * propNode = FindNode ( &tree, expPath, kXMP_ExistingOnly );
	if ( propNode == 0 ) XMP_Throw ( "Specified property does not exist", kXMPErr_BadXPath );

	XMP_VarString qualPath;
	XMPUtils::ComposeQualifierPath ( schemaNS, propName, qualNS, qualName, &qualPath );
	SetProperty ( schemaNS, qualPath.c_str(), qualValue, options );

}	// SetQualifier


// -------------------------------------------------------------------------------------------------
// DeleteProperty
// --------------

void
XMPMeta::DeleteProperty	( XMP_StringPtr	schemaNS,
						  XMP_StringPtr	propName )
{
	XMP_Assert ( (schemaNS != 0) && (propName != 0) );	// Enforced by wrapper.

	XMP_ExpandedXPath	expPath;
	ExpandXPath ( schemaNS, propName, &expPath );
	
	XMP_NodePtrPos ptrPos;
	XMP_Node * propNode = FindNode ( &tree, expPath, kXMP_ExistingOnly, kXMP_NoOptions, &ptrPos );
	if ( propNode == 0 ) return;
	XMP_Node * parentNode = propNode->parent;
	
	// Erase the pointer from the parent's vector, then delete the node and all below it.
	
	if ( ! (propNode->options & kXMP_PropIsQualifier) ) {

		parentNode->children.erase ( ptrPos );
		DeleteEmptySchema ( parentNode );

	} else {

		if ( propNode->name == "xml:lang" ) {
			XMP_Assert ( parentNode->options & kXMP_PropHasLang );	// *** &= ~flag would be safer
			parentNode->options ^= kXMP_PropHasLang;
		} else if ( propNode->name == "rdf:type" ) {
			XMP_Assert ( parentNode->options & kXMP_PropHasType );
			parentNode->options ^= kXMP_PropHasType;
		}

		parentNode->qualifiers.erase ( ptrPos );
		XMP_Assert ( parentNode->options & kXMP_PropHasQualifiers );
		if ( parentNode->qualifiers.empty() ) parentNode->options ^= kXMP_PropHasQualifiers;

	}
	
	delete propNode;	// ! The destructor takes care of the whole subtree.
	
}	// DeleteProperty


// -------------------------------------------------------------------------------------------------
// DeleteArrayItem
// ---------------

void
XMPMeta::DeleteArrayItem ( XMP_StringPtr schemaNS,
						   XMP_StringPtr arrayName,
						   XMP_Index	 itemIndex )
{
	XMP_Assert ( (schemaNS != 0) && (arrayName != 0) );	// Enforced by wrapper.

	XMP_VarString itemPath;
	XMPUtils::ComposeArrayItemPath ( schemaNS, arrayName, itemIndex, &itemPath );
	DeleteProperty ( schemaNS, itemPath.c_str() );

}	// DeleteArrayItem


// -------------------------------------------------------------------------------------------------
// DeleteStructField
// -----------------

void
XMPMeta::DeleteStructField ( XMP_StringPtr schemaNS,
							 XMP_StringPtr structName,
							 XMP_StringPtr fieldNS,
							 XMP_StringPtr fieldName )
{
	XMP_Assert ( (schemaNS != 0) && (structName != 0) && (fieldNS != 0) && (fieldName != 0) );	// Enforced by wrapper.

	XMP_VarString fieldPath;
	XMPUtils::ComposeStructFieldPath ( schemaNS, structName, fieldNS, fieldName, &fieldPath );
	DeleteProperty ( schemaNS, fieldPath.c_str() );

}	// DeleteStructField


// -------------------------------------------------------------------------------------------------
// DeleteQualifier
// ---------------

void
XMPMeta::DeleteQualifier ( XMP_StringPtr schemaNS,
						   XMP_StringPtr propName,
						   XMP_StringPtr qualNS,
						   XMP_StringPtr qualName )
{
	XMP_Assert ( (schemaNS != 0) && (propName != 0) && (qualNS != 0) && (qualName != 0) );	// Enforced by wrapper.

	XMP_VarString qualPath;
	XMPUtils::ComposeQualifierPath ( schemaNS, propName, qualNS, qualName, &qualPath );
	DeleteProperty ( schemaNS, qualPath.c_str() );

}	// DeleteQualifier


// -------------------------------------------------------------------------------------------------
// DoesPropertyExist
// -----------------

bool
XMPMeta::DoesPropertyExist ( XMP_StringPtr schemaNS,
							 XMP_StringPtr propName ) const
{
	XMP_Assert ( (schemaNS != 0) && (propName != 0) );	// Enforced by wrapper.

	XMP_ExpandedXPath	expPath;
	ExpandXPath ( schemaNS, propName, &expPath );

	XMP_Node * propNode = FindConstNode ( &tree, expPath );
	return (propNode != 0);
	
}	// DoesPropertyExist


// -------------------------------------------------------------------------------------------------
// DoesArrayItemExist
// ------------------

bool
XMPMeta::DoesArrayItemExist	( XMP_StringPtr	schemaNS,
							  XMP_StringPtr	arrayName,
							  XMP_Index		itemIndex ) const
{
	XMP_Assert ( (schemaNS != 0) && (arrayName != 0) );	// Enforced by wrapper.

	XMP_VarString itemPath;
	XMPUtils::ComposeArrayItemPath ( schemaNS, arrayName, itemIndex, &itemPath );
	return DoesPropertyExist ( schemaNS, itemPath.c_str() );

}	// DoesArrayItemExist


// -------------------------------------------------------------------------------------------------
// DoesStructFieldExist
// --------------------

bool
XMPMeta::DoesStructFieldExist ( XMP_StringPtr schemaNS,
								XMP_StringPtr structName,
								XMP_StringPtr fieldNS,
								XMP_StringPtr fieldName ) const
{
	XMP_Assert ( (schemaNS != 0) && (structName != 0) && (fieldNS != 0) && (fieldName != 0) );	// Enforced by wrapper.

	XMP_VarString fieldPath;
	XMPUtils::ComposeStructFieldPath ( schemaNS, structName, fieldNS, fieldName, &fieldPath );
	return DoesPropertyExist ( schemaNS, fieldPath.c_str() );

}	// DoesStructFieldExist


// -------------------------------------------------------------------------------------------------
// DoesQualifierExist
// ------------------

bool
XMPMeta::DoesQualifierExist	( XMP_StringPtr	schemaNS,
							  XMP_StringPtr	propName,
							  XMP_StringPtr	qualNS,
							  XMP_StringPtr	qualName ) const
{
	XMP_Assert ( (schemaNS != 0) && (propName != 0) && (qualNS != 0) && (qualName != 0) );	// Enforced by wrapper.

	XMP_VarString qualPath;
	XMPUtils::ComposeQualifierPath ( schemaNS, propName, qualNS, qualName, &qualPath );
	return DoesPropertyExist ( schemaNS, qualPath.c_str() );

}	// DoesQualifierExist


// -------------------------------------------------------------------------------------------------
// GetLocalizedText
// ----------------

bool
XMPMeta::GetLocalizedText ( XMP_StringPtr	 schemaNS,
							XMP_StringPtr	 arrayName,
							XMP_StringPtr	 _genericLang,
							XMP_StringPtr	 _specificLang,
							XMP_StringPtr *	 actualLang,
							XMP_StringLen *	 langSize,
							XMP_StringPtr *	 itemValue,
							XMP_StringLen *	 valueSize,
							XMP_OptionBits * options ) const
{
	XMP_Assert ( (schemaNS != 0) && (arrayName != 0) && (_genericLang != 0) && (_specificLang != 0) );	// Enforced by wrapper.
	XMP_Assert ( (actualLang != 0) && (langSize != 0) );	// Enforced by wrapper.
	XMP_Assert ( (itemValue != 0) && (valueSize != 0) && (options != 0) );	// Enforced by wrapper.

	XMP_VarString zGenericLang  ( _genericLang );
	XMP_VarString zSpecificLang ( _specificLang );
	NormalizeLangValue ( &zGenericLang );
	NormalizeLangValue ( &zSpecificLang );
	
	XMP_StringPtr genericLang  = zGenericLang.c_str();
	XMP_StringPtr specificLang = zSpecificLang.c_str();
	
	XMP_ExpandedXPath arrayPath;
	ExpandXPath ( schemaNS, arrayName, &arrayPath );
	
	const XMP_Node * arrayNode = FindConstNode ( &tree, arrayPath );	// *** This expand/find idiom is used in 3 Getters.
	if ( arrayNode == 0 ) return false;			// *** Should extract it into a local utility.
	
	XMP_CLTMatch match;
	const XMP_Node * itemNode;
	
	match = ChooseLocalizedText ( arrayNode, genericLang, specificLang, &itemNode );
	if ( match == kXMP_CLT_NoValues ) return false;
	
	*actualLang = itemNode->qualifiers[0]->value.c_str();
	*langSize   = static_cast<XMP_Index>( itemNode->qualifiers[0]->value.size() );
	*itemValue  = itemNode->value.c_str();
	*valueSize  = static_cast<XMP_Index>( itemNode->value.size() );
	*options    = itemNode->options;

	return true;
	
}	// GetLocalizedText


// -------------------------------------------------------------------------------------------------
// SetLocalizedText
// ----------------

void
XMPMeta::SetLocalizedText ( XMP_StringPtr  schemaNS,
							XMP_StringPtr  arrayName,
							XMP_StringPtr  _genericLang,
							XMP_StringPtr  _specificLang,
							XMP_StringPtr  itemValue,
							XMP_OptionBits options )
{
	IgnoreParam(options);

	XMP_Assert ( (schemaNS != 0) && (arrayName != 0) && (_genericLang != 0) && (_specificLang != 0) );	// Enforced by wrapper.

	XMP_VarString zGenericLang  ( _genericLang );
	XMP_VarString zSpecificLang ( _specificLang );
	NormalizeLangValue ( &zGenericLang );
	NormalizeLangValue ( &zSpecificLang );
	
	XMP_StringPtr genericLang  = zGenericLang.c_str();
	XMP_StringPtr specificLang = zSpecificLang.c_str();
	
	XMP_ExpandedXPath arrayPath;
	ExpandXPath ( schemaNS, arrayName, &arrayPath );
	
	// Find the array node and set the options if it was just created.
	XMP_Node * arrayNode = FindNode ( &tree, arrayPath, kXMP_CreateNodes,
									  (kXMP_PropValueIsArray | kXMP_PropArrayIsOrdered | kXMP_PropArrayIsAlternate) );
	if ( arrayNode == 0 ) XMP_Throw ( "Failed to find or create array node", kXMPErr_BadXPath );
	if ( ! XMP_ArrayIsAltText(arrayNode->options) ) {
		if ( arrayNode->children.empty() && XMP_ArrayIsAlternate(arrayNode->options) ) {
			arrayNode->options |= kXMP_PropArrayIsAltText;
		} else {
			XMP_Throw ( "Localized text array is not alt-text", kXMPErr_BadXPath );
		}
	}
	
	// Make sure the x-default item, if any, is first.
	
	size_t itemNum, itemLim;
	XMP_Node * xdItem = 0;
	bool haveXDefault = false;
	
	for ( itemNum = 0, itemLim = arrayNode->children.size(); itemNum < itemLim; ++itemNum ) {
		XMP_Node * currItem = arrayNode->children[itemNum];
		XMP_Assert ( XMP_PropHasLang(currItem->options) );
		if ( currItem->qualifiers.empty() || (currItem->qualifiers[0]->name != "xml:lang") ) {
			XMP_Throw ( "Language qualifier must be first", kXMPErr_BadXPath );
		}
		if ( currItem->qualifiers[0]->value == "x-default" ) {
			xdItem = currItem;
			haveXDefault = true;
			break;
		}
	}
	
	if ( haveXDefault && (itemNum != 0) ) {
		XMP_Assert ( arrayNode->children[itemNum]->qualifiers[0]->value == "x-default" );
		XMP_Node * temp = arrayNode->children[0];
		arrayNode->children[0] = arrayNode->children[itemNum];
		arrayNode->children[itemNum] = temp;
	}
	
	// Find the appropriate item. ChooseLocalizedText will make sure the array is a language alternative.
		
	const XMP_Node * cItemNode;	// ! ChooseLocalizedText returns a pointer to a const node.
	XMP_CLTMatch match = ChooseLocalizedText ( arrayNode, genericLang, specificLang, &cItemNode );
	XMP_Node * itemNode = const_cast<XMP_Node*> ( cItemNode );

	const bool specificXDefault = XMP_LitMatch ( specificLang, "x-default" );
	
	switch ( match ) {

		case kXMP_CLT_NoValues :

			// Create the array items for the specificLang and x-default, with x-default first.
			AppendLangItem ( arrayNode, "x-default", itemValue );
			haveXDefault = true;
			if ( ! specificXDefault ) AppendLangItem ( arrayNode, specificLang, itemValue );
			break;
			
		case kXMP_CLT_SpecificMatch :
		
			if ( ! specificXDefault ) {
				// Update the specific item, update x-default if it matches the old value.
				if ( xdItem != NULL && haveXDefault && (xdItem != itemNode) && (xdItem->value == itemNode->value) ) {
					SetNodeValue ( xdItem, itemValue );
				}
				SetNodeValue ( itemNode, itemValue );	// ! Do this after the x-default check!
			} else {
				// Update all items whose values match the old x-default value.
				XMP_Assert ( xdItem != NULL && haveXDefault && (xdItem == itemNode) );
				for ( itemNum = 0, itemLim = arrayNode->children.size(); itemNum < itemLim; ++itemNum ) {
					XMP_Node * currItem = arrayNode->children[itemNum];
					if ( (currItem == xdItem) || (currItem->value != xdItem->value) ) continue;
					SetNodeValue ( currItem, itemValue );
				}
				SetNodeValue ( xdItem, itemValue );	// And finally do the x-default item.
			}
			break;

		case kXMP_CLT_SingleGeneric :
		
			// Update the generic item, update x-default if it matches the old value.
			if (  xdItem != NULL && haveXDefault && (xdItem != itemNode) && (xdItem->value == itemNode->value) ) {
				SetNodeValue ( xdItem, itemValue );
			}
			SetNodeValue ( itemNode, itemValue );	// ! Do this after the x-default check!
			break;

		case kXMP_CLT_MultipleGeneric :
		
			// Create the specific language, ignore x-default.
			AppendLangItem ( arrayNode, specificLang, itemValue );
			if ( specificXDefault ) haveXDefault = true;
			break;
			
		case kXMP_CLT_XDefault :

			// Create the specific language, update x-default if it was the only item.
			if ( arrayNode->children.size() == 1 ) SetNodeValue ( xdItem, itemValue );
			AppendLangItem ( arrayNode, specificLang, itemValue );
			break;

		case kXMP_CLT_FirstItem	:

			// Create the specific language, don't add an x-default item.
			AppendLangItem ( arrayNode, specificLang, itemValue );
			if ( specificXDefault ) haveXDefault = true;
			break;
			
		default :
			XMP_Throw ( "Unexpected result from ChooseLocalizedText", kXMPErr_InternalFailure );

	}

	// Add an x-default at the front if needed.
	if ( (! haveXDefault) && (arrayNode->children.size() == 1) ) {
		AppendLangItem ( arrayNode, "x-default", itemValue );
	}

}	// SetLocalizedText

// -------------------------------------------------------------------------------------------------
// DeleteLocalizedText
// -------------------

void
XMPMeta::DeleteLocalizedText ( XMP_StringPtr schemaNS,
                               XMP_StringPtr arrayName,
                               XMP_StringPtr _genericLang,
                               XMP_StringPtr _specificLang )
{
	XMP_Assert ( (schemaNS != 0) && (arrayName != 0) && (_genericLang != 0) && (_specificLang != 0) );	// Enforced by wrapper.

	XMP_VarString zGenericLang  ( _genericLang );
	XMP_VarString zSpecificLang ( _specificLang );
	NormalizeLangValue ( &zGenericLang );
	NormalizeLangValue ( &zSpecificLang );

	XMP_StringPtr genericLang  = zGenericLang.c_str();
	XMP_StringPtr specificLang = zSpecificLang.c_str();

	XMP_ExpandedXPath arrayPath;
	ExpandXPath ( schemaNS, arrayName, &arrayPath );
	
	// Find the LangAlt array and the selected array item.

	XMP_Node * arrayNode = FindNode ( &tree, arrayPath, kXMP_ExistingOnly );
	if ( arrayNode == 0 ) return;
	size_t arraySize = arrayNode->children.size();

	XMP_CLTMatch match;
	XMP_Node * itemNode;

	match = ChooseLocalizedText ( arrayNode, genericLang, specificLang, (const XMP_Node **) &itemNode );
	if ( match != kXMP_CLT_SpecificMatch ) return;

	size_t itemIndex = 0;
	for ( ; itemIndex < arraySize; ++itemIndex ) {
		if ( arrayNode->children[itemIndex] == itemNode ) break;
	}
	XMP_Enforce ( itemIndex < arraySize );
	
	// Decide if the selected item is x-default or not, find relevant matching item.
	
	bool itemIsXDefault = false;
	if ( ! itemNode->qualifiers.empty() ) {
		XMP_Node * qualNode = itemNode->qualifiers[0];
		if ( (qualNode->name == "xml:lang") && (qualNode->value == "x-default") ) itemIsXDefault = true;
	}
	
	if ( itemIsXDefault && (itemIndex != 0) ) {	// Enforce the x-default is first policy.
		XMP_Node * temp = arrayNode->children[0];
		arrayNode->children[0] = arrayNode->children[itemIndex];
		arrayNode->children[itemIndex] = temp;
		itemIndex = 0;
	}
	
	XMP_Node * assocNode = 0;
	size_t assocIndex;
	size_t assocIsXDefault = false;
	
	if ( itemIsXDefault ) {

		for ( assocIndex = 1; assocIndex < arraySize; ++assocIndex ) {
			if ( arrayNode->children[assocIndex]->value == itemNode->value ) {
				assocNode = arrayNode->children[assocIndex];
				break;
			}
		}

	} else if ( itemIndex > 0 ) {

		XMP_Node * itemZero = arrayNode->children[0];
		if ( itemZero->value == itemNode->value ) {
			XMP_Node * qualNode = itemZero->qualifiers[0];
			if ( (qualNode->name == "xml:lang") && (qualNode->value == "x-default") ) {
				assocNode = arrayNode->children[0];
				assocIndex = 0;
				assocIsXDefault = true;
			}
		}

	}
	
	// Delete the appropriate nodes.
	
	XMP_NodePtrPos arrayBegin = arrayNode->children.begin();
	
	if ( assocNode == 0 ) {
		arrayNode->children.erase ( arrayBegin + itemIndex );
	} else if ( itemIndex < assocIndex ) {
		arrayNode->children.erase ( arrayBegin + assocIndex );
		arrayNode->children.erase ( arrayBegin + itemIndex );
	} else {
		arrayNode->children.erase ( arrayBegin + itemIndex );
		arrayNode->children.erase ( arrayBegin + assocIndex );
	}

	delete itemNode;
	if ( assocNode != 0 ) delete assocNode;

}	// DeleteLocalizedText

// -------------------------------------------------------------------------------------------------
// GetProperty_Bool
// ----------------

bool
XMPMeta::GetProperty_Bool ( XMP_StringPtr	 schemaNS,
							XMP_StringPtr	 propName,
							bool *			 propValue,
							XMP_OptionBits * options ) const
{
	XMP_Assert ( (schemaNS != 0) && (propName != 0) );	// Enforced by wrapper.
	XMP_Assert ( (propValue != 0) && (options != 0) );	// Enforced by wrapper.

	XMP_StringPtr	valueStr;
	XMP_StringLen	valueLen;
	
	bool found = GetProperty ( schemaNS, propName, &valueStr, &valueLen, options );
	if ( found ) {
		if ( ! XMP_PropIsSimple ( *options ) ) XMP_Throw ( "Property must be simple", kXMPErr_BadXPath );
		*propValue = XMPUtils::ConvertToBool ( valueStr );
	}
	return found;
	
}	// GetProperty_Bool


// -------------------------------------------------------------------------------------------------
// GetProperty_Int
// ---------------

bool
XMPMeta::GetProperty_Int ( XMP_StringPtr	schemaNS,
						   XMP_StringPtr	propName,
						   XMP_Int32 *		propValue,
						   XMP_OptionBits *	options ) const
{
	XMP_Int64 tempValue64 = 0;
	if ( GetProperty_Int64( schemaNS, propName, &tempValue64, options ) ) {
		if ( tempValue64 < (XMP_Int64) Min_XMP_Int32 || tempValue64 > (XMP_Int64) Max_XMP_Int32 ) {
			// overflow condition
			XMP_Throw ( "Overflow condition", kXMPErr_BadValue );
		} else {
			*propValue = (XMP_Int32) tempValue64;
			return true;
		}
	}
	return false;

}	// GetProperty_Int


// -------------------------------------------------------------------------------------------------
// GetProperty_Int64
// -----------------

bool
XMPMeta::GetProperty_Int64 ( XMP_StringPtr	  schemaNS,
						     XMP_StringPtr	  propName,
						     XMP_Int64 *	  propValue,
						     XMP_OptionBits * options ) const
{
	XMP_Assert ( (schemaNS != 0) && (propName != 0) );	// Enforced by wrapper.
	XMP_Assert ( (propValue != 0) && (options != 0) );	// Enforced by wrapper.

	XMP_StringPtr	valueStr;
	XMP_StringLen	valueLen;
	
	bool found = GetProperty ( schemaNS, propName, &valueStr, &valueLen, options );
	if ( found ) {
		if ( ! XMP_PropIsSimple ( *options ) ) XMP_Throw ( "Property must be simple", kXMPErr_BadXPath );
		std::string propValueStr;
		propValueStr.append( valueStr, valueLen );
		XMPUtils::Trim( propValueStr );
		*propValue = XMPUtils::ConvertToInt64 ( propValueStr.c_str() );
	}
	return found;
	
}	// GetProperty_Int64


// -------------------------------------------------------------------------------------------------
// GetProperty_Float
// -----------------

bool
XMPMeta::GetProperty_Float ( XMP_StringPtr	  schemaNS,
							 XMP_StringPtr	  propName,
							 double *		  propValue,
							 XMP_OptionBits * options ) const
{
	XMP_Assert ( (schemaNS != 0) && (propName != 0) );	// Enforced by wrapper.
	XMP_Assert ( (propValue != 0) && (options != 0) );	// Enforced by wrapper.

	XMP_StringPtr	valueStr;
	XMP_StringLen	valueLen;
	
	bool found = GetProperty ( schemaNS, propName, &valueStr, &valueLen, options );
	if ( found ) {
		if ( ! XMP_PropIsSimple ( *options ) ) XMP_Throw ( "Property must be simple", kXMPErr_BadXPath );
		std::string propValueStr;
		propValueStr.append( valueStr, valueLen );
		XMPUtils::Trim( propValueStr );
		*propValue = XMPUtils::ConvertToFloat ( propValueStr.c_str() );
	}
	return found;
	
}	// GetProperty_Float


// -------------------------------------------------------------------------------------------------
// GetProperty_Date
// ----------------

bool
XMPMeta::GetProperty_Date ( XMP_StringPtr	 schemaNS,
							XMP_StringPtr	 propName,
							XMP_DateTime *	 propValue,
							XMP_OptionBits * options ) const
{
	XMP_Assert ( (schemaNS != 0) && (propName != 0) );	// Enforced by wrapper.
	XMP_Assert ( (propValue != 0) && (options != 0) );	// Enforced by wrapper.

	XMP_StringPtr	valueStr;
	XMP_StringLen	valueLen;
	
	bool found = GetProperty ( schemaNS, propName, &valueStr, &valueLen, options );
	if ( found )  {
		if ( ! XMP_PropIsSimple ( *options ) ) XMP_Throw ( "Property must be simple", kXMPErr_BadXPath );
		XMPUtils::ConvertToDate ( valueStr, propValue );
	}
	return found;
	
}	// GetProperty_Date


// -------------------------------------------------------------------------------------------------
// SetProperty_Bool
// ----------------

void
XMPMeta::SetProperty_Bool ( XMP_StringPtr  schemaNS,
							XMP_StringPtr  propName,
							bool		   propValue,
							XMP_OptionBits options )
{
	XMP_Assert ( (schemaNS != 0) && (propName != 0) );	// Enforced by wrapper.

	XMP_VarString valueStr;
	XMPUtils::ConvertFromBool ( propValue, &valueStr );
	SetProperty ( schemaNS, propName, valueStr.c_str(), options );
	
}	// SetProperty_Bool


// -------------------------------------------------------------------------------------------------
// SetProperty_Int
// ---------------

void
XMPMeta::SetProperty_Int ( XMP_StringPtr  schemaNS,
						   XMP_StringPtr  propName,
						   XMP_Int32	  propValue,
						   XMP_OptionBits options )
{
	XMP_Assert ( (schemaNS != 0) && (propName != 0) );	// Enforced by wrapper.

	XMP_VarString valueStr;
	XMPUtils::ConvertFromInt ( propValue, "", &valueStr );
	SetProperty ( schemaNS, propName, valueStr.c_str(), options );
	
}	// SetProperty_Int


// -------------------------------------------------------------------------------------------------
// SetProperty_Int64
// -----------------

void
XMPMeta::SetProperty_Int64 ( XMP_StringPtr  schemaNS,
						     XMP_StringPtr  propName,
						     XMP_Int64	    propValue,
						     XMP_OptionBits options )
{
	XMP_Assert ( (schemaNS != 0) && (propName != 0) );	// Enforced by wrapper.

	XMP_VarString valueStr;
	XMPUtils::ConvertFromInt64 ( propValue, "", &valueStr );
	SetProperty ( schemaNS, propName, valueStr.c_str(), options );
	
}	// SetProperty_Int64


// -------------------------------------------------------------------------------------------------
// SetProperty_Float
// -----------------

void
XMPMeta::SetProperty_Float ( XMP_StringPtr	schemaNS,
							 XMP_StringPtr	propName,
							 double			propValue,
							 XMP_OptionBits	options )
{
	XMP_Assert ( (schemaNS != 0) && (propName != 0) );	// Enforced by wrapper.

	XMP_VarString valueStr;
	XMPUtils::ConvertFromFloat ( propValue, "", &valueStr );
	SetProperty ( schemaNS, propName, valueStr.c_str(), options );
	
}	// SetProperty_Float


// -------------------------------------------------------------------------------------------------
// SetProperty_Date
// ----------------

void
XMPMeta::SetProperty_Date ( XMP_StringPtr		   schemaNS,
							XMP_StringPtr		   propName,
							const	XMP_DateTime & propValue,
							XMP_OptionBits		   options )
{
	XMP_Assert ( (schemaNS != 0) && (propName != 0) );	// Enforced by wrapper.

	XMP_VarString valueStr;
	XMPUtils::ConvertFromDate ( propValue, &valueStr );
	SetProperty ( schemaNS, propName, valueStr.c_str(), options );
	
}	// SetProperty_Date

// =================================================================================================

#if AdobePrivate	// These are private CS4 performance hacks.

// -------------------------------------------------------------------------------------------------

static inline void
GetStringInfo ( const XMP_Node * source, XMPDMO_StringInfo * dest )
{
	dest->ptr = source->value.c_str();
	dest->len = static_cast<XMP_Index>( source->value.size() );
}	// GetStringInfo

#if ENABLE_CPP_DOM_MODEL
inline void
GetStringInfo( const AdobeXMPCore::spcINode & source, XMPDMO_StringInfo * dest)
{
	using namespace AdobeXMPCore;
	if (source->GetNodeType() == INode::kNTSimple) {

		auto valueStr = source->ConvertToSimpleNode()->GetValue();
		dest->ptr =  valueStr->c_str();
		dest->len = (XMP_Uns32)valueStr->size();
	}
}	// GetStringInfo
#endif

inline void
GetStringInfo(const XMP_VarString & value, XMPDMO_StringInfo * dest)
{
	

	dest->ptr = value.c_str();
	dest->len = (XMP_Uns32)value.length();
	
}	// GetStringInfo


static inline void
GetCuePointParams ( const XMP_Node * libField,
					XMPDMO_MarkerInfo * clientMarker,
					XMPDMO_AllocClientCuePointsProc AllocCuePointParams )
{
	if ( ! XMP_PropIsArray ( libField->options ) ) return;	// Ignore bad data.
	
	size_t cppLimit = libField->children.size();
	if ( cppLimit == 0 ) return;
	
	XMPDMO_CuePointInfo * clientInfo = (XMPDMO_CuePointInfo*) (*AllocCuePointParams) ( clientMarker, (XMP_Index)cppLimit );
	if ( clientInfo == 0 ) XMP_Throw ( "Can't create CuePointParams", kXMPErr_NoMemory );

	for ( size_t cpp = 0; cpp < cppLimit; ++cpp, ++clientInfo ) {

		const XMP_Node * libCPP   = libField->children[cpp];
		const XMP_Node * libKey   = 0;
		const XMP_Node * libValue = 0;

		if ( ! XMP_PropIsStruct ( libCPP->options ) ) continue;	// Ignore bad data.
		if ( libCPP->children.size() != 2 ) continue;

		if ( libCPP->children[0]->name == "xmpDM:key" ) {
			libKey   = libCPP->children[0];
			libValue = libCPP->children[1];
		} else if ( libCPP->children[1]->name == "xmpDM:key" ) {
			libKey   = libCPP->children[1];
			libValue = libCPP->children[0];
		} else {
			continue;
		}
		if ( libValue->name != "xmpDM:value" ) continue;
		
		GetStringInfo ( libKey, &clientInfo->key );
		GetStringInfo ( libValue, &clientInfo->value );

	}

}	// GetCuePointParams

#if ENABLE_CPP_DOM_MODEL
static inline void
GetCuePointParams( const AdobeXMPCore::spcINode & libField,
                   XMPDMO_MarkerInfo * clientMarker,
                   XMPDMO_AllocClientCuePointsProc AllocCuePointParams)
{
	using namespace AdobeXMPCore;

	if (!libField && libField->GetNodeType() != INode::kNTArray) return;
	XMP_OptionBits libFieldOptions = XMPUtils::GetIXMPOptions(libField);
	if (!XMP_PropIsArray(libFieldOptions)) return;	// Ignore bad data.

    size_t cppLimit = XMPUtils::GetNodeChildCount(AdobeXMPCore_Int::const_pointer_cast<INode>(libField));

	if (!cppLimit) return;

	XMPDMO_CuePointInfo * clientInfo = (XMPDMO_CuePointInfo*)(*AllocCuePointParams) (clientMarker, (XMP_Index)cppLimit);
	if (clientInfo == 0) XMP_Throw("Can't create CuePointParams", kXMPErr_NoMemory);
	spcINodeIterator childIter;
	if (libField->GetNodeType() == INode::kNTArray) {
		childIter = libField->ConvertToArrayNode()->Iterator();
	}
	else if (libField->GetNodeType() == INode::kNTStructure) {
		childIter = libField->ConvertToStructureNode()->Iterator();
	}

	for (size_t cpp = 0; cpp < cppLimit; ++cpp, ++clientInfo, childIter = childIter->Next()) {

		spcINode   libCPP = childIter->GetNode();
		spcINode   libKey;
		spcINode   libValue;
		XMP_OptionBits libCPPOptions = XMPUtils::GetIXMPOptions(libCPP);
		if (!XMP_PropIsStruct(libCPPOptions)) continue;	// Ignore bad data.

		if (XMPUtils::GetNodeChildCount(libCPP) != 2) continue;
		spcINodeIterator libCPPChildIter = XMPUtils::GetNodeChildIterator(libCPP);
		spcINode libCPPFirstChild = libCPPChildIter->GetNode();
		libCPPChildIter = libCPPChildIter->Next();
		spcINode libCPPSecondChild = libCPPChildIter->GetNode();
		if (XMP_LitMatch(libCPPFirstChild->GetName()->c_str(), "key")) {
			libKey = libCPPFirstChild;
			libValue = libCPPSecondChild;
		}
		else if (XMP_LitMatch(libCPPSecondChild->GetName()->c_str(), "key")) {
			libKey = libCPPSecondChild;
			libValue = libCPPFirstChild;
		}
		else {
			continue;
		}
		if (!XMP_LitMatch(libValue->GetName()->c_str(), "value")) continue;

		GetStringInfo(libKey, &clientInfo->key);
		GetStringInfo(libValue, &clientInfo->value);

	}

}	
#endif
// -------------------------------------------------------------------------------------------------

static inline void AddStringField(const XMPDMO_StringInfo & valueInfo, XMP_StringPtr name, XMP_Node * parent)
{

	if (valueInfo.len > 0) {
		XMP_Node * libField = new XMP_Node(parent, name, 0);
		if (libField == 0) XMP_Throw("Can't create string field", kXMPErr_NoMemory);
		parent->children.push_back(libField);
		SetNodeValue(libField, valueInfo.ptr);
	}

}	// AddStringField

static inline void AddStringField(const XMP_VarString & valueInfo, XMP_StringPtr name, XMP_Node * parent)
{

	if (valueInfo.length() > 0) {
		XMP_Node * libField = new XMP_Node(parent, name, 0);
		if (libField == 0) XMP_Throw("Can't create string field", kXMPErr_NoMemory);
		parent->children.push_back(libField);
		SetNodeValue(libField, valueInfo.c_str());
	}

}	// AddStringField

static inline void AddStringField(XMP_StringPtr valueInfo, XMP_StringPtr name, XMP_Node * parent)
{

	if (valueInfo && strlen(valueInfo)) {
		XMP_Node * libField = new XMP_Node(parent, name, 0);
		if (libField == 0) XMP_Throw("Can't create string field", kXMPErr_NoMemory);
		parent->children.push_back(libField);
		SetNodeValue(libField, valueInfo);
	}

}	// AddStringField

#if ENABLE_CPP_DOM_MODEL
static inline void AddStringField(XMP_StringPtr valueInfo, XMP_StringPtr name, const AdobeXMPCore::spINode & parent)
{
	using namespace  AdobeXMPCore;

	if (valueInfo && strlen(valueInfo) > 0) {
		spISimpleNode libField = ISimpleNode::CreateSimpleNode( kXMP_NS_DM, AdobeXMPCommon::npos, name, AdobeXMPCommon::npos, "", AdobeXMPCommon::npos );
		if (!libField) XMP_Throw("Can't create string field", kXMPErr_NoMemory);

		if (parent->GetNodeType() == INode::kNTArray) {
			parent->ConvertToArrayNode()->InsertNodeAtIndex( libField, XMPUtils::GetNodeChildCount( parent ) + 1 );
		} else if (parent->GetNodeType() == INode::kNTStructure) {
			auto structParent = parent->ConvertToStructureNode();
			if ( !structParent->GetNode(kXMP_NS_DM, AdobeXMPCommon::npos, name, AdobeXMPCommon::npos )) {
				structParent->InsertNode(libField);
			}
			else {
				libField = structParent->GetNode( kXMP_NS_DM, AdobeXMPCommon::npos, name, AdobeXMPCommon::npos )->ConvertToSimpleNode();
			}
		}
		XMP_OptionBits options = 0;
		XMPUtils::SetNode( libField, valueInfo, options);
	}

}
#endif

static const XMP_VarString extensionPrefix = "Extensions_";
static XMP_VarString doubleQuotes = "&quot;";
static const XMP_VarString openingBrace = "{";
static const XMP_VarString closingBrace = "}"; 
static const XMP_VarString markerGUID = "marker_guid";
const std::string guidNodeName = "xmpDM:guid";
const std::string keyNodeName = "xmpDM:key";
const std::string valueNodeName = "xmpDM:value";


bool XMPUtils::IsSuitableForJSONSerialization(const XMP_Node * xmpNode) {

	if (!xmpNode) return false;
	if (!(xmpNode->options & kXMP_PropValueIsStruct)) return false;
	if (xmpNode->qualifiers.size())  return false;
	XMP_Index  childSize = (XMP_Index)xmpNode->children.size();

	for (XMP_Index childIter = 0; childIter < childSize; childIter++) {

		const XMP_Node * childNode = xmpNode->children[childIter];
		if (childNode->children.size()) return false;
		if (childNode->qualifiers.size()) return false;
		if (childNode->options & kXMP_PropHasQualifiers) return false;
		if (childNode->options & kXMP_PropValueIsArray)  return false;
		if (childNode->options & kXMP_PropValueIsStruct) return false;
	}

	return true;
}

static bool IsKeyValidExtension(XMP_VarString cppKey) {

	XMP_StringLen cppKeyLen = (XMP_StringLen)cppKey.length();
	if (cppKey.find_first_of(extensionPrefix) != 0) return false;
	return true;

}
bool XMPUtils::GetSerializedJSONForExtensionNode(const XMP_Node * xmpNode, XMP_VarString &extensionAsKey, XMP_VarString & serializedJSON) {


	doubleQuotes = "&quot;";
	
	if (!IsSuitableForJSONSerialization(xmpNode)) {
		return false;
	}
	XMP_StringPtr extensionURI, extensionName;
	XMP_StringLen extensionURILen, extensionNameLen;
	xmpNode->GetFullQualifiedName(&extensionURI, &extensionURILen, &extensionName, &extensionNameLen);
	extensionAsKey = extensionPrefix;
	extensionAsKey.append(extensionURI);
	extensionAsKey.append(":");
	extensionAsKey.append(extensionName);
	serializedJSON = openingBrace;
	XMP_Index childSize = (XMP_Index)xmpNode->children.size();

	for (XMP_Index childIter = 0; childIter < childSize; ++childIter) {

		const XMP_Node * childNode = xmpNode->children[childIter];
		XMP_StringPtr childURI, childName;
		XMP_StringLen childURILen, childNameLen;
		//TODO verify from Aman whether childURI or childNamespaceprefix should be there
		childNode->GetFullQualifiedName(&childURI, &childURILen, &childName, &childNameLen);
		serializedJSON.append(doubleQuotes);
		serializedJSON.append(childURI); 
		serializedJSON.append(":");
		serializedJSON.append(childName);
		serializedJSON.append(doubleQuotes);
		serializedJSON.append(":");
		serializedJSON.append(doubleQuotes);
		serializedJSON.append(childNode->value);
		serializedJSON.append(doubleQuotes);
		if (childIter != childSize - 1) {

			serializedJSON.append(",");
		}
	}

	serializedJSON.append(closingBrace);
	return true;

}

static bool SplitNameSpaceAndName(const XMP_VarString & qualifiedName, XMP_VarString & nameSpacePrefix, XMP_VarString & name, bool needPrefix  = true) {

	size_t colonPos = qualifiedName.find_last_of(":");
	if (colonPos == std::string::npos) return false;
	if (needPrefix) {
		XMP_StringPtr nsPrefix;
		bool nsFound = sRegisteredNamespaces->GetPrefix(qualifiedName.substr(0, colonPos).c_str(), &nsPrefix, 0);
		if (!nsFound) return false;
		nameSpacePrefix = nsPrefix;
	}
	else {
		nameSpacePrefix = qualifiedName.substr(0, colonPos);
	}
	name = qualifiedName.substr(colonPos + 1);
	return true;
}


static bool GetExtensionNameFromCPPKey(const XMP_VarString &extensionAsKey, XMP_VarString &extensionName) {


	XMP_Index colonPos = (XMP_Index)extensionAsKey.find_last_of(":");
	if (colonPos == (XMP_Index)XMP_VarString::npos) {
		return false;
	}
	extensionName = extensionAsKey.substr(extensionPrefix.length(), colonPos - extensionPrefix.length());
	XMP_StringPtr nsPrefix;
	bool nsFound = sRegisteredNamespaces->GetPrefix(extensionName.c_str(), &nsPrefix, 0);
	if (!nsFound) return false;
	extensionName = nsPrefix;
	extensionName.append(extensionAsKey.substr(colonPos + 1));
	return true;
}
bool XMPUtils::CreateExtensionNode(XMP_Node ** xmpNode, const XMP_VarString & serializedJSON, const XMP_VarString & doubleQuotesString) {
	
	// TODO  handle quotes within URI, quotes within value

	try {

		doubleQuotes = doubleQuotesString;
		XMP_Index jsonIter = 1;
		size_t jsonLen = serializedJSON.length();
		if (!(jsonLen > 1)) throw 1;
		if (!(serializedJSON[0] == '{' && serializedJSON[jsonLen - 1] == '}')) throw 1;

		while (jsonIter + 1 < (XMP_Index)jsonLen) {

			if (!((XMP_Index)serializedJSON.find(doubleQuotes, jsonIter) == jsonIter)) throw 1 ;
			jsonIter += doubleQuotes.length();
			size_t qualifiedNameEnd = serializedJSON.find(doubleQuotes, jsonIter);
			if (!(qualifiedNameEnd != std::string::npos)) throw 1;
			XMP_VarString childNameSpace, childName;
			bool isSplit = SplitNameSpaceAndName(serializedJSON.substr(jsonIter, qualifiedNameEnd - jsonIter), childNameSpace, childName);
			if (!isSplit) throw 1;
			XMP_VarString childPrefixName = childNameSpace +  childName;
			jsonIter = (XMP_Index)(qualifiedNameEnd + doubleQuotes.length());

			if (!(serializedJSON[jsonIter] == ':')) throw 1;

			jsonIter++;
			if (!((XMP_Index)serializedJSON.find(doubleQuotes, jsonIter) == jsonIter)) throw 1;
			jsonIter += doubleQuotes.length();

			size_t valueEnd = serializedJSON.find(doubleQuotes, jsonIter);
			
			if (!(qualifiedNameEnd != std::string::npos)) throw 1;

			XMP_Node * newChildNode = new XMP_Node((*xmpNode), childPrefixName.c_str(), 0);

			newChildNode->SetValue(serializedJSON.substr(jsonIter, valueEnd - jsonIter).c_str());

			jsonIter = (XMP_Index)(valueEnd + doubleQuotes.length());
			(*xmpNode)->children.push_back(newChildNode);
			if (!(jsonIter == (XMP_Index)jsonLen - 1 || serializedJSON[jsonIter] == ',')) throw 1;
			jsonIter++;
		}

	}

	catch (...) {

		size_t childSize = (*xmpNode)->children.size();
		(*xmpNode)->ClearNode();
		
		
		return false;
	}

	return true;

}

#if ENABLE_CPP_DOM_MODEL
bool XMPUtils::CreateExtensionNode( const AdobeXMPCore::spIStructureNode & xmpNode, const XMP_VarString & serializedJSON, const string & doubleQuotesStr) {

	// TODO  handle quotes within URI, quotes within value
	using namespace AdobeXMPCore;
	try {

		doubleQuotes = doubleQuotesStr;
		XMP_Index jsonIter = 1;
		size_t jsonLen = serializedJSON.length();
		if (!(jsonLen > 1)) throw 1;
		if (!(serializedJSON[0] == '{' && serializedJSON[jsonLen - 1] == '}')) throw 1; 

		while (jsonIter + 1 < (XMP_Index)jsonLen) {

			if (!((XMP_Index)serializedJSON.find(doubleQuotes, jsonIter) == jsonIter)) throw 1;
			jsonIter += doubleQuotes.length();
			size_t qualifiedNameEnd = serializedJSON.find(doubleQuotes, jsonIter);
			if (!(qualifiedNameEnd != std::string::npos)) throw 1;
			XMP_VarString childNameSpace, childName;
			bool isSplit = SplitNameSpaceAndName(serializedJSON.substr(jsonIter, qualifiedNameEnd - jsonIter), childNameSpace, childName, false);
			if (!isSplit) throw 1;
			XMP_VarString childPrefixName = childNameSpace + childName;
			jsonIter = (XMP_Index)(qualifiedNameEnd + doubleQuotes.length());

			if (!(serializedJSON[jsonIter] == ':')) throw 1;

			jsonIter++;
			if (!((XMP_Index)serializedJSON.find(doubleQuotes, jsonIter) == jsonIter)) throw 1;
			jsonIter += doubleQuotes.length();

			size_t valueEnd = serializedJSON.find(doubleQuotes, jsonIter);

			if (!(qualifiedNameEnd != std::string::npos)) throw 1;

			spISimpleNode newChildNode = ISimpleNode::CreateSimpleNode( childNameSpace.c_str(), AdobeXMPCommon::npos, childName.c_str(), AdobeXMPCommon::npos, "", AdobeXMPCommon::npos );

			newChildNode->SetValue(serializedJSON.substr(jsonIter, valueEnd - jsonIter).c_str(), AdobeXMPCommon::npos );

			jsonIter = (XMP_Index)(valueEnd + doubleQuotes.length());
			xmpNode->AppendNode(newChildNode);
			if (!(jsonIter == (XMP_Index)jsonLen - 1 || serializedJSON[jsonIter] == ',')) throw 1;
			jsonIter++;
		}

	}
	catch (...) {
		xmpNode->Clear();
		
		return false;
	}

	return true;

}
#endif

static XMP_Node * CloneSubtreeNode(const XMP_Node * origRoot, XMP_Node * cloneParent, bool skipEmpty /* = false */)
{


	XMP_Node * cloneRoot = new XMP_Node(cloneParent, origRoot->name, origRoot->value, origRoot->options);
	CloneOffspring(origRoot, cloneRoot, skipEmpty);

	if (skipEmpty && cloneRoot->value.empty() && cloneRoot->children.empty()) {
		// ! Can't do earlier, CloneOffspring might be skipping empty children.
		delete cloneRoot;
		return 0;
	}

	cloneParent->children.push_back(cloneRoot);
	return cloneRoot;

}
// CloneSubtreeNode

static void mergeNodes(XMP_Node ** sourceRootNode, XMP_Node * destNode, std::map<XMP_VarString, size_t> & childrenMap) {

	if (!*sourceRootNode || !destNode) return;
	size_t sourceChildIndex = childrenMap.count(destNode->name) ? childrenMap[destNode->name] : (size_t)-1;
	if (sourceChildIndex == (size_t)-1 ) {
		// destNode is not present in sourceRootNode
		(*sourceRootNode)->children.push_back(destNode);
		childrenMap[destNode->name] = (*sourceRootNode)->children.size();
	}
	else {
		XMP_Node * sourceNode = (*sourceRootNode)->children[sourceChildIndex];
		if (!XMPUtils::IsSuitableForJSONSerialization(sourceNode)) {
		}
		else {
			std::map<XMP_VarString, size_t > childrenAlreadyPresent;
			size_t sourceChildCount = sourceNode->children.size();
			for (size_t childIter = 0; childIter < sourceChildCount; ++childIter) {
				childrenAlreadyPresent[sourceNode->children[childIter]->name] = childIter;
			}
			size_t destChildCount = destNode->children.size();
			for (size_t destNodeChildIter = 0; destNodeChildIter < destChildCount; ++destNodeChildIter) {

				XMP_Node * destChildNode = destNode->children[destNodeChildIter];
				if (!childrenAlreadyPresent.count(destChildNode->name)) {
					XMP_Node * cloneOfDestChildNode = new XMP_Node(sourceNode, destChildNode->name, destChildNode->value, destChildNode->options);
					sourceNode->children.push_back(cloneOfDestChildNode);
				}
			}
		}
		delete destNode;
		destNode = NULL;
	}
}

#if ENABLE_CPP_DOM_MODEL

static void mergeNodes( const AdobeXMPCore::spIStructureNode & sourceRootNode, const AdobeXMPCore::spIStructureNode & destNode) {

	using namespace AdobeXMPCore;
	if (!sourceRootNode || !destNode) return;
	spINode sourceNode = sourceRootNode->GetNode(destNode->GetNameSpace()->c_str(), AdobeXMPCommon::npos, destNode->GetName()->c_str(), AdobeXMPCommon::npos );
	if (!sourceNode) {
		// destNode is not present in sourceRootNode
		sourceRootNode->AppendNode(destNode);
	}
	else {
		
		if (!XMPUtils::IsExtensionValidForBackwardCompatibility(sourceNode)) {

		}
		else {
			spIStructureNode sourceStructureNode = sourceNode->ConvertToStructureNode();
			for (auto destNodeChildIter = destNode->Iterator() ; destNodeChildIter; destNodeChildIter = destNodeChildIter->Next()) {

				spISimpleNode destChildNode = destNodeChildIter->GetNode()->ConvertToSimpleNode();
				if (!destChildNode) continue;
				if ( !sourceStructureNode->GetNode( destChildNode->GetNameSpace()->c_str(), AdobeXMPCommon::npos, destChildNode->GetName()->c_str(), AdobeXMPCommon::npos ) ) {
					spINode cloneOfDestChildNode = destChildNode->Clone();
					sourceStructureNode->AppendNode(cloneOfDestChildNode);
				}
			}
		}
		destNode->Clear();
	}
}
#endif

#if ENABLE_CPP_DOM_MODEL
bool XMPUtils::IsExtensionValidForBackwardCompatibility( const AdobeXMPCore::spINode & extensionNode) {

	using namespace AdobeXMPCore;
	if (!extensionNode) return false;
	if (extensionNode->HasQualifiers()) return false;
	if (extensionNode->GetNodeType() != INode::kNTStructure) return false;

	spINodeIterator childIter = extensionNode->ConvertToStructureNode()->Iterator();

	for (; childIter; childIter = childIter->Next()) {

		spcINode childNode = childIter->GetNode();
		if (childNode->GetNodeType() != INode::kNTSimple) return false;
		if (childNode->HasQualifiers()) return false;
	}

	return true;
}
#endif

#if ENABLE_CPP_DOM_MODEL
static void GetNodeQualifiedName(const AdobeXMPCore::spcINode & node, std::string & qualifiedName) {

	using namespace AdobeXMPCore;
	qualifiedName.clear();
	qualifiedName.append(node->GetNameSpace()->c_str());
	qualifiedName.append(":");
	qualifiedName.append(node->GetName()->c_str());

}
#endif 

static void CreateStringForSerialization(std::string & inputStr) {

	inputStr = "&quot;" + inputStr + "&quot;";
}

#if ENABLE_CPP_DOM_MODEL
bool XMPUtils::SerializeExtensionAsJSON( const AdobeXMPCore::spINode & extensionNode, std::string & key, std::string& value) {

	using namespace AdobeXMPCore;

	if (!IsExtensionValidForBackwardCompatibility(extensionNode)) {

		return false;
	}
	key = extensionPrefix;
	std::string extensionQualifiedName;

	GetNodeQualifiedName(extensionNode, extensionQualifiedName);
	key.append(extensionQualifiedName);
	value.clear();
	value.append(std::string(1, '{'));
	spINodeIterator extensionNodeChildIter;
	if (extensionNode->GetNodeType() == INode::kNTStructure) {
		extensionNodeChildIter = extensionNode->ConvertToStructureNode()->Iterator();
	}
	else if (extensionNode->GetNodeType() == INode::kNTArray) {
		extensionNodeChildIter = extensionNode->ConvertToArrayNode()->Iterator();
	}
	bool isNotFirstChild = false;
	for (; extensionNodeChildIter; extensionNodeChildIter = extensionNodeChildIter->Next()) {

		if (isNotFirstChild) {
			value.append(",");
		}
		else {
			isNotFirstChild = true;
		}

		spcINode childNode = extensionNodeChildIter->GetNode();
		std::string childQualifiedName;
		GetNodeQualifiedName(childNode, childQualifiedName);
		CreateStringForSerialization(childQualifiedName);
		value.append(childQualifiedName);
		value.append(":");
		std::string childNodeValue = childNode->ConvertToSimpleNode()->GetValue()->c_str();
		CreateStringForSerialization(childNodeValue);
		value.append(childNodeValue);
	}
	value.append(std::string(1, '}'));
	return true;
}
#endif

/*Checks whether cuePointParam meets all the criteria*/

static bool IsValidCuePointParam(const XMP_Node * cuePointParam, XMP_Node * &keyNode, XMP_Node * &valueNode)
{

	if (!cuePointParam || cuePointParam->children.size() != 2) return false;

	keyNode = cuePointParam->children[0];
	valueNode = cuePointParam->children[1];

	if (!keyNode->name.compare(keyNodeName)) {

		if (!valueNode->name.compare(valueNodeName)){

			return true;
		}
		return false;
	}
	else if (!valueNode->name.compare(keyNodeName) ) {

		
		if (!keyNode->name.compare(valueNodeName)) {
			XMP_Node * temp(valueNode);
			valueNode = keyNode;
			keyNode = temp;
			return true;
		}
		return false;
	}

	return false;
}


/* class static */ bool
XMPUtils::GetBulkMarkers(const XMPMeta & xmp,
XMP_StringPtr   ns,
XMP_StringPtr   path,
void *          clientPtr,
XMP_Uns32       clientElemSize,
XMPDMO_AllocClientMarkersProc AllocClientMarkers,
XMPDMO_AllocClientCuePointsProc  AllocClientCuePoints)
{
	static bool latestMarkerVersion = sizeof( XMPDMO_MarkerInfo ) == clientElemSize;
	XMP_Assert((ns != 0) && (path != 0));	// Enforced by wrapper.
	XMP_Assert((clientPtr != 0) && (clientElemSize != 0));	// Enforced by wrapper.
	XMP_Assert((AllocClientMarkers != 0) && (AllocClientCuePoints != 0));	// Enforced by wrapper.
#if ENABLE_CPP_DOM_MODEL
	if(sUseNewCoreAPIs) {
		const XMPMeta2 & xmpMeta2 = dynamic_cast<const XMPMeta2 &>(xmp);
		return GetBulkMarkers(xmpMeta2, ns, path, clientPtr, clientElemSize, AllocClientMarkers, AllocClientCuePoints);
	}

#endif
	XMP_ExpandedXPath expPath;
	ExpandXPath(ns, path, &expPath);

	const XMP_Node * arrayNode = FindConstNode(&xmp.tree, expPath);
	if (arrayNode == 0) return false;

	if (!XMP_PropIsArray(arrayNode->options)) XMP_Throw("Markers must be an array", kXMPErr_BadParam);

	XMP_Index markerCount = (XMP_Index)arrayNode->children.size();
	if (markerCount == 0) return false;

	XMP_Uns8 * clientArray = (XMP_Uns8*)AllocClientMarkers(clientPtr, markerCount);

	for (XMP_Index m = 0; m < markerCount; ++m) {
		bool isGUIDEntryPresentInCPP = false;
		XMPDMO_MarkerInfo * clientMarker = (XMPDMO_MarkerInfo *)(clientArray + m*clientElemSize);
		const XMP_Node * libMarker = arrayNode->children[m];
		XMP_Assert(libMarker != 0);

		XMP_Node * extensionNode(NULL);
		XMP_Node * cuePointParamsNode(NULL);
		XMP_Node * guidNode(NULL);

		if (!XMP_PropIsStruct(libMarker->options)) XMP_Throw("Each Marker must be a struct", kXMPErr_BadParam);

		for (size_t f = 0, fields = libMarker->children.size(); f < fields; ++f) {

			XMP_Node * libField = libMarker->children[f];
			XMP_Assert(libField != 0);

			if (libField->name == "xmpDM:startTime") {
				GetStringInfo(libField, &clientMarker->startTime);
			}
			else if (libField->name == "xmpDM:duration") {
				GetStringInfo(libField, &clientMarker->duration);
			}
			else if (libField->name == "xmpDM:comment") {
				GetStringInfo(libField, &clientMarker->comment);
			}
			else if (libField->name == "xmpDM:name") {
				GetStringInfo(libField, &clientMarker->name);
			}
			else if (libField->name == "xmpDM:location") {
				GetStringInfo(libField, &clientMarker->location);
			}
			else if (libField->name == "xmpDM:target") {
				GetStringInfo(libField, &clientMarker->target);
			}
			else if (libField->name == "xmpDM:type") {
				GetStringInfo(libField, &clientMarker->type);
			}
			else if (libField->name == "xmpDM:cuePointType") {
				GetStringInfo(libField, &clientMarker->cuePointType);
			}
			else if (libField->name == "xmpDM:speaker") {
				GetStringInfo(libField, &clientMarker->speaker);
			}
			else if (libField->name == "xmpDM:probability") {
				GetStringInfo(libField, &clientMarker->probability);
			}
			else if (libField->name == "xmpDM:cuePointParams") {
				cuePointParamsNode = libField;
			}
			else if (libField->name == "xmp:Extensions") {
				extensionNode = libField;
			}
			else if (libField->name == "xmpDM:guid") {
				if (libField->value.length()) {
					guidNode = libField;
				}
			}
		}
#if XMP_MARKER_EXTENSIBILITY_BACKWARD_COMPATIBILITY
		if (extensionNode && cuePointParamsNode) {

			std::map< XMP_VarString, size_t> extensionChildrenMap;

			size_t extensionChildCount = extensionNode->children.size();
			for (size_t extensionIter = 0; extensionIter < extensionChildCount; ++extensionIter) {
				extensionChildrenMap[extensionNode->children[extensionIter]->name] = extensionIter;
			}
			size_t cppCount = cuePointParamsNode->children.size();
			for (XMP_Index cpp = 0; cpp < (XMP_Index)cppCount; ++cpp) {
				
				XMP_Node * keyNode(NULL), *valueNode(NULL);
				
				if (!IsValidCuePointParam(cuePointParamsNode->children[cpp], keyNode, valueNode)) continue;

				if (IsKeyValidExtension(keyNode->value)) {
					// create extension node, add it to dom
					XMP_VarString extensionName;
					if (!GetExtensionNameFromCPPKey(keyNode->value, extensionName)) continue;

					XMP_Node * extensionChildNode = new XMP_Node(extensionNode, extensionName.c_str(), kXMP_PropValueIsStruct);
					if (!(CreateExtensionNode(&extensionChildNode, valueNode->value, "&quot;") || CreateExtensionNode(&extensionChildNode, valueNode->value, "\""))) {

						delete extensionChildNode;
					}
					else {
						if (extensionChildNode) {
							mergeNodes(&extensionNode, extensionChildNode, extensionChildrenMap);
						}
					}

					// Add version 1 
				}
			}
		}
		if (extensionNode || guidNode) {
			// populate cue point param node
			if (!cuePointParamsNode) {
				XMP_Node * parentNode = const_cast<XMP_Node *>(libMarker);
				cuePointParamsNode = new XMP_Node(parentNode, "xmpDM:cuePointParams", (kXMP_PropValueIsArray | kXMP_PropArrayIsOrdered));
				if (cuePointParamsNode == 0) XMP_Throw("Can't create marker field", kXMPErr_NoMemory);
				parentNode->children.push_back(cuePointParamsNode);
			}
			size_t existingEntriesCount = (cuePointParamsNode) ? cuePointParamsNode->children.size() : 0 ;

			// pushing guid to cue point params
			if (guidNode && guidNode->value.length() > 0) {
				bool entryFoundAndReplaced = false;
				for (size_t index = 0; index < existingEntriesCount; index++) {

					XMP_Node * keyNode(NULL), * valueNode(NULL);
					if (!IsValidCuePointParam(cuePointParamsNode->children[index], keyNode, valueNode)) continue;

					if (keyNode->value.compare(markerGUID) == 0) {
						valueNode->SetValue(guidNode->value.c_str());
						entryFoundAndReplaced = true;
					}
				}

				if (!entryFoundAndReplaced) {
					XMP_Node * guidNodeInCuePointParams = new XMP_Node(cuePointParamsNode, kXMP_ArrayItemName, kXMP_PropValueIsStruct);
					if (guidNodeInCuePointParams == 0) XMP_Throw("Can't create node in cuePointParams", kXMPErr_NoMemory);
					cuePointParamsNode->children.push_back(guidNodeInCuePointParams);

					AddStringField(markerGUID, "xmpDM:key", guidNodeInCuePointParams);
					AddStringField(guidNode->value, "xmpDM:value", guidNodeInCuePointParams);
				}

			}

			// pushing extensions (which are possible) to cue point params
			if (extensionNode) {
				size_t extensionSize = extensionNode->children.size();
				for (XMP_Index extensionIter = 0; extensionIter < (XMP_Index)extensionSize; ++extensionIter) {
					const XMP_Node * extensionChildNode = extensionNode->children[extensionIter];

					if (!IsSuitableForJSONSerialization(extensionChildNode)) {
						continue;
					}

					XMP_VarString extensionNodeAsKey, extensionNodeSerializedAsJSON;
					GetSerializedJSONForExtensionNode(extensionChildNode, extensionNodeAsKey, extensionNodeSerializedAsJSON);

					bool isExtensionPresentInCPP = false;

					for (size_t index = 0; index < existingEntriesCount; index++) {
						XMP_Node * keyNode(NULL), * valueNode(NULL);
						if (!IsValidCuePointParam(cuePointParamsNode->children[index], keyNode, valueNode)) continue;

						if (keyNode->value.compare(extensionNodeAsKey) == 0) {
							valueNode->SetValue(extensionNodeSerializedAsJSON.c_str());
							isExtensionPresentInCPP = true;
						}
					}

					if (!isExtensionPresentInCPP) {
						XMP_Node * extensionNodeInCuePointParams = new XMP_Node(cuePointParamsNode, kXMP_ArrayItemName, kXMP_PropValueIsStruct);
						if (extensionNodeInCuePointParams == 0) XMP_Throw("Can't create node in cuePointParams", kXMPErr_NoMemory);
						cuePointParamsNode->children.push_back(extensionNodeInCuePointParams);

						AddStringField(extensionNodeAsKey, "xmpDM:key", extensionNodeInCuePointParams);
						AddStringField(extensionNodeSerializedAsJSON, "xmpDM:value", extensionNodeInCuePointParams);
					}
				}
			}
		}
#endif
		if (cuePointParamsNode)
			GetCuePointParams(cuePointParamsNode, clientMarker, AllocClientCuePoints);
#if XMP_MARKER_EXTENSIBILITY_BACKWARD_COMPATIBILITY
		if (latestMarkerVersion) {

			if (extensionNode) {			
				XMP_Node * cloneOfExtension = CloneSubtreeNode(extensionNode, const_cast< XMP_Node * >(libMarker), false);
				if (cloneOfExtension) {
					const_cast< XMP_Node * >(libMarker)->children.pop_back(); // Hack
					clientMarker->SetPrivateData(cloneOfExtension, ReleaseXMP_Node);
				}
				
			}

			if (!guidNode) {
				size_t existingEntriesCount = cuePointParamsNode ? cuePointParamsNode->children.size() : 0;
				for (size_t index = 0; index < existingEntriesCount; index++) {

					XMP_Node * keyNode(NULL), *valueNode(NULL);

					if (!IsValidCuePointParam(cuePointParamsNode->children[index], keyNode, valueNode)) continue;

					if (keyNode->value.compare(markerGUID) == 0 && valueNode->value.length()) {
						guidNode = valueNode;
						break;
					}
				}
			}
			if (guidNode) {
				// Add issue
				clientMarker->guid.ptr = guidNode->value.c_str();
				clientMarker->guid.len = (XMP_StringLen)guidNode->value.length();
			}

		}
#endif
	}
	return true;

}	// GetBulkMarkers



#if ENABLE_CPP_DOM_MODEL
/* class static */ bool
XMPUtils::GetBulkMarkers(const XMPMeta2 & xmp,
	XMP_StringPtr   ns,
	XMP_StringPtr   path,
	void *          clientPtr,
	XMP_Uns32       clientElemSize,
	XMPDMO_AllocClientMarkersProc AllocClientMarkers,
	XMPDMO_AllocClientCuePointsProc  AllocClientCuePoints)
{
	static bool latestMarkerVersion = sizeof( XMPDMO_MarkerInfo ) == clientElemSize;
	XMP_Assert((ns != 0) && (path != 0));	// Enforced by wrapper.
	XMP_Assert( (clientPtr != 0) && (clientElemSize != 0));	// Enforced by wrapper.
	XMP_Assert((AllocClientMarkers != 0) && (AllocClientCuePoints != 0));	// Enforced by wrapper.

	using namespace AdobeXMPCore;
	auto defaultMap = INameSpacePrefixMap::GetDefaultNameSpacePrefixMap();

	XMP_ExpandedXPath expPath;
	ExpandXPath(ns, path, &expPath);

	spIArrayNode arrayNode;
	XMP_OptionBits arrayOptions = 0;
	spINode destNode;
	XMPUtils::FindCnstNode(xmp.mDOM, expPath, destNode, &arrayOptions);

	if (!destNode) return false;
	arrayNode = destNode->ConvertToArrayNode();

	if ( !arrayNode ) XMP_Throw("Markers must be an array", kXMPErr_BadParam);

	sizet markerCount = arrayNode->ChildCount();
	if (markerCount == 0) return false;

	XMP_Uns8 * clientArray = (XMP_Uns8*)AllocClientMarkers(clientPtr, (XMP_Index)markerCount);

	spINodeIterator marker;
	sizet markerIndex( 0 );

	for (marker = arrayNode->Iterator(); marker; marker = marker->Next(), markerIndex++) {

		XMPDMO_MarkerInfo *clientMarker = (XMPDMO_MarkerInfo *)(clientArray + markerIndex * clientElemSize);

		spINode tempNode = marker->GetNode();
		XMP_Assert(tempNode);
		spIStructureNode markerNode = tempNode->ConvertToStructureNode();
		if (!markerNode) XMP_Throw("Each Marker must be a struct", kXMPErr_BadParam);

		spISimpleNode guidNode;
		spIStructureNode extensionNode;
		spIArrayNode cuePointParamsNode;


		for (spINodeIterator fieldIter = markerNode->Iterator(); fieldIter; fieldIter = fieldIter->Next()) {

			spINode libField = fieldIter->GetNode();
			XMP_Assert(libField);

			XMP_VarString libFieldName = libField->GetName()->c_str();

			if (libFieldName == "startTime") {
				GetStringInfo(libField, &clientMarker->startTime);
			}
			else if (libFieldName == "duration") {
				GetStringInfo(libField, &clientMarker->duration);
			}
			else if (libFieldName == "comment") {
				GetStringInfo(libField, &clientMarker->comment);
			}
			else if (libFieldName == "name") {
				GetStringInfo(libField, &clientMarker->name);
			}
			else if (libFieldName == "location") {
				GetStringInfo(libField, &clientMarker->location);
			}
			else if (libFieldName == "target") {
				GetStringInfo(libField, &clientMarker->target);
			}
			else if (libFieldName == "type") {
				GetStringInfo(libField, &clientMarker->type);
			}
			else if (libFieldName == "cuePointType") {
				GetStringInfo(libField, &clientMarker->cuePointType);
			}
			else if (libFieldName == "speaker") {
				GetStringInfo(libField, &clientMarker->speaker);
			}
			else if (libFieldName == "probability") {
				GetStringInfo(libField, &clientMarker->probability);
			}
			else if (libFieldName == "cuePointParams") {
				cuePointParamsNode = libField->ConvertToArrayNode();
				if (!cuePointParamsNode) XMP_Throw("Cue Point Param Node must be an array", kXMPErr_BadParam);
				//GetCuePointParams(libField, clientMarker, AllocClientCuePoints);
			}
			else if (libFieldName == "Extensions" && !strcmp(libField->GetNameSpace()->c_str(), kXMP_NS_XMP)){
				extensionNode = libField->ConvertToStructureNode();
				if (!extensionNode) XMP_Throw("Extension Node must be a structure", kXMPErr_BadParam);
			}
			else if (libFieldName == "guid") {
				spISimpleNode guidNodeCopy = libField->ConvertToSimpleNode();
				if (!guidNodeCopy) XMP_Throw("guid node must be simple", kXMPErr_BadParam);
				if (guidNodeCopy->GetValue() && guidNodeCopy->GetValue()->size()) guidNode = guidNodeCopy;
			}
		}
#if XMP_MARKER_EXTENSIBILITY_BACKWARD_COMPATIBILITY
		if (extensionNode || guidNode) {
			// populate cue point param node
			if (!cuePointParamsNode) {
				cuePointParamsNode = IArrayNode::CreateOrderedArrayNode( kXMP_NS_DM, AdobeXMPCommon::npos, "cuePointParams", AdobeXMPCommon::npos );
				if (!cuePointParamsNode) XMP_Throw("Can't create marker field", kXMPErr_NoMemory);
				markerNode->AppendNode(cuePointParamsNode);
			}

			sizet existingEntriesCount = cuePointParamsNode->ChildCount();

			// pushing guid to cue point params
			if (guidNode && guidNode->GetValue() && guidNode->GetValue()->size() > 0 ) {
				// iterate and find existing guid node, if present and value not same change value else create a node
				bool entryFoundAndReplaced = false;
				for (size_t i = 0; i < existingEntriesCount; i++) {
					spINode currentNode = cuePointParamsNode->GetNodeAtIndex(i + 1);
					if (!currentNode) continue;
					spIStructureNode currentStructureNode = currentNode->ConvertToStructureNode();
					if (!currentStructureNode) continue;
					spINode keyNode = currentStructureNode->GetNode(kXMP_NS_DM, AdobeXMPCommon::npos, "key", AdobeXMPCommon::npos );
					if (keyNode) {
						spISimpleNode keySimpleNode = keyNode->ConvertToSimpleNode();
						if (strcmp(keySimpleNode->GetValue()->c_str(), markerGUID.c_str()) == 0) {
							//keySimpleNode->ChangeValue(guidNode->GetValue_I()->c_str());
							spINode valueNode = currentStructureNode->GetNode(kXMP_NS_DM, AdobeXMPCommon::npos, "value", AdobeXMPCommon::npos );
							if (valueNode ) {
								spISimpleNode valueSimpleNode = valueNode->ConvertToSimpleNode();
								if (valueSimpleNode) {
									valueSimpleNode->SetValue(guidNode->GetValue()->c_str(), guidNode->GetValue()->size());
								}
							}
							entryFoundAndReplaced = true;
						}
					}
				}

				if (!entryFoundAndReplaced) {
					spIStructureNode guidEntryInCuePointParams = IStructureNode::CreateStructureNode(kXMP_NS_DM, AdobeXMPCommon::npos, "temp", AdobeXMPCommon::npos );
					guidEntryInCuePointParams->AppendNode(ISimpleNode::CreateSimpleNode(kXMP_NS_DM, AdobeXMPCommon::npos, "key", AdobeXMPCommon::npos, markerGUID.c_str(), AdobeXMPCommon::npos ));
					guidEntryInCuePointParams->AppendNode(ISimpleNode::CreateSimpleNode(kXMP_NS_DM, AdobeXMPCommon::npos, "value", AdobeXMPCommon::npos, guidNode->GetValue()->c_str(), AdobeXMPCommon::npos ));
					cuePointParamsNode->AppendNode(guidEntryInCuePointParams);
				}
			}
			if (extensionNode && cuePointParamsNode) {

				for (auto cuePointParamIter = cuePointParamsNode->Iterator(); cuePointParamIter; cuePointParamIter = cuePointParamIter->Next()) {

					spIStructureNode cuePointParamItem = cuePointParamIter->GetNode()->ConvertToStructureNode();
					spINode key = cuePointParamItem->GetNode(kXMP_NS_DM, AdobeXMPCommon::npos, "key", AdobeXMPCommon::npos );
					spINode value = cuePointParamItem->GetNode(kXMP_NS_DM, AdobeXMPCommon::npos, "value", AdobeXMPCommon::npos );
					if (!key || !value) continue;
					spISimpleNode keyNode = key->ConvertToSimpleNode();
					spISimpleNode valueNode = value->ConvertToSimpleNode();
					if (!keyNode || !valueNode) continue;
					if (IsKeyValidExtension(keyNode->GetValue()->c_str())){
						XMP_VarString extensionName;
						if (!GetExtensionNameFromCPPKey(keyNode->GetValue()->c_str(), extensionName)) continue;
						size_t colonPos = extensionName.find_last_of(':');
						XMP_VarString  extensionURI = extensionName.substr(0, colonPos);
						if (defaultMap->GetNameSpace(extensionURI.c_str(), AdobeXMPCommon::npos )) {
							extensionURI = defaultMap->GetNameSpace(extensionURI.c_str(), AdobeXMPCommon::npos )->c_str();
						}
						XMP_VarString  extensionPartName = extensionName.substr(colonPos + 1);
						spIStructureNode extensionNodeToBeAdded = IStructureNode::CreateStructureNode(extensionURI.c_str(), AdobeXMPCommon::npos, extensionPartName.c_str(), AdobeXMPCommon::npos );
						if (CreateExtensionNode(extensionNodeToBeAdded, valueNode->GetValue()->c_str(), "&quot;") || CreateExtensionNode(extensionNodeToBeAdded, valueNode->GetValue()->c_str(), "\"")) {

							if (extensionNodeToBeAdded) {
								mergeNodes(extensionNode, extensionNodeToBeAdded);
							}
						}
					}
				}
			}
			// pushing extensions (which are possible) to cue point params
			if (extensionNode) {
				sizet extensionSize = extensionNode->ChildCount();
				for (spINodeIterator nodeIterator = extensionNode->Iterator(); nodeIterator; nodeIterator = nodeIterator->Next()) {
					spINode node = nodeIterator->GetNode();

					if (!IsExtensionValidForBackwardCompatibility( node )) continue;
					XMP_VarString extensionNodeAsKey, extensionNodeSerializedAsJSON;
					if (!SerializeExtensionAsJSON( node, extensionNodeAsKey, extensionNodeSerializedAsJSON ) ) continue;

					bool entryFoundAndReplaced = false;
					for (size_t i = 0; i < existingEntriesCount; i++) {
						spINode currentNode = cuePointParamsNode->GetNodeAtIndex(i + 1);
						spIStructureNode currentStructureNode = currentNode->ConvertToStructureNode();
						spINode keyNode = currentStructureNode->GetNode(kXMP_NS_DM, AdobeXMPCommon::npos, "key", AdobeXMPCommon::npos );
						if (keyNode) {
							spISimpleNode keySimpleNode = keyNode->ConvertToSimpleNode();
							if (keySimpleNode && strcmp(keySimpleNode->GetValue()->c_str(), extensionNodeAsKey.c_str()) == 0) {
								spINode valueNode = currentStructureNode->GetNode(kXMP_NS_DM, AdobeXMPCommon::npos, "value", AdobeXMPCommon::npos );
								if (valueNode) {
									spISimpleNode valueSimpleNode = valueNode->ConvertToSimpleNode();
									if (valueSimpleNode) {
										valueSimpleNode->SetValue(extensionNodeSerializedAsJSON.c_str(), AdobeXMPCommon::npos );
									}
								}
								entryFoundAndReplaced = true;
							}
						}
					}

					if (!entryFoundAndReplaced) {
						spIStructureNode guidEntryInCuePointParams = IStructureNode::CreateStructureNode(kXMP_NS_DM, AdobeXMPCommon::npos, "temp", AdobeXMPCommon::npos );
						guidEntryInCuePointParams->AppendNode(ISimpleNode::CreateSimpleNode(kXMP_NS_DM, AdobeXMPCommon::npos, "key", AdobeXMPCommon::npos, extensionNodeAsKey.c_str(), AdobeXMPCommon::npos ));
						guidEntryInCuePointParams->AppendNode(ISimpleNode::CreateSimpleNode(kXMP_NS_DM, AdobeXMPCommon::npos, "value", AdobeXMPCommon::npos, extensionNodeSerializedAsJSON.c_str(), AdobeXMPCommon::npos ));
						cuePointParamsNode->AppendNode(guidEntryInCuePointParams);
					}

				}
			}
		}
#endif
		if (cuePointParamsNode)
			GetCuePointParams(cuePointParamsNode, clientMarker, AllocClientCuePoints);
#if XMP_MARKER_EXTENSIBILITY_BACKWARD_COMPATIBILITY
		if (latestMarkerVersion) {
			if (extensionNode) {
				pIStructureNode_base ptr = extensionNode.get();
				extensionNode->Acquire();
				clientMarker->SetPrivateData(ptr, ReleaseIStructureNode);
			}
			if (!guidNode) {
				size_t existingEntriesCount = cuePointParamsNode ? cuePointParamsNode->ChildCount() : 0;
				for (size_t i = 0; i < existingEntriesCount; i++) {
					spINode currentNode = cuePointParamsNode->GetNodeAtIndex(i + 1);
					if (!currentNode) continue;
					spIStructureNode currentStructureNode = currentNode->ConvertToStructureNode();
					if (!currentStructureNode) continue;
					spINode keyNode = currentStructureNode->GetNode(kXMP_NS_DM, AdobeXMPCommon::npos, "key", AdobeXMPCommon::npos );
					if (keyNode) {
						spISimpleNode keySimpleNode = keyNode->ConvertToSimpleNode();
						if (strcmp(keySimpleNode->GetValue()->c_str(), markerGUID.c_str()) == 0) {

							spINode valueNode = currentStructureNode->GetNode(kXMP_NS_DM, AdobeXMPCommon::npos, "value", AdobeXMPCommon::npos );
							if (valueNode) {
								spISimpleNode guidNodeTemp = valueNode->ConvertToSimpleNode();
								if (guidNodeTemp && guidNodeTemp->GetValue() && guidNodeTemp->GetValue()->size()) guidNode = guidNodeTemp;
								if (guidNode) break;
							}
						}
					}
				}

			}
			if (guidNode) {
				// Add issue
				clientMarker->guid.ptr = guidNode->GetValue()->c_str();
				clientMarker->guid.len = (XMP_StringLen)guidNode->GetValue()->size();

			}
		}
#endif
	}

	return true;

}	// GetBulkMarkers
#endif


#if ENABLE_CPP_DOM_MODEL
/* class static */ 
void XMPUtils::SetBulkMarkers(XMPMeta2 *     xmp,
						 XMP_StringPtr ns,
						 XMP_StringPtr path,
						 XMP_Index     count,
						 const void *  clientArray,
						 XMP_Uns32     clientElemSize,
						 XMPDMO_GetCuePointParamsProc GetCuePointParams)
{
	static bool latestMarkerVersion = sizeof( XMPDMO_MarkerInfo ) == clientElemSize;
	using namespace  AdobeXMPCore;
	using namespace AdobeXMPCommon;

	XMP_Assert((ns != 0) && (path != 0));	// Enforced by wrapper.
	auto defaultMap = INameSpacePrefixMap::GetDefaultNameSpacePrefixMap();
	XMP_ExpandedXPath expPath;
	ExpandXPath(ns, path, &expPath);
	
	XMP_OptionBits arrayOptions;
	spIArrayNode arrayNode;
	spINode tempArrayNode;
	XMPUtils::FindCnstNode(xmp->mDOM, expPath, tempArrayNode, &arrayOptions);

	
	if (tempArrayNode && tempArrayNode->GetNodeType() != INode::kNTArray) {
		if (!XMP_PropIsArray(arrayOptions)) XMP_Throw("Markers must be an array", kXMPErr_BadParam);
		 tempArrayNode->Clear();
	}
	else {
		arrayNode = tempArrayNode ? tempArrayNode->ConvertToArrayNode() : spIArrayNode();
		XPathStepInfo  lastPathSegment(expPath.back());
		XMP_VarString arrayStep = lastPathSegment.step;
		XMP_Index dummyIndex = 0;
		spINode destNode;
		if (!XMPUtils::FindNode(xmp->mDOM, expPath, kXMP_CreateNodes, arrayOptions, destNode, &dummyIndex, true)) {
			XMP_Throw("Failure creating array node", kXMPErr_BadXPath);
		}

		std::string arrayNameSpace, arrayName;
		XMPUtils::GetNameSpaceAndNameFromStepValue(lastPathSegment.step, defaultMap, arrayNameSpace, arrayName);
		arrayNode = IArrayNode::CreateOrderedArrayNode(arrayNameSpace.c_str(), AdobeXMPCommon::npos, arrayName.c_str(), AdobeXMPCommon::npos);
		if (!arrayNode) XMP_Throw("Can't create Markers array", kXMPErr_BadXPath);
		arrayOptions = (kXMP_PropValueIsArray | kXMP_PropArrayIsOrdered);
		if (destNode->GetNodeType() == INode::kNTArray) {
			destNode->ConvertToArrayNode()->InsertNodeAtIndex(arrayNode, (sizet)dummyIndex);
		}
		else if( destNode->GetNodeType() == INode::kNTStructure) {
			spIStructureNode structNode = destNode->ConvertToStructureNode();
			structNode->RemoveNode(arrayNode->GetNameSpace()->c_str(), arrayNode->GetNameSpace()->size(), arrayNode->GetName()->c_str(), arrayNode->GetName()->size());
			destNode->ConvertToStructureNode()->InsertNode(arrayNode);
		}
		else {

			return;
		}
	}
	for (XMP_Index m = 0; m < count; ++m) {

		spIStructureNode libMarker = AdobeXMPCore_Int::IStructureNode_I::CreateStructureNode(arrayNode->GetNameSpace(), arrayNode->GetName() );
		bool isGUIDPresentInCPP = false;
		arrayNode->InsertNodeAtIndex(libMarker, m + 1);
		const XMPDMO_MarkerInfo & clientMarker = *((const XMPDMO_MarkerInfo *)((XMP_Uns8*)clientArray + m*clientElemSize));


		spIStructureNode extensionRootNode;
		XMP_VarString guidString;
		map<XMP_VarString, vector<size_t> > cuePointParamsMap;
		bool isGUIDPresentInMarkerStructure = false;

		if (!libMarker) XMP_Throw("Can't create marker", kXMPErr_NoMemory);
		AddStringField(clientMarker.startTime.ptr, "startTime", libMarker);
		AddStringField(clientMarker.duration.ptr, "duration", libMarker);
		AddStringField(clientMarker.comment.ptr, "comment", libMarker);
		AddStringField(clientMarker.name.ptr, "name", libMarker);
		AddStringField(clientMarker.location.ptr, "location", libMarker);
		AddStringField(clientMarker.target.ptr, "target", libMarker);
		AddStringField(clientMarker.type.ptr, "type", libMarker);
		AddStringField(clientMarker.cuePointType.ptr, "cuePointType", libMarker);
		AddStringField(clientMarker.speaker.ptr, "speaker", libMarker);
		AddStringField(clientMarker.probability.ptr, "probability", libMarker);

#if XMP_MARKER_EXTENSIBILITY_BACKWARD_COMPATIBILITY

		if (latestMarkerVersion) {
			// read guid from the new marker
			if (clientMarker.guid.ptr != NULL && clientMarker.guid.len) {
				AddStringField(clientMarker.guid.ptr, "guid", libMarker);
				guidString = clientMarker.guid.ptr;
				isGUIDPresentInMarkerStructure = true;
			}
			void * extensionPrivateData = const_cast<XMPDMO_MarkerInfo &>(clientMarker).extension;
			pIStructureNode_base clientExtensionShallowCopy(NULL);
			if (extensionPrivateData) {
				clientExtensionShallowCopy = (pIStructureNode_base)extensionPrivateData;
				extensionRootNode = clientExtensionShallowCopy->Clone()->ConvertToStructureNode();
				if ( !libMarker->GetNode( extensionRootNode->GetNameSpace()->c_str(), AdobeXMPCommon::npos, extensionRootNode->GetName()->c_str(), AdobeXMPCommon::npos ) )
					libMarker->AppendNode( extensionRootNode );
			}

		}
#endif

		const XMPDMO_CuePointInfo * clientCuePointParams;
		XMP_Index cppCount;

		(*GetCuePointParams) (&clientMarker, &clientCuePointParams, &cppCount);

		spIArrayNode libCuePointParams = IArrayNode::CreateOrderedArrayNode(kXMP_NS_DM, AdobeXMPCommon::npos, "cuePointParams", AdobeXMPCommon::npos);
		if (!libCuePointParams) XMP_Throw("Can't create marker field", kXMPErr_NoMemory);
		libMarker->AppendNode(libCuePointParams);

#if XMP_MARKER_EXTENSIBILITY_BACKWARD_COMPATIBILITY

		if (!extensionRootNode) {
			extensionRootNode = IStructureNode::CreateStructureNode(kXMP_NS_XMP, AdobeXMPCommon::npos, "Extensions", AdobeXMPCommon::npos );
		}

#endif
		for (XMP_Index cpp = 0; cpp < cppCount; ++cpp) {

			spIStructureNode libCPP = IStructureNode::CreateStructureNode(kXMP_NS_DM, AdobeXMPCommon::npos, "cuePointParams", AdobeXMPCommon::npos );
			if (!libCPP) XMP_Throw("Can't create cuePointParams", kXMPErr_NoMemory);
			libCuePointParams->InsertNodeAtIndex(libCPP, cpp + 1);
			const XMPDMO_CuePointInfo & clientCPP = clientCuePointParams[cpp];
			AddStringField(clientCPP.key.ptr, "key", libCPP);
			AddStringField(clientCPP.value.ptr, "value", libCPP);
			if (!clientCPP.key.ptr || !clientCPP.value.ptr) continue;
			cuePointParamsMap[clientCPP.key.ptr].push_back(cpp + 1);
#if XMP_MARKER_EXTENSIBILITY_BACKWARD_COMPATIBILITY
			if ( !isGUIDPresentInMarkerStructure && clientCPP.key.len && !strncmp ( clientCPP.key.ptr, markerGUID.c_str ( ), clientCPP.key.len )) {
				guidString = clientCPP.value.ptr;
				AddStringField(clientCPP.value.ptr, "guid", libMarker);
				isGUIDPresentInMarkerStructure = true;
			}

			if (IsKeyValidExtension(clientCPP.key.ptr)){
				XMP_VarString extensionName;
				if (!GetExtensionNameFromCPPKey(clientCPP.key.ptr, extensionName)) continue;
				size_t colonPos = extensionName.find_last_of(':');
				XMP_VarString  extensionURI = extensionName.substr(0, colonPos);
				if (defaultMap->GetNameSpace(extensionURI.c_str(), AdobeXMPCommon::npos )) {
					extensionURI = defaultMap->GetNameSpace(extensionURI.c_str(), AdobeXMPCommon::npos )->c_str();
				}
				XMP_VarString  extensionPartName = extensionName.substr(colonPos + 1);
				spIStructureNode extensionNodeToBeAdded = IStructureNode::CreateStructureNode(extensionURI.c_str(), AdobeXMPCommon::npos, extensionPartName.c_str(), AdobeXMPCommon::npos );
				if (CreateExtensionNode(extensionNodeToBeAdded, clientCPP.value.ptr, "&quot;") || CreateExtensionNode(extensionNodeToBeAdded, clientCPP.value.ptr, "\"")) {

					if (extensionNodeToBeAdded) {
						mergeNodes(extensionRootNode, extensionNodeToBeAdded);
					}
				}	
			}
			if (extensionRootNode && !extensionRootNode->ChildCount()) {
				extensionRootNode->Clear();
				extensionRootNode = spIStructureNode();
			}
			else {
				if (extensionRootNode) {
					if (!libMarker->GetNode(extensionRootNode->GetNameSpace()->c_str(), AdobeXMPCommon::npos, extensionRootNode->GetName()->c_str(), AdobeXMPCommon::npos ))
					libMarker->AppendNode(extensionRootNode);
				}
			}
#endif
			
		}

#if XMP_MARKER_EXTENSIBILITY_BACKWARD_COMPATIBILITY

		if (!isGUIDPresentInMarkerStructure) {

			XMPDocOps::CreateID("xmp:id", &guidString);
			AddStringField(guidString.c_str(), "guid", libMarker);
			isGUIDPresentInMarkerStructure = true;
		}

		if (extensionRootNode) {
			size_t extensionCount = extensionRootNode->ChildCount();
			for (spINodeIterator extensionIter = extensionRootNode->Iterator(); extensionIter; extensionIter = extensionIter->Next()) {
				spINode extensionChildNode = extensionIter->GetNode();

				
				XMP_VarString expectedKey, serializedJSON;
				bool isSerializable = SerializeExtensionAsJSON(extensionChildNode, expectedKey, serializedJSON);
				if (!isSerializable) continue;
				auto & cuePointParamsVectorByKey = cuePointParamsMap[expectedKey];
				if (cuePointParamsVectorByKey.size()) {

					for (size_t idx = 0; idx < cuePointParamsVectorByKey.size(); ++idx) {
						size_t cppIdx = cuePointParamsVectorByKey[idx];
						spINode libCuePointParamItem = libCuePointParams->GetNodeAtIndex(cppIdx);
						if (libCuePointParamItem->GetNodeType() != INode::kNTStructure) continue;
						spINode valueNode = libCuePointParamItem->ConvertToStructureNode()->GetNode( kXMP_NS_DM, AdobeXMPCommon::npos, "value", AdobeXMPCommon::npos );
						if (!valueNode || valueNode->GetNodeType() != INode::kNTSimple || XMPUtils::GetNodeChildCount(libCuePointParamItem) != 2) continue;
						valueNode->ConvertToSimpleNode()->SetValue(serializedJSON.c_str(), AdobeXMPCommon::npos );
					}
					
				}
				else {

					spIStructureNode libCPP = IStructureNode::CreateStructureNode(kXMP_NS_DM, AdobeXMPCommon::npos, "cuePointParams", AdobeXMPCommon::npos );
					
					AddStringField(expectedKey.c_str(), "key", libCPP);
					AddStringField(serializedJSON.c_str(), "value", libCPP);
					libCuePointParams->AppendNode(libCPP);
				}
			}
		}
		if (isGUIDPresentInMarkerStructure) {

			
			
			auto & cuePointParamsVectorByKey = cuePointParamsMap[markerGUID];
			if (cuePointParamsMap[markerGUID].size()) {

				for (size_t idx = 0; idx < cuePointParamsVectorByKey.size(); ++idx) {
					size_t cppIdx = cuePointParamsVectorByKey[idx];
					spINode libCuePointParamItem = libCuePointParams->GetNodeAtIndex( cppIdx );
					spINode valueNode = libCuePointParamItem->ConvertToStructureNode()->GetNode ( kXMP_NS_DM, AdobeXMPCommon::npos, "value", AdobeXMPCommon::npos );
					if (!valueNode || valueNode->GetNodeType() != INode::kNTSimple || XMPUtils::GetNodeChildCount(libCuePointParamItem) != 2) continue;
					valueNode->ConvertToSimpleNode()->SetValue( guidString.c_str(), AdobeXMPCommon::npos );
				}

			}
			else {

				spIStructureNode libCPP = IStructureNode::CreateStructureNode(kXMP_NS_DM, AdobeXMPCommon::npos, "cuePointParams", AdobeXMPCommon::npos );
				AddStringField(markerGUID.c_str(), "key", libCPP);
				AddStringField(guidString.c_str(), "value", libCPP);
				libCuePointParams->AppendNode(libCPP);
			}
		}
#endif
		if (libCuePointParams && !libCuePointParams->ChildCount()) {

			size_t libMarkerChildCount = libMarker->ChildCount();
			size_t idxToBeCleared = libMarkerChildCount;
			libMarker->RemoveNode(libCuePointParams->GetNameSpace()->c_str(), AdobeXMPCommon::npos, libCuePointParams->GetName()->c_str(), AdobeXMPCommon::npos );
			libCuePointParams->Clear();
		}
	}
}	// SetBulkMarkers
#endif


/* class static */ void
XMPUtils::SetBulkMarkers( XMPMeta *     xmp,
						  XMP_StringPtr ns,
						  XMP_StringPtr path,
                          XMP_Index     count,
                          const void *  clientArray,
                          XMP_Uns32     clientElemSize,
                          XMPDMO_GetCuePointParamsProc GetCuePointParams)
{
	static bool latestMarkerVersion = sizeof(XMPDMO_MarkerInfo) == clientElemSize;
#if ENABLE_CPP_DOM_MODEL

		XMPMeta2 * xmpMeta2Ptr = dynamic_cast<XMPMeta2 *> (xmp);
		if (xmpMeta2Ptr) {
			return SetBulkMarkers(xmpMeta2Ptr, ns, path, count, clientArray, clientElemSize, GetCuePointParams);
		}
	
#endif
	XMP_Assert((ns != 0) && (path != 0));	// Enforced by wrapper.

	XMP_ExpandedXPath expPath;
	ExpandXPath(ns, path, &expPath);
	XMP_VarString extensionNodeAsKey, extensionNodeSerializedAsJSON;
	XMP_Node * arrayNode = ::FindNode(&xmp->tree, expPath, kXMP_ExistingOnly);

	if (arrayNode != 0) {
		if (!XMP_PropIsArray(arrayNode->options)) XMP_Throw("Markers must be an array", kXMPErr_BadParam);
		arrayNode->RemoveChildren();
	}
	else {
		arrayNode = ::FindNode(&xmp->tree, expPath, kXMP_CreateNodes);
		if (arrayNode == 0) XMP_Throw("Can't create Markers array", kXMPErr_BadXPath);
		arrayNode->options |= (kXMP_PropValueIsArray | kXMP_PropArrayIsOrdered);
	}

	arrayNode->children.reserve(count);
	XMP_VarString newGUID;
	std::map< XMP_VarString , std::vector<size_t> > cuePointParamsMap;
	std::map< XMP_VarString , size_t > extensionChildrenMap;
	std::vector<XMP_Index> guidVector;
	for (XMP_Index m = 0; m < count; ++m) {

		XMP_Node * libMarker = new XMP_Node(arrayNode, kXMP_ArrayItemName, kXMP_PropValueIsStruct);
		if (libMarker == 0) XMP_Throw("Can't create marker", kXMPErr_NoMemory);
		arrayNode->children.push_back(libMarker);
		bool isGUIDPresentInMarkerStructure = false;
		
		const XMPDMO_MarkerInfo & clientMarker = *((const XMPDMO_MarkerInfo *)((XMP_Uns8*)clientArray + m*clientElemSize));
		
		XMP_Node * extensionRootNode( NULL );
		XMP_StringPtr guidString = NULL;
		AddStringField(clientMarker.startTime, "xmpDM:startTime", libMarker);
		AddStringField(clientMarker.duration, "xmpDM:duration", libMarker);
		AddStringField(clientMarker.comment, "xmpDM:comment", libMarker);
		AddStringField(clientMarker.name, "xmpDM:name", libMarker);
		AddStringField(clientMarker.location, "xmpDM:location", libMarker);
		AddStringField(clientMarker.target, "xmpDM:target", libMarker);
		AddStringField(clientMarker.type, "xmpDM:type", libMarker);
		AddStringField(clientMarker.cuePointType, "xmpDM:cuePointType", libMarker);
		AddStringField(clientMarker.speaker, "xmpDM:speaker", libMarker);
		AddStringField(clientMarker.probability, "xmpDM:probability", libMarker);
		bool guidPresentInCPP = false;

#if XMP_MARKER_EXTENSIBILITY_BACKWARD_COMPATIBILITY

		if (latestMarkerVersion) { 
			// read guid from the new marker
			if (clientMarker.guid.ptr != NULL && clientMarker.guid.len) {
				AddStringField(clientMarker.guid, guidNodeName.c_str(), libMarker);
				guidString = clientMarker.guid.ptr;
				isGUIDPresentInMarkerStructure = true;
			}

			void * extensionPrivateData = const_cast<XMPDMO_MarkerInfo &>(clientMarker).extension;
			XMP_Node * clientExtensionShallowCopy(NULL);
			if (extensionPrivateData) {
				clientExtensionShallowCopy = (XMP_Node *)extensionPrivateData;
				extensionRootNode = CloneSubtreeNode(clientExtensionShallowCopy, libMarker, true);
				if (extensionRootNode) {
					libMarker->children.pop_back(); // Hack
				}

			}
		}
#endif

		const XMPDMO_CuePointInfo * clientCuePointParams;
		XMP_Index cppCount;

		(*GetCuePointParams) (&clientMarker, &clientCuePointParams, &cppCount);


		XMP_Node * libCuePointParams = new XMP_Node(libMarker, "xmpDM:cuePointParams", (kXMP_PropValueIsArray | kXMP_PropArrayIsOrdered));
		if (libCuePointParams == 0) XMP_Throw("Can't create marker field", kXMPErr_NoMemory);
		libMarker->children.push_back(libCuePointParams);
		cuePointParamsMap.clear();
		extensionChildrenMap.clear();
		guidVector.clear();
		guidVector.reserve(2);

#if XMP_MARKER_EXTENSIBILITY_BACKWARD_COMPATIBILITY

		if(extensionRootNode) {
			for (size_t childIter = 0; childIter < extensionRootNode->children.size(); ++childIter) {

				extensionChildrenMap[extensionRootNode->children[childIter]->name] = childIter;
			}
		}

#endif
		bool entryFoundFirstInCuePointParams = guidPresentInCPP;
		for (XMP_Index cpp = 0; cpp < cppCount; ++cpp) {

			XMP_Node * libCPP = new XMP_Node(libCuePointParams, kXMP_ArrayItemName, kXMP_PropValueIsStruct);
			if (libCPP == 0) XMP_Throw("Can't create cuePointParams", kXMPErr_NoMemory);
			libCuePointParams->children.push_back(libCPP);

			const XMPDMO_CuePointInfo & clientCPP = clientCuePointParams[cpp];
			
			AddStringField(clientCPP.key, keyNodeName.c_str(), libCPP);
			AddStringField(clientCPP.value, valueNodeName.c_str(), libCPP);
		
			if ( !clientCPP.key.ptr || !clientCPP.value.ptr ) continue;

#if XMP_MARKER_EXTENSIBILITY_BACKWARD_COMPATIBILITY
			if (!strcmp(clientCPP.key.ptr, markerGUID.c_str())) {

				guidVector.push_back(cpp);

				guidPresentInCPP = true;
				if (!isGUIDPresentInMarkerStructure) {
					entryFoundFirstInCuePointParams = true;
					guidString = clientCPP.value.ptr;
					AddStringField(guidString, guidNodeName.c_str(), libMarker);
				}
				isGUIDPresentInMarkerStructure = true;
				
			}
			
			else if (IsKeyValidExtension(clientCPP.key.ptr)) {
				cuePointParamsMap[clientCPP.key.ptr].push_back(cpp);
				
				XMP_VarString extensionName;
				if (!GetExtensionNameFromCPPKey(clientCPP.key.ptr, extensionName)) continue;
				if (!extensionRootNode) {
					extensionRootNode = new XMP_Node(libMarker, "xmp:Extensions", kXMP_PropValueIsStruct);
				}
				XMP_Node * extensionChildNode = new XMP_Node(extensionRootNode, extensionName.c_str(), kXMP_PropValueIsStruct);
				if (!(CreateExtensionNode(&extensionChildNode, clientCPP.value.ptr, "&quot;") || CreateExtensionNode(&extensionChildNode, clientCPP.value.ptr, "\""))) {

					delete extensionChildNode;
				}
				else {
					if (extensionChildNode) {
						
						mergeNodes(&extensionRootNode, extensionChildNode, extensionChildrenMap);
					}
				}
			}	
#endif
		}

#if XMP_MARKER_EXTENSIBILITY_BACKWARD_COMPATIBILITY

		if (extensionRootNode && !extensionRootNode->children.size()) {
			delete extensionRootNode;
			extensionRootNode = NULL;
		}
		else {

			if (extensionRootNode && extensionRootNode->children.size()) {
				libMarker->children.push_back(extensionRootNode);
			}
		}
		if (!isGUIDPresentInMarkerStructure) {
			
			XMPDocOps::CreateID("xmp:id", &newGUID);
			AddStringField(newGUID.c_str(), guidNodeName.c_str(), libMarker);
			guidString = newGUID.c_str();
			isGUIDPresentInMarkerStructure = true;
		}

		if (extensionRootNode != NULL) {
			size_t extensionCount = extensionRootNode->children.size();
			for (size_t extensionIter = 0; extensionIter < extensionCount; extensionIter++) {
				const XMP_Node * extensionChildNode = extensionRootNode->children[extensionIter];

				if (!IsSuitableForJSONSerialization(extensionChildNode)) continue;
				
				GetSerializedJSONForExtensionNode(extensionChildNode, extensionNodeAsKey, extensionNodeSerializedAsJSON);
				if (!cuePointParamsMap[extensionNodeAsKey].size()) {
					XMP_Node * extensionNodeInCuePointParams = new XMP_Node(libCuePointParams, kXMP_ArrayItemName, kXMP_PropValueIsStruct);
					if (extensionNodeInCuePointParams == 0) XMP_Throw("Can't create node in cuePointParams", kXMPErr_NoMemory);
					libCuePointParams->children.push_back(extensionNodeInCuePointParams);
					AddStringField(extensionNodeAsKey, keyNodeName.c_str(), extensionNodeInCuePointParams);
					AddStringField(extensionNodeSerializedAsJSON, valueNodeName.c_str(), extensionNodeInCuePointParams);
				}
				else {

					
					std::vector<size_t> & cuePointParamsVectorByKey = cuePointParamsMap[extensionNodeAsKey];
					size_t extensionValuesPresentInCuePointParams = cuePointParamsVectorByKey.size();
					size_t start = (entryFoundFirstInCuePointParams) ? 1 : 0;
					for (size_t index = start; index < extensionValuesPresentInCuePointParams; index++) {
						size_t cuePointParamsIndex = cuePointParamsVectorByKey[index];

						XMP_Node *keyNode(NULL), * valueNode(NULL);

						if (!IsValidCuePointParam(libCuePointParams->children[cuePointParamsIndex], keyNode, valueNode)) continue;;
					
						valueNode->SetValue(extensionNodeSerializedAsJSON.c_str());
					}
				}
			}
		}
		if(isGUIDPresentInMarkerStructure) {

			if (!guidPresentInCPP) {
				XMP_Node * guidNodeInCuePointParams = new XMP_Node(libCuePointParams, kXMP_ArrayItemName, kXMP_PropValueIsStruct);
				if (guidNodeInCuePointParams == 0) XMP_Throw("Can't create node in cuePointParams", kXMPErr_NoMemory);
				libCuePointParams->children.push_back(guidNodeInCuePointParams);
				AddStringField(markerGUID, keyNodeName.c_str(), guidNodeInCuePointParams);
				AddStringField(guidString, valueNodeName.c_str(), guidNodeInCuePointParams);
			}
			else {
				
				size_t guidPresentInCuePointParams = guidVector.size();
				size_t cuePointParamsIndex = 0;
				XMP_Node * keyNode(NULL), *valueNode(NULL);
				for (size_t index = 0; index < guidPresentInCuePointParams; index++) {
					cuePointParamsIndex = guidVector[index];	
					if (!IsValidCuePointParam(libCuePointParams->children[cuePointParamsIndex], keyNode, valueNode)) continue;
					
					valueNode->SetValue(guidString);
				}
			}
		}
#endif
		if (libCuePointParams && !libCuePointParams->children.size()) {
			
			size_t libMarkerChildCount = libMarker->children.size();
			size_t idxToBeCleared = libMarkerChildCount;
			for (size_t childIdx = 0; childIdx < libMarkerChildCount; ++childIdx) {
				if (libMarker->name == libCuePointParams->name) {
					idxToBeCleared = childIdx;
					break;
				}
			}
			libMarker->children.erase(libMarker->children.begin() + idxToBeCleared);
			delete libCuePointParams;
		}
	}

}	// SetBulkMarkers_v2

#endif	// AdobePrivate
