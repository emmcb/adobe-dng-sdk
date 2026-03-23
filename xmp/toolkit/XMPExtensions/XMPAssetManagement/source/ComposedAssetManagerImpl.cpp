// =================================================================================================
// ADOBE SYSTEMS INCORPORATED
// Copyright 2014 Adobe Systems Incorporated
// All Rights Reserved
//
// NOTICE:  Adobe permits you to use, modify, and distribute this file in accordance with the terms
// of the Adobe license agreement accompanying it.
// =================================================================================================

#define IMPLEMENTATION_HEADERS_CAN_BE_INCLUDED 1
	#include "XMPAssetManagement/ImplHeaders/ComposedAssetManagerImpl.h"
	#include "XMPExtensions/include/ExtensionsConfigurationManagerImpl.h"
#undef IMPLEMENTATION_HEADERS_CAN_BE_INCLUDED


#include "XMPCommon/Interfaces/IErrorNotifier_I.h"
#include "XMPCommon/Utilities/AutoSharedLock.h"
#include "XMPCommon/Interfaces/IUTF8String_I.h"
#include "XMPAssetManagement/Interfaces/IAssetPart_I.h"
#include "XMPAssetManagement/Interfaces/IAssetPagePart_I.h"
#include "XMPAssetManagement/Interfaces/IAssetLayerPart_I.h"
#include "XMPAssetManagement/Interfaces/IAssetTimePart_I.h"
#include "XMPAssetManagement/Interfaces/ICompositionRelationship_I.h"
#include "XMPCore/Interfaces/IMetadata.h"
#include "XMPCore/Interfaces/ISimpleNode.h"
#include "XMPCore/Interfaces/IArrayNode.h"
#include "XMPCore/Interfaces/INodeIterator.h"
#include "XMPCore/Interfaces/IDOMImplementationRegistry.h"
#include "XMPCommon/Interfaces/IError_I.h"
#include "XMPCommon/XMPCommonErrorCodes.h"
#include "XMPCommon/Utilities/TSmartPointers_I.h"
#include "XMPCore/Interfaces/IDOMSerializer.h"
#include "XMPAssetManagement/Interfaces/IAssetUtilities_I.h"
 
#include <iostream>
#include <string>

#define TXMP_STRING_TYPE std::string 
// Ensure XMP templates are instantiated

// Provide access to the API

#define OLD_LINKING_XMPCORE_LIB LINKING_XMPCORE_LIB
#undef LINKING_XMPCORE_LIB
#define LINKING_XMPCORE_LIB 1
#include "public/include/XMP.hpp"
#include "public/include/XMP.incl_cpp"
#undef LINKING_XMPCORE_LIB
#define LINKING_XMPCORE_LIB OLD_LINKING_XMPCORE_LIB
#undef OLD_LINKING_XMPCORE_LIB

namespace AdobeXMPAM_Int {
	//static function used in this file
	//For debugging
	void static SerializeMetadata(spINode imetadata)
	{
		spIDOMImplementationRegistry DOMRegistry = IDOMImplementationRegistry::GetDOMImplementationRegistry();
		spIDOMSerializer DOMSerializer = DOMRegistry->GetSerializer("rdf");
		std::string targetfilebuffer = DOMSerializer->Serialize(imetadata)->c_str();
		std::cout << targetfilebuffer << std::endl;
	}


	//definition of static factory methods of internal interface
	static std::string GetCurrentDateAndTime()
	{
		XMP_DateTime now;
		std::string dateStr;
		SXMPUtils::CurrentDateTime(&now);
		SXMPUtils::ConvertToLocalTime(&now);
		SXMPUtils::ConvertFromDate(now, &dateStr);

		return dateStr;
	}

	//Get qualifier, if present add into the IXMPSimpleNode
	//Adding qualifier may change and name may be like Get/Set Description, at that time this code may move in some other function
	static void AddQualifier(const spcIAssetPart& part, spISimpleNode& node)
	{
		spcIUTF8String qualifier;
		switch (part->GetIAssetPart_I()->GetType()) {
			//case IAssetPart_v1::kAssetPartTypeGeneric: 	qualifier = part->GetDescription()->c_str(); break;
			//case IAssetPart_v1::kAssetPartTypeTime: 	qualifier = NS_XMPCORE::dynamic_pointer_cast<const IAssetTimePart>(part)->get>get(); break;
			//case IAssetPart_v1::kAssetPartTypeArtboard: 	qualifier = composedPart->GetDescription(); break;
		case IAssetPart_v1::kAssetPartTypePage: 	qualifier = dynamic_pointer_cast<const IAssetPagePart>(part)->GetDescriptivePageNumber(); break;
		case IAssetPart_v1::kAssetPartTypeLayer: 	qualifier = dynamic_pointer_cast<const IAssetLayerPart>(part)->GetName(); break;
		}
		std::string qualifierStr;
		if (qualifier)
			qualifierStr = qualifier->c_str();

		if ( !qualifierStr.empty() ) {
			spISimpleNode toPartQualifier = ISimpleNode::CreateSimpleNode(kXMP_NS_XMP_ResourceRef, AdobeXMPCommon::npos, "Description", AdobeXMPCommon::npos, qualifier->c_str(), qualifier->size());
			node->InsertQualifier(toPartQualifier);
		}
	}

	//got the two parts, parse the ingredient and delete it from there
	static void SearchAndRemoveFromIngredientAndPantry(spIStructureNode ingredientNode, spIArrayNode composedIngredient, spIArrayNode composedPantry, std::map<std::string, uint32>& componentCountPantryList)
	{
		for (sizet ingredientIndex = composedIngredient->ChildCount(); ingredientIndex >= 1; --ingredientIndex) {
			//spINode tnode = composedIngredient->GetNodeAtIndex(ingredientIndex);
			//spIStructureNode ingredientEntry = (composedIngredient->GetNodeAtIndex(ingredientIndex))->ConvertToStructureNode();
			spIStructureNode ingredientEntry = composedIngredient->GetStructureNodeAtIndex(ingredientIndex);
			std::string componentDocumentID, componentInstanceId, toPartString;
			spcINode node = ingredientEntry->GetNode(kXMP_NS_XMP_ResourceRef, AdobeXMPCommon::npos, "documentID", AdobeXMPCommon::npos);
			if (node)
				componentDocumentID = node->ConvertToSimpleNode()->GetValue()->c_str();
			
			node = ingredientEntry->GetNode(kXMP_NS_XMP_ResourceRef, AdobeXMPCommon::npos, "instanceID", AdobeXMPCommon::npos);
			if (node)
				componentInstanceId = node->ConvertToSimpleNode()->GetValue()->c_str();

			node = ingredientEntry->GetNode(kXMP_NS_XMP_ResourceRef, AdobeXMPCommon::npos, "toPart", AdobeXMPCommon::npos);
			if (node)
				toPartString = node->ConvertToSimpleNode()->GetValue()->c_str();

			if (ingredientNode->GetActualIStructureNode() == ingredientEntry->GetActualIStructureNode()) {
				composedIngredient->RemoveNodeAtIndex(ingredientIndex);
			} else
				continue;

			//decrease the count of document of the above matching InstanceID+documentID, if the count is 0, delete it from pantry
			uint32 componentReferenceCount = componentCountPantryList[componentInstanceId + componentDocumentID];
			if (componentReferenceCount > 1) {
				componentReferenceCount = componentReferenceCount - 1;
			}
			else{
				//search this component from pantry and remove it
				uint32 pantryIndex = 1;
				for (sizet pantryIndex = composedPantry->ChildCount(); pantryIndex >= 1; --pantryIndex) {
					spIStructureNode pantryEntry = (composedPantry->GetStructureNodeAtIndex(pantryIndex));

					std::string componentDocumentIDPantryEntry, componentInstanceIdPantryEntry;
					spINode node = pantryEntry->GetNode(kXMP_NS_XMP_MM, AdobeXMPCommon::npos, "DocumentID", AdobeXMPCommon::npos);
					if (node)
						componentDocumentIDPantryEntry = node->ConvertToSimpleNode()->GetValue()->c_str();
					node = pantryEntry->GetNode(kXMP_NS_XMP_MM, AdobeXMPCommon::npos, "InstanceID", AdobeXMPCommon::npos);
					if (node)
						componentInstanceIdPantryEntry = node->ConvertToSimpleNode()->GetValue()->c_str();

					if ((componentInstanceIdPantryEntry.compare(componentInstanceId) == 0) && 
						(componentDocumentIDPantryEntry.compare(componentDocumentID)) == 0) {
						composedPantry->RemoveNodeAtIndex(pantryIndex);
						return;
					}
					else
						continue;
				}
			}
		}
	}

	
	static void UpdateHistoryAndMetadataNode(spIMetadata& ixmpmetadata, std::string sAction, std::string sChanged, std::string sAgent, std::string sInstanceID="")
	{
		spIArrayNode historyArrayNode;
		spINode node = ixmpmetadata->GetNode(kXMP_NS_XMP_MM, AdobeXMPCommon::npos, "History", AdobeXMPCommon::npos);
		if (node)
			historyArrayNode = node->ConvertToArrayNode();
		ITSingleton< IExtensionsConfigurationManager_I >::GetInstance()->UpdateHistoryEntry(historyArrayNode);

		if (sInstanceID.empty()) {
			std::string NewInstanceIDPrefix = ITSingleton< IExtensionsConfigurationManager_I >::GetInstance()->GetPrefix(IExtensionsConfigurationManager_v1::kInstanceIDPrefix);
			SXMPDocOps::CreateID(&sInstanceID, /*"xmp.iid:"*/ NewInstanceIDPrefix.c_str());
		}

		(ixmpmetadata->GetSimpleNode(kXMP_NS_XMP_MM, AdobeXMPCommon::npos, "InstanceID", AdobeXMPCommon::npos)->SetValue(sInstanceID.c_str(), sInstanceID.size()));

		spISimpleNode xmpMMMetadataDate;
		node = ixmpmetadata->GetNode(kXMP_NS_XMP_MM, AdobeXMPCommon::npos, "MetadataDate", AdobeXMPCommon::npos);
		if(node)
			xmpMMMetadataDate = node->ConvertToSimpleNode();
		if (xmpMMMetadataDate == NULL) {
			xmpMMMetadataDate = ISimpleNode::CreateSimpleNode(kXMP_NS_XMP_MM, AdobeXMPCommon::npos, "MetadataDate", AdobeXMPCommon::npos, GetCurrentDateAndTime().c_str(), GetCurrentDateAndTime().size());
			ixmpmetadata->InsertNode(xmpMMMetadataDate);
		}
		else {
			(xmpMMMetadataDate)->ConvertToSimpleNode()->SetValue(GetCurrentDateAndTime().c_str(), GetCurrentDateAndTime().size());
		}
	}

	static void CreateIdBasedMapFromPantry(spIArrayNode assetPantry, std::map<std::string, spIStructureNode >& composedPantryList, std::map<std::string, uint32>& componentCountPantryList)
	{
		if (assetPantry == NULL) return;

		//iterate through the Pantry array and fill the documentPantryMap
		for (spINodeIterator pantryIterator = assetPantry->Iterator(); pantryIterator != NULL; pantryIterator = pantryIterator->Next()) {
			spIStructureNode pantryEntry = (pantryIterator->GetNode())->ConvertToStructureNode();

			//Get documentid and instance id
			std::string docid_pantryentry, instanceid_pantryentry;
			spINode node = pantryEntry->GetNode(kXMP_NS_XMP_MM, AdobeXMPCommon::npos, "InstanceID", AdobeXMPCommon::npos);
			if (node != NULL)
				instanceid_pantryentry = node->ConvertToSimpleNode()->GetValue()->c_str();

			node = pantryEntry->GetNode(kXMP_NS_XMP_MM, AdobeXMPCommon::npos, "DocumentID", AdobeXMPCommon::npos);
			if (node != NULL)
				docid_pantryentry = node->ConvertToSimpleNode()->GetValue()->c_str();

			if (instanceid_pantryentry.empty() || docid_pantryentry.empty()) continue; //no ids, ignore it

			std::string key = instanceid_pantryentry + docid_pantryentry;
			composedPantryList[key] = pantryEntry; 
			componentCountPantryList[key] = 1;
		}
	} //end _ParsePantry

	static void ParseIngredientsForCompositionRelationship(spIMetadata composedAssetMetadata, std::list<std::pair<spICompositionRelationship, spIStructureNode> >& compositeRelationshipList, std::map<std::string, spIStructureNode >& composedPantryList, spICompositionRelationshipList& relationshipList)
	{
		spIArrayNode assetIngredients;
		spINode tnode = composedAssetMetadata->GetNode(kXMP_NS_XMP_MM, AdobeXMPCommon::npos, "Ingredients", AdobeXMPCommon::npos);
		if (tnode)
			assetIngredients = tnode->ConvertToArrayNode();
		
		if (!assetIngredients) return;

		for (sizet assetIngredientChildcount = assetIngredients->ChildCount(), count = 1; count <= assetIngredientChildcount; ++count) {
			spIStructureNode ingredientEntry = (assetIngredients->GetNodeAtIndex(count))->ConvertToStructureNode();

			tnode = ingredientEntry->GetNode(kXMP_NS_XMP_ResourceRef, AdobeXMPCommon::npos, "toPart", AdobeXMPCommon::npos);
			spISimpleNode node;
			if (tnode)
				node = tnode->ConvertToSimpleNode();
			std::string part_value, instanceId, documentId;
			spcIAssetPart_I compositePart, componentPart;
			if ( node ) {
				compositePart = IAssetPart_I::ParseAndCreatePartFromRDFSyntax(node->GetValue()->c_str(), node->GetValue()->size() );
			}
			tnode = ingredientEntry->GetNode(kXMP_NS_XMP_ResourceRef, AdobeXMPCommon::npos, "fromPart", AdobeXMPCommon::npos);
			if(tnode)
				node = tnode->ConvertToSimpleNode();
			if ( node ) {
				componentPart = IAssetPart_I::ParseAndCreatePartFromRDFSyntax(node->GetValue()->c_str(), node->GetValue()->size() );
			}

			tnode = ingredientEntry->GetNode(kXMP_NS_XMP_ResourceRef, AdobeXMPCommon::npos, "instanceID", AdobeXMPCommon::npos);
			if (tnode)
				node = tnode->ConvertToSimpleNode();
			if ( node )
				instanceId = node->GetValue()->c_str();

			tnode = ingredientEntry->GetNode(kXMP_NS_XMP_ResourceRef, AdobeXMPCommon::npos, "documentID", AdobeXMPCommon::npos);
			if (tnode)
				node = tnode->ConvertToSimpleNode();
			if ( node )
				documentId = node->GetValue()->c_str();

			//from the pantry, get the componentIXMPStruct
			spIStructureNode componentIXMPStruct = composedPantryList[instanceId + documentId];
			//if any of the above is missing, raise error
			if (!compositePart || !componentIXMPStruct || !componentPart) continue; //raise an exception

			//create a relationship with the above values and add into compositeRelationshipList
			spICompositionRelationship_I compositeAssetRelationshipNode = ICompositionRelationship_I::CreateCompositionRelationship(composedAssetMetadata, compositePart, componentIXMPStruct, componentPart);

			//check for other properties and put it in compositeAssetRelationshipNode
			//stRef:maskMarkers
			std::string propvalue;
			tnode = ingredientEntry->GetNode(kXMP_NS_XMP_ResourceRef, AdobeXMPCommon::npos, "maskMarkers", AdobeXMPCommon::npos);
			if (tnode)
				node = tnode->ConvertToSimpleNode();
			if ( !node && !compositeAssetRelationshipNode ) {
				propvalue = node->GetValue()->c_str();
				if (propvalue.compare("All") == 0)
					compositeAssetRelationshipNode->SetMaskMarkersFlag(ICompositionRelationship_v1::kCompositionMaskMarkersAll);
				if (propvalue.compare("None") == 0)
					compositeAssetRelationshipNode->SetMaskMarkersFlag(ICompositionRelationship_v1::kCompositionMaskMarkersNone);
				compositeAssetRelationshipNode->SetMaskMarkersFlag(ICompositionRelationship_v1::kCompositionMaskMarkersNotSpecified);
			}

			//stRef:mapping
			tnode = ingredientEntry->GetNode(kXMP_NS_XMP_ResourceRef, AdobeXMPCommon::npos, "maskMarkers", AdobeXMPCommon::npos);
			if (tnode)
				node = tnode->ConvertToSimpleNode();
			if (!node && !compositeAssetRelationshipNode) {
				compositeAssetRelationshipNode->SetMappingFunctionName(node->GetValue()->c_str(), node->GetValue()->size());
			}

			//stRef:filePath
			tnode = ingredientEntry->GetNode(kXMP_NS_XMP_ResourceRef, AdobeXMPCommon::npos, "filePath", AdobeXMPCommon::npos);
			if (tnode)
				node = tnode->ConvertToSimpleNode();
			if (!node && !compositeAssetRelationshipNode) {
				compositeAssetRelationshipNode->SetComponentFilePath(node->GetValue()->c_str(), node->GetValue()->size());
			}

			//stRef:alternatePaths
			tnode = ingredientEntry->GetNode(kXMP_NS_XMP_ResourceRef, AdobeXMPCommon::npos, "alternatePaths", AdobeXMPCommon::npos);
			if (tnode)
				node = tnode->ConvertToSimpleNode();
			if (!node && !compositeAssetRelationshipNode) {
				compositeAssetRelationshipNode->AppendComponentAlternatePath(node->GetValue()->c_str(), node->GetValue()->size());
			}
			
			compositeAssetRelationshipNode->SetIngredientNode(ingredientEntry);
			
			//add it into list
			relationshipList->push_back(compositeAssetRelationshipNode);
		}
	}

	//definition of static factory methods of internal interface

	spIComposedAssetManager_I IComposedAssetManager_I::CreateComposedAssetManager( const spIMetadata & metadata )
	{
		pIAssetUtilities utilityObj = IAssetUtilities_I::GetInstance();
		if (utilityObj->IsTrackable(metadata) == false)
			NOTIFY_ERROR(IError_base::kEDGeneral, kGECParametersNotAsExpected,
			"Component is not Trackable", IError_base::kESOperationFatal, false, false);

		return MakeUncheckedSharedPointer< IComposedAssetManager_I >(
			new ComposedAssetManagerImpl( metadata ), __FILE__, __LINE__
		);
	}

	//definition of class methods

	/*
	**	Algo:
	**		Call the composedPart for it's serialization and then make an entry into relationshipList and
	**		also create one entry in ingredients, put the component in pantry list
	*/

	spICompositionRelationship APICALL ComposedAssetManagerImpl::AddComponent( const spcIAssetPart & composedPart,
		const spcIStructureNode & component, const spcIAssetPart & componentPart )
	{
		if ((composedPart == NULL) || (component == NULL) || (componentPart == NULL))
			NOTIFY_ERROR(IError_base::kEDGeneral, kGECParametersNotAsExpected,
			"Parameters not as expected by ComposedAssetManagerImpl::AddComponent", IError_base::kESOperationFatal, false, false);
		
		GetIXMPMetaIngredientNode();
		GetIXMPMetaPantryNode();

		//create a new structure for entry into ingredients
		spIStructureNode newIngredientEntryNode = IStructureNode::CreateStructureNode(kXMP_NS_XMP_MM, AdobeXMPCommon::npos, "Ingredients", AdobeXMPCommon::npos);
		spcISimpleNode xmpMMInstanceIDNode = (component->GetSimpleNode(kXMP_NS_XMP_MM, AdobeXMPCommon::npos, "InstanceID", AdobeXMPCommon::npos));
		spcISimpleNode xmpMMDocumentIDNode = (component->GetSimpleNode(kXMP_NS_XMP_MM, AdobeXMPCommon::npos, "DocumentID", AdobeXMPCommon::npos));


		if (!xmpMMInstanceIDNode || std::string(xmpMMInstanceIDNode->GetValue()->c_str()).empty() || !xmpMMDocumentIDNode || std::string(xmpMMDocumentIDNode->GetValue()->c_str()).empty())
			NOTIFY_ERROR(IError_base::kEDGeneral, kGECParametersNotAsExpected,
			"Component is not Trackable", IError_base::kESOperationFatal, false, false);
		
		//to & from Part
		spcIUTF8String toPartUTF8String = composedPart->GetIAssetPart_I()->SerializeAsRDFSyntax();
		spISimpleNode stReftoPartSimpleNode = ISimpleNode::CreateSimpleNode(kXMP_NS_XMP_ResourceRef, AdobeXMPCommon::npos, "toPart", AdobeXMPCommon::npos, toPartUTF8String->c_str(), toPartUTF8String->size());
		AddQualifier(composedPart, stReftoPartSimpleNode);

		spcIUTF8String fromPartUTF8String = componentPart->GetIAssetPart_I()->SerializeAsRDFSyntax();
		spISimpleNode stReffromPartSimpleNode = ISimpleNode::CreateSimpleNode(kXMP_NS_XMP_ResourceRef, AdobeXMPCommon::npos, "fromPart", AdobeXMPCommon::npos, fromPartUTF8String->c_str(), fromPartUTF8String->size());
		AddQualifier(componentPart, stReffromPartSimpleNode);
		
		spISimpleNode stRefinstanceID = ISimpleNode::CreateSimpleNode(kXMP_NS_XMP_ResourceRef, AdobeXMPCommon::npos, "instanceID", AdobeXMPCommon::npos,xmpMMInstanceIDNode->GetValue()->c_str(), xmpMMInstanceIDNode->GetValue()->size());
		spISimpleNode stRefdocumentID = ISimpleNode::CreateSimpleNode(kXMP_NS_XMP_ResourceRef, AdobeXMPCommon::npos, "documentID", AdobeXMPCommon::npos,xmpMMDocumentIDNode->GetValue()->c_str(), xmpMMDocumentIDNode->GetValue()->size());


		newIngredientEntryNode->InsertNode(stReftoPartSimpleNode);
		newIngredientEntryNode->InsertNode(stReffromPartSimpleNode);
		newIngredientEntryNode->InsertNode(stRefinstanceID);
		newIngredientEntryNode->InsertNode(stRefdocumentID);

		//Get & set other properties of relationship node


		if (mComposedIngredientArray)
			mComposedIngredientArray->AppendNode(newIngredientEntryNode);
		
		std::string xmpMMInstanceIDString = xmpMMInstanceIDNode->GetValue()->c_str();
		std::string xmpMMDocumentIDString = xmpMMDocumentIDNode->GetValue()->c_str();
		
		if (mComposedPantryList.find(xmpMMInstanceIDString + xmpMMDocumentIDString) == mComposedPantryList.end()) {
			//Add component structure into pantry array
			if (mComposedPantryArray) {
				spIStructureNode newPantryNode = IStructureNode::CreateStructureNode(kXMP_NS_XMP_MM, AdobeXMPCommon::npos, "Pantry", AdobeXMPCommon::npos);
				bool IsComponentPantry = false;
				//copy all the content except pantry, if there is pantry, parse it 1 by 1 and create new pantry entry
				for (auto it = component->Iterator(); it; it = it->Next()) {
					if (strcmp(it->GetNode()->GetNameSpace()->c_str(), kXMP_NS_XMP_MM) == 0 && strcmp(it->GetNode()->GetName()->c_str(), "Pantry") == 0) {
						IsComponentPantry = true;
						continue;
					}
					//check in pantry whether it is already present or not
					newPantryNode->InsertNode(it->GetNode()->Clone());
				}
				mComposedPantryArray->AppendNode(newPantryNode);
				mComposedPantryList[xmpMMInstanceIDString + xmpMMDocumentIDString] = newPantryNode;
				mComponentCountPantryList[xmpMMInstanceIDString + xmpMMDocumentIDString] = 1;
				//move pantry up
				if (IsComponentPantry) {
					spIArrayNode componentPantryNode = (component->GetNode(kXMP_NS_XMP_MM, AdobeXMPCommon::npos, "Pantry", AdobeXMPCommon::npos)->Clone())->ConvertToArrayNode();
					for (auto componentPantryIt = componentPantryNode->Iterator(); componentPantryIt; componentPantryIt = componentPantryIt->Next()) {
						//check whether that has been already processed or not; remove duplicacy
						spIStructureNode pantryEntryStructureNode = (componentPantryIt->GetNode())->ConvertToStructureNode();
						spISimpleNode node = (pantryEntryStructureNode->GetNode(kXMP_NS_XMP_MM, AdobeXMPCommon::npos, "InstanceID", AdobeXMPCommon::npos))->ConvertToSimpleNode();
						std::string  pantryEntryInstanceID, pantryEntryDocumentID;
						if (node)
							pantryEntryInstanceID = node->GetValue()->c_str();
						node = (pantryEntryStructureNode->GetNode(kXMP_NS_XMP_MM, AdobeXMPCommon::npos, "DocumentID", AdobeXMPCommon::npos))->ConvertToSimpleNode();
						if (node)
							pantryEntryDocumentID = node->GetValue()->c_str();

						if (mComponentCountPantryList.find(pantryEntryInstanceID + pantryEntryDocumentID) != mComponentCountPantryList.end()) {
							continue;
						}
						mComposedPantryArray->AppendNode(componentPantryIt->GetNode()->Clone());
						mComposedPantryList[pantryEntryInstanceID + pantryEntryDocumentID] = dynamic_pointer_cast<IStructureNode>(componentPantryIt);      // refactor convert to structure node
						mComponentCountPantryList[pantryEntryInstanceID + pantryEntryDocumentID] = 1;
					}
				}
			}
		}
		else {
			mComponentCountPantryList[xmpMMInstanceIDString + xmpMMDocumentIDString] = mComponentCountPantryList[xmpMMInstanceIDString + xmpMMDocumentIDString] + 1;
		}

		UpdateHistoryAndMetadataNode(mComposedAssetMetadata, "saved", "/metadata", "", "");
		
		spICompositionRelationship_I newRelationship = ICompositionRelationship_I::CreateCompositionRelationship(mComposedAssetMetadata, composedPart, component, componentPart);
		mRelationshipList->push_back(newRelationship);
		newRelationship->SetIngredientNode(newIngredientEntryNode);

		return newRelationship; 
	}

	sizet APICALL ComposedAssetManagerImpl::RemoveComponents( const spcIStructureNode & component,
		const spcIAssetPart & componentPart /*= spcIAssetPart_I() */ )
	{
		if (component == NULL)
			return 0;

		bool isRelationshipPresent = false;
		sizet relationshipRemovedCount = 0;

		spcISimpleNode componentInstanceIDNode = (component->GetSimpleNode(kXMP_NS_XMP_MM, AdobeXMPCommon::npos, "InstanceID", AdobeXMPCommon::npos));
		spcISimpleNode componentDocumentIDNode = (component->GetSimpleNode(kXMP_NS_XMP_MM, AdobeXMPCommon::npos, "DocumentID", AdobeXMPCommon::npos));
		if (!componentInstanceIDNode || !componentDocumentIDNode)
			return 0;

		std::string componentInstanceID = componentInstanceIDNode->GetValue()->c_str();
		std::string componentDocumentID = componentDocumentIDNode->GetValue()->c_str();


		for (ICompositionRelationshipList::iterator cIt = mRelationshipList->begin(); cIt != mRelationshipList->end();) {
			std::string composedPartValue;
			
			spcIStructureNode relationshipComponentPart = (*cIt)->GetComponentMetadata();
			std::string relationshipComponentInstanceID = (relationshipComponentPart->GetNode(kXMP_NS_XMP_MM, AdobeXMPCommon::npos, "InstanceID", AdobeXMPCommon::npos))->ConvertToSimpleNode()->GetValue()->c_str();
			std::string relationshipComponentDocumentID = (relationshipComponentPart->GetNode(kXMP_NS_XMP_MM, AdobeXMPCommon::npos, "DocumentID", AdobeXMPCommon::npos))->ConvertToSimpleNode()->GetValue()->c_str();

			if ((componentInstanceID.compare(relationshipComponentInstanceID) != 0) || (componentDocumentID.compare(relationshipComponentDocumentID) != 0)) {
				++cIt;
				continue;
			}

			spIStructureNode ingredientNode;
			if (componentPart) {
				std::string listComponentPartValue = (*cIt)->GetComponentPart()->GetIAssetPart_I()->SerializeAsRDFSyntax()->c_str();
				std::string componentPartValue = componentPart->GetIAssetPart_I()->SerializeAsRDFSyntax()->c_str();

				if (listComponentPartValue.compare(componentPartValue) == 0) {
					ingredientNode = (*cIt)->GetICompositionRelationship_I()->GetIngredientNode();
					cIt = mRelationshipList->erase(cIt);
					++relationshipRemovedCount;
					isRelationshipPresent = true;
				}
			}
			else {
				ingredientNode = (*cIt)->GetICompositionRelationship_I()->GetIngredientNode();
				cIt = mRelationshipList->erase(cIt);
				++relationshipRemovedCount;
				isRelationshipPresent = true;
			}
			if (isRelationshipPresent) {
				SearchAndRemoveFromIngredientAndPantry(ingredientNode, mComposedIngredientArray, mComposedPantryArray, mComponentCountPantryList);
			}

			if ((cIt != mRelationshipList->end()) && (isRelationshipPresent == false))
				++cIt;
		}

		if (isRelationshipPresent) {
			UpdateHistoryAndMetadataNode(mComposedAssetMetadata, "saved", "/metadata", "", "");
			ResetIngredientAndPantryEntry();
		}

		return relationshipRemovedCount;
	}

	sizet APICALL ComposedAssetManagerImpl::RemoveComponents( const char * documentID,sizet docIDLength, const char * instanceID /*= NULL*/, sizet insIDLength,
		const spcIAssetPart & componentPart /*= spcIAssetPart_I( ) */ )
	{
		bool isRelationshipPresent = false;
		sizet relationshipRemovedCount = 0;

		if (documentID == NULL)
			return 0;

		std::string argDocumentId(documentID);
		std::string argInstanceID;
		if (instanceID) {
			argInstanceID = instanceID;
		}

		for (ICompositionRelationshipList::iterator cIt = mRelationshipList->begin(); cIt != mRelationshipList->end();) {
			spcIStructureNode componentMetadata = ((*cIt)->GetComponentMetadata())->ConvertToStructureNode();
			std::string composedPartValue;

			std::string componentInstanceID = (componentMetadata->GetNode(kXMP_NS_XMP_MM, AdobeXMPCommon::npos, "InstanceID", AdobeXMPCommon::npos))->ConvertToSimpleNode()->GetValue()->c_str();
			std::string componentDocumentID = (componentMetadata->GetNode(kXMP_NS_XMP_MM, AdobeXMPCommon::npos, "DocumentID", AdobeXMPCommon::npos))->ConvertToSimpleNode()->GetValue()->c_str();

			if ((argDocumentId.compare(componentDocumentID) == 0) && ((instanceID == NULL) || (argInstanceID.compare(componentInstanceID) == 0))) {
				spIStructureNode ingredientNode;
				if (componentPart) {
					std::string listComponentPartValue = (*cIt)->GetComponentPart()->GetIAssetPart_I()->SerializeAsRDFSyntax()->c_str();
					std::string componentPartValue = componentPart->GetIAssetPart_I()->SerializeAsRDFSyntax()->c_str();

					if (listComponentPartValue.compare(componentPartValue) == 0) {
						ingredientNode = (*cIt)->GetICompositionRelationship_I()->GetIngredientNode();
						cIt = mRelationshipList->erase(cIt);
						++relationshipRemovedCount;
						isRelationshipPresent = true;
					}
				}
				else {
					ingredientNode = (*cIt)->GetICompositionRelationship_I()->GetIngredientNode();
					cIt = mRelationshipList->erase(cIt);
					isRelationshipPresent = true;
					++relationshipRemovedCount;
				}
				if (isRelationshipPresent) {
					SearchAndRemoveFromIngredientAndPantry(ingredientNode, mComposedIngredientArray, mComposedPantryArray, mComponentCountPantryList);
				}

				if ((cIt != mRelationshipList->end()) && (isRelationshipPresent == false))
					++cIt;
			} else {  //document id mismatched
				++cIt;
				continue;
			}
		}

		if (isRelationshipPresent) {
			UpdateHistoryAndMetadataNode(mComposedAssetMetadata, "saved", "/metadata", "", "");
			ResetIngredientAndPantryEntry();
		}

		return relationshipRemovedCount;
	}


	//search the relationship list, if found erase it, part can be composed or component
	//also make sure, ingredients are removed and if required their pantry too.
	//also update MetadataDate and history
	sizet APICALL ComposedAssetManagerImpl::RemoveComponents( const spcIAssetPart & composedPart ) {
		if (composedPart == NULL)
			return 0;

		bool isRelationshipPresent = false;
		sizet relationshipRemovedCount = 0;

		for (ICompositionRelationshipList::iterator cIt = mRelationshipList->begin(); cIt != mRelationshipList->end(); ) {
			spIStructureNode ingredientNode;

			std::string listComponentPartValue = (*cIt)->GetComposedPart()->GetIAssetPart_I()->SerializeAsRDFSyntax()->c_str();
			std::string composedPartValue = composedPart->GetIAssetPart_I()->SerializeAsRDFSyntax()->c_str();

			if (listComponentPartValue.compare(composedPartValue) == 0) {
				ingredientNode = (*cIt)->GetICompositionRelationship_I()->GetIngredientNode();
				cIt = mRelationshipList->erase(cIt);
				++relationshipRemovedCount;
				isRelationshipPresent = true;
			}

			if (isRelationshipPresent) {
				SearchAndRemoveFromIngredientAndPantry(ingredientNode, mComposedIngredientArray, mComposedPantryArray, mComponentCountPantryList);
			}

			if ((cIt != mRelationshipList->end()) && (isRelationshipPresent == false))
				++cIt;
		}
		
		if (isRelationshipPresent) {
			UpdateHistoryAndMetadataNode(mComposedAssetMetadata, "saved", "/metadata", "", "");
			ResetIngredientAndPantryEntry();
		}

		return relationshipRemovedCount;
	}


	//search the relationship list, if found erase it, part can be composed or component
	//also make sure, ingredients are removed and if required their pantry too.
	//also update MetadataDate and history
	bool APICALL ComposedAssetManagerImpl::RemoveComponent(const spcICompositionRelationship & relationship) {
		if (relationship == NULL)
			return false;

		bool isRelationshipPresent = false;
		bool relationshipRemovedCount = false;

		for (ICompositionRelationshipList::iterator cIt = mRelationshipList->begin(); cIt != mRelationshipList->end(); ) {
			spIStructureNode ingredientNode;

			if (relationship == *cIt) {
				ingredientNode = (*cIt)->GetICompositionRelationship_I()->GetIngredientNode();
				cIt = mRelationshipList->erase(cIt);
				++relationshipRemovedCount;
				isRelationshipPresent = true;
			}

			if (isRelationshipPresent) {
				SearchAndRemoveFromIngredientAndPantry(ingredientNode, mComposedIngredientArray, mComposedPantryArray, mComponentCountPantryList);
			}

			if ((cIt != mRelationshipList->end()) && (isRelationshipPresent == false))
				++cIt;
		}

		if (isRelationshipPresent) {
			UpdateHistoryAndMetadataNode(mComposedAssetMetadata, "saved", "/metadata", "", "");
			ResetIngredientAndPantryEntry();
		}
		
		return relationshipRemovedCount;
	}

	spcICompositionRelationshipList APICALL ComposedAssetManagerImpl::GetAllRelationships() const {
		spcICompositionRelationshipList matchedRelationshipList(new cICompositionRelationshipList());

		for (ICompositionRelationshipList::const_iterator cIt = mRelationshipList->begin(); cIt != mRelationshipList->end(); ++cIt) {
			matchedRelationshipList->push_back(*cIt);
		}
		return matchedRelationshipList;
	}

	spcICompositionRelationshipList APICALL ComposedAssetManagerImpl::GetRelationships( const spcIStructureNode & component,
		const spcIAssetPart & componentPart /*= spcIAssetPart_I() */ ) const
	{
		spcICompositionRelationshipList matchedRelationshipList(new cICompositionRelationshipList());

		if (component == NULL) return matchedRelationshipList;

		spcISimpleNode componentInstanceIDNode = (component->GetSimpleNode(kXMP_NS_XMP_MM, AdobeXMPCommon::npos, "InstanceID", AdobeXMPCommon::npos));
		spcISimpleNode componentDocumentIDNode = (component->GetSimpleNode(kXMP_NS_XMP_MM, AdobeXMPCommon::npos, "DocumentID", AdobeXMPCommon::npos));
		
		if (componentInstanceIDNode && componentDocumentIDNode) {

			std::string componentInstanceID = componentInstanceIDNode->GetValue()->c_str();
			std::string componentDocumentID = componentDocumentIDNode->GetValue()->c_str();

			for (ICompositionRelationshipList::const_iterator cIt = mRelationshipList->begin(); cIt != mRelationshipList->end(); ++cIt) {

				spcIStructureNode relationshipComponentPart = (*cIt)->GetComponentMetadata();
				std::string relationshipComponentInstanceID = (relationshipComponentPart->GetNode(kXMP_NS_XMP_MM, AdobeXMPCommon::npos, "InstanceID", AdobeXMPCommon::npos))->ConvertToSimpleNode()->GetValue()->c_str();
				std::string relationshipComponentDocumentID = (relationshipComponentPart->GetNode(kXMP_NS_XMP_MM, AdobeXMPCommon::npos, "DocumentID", AdobeXMPCommon::npos))->ConvertToSimpleNode()->GetValue()->c_str();

				if ((componentInstanceID.compare(relationshipComponentInstanceID) != 0) || (componentDocumentID.compare(relationshipComponentDocumentID) != 0)) continue;

				if (!componentPart) {
					matchedRelationshipList->push_back(*cIt);
					continue;
				}

				std::string listComponentPartValue = (*cIt)->GetComponentPart()->GetIAssetPart_I()->SerializeAsRDFSyntax()->c_str();
				std::string componentPartValue = componentPart->GetIAssetPart_I()->SerializeAsRDFSyntax()->c_str();

				if (listComponentPartValue.compare(componentPartValue) == 0) {
					matchedRelationshipList->push_back(*cIt);
				}
			}
		}

		return matchedRelationshipList;
	}

	spcICompositionRelationshipList APICALL ComposedAssetManagerImpl::GetRelationships( const char * documentID, sizet docIDLength,
		const char * instanceID /*= NULL*/, sizet insIDLength, const spcIAssetPart & componentPart /*= spcIAssetPart() */ ) const
	{
		spcICompositionRelationshipList matchedRelationshipList(new cICompositionRelationshipList());

		if (documentID == NULL) return matchedRelationshipList;

		bool considerInstanceID = false, considerComponentPart = false;

		std::string argDocumentId(documentID);
		std::string argInstanceID;
		if (instanceID) {
			argInstanceID = instanceID;
		}

		for (ICompositionRelationshipList::const_iterator cIt = mRelationshipList->begin(); cIt != mRelationshipList->end(); ++cIt) {
			spcIStructureNode componentMetadata = ((*cIt)->GetComponentMetadata())->ConvertToStructureNode();
			
			std::string componentInstanceID = (componentMetadata->GetNode(kXMP_NS_XMP_MM, AdobeXMPCommon::npos, "InstanceID", AdobeXMPCommon::npos))->ConvertToSimpleNode()->GetValue()->c_str();
			std::string componentDocumentID = (componentMetadata->GetNode(kXMP_NS_XMP_MM, AdobeXMPCommon::npos, "DocumentID", AdobeXMPCommon::npos))->ConvertToSimpleNode()->GetValue()->c_str();

			if (argDocumentId.compare(componentDocumentID) != 0)  continue; //document id mismatched

			if ( !argInstanceID.empty() && (argInstanceID.compare(componentInstanceID) != 0) )  continue; //instnace id given but mismatched

			if (!componentPart) {
				matchedRelationshipList->push_back(*cIt);
				continue;
			}

			std::string listComponentPartValue = (*cIt)->GetComponentPart()->GetIAssetPart_I()->SerializeAsRDFSyntax()->c_str();
			std::string componentPartValue = componentPart->GetIAssetPart_I()->SerializeAsRDFSyntax()->c_str();

			if (listComponentPartValue.compare(componentPartValue) == 0) {
				matchedRelationshipList->push_back(*cIt);
			}
		}

		return matchedRelationshipList;
	}

	//create a new list of composition relationship, search the mRelationshipList, and put it in
	//the part can be composed part or component part
	spcICompositionRelationshipList APICALL ComposedAssetManagerImpl::GetRelationships( const spcIAssetPart & composedPart ) const {
		spcICompositionRelationshipList matchedRelationshipList(new cICompositionRelationshipList());

		if (composedPart == NULL)
			return matchedRelationshipList;

		for (ICompositionRelationshipList::const_iterator cIt = mRelationshipList->begin(); cIt != mRelationshipList->end(); ++cIt) {
			std::string listComposedPartValue = (*cIt)->GetComposedPart()->GetIAssetPart_I()->SerializeAsRDFSyntax()->c_str();
			std::string composedPartValue = composedPart->GetIAssetPart_I()->SerializeAsRDFSyntax()->c_str();

			if (listComposedPartValue.compare(composedPartValue) == 0) {
				matchedRelationshipList->push_back(*cIt);
			}
		}
	
		return matchedRelationshipList;
	}

	//a component will be directory composed if it is in ingredient list / relationship list
	bool APICALL ComposedAssetManagerImpl::IsDirectlyComposedOf( const spcIStructureNode & component,
		const spcIAssetPart & componentPart /*= spcIAssetPart_I() */ ) const
	{
		if (component == NULL)
			return false;

		std::string componentInstanceID = (component->GetNode(kXMP_NS_XMP_MM, AdobeXMPCommon::npos, "InstanceID", AdobeXMPCommon::npos))->ConvertToSimpleNode()->GetValue()->c_str();
		std::string componentDocumentID = (component->GetNode(kXMP_NS_XMP_MM, AdobeXMPCommon::npos, "DocumentID", AdobeXMPCommon::npos))->ConvertToSimpleNode()->GetValue()->c_str();

		for (ICompositionRelationshipList::const_iterator cIt = mRelationshipList->begin(); cIt != mRelationshipList->end(); ++cIt) {

			spcIStructureNode relationshipComponentPart = (*cIt)->GetComponentMetadata();
			std::string relationshipComponentInstanceID = (relationshipComponentPart->GetNode(kXMP_NS_XMP_MM, AdobeXMPCommon::npos, "InstanceID", AdobeXMPCommon::npos))->ConvertToSimpleNode()->GetValue()->c_str();
			std::string relationshipComponentDocumentID = (relationshipComponentPart->GetNode(kXMP_NS_XMP_MM, AdobeXMPCommon::npos, "DocumentID", AdobeXMPCommon::npos))->ConvertToSimpleNode()->GetValue()->c_str();

			if ((componentInstanceID.compare(relationshipComponentInstanceID) != 0) || (componentDocumentID.compare(relationshipComponentDocumentID) != 0)) continue;

			if (!componentPart) return true;

			std::string listComponentPartValue = (*cIt)->GetComponentPart()->GetIAssetPart_I()->SerializeAsRDFSyntax()->c_str();
			std::string componentPartValue = componentPart->GetIAssetPart_I()->SerializeAsRDFSyntax()->c_str();
			if (listComponentPartValue.compare(componentPartValue) == 0) {
				return true;
			}
		}

		return false;
	}

	bool APICALL ComposedAssetManagerImpl::IsDirectlyComposedOf( const char * documentID, sizet docIDLength, const char * instanceID /*= NULL*/, sizet insIDLength,
		const spcIAssetPart & componentPart /*= spcIAssetPart_I() */ ) const
	{
		if (documentID == NULL)
			return false;

		bool considerInstanceID = false, considerComponentPart = false;

		std::string argDocumentId(documentID);
		std::string argInstanceID;
		if (instanceID) {
			argInstanceID = instanceID;
		}

		for (ICompositionRelationshipList::const_iterator cIt = mRelationshipList->begin(); cIt != mRelationshipList->end(); ++cIt) {
			spcIStructureNode componentMetadata = ((*cIt)->GetComponentMetadata())->ConvertToStructureNode();

			std::string componentInstanceID = (componentMetadata->GetNode(kXMP_NS_XMP_MM, AdobeXMPCommon::npos, "InstanceID", AdobeXMPCommon::npos))->ConvertToSimpleNode()->GetValue()->c_str();
			std::string componentDocumentID = (componentMetadata->GetNode(kXMP_NS_XMP_MM, AdobeXMPCommon::npos, "DocumentID", AdobeXMPCommon::npos))->ConvertToSimpleNode()->GetValue()->c_str();

			if (argDocumentId.compare(componentDocumentID) != 0)  continue; //document id mismatched

			if (!argInstanceID.empty() && (argInstanceID.compare(componentInstanceID) != 0))  continue; //instnace id given but mismatched

			if (!componentPart) return true;

			std::string listComponentPartValue = (*cIt)->GetComponentPart()->GetIAssetPart_I()->SerializeAsRDFSyntax()->c_str();
			std::string componentPartValue = componentPart->GetIAssetPart_I()->SerializeAsRDFSyntax()->c_str();
			if (listComponentPartValue.compare(componentPartValue) == 0) {
				return true;
			}
		}

		return false;
	}

	bool APICALL ComposedAssetManagerImpl::IsComposedOf( const spcIStructureNode & component,
		const spcIAssetPart & componentPart /*= spcIAssetPart() */ ) const
	{
		if ( component == NULL )
			NOTIFY_ERROR(IError_base::kEDGeneral, kGECParametersNotAsExpected,
			"Parameters not as expected by ComposedAssetManagerImpl::IsDirectlyComposedOf", IError_base::kESOperationFatal, false, false);

		if (IsDirectlyComposedOf(component, componentPart)) return true;

		//given component & component part is not directly composed, 
		//for each pantry entry, go to all ingredient and check for match
		std::string componentInstanceID = (component->GetNode(kXMP_NS_XMP_MM, AdobeXMPCommon::npos, "InstanceID", AdobeXMPCommon::npos))->ConvertToSimpleNode()->GetValue()->c_str();
		std::string componentDocumentID = (component->GetNode(kXMP_NS_XMP_MM, AdobeXMPCommon::npos, "DocumentID", AdobeXMPCommon::npos))->ConvertToSimpleNode()->GetValue()->c_str();

		return IsComposedOf(componentDocumentID.c_str(), componentDocumentID.size(), componentInstanceID.c_str(), componentInstanceID.size(), componentPart);
	}

	bool APICALL ComposedAssetManagerImpl::IsComposedOf(const char * documentID, sizet docIDLength, const char * instanceID /*= NULL*/, sizet insIDLength,
		const spcIAssetPart & componentPart /*= spcIAssetPart() */ ) const
	{
		if (documentID == NULL)
			NOTIFY_ERROR(IError_base::kEDGeneral, kGECParametersNotAsExpected,
			"Parameters not as expected by ComposedAssetManagerImpl::IsDirectlyComposedOf", IError_base::kESOperationFatal, false, false);

		if (IsDirectlyComposedOf(documentID, docIDLength, instanceID, insIDLength, componentPart)) return true;

		std::string componentDocumentID(documentID);
		std::string componentInstanceID;
		if (instanceID) {
			componentInstanceID = instanceID;
		}

		for (sizet pantryCount = mComposedPantryArray->ChildCount(), pc = 1; pc <= pantryCount; ++pc) {
			spIStructureNode pantryEntry = (mComposedPantryArray->GetNodeAtIndex(pc))->ConvertToStructureNode();

			//check whether there is ingredient entry or not, if not go through and check for entry
			spIArrayNode pantryEntryIngredient = (pantryEntry->GetArrayNode(kXMP_NS_XMP_MM, AdobeXMPCommon::npos, "Ingredients", AdobeXMPCommon::npos));
			if (!pantryEntryIngredient) continue;

			for (sizet pantryEntryIngredientCount = pantryEntryIngredient->ChildCount(), pEIc = 1; pEIc <= pantryEntryIngredientCount; ++pEIc) {
				spIStructureNode pantryEntryIngredientNode = (pantryEntryIngredient->GetNodeAtIndex(pEIc))->ConvertToStructureNode();
				std::string componentDocumentIDPantryEntry, componentInstanceIdPantryEntry;
				spISimpleNode node = (pantryEntryIngredientNode->GetNode(kXMP_NS_XMP_ResourceRef, AdobeXMPCommon::npos, "documentID", AdobeXMPCommon::npos))->ConvertToSimpleNode();
				if (node)
					componentDocumentIDPantryEntry = node->GetValue()->c_str();
				node = (pantryEntryIngredientNode->GetNode(kXMP_NS_XMP_ResourceRef, AdobeXMPCommon::npos, "instanceID", AdobeXMPCommon::npos))->ConvertToSimpleNode();
				if (node)
					componentInstanceIdPantryEntry = node->GetValue()->c_str();

				if ((componentDocumentID.compare(componentDocumentIDPantryEntry) == 0)&&
					(componentInstanceID.size() ? (componentInstanceID.compare(componentInstanceIdPantryEntry) == 0) : true)) {
					if (componentPart->GetIAssetPart_I()) {
						node = (pantryEntryIngredientNode->GetNode(kXMP_NS_XMP_ResourceRef, AdobeXMPCommon::npos, "fromPart", AdobeXMPCommon::npos))->ConvertToSimpleNode();
						std::string listComponentPartValue;
						if (node) 
							listComponentPartValue = node->GetValue()->c_str();
						
						std::string componentPartValue = componentPart->GetIAssetPart_I()->SerializeAsRDFSyntax()->c_str();
						if (listComponentPartValue.compare(componentPartValue) == 0) {
							return true;
						}
					}
					else {
						return true;
					}
				}
			}
		}
		return false;
	}

	sizet APICALL ComposedAssetManagerImpl::RemoveAllComponents() __NOTHROW__ {
		spIArrayNode ingredientNode = (mComposedAssetMetadata->GetNode(kXMP_NS_XMP_MM, AdobeXMPCommon::npos, "Ingredients", AdobeXMPCommon::npos))->ConvertToArrayNode();
		
		if (ingredientNode == NULL) return 0;

		sizet ingredientNodeCount = ingredientNode->ChildCount();

		mComposedAssetMetadata->RemoveNode(kXMP_NS_XMP_MM, AdobeXMPCommon::npos, "Ingredients", AdobeXMPCommon::npos);

		spIArrayNode pantryNode = (mComposedAssetMetadata->GetArrayNode(kXMP_NS_XMP_MM, AdobeXMPCommon::npos, "Pantry", AdobeXMPCommon::npos));
		if (pantryNode)
			mComposedAssetMetadata->RemoveNode(kXMP_NS_XMP_MM, AdobeXMPCommon::npos, "Pantry", AdobeXMPCommon::npos);
		
		mRelationshipList->clear();

		return ingredientNodeCount;
	}

	ComposedAssetManagerImpl::ComposedAssetManagerImpl( const spIMetadata & composedAssetMetadata )
	try
		//: mSharedMutex( ISharedMutex_I::CreateSharedMutexIfRequired_I( multiThreadingSupport ) )
	{
		mComposedAssetMetadata = composedAssetMetadata;

		//initialize the shared pointer data
		mRelationshipList.reset(new ICompositionRelationshipList());

		//parse the given ixmpmetadata, creates relationship node from Ingredients
		//for toPart & fromPart, create AssetParts, from id, get struct from pantry
		//for other values call CompositionRelationship functions
		
		//pantry
		spINode node = mComposedAssetMetadata->GetNode(kXMP_NS_XMP_MM, AdobeXMPCommon::npos, "Pantry", AdobeXMPCommon::npos);
		if (node)
			mComposedPantryArray = node->ConvertToArrayNode();
		if (mComposedPantryArray)
			CreateIdBasedMapFromPantry(mComposedPantryArray, mComposedPantryList, mComponentCountPantryList);

		//ingredients
		node = mComposedAssetMetadata->GetNode(kXMP_NS_XMP_MM, AdobeXMPCommon::npos, "Ingredients", AdobeXMPCommon::npos);
		if (node)
			mComposedIngredientArray = node->ConvertToArrayNode();
		if (mComposedIngredientArray)
			ParseIngredientsForCompositionRelationship(mComposedAssetMetadata, mRelationshipAndIngredientPtrList, mComposedPantryList, mRelationshipList);
	} catch ( ... ) {
		NOTIFY_ERROR( IError_v1::kEDGeneral, kGECParametersNotAsExpected,
			"Parameters not as expected by ComposedAssetManagerImpl::ComposedAssetManagerImpl",
			IError_v1::kESOperationFatal, true, ( void * ) composedAssetMetadata.get() );
	}

	spIArrayNode ComposedAssetManagerImpl::GetIXMPMetaIngredientNode()
	{
		spINode node = mComposedAssetMetadata->GetNode(kXMP_NS_XMP_MM, AdobeXMPCommon::npos, "Ingredients", AdobeXMPCommon::npos);
		if (node)
			mComposedIngredientArray = node->ConvertToArrayNode();
		if (!mComposedIngredientArray) {
			mComposedIngredientArray = IArrayNode::CreateUnorderedArrayNode(kXMP_NS_XMP_MM, AdobeXMPCommon::npos, "Ingredients", AdobeXMPCommon::npos);
			mComposedAssetMetadata->InsertNode(mComposedIngredientArray);
		}
		return mComposedIngredientArray;
	}

	spIArrayNode ComposedAssetManagerImpl::GetIXMPMetaPantryNode()
	{
		spINode node = mComposedAssetMetadata->GetNode(kXMP_NS_XMP_MM, AdobeXMPCommon::npos, "Pantry", AdobeXMPCommon::npos);
		if (node)
			mComposedPantryArray = node->ConvertToArrayNode();
		if (!mComposedPantryArray) {
			mComposedPantryArray = IArrayNode::CreateUnorderedArrayNode(kXMP_NS_XMP_MM, AdobeXMPCommon::npos, "Pantry", AdobeXMPCommon::npos);
			mComposedAssetMetadata->InsertNode(mComposedPantryArray);
		}
		return mComposedPantryArray;
	}

	void ComposedAssetManagerImpl::ResetIngredientAndPantryEntry()
	{
		if (mComposedIngredientArray->ChildCount() == 0) {
			mComposedAssetMetadata->RemoveNode(kXMP_NS_XMP_MM, AdobeXMPCommon::npos, "Ingredients", AdobeXMPCommon::npos);
			mComposedAssetMetadata->RemoveNode(kXMP_NS_XMP_MM, AdobeXMPCommon::npos, "Pantry", AdobeXMPCommon::npos);
		}
	}

}

namespace AdobeXMPAM {
	using namespace AdobeXMPAM_Int;
	spIComposedAssetManager IComposedAssetManager_v1::MakeShared(pIComposedAssetManager_base ptr) {
		if (!ptr) return spIComposedAssetManager();
		pIComposedAssetManager p = IComposedAssetManager::GetInterfaceVersion() > 1 ? ptr->GetInterfacePointer< IComposedAssetManager >() : ptr;
		return MakeUncheckedSharedPointer(p, __FILE__, __LINE__, false);
	}
}
