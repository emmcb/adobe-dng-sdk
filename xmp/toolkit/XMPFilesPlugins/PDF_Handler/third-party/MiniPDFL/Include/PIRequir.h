/*********************************************************************

 ADOBE SYSTEMS INCORPORATED
 Copyright (C) 1994-2006 Adobe Systems Incorporated
 All rights reserved.

 NOTICE: Adobe permits you to use, modify, and distribute this file
 in accordance with the terms of the Adobe license agreement
 accompanying it. If you have received this file from a source other
 than Adobe, then your use, modification, or distribution of it
 requires the prior written permission of Adobe.

 ---------------------------------------------------------------------

 PIRequir.h

 - Used to determine which HFTs (and which versions of those HFTS)
   the plug-in requires.

 - The SDK provided version of this file provides minimum requirements
   to run on Acrobat 5.0.  If your plugin is designed to run on earlier
   versions you will need to lower the version numbers, while if your
   plugin requires routines from higher versions, you will need to raise
   the version numbers.  See the API Overview for details on how to build
   for multiple versions of Acrobat.

   To reduce the footprint and overhead of your plug-in, as
   well as provide for maximum future compatibility, we suggest that
   when your plug-in is "done" you edit this file to remove references
   to HFTs that are not being used. This is simply accomplished by
   setting the version number of the HFT to zero.

   For instance, if you don't use any of the Cos functions you should
   change the #define for PI_COS_VERSION to:

		#define PI_COS_VERSION		0

 - ALL plug-ins use one or more functions in the Core HFT. NEVER set
   PI_CORE_VERSION to zero.

*********************************************************************/

#ifndef _H_PIRequir
#define _H_PIRequir

#ifdef __cplusplus
extern "C" {
#endif

#include "Environ.h"


#ifdef MI_VERSN
#include MI_VERSN
#endif

/*
** Everybody needs the Core HFT, so don't omit this one.
*/
#ifndef PI_CORE_VERSION
/**
	Specifies the version of the HFT. This is automatically set by PIRequir.h.

	<p>If the HFT version is higher than the viewer loading the client supports, it displays an alert box with the message 
	"There was an error while loading the client <i><plug-in name></i>. The client is incompatible with this version of the Viewer."</p>

	@see PI_ACROVIEW_VERSION
	@see PI_ACROSUPPORT_VERSION
	@see PI_COS_VERSION
	@see PI_MACINTOSH_VERSION
	@see PI_PDMODEL_VERSION
	@see PI_UNIX_VERSION
	@see PI_WIN_VERSION
*/
#define PI_CORE_VERSION			CoreHFT_VERSION_5
#endif

/* 
** Most plug-ins will use calls from the AdobeSupport family 
*/
#ifndef PI_ACROSUPPORT_VERSION
/**
	Specifies the version of the Acrobat support level HFT. This is automatically set by PIRequir.h.

	<p>If the HFT version is higher than the viewer loading the client supports, it displays an alert box with the message 
	"There was an error while loading the client <i><plug-in name></i>. The client is incompatible with this version of the Viewer."</p>

	@see PI_ACROVIEW_VERSION
	@see PI_CORE_VERSION
	@see PI_COS_VERSION
	@see PI_MACINTOSH_VERSION
	@see PI_PDMODEL_VERSION
	@see PI_UNIX_VERSION
	@see PI_WIN_VERSION
*/
#define PI_ACROSUPPORT_VERSION	ASCallsHFT_VERSION_8
#endif

/* 
** Many plug-ins will not need to access the low-level Cos functionality 
*/
#ifndef PI_COS_VERSION
/**
	Specifies the version of the Cos-level HFT. This is automatically set by PIRequir.h.

	<p>If the HFT version is higher than the viewer loading the client supports, it displays an alert box with the message 
	"There was an error while loading the client <i><plug-in name></i>. The client is incompatible with this version of the Viewer."</p>

	@see PI_ACROVIEW_VERSION
	@see PI_CORE_VERSION
	@see PI_ACROSUPPORT_VERSION
	@see PI_MACINTOSH_VERSION
	@see PI_PDMODEL_VERSION
	@see PI_UNIX_VERSION
	@see PI_WIN_VERSION
*/
#define PI_COS_VERSION			CosHFT_VERSION_6
#endif

/* 
** PDModel methods 
*/
#ifndef PI_PDMODEL_VERSION
/**
	Specifies the version of the PD level HFT. This is automatically set by PIRequir.h.

	<p>If the HFT version is higher than the viewer loading the client supports, it displays an alert box with the message 
	"There was an error while loading the client <i><plug-in name></i>. The client is incompatible with this version of the Viewer."</p>

	@see PI_ACROSUPPORT_VERSION
	@see PI_CORE_VERSION
	@see PI_COS_VERSION
	@see PI_MACINTOSH_VERSION
	@see PI_ACROVIEW_VERSION
	@see PI_UNIX_VERSION
	@see PI_WIN_VERSION
*/
#define PI_PDMODEL_VERSION		PDModelHFT_VERSION_6
#endif

/* 
** AcroView methods 
*/
#ifndef PI_ACROVIEW_VERSION
/**
	Specifies the version of the Acrobat viewer level HFT. This is automatically set by PIRequir.h.

	<p>If the HFT version is higher than the viewer loading the client supports, it displays an alert box with the message 
	"There was an error while loading the client <i><plug-in name></i>. The client is incompatible with this version of the Viewer."</p>

	@see PI_ACROSUPPORT_VERSION
	@see PI_CORE_VERSION
	@see PI_COS_VERSION
	@see PI_MACINTOSH_VERSION
	@see PI_PDMODEL_VERSION
	@see PI_UNIX_VERSION
	@see PI_WIN_VERSION
*/
#ifdef PDFLPI
#define PI_ACROVIEW_VERSION		0
#else
#define PI_ACROVIEW_VERSION		AcroViewHFT_VERSION_6
#endif
#endif


/*
** ASExtra methods
*/
#ifndef PI_ASEXTRA_VERSION
#define PI_ASEXTRA_VERSION		ASExtraHFT_VERSION_6
#endif

/*
** AcroViewSweetPea methods
*/
#ifndef PI_ACROVIEW_SWEETPEA_VERSION
#if  defined(PDFLPI)
#define PI_ACROVIEW_SWEETPEA_VERSION            0
#else
#define PI_ACROVIEW_SWEETPEA_VERSION		SweetPeaHFT_VERSION_5
#endif
#endif

#ifndef PI_MACINTOSH_VERSION
#if MAC_PLATFORM
/* 
** Macintosh specific methods (AppleEvents, AVRect conversions, etc.) 
*/
/**
	Specifies the version of the Mac OS-only methods HFT. This is automatically set by PIRequir.h.

	<p>If the HFT version is higher than the viewer loading the client supports, it displays an alert box with the message 
	"There was an error while loading the client <i>&lt;plug-in name&gt;</i>. The client is incompatible with this version of the Viewer."</p>

	@see PI_ACROSUPPORT_VERSION
	@see PI_CORE_VERSION
	@see PI_COS_VERSION
	@see PI_ACROVIEW_VERSION
	@see PI_PDMODEL_VERSION
	@see PI_UNIX_VERSION
	@see PI_WIN_VERSION
*/
#define PI_MACINTOSH_VERSION		MacintoshHFT_VERSION_2_2
#else
#define PI_MACINTOSH_VERSION		(0L)
#endif
#endif

#if UNIX_PLATFORM
/**
	Specifies the version of the UNIX-only methods HFT. This is automatically set by PIRequir.h.

	<p>If the HFT version is higher than the viewer loading the client supports, it displays an alert box with the message 
	"There was an error while loading the client <i><plug-in name></i>. The client is incompatible with this version of the Viewer."</p>

	@see PI_ACROSUPPORT_VERSION
	@see PI_CORE_VERSION
	@see PI_COS_VERSION
	@see PI_ACROVIEW_VERSION
	@see PI_PDMODEL_VERSION
	@see PI_MACINTOSH_VERSION
	@see PI_WIN_VERSION
*/
#define PI_UNIX_VERSION UnixHFT_VERSION_5
#else
#define PI_UNIX_VERSION (0L)
#endif

#if WIN_PLATFORM
#ifndef PI_WIN_VERSION
/**
	Specifies the version of the Windows-only methods HFT. This is automatically set by PIRequir.h.

	<p>If the HFT version is higher than the viewer loading the client supports, it displays an alert box with the message 
	"There was an error while loading the client <i><plug-in name></i>. The client is incompatible with this version of the Viewer."</p>

	@see PI_ACROSUPPORT_VERSION
	@see PI_CORE_VERSION
	@see PI_COS_VERSION
	@see PI_ACROVIEW_VERSION
	@see PI_PDMODEL_VERSION
	@see PI_MACINTOSH_VERSION
	@see PI_UNIX_VERSION
*/
#define PI_WIN_VERSION WINHFT_VERSION_5
#endif
#else
#define PI_WIN_VERSION (0L)
#endif


/* 
** PDFEdit Write methods 
*/
#ifndef PI_PDFEDIT_WRITE_VERSION
#define PI_PDFEDIT_WRITE_VERSION		PDFEditWriteHFT_VERSION_6
#endif


/* 
** PDFEdit Read methods 
*/
#ifndef PI_PDFEDIT_READ_VERSION
#define PI_PDFEDIT_READ_VERSION		PDFEditReadHFT_VERSION_6
#endif

/* 
** PDSysFont methods 
*/
#ifndef PI_PDSYSFONT_VERSION
#define PI_PDSYSFONT_VERSION		PDSysFontHFT_VERSION_4
#endif

/* 
** PagePDEContent  methods 
*/
#ifndef PI_PAGE_PDE_CONTENT_VERSION
#define PI_PAGE_PDE_CONTENT_VERSION		PagePDEContentHFT_VERSION_6
#endif
	

#ifndef PI_PDSEDIT_WRITE_VERSION
#define PI_PDSEDIT_WRITE_VERSION	PDSWrite_VERSION_6
#endif

#ifndef PI_PDSEDIT_READ_VERSION
#define PI_PDSEDIT_READ_VERSION		PDSRead_VERSION_6
#endif

#if PDMETADATA_HFT 
               
#ifndef PI_PDMETADATA_VERSION
#define PI_PDMETADATA_VERSION       PDMetadataHFT_VERSION_6
#endif

#endif /* PDMETADATA_HFT */

#ifndef PI_ACROCOLOR_VERSION
#define PI_ACROCOLOR_VERSION		0 /* define to AcroColorHFT_VERSION_6 or later to get this (not available in Reader) */
#endif /* PI_ACROCOLOR_VERSION */

#ifdef __cplusplus
}
#endif

#endif /* _H_PIRequir */
