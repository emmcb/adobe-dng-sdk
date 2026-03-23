#ifndef __XMPRelationshipNode_Impl_h__
#define __XMPRelationshipNode_Impl_h__ 1

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
#include "XMPAssetManagement/Interfaces/IXMPRelationshipNode.h"
#include "XMPAssetManagement\ImplHeaders\XMPResourceRef_Impl.h"


namespace AdobeXMPAM {
	class XMPRelationshipNode_Impl : public IXMPRelationshipNode {
	public:
		XMPRelationshipNode_Impl();
		XMPRelationshipNode_Impl(spIXMPPart toPart, AdobeXMPCommon::spIXMPMetadata fromDoc, spIXMPPart fromPart, std::string mapping, AdobeXMPAM::spIXMPResourceRef spResRef);

		spIXMPPart GetToPart(void);

		AdobeXMPCommon::spIXMPMetadata GetFromPartIXMPMetadata(void);

		spIXMPPart GetFromPart(void);

		std::string GetMapping(void);
		void SetMapping(std::string);

		void Set(kResourceRefEnum, std::string);
		std::string Get(kResourceRefEnum) const;

		AdobeXMPAM::spIXMPResourceRef GetResourceRef(void);
	//	friend class Iterator;
	//	friend class XMPDocumentRelationship_Impl;
	
	private:
		spIXMPPart toPart;
		AdobeXMPCommon::spIXMPMetadata fromdoc;
		spIXMPPart fromPart;
		std::string mapping;
		AdobeXMPAM::spIXMPResourceRef spResourceRef;
		//XMPResourceRef_Impl mResourceRef;
	};
};
#endif	// __XMPRelationshipNode_Impl_h__
