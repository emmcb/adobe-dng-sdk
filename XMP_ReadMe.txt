The DNG SDK requires Adobe XMP SDK.

For your convenience, this distribution includes a source code version of the Adobe XMP SDK.

You may also obtain the Adobe XMP SDK from:

https://www.adobe.com/devnet/xmp.html

XMP also uses additional public source code for the expat and zlib libraries.

For expat, see this document: xmp/toolkit/XMPCore/third-party/expat/ReadMe.txt

For zlib, see this document: xmp/toolkit/third-party/zlib/ReadMe.txt

----------------------------------------------------------------------

To build XMPCore and XMPFiles manually using the included XMP SDK:

The following steps have been tested with Xcode 12 thru 14 on macOS and Visual Studio 2019 and 2022 on Windows.

macOS:

    1. Install the expat and zlib prerequisites.

    2. Open this project:

    xmp/toolkit/XMPCore/build/CMake64_libcpp_Static/XMPCore64.xcodeproj

    3. Select the XMPCoreStatic scheme.

    4. Choose either Debug or Release in the scheme options.

    5. Build.

    6. Open this project:

    xmp/toolkit/XMPFiles/build/CMake64_libcpp_Static/XMPFiles64.xcodeproj

    7. Select the XMPFilesStatic scheme.

    8. Choose either Debug or Release in the scheme options.

    9. Build.

Windows:

    1. Install the expat and zlib prerequisites.

    2. Open this solution:

    xmp/toolkit/XMPCore/build/CMake64Static_VC16/XMPCore64.sln

    3. Select the desired Debug/Release and ARM64/x64 configuration.

    4. Build.

    5. Open this solution:

    xmp/toolkit/XMPFiles/build/CMake64Static_VC16/XMPFiles64.sln

    6. Select the desired Debug/Release and ARM64/x64 configuration.

    7. Build.

