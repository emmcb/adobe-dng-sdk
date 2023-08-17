#ifndef __XMPCore_Impl_hpp__
#define __XMPCore_Impl_hpp__ 1

// =================================================================================================
// Copyright 2004 Adobe
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
//  AWL Alan Lillich
//
// mm-dd-yy who Description of changes, most recent on top.
//
// 02-06-15 AJ  5.6-c037 Fixing warnings due to implicit typecasting
// 07-31-14 ADC 5.6-c022 [3790748] PDFLChef crashes/hangs indefinitely on main branch due to xmp library.
// 07-10-14 ADC 5.6-c015 Refactoring, partial documentation and bug fixes of XMPCommon and XMPCore C++ APIs.
//
// 08-14-12 AWL 5.5-c009 Add XMPCore error notifications for RDF parsing errors.
//
// 07-01-10 AWL 5.1-c008 Add XMP_Node::GetLocalURI.
//
// 10-22-09 AWL 5.0-c056 [2422169,2422172] Fix ApplyTemplate to not add empty array items or struct fields.
// 06-18-09 AWL 5.0-c037 Minor tweaks from code review.
// 06-11-09 AWL 5.0-c034 Finish threading revamp, implement friendly reader/writer locking.
// 05-21-09 AWL 5.0-c032 Revamp glue again to pass SetClientString with each function.
// 05-19-09 AWL 5.0-c031 First part of threading improvements, revamp the client glue.
// 05-14-09 AWL 5.0-c030 Improve the call tracing mechanism.
// 04-03-09 AWL 5.0-c022 Change PlainXMP to TransformXMP.
//
// 07-01-08 AWL 4.3-c060 Hide all uses of PlainXMP.
//
// 02-05-08 AWL 4.2-c038 Use client memory routines for malloc/free also, so that leak checking works.
// 11-30-07 AWL 4.2-c027 Expose XML_Node and ExpatAdapter so that XMPFiles can use them.
// 11-07-07 AWL 4.2-c025 More progress implementing XMPDocOps.
// 10-31-07 AWL 4.2-c023 Add new class XMPDocOps.
// 08-24-07 AWL 4.2-c019 Remove support for ancient CXMP_* init/term routines and non-public (AXE) expat.
// 07-17-07 AWL 4.2-c018 Change Mac to use pthread synchronization, needed for 64-bit builds.
//
// 10-17-06 AWL 4.1-c026 To be on the safe side, don't try to delete null children or qualifiers.
// 10-12-06 AWL 4.1-c021 [1235816] Remove the new/delete overrides from static builds.
//
// 06-16-06 AWL 4.0-c008 Make XMP_ENTER_WRAPPER check sXMPInitCount in debug builds.
// 05-01-06 AWL 4.0-c005 Revamp the embedded, Mac PList, and WIndows Property info.
// 03-24-06 AWL 4.0-c001 Adapt for move to ham-perforce, integrate XMPFiles, bump version to 4.
//
// 02-03-06 AWL 3.3-013 Add exception tracing to the XMP_Impl support macros. Rearrange XMP_Impl.hpp
//				to be make it easier to find things.
// 01-25-06 AWL 3.3-011 Fix iterator compare mistake in DumpNamespaces, detected by VC8. Replace
//						null iterator notion, VC8 complains about compares to default iterator.
// 01-24-06 AWL 3.3-010 Fix null iterator constant for VC8 strictness.
// 08-08-05 AWL 3.3-004 Change ResolveAlias to allow general alias path.
//
// 06-03-05 AWL 3.2-107 Make the XMP_Node and XML_Node destructors be virtual.
// 05-16-05 AWL 3.2-100 Complete the deBIBification, integrate the internal and SDK source.
// 04-11-05 AWL 3.2-016 Add AdobePrivate conditionals where appropriate.
// 04-05-05 AWL 3.2-011 [0532345] Normalize xml:lang values so that compares are in effect case
//				insensitive as required by RFC 3066. Change parsing and serializing to force the
//				x-default item to be first.
// 04-01-05 AWL 3.2-010 Add leafOptions parameter to FindNode, used when creating new nodes.
// 04-01-05 AWL 3.2-009 [0621112,1033629] Add thread locks for UNIX.
// 03-17-05 AWL 3.2-006 Revise Plain XMP parsing and serialization for latest proposal.
// 02-16-05 AWL 3.2-005 Add first cut of Plain XMP parsing and serialization.
// 02-14-05 AWL 3.2-003 Add thread locks.
// 01-28-05 AWL 3.2-001 Remove BIB.
//
// 01-25-05 AWL 3.1.1-106 [1141007] Add XMPMeta::RegisterAssertNotify.
// 11-04-04 AWL 3.1.1-090 [1014853] Add XMPUtils::RemoveMultiValueInfo. Fix AppendProperties to call
//				RemoveMultiValueInfo, i.e. to mimic a user edit.
// 10-06-04 AWL 3.1.1-083 [1061778] Add lock tracing under TraceXMPLocking.
//
// 07-29-04 AWL 3.1-071 [1014855] Normalize iterator root path by expanding and recompressing.
// 07-13-04 AWL 3.1-058 [1014255] Remove empty schema nodes.
// 06-23-04 AWL 3.1-048 Mac side of BIB threading fixes. Mainly #define BIB_MULTI_THREAD.
//
// 04-30-04 AWL Add new & delete operators that call BIBMemory functions. Change static objects that
//				require allocation to explicit pointers. Clean up memory leaks.
// 03-17-04 AWL Cleanup error exceptions, make sure all have a reasonable message.
// 02-17-04 AWL Add CompareSubtrees.
// 02-10-04 AWL Add ConjureUUID.
// 01-17-04 AWL Move into new Perforce depot, cosmetic cleanup.
//
// =================================================================================================
#endif // AdobePrivate

#include "public/include/XMP_Environment.h"	// ! Must be the first #include!
#include "public/include/XMP_Const.h"
#include "build/XMP_BuildInfo.h"
#include "source/XMP_LibUtils.hpp"

// #include "XMPCore/source/XMPMeta.hpp"

#include "public/include/client-glue/WXMP_Common.hpp"

#include <vector>
#include <string>
#include <map>
#include <cassert>
#include <cstring>
#include <cstdlib>

#if XMP_WinBuild
	#pragma warning ( disable : 4244 )	// possible loss of data (temporary for 64 bit builds)
	#pragma warning ( disable : 4267 )	// possible loss of data (temporary for 64 bit builds)
#endif

// =================================================================================================
// Primary internal types

class XMP_Node;
class XML_Node;
class XPathStepInfo;

typedef XMP_Node *	XMP_NodePtr;

typedef std::vector<XMP_Node*>		XMP_NodeOffspring;
typedef XMP_NodeOffspring::iterator	XMP_NodePtrPos;

typedef XMP_VarString::iterator			XMP_VarStringPos;
typedef XMP_VarString::const_iterator	XMP_cVarStringPos;

typedef std::vector < XPathStepInfo >		XMP_ExpandedXPath;
typedef XMP_ExpandedXPath::iterator			XMP_ExpandedXPathPos;
typedef XMP_ExpandedXPath::const_iterator	XMP_cExpandedXPathPos;

typedef std::map < XMP_VarString, XMP_ExpandedXPath > XMP_AliasMap;	// Alias name to actual path.
typedef XMP_AliasMap::iterator			XMP_AliasMapPos;
typedef XMP_AliasMap::const_iterator	XMP_cAliasMapPos;

// =================================================================================================
// General global variables and macros

extern XMP_Int32 sXMP_InitCount;

#if !AdobePrivate
typedef void * (*XMP_AllocateProc) (size_t size);

typedef void(*XMP_DeleteProc)   (void * ptr);

#endif
#if ! XMP_StaticBuild

	extern XMP_AllocateProc sXMP_MemAlloc;
	extern XMP_DeleteProc   sXMP_MemFree;

	#define malloc(size) (*sXMP_MemAlloc) ( size )
	#define free(addr)   (*sXMP_MemFree) ( addr )

#endif


extern XMP_Bool sUseNewCoreAPIs;
extern XMP_NamespaceTable * sRegisteredNamespaces;

extern XMP_AliasMap * sRegisteredAliasMap;

extern XMP_ReadWriteLock * sDefaultNamespacePrefixMapLock;

#define WtoXMPMeta_Ref(xmpRef)	(const XMPMeta &) (*((XMPMeta*)(xmpRef)))
#define WtoXMPMeta_Ptr(xmpRef)	((XMPMeta*)(xmpRef))

#define WtoXMPDocOps_Ptr(docRef)	((XMPDocOps*)(docRef))

#if AdobePrivate
	extern XMP_AssertNotifyProc sAssertNotify;
	extern void *               sAssertRefCon;
#endif


// **** see CTECHXMP-4169947 ***//

//extern void *			voidVoidPtr;	// Used to backfill null output parameters.
//extern XMP_StringPtr	voidStringPtr;
//extern XMP_StringLen	voidStringLen;
//extern XMP_OptionBits	voidOptionBits;
//extern XMP_Bool			voidByte;
//extern bool				voidBool;
//extern XMP_Int32		voidInt32;
//extern XMP_Int64		voidInt64;
//extern double			voidDouble;
//extern XMP_DateTime		voidDateTime;
//extern WXMP_Result		void_wResult;

//#if AdobePrivate
//	extern XMP_DerivedDocInfo	voidDocInfo;
//	extern XMP_EmbeddedDocInfo	voidEmbeddedInfo;
//	extern XMP_PacketInfo       voidPacketInfo;
//#endif

#define kHexDigits "0123456789ABCDEF"

#define XMP_LitMatch(s,l)		(strcmp((s),(l)) == 0)
#define XMP_LitNMatch(s,l,n)	(strncmp((s),(l),(n)) == 0)
	// *** Use the above macros!

#if XMP_WinBuild
	#define snprintf _snprintf
#endif

// =================================================================================================
// Version info

#if XMP_DebugBuild
	#define kXMPCore_DebugFlag 1
#else
	#define kXMPCore_DebugFlag 0
#endif

#define kXMPCore_VersionNumber	( (kXMPCore_DebugFlag << 31)    |	\
                                  (XMP_API_VERSION_MAJOR << 24) |	\
						          (XMP_API_VERSION_MINOR << 16) |	\
						          (XMP_API_VERSION_MICRO << 8) )

#if AdobePrivate
	#define kXMPCoreName   "Adobe XMP Core"
	#define kXMPCore_BasicVersion   XMPCORE_API_VERSION_STRING "-c" kXMPCore_EngString " " BUILDVERSIONSTR
	#define kXMPCore_VersionString	kXMPCoreName " " kXMPCore_BasicVersion
	#define kXMPCore_VersionMessage	kXMPCore_VersionString ", " BUILDDATESTR kXMPCore_DebugString
#else
	#define kXMPCoreName "XMP Core"
	#define kXMPCore_VersionMessage	kXMPCoreName " " XMPCORE_API_VERSION_STRING
#endif

// =================================================================================================
// Support for call tracing

#ifndef XMP_TraceCoreCalls
	#define XMP_TraceCoreCalls			0
	#define XMP_TraceCoreCallsToFile	0
#endif

#if XMP_TraceCoreCalls

	#undef AnnounceThrow
	#undef AnnounceCatch

	#undef AnnounceEntry
	#undef AnnounceNoLock
	#undef AnnounceExit

	extern FILE * xmpCoreLog;

	#define AnnounceThrow(msg)	\
		fprintf ( xmpCoreLog, "XMP_Throw: %s\n", msg ); fflush ( xmpCoreLog )
	#define AnnounceCatch(msg)	\
		fprintf ( xmpCoreLog, "Catch in %s: %s\n", procName, msg ); fflush ( xmpCoreLog )

	#define AnnounceEntry(proc)			\
		const char * procName = proc;	\
		fprintf ( xmpCoreLog, "Entering %s\n", procName ); fflush ( xmpCoreLog )
	#define AnnounceNoLock(proc)		\
		const char * procName = proc;	\
		fprintf ( xmpCoreLog, "Entering %s (no lock)\n", procName ); fflush ( xmpCoreLog )
	#define AnnounceExit()	\
		fprintf ( xmpCoreLog, "Exiting %s\n", procName ); fflush ( xmpCoreLog )

#endif

// =================================================================================================
// ExpandXPath, FindNode, and related support

// *** Normalize the use of "const xx &" for input params

#define kXMP_ArrayItemName	"[]"

#define kXMP_CreateNodes	true
#define kXMP_ExistingOnly	false

#define FindConstSchema(t,u)	FindSchemaNode ( const_cast<XMP_Node*>(t), u, kXMP_ExistingOnly, 0 )
#define FindConstChild(p,c)		::FindChildNode ( const_cast<XMP_Node*>(p), c, kXMP_ExistingOnly, 0 )
#define FindConstQualifier(p,c)	FindQualifierNode ( const_cast<XMP_Node*>(p), c, kXMP_ExistingOnly, 0 )
#define FindConstNode(t,p)		::FindNode ( const_cast<XMP_Node*>(t), p, kXMP_ExistingOnly, 0 )

void
SplitNameAndValue(const XMP_VarString & selStep, 
				  XMP_VarString *       nameStr, 
				  XMP_VarString *       valueStr);

extern XMP_OptionBits
VerifySetOptions ( XMP_OptionBits options, XMP_StringPtr propValue );

extern void
ComposeXPath ( const XMP_ExpandedXPath & expandedXPath,
			   XMP_VarString * stringXPath );

extern void
ExpandXPath	( XMP_StringPtr			schemaNS,
			  XMP_StringPtr			propPath,
			  XMP_ExpandedXPath *	expandedXPath );

typedef bool (*PrefixSearchFnPtr) ( void * privateData, XMP_StringPtr nsURI, XMP_StringPtr * namespacePrefix, XMP_StringLen * prefixSize );

extern XMP_Node *
FindSchemaNode ( XMP_Node *		  xmpTree,
				 XMP_StringPtr	  nsURI,
				 bool			  createNodes,
				 XMP_NodePtrPos * ptrPos = 0,
				 PrefixSearchFnPtr prefixSearchFnPtr = NULL,
				 void * privateData = NULL );

extern XMP_Node *
FindChildNode ( XMP_Node *		 parent,
				XMP_StringPtr	 childName,
				bool			 createNodes,
				XMP_NodePtrPos * ptrPos = 0 );

extern XMP_Node *
FindQualifierNode ( XMP_Node *		 parent,
					XMP_StringPtr	 qualName,
					bool			 createNodes,
					XMP_NodePtrPos * ptrPos = 0 );

extern XMP_Node *
FindNode ( XMP_Node *		xmpTree,
		   const XMP_ExpandedXPath & expandedXPath,
		   bool				createNodes,
		   XMP_OptionBits	leafOptions = 0,
		   XMP_NodePtrPos * ptrPos = 0 );

extern XMP_Index
LookupLangItem ( const XMP_Node * arrayNode, XMP_VarString & lang );	// ! Lang must be normalized!

extern XMP_Index
LookupFieldSelector ( const XMP_Node * arrayNode, XMP_StringPtr fieldName, XMP_StringPtr fieldValue );

extern void
CloneOffspring ( const XMP_Node * origParent, XMP_Node * cloneParent, bool skipEmpty = false );

extern XMP_Node *
CloneSubtree ( const XMP_Node * origRoot, XMP_Node * cloneParent, bool skipEmpty = false );

extern bool
CompareSubtrees ( const XMP_Node & leftNode, const XMP_Node & rightNode );

extern void
DeleteSubtree ( XMP_NodePtrPos rootNodePos );

extern void
DeleteEmptySchema ( XMP_Node * schemaNode );

extern void
NormalizeLangValue ( XMP_VarString * value );

extern void
NormalizeLangArray ( XMP_Node * array );

extern void
DetectAltText ( XMP_Node * xmpParent );

extern void
SortNamedNodes ( XMP_NodeOffspring & nodeVector );

static inline bool
IsPathPrefix ( XMP_StringPtr fullPath, XMP_StringPtr prefix )
{
	bool isPrefix = false;
	XMP_StringLen prefixLen = static_cast< XMP_StringLen >( strlen(prefix) );
	if ( XMP_LitNMatch ( prefix, fullPath, prefixLen ) ) {
		char separator = fullPath[prefixLen];
		if ( (separator == 0) || (separator == '/') ||
			 (separator == '[') || (separator == '*') ) isPrefix = true;
	}
	return isPrefix;
}

// -------------------------------------------------------------------------------------------------

class XPathStepInfo {
public:
	XMP_VarString	step;
	XMP_OptionBits	options;
	XPathStepInfo ( XMP_StringPtr _step, XMP_OptionBits _options ) : step(_step), options(_options) {};
	XPathStepInfo ( XMP_VarString _step, XMP_OptionBits _options ) : step(_step), options(_options) {};
private:
	XPathStepInfo() : options(0) {};	// ! Hide the default constructor.
};

enum { kSchemaStep = 0, kRootPropStep = 1, kAliasIndexStep = 2 };

enum {	// Bits for XPathStepInfo options.	// *** Add mask check to init code.
	kXMP_StepKindMask		= 0x0F,	// ! The step kinds are mutually exclusive numbers.
	kXMP_StructFieldStep	= 0x01,	// Also for top level nodes (schema "fields").
	kXMP_QualifierStep		= 0x02,	// ! Order is significant to separate struct/qual from array kinds!
	kXMP_ArrayIndexStep		= 0x03,	// ! The kinds must not overlay array form bits!
	kXMP_ArrayLastStep		= 0x04,
	kXMP_QualSelectorStep	= 0x05,
	kXMP_FieldSelectorStep	= 0x06,
	kXMP_StepIsAlias        = 0x10
};

#define GetStepKind(f)	((f) & kXMP_StepKindMask)

#define kXMP_NewImplicitNode	kXMP_InsertAfterItem

// =================================================================================================
// XMP_Node details

#if 0	// Pattern for iterating over the children or qualifiers:
	for ( size_t xxNum = 0, xxLim = _node_->_offspring_.size(); xxNum < xxLim; ++xxNum ) {
		const XMP_Node * _curr_ = _node_->_offspring_[xxNum];
	}
#endif

class XMP_Node {
public:

	XMP_OptionBits		options;
	XMP_VarString		name, value;
	XMP_Node *			parent;
	XMP_NodeOffspring	children;
	XMP_NodeOffspring	qualifiers;
	#if XMP_DebugBuild
		// *** XMP_StringPtr	_namePtr, _valuePtr;	// *** Not working, need operator=?
	#endif

	XMP_Node ( XMP_Node * _parent, XMP_StringPtr _name, XMP_OptionBits _options )
		: options(_options), name(_name), parent(_parent)
	{
		#if XMP_DebugBuild
			XMP_Assert ( (name.find ( ':' ) != XMP_VarString::npos) || (name == kXMP_ArrayItemName) ||
			             (options & kXMP_SchemaNode) || (parent == 0) );
			// *** _namePtr  = name.c_str();
			// *** _valuePtr = value.c_str();
		#endif
	};

	XMP_Node ( XMP_Node * _parent, const XMP_VarString & _name, XMP_OptionBits _options )
		: options(_options), name(_name), parent(_parent)
	{
		#if XMP_DebugBuild
			XMP_Assert ( (name.find ( ':' ) != XMP_VarString::npos) || (name == kXMP_ArrayItemName) ||
			             (options & kXMP_SchemaNode) || (parent == 0) );
			// *** _namePtr  = name.c_str();
			// *** _valuePtr = value.c_str();
		#endif
	};

	XMP_Node ( XMP_Node * _parent, XMP_StringPtr _name, XMP_StringPtr _value, XMP_OptionBits _options )
		: options(_options), name(_name), value(_value), parent(_parent)
	{
		#if XMP_DebugBuild
			XMP_Assert ( (name.find ( ':' ) != XMP_VarString::npos) || (name == kXMP_ArrayItemName) ||
			             (options & kXMP_SchemaNode) || (parent == 0) );
			// *** _namePtr  = name.c_str();
			// *** _valuePtr = value.c_str();
		#endif
	};

	XMP_Node ( XMP_Node * _parent, const XMP_VarString & _name, const XMP_VarString & _value, XMP_OptionBits _options )
		: options(_options), name(_name), value(_value), parent(_parent)
	{
		#if XMP_DebugBuild
			XMP_Assert ( (name.find ( ':' ) != XMP_VarString::npos) || (name == kXMP_ArrayItemName) ||
			             (options & kXMP_SchemaNode) || (parent == 0) );
			// *** _namePtr  = name.c_str();
			// *** _valuePtr = value.c_str();
		#endif
	};

	void GetLocalURI ( XMP_StringPtr * uriStr, XMP_StringLen * uriSize ) const;

	void GetFullQualifiedName( XMP_StringPtr * uriStr, XMP_StringLen * uriSize, XMP_StringPtr * nameStr, XMP_StringLen * nameSize ) const;

	void RemoveChildren()
	{
		for ( size_t i = 0, vLim = children.size(); i < vLim; ++i ) {
			if ( children[i] != 0 ) delete children[i];
		}
		children.clear();
	}

	void RemoveQualifiers()
	{
		for ( size_t i = 0, vLim = qualifiers.size(); i < vLim; ++i ) {
			if ( qualifiers[i] != 0 ) delete qualifiers[i];
		}
		qualifiers.clear();
	}

	void ClearNode()
	{
		options = 0;
		name.erase();
		value.erase();
		this->RemoveChildren();
		this->RemoveQualifiers();
	}

	void SetValue( XMP_StringPtr value );

	virtual ~XMP_Node() { RemoveChildren(); RemoveQualifiers(); };

private:
	XMP_Node() : options(0), parent(0)	// ! Make sure parent pointer is always set.
	{
		#if XMP_DebugBuild
			// *** _namePtr  = name.c_str();
			// *** _valuePtr = value.c_str();
		#endif
	};

};

class XMP_AutoNode {	// Used to hold a child during subtree construction.
public:
	XMP_Node * nodePtr;
	XMP_AutoNode() : nodePtr(0) {};
	~XMP_AutoNode() { if ( nodePtr != 0 ) delete ( nodePtr ); nodePtr = 0; };
	XMP_AutoNode ( XMP_Node * _parent, XMP_StringPtr _name, XMP_OptionBits _options )
		: nodePtr ( new XMP_Node ( _parent, _name, _options ) ) {};
	XMP_AutoNode ( XMP_Node * _parent, const XMP_VarString & _name, XMP_OptionBits _options )
		: nodePtr ( new XMP_Node ( _parent, _name, _options ) ) {};
	XMP_AutoNode ( XMP_Node * _parent, XMP_StringPtr _name, XMP_StringPtr _value, XMP_OptionBits _options )
		: nodePtr ( new XMP_Node ( _parent, _name, _value, _options ) ) {};
	XMP_AutoNode ( XMP_Node * _parent, const XMP_VarString & _name, const XMP_VarString & _value, XMP_OptionBits _options )
		: nodePtr ( new XMP_Node ( _parent, _name, _value, _options ) ) {};
};

// =================================================================================================

#endif	// __XMPCore_Impl_hpp__
