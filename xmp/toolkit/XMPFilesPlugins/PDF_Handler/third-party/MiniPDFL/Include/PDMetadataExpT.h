/*********************************************************************

 ADOBE SYSTEMS INCORPORATED
 Copyright (C) 2000-2006 Adobe Systems Incorporated
 All rights reserved.

 NOTICE: Adobe permits you to use, modify, and distribute this file
 in accordance with the terms of the Adobe license agreement
 accompanying it. If you have received this file from a source other
 than Adobe, then your use, modification, or distribution of it
 requires the prior written permission of Adobe.

 ---------------------------------------------------------------------

 PDMetadataExpT.h

 - Types required to use the PDMetadata HFT.

*********************************************************************/

#ifndef _H_PDMETADATAExpT
#define _H_PDMETADATAExpT

#include "PDExpT.h" 

#ifdef __cplusplus
extern "C" {
#endif


/**
	Calculates implicit metadata. Clients that maintain metadata 
	items that have to be recalculated should register for the 
	PDDocCalculateMetadata() notification with this callback. 
	The callback should obtain the metadata with which it is 
	concerned, change it, and put the changed metadata back 
	in the object from which it was obtained. 
	@param pdDoc The document containing the metadata. 
	@param data User-supplied data.
	@notify PDDocCalculateMetadata 
*/
typedef ACCBPROTO1 void (ACCBPROTO2 *PDImplicitMetadataProc)(PDDoc pdDoc,
                                                             void * data);

/***********************************************************************
 * New in Acrobat 7
 */

/**
   Receives the notification that the XMP metadata describing a document
   as a whole has changed.

   @param pdDoc  The PDDoc whose describing XMP metadata has changed.
   @param newMetadata A serialized representation of the new describing XMP metadata.
   You do not own this.
   @param data Notification data provided at notification registration time.
   @notify PDDocXAPMetadataDidChange
 */
typedef ACCBPROTO1 void (ACCBPROTO2 *PDDocXAPMetadataDidChangeProc)(PDDoc pdDoc,
																	ASText newMetadata,
																	void * data);

/**
   Receives the notification that the XMP metadata describing an object
   represented by a Cos dictionary or stream has changed.

   @param dict The Cos dictionary or stream representing an object whose describing XMP metadata has changed.
   @param newMetadata A serialized representation of the new describing XMP metadata
   You do not own this.
   @param data Notification data provided at notification registration time.
   @notify CosDictXAPMetadataDidChange

 */
typedef ACCBPROTO1 void (ACCBPROTO2 *CosDictXAPMetadataDidChangeProc)(CosObj dict,
																	  ASText newMetadata,
																	  void * data);

/**
   Receives the notification that the XMP metadata describing a marked content
   sequence has changed.

   @param container The PDEContainer representing a marked content sequence
   whose describing XMP metadata has changed.
   @param newMetadata A serialized representation of the new describing XMP metadata
   You do not own this.
   @param data Notification data provided at notification registration time.
   @notify PDEContainerXAPMetadataDidChange

 */
typedef ACCBPROTO1 void (ACCBPROTO2 *PDEContainerXAPMetadataDidChangeProc)(PDEContainer container,
																		   ASText newMetadata,
																		   void * data);



#ifdef __cplusplus
}
#endif

#endif
