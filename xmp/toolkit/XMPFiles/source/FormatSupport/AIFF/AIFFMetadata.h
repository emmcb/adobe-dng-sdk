// =================================================================================================
// Copyright Adobe
// Copyright 2010 Adobe
// All Rights Reserved
//
// NOTICE: Adobe permits you to use, modify, and distribute this file in accordance with the terms
// of the Adobe license agreement accompanying it. If you have received this file from a source other 
// than Adobe, then your use, modification, or distribution of it requires the prior written permission
// of Adobe.
// =================================================================================================

#ifndef _AIFFMetadata_h_
#define _AIFFMetadata_h_

#if AdobePrivate
// =================================================================================================
// Change history
// ==============
//
// Writers:
//	JEH Joerg Ehrlich
//
// mm-dd-yy who Description of changes, most recent on top
//
// 10-26-10 PW  5.3-f015 Integration from winwood
//							New design for Reconciliation code
// 08-18-10 AWL 5.3-f002 Don't include XIO.hpp in any headers, only .cpp files.
// 08-17-10 AWL 5.3-f001 Integrate I/O revamp to main.
// 07-28-10	JEH	5.2-f001 Initial creation of new AIFF handler.

//
// =================================================================================================
#endif // AdobePrivate

#include "public/include/XMP_Environment.h"	// ! XMP_Environment.h must be the first included header.

#include "public/include/XMP_Const.h"
#include "public/include/XMP_IO.hpp"

#include "XMPFiles/source/XMPFiles_Impl.hpp"
#include "source/XMPFiles_IO.hpp"

#include "source/XMP_LibUtils.hpp"

#include "XMPFiles/source/FormatSupport/IFF/IChunkData.h"
#include "XMPFiles/source/NativeMetadataSupport/IMetadata.h"


namespace IFF_RIFF
{

/**
 *	AIFF Metadata model.
 *	Implements the IMetadata interface
 */
class AIFFMetadata : public IMetadata
{
public:
	enum
	{
		kName,				// std::string
		kAuthor,			// std::string
		kCopyright,			// std::string
		kAnnotation			// std::string
	};

public:
	AIFFMetadata();
	~AIFFMetadata();

protected:
	/**
	 * @see IMetadata::isEmptyValue
	 */
	virtual	bool isEmptyValue( XMP_Uns32 id, ValueObject& valueObj );

private:
	// Operators hidden on purpose
	AIFFMetadata( const AIFFMetadata& ) {};
	AIFFMetadata& operator=( const AIFFMetadata& ) { return *this; };
};

} // namespace

#endif
