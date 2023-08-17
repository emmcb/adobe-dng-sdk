// =================================================================================================
// Copyright 2006-2007 Adobe Systems Incorporated
// All Rights Reserved.
//
// NOTICE:	Adobe permits you to use, modify, and distribute this file in accordance with the terms
// of the Adobe license agreement accompanying it.
// =================================================================================================


var XMPScripting;

function CoverageGetSet()
{
    /* Tests set- and getProperty */
    this.testGetSetProperty = function()
    {
        xmp = new XMPMeta();
        
        xmp.setProperty(XMPConst.NS_XMP, "CreatorTool", "ExtendScript");
        prop = xmp.getProperty(XMPConst.NS_XMP, "CreatorTool");
        this.assert(prop instanceof XMPProperty);
        this.assertEquals(XMPConst.NS_XMP, prop.namespace);
        this.assertEquals("CreatorTool", prop.path);
        this.assertEquals("ExtendScript", prop.value);
    }


    /* Tests set- and getProperty with a path */
    this.testGetSetProperty2 = function()
    {
        xmp = new XMPMeta();
        
        xmp.setProperty(XMPConst.NS_DC, "dc:prop/dc:prop2/dc:prop3", "value");
        prop = xmp.getProperty(XMPConst.NS_DC, "dc:prop/dc:prop2/dc:prop3");
        this.assert(prop instanceof XMPProperty);
        this.assertEquals(XMPConst.NS_DC, prop.namespace);
        this.assertEquals("dc:prop/dc:prop2/dc:prop3", prop.path);
        this.assertEquals("value", prop.value);
    }


    /* Tests set- and getProperty with a path */
    this.testDoesPropertyExists = function()
    {
        xmp = new XMPMeta();
        
        xmp.setProperty(XMPConst.NS_XMP, "CreatorTool", "ExtendScript");
        xmp.setProperty(XMPConst.NS_DC, "dc:prop/dc:prop2/dc:prop3", "value");
        
        // positive
        this.assert(xmp.doesPropertyExist(XMPConst.NS_XMP, "CreatorTool"));
        this.assert(xmp.doesPropertyExist(XMPConst.NS_DC, "dc:prop/dc:prop2/dc:prop3"));
        
        // negative
        this.assert(!xmp.doesPropertyExist(XMPConst.NS_XMP, "ldfkdsljf"));
        this.assert(!xmp.doesPropertyExist(XMPConst.NS_DC, "dc:prop/dc:prop2/dc:prop4711"));
    }


    /* Tests deleteProperty */
    this.testDeleteProperty = function()
    {
        xmp = new XMPMeta();
        
        xmp.setProperty(XMPConst.NS_XMP, "CreatorTool", "ExtendScript");
        xmp.deleteProperty(XMPConst.NS_XMP, "CreatorTool");
        // delete unknown property is allowed 
        xmp.deleteProperty(XMPConst.NS_XMP, "dfdsfkjl");
        
        this.assert(!xmp.doesPropertyExist(XMPConst.NS_XMP, "CreatorTool"));
    }


    /* Tests localized set-/getProperty() */
    this.testLocalizedProperties = function()
    {
        xmp = new XMPMeta();

        xmp.setLocalizedText(XMPConst.NS_DC, "description", null, "de-de", "Beschreibung Deutschland");
        xmp.setLocalizedText(XMPConst.NS_DC, "description", null, "de-CH", "Beschriftung Schweiz");
        xmp.setLocalizedText(XMPConst.NS_DC, "description", null, "de", "Beschriftung Deutsch");

        // retrieve local
        loc = xmp.getLocalizedText(XMPConst.NS_DC,"description", null, "de-CH");
        this.assertEquals("Beschriftung Schweiz", loc.value);
        this.assertEquals("de-CH", loc.locale);
        this.assertEquals(0x50, loc.options); // TODO: PROP_HAS_LANG | PROP_HAS_QUALIFIERS

        // fallback to language
        loc = xmp.getLocalizedText(XMPConst.NS_DC,"description", null, "de");
        this.assertEquals("Beschriftung Deutsch", loc.value);
        this.assertEquals("de", loc.locale);
        this.assertEquals(0x50, loc.options); // TODO: PROP_HAS_LANG | PROP_HAS_QUALIFIERS
    }


    /* Tests set- and getProperty with "boolean" */
    this.testGetSetPropertyBoolean = function()
    {
        xmp = new XMPMeta();

        // dangerous, because XMP uses the string "True" 
        xmp.setProperty(XMPConst.NS_DC, "boolTrue", true);
        xmp.setProperty(XMPConst.NS_DC, "boolTrue2", true, 0, "boolean");
        // dangerous, because XMP uses the string "False" 
        xmp.setProperty(XMPConst.NS_DC, "boolFalse", "false"); // this sets a string
        xmp.setProperty(XMPConst.NS_DC, "boolFalse2", false, 0, "boolean");
        // returns String
        this.assertEquals("True", xmp.getProperty(XMPConst.NS_DC, "boolTrue").value);
        // returns forced boolean
        this.assertEquals(true, xmp.getProperty(XMPConst.NS_DC, "boolTrue2", "boolean").value);
        // returns String
        this.assertEquals("false", xmp.getProperty(XMPConst.NS_DC, "boolFalse").value);
        // returns forced boolean
        this.assertEquals(false, xmp.getProperty(XMPConst.NS_DC, "boolFalse2", "boolean").value);
    }


    /* Tests set- and getProperty with "integer" */
    this.testGetSetPropertyInteger = function()
    {
        xmp = new XMPMeta();

        xmp.setProperty(XMPConst.NS_DC, "int", 4711);
        xmp.setProperty(XMPConst.NS_DC, "int2", 4711, 0, "integer");
        xmp.setProperty(XMPConst.NS_DC, "int3", 4711.9998, 0, "integer");
        
        // integer is converted automatically anyway
        this.assertEquals(4711, xmp.getProperty(XMPConst.NS_DC, "int").value);
        this.assertEquals("4711", xmp.getProperty(XMPConst.NS_DC, "int").value);
        this.assertEquals(4711, xmp.getProperty(XMPConst.NS_DC, "int2", "integer").value);
        this.assertEquals("4711", xmp.getProperty(XMPConst.NS_DC, "int2", "integer").value);

        // with floating point numbers it gets more interesting
        this.assertEquals(4711.9998, xmp.getProperty(XMPConst.NS_DC, "int3").value);
        
        // recognized non-integer
        try
        {
            xmp.getProperty(XMPConst.NS_DC, "int3", "integer");
            this.fail("Expected Exception!");
        }
        catch (e)
        {
            this.assertEquals("Error: XMP Exception: Invalid integer string", e.toString());
        }    
    }


    /* Tests set- and getProperty with "number" */
    this.testGetSetPropertyNumber = function()
    {
        xmp = new XMPMeta();

        // type number
        xmp.setProperty(XMPConst.NS_DC, "numberStr", 123);
        xmp.setProperty(XMPConst.NS_DC, "numberDecimalStr", 123.456);
        xmp.setProperty(XMPConst.NS_DC, "numberForced", 123, 0, "number");
        xmp.setProperty(XMPConst.NS_DC, "numberDecimalForced", 123.456, 0, "number");

        // normal get (comparison with string makes it more clear)
        this.assertEquals("123", xmp.getProperty(XMPConst.NS_DC, "numberStr").value);
        this.assertEquals("123.456000", xmp.getProperty(XMPConst.NS_DC, "numberDecimalStr").value);
        this.assertEquals("123.000000", xmp.getProperty(XMPConst.NS_DC, "numberForced").value);
        this.assertEquals("123.456000", xmp.getProperty(XMPConst.NS_DC, "numberDecimalForced").value);

        // forced get
        this.assertEquals("123", xmp.getProperty(XMPConst.NS_DC, "numberStr", "number").value);
        this.assertEquals("123.456", xmp.getProperty(XMPConst.NS_DC, "numberDecimalStr", "number").value);
        this.assertEquals("123", xmp.getProperty(XMPConst.NS_DC, "numberForced", "number").value);
        this.assertEquals("123.456", xmp.getProperty(XMPConst.NS_DC, "numberDecimalForced", "number").value);
        
        // take only the integer part of the number (fails)
        try {
            // get a number as integer
            xmp.getProperty(XMPConst.NS_DC, "numberDecimalStr", "integer");
            this.fail("Exception expected!");
        } catch (e) {
            this.assertEquals("XMP Exception: Invalid integer string", e.message);
        }
    }


    /* Tests set- and getProperty with "date" */
    this.testGetSetPropertyDate = function()
    {
        xmp = new XMPMeta();

        // type xmpdate
        dt = new XMPDateTime(new Date()); // current date
        refStr = dt.toString();
        
        xmp.setProperty(XMPConst.NS_DC, "dateStr", dt);
        xmp.setProperty(XMPConst.NS_DC, "dateForced", dt, 0, "xmpdate");
        try
        {
            xmp.setProperty(XMPConst.NS_DC, "dateForced", dt.toString(), 0, "xmpdate");
        }
        catch (e)
        {
            this.assertEquals("Bad argument - The (optional) value type can only be one of " + 
                "'string', 'integer', 'number', 'boolean' or 'xmpdate' " +
                "used with a corresponding value type!", e.message);
        }   
        
        this.assertEquals(refStr, xmp.getProperty(XMPConst.NS_DC, "dateStr"));
        this.assertEquals(refStr, xmp.getProperty(XMPConst.NS_DC, "dateForced"));
        this.assertEquals(refStr, xmp.getProperty(XMPConst.NS_DC, "dateStr", "xmpdate"));
        this.assertEquals(refStr, xmp.getProperty(XMPConst.NS_DC, "dateForced", "xmpdate"));
    }


    /* Tests getProperty() when its not existing */
    this.testGetPropertyNotExisting = function()
    {
        xmp = new XMPMeta();

	// not existing string property
        this.assertEquals(undefined, xmp.getProperty(XMPConst.NS_DC, "notexisting"));
	// not existing string property 2
        this.assertEquals(undefined, xmp.getProperty(XMPConst.NS_DC, "notexisting", "string"));
	// not existing boolean property
        this.assertEquals(undefined, xmp.getProperty(XMPConst.NS_DC, "notexisting", "boolean"));
	// not existing integer property
        this.assertEquals(undefined, xmp.getProperty(XMPConst.NS_DC, "notexisting", "integer"));
	// not existing number property
        this.assertEquals(undefined, xmp.getProperty(XMPConst.NS_DC, "notexisting", "number"));
	// not existing date property
        this.assertEquals(undefined, xmp.getProperty(XMPConst.NS_DC, "notexisting", "xmpdate"));
    }


	/* Test unicode handling */
    this.testGetSetUnicode = function()
    {
        xmp = new XMPMeta();
		
		xmp.appendArrayItem(XMPConst.NS_DC, "creator", "木星", 0, XMPConst.ARRAY_IS_ORDERED);
		xmp.setProperty(XMPConst.NS_PHOTOSHOP, "City", "木星");
		xmp.setQualifier(XMPConst.NS_PHOTOSHOP, "City", XMPConst.NS_PHOTOSHOP, "cityQual", "木星");
		xmp.setLocalizedText(XMPConst.NS_DC, "title", null, "zh-CN", "木星");
		xmp.setStructField(XMPConst.NS_IPTC_CORE, "CreatorContactInfo", XMPConst.NS_IPTC_CORE, "CiAdrCity", "木星");
		
		this.assertEquals(1, xmp.countArrayItems(XMPConst.NS_DC, "creator"));
		this.assertEquals("木星", xmp.getArrayItem(XMPConst.NS_DC, "creator", 1));
		this.assertEquals("木星", xmp.getProperty(XMPConst.NS_PHOTOSHOP, "City"));
		this.assertEquals("木星", xmp.getQualifier(XMPConst.NS_PHOTOSHOP, "City", XMPConst.NS_PHOTOSHOP, "cityQual"));
		this.assertEquals("木星", xmp.getLocalizedText(XMPConst.NS_DC,"title", null, "zh-CN"));
		this.assertEquals("木星", xmp.getStructField(XMPConst.NS_IPTC_CORE, "CreatorContactInfo", XMPConst.NS_IPTC_CORE, "CiAdrCity"));
	}
	
	
    /* Makes sure that the lib has been loaded. */
    this.setUp = function ()
    {
        if (XMPScripting == undefined)
        {
            XMPScripting = new ExternalObject("lib:../AdobeXMPScript");
        }    
    }


    this.tearDown = function()
    {
         // Empty
    }
}