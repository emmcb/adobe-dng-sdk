/*********************************************************************

ADOBE SYSTEMS INCORPORATED
Copyright (C) 1996-2006 Adobe Systems Incorporated
All rights reserved.

NOTICE: Adobe permits you to use, modify, and distribute this file
in accordance with the terms of the Adobe license agreement
accompanying it. If you have received this file from a source other
than Adobe, then your use, modification, or distribution of it
requires the prior written permission of Adobe.

---------------------------------------------------------------------

PDSysFontExpT.h

- PDSysFont Types header file.

*********************************************************************/

#ifndef _H_PSFExpT
#define _H_PSFExpT


#include "Environ.h"
#if PLUGIN || ACROBAT_LIBRARY
#include "CoreExpT.h"
#else
#include "PubTypes.h"
#endif

#if WIN_PLATFORM
#include <Windows.h>
#endif

#if MAC_PLATFORM
#if AS_ARCH_64BIT
//#include <CoreServices/CoreServices.h>
typedef UInt32  ATSFontRef;
typedef UInt32	ATSFontFamilyRef;
#else
//#include <ApplicationServices/ApplicationServices.h>
#endif
#endif

#if defined( WINDOWS ) || defined( WIN_ENV )
#pragma pack(push, peexpt, 8)
#endif

typedef ASInt16 PDSysFontMode;

#if MAC_PLATFORM
#if IOS
typedef int PDSysFontFID;
typedef ASInt16 PDSysFontFStyle;
typedef void* PDSysFontFRef;
#else
typedef ATSFontRef PDSysFontFRef;
#if AS_ARCH_64BIT
typedef ATSFontFamilyRef PDSysFontFontFamilyRef;
#else
typedef FMFontFamily PDSysFontFID;
#endif
typedef ASInt16 PDSysFontFStyle;
#endif
#endif	// MAC_PLATFORM

/** A reference to a font installed in the host system. PDSysFont methods allow you to
list the fonts available in the host system and to find a font in the system that matches
a PDEFont, if it is present.
@see PDEnumSysFonts
@see PDFindSysFont
@see PDFindSysFontEx
@see PDFindSysFontForPDEFont
@see PDEnumSysFonts
*/
typedef struct _t_PDSysFont *PDSysFont;


/** Font matching flags for PDFindSysFontForPDEFont() and PDFindSysFont().
@see PDFindSysFont
@see PDFindSysFontForPDEFont
*/
typedef enum {

	/** Match the font name and character set. */
	kPDSysFontMatchNameAndCharSet = 0x0001,

	/** Match the font type. */
	kPDSysFontMatchFontType = 0x0002,

	/** Match the writing mode (horizontal or vertical). */
	kPDSysFontMatchWritingMode = 0x0004
} PDSysFontMatchFlags;

/** PDEFont information.
@see PDSysFontGetInfo
*/
typedef struct _t_PDEFontInfo {

	/** An ASAtom for the font name (for example, <code>"Times-Roman"</code>). */
	ASAtom	name;

	/**An ASAtom for font type (for example, <code>"Type 1"</code>, <code>"TrueType"</code>, and so on).  */
	ASAtom	type;

	/** An ASAtom for <code>"Roman"</code>, or ASAtomNull. If it is <code>"Roman"</code>, the
	characters must be a subset of the Adobe Standard Roman Character Set.
	*/
	ASAtom	charSet;

	/** An ASAtom for font encoding, such as WinAnsiEncoding. */
	ASAtom	encoding;

	/** The writing mode: <code>0</code> means horizontal and <code>1</code> means vertical. */
	PDSysFontMode	wMode;
} PDEFontInfoRec, *PDEFontInfoP;


/** PDSysFontPlatData */
#if WIN_PLATFORM
typedef struct _t_PDSysFontPlatData {

	/** This must be <code>sizeof(PDSysFontPlatData)</code>. */
	ASSize_t size;

	/** (Windows only) The Windows <code>LOGFONTA</code> structure defining font attributes.*/
	LOGFONTA *lf;

	/** (Optional) This is only set if <code>LOGFONTA</code> is not present. */
	ASPathName fontPath;

	/** (Optional) This is only set if <code>LOGFONTA</code> is not present. */
	ASPathName afmPath;

	/** (Windows only) The Windows <code>LOGFONTW</code> structure defining font attributes. Whenever
	** we return a valid <code>LOGFONTA</code>, we return a valid <code>LOGFONTW</code>. */
	LOGFONTW *lfw;
} PDSysFontPlatData, *PDSysFontPlatDataP;
#elif MAC_PLATFORM
typedef struct _t_PDSysFontPlatData {

	/** This must be <code>sizeof(PDSysFontPlatData)</code>. */
	ASSize_t size;

#if !AS_ARCH_64BIT

	/** IMPORTANT: The 2 fields fontID & fontStyle have been deprecated and are also not available on Mac64.
	Clients should not use these 2 fields. They should use the ATSFontRef returned back in fontRef. */

	/** DEPRECATED: The font ID.  */
	PDSysFontFID fontID;

	/** DEPRECATED: The style value within that font. */
	PDSysFontFStyle fontStyle;

#endif // !AS_ARCH_64BIT	

	/* The ATSFontRef of the sys font.*/
	PDSysFontFRef fontRef;
} PDSysFontPlatData, *PDSysFontPlatDataP;
#elif UNIX_PLATFORM
typedef struct _t_PDSysFontPlatData {

	/** This must be <code>sizeof(PDSysFontPlatData)</code>. */
	ASSize_t size;

	/** A path to the font file. It is set only if <code>lf</code> is not present. */
	ASPathName fontPath;

	/** A path to the font AFM file. It is set only if <code>lf</code> is not present. */
	ASPathName afmPath;
} PDSysFontPlatData, *PDSysFontPlatDataP;
#endif

/* PDSysFontEnumProc */

/**
A callback for PDEnumSysFonts(). It is called once for each
system font.
@param sysFont IN/OUT The system font.
@param clientData IN/OUT User-supplied data that was specified
in the call to PDEnumSysFonts().
@return <code>true</code> to continue enumeration, <code>false</code> to halt enumeration.

@see PDEnumSysFonts
*/
typedef ACCBPROTO1 ASBool(ACCBPROTO2 *PDSysFontEnumProc)(PDSysFont sysFont, void *clientData);

#if defined( WINDOWS ) || defined( WIN_ENV )
#pragma pack (pop, peexpt )		/* reset to /Zp  */
#endif

#endif /* _H_PSFExpT */
