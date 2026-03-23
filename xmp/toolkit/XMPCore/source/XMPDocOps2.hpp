#if ! __XMPDocOps2_hpp__
#define __XMPDocOps2_hpp__ 1

// =================================================================================================
// Copyright 2007 Adobe
// All Rights Reserved.
//
// NOTICE:  Adobe permits you to use, modify, and distribute this file in accordance with the terms
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
// AJ Abhishek Jindal
// 
//
// mm-dd-yy who Description of changes, most recent on top.
//
//
// 12-21-14 AJ  5.6-c031 Routed XMPDocOps to new DOM APIs.
//
// =================================================================================================
#endif // AdobePrivate

#include "public/include/XMP_Environment.h"
#include "public/include/XMP_Const.h"
#include "XMPCore/source/XMPCore_Impl.hpp"

#include "XMPCore/source/XMPMeta.hpp"
#include "XMPCore/source/XMPDocOps.hpp"

// =================================================================================================

class XMPDocOps2 :public XMPDocOps {
public:

	
	XMPDocOps2();
	virtual ~XMPDocOps2() RELEASE_NO_THROW;

	
	virtual XMP_TriState HasPartChanged ( XMP_StringPtr   prevInstanceID,
								  XMP_StringPtr * partsPtr,
								  XMP_Index       partsCount ) const;

	virtual bool GetPartChangeID ( XMP_StringPtr * partsPtr,
						   XMP_Index       partsCount,
						   XMP_StringPtr * idStr,
						   XMP_StringLen * idLen ) const;

	
private:

	virtual void PruneRedundantHistory();
	

};	// XMPDocOps2

#endif  // __XMPDocOps2_hpp__
