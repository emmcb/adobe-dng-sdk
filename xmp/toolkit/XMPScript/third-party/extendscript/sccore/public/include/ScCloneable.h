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
#ifndef _ScCloneable_h
#define _ScCloneable_h

#include "ScRoot.h"
#include "ScPreConfig.h"

namespace ScCore
{

/**
Cloneable objects are core objects that have a generic, virtual duplication method. 
*/

class SC_API Cloneable : public Root
{
public:
	virtual	~Cloneable();
	/**
	The clone method creates an exact duplicate of this instance if the optional
	parameter "deepClone" is true. If false, the method creates a "light" clone.
	How the parameter is interpreted depends on the implementer of the class.
	The return value is an error code. Many error codes are predefined in ScError.h.
	Must be implemented.
	*/
	virtual scerror_t clone (Cloneable** targetLocation, bool deepClone = true) const = 0;
};

}	// end Namespace

#include "ScPostConfig.h"

#endif

