#ifndef __XMPDocumentRelationship_Impl_h__
#define __XMPDocumentRelationship_Impl_h__ 1

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

#include <cstdio>
#include <string>
#include <cstring>
#include <list>

// Ensure XMP templates are instantiated
#define TXMP_STRING_TYPE std::string 

// Provide access to the API
#include "public/include/XMP.hpp"
#include "public/include/XMPAssetManagement/DOMAPI.h"
#include "XMPAssetManagement\XMPAssetManagementFwdDeclarations.h"
#include "XMPAssetManagement/Interfaces/IXMPDocumentRelationship.h"
#include "XMPAssetManagement/Interfaces/IXMPRelationShipNode.h"

namespace AdobeXMPAM {
	class XMPDocumentRelationship_Impl /* : public IXMPDocumentRelationship */ {
	public:
		spIXMPRelationshipNode CreateRelation(spIXMPPart toPart, AdobeXMPCommon::spIXMPMetadata fromDoc, spIXMPPart fromPart, std::string mapping, AdobeXMPAM::spIXMPResourceRef);
		void ProcessIngredientListData(AdobeXMPCore::spIStructureNode xmpdoc);
		spIXMPRelationshipNodeList Find(spIXMPPart topart, spIXMPDocument xmpfromdoc, spIXMPPart frompart);
		bool RemovePart(spIXMPPart topart, spIXMPDocument xmpfromdoc, spIXMPPart frompart);
		
		spIXMPRelationshipNodeList GetRelatioinshipList(void);

	private:
		spIXMPRelationshipNodeList  RelationshipList;
	public:

		~XMPDocumentRelationship_Impl();
	};
	inline XMPDocumentRelationship_Impl::~XMPDocumentRelationship_Impl() {}
};
#endif	// __XMPDocumentRelationship_Impl_h__
