// Initialize XMPScript
var XMPScripting;
initialize();


// tools: showCreator, showCameraType, setCreator, renameToModifyDate
var picFolder = "/c/temp/photos";
// folder of processed files, procedure, updateXMP, custom data
process(picFolder, renameToModifyDate);


function initialize()
{
    if (XMPScripting == undefined)
    {
        XMPScripting = new ExternalObject("lib:../libraries/windows/debug/XMPScript");
        print(XMPMeta.version);
    }    
}


function process(folder, procedure, updateXMP, data)
{
    updateXMP = updateXMP ? updateXMP : false;
    print(updateXMP);
    pics = Folder(picFolder).getFiles();
    for (file in pics)
    {
        print("process file: " + pics[file].fsName);
        file = pics[file];
        // check if its a file
        if (file instanceof File)
        {
            xmpFile = new XMPFile(file.fsName, XMPConst.UNKNOWN, updateXMP ? XMPConst.OPEN_FOR_UPDATE : XMPConst.OPEN_FOR_READ);
            xmp = xmpFile.getXMP();
            
            procedure(file, xmp, data);

            if (xmpFile.canPutXMP(xmp))
            {
                xmpFile.putXMP(xmp);
            }
            xmpFile.closeFile(XMPConst.CLOSE_UPDATE_SAFELY);
        }
    }
}


function showCreator(file, xmp)
{
    var author = "None specified";
    if (xmp.doesArrayItemExist(XMPConst.NS_DC, "creator", 1))
    {
        author = xmp.getArrayItem(XMPConst.NS_DC, "creator", 1);
    }
    else if (xmp.doesPropertyExist(XMPConst.NS_DC, "creator"))
    {
        author = xmp.getProperty(XMPConst.NS_DC, "creator");
    }
    print ("Author: ", author);
}


function showCameraType(file, xmp)
{
    var camera = xmp.getProperty(XMPConst.NS_TIFF, "Make") + " - " +
                 xmp.getProperty(XMPConst.NS_TIFF, "Model");
    if (camera == " - ")
        camera = "<None specified>";    
    print ("Camera Type: ", camera);
}


function setCreator(file, xmp, data)
{
    if (data)
    {
        xmp.deleteProperty(XMPConst.NS_DC, "creator");
        xmp.appendArrayItem(XMPConst.NS_DC, "creator", data, 0, XMPConst.ARRAY_IS_ORDERED);
    }
    else
    {
        print("Please provide a creator name!");
    }
}



function renameToModifyDate(file, xmp)
{
    dateTime = xmp.getDateProperty(XMPConst.NS_XMP, "ModifyDate").value;
    dateTime.convertToUTCTime();
    //fileName = dateTime.year + dateTime.month + dateTime.day + " " + dateTime.hour + dateTime.minute + dateTime.second;
    fileName = dateTime.toString();
    fileName = fileName.replace(":", "-");
    fileName = fileName.replace(":", "-");
    fileName = fileName.replace(":", "-");
    
    extension = "";
    index = file.fsName.lastIndexOf(".");
    if (index >= 0)
        extension = file.fsName.substring(index);
    if (fileName.length > 0  &&  fileName.search("^00|-") < 0)
    {                   
        file.rename(fileName + extension);  
    }
}