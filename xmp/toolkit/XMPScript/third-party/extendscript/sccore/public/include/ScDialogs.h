/**************************************************************************
*
* ADOBE CONFIDENTIAL
* ___________________
*
*  Copyright 1998 - 2006 Adobe Systems Incorporated
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

#if SC_ONCE
#pragma once
#endif
#ifndef _ScDialogs_h
#define _ScDialogs_h

#include "ScHeap.h"
#include "ScPreConfig.h"

namespace ScCore
{

class String;
class Variant;

/**
The Dialogs class acts as a container for various dialogs and alerts.
Currently, dialogs are not implemented for Unix flavors. If called,
the dialog methods act as if the Cancel button was clicked. The versions
that have a "title" argument display the title if the OS permits so. 
On Mac OS X, alert() and confirm() do not display any title.
*/

class SC_API Dialogs
{
public:
	/// The definition of the enter/leave app modality callback.
	typedef	void			(*AppModalityCallback) (bool modal);
	/**
	On some UI environments, you need to surround all dialogs with calls that inform
	the application to enter and to leave a modal state. This static method allows
	you to define such a callback function. Null is an accepted value (and the default).
	It defines an application global callback.
	@param	callback		the callback to set
	@return					the old callback
	*/
	static	AppModalityCallback setAppModalityCallback (AppModalityCallback callback);
	/**
	Call the application modality set function in a thread-safe manner.
	Do Not Use this API anymore - please use the DialogModality API. 
	The method throws an assert in debug mode.
	@param	set				true to set the modal state
	*/
	static	void			setAppModality (bool set);
	/**
	Display an Alert box containing the given text and an OK button. On the Mac,
	the first line of the text is displayed as some sort of header.
	*/
	static	void			alert (const String& msg);
	///
	static	void			alert (const String& title, const String& msg);
	///
	static	void			alert (const String& title, const String& msg, bool errorIcon);
	/**
	Display an Confirm box containing the given text and OK and Cancel buttons. On the Mac,
	the first line of the text is displayed as some sort of header. Return true if OK is
	clicked and false if Cancel is clicked. If you set yesAsDefault to false, Cancel is 
	highlighted as the default choice.
	*/
	static	bool			confirm (const String& msg, bool yesAsDflt = true);
	///
	static	bool			confirm (const String& title, const String& msg, bool yesAsDflt = true);
	/**
	Display a dialog containing the given text, an edit field preset to the supplied
	text and OK and Cancel buttons. Return the text of the edit box in the variant if OK is
	clicked or set the variant to null if Cancel is clicked. 
	*/
	static	void			prompt (const String& msg, const String& preset, Variant& result);
	///
	static	void			prompt (const String& title, const String& msg, const String& preset, 
									Variant& result);
	/**
	Pick a color. The argument as well as the return value is a RGB color in the format
	0x00RRGGBB (hex). If the dialog was canceled, the return value is -1. If -1 is supplied
	as argument, no color is preselected.
	*/
	static	scint32_t			colorPicker (scint32_t color = -1);
};

class DialogModalityData;

/**
The DialogModality class is a helper class that you should instantiate immediately
before popping up a modal dialog. This class does some framework specific pre- and
postprocessing, and it defines a callback hook that enables the polling of the debugging
BridgeTalk queue even during the display of a modal dialog. This gives the Toolkit
the chance to inspect data, or even to break a script during the display of a modal
dialog. If you break a script during a modal dialog, the dialog is dismissed as if
the Cancel button has been clicked; if there is no Cancel button, the click of the OK
button is simulated; and if there is no OK button, the window is closed. Finally,
the class attempts to localize the buttons OK, Cancel, Yes and No using the defined
Localizer chain.
<p>
All of the built-in ScCore dialogs use this class already, so please do not use it together
with these dialogs, since the DialogModality class cannot do recursive initialization.
The Unix brands of ScCore do not have dialogs implemented, so this class does nothing.
*/

class SC_API DialogModality : public Heap
{
	/// Private copy ctor, because this class has pointer data members.
						DialogModality (const DialogModality&);
	/// Private assignment operator, because this class has pointer data members.
	DialogModality& 	operator= (const DialogModality&);

	DialogModalityData*	mData;
public:
						DialogModality();
						~DialogModality();
};

}	// end namespace

#include "ScPostConfig.h"

#endif

