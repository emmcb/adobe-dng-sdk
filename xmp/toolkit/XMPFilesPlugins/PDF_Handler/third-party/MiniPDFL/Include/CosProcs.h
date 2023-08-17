/*************************************************************************
* WARNING: All additions to this file must go at the end of the file.
* This file is #included to build a table of function pointers (an HFT).
* All components which make use of this HFT rely on the order and
* positioning of these functions (e.g. plug-ins). Adding entries in the 
* middle of this file or modifying existing entries will cause these 
* components to break.
**************************************************************************/

/*********************************************************************

 ADOBE SYSTEMS INCORPORATED
 Copyright (C) 1994-2006 Adobe Systems Incorporated
 All rights reserved.

 NOTICE: Adobe permits you to use, modify, and distribute this file
 in accordance with the terms of the Adobe license agreement
 accompanying it. If you have received this file from a source other
 than Adobe, then your use, modification, or distribution of it
 requires the prior written permission of Adobe.

 ---------------------------------------------------------------------

 CosProcs.h

 - Catalog of functions exported by Cos.

*********************************************************************/

/**
	Tests whether two Cos objects are equal. Cos objects are 
	equal when all of the following conditions are true: 
	<ul>
	<li> They are either both direct or both indirect. </li>
	<li> They have the same type. </li>
	<li> If they are indirect, they have the same generation number. </li>
	<li> If they are scalars, they have the same value. (Two <code>NULL</code> 
	objects are equal.) </li>
	<li> If they are non-scalar, they reference the same value. </li>
	</ul>

	<p>The last condition implies that the comparison is <i>shallow</i>. 
	For example: </p>

	<p><code>CosObj a, b, c; a = CosNewString (doc, "XYZ"); b = CosNewString(doc, "XYZ"); c = b; </code></p>

	<p>In this case, <code>CosObjEqual(a,b)</code> is <code>false</code>, but <code>CosObjEqual(b,c)</code> 
	is <code>true</code>. </p>
	@param obj1 An object to compare with <code>obj2</code>.
	@param obj2 An object to compare with <code>obj1</code>.
	@return <code>true</code> if <code>obj1</code> and <code>obj2</code> are equal, <code>false</code> otherwise. 
	@see CosObjCmp 
	@since PI_COS_VERSION >= 0x00020000
*/
NPROC(ASBool, CosObjEqual, (CosObj obj1, CosObj obj2))

/**
	Gets an object's type. 
	@param obj The object whose type is obtained.
	@return The object's type. 
	@since PI_COS_VERSION >= 0x00020000
*/
NPROC(CosType, CosObjGetType, (CosObj obj))

/**
	Tests whether an object is indirect. 
	@param obj The object to test.
	@return <code>true</code> if <code>obj</code> is indirect, <code>false</code> if <code>obj</code> is direct. 
	@since PI_COS_VERSION >= 0x00040000
*/
NPROC(ASBool, CosObjIsIndirect, (CosObj obj))

/**
	Enumerates the elements of a Cos object by calling a user-supplied 
	procedure for each component of the object. 
	@param obj The object whose elements are enumerated.
	<ul>
	<li> For scalars or strings, the <code>proc</code> is not called, and CosObjEnum()
	returns <code>true</code>.</li>
	<li> For dictionaries, <code>proc</code> is called for each key-value pair. The order in 
	which the key-value pairs are enumerated is undefined.</li>
	<li> For arrays, <code>proc</code> is called with each element as the first paramater to <code>proc</code>,
	and the <code>NULL</code> object as the second parameter. Array elements 
	are enumerated in ascending order of index.</li>
	<li> For streams, <code>proc</code> is called once,
	with the stream's dictionary as the first parameter to the
	<code>proc</code> and the <code>NULL</code> object as the second parameter.</li>
	</ul>	
	@param proc A user-supplied callback to call for each element 
	of <code>obj</code>.
	Neither <code>proc</code> nor any routine called by <code>proc</code> may modify 
	<code>obj</code>. Doing so can produce undefined results or errors. For 
	example, if <code>obj</code> is an array, <code>proc</code> must not call CosArrayRemove(); 
	if <code>obj</code> is a dictionary, <code>proc</code> must not call CosDictPut(). 
	@param clientData A pointer to user-supplied data to pass 
	to <code>proc</code> each time it is called.
	@return Returns <code>true</code> if every call to <code>proc</code> returned <code>true</code>.
	As soon as any call to <code>proc</code> returns <code>false</code>, the enumeration stops
	and CosObjEnum() returns <code>false</code>.
	@see CosArrayGet 
	@see CosDictGet 
	@see CosDocEnumEOFs 
	@see CosDocEnumIndirect
	@ingroup Enumerators
	@since PI_COS_VERSION >= 0x00020000
*/
NPROC(ASBool, CosObjEnum, (CosObj obj, CosObjEnumProc proc, void *clientData))

/**
	Gets the CosDoc containing the specified object. This is 
	defined only for indirect or non-scalar objects. 
	@param obj The object whose CosDoc is obtained.
	@return The object's CosDoc. 
	@exception cosErrInvalidObj is raised if the object is a direct scalar 
	object. 
	@see PDDocGetCosDoc 
	@since PI_COS_VERSION >= 0x00020000
*/
NPROC(CosDoc, CosObjGetDoc, (CosObj obj))

/**
	<p>Returns a direct object of type CosNull. This <code>NULL</code> object 
	is said to be invalid. You can compare an object to <code>NULL</code> 
	using either of the following methods (the second is more
	efficient):</p>

	<p><code>CosObjEqual(obj, CosNewNull()); </code></p>
	<p><code>CosObjGetType(obj) == CosNull; </code></p>

	<p>In general, use CosNewNull() only to initialize a local variable 
	or pass a parameter. <code>NULL</code> objects may be stored as array 
	elements, but not as dictionary values. The following statements 
	are equivalent: </p>

	<p><code>CosDictPut(dict, key, CosNewNull());  </code></p>
	<p><code>CosDictRemove(dict, key);  </code></p>

	@return A <code>NULL</code> Cos object. 
	@see CosObjGetType 
	@since PI_COS_VERSION >= 0x00020000
*/
NPROC(CosObj, CosNewNull, (void))

/**
        Creates a new 32-bit integer object associated with the specified 
        document and having the specified value. 
        @param dP IN The document in which the integer is used. 
        @param indirect IN If <code>true</code>, it creates the integer object as 
        an indirect object, and sets the document <code>dP</code> object's <code>PDDocNeedsSave</code> 
        flag (see PDDocFlags). If <code>false</code>, it creates the integer as 
        a direct object. 
        @param value IN The value, represented as a 32-bit integer.
        @return An object of type CosInteger.
        @see CosIntegerValue 
        @see CosNewFixed 
		@see CosNewFloat
        @see CosObjDestroy 
        @since PI_COS_VERSION >= 0x00020000
*/
NPROC(CosObj, CosNewInteger, (CosDoc dP, ASBool indirect, ASInt32 value))

/**
	Creates a new real-number object from a fixed-point number 
	associated with the specified document. 
	@param dP The document in which the number is used. 
	
	@param indirect If <code>true</code>, it creates the real-number object 
	as an indirect object, and sets the document (<code>dP</code>) object's <code>PDDocNeedsSave</code> 
	flag (see <code>PDDocFlags</code>). If <code>false</code>, it creates the number 
	as a direct object. 
	@param value The real number, represented as a fixed-point number.
	@return A Cos object of type <code>CosReal</code> (<code>CosFixed</code>).
	@see CosFixedValue 
	@see CosNewInteger
	@see CosNewFloat
	@see CosObjDestroy 
	@since PI_COS_VERSION >= 0x00020000
*/
#if ADD_ASFIXED //PKG
NPROC(CosObj, CosNewFixed, (CosDoc dP, ASBool indirect, ASFixed value))
#endif //ADD_ASFIXED //PKG

/**
    Creates a new boolean object associated with the specified 
    document and having the specified value. 
    @param dP IN The document in which the boolean is used. 
    @param indirect IN If <code>true</code>, it creates the boolean object as 
    an indirect object, and sets the document (<code>dP</code>) object's <code>PDDocNeedsSave</code> 
    flag (see <code>PDDocFlags</code>). If <code>false</code>, it creates the boolean object as 
    a direct object. 
    @param value IN The value the new boolean object will have. 
    @return A Cos boolean object. 
    @see CosBooleanValue 
    @see CosObjDestroy 
    @since PI_COS_VERSION >= 0x00020000
*/
NPROC(CosObj, CosNewBoolean, (CosDoc dP, ASBool indirect, ASBool value))


/**
	Creates a new name object associated with the specified 
	document and having the specified value. 
	@param dP The document in which the new name is used. 	
	@param indirect If <code>true</code>, it creates the name as an indirect 
	object, and sets the document's <code>PDDocNeedsSave</code> flag (see 
	<code>PDDocFlags</code>) flag. If <code>false</code>, it creates the name as a direct 
	object. 
	@param name The <code>ASAtom</code> corresponding to the name to create. 
	A C string can be converted to an <code>ASAtom</code> using <code>ASAtomFromString()</code>.
	Note that a name object can be created directly from a C string,
	without creating an <code>ASAtom</code>, by using <code>CosNewNameFromString()</code>.
	@return The newly created name Cos object. 
	@see CosNameValue
	@see CosNewNameFromString
	@see CosCopyNameStringValue
	@see CosObjDestroy 
	@since PI_COS_VERSION >= 0x00020000
*/
NPROC(CosObj, CosNewName, (CosDoc dP, ASBool indirect, ASAtom name))


/**
	Creates and returns a new Cos string object. 
	@param dP The document in which the string is used. 
	@param indirect If <code>true</code>, it creates the string as an indirect 
	object, and sets the document (<code>dP</code>) object's <code>PDDocNeedsSave</code> flag (see 
	<code>PDDocFlags</code>). If <code>false</code>, it creates the string as a direct object. 
	
	@param str The value that the new string will have. It 
	is not a C string, since Cos strings can contain <code>NULL</code> characters. 
	The data in <code>str</code> is copied; that is, if <code>str</code> was dynamically 
	allocated, it can be freed after this call. 
	@param nBytes The length of <code>str</code>.
	@return The newly created string Cos object. 
	@see CosStringValue 
	@see CosObjDestroy 
	@since PI_COS_VERSION >= 0x00020000
*/
NPROC(CosObj, CosNewString, (CosDoc dP, ASBool indirect, const char *str, ASTArraySize nBytes))


/**
	Creates and returns a new array Cos object. 
	@param dP The document in which the array is used. 
	@param indirect If <code>true</code>, it creates the array as an indirect 
	Cos object, and sets the document's <code>PDDocNeedsSave</code> flag 
	(see <code>PDDocSetFlags</code>). If <code>false</code>, it creates the array as a direct 
	object. 
	@param nElements The number of elements that will be in 
	the array. <code>nElements</code> is only a hint; Cos arrays grow dynamically 
	as needed.
	@return The newly created array Cos object. 
	@see CosObjDestroy 
	@see CosArrayGet 
	@see CosArrayInsert 
	@see CosArrayLength 
	@see CosArrayPut 
	@see CosArrayRemove 
	@since PI_COS_VERSION >= 0x00020000
*/
NPROC(CosObj, CosNewArray, (CosDoc dP, ASBool indirect, ASTArraySize nElements))


/**
	<p>Creates a new dictionary. </p>

	<p>See the <i>PDF Reference</i> for information on dictionary objects 
	that are in standard PDF files, such as annotations or page 
	objects. </p>
	@param dP The document in which the dictionary is used. 
	
	@param indirect If <code>true</code>, it creates the dictionary as an 
	indirect Cos object, and sets the <code>dP</code> object's <code>PDDocNeedsSave</code> flag (see 
	<code>PDDocFlags</code>). If <code>false</code>, it creates the dictionary as a direct 
	object. 
	@param nEntries The number of entries in the dictionary. This 
	value is only a hint; Cos dictionaries grow dynamically as 
	needed.
	@return The newly created dictionary Cos object. 
	@see CosDictGet 
	@see CosDictKnown 
	@see CosDictPut 
	@see CosDictRemove 
	@see CosObjDestroy 
	@since PI_COS_VERSION >= 0x00020000
*/
NPROC(CosObj, CosNewDict, (CosDoc dP, ASBool indirect, ASTArraySize nEntries))


/**
	<p>Creates a new Cos stream, using data from an existing <code>ASStm</code>. 
	The data is copied, so the source stream may be closed after 
	CosNewStream returns.</p> 

	<p>This method creates a Cos stream object by writing its PDF 
	representation to an intermediate file, in the format specified 
	in the <i>PDF Reference</i>:</p>

	<code>
	<p>&lt;&lt;/Length ... /Filter ... /DecodeParms ...&gt;&gt;</p>
	<p>stream</p>
	<p>... data, possibly encoded ...</p>
	<p>endstream</p>
	</code>

	<p>This occurs in four steps: </p>

	<p><b>Step 1: Writing the attribute dictionary </b></p>

	<p>If <code>attributesDict</code> is a valid Cos dictionary, the method 
	writes that dictionary to the intermediate file. Otherwise, 
	it creates a new direct dictionary, determining a <code>Length</code> 
	key according to the <code>sourceLength</code> value:</p>
	<ul>
	<li> If <code>sourceLength</code> is negative, or if the source data is to
	be encoded (see below), the value of the <code>Length</code> key is a reference to a 
	new indirect object, whose value will be set in <b>Step 4</b>. </li>
	<li> Otherwise, <code>Length</code> is a direct scalar representing <code>sourceLength</code>. </li>
	</ul>

	<p>The dictionary that is written becomes the new stream's 
	attribute dictionary. </p>

	<p><b>Step 2: Reading the data </b></p>

	<p><code>sourceStart</code> determines where in the source stream to begin 
	reading, and whether the source is seekable.</p>
	
	<ul>
	<li> If <code>sourceStart</code> is a negative number, the source is assumed to be non-seekable 
	but positioned at the point where reading should start. </li>
	<li> Otherwise, the source is assumed to be seekable, and reading 
	starts at the position indicated by <code>sourceStart</code>. If <code>sourceStart</code> 
	is zero, data is read from the beginning of the source stream. 
	Positive values for <code>sourceStart</code> may be used, for instance, 
	to skip over initial data in the stream. </li>
	</ul>

	<p><b>Step 3: Encoding the data </b></p>
	
	<p>If <code>attributesDict</code> is a valid Cos dictionary,  
	it contains a <code>Filter</code> key, and <code>encodeTheSourceData</code> is <code>true</code>, 
	the method encodes the data after reading it from the source 
	stream and before writing it to the intermediate file. </p>

	<p>The <code>attributesDict</code> is used as the new stream's dictionary. 
	The <code>Filter</code> entry in this dictionary indicates how the data 
	in the resulting Cos stream object will be subsequently 
	decoded; the value may be the name of a decoding filter 
	or an array of such names. Specify multiple filters in the 
	order they should be applied to decode the data (if parameters 
	are needed to decode the data, they are specified as the 
	value of the <code>DecodeParms</code> key in <code>attributesDict</code>. See the 
	<i>PDF Reference</i> for details). For each decoding filter, there 
	is a corresponding encoding filter, which the method applies 
	to the source data during this step. </p>

	<p>If parameters are needed to encode the data, they must be 
	specified in the call by <code>encodeParms</code> (the encoding 
	parameters are often different from the decoding parameters). 
	The <code>encodeParms</code> parameter is optional for all encoding filters 
	except <code>DCTDecode</code> and <code>JBIG2Decode</code>. See the <code>encodeParms</code> field 
	of <code>PDEFilterSpec</code>. </p>

	<p>If an array of filters is supplied, and at least one of 
	them requires encoding parameters, then a corresponding 
	array of encoding parameters is also required. Use the <code>NULL</code> 
	object to represent default parameters for filters that 
	have defaults.</p>
	
	<p>In any other case, the method copies the 
	source data directly into the Cos stream with no encoding. 
	If <code>sourceLength</code> is negative, it reads bytes until the source 
	reaches its EOF. Otherwise, <code>sourceLength</code> indicates how many 
	bytes to read from the source, and an exception is raised 
	if the source reaches EOF before that. </p>

	<p><b>Step 4: Writing the data</b></p>

	<p>After the data is written, if the value of the <code>Length</code> key 
	in the attributes dictionary was an indirect reference (either 
	because it was supplied that way in <code>attributesDict</code>, or because 
	it was created that way in <b>Step 1</b>, the value of that indirect 
	object is set to the number of bytes actually written (that 
	is, the encoded length if the data was encoded). An indirect 
	<code>Length</code> key is useful for one-pass writing, when the size 
	of the written data is not known in advance, either because 
	the data was to be encoded, or because there was no way 
	to know how much data there would be before the source reached 
	its EOF. </p>

	<p>An exception is raised if <code>attributesDict</code> is neither the <code>NULL</code> 
	object nor a direct Cos dictionary, <code>sourceStart</code> is nonnegative but the 
	source is not seekable, or if <code>sourceLength</code> is nonnegative but the 
	source stream reaches EOF before that many bytes have been read. </p>

	@param dP The Cos document in which the newly created 
	stream will be used.
	@param indirect Must always be <code>true</code>, specifying that the 
	Cos stream is created as an indirect object (all streams 
	are indirect). This also sets the document's <code>PDDocNeedsSave</code> 
	flag (see <code>PDDocFlags</code>). 
	@param stm The source stream containing the data to copy 
	into the new stream. The caller is responsible for closing 
	<code>stm</code> after <code>CosNewStream()</code> returns. The source stream can be 
	any readable <code>ASStm</code>. Typical sources are:
	<ul>
	<li> Files (<code>ASFileStmRdOpen()</code>) or memory (<code>ASMemStmRdOpen()</code>). These streams are always 
	seekable.</li>
	<li> Arbitrary procedures (<code>ASProcStmRdOpen()</code> or 
	<code>ASProcStmRdOpenEx()</code>), or other Cos streams (<code>CosStreamOpenStm()</code>). 
	These streams are always non-seekable.</li> 
	</ul>
	@param sourceStart The byte offset into <code>stm</code> from which 
	data reading starts for a seekable stream. If the value 
	is negative, it specifies that the stream is not seekable. 
	
	@param encodeTheSourceData Determines whether the data in <code>stm</code> should 
	be encoded using filters specified in <code>attributesDict</code> before 
	it is written to the Cos stream. See the description of 
	the encoding step above. If <code>attributesDict</code> is a <code>NULL</code> object 
	or if the dictionary has no <code>Filter</code> key, this value is ignored. 
	
	@param attributesDict Either the <code>NULL</code> Cos object, or a 
	direct Cos dictionary containing stream attributes, such 
	as the length of the Cos stream data and a list of decoding 
	filters and parameters to apply to the data, as defined 
	in Section 3.2.7 in the <i>PDF Reference</i>. See the encoding 
	step in the description above. 
	@param encodeParms The parameters to be used by the filters 
	if the source data is encoded before it is written to the 
	file. The parameters follow the structure for the value 
	of the <code>DecodeParms</code> stream attribute described in Table 3.4 
	in the <i>PDF Reference</i>. See the encoding step in the description 
	above. If no encoding parameters are needed, this value 
	is ignored. 
	@param sourceLength The amount of data to be read from 
	the source. If negative (typically <code>-1</code>), data is read from 
	the source until it reaches its EOF. See <b>Step 1</b> in the description 
	above.
	@return The newly created stream Cos object. 
	@see CosObjDestroy 
	@see CosNewStream64

	@note CosNewStream() sets the document <code>PDDocNeedsSave</code> flag 
	(see PDDocFlags). 

	@note You cannot call <code>CosStreamPos()</code> on a stream created with 
	<code>CosNewStream()</code> until the file has been saved. 
	@since PI_COS_VERSION >= 0x00020000
*/
NPROC(CosObj, CosNewStream, (CosDoc dP, ASBool indirect, ASStm stm, CosStreamStartAndCode sourceStart, ASBool encodeTheSourceData, CosObj attributesDict, CosObj encodeParms, CosByteMax sourceLength))

/**
	Destroys a Cos object. This method does nothing if <code>obj</code> is 
	a direct scalar object, such as the <code>NULL</code> object. 

	<p>If a composite object (an array, dictionary or stream) is destroyed: </p>
	<ul>
	<li>All the direct objects in it are automatically destroyed. </li>
	<li>The indirect objects in it are not destroyed. </li>
	</ul>
	@param obj The object to destroy.
	@see CosNewArray 
	@see CosNewBoolean 
	@see CosNewDict 
	@see CosNewFixed 
	@see CosNewInteger 
	@see CosNewName 
	@see CosNewStream 
	@see CosNewString 
	@since PI_COS_VERSION >= 0x00020000
*/
NPROC(void, CosObjDestroy, (CosObj obj))

/**
	Gets the 32-bit integer value of a specified number object. 
	@param obj The object whose integer value is obtained. 
	It must have type <code>CosInteger</code> or <code>CosReal</code> (<code>CosFixed</code>). If it is <code>CosReal</code>, 
	its value is rounded to the nearest integer. The result 
	is undefined if the real value is outside the range of <code>ASInt32</code> 
	numbers.
	<p>An exception is raised if the given object has the wrong Cos type. </p>
	@return The 32-bit integer value of <code>obj</code>. 
	@see CosNewInteger 
	@see CosNewFixed 
	@see CosNewFloat
	@since PI_COS_VERSION >= 0x00020000
*/
NPROC(ASInt32, CosIntegerValue, (CosObj obj))

/**
	Gets the value of <code>obj</code> as a fixed-point real number. 
	@param obj The object whose value is obtained. It must 
	have type <code>CosInteger</code> or <code>CosReal</code> (<code>CosFixed</code>). The result is undefined 
	if the real value is outside the range of <code>ASFixed</code> numbers.

	<p>An exception is raised if the given object has the wrong Cos type. </p>
	
	@return The numeric value of <code>obj</code>, represented as a fixed-point number. 
	@see CosIntegerValue 
	@see CosNewFixed 
	@since PI_COS_VERSION >= 0x00020000
*/
NPROC(ASFixed, CosFixedValue, (CosObj obj))

/**
	Gets the value of the specified boolean object. 
	<p>An exception is raised if <code>obj</code> has the wrong Cos type. </p>

	@param obj The boolean Cos object whose value is obtained.
	@return The value of <code>obj</code>. 
	@see CosNewBoolean 
	@since PI_COS_VERSION >= 0x00020000
*/
NPROC(ASBool, CosBooleanValue, (CosObj obj))


/**
	Gets the value of a name object. 
	@param obj The object of type <code>CosName</code> whose value is obtained.
	@return The <code>ASAtom</code> corresponding to the specified name object. An <code>ASAtom</code> 
	can be converted to a string using <code>ASAtomGetString()</code>.
	Note that <code>CosCopyNameStringValue()</code> can be used to obtain the name as a
	string, without creating an <code>ASAtom</code> (<code>ASAtom</code> objects consume global memory
	that is not deallocated).
	<p>An exception is raised if <code>obj</code> has the wrong type, if storage 
	is exhausted, or if file access fails. </p>
	@see CosNewName 
	@see CosNewNameFromString
	@see CosCopyNameStringValue
	@since PI_COS_VERSION >= 0x00020000
*/
NPROC(ASAtom, CosNameValue, (CosObj obj))

/**
	Gets the value of a string Cos object, and the string's length. 

	<p>An exception is raised if the type of <code>obj</code> is not a <code>CosString</code>. </p>
	
	@note The pointer returned from this method is not guaranteed 
	to remain valid if <code>CosStringValue()</code> is called again. It is 
	recommended that you use <code>CosStringValueSafe()</code> or <code>CosCopyStringValue()</code> 
	instead; these methods place the string into a user-allocated 
	buffer. 
	@param obj IN The object whose value is obtained. 
	@param nBytes OUT (Filled by the method) The length of the 
	string, in bytes. It must be a non-<code>NULL</code> pointer.
	@return The value of <code>obj</code>. 
	
	@see CosNewString 
	@see CosCopyStringValue 

	@note The caller must immediately copy the returned string. 
	The memory pointed to be the return value may become invalid 
	if any memory-allocating calls are made. In particular, 
	consider the sequence: 

		<code>
		<p>str1 = CosStringValue(...); </p>
		<p>str2 = CosStringValue(...); </p>
		</code>
	
	<p>In this case, the contents of <code>str1</code> 
	may be invalid by the time the second CosStringValue() call 
	returns. </p>

	@note The returned value is not a C-style string. Cos string 
	objects can contain <code>NULL</code> bytes. Standard C string-handling 
	functions may not work as expected. 
	@since PI_COS_VERSION >= 0x00020000
*/
NPROC(char *, CosStringValue, (CosObj obj, ASTCount *nBytes))


/**
	Gets the value of the specified key in the specified dictionary. 
	If it is called with a stream object instead of a dictionary object, 
	this method gets the value of the specified key from the 
	stream's attributes dictionary. 
	
	@param dict The dictionary or stream from which a value 
	is obtained. 
	@param key The key whose value is obtained, repesented as an ASAtom.
	See the <i>PDF Reference</i> to obtain the names of keys in dictionary objects 
	that are part of standard PDF, such as annotations or page 
	objects (for example, <code>CosDictGet(dict, ASAtomFromString("Length"))</code> ).

	<p>Note that strings can be used directly as keys, by calling
	<code>CosDictGetKeyString()</code> (for example, <code>CosDictGetKeyString(dict, "Length") </code>).
	This method is preferred, because it avoids the creation of new ASAtom objects.</p>

	<p><b>Key Names:</b> Even though key names in a PDF file are written with
	a leading slash (e.g., <code>&lt;&lt;/Length 42&gt;&gt;</code>), the slash is omitted
	when creating an <code>ASAtom</code> to be used as a key, or when using the
	string directly as a key, as in the examples above.</p>

	<p>Cos name objects can also be used as keys, by calling <code>CosDictGetKey()</code>.
	This method will also avoid the creation of new <code>ASAtom</code> objects and is often
	more convenient than using <code>ASAtom</code> objects or strings.</p>

	@return The object associated with the specified key. If <code>key</code> is 
	not present or if its value is <code>NULL</code> (which is equivalent), it returns 
	a <code>NULL</code> Cos object (a Cos object of type <code>CosNull</code>.) 
	@see CosDictGetKey
	@see CosDictGetKeyString
	@see CosDictPut 
	@see CosDictPutKey
	@see CosDictPutKeyString
	@see CosDictKnown 
	@see CosDictKnownKey
	@see CosDictKnownKeyString
	@see CosStreamDict 

	@note Use CosObjEnum() to list all key-value pairs in a dictionary. 
	@since PI_COS_VERSION >= 0x00020000
*/
NPROC(CosObj, CosDictGet, (CosObj dict, ASAtom key))

/**
	<p>Sets the value of a dictionary key, adding the key to the 
	dictionary if it is not already present. Sets the <code>PDDocNeedsSave</code> 
	flag (see <code>PDDocSetFlags</code>) of the <code>dict</code> object's <code>CosDoc</code> if <code>dict</code> is indirect 
	or is a direct object with an indirect composite object 
	at the root of its container chain. </p>

	<p>This method can also be used with a stream object. In that 
	case, the key-value pair is added to the stream's attributes 
	dictionary. </p>

	<p>It is not safe to call <code>CosDictPut()</code> during a call to <code>CosObjEnum()</code>
	on that same dictionary (for example, from within the callback procedure).</p>

	<p>An exception is raised if <code>val</code> is a direct non-scalar object 
	that is already contained in another dictionary, array, or stream,
    or if <code>dict</code> and <code>val</code> belong to different documents.</p>

	@param dict The dictionary or stream in which a value 
	is set. 
	@param key The key whose value is set, represented as an <code>ASAtom</code>.
	See the <i>PDF Reference</i> to obtain the names of keys in dictionary objects that are 
	part of standard PDF, such as annotations or page objects (see <code>CosDictGet()</code> for <b>Key Names</b>).

	<p>Note that strings can be used directly as keys, by calling
	<code>CosDictPutKeyString()</code> (for example, <code>CosDictPutKeyString(dict, "Length", lenObj) </code>).
	This method is preferred, because it avoids the creation of new <code>ASAtom</code> objects.</p>

	<p>Cos name objects can also be used as keys, by calling <code>CosDictPutKey()</code>.
	This method will also avoid the creation of new <code>ASAtom</code> objects and is often
	more convenient than using ASAtom objects or strings.</p>

	@param val The value to set.
	
	@see CosDictGet 
	@see CosDictGetKey
	@see CosDictGetKeyString
	@see CosDictPutKey
	@see CosDictPutKeyString
	@see CosDictKnown 
	@see CosDictKnownKey
	@see CosDictKnownKeyString
	@see CosStreamDict 

	@note A dictionary entry whose value is <code>NULL</code> is equivalent 
	to an absent entry; using <code>CosDictPut()</code> to put a <code>NULL</code> value 
	in a dictionary has the same effect as calling <code>CosDictRemove()</code> 
	to remove it from the dictionary. 
	@since PI_COS_VERSION >= 0x00020000
*/
NPROC(void, CosDictPut, (CosObj dict, ASAtom key, CosObj val))

/**
	<p>Removes a key-value pair from a dictionary. Sets the <code>PDDocNeedsSave</code> 
	flag (see <code>PDDocSetFlags</code>) of the <code>dict</code> object's <code>CosDoc</code> if the dictionary 
	is indirect or has an indirect composite object at the root 
	of its container chain.</p>

	<p>If it is called with a stream object instead of a dictionary object, 
	this method removes the value of the specified key from the 
	stream's attributes dictionary. </p>

	<p>It is not safe to call <code>CosDictRemove()</code> during a call to <code>CosObjEnum()</code>
	on that same dictionary (for example, from within the callback procedure).</p>

	<p>If the key is not present in the dictionary, <code>CosDictRemove()</code> has no effect.</p>

	@param dict The dictionary from which the key-value pair is removed. 
	@param key The key to remove, represented as an ASAtom. 
	See the <i>PDF Reference</i> to obtain the names of keys in dictionary objects that are 
	part of standard PDF, such as annotations or page objects (see <code>CosDictGet()</code> for <b>Key Names</b>).

	<p>Note that strings can be used directly as keys, by calling
	CosDictRemoveString() (for example, <code>CosDictRemoveString(dict, "Length") </code>).
	This method is preferred, because it avoids the creation of new <code>ASAtom</code> objects.</p>

	<p>Cos name objects can also be used as keys, by calling <code>CosDictRemoveKey()</code>.
	This method will also avoid the creation of new <code>ASAtom</code> objects and is often
	more convenient than using <code>ASAtom</code> objects or strings.</p>

	@see CosDictGet 
	@see CosDictGetKey
	@see CosDictGetKeyString
	@see CosDictPut
	@see CosDictPutKey
	@see CosDictPutKeyString
	@see CosDictKnown 
	@see CosDictKnownKey
	@see CosDictKnownKeyString
	@see CosDictRemoveKey
	@see CosDictRemoveKeyString
	@see CosStreamDict 
	@since PI_COS_VERSION >= 0x00020000
*/
NPROC(void, CosDictRemove, (CosObj dict, ASAtom key))

/**
	<p>Tests whether a specific key is found in the specified dictionary. 
	Calling this method is equivalent to checking if the value 
	returned from <code>CosDictGet()</code> is a <code>NULL</code> Cos object. </p>

	<p>If it is called with a stream object instead of a dictionary object, 
	this method tests whether the specified key is found in the 
	stream's attributes dictionary. </p>

	@param dict The dictionary or stream in which to look for <code>key</code>.
	@param key The key to find. See the <i>PDF Reference</i> to obtain 
	the names of keys in dictionary objects that are part of 
	standard PDF, such as annotations or page objects (see <code>CosDictGet()</code> for <b>Key Names</b>).

	<p>Note that strings can be used directly as keys, by calling
	<code>CosDictKnownKeyString()</code> (for example, <code>CosDictKnownKeyString(dict, "Length") </code>).
	This method is preferred, because it avoids the creation of new <code>ASAtom</code> objects.</p>

	<p>Cos name objects can also be used as keys, by calling <code>CosDictKnownKey()</code>.
	This method will also avoid the creation of new <code>ASAtom</code> objects and is often
	more convenient than using <code>ASAtom</code> objects or strings.</p>

	@return <code>true</code> if the value of a key is known (exists and is not <code>NULL</code>) 
	in <code>dict</code>, <code>false</code> otherwise. 
	@see CosDictGet 
	@see CosDictGetKey
	@see CosDictGetKeyString
	@see CosDictPut
	@see CosDictPutKey
	@see CosDictPutKeyString
	@see CosDictKnownKey
	@see CosDictKnownKeyString
	@see CosStreamDict 
	@since PI_COS_VERSION >= 0x00020000
*/
NPROC(ASBool, CosDictKnown, (CosObj dict, ASAtom key))

/**
	Gets the specified element from an array. 
	@param array The array from which an element is obtained. 
	
	@param index The array element to obtain. The first element 
	in an array has an index of zero. 
	@return The Cos object occupying the <code>index</code> element of <code>array</code>. It returns 
	a <code>NULL</code> Cos object if <code>index</code> is outside the array bounds. 
	
	@see CosArrayLength 
	@see CosArrayPut 
	@see CosArrayInsert 
	@since PI_COS_VERSION >= 0x00020000
*/
NPROC(CosObj, CosArrayGet, (CosObj array, ASTArraySize index))


/**
	<p>Puts the specified object into the specified location in 
	an array. The array is extended as much as necessary and 
	<code>NULL</code> objects are stored in empty slots. It sets the <code>PDDocNeedsSave</code> 
	flag (see <code>PDDocSetFlags</code>) flag of the <code>array</code> object's CosDoc if <code>array</code> 
	is indirect or is a direct object with an indirect composite 
	object at the root of its container chain.</p>

	<p>It is not safe to call <code>CosArrayPut()</code> during a call to <code>CosObjEnum()</code>
	on that same array (for example, from within the callback procedure), if
	doing so would extend the length of the array.</p>

	<p>An exception is raised if the object to insert is a direct object 
	that is already contained in another object, or if the object to insert belongs to another document. </p>

	@param array The array in which <code>obj</code> is stored. 
	@param index The location in <code>array</code> to store <code>obj</code>. The first 
	element of an array has an index of zero. 
	@param obj The Cos object to insert into <code>array</code>.
	
	@see CosArrayLength 
	@see CosArrayGet 
	@see CosArrayInsert 
	@since PI_COS_VERSION >= 0x00020000
*/
NPROC(void, CosArrayPut, (CosObj array, ASTArraySize index, CosObj obj))

/**
	<p>Inserts an object into an array. </p>

	<p>An exception is raised if the object to insert is a direct object 
	that is already contained in another object, or if the object to insert belongs to another document. </p>

	@param array The array into which the object is inserted. 
	
	@param pos The location in the array to insert the object. 
	The object is inserted before the specified location. The 
	first element in an array has a pos of zero. If <code>pos >= CosArrayLength(array)</code>, 
	<code>obj</code> is added at the end of the array. The length of the 
	array always increases by <code>1</code>.

	<p>It is not safe to call <code>CosArrayInsert()</code> during a call to <code>CosObjEnum()</code>
	on that same array (for example, from within the callback procedure).</p>

	@param obj The object to insert.
	
	@see CosArrayLength 
	@see CosArrayRemove 
	@see CosArrayGet 
	@since PI_COS_VERSION >= 0x00020000
*/
NPROC(void, CosArrayInsert, (CosObj array, ASTArraySize pos, CosObj obj))

/**
	<p>Finds the first element, if any, equal to the specified 
	object and removes it from the array. <code>CosObjEqual()</code> is used 
	to determine whether an array element is equal to the specified 
	object. </p>

	<p>The array is compressed after removing the element. The 
	compression is accomplished by moving each element following 
	the deleted element to the slot with the next smaller index 
	and decrementing the array's length by <code>1</code>. </p>

	<p>It is not safe to call <code>CosArrayRemove()</code> during a call to <code>CosObjEnum()</code>
	on that same dictionary (for example, from within the callback procedure).</p>

	@param array The array from which <code>obj</code> is removed. 
	@param obj The object to remove.
	@see CosArrayInsert 
	@since PI_COS_VERSION >= 0x00020000
*/
NPROC(void, CosArrayRemove, (CosObj array, CosObj obj))

/**
	Gets the number of elements in <code>array</code>. 
	@param array IN/OUT The array for which the number of elements 
	is determined. 
	@return The number of elements in <code>array</code>. 
	@since PI_COS_VERSION >= 0x00020000
*/
NPROC(ASTArraySize, CosArrayLength, (CosObj array))


/**
	<p>Gets the length of a Cos stream from the <code>Length</code> key in the 
	stream's attributes dictionary. This specifies the length 
	of the undecoded data, which is the number of bytes in the 
	stream before the <code>Filter</code> (if any) is applied.</p>

	<p>This has the same effect as calling
	<code>CosIntegerValue(CosDictGetKeyString(stream, "Length"))</code>.</p>

	<p>An exception is raised if the <code>Length</code> key is not found in the 
	attributes dictionary, if its value is not an integer, or if its value is outside the
	range of 32-bit integers.</p>

	@param stream The stream whose length is obtained.
	@return The length of the stream. 
	@see CosStreamDict 
	@see CosStreamPos
	@see CosStreamLength64
	@since PI_COS_VERSION >= 0x00020000
*/
NPROC(ASTArraySize, CosStreamLength, (CosObj stream))

/**
	Gets a stream's attributes dictionary. 
	@param stream IN/OUT The stream whose attributes dictionary is 
	obtained. 
	@return The stream's attributes dictionary Cos object. 
	@see CosStreamLength 
	@see CosStreamPos 
	@see CosDictGet 
	@see CosDictPut 
	@since PI_COS_VERSION >= 0x00020000
*/
NPROC(CosObj, CosStreamDict, (CosObj stream))

/**
	Creates a new, non-seekable <code>ASStm</code> for reading data from 
	a Cos stream. The data in the Cos stream may be filtered 
	and encrypted. After opening the Cos stream, data can be 
	read from it into memory using <code>ASStmRead()</code>. When reading is 
	completed, close the stream using <code>ASStmClose()</code>. 
	@param stream The Cos stream object for which an <code>ASStm</code>
	is opened. 
	@param mode This must be one of the <code>CosStreamOpenMode</code> values.
	@return The newly-opened <code>ASStm</code>. 
	@see ASStmRead 
	@see ASStmWrite 
	@see CosNewStream 
	@since PI_COS_VERSION >= 0x00020000
*/
NPROC(ASStm, CosStreamOpenStm, (CosObj stream, CosStreamOpenMode mode))

/**
	<p>Gets the byte offset of the start of a Cos stream's data 
	in the PDF file (which is the byte offset of the beginning 
	of the line following the <code>stream</code> token). Use this method 
	to obtain the file location of any private data in a stream 
	that you need to read directly rather than letting it pass 
	through the normal Cos mechanisms. For example, this could apply to a QuickTime 
	video embedded in a PDF file. </p>

	<p><code>CosStreamPos()</code> is only valid when called on a stream that 
	is already stored in a PDF document. If the stream was created 
	using <code>CosNewStream()</code>, the new stream is stored in the document's 
	temp file, and you cannot invoke <code>CosStreamPos()</code> on it. After 
	the file has been saved, you can use <code>CosStreamPos()</code> on the 
	stream.</p> 
	@param stream The stream whose current position is obtained.
	@return The byte offset of the start of the Cos stream's data in 
	the PDF file. 
	@exception cosErrInvalidObj is raised if the stream object has not yet 
	been saved to the PDF file. In other words, before you can call <code>CosStreamPos()</code> on a newly 
	created stream, you must first save the PDF file. 
	@see CosStreamDict 
	@see CosStreamLength 
	@since PI_COS_VERSION >= 0x00020000
*/
NPROC(ASTCount, CosStreamPos, (CosObj stream))

/**
	Gets the <code>Catalog</code> (the root object) for the specified document. 
	See Section 3.6.1 in the <i>PDF Reference</i> for a description 
	of the <code>Catalog</code>. 
	@param dP IN/OUT The document whose <code>Catalog</code> is obtained. 
	@return The document's <code>Catalog</code> dictionary Cos object. 
	@see CosDocGetInfoDict 
	@since PI_COS_VERSION >= 0x00020000
*/
NPROC(CosObj, CosDocGetRoot, (CosDoc dP))

/**
	Gets the specified document's <code>Info</code> dictionary. In general, 
	access the document's <code>Info</code> dictionary using PDDocGetInfo() 
	and <code>PDDocSetInfo()</code> wherever possible. 
	@param dP IN/OUT The document whose <code>Info</code> dictionary is obtained. 
	
	@return The document's <code>Info</code> dictionary Cos object. 
	@see CosDocGetRoot 
	@see PDDocGetInfo 
	@see PDDocSetInfo 
	@since PI_COS_VERSION >= 0x00020000
*/
NPROC(CosObj, CosDocGetInfoDict, (CosDoc dP))

#ifndef ANDROID_ENV
/* The following method APIs are in PICrypt.h. */

/**
	<p>Decrypts data in a buffer using the specified encryption 
	key. The standard Acrobat viewer encryption/decryption algorithm 
	(RC4 from RSA Data Security, Inc.) is used. </p>

	<p>An exception is raised if encryption encounters an internal 
	error.</p> 

	@param src The buffer containing the data to decrypt. 
	
	@param len The number of bytes in <code>src</code>. 
	@param dst (Filled by the method) The buffer into which 
	the decrypted data will be placed. This may point to the 
	same location as <code>src</code>. 
	@param cryptData The encryption key. 
	@param cryptDataLen The length of the encryption key in bytes. 
	It cannot be greater than <code>5</code>.
	@see CosEncryptData 
	@since PI_COS_VERSION >= 0x00020000
*/
NPROC(void, CosDecryptData, (void *src, ASTArraySize len, void *dst, char *cryptData, ASTArraySize cryptDataLen))

/**
	<p>Encrypts data in a buffer using the specified encryption 
	key. The standard Acrobat viewer encryption/decryption algorithm 
	(RC4 from RSA Data Security, Inc.) is used. </p>

	<p>An exception is raised if encryption encounters an internal 
	error.</p> 

	@param src The buffer containing the data to encrypt. 
	
	@param len The number of bytes in <code>src</code>. 
	@param dst (Filled by the method) The buffer into which 
	the encrypted data will be placed. This may point to the 
	same location as <code>src</code>. 
	@param cryptData The encryption key. 
	@param cryptDataLen Length of the encryption key, in bytes. 
	It cannot be greater than <code>5</code>.
	@see CosDecryptData 
	@since PI_COS_VERSION >= 0x00020000
*/
NPROC(void, CosEncryptData, (void *src, ASTArraySize len, void *dst, char *cryptData, ASTArraySize cryptDataLen))
#endif

/* Acrobat 3.0 additions */

/**
	<p>Opens a Cos document. The document does not need to be a 
	PDF document. In <code>params</code>, the client specifies a file system 
	and path name from which to open the document. The client 
	may also specify a header string other than <code>"%PDF-"</code>. For 
	example, a client might want to open a private file type, 
	such as <code>"%FDF-"</code>. </p>

	<p>Various exceptions may be raised. Opening non-Cos docs with this API is unsupported and may lock the file after an open attempt. </p>

	<p>If the <code>doRepair</code> flag is set in the open flags, a minimal 
	document can be opened. A minimal document contains the 
	header string and a trailer dictionary. It may contain indirect 
	objects before the trailer dictionary, and the trailer dictionary 
	can refer to those objects, as shown in the following example:</p> 
	
	
	<p><code>%FDF-1.0</code></p>
	<p><code>1 0 obj</code></p>
	<p><code>&lt;&lt; /Version /1.5</code></p>
	<p><code>/FDF &lt;&lt; /F 20 0 R /JavaScript 5 0 R &gt;&gt;</code></p>
	<p><code>&gt;&gt;</code></p>
	<p><code>trailer</code></p>
	<p><code>&lt;&lt;</code></p>
	<p><code>/Root 1 0 R</code></p>
	<p><code>&gt;&gt; </code></p>
	

	@param params Specifies how to open the document.
	@return A Cos document. 
	@see CosDocClose 
	@since PI_COS_VERSION >= 0x00020002
*/
NPROC(CosDoc, CosDocOpenWithParams, (CosDocOpenParams params))

/**
	Closes a Cos document. You should only call this method 
	with a document obtained via <code>CosDocOpenWithParams()</code> to release 
	resources used by the Cos document. 
	@param cosDoc IN/OUT The document to close. 
	@see CosDocOpenWithParams 
	@since PI_COS_VERSION >= 0x00020002
*/
NPROC(void, CosDocClose, (CosDoc cosDoc))

/**
	Creates an empty Cos document. 
	@param createFlags An inclusive OR of bit flags that specify 
	the attributes of a CosDoc when created by <code>CosDocCreate()</code>. 
	The only flag currently defined is <code>cosDocCreateInfoDict (0x01)</code>, which creates an Info dictionary for the document.
	@return An empty Cos document. 
	@see CosDocSaveToFile 
	@since PI_COS_VERSION >= 0x00020002
*/
NPROC(CosDoc, CosDocCreate, (ASFlagBits createFlags))

/**
	Saves a Cos document to a file handle. <code>CosDocSaveToFile()</code> 
	will not generate an cross-reference table in the saved file. If you 
	want the cross-reference to be generated, then you have to use <code>CosDocSaveWithParams()</code>, 
	which generates the cross-reference table by default. 
	@param cosDoc IN/OUT The document to save. 
	@param asFile IN/OUT The file to which the document is written; it must 
	be open in write mode. This file is not necessarily position-able. 
	@param saveFlags IN/OUT An <code>OR</code> of the values listed in <code>CosDocSave</code> 
	Flags specifying how to save the document. 
	@param saveParams IN/OUT Optional parameters for use when saving 
	a document, as described in CosDocSaveParams(). 
	@exception cosErrAfterSave 
	@exception cosErrNeedFullSave 
	@exception genErrBadParm 
	@see CosDocCreate 
	@see CosDocSaveWithParams 
	@since PI_COS_VERSION >= 0x00020002
*/
#if READER || READER_PLUGIN
NOPROC(CosDocSaveToFile)
#else

/**
	Saves a Cos document to a file. <code>CosDocSaveToFile()</code> will not 
	generate a cross-reference index (table or stream) in the 
	saved file. If you want the index to be generated, then 
	you must use <code>CosDocSaveWithParams()</code>, which generates it 
	by default. 
	@param cosDoc The document to save. 
	@param asFile The file to which the document is written; it must 
	be open in write mode. This file is not necessarily position-able. 
	
	@param saveFlags An <code>OR</code> of the <code>CosDocSaveFlags</code> bit flag 
	values specifying how to save the document. 
	@param saveParams Optional parameters for use when saving 
	a document, as described in <code>CosDocSaveParams()</code>.
	@exception cosErrAfterSave 
	@exception cosErrNeedFullSave 
	@exception genErrBadParm 
	@see CosDocCreate 
	@see CosDocSaveWithParams 
	@since PI_COS_VERSION >= 0x00020002
*/
NPROC(void, CosDocSaveToFile, (CosDoc cosDoc, ASFile asFile, CosDocSaveFlags saveFlags, CosDocSaveParams saveParams))

#endif

/**
	Sets a Cos document's <code>dirty</code> flag to a given boolean value. 
	If this flag is <code>true</code> when the document is closed, it indicates 
	that the document must be saved to preserve changes. 
	@param cosDoc The Cos document whose <code>dirty</code> flag is set. 
	
	@param isDirty <code>true</code> if dirty, <code>false</code> otherwise.
	@see CosDocSaveToFile 
	@see CosDocSaveWithParams 
	@since PI_COS_VERSION >= 0x00020002
*/
NPROC(void, CosDocSetDirty, (CosDoc cosDoc, ASBool isDirty))

/* Acrobat 4.0 additions */

/**
	Gets the local master index for an indirect object. For 
	indirect objects, the local master index is the same as 
	the indirect object index that appears in the PDF file. 
	
	@param obj IN/OUT The indirect <code>CosObj</code> for which the ID is obtained. 
	A <code>CosObj</code> can be determined to be indirect using <code>CosObjIsIndirect()</code>. 
	
	@return The ID of <code>obj</code>. 
	@exception cosErrInvalidObj is raised if the object is not valid or is 
	not indirect. 
	@see CosDocGetObjByID 
	@see CosObjGetGeneration 
	@see CosObjIsIndirect 
	@since PI_COS_VERSION >= 0x00040000
*/
NPROC(CosID, CosObjGetID,(CosObj obj))

/**
	Gets the generation number of an indirect Cos object. See 
	Section 3.2.9 in the <i>PDF Reference</i> for more information. 
	
	@param obj IN/OUT The indirect <code>CosObj</code> for which the generation 
	number is obtained. A <code>CosObj</code> can be determined to be indirect 
	using <code>CosObjIsIndirect()</code>. 
	@return The generation number of <code>cosObj</code>. 
	@exception cosErrInvalidObj is raised if the object is not valid or is 
	not indirect. 
	@see CosObjGetID 
	@see CosObjIsIndirect 
	@since PI_COS_VERSION >= 0x00040000
*/
NPROC(CosGeneration, CosObjGetGeneration, (CosObj obj))

/**
	Gets the indirect <code>CosObj</code> with the latest generation number. 
	
	@param dP The <code>CosDoc</code> to search for the matching Cos object. 
	
	@param objNum The local master index for the indirect 
	Cos object to return.
	@return The <code>CosObj</code> with the latest generation number whose ID (object 
	number) equals <code>objNum</code>, or the <code>NULL</code> object if there is no 
	object with this ID. 
	@see CosObjGetID 
	@since PI_COS_VERSION >= 0x00040000
*/
NPROC(CosObj, CosDocGetObjByID, (CosDoc dP, CosID objNum))

/**
	Saves a Cos document, optionally to a new file handle. It generates 
	an cross-reference table by default. 
	@param cosDoc IN/OUT The <code>CosDoc</code> for the document to save. 
	@param asFile IN/OUT (Optional) If saving to the same file, do 
	not pass in an <code>ASFile</code>. If saving to a different file, specify 
	the file to which the document is written; it must be open 
	in write mode. If <code>NULL</code>, this method saves to the ASFile 
	originally associated with the CosDoc. 
	@param saveFlags IN/OUT A bit field composed of the <code>CosDocSaveFlags</code>
	specifying how to save the document. 
	@param saveParams IN/OUT (Optional) <code>CosDocSaveParams</code> parameters 
	for use when saving the <code>CosDoc</code> document. 
	@exception cosErrAfterSave 
	@exception cosErrNeedFullSave 
	@exception genErrBadParm 
	@see CosDocCreate 
	@see CosDocSaveToFile 
	@since PI_COS_VERSION >= 0x00040000
*/
#if READER || READER_PLUGIN
NOPROC(CosDocSaveWithParams)
#else

/**
	Saves a Cos document, optionally to a new file. It generates 
	a cross-reference index (table or stream) by default. 
	@param cosDoc The <code>CosDoc</code> for the document to save. 
	@param asFile The file to which the document will be written. 
	This file must already be open in write mode. If you pass 
	<code>NULL</code>, <code>cosDoc</code> is saved to the file with which it was originally 
	associated. 
	@param saveFlags An <code>OR</code> of the <code>CosDocSaveFlags</code> bit flag 
	values specifying how to save the document. 
	@param saveParams <code>CosDocSaveParams</code> parameters for use 
	when saving the <code>CosDoc</code> document. 
	@exception cosErrAfterSave 
	@exception cosErrNeedFullSave 
	@exception genErrBadParm 
	@see CosDocCreate 
	@see CosDocSaveToFile 
	@since PI_COS_VERSION >= 0x00040000
*/
NPROC(void, CosDocSaveWithParams, (CosDoc cosDoc, ASFile asFile, CosDocSaveFlags saveFlags, CosDocSaveParams saveParams))

#endif

/**
	Calls the specified procedure for each EOF in a given <code>CosDoc</code>, 
	where the EOF is a position in a PDF file after a <code>%%EOF</code> 
	keyword that marks the end of either a main cross-reference 
	section, or an update cross-reference section that corresponds 
	to an incremental save. Not every <code>%%EOF</code> keyword fits these 
	criteria. For example, the first <code>%%EOF</code> in a linearized (optimized for the web) file 
	does not, so its position is not be passed to <code>proc</code>. 

	<p>If <code>cosDoc</code> was created in memory (using CosDocCreate()), or 
	if it was damaged and needed to be repaired, the procedure 
	is not called at all. </p>
	@param cosDoc The <code>CosDoc</code> in which the EOF's are enumerated. 
	
	@param proc The <code>CosDocEnumEOFsProc()</code> to call for each EOF. 
	
	@param clientData A pointer to user-supplied data to pass 
	to <code>proc</code> each time it is called.
	@return <code>true</code> if all of the calls to <code>proc</code> return <code>true</code>. <code>false</code> as soon 
	as a call to <code>proc</code> returns <code>false</code>. 
	@see CosDocEnumIndirect 
	@see CosDocEnumEOFs64
	@ingroup Enumerators
	@since PI_COS_VERSION >= 0x00040000
*/
NPROC(ASBool, CosDocEnumEOFs, (CosDoc cosDoc, CosDocEnumEOFsProc proc, void * clientData))

/**
	Sets the hex flag of the <code>CosString</code>. The hex flag specifies 
	whether the <code>CosString</code> should be written out as hex when 
	writing the Cos Object to file. 
	@param cosObj The <code>CosString</code> for which the hex flag is 
	set. 
	@param setHex The value to set for the flag.
	@return The value of <code>setHex</code>. 
	@exception cosErrExpectedString 
	@see CosStringGetHexFlag 
	@since PI_COS_VERSION >= 0x00040000
*/
NPROC(ASBool, CosStringSetHexFlag, (CosObj cosObj, ASBool setHex))

/**
	Gets the hex flag of the <code>CosString</code>. The hex flag specifies 
	whether the <code>CosString</code> should be written out as hex when 
	writing the Cos Object to file. 
	@param cosObj IN/OUT The <code>CosString</code> for which the hex flag is obtained. 
	
	@return The current value of the flag. 
	@exception cosErrExpectedString 
	@see CosStringSetHexFlag 
	@since PI_COS_VERSION >= 0x00040000
*/
NPROC(ASBool, CosStringGetHexFlag, (CosObj cosObj))

/**
	Gets a 32-bit hash code for the given <code>CosObj</code>. 

	<p>Two <code>CosObj</code> objects with equal hash codes are not necessarily 
	equal, however. Use <code>CosObjEqual()</code> to determine the equality 
	of Cos objects. </p>
	@param obj The <code>CosObj</code> for which to obtain a hash code.
	@return 32-bit hash code for the given <code>CosObj</code>, or <code>CosNewNull()</code> if 
	there is no object with this ID. 
	@see CosObjEqual 
	@since PI_COS_VERSION >= 0x00040000
*/
NPROC(CosHashCode, CosObjHash, (CosObj obj))

/**
	Copies a <code>CosObj</code> from one document to another (or the same 
	document). 
	@param srcObj The <code>CosObj</code> to copy. 
	@param destDoc The <code>CosDoc</code> for the document into which 
	the <code>CosObj</code> is copied. 
	@param copyIndirect <code>true</code> if all indirectly referenced 
	objects from <code>srcObj</code> are copied to <code>destDoc</code>, <code>false</code> otherwise.
	@return The <code>CosObj</code> which has been copied to the destination document. 
	
	@see CosObjEqual 
	@since PI_COS_VERSION >= 0x00040000
*/
NPROC(CosObj, CosObjCopy, (CosObj srcObj, CosDoc destDoc, ASBool copyIndirect))

/**
	Checks whether the position is within the array bounds,  
	removes it from the array, moves each subsequent 
	element to the slot with the next smaller index, and decrements 
	the array's length by <code>1</code>. It sets the <code>dirty</code> flag of the <code>array</code> object's 
	<code>CosDoc</code>. 
	@param array IN/OUT The <code>CosArray</code> from which to remove the member. 
	@param pos IN/OUT The index for the array member to remove. Array 
	indices start at <code>0</code>. 
	@see CosArrayRemove 
	@since PI_COS_VERSION >= 0x00040000
*/
NPROC(void, CosArrayRemoveNth, (CosObj array, ASTArraySize pos))

/**
	<p>Enumerates all the indirect objects of a given <code>CosDoc</code>. </p>

	<p>The objects are enumerated in no particular order. Successive 
	enumerations of the same Cos document are not guaranteed 
	to enumerate objects in the same order.</p> 

	<p>This method does not enumerate invalid objects, which include 
	objects that are defined as <code>NULL</code>, objects that are not defined 
	at all (those having no cross-reference entry), and objects 
	that are on the free list (see the <i>PDF Reference</i>). </p>

	<p>This re-raises any exception that <code>proc</code> raises. </p>

	@param dP The <code>CosDoc</code> whose indirect objects are enumerated. 
	
	@param proc A user-supplied callback to call for each indirect 
	object in <code>dP</code>. Enumeration ends when <code>proc</code> returns <code>false</code> or 
	all indirect objects have been enumerated. The value parameter 
	returned in <code>proc</code> is always the <code>NULL</code> Cos object. 
	@param clientData A pointer to user-supplied data to pass 
	to <code>proc</code> each time it is called.
	@return <code>true</code> if all of the calls to <code>proc</code> returned <code>true</code>. It returns <code>false</code> as 
	soon as a call to <code>proc</code> returns <code>false</code>. 
	@see CosObjEnum 
	@ingroup Enumerators
	@since PI_COS_VERSION >= 0x00040000
*/
NPROC(ASBool, CosDocEnumIndirect, (CosDoc dP, CosObjEnumProc proc, void * clientData))

#ifndef ANDROID_ENV
/* Acrobat 4.05 additions */

/**
	Gets the current version number of the encryption algorithm 
	supported. 
	@return The current version number of the encryption supported. 
	@see CosDecryptGetMaxKeyBytes 
	@see CosEncryptGetMaxKeyBytes 
	@since PI_COS_VERSION >= 0x00040005
*/
NPROC(ASTVersion, CosCryptGetVersion,       ())

/**
	Gets the maximum number of the decryption key length, in 
	bytes, for the specified <code>cryptVersion</code>. 
	@param cryptVersion IN/OUT The Cos crypt version, which is the version 
	of the algorithm that is used to encrypt and decrypt document 
	data. <code>cryptVersion</code> equal to <code>0</code> is treated as <code>cryptVersion</code> 
	equal to <code>1</code> to maintain backward compatibility. 
	@return The maximum number of key length, in bytes, for the specified 
	<code>cryptVersion</code>. If <code>cryptVersion</code> is not currently supported, 
	it returns <code>-1</code>. 
	@see CosCryptGetVersion 
	@see CosEncryptGetMaxKeyBytes 
	@since PI_COS_VERSION >= 0x00040005
*/
NPROC(CosByteMax, CosDecryptGetMaxKeyBytes, (ASTVersion cryptVersion))

/**
	Gets the maximum number of the encryption key length, in 
	bytes, for the specified <code>cryptVersion</code>. 
	@param cryptVersion IN/OUT The Cos crypt version, which is the version 
	of the algorithm that is used to encrypt and decrypt document 
	data. <code>cryptVersion</code> equal to <code>0</code> is treated as <code>cryptVersion</code> 
	equal to <code>1</code> to maintain backward compatibility. 
	@return The maximum number of key length, in bytes, for the specified 
	<code>cryptVersion</code>. If <code>cryptVersion</code> is not currently supported, 
	it returns <code>-1</code>. 
	@see CosCryptGetVersion 
	@see CosDecryptGetMaxKeyBytes 
	@since PI_COS_VERSION >= 0x00040005
*/
NPROC(CosByteMax, CosEncryptGetMaxKeyBytes, (ASTVersion cryptVersion))
#endif
/* Acrobat 5.0 additions */

/**
	<p>Returns a newly allocated buffer containing a copy of the 
	Cos object's string value. Upon return, <code>nBytes</code> contains the 
	number of bytes in the original Cos string. <code>CosCopyStringValue()</code> 
	never returns <code>NULL</code>; it raises an exception if the allocation 
	fails. The client is responsible for freeing the result 
	by calling <code>ASfree()</code>.</p> 

	<p><code>CosCopyStringValue()</code> allocates extra memory past the end of 
	the string and writes zeros into these extra bytes to ensure 
	that the string is <code>NULL</code>-terminated whether viewed as a UTF-16 
	(Unicode) string or as a C string (these bytes are not 
	included in the number returned in <code>nBytes</code>). If the Cos string 
	has <code>0</code> length, <code>nBytes</code> will be <code>0</code>, and a pointer to newly allocated 
	memory containing some zero bytes is returned (that is, 
	<code>CosCopyStringValue()</code> still returns a <code>NULL</code>-terminated string 
	but with zero length). </p>

	<p>An out-of-memory exception is raised if insufficient memory 
	is available. It can also raise any exception that CosStringValue() can raise. </p>

	@note In general, the returned value is not a <code>NULL</code>-terminated 
	C string. Cos string objects are binary and can contain 
	arbitrary byte sequences, including <code>NULL</code> characters. Standard 
	C string handling functions may not work as expected. 
	@param obj IN The Cos object whose string value is copied 
	and returned. 
	@param nBytes OUT (Filled by the method) The length of the 
	original Cos string in bytes. It can be <code>NULL</code> if you do not care 
	how many bytes were in the original string.
	@return A copy of the Cos object's string value. 
	@see CosStringValueSafe 
	@since PI_COS_VERSION >= 0x00050000
*/
NPROC(char *, CosCopyStringValue, (CosObj obj, ASTCount *nBytes))

/**
	<p>Copies at most <code>bufferSize</code> bytes from the <code>obj</code> parameter's string value into 
	<code>buffer</code>, and stores the actual length of the Cos string in 
	<code>*nBytes</code>. If <code>bufferSize</code> is greater than the length of the 
	Cos string, the remaining bytes in <code>buffer</code> have undefined 
	values upon return. </p>

	<p>A bad-parameter exception is raised if <code>bufferSize</code> is less 
	than <code>0</code> or <code>nBytes</code> is <code>NULL</code>. It can also raise any exception that <code>CosStringValue()</code> can raise. </p>

	@note In general, the returned value is not a <code>NULL</code>-terminated 
	C string. Cos string objects are binary data and can contain 
	any arbitrary byte sequence, including embedded <code>NULL</code> characters. 
	Standard C string handling functions may not work as expected. 
	@param obj The Cos object whose string value is copied. 
	@param buffer The buffer into which the Cos string value 
	is copied, or <code>NULL</code>. 
	@param bufferSize The length of <code>buffer</code> or <code>0</code>. 
	@param nBytes (Filled by the method) The length of the 
	original Cos string in bytes (which may be more than <code>bufferSize</code>). 
	It must be a non-<code>NULL</code> pointer.
	@return A copy of the Cos string value or an exception. It will never 
	return <code>NULL</code>. 
	
	@see CosCopyStringValue 
	@since PI_COS_VERSION >= 0x00050000
*/
NPROC(char *, CosStringValueSafe, (CosObj obj, char *buffer, ASTArraySize bufferSize, ASTCount *nBytes))


/**
	Returns two ID byte arrays identifying the CosDoc. The client 
	should copy these arrays before making the next call to 
	Acrobat. 
	@param dP IN/OUT The CosDoc whose ID byte arrays are returned. 
	
	@param pInstanceID IN/OUT (Filled by the method) The instance 
	ID. 
	@param pPermaID IN/OUT (Filled by the method) The permanent ID. 
	
	@param instIDLength IN/OUT The length of <code>pInstanceID</code> in bytes. 
	
	@param permIDLength IN/OUT The length of <code>pPermaID</code> in bytes. 
	@return <code>true</code> if the ID is returned, <code>false</code> otherwise. 
	@since PI_COS_VERSION >= 0x00050000
*/
NPROC(ASBool, CosDocGetID, (CosDoc dP, CosByte **pInstanceID, CosByte **pPermaID, ASTCount *instIDLength, ASTCount *permIDLength))

/**
	<p>Compares the two <code>CosObj</code> objects. The result is <code>0</code> only if 
	<code>CosObjEqual(obj1, obj2)</code> is <code>true</code>. Otherwise, the result is 
	either <code>-1</code> or <code>1</code>. The result is useful for ordering or sorting 
	Cos objects. No other significance should be attached to 
	the result. In particular, a nonzero result indicates nothing 
	about the type of either object. </p>

	<p>The result is valid only within a single instance of the 
	document. That is, if CosObjCmp() returns a nonzero value 
	and the document is closed and then reopened, there is no 
	guarantee that it will return the same nonzero value for 
	those same objects. </p>

	<p>The following conditions apply: </p>
	<ul>
	<li> If <code>CosObjCmp(a, b) == 0</code>, then <code>CosObjCmp(b, a) == 0</code>. </li>
	<li> If <code>CosObjCmp(a, b) > 0</code>, then <code>CosObjCmp(b, a) < 0</code>. </li>
	<li> If <code>CosObjCmp(a, b) < 0</code>, then <code>CosObjCmp(b, a) > 0</code>. </li>
	<li> If <code>CosObjCmp(a, b) == 0</code> and <code>CosObjCmp(b, c) == 0</code>, then 
	<code>CosObjCmp ( a, c ) == 0</code>. </li>
	<li> If <code>CosObjCmp(a, b) > 0</code> and <code>CosObjCmp(b, c) > 0</code>, then 
	<code>CosObjCmp (a, c) > 0</code>. </li>
	<li> If <code>CosObjCmp(a, b) < 0</code> and <code>CosObjCmp(b, c) < 0</code>, then 
	<code>CosObjCmp(a, c) < 0</code>. </li>
	</ul>
	@param obj1 The first <code>CosObj</code> to compare. 
	@param obj2 The second <code>CosObj</code> to compare.
	@return Returns zero if the two objects are equal, <code>-1</code> if <code>obj1</code> is 
	less than <code>obj2</code>, <code>1</code> if <code>obj1</code> is greater than <code>obj2</code>. 
	@see CosObjEqual 
	@since PI_COS_VERSION >= 0x00050000
*/
NPROC(ASInt32, CosObjCmp, (CosObj obj1, CosObj obj2))

/**
	Puts a limit on the amount of memory (RAM) that can be used to store Cos
	objects per doc. The default, minimum and maximum values of this limit are 
	30 MB, 512 KB and 40 MB respectively. This method can be used to increase 
	or decrease the amount of memory reserved for Cos objects within this limit. 
	Beyond the limit, Cos objects may be stored on disk.
	@param maxMemory The maximum amount of RAM (in bytes) that will be used to
	store fixed-size Cos objects.
	@since PI_COS_VERSION >= 0x00050001
*/
NPROC(void, CosSetMaxDocStorage, (ASInt32 maxMemory))

/* Acrobat 6.0 additions */

/**
	<p>Tests whether the definition of a specified Cos object, 
	in the file associated with the object's CosDoc, begins 
	within any of a set of byte ranges. The test is inclusive; 
	that is the object may begin at the first or last byte of 
	a range. </p>

	<p>An exception is raised if <code>obj</code> is a direct object or <code>numEntries</code> 
	is an odd number. </p>

	@param obj The Cos object (must be indirect). 
	@param byteRanges An array containing pairs of byte offsets 
	within the document. Each pair is a start and end offset 
	from the beginning of the document. 
	@param numEntries The number of byte offsets (not pairs) 
	in the <code>byteRanges</code> array.
	@return <code>true</code> if the object begins within any of the given ranges 
	and has not been modified, <code>false</code> otherwise. 
	@since PI_COS_VERSION >= 0x00060000
*/
NPROC(ASBool, CosDocObjIsWithinRange, (CosObj obj, ASInt32 byteRanges[], ASInt32 numEntries))

/**
	Tests whether an object is compressed (part of a CosObjCollection). 
	@param obj The object to test.
	@return <code>true</code> if <code>obj</code> is compressed, <code>false</code> otherwise. 
	@since PI_COS_VERSION >= 0x00060000
*/
NPROC(ASBool, CosObjIsCompressed, (CosObj obj))

/**
	Creates a new object collection for objects in a document. 
	
	@param dP The document whose objects are collected, or 
	<code>NULL</code> to create a <code>NULL</code> collection (a <code>NULL</code> collection is 
	not associated with a document and cannot store objects; 
	it is generally used only as an initial value for a variable 
	of type CosObjCollection).
	@return The newly created Cos object collection. 
	@see CosObjAddToCollection 
	@see CosObjCollectionEnum 
	@see CosObjGetCollection 
	@see CosObjCollectionIsNull 
	@since PI_COS_VERSION >= 0x00060000
*/
NPROC(CosObjCollection, CosNewObjCollection, (CosDoc dP))

/**
	Tests whether an object collection is <code>NULL</code>. A <code>NULL</code> collection 
	is not associated with a document and cannot store objects; 
	it is generally used only as an initial value for a variable 
	of type <code>CosObjCollection</code>. 
	@param coll The object collection to test.
	@return <code>true</code> if <code>coll</code> is <code>NULL</code>, <code>false</code> otherwise. 
	@see CosNewObjCollection 
	@since PI_COS_VERSION >= 0x00060000
*/
NPROC(ASBool, CosObjCollectionIsNull, (CosObjCollection coll))

/**
	<p>Gets the <code>CosObjCollection</code> containing the specified object. 
	If the object is not in a collection, the method raises 
	an exception. </p>

	<p>An error is raised if <code>obj</code> is not in a collection.</p>

	@param obj The object whose <code>CosObjCollection</code> is obtained.
	@return The <code>CosObjCollection</code> to which the object belongs. 
	@see CosObjAddToCollection 
	@see CosObjIsCompressed 
	@since PI_COS_VERSION >= 0x00060000
*/
NPROC(CosObjCollection, CosObjGetCollection, (CosObj obj))

/**
	<p>Adds a Cos object to a collection; see <code>CosObjCollection</code> 
	for requirements of these collections. This method sets 
	the dirty flag of the collection's Cos document. </p>

	<p>An exception is raised if the collection and the object belong to different Cos documents. </p>

	@param coll The Cos object collection. 
	@param item The object to add.
	@return <code>true</code> if <code>obj</code> was successfully added to the collection, <code>false</code> 
	otherwise. 
	@see CosObjGetCompressibility 
	@see CosObjIsCompressed 
	@see CosObjRemoveFromCollection 
	@see CosObjSetCompressibility 
	@since PI_COS_VERSION >= 0x00060000
*/
NPROC(ASBool, CosObjAddToCollection, (CosObjCollection coll, CosObj item))

/**
	<p>Removes a Cos object from the <code>CosObjCollection</code> to which 
	it belongs. </p>

	<p>An exception is raised if the object is not in the collection. </p>

	@param obj The object to remove.
	@see CosObjAddToCollection 
	@see CosObjGetCompressibility 
	@see CosObjIsCompressed 
	@see CosObjSetCompressibility 
	@since PI_COS_VERSION >= 0x00060000
*/
NPROC(void, CosObjRemoveFromCollection, (CosObj obj))

/**
	<p>Controls whether a Cos object can be compressed. A compressible 
	object can be added to a CosObjCollection. </p>

	<p>If you set the compressibility to <code>false</code>, calling <code>CosObjAddToCollection()</code>
	on that object has no effect. If the object is already compressed, 
	it is removed from the object collection to which it belongs 
	and then marked as incompressible. </p>

	<p>This method does nothing if applied to a direct object, 
	a stream, or an object whose generation number is not zero. 
	Objects of these types are never compressible. </p>
	@param obj The object whose compressibility is set. 
	@param compressible <code>true</code> if the object can be made part 
	of a <code>CosObjCollection</code>, <code>false</code> otherwise.
	@return <code>true</code> if <code>obj</code> is marked as compressible, <code>false</code> otherwise. 
	
	@see CosObjAddToCollection 
	@see CosObjGetCompressibility 
	@see CosObjIsCompressed 
	@since PI_COS_VERSION >= 0x00060000
*/
NPROC(void, CosObjSetCompressibility, (CosObj obj, ASBool compressible))

/**
	Tests whether an object is <i>compressible</i>. A compressible 
	object can be added to a <code>CosObjCollection</code>. 
	
	<p>An object is compressible only if all of the following conditions are true: </p>
	<ul>
	<li> It is indirect. </li>
	<li> It has a generation number of zero. </li>
	<li> It is not a stream. </li>
	<li> It has not been marked as incompressible by <code>CosObjSetCompressibility()</code>. </li>
	</ul>

	@param obj The object to test.
	@return <code>true</code> if <code>obj</code> is compressible, <code>false</code> otherwise. 
	@see CosObjIsCompressed 
	@see CosObjSetCompressibility 
	@since PI_COS_VERSION >= 0x00060000
*/
NPROC(ASBool, CosObjGetCompressibility, (CosObj obj))

/**
	Gets the number of objects in an object collection. The 
	size of a <code>NULL</code> collection is zero. 
	@param coll The object collection whose size is obtained.
	@return The number of objects in the collection. 
	@see CosObjAddToCollection 
	@see CosObjRemoveFromCollection 
	@see CosObjCollectionEnum 
	@since PI_COS_VERSION >= 0x00060000
*/
NPROC(ASUns32, CosObjCollectionSize, (CosObjCollection coll))

/**
	Tests whether two Cos object collections are the same collection. 
	Two <code>NULL</code> collections are always equal (a <code>NULL</code> collection 
	is not associated with a document and cannot store objects; 
	it is generally used only as an initial value for a variable 
	of type <code>CosObjCollection</code>). 
	@param c1 An object collection to compare.
	@param c2 An object collection to compare.
	@return <code>true</code> if <code>c1</code> and <code>c2</code> are the same collection, <code>false</code> otherwise. 
	
	@see CosNewObjCollection 
	@see CosObjGetCollection 
	@see CosObjCollectionIsNull 
	@since PI_COS_VERSION >= 0x00060000
*/
NPROC(ASBool, CosObjCollectionEqual, (CosObjCollection c1, CosObjCollection c2))

/**
	Enumerates the members of a Cos object collection, calling 
	a user-supplied procedure for each member object. The order 
	in which the objects are enumerated is undefined. 
	@param coll The object collection whose members are enumerated. 
	
	@param proc A user-supplied callback to call for each member 
	object of <code>coll</code>. Enumeration ends if <code>proc</code> returns <code>false</code>. 
	The callback must not modify the collection (for example, by adding or 
	removing objects). Doing so produces undefined 
	results or errors. 
	@param clientData A pointer to user-supplied data to pass 
	to <code>proc</code> each time it is called.
	@return Returns the value that <code>proc</code> returned (meaning that it returns <code>true</code> 
	if all the member objects were enumerated, <code>false</code> if enumeration 
	was terminated at the request of <code>proc</code>). 
	@see CosObjGetCollection 
	@since PI_COS_VERSION >= 0x00060000
*/
NPROC(ASBool, CosObjCollectionEnum, (CosObjCollection coll, CosObjEnumProc proc,
								    void *clientData))

/**
	<p>In Acrobat 6.0, this method
	updates an indirect Cos object after a linearized save operation. 
	Linearizing renumbers all indirect objects; this function 
	returns the new renumbered Cos object, which should be used 
	from this point on. This call is only valid from within 
	notification callbacks responding to the PDDocDidSave() notification. 
	If called from outside this context, or if the passed Cos 
	object is direct, the function does not modify the object.</p>

	<p>In Acrobat 7.0 and later, linearizing does not renumber objects,
	and this method has no effect.</p>
	
	@param obj A pointer to the object to refresh. The object 
	is updated by the method. 
	@param doc The document that was saved.
	@since PI_COS_VERSION >= 0x00060000
*/
NPROC(void, CosObjRefreshAfterLinearizedSave, (CosObj *obj, CosDoc doc))

/**
	<p>Tests whether the Cos document is fully compressed. In a 
	fully compressed document, most objects are stored in object 
	streams, which are normally Flate-encoded to reduce the 
	size of the PDF file. Cross-reference information for these 
	objects is stored in cross-reference streams, which are 
	also normally Flate-encoded. See the <i>PDF Reference</i>. </p>

	@note Fully compressed files are not compatible with PDF 
	1.4 and earlier viewers. 
	@param doc The document whose compression is checked.
	@return <code>true</code> if the document is fully compressed, <code>false</code> otherwise. 
	
	@see CosDocHasPartialCompression 
	@since PI_COS_VERSION >= 0x00060000
*/
NPROC(ASBool, CosDocHasFullCompression, (CosDoc doc))

/**
	<p>Tests whether the Cos document is partially compressed. 
	In a partially compressed file, the size of the logical 
	structure information is reduced; however, this information 
	is unavailable to pre-PDF 1.5 viewers, while the document 
	can still be viewed and printed. PDF 1.5 viewers (such as 
	Acrobat 6 and later) have full access to the structure information. </p>

	<p>In a partially compressed document, objects related to logical 
	structure are stored in object streams, which are normally 
	Flate-encoded to compress the document. Their cross-reference 
	information is stored twice: in a cross-reference stream, 
	to which there is a reference in the trailer of an update 
	section, and in the main cross-reference table, which indicates 
	that the objects are on the free list. See the <i>PDF Reference</i>. </p>
	
	@param doc The document whose compression is checked.
	@return <code>true</code> if the document is partially compressed, <code>false</code> otherwise. 
	
	@see CosDocHasFullCompression 
	@since PI_COS_VERSION >= 0x00060000
*/
NPROC(ASBool, CosDocHasPartialCompression, (CosDoc doc))

/**
	Acrobat 7 additions
*/

/**
    Creates a new 64-bit integer object associated with the specified 
    document and having the specified value. 
    @param dP IN The document in which the integer is used. 
    @param indirect IN If <code>true</code>, it creates the integer object as 
    an indirect object, and sets the document <code>dP</code> object's <code>PDDocNeedsSave</code> 
    flag (see PDDocFlags). If <code>false</code>, it creates the integer as 
    a direct object. 
    @param value IN The value, represented as a 64-bit integer.
    @return An object of type CosInteger.
    @see CosInteger64Value
	@see CosNewInteger
    @see CosNewFixed 
    @see CosNewFloat 
    @since PI_COS_VERSION >= 0x00070000
*/
NPROC(CosObj, CosNewInteger64, (CosDoc dP, ASBool indirect, ASInt64 value))

/**
	<p>Gets the 64-bit integer value of a specified number object. </p>

	<p>An exception is raised if the given object has the wrong Cos type. </p>

	@param obj The object whose integer value is obtained. 
	It must have type <code>CosInteger</code> or <code>CosReal</code> (<code>CosFixed</code>). If it is <code>CosReal</code>, 
	its value is rounded to the nearest integer. The result 
	is undefined if the real value is outside the range of <code>ASInt64</code>
	numbers.
	@return The 64-bit integer value of <code>obj</code>. 
	@see CosNewInteger64
	@see CosIntegerValue
	@see CosFixed Value
	@see CosFloatValue
	@since PI_COS_VERSION >= 0x00070000
*/
NPROC(ASInt64, CosInteger64Value, (CosObj obj))

/**
	Creates a new real-number object from a single-precision floating-point number 
	associated with the specified document. 
	@param dP The document in which the number is used. 
	
	@param indirect If <code>true</code>, it creates the real-number object 
	as an indirect object, and sets the document <code>dP</code> object's <code>PDDocNeedsSave</code> 
	flag (see <code>PDDocFlags</code>). If <code>false</code>, it creates the number 
	as a direct object. 
	@param value The real number, represented as a single-precision floating-point number.
	@return A Cos object of type <code>CosReal</code> (<code>CosFixed</code>).
	@see CosFloatValue 
	@see CosNewInteger
	@see CosNewInteger64
	@see CosNewFixed 
	@since PI_COS_VERSION >= 0x00070000
*/
NPROC(CosObj, CosNewFloat, (CosDoc dP, ASBool indirect, float value))

/**
	Gets the value of <code>obj</code> as a single-precision floating-point real number. 

	<p>An exception is raised if the given object has the wrong Cos type.</p>

	@param obj The object whose value is obtained. It must 
	have type <code>CosInteger</code> or <code>CosReal</code> (<code>CosFixed</code>). The result is undefined 
	if the real value is outside the range of floating-point numbers.
	@return The numeric value of <code>obj</code>, represented as a floating-point number. 
	@see CosNewFloat
	@see CosIntegerValue
	@see CosInteger64Value
	@see CosFixedValue
	@since PI_COS_VERSION >= 0x00070000
*/
NPROC(float, CosFloatValue, (CosObj obj))

/**
	Gets the value of the specified key in the specified dictionary. For more details, see <code>CosDictGet()</code>.

	@param dict The dictionary or stream from which a value is obtained. 
	@param key The key whose value is obtained, represented as a Cos name object. 
	@return The object associated with the specified key. If <code>key</code> is 
	not present, it returns a <code>NULL</code> Cos object. 
	@see CosDictGet
	@see CosDictGetKeyString
	@since PI_COS_VERSION >= 0x00070000
*/	
NPROC( CosObj, CosDictGetKey,	(CosObj dict, CosObj key))

/**
	Tests whether a specific key is found in the specified dictionary. 
	Calling this method is equivalent to checking if the value 
	returned from <code>CosDictGetKey()</code> is a <code>NULL</code> Cos object. For more details, see CosDictKnown().

	@param dict The dictionary or stream in which to look for <code>key</code>.
	@param key The key to find, represented as a Cos name object.

	@return <code>true</code> if the value of a key is known (exists and is not <code>NULL</code>) 
	in <code>dict</code>, <code>false</code> otherwise. 
	@see CosDictKnownKey
	@see CosDictKnownKeyString
	@since PI_COS_VERSION >= 0x00070000
*/
NPROC( ASBool, CosDictKnownKey,	(CosObj dict, CosObj key))

/**
	<p>Sets the value of a dictionary key, adding the key to the 
	dictionary if it is not already present. For more details, see <code>CosDictPut()</code>.</p>

	<p>It is not safe to call <code>CosDictPutKey()</code> during a call to <code>CosObjEnum()</code>
	on that same dictionary (for example, from within the callback procedure)</p>

	<p>An exception is raised if <code>val</code> is a direct non-scalar object 
	that is already contained in another dictionary, array, or stream,
    or if <code>dict</code> and <code>val</code> belong to different documents.</p>

	@param dict The dictionary or stream in which a value is set. 
	@param key The key whose value is set, represented as a Cos name object.

	@param val The value to set.
	
	@see CosDictPut
	@see CosDictPutKeyString
	@since PI_COS_VERSION >= 0x00070000
*/
NPROC( void,   CosDictPutKey,	(CosObj dict, CosObj key, CosObj val))

/**
	Removes a key-value pair from a dictionary. For more details, see <code>CosDictRemove()</code>.

	@param dict The dictionary from which the key-value pair 
	is removed. 
	@param key The key to remove, represented as a Cos name object.

	@see CosDictRemove 
	@see CosDictRemoveKeyString
	@since PI_COS_VERSION >= 0x00070000
*/
NPROC( void,   CosDictRemoveKey,(CosObj dict, CosObj key))

/**
	Gets the value of the specified key in the specified dictionary. For more details, see <code>CosDictGet()</code>.

	@param dict The dictionary or stream from which a value is obtained. 
	@param key The key whose value is obtained, represented as a string. 
	@return The object associated with the specified key. If key is 
	not present, returns a <code>NULL</code> Cos object. 
	@see CosDictGet
	@see CosDictGetKey
	@since PI_COS_VERSION >= 0x00070000
*/	
NPROC( CosObj, CosDictGetKeyString,		(CosObj dict, const char* key))

/**
	Tests whether a specific key is found in the specified dictionary. 
	Calling this method is equivalent to checking if the value 
	returned from <code>CosDictGetKeyString()</code> is a <code>NULL</code> Cos object. For more details, see <code>CosDictKnown()</code>.

	@param dict The dictionary or stream in which to look for key.
	@param key The key to find, represented as a string.

	@return <code>true</code> if the value of a key is known (exists and is not <code>NULL</code>) 
	in <code>dict</code>, <code>false</code> otherwise. 
	@see CosDictKnownKey
	@see CosDictKnownKeyString
	@since PI_COS_VERSION >= 0x00070000
*/
NPROC( ASBool, CosDictKnownKeyString,	(CosObj dict, const char* key))

/**
	<p>Sets the value of a dictionary key, adding the key to the 
	dictionary if it is not already present. For more details, see <code>CosDictPut()</code>.</p>

	<p>It is not safe to call <code>CosDictPutKey()</code> during a call to <code>CosObjEnum()</code>
	on that same dictionary (for example, from within the callback procedure).</p>

	<p>An exception is raised if <code>val</code> is a direct non-scalar object 
	that is already contained in another dictionary, array, or stream,
    or if <code>dict</code> and <code>val</code> belong to different documents.</p>

	@param dict The dictionary or stream in which a value is set. 
	@param key The key whose value is set, represented as a string.
	@param val The value to set.
	
	@see CosDictPut
	@see CosDictPutKey
	@since PI_COS_VERSION >= 0x00070000
*/
NPROC( void,   CosDictPutKeyString,		(CosObj dict, const char* key, CosObj val))

/**
	Removes a key-value pair from a dictionary. For more details, see <code>CosDictRemove()</code>.

	@param dict The dictionary from which the key-value pair is removed. 
	@param key The key to remove, represented as a string.

	@see CosDictRemove 
	@see CosDictRemoveKey
	@since PI_COS_VERSION >= 0x00070000
*/
NPROC( void,   CosDictRemoveKeyString,	(CosObj dict, const char* key))


/**
	<p><i>Weak</i> and <i>strong</i> references.</p>

	<p>When a Cos document is saved in full-save mode, objects that are
	not accessible from the root of the document are destroyed. This
	process uses a mark-and-sweep garbage collector: the root is marked,
	and then every object to which it refers is marked, and so on. At
	the end of this marking phase, objects that are not marked are
	destroyed.</p>

	<p>A so-called weak reference changes this policy: during the marking
	phase, a reference that has been declared to be weak will not be
	marked. For example, when a dictionary is marked, all its keys and
	values are normally also marked. But if a certain key has been set as a weak
	reference, then the corresponding value will not be marked. Consequently, if there
	are no other references to that value, it will be destroyed.</p>

	<p>A so-called strong reference also changes this policy, but in the
	opposite direction. An object for which there is a strong reference
	will be marked (and therefore will not be garbage-collected), even if
	there is no path to the object from the root of the document, and even
	if a weak reference exists for it.</p>

	<p><code>CosDictSetWeakReference()</code> establishes or removes a weak reference from a dictionary.</p>

	@param dict The dictionary containing the weak reference.
	@param key The name of a key in the dictionary.
	@param isWeak If <code>true</code>, the object stored in <code>dict</code> under <code>key</code> at the time
	of every subsequent full-save garbage collection will not be marked as a component of the 
	dictionary. If there is no
	other path to that object from the root of the document, then it will be garbage-
	collected (destroyed) by garbage collection.
	<p>It is not an error if there is no such value at the time of garbage collection or
	at the time of the call to this function.</p>

	<p>If <code>isWeak</code> is <code>false</code> (the default condition), then there is no such behavior,
	and the value, if any, will be marked in the normal manner. 
	The case where <code>isWeak</code> is specified as <code>false</code> is intended primarily to reverse the 
	effect of a previous call in which <code>isWeak</code> was <code>true</code>.</p>
	@since PI_COS_VERSION >= 0x00070000
*/
NPROC(void, CosDictSetWeakReference, (CosObj dict, const char* key, ASBool isWeak))

/**
	Gets the state of a weak reference. For details, see <code>CosDictSetWeakReference()</code>.
	@param dict A dictionary.
	@param key The name of a key.
	@return Returns the value of the <code>isWeak</code> parameter in the most recent call
	to CosDictSetWeakReference() with these parameters, or <code>false</code> if there has been
	no such call.
	@since PI_COS_VERSION >= 0x00070000
*/
NPROC(ASBool, CosDictIsWeakReference, (CosObj dict, const char* key))

/**
	Establishes or removes a weak reference from an array. For a description of weak references, see <code>CosDictSetWeakReference()</code>.

	@param array An array.
	@param n The index of the element that is the weak reference. Note
	that the weak reference <i>travels</i> with the element; that is, if
	an item is marked as a weak reference, and an item is subsequently
	inserted before that item, the weak reference applies to the same 
	element as it did previously.
	@param isWeak Sets a weak reference for an array.
	@since PI_COS_VERSION >= 0x00070000

*/
NPROC(void, CosArraySetWeakReference, (CosObj array, ASInt32 n, ASBool isWeak))

/**
	Return the state of a weak reference in an array. See <code>CosDictIsWeakReference()</code> for details.
	@param array An array.
	@param n The index of an item in the array.
	@return Returns the value of the <code>isWeak</code> parameter in the most recent call
	to <code>CosArraySetWeakReference()</code> with these parameters, or <code>false</code> if there
	has been no such call.
	@since PI_COS_VERSION >= 0x00070000
*/
NPROC(ASBool, CosArrayIsWeakReference, (CosObj array, ASInt32 n))

/**
	Create a strong reference for an object. For a description of strong references, see <code>CosDictSetWeakReference()</code>.

	<p>For indirect objects and direct nonscalars, <code>CosObjAcquire()</code> increments an internal
	reference count for <code>obj</code>. The reference count is used
	by the garbage collector, which is invoked during a full-save of the document. 
	If the reference count is positive at the time of garbage collection (it is initially <code>0</code>), then the object
	will not be garbage-collected, regardless of whether the object is accessible from
	the root of the document.</p>
	@param obj A Cos object.
	@since PI_COS_VERSION >= 0x00070000
*/
NPROC(void, CosObjAcquire, (CosObj obj))

/**
	<p>Removes a strong reference for an object. For a description of strong references, see <code>CosDictSetWeakReference()</code>.</p>

	<p>For indirect objects and direct nonscalars, <code>CosObjRelease()</code> decrements an internal
	reference count for <code>obj</code>. The reference count is used
	by the garbage collector, which is invoked during a full-save of the document. 
	If the reference count is positive at the time of garbage collection (it is initially <code>0</code>), then the object
	will not be garbage-collected, regardless of whether the object is accessible from
	the root of the document.</p>
	@param obj A Cos object.
	@since PI_COS_VERSION >= 0x00070000
*/
NPROC(void, CosObjRelease, (CosObj obj))

/**
	Creates a new name object associated with the specified 
	document and having the specified value. 
	@param dP The document in which the new name is used. 	
	@param indirect If <code>true</code>, it creates the name as an indirect 
	object, and sets the document's <code>PDDocNeedsSave</code> flag (see 
	<code>PDDocFlags</code>) flag. If <code>false</code>, it creates the name as a direct 
	object. 
	@param namestring The name to create.
	This routine will not create an <code>ASAtom</code> corresponding to <code>namestring</code>
	and is generally more efficient than <code>CosNewName()</code>. 
	(<code>ASAtom</code> objects consume global memory that is not deallocated.)
	@return The newly created name Cos object. 
	@see CosNewName
	@see CosNameValue
	@see CosCopyNameStringValue
	@see CosObjDestroy 
	@since PI_COS_VERSION >= 0x00070000
*/	
NPROC(CosObj, CosNewNameFromString, (CosDoc dP, ASBool indirect, const char* namestring))

/**
	<p>Returns a newly allocated buffer containing a copy of the 
	Cos object's name as a <code>NULL</code>-terminated string. Upon return, <code>nBytes</code> contains the 
	number of bytes in the string. <code>CosCopyNameStringValue()</code> 
	never returns <code>NULL</code>; it raises an exception if the allocation 
	fails. The client is responsible for freeing the result 
	by calling <code>ASfree()</code>. </p>

	<p>Unlike Cos strings, the strings corresponding to Cos names are
	<code>NULL</code>-terminated.</p>

	This routine will avoid creating an <code>ASAtom</code> corresponding to the object's
	name and is generally more efficient than copying the value returned by
	<code>ASAtomGetString(CosNameValue(obj))</code>. (<code>ASAtom</code> objects consume global memory that is not deallocated.)

	<p>An out-of-memory exception is raised if insufficient memory is available.</p>

	@param obj IN A Cos name object. 
	@param nBytes OUT The length of the name
	of the Cos object, and therefore the length of the returned
	string. <code>nBytes</code> may be <code>NULL</code> if you do not care 
	how many bytes are in the name.
	@return A copy of the Cos object's name, as a <code>NULL</code>-terminated string. 
	@see CosNewName
	@see CosNewNameFromString
	@see CosNameValue
	@see CosCopyStringValue
	@since PI_COS_VERSION >= 0x00070000
*/
NPROC(char *, CosCopyNameStringValue, (CosObj obj, ASTCount *nBytes))



/**
	Calls the specified procedure for each EOF in a given <code>CosDoc</code>.
	For details, see <code>CosDocEnumEOFs()</code>. This is the same as <code>CosDocEnumEOFs()</code>, 
	except that the callback proc takes a
	64-bit file position instead of a 32-bit file position.
	@param cosDoc The <code>CosDoc</code> in which the EOF's are enumerated. 
	
	@param proc The <code>CosDocEnumEOFsProc64()</code> to call for each EOF. 
	
	@param clientData A pointer to user-supplied data to pass 
	to <code>proc</code> each time it is called.
	@return <code>true</code> if all of the calls to <code>proc</code> return <code>true</code>, <code>false</code> as soon 
	as a call to <code>proc</code> returns <code>false</code>. 
	@see CosDocEnumIndirect 
	@see CosDocEnumEOFs
	@ingroup Enumerators
	@since PI_COS_VERSION >= 0x00070000
*/
NPROC(ASBool, CosDocEnumEOFs64, (CosDoc cosDoc, CosDocEnumEOFsProc64 proc, void * clientData))


/**
	<p>Tests whether the value of a Cos number is inside the range of 32-bit integers,
	<code>[-2147483648, +2147483647]</code>. </p>
	<p>If so, the 32-bit value may be obtained by calling <code>CosIntegerValue()</code>.</p>
	<p>If not, the 64-bit value may be obtained by calling <code>CosIntegerValue64()</code>.</p>

	<p>It raises an exception if <code>obj</code> is not a number (<code>CosInteger</code> or <code>CosReal</code>).</p>
	@param obj A Cos integer or real number.
	@return <code>true</code> if the value of the number is in the range of 32-bit integers,
	<code>false</code> otherwise.
	@see CosIntegerValue
	@see CosIntegervalue64
	@since PI_COS_VERSION >= 0x00070000
*/
NPROC(ASBool, CosNumberIsWithinASInt32Range, (CosObj obj))


/**
	<p>Tests whether the value of a Cos number is inside the range of <code>ASFixed</code> numbers,
	<code>[-32768.0, +32768.0)</code>. </p>
	<p>If so, the <code>ASFixed</code> value may be obtained by calling CosFixedValue().</p>
	<p>If not, the floating-point value may be obtained by calling CosFloatValue().</p>

	It raises an exception if <code>obj</code> is not a number (<code>CosInteger</code> or <code>CosReal</code>).
	@param obj A Cos integer or real number.
	@return <code>true</code> if the value of the number is in the range of <code>ASFixed</code>,
	<code>false</code> otherwise.
	@see CosFixedValue
	@see CosFloatValue
	@since PI_COS_VERSION >= 0x00070000
*/
NPROC(ASBool, CosNumberIsWithinASFixedRange, (CosObj obj))


/**
	Tests whether the definition of a specified Cos object, 
	in the file associated with the object's <code>CosDoc</code>, begins 
	within any of a set of byte ranges. For details, see <code>CosDocObjIsWithinRange()</code>.
	This is the same as <code>CosDocObjIsWithinRange()</code>, except that the byte ranges are
	64-bit file positions instead of a 32-bit file positions.

	<p>An exception is raised if <code>obj</code> is a direct object or <code>numEntries</code> is an odd number. </p>

	@param obj The Cos object (must be indirect). 
	@param byteRanges An array containing pairs of byte offsets 
	within the document. Each pair is a start and end offset 
	from the beginning of the document. 
	@param numEntries The number of byte offsets (not pairs) 
	in the <code>byteRanges</code> array.
	@return <code>true</code> if the object begins within any of the given ranges 
	and has not been modified, <code>false</code> otherwise. 
	@since PI_COS_VERSION >= 0x00070000
*/
NPROC(ASBool, CosDocObjIsWithinRange64, (CosObj obj, 
										 ASFilePos64 byteRanges[],
										 ASInt32 numEntries))

/**
	<p>Creates a new Cos stream, using data from an existing <code>ASStm</code>. For details, see <code>CosNewStream()</code>.</p>

	<p>This is the same as <code>CosNewStream()</code>, except that <code>decodeLength</code> is a
	64-bit value instead of a 32-bit value, and <code>allowDelayedRead</code> enables
	the implementation to avoid making an intermediate copy of the stream data.
	This is useful when creating very large streams of data.</p>

	@param dP The Cos document in which the newly created stream will be used.
	@param indirect Must always be <code>true</code>, specifying that the 
	Cos stream is created as an indirect object. 
	@param stm The source stream containing the data to copy 
	into the new stream. 
	@param stmStartPos Starting position for the stream.  Its default is <code>0</code>.
	@param stmDataIsDecoded A boolean value indicating whether the data in <code>stm</code> should 
	be encoded using filters specified in <code>attributesDict</code>. 
	@param attributesDict Either the <code>NULL</code> Cos object, or a 
	direct Cos dictionary containing stream attributes. 
	@param encodeParms The parameters to be used by the filters 
	if the source data is to be encoded. 
	@param sourceLength The amount of data to be read from the source.
	@param allowDelayedRead If this is
	<code>true</code> and <code>stm</code> permits seek operations, then the data from <code>stm</code> will not be read during
	this call, but rather at a subsequent time, and it may be read more
	than once. 
	
	<p><b>Important:</b> In this case, the caller must not close <code>stm</code> until it is established, through
	some independent mechanism, that the data will not be read again (see <code>ASProcStmRdOpenEx()</code> for further details on this feature).</p>

	<p>If <code>allowDelayedRead</code> is <code>false</code>, the source data is copied during this call,
	so the source stream may be closed after <code>CosNewStream64()</code> returns.</p>

	@return The newly created stream Cos object. 
	@see CosNewStream
	@see ASProcStmRdOpenEx
	@since PI_COS_VERSION >= 0x00070000
*/
NPROC(CosObj, CosNewStream64, (CosDoc dP,
							   ASBool indirect,
							   ASStm stm,
							   ASInt64 stmStartPos, 
                               ASBool stmDataIsDecoded,
							   CosObj attributesDict,
							   CosObj encodeParms,
							   ASInt64 sourceLength,
							   ASBool allowDelayedRead))

/**
	<p>Gets the length of a Cos stream from the <code>Length</code> key in the 
	stream's attributes dictionary. See <code>CosStreamLength()</code> for details.
	This is the same as <code>CosStreamLength()</code>, except that the return value is a
	64-bit integer instead of a 32-bit integer.</p>

	<p>This has the same effect as calling
	<code>CosInteger64Value(CosDictGetKeyString(stream, "Length"))</code></p>

	<p>An exception is raised if the Length key is not found in the attributes dictionary, or if its value is not an integer.</p>

	@param stream The stream whose length is obtained.
	@return The length of the stream. 
	@see CosStreamDict 
	@see CosStreamLength 
	@since PI_COS_VERSION >= 0x00070000
*/
NPROC(ASInt64, CosStreamLength64, (CosObj stream))

/**
	Gets the byte offset of the start of a Cos stream's data 
	in the PDF file. For details, see <code>CosStreamPos()</code>.
	This is the same as <code>CosStreamPos()</code>, except that the return value is a
	64-bit file position instead of a 32-bit file position.
	@param stream The stream whose current position is obtained.
	@return The byte offset of the start of the Cos stream's data in 
	the PDF file. 
	@exception cosErrInvalidObj is raised if the stream object has not yet 
	been saved to the PDF file. 
	@see CosStreamPos 
	@see CosStreamLength 
	@since PI_COS_VERSION >= 0x00070000
*/
NPROC(ASFilePos64, CosStreamPos64, (CosObj stream))

/**
	Creates a new real-number object from a double-precision floating-point number 
	associated with the specified document. 
	@param dP The document in which the number is used. 
	
	@param indirect If <code>true</code>, it creates the real-number object 
	as an indirect object, and sets the document <code>dP</code> object's <code>PDDocNeedsSave</code> 
	flag (see <code>PDDocFlags</code>). If <code>false</code>, it creates the number 
	as a direct object. 
	@param value The real number, represented as a double-precision floating-point number.
	@return A Cos object of type <code>CosReal</code> (<code>CosFixed</code>).
	@see CosDoubleValue
	@see CosFloatValue 
	@see CosNewFloat
	@see CosNewInteger
	@see CosNewInteger64
	@see CosNewFixed 
	@since PI_COS_VERSION >= 0x00090000
*/
NPROC(CosObj, CosNewDouble, (CosDoc dP, ASBool indirect, double value))

/**
	Creates a new real-number object from a double-precision floating-point number 
	associated with the specified document. 
	@param dP The document in which the number is used. 
	
	@param indirect If <code>true</code>, it creates the real-number object 
	as an indirect object, and sets the document <code>dP</code> object's <code>PDDocNeedsSave</code> 
	flag (see <code>PDDocFlags</code>). If <code>false</code>, it creates the number 
	as a direct object. 
	@param value The real number, represented as a double-precision floating-point number.
	@param value The maximum number of significant digits to use when this object is written to a file.
	Legal values are 6-13 for direct objects, 6-16 for indirect objects
	@return A Cos object of type <code>CosReal</code> (<code>CosFixed</code>).
	@see CosDoubleValue
	@see CosFloatValue 
	@see CosNewFloat
	@see CosNewInteger
	@see CosNewInteger64
	@see CosNewFixed 
	@since PI_COS_VERSION >= 0x00090000
*/
NPROC(CosObj, CosNewDoubleEx, (CosDoc dP, ASBool indirect, double value, ASUns8 numSigDigs))

/**
	<p>Gets the value of <code>obj</code> as a double-precision floating-point real number. </p>

	<p>An exception is raised if the given object has the wrong Cos type.</p>

	@param obj The object whose value is obtained. It must 
	have type <code>CosInteger</code> or <code>CosReal</code> (<code>CosFixed</code>). The result is undefined 
	if the real value is outside the range of floating-point numbers.
	@return The numeric value of <code>obj</code>, represented as a floating-point number. 
	@see CosNewFloat
	@see CosNewDouble
	@see CosFloatValue
	@see CosIntegerValue
	@see CosInteger64Value
	@see CosFixedValue
	@since PI_COS_VERSION >= 0x00090000
*/
NPROC(double, CosDoubleValue, (CosObj obj))

/**
	<p>Tests whether the supplied <code>CosDoc</code> contains as extensions to ISO 32000 (aka ISO PDF) </p>

	@param dP The Cos document to test.
	@return <code>true</code> if the file contains an ISO 32000 Extensions dictionary, <code>false</code> otherwise.  
	@see CosDocGetAdobeExtensionLevel
	@see CosDocSetAdobeExtensionLevel
	@since PI_COS_VERSION >= 0x00090000
*/
NPROC(ASBool, CosDocHasISOExtensions, (CosDoc dP))

/**
	<p>Tests whether the supplied <code>CosDoc</code> contains Adobe extensions to ISO 32000 (aka ISO PDF),
	and if so, returns the BaseVersion and ExtensionLevel</p>

	@param dP IN The Cos document to test.
	@param baseVersion OUT The PDF version on which the extensions are based (will be of type <code>CosName</code>).
	@param extension OUT The level of the extension expressed as a monotonically increasing integer.
	@return <code>true</code> if the file contains Adobe's ISO 32000 Extensions dictionary, <code>false</code> otherwise.  
	@see CosDocHasISOExtensions
	@see CosDocSetAdobeExtensionLevel
	@since PI_COS_VERSION >= 0x00090000
*/
NPROC(ASBool, CosDocGetAdobeExtensionLevel, (CosDoc dP, CosObj *baseVersion, ASUns32 *extension))

/**
	<p>Adds the necessary data structures to the supplied <code>CosDoc</code> to identify it as containing
	Adobe extensions to ISO 32000 (aka ISO PDF)</p>

	@param dP The Cos document to set.
	@param baseVersion The PDF version on which the extensions are based (will be of type <code>CosName</code>).
	@param extension The level of the extension expressed as a monotonically increasing integer.
	@see CosDocHasISOExtensions
	@see CosDocGetAdobeExtensionLevel
	@since PI_COS_VERSION >= 0x00090000
*/
NPROC(void, CosDocSetAdobeExtensionLevel, (CosDoc dP, CosObj baseVersion, ASUns32 extension))

