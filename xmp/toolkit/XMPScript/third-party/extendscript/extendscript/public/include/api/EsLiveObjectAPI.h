/**************************************************************************
*
* ADOBE CONFIDENTIAL
* ___________________
*
*  Copyright 1998 Adobe Systems Incorporated
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
#ifndef _EsLiveObjectAPI_h
#define _EsLiveObjectAPI_h

#include "ExtendScript.h"
#include "ScPreConfig.h"

namespace ScCore
{
	class XML;
}

namespace ScScript
{

/// The name of this API

const scint32_t kLiveObjectAPI = 0x6C697665; // 'live'

/**
The LiveObject API provides a method to access the global LiveObject.
*/

class ES_API LiveObjectAPI : public API
{
	friend class RealEngine;
								LiveObjectAPI (Engine&);
public:
	/**
	Retrieve the global LiveObject. On every engine reset, the global object's
	reset() method is called, erasing all dynamic properties and methods. Any
	LiveComponents that you add will remain. The default LiveObject is a simple
	LiveObject that has the class name "global".
	*/
			ScCore::LiveObject&	getGlobalObject() const;
	/**
	Set the global LiveObject. You can set a LiveObject of your choice to be the
	global object. This object will not be populated with the usual JavaScript stuff
	like e.g. the String constructor or the Math object, since the global LiveObject
	will be behind the JavaScript stuff. For this reason, you cannot redefine any of
	the built-in JavaScript objects by redefining them in the global LiveObject.
	@param	global		the new global LiveObject
	*/
			void				setGlobalObject (ScCore::LiveObject& global) const;
	/**
	Get the embedded XML of an XML object. This method returns NULL if XML is disabled.
	@param	arg					a Variant containing a LiveObject
	@return						a XML pointer or NULL.
	*/
			ScCore::XML*		getXML (const ScCore::Variant& val) const;
	/**
	Create an XML object from existing XML. Do not use XML returned from getXML(), and
	remember to release the XML after use. If XML is disabled, this method sets the
	variant to NULL.
	@param	xml					the XML to embed
	@param	arg					the variant to set
	*/
			void				makeXMLObject (const ScCore::XML& xml, ScCore::Variant& val) const;
};

/**
The GlobalDialogs component is a LiveComponent containing additional UI functionality.
The component contains the following three functions:
<ul>
<li>alert() - display an alert box with a message.
<li>confirm() - display a question and return true for Yes or false for No.
<li>prompt() - display a message and return the reply or NULL if Cancel was clicked.
</ul>
If you want to add these functions to the global namespace, retrieve the global object
by calling LiveObjectAPI::getGlobalobject(), and add a GlobalDialogs instance to that
global object. You can use the same component for all global objects if you like.
To enable the above functions, do the following:
<pre>
ScCore::LiveObject& global = liveObjectAPI->getGlobalObject();
ScCore::LiveComponent* c = ScScript::new GlobalDialogs;
global.addComponent (*C);
c->release();
</pre>
*/

class ES_API GlobalDialogs : public ScCore::LiveComponent
{
	ScCore::LivePropertyManager	mManager;
public:
	/** Resource definitions for alert(), confirm() and prompt()
	<br>{ "alert",		kAlertID,		METHOD, "undefined,prompt:string" },
	<br>{ "confirm",	kConfirmID,		METHOD, "boolean,prompt:string"   },
	<br>{ "prompt",		kPromptID,		METHOD, "string,prompt:string"    },
	*/
	enum
	{
		/// alert()
		kAlertID			= 0x4553414C, // 'ESAL'
		/// confirm()
		kConfirmID			= 0x4553434F, // 'ESCO'
		/// prompt()
		kPromptID			= 0x45535052  // 'ESPR'
	};
	/// The internal property table for inclusion in static dictionaries.
	static	const ScCore::LivePropertyInfo& getPropertyTable();
								GlobalDialogs();
	virtual	scerror_t			call (ScCore::LiveObject& thisObj, scint32_t id, 
								      const ScCore::Array& args, ScCore::Variant& result, 
									  ScCore::Error* errors = NULL);
};

}	// end namespace

#include "ScPostConfig.h"

#endif

