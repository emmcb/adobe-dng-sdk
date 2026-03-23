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
set(TARGET_NAME XMPFiles)
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

set(TP_ZUID_PATH "${XMPROOT_DIR}/third-party/zuid/sources")
set(TP_EXPAT_PATH "${XMPROOT_DIR}/XMPCore/third-party/expat/public/lib")
set(LIB_ADOBEXMP AdobeXMP)

if(ANDROID)
set(LIB_ADOBEXMP libAdobeXMP.so)
endif()

# ==============================================================================
# platform specific config
# ==============================================================================
if(UNIX)
	if(APPLE)
		# OSX -------------------------------------------
		if(XMP_BUILD_STATIC)
			set(XMPFILES_LIB "XMPFilesStatic$(CONFIGURATION)")
		else()
			set(XMPFILES_LIB "AdobeXMPFiles")
		endif()
	else(APPLE)
		# Linux -------------------------------------------
		if(XMP_BUILD_STATIC)
			set(XMPFILES_LIB "staticXMPFiles")
		else()
			set(XMPFILES_LIB "AdobeXMPFiles")
		endif()
	endif(APPLE)
else(UNIX)
	if(WIN32)
		# Windows -------------------------------------------
		if(XMP_BUILD_STATIC)
			set(XMPFILES_LIB "XMPFilesStatic${CMAKE_CFG_INTDIR}")
		else(XMP_BUILD_STATIC)
			set(XMPFILES_LIB "AdobeXMPFiles")
		endif()
	endif(WIN32)
endif(UNIX)

# ==============================================================================
# The file is only for Toolkit and not SDK
# ==============================================================================
list (APPEND INTERNAL_HEADER_COMMONCODE
	${SOURCE_ROOT}/XMPFiles_ChangeLog.hpp
	)
	
if(XMPWASM)
list (APPEND INTERNAL_HEADER_COMMONCODE
	${XMPROOT_DIR}/XMPWasm/include/XMPFileOps.hpp
	${XMPROOT_DIR}/XMPWasm/include/XMPFileOpsWrapper.hpp
	${XMPROOT_DIR}/XMPWasm/include/XMPOpsException.hpp
	${XMPROOT_DIR}/source/XMPStream_IO.hpp
	)

list (APPEND SOURCEFILES_COMMONCODE
	${XMPROOT_DIR}/XMPWasm/source/XMPFileOps.cpp
	${XMPROOT_DIR}/XMPWasm/source/XMPFileOpsWrapper.cpp
	${XMPROOT_DIR}/source/XMPStream_IO.cpp
	)
	
endif()

list (APPEND PUBLIC_HEADER_CLIENTGLUE
	${XMPROOT_DIR}/public/include/client-glue/TXMPAlbumArt.incl_cpp
	)
	
list (APPEND PUBLIC_HEADER
	${XMPROOT_DIR}/public/include/TXMPAlbumArt.hpp
	)

include(${CMAKE_CURRENT_SOURCE_DIR}/${XMP_THIS_PROJECT_RELATIVEPATH}/toolkit/XMPFiles/build/CMakeListsCommon.txt)

# ==============================================================================
# Post build
# ==============================================================================

if(UNIX)
	if(APPLE AND NOT ${XMP_BUILD_STATIC})
		string(TOLOWER ${CMAKE_BUILD_TYPE} LOWERCASE_CMAKE_BUILD_TYPE)
		set(CLIENT_DIR ${XMPROOT_DIR}/target/${XMP_PLATFORM_FOLDER}/${LOWERCASE_CMAKE_BUILD_TYPE}/Frameworks)

		add_custom_command (TARGET ${TARGET_NAME}
			POST_BUILD
			COMMAND mkdir -p \"${CLIENT_DIR}\"
			COMMAND rm -f \"${CLIENT_DIR}/${XMPFILES_LIB}.framework\"
			COMMAND ln -fs \"${OUTPUT_DIR}/${XMPFILES_LIB}.framework\" \"${CLIENT_DIR}/${XMPFILES_LIB}.framework\"
		)
		if(APPLE_IOS)
			add_custom_command (TARGET ${TARGET_NAME} 
				POST_BUILD
				COMMAND strip -x \"${OUTPUT_DIR}$(CONFIGURATION)/${XMPFILES_LIB}.framework/${XMPFILES_LIB}\"
		)
		endif()
	endif()
endif()

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
