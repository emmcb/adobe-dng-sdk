// =================================================================================================
// Copyright 2006-2007 Adobe Systems Incorporated
// All Rights Reserved.
//
// NOTICE:	Adobe permits you to use, modify, and distribute this file in accordance with the terms
// of the Adobe license agreement accompanying it.
// =================================================================================================


/* An example test for the simple JSUnit framework.
   Important: the  */


/*
 * This constructor function is equal to javas class name and should begin with a capital letter.
 * It is crucial that this object has the same name as the file,
 * e.g. when the file is called "ExampleTest.js", the function must be "function ExampleTest()".
 */
function ExampleTest()
{
    /*
     * Sets up the test environment
     */
    this.setUp = function ()
    {
         print("--> Example Test setUp()");
    }


    this.testSuccessful = function()
    {
        print("--> successful test");
    }


    this.testFailure = function()
    {
        // print("--> fail test");
        this.fail("This test failed for unambiguous reasons!");
    }


    this.testAssert = function()
    {
        // print("--> assert test");
        this.assert(false, "The result is not true!");
    }


    this.testAssertEquals = function()
    {
        // print("--> assertEquals test");
        this.assertEquals(1, 2, "The result is not true!");
    }


    this.testAbort = function()
    {
        // print("--> Abort test");
        throw ("CRASH!");
    }


    /* 
     * Cleans up the test environment, e.g. temporary 
     * files or objects created during the tests. 
     */
    this.tearDown = function()
    {
         print("--> Example Test tearDown()");
    }

}