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
*  Copyright 1998 - 2003 Adobe Systems Incorporated
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
#ifndef _ScTime_h
#define _ScTime_h

#include "ScDefs.h"
#include "ScPreConfig.h"

namespace ScCore
{

/**
The Time class returns the time in milliseconds elapsed since Jan 1, 1970.
This value is as precise as possible, and may thus also be used for measurements.
*/

class SC_API Time
{
public:
	/**
	Get a tick count in 1/20 secs. Called repeatedly to measure elapsed time in a quick way.
	*/
	static	scuint32_t 	getTicks();
	/**
	Get the contents of a high-resolution timer in microseconds. If the hardware
	does not support high-resolution timers, return getUTCTime() * 1000.
	*/
	static	scint64_t		getHiResTimer();
	/**
	Return the number of milliseconds elapsed since January 1, 1970 in UTC.
	*/
	static	double		getUTCTime();
	/**
	Check the given time for being within DST. Return 1 is yes, 0 if no, or -1 for don't know.
	*/
	static	int			isDST (double time);
	/**
	Check the current time for being within DST. Return 1 is yes, 0 if no, or -1 for don't know.
	*/
	static	int			isDST();
	/**
	Return the difference to the UTC time in milliseconds. This is a positive
	value for times east of GMT, and a negative value for times west of GMT.
	The value should NEVER include Daylight Saving Time.
	@return				the offset
	*/
	static	double		getTimeZoneOffset();
};

}	// end Namespace

#include "ScPostConfig.h"

#endif // _ScTime_h

