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

## Building the SDK using the Docker image

The SDK can be built using the provided Docker image, which includes the necessary dependencies. Additionally, the command line tool `dng-validate` can be executed using the is built and can be used to validate DNG files.

```bash
# building the docker image
docker compose build

# building the sdk
docker compose run --rm build

# running dng-validate on a DNG file, path has to be passed relative to ./images directory
docker compose run --rm dng-validate [<parameters ...>] <dng file>
```
