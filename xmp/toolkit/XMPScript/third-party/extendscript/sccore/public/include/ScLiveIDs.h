/**************************************************************************
*
* ADOBE CONFIDENTIAL
* ___________________
*
*  Copyright 1998 - 2007 Adobe Systems Incorporated
*  All Rights Reserved.
*
* NOTICE:  All information contained herein is, and remains the property of
* Adobe Systems Incorporated  and its suppliers,  if any.  The intellectual 
* and technical concepts contained herein are proprietary to  Adobe Systems 
* Incorporated  and its suppliers  and may be  covered by U.S.  and Foreign 
* Patents,patents in process,and are protected by trade secret or copyright 
* law.  Dissemination of this  information or reproduction of this material
* is strictly  forbidden  unless prior written permission is  obtained from 
* Adobe Systems Incorporated.
* $File$
* $Author$
* $DateTime$
* $Revision$
* $Change$
**************************************************************************/

#if SC_ONCE
#pragma once
#endif
#ifndef _ScLiveIDs_h
#define	_ScLiveIDs_h

/**
This enumeration contains a list of pre-registered characters for 
Adobe apps. If you want to publish your LiveIDs, please have a character 
registered for you.
*/

enum LiveIDs : scint32_t
{
	/// All core LiveIDs start with a ' '.
	kLiveIDScCore			= 0x20202020, // '    ',
	/// All AfterEffects LiveIDs start with a 'a'.
	kLiveIDAfterEffects		= 0x61202020, // 'a   ',
	/// All GoLive LiveIDs start with a 'g'.
	kLiveIDGoLive			= 0x67202020, // 'g   ',
	/// All InDesign LiveIDs start with a 'i'.
	kLiveIDInDesign			= 0x69202020, // 'i   ',
	/// All Illustrator LiveIDs start with a 'u'.
	kLiveIDIllustrator		= 0x75202020, // 'u   ',
	/// All MultiScript LiveIDs start with a 'm'.
	kLiveIDMultiScript		= 0x6D202020, // 'm   ',
	///	All ScriptUI LiveIDs start with a 'U'.
	kLiveIDScriptUI			= 0x4D202020,  // 'U   '
};

#endif


