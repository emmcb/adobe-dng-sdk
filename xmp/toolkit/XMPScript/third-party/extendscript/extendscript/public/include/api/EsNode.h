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
#ifndef _EsNode_h
#define _EsNode_h

#include "ExtendScript.h"
#include "ScPreConfig.h"

class jsParser;

namespace ScScript
{

struct ScanInfo;
class Node;
class UnaryNode;
class BinaryNode;
class TernaryNode;
class ListNode;
class LabelNode;
class ScopeNode;
class XMLConstantNode;
class Engine;
class ScopeInfo;

/**
The base node class contains a sID value that describes the type of the node.
An empty node is a placeholder for code branches not used, like a missing
else clause or a missing finally clause. The parser ensures that the tree
does not contain NULL pointers by supplying empty nodes. The line and column
information is set by the parser during construction with a ScanInfo data
block, which is internal. If lines or columns exceed 65535, the values are set 
to 65535.
*/

class ES_API Node : public ScCore::Root
{
	/// Private copy ctor, because this class has pointer data members.
						Node (const Node&);
	/// Private assignment operator, because this class has pointer data members.
	Node&				operator= (const Node&);

public:
	/// Each node contains additional info about its sID.

	enum IDInfo
	{
		/// This node is empty.
		kEmpty			= 0,
		/// No information.
		kNone			= 1,
		/// The node is a root node of a symbol. In a simple node, sID is a symbol.
		kSymbol			= 2,
		/// The sID is a property.
		kProperty		= 3,
		/// The sID is sID_EMPTY, and iValue contains the number.
		kNumericValue	= 4,
		/// The sID is sID_EMPTY, and iValue contains the string.
		kStringValue	= 5,
		/// The sID is sID_EMPTY, and iValue contains the RegExp pattern.
		kRegExpValue	= 6,
		/// The node contains function arguments.
		kParameters		= 7,
		/// The node contains index value(s).
		kIndex			= 8,
		/// The node is actually a label.
		kLabel			= 9,
		/// The node is an object descriptor { a:1, b:2 }. The ID is the class name.
		kObject			= 10,
		/// The node is an expression node, but the result should be discarded (var a=5 e.g.)
		kDiscard		= 11,
		/// The node is an XML constant node
		kXMLConstant	= 12,
		/// The node is an embedded XML expression, with element encoding
		kXMLElemExpr	= 13,
		/// The node is an embedded XML expression, with attribute value encoding and quoted
		kXMLAttrExpr	= 14,
		__IDInfo__32bit__	= 0x7FFFFFFF
	};

	/// The constant value of this node. NULL if this is not a constant.
	mutable	ScCore::Variant* iValue;
	/// The ID of this node. If sID_EMPTY, this is a placeholder.
	sID					iID;
	/// The siD of the source file name.
	sID					iFile;
	/// The line number of the source code.
	scint32_t			iLine;
	/// The starting column number of the source code.
	scint32_t			iColumn;
	/// The size of the source code representing the symbol.
	scint32_t			iLength;
	/// Additional info about the sID.
	IDInfo				iInfo;

						Node (const ScanInfo&);
	virtual				~Node();
	/**
	Is this a placeholder?
	*/
	inline	bool		isEmpty() const { return iInfo == kEmpty; }
	/**
	Get the constant value. If NULL, this is not a constant.
	*/
	inline	const ScCore::Variant* getConstant() const { return iValue; }
	/**
	Attempt to fold constants for this node and its descendants.
	This is implemented for all kinds of operators. The return
	value is either the node itself or a new node describing the
	constant. In the latter case, this node and its subtree is deleted.
	@return				either this node or a new node (this node is deleted if folded)
	*/
	virtual	Node*		foldConstants();
	/// Type casting without RTTI: To an unary node
	virtual	const UnaryNode*		toUnaryNode() const;
	/// Type casting without RTTI: To a binary node
	virtual	const BinaryNode*		toBinaryNode() const;
	/// Type casting without RTTI: To a ternary node
	virtual	const TernaryNode*		toTernaryNode() const;
	/// Type casting without RTTI: To a list node
	virtual	const ListNode*			toListNode() const;
	/// Type casting without RTTI: To a scope node
	virtual	const LabelNode*		toLabelNode() const;
	/// Type casting without RTTI: To a scope node
	virtual	const ScopeNode*		toScopeNode() const;

	/// Convert this node to a string to support automated testing.
	virtual	ScCore::String	toString() const;
	/// Define the dump() method in debug mode.
	virtual void		dump (Engine&, scint32_t level = 0);
};

/**
An empty node (sID == sID_EMPTY and iInfo == kEmpty).
*/

class ES_API EmptyNode : public Node
{
public:
						EmptyNode (const ScanInfo&);
};

/**
A simple unary node with one child.
*/

class ES_API UnaryNode : public Node
{
	/// Private copy ctor, because this class has pointer data members.
						UnaryNode (const UnaryNode&);
	/// Private assignment operator, because this class has pointer data members.
	UnaryNode&			operator= (const UnaryNode&);

public:
	Node*				iChild;
						UnaryNode (const ScanInfo&, Node* op);
	virtual				~UnaryNode();
	/**
	Attempt to fold constants for this node and its descendants.
	This is implemented for all kinds of operators. The return
	value is either the node itself or a new node describing the
	constant. In the latter case, this node and its subtree is deleted.
	@return				either this node or a new node (this node is deleted if folded)
	*/
	virtual	Node*		foldConstants();
	/// Type casting without RTTI: To an unary node
	virtual	const UnaryNode*	toUnaryNode() const;
	/// Convert this node to a string to support automated testing.
	virtual	ScCore::String	toString() const;
	/// Define the dump() method in debug mode.
	virtual void		dump (Engine&, scint32_t level = 0);
};

/**
A simple binary node.
*/

class ES_API BinaryNode : public Node
{
	/// Private copy ctor, because this class has pointer data members.
						BinaryNode (const BinaryNode&);
	/// Private assignment operator, because this class has pointer data members.
	BinaryNode&			operator= (const BinaryNode&);

public:
	Node*				iChild1;
	Node*				iChild2;
						BinaryNode (const ScanInfo&, Node* l, Node* r);
	virtual				~BinaryNode();
	/**
	Attempt to fold constants for this node and its descendants.
	This is implemented for all kinds of operators. The return
	value is either the node itself or a new node describing the
	constant. In the latter case, this node and its subtree is deleted.
	@return				either this node or a new node (this node is deleted if folded)
	*/
	virtual	Node*		foldConstants();
	/// Type casting without RTTI: To a binary node
	virtual	const BinaryNode*	toBinaryNode() const;
	/// Convert this node to a string to support automated testing.
	virtual	ScCore::String	toString() const;
	/// Define the dump() method in debug mode.
	virtual void		dump (Engine&, scint32_t level = 0);
};

/**
A ternary node has three child nodes.
*/

class ES_API TernaryNode : public Node
{
	/// Private copy ctor, because this class has pointer data members.
						TernaryNode (const TernaryNode&);
	/// Private assignment operator, because this class has pointer data members.
	TernaryNode&		operator= (const TernaryNode&);

public:
	Node*				iChild1;
	Node*				iChild2;
	Node*				iChild3;
						TernaryNode (const ScanInfo&, Node* n1, Node* n2, Node* n3);
	virtual				~TernaryNode();
	/**
	Attempt to fold constants for this node and its descendants.
	This is implemented for all kinds of operators. The return
	value is either the node itself or a new node describing the
	constant. In the latter case, this node and its subtree is deleted.
	@return				either this node or a new node (this node is deleted if folded)
	*/
	virtual	Node*		foldConstants();
	/// Type casting without RTTI: To a ternary node
	virtual	const TernaryNode*	toTernaryNode() const;
	/// Convert this node to a string to support automated testing.
	virtual	ScCore::String	toString() const;
	/// Define the dump() method in debug mode.
	virtual void		dump (Engine&, scint32_t level = 0);
};

/**
A list node is a list of several other nodes. List nodes are parameter lists, array
initializers and statement sequences. The iID member is initially sID_EMPTY, and
the iInfo is initially kNone.
*/

class ES_API ListNode : public Node
{
	friend class ::jsParser;
	ScCore::SimpleArray			iListArray;
	ScCore::TSimpleArray<Node>&	iList;
public:
						ListNode (const ScanInfo&);
	virtual				~ListNode();
	/// Get the number of nodes
	inline	scint32_t	length() const { return iList.length(); }
	/// Retrieve a node
	inline	Node*		getNode (scint32_t index) const { return iList [index]; }
	/// Type casting without RTTI: To a list node
	virtual	const ListNode*	toListNode() const;
	/**
	Attempt to fold a comma-separated expression into one constant.
	@return				either this node or a new node (this node is deleted if folded)
	*/
	virtual	Node*		foldConstants();
	/// Convert this node to a string to support automated testing.
	virtual	ScCore::String	toString() const;
	/// Define the dump() method in debug mode.
	virtual void		dump (Engine&, scint32_t level = 0);
};

/**
A label node contains a list of labels and a child node containing the labelled statement.
*/

class ES_API LabelNode : public Node
{
	/// Private copy ctor, because this class has pointer data members.
						LabelNode (const LabelNode&);
	/// Private assignment operator, because this class has pointer data members.
	LabelNode& 			operator= (const LabelNode&);

public:
	/// The label list.
	ScCore::SimpleArray	iLabels;
	/// The statement.
	Node*				iChild;
						LabelNode (const ScanInfo&);
	virtual				~LabelNode();
	/// Type casting without RTTI: To a scope node
	virtual	const LabelNode*	toLabelNode() const;
	/// Convert this node to a string to support automated testing.
	virtual	ScCore::String	toString() const;
	/// Define the dump() method in debug mode.
	virtual void		dump (Engine&, scint32_t level = 0);
};

/**
A scope node covers either a compilation unit or a function definition. It contains
scope info like variable names, and the body of the scriptlet or function, which is
a list node. A parser run always generates a scriptlet which may contain functions.
*/

class ES_API ScopeNode : public Node
{
	/// Private copy ctor, because this class has pointer data members.
						ScopeNode (const ScopeNode&);
	/// Private assignment operator, because this class has pointer data members.
	ScopeNode&			operator= (const ScopeNode&);

public:
	/// The scope info structure.
	ScopeInfo*			iInfo;
	/// The last line of the code. Useful for debugger opcodes.
	scint32_t			iLastLine;
	/// The function body
	Node*				iChild;

						ScopeNode (const ScanInfo&);
	virtual				~ScopeNode();
	/// Type casting without RTTI: To a scope node
	virtual	const ScopeNode*	toScopeNode() const;
	/// Convert this node to a string to support automated testing.
	virtual	ScCore::String	toString() const;
	/// Define the dump() method in debug mode.
	virtual void		dump (Engine&, scint32_t level = 0);
};

}	// end namespace

#include "ScPostConfig.h"

#endif

