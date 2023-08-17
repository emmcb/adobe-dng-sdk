#ifndef __XMPUtils_hpp__
#define __XMPUtils_hpp__

// =================================================================================================
// Copyright 2003 Adobe
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
//  AWL Alan Lillich
//  ADC Amandeep Chawla
//  AJ  Abhishek Jindal
//
// mm-dd-yy who Description of changes, most recent on top.
//
// 03-27-15 AJ  5.6-c068 Checking in complete implementation of XMPUtils with the new Core APIs
// 03-26-15 AJ  5.6-c065 Disabling the code for linux for integrating into green as shared_ptr is not available on gcc 4.4.4
// 03-16-15 AJ  5.6-c047 Completing Rework of Get/SetBulkMarkers()
// 12-03-15 AJ  5.6-c045 Reworking glue code for Get/SetBulkMarkers()
// 09-03-15 AJ  5.6-c044 Get/SetBulkMarkers version2 with the new DOM APIs and some bug fixes for the same
// 03-02-15 AJ  5.6-c039 Marker Extensibility - Backward compatibility for extensions in GetBulkMarkers() and SetBulkMarkers()
// 12-21-14 AJ  5.6-c031 Routed XMPDocOps to new DOM APIs.
// 07-10-14 ADC 5.6-c015 Refactoring, partial documentation and bug fixes of XMPCommon and XMPCore C++ APIs.
//
// 06-26-13 ADC 5.5-c019 [3083403] Failure to open a PDF file.
//						 GetProperty_Int, GetProperty_Int64 and GetProperrty_Float will tolerate presence of
//						 whitespace characters at the beginning and end.
//
// 07-21-09 AWL 5.0-c047 Add XMPUtils::ApplyTemplate.
// 06-11-09 AWL 5.0-c034 Finish threading revamp, implement friendly reader/writer locking.
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
// 11-02-07 AWL 4.2-c024 Start implementing XMPDocOps.
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
//
// 08-18-04 AWL 3.1-075 Add HasContainedDoc.
//
// 04-30-04 AWL Add new & delete operators that call BIBMemory functions. Change static objects that
//				require allocation to explicit pointers.
// 03-29-04 AWL Add Q&D versions of GetMainPacket and UpdateMainPacket.
// 02-17-04 AWL Add multi-file utilities. Rename CreateXyzDocument to InitializeXyzDocument.
// 02-09-04 AWL Start adding the new Document Operation utilities.
// 05-03-03 AWL Initial start on the new implementation.
//
// =================================================================================================
#endif /* AdobePrivate */

#include "public/include/XMP_Environment.h"
#include "public/include/XMP_Const.h"

#include "XMPCore/source/XMPMeta.hpp"
#include "XMPCore/source/XMPCore_Impl.hpp"
#include "public/include/client-glue/WXMPUtils.hpp"
#include "XMPCore/XMPCoreDefines.h"

#if ENABLE_CPP_DOM_MODEL
#include "XMPCommon/Interfaces/IError.h"
#include "XMPCore/XMPCoreFwdDeclarations.h"
#include "XMPCore/source/XMPMeta2.hpp"
#endif

#include "XMPCore/source/XMPCore_Impl.hpp"
#include "source/XMLParserAdapter.hpp"
#include "XMPCore/source/XMPMeta.hpp"
#include "third-party/zuid/interfaces/MD5.h"



bool
IsInternalProperty(const XMP_VarString & schema,
				   const XMP_VarString & prop);
#if AdobePrivate
bool
CheckSpecialProperty(XMP_VarString propName,
					 XMP_StringPtr specialNames[]);
#endif


// -------------------------------------------------------------------------------------------------

class	XMPUtils {
public:

	static bool
		Initialize();	// ! For internal use only!

	static void
		Terminate() RELEASE_NO_THROW;	// ! For internal use only!

	// ---------------------------------------------------------------------------------------------	

#if ENABLE_CPP_DOM_MODEL
	static void SetNode(const AdobeXMPCore::spINode & node, XMP_StringPtr value, XMP_OptionBits options);
	static XMP_OptionBits ConvertNewArrayFormToOldArrayForm(const AdobeXMPCore::spcIArrayNode & arrayNode);
	static AdobeXMPCore::spINode CreateArrayChildNode(const AdobeXMPCore::spIArrayNode & arrayNode, XMP_OptionBits options);
	static void DoSetArrayItem(const AdobeXMPCore::spIArrayNode & arrayNode, XMP_Index itemIndex, XMP_StringPtr itemValue, XMP_OptionBits options);
	static void SetImplicitNodeInformation(bool & firstImplicitNodeFound, AdobeXMPCore::spINode & implicitNodeRoot, AdobeXMPCore::spINode & destNode,
		XMP_Index & implicitNodeIndex, XMP_Index index = 0);
	static void GetNameSpaceAndNameFromStepValue(const XMP_VarString & stepStr, const AdobeXMPCore::spcINameSpacePrefixMap & defaultMap,
		XMP_VarString & stepNameSpace, XMP_VarString & stepName);
	static bool FindNode(const AdobeXMPCore::spIMetadata & mDOM, XMP_ExpandedXPath & expPath, bool createNodes, XMP_OptionBits leafOptions,
		AdobeXMPCore::spINode & retNode, XMP_Index * nodeIndex = 0, bool ignoreLastStep = 0);
	static bool FindCnstNode(const AdobeXMPCore::spIMetadata & mDOM, XMP_ExpandedXPath & expPath, AdobeXMPCore::spINode & destNode, XMP_OptionBits * options = 0,
		XMP_Index * arrayIndex = 0);
	static AdobeXMPCore::spINode FindChildNode(const AdobeXMPCore::spINode & parent, XMP_StringPtr childName, XMP_StringPtr childNameSpace, bool createNodes, size_t *	pos /* = 0 */);
	static size_t GetNodeChildCount(const AdobeXMPCore::spcINode & node);
	static AdobeXMPCore::spcINodeIterator GetNodeChildIterator(const AdobeXMPCore::spcINode & node);
	static std::vector< AdobeXMPCore::spcINode > GetChildVector(const AdobeXMPCore::spINode & node);
	static XMP_OptionBits GetIXMPOptions(const AdobeXMPCore::spcINode & node);
	static bool HandleConstAliasStep(const AdobeXMPCore::spIMetadata & mDOM, AdobeXMPCore::spINode & destNode, const XMP_ExpandedXPath & expandedXPath,
		XMP_Index  * nodeIndex = 0);
	static bool HandleAliasStep(const AdobeXMPCore::spIMetadata &  mDOM, XMP_ExpandedXPath & expandedXPath, bool createNodes, XMP_OptionBits leafOptions,
		AdobeXMPCore::spINode & destNode, XMP_Index * nodeIndex, bool ignoreLastStep);
	static AdobeXMPCommon::spcIUTF8String GetNodeValue(const AdobeXMPCore::spINode & node);
	static XMP_Index LookupFieldSelector_v2(const AdobeXMPCore::spIArrayNode & arrayNode, XMP_VarString fieldName, XMP_VarString fieldValue);
	static AdobeXMPCore::spINode CreateTerminalNode(const char* nameSpace, const char * name, XMP_OptionBits options);

#endif


	static void
		ComposeArrayItemPath(XMP_StringPtr   schemaNS,
		XMP_StringPtr   arrayName,
		XMP_Index	   itemIndex,
		XMP_VarString * fullPath);

	static void
		ComposeStructFieldPath(XMP_StringPtr	 schemaNS,
		XMP_StringPtr	 structName,
		XMP_StringPtr	 fieldNS,
		XMP_StringPtr	 fieldName,
		XMP_VarString * fullPath);

	static void
		ComposeQualifierPath(XMP_StringPtr   schemaNS,
		XMP_StringPtr   propName,
		XMP_StringPtr   qualNS,
		XMP_StringPtr   qualName,
		XMP_VarString * fullPath);

	static void
		ComposeLangSelector(XMP_StringPtr	  schemaNS,
		XMP_StringPtr	  arrayName,
		XMP_StringPtr	  langName,
		XMP_VarString * fullPath);

	static void
		ComposeFieldSelector(XMP_StringPtr   schemaNS,
		XMP_StringPtr   arrayName,
		XMP_StringPtr   fieldNS,
		XMP_StringPtr   fieldName,
		XMP_StringPtr   fieldValue,
		XMP_VarString * fullPath);

	// ---------------------------------------------------------------------------------------------

	static void
		ConvertFromBool(bool			  binValue,
		XMP_VarString * strValue);

	static void
		ConvertFromInt(XMP_Int32		 binValue,
		XMP_StringPtr	 format,
		XMP_VarString * strValue);

	static void
		ConvertFromInt64(XMP_Int64	   binValue,
		XMP_StringPtr   format,
		XMP_VarString * strValue);

	static void
		ConvertFromFloat(double		   binValue,
		XMP_StringPtr   format,
		XMP_VarString * strValue);

	static void
		ConvertFromDate(const XMP_DateTime & binValue,
		XMP_VarString *	   strValue);

	// ---------------------------------------------------------------------------------------------

	static bool
		ConvertToBool(XMP_StringPtr strValue);

	static XMP_Int32
		ConvertToInt(XMP_StringPtr strValue);

	static XMP_Int64
		ConvertToInt64(XMP_StringPtr strValue);

	static double
		ConvertToFloat(XMP_StringPtr strValue);

	static void
		ConvertToDate(XMP_StringPtr	strValue,
		XMP_DateTime * binValue);

	// ---------------------------------------------------------------------------------------------

	static void
		CurrentDateTime(XMP_DateTime * time);

	static void
		SetTimeZone(XMP_DateTime * time);

	static void
		ConvertToUTCTime(XMP_DateTime * time);

	static void
		ConvertToLocalTime(XMP_DateTime * time);

	static int
		CompareDateTime(const XMP_DateTime & left,
		const XMP_DateTime & right);
	// ---------------------------------------------------------------------------------------------

	static void
		EncodeToBase64(XMP_StringPtr	 rawStr,
		XMP_StringLen	 rawLen,
		XMP_VarString * encodedStr);

	static void
		DecodeFromBase64(XMP_StringPtr   encodedStr,
		XMP_StringLen   encodedLen,
		XMP_VarString * rawStr);

	// ---------------------------------------------------------------------------------------------

	static void
		PackageForJPEG(const XMPMeta & xmpObj,
		XMP_VarString * stdStr,
		XMP_VarString * extStr,
		XMP_VarString * digestStr);


#if ENABLE_CPP_DOM_MODEL
	static void
		PackageForJPEG(const XMPMeta2 & xmpObj,
		XMP_VarString * stdStr,
		XMP_VarString * extStr,
		XMP_VarString * digestStr);
#endif
	static void
		MergeFromJPEG(XMPMeta *       fullXMP,
		const XMPMeta & extendedXMP);




	// ---------------------------------------------------------------------------------------------

	static void
		CatenateArrayItems(const XMPMeta & xmpObj,
		XMP_StringPtr	 schemaNS,
		XMP_StringPtr	 arrayName,
		XMP_StringPtr	 separator,
		XMP_StringPtr	 quotes,
		XMP_OptionBits	 options,
		XMP_VarString * catedStr);

	static void
		SeparateArrayItems(XMPMeta *		xmpObj,
		XMP_StringPtr	schemaNS,
		XMP_StringPtr	arrayName,
		XMP_OptionBits options,
		XMP_StringPtr	catedStr);

	static void
		ApplyTemplate(XMPMeta *	    workingXMP,
		const XMPMeta & templateXMP,
		XMP_OptionBits  actions);


	static void
		RemoveProperties(XMPMeta *	  xmpObj,
		XMP_StringPtr  schemaNS,
		XMP_StringPtr  propName,
		XMP_OptionBits options);


#if AdobePrivate

	static void
		AppendProperties(const XMPMeta & source,
		XMPMeta *	   dest,
		XMP_OptionBits  options);
#endif

	static void
		DuplicateSubtree(const XMPMeta & source,
		XMPMeta *	   dest,
		XMP_StringPtr   sourceNS,
		XMP_StringPtr   sourceRoot,
		XMP_StringPtr   destNS,
		XMP_StringPtr   destRoot,
		XMP_OptionBits  options);


#if AdobePrivate

	static void
		CollectMultiFileXMP(const XMPMeta & inputXMP,
		XMPMeta *       multiXMP,
		XMP_OptionBits  options);


	static void
		DistributeMultiFileXMP(const XMPMeta & multiXMP,
		XMPMeta *       outputXMP,
		XMP_OptionBits  options);


	static bool
		IsPropertyMultiValued(const XMPMeta & multiXMP,
		XMP_StringPtr   propNS,
		XMP_StringPtr   propName);

	static bool
		GetDateRange(const XMPMeta & multiXMP,
		XMP_StringPtr   propNS,
		XMP_StringPtr   propName,
		XMP_DateTime *  oldest,
		XMP_DateTime *  newest);


	static bool
		GetMergedListPath(const XMPMeta & multiXMP,
		XMP_StringPtr   propNS,
		XMP_StringPtr   propName,
		XMP_VarString * pathStr);



	static void
		RemoveMultiValueInfo(XMPMeta *     multiXMP,
		XMP_StringPtr propNS,
		XMP_StringPtr propName);

#endif	// AdobePrivate

#if AdobePrivate

	// ---------------------------------------------------------------------------------------------

	static bool
		GetBulkMarkers(const XMPMeta & xmp,
		XMP_StringPtr   ns,
		XMP_StringPtr   path,
		void *          clientPtr,
		XMP_Uns32       clientElemSize,
		XMPDMO_AllocClientMarkersProc AllocClientMarkers,
		XMPDMO_AllocClientCuePointsProc  AllocClientCuePoints);

	static void
		SetBulkMarkers(XMPMeta *     xmp,
		XMP_StringPtr ns,
		XMP_StringPtr path,
		XMP_Index     count,
		const void *  clientArray,
		XMP_Uns32     clientElemSize,
		XMPDMO_GetCuePointParamsProc GetCuePointParams);

#if ENABLE_CPP_DOM_MODEL
	static bool
		GetBulkMarkers(const XMPMeta2 & xmp,
		XMP_StringPtr   ns,
		XMP_StringPtr   path,
		void *          clientPtr,
		XMP_Uns32       clientElemSize,
		XMPDMO_AllocClientMarkersProc AllocClientMarkers,
		XMPDMO_AllocClientCuePointsProc  AllocClientCuePoints);

	static void
		SetBulkMarkers(XMPMeta2 *     xmp,
		XMP_StringPtr ns,
		XMP_StringPtr path,
		XMP_Index     count,
		const void *  clientArray,
		XMP_Uns32     clientElemSize,
		XMPDMO_GetCuePointParamsProc GetCuePointParams);

#endif // ENABLE_CPP_DOM_MODEL

#endif	// AdobePrivate

	// ---------------------------------------------------------------------------------------------

	static std::string& Trim(std::string& string);

	static std::string * WhiteSpaceStrPtr;

#if ENABLE_CPP_DOM_MODEL
	static void MapXMPErrorToIError(XMP_Int32 xmpErrorCodes, AdobeXMPCommon::IError::eErrorDomain & domain, AdobeXMPCommon::IError::eErrorCode & code);
	static bool SerializeExtensionAsJSON(const AdobeXMPCore::spINode & extensionNode, std::string & key, std::string & value);
	static bool IsExtensionValidForBackwardCompatibility(const AdobeXMPCore::spINode & extensionNode);
	static bool CreateExtensionNode(const AdobeXMPCore::spIStructureNode & xmpNode, const XMP_VarString & serializedJSON, const XMP_VarString & doubleQuotesStr);


	static void
		CatenateArrayItems_v2(const XMPMeta & xmpObj,
		XMP_StringPtr	 schemaNS,
		XMP_StringPtr	 arrayName,
		XMP_StringPtr	 separator,
		XMP_StringPtr	 quotes,
		XMP_OptionBits	 options,
		XMP_VarString * catedStr);
	static void
		SeparateArrayItems_v2(XMPMeta *		xmpObj,
		XMP_StringPtr	schemaNS,
		XMP_StringPtr	arrayName,
		XMP_OptionBits options,
		XMP_StringPtr	catedStr);

	static void
		RemoveProperties_v2(XMPMeta *		xmpMetaPtr,
		XMP_StringPtr	schemaNS,
		XMP_StringPtr	propName,
		XMP_OptionBits options);
	
	static void
		ApplyTemplate_v2(XMPMeta *	      workingXMP,
		const XMPMeta & templateXMP,
		XMP_OptionBits  actions);

	static void
		DuplicateSubtree_v2(const XMPMeta & source,
		XMPMeta *		 dest,
		XMP_StringPtr	 sourceNS,
		XMP_StringPtr	 sourceRoot,
		XMP_StringPtr	 destNS,
		XMP_StringPtr	 destRoot,
		XMP_OptionBits	 options);

#if AdobePrivate
	static void
		AppendProperties_v2(const XMPMeta & sourceptr,
		XMPMeta *		 destPtr,
		XMP_OptionBits	 options);

	static void
		CollectMultiFileXMP_v2(const XMPMeta & inputXMP,
		XMPMeta *       multiXMP,
		XMP_OptionBits  options);

	static void
		DistributeMultiFileXMP_v2(const XMPMeta & multiXMP,
		XMPMeta *       outputXMP,
		XMP_OptionBits  options);

	static bool
		IsPropertyMultiValued_v2(const XMPMeta & multiXMP,
		XMP_StringPtr   propNS,
		XMP_StringPtr   propName);

	static bool
		GetDateRange_v2(const XMPMeta & multiXMP,
		XMP_StringPtr   propNS,
		XMP_StringPtr   propName,
		XMP_DateTime *  oldest,
		XMP_DateTime *  newest);

	static bool
		GetMergedListPath_v2(const XMPMeta & multiXMP,
		XMP_StringPtr   propNS,
		XMP_StringPtr   propName,
		XMP_VarString * pathStr);
	static void
		RemoveMultiValueInfo_v2(XMPMeta *     multiXMP,
		XMP_StringPtr propNS,
		XMP_StringPtr propName);

#endif //AdobePrivate
#endif

	static bool CreateExtensionNode(XMP_Node ** xmpNode, const XMP_VarString & serializedJSON, const XMP_VarString & doubleQuotesString);

	static bool GetSerializedJSONForExtensionNode(const XMP_Node * xmpNode, XMP_VarString &extensionAsKey, XMP_VarString & serializedJSON);

	static bool IsSuitableForJSONSerialization(const XMP_Node * xmpNode);

};	// XMPUtils

// =================================================================================================

#endif	// __XMPUtils_hpp__
