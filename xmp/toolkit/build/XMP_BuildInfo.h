#ifndef __XMP_BuildInfo_h__
#define __XMP_BuildInfo_h__ 1

/* --------------------------------------------------------------------------------------------- */
/* ** IMPORTANT ** This file must be usable by strict ANSI C compilers. No "//" comments, etc.   */
/* --------------------------------------------------------------------------------------------- */

/*
// =================================================================================================
// Copyright 2002-2008 Adobe
// All Rights Reserved.
//
// NOTICE:  Adobe permits you to use, modify, and distribute this file in accordance with the terms
// of the Adobe license agreement accompanying it. If you have received this file from a source other 
// than Adobe, then your use, modification, or distribution of it requires the prior written permission
// of Adobe.
// =================================================================================================
*/

#if AdobePrivate
/*
// =================================================================================================
// Change history
// ==============
//
// Writers:
//  AWL Alan Lillich
//  SAM Samy Makki
//
// mm-dd-yy who Description of changes, most recent on top.
//
// 11-19-14 ADC 1.0-a001 Framework in place for Asset Management Library.
// 30-01-09 SAM changing to non-quoted version strings again (Baton#430).
// 12-03-08 SAM following versioning guidelines.
// 02-14-08 AWL [1507142] Tweak the copyright strings.
// 05-15-07 ALB New version #defines.
// 02-16-07 AWL Hide some parts from the SDK.
// 02-13-07 AWL Centralize the Adobe IP constant.
// 01-28-05 AWL 3.2-001 Remove BIB.
// 03-11-04 AWL Fix build number comments.
// 02-02-04 AWL Initial creation.
//
// =================================================================================================
*/
#endif

#if AdobePrivate
/*
// =================================================================================================
// This file provides build numbers that are set automatically by the external build system. This is
// in contrast to the manual version numbers of XMP_ChangeLog.h.
//
// The value of BUILDDATE is set to the date of the build, in some reasonable format. It would
// be nice if the month used a name instead of a number and if the year has 4 digits. But it isn't
// worth going to great lengths for this, a numeric date is OK.

// The value of BUILDVERSION is set to some number that is meaningful to the build system, such
// as a Perforce changelist number.
// =================================================================================================
*/
#endif

#define kXMP_Copyright Copyright 1987 Adobe Inc. All rights reserved.
#define kXMP_CopyrightStr "Copyright 1987 Adobe Inc. All rights reserved."

#define kXMP_AdobeIPStr "<AdobeIP#0000867>"

// Windows: Stringification convention
#define MAKESTR2(a) #a
#define MAKESTR(a) MAKESTR2(a)

// MAC: Symbols needed for Info.plist; ugly multistage construction because
// they need to be symbols, not strings, no quotes, no whitespace
#define MACRO_PASTE2(a, b, c) a##b##c
#define MACRO_PASTE(a, b, c) MACRO_PASTE2(a, b, c)

#define BUILDID 1

#define BUILDDATE 0000/00/00-00:00:00
#define BUILDBRANCH 1
#define BUILDCHANGELIST 000000
// No RE_REBUILD for now, as not part of SRM2.

#define BUILDVERSION_MAC MACRO_PASTE(BUILDBRANCH, ., BUILDCHANGELIST)

// This mapping is just to aviod changing all the old variables in the code
#define BUILDDATESTR MAKESTR(BUILDDATE)
#define BUILDBRANCHSTR MAKESTR(BUILDBRANCH)
#define BUILDVERSIONSTR MAKESTR(BUILDVERSION_MAC)
#define BUILDIDSTR MAKESTR(BUILDID)

#endif	/* __XMP_BuildInfo_h__ */
