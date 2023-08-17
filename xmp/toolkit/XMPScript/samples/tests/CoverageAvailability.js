// =================================================================================================
// Copyright 2006-2008 Adobe Systems Incorporated
// All Rights Reserved.
//
// NOTICE:	Adobe permits you to use, modify, and distribute this file in accordance with the terms
// of the Adobe license agreement accompanying it. 
// =================================================================================================


var XMPScripting;


function CoverageAvailability()
{
	/* Tests the ExternalObject version-property and getVersion()-method. */
	this.testExternalObjectVersions = function()
	{
        this.assertDefined("ExternalObject.AdobeXMPScript");
		this.assertEquals(1, ExternalObject.AdobeXMPScript.version);
		if ($.os.indexOf("Win") == 0)
		{
			// not defined on Mac
			this.assert(ExternalObject.AdobeXMPScript.getVersion().match("Adobe XMP Script 5[\.0-9]+-s[0-9][0-9][0-9]") != null);
		}	
	}

	
	/* Tests the XMPCore version. */
	this.testXMPCoreVersion = function()
	{
     	this.assert(XMPMeta.version.match("Adobe XMP Core 6[\.0-9]+-c[0-9][0-9][0-9]") != null);
	}


	/* Tests the XMPFile version. */
	this.testXMPFileVersion = function()
	{
     	this.assert(XMPFile.version.match("Adobe XMP Files 6[\.0-9]+-f[0-9][0-9][0-9]") != null);
	}
	
	
    /* Tests if all XMPScript objects that are created by factories are available */
    this.testAvalability = function()
    {
        this.assertDefined("XMPMeta");
        this.assertDefined("XMPFile");
        this.assertDefined("XMPDateTime");
        this.assertDefined("XMPUtils");
        this.assertDefined("XMPConst");
    }


    /* Tests availability of XMPMeta */
    this.testAvailabilityXMPMeta = function()
    {
        // instance properties
        xmp = new XMPMeta();
        this.assertEquals(26, xmp.reflect.methods.length);
        this.assertEquals(1, xmp.reflect.properties.length);

        // static properties
        this.assertEquals(8, XMPMeta.reflect.methods.length);
        this.assertEquals(8, XMPMeta.reflect.properties.length);
    }


    /* Tests availability of XMPFile */
	this.testAvailabilityXMPFile = function()
	{
		// instance properties
		f = new XMPFile("tests/CoverageXMPFile.psd", 0, 0);
		this.assertEquals(6, f.reflect.methods.length);
		this.assertEquals(1, f.reflect.properties.length);

		// static properties
		this.assertEquals(4, XMPFile.reflect.methods.length);
		this.assertEquals(8, XMPFile.reflect.properties.length);
		
		f.closeFile();
	}

	
    /* Tests availability of XMPDateTime */
    this.testAvailabilityXMPDateTime = function()
    {
        // instance properties
        dt = new XMPDateTime();
        this.assertEquals(8, dt.reflect.methods.length);
        this.assertEquals(11, dt.reflect.properties.length);

        // static properties
        this.assertEquals(3, XMPDateTime.reflect.methods.length);
        this.assertEquals(7, XMPDateTime.reflect.properties.length);
    }


    /* Tests availability of XMPUtils */
    this.testAvailabilityXMPUtils = function()
    {
		 // static properties
		XMPUtils.reflect.properties.length; // make sure it has its final length
        this.assertEquals(13, XMPUtils.reflect.methods.length);
        this.assertEquals(7, XMPUtils.reflect.properties.length);

        // has no constructor
        try
        {
            new XMPConst()
            fail("Instances are not allowed!");
        }
        catch (e)
        {
            // EMPTY            
        }   
    }


    /* Tests availability of XMPUtils */
    this.testAvailabilityXMPConst = function()
    {
		XMPConst.reflect.properties.length; // make sure it has its final length
        // static properties
		XMPUtils.reflect.properties.length; // make sure it has its final length
        this.assertEquals(3, XMPConst.reflect.methods.length);
        this.assertEquals(168, XMPConst.reflect.properties.length);

        // has no constructor
        try
        {
            new XMPConst()
            fail("Instances are not allowed!");
        }
        catch (e)
        {
            // EMPTY            
        }   
    }

	
    /* Tests availability of XMPUtils */
    this.testXMPConst = function()
    {
		expected = 
			"TRUE,FALSE,STRING,INTEGER,NUMBER,BOOLEAN,XMPDATE,NS_DC,NS_IPTC_CORE,NS_RDF,NS_XML,NS_EXIFEX,NS_XMP,NS_XMP_RIGHTS,NS_XMP_MM" +
			",NS_XMP_BJ,NS_XMP_NOTE,NS_PDF,NS_PDFX,NS_PDFX_ID,NS_PHOTOSHOP,NS_PS_ALBUM,NS_EXIF,NS_EXIF_AUX,NS_TIFF,NS_PNG,NS_SWF,NS_JPEG,NS_JP2K" +
			",NS_CAMERA_RAW,NS_DM,NS_ASF,NS_WAV,NS_ADOBE_STOCK_PHOTO,TYPE_IDENTIFIER_QUAL,TYPE_DIMENSIONS,TYPE_TEXT,TYPE_PAGEDFILE,TYPE_GRAP" +
			"HICS,TYPE_IMAGE,TYPE_FONT,TYPE_RESOURCE_EVENT,TYPE_RESOURCE_REF,TYPE_ST_VERSION,TYPE_ST_JOB,TYPE_MANIFEST_ITEM,TYPE_PDFA" +
			"_SCHEMA,TYPE_PDFA_PROPERTY,TYPE_PDFA_TYPE,TYPE_PDFA_FIELD,TYPE_PDFA_ID,TYPE_PDFA_EXTENSION,NO_OPTIONS,SCHEMA_NODE,PROP_V" +
			"ALUE_IS_URI,PROP_HAS_QUALIFIERS,PROP_IS_QUALIFIER,PROP_HAS_LANG,PROP_HAS_TYPE,PROP_IS_ALIAS,PROP_HAS_ALIASES,PROP_IS_INT" +
			"ERNAL,PROP_IS_STABLE,PROP_IS_DERIVED,PROP_IS_STALE,ALIAS_TO_SIMPLE_PROP,ALIAS_TO_ARRAY,ALIAS_TO_OR" +
			"DERED_ARRAY,ALIAS_TO_ALT_ARRAY,ALIAS_TO_ALT_TEXT,PROP_IS_STRUCT,PROP_IS_ARRAY,ARRAY_IS_UNORDERED,ARRAY_IS_ORDERED,ARRAY_" +
			"IS_ALTERNATIVE,ARRAY_IS_ALT_TEXT,ARRAY_LAST_ITEM,SERIALIZE_OMIT_PACKET_WRAPPER,SERIALIZE_READ_ONLY_PACKET,SERIALIZE_USE_" +
			"COMPACT_FORMAT,SERIALIZE_INCLUDE_THUMBNAIL_PAD,SERIALIZE_EXACT_PACKET_LENGTH,SERIALIZE_WRITE_ALI" +
			"AS_COMMENTS,SERIALIZE_OMIT_ALL_FORMATTING,ENCODE_UTF8,ENCODE_UTF16_BIG,ENCODE_UTF16_LITTLE,ENCODE_UTF32_BIG,ENCODE_UTF32" +
			"_LITTLE,ITERATOR_JUST_CHILDREN,ITERATOR_JUST_LEAFNODES,ITERATOR_JUST_LEAFNAME,ITERATOR_OMIT_QUA" +
			"LIFIERS,REMOVE_ALL_PROPERTIES,REMOVE_INCLUDE_ALIASES,APPEND_ALL_PROPERTIES,APPEND_REPLACE_OLD_VALUES,APPEND_DELETE_EMPTY" +
			"_VALUES,SEPARATE_ALLOW_COMMAS,TIME_WEST_OF_UTC,TIME_UTC,TIME_EAST_OF_UTC,FILE_UNKNOWN,FILE_PDF,FILE_POSTSCRIPT,FILE_EPS," +
			"FILE_JPEG,FILE_JPEG2K,FILE_TIFF,FILE_GIF,FILE_PNG,FILE_SWF,FILE_FLA,FILE_FLV,FILE_MOV,FILE_AVI,FILE_CIN,FILE_WAV,FILE_MP" +
			"3,FILE_SES,FILE_CEL,FILE_MPEG,FILE_MPEG2,FILE_MPEG4,FILE_WMAV,FILE_AIFF,FILE_HTML,FILE_XML,FILE_TEXT,FILE_PHOTOSHOP,FILE" +
			"_ILLUSTRATOR,FILE_INDESIGN,FILE_AE_PROJECT,FILE_AE_PROJECT_TEMPLATE,FILE_AE_FILTER_PRESET,FILE_ENCORE_PROJECT,FILE_PREMI" +
			"ERE_PROJECT,FILE_PREMIERE_TITLE,FILE_WEBP,OPEN_FOR_READ,OPEN_FOR_UPDATE,OPEN_ONLY_XMP,OPEN_CACHE_TNAIL,OPEN_STRICTLY,OPEN_USE_SMAR" +
			"T_HANDLER,OPEN_USE_PACKET_SCANNING,OPEN_LIMITED_SCANNING,HANDLER_CAN_INJECT_XMP,HANDLER_CAN_EXPAND,HANDLER_CAN_REWRITE,H" +
			"ANDLER_PPEFERS_IN_PLACE,HANDLER_CAN_RECONCILE,HANDLER_ALLOWS_ONLY_XMP,HANDLER_RETURNS_RAW_PACKETS,HANDLER_RETURNS_TNAIL," +
			"HANDLER_OWNS_FILE,HANDLER_ALLOWS_SAFE_UPDATE,HANDLER_NEEDS_READONLY_PACKET,HANDLER_USES_SIDECAR_XMP,HANDLER_CAN_NOTIFY_PROGRESS,CLOSE_UPDATE_SAFELY";
			
		str = XMPConst.reflect.properties.toString();
		str = str.substring(0, str.indexOf(",prototype"));
		
        // compare property string
        this.assertEquals(expected, str);
    }

	
    this.setUp = function ()
    {
         // Empty
    }


    this.tearDown = function()
    {
         // Empty
    }
 
 
//    /* Utility to count properties without the standards */
//    this.countProperties = function(obj)
//    {
//        count = 0;
//        props = obj.reflect.properties;
//        for (p in props)
//        {
//            alert(p);
//            if (!props[p].name.match("^(__proto__|name|arity|arguments|prototype|length)$"))
//                count++;
//        }
//        return count;    
//    }


//    /* Utility to count properties without the standards */
//    this.countMethods = function(obj)
//    {
//        alert(m);
//        count = 0;
//        methods = obj.reflect.methods;
//        for (m in methods)
//        {
//            if (!methods[m].name.match("^(apply|call|toSource)$"))
//                count++;
//        }
//        return count;    
//    }    
}
