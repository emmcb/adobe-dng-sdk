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
//   AJ Abhishek Jindal
//
// mm-dd-yy who Description of changes, most recent on top.
//
// 02-06-15 AJ  5.6-c037 Fixing warnings due to implicit typecasting
// 10-10-12 ADC 5.5-c012 Changed internal implementation of common error notification infrastructure.
// 08-10-12 AWL 5.5-c008 Change XMPCore XML parsing error notifications to be recoverable.
// 08-08-12 AWL 5.5-c007 XMPCore error notifications for one case of XML parsing, no existing test failures.
//
// 06-11-09 AWL 5.0-c034 Finish threading revamp, implement friendly reader/writer locking.
// 04-03-09 AWL 5.0-c022 Change PlainXMP to TransformXMP.
//
// 08-06-08 AWL Add disabled conditional code for limiting XML entity usage.
// 07-01-08 AWL 4.3-c060 Hide all uses of PlainXMP.
//
// 12-10-07 AWL 4.2-c033 Save prefix length in XML_Node, change XML_Node::GetNamedElement to take URI and local name.
// 11-30-07 AWL 4.2-c027 Expose XML_Node and ExpatAdapter so that XMPFiles can use them.
// 01-10-07 AWL 4.2-c005 Change ExpatAdapter::ParseBuffer to not include the detailed message for now.
//
// 08-16-06 AWL 4.1-c015 [1312441,1340116] Change the parsing code to tolerate ISO Latin-1 input. At
//				the same time, get rid of the ugly hackery that tries to deal with unescaped ASCII
//				control characters. Also get rid of the check and complaint about parsing into an
//				empty object. This is predominantly what people want, so clear the XMPMeta object.
//
// 07-18-06 AWL 4.0-c011 [1269158] Cleanup bad dc: namespace URI when parsing. Early versions of
//				Flash put XMP in SWF files using "http://purl.org/dc/1.1/", the proper URI is
//				"http://purl.org/dc/elements/1.1/".
// 06-27-06 AWL 4.0-c009 Changes for Acrobat security audit.
// 03-24-06 AWL 4.0-c001 Adapt for move to ham-perforce, integrate XMPFiles, bump version to 4.
//
// 01-24-06 AWL 3.3-010 Turn off snprintf warning.
// 10-13-05 AWL 3.3-007 [1247060,1246288,1246286] Fix handling of special rdf: attributes, allow and
//				ignore inner XML processing instructions, add Expat handler for CDATA sections.
// 04-06-05 AWL 3.2-013 [0509601] Normalize "single value" alt-text arrays. Improve the way the root
//				XML node is found and extract the previous toolkit version number.
// 04-05-05 AWL 3.2-011 [0532345] Normalize xml:lang values so that compares are in effect case
//				insensitive as required by RFC 3066. Change parsing and serializing to force the
//				x-default item to be first.
// 03-31-05 AWL 3.2-007 Fix parsing for UTF-16 and UTF-32.
//
// 02-03-05 AWL Initial creation.
//
// =================================================================================================
#endif // AdobePrivate

#include "public/include/XMP_Environment.h"	// ! Must be the first #include!
#include "XMPCore/source/XMPCore_Impl.hpp"

#include "source/ExpatAdapter.hpp"
#include "XMPCore/source/XMPMeta.hpp"

#if AdobePrivate
#include "XMPCore/third-party/expat/public/lib/expat.h"
#else
#include "third-party/expat/lib/expat.h"
#endif

#include <string.h>

using namespace std;

#if XMP_WinBuild
	#pragma warning ( disable : 4996 )	// '...' was declared deprecated
#endif

// *** Set memory handlers.

#ifndef DumpXMLParseEvents
	#define DumpXMLParseEvents	0
#endif

#define FullNameSeparator	'@'

// =================================================================================================

static void StartNamespaceDeclHandler    ( void * userData, XMP_StringPtr prefix, XMP_StringPtr uri );
static void EndNamespaceDeclHandler      ( void * userData, XMP_StringPtr prefix );

static void StartElementHandler          ( void * userData, XMP_StringPtr name, XMP_StringPtr* attrs );
static void EndElementHandler            ( void * userData, XMP_StringPtr name );

static void CharacterDataHandler         ( void * userData, XMP_StringPtr cData, int len );
static void StartCdataSectionHandler     ( void * userData );
static void EndCdataSectionHandler       ( void * userData );

static void ProcessingInstructionHandler ( void * userData, XMP_StringPtr target, XMP_StringPtr data );
static void CommentHandler               ( void * userData, XMP_StringPtr comment );

#if BanAllEntityUsage

	// For now we do this by banning DOCTYPE entirely. This is easy and consistent with what is
	// available in recent Java XML parsers. Another, somewhat less drastic, approach would be to 
	// ban all entity declarations. We can't allow declarations and ban references, Expat does not
	// call the SkippedEntityHandler for references in attribute values.
	
	// ! Standard entities (&amp;, &lt;, &gt;, &quot;, &apos;, and numeric character references) are
	// ! not banned. Expat handles them transparently no matter what.

	static void StartDoctypeDeclHandler ( void * userData, XMP_StringPtr doctypeName,
										  XMP_StringPtr sysid, XMP_StringPtr pubid, int has_internal_subset );

#endif

// =================================================================================================

extern "C" ExpatAdapter * XMP_NewExpatAdapter ( bool useGlobalNamespaces )
{

	return new ExpatAdapter ( useGlobalNamespaces );

}	// XMP_NewExpatAdapter

// =================================================================================================

ExpatAdapter::ExpatAdapter ( bool useGlobalNamespaces ) : parser(0), registeredNamespaces(0)
{

	#if XMP_DebugBuild
		this->elemNesting = 0;
		#if DumpXMLParseEvents
			if ( this->parseLog == 0 ) this->parseLog = stdout;
		#endif
	#endif

	this->parser = XML_ParserCreateNS ( 0, FullNameSeparator );
	if ( this->parser == 0 ) {
		XMP_Error error(kXMPErr_NoMemory, "Failure creating Expat parser" );
		this->NotifyClient ( kXMPErrSev_ProcessFatal, error );
	}else{
		if ( useGlobalNamespaces ) {
			this->registeredNamespaces = sRegisteredNamespaces;
		} else {
			this->registeredNamespaces = new XMP_NamespaceTable ( *sRegisteredNamespaces );
		}
	
		XML_SetUserData ( this->parser, this );
	
		XML_SetNamespaceDeclHandler ( this->parser, StartNamespaceDeclHandler, EndNamespaceDeclHandler );
		XML_SetElementHandler ( this->parser, StartElementHandler, EndElementHandler );

		XML_SetCharacterDataHandler ( this->parser, CharacterDataHandler );
		XML_SetCdataSectionHandler ( this->parser, StartCdataSectionHandler, EndCdataSectionHandler );

		XML_SetProcessingInstructionHandler ( this->parser, ProcessingInstructionHandler );
		XML_SetCommentHandler ( this->parser, CommentHandler );

		#if BanAllEntityUsage
			XML_SetStartDoctypeDeclHandler ( this->parser, StartDoctypeDeclHandler );
			isAborted = false;
		#endif

		this->parseStack.push_back ( &this->tree );	// Push the XML root node.
	}
}	// ExpatAdapter::ExpatAdapter

// =================================================================================================

ExpatAdapter::~ExpatAdapter()
{

	if ( this->parser != 0 ) XML_ParserFree ( this->parser );
	this->parser = 0;
	
	if ( this->registeredNamespaces != sRegisteredNamespaces ) delete ( this->registeredNamespaces );
	this->registeredNamespaces = 0;

}	// ExpatAdapter::~ExpatAdapter

// =================================================================================================

#if XMP_DebugBuild
	static XMP_VarString sExpatMessage;
#endif

static const char * kOneSpace = " ";

void ExpatAdapter::ParseBuffer ( const void * buffer, size_t length, bool last /* = true */ )
{
	enum XML_Status status;
	
	if ( length == 0 ) {	// Expat does not like empty buffers.
		if ( ! last ) return;
		buffer = kOneSpace;
		length = 1;
	}
	
	status = XML_Parse ( this->parser, (const char *)buffer, static_cast< XMP_StringLen >( length ), last );
	
	#if BanAllEntityUsage
		if ( this->isAborted ) {
			XMP_Error error(kXMPErr_BadXML, "DOCTYPE is not allowed" );
			this->NotifyClient ( kXMPErrSev_Recoverable, error );
		}
	#endif

	if ( status != XML_STATUS_OK ) {
	
		XMP_StringPtr errMsg = "XML parsing failure";

		#if 0	// XMP_DebugBuild	// Disable for now to make test output uniform. Restore later with thread safety.
		
			// *** This is a good candidate for a callback error notification mechanism.
			// *** This code is not thread safe, the sExpatMessage isn't locked. But that's OK for debug usage.

			enum XML_Error expatErr = XML_GetErrorCode ( this->parser );
			const char *   expatMsg = XML_ErrorString ( expatErr );
			int errLine = XML_GetCurrentLineNumber ( this->parser );
		
			char msgBuffer[1000];
			// AUDIT: Use of sizeof(msgBuffer) for snprintf length is safe.
			snprintf ( msgBuffer, sizeof(msgBuffer), "# Expat error %d at line %d, \"%s\"", expatErr, errLine, expatMsg );
			sExpatMessage = msgBuffer;
			errMsg = sExpatMessage.c_str();

			#if  DumpXMLParseEvents
				if ( this->parseLog != 0 ) fprintf ( this->parseLog, "%s\n", errMsg, expatErr, errLine, expatMsg );
			#endif

		#endif

		XMP_Error error(kXMPErr_BadXML, errMsg);
		this->NotifyClient ( kXMPErrSev_Recoverable, error );

	}
	
}	// ExpatAdapter::ParseBuffer

// =================================================================================================
// =================================================================================================

#if XMP_DebugBuild & DumpXMLParseEvents

	static inline void PrintIndent ( FILE * file, size_t count )
	{
		for ( ; count > 0; --count ) fprintf ( file, "  " );
	}

#endif

// =================================================================================================

static void SetQualName ( ExpatAdapter * thiz, XMP_StringPtr fullName, XML_Node * node )
{
	// Expat delivers the full name as a catenation of namespace URI, separator, and local name.

	// As a compatibility hack, an "about" or "ID" attribute of an rdf:Description element is
	// changed to "rdf:about" or rdf:ID. Easier done here than in the RDF recognizer.
	
	// As a bug fix hack, change a URI of "http://purl.org/dc/1.1/" to ""http://purl.org/dc/elements/1.1/.
	// Early versions of Flash that put XMP in SWF used a bad URI for the dc: namespace.

	// ! This code presumes the RDF namespace prefix is "rdf".

	size_t sepPos = strlen(fullName);
	for ( --sepPos; sepPos > 0; --sepPos ) {
		if ( fullName[sepPos] == FullNameSeparator ) break;
	}

	if ( fullName[sepPos] == FullNameSeparator ) {

		XMP_StringPtr prefix;
		XMP_StringLen prefixLen;
		XMP_StringPtr localPart = fullName + sepPos + 1;

		node->ns.assign ( fullName, sepPos );
		if ( node->ns == "http://purl.org/dc/1.1/" ) node->ns = "http://purl.org/dc/elements/1.1/";

		bool found = thiz->registeredNamespaces->GetPrefix ( node->ns.c_str(), &prefix, &prefixLen );
		if ( ! found ) {
			XMP_Error error(kXMPErr_ExternalFailure, "Unknown URI in Expat full name" );
			thiz->NotifyClient ( kXMPErrSev_OperationFatal, error );
		}
		node->nsPrefixLen = prefixLen;	// ! Includes the ':'.
		
		node->name = prefix;
		node->name += localPart;

	} else {

		node->name = fullName;	// The name is not in a namespace.
	
		if ( node->parent->name == "rdf:Description" ) {
			if ( node->name == "about" ) {
				node->ns   = kXMP_NS_RDF;
				node->name = "rdf:about";
				node->nsPrefixLen = 4;	// ! Include the ':'.
			} else if ( node->name == "ID" ) {
				node->ns   = kXMP_NS_RDF;
				node->name = "rdf:ID";
				node->nsPrefixLen = 4;	// ! Include the ':'.
			}
		}
		
	}

}	// SetQualName

// =================================================================================================

static void StartNamespaceDeclHandler ( void * userData, XMP_StringPtr prefix, XMP_StringPtr uri )
{
	IgnoreParam(userData);
	
	// As a bug fix hack, change a URI of "http://purl.org/dc/1.1/" to ""http://purl.org/dc/elements/1.1/.
	// Early versions of Flash that put XMP in SWF used a bad URI for the dc: namespace.
	
	ExpatAdapter * thiz = (ExpatAdapter*)userData;

	if ( prefix == 0 ) prefix = "_dflt_";	// Have default namespace.
	if ( uri == 0 ) return;	// Ignore, have xmlns:pre="", no URI to register.
	
	#if XMP_DebugBuild & DumpXMLParseEvents
		if ( thiz->parseLog != 0 ) {
			PrintIndent ( thiz->parseLog, thiz->elemNesting );
			fprintf ( thiz->parseLog, "StartNamespace: %s - \"%s\"\n", prefix, uri );
		}
	#endif
	
	if ( XMP_LitMatch ( uri, "http://purl.org/dc/1.1/" ) ) uri = "http://purl.org/dc/elements/1.1/";
	if (thiz->registeredNamespaces == sRegisteredNamespaces) {
		(void)XMPMeta::RegisterNamespace(uri, prefix, 0, 0);
	}
	else {
		(void)thiz->registeredNamespaces->Define(uri, prefix, 0, 0);
	}

}	// StartNamespaceDeclHandler

// =================================================================================================

static void EndNamespaceDeclHandler ( void * userData, XMP_StringPtr prefix )
{
	IgnoreParam(userData);

	#if XMP_DebugBuild & DumpXMLParseEvents		// Avoid unused variable warning.
		ExpatAdapter * thiz = (ExpatAdapter*)userData;
	#endif

	if ( prefix == 0 ) prefix = "_dflt_";	// Have default namespace.
	
	#if XMP_DebugBuild & DumpXMLParseEvents
		if ( thiz->parseLog != 0 ) {
			PrintIndent ( thiz->parseLog, thiz->elemNesting );
			fprintf ( thiz->parseLog, "EndNamespace: %s\n", prefix );
		}
	#endif
	
	// ! Nothing to do, Expat has done all of the XML processing.

}	// EndNamespaceDeclHandler

// =================================================================================================

static void StartElementHandler ( void * userData, XMP_StringPtr name, XMP_StringPtr* attrs )
{
	XMP_Assert ( attrs != 0 );
	ExpatAdapter * thiz = (ExpatAdapter*)userData;
	
	size_t attrCount = 0;
	for ( XMP_StringPtr* a = attrs; *a != 0; ++a ) ++attrCount;
	if ( (attrCount & 1) != 0 ) {
		XMP_Error error(kXMPErr_ExternalFailure, "Expat attribute info has odd length");
		thiz->NotifyClient ( kXMPErrSev_OperationFatal, error );
	}
	attrCount = attrCount/2;	// They are name/value pairs.
	
	#if XMP_DebugBuild & DumpXMLParseEvents
		if ( thiz->parseLog != 0 ) {
			PrintIndent ( thiz->parseLog, thiz->elemNesting );
			fprintf ( thiz->parseLog, "StartElement: %s, %d attrs", name, attrCount );
			for ( XMP_StringPtr* attr = attrs; *attr != 0; attr += 2 ) {
				XMP_StringPtr attrName = *attr;
				XMP_StringPtr attrValue = *(attr+1);
				fprintf ( thiz->parseLog, ", %s = \"%s\"", attrName, attrValue );
			}
			fprintf ( thiz->parseLog, "\n" );
		}
	#endif

	XML_Node * parentNode = thiz->parseStack.back();
	XML_Node * elemNode   = new XML_Node ( parentNode, "", kElemNode );
	
	SetQualName ( thiz, name, elemNode );
	
	for ( XMP_StringPtr* attr = attrs; *attr != 0; attr += 2 ) {

		XMP_StringPtr attrName = *attr;
		XMP_StringPtr attrValue = *(attr+1);
		XML_Node * attrNode = new XML_Node ( elemNode, "", kAttrNode );

		SetQualName ( thiz, attrName, attrNode );
		attrNode->value = attrValue;
		if ( attrNode->name == "xml:lang" ) NormalizeLangValue ( &attrNode->value );
		elemNode->attrs.push_back ( attrNode );

	}
	
	parentNode->content.push_back ( elemNode );
	thiz->parseStack.push_back ( elemNode );
	
#if AdobePrivate
	if ( (elemNode->name == "rdf:RDF") || (elemNode->name == "txmp:XMP_Packet") ) {
		thiz->rootNode = elemNode;
		++thiz->rootCount;
	}
#else
	if ( elemNode->name == "rdf:RDF" ) {
		thiz->rootNode = elemNode;
		++thiz->rootCount;
	}
#endif

	#if XMP_DebugBuild
		++thiz->elemNesting;
	#endif

}	// StartElementHandler

// =================================================================================================

static void EndElementHandler ( void * userData, XMP_StringPtr name )
{
	IgnoreParam(name);
	
	ExpatAdapter * thiz = (ExpatAdapter*)userData;

	#if XMP_DebugBuild
		--thiz->elemNesting;
	#endif
	(void) thiz->parseStack.pop_back();
	
	#if XMP_DebugBuild & DumpXMLParseEvents
		if ( thiz->parseLog != 0 ) {
			PrintIndent ( thiz->parseLog, thiz->elemNesting );
			fprintf ( thiz->parseLog, "EndElement: %s\n", name );
		}
	#endif

}	// EndElementHandler

// =================================================================================================

static void CharacterDataHandler ( void * userData, XMP_StringPtr cData, int len )
{
	ExpatAdapter * thiz = (ExpatAdapter*)userData;
	
	if ( (cData == 0) || (len == 0) ) { cData = ""; len = 0; }
	
	#if XMP_DebugBuild & DumpXMLParseEvents
		if ( thiz->parseLog != 0 ) {
			PrintIndent ( thiz->parseLog, thiz->elemNesting );
			fprintf ( thiz->parseLog, "CharContent: \"" );
			for ( int i = 0; i < len; ++i ) fprintf ( thiz->parseLog, "%c", cData[i] );
			fprintf ( thiz->parseLog, "\"\n" );
		}
	#endif
	
	XML_Node * parentNode = thiz->parseStack.back();
	XML_Node * cDataNode  = new XML_Node ( parentNode, "", kCDataNode );
	
	cDataNode->value.assign ( cData, len );
	parentNode->content.push_back ( cDataNode );
	
}	// CharacterDataHandler

// =================================================================================================

static void StartCdataSectionHandler ( void * userData )
{
	IgnoreParam(userData);

	#if XMP_DebugBuild & DumpXMLParseEvents		// Avoid unused variable warning.
		ExpatAdapter * thiz = (ExpatAdapter*)userData;
	#endif
	
	#if XMP_DebugBuild & DumpXMLParseEvents
		if ( thiz->parseLog != 0 ) {
			PrintIndent ( thiz->parseLog, thiz->elemNesting );
			fprintf ( thiz->parseLog, "StartCDATA\n" );
		}
	#endif
	
	// *** Since markup isn't recognized inside CDATA, this affects XMP's double escaping.
	
}	// StartCdataSectionHandler

// =================================================================================================

static void EndCdataSectionHandler ( void * userData )
{
	IgnoreParam(userData);

	#if XMP_DebugBuild & DumpXMLParseEvents		// Avoid unused variable warning.
		ExpatAdapter * thiz = (ExpatAdapter*)userData;
	#endif
	
	#if XMP_DebugBuild & DumpXMLParseEvents
		if ( thiz->parseLog != 0 ) {
			PrintIndent ( thiz->parseLog, thiz->elemNesting );
			fprintf ( thiz->parseLog, "EndCDATA\n" );
		}
	#endif	

}	// EndCdataSectionHandler

// =================================================================================================

static void ProcessingInstructionHandler ( void * userData, XMP_StringPtr target, XMP_StringPtr data )
{
	XMP_Assert ( target != 0 );
	ExpatAdapter * thiz = (ExpatAdapter*)userData;

	if ( ! XMP_LitMatch ( target, "xpacket" ) ) return;	// Ignore all PIs except the XMP packet wrapper.
	if ( data == 0 ) data = "";
	
	#if XMP_DebugBuild & DumpXMLParseEvents
		if ( thiz->parseLog != 0 ) {
			PrintIndent ( thiz->parseLog, thiz->elemNesting );
			fprintf ( thiz->parseLog, "PI: %s - \"%s\"\n", target, data );
		}
	#endif
	
	XML_Node * parentNode = thiz->parseStack.back();
	XML_Node * piNode  = new XML_Node ( parentNode, target, kPINode );
	
	piNode->value.assign ( data );
	parentNode->content.push_back ( piNode );
	
}	// ProcessingInstructionHandler

// =================================================================================================

static void CommentHandler ( void * userData, XMP_StringPtr comment )
{
	IgnoreParam(userData);

	#if XMP_DebugBuild & DumpXMLParseEvents		// Avoid unused variable warning.
		ExpatAdapter * thiz = (ExpatAdapter*)userData;
	#endif

	if ( comment == 0 ) comment = "";
	
	#if XMP_DebugBuild & DumpXMLParseEvents
		if ( thiz->parseLog != 0 ) {
			PrintIndent ( thiz->parseLog, thiz->elemNesting );
			fprintf ( thiz->parseLog, "Comment: \"%s\"\n", comment );
		}
	#endif
	
	// ! Comments are ignored.
	
}	// CommentHandler

// =================================================================================================

#if BanAllEntityUsage
static void StartDoctypeDeclHandler ( void * userData, XMP_StringPtr doctypeName,
									  XMP_StringPtr sysid, XMP_StringPtr pubid, int has_internal_subset )
{
	IgnoreParam(userData);

	ExpatAdapter * thiz = (ExpatAdapter*)userData;

	#if XMP_DebugBuild & DumpXMLParseEvents		// Avoid unused variable warning.
		if ( thiz->parseLog != 0 ) {
			PrintIndent ( thiz->parseLog, thiz->elemNesting );
			fprintf ( thiz->parseLog, "DocType: \"%s\"\n", doctypeName );
		}
	#endif
	
	thiz->isAborted = true;	// ! Can't throw an exception across the plain C Expat frames.
	(void) XML_StopParser ( thiz->parser, XML_FALSE /* not resumable */ );

}	// StartDoctypeDeclHandler
#endif

// =================================================================================================
