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
// AJ  Abhishek Jindal
// mm-dd-yy who Description of changes, most recent on top.
//
// 02-06-15 AJ  5.6-c037 Fixing warnings due to implicit typecasting
// 12-21-14 AJ	         Routing XMPDocOps to new DOM APIs
// 06-23-09 AWL 5.0-c040-f049 Add hack to revert to global DLL locking.
// 06-11-09 AWL 5.0-c034 Finish threading revamp, implement friendly reader/writer locking.
// 05-21-09 AWL 5.0-c032 Revamp glue again to pass SetClientString with each function.
// 05-19-09 AWL 5.0-c031 First part of threading improvements, revamp the client glue.
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

#include "public/include/XMP_Environment.h"	// ! This must be the first include!
#include "public/include/XMP_Const.h"

#include "public/include/client-glue/WXMPDocOps.hpp"

#include "XMPCore/source/XMPCore_Impl.hpp"
#include "XMPCore/source/XMPDocOps.hpp"
#if AdobePrivate
#include "XMPCore/XMPCoreDefines_I.h"
#if ENABLE_CPP_DOM_MODEL
#include "XMPCore/source/XMPDocOps2.hpp"
#endif
#endif

#if XMP_WinBuild
	#pragma warning ( disable : 4101 ) // unreferenced local variable
	#pragma warning ( disable : 4189 ) // local variable is initialized but not referenced
	#pragma warning ( disable : 4702 ) // unreachable code
	#pragma warning ( disable : 4800 ) // forcing value to bool 'true' or 'false' (performance warning)
	#if XMP_DebugBuild
		#pragma warning ( disable : 4297 ) // function assumed not to throw an exception but does
	#endif
#endif

#if __cplusplus
extern "C" {
#endif

// =================================================================================================
// CTor/DTor Wrappers
// ==================

void
WXMPDocOps_CTor_1 ( WXMP_Result * wResult )
{
	XMP_ENTER_Static("WXMPDocOps_CTor_1")	// No lib object yet, use the static entry.
		bool isCreated = false;
		XMPDocOps * xmpDoc = 0;
#if AdobePrivate
#if ENABLE_CPP_DOM_MODEL
		if (sUseNewCoreAPIs) {
			xmpDoc = new XMPDocOps2();
			isCreated = true;
		}
#endif
#endif
		if (!isCreated) {
			xmpDoc = new XMPDocOps();
		}
		++xmpDoc->clientRefs;
		XMP_Assert ( xmpDoc->clientRefs == 1 );
		wResult->ptrResult = XMPDocOpsRef ( xmpDoc );

	XMP_EXIT
}

// -------------------------------------------------------------------------------------------------

void
WXMPDocOps_IncrementRefCount_1 ( XMPDocOpsRef xmpObjRef )
{
	WXMP_Result void_wResult;
	WXMP_Result * wResult = &void_wResult;	// ! Needed to "fool" the EnterWrapper macro.
	XMP_ENTER_ObjWrite ( XMPDocOps, "WXMPDocOps_IncrementRefCount_1" )

		++thiz->clientRefs;
		XMP_Assert ( thiz->clientRefs > 0 );

	XMP_EXIT_NoThrow
}

// -------------------------------------------------------------------------------------------------

void
WXMPDocOps_DecrementRefCount_1 ( XMPDocOpsRef xmpObjRef )
{	
	WXMP_Result void_wResult;
	WXMP_Result * wResult = &void_wResult;	// ! Needed to "fool" the EnterWrapper macro.
	XMP_ENTER_ObjWrite ( XMPDocOps, "WXMPDocOps_DecrementRefCount_1" )

		XMP_Assert ( thiz->clientRefs > 0 );
		--thiz->clientRefs;
		if ( thiz->clientRefs <= 0 ) {
			objLock.Release();
			delete ( thiz );
		}

	XMP_EXIT_NoThrow
}

// =================================================================================================
// Class Static Wrappers
// =====================

/* class static */ void
WXMPDocOps_SetAppName_1 ( XMP_StringPtr appName,
						  WXMP_Result * wResult )
{
	XMP_ENTER_Static ( "WXMPDocOps_SetAppName_1" )

		XMPDocOps::SetAppName ( appName );

	XMP_EXIT
}

// -------------------------------------------------------------------------------------------------

/* class static */ void
WXMPDocOps_CreateID_1 ( XMP_StringPtr prefix,
						void *        clientID,
						SetClientStringProc SetClientString,
						WXMP_Result * wResult )
{
	XMP_ENTER_Static ( "WXMPDocOps_CreateID_1" )

		if ( (prefix == 0) || (prefix[0] == 0) ) prefix = "xmp.id";
		XMP_VarString localStr;
		XMPDocOps::CreateID ( prefix, &localStr );
		if ( clientID != 0 ) (*SetClientString) ( clientID, localStr.c_str(), static_cast< XMP_StringLen >( localStr.size() ) );

	XMP_EXIT
}

// -------------------------------------------------------------------------------------------------

#if XMP_AndroidBuild

/* class static */ void
WXMPDocOps_SetJVM_1 ( JavaVM* jvm,
                             WXMP_Result * wResult )
{
    XMP_ENTER_Static ( "WXMPDocOps_SetJVM_1" )
        
        XMPDocOps::SetJVM ( jvm );
        
    XMP_EXIT
}

#endif

    

// =================================================================================================
// Class Method Wrappers
// =====================

void
WXMPDocOps_NewXMP_1 ( XMPDocOpsRef   xmpObjRef,
					  XMPMetaRef     metaRef,
					  XMP_StringPtr  mimeType,
					  XMP_OptionBits options,
					  WXMP_Result *  wResult )
{
	XMP_ENTER_ObjWrite ( XMPDocOps, "WXMPDocOps_NewXMP_1" )

		if ( metaRef == 0 ) XMP_Throw ( "Must pass SXMPMeta pointer", kXMPErr_BadParam );
		if ( mimeType == 0 ) mimeType = "";

		XMPMeta * xmpMeta = WtoXMPMeta_Ptr ( metaRef );
		XMP_AutoLock metaLock ( &xmpMeta->lock, kXMP_WriteLock );

		thiz->NewXMP ( xmpMeta, mimeType, options );

	XMP_EXIT
}

// -------------------------------------------------------------------------------------------------

void
WXMPDocOps_OpenXMP_1 ( XMPDocOpsRef   xmpObjRef,
					   XMPMetaRef     metaRef,
					   XMP_StringPtr  mimeType,
					   XMP_StringPtr  filePath,
					   XMP_DateTime * fileModTime,
					   XMP_OptionBits options,
					   WXMP_Result *  wResult )
{
	XMP_ENTER_ObjWrite ( XMPDocOps, "WXMPDocOps_OpenXMP_1" )

		if ( metaRef == 0 ) XMP_Throw ( "Must pass SXMPMeta pointer", kXMPErr_BadParam );

		if ( mimeType == 0 ) mimeType = "";
		if ( filePath == 0 ) filePath = "";

		XMPMeta * xmpMeta = WtoXMPMeta_Ptr ( metaRef );
		XMP_AutoLock metaLock ( &xmpMeta->lock, kXMP_WriteLock );

		thiz->OpenXMP ( xmpMeta, mimeType, filePath, fileModTime, options );

	XMP_EXIT
}

// -------------------------------------------------------------------------------------------------

void
WXMPDocOps_BranchXMP_1 ( XMPDocOpsRef   xmpObjRef,
					     XMPDocOpsRef   derivedDocRef,
						 XMPMetaRef     derivedMetaRef,
						 XMP_StringPtr  mimeType,
						 XMP_OptionBits options,
						 WXMP_Result *  wResult )
{
	XMP_ENTER_ObjWrite ( XMPDocOps, "WXMPDocOps_BranchXMP_1" )

		if ( derivedDocRef == 0 ) XMP_Throw ( "Must pass derived SXMPDocOps pointer", kXMPErr_BadParam );
		// ! The derivedMetaRef can be null, checked later.

		if ( mimeType == 0 ) mimeType = "";

		XMPDocOps * derivedDoc  = WtoXMPDocOps_Ptr ( derivedDocRef );
		XMP_AutoLock docLock ( &derivedDoc->lock, kXMP_WriteLock, (derivedDoc != thiz) );
		XMPMeta *   derivedMeta = WtoXMPMeta_Ptr ( derivedMetaRef );
		// AUDIT warning C6011: getting the address of the lock is save. 
		// Lock is only dereferenced in metaLock if derivedMeta != 0
		XMP_AutoLock metaLock ( &derivedMeta->lock, kXMP_WriteLock, (derivedMeta != 0) );

		thiz->BranchXMP ( derivedDoc, derivedMeta, mimeType, options );

	XMP_EXIT
}

// -------------------------------------------------------------------------------------------------

void
WXMPDocOps_PrepareForSave_1 ( XMPDocOpsRef   xmpObjRef,
							  XMP_StringPtr  mimeType,
							  XMP_StringPtr  filePath,
							  XMP_DateTime * fileModTime,
							  XMP_OptionBits options,
							  WXMP_Result *  wResult )
{
	XMP_ENTER_ObjWrite ( XMPDocOps, "WXMPDocOps_PrepareForSave_1" )

		if ( mimeType == 0 ) mimeType = "";
		if ( filePath == 0 ) filePath = "";

		XMP_Assert( thiz->docXMP != NULL );
		XMP_AutoLock metaLock ( &thiz->docXMP->lock, kXMP_WriteLock, (thiz->docXMP != 0) );

		thiz->PrepareForSave ( mimeType, filePath, fileModTime, options );

	XMP_EXIT
}

// -------------------------------------------------------------------------------------------------

void
WXMPDocOps_NoteChange_1 ( XMPDocOpsRef    xmpObjRef,
						  XMP_StringPtr * partsPtr,
						  XMP_Index       partsCount,
						  WXMP_Result *   wResult )
{
	XMP_ENTER_ObjWrite ( XMPDocOps, "WXMPDocOps_NoteChange_1" )

		if ( (partsPtr == 0) && (partsCount > 0) ) XMP_Throw ( "Null parts pointer", kXMPErr_BadParam );

		XMP_Assert( thiz->docXMP != NULL );
		XMP_AutoLock metaLock ( &thiz->docXMP->lock, kXMP_WriteLock, (thiz->docXMP != 0) );

		thiz->NoteChange ( partsPtr, partsCount );

	XMP_EXIT
}

// -------------------------------------------------------------------------------------------------

void
WXMPDocOps_HasPartChanged_1 ( XMPDocOpsRef    xmpObjRef,
							  XMP_StringPtr   prevInstanceID,
							  XMP_StringPtr * partsPtr,
							  XMP_Index       partsCount,
							  WXMP_Result *   wResult ) /* const */
{
	XMP_ENTER_ObjRead ( XMPDocOps, "WXMPDocOps_HasPartChanged_1" )

		if ( (partsPtr == 0) && (partsCount > 0) ) XMP_Throw ( "Null parts pointer", kXMPErr_BadParam );
		if ( prevInstanceID == 0 ) prevInstanceID = "";

		XMP_Assert( thiz.docXMP != NULL );
		XMP_AutoLock metaLock ( &thiz.docXMP->lock, kXMP_ReadLock, (thiz.docXMP != 0) );

		XMP_TriState state = thiz.HasPartChanged ( prevInstanceID, partsPtr, partsCount );
		wResult->int32Result = state;

	XMP_EXIT
}

// -------------------------------------------------------------------------------------------------

void
WXMPDocOps_GetPartChangeID_1 ( XMPDocOpsRef    xmpObjRef,
							   XMP_StringPtr * partsPtr,
							   XMP_Index       partsCount,
							   void *          clientID,
							   SetClientStringProc SetClientString,
							   WXMP_Result *   wResult ) /* const */
{
	XMP_ENTER_ObjRead ( XMPDocOps, "WXMPDocOps_GetPartChangeID_1" )

		if ( (partsPtr == 0) && (partsCount > 0) ) XMP_Throw ( "Null parts pointer", kXMPErr_BadParam );

		XMP_Assert( thiz.docXMP != NULL );
		XMP_AutoLock metaLock ( &thiz.docXMP->lock, kXMP_ReadLock, (thiz.docXMP != 0) );

		XMP_StringPtr idStr;
		XMP_StringLen idLen;
		bool found = thiz.GetPartChangeID ( partsPtr, partsCount, &idStr, &idLen );
		if ( found && (clientID != 0) ) (*SetClientString) ( clientID, idStr, idLen );
		wResult->int32Result = found;

	XMP_EXIT
}

// -------------------------------------------------------------------------------------------------

void
WXMPDocOps_EnsureIDsExist_1 ( XMPDocOpsRef   xmpObjRef,
							  XMP_OptionBits options,
							  WXMP_Result *  wResult )
{
	XMP_ENTER_ObjWrite ( XMPDocOps, "WXMPDocOps_EnsureIDsExist_1" )

		bool newIDs = thiz->EnsureIDsExist ( options );
		wResult->int32Result = newIDs;

	XMP_EXIT
}

// -------------------------------------------------------------------------------------------------

void
WXMPDocOps_IsDirty_1 ( XMPDocOpsRef     xmpObjRef,
					   XMP_OptionBits * reasons,
					   WXMP_Result *    wResult ) /* const */
{
	XMP_ENTER_ObjRead ( XMPDocOps, "WXMPDocOps_IsDirty_1" )

		bool isDirty = thiz.IsDirty ( reasons );
		wResult->int32Result = isDirty;

	XMP_EXIT
}

// -------------------------------------------------------------------------------------------------

void
WXMPDocOps_Clone_1 ( XMPDocOpsRef   xmpObjRef,
					 XMP_OptionBits options,
					 WXMP_Result *  wResult ) /* const */
{
	XMP_ENTER_ObjRead ( XMPDocOps, "WXMPDocOps_Clone_1" )

		XMP_Assert( thiz.docXMP != NULL );
		XMP_AutoLock metaLock ( &thiz.docXMP->lock, kXMP_ReadLock, (thiz.docXMP != 0) );
		bool isCloned = false;
		XMPDocOps * xClone = 0;
#if AdobePrivate
	#if ENABLE_CPP_DOM_MODEL
		if (sUseNewCoreAPIs) {
			isCloned = true;
			xClone = new XMPDocOps2;
		}
	#endif
#endif
		if (!isCloned) {
			xClone = new XMPDocOps;
		}
		thiz.Clone ( xClone, options );
		XMP_Assert ( xClone->clientRefs == 0 );	// ! Gets incremented in TXMPDocOps::Clone.
		wResult->ptrResult = xClone;

	XMP_EXIT
}

// -------------------------------------------------------------------------------------------------

void
WXMPDocOps_GetMetaRef_1 ( XMPDocOpsRef  xmpObjRef,
						  WXMP_Result * wResult ) /* const */
{
	XMP_ENTER_ObjRead ( XMPDocOps, "WXMPDocOps_GetMetaRef_1" )

		XMPMetaRef metaRef = thiz.GetMetaRef();
		wResult->ptrResult = metaRef;

	XMP_EXIT
}

// =================================================================================================

#if __cplusplus
} /* extern "C" */
#endif
