/*************************************************************************
* WARNING: All additions to this file must go at the end of the file.
* This file is #included to build a table of function pointers (an HFT).
* All components which make use of this HFT rely on the order and
* positioning of these functions (e.g. plug-ins). Adding entries in the 
* middle of this file or modifying existing entries will cause these 
* components to break.
**************************************************************************/

/*
** AcroColorProcs.h
**
** (c) Copyright 2002,2004,2006 Adobe Systems, Inc.  All Rights Reserved.
** Exposure to ACE functions
** 
*/

#if !EXTERNAL_ACROCOLORPROCS_USER  /* External user of this header file, e.g. PISDK */
#if CAN_EDIT && !READER /* Restore X Macros -- was CAN_EDIT */
#define XNPROC(returnType, name, params) NPROC(returnType, name, params)
#define XPROC(returnType, name, params) PROC(returnType, name, params)
#define XSPROC(returnType, name, params, stubProc) SPROC(returnType, name, params, stubProc)
#else
#define XNPROC(returnType, name, params) NOPROC(name)
#define XPROC(returnType, name, params) NOPROC(name)
#define XSPROC(returnType, name, params, stubProc) NOPROC(name)
#endif /* CAN_EDIT && !READER */

#endif /* EXTERNAL_AcroColorPROCS_USER */

/**
	Gets the number of Color Management System/Color Management 
	Module (CMS/CMM) choices available for the AcroColor engine 
	(ACE). 
	@param count (Filled by the method) A pointer to the count 
	value.
	@return <code>0</code> if successful, a non-zero error code otherwise. 
	@see ACEngineInfo 
	@see ACSetEngine 
*/
XNPROC(AC_Error, ACEngineCount, (ASUns32 *count))

/**
	Gets information for a CMS/CMM in the AcroColor engine (ACE) 
	by index. The CMS and CMM identifiers specify an engine 
	to the ACE. Engine names should only 
	be used as the text for popup menus. It is better to store 
	the identifiers in settings files (rather than names), because 
	they are independent of localization. 
	@param index The zero-based index of the CMS/CMM. The highest 
	legal value is <code>AC_EngineCount - 1</code>. 
	@param name (Filled by the method) Optional. If it is not <code>NULL</code>, 
	the parameter returns the name of the CMS/CMM. 
	@param cmsID (Filled by the method) Returns the CMS identifier. 
	
	@param cmmID (Filled by the method) Returns the CMM identifier.
	@return <code>0</code> if successful, a non-zero error code otherwise. 
	@see ACEngineCount 
	@see ACSetEngine 

*/
XNPROC(AC_Error, ACEngineInfo, (ASUns32 index, AC_String *name, ASUns32 *cmsID, ASUns32 *cmmID))

/**
	Sets the AcroColor engine (ACE) for the system, changing 
	the global default CMS/CMM choice. 

	<p>This method rebuilds all existing transforms using the new 
	engine. </p>

	<p>If the user aborts the process, or if the ACE runs out of 
	resources during the rebuilding process, an error code is 
	returned and some existing transforms may still use the 
	previous engine choice. Everything will still work, since 
	multiple engines can be used at once. Call the method again 
	to restart the transform rebuilding process. </p>

	@param cmsID The Color Management System identifier 
	for the new engine default, as returned by ACEngineInfo(). 
	
	@param cmmID The Color Management Module identifier 
	for the new engine default, as returned by ACEngineInfo().
	@return <code>0</code> if successful, a non-zero error code otherwise. 
	@see ACEngineCount 
	@see ACEngineInfo 
*/
XNPROC(AC_Error, ACSetEngine, (ASUns32 cmsID, ASUns32 cmmID))


/**
	Creates a list of device color profiles of a given type. 
	
	<p>Builds a list of those profiles from the database that meet 
	the criterion of the specified selector. If the profile 
	database has never been built, it will be automatically 
	built without a progress callback. Clients should call 
	ACUnReferenceProfileList() when done with the profile list. </p>
	
	@param list (Filled by the method) A pointer to the new 
	profile list object. 
	@param selector The code for the type of device profile 
	to include in the list.
	@return <code>0</code> if successful, a non-zero error code otherwise. 
	@see ACProfileListCount 
	@see ACProfileListItemCode 
	@see ACProfileListItemDescription 
	@see ACUnReferenceProfileList 

*/
XNPROC(AC_Error, ACMakeProfileList,(AC_ProfileList *list, AC_SelectorCode selector ))

/**
	Gets the number of profiles in a device color profile list. 
	
	@param list The profile list. 
	@param count (Filled by the method) A pointer to the number 
	of profiles in the list.
	@return <code>0</code> if successful, a non-zero error code otherwise. 
	@see ACMakeProfileList 
	@see ACProfileListItemCode 
	@see ACProfileListItemDescription 

*/
XNPROC(AC_Error, ACProfileListCount, (AC_ProfileList list, ASUns32 *count) )

/**
	Returns the description string of a specified profile in 
	a list. The returned description string always contains 
	both ASCII and Unicode data, even if the profile itself 
	only contains an ASCII version. You can store only the Unicode 
	data in settings files if you wish; the ACProfileFromDescription() 
	method finds the correct profile when passed the Unicode-only 
	string. 
	@param list The profile list. 
	@param index The index for the profile in the list. 
	@param description (Filled by the method) A pointer to 
	the profile description string.
	@return <code>0</code> if successful, a non-zero error code otherwise. 
	@see ACMakeProfileList 
	@see ACMakeString 
	@see ACProfileFromDescription 
	@see ACProfileListCount 
	@see ACProfileListItemCode 

*/
XNPROC(AC_Error, ACProfileListItemDescription, (AC_ProfileList list, ASUns32 index, AC_String *description ))

/**
	Gets the profile code of a specified profile in a profile 
	list.  

	<p>While this routine is not absolutely required, since the 
	description string is always a unique reference, profile 
	codes have the advantage that they are localization-independent. </p>
	
	@param list The profile list. 
	@param index The index for the profile in the list. 
	@param code (Filled by the method) A pointer to the profile 
	code. If the specified profile does not have a code, this method returns AC_Profile_Null.
	@return <code>0</code> if successful, a non-zero error code otherwise.
	@see ACMakeProfileList 
	@see ACProfileListCount 
	@see ACProfileListItemDescription 
*/
XNPROC(AC_Error, ACProfileListItemCode,(AC_ProfileList list, ASUns32 index, AC_ProfileCode *code ))

/**
	Decrements the reference count of a device color profile 
	list object. If this causes the object's reference count 
	to reach zero, the method deletes it. 
	@param list The profile list object.
	@return <code>0</code> if successful, a non-zero error code otherwise. 
	@see ACMakeProfileList 
*/
XNPROC(AC_Error, ACUnReferenceProfileList, (AC_ProfileList list))


/**
	Creates a list of preset AcroColor engine (ACE) settings 
	of the specified type. Clients should call ACUnReferencePresetList() 
	when done with the preset list. 

	<p>A preset list is a list of predefined color settings that 
	specifies the source and destination working color profiles 
	to be used for color conversion. </p>

	@param list (Filled by the method) A pointer to the new 
	preset list object. 
	@param type The settings type (AC_SettingsType_Color or AC_SettingsType_Proof).
	@return <code>0</code> if successful, a non-zero error code otherwise. 
	@see ACPresetListCount 
	@see ACPresetListItemFile 
	@see ACUnReferencePresetList 
*/
XNPROC(AC_Error, ACMakePresetList, (AC_PresetList *list, AC_SettingsType type))

/**
	Gets the number of predefined color settings, as listed 
	in the color management settings in 
	the Acrobat user interface. 
	@param list The preset list object. 
	@param count (Filled by the method) A pointer to the number 
	of settings in the list.
	@return <code>0</code> if successful, a non-zero error code otherwise. 
	@see ACMakePresetList 
	@see ACPresetListItemFile 
	@see ACUnReferencePresetList 
*/
XNPROC(AC_Error, ACPresetListCount, (AC_PresetList list, ASUns32 *count))

#if !MAC_PLATFORM || !AS_ARCH_64BIT

/**
	[DEPRECATED]
	Gets the file specification for a preset settings item in 
	a preset list. This API is not available on Mac 64.
	@param list The preset list object. 
	@param index The item index in the list. 
	@param file (Filled by the method) A pointer to the file 
	specification for the item.
	@return <code>0</code> if successful, a non-zero error code otherwise. 
	@see ACLoadSettings 
	@see ACMakePresetList 
	@see ACPresetFileToName 
	@see ACPresetListCount 
	@see ACUnReferencePresetList
	@see replacement ACPresetListItemFileAndNameU

*/
XNPROC(AC_Error, ACPresetListItemFile, (AC_PresetList list, ASUns32 index, AC_FileSpec* file))

#else

NOPROC(ACPresetListItemFile)

#endif

#if !AS_ARCH_64BIT
/**
	[DEPRECATED]
	Translates a preset settings file specification to a name 
	ready to be displayed in menus (with directory paths and 
	file extensions removed). The client should call ACUnReferenceString() 
	when done with the name. This API is not available on Mac 64.
	<ul>
	<li> If the file contains an internal name tag, the returned 
	string is created from the internal name.</li> 
	<li> If the file does not contain an internal name tag, the 
	returned string is built from the file name. In this case, 
	the method assumes that the file name and the ASCII data of the returned string are in the local script 
	code.</li> 
	</ul>
	@param file A pointer to the preset file specification. 
	@param name (Filled by the method) A pointer to the display name string.
	@return <code>0</code> if successful, a non-zero error code otherwise. 
	@see ACPresetListItemFile 
	@see ACUnReferenceString 
	@see replacement ACPresetListItemFileAndNameU
*/
XNPROC(AC_Error, ACPresetFileToName, (const AC_FileSpec* file, AC_String* name))

#else

NOPROC(ACPresetFileToName)

#endif

/**
	Decrements the reference count of a preset list object. 
	If this causes the object's reference count to reach zero, 
	the method deletes it. 
	@param list The preset list object.
	@return <code>0</code> if successful, a non-zero error code otherwise. 
	@see ACMakePresetList 
*/
XNPROC(AC_Error, ACUnReferencePresetList, (AC_PresetList list)) 


/**
	Gets the current color profile for a given key from the 
	AcroColor engine (ACE) <code>settings</code> object. 
	<ul>
	<li> If the settings file contains a profile entry with the 
	specified key, that profile is returned.</li> 
	<li> If the settings file contains a special <code>NULL</code> entry with 
	the key, a <code>NULL</code> profile is returned.</li>
	<li> If the settings file contains a string with this key rather 
	than an embedded profile, this method returns an installed profile whose 
	description matches the string, if found. </li>
	<li> In all other cases, AC_Error_MissingKey is returned.</li>
	</ul>
	The method does not check for known keys or legal key values. 
	It is up to the client to write only legal key values, and 
	to verify key values when reading. 
	@param settings The <code>settings</code> object from which the profile 
	is obtained. 
	@param key The value key constant. 
	@param profile (Filled by the method) A pointer to the 
	current color profile value of the given key.
	@return <code>0</code> if successful, a non-zero error code otherwise. 
	@see ACGetSettingsString 
	@see ACGetSettingsUnsigned32 
	@see ACLoadSettings 
	@see ACMakeSettings 
	@see ACUnReferenceProfile 
	@see ACUnReferenceSettings 
*/
XNPROC(AC_Error, ACGetSettingsProfile, (AC_Settings settings, AC_SettingsKey key, AC_Profile *profile))

/**
	Creates an AcroColor engine (ACE) <code>settings</code> object of a given 
	type, with no entries. When done with all operations, call 
	ACUnReferenceSettings() to free the <code>settings</code> object. 
	@param settings (Filled by the method) A pointer to the 
	new <code>settings</code> object. 
	@param type The settings type (AC_SettingsType_Color or AC_SettingsType_Proof).
	@return <code>0</code> if successful, a non-zero error code otherwise. 
	@see ACGetSettingsProfile 
	@see ACGetSettingsString 
	@see ACGetSettingsUnsigned32 
	@see ACLoadSettings 
	@see ACUnReferenceSettings 
*/
XNPROC(AC_Error, ACMakeSettings, (AC_Settings *settings, AC_SettingsType type))

#if !MAC_PLATFORM || !AS_ARCH_64BIT

/**
	[DEPRECATED]
	Loads the AcroColor engine (ACE) settings from a file. 

	<p>This method reads the settings entries from the specified file and stores 
	them in the <code>settings</code> object, including entries with unknown 
	keys or data formats. As a general rule, the client should 
	keep the <code>settings</code> object around so these unknown keys are 
	preserved when the settings are saved out. The only time 
	the client should start with a fresh <code>settings</code> object is 
	when performing another settings load. This API is not available on Mac 64. </p>

	@param settings (Filled by the method) The <code>settings</code> object. 
	
	@param file A pointer to the file specification for the 
	file containing the settings.
	@return <code>0</code> if successful, a non-zero error code otherwise. 
	@see ACGetSettingsProfile 
	@see ACGetSettingsString 
	@see ACGetSettingsUnsigned32 
	@see ACMakeSettings 
	@see ACPresetListItemFile 
	@see ACUnReferenceSettings 
	@see replacment ACLoadSettingsU
*/
XNPROC(AC_Error, ACLoadSettings, (AC_Settings settings, AC_FileSpec *file))

#else

NOPROC(ACLoadSettings)

#endif

/**
	Gets the current string value for a given key from the AcroColor 
	engine (ACE) <code>settings</code> object. 
	<ul>
	<li> If the settings file contains a string entry with the 
	specified key, the method returns the entry.</li> 
	<li> If the settings file contains a special <code>NULL</code> entry with 
	the key, the method returns a <code>NULL</code> string.</li>
	<li> In all other cases, the method returns AC_Error_MissingKey.</li>
	</ul>
	@param settings The <code>settings</code> object from which the string 
	is obtained. 
	@param key The value key constant. 
	@param string (Filled by the method) A pointer to the 
	current string value of the given key.
	@return <code>0</code> if successful, a non-zero error code otherwise. 
	@see ACGetSettingsProfile 
	@see ACGetSettingsUnsigned32 
	@see ACLoadSettings 
	@see ACMakeSettings 
	@see ACUnReferenceSettings 
	@see ACUnReferenceString 

*/
XNPROC(AC_Error, ACGetSettingsString, (AC_Settings settings, AC_SettingsKey key, AC_String *string))

/**
	Gets the current numeric value for a given key from the 
	AcroColor engine (ACE) <code>settings</code> object. 
	<ul>
	<li> If the settings file contains an unsigned 32-bit numeric 
	entry with the specified key, the method returns the entry.</li>
	<li> In all other cases, the method returns AC_Error_MissingKey. </li>
	</ul>
	@param settings The <code>settings</code> object from which the value 
	is obtained. 
	@param key The value key constant. 
	@param value (Filled by the method) A pointer to the current 
	numeric value of the given key. 
	@return <code>0</code> if successful, a non-zero error code otherwise. 
	@see ACGetSettingsProfile 
	@see ACGetSettingsString 
	@see ACLoadSettings 
	@see ACMakeSettings 
	@see ACUnReferenceSettings 
*/
XNPROC(AC_Error, ACGetSettingsUnsigned32, (AC_Settings settings, AC_SettingsKey key, ASUns32 *value))

/**
	Decrements the reference count of an AcroColor engine <code>settings</code> 
	object. If this causes the object's reference count to reach 
	zero, the method deletes it. 
	@param settings The <code>settings</code> object.
	@return <code>0</code> if successful, a non-zero error code otherwise. 
	@see ACMakeSettings 

*/
XNPROC(AC_Error, ACUnReferenceSettings, (AC_Settings settings))


/**
	Gets the description of a device profile. The returned description 
	string contains both ASCII and Unicode data, even if the 
	profile itself only contains ASCII data. 
	@param profile The device color profile. 
	@param description (Filled by the method) A pointer to 
	the description string.
	@return <code>0</code> if successful, a non-zero error code otherwise. 
	@see ACMakeString 
	@see ACProfileColorSpace 
	@see ACProfileData 
	@see ACProfileListItemDescription 
	@see ACProfileFromDescription 
*/
XNPROC(AC_Error, ACProfileDescription, (AC_Profile profile, AC_String *description))

/**
	Finds a profile matching the description string in the database. 
	The client should call ACUnReferenceProfile() when done with 
	the profile. 
	<ul>
	<li> If the description string contains Unicode text, the Unicode 
	text is used to find the profile. </li>
	<li> If the description string contains only ASCII text, the 
	method tries to find a match. However, the AcroColor engine 
	requires only Unicode descriptions to be unique, so this 
	might return the wrong profile is in some rare cases. Use 
	ASCII-only description strings only when Unicode description 
	string are unavailable. </li>
	</ul>
	@param profile (Filled by the method) A pointer to the 
	device color profile object. 
	@param description The description string.
	@return <code>0</code> if successful, a non-zero error code otherwise. 
	@see ACGetSettingsProfile 
	@see ACGetWorkingSpaceProfile 
	@see ACMakeBufferProfile 
	@see ACMakeCalGray 
	@see ACMakeCalLab 
	@see ACMakeCalRGB 
	@see ACMakeString 
	@see ACMonitorProfile 
	@see ACProfileFromCode 
	@see ACProfileListItemDescription 
	@see ACUnReferenceProfile 
*/
XNPROC(AC_Error, ACProfileFromDescription, (AC_Profile *profile, AC_String description))

/**
	Creates a device profile from a device profile type code. 
	The client should call ACUnReferenceProfile() when done with 
	the profile. 
	@param profile (Filled by the method) A pointer to the 
	device color profile object. 
	@param code The profile type code.
	@return <code>0</code> if successful, a non-zero error code otherwise. 
	@see ACGetSettingsProfile 
	@see ACGetWorkingSpaceProfile 
	@see ACMakeBufferProfile 
	@see ACMakeCalGray 
	@see ACMakeCalLab 
	@see ACMakeCalRGB 
	@see ACMonitorProfile 
	@see ACProfileFromDescription 
	@see ACUnReferenceProfile 

*/
XNPROC(AC_Error, ACProfileFromCode, (AC_Profile *profile, AC_ProfileCode code))

#if !MAC_PLATFORM || !AS_ARCH_64BIT
/**
	Gets a device color profile for a specific monitor device. 
	The returned profile may be either RGB or grayscale. If 
	no profile is specified by the system, the method returns a default 
	platform profile (sRGB on Windows, Apple RGB on Mac OS). 
	The client should call ACUnReferenceProfile() when done with 
	the returned profile. This API is not available on Mac 64.
	@param profile (Filled by the method) A pointer to the 
	profile object. 
	@param monitorID A pointer to the platform-specific monitor 
	device identifier. On Windows, this is a <code>NULL</code>-terminated ASCII string 
	containing the monitor's device name (for example, <code>"Display"</code>) 
	On Mac OS, this is the monitor's AVID.
	
	@return <code>0</code> if successful, a non-zero error code otherwise. 
	@see ACGetSettingsProfile 
	@see ACGetWorkingSpaceProfile 
	@see ACMakeBufferProfile 
	@see ACMakeCalGray 
	@see ACMakeCalLab 
	@see ACMakeCalRGB 
	@see ACProfileFromCode 
	@see ACProfileFromDescription 
	@see ACUnReferenceProfile 
*/
XNPROC(AC_Error, ACMonitorProfile, (AC_Profile *profile, void *monitorID))

#else

NOPROC(ACMonitorProfile)

#endif

/**
	Creates a device color profile object from a data buffer 
	containing the raw ICC profile data. The method copies the data, so 
	the client can dispose of the source data. The client should 
	call ACUnReferenceProfile() when done with the profile. 
	@param profile (Filled by the method) The device profile. 
	
	@param data The buffer containing the device profile data. 
	
	@param dataSize The size in bytes of the data buffer.
	@return <code>0</code> if successful, a non-zero error code otherwise. 
	@see ACGetSettingsProfile 
	@see ACGetWorkingSpaceProfile 
	@see ACMakeCalGray 
	@see ACMakeCalLab 
	@see ACMakeCalRGB 
	@see ACMonitorProfile 
	@see ACProfileFromCode 
	@see ACProfileFromDescription 
	@see ACUnReferenceProfile 

*/
XNPROC(AC_Error, ACMakeBufferProfile, (AC_Profile *profile, void *data, ASUns32 dataSize))


/**
	Creates a device color profile object from a calibrated 
	RGB color space, with the specified rendering intent and 
	descriptive string. The client should call ACUnReferenceProfile() 
	when done with the profile. 
	@param profile (Filled by the method) A pointer to the 
	new device color profile. 
	@param spec The calibrated RGB color space specification. 
	
	@param intent The rendering intent. 
	@param description The description of the new profile. If it is non-<code>NULL</code>, the description must contain ASCII data, and may contain 
	Unicode data also. If it is <code>NULL</code>, a hard-coded default description 
	is used.
	@return <code>0</code> if successful, a non-zero error code otherwise. 
	@see ACGetSettingsProfile 
	@see ACGetWorkingSpaceProfile 
	@see ACMakeBufferProfile 
	@see ACMakeCalGray 
	@see ACMakeCalLab 
	@see ACMonitorProfile 
	@see ACProfileFromCode 
	@see ACProfileFromDescription 
	@see ACUnReferenceProfile 
*/
XNPROC(AC_Error, ACMakeCalRGB, (AC_Profile *profile, ACCalRGB *spec, AC_RenderIntent intent, AC_String description))

/**
	Creates a device color profile object from a calibrated 
	grayscale color space with the specified rendering intent 
	and description string. The client should call ACUnReferenceProfile() 
	when done with the profile. 
	@param profile (Filled by the method) A pointer to the 
	new device color profile. 
	@param spec A pointer to the calibrated grayscale color 
	space specification. 
	@param intent The rendering intent. 
	@param description The description of the new profile. 
	If it is non-<code>NULL</code>, it must contain ASCII data, and may contain 
	Unicode data also. If <code>NULL</code>, a hard-coded default description 
	is used. 
	@return <code>0</code> if successful, a non-zero error code otherwise. 
	@see ACGetSettingsProfile 
	@see ACGetWorkingSpaceProfile 
	@see ACMakeBufferProfile 
	@see ACMakeCalLab 
	@see ACMakeCalRGB 
	@see ACMonitorProfile 
	@see ACProfileFromCode 
	@see ACProfileFromDescription 
	@see ACUnReferenceProfile 
*/
XNPROC(AC_Error, ACMakeCalGray, (AC_Profile *profile, ACCalGray *spec, AC_RenderIntent intent, AC_String description))

/**
	Creates a device color profile object from a calibrated 
	Lab color space with the specified rendering intent and 
	description string. The client should call ACUnReferenceProfile() 
	when done with the profile. 
	@param profile (Filled by the method) A pointer to the 
	new device color profile. 
	@param spec The calibrated Lab color space specification. 
	
	@param intent The rendering intent. 
	@param description The description of the new profile. 
	If it is non-<code>NULL</code>, it must contain ASCII data, and may contain 
	Unicode data also. If it is <code>NULL</code>, a hard-coded default description 
	is used. 
	@return <code>0</code> if successful, a non-zero error code otherwise. 
	@see ACGetSettingsProfile 
	@see ACGetWorkingSpaceProfile 
	@see ACMakeBufferProfile 
	@see ACMakeCalGray 
	@see ACMakeCalRGB 
	@see ACMonitorProfile 
	@see ACProfileFromCode 
	@see ACProfileFromDescription 
	@see ACUnReferenceProfile 

*/
XNPROC(AC_Error, ACMakeCalLab, (AC_Profile *profile, ACCalLab *spec, AC_RenderIntent intent, AC_String description))

/**
	Gets the color space for a device profile. 
	@param profile The device color profile. 
	@param space (Filled by the method) A pointer to the color 
	space.
	@return <code>0</code> if successful, a non-zero error code otherwise. 
	@see ACProfileData 
	@see ACProfileDescription 
*/
XNPROC(AC_Error, ACProfileColorSpace, (AC_Profile profile, AC_ColorSpace *space))

/**
	Gets the size in bytes of the raw ICC profile data in 
	a device profile. 
	@param profile The device color profile object. 
	@param size (Filled by the method) A pointer to the profile 
	data size.
	@return <code>0</code> if successful, a non-zero error code otherwise. 
	@see ACProfileData 
*/
XNPROC(AC_Error, ACProfileSize, (AC_Profile profile, ASUns32 *size))

/**
	Gets the data for a device profile. Copies the raw ICC profile 
	data into a supplied buffer. 
	@param profile The device color profile. 
	@param data (Filled by the method) A pointer to the color 
	profile data.
	@return <code>0</code> if successful, a non-zero error code otherwise. 
	@see ACProfileColorSpace 
	@see ACProfileDescription 
	@see ACProfileSize 

*/
XNPROC(AC_Error, ACProfileData, (AC_Profile profile, void *data))

/**
	Decrements the reference count of a device color profile 
	object. If this causes the object's reference count to reach 
	zero, the method deletes it. 
	@param profile The profile object.
	@return <code>0</code> if successful, a non-zero error code otherwise. 
	@see ACGetSettingsProfile 
	@see ACGetWorkingSpaceProfile 
	@see ACMakeBufferProfile 
	@see ACMakeCalGray 
	@see ACMakeCalLab 
	@see ACMakeCalRGB 
	@see ACMonitorProfile 
	@see ACProfileFromCode 
	@see ACProfileFromDescription 
	@see ACProfileFromDescription 

*/
XNPROC(AC_Error, ACUnReferenceProfile, (AC_Profile profile))


/**
	Creates a color transformation object. 

	<p>The client can dispose of the source and destination profiles 
	as soon as the transform is created. If the source profile 
	is a device link or abstract profile, then the destination 
	profile must be <code>NULL</code>; otherwise it must be non-<code>NULL</code>. </p>

	@param transform (Filled by the method) A pointer to the 
	new color transformation object. 
	@param srcProfile The source profile from which to transform 
	color data. 
	@param dstProfile The destination profile to which to 
	transform color data. 
	@param intent The rendering intent for colors outside 
	the gamut of the destination profile.
	@return <code>0</code> if successful, a non-zero error code otherwise. 
	@see ACApplyTransform 
	@see ACUnReferenceTransform 

*/
XNPROC(AC_Error, ACMakeColorTransform, (AC_Transform *transform, AC_Profile srcProfile, AC_Profile dstProfile, AC_RenderIntent intent /* options */))

/**
	Applies a color conversion or gamut test transformation. 
	It processes the number of colors specified by <code>count</code>, using 
	the memory formats for the source and destination data specified 
	in <code>srcPacking</code> and <code>dstPacking</code>. The source data and destination 
	data can point to the same block of memory if the source 
	and destination packing formats use the same number of bits 
	per color. 
	@param transform The color conversion or tranformation 
	to apply. 
	@param srcData The source data to tranform. 
	@param dstData The destination for the transformed data. 
	
	@param count The number of colors to transform. 
	@param srcPacking The packing type used in the source 
	data. 
	@param dstPacking The packing type to use in the destination 
	data.
	@return <code>0</code> if successful, a non-zero error code otherwise. 
	@see ACMakeColorTransform 

*/
XNPROC(AC_Error, ACApplyTransform, (AC_Transform transform, const void* srcData, void*	dstData, ASUns32 count, AC_PackingCode srcPacking, AC_PackingCode	dstPacking /* options */))

/**
	Decrements the reference count of a color transformation 
	object. If this causes the object's reference count to reach 
	zero, the method deletes it. 
	@param transform The tranform object.
	@return <code>0</code> if successful, a non-zero error code otherwise. 
	@see ACMakeColorTransform 
*/
XNPROC(AC_Error, ACUnReferenceTransform, (AC_Transform transform))


/**
	Creates an AcroColor string from a <code>NULL</code>-terminated ASCII 
	string or a <code>NULL</code>-terminated Unicode string, or both. If 
	both ASCII and Unicode data are specified, the AC_String 
	object keeps track of both in parallel, returning the ASCII 
	data when asked for ASCII, and the Unicode data when asked 
	for Unicode. 

	<p>These dual-encoded strings are useful as description strings 
	for ICC profiles, which can store both ASCII and Unicode 
	data in their description tags. The ICC profile standard 
	requires that the ASCII version of the description string be 
	limited to 7-bit ASCII characters. The AcroColor engine 
	requires only the Unicode descriptions to be unique among 
	profile descriptions. </p>

	@param string (Filled by the method) A pointer to the 
	new string object. 
	@param ascii The ASCII data. It should be limited to 7-bit 
	ASCII characters for use in profile descriptions. 
	@param unicode The Unicode data. All Unicode characters 
	are two byte characters, in native byte order, including 
	the trailing <code>NULL</code>.
	@return <code>0</code> if successful, a non-zero error code otherwise. 
	@see ACProfileFromDescription 
	@see ACProfileListItemDescription 
	@see ACStringASCII 
	@see ACStringLocalized 
	@see ACStringUnicode 
	@see ACUnReferenceString 

*/
XNPROC(AC_Error, ACMakeString, (AC_String* string, const char* ascii,  const ASUTF16Val* unicode))

/**
	Copies the ASCII version of a string into a supplied buffer. 
	Either the <code>buffer</code> or the <code>count</code> can be <code>NULL</code>. 

	<p>The ICC profile standard requires that ASCII version of 
	the profile description string be limited to 7-bit ASCII 
	characters. </p>

	<p>Depending on the API, operating system, file contents, and so on, the 
	method can return strings in the local script code (8 bit 
	single byte or 8 bit encoded multi-byte). Clients 
	should always assume that the ASCII data is in the local 
	script code (of which the 7-bit ASCII characters are a subset). </p>
	
	@param string The AcroColor string containing ASCII data. 
	If the string does not contain an ASCII version, the method 
	returns AC_Error_NoASCII. 
	@param buffer (Filled by the method) A buffer to contain 
	a copy of the ASCII data. 
	@param count (Filled by the method) A pointer to the size 
	of <code>buffer</code> in bytes, including the trailing <code>NULL</code> character. 
	
	@param maxCount The maximum size of <code>buffer</code> in bytes. If 
	the length of the string is longer than this value, the method 
	copies a truncated string to the <code>buffer</code> and returns AC_Error_StringOverflow.
	@return <code>0</code> if successful, a non-zero error code otherwise. 
	@see ACMakeString 
	@see ACStringLocalized 
	@see ACStringUnicode 
	@see ACUnReferenceString 

*/
XNPROC(AC_Error, ACStringASCII, (AC_String string, char* buffer, ASUns32* count, ASUns32 maxCount))

/**
	Copies the localized Unicode version of a string into a 
	supplied buffer. Either the <code>buffer</code> or the <code>count</code> can be <code>NULL</code>. 
	
	<p>The settings file format and ICC profiles later than version 
	2 can contain text in multiple languages or countries. 
	When the AcroColor engine (ACE) create strings from these 
	files or profiles, it uses the current language and country 
	codes to create strings with a third fork: a localized Unicode 
	version. These localized versions are intended for user 
	display only and should not be stored in preferences files 
	or action scripts, since they vary from country to country 
	and are not portable. </p>

	@param string The AcroColor string containing localized 
	Unicode data. If the string does not contain a localized 
	Unicode version, the method returns AC_Error_NoLocalized. 
	
	@param buffer (Filled by the method) A buffer to contain 
	a copy of the localized Unicode data. 
	@param count (Filled by the method) A pointer to the size (in bytes)
	of the <code>buffer</code>, including the trailing <code>NULL</code> character. 
	
	@param maxCount The maximum size of the <code>buffer</code> in bytes. If 
	the length of the string is longer than this value, the method 
	copies a truncated string to the <code>buffer</code> and returns AC_Error_StringOverflow.
	@return <code>0</code> if successful, a non-zero error code otherwise. 
	@see ACMakeString 
	@see ACStringASCII 
	@see ACStringUnicode 
	@see ACUnReferenceString 
*/
XNPROC(AC_Error, ACStringLocalized, (AC_String string, ASUTF16Val* buffer,ASUns32* count, ASUns32 maxCount))
	  

/**
	Copies the Unicode version of a string into a supplied buffer. 
	Either the <code>buffer</code> or the <code>count</code> can be <code>NULL</code>. 
	@param string The AcroColor string containing localized 
	Unicode data. If the string does not contain a Unicode version, 
	the method returns AC_Error_NoUnicode. 
	@param buffer (Filled by the method) A buffer to contain 
	a copy of the Unicode data. 
	@param count (Filled by the method) A pointer to the size 
	of <code>buffer</code> in bytes, including the trailing <code>NULL</code> character. 
	
	@param maxCount The maximum size of <code>buffer</code> in bytes. If 
	the length of the string is longer than this value, the method 
	copies a truncated string to the <code>buffer</code> and returns AC_Error_StringOverflow.
	@return <code>0</code> if successful, a non-zero error code otherwise. 
	@see ACMakeString 
	@see ACStringASCII 
	@see ACStringLocalized 
	@see ACUnReferenceString 

*/
XNPROC(AC_Error, ACStringUnicode, (AC_String string, ASUTF16Val* buffer, ASUns32* count, ASUns32 maxCount))

/**
	Decrements the reference count of a string object. If this 
	causes the object's reference count to reach zero, the method 
	deletes it. 
	@param string The string object.
	@return <code>0</code> if successful, a non-zero error code otherwise. 
	@see ACMakeString 
*/
XNPROC(AC_Error, ACUnReferenceString, (AC_String string))


/**
	Gets a working color profile in a specified color space. 
	
	@param space The type of color space of the profile to 
	obtain. 
	@param workingProfile (Filled by the method) A pointer 
	to the working profile. When done with this object, dereference 
	it using ACUnReferenceProfile().
	@return <code>0</code> if successful, a non-zero error code otherwise. 
	@see ACProfilesMatch 
	@see ACUnReferenceProfile 
*/
XNPROC(AC_Error, ACGetWorkingSpaceProfile, (ACWorkingSpace space, AC_Profile *workingProfile))

/**
	Compares the working device profile with the document device 
	profile to determine if they are the same. This comparison 
	ignores rendering intents, and is <i>fuzzy</i>, allowing very 
	close, but not exactly the same, profiles to match. Equivalent 
	profiles always match, but some non-equivalent profiles 
	may also match. 
	@param workingProfile The working device color profile. 
	
	@param documentProfile The document's device color profile. 
	
	@param match (Filled by the method) A pointer to the result: 
	<code>true</code> if the profiles match, <code>false</code> otherwise.
	@return <code>0</code> if successful, a non-zero error code otherwise. 
	@see ACGetSettingsProfile 
	@see ACGetWorkingSpaceProfile 
	@see ACUnReferenceProfile 

*/
XNPROC(AC_Error, ACProfilesMatch, (AC_Profile workingProfile, AC_Profile documentProfile, ASBool *match))


/**
    Converts the colors (in place) on a page, as specified by the <code>params</code> block.
    
    @param doc The document in which to convert a page.
    @param params The parameter block that describes how color conversions are to be performed.
    @param pageNum The page number of the page to convert.
    @param progMon The progress monitor callback. This call will set the duration of the monitor to the number of elements in the top-level content stream, and will update the value as the elements are converted. If this parameter is zero, no progress monitor callback is called.
    @param progMonData The data element to be passed into progress monitor calls.
    @param reportProc The reporting callback; it reports the attributes and action of each object converted to the callback. Passing in a zero reporting callback means that no reporting will be done.
    @param reportProcData The data element to be passed into <code>reportProc</code>.
    @return <code>true</code> if the conversion was aborted or failed.
    @exception asGenErrBadParm The <code>params</code> block is malformed (for example, a reference or alias to a non-existent ink, or a circular alias).
*/
    XNPROC(ASBool, PDDocColorConvertPage, (PDDoc doc, PDColorConvertParams params, ASInt32 pageNum, ASProgressMonitor progMon, void *progMonData, PDColorConvertReportProc reportProc, void *reportProcData, ASBool *changed))

/**
    Convert the colors (in place) in a page as specified by the <code>params</code> block.
    Takes an extended parameters block.
    
    @param doc The document in which to convert a page.
    @param params The parameters block that describes how color conversions are to be performed.
    @param pageNum The page number of the page to convert.
    @param progMon The progress monitor callback. This call will set the duration of the monitor to the number of elements in the top-level content stream, and will update the value as the elements are converted. If this parameter is zero, no progress monitor callback is called.
    @param progMonData The data element to be passed into progress monitor calls.
    @param reportProc The reporting callback; it reports the attributes and action of each object converted to the callback. Passing in a zero reporting callback means that no reporting will be done.
    @param reportProcData The data element to be passed into <code>reportProc</code>.
    @return <code>true</code> if the conversion was aborted or failed.
    @exception asGenErrBadParm The <code>params</code> block is ill-formed (for example, a reference or alias to a non-existent ink, or a circular alias).
*/
    XNPROC(ASBool, PDDocColorConvertPageEx, (PDDoc doc, PDColorConvertParamsEx paramsEx, ASInt32 pageNum, ASProgressMonitor progMon, void *progMonData, PDColorConvertReportProc reportProc, void *reportProcData, ASBool *changed))

/**
    Embeds an output intent into a document.

    @param doc The document in which to embed the output intent.
    @param params The parameters block from which to get the output intent, described as the target space.
*/

    XNPROC(void, PDDocColorConvertEmbedOutputIntent, (PDDoc doc, AC_Profile OIProfile))

/**
    Converts a PDEElement to the supplied color space.

    @param doc The document in which the element is located.
    @param elem The element to convert.
    @param targetProfile The ICC profile to which the color should be converted.
    @param intent The rendering intent to use for the conversion. AC_UseProfileIntent can be passed in order
    to use the default intent. (Note that it is not actually using the profile intent, but is using the
    current intent in the PDF graphics state).
    @param embed If <code>true</code>, embed the color space and make the object calibrated. If it is <code>false</code> and the
    target profile is CMYK, RGB, or Gray, the colors space of the resulting object, after conversion, will
    be DeviceCMYK, DeviceRGB, or DeviceGray, respectively.
    @returns A PDEElement containing the converted data. Note that the source element is copied and its
    reference count is not decremented, so the caller should decrement the source element's reference count
    if it is no longer needed.

	@see PDColorConvertPDEElementEx
*/
		XNPROC(PDEElement, PDColorConvertPDEElement, (PDDoc doc, PDEElement elem, AC_Profile targetProfile, AC_RenderIntent intent, ASBool embed))

/* Swatchbook API */
    

/**
	Retrieves an <code>ACSwatchBookDB</code> database object, containing the swatchbooks found by searching the folders given.
	The folders are usually determined by using the <code>AVAcquireSpecialFolderPathName</code>. This always scans
	the swatchbook directories, so this should be called every time one is going to make a list of the
	swatchbooks, in case a user installed a swatchbook in the user directory while the application was open.
	PDF library clients must pass in whatever folder location is appropriate.
	@param count The number of folders in the folders array.
	@param folders A pointer to an array of path names for the folders to search.
	@returns A swatchbook database object. Call <code>ACSwatchBookDBDestroy()</code> when this is no longer needed.
*/
	XNPROC(ACSwatchBookDB, ACSwatchBooksFind, (ASUns32 count, ASFileSys fs, ASPathName *folders))

/**
	Retrieves the number of swatchbooks available in the swatchbook database that was returned by <code>ACSwatchBookFind()</code>.
	@param dbp A pointer to the swatchbook database object.
	@returns The number of swatchbooks in the database.
*/
	XNPROC(ASUns32, ACSwatchBookCount, (ACSwatchBookDB dbp))

/**
	Retrieves the title of a swatchbook. 
	@param dbp A pointer to the swatchbook database object.
	@param ix The index of the swatchbook item. Its value is in the range <code>[0, swatchBookCount-1]</code>.
	@returns The title of the swatchbook.
*/
	XNPROC(ASText, ACSwatchBookTitle, (ACSwatchBookDB dbp, ASUns32 ix))

/**
	Retrieves the description string for a swatchbook.
	@param dbp A pointer to the swatchbook database object.
	@param ix The index of the swatchbook item. Its value is in the range <code>[0, swatchBookCount-1]</code>.
	@returns The description string for the swatchbook.
*/
	XNPROC(ASText, ACSwatchBookDescription, (ACSwatchBookDB dbp, ASUns32 ix))

/**
	Destroys the swatchbook database and frees any memory associated with it.
	@param dbp A pointer to the swatchbook database object to be destroyed.
*/
	XNPROC(void, ACSwatchBookDBDestroy, (ACSwatchBookDB dbp))

/**	
    Retrieves an opaque <code>ACSwatchBook</code> object for the nth swatchbook.
    This loads the swatchbook into memory. Zero will be returned if there was an error.
	@param dbp The swatchbook database.
	@param n The index of swatchbook to load. Its value is in the range <code>[0, swatchBookCount-1]</code>.
	@returns The swatchbook object. Call <code>ACSwatchBookDestroy()</code> when it is no longer needed.
*/
	XNPROC(ACSwatchBook, ACSwatchBookLoad, (ACSwatchBookDB dbp, ASUns32 ix))
/**	
    Retrieves an opaque <code>ACSwatchBook</code> object using the specified path.
    This loads the swatchbook into memory. Zero will be returned if there was an error.
	@param dbp The swatchbook database.
	@param path The path to the swatchbook file.
	@returns The swatchbook object. Call <code>ACSwatchBookDestroy()</code> when it is no longer needed.
*/
	XNPROC(ACSwatchBook, ACSwatchBookLoadFromPath, (ACSwatchBookDB dbp, ASPathName path))

/**
	Destroys the swatchbook and frees any memory associated with it.
	@param dbp A pointer to the swatchbook object to be destroyed.
*/
	XNPROC(void, ACSwatchBookDestroy, (ACSwatchBook bp))

/**
	Retrieves the number of color swatches in the swatchbook.
	@param dbp A pointer to the swatchbook object.
	@returns The number of swatches in the swatchbook.
*/
	XNPROC(ASUns32, ACSwatchBookNumberOfColors, (ACSwatchBook bp))

/**
	Retrieves the color space of the swatches in the swatchbook.
	@param dbp A pointer to the swatchbook object.
	@returns The color space of the swatchbook object (for example, all swatches are in this space).
*/
	XNPROC(AC_ColorSpace, ACSwatchBookColorSpace, (ACSwatchBook bp))

/**
	Determines whether the swatchbook is for a process color mode. 
	@param dbp A pointer to the swatchbook object.
	@returns <code>true</code> if the swatchbook is for a process color mode, <code>false</code> if it is for spot.
*/
	XNPROC(ASBool, ACSwatchBookIsProcess, (ACSwatchBook bp))

/**
	Retrieves the name of a color swatch.
	@param dbp A pointer to the swatchbook object.
	@param ix The index of the swatch. Its value is in the range <code>[0, swatchCount-1]</code>.
	@returns The name of the color swatch.
*/
	XNPROC(ASText, ACSwatchBookGetSwatchName, (ACSwatchBook bp, ASUns32 ix))

/**
	Retrieves the color values associated with a color swatch.
	@param dbp A pointer to the swatchbook object.
	@param ix The index of the swatch. Its value is in the range <code>[0, swatchCount-1]</code>.
	@param values Values that are filled in by this call.
*/
	XNPROC(void, ACSwatchBookGetSwatchValues, (ACSwatchBook bp, ASUns32 ix, float *values))

/**
	Returns the file specification and a display name of a specified preset
	in a list. The client should call ACE_UnReferenceString when done with the
	file and name.
 
	Either the file or name parameter can be NULL if the client does not
	need that value returned. It is an error if both parameters are NULL.
 
	Note: If the preset file contains an internal name tag, the returned string
	is created from the internal name. If the preset file does not contain an
	internal name tag, the returned string is built from the file name.
	Any leading path or trailing extension will be removed from the string.
*/ 
	XNPROC(AC_Error, ACPresetListItemFileAndNameU, (AC_PresetList list, ASUns32 index, AC_String* file, AC_String* name))

/**
	Reads the settings entries from the specified file.  All entries are
	stored in the settings object, even entries with unknown keys or data
	formats.  As a general rule, the client should keep the settings object
	around so these unknown keys are preserved when the settings are saved out.
	The only time the client should start with a fresh settings object is when
	performing another settings load.
 
	@see ACLoadSettings
*/
	XNPROC(AC_Error, ACLoadSettingsU, (AC_Settings settings, AC_String file))
	XNPROC(AC_Error, ACMonitorProfileN, (AC_Profile *profile, void *monitorID))


/**
    Converts a PDEElement to the supplied color space.

    @param doc The document in which the element is located.
    @param elem The element to convert.
	@param params The parameters block that describes how color conversions are to be performed.
    @returns A PDEElement containing the converted data. Note that the source element is copied and its
    reference count is not decremented, so the caller should decrement the source element's reference count
    if it is no longer needed.

	@see PDColorConvertPDEElement
*/
	XNPROC(PDEElement, PDColorConvertPDEElementEx, (PDDoc doc, PDEElement elem, PDColorConvertParamsEx params))

/**
	Get the black-point compensation flag. 
	@param bpc It will receive the flag value, flag value equal to 1 implies black-point compensation is on , value 0 otherwise.
	@return <code>0</code> if successful, a non-zero error code otherwise.
	@see ACGetBlackPointCompensation
*/
	XNPROC(AC_Error, ACGetBlackPointCompensation, (ASUns32 *bpc))

/**
	Sets the black-point compensation flag, which controls whether to adjust for differences in black points when converting 
	colors between color spaces. When enabled, the full dynamic range of the source space is mapped into the full dynamic 
	range of the destination space. When disabled, the dynamic range of the source space is simulated in the destination 
	space (which can result in blocked or gray shadows). 
	@param bpc <code>1</code> to enable black-point compensation, <code>0</code> 
	otherwise.
	@return <code>0</code> if successful, a non-zero error code otherwise.
	@see ACSetBlackPointCompensation
*/
	XNPROC(AC_Error, ACSetBlackPointCompensation, (ASUns32 bpc))

#undef XSPROC
#undef XNPROC
#undef XPROC
