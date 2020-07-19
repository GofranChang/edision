set(AVCOMMON_SRC
  ${PROJECT_SOURCE_DIR}/src/common/AVGuard.h
  ${PROJECT_SOURCE_DIR}/src/common/AVGuard.cpp
  ${PROJECT_SOURCE_DIR}/src/common/AVConfig.h
  ${PROJECT_SOURCE_DIR}/src/common/AVConfig.cpp
  ${PROJECT_SOURCE_DIR}/src/common/AVError.h
  ${PROJECT_SOURCE_DIR}/src/common/base/AVDataSinkBase.h
  ${PROJECT_SOURCE_DIR}/src/common/base/AVDataSourceBase.h
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
  ${PROJECT_SOURCE_DIR}/src/codec/base/EncoderBase.h
  ${PROJECT_SOURCE_DIR}/src/codec/base/EncoderBase.cpp
)

include_directories("${FFMPEG_DIR}/include")
include_directories("${PROJECT_SOURCE_DIR}/src/common")
include_directories("${PROJECT_SOURCE_DIR}/src/device")
include_directories("${PROJECT_SOURCE_DIR}/src/codec")
include_directories("${PROJECT_SOURCE_DIR}/src/codec/base")

link_directories("${FFMPEG_DIR}/lib")

source_group(TREE ${CMAKE_SOURCE_DIR} FILES ${AVCOMMON_SRC} ${AVDEVICE_SRC})

#set_target_properties(audiocapture PROPERTIES RESOURCE "${PROJECT_SOURCE_DIR}/src/device/Info.plist")

add_library(edision ${AVDEVICE_SRC} ${AVCOMMON_SRC})
# set_target_properties(audiocapture PROPERTIES MACOSX_BUNDLE_INFO_PLIST ${PROJECT_SOURCE_DIR}/src/device/Info.plist)

target_link_libraries(edision avformat avutil avdevice avcodec swresample logger)

set(SAMPLE_SRC
  ${PROJECT_SOURCE_DIR}/src/sample/main.cpp
)

add_executable(edisionTest ${SAMPLE_SRC})

target_link_libraries(edisionTest edision)

