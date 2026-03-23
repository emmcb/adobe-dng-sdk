// (c) Copyright 1999.  Adobe Systems, Incorporated.  All rights reserved.
//  $Id: $ 

#ifndef __ZUIDSYSDEP_h__
#define __ZUIDSYSDEP_h__

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
  
#include "public/include/XMP_Const.h"	// For safe fixed integer sizes.

  /* remove the following define if you aren't running WIN32 */
#ifdef WIN_ENV

#include <windows.h>

#else

//#include <Types.h>
#include <time.h>

#endif

  /* set the following to the number of 100ns ticks of the actual
  resolution of
  your system's clock */
  #define UUIDS_PER_TICK 10

  /* Set the following to a call to acquire a system wide global lock
  */
  #define STD_LOCK
  #define STD_UNLOCK

#ifdef XMP_OS_WINUNIVERSAL 
	#define MAX_COMPUTERNAME_LENGTH 15 
#endif //#ifdef XMP_OS_WINUNIVERSAL 

#if 1
  #define I64(C) C##ULL
#else
  /* Set this to what your compiler uses for 64 bit data type */
  #ifdef WIN_ENV
  #define unsigned64_t unsigned __int64
  #define I64(C) C
  #else
  #define unsigned64_t unsigned long long
  #define I64(C) C##LL
  #endif
#endif

  typedef XMP_Uns64 uuid_time_t;
  typedef struct {
    char nodeID[6];
  } uuid_node_t;

  extern void get_ieee_node_identifier(uuid_node_t *node);
  extern void get_system_time(uuid_time_t *uuid_time);
  extern void get_random_info(XMP_Uns8 seed[16]);
  
  extern int read_state(XMP_Uns16 *clockseq, uuid_time_t *timestamp,  uuid_node_t * node);
  extern void write_state(XMP_Uns16 clockseq, uuid_time_t timestamp, uuid_node_t node);

/******************************************************************************/

#endif
