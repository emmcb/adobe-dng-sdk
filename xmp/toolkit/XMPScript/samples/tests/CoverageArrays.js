var XMPScripting;

function CoverageArrays()
{
    /* Tests create different array types */
    this.testCreateArrays = function()
    {
        xmp = new XMPMeta();
        
        // beside different array types, use different possiblities for absend values: null, undefined and ""
        xmp.setProperty(XMPConst.NS_DC, "bagArray", undefined, XMPConst.PROP_IS_ARRAY);
        xmp.setProperty(XMPConst.NS_DC, "sequenceArray", "", XMPConst.ARRAY_IS_ORDERED);
        xmp.setProperty(XMPConst.NS_DC, "alternativeArray", null, XMPConst.ARRAY_IS_ALTERNATIVE);

        options = xmp.getProperty(XMPConst.NS_DC, "bagArray").options;
        this.assertEquals(XMPConst.PROP_IS_ARRAY, options);
        options = xmp.getProperty(XMPConst.NS_DC, "sequenceArray").options;
        this.assertEquals(XMPConst.PROP_IS_ARRAY | XMPConst.ARRAY_IS_ORDERED, options);
        options = xmp.getProperty(XMPConst.NS_DC, "alternativeArray").options;
        this.assertEquals(XMPConst.PROP_IS_ARRAY | XMPConst.ARRAY_IS_ALTERNATIVE | XMPConst.ARRAY_IS_ORDERED, options);
    }


    /* Tests appendArrayItem() */
    this.testAppendArrayItem = function()
    {
        xmp = new XMPMeta();

        // implicitly create array
        xmp.appendArrayItem(XMPConst.NS_DC, "sequenceArray", "item 1", 0, XMPConst.ARRAY_IS_ORDERED);
        xmp.appendArrayItem(XMPConst.NS_DC, "sequenceArray", "item 2");
        xmp.appendArrayItem(XMPConst.NS_DC, "sequenceArray", "item 3");
        
        // test countArrayItems()
        this.assertEquals(3, xmp.countArrayItems(XMPConst.NS_DC, "sequenceArray"));
        
        this.assertEquals("item 1", xmp.getArrayItem(XMPConst.NS_DC, "sequenceArray", 1));
        this.assertEquals("item 2", xmp.getArrayItem(XMPConst.NS_DC, "sequenceArray", 2));
        this.assertEquals("item 3", xmp.getArrayItem(XMPConst.NS_DC, "sequenceArray", 3).value);
        // use the special constant "last item"
        this.assertEquals("item 3", xmp.getArrayItem(XMPConst.NS_DC, "sequenceArray", XMPConst.ARRAY_LAST_ITEM));
    }

	
    /* Tests setArrayItem() */
    this.testSetArrayItem = function()
    {
        xmp = new XMPMeta();

        // implicitly create array
        xmp.appendArrayItem(XMPConst.NS_DC, "sequenceArray", "item 1", 0, XMPConst.ARRAY_IS_ORDERED);
        xmp.appendArrayItem(XMPConst.NS_DC, "sequenceArray", "item 2");
        xmp.appendArrayItem(XMPConst.NS_DC, "sequenceArray", "item 3");
        xmp.setArrayItem(XMPConst.NS_DC, "sequenceArray", 1, "new item 1");
        xmp.setArrayItem(XMPConst.NS_DC, "sequenceArray", 2, "new item 2");
        xmp.setArrayItem(XMPConst.NS_DC, "sequenceArray", 3, "new item 3");
        xmp.setArrayItem(XMPConst.NS_DC, "sequenceArray", 4, "new item appended");
        xmp.dumpObject();

        // test countArrayItems()
        this.assertEquals(4, xmp.countArrayItems(XMPConst.NS_DC, "sequenceArray"));
        
        this.assertEquals("new item 1", xmp.getArrayItem(XMPConst.NS_DC, "sequenceArray", 1));
        this.assertEquals("new item 2", xmp.getArrayItem(XMPConst.NS_DC, "sequenceArray", 2));
        this.assertEquals("new item 3", xmp.getArrayItem(XMPConst.NS_DC, "sequenceArray", 3).value);
        this.assertEquals("new item appended", xmp.getArrayItem(XMPConst.NS_DC, "sequenceArray", 4).value);
    }
	

    /* Tests insertArrayItem() */
    this.testInsertArrayItem = function()
    {
        xmp = new XMPMeta();

        // create an array
        xmp.appendArrayItem(XMPConst.NS_DC, "sequenceArray", "item 1,0", 0, XMPConst.ARRAY_IS_ORDERED);
        xmp.appendArrayItem(XMPConst.NS_DC, "sequenceArray", "item 2,0");
        xmp.appendArrayItem(XMPConst.NS_DC, "sequenceArray", "item 3,0");

        // insert additional items
        xmp.insertArrayItem(XMPConst.NS_DC, "sequenceArray", 1, "item 0,5");
        xmp.insertArrayItem(XMPConst.NS_DC, "sequenceArray", 3, "item 1,5");
        xmp.insertArrayItem(XMPConst.NS_DC, "sequenceArray", 5, "item 2,5");
        xmp.insertArrayItem(XMPConst.NS_DC, "sequenceArray", 7, "item 3,5");

        this.compareWithFile(xmp.serialize(0,1), "tests/CoverageArrays.xmp");
    }

	
    /* Tests insertArrayItem() */
    this.testArrayMethodsWithStructItems = function()
    {
        xmp = new XMPMeta();

        // create an array
		xmp.appendArrayItem(XMPConst.NS_XMP, "History", null, XMPConst.PROP_IS_STRUCT, XMPConst.ARRAY_IS_ORDERED);
		xmp.setArrayItem(XMPConst.NS_XMP, "History", 2, null, XMPConst.PROP_IS_STRUCT);
		xmp.insertArrayItem(XMPConst.NS_XMP, "History", 3, undefined, XMPConst.PROP_IS_STRUCT);

        // test countArrayItems()
        this.assertEquals(3, xmp.countArrayItems(XMPConst.NS_XMP, "History"));
        
        this.assertEquals(XMPConst.PROP_IS_STRUCT, xmp.getArrayItem(XMPConst.NS_XMP, "History", 1).options);
        this.assertEquals(XMPConst.PROP_IS_STRUCT, xmp.getArrayItem(XMPConst.NS_XMP, "History", 2).options);
        this.assertEquals(XMPConst.PROP_IS_STRUCT, xmp.getArrayItem(XMPConst.NS_XMP, "History", 3).options);
    }

	
    /* Tests insertArrayItem() */
    this.testDoesArrayItemExist = function()
    {
        xmp = new XMPMeta();

        // create an array
        xmp.appendArrayItem(XMPConst.NS_DC, "sequenceArray", "item 1", 0, XMPConst.ARRAY_IS_ORDERED);
        xmp.appendArrayItem(XMPConst.NS_DC, "sequenceArray", "item 2");
        xmp.appendArrayItem(XMPConst.NS_DC, "sequenceArray", "item 3");
        
        this.assert(xmp.doesArrayItemExist(XMPConst.NS_DC, "sequenceArray", 1));
        this.assert(xmp.doesArrayItemExist(XMPConst.NS_DC, "sequenceArray", XMPConst.ARRAY_LAST_ITEM));
    }
    
    
    /* Tests deleteArrayItem() */
    this.testDeleteArrayItem = function()
    {
        xmp = new XMPMeta();

        // implicitly create array
        xmp.appendArrayItem(XMPConst.NS_DC, "sequenceArray", "item 1", 0, XMPConst.ARRAY_IS_ORDERED);
        xmp.appendArrayItem(XMPConst.NS_DC, "sequenceArray", "item 2");
        xmp.appendArrayItem(XMPConst.NS_DC, "sequenceArray", "item 3");
        
        xmp.deleteArrayItem(XMPConst.NS_DC, "sequenceArray", 2);
        this.assertEquals(2, xmp.countArrayItems(XMPConst.NS_DC, "sequenceArray"));
    }
    

    /** Tests insertion in not created arrays */
    this.testErrorArrayCannotHaveValue = function()
    {
        try
        {
            xmp = new XMPMeta();
            xmp.setProperty(XMPConst.NS_DC, "sequenceArray", "not allowed value", XMPConst.ARRAY_IS_ORDERED);
            fail("Exception expected!");
        }
        catch (e)
        {
            this.assertEquals("Error: XMP Exception: Structs and arrays can't have string values", e.toString());
        }
    }

    
    /** Tests insertion in not created arrays */
    this.testErrorInsertWithoutArray = function()
    {
        try
        {
            xmp = new XMPMeta();
            xmp.insertArrayItem(XMPConst.NS_DC, "sequenceArray", 1, "item 0,5");
            fail("Exception expected!");
        }
        catch (e)
        {
            this.assertEquals("Error: XMP Exception: Specified array does not exist", e.toString());
        }
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