// =================================================================================================
// Copyright 2006-2007 Adobe Systems Incorporated
// All Rights Reserved.
//
// NOTICE:	Adobe permits you to use, modify, and distribute this file in accordance with the terms
// of the Adobe license agreement accompanying it.
// =================================================================================================


/*
 * JSUnit test mini framework.
 * It is based on the functionality of ExtendScript.
 */

 
/*
 * Main method to run one ore more tests
 */
function runTests(testFolderPath, outputFilePath)
{
    // As this framework is exclusivly used by XMPScript,
    // I load the library at this place
    loadLibrary();
	
    var testFolder = new Folder(testFolderPath);
    if (testFolder.toString().search("\.js$") > 0)
    {
        testFolder = new File(testFolderPath);
    }    

    if (testFolder.exists)
    {
        // initialize test results and open output file
        results = new TestResults(outputFilePath);

        recurseTests(results, testFolder);
        
        writeSummary(results);

        // close output file
        results.outputFile.close();

        if ((results.testCount == 0  ||  results.testFailures != 0)  ||  (results.testExceptions != 0))
        {
            throw "JSUnit tests FAILED"; //throwing exception to create bad (!=0) return value
        }
        else
        {
            _print("JSUnit tests PASSED");
        }
    }    
    else
    {
        _print("TestFolder or -file does not exist!");
		throw "JSUnit tests FAILED"; //throwing exception to create bad (!=0) return value
    }  


    // Unload the AdobeXMPScript library at the end of the tests.
    unloadLibrary();
}


/*
 * Runs all tests in the test folder and subfolders.
 * A test file is defined by its extension ".js",
 * there must not be other JavaScript-files unter the test folder.
 */
function recurseTests(results, testFolder)
{
    if (testFolder instanceof Folder)
    {
        // iterate through the test files
        testFiles = testFolder.getFiles();
        for (i in testFiles)
        {
            recurseTests(results, testFiles[i]);
        }
    }
    else if (testFolder.toString().search("\.js$") > 0)
    {
        // run single test file
        runTestCase(results, testFolder);
    }
}


/*
 * Runs all tests in a test file:
 *     1. TestCase#setUp() is called.
 *     2. All methods that start with "test" are called in undefined order.
 *     3. TestCase#shutDown() is called.
 */
function runTestCase(results, testFile)
{
    pathItems = testFile.toString().split("/");
    testName = pathItems[pathItems.length - 1];
    testName = testName.substring(0, testName.length - 3);
    
    // load test object, create and decorate it
    try
    {
        // load test file
        results.log ("\n\n");
        header = testName + " (" + testFile + ")\n";
        l = header.length - 1;
        for (i = 0; i < l; i++) { header += "="; }
        results.log (header);
        load(testFile);

        // count successful loaded test files
        results.testFiles++;

        // create instance
        test = eval("new " + testName);
        decorateTest(test, results);

        runSetUp(results, test);

        // iterate over the test functions
        methods = test.reflect.methods
        for (testMethod in methods)
        {
            if ("test" == methods[testMethod].name.substring(0,4))
            {
                runTestMethod(results, test, methods[testMethod].name);
            }
        }
        
        runTearDown(results, test);
        
        // cleanup: remove test case and all instances
        test = undefined;
        eval(testName + " = undefined");
    }
    catch(e)
    {
        results.log("Error: " + e);
    }
}


/*
 * Runs the setUp()-method in a test file.
 */
function runSetUp(results, test)
{
    // set up the test
    var setupMethod = test.reflect.find("setUp");
    if (setupMethod  &&  "method" == setupMethod.type)
    {
        try
        {
            eval("test.setUp()");
        }
        catch (e)
        {
            log("Error: setUp() failed (" + e + ")");
        }    
    }
}


/*
 * Runs one test...()-method in a test file and logs the test result.
 */
function runTestMethod(results, test, methodName)
{
    results.testCount++;
    logStr = results.testCount + ". " + methodName + ": ";
    try
    {
        eval("test." + methods[testMethod].name + "()");
        results.log(logStr + "passed");
    }
    catch (e)
    {
        if ("FAILURE" == e.toString().substring(0,7))
        {
            results.testFailures++;
            results.log(logStr + e);
        }    
        else
        {
            results.testExceptions++;
            results.log(logStr + "EXCEPTION (" + e + ")");
        }
    }
}


/*
 * Runs the tearDown()-method in a test file.
 */
function runTearDown(results, test)
{
    // set up the test
    var teardownMethod = test.reflect.find("tearDown");
    if (teardownMethod  &&  "method" == teardownMethod.type)
    {
        try
        {
            eval("test.tearDown()");
        }
        catch (e)
        {
            log("Error: tearDown() failed (" + e + ")");
        }    
    }
}


/*
 * Logs the overall result of all test cases.
 */
function writeSummary(results)
{
    results.log("\n\nSUMMARY\n=======");
    results.log("Test Files: " + results.testFiles);
    results.log("Test Cases: " + results.testCount);
    results.log("Failures:   " + results.testFailures);
    results.log("Exceptions: " + results.testExceptions);
}


/*
 * Load the AdobeXMPScript library
 */
function loadLibrary()
{
    if (ExternalObject.AdobeXMPScript == undefined)
    {
    	var libPath;
        if ($.os.indexOf("Win") == 0)
        {
            libPath = "lib:../AdobeXMPScript"; // Windows
        }
        else
        {
            libPath = "lib:../Frameworks/AdobeXMPScript.framework"; // Macintosh
        }   
        ExternalObject.AdobeXMPScript = new ExternalObject(libPath);
    }
}


/*
 * Unload the AdobeXMPScript library
 */
function unloadLibrary()
{
    // unload the libray
    if (ExternalObject. AdobeXMPScript != undefined)
    {
        ExternalObject.AdobeXMPScript.unload();
        ExternalObject. AdobeXMPScript = undefined;
    }
}


/*
 * Methods that are attached to the test case prototypes 
 * perform asserts and checks.
 */
function decorateTest(test, results)
{
	/*
	  * Fails the test controlled with a comment.
	  * Helpful if a code line must not reached for example.
	  */
    test.fail = function(comment)
    {
        // throws JS exception with the comment.
        // debug: _print("### fail(" + comment + ") called.");
        comment = comment ? comment : "fail() called";
        throw ("FAILURE (" + comment + ")");
    }


    /*
     * Assert that an expression results to true, 
     * otherwise fail with a comment.
     */
    test.assert = function(expr, comment)
    {
        // debug: _print("### assert(" + comment + ") called. result: " + expr);
        if (!expr)
        {
            comment = comment ? comment : "assert() is false";
            throw ("FAILURE (" + comment + ")");
        }    
    }
    
    
    /*
     * Assert that two expressions have the same result, 
     * otherwise fail with a comment.
     */
    test.assertEquals = function(value1, value2, comment)
    {
        // debug: _print("### assertEquals(" + comment + ") called. result: " + value1 == value2);
        if (value1 != value2)
        {
            comment = 
                (comment ? comment + " - " : "") + 
                "'" + value1 + "' expected, but was '" + value2 + "'";
            throw ("FAILURE (" + comment + ")");
        }    
    }


    /*
     * Assert that a code piece in a string - normally an object name -
     * evaluates without exception.
     */
    test.assertDefined = function(obj, comment)
    {
        try
        {
            eval(obj)
        }
        catch (e)
        {
            comment = comment ? comment : "'" + obj + "' is not defined";
            throw ("FAILURE (" + comment + ")");
        }    
    }


    /* 
     * Utility to compare a string to the content of a file.
     * Note: removes the versin from the serialized packets.
     */
    test.compareWithFile = function(string, filePath)
    {
        // load ref string
        ref = new File(filePath);
        ref.encoding = "UTF-8";    
        ref.open();
        refStr = ref.read();
        refStr = refStr.replace(/\sx:xmptk=\"Adobe[^"]*\"/,"")
        refStr = refStr.replace(/(\n|\r)+/g,"\n")
        ref.close();
        
        // remove toolkit version
        string = string.replace(/\sx:xmptk=\"Adobe[^"]*\"/,"")
        string = string.replace(/(\n|\r)+/g,"\n")
        
        this.assertEquals(refStr, string);
    }
    
    
    /*
     * Utility to create test files 
     */
    test.writeToFile = function(filePath, string)
    {
        out = new File(filePath);
        out.encoding = "UTF-8";    
        out.open("w");
        out.write(string);
        out.close();    
    }

}


/*
 * An object storing the test results and summary.
 */
function TestResults(outputFilePath)
{
    // counts test files
    this.testFiles = 0;
    // counts all tests
    this.testCount = 0;
    // counts failed tests
    this.testFailures = 0;
    // counts tests aborted with an exception
    this.testExceptions = 0;
    // the result output file 
    this.outputFile = new File(outputFilePath);
    
    this.outputFile.encoding = "UTF-8";    
    this.outputFile.open("w");
    
    this.log = function(text)
    {
        _print(text);
        this.outputFile.writeln(text);
    }
}

