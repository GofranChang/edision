set(LOGGER_SRC
  log/MyLogger.h
  log/MyLogger.cpp
)

file(GLOB SPDLOG_SRC
  "log/spdlog/*.h"
  "log/spdlog/details/*.h"
  "log/spdlog/fmt/*.h"
  "log/spdlog/fmt/bundled/*.h"
  "log/spdlog/sinks/*.h"
)

if(IOS)
add_definitions(-DSPDLOG_NO_TLS)
endif()

include_directories("log")
source_group(TREE ${CMAKE_SOURCE_DIR} FILES ${LOGGER_SRC} ${SPDLOG_SRC})

add_library(logger ${LOGGER_SRC} ${SPDLOG_SRC})