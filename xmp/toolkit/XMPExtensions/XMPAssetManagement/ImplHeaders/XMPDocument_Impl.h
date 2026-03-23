#ifndef __XMPDocument_Impl_h__
#define __XMPDocument_Impl_h__ 1

// =================================================================================================
// ADOBE SYSTEMS INCORPORATED
// Copyright 2014 Adobe Systems Incorporated
// All Rights Reserved
//
// NOTICE: Adobe permits you to use, modify, and distribute this file in accordance with the terms
// of the Adobe license agreement accompanying it.
// =================================================================================================

#if AdobePrivate
// =================================================================================================
// Change history
// ==============
//
// Writers:
//  SKP Sunil Kishor Pathak
//
// mm-dd-yy who Description of changes, most recent on top
//
// 09-18-14 SKP 1.0 First draft for Asset Relationships API.
//
// =================================================================================================
#endif // AdobePrivate
// Ensure XMP templates are instantiated
#include <string>
#include <map>
#include <set>

// Provide access to the API
#include "public/include/XMP.hpp"

#include "XMPAssetManagement/DOMAPI.h"
#include "XMPAssetManagement/XMPAssetManagementFwdDeclarations.h"
#include "XMPAssetManagement/Interfaces/IXMPDocument.h"
#include "XMPAssetManagement\ImplHeaders\XMPDocumentRelationship_Impl.h"
//#include "XMPDocumentPart_Impl.h"
//using namespace NS_XMPASSET MANAGEMENT;

namespace AdobeXMPAM {
class XMPDocument_Impl : public IXMPDocument
	/*: public virtual NS_INT_XMPCOMMON::IntrusiveRefCountSharedObject */ {
public:
	XMPDocument_Impl() { xmpDocument.reset(); assetRelationships =  new XMPDocumentRelationship_Impl(); m_hasIngredientDataChanged = false; m_hasPantryDataChanged=false;}
	XMPDocument_Impl(AdobeXMPCore::spIXMPMetadata xmpDocument, DocumentCreateOptions opts = kXMP_None, bool *clearStructures=0 /*,const ParentConstituentInfo* parentPantryEntries=0*/);

	/*
	** OpenDocument : Given an IXMPMetadata object, return an XMP_Document
	**		- assign xmpDom to xmpDocuement
	**		- Check for docIds, if not create it
	*/
	virtual void Open( AdobeXMPCore::spIXMPMetadata xmpDoc, DocumentCreateOptions opts = kXMP_None );

	/*
	** Derive : create and return the newly created IXMPMetadata
	**		- Create a new IXMPDocument
	**		- Add initial properties if given any
	**		- Create DerivedFrom
	**		- Modify History entry
	**		- Keep original document Id, but new document ID and instance id
	*/
	virtual spIXMPMetadata Derive(/*, NS_XMPCORE::spIStructureNode initialProps*/);



	/*
	** GetDocumentIdentifier :instanceid has been used as document identifier, if found return else throw exception
	**		- Get "instance Id" and return it
	**		- else throw error
	*/
	virtual std::string GetIdentifier();

	/*
	** UpdateDocumentProperties : Update the top level properties
	**		- User has passed a structure of top level properties, parse those and add in this->xmpDocument
	*/
	virtual void UpdateProperties( AdobeXMPCore::spIStructureNode initialProps );

	/*
	** DocumentUpdated : CURRENTLY THERE IS NO NEED OF THIS FUNCTION
	*/
	virtual void Updated(AdobeXMPCore::spIStructureNode eventInfo );
	 
	/*
	** SaveDocument : If the document is dirty, save document will change the IXMPMetadata i.e. this-xmpDocument
    ** There are two list, one multimap<key, ingredients> for ingredients and other map<key, pantry> for pantry
    **
    ** What about the ingredient : append these all ingredients entry in the ingredients field
	*/
	virtual void Save();

	/*
	** 
	*/
	//virtual NS_XMPCORE::spIXMPMetadata Destroy();

	/*
	** GetDocumentRelationship() : Return the relationship list
	** Relationship is a list of IXMPRelationshipNode {toPart, fromPartDoc, fromPart, mapping}
	** User can iterate it and for each RelationshipNode user can call GetToPart()/GetFromPart() API etc
	*/
	virtual spIXMPRelationshipNodeList GetRelationshipList();


	/*
	** Add Part
	*/
	virtual void CreateRelationship(spIXMPPart toPart, AdobeXMPCommon::spIXMPMetadata fromDoc, spIXMPPart fromPart, std::string mapping, AdobeXMPAM::spIXMPResourceRef);


	//virtual bool IsPartPresent(std::string toPart, std::string toPartQualifier, NS_XMPCORE::spIXMPMetadata fromPartDoc = nullptr, std::string fromPartValue="", std::string fromPartQualifier="", std::string mapping="");

	/*
	** IsPartPresent (XMPDocumentPart toPart, XMPDocumentPart fromPart)
	**	Search both toPart and fromPart documentPart
	**	fromPartResourceRef: All the kResourceRefEnum values which user wants to match
	*/
	//virtual bool IsPartPresent(std::string toPart, std::string toPartQualifier, spIXMPDocumentPart spFromPartDoc, XMP_Uns16 fromPartResourceRef);


	/*
	** RemovePart(std::string toPart, std::string toPartQualifier, option = 0) 
	**	Remove the toPart entry which has given qualifier
	**
	**	fromPartResourceRef:
	**		All the kResourceRefEnum values which user wants to match
	**	Option : 
	**		0 : First found entry (default)
	**		1 : All found entry
	*/
	//virtual void RemovePart(std::string toPart, std::string toPartQualifier, NS_XMPCORE::spIXMPMetadata fromPartDoc = nullptr, std::string fromPartValue="", std::string fromPartQualifier="", XMP_Uns16 fromPartResourceRef = 0, bool option = 0);

	/*
	** RemovePart(std::string toPart, std::string toPartQualifier, spXMPDocumentPart fromPart, options = 0) 
	**	Match the toPart, toPartQualifier and everyProperty of the XMPDocumentpart (document ID, instance ID,
	**		all ResourceRef)
	**
	**	fromPartResourceRef:
	**		All the kResourceRefEnum values which user wants to match
	**	Option:
	**		0 : First found entry (default)
	**		1 : All found entry
	**	
	*/
	//virtual void RemovePart(std::string toPart, std::string toPartQualifier, spIXMPDocumentPart spFromPartDoc, , XMP_Uns16 fromPartResourceRef = 0, bool options=0);

	//FUTURE
	/*
	** std::vector<XMPDocumentPart>& GetToPartList(XMPDocumentPart fromPart);
	**	FromPart XMPDocumentPart has been given, return the vector of all
	**	toPart XMPDocumentPart which has included this fromPart
	*/
	//Need to this more, what will be the use of instance id, why not XMPDocumentPart
	//	std::vector<XMPDocumentPart>& GetToPartList(XMPDocumentPart fromPart);

	//RegisterFunction : Which will be called when some value of toPart or fromPart is changed

	//Notify : Notify the above registered functions


	//Pantry related functions
	/*
	** std::string GetParentID(std::string documentID);
	**	Search in pantry and return the instance id of the document which has included it
	*/
	//std::string GetParentID(std::string documentID);


	/*
	** FindToPart : A part has been given, find it and return the relationship node {toPart, fromIXMPDoc, fromPart, "mapping"}
	**		If some field is not required to be searched set it NULL
	*/
	virtual spIXMPRelationshipNodeList FindRelationship(spIXMPPart topart, spIXMPDocument xmpfromdoc, spIXMPPart frompart);

	
	/*
	** Remove part entry
	**	If topart given, remove all part entry {topart, fromdoc, frompart, ...} which mataches the given one
	**	Options are : topart, xmpfromdoc, xmpfromdoc+frompart
	*/

	virtual bool RemoveRelationship(spIXMPPart topart, spIXMPDocument xmpfromdoc, spIXMPPart frompart);

	/*
	** GetIXMPMetadata : Return the actual IXMPMetadata
	*/

	virtual AdobeXMPCore::spIXMPMetadata  GetIXMPMetadata(void);

	AdobeXMPCore::spIStructureNode _getXMPDocument();

	virtual ~XMPDocument_Impl();

	static void _addNewDocProperties(AdobeXMPCore::spIStructureNode ixmpDocStruct);
	static std::string getCurrentDateString(void);
	static void getCurrentDateString(std::string& dateStr);
private:	
	/*static*/ void _OpenDocument( AdobeXMPCore::spIXMPMetadata xmpDom, 	DocumentCreateOptions opts ,bool *clearStructures /*,const ParentConstituentInfo * parentPantryEntries*/);
	void _createInternalComposedStructure(bool *clearStructures = 0 /*, const ParentConstituentInfo * parentPantryEntries = 0*/);

	//when document is opened, parse the pantry and fill the pantry list map; key will be documentid+instanceid
	void _ParsePantry(void);
	std::map<std::string, AdobeXMPCore::spIStructureNode> documentPantryMap;
	//parse ingredients and for each entry, create a relationshipnode
	void _ParseIngredients(void);

	//take the type and value, create IXMPxxxPart and return IXMPPart of that
	spIXMPPart _CreatePartFromTypeAndValue(std::string type, std::string value);


	bool _getLastModTime(XMP_DateTime& lastModDate);
	
	bool _forceDocTrackingIds();
	void _moveDocTrackingIds();
	void _copyTopLevelProperties(AdobeXMPCore::spIStructureNode parent, AdobeXMPCore::spIStructureNode initialProps, bool moveDates=false);

	//Private methods to serialize pantry and ingredients
	void _SerializeAllConstituents( AdobeXMPCore::spIArrayNode pantryArray, std::set<std::string>& serializedInstances );
	void _SerializetopLevelConstituents( AdobeXMPCore::spIArrayNode ingredientsArrNode );

	void _addConstituentSpec( AdobeXMPCore::spIStructureNode resourceRef ) ;

	std::string _getIdentifier(const char* idName);
	

	//Returns true if all the constituents were successfully read 
	void _createConstituentDocs( AdobeXMPCore::spIStructureNode currentxmpDoc, bool *clearStructures/*,const ParentConstituentInfo * parentPantryEntries*/);

	XMPDocument_Impl(const std::string& packet, DocumentCreateOptions opts = kXMP_None);
	
	XMPDocument_Impl(const XMPDocument_Impl& xmpDoc);
	XMPDocument_Impl operator= (const XMPDocument_Impl& indoc);

	DocumentCreateOptions docFlags;
	AdobeXMPCore::spIStructureNode xmpDocument;
//	NS_XMPCORE::spIStructureNode xmpTemp;

 
	bool dirty;
	bool contentChanged;
	bool constituentsChanged;

    bool         m_hasIngredientDataChanged;
	bool         m_hasPantryDataChanged;


	//list of relationship node
	XMPDocumentRelationship_Impl* assetRelationships;
  


    void AppendHistory ( XMP_StringPtr action, XMP_StringPtr params=0);
	//	void AppendHistory ( XMP_StringPtr action, XMP_StringPtr params, XMP_StringPtr instanceID, XMP_StringPtr when );
	}; // class XMP_Document
} //namespace AdobeXMPAM

// =================================================================================================

#endif	// __XMPDocument_Impl_h__
