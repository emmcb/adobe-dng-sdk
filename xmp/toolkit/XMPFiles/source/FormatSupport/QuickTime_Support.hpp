#ifndef __QuickTime_Support_hpp__
#define __QuickTime_Support_hpp__ 1

// =================================================================================================
// Copyright Adobe
// Copyright 2009 Adobe
// All Rights Reserved
//
// NOTICE: Adobe permits you to use, modify, and distribute this file in accordance with the terms
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
//
// mm-dd-yy who Description of changes, most recent on top
//
// 05-24-13 ADC 5.6-f062 [3564457] Cannot write property Dynamic Media / Disc Number into Quicktime files.
//
// 10-18-12 ADC 5.5-f052 Support for iTunes metadata.
//
// 08-17-10 AWL 5.3-f001 Integrate I/O revamp to main.
//
// 09-24-09 AWL 5.0-f081 Revamp MPEG-4 handler to keep the entire 'moov' subtree in memory.
// 08-28-09 AWL 5.0-f076 [2417142] Fix export of timecode-derived QT 'udta' items.
// 08-26-09 AWL 5.0-f074 [2398293] Fix MPEG-4 handler to not always think the iTunes metadata changed.
// 08-24-09 AWL 5.0-f069 Add support for QuickTime timecode information.
// 05-06-09 AWL 5.0-f035 [2307195] Fix Mac text conversions to work better on Windows.
// 03-10-09 AWL 5.0-f027 Initial integration of QuickTime support into MPEG-4 handler.
//
// =================================================================================================
#endif // AdobePrivate

#include "public/include/XMP_Environment.h"	// ! This must be the first include.

#include "public/include/XMP_Const.h"
#include "public/include/XMP_IO.hpp"

#include <string>
#include <vector>
#include <map>

#include "XMPFiles/source/XMPFiles_Impl.hpp"
#include "XMPFiles/source/FormatSupport/ISOBaseMedia_Support.hpp"
#include "XMPFiles/source/FormatSupport/MOOV_Support.hpp"

// =================================================================================================
// =================================================================================================

// =================================================================================================
// TradQT_Manager
// ==============

// Support for selected traditional QuickTime metadata items. The supported items are the children
// of the 'moov'/'udta' box whose type begins with 0xA9, a MacRoman copyright symbol. Each of these
// is a box whose contents are a sequence of "mini boxes" analogous to XMP AltText arrays. Each mini
// box has a 16-bit size, 16-bit language code, and text. The language code values are the old
// Macintosh Script Manager langXyz codes, the text encoding is implicit, see Mac Script.h.

enum {	// List of recognized items from the QuickTime 'moov'/'udta' box.
#if AdobePrivate
#if EnableQuickTimeMetadata
	// These items are defined by Apple.
	kQTilst_Album       = 0xA9616C62UL,	// '©alb'  (high order byte is MacRoman copyright symbol)
	kQTilst_Artist      = 0xA9415254UL,	// '©ART'
	kQTilst_Author      = 0xA9617574UL,	// '©aut'
	kQTilst_Comment     = 0xA9636D74UL,	// '©cmt'
	kQTilst_Copyright   = 0xA9637079UL,	// '©cpy'
	kQTilst_Description = 0xA9646573UL,	// '©des'
	kQTilst_Director    = 0xA9646972UL,	// '©dir'
	kQTilst_Information = 0xA9696E66UL,	// '©inf'
	kQTilst_Keywords    = 0xA96B6579UL,	// '©key'
	kQTilst_Producer    = 0xA9707264UL,	// '©prd'
	kQTilst_Software    = 0xA9737772UL,	// '©swr'
	kQTilst_Title       = 0xA96E616DUL,	// '©nam'
#endif
#endif
	// These items are defined by Adobe.
	kQTilst_Reel        = 0xA952454CUL, // '©REL'
	kQTilst_Timecode    = 0xA954494DUL, // '©TIM'
	kQTilst_TimeScale   = 0xA9545343UL, // '©TSC'
	kQTilst_TimeSize    = 0xA954535AUL  // '©TSZ'
};


enum {
	kNoMacLang   = 0xFFFF,
	kNoMacScript = 0xFFFF
};

enum {	// Values for the type field at the start of a 'data' box.
	kData_Implicit = 0,	// Implicit (unique) based on the box type
	kData_UTF_8 = 1,	// UTF-8 text, no nul terminator
	kData_UTF_16 = 2,	// Big endian UTF-16 text
	kData_ShiftJIS = 3,	// ShiftJIS text
};

extern bool ConvertToMacLang ( const std::string & utf8Value, XMP_Uns16 macLang, std::string * macValue );
extern bool ConvertFromMacLang ( const std::string & macValue, XMP_Uns16 macLang, std::string * utf8Value );

class TradQT_Manager {
public:

	TradQT_Manager() : changed(false) {};

	bool ParseCachedBoxes ( const MOOV_Manager & moovMgr );

	bool ImportSimpleXMP  ( XMP_Uns32 id, SXMPMeta * xmp, XMP_StringPtr ns, XMP_StringPtr prop ) const;
	bool ImportLangAltXMP ( XMP_Uns32 id, SXMPMeta * xmp, XMP_StringPtr ns, XMP_StringPtr langArray ) const;

	void ExportSimpleXMP  ( XMP_Uns32 id, const SXMPMeta & xmp, XMP_StringPtr ns, XMP_StringPtr prop,
							bool createWithZeroLang = false );
	void ExportLangAltXMP ( XMP_Uns32 id, const SXMPMeta & xmp, XMP_StringPtr ns, XMP_StringPtr langArray );

	bool IsChanged() const { return this->changed; };

	void UpdateChangedBoxes ( MOOV_Manager * moovMgr );

	//Functions for supporting "moov/meta" atom

#if AdobePrivate
	bool ParseMetaBox(const MOOV_Manager & moovMgr);

	bool ImportCreateDateMetaAtom(SXMPMeta * xmp);

	bool ImportLocationMetaAtom(SXMPMeta * xmp);

	bool ImportModelMetaAtom(SXMPMeta * xmp);

	bool FindMetaAtom(const std::string& id, std::string& prop , XMP_Uns32 & index );

	bool ConvertGPSToXMPFormat(const char* str, SXMPMeta * xmp, XMP_Uns32 len, bool priority);

#if 0
	bool ExportCreateDateMetaAtom(const SXMPMeta& xmp, MOOV_Manager* moovMgr);

	bool ExportLocationMetaAtom(const SXMPMeta& xmp, MOOV_Manager* moovMgr);
#endif

#endif

private:

	struct ValueInfo {
		bool marked;
		XMP_Uns16 macLang;
		XMP_StringPtr xmpLang;	// ! Only set if macLang is known, i.e. the value can be converted.
		std::string   macValue;
		ValueInfo() : marked(false), macLang(kNoMacLang), xmpLang("") {};
	};
	typedef std::vector<ValueInfo> ValueVector;
	typedef ValueVector::iterator ValueInfoPos;
	typedef ValueVector::const_iterator ValueInfoCPos;

	struct ParsedBoxInfo {
		XMP_Uns32 id;
		ValueVector values;
		bool changed;
		ParsedBoxInfo() : id(0), changed(false) {};
		ParsedBoxInfo ( XMP_Uns32 _id ) : id(_id), changed(false) {};
	};

	typedef std::map < XMP_Uns32, ParsedBoxInfo > InfoMap;	// Metadata item kind and content info.
	typedef InfoMap::iterator InfoMapPos;
	typedef InfoMap::const_iterator InfoMapCPos;

	InfoMap parsedBoxes;
	bool changed;

	bool ImportLangItem ( const ValueInfo & qtItem, SXMPMeta * xmp, XMP_StringPtr ns, XMP_StringPtr langArray ) const;

	//Data Structures to handle metadata in moov/meta atom

	struct DataBoxInfo {
		std::string value;
		XMP_Uns32 dataType;
		XMP_Uns32 localeInfo;
		DataBoxInfo(XMP_Uns32 dt, XMP_Uns32 l) : dataType(dt), localeInfo(l) {};
	};

	typedef std::vector<DataBoxInfo> DataBoxInfoVector;
	
	struct MetaItemBoxInfo {
		std::string key;
		XMP_Uns32  type;//key_index
		DataBoxInfoVector values;
		size_t firstText;	// Index of the first text value.
		MetaItemBoxInfo() : type(0), firstText(std::string::npos) {};
		MetaItemBoxInfo(XMP_Uns32 t) : type(t), firstText(std::string::npos) {};
	};

	typedef std::vector<MetaItemBoxInfo> MetaItemBoxInfoVector;
	MetaItemBoxInfoVector MetaBoxItemList;

	bool ConvertQTDateTime(XMP_StringPtr dateStr, XMP_DateTime* xmpDateTime);
	bool ConvertXMPDateTime(std::string& strValue);
	bool FormatLocationToGPSProperty(XMP_StringPtr, XMP_StringPtr, std::string&, XMP_Uns32 &);
	bool FormatGPSPropertyToLocation(std::string &xmpValue, XMP_StringPtr propName);
	bool isValidType(XMP_Uns32 typeCode);
	bool DecodeString(const std::string & iTunesValue, XMP_Uns16 typeCode, std::string * utf8Value);
	
};	// TradQT_Manager

#if AdobePrivate
#if EnableITunesMetadata
// =================================================================================================
// iTunes_Manager
// ==============
//
// Support for iTunes 'moov'/'udta'/'meta'/'ilst' metadata.

enum {	// List of recognized iTunes metadata items. All values are text unless noted.
	// Constants that are the same as traditional QuickTime.
	kiTunes_Album             = 0xA9616C62UL,	// '©alb'  (high order byte is MacRoman copyright symbol)
	kiTunes_Artist            = 0xA9415254UL,	// '©ART'
	kiTunes_Lyricist          = 0xA9617574UL,	// '©aut'
	kiTunes_Comment           = 0xA9636D74UL,	// '©cmt'
	kiTunes_Description       = 0xA9646573UL,	// '©des'
	kiTunes_Director          = 0xA9646972UL,	// '©dir'
	kiTunes_Producer          = 0xA9707264UL,	// '©prd'
	kiTunes_SongName          = 0xA96E616DUL,	// '©nam'
	// Other "0xA9" constants beyond traditional QuickTime.
	kiTunes_ReleaseDate       = 0xA9646179UL,	// '©day'
	kiTunes_EncodedBy         = 0xA9656E63UL,	// '©enc'
	kiTunes_UserGenres        = 0xA967656EUL,	// '©gen'	Comma separated list
	kiTunes_TrackSubTitle     = 0xA9737433UL,	// '©st3'
	kiTunes_EncodingTool      = 0xA9746F6FUL,	// '©too'
	kiTunes_Composer          = 0xA9777274UL,	// '©wrt'
	kiTunes_ArtDirector       = 0xA9617264UL,	// '©ard'
	kiTunes_Arranger          = 0xA9617267UL,	// '©arg'
	kiTunes_CopyrightAck      = 0xA963616BUL,	// '©cak'
	kiTunes_Conductor         = 0xA9636F6EUL,	// '©con'
	kiTunes_LinerNotes        = 0xA96C6E74UL,	// '©lnt'
	kiTunes_RecordCoURL       = 0xA96D616BUL,	// '©mak'
	kiTunes_OriginalArtist    = 0xA96F7065UL,	// '©ope'
	kiTunes_PhonogramRights   = 0xA9706867UL,	// '©phg'
	kiTunes_Performer         = 0xA9707266UL,	// '©prf'
	kiTunes_Publisher         = 0xA9707562UL,	// '©pub'
	kiTunes_SoundEngineer     = 0xA9736E65UL,	// '©sne'
	kiTunes_Soloist           = 0xA9736F6CUL,	// '©sol'
	kiTunes_Credits           = 0xA9737263UL,	// '©src'
	kiTunes_Dedications       = 0xA9746878UL,	// '©thx'
	kiTunes_OnlineExtras      = 0xA975726CUL,	// '©url'
	kiTunes_ExecutiveProducer = 0xA9787064UL,	// '©xpd'
	// Other non-"0xA9" constants.
	kiTunes_Copyright         = 0x63707274UL,	// 'cprt'
	kiTunes_StdGenres         = 0x676E7265UL,	// 'gnre'	UInt16 enumerations
	kiTunes_AlbumArtist       = 0x61415254UL,	// 'aART'
	kiTunes_Grouping          = 0x67727570UL,	// 'grup'
	kiTunes_HasRating         = 0x7274676EUL,	// 'rtgn'	Int8 Boolean
	kiTunes_Tempo             = 0x746D706FUL,	// 'tmpo'	Int16, beats/minute (specification says Int32 but not the case)
	kiTunes_TrackNumber       = 0x74726b6eUL,	// 'trkn'	implicit
	kiTunes_DiskNumber        = 0x6469736bUL,	// 'disk'	implicit
};

enum {	// Values for the type field at the start of a 'data' box.
	kiTunesData_Implicit =  0,	// Implicit (unique) based on the box type
	kiTunesData_UTF_8    =  1,	// UTF-8 text, no nul terminator
	kiTunesData_UTF_16   =  2,	// Big endian UTF-16 text
	kiTunesData_ShiftJIS =  3,	// ShiftJIS text
	kiTunesData_HTML     =  6,	// HTML text
	kiTunesData_XML      =  7,	// XML text
	kiTunesData_UUID     =  8,	// 16 binary bytes
	kiTunesData_ISRC     =  9,	// UTF-8 text, no nul terminator
	kiTunesData_MI3P     = 10,	// UTF-8 text, no nul terminator
	kiTunesData_GIF      = 12,	// GIF image
	kiTunesData_JPEG     = 13,	// JPEG image
	kiTunesData_PNG      = 14,	// PNG image
	kiTunesData_URL      = 15,	// UTF-8 text, no nul terminator
	kiTunesData_Duration = 16,	// Big endian UInt32 milliseconds
	kiTunesData_DateTime = 17,	// Big endian UInt32 or UInt64, UTC, seconds since midnight Jan 1 1904
	kiTunesData_Genres   = 18,	// List of big endian UInt16 genres
	kiTunesData_Integer  = 21,	// Big endian signed integer, 1, 2, 3, 4, or 8 bytes
	kiTunesData_RIAA_PA  = 24,	// RIAA Parental Advisory, UInt8
	kiTunesData_UPC      = 25,	// UPC code as UTF-8 text
	kiTunesData_BMP      = 27	// BMP image.
};

// The UInt16 genres are actually UInt8 pairs. The high order byte is a set code and the low order
// byte is a genre within the set. Set 0 are the ID3 genres, but the low order byte is ID3 plus 1.

enum { kMaxGenreID = 126 };

static const char * kGenreNames [kMaxGenreID+2] = {	// Array index is the ID3 value.
	/*   0 */ "Blues",       "Classic Rock",      "Country",           "Dance",            "Disco",
	/*   5 */ "Funk",        "Grunge",            "Hip-Hop",           "Jazz",             "Metal",
	/*  10 */ "New Age",     "Oldies",            "Other",             "Pop",              "R&B",
	/*  15 */ "Rap",         "Reggae",            "Rock",              "Techno",           "Industrial",
	/*  20 */ "Alternative", "Ska",               "Death Metal",       "Pranks",           "Soundtrack",
	/*  25 */ "Euro-Techno", "Ambient",           "Trip-Hop",          "Vocal",            "Jazz+Funk",
	/*  30 */ "Fusion",      "Trance",            "Classical",         "Instrumental",     "Acid",
	/*  35 */ "House",       "Game",              "Sound Clip",        "Gospel",           "Noise",
	/*  40 */ "AlternRock",  "Bass",              "Soul",              "Punk",             "Space",
	/*  45 */ "Meditative",  "Instrumental Pop",  "Instrumental Rock", "Ethnic",           "Gothic",
	/*  50 */ "Darkwave",    "Techno-Industrial", "Electronic",        "Pop-Folk",         "Eurodance",
	/*  55 */ "Dream",       "Southern Rock",     "Comedy",            "Cult",             "Gangsta",
	/*  60 */ "Top 40",      "Christian Rap",     "Pop/Funk",          "Jungle",           "Native American",
	/*  65 */ "Cabaret",     "New Wave",          "Psychadelic",       "Rave",             "Showtunes",
	/*  70 */ "Trailer",     "Lo-Fi",             "Tribal",            "Acid Punk",        "Acid Jazz",
	/*  75 */ "Polka",       "Retro",             "Musical",           "Rock & Roll",      "Hard Rock",
	/*  80 */ "Folk",        "Folk-Rock",         "National Folk",     "Swing",            "Fast Fusion",
	/*  85 */ "Bebob",       "Latin",             "Revival",           "Celtic",           "Bluegrass",
	/*  90 */ "Avantgarde",  "Gothic Rock",       "Progressive Rock",  "Psychedelic Rock", "Symphonic Rock",
	/*  95 */ "Slow Rock",   "Big Band",          "Chorus",            "Easy Listening",   "Acoustic",
	/* 100 */ "Humour",      "Speech",            "Chanson",           "Opera",            "Chamber Music",
	/* 105 */ "Sonata",      "Symphony",          "Booty Bass",        "Primus",           "Porn Groove",
	/* 110 */ "Satire",      "Slow Jam",          "Club",              "Tango",            "Samba",
	/* 115 */ "Folklore",    "Ballad",            "Power Ballad",      "Rhythmic Soul",    "Freestyle",
	/* 120 */ "Duet",        "Punk Rock",         "Drum Solo",         "A capella",        "Euro-House",
	/* 125 */ "Dance Hall",  "Unknown",
			  ""	// Final empty string sentinel.
};

class iTunes_Manager {
public:

	iTunes_Manager() : changed(false) {};

	bool ParseCachedBoxes ( const MOOV_Manager & moovMgr );

	bool IsChanged() const { return this->changed; };

	void UpdateChangedBoxes ( MOOV_Manager * moovMgr );

	bool GetName ( std::string & song ) const;
	bool GetArtist ( std::string & artist ) const;
	bool GetAlbumArtist ( std::string & albumArtist ) const;
	bool GetAlbum ( std::string & album ) const;
	bool GetGenre ( std::string & genre ) const;
	bool GetReleaseDate ( XMP_DateTime & releaseDate ) const;
	bool GetComposer ( std::string & composer ) const;
	bool GetComments ( std::string & comments ) const;
	bool GetTrackNumber ( XMP_Uns16 & trackNumber ) const;
	bool GetDiskNumber ( XMP_Uns16 & diskNumber, XMP_Uns16 & totalDisks ) const;
	bool GetTempo ( XMP_Uns16 & tempo ) const;
	bool GetCopyright ( std::string & copyright ) const;

	void RemoveName ();
	void RemoveArtist ();
	void RemoveAlbumArtist ();
	void RemoveAlbum ();
	void RemoveGenre ();
	void RemoveReleaseDate ();
	void RemoveComposer ();
	void RemoveComments ();
	void RemoveTrackNumber ();
	void RemoveDiskNumber ();
	void RemoveTempo ();
	void RemoveCopyright ();

	void SetName ( const std::string & name );
	void SetArtist ( const std::string & artist );
	void SetAlbumArtist ( const std::string albumArtist );
	void SetAlbum ( const std::string & album );
	void SetGenre ( const std::string & genre );
	void SetReleaseDate ( XMP_DateTime releaseDate );
	void SetComposer ( const std::string composer );
	void SetComments ( const std::string comments );
	void SetTrackNumber ( XMP_Uns16 trackNumber );
	void SetDiskNumber ( XMP_Uns16 diskNumber, XMP_Uns16 totalDisks );
	void SetTempo ( XMP_Uns16 tempo );
	void SetCopyright ( const std::string & copyright );

private:

	bool GetTextValue ( XMP_Uns32 id, std::string & utf8Value ) const;
	bool GetTextValue ( const std::string & fullName, std::string & utf8Value ) const;

	void SetTextValue ( XMP_Uns32 id, const std::string & utf8Value, XMP_Uns16 typeCode );
	void SetTextValue ( const std::string & fullName, const std::string & utf8Value, XMP_Uns16 typeCode );

	bool GetDataBuffer ( XMP_Uns32 id, std::string & data, XMP_Uns16 typeCode = kiTunesData_Implicit ) const;
	void SetDataBuffer ( XMP_Uns32 id, const std::string & data, XMP_Uns16 typeCode = kiTunesData_Implicit);

	bool GetNumericValue ( XMP_Uns32 id, XMP_Int64 & value, XMP_Uns8 nBytes = 8 ) const;
	void SetNumericValue ( XMP_Uns32 id, XMP_Int64 value, XMP_Uns8 nBytes = 8 );

	void DeleteItem ( XMP_Uns32 id );
	void DeleteItem ( const std::string & fullName );

	struct DataBoxInfo {
		XMP_Uns16   typeCode;
		XMP_Uns32   locale;
		std::string value;
		DataBoxInfo() : typeCode(0), locale(0) {};
		DataBoxInfo ( XMP_Uns16 t, XMP_Uns32 l ) : typeCode(t), locale(l) {};
	};
	typedef std::vector<DataBoxInfo> DataVector;

	struct PlainBoxInfo {	// For metadata items with outer box type other than '----'.
		XMP_Uns32  type;
		DataVector values;
		size_t firstText;	// Index of the first text value.
		bool changed;
		PlainBoxInfo() : type(0), firstText(std::string::npos), changed(false) {};
		PlainBoxInfo ( XMP_Uns32 t ) : type(t), firstText(std::string::npos), changed(false) {};
	};

	struct FancyBoxInfo {	// For metadata items with outer box type '----'.
		XMP_Uns32   meanVFlags, nameVFlags;	// The 'mean' and 'name' boxes are FullBox derivatives.
		std::string meanStr, nameStr;	// The nameStr is empty if there is no 'name' box.
		DataVector  values;
		size_t firstText;	// Index of the first text value.
		bool changed;
		FancyBoxInfo() : meanVFlags(0), nameVFlags(0), firstText(std::string::npos), changed(false) {};
		FancyBoxInfo ( XMP_Uns32 m, XMP_Uns32 n) : meanVFlags(m), nameVFlags(n), firstText(std::string::npos), changed(false) {};
	};

	typedef std::map < XMP_Uns32, PlainBoxInfo > PlainMap;	// Key is the box type.
	typedef PlainMap::iterator PlainMapPos;
	typedef PlainMap::const_iterator PlainMapCPos;

	typedef std::map < std::string, FancyBoxInfo > FancyMap;	// Key is the full name.
	typedef FancyMap::iterator FancyMapPos;
	typedef FancyMap::const_iterator FancyMapCPos;

	PlainMap plainItems;
	FancyMap fancyItems;

	bool changed;

	bool AppendOneValue ( const XMP_Uns8 * contentPtr, XMP_Uns32 contentSize, DataVector * values );

	static void CreateDataBox ( const DataBoxInfo & dataInfo,
								const MOOV_Manager::BoxRef parentRef, MOOV_Manager * moovMgr );

	static size_t FindFancyBox ( const FancyBoxInfo & fancyInfo, const MOOV_Manager & moovMgr,
								 const MOOV_Manager::BoxRef ilstRef, const MOOV_Manager::BoxInfo & ilstInfo );

};	// iTunes_Manager
#endif
#endif

#endif	// __QuickTime_Support_hpp__
