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

// Postfix Configuration File for public headers


#if defined (SC_MAC) && defined (__MWERKS__)
	#pragma pop
#elif defined(SC_MAC_XCODE)
	//#pragma pack(pop)
	//#pragma options align=reset
#elif defined (SC_WIN)
	#pragma pack( pop, stdobjects_settings )
#elif defined (SC_AIX)
	#pragma report (pop)
#endif


