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

#if SC_ONCE
#pragma once
#endif
#ifndef _ScStaticLibDefs_h
#define _ScStaticLibDefs_h

/** This file is intended to be a config file for static library builds of
the ExtendScript system. The file contains various defines that permit the
user to strip unwanted functionality out of the ExtendScript system. Set any
of the defines below to 1 to exclude stuff.
To have this included, define one of the constants SC_LIB or ES_LIB which 
causes ScDefs.h to include this file.
*/

#ifndef SC_LIB
#define SC_LIB 1
#endif
#ifndef ES_LIB
#define ES_LIB 1
#endif

/// Do not include unit values.
#ifndef SC_NO_UNITVALUE
#define SC_NO_UNITVALUE		0
#endif

/** Do not include File and Folder classes as well as all ScCore file related classes.
Setting this constant to 1 will also disable the #include and #includepath preprocessor
statements. */
#ifndef SC_NO_DISKIO
#define SC_NO_DISKIO		0
#endif

/** Compile without XML support. 
If you define the following symbol to be 1, Do Not Include the following subdirectories 
in your project:
source/xml
source/xpath
source/expat
*/

#ifndef SC_NO_XML
#define SC_NO_XML	0
#endif

#endif

