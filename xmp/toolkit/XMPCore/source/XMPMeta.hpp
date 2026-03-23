#ifndef __XMPMeta_hpp__
#define __XMPMeta_hpp__

// =================================================================================================
// Copyright 2003 Adobe
// All Rights Reserved.
//
// NOTICE:	Adobe permits you to use, modify, and distribute this file in accordance with the terms
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
//  FNO Frank Nocke
//  ADC Amandeep Chawla
//
// mm-dd-yy who Description of changes, most recent on top.
//
// 10-10-12 ADC 5.5-c012 Changed internal implementation of common error notification infrastructure.
// 09-21-12 AWL 5.5-c011 Remove Transform XMP.
// 08-14-12 AWL 5.5-c009 Add XMPCore error notifications for RDF parsing errors.
// 08-08-12 AWL 5.5-c007 XMPCore error notifications for one case of XML parsing, no existing test failures.
// 08-03-12 AWL 5.5-c006 Remove defunct XMPMeta prevTkVer data member.
// 08-02-12 AWL 5.5-c005 Implement the internal infrastructure for XMPCore error notifications.
// 08-01-12 AWL 5.5-c004 Implement public API and glue layer for XMPCore error notifications.
//
// 07-16-09 AWL 5.0-c046 Fix XMPDocOps::Clone to not copy all of the thread lock state.
// 06-11-09 AWL 5.0-c034 Finish threading revamp, implement friendly reader/writer locking.
// 05-27-09 AWL 5.0-c033 Remove XMPMeta::SendAssertNotify.
// 05-12-09 AWL 5.0-c029 Finish deprecated function removal.
// 02-16-09 FNO 5.0-c008 [1647989] Adding 3rd patch by H. Figuiere: adding delete-localized-text function.
//
// 02-28-08 AWL 4.2-c046 Add SXMPMeta::Erase.
// 11-07-07 AWL 4.2-c025 More progress implementing XMPDocOps.
// 08-27-07 AWL 4.2-c020 Add Sort, change the Dump* routines to hexify non-ASCII characters.
//
// 10-12-06 AWL 4.1-c021 [1235816] Remove the new/delete overrides from static builds.
//
// 03-24-06 AWL 4.0-c001 Adapt for move to ham-perforce, integrate XMPFiles, bump version to 4.
//
// 05-16-05 AWL 3.3-100 Complete the deBIBification, integrate the internal and SDK source. Bump the
//				version to 3.3 and build to 100, well ahead of main's latest 3.3-009.
//
// 04-14-05 AWL 3.2-018 Move the padding param, add overloads to simplify use of SerializeToBuffer.
// 04-06-05 AWL 3.2-013 [0509601] Normalize "single value" alt-text arrays. Improve the way the root
//				XML node is found and extract the previous toolkit version number.
// 02-11-05 AWL 3.2-002 Add client reference counting.
// 01-28-05 AWL 3.2-001 Remove BIB.
//
// 01-26-05 AWL 3.1.1-107 [1141684] Add XMPMeta::UnregisterAssertNotify and XMPMeta::SendAssertNotify.
// 01-25-05 AWL 3.1.1-106 [1141007] Add XMPMeta::RegisterAssertNotify.
// 10-20-04 AWL 3.1.1-085 [1084185] Fix XMP_InternalRef to not depend on BIBContainerBase layout.
//
// 04-30-04 AWL Add new & delete operators that call BIBMemory functions. Change static objects that
//				require allocation to explicit pointers.
// 01-29-04 AWL Add AppendArrayItem.
// 04-24-03 AWL Initial start on the new implementation.
//
// =================================================================================================
#endif /* AdobePrivate */

#include "public/include/XMP_Environment.h"
#include "public/include/XMP_Const.h"
#include "XMPCore/source/XMPCore_Impl.hpp"
#include "source/XMLParserAdapter.hpp"

// -------------------------------------------------------------------------------------------------

#ifndef DumpXMLParseTree
	#define DumpXMLParseTree 0
#endif

extern XMP_VarString * xdefaultName;	// Needed in XMPMeta-Parse.cpp, MoveExplicitAliases.

class XMPIterator;
class XMPUtils;

// -------------------------------------------------------------------------------------------------

class XMPMeta {
public:

	static void
	GetVersionInfo ( XMP_VersionInfo * info );
	
#if ! AdobePrivate
	static bool
	Initialize();
#else
	static bool
	Initialize ( XMP_AllocateProc AllocateProc,
                 XMP_DeleteProc   DeleteProc );
#endif

	static void
	Terminate() RELEASE_NO_THROW;

	// ---------------------------------------------------------------------------------------------

	XMPMeta();
	
	virtual ~XMPMeta() RELEASE_NO_THROW;

	// ---------------------------------------------------------------------------------------------
	
	static XMP_OptionBits
	GetGlobalOptions();
	
	static void
	SetGlobalOptions ( XMP_OptionBits options );

	// ---------------------------------------------------------------------------------------------

#if AdobePrivate
	static void
	RegisterAssertNotify ( XMP_AssertNotifyProc	notifyProc,
						   void *				refCon );

	static void
	UnregisterAssertNotify ( XMP_AssertNotifyProc notifyProc );

	// ---------------------------------------------------------------------------------------------
#endif
	
	static XMP_Status
	DumpNamespaces ( XMP_TextOutputProc outProc,
					 void *				refCon );
	
	#if AdobePrivate
	static XMP_Status
	DumpPropertyTraits ( XMP_TextOutputProc outProc,
						 void *				refCon );
	#endif
	
	// ---------------------------------------------------------------------------------------------
	
	static bool
	RegisterNamespace ( XMP_StringPtr	namespaceURI,
						XMP_StringPtr	suggestedPrefix,
						XMP_StringPtr * registeredPrefix,
						XMP_StringLen * prefixSize );
	
	static bool
	GetNamespacePrefix ( XMP_StringPtr	 namespaceURI,
						 XMP_StringPtr * namespacePrefix,
						 XMP_StringLen * prefixSize );
	
	static bool
	GetNamespaceURI ( XMP_StringPtr	  namespacePrefix,
					  XMP_StringPtr * namespaceURI,
					  XMP_StringLen * uriSize );
	
	static void
	DeleteNamespace ( XMP_StringPtr namespaceURI );

	// ---------------------------------------------------------------------------------------------
	
	#if AdobePrivate
	static void
	RegisterPropertyTraits ( XMP_StringPtr	schemaNS,
							 XMP_StringPtr	propName,
							 XMP_OptionBits options );

	// ---------------------------------------------------------------------------------------------
	#endif
	
	virtual bool
	GetProperty ( XMP_StringPtr	   schemaNS,
				  XMP_StringPtr	   propName,
				  XMP_StringPtr *  propValue,
				  XMP_StringLen *  valueSize,
				  XMP_OptionBits * options ) const;
	
	virtual bool
	GetArrayItem ( XMP_StringPtr	schemaNS,
				   XMP_StringPtr	arrayName,
				   XMP_Index		itemIndex,
				   XMP_StringPtr *	itemValue,
				   XMP_StringLen *	valueSize,
				   XMP_OptionBits * options ) const;
	
	virtual bool
	GetStructField ( XMP_StringPtr	  schemaNS,
					 XMP_StringPtr	  structName,
					 XMP_StringPtr	  fieldNS,
					 XMP_StringPtr	  fieldName,
					 XMP_StringPtr *  fieldValue,
					 XMP_StringLen *  valueSize,
					 XMP_OptionBits * options ) const;
	
	virtual bool
	GetQualifier ( XMP_StringPtr	schemaNS,
				   XMP_StringPtr	propName,
				   XMP_StringPtr	qualNS,
				   XMP_StringPtr	qualName,
				   XMP_StringPtr *	qualValue,
				   XMP_StringLen *	valueSize,
				   XMP_OptionBits * options ) const;
	
	// ---------------------------------------------------------------------------------------------
	
	virtual void
	SetProperty ( XMP_StringPtr	 schemaNS,
				  XMP_StringPtr	 propName,
				  XMP_StringPtr	 propValue,
				  XMP_OptionBits options );
	
	virtual void
	SetArrayItem ( XMP_StringPtr  schemaNS,
				   XMP_StringPtr  arrayName,
				   XMP_Index	  itemIndex,
				   XMP_StringPtr  itemValue,
				   XMP_OptionBits options );
	
	virtual void
	AppendArrayItem ( XMP_StringPtr	 schemaNS,
					  XMP_StringPtr	 arrayName,
					  XMP_OptionBits arrayOptions,
					  XMP_StringPtr	 itemValue,
					  XMP_OptionBits options );
	
	void
	SetStructField ( XMP_StringPtr	schemaNS,
					 XMP_StringPtr	structName,
					 XMP_StringPtr	fieldNS,
					 XMP_StringPtr	fieldName,
					 XMP_StringPtr	fieldValue,
					 XMP_OptionBits options );
	
	virtual void
	SetQualifier ( XMP_StringPtr  schemaNS,
				   XMP_StringPtr  propName,
				   XMP_StringPtr  qualNS,
				   XMP_StringPtr  qualName,
				   XMP_StringPtr  qualValue,
				   XMP_OptionBits options );
	
	// ---------------------------------------------------------------------------------------------
	
	virtual void
	DeleteProperty ( XMP_StringPtr schemaNS,
					 XMP_StringPtr propName );
	
	virtual void
	DeleteArrayItem ( XMP_StringPtr schemaNS,
					  XMP_StringPtr arrayName,
					  XMP_Index		itemIndex );
	
	virtual void
	DeleteStructField ( XMP_StringPtr schemaNS,
						XMP_StringPtr structName,
						XMP_StringPtr fieldNS,
						XMP_StringPtr fieldName );
	
	virtual void
	DeleteQualifier ( XMP_StringPtr schemaNS,
					  XMP_StringPtr propName,
					  XMP_StringPtr qualNS,
					  XMP_StringPtr qualName );
	
	// ---------------------------------------------------------------------------------------------
	
	virtual bool
	DoesPropertyExist ( XMP_StringPtr schemaNS,
						XMP_StringPtr propName ) const;
	
	bool
	DoesArrayItemExist ( XMP_StringPtr schemaNS,
						 XMP_StringPtr arrayName,
						 XMP_Index	   itemIndex ) const;
	
	bool
	DoesStructFieldExist ( XMP_StringPtr schemaNS,
						   XMP_StringPtr structName,
						   XMP_StringPtr fieldNS,
						   XMP_StringPtr fieldName ) const;
	
	bool
	DoesQualifierExist ( XMP_StringPtr schemaNS,
						 XMP_StringPtr propName,
						 XMP_StringPtr qualNS,
						 XMP_StringPtr qualName ) const;
	
	// ---------------------------------------------------------------------------------------------
	
	virtual bool
	GetLocalizedText ( XMP_StringPtr	schemaNS,
					   XMP_StringPtr	altTextName,
					   XMP_StringPtr	genericLang,
					   XMP_StringPtr	specificLang,
					   XMP_StringPtr *	actualLang,
					   XMP_StringLen *	langSize,
					   XMP_StringPtr *	itemValue,
					   XMP_StringLen *	valueSize,
					   XMP_OptionBits * options ) const;
	
	virtual void
	SetLocalizedText ( XMP_StringPtr  schemaNS,
					   XMP_StringPtr  altTextName,
					   XMP_StringPtr  genericLang,
					   XMP_StringPtr  specificLang,
					   XMP_StringPtr  itemValue,
					   XMP_OptionBits options );
	
	virtual void
	DeleteLocalizedText (	XMP_StringPtr	schemaNS,
							XMP_StringPtr	altTextName,
							XMP_StringPtr	genericLang,
							XMP_StringPtr	specificLang);

	// ---------------------------------------------------------------------------------------------
	
	bool
	GetProperty_Bool ( XMP_StringPtr	schemaNS,
					   XMP_StringPtr	propName,
					   bool *			propValue,
					   XMP_OptionBits * options ) const;
	
	bool
	GetProperty_Int ( XMP_StringPtr	   schemaNS,
					  XMP_StringPtr	   propName,
					  XMP_Int32 *	   propValue,
					  XMP_OptionBits * options ) const;
	
	bool
	GetProperty_Int64 ( XMP_StringPtr	 schemaNS,
					    XMP_StringPtr	 propName,
					    XMP_Int64 *	     propValue,
					    XMP_OptionBits * options ) const;
	
	bool
	GetProperty_Float ( XMP_StringPtr	 schemaNS,
						XMP_StringPtr	 propName,
						double *		 propValue,
						XMP_OptionBits * options ) const;
	
	bool
	GetProperty_Date ( XMP_StringPtr	schemaNS,
					   XMP_StringPtr	propName,
					   XMP_DateTime *	propValue,
					   XMP_OptionBits * options ) const;
	
	// ---------------------------------------------------------------------------------------------
	
	void
	SetProperty_Bool ( XMP_StringPtr  schemaNS,
					   XMP_StringPtr  propName,
					   bool			  propValue,
					   XMP_OptionBits options );
	
	void
	SetProperty_Int ( XMP_StringPtr	 schemaNS,
					  XMP_StringPtr	 propName,
					  XMP_Int32		 propValue,
					  XMP_OptionBits options );
	
	void
	SetProperty_Int64 ( XMP_StringPtr  schemaNS,
					    XMP_StringPtr  propName,
					    XMP_Int64	   propValue,
					    XMP_OptionBits options );
	
	void
	SetProperty_Float ( XMP_StringPtr  schemaNS,
						XMP_StringPtr  propName,
						double		   propValue,
						XMP_OptionBits options );
	
	void
	SetProperty_Date ( XMP_StringPtr		schemaNS,
					   XMP_StringPtr		propName,
					   const XMP_DateTime & propValue,
					   XMP_OptionBits		options );
	
	// ---------------------------------------------------------------------------------------------
	
	virtual void
	GetObjectName ( XMP_StringPtr * namePtr,
					XMP_StringLen * nameLen ) const;

	virtual void
	SetObjectName ( XMP_StringPtr name );

	XMP_OptionBits
	GetObjectOptions() const;
	
	void
	SetObjectOptions ( XMP_OptionBits options );

	virtual void
	Sort();

	virtual void
	Erase();

	virtual void
	Clone ( XMPMeta * clone, XMP_OptionBits options ) const;
	
	virtual XMP_Index
	CountArrayItems ( XMP_StringPtr schemaNS,
					  XMP_StringPtr arrayName ) const;
	
	#if AdobePrivate
	void
	MarkStaleProperties ( XMP_OptionBits options );
	#endif
	
	virtual void
	DumpObject ( XMP_TextOutputProc outProc,
				 void *				refCon ) const;
	
	// ---------------------------------------------------------------------------------------------
	
	virtual void
	ParseFromBuffer ( XMP_StringPtr	 buffer,
					  XMP_StringLen	 bufferSize,
					  XMP_OptionBits options );
	
	virtual void
	SerializeToBuffer ( XMP_VarString * rdfString,
						XMP_OptionBits	options,
						XMP_StringLen	padding,
						XMP_StringPtr	newline,
						XMP_StringPtr	indent,
						XMP_Index		baseIndent ) const;
	
	// ---------------------------------------------------------------------------------------------

	static void
	SetDefaultErrorCallback ( XMPMeta_ErrorCallbackWrapper wrapperProc,
							  XMPMeta_ErrorCallbackProc    clientProc,
							  void *    context,
							  XMP_Uns32 limit );

	virtual void
	SetErrorCallback ( XMPMeta_ErrorCallbackWrapper wrapperProc,
					   XMPMeta_ErrorCallbackProc    clientProc,
					   void *    context,
					   XMP_Uns32 limit );

	virtual void
	ResetErrorCallbackLimit ( XMP_Uns32 limit );
	
	class ErrorCallbackInfo : public GenericErrorCallback {
	public:

		XMPMeta_ErrorCallbackWrapper wrapperProc;
		XMPMeta_ErrorCallbackProc    clientProc;
		void * context;

		ErrorCallbackInfo() : wrapperProc(0), clientProc(0), context(0) {};
		
		void Clear() { this->wrapperProc = 0; this->clientProc = 0; this->context = 0;
					   GenericErrorCallback::Clear(); };

		bool CanNotify() const;
		bool ClientCallbackWrapper ( XMP_StringPtr filePath, XMP_ErrorSeverity severity, XMP_Int32 cause, XMP_StringPtr messsage ) const;
	};

	// =============================================================================================

	// ---------------------------------------------------------------------------------------------
	// - Everything is built out of standard nodes. Each node has a name, value, option flags, a
	// vector of child nodes, and a vector of qualifier nodes.
	//
	// - The option flags are those passed to SetProperty and returned from GetProperty. They tell
	// if the node is simple, a struct or an array; whether it has qualifiers, etc.
	//
	// - The name of the node is an XML qualified name, of the form "prefix:simple-name". Since we
	// force all namespaces to be known and to have unique prefixes, this is semantically equivalent
	// to using a URI and simple name pair.
	//
	// - Although the value part is only for leaf properties and the children part is only for
	// structs and arrays, it is easier to simply have them in every node. This keeps things visible
	// so that debugging is easier
	//
	// - The top level node children are the namespaces that contain properties, the next level are
	// the top level properties, lower levels are the fields of structs or items of arrays. The name
	// of the top level nodes is just the namespace prefix, with the colon terminator. The name of
	// top level properties includes the namespace prefix.
	//
	// - Any property node, at any level, can have qualifiers. These are themselves general property
	// nodes. And could in fact themselves have qualifiers!

	// ! Expose the implementation so that file static functions can see the data.

	XMP_Int32 clientRefs;	// ! Must be signed to allow decrement from 0.
	XMP_ReadWriteLock lock;

	// ! Any data member changes must be propagted to the Clone function!

	XMP_Node tree;
	XMLParserAdapter * xmlParser;
	ErrorCallbackInfo errorCallback;
	
	friend class XMPIterator;
	friend class XMPUtils;

private:
  
	// ! These are hidden on purpose:
	XMPMeta ( const XMPMeta & /* original */ ) : tree(XMP_Node(0,"",0)), clientRefs(0), xmlParser(0)
		{ XMP_Throw ( "Call to hidden constructor", kXMPErr_InternalFailure ); };
	void operator= ( const XMPMeta & /* rhs */ )  
		{ XMP_Throw ( "Call to hidden operator=", kXMPErr_InternalFailure ); };

	// Special support routines for parsing, here to be able to access the errorCallback.
	void ProcessXMLTree ( XMP_OptionBits options );
	bool ProcessXMLBuffer ( XMP_StringPtr buffer, XMP_StringLen xmpSize, bool lastClientCall );
	void ProcessRDF ( const XML_Node & xmlTree, XMP_OptionBits options );

};	// class XMPMeta

// =================================================================================================

void
DumpNodeOptions(XMP_OptionBits	   options,
				XMP_TextOutputProc outProc,
				void *			   refCon);

void
NormalizeDCArrays(XMP_Node * xmpTree);

void
MoveExplicitAliases(XMP_Node *                   tree,
					XMP_OptionBits               parseOptions,
					XMPMeta::ErrorCallbackInfo & errorCallback);

void
TouchUpDataModel(XMPMeta *                    xmp,
				 XMPMeta::ErrorCallbackInfo & errorCallback);

#endif	// __XMPMeta_hpp__
