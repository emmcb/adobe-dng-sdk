#if ! __XMPDocOpsUtils_hpp__
#define __XMPDocOpsUtils_hpp__ 1

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
// 12-21-14 AJ  5.6-c031 Routed XMPDocOps to new DOM APIs.
// 06-08-12 AWL 5.5-c002 Use standard O/S utilities for GUID creation.
//
// 11-22-11	JEH	5.3-c008 Moved DocOps utilities to this class, to be able to write unit tests
//
// =================================================================================================
#endif // AdobePrivate

#include "public/include/XMP_Environment.h"
#include "public/include/XMP_Const.h"
#include "XMPCore/source/XMPMeta.hpp"
#if AdobePrivate
#include "XMPCore/XMPCoreDefines_I.h"
	#if ENABLE_CPP_DOM_MODEL
		#include "XMPCore/XMPCoreFwdDeclarations_I.h"
	#endif // ENABLE_CPP_DOM_MODEL
#endif // AdobePrivate
// =================================================================================================

class XMPDocOpsUtils
{
public:

	XMPDocOpsUtils() {}
	~XMPDocOpsUtils() {}

	// Conjure a UUID and format it as a URI.
	static void	ConjureUURI(XMP_StringPtr prefix, XMP_VarString * idStr, XMP_ReadWriteLock * threadLock = 0);
	// Look for the last "saved" or "created" xmpMM:History entry and extract its timestamp.
	static bool	GetLastModTime(const XMPMeta & xmpMeta, XMP_DateTime * modTime);
	// Make sure any existing xmpMM:History is an array, delete it if not.
	static void EnsureHistoryForm(XMPMeta * xmpMeta);
	// Copies history resource references
	static void	FillResourceRef(const XMPMeta & srcXMP, XMPMeta * destXMP, XMP_StringPtr destNS, XMP_StringPtr destProp);
	// Enforce a leading '/', strip the tail of the path if it has any unknown operators.
	static void	NormalizePartPath(std::string & part);
	// Sets a field in the last history entry
	static inline void AppendHistoryField(XMPMeta * xmpMeta, XMP_StringPtr field, XMP_StringPtr value)
	{
		XMP_Assert(field != 0);
		XMP_Assert(value != 0);

		xmpMeta->SetStructField(kXMP_NS_XMP_MM, "History[last()]", kXMP_NS_XMP_ResourceEvent, field, value, kXMP_NoOptions);
	}	// AppendHistoryField

	// Go forward through the history and normalize the stEvt:changed fields.
	static void	NormalizePartsList(XMP_NodePtr stEvt_changed);
	// Prune redundant "saved" entries from the history, looking for a run ending at the given index.
	static XMP_Int32 PruneOneRedundantRun(XMP_NodePtr historyNode, XMP_Int32 endIndex);
	// Look for a specific part in the list of parts from saved events
	static bool	IsPartInList(XMP_StringPtr * newPartsPtr, XMP_Index newPartsCount, const XMP_Node & oldPartsNode);
#if AdobePrivate
	#if ENABLE_CPP_DOM_MODEL
	static XMP_Int32 PruneOneRedundantRun_v2( const AdobeXMPCore::spIArrayNode & historyNode,  XMP_Int32 endIndex);
	static bool IsPartInList_v2(XMP_StringPtr * newPartsPtr, XMP_Index newPartsCount, const AdobeXMPCore::spINode & oldPartsNode);
	static void NormalizePartsList_v2( const AdobeXMPCore::spINode & stEvt_changed);
	static bool GetLastModTime_v2(const XMPMeta & xmpMeta, XMP_DateTime * modTime);
	#endif
#endif

};	// XMPDocOpsUtils

#endif  // __XMPDocOpsUtils_hpp__
