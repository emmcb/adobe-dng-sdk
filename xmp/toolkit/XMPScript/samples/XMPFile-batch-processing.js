// Use XMPFiles for batch processing
// =================================
//
// This example iterates through a folder of pictures (defined in the variable "pics") 
// and processes the metadata. For each picture these steps are carried out:
// 1. First the metadata is read and parsed (if there is no XMP existing in the file, 
//    the legacy metadata is automatically converted to XMP)
// 2. The list of existing authors/creators is deleted
// 3. A new creator is added
// 4. The metadata is written back into the file.

$.writeln("XMPFiles batch processing example");

// define folder containing photographs (make sure that you use copies)
var picFolder = "/c/temp/photos";

// load the XMPScript library
if (ExternalObject.AdobeXMPScript == undefined)
	ExternalObject.AdobeXMPScript = new	ExternalObject('lib:AdobeXMPScript');

// iterate through the photos in the folder
var pics = Folder(picFolder).getFiles();
for (f in pics) {
	var file = pics[f];
	$.writeln("process file: " + file.fsName);
	
	// applies only to files, not to folders
	if (file instanceof File) {
		var xmpFile = new XMPFile(file.fsName, XMPConst.UNKNOWN, XMPConst.OPEN_FOR_UPDATE);
		var xmp = xmpFile.getXMP();

		// delete existing authors and add a new one, existing metadata stays untouched
        	xmp.deleteProperty(XMPConst.NS_DC, "creator");
       		xmp.appendArrayItem(XMPConst.NS_DC, "creator", "Judy", 0, XMPConst.ARRAY_IS_ORDERED);

		// write updated metadata into the file
		if (xmpFile.canPutXMP(xmp))
		{
			xmpFile.putXMP(xmp);
		}
		xmpFile.closeFile(XMPConst.CLOSE_UPDATE_SAFELY);
	}
}