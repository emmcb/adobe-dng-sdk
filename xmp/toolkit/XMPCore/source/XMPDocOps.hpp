#if ! __XMPDocOps_hpp__
#define __XMPDocOps_hpp__ 1

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
// AWL Alan Lillich
// JEH Joerg Ehrlich
//
// mm-dd-yy who Description of changes, most recent on top.
//
// 12-21-14 AJ  5.6-c031 Routed XMPDocOps to new DOM APIs.
// 11-22-11	JEH	5.3-c008 [3045488] Prune document's History metadata if it grows too large.
//							Moved DocOps utilities to distinct class, to be able to write unit tests
//
// 07-16-09 AWL 5.0-c046 Fix XMPDocOps::Clone to not copy all of the thread lock state.
// 06-11-09 AWL 5.0-c034 Finish threading revamp, implement friendly reader/writer locking.
//
// 04-24-08 AWL 4.2.2-c051 [1765983] Add SXMPDocOps::IsDirty overload to return reason mask.
//
// 02-22-08 AWL 4.2-c041 Add routines to SXMPDocOps - EnsureIDsExist, IsDirty, Clone, and GetMetaRef.
// 12-10-07 AWL Change name of fileType params to mimeType.
// 12-03-07 AWL 4.2-c028 Change file type parameters in XMPDocOps from XMP_FileFormat enum to a MIME string.
// 11-12-07 AWL 4.2-c026 More progress implementing XMPDocOps.
// 11-07-07 AWL 4.2-c025 More progress implementing XMPDocOps.
// 11-02-07 AWL 4.2-c024 Start implementing XMPDocOps.
// 10-31-07 AWL 4.2-c023 Add new class XMPDocOps.
//
// =================================================================================================
#endif // AdobePrivate

#include "public/include/XMP_Environment.h"
#include "public/include/XMP_Const.h"
#include "XMPCore/source/XMPCore_Impl.hpp"

#include "XMPCore/source/XMPMeta.hpp"
#if XMP_AndroidBuild
	#include "XMPCore/source/Android_Utils.hpp"
#endif

// =================================================================================================

class XMPDocOps {
public:

	static bool Initialize();	// ! For internal use only!

	static void Terminate() RELEASE_NO_THROW;	// ! For internal use only!

	XMPDocOps();
	virtual ~XMPDocOps() RELEASE_NO_THROW;

	// ---------------------------------------------------------------------------------------------

	static void SetAppName ( XMP_StringPtr appName );

	static void CreateID ( XMP_StringPtr   prefix,
						   XMP_VarString * idStr );
#if XMP_AndroidBuild
    static void SetJVM ( JavaVM* vm );
#endif

	// ---------------------------------------------------------------------------------------------

	void NewXMP ( XMPMeta *      xmpMeta,
				  XMP_StringPtr  mimeType,
				  XMP_OptionBits options );

	void OpenXMP ( XMPMeta *      xmpMeta,
				   XMP_StringPtr  mimeType,
				   XMP_StringPtr  filePath,
				   XMP_DateTime * fileModTime,
				   XMP_OptionBits options );

	void BranchXMP ( XMPDocOps *    derivedDoc,
					 XMPMeta *      derivedMeta,
					 XMP_StringPtr  mimeType,
					 XMP_OptionBits options );

	void PrepareForSave ( XMP_StringPtr  mimeType,
						  XMP_StringPtr  filePath,
						  XMP_DateTime * fileModTime,
						  XMP_OptionBits options );

	void NoteChange ( XMP_StringPtr * partsPtr,
					  XMP_Index       partsCount );

	virtual XMP_TriState HasPartChanged ( XMP_StringPtr   prevInstanceID,
								  XMP_StringPtr * partsPtr,
								  XMP_Index       partsCount ) const;

	virtual bool GetPartChangeID ( XMP_StringPtr * partsPtr,
						   XMP_Index       partsCount,
						   XMP_StringPtr * idStr,
						   XMP_StringLen * idLen ) const;

	bool EnsureIDsExist ( XMP_OptionBits options );
	
	bool IsDirty ( XMP_OptionBits * reasons = 0 ) const;
	
	void Clone ( XMPDocOps * clone, XMP_OptionBits options ) const;
	
	XMPMetaRef GetMetaRef() const;

	// ! Expose so that wrappers and file static functions can see the data.

	XMP_Int32 clientRefs;	// ! Must be signed to allow decrement from zero.
	XMP_ReadWriteLock lock;

	XMPMeta * docXMP;

protected:

	// ! Any data member changes must be propagted to the Clone function!

	bool isNew, isDirty, isDerived, contentChanged, allHistoryCleaned;
	
	XMP_OptionBits dirtyReasons;
	
	std::string prevMIMEType, prevFilePath, nextInstanceID;
	
	std::vector<std::string> changedParts;
	
	void ResetDoc ( XMPMeta * xmpMeta );
	
	void InternalNoteChangeAll();
	
	void AppendHistory ( XMP_StringPtr action, XMP_StringPtr params );
	void AppendHistory ( XMP_StringPtr action, XMP_StringPtr params,
						 XMP_StringPtr instanceID, XMP_StringPtr when );

	virtual void PruneRedundantHistory();
};	// XMPDocOps

#endif  // __XMPDocOps_hpp__
