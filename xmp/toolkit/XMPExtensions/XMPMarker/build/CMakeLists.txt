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
set(TARGET_NAME XMPAssetManagement)
if(CMAKE_CL_64)
	project(${TARGET_NAME}64)
else(CMAKE_CL_64)
	project(${TARGET_NAME})
endif(CMAKE_CL_64)

set(STATIC_STR "Static")
if (XMP_BUILD_STATIC)
	set(TARGET_NAME "${TARGET_NAME}${STATIC_STR}")
	set(PROJECT_LABEL_STR "${TARGET_NAME} ${STATIC_STR}")
else(XMP_BUILD_STATIC)
	set(PROJECT_LABEL_STR "${TARGET_NAME}")
endif(XMP_BUILD_STATIC)

# ==============================================================================
# Shared config
# ==============================================================================
# setup some values before calling shared config
set(XMP_THIS_PROJECT_RELATIVEPATH "../../..")

include(${CMAKE_CURRENT_SOURCE_DIR}/${XMP_THIS_PROJECT_RELATIVEPATH}/toolkit/build/XMP_Config.cmake)

add_definitions(-DBUILDING_XMPASSETMANAGEMENT_LIB=1 -DDEPENDS_ON_XMPCORE=0)

set(LIB_ADOBEXMP AdobeXMP)

# ==============================================================================
# platform specific config
# ==============================================================================
if(UNIX)
	if(APPLE)
		# OSX -------------------------------------------
		if(XMP_BUILD_STATIC)
			set(XMPASSETMANAGEMENT_LIB "XMPAssetManagementStatic$(CONFIGURATION)")
		else()
			set(XMPASSETMANAGEMENT_LIB "AdobeXMPAssetManagement")
		endif()
	else(APPLE)
		# Linux -------------------------------------------
		if(XMP_BUILD_STATIC)
			set(XMPASSETMANAGEMENT_LIB "staticXMPAssetManagement}")
		else()
			set(XMPASSETMANAGEMENT_LIB "AdobeXMPAssetManagement")
		endif()
	endif(APPLE)
else(UNIX)
	if(WIN32)
		# Windows -------------------------------------------
		if(XMP_BUILD_STATIC)
			set(XMPASSETMANAGEMENT_LIB "XMPAssetManagementStatic${CMAKE_CFG_INTDIR}")
		else(XMP_BUILD_STATIC)
			set(XMPASSETMANAGEMENT_LIB "AdobeXMPAssetManagement")
		endif()
	endif(WIN32)
endif(UNIX)

# ==============================================================================
# The file is only for Toolkit and not SDK
# ==============================================================================
list (APPEND INTERNAL_HEADER_COMMONCODE
	${SOURCE_ROOT}/XMPAssetManagement_ChangeLog.hpp
	)

include(${CMAKE_CURRENT_SOURCE_DIR}/${XMP_THIS_PROJECT_RELATIVEPATH}/toolkit/XMPAssetManagement/build/CMakeListsCommon.txt)

# ==============================================================================
# Post build
# ==============================================================================

if(UNIX)
	if(APPLE AND NOT ${XMP_BUILD_STATIC})
		string(TOLOWER ${CMAKE_BUILD_TYPE} LOWERCASE_CMAKE_BUILD_TYPE)
		set(CLIENT_DIR ${XMPROOT_DIR}/target/${XMP_PLATFORM_FOLDER}/${LOWERCASE_CMAKE_BUILD_TYPE}/Frameworks)

		add_custom_command (TARGET ${TARGET_NAME}
			POST_BUILD
			COMMAND mkdir -p ${CLIENT_DIR}
			COMMAND rm -f ${CLIENT_DIR}/${XMPASSETMANAGEMENT_LIB}.framework
			COMMAND ln -fs ${OUTPUT_DIR}/${XMPASSETMANAGEMENT_LIB}.framework ${CLIENT_DIR}/${XMPASSETMANAGEMENT_LIB}.framework
		)
	endif()
else()
	if(WIN32 AND NOT ${XMP_BUILD_STATIC})
		add_custom_command (TARGET ${PROJECT_NAME}
			POST_BUILD
			COMMAND ${XMPROOT_DIR}/build/CopyTargetDLL.bat ${XMPASSETMANAGEMENT_LIB} ${CMAKE_CFG_INTDIR}
		)
	endif()
endif()