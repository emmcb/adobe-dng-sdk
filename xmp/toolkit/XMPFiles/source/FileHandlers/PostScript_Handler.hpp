#ifndef __PostScript_Handler_hpp__
#define __PostScript_Handler_hpp__	1

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
//  IJS Inder Jeet Singh
//
// mm-dd-yy who Description of changes, most recent on top
//
// 10-09-12 IJS 5.5-f043 Reverting IOBuffer Changes because of perf issues in large PS files.
// 10-03-12 IJS 5.5-f040 Remove IOBuffer from PostScript Handler.
// 09-20-12 IJS 5.5-f035 Add Progress Notification flag to MPEG-4,PSD,AIFF,WAVE,ASF,TIFF,FLV and PS Handlers.
// 08-13-12 IJS 5.5-f023 Added support to write(expand), inject and reconcile metadata in PS/EPS files.
//
// 08-17-10 AWL 5.3-f001 Integrate I/O revamp to main.
//
// 04-07-06 AWL 4.0-f002 Add XMPFiles::GetThumbnail. Change XMPFiles::OpenFile to close the disk file
//				early for read-only access. Change XMPFileHandler::ExtractXMP to CacheFileData.
// 03-24-06 AWL 4.0-f001 Adapt for move to ham-perforce, integrate XMPFiles, bump version to 4.
//
// 09-01-05 AWL 1.3-003 [1225858] Don't implicitly set the SafeUpdate flag for non-owning handlers.
//
// 04-13-05 AWL 1.1-052 Add tweaks to the CheckProc needed by the MDKit-based JPEG/TIFF/PSD handler.
//
// 01-06-05 AWL 1.0-027 [1014854] Cleanup handling for UTF-16 and UTF-32. This involves some general
//				changes to the separation of work between the common code and handlers, making the
//				storage of the SXMPMeta object and packet string more visible.
//
// 08-19-04 AWL First draft.
//
// =================================================================================================
#endif // AdobePrivate

#include "public/include/XMP_Environment.h"	// ! XMP_Environment.h must be the first included header.

#include "public/include/XMP_Const.h"
#include "public/include/XMP_IO.hpp"

#include "XMPFiles/source/FormatSupport/PostScript_Support.hpp"



// =================================================================================================
/// \file PostScript_Handler.hpp
/// \brief File format handler for PostScript and EPS files.
///
/// This header ...
///
// =================================================================================================

extern XMPFileHandler * PostScript_MetaHandlerCTor ( XMPFiles * parent );

extern bool PostScript_CheckFormat ( XMP_FileFormat format,
									 XMP_StringPtr  filePath,
			                         XMP_IO *       fileRef,
			                         XMPFiles *     parent );

static const XMP_OptionBits kPostScript_HandlerFlags = (
		kXMPFiles_CanInjectXMP
		|kXMPFiles_CanExpand
		|kXMPFiles_CanRewrite
		|kXMPFiles_PrefersInPlace
		|kXMPFiles_CanReconcile
		|kXMPFiles_AllowsOnlyXMP
		|kXMPFiles_ReturnsRawPacket
		|kXMPFiles_AllowsSafeUpdate 
		|kXMPFiles_CanNotifyProgress );

class PostScript_MetaHandler : public XMPFileHandler
{
public:

	PostScript_MetaHandler ( XMPFiles * parent );
	~PostScript_MetaHandler();

	void CacheFileData(); 
	void UpdateFile ( bool doSafeUpdate );
	void ProcessXMP ( );
	void WriteTempFile ( XMP_IO* tempRef );

	int psHint;
	/* Structure used to keep
		Track of Tokens in 
		EPS files
	*/
	struct TokenLocation{
		//offset from the begining of the file 
		// at which the token string starts
		XMP_Int64 offsetStart;
		//Total length of the token string
		XMP_Int64 tokenlen;
		TokenLocation():offsetStart(-1),tokenlen(0)
		{}
	};
protected:
	//Determines the postscript hint in the DSC comments
	int FindPostScriptHint();

	// Helper methods to get the First or the Last packet from the 
	// PS file based upon the PostScript hint that is present in the PS file
	bool FindFirstPacket();
	bool FindLastPacket();

	
	//Facilitates read time reconciliation of PS native metadata
	void ReconcileXMP( const std::string &xmpStr, std::string *outStr );

	//Facilitates reading of XMP packet , if one exists
	void ReadXMPPacket ( std::string & xmpPacket);

	// Parses the PS file to record th epresence and location of 
	// XMP packet and native metadata in the file
	void ParsePSFile();

	// Helper function to record the native metadata key/avlue pairs 
	// when parsing the PS file
	void RegisterKeyValue(std::string& key, std::string& value);

	// Helper Function to record the location and length of the Tokens
	// in the opened PS file
	void setTokenInfo(TokenFlag tFlag,XMP_Int64 offset,XMP_Int64 length);

	// Getter to get the location of a token ina PS file.
	TokenLocation& getTokenInfo(TokenFlag tFlag);

	//modifies the Binary Header of a PS file as per the modifications
	void modifyHeader(XMP_IO* fileRef,XMP_Int64 extrabytes,XMP_Int64 offset );

	//Extract the values for different DSC comments
	bool ExtractDSCCommentValue(IOBuffer &ioBuf,NativeMetadataIndex index);

	//Extract value for ADO_ContainsXMP Comment
	bool ExtractContainsXMPHint(IOBuffer &ioBuf,XMP_Int64 containsXMPStartpos);
	
	//Extract values from DocInfo Dict
	bool ExtractDocInfoDict(IOBuffer &ioBuf);

	//Determine the update method to be used 
	UpdateMethod DetermineUpdateMethod(std::string & outStr);
	void DetermineInsertionOffsets(XMP_Int64& ADOhintOffset,XMP_Int64& InjectData1Offset,
							XMP_Int64& InjectData3Offset);
	//Different update methods
	void InplaceUpdate (std::string &outStr,XMP_IO* &tempRef, bool doSafeUpdate);
	void ExpandingSFDFilterUpdate (std::string &outStr,XMP_IO* &tempRef, bool doSafeUpdate );
	void InsertNewUpdate ( std::string &outStr,XMP_IO* &tempRef, bool doSafeUpdate );
private:
	//Flag tracks DSC comments 
	XMP_Uns32 dscFlags;
	//Flag tracks DOCINFO keys
	XMP_Uns32 docInfoFlags;
	//stores the native metadata values. Index values an enum var NativeMetadataIndex
	std::string nativeMeta[kPS_MaxNativeIndexValue];
	//all offsets are to the end of the comment after atleast one whitespace
	TokenLocation fileTokenInfo[25];
	//Indicates the presence of both XMP hint and XMP
	bool containsXMPHint;
	//Keeps track whether a PS or EPS
	XMP_FileFormat fileformat;
	//keep the first packet info
	XMP_PacketInfo firstPacketInfo;	
	//keep the last packet info
	XMP_PacketInfo lastPacketInfo;	

};	// PostScript_MetaHandler

// =================================================================================================

#endif /* __PostScript_Handler_hpp__ */
