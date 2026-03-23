// =================================================================================================
// ADOBE SYSTEMS INCORPORATED
// Copyright 2015 Adobe Systems Incorporated
// All Rights Reserved
//
// NOTICE:  Adobe permits you to use, modify, and distribute this file in accordance with the terms
// of the Adobe license agreement accompanying it.
// =================================================================================================



#include "XMPMarker/ImplHeaders/ExtensionHandlerRegistryImpl.h"
#include "XMPMarker/Interfaces/IExtensionHandler_I.h"
#include <cassert>

#define TXMP_STRING_TYPE std::string
#include "XMP.incl_cpp"
#include <string>
#include <sstream>

namespace AdobeXMPAM_Int {

	using namespace AdobeXMPAM_Int;
    
    std::string ConverToString(unsigned long long num ) {
        
        std::string number;
        std::stringstream strstream;
        strstream << num;
        strstream >> number;
        return number;
    }
    
	bool ExtensionHandlerRegistryImpl::IsHandlerRegistered( const std::string & nameSpace, const std::string & name ) const {
		return IsHandlerRegistered( GetCombinedString( nameSpace, name ) );
		
	}

	bool ExtensionHandlerRegistryImpl::IsHandlerRegistered( const std::string & combinedName ) const {
		return mExtensionHandlerMap.find( combinedName ) != mExtensionHandlerMap.end();
	}

	const std::string ExtensionHandlerRegistryImpl::GetCombinedString( const std::string & nameSpace, const std::string & name ) const {
		std::string combinedString( nameSpace );
		combinedString.append( ":" ).append( name );
		return combinedString;
	}

	bool ExtensionHandlerRegistryImpl::RegisterHandler( const std::string & nameSpace, const std::string & name, pIExtensionHandler  handler ) {
	
		std::string combinedString = GetCombinedString( nameSpace, name );
		if ( handler ) {
			if ( IsHandlerRegistered( combinedString ) ) return false;
			
			mExtensionHandlerMap.insert( std::make_pair( combinedString, handler ) );
			std::string registeredPrefix;
			if ( !SXMPMeta::GetNamespacePrefix( nameSpace.c_str(), &registeredPrefix) ) {
				static long long unsigned int count( 1 );
				std::string prefix( "cm" );
				prefix.append( ConverToString( count ) );
				SXMPMeta::RegisterNamespace( nameSpace.c_str(), prefix.c_str(), &registeredPrefix );
				count++;
			}
			return true;
		} else {
			if ( IsHandlerRegistered( combinedString ) ) {
				mExtensionHandlerMap.erase( combinedString );
				return true;
			}
			return false;
		}
	}

	pIExtensionHandler ExtensionHandlerRegistryImpl::GetHandler( const std::string & nameSpace, const std::string & name ) {
		std::string combinedString = GetCombinedString( nameSpace, name );
		if ( IsHandlerRegistered( combinedString ) ) {
			return mExtensionHandlerMap.find( combinedString )->second;
		}
		return NULL;
	}

	pcIExtensionHandler ExtensionHandlerRegistryImpl::GetHandler( const std::string & nameSpace, const std::string & name ) const {
		return const_cast< ExtensionHandlerRegistryImpl * >( this )->GetHandler( nameSpace, name );
	}

	static ExtensionHandlerRegistryImpl * sExtensionHandlerRegistryImplPtr( NULL );

	pIExtensionHandlerRegistry IExtensionHandlerRegistry::CreateInstance() {
		assert( sExtensionHandlerRegistryImplPtr == NULL );
		sExtensionHandlerRegistryImplPtr = new ExtensionHandlerRegistryImpl();
		return sExtensionHandlerRegistryImplPtr;
	}

	pIExtensionHandlerRegistry IExtensionHandlerRegistry::GetInstance() {
		if ( sExtensionHandlerRegistryImplPtr )
			return sExtensionHandlerRegistryImplPtr;
		else
			return CreateInstance();
	}

	pcIExtensionHandlerRegistry IExtensionHandlerRegistry::GetConstInstance() {
		return GetInstance();
	}

	void IExtensionHandlerRegistry::DestroyInstance() {
		if ( sExtensionHandlerRegistryImplPtr )
			delete sExtensionHandlerRegistryImplPtr;
		sExtensionHandlerRegistryImplPtr = NULL;
	}

}
