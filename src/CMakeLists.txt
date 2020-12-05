file(GLOB HEADER_FILES
    ${PROJECT_SOURCE_DIR}/include/common/*.h
    ${PROJECT_SOURCE_DIR}/include/devices/*.h
    ${PROJECT_SOURCE_DIR}/include/codec/*.h
)

set(EDISION_SRC
    ${PROJECT_SOURCE_DIR}/src/common/IAVFormat.cc
    ${PROJECT_SOURCE_DIR}/src/common/IAVDataSource.cc
    ${PROJECT_SOURCE_DIR}/src/devices/IDevices.cc
    ${PROJECT_SOURCE_DIR}/src/devices/ffmpeg/FfmpegVideoRecorder.h
    ${PROJECT_SOURCE_DIR}/src/devices/ffmpeg/FfmpegVideoRecorder.cc
    ${PROJECT_SOURCE_DIR}/src/codec/ICodec.cc
    ${PROJECT_SOURCE_DIR}/src/codec/ffmpeg/FfmpegVideoEncoder.h
    ${PROJECT_SOURCE_DIR}/src/codec/ffmpeg/FfmpegVideoEncoder.cc
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
include_directories("${PROJECT_SOURCE_DIR}/include")
include_directories("${PROJECT_SOURCE_DIR}/src/devices")

link_directories("${FFMPEG_DIR}/lib")

set(X264_DIR "${PROJECT_SOURCE_DIR}/deps/${BUILD_PLATFORM}/x264")
set(FDK_AAC_DIR "${PROJECT_SOURCE_DIR}/deps/${BUILD_PLATFORM}/fdk-aac")
link_directories("${X264_DIR}")
link_directories("${FDK_AAC_DIR}")

if(APPLE)
    set(CMAKE_MACOSX_RPATH 1)
    set(MACOSX_RPATH ".")

    if(IOS)
    else()
        message("MAC Build Setting")
        set(SDL2_DIR "${PROJECT_SOURCE_DIR}/deps/${BUILD_PLATFORM}/sdl2")
        set(X265_DIR "${PROJECT_SOURCE_DIR}/deps/${BUILD_PLATFORM}/x265")
        set(CMAKE_FRAMEWORK_PATH "${SDL2_DIR}")
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
            "x265"
            ${SDL2_LIB}
        )
    endif(IOS)
endif(APPLE)

add_library(edision SHARED ${HEADER_FILES} ${EDISION_SRC})
target_link_libraries(edision ${FFMPEG_LIB} logger ${PLATFORM_LIB})

set(EDISION_DEMO
    ${PROJECT_SOURCE_DIR}/test/main.cc
)

add_executable(edisionTest ${EDISION_DEMO})
target_link_libraries(edisionTest edision)
