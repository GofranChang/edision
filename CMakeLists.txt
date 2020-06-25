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

include(CheckIncludeFiles)
include(CheckFunctionExists)
include(CheckSymbolExists)
include(CheckCXXCompilerFlag)

if(NOT FFMPEG_DIR)
  set(FFMPEG_DIR "/usr/local/ffmpeg")
endif()

add_subdirectory(utils)
add_subdirectory(device)
# include_directories(device)

# include_directories("./deps/MAC/ffmpeg/include")