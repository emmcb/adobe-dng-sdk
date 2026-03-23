    // Provide access to the DocOps functions
    //#include "XMPCore/source/XMPDocOps-Utils.cpp"
#include "MetaOps.hpp"
#include <algorithm>

    typedef XMP_Uns8 BinaryID [16];
/*
 Helper function starts here
 */
// =================================================================================================

#if XMP_MacBuild | XMP_iOSBuild

#include <CoreFoundation/CoreFoundation.h>

static void CreateBinaryID ( BinaryID * binID ) {
    //XMP_Assert ( sizeof ( BinaryID ) == sizeof ( CFUUIDBytes ) );
    CFUUIDRef cfID = CFUUIDCreate ( kCFAllocatorDefault );
    *((CFUUIDBytes*)binID) = CFUUIDGetUUIDBytes ( cfID );
    CFRelease ( cfID );
}

#elif XMP_WinBuild

#include <Windows.h>
#include "Objbase.h"
#include "rpcdce.h"

static void CreateBinaryID ( BinaryID * binID ) {
    //XMP_Assert ( sizeof ( BinaryID ) == sizeof ( UUID ) );
    HRESULT err = CoCreateGuid( (UUID*)binID );
    if ( err != S_OK ) {
        //XMP_Throw ( "Failure from UuidCreate", kXMPErr_ExternalFailure );
    }
    
    
}

#elif XMP_UNIXBuild | XMP_AndroidBuild

#include <boost/uuid/uuid.hpp>            
#include <boost/uuid/uuid_generators.hpp> 


	static void CreateBinaryID(BinaryID * binID) {
		boost::uuids::random_generator generator;
		boost::uuids::uuid uuid = generator();

		XMP_Index index = 0;
		for (boost::uuids::uuid::const_iterator it = uuid.begin(); it != uuid.end(); ++it) {
			(*binID)[index] = *it;
			index++;
			if (index > 15)
				break;

		}

	}

#endif


static const char * Hex = "0123456789abcdef";
static void FormatByte(XMP_Uns8 b, char * ch) {
    ch[0] = Hex[b >> 4];
    ch[1] = Hex[b & 0xF];
}
static void CreateID(std::string& id) {
    BinaryID binId;
    CreateBinaryID(&binId);
    
    
    //size_t prefixLen = strlen(prefix);
    id.reserve(36);    // ...:12345678-1234-1234-1234-123456789012
    
    id += "12345678-1234-1234-1234-123456789012";
    
    // Replace the suffix template with the actual ID.
    
    XMP_Uns8 * b = &binId[0];
    char * ch = &((id)[0]);    // Should point to the first character of the suffix.
    //XMP_Assert((*(ch - 1) == ':') && (*ch == '1'));
    
    for (XMP_Int32 i = 0; i < 4; ++i, ++b, ch += 2) FormatByte(*b, ch);
    ++ch;    // Skip the '-'.
    for (XMP_Int32 i = 0; i < 2; ++i, ++b, ch += 2) FormatByte(*b, ch);
    ++ch;    // Skip the '-'.
    for (XMP_Int32 i = 0; i < 2; ++i, ++b, ch += 2) FormatByte(*b, ch);
    ++ch;    // Skip the '-'.
    for (XMP_Int32 i = 0; i < 2; ++i, ++b, ch += 2) FormatByte(*b, ch);
    ++ch;    // Skip the '-'.
    for (XMP_Int32 i = 0; i < 6; ++i, ++b, ch += 2) FormatByte(*b, ch);
    
}

        XMPMetaOps::XMPMetaOps()
        {
            try{
            SXMPUtils::ComposeArrayItemPath(kXMP_NS_XMP_MM, "History", kXMP_ArrayLastItem, &historyLast);
            }
            catch (XMP_Error &exception) {
                throw XMPOpsException(kXMPOpsErr_XMPException, exception.GetErrMsg() );
            }
        }

       /* XMPMetaOps::XMPMetaOps(SXMPMeta meta)
        {
            SXMPUtils::ComposeArrayItemPath(kXMP_NS_XMP_MM, "History", kXMP_ArrayLastItem, &historyLast);
            this->meta = meta;
        
        }
    */
        XMPMetaOps::XMPMetaOps(XMPMetaOps* metaops)
        {
            try{
            SXMPUtils::ComposeArrayItemPath(kXMP_NS_XMP_MM, "History", kXMP_ArrayLastItem, &historyLast);
            this->meta = metaops->GetMetadata().Clone();
            this->updatableHistoryId = metaops->updatableHistoryId;
            }
            catch (XMP_Error &exception) {
                throw XMPOpsException(kXMPOpsErr_XMPException, exception.GetErrMsg() );
            }
        }

        XMPMetaOps::~XMPMetaOps(){}

        std::string XMPMetaOps::SerializeToBuffer (XMP_OptionBits options)
        {
            try{
            std::string buffer;     //exception of type ...
            meta.SerializeToBuffer(&buffer, options);
            return buffer;
            }
            catch (XMP_Error &exception) {
                throw XMPOpsException(kXMPOpsErr_XMPException, exception.GetErrMsg() );
            }
        }

        void XMPMetaOps::SetMetadataFromStream ( std::string xmpString )
        {
            try{
            SXMPMeta meta_xmpString(xmpString.c_str(),kXMP_UseNullTermination);
            this->meta = meta_xmpString;
            //XMPMetaOps* metaops = new XMPMetaOps(meta_xmpString);
            CheckForHistoryId();  //checkForHistoryid as in assetops
            //return metaops;
            }
            catch (XMP_Error &exception) {
                throw XMPOpsException(kXMPOpsErr_XMPException, exception.GetErrMsg() );
            }
        }

		void XMPMetaOps::SetMetadataFromMeta(SXMPMeta meta)
		{
			this->meta = meta;
		}

        SXMPMeta XMPMetaOps::GetMetadata()
        {
            return this->meta;
        }

        XMPMetaOps* XMPMetaOps::Clone()
        {
            try{
            return new XMPMetaOps(this);
            }
            catch (XMP_Error &exception) {
                throw XMPOpsException(kXMPOpsErr_XMPException, exception.GetErrMsg() );
            }
        }

        void XMPMetaOps::SetMetadata(XMPMetaOps* other)
        {
            
            if(other==0 /*|| other->GetMetadata()*/){
                // throw exception here
                throw  XMPOpsException(kXMPOpsErr_BadValue, "Input parameter is not valid");
            }
           this->meta = other->GetMetadata();
        }

        void XMPMetaOps::SetCreatorTool (std::string creator)
        {
            this->creatorTool=creator;
        }

        void XMPMetaOps::ClearUpdatableHistory(){
            updatableHistoryId = "";
        }


        bool XMPMetaOps::UpdateMetadata(std::string createdStr, std::string modifiedStr, std::string id, std::string type)
        {
            try{
                /*ISO8601Converter.parse(createdStr);
                ISO8601Converter.parse(modifiedStr);*/
                XMP_DateTime time;
                std::string created = createdStr;
                if (!createdStr.empty()) {
                    SXMPUtils::ConvertToDate(createdStr, &time);
                    SXMPUtils::ConvertFromDate(time, &created);
                }

                std::string modified = "";
                SXMPUtils::ConvertToDate(modifiedStr, &time);
                SXMPUtils::ConvertFromDate(time,&modified);
            
                if (created.empty())
                    created = modified;

                meta.SetProperty(kXMP_NS_XMP,"CreatorTool",creatorTool);
                meta.SetProperty(kXMP_NS_XMP,"CreateDate",created);
                meta.SetProperty(kXMP_NS_XMP,"ModifyDate",modified);
                meta.SetProperty(kXMP_NS_XMP,"MetadataDate",modified);
            
                if(type.length() > 0)
                    meta.SetProperty(kXMP_NS_DC,"format",type);
            
                meta.SetProperty(kXMP_NS_XMP_MM,"DocumentID",id);
                meta.SetProperty(kXMP_NS_XMP_MM,"OriginalDocumentID",id);
                meta.SetProperty(kXMP_NS_XMP_MM,"InstanceID",id);
                    
                meta.AppendArrayItem(kXMP_NS_XMP_MM, "History", kXMP_PropArrayIsOrdered,NULL,kXMP_PropValueIsStruct);
            
                meta.SetStructField(kXMP_NS_XMP_MM, historyLast.c_str(), kXMP_NS_XMP_ResourceEvent, "action","created");
                meta.SetStructField(kXMP_NS_XMP_MM, historyLast.c_str(), kXMP_NS_XMP_ResourceEvent, "instanceID",id);
                meta.SetStructField(kXMP_NS_XMP_MM, historyLast.c_str(), kXMP_NS_XMP_ResourceEvent, "when",modifiedStr);
                meta.SetStructField(kXMP_NS_XMP_MM, historyLast.c_str(), kXMP_NS_XMP_ResourceEvent, "softwareAgent",creatorTool);
            }
            catch (XMP_Error &exception) {
                throw XMPOpsException(kXMPOpsErr_XMPException, exception.GetErrMsg() );
            }
            return true;
        }

        /*bool XMPMetaOps::UpdateMetadata(std::string dateStr, std::string id)
        {
            try{
                //ISO8601Converter.parse(dateStr);
                XMP_DateTime time;
                SXMPUtils::ConvertToDate(dateStr, &time);
                std::string date;
                SXMPUtils::ConvertFromDate(time,&date);
            
                meta.SetProperty(kXMP_NS_XMP, "CreateDate", date);
                meta.SetProperty(kXMP_NS_XMP, "ModifyDate", date);
                meta.SetProperty(kXMP_NS_XMP, "MetadataDate", date);
            
                meta.SetProperty(kXMP_NS_XMP_MM, "DocumentID", id);
                meta.SetProperty(kXMP_NS_XMP_MM, "OriginalDocumentID", id);
                meta.SetProperty(kXMP_NS_XMP_MM, "InstanceID", id);
            
                meta.AppendArrayItem(kXMP_NS_XMP_MM, "History", kXMP_PropArrayIsOrdered,NULL, kXMP_PropValueIsStruct);
            
                meta.SetStructField(kXMP_NS_XMP_MM, historyLast.c_str(), kXMP_NS_XMP_ResourceEvent, "action", "created");
                meta.SetStructField(kXMP_NS_XMP_MM, historyLast.c_str(), kXMP_NS_XMP_ResourceEvent, "instanceID", id);
                meta.SetStructField(kXMP_NS_XMP_MM, historyLast.c_str(), kXMP_NS_XMP_ResourceEvent, "when", dateStr);
                meta.SetStructField(kXMP_NS_XMP_MM, historyLast.c_str(), kXMP_NS_XMP_ResourceEvent, "softwareAgent",creatorTool);
            
                meta.SetProperty(kXMP_NS_XMP, "CreatorTool", creatorTool); //why delete DeleteExisting Flag
            }
            catch (XMP_Error &exception) {
                throw XMPOpsException(kXMPOpsErr_XMPException, exception.GetErrMsg() );
            }
            
            return true;
            
        }*/

        void XMPMetaOps::MakeDerivedWithAction(XMPMetaOps* deriveFrom, std::string action, std::string docId)
        {
            try{
            if (docId == "") {
                CreateID(docId);
            }
            
                std::string srcDocId;
            this->meta = deriveFrom->GetMetadata().Clone();
            meta.GetProperty(kXMP_NS_XMP_MM, "DocumentID", &srcDocId, NULL);
            std::string originalDocId;
            meta.GetProperty(kXMP_NS_XMP_MM, "OriginalDocumentID", &originalDocId, NULL);
            if (originalDocId.empty()) {
                originalDocId = srcDocId;
            }
            std::string originalInstanceId;
            meta.GetProperty(kXMP_NS_XMP_MM, "InstanceID", &originalInstanceId, NULL);
            std::string originalModifiedTime;
            meta.GetProperty(kXMP_NS_XMP, "ModifyDate", &originalModifiedTime, NULL);
            if(!srcDocId.empty())
                meta.SetStructField(kXMP_NS_XMP_MM, "DerivedFrom", kXMP_NS_XMP_ResourceRef, "documentID", srcDocId);
            if(!originalDocId.empty())
                meta.SetStructField(kXMP_NS_XMP_MM, "DerivedFrom", kXMP_NS_XMP_ResourceRef, "originalDocumentID", originalDocId);
            if(!originalInstanceId.empty())
                meta.SetStructField(kXMP_NS_XMP_MM, "DerivedFrom", kXMP_NS_XMP_ResourceRef, "instanceID", originalInstanceId);
            if(!originalModifiedTime.empty())
                meta.SetStructField(kXMP_NS_XMP_MM, "DerivedFrom", kXMP_NS_XMP_ResourceRef, "lastModifyDate", originalModifiedTime);
            
            meta.SetProperty(kXMP_NS_XMP_MM, "DocumentID", docId, kXMP_DeleteExisting);
            
            // The current app is creating the derived document.
            //std::string agent = self.class.creatorTool.UTF8String;
            meta.SetProperty(kXMP_NS_XMP, "CreatorTool", creatorTool, kXMP_DeleteExisting);
        
            std::string now;
            XMP_DateTime time;
            SXMPUtils::CurrentDateTime(&time);
            SXMPUtils::ConvertFromDate(time,&now);
            AppendHistoryEvent(action,NULL,now,false);
            }
            catch (XMP_Error &exception) {
                throw XMPOpsException(kXMPOpsErr_XMPException, exception.GetErrMsg() );
            
            }
        }

        std::string XMPMetaOps::AppendHistoryEvent(std::string actionStr, std::string param, std::string when, bool updateLast)
        {
            try{
            //std::string actionStr = action.UTF8String;
            if (when.empty())
            {
                XMP_DateTime time;
                SXMPUtils::CurrentDateTime(&time);
                SXMPUtils::ConvertFromDate(time, &when);
            }
                std::string instanceId;
            CreateID(instanceId);
            //std::string agent = self.class.creatorTool.UTF8String;
            //std::string dateStr = [self.class.dateFormatter stringFromDate:when].UTF8String;
            
            meta.SetProperty(kXMP_NS_XMP_MM, "InstanceID", instanceId, kXMP_DeleteExisting);
            meta.SetProperty(kXMP_NS_XMP, "ModifyDate", when, kXMP_DeleteExisting);
            meta.SetProperty(kXMP_NS_XMP, "MetadataDate", when, kXMP_DeleteExisting);

            bool exist = meta.DoesPropertyExist(kXMP_NS_XMP_MM, "History");
            
            if (!updateLast || !exist) {
                meta.AppendArrayItem(kXMP_NS_XMP_MM, "History", kXMP_PropArrayIsOrdered, NULL, kXMP_PropValueIsStruct);
            }
            
            meta.SetStructField(kXMP_NS_XMP_MM, historyLast.c_str(), kXMP_NS_XMP_ResourceEvent, "action", actionStr);
            // xmp->SetStructField(kXMP_NS_XMP_MM, historyLast.c_str(), kXMP_NS_XMP_ResourceEvent, "documentID", documentId);
            meta.SetStructField(kXMP_NS_XMP_MM, historyLast.c_str(), kXMP_NS_XMP_ResourceEvent, "instanceID", instanceId);
            meta.SetStructField(kXMP_NS_XMP_MM, historyLast.c_str(), kXMP_NS_XMP_ResourceEvent, "when", when);
            meta.SetStructField(kXMP_NS_XMP_MM, historyLast.c_str(), kXMP_NS_XMP_ResourceEvent, "softwareAgent", creatorTool);
            
            return instanceId;
            }
            catch (XMP_Error &exception) {
                throw XMPOpsException(kXMPOpsErr_XMPException, exception.GetErrMsg() );
            }
        }

       /* std::string XMPMetaOps::AppendHistoryEvent(std::string actionStr, std::string param, std::string when)
        {
            return AppendHistoryEvent(actionStr, param, when,false);
        }

        std::string XMPMetaOps::AppendHistoryEvent(std::string actionStr, std::string param)
        {
            try{
            std::string now;
            XMP_DateTime time;
            SXMPUtils::CurrentDateTime(&time);
            SXMPUtils::ConvertFromDate(time,&now);
            return AppendHistoryEvent(actionStr, param, now);
            }
            catch (XMP_Error &exception) {
                throw XMPOpsException(kXMPOpsErr_XMPException, exception.GetErrMsg() );
            }
        }*/

        void XMPMetaOps::CheckForHistoryId()
        {
            //XMPProperty prop = NULL;
            try{
            bool prop;
            std::string propValue = "";
           
            prop =  meta.GetStructField(kXMP_NS_XMP_MM, historyLast.c_str(), kXMP_NS_XMP_ResourceEvent, "action",&propValue,0);
    
                if(propValue!="" && propValue.compare("saved")==0){
                    prop = meta.GetStructField(kXMP_NS_XMP_MM, historyLast.c_str(), kXMP_NS_XMP_ResourceEvent, "softwareAgent",&propValue,0);
                    
                        if(!propValue.empty() && propValue.compare(creatorTool)==0){
                            prop =  meta.GetStructField(kXMP_NS_XMP_MM, historyLast.c_str(), kXMP_NS_XMP_ResourceEvent, "instanceID",&propValue,0);
                            if(prop){
                                updatableHistoryId = propValue;
                            }
                        }
                }
            }
            catch (XMP_Error &exception) {
                throw XMPOpsException(kXMPOpsErr_XMPException, exception.GetErrMsg() );
            }
        }

        void XMPMetaOps::AppendSavedEvent()
        {
            try{
            // Update or append a "saved" history event
            CheckForHistoryId();
            std::string str;
            bool updateLast = false;
            if (meta.GetStructField(kXMP_NS_XMP_MM, historyLast.c_str(), kXMP_NS_XMP_ResourceEvent, "instanceID", &str, NULL)) {
                if (str == updatableHistoryId) {
                    updateLast = true;
                }
            }
            XMP_DateTime time;
            std::string now;
            SXMPUtils::CurrentDateTime(&time);
            SXMPUtils::ConvertFromDate(time,&now);
            updatableHistoryId = AppendHistoryEvent("saved", NULL, now, updateLast);
            }
            catch (XMP_Error &exception) {
                throw XMPOpsException(kXMPOpsErr_XMPException, exception.GetErrMsg() );
            }
        }

        void XMPMetaOps::AssignNewDocumentId()
        {
            try{
            std::string originalInstanceId;
            meta.GetProperty(kXMP_NS_XMP_MM, "InstanceID", &originalInstanceId, NULL);
            std::string newDocId;
            CreateID(newDocId);
            //std::string newDocId = "[AdobeCommonUtils generateUUID].UTF8String";
            meta.SetProperty(kXMP_NS_XMP_MM, "DocumentID", newDocId, kXMP_DeleteExisting);
            meta.SetProperty(kXMP_NS_XMP_MM, "InstanceID", newDocId, kXMP_DeleteExisting);
            
            // Update the last history record if it has the old instance ID
            std::string value;
            XMP_OptionBits options;
            meta.GetStructField(kXMP_NS_XMP_MM, historyLast.c_str(), kXMP_NS_XMP_ResourceEvent, "instanceID", &value, &options);
            if (!value.empty() && value == originalInstanceId) {
                meta.SetStructField(kXMP_NS_XMP_MM, historyLast.c_str(), kXMP_NS_XMP_ResourceEvent, "instanceID", newDocId);
                // to discuss the following
                meta.SetStructField(kXMP_NS_XMP_MM, historyLast.c_str(), kXMP_NS_XMP_ResourceEvent, "documentID", newDocId);
            }
            }
            catch (XMP_Error &exception) {
                throw XMPOpsException(kXMPOpsErr_XMPException, exception.GetErrMsg() );
            }
        }

        void XMPMetaOps::AddOriginURL(std::string originURL, std::string manageUiUrl)
        {
            try{
            if (!originURL.empty()) {
                meta.SetProperty(kXMP_NS_XMP_MM, "ManageTo", originURL, kXMP_DeleteExisting);
            }
            if (!manageUiUrl.empty()) {
                meta.SetProperty(kXMP_NS_XMP_MM, "ManageUI", manageUiUrl, kXMP_DeleteExisting);
            }
            }
            catch (XMP_Error &exception) {
                throw XMPOpsException(kXMPOpsErr_XMPException, exception.GetErrMsg() );
            }
        }

        void XMPMetaOps::AddLicenseInfo (std::string licenseURL,std::string licenseName, std::string attributionURL, std::string attributionName)
        {
            try{
            std::string ns;
            SXMPMeta::RegisterNamespace("http://creativecommons.org/ns#", "cc",&ns);
            //
            // Map Creative Commons license abbreviation to url and name
            //
            
           
                if(licenseURL != "")
                    meta.SetProperty("http://creativecommons.org/ns#", "license", licenseURL);
                if(licenseURL != "" && licenseName != ""){
                    meta.SetProperty(kXMP_NS_XMP_Rights, "Marked", "True");
                    std::string usageTerms = "This work is licensed to the public under the Creative Commons "+licenseName+" license "+licenseURL;
                    meta.DeleteProperty(kXMP_NS_XMP_Rights, "UsageTerms");
                    meta.SetLocalizedText(kXMP_NS_XMP_Rights,"UsageTerms" , "x-default", "x-default", usageTerms);
                    meta.DeleteProperty(kXMP_NS_DC, "rights");
                    meta.SetLocalizedText(kXMP_NS_DC,"rights" , "x-default", "x-default", usageTerms);
                }
                if(attributionURL != "")
                    meta.SetProperty("http://creativecommons.org/ns#", "attributionURL", attributionURL);
                if(attributionName != "")
                    meta.SetProperty("http://creativecommons.org/ns#", "attributionName", attributionName);
            }
            catch (XMP_Error &exception) {
                throw XMPOpsException(kXMPOpsErr_XMPException, exception.GetErrMsg() );
            }
        }

        void XMPMetaOps::RemovePublishingInfo()
        {
            try{
            // When a published composition is remixed, these publishing properties are no longer
            // appropriate since they refer to the original work rather than the derived work.

			std::string ns;
			SXMPMeta::RegisterNamespace("http://creativecommons.org/ns#", "cc", &ns);
            meta.DeleteProperty("http://creativecommons.org/ns#", "license");
            meta.DeleteProperty(kXMP_NS_XMP_Rights, "Marked");
            meta.DeleteProperty(kXMP_NS_XMP_Rights, "UsageTerms");
            meta.DeleteProperty(kXMP_NS_DC, "rights");
            meta.DeleteProperty("http://creativecommons.org/ns#","attributionURL");
            meta.DeleteProperty("http://creativecommons.org/ns#","attributionName");
            meta.DeleteProperty(kXMP_NS_XMP_MM, "ManageTo");
            meta.DeleteProperty(kXMP_NS_XMP_MM, "ManageUI");
            }
            catch (XMP_Error &exception) {
                throw XMPOpsException(kXMPOpsErr_XMPException, exception.GetErrMsg() );
            }
        }

        void XMPMetaOps::AddTag(std::string tag)
        {
            //PropertyOptions opns = new PropertyOptions();
            //opns.setArray(true);
            try{
                if(!HasTag(tag))
                    meta.AppendArrayItem(kXMP_NS_DC, "subject", kXMP_PropArrayIsUnordered ,tag, NULL);
                    }
            catch (XMP_Error &exception) {
                throw XMPOpsException(kXMPOpsErr_XMPException, exception.GetErrMsg() );
            }
        }

        void XMPMetaOps::AddTags(std::vector<std::string> tags)
        {
            if (tags.size()>0) {
                for (auto tag : tags) {
                    AddTag(tag);
                }
            }
        }

        bool XMPMetaOps::HasTag(std::string tag)
        {
            try{
            SXMPIterator it(meta,kXMP_NS_DC,"subject",NULL);
            std::string ns,path,value;
                while(it.Next(&ns,&path,&value,NULL)){
                    if(tag.compare(value)==0)
                        return true;
                }
                return false;
            }
            catch (XMP_Error &exception) {
                throw XMPOpsException(kXMPOpsErr_XMPException, exception.GetErrMsg() );
            }
        }


        void XMPMetaOps::InsertHistoryItem(SXMPMeta* toXMP, XMP_Index toIndex, const SXMPMeta* fromXMP, XMP_Index fromIndex)
        {
            try{
            std::string fromPath;
            SXMPUtils::ComposeArrayItemPath(kXMP_NS_XMP_MM, "History", fromIndex, &fromPath);
            std::string toPath;
            SXMPUtils::ComposeArrayItemPath(kXMP_NS_XMP_MM, "History", toIndex+1, &toPath);
            
            SXMPIterator iterCurrent(*fromXMP, kXMP_NS_XMP_MM, fromPath.c_str());
            std::string schemaStr, nameStr, value;
            XMP_OptionBits options;
            if (!iterCurrent.Next(&schemaStr, &nameStr, &value, &options)) {
                // discard outer History element
                return;
            }
            toXMP->SetArrayItem(kXMP_NS_XMP_MM, "History", toIndex, NULL, kXMP_PropValueIsStruct|kXMP_InsertAfterItem);
            while (iterCurrent.Next(&schemaStr, &nameStr, &value, &options)) {
                // You're not really supposed to muck about in path expressions, but there doesn't seem to be another way.
                // The returned path is something like xmpMM:History[1]/stEvt:action but we want just the "action" part.
                std::string::size_type pos = nameStr.rfind(':');
                if (pos != std::string::npos) {
                    nameStr = nameStr.substr(pos+1);
                }
                toXMP->SetStructField(kXMP_NS_XMP_MM, toPath.c_str(), kXMP_NS_XMP_ResourceEvent, nameStr.c_str(), value);
            }
            }
            catch (XMP_Error &exception) {
                throw XMPOpsException(kXMPOpsErr_XMPException, exception.GetErrMsg() );
            }
        }

        //
        // Updates the pulled metadata using current and, if present, base metadata.
        //
        bool XMPMetaOps::MergeMetadataFrom(XMPMetaOps* from, XMPMetaOps* base)
        {
            try{
                if (from==NULL){
                    throw XMPOpsException(kXMPOpsErr_BadParam, "from argument cannot be NULL" );
                }
                

                const SXMPMeta fromXMP = from->GetMetadata();
                bool isBasePresent = base?true:false;
                //const SXMPMeta baseXMP = base ? base->GetMetadata() : 0;
                
                bool changed = false;
                
                // Visit each property in the pulled metadata and update from "current" and "base", if present,
                // to determine whether the pulled or current version is newer.
                SXMPIterator iter(meta);
                std::string schemaStr, nameStr, value, valueFrom, valueBase;
                XMP_OptionBits options, optionsFrom, optionsBase;
                while (iter.Next(&schemaStr, &nameStr, &value, &options)) {
                    // Ignore results with no name
                    if (nameStr.empty()) {
                        continue;
                    }
                    
                    // Ignore "History" for now
                    if (schemaStr == kXMP_NS_XMP_MM && nameStr == "xmpMM:History") {
                        iter.Skip(kXMP_IterSkipSubtree);
                        continue;
                    }
                    
                    
                    if (fromXMP.GetProperty(schemaStr.c_str(), nameStr.c_str(), &valueFrom, &optionsFrom)) {
                        // Property exists in "current". Is it different?
                        if (value != valueFrom || options != optionsFrom) {
                            // Value is different in current metadata.
                            // If there's no base or the pulled value hasn't changed, then take current value.
                            bool takeFrom = !isBasePresent;
                            
                            if (isBasePresent && base->GetMetadata().GetProperty(schemaStr.c_str(), nameStr.c_str(), &valueBase, &optionsBase)) {
                                if (value == valueBase && options == optionsBase) {
                                    takeFrom = true;
                                }
                            }
                            if (takeFrom) {
                                bool nullValue = optionsFrom & kXMP_PropCompositeMask;
                                try {
                                    meta.SetProperty(schemaStr.c_str(), nameStr.c_str(),
                                                       nullValue ? 0 : valueFrom.c_str(), optionsFrom);
                                    changed = true;
                                } catch (XMP_Error /*&e*/) {
                                   // many things can cause an exception, for instance incompatible options.
                                   // since SetProperty() does not allow qualifiers to be set in XMPCore, merging of qualifiers throws an exception.
                                   // Deatils are mentiones in AssetOps wiki.
                                   // throw XMPOpsException(e.GetID(), e.GetErrMsg() );
                                }
                            }
                        }
                    } else {
                        // Property doesn't exist in from metadata. Delete it if value is the same in base.
                        if (isBasePresent && base->GetMetadata().GetProperty(schemaStr.c_str(), nameStr.c_str(), &valueBase, &optionsBase)) {
                            if (value == valueBase && options == optionsBase) {
                                // Make sure we're not iterating in the thing we're deleting
                                iter.Skip(kXMP_IterSkipSubtree);
                                try {
                                    meta.DeleteProperty(schemaStr.c_str(), nameStr.c_str());
                                    changed = true;
                                } catch (XMP_Error &e) {
                                    // many things can cause an exception, for instance incompatible options.
                                    // nothing we can do.
                                    //NSLog(@"Unexpected XMP error%d, %s", e.GetID(), e.GetErrMsg());
                                    throw XMPOpsException(e.GetID(), e.GetErrMsg() ); //rev:throw??
                                }
                            }
                        }
                    }
                }
                // Visit the nodes of the "from" metadata to see if there are any additions
                SXMPIterator iterFrom(fromXMP);
                while (iterFrom.Next(&schemaStr, &nameStr, &valueFrom, &optionsFrom)) {
                    // Ignore results with no "name"
                    if (nameStr.empty()) {
                        continue;
                    }
                    // Ignore "History"
                    if (schemaStr == kXMP_NS_XMP_MM && nameStr == "xmpMM:History") {
                        iterFrom.Skip(kXMP_IterSkipSubtree);
                        continue;
                    }
                    if (!meta.GetProperty(schemaStr.c_str(), nameStr.c_str(), &value, &options)) {
                        // Property is in current but not in pulled. Is it an addition?
                        if (isBasePresent && !base->GetMetadata().GetProperty(schemaStr.c_str(), nameStr.c_str(), &valueBase, &optionsBase)) {
                            bool nullValue = optionsFrom & kXMP_PropCompositeMask;
                            try {
                                meta.SetProperty(schemaStr.c_str(), nameStr.c_str(),
                                                   nullValue ? 0 : valueFrom.c_str(), optionsFrom);
                                changed = true;
                            } catch (XMP_Error &e) {
                                // many things can cause an exception, for instance incompatible options.
                                //NSLog(@"Unexpected XMP error%d, %s", e.GetID(), e.GetErrMsg());
                                throw XMPOpsException(e.GetID(), e.GetErrMsg() ); //rev: throw?
                            }
                        }
                    }
                }
                
                // Now merge the histories of myXMP and "from"
                // Formerly, this code ran down both histories, merging and eliminating duplicates.
                // But this assumes that timestamps are in chronological order. Unfortunately,
                // that cannot be assumed. If they aren't then duplicates are not properly eliminated
                // and the history nearly doubles at each attempt ot merge.
                
                struct HistoryRecord {
                    XMP_DateTime timestamp;
                    std::string instanceId;
                    bool from;
                    XMP_Index index;
                    XMP_Index insertAfter;
                } historyItem;
                std::vector<HistoryRecord> history;
                
                // Collect history records
                historyItem.from = false;
                historyItem.index = 0;
                historyItem.insertAfter = -1;
                while (getNextHistoryRecord(&meta, &historyItem.index, &historyItem.instanceId, &historyItem.timestamp)) {
                    if (historyItem.timestamp.hasDate) {
                        history.push_back(historyItem);
                    }
                }
                
                historyItem.from = true;
                historyItem.index = 0;
                historyItem.insertAfter = 0;
                while (getNextHistoryRecord(&fromXMP, &historyItem.index, &historyItem.instanceId, &historyItem.timestamp)) {
                    if (historyItem.timestamp.hasDate) {
                        history.push_back(historyItem);
                    }
                }
                
                // Sort by time.
                // Because the sort is stable, the "to" entries are before the from entries of the same timestamp
                stable_sort(history.begin(), history.end(), [](const HistoryRecord& a, const HistoryRecord& b) {
                    XMP_Int32 i = compare(a.timestamp, b.timestamp);
                    return i < 0;
                });
                
                // Find which items to insert and where to insert them
                XMP_Index insertAfter = 0;
        for (size_t i = 0; i <history.size(); ++i) {
                    if (history[i].from) {
                        if (history[i].insertAfter != -1) {
                            history[i].insertAfter = insertAfter;
                        }
                    } else {
                        history[i].insertAfter = -1; // current items are not re-inserted
                        insertAfter = history[i].index;
                        // Scan forward among items with the same timestamp,
                        // marking "from" items with the same instanceId for deletion
                        for (size_t j = i+1; j < history.size() && compare(history[i].timestamp, history[j].timestamp) == 0; ++j) {
                            if (history[j].from && history[i].instanceId == history[j].instanceId) {
                                history[j].insertAfter = -1;
                            }
                        }
                    }
                }
                
                // Sort on insertion location
                stable_sort(history.begin(), history.end(), [](const HistoryRecord& a, const HistoryRecord& b) {
                    return a.insertAfter < b.insertAfter;
                });
                
                // Now go backwards through sorted history and add things in "from" that are not
                // in the current history
                for (XMP_Int32 i = (XMP_Int32)(history.size())-1; i >= 0; --i) {
                    if (history[i].insertAfter != -1) {
                        InsertHistoryItem(&meta, history[i].insertAfter, &fromXMP, history[i].index);
                        changed = true;
                    }
                }
                
                if (changed) {
                    AppendHistoryEvent("merged", NULL, NULL, false);
                }
                //dump(meta);
                return changed;
            }
            catch (XMP_Error &exception) {
                throw XMPOpsException(kXMPOpsErr_XMPException, exception.GetErrMsg() );
            }
        }

        bool XMPMetaOps::GetProperty( std::string ns, std::string name, std::string* propVal) {
            try{
              bool  prop  = meta.GetProperty(ns.c_str(), name.c_str(),propVal, NULL);
            return prop;
            }
            catch (XMP_Error &exception) {
                throw XMPOpsException(kXMPOpsErr_XMPException, exception.GetErrMsg() );
            }
        }

        void XMPMetaOps::SetProperty(std::string ns, std::string name, std::string value){
            try {
                meta.SetProperty(ns.c_str(), name.c_str(), value,0);
            } catch (XMP_Error &exception) {
                throw XMPOpsException(kXMPOpsErr_XMPException, exception.GetErrMsg() );
            }
        }

void XMPMetaOps::DeleteProperty(std::string ns, std::string name){
        try{
            meta.DeleteProperty(ns.c_str(), name.c_str());
        
        } catch (XMP_Error &exception) {
            throw XMPOpsException(kXMPOpsErr_XMPException, exception.GetErrMsg() );
        }
        }

        void XMPMetaOps::SetLocalizedTextWithPath(std::string ns, std::string altTextName, std::string genericLang, std::string specificLang, std::string itemValue){
            try {
                meta.SetLocalizedText(ns.c_str(), altTextName.c_str(), genericLang.c_str(), specificLang.c_str(), itemValue.c_str());
            } catch (XMP_Error &exception) {
                throw XMPOpsException(kXMPOpsErr_XMPException, exception.GetErrMsg() );
            }
        }

bool XMPMetaOps::getNextHistoryRecord(const SXMPMeta* xmp, XMP_Index *itemp, std::string *instanceIdp, XMP_DateTime *dateP)
        {
            try {
            std::string val, path;
            XMP_OptionBits options;
            *itemp += 1;
            SXMPUtils::ComposeArrayItemPath(kXMP_NS_XMP_MM, "History", *itemp, &path);
           
                if (xmp->GetStructField(kXMP_NS_XMP_MM, path.c_str(), kXMP_NS_XMP_ResourceEvent, "when", &val, &options)) {
                    SXMPUtils::ConvertToDate(val, dateP);
                    xmp->GetStructField(kXMP_NS_XMP_MM, path.c_str(), kXMP_NS_XMP_ResourceRef, "instanceID", instanceIdp, &options);
                    return true;
               }
            } catch (XMP_Error &e) {
                // Even slightly-off date formats can result in an exception!
                //NSLog(@"Unexpected XMP error%d, %s", e.GetID(), e.GetErrMsg());
                throw XMPOpsException(e.GetID(), e.GetErrMsg() ); //rev: throw??
            }
            *itemp = 0;
            dateP->hasTime = false;
            return false;
        }

        XMP_Int32 XMPMetaOps::compare(const XMP_DateTime&a, const XMP_DateTime& b) { 
            XMP_Int32 i = (XMP_Int32)a.year - (XMP_Int32)b.year;
            if (i != 0) return i;
            i = (XMP_Int32)a.month - (XMP_Int32)b.month;
            if (i != 0) return i;
            i = (XMP_Int32)a.day - (XMP_Int32)b.day;
            if (i != 0) return i;
            i = (XMP_Int32)a.hour - (XMP_Int32)b.hour;
            if (i != 0) return i;
            i = (XMP_Int32)a.minute - (XMP_Int32)b.minute;
            if (i != 0) return i;
            i = (XMP_Int32)a.second - (XMP_Int32)b.second;
            if (i != 0) return i;
            i = (XMP_Int32)a.nanoSecond - (XMP_Int32)b.nanoSecond;
            return i;
        }


            // Test code starts here
        /*
        // ***************************************
        bool XMPCore_GetProperty (std::string    schemaNS,
                                  std::string    propName,
                                  char* propValue,
                                  XMP_OptionBits *    options )
        {
            std::string propV;
            bool ok=meta.GetProperty(schemaNS,propName,&propV,options);
            memcpy(propValue, propV.c_str(), propV.length());
            propValue[propV.length()] = '\0';
            return ok;
        }
        
        void XMPCore_SetProperty (std::string    schemaNS,
                                  std::string    propName,
                                  char*    propValue,
                                  XMP_OptionBits *    options)
        {
            meta.SetProperty(schemaNS,propName,propValue);
        }
        
        const char* XMPCore_SerializeToBuffer ()
        {
            std::string buffer;
            meta.SerializeToBuffer(&buffer);
            auto buffer_mem = (char*) malloc(buffer.length() + 1);
            buffer_mem[buffer.length()] = '\0';
            memcpy(buffer_mem, buffer.c_str(), buffer.length());
            return buffer_mem;
        }
        
        void XMPCore_AppendArrayItem (std::string    schemaNS,
                                           std::string    arrayName,
                                           XMP_OptionBits arrayOptions,
                                           char*    itemValue,
                                           XMP_OptionBits itemOptions)
        {
            meta.AppendArrayItem(schemaNS, arrayName, arrayOptions, itemValue,itemOptions);
        }
        
        void XMPCore_SetArrayItem (std::string    schemaNS,
                                        std::string    arrayName,
                                        XMP_Index itemIndex,
                                        char*    itemValue,
                                        XMP_OptionBits options)
        {
            meta.SetArrayItem(schemaNS, arrayName, itemIndex, itemValue,options);
        }
        
        void XMPCore_SetStructField (std::string    schemaNS,
                                          std::string    structName,
                                          std::string     fieldNS,
                                          std::string     fieldName,
                                          std::string     fieldValue,
                                          XMP_OptionBits options)
        {
            meta.SetStructField(schemaNS, structName, fieldNS, fieldName, fieldValue,options);
        }
        
        bool XMPCore_GetArrayItem (std::string    schemaNS,
                                   std::string    arrayName,
                                   XMP_Index itemIndex,
                                   char* itemValue,
                                   XMP_OptionBits *    options )
        {
            std::string propV;
            bool ok=meta.GetArrayItem(schemaNS, arrayName, itemIndex, &propV, options);
            memcpy(itemValue, propV.c_str(), propV.length());
            itemValue[propV.length()] = '\0';
            return ok;
        }
        
        bool XMPCore_GetStructField (std::string    schemaNS,
                                     std::string    structName,
                                     std::string       fieldNS,
                                     string       fieldName,
                                     char*  fieldValue,
                                     XMP_StringLen *  valueSize,
                                     XMP_OptionBits * options )
        {
            std::string propV;
            bool ok=meta.GetStructField(schemaNS, structName, fieldNS, fieldName, &propV, options);
            memcpy(fieldValue, propV.c_str(), propV.length());
            fieldValue[propV.length()] = '\0';
            return ok;
        }
         */ //Test code ends here

    //};

