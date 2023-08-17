var XMPScripting;

function CoverageDateTime()
{
    /* Tests conversion between JS Date and XMPDateTime. */
    this.testDateTimeConversion = function()
    {
        jsDate = new Date();
        xmpDate = new XMPDateTime(jsDate);
        this.assertEquals(jsDate.toString(), new XMPDateTime(jsDate).getDate().toString());
    }


    /* Tests 0-Date */
    this.testNullDate = function()
    {
        this.assertEquals("0000", new XMPDateTime() );
        this.assertEquals("0000", new XMPDateTime('0'));
    }


    /* Tests initialisation with an IS08601-String */
    this.testInitWithISO8601 = function()
    {
        this.assertEquals("2000-01-02T03:04:05.0678+09:10", new XMPDateTime("2000-01-02T03:04:05.0678+09:10"));
    }
    
    
    /* Tests convertToUTCTime() */
    this.testConvertToUTCTime = function()
    {    
        xmpDate = new XMPDateTime("2000-01-02T03:04:05.0678+09:10");
        xmpDate.convertToUTCTime();
        this.assertEquals("2000-01-01T17:54:05.0678Z" , xmpDate);
    }    


    /* Tests convertToLocalTime() 
       TODO this test does not work in other time zones
    this.testConvertToLocalTime = function()
    {    
        xmpDate = new XMPDateTime("2000-01-02T03:04:05.0678+09:10");
        xmpDate.convertToLocalTime();
        this.assertEquals("2000-01-01T18:54:05.0678+01:00", xmpDate);
    }
    */
    
    
    /* Tests setLocalTimeZone() 
       TODO this test does not work in other time zones
    this.testSetLocalTimeZone = function()
    {
        xmpDate = new XMPDateTime("2000-01-01T18:54:05.0678");
        xmpDate.setLocalTimeZone();
        this.assertEquals("2000-01-01T18:54:05.0678+01:00", xmpDate);
    }
    */

    this.setUp = function ()
    {
         // Empty
    }


    this.tearDown = function()
    {
         // Empty
    }
}