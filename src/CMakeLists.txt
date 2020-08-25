set(RESOURCE_FILES
    ${PROJECT_SOURCE_DIR}/README.md
)

set(AVCOMMON_SRC
    ${PROJECT_SOURCE_DIR}/src/common/AVGuard.h
    ${PROJECT_SOURCE_DIR}/src/common/AVGuard.cpp
    ${PROJECT_SOURCE_DIR}/src/device/base/IDevice.h
    ${PROJECT_SOURCE_DIR}/src/device/base/IDevice.cpp
    ${PROJECT_SOURCE_DIR}/src/common/AVError.h
    ${PROJECT_SOURCE_DIR}/src/format/IAVFormat.h
    ${PROJECT_SOURCE_DIR}/src/format/IAVFormat.cpp
    ${PROJECT_SOURCE_DIR}/src/common/base/IAVDataSink.h
    ${PROJECT_SOURCE_DIR}/src/common/base/IAVDataSource.h
)

set(AVDEVICE_SRC
    ${PROJECT_SOURCE_DIR}/src/device/AudioDevice.h
    ${PROJECT_SOURCE_DIR}/src/device/AudioDevice.cpp
    ${PROJECT_SOURCE_DIR}/src/device/VideoDevice.h
    ${PROJECT_SOURCE_DIR}/src/device/VideoDevice.cpp
    ${PROJECT_SOURCE_DIR}/src/codec/AudioEncoder.h
    ${PROJECT_SOURCE_DIR}/src/codec/AudioEncoder.cpp
    ${PROJECT_SOURCE_DIR}/src/codec/VideoEncoder.h
    ${PROJECT_SOURCE_DIR}/src/codec/VideoEncoder.cpp
    ${PROJECT_SOURCE_DIR}/src/codec/base/IEncoder.h
    ${PROJECT_SOURCE_DIR}/src/codec/base/IEncoder.cpp
)

set(HEADER_FILES
    ${PROJECT_SOURCE_DIR}/src/device/base/IDevice.h
    ${PROJECT_SOURCE_DIR}/src/format/IAVFormat.h
    ${PROJECT_SOURCE_DIR}/src/common/base/IAVDataSink.h
    ${PROJECT_SOURCE_DIR}/src/common/base/IAVDataSource.h
    ${PROJECT_SOURCE_DIR}/src/codec/base/IEncoder.h
    ${PROJECT_SOURCE_DIR}/src/common/AVError.h
)

set(FFMPEG_LIB
    "swscale"
    "avfilter"
    "avformat"
    "avutil"
    "avdevice"
    "avcodec"
    "swresample"
    "postproc"
)

include_directories("${FFMPEG_DIR}/include")
include_directories("${PROJECT_SOURCE_DIR}/src/common")
include_directories("${PROJECT_SOURCE_DIR}/src/device")
include_directories("${PROJECT_SOURCE_DIR}/src/codec")
include_directories("${PROJECT_SOURCE_DIR}/src/format")
include_directories("${PROJECT_SOURCE_DIR}/src/codec/base")
include_directories("${PROJECT_SOURCE_DIR}/src/device/base")

link_directories("${FFMPEG_DIR}/lib")

source_group(TREE ${CMAKE_SOURCE_DIR} FILES ${AVCOMMON_SRC} ${AVDEVICE_SRC})

if(APPLE)
    set(CMAKE_MACOSX_RPATH 1)
    set(MACOSX_RPATH ".")

    set(X264_DIR "${PROJECT_SOURCE_DIR}/deps/${BUILD_PLATFORM}/x264")
    set(FDK_AAC_DIR "${PROJECT_SOURCE_DIR}/deps/${BUILD_PLATFORM}/fdk-aac")
    link_directories("${X264_DIR}")
    link_directories("${FDK_AAC_DIR}")
    if(IOS)
        # IOS Build setting
        message("IOS Build Setting")
        add_definitions(-DSPDLOG_NO_TLS)
        set(PLATFORM_LIB
            "-framework Foundation"
            "-framework AVFoundation"
            "-framework CoreGraphics"
            "-framework CoreMedia"
            "-framework VideoToolbox"
            "-framework AudioToolbox"
            "-framework CoreImage"
            "-framework CoreAudio"
            "-framework CoreVideo"
            "-framework Security"
            "iconv"
            "bz2"
            "z"
            "x264"
            "fdk-aac"
        )
    else()
        # MACOS Build setting
        message("MAC Build Setting")
        set(CMAKE_FRAMEWORK_PATH "${SDL2_DIR}")
        set(SDL2_DIR "${PROJECT_SOURCE_DIR}/deps/${BUILD_PLATFORM}/sdl2")
        set(X265_DIR "${PROJECT_SOURCE_DIR}/deps/${BUILD_PLATFORM}/x265")
        link_directories("${SDL2_DIR}")
        link_directories("${X265_DIR}")
        find_library(
            SDL2_LIB
            SDL2
            ${SDL2_DIR}
        )
        set(PLATFORM_LIB
            "-framework Cocoa"
            "-framework Foundation"
            "-framework AVFoundation"
            "-framework CoreMedia"
            "-framework VideoToolbox"
            "-framework AudioToolbox"
            "-framework CoreImage"
            "-framework CoreAudio"
            "-framework CoreVideo"
            "-framework OpenGL"
            "-framework Security"
            "iconv"
            "bz2"
            "z"
            "lzma"
            "postproc"
            "x264"
            "fdk-aac"
            # "x265"
            # ${SDL2_LIB}
        )
    endif(IOS)
endif(APPLE)

add_library(edision SHARED ${AVDEVICE_SRC} ${AVCOMMON_SRC})

target_link_libraries(edision ${FFMPEG_LIB} logger ${PLATFORM_LIB})

if(APPLE)
set_target_properties(edision PROPERTIES
    LINK_FLAGS "-read_only_relocs suppress"
)
# set(MACOSX_RPATH ".")

set_target_properties(edision PROPERTIES
    FRAMEWORK TRUE
    FRAMEWORK_VERSION A
    MACOSX_FRAMEWORK_IDENTIFIER cn.edision.edision_model
    # MACOSX_FRAMEWORK_INFO_PLIST Info.plist
    # "current version" in semantic format in Mach-O binary file
    VERSION 1.0.0
    # "compatibility version" in semantic format in Mach-O binary file
    SOVERSION 1.0.0
    # PUBLIC_HEADER "${HEADER_FILES}"
    RESOURCE      "${RESOURCE_FILES}"
    # XCODE_ATTRIBUTE_CODE_SIGN_IDENTITY "iPhone Developer"
)

file(GLOB AVCODEC_HEADER
    ${PROJECT_SOURCE_DIR}/deps/${BUILD_PLATFORM}/ffmpeg/include/libavcodec/*.h
)

#Copy header file
set_property(SOURCE ${PROJECT_SOURCE_DIR}/src/device/base/IDevice.h PROPERTY MACOSX_PACKAGE_LOCATION Headers/)
set_property(SOURCE ${PROJECT_SOURCE_DIR}/src/format/IAVFormat.h PROPERTY MACOSX_PACKAGE_LOCATION Headers/)
set_property(SOURCE ${PROJECT_SOURCE_DIR}/src/codec/base/IEncoder.h PROPERTY MACOSX_PACKAGE_LOCATION Headers/)
set_property(SOURCE ${PROJECT_SOURCE_DIR}/src/common/base/IAVDataSink.h PROPERTY MACOSX_PACKAGE_LOCATION Headers/base/)
set_property(SOURCE ${PROJECT_SOURCE_DIR}/src/common/base/IAVDataSource.h PROPERTY MACOSX_PACKAGE_LOCATION Headers/base/)
set_property(SOURCE ${PROJECT_SOURCE_DIR}/src/common/AVError.h PROPERTY MACOSX_PACKAGE_LOCATION Headers/)
# foreach(hfile ${AVCODEC_HEADER})
#     message("!!!!!!!!!!! : ${hfile}")
#     set_property(SOURCE ${hfile} PROPERTY MACOSX_PACKAGE_LOCATION Headers/libavcodec/)
# endforeach(hfile)

endif(APPLE)

set(SAMPLE_SRC
    ${PROJECT_SOURCE_DIR}/src/sample/main.cpp
)

add_executable(edisionTest ${SAMPLE_SRC})

target_link_libraries(edisionTest edision)
