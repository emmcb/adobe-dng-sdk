#include "MetaOpsWrapper.hpp"
#if __cplusplus
extern "C"{
#endif
    
    bool Initialize ( XMP_Int32* id, char* msg)
    {
		XMPOps_Assert_2(id, msg);
        TRY
            return SXMPMeta::Initialize();
        }
        catch (XMP_Error e) {
            *id=e.GetID();
            memcpy(msg, e.GetErrMsg(), strlen(e.GetErrMsg()));
            msg[strlen(e.GetErrMsg())] = '\0';
            
        }
        catch (...) {
            *id=kXMPOpsErr_InternalFailure;
            std::string message="Internal Failure";
            memcpy(msg,message.c_str() ,message.length() );
            msg[message.length()] = '\0';
        }
        return false;
    }
    
    void Terminate ()
    {
        SXMPMeta::Terminate();
    }
    
    XMPMetaOps* CreateMetaOps(XMP_Int32* id, char* msg)
    {
        XMPOps_Assert_2(id, msg);
		TRY
        return new XMPMetaOps();
        XMPOPS_CATCH_EXCEPTIONS
        return 0;
    }
    
    XMPMetaOps* CreateMetaOpsFrom(XMPMetaOps* metaops, XMP_Int32* id, char* msg)
    {
		XMPOps_Assert_3(metaops, id, msg);
        TRY
        return new XMPMetaOps(metaops);
        XMPOPS_CATCH_EXCEPTIONS
        return 0;
    }
    
    void SetMetadataFromStream(XMPMetaOps *metaops, XMP_StringPtr xmpString, XMP_Int32* id, char* msg)
    {
        XMPOps_Assert_3(metaops, id, msg);
		TRY
        metaops->SetMetadataFromStream(xmpString);
        XMPOPS_CATCH_EXCEPTIONS
    }
    
    void SetCreatorTool ( XMPMetaOps *metaops,XMP_StringPtr creatorTool,XMP_Int32* id, char* msg )
    {
		XMPOps_Assert_3(metaops, id, msg);
        TRY
        metaops->SetCreatorTool(creatorTool);
        XMPOPS_CATCH_EXCEPTIONS
    }
    
    /*SXMPMeta GetMetadata(XMPMetaOps *metaops){
     return metaops->GetMetadata();
     }*/
    
    void SetMetadata(XMPMetaOps* metaops,XMPMetaOps* other, XMP_Int32* id, char* msg){
		XMPOps_Assert_4(metaops, other, id, msg);
		TRY
        metaops->SetMetadata(other);
        XMPOPS_CATCH_EXCEPTIONS
    }
    
    void ClearUpdatableHistory(XMPMetaOps *metaops, XMP_Int32* id, char* msg){
       
        XMPOps_Assert_3(metaops, id, msg);
		TRY
        metaops->ClearUpdatableHistory();
        XMPOPS_CATCH_EXCEPTIONS
    }
    
    XMPMetaOps* Clone(XMPMetaOps *metaops, XMP_Int32* id, char* msg)
    {
        
        XMPOps_Assert_3(metaops, id, msg);
		TRY
        return metaops->Clone();
        XMPOPS_CATCH_EXCEPTIONS
        return 0;
    }
    
    void SetProperty ( XMPMetaOps *metaops,XMP_StringPtr    schemaNS,
                      XMP_StringPtr    propName,
                      XMP_StringPtr    propValue, XMP_Int32* id, char* msg)
    {
        
        XMPOps_Assert_3(metaops, id, msg);
		TRY
        metaops->SetProperty(schemaNS, propName, propValue);
        XMPOPS_CATCH_EXCEPTIONS
    }
    
    bool GetProperty (XMPMetaOps *metaops,XMP_StringPtr    schemaNS,
                      XMP_StringPtr    propName,
                      char* propVal, XMP_Int32* size,  XMP_Int32* id, char* msg)
    {
        
        XMPOps_Assert_4(metaops, size, id, msg);
		TRY
        bool getProp;
        std::string propValue;
        getProp=metaops->GetProperty(schemaNS, propName,&propValue);
        *size = propValue.size() + 1;
        if (propVal == NULL)
            return true;
        memcpy(propVal, propValue.c_str(), propValue.length());
        propVal[propValue.length()] = '\0';
        return getProp;
        XMPOPS_CATCH_EXCEPTIONS
        return false;
    }
    
    void DeleteProperty ( XMPMetaOps *metaops,XMP_StringPtr    schemaNS,
                         XMP_StringPtr propName, XMP_Int32* id, char* msg)
    {
        
        XMPOps_Assert_3(metaops, id, msg);
		TRY
        metaops->DeleteProperty(schemaNS, propName);
        XMPOPS_CATCH_EXCEPTIONS
    }
    
    bool UpdateMetadata(XMPMetaOps *metaops, XMP_StringPtr createdStr, XMP_StringPtr modifiedStr, XMP_StringPtr metaId, XMP_StringPtr type, XMP_Int32* id, char* msg) {
        
        XMPOps_Assert_3(metaops, id, msg);
		TRY
        return metaops->UpdateMetadata(createdStr, modifiedStr, metaId, type );
        XMPOPS_CATCH_EXCEPTIONS
        return 0;
    }
    
    /* bool UpdateMetadataWithoutType(XMPMetaOps *metaops, XMP_StringPtr dateStr, XMP_StringPtr elementId, XMP_Int32* id, char* msg) {
     TRY
     XMPOps_Assert_2(metaops,msg);
     return metaops->UpdateMetadata(dateStr, elementId );
     XMPOPS_CATCH_EXCEPTIONS
     return 0;
     }*/
    
    void MakeDerivedWithAction(XMPMetaOps *metaops, XMPMetaOps* deriveFrom, XMP_StringPtr action, XMP_StringPtr docId, XMP_Int32* id, char* msg)
    {
        
        XMPOps_Assert_4(metaops, deriveFrom, id, msg);
		TRY
        metaops->MakeDerivedWithAction(deriveFrom, action, docId);
        XMPOPS_CATCH_EXCEPTIONS
    }
    
    void AppendHistoryEvent(XMPMetaOps *metaops,XMP_StringPtr actionStr, XMP_StringPtr param, XMP_StringPtr when, bool updateLast,char* instanceId, XMP_Int32* id, char* msg){
        
        XMPOps_Assert_4(metaops, instanceId, id, msg);
		TRY
        std::string historyevent = metaops->AppendHistoryEvent(actionStr, param, when, updateLast);
        memcpy(instanceId, historyevent.c_str(), historyevent.length());
        instanceId[historyevent.length()] = '\0';
        XMPOPS_CATCH_EXCEPTIONS
    }
    
    /* XMP_StringPtr AppendHistoryEventWithDateOnly(XMPMetaOps *metaops,XMP_StringPtr actionStr, XMP_StringPtr param, XMP_StringPtr when, XMP_Int32* id, char* msg){
     TRY
     XMPOps_Assert_2(metaops,msg);
     std::string historyevent = metaops->AppendHistoryEvent(actionStr, param, when);
     return historyevent.c_str();
     XMPOPS_CATCH_EXCEPTIONS
     return 0;
     }
     
     XMP_StringPtr AppendHistoryEventWithoutDate(XMPMetaOps *metaops,XMP_StringPtr actionStr, XMP_StringPtr param, XMP_Int32* id, char* msg){
     TRY
     XMPOps_Assert_2(metaops,msg);
     std::string historyevent = metaops->AppendHistoryEvent(actionStr, param);
     return historyevent.c_str();
     XMPOPS_CATCH_EXCEPTIONS
     return 0;
     }*/
    
    void AppendSavedEvent(XMPMetaOps *metaops, XMP_Int32* id, char* msg) {
        
        XMPOps_Assert_3(metaops, id, msg);
		TRY
        metaops->AppendSavedEvent();
        XMPOPS_CATCH_EXCEPTIONS
    }
    
    void AssignNewDocumentId(XMPMetaOps *metaops, XMP_Int32* id, char* msg)
    {
        
        XMPOps_Assert_3(metaops, id, msg);
		TRY
        metaops->AssignNewDocumentId();
        XMPOPS_CATCH_EXCEPTIONS
    }
    
    void AddOriginURL(XMPMetaOps *metaops,XMP_StringPtr originURL, XMP_StringPtr manageUiUrl, XMP_Int32* id, char* msg){
        
        XMPOps_Assert_3(metaops, id, msg);
		TRY
        metaops->AddOriginURL(originURL, manageUiUrl);
        XMPOPS_CATCH_EXCEPTIONS
    }
    
    void AddLicenseInfo (XMPMetaOps *metaops,XMP_StringPtr licenseURL,XMP_StringPtr licenseName, XMP_StringPtr attributionURL, XMP_StringPtr attributionName, XMP_Int32* id, char* msg){
        
        XMPOps_Assert_3(metaops, id, msg);
		TRY
        metaops->AddLicenseInfo(licenseURL, licenseName, attributionURL, attributionName);
        XMPOPS_CATCH_EXCEPTIONS
    }
    
    void DestroyMetaOps(XMPMetaOps* metaops)
    {
        //TRY
        XMPOps_Assert_1(metaops);
        delete metaops;
    }
    
    XMP_Int32 SerializeToBuffer (XMPMetaOps *metaops,XMP_OptionBits options,char* buffstr, XMP_Int32* id, char* msg)
    {
        
        XMPOps_Assert_3(metaops, id, msg);
		TRY
        std::string buffer=metaops->SerializeToBuffer(options);
        if (buffstr == NULL) {
            return buffer.size() + 1;
        }
        memcpy(buffstr, buffer.c_str(), buffer.length());
        buffstr[buffer.length()] = '\0';
        return buffer.size() + 1;
        XMPOPS_CATCH_EXCEPTIONS
        return 0;
    }
    
    void RemovePublishingInfo(XMPMetaOps* metaops, XMP_Int32* id, char* msg)
    {
        
        XMPOps_Assert_3(metaops, id, msg);
		TRY
        metaops->RemovePublishingInfo();
        XMPOPS_CATCH_EXCEPTIONS
    }
    
    bool HasTag(XMPMetaOps* metaops, XMP_StringPtr tag, XMP_Int32* id, char* msg)
    {
        
        XMPOps_Assert_3(metaops, id, msg);
		TRY
        return metaops->HasTag(tag);
        XMPOPS_CATCH_EXCEPTIONS
        return 0;
    }
    
    void AddTags(XMPMetaOps* metaops, std::vector<std::string> tags, XMP_Int32* id, char* msg)
    {
        
        XMPOps_Assert_3(metaops, id, msg);
		TRY
        metaops->AddTags(tags);
        XMPOPS_CATCH_EXCEPTIONS
    }
    
    void AddTag(XMPMetaOps* metaops, XMP_StringPtr tag, XMP_Int32* id, char* msg)
    {
        
        XMPOps_Assert_3(metaops, id, msg);
		TRY
        metaops->AddTag(tag);
        XMPOPS_CATCH_EXCEPTIONS
    }

  /*  void InsertHistoryItem(XMPMetaOps* metaops,SXMPMeta* toXMP, int toIndex, const SXMPMeta* fromXMP, int fromIndex, XMP_Int32* id, char* msg)
    {
        TRY
        XMPOps_Assert_2(metaops,msg);
        metaops->InsertHistoryItem(toXMP, toIndex, fromXMP, fromIndex);
        XMPOPS_CATCH_EXCEPTIONS
    }*/
    
    bool MergeMetadataFrom(XMPMetaOps* metaops, XMPMetaOps* from, XMPMetaOps* base, XMP_Int32* id, char* msg)
    {
        
        XMPOps_Assert_3(metaops, id, msg);
		TRY
        return metaops->MergeMetadataFrom(from, base);
        XMPOPS_CATCH_EXCEPTIONS
        return 0;
    }
    
    void SetLocalizedTextWithPath(XMPMetaOps* metaops, XMP_StringPtr ns, XMP_StringPtr altTextName, XMP_StringPtr genericLang, XMP_StringPtr specificLang, XMP_StringPtr itemValue, XMP_Int32* id, char* msg)
    {
        
        XMPOps_Assert_3(metaops, id, msg);
		TRY
        metaops->SetLocalizedTextWithPath(ns, altTextName, genericLang, specificLang, itemValue);
        XMPOPS_CATCH_EXCEPTIONS
    }
    
#if __cplusplus
}
#endif

