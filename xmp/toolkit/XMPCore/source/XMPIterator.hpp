#ifndef __XMPIterator_hpp__
#define __XMPIterator_hpp__

// =================================================================================================
// Copyright 2003 Adobe
// All Rights Reserved.
//
// NOTICE:  Adobe permits you to use, modify, and distribute this file in accordance with the terms
// of the Adobe license agreement accompanying it. If you have received this file from a source other 
// than Adobe, then your use, modification, or distribution of it requires the prior written permission
// of Adobe.
// =================================================================================================

#if AdobePrivate
// =================================================================================================
// Change history, most recent first
// =================================
//
// Writers:
//  AWL Alan Lillich
//
// mm-dd-yy who Description of changes.
//
// 06-11-09 AWL 5.0-c034 Finish threading revamp, implement friendly reader/writer locking.
//
// 03-24-06 AWL 4.0-c001 Adapt for move to ham-perforce, integrate XMPFiles, bump version to 4.
//
// 01-25-06 AWL 3.3-011 Fix iterator compare mistake in DumpNamespaces, detected by VC8. Replace
//						null iterator notion, VC8 complains about compares to default iterator.
// 01-24-06 AWL 3.3-010 Fix null iterator constant for VC8 strictness.
// 05-16-05 AWL 3.3-100 Complete the deBIBification, integrate the internal and SDK source. Bump the
//              version to 3.3 and build to 100, well ahead of main's latest 3.3-009.
//
// 02-11-05 AWL 3.2-002 Add client reference counting.
// 01-28-05 AWL 3.2-001 Remove BIB.
//
// 04-30-04 AWL Add new & delete operators that call BIBMemory functions. Change static objects that
//				require allocation to explicit pointers.
// 04-30-04 AWL Add new & delete operators that call BIBMemory functions. Remove static objects.
// 01-17-04 AWL Move into new Perforce depot, cosmetic cleanup.
// 05-03-03	AWL	Initial start on the new implementation.
//
// =================================================================================================
#endif /* AdobePrivate */

#include "public/include/XMP_Environment.h"
#include "public/include/XMP_Const.h"
#include "XMPCore/source/XMPMeta.hpp"

// =================================================================================================

struct IterNode;
typedef std::vector < IterNode >	IterOffspring;
typedef IterOffspring::iterator		IterPos;

typedef std::pair < IterPos, IterPos > IterPosPair;
typedef std::vector < IterPosPair >	IterPosStack;

enum {	// Values for the visitStage field, used to decide how to proceed past a node.
	kIter_BeforeVisit		= 0,	// Have not visited this node at all.
	kIter_VisitSelf			= 1,	// Have visited this node and returned its value/options portion.
	kIter_VisitQualifiers	= 2,	// In the midst of visiting this node's qualifiers.
	kIter_VisitChildren		= 3		// In the midst of visiting this node's children.
};

struct IterNode {

	XMP_OptionBits	options;
	XMP_VarString	fullPath;
	size_t			leafOffset;
	IterOffspring	children, qualifiers;
	XMP_Uns8		visitStage;
	#if 0	// *** XMP_DebugBuild
		XMP_StringPtr	_pathPtr, _leafPtr;	// *** Not working, need operator=?
	#endif

	IterNode() : options(0), leafOffset(0), visitStage(kIter_BeforeVisit)
	{
		#if 0	// *** XMP_DebugBuild
			_pathPtr = _leafPtr = 0;
		#endif
	};

	IterNode ( XMP_OptionBits _options, const XMP_VarString& _fullPath, size_t _leafOffset )
			 : options(_options), fullPath(_fullPath), leafOffset(_leafOffset), visitStage(kIter_BeforeVisit)
	{
		#if 0	// *** XMP_DebugBuild
			_pathPtr = fullPath.c_str();
			_leafPtr = _pathPtr + leafOffset;
		#endif
	};

};

struct IterInfo {

	XMP_OptionBits	options;
	const XMPMeta *	xmpObj;
	XMP_VarString	currSchema;
	IterPos			currPos, endPos;
	IterPosStack	ancestors;
	IterNode 		tree;
	#if 0	// *** XMP_DebugBuild
		XMP_StringPtr	_schemaPtr;	// *** Not working, need operator=?
	#endif

	IterInfo() : options(0), xmpObj(0)
	{
		#if 0	// *** XMP_DebugBuild
			_schemaPtr = 0;
		#endif
	};

	IterInfo ( XMP_OptionBits _options, const XMPMeta * _xmpObj ) : options(_options), xmpObj(_xmpObj)
	{
		#if 0	// *** XMP_DebugBuild
			_schemaPtr = 0;
		#endif
	};

};

// =================================================================================================

class XMPIterator {
public:
	
	static bool
	Initialize();	// ! For internal use only!
	
	static void
	Terminate() RELEASE_NO_THROW;	// ! For internal use only!

	XMPIterator ( const XMPMeta & xmpObj,	// Construct a property iterator.
				  XMP_StringPtr	  schemaNS,
				  XMP_StringPtr	  propName,
				  XMP_OptionBits  options );

	XMPIterator	( XMP_StringPtr	 schemaNS,	// Construct a table iterator.
				  XMP_StringPtr	 propName,
				  XMP_OptionBits options );

	virtual ~XMPIterator() RELEASE_NO_THROW;

	virtual bool
	Next ( XMP_StringPtr *  schemaNS,
		   XMP_StringLen *  nsSize,
		   XMP_StringPtr *  propPath,
		   XMP_StringLen *  pathSize,
		   XMP_StringPtr *  propValue,
		   XMP_StringLen *  valueSize,
		   XMP_OptionBits * propOptions );

	virtual void
	Skip ( XMP_OptionBits options );

	// ! Expose so that wrappers and file static functions can see the data.

	XMP_Int32 clientRefs;	// ! Must be signed to allow decrement from 0.
	XMP_ReadWriteLock lock;

	IterInfo info;

private:

	// ! These are hidden on purpose:
	XMPIterator() : clientRefs(0)
		{ XMP_Throw ( "Call to hidden constructor", kXMPErr_InternalFailure ); };
	XMPIterator ( const XMPIterator & /* original */ ) : clientRefs(0)
		{ XMP_Throw ( "Call to hidden constructor", kXMPErr_InternalFailure ); };
	void operator= ( const XMPIterator & /* rhs */ )
		{ XMP_Throw ( "Call to hidden operator=", kXMPErr_InternalFailure ); };

};

// =================================================================================================

#endif	// __XMPIterator_hpp__
