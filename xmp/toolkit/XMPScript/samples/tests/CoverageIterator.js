var XMPScripting;

function CoverageIterator()
{
    /* Tests iteration of example file */
    this.testIterator = function()
    {
        // load example file
        file = new File("tests/CoverageIterator.xmp");
        file.encoding = "UTF-8";    
        file.open();
        xmpStr = file.read();
        file.close();
    
        xmp = new XMPMeta(xmpStr);
        it = xmp.iterator();
        result = "";
        prop = 1;
        while (prop != null)
        {
            prop = it.next();
            if (prop != null)
            {
                result += prop.namespace + " -- " + prop.path + " -- " + prop.options + " -- " + prop.value + "\n";
            }
        }
        
        this.compareWithFile(result, "tests/CoverageIterator.txt");
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