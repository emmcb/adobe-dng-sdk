// =================================================================================================
// Copyright 2006-2009 Adobe
// All Rights Reserved.
//
// NOTICE:	Adobe permits you to use, modify, and distribute this file in accordance with the terms
// of the Adobe license agreement accompanying it. If you have received this file from a source other 
// than Adobe, then your use, modification, or distribution of it requires the prior written permission
// of Adobe.
// =================================================================================================

#include "XMPConst.h"

using namespace ScCore;


// This section defines the properties and uses the LivePropertyManager
// to do the usual handling of the properties.

// Datatypes for getProperty()
#define STRING					150
#define INTEGER					151
#define NUMBER					152
#define BOOLEAN					153
#define XMPDATE					154

// Miscellaneous constants
#define XMP_TRUE				100
#define XMP_FALSE				101

// Non-Adobe Schema Namespaces
#define NS_DC					200
#define NS_IPTC_CORE			201
#define NS_RDF					218
#define NS_XML					219
#define NS_EXIFEX				226

// Adobe Schema Namespaces
#define NS_XMP					202
#define NS_XMP_RIGHTS			203
#define NS_XMP_MM				204
#define NS_XMP_BJ				205
#define NS_XMP_NOTE				220
#define NS_PDF					206
#define NS_PDFX					221
#define NS_PDFX_ID				225
#define NS_PHOTOSHOP			207
#define NS_PS_ALBUM				208
#define NS_EXIF					209
#define NS_EXIF_AUX				210
#define NS_TIFF					211
#define NS_PNG					212
#define NS_SWF					222
#define NS_JPEG					213
#define NS_JP2K					214
#define NS_CAMERA_RAW			215
#define NS_DM					216
#define NS_ASF					223
#define NS_WAV					224
#define NS_ADOBE_STOCK_PHOTO	217

// Adobe Value Type Namespaces
#define TYPE_IDENTIFIER_QUAL	250
#define TYPE_DIMENSIONS			251
#define TYPE_TEXT				252
#define TYPE_PAGEDFILE			253
#define TYPE_GRAPHICS			254
#define TYPE_IMAGE				255
#define TYPE_FONT				256
#define TYPE_RESOURCE_EVENT		257
#define TYPE_RESOURCE_REF		258
#define TYPE_ST_VERSION			259
#define TYPE_ST_JOB				260
#define TYPE_MANIFEST_ITEM		261
#define TYPE_PDFA_SCHEMA		262
#define TYPE_PDFA_PROPERTY		263
#define TYPE_PDFA_TYPE			264
#define TYPE_PDFA_FIELD			265
#define TYPE_PDFA_ID			266
#define TYPE_PDFA_EXTENSION		267

// Alias forms
#define	ALIAS_TO_SIMPLE_PROP		300
#define	ALIAS_TO_ARRAY				301
#define ALIAS_TO_ORDERED_ARRAY		302
#define ALIAS_TO_ALT_ARRAY			303
#define ALIAS_TO_ALT_TEXT			304

// General property options
#define NO_OPTIONS					350
#define SCHEMA_NODE					351
#define PROP_VALUE_IS_URI			352
#define PROP_HAS_QUALIFIERS			353
#define PROP_IS_QUALIFIER			354
#define PROP_HAS_LANG				355
#define PROP_HAS_TYPE				356
#define PROP_IS_ALIAS				357
#define PROP_HAS_ALIASES			358
#define PROP_IS_INTERNAL			359
#define PROP_IS_STABLE				360
#define PROP_IS_DERIVED				361
#define PROP_IS_STALE				362

// Array options
#define	PROP_IS_STRUCT			400
#define	PROP_IS_ARRAY			401
#define	ARRAY_IS_UNORDERED		402
#define	ARRAY_IS_ORDERED		403
#define	ARRAY_IS_ALTERNATIVE	404
#define	ARRAY_IS_ALT_TEXT		405
#define	ARRAY_LAST_ITEM			406

// Serializing options
#define SERIALIZE_OMIT_PACKET_WRAPPER	500
#define SERIALIZE_READ_ONLY_PACKET		501
#define SERIALIZE_USE_COMPACT_FORMAT	502
// #define SERIALIZE_USE_PLAIN_XMP			503
#define SERIALIZE_INCLUDE_THUMBNAIL_PAD	504
#define SERIALIZE_EXACT_PACKET_LENGTH	505
#define SERIALIZE_WRITE_ALIAS_COMMENTS	506
#define SERIALIZE_OMIT_ALL_FORMATTING	507
#define ENCODE_UTF8						508
#define ENCODE_UTF16_BIG				509
#define ENCODE_UTF16_LITTLE				510
#define ENCODE_UTF32_BIG				511
#define ENCODE_UTF32_LITTLE				512

// Iterator options
#define ITERATOR_JUST_CHILDREN		600
#define ITERATOR_JUST_LEAFNODES		601
#define ITERATOR_JUST_LEAFNAME		602
#define ITERATOR_OMIT_QUALIFIERS	604

// XMPUtils constants
#define REMOVE_ALL_PROPERTIES		700
#define REMOVE_INCLUDE_ALIASES		701
#define APPEND_ALL_PROPERTIES		702
#define APPEND_REPLACE_OLD_VALUES	703
#define APPEND_DELETE_EMPTY_VALUES	704
#define SEPARATE_ALLOW_COMMAS		705

// XMPDateTime constants
#define TIME_WEST_OF_UTC			800
#define TIME_UTC					801
#define TIME_EAST_OF_UTC			802

//File formats suppported by XMPFile
#define FILE_UNKNOWN				1000
#define FILE_PDF					1001
#define FILE_POSTSCRIPT				1002
#define FILE_EPS					1003
#define FILE_JPEG					1004
#define FILE_JPEG2K					1005
#define FILE_TIFF					1006
#define FILE_GIF					1007
#define FILE_PNG					1008
#define FILE_SWF					1009
#define FILE_FLA					1010
#define FILE_FLV					1011
#define FILE_MOV					1012
#define FILE_AVI					1013
#define FILE_CIN					1014
#define FILE_WAV					1015
#define FILE_MP3					1016
#define FILE_SES					1017
#define FILE_CEL					1018
#define FILE_MPEG					1019
#define FILE_MPEG2					1020
#define FILE_MPEG4					1035
#define FILE_WMAV					1021
#define FILE_AIFF					1022
#define FILE_HTML					1023
#define FILE_XML				    1024
#define FILE_TEXT					1025
#define FILE_PHOTOSHOP				1026
#define FILE_ILLUSTRATOR			1027
#define FILE_INDESIGN				1028
#define FILE_AE_PROJECT				1029
#define FILE_AE_PROJECT_TEMPLATE	1030
#define FILE_AE_FILTER_PRESET		1031
#define FILE_ENCORE_PROJECT			1032
#define FILE_PREMIERE_PROJECT		1033
#define FILE_PREMIERE_TITLE			1034
#define FILE_SVG					1036
#define FILE_WEBP					1037

// Options for new XMPFile() to open a file containing Metadata
#define OPEN_FOR_READ				1100
#define OPEN_FOR_UPDATE				1101
#define OPEN_ONLY_XMP				1102
#define OPEN_CACHE_TNAIL			1103
#define OPEN_STRICTLY				1104
#define OPEN_USE_SMART_HANDLER		1105
#define OPEN_USE_PACKET_SCANNING	1106
#define OPEN_LIMITED_SCANNING		1107

// Options describing the smart handler abilities in XMPFile#getFormatInfo()
#define HANDLER_CAN_INJECT_XMP			1200
#define HANDLER_CAN_EXPAND				1201
#define HANDLER_CAN_REWRITE				1202
#define HANDLER_PPEFERS_IN_PLACE		1203
#define HANDLER_CAN_RECONCILE			1204
#define HANDLER_ALLOWS_ONLY_XMP			1205
#define HANDLER_RETURNS_RAW_PACKETS		1206
#define HANDLER_RETURNS_TNAIL			1207
#define HANDLER_OWNS_FILE				1208
#define HANDLER_ALLOWS_SAFE_UPDATE		1209
#define HANDLER_NEEDS_READONLY_PACKET	1210
#define HANDLER_USES_SIDECAR_XMP		1211
#define HANDLER_CAN_NOTIFY_PROGRESS		1212


// Options for XMPFile#close() to save the metadata an close the file
#define CLOSE_UPDATE_SAFELY			1300



static const ScCore::LivePropertyInfo INSTANCE_PROPERTIES[] = {
	// Miscellaneous constants
	{ "TRUE",		XMP_TRUE,	ROPROP, "string" },
	{ "FALSE",		XMP_FALSE,	ROPROP, "string" },
	{ "STRING",		STRING,		ROPROP, "string" },
	{ "INTEGER",	INTEGER,	ROPROP, "string" },
	{ "NUMBER",		NUMBER,		ROPROP, "string" },
	{ "BOOLEAN",	BOOLEAN,	ROPROP, "string" },
	{ "XMPDATE",	XMPDATE,	ROPROP, "string" },

	// Non-Adobe Schema Namespaces
	{ "NS_DC",					NS_DC,			ROPROP, "string" },
	{ "NS_IPTC_CORE",			NS_IPTC_CORE,	ROPROP, "string" },
	{ "NS_RDF",					NS_RDF,			ROPROP, "string" },
	{ "NS_XML",					NS_XML,			ROPROP, "string" },
	{ "NS_EXIFEX",				NS_EXIFEX,		ROPROP, "string" },

	// Adobe Schema Namespaces
	{ "NS_XMP",					NS_XMP,					ROPROP, "string" },
	{ "NS_XMP_RIGHTS",			NS_XMP_RIGHTS,			ROPROP, "string" },
	{ "NS_XMP_MM",				NS_XMP_MM,				ROPROP, "string" },
	{ "NS_XMP_BJ",				NS_XMP_BJ,				ROPROP, "string" },
	{ "NS_XMP_NOTE",			NS_XMP_NOTE,			ROPROP, "string" },
	{ "NS_PDF",					NS_PDF,					ROPROP, "string" },
	{ "NS_PDFX",				NS_PDFX,				ROPROP, "string" },
	{ "NS_PDFX_ID",				NS_PDFX_ID,				ROPROP, "string" },
	{ "NS_PHOTOSHOP",			NS_PHOTOSHOP,			ROPROP, "string" },
	{ "NS_PS_ALBUM",			NS_PS_ALBUM,			ROPROP, "string" },
	{ "NS_EXIF",				NS_EXIF,				ROPROP, "string" },
	{ "NS_EXIF_AUX",			NS_EXIF_AUX,			ROPROP, "string" },
	{ "NS_TIFF",				NS_TIFF,				ROPROP, "string" },
	{ "NS_PNG",					NS_PNG,					ROPROP, "string" },
	{ "NS_SWF",					NS_SWF,					ROPROP, "string" },
	{ "NS_JPEG",				NS_JPEG,				ROPROP, "string" },
	{ "NS_JP2K",				NS_JP2K,				ROPROP, "string" },
	{ "NS_CAMERA_RAW",			NS_CAMERA_RAW,			ROPROP, "string" },
	{ "NS_DM",					NS_DM,					ROPROP, "string" },
	{ "NS_ASF",					NS_ASF,					ROPROP, "string" },
	{ "NS_WAV",					NS_WAV,					ROPROP, "string" },
	{ "NS_ADOBE_STOCK_PHOTO",	NS_ADOBE_STOCK_PHOTO,	ROPROP, "string" },

	// Adobe Value Type Namespaces
	{ "TYPE_IDENTIFIER_QUAL",	TYPE_IDENTIFIER_QUAL,	ROPROP, "string" },
	{ "TYPE_DIMENSIONS",		TYPE_DIMENSIONS,		ROPROP, "string" },
	{ "TYPE_TEXT",				TYPE_TEXT,				ROPROP, "string" },
	{ "TYPE_PAGEDFILE",			TYPE_PAGEDFILE,			ROPROP, "string" },
	{ "TYPE_GRAPHICS",			TYPE_GRAPHICS,			ROPROP, "string" },
	{ "TYPE_IMAGE",				TYPE_IMAGE,				ROPROP, "string" },
	{ "TYPE_FONT",				TYPE_FONT,				ROPROP, "string" },
	{ "TYPE_RESOURCE_EVENT",	TYPE_RESOURCE_EVENT,	ROPROP, "string" },
	{ "TYPE_RESOURCE_REF",		TYPE_RESOURCE_REF,		ROPROP, "string" },
	{ "TYPE_ST_VERSION",		TYPE_ST_VERSION,		ROPROP, "string" },
	{ "TYPE_ST_JOB",			TYPE_ST_JOB,			ROPROP, "string" },
	{ "TYPE_MANIFEST_ITEM",		TYPE_MANIFEST_ITEM,		ROPROP, "string" },
	{ "TYPE_PDFA_SCHEMA",		TYPE_PDFA_SCHEMA,		ROPROP, "string" },
	{ "TYPE_PDFA_PROPERTY",		TYPE_PDFA_PROPERTY,		ROPROP, "string" },
	{ "TYPE_PDFA_TYPE",			TYPE_PDFA_TYPE,			ROPROP, "string" },
	{ "TYPE_PDFA_FIELD",		TYPE_PDFA_FIELD,		ROPROP, "string" },
	{ "TYPE_PDFA_ID",			TYPE_PDFA_ID,			ROPROP, "string" },
	{ "TYPE_PDFA_EXTENSION",	TYPE_PDFA_EXTENSION	,	ROPROP, "string" },

	// General property options
	{ "NO_OPTIONS",				NO_OPTIONS,				ROPROP, "number" },
	{ "SCHEMA_NODE",			SCHEMA_NODE	,			ROPROP, "number" },
	{ "PROP_VALUE_IS_URI",		PROP_VALUE_IS_URI,		ROPROP, "number" },
	{ "PROP_HAS_QUALIFIERS",	PROP_HAS_QUALIFIERS,	ROPROP, "number" },
	{ "PROP_IS_QUALIFIER",		PROP_IS_QUALIFIER,		ROPROP, "number" },
	{ "PROP_HAS_LANG",			PROP_HAS_LANG,			ROPROP, "number" },
	{ "PROP_HAS_TYPE",			PROP_HAS_TYPE,			ROPROP, "number" },
	{ "PROP_IS_ALIAS",			PROP_IS_ALIAS,			ROPROP, "number" },
	{ "PROP_HAS_ALIASES",		PROP_HAS_ALIASES,		ROPROP, "number" },
	{ "PROP_IS_INTERNAL",		PROP_IS_INTERNAL,		ROPROP, "number" },
	{ "PROP_IS_STABLE",			PROP_IS_STABLE,			ROPROP, "number" },
	{ "PROP_IS_DERIVED",		PROP_IS_DERIVED,		ROPROP, "number" },
	{ "PROP_IS_STALE",			PROP_IS_STALE,			ROPROP, "number" },

	// Alias array forms
	{ "ALIAS_TO_SIMPLE_PROP",	ALIAS_TO_SIMPLE_PROP,	ROPROP, "number" },
	{ "ALIAS_TO_ARRAY",			ALIAS_TO_ARRAY,			ROPROP, "number" },
	{ "ALIAS_TO_ORDERED_ARRAY",	ALIAS_TO_ORDERED_ARRAY,	ROPROP, "number" },
	{ "ALIAS_TO_ALT_ARRAY",		ALIAS_TO_ALT_ARRAY,		ROPROP, "number" },
	{ "ALIAS_TO_ALT_TEXT",		ALIAS_TO_ALT_TEXT,		ROPROP, "number" },

	// Array options
	{ "PROP_IS_STRUCT",			PROP_IS_STRUCT,			ROPROP, "number" },
	{ "PROP_IS_ARRAY",			PROP_IS_ARRAY,			ROPROP, "number" },
	{ "ARRAY_IS_UNORDERED",		ARRAY_IS_UNORDERED,		ROPROP, "number" },
	{ "ARRAY_IS_ORDERED",		ARRAY_IS_ORDERED,		ROPROP, "number" },
	{ "ARRAY_IS_ALTERNATIVE",	ARRAY_IS_ALTERNATIVE,	ROPROP, "number" },
	{ "ARRAY_IS_ALT_TEXT",		ARRAY_IS_ALT_TEXT,		ROPROP, "number" },
	{ "ARRAY_LAST_ITEM",		ARRAY_LAST_ITEM,		ROPROP, "number" },

	// Serialization constants
	{ "SERIALIZE_OMIT_PACKET_WRAPPER",		SERIALIZE_OMIT_PACKET_WRAPPER,		ROPROP, "number" },
	{ "SERIALIZE_READ_ONLY_PACKET",			SERIALIZE_READ_ONLY_PACKET,			ROPROP, "number" },
	{ "SERIALIZE_USE_COMPACT_FORMAT",		SERIALIZE_USE_COMPACT_FORMAT,		ROPROP, "number" },
//	{ "SERIALIZE_USE_PLAIN_XMP",			SERIALIZE_USE_PLAIN_XMP,			ROPROP, "number" },
	{ "SERIALIZE_INCLUDE_THUMBNAIL_PAD",	SERIALIZE_INCLUDE_THUMBNAIL_PAD,	ROPROP, "number" },
	{ "SERIALIZE_EXACT_PACKET_LENGTH",		SERIALIZE_EXACT_PACKET_LENGTH,		ROPROP, "number" },
	{ "SERIALIZE_WRITE_ALIAS_COMMENTS",		SERIALIZE_WRITE_ALIAS_COMMENTS,		ROPROP, "number" },
	{ "SERIALIZE_OMIT_ALL_FORMATTING",		SERIALIZE_OMIT_ALL_FORMATTING,		ROPROP, "number" },
	{ "ENCODE_UTF8",						ENCODE_UTF8,						ROPROP, "number" },
	{ "ENCODE_UTF16_BIG",					ENCODE_UTF16_BIG,					ROPROP, "number" },
	{ "ENCODE_UTF16_LITTLE",				ENCODE_UTF16_LITTLE,				ROPROP, "number" },
	{ "ENCODE_UTF32_BIG",					ENCODE_UTF32_BIG,					ROPROP, "number" },
	{ "ENCODE_UTF32_LITTLE",				ENCODE_UTF32_LITTLE,				ROPROP, "number" },

	// Iterator options
	{ "ITERATOR_JUST_CHILDREN",		ITERATOR_JUST_CHILDREN,		ROPROP, "number" },
	{ "ITERATOR_JUST_LEAFNODES",	ITERATOR_JUST_LEAFNODES,	ROPROP, "number" },
	{ "ITERATOR_JUST_LEAFNAME",		ITERATOR_JUST_LEAFNAME,		ROPROP, "number" },
	{ "ITERATOR_OMIT_QUALIFIERS",	ITERATOR_OMIT_QUALIFIERS,	ROPROP, "number" },

	// XMPUtils constants
	{ "REMOVE_ALL_PROPERTIES",		REMOVE_ALL_PROPERTIES,		ROPROP, "number" },
	{ "REMOVE_INCLUDE_ALIASES",		REMOVE_INCLUDE_ALIASES,		ROPROP, "number" },
	{ "APPEND_ALL_PROPERTIES",		APPEND_ALL_PROPERTIES,		ROPROP, "number" },
	{ "APPEND_REPLACE_OLD_VALUES",	APPEND_REPLACE_OLD_VALUES,	ROPROP, "number" },
	{ "APPEND_DELETE_EMPTY_VALUES",	APPEND_DELETE_EMPTY_VALUES,	ROPROP, "number" },
	{ "SEPARATE_ALLOW_COMMAS",		SEPARATE_ALLOW_COMMAS,		ROPROP, "number" },

	// XMPDateTime constants
	{ "TIME_WEST_OF_UTC",	TIME_WEST_OF_UTC,	ROPROP, "number" },
	{ "TIME_UTC",			TIME_UTC,			ROPROP, "number" },
	{ "TIME_EAST_OF_UTC",	TIME_EAST_OF_UTC,	ROPROP, "number" },

	// File formats supported by XMPFile
	{ "FILE_UNKNOWN",			FILE_UNKNOWN,   			ROPROP, "number" },
	{ "FILE_PDF",				FILE_PDF,  					ROPROP, "number" },
	{ "FILE_POSTSCRIPT",		FILE_POSTSCRIPT,			ROPROP, "number" },
	{ "FILE_EPS",				FILE_EPS,	 				ROPROP, "number" },
	{ "FILE_JPEG",				FILE_JPEG,   				ROPROP, "number" },
	{ "FILE_JPEG2K",			FILE_JPEG2K,				ROPROP, "number" },
	{ "FILE_TIFF",				FILE_TIFF,  				ROPROP, "number" },
	{ "FILE_GIF",				FILE_GIF, 					ROPROP, "number" },
	{ "FILE_PNG",				FILE_PNG, 					ROPROP, "number" },
	{ "FILE_SWF",				FILE_SWF, 					ROPROP, "number" },
	{ "FILE_FLA",				FILE_FLA, 					ROPROP, "number" },
	{ "FILE_FLV",				FILE_FLV, 					ROPROP, "number" },
	{ "FILE_MOV",				FILE_MOV,  					ROPROP, "number" },
	{ "FILE_AVI",				FILE_AVI,  					ROPROP, "number" },
	{ "FILE_CIN",				FILE_CIN,  					ROPROP, "number" },
	{ "FILE_WAV",				FILE_WAV,  					ROPROP, "number" },
	{ "FILE_MP3",				FILE_MP3,  					ROPROP, "number" },
	{ "FILE_SES",				FILE_SES,  					ROPROP, "number" },
	{ "FILE_CEL",				FILE_CEL,  					ROPROP, "number" },
	{ "FILE_MPEG",				FILE_MPEG,					ROPROP, "number" },	
	{ "FILE_MPEG2",				FILE_MPEG2,			 		ROPROP, "number" },
	{ "FILE_MPEG4",				FILE_MPEG4,			 		ROPROP, "number" },
	{ "FILE_WMAV",				FILE_WMAV,	 				ROPROP, "number" },
	{ "FILE_AIFF",				FILE_AIFF,					ROPROP, "number" },
	{ "FILE_HTML",				FILE_HTML,	  				ROPROP, "number" },
	{ "FILE_XML",				FILE_XML,					ROPROP, "number" },
	{ "FILE_TEXT",				FILE_TEXT,					ROPROP, "number" },
	{ "FILE_PHOTOSHOP",			FILE_PHOTOSHOP,			  	ROPROP, "number" },
	{ "FILE_ILLUSTRATOR",		FILE_ILLUSTRATOR,			ROPROP, "number" },
	{ "FILE_INDESIGN",			FILE_INDESIGN,				ROPROP, "number" },
	{ "FILE_AE_PROJECT",		FILE_AE_PROJECT,			ROPROP, "number" },
	{ "FILE_AE_PROJECT_TEMPLATE", FILE_AE_PROJECT_TEMPLATE,	ROPROP, "number" },
	{ "FILE_AE_FILTER_PRESET",	FILE_AE_FILTER_PRESET,		ROPROP, "number" },
	{ "FILE_ENCORE_PROJECT",	FILE_ENCORE_PROJECT,		ROPROP, "number" },
	{ "FILE_PREMIERE_PROJECT",	FILE_PREMIERE_PROJECT,		ROPROP, "number" },
	{ "FILE_PREMIERE_TITLE",	FILE_PREMIERE_TITLE,		ROPROP, "number" },
	{ "FILE_WEBP",				FILE_WEBP,					ROPROP, "number" },

	// Options for new XMPFile() to open a file containing Metadata
	{ "OPEN_FOR_READ",				OPEN_FOR_READ,				ROPROP, "number" },
	{ "OPEN_FOR_UPDATE",			OPEN_FOR_UPDATE,			ROPROP, "number" },
	{ "OPEN_ONLY_XMP",				OPEN_ONLY_XMP,				ROPROP, "number" },
	{ "OPEN_CACHE_TNAIL",			OPEN_CACHE_TNAIL,			ROPROP, "number" },
	{ "OPEN_STRICTLY",				OPEN_STRICTLY,				ROPROP, "number" },
	{ "OPEN_USE_SMART_HANDLER",		OPEN_USE_SMART_HANDLER,		ROPROP, "number" },
	{ "OPEN_USE_PACKET_SCANNING",	OPEN_USE_PACKET_SCANNING,	ROPROP, "number" },
	{ "OPEN_LIMITED_SCANNING",		OPEN_LIMITED_SCANNING,		ROPROP, "number" },

	// Options describing the smart handler abilities in XMPFile#getFormatInfo()
	{ "HANDLER_CAN_INJECT_XMP",			HANDLER_CAN_INJECT_XMP,			ROPROP, "number" },
	{ "HANDLER_CAN_EXPAND",				HANDLER_CAN_EXPAND,				ROPROP, "number" },
	{ "HANDLER_CAN_REWRITE",			HANDLER_CAN_REWRITE,			ROPROP, "number" },
	{ "HANDLER_PPEFERS_IN_PLACE",		HANDLER_PPEFERS_IN_PLACE,		ROPROP, "number" },
	{ "HANDLER_CAN_RECONCILE",			HANDLER_CAN_RECONCILE,			ROPROP, "number" },
	{ "HANDLER_ALLOWS_ONLY_XMP",		HANDLER_ALLOWS_ONLY_XMP,		ROPROP, "number" },
	{ "HANDLER_RETURNS_RAW_PACKETS",	HANDLER_RETURNS_RAW_PACKETS,	ROPROP, "number" },
	{ "HANDLER_RETURNS_TNAIL",			HANDLER_RETURNS_TNAIL,			ROPROP, "number" },
	{ "HANDLER_OWNS_FILE",				HANDLER_OWNS_FILE,				ROPROP, "number" },
	{ "HANDLER_ALLOWS_SAFE_UPDATE",		HANDLER_ALLOWS_SAFE_UPDATE,		ROPROP, "number" },
	{ "HANDLER_NEEDS_READONLY_PACKET",	HANDLER_NEEDS_READONLY_PACKET,	ROPROP, "number" },
	{ "HANDLER_USES_SIDECAR_XMP",		HANDLER_USES_SIDECAR_XMP,		ROPROP, "number" },
	{ "HANDLER_CAN_NOTIFY_PROGRESS",	HANDLER_CAN_NOTIFY_PROGRESS,	ROPROP, "number" },

	// Options for XMPFile#close() to save the metadata an close the file
	{ "CLOSE_UPDATE_SAFELY",		CLOSE_UPDATE_SAFELY,		ROPROP, "number" },

	{ null }
};


XMPConst::XMPConst() : XMPBase ("XMPConst"), properties (INSTANCE_PROPERTIES)
{
	setPropertyManager (&properties);
}


/** The overridden function prevents the new operator. */
scerror_t XMPConst::clone (LiveObject** newObj, bool) const
{
	(void)newObj;
	return kErrObjectExpected;
}


scerror_t XMPConst::get (int32_t id, ScCore::Variant& result, ScCore::Error* errs) const
{
	scerror_t errorCode = ScCore::kErrOK;
	switch (id)
	{
		// Miscellaneous constants
		case XMP_TRUE:		result.setString(kXMP_TrueStr); break;
		case XMP_FALSE:		result.setString(kXMP_FalseStr); break;
		case STRING:		result.setString("string"); break;
		case INTEGER:		result.setString("integer"); break;
		case NUMBER:		result.setString("number"); break;
		case BOOLEAN:		result.setString("boolean"); break;
		case XMPDATE:		result.setString("xmpdate"); break;

		// Non-Adobe Schema Namespaces
		case NS_DC:			result.setString(kXMP_NS_DC); break;
		case NS_IPTC_CORE:	result.setString(kXMP_NS_IPTCCore); break;
		case NS_RDF:		result.setString(kXMP_NS_RDF); break;
		case NS_XML:		result.setString(kXMP_NS_XML); break;
		case NS_EXIFEX:		result.setString(kXMP_NS_ExifEX); break;

		// Adobe Schema Namespaces
		case NS_XMP:		result.setString(kXMP_NS_XMP); break;
		case NS_XMP_RIGHTS:	result.setString(kXMP_NS_XMP_Rights); break;
		case NS_XMP_MM:		result.setString(kXMP_NS_XMP_MM); break;
		case NS_XMP_BJ:		result.setString(kXMP_NS_XMP_BJ); break;
		case NS_XMP_NOTE:	result.setString(kXMP_NS_XMP_Note); break;
		case NS_PDF:		result.setString(kXMP_NS_PDF); break;
		case NS_PDFX:		result.setString(kXMP_NS_PDFX); break;
		case NS_PDFX_ID:	result.setString(kXMP_NS_PDFX_ID); break;
		case NS_PHOTOSHOP:	result.setString(kXMP_NS_Photoshop); break;
		case NS_PS_ALBUM:	result.setString(kXMP_NS_PSAlbum); break;
		case NS_EXIF:		result.setString(kXMP_NS_EXIF); break;
		case NS_EXIF_AUX:	result.setString(kXMP_NS_EXIF_Aux); break;
		case NS_TIFF:		result.setString(kXMP_NS_TIFF); break;
		case NS_PNG:		result.setString(kXMP_NS_PNG); break;
		case NS_SWF:		result.setString(kXMP_NS_SWF); break;
		case NS_JPEG:		result.setString(kXMP_NS_JPEG); break;
		case NS_JP2K:		result.setString(kXMP_NS_JP2K); break;
		case NS_CAMERA_RAW:	result.setString(kXMP_NS_CameraRaw); break;
		case NS_DM:			result.setString(kXMP_NS_DM); break;
		case NS_ASF:		result.setString(kXMP_NS_ASF); break;
		case NS_WAV:		result.setString(kXMP_NS_WAV); break;
		case NS_ADOBE_STOCK_PHOTO:	result.setString(kXMP_NS_AdobeStockPhoto); break;

		// Adobe Value Type Namespaces
		case TYPE_IDENTIFIER_QUAL:	result.setString(kXMP_NS_XMP_IdentifierQual); break;
		case TYPE_DIMENSIONS:		result.setString(kXMP_NS_XMP_Dimensions); break;
		case TYPE_TEXT:				result.setString(kXMP_NS_XMP_Text); break;
		case TYPE_PAGEDFILE:		result.setString(kXMP_NS_XMP_PagedFile); break;
		case TYPE_GRAPHICS:			result.setString(kXMP_NS_XMP_Graphics); break;
		case TYPE_IMAGE:			result.setString(kXMP_NS_XMP_Image); break;
		case TYPE_FONT:				result.setString(kXMP_NS_XMP_Font); break;
		case TYPE_RESOURCE_EVENT:	result.setString(kXMP_NS_XMP_ResourceEvent); break;
		case TYPE_RESOURCE_REF:		result.setString(kXMP_NS_XMP_ResourceRef); break;
		case TYPE_ST_VERSION:		result.setString(kXMP_NS_XMP_ST_Version); break;
		case TYPE_ST_JOB:			result.setString(kXMP_NS_XMP_ST_Job); break;
		case TYPE_MANIFEST_ITEM:	result.setString(kXMP_NS_XMP_ManifestItem); break;
		case TYPE_PDFA_SCHEMA:		result.setString(kXMP_NS_PDFA_Schema); break;
		case TYPE_PDFA_PROPERTY:	result.setString(kXMP_NS_PDFA_Property); break;
		case TYPE_PDFA_TYPE:		result.setString(kXMP_NS_PDFA_Type); break;
		case TYPE_PDFA_FIELD:		result.setString(kXMP_NS_PDFA_Field); break;
		case TYPE_PDFA_ID:			result.setString(kXMP_NS_PDFA_ID); break;
		case TYPE_PDFA_EXTENSION:	result.setString(kXMP_NS_PDFA_Extension); break;

		// General property options
		case NO_OPTIONS:				result.setInteger((uint32_t)kXMP_NoOptions); break;
		case SCHEMA_NODE:				result.setInteger((uint32_t)kXMP_SchemaNode); break;
		case PROP_VALUE_IS_URI:			result.setInteger(kXMP_PropValueIsURI); break;
		case PROP_HAS_QUALIFIERS:		result.setInteger(kXMP_PropHasQualifiers); break;
		case PROP_IS_QUALIFIER:			result.setInteger(kXMP_PropIsQualifier); break;
		case PROP_HAS_LANG:				result.setInteger(kXMP_PropHasLang); break;
		case PROP_HAS_TYPE:				result.setInteger(kXMP_PropHasType); break;
		case PROP_IS_ALIAS:				result.setInteger(kXMP_PropIsAlias); break;
		case PROP_HAS_ALIASES:			result.setInteger(kXMP_PropHasAliases); break;
		case PROP_IS_INTERNAL:			result.setInteger(kXMP_PropIsInternal); break;
		case PROP_IS_STABLE:			result.setInteger(kXMP_PropIsStable); break;
		case PROP_IS_DERIVED:			result.setInteger(kXMP_PropIsDerived); break;
		case PROP_IS_STALE:				result.setInteger(kXMP_PropIsStale); break;

		// Alias array forms
		case ALIAS_TO_SIMPLE_PROP:		result.setInteger((uint32_t)kXMP_NoOptions); break;
		case ALIAS_TO_ARRAY:			result.setInteger(kXMP_PropValueIsArray); break;
		case ALIAS_TO_ORDERED_ARRAY:	result.setInteger(kXMP_PropArrayIsOrdered); break;
		case ALIAS_TO_ALT_ARRAY:		result.setInteger(kXMP_PropArrayIsAlternate); break;
		case ALIAS_TO_ALT_TEXT:			result.setInteger(kXMP_PropArrayIsAltText); break;

		// Array options
		case PROP_IS_STRUCT:			result.setInteger(kXMP_PropValueIsStruct); break;
		case PROP_IS_ARRAY:				result.setInteger(kXMP_PropValueIsArray); break;
		case ARRAY_IS_UNORDERED:		result.setInteger(kXMP_PropArrayIsUnordered); break;
		case ARRAY_IS_ORDERED:			result.setInteger(kXMP_PropArrayIsOrdered); break;
		case ARRAY_IS_ALTERNATIVE:		result.setInteger(kXMP_PropArrayIsAlternate); break;
		case ARRAY_IS_ALT_TEXT:			result.setInteger(kXMP_PropArrayIsAltText); break;
		case ARRAY_LAST_ITEM:			result.setInteger(kXMP_ArrayLastItem); break;

		// Parsing constants (if options are added to constructor)
		// case PARSE_REQUIRE_XMPMETA:		result.setInteger(kXMP_RequireXMPMeta); break;
		// case PARSE_STRICT_ALIASING:		result.setInteger(kXMP_StrictAliasing); break;

		// Serialization constants
		case SERIALIZE_OMIT_PACKET_WRAPPER:		result.setInteger(kXMP_OmitPacketWrapper); break;
		case SERIALIZE_READ_ONLY_PACKET:		result.setInteger(kXMP_ReadOnlyPacket); break;
		case SERIALIZE_USE_COMPACT_FORMAT:		result.setInteger(kXMP_UseCompactFormat); break;
//		case SERIALIZE_USE_PLAIN_XMP:			result.setInteger(kXMP_UsePlainXMP); break;
		case SERIALIZE_INCLUDE_THUMBNAIL_PAD:	result.setInteger(kXMP_IncludeThumbnailPad); break;
		case SERIALIZE_EXACT_PACKET_LENGTH:		result.setInteger(kXMP_ExactPacketLength); break;
		case SERIALIZE_OMIT_ALL_FORMATTING:		result.setInteger(kXMP_OmitAllFormatting); break;
		case ENCODE_UTF8:						result.setInteger(kXMP_EncodeUTF8); break;
		case ENCODE_UTF16_BIG:					result.setInteger(kXMP_EncodeUTF16Big); break;
		case ENCODE_UTF16_LITTLE:				result.setInteger(kXMP_EncodeUTF16Little); break;
		case ENCODE_UTF32_BIG:					result.setInteger(kXMP_EncodeUTF32Big); break;
		case ENCODE_UTF32_LITTLE:				result.setInteger(kXMP_EncodeUTF32Little); break;

		// Iterator Options
		case ITERATOR_JUST_CHILDREN:	result.setInteger(kXMP_IterJustChildren); break;
		case ITERATOR_JUST_LEAFNODES:	result.setInteger(kXMP_IterJustLeafNodes); break;
		case ITERATOR_JUST_LEAFNAME:	result.setInteger(kXMP_IterJustLeafName); break;
		case ITERATOR_OMIT_QUALIFIERS:	result.setInteger(kXMP_IterOmitQualifiers); break;

		// XMPUtils constants
		case REMOVE_ALL_PROPERTIES:			result.setInteger(kXMPUtil_DoAllProperties); break;
		case REMOVE_INCLUDE_ALIASES:		result.setInteger(kXMPUtil_IncludeAliases); break;
		case APPEND_ALL_PROPERTIES:			result.setInteger(kXMPUtil_DoAllProperties); break;
		case APPEND_REPLACE_OLD_VALUES:		result.setInteger(kXMPUtil_ReplaceOldValues); break;
		case APPEND_DELETE_EMPTY_VALUES:	result.setInteger(kXMPUtil_DeleteEmptyValues); break;
		case SEPARATE_ALLOW_COMMAS:			result.setInteger(kXMPUtil_AllowCommas); break;

		// XMPDateTime constants
		case TIME_WEST_OF_UTC:			result.setInteger(kXMP_TimeWestOfUTC); break;
		case TIME_UTC:					result.setInteger(kXMP_TimeIsUTC); break;
		case TIME_EAST_OF_UTC:			result.setInteger(kXMP_TimeEastOfUTC); break;

		// File formats supported by XMPFile
		case FILE_UNKNOWN:				result.setInteger(kXMP_UnknownFile); break;
		
		case FILE_PDF:					result.setInteger(kXMP_PDFFile); break;
		case FILE_POSTSCRIPT:			result.setInteger(kXMP_PostScriptFile); break;
		case FILE_EPS:					result.setInteger(kXMP_EPSFile); break;
		case FILE_JPEG:					result.setInteger(kXMP_JPEGFile); break;
		case FILE_JPEG2K:				result.setInteger(kXMP_JPEG2KFile); break;
		case FILE_TIFF:					result.setInteger(kXMP_TIFFFile); break;
		case FILE_GIF:					result.setInteger(kXMP_GIFFile); break;
		case FILE_PNG :					result.setInteger(kXMP_PNGFile); break;
		case FILE_SWF:					result.setInteger(kXMP_SWFFile); break;
		case FILE_FLA:					result.setInteger(kXMP_FLAFile); break;
		case FILE_FLV:					result.setInteger(kXMP_FLVFile); break;
		case FILE_MOV:					result.setInteger(kXMP_MOVFile); break;
		case FILE_AVI:					result.setInteger(kXMP_AVIFile); break;
		case FILE_CIN:					result.setInteger(kXMP_CINFile); break;
		case FILE_WAV:					result.setInteger(kXMP_WAVFile); break;
		case FILE_MP3:					result.setInteger(kXMP_MP3File); break;
		case FILE_SES:					result.setInteger(kXMP_SESFile); break;
		case FILE_CEL:					result.setInteger(kXMP_CELFile); break;
		case FILE_MPEG:					result.setInteger(kXMP_MPEGFile); break;
		case FILE_MPEG2:				result.setInteger(kXMP_MPEG2File); break;
		case FILE_MPEG4:				result.setInteger(kXMP_MPEG4File); break;
		case FILE_WMAV:					result.setInteger(kXMP_WMAVFile); break;
		case FILE_AIFF:					result.setInteger(kXMP_AIFFFile); break;
		case FILE_HTML:					result.setInteger(kXMP_HTMLFile); break;
		case FILE_XML:					result.setInteger(kXMP_XMLFile); break;
		case FILE_TEXT:					result.setInteger(kXMP_TextFile); break;
		case FILE_SVG:					result.setInteger( kXMP_SVGFile ); break;
		case FILE_PHOTOSHOP:			result.setInteger(kXMP_PhotoshopFile); break;
		case FILE_ILLUSTRATOR:			result.setInteger(kXMP_IllustratorFile); break;
		case FILE_INDESIGN:				result.setInteger(kXMP_InDesignFile); break;
		case FILE_AE_PROJECT:			result.setInteger(kXMP_AEProjectFile); break;
		case FILE_AE_PROJECT_TEMPLATE:	result.setInteger(kXMP_AEProjTemplateFile); break;
		case FILE_AE_FILTER_PRESET:		result.setInteger(kXMP_AEFilterPresetFile); break;
		case FILE_ENCORE_PROJECT:		result.setInteger(kXMP_EncoreProjectFile); break;
		case FILE_PREMIERE_PROJECT:		result.setInteger(kXMP_PremiereProjectFile); break;
		case FILE_PREMIERE_TITLE:		result.setInteger(kXMP_PremiereTitleFile); break;
		case FILE_WEBP:					result.setInteger(kXMP_WebPFile); break;

			// Options for new XMPFile() to open a file containing Metadata
		case OPEN_FOR_READ:				result.setInteger(kXMPFiles_OpenForRead); break;
		case OPEN_FOR_UPDATE:			result.setInteger(kXMPFiles_OpenForUpdate); break;
		case OPEN_ONLY_XMP:				result.setInteger(kXMPFiles_OpenOnlyXMP); break;
		case OPEN_STRICTLY:				result.setInteger(kXMPFiles_OpenStrictly); break;
		case OPEN_USE_SMART_HANDLER:	result.setInteger(kXMPFiles_OpenUseSmartHandler); break;
		case OPEN_USE_PACKET_SCANNING:	result.setInteger(kXMPFiles_OpenUsePacketScanning); break;
		case OPEN_LIMITED_SCANNING:		result.setInteger(kXMPFiles_OpenLimitedScanning); break;
		
		// Options describing the smart handler abilities in XMPFile#getFormatInfo()
		case HANDLER_CAN_INJECT_XMP:		result.setInteger(kXMPFiles_CanInjectXMP); break;
		case HANDLER_CAN_EXPAND:			result.setInteger(kXMPFiles_CanExpand); break;
		case HANDLER_CAN_REWRITE:			result.setInteger(kXMPFiles_CanRewrite); break;
		case HANDLER_PPEFERS_IN_PLACE:		result.setInteger(kXMPFiles_PrefersInPlace); break;
		case HANDLER_CAN_RECONCILE:			result.setInteger(kXMPFiles_CanReconcile); break;
		case HANDLER_ALLOWS_ONLY_XMP:		result.setInteger(kXMPFiles_AllowsOnlyXMP); break;
		case HANDLER_RETURNS_RAW_PACKETS:	result.setInteger(kXMPFiles_ReturnsRawPacket); break;
		case HANDLER_OWNS_FILE:				result.setInteger(kXMPFiles_HandlerOwnsFile); break;
		case HANDLER_ALLOWS_SAFE_UPDATE:	result.setInteger(kXMPFiles_AllowsSafeUpdate); break;
		case HANDLER_NEEDS_READONLY_PACKET:	result.setInteger(kXMPFiles_NeedsReadOnlyPacket); break;
		case HANDLER_USES_SIDECAR_XMP:		result.setInteger(kXMPFiles_UsesSidecarXMP); break;
		case HANDLER_CAN_NOTIFY_PROGRESS:   result.setInteger(kXMPFiles_CanNotifyProgress);break;
		
		// Options for XMPFile#close() to save the metadata an close the file
		case CLOSE_UPDATE_SAFELY:		result.setInteger(kXMPFiles_UpdateSafely); break;

		default: errorCode = LiveObject::get (id, result, errs);
	}
	return setError (errorCode, id, errs);
}


scerror_t XMPConst::enumerate (SimpleArray& propertyIDs, int32_t lang) const
{
	return properties.enumerate(propertyIDs, lang);
}
