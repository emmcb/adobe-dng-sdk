/**************************************************************************
*
* ADOBE CONFIDENTIAL
* ___________________
*
*  Copyright 1998 - 2006 Adobe Systems Incorporated
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

#ifndef _ExtendScript_h
#define _ExtendScript_h

/**
@name Introduction
The ExtendScript libray is the Adobe JavaScript engine. It provides a fully
customizable, fast JavaScript interpreter that fully conforms to the current
ECMA 262 standard. ExtendScript uses the scripting core classes DLL and
leverages the LiveObjects technology for hgost-supplied objects.
<p>
The general namespace for ExtendScript classes is <b>ScScript</b>.
*/
///

#include "ScCore.h"

#include "EsCallback.h"
#include "EsDataPool.h"
#include "EsDebugger.h"
#include "EsDispatcher.h"
#include "EsScript.h"
#include "EsEngine.h"
#include "EsInitTerm.h"
#include "EsFileDisp.h"
#include "EsPreprocessor.h"
#include "EsScriptContainer.h"
#include "api/EsScopeInfo.h"
#include "api/EsDebugAPI.h"
#include "api/EsHiliteAPI.h"
#include "api/EsLiveObjectAPI.h"
#include "api/EsNode.h"
#include "api/EsParserAPI.h"

#endif

