// =================================================================================================
// Copyright 2006-2007 Adobe Systems Incorporated
// All Rights Reserved.
//
// NOTICE:	Adobe permits you to use, modify, and distribute this file in accordance with the terms
// of the Adobe license agreement accompanying it.
// =================================================================================================


var XMPScripting;

function CoverageSort()
{
    /* Tests set- and getProperty */
    this.testSort = function()
    {
        var file = new File("tests/CoverageSort#testSort.xmp");
        file.encoding = "UTF-8";    
        file.open();
        var xmpStr = file.read();
        file.close();

        var xmp = new XMPMeta(xmpStr);
		xmp.sort();
		
        var actual = xmp.dumpObject();

        var file = new File("tests/CoverageSort#testSort.txt");
        file.encoding = "UTF-8";    
		file.open("w");
		file.write(actual);
        file.close();
		
		// this.writeToFile("tests/CoverageSort#testSort.txt", actual);
        this.compareWithFile(actual, "tests/CoverageSort#testSort.txt");
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