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

 PICommon.h

 - Globals and function declarations used by every plug-in.

*********************************************************************/

#ifndef _H_PICommon
#define _H_PICommon

#ifdef __cplusplus
extern "C" {
#endif

#ifndef PLUGIN
#define PLUGIN 1
#endif

#include "PIVersn.h"
#include "PIRequir.h"	/* PIRequir.h may be edited by authors to reduce Plug-in's footprint */
extern ExtensionID gExtensionID;


/* 
** Protype for PIHandshake(), which  must be provided by the plug-in author.
*/
extern ACCB1 ASBool ACCB2 PIHandshake(ASUns32 handshakeVersion, void *handshakeData);

/*
** Backwards compatibility section.
These macros reflect early API architecture. Callback-related things ended up part of the AS
mechanism. The other three macros are miscellaneous convenience macros. 
*/
#define CallbackCreate(a,b) ASCallbackCreate(b)
#define CallbackCreateProto(funcType, proc) ASCallbackCreateProto(funcType, proc)
#define CallbackCreateReplacement(sel, proc) ASCallbackCreateProto(sel##PROTO, proc)
#define CallbackCreateNotification(nsel, proc)	ASCallbackCreateProto(nsel##NPROTO, proc)
#if PI_ACROSUPPORT_VERSION >= 0x00050000
/**
	Replaces one of the Acrobat viewer's built-in methods. The method being replaced must be one of the ReplaceableMethods. 
	The method's HFTEntryReplaceable flag is automatically set, allowing it to be subsequently replaced.

	<p>All clients, and the Acrobat viewer itself, share a single copy of each HFT. As a result, when a client replaces the 
	implementation of a method, all other clients and the Acrobat viewer also use the new implementation of that method. 
	In addition, once a method's implementation has been replaced, there is no way to remove the new implementation without restarting the Acrobat viewer.</p>

	@note The CALL_REPLACED_PROC macro is available to call the previous HFT entry function that was replaced.

	@param hft		The HFT containing the method to replace. For example, gAcroViewHFT.
	@param sel		The selector for the method to replace. The name must have the characters <code>SEL</code> appended. For example, <code>AVAlertSEL</code>.
	@param proc		A callback containing the replacement method. The callback must have been created using ASCallbackCreateReplacement().

	@example	<code>myAlertCallback = ASCallbackCreateReplacement(AVAlertSEL, myAlert);
				<p>REPLACE(gAcroViewHFT, AVAlertSEL, myAlertCallback);</p></code>

	@see CALL_REPLACED_PROC
	@see ASCallbackCreateReplacement
	@see HFTReplaceEntry
	@ref ReplaceableMethods
*/
#define REPLACE(hft, sel, proc) HFTReplaceEntryEx((hft), sel, (proc), gExtensionID, HFTEntryReplaceable)
#else
#define REPLACE(hft, sel, proc) HFTReplaceEntry((hft), sel, (proc), HFTEntryReplaceable)
#endif
/**
	Calls the previous implementation of a replaced method (that is, the code that would have been executed before the method was replaced 
	using <code>REPLACE</code>).

	@param hft		The HFT containing the replaced method to execute. For example, gAcroViewHFT.
	@param sel		The selector for the replaced method to execute. The name must have the characters <code>SEL</code> appended. 
					For example, <code>AVAlertSEL</code>.
	@param oldProc	The callback whose previous implementation is called. A method may be replaced more than once, and all replacements 
					for a particular method are kept in a linked list. <code>oldProc</code> must be an element in that linked list, 
					and the entry before <code>oldProc</code> is the one that is called.

	@example	<code>CALL_REPLACED_PROC(gAcroViewHFT, AVAlertSEL, myAlertCallback)(iconType, gsm, button1, button2, button3, beep);</code>

	@see REPLACE
	@see HFTGetReplacedEntry
*/
#define CALL_REPLACED_PROC(hft, sel, oldProc) (*((sel##PROTO)(HFTGetReplacedEntry((hft), sel, (oldProc)))))

/* assuming most people won't cast the second parameter... */
/**
	A macro to register a notification. It uses AVAppRegisterNotification().
*/
#define REGISTER_NOTIFICATION(nselName, proc, rock) AVAppRegisterNotification(nselName##NSEL, gExtensionID, (void*)(proc), (rock))

#ifdef __cplusplus
}
#endif

#endif /* _H_PICOMMON */
