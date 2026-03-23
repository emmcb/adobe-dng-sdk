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

#ifndef _EsVersion_h
#define	_EsVersion_h

#include "ScVersion.h"

/**
The ExtendScript version number is a 32 bit integer with four sub-fields. 
It consists of a "debug vs. release" field, a "major version" field, a 
"minor version" field, and an "engineering" or "build number" field. The 
highest bit (0x80000000) of the integer is set to 1 for debug builds of 
ExtendScript and is set to 0 for release builds. The next highest 15 bits 
(0x7fff0000) are the major version. The next highest 8 bits (0x0000ff00) 
are the minor version. The lowest 8 bits (0x000000ff) are the engineering 
version.
<p>
On Windows the ExtendScript version number number is displayed in the 
ExtendScript.dll file by looking in the Properties dialog (right-click on 
the ExtendScript.dll file and choose "Properties" from the pop-up menu). 
In this dialog, look at the "Version" panel. The version number is displayed 
as the "File version:"
<p>
On Macintosh the version number can be seen by selecting the ExtendScriptLib 
file in the finder and choosing "Get Info" from the "File" menu. In that 
dialog (in the "General Information" panel) the version number is displayed 
at the beginning of the "Version:" field. Alternatively, run the JavaScript
"$.about()", which pops up the About dialog box containing all build info.
<p>
The version number can be determined programatically by calling the 
sEngine::getVersion() function. This will return the version number as an 
integer. The versioning scheme is the same as for all CoreTech components.
<p>
Currently, the ExtendScript version number matches the version number of ScCore.
*/

#define	ES_VERSION_STRING		SC_VERSION_STRING
#define ES_VERSION_MAJOR		SC_VERSION_MAJOR
#define ES_VERSION_MINOR		SC_VERSION_MINOR
#define ES_VERSION_ENG			SC_VERSION_ENG

/// The ES_VERSION constant does not include the debug bit.

#define ES_VERSION ((ES_VERSION_MAJOR << 16) | (ES_VERSION_MINOR << 8) | ES_VERSION_ENG)

#endif

