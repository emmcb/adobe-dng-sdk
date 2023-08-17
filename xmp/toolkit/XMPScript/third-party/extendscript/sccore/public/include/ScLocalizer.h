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
#ifndef _ScLocalizer_h
#define _ScLocalizer_h

#include "ScRoot.h"
#include "ScPreConfig.h"

namespace ScCore
{

class String;
class Array;

/**
The localizer interface is the central instance for localization purposes. All ScCore
classes that need to localize strings use this class. There is one global instance that
you can get with the static get() method. To implement your own localization, overload
this class and set an instance of your new localizer so every class can use it.
<p>
Behind the get/set interface, there is a chain of localizers, which you can access with
push/pop methods. The push() method adds a localizer to the top of the chain, while
pop() removes it from the top of the chain. If the current localizer cannot satisfy
the request, the localizer chain is traversed. The chain, as well as the current localizer,
is thread local.
<p>
The localizer as well as ScCore and ExtendScript support ZStrings. Therefore, the routine
that you want to overload is the getZString() method. For a list of localizable ZStrings,
see technote ES0006.
<p>
The localize() method does a two-step translation. First, it fetches the localized string 
by calling getZString() if the string is a ZString. In the second step, it uses a supplied 
array of character constants or Variant instances to replace every occurrence of the "%" 
character, followed by a number from 1 on, by the respective character constant. The string 
"%1" is replaced with the first element in the array, "%2" with the second and so on. To 
supply the percent sign as such, use "%%". If the character(s) behind the "%" character
are not numeric, the entire sequence if left unchanged, like e.g. "%x".
<p>
ExtendScript engines may have their own localizer. If this is so, the engine switches the
current localizer to that localizer before exectuting a script. After the run, the engine 
switches back to the previous localizer.
*/

class SC_API Localizer : public Root
{
public:
	// //////////////////////////// Static methods ///////////////////////////

	/**@name Static methods */
	//@{
	/**
	Get the current localizer instance. This instance is global for all ScCore and
	ExtendScript classes. It is thread local and part of the Context object, and it
	has precedence over the localizer lookup chain.
	*/
	static	const Localizer&	get();
	/**
	Get the default localizer instance. This instance is global for all ScCore and
	ExtendScript classes. Do Not Attempt to delete this instance. The instance is
	global for all threads.
	*/
	static	const Localizer&	getDefault();
	/**
	Set the current localizer instance. This instance is global for all ScCore and
	ExtendScript classes. Using null as the argument resets the current localizer to 
	the default localizer. The localizer remains in the ownership of the caller; ScCore
	will not delete it. The location is thread local and part of the Context object.
	*/
	static	void		set (const Localizer* localizer);
	/**
	Push a localizer to the top of the lookup chain. The chain is thread local and kicks
	in if the current localizer cannot fulfill the request. The localizer remains in the
	ownership of the caller; ScCore will not delete it.
	@param	localizer	the localizer to add
	*/
	static	void		push (const Localizer& localizer);
	/**
	Pop a localizer off the lookup chain. The chain is thread local and kicks
	in if the current localizer cannot fulfill the request.
	@return				the popped localizer or null if the chain was empty
	*/
	static	const Localizer* pop();
	/**
	Remove a localizer from the lookup chain. The chain is thread local and kicks
	in if the current localizer cannot fulfill the request. Return true if the
	localizer was removed.
	*/
	static	bool		remove (const Localizer& toRemove);
	/**
	ZStrings support: This method collects the current locale either from the current
	ExtendScript engine or from a call to the current localizer's getLocale() method.
	If an ExtendScript engine is running, it also collects the content of $.zstringInfo.
	Finally, it calls the virtual method getZString() with all of these arguments to
	do the actual localization.
	@param	zstring		the ZString to localize
	*/
	static	void		getZString (String& zstring);
	/**
	This method calls localize() for the current localizer and walks the lookup chain
	the method returned false until a chained localizer returns true.
	@param	text		the string to localize; takes the localized string
	@param	args		the arguments array
	@return				the localized text
	*/
	static	String		localize (const String& string, const Array* args = NULL);
	/**
	This method calls localizedCaseConvert() for the current localizer and walks the lookup chain
	the method returned false until a chained localizer returns true.
	@param	text		the text to convert
	@param	lower		true for converting to lower case, false for upper case
	@return				the converted text
	*/
	static	String		caseConvert (const String& text, bool toLower);
	/**
	This method calls localizedCompare() for the current localizer and walks the lookup chain
	the method returned false until a chained localizer returns something else than 0x7FFFFFFF.
	@param	first		the first string
	@param	second		the second string
	@return				negative if first > second, 0 if first == second, positive if first < second;
						return 0x7FFFFFFF if you cannot compare
	*/
	static	int			compare (const String& first, const String& second);
	/**
	This method calls getErrorMsg() for the current localizer and walks the lookup chain
	the method returned false until a chained localizer returns true.
	@param	code		the error code to get the message for
	@param	text		takes the text of the message
	@return				true if the text has a valid error message filled in
	*/
	static	bool		getErrorMessage (scerror_t code, ScCore::String& text);
	/**
	This method calls getErrorMsg() for the current localizer and walks the lookup chain
	the method returned false until a chained localizer returns true.
	*/
	static	String		getLocale();
	//@}

	// //////////////////////////// Overridable methods ///////////////////////////
protected:
	/**@name Overridable methods */
	//@{
	/**
	Translate the given string to your likings. Return true if translated. The static
	version of localize() calls this method if the string to be translated is not a ZString.
	The default implementation returns false.
	@param	text		the string to localize
	@return				true if translated
	*/
	virtual	bool		localizeText (String& zstring) const;
	/**
	Convert the given string to either upper or lower case. The method is called by
	the static method caseConvert(). Return true if converted. The default implementation 
	returns false.
	@param	text		the text to convert
	@param	lower		true for converting to lower case, false for upper case
	@param				true if the conversion was successful
	*/
	virtual	bool		localizedCaseConvert (String& text, bool toLower) const;
	/**
	Convert the given string to either upper or lower case. The method is called by
	the static method compare(). Return 0x7FFFFFFF if not comparable. The default 
	implementation return 0x7FFFFFFF.
	@param	first		the first string
	@param	second		the second string
	@return				negative if first > second, 0 if first == second, positive if first < second;
						return 0x7FFFFFFF if you cannot compare
	*/
	virtual	int			localizedCompare (const String& first, const String& second) const;
	/**
	Get a localized error message. Use this interface if you want the app to provide the
	error message. If e.g. your LiveObjects return an app specific error code, you could
	either define these errors using the Error::defineError() method, or you could
	overload this method to return the English or localized text for the given message.
	Especially apps with a large number of own error codes may want to overload this method
	instead of defining all of their private errors. If there is a text for this message,
	either supplied during Error::push() or an Error constructor, or if there is an error
	text registered for this error, the text argument is pre-filled with that message,
	formatted as a ZString.	The message returned can be a ZString. If so, ScCore would 
	call getZString to localize that message. The default implementation returns false.
	The method is called by the static method getErrorMessage().
	@param	code		the error code to get the message for
	@param	text		takes the text of the message
	@return				true if the text has a valid error message filled in
	*/
	virtual	bool		getErrorMsg (scerror_t code, ScCore::String& text) const;
	/**
	ZStrings support: if ScriptUI or the JavaScript function localize() is called with a ZString,
	it calls this method to have the ZString translated. Overload this method to forward
	the string to your ZStrings library. All ScCore/ExtendScript strings are also ZStrings now.
	A ZString has the form "$$$/specifier=english text". The default implementation just
	strips the first part up to and including the "=", stores the second part into the argument
	and returns true if there is no lookup chain; otherwise, it return false.
	<p>
	The second argument is the current locale identifier in ISO 3166 notation. 
	If a scripter has set this value by assigning it to $.locale, this value is used; otherwise, 
	the result of a call to Localizer::getLocale() is used. The third argument is the contents 
	of the ExtendScript	variable $.zstringInfo. If ExtendScript is not active, or if the 
	variable is undefined, this string is empty. 
	@param	zstring		Replace this string with the localized string
	@param	locale		the current locale ID
	@param	info		the content of the variable $.zstringInfo if called from ExtendScript
	@return				true if the ZString was localized; the default implementation returns false
	*/
	virtual	bool		getZString (String& zstring, const String& locale, const String& info) const;
	/**
	Get the current locale string as ISO-3166, up to 5 chars, like e.g. "en_US" or "de".
	The default implementation returns the OS locale. Note that the OS locale is coerced
	into an Adobe supported locale. The locale for Swiss German, de_CH, is e.g. coerced to
	de_DE. Also note that on Mac OS systems prior to 10.3, the locale returned is dependent
	on the presence of a locale.lproj file in the app bundle. To support a multilingual
	app running in e.g. French if the user selected French as the default language, either
	a folder fr.lproj or fr_FR.lproj has to be present. The folder can be empty. On Mac
	OS 10.3 and above, this is not longer necessary.
	*/
	virtual	bool		getLocale (String& result) const;
	//@}
};

}	// end Namespace

#include "ScPostConfig.h"

#endif

