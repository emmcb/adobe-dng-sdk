// =================================================================================================
// Copyright 2006-2007 Adobe Systems Incorporated
// All Rights Reserved.
//
// NOTICE:	Adobe permits you to use, modify, and distribute this file in accordance with the terms
// of the Adobe license agreement accompanying it.
// =================================================================================================


var XMPScripting;

function CoverageParseAndSerialize()
{
    /* Tests serialization of new XMPMeta instance */
    this.testNewMetadata = function()
    {
        xmp = new XMPMeta();
        actual = xmp.serialize(XMPConst.SERIALIZE_USE_COMPACT_FORMAT, 1);
        this.compareWithFile(actual, "tests/CoverageParseAndSerialize#testNewMetadata.xmp");
    }


    /* Tests parsing of an example XMPMeta-packet*/
    this.testParseMetadata = function()
    {
        file = new File("tests/CoverageParseAndSerialize#testParseMetadata.xmp");
        file.encoding = "UTF-8";    
        file.open();
        xmpStr = file.read();
        file.close();

        this.xmp = new XMPMeta(xmpStr);
        actual = this.xmp.dumpObject();
        this.compareWithFile(actual, "tests/CoverageParseAndSerialize#testParseMetadata.txt");
    }


    /* Tests serialising an example XMPMeta-packet*/
    this.testSerializeMetadata = function()
    {
//        actual = this.xmp.serialize(0, 1000, undefined, "-->", 2);
//        this.compareWithFile(actual, "tests/CoverageParseAndSerialize#testSerializeMetadata.xmp");
    }


    this.setUp = function ()
    {
         // Empty
    }


    this.tearDown = function()
    {
         // XMPScripting.unload();
    }
}
