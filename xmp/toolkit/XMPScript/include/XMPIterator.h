// =================================================================================================
// Copyright 2006-2007 Adobe
// All Rights Reserved.
//
// NOTICE:	Adobe permits you to use, modify, and distribute this file in accordance with the terms
// of the Adobe license agreement accompanying it. If you have received this file from a source other 
// than Adobe, then your use, modification, or distribution of it requires the prior written permission
// of Adobe.
// =================================================================================================

#ifndef _XMPIterator_h
#define _XMPIterator_h

#include "XMPBase.h"
#include "XMPProperty.h"


/**
The XMPIterator object is created by a call to iterator(...) of an XMPMeta-instance. 
It can not be instantiated by itself. It walks recursively through the properties 
and qualifiers of the metadata object and returns them as XMPPropery objects. 
To abbreviate the browsing, the functions skipSubtree() and skipSiblings() can be used.
*/
class XMPIterator : public XMPBase
{
	/** the properties manager */
	ScCore::LivePropertyManager	properties;
	/** Contains the actual iterator */
	SXMPIterator iterator;
	/** Contains the iterator options */
//	int iteratorOptions;
	/** Contains the namespace of a property */
//	ScCore::String schemaNS;
	/** Contains the name of a property */
//	ScCore::String name;


public:
	/** Constructor with meta-object and all optional: options, schema namespace and property name */
	XMPIterator(SXMPMeta meta, int iteratorOptions, ScCore::String schemaNS, ScCore::String propName);


	/**
	Call a method. Return 0 if the method could be called, an error code otherwise.
	@param	id					the property ID
	@param	args				the method arguments
	@param	result				the return value.
	@param	errors				an optional pointer to extended error information
	@return						kErrUndefined
	*/
	virtual	scerror_t			call (int32_t id, const ScCore::Array& args, 
									  ScCore::Variant& result, ScCore::Error* errors = null);
};

#endif
