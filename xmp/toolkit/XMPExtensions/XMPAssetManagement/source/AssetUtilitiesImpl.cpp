// =================================================================================================
// ADOBE SYSTEMS INCORPORATED
// Copyright 2014 Adobe Systems Incorporated
// All Rights Reserved
//
// NOTICE:  Adobe permits you to use, modify, and distribute this file in accordance with the terms
// of the Adobe license agreement accompanying it.
// =================================================================================================

#if AdobePrivate
// =================================================================================================
// Change history
// ==============
//
// Writers:
//	SKP Sunil Kishor Pathak
//  ADC	Amandeep Chawla
//
// mm-dd-yy who Description of changes, most recent first.
//
// 03-05-15 SKP 1.0-a076 Bug: 3947546, 3947499 & 3947421. Refactored code of RemoveNode and added
//						 internal function RemoveEmptyNode() to remove DocumentID, InstanceID &
//						 OriginalDocumentID if these are present but are empty and client called
//						 MakeTrackable()
// 02-26-15 SKP 1.0-a069 Fixed build break on Mac.
// 02-26-15 SKP 1.0-a067 Bug: 3945356 Istrackable should return false in case InstanceID(any 
//						 trackableID) present is empty while it is returning true.
// 02-19-15 SKP 1.0-a058 Fixed API test cases 
//						 IAssetUtilitiesTests::OriginalDocumentId_DerivedFrom_Present & some more.
// 02-19-15 SKP 1.0-a057 Bug : 3937072 CreateComposedAssetManger creates its object if the document
//						 to be composed is not trackable while it should throw exception in this case
// 02-19-15 SKP 1.0-a056 Fixed Allocator, ErrorNotification and threading handling in AssetConfigMgr
// 02-18-15 SKP 1.0-a055 Fixed testcase : IAssetUtilitiesTests::NoTrackingID_HistoryPresent()
// 02-18-15 SKP 1.0-a054 Fixed bug: 3921227 & 3920761 Setting correct document ID if Original-
//						 DocumentID is present with/without DerivedFrom and with/without History
// 02-16-15 SKP 1.0-a053 Bug: 3920761 When OriginalDocumentID and DerivedFrom is present , 
//						 MakeTrackable copying OriginalDocumentID into DocumentID , while it should
//						 create new DocumentID
// 02-11-15 SKP 1.0-a049 Added initial support for client strategies. Using these strategies client
//						 can set prefix, AppName, history update policy and it's own datetime
//						 function which will be used to get time for ModifyDate, Metadata date etc.
// 01-16-15 SKP 1.0-a048 Fixed the logic error to fix issue in creating a document trackable.
// 12-10-14 SKP 1.0-a046 Fixed crash in MakeTrackable() and fix typo error.
// 12-12-14 SKP 1.0-a045 MakeTrakable() was not inserting OriginalDocumentID in case if 
//						 OriginalDocumentID is not present but DerivedFrom is present 
//						 having documentID
// 12-11-14 SKP 1.0-a044 Fixed return value in IsTrackable, fixed issues in RemoveComponent,
//						 Added GetIngredientNode() to return the reference of node within the
//						 the relationship to the ingredient entry.
// 12-10-14 SKP 1.0-a043 Updated instance id in MakeTrackable().
// 12-10-14 ADC 1.0-a042 Fixed crash in MakeTrackable() in case when DocumentID is present but
//						 OriginalDocumentID is missing.
// 12-05-14 SKP 1.0-a038 Changed API name from IsTrackAble() to IsTrackable() and from 
//						 MakeTrackAble() to MakeTrackable()
// 11-27-14 SKP 1.0-a017 Implemented Asset composition related functionality for basic part addition.
// 11-26-14 SKP 1.0-a012 Made changed to build AML on Mac cl:157148 
// 11-21-14 SKP 1.0-a003 Added check of metadata for NULL and throw exception for NULL metadata
// 11-19-14 ADC 1.0-a001 Framework in place for Asset Management Library.
//
// =================================================================================================
#endif  // AdobePrivate


#define IMPLEMENTATION_HEADERS_CAN_BE_INCLUDED 1
	#include "XMPAssetManagement/ImplHeaders/AssetUtilitiesImpl.h"
#undef IMPLEMENTATION_HEADERS_CAN_BE_INCLUDED

#include "XMPCommon/Interfaces/IErrorNotifier_I.h"

#include "XMPCommon/Interfaces/IError_I.h"
#include "XMPCommon/XMPCommonErrorCodes.h"
#include "XMPCommon/Interfaces/IUTF8String_I.h"
#include "XMPCommon/Utilities/UTF8String.h"
#include "XMPCommon/Utilities/TSmartPointers_I.h"
#include "XMPCore/Interfaces/IMetadata.h"
#include "XMPCore/Interfaces/ISimpleNode.h"
#include "XMPCore/Interfaces/IArrayNode.h"

#define TXMP_STRING_TYPE std::string 
// Ensure XMP templates are instantiated

// Provide access to the API
#include "public/include/XMP.hpp"
#include "XMPCore/source/XMPDocOps-Utils.hpp"

#define IMPLEMENTATION_HEADERS_CAN_BE_INCLUDED 1
#include "XMPExtensions/include//ExtensionsConfigurationManagerImpl.h"
#undef IMPLEMENTATION_HEADERS_CAN_BE_INCLUDED


namespace AdobeXMPAM_Int {

	//definition of class methods
	bool APICALL AssetUtilitiesImpl::IsTrackable(const spcIMetadata & metadata) {
		if (metadata) {
			spcINode node = metadata->GetNode(kXMP_NS_XMP_MM, AdobeXMPCommon::npos,"OriginalDocumentID", AdobeXMPCommon::npos);
			if (!node || node->ConvertToSimpleNode()->GetValue()->empty()) return false;
			
			node = metadata->GetNode(kXMP_NS_XMP_MM, AdobeXMPCommon::npos, "DocumentID", AdobeXMPCommon::npos);
			if (!node || node->ConvertToSimpleNode()->GetValue()->empty()) return false;

			node = metadata->GetNode(kXMP_NS_XMP_MM, AdobeXMPCommon::npos, "InstanceID", AdobeXMPCommon::npos);
			if (!node || node->ConvertToSimpleNode()->GetValue()->empty()) return false;
		} else {
			return false;
		}
		return true;
	}

	/*
	**	If any of the Ids (OriginalDocumentID, DocumentID or InstanceID) is missing, document will not be trackable.
	**
	**	To mae a document trackable, we need to embed all these three Ids or the IDs which are not present. Before
	**	generating the new Ids, first try to get the IDs from either DerivedFrom (which has documentId and InstanceID)
	**	and history entry which can have instance IDs. Based on the above following are the strategies
	**
	**	History	OrgDocumentID	DocumentID	InstanceID	DerivedFrom		Strategy
	**	N		N				N			Y/N			N			Create new tracking IDs for the doc and add a history entry
	**  N		N				N			Y/N			Y			Pick up Original Document ID from DerivedFrom attribute. Also
	**																	use this as the Document ID. Update Instance ID and add History entry.
	**	N		N				Y			Y/N			N			Use the Document ID as the Original Document ID too. Update Instance ID and add History entry.
	**	N		N				Y			Y/N			Y			Pick DocumentID from DerivedFrom and set as OriginalDocumentID
	**	N		Y				N			Y/N			N			Use the OriginalDocumentID as DocumentID and update Instance ID
	**	N		Y				N			Y/N			Y			Create new document id;
	**	N		Y				Y			Y/N			Y			Update the Instance ID and add into history
	**	Y		N				N			Y/N			N			Check History. Pick up the oldest Instance ID (oldest "when" which is previous to the current
	**																metadata date) as the  Original Document ID and Document ID. Update Instance ID and History.
	**	Y		N				N			Y/N			Y			Pick DocumentID from DF and set it in OrgDocumentID, DocumentID update InstanceID and history
	**	Y		N				Y			Y/N			Y			Pick DocumentID from DF and set it in OrgDocumentID update InstanceID and history
	**	Y		Y				N			Y/N			Y			Use the oldest Instance ID from the History entry as the Document ID. Update the Instance ID and History.
	**	Y		Y				Y			Y/N			N			Update Instance ID (if it missing) and History Otherwise do nothing.
	**	Y		Y				Y			Y/N			Y			Update Instance ID (if it missing) and History Otherwise do nothing.
	**	
	**
	**	Summarising the above table:
	**		If Original DocumentID is missing  & DF is present: Pick the document entry from the DerivedFrom and set it as Original Document ID
	**		If there is history, pick the instance id prior to CreateDate and set it as documentID and Original Document ID
	**		If OriginalDocumentID & DocumentID are present but InstanceID is missing, create and set it
	*/
	bool APICALL AssetUtilitiesImpl::MakeTrackable( const spIMetadata & metadata ) {
		//if metadata is NULL; raise exception
		if ( !metadata )
			NOTIFY_ERROR(IError_base::kEDGeneral, kGECParametersNotAsExpected,
				"metadata is NULL", IError_base::kESOperationFatal, false, false);

		if (IsTrackable(metadata)) return true;

		spIStructureNode ixmpstruct = metadata->ConvertToStructureNode();
		
		//bug : 3947546 Make Trackable should add document id if document id present in metadata is empty.
		AssetUtilitiesImpl aui;
		RemoveEmptyNode(ixmpstruct, kXMP_NS_XMP_MM, "OriginalDocumentID");
		RemoveEmptyNode(ixmpstruct, kXMP_NS_XMP_MM, "DocumentID");
		RemoveEmptyNode(ixmpstruct, kXMP_NS_XMP_MM, "InstanceID");

		//First set the Original document id
		spISimpleNode documentIDNode, instanceIDNode;
		bool InitiallyOriginalDocumentIDPresent = false;
		
		spISimpleNode orgdocidnode = ixmpstruct->GetSimpleNode(kXMP_NS_XMP_MM, AdobeXMPCommon::npos, "OriginalDocumentID", AdobeXMPCommon::npos);
		if (orgdocidnode) {
			InitiallyOriginalDocumentIDPresent = true;
		}

		if ( orgdocidnode.get() == NULL) {
			spIStructureNode derivedfromStruct = ixmpstruct->GetStructureNode(kXMP_NS_XMP_MM, AdobeXMPCommon::npos, "DerivedFrom", AdobeXMPCommon::npos);
			if (derivedfromStruct) {
				spISimpleNode derivedDocId = derivedfromStruct->GetSimpleNode(kXMP_NS_XMP_ResourceRef, AdobeXMPCommon::npos, "documentID", AdobeXMPCommon::npos);
				if (derivedDocId) {
					orgdocidnode = ISimpleNode::CreateSimpleNode(kXMP_NS_XMP_MM, AdobeXMPCommon::npos, "OriginalDocumentID", AdobeXMPCommon::npos, derivedDocId->GetValue()->c_str(), derivedDocId->GetValue()->size());
					metadata->InsertNode(orgdocidnode);
				}
			}
		}

		//if OriginalDocumentID not found from derivedfrom's documentid; check for document id
		if (orgdocidnode.get() == NULL) {
			documentIDNode = ixmpstruct->GetSimpleNode(kXMP_NS_XMP_MM, AdobeXMPCommon::npos, "DocumentID", AdobeXMPCommon::npos);
			if (documentIDNode.get() != NULL) {
				orgdocidnode = ISimpleNode::CreateSimpleNode(kXMP_NS_XMP_MM, AdobeXMPCommon::npos, "OriginalDocumentID", AdobeXMPCommon::npos, documentIDNode->GetValue()->c_str(), documentIDNode->GetValue()->size());
				metadata->InsertNode(orgdocidnode);
			}
		}

		if (orgdocidnode.get() == NULL) { //OriginalDocumentID not found; docID too not found, check from history and if found update both
			std::string historyEntryInstanceID;
			bool InstanceIdInhistoryPresent = GetOldestHistoryInstanceID(ixmpstruct, historyEntryInstanceID);
			
			if (InstanceIdInhistoryPresent) {
				std::string prefix = "xmp.iid";
				if (historyEntryInstanceID.compare(0, prefix.size(), prefix) == 0)
					historyEntryInstanceID[4] = 'd';
				orgdocidnode = ISimpleNode::CreateSimpleNode(kXMP_NS_XMP_MM, AdobeXMPCommon::npos, "OriginalDocumentID", AdobeXMPCommon::npos, historyEntryInstanceID.c_str(), historyEntryInstanceID.size());
				metadata->InsertNode(orgdocidnode);
			}
		}

		//instance ID : create a new one
		std::string NewInstanceIDPrefix = ITSingleton< IExtensionsConfigurationManager_I >::GetInstance()->GetPrefix(IExtensionsConfigurationManager_v1::kInstanceIDPrefix);
		std::string NewInstanceID;
		SXMPDocOps::CreateID(/*"xmp.iid:"*/ &NewInstanceID, NewInstanceIDPrefix.c_str());

		documentIDNode = ixmpstruct->GetSimpleNode(kXMP_NS_XMP_MM, AdobeXMPCommon::npos, "DocumentID", AdobeXMPCommon::npos);
		if ((documentIDNode.get() == NULL) && (orgdocidnode.get() != NULL)) {
			//check for xmpMM:DerivedFrom; if present create new document id; else make document id as OriginalDocumentID
			if (InitiallyOriginalDocumentIDPresent) {
				spIStructureNode derivedfromStruct = ixmpstruct->GetStructureNode(kXMP_NS_XMP_MM, AdobeXMPCommon::npos, "DerivedFrom", AdobeXMPCommon::npos);
				if (derivedfromStruct) {
					std::string historyOldestInstanceId;
					spISimpleNode node;
					if (GetOldestHistoryInstanceID(metadata, historyOldestInstanceId)) {
						std::string prefix = "xmp.iid";
						if (historyOldestInstanceId.compare(0, prefix.size(), prefix) == 0)
							historyOldestInstanceId[4] = 'd';
						node = ISimpleNode::CreateSimpleNode(kXMP_NS_XMP_MM, AdobeXMPCommon::npos, "DocumentID", AdobeXMPCommon::npos, historyOldestInstanceId.c_str(), historyOldestInstanceId.size());
					}
					else {
						node = ISimpleNode::CreateSimpleNode(kXMP_NS_XMP_MM, AdobeXMPCommon::npos, "DocumentID", AdobeXMPCommon::npos, NewInstanceID.c_str(), NewInstanceID.size());
					}
					ixmpstruct->InsertNode(node);
				}
				else {
					documentIDNode = ISimpleNode::CreateSimpleNode(kXMP_NS_XMP_MM, AdobeXMPCommon::npos, "DocumentID", AdobeXMPCommon::npos, orgdocidnode->GetValue()->c_str(), orgdocidnode->GetValue()->size());
					metadata->InsertNode(documentIDNode);
				}
			}
			else {
				documentIDNode = ISimpleNode::CreateSimpleNode(kXMP_NS_XMP_MM, AdobeXMPCommon::npos, "DocumentID", AdobeXMPCommon::npos, orgdocidnode->GetValue()->c_str(), orgdocidnode->GetValue()->size());
				metadata->InsertNode(documentIDNode);
			}
		}

		//if both OriginalDocumentID and DocumentID are NULL, set them as NewInstanceID
		if (orgdocidnode.get() == NULL) {
			//std::string NewDocumentID = NewInstanceID;
			std::string newOrgDocID = ITSingleton< IExtensionsConfigurationManager_I >::GetInstance()->GetPrefix(IExtensionsConfigurationManager_v1::kOriginalDocumentIDPrefix) + NewInstanceID.substr(NewInstanceIDPrefix.size());
			//NewDocumentID[4] = 'd';
			spISimpleNode node = ISimpleNode::CreateSimpleNode(kXMP_NS_XMP_MM, AdobeXMPCommon::npos, "OriginalDocumentID", AdobeXMPCommon::npos, newOrgDocID.c_str(), newOrgDocID.size());
			ixmpstruct->InsertNode(node);
			std::string newDocID = ITSingleton< IExtensionsConfigurationManager_I >::GetInstance()->GetPrefix(IExtensionsConfigurationManager_v1::kDocumentIDPrefix) + NewInstanceID.substr(NewInstanceIDPrefix.size());
			node = ISimpleNode::CreateSimpleNode(kXMP_NS_XMP_MM, AdobeXMPCommon::npos, "DocumentID", AdobeXMPCommon::npos, newDocID.c_str(), newDocID.size());
			ixmpstruct->InsertNode(node);
		}
		//update history entry; action:saved, instanceID:currentInstanceID, when:currenttime, changed:/metadata, softwareAgent:Adobe XMP AML Toolkit
		spINode node = ixmpstruct->GetNode(kXMP_NS_XMP_MM, AdobeXMPCommon::npos, "History", AdobeXMPCommon::npos);
		spIArrayNode historyArrayNode;
		if (node)
			historyArrayNode = node->ConvertToArrayNode();
		if (!historyArrayNode) {
			historyArrayNode = IArrayNode::CreateOrderedArrayNode(kXMP_NS_XMP_MM, AdobeXMPCommon::npos, "History", AdobeXMPCommon::npos);
			ixmpstruct->InsertNode(historyArrayNode);
		}
		ITSingleton< IExtensionsConfigurationManager_I >::GetInstance()->UpdateHistoryEntry(historyArrayNode);
		/*
		spIXMPStructureNode historyEntryNewNode = NS_XMPCOMMON::IXMPStructureNode::CreateStructureNode(kXMP_NS_XMP_MM, "History");
		spIXMPSimpleNode SimpleNodeForHistoryEntry = NS_XMPCOMMON::IXMPSimpleNode::CreateSimpleNode(kXMP_NS_XMP_ResourceEvent, "action", "saved");
		historyEntryNewNode->InsertNode(SimpleNodeForHistoryEntry);
		SimpleNodeForHistoryEntry = NS_XMPCOMMON::IXMPSimpleNode::CreateSimpleNode(kXMP_NS_XMP_ResourceEvent, "changed", "/metadata");
		historyEntryNewNode->InsertNode(SimpleNodeForHistoryEntry);
		SimpleNodeForHistoryEntry = NS_XMPCOMMON::IXMPSimpleNode::CreateSimpleNode(kXMP_NS_XMP_ResourceEvent, "softwareAgent", ExtensionsConfigurationManagerImpl::GetInstance()->GetAppName().c_str());
		historyEntryNewNode->InsertNode(SimpleNodeForHistoryEntry);
		SimpleNodeForHistoryEntry = NS_XMPCOMMON::IXMPSimpleNode::CreateSimpleNode(kXMP_NS_XMP_ResourceEvent, "instanceID", NewInstanceID.c_str());
		historyEntryNewNode->InsertNode(SimpleNodeForHistoryEntry);


		std::string dateStr;
		ExtensionsConfigurationManagerImpl::GetInstance()->GetDateTime(dateStr);
		SimpleNodeForHistoryEntry = NS_XMPCOMMON::IXMPSimpleNode::CreateSimpleNode(kXMP_NS_XMP_ResourceEvent, "when", dateStr.c_str());
		historyEntryNewNode->InsertNode(SimpleNodeForHistoryEntry);

		
		historyArrayNode->AppendNode(historyEntryNewNode);
		*/

		//update the document instance id
		spISimpleNode spInstanceIDNode = ixmpstruct->GetSimpleNode(kXMP_NS_XMP_MM, AdobeXMPCommon::npos, "InstanceID", AdobeXMPCommon::npos);
		if (spInstanceIDNode) {
			spInstanceIDNode->SetValue(NewInstanceID.c_str(), NewInstanceID.size());
		}
		else {
			spInstanceIDNode = ISimpleNode::CreateSimpleNode(kXMP_NS_XMP_MM, AdobeXMPCommon::npos, "InstanceID", AdobeXMPCommon::npos, NewInstanceID.c_str(), NewInstanceID.size());
			ixmpstruct->InsertNode(spInstanceIDNode);
		}
		return true;
	}

	bool AssetUtilitiesImpl::GetOldestHistoryInstanceID(const spIStructureNode ixmpstruct, std::string& historyEntryInstanceID)
	{
		spIArrayNode historyArrayNode = ixmpstruct->GetArrayNode(kXMP_NS_XMP_MM, AdobeXMPCommon::npos, "History", AdobeXMPCommon::npos);

		if (historyArrayNode) {
			sizet historyEntryCount = historyArrayNode->ChildCount();
			for (sizet historyChildIndex = 1; historyChildIndex <= historyEntryCount; ++historyChildIndex) {
				spIStructureNode historyEntryNode = (historyArrayNode->GetStructureNodeAtIndex(historyChildIndex));
				if (historyEntryNode) {
					spISimpleNode historyNodeInstanceID = historyEntryNode->GetSimpleNode(kXMP_NS_XMP_ResourceEvent, AdobeXMPCommon::npos, "instanceID", AdobeXMPCommon::npos);
					if (historyNodeInstanceID) {
						historyEntryInstanceID = historyNodeInstanceID->GetValue()->c_str();//double conversion to simple node dats why removing this one
						return true;
					}
				}
			}
			return false;
		}
		return false;
	}

	bool AssetUtilitiesImpl::RemoveEmptyNode(spIStructureNode stnode, std::string ns, std::string prop)
	{
		if (stnode == NULL ) return false;

		spINode node = stnode->GetNode(ns.c_str(),ns.size(),  prop.c_str(), prop.size());
		
		if (node && (node->GetNodeType() == INode::kNTSimple) && std::string(((node)->ConvertToSimpleNode())->GetValue()->c_str()).empty())
			stnode->RemoveNode(ns.c_str(), ns.size(), prop.c_str(), prop.size());

		return true;
	}
	
}

#include "XMPAssetManagement/Interfaces/TSingletonExtension.h"

namespace AdobeXMPAM {
	using namespace AdobeXMPCommon_Int;
	using namespace AdobeXMPAM_Int;

	template< >
	pIAssetUtilities_I ITSingleton< IAssetUtilities_I >::GetInstance() {
		return &TSingleton< AssetUtilitiesImpl >::Instance();
	}

	template< >
	void ITSingleton< IAssetUtilities_I >::CreateInstance() {
		GetInstance();
	}

	template< >
	void ITSingleton< IAssetUtilities_I >::DestroyInstance() {
		TSingleton< AssetUtilitiesImpl >::Destroy();
	}
}


namespace AdobeXMPAM {
	using namespace AdobeXMPAM_Int;
	spIAssetUtilities IAssetUtilities_v1::MakeShared(pIAssetUtilities_base ptr) {
		if (!ptr) return spIAssetUtilities();
		pIAssetUtilities p = IAssetUtilities::GetInterfaceVersion() > 1 ? ptr->GetInterfacePointer< IAssetUtilities >() : ptr;
		return MakeUncheckedSharedPointer(p, __FILE__, __LINE__, false);
	}
}

