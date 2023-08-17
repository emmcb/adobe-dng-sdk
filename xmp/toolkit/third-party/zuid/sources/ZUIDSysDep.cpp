// (c) Copyright 1999.  Adobe Systems, Incorporated.  All rights reserved.
//  $Id: $ 

// Win can't see directives placed before pre-comp headers

#include "third-party/zuid/interfaces/ZUIDSysDep.h"
#include "third-party/zuid/interfaces/MD5.h"
#include <cassert>
#include <cstring>

using namespace std;

/******************************************************************************/

/*
	Copyright (c) 1990 - 1993, 1996 Open Software Foundation, Inc.
	Copyright (c) 1989 by Hewlett-Packard Company, Palo Alto, Ca. &
	Digital Equipment Corporation, Maynard, Mass.
	Copyright (c) 1998 Microsoft.
  
	To anyone who acknowledges that this file is provided "AS IS" without any 
	express or implied warranty: permission to use, copy, modify, and 
	distribute this file for any purpose is hereby granted without fee, 
	provided that the above copyright notices and this notice appears in all 
	source code copies, and that none of the names of Open Software Foundation, 
	Inc., Hewlett-Packard Company, or Digital Equipment Corporation be used in 
	advertising or publicity pertaining to distribution of the software without 
	specific, written prior permission. Neither Open Software Foundation, Inc., 
	Hewlett-Packard Company, Microsoft, nor Digital Equipment Corporation makes 
	any representations about the suitability of this software for any purpose.
*/

/******************************************************************************/

static void GetRandomAddress ( uuid_node_t * node )
	{
	XMP_Uns8 seed[16];
	
	get_random_info(seed);
    seed[0] |= 0x80;	// Mark this as Multicast
    
    assert ( sizeof(*node) == sizeof(uuid_node_t) );
    memcpy ( node, seed, sizeof(*node) );	// AUDIT: Safe, using sizeof destination.
    }
  	
/******************************************************************************/

#if defined ( MAC_ENV ) && !defined(IOS_ENV)

#include <CoreServices/CoreServices.h>
#include <mach/mach_time.h>

static uuid_time_t GetBootTime()
	{
	static bool inited = false;
	static uuid_time_t bootTime;
	if (!inited)
		{
	    XMP_Uns32 time = 0;
// *** Mac-64	    GetDateTime(&time);
				
		/*
			MacOS keeps time in seconds since Jan 1, 1904. UUIDs use time in 100ns ticks
			since Oct 15, 1582. The difference is 10,136,620,800 seconds.
	    */
	    
		XMP_Uns64 longTime = time;
		longTime += I64(10136620800);	// See comment above
		longTime *= I64(1000000);		// Microseconds
		
		XMP_Uns64 microseconds = 0;
		//Microseconds((UnsignedWide*)&microseconds); // since boot
        microseconds = mach_absolute_time();
		
		longTime -= microseconds;	// boot time in microseconds;
		bootTime = longTime * UUIDS_PER_TICK;	// 100ns ticks
		
		inited = true;
		}
		
	return bootTime;
	}
  	
/******************************************************************************/

 void get_random_info(XMP_Uns8 seed[16])
  	{
    struct randomness
    	{
    	XMP_Uns32			freeMem;
// *** Mac-64        THz				applicationZone;
        XMP_Uns32	dateTime;
        UnsignedWide	microseconds;
        Str255			userName;
 		};
 		
    randomness r;

	r.freeMem			= 0;	// *** Mac-64	FreeMem();

// *** Mac-64	r.applicationZone	= 0;	// Old Mac OS cruft.

	r.dateTime = 0;
// *** Mac-64	GetDateTime(&r.dateTime);

	//Microseconds(&r.microseconds);
        XMP_Uns64 microseconds_64 =0;
        microseconds_64 = mach_absolute_time();
        r.microseconds= *(UnsignedWide*)&microseconds_64;
	
// *** Mac-64	StringHandle userName = GetString(-16096);
// *** Mac-64	if (userName != NULL && *userName != NULL)
// *** Mac-64		BlockMoveData(*userName, &r.userName, (*userName)[0]);
// *** Mac-64	else
		r.userName[0] = 0;
    
    MD5_CTX c;
    
    MD5Init(&c);
    MD5Update(&c, (XMP_Uns8*)&r, sizeof(randomness));
    MD5Final(seed, &c);
	}
  	
/******************************************************************************/
  	
void get_system_time(uuid_time_t* uuid_time)
	{
	XMP_Uns64 microseconds;
	//Microseconds((UnsignedWide*)&microseconds); // since boot
    microseconds = mach_absolute_time();
        
	*uuid_time = microseconds * UUIDS_PER_TICK + GetBootTime();
	}
  	
/******************************************************************************/

#elif defined ( IOS_ENV )

#include <sys/time.h>

/******************************************************************************/

void get_random_info(XMP_Uns8 seed[16])
{
    struct randomness
    {
    	XMP_Uns32			freeMem;
        // *** Mac-64        THz				applicationZone;
        XMP_Uns32	dateTime;
        XMP_Uns32	microseconds_lo;
        XMP_Uns32	microseconds_hi;
		XMP_Uns8    userName[256];
    };
    
    randomness r;
    
	r.freeMem = 0;
    
	r.dateTime = 0;
    
    struct timeval tp;
    gettimeofday(&tp, 0);
    r.microseconds_hi = (XMP_Uns32)tp.tv_sec;
    r.microseconds_lo = tp.tv_usec * 10;
	
    r.userName[0] = 0;
    
    MD5_CTX c;
    
    MD5Init(&c);
    MD5Update(&c, (XMP_Uns8*)&r, sizeof(randomness));
    MD5Final(seed, &c);
}

/******************************************************************************/

void get_system_time(uuid_time_t* uuid_time)
{
    struct timeval tp;
    gettimeofday(&tp, 0);
    
    // Offset between UUID formatted times and Unix formatted times.
    // UUID UTC base time is October 15, 1582.
    // Unix base time is January 1, 1970.
    *uuid_time = ((uuid_time_t)tp.tv_sec * 10000000) + ((uuid_time_t)tp.tv_usec * 10) +
    I64(0x01B21DD213814000);
}

/******************************************************************************/


#elif defined ( WIN_ENV )
  	
/******************************************************************************/

void get_system_time(uuid_time_t *uuid_time)
	{
    ULARGE_INTEGER time;

    GetSystemTimeAsFileTime((FILETIME *)&time);

      /* NT keeps time in FILETIME format which is 100ns ticks since
       Jan 1, 1601.  UUIDs use time in 100ns ticks since Oct 15, 1582.
       The difference is 17 Days in Oct + 30 (Nov) + 31 (Dec)
       + 18 years and 5 leap days.
    */

      time.QuadPart +=
            (unsigned __int64) (1000*1000*10)       // seconds
          * (unsigned __int64) (60 * 60 * 24)       // days
          * (unsigned __int64) (17+30+31+365*18+5); // # of days

    *uuid_time = time.QuadPart;
	}
  	
/******************************************************************************/

void get_random_info(XMP_Uns8 seed[16])
	{
    MD5_CTX c;

    typedef struct {
        MEMORYSTATUS m;
        SYSTEM_INFO s;
        FILETIME t;
        LARGE_INTEGER pc;
#ifndef XMP_OS_WINUNIVERSAL
        DWORD tc;
#else
		ULONGLONG tc;
#endif //#ifdef XMP_OS_WINUNIVERSAL
        DWORD l;
		DWORD processID;
        char hostname[MAX_COMPUTERNAME_LENGTH + 1];
    } randomness;
    randomness r;

    MD5Init(&c);
#ifndef XMP_OS_WINUNIVERSAL
    /* memory usage stats */
    GlobalMemoryStatus(&r.m);
    /* random system stats */
    GetSystemInfo(&r.s);
#else
#ifndef XMP_OS_WINUNIVERSAL
	/* memory usage stats */
    GlobalMemoryStatusEx(&r.m); //not available for winRT.
#endif //#ifndef WINRT
	memset(&r.m, 0, sizeof(MEMORYSTATUS));
    /* random system stats */
    GetNativeSystemInfo(&r.s);
#endif //#ifdef XMP_OS_WINUNIVERSAL
    /* 100ns resolution (nominally) time of day */
    GetSystemTimeAsFileTime(&r.t);
    /* high resolution performance counter */
    QueryPerformanceCounter(&r.pc);
#ifndef XMP_OS_WINUNIVERSAL
	/* milliseconds since last boot */
    r.tc = GetTickCount();
    r.l = MAX_COMPUTERNAME_LENGTH + 1;
#else
	r.tc = GetTickCount64();
	r.l = MAX_COMPUTERNAME_LENGTH + 1;
#endif //#ifdef XMP_OS_WINUNIVERSAL
	r.processID = GetCurrentProcessId();

    // *** AWL - disable, need new ZUID code anyway: GetComputerName(r.hostname, &r.l );
    MD5Update(&c, (XMP_Uns8*)&r, sizeof(randomness));
    MD5Final(seed, &c);
	}

#elif defined ( UNIX_ENV ) || defined (ANDROID_ENV) || defined(WEB_ENV)

// *** #error "Need UNIX implementation"

void get_system_time(uuid_time_t *uuid_time)
{
	memset ( uuid_time, 0, sizeof(uuid_time_t) );
}

void get_random_info(XMP_Uns8 seed[16])
{
	memset ( seed, 0, sizeof(XMP_Uns8)*16 );
}

#endif
  	
/******************************************************************************/
  
void get_ieee_node_identifier(uuid_node_t* node)
  	{
  	static bool inited = false;
    static uuid_node_t saved_node = { {0,0,0,0,0,0} };
    
  	if (!inited) 
  		{
  		GetRandomAddress(&saved_node);
  		inited = true;
  		}
  		
  	*node = saved_node;
  	}

/******************************************************************************/

/* data type for UUID generator persistent state */
struct uuid_state
	{
    uuid_time_t ts;       /* saved timestamp */
    uuid_node_t node;     /* saved node ID */
    XMP_Uns16 cs;    /* saved clock sequence */
    };

static uuid_state st;
static bool stateInited = false;

/* read_state -- read UUID generator state from non-volatile store */
int read_state(XMP_Uns16 *clockseq, uuid_time_t *timestamp, uuid_node_t *node)
	{
	if (!stateInited) return (0);

	*clockseq	= st.cs;
	*timestamp	= st.ts;
	*node		= st.node;

	return(1);
	}

/* write_state -- save UUID generator state back to non-volatile storage */
void write_state(XMP_Uns16 clockseq, uuid_time_t timestamp, uuid_node_t node)
	{

    /* always save state to volatile shared state */
    st.cs	= clockseq;
    st.ts	= timestamp;
    st.node = node;
    
    stateInited = true;
  	}
  	
/******************************************************************************/
