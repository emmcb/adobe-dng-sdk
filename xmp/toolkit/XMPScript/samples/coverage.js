// -----------------------------------------------------------------------------
// Coverage examples for XMPScript
// -----------------------------------------------------------------------------

// to run this script, run "Shell.exe" and type "load('coverage.js')"


var XMPScripting;
if (XMPScripting == undefined)
{
    XMPScripting = new ExternalObject("lib:../AdobeXMPScript");
}    

print("\n\n-----------------------------------------------------------------------------");
print("Step 1: load test file ");
print("-----------------------------------------------------------------------------");

file = new File("example.xmp");
file.encoding = "UTF-8";    
file.open();
xmpStr = file.read();
file.close();
print(xmpStr);


print("\n\n-----------------------------------------------------------------------------");
print("Step 2: parse the String into a XMPMeta-object and validate if its there");
print("-----------------------------------------------------------------------------");

xmp = new XMPMeta(xmpStr);
print(xmp.serialize());


print("\n\n-----------------------------------------------------------------------------");
print("Step 3: Find out, who is the creator of the document");
print("-----------------------------------------------------------------------------");

count = xmp.countArrayItems(XMPConst.NS_DC, "creator");
print(count);
creator = xmp.getProperty(XMPConst.NS_DC, "creator[1]");
print(creator);


print("\n\n-----------------------------------------------------------------------------");
print("Step 4: Correct the creator");
print("-----------------------------------------------------------------------------");

xmp.setProperty(XMPConst.NS_DC, "creator[1]", "Alan Lillich");
print(xmp.getProperty(XMPConst.NS_DC, "creator[1]"));


print("\n\n-----------------------------------------------------------------------------");
print("Step 5: Serialize and save file");
print("-----------------------------------------------------------------------------");

xmpStr = xmp.serialize();
out = new File("example updated.xmp");
out.encoding = "UTF-8";    
out.open("w");
out.write(xmpStr);
out.close();


print("\n\n-----------------------------------------------------------------------------");
print("Step 6: Create an XMP metadata object from scratch");
print("-----------------------------------------------------------------------------");

xmp = new XMPMeta();
xmp.setProperty(XMPConst.NS_XMP, "CreatorTool", "ExtendScript");
result = xmp.serialize(0, 1); // done!
print(result);


print("\n\n-----------------------------------------------------------------------------");
print("Step 7: Some other stuff");
print("-----------------------------------------------------------------------------");

print(XMPMeta.dumpNamespaces());
print(XMPMeta.dumpAliases());

print(XMPMeta.getNamespacePrefix("http://ns.adobe.com/pdf/1.3/"));
print(XMPMeta.getNamespaceURI("pdf"));

alias = XMPMeta.resolveAlias(XMPConst.NS_XMP, "Author");
print(alias.namespace);
print(alias.name);      // equal to print(alias)
print(alias.arrayForm);


print("\n\n-----------------------------------------------------------------------------");
print("Step 8: Exceptions");
print("-----------------------------------------------------------------------------");

try {
    new XMPMeta("Not well-formed XML!");  // forwarding of XMP Exceptions
} catch (e) {
    print("EXPECTED EXCEPTION: " + e.message);
}
   

print("\n\n-----------------------------------------------------------------------------");
print("Step 9: Localized Text");
print("-----------------------------------------------------------------------------");

xmp = new XMPMeta();
xmp.setLocalizedText(XMPConst.NS_DC, "desc", null, "de-de", "Beschreibung Deutschland");
xmp.setLocalizedText(XMPConst.NS_DC, "desc", null, "de-CH", "Beschriftung Schweiz");
xmp.setLocalizedText(XMPConst.NS_DC, "desc", null, "de", "Beschriftung Deutsch");

// retrieve local
loc = xmp.getLocalizedText(XMPConst.NS_DC,"desc", null, "de-CH");
print("Localized property value: " + loc.value);
print("Localized property locale: " + loc.locale);
print("Localized property options: " + loc.options);

// fallback to language
loc = xmp.getLocalizedText(XMPConst.NS_DC,"desc", null, "de");
print("Localized property value: " + loc.value);
print("Localized property locale: " + loc.locale);
print("Localized property options: " + loc.options);


print("\n\n-----------------------------------------------------------------------------");
print("Step 10: Array handling");
print("-----------------------------------------------------------------------------");

xmp = new XMPMeta();
xmp.setProperty(XMPConst.NS_DC, "bagArray", undefined, XMPConst.PROP_IS_ARRAY);
// xmp.setProperty(XMPConst.NS_DC, "sequenceArray", undefined, XMPConst.ARRAY_IS_ORDERED);
xmp.setProperty(XMPConst.NS_DC, "alternativeArray", undefined, XMPConst.ARRAY_IS_ALTERNATIVE);

xmp.appendArrayItem(XMPConst.NS_DC, "sequenceArray", "item 1,0", 0, XMPConst.ARRAY_IS_ORDERED);
xmp.appendArrayItem(XMPConst.NS_DC, "sequenceArray", "item 2,0");
xmp.appendArrayItem(XMPConst.NS_DC, "sequenceArray", "item 3,0");

xmp.insertArrayItem(XMPConst.NS_DC, "sequenceArray", 1, "item 0,5");
xmp.insertArrayItem(XMPConst.NS_DC, "sequenceArray", 3, "item 1,5");
xmp.insertArrayItem(XMPConst.NS_DC, "sequenceArray", 5, "item 2,5");
xmp.insertArrayItem(XMPConst.NS_DC, "sequenceArray", 7, "item 3,5");

print ("Last item: " + xmp.getArrayItem(XMPConst.NS_DC, "sequenceArray", XMPConst.ARRAY_LAST_ITEM));

print(xmp.serialize(0,1));


print("\n\n-----------------------------------------------------------------------------");
print("Step 11: Struct handling");
print("-----------------------------------------------------------------------------");

xmp = new XMPMeta();
xmp.setProperty(XMPConst.NS_DC, "struct", "", XMPConst.PROP_IS_STRUCT);
xmp.setStructField(XMPConst.NS_DC, "struct", XMPConst.TYPE_TEXT, "field1", 10);
xmp.setStructField(XMPConst.NS_DC, "struct", XMPConst.TYPE_TEXT, "field2", "Stefan");

print(xmp.getStructField(XMPConst.NS_DC, "struct", XMPConst.TYPE_TEXT, "field1"));
print(xmp.getStructField(XMPConst.NS_DC, "struct", XMPConst.TYPE_TEXT, "field2"));


print("\n\n-----------------------------------------------------------------------------");
print("Step 12: Qualifier handling");
print("-----------------------------------------------------------------------------");

xmp = new XMPMeta();
xmp.setProperty(XMPConst.NS_DC, "simple", "simpleValue");
xmp.setQualifier(XMPConst.NS_DC, "simple", XMPConst.TYPE_TEXT, "qual1", 10);
xmp.setQualifier(XMPConst.NS_DC, "simple", XMPConst.TYPE_TEXT, "qual2", "Stefan");

print(xmp.getQualifier(XMPConst.NS_DC, "simple", XMPConst.TYPE_TEXT, "qual1"));
print(xmp.getQualifier(XMPConst.NS_DC, "simple", XMPConst.TYPE_TEXT, "qual2"));


print("\n\n-----------------------------------------------------------------------------");
print("Step 13: Checking for property existance");
print("-----------------------------------------------------------------------------");

xmp = new XMPMeta();

xmp.setProperty(XMPConst.NS_DC, "sequenceArray", undefined, XMPConst.ARRAY_IS_ORDERED);
xmp.appendArrayItem(XMPConst.NS_DC, "sequenceArray", "item 1,0");
xmp.appendArrayItem(XMPConst.NS_DC, "sequenceArray", "item 2,0");
xmp.appendArrayItem(XMPConst.NS_DC, "sequenceArray", "item 3,0");

xmp.setProperty(XMPConst.NS_DC, "struct", "", XMPConst.PROP_IS_STRUCT);
xmp.setStructField(XMPConst.NS_DC, "struct", XMPConst.TYPE_TEXT, "field1", 10);
xmp.setStructField(XMPConst.NS_DC, "struct", XMPConst.TYPE_TEXT, "field2", "Stefan");

xmp.setProperty(XMPConst.NS_DC, "simple", "simpleValue");
xmp.setQualifier(XMPConst.NS_DC, "simple", XMPConst.TYPE_TEXT, "qual1", 10);
xmp.setQualifier(XMPConst.NS_DC, "simple", XMPConst.TYPE_TEXT, "qual2", "Stefan");

print("property exists: " + 
    xmp.doesPropertyExist(XMPConst.NS_DC, "simple"));
print("property NOT exists: " + 
    xmp.doesPropertyExist(XMPConst.NS_DC, "complicated"));
print("array item exists: " + 
    xmp.doesArrayItemExist(XMPConst.NS_DC, "sequenceArray", 2));
print("array item NOT exists: " + 
    xmp.doesArrayItemExist(XMPConst.NS_DC, "sequenceArray", 1234));
print("struct field exists: " + 
    xmp.doesStructFieldExist(XMPConst.NS_DC, "struct", XMPConst.TYPE_TEXT, "field2"));
print("struct field NOT exists: " + 
    xmp.doesStructFieldExist(XMPConst.NS_DC, "struct", XMPConst.TYPE_TEXT, "notexisting"));
print("qualifier exists: " + 
    xmp.doesQualifierExist(XMPConst.NS_DC, "simple", XMPConst.TYPE_TEXT, "qual1"));
print("qualifier NOT exists: " + 
    xmp.doesQualifierExist(XMPConst.NS_DC, "simple", XMPConst.TYPE_TEXT, "notexisting"));


print("\n\n-----------------------------------------------------------------------------");
print("Step 14: Deleting properties, fields and qualifiers");
print("-----------------------------------------------------------------------------");

xmp.deleteArrayItem(XMPConst.NS_DC, "sequenceArray", 2);
xmp.deleteProperty(XMPConst.NS_DC, "sequenceArray"); // deltes the complete array

xmp.deleteStructField(XMPConst.NS_DC, "struct", XMPConst.TYPE_TEXT, "field1");
xmp.deleteProperty(XMPConst.NS_DC, "struct");

xmp.deleteQualifier(XMPConst.NS_DC, "simple", XMPConst.TYPE_TEXT, "qual2");
xmp.deleteProperty(XMPConst.NS_DC, "simple");
print(xmp.serialize(0,1));


print("\n\n-----------------------------------------------------------------------------");
print("Step 15: Specialized getter and setter-methods");
print("-----------------------------------------------------------------------------");

xmp = new XMPMeta();

// type boolean
xmp.setProperty(XMPConst.NS_DC, "boolTrue", true);
xmp.setProperty(XMPConst.NS_DC, "boolTrue2", true, 0, "boolean");
xmp.setProperty(XMPConst.NS_DC, "boolFalse", "false");
xmp.setProperty(XMPConst.NS_DC, "boolFalse2", false, 0, "boolean");
print("BoolTrue: " + xmp.getProperty(XMPConst.NS_DC, "boolTrue").value);
print("BoolTrue2 (forced): " + xmp.getProperty(XMPConst.NS_DC, "boolTrue2", "boolean").value);
print("BoolFalse: " + xmp.getProperty(XMPConst.NS_DC, "boolFalse").value);
print("BoolFalse2 (forced): " + xmp.getProperty(XMPConst.NS_DC, "boolFalse2", "boolean").value);

// type integer
xmp.setProperty(XMPConst.NS_DC, "int", 4711);
xmp.setProperty(XMPConst.NS_DC, "int2", 4711, 0, "integer");
xmp.setProperty(XMPConst.NS_DC, "int3", 4711.9998, 0, "integer");
print("Integer:  " + xmp.getProperty(XMPConst.NS_DC, "int").value);
print("Integer2: " + xmp.getProperty(XMPConst.NS_DC, "int2").value);
print("Integer3: " + xmp.getProperty(XMPConst.NS_DC, "int3").value);
print("Integer (forced):  " + xmp.getProperty(XMPConst.NS_DC, "int", "integer").value);

// type number
xmp.setProperty(XMPConst.NS_DC, "numberStr", 123);
xmp.setProperty(XMPConst.NS_DC, "numberDecimalStr", 123.456);
xmp.setProperty(XMPConst.NS_DC, "numberForced", 123, 0, "number");
xmp.setProperty(XMPConst.NS_DC, "numberDecimalForced", 123.456, 0, "number");
print("numberStr:  " + xmp.getProperty(XMPConst.NS_DC, "numberStr").value);
print("numberDecimalStr:  " + xmp.getProperty(XMPConst.NS_DC, "numberDecimalStr").value);
print("numberForced: " + xmp.getProperty(XMPConst.NS_DC, "numberForced").value);
print("NumberDecimalForced: " + xmp.getProperty(XMPConst.NS_DC, "numberDecimalForced").value);
print("numberStr (forced get):  " + xmp.getProperty(XMPConst.NS_DC, "numberStr", "number").value);
print("numberDecimalStr (forced get):  " + xmp.getProperty(XMPConst.NS_DC, "numberDecimalStr", "number").value);
print("numberForced (forced get): " + xmp.getProperty(XMPConst.NS_DC, "numberForced", "number").value);
print("NumberDecimalForced (forced get): " + xmp.getProperty(XMPConst.NS_DC, "numberDecimalForced", "number").value);
// take only the integer part of the number
try {
        // get a number as integer
        xmp.getProperty(XMPConst.NS_DC, "numberDecimalStr", "integer");
        throw "FAIL()!";
} catch (e) {
    print("EXPECTED EXCEPTION: " + e.message);
}

// type xmpdate
dt = new XMPDateTime(new Date()); // current date
xmp.setProperty(XMPConst.NS_DC, "dateStr", dt);
xmp.setProperty(XMPConst.NS_DC, "dateForced", dt, 0, "xmpdate");
try
{
    xmp.setProperty(XMPConst.NS_DC, "dateForced", dt.toString(), 0, "xmpdate");
}
catch (e)
{
    print ("EXPECTED EXCEPTION: " + e);
}   
print("dateStr: " + xmp.getProperty(XMPConst.NS_DC, "dateStr"));
print("dateForced: " + xmp.getProperty(XMPConst.NS_DC, "dateForced"));
print("dateStr (forced get): " + xmp.getProperty(XMPConst.NS_DC, "dateStr", "xmpdate"));
print("dateForced (forced get): " + xmp.getProperty(XMPConst.NS_DC, "dateForced", "xmpdate"));


print("\n\n-----------------------------------------------------------------------------");
print("Step 16: Iteration");
print("-----------------------------------------------------------------------------");

xmp = new XMPMeta(xmpStr);
it = xmp.iterator();
prop = 1;
while (prop != null)
{
    prop = it.next();
    if (prop != null)
        print(prop.propPath + " --> " + prop.value);
}    


print("\n\n-----------------------------------------------------------------------------");
print("Step 17: XMPDateTime");
print("-----------------------------------------------------------------------------");

print( "Zero time: " + new XMPDateTime() );
print( "Current Date:                 " + new XMPDateTime(new Date()) );
print( "Current Date back to JS Date: " + new XMPDateTime(new Date()).getDate() );
print( "Initialize XMPDateTime with ISO8601 ('0'): " + new XMPDateTime('0') );
print( "Initialize XMPDateTime with ISO8601: " + new XMPDateTime("2000-01-02T03:04:05.0678+09:10") );

// Timezone methods
dateTime = new XMPDateTime(new Date());
print( "\nCurrent Date:               " + dateTime );
dateTime.convertToUTCTime();
print( "--> convertToUTCTime():       " + dateTime );
dateTime.convertToLocalTime();
print( "--> convertToLocalTime():     " + dateTime );
dateTime = new XMPDateTime("0000");
print( "--> 0000-Date:                " + dateTime );
dateTime.setLocalTimeZone();
print( "--> setLocalTimeZone():       " + dateTime );


print("\n\n-----------------------------------------------------------------------------");
print("Step 18: XMPUtils");
print("-----------------------------------------------------------------------------");

// Path composing

var myPath =
    XMPUtils.composeArrayItemPath(XMPConst.NS_DC, "seq", 10) + "/" +
    XMPUtils.composeStructFieldPath(XMPConst.NS_PDF, "struct", XMPConst.TYPE_TEXT, "structItem") + "/" +
    XMPUtils.composeLangSelector(XMPConst.NS_PHOTOSHOP, "langSelector", "de-CH") + "/" +
    XMPUtils.composeFieldSelector(XMPConst.NS_EXIF, "fieldSelector", XMPConst.TYPE_FONT, "Fontname", "Arial") + "/" +
    XMPUtils.composeQualifierPath(XMPConst.NS_TIFF, "qualifiedProp", XMPConst.TYPE_GRAPHICS, "qualifier");    
    
print(myPath);    

// Catenate and SeparateArrays

xmp = new XMPMeta();
xmp.setProperty( XMPConst.NS_XMP, "Array1", undefined, XMPConst.PROP_IS_ARRAY );
xmp.appendArrayItem ( XMPConst.NS_XMP, "Array1", "one" );
xmp.appendArrayItem ( XMPConst.NS_XMP, "Array1", "two" );
xmp.appendArrayItem ( XMPConst.NS_XMP, "Array1", "3, four" );
xmp.appendArrayItem ( XMPConst.NS_XMP, "Array1", "five; 6" );

str = XMPUtils.catenateArrayItems(xmp, XMPConst.NS_XMP, "Array1", "; ", "\"", undefined);
print(str);
XMPUtils.separateArrayItems ( xmp, XMPConst.NS_XMP, "Array2-1", null, str);
XMPUtils.separateArrayItems ( xmp, XMPConst.NS_XMP, "Array2-2", XMPConst.SEPARATE_ALLOW_COMMAS, str);
XMPUtils.separateArrayItems ( xmp, XMPConst.NS_XMP, "Array3-1", XMPConst.ARRAY_IS_ORDERED, str);
XMPUtils.separateArrayItems ( xmp, XMPConst.NS_XMP, "Array3-2", XMPConst.ARRAY_IS_ORDERED | XMPConst.SEPARATE_ALLOW_COMMAS, str);

print(xmp.serialize(0,1));


print("\n\n-----------------------------------------------------------------------------");
print("Step 19: XMPFiles");
print("-----------------------------------------------------------------------------");

print(XMPFile.version);
print("File format info flags for PSD: " + XMPFile.getFormatInfo(XMPConst.FILE_PHOTOSHOP));

file = new XMPFile("Purple Square.psd", XMPConst.PSD, XMPConst.OPEN_USE_SMART_HANDLER | XMPConst.OPEN_FOR_UPDATE);
fileInfo = file.getFileInfo();
print("FileInfo#path: " + fileInfo.filePath);
print("FileInfo#openFlags: " + fileInfo.openFlags);
print("FileInfo#format: " + fileInfo.format);
print("FileInfo#handlerFags: " + fileInfo.handlerFlags);

packetInfo = file.getPacketInfo();
print("PacketInfo#packet: " + packetInfo.packet);
print("PacketInfo#offset: " + packetInfo.offset);
print("PacketInfo#length: " + packetInfo.length);
print("PacketInfo#padSize: " + packetInfo.padSize);
print("PacketInfo#charForm: " + packetInfo.charForm);
print("PacketInfo#writeable: " + packetInfo.writeable);

xmp = file.getXMP();

// change modification date
dateTime = new XMPDateTime(new Date()); // now
print("old mod date: " + xmp.getProperty(XMPConst.NS_XMP, "ModifyDate", "xmpdate").value);
xmp.setProperty(XMPConst.NS_XMP, "ModifyDate", dateTime, 0, "xmpdate");
print("new mod date: " + xmp.getProperty(XMPConst.NS_XMP, "ModifyDate", "xmpdate").value);
xmp.setProperty(XMPConst.NS_XMP, "Author", "Llywelyn");

if (file.canPutXMP(xmp))
{
    print("--> XMP can be updated.");
    file.putXMP(xmp);
}

file.closeFile(XMPConst.CLOSE_UPDATE_SAFELY);
print("--> File modified and closed.");


