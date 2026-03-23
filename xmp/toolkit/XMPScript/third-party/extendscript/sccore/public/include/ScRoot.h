/**************************************************************************
* $File$
* $Author$
* $DateTime$
* $Revision$
* $Change$
*
* ADOBE CONFIDENTIAL
* ___________________
*
*  Copyright 1998 - 2002 Adobe Systems Incorporated
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
**************************************************************************/

#if SC_ONCE
#pragma once
#endif
#ifndef _ScRoot_h
#define _ScRoot_h

#include "ScHeap.h"
#include "ScPreConfig.h"

namespace ScCore
{

/**
The root class for all ScCore objects adds a virtual destructor to the Heap class.
*/

class SC_API SC_PURE Root : public Heap
{
					Root (const Root&);
	Root& 			operator= (const Root&);
public:
	/// Inline constructor to keep some compilers happy.
	inline			Root() {}
	/// The virtual desctructor is empty.
	virtual		    ~Root();
};

}	// end Namespace

#include "ScPostConfig.h"

#endif


