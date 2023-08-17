var Module = require('./XMP.js');
var XMPOpsConst = require('./XMPOpsConst.js');

var InitializeXMPFiles = function () {
    var id = Module._malloc(4);
    var msg = Module._malloc(2048);
    var init = Module.ccall('InitializeXMPFile', 'boolean', ['number', 'number'], [id, msg]);
    XMPOpsConst.ThrowExceptionFromNativeStatic(id, msg);
    return init;
};


//todo, check for exception (ever be thrown)
var TerminateXMPFiles = function () {
    var id = Module._malloc(4);
    var msg = Module._malloc(2048);
    Module.ccall('TerminateXMPFile', null, ['number', 'number'], [id, msg]);
    XMPOpsConst.ThrowExceptionFromNativeStatic(id, msg);
};


var XMPFileOps = function () {
    var destroyed = false;
    var errorId = Module._malloc(4);
    var errorMessage = Module._malloc(2048);

    var xmpFileOpsRef = Module.ccall('CreateFileOps', 'number', ['number', 'number'], [errorId, errorMessage]);

    XMPOpsConst.ThrowExceptionFromNative(errorId, errorMessage);

    var GetFileOpsRef = function () {
        return xmpFileOpsRef;
    };
    

    var OpenFile = function (fileStream, length, format, openFlags) {
        if (destroyed) 
            XMPOpsConst.Throw();
        XMPOpsConst.VerifyStringArguments([fileStream]);
        XMPOpsConst.VerifyNumberArguments([length])
        formatVal = XMPOpsConst.Format.kXMP_UnknownFile
        openFlagsval = 0
        if (format) {
            XMPOpsConst.VerifyNumberArguments([format])
            optionsVal = format
        }
        if (openFlags) {
            XMPOpsConst.VerifyNumberArguments([openFlags])
            optionsVal = openFlags
        }
        var opened = Module.ccall('OpenFileStream', 'boolean', ['number', 'string', 'number', 'number', 'number', 'number', 'number'], [xmpFileOpsRef, fileStream, length, format, openFlags, errorId, errorMessage]);
        XMPOpsConst.ThrowExceptionFromNative(errorId, errorMessage);
        return opened;
        
    };


    var GetXMP = function (metaOps) {
        if (destroyed) 
            XMPOpsConst.Throw();
        XMPOpsConst.VerifyXMPMetaOpsArguments([metaOps])
        var xmpPresent = Module.ccall('GetXMP', 'boolean', ['number', 'number', 'number', 'number'], [xmpFileOpsRef, metaOps.GetMetaOpsRef(), errorId, errorMessage]);
        XMPOpsConst.ThrowExceptionFromNative(errorId, errorMessage);
        return xmpPresent;
        
    };

    var PutXMP = function (metaOps) {
        if (destroyed) 
            XMPOpsConst.Throw();
        XMPOpsConst.VerifyXMPMetaOpsArguments([metaOps])
        Module.ccall('PutXMP', null, ['number', 'number', 'number', 'number'], [xmpFileOpsRef, metaOps.GetMetaOpsRef(), errorId, errorMessage]);
        XMPOpsConst.ThrowExceptionFromNative(errorId, errorMessage);
    };

    var CloseFile = function () {
        if (destroyed) 
            XMPOpsConst.Throw();
        Module.ccall('CloseFile', null, ['number', 'number', 'number'], [xmpFileOpsRef, errorId, errorMessage]);
        XMPOpsConst.ThrowExceptionFromNative(errorId, errorMessage);  
    };

    var GetStream = function () {
        if (destroyed)
            XMPOpsConst.Throw();  
        var size = Module.ccall('GetStreamSize', 'number', ['number'], [xmpFileOpsRef]);
        var output = Module._malloc(size);
        Module.stringToUTF8("", output, 1);
        size = Module.ccall('GetStream', null, ['number', 'number', 'number', 'number'], [xmpFileOpsRef, output, errorId, errorMessage]);
        var buffer = Module.Pointer_stringify(output)
        Module._free(output);
        XMPOpsConst.ThrowExceptionFromNative(errorId, errorMessage);
        return buffer;
    };
    
    var DestroyFileOps = function () {
        if (destroyed) 
            XMPOpsConst.Throw();
        Module.ccall('DestroyXMPFiles', null, ['number'], [xmpFileOpsRef]);
        Module._free(errorId);
        Module._free(errorMessage);
        xmpFileOpsRef = 0;
        destroyed = true;
    };


    return {
        OpenFile: OpenFile,
        GetXMP: GetXMP,
        PutXMP: PutXMP,
        CloseFile: CloseFile,
        GetFileOpsRef: GetFileOpsRef,
        DestroyFileOps: DestroyFileOps
    };
}

module.exports = {
    InitializeXMPFiles: InitializeXMPFiles,
    TerminateXMPFiles: TerminateXMPFiles,
    XMPFileOps: XMPFileOps
}
