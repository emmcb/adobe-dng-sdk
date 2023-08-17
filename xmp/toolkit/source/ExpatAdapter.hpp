#ifndef __ExpatAdapter_hpp__
#define __ExpatAdapter_hpp__

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
//  ADC	Amandeep Chawla
//
// mm-dd-yy who Description of changes, most recent on top.
//
// 02-24-14 ADC 5.6-c001 XMPCommon Framework and XMPCore new APIs ported to Mac Environment.
//
// 06-11-09 AWL 5.0-c034 Finish threading revamp, implement friendly reader/writer locking.
//
// 08-06-08 AWL Add disabled conditional code for limiting XML entity usage.
//
// 11-30-07 AWL 4.2-c027 Expose XML_Node and ExpatAdapter so that XMPFiles can use them.
// 08-24-07 AWL 4.2-c019 Remove support for ancient CXMP_* init/term routines and non-public (AXE) expat.
//
// 08-16-06 AWL 4.1-c015 [1312441,1340116] Change the parsing code to tolerate ISO Latin-1 input. At
//				the same time, get rid of the ugly hackery that tries to deal with unescaped ASCII
//				control characters. Also get rid of the check and complaint about parsing into an
//				empty object. This is predominantly what people want, so clear the XMPMeta object.
//
// 03-24-06 AWL 4.0-c001 Adapt for move to ham-perforce, integrate XMPFiles, bump version to 4.
//
// 02-03-05 AWL Initial creation.
//
// =================================================================================================
#endif // AdobePrivate

#include "public/include/XMP_Environment.h"	// ! Must be the first #include!
#include "source/XMLParserAdapter.hpp"

// =================================================================================================
// Derived XML parser adapter for Expat.
// =================================================================================================

#ifndef BanAllEntityUsage
	#define BanAllEntityUsage	0
#endif

struct XML_ParserStruct;	// ! Hack to avoid exposing expat.h to all clients.
typedef struct XML_ParserStruct *XML_Parser;

class ExpatAdapter : public XMLParserAdapter {
public:

	XML_Parser parser;
	XMP_NamespaceTable * registeredNamespaces;
	
	#if BanAllEntityUsage
		bool isAborted;
	#endif
	
	#if XMP_DebugBuild
		size_t elemNesting;
	#endif
	
	static const bool kUseGlobalNamespaces = true;
	static const bool kUseLocalNamespaces  = false;
	
	ExpatAdapter ( bool useGlobalNamespaces );
	virtual ~ExpatAdapter();
	
	void ParseBuffer ( const void * buffer, size_t length, bool last = true );

private:

	ExpatAdapter() : registeredNamespaces(0) {};	// ! Force use of constructor with namespace parameter.

};

extern "C" ExpatAdapter *
XMP_PUBLIC XMP_NewExpatAdapter ( bool useGlobalNamespaces );

// =================================================================================================

#endif	// __ExpatAdapter_hpp__
