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
*  Copyright 1998 - 2004 Adobe Systems Incorporated
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
#ifndef _ScLiveProperty_h
#define _ScLiveProperty_h

#include "ScClassInfo.h"
#include "ScMessage.h"

#include "ScPreConfig.h"

namespace ScCore
{

/** LiveObjects use the LiveProperty class when creating a dynamic property.
The interface to create this property can be overloaded so you can create
specialized property classes that contain additional data.
*/

class LiveProperty : public Root
{
	friend class LiveObject;
	friend class LiveObjectData;

	/// Private assignment operator, because LiveProperty has pointer data members.
	LiveProperty&			operator= (const LiveProperty&);

private:
											// do not assume that this data will stay as is
	LiveObject&				mParent;		// the LiveObject owning this property
	LiveProperty*			mNext;			// the next property in the internal chain
	String					mName;			// the property name
	scint32_t					mOrdinal;		// for Javascript-conformant enumeration
protected:
	Variant					mValue;			// the property value
	scint32_t					mArgCount;		// argument count for methods (initially 0)
	scint32_t					mID;			// the property ID (modifiable by derived classes)
	scuint32_t				mFlags;			// creation flags (modifiable by derived classes)
	InfoState				mState;			// the state (used by getServiceInfo, modifiable)
							LiveProperty (const LiveProperty& r);
public:
	/**
	The constructor takes the parent, the name, the ID and the creation flags, which also contain the type.
	*/
							LiveProperty (LiveObject& parent, const String& name, scint32_t id, scint32_t flags);
	/**@name Comparison */
	//@{
	/** 
	Compare two properties by comparing their value.
	*/
	virtual	int				operator== (const LiveProperty& r) const;
	/**
	The less-than operator permits the property to live within STL containers.
	*/
	virtual	int				operator< (const LiveProperty& r) const;
	//@}
	/**@name Getters and setters */
	//@{
	///
			void			getName (String& target) const;
	///
	inline	scint32_t			getID() const { return mID; }
	///
	inline	InfoType		getType() const { return (InfoType) (mFlags & 0x0F); }
	//
	inline	scuint32_t		getFlags() const { return mFlags; }
	///
	inline	scint32_t			getParamCount() const { return mArgCount; }
	/// Set the arg count if you define this to be a method. For reflection only.
	inline	void			setArgCount (scint32_t argc) { mArgCount = argc; }
	/**
	Get the value of this property. Overload if necessary, and return an error code on errors.
	*/
	virtual	scerror_t		get (Variant& value) const;
	/**
	Set the value of this property. Overload if necessary, and return an error code on errors.
	*/
	virtual	scerror_t		put (const Variant& value);
	/**
	Invoke this property. You need to overload this method if you declare a property
	to be a callable method by supplying the kMethod IntoType to the creation flags.
	This implementation returns kErrNoFunction.
	*/
	virtual	scerror_t		call (const Array& args, Variant& retval);
};

}	// end Namespace

#include "ScPostConfig.h"
#endif

