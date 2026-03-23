// =================================================================================================
// ADOBE SYSTEMS INCORPORATED
// Copyright 2015 Adobe Systems Incorporated
// All Rights Reserved
//
// NOTICE:  Adobe permits you to use, modify, and distribute this file in accordance with the terms
// of the Adobe license agreement accompanying it.
// =================================================================================================

#include "XMPCore/Interfaces/ICoreObjectFactory.h"
#include "XMPMarker/ImplHeaders/ExtensionHandlerImpl.h"
#include "XMPCore/Interfaces/IArrayNode.h"
#include "XMPCore/Interfaces/IStructureNode.h"
#include "XMPCore/Interfaces/ISimpleNode.h"
#include "XMPCommon/Interfaces/IUTF8String.h"
#include <assert.h>

namespace AdobeXMPAM_Int {

	using namespace	AdobeXMPAM;
	using namespace AdobeXMPAM_Int;
	using namespace AdobeXMPCore;
	using namespace AdobeXMPCommon;
	using namespace AdobeXMPCommon_Int;

	
	ExtensionHandlerImpl::ExtensionHandlerImpl( pcIExtension Extension, const std::string & ExtensionNameSpace,
		const std::string & ExtensionName, const spIStructureNode & baseNode )
		: mNameSpace( ExtensionNameSpace )
		, mBaseNode( baseNode )
	{
		spIStructureNode node = IStructureNode::CreateStructureNode( mNameSpace.c_str(), mNameSpace.length(), ExtensionName.c_str(), ExtensionName.length() );
		mParents.push( Parent( node ) );
	}

	uint32 ExtensionHandlerImpl::BeginExtension(const char * extensionNameSpace, sizet extensionNameSpaceLen, const char * extensionName, sizet extensionNameLen) __NOTHROW__{
		return true;
	}

	pIExtension ExtensionHandlerImpl::EndExtension(const char * extensionNameSpace, sizet extensionNameSpaceLen, const char * extensionName, sizet extensionNameLen) __NOTHROW__{
		assert( mParents.size() == 1 );
		mBaseNode->InsertNode( mParents.top().mStructNode );
		mParents.pop();
		return NULL;
	}

	uint32 ExtensionHandlerImpl::BeginStructure(const char * structureNameSpace, sizet structureNameSpaceLen, const char * structureName, sizet structureNameLen) __NOTHROW__{
		spIStructureNode node = IStructureNode::CreateStructureNode( structureNameSpace, structureNameSpaceLen, structureName, structureNameLen );
		mParents.push( Parent( node ) );
		return true;
	}

	uint32 ExtensionHandlerImpl::EndStructure(const char * structureNameSpace, sizet structureNameSpaceLen, const char * structureName, sizet structureNameLen) __NOTHROW__{
		assert( mParents.top().mIsArray == false );
		assert( strncmp(structureName, mParents.top().mStructNode->GetName()->c_str(), structureNameLen ) == 0 );
		assert( mParents.size() > 1 );

		auto node = mParents.top().mStructNode;
		mParents.pop();
		if ( mParents.top().mIsArray )
			mParents.top().mArrayNode->AppendNode( node );
		else
			mParents.top().mStructNode->AppendNode( node );
		return true;
	}

	uint32 ExtensionHandlerImpl::BeginArray(const char * arrayNameSpace, sizet arrayNameSpaceLen, const char * arrayName, sizet arrayNameLen) __NOTHROW__{
		spIArrayNode node = IArrayNode::CreateUnorderedArrayNode( arrayNameSpace, arrayNameSpaceLen, arrayName, arrayNameLen );
		mParents.push( Parent( node ) );
		return true;
	}

	uint32 ExtensionHandlerImpl::EndArray(const char * arrayNameSpace, sizet arrayNameSpaceLen, const char * arrayName, sizet arrayNameLen) __NOTHROW__{
		assert( mParents.top().mIsArray == true );
		//TODO check
		assert(strncmp(arrayName, mParents.top().mArrayNode->GetName()->c_str(), arrayNameLen ) == 0);
		assert( mParents.size() > 1 );

		auto node = mParents.top().mArrayNode;
		mParents.pop();
		if ( mParents.top().mIsArray )
			mParents.top().mArrayNode->AppendNode( node );
		else
			mParents.top().mStructNode->AppendNode( node );
		return true;
	}

	uint32 ExtensionHandlerImpl::AddKeyValuePair(const char * keyNameSpace, sizet keyNameSpaceLen, const char * keyName, sizet keyNameLen, const char * value, sizet valueLen) __NOTHROW__  {
		if ( mParents.top().mIsArray )
			mParents.top().mArrayNode->AppendNode( ISimpleNode::CreateSimpleNode( keyNameSpace, keyNameSpaceLen, keyName, keyNameLen, value, valueLen ) );
		else
			mParents.top().mStructNode->AppendNode( ISimpleNode::CreateSimpleNode( keyNameSpace, keyNameSpaceLen, keyName, keyNameLen, value, valueLen ) );
		return true;
	}

	uint32 ExtensionHandlerImpl::Serialize( pIExtension  data, pIExtensionHandler ExtensionCreator ) const __NOTHROW__{
		return false;
	}

	ExtensionHandlerImpl::Parent::Parent( const spIStructureNode & structureNode )
		: mStructNode( structureNode )
		, mIsArray( false ) { }

	ExtensionHandlerImpl::Parent::Parent( const spIArrayNode & arrayNode )
		: mArrayNode( arrayNode )
		, mIsArray( true ) { }

	pIExtensionHandler_I IExtensionHandler_I::CreateSerializerHandler(const pcIExtension & extension, const std::string & extensionNameSpace,
		const std::string & extensionName, const AdobeXMPCore::spIStructureNode & baseNode)

	{
		return new ExtensionHandlerImpl(extension, extensionNameSpace, extensionName, baseNode);
	}
}

