// =================================================================================================
// Copyright 2006-2007 Adobe
// All Rights Reserved.
//
// NOTICE:	Adobe permits you to use, modify, and distribute this file in accordance with the terms
// of the Adobe license agreement accompanying it. If you have received this file from a source other 
// than Adobe, then your use, modification, or distribution of it requires the prior written permission
// of Adobe.
// =================================================================================================

#ifndef _XMPFileInfo_h
#define _XMPFileInfo_h

#include <string>
#include "ScCore.h"

#include "XMPBase.h"


/**
Information for an open file in XMPFiles.
*/
class XMPFileInfo : public XMPBase
{
	/** the properties manager */
	ScCore::LivePropertyManager	properties;

public:
	/** Constructor with initial values */
	XMPFileInfo(std::string filePath, XMP_OptionBits openFlags, int format, XMP_OptionBits handlerFlags);


	/**
	Read the value of a property. Return 0 if the property could
	be read, an error code otherwise.
	@param	id					the property ID
	@param	value				the location to take the value
	@param	errors				an optional pointer to extended error information
	@return						kErrUndefined
	*/
	virtual	scerror_t			get (int32_t id, ScCore::Variant& value, ScCore::Error* errors = null) const;


	/**
	Return the string "[PacketInfo]".
	*/
	virtual	void				toString (ScCore::String& result) const;


private:
	/** Contains the absolute path of the file */
	std::string filePath;
	/** Contains the open flags */
	XMP_OptionBits openFlags;
	/** Contains format of the opened files */
	int format;
	/** Contains the flags of the smart handler */
	XMP_OptionBits handlerFlags;
};

#endif
