#ifndef XMPAssetManagement_ChangeLog_h__
#define XMPAssetManagement_ChangeLog_h__ 1

// =================================================================================================
// ADOBE SYSTEMS INCORPORATED
// Copyright 2014 Adobe Systems Incorporated
// All Rights Reserved
//
// NOTICE:  Adobe permits you to use, modify, and distribute this file in accordance with the terms
// of the Adobe license agreement accompanying it.
// =================================================================================================

// =================================================================================================
// This file has two purposes, to provide the current engineering build number and to maintain a log
// of source changes going into each build. All but the most trivial changes should be noted here.
// This is the primary source for release notes. It is not necessary to change the build number for
// every change, but it should be updated prior to weekly builds if there are changes. The build
// number is maintained by hand. It reflects the state of the source. Official builds also include
// a Perforce changelist number and timestamp, so that separate builds from identical source can be
// differentiated.
// =================================================================================================

#if 0
	// ! Can't use this because Xcode does not do includes for plist processing. 
	#include "public/include/XMP_Environment.h"	// ! Must be the first include.
#else
	#ifndef XMP_DebugBuild
		#if NDEBUG
			#define XMP_DebugBuild 0
		#else
			#define XMP_DebugBuild 1
		#endif
	#endif
#endif

#define kXMPExtensions_EngBuild    0
#define kXMPExtensions_EngString "000"

#if !XMP_DebugBuild
	#define kXMPExtensions_DebugSuffix
	#define kXMPExtensions_DebugString "        "
#else
	#define kXMPExtensions_DebugSuffix (debug)
	#define kXMPExtensions_DebugString " (debug)"
#endif

#if AdobePrivate
// =================================================================================================
// Change history
// ==============
//
// Writers:
//  ADC	Amandeep Chawla
//  SKP	Sunil Kishor Pathak
//  ANS	Ankita Sharma
//  PRI Priyanka Gupta
//
// mm-dd-yy who Description of changes, most recent first.
//
// 07/23/15 SKP 1.0-a092 Fixed bugs : 3998474, 3998471
// 07/21/15 PRI 1.0-a091 Fixing warnings in public headers after OSX10.10 upgrade.
// 06/04/15 PRI 1.0-a090 Fixing build error on mac32.
// 05/19/15 SKP 1.0-a089 Removed Clone() compilation error (bug: 3982490)
// 04/29/15 SKP 1.0-a088 Bug :3973168 Crash in application on calling clone method for 
//						 Page/Layer/Time/Artboard Part
// 04/27/15 SKP 1.0-a087 Fixed condition to incr the pantry count for a component. (Bug: 3947421)
// 04/24/15 SKP 1.0-a086 Fixed issues/missing part in API documetaion.
// 04/24/15 SKP 1.0-a085 Submitting unsaved changed which were missed in cl:157801
// 04/23/15 SKP 1.0-a084 Incorporating review comments. Removed the prefix name from Enum
//						 member which is same enum name. 
// 04/21/15 SKP 1.0-a083 Fixing issue in internal interface of RemoveComponent. 
// 04/21/15 SKP 1.0-a082 Bug 3950485: Add RemoveComponent API's with relationship as argument,
//						 so that client can remove component on the basis of relationship.
// 04/20/15 SKP 1.0-a081 Added check for page number 0 for custom page part. 
// 04/17/15 SKP 1.0-a080 Bug : 3971351 SetTimeDuration returning false when start time is zero. 
// 04/17/15 SKP 1.0-a079 Bug: 3949982 CreatePagePartPage should throw exception if page number
//						 specified is zero.
// 03-05-15 SKP 1.0-a078 Bug: 3924437 Crash in application on calling clone method of AssetPart.
// 03-05-15 SKP 1.0-a077 Fixed test case. Fixed the copy-paste error in ComposedAssetManagerImpl.cpp
// 03-05-15 SKP 1.0-a076 Bug: 3947546, 3947499 & 3947421. Refactored code of RemoveNode and added
//						 internal function RemoveEmptyNode() to remove DocumentID, InstanceID &
//						 OriginalDocumentID if these are present but are empty and client called
//						 MakeTrackable()
// 03-04-15 SKP 1.0-a075 Bug: 3947384, 3947379
// 03-01-15 SKP 1.0-a074 Watson bug: 3947393, 3947390
// 03-01-15 SKP 1.0-a073 Enable XMPAssetUnit test runner and fixed the PK build break.
// 03-01-15 SKP 1.0-a072 Reverted back changed in cl: 157586; fixed build break.
// 03-01-15 SKP 1.0-a071 Fixed PK build break. Stopped building XMPAM unit test project.
// 02-28-15 SKP 1.0-a070 XMPAM_ClientInitialize() returns null pointer, therefore not able to call
//						 IAssetConfigurationManager api's.
// 02-26-15 SKP 1.0-a069 Fixed build break on Mac.
// 02-26-15 SKP 1.0-a068 Fixed build break on Mac.
// 02-26-15 SKP 1.0-a067 Bug: 3945356 Istrackable should return false in case InstanceID(any 
//						 trackableID) present is empty while it is returning true.
// 02-26-15 SKP 1.0-a066 Refactor code to fix WorkflowTests::DiamondTest test cases.
// 02-26-15 SKP 1.0-a065 Refactored IsComposedOf and RemoveAllRelationship.
// 02-26-15 SKP 1.0-a064 Refactored IsComposedOf and some other methods.
// 02-25-15 SKP 1.0-a063 Removed cout expression; fixed bug: 3935528, 3935532
// 02-25-15 SKP 1.0-a062 Fixed projectknow build break and also fixed the issue in 
//						 ComposedAssetManagerImpl
// 02-24-15 SKP 1.0-a061 Bug: 3934400; Empty ingredient and pantry was getting created.
// 02-23-15 SKP 1.0-a060 Fixed bug: 3935524, 3943088
// 02-20-15 SKP 1.0-a059 Bug : 3935518 Exception not handled when NULL value is passed in 
//						 RemoveComponents , AddComponent , IsDirectlyComposedOf , IsComposedOf.
// 02-19-15 SKP 1.0-a058 Fixed API test cases 
//						 IAssetUtilitiesTests::OriginalDocumentId_DerivedFrom_Present & some more.
// 02-19-15 SKP 1.0-a057 Bug : 3937072 CreateComposedAssetManger creates its object if the document
//						 to be composed is not trackable while it should throw exception in this case
// 02-19-15 SKP 1.0-a056 Fixed Allocator, ErrorNotification and threading handling in AssetConfigMgr
// 02-18-15 SKP 1.0-a055 Fixed testcase : IAssetUtilitiesTests::NoTrackingID_HistoryPresent()
// 02-18-15 SKP 1.0-a054 Fixed bug: 3921227 & 3920761 Setting correct document ID if Original-
//						 DocumentID is present with/without DerivedFrom and with/without History
// 02-16-15 SKP 1.0-a053 Fixed XMPToolkit build break
// 02-12-15 SKP 1.0-a052 Fixed mac build break
// 02-12-15 SKP 1.0-a051 Implemented history policy support. After this support during composition
//						 history entry will be updated based oneHistoryUpdatePolicy and 
//						 eHistoryUpdateAction (IAssetConfigManager.h). Currently eHistoryUpdatePolicy
//						 ::kUpdateEveryTime, eHistoryUpdatePolicy::kUpdateIgnore policy and action 
//						 eHistoryUpdateAction::kHistoryActionAll is supported.
// 02-11-15 SKP 1.0-a050 Implementing support for XMPDateTime, History update policy and action
// 02-11-15 SKP 1.0-a049 Added initial support for client strategies. Using these strategies client
//						 can set prefix, AppName, history update policy and it's own datetime
//						 function which will be used to get time for ModifyDate, Metadata date etc.
// 01-16-15 SKP 1.0-a048 Fixed the logic error to fix issue in creating a document trackable.
// 12-10-14 SKP 1.0-a046 Fixed crash in MakeTrackable() and fix typo error.
// 12-12-14 SKP 1.0-a045 MakeTrakable() was not inserting OriginalDocumentID in case if 
//						 OriginalDocumentID is not present but DerivedFrom is present 
//						 having documentID
// 12-11-14 SKP 1.0-a044 Fixed return value in IsTrackable, fixed issues in RemoveComponent,
//						 Added GetIngredientNode() to return the reference of node within the
//						 the relationship to the ingredient entry.
// 12-10-14 SKP 1.0-a043 Updated instance id in MakeTrackable().
// 12-10-14 ADC 1.0-a042 Fixed crash in MakeTrackable() in case when DocumentID is present but
//						 OriginalDocumentID is missing.
// 12-07-14 ADC 1.0-a041 Refactored Part Classes and Adding support for IAssetArtboardPart.
// 12-05-14 SKP 1.0-a040 Added support for part SetDescription
// 12-05-14 SKP 1.0-a039 Fixed issue in SetDescription()(). Fixed argment in CreateFromCStr() call.
// 12-05-14 SKP 1.0-a038 Changed API name from IsTrackAble() to IsTrackable() and from 
//						 MakeTrackAble() to MakeTrackable()
// 12-04-14 SKP 1.0-a037 Implemented IsDirectlyComposedOf()
// 12-04-14 SKP 1.0-a036 Fixed issue in SetMappingFunctionName().
// 12-03-14 SKP 1.0-a035 Removed debug code & updated history on relationship deletion.
// 12-03-14 SKP 1.0-a034 Fixed issues in creating relationship from existing ingredient.
// 12-03-14 SKP 1.0-a033 Removed SerializeMetadata() which was used for debugging.
// 12-02-14 ADC 1.0-a032 Fixed Memory leaks in client glue code.
// 12-02-14 SKP 1.0-a031 Fixed MAC build break. 
// 12-02-14 SKP 1.0-a030 stringstream '=' operator is private. In order to reset the stringstream
//						 object, reset it calling it's () operator. Fix build error on Mac/iOS.
// 12-02-14 SKP 1.0-a029 Implemented time serialization and fixed issues in layer implementation.
// 12-02-14 SKP 1.0-a028 1. Moved standardComponentStrings in IAssetPart_I.cpp so that it can be
//							used in other cpp files,
//						 2. Fixed issues in SerializeAsRDFSyntax(),
//						 3. Fixed issue in GetAssetPartComponentIndex() and 4. fixed issues
//							while composing from composed component.
// 12-02-14 SKP 1.0-a027 Fixed copy-paste error, replaced filePath with maskMarkers.
// 12-01-14 ADC 1.0-a026 Fixing inheritance hierarchy issue in public exposed proxy classes
//						 for specialized part interfaces.
// 12-01-14 SKP 1.0-a025 Added functionality to add stRef on relationship.
// 12-01-14 SKP 1.0-a024 Implemented remove and search relationship functionality.
// 12-01-14 ADC 1.0-a023 Fixed the issue with public source not compiling on iOS.
// 12-01-14 ADC 1.0-a022 Fixed the issue in the Get[All]Relationships client glue code.
// 11-28-14 SKP 1.0-a021 Fixed the build break on mac. commented out std::logic_error
// 11-28-14 SKP 1.0-a020 Fixed Mac build break & Implemeted basic part removal funationlity
// 11-28-14 SKP 1.0-a019 Implemented Find & Remove Relationship functionality for composed Part
// 11-28-14 SKP 1.0-a018 Fixed issues in genericpart addition & history entry
// 11-28-14 ADC 1.0-a017 Submitting changes for fixing issue with the Mac build.
// 11-27-14 ADC 1.0-a016 Submitting changes related to iOS build.
// 11-26-14 SKP 1.0-a015 Fixed mac build break.
// 11-26-14 SKP 1.0-a014 Fixed mac build break.
// 11-26-14 ADC 1.0-a013 Added the full skeleton for IComposedAssetManager interface.
// 11-26-14 SKP 1.0-a012 Made changed to build AML on Mac cl:157148 
// 11-26-14 SKP 1.0-a011 Implemented SerializeAsRDFSyntax() to get serialized content of part
// 11-26-14 SKP 1.0-a010 Implemented function for parsing string and creating parts for relation
// 11-25-14 ADC 1.0-a009 Fixing compile and linking issues.
// 11-24-14 ADC 1.0-a008 Support for ICompositionRelationship interface.
// 11-24-14 ANS 1.0-a007 Fixing build errors
// 11-23-14 SKP 1.0-a006 Support for IAssetPagePart interface.
// 11-21-14 ANS 1.0-a005 Fixing build issues in client glue code for AssetUtilities
// 11-21-14 ADC 1.0-a004 Support for IAssetLayerPart interface.
// 11-21-14 SKP 1.0-a003 Added check of metadata for NULL and throw exception for NULL metadata
// 11-21-14 ADC 1.0-a002 Support for IAssetPart and IAssetTimePart interfaces.
// 11-19-14 ADC 1.0-a001 Framework in place for Asset Management Library.
//
// =================================================================================================
#endif  // AdobePrivate


#endif  // XMPAssetManagement_ChangeLog_h__
