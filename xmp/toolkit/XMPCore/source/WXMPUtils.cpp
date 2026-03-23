// =================================================================================================
// Copyright 2004 Adobe
// All Rights Reserved.
//
// NOTICE:	Adobe permits you to use, modify, and distribute this file in accordance with the terms
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
//	AWL Alan Lillich
//   AJ Abhishek Jindal
//
// mm-dd-yy who Description of changes, most recent on top.
//
// 03-16-15 AJ  5.6-c048 Fixing build break due to XMPCore.def
// 03-16-15 AJ  5.6-c047 Completing Rework of Get/SetBulkMarkers()
// 12-03-15 AJ  5.6-c045 Reworking glue code for Get/SetBulkMarkers()
// 03-02-15 AJ  5.6-c039 Marker Extensibility - Backward compatibility for extensions in GetBulkMarkers() and SetBulkMarkers()
// 02-06-15 AJ  5.6-c037 Fixing warnings due to implicit typecasting
// 07-21-09 AWL 5.0-c047 Add XMPUtils::ApplyTemplate.
// 06-23-09 AWL 5.0-c040-f049 Add hack to revert to global DLL locking.
// 06-11-09 AWL 5.0-c034 Finish threading revamp, implement friendly reader/writer locking.
// 05-21-09 AWL 5.0-c032 Revamp glue again to pass SetClientString with each function.
// 05-19-09 AWL 5.0-c031 First part of threading improvements, revamp the client glue.
// 05-12-09 AWL 5.0-c029 Finish deprecated function removal.
// 01-23-09 AWL 5.0-c005 [2261961] Fix Get/SetBulkMarkers to tolerate varying size of cuePointParams vector.
//
// 10-17-08 AWL 4.4-c010 [1908673] Restore the defunct XMPUtils version of DocOps routines for
//				Mac and Win, not UNIX. Illustrator CS4 is flakey without them, PDFL testing needs this.
// 10-13-08 AWL 4.4-c009 Add support for zone-less times. Get rid of old XMPUtils forms of DocOps.
//
// 05-05-08 AWL 4.2.2-c055 [1768777] Add private callbacks for CuePointParams storage in Get/SetBulkMarkers.
// 04-29-08 AWL 4.2.2-c052 [1768777,1768782] Add private performance enhancements for temporal metadata.
//
// 05-16-06 AWL 4.0-c006 Add SXMPUtils::PackageForJPEG and SXMPUtils::MergeFromJPEG.
// 03-24-06 AWL 4.0-c001 Adapt for move to ham-perforce, integrate XMPFiles, bump version to 4.
//
// 02-21-06 AWL 3.3-015 Expose some of the "File Info" utils. The Adobe Labs build of XMPFiles needs
//				SXMPUtils::RemoveProperties and SXMPUtils::AppendProperties.
// 07-15-05 AWL 3.3-001 [1214033] Bump version to 3.3, the SDK is out. Put back missing multi-file utils.
//
// 05-16-05 AWL 3.2-100 Complete the deBIBification, integrate the internal and SDK source.
// 04-11-05 AWL 3.2-016 Add AdobePrivate conditionals where appropriate.
// 01-28-05 AWL 3.2-001 Remove BIB.
//
// 11-04-04 AWL 3.1.1-090 [1014853] Add XMPUtils::RemoveMultiValueInfo.
// 10-06-04 AWL 3.1.1-083 [1061778] Fix GetMergedListPath to properly release thread lock.
//				Add lock tracing under TraceXMPLocking.
//
// 08-18-04 AWL 3.1-075 Add HasContainedDoc.
// 07-15-04 AWL 3.1-065 [0658037] Fix wrappers to catch empty schema, property, field, etc. names.
// 07-13-04 AWL 3.1-056 [1013358] Fix SetAppName wrapper to change null pointer to "".
// 06-24-04 AWL 3.1-050 Keep the lock when exiting GetMergedListPath.
//
// 04-23-04 AWL [1014270] Fix ConvertToDate and internal callers to handle date properties with
//				empty values, don't assert in ConvertToDate.
// 03-29-04 AWL Add Q&D versions of GetMainPacket and UpdateMainPacket.
// 03-17-04 AWL Cleanup error exceptions, make sure all have a reasonable message.
// 02-17-04 AWL Add multi-file utilities. Rename CreateXyzDocument to InitializeXyzDocument.
// 02-09-04 AWL Start adding the new Document Operation utilities.
// 01-17-04 AWL Move into new Perforce depot, cosmetic cleanup.
//
// =================================================================================================
#endif	// AdobePrivate

// *** Should change "type * inParam" to "type & inParam"

#include "public/include/XMP_Environment.h"	// ! This must be the first include!
#include "public/include/XMP_Const.h"
#include "public/include/client-glue/WXMPUtils.hpp"

#include "XMPCore/source/XMPCore_Impl.hpp"
#include "XMPCore/source/XMPUtils.hpp"

#if XMP_WinBuild
	#pragma warning ( disable : 4101 ) // unreferenced local variable
	#pragma warning ( disable : 4189 ) // local variable is initialized but not referenced
	#pragma warning ( disable : 4800 )	// forcing value to bool 'true' or 'false' (performance warning)
	#if XMP_DebugBuild
		#pragma warning ( disable : 4297 ) // function assumed not to throw an exception but does
	#endif
#endif

#if __cplusplus
extern "C" {
#endif

// =================================================================================================
// Class Static Wrappers
// =====================

void
WXMPUtils_ComposeArrayItemPath_1 ( XMP_StringPtr   schemaNS,
								   XMP_StringPtr   arrayName,
								   XMP_Index	   itemIndex,
								   void *          itemPath,
								   SetClientStringProc SetClientString,
								   WXMP_Result *   wResult )
{
	XMP_ENTER_Static ( "WXMPUtils_ComposeArrayItemPath_1" )

		if ( (schemaNS == 0) || (*schemaNS == 0) ) XMP_Throw ( "Empty schema namespace URI", kXMPErr_BadSchema );
		if ( (arrayName == 0) || (*arrayName == 0) ) XMP_Throw ( "Empty array name", kXMPErr_BadXPath );

		XMP_VarString localStr;

		XMPUtils::ComposeArrayItemPath ( schemaNS, arrayName, itemIndex, &localStr );
		if ( itemPath != 0 ) (*SetClientString) ( itemPath, localStr.c_str(), static_cast< XMP_StringLen >( localStr.size() ) );

	XMP_EXIT
}

// -------------------------------------------------------------------------------------------------

void
WXMPUtils_ComposeStructFieldPath_1 ( XMP_StringPtr	 schemaNS,
									 XMP_StringPtr	 structName,
									 XMP_StringPtr	 fieldNS,
									 XMP_StringPtr	 fieldName,
									 void *          fieldPath,
									 SetClientStringProc SetClientString,
									 WXMP_Result *	 wResult )
{
	XMP_ENTER_Static ( "WXMPUtils_ComposeStructFieldPath_1" )

		if ( (schemaNS == 0) || (*schemaNS == 0) ) XMP_Throw ( "Empty schema namespace URI", kXMPErr_BadSchema );
		if ( (structName == 0) || (*structName == 0) ) XMP_Throw ( "Empty struct name", kXMPErr_BadXPath );
		if ( (fieldNS == 0) || (*fieldNS == 0) ) XMP_Throw ( "Empty field namespace URI", kXMPErr_BadSchema );
		if ( (fieldName == 0) || (*fieldName == 0) ) XMP_Throw ( "Empty field name", kXMPErr_BadXPath );

		XMP_VarString localStr;

		XMPUtils::ComposeStructFieldPath ( schemaNS, structName, fieldNS, fieldName, &localStr );
		if ( fieldPath != 0 ) (*SetClientString) ( fieldPath, localStr.c_str(), static_cast< XMP_StringLen >( localStr.size() ));

	XMP_EXIT
}

// -------------------------------------------------------------------------------------------------

void
WXMPUtils_ComposeQualifierPath_1 ( XMP_StringPtr   schemaNS,
								   XMP_StringPtr   propName,
								   XMP_StringPtr   qualNS,
								   XMP_StringPtr   qualName,
								   void *          qualPath,
								   SetClientStringProc SetClientString,
								   WXMP_Result *   wResult )
{
	XMP_ENTER_Static ( "WXMPUtils_ComposeQualifierPath_1" )

		if ( (schemaNS == 0) || (*schemaNS == 0) ) XMP_Throw ( "Empty schema namespace URI", kXMPErr_BadSchema );
		if ( (propName == 0) || (*propName == 0) ) XMP_Throw ( "Empty property name", kXMPErr_BadXPath );
		if ( (qualNS == 0) || (*qualNS == 0) ) XMP_Throw ( "Empty qualifier namespace URI", kXMPErr_BadSchema );
		if ( (qualName == 0) || (*qualName == 0) ) XMP_Throw ( "Empty qualifier name", kXMPErr_BadXPath );

		XMP_VarString localStr;

		XMPUtils::ComposeQualifierPath ( schemaNS, propName, qualNS, qualName, &localStr );
		if ( qualPath != 0 ) (*SetClientString) ( qualPath, localStr.c_str(), static_cast< XMP_StringLen >( localStr.size() ) );

	XMP_EXIT
}

// -------------------------------------------------------------------------------------------------

void
WXMPUtils_ComposeLangSelector_1 ( XMP_StringPtr	  schemaNS,
								  XMP_StringPtr	  arrayName,
								  XMP_StringPtr	  langName,
								  void *          selPath,
								  SetClientStringProc SetClientString,
								  WXMP_Result *	  wResult )
{
	XMP_ENTER_Static ( "WXMPUtils_ComposeLangSelector_1" )

		if ( (schemaNS == 0) || (*schemaNS == 0) ) XMP_Throw ( "Empty schema namespace URI", kXMPErr_BadSchema );
		if ( (arrayName == 0) || (*arrayName == 0) ) XMP_Throw ( "Empty array name", kXMPErr_BadXPath );
		if ( (langName == 0) || (*langName == 0) ) XMP_Throw ( "Empty language name", kXMPErr_BadParam );

		XMP_VarString localStr;

		XMPUtils::ComposeLangSelector ( schemaNS, arrayName, langName, &localStr );
		if ( selPath != 0 ) (*SetClientString) ( selPath, localStr.c_str(), static_cast< XMP_StringLen >( localStr.size() ) );

	XMP_EXIT
}

// -------------------------------------------------------------------------------------------------

void
WXMPUtils_ComposeFieldSelector_1 ( XMP_StringPtr   schemaNS,
								   XMP_StringPtr   arrayName,
								   XMP_StringPtr   fieldNS,
								   XMP_StringPtr   fieldName,
								   XMP_StringPtr   fieldValue,
								   void *          selPath,
								   SetClientStringProc SetClientString,
								   WXMP_Result *   wResult )
{
	XMP_ENTER_Static ( "WXMPUtils_ComposeFieldSelector_1" )

		if ( (schemaNS == 0) || (*schemaNS == 0) ) XMP_Throw ( "Empty schema namespace URI", kXMPErr_BadSchema );
		if ( (arrayName == 0) || (*arrayName == 0) ) XMP_Throw ( "Empty array name", kXMPErr_BadXPath );
		if ( (fieldNS == 0) || (*fieldNS == 0) ) XMP_Throw ( "Empty field namespace URI", kXMPErr_BadSchema );
		if ( (fieldName == 0) || (*fieldName == 0) ) XMP_Throw ( "Empty field name", kXMPErr_BadXPath );
		if ( fieldValue == 0 ) fieldValue = "";

		XMP_VarString localStr;

		XMPUtils::ComposeFieldSelector ( schemaNS, arrayName, fieldNS, fieldName, fieldValue, &localStr );
		if ( selPath != 0 ) (*SetClientString) ( selPath, localStr.c_str(), static_cast< XMP_StringLen >( localStr.size() ));

	XMP_EXIT
}

// =================================================================================================

void
WXMPUtils_ConvertFromBool_1 ( XMP_Bool		binValue,
							  void *        strValue,
							  SetClientStringProc SetClientString,
							  WXMP_Result *	wResult )
{
	XMP_ENTER_Static ( "WXMPUtils_ConvertFromBool_1" )

		XMP_VarString localStr;

		XMPUtils::ConvertFromBool ( binValue, &localStr );
		if ( strValue != 0 ) (*SetClientString) ( strValue, localStr.c_str(), static_cast< XMP_StringLen >( localStr.size() ) );

	XMP_EXIT
}

// -------------------------------------------------------------------------------------------------

void
WXMPUtils_ConvertFromInt_1 ( XMP_Int32	   binValue,
							 XMP_StringPtr format,
							 void *        strValue,
							 SetClientStringProc SetClientString,
							 WXMP_Result * wResult )
{
	XMP_ENTER_Static ( "WXMPUtils_ConvertFromInt_1" )

		if ( format == 0 ) format = "";

		XMP_VarString localStr;

		XMPUtils::ConvertFromInt ( binValue, format, &localStr );
		if ( strValue != 0 ) (*SetClientString) ( strValue, localStr.c_str(), static_cast< XMP_StringLen >( localStr.size() ) );

	XMP_EXIT
}

// -------------------------------------------------------------------------------------------------

void
WXMPUtils_ConvertFromInt64_1 ( XMP_Int64	 binValue,
							   XMP_StringPtr format,
							   void *        strValue,
							   SetClientStringProc SetClientString,
							   WXMP_Result * wResult )
{
	XMP_ENTER_Static ( "WXMPUtils_ConvertFromInt64_1" )

		if ( format == 0 ) format = "";

		XMP_VarString localStr;

		XMPUtils::ConvertFromInt64 ( binValue, format, &localStr );
		if ( strValue != 0 ) (*SetClientString) ( strValue, localStr.c_str(), static_cast< XMP_StringLen >( localStr.size() ) );

	XMP_EXIT
}

// -------------------------------------------------------------------------------------------------

void
WXMPUtils_ConvertFromFloat_1 ( double		 binValue,
							   XMP_StringPtr format,
							   void *        strValue,
							   SetClientStringProc SetClientString,
							   WXMP_Result * wResult )
{
	XMP_ENTER_Static ( "WXMPUtils_ConvertFromFloat_1" )

		if ( format == 0 ) format = "";

		XMP_VarString localStr;

		XMPUtils::ConvertFromFloat ( binValue, format, &localStr );
		if ( strValue != 0 ) (*SetClientString) ( strValue, localStr.c_str(), static_cast< XMP_StringLen >( localStr.size() ) );

	XMP_EXIT
}

// -------------------------------------------------------------------------------------------------

void
WXMPUtils_ConvertFromDate_1 ( const XMP_DateTime & binValue,
							  void *               strValue,
							  SetClientStringProc SetClientString,
							  WXMP_Result *		   wResult )
{
	XMP_ENTER_Static ( "WXMPUtils_ConvertFromDate_1" )

		XMP_VarString localStr;

		XMPUtils::ConvertFromDate( binValue, &localStr );
		if ( strValue != 0 ) (*SetClientString) ( strValue, localStr.c_str(), static_cast< XMP_StringLen >( localStr.size() ) );

	XMP_EXIT
}

// =================================================================================================

void
WXMPUtils_ConvertToBool_1 ( XMP_StringPtr strValue,
							WXMP_Result * wResult )
{
	XMP_ENTER_Static ( "WXMPUtils_ConvertToBool_1" )

		if ( (strValue == 0) || (*strValue == 0) ) XMP_Throw ( "Empty string value", kXMPErr_BadParam);
		XMP_Bool result = XMPUtils::ConvertToBool ( strValue );
		wResult->int32Result = result;

	XMP_EXIT
}

// -------------------------------------------------------------------------------------------------

void
WXMPUtils_ConvertToInt_1 ( XMP_StringPtr strValue,
						   WXMP_Result * wResult )
{
	XMP_ENTER_Static ( "WXMPUtils_ConvertToInt_1" )

		if ( (strValue == 0) || (*strValue == 0) ) XMP_Throw ( "Empty string value", kXMPErr_BadParam);
		XMP_Int32 result = XMPUtils::ConvertToInt ( strValue );
		wResult->int32Result = result;

	XMP_EXIT
}

// -------------------------------------------------------------------------------------------------

void
WXMPUtils_ConvertToInt64_1 ( XMP_StringPtr strValue,
						     WXMP_Result * wResult )
{
	XMP_ENTER_Static ( "WXMPUtils_ConvertToInt64_1" )

		if ( (strValue == 0) || (*strValue == 0) ) XMP_Throw ( "Empty string value", kXMPErr_BadParam);
		XMP_Int64 result = XMPUtils::ConvertToInt64 ( strValue );
		wResult->int64Result = result;

	XMP_EXIT
}

// -------------------------------------------------------------------------------------------------

void
WXMPUtils_ConvertToFloat_1 ( XMP_StringPtr strValue,
							 WXMP_Result * wResult )
{
	XMP_ENTER_Static ( "WXMPUtils_ConvertToFloat_1")

		if ( (strValue == 0) || (*strValue == 0) ) XMP_Throw ( "Empty string value", kXMPErr_BadParam);
		double result = XMPUtils::ConvertToFloat ( strValue );
		wResult->floatResult = result;

	XMP_EXIT
}

// -------------------------------------------------------------------------------------------------

void
WXMPUtils_ConvertToDate_1 ( XMP_StringPtr  strValue,
							XMP_DateTime * binValue,
							WXMP_Result *  wResult )
{
	XMP_ENTER_Static ( "WXMPUtils_ConvertToDate_1" )

		if ( binValue == 0 ) XMP_Throw ( "Null output date", kXMPErr_BadParam); // ! Pointer is from the client.
		XMPUtils::ConvertToDate ( strValue, binValue );

	XMP_EXIT
}

// =================================================================================================

void
WXMPUtils_CurrentDateTime_1 ( XMP_DateTime * time,
							  WXMP_Result *	 wResult )
{
	XMP_ENTER_Static ( "WXMPUtils_CurrentDateTime_1" )

		if ( time == 0 ) XMP_Throw ( "Null output date", kXMPErr_BadParam);
		XMPUtils::CurrentDateTime ( time );

	XMP_EXIT
}

// -------------------------------------------------------------------------------------------------

void
WXMPUtils_SetTimeZone_1 ( XMP_DateTime * time,
						  WXMP_Result *	 wResult )
{
	XMP_ENTER_Static ( "WXMPUtils_SetTimeZone_1" )

		if ( time == 0 ) XMP_Throw ( "Null output date", kXMPErr_BadParam);
		XMPUtils::SetTimeZone ( time );

	XMP_EXIT
}

// -------------------------------------------------------------------------------------------------

void
WXMPUtils_ConvertToUTCTime_1 ( XMP_DateTime * time,
							   WXMP_Result *  wResult )
{
	XMP_ENTER_Static ( "WXMPUtils_ConvertToUTCTime_1" )

		if ( time == 0 ) XMP_Throw ( "Null output date", kXMPErr_BadParam);
		XMPUtils::ConvertToUTCTime ( time );

	XMP_EXIT
}

// -------------------------------------------------------------------------------------------------

void
WXMPUtils_ConvertToLocalTime_1 ( XMP_DateTime * time,
								 WXMP_Result *	wResult )
{
	XMP_ENTER_Static ( "WXMPUtils_ConvertToLocalTime_1" )

		if ( time == 0 ) XMP_Throw ( "Null output date", kXMPErr_BadParam);
		XMPUtils::ConvertToLocalTime ( time );

	XMP_EXIT
}

// -------------------------------------------------------------------------------------------------

void
WXMPUtils_CompareDateTime_1 ( const XMP_DateTime & left,
							  const XMP_DateTime & right,
							  WXMP_Result *		   wResult )
{
	XMP_ENTER_Static ( "WXMPUtils_CompareDateTime_1" )

		int result = XMPUtils::CompareDateTime ( left, right );
		wResult->int32Result = result;

	XMP_EXIT
}

// =================================================================================================

void
WXMPUtils_EncodeToBase64_1 ( XMP_StringPtr rawStr,
							 XMP_StringLen rawLen,
							 void *        encodedStr,
							 SetClientStringProc SetClientString,
							 WXMP_Result * wResult )
{
	XMP_ENTER_Static ( "WXMPUtils_EncodeToBase64_1" )

		XMP_VarString localStr;

		XMPUtils::EncodeToBase64 ( rawStr, rawLen, &localStr );
		if ( encodedStr != 0 ) (*SetClientString) ( encodedStr, localStr.c_str(), static_cast< XMP_StringLen >( localStr.size()) );

	XMP_EXIT
}

// -------------------------------------------------------------------------------------------------

void
WXMPUtils_DecodeFromBase64_1 ( XMP_StringPtr encodedStr,
							   XMP_StringLen encodedLen,
							   void *        rawStr,
							   SetClientStringProc SetClientString,
							   WXMP_Result * wResult )
{
	XMP_ENTER_Static ( "WXMPUtils_DecodeFromBase64_1" )

		XMP_VarString localStr;

		XMPUtils::DecodeFromBase64 ( encodedStr, encodedLen, &localStr );
		if ( rawStr != 0 ) (*SetClientString) ( rawStr, localStr.c_str(), static_cast< XMP_StringLen >( localStr.size()) );

	XMP_EXIT
}

// =================================================================================================

void
WXMPUtils_PackageForJPEG_1 ( XMPMetaRef    wxmpObj,
                             void *        stdStr,
                             void *        extStr,
                             void *        digestStr,
                             SetClientStringProc SetClientString,
                             WXMP_Result * wResult )
{
	XMP_ENTER_Static ( "WXMPUtils_PackageForJPEG_1" )

		XMP_VarString localStdStr;
		XMP_VarString localExtStr;
		XMP_VarString localDigestStr;

		const XMPMeta & xmpObj = WtoXMPMeta_Ref ( wxmpObj );
		XMP_AutoLock metaLock ( &xmpObj.lock, kXMP_ReadLock );

		XMPUtils::PackageForJPEG ( xmpObj, &localStdStr, &localExtStr, &localDigestStr );
		if ( stdStr != 0 ) (*SetClientString) ( stdStr, localStdStr.c_str(), static_cast< XMP_StringLen >( localStdStr.size()) );
		if ( extStr != 0 ) (*SetClientString) ( extStr, localExtStr.c_str(), static_cast< XMP_StringLen >( localExtStr.size()) );
		if ( digestStr != 0 ) (*SetClientString) ( digestStr, localDigestStr.c_str(), static_cast< XMP_StringLen >( localDigestStr.size()) );

	XMP_EXIT
}

// -------------------------------------------------------------------------------------------------

void
WXMPUtils_MergeFromJPEG_1 ( XMPMetaRef    wfullXMP,
                            XMPMetaRef    wextendedXMP,
                            WXMP_Result * wResult )
{
	XMP_ENTER_Static ( "WXMPUtils_MergeFromJPEG_1" )

		if ( wfullXMP == 0 ) XMP_Throw ( "Output XMP pointer is null", kXMPErr_BadParam );
		if ( wfullXMP == wextendedXMP ) XMP_Throw ( "Full and extended XMP pointers match", kXMPErr_BadParam );

		XMPMeta * fullXMP = WtoXMPMeta_Ptr ( wfullXMP );
		XMP_AutoLock fullXMPLock ( &fullXMP->lock, kXMP_WriteLock );

		const XMPMeta & extendedXMP = WtoXMPMeta_Ref ( wextendedXMP );
		XMP_AutoLock extendedXMPLock ( &extendedXMP.lock, kXMP_ReadLock );

		XMPUtils::MergeFromJPEG ( fullXMP, extendedXMP );

	XMP_EXIT
}

// =================================================================================================

void
WXMPUtils_CatenateArrayItems_1 ( XMPMetaRef 	wxmpObj,
								 XMP_StringPtr	schemaNS,
								 XMP_StringPtr	arrayName,
								 XMP_StringPtr	separator,
								 XMP_StringPtr	quotes,
								 XMP_OptionBits	options,
								 void *         catedStr,
								 SetClientStringProc SetClientString,
								 WXMP_Result *	wResult )
{
	XMP_ENTER_Static ( "WXMPUtils_CatenateArrayItems_1" )

		if ( (schemaNS == 0) || (*schemaNS == 0) ) XMP_Throw ( "Empty schema namespace URI", kXMPErr_BadSchema );
		if ( (arrayName == 0) || (*arrayName == 0) ) XMP_Throw ( "Empty array name", kXMPErr_BadXPath );

		if ( separator == 0 ) separator = "; ";
		if ( quotes == 0 ) quotes = "\"";

		XMP_VarString localStr;

		const XMPMeta & xmpObj = WtoXMPMeta_Ref ( wxmpObj );
		XMP_AutoLock metaLock ( &xmpObj.lock, kXMP_ReadLock );

		XMPUtils::CatenateArrayItems ( xmpObj, schemaNS, arrayName, separator, quotes, options, &localStr );
		if ( catedStr != 0 ) (*SetClientString) ( catedStr, localStr.c_str(), static_cast< XMP_StringLen >( localStr.size() ));

	XMP_EXIT
}

// -------------------------------------------------------------------------------------------------

void
WXMPUtils_SeparateArrayItems_1 ( XMPMetaRef 	wxmpObj,
								 XMP_StringPtr	schemaNS,
								 XMP_StringPtr	arrayName,
								 XMP_OptionBits options,
								 XMP_StringPtr	catedStr,
								 WXMP_Result *	wResult )
{
	XMP_ENTER_Static ( "WXMPUtils_SeparateArrayItems_1" )

		if ( wxmpObj == 0 ) XMP_Throw ( "Output XMP pointer is null", kXMPErr_BadParam );
		if ( (schemaNS == 0) || (*schemaNS == 0) ) XMP_Throw ( "Empty schema namespace URI", kXMPErr_BadSchema );
		if ( (arrayName == 0) || (*arrayName == 0) ) XMP_Throw ( "Empty array name", kXMPErr_BadXPath );
		if ( catedStr == 0 ) catedStr = "";

		XMPMeta * xmpObj = WtoXMPMeta_Ptr ( wxmpObj );
		XMP_AutoLock metaLock ( &xmpObj->lock, kXMP_WriteLock );

		XMPUtils::SeparateArrayItems ( xmpObj, schemaNS, arrayName, options, catedStr );

	XMP_EXIT
}

// -------------------------------------------------------------------------------------------------

void
WXMPUtils_ApplyTemplate_1 ( XMPMetaRef     wWorkingXMP,
							XMPMetaRef 	   wTemplateXMP,
							XMP_OptionBits actions,
							WXMP_Result *  wResult )
{
	XMP_ENTER_Static ( "WXMPUtils_ApplyTemplate_1" )

		XMP_Assert ( (wWorkingXMP != 0) && (wTemplateXMP != 0) );	// Client glue enforced.

		XMPMeta * workingXMP = WtoXMPMeta_Ptr ( wWorkingXMP );
		XMP_AutoLock workingLock ( &workingXMP->lock, kXMP_WriteLock );

		const XMPMeta & templateXMP = WtoXMPMeta_Ref ( wTemplateXMP );
		XMP_AutoLock templateLock ( &templateXMP.lock, kXMP_ReadLock );

		XMPUtils::ApplyTemplate ( workingXMP, templateXMP, actions );

	XMP_EXIT
}

// -------------------------------------------------------------------------------------------------

void
WXMPUtils_RemoveProperties_1 ( XMPMetaRef 	  wxmpObj,
							   XMP_StringPtr  schemaNS,
							   XMP_StringPtr  propName,
							   XMP_OptionBits options,
							   WXMP_Result *  wResult )
{
	XMP_ENTER_Static ( "WXMPUtils_RemoveProperties_1" )

		if ( wxmpObj == 0 ) XMP_Throw ( "Output XMP pointer is null", kXMPErr_BadParam );
		if ( schemaNS == 0 ) schemaNS = "";
		if ( propName == 0 ) propName = "";

		XMPMeta * xmpObj = WtoXMPMeta_Ptr ( wxmpObj );
		XMP_AutoLock metaLock ( &xmpObj->lock, kXMP_WriteLock );

		XMPUtils::RemoveProperties ( xmpObj, schemaNS, propName, options );

	XMP_EXIT
}

// -------------------------------------------------------------------------------------------------

#if AdobePrivate
void
WXMPUtils_AppendProperties_1 ( XMPMetaRef     wSource,
							   XMPMetaRef 	  wDest,
							   XMP_OptionBits options,
							   WXMP_Result *  wResult )
{
	XMP_ENTER_Static ( "WXMPUtils_AppendProperties_1" )

		if ( wDest == 0 ) XMP_Throw ( "Output XMP pointer is null", kXMPErr_BadParam );
		if ( wDest == wSource ) XMP_Throw ( "Source and destination XMP must differ", kXMPErr_BadParam );

		const XMPMeta & source = WtoXMPMeta_Ref ( wSource );
		XMP_AutoLock sourceLock ( &source.lock, kXMP_ReadLock );

		XMPMeta * dest = WtoXMPMeta_Ptr ( wDest );
		XMP_AutoLock destLock ( &dest->lock, kXMP_WriteLock );

		XMPUtils::AppendProperties ( source, dest, options );

	XMP_EXIT
}
#endif

// -------------------------------------------------------------------------------------------------

void
WXMPUtils_DuplicateSubtree_1 ( XMPMetaRef     wSource,
							   XMPMetaRef 	  wDest,
							   XMP_StringPtr  sourceNS,
							   XMP_StringPtr  sourceRoot,
							   XMP_StringPtr  destNS,
							   XMP_StringPtr  destRoot,
							   XMP_OptionBits options,
							   WXMP_Result *  wResult )
{
	XMP_ENTER_Static ( "WXMPUtils_DuplicateSubtree_1" )

		if ( wDest == 0 ) XMP_Throw ( "Output XMP pointer is null", kXMPErr_BadParam );
		if ( (sourceNS == 0) || (*sourceNS == 0) ) XMP_Throw ( "Empty source schema URI", kXMPErr_BadSchema );
		if ( (sourceRoot == 0) || (*sourceRoot == 0) ) XMP_Throw ( "Empty source root name", kXMPErr_BadXPath );
		if ( destNS == 0 ) destNS = sourceNS;
		if ( destRoot == 0 ) destRoot = sourceRoot;

		const XMPMeta & source = WtoXMPMeta_Ref ( wSource );
		XMP_AutoLock sourceLock ( &source.lock, kXMP_ReadLock, (wSource != wDest) );

		XMPMeta * dest = WtoXMPMeta_Ptr ( wDest );
		XMP_AutoLock destLock ( &dest->lock, kXMP_WriteLock );

		XMPUtils::DuplicateSubtree ( source, dest, sourceNS, sourceRoot, destNS, destRoot, options );

	XMP_EXIT
}

#if AdobePrivate

// -------------------------------------------------------------------------------------------------

void
WXMPUtils_CollectMultiFileXMP_1 ( XMPMetaRef     wInputXMP,
							      XMPMetaRef 	 wMultiXMP,
							      XMP_OptionBits options,
							      WXMP_Result *  wResult )
{
	XMP_ENTER_Static ( "WXMPUtils_CollectMultiFileXMP_1" )

		if ( wMultiXMP == 0 ) XMP_Throw ( "Output XMP pointer is null", kXMPErr_BadParam );
		if ( wInputXMP == wMultiXMP ) XMP_Throw ( "Input and Multi XMP must differ", kXMPErr_BadParam );

		const XMPMeta & inputXMP = WtoXMPMeta_Ref ( wInputXMP );
		XMP_AutoLock inputLock ( &inputXMP.lock, kXMP_ReadLock );

		XMPMeta * multiXMP = WtoXMPMeta_Ptr ( wMultiXMP );
		XMP_AutoLock multiLock ( &multiXMP->lock, kXMP_WriteLock );

		XMPUtils::CollectMultiFileXMP ( inputXMP, multiXMP, options );

	XMP_EXIT
}

// -------------------------------------------------------------------------------------------------

void
WXMPUtils_DistributeMultiFileXMP_1 ( XMPMetaRef     wMultiXMP,
							         XMPMetaRef 	wOutputXMP,
							         XMP_OptionBits options,
							         WXMP_Result *  wResult )
{
	XMP_ENTER_Static ( "WXMPUtils_DistributeMultiFileXMP_1" )

		if ( wOutputXMP == 0 ) XMP_Throw ( "Output XMP pointer is null", kXMPErr_BadParam );
		if ( wOutputXMP == wMultiXMP ) XMP_Throw ( "Output and Multi XMP must differ", kXMPErr_BadParam );

		const XMPMeta & multiXMP = WtoXMPMeta_Ref ( wMultiXMP );
		XMP_AutoLock multiLock ( &multiXMP.lock, kXMP_ReadLock );

		XMPMeta * outputXMP = WtoXMPMeta_Ptr ( wOutputXMP );
		XMP_AutoLock outputLock ( &outputXMP->lock, kXMP_WriteLock );

		XMPUtils::DistributeMultiFileXMP ( multiXMP, outputXMP, options );

	XMP_EXIT
}

// -------------------------------------------------------------------------------------------------

void
WXMPUtils_IsPropertyMultiValued_1 ( XMPMetaRef    wMultiXMP,
							        XMP_StringPtr propNS,
	                                XMP_StringPtr propName,
							        WXMP_Result * wResult )
{
	XMP_ENTER_Static ( "WXMPUtils_IsPropertyMultiValued_1" )

    	if ( (propNS == 0) || (*propNS == 0) ) XMP_Throw ( "Empty schema namespace URI", kXMPErr_BadSchema );
    	if ( (propName == 0) || (*propName == 0) ) XMP_Throw ( "Empty property name", kXMPErr_BadXPath );

		const XMPMeta & multiXMP = WtoXMPMeta_Ref ( wMultiXMP );
		XMP_AutoLock multiLock ( &multiXMP.lock, kXMP_ReadLock );

		bool result = XMPUtils::IsPropertyMultiValued ( multiXMP, propNS, propName );
		wResult->int32Result = result;

	XMP_EXIT
}

// -------------------------------------------------------------------------------------------------

void
WXMPUtils_GetDateRange_1 ( XMPMetaRef     wMultiXMP,
                           XMP_StringPtr  propNS,
                           XMP_StringPtr  propName,
                           XMP_DateTime * oldest,
                           XMP_DateTime * newest,
                           WXMP_Result *  wResult )
{
	XMP_ENTER_Static ( "WXMPUtils_GetDateRange_1" )

    	if ( (propNS == 0) || (*propNS == 0) ) XMP_Throw ( "Empty schema namespace URI", kXMPErr_BadSchema );
    	if ( (propName == 0) || (*propName == 0) ) XMP_Throw ( "Empty property name", kXMPErr_BadXPath );

		const XMPMeta & multiXMP = WtoXMPMeta_Ref ( wMultiXMP );
		XMP_AutoLock multiLock ( &multiXMP.lock, kXMP_ReadLock );

		bool result = XMPUtils::GetDateRange ( multiXMP, propNS, propName, oldest, newest );
		wResult->int32Result = result;

	XMP_EXIT
}

// -------------------------------------------------------------------------------------------------

void
WXMPUtils_GetMergedListPath_1 ( XMPMetaRef    wMultiXMP,
							    XMP_StringPtr propNS,
	                            XMP_StringPtr propName,
	                            void *        pathStr,
	                            SetClientStringProc SetClientString,
							    WXMP_Result * wResult )
{
	XMP_ENTER_Static ( "WXMPUtils_GetMergedListPath_1" )

    	if ( (propNS == 0) || (*propNS == 0) ) XMP_Throw ( "Empty schema namespace URI", kXMPErr_BadSchema );
    	if ( (propName == 0) || (*propName == 0) ) XMP_Throw ( "Empty property name", kXMPErr_BadXPath );

		XMP_VarString localStr;

		const XMPMeta & multiXMP = WtoXMPMeta_Ref ( wMultiXMP );
		XMP_AutoLock multiLock ( &multiXMP.lock, kXMP_ReadLock );

		bool found = XMPUtils::GetMergedListPath ( multiXMP, propNS, propName, &localStr );
		wResult->int32Result = found;
		if ( pathStr != 0 ) (*SetClientString) ( pathStr, localStr.c_str(), static_cast< XMP_StringLen >( localStr.size() ) );

	XMP_EXIT
}

// -------------------------------------------------------------------------------------------------

void
WXMPUtils_RemoveMultiValueInfo_1 ( XMPMetaRef    wMultiXMP,
							       XMP_StringPtr propNS,
	                               XMP_StringPtr propName,
							       WXMP_Result * wResult )
{
	XMP_ENTER_Static ( "WXMPUtils_RemoveMultiValueInfo_1" )

		if ( wMultiXMP == 0 ) XMP_Throw ( "Output XMP pointer is null", kXMPErr_BadParam );
    	if ( propNS == 0 ) propNS = "";
    	if ( propName == 0 ) propName = "";

		XMPMeta * multiXMP = WtoXMPMeta_Ptr ( wMultiXMP );
		XMP_AutoLock multiLock ( &multiXMP->lock, kXMP_WriteLock );

		XMPUtils::RemoveMultiValueInfo ( multiXMP, propNS, propName );

	XMP_EXIT
}

#endif	// AdobePrivate

#if AdobePrivate

// =================================================================================================

void
WXMPUtils_GetBulkMarkers_1 ( XMPMetaRef    wXMP,
							 XMP_StringPtr ns,
							 XMP_StringPtr path,
							 void *        clientPtr,
							 XMP_Uns32     clientElemSize,
							 XMPDMO_AllocClientMarkersProc AllocClientMarkers,
							 XMPDMO_AllocClientCuePointsProc  AllocClientCuePoints,
							 WXMP_Result * wResult )
{
	XMP_ENTER_Static ( "WXMPUtils_GetBulkMarkers_1" )

		if ( clientPtr == 0 ) XMP_Throw ( "Output param pointer is null", kXMPErr_BadParam );

		const XMPMeta & xmpObj = WtoXMPMeta_Ref ( wXMP );
		XMP_AutoLock metaLock ( &xmpObj.lock, kXMP_ReadLock );

		XMP_Bool found = XMPUtils::GetBulkMarkers ( xmpObj, ns, path, clientPtr, clientElemSize,
													AllocClientMarkers, AllocClientCuePoints );
		wResult->int32Result = found;

	XMP_EXIT
}

// -------------------------------------------------------------------------------------------------

void
WXMPUtils_SetBulkMarkers_1 ( XMPMetaRef    wXMP,
							 XMP_StringPtr ns,
							 XMP_StringPtr path,
							 XMP_Index     count,
							 const void *  clientArray,
							 XMP_Uns32     clientElemSize,
							 XMPDMO_GetCuePointParamsProc GetCuePointParams,
							 WXMP_Result * wResult )
{
	XMP_ENTER_Static ( "WXMPUtils_SetBulkMarkers_1" )

		if ( wXMP == 0 ) XMP_Throw ( "Output XMP pointer is null", kXMPErr_BadParam );

		XMPMeta * xmpObj = WtoXMPMeta_Ptr ( wXMP );
		XMP_AutoLock metaLock ( &xmpObj->lock, kXMP_WriteLock );

		XMPUtils::SetBulkMarkers ( xmpObj, ns, path, count, clientArray, clientElemSize, GetCuePointParams );

	XMP_EXIT
}

// =================================================================================================

#endif	// AdobePrivate

// =================================================================================================

#if __cplusplus
} /* extern "C" */
#endif
