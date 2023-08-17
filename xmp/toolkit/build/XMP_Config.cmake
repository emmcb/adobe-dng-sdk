#/*************************************************************************
#*
#* ADOBE CONFIDENTIAL
#* ___________________
#*
#* Copyright [2012] Adobe Systems Incorporated
#* All Rights Reserved.
#*
#* NOTICE:  All information contained herein is, and remains
#* the property of Adobe Systems Incorporated and its suppliers,
#* if any.  The intellectual and technical concepts contained
#* herein are proprietary to Adobe Systems Incorporated and its
#* suppliers and are protected by trade secret or copyright law.
#* Dissemination of this information or reproduction of this material
#* is strictly forbidden unless prior written permission is obtained
#* from Adobe Systems Incorporated.
#**************************************************************************/

# ==============================================================================
# define minimum cmake version
# For Android always build with make 3.6
if(ANDROID)
	cmake_minimum_required(VERSION 3.5.2)
else(ANDROID)
	cmake_minimum_required(VERSION 3.15.5)
endif(ANDROID)

# ==============================================================================
# XMP config for XMPTOOLKIT and TestRunner
# ==============================================================================
if(NOT DEFINED XMP_ROOT)
	set(XMP_ROOT ${CMAKE_CURRENT_SOURCE_DIR}/${XMP_THIS_PROJECT_RELATIVEPATH}/toolkit)
endif()

if(NOT DEFINED COMMON_BUILD_SHARED_DIR)
	set(COMMON_BUILD_SHARED_DIR ${XMP_ROOT}/../build/shared)
endif()

add_definitions(-DAdobePrivate=1)

set(INCLUDE_CPP_DOM_SOURCE FALSE)

if (INCLUDE_CPP_DOM_SOURCE)
	add_definitions(-DENABLE_CPP_DOM_MODEL=1)
else (INCLUDE_CPP_DOM_SOURCE)
	add_definitions(-DENABLE_CPP_DOM_MODEL=0)
endif(INCLUDE_CPP_DOM_SOURCE)

include(${XMP_ROOT}/build/XMP_ConfigCommon.cmake)
