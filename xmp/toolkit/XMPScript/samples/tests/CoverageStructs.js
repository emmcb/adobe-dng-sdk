var XMPScripting;

function CoverageStructs()
{
    /* Tests set- and getStructField() */
    this.testGetSetStructField = function()
    {
        xmp = new XMPMeta();
        
        xmp.setProperty(XMPConst.NS_DC, "struct", "", XMPConst.PROP_IS_STRUCT); // optional
        xmp.setStructField(XMPConst.NS_DC, "struct", XMPConst.TYPE_TEXT, "field1", 10);
        xmp.setStructField(XMPConst.NS_DC, "struct", XMPConst.TYPE_TEXT, "field2", "Stefan");
        this.assertEquals(10, xmp.getStructField(XMPConst.NS_DC, "struct", XMPConst.TYPE_TEXT, "field1"));
        this.assertEquals("Stefan", xmp.getStructField(XMPConst.NS_DC, "struct", XMPConst.TYPE_TEXT, "field2"));
    }


    /* Tests set- and getStructField() with a complex path */
    this.testGetSetStructField2 = function()
    {
        xmp = new XMPMeta();
        
        xmp.setProperty(XMPConst.NS_DC, "struct/xmp:substruct/pdf:subsubstruct", "", XMPConst.PROP_IS_STRUCT); // optional
        xmp.setStructField(XMPConst.NS_DC, "struct/xmp:substruct/pdf:subsubstruct", XMPConst.TYPE_TEXT, "field1", 10);
        xmp.setStructField(XMPConst.NS_DC, "struct/xmp:substruct/pdf:subsubstruct", XMPConst.TYPE_TEXT, "field2", "Stefan");
        this.assertEquals(10, xmp.getStructField(XMPConst.NS_DC, "struct/xmp:substruct/pdf:subsubstruct", XMPConst.TYPE_TEXT, "field1"));
        this.assertEquals("Stefan", xmp.getStructField(XMPConst.NS_DC, "struct/xmp:substruct/pdf:subsubstruct", XMPConst.TYPE_TEXT, "field2"));
    }


    /* Tests doesStructFieldExist() */
    this.testDoesStructFieldExist = function()
    {
        xmp.setStructField(XMPConst.NS_DC, "struct", XMPConst.TYPE_TEXT, "field1", 10);
        xmp.setStructField(XMPConst.NS_DC, "struct", XMPConst.TYPE_TEXT, "field2", "Stefan");
        
        // positive
        this.assert(xmp.doesStructFieldExist(XMPConst.NS_DC, "struct", XMPConst.TYPE_TEXT, "field1"));
        this.assert(xmp.doesStructFieldExist(XMPConst.NS_DC, "struct", XMPConst.TYPE_TEXT, "field2"));

        // negative
        this.assert(!xmp.doesStructFieldExist(XMPConst.NS_DC, "struct", XMPConst.TYPE_DIMENSIONS, "field1"));
        this.assert(!xmp.doesStructFieldExist(XMPConst.NS_DC, "struct", XMPConst.TYPE_TEXT, "field4711"));
    }


    /* Tests set- and getStructField() */
    this.testDeleteStructField = function()
    {
        xmp = new XMPMeta();
        
        xmp.setProperty(XMPConst.NS_DC, "struct", "", XMPConst.PROP_IS_STRUCT); // optional
        xmp.setStructField(XMPConst.NS_DC, "struct", XMPConst.TYPE_TEXT, "field1", 10);
        xmp.setStructField(XMPConst.NS_DC, "struct", XMPConst.TYPE_TEXT, "field2", "Stefan");
        xmp.deleteStructField(XMPConst.NS_DC, "struct", XMPConst.TYPE_TEXT, "field1");
        xmp.deleteStructField(XMPConst.NS_DC, "struct", XMPConst.TYPE_TEXT, "field2");
        this.assert(!xmp.doesStructFieldExist(XMPConst.NS_DC, "struct", XMPConst.TYPE_TEXT, "field1"));
        this.assert(!xmp.doesStructFieldExist(XMPConst.NS_DC, "struct", XMPConst.TYPE_TEXT, "field2"));
    }


    this.setUp = function ()
    {
         // Empty
    }


    this.tearDown = function()
    {
         // Empty
    }
}