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
set(PRODUCT_NAME RED_Handler)
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
SetPluginOutputPath()

# ==============================================================================
# For convenience we define the sources as a variable. You can add 
# header files and cpp/c files and CMake will sort them out
# ==============================================================================

AddPluginApiFiles("../../..")

list (APPEND SOURCE_UTIL_FILES ${XMPROOT_DIR}/source/UnicodeConversions.cpp)
list (APPEND SOURCE_UTIL_FILES ${XMPROOT_DIR}/source/XMP_LibUtils.cpp)
list (APPEND SOURCE_UTIL_FILES ${XMPROOT_DIR}/source/IOUtils.cpp)
list (APPEND SOURCE_UTIL_FILES ${XMPROOT_DIR}/source/XIO.cpp)
if(UNIX)
	list(APPEND SOURCE_UTIL_FILES ${XMPROOT_DIR}/source/Host_IO-POSIX.cpp)
else()
	list(APPEND SOURCE_UTIL_FILES ${XMPROOT_DIR}/source/Host_IO-Win.cpp)
endif()
source_group("Source Files\\Utilities" FILES ${SOURCE_UTIL_FILES})

list (APPEND SOURCE_FILES
		${SOURCE_ROOT}/RED_Handler.cpp
	)
source_group("Source Files" FILES ${SOURCE_FILES})

# resources
list(APPEND RESOURCE_FILES ${PROJECT_ROOT}/resource/txt/XMPPLUGINUIDS.txt)
AddPluginResourceFiles()

file (GLOB PUBLIC_XMPCOMMON_CLIENTGLUE_FILES ${XMPROOT_DIR}/public/include/XMPCommon/source/*.*)
source_group("Source Files\\Public\\XMPCommon" FILES ${PUBLIC_XMPCOMMON_CLIENTGLUE_FILES})

file (GLOB PUBLIC_XMPCORE_CLIENTGLUE_FILES ${XMPROOT_DIR}/public/include/XMPCore/source/*.*)
source_group("Source Files\\Public\\XMPCore" FILES ${PUBLIC_XMPCORE_CLIENTGLUE_FILES})

# merge list elements
list(APPEND SOURCE_FILES ${API_INCLUDE_FILES} ${API_SOURCE_FILES} ${RESOURCE_FILES} ${SOURCE_UTIL_FILES} ${SOURCE_FILES}
	${PUBLIC_XMPCOMMON_CLIENTGLUE_FILES} 
	${PUBLIC_XMPCORE_CLIENTGLUE_FILES})

# include directories
include_directories(
	${XMPROOT_DIR}
	${XMPROOT_DIR}/public/include
	${XMPROOT_DIR}/public
	${PROJECT_ROOT}/../third-party/REDSDK/Include 
)

#additional link directory
#unix library folder structure differes with Mac and windows.

if(WIN32)
	link_directories("${XMPROOT_DIR}/public/libraries/${XMP_PLATFORM_FOLDER}")
	if(${CMAKE_GENERATOR} STREQUAL "Visual Studio 10" OR ${CMAKE_GENERATOR} STREQUAL "Visual Studio 10 Win64" )
		set(LIB_REDSDK R3DSDK-2010MD)
	elseif(${CMAKE_GENERATOR} STREQUAL "Visual Studio 11" OR ${CMAKE_GENERATOR} STREQUAL "Visual Studio 11 Win64" )
		set(LIB_REDSDK R3DSDK-2012MD)
	elseif(${CMAKE_GENERATOR} STREQUAL "Visual Studio 12 2013" OR ${CMAKE_GENERATOR} STREQUAL "Visual Studio 12 2013 Win64" OR ${CMAKE_GENERATOR} STREQUAL "Visual Studio 12 Win64" OR ${CMAKE_GENERATOR} STREQUAL "Visual Studio 12" )
		set(LIB_REDSDK R3DSDK-2013MD)
	elseif((${CMAKE_GENERATOR} STREQUAL "Visual Studio 14 2015" OR ${CMAKE_GENERATOR} STREQUAL "Visual Studio 14 2015 Win64" OR ${CMAKE_GENERATOR} STREQUAL "Visual Studio 14 Win64" OR ${CMAKE_GENERATOR} STREQUAL "Visual Studio 14" ) )
		set(LIB_REDSDKd R3DSDK-2015MDd)
		set(LIB_REDSDK R3DSDK-2015MD)
	elseif((${CMAKE_GENERATOR} STREQUAL "Visual Studio 15 2017" OR ${CMAKE_GENERATOR} STREQUAL "Visual Studio 15 2017 Win64" OR ${CMAKE_GENERATOR} STREQUAL "Visual Studio 15 Win64" OR ${CMAKE_GENERATOR} STREQUAL "Visual Studio 15" ) )
		set(LIB_REDSDKd R3DSDK-2015MDd)
		set(LIB_REDSDK R3DSDK-2015MD) # change it to 2017MD once we obtain 2017 libs for REDSDK 
	elseif((${CMAKE_GENERATOR} STREQUAL "Visual Studio 16 2019" OR ${CMAKE_GENERATOR} STREQUAL "Visual Studio 16 2019 Win64" OR ${CMAKE_GENERATOR} STREQUAL "Visual Studio 16 Win64" OR ${CMAKE_GENERATOR} STREQUAL "Visual Studio 16" ) )
		set(LIB_REDSDKd R3DSDK-2017MDd)
		set(LIB_REDSDK R3DSDK-2017MD) # change it to 2019MD once we obtain 2019 libs for REDSDK 
	endif()
	link_directories("${XMPROOT_DIR}/XMPFilesPlugins/${PRODUCT_NAME}/third-party/REDSDK/lib/${XMP_PLATFORM_SHORT}${BITS}")
elseif(UNIX)
	if(APPLE)
		if(CMAKE_LIBCPP)
			set(LIB_REDSDK R3DSDK-libcpp)
		else(CMAKE_LIBCPP)
			set(LIB_REDSDK R3DSDK)
		endif(CMAKE_LIBCPP)
		link_directories("${XMPROOT_DIR}/XMPFilesPlugins/${PRODUCT_NAME}/third-party/REDSDK/Lib/mac64")
	else()
		set(LIB_REDSDK R3DSDKPIC)
		string(TOLOWER ${CMAKE_BUILD_TYPE} TEMP_DEBUG_STRING)
		message (STATUS " {linux link_directories}		")
		link_directories("${XMPROOT_DIR}/public/libraries/${XMP_PLATFORM_FOLDER}/${TEMP_DEBUG_STRING}")
		link_directories("${XMPROOT_DIR}/XMPFilesPlugins/${PRODUCT_NAME}/third-party/REDSDK/Lib/${XMP_PLATFORM_SHORT}${BITS}")
	endif()
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
		${LIB_REDSDK}
	)
elseif(WIN32)
	target_link_libraries(
		${TARGET_NAME} 
		debug ${LIB_REDSDKd} 
		optimized ${LIB_REDSDK} 
		XMPCore 
		XMPFiles 
		${XMP_PLATFORM_LINK}
	)
else()
	target_link_libraries(
		${TARGET_NAME}
		XMPCore
		XMPFiles
		${LIB_REDSDK}
		${XMP_PLATFORM_LINK}
	)
endif()

set(FRAMEWORK_LIST "Mac:CoreFoundation" "Mac:Foundation" "Mac:CoreServices" "Mac:IOKit")
AddMacFramework(${TARGET_NAME} FRAMEWORK_LIST)

SetWinLinkFlags(${TARGET_NAME} "" "${TARGET_NAME}")

if(APPLE)
	set_target_properties(${TARGET_NAME} PROPERTIES BUILD_WITH_INSTALL_RPATH ON BUILD_WITH_INSTALL_NAME_DIR ON INSTALL_NAME_DIR "@executable_path/../Frameworks")
endif()

set(DEPEND_LIBS "")
CreatePlugin(${TARGET_NAME} ${XMPPLUGIN_OUTPUT_DIR} "${PROJECT_ROOT}/resource/txt/*.txt" XMP_PLUGIN_DEPENDENCIES DEPEND_LIBS)

if (UNIX)
	if(APPLE)
		if (XMP_BUILD_STATIC)
			add_custom_command (TARGET ${TARGET_NAME} POST_BUILD 
				COMMAND cp -f ${PROJECT_ROOT}/../third-party/REDSDK/Redistributable/mac/REDR3D.dylib ${XMPPLUGIN_OUTPUT_DIR}/
				COMMENT "Copy R3D dynamic libraries")
		else()
			add_custom_command (TARGET ${TARGET_NAME} POST_BUILD 
				COMMAND cp -f ${PROJECT_ROOT}/resource/txt/MODULE_IDENTIFIER.txt ${XMPPLUGIN_OUTPUT_DIR}/${TARGET_NAME}.xpi/Versions/A/Resources/MODULE_IDENTIFIER.txt
				COMMAND cp -f ${PROJECT_ROOT}/resource/txt/XMPPLUGINUIDS-${BITS}.txt ${XMPPLUGIN_OUTPUT_DIR}/${TARGET_NAME}.xpi/Versions/A/Resources/XMPPLUGINUIDS.txt
				COMMAND cp -f ${PROJECT_ROOT}/../third-party/REDSDK/Redistributable/mac/REDR3D.dylib ${XMPPLUGIN_OUTPUT_DIR}/
				COMMENT "Copy resource and Copy R3D dynamic libraries")
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
		if(${BITS} STREQUAL "64")
			add_custom_command (TARGET ${TARGET_NAME} POST_BUILD 
				COMMAND mkdir -p ${XMPPLUGIN_OUTPUT_DIR}/${TARGET_NAME}.resources
				COMMAND cp -f ${PROJECT_ROOT}/resource/txt/MODULE_IDENTIFIER.txt ${XMPPLUGIN_OUTPUT_DIR}/${TARGET_NAME}.resources/MODULE_IDENTIFIER.txt
				COMMAND cp -f ${PROJECT_ROOT}/resource/txt/XMPPLUGINUIDS-${BITS}.txt ${XMPPLUGIN_OUTPUT_DIR}/${TARGET_NAME}.resources/XMPPLUGINUIDS.txt
				COMMAND cp -f ${PROJECT_ROOT}/../third-party/REDSDK/Redistributable/linux/REDR3D-x64.so ${XMPPLUGIN_OUTPUT_DIR}/
				COMMENT "Copy resource and R3D binaries")
		else()
			add_custom_command (TARGET ${TARGET_NAME} POST_BUILD 
				COMMAND mkdir -p ${XMPPLUGIN_OUTPUT_DIR}/${TARGET_NAME}.resources
				COMMAND cp -f ${PROJECT_ROOT}/resource/txt/MODULE_IDENTIFIER.txt ${XMPPLUGIN_OUTPUT_DIR}/${TARGET_NAME}.resources/MODULE_IDENTIFIER.txt
				COMMAND cp -f ${PROJECT_ROOT}/resource/txt/XMPPLUGINUIDS-${BITS}.txt ${XMPPLUGIN_OUTPUT_DIR}/${TARGET_NAME}.resources/XMPPLUGINUIDS.txt
				COMMAND cp -f ${PROJECT_ROOT}/../third-party/REDSDK/Redistributable/linux/REDR3D-x86.so ${XMPPLUGIN_OUTPUT_DIR}/
				COMMENT "Copy resource and R3D binaries")
		endif()
	endif()
endif()
if (WIN32) 
	if(${BITS} STREQUAL "64")	
		add_custom_command (TARGET ${TARGET_NAME} POST_BUILD COMMAND ${CMAKE_COMMAND} -E copy ${PROJECT_ROOT}/../third-party/REDSDK/Redistributable/win/REDR3D-x64.dll  ${XMPPLUGIN_OUTPUT_DIR}/${TARGET_NAME}/
		COMMENT "Copying R3D dll")
	elseif(${BITS} STREQUAL "32")
		add_custom_command (TARGET ${TARGET_NAME} POST_BUILD COMMAND ${CMAKE_COMMAND} -E copy ${PROJECT_ROOT}/../third-party/REDSDK/Redistributable/win/REDR3D-x86.dll  ${XMPPLUGIN_OUTPUT_DIR}/${TARGET_NAME}/
		COMMENT "Copying R3D dll")
	endif()
endif()
# ==============================================================================
# configuration feedback
# ==============================================================================

message (STATUS "===========================================================================")
message (STATUS " ${TARGET_NAME}		")
message (STATUS "===========================================================================")

