# Adobe DNG SDK

This repository is a mirror to the Adobe DNG SDK version 1.7.0.0, including up-to-date July 2023 release of the XMP SDK, with additional CMake script that allows for easy integration in external projects:

```cmake
include(FetchContent)

FetchContent_Declare(
    adobe-dng-sdk
    GIT_REPOSITORY "https://github.com/emmcb/adobe-dng-sdk.git"
    GIT_TAG "master"
)
FetchContent_MakeAvailable(adobe-dng-sdk)

...

target_link_libraries(your_target PRIVATE DNG::dng)
```
