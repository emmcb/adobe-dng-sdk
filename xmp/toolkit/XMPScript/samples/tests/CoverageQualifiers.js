// =================================================================================================
// Copyright 2006-2007 Adobe Systems Incorporated
// All Rights Reserved.
//
// NOTICE:	Adobe permits you to use, modify, and distribute this file in accordance with the terms
// of the Adobe license agreement accompanying it.
// =================================================================================================


var XMPScripting;

function CoverageQualifiers()
{
    /* Tests set- and getQualifier() */
    this.testGetSetQualifier = function()
    {
        xmp = new XMPMeta();

        xmp.setProperty(XMPConst.NS_DC, "simple", "simpleValue");
        xmp.setQualifier(XMPConst.NS_DC, "simple", XMPConst.TYPE_TEXT, "qual1", 10);
        xmp.setQualifier(XMPConst.NS_DC, "simple", XMPConst.TYPE_TEXT, "qual2", "Stefan");

        this.assertEquals(10, xmp.getQualifier(XMPConst.NS_DC, "simple", XMPConst.TYPE_TEXT, "qual1"));
        this.assertEquals("Stefan", xmp.getQualifier(XMPConst.NS_DC, "simple", XMPConst.TYPE_TEXT, "qual2"));
    }


    /* Tests set- and getQualifier() with a complex path */
    this.testGetSetQualifier2 = function()
    {
        xmp = new XMPMeta();
        
        xmp.setProperty(XMPConst.NS_DC, "struct/xmp:substruct/pdf:subsubstruct", "", XMPConst.PROP_IS_STRUCT); // optional
        xmp.setQualifier(XMPConst.NS_DC, "struct/xmp:substruct/pdf:subsubstruct", XMPConst.TYPE_TEXT, "qual1", 10);
        xmp.setQualifier(XMPConst.NS_DC, "struct/xmp:substruct/pdf:subsubstruct", XMPConst.TYPE_TEXT, "qual2", "Stefan");
        this.assertEquals(10, xmp.getQualifier(XMPConst.NS_DC, "struct/xmp:substruct/pdf:subsubstruct", XMPConst.TYPE_TEXT, "qual1"));
        this.assertEquals("Stefan", xmp.getQualifier(XMPConst.NS_DC, "struct/xmp:substruct/pdf:subsubstruct", XMPConst.TYPE_TEXT, "qual2"));
    }


    /* Tests doesQualifierExist() */
    this.testDoesQualifierExist = function()
    {
        xmp.setProperty(XMPConst.NS_DC, "simple", "simpleValue");
        xmp.setQualifier(XMPConst.NS_DC, "simple", XMPConst.TYPE_TEXT, "qual1", 10);
        xmp.setQualifier(XMPConst.NS_DC, "simple", XMPConst.TYPE_TEXT, "qual2", "Stefan");
        
        // positive
        this.assert(xmp.doesQualifierExist(XMPConst.NS_DC, "simple", XMPConst.TYPE_TEXT, "qual1"));
        this.assert(xmp.doesQualifierExist(XMPConst.NS_DC, "simple", XMPConst.TYPE_TEXT, "qual2"));

        // negative
        this.assert(!xmp.doesQualifierExist(XMPConst.NS_DC, "simple", XMPConst.TYPE_TEXT, "qual3"));
        this.assert(!xmp.doesQualifierExist(XMPConst.NS_DC, "simple", XMPConst.TYPE_PAGEDFILE, "qual2"));
    }


    /* Tests deleteQualifier() */
    this.testDeleteQualifier = function()
    {
        xmp.setProperty(XMPConst.NS_DC, "simple", "simpleValue");
        xmp.setQualifier(XMPConst.NS_DC, "simple", XMPConst.TYPE_TEXT, "qual1", 10);
        xmp.setQualifier(XMPConst.NS_DC, "simple", XMPConst.TYPE_TEXT, "qual2", "Stefan");
        xmp.deleteQualifier(XMPConst.NS_DC, "simple", XMPConst.TYPE_TEXT, "qual1");
        xmp.deleteQualifier(XMPConst.NS_DC, "simple", XMPConst.TYPE_TEXT, "qual2");
        
        // should be gone
        this.assert(!xmp.doesQualifierExist(XMPConst.NS_DC, "simple", XMPConst.TYPE_TEXT, "qual1"));
        this.assert(!xmp.doesQualifierExist(XMPConst.NS_DC, "simple", XMPConst.TYPE_TEXT, "qual2"));
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