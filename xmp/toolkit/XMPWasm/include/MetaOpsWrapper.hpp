#ifndef MetaOpsWrapper_h
#define MetaOpsWrapper_h


#endif /* MetaOpsWrapper_h */

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
/// \file MetaOpsWrapper.hpp
/// \brief Wrapper APIs to be accessed from the JS module
///
/// \c MetaOpsWrapper contains methods that are be to used by DCX team for implementing their JS workflows.
///
// =================================================================================================


#include "MetaOps.hpp"

#if __cplusplus
extern "C"{
#endif
    
    /// Initializes the XMP Toolkit.
    ///
    /// Call this function before making any other calls to the \c XMPMetaOps functions.
    ///
    /// @return True on success. */
    
    bool Initialize( XMP_Int32* id, char* msg);
    
    // ---------------------------------------------------------------------------------------------
    /// @brief \c Terminate() explicitly terminates usage of the XMP Toolkit.
    ///
    /// Frees structures created on initialization.
    ///
    
    void Terminate();
    
    /// Creates a new metaops object
    ///
    /// Call this function after calling \c Initialize function.
    ///
    /// @return metaops pointer */
    
    XMPMetaOps* CreateMetaOps( XMP_Int32* id, char* msg);
    
    /// Creates a new metaops object from an existing metaops pointer
    ///
    /// Call this function after calling \c Initialize function.
    ///
    /// @return metaops pointer. */
    
    XMPMetaOps* CreateMetaOpsFrom(XMPMetaOps* metaops, XMP_Int32* id, char* msg);
    
    /// Sets the meta object from a character stream.
    ///
    /// Call this function after calling \c Initialize function.
    ///
    /// Takes the metaops pointer and character stream as input. */
    
    void SetMetadataFromStream(XMPMetaOps *metaops, XMP_StringPtr xmpString, XMP_Int32* id, char* msg);
    
    /// Sets the XMP property CreatorTool on the XMPMetaOps object, not in XMP metadata.
    ///
    /// Call this function when you need to set custom CreatorTool.
    ///
    
    void SetCreatorTool ( XMPMetaOps *metaops,XMP_StringPtr creatorTool, XMP_Int32* id, char* msg );
    
    /// Returns the XMP Metadata object.
    ///
    /// Call this function when you need metadata object.
    ///
    ///
    /// @return SXMPMeta object.
    
    /*SXMPMeta GetMetadata(XMPMetaOps *metaops);*/
    
    /// Sets the XMP Metadata from the object passed.
    ///
    /// Call this function when you need to set metadata from some other object.
    ///
    
    void SetMetadata(XMPMetaOps *metaops,XMPMetaOps* other, XMP_Int32* id, char* msg);
    
    /// Clear the lastupdatehistory attribute of the XMPMetaOps object
    
    void ClearUpdatableHistory(XMPMetaOps *metaops, XMP_Int32* id, char* msg);
    
    // ---------------------------------------------------------------------------------------------
    /// @brief \c Clone() creates a deep copy of an XMPMetaOps object.
    ///
    /// Use this function to copy an entire XMP metadata tree. This function returns a pointer to the
    /// XMPMetaOps object.
    ///
    /// @return An XMPMetaOps object pointer cloned from the original.
    
    XMPMetaOps* Clone(XMPMetaOps *metaops, XMP_Int32* id, char* msg);
    
    /*
     * Sets the value of simple value property on metaops object.
     */
    
    void SetProperty ( XMPMetaOps *metaops,XMP_StringPtr schemaNS,
                      XMP_StringPtr    propName,
                      XMP_StringPtr    propValue,
                      XMP_Int32* id, char* msg );
    
    /*
     * Get the value of simple value property in String from metaops object in "propValue" parameter
     */
    
    bool GetProperty (XMPMetaOps *metaops,XMP_StringPtr    schemaNS,
                      XMP_StringPtr    propName,
                      char* propVal, XMP_Int32* size, XMP_Int32* id, char* msg);
    
    /*
     * Deletes the simple value property from metaops object.
     */
    
    void DeleteProperty ( XMPMetaOps *metaops,XMP_StringPtr    schemaNS,
                         XMP_StringPtr  propName, XMP_Int32* id, char* msg);
    
    /*
     * Sets CreateDate property with createDate and ModifyDate, MetadataDate with modifiedDate.
     * createDate and modifiedDate should be valid ISO 8601 strings else an AssetOpsException is thrown.
     *  Also sets the OriginalDocumentID, DocumentID and InstanceId with the Id provided
     *  dc:format with type provided.
     * The history entry is added with the action as "created", "instanceID" as Id, "when" as modifiedDate and softwareAgent is
     * set to the value set by setCreatorTool(String toolName).
     * All the update operations are performed on metaops object
     */
    
    bool UpdateMetadata(XMPMetaOps *metaops, XMP_StringPtr createdStr, XMP_StringPtr modifiedStr, XMP_StringPtr metaId, XMP_StringPtr type, XMP_Int32* id, char* msg);
    
    // bool UpdateMetadataWithoutType(XMPMetaOps *metaops, XMP_StringPtr dateStr, XMP_StringPtr elementId, XMP_Int32* id, char* msg);
    
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
     * All the update operations are performed on metaops object
     */
    
    void MakeDerivedWithAction(XMPMetaOps *metaops,XMPMetaOps* deriveFrom, XMP_StringPtr action, XMP_StringPtr docId, XMP_Int32* id, char* msg);
    
    /*Sets a new xmpMM:InstanceID for the doc and returns it.
     * xmp:ModifyDate and xmp:MetadataDate is set to the current date-time.
     * Sets the properties in a History Entry with the given parameters.
     * Appends a history entry/ updates last history entry with action provided, "when" as when provided, instanceID as the new generated instance ID and
     * softwareAgent to the value set by setCreatorTool for this meta reference.
     * updateLast decides whether to update the last history entry with new values or
     * to append a new entry
     * param is dcx specific , can be null for other purposes
     * All the update operations are performed on metaops object
     */
    
    void AppendHistoryEvent(XMPMetaOps *metaops,XMP_StringPtr actionStr, XMP_StringPtr param, XMP_StringPtr when, bool updateLast,char* instanceId, XMP_Int32* id, char* msg);
    
    // XMP_StringPtr AppendHistoryEventWithDateOnly(XMPMetaOps *metaops,XMP_StringPtr actionStr, XMP_StringPtr param, XMP_StringPtr when, XMP_Int32* id, char* msg);
    
    // XMP_StringPtr AppendHistoryEventWithoutDate(XMPMetaOps *metaops,XMP_StringPtr actionStr, XMP_StringPtr param, XMP_Int32* id, char* msg);
    
    /*Sets a new xmpMM:InstanceID for the doc.
     * xmp:ModifyDate and xmp:MetadataDate is set to the current date-time.
     * Appends a history entry with action as "saved" and "when" as current date-time, instanceID as the new generated instance ID and softwareAgent to the value set by setCreatorTool for this meta reference.
     * All the update operations are performed on metaops object
     */
    
    void AppendSavedEvent(XMPMetaOps *metaops, XMP_Int32* id, char* msg);
    
    /*
     * Assigns a new documentId and InstanceID to the asset. Both values are same.
     * If the instanceID of the last history entry is null or has the same value as the
     * original InstanceID of the asset, then the InstanceID and Document ID of the
     * last history entry are changed to the new generated Id.
     * All the update operations are performed on metaops object
     */
    
    void AssignNewDocumentId(XMPMetaOps *metaops, XMP_Int32* id, char* msg);
    
    /*
     * Sets the value of xmpMM:ManageTo and xmpMM:ManageUI
     * All the update operations are performed on metaops object
     */
    
    void AddOriginURL(XMPMetaOps *metaops,XMP_StringPtr originURL, XMP_StringPtr manageUiUrl, XMP_Int32* id, char* msg);
    
    /*
     * Add licensing information
     * All the update operations are performed on metaops object
     */
    
    void AddLicenseInfo (XMPMetaOps *metaops,XMP_StringPtr licenseURL,XMP_StringPtr licenseName, XMP_StringPtr attributionURL, XMP_StringPtr attributionName, XMP_Int32* id, char* msg);
    
    void DestroyMetaOps(XMPMetaOps* metaops);
    
    XMP_Int32 SerializeToBuffer (XMPMetaOps *metaops,XMP_OptionBits options,char* buffstr, XMP_Int32* id, char* msg);
    
    void RemovePublishingInfo(XMPMetaOps* metaops, XMP_Int32* id, char* msg);
    
    bool HasTag(XMPMetaOps* metaops, XMP_StringPtr tag, XMP_Int32* id, char* msg);
    
    void AddTags(XMPMetaOps* metaops, std::vector<std::string> tags, XMP_Int32* id, char* msg);
    
    void AddTag(XMPMetaOps* metaops, XMP_StringPtr tag, XMP_Int32* id, char* msg);
    
    bool MergeMetadataFrom(XMPMetaOps* metaops, XMPMetaOps* from, XMPMetaOps* base, XMP_Int32* id, char* msg);
    
    void SetLocalizedTextWithPath(XMPMetaOps* metaops, XMP_StringPtr ns, XMP_StringPtr altTextName, XMP_StringPtr genericLang, XMP_StringPtr specificLang, XMP_StringPtr itemValue, XMP_Int32* id, char* msg);
    
#if __cplusplus
}
#endif


