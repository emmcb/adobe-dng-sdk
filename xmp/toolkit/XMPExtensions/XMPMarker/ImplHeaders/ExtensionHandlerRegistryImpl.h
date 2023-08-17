#ifndef ExtensionHandlerRegistryImpl_h__
#define ExtensionHandlerRegistryImpl_h__ 1

// =================================================================================================
// ADOBE SYSTEMS INCORPORATED
// Copyright 2015 Adobe Systems Incorporated
// All Rights Reserved
//
// NOTICE:  Adobe permits you to use, modify, and distribute this file in accordance with the terms
// of the Adobe license agreement accompanying it.
// =================================================================================================

#include "XMPMarker/Interfaces/IExtensionHandlerRegistry_I.h"
#include <string>
#include <map>

namespace AdobeXMPAM_Int {
	class ExtensionHandlerRegistryImpl 
		: public IExtensionHandlerRegistry
	{
	public:
		virtual bool IsHandlerRegistered( const std::string & nameSpace, const std::string & name ) const;
		virtual bool RegisterHandler( const std::string & nameSpace, const std::string & name,  pIExtensionHandler  handler );
		virtual pIExtensionHandler GetHandler( const std::string & nameSpace, const std::string & name );
		virtual pcIExtensionHandler GetHandler( const std::string & nameSpace, const std::string & name ) const;

		bool IsHandlerRegistered( const std::string & combinedName ) const;
		const std::string GetCombinedString( const std::string & nameSpace, const std::string & name ) const;

	protected:
		std::map< std::string, pIExtensionHandler >		mExtensionHandlerMap;

	};
}

#endif  // ExtensionHandlerRegistryImpl_h__
