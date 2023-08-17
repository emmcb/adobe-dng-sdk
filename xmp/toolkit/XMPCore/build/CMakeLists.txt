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
set(TARGET_NAME XMPCore)
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
set(PROJECT_LABEL_STR "${TARGET_NAME}")

# ==============================================================================
# Shared config
# ==============================================================================
# setup some values before calling shared config
set(XMP_THIS_PROJECT_RELATIVEPATH "../../..")

include(${CMAKE_CURRENT_SOURCE_DIR}/${XMP_THIS_PROJECT_RELATIVEPATH}/toolkit/build/XMP_Config.cmake)

add_definitions(-DBUILDING_XMPCORE_LIB=1 -DXMP_COMPONENT_INT_NAMESPACE=AdobeXMPCore_Int)
if (XMP_BUILD_STATIC)
	add_definitions(-DBUILDING_XMPCORE_AS_STATIC=1)
else()
	add_definitions(-DBUILDING_XMPCORE_AS_DYNAMIC=1)
endif()
set(TP_ZUID_PATH "${XMPROOT_DIR}/third-party/zuid/sources")
set(TP_EXPAT_PATH "${PROJECT_ROOT}/../third-party/expat/public/lib")

# ==============================================================================
# Project specific config starts here
# ==============================================================================
if(UNIX)
	if(APPLE)
		# OSX -------------------------------------------
		if(XMP_BUILD_STATIC)
			set(XMPCORE_LIB "XMPCoreStatic$(CONFIGURATION)")
		else(XMP_BUILD_STATIC)
			set(XMPCORE_LIB "AdobeXMP")
		endif()
	else(APPLE)
		# Linux -------------------------------------------
		if(XMP_BUILD_STATIC)
			set(XMPCORE_LIB "staticXMPCore")
		else(XMP_BUILD_STATIC)
			set(XMPCORE_LIB "AdobeXMP")
		endif()
	endif(APPLE)
else(UNIX)
	if(WIN32)
		# Windows -------------------------------------------
		if(XMP_BUILD_STATIC)
			set(XMPCORE_LIB "XMPCoreStatic${CMAKE_CFG_INTDIR}")
		else(XMP_BUILD_STATIC)
			set(XMPCORE_LIB "AdobeXMP")
		endif()
	endif(WIN32)
endif(UNIX)

# ==============================================================================
# Only the files which are not included in SDK
# ==============================================================================
list (APPEND INTERNAL_HEADER_FILES
	${SOURCE_ROOT}/XMPCore_ChangeLog.hpp
	${SOURCE_ROOT}/XMPDocOps-Utils.hpp
	${SOURCE_ROOT}/XMPDocOps.hpp
	)

if (ANDROID)
list (APPEND INTERNAL_HEADER_FILES
	${SOURCE_ROOT}/Android_Utils.hpp
	)

file(GLOB ANDROID_UTILS ${SOURCE_ROOT}/Android_Utils.cpp)
source_group("Source Files\\Private\\XMPCore" FILES ${ANDROID_UTILS})

endif(ANDROID)

if (XMPWASM)
list (APPEND INTERNAL_HEADER_FILES
	${XMPROOT_DIR}/XMPWasm/include/MetaOpsWrapper.hpp
	${XMPROOT_DIR}/XMPWasm/include/MetaOps.hpp
	${XMPROOT_DIR}/XMPWasm/include/XMPOpsException.hpp
	)
	
	list (APPEND UTILITY_SOURCE_FILES
	${XMPROOT_DIR}/XMPWasm/source/MetaOpsWrapper.cpp
	${XMPROOT_DIR}/XMPWasm/source/MetaOps.cpp
	)
endif()

include(${CMAKE_CURRENT_SOURCE_DIR}/${XMP_THIS_PROJECT_RELATIVEPATH}/toolkit/XMPCore/build/CMakeListsCommon.txt)
# ==============================================================================
# Post build - only required for Toolkit
# ==============================================================================
if (UNIX)
	if(APPLE AND NOT ${XMP_BUILD_STATIC})
		string(TOLOWER ${CMAKE_BUILD_TYPE} LOWERCASE_CMAKE_BUILD_TYPE)
		set(CLIENT_DIR ${XMPROOT_DIR}/target/${XMP_PLATFORM_FOLDER}/${LOWERCASE_CMAKE_BUILD_TYPE}/Frameworks)
		
		# env OTHER_CFLAGS contains proper debug/release preprocessor defines

		add_custom_command (TARGET ${TARGET_NAME} 
			POST_BUILD
			COMMAND mkdir -p \"${CLIENT_DIR}\"
			COMMAND rm -f \"${CLIENT_DIR}/${PRODUCT_NAME}.framework\"
			COMMAND ln -fs \"${OUTPUT_DIR}/${XMPCORE_LIB}.framework\" \"${CLIENT_DIR}/${XMPCORE_LIB}.framework\"
		)
		if(APPLE_IOS)
			add_custom_command (TARGET ${TARGET_NAME} 
				POST_BUILD
				COMMAND strip -x \"${OUTPUT_DIR}$(CONFIGURATION)/${XMPCORE_LIB}.framework/${XMPCORE_LIB}\"
		)
		endif()
	endif()
endif()
# ==================================================================================================================
# Post build - only required for copying PDBs of Windows Static libs as linker does not copy them directly as in DLL
# ==================================================================================================================

# if(WIN32 AND ${XMP_BUILD_STATIC})
# set(pdbpath "${CMAKE_CURRENT_SOURCE_DIR}/${XMP_CMAKEFOLDER_NAME}/${TARGET_NAME}.dir/$(CONFIGURATION)/${PROJECT_LABEL_STR}.pdb")
# 		set(pdboutputPath "${OUTPUT_DIR}/${TARGET_NAME}$(CONFIGURATION).pdb")
# 		add_custom_command(TARGET ${TARGET_NAME} 
# 			POST_BUILD
# 			COMMAND echo - copying PDBs from int dir ${pdbpath} to output dir ${pdboutputPath}
# 			COMMAND ${CMAKE_COMMAND} -E copy
#                 ${pdbpath}
#                 ${pdboutputPath})
# endif()



