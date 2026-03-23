#ifndef XMPANDROID_ANDROID_UTILS_HPP
#define XMPANDROID_ANDROID_UTILS_HPP

// =================================================================================================
// Copyright 2007 Adobe
// All Rights Reserved.
//
// NOTICE:  Adobe permits you to use, modify, and distribute this file in accordance with the terms
// of the Adobe license agreement accompanying it. If you have received this file from a source other 
// than Adobe, then your use, modification, or distribution of it requires the prior written permission
// of Adobe.
// =================================================================================================

#include "source/XMP_LibUtils.hpp"
#if XMP_AndroidBuild
#include "jni.h"

typedef XMP_Uns8 BinaryID [16];

// to cache instance of JVM for calling UUID java lib from c++
static JavaVM* cachedJVM;

void GuidGenerator(BinaryID &binID );

#endif //XMP_AndroidBuild

#endif //XMPANDROID_ANDROID_UTILS_HPP

