#/*************************************************************************
#*
#* ADOBE CONFIDENTIAL
#* ___________________
#*
#* Copyright [2014] Adobe Systems Incorporated
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
set(PRODUCT_NAME MP4OEM_Handler)
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

# ==============================================================================
# Add or overwite shared config variables
# ==============================================================================
# Windows specific
if(WIN32)
	set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -D_WINDOWS")
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -D_WINDOWS")
	SET(CMAKE_EXE_LINKER_FLAGS /NODEFAULTLIB:LIBCMT)
endif()

# all platform
set(CXX_FLAGS_DEBUG "-D_DEBUG=1 -DDEBUG=1")
set(CXX_FLAGS_RELEASE "-DNDEBUG=1")

# ==============================================================================
# Locations
# ==============================================================================
SetOptionalPluginOutputPath()

# ==============================================================================
# For convenience we define the sources as a variable. You can add 
# header files and cpp/c files and CMake will sort them out
# ==============================================================================

AddPluginApiFiles("../../..")

list (APPEND SOURCE_UTIL_FILES ${XMPROOT_DIR}/source/UnicodeConversions.cpp)
list (APPEND SOURCE_UTIL_FILES ${XMPROOT_DIR}/source/XIO.cpp)
list (APPEND SOURCE_UTIL_FILES ${XMPROOT_DIR}/source/XMPFiles_IO.cpp)
list (APPEND SOURCE_UTIL_FILES ${XMPROOT_DIR}/source/XMP_LibUtils.cpp)
list (APPEND SOURCE_UTIL_FILES ${XMPROOT_DIR}/source/XMP_ProgressTracker.cpp)
list (APPEND SOURCE_UTIL_FILES ${XMPROOT_DIR}/source/PerfUtils.cpp)
list (APPEND SOURCE_UTIL_FILES ${XMPROOT_DIR}/source/XML_Node.cpp)

list (APPEND SOURCE_FORMAT_SUPPORT_FILES ${XMPROOT_DIR}/XMPFiles/source/FormatSupport/ISOBaseMedia_Support.cpp)
list (APPEND SOURCE_FORMAT_SUPPORT_FILES ${XMPROOT_DIR}/XMPFiles/source/FormatSupport/MOOV_Support.cpp)
list (APPEND SOURCE_FORMAT_SUPPORT_FILES ${XMPROOT_DIR}/XMPFiles/source/FormatSupport/META_Support.cpp)
list (APPEND SOURCE_FORMAT_SUPPORT_FILES ${XMPROOT_DIR}/XMPFilesPlugins/FormatSupport/OEM_Support.cpp)
list (APPEND SOURCE_FORMAT_SUPPORT_FILES ${XMPROOT_DIR}/XMPFiles/source/FormatSupport/TIFF_MemoryReader.cpp)
list (APPEND SOURCE_FORMAT_SUPPORT_FILES ${XMPROOT_DIR}/XMPFiles/source/FormatSupport/TIFF_Support.cpp)
list (APPEND SOURCE_FORMAT_SUPPORT_FILES ${XMPROOT_DIR}/XMPFiles/source/FormatSupport/Reconcile_Impl.cpp)
# Format support are dependent upon XMPFiles_Impl.cpp
list (APPEND SOURCE_FORMAT_SUPPORT_FILES ${XMPROOT_DIR}/XMPFiles/source/XMPFiles_Impl.cpp)

list (APPEND SOURCE_FILES ${SOURCE_ROOT}/MP4OEM_Handler.cpp)

if(UNIX)
	list(APPEND SOURCE_UTIL_FILES ${XMPROOT_DIR}/source/Host_IO-POSIX.cpp)
else()
	list(APPEND SOURCE_UTIL_FILES ${XMPROOT_DIR}/source/Host_IO-Win.cpp)
endif()

source_group("Source Files\\Utilities" FILES ${SOURCE_UTIL_FILES})
source_group("Source Files\\FormatSupport" FILES ${SOURCE_FORMAT_SUPPORT_FILES})
source_group("Source Files" FILES ${SOURCE_FILES})

# resources
list(APPEND RESOURCE_FILES ${PROJECT_ROOT}/resource/txt/XMPPLUGINUIDS.txt)
AddPluginResourceFiles()

file (GLOB PUBLIC_XMPCOMMON_CLIENTGLUE_FILES ${XMPROOT_DIR}/public/include/XMPCommon/source/*.*)
source_group("Source Files\\Public\\XMPCommon" FILES ${PUBLIC_XMPCOMMON_CLIENTGLUE_FILES})

file (GLOB PUBLIC_XMPCORE_CLIENTGLUE_FILES ${XMPROOT_DIR}/public/include/XMPCore/source/*.*)
source_group("Source Files\\Public\\XMPCore" FILES ${PUBLIC_XMPCORE_CLIENTGLUE_FILES})

# merge list elements
list(APPEND SOURCE_FILES 
	${API_INCLUDE_FILES} 
	${API_SOURCE_FILES} 
	${RESOURCE_FILES} 
	${SOURCE_UTIL_FILES} 
	${SOURCE_FILES} 
	${SOURCE_FORMAT_SUPPORT_FILES}
	${PUBLIC_XMPCOMMON_CLIENTGLUE_FILES} 
	${PUBLIC_XMPCORE_CLIENTGLUE_FILES}
	)

# include directories
include_directories(
	${XMPROOT_DIR}
	${XMPROOT_DIR}/public/include
	${PROJECT_ROOT}/../../api/source
)

#additional link directory
#unix library folder structure differes with Mac and windows.
if(WIN32)
	link_directories("${XMPROOT_DIR}/public/libraries/${XMP_PLATFORM_FOLDER}")
elseif(UNIX AND NOT APPLE)
	string(TOLOWER ${CMAKE_BUILD_TYPE} TEMP_DEBUG_STRING)
	link_directories("${XMPROOT_DIR}/public/libraries/${XMP_PLATFORM_FOLDER}/${TEMP_DEBUG_STRING}")
endif()

# ==============================================================================
# Define what to do, lib, exe, etc
set(DEPENDENCY_LIST "DLL:XMPCore" "DLL:XMPFiles")
AddLibraryAndDependencies(${TARGET_NAME} ${XMP_BUILD_STATIC} YES "SHARED" SOURCE_FILES DEPENDENCY_LIST)
# ==============================================================================
# Link dependencies

if(UNIX AND NOT APPLE)
	target_link_libraries(
		${TARGET_NAME}
		XMPCore
		XMPFiles
	)
else()
	target_link_libraries(
		${TARGET_NAME}
		XMPCore
		XMPFiles
		${XMP_PLATFORM_LINK}
	)
endif()

set(FRAMEWORK_LIST "Mac:CoreFoundation" "Mac:Foundation" "Mac:CoreServices" "Mac:${XMP_PLATFORM_LINK}")
AddMacFramework(${TARGET_NAME} FRAMEWORK_LIST)

SetWinLinkFlags(${TARGET_NAME} "" "${TARGET_NAME}")

if(APPLE)
	set_target_properties(${TARGET_NAME} PROPERTIES BUILD_WITH_INSTALL_RPATH ON BUILD_WITH_INSTALL_NAME_DIR ON INSTALL_NAME_DIR "@executable_path/../Frameworks")
endif()

set(DEPEND_LIBS "")
CreatePlugin(${TARGET_NAME} ${XMPPLUGIN_OUTPUT_DIR} "${PROJECT_ROOT}/resource/txt/*.txt" XMP_PLUGIN_DEPENDENCIES DEPEND_LIBS)

if (UNIX)
	if(APPLE)
		if(NOT XMP_BUILD_STATIC)
			add_custom_command (TARGET ${TARGET_NAME} POST_BUILD 
				COMMAND cp -f ${PROJECT_ROOT}/resource/txt/MODULE_IDENTIFIER.txt ${XMPPLUGIN_OUTPUT_DIR}/${TARGET_NAME}.xpi/Versions/A/Resources/MODULE_IDENTIFIER.txt
				COMMAND cp -f ${PROJECT_ROOT}/resource/txt/XMPPLUGINUIDS-${BITS}.txt ${XMPPLUGIN_OUTPUT_DIR}/${TARGET_NAME}.xpi/Versions/A/Resources/XMPPLUGINUIDS.txt
				COMMENT "Copy resource")
		endif()
	else()
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
		add_custom_command (TARGET ${TARGET_NAME} POST_BUILD 
			COMMAND mkdir -p ${XMPPLUGIN_OUTPUT_DIR}/${TARGET_NAME}.resources
			COMMAND cp -f ${PROJECT_ROOT}/resource/txt/MODULE_IDENTIFIER.txt ${XMPPLUGIN_OUTPUT_DIR}/${TARGET_NAME}.resources/MODULE_IDENTIFIER.txt
			COMMAND cp -f ${PROJECT_ROOT}/resource/txt/XMPPLUGINUIDS-${BITS}.txt ${XMPPLUGIN_OUTPUT_DIR}/${TARGET_NAME}.resources/XMPPLUGINUIDS.txt
			COMMENT "Copy resource")
	endif()
endif()

# ==============================================================================
# configuration feedback
# ==============================================================================

message (STATUS "===========================================================================")
message (STATUS " ${TARGET_NAME}		")
message (STATUS "===========================================================================")

