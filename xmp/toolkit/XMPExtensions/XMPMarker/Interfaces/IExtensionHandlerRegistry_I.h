#ifndef IExtensionHandlerRegistry_h__
#define IExtensionHandlerRegistry_h__  1

#include "XMPMarker/XMPMarkerFwdDeclarations_I.h"

namespace AdobeXMPAM_Int {

	using namespace AdobeXMPAM_Int;
	using namespace AdobeXMPAM;

	class IExtensionHandlerRegistry {
	public:
		virtual bool IsHandlerRegistered( const std::string & nameSpace, const std::string & name ) const = 0;

		virtual bool RegisterHandler( const std::string & nameSpace, const std::string & name, pIExtensionHandler  handler ) = 0;

		virtual pIExtensionHandler GetHandler( const std::string & nameSpace, const std::string & name ) = 0;
		virtual pcIExtensionHandler GetHandler( const std::string & nameSpace, const std::string & name ) const = 0;

		static pIExtensionHandlerRegistry GetInstance();
		static pcIExtensionHandlerRegistry GetConstInstance();

		static pIExtensionHandlerRegistry CreateInstance();
		static void DestroyInstance();

		virtual ~IExtensionHandlerRegistry() = 0;

	};

	inline IExtensionHandlerRegistry::~IExtensionHandlerRegistry() { }

}
#endif  // IExtensionHandlerRegistry_h__
