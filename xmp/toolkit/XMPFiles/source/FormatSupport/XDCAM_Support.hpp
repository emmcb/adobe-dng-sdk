#ifndef __XDCAM_Support_hpp__
#define __XDCAM_Support_hpp__	1

// =================================================================================================
// Copyright Adobe
// Copyright 2008 Adobe
// All Rights Reserved
//
// NOTICE: Adobe permits you to use, modify, and distribute this file in accordance with the terms
// of the Adobe license agreement accompanying it. If you have received this file from a source other 
// than Adobe, then your use, modification, or distribution of it requires the prior written permission
// of Adobe.
// =================================================================================================

#if AdobePrivate
// =================================================================================================
// Change history
// ==============
//
// Writers:
//	GEM Gerry Miller
//	AWL Alan Lillich
//
// mm-dd-yy who Description of changes, most recent on top
//
// 11-24-10	AWL	5.3-f020 [2755623,2755632] Add XDCAM fixes for non-XMP metadata problems.
//
// 02-05-08 AWL 4.2-f069 Integrate latest advanced video handlers.
// 01-11-08 GEM First draft.
//
// =================================================================================================
#endif // AdobePrivate

#include "public/include/XMP_Environment.h"	// ! This must be the first include.
#include "public/include/XMP_Const.h"
#include "XMPFiles/source/XMPFiles_Impl.hpp"
#include "source/ExpatAdapter.hpp"

// =================================================================================================
/// \file XDCAM_Support.hpp
/// \brief XMPFiles support for XDCAM streams.
///
// =================================================================================================

namespace XDCAM_Support 
{

	// Read XDCAM XML metadata from MEDIAPRO.XML and translate to appropriate XMP.
	bool GetMediaProLegacyMetadata ( SXMPMeta * xmpObjPtr,
									 const std::string&	umid,
									 const std::string& mediaProPath,
									 bool digestFound);

	// Read XDCAM XML metadata and translate to appropriate XMP.
	bool GetLegacyMetadata ( SXMPMeta *		xmpObjPtr,
							 XML_NodePtr	rootElem,
							 XMP_StringPtr	legacyNS,
							 bool			digestFound,
							 std::string&	umid );

	// Write XMP metadata back to XDCAM XML.
	bool SetLegacyMetadata ( XML_Node *		clipMetadata,
							 SXMPMeta *		xmpObj,
							 XMP_StringPtr	legacyNS );


} // namespace XDCAM_Support

// =================================================================================================

#endif	// __XDCAM_Support_hpp__
