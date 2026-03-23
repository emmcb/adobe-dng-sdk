// =================================================================================================
// Copyright Adobe
// Copyright 2014 Adobe
// All Rights Reserved
//
// NOTICE:  Adobe permits you to use, modify, and distribute this file in accordance with the terms
// of the Adobe license agreement accompanying it. If you have received this file from a source other 
// than Adobe, then your use, modification, or distribution of it requires the prior written permission
// of Adobe.
// =================================================================================================

#define IMPLEMENTATION_HEADERS_CAN_BE_INCLUDED 1
	#include "XMPCompareAndMerge/ImplHeaders/ThreeWayMergeImpl.h"
#undef IMPLEMENTATION_HEADERS_CAN_BE_INCLUDED

#include "XMPCommon/Interfaces/IError_I.h"
#include "XMPCompareAndMerge/XMPCompareAndMergeErrorCodes.h"
#include "XMPCompareAndMerge/Interfaces/IThreeWayConflictResolutionEngine.h"
#include "XMPCompareAndMerge/Interfaces/IThreeWayConflictIdentificationEngine.h"
#include "XMPCompareAndMerge/Interfaces/IThreeWayMergeResult_I.h"

#include "XMPCommon/Interfaces/IUTF8String.h"
#include "XMPCommon/Utilities/TSmartPointers_I.h"
#include "XMPCore/Interfaces/IMetadata.h"
#include "XMPCore/Interfaces/IDOMParser.h"
#include "XMPCore/Interfaces/IDOMSerializer.h"
#include "XMPCore/Interfaces/IDOMImplementationRegistry.h"
#include "XMPCore/Interfaces/IMetadataConverterUtils.h"

#include "XMPCommon/XMPCommonErrorCodes.h"
#include "XMPCommon/Interfaces/IError_I.h"

#include "XMPCompareAndMerge/ImplHeaders/UniversalOldestDateStrategyImpl.h"
#include "XMPCompareAndMerge/ImplHeaders/UniversalLatestDateStrategyImpl.h"
#include "XMPCompareAndMerge/ImplHeaders/UniversalArrayOfSimpleNodeImpl.h"
#include "XMPCompareAndMerge/ImplHeaders/UniversalArrayOfStructureOfSimpleNodeImpl.h"
#include "XMPCompareAndMerge/ImplHeaders/UniversalSimpleNodeStrategyImpl.h"
#include "XMPCompareAndMerge/ImplHeaders/UniversalIngredientsStrategyImpl.h"
#include "XMPCompareAndMerge/ImplHeaders/UniversalPantryStrategyImpl.h"
#include "XMPCompareAndMerge/ImplHeaders/UniversalHistoryStrategyImpl.h"
#include "XMPCompareAndMerge/ImplHeaders/UniversalVersionsStrategyImpl.h"
#include "XMPCompareAndMerge/ImplHeaders/UniversalArrayOfDateImpl.h"
#include "XMPCompareAndMerge/ImplHeaders/UniversalArrayOfStructureOfSimpleAndStructOfSimpleImpl.h"
#include "XMPCompareAndMerge/ImplHeaders/UniversalArrayOfStructureOfSimpleAndOrderedArrOfSimpleImpl.h"
#include "XMPCompareAndMerge/ImplHeaders/UniversalBlindStrategyImpl.h"
#include "XMPCompareAndMerge/ImplHeaders/UniversalArrayOfStructureOfSimpleNodeWithPrimaryKeyImpl.h"

#include <map>
namespace AdobeXMPCompareAndMerge_Int {

	ThreeWayMergeImpl::ThreeWayMergeImpl()
		: mResolutionEngine( IThreeWayConflictResolutionEngine::CreateThreeWayConflictResolutionEngine() ) { }

	bool APICALL ThreeWayMergeImpl::RegisterThreeWayStrategy( const spIThreeWayStrategy & strategy ) {
		return mResolutionEngine->RegisterResolutionStrategy( strategy );
	}

	spcIThreeWayMergeResult APICALL ThreeWayMergeImpl::Merge( const spcIMetadata & baseVersionMetadata, const spcIMetadata & latestVersionMedata, const spcIMetadata & userVersionMetadata ) const {
		// get the 3 way conflict Identification engine
		spIThreeWayConflictIdentificationEngine conflictIdentificationEngine = IThreeWayConflictIdentificationEngine::CreateThreeWayConflictIdentificationEngine();
		ThreeWayUnResolvedResolvedConflicts conflicts = conflictIdentificationEngine->IdentifyConflicts( baseVersionMetadata, latestVersionMedata, userVersionMetadata );

		spIThreeWayMergeResult mergeResult = IThreeWayMergeResult_I::CreateThreeWayMergeResult( latestVersionMedata, userVersionMetadata, conflicts.second, conflicts.first );

		pIThreeWayMergeResult_I mergeResult_I = mergeResult->GetIThreeWayMergeResult_I();
		spIThreeWayUnResolvedConflicts unresolvedList = mergeResult_I->GetUnResolvedConflicts();
		spIThreeWayResolvedConflicts resolvedList = mergeResult_I->GetResolvedConflicts();

		mResolutionEngine->ResolveConflicts( unresolvedList, resolvedList );

		return mergeResult;
	}

	spcIThreeWayMergeResult APICALL ThreeWayMergeImpl::AutoMerge ( const spcIMetadata & baseVersionMetadata, const spcIMetadata & latestVersionMetadata, const spcIMetadata & userVersionMetadata, eAutoMergeStrategy skipAutoMergeStrategy ) const {

		if ( latestVersionMetadata == NULL || userVersionMetadata == NULL ) {
			NOTIFY_ERROR ( IError_v1::kEDGeneral, kGECParametersNotAsExpected,
				"Metadata should not be null", IError_v1::kESOperationFatal,
				false, false );
		}

		spIThreeWayGenericStrategy strategyDate;
		spIThreeWayStrategy strategy;

		if ( !(skipAutoMergeStrategy & IThreeWayMerge::kAMSLatestDate) )
		{
			strategyDate = UniversalLatestDateStrategyImpl::CreateUniversalLatestDateStrategyImpl ( );
			strategyDate->AppendPath ( kXMP_NS_XMP":MetadataDate" , AdobeXMPCommon::npos );
			strategyDate->AppendPath ( kXMP_NS_XMP":ModifyDate" , AdobeXMPCommon::npos );
			strategyDate->AppendPath ( kXMP_NS_XMP_MM":VersionID" , AdobeXMPCommon::npos );
			strategyDate->AppendPath ( kXMP_NS_XMP_MM":InstanceID" , AdobeXMPCommon::npos );
			strategyDate->AppendPath ( kXMP_NS_XMP_MM":DocumentID" , AdobeXMPCommon::npos );
			strategyDate->AppendPath ( kXMP_NS_XMP_MM":OriginalDocumentID" , AdobeXMPCommon::npos );
			strategyDate->AppendPath ( kXMP_NS_PDF":PDFVersion" , AdobeXMPCommon::npos );
			auto spStrategyDate = MakeUncheckedSharedPointer ( strategyDate->GetIThreeWayGenericStrategy_I ( ) , __FILE__ , __LINE__ , true );
			mResolutionEngine->RegisterResolutionStrategy ( spStrategyDate );
		}

		if ( !(skipAutoMergeStrategy & IThreeWayMerge::kAMSOldestDate) )
		{
			strategyDate = UniversalOldestDateStrategyImpl::CreateUniversalOldestDateStrategyImpl ( );
			strategyDate->AppendPath ( kXMP_NS_XMP_Dimensions":releaseDate" , AdobeXMPCommon::npos );
			strategyDate->AppendPath ( kXMP_NS_XMP_Dimensions":shotDate" , AdobeXMPCommon::npos );
			strategyDate->AppendPath ( kXMP_NS_XMP":CreateDate" , AdobeXMPCommon::npos );
			strategyDate->AppendPath ( kXMP_NS_Photoshop":DateCreated" , AdobeXMPCommon::npos );

			auto spStrategyDate = MakeUncheckedSharedPointer ( strategyDate->GetIThreeWayGenericStrategy_I ( ) , __FILE__ , __LINE__ , true );
			mResolutionEngine->RegisterResolutionStrategy ( spStrategyDate );
		}

		if (!(skipAutoMergeStrategy & IThreeWayMerge::kAMSHistory))
		{
			strategy = MakeUncheckedSharedPointer(new UniversalHistoryStrategyImpl(), __FILE__, __LINE__, true);
			mResolutionEngine->RegisterResolutionStrategy(strategy);
		}

		if (!(skipAutoMergeStrategy & IThreeWayMerge::kAMSVersions))
		{
			strategy = MakeUncheckedSharedPointer(new UniversalVersionsStrategyImpl(), __FILE__, __LINE__, true);
			mResolutionEngine->RegisterResolutionStrategy(strategy);
		}

		if ( !(skipAutoMergeStrategy & IThreeWayMerge::kAMSIngredients) )
		{
			strategy = MakeUncheckedSharedPointer ( new UniversalIngredientsStrategyImpl ( ) , __FILE__ , __LINE__ , true );
			mResolutionEngine->RegisterResolutionStrategy ( strategy );
		}

		if ( !(skipAutoMergeStrategy & IThreeWayMerge::kAMSPantry) )
		{
			strategy = MakeUncheckedSharedPointer ( new UniversalPantryStrategyImpl ( ) , __FILE__ , __LINE__ , true );
			mResolutionEngine->RegisterResolutionStrategy ( strategy );
		}

		if ( !(skipAutoMergeStrategy & IThreeWayMerge::kAMSArrayOfDate) )
		{
			strategyDate = UniversalArrayOfDateImpl::CreateArrayOfDateThreeWayGenericStrategy ( );
			strategyDate->AppendPath ( kXMP_NS_DC":date" , AdobeXMPCommon::npos );
			
			auto spStrategyDate = MakeUncheckedSharedPointer ( strategyDate->GetIThreeWayGenericStrategy_I ( ) , __FILE__ , __LINE__ , true );
			mResolutionEngine->RegisterResolutionStrategy ( spStrategyDate );
		}

		if ( !(skipAutoMergeStrategy & IThreeWayMerge::kAMSArrayOfStructOfSimpleNodeWithPrimaryKey) )
		{
			std::map <std::pair<std::string , std::string> , std::pair<std::string , std::string> > mprimaryKeyMap;
			mprimaryKeyMap.insert ( make_pair ( std::make_pair ( kXMP_NS_XMP_BJ , "JobRef" ) , std::make_pair ( kXMP_NS_XMP_ST_Job , "id" ) ) );
			mprimaryKeyMap.insert ( make_pair ( std::make_pair ( kXMP_NS_Photoshop , "DocumentAncestors" ) , std::make_pair ( kXMP_NS_Photoshop , "AncestorID" ) ) );
			mprimaryKeyMap.insert ( make_pair ( std::make_pair ( kXMP_NS_Photoshop , "TextLayers" ) , std::make_pair ( kXMP_NS_Photoshop , "LayerName" ) ) );
			strategy = MakeUncheckedSharedPointer ( new UniversalArrayOfStructureOfSimpleNodeWithPrimaryKeyImpl ( mprimaryKeyMap ) , __FILE__ , __LINE__ , true );
			mResolutionEngine->RegisterResolutionStrategy ( strategy );
		}

		if ( !(skipAutoMergeStrategy & IThreeWayMerge::kAMSArrayOfStructOfSimpleNode) )
		{
			strategy = MakeUncheckedSharedPointer ( new UniversalArrayOfStructureOfSimpleNodeImpl ( ) , __FILE__ , __LINE__ , true );
			mResolutionEngine->RegisterResolutionStrategy ( strategy );
		}
		
		if ( !(skipAutoMergeStrategy & IThreeWayMerge::kAMSArrayOfStructureOfSimpleAndStructOfSimple) )
		{
			strategy = MakeUncheckedSharedPointer ( new UniversalArrayOfStructureOfSimpleAndStructOfSimpleImpl ( ) , __FILE__ , __LINE__ , true );
			mResolutionEngine->RegisterResolutionStrategy ( strategy );
		}

		if ( !(skipAutoMergeStrategy & IThreeWayMerge::kAMSArrayOfStructureOfSimpleAndOrderedArrOfSimple) )
		{
			strategy = MakeUncheckedSharedPointer ( new UniversalArrayOfStructureOfSimpleAndOrderedArrOfSimpleImpl ( ) , __FILE__ , __LINE__ , true );
			mResolutionEngine->RegisterResolutionStrategy ( strategy );
		}

		if ( !(skipAutoMergeStrategy & IThreeWayMerge::kAMSArrayOfSimpleNode) )
		{
			strategy = MakeUncheckedSharedPointer ( new UniversalArrayOfSimpleNodeImpl ( ) , __FILE__ , __LINE__ , true );
			mResolutionEngine->RegisterResolutionStrategy ( strategy );
		}

		if ( !( skipAutoMergeStrategy & IThreeWayMerge::kAMSSimpleNode ) )
		{
			strategy = MakeUncheckedSharedPointer ( new UniversalSimpleNodeStrategyImpl (), __FILE__, __LINE__, true );
			mResolutionEngine->RegisterResolutionStrategy ( strategy );
		}

		if ( !( skipAutoMergeStrategy & IThreeWayMerge::kAMSBlind ) )
		{
			strategy = MakeUncheckedSharedPointer ( new UniversalBlindStrategyImpl (), __FILE__, __LINE__, true );
			mResolutionEngine->RegisterResolutionStrategy ( strategy );
		}

		spcIMetadata baseMetadata = baseVersionMetadata;
		if ( ! baseMetadata )
			baseMetadata = userVersionMetadata->Clone ( )->ConvertToMetadata ( );
		
		return Merge ( baseMetadata , latestVersionMetadata, userVersionMetadata );

	}

	spcIUTF8String APICALL ThreeWayMergeImpl::AutoMerge ( const char * baseVersionMetadata, sizet baseVersionMetadataLength, const char * latestVersionMetadata, sizet latestVersionMetadataLength, const char * userVersionMetadata, sizet userVersionMetadataLength, bool& isMerged, eAutoMergeStrategy skipAutoMergeStrategy ) const {
	
		if ( latestVersionMetadata == NULL || userVersionMetadata == NULL ) {
			NOTIFY_ERROR ( IError_v1::kEDGeneral, kGECParametersNotAsExpected,
				"Metadata should not be null", IError_v1::kESOperationFatal,
				false, false );
		}

		spIDOMImplementationRegistry DOMRegistry = IDOMImplementationRegistry::GetDOMImplementationRegistry ();
		spIDOMParser parser = DOMRegistry->GetParser ( "rdf" );
		
		spIMetadata baseMeta;
		if (baseVersionMetadata)
			 baseMeta = parser->Parse ( baseVersionMetadata, baseVersionMetadataLength );
		else
			baseMeta = parser->Parse ( userVersionMetadata, userVersionMetadataLength );

		spIMetadata latestMeta = parser->Parse ( latestVersionMetadata, latestVersionMetadataLength );
		spIMetadata userMeta = parser->Parse ( userVersionMetadata, userVersionMetadataLength );

		spcIThreeWayMergeResult result = AutoMerge ( baseMeta , latestMeta, userMeta, skipAutoMergeStrategy );
		
		if ( result->CountOfResolvedConflicts () != 0 && result->CountOfUnResolvedConflicts () == 0 )
			isMerged = true;
		else
			isMerged = false;
		if ( result->GetMergedStatus() != IThreeWayMergeResult::kMSConflictsPending )
		{
			AdobeXMPCore::spIDOMImplementationRegistry spRegistry = AdobeXMPCore::IDOMImplementationRegistry::GetDOMImplementationRegistry ();
			AdobeXMPCore::spIDOMSerializer spSerializer = spRegistry->GetSerializer ( "rdf" );
			return (spSerializer->Serialize ( result->GetMergedMetadata() ));

		}
		return spcIUTF8String();
	}
    
    SXMPMeta APICALL ThreeWayMergeImpl::AutoMerge ( const SXMPMeta * baseVersionMetadata, const SXMPMeta * latestVersionMetadata, const SXMPMeta * userVersionMetadata, bool& isMerged , eAutoMergeStrategy skipAutoMergeStrategy ) const
    {
        AdobeXMPCore::spIMetadata spBaseVersion = IMetadataConverterUtils::ConvertXMPMetatoIMetadata(baseVersionMetadata);
        AdobeXMPCore::spIMetadata spLatestVersion = IMetadataConverterUtils::ConvertXMPMetatoIMetadata(latestVersionMetadata);
        AdobeXMPCore::spIMetadata spUserVersion = IMetadataConverterUtils::ConvertXMPMetatoIMetadata(userVersionMetadata);
        
        spcIThreeWayMergeResult result = AutoMerge ( spBaseVersion , spLatestVersion, spUserVersion, skipAutoMergeStrategy );
        
        if ( result->CountOfResolvedConflicts () != 0 && result->CountOfUnResolvedConflicts () == 0 )
            isMerged = true;
        else
            isMerged = false;
        if ( result->GetMergedStatus() != IThreeWayMergeResult::kMSConflictsPending )
        {
            return IMetadataConverterUtils::ConvertIMetadatatoXMPMeta(result->GetMergedMetadata());
        }
        return SXMPMeta();

    }
}

namespace AdobeXMPCompareAndMerge {
	using namespace AdobeXMPCompareAndMerge_Int;

	spIThreeWayMerge IThreeWayMerge_v1::MakeShared( pIThreeWayMerge_base ptr ) {
		if ( !ptr ) return spIThreeWayMerge();
		pIThreeWayMerge p = IThreeWayMerge::GetInterfaceVersion() > 1 ? ptr->GetInterfacePointer< IThreeWayMerge >() : ptr;
		return MakeUncheckedSharedPointer( p, __FILE__, __LINE__, false );
	}

	spIThreeWayMerge IThreeWayMerge_v1::CreateThreeWayMerge() {
		return MakeUncheckedSharedPointer( new ThreeWayMergeImpl(), __FILE__, __LINE__, true );
	}
}
