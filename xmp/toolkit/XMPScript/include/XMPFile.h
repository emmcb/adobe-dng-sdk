// =================================================================================================
// Copyright 2006-2007 Adobe
// All Rights Reserved.
//
// NOTICE:	Adobe permits you to use, modify, and distribute this file in accordance with the terms
// of the Adobe license agreement accompanying it. If you have received this file from a source other 
// than Adobe, then your use, modification, or distribution of it requires the prior written permission
// of Adobe.
// =================================================================================================

#ifndef _XMPFile_h
#define _XMPFile_h

#include "XMPBase.h"


/**
Provides XMPFiles functionality as LiveObject.
*/
class XMPFile : public XMPBase
{
private:
	/** the properties manager */
	ScCore::LivePropertyManager	properties;
	/** Name of the object, unused at the moment */
	ScCore::String				name;
	/** the wrapped metadata object */
	SXMPFiles					file;



public:
	///** Constructor with initial values */
	//XMPFile(ScCore::String filePath, int format, int openFlags);

	/** Default constructor */
	XMPFile();


	/**
	Initialize this object. ExtendScript calls this method after creating a new 
	object by calling the factory object's clone() method. Its arguments are the same
	as passed in to the "new Object()" call.
	*/
	virtual	scerror_t			initialize (const ScCore::Array& args, ScCore::Error* error = null);


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

	virtual void				setName(ScCore::String name) const;
};

#endif
