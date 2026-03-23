#include "XMPFilesPlugins/api/source/PluginBase.h"
#include "XMPFilesPlugins/PDF_Handler/third-party/MiniPDFL/Include/ASExpT.h"
#include "XMPFilesPlugins/PDF_Handler/third-party/MiniPDFL/Include/CosExpT.h"
#include <stdio.h>
#include <stdlib.h>
#include <vector>

#if UNIT_TEST

namespace PDF_Handler
{
extern ASAtom ASAtomFromString( const char *nameStr );
extern const char * ASAtomGetString(ASAtom atom);
}

#define XML_NAME 0

#else
#include "XMPFilesPlugins/PDF_Handler/third-party/MiniPDFL/Include/ASCalls.h"
#include "XMPFilesPlugins/PDF_Handler/third-party/MiniPDFL/Include/CorCalls.h"
#include "XMPFilesPlugins/PDF_Handler/third-party/MiniPDFL/Include/ASExtraCalls.h"
#define XML_NAME 1
#endif

namespace PDF_Handler
{


#if XML_NAME
class XMLNameEscaper
{
public:
	XMLNameEscaper(ASAtom sourceASAtom);
	XMLNameEscaper(const std::string & sourceUTF8);
	XMLNameEscaper(const char * sourceUTF8);
	virtual ~XMLNameEscaper();
	std::string GetEscaped();
	std::string GetEscapedContent();
	std::vector<ASUTF16Val> GetUnescaped();
	std::string GetUnescapedUTF8();
	bool IsXMLName();
private:
	XMLNameEscaper();
	typedef void (XMLNameEscaper::*SourceTraverser)(std::vector<ASUTF16Val>::iterator &,
													std::vector<ASUTF16Val> &);
	void NameCharTraverser(std::vector<ASUTF16Val>::iterator & sourceIterator,
						   std::vector<ASUTF16Val> & output);
	void XMLCharTraverser(std::vector<ASUTF16Val>::iterator & sourceIterator,
						  std::vector<ASUTF16Val> & output);
	std::string GetEscapedCommon(SourceTraverser traverser);
	void AcceptUTF8CString(const char * utf8CString);
	void MaybeEscape(ASUTF16Val character,
					 bool (* predicate)(ASUTF16Val character),
					 std::vector<ASUTF16Val> & output);
	std::string ToUTF8String(std::vector<ASUTF16Val> & utf16);
	static void Escape(ASUTF16Val character, std::vector<ASUTF16Val> & output);
	static ASUTF16Val Hex(ASUTF16Val character, ASInt32 nibbleIndex);
	static ASUTF16Val HexValue(ASUTF16Val code, bool & isHexDigit);
	static bool IsHexDigitInternal(ASUTF16Val code);
	static void StoreHex(ASUTF16Val sourceHexDigit,
						 ASInt32 intoWhichNibble,
						 ASUTF16Val & target16Bit);
	static bool IsXMLNameStart(ASUTF16Val code);
	static bool IsXMLNameChar(ASUTF16Val code);
	static bool IsXMLLetter(ASUTF16Val code);
	static bool IsXMLBaseChar(ASUTF16Val code);
	static bool IsXMLIdeographic(ASUTF16Val code);
	static bool IsXMLDigit(ASUTF16Val code);
	static bool IsXMLCombiningChar(ASUTF16Val code);
	static bool IsXMLExtender(ASUTF16Val code);
	static bool IsXMLCharacter(ASUTF16Val code);
	static bool Always(ASUTF16Val code);
	static const ASUTF16Val TheEscapeChar;


	//-----------------------------------------------------------------
	// Data members

	std::vector<ASUTF16Val> sourceUTF16;
};

//**********************************************************************
// Implementation of XMLNameEscaper

XMLNameEscaper::
XMLNameEscaper(ASAtom sourceASAtom)
{
	const char * atomUTF8CString = ASAtomGetString(sourceASAtom);
	this->AcceptUTF8CString(atomUTF8CString);
}

XMLNameEscaper::
XMLNameEscaper(const std::string & sourceUTF8)
{
	const char * sourceUTF8CString = sourceUTF8.c_str();
	this->AcceptUTF8CString(sourceUTF8CString);
}

XMLNameEscaper::
XMLNameEscaper(const char * sourceUTF8)
{
	this->AcceptUTF8CString(sourceUTF8);
}

void
XMLNameEscaper::
AcceptUTF8CString(const char * utf8CString)
{
	const ASUTF16Val * bogus = reinterpret_cast<const ASUTF16Val *>(utf8CString);
	ASText text = ASTextFromUnicode(bogus, kUTF8);
	const ASUTF16Val * utf16Buffer = ASTextGetUnicode(text);
	this->sourceUTF16.clear();
	for (const ASUTF16Val * utf16ptr = utf16Buffer;
		 *utf16ptr != 0;
		 ++utf16ptr) {
		this->sourceUTF16.push_back(*utf16ptr);
	}
	ASTextDestroy(text);
}

XMLNameEscaper::
XMLNameEscaper()
{
}

XMLNameEscaper::
~XMLNameEscaper()
{
	this->sourceUTF16.clear();
}

std::string
XMLNameEscaper::
GetEscaped()
{
	return this->GetEscapedCommon(&XMLNameEscaper::NameCharTraverser);
}

std::string
XMLNameEscaper::
GetEscapedContent()
{
	return this->GetEscapedCommon(&XMLNameEscaper::XMLCharTraverser);
}

void
XMLNameEscaper::
NameCharTraverser(std::vector<ASUTF16Val>::iterator & sourceIterator,
				  std::vector<ASUTF16Val> & output)
{
	ASUTF16Val firstCharacter = *sourceIterator;
	MaybeEscape(firstCharacter, IsXMLNameStart, output);
	++sourceIterator;
	while (sourceIterator != this->sourceUTF16.end()) {
		MaybeEscape(*sourceIterator, IsXMLNameChar, output);
		++sourceIterator;
	}
}

void
XMLNameEscaper::
XMLCharTraverser(std::vector<ASUTF16Val>::iterator & sourceIterator,
				 std::vector<ASUTF16Val> & output)
{
	while (sourceIterator != this->sourceUTF16.end()) {
#if AdobePrivate
		// Dan Rabin Fri Jul 09 11:05:54 2004
		// We had been doing our own escaping here, but Alan Lillich
		// has convinced me that it's better to change XMP Toolkit
		// to do it.  I'm backing out our own escape convention
		// by changing the needs-escaping predicate from "IsXMLCharacter"
		// to Always.
#endif
		MaybeEscape(*sourceIterator, Always, output);
		++sourceIterator;
	}
}

std::string
XMLNameEscaper::
GetEscapedCommon(SourceTraverser traverser)
{
	if (this->sourceUTF16.size() == 0) {
		std::string result;
		return result;
	} else {
		std::vector<ASUTF16Val> output;
		std::vector<ASUTF16Val>::iterator sourceIterator = this->sourceUTF16.begin();
		(this->*traverser)(sourceIterator, output);
		std::string result = ToUTF8String(output);
		return result;
	}
}

std::string
XMLNameEscaper::
GetUnescapedUTF8()
{
	std::vector<ASUTF16Val> text = this->GetUnescaped();
	if (text.empty())
		text.push_back(0);
	std::string result = ToUTF8String(text);
	return result;
}

std::vector<ASUTF16Val>
XMLNameEscaper::
GetUnescaped()
{
	std::vector<ASUTF16Val> output;
	if (this->sourceUTF16.size() == 0) {
		return output;
	} else {
		std::vector<ASUTF16Val>::iterator sourceIterator = this->sourceUTF16.begin();
		std::vector<ASUTF16Val>::iterator endOfString = this->sourceUTF16.end();
	nextCharacter:
		if (sourceIterator == endOfString) {
			goto done;
		}
		if (*sourceIterator != TheEscapeChar) {
			output.push_back(*sourceIterator);
			++sourceIterator;
			goto nextCharacter;
		} else {
			++sourceIterator;
			goto justSawEscapeChar;
		}
	justSawEscapeChar:
		{
			ASUTF16Val builtFromHex = 0;
			for (ASInt32 byteIndex = 3;
				 byteIndex >= 0;
				 --byteIndex) {
				if (sourceIterator == endOfString) {
					goto abort;
				}
				bool isHexDigit;
				ASUTF16Val hexValue = HexValue(*sourceIterator, isHexDigit);
				if (isHexDigit) {
					StoreHex(hexValue, byteIndex, builtFromHex);
					++sourceIterator;
				} else {
					goto abort;
				}
			}
			output.push_back(builtFromHex);
			goto nextCharacter;
		}
	abort:
		// Couldn't be interpreted as an escaped string, so we just return
		// the string as given.
		return this->sourceUTF16;
	done:
		return output;
	}
}

bool
XMLNameEscaper::
IsHexDigitInternal(ASUTF16Val code)
{
	return ((0x0030 <= code) && (code <= 0x0039)) ||
		((0x0041 <= code) && (code <= 0x0046)) ||
		((0x0061 <= code) && (code <= 0x0066));
}

void
XMLNameEscaper::
StoreHex(ASUTF16Val hexValue,
		 ASInt32 intoWhichNibble,
		 ASUTF16Val & target16Bit)
{
	ASInt32 shift = 4 * intoWhichNibble;
	ASUTF16Val mask = 0x000F << shift;
	ASUTF16Val shiftedValue = hexValue << shift;
	target16Bit &= ~mask;
	target16Bit |= shiftedValue;
}

std::string
XMLNameEscaper::
ToUTF8String(std::vector<ASUTF16Val> & utf16)
{
	ASText text = ASTextFromSizedUnicode(&utf16[0],
										 kUTF16HostEndian,
										 // Pass length in bytes, not elements.
										 ASInt32( 2 * utf16.size() ) );
	ASUTF16Val * bogus = ASTextGetUnicodeCopy(text, kUTF8);
	char * utf8CString = reinterpret_cast<char *>(bogus);
	std::string result(utf8CString);
	ASTextDestroy(text);
	ASfree(utf8CString);
	return result;
}

const ASUTF16Val
XMLNameEscaper::
TheEscapeChar = 0x2182; // ROMAN NUMERAL TEN THOUSAND

void
XMLNameEscaper::
MaybeEscape(ASUTF16Val character,
			bool (* predicate)(ASUTF16Val character),
			std::vector<ASUTF16Val> & output)
{
	if (character == TheEscapeChar) {
		Escape(character, output);
	} else if ((* predicate)(character)) {
		output.push_back(character);
	} else {
		Escape(character, output);
	}
}

void
XMLNameEscaper::
Escape(ASUTF16Val character, std::vector<ASUTF16Val> & output)
{
	output.push_back(TheEscapeChar);
	for (ASInt32 nibbleIndex = 3;
		 nibbleIndex >= 0;
		 --nibbleIndex) {
		output.push_back(Hex(character, nibbleIndex));
	}
}

ASUTF16Val
XMLNameEscaper::
Hex(ASUTF16Val character, ASInt32 nibbleIndex)
{
	ASUTF16Val hexValue = (character >> (4 * nibbleIndex)) & 0x0000000F;
	if (hexValue < 10) {
		return 0x0030 + hexValue;
	} else if (hexValue < 16) {
		return 0x0041 + hexValue - 10;
	}
	return 0x0000;
}

ASUTF16Val
XMLNameEscaper::
HexValue(ASUTF16Val code, bool & isHexDigit)
{
	if ((0x0030 <= code) && (code <= 0x0039)) {
		isHexDigit = true;
		return code - 0x0030;
	} else if ((0x0041 <= code) && (code <= 0x0046)) {
		isHexDigit = true;
		return code - 0x0041 + 10;
	} else if ((0x0061 <= code) && (code <= 0x0066)) {
		isHexDigit = true;
		return code - 0x0061 + 10;
	} else {
		isHexDigit = false;
		return 0;
	}
}

//----------------------------------------------------------------------
// The following functions for checking that a character code represents
// an XML Name character are composed of code generated by a program
// based on data cut-and-pasted from the XML 1.0 Recommendation,
// Second Edition.

bool
XMLNameEscaper::
IsXMLNameStart(ASUTF16Val code)
{
	return IsXMLLetter(code) ||
		code == 0x005F;  // Underscore
		// Dan Rabin Thu Jun 10 09:35:50 2004
		// To fix bug #0607898, don't ever treat colon as a character that
		// can pass unescaped.  The reason is that we're always passing the
		// escaped results to XMP toolkit calls that treat colon-delimited
		// prefixes as schema prefixes.  Since the Info dict key could not
		// have been devised with such an interpretation in mind, we want to
		// escape it.
		// Usually, the symptom of passing such a key is a thrown exception from
		// the XMP toolkit resulting from the spurious schema name not being
		// recognized.
		// Actually, since the existing code would succeed if the schema name
		// happened to be legitimate, and since an older bug (before the introduction
		// of StripQualifier above) could put legitimate schema-prefixed keys into the
		// Info dictionary, the totally correct thing to do would be to
		// do a trial probe of the colon-unescaped version with the XMP toolkit.  If the
		// toolkit recognizes the schema name, use the unescaped version.  Otherwise, try
		// again with a version in which the colons are escaped with the five-character
		// convention implemented here.
		// I'm going to choose not to be totally correct right now.
		// code == 0x003A;   // Colon
}

bool
XMLNameEscaper::
IsXMLNameChar(ASUTF16Val code)
{
	return IsXMLNameStart(code) ||
		IsXMLDigit(code) ||
		code == 0x002D ||  // Hyphen
		code == 0x002E ||  // Full Stop/Period
		IsXMLCombiningChar(code) ||
		IsXMLExtender(code);
}

bool
XMLNameEscaper::
IsXMLLetter(ASUTF16Val code)
{
	return IsXMLBaseChar(code) || IsXMLIdeographic(code);
}

bool
XMLNameEscaper::
IsXMLBaseChar(ASUTF16Val code)
{
	return ((code >= 0x0041) && (code <= 0x005A)) ||
		((code >= 0x0061) && (code <= 0x007A)) ||
		((code >= 0x00C0) && (code <= 0x00D6)) ||
		((code >= 0x00D8) && (code <= 0x00F6)) ||
		((code >= 0x00F8) && (code <= 0x00FF)) ||
		((code >= 0x0100) && (code <= 0x0131)) ||
		((code >= 0x0134) && (code <= 0x013E)) ||
		((code >= 0x0141) && (code <= 0x0148)) ||
		((code >= 0x014A) && (code <= 0x017E)) ||
		((code >= 0x0180) && (code <= 0x01C3)) ||
		((code >= 0x01CD) && (code <= 0x01F0)) ||
		((code >= 0x01F4) && (code <= 0x01F5)) ||
		((code >= 0x01FA) && (code <= 0x0217)) ||
		((code >= 0x0250) && (code <= 0x02A8)) ||
		((code >= 0x02BB) && (code <= 0x02C1)) ||
		(code == 0x0386) ||
		((code >= 0x0388) && (code <= 0x038A)) ||
		(code == 0x038C) ||
		((code >= 0x038E) && (code <= 0x03A1)) ||
		((code >= 0x03A3) && (code <= 0x03CE)) ||
		((code >= 0x03D0) && (code <= 0x03D6)) ||
		(code == 0x03DA) ||
		(code == 0x03DC) ||
		(code == 0x03DE) ||
		(code == 0x03E0) ||
		((code >= 0x03E2) && (code <= 0x03F3)) ||
		((code >= 0x0401) && (code <= 0x040C)) ||
		((code >= 0x040E) && (code <= 0x044F)) ||
		((code >= 0x0451) && (code <= 0x045C)) ||
		((code >= 0x045E) && (code <= 0x0481)) ||
		((code >= 0x0490) && (code <= 0x04C4)) ||
		((code >= 0x04C7) && (code <= 0x04C8)) ||
		((code >= 0x04CB) && (code <= 0x04CC)) ||
		((code >= 0x04D0) && (code <= 0x04EB)) ||
		((code >= 0x04EE) && (code <= 0x04F5)) ||
		((code >= 0x04F8) && (code <= 0x04F9)) ||
		((code >= 0x0531) && (code <= 0x0556)) ||
		(code == 0x0559) ||
		((code >= 0x0561) && (code <= 0x0586)) ||
		((code >= 0x05D0) && (code <= 0x05EA)) ||
		((code >= 0x05F0) && (code <= 0x05F2)) ||
		((code >= 0x0621) && (code <= 0x063A)) ||
		((code >= 0x0641) && (code <= 0x064A)) ||
		((code >= 0x0671) && (code <= 0x06B7)) ||
		((code >= 0x06BA) && (code <= 0x06BE)) ||
		((code >= 0x06C0) && (code <= 0x06CE)) ||
		((code >= 0x06D0) && (code <= 0x06D3)) ||
		(code == 0x06D5) ||
		((code >= 0x06E5) && (code <= 0x06E6)) ||
		((code >= 0x0905) && (code <= 0x0939)) ||
		(code == 0x093D) ||
		((code >= 0x0958) && (code <= 0x0961)) ||
		((code >= 0x0985) && (code <= 0x098C)) ||
		((code >= 0x098F) && (code <= 0x0990)) ||
		((code >= 0x0993) && (code <= 0x09A8)) ||
		((code >= 0x09AA) && (code <= 0x09B0)) ||
		(code == 0x09B2) ||
		((code >= 0x09B6) && (code <= 0x09B9)) ||
		((code >= 0x09DC) && (code <= 0x09DD)) ||
		((code >= 0x09DF) && (code <= 0x09E1)) ||
		((code >= 0x09F0) && (code <= 0x09F1)) ||
		((code >= 0x0A05) && (code <= 0x0A0A)) ||
		((code >= 0x0A0F) && (code <= 0x0A10)) ||
		((code >= 0x0A13) && (code <= 0x0A28)) ||
		((code >= 0x0A2A) && (code <= 0x0A30)) ||
		((code >= 0x0A32) && (code <= 0x0A33)) ||
		((code >= 0x0A35) && (code <= 0x0A36)) ||
		((code >= 0x0A38) && (code <= 0x0A39)) ||
		((code >= 0x0A59) && (code <= 0x0A5C)) ||
		(code == 0x0A5E) ||
		((code >= 0x0A72) && (code <= 0x0A74)) ||
		((code >= 0x0A85) && (code <= 0x0A8B)) ||
		(code == 0x0A8D) ||
		((code >= 0x0A8F) && (code <= 0x0A91)) ||
		((code >= 0x0A93) && (code <= 0x0AA8)) ||
		((code >= 0x0AAA) && (code <= 0x0AB0)) ||
		((code >= 0x0AB2) && (code <= 0x0AB3)) ||
		((code >= 0x0AB5) && (code <= 0x0AB9)) ||
		(code == 0x0ABD) ||
		(code == 0x0AE0) ||
		((code >= 0x0B05) && (code <= 0x0B0C)) ||
		((code >= 0x0B0F) && (code <= 0x0B10)) ||
		((code >= 0x0B13) && (code <= 0x0B28)) ||
		((code >= 0x0B2A) && (code <= 0x0B30)) ||
		((code >= 0x0B32) && (code <= 0x0B33)) ||
		((code >= 0x0B36) && (code <= 0x0B39)) ||
		(code == 0x0B3D) ||
		((code >= 0x0B5C) && (code <= 0x0B5D)) ||
		((code >= 0x0B5F) && (code <= 0x0B61)) ||
		((code >= 0x0B85) && (code <= 0x0B8A)) ||
		((code >= 0x0B8E) && (code <= 0x0B90)) ||
		((code >= 0x0B92) && (code <= 0x0B95)) ||
		((code >= 0x0B99) && (code <= 0x0B9A)) ||
		(code == 0x0B9C) ||
		((code >= 0x0B9E) && (code <= 0x0B9F)) ||
		((code >= 0x0BA3) && (code <= 0x0BA4)) ||
		((code >= 0x0BA8) && (code <= 0x0BAA)) ||
		((code >= 0x0BAE) && (code <= 0x0BB5)) ||
		((code >= 0x0BB7) && (code <= 0x0BB9)) ||
		((code >= 0x0C05) && (code <= 0x0C0C)) ||
		((code >= 0x0C0E) && (code <= 0x0C10)) ||
		((code >= 0x0C12) && (code <= 0x0C28)) ||
		((code >= 0x0C2A) && (code <= 0x0C33)) ||
		((code >= 0x0C35) && (code <= 0x0C39)) ||
		((code >= 0x0C60) && (code <= 0x0C61)) ||
		((code >= 0x0C85) && (code <= 0x0C8C)) ||
		((code >= 0x0C8E) && (code <= 0x0C90)) ||
		((code >= 0x0C92) && (code <= 0x0CA8)) ||
		((code >= 0x0CAA) && (code <= 0x0CB3)) ||
		((code >= 0x0CB5) && (code <= 0x0CB9)) ||
		(code == 0x0CDE) ||
		((code >= 0x0CE0) && (code <= 0x0CE1)) ||
		((code >= 0x0D05) && (code <= 0x0D0C)) ||
		((code >= 0x0D0E) && (code <= 0x0D10)) ||
		((code >= 0x0D12) && (code <= 0x0D28)) ||
		((code >= 0x0D2A) && (code <= 0x0D39)) ||
		((code >= 0x0D60) && (code <= 0x0D61)) ||
		((code >= 0x0E01) && (code <= 0x0E2E)) ||
		(code == 0x0E30) ||
		((code >= 0x0E32) && (code <= 0x0E33)) ||
		((code >= 0x0E40) && (code <= 0x0E45)) ||
		((code >= 0x0E81) && (code <= 0x0E82)) ||
		(code == 0x0E84) ||
		((code >= 0x0E87) && (code <= 0x0E88)) ||
		(code == 0x0E8A) ||
		(code == 0x0E8D) ||
		((code >= 0x0E94) && (code <= 0x0E97)) ||
		((code >= 0x0E99) && (code <= 0x0E9F)) ||
		((code >= 0x0EA1) && (code <= 0x0EA3)) ||
		(code == 0x0EA5) ||
		(code == 0x0EA7) ||
		((code >= 0x0EAA) && (code <= 0x0EAB)) ||
		((code >= 0x0EAD) && (code <= 0x0EAE)) ||
		(code == 0x0EB0) ||
		((code >= 0x0EB2) && (code <= 0x0EB3)) ||
		(code == 0x0EBD) ||
		((code >= 0x0EC0) && (code <= 0x0EC4)) ||
		((code >= 0x0F40) && (code <= 0x0F47)) ||
		((code >= 0x0F49) && (code <= 0x0F69)) ||
		((code >= 0x10A0) && (code <= 0x10C5)) ||
		((code >= 0x10D0) && (code <= 0x10F6)) ||
		(code == 0x1100) ||
		((code >= 0x1102) && (code <= 0x1103)) ||
		((code >= 0x1105) && (code <= 0x1107)) ||
		(code == 0x1109) ||
		((code >= 0x110B) && (code <= 0x110C)) ||
		((code >= 0x110E) && (code <= 0x1112)) ||
		(code == 0x113C) ||
		(code == 0x113E) ||
		(code == 0x1140) ||
		(code == 0x114C) ||
		(code == 0x114E) ||
		(code == 0x1150) ||
		((code >= 0x1154) && (code <= 0x1155)) ||
		(code == 0x1159) ||
		((code >= 0x115F) && (code <= 0x1161)) ||
		(code == 0x1163) ||
		(code == 0x1165) ||
		(code == 0x1167) ||
		(code == 0x1169) ||
		((code >= 0x116D) && (code <= 0x116E)) ||
		((code >= 0x1172) && (code <= 0x1173)) ||
		(code == 0x1175) ||
		(code == 0x119E) ||
		(code == 0x11A8) ||
		(code == 0x11AB) ||
		((code >= 0x11AE) && (code <= 0x11AF)) ||
		((code >= 0x11B7) && (code <= 0x11B8)) ||
		(code == 0x11BA) ||
		((code >= 0x11BC) && (code <= 0x11C2)) ||
		(code == 0x11EB) ||
		(code == 0x11F0) ||
		(code == 0x11F9) ||
		((code >= 0x1E00) && (code <= 0x1E9B)) ||
		((code >= 0x1EA0) && (code <= 0x1EF9)) ||
		((code >= 0x1F00) && (code <= 0x1F15)) ||
		((code >= 0x1F18) && (code <= 0x1F1D)) ||
		((code >= 0x1F20) && (code <= 0x1F45)) ||
		((code >= 0x1F48) && (code <= 0x1F4D)) ||
		((code >= 0x1F50) && (code <= 0x1F57)) ||
		(code == 0x1F59) ||
		(code == 0x1F5B) ||
		(code == 0x1F5D) ||
		((code >= 0x1F5F) && (code <= 0x1F7D)) ||
		((code >= 0x1F80) && (code <= 0x1FB4)) ||
		((code >= 0x1FB6) && (code <= 0x1FBC)) ||
		(code == 0x1FBE) ||
		((code >= 0x1FC2) && (code <= 0x1FC4)) ||
		((code >= 0x1FC6) && (code <= 0x1FCC)) ||
		((code >= 0x1FD0) && (code <= 0x1FD3)) ||
		((code >= 0x1FD6) && (code <= 0x1FDB)) ||
		((code >= 0x1FE0) && (code <= 0x1FEC)) ||
		((code >= 0x1FF2) && (code <= 0x1FF4)) ||
		((code >= 0x1FF6) && (code <= 0x1FFC)) ||
		(code == 0x2126) ||
		((code >= 0x212A) && (code <= 0x212B)) ||
		(code == 0x212E) ||
		((code >= 0x2180) && (code <= 0x2182)) ||
		((code >= 0x3041) && (code <= 0x3094)) ||
		((code >= 0x30A1) && (code <= 0x30FA)) ||
		((code >= 0x3105) && (code <= 0x312C)) ||
		((code >= 0xAC00) && (code <= 0xD7A3));
}

bool
XMLNameEscaper::
IsXMLIdeographic(ASUTF16Val code)
{
	return ((code >= 0x4E00) && (code <= 0x9FA5)) ||
		(code == 0x3007) ||
		((code >= 0x3021) && (code <= 0x3029));
}

bool
XMLNameEscaper::
IsXMLDigit(ASUTF16Val code)
{
	return ((code >= 0x0030) && (code <= 0x0039)) ||
		((code >= 0x0660) && (code <= 0x0669)) ||
		((code >= 0x06F0) && (code <= 0x06F9)) ||
		((code >= 0x0966) && (code <= 0x096F)) ||
		((code >= 0x09E6) && (code <= 0x09EF)) ||
		((code >= 0x0A66) && (code <= 0x0A6F)) ||
		((code >= 0x0AE6) && (code <= 0x0AEF)) ||
		((code >= 0x0B66) && (code <= 0x0B6F)) ||
		((code >= 0x0BE7) && (code <= 0x0BEF)) ||
		((code >= 0x0C66) && (code <= 0x0C6F)) ||
		((code >= 0x0CE6) && (code <= 0x0CEF)) ||
		((code >= 0x0D66) && (code <= 0x0D6F)) ||
		((code >= 0x0E50) && (code <= 0x0E59)) ||
		((code >= 0x0ED0) && (code <= 0x0ED9)) ||
		((code >= 0x0F20) && (code <= 0x0F29));
}

bool XMLNameEscaper::
IsXMLCombiningChar(ASUTF16Val code)
{
	return ((code >= 0x0300) && (code <= 0x0345)) ||
		((code >= 0x0360) && (code <= 0x0361)) ||
		((code >= 0x0483) && (code <= 0x0486)) ||
		((code >= 0x0591) && (code <= 0x05A1)) ||
		((code >= 0x05A3) && (code <= 0x05B9)) ||
		((code >= 0x05BB) && (code <= 0x05BD)) ||
		(code == 0x05BF) ||
		((code >= 0x05C1) && (code <= 0x05C2)) ||
		(code == 0x05C4) ||
		((code >= 0x064B) && (code <= 0x0652)) ||
		(code == 0x0670) ||
		((code >= 0x06D6) && (code <= 0x06DC)) ||
		((code >= 0x06DD) && (code <= 0x06DF)) ||
		((code >= 0x06E0) && (code <= 0x06E4)) ||
		((code >= 0x06E7) && (code <= 0x06E8)) ||
		((code >= 0x06EA) && (code <= 0x06ED)) ||
		((code >= 0x0901) && (code <= 0x0903)) ||
		(code == 0x093C) ||
		((code >= 0x093E) && (code <= 0x094C)) ||
		(code == 0x094D) ||
		((code >= 0x0951) && (code <= 0x0954)) ||
		((code >= 0x0962) && (code <= 0x0963)) ||
		((code >= 0x0981) && (code <= 0x0983)) ||
		(code == 0x09BC) ||
		(code == 0x09BE) ||
		(code == 0x09BF) ||
		((code >= 0x09C0) && (code <= 0x09C4)) ||
		((code >= 0x09C7) && (code <= 0x09C8)) ||
		((code >= 0x09CB) && (code <= 0x09CD)) ||
		(code == 0x09D7) ||
		((code >= 0x09E2) && (code <= 0x09E3)) ||
		(code == 0x0A02) ||
		(code == 0x0A3C) ||
		(code == 0x0A3E) ||
		(code == 0x0A3F) ||
		((code >= 0x0A40) && (code <= 0x0A42)) ||
		((code >= 0x0A47) && (code <= 0x0A48)) ||
		((code >= 0x0A4B) && (code <= 0x0A4D)) ||
		((code >= 0x0A70) && (code <= 0x0A71)) ||
		((code >= 0x0A81) && (code <= 0x0A83)) ||
		(code == 0x0ABC) ||
		((code >= 0x0ABE) && (code <= 0x0AC5)) ||
		((code >= 0x0AC7) && (code <= 0x0AC9)) ||
		((code >= 0x0ACB) && (code <= 0x0ACD)) ||
		((code >= 0x0B01) && (code <= 0x0B03)) ||
		(code == 0x0B3C) ||
		((code >= 0x0B3E) && (code <= 0x0B43)) ||
		((code >= 0x0B47) && (code <= 0x0B48)) ||
		((code >= 0x0B4B) && (code <= 0x0B4D)) ||
		((code >= 0x0B56) && (code <= 0x0B57)) ||
		((code >= 0x0B82) && (code <= 0x0B83)) ||
		((code >= 0x0BBE) && (code <= 0x0BC2)) ||
		((code >= 0x0BC6) && (code <= 0x0BC8)) ||
		((code >= 0x0BCA) && (code <= 0x0BCD)) ||
		(code == 0x0BD7) ||
		((code >= 0x0C01) && (code <= 0x0C03)) ||
		((code >= 0x0C3E) && (code <= 0x0C44)) ||
		((code >= 0x0C46) && (code <= 0x0C48)) ||
		((code >= 0x0C4A) && (code <= 0x0C4D)) ||
		((code >= 0x0C55) && (code <= 0x0C56)) ||
		((code >= 0x0C82) && (code <= 0x0C83)) ||
		((code >= 0x0CBE) && (code <= 0x0CC4)) ||
		((code >= 0x0CC6) && (code <= 0x0CC8)) ||
		((code >= 0x0CCA) && (code <= 0x0CCD)) ||
		((code >= 0x0CD5) && (code <= 0x0CD6)) ||
		((code >= 0x0D02) && (code <= 0x0D03)) ||
		((code >= 0x0D3E) && (code <= 0x0D43)) ||
		((code >= 0x0D46) && (code <= 0x0D48)) ||
		((code >= 0x0D4A) && (code <= 0x0D4D)) ||
		(code == 0x0D57) ||
		(code == 0x0E31) ||
		((code >= 0x0E34) && (code <= 0x0E3A)) ||
		((code >= 0x0E47) && (code <= 0x0E4E)) ||
		(code == 0x0EB1) ||
		((code >= 0x0EB4) && (code <= 0x0EB9)) ||
		((code >= 0x0EBB) && (code <= 0x0EBC)) ||
		((code >= 0x0EC8) && (code <= 0x0ECD)) ||
		((code >= 0x0F18) && (code <= 0x0F19)) ||
		(code == 0x0F35) ||
		(code == 0x0F37) ||
		(code == 0x0F39) ||
		(code == 0x0F3E) ||
		(code == 0x0F3F) ||
		((code >= 0x0F71) && (code <= 0x0F84)) ||
		((code >= 0x0F86) && (code <= 0x0F8B)) ||
		((code >= 0x0F90) && (code <= 0x0F95)) ||
		(code == 0x0F97) ||
		((code >= 0x0F99) && (code <= 0x0FAD)) ||
		((code >= 0x0FB1) && (code <= 0x0FB7)) ||
		(code == 0x0FB9) ||
		((code >= 0x20D0) && (code <= 0x20DC)) ||
		(code == 0x20E1) ||
		((code >= 0x302A) && (code <= 0x302F)) ||
		(code == 0x3099) ||
		(code == 0x309A);
}

bool
XMLNameEscaper::
IsXMLExtender(ASUTF16Val code)
{
	return (code == 0x00B7) ||
		(code == 0x02D0) ||
		(code == 0x02D1) ||
		(code == 0x0387) ||
		(code == 0x0640) ||
		(code == 0x0E46) ||
		(code == 0x0EC6) ||
		(code == 0x3005) ||
		((code >= 0x3031) && (code <= 0x3035)) ||
		((code >= 0x309D) && (code <= 0x309E)) ||
		((code >= 0x30FC) && (code <= 0x30FE));
}

bool
XMLNameEscaper::
IsXMLCharacter(ASUTF16Val code)
{
	return ((code >= 0x0020) && (code <= 0xFFFD)) ||
		(code == 0x0009) ||
		(code == 0x000A) ||
		(code == 0x000D);
}

bool
XMLNameEscaper::
Always(ASUTF16Val code)
{
	return true;
}

#endif

extern void SetTextPropertyInInfo(CosDoc cosDoc, ASAtom key, std::string & value);
extern void GetUTF8FromFromPDString( const std::string& value, std::string& utf8Value );

static const char *const PDDATE_STRING_PREFIX   =   "D:";
static const char *const kXMP_NS_PDFE           =   "http://www.aiim.org/pdfe/ns/id/";
static const char *const kXAP_NS_PDFX           =   "http://ns.adobe.com/pdfx/1.3/";
static const char *const kXMP_NS_PDFX_ID_prefix =   "pdfxid";
static const char *const kXMP_NS_PDFE_prefix    =   "pdfe";
static const char *const kXAP_NS_PDFX_prefix    =   "pdfx";
static const char *const kXMP_NS_PDFA_ID_prefix =   "pdfaid";

void XAPDateTimeToASTimeRec(XMP_DateTime & xapDateTime, ASTimeRec & asTimeRec)
{
	asTimeRec.year =  static_cast<ASInt16>(xapDateTime.year);
    asTimeRec.month = static_cast<ASInt16>(xapDateTime.month);
    asTimeRec.date = static_cast<ASInt16>(xapDateTime.day);
    asTimeRec.hour = static_cast<ASInt16>(xapDateTime.hour);
    asTimeRec.minute = static_cast<ASInt16>(xapDateTime.minute);
    asTimeRec.second = static_cast<ASInt16>(xapDateTime.second);
    asTimeRec.millisecond = 0;

    if (xapDateTime.tzSign >= 0) 
	{
        asTimeRec.gmtOffset = static_cast<ASInt16>( (2 * xapDateTime.tzHour + xapDateTime.tzMinute / 30) );
    }
	else 
	{
        asTimeRec.gmtOffset = static_cast<ASInt16>(- (2 * xapDateTime.tzHour + xapDateTime.tzMinute / 30) );
    }
}

void PDTimeRecToString(const ASTimeRecP time, char *buf, Int32 bufSize)
{
	char s1[32],*s;
	/* RTC: Increased size of diff as "+16166'30'" was causing an overflow */
	char diff[11];

	/* diff format is one of "Z" or "" or "-08'30'" */
	if( time->gmtOffset == 0 ) {
		strcpy( diff, "Z" );
	} else if( time->gmtOffset > -48 ) {	/* gmt offset has valid value */
		bool bMinus = (time->gmtOffset < 0) ? true : false;
		int mDiff = ( time->gmtOffset * 30 ) % 60;
		int hDiff = time->gmtOffset / 2;
		if( bMinus ) {
			hDiff = 0 - hDiff;
			mDiff = 0 - mDiff;
		}
		sprintf( diff, "%c%02d\'%02d\'", bMinus ? '-' : '+', hDiff, mDiff );
	} else {	/* gmt offset unknown */
		diff[0] = '\0';
	}

	/* D:YYYYMMDDHHMMSSGG */
	s = (bufSize < 28) ? s1 : buf;
	/*** XXX no checks on valid time ***/
	sprintf( s, "%s%04d%02d%02d%02d%02d%02d", PDDATE_STRING_PREFIX,
		time->year, time->month, time->date, time->hour,
		time->minute, time->second);
	if( diff[0] != '\0' )
		strncat( s, diff, 11 );
	if (bufSize < 28) {
		strncpy(buf, s, bufSize-1);
		buf[bufSize-1] = '\0';
	}
}

void XAPDateTimeToPDFDate(std::string & pdfDate, XMP_DateTime & xapDateTime)
{
    ASTimeRec timeRec;
    XAPDateTimeToASTimeRec(xapDateTime, timeRec);
    char buffer[100];
    PDTimeRecToString(&timeRec, buffer, 100);
    std::string result(buffer);
    pdfDate = result;
}

ASBool PDStringToTimeRec(const char *buf, ASTimeRecP timeRec)
{
	const char	*p, *lastp;
	char 	str[8];
	size_t	prefixLen;
	ASBool bMinus;

	memset(timeRec, 0, sizeof(ASTimeRec));
	timeRec->month = timeRec->date = 1;

	p = buf;
	lastp = p + strlen(buf);				/* first byte beyond end of string */
	prefixLen = strlen(PDDATE_STRING_PREFIX);
	if (!strncmp(p, PDDATE_STRING_PREFIX, prefixLen))		/* optional */
		p += prefixLen;

	/* Pick up the elements one by one.  If the string is not long enough,
	 * values other than year are set to default.
	 */
	if (p+4 > lastp)
		return false;
	str[0] = *p++; if (!isdigit(str[0])) return false;
	str[1] = *p++; if (!isdigit(str[1])) return false;
	str[2] = *p++; if (!isdigit(str[2])) return false;
	str[3] = *p++; if (!isdigit(str[3])) return false;
	str[4] = 0;
	timeRec->year = (ASInt16) atoi(str);

	if (p+2 > lastp)
		return true;
	str[0] = *p++; if (!isdigit(str[0])) return false;
	str[1] = *p++; if (!isdigit(str[1])) return false;
	str[2] = 0;
	timeRec->month = (ASInt16) atoi(str);
	if (timeRec->month > 12) return false;

	if (p+2 > lastp)
		return true;
	str[0] = *p++; if (!isdigit(str[0])) return false;
	str[1] = *p++; if (!isdigit(str[1])) return false;
	timeRec->date = (ASInt16) atoi(str);
	if (timeRec->date == 0 || timeRec->date > 31) return false;

	if (p+2 > lastp)
		return true;
	str[0] = *p++; if (!isdigit(str[0])) return false;
	str[1] = *p++; if (!isdigit(str[1])) return false;
	timeRec->hour = (ASInt16) atoi(str);
	if (timeRec->hour > 59) return false;

	if (p+2 > lastp)
		return true;
	str[0] = *p++; if (!isdigit(str[0])) return false;
	str[1] = *p++; if (!isdigit(str[1])) return false;
	timeRec->minute = (ASInt16) atoi(str);
	if (timeRec->minute > 59) return false;

	if (p+2 > lastp)
		return true;
	str[0] = *p++; if (!isdigit(str[0])) return false;
	str[1] = *p++; if (!isdigit(str[1])) return false;
	timeRec->second = (ASInt16) atoi(str);
	if (timeRec->second > 59) return false;

	/* From here on is new, to parse GMT offsets */
	if( *p == 'Z' ) { timeRec->gmtOffset = 0; return true; }
	if (p+4 > lastp)
	{
		timeRec->gmtOffset = -48;
		return true;
	}

	str[0] = *p++;
	if( str[0] == '-' ) bMinus = true;
	else if( str[0] == '+' ) bMinus = false;
	else return false;
	str[0] = *p++; if (!isdigit(str[0])) return false;
	str[1] = *p++; if (!isdigit(str[1])) return false;
	timeRec->gmtOffset = (ASInt16)  (2 * atoi(str));
	if (timeRec->gmtOffset > 48) return false;
	if( *p++ != '\'' ) return false;

	if (p+3 > lastp)
		return true;
	str[0] = *p++; if (!isdigit(str[0])) return false;
	str[1] = *p++; if (!isdigit(str[1])) return false;
	timeRec->gmtOffset += atoi(str)/30;	/* measured in 30 minute chunks */
	if (timeRec->gmtOffset >= 48) return false;
	if( *p++ != '\'' ) return false;
	if( bMinus ) timeRec->gmtOffset = (ASInt16)  (0 - timeRec->gmtOffset);

	return true;
}

void ASTimeRecToXAPDateTime(const ASTimeRec & timeRec, XMP_DateTime & xapDateTime)
{
    xapDateTime.year = timeRec.year;
    xapDateTime.month = timeRec.month;
    xapDateTime.day = timeRec.date;
    xapDateTime.hour = timeRec.hour;
    xapDateTime.minute = timeRec.minute;
    xapDateTime.second = timeRec.second;
    xapDateTime.nanoSecond = timeRec.millisecond * 1000;
    xapDateTime.hasDate = true;
    xapDateTime.hasTime = true;
    // Half hours to hours.  Watch out for the implementation dependency of
    // "/" for negative arguments.
	
	// Dan Rabin Wed Mar 24 13:16:32 2004
	// Since XMP 3.0, time-zone offset sign has been separately encoded.
	if (timeRec.gmtOffset == -48) { // defined to be "unknown"
		xapDateTime.tzHour = 0; // set to zero?
		xapDateTime.tzMinute = 0;
		xapDateTime.tzSign = 0;
		xapDateTime.hasTimeZone = false;
	} else if (timeRec.gmtOffset > 0) {
		xapDateTime.tzHour = timeRec.gmtOffset / 2; // Integer division
		xapDateTime.tzMinute = (timeRec.gmtOffset - 2 * xapDateTime.tzHour) * 30;
		xapDateTime.tzSign = 1;
		xapDateTime.hasTimeZone = true;
	} else if (timeRec.gmtOffset == 0) {
		xapDateTime.tzHour = 0;
		xapDateTime.tzMinute = 0;
		xapDateTime.tzSign = 0;
		xapDateTime.hasTimeZone = true;
	} else {
		xapDateTime.tzHour = (- timeRec.gmtOffset) / 2; // Integer division
		xapDateTime.tzMinute = ((- timeRec.gmtOffset) - 2 * xapDateTime.tzHour) * 30;
		xapDateTime.tzSign = -1;
		xapDateTime.hasTimeZone = true;
	}
}

ASBool PDFDateToXAPDateTime(const char *buf, XMP_DateTime & xapDateTime)
{
    ASTimeRec timeRec;
	if( PDStringToTimeRec(buf, &timeRec) )
	{
		ASTimeRecToXAPDateTime(timeRec, xapDateTime);
		return true;
	}
	return false;
}



static inline ASBool CheckOptionBit(XMP_OptionBits options) 
{ 
	return !XMP_NodeIsSchema(options) && !XMP_PropIsQualifier(options) && XMP_PropIsSimple(options) ;
}


ASAtom GetInfoKeyForXAPPath(const std::string& path)
{
#if XML_NAME
	XMLNameEscaper escaper(path);
	std::string unescaped(escaper.GetUnescapedUTF8());
	const char* ptr = strchr(unescaped.c_str(), ':');
#else
	const char* ptr = strchr(path.c_str(), ':');
#endif

	if(!ptr)
		ptr = path.c_str();
	else
		ptr++;
	return ASAtomFromString(ptr);
}

bool IsDateProperty(ASAtom nameAtom)
{
    return
        nameAtom == ASAtomFromString("ModDate")
        || nameAtom == ASAtomFromString("CreationDate");
}

std::string GetTextDatePropertyName(const char * key)
{
	std::string keyString(key);
	if (keyString == "CreationDate") {
		return "CreationDate--Text";
	} else if (keyString == "ModDate") {
		return "ModDate--Text";
	} else {
		return "";
	}
}

void CopyPropertyFromXAPToInfo( const SXMPMeta& metaXAP, CosDoc cosDoc,
                          const std::string& nmspace,
                          const std::string& path)
{
    std::string value;
    XMP_OptionBits features;
	
	ASAtom infoKey = GetInfoKeyForXAPPath(path);
	// It is possible for a date to be present both as a well-formed date field
	// in the official XMP property (such as pdf:CreationDate) and also in the
	// special property (such as pdf:CreationDate--Text) that is used to store
	// values from the Doc Info dictionary that couldn't be parsed correctly as
	// dates.
	// We always preferentially choose the "--Text" version, if it exists,
	// to copy back to the Doc Info property, leaving the well-formed date to
	// convey the information from the XMP side to clients that know to look for
	// it.  This choice keeps the round trip from Info to XMP back to Info in line
	// with the expectations of legacy clients.
		
	if (IsDateProperty(infoKey)) 
	{
		std::string infoString = GetTextDatePropertyName(ASAtomGetString(infoKey));
		if( !metaXAP.DoesPropertyExist( kXMP_NS_PDF, infoString.c_str() )  ) 
		{
			XMP_DateTime xapDateTime;
			bool hasValue = metaXAP.GetProperty_Date(nmspace.c_str(),
										path.c_str(),
										&xapDateTime,
										&features);
			if (hasValue)
			{
				std::string pdfDate;
				XAPDateTimeToPDFDate(pdfDate, xapDateTime);
				SetTextPropertyInInfo(cosDoc,
										infoKey,
										pdfDate);
			}
		} 
		else 
		{
			metaXAP.GetProperty(nmspace.c_str(),
									infoString.c_str(),
									&value,
									&features);
			SetTextPropertyInInfo(cosDoc,
									infoKey,
									value);
		}
	} 
	else 
	{
		metaXAP.GetProperty(nmspace.c_str(),
								path.c_str(),
								&value,
								&features);
		SetTextPropertyInInfo(cosDoc,
								infoKey,
								value);
	}
}

void CopyPropertiesFromXAPToInfo( const SXMPMeta& metaXAP, CosDoc cosDoc, const char * nmspace)
{
	SXMPIterator itPaths(metaXAP, nmspace);
	bool hasMore = true;
	do {
		std::string nmspaceSeen;
		std::string path;
		std::string value;
		XMP_OptionBits options;
		hasMore = itPaths.Next(&nmspaceSeen, &path, &value, &options);
		if (hasMore
			&& !XMP_NodeIsSchema(options)
			&& !XMP_PropIsQualifier(options)
		// 1112110 Don't copy anything but simple values
			&& XMP_PropIsSimple(options)) 
		{
			CopyPropertyFromXAPToInfo(metaXAP, cosDoc, nmspaceSeen, path); /* this raises */
		}
	} while ( hasMore );
}

void CopyPropertiesFromXAPToInfo_Aliases( const SXMPMeta& metaXAP, CosDoc cosDoc )
{
	std::string value;		
	XMP_OptionBits options;

	if( metaXAP.GetArrayItem ( kXMP_NS_DC, "creator", 1, &value, &options) && CheckOptionBit(options) )
		SetTextPropertyInInfo(cosDoc, ASAtomFromString("Author"), value);

	if( metaXAP.GetProperty ( kXMP_NS_XMP, "BaseURL", &value, &options ) && CheckOptionBit(options) )
		SetTextPropertyInInfo(cosDoc, ASAtomFromString("BaseURL"), value);

	if( metaXAP.GetProperty ( kXMP_NS_XMP, "CreatorTool", &value, &options ) && CheckOptionBit(options) )
		SetTextPropertyInInfo(cosDoc, ASAtomFromString("Creator"), value);

	if( metaXAP.GetLocalizedText ( kXMP_NS_DC, "description", "", "x-default", NULL, &value, &options ) && CheckOptionBit(options) )
		SetTextPropertyInInfo(cosDoc, ASAtomFromString("Subject"), value);

	if( metaXAP.GetLocalizedText ( kXMP_NS_DC, "title", "", "x-default", NULL, &value, &options ) && CheckOptionBit(options) )
		SetTextPropertyInInfo(cosDoc, ASAtomFromString("Title"), value);

	/* DateProperties are handled differently. These will be taken care by CopyPropertyFromXAPToInfo */
	if( metaXAP.GetProperty ( kXMP_NS_XMP, "CreateDate", &value, &options ) && CheckOptionBit(options) )
		CopyPropertyFromXAPToInfo(metaXAP, cosDoc, kXMP_NS_PDF, "pdf:CreationDate");

	if( metaXAP.GetProperty ( kXMP_NS_XMP, "ModifyDate", &value, &options ) && CheckOptionBit(options) )
		CopyPropertyFromXAPToInfo(metaXAP, cosDoc, kXMP_NS_PDF, "pdf:ModDate");
		
}

void CosDocUpdateDocInfoFromXAP(CosDoc cosDoc, const SXMPMeta& metaXAP)
{
	/* SXMPIterator has stoped honoring kXMP_IterIncludeAliases flag. This function 
	* will copy properties which has been left out by not honoring kXMP_IterIncludeAliases. */		
	CopyPropertiesFromXAPToInfo_Aliases(metaXAP, cosDoc );
	CopyPropertiesFromXAPToInfo(metaXAP, cosDoc, kXMP_NS_PDF);
	CopyPropertiesFromXAPToInfo(metaXAP, cosDoc, kXAP_NS_PDFX);
	{
	// [1511296] Workaround for SXMPIterator bug with kXMP_IterIncludeAliases; delete for Nova
		std::string	prefix;
		SXMPMeta::RegisterNamespace( kXMP_NS_PDFX_ID, kXMP_NS_PDFX_ID_prefix, &prefix );
	}
	CopyPropertiesFromXAPToInfo(metaXAP, cosDoc, kXMP_NS_PDFX_ID);
}

void SetupMetaXAP(SXMPMeta * metaXAP)
{
	std::string prefix;
	metaXAP->RegisterNamespace(kXAP_NS_PDFX, kXAP_NS_PDFX_prefix, &prefix);
	metaXAP->RegisterNamespace(kXMP_NS_PDFX_ID, kXMP_NS_PDFX_ID_prefix, &prefix);
	metaXAP->RegisterNamespace(kXMP_NS_PDFE, kXMP_NS_PDFE_prefix, &prefix);
	metaXAP->RegisterNamespace(kXMP_NS_PDFA_ID, kXMP_NS_PDFA_ID_prefix, &prefix);
}

void SetTextItemInXAP(SXMPMeta * metaXAP,
                 const std::string & nmspace,
                 const std::string & key,
                 const std::string & value)
{
	std::string utf8Value;
	GetUTF8FromFromPDString( value, utf8Value );
	
#if XML_NAME
	XMLNameEscaper valueEscaper(utf8Value);
#endif
    
	metaXAP->SetProperty(nmspace.c_str(),
							key.c_str(),
#if XML_NAME
							valueEscaper.GetEscapedContent().c_str());
#else
							utf8Value.c_str());
#endif
}

void SetDateItemInXAP( SXMPMeta * metaXAP, const std::string & nmspace, const char * key, const std::string & value )
{
	// Is the date item really a date?
	// Bug #554069 points out that this wasn't required in PDF 1.0,
	// and it could happen in bad files in subsequent versions (such as the PDF
	// produced by Photoshop 5).
	// In order not to lose the user's data by converting to a bad date,
	// we transport the non-date string unchanged into a special XMP property
	// set aside for the purpose.
	// Dan Rabin Thu Feb  6 15:15:31 2003
	
	XMP_DateTime xapDateTime;
	if( PDFDateToXAPDateTime(value.c_str(), xapDateTime) )
	{
		// Includes converstion to UTF-8.
		metaXAP->SetProperty_Date(nmspace.c_str(),
									key,
									xapDateTime);
	} else 
	{
		const std::string keyToUse = GetTextDatePropertyName(key);
		if( keyToUse.size() > 0 ) 
		{
			SetTextItemInXAP(metaXAP, kXMP_NS_PDF, keyToUse, value);
		}
	}
}

static void SetPropertyIfAbsent(SXMPMeta * metaXAP,  const char * ns, const char * property, const char * newValue)
{
	if( ! metaXAP->GetProperty(ns, property, NULL, 0)  ) 
	{
		metaXAP->SetProperty(ns, property, newValue, 0);
	}
}

static void CreateVersionIDifAbsent(SXMPMeta * metaXAP)
{
	std::string versionID ("1");
	SetPropertyIfAbsent(metaXAP,
							kXMP_NS_XMP_MM,
							"VersionID",
							(XMP_StringPtr)(versionID.c_str()));
}

bool IsStandardDocInfoProperty(ASAtom nameAtom)
{
    return
        nameAtom == ASAtomFromString("ModDate")
        || nameAtom == ASAtomFromString("CreationDate")
        || nameAtom == ASAtomFromString("Title")
        || nameAtom == ASAtomFromString("Author")
        || nameAtom == ASAtomFromString("Creator")
        || nameAtom == ASAtomFromString("Producer")
        || nameAtom == ASAtomFromString("Subject")
        || nameAtom == ASAtomFromString("Keywords")
        || nameAtom == ASAtomFromString("Trapped")
        ;
}

void SetInfoItemInXAP(SXMPMeta * metaXAP, const char * key, const std::string& value)
{
    const std::string keyString(key);

    if( IsStandardDocInfoProperty(ASAtomFromString(key)) ) 
	{
        if (IsDateProperty(ASAtomFromString(key))) {
            SetDateItemInXAP(metaXAP,
                             kXMP_NS_PDF,
                             key,
                             value);
        } 
		else 
		{
            SetTextItemInXAP(metaXAP,
                             kXMP_NS_PDF,
                             key,
                             value);
        }
    } 
	else 
	{
#if XML_NAME
		XMLNameEscaper escaper(key);
#endif
		if( ASAtomFromString( key ) == ASAtomFromString("ISO_PDFEVersion") )
		{
			SetTextItemInXAP(metaXAP,
				kXMP_NS_PDFE,
#if XML_NAME
				escaper.GetEscaped(),
#else
				key,
#endif
				value);
		}
		else if ( ASAtomFromString( key ) == ASAtomFromString("GTS_PDFXVersion") )
		{
			SetTextItemInXAP(metaXAP,
				kXMP_NS_PDFX_ID,
#if XML_NAME
				escaper.GetEscaped(),
#else
				key,
#endif
				value);

				CreateVersionIDifAbsent(metaXAP);		//bigyan: fix for bug ID#1702557. Add <xmpMM:VersionID> to satisfy Preflight compliance for PDF/X-4
				// fall through
		}
		else
		{
			SetTextItemInXAP(metaXAP,
				kXAP_NS_PDFX,
#if XML_NAME
				escaper.GetEscaped(),
#else
				key,
#endif
				value);
		}
	}
}

} //namespace PDF_Handler
