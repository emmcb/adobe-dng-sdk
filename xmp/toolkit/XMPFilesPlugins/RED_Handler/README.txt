Redcode Raw (R3D) is a proprietary multimedia audio/video file format owned by Red Digital Cinema Camera Company.The company provides R3D SDK for partners comapnies to integrate loading , editing and decoding R3D clips into their own software products on Windows, Mac and Linux.RED Plugin Handler uses methods Clip::GetRmdXmp and Clip::CreateOrUpdateRmd to get and update the XMP packet to the R3D Metadata File(RMD).The following is the directory structure of the R3D SDK
REDSDK
¦   R3D SDK readme.txt
¦   Release history.txt
¦   Rocket driver and firmware.txt
¦   GPU extension SDK.txt
¦
+---Include
¦       R3DSDK.h
¦       R3DSDKDefinitions.h
¦       R3DSDKMetadata.h
¦       R3DSDKRocket.h
¦
+---Lib
¦   +---linux32
¦   ¦       libR3DSDK.a
¦   ¦       libR3DSDKPIC.a
¦   ¦
¦   +---linux64
¦   ¦       libR3DSDK.a
¦   ¦       libR3DSDKPIC.a
¦   ¦
¦   +---mac32_64
¦   ¦       libR3DSDK.a
¦   ¦
¦   +---win32
¦   ¦       R3DSDK-2008MD.lib
¦   ¦       R3DSDK-2008MDd.lib
¦   ¦       R3DSDK-2008MT.lib
¦   ¦       R3DSDK-2008MTd.lib
¦   ¦       R3DSDK-2010MD.lib
¦   ¦       R3DSDK-2010MDd.lib
¦   ¦       R3DSDK-2010MT.lib
¦   ¦       R3DSDK-2010MTd.lib
¦   ¦       R3DSDK-2012MD.lib
¦   ¦       R3DSDK-2012MDd.lib
¦   ¦       R3DSDK-2012MT.lib
¦   ¦       R3DSDK-2012MTd.lib
¦   ¦       R3DSDK-2013MD.lib
¦   ¦       R3DSDK-2013MDd.lib
¦   ¦       R3DSDK-2013MT.lib
¦   ¦       R3DSDK-2013MTd.lib
¦   ¦
¦   +---win64
¦           R3DSDK-2008MD.lib
¦           R3DSDK-2008MDd.lib
¦           R3DSDK-2008MT.lib
¦           R3DSDK-2008MTd.lib
¦           R3DSDK-2010MD.lib
¦           R3DSDK-2010MDd.lib
¦           R3DSDK-2010MT.lib
¦           R3DSDK-2010MTd.lib
¦           R3DSDK-2012MD.lib
¦           R3DSDK-2012MDd.lib
¦           R3DSDK-2012MT.lib
¦           R3DSDK-2012MTd.lib
¦           R3DSDK-2013MD.lib
¦           R3DSDK-2013MDd.lib
¦           R3DSDK-2013MT.lib
¦           R3DSDK-2013MTd.lib
¦
+---Sample code
¦       14 Code Samples using R3D SDK APIs
¦
+---Support
        Some RED Rocket zip packages

Directory structure of R3D SDK inside RED Handler
RED_HANDLER\THIRD-PARTY
+---REDSDK
    ¦   R3D SDK readme.txt
    ¦   Release history.txt
    ¦   Rocket driver and firmware.txt
	¦   GPU extension SDK.txt
    ¦
    +---Include
    ¦       R3DSDK.h
    ¦       R3DSDKDefinitions.h
    ¦       R3DSDKMetadata.h
    ¦       R3DSDKRocket.h
    ¦
    +---Lib
        +---linux32
        ¦       libR3DSDKPIC.a
        ¦
        +---linux64
        ¦       libR3DSDKPIC.a
        ¦
        +---mac32_64
        ¦       libR3DSDK.a
        ¦
        +---win32
        ¦   +---debug
        ¦   ¦       R3DSDK-2010MD.lib
		¦   ¦       R3DSDK-2012MD.lib
		¦   ¦       R3DSDK-2013MD.lib
        ¦   ¦
        ¦   +---release
        ¦           R3DSDK-2010MD.lib
		¦   ¦       R3DSDK-2012MD.lib
		¦   ¦       R3DSDK-2013MD.lib
        ¦
        +---win64
            +---debug
            ¦       R3DSDK-2010MD.lib
			¦       R3DSDK-2012MD.lib
			¦       R3DSDK-2013MD.lib
            ¦
            +---release
            ¦       R3DSDK-2010MD.lib
			¦       R3DSDK-2012MD.lib
			¦       R3DSDK-2013MD.lib


The Include folder path should be added to the Include file Directory for compiling RED Handler.

Linking to the R3D SDK
-----------------------------------------------------------
Windows: to not get link errors and/or runtime crashes you
need to link to the same runtime library in your Visual
Studio project as the SDK library you are using (either
multi-threaded [/MT] or multi-threaded DLL [/MD] for release
or multi-threaded debug [/MTd] or multi-threaded debug DLL
[/MDd] for your debug projects).

Mac: also link in CoreServices & IOKit framework. For Xcode 5
the C++ Standard Library setting must be set to "libstdc++
(GNU C++ standard library)" instead of the default "libc++
(LLVM C++ standard library...)". This setting can be found
under "Apple LLVM 5.0 - Language - C++" section in the
target's Build Settings tab.

Linux: also link in pthreads and libuuid.
