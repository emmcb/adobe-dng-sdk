#ifndef XMPAssetManagementFwdDeclarations_I_h__
#define XMPAssetManagementFwdDeclarations_I_h__ 1

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
// 02-11-15 SKP 1.0-a049 Added initial support for client strategies. Using these strategies client
//						 can set prefix, AppName, history update policy and it's own datetime
//						 function which will be used to get time for ModifyDate, Metadata date etc.
// 12-07-14 ADC 1.0-a041 Refactored Part Classes and Adding support for IAssetArtboardPart.
// 11-26-14 ADC 1.0-a013 Added the full skeleton for IComposedAssetManager interface.
// 11-24-14 ADC 1.0-a008 Support for ICompositionRelationship interface.
// 11-23-14 SKP 1.0-a006 Support for IAssetPagePart interface.
// 11-21-14 ADC 1.0-a004 Support for IAssetLayerPart interface.
// 11-21-14 ADC 1.0-a002 Support for IAssetPart and IAssetTimePart interfaces.
// 11-19-14 ADC 1.0-a001 Framework in place for Asset Management Library.
//
// =================================================================================================
#endif  // AdobePrivate

#include "XMPCommon/XMPCommonFwdDeclarations_I.h"
#include "XMPAssetManagement/XMPAssetManagementDefines_I.h"
#include "XMPAssetManagement/XMPAssetManagementFwdDeclarations.h"

#include "XMPCommon/Utilities/TAllocator.h"

namespace AdobeXMPAM_Int {
	using AdobeXMPCommon_Int::shared_ptr;
	using AdobeXMPAM_Int::TAllocator;
	 

	typedef shared_ptr< IAssetManager_I >			spIAssetManager_I;
	typedef shared_ptr< const IAssetManager_I >		spcIAssetManager_I;

	// IExtensionsObjectFactory_I
	class IExtensionsObjectFactory_I;
	typedef IExtensionsObjectFactory_I *						pIExtensionsObjectFactory_I;
	typedef const IExtensionsObjectFactory_I *					pcIExtensionsObjectFactory_I;
	typedef shared_ptr< IExtensionsObjectFactory_I >			spIExtensionsObjectFactory_I;
	typedef shared_ptr< const IExtensionsObjectFactory_I >		spcIExtensionsObjectFactory_I;

	// IAssetUtilities_I
	class IAssetUtilities_I;
	typedef IAssetUtilities_I *										pIAssetUtilities_I;
	typedef const IAssetUtilities_I *								pcIAssetUtilities_I;
	typedef shared_ptr< IAssetUtilities_I >							spIAssetUtilities_I;
	typedef shared_ptr< const IAssetUtilities_I >					spcIAssetUtilities_I;

	// IAssetPart_I
	class IAssetPart_I;
	typedef IAssetPart_I *											pIAssetPart_I;
	typedef const IAssetPart_I *									pcIAssetPart_I;
	typedef shared_ptr< IAssetPart_I >								spIAssetPart_I;
	typedef shared_ptr< const IAssetPart_I >						spcIAssetPart_I;

	// IAssetTimePart_I
	class IAssetTimePart_I;
	typedef IAssetTimePart_I *										pIAssetTimePart_I;
	typedef const IAssetTimePart_I *								pcIAssetTimePart_I;
	typedef shared_ptr< IAssetTimePart_I >							spIAssetTimePart_I;
	typedef shared_ptr< const IAssetTimePart_I >					spcIAssetTimePart_I;

	// IAssetLayerPart_I
	class IAssetLayerPart_I;
	typedef IAssetLayerPart_I *										pIAssetLayerPart_I;
	typedef const IAssetLayerPart_I *								pcIAssetLayerPart_I;
	typedef shared_ptr< IAssetLayerPart_I >							spIAssetLayerPart_I;
	typedef shared_ptr< const IAssetLayerPart_I >					spcIAssetLayerPart_I;

	// IAssetPagePart_I
	class IAssetPagePart_I;
	typedef IAssetPagePart_I *										pIAssetPagePart_I;
	typedef const IAssetPagePart_I *								pcIAssetPagePart_I;
	typedef shared_ptr< IAssetPagePart_I >							spIAssetPagePart_I;
	typedef shared_ptr< const IAssetPagePart_I >					spcIAssetPagePart_I;

	// IAssetArtboardPart_I
	class IAssetArtboardPart_I;
	typedef IAssetArtboardPart_I *									pIAssetArtboardPart_I;
	typedef const IAssetArtboardPart_I *							pcIAssetArtboardPart_I;
	typedef shared_ptr< IAssetArtboardPart_I >						spIAssetArtboardPart_I;
	typedef shared_ptr< const IAssetArtboardPart_I >				spcIAssetArtboardPart_I;

	// ICompositionRelationship_I
	class ICompositionRelationship_I;
	typedef ICompositionRelationship_I *							pICompositionRelationship_I;
	typedef const ICompositionRelationship_I *						pcICompositionRelationship_I;
	typedef shared_ptr< ICompositionRelationship_I >				spICompositionRelationship_I;
	typedef shared_ptr< const ICompositionRelationship_I >			spcICompositionRelationship_I;

	// IComposedAssetManager_I
	class IComposedAssetManager_I;
	typedef IComposedAssetManager_I *								pIComposedAssetManager_I;
	typedef const IComposedAssetManager_I *							pcIComposedAssetManager_I;
	typedef shared_ptr< IComposedAssetManager_I >					spIComposedAssetManager_I;
	typedef shared_ptr< const IComposedAssetManager_I >				spcIComposedAssetManager_I;


	// typedefs for vectors and their corresponding shared pointers

	typedef std::vector< spICompositionRelationship_I, TAllocator< spICompositionRelationship_I > >			ICompositionRelationshipList_I;
	typedef std::vector< spcICompositionRelationship_I, TAllocator< spcICompositionRelationship_I > >		cICompositionRelationshipList_I;

	typedef shared_ptr< ICompositionRelationshipList_I >													spICompositionRelationshipList_I;
	typedef shared_ptr< cICompositionRelationshipList_I >													spcICompositionRelationshipList_I;

	// refcator earlier in XMPCommon
	/*typedef std::vector< spIUTF8String_I >					IUTF8StringList_I;
	typedef std::vector< spcIUTF8String_I >					cIUTF8StringList_I;

	typedef shared_ptr< IUTF8StringList_I >					spIUTF8StringList_I;
	typedef shared_ptr< cIUTF8StringList_I >				spcIUTF8StringList_I;
	*/

	// IExtensionsConfigurationManager_I
	class IExtensionsConfigurationManager_I;
	typedef IExtensionsConfigurationManager_I *							pIExtensionsConfigurationManager_I;
	typedef const IExtensionsConfigurationManager_I *					pcIExtensionsConfigurationManager_I;
	typedef shared_ptr< IExtensionsConfigurationManager_I >				spIExtensionsConfigurationManager_I;
	typedef shared_ptr< const IExtensionsConfigurationManager_I >		spcIExtensionsConfigurationManager_I;

	// IExtensionsConfigurationManager_I
	class ExtensionsConfigurationManagerImpl;
	typedef ExtensionsConfigurationManagerImpl *							pExtensionsConfigurationManagerImpl;
	typedef const ExtensionsConfigurationManagerImpl *					pcExtensionsConfigurationManagerImpl;
	typedef shared_ptr< ExtensionsConfigurationManagerImpl >				spExtensionsConfigurationManagerImpl;
	typedef shared_ptr< const ExtensionsConfigurationManagerImpl >		spcExtensionsConfigurationManagerImpl;
}

#endif  // XMPAssetManagementFwdDeclarations_I_h__
