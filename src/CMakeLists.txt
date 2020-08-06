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

include_directories("${FFMPEG_DIR}/include")
include_directories("${PROJECT_SOURCE_DIR}/src/common")
include_directories("${PROJECT_SOURCE_DIR}/src/device")
include_directories("${PROJECT_SOURCE_DIR}/src/codec")
include_directories("${PROJECT_SOURCE_DIR}/src/format")
include_directories("${PROJECT_SOURCE_DIR}/src/codec/base")
include_directories("${PROJECT_SOURCE_DIR}/src/device/base")

link_directories("${FFMPEG_DIR}/lib")

source_group(TREE ${CMAKE_SOURCE_DIR} FILES ${AVCOMMON_SRC} ${AVDEVICE_SRC})

#set_target_properties(audiocapture PROPERTIES RESOURCE "${PROJECT_SOURCE_DIR}/src/device/Info.plist")

add_library(edision SHARED ${AVDEVICE_SRC} ${AVCOMMON_SRC})
# set_target_properties(audiocapture PROPERTIES MACOSX_BUNDLE_INFO_PLIST ${PROJECT_SOURCE_DIR}/src/device/Info.plist)

target_link_libraries(edision postproc swscale avfilter avformat avutil avdevice avcodec swresample x264 fdk-aac logger)

if(IOS)
message("IOS 编译设置")

add_definitions(-DSPDLOG_NO_TLS)

# set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fembed-bitcode")
# set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -fembed-bitcode")

target_link_libraries(edision
  # PRIVATE luaframework
  PRIVATE "-framework Foundation"
  PRIVATE "-framework AVFoundation"
  PRIVATE "-framework CoreGraphics"
  PRIVATE "-framework CoreMedia"
  PRIVATE "-framework VideoToolbox"
  PRIVATE "-framework AudioToolbox"
  PRIVATE "-framework CoreImage"
  PRIVATE "-framework CoreAudio"
  PRIVATE "-framework CoreVideo"
  PRIVATE "iconv"
  PRIVATE "bz2"
  PRIVATE "z"
)

set_target_properties(edision PROPERTIES
FRAMEWORK TRUE
FRAMEWORK_VERSION A
MACOSX_FRAMEWORK_IDENTIFIER cn.tvm.tvm_model
# MACOSX_FRAMEWORK_INFO_PLIST Info.plist
# "current version" in semantic format in Mach-O binary file
VERSION 1.0.0
# "compatibility version" in semantic format in Mach-O binary file
SOVERSION 1.0.0
PUBLIC_HEADER "${HEADER_FILES1}"
RESOURCE      "${RESOURCE_FILES}"
# XCODE_ATTRIBUTE_CODE_SIGN_IDENTITY "iPhone Developer"
)
endif()

set(SAMPLE_SRC
  ${PROJECT_SOURCE_DIR}/src/sample/main.cpp
)

add_executable(edisionTest ${SAMPLE_SRC})

target_link_libraries(edisionTest edision)
