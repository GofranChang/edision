set(AVDEVICE_SRC
  AudioCapture.h
  AudioCapture.cpp
)

include_directories("${FFMPEG_DIR}/include")
include_directories("${PROJECT_SOURCE_DIR}/utils/log")

link_directories("${FFMPEG_DIR}/lib")

add_executable(audiocapture ${AVDEVICE_SRC})
target_link_libraries(audiocapture avformat avutil avdevice swresample logger)
