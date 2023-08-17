#ifndef IExtensionHandler_I_h__
#define IExtensionHandler_I_h__ 1

// =================================================================================================
// ADOBE SYSTEMS INCORPORATED
// Copyright 2015 Adobe Systems Incorporated
// All Rights Reserved
//
// NOTICE:  Adobe permits you to use, modify, and distribute this file in accordance with the terms
// of the Adobe license agreement accompanying it.
// =================================================================================================

#include "XMPMarker/Interfaces/IExtensionHandler.h"
#include "XMPMarker/XMPMarkerFwdDeclarations_I.h"
#include "XMPCore/XMPCoreFwdDeclarations.h"



namespace AdobeXMPAM_Int {

	using namespace AdobeXMPAM_Int;
	
	using namespace AdobeXMPCore;
	using namespace AdobeXMPAM;

	class IExtensionHandler_I
		: public IExtensionHandler
	{
	public:


		virtual uint32 BeginExtension(const char * extensionNameSpace, sizet extensionNameSpaceLen, const char * extensionName, sizet extensionNameLen) __NOTHROW__ = 0;
		virtual pIExtension EndExtension(const char * extensionNameSpace, sizet extensionNameSpaceLen, const char * extensionName, sizet extensionNameLen) __NOTHROW__ = 0;

		virtual uint32 BeginStructure(const char * structureNameSpace, sizet structureNameSpaceLen, const char * structureName, sizet structureNameLen) __NOTHROW__ = 0;
		virtual uint32 EndStructure(const char * structureNameSpace, sizet structureNameSpaceLen, const char * structureName, sizet structureNameLen) __NOTHROW__ = 0;

		virtual uint32 BeginArray(const char * arrayNameSpace, sizet arrayNameSpaceLen, const char * arrayName, sizet arrayNameLen) __NOTHROW__ = 0;
		virtual uint32 EndArray(const char * arrayNameSpace, sizet arrayNameSpaceLen, const char * arrayName, sizet arrayNameLen) __NOTHROW__ = 0;

		virtual uint32 AddKeyValuePair(const char * keyNameSpace, sizet keyNameSpaceLen, const char * keyName, sizet keyNameLen, const char * value, sizet valueLen) __NOTHROW__ = 0;

		virtual uint32 Serialize(pIExtension  data, pIExtensionHandler extensionCreator) const __NOTHROW__ = 0;

		static pIExtensionHandler_I CreateSerializerHandler(const pcIExtension & extension, const std::string & extensionNameSpace, const std::string & extensionName, const AdobeXMPCore::spIStructureNode & baseNode);
		

	protected:
		virtual ~IExtensionHandler_I() = 0;

#ifndef FRIEND_CLASS_DECLARATION
	#define FRIEND_CLASS_DECLARATION() 
#endif
		FRIEND_CLASS_DECLARATION();
	};
		
	
	inline IExtensionHandler_I::~IExtensionHandler_I() __NOTHROW__{}

}

#endif  // IExtensionHandler_I_h__
