/**************************************************************************
*
* ADOBE CONFIDENTIAL
* ___________________
*
*  Copyright 2006 Adobe Systems Incorporated
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

#ifndef _ScXML_h
#define _ScXML_h

#if SC_NO_XML == 1
#error XML not supported in this static library configuration, please edit ScStaticLibDefs.h and recompile!
#endif

#include "ScRefcountable.h"
#include "ScHashTable.h"
#include "ScString.h"
#include "ScSimpleArray.h"
#include "ScError.h"
#include "ScPreConfig.h"

namespace ScCore
{

class XML;

/**
The XML iterator is used as a callback to iterate over the children of
a node.
@param		node			the current node
@param		userData		the user data as supplied to iterate()
@return						false if to stop iteration.
*/
typedef bool (*XMLIterator) (const XML& currentNode, void* userData);

/// Convenience constant: the value used for appending a node.
#define kXMLAtEnd			0x7FFFFFFF

/**
The XML class wraps XML trees. One node can be of several different types,
as described below. Only lists and element nodes can have children, and 
only element nodes can have attributes. All nodes that end up as a child
if a node receive that node as parent. If a child node is moved
to another parent, its parent entry is changed, which may lead to confusing
results if the node is left within the original parent. Nodes are orphaned
when removed from their parent.
<p>
Node names are local names. The namespace of a node is a kNamespace node
attached to a node. The high-level APIs get_r(), getText(), and put() accept
names that may contain the namespace prefix, a colon, and the local-name.
If no qualifier is given, lookups use the default namespace for kElement nodes, 
and no namespace for kAttribute nodes. For a lookup over all namespaces, use
the namespace prefix "*", like in *:element". For attributes, use the prefix '@'.
If using qualified names, attributes can either be "@ns:attr" or "ns:@attr";
also, "@*:attr" or "*:@attr" is OK. Xpath-like path names are supported in 
these APIs (use the slash character to separate the entries). A valid path 
would e.g. be "document/title/@xml:lang". Instead of a prefix, an URI can
be used by as a namespace designator by using a double colon. In that case,
paths cannot be used, since an URI may contain slashes. The qualified name 
"http://example.com/test::name" illustrates the problem.
<p>
Namespaces nodes can have undefined prefixes. For an undefined prefix, use
a question mark "?" when creating a Namespace node. The toString() and
toXMLString() method auto-generate prefixes for undefined namespace prefixes.
<p>
kList nodes are collections of XML nodes returned by most of the methods. 
They cannot be part of an XML tree; if they are added to the tree, their 
contents are added instead. List nodes do not have a parent. Lists are 
treated as element nodes if they contain only one element node.
<p>
The XML class is refcounted, which means that a parsed tree must be released
and not deleted. Several methods return fresh XML (mostly lists) that also
needs to be released, while the XML returned by several other methods must
not be released. To make life easier, all methods that return XML that must
be released end with "_r", like "get_r".
<p>
Note than none of the APIs check the given names for being valid XML syntax.
A static method checkSymbolSyntax() is available to check a symbol for containing
valid XML Unicode characters.
*/

class SC_API XML : public Refcountable
{
public:
	/// XML node types.
	enum Type
	{
		/**
		An element node can have attributes as well as children. In 
		the real world, an node corresponds to a tag. Only nodes
		and lists can have children.
		*/
		kElement	= 1,
		/**
		A list is returned by many methods. Lists may be empty, and a
		list containing one element node is often treated as if it was 
		a single element node. Only nodes and lists can have children.
		*/
		kList		= 2,
		/**
		Text nodes usually have a value only, but a text node can have the
		name "CDATA" if the text node is a CDATA node. The normalize() 
		method removes empty text nodes and concatenates adjacent text nodes.
		During conversion to a string, a text node whose name is CDATA creates
		a CDATA element instead of plain text.
		*/
		kText		= 3,
		/**
		Attributes have a name and a value. Only element nodes can have 
		attributes.
		*/
		kAttribute	= 4,
		/**
		A comment has a value only, like a text node.
		*/
		kComment	= 5,
		/**
		A processing instruction has a value only, like a text node. The
		XML definition instruction never shows up as a node.
		*/
		kProcessing	= 6,
		/**
		A namespace node can be found at element nodes. The name is the 
		prefix (the default namespace has the empty string as name), and 
		the value is the URI.
		*/
		kNamespace	= 7,
		__Type_32bit__	= 0x7FFFFFFF
	};
	/// XML access flags.
	enum
	{
		/// Include the current node(s), not only children.
		kAccessSelf		= 1,
		/// Do a deep access over all children, grandchildren etc.
		kAccessDeep		 = 2,
		/** Access all nodes without testing. This flag is used with the
		get() with a name, and is the same as if the name was empty or "*".
		When getting the children by type, the flag causes the type to be
		ignored. In short: the kAccessAll flag collects all children into a
		list node. */
		kAccessAll		= 4,
		/**
		Access attributes only. Same as using '@' as the first character of
		the local-name of the the string to find.
		*/
		kAccessAttr		= 8,
		/**
		Access all namespaces. Same as using "*" in a qualified name.
		*/
		kAccessAllNS	= 16,
		/// Reserved.
		kAccessReserved = 0xFF000000,
		__Flags_32bit__	= 0x7FFFFFFF
	};
	/**
	Get the default namespace URI. This value is thread local. The default
	namespace affects the namespace of kElement nodes and kAttribute nodes, 
	not the namespace of any other node.
	*/
		static	String			getDefaultNamespaceURI();
	/**
	Set the default namespace URI, returning the old value. This value is thread local.
	*/
		static	String			setDefaultNamespaceURI (const String& uri);
	/**
	Get the default namespace as an XML element. Must be released after use.
	*/
		static	const XML*		getDefaultNamespace_r();
	/**
	Get the current namespace filter. This is a thread-local Namespace element
	that is used as a filter in searches, or as the namespace to set (if the URI
	is not "") for new elements. The prefix may be "*" to indicate an undefined
	prefix. Namespace elements with undefined prefixes play a role when formatting
	output; the class will generate prefixes for Namespaces with undefined prefixes.
	a NULL value means to match elements without checking for a namespace. Using
	an empty URI means to explicitly check for elements with no namespace, and using
	"*" collects over all namespaces during a search. The return XML must be released 
	after use.
	*/
		static	const XML*		getCurrentNamespace_r();
	/**
	Set the current namespace. The XML's reference count is increased by one.
	Setting the current namespace to NULL means to address all namespaces during
	a lookup, or using the current XML namespace (if the URI is not "") during
	the creation of elements.
	*/
		static	const XML*		setCurrentNamespace_r (const XML* ns);
	/**
	Check the given symbol for containing valid Unicode characters as an XML symbol.
	The first character must be one of the Unicode typesLu, Ll, Lt, Lm, Lo, Nl, or 
	the underline characters. Subsequent characters must be one of the above types, 
	the Unicode type Nd, or a dot or a hyphen.
	*/
		static	bool			checkSymbolSyntax (const String& symbol);
	/**
	DEPRECATED. DO NOT USE.
	*/
	static	XML*				parse (const String& text, scint32_t& offset,
									   Error& error, const XML*,
									   bool stripWS = true, bool noComments = true, 
									   bool noProcessing = true);
	/**
	Parse a string and generate XML. Apply the given settings if supplied; 
	if not, use the default settings. The returned node is usually an element
	node. If the top level XML contains comments and/or PIs, and the processing
	of these elements is enabled, the returned XML is a list. To create an XML
	list, use the special XML tags "<></>". Note that if top-level comments or
	preprocessing instructions are parsed, the returned value is a kList node,
	not a kElement node.
	@param	text				the text to parse
	@param	offset				the offset to start parsing; the parser
								updates this position to the position after
								the XML
	@param	error				takes the complete error message
	@param	stripWS				strip whitespace from text
	@param	noComments			ignore comment tags
	@param	noProcessing		ignore processing instructions
	@return						the generated XML or NULL on errors
	*/
	static	XML*				parse (const String& text, scint32_t& offset, Error& error,
									   bool stripWS = true, bool noComments = true, 
									   bool noProcessing = true);
	/**
	Construct any type of XML.
	@param	type				the type
	*/
								XML (scint32_t type = kList);
	/**
	Construct an XML element (kElement) with the given name.
	@param	name				the element name
	*/
								XML (const String& name);
	/**
	Construct an XML node. If the node can have a name, the "name" argument is the
	name. If the node cannot have a name, the "name" argument is the value. The latter
	is true for kText and kComment nodes.
	@param	type				the type
	@param	name				the node name or value
	*/
								XML (scint32_t type, const String& name);
	/**
	Construct an element node having the given XML as children. If the XML
	is a list, all nodes are added.
	*/
								XML (const String& name, const XML& content);
	/**@name Accessors.
	*/
	//@{
		/// Return the parent.
		inline	const XML*		getParent() const { return mParent; }
		///
		inline	XML*			getParent() { return mParent; }
		/// Return the type.
		inline	scint32_t			getType() const { return mType; }
		/**
		Return the type as one of the strings list, node, text,
		comment, processing-instruction, attribute, or namespace.
		*/
				const char*		getTypeAsString() const;
		/**
		Return this node as an element node. This method returns the node
		itself if it is an element node. If this node is a list, and the 
		list contains one element node, it returns that node; otherwise,
		the call returns NULL.
		*/
				const XML*		asElementNode() const;
		///
				XML*			asElementNode();
		/**
		Return the node namespace. This method can return non-NULL for
		element and attribute nodes only. The returned value is an
		attribute node.
		*/
		inline	const XML*		getNamespace() const { return mNamespace; }
		/**
		Set the node namespace. If this node is not kElement or kAttribute,
		do nothing and return false. Note that this method does not add
		the namespace node to the list of declared namespaces. If this is
		desired, use insert() instead.
		@param	namespace		the namespace node
		@return					true if set
		*/
				bool			setNamespace (const XML* ns);
		/**
		Return the node name.
		*/
		inline	const String&	getName() const { return mName; }
		/**
		Set the node name. If this node is not kElement, do nothing 
		and return false.
		@param	name			the new name
		@return					true if set
		*/
				bool			setName (const String& name);
		/**
		Return the node value.
		*/
		inline	const String&	getValue() const { return mValue; }
		/**
		Set the node value. kElement and kList nodes cannot have a value.
		@param	value			the new value
		@return					true if set
		*/
				bool			setValue (const String& value);
		/**
		Get the number of children.
		*/
				scint32_t		length() const;
		/**
		Get a child by index. Only kElement and kList have children.
		*/
				XML*			getChild (scint32_t index) const;
		/**
		Iterate over the children of this node. If the iterator returns false,
		abort iteration and return false. Iterate over the attributes instead
		of the children if the flag kAccessAttr is set. Do a deep iteration if
		the flag kAccessDeep is set. In that case, call the callback first for the
		node, then for its children. If kAccessSelf is set, first call the callback
		on this node unless the node is a list.	
		Never remove a node inside this iterator!
		*/
				bool			iterate (XMLIterator iter, scint32_t flags = 0, void* userData = NULL) const;
	//@}
	/**@name Tests.
	*/
	//@{
		/**
		Return true if this node not an element node containing other element
		nodes.
		*/
			bool				isSimple() const;
		/**
		Return true if the given node is part of this tree. If the node is a list,
		all child nodes of the node are checked, and the result is true is at least
		one node is present. If identity is false, the isEqual() method is used
		to determine whether a node is contained. This allows for comparisons like
		xml.contains (new XML ("elem")) (forget about the refcounts in this example).
		*/
			bool				contains (const XML& xml, bool identity = true) const;
		/**
		Check this node for being equal to the given node by doing a deep compare.
		If ecma357 is true, a textual compare is made for attributes and text nodes
		following Ecma-357 (the other node's isSimple() must return true). Namespaces
		prefixes are ignored.
		*/
			bool				isEqual (const XML& xml, bool ecma357) const;
		/**
		Check this node for being equal to the given node by doing a deep compare.
		*/
			inline bool			operator == (const XML& xml) const { return isEqual (xml, false); }
		///
			inline bool			operator != (const XML& xml) const { return !isEqual (xml, false); }

	/**@name High-level APIs.
	Most of these methods work with node names, where the name is an 
	attribute if the first character is a '@'. The name can be a fully
	qualified name with a namespace prefix, like "xml:lang" or "ns:@*"
	(where the asterisk stands for all names in get() or erase() mode).
	Unqualified names match all namespaces, while qualified names only
	match the given namespace. 
	*/
	//@{
		/**
		Delete all nodes of the given name. Return the the index of the first 
		removed node, or -1 if no nodes were removed.
		If the string is empty, "@*", or "*", remove all nodes.
		*/
			scint32_t			remove (const String& name);
		/**
		Remove a node from this node. The node will be released.
		If the node is a list, remove all nodes in that list from
		their respective parents.
		*/
			void				remove (XML& xml);
		/**
		Find the first occurrance of the given node and returns its index.
		Return -1 if not found. If last is true, find the last occurrance.
		If the string is empty or "*" or "@*", return the first node found.
		*/
			scint32_t			indexOf (const String& name, bool last = false) const;
		/// Same, but for an XML node.
			scint32_t			indexOf (const XML& xml, bool last = false) const;
		/**
		Get all requested nodes into a list. This list may be empty if
		no node can be found. If this node is a list, all nodes
		are searched. If the string is empty or "*" or "@*", get all nodes.
		The string may contain a namespace prefix like "xml:lang" or "ns:*".
		If any of the path nodes except for the last given in a path is not 
		unique, the returned value is an empty list.
		@param	name			the name to find
		@param	flags			getter flags
		*/
			XML*				get_r (const String& name, scint32_t flags = 0) const;
		/// Same, but with a type instead of a name.
			XML*				get_r (scint32_t type, scint32_t flags = 0) const;
		/**
		Get the contents of an XML node as a Boolean value. This call accepts a 
		"/"-separated path of XML nodes, like "document/title/@id", and converts 
		the last node to a Boolean. The string must either be "true" or a non-zero
		number to return true. If any of the path nodes except for the last is not 
		unique, the returned value is false. 
		@param	path			the slash-separated path
		@return					the Boolean value of the last node
		*/
			bool				getBool (const String& path) const;
		/**
		Get the contents of an XML node as an integer number. This call accepts a 
		"/"-separated path of XML nodes, like "document/title/@id", and converts 
		the last node to a number. If the content is not entirely numeric, or if 
		any of the path nodes except for the last is not unique, the returned value is 0.
		@param	path			the slash-separated path
		@return					the numeric value of the last node
		*/
			scint32_t			getInteger (const String& path) const;
		/**
		Get the contents of an XML node as a floating-point number. This call accepts a "/"-separated 
		path of XML nodes, like "document/title/@id", and converts the last node to 
		a number. If the content is not entirely numeric, or if any of the path nodes 
		except for the last is not unique, the returned value is 0.
		@param	path			the slash-separated path
		@return					the numeric value of the last node
		*/
			double				getDouble (const String& path) const;
		/**
		Get the contents of an XML node as a string. This call accepts a "/"-separated 
		path of XML nodes, like "document/title/@id", and converts the last node to 
		a string using its toString() method. If any of the path nodes except for the 
		last is not unique, the returned value is the empty string.
		@param	path			the slash-separated path
		@return					the string contents of the last node
		*/
			String				getText (const String& path) const;
		/**
		Get a declared namespace. Use either the prefix, the URI, or both to compare.
		If neither prefix nor URI is given, NULL is returned. If local is false,
		do not search the parents, and do not search the kids. This method is the 
		only way to get the	default XML namespace "xml", because this namespace is 
		declared implicitly.
		*/
			const XML*			getNS (const String* prefix, const String* uri, bool local = false) const;
		/**
		Remove a namespace from this node and all child nodes. Usually,
		namespaces that refer to qualified names are not removed unless
		force is set to true; in that case, it will also strip a namespace 
		from qualified names. Both the prefix and the URI of the namespace 
		must match if the prefix is present. Returns true if the namespace
		was found within this tree.
		*/
			bool				removeNS (const XML& ns, bool force = false);
		/**
		Store the given XML as an element node with the given name. If the name
		starts with "@", set an attribute by converting the XML to text as the 
		attribute value. The name can be a "/"-separated path of XML element names,
		like "document/title/@id". If any of the path elements excepts for the 
		last is not unique, the put fails and return NULL. If any element does not
		exists, it is created. If the XML is a list, store the entire list under the
		given name. This call replaces all elements of that name (including namespace)
		and inserts the XML at the index of the first element found. If no element 
		was removed, append the XML. Insert element, text, and comment nodes, and convert
		everything else to text. Return false if the node could not be inserted.
		Any namespace declarations in the inserted nodes are removed if that
		namespace has already been declared in the tree above the insertion point.
		Note that it is not possible to create namespace declarations by using the
		string "@xmlns:name", because any namespaces must exist before using this
		method. The returned XML is the XML node having the given name.
		@param	name			the element name; can be a "/"-separated path
		@param	xml				the XML to put
		@param	release			set to true to release XML after the put
		*/
			XML*				put (const String& name, const XML& xml, bool release = false);
		/**
		Store the given string as the content of this node. For element
		nodes, replace the contents with a single text node; for attributes,
		set its value. As with the other put() method, the name can be a "/"-separated 
		path. If the value is the empty string, the contents of the elements are removed, 
		or an attribute value is set to the empty string.
		Note that it is not possible to create namespace declarations by using the
		string "@xmlns:name", because any namespaces must exist before using this
		method.
		@param	name			the element name; can be a "/"-separated path
		@param	value			the node value
		*/
			XML*				put (const String& name, const String& value);
		/// Convenience form: create or erase an empty element or attribute.
			inline XML*			put (const String& name) { return put (name, String::emptyString()); }
		/// Instead of replacing any found XML, append the given XML.
			XML*				add (const String& name, const XML& xml, bool release = false);
		/// Instead of replacing any found XML, append the given string.
			XML*				add (const String& name, const String& value);
		/// Convenience form: append an empty element or attribute.
			XML*				add (const String& name);
	//@}
	/**@name Axis methods.
	These methods create a collection of nodes. There are forward and backwards
	collections; backward ones are filled in reverse order. These methods reflect
	XPath axes. If "this" is a list, the returned lists are empty if "this"
	contains more than one element node. All lists must be released after use.
	*/
	//@{
		/**
		Get the top root node of this tree. This can either be an element node,
		or a list node. If this node does not have a parent, this node is returned.
		*/
			const XML&			root() const;
		///
			XML&				root();
			/// Get the parent(s) of this node as a list.
			XML*				parent_r() const;
		/// Get all descendants into a flattened list.
			XML*				descendants_r() const;
		/// Get all ancestors; add this node to the beginning if self is true (reverse collection).
			XML*				ancestors_r (bool self = false) const;
		/// Get all previous siblings of this node (reverse collection).
			XML*				prevSiblings_r() const;
		/// Get all following siblings of this node.
			XML*				nextSiblings_r() const;
		/// Get all previous nodes in the tree of this node (reverse collection).
			XML*				prevElements_r() const;
		/// Get all following siblings in the tree of this node.
			XML*				nextElements_r() const;
		/// Get the namespaces for this node.
			XML*				namespaces_r() const;
	//@}
	/**@name Low-level methods.
	These methods do not do any name resolution via a first '@' character.
	*/
	//@{
		/**
		Insert the given XML according to its type. Only list or element
		nodes access other nodes. Lists always insert the node as a child.
		The insertion of the node depends on its type:
		<ul>
		<li>kList - insert each node
		<li>kElement, kText, kProcessing, kComment - insert as a child
		<li>kAttribute - insert the attribute if this is an element node, replacing 
		    an attribute with the same name
		<li>kProcessing - insert if this node is the root, or an element node
		<li>kNamespace - insert into the namespaces array if not declared in scope yet
		</ul>
		It is possible to insert a single node multiple times. The parent of the
		node is always changed to this node. Any namespace declarations in the 
		inserted node are removed if that namespace has already been declared in 
		the tree above the insertion point. If release is true, the inserted XML is released.
		@param	xml				the XML to insert
		@param	pos				where to insert
		@param	release			if true, the inserted XML is released
		@return					true if inserted
		*/
			bool				insert (const XML& xml, scint32_t pos = kXMLAtEnd, bool release = false);
		/** 
		Insert a node as child. This is much the same as the insert() method, but
		if this node is an element node, nodes that usually could not be inserted
		(attributes, namespaces and processing instructions), their string value is 
		inserted as a text node instead. If release is true, the inserted XML is released.
		@param	xml				the XML to insert
		@param	pos				where to insert
		@param	release			if true, the inserted XML is released
		@return					true if inserted
		*/
				bool			insertChild (const XML& xml, scint32_t pos = kXMLAtEnd, bool release = false);
		/**
		Delete an node by index. The flags kAccessAttr determines
		what to erase: a child, or an attribute. Namespace nodes
		cannot be removed.
		*/
			void				remove (scint32_t at, scint32_t flags = 0);
	//@}
	/**@Other methods.
	*/
	//@{
		/**
		Execute the given XPath expression on the XML and return the result.
		If the XPath expression is faulty, return NULL and set the error. 
		If this node is a list, execute the expression on each kElement 
		node in this list and return a list containing the results in the 
		order of execution. If the XPath evaluation does not return
		a node-set, return NULL and set the supplied Variant.
		<p>
		This XPath engine supports XPath variables. By default, none are defined,
		but it is possible to supply a has table of strings that are name/value pairs.
		@param	expr			the XPath expression
		@param	err				takes an error message if supplied
		@param	variables		takes an optional hash table of name/value pairs
		@return					the resulting list, or NULL on errors or non-node-set results
		*/
			XML*				xpath_r (const String& expr, Variant& result,
									   Error& error,
									   const THashTable<String>* variables = NULL) const;
		/**
		Copy this XML. If deep is false, do not copy any kids, but always copy
		any attributes.
		*/
			XML*				copy_r (bool deep = true) const;
		/**
		Normalize this node by concatenating adjacent text nodes and removing empty ones,
		or text nodes that are whitespace only.
		*/
			void				normalize();
		/**
		Return the value of this node with leading and trailing XML whitespace (space,
		tab, CR, LF) stripped.
		@return					the stripped value
		*/
			String				getStrippedValue() const;
		/**
		Convert this XML to a string. Convert attributes and text nodes to their value.
		If the node is simple (isSimple() is true), concatenate all nodes except for
		comments and PIs; otherwise, call toXMLString() without prettyprinting.
		@param	indent			indent value to be used if calling toXMLString()
		*/
			String				toString (scint32_t indent = 0) const;
		/**
		Convert this XML to an XML string following the given settings. If there are no
		settings, the default settings are applied.
		@param	indent			indentation to use; 
		*/
			String				toXMLString (scint32_t indent = 2) const;
	//@}
private:
								XML (const XML&);
			XML&				operator= (const XML&);
	virtual						~XML();

	friend	class				XMLParser;
	typedef	TISimpleArray<XML>	XMLArray;
			String				mName;			// the local name
			String				mValue;			// any value
	const	XML*				mNamespace;
			XML*				mParent;
			XMLArray*			mKids;
			XMLArray*			mAttrs;
			XMLArray*			mNS;			// namespace declarations in this scope
			scint32_t			mType;			// see enumeration above

			void				get (const String& name, XML& list, const XML* ns, scint32_t flags) const;
			void				get (scint32_t type, XML& list, scint32_t flags) const;
	const	XML*				getNSTree (const String* prefix, const String* uri, bool kids) const;
			const XMLArray*		getArray (scint32_t flags) const;
	const	XML*				prepareName (String& name, scint32_t& flags) const;
			bool				testName (const String& name, const XML* ns, scint32_t flags) const;
	static	bool				arrayIsEqual (const XMLArray& a, const XMLArray& b, bool ecma357);
	static	bool				attrsAreEqual (const XMLArray& a, const XMLArray& b, bool ecma357);
			XML*				copy (const XML* inScopeDecls, const XML* parent, bool deep) const;
	static	XMLArray*			copyArray (const XML* parent, const XMLArray* arr);
			void				updateNS (const XML& oldNS, const XML& newNS);
			void				namespaces (XML& list, bool decls) const;
			void				nsToString (String& r, void* p, bool top) const;
			void				attrsToString (String& r, void* p) const;
			String				makeXMLString (void* p, bool top) const;
};

}	// end namespace

#include "ScPostConfig.h"

#endif
