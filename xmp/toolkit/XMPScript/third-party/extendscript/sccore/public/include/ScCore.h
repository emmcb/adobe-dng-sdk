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
#ifndef _ScCore_h
#define _ScCore_h

/**
@name Introduction
The ScCore library contains a set of core classes that all scripting components use.
These core classes use a common memory allocation strategy that can be altered by clients
if they wish by setting a new memory allocator. Most classes are container classes,
or they are Unicode strings with additional encoding or decoding features. You have
cross-platform classes for file handling, and finally, you have all of the LiveObject
classes that permit you to write scriptable objects.
<p>
The general namespace for all core classes is <b>ScCore</b>.
*/
///

#include "ScAllocator.h"
#include "ScArray.h"
#include "ScDialogs.h"
#include "ScDictionary.h"
#include "ScError.h"
#include "ScHashTable.h"
#include "ScEncoder.h"
#include "ScInitTerm.h"
#include "ScLiveIDs.h"
#include "ScLiveMessage.h"
#include "ScLiveComponent.h"
#include "ScLiveArray.h"
#include "ScLivePropertyManager.h"
#include "ScLocalizer.h"
#include "ScRegExp.h"
#include "ScSimpleArray.h"
#include "ScSparseArray.h"
#include "ScString.h"
#include "ScTime.h"
#include "ScUnicodeUtils.h"
#include "ScVariant.h"

#include "ScPointRect.h"
#if SC_NO_DISKIO == 0
#include "ScFileSpec.h"
#include "ScFileUtils.h"
#include "ScFile.h"
#include "ScFileWrapper.h"
#include "ScFolder.h"
#endif

#if SC_NO_UNITVALUE == 0
#include "ScUnitConverter.h"
#include "ScUnitValue.h"
#endif

#if SC_NO_XML == 10
#include "ScXML.h"
#endif

#include "ScLockable.h"
#include "ScCriticalSection.h"
#include "ScContext.h"
#include "ScThread.h"
#include "ScVersion.h"

#endif

