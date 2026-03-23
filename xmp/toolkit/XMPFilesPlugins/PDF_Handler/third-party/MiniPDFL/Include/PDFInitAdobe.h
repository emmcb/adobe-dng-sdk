
/*
** PDFInitAdobe:  Adobe-private extensions to PDFL
** Copyright (C) 1996,1999,2003 Adobe Systems Inc.  All Rights Reserved.
**
 */
 
#ifndef _H_PDFInitAdobe
#define _H_PDFInitAdobe

#include "BIBInit.h"
#include "CT.h"
#include "PDFInit.h"
#if ACROBAT_LIBRARY || PDFL_EXTENSION
#include "PDFLPrivCalls.h"
#endif

#ifdef __cplusplus
extern "C" {
#endif

/* Internal Adobe initization structure containing 
   font context information. */
typedef struct _PDFLInitAdobeRec {
	/* Value returned from call to BIBHostInit or BIBGetProcAddress.
		This parameter may be NULL. If so, the value will be optained
		from BIB.*/
	BIBGetAddressProc bibAddressProc;
	/* Font set. This parameter may be NULL. */
	CTFontSet *fontSet;
} PDFLInitAdobeRec, *PDFLInitAdobeP;

/* PDFLInitFriends is an alternate method of initializing the PDF Library
	for internal use only. The standard initialization call, PDFLInit
	assumes that other CoreTech components including BIB, AGM, ACE, OPP,
	and CoolType have not been initialized. Many internal clients of the
	PDF Library make use of these components outside the library and wish
	to initialize them for their own purposes. In that case, PDFLInitFriends
	should be used.

	Prior to calling PDFLInitFriends, all the other CoreTech components listed
	above must be initialized. The first parameter to PDFLInitFriends, data,
	is the same as the parameter to PDFLInit. Refer to PDFInit.h for details
	on the parameter. The second parameter, fontData, provides additional
	initialization information that is only needed because the other CoreTech
	components have already been initialized. This parameter may be NULL if
	none of the values in the PDFLInitAdobeRec need to be specified.*/


ACEX1 ASInt32 ACEX2 PDFLInitFriends(PDFLData data,
                                    PDFLInitAdobeP fontData);

#ifndef PDFLTermFriends
	/* PDFLTermFriends must be used to terminate the PDF Library if it was 
	initialized with PDFLInitFriends. */
ACEX1 void ACEX2 PDFLTermFriends(void);
#endif

/* PDFLInitFriendsHFT and PDFLTermFriendsHFT function identically with
   PDFLInitFriends and PDFLTermFriends respectively except they also initialize
   the HFT mechanism to allow indirect calls into the library. These functions
   are implemented in client side code. PDFLTermFriendsHFT must be used to
   terminate the PDF Library if it was initialized with PDFLInitFriendsHFT.*/
ASInt32 PDFLInitFriendsHFT(PDFLData data,
                                    PDFLInitAdobeP fontData);

void PDFLTermFriendsHFT(void);

/* PDFLInitFriendsAndLoadHFT and PDFLTermAndUnloadFriendsHFT are the same as
   PDFLInitFriendsHFT and PDFLTermFriendsHFT except they load and unload the
   library. If these functions are used, the library should not be linked with
   PDFLxx.lib. These functions are also implemented in client side code.*/

ASInt32 PDFLInitAndLoadFriendsHFT(PDFLData data,
                                    PDFLInitAdobeP fontData);

void PDFLTermAndUnloadFriendsHFT(void);

enum {

	/** Exclude steps in PDFLib initialization not required for word Extraction
	*/
	kPDFLInitExcludeForWordExtraction = 0x1000
};

#ifdef __cplusplus
}
#endif

#endif /* _H_PEPCalls */

