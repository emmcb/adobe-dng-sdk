// (c) Copyright 1999.  Adobe Systems, Incorporated.  All rights reserved.
//  $Id: $ 

// Win can't see directives placed before pre-comp headers
 
#include "third-party/zuid/interfaces/ZUIDUUID.h"

#include "third-party/zuid/interfaces/ZUIDSysDep.h"
#include "third-party/zuid/interfaces/MD5.h"

#include <cassert>
#include <cstring>
#include <cstdlib>

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

  /* various forward declarations */
  
  static void format_uuid_v1(uuid_data * uuid, XMP_Uns16 clockseq, uuid_time_t timestamp, uuid_node_t node);
  static void format_uuid_v3(uuid_data * uuid, XMP_Uns8 hash[16]);
  static void get_current_time(uuid_time_t * timestamp);
  static XMP_Uns16 true_random(void);

  /* uuid_create -- generator a UUID */
  int uuid_create(uuid_data * uuid)
  	{
    uuid_time_t timestamp, last_time;
    XMP_Uns16	clockseq;
    uuid_node_t	node;
    uuid_node_t	last_node;
    int f;

    /* get current time */
    get_current_time(&timestamp);

    /* get node ID */
    get_ieee_node_identifier(&node);

    /* get saved state from NV storage */
    f = read_state(&clockseq, &last_time, &last_node);

    /* if no NV state, or if clock went backwards, or node ID changed
       (e.g., net card swap) change clockseq */
       
    if (!f || memcmp(&node, &last_node, sizeof(uuid_node_t)))
        clockseq = true_random();
    else if (timestamp < last_time)
        clockseq++;

    /* stuff fields into the UUID */
    format_uuid_v1(uuid, clockseq, timestamp, node);

    /* save the state for next time */
    write_state(clockseq, timestamp, node);

    return(1);
  };

  /* format_uuid_v1 -- make a UUID from the timestamp, clockseq,
                       and node ID */
void format_uuid_v1(uuid_data * uuid, XMP_Uns16 clock_seq, uuid_time_t timestamp, uuid_node_t node)
	{
	// Construct a version 1 uuid with the information we've gathered plus a few constants.
	
    uuid->Data1 = (XMP_Uns32)(timestamp & 0xFFFFFFFF);
	uuid->Data2 = (XMP_Uns16)((timestamp >> 32) & 0xFFFF);
	uuid->Data3 = (XMP_Uns16)((timestamp >> 48) & 0x0FFF);
	uuid->Data3 |= (1 << 12);
	uuid->Data4[1] = XMP_Uns8(clock_seq & 0xFF);
	uuid->Data4[0] = XMP_Uns8((clock_seq & 0x3F00) >> 8);
	uuid->Data4[0] |= 0x80;
	assert ( sizeof(uuid->Data4) >= 8 );
	memcpy ( &uuid->Data4[2], &node, 6 );	// AUDIT: Safe, Data4 is char[8].
	};
  
/*
get-current_time -- get time as 60 bit 100ns ticks since whenever.
Compensate for the fact that real clock resolution is
less than 100ns.
*/

// NOTE (SRP) : This code has been modified for ZUIDs. The code used
// to throttle on the clock and be limited to UUIDS_PER_TICK. The new
// code will increment first and reset on the clock every UUIDS_PER_TICK.
  
void get_current_time(uuid_time_t * timestamp)
	{
	static uuid_time_t  	time_last;
    static XMP_Uns16 		uuids_this_tick = 0;
    static bool             inited			= false;

    if (!inited)
    	{
       	get_system_time(&time_last);
        inited = true;
    	};

  	if (uuids_this_tick < UUIDS_PER_TICK)
  		{
        ++uuids_this_tick;
  		}
  	else
  		{
		uuid_time_t	time_now;
		
		get_system_time(&time_now);

    	// if the clock is outrunning the counter
    	
      	if (time_now > uuid_time_t(time_last + uuids_this_tick))
      		{
         	// reset count of uuids gen'd with this clock reading
         	uuids_this_tick	= 0;
         	time_last		= time_now;
    		}
    	else
    		{
    		++uuids_this_tick;
    		}
    	}

    /* add the count of uuids to low order bits of the clock reading */
    *timestamp = time_last + uuids_this_tick;
  };

  /* true_random -- generate a crypto-quality random number.
     This sample doesn't do that. */
  static XMP_Uns16
  true_random(void)
  {
    static int inited = 0;
    uuid_time_t time_now;

    if (!inited) {
        get_system_time(&time_now);
        time_now = time_now/UUIDS_PER_TICK;
        srand((XMP_Uns32)(((time_now >> 32) ^ time_now)&0xffffffff));
        inited = 1;
    };

      return (XMP_Uns16(rand()));
  }

  /* uuid_create_from_name -- create a UUID using a "name" from a "name space" */
  void uuid_create_from_name(
    uuid_data * uuid,        /* resulting UUID */
    uuid_data nsid,          /* UUID to serve as context, so identical
                             names from different name spaces generate
                             different UUIDs */
    XMP_Uns8* name,          /* the name from which to generate a UUID */
    XMP_Uns32 namelen           /* the length of the name */
  ) {
    MD5_CTX c;
    XMP_Uns8 hash[16];
    uuid_data net_nsid;      /* context UUID in network byte order */

    /* put name space ID in network byte order so it hashes the same
        no matter what endian machine we're on */
    net_nsid = nsid;
#if 0	// *** XMP doesn't care?
    net_nsid.Data1 = PSHostToNetLong(net_nsid.Data1);
    net_nsid.Data2 = PSHostToNetShort(net_nsid.Data2);
    net_nsid.Data3 = PSHostToNetShort(net_nsid.Data3);
#endif

    MD5Init(&c);
    MD5Update(&c, (XMP_Uns8*)&net_nsid, sizeof(uuid_data));
    MD5Update(&c, name, namelen);
    MD5Final(hash, &c);

    /* the hash is in network byte order at this point */
    format_uuid_v3(uuid, hash);
  };

  /* format_uuid_v3 -- make a UUID from a (pseudo)random 128 bit number
  */
  void format_uuid_v3(uuid_data * uuid, XMP_Uns8 hash[16]) {
      /* Construct a version 3 uuid with the (pseudo-)random number
       * plus a few constants. */

      assert ( sizeof(*uuid) == sizeof(uuid_data) );
      memcpy ( uuid, hash, sizeof(*uuid) );	// AUDIT: Safe, using sizeof destination.

#if 0	// *** XMP doesn't care?
    /* convert UUID to local byte order */
    uuid->Data1 = PSNetToHostLong(uuid->Data1);
    uuid->Data2 = PSNetToHostShort(uuid->Data2);
    uuid->Data3 = PSNetToHostShort(uuid->Data3);
#endif

    /* put in the variant and version bits */
      uuid->Data3 &= 0x0FFF;
      uuid->Data3 |= (3 << 12);
      uuid->Data4[0] &= 0x3F;
      uuid->Data4[0] |= 0x80;
  };

  /* uuid_compare --  Compare two UUID's "lexically" and return
         -1   u1 is lexically before u2
          0   u1 is equal to u2
          1   u1 is lexically after u2

  Leach, Salz              expires  Aug 1998                   [Page 22]


  Internet-Draft        UUIDs and GUIDs (DRAFT)                 02/04/98


      Note:   lexical ordering is not temporal ordering!
  */
int uuid_compare(const uuid_data *u1, const uuid_data *u2)
  {
    int i;

  #define CHECK(f1, f2) if (f1 != f2) return f1 < f2 ? -1 : 1;
  
    CHECK(u1->Data1, u2->Data1);
    CHECK(u1->Data2, u2->Data2);
    CHECK(u1->Data3, u2->Data3);
    for (i = 0; i < 8; ++i)
    	{
  		CHECK(u1->Data4[i], u2->Data4[i]);
      	}
    return 0;
  };


