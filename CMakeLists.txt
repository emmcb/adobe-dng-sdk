cmake_minimum_required(VERSION 3.21)
project(adobe-dng-sdk)

option(BUILD_DNG_VALIDATE "Enable dng_validate command line tool" ${PROJECT_IS_TOP_LEVEL})
option(DNG_THREAD_SAFE "Enable thread-safe support in the DNG SDK" ON)
option(DNG_WITH_JPEG "Enable JPEG support in the DNG SDK" ON)
option(DNG_WITH_XMP "Enable XMP support in the DNG SDK" ON)

find_package(ZLIB REQUIRED)

if(DNG_WITH_JPEG)
    find_package(JPEG REQUIRED)
endif()
if(DNG_WITH_XMP)
    find_package(EXPAT REQUIRED)
endif()

#
# XMP SDK
#

if(DNG_WITH_XMP)
    set(TARGET XMPCore)
    set(SRC_DIR xmp/toolkit)
    set(PUBLIC_HDR_DIR xmp/toolkit/public/include)
    set(PRIVATE_HDR_DIR xmp/toolkit)

    set(SRCS
        ${SRC_DIR}/source/UnicodeConversions.cpp
        ${SRC_DIR}/source/XML_Node.cpp
        ${SRC_DIR}/source/XMP_LibUtils.cpp
        ${SRC_DIR}/third-party/zuid/interfaces/MD5.cpp
        ${SRC_DIR}/XMPCommon/source/AutoSharedLock.cpp
        ${SRC_DIR}/XMPCommon/source/ConfigurableImpl.cpp
        ${SRC_DIR}/XMPCommon/source/ConfigurationManagerImpl.cpp
        ${SRC_DIR}/XMPCommon/source/DefaultErrorNotifierImpl.cpp
        ${SRC_DIR}/XMPCommon/source/ErrorImpl.cpp
        ${SRC_DIR}/XMPCommon/source/ErrorNotifierWrapperImpl.cpp
        ${SRC_DIR}/XMPCommon/source/IConfigurable_I.cpp
        ${SRC_DIR}/XMPCommon/source/IConfigurationManager_I.cpp
        ${SRC_DIR}/XMPCommon/source/IErrorNotifier_I.cpp
        ${SRC_DIR}/XMPCommon/source/IError_I.cpp
        ${SRC_DIR}/XMPCommon/source/IMemoryAllocator_I.cpp
        ${SRC_DIR}/XMPCommon/source/IUTF8StringComparator.cpp
        ${SRC_DIR}/XMPCommon/source/IUTF8String_I.cpp
        ${SRC_DIR}/XMPCommon/source/MemoryAllocatorWrapperImpl.cpp
        ${SRC_DIR}/XMPCommon/source/MemoryManagedObject.cpp
        ${SRC_DIR}/XMPCommon/source/SharedMutexImpl.cpp
        ${SRC_DIR}/XMPCommon/source/SharedObjectImpl.cpp
        ${SRC_DIR}/XMPCommon/source/ThreadSafeImpl.cpp
        ${SRC_DIR}/XMPCommon/source/UTF8StringImpl.cpp
        ${SRC_DIR}/XMPCommon/source/XMPCommon.cpp
        ${SRC_DIR}/XMPCore/source/ArrayNodeImpl.cpp
        ${SRC_DIR}/XMPCore/source/ClientDOMParserWrapperImpl.cpp
        ${SRC_DIR}/XMPCore/source/ClientDOMSerializerWrapperImpl.cpp
        ${SRC_DIR}/XMPCore/source/CompositeNodeImpl.cpp
        ${SRC_DIR}/XMPCore/source/CoreConfigurationManagerImpl.cpp
        ${SRC_DIR}/XMPCore/source/CoreObjectFactoryImpl.cpp
        ${SRC_DIR}/XMPCore/source/DOMImplementationRegistryImpl.cpp
        ${SRC_DIR}/XMPCore/source/DOMParserImpl.cpp
        ${SRC_DIR}/XMPCore/source/DOMSerializerImpl.cpp
        ${SRC_DIR}/XMPCore/source/ExpatAdapter.cpp
        ${SRC_DIR}/XMPCore/source/IArrayNode_I.cpp
        ${SRC_DIR}/XMPCore/source/ICompositeNode_I.cpp
        ${SRC_DIR}/XMPCore/source/ICoreConfigurationManager_I.cpp
        ${SRC_DIR}/XMPCore/source/ICoreObjectFactory_I.cpp
        ${SRC_DIR}/XMPCore/source/IDOMImplementationRegistry_I.cpp
        ${SRC_DIR}/XMPCore/source/IDOMParser_I.cpp
        ${SRC_DIR}/XMPCore/source/IDOMSerializer_I.cpp
        ${SRC_DIR}/XMPCore/source/IMetadataConverterUtils_I.cpp
        ${SRC_DIR}/XMPCore/source/IMetadata_I.cpp
        ${SRC_DIR}/XMPCore/source/INameSpacePrefixMap_I.cpp
        ${SRC_DIR}/XMPCore/source/INodeIterator_I.cpp
        ${SRC_DIR}/XMPCore/source/INode_I.cpp
        ${SRC_DIR}/XMPCore/source/IPathSegment_I.cpp
        ${SRC_DIR}/XMPCore/source/IPath_I.cpp
        ${SRC_DIR}/XMPCore/source/ISimpleNode_I.cpp
        ${SRC_DIR}/XMPCore/source/IStructureNode_I.cpp
        ${SRC_DIR}/XMPCore/source/MetadataConverterUtilsImpl.cpp
        ${SRC_DIR}/XMPCore/source/MetadataImpl.cpp
        ${SRC_DIR}/XMPCore/source/NameSpacePrefixMapImpl.cpp
        ${SRC_DIR}/XMPCore/source/NodeImpl.cpp
        ${SRC_DIR}/XMPCore/source/ParseRDF.cpp
        ${SRC_DIR}/XMPCore/source/PathImpl.cpp
        ${SRC_DIR}/XMPCore/source/PathSegmentImpl.cpp
        ${SRC_DIR}/XMPCore/source/RDFDOMParserImpl.cpp
        ${SRC_DIR}/XMPCore/source/RDFDOMSerializerImpl.cpp
        ${SRC_DIR}/XMPCore/source/SimpleNodeImpl.cpp
        ${SRC_DIR}/XMPCore/source/StructureNodeImpl.cpp
        ${SRC_DIR}/XMPCore/source/WXMPIterator.cpp
        ${SRC_DIR}/XMPCore/source/WXMPMeta.cpp
        ${SRC_DIR}/XMPCore/source/WXMPUtils.cpp
        ${SRC_DIR}/XMPCore/source/XMPCore_Impl.cpp
        ${SRC_DIR}/XMPCore/source/XMPIterator.cpp
        ${SRC_DIR}/XMPCore/source/XMPIterator2.cpp
        ${SRC_DIR}/XMPCore/source/XMPMeta.cpp
        ${SRC_DIR}/XMPCore/source/XMPMeta2-GetSet.cpp
        ${SRC_DIR}/XMPCore/source/XMPMeta-GetSet.cpp
        ${SRC_DIR}/XMPCore/source/XMPMeta-Parse.cpp
        ${SRC_DIR}/XMPCore/source/XMPMeta-Serialize.cpp
        ${SRC_DIR}/XMPCore/source/XMPUtils.cpp
        ${SRC_DIR}/XMPCore/source/XMPUtils2.cpp
        ${SRC_DIR}/XMPCore/source/XMPUtils-FileInfo.cpp
    )

    add_library(${TARGET} STATIC ${SRCS})
    if(NOT PROJECT_IS_TOP_LEVEL)
        add_library(DNG::${TARGET} ALIAS ${TARGET})
    endif()

    target_link_libraries(${TARGET} PRIVATE EXPAT::EXPAT)
    target_include_directories(
        ${TARGET}
        PUBLIC $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/${PUBLIC_HDR_DIR}>
        PRIVATE ${PRIVATE_HDR_DIR}
    )
    target_compile_definitions(
        ${TARGET}
        PUBLIC XMP_StaticBuild $<$<PLATFORM_ID:Darwin>:MAC_ENV> $<$<PLATFORM_ID:Linux>:UNIX_ENV>
               $<$<PLATFORM_ID:Windows>:WIN_ENV>
        PRIVATE XMP_COMPONENT_INT_NAMESPACE=AdobeXMPCore_Int
    )
endif()

#
# DNG SDK
#

set(SRC_DIR dng_sdk/source)
set(HDR_DIR dng_sdk/source)

set(TARGETS dng)
if(BUILD_DNG_VALIDATE)
    set(TARGETS ${TARGETS} dng-validate)
endif()

foreach(TARGET IN ITEMS ${TARGETS})
    set(SRCS
        ${SRC_DIR}/dng_1d_function.cpp
        ${SRC_DIR}/dng_1d_table.cpp
        ${SRC_DIR}/dng_abort_sniffer.cpp
        ${SRC_DIR}/dng_area_task.cpp
        ${SRC_DIR}/dng_bad_pixels.cpp
        ${SRC_DIR}/dng_big_table.cpp
        ${SRC_DIR}/dng_bmff.cpp
        ${SRC_DIR}/dng_bottlenecks.cpp
        ${SRC_DIR}/dng_camera_profile.cpp
        ${SRC_DIR}/dng_color_space.cpp
        ${SRC_DIR}/dng_color_spec.cpp
        ${SRC_DIR}/dng_date_time.cpp
        ${SRC_DIR}/dng_exceptions.cpp
        ${SRC_DIR}/dng_exif.cpp
        ${SRC_DIR}/dng_file_stream.cpp
        ${SRC_DIR}/dng_filter_task.cpp
        ${SRC_DIR}/dng_fingerprint.cpp
        ${SRC_DIR}/dng_gain_map.cpp
        ${SRC_DIR}/dng_globals.cpp
        ${SRC_DIR}/dng_host.cpp
        ${SRC_DIR}/dng_hue_sat_map.cpp
        ${SRC_DIR}/dng_ifd.cpp
        ${SRC_DIR}/dng_image.cpp
        ${SRC_DIR}/dng_image_writer.cpp
        ${SRC_DIR}/dng_info.cpp
        ${SRC_DIR}/dng_iptc.cpp
        ${SRC_DIR}/dng_jpeg_image.cpp
        ${SRC_DIR}/dng_lens_correction.cpp
        ${SRC_DIR}/dng_linearization_info.cpp
        ${SRC_DIR}/dng_local_string.cpp
        ${SRC_DIR}/dng_lossless_jpeg.cpp
        ${SRC_DIR}/dng_matrix.cpp
        ${SRC_DIR}/dng_memory.cpp
        ${SRC_DIR}/dng_memory_stream.cpp
        ${SRC_DIR}/dng_misc_opcodes.cpp
        ${SRC_DIR}/dng_mosaic_info.cpp
        ${SRC_DIR}/dng_mutex.cpp
        ${SRC_DIR}/dng_negative.cpp
        ${SRC_DIR}/dng_opcodes.cpp
        ${SRC_DIR}/dng_opcode_list.cpp
        ${SRC_DIR}/dng_orientation.cpp
        ${SRC_DIR}/dng_parse_utils.cpp
        ${SRC_DIR}/dng_pixel_buffer.cpp
        ${SRC_DIR}/dng_point.cpp
        ${SRC_DIR}/dng_preview.cpp
        ${SRC_DIR}/dng_rational.cpp
        ${SRC_DIR}/dng_read_image.cpp
        ${SRC_DIR}/dng_rect.cpp
        ${SRC_DIR}/dng_reference.cpp
        ${SRC_DIR}/dng_ref_counted_block.cpp
        ${SRC_DIR}/dng_render.cpp
        ${SRC_DIR}/dng_resample.cpp
        ${SRC_DIR}/dng_safe_arithmetic.cpp
        ${SRC_DIR}/dng_shared.cpp
        ${SRC_DIR}/dng_simple_image.cpp
        ${SRC_DIR}/dng_spline.cpp
        ${SRC_DIR}/dng_stream.cpp
        ${SRC_DIR}/dng_string.cpp
        ${SRC_DIR}/dng_string_list.cpp
        ${SRC_DIR}/dng_tag_types.cpp
        ${SRC_DIR}/dng_temperature.cpp
        ${SRC_DIR}/dng_tile_iterator.cpp
        ${SRC_DIR}/dng_tone_curve.cpp
        ${SRC_DIR}/dng_utils.cpp
        ${SRC_DIR}/dng_xy_coord.cpp
    )

    if("${TARGET}" STREQUAL "dng-validate")
        add_executable(${TARGET} ${SRCS} ${SRC_DIR}/dng_validate.cpp)
        target_compile_definitions(${TARGET} PUBLIC qDNGValidateTarget=1)
    else()
        add_library(${TARGET} STATIC ${SRCS})
        if(NOT PROJECT_IS_TOP_LEVEL)
            add_library(DNG::${TARGET} ALIAS ${TARGET})
        endif()
    endif()

    target_link_libraries(${TARGET} PRIVATE ZLIB::ZLIB)
    target_include_directories(${TARGET} PUBLIC $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/${HDR_DIR}>)
    target_compile_definitions(
        ${TARGET} PUBLIC $<$<PLATFORM_ID:Darwin>:qMacOS=1> $<$<PLATFORM_ID:Linux>:qLinux=1>
                         $<$<PLATFORM_ID:Windows>:qWinOS=1>
    )

    if(DNG_THREAD_SAFE)
        target_sources(${TARGET} PRIVATE ${SRC_DIR}/dng_pthread.cpp)
        target_compile_definitions(${TARGET} PUBLIC qDNGThreadSafe=1)
    else()
        target_compile_definitions(${TARGET} PUBLIC qDNGThreadSafe=0)
    endif()

    if(DNG_WITH_JPEG)
        target_sources(${TARGET} PRIVATE ${SRC_DIR}/dng_jpeg_memory_source.cpp)
        target_link_libraries(${TARGET} PRIVATE JPEG::JPEG)
        target_compile_definitions(${TARGET} PUBLIC qDNGUseLibJPEG=1)
    else()
        target_compile_definitions(${TARGET} PUBLIC qDNGUseLibJPEG=0)
    endif()

    if(DNG_WITH_XMP)
        target_sources(${TARGET} PRIVATE ${SRC_DIR}/dng_xmp.cpp ${SRC_DIR}/dng_xmp_sdk.cpp)
        target_link_libraries(${TARGET} PRIVATE XMPCore)
        target_compile_definitions(${TARGET} PUBLIC qDNGUseXMP=1 qDNGXMPFiles=0 qDNGXMPDocOps=0)
    else()
        target_compile_definitions(${TARGET} PUBLIC qDNGUseXMP=0)
    endif()

    # TODO: detect JXL support
    target_compile_definitions(${TARGET} PUBLIC qDNGSupportJXL=0)

    target_compile_options(${TARGET} PRIVATE $<$<CXX_COMPILER_ID:GNU>:-Wno-stringop-overflow>)
endforeach()
