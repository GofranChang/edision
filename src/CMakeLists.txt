set(AVCOMMON_SRC
  ${PROJECT_SOURCE_DIR}/src/common/AVGuard.h
)

set(AVDEVICE_SRC
  ${PROJECT_SOURCE_DIR}/src/device/AudioDevice.h
  ${PROJECT_SOURCE_DIR}/src/device/AudioDevice.cpp
  ${PROJECT_SOURCE_DIR}/src/device/main.cpp
)

include_directories("${FFMPEG_DIR}/include")
include_directories("${PROJECT_SOURCE_DIR}/utils/log")
include_directories("${PROJECT_SOURCE_DIR}/src/common")
include_directories("${PROJECT_SOURCE_DIR}/src/device")

link_directories("${FFMPEG_DIR}/lib")

add_executable(audiocapture ${AVDEVICE_SRC} ${AVCOMMON_SRC})
target_link_libraries(audiocapture avformat avutil avdevice avcodec swresample logger)
