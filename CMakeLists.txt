# vim: syntax=cmake
if(NOT CMAKE_BUILD_TYPE)
    # default to Release build for GCC builds
    set(CMAKE_BUILD_TYPE Release CACHE STRING
        "Choose the type of build, options are: None(CMAKE_CXX_FLAGS or CMAKE_C_FLAGS used) Debug Release RelWithDebInfo MinSizeRel."
        FORCE)
endif()

project(edision)

set(CMAKE_CXX_STANDARD 11)

cmake_minimum_required(VERSION 2.8)

include_directories("${PROJECT_SOURCE_DIR}/utils/log")

include(CheckIncludeFiles)
include(CheckFunctionExists)
include(CheckSymbolExists)
include(CheckCXXCompilerFlag)

set(FFMPEG_DIR "${PROJECT_SOURCE_DIR}/deps/${BUILD_PLATFORM}/ffmpeg")
set(X264_DIR "${PROJECT_SOURCE_DIR}/deps/${BUILD_PLATFORM}/x264")
set(X265_DIR "${PROJECT_SOURCE_DIR}/deps/${BUILD_PLATFORM}/x265")
set(SDL2_DIR "${PROJECT_SOURCE_DIR}/deps/${BUILD_PLATFORM}/sdl2")
set(FDK_AAC_DIR "${PROJECT_SOURCE_DIR}/deps/${BUILD_PLATFORM}/fdk-aac")

add_subdirectory(utils)
add_subdirectory(src)
