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
set(PRODUCT_NAME PluginTemplate)
set(TARGET_NAME ${PRODUCT_NAME})

if(CMAKE_CL_64)
	project(${TARGET_NAME}64)
else()
	project(${TARGET_NAME})
endif()

# ==============================================================================
# Shared config
# ==============================================================================
# setup some values before calling shared config
set(XMP_THIS_PROJECT_RELATIVEPATH "../../../..")

include(${CMAKE_CURRENT_SOURCE_DIR}/${XMP_THIS_PROJECT_RELATIVEPATH}/toolkit/build/XMP_Config.cmake)

# ==============================================================================
# Add or overwite shared config variables
# ==============================================================================
# Windows specific
if(WIN32)
	set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS}")
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}")
	set (LINK_FLAGS "${LINK_FLAGS} /SUBSYSTEM:WINDOWS /DYNAMICBASE /NXCOMPAT")
endif()

# all platform
set(CXX_FLAGS_DEBUG "-D_DEBUG=1 -DDEBUG=1")
set(CXX_FLAGS_RELEASE "-DNDEBUG=1")

# ==============================================================================
# Locations
# ==============================================================================
SetPluginOutputPath()

# ==============================================================================
# For convenience we define the sources as a variable. You can add 
# header files and cpp/c files and CMake will sort them out
# ==============================================================================

AddPluginApiFiles("../../..")

list (APPEND COMMON_SOURCE_FILES ${XMPROOT_DIR}/source/UnicodeConversions.cpp)
list (APPEND SOURCE_FILES ${XMPROOT_DIR}/source/PerfUtils.cpp)
list (APPEND SOURCE_FILES ${XMPROOT_DIR}/source/XMP_ProgressTracker.cpp)
if(UNIX)
	list(APPEND COMMON_SOURCE_FILES ${XMPROOT_DIR}/source/Host_IO-POSIX.cpp)
else()
	list(APPEND COMMON_SOURCE_FILES ${XMPROOT_DIR}/source/Host_IO-Win.cpp)
endif()
source_group("Source Files\\common" FILES ${COMMON_SOURCE_FILES})

list(APPEND SOURCE_FILES
	${SOURCE_ROOT}/Template_Handler.cpp
	)
source_group("Source Files" FILES ${SOURCE_FILES})

# resources
AddPluginResourceFiles()

# merge list elements
list(APPEND SOURCE_FILES ${API_INCLUDE_FILES} ${API_SOURCE_FILES} ${COMMON_SOURCE_FILES} ${SOURCE_FILES} ${RESOURCE_FILES})

# include directories
include_directories(
	${XMPROOT_DIR}
	${XMPROOT_DIR}/public/include
)

#link directories
set(LIB_ADOBEXMP AdobeXMP)
link_directories(${XMPROOT_DIR}/public/libraries/${XMP_PLATFORM_FOLDER})

# ==============================================================================
# Define what to do, lib, exe, etc
set(DEPENDENCY_LIST "")
AddLibraryAndDependencies(${TARGET_NAME} ${XMP_BUILD_STATIC} YES "SHARED" SOURCE_FILES DEPENDENCY_LIST)
# ==============================================================================
# Link dependencies

if(WIN32)
	target_link_libraries(
		${TARGET_NAME}
		${LIB_ADOBEXMP}
		${XMP_PLATFORM_LINK}
		)
else(WIN32)
	if(UNIX AND NOT APPLE)
		target_link_libraries(
			${TARGET_NAME}
			${LIB_ADOBEXMP}
		)
	endif()
endif()

set(FRAMEWORK_LIST "Mac:CoreFoundation" "Mac:Foundation" "Mac:CoreServices" "Mac:${LIB_ADOBEXMP}" "Mac:${XMP_PLATFORM_LINK}")
AddMacFramework(${TARGET_NAME} FRAMEWORK_LIST)

SetWinLinkFlags(${TARGET_NAME} "" "${TARGET_NAME}")

set(DEPEND_LIBS "")
CreatePlugin(${TARGET_NAME} ${XMPPLUGIN_OUTPUT_DIR} "${PROJECT_ROOT}/resource/txt/*.txt" XMP_PLUGIN_DEPENDENCIES DEPEND_LIBS)

# ==============================================================================
# Post build
# ==============================================================================

if (UNIX)
	if(NOT APPLE)
		if((${CMAKE_BUILD_TYPE} MATCHES "Debug") OR (${CMAKE_BUILD_TYPE} MATCHES "debug") )
			add_custom_command (TARGET ${TARGET_NAME}
				POST_BUILD
				COMMAND ls -l ${XMPPLUGIN_OUTPUT_DIR}/${TARGET_NAME}.xpi
			)
		else()
			add_custom_command (TARGET ${TARGET_NAME}
				POST_BUILD
				COMMAND strip ${XMPPLUGIN_OUTPUT_DIR}/${TARGET_NAME}.xpi
				COMMAND ls -l ${XMPPLUGIN_OUTPUT_DIR}/${TARGET_NAME}.xpi
			)
		endif()
	endif()
endif()
# ==============================================================================
# configuration feedback
# ==============================================================================

message (STATUS "===========================================================================")
message (STATUS " ${TARGET_NAME}		")
message (STATUS "===========================================================================")

