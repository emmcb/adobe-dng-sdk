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

# ==============================================================================
# This project
# ==============================================================================
set(PRODUCT_NAME PDF_Handler)
set(TARGET_NAME ${PRODUCT_NAME})

if(CMAKE_CL_64)
	project(${TARGET_NAME}64)
	set(BITS 64)
else()
	project(${TARGET_NAME})
	set(BITS 32)
endif()


# ==============================================================================
# Shared config
# ==============================================================================
# setup some values before calling shared config
set(XMP_THIS_PROJECT_RELATIVEPATH "../../../..")

include(${CMAKE_CURRENT_SOURCE_DIR}/${XMP_THIS_PROJECT_RELATIVEPATH}/toolkit/build/XMP_Config.cmake)

if(WIN32 OR APPLE)
	link_directories("${XMPROOT_DIR}/public/libraries/${XMP_PLATFORM_FOLDER}")
endif()

# ==============================================================================
# Define what to do, lib, exe, etc
set(DEPENDENCY_LIST "STATIC:XMPCoreStatic" )


include(${CMAKE_CURRENT_SOURCE_DIR}/${XMP_THIS_PROJECT_RELATIVEPATH}/toolkit/XMPFilesPlugins/PDF_Handler/build/CMakeListsCommon.txt)

# ==============================================================================
# Link dependencies

if(UNIX AND NOT APPLE)
	target_link_libraries(
		${TARGET_NAME}
		XMPCore
		XMPFiles
		#${LIB_MINIPDFL}
	)
else()
	target_link_libraries(
		${TARGET_NAME}
		XMPCore
		XMPFiles
		#${LIB_MINIPDFL}
		${XMP_PLATFORM_LINK}
	)
endif()

SetWinLinkFlags(${TARGET_NAME} "" "${TARGET_NAME}")

