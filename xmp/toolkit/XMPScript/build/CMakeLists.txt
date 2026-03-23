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
set(TARGET_NAME XMPScript)
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

# ==============================================================================
# platform specific config
# ==============================================================================
if(UNIX)
	if(APPLE)
		# OSX -------------------------------------------
		if(XMP_BUILD_STATIC)
			set(XMPSCRIPT_LIB "XMPScriptStatic${CMAKE_CFG_INTDIR}")
		else()
			set(XMPSCRIPT_LIB "AdobeXMPScript")
		endif()
	else(APPLE)
		# Linux -------------------------------------------
		set(XMPSCRIPT_LIB "libAdobeXMPScript")
		set(LIB_EXT ".so")
	endif(APPLE)
else(UNIX)
	if(WIN32)
		# Windows -------------------------------------------
		if(XMP_BUILD_STATIC)
			set(XMPSCRIPT_LIB "XMPScriptStatic${CMAKE_CFG_INTDIR}")
		else(XMP_BUILD_STATIC)
			set(XMPSCRIPT_LIB "AdobeXMPScript")
		endif()
	endif(WIN32)
endif(UNIX)

# ==============================================================================
# For convenience we define the sources as a variable. You can add 
# header files and cpp/c files and CMake will sort them out
# ==============================================================================

#header files
file(GLOB HEADER_FILES ${PROJECT_ROOT}/../include/*.h)
list(APPEND HEADER_FILES ${SOURCE_ROOT}/XMPScript_ChangeLog.hpp)
source_group("Header Files" FILES ${HEADER_FILES})

#resource files
file (GLOB RESOURCE_FILES ${RESOURCE_ROOT}/${XMP_PLATFORM_SHORT}/${TARGET_NAME}.*)
if(WIN32 AND ${XMP_BUILD_STATIC})
		list (REMOVE_ITEM RESOURCE_FILES ${RESOURCE_ROOT}/${XMP_PLATFORM_SHORT}/${TARGET_NAME}.rc)
endif()
source_group("Resource Files" FILES ${RESOURCE_FILES})

#source files
file (GLOB SOURCE_FILES ${SOURCE_ROOT}/*.cpp)
source_group("Source Files" FILES ${SOURCE_FILES})

file (GLOB PUBLIC_XMPCOMMON_CLIENTGLUE_FILES ${XMPROOT_DIR}/public/include/XMPCommon/source/*.*)
source_group("Source Files\\Public\\XMPCommon" FILES ${PUBLIC_XMPCOMMON_CLIENTGLUE_FILES})

file (GLOB PUBLIC_XMPCORE_CLIENTGLUE_FILES ${XMPROOT_DIR}/public/include/XMPCore/source/*.*)
source_group("Source Files\\Public\\XMPCore" FILES ${PUBLIC_XMPCORE_CLIENTGLUE_FILES})


list(APPEND SOURCE_FILES ${HEADER_FILES}  ${RESOURCE_FILES} ${SOURCE_FILES} ${PUBLIC_XMPCOMMON_CLIENTGLUE_FILES} ${PUBLIC_XMPCORE_CLIENTGLUE_FILES})

# include directories
include_directories(${XMPROOT_DIR})
include_directories(${XMPROOT_DIR}/build)
include_directories(${PROJECT_ROOT}/../include)
include_directories(${XMPROOT_DIR}/public/include)
include_directories(${PROJECT_ROOT}/../third-party/extendscript/sccore/public/include)
include_directories(${PROJECT_ROOT}/../third-party/extendscript/extendscript/public/include)
include_directories(${PROJECT_ROOT}/../third-party/extendscript/scriptobjects/public/include)
include_directories(${RESOURCE_ROOT}/${XMP_PLATFORM_SHORT})

# ==============================================================================
#Define-undefine some project specific preprocessor macros, which has been not
#list(APPEND CMAKE_CXX_FLAGS_DEBUG " -D_CRTDBG_MAP_ALLOC=1")
add_definitions(-DXMP_INCLUDE_XMPFILES=1)
remove_definitions(-DHAVE_EXPAT_CONFIG_H=1 -DXML_STATIC=1 -DAdobePrivate=1)

#additional link directory
if(UNIX)
	if(APPLE)
		set(XMP_FOLDER ${XMP_PLATFORM_FOLDER})
		if(CMAKE_LIBCPP)
			string (REPLACE "_libcpp" "" XMP_FOLDER ${XMP_FOLDER})
		endif()
		find_library(LIB_EXTENDSCRIPT NAMES AdobeExtendScript PATHS ${PROJECT_ROOT}/../third-party/extendscript/extendscript/public/libraries/${XMP_FOLDER}/release)
		find_library(LIB_SCCORE NAMES AdobeScCore PATHS ${PROJECT_ROOT}/../third-party/extendscript/sccore/public/libraries/${XMP_FOLDER}/release)
		#	find_library(LIB_ADOBEXMP NAMES AdobeXMP PATHS ${XMPROOT_DIR}/public/libraries/${XMP_PLATFORM_FOLDER}/${CMAKE_BUILD_TYPE})
		#	find_library(LIB_ADOBEXMPFILES NAMES AdobeXMPFiles PATHS ${XMPROOT_DIR}/public/libraries/${XMP_PLATFORM_FOLDER}/${CMAKE_BUILD_TYPE})
	endif()
else()
	set(LIB_EXTENDSCRIPT ExtendScript)
	link_directories(${OUTPUT_DIR})
	set(LIB_SCCORE ScCore)
	link_directories(${OUTPUT_DIR})
	set(LIB_ADOBEXMP AdobeXMP)
	link_directories(${OUTPUT_DIR})
	set(LIB_ADOBEXMPFILES AdobeXMPFiles)
	link_directories(${OUTPUT_DIR})
endif()
set(LIB_ADOBEXMP AdobeXMP)
set(LIB_ADOBEXMPFILES AdobeXMPFiles)

if ( NOT APPLE )
	link_directories(${OUTPUT_DIR} ${PROJECT_ROOT}/../third-party/extendscript/sccore/public/libraries/${XMP_PLATFORM_FOLDER}/release ${PROJECT_ROOT}/../third-party/extendscript/extendscript/public/libraries/${XMP_PLATFORM_FOLDER}/release)
endif()

# ==============================================================================
# Define what to do, lib, exe, etc
if (UNIX AND APPLE AND NOT ${XMP_BUILD_STATIC})
	# preprocess Info.plist
	add_custom_target(${TARGET_NAME}InfoPlist
		COMMAND mkdir -p ${CMAKE_CURRENT_BINARY_DIR}/${CMAKE_CFG_INTDIR}
		COMMAND if [ $(CONFIGURATION) != Debug ]; then
			${GCCTOOL} -E -P -x c ${RESOURCE_ROOT}/${XMP_PLATFORM_SHORT}/${TARGET_NAME}.plist
			-F${CMAKE_CURRENT_BINARY_DIR}/${CMAKE_CFG_INTDIR}/
			-DPRODUCT_NAME=${TARGET_NAME} -DMAC_ENV=1 -DNDEBUG=1
			-include ${RESOURCE_ROOT}/${XMP_PLATFORM_SHORT}/${TARGET_NAME}PList.h
			-o ${CMAKE_CURRENT_BINARY_DIR}/${CMAKE_CFG_INTDIR}/Info.plist
			\; else
			${GCCTOOL} -E -P -x c ${RESOURCE_ROOT}/${XMP_PLATFORM_SHORT}/${TARGET_NAME}.plist
			-F${CMAKE_CURRENT_BINARY_DIR}/${CMAKE_CFG_INTDIR}/
			-DPRODUCT_NAME=${TARGET_NAME} -DMAC_ENV=1 -DDEBUG=1
			-include ${RESOURCE_ROOT}/${XMP_PLATFORM_SHORT}/${TARGET_NAME}PList.h
			-o ${CMAKE_CURRENT_BINARY_DIR}/${CMAKE_CFG_INTDIR}/Info.plist
			\; fi
		COMMAND rm -f ${CMAKE_CURRENT_BINARY_DIR}/${CMAKE_CFG_INTDIR}/../CMakeFiles/${TARGET_NAME}.dir/Info.plist
		COMMAND cp ${CMAKE_CURRENT_BINARY_DIR}/${CMAKE_CFG_INTDIR}/Info.plist ${CMAKE_CURRENT_BINARY_DIR}/${CMAKE_CFG_INTDIR}/../CMakeFiles/${TARGET_NAME}.dir/Info.plist
		COMMENT "Preprocessing Info-plist"
	)
	set(DEPENDENCY_LIST "ALL:${TARGET_NAME}InfoPlist" "DLL:XMPCore" "DLL:XMPFiles")
else ()
	set(DEPENDENCY_LIST "DLL:XMPCore" "DLL:XMPFiles")
endif()

AddLibraryAndDependencies(${TARGET_NAME} ${XMP_BUILD_STATIC} YES "SHARED" SOURCE_FILES DEPENDENCY_LIST)

# link flags for Linux
if(UNIX AND NOT APPLE)
	# Linux -------------------------------------------
	set_target_properties(${PROJECT_NAME} PROPERTIES LINK_FLAGS "-shared -fPIC -z defs -Xlinker -Bsymbolic -Xlinker --version-script -Xlinker XMPCore.exp")
endif()

if(WIN32 OR (UNIX AND NOT APPLE))
	target_link_libraries(
		${TARGET_NAME}
		#${LIB_EXTENDSCRIPT}
		#${LIB_SCCORE}
		${LIB_ADOBEXMP}
		${LIB_ADOBEXMPFILES}
		${XMP_PLATFORM_LINK}
	)
else()
	target_link_libraries(
		${TARGET_NAME}
		#${LIB_EXTENDSCRIPT}
		#${LIB_SCCORE}
	)
endif()

set(FRAMEWORK_LIST "Mac:CoreFoundation" "Mac:CoreServices" "Mac:${LIB_ADOBEXMP}" "Mac:${LIB_ADOBEXMPFILES}" "Mac:${XMP_PLATFORM_LINK}" "Mac:${XMP_PLATFORM_LINK}")
AddMacFramework(${TARGET_NAME} FRAMEWORK_LIST)

if (WIN32)
	SetWinLinkFlags(${TARGET_NAME} "rpcrt4.lib" "${XMPSCRIPT_LIB}")
endif()

if(APPLE)
	SetWinLinkFlags(${TARGET_NAME} "-exported_symbols_list ${RESOURCE_ROOT}/${XMP_PLATFORM_SHORT}/XMPScript.exp" "${XMPSCRIPT_LIB}")
	set_target_properties(${TARGET_NAME} PROPERTIES BUILD_WITH_INSTALL_RPATH ON BUILD_WITH_INSTALL_NAME_DIR ON INSTALL_NAME_DIR "@executable_path/../Frameworks")
endif()

set_target_properties(${TARGET_NAME} PROPERTIES OUTPUT_NAME  ${XMPSCRIPT_LIB})

# ==============================================================================
# Define output for this project
SetOutputPath(${OUTPUT_DIR} 0)

if (UNIX)
		#hack for unix to rename the output static library. cmake add lib and extenstion as .a, rename it
		if (${XMP_BUILD_STATIC} AND NOT APPLE)
			add_custom_command (TARGET ${TARGET_NAME}
				POST_BUILD
				COMMAND  mv ${OUTPUT_DIR}/lib${XMPCORE_LIB}.a  ${OUTPUT_DIR}/${XMPCORE_LIB}.ar
			)
		endif()

		if(APPLE AND NOT ${XMP_BUILD_STATIC})
			string(TOLOWER ${CMAKE_BUILD_TYPE} LOWERCASE_CMAKE_BUILD_TYPE)
			set(CLIENT_DIR ${XMPROOT_DIR}/target/${XMP_PLATFORM_FOLDER}/${LOWERCASE_CMAKE_BUILD_TYPE}/Frameworks)
			add_custom_command (TARGET ${TARGET_NAME}
					POST_BUILD
					COMMAND mkdir -p ${CLIENT_DIR}
					COMMAND rm -f ${CLIENT_DIR}/${XMPSCRIPT_LIB}.framework
					COMMAND ln -fs ${OUTPUT_DIR}/${XMPSCRIPT_LIB}.framework ${CLIENT_DIR}/${XMPSCRIPT_LIB}.framework
			)
	endif()
else()
		set_target_properties(${TARGET_NAME} PROPERTIES PROJECT_LABEL ${PROJECT_LABEL_STR})
endif()

if(WIN32 AND ${XMP_BUILD_STATIC})
set(pdbpath "${CMAKE_CURRENT_SOURCE_DIR}/${XMP_CMAKEFOLDER_NAME}/${TARGET_NAME}.dir/$(CONFIGURATION)/${PROJECT_LABEL_STR}.pdb")
		set(pdboutputPath "${OUTPUT_DIR}/${TARGET_NAME}$(CONFIGURATION).pdb")
		add_custom_command(TARGET ${TARGET_NAME} 
			POST_BUILD
			COMMAND echo - copying PDBs from int dir ${pdbpath} to output dir ${pdboutputPath}
			COMMAND ${CMAKE_COMMAND} -E copy
                ${pdbpath}
                ${pdboutputPath})
endif()

message (STATUS "===========================================================================")
message (STATUS " ${PROJECT_NAME} ")
message (STATUS "===========================================================================")
message (STATUS " OUTPUT_DIR = ${OUTPUT_DIR}")


