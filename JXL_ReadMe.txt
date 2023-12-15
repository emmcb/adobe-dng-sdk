DNG supports JPEG XL compression starting with version 1.7. Building
the DNG SDK with JPEG XL support (see qDNGSupportJXL in dng_flags.h)
requires libjxl.

For your convenience, a copy of libjxl is included in this package.
You may also obtain libjxl directly from:

  https://github.com/libjxl/libjxl

Be aware that if you update libjxl, you may also need to update the 
project files included in this package, located here:

  libjxl/client_projects
