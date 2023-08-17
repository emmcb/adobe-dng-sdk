var Module = require('./XMP.js');
var XMPOpsConst = require('./XMPOpsConst.js');

var Initialize = function()
{
    var id = Module._malloc(4);
    var msg = Module._malloc(2048);
    var init = Module.ccall('Initialize', 'boolean', ['number', 'number'], [id, msg]);
    XMPOpsConst.ThrowExceptionFromNativeStatic(id, msg);
    return init;
};


var Terminate = function()
{
    Module.ccall('Terminate', null, null, null);
  
};


 var XMPMetaOps = function() {

    var destroyed = false;
    var size = Module._malloc(4);
    var errorId = Module._malloc(4);
    var errorMessage = Module._malloc(2048);

    var xmpMetaOpsRef = Module.ccall('CreateMetaOps', 'number', ['number', 'number'], [errorId, errorMessage]);
   
    XMPOpsConst.ThrowExceptionFromNative(errorId, errorMessage);

    var GetMetaOpsRef = function () {
        return xmpMetaOpsRef;
    };

    var SetMetadataFromStream = function (xmpStream) {
        if (destroyed) 
            XMPOpsConst.Throw();
        XMPOpsConst.VerifyStringArguments([xmpStream]);
        Module.ccall('SetMetadataFromStream', null, ['number', 'string', 'number', 'number'], [xmpMetaOpsRef, xmpStream, errorId, errorMessage]);
        XMPOpsConst.ThrowExceptionFromNative(errorId, errorMessage);
    };

  
     //null value in String arguments are accepted as AssetOps does so. null checks should be present in the library. If missing in any case, then they should be incorporated in 
     //either this file or in c++ whichever is better.
    var SetCreatorTool = function (creatorTool) {
        if (destroyed) 
            XMPOpsConst.Throw();
        XMPOpsConst.VerifyStringArguments([creatorTool]);
        Module.ccall('SetCreatorTool', null, ['number', 'string', 'number', 'number'], [xmpMetaOpsRef, creatorTool, errorId, errorMessage]);
        XMPOpsConst.ThrowExceptionFromNative(errorId, errorMessage);      
    };

    var SetMetadata = function (other) {
        if (destroyed) 
            XMPOpsConst.Throw();
        XMPOpsConst.VerifyXMPMetaOpsArguments([other])
        Module.ccall('SetMetadata', null, ['number', 'number', 'number', 'number'], [xmpMetaOpsRef, other.GetMetaOpsRef(), errorId, errorMessage]);
        XMPOpsConst.ThrowExceptionFromNative(errorId, errorMessage);
    };

    var ClearUpdatableHistory = function () {
        if (destroyed) 
            XMPOpsConst.Throw();
        Module.ccall('ClearUpdatableHistory', null, ['number', 'number', 'number'], [xmpMetaOpsRef, errorId, errorMessage]);
        XMPOpsConst.ThrowExceptionFromNative(errorId, errorMessage);
    };


    
    var SetProperty = function (schemaNS, propName, propValue) {
        if (destroyed) 
            XMPOpsConst.Throw();
        XMPOpsConst.VerifyStringArguments([schemaNS, propName, propValue]);
        Module.ccall('SetProperty', null, ['number', 'string', 'string', 'string', 'number', 'number'], [xmpMetaOpsRef, schemaNS, propName, propValue, errorId, errorMessage]);
        XMPOpsConst.ThrowExceptionFromNative(errorId, errorMessage);
    };

    // todo
   
    var GetProperty = function (schemaNS, propName ) {
        if (destroyed) 
           XMPOpsConst.Throw();
        XMPOpsConst.VerifyStringArguments([schemaNS, propName]);
        var isPresentBool = Module.ccall('GetProperty', 'boolean', ['number', 'string', 'string', 'number', 'number', 'number', 'number'], [xmpMetaOpsRef, schemaNS, propName, 0, size, errorId, errorMessage]);
        XMPOpsConst.ThrowExceptionFromNative(errorId, errorMessage);
        var sizeVal = Module.getValue(size, 'i32');
        var propValPtr = Module._malloc(sizeVal);
        Module.stringToUTF8("", propValPtr, 1);
        var isPresentBool = Module.ccall('GetProperty', 'boolean', ['number', 'string', 'string', 'number','number', 'number', 'number'], [xmpMetaOpsRef, schemaNS, propName, propValPtr, size, errorId, errorMessage]);
        var propVal = Module.Pointer_stringify(propValPtr);
        Module._free(propValPtr);
        XMPOpsConst.ThrowExceptionFromNative(errorId, errorMessage);
           
        return {
            isPresent: isPresentBool,
            value: propVal
        }
   };

    var DeleteProperty = function (schemaNS, propName) {
        if (destroyed) 
            XMPOpsConst.Throw();
        XMPOpsConst.VerifyStringArguments([schemaNS, propName]);
        Module.ccall('DeleteProperty', null, ['number', 'string', 'string', 'number', 'number'], [xmpMetaOpsRef, schemaNS, propName, errorId, errorMessage]);
        XMPOpsConst.ThrowExceptionFromNative(errorId, errorMessage);
    };

    //createDate and type are optional
    var UpdateMetadata = function (id, modifiedDate, createDate, type) {
        if (destroyed) 
            XMPOpsConst.Throw();
        XMPOpsConst.VerifyStringArguments([id, modifiedDate]);
        var createDateStr = "", typeStr = "";
        if (createDate) {
            XMPOpsConst.VerifyStringArguments([createDate])
            createDateStr = createDate;
        }
        if (type) {
            XMPOpsConst.VerifyStringArguments([type])
            typeStr = type;
        }
        var update = Module.ccall('UpdateMetadata', 'boolean', ['number', 'string', 'string', 'string', 'string', 'number', 'number'], [xmpMetaOpsRef, createDateStr, modifiedDate, id, typeStr, errorId, errorMessage]);
        XMPOpsConst.ThrowExceptionFromNative(errorId, errorMessage);
        return update;
    };
   

    var MakeDerivedWithAction = function (deriveFrom, action, docId) {
        if (destroyed) 
            XMPOpsConst.Throw();
        XMPOpsConst.VerifyXMPMetaOpsArguments([deriveFrom])
        XMPOpsConst.VerifyStringArguments([action]);
        if (!docId)
            docId = "";
        else
            XMPOpsConst.VerifyStringArguments([docId]);
        Module.ccall('MakeDerivedWithAction', null, ['number', 'number', 'string', 'string', 'number', 'number'], [xmpMetaOpsRef, deriveFrom.GetMetaOpsRef(), action, docId, errorId, errorMessage]);
        XMPOpsConst.ThrowExceptionFromNative(errorId, errorMessage);
    };

    
    
    var AppendHistoryEvent = function (actionStr, param, when, updateLast) {
        if (destroyed) 
            XMPOpsConst.Throw();
        XMPOpsConst.VerifyStringArguments([actionStr, param]);
        var whenStr ="", updateLastBool = false;
        if (when) {
            XMPOpsConst.VerifyStringArguments([when]);
            whenStr = when;
        }
        if (updateLast) {
            XMPOpsConst.VerifyBooleanArguments([updateLast])
            updateLastBool = updateLast;
        }
            
        var ins = Module._malloc(37); //size of id
        Module.stringToUTF8("", ins, 1);
        Module.ccall('AppendHistoryEvent', null, ['number', 'string', 'string', 'string', 'boolean', 'number','number', 'number'], [xmpMetaOpsRef, actionStr, param, whenStr, updateLastBool, ins, errorId, errorMessage]);
        instanceId = Module.Pointer_stringify(ins);
        Module._free(ins);
           
        XMPOpsConst.ThrowExceptionFromNative(errorId, errorMessage);
          
        return instanceId;
    };
  

    var AppendSavedEvent = function () {
        if (destroyed) 
            XMPOpsConst.Throw();
        Module.ccall('AppendSavedEvent', null, ['number', 'number', 'number'], [xmpMetaOpsRef, errorId, errorMessage]);
        XMPOpsConst.ThrowExceptionFromNative(errorId, errorMessage);
    };
    
    var AssignNewDocumentId = function () {
        if (destroyed) 
            XMPOpsConst.Throw();
        Module.ccall('AssignNewDocumentId', null, ['number', 'number', 'number'], [xmpMetaOpsRef, errorId, errorMessage]);
        XMPOpsConst.ThrowExceptionFromNative(errorId, errorMessage);
    };

   
    var AddOriginURL = function (originURL, manageUiUrl) {
        if (destroyed) 
            XMPOpsConst.Throw();
        XMPOpsConst.VerifyStringArguments([originURL, manageUiUrl]);
        Module.ccall('AddOriginURL', null, ['number', 'string', 'string', 'number', 'number'], [xmpMetaOpsRef, originURL, manageUiUrl, errorId, errorMessage]);
        XMPOpsConst.ThrowExceptionFromNative(errorId, errorMessage);
    };

    var AddLicenseInfo = function (licenseURL, licenseName, attributionURL, attributionName) {
        if (destroyed) 
            XMPOpsConst.Throw();
        XMPOpsConst.VerifyStringArguments([licenseURL, licenseName, attributionURL, attributionName]);
        Module.ccall('AddLicenseInfo', null, ['number', 'string', 'string', 'string', 'string', 'number', 'number'], [xmpMetaOpsRef, licenseURL, licenseName, attributionURL, attributionName, errorId, errorMessage]);
        XMPOpsConst.ThrowExceptionFromNative(errorId, errorMessage);
    };

    var SerializeToBuffer = function (options) {
        if (destroyed) 
            XMPOpsConst.Throw();
        optionsVal = 0
        if (options) {
            XMPOpsConst.VerifyNumberArguments([options])
            optionsVal = options
        }
        var size = Module.ccall('SerializeToBuffer', 'number', ['number', 'number', 'number', 'number', 'number'], [xmpMetaOpsRef, optionsVal, 0, errorId, errorMessage]);
        XMPOpsConst.ThrowExceptionFromNative(errorId, errorMessage);
        var propVal = Module._malloc(size);
        Module.stringToUTF8("", propVal,1);
        size = Module.ccall('SerializeToBuffer', 'number', ['number', 'number', 'number', 'number', 'number'], [xmpMetaOpsRef, options, propVal, errorId, errorMessage]);
        var buffer = Module.Pointer_stringify(propVal)
        Module._free(propVal);
        XMPOpsConst.ThrowExceptionFromNative(errorId, errorMessage);
        return buffer;
    };

    var RemovePublishingInfo = function () {
        if (destroyed) 
            XMPOpsConst.Throw();
        Module.ccall('RemovePublishingInfo', null, ['number', 'number', 'number'], [xmpMetaOpsRef, errorId, errorMessage]);
        XMPOpsConst.ThrowExceptionFromNative(errorId, errorMessage);
    };

    
    var MergeMetadataFrom = function (from, base) {
        if (destroyed) 
            XMPOpsConst.Throw();

        if (from === undefined || from == 0)
            from = null;
        if (base === undefined || base == 0)
            base = null;
            
        if (String(from) == "" || (from && !from.GetMetaOpsRef)) {
            throw XMPOpsConst.XMPOpsException(XMPOpsConst.XMPOpsErrorId.kXMPOpsErr_BadParam, "Parameters must be of type XMPMetaOps");
        }
        if (String(base) == "" || (base && !base.GetMetaOpsRef)) {
            throw XMPOpsConst.XMPOpsException(XMPOpsConst.XMPOpsErrorId.kXMPOpsErr_BadParam, "Parameters must be of type XMPMetaOps");
        }

        var changed = Module.ccall('MergeMetadataFrom', 'boolean', ['number', 'number', 'number', 'number', 'number'], [xmpMetaOpsRef, from? from.GetMetaOpsRef():0, base? base.GetMetaOpsRef():0, errorId, errorMessage]);
        XMPOpsConst.ThrowExceptionFromNative(errorId, errorMessage);
        return changed;
    };
    
   
    var SetLocalizedTextWithPath = function (namespace, altTextName, genericLang, specificLang, itemValue) {
        if (destroyed) 
            XMPOpsConst.Throw();
        XMPOpsConst.VerifyStringArguments([namespace, altTextName, genericLang, specificLang, itemValue]);
        Module.ccall('SetLocalizedTextWithPath', null, ['number', 'string', 'string', 'string', 'string', 'string', 'number', 'number'], [xmpMetaOpsRef, namespace, altTextName, genericLang, specificLang, itemValue, errorId, errorMessage]);
        XMPOpsConst.ThrowExceptionFromNative(errorId, errorMessage);
    };

    var DestroyMetaOps = function () {
        if (destroyed) 
            XMPOpsConst.Throw();
        Module.ccall('DestroyMetaOps', null, ['number'], [xmpMetaOpsRef]);
        Module._free(size);
        Module._free(errorId);
        Module._free(errorMessage);
        xmpMetaOpsRef = 0;
        destroyed = true;
    };


    return {
        SetMetadataFromStream: SetMetadataFromStream,
        SetCreatorTool: SetCreatorTool,
        SetMetadata: SetMetadata,
        ClearUpdatableHistory: ClearUpdatableHistory,
        SetProperty: SetProperty,
        GetProperty: GetProperty,
        DeleteProperty: DeleteProperty,
        UpdateMetadata: UpdateMetadata,
        MakeDerivedWithAction: MakeDerivedWithAction,
        AppendHistoryEvent: AppendHistoryEvent,
        AppendSavedEvent: AppendSavedEvent,
        AssignNewDocumentId: AssignNewDocumentId,
        AddLicenseInfo: AddLicenseInfo,
        AddOriginURL: AddOriginURL,
        SerializeToBuffer: SerializeToBuffer,
        RemovePublishingInfo: RemovePublishingInfo,
        MergeMetadataFrom: MergeMetadataFrom,
        SetLocalizedTextWithPath: SetLocalizedTextWithPath,
        GetMetaOpsRef: GetMetaOpsRef,
        DestroyMetaOps: DestroyMetaOps
    };
}

module.exports = {
    Initialize: Initialize,
    Terminate: Terminate,
    XMPMetaOps: XMPMetaOps
}
