var Module = require('./XMP.js');
var NamespacesObj = {
 kXMP_NS_XMP              : "http://ns.adobe.com/xap/1.0/",

 kXMP_NS_XMP_Rights       : "http://ns.adobe.com/xap/1.0/rights/",
 kXMP_NS_XMP_MM           : "http://ns.adobe.com/xap/1.0/mm/",
 kXMP_NS_XMP_BJ           : "http://ns.adobe.com/xap/1.0/bj/",

 kXMP_NS_PDF              : "http://ns.adobe.com/pdf/1.3/",
 kXMP_NS_Photoshop        : "http://ns.adobe.com/photoshop/1.0/",
 kXMP_NS_PSAlbum          : "http://ns.adobe.com/album/1.0/",
 kXMP_NS_EXIF             : "http://ns.adobe.com/exif/1.0/",
 kXMP_NS_EXIF_Aux         : "http://ns.adobe.com/exif/1.0/aux/",
 kXMP_NS_TIFF             :  "http://ns.adobe.com/tiff/1.0/",
 kXMP_NS_PNG              :  "http://ns.adobe.com/png/1.0/",
 kXMP_NS_SWF              :  "http://ns.adobe.com/swf/1.0/",
 kXMP_NS_JPEG             :  "http://ns.adobe.com/jpeg/1.0/",
 kXMP_NS_JP2K             :  "http://ns.adobe.com/jp2k/1.0/",
 kXMP_NS_CameraRaw        : "http://ns.adobe.com/camera-raw-settings/1.0/",
 kXMP_NS_DM               :   "http://ns.adobe.com/xmp/1.0/DynamicMedia/",
 kXMP_NS_Script           :   "http://ns.adobe.com/xmp/1.0/Script/",
 kXMP_NS_ASF     		   :   "http://ns.adobe.com/asf/1.0/",
 kXMP_NS_WAV     		   :  "http://ns.adobe.com/xmp/wav/1.0/",
 kXMP_NS_BWF     		   :  "http://ns.adobe.com/bwf/bext/1.0/",
 kXMP_NS_AEScart  		   :  "http://ns.adobe.com/aes/cart/",
 kXMP_NS_RIFFINFO		   :  "http://ns.adobe.com/riff/info/",
 kXMP_NS_iXML    		   :  "http://ns.adobe.com/ixml/1.0/",
 kXMP_NS_XMP_Note         : "http://ns.adobe.com/xmp/note/",

 kXMP_NS_AdobeStockPhoto  : "http://ns.adobe.com/StockPhoto/1.0/",
 kXMP_NS_CreatorAtom      : "http://ns.adobe.com/creatorAtom/1.0/",

 kXMP_NS_ExifEX	       :	"http://cipa.jp/exif/1.0/",


 kXMP_NS_Transient : "http://ns.adobe.com/xmp/transient/1.0/",


 kXMP_NS_XMP_IdentifierQual : "http://ns.adobe.com/xmp/Identifier/qual/1.0/",
 kXMP_NS_XMP_Dimensions   :  "http://ns.adobe.com/xap/1.0/sType/Dimensions#",
 kXMP_NS_XMP_Text       :    "http://ns.adobe.com/xap/1.0/t/",
 kXMP_NS_XMP_PagedFile   :   "http://ns.adobe.com/xap/1.0/t/pg/",
 kXMP_NS_XMP_Graphics    :   "http://ns.adobe.com/xap/1.0/g/",
 kXMP_NS_XMP_Image      :    "http://ns.adobe.com/xap/1.0/g/img/",
 kXMP_NS_XMP_Font       :    "http://ns.adobe.com/xap/1.0/sType/Font#",
 kXMP_NS_XMP_ResourceEvent :  "http://ns.adobe.com/xap/1.0/sType/ResourceEvent#",
 kXMP_NS_XMP_ResourceRef :   "http://ns.adobe.com/xap/1.0/sType/ResourceRef#",
 kXMP_NS_XMP_ST_Version  :   "http://ns.adobe.com/xap/1.0/sType/Version#",
 kXMP_NS_XMP_ST_Job     :    "http://ns.adobe.com/xap/1.0/sType/Job#",
 kXMP_NS_XMP_ManifestItem  : "http://ns.adobe.com/xap/1.0/sType/ManifestItem#",

// Deprecated XML namespace ants
 kXMP_NS_XMP_T   :  "http://ns.adobe.com/xap/1.0/t/",
 kXMP_NS_XMP_T_PG : "http://ns.adobe.com/xap/1.0/t/pg/",
 kXMP_NS_XMP_G_IMG : "http://ns.adobe.com/xap/1.0/g/img/",


 kXMP_NS_DC        :     "http://purl.org/dc/elements/1.1/",

 kXMP_NS_IPTCCore   :    "http://iptc.org/std/Iptc4xmpCore/1.0/xmlns/",
 kXMP_NS_IPTCExt    :    "http://iptc.org/std/Iptc4xmpExt/2008-02-29/",

 kXMP_NS_DICOM      :    "http://ns.adobe.com/DICOM/",

 kXMP_NS_PLUS       :    "http://ns.useplus.org/ldf/xmp/1.0/",

 kXMP_NS_PDFA_Schema  :  "http://www.aiim.org/pdfa/ns/schema#",
 kXMP_NS_PDFA_Property : "http://www.aiim.org/pdfa/ns/property#",
 kXMP_NS_PDFA_Type   :   "http://www.aiim.org/pdfa/ns/type#",
 kXMP_NS_PDFA_Field  :   "http://www.aiim.org/pdfa/ns/field#",
 kXMP_NS_PDFA_ID      :  "http://www.aiim.org/pdfa/ns/id/",
 kXMP_NS_PDFA_Extension : "http://www.aiim.org/pdfa/ns/extension/",

 kXMP_NS_PDFX       :    "http://ns.adobe.com/pdfx/1.3/",
 kXMP_NS_PDFX_ID   :     "http://www.npes.org/pdfx/ns/id/",

 kXMP_NS_RDF         :   "http://www.w3.org/1999/02/22-rdf-syntax-ns#",
 kXMP_NS_XML : "http://www.w3.org/XML/1998/namespace"
};

var SerializeOptionsObj = {

// *** Option to remove empty struct/array, or leaf with empty value?

/// Omit the XML packet wrapper.
    kXMP_OmitPacketWrapper   : 0x0010,

	/// Defat is a writeable packet.
    kXMP_ReadOnlyPacket      : 0x0020,

	/// Use a compact form of RDF.
    kXMP_UseCompactFormat    : 0x0040,

	/// Use a canonical form of RDF.
    kXMP_UseCanonicalFormat    : 0x0080,

	/// Include a padding allowance for a thumbnail image.
    kXMP_IncludeThumbnailPad : 0x0100,

	/// The padding parameter is the overall packet length.
    kXMP_ExactPacketLength   : 0x0200,

	/// Omit all formatting whitespace.
    kXMP_OmitAllFormatting   : 0x0800,

    /// Omit the x:xmpmeta element surrounding the rdf:RDF element.
	kXMP_OmitXMPMetaElement  : 0x1000,    
	
	/// Include a rdf Hash and Merged flag in x:xmpmeta element.
	kXMP_IncludeRDFHash      : 0x2000
};

var OpenFlagsObj = {
    /// Open for read-only access.
    kXMPFiles_OpenForRead           : 0x00000001,

    /// Open for reading and writing.
    kXMPFiles_OpenForUpdate         : 0x00000002,

    /// Only the XMP is wanted, allows space/time optimizations.
    kXMPFiles_OpenOnlyXMP           : 0x00000004,

    /// Force use of the given handler (format), do not even verify the format.
    kXMPFiles_ForceGivenHandler     : 0x00000008,
	
    /// Be strict about only attempting to use the designated file handler, no fallback to other handlers.
    kXMPFiles_OpenStrictly          : 0x00000010,

    /// Require the use of a smart handler.
    kXMPFiles_OpenUseSmartHandler   : 0x00000020,

    /// Force packet scanning, do not use a smart handler.
    kXMPFiles_OpenUsePacketScanning : 0x00000040,

    /// Only packet scan files "known" to need scanning.
    kXMPFiles_OpenLimitedScanning   : 0x00000080,

    /// Attempt to repair a file opened for update, default is to not open (throw an exception).
    kXMPFiles_OpenRepairFile        : 0x00000100,

    /// When updating a file, spend the effort necessary to optimize file layout.
    kXMPFiles_OptimizeFileLayout    : 0x00000200,

    /// Open the file using Generic Handler if there is no Smart handler for the file format.
    kXMPFiles_OpenUseGenericHandler : 0x00000400

};


var FormatObj = {

// ! Hex used to avoid gcc warnings. Leave the constants so the text reads big endian. There
// ! seems to be no decent way on UNIX to determine the target endianness at compile time.
// ! Forcing it on the client isn't acceptable.

// --------------------
// Public file formats.

/// Public file format constant: 'PDF '
    kXMP_PDFFile             : 0x50444620 ,
	/// Public file format constant: 'PS  ', general PostScript following DSC conventions
    kXMP_PostScriptFile      : 0x50532020 ,
	/// Public file format constant: 'EPS ', encaps ated PostScript
    kXMP_EPSFile             : 0x45505320 ,

	/// Public file format constant: 'JPEG'
    kXMP_JPEGFile            : 0x4A504547 ,
	/// Public file format constant: 'JPX ', JPEG 2000, ISO 15444-1
    kXMP_JPEG2KFile          : 0x4A505820 ,
	/// Public file format constant: 'TIFF'
    kXMP_TIFFFile            : 0x54494646 ,
	/// Public file format constant: 'GIF '
    kXMP_GIFFile             : 0x47494620 ,
	/// Public file format constant: 'PNG '
    kXMP_PNGFile             : 0x504E4720 ,

	/// Public file format constant: 'SWF '
    kXMP_SWFFile             : 0x53574620 ,
	/// Public file format constant: 'FLA '
    kXMP_FLAFile             : 0x464C4120 ,
	/// Public file format constant: 'FLV '
    kXMP_FLVFile             : 0x464C5620 ,

	/// Public file format constant: 'MOV ', Quicktime
    kXMP_MOVFile             : 0x4D4F5620 ,
	/// Public file format constant: 'AVI '
    kXMP_AVIFile             : 0x41564920 ,
	/// Public file format constant: 'CIN ', Cineon
    kXMP_CINFile             : 0x43494E20 ,
 	/// Public file format constant: 'WAV '
    kXMP_WAVFile             : 0x57415620 ,
	/// Public file format constant: 'MP3 '
    kXMP_MP3File             : 0x4D503320 ,
	/// Public file format constant: 'SES ', Audition session
    kXMP_SESFile             : 0x53455320 ,
	/// Public file format constant: 'CEL ', Audition loop
    kXMP_CELFile             : 0x43454C20 ,
	/// Public file format constant: 'MPEG'
    kXMP_MPEGFile            : 0x4D504547 ,
	/// Public file format constant: 'MP2 '
    kXMP_MPEG2File           : 0x4D503220 ,
	/// Public file format constant: 'MP4 ', ISO 14494-12 and -14
    kXMP_MPEG4File           : 0x4D503420 ,
	/// Public file format constant: 'MXF '
    kXMP_MXFFile             : 0x4D584620 ,
	/// Public file format constant: 'WMAV', Windows Media Audio and Video
    kXMP_WMAVFile            : 0x574D4156 ,
	/// Public file format constant:  'AIFF'
    kXMP_AIFFFile            : 0x41494646 ,
	/// Public file format constant:  'RED ', RED file format
    kXMP_REDFile            : 0x52454420 ,
    /// Public file format constant:  'ARRI', ARRI file format
    kXMP_ARRIFile           : 0x41525249 ,
	/// Public file format constant:  'HEIF', HEIF file format
	kXMP_HEIFFile : 0x48454946 ,
	/// Public file format constant:  'P2  ', a collection not really a single file
    kXMP_P2File              : 0x50322020 ,
	/// Public file format constant:  'XDCF', a collection not really a single file
    kXMP_XDCAM_FAMFile       : 0x58444346 ,
	/// Public file format constant:  'XDCS', a collection not really a single file
    kXMP_XDCAM_SAMFile       : 0x58444353 ,
	/// Public file format constant:  'XDCX', a collection not really a single file
    kXMP_XDCAM_EXFile        : 0x58444358 ,
	/// Public file format constant:  'AVHD', a collection not really a single file
    kXMP_AVCHDFile           : 0x41564844 ,
	/// Public file format constant:  'SHDV', a collection not really a single file
    kXMP_SonyHDVFile         : 0x53484456 ,
	/// Public file format constant:  'CNXF', a collection not really a single file
    kXMP_CanonXFFile         : 0x434E5846 ,
	/// Public file format constant:  'AVCU', a collection not really a single file
	kXMP_AVCUltraFile		 : 0x41564355 ,

	/// Public file format constant: 'HTML'
    kXMP_HTMLFile            : 0x48544D4C ,
	/// Public file format constant: 'XML '
    kXMP_XMLFile             : 0x584D4C20 ,
	/// Public file format constant:  'text'
    kXMP_TextFile            : 0x74657874 ,
	/// Public file format constant:  'SVG '
	kXMP_SVGFile			 : 0x53564720 ,


	// -------------------------------
    // Adobe application file formats.

	/// Adobe application file format constant: 'PSD '
    kXMP_PhotoshopFile       : 0x50534420 ,
	/// Adobe application file format constant: 'AI  '
    kXMP_IllustratorFile     : 0x41492020 ,
	/// Adobe application file format constant: 'INDD'
    kXMP_InDesignFile        : 0x494E4444 ,
	/// Adobe application file format constant: 'AEP '
    kXMP_AEProjectFile       : 0x41455020 ,
	/// Adobe application file format constant: 'AET ', After Effects Project Template
    kXMP_AEProjTemplateFile  : 0x41455420 ,
	/// Adobe application file format constant: 'FFX '
    kXMP_AEFilterPresetFile  : 0x46465820 ,
	/// Adobe application file format constant: 'NCOR'
    kXMP_EncoreProjectFile   : 0x4E434F52 ,
	/// Adobe application file format constant: 'PRPJ'
    kXMP_PremiereProjectFile : 0x5052504A ,
	/// Adobe application file format constant: 'PRTL'
    kXMP_PremiereTitleFile   : 0x5052544C ,
	/// Adobe application file format constant: 'UCF ', Universal Container Format
	kXMP_UCFFile             : 0x55434620 ,

	// -------
    // Others.

	/// Unknown file format constant: '    '
    kXMP_UnknownFile         : 0x20202020 

};

var XMPOpsErrorIdObj  = {
    /// No error
    kXMPOpsErr_NoError          :  0,
   
    kXMPOpsErr_Unknown          :  1,

    /// Generic bad parameter error
    kXMPOpsErr_BadParam         :   2,
    /// Generic bad value error
    kXMPOpsErr_BadValue         :   3,
    /// Generic internal failure
    kXMPOpsErr_InternalFailure  :   4,
    /// Generic deprecated error
    kXMPOpsErr_BadOptions       :  5,
    /// Generic external failure
    kXMPOpsErr_BadIndex  :  6,
    /// Generic standard exception
    kXMPOpsErr_BadXMP     :  7,
    /// Generic unknown exception
    kXMPOpsErr_BadStream :  8,
    /// Generic user abort error
    kXMPOpsErr_BadFileIO        :  9,
    /// Generic out-of-memory error
    kXMPOpsErr_XMPException         :  10,
    /// Generic unknown error
   

    kXMPOpsErr_NonExistingObject : 11
};

var XMPOpsException = function (id, message) {
    errorId = id;
    errorMessage = message;

    var getErrorId = function () {
        return errorId;
    }

    var getErrorMessage = function () {
        return errorMessage;
    }

    return {
        getErrorId: getErrorId,
        getErrorMessage: getErrorMessage
    }
};




const Namespaces = Object.freeze(NamespacesObj);
const SerializeOptions = Object.freeze(SerializeOptionsObj);
const XMPOpsErrorId = Object.freeze(XMPOpsErrorIdObj);
const Format = Object.freeze(FormatObj);
const OpenFlags = Object.freeze(OpenFlagsObj);

var VerifyStringArguments = function (x) {
    var result = x.every(function (i) { return typeof i === "string" || i == null });
    if (result == false) {
        throw XMPOpsException(XMPOpsErrorId.kXMPOpsErr_BadParam, "Parameter is expected to be string");
    }
}

var VerifyBooleanArguments = function (x) {
    var result = x.every(function (i) { return typeof i === "boolean" });
    if (result == false) {
        throw XMPOpsException(XMPOpsErrorId.kXMPOpsErr_BadParam, "Parameter is expected to be boolean");
    }
}

var VerifyNumberArguments = function (x) {
    var result = x.every(function (i) { return typeof i === "number" });
    if (result == false) {
        throw XMPOpsException(XMPOpsErrorId.kXMPOpsErr_BadParam, "Parameter is expected to be number");
    }
}

var VerifyXMPMetaOpsArguments = function (x) {
    var result = x.every(function (i) { return typeof i === "object" && i.GetMetaOpsRef });
    if (result == false) {
        throw XMPOpsException(XMPOpsErrorId.kXMPOpsErr_BadParam, "Parameter is expected to be XMPMetaOps");
    }
}

var ThrowExceptionFromNativeStatic = function (errorId, errorMessage) {
    var id = Module.getValue(errorId, 'i32');
    var msg = Module.Pointer_stringify(errorMessage);
    Module._free(errorId);
    Module._free(errorMessage);
    if (id != XMPOpsErrorId.kXMPOpsErr_NoError) {
        throw XMPOpsException(id, msg);
    }
};

var Throw = function () {
    throw XMPOpsException(XMPOpsErrorId.kXMPOpsErr_NonExistingObject, "The object is already destroyed");
};

var ThrowExceptionFromNative = function (errorId, errorMessage) {
    var id = Module.getValue(errorId, 'i32');
    if (id != XMPOpsErrorId.kXMPOpsErr_NoError) {
        var msg = Module.Pointer_stringify(errorMessage);
        throw XMPOpsException(id, msg);
    }
};

module.exports = {
    Namespaces: Namespaces,
    XMPOpsException: XMPOpsException,
    XMPOpsErrorId: XMPOpsErrorId,
    SerializeOptions: SerializeOptions,
    Format: Format,
    OpenFlags: OpenFlags,
    VerifyStringArguments: VerifyStringArguments,
    VerifyBooleanArguments: VerifyBooleanArguments,
    VerifyNumberArguments: VerifyNumberArguments,
    VerifyXMPMetaOpsArguments: VerifyXMPMetaOpsArguments,
    ThrowExceptionFromNativeStatic: ThrowExceptionFromNativeStatic,
    Throw: Throw,
    ThrowExceptionFromNative: ThrowExceptionFromNative
};