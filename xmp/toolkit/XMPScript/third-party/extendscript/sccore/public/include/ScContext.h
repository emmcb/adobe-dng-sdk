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
*  Copyright 1998 - 2005 Adobe Systems Incorporated
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

#if SC_ONCE
#pragma once
#endif
#ifndef _ScContext_h
#define _ScContext_h

#include "ScRoot.h"
#include "ScPreConfig.h"

namespace ScScript
{
	class Engine;
	class ESContext;
}

namespace ScCore
{

class Encoder;
class Localizer;

/**
The modality dialog callback receives the engine that invoked the modal dialog (which may be null).
Currently, this callback is implemented by the Toolkit backend code to process BridgeTalk messages in
the background during a modal dialog, and the engine is used to determine whether a Break or Halt
request has been issued for that engine; if so, the routine returns false so the dialog can be
dismissed, and the Toolkit can take over control.
*/

typedef bool (*ModalDialogCallback) (ScScript::Engine* owningEngine);

/**
The context data block is thread local. It contains some thread local data
which you can read. There is room for own thread local data if desired. 
In a single-threaded system, there is only one context; it is associated with 
the main thread. Note that this class does not use any supplied allocators,
since it is not derived from Heap (although its data uses allocators).
*/

class SC_API Context
{
	/// Private copy ctor, because LockRef has pointer data members.
					Context (const Context&);
	/// Private assignment operator, because LockRef has pointer data members.
	Context& 		operator= (const Context&);

protected:
								Context();
								~Context();

	/** Context should not have a virtual destructor. Your class should derive Context with 'protected' and not public
		in order to prevent 'Context *ptrToContext = new ClassDerivedFromContext();'.
		Derived classes should also call Context::destructThis() in their destructor.
	*/
	void						destructThis();
	
public:
	/**
	Get the current context data. Create the data and associate it with the
	calling thread if it does not exist.
	*/
	static	Context&			get();
	/**
	The default encoder is stored here. This pointer is preset to the system encoder
	when the thread starts; you can change it by calling Encoder::setDefault().
	*/
	const Encoder* const		mDfltEncoder;
	/**
	The current localizer instance. Change this pointer by calling Localizer::set().
	*/
	const Localizer* const		mLocalizer;
	/**
	If your code is called from within an ExtendScript engine, the pointer to the
	engine that called your code is stored here.
	*/
	ScScript::Engine* const		mEngine;
	/**
	A pointer to user-defined thread local data. This pointer is initially null.
	You can store whatever data you want; the data is yours, so you are responsible
	for deleting the data. The thread destructor deletes this data, so you do not
	need to delete the data when you are finished.
	*/
	Root*						mUserData;
	/**
	If this variable is non-null, the environment is multithreaded. Do Not Use.
	*/
	void* const					mMultiThreaded;
	/**
	Thread local data for the ExtendScript environment. Do Not Use.
	*/
	ScScript::ESContext* const	mESContext;
	/**
	A callback routine that is called in about 20 msecs intervals during the
	execution of a modal dialog. Having this routine return false dismisses
	the dialog as if CANCEL had been clicked or the dialog window had been
	closed. This is true fo all ScCore dialogs as well as for other modal
	dialogs where a DialogModality instance has been created before they
	are executed.
	*/
	ModalDialogCallback			mModalDialogCallback;
};

}	// end namespace

#include "ScPostConfig.h"

#endif

