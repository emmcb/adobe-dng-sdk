/**************************************************************************
*
* ADOBE CONFIDENTIAL
* ___________________
*
*  Copyright 1998 - 2007 Adobe Systems Incorporated
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

// Prefix Configuration File for public headers

#if defined (SC_MAC) && defined (__MWERKS__)
	#pragma push
	#pragma RTTI on
	#pragma pool_strings off
	#pragma dont_reuse_strings on 
	#pragma bool on
	#pragma enumsalwaysint off
	#if defined (__INTEL__)
		#pragma pack(2)
	#elif defined (__POWERPC__)
		#pragma options align=power
		#pragma readonly_strings on
	#endif
#elif defined (SC_MAC_XCODE)
	//#pragma pack(push,2)
	//#pragma options align=power
#elif defined (SC_WIN)
    #pragma warning( disable : 4103 4290 4511 4512 4514 4710 4786 4932 )
    #pragma pack( push, stdobjects_settings, 8 )
#elif defined (SC_AIX)
    #pragma report (disable,CPPC0802)
    #pragma report (disable,CPPC1608)
#endif

