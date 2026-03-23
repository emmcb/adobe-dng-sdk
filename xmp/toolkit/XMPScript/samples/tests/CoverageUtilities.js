// =================================================================================================
// Copyright 2006-2007 Adobe Systems Incorporated
// All Rights Reserved.
//
// NOTICE:	Adobe permits you to use, modify, and distribute this file in accordance with the terms
// of the Adobe license agreement accompanying it.
// =================================================================================================


var XMPScripting;

function CoverageUtilities()
{
    /* Test path composing */
    this.testPathComposing = function()
    {
        var myPath =
            XMPUtils.composeArrayItemPath(XMPConst.NS_DC, "seq", 10) + "/" +
            XMPUtils.composeStructFieldPath(XMPConst.NS_PDF, "struct", XMPConst.TYPE_TEXT, "structItem") + "/" +
            XMPUtils.composeLangSelector(XMPConst.NS_PHOTOSHOP, "langSelector", "de-CH") + "/" +
            XMPUtils.composeFieldSelector(XMPConst.NS_EXIF, "fieldSelector", XMPConst.TYPE_FONT, "Fontname", "Arial") + "/" +
            XMPUtils.composeQualifierPath(XMPConst.NS_TIFF, "qualifiedProp", XMPConst.TYPE_GRAPHICS, "qualifier");    
        this.assertEquals(
            'seq[10]/struct/xmpT:structItem/' +
            'langSelector[?xml:lang="de-CH"]/' + 
            'fieldSelector[stFnt:Fontname="Arial"]/' +
            'qualifiedProp/?xmpG:qualifier',
             myPath);
    }


    /* Test catenate and searate array methods */
    this.testCatenateArray = function()
    {
        xmp = new XMPMeta();
        xmp.setProperty( XMPConst.NS_XMP, "Array1", undefined, XMPConst.PROP_IS_ARRAY );
        xmp.appendArrayItem ( XMPConst.NS_XMP, "Array1", "one" );
        xmp.appendArrayItem ( XMPConst.NS_XMP, "Array1", "two" );
        xmp.appendArrayItem ( XMPConst.NS_XMP, "Array1", "3, four" );
        xmp.appendArrayItem ( XMPConst.NS_XMP, "Array1", "five; 6" );

        str = XMPUtils.catenateArrayItems(xmp, XMPConst.NS_XMP, "Array1", "; ", "\"", undefined);
        this.assertEquals('one; two; "3, four"; "five; 6"', str);

        XMPUtils.separateArrayItems ( xmp, XMPConst.NS_XMP, "Array2-1", null, str);
        XMPUtils.separateArrayItems ( xmp, XMPConst.NS_XMP, "Array2-2", XMPConst.SEPARATE_ALLOW_COMMAS, str);
        XMPUtils.separateArrayItems ( xmp, XMPConst.NS_XMP, "Array3-1", XMPConst.ARRAY_IS_ORDERED, str);
        XMPUtils.separateArrayItems ( xmp, XMPConst.NS_XMP, "Array3-2", XMPConst.ARRAY_IS_ORDERED | XMPConst.SEPARATE_ALLOW_COMMAS, str);

        str = xmp.serialize(0,1);
        this.compareWithFile(str, "tests/CoverageUtilities.xmp");
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