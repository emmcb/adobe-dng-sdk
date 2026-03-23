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
// 12-11-13 HK  5.6-f086 [3682902] Defining mapping of Mac language to available encodings on IOS.
// 12-04-13 HK  5.6-f085 [3677602] Fixing Unicode conversion issues on IOS.
// 05-24-13 ADC 5.6-f062 [3564457] Cannot write property Dynamic Media / Disc Number into Quicktime files.
//
// 10-18-12 ADC 5.5-f052 Support for iTunes metadata.
//
// 08-18-10 AWL 5.3-f002 Don't include XIO.hpp in any headers, only .cpp files.
//
// 02-05-10 AWL 5.1-f003 Fix build warnings from Visual Studio.
//
// 10-06-09 AWL 5.0-f084 [2426060,2434421,2424514] Fix TrimTrailingSpaces in ReconcileTIFF.cpp to handle all
//				blank input. Fix Mac script conversions to use the right script. Fix MPEG-4 imports
//				to not let one failure abort all. Fix TIFF handler to keep the XMP the same size if possible.
// 09-24-09 AWL 5.0-f081 Revamp MPEG-4 handler to keep the entire 'moov' subtree in memory.
// 08-28-09 AWL 5.0-f076 [2417142] Fix export of timecode-derived QT 'udta' items.
// 08-26-09 AWL 5.0-f074 [2398293] Fix MPEG-4 handler to not always think the iTunes metadata changed.
// 08-24-09 AWL 5.0-f069 Add support for QuickTime timecode information.
// 05-06-09 AWL 5.0-f035 [2307195] Fix Mac text conversions to work better on Windows.
// 04-20-09 AWL 5.0-f033 [2307195] Add language parameter for Mac text conversions.
// 03-26-09 AWL 5.0-f029 More QT related fixes. Properly handle OpenStrictly for MPEG-4 and MOV.
//				FIll in remaining Mac langXyz to XMP (ISO 639) mappings. Add 0xA9 to set of OK chars
//				for unexpected top level boxes in CheckFormat. Fix byte order of ISO 'cprt' language.
//				Output iTunes metadata if it did not yet exist.
// 03-10-09 AWL 5.0-f027 Initial integration of QuickTime support into MPEG-4 handler.
//
// =================================================================================================
#endif // AdobePrivate

#include "public/include/XMP_Environment.h"	// ! XMP_Environment.h must be the first included header.
#include "public/include/XMP_Const.h"

#if XMP_MacBuild
	#include <CoreServices/CoreServices.h>
#elif XMP_iOSBuild
    #include <CoreFoundation/CoreFoundation.h>
    #include "XMPFiles/source/FormatSupport/MacScriptExtracts.h"
#else
	#include "XMPFiles/source/FormatSupport/MacScriptExtracts.h"
#endif

#include "XMPFiles/source/FormatSupport/QuickTime_Support.hpp"

#include "source/UnicodeConversions.hpp"
#include "source/UnicodeInlines.incl_cpp"
#include "XMPFiles/source/FormatSupport/Reconcile_Impl.hpp"
#include "source/XIO.hpp"
#include "source/EndianUtils.hpp"

// =================================================================================================

static const char * kMacRomanUTF8 [128] = {	// UTF-8 mappings for MacRoman 80..FF.
	"\xC3\x84", "\xC3\x85", "\xC3\x87", "\xC3\x89", "\xC3\x91", "\xC3\x96", "\xC3\x9C", "\xC3\xA1",
	"\xC3\xA0", "\xC3\xA2", "\xC3\xA4", "\xC3\xA3", "\xC3\xA5", "\xC3\xA7", "\xC3\xA9", "\xC3\xA8",
	"\xC3\xAA", "\xC3\xAB", "\xC3\xAD", "\xC3\xAC", "\xC3\xAE", "\xC3\xAF", "\xC3\xB1", "\xC3\xB3",
	"\xC3\xB2", "\xC3\xB4", "\xC3\xB6", "\xC3\xB5", "\xC3\xBA", "\xC3\xB9", "\xC3\xBB", "\xC3\xBC",
	"\xE2\x80\xA0", "\xC2\xB0", "\xC2\xA2", "\xC2\xA3", "\xC2\xA7", "\xE2\x80\xA2", "\xC2\xB6", "\xC3\x9F",
	"\xC2\xAE", "\xC2\xA9", "\xE2\x84\xA2", "\xC2\xB4", "\xC2\xA8", "\xE2\x89\xA0", "\xC3\x86", "\xC3\x98",
	"\xE2\x88\x9E", "\xC2\xB1", "\xE2\x89\xA4", "\xE2\x89\xA5", "\xC2\xA5", "\xC2\xB5", "\xE2\x88\x82", "\xE2\x88\x91",
	"\xE2\x88\x8F", "\xCF\x80", "\xE2\x88\xAB", "\xC2\xAA", "\xC2\xBA", "\xCE\xA9", "\xC3\xA6", "\xC3\xB8",
	"\xC2\xBF", "\xC2\xA1", "\xC2\xAC", "\xE2\x88\x9A", "\xC6\x92", "\xE2\x89\x88", "\xE2\x88\x86", "\xC2\xAB",
	"\xC2\xBB", "\xE2\x80\xA6", "\xC2\xA0", "\xC3\x80", "\xC3\x83", "\xC3\x95", "\xC5\x92", "\xC5\x93",
	"\xE2\x80\x93", "\xE2\x80\x94", "\xE2\x80\x9C", "\xE2\x80\x9D", "\xE2\x80\x98", "\xE2\x80\x99", "\xC3\xB7", "\xE2\x97\x8A",
	"\xC3\xBF", "\xC5\xB8", "\xE2\x81\x84", "\xE2\x82\xAC", "\xE2\x80\xB9", "\xE2\x80\xBA", "\xEF\xAC\x81", "\xEF\xAC\x82",
	"\xE2\x80\xA1", "\xC2\xB7", "\xE2\x80\x9A", "\xE2\x80\x9E", "\xE2\x80\xB0", "\xC3\x82", "\xC3\x8A", "\xC3\x81",
	"\xC3\x8B", "\xC3\x88", "\xC3\x8D", "\xC3\x8E", "\xC3\x8F", "\xC3\x8C", "\xC3\x93", "\xC3\x94",
	"\xEF\xA3\xBF", "\xC3\x92", "\xC3\x9A", "\xC3\x9B", "\xC3\x99", "\xC4\xB1", "\xCB\x86", "\xCB\x9C",
	"\xC2\xAF", "\xCB\x98", "\xCB\x99", "\xCB\x9A", "\xC2\xB8", "\xCB\x9D", "\xCB\x9B", "\xCB\x87"
};

static const XMP_Uns32 kMacRomanCP [128] = {	// Unicode codepoints for MacRoman 80..FF.
	0x00C4, 0x00C5, 0x00C7, 0x00C9, 0x00D1, 0x00D6, 0x00DC, 0x00E1,
	0x00E0, 0x00E2, 0x00E4, 0x00E3, 0x00E5, 0x00E7, 0x00E9, 0x00E8,
	0x00EA, 0x00EB, 0x00ED, 0x00EC, 0x00EE, 0x00EF, 0x00F1, 0x00F3,
	0x00F2, 0x00F4, 0x00F6, 0x00F5, 0x00FA, 0x00F9, 0x00FB, 0x00FC,
	0x2020, 0x00B0, 0x00A2, 0x00A3, 0x00A7, 0x2022, 0x00B6, 0x00DF,
	0x00AE, 0x00A9, 0x2122, 0x00B4, 0x00A8, 0x2260, 0x00C6, 0x00D8,
	0x221E, 0x00B1, 0x2264, 0x2265, 0x00A5, 0x00B5, 0x2202, 0x2211,
	0x220F, 0x03C0, 0x222B, 0x00AA, 0x00BA, 0x03A9, 0x00E6, 0x00F8,
	0x00BF, 0x00A1, 0x00AC, 0x221A, 0x0192, 0x2248, 0x2206, 0x00AB,
	0x00BB, 0x2026, 0x00A0, 0x00C0, 0x00C3, 0x00D5, 0x0152, 0x0153,
	0x2013, 0x2014, 0x201C, 0x201D, 0x2018, 0x2019, 0x00F7, 0x25CA,
	0x00FF, 0x0178, 0x2044, 0x20AC, 0x2039, 0x203A, 0xFB01, 0xFB02,
	0x2021, 0x00B7, 0x201A, 0x201E, 0x2030, 0x00C2, 0x00CA, 0x00C1,
	0x00CB, 0x00C8, 0x00CD, 0x00CE, 0x00CF, 0x00CC, 0x00D3, 0x00D4,
	0xF8FF, 0x00D2, 0x00DA, 0x00DB, 0x00D9, 0x0131, 0x02C6, 0x02DC,	// ! U+F8FF is private use solid Apple icon.
	0x00AF, 0x02D8, 0x02D9, 0x02DA, 0x00B8, 0x02DD, 0x02DB, 0x02C7
};

// -------------------------------------------------------------------------------------------------

static const XMP_Uns16 kMacLangToScript_0_94 [95] = {

	/* langEnglish (0) */		smRoman,
	/* langFrench (1) */		smRoman,
	/* langGerman (2) */		smRoman,
	/* langItalian (3) */		smRoman,
	/* langDutch (4) */			smRoman,
	/* langSwedish (5) */		smRoman,
	/* langSpanish (6) */		smRoman,
	/* langDanish (7) */		smRoman,
	/* langPortuguese (8) */	smRoman,
	/* langNorwegian (9) */		smRoman,

	/* langHebrew (10) */		smHebrew,
	/* langJapanese (11) */		smJapanese,
	/* langArabic (12) */		smArabic,
	/* langFinnish (13) */		smRoman,
	/* langGreek (14) */		smRoman,
	/* langIcelandic (15) */	smRoman,
	/* langMaltese (16) */		smRoman,
	/* langTurkish (17) */		smRoman,
	/* langCroatian (18) */		smRoman,
	/* langTradChinese (19) */	smTradChinese,

	/* langUrdu (20) */			smArabic,
	/* langHindi (21) */		smDevanagari,
	/* langThai (22) */			smThai,
	/* langKorean (23) */		smKorean,
	/* langLithuanian (24) */	smCentralEuroRoman,
	/* langPolish (25) */		smCentralEuroRoman,
	/* langHungarian (26) */	smCentralEuroRoman,
	/* langEstonian (27) */		smCentralEuroRoman,
	/* langLatvian (28) */		smCentralEuroRoman,
	/* langSami (29) */			kNoMacScript,	// ! Not known, missing from Apple comments.

	/* langFaroese (30) */		smRoman,
	/* langFarsi (31) */		smArabic,
	/* langRussian (32) */		smCyrillic,
	/* langSimpChinese (33) */	smSimpChinese,
	/* langFlemish (34) */		smRoman,
	/* langIrishGaelic (35) */	smRoman,
	/* langAlbanian (36) */		smRoman,
	/* langRomanian (37) */		smRoman,
	/* langCzech (38) */		smCentralEuroRoman,
	/* langSlovak (39) */		smCentralEuroRoman,

	/* langSlovenian (40) */	smRoman,
	/* langYiddish (41) */		smHebrew,
	/* langSerbian (42) */		smCyrillic,
	/* langMacedonian (43) */	smCyrillic,
	/* langBulgarian (44) */	smCyrillic,
	/* langUkrainian (45) */	smCyrillic,
	/* langBelorussian (46) */	smCyrillic,
	/* langUzbek (47) */		smCyrillic,
	/* langKazakh (48) */		smCyrillic,
	/* langAzerbaijani (49) */	smCyrillic,

	/* langAzerbaijanAr (50) */	smArabic,
	/* langArmenian (51) */		smArmenian,
	/* langGeorgian (52) */		smGeorgian,
	/* langMoldavian (53) */	smCyrillic,
	/* langKirghiz (54) */		smCyrillic,
	/* langTajiki (55) */		smCyrillic,
	/* langTurkmen (56) */		smCyrillic,
	/* langMongolian (57) */	smMongolian,
	/* langMongolianCyr (58) */	smCyrillic,
	/* langPashto (59) */		smArabic,

	/* langKurdish (60) */		smArabic,
	/* langKashmiri (61) */		smArabic,
	/* langSindhi (62) */		smArabic,
	/* langTibetan (63) */		smTibetan,
	/* langNepali (64) */		smDevanagari,
	/* langSanskrit (65) */		smDevanagari,
	/* langMarathi (66) */		smDevanagari,
	/* langBengali (67) */		smBengali,
	/* langAssamese (68) */		smBengali,
	/* langGujarati (69) */		smGujarati,

	/* langPunjabi (70) */		smGurmukhi,
	/* langOriya (71) */		smOriya,
	/* langMalayalam (72) */	smMalayalam,
	/* langKannada (73) */		smKannada,
	/* langTamil (74) */		smTamil,
	/* langTelugu (75) */		smTelugu,
	/* langSinhalese (76) */	smSinhalese,
	/* langBurmese (77) */		smBurmese,
	/* langKhmer (78) */		smKhmer,
	/* langLao (79) */			smLao,

	/* langVietnamese (80) */	smVietnamese,
	/* langIndonesian (81) */	smRoman,
	/* langTagalog (82) */		smRoman,
	/* langMalayRoman (83) */	smRoman,
	/* langMalayArabic (84) */	smArabic,
	/* langAmharic (85) */		smEthiopic,
	/* langTigrinya (86) */		smEthiopic,
	/* langOromo (87) */		smEthiopic,
	/* langSomali (88) */		smRoman,
	/* langSwahili (89) */		smRoman,

	/* langKinyarwanda (90) */	smRoman,
	/* langRundi (91) */		smRoman,
	/* langNyanja (92) */		smRoman,
	/* langMalagasy (93) */		smRoman,
	/* langEsperanto (94) */	smRoman

};	// kMacLangToScript_0_94

static const XMP_Uns16 kMacLangToScript_128_151 [24] = {

	/* langWelsh (128) */				smRoman,
	/* langBasque (129) */				smRoman,

	/* langCatalan (130) */				smRoman,
	/* langLatin (131) */				smRoman,
	/* langQuechua (132) */				smRoman,
	/* langGuarani (133) */				smRoman,
	/* langAymara (134) */				smRoman,
	/* langTatar (135) */				smCyrillic,
	/* langUighur (136) */				smArabic,
	/* langDzongkha (137) */			smTibetan,
	/* langJavaneseRom (138) */			smRoman,
	/* langSundaneseRom (139) */		smRoman,

	/* langGalician (140) */			smRoman,
	/* langAfrikaans (141) */			smRoman,
	/* langBreton (142) */				smRoman,
	/* langInuktitut (143) */			smEthiopic,
	/* langScottishGaelic (144) */		smRoman,
	/* langManxGaelic (145) */			smRoman,
	/* langIrishGaelicScript (146) */	smRoman,
	/* langTongan (147) */				smRoman,
	/* langGreekAncient (148) */		smGreek,
	/* langGreenlandic (149) */			smRoman,

	/* langAzerbaijanRoman (150) */		smRoman,
	/* langNynorsk (151) */				smRoman
	
};	// kMacLangToScript_128_151

// -------------------------------------------------------------------------------------------------

static const char * kMacToXMPLang_0_94 [95] = {

	/* langEnglish (0) */		"en",
	/* langFrench (1) */		"fr",
	/* langGerman (2) */		"de",
	/* langItalian (3) */		"it",
	/* langDutch (4) */			"nl",
	/* langSwedish (5) */		"sv",
	/* langSpanish (6) */		"es",
	/* langDanish (7) */		"da",
	/* langPortuguese (8) */	"pt",
	/* langNorwegian (9) */		"no",

	/* langHebrew (10) */		"he",
	/* langJapanese (11) */		"ja",
	/* langArabic (12) */		"ar",
	/* langFinnish (13) */		"fi",
	/* langGreek (14) */		"el",
	/* langIcelandic (15) */	"is",
	/* langMaltese (16) */		"mt",
	/* langTurkish (17) */		"tr",
	/* langCroatian (18) */		"hr",
	/* langTradChinese (19) */	"zh",

	/* langUrdu (20) */			"ur",
	/* langHindi (21) */		"hi",
	/* langThai (22) */			"th",
	/* langKorean (23) */		"ko",
	/* langLithuanian (24) */	"lt",
	/* langPolish (25) */		"pl",
	/* langHungarian (26) */	"hu",
	/* langEstonian (27) */		"et",
	/* langLatvian (28) */		"lv",
	/* langSami (29) */			"se",

	/* langFaroese (30) */		"fo",
	/* langFarsi (31) */		"fa",
	/* langRussian (32) */		"ru",
	/* langSimpChinese (33) */	"zh",
	/* langFlemish (34) */		"nl",
	/* langIrishGaelic (35) */	"ga",
	/* langAlbanian (36) */		"sq",
	/* langRomanian (37) */		"ro",
	/* langCzech (38) */		"cs",
	/* langSlovak (39) */		"sk",

	/* langSlovenian (40) */	"sl",
	/* langYiddish (41) */		"yi",
	/* langSerbian (42) */		"sr",
	/* langMacedonian (43) */	"mk",
	/* langBulgarian (44) */	"bg",
	/* langUkrainian (45) */	"uk",
	/* langBelorussian (46) */	"be",
	/* langUzbek (47) */		"uz",
	/* langKazakh (48) */		"kk",
	/* langAzerbaijani (49) */	"az",

	/* langAzerbaijanAr (50) */	"az",
	/* langArmenian (51) */		"hy",
	/* langGeorgian (52) */		"ka",
	/* langMoldavian (53) */	"ro",
	/* langKirghiz (54) */		"ky",
	/* langTajiki (55) */		"tg",
	/* langTurkmen (56) */		"tk",
	/* langMongolian (57) */	"mn",
	/* langMongolianCyr (58) */	"mn",
	/* langPashto (59) */		"ps",

	/* langKurdish (60) */		"ku",
	/* langKashmiri (61) */		"ks",
	/* langSindhi (62) */		"sd",
	/* langTibetan (63) */		"bo",
	/* langNepali (64) */		"ne",
	/* langSanskrit (65) */		"sa",
	/* langMarathi (66) */		"mr",
	/* langBengali (67) */		"bn",
	/* langAssamese (68) */		"as",
	/* langGujarati (69) */		"gu",

	/* langPunjabi (70) */		"pa",
	/* langOriya (71) */		"or",
	/* langMalayalam (72) */	"ml",
	/* langKannada (73) */		"kn",
	/* langTamil (74) */		"ta",
	/* langTelugu (75) */		"te",
	/* langSinhalese (76) */	"si",
	/* langBurmese (77) */		"my",
	/* langKhmer (78) */		"km",
	/* langLao (79) */			"lo",

	/* langVietnamese (80) */	"vi",
	/* langIndonesian (81) */	"id",
	/* langTagalog (82) */		"tl",
	/* langMalayRoman (83) */	"ms",
	/* langMalayArabic (84) */	"ms",
	/* langAmharic (85) */		"am",
	/* langTigrinya (86) */		"ti",
	/* langOromo (87) */		"om",
	/* langSomali (88) */		"so",
	/* langSwahili (89) */		"sw",

	/* langKinyarwanda (90) */	"rw",
	/* langRundi (91) */		"rn",
	/* langNyanja (92) */		"ny",
	/* langMalagasy (93) */		"mg",
	/* langEsperanto (94) */	"eo"

};	// kMacToXMPLang_0_94

static const char * kMacToXMPLang_128_151 [24] = {

	/* langWelsh (128) */				"cy",
	/* langBasque (129) */				"eu",

	/* langCatalan (130) */				"ca",
	/* langLatin (131) */				"la",
	/* langQuechua (132) */				"qu",
	/* langGuarani (133) */				"gn",
	/* langAymara (134) */				"ay",
	/* langTatar (135) */				"tt",
	/* langUighur (136) */				"ug",
	/* langDzongkha (137) */			"dz",
	/* langJavaneseRom (138) */			"jv",
	/* langSundaneseRom (139) */		"su",

	/* langGalician (140) */			"gl",
	/* langAfrikaans (141) */			"af",
	/* langBreton (142) */				"br",
	/* langInuktitut (143) */			"iu",
	/* langScottishGaelic (144) */		"gd",
	/* langManxGaelic (145) */			"gv",
	/* langIrishGaelicScript (146) */	"ga",
	/* langTongan (147) */				"to",
	/* langGreekAncient (148) */		"",		// ! Has no ISO 639-1 2 letter code.
	/* langGreenlandic (149) */			"kl",

	/* langAzerbaijanRoman (150) */		"az",
	/* langNynorsk (151) */				"nn"
	
};	// kMacToXMPLang_128_151

// -------------------------------------------------------------------------------------------------

#if XMP_WinBuild

static UINT kMacScriptToWinCP[33] = {
	/* smRoman (0) */				10000,	// There don't seem to be symbolic constants.
	/* smJapanese (1) */			10001,	// From http://msdn.microsoft.com/en-us/library/dd317756(VS.85).aspx
	/* smTradChinese (2) */			10002,
	/* smKorean (3) */				10003,
	/* smArabic (4) */				10004,
	/* smHebrew (5) */				10005,
	/* smGreek (6) */				10006,
	/* smCyrillic (7) */			10007,
	/* smRSymbol (8) */				0,
	/* smDevanagari (9) */			0,
	/* smGurmukhi (10) */			0,
	/* smGujarati (11) */			0,
	/* smOriya (12) */				0,
	/* smBengali (13) */			0,
	/* smTamil (14) */				0,
	/* smTelugu (15) */				0,
	/* smKannada (16) */			0,
	/* smMalayalam (17) */			0,
	/* smSinhalese (18) */			0,
	/* smBurmese (19) */			0,
	/* smKhmer (20) */				0,
	/* smThai (21) */				10021,
	/* smLao (22) */				0,
	/* smGeorgian (23) */			0,
	/* smArmenian (24) */			0,
	/* smSimpChinese (25) */		10008,
	/* smTibetan (26) */			0,
	/* smMongolian (27) */			0,
	/* smEthiopic/smGeez (28) */	0,
	/* smCentralEuroRoman (29) */	10029,
	/* smVietnamese (30) */			0,
	/* smExtArabic (31) */			0,
	/* smUninterp (32) */			0
};	// kMacScriptToWinCP

static UINT kMacToWinCP_0_94 [95] = {

	/* langEnglish (0) */		0,
	/* langFrench (1) */		0,
	/* langGerman (2) */		0,
	/* langItalian (3) */		0,
	/* langDutch (4) */			0,
	/* langSwedish (5) */		0,
	/* langSpanish (6) */		0,
	/* langDanish (7) */		0,
	/* langPortuguese (8) */	0,
	/* langNorwegian (9) */		0,

	/* langHebrew (10) */		10005,
	/* langJapanese (11) */		10001,
	/* langArabic (12) */		10004,
	/* langFinnish (13) */		0,
	/* langGreek (14) */		10006,
	/* langIcelandic (15) */	10079,
	/* langMaltese (16) */		0,
	/* langTurkish (17) */		10081,
	/* langCroatian (18) */		10082,
	/* langTradChinese (19) */	10002,

	/* langUrdu (20) */			0,
	/* langHindi (21) */		0,
	/* langThai (22) */			10021,
	/* langKorean (23) */		10003,
	/* langLithuanian (24) */	0,
	/* langPolish (25) */		0,
	/* langHungarian (26) */	0,
	/* langEstonian (27) */		0,
	/* langLatvian (28) */		0,
	/* langSami (29) */			0,

	/* langFaroese (30) */		0,
	/* langFarsi (31) */		0,
	/* langRussian (32) */		0,
	/* langSimpChinese (33) */	10008,
	/* langFlemish (34) */		0,
	/* langIrishGaelic (35) */	0,
	/* langAlbanian (36) */		0,
	/* langRomanian (37) */		10010,
	/* langCzech (38) */		0,
	/* langSlovak (39) */		0,

	/* langSlovenian (40) */	0,
	/* langYiddish (41) */		0,
	/* langSerbian (42) */		0,
	/* langMacedonian (43) */	0,
	/* langBulgarian (44) */	0,
	/* langUkrainian (45) */	10017,
	/* langBelorussian (46) */	0,
	/* langUzbek (47) */		0,
	/* langKazakh (48) */		0,
	/* langAzerbaijani (49) */	0,

	/* langAzerbaijanAr (50) */	0,
	/* langArmenian (51) */		0,
	/* langGeorgian (52) */		0,
	/* langMoldavian (53) */	0,
	/* langKirghiz (54) */		0,
	/* langTajiki (55) */		0,
	/* langTurkmen (56) */		0,
	/* langMongolian (57) */	0,
	/* langMongolianCyr (58) */	0,
	/* langPashto (59) */		0,

	/* langKurdish (60) */		0,
	/* langKashmiri (61) */		0,
	/* langSindhi (62) */		0,
	/* langTibetan (63) */		0,
	/* langNepali (64) */		0,
	/* langSanskrit (65) */		0,
	/* langMarathi (66) */		0,
	/* langBengali (67) */		0,
	/* langAssamese (68) */		0,
	/* langGujarati (69) */		0,

	/* langPunjabi (70) */		0,
	/* langOriya (71) */		0,
	/* langMalayalam (72) */	0,
	/* langKannada (73) */		0,
	/* langTamil (74) */		0,
	/* langTelugu (75) */		0,
	/* langSinhalese (76) */	0,
	/* langBurmese (77) */		0,
	/* langKhmer (78) */		0,
	/* langLao (79) */			0,

	/* langVietnamese (80) */	0,
	/* langIndonesian (81) */	0,
	/* langTagalog (82) */		0,
	/* langMalayRoman (83) */	0,
	/* langMalayArabic (84) */	0,
	/* langAmharic (85) */		0,
	/* langTigrinya (86) */		0,
	/* langOromo (87) */		0,
	/* langSomali (88) */		0,
	/* langSwahili (89) */		0,

	/* langKinyarwanda (90) */	0,
	/* langRundi (91) */		0,
	/* langNyanja (92) */		0,
	/* langMalagasy (93) */		0,
	/* langEsperanto (94) */	0

};	// kMacToWinCP_0_94

#endif


#if XMP_iOSBuild

static XMP_Uns32 kMacScriptToIOSEncodingCF[33] = {
	/* smRoman (0) */				kCFStringEncodingMacRoman,
	/* smJapanese (1) */			kCFStringEncodingMacJapanese,
	/* smTradChinese (2) */			kCFStringEncodingMacChineseTrad,
	/* smKorean (3) */				kCFStringEncodingMacKorean,
	/* smArabic (4) */				kCFStringEncodingMacArabic,
	/* smHebrew (5) */				kCFStringEncodingMacHebrew,
	/* smGreek (6) */				kCFStringEncodingMacGreek,
	/* smCyrillic (7) */			kCFStringEncodingMacCyrillic,
	/* smRSymbol (8) */				kCFStringEncodingMacSymbol,
	/* smDevanagari (9) */			kCFStringEncodingMacDevanagari,
	/* smGurmukhi (10) */			kCFStringEncodingMacGurmukhi,
	/* smGujarati (11) */			kCFStringEncodingMacGujarati,
	/* smOriya (12) */				kCFStringEncodingMacOriya,
	/* smBengali (13) */			kCFStringEncodingMacBengali,
	/* smTamil (14) */				kCFStringEncodingMacTamil,
	/* smTelugu (15) */				kCFStringEncodingMacTelugu,
	/* smKannada (16) */			kCFStringEncodingMacKannada,
	/* smMalayalam (17) */			kCFStringEncodingMacMalayalam,
	/* smSinhalese (18) */			kCFStringEncodingMacSinhalese,
	/* smBurmese (19) */			kCFStringEncodingMacBurmese,
	/* smKhmer (20) */				kCFStringEncodingMacKhmer,
	/* smThai (21) */				kCFStringEncodingMacThai,
	/* smLao (22) */				kCFStringEncodingMacLaotian,
	/* smGeorgian (23) */			kCFStringEncodingMacGeorgian,
	/* smArmenian (24) */			kCFStringEncodingMacArmenian,
	/* smSimpChinese (25) */		kCFStringEncodingMacChineseSimp,
	/* smTibetan (26) */			kCFStringEncodingMacTibetan,
	/* smMongolian (27) */			kCFStringEncodingMacMongolian,
	/* smEthiopic/smGeez (28) */	kCFStringEncodingMacEthiopic,
	/* smCentralEuroRoman (29) */	kCFStringEncodingMacCentralEurRoman,
	/* smVietnamese (30) */			kCFStringEncodingMacVietnamese,
	/* smExtArabic (31) */			kCFStringEncodingMacExtArabic,
	/* smUninterp (32) */           kCFStringEncodingMacVT100
};	// kMacScriptToIOSEncodingCF

static XMP_Uns32 kMacToIOSEncodingCF_0_94 [95] = {
    
	/* langEnglish (0) */		kCFStringEncodingMacRoman,
	/* langFrench (1) */		kCFStringEncodingMacRoman,
	/* langGerman (2) */		kCFStringEncodingMacRoman,
	/* langItalian (3) */		kCFStringEncodingMacRoman,
	/* langDutch (4) */			kCFStringEncodingMacRoman,
	/* langSwedish (5) */		kCFStringEncodingMacRoman,
	/* langSpanish (6) */		kCFStringEncodingMacRoman,
	/* langDanish (7) */		kCFStringEncodingMacRoman,
	/* langPortuguese (8) */	kCFStringEncodingMacRoman,
	/* langNorwegian (9) */		kCFStringEncodingMacRoman,
    
	/* langHebrew (10) */		kCFStringEncodingMacHebrew,
	/* langJapanese (11) */		kCFStringEncodingMacJapanese,
	/* langArabic (12) */		kCFStringEncodingMacArabic,
	/* langFinnish (13) */		kCFStringEncodingMacRoman,
	/* langGreek (14) */		kCFStringEncodingMacGreek,
	/* langIcelandic (15) */	kCFStringEncodingMacIcelandic,
	/* langMaltese (16) */		kCFStringEncodingMacRoman,
	/* langTurkish (17) */		kCFStringEncodingMacTurkish,
	/* langCroatian (18) */		kCFStringEncodingMacCroatian,
	/* langTradChinese (19) */	kCFStringEncodingMacChineseTrad,
    
	/* langUrdu (20) */			kCFStringEncodingMacArabic,
	/* langHindi (21) */		kCFStringEncodingMacDevanagari,
	/* langThai (22) */			kCFStringEncodingMacThai,
	/* langKorean (23) */		kCFStringEncodingMacKorean,
	/* langLithuanian (24) */	kCFStringEncodingMacCentralEurRoman,
	/* langPolish (25) */		kCFStringEncodingMacCentralEurRoman,
	/* langHungarian (26) */	kCFStringEncodingMacCentralEurRoman,
	/* langEstonian (27) */		kCFStringEncodingMacCentralEurRoman,
	/* langLatvian (28) */		kCFStringEncodingMacCentralEurRoman,
	/* langSami (29) */			kCFStringEncodingInvalidId,
    
	/* langFaroese (30) */		kCFStringEncodingMacRoman,
	/* langFarsi (31) */		kCFStringEncodingMacFarsi,
	/* langRussian (32) */		kCFStringEncodingMacCyrillic,
	/* langSimpChinese (33) */	kCFStringEncodingMacChineseSimp,
	/* langFlemish (34) */		kCFStringEncodingMacRoman,
	/* langIrishGaelic (35) */	kCFStringEncodingMacRoman,
	/* langAlbanian (36) */		kCFStringEncodingMacRoman,
	/* langRomanian (37) */		kCFStringEncodingMacRomanian,
	/* langCzech (38) */		kCFStringEncodingMacCentralEurRoman,
	/* langSlovak (39) */		kCFStringEncodingMacCentralEurRoman,
    
	/* langSlovenian (40) */	kCFStringEncodingMacRoman,
	/* langYiddish (41) */		kCFStringEncodingMacHebrew,
	/* langSerbian (42) */		kCFStringEncodingMacCyrillic,
	/* langMacedonian (43) */	kCFStringEncodingMacCyrillic,
	/* langBulgarian (44) */	kCFStringEncodingMacCyrillic,
	/* langUkrainian (45) */	kCFStringEncodingMacUkrainian,
	/* langBelorussian (46) */	kCFStringEncodingMacCyrillic,
	/* langUzbek (47) */		kCFStringEncodingMacCyrillic,
	/* langKazakh (48) */		kCFStringEncodingMacCyrillic,
	/* langAzerbaijani (49) */	kCFStringEncodingMacCyrillic,
    
	/* langAzerbaijanAr (50) */	kCFStringEncodingMacArabic,
	/* langArmenian (51) */		kCFStringEncodingMacArmenian,
	/* langGeorgian (52) */		kCFStringEncodingMacGeorgian,
	/* langMoldavian (53) */	kCFStringEncodingMacCyrillic,
	/* langKirghiz (54) */		kCFStringEncodingMacCyrillic,
	/* langTajiki (55) */		kCFStringEncodingMacCyrillic,
	/* langTurkmen (56) */		kCFStringEncodingMacCyrillic,
	/* langMongolian (57) */	kCFStringEncodingMacMongolian,
	/* langMongolianCyr (58) */	kCFStringEncodingMacCyrillic,
	/* langPashto (59) */		kCFStringEncodingMacArabic,
    
	/* langKurdish (60) */		kCFStringEncodingMacArabic,
	/* langKashmiri (61) */		kCFStringEncodingMacArabic,
	/* langSindhi (62) */		kCFStringEncodingMacArabic,
	/* langTibetan (63) */		kCFStringEncodingMacTibetan,
	/* langNepali (64) */		kCFStringEncodingMacDevanagari,
	/* langSanskrit (65) */		kCFStringEncodingMacDevanagari,
	/* langMarathi (66) */		kCFStringEncodingMacDevanagari,
	/* langBengali (67) */		kCFStringEncodingMacBengali,
	/* langAssamese (68) */		kCFStringEncodingMacBengali,
	/* langGujarati (69) */		kCFStringEncodingMacGujarati,
    
	/* langPunjabi (70) */		kCFStringEncodingMacGurmukhi,
	/* langOriya (71) */		kCFStringEncodingMacOriya,
	/* langMalayalam (72) */	kCFStringEncodingMacMalayalam,
	/* langKannada (73) */		kCFStringEncodingMacKannada,
	/* langTamil (74) */		kCFStringEncodingMacTamil,
	/* langTelugu (75) */		kCFStringEncodingMacTelugu,
	/* langSinhalese (76) */	kCFStringEncodingMacSinhalese,
	/* langBurmese (77) */		kCFStringEncodingMacBurmese,
	/* langKhmer (78) */		kCFStringEncodingMacKhmer,
	/* langLao (79) */			kCFStringEncodingMacLaotian,
    
	/* langVietnamese (80) */	kCFStringEncodingMacVietnamese,
	/* langIndonesian (81) */	kCFStringEncodingMacRoman,
	/* langTagalog (82) */		kCFStringEncodingMacRoman,
	/* langMalayRoman (83) */	kCFStringEncodingMacRoman,
	/* langMalayArabic (84) */	kCFStringEncodingMacArabic,
	/* langAmharic (85) */		kCFStringEncodingMacEthiopic,
	/* langTigrinya (86) */		kCFStringEncodingMacEthiopic,
	/* langOromo (87) */		kCFStringEncodingMacEthiopic,
	/* langSomali (88) */		kCFStringEncodingMacRoman,
	/* langSwahili (89) */		kCFStringEncodingMacRoman,
    
	/* langKinyarwanda (90) */	kCFStringEncodingMacRoman,
	/* langRundi (91) */		kCFStringEncodingMacRoman,
	/* langNyanja (92) */		kCFStringEncodingMacRoman,
	/* langMalagasy (93) */		kCFStringEncodingMacRoman,
	/* langEsperanto (94) */	kCFStringEncodingMacRoman
    
};	// kMacToIOSEncodingCF_0_94

#endif

// =================================================================================================
// GetMacScript
// ============

static XMP_Uns16 GetMacScript ( XMP_Uns16 macLang )
{
	XMP_Uns16 macScript = kNoMacScript;
	
	if ( macLang <= 94 ) {
		macScript = kMacLangToScript_0_94[macLang];
	} else if ( (128 <= macLang) && (macLang <= 151) ) {
		macScript = kMacLangToScript_128_151[macLang-128];
	}
	
	return macScript;
	
}	// GetMacScript


#if XMP_iOSBuild
// =================================================================================================
// GetIOSEncodingCF
// ========

static XMP_Uns32 GetIOSEncodingCF ( XMP_Uns16 macLang )
{
	XMP_Uns32 encCF = kCFStringEncodingInvalidId;
	
	if ( macLang <= 94 ) encCF = kMacToIOSEncodingCF_0_94[macLang];
	
	if ( encCF == kCFStringEncodingInvalidId || !CFStringIsEncodingAvailable(encCF)) {
		XMP_Uns16 macScript = GetMacScript ( macLang );
		if ( macScript != kNoMacScript ) encCF = kMacScriptToIOSEncodingCF[macScript];
	}
	
	return encCF;
	
}	// GetIOSEncodingCF
#endif

// =================================================================================================
// GetWinCP
// ========

#if XMP_WinBuild

static UINT GetWinCP ( XMP_Uns16 macLang )
{
	UINT winCP = 0;
	
	if ( macLang <= 94 ) winCP = kMacToWinCP_0_94[macLang];
	
	if ( winCP == 0 ) {
		XMP_Uns16 macScript = GetMacScript ( macLang );
		if ( macScript != kNoMacScript ) winCP = kMacScriptToWinCP[macScript];
	}
	
	return winCP;
	
}	// GetWinCP

#endif

// =================================================================================================
// GetXMPLang
// ==========

static XMP_StringPtr GetXMPLang ( XMP_Uns16 macLang )
{
	XMP_StringPtr xmpLang = "";
	
	if ( macLang <= 94 ) {
		xmpLang = kMacToXMPLang_0_94[macLang];
	} else if ( (128 <= macLang) && (macLang <= 151) ) {
		xmpLang = kMacToXMPLang_128_151[macLang-128];
	}
	
	return xmpLang;
	
}	// GetXMPLang

// =================================================================================================
// GetMacLang
// ==========

static XMP_Uns16 GetMacLang ( std::string * xmpLang )
{
	if ( *xmpLang == "" ) return kNoMacLang;
	
	size_t hyphenPos = xmpLang->find ( '-' );	// Make sure the XMP language is "generic".
	if ( hyphenPos != std::string::npos ) xmpLang->erase ( hyphenPos );
	
	for ( XMP_Uns16 i = 0; i <= 94; ++i ) {	// Using std::map would be faster.
		if ( *xmpLang == kMacToXMPLang_0_94[i] ) return i;
	}
	
	for ( XMP_Uns16 i = 128; i <= 151; ++i ) {	// Using std::map would be faster.
		if ( *xmpLang == kMacToXMPLang_128_151[i-128] ) return i;
	}
	
	return kNoMacLang;
	
}	// GetMacLang

// =================================================================================================
// MacRomanToUTF8
// ==============

static void MacRomanToUTF8 ( const std::string & macRoman, std::string * utf8 )
{
	utf8->erase();
	
	for ( XMP_Uns8* chPtr = (XMP_Uns8*)macRoman.c_str(); *chPtr != 0; ++chPtr ) {	// ! Don't trust that char is unsigned.
		if ( *chPtr < 0x80 ) {
			(*utf8) += (char)*chPtr;
		} else {
			(*utf8) += kMacRomanUTF8[(*chPtr)-0x80];
		}
	}

}	// MacRomanToUTF8

// =================================================================================================
// UTF8ToMacRoman
// ==============

static void UTF8ToMacRoman ( const std::string & utf8, std::string * macRoman )
{
	macRoman->erase();
	bool inNonMRSpan = false;
	
	for ( const XMP_Uns8 * chPtr = (XMP_Uns8*)utf8.c_str(); *chPtr != 0; ++chPtr ) {	// ! Don't trust that char is unsigned.
		if ( *chPtr < 0x80 ) {
			(*macRoman) += (char)*chPtr;
			inNonMRSpan = false;
		} else {
			XMP_Uns32 cp = GetCodePoint ( &chPtr );
			--chPtr;	// Make room for the loop increment.
			XMP_Uns8  mr;
			for ( mr = 0; (mr < 0x80) && (cp != kMacRomanCP[mr]); ++mr ) {};	// Using std::map would be faster.
			if ( mr < 0x80 ) {
				(*macRoman) += (char)(mr+0x80);
				inNonMRSpan = false;
			} else if ( ! inNonMRSpan ) {
				(*macRoman) += '?';
				inNonMRSpan = true;
			}
		}
	}

}	// UTF8ToMacRoman

// =================================================================================================
// IsMacLangKnown
// ==============

static inline bool IsMacLangKnown ( XMP_Uns16 macLang )
{
	XMP_Uns16 macScript = GetMacScript ( macLang );
	if ( macScript == kNoMacScript ) return false;

	#if XMP_UNIXBuild | XMP_AndroidBuild
		if ( macScript != smRoman ) return false;
	#elif XMP_WinBuild
		if ( GetWinCP(macLang) == 0 ) return false;
	#endif
	
	return true;

}	// IsMacLangKnown

// =================================================================================================
// ConvertToMacLang
// ================

bool ConvertToMacLang ( const std::string & utf8Value, XMP_Uns16 macLang, std::string * macValue )
{
	macValue->erase();
	if ( macLang == kNoMacLang ) macLang = 0;	// *** Zero is English, ought to use the "active" OS lang.
	if ( ! IsMacLangKnown ( macLang ) ) return false;

	#if XMP_MacBuild
		XMP_Uns16 macScript = GetMacScript ( macLang );
		ReconcileUtils::UTF8ToMacEncoding ( macScript, macLang, (XMP_Uns8*)utf8Value.c_str(), utf8Value.size(), macValue );
	#elif XMP_UNIXBuild | XMP_AndroidBuild
		UTF8ToMacRoman ( utf8Value, macValue );
	#elif XMP_WinBuild
		UINT winCP = GetWinCP ( macLang );
		ReconcileUtils::UTF8ToWinEncoding ( winCP, (XMP_Uns8*)utf8Value.c_str(), utf8Value.size(), macValue );
    #elif XMP_iOSBuild
        XMP_Uns32 iosEncCF = GetIOSEncodingCF(macLang);
        ReconcileUtils::IOSConvertEncoding(kCFStringEncodingUTF8, iosEncCF, (XMP_Uns8*)utf8Value.c_str(), utf8Value.size(), macValue);
    #endif
	
	return true;

}	// ConvertToMacLang

// =================================================================================================
// ConvertFromMacLang
// ==================

bool ConvertFromMacLang ( const std::string & macValue, XMP_Uns16 macLang, std::string * utf8Value )
{
	utf8Value->erase();
	if ( ! IsMacLangKnown ( macLang ) ) return false;
	
	#if XMP_MacBuild
		XMP_Uns16 macScript = GetMacScript ( macLang );
		ReconcileUtils::MacEncodingToUTF8 ( macScript, macLang, (XMP_Uns8*)macValue.c_str(), macValue.size(), utf8Value );
	#elif XMP_UNIXBuild | XMP_AndroidBuild
		MacRomanToUTF8 ( macValue, utf8Value );
	#elif XMP_WinBuild
		UINT winCP = GetWinCP ( macLang );
		ReconcileUtils::WinEncodingToUTF8 ( winCP, (XMP_Uns8*)macValue.c_str(), macValue.size(), utf8Value );
    #elif XMP_iOSBuild
        XMP_Uns32 iosEncCF = GetIOSEncodingCF(macLang);
        ReconcileUtils::IOSConvertEncoding(iosEncCF, kCFStringEncodingUTF8, (XMP_Uns8*)macValue.c_str(), macValue.size(), utf8Value);
#endif
	
	return true;

}	// ConvertFromMacLang

#if AdobePrivate
#if EnableITunesMetadata
// =================================================================================================
// IsTextType
// ==========
	
static bool IsTextType ( XMP_Uns16 typeCode ) {

	switch ( typeCode ) {
		case kiTunesData_UTF_8    :
		case kiTunesData_UTF_16   :
		case kiTunesData_ShiftJIS :
		case kiTunesData_HTML     :
		case kiTunesData_XML      :
		case kiTunesData_ISRC     :
		case kiTunesData_MI3P     :
		case kiTunesData_URL      :
		case kiTunesData_RIAA_PA  :
		case kiTunesData_UPC      : return true;
	}
	
	return false;

}
#endif
#endif

// =================================================================================================
// =================================================================================================
// TradQT_Manager
// =================================================================================================
// =================================================================================================

// =================================================================================================
// TradQT_Manager::ParseCachedBoxes
// ================================
//
// Parse the cached '�...' children of the 'moov'/'udta' box. The contents of each cached box are
// a sequence of "mini boxes" analogous to XMP AltText arrays. Each mini box has a 16-bit size,
// 16-bit language code, and text. The size is only the text size. The language codes are Macintosh
// Script Manager langXyz codes. The text encoding is implicit in the language, see comments in
// Apple's Script.h header.

bool TradQT_Manager::ParseCachedBoxes ( const MOOV_Manager & moovMgr )
{
	MOOV_Manager::BoxInfo udtaInfo;
	MOOV_Manager::BoxRef  udtaRef = moovMgr.GetBox ( "moov/udta", &udtaInfo );
	if ( udtaRef == 0 ) return false;

	for ( XMP_Uns32 i = 0; i < udtaInfo.childCount; ++i ) {

		MOOV_Manager::BoxInfo currInfo;
		MOOV_Manager::BoxRef  currRef = moovMgr.GetNthChild ( udtaRef, i, &currInfo );
		if ( currRef == 0 ) break;	// Sanity check, should not happen.
		if ( (currInfo.boxType >> 24) != 0xA9 ) continue;
		if ( currInfo.contentSize < 2+2+1 ) continue;	// Want enough for a non-empty value.
		
		InfoMapPos newInfo = this->parsedBoxes.insert ( this->parsedBoxes.end(),
														InfoMap::value_type ( currInfo.boxType, ParsedBoxInfo ( currInfo.boxType ) ) );
		std::vector<ValueInfo> * newValues = &newInfo->second.values;
		
		XMP_Uns8 * boxPtr = (XMP_Uns8*) currInfo.content;
		XMP_Uns8 * boxEnd = boxPtr + currInfo.contentSize;
		XMP_Uns16 miniLen, macLang;

		for ( ; boxPtr < boxEnd-4; boxPtr += miniLen ) {

			miniLen = 4 + GetUns16BE ( boxPtr );	// ! Include header in local miniLen.
			macLang  = GetUns16BE ( boxPtr+2);
			if ( (miniLen <= 4) || (miniLen > (boxEnd - boxPtr)) ) continue;	// Ignore bad or empty values.
			
			XMP_StringPtr valuePtr = (char*)(boxPtr+4);
			size_t valueLen = miniLen - 4;

			newValues->push_back ( ValueInfo() );
			ValueInfo * newValue = &newValues->back();
			
			// Only set the XMP language if the Mac script is known, i.e. the value can be converted.
			
			newValue->macLang = macLang;
			if ( IsMacLangKnown ( macLang ) ) newValue->xmpLang = GetXMPLang ( macLang );
			newValue->macValue.assign ( valuePtr, valueLen );

		}

	}
	
	return (! this->parsedBoxes.empty());

}	// TradQT_Manager::ParseCachedBoxes

/*Parsed the metadata present in moov/meta atom. The moov/meta box contains 3 mandatory child atoms namely 
   - metadata handler atom 'hdlr*, 
   - metadata item keys atom 'keys'
   - metadata item list atom 'ilst'
  hdlr atom contains information about the structure followed in meta atom, 
  If the handler type is not mdta, meta atom does not follow specs according to QT and function will not parse moov/meta atom further
  
  'keys' atom  holds a list of the metadata keys that may be present in the meta atom. These keys are indexed starting from 1
  
  'ilst' atom  holds a list of actual metadata values that are present in the metadata atom. The metadata items are formatted as a list of items.
  'ilst' atom contains child atoms whose atom type should be set equal to the index of the key from the metadata item keys atom. In addition, 
   each child atom contains a Value Atom, to hold the value of the metadata item.. 
*/
#if AdobePrivate
bool TradQT_Manager::ParseMetaBox(const MOOV_Manager & moovMgr) {

	MOOV_Manager::BoxInfo moovMetaInfo;
	MOOV_Manager::BoxRef  moovMetaRef = moovMgr.GetBox("moov/meta", &moovMetaInfo);
	
	if (moovMetaRef == 0) return false;

	//Check that 1st atom is Metadata handler atom and handler type is 'mdta'

	MOOV_Manager::BoxInfo currInfo;
	MOOV_Manager::BoxRef  currRef = moovMgr.GetNthChild(moovMetaRef, 0, &currInfo); //First child should be of box type 'hdlr'
	if (currRef == 0 || currInfo.boxType != ISOMedia::k_hdlr || currInfo.contentSize < 24)
		return false;
	XMP_Uns32 u32type = GetUns32BE(currInfo.content + 8);
	if (u32type != 0x6D647461UL) //if hnadler type is not "mdta", return false;
		return false;
	

	for (XMP_Uns32 i = 1; i < moovMetaInfo.childCount; ++i) {
		currRef = moovMgr.GetNthChild(moovMetaRef, i, &currInfo);
		if (currRef == 0) break;	// Sanity check, should not happen.
		
		if (currInfo.boxType == ISOMedia::k_keys) {

			if (currInfo.contentSize <= 4)
				break;

			
			XMP_Int32 entryCount = GetUns32BE(currInfo.content + 4);
			if (entryCount == 0)
				break;
			const XMP_Uns8 *keyStart = currInfo.content + 8                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                         ;
			const XMP_Uns8 *keyEnd = currInfo.content + currInfo.contentSize;
			MetaBoxItemList.reserve(entryCount);

			for (int index = 0;index < entryCount && keyStart < keyEnd;++index) {
				XMP_Int32 keySize = GetUns32BE(keyStart);
				if (keySize <= 8 || ((keyStart + keySize) > keyEnd - (entryCount - index - 1) * 8))
				{
					if (MetaBoxItemList.empty()) //Not a single key is processed, so no need to process meta box further
						return false;
					break; //try to get value of keys which are processed till now.
				}
				MetaBoxItemList.push_back(TradQT_Manager::MetaItemBoxInfo());
				TradQT_Manager::MetaItemBoxInfo& info = MetaBoxItemList.back();
				info.key.append((const char*)keyStart + 8, keySize - 8);
				keyStart += keySize;
			}

		}
		else if (currInfo.boxType == ISOMedia::k_ilst) {
			for (XMP_Uns32 j = 0; j < currInfo.childCount; ++j) {
				MOOV_Manager::BoxInfo metaItem;
				MOOV_Manager::BoxRef metaItemRef = moovMgr.GetNthChild(currRef, j, &metaItem);

				if (metaItemRef != 0) {
					XMP_Int32 keyIndex = metaItem.boxType;
					if (keyIndex > (XMP_Int32)MetaBoxItemList.size() || keyIndex <= 0) {
						continue;//check
					}
					TradQT_Manager::MetaItemBoxInfo& info = MetaBoxItemList[keyIndex - 1];
					for (XMP_Uns32 k = 0; k < metaItem.childCount; ++k) {
						MOOV_Manager::BoxInfo dataBoxInfo;
						MOOV_Manager::BoxRef dataBoxRef = moovMgr.GetNthChild(metaItemRef, k, &dataBoxInfo);
						if (dataBoxInfo.boxType == ISOMedia::k_data) {
                        #if  XMP_ANDROID_ARM
							XMP_Uns8 *start = new XMP_Uns8[dataBoxInfo.contentSize];
							memcpy(start,dataBoxInfo.content,dataBoxInfo.contentSize);
                        #else
                            const XMP_Uns8 *start = dataBoxInfo.content;
                        #endif
							const XMP_Uns8 *end = dataBoxInfo.content + dataBoxInfo.contentSize;

							

							XMP_Uns32 dataType = GetUns32BE(start);
							XMP_Uns32 localeInfo = GetUns32BE(start + 4);
							info.values.push_back(DataBoxInfo (dataType, localeInfo));
							info.values.back().value.assign((char*)(start + 8), (size_t)(dataBoxInfo.contentSize - 8));
							if(isValidType(dataType) && (info.firstText == std::string::npos)) info.firstText = info.values.size() - 1;
                        #if  XMP_ANDROID_ARM
							delete[] start;
                        #endif
						}
					}
				}

			}
		}
	}
	return true;
}

bool TradQT_Manager::DecodeString(const std::string & value, XMP_Uns16 typeCode, std::string * utf8Value)
{
	XMP_Assert(isValidType(typeCode));

	if (typeCode == kData_UTF_16) {
		FromUTF16((UTF16Unit*)value.data(), value.size() / 2, utf8Value, true /* big endian */);
	}
	else if (typeCode != kData_ShiftJIS) {
		*utf8Value = value;	// The value is already UTF-8.
	}
	else {
#if XMP_MacBuild
		ReconcileUtils::MacEncodingToUTF8(smJapanese, langJapanese, (XMP_Uns8*)value.c_str(), value.size(), utf8Value);
#elif XMP_WinBuild
		ReconcileUtils::WinEncodingToUTF8(932, (XMP_Uns8*)value.c_str(), value.size(), utf8Value);
#elif XMP_iOSBuild
		ReconcileUtils::IOSConvertEncoding(kCFStringEncodingMacJapanese, kCFStringEncodingUTF8, (XMP_Uns8*)value.c_str(), value.size(), utf8Value);
#else
		return false;
#endif
	}
	return true;
}

bool TradQT_Manager::ImportLocationMetaAtom( SXMPMeta * xmp) {
	std::string propValue;
	XMP_Uns32 len;
	bool found = FindMetaAtom("com.apple.quicktime.location.ISO6709", propValue, len);
	bool altPresent=true;
	len=(XMP_Uns32)propValue.length();
	
	if (found) {
		return ConvertGPSToXMPFormat(propValue.c_str(), xmp, len, true);
	}
    return found;

}

bool TradQT_Manager::FindMetaAtom(const std::string& id,std::string& propValue,XMP_Uns32 & index) {
	if (MetaBoxItemList.empty())
		return false;
	std::vector<MetaItemBoxInfo>::iterator itr = MetaBoxItemList.begin();
	index = 0;
	while (itr != MetaBoxItemList.end()) {

		if (itr->key == id)
		{
			DecodeString(itr->values[itr->firstText].value, ((XMP_Uns16)itr->values[itr->firstText].dataType), &propValue);
			return true;
		}
		++itr; index++;
	}
	return false;
}

bool TradQT_Manager::ImportCreateDateMetaAtom(SXMPMeta * xmp) {
	if (MetaBoxItemList.empty())
		return false;
	std::vector<MetaItemBoxInfo>::iterator itr = MetaBoxItemList.begin();
	XMP_DateTime dateTime;
	std::string data;
	XMP_Uns32 index;
	bool found = FindMetaAtom("com.apple.quicktime.creationdate", data, index);
	if (found) {
		bool ret = ConvertQTDateTime(data.c_str(), &dateTime);
		if (ret) xmp->SetProperty_Date(kXMP_NS_XMP, "CreateDate", dateTime);
		return ret;
	}
	return false;
}

bool TradQT_Manager::ImportModelMetaAtom(SXMPMeta * xmp) {
	if (MetaBoxItemList.empty())
		return false;
	std::vector<MetaItemBoxInfo>::iterator itr = MetaBoxItemList.begin();
	std::string data;
	XMP_Uns32 index;
	bool found = FindMetaAtom("com.apple.quicktime.model", data, index);
	if(found)
		xmp->SetProperty(kXMP_NS_DM, "cameraModel", data);
	return found;
}

#if 0
bool TradQT_Manager::ExportLocationMetaAtom(const SXMPMeta& xmp, MOOV_Manager* moovMgr) {
	if (MetaBoxItemList.empty()) return false;

	std::string propValue,crs,location;
	XMP_Uns32 index;
	bool foundInAtom = FindMetaAtom("com.apple.quicktime.location.ISO6709", propValue,index);

	/*Check for CRS system 
	**only CRSWGS_84 supported for now. If none is present we consider it to be default
	*/
	if (strstr(propValue.c_str(), "CRS"))
	{
		if (strstr(propValue.c_str(), "CRSWGS-84/"))
			crs = "CRSWGS-84/";
		else
			return false;
	}
	else
		crs = "/";

	//For now providing only updating Location functionality and not adding it
	if (foundInAtom) {
		std::string latitude, longitude, altitude, altitudeRef;
		bool foundInXMP = true,altPresent=true,success;
		foundInXMP = xmp.GetProperty(kXMP_NS_EXIF, "GPSLatitude", &latitude, NULL);
		foundInXMP &= xmp.GetProperty(kXMP_NS_EXIF, "GPSLongitude", &longitude, NULL);
		altPresent = xmp.GetProperty(kXMP_NS_EXIF, "GPSAltitude", &altitude, NULL);
		altPresent &= xmp.GetProperty(kXMP_NS_EXIF, "GPSAltitudeRef", &altitudeRef, NULL);
		if (foundInXMP) {
			success = FormatGPSPropertyToLocation(latitude, "GPSLatitude");
			success &= FormatGPSPropertyToLocation(longitude, "GPSLongitude");
			if (success) {
				if (altPresent) {
					char charAlt;
					std::string ref;
					double alt;
					if (sscanf(altitude.c_str(), "%lf%c", &alt, &ref) == 1) {
						if (altitudeRef == "0")
							ref = '+';
						else {
							if (altitudeRef == "1")
								ref = '-';
							else
								return false;
						}

						altitude = ref + altitude;
						location = latitude + longitude + altitude +crs;
					}
				}
				else {
					location = latitude + longitude + crs;
				}
				XMP_Uns8* c= (XMP_Uns8*)malloc(location.size());
				
				memcpy((void*)c, location.c_str(), location.size());
				MetaBoxItemList[index].data = c; //(XMP_Uns8*)location.c_str();
				//memcpy((void*)MetaBoxItemList[index].data, location.c_str(), location.size());
				MetaBoxItemList[index].changed = true;
				return true;
			}
			
		}
	}
	return false;
}

bool TradQT_Manager::ExportCreateDateMetaAtom(const SXMPMeta& xmp, MOOV_Manager* moovMgr) {

	MOOV_Manager::BoxInfo moovMetaInfo;
	MOOV_Manager::BoxRef  moovMetaRef = moovMgr->GetBox("moov/meta", &moovMetaInfo);

	if (MetaBoxItemList.empty()) return false;
	
	std::string xmpValue;
	bool found = xmp.GetProperty(kXMP_NS_XMP, "CreateDate", &xmpValue, NULL);
	if (!found)
		return false;
	std::vector<MetaItemBoxInfo>::iterator itr = MetaBoxItemList.begin();
	XMP_Int32 index = 0;
	while (itr != MetaBoxItemList.end()) {
		if (itr->key == "com.apple.quicktime.creationdate") break;
		++index;++itr;
	}

	if (ConvertXMPDateTime(xmpValue) == false) return false;

	if (index == MetaBoxItemList.size() || MetaBoxItemList[index].dataSize != xmpValue.size()) return false;
	memcpy((void*)MetaBoxItemList[index].data, xmpValue.c_str(), xmpValue.size());
	return true;
}

#endif
//2016-12-05T17:36:17+0530 ==>2016-12-05T17:36:17+05:30
bool TradQT_Manager::ConvertQTDateTime(XMP_StringPtr strValue, XMP_DateTime* binValue) {

	if ((strValue == 0) || (*strValue == 0))
		return false;

	size_t strSize = strlen(strValue);
	if (strSize <= 5) //No Time Zone Info , stick to UTC time from moov atom
		return false;

	char sign = strValue[strSize - 5]; //take the timezone sign from the last

	if (sign != '+' && sign != '-')//No Time Zone Info , stick to UTC time from moov atom
		return false;

	std::string str(strValue, 0, strSize - 5);
	str.append("Z");
	std::string tzStr(strValue, strSize - 4, strSize);
	try {
		SXMPUtils::ConvertToDate(str, binValue);
	}
	catch (XMP_Error) {
		
		return false;
	}

	XMP_Assert(binValue);


	binValue->hasTimeZone = true;
	binValue->tzSign = (sign == '+') ? 1 : -1;
	binValue->tzHour = (tzStr[0]-48) * 10 + (tzStr[1]-48);
	binValue->tzMinute = (tzStr[2]-48) * 10 + (tzStr[3]-48);

	return true;
}

//2016-12-05T17:36:17+05:30 ==> 2016-12-05T17:36:17+0530
bool TradQT_Manager::ConvertXMPDateTime(std::string& strValue) {

	XMP_DateTime binValue;
	try {
		SXMPUtils::ConvertToDate(strValue, &binValue);
	}
	catch (XMP_Error) {
		return false;
	}

	if (binValue.hasTimeZone == false) return false;

	if (strValue[strValue.size() - 3] != ':') return false;

	std::string temp = strValue.substr(strValue.size() - 2);
	strValue.replace(strValue.size() - 3, 3, temp);
	return true;
}

bool  TradQT_Manager::ConvertGPSToXMPFormat(const char* strValue,SXMPMeta * xmp, XMP_Uns32 len, bool priority) {
	
	bool altPresent = true;

	if ((strValue == 0) || (*strValue == 0) || (*(strValue + len - 1) != '/'))
		return false;
	if ((*strValue != '+') && (*strValue != '-'))
		return false;
	std::string latitude, longitude, altRef;
	len = 0;
	char next[12];
	bool success = FormatLocationToGPSProperty(strValue, "GPSLatitude", latitude, len);
	if ((*strValue != '+') && (*strValue != '-'))
		return false;
	strValue = strValue + len;
	success &= FormatLocationToGPSProperty(strValue, "GPSLongitude", longitude, len);
	strValue = strValue + len;

	if ((*strValue != '+') && (*strValue != '-')) {
		altPresent = false;
		if (*strValue == '/' || strstr(strValue, "CRSWGS-84/")) //Not supporting any CRS other than WGS_84. If none is present we consider it to be default
			success &= true;
		else
			success &= false;
	}
	char altitude[120];
	if (altPresent) {
		//0 --> At or above sea level (for default CRS)
		//1 --> below sea level
		altRef = (*strValue) == '+' ? "0" : "1";
		XMP_Uns32 num = 0, denom = 0;
		strValue++;
		while (('0' <= *strValue) && (*strValue <= '9')) {
			num = num * 10 + (*strValue - '0');
			++strValue;
			denom *= 10;
			if ((*strValue) == '.')
			{
				denom = 1;
				++strValue;
			}
		}
		if (denom == 1) success &= false;
		if (denom == 0) denom = 1;
		////////////////////////////////////////////////////////////////
		// ACR: replaced sprintf with sprintf_safe
		#if 0
		sprintf(altitude, "%u/%u", num, denom);
		#else
		sprintf_safe(altitude, sizeof(altitude), "%u/%u", num, denom);
		#endif
		////////////////////////////////////////////////////////////////

		if (sscanf(strValue, "%s", next) == 1) {
			if (strcmp(next, "CRSWGS-84/") == 0 || strcmp(next, "/") == 0)
				success &= true;
			else
				success &= false;
		}
		else
			success &= false;
	}

	if (success)
	{
		if (priority) {
			xmp->DeleteProperty(kXMP_NS_EXIF, "GPSLatitude");
			xmp->DeleteProperty(kXMP_NS_EXIF, "GPSLongitude");
			xmp->DeleteProperty(kXMP_NS_EXIF, "GPSAltitude");
			xmp->DeleteProperty(kXMP_NS_EXIF, "GPSAltitudeRef");
			xmp->SetProperty(kXMP_NS_EXIF, "GPSLatitude", latitude.c_str());
			xmp->SetProperty(kXMP_NS_EXIF, "GPSLongitude", longitude.c_str());
			if (altPresent) {
				xmp->SetProperty(kXMP_NS_EXIF, "GPSAltitudeRef", altRef);
				xmp->SetProperty(kXMP_NS_EXIF, "GPSAltitude", altitude);
			}
		}
		else {
			if (xmp->DoesPropertyExist(kXMP_NS_EXIF, "GPSLatitude") || xmp->DoesPropertyExist(kXMP_NS_EXIF, "GPSLongitude") || xmp->DoesPropertyExist(kXMP_NS_EXIF, "GPSAltitude")) {
				return false;
			}
			else {
				xmp->SetProperty(kXMP_NS_EXIF, "GPSLatitude", latitude.c_str());
				xmp->SetProperty(kXMP_NS_EXIF, "GPSLongitude", longitude.c_str());
				if (altPresent) {
					xmp->SetProperty(kXMP_NS_EXIF, "GPSAltitudeRef", altRef);
					xmp->SetProperty(kXMP_NS_EXIF, "GPSAltitude", altitude);
				}
			}
		}
	
		return true;
	}
	return false;
	
}
//According to ISO Spec 6709:2008 Anex H
bool TradQT_Manager::FormatLocationToGPSProperty(XMP_StringPtr ptrToStr, XMP_StringPtr property,std::string & value, XMP_Uns32 &len)
{
	XMP_Index degLength = 0;
	len = 0;
	char ref;
	XMP_Int32 decimalPosition = 0;
    if (strncmp(property,"GPSLongitude",strlen("GPSLongitude")) == 0) {
		decimalPosition--;
		ref = (*ptrToStr) == '+' ? 'E' : 'W';
	}
	else{
		ref = (*ptrToStr) == '+' ? 'N' : 'S';
	}
	
	++ptrToStr;
	len++;

	XMP_Uns32 deg = 0, min = 0, sec = 0, integralPart = 0;
	XMP_Uns32 decimalPart = 0, denom = 1;
	double f = 0;
	
	for (; ('0' <= *ptrToStr) && (*ptrToStr <= '9'); ++ptrToStr) {
		if (decimalPosition == 2) break;
		deg = deg * 10 + (*ptrToStr - '0');
		decimalPosition++; len++;
	}
	for (; ('0' <= *ptrToStr) && (*ptrToStr <= '9'); ++ptrToStr) {
		integralPart = integralPart * 10 + (*ptrToStr - '0');
		decimalPosition++; len++;
	}
	if ((*ptrToStr) == '.') {
		++ptrToStr; len++;
		for (; ('0' <= *ptrToStr) && (*ptrToStr <= '9'); ++ptrToStr) {
			decimalPart = decimalPart * 10 + (*ptrToStr - '0');
			denom *= 10;
			len++;
		}
		f = decimalPart / (double)denom;
	}
	else
		decimalPosition = 0;
	double fracMin = 0;
	switch (decimalPosition) {
		case 0: if (decimalPosition - 4 > 0) { min = integralPart / 100; sec = integralPart % 100;  }
				else if (decimalPosition - 2 > 0) min = integralPart;
				break;
		case 2: fracMin = f * 60; 
				break;
		case 4: fracMin = integralPart + f; 
				break;
		case 6: min = integralPart / 100; sec = integralPart % 100; fracMin = min + (sec + f) / 60.0; 
				break;
		default: return false;
	}
	char xmpValue[120];
	if (decimalPosition > 0)
		{
		////////////////////////////////////////////////////////////////
		// ACR: replaced sprintf with sprintf_safe
		#if 0
		sprintf(xmpValue, "%d,%.5lf%c", deg, fracMin, ref);
		#else
		sprintf_safe ( xmpValue, sizeof (xmpValue), "%d,%.5lf%c", deg, fracMin, ref);
		#endif
		////////////////////////////////////////////////////////////////
		}
	else
		{
		////////////////////////////////////////////////////////////////
		// ACR: replaced sprintf with sprintf_safe
		#if 0
		sprintf(xmpValue, "%d,%d,%d%c", deg, min, sec, ref);
		#else
		sprintf_safe ( xmpValue, sizeof (xmpValue), "%d,%d,%d%c", deg, min, sec, ref);
		#endif
		////////////////////////////////////////////////////////////////
		}
	value = xmpValue;
	
	return true;
}

bool TradQT_Manager::FormatGPSPropertyToLocation(std::string &xmpValue, XMP_StringPtr propName) {

	const char * strPtr = xmpValue.c_str();
	XMP_Uns32 deg = 0, min = 0, minDenom = 1, sec = 0, degCheck = 0;
	if ((*strPtr < '0') || (*strPtr > '9')) return false;	// Bad XMP string.
	for (; ('0' <= *strPtr) && (*strPtr <= '9'); ++strPtr) deg = deg * 10 + (*strPtr - '0');
	if (*strPtr == ',') strPtr++;
	else return false;
	char value[120];
	if (('0' <= *strPtr) && (*strPtr <= '9')) {
		for (; ('0' <= *strPtr) && (*strPtr <= '9'); ++strPtr) min = min * 10 + (*strPtr - '0');

		// Extract the fractional minutes or seconds if present.

		if (*strPtr == '.') {
			++strPtr;	// Skip the period.
			for (; ('0' <= *strPtr) && (*strPtr <= '9'); ++strPtr) {
				minDenom *= 10;
				min = min * 10 + (*strPtr - '0');
			}
		}
		else {
			if (*strPtr == ',') ++strPtr;
			for (; ('0' <= *strPtr) && (*strPtr <= '9'); ++strPtr) sec = sec * 10 + (*strPtr - '0');
		}

		double decimalDegrees = deg + min / (minDenom*60.0);
		char ref = *strPtr;
		char d[4], m[3], s[3], mref[2];

		if (strncmp(propName,"GPSLatitude",strlen("GPSLatitude")) == 0) {
			degCheck = 10;
			if ((ref != 'N') && (ref != 'S')) return false;
			else {
				mref[0] = (*strPtr == 'N') ? '+' : '-';
				mref[1] = '\0';
				////////////////////////////////////////////////////////////////
				// ACR: replaced sprintf with sprintf_safe
				#if 0
				(deg < 10) ? sprintf(d, "0%d", deg) : sprintf(d, "%d", deg);
				#else
				(deg < 10) ? sprintf_safe(d, sizeof (d), "0%d", deg) : sprintf_safe(d, sizeof (d), "%d", deg);
				#endif
				////////////////////////////////////////////////////////////////
			}

		}
		else {
			degCheck = 100;
			if ((ref != 'E') && (ref != 'W')) return false;
			else {
				////////////////////////////////////////////////////////////////
				// ACR: replaced sprintf with sprintf_safe
				#if 0
				if (deg < 10)
					sprintf(d, "00%d", deg);
				else if (deg<100)
					sprintf(d, "0%d", deg);
				else
					sprintf(d, "%d", deg);
				#else
				if (deg < 10)
					sprintf_safe(d, sizeof(d), "00%d", deg);
				else if (deg<100)
					sprintf_safe(d, sizeof(d), "0%d", deg);
				else
					sprintf_safe(d, sizeof(d), "%d", deg);
				#endif
				////////////////////////////////////////////////////////////////
				mref[0] = (*strPtr == 'E') ? '+' : '-';
				mref[1] = '\0';
			}

		}

		if (minDenom == 1 && sec < 100 && min < 100) {
			////////////////////////////////////////////////////////////////
			// ACR: replaced sprintf with sprintf_safe
			#if 0
			(min < 10) ? sprintf(m, "0%d", min) : sprintf(m, "%d", min);
			(sec < 10) ? sprintf(s, "0%d", sec) : sprintf(s, "%d", sec);
			#else
			(min < 10) ? sprintf_safe(m, sizeof (m), "0%d", min) : sprintf_safe(m, sizeof(m), "%d", min);
			(sec < 10) ? sprintf_safe(s, sizeof (s), "0%d", sec) : sprintf_safe(s, sizeof(s), "%d", sec);
			#endif
			////////////////////////////////////////////////////////////////
			strcpy(value, mref);
			strcat(value, d);
			strcat(value, m);
			strcat(value, s);
		}

		else {
			////////////////////////////////////////////////////////////////
			// ACR: replaced sprintf with sprintf_safe
			#if 0
			if (deg >= degCheck)
				sprintf(value, "%c%0.5lf", mref[0], decimalDegrees);
			else
				if ((deg < 10 && (strncmp(propName,"GPSLatitude",strlen("GPSLatitude")) == 0)) || (deg < 100 && (strncmp(propName,"GPSLongitude", strlen("GPSLongitude")) == 0)))
					sprintf(value, "%c0%0.5lf", mref[0], decimalDegrees);
				else
					sprintf(value, "%c00%0.5lf", mref[0], decimalDegrees);
			#else
			if (deg >= degCheck)
				sprintf_safe(value, sizeof (value), "%c%0.5lf", mref[0], decimalDegrees);
			else
				if ((deg < 10 && (strncmp(propName,"GPSLatitude",strlen("GPSLatitude")) == 0)) || (deg < 100 && (strncmp(propName,"GPSLongitude", strlen("GPSLongitude")) == 0)))
					sprintf_safe(value, sizeof (value), "%c0%0.5lf", mref[0], decimalDegrees);
				else
					sprintf_safe(value, sizeof (value), "%c00%0.5lf", mref[0], decimalDegrees);
			#endif
			////////////////////////////////////////////////////////////////
		}
	}
	xmpValue = value;
	return true;
}

bool TradQT_Manager::isValidType(XMP_Uns32 typeCode) {
	switch (typeCode) {
		case kData_Implicit:
		case kData_UTF_8:
		case kData_UTF_16:
		case kData_ShiftJIS: return true;
	}

	return false;
};

#endif
// =================================================================================================
// TradQT_Manager::ImportSimpleXMP
// ===============================
//
// Update a simple XMP property if the QT value looks newer.

bool TradQT_Manager::ImportSimpleXMP ( XMP_Uns32 id, SXMPMeta * xmp, XMP_StringPtr ns, XMP_StringPtr prop ) const
{

	try {

		InfoMapCPos infoPos = this->parsedBoxes.find ( id );
		if ( infoPos == this->parsedBoxes.end() ) return false;
		if ( infoPos->second.values.empty() ) return false;
		
		std::string xmpValue, tempValue;
		XMP_OptionBits flags;
		bool xmpExists = xmp->GetProperty ( ns, prop, &xmpValue, &flags );
		if ( xmpExists && (! XMP_PropIsSimple ( flags )) ) {
			XMP_Throw ( "TradQT_Manager::ImportSimpleXMP - XMP property must be simple", kXMPErr_BadParam );
		}
		
		bool convertOK;
		const ValueInfo & qtItem = infoPos->second.values[0];	// ! Use the first QT entry.
	
		if ( xmpExists ) {
			convertOK = ConvertToMacLang ( xmpValue, qtItem.macLang, &tempValue );
			if ( ! convertOK ) return false;	// throw?
			if ( tempValue == qtItem.macValue ) return false;	// QT value matches back converted XMP value.
		}
	
		convertOK = ConvertFromMacLang ( qtItem.macValue, qtItem.macLang, &tempValue );
		if ( ! convertOK ) return false;	// throw?
		xmp->SetProperty ( ns, prop, tempValue.c_str() );
		return true;
	
	} catch ( ... ) {

		return false;	// Don't let one failure abort other imports.	
	
	}
		
}	// TradQT_Manager::ImportSimpleXMP

// =================================================================================================
// TradQT_Manager::ImportLangItem
// ==============================
//
// Update a specific XMP AltText item if the QuickTime value looks newer.

bool TradQT_Manager::ImportLangItem ( const ValueInfo & qtItem, SXMPMeta * xmp,
									  XMP_StringPtr ns, XMP_StringPtr langArray ) const
{

	try {

		XMP_StringPtr genericLang, specificLang;
		if ( qtItem.xmpLang[0] != 0 ) {
			genericLang  = qtItem.xmpLang;
			specificLang = qtItem.xmpLang;
		} else {
			genericLang  = "";
			specificLang = "x-default";
		}
	
		bool convertOK;
		std::string xmpValue, tempValue, actualLang;
		bool xmpExists = xmp->GetLocalizedText ( ns, langArray, genericLang, specificLang, &actualLang, &xmpValue, 0 );
		if ( xmpExists ) {
			convertOK = ConvertToMacLang ( xmpValue, qtItem.macLang, &tempValue );
			if ( ! convertOK ) return false;	// throw?
			if ( tempValue == qtItem.macValue ) return true;	// QT value matches back converted XMP value.
			specificLang = actualLang.c_str();
		}
	
		convertOK = ConvertFromMacLang ( qtItem.macValue, qtItem.macLang, &tempValue );
		if ( ! convertOK ) return false;	// throw?
		xmp->SetLocalizedText ( ns, langArray, "", specificLang, tempValue.c_str() );
		return true;
	
	} catch ( ... ) {

		return false;	// Don't let one failure abort other imports.	
	
	}

}	// TradQT_Manager::ImportLangItem

// =================================================================================================
// TradQT_Manager::ImportLangAltXMP
// ================================
//
// Update items in the XMP array if the QT value looks newer.

bool TradQT_Manager::ImportLangAltXMP ( XMP_Uns32 id, SXMPMeta * xmp, XMP_StringPtr ns, XMP_StringPtr langArray ) const
{

	try {

		InfoMapCPos infoPos = this->parsedBoxes.find ( id );
		if ( infoPos == this->parsedBoxes.end() ) return false;
		if ( infoPos->second.values.empty() ) return false;	// Quit now if there are no values.
		
		XMP_OptionBits flags;
		bool xmpExists = xmp->GetProperty ( ns, langArray, 0, &flags );
		if ( ! xmpExists ) {
			xmp->SetProperty ( ns, langArray, 0, kXMP_PropArrayIsAltText );
		} else if ( ! XMP_ArrayIsAltText ( flags ) ) {
			XMP_Throw ( "TradQT_Manager::ImportLangAltXMP - XMP array must be AltText", kXMPErr_BadParam );
		}
	
		// Process all of the QT values, looking up the appropriate XMP language for each.
		
		bool haveMappings = false;
		const ValueVector & qtValues = infoPos->second.values;
		
		for ( size_t i = 0, limit = qtValues.size(); i < limit; ++i ) {
			const ValueInfo & qtItem = qtValues[i];
			if ( *qtItem.xmpLang == 0 ) continue;	// Only do known mappings in the loop.
			haveMappings |= this->ImportLangItem ( qtItem, xmp, ns, langArray );
		}
		
		if ( ! haveMappings ) {
			// If nothing mapped, process the first QT item to XMP's "x-default".
			haveMappings = this->ImportLangItem ( qtValues[0], xmp, ns, langArray );	// ! No xmpLang implies "x-default".
		}
	
		return haveMappings;
	
	} catch ( ... ) {

		return false;	// Don't let one failure abort other imports.	
	
	}

}	// TradQT_Manager::ImportLangAltXMP

// =================================================================================================
// TradQT_Manager::ExportSimpleXMP
// ===============================
//
// Export a simple XMP value to the first existing QuickTime item. Delete all of the QT values if the
// XMP value is empty or the XMP does not exist.

// ! We don't create new QuickTime items since we don't know the language.

void TradQT_Manager::ExportSimpleXMP ( XMP_Uns32 id, const SXMPMeta & xmp, XMP_StringPtr ns, XMP_StringPtr prop,
									   bool createWithZeroLang /* = false */  )
{
	std::string xmpValue, macValue;

	InfoMapPos infoPos = this->parsedBoxes.find ( id );
	bool qtFound = (infoPos != this->parsedBoxes.end()) && (! infoPos->second.values.empty());

	bool xmpFound = xmp.GetProperty ( ns, prop, &xmpValue, 0 );
	if ( (! xmpFound) || (xmpValue.empty()) ) {
		if ( qtFound ) {
			this->parsedBoxes.erase ( infoPos );
			this->changed = true;
		}
		return;
	}
	
	XMP_Assert ( xmpFound );
	if ( ! qtFound ) {
		if ( ! createWithZeroLang ) return;
		infoPos = this->parsedBoxes.insert ( this->parsedBoxes.end(),
											 InfoMap::value_type ( id, ParsedBoxInfo ( id ) ) );
		ValueVector * newValues = &infoPos->second.values;
		newValues->push_back ( ValueInfo() );
		ValueInfo * newValue = &newValues->back();
		newValue->macLang = 0;	// Happens to be langEnglish.
		newValue->xmpLang = kMacToXMPLang_0_94[0];
		this->changed = infoPos->second.changed = true;
	}
		
	ValueInfo * qtItem = &infoPos->second.values[0];	// ! Use the first QT entry.
	if ( ! IsMacLangKnown ( qtItem->macLang ) ) return;
	
	bool convertOK = ConvertToMacLang ( xmpValue, qtItem->macLang, &macValue );
	if ( convertOK && (macValue != qtItem->macValue) ) {
		qtItem->macValue = macValue;
		this->changed = infoPos->second.changed = true;
	}
	
}	// TradQT_Manager::ExportSimpleXMP

// =================================================================================================
// TradQT_Manager::ExportLangAltXMP
// ================================
//
// Export XMP LangAlt array items to QuickTime, where the language and encoding mappings are known.
// If there are no known language and encoding mappings, map the XMP default item to the first
// existing QuickTime item.

void TradQT_Manager::ExportLangAltXMP ( XMP_Uns32 id, const SXMPMeta & xmp, XMP_StringPtr ns, XMP_StringPtr langArray )
{
	bool haveMappings = false;
	std::string xmpPath, xmpValue, xmpLang, macValue;

	InfoMapPos infoPos = this->parsedBoxes.find ( id );
	if ( infoPos == this->parsedBoxes.end() ) {
		infoPos = this->parsedBoxes.insert ( this->parsedBoxes.end(), 
											 InfoMap::value_type ( id, ParsedBoxInfo ( id ) ) );
	}
	
	ValueVector * qtValues = &infoPos->second.values;
	XMP_Index xmpCount = xmp.CountArrayItems ( ns, langArray );
	bool convertOK;

	if ( xmpCount == 0 ) {
		// Delete the "mappable" QuickTime items if there are no XMP values. Leave the others alone.
		for ( int i = (int)qtValues->size()-1; i > 0; --i ) {	// ! Need a signed index.
			if ( (*qtValues)[i].xmpLang[0] != 0 ) {
				qtValues->erase ( qtValues->begin() + i );
				this->changed = infoPos->second.changed = true;
			}
		}
		return;
	}
	
	// Go through the XMP and look for a related macLang QuickTime item to update or create.
	
	for ( XMP_Index xmpIndex = 1; xmpIndex <= xmpCount; ++xmpIndex ) {	// ! XMP index starts at 1!

		SXMPUtils::ComposeArrayItemPath ( ns, langArray, xmpIndex, &xmpPath );
		if ( !xmp.GetProperty ( ns, xmpPath.c_str(), &xmpValue, 0 ) ) continue;
		xmp.GetQualifier ( ns, xmpPath.c_str(), kXMP_NS_XML, "lang", &xmpLang, 0 );
		if ( xmpLang == "x-default" ) continue;
		
		XMP_Uns16 macLang = GetMacLang ( &xmpLang );
		if ( macLang == kNoMacLang ) continue;
		
		size_t qtIndex, qtLimit;
		for ( qtIndex = 0, qtLimit = qtValues->size(); qtIndex < qtLimit; ++qtIndex ) {
			if ( (*qtValues)[qtIndex].macLang == macLang ) break;
		}

		if ( qtIndex == qtLimit ) {
			// No existing QuickTime item, try to create one.
			if ( ! IsMacLangKnown ( macLang ) ) continue;
			qtValues->push_back ( ValueInfo() );
			qtIndex = qtValues->size() - 1;
			ValueInfo * newItem = &((*qtValues)[qtIndex]);
			newItem->macLang = macLang;
			newItem->xmpLang = GetXMPLang ( macLang );	// ! Use the 2 character root language.
		}
		
		ValueInfo * qtItem = &((*qtValues)[qtIndex]);
		qtItem->marked = true;	// Mark it whether updated or not, don't delete it in the next pass.

		convertOK = ConvertToMacLang ( xmpValue, qtItem->macLang, &macValue );
		if ( convertOK && (macValue != qtItem->macValue) ) {
			qtItem->macValue.swap ( macValue );	// No need to make a copy.
			haveMappings = true;
		}

	}
	this->changed |= haveMappings;
	infoPos->second.changed |= haveMappings;
	
	// Go through the QuickTime items that are unmarked and delete those that have an xmpLang
	// and known macScript. Clear all marks.

	for ( int i = (int)qtValues->size()-1; i > 0; --i ) {	// ! Need a signed index.
		ValueInfo * qtItem = &((*qtValues)[i]);
		if ( qtItem->marked ) {
			qtItem->marked = false;
		} else if ( (qtItem->xmpLang[0] != 0) && IsMacLangKnown ( qtItem->macLang ) ) {
			qtValues->erase ( qtValues->begin() + i );
			this->changed = infoPos->second.changed = true;
		}
	}
	
	// If there were no mappings, export the XMP default item to the first QT item.

	if ( (! haveMappings) && (! qtValues->empty()) ) {
	
		bool ok = xmp.GetLocalizedText ( ns, langArray, "", "x-default", 0, &xmpValue, 0 );
		if ( ! ok ) return;
		
		ValueInfo * qtItem = &((*qtValues)[0]);
		if ( ! IsMacLangKnown ( qtItem->macLang ) ) return;
		
		convertOK = ConvertToMacLang ( xmpValue, qtItem->macLang, &macValue );
		if ( convertOK && (macValue != qtItem->macValue) ) {
			qtItem->macValue.swap ( macValue );	// No need to make a copy.
			this->changed = infoPos->second.changed = true;
		}

	}

}	// TradQT_Manager::ExportLangAltXMP

// =================================================================================================
// TradQT_Manager::UpdateChangedBoxes
// ==================================

void TradQT_Manager::UpdateChangedBoxes ( MOOV_Manager * moovMgr )
{
	MOOV_Manager::BoxInfo udtaInfo;
	MOOV_Manager::BoxRef  udtaRef = moovMgr->GetBox ( "moov/udta", &udtaInfo );
	XMP_Assert ( (udtaRef != 0) || (udtaInfo.childCount == 0) );
	
	if ( udtaRef != 0 ) {	// Might not have been a moov/udta box in the parse.

		// First go through the moov/udta/�... children and delete those that are not in the map.
	
		for ( XMP_Uns32 ordinal = udtaInfo.childCount; ordinal > 0; --ordinal ) {	// ! Go backwards because of deletions.
	
			MOOV_Manager::BoxInfo currInfo;
			MOOV_Manager::BoxRef  currRef = moovMgr->GetNthChild ( udtaRef, (ordinal-1), &currInfo );
			if ( currRef == 0 ) break;	// Sanity check, should not happen.
			if ( (currInfo.boxType >> 24) != 0xA9 ) continue;
			if ( currInfo.contentSize < 2+2+1 ) continue;	// These were skipped by ParseCachedBoxes.
			
			InfoMapPos infoPos = this->parsedBoxes.find ( currInfo.boxType );
			if ( infoPos == this->parsedBoxes.end() ) moovMgr->DeleteNthChild ( udtaRef, (ordinal-1) );
		
		}
	
	}
	
	// Now go through the changed items in the map and update them in the moov/udta subtree.
	
	InfoMapCPos infoPos = this->parsedBoxes.begin();
	InfoMapCPos infoEnd = this->parsedBoxes.end();
	
	for ( ; infoPos != infoEnd; ++infoPos ) {

		ParsedBoxInfo * qtItem = (ParsedBoxInfo*) &infoPos->second;
		if ( ! qtItem->changed ) continue;
		qtItem->changed = false;
		
		XMP_Uns32 qtTotalSize = 0;	// Total size of the QT values, ignoring empty values.
		for ( size_t i = 0, limit = qtItem->values.size(); i < limit; ++i ) {
			if ( ! qtItem->values[i].macValue.empty() ) {
				if ( qtItem->values[i].macValue.size() > 0xFFFF ) qtItem->values[i].macValue.erase ( 0xFFFF );
				qtTotalSize += (XMP_Uns32)(2+2 + qtItem->values[i].macValue.size());
			}
		}
		
		if ( udtaRef == 0 ) {	// Might not have been a moov/udta box in the parse.
			moovMgr->SetBox ( "moov/udta", 0, 0 );
			udtaRef = moovMgr->GetBox ( "moov/udta", &udtaInfo );
			XMP_Assert ( udtaRef != 0 );
		}
		
		if ( qtTotalSize == 0 ) {
		
			moovMgr->DeleteTypeChild ( udtaRef, qtItem->id );
		
		} else {
		
			// Compose the complete box content.
			
			RawDataBlock fullValue;
			fullValue.assign ( qtTotalSize, 0 );
			XMP_Uns8 * valuePtr = &fullValue[0];
		
			for ( size_t i = 0, limit = qtItem->values.size(); i < limit; ++i ) {
				XMP_Assert ( qtItem->values[i].macValue.size() <= 0xFFFF );
				XMP_Uns16 textSize = (XMP_Uns16)qtItem->values[i].macValue.size();
				if ( textSize == 0 ) continue;
				PutUns16BE ( textSize, valuePtr );									valuePtr += 2;
				PutUns16BE ( qtItem->values[i].macLang, valuePtr );					valuePtr += 2;
				memcpy ( valuePtr, qtItem->values[i].macValue.c_str(), textSize );	valuePtr += textSize;
			}
			
			// Look for an existing box to update, else add a new one.

			MOOV_Manager::BoxInfo itemInfo;
			MOOV_Manager::BoxRef  itemRef = moovMgr->GetTypeChild ( udtaRef, qtItem->id, &itemInfo );
			
			if ( itemRef != 0 ) {
				moovMgr->ISOBaseMedia_Manager::SetBox( itemRef, &fullValue[0], qtTotalSize );
			} else {
				moovMgr->AddChildBox ( udtaRef, qtItem->id, &fullValue[0], qtTotalSize );
			}
			
		}

	}

}	// TradQT_Manager::UpdateChangedBoxes

#if AdobePrivate
#if EnableITunesMetadata

// =================================================================================================
// =================================================================================================
// iTunes_Manager
// =================================================================================================
// =================================================================================================

// =================================================================================================
// iTunes_Manager::AppendOneValue
// ==============================

bool iTunes_Manager::AppendOneValue ( const XMP_Uns8 * contentPtr, XMP_Uns32 contentSize, DataVector * values )
{
	if ( contentSize < 8 ) return false;

	XMP_Uns16 typeCode = GetUns16BE ( contentPtr+2 );
	XMP_Uns32 locale   = GetUns32BE ( contentPtr+4 );
	
	values->push_back ( DataBoxInfo ( typeCode, locale ) );
	values->back().value.assign ( (char*)(contentPtr+8), (size_t)(contentSize-8) );

	return IsTextType ( typeCode );

}	// iTunes_Manager::AppendOneValue

// =================================================================================================
// iTunes_Manager::ParseCachedBoxes
// ================================
//
// Parse the children of the 'moov'/'udta'/'meta'/'ilst' box. This is a sequence of "plain" boxes
// (type other than '----') and "fancy" boxes (type is '----'). The plain boxes have nested 'data'
// value boxes. The fancy boxes have nested 'mean', 'name', and 'data' value boxes. For both plain
// and fancy, there can be any number of 'data' boxes - strictly one or more - that contain various
// forms of a value. The 'data' content is a UInt32 data type, a UInt32 locale (must be zero), and
// and a value of the given type. Some typical types include UTF-8 text, UTF-16 text, JIS text, a
// UInt32 duration in milliseconds, a numberic genre code, etc.
//
// The plain boxes are identified by their type. The fancy boxes are identified by a meaning or
// component name that comes from the 'mean' and 'name' boxes. This uses reverse DNS notation, e.g.
// "com.apple.quicktime.windowlocation". The full meaning can be in the 'mean' box, with no 'name'
// box. Or the leaf part can be in the 'name' box, e.g, "com.apple.quicktime" and "windowlocation".

bool iTunes_Manager::ParseCachedBoxes ( const MOOV_Manager & moovMgr )
{
	MOOV_Manager::BoxInfo ilstInfo;
	MOOV_Manager::BoxRef  ilstRef = moovMgr.GetBox ( "moov/udta/meta/ilst", &ilstInfo );
	if ( ilstRef == 0 ) return false;
	
	for ( size_t i = 0; i < ilstInfo.childCount; ++i ) {
	
		MOOV_Manager::BoxInfo itemInfo;
		MOOV_Manager::BoxRef  itemRef = moovMgr.GetNthChild ( ilstRef, i, &itemInfo );
		if ( itemRef == 0 ) break;	// Sanity check, should not happen.
		if ( itemInfo.childCount == 0 ) continue;	// Only process children with more nested boxes.
	
		bool isText;
		MOOV_Manager::BoxInfo dataInfo, meanInfo, nameInfo;
		MOOV_Manager::BoxRef  dataRef,  meanRef,  nameRef;

		if ( itemInfo.boxType != ISOMedia::k_hyphens ) {
		
			// This is a "plain" metadata item, process all of the nested 'data' boxes.
			
			PlainMapPos newMapPos = this->plainItems.insert ( this->plainItems.end(),
															  PlainMap::value_type ( itemInfo.boxType,
																				     PlainBoxInfo ( itemInfo.boxType ) ) );
			PlainBoxInfo * newItem = &newMapPos->second;

			for ( XMP_Uns32 j = 0; j < itemInfo.childCount; ++j ) {

				dataRef = moovMgr.GetNthChild ( itemRef, j, &dataInfo );
				if ( dataRef == 0 ) break;	// Sanity check, should not happen.
				if ( (dataInfo.boxType != ISOMedia::k_data) || (dataInfo.contentSize == 0) ) continue;

				isText = this->AppendOneValue ( dataInfo.content, dataInfo.contentSize, &newItem->values );
				if ( isText && (newItem->firstText == std::string::npos) ) newItem->firstText = newItem->values.size()-1;

			}
		
		} else {

			// This is a "fancy" metadata item, find the 'mean' and 'name' children, process the 'data' children.
			// There must be one 'mean' child, an optional 'name' child, any number of 'data' children.

			FancyMapPos newMapPos;
			XMP_Uns32   meanVFlags, nameVFlags;
			std::string meanStr, nameStr;
			
			meanRef = moovMgr.GetTypeChild ( itemRef, ISOMedia::k_mean, &meanInfo );
			if ( (meanRef == 0) || (meanInfo.contentSize < 4+1) ) continue;
			meanVFlags = GetUns32BE ( meanInfo.content );
			meanStr.assign ( (char*) (meanInfo.content + 4), (meanInfo.contentSize - 4) );

			nameRef = moovMgr.GetTypeChild ( itemRef, ISOMedia::k_name, &nameInfo );
			if ( nameInfo.contentSize < 4+1 ) nameRef = 0;

			if ( nameRef == 0 ) {
				nameVFlags = 0;
				newMapPos = this->fancyItems.insert ( this->fancyItems.end(),
													  FancyMap::value_type ( meanStr, FancyBoxInfo() ) );
			} else {
				nameVFlags = GetUns32BE ( nameInfo.content );
				nameStr.assign ( (char*) (nameInfo.content + 4), (nameInfo.contentSize - 4) );
				std::string fullItemName = meanStr;
				fullItemName += '.';
				fullItemName += nameStr;
				newMapPos = this->fancyItems.insert ( this->fancyItems.end(),
													  FancyMap::value_type ( fullItemName, FancyBoxInfo() ) );
			}
			
			FancyBoxInfo * newItem = &newMapPos->second;
			
			newItem->meanVFlags = meanVFlags;
			newItem->nameVFlags = nameVFlags;
			newItem->meanStr.swap ( meanStr );
			newItem->nameStr.swap ( nameStr );
			
			for ( XMP_Uns32 j = 0; j < itemInfo.childCount; ++j ) {

				dataRef = moovMgr.GetNthChild ( itemRef, j, &dataInfo );
				if ( dataRef == 0 ) break;	// Sanity check, should not happen.
				if ( (dataInfo.boxType != ISOMedia::k_data) || (dataInfo.contentSize == 0) ) continue;

				isText = this->AppendOneValue ( dataInfo.content, dataInfo.contentSize, &newItem->values );
				if ( isText && (newItem->firstText == std::string::npos) ) newItem->firstText = newItem->values.size()-1;

			}

		}
	
	}
	
	return ( (! this->plainItems.empty()) || (! this->fancyItems.empty()) );

}	// iTunes_Manager::ParseCachedBoxes

// =================================================================================================
// DecodeString
// ============

static bool DecodeString ( const std::string & iTunesValue, XMP_Uns16 typeCode, std::string * utf8Value )
{
	XMP_Assert ( IsTextType ( typeCode ) );
	
	if ( typeCode == kiTunesData_UTF_16 ) {
		FromUTF16 ( (UTF16Unit*)iTunesValue.data(), iTunesValue.size()/2, utf8Value, true /* big endian */ );
	} else if ( typeCode != kiTunesData_ShiftJIS ) {
		*utf8Value = iTunesValue;	// The value is already UTF-8.
	} else {
		#if XMP_MacBuild
			ReconcileUtils::MacEncodingToUTF8 ( smJapanese, langJapanese, (XMP_Uns8*)iTunesValue.c_str(), iTunesValue.size(), utf8Value );
		#elif XMP_WinBuild
			ReconcileUtils::WinEncodingToUTF8 ( 932, (XMP_Uns8*)iTunesValue.c_str(), iTunesValue.size(), utf8Value );
        #elif XMP_iOSBuild
            ReconcileUtils::IOSConvertEncoding(kCFStringEncodingMacJapanese, kCFStringEncodingUTF8, (XMP_Uns8*)iTunesValue.c_str(), iTunesValue.size(), utf8Value);
		#else
			return false;
		#endif
	}
	
	return true;
	
}	// DecodeString

// =================================================================================================
// EncodeString
// ============

static bool EncodeString ( const std::string & utf8Value, XMP_Uns16 typeCode, std::string * iTunesValue )
{
	XMP_Assert ( IsTextType ( typeCode ) );
	
	if ( typeCode == kiTunesData_UTF_16 ) {
		ToUTF16 ( (UTF8Unit*)utf8Value.data(), utf8Value.size(), iTunesValue, true /* big endian */ );
	} else if ( typeCode != kiTunesData_ShiftJIS ) {
		*iTunesValue = utf8Value;	// The value is already UTF-8.
	} else {
		#if XMP_MacBuild
			ReconcileUtils::UTF8ToMacEncoding ( smJapanese, langJapanese, (XMP_Uns8*)utf8Value.c_str(), utf8Value.size(), iTunesValue );
		#elif XMP_WinBuild
			ReconcileUtils::UTF8ToWinEncoding ( 932, (XMP_Uns8*)utf8Value.c_str(), utf8Value.size(), iTunesValue );
        #elif XMP_iOSBuild
            ReconcileUtils::IOSConvertEncoding(kCFStringEncodingUTF8, kCFStringEncodingMacJapanese, (XMP_Uns8*)utf8Value.c_str(), utf8Value.size(), iTunesValue);
		#else
			return false;
		#endif
	}
	
	return true;
	
}	// EncodeString

// =================================================================================================
// iTunes_Manager::GetTextValue
// ============================

bool iTunes_Manager::GetTextValue ( XMP_Uns32 id, std::string & utf8Value ) const
{

	try {
	
		PlainMapCPos plainPos = this->plainItems.find ( id );
		if ( plainPos == this->plainItems.end() ) return false;
		
		const PlainBoxInfo & plainInfo = plainPos->second;
		if ( plainInfo.firstText == std::string::npos ) return false;
		
		const DataBoxInfo & iTunesValue = plainInfo.values[plainInfo.firstText];
		return DecodeString ( iTunesValue.value, iTunesValue.typeCode, &utf8Value );
	
	} catch ( ... ) {

		return false;	// Don't let one failure abort other imports.	
	
	}
	
}	// iTunes_Manager::GetTextValue

// =================================================================================================
// iTunes_Manager::GetTextValue
// ============================

bool iTunes_Manager::GetTextValue ( const std::string & fullName, std::string & utf8Value ) const
{

	try {
	
		FancyMapCPos fancyPos = this->fancyItems.find ( fullName );
		if ( fancyPos == this->fancyItems.end() ) return false;
		
		const FancyBoxInfo & fancyInfo = fancyPos->second;
		if ( fancyInfo.firstText == std::string::npos ) return false;
		
		const DataBoxInfo & iTunesValue = fancyInfo.values[fancyInfo.firstText];
		return DecodeString ( iTunesValue.value, iTunesValue.typeCode, &utf8Value );
	
	} catch ( ... ) {

		return false;	// Don't let one failure abort other imports.	
	
	}

}	// iTunes_Manager::GetTextValue

// =================================================================================================
// iTunes_Manager::SetTextValue
// ============================

void iTunes_Manager::SetTextValue ( XMP_Uns32 id, const std::string & utf8Value, XMP_Uns16 typeCode )
{
	PlainMapPos plainPos = this->plainItems.find ( id );
	if ( plainPos == this->plainItems.end() ) {
		plainPos = this->plainItems.insert ( this->plainItems.end(),
											 PlainMap::value_type ( id, PlainBoxInfo ( id ) ) );
		this->changed = plainPos->second.changed = true;
	}
	
	PlainBoxInfo * plainInfo = &plainPos->second;
	
	XMP_Uns32 locale = 0;
	std::string newValue;
	bool ok;
	
	if ( plainInfo->firstText == std::string::npos ) {
		ok = EncodeString ( utf8Value, typeCode, &newValue );
		if ( ! ok ) return;
	} else {
		DataBoxInfo & oldData = plainInfo->values[plainInfo->firstText];
		typeCode = oldData.typeCode;
		locale   = oldData.locale;
		ok = EncodeString ( utf8Value, typeCode, &newValue );
		if ( (! ok) || (newValue == oldData.value) ) return;	// No change.
	}
	
	plainInfo->values.clear();
	plainInfo->values.push_back ( DataBoxInfo ( typeCode, locale ) );
	plainInfo->values[0].value.swap ( newValue );
	plainInfo->firstText = 0;
	
	this->changed = plainInfo->changed = true;
	
}	// iTunes_Manager::SetTextValue

// =================================================================================================
// iTunes_Manager::SetTextValue
// ============================

void iTunes_Manager::SetTextValue ( const std::string & fullName, const std::string & utf8Value, XMP_Uns16 typeCode )
{
	FancyMapPos fancyPos = this->fancyItems.find ( fullName );

	if ( fancyPos == this->fancyItems.end() ) {

		fancyPos = this->fancyItems.insert ( this->fancyItems.end(),
											 FancyMap::value_type ( fullName, FancyBoxInfo() ) );
		FancyBoxInfo * newItem = &fancyPos->second;
		this->changed = newItem->changed = true;
		
		size_t lastDot = fullName.size();
		for ( --lastDot; (lastDot > 0) && (fullName[lastDot] != '.'); --lastDot ) {}
		if ( fullName[lastDot] != '.') {
			newItem->meanStr = fullName;	// No dots, just set the 'mean' part.
		} else {
			newItem->meanStr.assign ( fullName.c_str(), lastDot );
			newItem->nameStr.assign ( &fullName[lastDot+1] );
		}

	}
	
	FancyBoxInfo * fancyInfo = &fancyPos->second;
	
	XMP_Uns32 locale = 0;
	std::string newValue;
	bool ok;
	
	if ( fancyInfo->firstText == std::string::npos ) {
		ok = EncodeString ( utf8Value, typeCode, &newValue );
		if ( ! ok ) return;
	} else {
		DataBoxInfo & oldData = fancyInfo->values[fancyInfo->firstText];
		typeCode = oldData.typeCode;
		locale   = oldData.locale;
		ok = EncodeString ( utf8Value, typeCode, &newValue );
		if ( (! ok) || (newValue == oldData.value) ) return;	// No change.
	}
	
	fancyInfo->values.clear();
	fancyInfo->values.push_back ( DataBoxInfo ( typeCode, locale ) );
	fancyInfo->values[0].value.swap ( newValue );
	fancyInfo->firstText = 0;
	
	this->changed = fancyInfo->changed = true;
	
}	// iTunes_Manager::SetTextValue

// =================================================================================================
// iTunes_Manager::DeleteItem
// ==========================

void iTunes_Manager::DeleteItem ( XMP_Uns32 id )
{
	PlainMapPos plainPos = this->plainItems.find ( id );

	if ( plainPos != this->plainItems.end() ) {
		this->plainItems.erase ( plainPos );
		this->changed = true;
	}

}	// DeleteItem

// =================================================================================================
// iTunes_Manager::DeleteItem
// ==========================

void iTunes_Manager::DeleteItem ( const std::string & fullName )
{
	FancyMapPos fancyPos = this->fancyItems.find ( fullName );

	if ( fancyPos == this->fancyItems.end() ) {
		this->fancyItems.erase ( fancyPos );
		this->changed = true;
	}
	
}	// DeleteItem

// =================================================================================================
// iTunes_Manager::CreateDataBox
// =============================

void iTunes_Manager::CreateDataBox ( const DataBoxInfo & dataInfo,
									 const MOOV_Manager::BoxRef parentRef, MOOV_Manager * moovMgr )
{
	if ( dataInfo.value.empty() ) return;	// Ignore empty values.

	RawDataBlock dataContent;
	dataContent.assign ( (4+4 + dataInfo.value.size()), 0 );
	PutUns16BE ( dataInfo.typeCode, &dataContent[2] );	// Take advantage of zero pre-fill.
	PutUns32BE ( dataInfo.locale, &dataContent[4] );
	memcpy ( &dataContent[8], dataInfo.value.c_str(), dataInfo.value.size() );
	
	(void) moovMgr->AddChildBox ( parentRef, ISOMedia::k_data, &dataContent[0], (XMP_Uns32)dataContent.size() );

}	// iTunes_Manager::CreateDataBox

// =================================================================================================
// iTunes_Manager::FindFancyBox
// ============================

size_t iTunes_Manager::FindFancyBox ( const FancyBoxInfo & fancyInfo, const MOOV_Manager & moovMgr,
									  const MOOV_Manager::BoxRef ilstRef, const MOOV_Manager::BoxInfo & ilstInfo )
{

	for ( size_t i = 0, limit = ilstInfo.childCount; i < limit; ++i ) {
	
		MOOV_Manager::BoxInfo itemInfo;
		MOOV_Manager::BoxRef  itemRef = moovMgr.GetNthChild ( ilstRef, i, &itemInfo );
		if ( itemRef == 0 ) break;	// Sanity check, should not happen.
		if ( itemInfo.boxType != ISOMedia::k_hyphens ) continue;
		if ( itemInfo.childCount == 0 ) continue;	// These were ignored by ParseCachedBoxes.
		
		MOOV_Manager::BoxInfo meanInfo;
		MOOV_Manager::BoxRef  meanRef = moovMgr.GetTypeChild ( itemRef, ISOMedia::k_mean, &meanInfo );
		if ( (meanRef == 0) || (meanInfo.contentSize != (4 + fancyInfo.meanStr.size())) ) continue;
		if ( fancyInfo.meanStr != (char*)(meanInfo.content + 4) ) continue;
		
		if ( ! fancyInfo.nameStr.empty() ) {
			MOOV_Manager::BoxInfo nameInfo;
			MOOV_Manager::BoxRef  nameRef = moovMgr.GetTypeChild ( itemRef, ISOMedia::k_name, &nameInfo );
			if ( (nameRef == 0) || (nameInfo.contentSize != (4 + fancyInfo.nameStr.size())) ) continue;
			if ( fancyInfo.nameStr != (char*)(nameInfo.content + 4) ) continue;
		}
		
		return i;
		
	}
	
	return (size_t)(-1);	// Not found if the loop finishes.

}	// iTunes_Manager::FindFancyBox

// =================================================================================================
// iTunes_Manager::UpdateChangedBoxes
// ==================================
//
// Update the changed children of moov/udta/meta/ilst. The plain boxes have nested 'data' value
// boxes. The fancy boxes have nested 'mean', 'name', and 'data' value boxes. The 'data' box is a
// normal box, the 'mean' and 'name' boxes are full boxes. For both plain and fancy, there can be
// any number of 'data' boxes - strictly one or more - that contain various forms of a value. The
// 'data' content is a UInt32 data type, a UInt32 locale (must be zero), and and a value of the
// given type. Some typical types include UTF-8 text, UTF-16 text, JIS text, a UInt32 duration in
// milliseconds, a numberic genre code, etc.
//
// The plain boxes are identified by their type. The fancy boxes are identified by a meaning or
// component name that comes from the 'mean' and 'name' boxes. This uses reverse DNS notation, e.g.
// "com.apple.quicktime.windowlocation". The full meaning can be in the 'mean' box, with no 'name'
// box. Or the leaf part can be in the 'name' box, e.g, "com.apple.quicktime" and "windowlocation".
//
// ! As of 2008 iTunes only supported the split form of fancy box meaning.

void iTunes_Manager::UpdateChangedBoxes ( MOOV_Manager * moovMgr )
{
	MOOV_Manager::BoxInfo ilstInfo;
	MOOV_Manager::BoxRef  ilstRef = moovMgr->GetBox ( "moov/udta/meta/ilst", &ilstInfo );
	XMP_Assert ( (ilstRef != 0) || (ilstInfo.childCount == 0) );
	
	if ( ilstRef != 0 ) {	// Might not have been a moov/udta/meta/ilst box in the parse.

		// First go through the moov/udta/meta/ilst children and delete those that are not in the map.

		for ( XMP_Uns32 ordinal = ilstInfo.childCount; ordinal > 0; --ordinal ) {	// ! Go backwards because of deletions.
		
			MOOV_Manager::BoxInfo itemInfo;
			MOOV_Manager::BoxRef  itemRef = moovMgr->GetNthChild ( ilstRef, (ordinal-1), &itemInfo );
			if ( itemRef == 0 ) break;	// Sanity check, should not happen.
			if ( itemInfo.childCount == 0 ) continue;	// These were ignored by ParseCachedBoxes.
		
			if ( itemInfo.boxType != ISOMedia::k_hyphens ) {
			
				// The plain items are keyed by the box type.
	
				PlainMapPos plainPos = this->plainItems.find ( itemInfo.boxType );
				if ( plainPos == this->plainItems.end() ) moovMgr->DeleteNthChild ( ilstRef, (ordinal-1) );
			
			} else {
	
				// The fancy items are keyed by the full name.
		
				std::string fullName;
				
				MOOV_Manager::BoxInfo meanInfo;
				MOOV_Manager::BoxRef  meanRef = moovMgr->GetTypeChild ( itemRef, ISOMedia::k_mean, &meanInfo );
				if ( (meanRef == 0) || (meanInfo.contentSize < 4+1) ) continue;
				fullName.assign ( (char*) (meanInfo.content + 4), (meanInfo.contentSize - 4) );
	
				MOOV_Manager::BoxInfo nameInfo;
				MOOV_Manager::BoxRef  nameRef = moovMgr->GetTypeChild ( itemRef, ISOMedia::k_name, &nameInfo );
				if ( nameInfo.contentSize < 4+1 ) nameRef = 0;
				if ( nameRef != 0 ) {
					fullName += '.';
					fullName.append ( (char*) (nameInfo.content + 4), (nameInfo.contentSize - 4) );
				}
	
				FancyMapPos fancyPos = this->fancyItems.find ( fullName );
				if ( fancyPos == this->fancyItems.end() ) moovMgr->DeleteNthChild ( ilstRef, (ordinal-1) );
	
			}
	
		}
	
	}

	// Now go through the changed items in both maps and update them in the moov/udta/meta/ilst subtree.

	PlainMapCPos plainPos = this->plainItems.begin();
	PlainMapCPos plainEnd = this->plainItems.end();

	for ( ; plainPos != plainEnd; ++plainPos ) {

		PlainBoxInfo * plainItem = (PlainBoxInfo*) &plainPos->second;
		if ( ! plainItem->changed ) continue;
		plainItem->changed = false;
		
		bool haveValues = false;	// Ignore empty values.
		for ( size_t i = 0, limit = plainItem->values.size(); (! haveValues) && (i < limit); ++i ) {
			haveValues = (! plainItem->values[i].value.empty());
		}
		
		if ( ilstRef == 0 ) {	// Might not have been a moov/udta/meta/ilst box in the parse.
			MOOV_Manager::BoxRef metaRef = moovMgr->GetBox ( "moov/udta/meta", 0 );
			if ( metaRef == 0 ) {
				XMP_Uns32 z32 = 0;
				moovMgr->SetBox ( "moov/udta/meta", &z32, 4 );	// ! The ISO 'meta' box is a FullBox!
			}
			moovMgr->SetBox ( "moov/udta/meta/ilst", 0, 0 );
			ilstRef = moovMgr->GetBox ( "moov/udta/meta/ilst", &ilstInfo );
			XMP_Assert ( ilstRef != 0 );
		}

		MOOV_Manager::BoxRef itemRef = moovMgr->GetTypeChild ( ilstRef, plainItem->type, 0 );

		// Delete no matter what, will replace all content anyway and this makes it easier.
		if ( itemRef != 0 ) moovMgr->DeleteTypeChild ( ilstRef, plainItem->type );
		
		if ( haveValues ) {
			itemRef = moovMgr->AddChildBox ( ilstRef, plainItem->type, 0, 0 );
			for ( size_t i = 0, limit = plainItem->values.size(); i < limit; ++i ) {
				CreateDataBox ( plainItem->values[i], itemRef, moovMgr );
			}
		}
		
	}
	
	FancyMapCPos fancyPos = this->fancyItems.begin();
	FancyMapCPos fancyEnd = this->fancyItems.end();
	
	for ( ; fancyPos != fancyEnd; ++fancyPos ) {

		FancyBoxInfo * fancyItem = (FancyBoxInfo*) &fancyPos->second;
		if ( ! fancyItem->changed ) continue;
		fancyItem->changed = false;
		
		bool haveValues = false;	// Ignore empty values.
		for ( size_t i = 0, limit = fancyItem->values.size(); (! haveValues) && (i < limit); ++i ) {
			haveValues = (! fancyItem->values[i].value.empty());
		}
		
		if ( ilstRef == 0 ) {	// Might not have been a moov/udta/meta/ilst box in the parse.
			MOOV_Manager::BoxRef metaRef = moovMgr->GetBox ( "moov/udta/meta", 0 );
			if ( metaRef == 0 ) {
				XMP_Uns32 z32 = 0;
				moovMgr->SetBox ( "moov/udta/meta", &z32, 4 );	// ! The ISO 'meta' box is a FullBox!
			}
			moovMgr->SetBox ( "moov/udta/meta/ilst", 0, 0 );
			ilstRef = moovMgr->GetBox ( "moov/udta/meta/ilst", &ilstInfo );
			XMP_Assert ( ilstRef != 0 );
		}
		
		// Delete no matter what, will replace all content anyway and this makes it easier.
		size_t itemIndex = FindFancyBox ( *fancyItem, *moovMgr, ilstRef, ilstInfo );
		if ( itemIndex != (size_t)(-1) ) moovMgr->DeleteNthChild ( ilstRef, itemIndex );
		
		if ( haveValues ) {
		
			MOOV_Manager::BoxRef itemRef = moovMgr->AddChildBox ( ilstRef, ISOMedia::k_hyphens, 0, 0 );
			
			RawDataBlock stringBlock;
			stringBlock.assign ( (4 + fancyItem->meanStr.size()), 0 );
			memcpy ( &stringBlock[4], fancyItem->meanStr.c_str(), fancyItem->meanStr.size() );
			(void) moovMgr->AddChildBox ( itemRef, ISOMedia::k_mean, &stringBlock[0], (XMP_Uns32)stringBlock.size() );
			
			if ( ! fancyItem->nameStr.empty() ) {
				stringBlock.assign ( (4 + fancyItem->nameStr.size()), 0 );
				memcpy ( &stringBlock[4], fancyItem->nameStr.c_str(), fancyItem->nameStr.size() );
				(void) moovMgr->AddChildBox ( itemRef, ISOMedia::k_name, &stringBlock[0], (XMP_Uns32)stringBlock.size() );
			}

			for ( size_t i = 0, limit = fancyItem->values.size(); i < limit; ++i ) {
				CreateDataBox ( fancyItem->values[i], itemRef, moovMgr );
			}
		
		}
		
	}

}	// iTunes_Manager::UpdateChangedBoxes

// =================================================================================================
// iTunes_Manager::GetDataBuffer
// ============================

bool iTunes_Manager::GetDataBuffer ( XMP_Uns32 id, std::string & data, XMP_Uns16 typeCode /*= kiTunesData_Implicit*/ ) const
{
	try {
		PlainMapCPos plainPos = this->plainItems.find ( id );

		if ( plainPos == this->plainItems.end() ) return false;

		const PlainBoxInfo & plainInfo = plainPos->second;
		if ( plainInfo.values.size() == 0 ) return false;
		const DataBoxInfo & oldData = plainInfo.values[0];
		XMP_Assert ( oldData.typeCode == typeCode );
		data.assign ( oldData.value );
		return true;
	} catch ( ... ) {
		return false;	// Don't let one failure abort other imports.
	}
}	// iTunes_Manager::GetDataBuffer

// =================================================================================================

void iTunes_Manager::SetDataBuffer ( XMP_Uns32 id, const std::string & data, XMP_Uns16 typeCode /*= kiTunesData_Implicit*/ )
{
	XMP_Uns32 locale = 0;

	PlainMapPos plainPos = this->plainItems.find ( id );
	PlainBoxInfo * plainInfo = NULL;

	if ( plainPos == this->plainItems.end() ) {
		plainPos = this->plainItems.insert ( this->plainItems.end(),
			PlainMap::value_type ( id, PlainBoxInfo ( id ) ) );
		plainInfo = &plainPos->second;
	} else {
		plainInfo = &plainPos->second;
		XMP_Assert ( plainInfo->values.size() > 0 );
		DataBoxInfo & oldData = plainInfo->values[0];
		XMP_Assert ( oldData.typeCode == typeCode );
		locale = oldData.locale;

		if ( data.compare ( oldData.value ) == 0 ) return;	// No change.
	}

	plainInfo->values.clear();
	plainInfo->values.push_back ( DataBoxInfo ( typeCode, locale ) );
	plainInfo->values[0].value = data;
	plainInfo->firstText = 0;

	this->changed = plainInfo->changed = true;
}	// iTunes_Manager::SetDataBuffer

// =================================================================================================

bool iTunes_Manager::GetNumericValue ( XMP_Uns32 id, XMP_Int64 & value, XMP_Uns8 nBytes /*= 8 */ ) const
{
	XMP_Enforce ( nBytes == 1 || nBytes == 2 || nBytes == 4 || nBytes == 8 );
	std::string data;
	if ( GetDataBuffer ( id, data, kiTunesData_Integer ) ) {
		XMP_Enforce ( data.size() == nBytes );

		switch ( nBytes ) {
		case 1:
			{
				XMP_Int8 temp = * ( data.c_str() );
				value = temp;
			}
			break;

		case 2:
			{
				XMP_Int16 temp = GetUns16BE ( data.c_str() );
				value = temp;
			}
			break;

		case 4:
			{
				XMP_Int32 temp = GetUns32BE ( data.c_str() );
				value = temp;
			}
			break;

		case 8:
			{
				XMP_Int64 temp = GetUns64BE ( data.c_str() );
				value = temp;
			}
			break;
		}

		return true;
	} else {
		return false;
	}
}	// iTunes_Manager::GetNumericValue

// =================================================================================================

void iTunes_Manager::SetNumericValue ( XMP_Uns32 id, XMP_Int64 value, XMP_Uns8 nBytes /*= 8 */ )
{
	XMP_Enforce ( nBytes == 1 || nBytes == 2 || nBytes == 4 || nBytes == 8 );

	char tempBuffer[8];

	switch ( nBytes ) {
	case 1:
		{
			XMP_Uns8 temp = ( XMP_Uns8 ) value;
			tempBuffer[0] = temp;
		}
		break;

	case 2:
		{
			XMP_Uns16 temp = ( XMP_Uns16 ) value;
			PutUns16BE( temp, &tempBuffer[0] );
		}
		break;

	case 4:
		{
			XMP_Uns32 temp = ( XMP_Uns32 ) value;
			PutUns32BE ( temp, &tempBuffer[0] );
		}
		break;

	case 8:
		{
			XMP_Uns64 temp = ( XMP_Uns64 ) value;
			PutUns64BE ( temp, &tempBuffer[0] );
		}
		break;
	}

	std::string data;
	for ( XMP_Uns8 i = 0; i < nBytes; i++ )
		data += tempBuffer[i];

	SetDataBuffer ( id, data, kiTunesData_Integer );
}	// iTunes_Manager::SetNumericValue

// =================================================================================================

bool iTunes_Manager::GetName( std::string & song ) const
{
	return GetTextValue ( kiTunes_SongName, song );
}

// =================================================================================================

bool iTunes_Manager::GetArtist( std::string & artist ) const
{
	return GetTextValue ( kiTunes_Artist, artist );
}

// =================================================================================================

bool iTunes_Manager::GetAlbumArtist( std::string & albumArtist ) const
{
	return GetTextValue ( kiTunes_AlbumArtist, albumArtist );
}

// =================================================================================================

bool iTunes_Manager::GetAlbum( std::string & album ) const
{
	return GetTextValue ( kiTunes_Album, album );
}

// =================================================================================================

bool iTunes_Manager::GetGenre( std::string & genre ) const
{
	bool found = GetTextValue ( kiTunes_UserGenres, genre );
	if ( found ) {
		return found;
	} else {
		found = GetDataBuffer ( kiTunes_StdGenres, genre );
		if ( found ) {
			XMP_Assert ( genre.size() == 2 );
			XMP_Uns8 value = ( XMP_Uns8 ) genre.c_str()[1];
			XMP_Enforce ( value > 0 && value <= kMaxGenreID );
			genre = kGenreNames [ value - 1 ];
		}
		return found;
	}
}

// =================================================================================================

bool iTunes_Manager::GetReleaseDate( XMP_DateTime & releaseDate ) const
{
	std::string tempValue;
	bool found = GetTextValue ( kiTunes_ReleaseDate, tempValue );
	if ( found ) {
		try {
			SXMPUtils::ConvertToDate ( tempValue, &releaseDate );
		} catch ( ... ) {
			return false;
		}
	}
	return found;
}

// =================================================================================================

bool iTunes_Manager::GetComposer ( std::string & composer ) const
{
	return GetTextValue ( kiTunes_Composer, composer );
}

// =================================================================================================

bool iTunes_Manager::GetComments( std::string & comments ) const
{
	return GetTextValue ( kiTunes_Comment, comments );
}

// =================================================================================================

bool iTunes_Manager::GetTrackNumber( XMP_Uns16 & trackNumber ) const
{
	std::string tempValue;
	bool found = GetDataBuffer ( kiTunes_TrackNumber, tempValue );

	if ( found ) {
		XMP_Assert ( tempValue.size() >= 7 );

		XMP_Uns8 reservedLength = ( XMP_Uns8 ) tempValue.c_str()[1];
		XMP_Uns16 offsetForTrackNumber = 2 + reservedLength;
		XMP_Uns16 offsetForPlaylistLength = offsetForTrackNumber + 4;

		XMP_Assert ( tempValue.size() >= ( size_t ) ( offsetForPlaylistLength + 1 ) );
		XMP_Uns8 playListLength = ( XMP_Uns8 ) tempValue.c_str()[offsetForPlaylistLength];

		XMP_Assert ( tempValue.size() >= ( size_t ) ( offsetForPlaylistLength + 1 + playListLength ) );

		XMP_Uns16 returnValue = GetUns16BE ( &tempValue.c_str()[offsetForTrackNumber] );
		trackNumber = returnValue;
	}
	return found;
}

// =================================================================================================

bool iTunes_Manager::GetDiskNumber( XMP_Uns16 & diskNumber, XMP_Uns16 & totalDiscs ) const
{
	std::string tempValue;
	bool found = GetDataBuffer ( kiTunes_DiskNumber, tempValue );

	if ( found ) {
		XMP_Assert ( tempValue.size() >= 6 );

		XMP_Uns8 reservedLength = ( XMP_Uns8 ) tempValue.c_str()[1];
		XMP_Uns16 offsetForDiskNumber = 2 + reservedLength;

		XMP_Assert ( tempValue.size() >= ( size_t ) ( offsetForDiskNumber + 4 ) );

		XMP_Uns16 returnValue = GetUns16BE ( &tempValue.c_str()[offsetForDiskNumber] );
		diskNumber = returnValue;

		returnValue = GetUns16BE ( &tempValue.c_str()[offsetForDiskNumber + 2] );
		totalDiscs = returnValue;
	}
	return found;

}

// =================================================================================================

bool iTunes_Manager::GetTempo( XMP_Uns16 & tempo ) const
{
	bool found;
	XMP_Int64 tempValue = -1;
	found = GetNumericValue( kiTunes_Tempo, tempValue, 2 );
	if ( found && tempValue >= 0 && tempValue <= 0xFFFF ) {
		tempo = (XMP_Uns16) tempValue;
		return true;
	}
	return false;
}

// =================================================================================================

bool iTunes_Manager::GetCopyright( std::string & copyright ) const
{
	return GetTextValue ( kiTunes_Copyright, copyright );
}

// =================================================================================================

void iTunes_Manager::RemoveName()
{
	DeleteItem ( kiTunes_SongName );
}

// =================================================================================================

void iTunes_Manager::RemoveArtist()
{
	DeleteItem ( kiTunes_Artist );
}

// =================================================================================================

void iTunes_Manager::RemoveAlbumArtist()
{
	DeleteItem ( kiTunes_AlbumArtist );
}

// =================================================================================================

void iTunes_Manager::RemoveAlbum()
{
	DeleteItem ( kiTunes_Album );
}

// =================================================================================================

void iTunes_Manager::RemoveGenre()
{
	DeleteItem ( kiTunes_StdGenres );
	DeleteItem ( kiTunes_UserGenres );
}

// =================================================================================================

void iTunes_Manager::RemoveReleaseDate()
{
	DeleteItem ( kiTunes_ReleaseDate );
}

// =================================================================================================

void iTunes_Manager::RemoveComposer()
{
	DeleteItem ( kiTunes_Composer );
}

// =================================================================================================

void iTunes_Manager::RemoveComments()
{
	DeleteItem ( kiTunes_Comment );
}

// =================================================================================================

void iTunes_Manager::RemoveTrackNumber()
{
	DeleteItem ( kiTunes_TrackNumber );
}

// =================================================================================================

void iTunes_Manager::RemoveDiskNumber()
{
	DeleteItem ( kiTunes_DiskNumber );
}

// =================================================================================================

void iTunes_Manager::RemoveTempo()
{
	DeleteItem ( kiTunes_Tempo );
}

// =================================================================================================

void iTunes_Manager::RemoveCopyright()
{
	DeleteItem ( kiTunes_Copyright );
}

// =================================================================================================

void iTunes_Manager::SetName( const std::string & name )
{
	SetTextValue ( kiTunes_SongName, name, kiTunesData_UTF_8 );
}

// =================================================================================================

void iTunes_Manager::SetArtist( const std::string & artist )
{
	SetTextValue ( kiTunes_Artist, artist, kiTunesData_UTF_8 );
}

// =================================================================================================

void iTunes_Manager::SetAlbumArtist( const std::string albumArtist )
{
	SetTextValue ( kiTunes_AlbumArtist, albumArtist, kiTunesData_UTF_8 );
}

// =================================================================================================

void iTunes_Manager::SetAlbum( const std::string & album )
{
	SetTextValue ( kiTunes_Album, album, kiTunesData_UTF_8 );
}

// =================================================================================================

void iTunes_Manager::SetGenre( const std::string & genre )
{
	XMP_Uns8 i = 0;
	bool found = false;
	for ( i = 0; i < kMaxGenreID; i++ ) {
		if ( genre.compare ( kGenreNames[i] ) == 0 ) {
			found = true;
			break;
		}
	}

	if ( found ) {
		i++;
		std::string dataBuffer;
		dataBuffer = '\0';
		dataBuffer += i;
		XMP_Assert ( dataBuffer.size() == 2 );
		SetDataBuffer ( kiTunes_StdGenres, dataBuffer );
		DeleteItem ( kiTunes_UserGenres );
	} else {
		SetTextValue ( kiTunes_UserGenres, genre, kiTunesData_UTF_8 );
		DeleteItem ( kiTunes_StdGenres );
	}
}

// =================================================================================================

void iTunes_Manager::SetReleaseDate( XMP_DateTime releaseDate )
{
	std::string strValue;
	releaseDate.hasTime = releaseDate.hasTimeZone = false;
	releaseDate.hour = releaseDate.minute = releaseDate.second = releaseDate.nanoSecond = 0;
	releaseDate.tzHour = releaseDate.tzMinute = 0;
	try {
		SXMPUtils::ConvertFromDate ( releaseDate, &strValue );
		if ( strValue.size() > 0 )
			SetTextValue ( kiTunes_ReleaseDate, strValue, kiTunesData_UTF_8 );
	} catch ( ... ) {
		// don't do anything
	}
}

// =================================================================================================

void iTunes_Manager::SetComposer( const std::string composer )
{
	SetTextValue ( kiTunes_Composer, composer, kiTunesData_UTF_8 );
}

// =================================================================================================

void iTunes_Manager::SetComments( const std::string comments )
{
	SetTextValue ( kiTunes_Comment, comments, kiTunesData_UTF_8 );
}

// =================================================================================================

void iTunes_Manager::SetTrackNumber( XMP_Uns16 trackNumber )
{
	std::string dataBuffer;
	bool found = GetDataBuffer ( kiTunes_TrackNumber, dataBuffer );
	if ( !found ) {
		dataBuffer.clear();
		dataBuffer.resize ( 8, '\0' );
	}

	XMP_Assert ( dataBuffer.size() >= 7 );

	XMP_Uns8 reservedLength = ( XMP_Uns8 ) dataBuffer.c_str()[1];
	XMP_Uns16 offsetForTrackNumber = 2 + reservedLength;
	XMP_Uns16 offsetForPlaylistLength = offsetForTrackNumber + 4;

	XMP_Assert ( dataBuffer.size() >= ( size_t ) ( offsetForPlaylistLength + 1 ) );
	XMP_Uns8 playListLength = ( XMP_Uns8 ) dataBuffer.c_str()[offsetForPlaylistLength];

	XMP_Assert ( dataBuffer.size() >= ( size_t ) ( offsetForPlaylistLength + 1 + playListLength ) );

	XMP_Uns8 buffer[2];
	PutUns16BE ( trackNumber, &buffer[0] );

	dataBuffer[offsetForTrackNumber] = buffer[0];
	dataBuffer[offsetForTrackNumber + 1] = buffer[1];

	XMP_Uns16 totalTracks = GetUns16BE ( &dataBuffer.c_str()[offsetForTrackNumber + 2] );
	if ( totalTracks < trackNumber ) {
		dataBuffer[offsetForTrackNumber + 2] = buffer[0];
		dataBuffer[offsetForTrackNumber + 3] = buffer[1];
	}

	SetDataBuffer ( kiTunes_TrackNumber, dataBuffer );
}

// =================================================================================================

void iTunes_Manager::SetDiskNumber( XMP_Uns16 diskNumber, XMP_Uns16 totalDisks )
{
	std::string dataBuffer;
	bool found = GetDataBuffer ( kiTunes_DiskNumber, dataBuffer );
	if ( !found ) {
		dataBuffer.clear();
		dataBuffer.resize ( 8, '\0' );
	}

	XMP_Assert ( dataBuffer.size() >= 6 );

	XMP_Uns8 reservedLength = ( XMP_Uns8 ) dataBuffer.c_str()[1];
	XMP_Uns16 offsetForDiskNumber = 2 + reservedLength;

	XMP_Assert ( dataBuffer.size() >= ( size_t ) ( offsetForDiskNumber + 4 ) );

	char buffer[2];
	PutUns16BE ( diskNumber, &buffer[0] );

	dataBuffer[offsetForDiskNumber] = buffer[0];
	dataBuffer[offsetForDiskNumber + 1] = buffer[1];

	PutUns16BE ( totalDisks, &buffer[0] );

	dataBuffer[offsetForDiskNumber + 2] = buffer[0];
	dataBuffer[offsetForDiskNumber + 3] = buffer[1];

	SetDataBuffer ( kiTunes_DiskNumber, dataBuffer );
}

// =================================================================================================

void iTunes_Manager::SetTempo( XMP_Uns16 tempo )
{
	SetNumericValue ( kiTunes_Tempo, tempo, 2 );
}

// =================================================================================================

void iTunes_Manager::SetCopyright( const std::string & copyright )
{
	SetTextValue ( kiTunes_Copyright, copyright, kiTunesData_UTF_8 );
}


#endif
#endif

// =================================================================================================
