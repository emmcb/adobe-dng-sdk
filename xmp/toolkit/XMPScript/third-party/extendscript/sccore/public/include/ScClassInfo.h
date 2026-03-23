/**************************************************************************
*
* ADOBE CONFIDENTIAL
* ___________________
*
*  Copyright 1998 - 2007 Adobe Systems Incorporated
*  All Rights Reserved.
*
* NOTICE:  All information contained herein is, and remains the property of
* Adobe Systems Incorporated  and its suppliers,  if any.  The intellectual 
* and technical concepts contained herein are proprietary to  Adobe Systems 
* Incorporated  and its suppliers  and may be  covered by U.S.  and Foreign 
* Patents,patents in process,and are protected by trade secret or copyright 
* law.  Dissemination of this  information or reproduction of this material
* is strictly  forbidden  unless prior written permission is  obtained from 
* Adobe Systems Incorporated.
* $File$
* $Author$
* $DateTime$
* $Revision$
* $Change$
**************************************************************************/

#if SC_ONCE
#pragma once
#endif
#ifndef _ScClassInfo_h
#define _ScClassInfo_h

#include "ScRefcountable.h"
#include "ScString.h"
#include "ScSimpleArray.h"
#include "ScVariantType.h"
#include "ScPreConfig.h"

namespace ScCore
{

class ClassInfo;
class ServiceInfo;
class ParamInfo;
class TypeInfo;
class Variant;
class FileSpec;
class XML;

/// Possible states of a class, method or property.

enum InfoState
{
	/** The item is visible and can be used from the 
	particular scripting language. */
	kNormalState			= 1,
	/// Reserved value. Will never be returned.
	kHiddenState			= 2,
	/** The object is not visible, but the item can be used
	from the particular scripting language. The item may be 
	removed in future versions of the object model. */
	kObsoleteState			= 3,
	/** The item is not visible, but it can be used from the 
	particular scripting language. */
	kInternalState			= 4,
	/// The value of the item is fixed and may be cached by the scripting system.
	kImmutableState			= 5,
	/** The class is an enumeration class. Only valid for classes.
	This state affects the way XML is generated. see toXML() for details.
	This value implies kNormalState. */
	kEnumerationState		= 6,
	__InfoState_32bit__		= 0x7FFFFFFF
};

/**
The ClassInfo is the data structure returned by the LiveObject method
getClassInfo(). It contains information about the class itself along
with an enumerate() call to actually enumerate the available services.
<p>
ClassInfo data as well as the data returned by other information
classes is language specific. You start off by asking a LiveObject
about its class info; there, you supply a language identifier so the 
LiveObject is able to generate language specific information. The ClassInfo 
instance is then able to return ServiceInfo instances that contain 
information about the services that a class is able to provide. 
A service is either a property or a method.
<p>
The Dictionary class returns ClassInfo instances from an internal static
dictionary. These ClassInfo instances (and the ServiceInfo instances
returned from that instance) may not support the APIs that accept or
return 32-bit property IDs. In any case, name-based APIs work.
<p>
You should call the release() method as soon as you are finished with
the object. This allows implementation to maintain the same object
during multiple calls, thus minimizing the usage of the application's
heap.
*/

class SC_API ClassInfo : public Refcountable
{
public:
	/**
	Get the name of the superclass of this class, if any.
	*/
	virtual	void				getSuperclass (String& name) const;
	/**
	Retrieve the language identifier that is valid for this information.
	The default returns kJavaScriptLanguage.
	*/
	virtual	scint32_t			getLanguage() const;
	/**
	Get the state of the item. The default returns kNormalState.
	*/
	virtual	InfoState			getState() const;
	/**
	Set the supplied string to the class name of the object, formatted to 
	fit the requested language. If the object does not have a class name, 
	clear the string.
	*/
	virtual	void				getName (String& name) const = 0;
	/**
	Return a descriptive text in the given string. If "longText" is false, 
	set the string to a short one-liner suitable for display in balloon help; 
	if it is true, return a thorough explanation. The default erases the string, 
	because there is no explanation for this.
	*/
	virtual	void				getDescription (bool longText, String& text) const;
	/**
	The selection bits for the enumerate() method allow you to fine-tune 
	the behavior of the enumerate() method.	The bits select what you want 
	enumerate() to return. The masks kEnumerateMethods, kEnumerateAllProperties 
	and kEnumerateReadWriteProperties determine what you want to have 
	returned in general. The default implementation of enumerate() interprets 
	these mask bits directly. The mask kEnumerateHiddenProperties adds hidden 
	services to the list, and the mask kEnumerateOverrideDontEnum also reports 
	services marked explicitly as not enumerable. The bits are the same as the
	enumerations defined in the LiveObject class.
	*/
	enum EnumerateMaskBits : scint32_t {
		kEnumerateMethodsBit				= 31,
		kEnumerateReadOnlyPropertiesBit		= 30,
		kEnumerateReadWritePropertiesBit	= 29,
		kEnumerateHiddenItemsBit			= 28,
		kEnumerateOverrideDontEnumBit		= 27,
		kEnumerateDontWalkProtoChainBit		= 26,
		kEnumerateIgnorePredefinedBit		= 25,
		kEnumerateStaticElementsBit			= 24,
		/// Enumerate only methods.
		kEnumerateMethods					= (1 << kEnumerateMethodsBit),
		/// Enumerate read-only properties.
		kEnumerateReadOnlyProperties		= (1 << kEnumerateReadOnlyPropertiesBit),
		/// Enumerate read-write properties.
		kEnumerateReadWriteProperties		= (1 << kEnumerateReadWritePropertiesBit),
		/// Enumerate also hidden items. Use with one or more of the above masks.
		kEnumerateHiddenItems				= (1 << kEnumerateHiddenItemsBit),
		/// Also force kAttrDontEnum properties to show up.
		kEnumerateOverrideDontEnum			= (1 << kEnumerateOverrideDontEnumBit),
		/// Dont walk the prototype chain for JavaScript objects; include only the immediate prototype.
		kEnumerateDontWalkProtoChain		= (1 << kEnumerateDontWalkProtoChainBit),
		/// Ignore predefined properties and methods.
		kEnumerateIgnorePredefined			= (1 << kEnumerateIgnorePredefinedBit),
		/// Enumerate static elements rather than instance elements.
		kEnumerateStaticElements			= (1 << kEnumerateStaticElementsBit),
		/// Enumerate all non-hidden properties.
		kEnumerateProperties				= (kEnumerateReadOnlyProperties 
											|  kEnumerateReadWriteProperties ),
		/// Enumerate all non-hidden properties and methods.
		kEnumeratePropertiesAndMethods		= (kEnumerateMethods 
											|  kEnumerateProperties ),
		/// Enumerate everything you can find.
		kEnumerateEverything				= (kEnumeratePropertiesAndMethods 
											|  kEnumerateHiddenItems
											|  kEnumerateOverrideDontEnum ),
	};
	/**
	Fill the supplied array with the IDs of all properties that this class 
	supports. You need to supply browse bits to specify the exact type of
	information. Dictionary-based ClassInfo instances may not support this
	API based on 32-bit APIs. In this case, the return value is kErrNotImplemented.
	@param	propertyIDs			the array to fill
	@param	browseBits			the browse bits
	@return						an error code (kErrOK for no error)
	@see	LiveBase
	*/
	virtual	scerror_t			enumerate (SimpleArray& propertyIDs, scint32_t browseBits) const;
	/**
	Fill the supplied array with the names of all properties that this class 
	supports. You need to supply browse bits to specify the exact type of
	information. Add kEnumerateStaticElements to enumerate static class elements.
	@param	propertyIDs			the array to fill; call deleteObjects() after use to release the strings
	@param	browseBits			the browse bits
	@return						an error code (kErrOK for no error)
	@see	LiveBase
	*/
	virtual	scerror_t			enumerate (TSimpleArray<const String>& serviceNames, scint32_t browseBits) const=0;
	/**
	Get the information about a specific service by	its property ID. 
	Remember to release the information once you are finished with it
	for best efficiency. Not all ClassInfo implementations returned by
	Dictionary implementations support property IDs, so the return value
	may be NULL. The returned instance may be static, so caching is not advised.
	@see Dictionary
	*/
	virtual	const ServiceInfo*	getServiceInfo (scint32_t id) const;
	/**
	Get the information about a specific service by	its name. 
	Remember to release the information once you are finished with it
	for best efficiency. The returned instance may be static, so caching 
	is not advised.
	*/
	virtual	const ServiceInfo*	getServiceInfo (const String& name) const = 0;
	/**
	Get the information about a specific static service by its property ID. 
	A static service is a service that appears to be part of the JavaScript
	constructor function, like e.g. Folder.current. Not all languages support
	static services. Remember to release the information once you are finished with it
	for best efficiency. Not all ClassInfo implementations returned by
	Dictionary implementations support property IDs, so the return value
	may be NULL. The returned instance may be static, so caching is not advised.
	Note that this API cannot be used to retrieve a constructor service, since
	a constructor method does not have an ID. Use the variant with a string
	instead, and supply the class name. The default returns NULL.
	@see Dictionary
	*/
	virtual	const ServiceInfo*	getStaticServiceInfo (scint32_t id) const;
	/**
	Get the information about a specific static service by its name. 
	A static service is a service that appears to be part of the JavaScript
	constructor function, like e.g. Folder.current. Not all languages support
	static services. To get the constructor service, use the same name as
	the class name.	This service may not be present for non-creatable classes.
	Remember to release the information once you are finished with it for best 
	efficiency. The returned instance may be static, so caching 
	is not advised. The default returns NULL.
	*/
	virtual	const ServiceInfo*	getStaticServiceInfo (const String& name) const;
	/**
	Return sample code for this class. If there is no sample code, the string is empty
	and the method returns false.
	*/
	virtual	bool				getSampleCode (String& code) const;
	/**
	Return a file specifier to sample code for this class. If there is no sample 
	code, the file specifier is left untouched and the method returns false.
	*/
	virtual	bool				getSampleCode (FileSpec& file) const;
	/**
	Return this ClassInfo as XML inside a <classdef> element as described in
	technote ES0021. If XML is not enabled, return NULL. This method uses 
	the virtual methods above to gather the required information. Remember 
	to call the XML's release() method to dispose of the memory. If the
	class' state is kEnumerationState, an <enumeration> element is generated.
	*/
			XML*				toXML() const;
};

/**
What kind of info is this? The values kInvalid and kRemove are actually
part of the LiveObject interface. They are never returned by any getType()
call of ClassInfo, ServiceInfo or ParamInfo.
*/

enum InfoType
{
	/// This is a read-only property.
	kReadOnly					= 1,
	/// This is a read-write property.
	kReadWrite					= 2,
	/// This is a property that is only valid during creation. AppleScript only.
	kCreateOnly					= 3,
	/// This is a method.
	kMethod						= 5,
	/// This is both a method and a read-write property.
	kMethodAndReadWrite			= 6,
	/// This is both a method and a read-only property.
	kMethodAndReadOnly			= 7,
	/**
	This value is used at the LiveObject interface if type information
	about an unknown property ID is requested. It is never returned here.
	*/
	kInvalid					= 16,
	/** 
	This value is used at the LiveComponent interface. If the getType()
	call on a LiveComponent returns kRemove, it actually removes the property
	from a previous implementation. It is never returned here.
	*/
	kRemove						= 17,
	__InfoType_32bit__			= 0x7FFFFFFF
};

typedef InfoType InspectInfoType;

/**
The TypeInfo class describes the data type of a service or parameter. 
It contains information about the basic data type. It also tells you 
whether a returned object actually is a collection, or whether a 
certain numeric range is valid as input.
*/

class SC_API TypeInfo : public Root
{
public:
	/**
	Return the expected data type/return value of the item. The return value
	should be kUnknownType if the type cannot be determined, kAnyType if the 
	type is determined at runtime, or kUndefined if a method does not return
	a value.
	*/
	virtual	VariantType			getDataType() const = 0;
	/**
	Helper method to return the data type as a string. This method returns
	one of the following data types: undefined, null, boolean, number, string,
	point, rect, array, regexp, unitvalue. For objects, the class name is 
	returned if getDataClass() returns anything, otherwise "object" is returned.
	*/
	virtual	void				getDataTypeAsString (String& toFill) const;
	/**
	Static helper method to return the data type as a string. This method returns
	one of the following data types: undefined, null, boolean, number, string,
	object, point, rect, array, regexp, unitvalue. 
	*/
	static	const char*			convertDataTypeToString (VariantType type);
	/**
	Return the class name for an expected returned object in the supplied string. 
	For primitive data, erase the string (which is the default here).
	*/
	virtual	void				getDataClass (String& toFill) const;
	/**
	Does this type specify a collection? The default says false.
	*/
	virtual	bool				isCollection() const;
	/**
	Return the range of valid inputs. If there is no range, set min to -Infinity 
	(optional) and max to +Infinity (optional) and return false. If there is no 
	minimum, set min to -Infinity, max to the maximum and return true. If there 
	is no maximum, set min to the minimum and max to +Infinity and return true.
	The default sets both to (-)Infinity and returns false.
	@param	min					the minimum (-Infinity if there is none)
	@param	max					the maximum (+Infinity if there is none)
	@return						false if there is no range defined
	*/
	virtual	bool				getValidRange (double& min, double& max) const;
	/**
	Return the default value. Useful to create UI where the default value is preset.
	The default implementation sets the value to undefined and returns false.
	@param	value				the value to be set
	@return						false if there is no default value
	*/
	virtual	bool				getDefaultValue (Variant& value) const;
	/**
	Return a short text describing the data type. In the OMV viwer, this text will replace
	the data type text. Instead of "Data type: any", the viewer would display "Data type:
	Array in the morning and Date in the afternoon" if that would be the description.
	*/
	virtual	void				getDescription (String& text) const;
	/**
	Parse a string into a TypeInfo instance. This method is used to parse argument descriptions
	in property tables. The string is a comma-separated list of values. The first value is the
	data type of a property, or the return type of a method. Other elements describe each 
	argument. Each parameter description starts with the name, followed by a colon (this is 
	optional, however; if no name is given, the name is "argX", where X is an index). The data 
	type is a multiple-element entity, separated by semicolons. The first element is required,
	and this is the data type. This should be one of "undefined","boolean","number","string" 
	or "Object" for generic objects. The data type can also be the class name of an object, 
	like "Document" or "Array" (note the initial upper case letter). If the data can be any 
	type, use the word "any". Other elements are the minimum value, the maximum value, the
	default value, and "true" if the data type is an array of elements. Examples: a number 
	between -3 and 3 with a default of 0 would be specified as "number;-3;3;0;false" or 
	"number;-3;3;0;". A Boolean having a default value of "true" would be "boolean;;;;true".
	If not present, the data type for properties, methods and parameters is assumed as "any".
	<p>
	The return type is optional. If missing, "any" is assumed. An example: the data information 
	for String.indexOf() would be "number,what:string",	or shorter "number,string". For type info
	about both a property and a method, the return type can be two strings, separated by a slash 
	character. An example would be XML.length(), which would return XML as property and a number
	as a function: "XML/number". You would want to supply a pointer to a second TypeInfo pointer
	where the second TypeInfo instance would be stored.
	<p>
	Help information can be appended with a tab character, followed by the short help text, and 
	optionally, a second tab character, followed by the long help text. 
	Example: "mode:string\tOpen mode", or  "mode:string\tMode\tThe open mode".
	@param	r					the string to parse
	@param	index				>= 0 for method arguments
	@param	namePtr				takes the name of the method argument if supplied
	@param	shortHelp			takes the short help if supplied
	@param	longHelp			takes the long help if suppplied
	@param	type2				takes a second TypeInfo if the index is -1 and a slash was found
	@return						a new TypeInfo element (delete it after use)
	*/
	static	TypeInfo*			parse (const String& r, scint32_t index=-1, String* namePtr = NULL,
									   String* shortHelp = NULL, String* longHelp = NULL,
									   TypeInfo** type2 = NULL);
};

/**
The ServiceInfo class contains a number of methods to access 
information about a property or method. It is reference counted, 
so call release() as soon as possible for optimal performance.
If the ServiceInfo describes a property as well as a method,
the getTypeInfo() method describes the function return value.
*/

class SC_API ServiceInfo : public Refcountable
{
public:
	/**
	Retrieve the language identifier that is valid for this information.
	The default returns kJavaScriptLanguage.
	*/
	virtual	scint32_t			getLanguage() const;
	/**
	Get the state of the item. The default returns kNormalState.
	*/
	virtual	InfoState			getState() const;
	/**
	Set the supplied string to the element name of this information, formatted to 
	fit the requested language.
	*/
	virtual	void				getName (String& name) const = 0;
	/**
	Return a descriptive text in the given string. If "longText" is false, 
	set the string to a short one-liner suitable for display in balloon help; 
	if it is true, return a thorough explanation. The default erases the string, 
	because there is no explanation for this.
	*/
	virtual	void				getDescription (bool longText, String& text) const;
	/**
	Get the LiveObject property ID for this service. This call must return 
	the unique 32-bit ID of the element that is used to invoke the corresponding 
	functionality in a LiveObject. The value is needed to generate ActionScript 
	header files.
	*/
	virtual	scint32_t			getID() const = 0;
	/**
	Get the type of the service. The values kInvalid and kRemove are never returned.
	*/
	virtual	InfoType			getType() const = 0;
	/**
	Get the type information about the data that this service returns.
	If the ServiceInfo describes a property as well as a method,
	the getTypeInfo() method describes the function return value.
	Use the getTypeInfo2() method to get the type information for
	the property definition.
	*/
	virtual	const TypeInfo&		getTypeInfo() const = 0;
	/**
	Return the type info for a property definition if this instance
	describes a property as well as a method. The default calls
	getTypeInfo() and return that result.
	*/
	virtual	const TypeInfo&		getTypeInfo2() const;
	/**
	Get the number of parameters. For properties, return 0 (which is the default).
	*/
	virtual	scint32_t 			getParamCount() const;
	/**
	Get the info for the nth parameter. Use index values between 0 and 
	getParamCount()-1. Remember to release the ParamInfo data after use
	for best efficiency. The default returns null. The returned instance
	may be static, so caching is not advised.
	*/
	virtual	const ParamInfo*	getParamInfo (scint32_t index) const;
	/**
	Return sample code for this class. If there is no sample code, the string is empty
	and the method returns false.
	*/
	virtual	bool				getSampleCode (String& code) const;
	/**
	Return a file speficier to sample code for this class. If there is no sample 
	code, the file specifier ls left untouched and the method returns false.
	*/
	virtual	bool				getSampleCode (FileSpec& file) const;
	/**
	Return this ServiceInfo as an XML structure inside a <method> or <property> 
	tag as described in technote ES0021. If XML is not enabled, return NULL. 
	This method uses the virtual methods above to gather the required information. 
	Remember to call the XML's release() method to dispose of the memory.
	If the ServiceInfo describes a property as well as a method, this method
	returns a two-element XML list, where the first element describes the
	property, and the second element describes the method.
	*/
			XML*				toXML() const;
};

/**
The ParamInfo class returns information about a method parameter.
It is reference counted, so call release() as soon as possible for 
optimal performance.
*/

class SC_API ParamInfo : public Refcountable
{
public:
	/**
	Retrieve the language identifier that is valid for this information.
	The default returns kJavaScriptLanguage.
	*/
	virtual	scint32_t			getLanguage() const;
	/**
	Get the state of the item. The default returns kNormalState.
	*/
	virtual	InfoState			getState() const;
	/**
	Set the supplied string to the parameter name, formatted to 
	fit the requested language. 
	*/
	virtual	void				getName (String& name) const;
	/**
	Return a descriptive text in the given string. If "longText" is false, 
	set the string to a short one-liner suitable for display in balloon help; 
	if it is true, return a thorough explanation. The default erases the string, 
	because there is no explanation for this.
	*/
	virtual	void				getDescription (bool longText, String& text) const;
	/**
	Get the type information about the data that this parameter contains.
	*/
	virtual	const TypeInfo&		getTypeInfo() const = 0;
};

}	// end namespace

#include "ScPostConfig.h"
#endif

