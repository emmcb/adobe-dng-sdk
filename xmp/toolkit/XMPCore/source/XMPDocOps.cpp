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
// 02-06-15 AJ  5.6-c037 Fixing warnings due to implicit typecasting
// 06-08-12 AWL 5.5-c002 Use standard O/S utilities for GUID creation.
//
// 11-25-11	JEH	5.3-c009 [3055349] fix recognition of redundant entries in sequential NoteChange calls
// 11-22-11	JEH	5.3-c008 [3045488] Prune document's History metadata if it grows too large.
//							Moved DocOps utilities to distinct class, to be able to write unit tests
// mm-dd-11 AWL 5.3-c0vv [3045488] Collapse redundant history in general, not just for metadata changes.
//
// 06-08-10 AWL 5.1-c007 [2548838] Fix string indexing errors in XMPDocOps::NoteChange.
//
// 07-22-09 AWL 5.0-c048 [2381643] Properly fix memory leaks from XMPDocOps::Clone.
// 07-16-09 AWL 5.0-c046 Fix XMPDocOps::Clone to not copy all of the thread lock state.
// 06-26-09 FNO 5.0-c041 Fix PrepareForSave() to check for property existence.
// 06-18-09 AWL 5.0-c037 Minor tweaks from code review.
// 06-11-09 AWL 5.0-c034 Finish threading revamp, implement friendly reader/writer locking.
// 03-03-09 FNO 5.0-c018 [1932212] SRS: Fixed edge case of straight-from-start "metadata/"-sequence.
// 03-03-09 FNO 5.0-c017 [1932212] SRS: Avoid XMP history growth from contiguous metadata changes.
// 07-14-08 AWL 4.4-c005 [1815630] Change stEvt:changed to a simple string instead of an array.
// 07-07-08 JEH 4.4-c004 Redo 4.4-c002 change, changed tests.
// 07-05-08 AWL 4.4-c003 Undo previous change, tests need tweaking too.
// 07-05-08 AWL 4.4-c002 Correct spelling of stEvt:params to stEvt:parameters in DocOps.
//
// 06-06-08 AWL 4.3-c058 [1816538] Fix XMPDocOps::PrepareForSave to ignore empty miimeType and filePath.
// 05-26-08 JEH 4.3-c057 [1790015] BranchXMP: stEvt:action "converted" only created when mimetypes differ.
//
// 04-24-08 AWL 4.2.2-c051 [1765983] Add SXMPDocOps::IsDirty overload to return reason mask.
// 04-04-08 AWL 4.2.2-c050 [1744696] Fix DocOps to delete bad History, e.g. not being an array.
// 04-01-08 AWL 4.2.2-c048 [1742664] Change the history field stEvt:parts to stEvt:changed.
//
// 02-22-08 AWL 4.2-c043 Fix SXMPDocOps::EnsureIDsExist to mark the document as dirty if IDs are created.
// 02-22-08 AWL 4.2-c042 Finish implementation of SXMPDocOps::EnsureIDsExist.
// 02-22-08 AWL 4.2-c041 Add routines to SXMPDocOps - EnsureIDsExist, IsDirty, Clone, and GetMetaRef.
// 02-20-08 AWL 4.2-c040 Fix XMPDocOps::BranchXMP to add the converted-from-to history to derivedDoc.
// 12-10-07 AWL Change name of fileType params to mimeType.
// 12-07-07 AWL 4.2-c031 Fix return value typos in XMPDocOps::HasPartChanged and GetPartChangeID.
// 12-04-07 AWL 4.2-c030 Change the XMPDocOps handling of part names to the new syntax.
// 12-03-07 AWL 4.2-c028 Change file type parameters in XMPDocOps from XMP_FileFormat enum to a MIME string.
// 11-12-07 AWL 4.2-c026 More progress implementing XMPDocOps.
// 11-07-07 AWL 4.2-c025 More progress implementing XMPDocOps.
// 11-02-07 AWL 4.2-c024 Start implementing XMPDocOps.
// 10-31-07 AWL 4.2-c023 Add new class XMPDocOps.
//
// =================================================================================================
#endif // AdobePrivate

#include "XMPCore/source/XMPDocOps.hpp"
#include "XMPCore/source/XMPDocOps-Utils.hpp"

#include "public/include/client-glue/WXMPMeta.hpp"
#include "XMPCore/source/XMPUtils.hpp"

// =================================================================================================

static XMP_VarString * sAppName = 0;
static XMP_ReadWriteLock * sGUIDCreationLock = 0;

static XMP_StringLen  voidLen;		// For "don't want" parameters to GetProperty.
static XMP_OptionBits voidOptions;

#if 0	// *** Not needed here, should become basis of public constants.

FileTypeToMIME standardMIMEMappings[] =	// ! List all of the file types for completeness.
	{ {kXMP_PDFFile,				"application/pdf"},
	  {kXMP_PostScriptFile,			"application/postscript"},
	  {kXMP_EPSFile,				""},
	  {kXMP_JPEGFile,				"image/jpeg"},
	  {kXMP_JPEG2KFile,				"image/jp2"},
	  {kXMP_TIFFFile,				"image/tiff"},
	  {kXMP_GIFFile,				"image/gif"},
	  {kXMP_PNGFile,				"image/png"},
	  {kXMP_SWFFile,				"video/vnd.sealed.swf"},
	  {kXMP_FLAFile,				""},
	  {kXMP_FLVFile,				""},
	  {kXMP_MOVFile,				"video/quicktime"},
	  {kXMP_AVIFile,				""},
	  {kXMP_CINFile,				""},
	  {kXMP_WAVFile,				""},
	  {kXMP_MP3File,				""},
	  {kXMP_SESFile,				""},
	  {kXMP_CELFile,				""},
	  {kXMP_MPEGFile,				"video/mpeg"},
	  {kXMP_MPEG2File,				"video/mpeg"},
	  {kXMP_MPEG4File,				"video/mp4"},
	  {kXMP_WMAVFile,				""},
	  {kXMP_AIFFFile,				""},
	  {kXMP_HTMLFile,				"text/html"},
	  {kXMP_XMLFile,				"text/xml"},
	  {kXMP_TextFile,				"text"},
	  {kXMP_SVGFile,				"image/svg+xml"},
	  {kXMP_PhotoshopFile,			"image/vnd.adobe.photoshop"},
	  {kXMP_IllustratorFile,		""},
	  {kXMP_InDesignFile,			""},
	  {kXMP_AEProjectFile,			""},
	  {kXMP_AEProjTemplateFile,		""},
	  {kXMP_AEFilterPresetFile,		""},
	  {kXMP_EncoreProjectFile,		""},
	  {kXMP_PremiereProjectFile,	""},
	  {kXMP_PremiereTitleFile,		""},
	  {kXMP_UCFFile,				""},
	  {kXMP_WebPFile,				"image/webp"},
	  {0, ""} };

#endif

// =================================================================================================
// =================================================================================================

// -------------------------------------------------------------------------------------------------
// Initialize
// ----------

/* class static */ bool
XMPDocOps::Initialize()
{

	sAppName = new std::string();
	if ( sAppName == 0 ) return false;
	
	sGUIDCreationLock = new XMP_ReadWriteLock();
	if ( sGUIDCreationLock == 0 ) return false;

	return true;

}	// Initialize

// -------------------------------------------------------------------------------------------------
// Terminate
// ---------

/* class static */ void
XMPDocOps::Terminate() RELEASE_NO_THROW
{

	EliminateGlobal ( sAppName );
	EliminateGlobal ( sGUIDCreationLock );

}	// Terminate

// -------------------------------------------------------------------------------------------------
// XMPDocOps
// ---------

XMPDocOps::XMPDocOps() : clientRefs(0), docXMP(0),
						 isNew(false), isDirty(false), isDerived(false),
						 contentChanged(false), allHistoryCleaned(false), dirtyReasons(0)
{

	// Nothing more to do, clientRefs is incremented in wrapper.

}	// XMPDocOps

// -------------------------------------------------------------------------------------------------
// ~XMPDocOps
// ----------

XMPDocOps::~XMPDocOps() RELEASE_NO_THROW
{

	if ( this->docXMP != 0 ) WXMPMeta_DecrementRefCount_1 ( (XMPMetaRef)this->docXMP );

}	// ~XMPDocOps

// =================================================================================================
// =================================================================================================

// -------------------------------------------------------------------------------------------------
// SetAppName
// ----------

/* class static */ void
XMPDocOps::SetAppName ( XMP_StringPtr appName )
{

	*sAppName = appName;

}	// SetAppName

// -------------------------------------------------------------------------------------------------
// CreateID
// --------

/* class static */ void
XMPDocOps::CreateID ( XMP_StringPtr   prefix,
					  XMP_VarString * idStr )
{
	XMP_Assert ( (prefix != 0) && (prefix[0] != 0) );	// Guaranteed by the wrapper.

	XMPDocOpsUtils::ConjureUURI ( prefix, idStr, sGUIDCreationLock );

}	// CreateID

// =================================================================================================
// =================================================================================================

// -------------------------------------------------------------------------------------------------
// ResetDoc
// --------
//
// Reset the XMPDocOps object to a just-constructed state plus the docXMP set. This allows reuse.
// A write lock on xmpMeta is already held from the wrapper of the caller (NewXMP, etc.), so direct
// increment of that ref count is safe. Need to call WXMPMeta_DecrementRefCount_1 on any existing
// this->docXMP though, both to get a write lock and to possibly trigger destruction of the XMPMeta.

void
XMPDocOps::ResetDoc ( XMPMeta * xmpMeta )
{

	if ( xmpMeta == 0 ) XMP_Throw ( "XMPDocOps object needs non-null XMPMeta", kXMPErr_BadParam );
	
	if ( this->docXMP != xmpMeta ) {
		if ( this->docXMP != 0 ) WXMPMeta_DecrementRefCount_1 ( (XMPMetaRef)this->docXMP );
		if ( xmpMeta != 0 ) ++xmpMeta->clientRefs;	// ! Safe, see above.
		this->docXMP = xmpMeta;
	}
	
	this->isDirty = this->isNew = this->isDerived = false;
	this->dirtyReasons = 0;

}	// ResetDoc


// -------------------------------------------------------------------------------------------------
// AppendHistory
// -------------
//
// Append a minimal xmpMM:History entry with just an action and parameters.

void
XMPDocOps::AppendHistory ( XMP_StringPtr action, XMP_StringPtr params )
{
	XMP_Assert ( (action != 0) && (action[0] != 0) );
	XMP_Assert ( params != 0 );

	XMPMeta * xmpMeta = this->docXMP;
	if ( xmpMeta == 0 ) return;

	XMPDocOpsUtils::EnsureHistoryForm ( xmpMeta );
	xmpMeta->AppendArrayItem ( kXMP_NS_XMP_MM, "History", kXMP_PropArrayIsOrdered, 0, kXMP_PropValueIsStruct );

	XMPDocOpsUtils::AppendHistoryField ( xmpMeta, "action", action );
	if ( params[0] != 0 ) XMPDocOpsUtils::AppendHistoryField ( xmpMeta, "parameters", params );

}	// AppendHistory

// -------------------------------------------------------------------------------------------------
// AppendHistory
// -------------
//
// Append a more or less full xmpMM:History entry.

void
XMPDocOps::AppendHistory ( XMP_StringPtr action, XMP_StringPtr params,
						   XMP_StringPtr instanceID, XMP_StringPtr when )
{
	XMP_Assert ( (action != 0) && (action[0] != 0) );
	XMP_Assert ( params != 0 );
	XMP_Assert ( instanceID != 0 );
	XMP_Assert ( when != 0 );

	XMPMeta * xmpMeta = this->docXMP;
	if ( xmpMeta == 0 ) return;

	XMPDocOpsUtils::EnsureHistoryForm ( xmpMeta );
	xmpMeta->AppendArrayItem ( kXMP_NS_XMP_MM, "History", kXMP_PropArrayIsOrdered, 0, kXMP_PropValueIsStruct );

	XMPDocOpsUtils::AppendHistoryField ( xmpMeta, "action", action );
	if ( params[0] != 0 ) XMPDocOpsUtils::AppendHistoryField ( xmpMeta, "parameters", params );
	if ( instanceID[0] != 0 ) XMPDocOpsUtils::AppendHistoryField ( xmpMeta, "instanceID", instanceID );
	if ( when[0] != 0 ) XMPDocOpsUtils::AppendHistoryField ( xmpMeta, "when", when );

	if ( ! sAppName->empty() ) XMPDocOpsUtils::AppendHistoryField ( xmpMeta, "softwareAgent", sAppName->c_str() );

	if ( XMP_LitMatch ( action, "saved" ) && (! this->changedParts.empty()) ) {

		// Record the changed parts for "saved" actions.

		XMP_VarString partField;
		XMPUtils::ComposeStructFieldPath ( kXMP_NS_XMP_MM, "History[last()]",
										   kXMP_NS_XMP_ResourceEvent, "changed", &partField );

		std::string partList = this->changedParts[0];
		for ( size_t i = 1, limit = this->changedParts.size(); i < limit; ++i ) {
			partList += ';';
			partList += this->changedParts[i];
		}

		xmpMeta->SetProperty ( kXMP_NS_XMP_MM, partField.c_str(), partList.c_str(), kXMP_NoOptions );

	}

}	// AppendHistory


// -------------------------------------------------------------------------------------------------
// PruneRedundantHistory
// ---------------------
//
// Prune sequences of "saved" actions with identical parts to just the first and last entries.
// Metadata editing in Bridge and frequent or auto saves in the creative applications often results
// in history bloat otherwise. This is called from PrepareForSave after appending the latest entry,
// which can be a "created" action in some cases. We always check the tail of the history in case
// the current save has created a sequence of three or more similar entries. If the history is still
// more than 100 entries we will do a global cleaning, once for the life of this XMPDocOps object.
// The global cleaning might reduce the total history for files that had problems before we started
// the tail pruning.

void
XMPDocOps::PruneRedundantHistory()
{

	// Find the xmpMM:History node.
	
	XMPMeta * xmpMeta = this->docXMP;
	XMP_NodePtr mmNode = FindSchemaNode ( &xmpMeta->tree, kXMP_NS_XMP_MM, false, 0 );
	XMP_NodePtr historyNode = FindChildNode ( mmNode, "xmpMM:History", false, 0 );
	XMP_Assert ( historyNode != 0 );	// Should always exist, called from PrepareForSave after AppendHistory.
	
	XMP_Int32 historySize = (XMP_Int32)historyNode->children.size();
	if ( historySize < 3 ) return;
	
	// Prune redundant "saved" entries from the tail of the history.
	
	XMPDocOpsUtils::PruneOneRedundantRun ( historyNode, (historySize - 1) );
	
	// If the history is still big do a global cleanup once for this XMPDocOps object. Use 1000
	// history entries as the general case limit. Use 100 as the limit for JPEG, trying to keep the
	// XMP under 64 KB. Each history entry is about 550 bytes, so 100 of them is about 55 KB.
	
	if ( this->allHistoryCleaned ) return;
	this->allHistoryCleaned = true;
	
	XMP_Int32 historyLimit = 1000;
	XMP_StringPtr dcFormat;
	bool found = xmpMeta->GetProperty ( kXMP_NS_DC, "format", &dcFormat, &voidLen, &voidOptions );
	if ( found && XMP_LitMatch ( dcFormat, "image/jpeg" ) ) historyLimit = 100;
	
	historySize = (XMP_Int32)historyNode->children.size();	// Set again, it got reduced above.
	if ( historySize < historyLimit ) return;
	
	// Go forward through the history and normalize the stEvt:changed fields.
	
	for ( XMP_Int32 item = 0; item < historySize; ++item ) {
		XMP_NodePtr currEntry = historyNode->children[item];
		XMP_NodePtr currParts = FindChildNode ( currEntry, "stEvt:changed", false, 0 );
		if ( currParts != 0 ) XMPDocOpsUtils::NormalizePartsList ( currParts );
	}
	
	// Repeat the redundant run pruning, now looking at the entire history array. Start at the end
	// again, the stEvt:changed normalization might have exposed a run.
	
	XMP_Int32 endIndex = historySize - 1;
	while ( endIndex >= 2 ) {
		endIndex = XMPDocOpsUtils::PruneOneRedundantRun ( historyNode, endIndex );
	}

}	// PruneRedundantHistory

// -------------------------------------------------------------------------------------------------
// NewXMP
// ------

void
XMPDocOps::NewXMP ( XMPMeta *      xmpMeta,
					XMP_StringPtr  mimeType,
					XMP_OptionBits options )
{
	IgnoreParam(options);	// Avoid unused parameter warnings.
	XMP_Assert ( mimeType != 0 );	// Wrapper forces null pointer to "".

	// Reset the XMPDocOps object data members, then mark it as new and dirty.

	this->ResetDoc ( xmpMeta );
	this->prevMIMEType = mimeType;
	this->isNew = this->isDirty = true;
	this->dirtyReasons |= kXMPDirtyDoc_New;

	// Set dc:format if the type is known.

	if ( mimeType[0] != 0 ) {
		xmpMeta->SetProperty ( kXMP_NS_DC, "format", mimeType, kXMP_DeleteExisting );
	}

	// Set the xmp:CreatorTool and xmp:CreateDate properties.

	if ( ! sAppName->empty() ) {
		xmpMeta->SetProperty ( kXMP_NS_XMP, "CreatorTool", sAppName->c_str(), kXMP_DeleteExisting );
	}

	XMP_DateTime now;
	XMPUtils::CurrentDateTime ( &now );
	XMPUtils::ConvertToLocalTime ( &now );
	xmpMeta->SetProperty_Date ( kXMP_NS_XMP, "CreateDate", now, kXMP_DeleteExisting );

	this->InternalNoteChangeAll();

}	// NewXMP

// -------------------------------------------------------------------------------------------------
// OpenXMP
// -------

#ifndef DeletePropsAtOpen	// Should properties known to be changing be deleted in OpenXMP?
	#define DeletePropsAtOpen 0
#endif

void
XMPDocOps::OpenXMP ( XMPMeta *      xmpMeta,
					 XMP_StringPtr  mimeType,
					 XMP_StringPtr  filePath,
					 XMP_DateTime * fileModTime,
					 XMP_OptionBits options )
{
	IgnoreParam(options);	// Avoid unused parameter warnings.
	XMP_Assert ( mimeType != 0 );	// Wrapper forces null pointer to "".
	XMP_Assert ( filePath != 0 );	// Wrapper forces null pointer to "".

	bool haveOld;

	this->ResetDoc ( xmpMeta );
	this->prevMIMEType = mimeType;
	this->prevFilePath = filePath;

	// See if the file type has been changed.

	if ( mimeType[0] != 0 ) {

		XMP_StringPtr oldType;
		haveOld = xmpMeta->GetProperty ( kXMP_NS_DC, "format", &oldType, &voidLen, &voidOptions );

		if ( haveOld && (! XMP_LitMatch ( mimeType, oldType )) ) {

			this->isDirty = this->isDerived = true;
			this->dirtyReasons |= kXMPDirtyDoc_TypeChangedAtOpen;

			#if DeletePropsAtOpen
				xmpMeta->DeleteProperty ( kXMP_NS_DC, "format" );
				xmpMeta->DeleteProperty ( kXMP_NS_XMP_MM, "InstanceID" );
				xmpMeta->DeleteProperty ( kXMP_NS_XMP_MM, "DocumentID" );
			#endif

			XMP_VarString histParams = "from ";
			histParams += oldType;
			histParams += " to ";
			histParams += mimeType;

			this->AppendHistory ( "converted", histParams.c_str() );

		}

		/*In order to propagate mimetype changes correctly between OpenXMP, BranchXMP and PrepareForSave
		the mimetype would need to be set in the XMP packet as well. OpenXMP as well as BranchXMP rely on the XMP packet
		as reference for the mimetype to check for changes whereas PrepareForSave does not.
		This needs further discussion therefore it is commented out.
		*/
		//xmpMeta->SetProperty ( kXMP_NS_DC, "format", mimeType, kXMP_DeleteExisting );
	}

	// See if the file modification time has changed.

	if ( fileModTime != 0 ) {

		XMP_DateTime oldModTime;
		haveOld = XMPDocOpsUtils::GetLastModTime ( *xmpMeta, &oldModTime );

		if ( haveOld && (XMPUtils::CompareDateTime ( *fileModTime, oldModTime ) > 0) ) {
			this->isDirty = true;
			this->dirtyReasons |= kXMPDirtyDoc_DateChangedAtOpen;
			#if DeletePropsAtOpen
				xmpMeta->DeleteProperty ( kXMP_NS_XMP_MM, "InstanceID" );
			#endif
			this->AppendHistory ( "modified", "unknown modifications" );
		}

	}

}	// OpenXMP

// -------------------------------------------------------------------------------------------------
// BranchXMP
// ---------

void
XMPDocOps::BranchXMP ( XMPDocOps *    derivedDoc,
					   XMPMeta *      derivedMeta,
					   XMP_StringPtr  mimeType,
					   XMP_OptionBits options )
{
	IgnoreParam(options);	// Avoid unused parameter warnings.
	XMP_Assert ( mimeType != 0 );	// Wrapper forces null pointer to "".

	XMPMeta * xmpMeta = this->docXMP;
	if ( xmpMeta == 0 ) XMP_Throw ( "Must call NewXMP or OpenXMP first", kXMPErr_BadObject );

	// Do some initial setup for the single or dual document cases.
	// ! Later code must allow for single document case!

	bool singleDoc = (this == derivedDoc);

	if ( singleDoc ) {

		if ( derivedMeta == 0 ) derivedMeta = xmpMeta;
		if ( derivedMeta != xmpMeta ) {
			XMP_Throw ( "Invalid single-document derived XMPMeta", kXMPErr_BadParam );
		}
		//PrepareForSave only checks prevMIMEType to detect mimetype changes.
		if ( mimeType[0] != 0 ) this->prevMIMEType = mimeType; 

	} else {

		if ( derivedMeta == 0 ) {
			XMP_Throw ( "Must pass derived XMPMeta pointer", kXMPErr_BadParam );
		}
		//transfer the path of the old doc so that the derived history event gets created if the paths change
		//this-prevFilePath value will be deleted with next PrepareForSave statement, therefore set it here.
		derivedDoc->prevFilePath = this->prevFilePath; 

		this->PrepareForSave ( "", "", 0, 0 );	// Make sure the source has IDs, if it is already dirty.
		derivedDoc->ResetDoc ( derivedMeta );
		if ( mimeType[0] != 0 ) derivedDoc->prevMIMEType = mimeType;
		xmpMeta->Clone ( derivedMeta, kXMP_NoOptions );

	}

	// If the source document is dirty, create the next InstanceID now so that it can go into the
	// derived document's DerivedFrom information (which gets set in PrepareForSave).

	if ( this->isDirty && this->nextInstanceID.empty() ) CreateID ( "xmp.iid:", &this->nextInstanceID );

	// Append a "converted" history entry to the derived document if the mimetype has changed, note that 
	//all of the derived document has changed, and mark the derived document as diry and derived.

	if ( mimeType[0] != 0 ) {
		XMP_StringPtr oldType;
		bool hasOld = xmpMeta->GetProperty ( kXMP_NS_DC, "format", &oldType, &voidLen, &voidOptions );

		if ( hasOld && (! XMP_LitMatch ( mimeType, oldType )) ) {
			XMP_VarString histParams = "from ";
			histParams += oldType;
			histParams += " to ";
			histParams += mimeType;

			derivedDoc->AppendHistory ( "converted", histParams.c_str() );
		}

		/*In order to propagate mimetype changes correctly between OpenXMP, BranchXMP and PrepareForSave
		the mimetype would need to be set in the XMP packet as well. OpenXMP as well as BranchXMP rely on the XMP packet
		as reference for the mimetype to check for changes whereas PrepareForSave does not.
		This needs further discussion therefore it is commented out.
		*/
		//derivedMeta->SetProperty ( kXMP_NS_DC, "format", mimeType, kXMP_DeleteExisting );
	}
	
	derivedDoc->InternalNoteChangeAll();

	derivedDoc->isDirty = derivedDoc->isDerived = true;
	derivedDoc->dirtyReasons |= kXMPDirtyDoc_Derived;

}	// BranchXMP

// -------------------------------------------------------------------------------------------------
// PrepareForSave
// --------------

void
XMPDocOps::PrepareForSave ( XMP_StringPtr  mimeType,
							XMP_StringPtr  filePath,
							XMP_DateTime * fileModTime,
							XMP_OptionBits options )
{
	IgnoreParam(options);	// Avoid unused parameter warnings.
	XMP_Assert ( mimeType != 0 );	// Wrapper forces null pointer to "".
	XMP_Assert ( filePath != 0 );	// Wrapper forces null pointer to "".

	XMPMeta * xmpMeta = this->docXMP;
	if ( xmpMeta == 0 ) XMP_Throw ( "Must call NewXMP or OpenXMP first", kXMPErr_BadObject );

	// See if there is an implicit derivation due to a file type or path change. Save the new type
	// and  path to check in a later PrepareForSave.

	bool newType = ( (mimeType[0] != 0) &&
					 (! this->prevMIMEType.empty()) &&
					 (this->prevMIMEType != mimeType) );

	bool newPath = ( (filePath[0] != 0) &&
					 (! this->prevFilePath.empty()) &&
					 (this->prevFilePath != filePath) );

	if ( newType | newPath ) {

		XMP_VarString params;

		if ( newType ) {
			params = "converted from ";
			params += this->prevMIMEType;
			params += " to ";
			params += mimeType;
		}

		if ( newPath ) {
			if ( newType ) params += ", ";
			params += "saved to new location";
		}

		this->AppendHistory ( "derived", params.c_str() );
		this->isDerived = this->isDirty = true;
		// ! No point setting this->dirtyReasons, this->isDirty will be cleared on return.

	}

	if ( mimeType[0] != 0 ) this->prevMIMEType = mimeType;
	if ( filePath[0] != 0 ) this->prevFilePath = filePath;

	// Quit if the document is not dirty.

	if ( this->isNew | this->isDerived ) {
		this->isDirty = true;	// Consistency check.
		// ! No point setting this->dirtyReasons, this->isDirty will be cleared on return.
	}
	if ( ! this->isDirty ) return;

	// Set dc:format, xmp:MetadataDate, and xmp:ModifyDate.

	if ( mimeType[0] != 0 ) {
		xmpMeta->SetProperty ( kXMP_NS_DC, "format", mimeType, kXMP_DeleteExisting );
	}

	XMP_DateTime localTime;
	if ( fileModTime == 0 ) {	// ! Yes, get a null pointer not an "empty" date.
		XMPUtils::CurrentDateTime ( &localTime );
		fileModTime = &localTime;
	}

	XMP_VarString dateStr;
	XMPUtils::ConvertFromDate ( *fileModTime, &dateStr );

	xmpMeta->SetProperty ( kXMP_NS_XMP, "MetadataDate", dateStr.c_str(), kXMP_DeleteExisting );

	if ( this->contentChanged ) {
		xmpMeta->SetProperty ( kXMP_NS_XMP, "ModifyDate", dateStr.c_str(), kXMP_DeleteExisting );
	}
	
	// Try to set xmpMM:DocumentID and xmpMM:OriginalDocumentID if they don't already exist. Do this
	// before the xmpMM:InstanceID or xmpMM:DocumentID get changed. We'll also check again later in
	// case they can't be set at this time.

	XMP_StringPtr docID;
	XMP_VarString docIDStr;

	bool haveDocID = xmpMeta->GetProperty ( kXMP_NS_XMP_MM, "DocumentID", &docID, &voidLen, &voidOptions );
	bool haveOrigID = xmpMeta->DoesPropertyExist ( kXMP_NS_XMP_MM, "OriginalDocumentID" );

	if ( ! haveDocID ) {
		haveDocID = xmpMeta->GetProperty ( kXMP_NS_XMP_MM, "InstanceID", &docID, &voidLen, &voidOptions );
		if ( haveDocID ) {
			if ( XMP_LitNMatch ( docID, "xmp.iid:", 8 ) ) {
				docIDStr = docID;
				docIDStr[4] = 'd';	// Change xmp.iid to xmp.did.
				docID = docIDStr.c_str();
			}
			xmpMeta->SetProperty ( kXMP_NS_XMP_MM, "DocumentID", docID, kXMP_NoOptions );
		}
	}

	if ( (! haveOrigID) && haveDocID ) {
		xmpMeta->SetProperty ( kXMP_NS_XMP_MM, "OriginalDocumentID", docID, kXMP_NoOptions );
		haveOrigID = true;
	}

	// Create xmpMM:DerivedFrom if appropriate.

	if ( this->isDerived ) {

		xmpMeta->SetProperty ( kXMP_NS_XMP_MM, "DerivedFrom", 0, (kXMP_PropValueIsStruct | kXMP_DeleteExisting) );
		XMPDocOpsUtils::FillResourceRef ( *xmpMeta, xmpMeta, kXMP_NS_XMP_MM, "DerivedFrom" );

		if ( this->nextInstanceID.empty() ) CreateID ( "xmp.iid:", &this->nextInstanceID );
		docID = this->nextInstanceID.c_str();
		if ( XMP_LitNMatch ( docID, "xmp.iid:", 8 ) ) {
			docIDStr = docID;
			docIDStr[4] = 'd';	// Change xmp.iid to xmp.did.
			docID = docIDStr.c_str();
		}
		xmpMeta->SetProperty ( kXMP_NS_XMP_MM, "DocumentID", docID, kXMP_DeleteExisting );
		haveDocID = true;

	}

	// Set the new xmpMM:InstanceID, xmpMM:DocumentID, and xmpMM:OriginalDocumentID.

	if ( this->nextInstanceID.empty() ) CreateID ( "xmp.iid:", &this->nextInstanceID );
	xmpMeta->SetProperty ( kXMP_NS_XMP_MM, "InstanceID", this->nextInstanceID.c_str(), kXMP_DeleteExisting );

	if ( ! haveDocID ) {
		docID = this->nextInstanceID.c_str();
		if ( XMP_LitNMatch ( docID, "xmp.iid:", 8 ) ) {
			docIDStr = docID;
			docIDStr[4] = 'd';	// Change xmp.iid to xmp.did.
			docID = docIDStr.c_str();
		}
		xmpMeta->SetProperty ( kXMP_NS_XMP_MM, "DocumentID", docID, kXMP_NoOptions );
	}

	if ( ! haveOrigID ) {
		xmpMeta->SetProperty ( kXMP_NS_XMP_MM, "OriginalDocumentID", docID, kXMP_NoOptions );
	}

	// Append the xmpMM:History entry, mark the document as clean.

	XMP_StringPtr action = "saved";
	if ( this->isNew ) {
		action = "created";
		XMP_StringPtr tmp;
		bool dateExists = xmpMeta->GetProperty ( kXMP_NS_XMP, "CreateDate", &tmp, &voidLen, &voidOptions );
		if ( dateExists ) dateStr = tmp;
	}
	this->AppendHistory ( action, "", this->nextInstanceID.c_str(), dateStr.c_str() );

	if ( XMP_LitNMatch (action, "saved", 5) ) this->PruneRedundantHistory();	// Reduce long sequences of save events.

	this->nextInstanceID.erase();
	this->changedParts.clear();
	this->isDirty = this->isNew = this->isDerived = this->contentChanged = false;
	this->dirtyReasons = 0;

}	// PrepareForSave

// -------------------------------------------------------------------------------------------------
// NoteChange
// ----------
//
// Part name syntax: There are 2 distinguished parts, "all" and "none", that mean exactly that.
// There are currently 2 general top level parts, "metadata" and "content". They can have colon
// separated subparts. All sibling parts are assumed to be disjoint.

void
XMPDocOps::NoteChange ( XMP_StringPtr * partsPtr,
						XMP_Index       partsCount )
{
	XMPMeta * xmpMeta = this->docXMP;
	if ( xmpMeta == 0 ) XMP_Throw ( "Must call NewXMP or OpenXMP first", kXMPErr_BadObject );

	if ( partsCount == 0 ) return;

	this->nextInstanceID.erase();	// Handle BranchXMP followed by further edits.

	// If the changed list already contains "/" any additional part can be ignored
	if( this->changedParts.size() == 1 && this->changedParts[0] == "/" )
	{
		return;
	}

	std::string newPart;
	
	for ( ; partsCount > 0; --partsCount, ++partsPtr ) {

		newPart = *partsPtr;
		XMPDocOpsUtils::NormalizePartPath ( newPart );
		size_t newLen = newPart.size();

		// Treat the "/" (all) part specially.

		if ( newPart == "/" ) {
			this->changedParts.clear();
			this->changedParts.push_back ( "/" );
			this->isDirty = this->contentChanged = true;
			this->dirtyReasons |= (kXMPDirtyDoc_ContentChanged | kXMPDirtyDoc_MetadataChanged);
			return;
		}

		// Integrate this part. Ignore if it or an ancestor are already present. Remove children if
		// this is a new ancestor.

		size_t old = 0;
		size_t limit = this->changedParts.size();

		for ( ; old < limit; ++old ) {

			XMP_StringPtr oldPart = this->changedParts[old].c_str();
			size_t oldLen  = this->changedParts[old].size();

			if ( newLen == oldLen ) {
				// Ignore this part if it is already present.
				if ( newPart == oldPart ) break;
			} else if ( newLen > oldLen ) {
				// Ignore this part if an ancestor is already present.
				if ( (newPart[oldLen] == '/') && XMP_LitNMatch ( newPart.c_str(), oldPart, oldLen ) ) break;
			} else {	// newLen < oldLen
				// Remove existing children of this part.
				if ( (oldPart[newLen] == '/') && XMP_LitNMatch ( newPart.c_str(), oldPart, newLen ) ) {
					this->changedParts.erase ( this->changedParts.begin() + old );
					--old;	// ! Decrement loop counter and limit, next pass should be same position.
					--limit;
				}
			}

		}

		if ( old == limit ) {	// Append this part if the loop finished.
			this->changedParts.push_back ( newPart );
			this->isDirty = true;
			if ( ( (newLen == 8) || ((newLen > 8) && (newPart[8] == '/')) ) &&
				 XMP_LitNMatch ( newPart.c_str(), "/content", 8 ) ) {
				this->contentChanged = true;
				this->dirtyReasons |= kXMPDirtyDoc_ContentChanged;
			} else if ( ( (newLen == 9) || ((newLen > 9) && (newPart[9] == '/')) ) &&
						XMP_LitNMatch ( newPart.c_str(), "/metadata", 9 ) ) {
				this->dirtyReasons |= kXMPDirtyDoc_MetadataChanged;
			}
		}

	}

}	// NoteChange

// -------------------------------------------------------------------------------------------------
// InternalNoteChangeAll
// ---------------------
//
// Note that all of a document has changed for internal reasons. Called from NewXMP and BranchXMP,
// it avoids clearing the nextInstanceID string.

void
XMPDocOps::InternalNoteChangeAll()
{

	this->changedParts.clear();
	this->changedParts.push_back ( "/" );
	this->isDirty = this->contentChanged = true;
	this->dirtyReasons |= (kXMPDirtyDoc_ContentChanged | kXMPDirtyDoc_MetadataChanged);

}	// InternalNoteChangeAll

// =================================================================================================
// =================================================================================================

// -------------------------------------------------------------------------------------------------
// HasPartChanged
// --------------
//
// Determine if any of the given parts have changed since the save with the given InstanceID. We get
// a yes answer from "saved" or "converted" events, a maybe answer from "copied" or "modified"
// events, and a maybe answer if no end point is found ("created" or "saved" with the given InstanceID).

XMP_TriState
XMPDocOps::HasPartChanged ( XMP_StringPtr   prevInstanceID,
							XMP_StringPtr * partsPtr,
							XMP_Index       partsCount ) const
{
	XMPMeta * xmpMeta = this->docXMP;
	if ( xmpMeta == 0 ) XMP_Throw ( "Must call NewXMP or OpenXMP first", kXMPErr_BadObject );

	if ( partsCount == 0 ) return kXMPTS_No;

	XMP_ExpandedXPath	expPath;
	ExpandXPath ( kXMP_NS_XMP_MM, "History", &expPath );
	const XMP_Node * arrayNode = FindConstNode ( &xmpMeta->tree, expPath );
	if ( arrayNode == 0 ) return kXMPTS_Maybe;
	if ( ! XMP_PropIsArray ( arrayNode->options ) ) return kXMPTS_Maybe;

	bool haveMaybe = false;	// Used for "copied" or "modified" maybes.
	XMP_NodePtr historyNode, actionNode, idNode, partsNode;

	for ( size_t item = (int)arrayNode->children.size(); item > 0; --item ) {

		historyNode = arrayNode->children[item-1];
		if ( ! XMP_PropIsStruct ( historyNode->options ) ) continue;	// Ignore bad history.

		// *** Depends on stEvt namespace prefix:
		actionNode = FindChildNode ( historyNode, "stEvt:action", kXMP_ExistingOnly );
		if ( actionNode == 0 ) continue;	// Ignore bad history.

		if ( actionNode->value == "saved" ) {

			// *** Depends on stEvt namespace prefix:
			idNode = FindChildNode ( historyNode, "stEvt:instanceID", kXMP_ExistingOnly );

			if ( (idNode != 0) && (idNode->value == prevInstanceID) ) {

				// Found the "saved" event with the given InstanceID.
				if ( haveMaybe ) return kXMPTS_Maybe;
				return kXMPTS_No;

			} else {

				// Found some other "saved" event, see what changed.
				// *** Depends on stEvt namespace prefix:
				partsNode = FindChildNode ( historyNode, "stEvt:changed", kXMP_ExistingOnly );
				if ( (partsNode == 0) || partsNode->value.empty() ) {
					haveMaybe = true;
				} else {
					if ( XMPDocOpsUtils::IsPartInList ( partsPtr, partsCount, *partsNode ) ) return kXMPTS_Yes;
				}

			}

		} else if ( actionNode->value == "created" ) {

			// *** Depends on stEvt namespace prefix:
			idNode = FindChildNode ( historyNode, "stEvt:instanceID", kXMP_ExistingOnly );
			if ( (idNode == 0) || (idNode->value != prevInstanceID) ) haveMaybe = true;
			if ( haveMaybe ) return kXMPTS_Maybe;
			return kXMPTS_No;

		} else if ( actionNode->value == "converted" ) {

			return kXMPTS_Yes;

		} else if ( (actionNode->value == "copied") || (actionNode->value == "modified") ) {

			haveMaybe = true;

		}

	}

	return kXMPTS_Maybe;	// The save for the given InstanceID was not found.

}	// HasPartChanged

// -------------------------------------------------------------------------------------------------
// GetPartChangeID
// ---------------
//
// Get the InstanceID of the most recent save that might have modified any of the given parts. This
// has to look at "converted", "copied", and "modified" events also as indications of change. Return
// true if an appropriate event is found. No ID is returned if the result is false.

bool
XMPDocOps::GetPartChangeID ( XMP_StringPtr * partsPtr,
							 XMP_Index       partsCount,
							 XMP_StringPtr * idStr,
							 XMP_StringLen * idLen ) const
{
	XMPMeta * xmpMeta = this->docXMP;
	if ( xmpMeta == 0 ) XMP_Throw ( "Must call NewXMP or OpenXMP first", kXMPErr_BadObject );

	if ( partsCount == 0 ) return false;

	XMP_ExpandedXPath	expPath;
	ExpandXPath ( kXMP_NS_XMP_MM, "History", &expPath );
	const XMP_Node * arrayNode = FindConstNode ( &xmpMeta->tree, expPath );
	if ( arrayNode == 0 ) return false;
	if ( ! XMP_PropIsArray ( arrayNode->options ) ) return false;

	XMP_NodePtr historyNode, actionNode, idNode, partsNode;
	XMP_NodePtr prevSave = 0;

	for ( size_t item = (int)arrayNode->children.size(); item > 0; --item ) {

		historyNode = arrayNode->children[item-1];
		if ( ! XMP_PropIsStruct ( historyNode->options ) ) continue;	// Ignore bad history.

		// *** Depends on stEvt namespace prefix:
		actionNode = FindChildNode ( historyNode, "stEvt:action", kXMP_ExistingOnly );
		if ( actionNode == 0 ) continue;	// Ignore bad history.

		if ( actionNode->value == "saved" ) {

			prevSave = historyNode;

			// *** Depends on stEvt namespace prefix:
			partsNode = FindChildNode ( historyNode, "stEvt:changed", kXMP_ExistingOnly );
			if ( (partsNode == 0) || partsNode->value.empty() ) return false;
			if ( ! XMPDocOpsUtils::IsPartInList ( partsPtr, partsCount, *partsNode ) ) continue;

			// *** Depends on stEvt namespace prefix:
			idNode = FindChildNode ( historyNode, "stEvt:instanceID", kXMP_ExistingOnly );
			if ( (idNode == 0) || idNode->value.empty() ) return false;
			*idStr = idNode->value.c_str();
			*idLen = static_cast< XMP_StringLen >( idNode->value.size());
			return true;

		} else if ( actionNode->value == "created" ) {

			// *** Depends on stEvt namespace prefix:
			idNode = FindChildNode ( historyNode, "stEvt:instanceID", kXMP_ExistingOnly );
			if ( (idNode == 0) || idNode->value.empty() ) return false;
			*idStr = idNode->value.c_str();
			*idLen = static_cast< XMP_StringLen >( idNode->value.size() );
			return true;

		} else if ( (actionNode->value == "converted") ||
					(actionNode->value == "copied") ||
					(actionNode->value == "modified") ) {

			if ( prevSave == 0 ) return false;

			// *** Depends on stEvt namespace prefix:
			idNode = FindChildNode ( prevSave, "stEvt:instanceID", kXMP_ExistingOnly );
			if ( (idNode == 0) || idNode->value.empty() ) return false;
			*idStr = idNode->value.c_str();
			*idLen = static_cast< XMP_StringLen >( idNode->value.size() );
			return true;

		}

	}

	return false;

}	// GetPartChangeID

// -------------------------------------------------------------------------------------------------
// EnsureIDsExist
// --------------

bool
XMPDocOps::EnsureIDsExist ( XMP_OptionBits options )
{
	bool madeNewIDs = false;
	
	XMPMeta * xmpMeta = this->docXMP;
	if ( xmpMeta == 0 ) XMP_Throw ( "Must have associated XMPMeta", kXMPErr_BadValue );

	XMP_VarString idStr;
	XMP_StringPtr idPtr = 0;
	
	bool haveInstID = xmpMeta->DoesPropertyExist ( kXMP_NS_XMP_MM, "InstanceID" );

	if ( ! haveInstID ) {
		if ( this->nextInstanceID.empty() ) CreateID ( "xmp.iid:", &this->nextInstanceID );
		idStr = this->nextInstanceID;
		xmpMeta->SetProperty ( kXMP_NS_XMP_MM, "InstanceID", idStr.c_str(), kXMP_DeleteExisting );
		idStr[4] = 'd';	// Change the prefix to "xmp.did" for further use.
		madeNewIDs = true;
	}
	
	bool haveDocID   = true;	// Assume we have it if ignored.
	bool ignoreDocID = XMP_OptionIsSet ( options, kXMPDocOps_IgnoreDocumentID );
	if ( ! ignoreDocID ) haveDocID = xmpMeta->DoesPropertyExist ( kXMP_NS_XMP_MM, "DocumentID" );

	if ( ! haveDocID ) {
		if ( idStr.empty() ) CreateID ( "xmp.did:", &idStr );
		idPtr = idStr.c_str();
		xmpMeta->SetProperty ( kXMP_NS_XMP_MM, "DocumentID", idPtr, kXMP_DeleteExisting );
		madeNewIDs = true;
	}

	bool haveOrigDocID   = true;	// Assume we have it if ignored.
	bool ignoreOrigDocID = ignoreDocID | XMP_OptionIsSet ( options, kXMPDocOps_IgnoreOriginalDocumentID );
	if ( ! ignoreOrigDocID ) haveOrigDocID = xmpMeta->DoesPropertyExist ( kXMP_NS_XMP_MM, "OriginalDocumentID" );

	if ( ! haveOrigDocID ) {
		if ( idPtr == 0 ) {
			haveDocID = xmpMeta->GetProperty ( kXMP_NS_XMP_MM, "DocumentID", &idPtr, &voidLen, &voidOptions );
			XMP_Assert ( haveDocID );	// Earlier checks should guarantee this.
			if ( ! haveDocID ) return madeNewIDs;	// Release build safety outlet.
		}
		xmpMeta->SetProperty ( kXMP_NS_XMP_MM, "OriginalDocumentID", idPtr, kXMP_DeleteExisting );
		madeNewIDs = true;
	}
	
	if ( madeNewIDs ) {
		this->isDirty = true;
		this->dirtyReasons |= (kXMPDirtyDoc_NewIDs | kXMPDirtyDoc_MetadataChanged);
	}
	return madeNewIDs;

}	// EnsureIDsExist

// -------------------------------------------------------------------------------------------------
// IsDirty
// -------

bool
XMPDocOps::IsDirty ( XMP_OptionBits * reasons /* = 0 */ ) const
{

	if ( reasons != 0 ) *reasons = this->dirtyReasons;
	return this->isDirty;

}	// IsDirty

// -------------------------------------------------------------------------------------------------
// Clone
// -----

void
XMPDocOps::Clone ( XMPDocOps * clone, XMP_OptionBits options ) const
{
	IgnoreParam(options);
	
	if ( clone == 0 ) XMP_Throw ( "Null clone pointer", kXMPErr_BadParam );
	if ( options != 0 ) XMP_Throw ( "No options are defined yet", kXMPErr_BadOptions );

	XMP_Assert ( clone->docXMP == 0 );
	if ( this->docXMP != 0 ) {
		bool isCloned = false;

#if AdobePrivate
	#if ENABLE_CPP_DOM_MODEL

		if (dynamic_cast<XMPMeta2 *> (this->docXMP)) {
			clone->docXMP = new XMPMeta2;
			isCloned = true;
		}
	#endif
#endif
		if(!isCloned) {
			clone->docXMP = new XMPMeta;
		}
		++clone->docXMP->clientRefs;	// ! Ensure ownership, thread safe because it is new.
		this->docXMP->Clone ( clone->docXMP, 0 );
	}
	
	clone->isNew = this->isNew;	// ! Don't copy everything, must not copy clientRefs and lock!
	clone->isDirty = this->isDirty;
	clone->isDerived = this->isDerived;
	clone->contentChanged = this->contentChanged;

	clone->dirtyReasons = this->dirtyReasons;

	clone->prevMIMEType = this->prevMIMEType;
	clone->prevFilePath = this->prevFilePath;
	clone->nextInstanceID = this->nextInstanceID;

	clone->changedParts = this->changedParts;
	
}	// Clone

// -------------------------------------------------------------------------------------------------
// GetMetaRef
// ----------

XMPMetaRef
XMPDocOps::GetMetaRef() const
{

	return (XMPMetaRef)this->docXMP;

}	// GetMetaRef

// =================================================================================================
