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


# Enable folder grouping of projects in IDEs
set_property(GLOBAL PROPERTY USE_FOLDERS ON)
set(CMAKE_C_IMPLICIT_LINK_DIRECTORIES "")
set(CMAKE_C_IMPLICIT_LINK_FRAMEWORK_DIRECTORIES "")
# ==============================================================================
# This project
option(CMAKE_CL_64 "64bit target?" OFF)
set(TARGET_NAME XMPToolkit)
if(CMAKE_CL_64)
	project(${TARGET_NAME}64)
else(CMAKE_CL_64)
	project(${TARGET_NAME})
endif(CMAKE_CL_64)

#if(NOT DEFINED XMP_TOPLEVELPROJECT)
#	set(XMP_TOPLEVELPROJECT "XMPBuild" CACHE INTERNAL "Identify top level project responsible for build XMP libs")
#endif()

# ==============================================================================
# Globals
# ==============================================================================

set(XMP_ROOT ${PROJECT_SOURCE_DIR}/..)

# For Android always build dynamic library
if(ANDROID)
	set(XMP_BUILD_STATIC OFF)
endif()


# ==============================================================================
# SET XMP and Plugin Version Numbers defined in an external file
# ==============================================================================

# Get Version numbers from an external file XMP_BuildNumber.txt
#file(STRINGS "XMP_BuildNumber.txt" XMP_BUILD_STRING) 

# Parse List for Minor Major and Branchnumber
#string(REGEX MATCHALL "[0-9]" XMP_BUILD_VERSION_LIST "${XMP_BUILD_STRING}")
#list(GET XMP_BUILD_VERSION_LIST 0 XMP_VERSION_MAJOR)
#list(GET XMP_BUILD_VERSION_LIST 1 XMP_VERSION_MINOR)
#list(GET XMP_BUILD_VERSION_LIST 2 XMP_BRANCH_NUMBER)
	
#set(XMP_VERSION ${XMP_VERSION_MAJOR}.${XMP_VERSION_MINOR}.${XMP_BRANCH_NUMBER})

# Write the Version infos to a header file
#configure_file (
#  "${XMP_ROOT}/../toolkit/build/XMP_VersionNumber.h.in"
#  "${XMP_ROOT}/../toolkit/public/include/XMP_VersionNumber.h"
#  )
  
# extract BUILDID from XMP_BuildInfo.h to be used by external CMake files, e.g. FaceDetector DLL
#file(STRINGS "XMP_BuildInfo.h" XMP_BUILDID_LINE REGEX "#define BUILDID ")
#string(REGEX MATCHALL "[0-9]+" XMP_BUILDID ${XMP_BUILDID_LINE})

# ==============================================================================
# Shared config
# ==============================================================================
# setup some values before calling shared config
#set(XMP_THIS_PROJECT_RELATIVEPATH "../..")
include(${XMP_ROOT}/build/XMP_Config.cmake)

# ==============================================================================
# Sub projects
# ==============================================================================
#add_subdirectory(${XMP_ROOT}/XMPCommon/build ${XMP_ROOT}/XMPCommon/build/${XMP_CMAKEFOLDER_NAME})
add_subdirectory(${XMP_ROOT}/XMPCore/build ${XMP_ROOT}/XMPCore/build/${XMP_CMAKEFOLDER_NAME})
	add_subdirectory(${XMP_ROOT}/XMPFiles/build ${XMP_ROOT}/XMPFiles/build/${XMP_CMAKEFOLDER_NAME})

if(NOT CMAKE_WINUNIVERSAL AND (WIN32 OR APPLE OR ANDROID ) )
		add_subdirectory(${XMP_ROOT}/XMPCompareAndMerge/build ${XMP_ROOT}/XMPCompareAndMerge/build/${XMP_CMAKEFOLDER_NAME})
	if( NOT APPLE_IOS AND NOT ANDROID AND CMAKE_CL_64)
			add_subdirectory(${XMP_ROOT}/XMPScript/build ${XMP_ROOT}/XMPScript/build/${XMP_CMAKEFOLDER_NAME})
	endif()
endif()

if (NOT APPLE_IOS AND NOT CMAKE_WINUNIVERSAL AND NOT ANDROID AND NOT XMP_BUILD_STATIC )
    if(NOT WIN32 OR NOT  ${CMAKE_ARCH} MATCHES "ARM64" AND NOT APPLE_UNIVERSAL)
	add_subdirectory(${XMP_ROOT}/XMPFilesPlugins/PDF_Handler/build ${XMP_ROOT}/XMPFilesPlugins/PDF_Handler/build/${XMP_CMAKEFOLDER_NAME})
	endif()
	if(WIN32 OR APPLE OR ANDROID OR APPLE_IOS OR CMAKE_WINUNIVERSAL)
		if(NOT WIN32 OR NOT  ${CMAKE_ARCH} MATCHES "ARM64")
			if(NOT WIN32 OR NOT ${CMAKE_ARCH} MATCHES "x86")
				add_subdirectory(${XMP_ROOT}/XMPFilesPlugins/RED_Handler/build ${XMP_ROOT}/XMPFilesPlugins/RED_Handler/build/${XMP_CMAKEFOLDER_NAME})
			endif()	
		endif()
	endif()
	add_subdirectory(${XMP_ROOT}/XMPFilesPlugins/MXF_Handler/build ${XMP_ROOT}/XMPFilesPlugins/MXF_Handler/build/${XMP_CMAKEFOLDER_NAME})
	add_subdirectory(${XMP_ROOT}/XMPFilesPlugins/MP4OEM_Handler/build ${XMP_ROOT}/XMPFilesPlugins/MP4OEM_Handler/build/${XMP_CMAKEFOLDER_NAME})
	add_subdirectory(${XMP_ROOT}/XMPFilesPlugins/MOVOEM_Handler/build ${XMP_ROOT}/XMPFilesPlugins/MOVOEM_Handler/build/${XMP_CMAKEFOLDER_NAME})
	if((APPLE OR WIN32) AND CMAKE_CL_64 AND NOT APPLE_UNIVERSAL)
		if(NOT WIN32 OR NOT  ${CMAKE_ARCH} MATCHES "ARM64")
			add_subdirectory(${XMP_ROOT}/XMPFilesPlugins/ARRI_Handler/build ${XMP_ROOT}/XMPFilesPlugins/ARRI_Handler/build/${XMP_CMAKEFOLDER_NAME})
		endif()
	endif()
endif()


# ==============================================================================
# Custom Targets
# ==============================================================================
if (APPLE AND NOT APPLE_IOS)
	add_custom_target(Documentation
		COMMAND rm -Rf ../../public/docs/XMPToolkit/
		COMMAND mkdir -p ../../public/docs/XMPToolkit/
		COMMAND ../../sdk/tools/doxygen/doxygen ../Doxyfile
		COMMAND cp ../../documents/toolkit/static/api.gif ../../public/docs/XMPToolkit/
		COMMAND cp ../../documents/toolkit/static/asn.gif ../../public/docs/XMPToolkit/
		COMMAND cp ../../documents/toolkit/static/1pTrans.gif ../../public/docs/XMPToolkit/
		COMMENT "Creating Doxygen Documentation"
	)
endif()


# ==============================================================================
# configuration feedback
# ==============================================================================

message (STATUS "===========================================================================")
message (STATUS " ${TARGET_NAME} ")
#message (STATUS " Version = ${XMP_VERSION}")
message (STATUS "===========================================================================")
#message (STATUS " ${XMP_EXTRA_BUILDMACHINE}")
message (STATUS " 64 bit build is: ${CMAKE_CL_64}")
message (STATUS " Static build is: ${XMP_BUILD_STATIC}")
message (STATUS " ${TARGET_NAME} ")
message (STATUS " ${XMP_PLATFORM_LINK} ")
message (STATUS " ${XMP_GLOBALS}")
message (STATUS " Compiler: ")
message (STATUS " CC=${CMAKE_C_COMPILER}")
message (STATUS " CXX=${CMAKE_CXX_COMPILER}")
message (STATUS "===========================================================================")
