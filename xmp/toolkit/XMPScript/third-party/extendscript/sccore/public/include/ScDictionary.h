/**************************************************************************
*
* ADOBE CONFIDENTIAL
* ___________________
*
*  Copyright 1998 Adobe Systems Incorporated
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
#ifndef _ScDictionary_h
#define _ScDictionary_h

#include "ScClassInfo.h"
#include "ScLivePropertyManager.h"

namespace ScCore
{

class DictionaryData;
class XML;

/**
This struct allows LiveObject implementers to combine several LivePropertyInfo
tables to one single static table which is then used to define a basic DOM.
As with the type info in a LivePropertyInfo entry, both the group name and
the class name can have a short help text appended. This text should be a
one-liner, separated from the class or group names by a tab (\t or 0x09)
character. There is only one help text (LivePropertyInfo entries support
both long help and short help).
*/
struct LivePropertyInfos {
	/// The table to add; use NULL for the last entry.
	const LivePropertyInfo*		table;
	/// The class name; must not be NULL. Help text may be appended with a tab char.
	const char*					className;
	/// The group name; use NULL for no specific group. Help text may be appended with a tab char.
	const char*					groupName;
};

/**
The Dictionary class provides an object dictionary. An application can have
as many dictionaries as it wishes. A dictionary can be named, and exactly
one dictionary must be the default dictionary. Two static get() calls retrieve 
either the default dictionary, or a dictionary by name. The app creates and 
populates a Dictionary instance and calls set() to set this dictionary. Once 
a dictionary has been set, the dictionary belongs to the system and is deleted
on shutdown.
<p>
A dictionary may contain a list of sub-dictionaries which implement the 
methods getGroups(), getClasses() and getClassInfo() to implement different 
lookup mechanisms. The default implementation of these three methods first 
scans the sub-dictionaries and then adds its own information. The default 
implementation maintains tables of LivePropertyInfo entries as they usually 
are defined within LiveObject implementations. There is a method to add a 
single table, and a method to add a table of tables in a big chunk. 
Multiple tables can be used to define a single object class, because an 
object class may consist of separate LiveComponents.
<p>
The dictionary generates an XML TOC, where each group is a top-level entry,
and each class is a second-level entry underneath its group top-level entry.
The class and group names can have text attached to the names, separated
with a tab (\t or 0x09) character. This text appears in the TOC. The format
is described as the {map} tag in ES0019.
*/

class SC_API Dictionary : public Root
{
public:
	/**
	Get all registered dictionaries. The dictionaries are in a hsash table
	accessible by name.
	*/
	static	const THashTable<const Dictionary>&	getAll();
	/**
	Retrieve the application-defined default Dictionary instance. 
	The pointer to that instance belongs to ScCore.
	ScCore deletes the instance during shutdown. Therefore,
	the instance must be allocated on the heap.
	*/
	static	Dictionary*			get();
	/**
	Retrieve the named Dictionary instance.
	@param	name				the dictionary name
	*/
	static	Dictionary*			get (const String& name);
	/**
	Set a Dictionary instance. The pointer to that instance belongs 
	to ScCore. ScCore deletes the instance during shutdown. Therefore,
	the instance must be allocated on the heap. When setting a dictionary,
	any previous dictionary with the same name is deleted. You can only
	set one default dictionary.
	*/
	static	void				set (Dictionary* dict);

	/**
	The main dictionary constructor takes the language to search for. The dictionary name
	is the empty string, and it is the default dictionary.
	*/
								Dictionary (scint32_t language = kJavaScriptLanguage);
	/**
	The named dictionary constructor takes the name and the language to search for.
	If the string is empty, the dictionary is considered to be the main dictionary.
	If the name "JavaScript" is used, a Dictionary instance is created that contains
	information about all built-in JavaScript classes. Set isDefault to true to declare
	the dictionary to be the default dictionary. There can only be one default dictionary.
	*/
								Dictionary (const String& name, bool isDefault, scint32_t language = kJavaScriptLanguage);
	/**
	OBSOLETE. Please use the above constructor. If the name is the empty string, this
	is assumed to be the default dictionary.
	*/
								Dictionary (const String& name, scint32_t language = kJavaScriptLanguage);
	virtual						~Dictionary();
	/**@name Getting information */
	//@{
	/**
	Is this dictionary the default dictionary?
	*/
			bool				isDefault() const;
	/**
	Get the name of this dictionary.
	*/
			const String&		getName() const;
	/**
	Get the list of supported display groups. The strings are
	added to the given array. The caller must delete the array contents 
	by calling its deleteObjects() method. The base class implementation
	scans and combines the groups returned by any sub-dictionaries before
	adding any groups defined with LivePropertyInfo tables.
	<p>
	Each group can have a short one-line description. If there is one, the
	class name is followed by a tab character (\t or 0x09) and the description.
	A group in this context corresponds to the top-level entries of a TOC.
	A plug-in may e.g. choose to create its own group.
	@param	groups				receives the list of group names
	*/
	virtual	void				getGroups (TSimpleArray<const String>& groups);
	/**
	Retrieve all class names for a group of classes. The strings are added 
	to the given array. The caller must delete the array contents by calling 
	its deleteObjects() method. The base class implementation scans and 
	combines the classes returned by any sub-dictionaries before adding any 
	classes defined with LivePropertyInfo tables.
	<p>
	Each class can have a short one-line description. This may be the same
	info as returned by a ClassInfo instance, but it can also be different.
	The info is here because it may not make sense to create a full ClassInfo
	instance just to display a one-line help. If there is one, the
	class name is followed by a tab character (\t or 0x09) and the description.
	A trailing help is stripped from the group name. This information corresponds
	to the second level of a TOC.
	@param	group				the group name
	@param	classes				receives the list of class names
	*/
	virtual	void				getClasses (const String& group, TSimpleArray<const String>& classes);
	/**
	Retrieve the class info for a given class name. The returned pointer
	is reference counted. Call release() when done. NOTE: The pointer
	may point to a static instance, so do not store the pointer.
	The base class implementation scans the sub-dictionaries before
	scanning the info defined with LivePropertyInfo tables. First come,
	first served. A trailing help (separated by a tab character (\t or 0x09))
	is stripped from the class name.
	@param	className			the class name
	@return						the class info, or NULL if not found
	*/
	virtual	const ClassInfo*	getClassInfo (const String& className);
	/**
	Retrieve the dictionary TOC as XML formatted according to the technote
	ES0021 as a <map> element. If XML is not enabled, return NULL. 
	This method uses the above virtual methods to collect the XML, where
	group entries are the first TOC level, and class entries are the second
	TOC level. The "href" attribute values in the TOC are the class names with
	the given prefix at the beginning. This prefix should most often be
	"#/" to indicate classes in the same file and with no package name.
	Remember to call the XML's release() method to dispose of the memory.
	@param	prefix				the optional prefix for the hrefs
	@return						the TOC as XML, or NULL if XML is disabled
	*/
			XML*				toXML (const String* prefix = NULL);
	/**
	Return a class info as XML inside a <classdef> element as described in
	technote ES0021. This is a convenience method to call a ClassInfo's toXML()
	method. If XML is not enabled, return NULL. Remember to call the XML's release() 
	method to dispose of the memory.
	@param	className			the class name
	@return						the class info as XML, or NULL if not found or XML is disabled
	*/
			XML*				getClassXML (const String& className);
	//@}

	/**@name Sub-dictionaries */
	//@{
	/**
	Add a sub-dictionary. A sub-dictionary can be added multiple times,
	but only one pointer is kept. The sub-dictionary belongs to this
	dictionary, and it is deleted when this dictionary is deleted.
	*/
			void				addSubDictionary (Dictionary& r);
	/**
	Remove a sub-dictionary. If removed, the caller must delete the
	sub-dictionary.
	*/
			void				removeSubDictionary (Dictionary& r);
	//@}
	/**@name Adding PropertyInfo tables */
	//@{
	/**
	Add a table of LivePropertyInfo entries to the dictionary. Implementers 
	should register all of their tables using this API. Multiple tables can
	be registered for one class name; if e.g. a LiveComponent is added to
	an object. Also, a table can be registered with multiple groups.
	<p>
	The class and group names can have text attached to the names, separated
	with a tab (\t or 0x09) character. This text appears in the TOC.
	@param	table				the table to add
	@param	className			the class name of the object; must not be NULL;
								help text may be appended with a tab char
	@param	group				the group of the object; NULL for All;
								help text may be appended with a tab char
	*/	
	virtual	void				addTable (const LivePropertyInfo& table,
										  const char* className, 
										  const char* group = NULL);
	/**
	Add a table of class definitions to the dictionary. Each entry 
	of this table contains the static property table, the class
	name and an optional group name. The last entry contains NULL 
	in its entries. This method allows the definition of one 
	single table for a set of class definition tables.
	@param	tables				the list of tables
	*/
	virtual	void				addTables (const LivePropertyInfos* tables);
	//@}

	/**@name Stamping */
	//@{
	/**
	Return the date/time of the dictionary as an ISO string that the JavaScript Date
	object can parse. Implement this method to have an Object Model Viewer detect
	the date/time stamp of a dictionary so it can discard or update any older, cached
	dictionary. The default implementation returns an empty string for no date.
	*/
	virtual	String				getDateTimeStampAsISO() const;
	//@}

private:
			DictionaryData*		mData;
};

}	// end namespace

#endif
