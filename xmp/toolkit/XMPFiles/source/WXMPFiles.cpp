// =================================================================================================
// Copyright Adobe
// Copyright 2004 Adobe
// All Rights Reserved
//
// NOTICE: Adobe permits you to use, modify, and distribute this file in accordance with the terms
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
//	PKG Praveen Kumar Goyal
//	ADC Amandeep Chawla
//	IJS Inder Jeet Singh
//   AJ Abhishek Jindal
//
// mm-dd-yy who Description of changes, most recent on top
//
// 02-06-15 AJ  5.6-f131 Fixing warnings due to implicit typecasting
// 05-13-13 ADC 5.6-f060 Removing usage of bool in APIs exposed at DLL boundaries.
// 01-30-13 IJS 5.6-f029 Removed LocateMetadataFiles API from XMPFiles
// 01-11-13 IJS 5.6-f020 Fixed Bugs 3469857 and 3473168.
// 01-07-13 IJS 5.6-f018 Changed name of API IsWritable to IsMetadataWritable.
//                       Added root Path folder as associated Resource for folder based Formats.
// 12-21-12 IJS 5.6-f008 Added GetAssociatedResources and IsWritable APIs 
//
// 09-28-12 AWL 5.5-f039 Add LocateMetadataFiles.
// 09-28-12 ADC 5.5-f038 Implement public API and glue layer for XMPFiles error notifications.
// 06-20-12 AWL 5.5-f018 Add outer layers for XMPFiles progress notifications, no handlers use it yet.
//
// 10-27-11 AWL 5.4-f030 [3007461] Improve performance of GetFileModDate.
// 09-23-11 AWL 5.4-f012 Add GetFileModDate.
// 06-27-11 PKG 5.4-f001 Initial checkin of plugin architecture.
//
// 08-17-10 AWL 5.3-f001 Integrate I/O revamp to main.
//
// 06-23-09 AWL 5.0-c040-f049 Add hack to revert to global DLL locking.
// 06-11-09 AWL 5.0-f043 Finish threading revamp, implement friendly reader/writer locking.
// 05-21-09 AWL 5.0-f040 Revamp glue again to pass SetClientString with each function.
// 05-19-09 AWL 5.0-f039 First part of threading improvements, revamp the client glue.
// 05-12-09 AWL 5.0-f037 Finish deprecated function removal.
//
// 01-28-08 AWL 4.2-f062 Add public CheckFileFormat and CheckPackageFormat functions.
// 01-11-07 AWL 4.2-f007 [1454747] Change QuickTime init/term calls to handle main and background thread issues.
//
// 11-28-06 AWL 4.1-f077 [1378220] Add conditional code to gather top level performance data.
//
// 05-19-06 AWL 4.0-f006 Change XMPFiles::OpenFile to return bool, don't throw if smart handler not
//				found when kXMPFiles_OpenUseSmartHandler is passed.
// 04-07-06 AWL 4.0-f002 Add XMPFiles::GetThumbnail. Change XMPFiles::OpenFile to close the disk file
//				early for read-only access. Change XMPFileHandler::ExtractXMP to CacheFileData.
// 03-24-06 AWL 4.0-f001 Adapt for move to ham-perforce, integrate XMPFiles, bump version to 4.
//
// 07-21-05 AWL 1.3-001 Remove BIB, same as XMP. Create Xcode project. Bump version to 1.3.
//
// 10-19-04 AWL 1.0-012 [1094742] Fix CanPutXMP to call GetFileInfo properly, and to return the
//				result from the wrapper.
// 10-08-04 AWL 1.0-010 Add thread locking.
// 10-07-04 AWL 1.0-008 Add missing SetAbortProc in WXMPFiles.cpp and export lists.
// 08-23-04 AWL Add GetVersion.
// 08-20-04 AWL Add handlerFlags output to GetFileInfo.
// 08-13-04 AWL Checkpoint, almost full implementation, compiles.
// 07-02-04 AWL First draft.
//
// =================================================================================================
#endif // AdobePrivate

#include "public/include/XMP_Environment.h"
#include "public/include/XMP_Const.h"

#include "public/include/client-glue/WXMPFiles.hpp"

#include "source/XMP_ProgressTracker.hpp"

#include "XMPFiles/source/XMPFiles_Impl.hpp"
#include "XMPFiles/source/XMPFiles.hpp"

#if XMP_WinBuild
	#if XMP_DebugBuild
		#pragma warning ( disable : 4297 ) // function assumed not to throw an exception but does
	#endif
#endif

#if __cplusplus
extern "C" {
#endif

// =================================================================================================

static WXMP_Result voidResult;	// Used for functions that  don't use the normal result mechanism.

// =================================================================================================

void WXMPFiles_GetVersionInfo_1 ( XMP_VersionInfo * versionInfo )
{
	WXMP_Result * wResult = &voidResult;	// ! Needed to "fool" the EnterWrapper macro.
	XMP_ENTER_NoLock ( "WXMPFiles_GetVersionInfo_1" )

		XMPFiles::GetVersionInfo ( versionInfo );

	XMP_EXIT_NoThrow
}

// -------------------------------------------------------------------------------------------------

void WXMPFiles_Initialize_1 ( XMP_OptionBits options,
							  WXMP_Result *  wResult )
{
	XMP_ENTER_NoLock ( "WXMPFiles_Initialize_1" )

		wResult->int32Result = XMPFiles::Initialize ( options, NULL, NULL );

	XMP_EXIT
}

// -------------------------------------------------------------------------------------------------

// -------------------------------------------------------------------------------------------------

void WXMPFiles_Initialize_2 ( XMP_OptionBits options,
							  const char *   pluginFolder,
							  const char *   plugins,
							  WXMP_Result *  wResult )
{
	XMP_ENTER_NoLock ( "WXMPFiles_Initialize_1" )

		wResult->int32Result = XMPFiles::Initialize ( options, pluginFolder, plugins );

	XMP_EXIT
}

// -------------------------------------------------------------------------------------------------

void WXMPFiles_Terminate_1()
{
	WXMP_Result * wResult = &voidResult;	// ! Needed to "fool" the EnterWrapper macro.
	XMP_ENTER_NoLock ( "WXMPFiles_Terminate_1" )

		XMPFiles::Terminate();

	XMP_EXIT_NoThrow
}

// =================================================================================================

void WXMPFiles_CTor_1 ( WXMP_Result * wResult )
{
	XMP_ENTER_Static ( "WXMPFiles_CTor_1" )	// No lib object yet, use the static entry.

		XMPFiles * newObj = new XMPFiles();
		++newObj->clientRefs;
		XMP_Assert ( newObj->clientRefs == 1 );
		wResult->ptrResult = newObj;

	XMP_EXIT
}

// -------------------------------------------------------------------------------------------------

void WXMPFiles_IncrementRefCount_1 ( XMPFilesRef xmpObjRef )
{
	WXMP_Result * wResult = &voidResult;	// ! Needed to "fool" the EnterWrapper macro.
	XMP_ENTER_ObjWrite ( XMPFiles, "WXMPFiles_IncrementRefCount_1" )

		++thiz->clientRefs;
		XMP_Assert ( thiz->clientRefs > 0 );

	XMP_EXIT_NoThrow
}

// -------------------------------------------------------------------------------------------------

void WXMPFiles_DecrementRefCount_1 ( XMPFilesRef xmpObjRef )
{
	WXMP_Result * wResult = &voidResult;	// ! Needed to "fool" the EnterWrapper macro.
	XMP_ENTER_ObjWrite ( XMPFiles, "WXMPFiles_DecrementRefCount_1" )

		XMP_Assert ( thiz->clientRefs > 0 );
		--thiz->clientRefs;
		if ( thiz->clientRefs <= 0 ) {
			objLock.Release();
			delete ( thiz );
		}

	XMP_EXIT_NoThrow
}

// -------------------------------------------------------------------------------------------------

void WXMPFiles_GetFormatInfo_1 ( XMP_FileFormat   format,
                                 XMP_OptionBits * flags,
                                 WXMP_Result *    wResult )
{
	XMP_ENTER_Static ( "WXMPFiles_GetFormatInfo_1" )

		wResult->int32Result = XMPFiles::GetFormatInfo ( format, flags );

	XMP_EXIT
}

// -------------------------------------------------------------------------------------------------

void WXMPFiles_CheckFileFormat_1 ( XMP_StringPtr filePath,
								   WXMP_Result * wResult )
{
	XMP_ENTER_Static ( "WXMPFiles_CheckFileFormat_1" )

		wResult->int32Result = XMPFiles::CheckFileFormat ( filePath );

	XMP_EXIT
}

// -------------------------------------------------------------------------------------------------

void WXMPFiles_CheckPackageFormat_1 ( XMP_StringPtr folderPath,
                       				  WXMP_Result * wResult )
{
	XMP_ENTER_Static ( "WXMPFiles_CheckPackageFormat_1" )

		wResult->int32Result = XMPFiles::CheckPackageFormat ( folderPath );

	XMP_EXIT
}

// -------------------------------------------------------------------------------------------------

void WXMPFiles_GetFileModDate_1 ( XMP_StringPtr    filePath,
								  XMP_DateTime *   modDate,
								  XMP_FileFormat * format,
								  XMP_OptionBits   options,
								  WXMP_Result *    wResult )
{
	XMP_ENTER_Static ( "WXMPFiles_GetFileModDate_1" )

		wResult->int32Result = XMPFiles::GetFileModDate ( filePath, modDate, format, options );

	XMP_EXIT
}

// -------------------------------------------------------------------------------------------------

void WXMPFiles_GetAssociatedResources_1 ( XMP_StringPtr             filePath,
										  void *                    resourceList,
										  XMP_FileFormat            format, 
										  XMP_OptionBits            options, 
					                      SetClientStringVectorProc SetClientStringVector,
										  WXMP_Result *             wResult )
{
	XMP_ENTER_Static ( "WXMPFiles_GetAssociatedResources_1" )

		if ( resourceList == 0 ) XMP_Throw ( "An result resource list vector must be provided", kXMPErr_BadParam );

		std::vector<std::string> resList;	// Pass a local vector, not the client's.
		(*SetClientStringVector) ( resourceList, 0, 0 );	// Clear the client's result vector.
		wResult->int32Result = XMPFiles::GetAssociatedResources ( filePath, &resList, format, options );

		if ( wResult->int32Result && (! resList.empty()) ) {
			const size_t fileCount = resList.size();
			std::vector<XMP_StringPtr> ptrArray;
			ptrArray.reserve ( fileCount );
			for ( size_t i = 0; i < fileCount; ++i ) ptrArray.push_back ( resList[i].c_str() );
			(*SetClientStringVector) ( resourceList, ptrArray.data(), static_cast<XMP_Uns32>(fileCount ));
		}
	XMP_EXIT
}

// -------------------------------------------------------------------------------------------------

void WXMPFiles_IsMetadataWritable_1 ( XMP_StringPtr    filePath,
							          XMP_Bool *       writable, 
							          XMP_FileFormat   format,
							          XMP_OptionBits   options, 
							          WXMP_Result *    wResult )
{
	XMP_ENTER_Static ( "WXMPFiles_IsMetadataWritable_1" )
		wResult->int32Result = XMPFiles::IsMetadataWritable ( filePath, writable, format, options );
	XMP_EXIT
}

// =================================================================================================

#if AdobePrivate
void WXMPFiles_GetAlbumArts_1( XMPFilesRef			xmpObjRef,
								void *				clientVecAlbumArt,
								PushArtInfoProc1	pushArtOnClientVecProc,
								WXMP_Result *		wResult )
{
	XMP_ENTER_ObjWrite( XMPFiles, "WXMPFiles_GetAlbumArts_1" )
		
	if ( clientVecAlbumArt == 0 ) XMP_Throw( "An resultant album art vector must be provided", kXMPErr_BadParam );

	std::vector<AlbumArt> localAlbArtVector;		// Pass a local vector, not the client's.
	wResult->int32Result = thiz->GetAlbumArts( &localAlbArtVector );

	// Storing the album arts into the client provided vector
	if ( wResult->int32Result && ( !localAlbArtVector.empty() ) )
	{
		std::vector<AlbumArt>::iterator iter;
		for ( iter = localAlbArtVector.begin(); iter !=  localAlbArtVector.end(); ++iter )
		{
			( *pushArtOnClientVecProc ) ( clientVecAlbumArt, iter->imageDataLength, iter->description, iter->descLength,
				iter->usageType, iter->formatType, iter->encodingType, iter->imageData );
			if ( iter->description != NULL )
			{
				delete[] iter->description;
				iter->description = 0;
			}
			if ( iter->imageData != NULL )
			{
				delete[] iter->imageData;
				iter->imageData = 0;
			}
		}
	}
	XMP_EXIT
}

// =================================================================================================

void WXMPFiles_PutAlbumArts_1( XMPFilesRef		xmpObjRef,
								void *			clientVecAlbumArt,
								XMP_Uns32		albumArtCount,
								GetArtInfoProc1	getArtInfoFromClientVecProc,
								WXMP_Result *	wResult )
{
	XMP_ENTER_ObjWrite( XMPFiles, "WXMPFiles_PutAlbumArts_1" )
		std::vector<AlbumArt> localArtVector;
	for ( XMP_Uns32 index = 0; index < albumArtCount; ++index )
	{
		AlbumArt albumArt;
		XMP_StringPtr emptyPtr = "";
		XMP_StringPtr descPtr = emptyPtr;
		XMP_BinaryData localArtData = (XMP_BinaryData)emptyPtr;
		( *getArtInfoFromClientVecProc )( clientVecAlbumArt, index, &albumArt.imageDataLength, &descPtr,
			&albumArt.descLength, &albumArt.usageType, &albumArt.formatType, &albumArt.encodingType,
			&localArtData );
		if ( albumArt.descLength != 0 && descPtr != emptyPtr )
		{
			albumArt.description = new char[ albumArt.descLength + 1 ];
			memset( ( void * ) albumArt.description, 0, albumArt.descLength + 1 );
			memcpy( ( void * ) albumArt.description, descPtr, albumArt.descLength );
		}
		if ( albumArt.imageDataLength != 0 && localArtData != ( XMP_BinaryData ) emptyPtr )
		{
			albumArt.imageData = new XMP_Uns8[ albumArt.imageDataLength ];
			memcpy( ( void * ) albumArt.imageData, localArtData, albumArt.imageDataLength );
		}
		localArtVector.push_back( albumArt );
	}
		wResult->int32Result = thiz->PutAlbumArts( localArtVector );
	XMP_EXIT
}

// =================================================================================================
#endif

void WXMPFiles_OpenFile_1 ( XMPFilesRef    xmpObjRef,
                            XMP_StringPtr  filePath,
			                XMP_FileFormat format,
			                XMP_OptionBits openFlags,
                            WXMP_Result *  wResult )
{
	XMP_ENTER_ObjWrite ( XMPFiles, "WXMPFiles_OpenFile_1" )
		StartPerfCheck ( kAPIPerf_OpenFile, filePath );

		bool ok = thiz->OpenFile ( filePath, format, openFlags );
		wResult->int32Result = ok;

		EndPerfCheck ( kAPIPerf_OpenFile );
	XMP_EXIT
}

// -------------------------------------------------------------------------------------------------

#if XMP_StaticBuild	// ! Client XMP_IO objects can only be used in static builds.
void WXMPFiles_OpenFile_2 ( XMPFilesRef    xmpObjRef,
                            XMP_IO*        clientIO,
			                XMP_FileFormat format,
			                XMP_OptionBits openFlags,
                            WXMP_Result *  wResult )
{
	XMP_ENTER_ObjWrite ( XMPFiles, "WXMPFiles_OpenFile_2" )
		StartPerfCheck ( kAPIPerf_OpenFile, "<client-managed>" );

		bool ok = thiz->OpenFile ( clientIO, format, openFlags );
		wResult->int32Result = ok;

		EndPerfCheck ( kAPIPerf_OpenFile );
	XMP_EXIT
}
#endif

// -------------------------------------------------------------------------------------------------

void WXMPFiles_CloseFile_1 ( XMPFilesRef    xmpObjRef,
                             XMP_OptionBits closeFlags,
                             WXMP_Result *  wResult )
{
	XMP_ENTER_ObjWrite ( XMPFiles, "WXMPFiles_CloseFile_1" )
		StartPerfCheck ( kAPIPerf_CloseFile, "" );

		thiz->CloseFile ( closeFlags );

		EndPerfCheck ( kAPIPerf_CloseFile );
	XMP_EXIT
}

// -------------------------------------------------------------------------------------------------

void WXMPFiles_GetFileInfo_1 ( XMPFilesRef      xmpObjRef,
                               void *           clientPath,
			                   XMP_OptionBits * openFlags,
			                   XMP_FileFormat * format,
			                   XMP_OptionBits * handlerFlags,
			                   SetClientStringProc SetClientString,
                               WXMP_Result *    wResult )
{
	XMP_ENTER_ObjRead ( XMPFiles, "WXMPFiles_GetFileInfo_1" )

		XMP_StringPtr pathStr;
		XMP_StringLen pathLen;

		bool isOpen = thiz.GetFileInfo ( &pathStr, &pathLen, openFlags, format, handlerFlags );
		if ( isOpen && (clientPath != 0) ) (*SetClientString) ( clientPath, pathStr, pathLen );
		wResult->int32Result = isOpen;

	XMP_EXIT
}

// -------------------------------------------------------------------------------------------------

void WXMPFiles_SetAbortProc_1 ( XMPFilesRef   xmpObjRef,
                         	    XMP_AbortProc abortProc,
							    void *        abortArg,
							    WXMP_Result * wResult )
{
	XMP_ENTER_ObjWrite ( XMPFiles, "WXMPFiles_SetAbortProc_1" )

		thiz->SetAbortProc ( abortProc, abortArg );

	XMP_EXIT
}

// -------------------------------------------------------------------------------------------------

void WXMPFiles_GetXMP_1 ( XMPFilesRef      xmpObjRef,
                          XMPMetaRef       xmpRef,
		                  void *           clientPacket,
		                  XMP_PacketInfo * packetInfo,
		                  SetClientStringProc SetClientString,
                          WXMP_Result *    wResult )
{
	XMP_ENTER_ObjWrite ( XMPFiles, "WXMPFiles_GetXMP_1" )
		StartPerfCheck ( kAPIPerf_GetXMP, "" );

		bool hasXMP = false;
		XMP_StringPtr packetStr = NULL;
		XMP_StringLen packetLen = 0;

		/*Adding check to handle case where a client might not send XMPMetaRef but still want xmp packet in return. eg. CTECHXMP-4170329*/
		if ( xmpRef == 0 && clientPacket != 0 ) {
			hasXMP = thiz->GetXMP( 0, &packetStr, &packetLen, packetInfo );
		}
		else if ( xmpRef == 0 && clientPacket == 0 ) {
			hasXMP = thiz->GetXMP( 0, 0, 0, packetInfo );
		}
		else {
			SXMPMeta xmpObj(xmpRef);
			hasXMP = thiz->GetXMP( &xmpObj, &packetStr, &packetLen, packetInfo );
		}

		if ( hasXMP && (clientPacket != 0) ) (*SetClientString) ( clientPacket, packetStr, packetLen );
		wResult->int32Result = hasXMP;

		EndPerfCheck ( kAPIPerf_GetXMP );
	XMP_EXIT
}

// -------------------------------------------------------------------------------------------------

void WXMPFiles_PutXMP_1 ( XMPFilesRef   xmpObjRef,
                          XMPMetaRef    xmpRef,	// ! Only one of the XMP object or packet are passed.
                          XMP_StringPtr xmpPacket,
                          XMP_StringLen xmpPacketLen,
                          WXMP_Result * wResult )
{
	XMP_ENTER_ObjWrite ( XMPFiles, "WXMPFiles_PutXMP_1" )
		StartPerfCheck ( kAPIPerf_PutXMP, "" );

		if ( xmpRef != 0 ) {
			thiz->PutXMP ( xmpRef );
		} else {
			thiz->PutXMP ( xmpPacket, xmpPacketLen );
		}

		EndPerfCheck ( kAPIPerf_PutXMP );
	XMP_EXIT
}

// -------------------------------------------------------------------------------------------------

void WXMPFiles_CanPutXMP_1 ( XMPFilesRef   xmpObjRef,
                             XMPMetaRef    xmpRef,	// ! Only one of the XMP object or packet are passed.
                             XMP_StringPtr xmpPacket,
                             XMP_StringLen xmpPacketLen,
                             WXMP_Result * wResult )
{
	XMP_ENTER_ObjWrite ( XMPFiles, "WXMPFiles_CanPutXMP_1" )
		StartPerfCheck ( kAPIPerf_CanPutXMP, "" );

		if ( xmpRef != 0 ) {
			wResult->int32Result = thiz->CanPutXMP ( xmpRef );
		} else {
			wResult->int32Result = thiz->CanPutXMP ( xmpPacket, xmpPacketLen );
		}

		EndPerfCheck ( kAPIPerf_CanPutXMP );
	XMP_EXIT
}

// =================================================================================================

void WXMPFiles_SetDefaultProgressCallback_1 ( XMP_ProgressReportWrapper wrapperProc,
											  XMP_ProgressReportProc    clientProc,
											  void *        context,
											  float         interval,
											  XMP_Bool      sendStartStop,
											  WXMP_Result * wResult )
{
	XMP_ENTER_Static ( "WXMPFiles_SetDefaultProgressCallback_1" )
	
		XMP_ProgressTracker::CallbackInfo cbInfo ( wrapperProc, clientProc, context, interval, ConvertXMP_BoolToBool( sendStartStop ) );
		XMPFiles::SetDefaultProgressCallback ( cbInfo );
	
	XMP_EXIT
}

// -------------------------------------------------------------------------------------------------

void WXMPFiles_SetProgressCallback_1 ( XMPFilesRef   xmpObjRef,
									   XMP_ProgressReportWrapper wrapperProc,
									   XMP_ProgressReportProc    clientProc,
									   void *        context,
									   float         interval,
									   XMP_Bool      sendStartStop,
									   WXMP_Result * wResult )
{
	XMP_ENTER_ObjWrite ( XMPFiles, "WXMPFiles_SetProgressCallback_1" )
	
		XMP_ProgressTracker::CallbackInfo cbInfo ( wrapperProc, clientProc, context, interval, ConvertXMP_BoolToBool( sendStartStop) );
		thiz->SetProgressCallback ( cbInfo );
	
	XMP_EXIT
}

// -------------------------------------------------------------------------------------------------

void WXMPFiles_SetDefaultErrorCallback_1 ( XMPFiles_ErrorCallbackWrapper	wrapperProc,
										   XMPFiles_ErrorCallbackProc		clientProc,
										   void *							context,
										   XMP_Uns32						limit,
										   WXMP_Result *					wResult )
{
	XMP_ENTER_Static ( "WXMPFiles_SetDefaultErrorCallback_1" )

		XMPFiles::SetDefaultErrorCallback ( wrapperProc, clientProc, context, limit );

	XMP_EXIT
}

// -------------------------------------------------------------------------------------------------

void WXMPFiles_SetErrorCallback_1 ( XMPFilesRef						xmpObjRef,
									XMPFiles_ErrorCallbackWrapper	wrapperProc,
									XMPFiles_ErrorCallbackProc		clientProc,
									void *							context,
									XMP_Uns32						limit,
									WXMP_Result *					wResult )
{
	XMP_ENTER_ObjWrite ( XMPFiles, "WXMPFiles_SetErrorCallback_1" )

		thiz->SetErrorCallback ( wrapperProc, clientProc, context, limit );

	XMP_EXIT
}

// -------------------------------------------------------------------------------------------------

void WXMPFiles_ResetErrorCallbackLimit_1 ( XMPFilesRef		xmpObjRef,
										   XMP_Uns32		limit,
										   WXMP_Result *	wResult )
{
	XMP_ENTER_ObjWrite ( XMPFiles, "WXMPFiles_ResetErrorCallbackLimit_1" )

		thiz->ResetErrorCallbackLimit ( limit );

	XMP_EXIT
}

// =================================================================================================

#if __cplusplus
}
#endif
