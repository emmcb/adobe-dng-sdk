#ifndef AssetUtilitiesImpl_h__
#define AssetUtilitiesImpl_h__ 1

// =================================================================================================
// ADOBE SYSTEMS INCORPORATED
// Copyright 2014 Adobe Systems Incorporated
// All Rights Reserved
//
// NOTICE:  Adobe permits you to use, modify, and distribute this file in accordance with the terms
// of the Adobe license agreement accompanying it.
// =================================================================================================

#if AdobePrivate
// =================================================================================================
// Change history
// ==============
//
// Writers:
//  ADC	Amandeep Chawla
//	SKP Sunil Kishor Pathak
//
// mm-dd-yy who Description of changes, most recent first.
//
// 03-05-15 SKP 1.0-a076 Bug: 3947546, 3947499 & 3947421. Refactored code of RemoveNode and added
//						 internal function RemoveEmptyNode() to remove DocumentID, InstanceID &
//						 OriginalDocumentID if these are present but are empty and client called
//						 MakeTrackable()
// 02-18-15 SKP 1.0-a054 Fixed bug: 3921227 & 3920761 Setting correct document ID if Original-
//						 DocumentID is present with/without DerivedFrom and with/without History
// 12-05-14 SKP 1.0-a038 Changed API name from IsTrackAble() to IsTrackable() and from 
//						 MakeTrackAble() to MakeTrackable()
// 11-19-14 ADC 1.0-a001 Framework in place for Asset Management Library.
//
// =================================================================================================
#endif  // AdobePrivate

#if !(IMPLEMENTATION_HEADERS_CAN_BE_INCLUDED)
	#error "Not adhering to design constraints"
	// this file should only be included from its own cpp file
#endif

#include "XMPAssetManagement/Interfaces/IAssetUtilities_I.h"

#include "XMPCommon/BaseClasses/MemoryManagedObject.h"
#include "XMPCommon/ImplHeaders/SharedObjectImpl.h"

#if XMP_WinBuild
	#pragma warning( push )
	#pragma warning( disable : 4250 )
#endif

namespace AdobeXMPAM_Int{
	using namespace AdobeXMPAM_Int;
	using namespace AdobeXMPCommon;
	using namespace AdobeXMPCommon_Int;
	
	class AssetUtilitiesImpl
		: public virtual IAssetUtilities_I
		, public virtual SharedObjectImpl
		, public virtual MemoryManagedObject
	{
	public:
		AssetUtilitiesImpl() {}
		virtual ~AssetUtilitiesImpl() __NOTHROW__;
		virtual bool APICALL IsTrackable( const spcIMetadata & metadata );
		virtual bool APICALL MakeTrackable( const spIMetadata & metadata );

	private:
		virtual bool GetOldestHistoryInstanceID(const spIStructureNode ixmpstruct, std::string& instanceid);
		virtual bool RemoveEmptyNode(spIStructureNode stnode, std::string ns, std::string prop);

	protected:
		
	#ifdef FRIEND_CLASS_DECLARATION
			FRIEND_CLASS_DECLARATION();
	#endif
			REQ_FRIEND_CLASS_DECLARATION();

	};
	
	inline AssetUtilitiesImpl::~AssetUtilitiesImpl() __NOTHROW__ { }
}

#if XMP_WinBuild
	#pragma warning( pop )
#endif

#endif  // AssetUtilitiesImpl_h__
