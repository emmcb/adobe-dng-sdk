/*
** AcroColorExpT.h
**
** Types, macros, structures, etc. required to use the Acrobat Color Host Function Table.
**
Copyright (C) 2002-2006 Adobe Systems Inc.  All Rights Reserved.
**
*/

#ifndef _H_AcroColorExpT
#define _H_AcroColorExpT
//#include "Environ.h"

/*
This version # is used in AcroColorCalls.h for the definition/inclusion of
the entry points. If missing, you don't get the right results. Clients
should not have to explicitly specify this to get it to work. They
will have to specify a later version number if they really want a
later HFT version.
*/

#if PLUGIN
#include "CoreExpT.h"
#include "ASExpT.h"
#include "PDExpT.h"
#include "PEExpT.h"
#endif /* PLUGIN */

#ifndef FOUR_CHAR_CODE
#ifdef __SUNPRO_CC
//SunStudio reads the byte in reverse order
#define FOUR_CHAR_CODE(x) (((ASUns32)(x)>>24) + (((x)>>8)&0x0ff00) + (((x)<<8)&0x0ff0000) + ((x)<<24))
#else
#define FOUR_CHAR_CODE(x)			(x)
#endif
#endif

#ifdef __cplusplus
extern "C" {
#endif


#define AcroColorHFTNAME "AcroColorHFT"

#ifdef __cplusplus

/**
	A string value type for use with AcroColor functions.
	@see ACEngineInfo
	@see ACGetSettingsString
	@see ACMakeCalGray
	@see ACMakeCalLab
	@see ACMakeCalRGB
	@see ACMakeString
	@see ACPresetFileToName
	@see ACProfileDescription
	@see ACProfileFromDescription
	@see ACProfileListItemDescription
	@see ACStringASCII
	@see ACStringLocalized
	@see ACStringUnicode
	@see ACUnReferenceString
*/
    class ACString;
    
typedef ACString *AC_String;

#else

typedef struct ACString *AC_String;

#endif

#ifdef __cplusplus

/**
	An opaque object representing a list of device color profiles.
	@see ACMakeProfileList
	@see ACProfileListCount
	@see ACProfileListItemCode
	@see ACProfileListItemDescription
	@see ACUnReferenceProfileList
*/
class ACProfileList;
typedef ACProfileList *AC_ProfileList;

#else

typedef struct ACProfileList *AC_ProfileList;

#endif

#ifdef __cplusplus

/** An opaque object representing a color tranformation.
	@see ACApplyTransform
	@see ACMakeColorTransform
	@see ACUnReferenceTranform
*/
class ACTransform;

typedef ACTransform *AC_Transform;

#else

typedef struct ACTransform *AC_Transform;

#endif

#ifdef __cplusplus

/**
	An opaque object representing the settings for the AcroColor engine (ACE).
	@see ACGetSettingsProfile
	@see ACGetSettingsString
	@see ACGetSettingsUnsigned32
	@see ACLoadSettings
	@see ACMakeSettings
	@see ACUnReferenceSettings
*/
class ACSettings;

typedef ACSettings *AC_Settings;

#else

typedef struct ACSettings *AC_Settings;

#endif

#ifdef __cplusplus

/**
	An opaque object representing a device color profile.
	@see ACGetSettingsProfile
	@see ACGetWorkingSpaceProfile
	@see ACMakeBufferProfile
	@see ACMakeCalGray
	@see ACMakeCalLab
	@see ACMakeCalRGB
	@see ACMakeColorTransform
	@see ACMonitorProfile
	@see ACProfileColorSpace
	@see ACProfileData
	@see ACProfileDescription
	@see ACProfileFromCode
	@see ACProfileFromDescription
	@see ACProfileSize
	@see ACProfilesMatch
	@see ACUnReferenceProfile
*/
class ACProfile;

typedef ACProfile *AC_Profile;

#else

typedef struct ACProfile *AC_Profile;

#endif

#ifdef __cplusplus
/**
	An opaque object representing a preset list. A preset list is a list of predefined color settings
	that specify the source and destination working color profiles to be used for color
	conversion.
	@see ACMakePresetList
	@see ACPresetListCount
	@see ACPresetListItemFile
	@see ACUnReferencePresetList
*/
class ACPresetList;

typedef ACPresetList *AC_PresetList;

#else

typedef struct ACPresetList *AC_PresetList;

#endif


/** Constants that specify the types of device profiles to include in a profile list. 
	@see ACMakeProfileList
*/
#if 0
typedef enum
{

	/** Standard (recommended) RGB profiles. These profiles are always
	   bidirectional. */
	AC_Selector_RGB_Standard				= FOUR_CHAR_CODE ('rStd'),
	

	/** RGB profiles that can be used as a source. These profiles may or
	   may not be usable as a destination. This constant does not include profiles selected
	   by AC_Selector_RGB_Standard. */
    AC_Selector_RGB_OtherInputCapable		= FOUR_CHAR_CODE ('rInp'),

	/** RGB profiles that can be used as a destination. These profiles are
	   also usable as a source. This constant does not include profiles selected
	   by AC_Selector_RGB_Standard. */
	AC_Selector_RGB_OtherOutputCapable		= FOUR_CHAR_CODE ('rOut'),

	/** Standard (recommended) CMYK profiles that can be used as a source.
	   These profiles may or may not be usable as a destination. */
	AC_Selector_CMYK_StandardInput			= FOUR_CHAR_CODE ('cSIn'),

	/** Standard (recommended) CMYK profiles that can be used as a destination.
	   These profiles are also usable as a source. */
	AC_Selector_CMYK_StandardOutput		= FOUR_CHAR_CODE ('cStd'),

	/** CMYK profiles that can be used as a source. These profiles may or
	   may not be usable as a destination. This constant does not include profiles selected
	   by AC_Selector_CMYK_StandardInput or AC_Selector_CMYK_StandardOutput. */
	AC_Selector_CMYK_OtherInputCapable		= FOUR_CHAR_CODE ('cInp'),

    /** CMYK profiles that can be used as a destination. These profiles are
	   also usable as a source. This constant does not include profiles selected
	   by AC_Selector_CMYK_StandardOutput. */
	AC_Selector_CMYK_OtherOutputCapable	= FOUR_CHAR_CODE ('cOut'),

	/** Standard (recommended) grayscale profiles. These profiles are always
	   bidirectional. */
	AC_Selector_Gray_Standard				= FOUR_CHAR_CODE ('gStd'),

	/** Grayscale profiles that can be used as a source. These profiles may
	   or may not be usable as a destination. This constant does not include profiles selected
	   by AC_Selector_Gray_Standard. */
	AC_Selector_Gray_OtherInputCapable		= FOUR_CHAR_CODE ('gInp'),

    /** Grayscale profiles that can be used as a destination. These profiles
       are also usable as a source. This constant does not include profiles selected
	   by AC_Selector_Gray_Standard. */
	AC_Selector_Gray_OtherOutputCapable	= FOUR_CHAR_CODE ('gOut'),

	/** Standard dot gain profiles. This constant is used by Photoshop to represent a single 
	   ink's dot gain curve, and is stored as an ICC gray output profile. */
	AC_Selector_DotGain_Standard			= FOUR_CHAR_CODE ('dStd'),

	/** Other grayscale printer profiles. This constant does not include profiles selected by
	   AC_Selector_DotGain_Standard, and does not include grayscale display
	   profiles. */
	AC_Selector_DotGain_Other				= FOUR_CHAR_CODE ('dOth'),

	/** PhotoYCC profiles that can be used as a source. */
	AC_Selector_PhotoYCC_InputCapable		= FOUR_CHAR_CODE ('iYCC'),

	/** This constant forces the enum to be 32 bits wide. */
    AC_Selector_MaxEnum			 		= 0xFFFFFFFFL
	} AC_SelectorCode;


/** 
	Error codes returned by AcroColor functions.
*/
typedef enum
{

	/** No error. */
	AC_Error_None				= 0,	

	/** Other error. */
	AC_Error_General			= FOUR_CHAR_CODE ('gen '),
	
	/** Bad parameters to an API call. */
	AC_Error_Param				= FOUR_CHAR_CODE ('parm'),

	/** Application and ACE library mismatch. */
	AC_Error_Version			= FOUR_CHAR_CODE ('ver '),

	/** The user aborted the operation. Returned by ACE when the
	   client progress callback returns <code>false</code>. */
	AC_Error_UserAbort			= FOUR_CHAR_CODE ('abrt'),

	/** Out of memory. */
	AC_Error_Memory			= FOUR_CHAR_CODE ('memF'),
	
	/** Out of stack space. */
	AC_Error_StackFull			= FOUR_CHAR_CODE ('stkF'),

	/** Client callback ran out of scratch space. */
	AC_Error_ScratchFull		= FOUR_CHAR_CODE ('scrF'),
		
	/** String does not fit in supplied buffer. */
	AC_Error_StringOverflow	= FOUR_CHAR_CODE ('strO'),

	/** String does not contain ASCII data. */
	AC_Error_NoASCII			= FOUR_CHAR_CODE ('noA '),

	/** String does not contain Unicode data. */
	AC_Error_NoUnicode			= FOUR_CHAR_CODE ('noU '),
	
	/** String does not contain localized data. */
	AC_Error_NoLocalized		= FOUR_CHAR_CODE ('noL '),

	/** Data is not correctly byte aligned. */
	AC_Error_BadAlignment		= FOUR_CHAR_CODE ('alig'),

	/** Invalid profile description. */
	AC_Error_BadDescription	= FOUR_CHAR_CODE ('bDes'),

	/** Unable to concatenate transforms. */
	AC_Error_BadConcat			= FOUR_CHAR_CODE ('bCat'),

	/** Unable to merge transforms. */
	AC_Error_BadMerge			= FOUR_CHAR_CODE ('bMrg'),

	/** Invalid profile. */
	AC_Error_BadProfile		= FOUR_CHAR_CODE ('bPro'),

	/** Unsupported CMS. */
	AC_Error_UnsupCMS			= FOUR_CHAR_CODE ('uCMS'),

	/** Unsupported ACE option. */
	AC_Error_UnsupOption		= FOUR_CHAR_CODE ('uOpt'),

	/** Unsupported packing code. */
	AC_Error_UnsupPacking		= FOUR_CHAR_CODE ('uPac'),

	/** Unsupported profile version. */
	AC_Error_UnsupProfile		= FOUR_CHAR_CODE ('uPro'),

	/** Unsupported profile code. */
	AC_Error_UnsupProfileCode	= FOUR_CHAR_CODE ('uPrC'),

	/** Unsupported color space. */
	AC_Error_UnsupSpace		= FOUR_CHAR_CODE ('uSpc'),
	
	/** Unsupported query code. */
	AC_Error_UnsupQuery		= FOUR_CHAR_CODE ('uQry'),
	
	/** A profile was missing from the disk. */
	AC_Error_MissingProfile	= FOUR_CHAR_CODE ('misP'),

	/** The profile on disk has been modified. */
	AC_Error_ModifiedProfile	= FOUR_CHAR_CODE ('modP'),

	/** File is missing from disk. */
	AC_Error_FileNotFound		= FOUR_CHAR_CODE ('fnf '),	

	/** End of file error. */
	AC_Error_EOF				= FOUR_CHAR_CODE ('eof '),

	/** File locked error. */
	AC_Error_FileLocked		= FOUR_CHAR_CODE ('flck'),

	/** Disk I/O error. */
	AC_Error_DiskIO			= FOUR_CHAR_CODE ('io  '),

	/** A problem using ColorSync. */
	AC_Error_ColorSync			= FOUR_CHAR_CODE ('csE '),

	/** A problem using ICM. */
	AC_Error_ICM				= FOUR_CHAR_CODE ('icmE'),

	/** The color settings does not contain this key. */
	AC_Error_MissingKey		= FOUR_CHAR_CODE ('mKey'),
	
	/** The color settings file is invalid. */
	AC_Error_InvalidSettings	= FOUR_CHAR_CODE ('iSet'),
	
	/** The color settings file is an incompatible version. */
	AC_Error_SettingsVersion	= FOUR_CHAR_CODE ('vSet'),

	/** The function is not implemented (subsetted library). */
	AC_Error_NotImplemented	= FOUR_CHAR_CODE ('nImp'),

	/** This constant forces the <code>enum</code> to be 32 bits wide. */
    AC_Error_MaxEnum			= 0xFFFFFFFFL
	
} AC_Error;

/** Constants that describe the type of a device color profile. 
	@see ACProfileFromCode
	@see ACProfileListItemCode
*/
typedef enum
{
	
	/** A <code>NULL</code> result, indication that a profile is not a built-in profile. */
	AC_Profile_Null				= 0,
	
	/** Adobe's standard Lab profile.  It has a white point of D50, and
	   exactly matches the ICC's Lab PCS space. */
	AC_Profile_Lab_D50				= FOUR_CHAR_CODE ('LD50'),
	
	/** An XYZ profile that exactly matches the ICC's XYZ PCS space. */
	AC_Profile_PCS_XYZ				= FOUR_CHAR_CODE ('pXYZ'),	

	/** An XYZ profile with a flat white point encoding (<code>X = Y = Z = 1.0</code>).
	   Photoshop uses this as an intermediate space in its display loop. */
	AC_Profile_Flat_XYZ			= FOUR_CHAR_CODE ('fXYZ'),

	/** HP's sRGB profile. The default Windows monitor profile. */
	AC_Profile_sRGB				= FOUR_CHAR_CODE ('sRGB'),
       
	/** Default RGB space using by Photoshop 4.0 and earlier. The
	   default Mac OS monitor profile.  */
	AC_Profile_AppleRGB			= FOUR_CHAR_CODE ('aRGB'),

	/** A wider gamut RGB space recommended by Adobe. */
	AC_Profile_AdobeRGB1998		= FOUR_CHAR_CODE ('AS98'),
       
	/** A simplified version of Radius' ColorMatch RGB space, without
	   Radius' non-zero black point. */
	AC_Profile_ColorMatchRGB		= FOUR_CHAR_CODE ('cmat'),   

    /** Grayscale display profile with a gamma of 18. */
    AC_Profile_Gamma18 			= FOUR_CHAR_CODE ('GG18'),

    /** Grayscale display profile with a gamma of 22. */
    AC_Profile_Gamma22 			= FOUR_CHAR_CODE ('GG22'),

    /** Grayscale printer profile with a dot gain of 10. */
    AC_Profile_DotGain10 			= FOUR_CHAR_CODE ('DG10'),

    /** Grayscale printer profile with a dot gain of 15. */
    AC_Profile_DotGain15 			= FOUR_CHAR_CODE ('DG15'),

    /** Grayscale printer profile with a dot gain of 20. */
    AC_Profile_DotGain20 			= FOUR_CHAR_CODE ('DG20'),

    /** Grayscale printer profile with a dot gain of 25. */
    AC_Profile_DotGain25 			= FOUR_CHAR_CODE ('DG25'),

    /** Grayscale printer profile with a dot gain of 30. */
    AC_Profile_DotGain30 			= FOUR_CHAR_CODE ('DG30'),

    /** The system "Monitor RGB" profile, which is the same profile as that
       returned by AC_MainMonitorProfile. */
    AC_Profile_MonitorRGB			= FOUR_CHAR_CODE ('mRGB'),

    /** The system default profiles for RGB color space.  (Currently a ColorSync 3.0 only feature.) */
    AC_Profile_SystemRGB			= FOUR_CHAR_CODE ('sysR'),

    /** The system default profiles for CMYK color space.  (Currently a ColorSync 3.0 only feature.) */
    AC_Profile_SystemCMYK			= FOUR_CHAR_CODE ('sysC'),

    /** The system default profiles for Gray color space.  (Currently a ColorSync 3.0 only feature.) */
    AC_Profile_SystemGray			= FOUR_CHAR_CODE ('sysG'), 

    /** The system default profiles for input device type. (Currently a ColorSync 3.0 only feature.) */
    AC_Profile_SystemInput			= FOUR_CHAR_CODE ('sysI'),

    /** The system default profiles for output device type.  (Currently a ColorSync 3.0 only feature.) */
    AC_Profile_SystemOutput		= FOUR_CHAR_CODE ('sysO'),

    /** The system default profiles for proofer device type.  (Currently a ColorSync 3.0 only feature.) */
    AC_Profile_SystemProofer		= FOUR_CHAR_CODE ('sysP'),
    
    /** The application working (RGB) color space profiles. (For use as
       abstact values only, since ACE does not keep track of these
       profiles, and thus cannot make profiles from these codes.) */
    AC_Profile_WorkingRGB			= FOUR_CHAR_CODE ('wRGB'),

    /** The application working (CMYK) color space profiles. (For use as
       abstact values only, since ACE does not keep track of these
       profiles, and thus cannot make profiles from these codes.) */
    AC_Profile_WorkingCMYK			= FOUR_CHAR_CODE ('wCMY'),

    /** The application working (gray) color space profiles. (For use as
       abstact values only, since ACE does not keep track of these
       profiles, and thus cannot make profiles from these codes.) */
    AC_Profile_WorkingGray			= FOUR_CHAR_CODE ('wGry'),
       
	/** This constant forces the enum to be 32 bits wide. */
	AC_Profile_MaxEnum			 	= 0xFFFFFFFFL
	
} AC_ProfileCode;


/** 
	Constants that specify the packing used in a color transformation.
*/
typedef enum
{
	
	/** 8-bit RGB (or grayscale destination), with a leading pad byte.
	   When grayscale is output in this format, the gray value is replicated
	   into to the R, G, and B values.
	   <p><code>R, G, B = 0</code> is black.</p>
	   <p><code>R, G, B = 255</code> is white.</p>
	   <p>Data must be 32-bit aligned.</p> */
    AC_Packing_pRGB8			= FOUR_CHAR_CODE ('prgb'),

    /** Same as AC_Packing_pRGB8, without the leading pad byte.
       Data need only be 8-bit aligned. */
    AC_Packing_RGB8			= FOUR_CHAR_CODE ('rgb '),  

    /** 15+ bit RGB (or grayscale destination), with a leading pad word.
	   When grayscale is output in this format, the gray value is replicated
	   into to the R, G, and B values.
       <p><code>R, G, B = 0</code> is black.</p>
       <p><code>R, G, B = 32768</code> is white.</p>
       <p>Values greater than <code>32768</code> are invalid.</p>
 	   <p>Data must be 64-bit aligned.</p> */
   AC_Packing_pRGB15			= FOUR_CHAR_CODE ('PRGB'),
   
	/** 8-bit CMYK.
	   <p><code>C, M, Y, K = 0</code> is 100% ink.</p>
	   <p><code>C, M, Y, K = 255</code> is 0% ink.</p>
	   <p>Data must be 32-bit aligned.</p> */
	AC_Packing_CMYK8_Black0	= FOUR_CHAR_CODE ('cmyk'),
    
	/** Same as AC_Packing_CMYK8_Black0 with inverse encoding. */
	AC_Packing_CMYK8_White0	= FOUR_CHAR_CODE ('cmyw'),
    
    /** 15+ bit CMYK.
	   <p><code>C, M, Y, K = 0</code> is 100% ink.</p>
	   <p><code>C, M, Y, K = 32768</code> is 0% ink.</p>
       <p>Values greater than <code>32768</code> are invalid.</p>
 	   <p>Data must be 64-bit aligned.</p> */
    AC_Packing_CMYK15_Black0	= FOUR_CHAR_CODE ('CMYK'), 

    /** 8-bit LAB, with a leading pad byte.
       <p><code>L = 0</code>  means <code>L* = 0.0</code></p>
       <p><code>L = 255</code> means <code>L* = 100.0</code></p>
       <p><code>a, b = 0</code> means <code>a*, b* = -128.0</code></p>
       <p><code>a, b = 128</code> means <code>a*, b* = 0.0</code></p>
       <p><code>a, b = 255</code> means <code>a*, b* = +127.0</code></p>
	   <p>Data must be 32-bit aligned.</p> */
    AC_Packing_pLab8			= FOUR_CHAR_CODE ('plab'),

    /** Same as AC_Packing_pLab8, without the leading pad byte.
        <p>Data need only be 8-bit aligned.</p> */
    AC_Packing_Lab8			= FOUR_CHAR_CODE ('lab '),
    
    /** 15+ bit LAB, with a leading pad word.
       <p><code>L = 0</code>  means <code>L* = 0.0</code></p>
       <p><code>L = 32768</code> means <code>L* = 100.0</code></p>
       <p><code>a, b = 0</code> means <code>a*, b* = -128.0</code></p>
       <p><code>a, b = 16384</code> means <code>a*, b* = 0.0</code></p>
       <p><code>a, b = 32768</code> means <code>a*, b* = +128.0</code></p>
       <p>Values greater than <code>32768</code> are invalid.</p>
	   <p>Data must be 64-bit aligned.</p> */
    AC_Packing_pLab15			= FOUR_CHAR_CODE ('PLAB'),
	
	/** 8-bit grayscale or gamut test results, no padding.
	   <p><code>G = 0</code> is 100% ink or black or fully out of gamut.</p>
	   <p><code>G = 255</code> is 0% ink or white or fully in gamut.</p>
	   <p>When used for gamut test results, any value greater than or equal to <code>128</code> should be
	   considered to be in gamut.</p> */
	AC_Packing_Gray8_Black0	= FOUR_CHAR_CODE ('g8k0'),

	/** Same as AC_Packing_Gray8_Black0 with inverse encoding. */
	AC_Packing_Gray8_White0	= FOUR_CHAR_CODE ('g8w0'),
	
	/** 15+ bit grayscale or gamut test results, no padding.
	   <p><code>G = 0</code> is 100% ink or black or fully out of gamut.</p>
	   <p><code>G = 32768</code> is 0% ink or white or fully in gamut.</p>
       <p>Values greater than <code>32768</code> are invalid.</p>
	   <p>Data must be 16-bit aligned.</p> */
	 AC_Packing_Gray15_Black0	= FOUR_CHAR_CODE ('G15K'),
	
    /** 16-bit XYZ, with a leading pad word.
       <p><code>X, Y, Z = 0</code> means <code>X, Y, Z = 0.0</code></p>
       <p><code>X, Y, Z = 32768</code> means <code>X, Y, Z = 1.0</code></p>
       <p><code>X, Y, Z = 65535</code> means <code>X, Y, Z = 1.9999694824</code>.</p>
	   <p>Data must be 64-bit aligned.</p> */
    AC_Packing_pXYZ16			= FOUR_CHAR_CODE ('PXYZ'),
	
	/** Generic padded 3-component 8-bit packing.
	   Data must be 32-bit aligned. */
	AC_Packing_pABC8			= FOUR_CHAR_CODE ('pabc'),
	
    /** Same as AC_Packing_pABC8, without the leading pad byte.
       Data need only be 8-bit aligned. */
    AC_Packing_ABC8			= FOUR_CHAR_CODE ('abc '),
    
	/** Generic padded 3-component 15+ bit packing.
	   Data must be 64-bit aligned. */
	AC_Packing_pABC15			= FOUR_CHAR_CODE ('pABC'),
	
	/** Generic 4-component 8-bit packing.
	   Data must be 32-bit aligned. */
	AC_Packing_ABCD8			= FOUR_CHAR_CODE ('abcd'),
	
	/** Generic 4-component 15+ bit packing.
	   Data must be 64-bit aligned. */
	AC_Packing_ABCD15			= FOUR_CHAR_CODE ('ABCD'),
	
	/** Packing codes for native 64-bit (gray) ColorSync formats (type "CMColor").
	   ICM2 also uses these packings formats (type "COLOR").
	   See the Apple ColorSync documentation for the details of these
	   formats. These are mostly intended for internal use by ACE,
	   and are not intended for use by most ACE clients.
	   Data must be 16-bit aligned. */
	AC_Packing_CS64_Gray		= FOUR_CHAR_CODE ('CS01'),

	/** Packing codes for native 64-bit (RGB) ColorSync formats (type "CMColor").
	   ICM2 also uses these packings formats (type "COLOR").
	   See the Apple ColorSync documentation for the details of these
	   formats. These are mostly intended for internal use by ACE,
	   and are not intended for use by most ACE clients.
	   Data must be 16-bit aligned. */
	AC_Packing_CS64_RGB		= FOUR_CHAR_CODE ('CS02'),

	/** Packing codes for native 64-bit (CMYK) ColorSync formats (type "CMColor").
	   ICM2 also uses these packings formats (type "COLOR").
	   See the Apple ColorSync documentation for the details of these
	   formats. These are mostly intended for internal use by ACE,
	   and are not intended for use by most ACE clients.
	   Data must be 16-bit aligned. */
	AC_Packing_CS64_CMYK		= FOUR_CHAR_CODE ('CS03'),

	/** Packing codes for native 64-bit (Lab) ColorSync formats (type "CMColor").
	   ICM2 also uses these packings formats (type "COLOR").
	   See the Apple ColorSync documentation for the details of these
	   formats. These are mostly intended for internal use by ACE,
	   and are not intended for use by most ACE clients.
	   Data must be 16-bit aligned. */
	AC_Packing_CS64_Lab		= FOUR_CHAR_CODE ('CS04'),

	/** Packing codes for native 64-bit (xyz) ColorSync formats (type "CMColor").
	   ICM2 also uses these packings formats (type "COLOR").
	   See the Apple ColorSync documentation for the details of these
	   formats. These are mostly intended for internal use by ACE,
	   and are not intended for use by most ACE clients.
	   Data must be 16-bit aligned. */
	AC_Packing_CS64_XYZ		= FOUR_CHAR_CODE ('CS05'),

	/** Packing codes for native 64-bit (abc) ColorSync formats (type "CMColor").
	   ICM2 also uses these packings formats (type "COLOR").
	   See the Apple ColorSync documentation for the details of these
	   formats. These are mostly intended for internal use by ACE,
	   and are not intended for use by most ACE clients.
	   Data must be 16-bit aligned. */
	AC_Packing_CS64_ABC		= FOUR_CHAR_CODE ('CS06'),

	/** Packing codes for native 64-bit (abcd)ColorSync formats (type "CMColor").
	   ICM2 also uses these packings formats (type "COLOR").
	   See the Apple ColorSync documentation for the details of these
	   formats. These are mostly intended for internal use by ACE,
	   and are not intended for use by most ACE clients.
	   Data must be 16-bit aligned. */
	AC_Packing_CS64_ABCD		= FOUR_CHAR_CODE ('CS07'),
	
	/** <code>NULL</code> data, for use with data in AC_Space_Null. */
	AC_Packing_Null			= FOUR_CHAR_CODE ('null'),
	
	/** None of the above; use the general packing specification. */
	AC_Packing_General			= 0,
	
	/** This constant forces the <code>enum</code> to be 32 bits wide. */
    AC_Packing_MaxEnum			= 0xFFFFFFFFL
	
} AC_PackingCode;
#endif

/** Constants that specify a standard ICC rendering intent for a device color profile. The
	rendering intent specifies the color translation method for colors that are outside the
	gamut of the color profile.
	@see ACMakeCalGray
	@see ACMakeCalLab
	@see ACMakeCalRGB
*/
typedef enum
{
	
	/** Tries to preserve the visual relationship between colors in
		a way that is perceived as natural to the human eye,
		although the color values themselves may change. This is
		the same as the Image intent in Adobe PageMaker and
		Illustrator. This option is suitable for photographic,
		continuous tone images.
	 */
	AC_Perceptual				= 0,

   	/** The same as absolute colorimetric, except that it
		compares the white point of the source color space to
		that of the destination color space and shifts all other
		colors accordingly, rather than comparing individual
		colors.
	*/
	AC_RelColorimetric			= 1,

	/** Tries to create vivid color at the expense of accurate
		color. It scales the source gamut to the destination
		gamut, but preserves relative saturation instead of hue,
		so that when scaling to a smaller gamut, hues may shift.
		This is the same as the Graphics intent in Adobe
		PageMaker and Illustrator. This option is suitable for
		business graphics and charts, where the exact
		relationship between colors is not as important as having
		bright saturated colors.
	 */
    AC_Saturation				= 2,

	/** Tries to maintain color accuracy at the expense of
		preserving relationships between colors. It leaves colors
		that fall inside the destination gamut unchanged. When
		translating to a smaller gamut, two colors that are
		distinct in the source space may be mapped to the same
		color in the destination space. This type of rendering can
		be more accurate than AC_RelColorimetric if the color
		profile of the image contains correct white point
		(extreme highlight) information.
	*/
    AC_AbsColorimetric			= 3,
    
    /** Use the source profile's rendering intent. */
    AC_UseProfileIntent		= 0xFFFFFFFFL
} AC_RenderIntent;


#ifdef WIN_ENV
#define kACMaxPathLength	260
#else
#define kACMaxPathLength	256
#endif


/** 
	Contains a platform-specific version of a file specification. This is an FSSpec on 
	Mac OS, or a full path name on other platforms.
	@see ACLoadSettings
	@see ACPresetFileToName
	@see ACPresetListItemFile
*/
#if (!MAC_PLATFORM || !AS_ARCH_64BIT)
typedef struct _t_AC_FileSpec
{
#if defined(MAC_ENV) && !defined(IOS_ENV)
    
    /** On Mac OS, use an FSSpec. */
    FSSpec spec;
	#else
	
	/** Use a full path name when not using Mac OS. */
	char path [kACMaxPathLength];
	
	#endif
	
} AC_FileSpec;
#endif
/* color types */

/** Floating-point XYZ color.  A pure black would be encoded as <code>0.0, 0.0, 0.0</code>, 
   while a D50-based pure white would be encoded as <code>0.9642, 1.0, 0.8249</code>. 
   @see ACCalCMYK
   @see ACCalGray
   @see ACCalLab
   @see ACCalRGB
*/
typedef struct _t_ACXYZColor
{
	/** X-component of a floating-point XYZ color. */
	double		X;

	/** Y-component of a floating-point XYZ color. */
	double		Y;

	/** Z-component of a floating-point XYZ color. */
	double		Z;	
} AC_XYZColor;

/******************************************************************************/


/** Floating-point xy chromaticity coordinate. These can be computed from XYZ
   colors using <code>x = X / (X + Y + Z)</code> and <code>y = Y / (X + Y + Z)</code>. */
typedef struct _t_ACXYColor
	{		
	double		x;
	double		y;
	} AC_XYColor;



/** A tone curve value for use in a calibrated CMYK color space specification. 
	@see ACCalCMYK
*/	
typedef struct _t_ACToneCurve
	{
	/** The number of bytes per value (1 or 2). */
	ASUns32		bytesPerValue;		
	/** Number of samples (often 256). */
	ASUns32		count;				
	/** A pointer to the samples. */
	void			*data;				
	} AC_ToneCurve;

/******************************************************************************/


/** <p>A simple 16-patch calibrated CMYK color space specification.</p>
    <p>The 16 <code>AC_XYZColor</code> values are the absolute XYZ values of all 16 CMYK ink combinations, including
	paper white.  The illuminant is assumed to be D50.</p>
	<p>The <code>AC_ToneCurve</code> values represent the dot gain curves for each ink. 
	Each one maps from ink percentage to the dot area percentage.</p>
*/
typedef struct _t_ACCalCMYK
	{

	/** Absolute XYZ value of a CMYK ink combination: paper white. */
	AC_XYZColor	w;

	/** Absolute XYZ value of a CMYK ink combination: K. */
	AC_XYZColor	k;

	/** Absolute XYZ value of a CMYK ink combination: C. */
	AC_XYZColor	c;

	/** Absolute XYZ value of a CMYK ink combination: M. */
	AC_XYZColor	m;

	/** Absolute XYZ value of a CMYK ink combination: Y. */
	AC_XYZColor	y;

	/** Absolute XYZ value of a CMYK ink combination: CM. */
	AC_XYZColor	cm;

	/** Absolute XYZ value of a CMYK ink combination: CY. */
	AC_XYZColor	cy;

	/** Absolute XYZ value of a CMYK ink combination: CK. */
	AC_XYZColor	ck;

	/** Absolute XYZ value of a CMYK ink combination: MY. */
	AC_XYZColor	my;

	/** Absolute XYZ value of a CMYK ink combination: MK. */
	AC_XYZColor	mk;

	/** Absolute XYZ value of a CMYK ink combination: YK. */
	AC_XYZColor	yk;

	/** Absolute XYZ value of a CMYK ink combination: CMY. */
	AC_XYZColor	cmy;

	/** Absolute XYZ value of a CMYK ink combination: CMK. */
	AC_XYZColor	cmk;

	/** Absolute XYZ value of a CMYK ink combination: CYK. */
	AC_XYZColor	cyk;

	/** Absolute XYZ value of a CMYK ink combination: MYK. */
	AC_XYZColor	myk;

	/** Absolute XYZ value of a CMYK ink combination: CMYK. */
	AC_XYZColor	cmyk;

	/** Dot gain curve for the cyan ink. */
	AC_ToneCurve	cTRC;

	/** Dot gain curve for the magenta ink. */
	AC_ToneCurve	mTRC;

	/** Dot gain curve for the yellow ink. */
	AC_ToneCurve	yTRC;

	/** Dot gain curve for the black ink. */
	AC_ToneCurve	kTRC;

	/** Adds a gamma curve to the output XYZ values,
	    after the CLUT.  This was used by old versions of Photoshop,
		and is now obsolete. It should always be set to <code>1.0</code>. */
	double			opticalGamma;

	/** The absolute XYZ value of paper white. It should be a duplicate of the
	    <code>w</code> entry. */
	AC_XYZColor	white;
	
	/** The absolute XYZ value of a legal (within total ink limits) four-color CMYK black. It is currently ignored. */
	AC_XYZColor	black;
	} ACCalCMYK;


/******************************************************************************/


/** Floating-point Lab color.  Pure white is encoded as <code>100.0, 0.0, 0.0</code>. The
   usual <code>a</code> and <code>b</code> range is <code>-128.0</code> to <code>+127.0</code>, but this structure supports any
   <code>a</code> and <code>b</code> range.
   @see ACMakeCalLab
*/
typedef struct _t_AC_LabColor
{
	double		L;
	double		a;
	double		b;
	} AC_LabColor;

/******************************************************************************/


/** A calibrated grayscale space specification.
	@see ACMakeCalGray
*/
typedef struct _t_ACCalGray
	{
	/** Gamma value. Normal values are greater than or equal to <code>1.0</code>. */

	double			gamma;

	/** Absolute XYZ value of a monitor white point (illuminant). The Y-value
	    should be equal to <code>1.0</code>. */

	AC_XYZColor	white;
	
	/** Absolute XYZ value of a monitor black point. */

	AC_XYZColor	black;
	} ACCalGray;					

/******************************************************************************/


/** A calibrated RGB space specification.
    The <code>red</code>, <code>green</code>, and <code>blue</code> members represent columns 
	of the matrix used to convert between RGB values and XYZ values.
	Normally, red + green + blue should exactly equal white (even if the
	monitor black point is non-zero). This means that, unless the monitor 
	black point is zero, these entries are not equal to the absolute
	XYZ values of the pure RGB components (due to non-zero offsets in the
	other two components). This interpretation of these entries is used to
	maximize compatibility with the PDF <code>CalRGB</code> color space, which is defined
	using a matrix (rather than component XYZ values).
	@see ACMakeCalRGB
*/
typedef struct _t_ACCalRGB
	{
	
	/** Red gamma value for each component. Normal values are greater than or
	    equal to <code>1.0</code>. */
	double			redGamma;

	/** Green gamma value for each component. Normal values are greater than or
	    equal to <code>1.0</code>. */
	double			greenGamma;

	/** Blue gamma value for each component. Normal values are greater than or
	    equal to <code>1.0</code>. */
	double			blueGamma;

	/** Red column of the matrix used to convert between RGB values and XYZ values. */
	AC_XYZColor	red;

	/** Green column of the matrix used to convert between RGB values and XYZ values. */
	AC_XYZColor	green;

	/** Blue column of the matrix used to convert between RGB values and XYZ values. */
	AC_XYZColor	blue;

	/** The absolute XYZ value of the monitor white point (illuminant). The Y-value
	    shall be equal to <code>1.0</code>. See <i>CalRGB Color Spaces</i> in the <i>PDF Reference</i>. */
	AC_XYZColor	white;

	/** The absolute XYZ value of the monitor black point. */
	AC_XYZColor	black;
	} ACCalRGB;					

/******************************************************************************/


/** 
	A calibrated Lab color space specification. The usual <code>min</code> and <code>max</code> values for <code>rangeA</code> and
	<code>rangeB</code> are <code>-128</code> and <code>+127</code>.
	@see ACMakeCalLab
*/
typedef struct _t_ACCalLab
	{

	/** Color space white point. The Y value should be equal to <code>1.0</code>. */

	AC_XYZColor	white;

	/** Color space black point. It is currently ignored by ACE when creating 
	    profiles, and is always set to zero when extracting from profiles. */
	   
	AC_XYZColor	black;

	/** Range limits for the <code>a*</code> component. Values outside this
	    range are clipped to this range. */

	struct
		{
		/** The usual value is <code>-128</code>. */
		ASInt32 min;		
		/** The usual value <code>+127</code>. */
		ASInt32 max;		
		} rangeA;

	/** Range limits for the <code>b*</code> component. Values outside this
	    range are clipped to this range. */

	struct
		{
		/** The usual value is <code>-128</code>. */
		ASInt32 min;		
		/** The usual value <code>+127</code>. */
		ASInt32 max;		
		} rangeB;
	} ACCalLab;					

/** Constant key values for an <code>AC_Settings</code> object. 
	@see ACGetSettingsProfile
	@see ACGetSettingsString
	@see ACGetSettingsUnsigned32
*/
#if 0
typedef enum
	{
	/** Settings file name string (if different than the file name). */
	AC_Key_Name				= FOUR_CHAR_CODE ('name'),

	/** Settings file description string. */
	AC_Key_Description			= FOUR_CHAR_CODE ('desc'),
	
	/** Name of application to write this settings file. */
	AC_Key_WriterName			= FOUR_CHAR_CODE ('wNam'),
	
	/** Working RGB profile. */
	AC_Key_WorkingRGB			= FOUR_CHAR_CODE ('wRGB'),
	
	/** Working CMYK profile. */
	AC_Key_WorkingCMYK			= FOUR_CHAR_CODE ('wCMY'),
	
	/** Working gray profile. */
	AC_Key_WorkingGray			= FOUR_CHAR_CODE ('wGry'),
	
	/** Working spot profile. */
	AC_Key_WorkingSpot			= FOUR_CHAR_CODE ('wSpt'),
	
	/** RGB color management policy (AC_Policy <code>enum</code>). */
	AC_Key_PolicyRGB			= FOUR_CHAR_CODE ('pRGB'),
	
	/** CMYK color management policy (AC_Policy <code>enum</code>). */
	AC_Key_PolicyCMYK			= FOUR_CHAR_CODE ('pCMY'),
	
	/** Gray color management policy (AC_Policy <code>enum</code>). */
	AC_Key_PolicyGray			= FOUR_CHAR_CODE ('pGry'),
	
	/** Ask about profile mismatches when opening (<code>0</code> = no, <code>1</code> = yes). */
	AC_Key_MismatchAskOpening	= FOUR_CHAR_CODE ('mAsk'),
	
	/** Ask about profile mismatches when pasting (<code>0</code> = no, <code>1</code> = yes). */
	AC_Key_MismatchAskPasting	= FOUR_CHAR_CODE ('pAsk'),
	
	/** Ask about missing profile when opening (<code>0</code> = no, <code>1</code> = yes). */
	AC_Key_MissingAskOpening	= FOUR_CHAR_CODE ('misA'),
	
	/** Conversion engine CMS code (4-char code, stored as <code>unsigned32</code>). */
	AC_Key_EngineCMS			= FOUR_CHAR_CODE ('eCMS'),
	
	/** Conversion engine CMM code (4-char code, stored as <code>unsigned32</code>). */
	AC_Key_EngineCMM			= FOUR_CHAR_CODE ('eCMM'),
	
	/** Conversion intent (standard ICC integer encoding). */
	AC_Key_Intent				= FOUR_CHAR_CODE ('cInt'),
	
	/** Conversion black point compensation (<code>0</code> = no, <code>1</code> = yes). */
	AC_Key_KPC					= FOUR_CHAR_CODE ('kpc '),
	
	/** Dither conversions between 8-bit color spaces (<code>0</code> = no, <code>1</code> = yes). */
	AC_Key_Dither				= FOUR_CHAR_CODE ('dith'),
	
	/** Enable or disable monitor compression (<code>0</code> = off, <code>1</code> = on). */
	AC_Key_CompressionEnabled	= FOUR_CHAR_CODE ('mce '),
	
	/** Monitor compression percent (in percent). */
	AC_Key_CompressionPercent	= FOUR_CHAR_CODE ('mcp '),
	
	/** Enable or disable RGB blending gamma (<code>0</code> = off, <code>1</code> = on). */
	AC_Key_BlendGammaEnabled	= FOUR_CHAR_CODE ('bge '),
	
	/** RGB blending gamma value (<code>100</code> = gamma 1.00). */
	AC_Key_BlendGammaValue		= FOUR_CHAR_CODE ('bgv '),
	
	/** Proof type (AC_ProofType <code>enum</code>). */
	AC_Key_ProofType			= FOUR_CHAR_CODE ('pTyp'),
	
	/** Proof profile. */
	AC_Key_ProofProfile		= FOUR_CHAR_CODE ('pPrf'),
	
	/** Display simulation (AC_Simulate <code>enum</code>). */
	AC_Key_Simulate			= FOUR_CHAR_CODE ('dSim'),
	
	/** This constant forces the <code>enum</code> to be 32 bits wide. */
    AC_Key_MaxEnum				= 0xFFFFFFFFL
	
} AC_SettingsKey;


/** Constant values that determine the type of an AC_Settings object.
	@see ACMakePresetList
	@see ACMakeSettings
*/
typedef enum
{
	/** Used to hold global color settings, such as working spaces. */
	AC_SettingsType_Color		= FOUR_CHAR_CODE ('AsCs'),
	
	/** Used to specify the parameters for proofing a document. 
		The Proof Setup Files generally control a per-window setting. */
	AC_SettingsType_Proof		= FOUR_CHAR_CODE ('AsPs'),
	
	/** This constant forces the <code>enum</code> to be 32 bits wide. */
    AC_SettingsType_MaxEnum	= 0xFFFFFFFFL
} AC_SettingsType;

/** 
	Constant values for ICC color space signatures.
	@see ACProfileColorSpace
*/
typedef enum
{

	/** 
		@ingroup StdICCColorSpaceSignatures
	*/
    AC_Space_XYZ				= FOUR_CHAR_CODE ('XYZ '),
	/** 
		@ingroup StdICCColorSpaceSignatures
	*/
    AC_Space_Lab				= FOUR_CHAR_CODE ('Lab '),
	/** 
		@ingroup StdICCColorSpaceSignatures
	*/
    AC_Space_RGB				= FOUR_CHAR_CODE ('RGB '),
	/** 
		@ingroup StdICCColorSpaceSignatures
	*/
    AC_Space_Gray				= FOUR_CHAR_CODE ('GRAY'),
	/** 
		@ingroup StdICCColorSpaceSignatures
	*/
    AC_Space_CMYK				= FOUR_CHAR_CODE ('CMYK'),
	/** 
		@ingroup StdICCColorSpaceSignatures
	*/
    AC_Space_Luv				= FOUR_CHAR_CODE ('Luv '),
	/** 
		@ingroup StdICCColorSpaceSignatures
	*/
    AC_Space_YCbCr				= FOUR_CHAR_CODE ('YCbr'),
	/** 
		@ingroup StdICCColorSpaceSignatures
	*/
    AC_Space_HSV				= FOUR_CHAR_CODE ('HSV '),
	/** 
		@ingroup StdICCColorSpaceSignatures
	*/
    AC_Space_HLS				= FOUR_CHAR_CODE ('HLS '),
	/** 
		@ingroup StdICCColorSpaceSignatures
	*/
    AC_Space_CMY				= FOUR_CHAR_CODE ('CMY '),

	/** 
		@ingroup GenericICCColorSpaceSignatures
	*/
    AC_Space_2Component		= FOUR_CHAR_CODE ('2CLR'),
	/** 
		@ingroup GenericICCColorSpaceSignatures
	*/
    AC_Space_3Component		= FOUR_CHAR_CODE ('3CLR'),
	/** 
		@ingroup GenericICCColorSpaceSignatures
	*/
    AC_Space_4Component		= FOUR_CHAR_CODE ('4CLR'),
	/** 
		@ingroup GenericICCColorSpaceSignatures
	*/
    AC_Space_5Component		= FOUR_CHAR_CODE ('5CLR'),
	/** 
		@ingroup GenericICCColorSpaceSignatures
	*/
    AC_Space_6Component		= FOUR_CHAR_CODE ('6CLR'),
	/** 
		@ingroup GenericICCColorSpaceSignatures
	*/
    AC_Space_7Component		= FOUR_CHAR_CODE ('7CLR'),
	/** 
		@ingroup GenericICCColorSpaceSignatures
	*/
    AC_Space_8Component		= FOUR_CHAR_CODE ('8CLR'),
	/** 
		@ingroup GenericICCColorSpaceSignatures
	*/
    AC_Space_9Component		= FOUR_CHAR_CODE ('9CLR'),
	/** 
		@ingroup GenericICCColorSpaceSignatures
	*/
    AC_Space_10Component		= FOUR_CHAR_CODE ('ACLR'),
	/** 
		@ingroup GenericICCColorSpaceSignatures
	*/
    AC_Space_11Component		= FOUR_CHAR_CODE ('BCLR'),
	/** 
		@ingroup GenericICCColorSpaceSignatures
	*/
    AC_Space_12Component		= FOUR_CHAR_CODE ('CCLR'),
	/** 
		@ingroup GenericICCColorSpaceSignatures
	*/
    AC_Space_13Component		= FOUR_CHAR_CODE ('DCLR'),
	/** 
		@ingroup GenericICCColorSpaceSignatures
	*/
    AC_Space_14Component		= FOUR_CHAR_CODE ('ECLR'),
	/** 
		@ingroup GenericICCColorSpaceSignatures
	*/
    AC_Space_15Component		= FOUR_CHAR_CODE ('FCLR'),
	/** Kodak's PhotoYCC space is stored as a generic 3-component space. 
		@ingroup GenericICCColorSpaceSignatures
	*/
    AC_Space_PhotoYCC			= AC_Space_3Component,
	/** A null color space.  Used to represent spot-only color spaces. 
		@ingroup GenericICCColorSpaceSignatures
	*/
	AC_Space_Null				= 0,

	/** This constant forces the enum to be 32 bits wide. */
	AC_Space_MaxEnum			= 0xFFFFFFFFL
	
} AC_ColorSpace;
#endif

/** Constants that specify the color space of working profiles.
	This enumeration is added for the purpose of ACGetWorkingSpaceProfile().
	The profile returned by this function must be unreferenced by the caller.
	@see ACGetWorkingSpaceProfile
 */
typedef enum
{
	/** Grayscale profile. */
	kACWorkingGray = 0,
	/** RGB profile. */
	kACWorkingRGB = 1,
	/** CMYK profile. */
	kACWorkingCMYK = 2,
	/** Working spaces. */
	kACWorkingSpaces = 3
}ACWorkingSpace;

#ifdef WIN_ENV
#define kACEMaxPathLength	260
#else
#define kACEMaxPathLength	256
#endif

//typedef struct _t_PDColorConvertParamsRec *PDColorConvertParams;

/*  Object Attributes: these are arranged as a bitmap. In order to */
/*  match, one must satisfy all ON bits. There are obviously combinations */
/*  that are redundant or don't make sense. */
#define _FLG(n) (1 << n)
    
/** Object attributes: these are arranged as a bitmap. */
typedef enum
{
	/** Object is an image. */
    kColorConvObj_Image         =      _FLG(0),   

	/** Object is a JPEG image. */
    kColorConvObj_JPEG          =      _FLG(1),    

	/** Object is a JPEG2000 image. */
    kColorConvObj_JPEG2000      =      _FLG(2),    

	/** Image is in a lossy space. */
    kColorConvObj_Lossy         =      _FLG(3),    

	/** Image is in a non-lossy space. */
    kColorConvObj_Lossless      =      _FLG(4),    

	/** Object is text. */
    kColorConvObj_Text          =      _FLG(5),    

	/** Object is line-art (fill, stroke). */
    kColorConvObj_LineArt       =      _FLG(6),    

	/** Object is a smooth shade. */
    kColorConvObj_Shade         =      _FLG(7),    

	/** Object is not opaque. */
    kColorConvObj_Transparent   =      _FLG(8),    

	/** Object overprints. */
    kColorConvObj_Overprinting  =      _FLG(9),    

	/** Overprint mode (OPM) is set to <code>1</code>. */
    kColorConvObj_OverprintMode =      _FLG(10),   

	/** Any object. */
    kColorConvObj_AnyObject     =      (_FLG(0)|_FLG(5)|_FLG(6)|_FLG(7)),

	/** Maximum enum value. */
    kColorConvObj_MaxEnum       = 0xFFFFFFFFL
                                        
} PDColorConvertObjectAttributeFlags;
typedef ASUns32 PDColorConvertObjectAttributes;
    

/**  Color Space attributes: these are arranged as a bitmap. */
typedef enum 
{
	/** Device color space. */
    kColorConvDeviceSpace      =      _FLG(0),

	/** Calibrated color space. */
    kColorConvCalibratedSpace  =      _FLG(1),

	/** Alternate color space. */
    kColorConvAlternateSpace   =      _FLG(3),     /*  This is an alternate color space */

	/** Base of an indexed space. */
    kColorConvBaseSpace        =      _FLG(4),     /*  Base of an indexed space */

	/** Indexed color space. */
    kColorConvIndexedSpace     =      _FLG(5),

	/** Separation color space. */
    kColorConvSeparationSpace  =      _FLG(6),

	/** DeviceN color space. */
    kColorConvDeviceNSpace     =      _FLG(7),

	/** NChannel color space. */
    kColorConvNChannelSpace    =      _FLG(8),

	/** RGB color space. This should only be set if either Device space (unless Lab) or Calibrated space is set. */
    kColorConvRGBSpace         =      _FLG(9),

	/** CMYK color space. This should only be set if either Device space (unless Lab) or Calibrated space is set. */
    kColorConvCMYKSpace        =      _FLG(10),

	/** Gray color space. This should only be set if either Device space (unless Lab) or Calibrated space is set. */
    kColorConvGraySpace        =      _FLG(11),

	/** Lab color space. */
    kColorConvLabSpace         =      _FLG(12),

	/** Any color space. */
    kColorConvAnySpace         =      (_FLG(0)|_FLG(1)|_FLG(2)|_FLG(3)|_FLG(4)|_FLG(5)|_FLG(6)|_FLG(7)|_FLG(8)|_FLG(9)|_FLG(10)|_FLG(11)|_FLG(12)),

	/** Maximum enum value. */
    kColorConvSpace_MaxEnum       = 0xFFFFFFFFL
} PDColorConvertSpaceTypeFlags;
typedef ASUns32 PDColorConvertSpaceType;

/**  Action types: these specify what to do when an object is matched. */
typedef enum
{
	/** Do nothing but handle ink aliases. */
    kColorConvPreserve,        

	/** Convert to target space. */
    kColorConvConvert,         

	/** Convert calibrated space to device space. */
    kColorConvDecalibrate,     

	/** Convert NChannel space to DeviceN space. */
    kColorConvDownConvert,      

	/** Convert spot to Alternate Space*/
	kColorConvToAltSpace,
	/** Maximum enum value. */
    kColorConvMaxEnum       = 0xFFFFFFFFL
} PDColorConvertActionType;
    
/**  
Defines a color conversion action for a combination 
of attributes, color space/family, and rendering intent. If all three 
match, the actions in the action list will be executed for any given 
object. 
*/
typedef struct
{
    /** Object attribute. The flags must all match for a match. */
    PDColorConvertObjectAttributes mMatchAttributesAll;

	/** Object attribute. The flags will allow any match. */
    PDColorConvertObjectAttributes mMatchAttributesAny;
    
    /** Type of color space.  The flags must all match for a match. */
    PDColorConvertSpaceType        mMatchSpaceTypeAll;
    
    /** Type of color space.  The flags will allow any match. */
    PDColorConvertSpaceType        mMatchSpaceTypeAny;

    /** The rendering intent of the object. If this is <code>AC_UseProfileIntent</code>, this action applies to any intent. */
    AC_RenderIntent	          mMatchIntent;

    /** Defines the action, indicating what to do when an object matches this record. */
    PDColorConvertActionType     mAction;

    /** If converting, use this intent to override the object's intent for performing color conversion.
	    <code>AC_UseProfileIntent</code> means that the intent of the object must not be overridden. */
    AC_RenderIntent             mConvertIntent; 


    /**  The target profile for the conversion. If the output intent is embedded, this should be the output intent profile. */
    AC_Profile                  mConvertProfile;

    /**  If <code>true</code>, embed the target profile. If <code>false</code> the resulting color is Device, if possible. */
    ASBool                      mEmbed;

    /**  If <code>true</code>, perform a black-preserving transform when converting. */
    ASBool                      mPreserveBlack;

    /*  (todo: perhaps there should be a "use current settings" option in */
    /*  addition to true/false) */
    /**  If <code>true</code>, turn on black point compensation for color conversions. */
    ASBool                      mUseBlackPointCompensation;

    /** For ink actions (which describe a single colorant, whether used in
        Separation or DeviceN, and which are not matched using the above
        matching fields) this describes the colorant of the ink.
    */
    ASAtom mColorantName;
    
    /** Ink alias; this only applies to ink actions. */
    ASAtom mAlias;

    /** <code>true</code> if this ink is a process ink. */
    ASBool mIsProcessColor;
    
    /**  For internal use. */
    void *                      mInternal; 
    
} PDColorConvertActionRec, *PDColorConvertAction;
    

/**
The list of actions in <code>PDColorConvertParams</code> is analogous
to the list of filters in most email clients: each object is
compared against the selection criteria for each of the actions,
in order, until a matching action is found. The action is then
executed on the object. Note that actions do not chain, except
in the case of aliased ink definitions. 
*/
typedef struct //_t_PDColorConvertParamsRec
{
    /** The number of color conversion actions. */
    ASInt32              mNumActions;

    /** The actions, arranged sequentially in memory. */
    PDColorConvertAction    mActions;

    /** The number of specific inks supplied. */
    ASInt32              mNumInks;

    /**  The list of ink actions, arranged sequentially in memory. */
    PDColorConvertAction  mInks;

} PDColorConvertParamsRec, *PDColorConvertParams;

/* Added min/max text size and promote gray to CMYK flag. */
/**  
  Defines a color conversion action for a combination 
  of attributes, color space/family, and rendering intent. If all three 
  match, actions in the action list will be executed for any given 
  object.  
*/
typedef struct
{
    /** The size (in bytes) of this structure. */
    ASSize_t                    mSize;
    
    /** The object attributes. The flags must all match for a match. */
    PDColorConvertObjectAttributes mMatchAttributesAll;

	/** The object attributes. The flags will allow any match. */
    PDColorConvertObjectAttributes mMatchAttributesAny;
    
    /** The type of color space. The flags must all match for a match. */
    PDColorConvertSpaceType     mMatchSpaceTypeAll;
    
    /** The type of color space. The flags will allow any match. */
    PDColorConvertSpaceType     mMatchSpaceTypeAny;

    /** The rendering intent of the object. If this is <code>AC_UseProfileIntent</code>, this action applies to any intent. */
    AC_RenderIntent	            mMatchIntent;

    /** The font size to match for text objects. */
    double                      mMatchMinFontSize, mMatchMaxFontSize;
    
    /** Defines the action, specifying what to do when an object matches this record. */
    PDColorConvertActionType    mAction;

    /** If converting, use this intent to override the object's intent for performing color conversion.
	    <code>AC_UseProfileIntent</code> means that the intent of the object must not be overridden. */
    AC_RenderIntent             mConvertIntent; 

    /** The target profile for the conversion. If the output intent is embedded, this should be the output intent profile. */
    AC_Profile                  mConvertProfile;

    /** If <code>true</code>, embed the target profile. If <code>false</code>, the resulting color is Device, if possible. */
    ASBool                      mEmbed;

    /** If <code>true</code>, perform a black-preserving transform when converting. */
    ASBool                      mPreserveBlack;

    /** If <code>true</code>, promote DeviceGray objects to DeviceCMYK with <code>K = 1.0 - gray value</code>. */
    ASBool                      mPromoteGrayToCMYK;

    /*  (todo: perhaps there should be a "use current settings" option in */
    /*  addition to true/false) */
    /** If <code>true</code>, turn on black point compensation for color conversions. */
    ASBool                      mUseBlackPointCompensation;

    /** For ink actions (which describe a single colorant, whether used in
        Separation or DeviceN, and which are not matched using the above
        matching fields) this describes the colorant of the ink.
    */
    ASAtom mColorantName;
    
    /** Ink alias; this only applies to ink actions. */
    ASAtom mAlias;

    /** <code>true</code> if this ink is a process ink. */
    ASBool mIsProcessColor;
    
    /** If <code>true</code>, use a primary-preserving CMYK to CMYK transform. */
    ASBool                      mPreserveCMYKPrimaries;

    /** For internal use. */
    void *                      mInternal; 
    
} PDColorConvertActionRecEx, *PDColorConvertActionEx;
    

/**
The list of actions in <code>PDColorConvertParams</code> is analogous
to the list of filters in most email clients: each object is
compared against the selection criteria for each of the actions,
in order, until a matching action is found. The action is then
executed on the object. Note that actions do not chain, except
in the case of aliased ink definitions.
This is the <i>Extended</i> structure, which supports additional elements
in the <code>PDColorConvertActionRecEx</code> action.
*/
typedef struct //_t_PDColorConvertParamsRecEx
{
    /** The size of this data structure. */
    ASSize_t			 mSize;

    /** The number of color conversion actions. */
    ASInt32              mNumActions;

    /** The actions, arranged sequentially in memory. */
    PDColorConvertActionEx    mActions;

    /** The number of specific inks supplied. */
    ASInt32              mNumInks;

    /** The list of ink actions, arranged sequentially in memory. */
    PDColorConvertActionEx  mInks;

} PDColorConvertParamsRecEx, *PDColorConvertParamsEx;

// Callback completion code
/** Callback completion code. */
typedef enum
{
	/** Success. */
    PDCompletionSuccess,

	/** Continue. */
    PDCompletionContinue,

	/** Abort. */
    PDCompletionAbort
} PDCompletionCode;

/** Callback reason code. */
typedef enum
{
	/** None. */
    PDReasonNone,

	/** None implemented. */
    PDReasonNotImplemented
} PDReasonCode;
     
/* Callback type for report proc */
typedef ACCBPROTO1 void (ACCBPROTO2 *PDColorConvertReportProc)(
    PDColorConvertObjectAttributes objectType,
    PDColorConvertSpaceType colorSpaceType,
    PDColorConvertActionType action,
    PDCompletionCode completionCode,
    PDReasonCode reasonCode,
    void *userData);


/* Swatchbook API types */
/** Swatchbook database object. */
typedef void* ACSwatchBookDB;

/** Swatchbook object. */
typedef void* ACSwatchBook;

#ifdef __cplusplus
}
#endif

#endif /* _H_AcroColorExpT */


