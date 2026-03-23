#ifndef __MetaOps_hpp__
#define __MetaOps_hpp__    1


#endif /* MetaOps_hpp */

// =================================================================================================
// Copyright Adobe
// Copyright 2002 Adobe
// All Rights Reserved
//
// NOTICE:  Adobe permits you to use, modify, and distribute this file in accordance with the terms
// of the Adobe license agreement accompanying it. If you have received this file from a source other 
// than Adobe, then your use, modification, or distribution of it requires the prior written permission
// of Adobe.
// =================================================================================================

// =================================================================================================
/// \file MetaOps.hpp
/// \brief API for access to core XMP functionality using a wrapper class
///
/// \c MetaOps contains methods that are be to used by DCX team for implementing their JS workflows.
/// The methods exposed in this class are intended to be used by MetaOpsWrapper file, which contains the methods that are to be exposed through the WebAssembly to the DCX JS module.

// =================================================================================================

#include <cstdio>
#include <vector>
#include <string>
#include <cstring>
//#define ENABLE_XMP_CPP_INTERFACE 1

// Must be defined to instantiate template classes
#define TXMP_STRING_TYPE std::string

// Must be defined to give access to XMPFiles
#define XMP_INCLUDE_XMPFILES 1

// Ensure XMP templates are instantiated
#include "public/include/XMP.incl_cpp"

// Provide access to the API
#include "public/include/XMP.hpp"
#include "XMPOpsException.hpp"

#include <iostream>
#include <fstream>

class XMPMetaOps{
    
private:
    SXMPMeta meta;
    std::string creatorTool;
    std::string updatableHistoryId;
    std::string historyLast;
    
    void CheckForHistoryId();
    
    static bool getNextHistoryRecord(const SXMPMeta* xmp, XMP_Index *itemp, std::string *instanceIdp, XMP_DateTime *dateP);
    
    static XMP_Int32 compare(const XMP_DateTime&a, const XMP_DateTime& b);
    
    static void InsertHistoryItem(SXMPMeta* toXMP, XMP_Index toIndex, const SXMPMeta* fromXMP, XMP_Index fromIndex);
    
public:
    
    // ---------------------------------------------------------------------------------------------
    /// Empty constructor
    ///
    XMPMetaOps();
    
    // ---------------------------------------------------------------------------------------------
    /// @brief Reconstructs an XMPMetaOps object from an internal reference.
    ///
    /// This constructor creates a new \c XMPMetaOps object that refers to the underlying reference object
    /// of an existing \c SXMPMeta object.
    ///
    /// @param meta The underlying reference object.
    ///
    /// @return The new object.
    
    XMPMetaOps(XMPMetaOps* metaops);
    
    // ---------------------------------------------------------------------------------------------
    /// Destructor
    ///
    ~XMPMetaOps();
    
    /// Sets the meta object from a character stream.
    ///
    /// Call this function after calling \c Initialize function.
    ///
    /// This function is static; make the call directly from the class (\c XMPMetaOps).
    ///
    /// @return The new object. */
    
    void SetMetadataFromStream ( std::string xmpString );
    
	/// Sets the meta object from another SXMPMeta

	///WASM clients do not need this.

	void SetMetadataFromMeta(SXMPMeta meta);

    /// Returns the XMP Metadata object.
    ///
    /// Call this function when you need metadata object.
    ///
    ///
    /// @return SXMPMeta object.
    
    SXMPMeta GetMetadata();
    
    // ---------------------------------------------------------------------------------------------
    /// @brief \c Clone() creates a deep copy of an XMPMetaOps object.
    ///
    /// Use this function to copy an entire XMP metadata tree. This function returns a pointer to the
    /// XMPMetaOps object.
    ///
    /// @return An XMP object cloned from the original.
    
    XMPMetaOps* Clone();
    
    /// Sets the XMP Metadata from the object passed.
    ///
    /// Call this function when you need to set metadata from some other object.
    ///
    
    void SetMetadata(XMPMetaOps* other);
    
    /// Sets the XMP property CreatorTool on the XMPMetaOps object, not in XMP metadata.
    ///
    /// Call this function when you need to set custom CreatorTool.
    ///
    
    void SetCreatorTool (std::string creator);
    
    /*
     * Sets CreateDate property with createDate and ModifyDate, MetadataDate with modifiedDate.
     * createDate and modifiedDate should be valid ISO 8601 strings else an AssetOpsException is thrown.
     *  Also sets the OriginalDocumentID, DocumentID and InstanceId with the Id provided
     *  dc:format with type provided.
     * The history entry is added with the action as "created", "instanceID" as Id, "when" as modifiedDate and softwareAgent is
     * set to the value set by setCreatorTool(String toolName).
     */

    bool UpdateMetadata(std::string createdStr, std::string modifiedStr, std::string id, std::string type);
    
    /*
     * The  ModifyDate, MetadataDate, CreateDate is set to date. OriginalDocumentID, DocumentID
     * date should be a valid ISO 8601 std::string else an AssetOpsException is thrown.
     * and InstanceId are set to Id.
     * CreatorTool is set to the value set by setCreatorTool(String toolName).
     * The history entry is added with the action as "created", "instanceID" as Id, "when" as date
     * and softwareAgent is set to the value set by setCreatorTool(String toolName).
     */
    
    //bool UpdateMetadata(std::string dateStr, std::string elementId);
    
    /*
     * The asset is derived from deriveFrom meta. deriveFrom would be the parent asset and the meta of this class would be the derived asset.
     * The meta of derived asset is cloned from parent asset.
     * The fields in DerivedFrom are set accordingly as:
     * stRef:documentID is set to the DocumentID of the deriveFrom asset, if they are not null or empty else field is not added.
     * stRef:instanceID is set to the InstanceID of the deriveFrom asset if they are not null or empty else field is not added.
     * stRef:lastModifyDate is set to ModifyDate of the deriveFrom asset if they are not null or empty else field is not added.
     * stRef:originalDocumentID is set to the OriginalDocumentID of the deriveFrom asset. If it is null or empty, it is set to the
     * DocumentID of the deriveFrom asset (unless it is also null or empty. If it is null or empty, the field is not added.)
     * The docId provided is set as the new DocumentId. If null is passed as docID, a new Id is generated.
     * xmp:CreatorTool is set to the value set by setCreatorTool().
     * Note: setCreatorTool should be called again for the derived MetadataOps object.
     * History Entry is made with the provided action using the API appendHistoryEvent(String actionStr, String param, String when)
     */
    
    void MakeDerivedWithAction(XMPMetaOps* deriveFrom, std::string action, std::string docId);
    
    /*Sets a new xmpMM:InstanceID for the doc and returns it.
     * xmp:ModifyDate and xmp:MetadataDate is set to the current date-time.
     * Sets the properties in a History Entry with the given parameters.
     * Appends a history entry/ updates last history entry with action provided, "when" as when provided, instanceID as the new generated instance ID and
     * softwareAgent to the value set by setCreatorTool for this meta reference.
     * updateLast decides whether to update the last history entry with new values or
     * to append a new entry
     * param is dcx specific , can be null for other purposes
     */
    
    std::string AppendHistoryEvent(std::string actionStr, std::string param, std::string when, bool updateLast);
    
    /*Sets a new xmpMM:InstanceID for the doc and returns it.
     * xmp:ModifyDate and xmp:MetadataDate is set to the current date-time.
     * Sets the properties in a History Entry with the given parameters.
     * Appends a history entry with action provided, "when" as when provided, instanceID as the new generated instance ID and
     * softwareAgent to the value set by setCreatorTool for this meta reference.
     * param is dcx specific , can be null for other purposes
     */
    
    //std::string AppendHistoryEvent(std::string actionStr, std::string param, std::string when);
    
    /*
     * Sets a new xmpMM:InstanceID for the doc.
     * xmp:ModifyDate and xmp:MetadataDate is set to the current date-time.
     * Sets the properties in a History Entry with the given parameters.
     * Appends a history entry with action provided, "when" as current-date-time, instanceID as the new generated instance ID and
     * softwareAgent to the value set by setCreatorTool for this meta reference.
     * param is dcx specific , can be null for other purposes
     */
    
    //std::string AppendHistoryEvent(std::string actionStr, std::string param);
    
    /*Sets a new xmpMM:InstanceID for the doc.
     * xmp:ModifyDate and xmp:MetadataDate is set to the current date-time.
     * Appends a history entry with action as "saved" and "when" as current date-time, instanceID as the new generated instance ID and softwareAgent to the value set by setCreatorTool for this meta reference.
     */
    
    void AppendSavedEvent();
    
    /*
     * Assigns a new documentId and InstanceID to the asset. Both values are same.
     * If the instanceID of the last history entry is null or has the same value as the
     * original InstanceID of the asset, then the InstanceID and Document ID of the
     * last history entry are changed to the new generated Id.
     */
    
    void AssignNewDocumentId();
    
    /*
     * Sets the value of xmpMM:ManageTo and xmpMM:ManageUI
     */
    
    void AddOriginURL(std::string originURL, std::string manageUiUrl);
    
    /*
     * Add licensing information
     */
    
    void AddLicenseInfo (std::string licenseURL,std::string licenseName, std::string attributionURL, std::string attributionName);
    
    /*
     * Remove publishing info (licensing and origin URLs).
     */
    
    void RemovePublishingInfo();
    
    /*
     * Add a tag to dc:subject if not already present
     */
    
    void AddTag(std::string tag);
    
    /*
     * Add tags to dc:subject if not present
     */
    
    void AddTags(std::vector<std::string> tags);
    
    /*
     * Checks dc:subject if tag is present or not
     */
    
    bool HasTag(std::string tag);
   
    
    /*
     * Merges the metadata packet of this object and fromOp. It uses baseOp as the base packet for these two packets. baseOp can be null.
     * fromOp should not be null. If null, AssetOpsException is thrown.
     * The following are the cases covered by this API (excluding history):
     *
     * 0->Property not present/deleted
     * 1->Property is present/added
     * Vi->value
     * packet1->meta of this class
     * packet2->meta of fromMetaOp
     *
     * Case1: Base =null
     *
     * packet1      packet2          Action/Merged output
     *  1, v1        1, v2             1, v2
     *  1, v1        0                 1, v1
     *  0            1, v2             0
     *
     * Case2: Base != null
     *
     * Packet1      packet2     base                Action/MergedOutput
     *  1, v1        0           1, v3        If property is simple property or qualifier and v1==v3, then  0
     *                                           If property is simple property or qualifier and v1!=v3, then 1,v1
     *                                        If property is composite node, then 0
     *  0            1, v2       1, v3        0
     *
     * 1, v1         0           0            1, v1
     *
     * 0             1, v2       0            1, v2
     *
     * Merging of History only takes into account packet1 and packet2. The array entries are merged in a sorted order.
     */
    
    bool MergeMetadataFrom(XMPMetaOps* from, XMPMetaOps* base);
    
    /*
     * Get the value of simple value property in String
     */
    
    bool GetProperty( std::string ns, std::string name, std::string* propVal);
    
    /*
     * Sets the value of simple value property.
     */
    
    void SetProperty(std::string ns, std::string name, std::string value);
    
    /*
     * Deletes the simple value property.
     */
    
    void DeleteProperty(std::string ns, std::string name);
    
    /*
     *  Modifies the value of a selected item in an alt-text array. Achieves the functionality of setLocalizedText API of xmpcore.
     */
    
    void SetLocalizedTextWithPath(std::string ns, std::string altTextName, std::string genericLang, std::string specificLang, std::string itemValue);
    
    std::string SerializeToBuffer(XMP_OptionBits options);
    /*
     * Internal methods - not to be directly used
     */
    void ClearUpdatableHistory();
    
  
};

