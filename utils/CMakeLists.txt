set(LOGGER_SRC
  log/MyLogger.h
  log/MyLogger.cpp
)

include_directories("log")
# link_directories("${FFMPEG_DIR}/lib")

add_library(logger ${LOGGER_SRC})
# target_link_libraries(logger pthread)