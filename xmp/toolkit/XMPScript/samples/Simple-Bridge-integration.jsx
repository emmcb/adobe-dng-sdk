// XMPScript Bridge Integration Example
// ====================================
//
// This script adds a context menu item to the Bridge Thumbnails that show some of the XMP Properties.
// To use this script, place it in the "Startup Scripts"-folder of Bridge.
// When you start Bridge, select a Thumbnail of a document that contains XMP, 
// press the right mouse button and select "Show XMP Properties". 

$.writeln("XMPScript Bridge Integration Example");

// load the XMPScript library
if (ExternalObject.AdobeXMPScript == undefined)
{
	ExternalObject.AdobeXMPScript = new	ExternalObject('lib:../AdobeXMPScript');
}

// add a context menu entry to Thumbnail
var xmpCommand = new MenuElement("command", "Show XMP Properties", "at the end of Thumbnail", "showProperties");

// the onselect event handler retrieves the metadata from the first selected item and
// displays some metadata properties.
xmpCommand.onSelect = function(m)
{
	try {
		thumb = app.document.selections[0];
		meta = thumb.metadata;
		if (meta)
		{
			xmp = new XMPMeta(meta.serialize());
			var msg = "Title: " + xmp.getLocalizedText(XMPConst.NS_DC, "title", null, "en") + "\n";
			msg += "Authors of the document:\n";
			no = xmp.countArrayItems(XMPConst.NS_DC, "creator");
			
			for (i = 1; i <= no; i++)
			{
				msg += "*   " + xmp.getArrayItem(XMPConst.NS_DC, "creator", i) + "\n";
			}
		
			msg += "Creation Date: " + xmp.getProperty(XMPConst.NS_XMP, "CreateDate")
			Window.alert(msg);
		}	
		else
			Window.alert("No XMP contained");
	} 
	catch(error)
	{
		Window.alert(error); 
	}
};
   
// the onDisplay event handler switches the menu item on and off depending on the selection
xmpCommand.onDisplay = function(m)
{
	try
	{
		// TODO check if all selected assets are files.
		var tn = app.document.selections[0]; //check the first selected node
		
		if (tn  &&  !tn.container)
			m.enabled = true; // yes, enable the command
		else
			m.enabled = false; // no, disable the command
	}
	catch(error)
	{
		Window.alert(error);
	}
};