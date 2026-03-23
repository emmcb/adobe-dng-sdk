// =================================================================================================
// Copyright 2006-2007 Adobe Systems Incorporated
// All Rights Reserved.
//
// NOTICE:	Adobe permits you to use, modify, and distribute this file in accordance with the terms
// of the Adobe license agreement accompanying it.
// =================================================================================================


var XMPScripting;

function CoverageNamespaces()
{
    /* Tests registering a new namespace */
    this.testRegisterNamespace = function()
    {
        // own namespace, prefix
        XMPMeta.registerNamespace("http://mynamespace", "my:");
        XMPMeta.registerNamespace("http://mynamespace2", "my2");
        this.assertEquals("my:", XMPMeta.getNamespacePrefix("http://mynamespace"));
        this.assertEquals("my2:", XMPMeta.getNamespacePrefix("http://mynamespace2"));
        this.assertEquals("http://mynamespace", XMPMeta.getNamespaceURI("my:"));
        this.assertEquals("http://mynamespace2", XMPMeta.getNamespaceURI("my2"));
        
        // existing namespace
        this.assertEquals("xmp:", XMPMeta.getNamespacePrefix("http://ns.adobe.com/xap/1.0/"));
        this.assertEquals("http://ns.adobe.com/xap/1.0/", XMPMeta.getNamespaceURI("xmp"));
    }


    /* Tests the dump of all registered namespaces */
    this.testDumpNamespaces = function()
    {
        dump = XMPMeta.dumpNamespaces();
        this.compareWithFile(dump, "tests/CoverageNamespaces.txt");
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