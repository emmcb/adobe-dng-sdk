#ifndef __XMLParserAdapter_hpp__
#define __XMLParserAdapter_hpp__

// =================================================================================================
// Copyright 2005 Adobe
// All Rights Reserved.
//
// NOTICE:  Adobe permits you to use, modify, and distribute this file in accordance with the terms
// of the Adobe license agreement accompanying it. If you have received this file from a source other 
// than Adobe, then your use, modification, or distribution of it requires the prior written permission
// of Adobe.
// =================================================================================================

#if AdobePrivate
// =================================================================================================
// Change history
// ==============
//
// Writers:
//  AWL Alan Lillich
//  ADC Amandeep Chawla
//  IJS Inder Jeet Singh
//
// mm-dd-yy who Description of changes, most recent on top.
//
// 03-19-14 IJS 5.6-f098 [3709896] P2 Handler Modified to support spanned clip.
// 10-10-12 ADC 5.5-c012 Changed internal implementation of common error notification infrastructure.
// 08-08-12 AWL 5.5-c007 XMPCore error notifications for one case of XML parsing, no existing test failures.
//
// 01-16-08 AWL 4.2-c035 Add XML_Node::SetAttrValue.
// 12-18-07 AWL 4.2-c034 Add XML_Node::IsEmptyLeafNode and XML_Node::GetAttrValue.
// 12-10-07 AWL 4.2-c033 Save prefix length in XML_Node, change XML_Node::GetNamedElement to take URI and local name.
// 12-07-07 AWL 4.2-c032 Add serialization and tree dump for the exposed Expat parser.
// 11-30-07 AWL 4.2-c027 Expose XML_Node and ExpatAdapter so that XMPFiles can use them.
//
// 08-16-06 AWL 4.1-c015 [1312441,1340116] Change the parsing code to tolerate ISO Latin-1 input. At
//				the same time, get rid of the ugly hackery that tries to deal with unescaped ASCII
//				control characters. Also get rid of the check and complaint about parsing into an
//				empty object. This is predominantly what people want, so clear the XMPMeta object.
//
// 03-24-06 AWL 4.0-c001 Adapt for move to ham-perforce, integrate XMPFiles, bump version to 4.
//
// 04-06-05 AWL 3.2-013 [0509601] Normalize "single value" alt-text arrays. Improve the way the root
//				XML node is found and extract the previous toolkit version number.
// 03-31-05 AWL 3.2-007 Fix parsing for UTF-16 and UTF-32.
// 02-03-05 AWL Initial creation.
//
// =================================================================================================
#endif // AdobePrivate

#include "public/include/XMP_Environment.h"	// ! Must be the first #include!
#include "public/include/XMP_Const.h"

#include "source/XMP_LibUtils.hpp"

#include <string>
#include <vector>

// =================================================================================================
// XML_Node details
//
// The XML_Nodes are used only during the XML/RDF parsing process. This presently uses an XML parser
// to create an XML tree, then a recursive descent RDF recognizer to build the corresponding XMP.
// This makes it easier to swap XML parsers and provides a clean separation of XML and RDF issues.
// The overall parsing would be faster and use less memory if the RDF recognition were done on the
// fly using a state machine. But it was much easier to write the recursive descent version. The
// current implementation is pretty fast in absolute terms, so being faster might not be crucial.
//
// Like the XMP tree, the XML tree contains vectors of pointers for down links, and offspring have
// a pointer to their parent. Unlike the XMP tree, this is an exact XML document tree. There are no
// introduced top level namespace nodes or rearrangement of the nodes..
//
// The exact state of namespaces can vary during the XML parsing, depending on the parser in use.
// By the time the RDF recognition is done though, the namespaces must be normalized. All of the
// used namespaces must be registered, this is done automatically if necessary. All of the "live"
// namespace prefixes will be unique. The ns field of an XML_Node is the namespace URI, the name
// field contains a qualified name (prefix:local). This includes default namespace mapping, the
// URI and prefix will be missing only for elements and attributes in no namespace.

class XML_Node;

typedef XML_Node *	XML_NodePtr;	// Handy for things like: XML_Node * a, b; - b is XML_Node, not XML_Node*!

enum { kRootNode = 0, kElemNode = 1, kAttrNode = 2, kCDataNode = 3, kPINode = 4 };

#define IsWhitespaceChar(ch)	( ((ch) == ' ') || ((ch) == 0x09) || ((ch) == 0x0A) || ((ch) == 0x0D) )

typedef std::vector<XML_NodePtr>		XML_NodeVector;
typedef XML_NodeVector::iterator		XML_NodePos;
typedef XML_NodeVector::const_iterator	XML_cNodePos;

#if 0	// Pattern for iterating over the children or attributes:
	for ( size_t xxNum = 0, xxLim = _node_->_offspring_.size(); xxNum < xxLim; ++xxNum ) {
		const XML_NodePtr _curr_ = _node_->_offspring_[xxNum];
	}
#endif

class XML_Node {
public:

	// Intended for lightweight internal use. Clients are expected to use the data directly.
	
	XMP_Uns8		kind;
	std::string		ns, name, value;
	size_t          nsPrefixLen;
	XML_NodePtr		parent;
	XML_NodeVector	attrs;
	XML_NodeVector	content;
	
	bool IsWhitespaceNode() const;
	bool IsLeafContentNode() const;	// An empty element or one with a single character data child node.
	bool IsEmptyLeafNode() const;

	XMP_StringPtr GetAttrValue ( XMP_StringPtr attrName ) const;
	void SetAttrValue ( XMP_StringPtr attrName, XMP_StringPtr attrValue );

	XMP_StringPtr GetLeafContentValue() const;
	std::string* GetLeafContentPtr() const;
	void SetLeafContentValue ( XMP_StringPtr value );
	
	size_t CountNamedElements ( XMP_StringPtr nsURI, XMP_StringPtr localName ) const;	// Number of child elements with this name.
	XML_NodePtr GetNamedElement ( XMP_StringPtr nsURI, XMP_StringPtr localName, size_t which = 0 );
	
	void Dump ( std::string * buffer );
	void Serialize ( std::string * buffer );
	
	void RemoveAttrs();
	void RemoveContent();
	void ClearNode();

	XML_Node ( XML_NodePtr _parent, XMP_StringPtr _name, XMP_Uns8 _kind )
		: kind(_kind), name(_name), parent(_parent), nsPrefixLen(0) {};

	XML_Node ( XML_NodePtr _parent, const std::string & _name, XMP_Uns8 _kind )
		: kind(_kind), name(_name), parent(_parent), nsPrefixLen(0) {};

	virtual ~XML_Node() { RemoveAttrs(); RemoveContent(); };

private:

	XML_Node() : kind(0), parent(0)	{};	// ! Hidden to make sure parent pointer is always set.

};

// =================================================================================================
// Abstract base class for XML parser adapters used by the XMP toolkit.

enum { kXMLPendingInputMax = 16 };

class XMLParserAdapter {
public:

	XMLParserAdapter() : tree(0,"",kRootNode), rootNode(0), rootCount(0),
	                     charEncoding(XMP_OptionBits(-1)), pendingCount(0),
	                     errorCallback(0)
	{
		#if XMP_DebugBuild
			parseLog = 0;
		#endif
	};

	virtual ~XMLParserAdapter() {};
	
	virtual void ParseBuffer ( const void * buffer, size_t length, bool last ) = 0;
	
	virtual void SetErrorCallback ( GenericErrorCallback * ec )
		{ this->errorCallback = ec; };

	virtual void NotifyClient ( XMP_ErrorSeverity severity, XMP_Error & error )
	{
		if (this->errorCallback)
			this->errorCallback->NotifyClient( severity, error );
	}

	XML_Node		tree;
	XML_NodeVector	parseStack;
	XML_NodePtr		rootNode;
	size_t			rootCount;

	XMP_OptionBits	charEncoding;
	size_t          pendingCount;
	unsigned char	pendingInput[kXMLPendingInputMax];	// Buffered input for character encoding checks.

	GenericErrorCallback * errorCallback;	// Set if the relevant XMPCore or XMPFiles object has one.

	#if XMP_DebugBuild
		FILE * parseLog;
	#endif

};

// =================================================================================================

#endif	// __XMLParserAdapter_hpp__
