#ifndef ExtensionHandlerImpl_h__
#define ExtensionHandlerImpl_h__ 1

// =================================================================================================
// ADOBE SYSTEMS INCORPORATED
// Copyright 2015 Adobe Systems Incorporated
// All Rights Reserved
//
// NOTICE:  Adobe permits you to use, modify, and distribute this file in accordance with the terms
// of the Adobe license agreement accompanying it.
// =================================================================================================


#include "XMPMarker/Interfaces/IExtensionHandler_I.h"
#include "XMPCore/XMPCoreFwdDeclarations.h"

#include <stack>

namespace AdobeXMPAM_Int {

	using namespace AdobeXMPAM_Int;

	using namespace AdobeXMPCore;
	using namespace AdobeXMPAM;

	class ExtensionHandlerImpl
		: public IExtensionHandler_I
	{
	public:
		ExtensionHandlerImpl( pcIExtension Extension, const std::string & ExtensionNameSpace,
			const std::string & ExtensionName, const spIStructureNode & baseNode );

		virtual uint32 BeginExtension(const char * extensionNameSpace, sizet extensionNameSpaceLen, const char * extensionName, sizet extensionNameLen) __NOTHROW__ ;
		virtual pIExtension EndExtension(const char * extensionNameSpace, sizet extensionNameSpaceLen, const char * extensionName, sizet extensionNameLen) __NOTHROW__;

		virtual uint32 BeginStructure(const char * structureNameSpace, sizet structureNameSpaceLen, const char * structureName, sizet structureNameLen) __NOTHROW__ ;
		virtual uint32 EndStructure(const char * structureNameSpace, sizet structureNameSpaceLen, const char * structureName, sizet structureNameLen) __NOTHROW__ ;

		virtual uint32 BeginArray(const char * arrayNameSpace, sizet arrayNameSpaceLen, const char * arrayName, sizet arrayNameLen) __NOTHROW__;
		virtual uint32 EndArray(const char * arrayNameSpace, sizet arrayNameSpaceLen, const char * arrayName, sizet arrayNameLen) __NOTHROW__ ;

		virtual uint32 AddKeyValuePair(const char * keyNameSpace, sizet keyNameSpaceLen, const char * keyName, sizet keyNameLen, const char * value, sizet valueLen) __NOTHROW__ ;

		virtual uint32 Serialize(pIExtension  data, pIExtensionHandler extensionCreator) const __NOTHROW__ ;

	protected:
		struct Parent {
			Parent( const spIStructureNode & structureNode );
			Parent( const spIArrayNode & arrayNode );
			spIStructureNode mStructNode;
			spIArrayNode mArrayNode;
			bool mIsArray;
		};

		std::stack< Parent >	mParents;
		const std::string &		mNameSpace;
		spIStructureNode		mBaseNode;
	};
}

#endif  // ExtensionHandlerImpl_h__
