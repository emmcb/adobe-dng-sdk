#ifndef __XMPScript_ChangeLog_hpp__
#define __XMPScript_ChangeLog_hpp__ 1

// =================================================================================================
// Copyright 2006-2008 Adobe
// All Rights Reserved.
//
// NOTICE:  Adobe permits you to use, modify, and distribute this file in accordance with the terms
// of the Adobe license agreement accompanying it. If you have received this file from a source other 
// than Adobe, then your use, modification, or distribution of it requires the prior written permission
// of Adobe.
// =================================================================================================

// =================================================================================================
// This file has two purposes, to provide the current engineering build number and to maintain a log
// of source changes going into each build. All but the most trivial changes should be noted here.
// This is the primary source for release notes. It is not necessary to change the build number for
// every change, but it should be updated prior to weekly builds if there are changes. The build
// number is maintained by hand. It reflects the state of the source. Official builds also include
// a Perforce changelist number and timestamp, so that separate builds from identical source can be
// differentiated.
// =================================================================================================

#define kXMPScript_EngBuild     0
#define kXMPScript_EngString "000"

#if ! XMP_DebugBuild
	#define kXMPScript_DebugSuffix
	#define kXMPScript_DebugString ""
#else
	#define kXMPScript_DebugSuffix (debug)
	#define kXMPScript_DebugString " (debug)"
#endif

// =================================================================================================
// Change history
// ==============
//
// Writers:
//    SM  Stefan Makswit
//    AWL Alan Lillich
//    FNO Frank NOcke
//	  JEH Joerg Ehrlich
//	  SAM Samy Makki
//	  DKG Deepak Kumar Garg
//
// yyyy-mm-dd who M.m-'s'bbb Description of changes, most recent on top.
//
// 2010-09-30 SAM 5.2-s002 Separate version numbers for XMPCore, XMPFiles and XMPScript
// 2010-07-28 JEH 5.2-s001 Bumping version to 5.2, reset build number (no changes).
//
// 2010-01-28 SAM 5.1-s001 Bump version to 5.1, reset build number.
//
// 2009-10-14 JEH 5.0-s003 Adding proper UTF-8 conversion to getter methods in XMPMeta
//
// 2009-08-14 FNO 5.0-c49f62i14s002 [2387633] Add missing CFBundleVersion in Info.plist.
//
// 2008-10-31 SM  5.0-s001 Bumped to version 5.0
//                         Bugfix #1914306 (Setting a Property with the Current Date Fails if your Clock is set to GMT);
//                         Support flags hasDate, hasTime and hasTimeZone in object XMPDateTime;
//                         added methods XMPDateTime#hasDate(), hasTime() and hasTimeZone();
//                         added readable error messages to all methods
//
// 2008-10-07 FNO 4.4-s002 [1817149] Show company names in file explorer properties tab.
//
// 2008-02-08 FNO 4.4-s001 Bumping build number to 4.4, reset Eng Builder Numbers to 1
//
// 2007-10-23 FNO 4.2-c22f30s11 added 'acrobat' linker flags and buffer checks
//
// 2007-09-26 SM  4.2-s010 Added XMPCore#sort() method.
//
// 2007-09-03 SM  4.2-s009 Updated to be compatible with ExtendScript 3.91.78
//
// 2007-05-15 ALB 4.2-s008 New version #defines (SRM).
//
// 2007-04-02 SM  4.2-s007 Add constant for standard namespace for the WAV legacy digest 
//                         and NS_PDFX_ID constant.
//
// 2007-02-13 AWL 4.2-s006 Centralize the Adobe IP constant.
//
// 2007-02-06 AWL 4.2-s005 Version info tweaks.
//
// 2007-01-16 SM  4.2-s004 Updated the copyright year to 2006-2007 
//
// 2007-01-09 SM  4.2-s003 Bugfix #1454864 (XMPConst.NS_XMP_MM is undefined); 
//                         fixed and added constants in XMPConst.cpp.
//
// 2006-12-13 SM  4.2-s002 Bugfix #1530826; Replaced isDefined() by args.length() > ... 
//                         to check number of method parameters.  
//
// 2006-12-06 AWL 4.2-s001 Bump API version to 4.2, reset eng build to 1. [1425907] Move build info
//						   (branch, changelist, date) to the FileVersion tag.
//
// 2006-11-29 SM  4.1-s005 Bugfix #1422050 (Exit Bridge will crash after set a property for 
//                         a new object of XMPMeta); updated XMPScript#ESTerminate()-method.
//
// 2006-09-06 SM  4.1-s004 Usage of standard strings to convert JS arrays
//
// 2006-08-10 SM  4.1-s003 Fixed that "undefined" is returned when a property is not found
//
// 2006-08-01 AWL 4.1-s002 Bump API version to 4.1.
//
// 2006-05-16 SM  4.1-s001 Initial version.
//
// =================================================================================================

#endif	/* __XMPScript_ChangeLog_hpp__ */
