Welcome to the Digital Negative Software Development Kit (DNG SDK)

This package comes with C++ source code for the DNG SDK (see
dng_sdk/source). This code has been tested using C++14 and C++17
language versions with various compilers on x64, arm32, and arm64
architectures.

This package also includes some project files for building a
command-line utility named "dng_validate" on macOS and Windows (see
dng_sdk/projects). These project files have been tested using the
following compilers:

macOS: Xcode 12.4 thru Xcode 13.1

Windows: Visual Studio 2019

Instructions to build dng_validate:

1. Follow the instructions in JPEG_ReadMe.txt to download and install
   libjpeg.

2. Follow the instructions in XMP_ReadMe.txt to download and install
   external dependencies, then build XMPCore and XMPFiles.

3. macOS: Open dng_sdk/projects/mac/dng_validate.xcodeproj and build.

   Windows: Open dng_sdk/projects/win/dng_validate.sln and build.
