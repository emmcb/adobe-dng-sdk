/**************************************************************************
*
* ADOBE CONFIDENTIAL
* ___________________
*
*  Copyright 1998 Adobe Systems Incorporated
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

#ifndef _ScVersion_h
#define	_ScVersion_h

/**
The core version number is a 32 bit integer with four sub-fields.
It consists of a "debug vs. release" field, a "major version" field, a
"minor version" field, and an "engineering" or "build number" field. The
highest bit (0x80000000) of the integer is set to 1 for debug builds of
ExtendScript and is set to 0 for release builds. The next highest 15 bits
(0x7fff0000) are the major version. The next highest 8 bits (0x0000ff00)
are the minor version. The lowest 8 bits (0x000000ff) are the engineering
version.
<p>
On Windows the core version number number is displayed in the
DLL file by looking in the Properties dialog (right-click on
the DLL file and choose "Properties" from the pop-up menu).
In this dialog, look at the "Version" panel. The version number is displayed
as the "File version:"
<p>
On Macintosh the version number can be seen by selecting the Bundle
file in the finder and choosing "Get Info" from the "File" menu. In that
dialog (in the "General Information" panel) the version number is displayed
at the beginning of the "Version:" field.
<p>
The version number can be determined programatically by calling the
ScCore::getVersion() function. This will return the version number as an
integer. The versioning scheme is the same as for all CoreTech components.
*/

#define SC_VERSION_MAJOR		4
#define SC_VERSION_MINOR		5
#define SC_VERSION_ENG			11

// The Copyright year appears in the version resources.

#define SC_COPYRIGHT_YEAR		2019

/// The ExtendScript Audit ID

#define SC_IP_AUDIT_ID			"<AdobeIP#0000936>"

/////////////////////////////////////////////////////////////////////////

// Stringizing operator

#ifndef ADOBE_STR
	#define ADOBE_STR( x ) _ADOBE_STR( x )
	#define _ADOBE_STR( x ) #x
#endif

/// The SC_VERSION constant does not include the debug bit.

#define SC_VERSION				((SC_VERSION_MAJOR << 16) | (SC_VERSION_MINOR << 8) | SC_VERSION_ENG)
#define	SC_VERSION_STRING		ADOBE_STR( SC_VERSION_MAJOR ) "." ADOBE_STR( SC_VERSION_MINOR) "." ADOBE_STR( SC_VERSION_ENG )


#endif

