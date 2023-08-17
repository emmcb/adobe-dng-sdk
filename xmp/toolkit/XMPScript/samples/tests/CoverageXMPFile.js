var XMPScripting;

function CoverageXMPFile()
{
    /* Tests the format info function */
    this.testFormatInfo = function() 
    {
        // Please keep the comments to make it easier to write more tests.  
        this.assertEquals(
            XMPConst.HANDLER_CAN_INJECT_XMP |
            XMPConst.HANDLER_CAN_EXPAND |
            XMPConst.HANDLER_CAN_REWRITE |
            XMPConst.HANDLER_PPEFERS_IN_PLACE |
            XMPConst.HANDLER_CAN_RECONCILE |
            XMPConst.HANDLER_ALLOWS_ONLY_XMP |
            XMPConst.HANDLER_RETURNS_RAW_PACKETS |
//            XMPConst.HANDLER_RETURNS_TNAIL |
//            XMPConst.HANDLER_OWNS_FILE |
            XMPConst.HANDLER_ALLOWS_SAFE_UPDATE|
            XMPConst.HANDLER_CAN_NOTIFY_PROGRESS,
            
            XMPFile.getFormatInfo(XMPConst.FILE_PHOTOSHOP)
        );
    }
    
    
    /* Tests the file info function */
    this.testFileInfo = function()
    {
        file = new XMPFile("tests/CoverageXMPFile.psd", XMPConst.FILE_PHOTOSHOP, 
            XMPConst.OPEN_FOR_READ | XMPConst.OPEN_USE_SMART_HANDLER);
        fileInfo = file.getFileInfo();
        this.assertEquals("tests/CoverageXMPFile.psd", fileInfo.filePath);
        this.assertEquals(XMPConst.OPEN_USE_SMART_HANDLER | XMPConst.OPEN_FOR_READ, fileInfo.openFlags);
        this.assertEquals(XMPConst.FILE_PHOTOSHOP, fileInfo.format);
        this.assertEquals(
            XMPConst.HANDLER_CAN_INJECT_XMP |
            XMPConst.HANDLER_CAN_EXPAND |
            XMPConst.HANDLER_CAN_REWRITE |
            XMPConst.HANDLER_PPEFERS_IN_PLACE |
            XMPConst.HANDLER_CAN_RECONCILE |
            XMPConst.HANDLER_ALLOWS_ONLY_XMP |
            XMPConst.HANDLER_RETURNS_RAW_PACKETS |
//            XMPConst.HANDLER_RETURNS_TNAIL |
//            XMPConst.HANDLER_OWNS_FILE |
            XMPConst.HANDLER_ALLOWS_SAFE_UPDATE|
            XMPConst.HANDLER_CAN_NOTIFY_PROGRESS,
        
            fileInfo.handlerFlags
        );
        file.closeFile(XMPConst.CLOSE_UPDATE_SAFELY);
    }
    
    
    /* Test the packet info function */
    this.testPacketInfo = function()
    { 
        file = new XMPFile("tests/CoverageXMPFile.psd", XMPConst.FILE_PHOTOSHOP, 
            XMPConst.OPEN_FOR_READ | XMPConst.OPEN_USE_SMART_HANDLER);
        packetInfo = file.getPacketInfo();
        // don't compare packet at the moment just the size, TODO: why is there a difference?
        this.assertEquals(16941, packetInfo.packet.length);
        this.assertEquals(236, packetInfo.offset);
        this.assertEquals(16956, packetInfo.length);
        this.assertEquals(12049, packetInfo.padSize);
        this.assertEquals(0, packetInfo.charForm);
        this.assertEquals(true, packetInfo.writeable);
        
        file.closeFile(XMPConst.CLOSE_UPDATE_SAFELY);
    }


    /* Test updating a packet */
    this.testUpdatePacket = function()
    { 
        // 1. Copy file to temp
        f = new File("tests/CoverageXMPFile.psd");
        this.assert(f.copy("tests/CoverageXMPFile_temp.psd"));
    
    
        // 2. Update packet
        file = new XMPFile("tests/CoverageXMPFile_temp.psd", XMPConst.FILE_PHOTOSHOP, 
            XMPConst.OPEN_FOR_UPDATE | XMPConst.OPEN_USE_SMART_HANDLER);

        xmp = file.getXMP();

        // change some properties
        dateTime = new XMPDateTime(new Date()); // now
        xmp.setProperty(XMPConst.NS_XMP, "ModifyDate", dateTime, 0, "xmpdate");
        
		xmp.appendArrayItem(XMPConst.NS_DC, "creator", "木星", 0, XMPConst.ARRAY_IS_ORDERED);
		xmp.setProperty(XMPConst.NS_PHOTOSHOP, "City", "Hamburg");
		xmp.setProperty(XMPConst.NS_PHOTOSHOP, "CaptionWriter", "木星");
		xmp.setLocalizedText(XMPConst.NS_DC, "title", null, "zh-CN", "木星");
		xmp.setStructField(XMPConst.NS_IPTC_CORE, "CreatorContactInfo", XMPConst.NS_IPTC_CORE, "CiAdrCity", "木星");

        if (file.canPutXMP(xmp))
        {
            file.putXMP(xmp);
        }
        else
        {
            this.fail("Update should be possible!");
        }

        file.closeFile(XMPConst.CLOSE_UPDATE_SAFELY);
        
        
        // 3. Get updated packet
        file = new XMPFile("tests/CoverageXMPFile_temp.psd", XMPConst.FILE_PHOTOSHOP, 
            XMPConst.OPEN_FOR_READ | XMPConst.OPEN_USE_SMART_HANDLER);

        xmp = file.getXMP();
        file.closeFile();
        
		//check the properties
        this.assertEquals(2, xmp.countArrayItems(XMPConst.NS_DC, "creator"));
		this.assertEquals("木星", xmp.getArrayItem(XMPConst.NS_DC, "creator", 2));
		this.assertEquals("Hamburg", xmp.getProperty(XMPConst.NS_PHOTOSHOP, "City"));
		this.assertEquals("木星", xmp.getProperty(XMPConst.NS_PHOTOSHOP, "CaptionWriter"));
		this.assertEquals("木星", xmp.getLocalizedText(XMPConst.NS_DC,"title", null, "zh-CN"));
		this.assertEquals("木星", xmp.getStructField(XMPConst.NS_IPTC_CORE, "CreatorContactInfo", XMPConst.NS_IPTC_CORE, "CiAdrCity"));
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