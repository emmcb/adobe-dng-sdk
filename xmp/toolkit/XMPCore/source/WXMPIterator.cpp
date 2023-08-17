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
// 06-23-09 AWL 5.0-c040-f049 Add hack to revert to global DLL locking.
// 06-11-09 AWL 5.0-c034 Finish threading revamp, implement friendly reader/writer locking.
// 05-21-09 AWL 5.0-c032 Revamp glue again to pass SetClientString with each function.
// 05-19-09 AWL 5.0-c031 First part of threading improvements, revamp the client glue.
//
// 03-24-06 AWL 4.0-c001 Adapt for move to ham-perforce, integrate XMPFiles, bump version to 4.
//
// 05-16-05 AWL 3.3-100 Complete the deBIBification, integrate the internal and SDK source. Bump the
//              version to 3.3 and build to 100, well ahead of main's latest 3.3-009.
//
// 02-11-05 AWL 3.2-002 Add client reference counting.
// 01-28-05 AWL 3.2-001 Remove BIB.
//
// 10-06-04 AWL 3.1.1-083 [1061778] Add lock tracing under TraceXMPLocking.
// 01-17-04 AWL Move into new Perforce depot, cosmetic cleanup.
//
// =================================================================================================
#endif // AdobePrivate

#include "public/include/XMP_Environment.h"	// ! This must be the first include!
#include "public/include/XMP_Const.h"

#include "public/include/client-glue/WXMPIterator.hpp"

#include "XMPCore/source/XMPCore_Impl.hpp"
#include "XMPCore/source/XMPIterator.hpp"
#if AdobePrivate
#if ENABLE_CPP_DOM_MODEL

#include "XMPCore/source/XMPIterator2.hpp"
#endif
#endif 

#if XMP_WinBuild
	#pragma warning ( disable : 4101 ) // unreferenced local variable
	#pragma warning ( disable : 4189 ) // local variable is initialized but not referenced
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
WXMPIterator_PropCTor_1 ( XMPMetaRef     xmpRef,
                          XMP_StringPtr  schemaNS,
                          XMP_StringPtr  propName,
                          XMP_OptionBits options,
                          WXMP_Result *  wResult )
{
    XMP_ENTER_Static ( "WXMPIterator_PropCTor_1" )	// No lib object yet, use the static entry.

		if ( schemaNS == 0 ) schemaNS = "";
		if ( propName == 0 ) propName = "";

		const XMPMeta & xmpObj = WtoXMPMeta_Ref ( xmpRef );
		XMP_AutoLock metaLock ( &xmpObj.lock, kXMP_ReadLock );
		
		XMPIterator * iter = NULL;
#if AdobePrivate
#if ENABLE_CPP_DOM_MODEL
		if( sUseNewCoreAPIs ) iter = new XMPIterator2(xmpObj, schemaNS, propName, options);
		else iter = new XMPIterator(xmpObj, schemaNS, propName, options);
#endif
#endif
		if (!iter) {

			iter = new XMPIterator(xmpObj, schemaNS, propName, options);
		}
		++iter->clientRefs;
		XMP_Assert ( iter->clientRefs == 1 );
		wResult->ptrResult = XMPIteratorRef ( iter );

    XMP_EXIT
}

// -------------------------------------------------------------------------------------------------

void
WXMPIterator_TableCTor_1 ( XMP_StringPtr  schemaNS,
                           XMP_StringPtr  propName,
                           XMP_OptionBits options,
                           WXMP_Result *  wResult )
{
    XMP_ENTER_Static ( "WXMPIterator_TableCTor_1" )	// No lib object yet, use the static entry.

		if ( schemaNS == 0 ) schemaNS = "";
		if ( propName == 0 ) propName = "";

		XMPIterator * iter = new XMPIterator ( schemaNS, propName, options );
		++iter->clientRefs;
		XMP_Assert ( iter->clientRefs == 1 );
		wResult->ptrResult = XMPIteratorRef ( iter );

    XMP_EXIT
}

// -------------------------------------------------------------------------------------------------

void
WXMPIterator_IncrementRefCount_1 ( XMPIteratorRef xmpObjRef )
{
	WXMP_Result void_wResult;
	WXMP_Result * wResult = &void_wResult;	// ! Needed to "fool" the EnterWrapper macro.
	XMP_ENTER_ObjWrite ( XMPIterator, "WXMPIterator_IncrementRefCount_1" )

		++thiz->clientRefs;
		XMP_Assert ( thiz->clientRefs > 1 );

	XMP_EXIT_NoThrow
}

// -------------------------------------------------------------------------------------------------

void
WXMPIterator_DecrementRefCount_1 ( XMPIteratorRef xmpObjRef )
{
	WXMP_Result void_wResult;
	WXMP_Result * wResult = &void_wResult;	// ! Needed to "fool" the EnterWrapper macro.
	XMP_ENTER_ObjWrite ( XMPIterator, "WXMPIterator_DecrementRefCount_1" )

		XMP_Assert ( thiz->clientRefs > 0 );
		--thiz->clientRefs;
		if ( thiz->clientRefs <= 0 ) {
			objLock.Release();
			delete ( thiz );
		}

	XMP_EXIT_NoThrow
}

// =================================================================================================
// Class Method Wrappers
// =====================

void
WXMPIterator_Next_1 ( XMPIteratorRef   xmpObjRef,
                      void *           schemaNS,
                      void *           propPath,
                      void *           propValue,
                      XMP_OptionBits * propOptions,
                      SetClientStringProc SetClientString,
                      WXMP_Result *    wResult )
{
    XMP_ENTER_ObjWrite ( XMPIterator, "WXMPIterator_Next_1" )

		XMP_StringPtr schemaPtr = 0;
		XMP_StringLen schemaLen = 0;
		XMP_StringPtr pathPtr = 0;
		XMP_StringLen pathLen = 0;
		XMP_StringPtr valuePtr = 0;
		XMP_StringLen valueLen = 0;
		
		XMP_OptionBits voidOptionBits = 0;
		if ( propOptions == 0 ) propOptions = &voidOptionBits;

		XMP_Assert( thiz->info.xmpObj != NULL );
		XMP_AutoLock metaLock ( &thiz->info.xmpObj->lock, kXMP_ReadLock, (thiz->info.xmpObj != 0) );

		XMP_Bool found = thiz->Next ( &schemaPtr, &schemaLen, &pathPtr, &pathLen, &valuePtr, &valueLen, propOptions );
		wResult->int32Result = found;
		
		if ( found ) {
			if ( schemaNS != 0 ) (*SetClientString) ( schemaNS, schemaPtr, schemaLen );
			if ( propPath != 0 ) (*SetClientString) ( propPath, pathPtr, pathLen );
			if ( propValue != 0 ) (*SetClientString) ( propValue, valuePtr, valueLen );
		}

    XMP_EXIT
}

// -------------------------------------------------------------------------------------------------

void
WXMPIterator_Skip_1 ( XMPIteratorRef xmpObjRef,
                      XMP_OptionBits options,
                      WXMP_Result *  wResult )
{
    XMP_ENTER_ObjWrite ( XMPIterator, "WXMPIterator_Skip_1" )

		XMP_Assert( thiz->info.xmpObj != NULL );
		XMP_AutoLock metaLock ( &thiz->info.xmpObj->lock, kXMP_ReadLock, (thiz->info.xmpObj != 0) );

		thiz->Skip ( options );

    XMP_EXIT
}

// =================================================================================================

#if __cplusplus
} /* extern "C" */
#endif
