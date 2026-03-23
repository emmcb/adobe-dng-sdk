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

#if SC_ONCE
#pragma once
#endif
#ifndef _ScLiveBase_h
#define _ScLiveBase_h

#include "ScRefcountable.h"
#include "ScLockable.h"
#include "ScPreConfig.h"

namespace ScCore
{

class LivePropertyManager;
class Error;
class String;

/**
The scripting language types for name/ID resolution may not be combined.
*/

enum LanguageBits
{
	// the bits are chosen so they can coexist with property attributes and enum selection bits.
    kJavaScriptLanguageBit        = 16,
    kAppleEventEnvironmentBit     = 17,
    kCOMEnvironmentBit            = 18,
	kInternalEnvironmentBit		  = 19,

    /// JavaScript : Case sensitive name comparison
    kJavaScriptLanguage				= ( 1 << kJavaScriptLanguageBit ),
 
	/// AppleScript: Case insensitive name comparison
    kAppleEventEnvironment			= ( 1 << kAppleEventEnvironmentBit ),

    /// Windows COM: Case insensitive name comparison
    kCOMEnvironment				    = ( 1 << kCOMEnvironmentBit ),

	/// Internal usage: for Liveobject factories that are internal use only.
	kInternalEnvironment			= ( 1 << kInternalEnvironmentBit ),

	/// Mask for all language bits
	kLanguageMask					= 0x00FF0000,
	__LanguageBits_32bit__			= 0x7FFFFFFF
};

/**
The LiveBase class is the base class for LiveObjects and LiveComponents.
<p>
<b>Refcounting and invalidation:</b> Both LiveObjects and LiveComponents are reference
counted. When created, the count is one. addRef() increases the reference count, and
release() decreases it. When the reference count reaches zero, the object is deleted. 
<p>
<b>Inspection:</b> LiveObjects have an information interface implemented. 
The getClassInfo() call returns a pointer to a ClassInfo structure that 
can be used to implement an object browser or to display help information about 
an object, a method or a property.
<p>
<b>Error handling:</b> Most methods return an error code as defined in ScErrors.h. Valid
error codes are from 1 to 32767. If the code is negative, it means that the error is
fatal. Fatal errors cannot be caught by e.g. a JavaScript try/catch statement; they
always cause a script run to terminate. The base class has a few getErrorText() methods
that expect an error code and optional, additional information. The localizer object is
asked for the localized error message correspondng to the code, then an eventual "%1" character
sequence is replaced with the additional information.
<p>
<b>Thread safety:</b>By default, LiveObjects and LiveComponents are not thread safe. To create
a lock, call createLock(). To lock and unlock a LiveObject/LiveComponent, call lock() and
unlock(). These two methods do nothing if createLock() has not been called.
<p>
<b>Property management:</b> An optional LivePropertyManager instance can be set that takes
over the name/ID mapping functionaliy for properties in the default implementations of
LiveObject and LiveComponent. Using a property manager makes it unnecessary to implement
a lot of lookup and enumeration methods.
@see ClassInfo
@see Localizer
*/

class SC_API LiveBase : public Refcountable, public Lockable
{
	/// Private copy ctor, because LiveBase has pointer data members.
					LiveBase (const LiveBase&);
	/// Private assignment operator, because LiveBase has pointer data members.
	LiveBase& 		operator= (const LiveBase&);
protected:
	/// Dont construct a LiveBase directly.
								LiveBase();
	/// And never delete a LiveBase directly!
	virtual						~LiveBase();
	/// Return true if the given language has caseless comparisons.
	static	bool				isCaseless (scint32_t lang) 
		{ return ((kLanguageMask & lang & kCOMEnvironment) != 0); }
private:
	/// An optional PropertyManager to do the name/ID mapping for derived classes.
	const LivePropertyManager*	mMgr;
	/// The valid flag is true as scint32_t as invalidate() has not been called.
			bool				mValid;
public:
	/**@name Property management */
	//@{
	/**
	Set a Property Manager to be used for property lookup. The default
	name/ID mapping implementations use this manager for lookups. Initially,
	there is no manager.
	*/
	inline	const LivePropertyManager* getPropertyManager() const { return mMgr; }
	/**
	Set a new property manager. The instance does not belong to the LiveObject.
	If you allocate the instance on the heap, you must delete it yourself.
	*/
	inline	void				setPropertyManager (const LivePropertyManager* mgr) { mMgr = mgr; }
	//@}
	/**@name Class name and casting */
	//@{
	/**
	These basic implementations return null. See the LiveObject documentation
	for details.
	@see LiveObject::getAs
	*/
	virtual void*				getAs (scint32_t liveID);
	///
	virtual const void*			getAs (scint32_t liveID) const;
	//@}
	/**
	LiveObjects and LiveComponents may use dynamic IDs out of a reserved range. 
	This range is chosen so it is very unlikely to collide with predefined IDs 
	in either the AppleScript or COM worlds. There are a few different ranges 
	of dynamic IDs. The first range is used in the getNextID() method. These IDs 
	are guaranteed to be unique. Finally, ExtendScript reserves another 
	dynamic range. If a LiveObject has the flag bit kPropertiesMayRemap set,
	ExtendScript usually calls getType() for a property on each access. This
	is NOT true for predefined ExtendScript IDs.
	*/
	enum ReservedIDs : scint32_t {
		/// The first reserved ID that is used for dynamic properties.
		kFirstDynamicLiveID		= 0x1F800000,
		/// The last reserved ID that is used for dynamic properties.
		kLastDynamicLiveID		= 0x1FEFFFFF,
		/// The first ID reserved for LiveObjects.
		kFirstLiveObjectID		= 0x1FF00000,
		/// The last ID reserved for LiveObjects.
		kLastLiveObjectID		= 0x1FF0FFFF,
		/// The first ID reserved for ExtendScript.
		kFirstExtendScriptLiveID = 0x1FF10000,
		/// ExtendScript ID: Direct invocation of an object, e.g. call a constructor as a function.
		kJSDirectInvoke			= kFirstExtendScriptLiveID,
		/// Operator overload: unary negation ~
		kJSOverloadNeg			= kFirstExtendScriptLiveID + 0x100,
		/// Operator overload: unary not !
		kJSOverloadNot			= kJSOverloadNeg + 1,
		/// Operator overload: *
		kJSOverloadTimes		= kJSOverloadNot + 3,
		/// Operator overload: /
		kJSOverloadDiv			= kJSOverloadTimes + 1,
		/// Operator overload: %
		kJSOverloadMod			= kJSOverloadDiv + 1,
		/// Operator overload: + (unary + has no arguments, binary + has two)
		kJSOverloadPlus			= kJSOverloadMod + 1,
		/// Operator overload: - (unary - has no arguments, binary - has two)
		kJSOverloadMinus		= kJSOverloadPlus + 1,
		/// Operator overload: <<
		kJSOverloadShl			= kJSOverloadMinus + 1,
		/// Operator overload: >>
		kJSOverloadShr			= kJSOverloadShl + 1,
		/// Operator overload: >>>
		kJSOverloadUShr			= kJSOverloadShr + 1,
		/// Operator overload: <
		kJSOverloadLt			= kJSOverloadUShr + 1,
		/// Operator overload: <=
		kJSOverloadLe			= kJSOverloadLt + 1,
		/// Operator overload: in
		kJSOverloadIn			= kJSOverloadLe + 3,
		/// Operator overload: instanceof
		kJSOverloadInstanceof	= kJSOverloadIn + 1,
		/// Operator overload: ==
		kJSOverloadEqual		= kJSOverloadInstanceof + 1,
		/// Operator overload: ===
		kJSOverloadEquality		= kJSOverloadEqual + 2,
		/// Operator overload: &
		kJSOverloadAnd			= kJSOverloadEquality + 2,
		/// Operator overload: |
		kJSOverloadOr			= kJSOverloadAnd + 1,
		/// Operator overload: /
		kJSOverloadXor			= kJSOverloadOr + 1,
		/// The first operator overload ID
		kJSFirstOverload		= kJSOverloadNeg,
		/// The last operator overload ID
		kJSLastOverload			= kJSOverloadXor,
		/// The last ID reserved for ExtendScript.
		kLastExtendScriptLiveID = 0x1FF1FFFF,
		/// The first ID reserved for COM and AppleScript.
		kFirstCOMLiveID			= 0x1FF20000,
		/// The last ID reserved for COM and AppleScript.
		kLastCOMLiveID			= 0x1FF2FFFF,
		/// The first ID reserved for future use.
		kFirstReservedLiveID	= 0x1FF30000,
		/// The last ID reserved for future use.
		kLastReservedLiveID		= 0x1FFFFFFF,
		/// Reserved property ID: function to create a LiveObject. Used by clone().
		kLiveIDCreate			= kFirstLiveObjectID,
		/// Reserved property ID: find an EventTarget component.
		kLiveIDEventTarget		= kLiveIDCreate + 1,
	};
	/**
	Get the next dynamic ID out of the range between kFirstDynamicLiveID and
	kLsstDynamicLiveID. There is no wraparound; if the range is exhausted, debug
	builds will throw an assert, and the ID will overflow into the next range.
	Since there are 7340031 IDs available, this should be very unlikely.
	*/
	static	scint32_t				getNextID();
	/**
	Invalidate a LiveObject or LiveComponent. The object should, if appropriate,
	take care to invalidate its contents and/or to ignore any access to itself. 
	The default implementation clears the iValid member.
	*/
	virtual	void				invalidate();
	/// Is this instance valid?
	inline	bool				isValid() const { return mValid; }
	/**
	Re-validate a LiveObject or LiveComponent. Usually, an invalidated instance
	should remain invalid, but under certain circumstances, if e.g. the instance
	is cached, or if it reactivated during an Undo operation, it may make sense 
	to revive the object again.
	*/
	virtual	void				revalidate();
	/**
	Generate a text for the given property ID. For indexed, this is "[n]".
	If the property is unknown to the parent object, generate either 'xxxx'
	if the ID seems to be a four-byte ID, or "#n" for anything else.
	@param	text				takes the text
	@param	propertyID			the property ID
	@param	isIndex				if true, the property name is "[n]"
	@return						an error code (0 for no error)
	*/
			scerror_t			getPropertyText (String& target, scint32_t propertyID, bool isIndex=false) const;
	/**
	Retrieve the full, localized message for an error code. Merge in the name
	of the given property ID.
	@param	text				takes the text
	@param	errCode				the error code
	@param	propertyID			the property ID
	@param	isIndex				if true, the property name is "[n]"
	@return						an error code (0 for no error)
	*/
	virtual	scerror_t			getErrorText (String& target, scerror_t errCode, scint32_t propertyID, bool isIndex=false) const;
	/**
	Retrieve the full, localized message for an error code. Merge in the
	given additional error information if present.
	@param	text				takes the text
	@param	errCode				the error code
	@param	moreInfo			optional more info
	@return						an error code (0 for no error)
	*/
	virtual	scerror_t			getErrorText (String& target, scerror_t errCode, const String* moreInfo = NULL) const;
	/**
	Populate an error stack instance with the given error. This method
	calls getErrorText() with the supplied arguments and pushes the error
	into the error stack object if the error code is not kErrOK and the 
	error stack instance is not null and does not yet contain an error. 
	This is a utility function to convert simple error codes into extended 
	error information.
	@param	errCode				the error code
	@param	propertyID			the property ID
	@param	errors				the error stack instance (may be null)
	@param	isIndex				if true, the property name is "[n]"
	@return						the errCode argument
	*/
			scerror_t			setError (scerror_t errCode, scint32_t id, Error* errors, bool isIndex=false) const;
};

}	// end Namespace

#include "ScPostConfig.h"
#endif

