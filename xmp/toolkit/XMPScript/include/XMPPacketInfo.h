// =================================================================================================
// Copyright 2006-2007 Adobe
// All Rights Reserved.
//
// NOTICE:	Adobe permits you to use, modify, and distribute this file in accordance with the terms
// of the Adobe license agreement accompanying it. If you have received this file from a source other 
// than Adobe, then your use, modification, or distribution of it requires the prior written permission
// of Adobe.
// =================================================================================================

#ifndef _XMPPacketInfo_h
#define _XMPPacketInfo_h

#include "XMPBase.h"


/**
Contains information about an XMPPacket after it has been retrieved from a file.
*/
class XMPPacketInfo : public XMPBase
{
	/** the properties manager */
	ScCore::LivePropertyManager	properties;

public:
	/** Constructor with initial values */
	XMPPacketInfo(std::string xmpStr, XMP_PacketInfo xmpPacketInfo);


	/**
	Read the value of a property. Return 0 if the property could
	be read, an error code otherwise.
	@param	id					the property ID
	@param	value				the location to take the value
	@param	errors				an optional pointer to extended error information
	@return						kErrUndefined
	*/
	virtual	scerror_t			get (int32_t id, ScCore::Variant& value, ScCore::Error* errors = null) const;

private:
	/** Contains the serialized XMP String */
	std::string xmpStr;
	/** Contains the packet info */
	XMP_PacketInfo packetInfo;
};

#endif
