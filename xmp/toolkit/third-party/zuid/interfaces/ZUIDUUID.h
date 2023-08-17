// (c) Copyright 1999.  Adobe Systems, Incorporated.  All rights reserved.
//  $Id: $ 

#ifndef __ZUIDUUID_h__
#define __ZUIDUUID_h__

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

struct uuid_data
	{
	XMP_Uns32	Data1;
	XMP_Uns16	Data2;
	XMP_Uns16	Data3;
	XMP_Uns8	Data4[8];
	};

/* uuid_create -- generate a UUID */
extern int uuid_create(uuid_data * uuid);

/* uuid_create_from_name -- create a UUID using a "name"
 from a "name space" */
extern void uuid_create_from_name(
uuid_data * uuid,        /* resulting UUID */
uuid_data nsid,          /* UUID to serve as context, so identical
                         names from different name spaces generate
                         different UUIDs */
XMP_Uns8 * name,          /* the name from which to generate a UUID */
XMP_Uns32 namelen           /* the length of the name */
);

/* uuid_compare --  Compare two UUID's "lexically" and return
      -1   u1 is lexically before u2
       0   u1 is equal to u2
       1   u1 is lexically after u2
 Note:   lexical ordering is not temporal ordering!
*/
extern int uuid_compare(const uuid_data *u1, const uuid_data *u2);

/******************************************************************************/

#endif
