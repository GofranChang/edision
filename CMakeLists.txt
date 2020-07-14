# vim: syntax=cmake
if(NOT CMAKE_BUILD_TYPE)
  # default to Release build for GCC builds
  set(CMAKE_BUILD_TYPE Release CACHE STRING
    "Choose the type of build, options are: None(CMAKE_CXX_FLAGS or CMAKE_C_FLAGS used) Debug Release RelWithDebInfo MinSizeRel."
    FORCE)
endif()

project(ffmpegDemo)

set(CMAKE_CXX_STANDARD 11)

cmake_minimum_required(VERSION 2.8)

include_directories("${PROJECT_SOURCE_DIR}/utils/log")

include(CheckIncludeFiles)
include(CheckFunctionExists)
include(CheckSymbolExists)
include(CheckCXXCompilerFlag)

if(NOT FFMPEG_DIR)
  set(FFMPEG_DIR "/usr/local/ffmpeg")
endif()

add_subdirectory(utils)
add_subdirectory(src)

#set_target_properties(ffmpegDemo PROPERTIES
#  MACOSX_BUNDLE TRUE
#  MACOSX_BUNDLE_INFO_PLIST ${CMAKE_CURRENT_SOURCE_DIR}/src/device/Info.plist
#)
# include_directories(device)

# include_directories("./deps/MAC/ffmpeg/include")
