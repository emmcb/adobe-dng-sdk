#ifndef __XMPResourceRef_Impl_h__
#define __XMPResourceRef_Impl_h__ 1

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
#include <vector>
#include <string>
#include <cstring>
#include <algorithm>

// Ensure XMP templates are instantiated
#define TXMP_STRING_TYPE std::string 

// Provide access to the API
#include "public/include/XMP.hpp"
#include "XMPAssetManagement/XMPAssetManagementFwdDeclarations.h"
#include "XMPAssetManagement\Interfaces\IXMPResourceRef.h"

namespace AdobeXMPAM { 

	typedef struct {
		std::string name;
		std::string value;
	} ResourceRefData;

	class XMPResourceRef_Impl : public IXMPResourceRef {
	public:
		//constructor
		XMPResourceRef_Impl();

		void Set(kResourceRefEnum, std::string);
		std::string Get(kResourceRefEnum) const;

		std::string GetName(kResourceRefEnum) const;
		std::string GetType(kResourceRefEnum) const;

	private:
		std::vector<ResourceRefData> mResourceRefValueVec;		
	}; // class __XMPResourceRef_Impl_h__
}

#endif	// __XMPResourceRef_h__
