// =================================================================================================
// Copyright 2006-2007 Adobe
// All Rights Reserved.
//
// NOTICE:	Adobe permits you to use, modify, and distribute this file in accordance with the terms
// of the Adobe license agreement accompanying it. If you have received this file from a source other 
// than Adobe, then your use, modification, or distribution of it requires the prior written permission
// of Adobe.
// =================================================================================================

#ifndef _XMPBase_h
#define _XMPBase_h

#include <string>
#include "ScCore.h"

// only place to define the string template, XMP.hpp and XMP_Const.h !!!
#define TXMP_STRING_TYPE	std::string
#include "XMP.hpp"


// defines for JS method and property types
#define BUILTIN		ScCore::LivePropertyInfo::kAttrDontEnum | ScCore::LivePropertyInfo::kAttrDontDelete
#define METHOD		ScCore::kMethod | BUILTIN
#define ROPROP		ScCore::kReadOnly | BUILTIN
#define RWPROP		ScCore::kReadWrite | BUILTIN


// library global error code for XMP exceptions
static const scerror_t kErrXMPException = 1000;
// commonly used error messages
static const ScCore::String kErrMsgBadArgList = "Bad argument list, usage: ";
static const ScCore::String kErrMsgNoArgsExpected("No arguments expected.");

/**
Base class for all XMPScript objects.
*/
class XMPBase: public ScCore::LiveObject
{

public:
	/** constructor */
	XMPBase(const char * objectName);


	/**
	Retrieve the ID of a property. If the property exists, fill in its ID 
	and type data and return 0 for no error. Return kErrUndefined if the
	property does not exist. Use the returned ID for subsequent property 
	accesses. The type field is set to one of the enum values kReadOnly, 
	kReadWrite or kMethod. For unrecognized names, set the ID to zero
	and the type to kInvalid and return kErrUndefined. Here, you would add 
	support for properties that can only be determined at runtime.
	@see	LiveMessage
	@param	name				the property name
	@param	id					takes the ID
	@param	type				takes the type of the property
	@param	lang				the language ID
	@return						an error code (0 for no error)
	*/
	virtual	scerror_t			getIDForName (const ScCore::String& name, int32_t& id, ScCore::InfoType& type,
									  int32_t lang = ScCore::kJavaScriptLanguage) const;


	/**
	Retrieve the name of a property. Here, you would add support for
	properties that can only be determined at runtime.
	@param	id					the property ID to match
	@param	name				takes the name
	@param	lang				the language ID
	@return						an error code (0 for no error)
	*/
	virtual	scerror_t			getNameForID (int32_t id, ScCore::String& name, int32_t lang = ScCore::kJavaScriptLanguage) const;
	
	
	/**
	Retrieve the type of a property. This call returns one of the enum
	values kReadOnly, kReadWrite or kMethod. For unrecognized
	IDs, return kInvalid. Here, you would add support for
	properties that can only be determined at runtime.
	@param	id					the property ID
	@return						the type of the property
	*/
	virtual	ScCore::InfoType	getType (int32_t id) const;


	/**
	@return Returns the registered name for the LiveObject.
	*/
	virtual	void				toString (ScCore::String& result) const;


protected:

	/**
	 Converts a ScCore-String to a c-str.
	 @param	scStr	an ScCore String
	 @return		Returns a c-str.
	 */
	std::string getStr(ScCore::String scStr);


	/**
	 Converts a ScCore-Variant to a c-str.
	 @param	scStr	an ScCore String
	 @return		Returns a c-str.
	 */
	std::string getStr(ScCore::Variant scVariant);
};

#endif
