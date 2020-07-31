set(RESOURCE_FILES
  ${PROJECT_SOURCE_DIR}/README.md
)

set(AVCOMMON_SRC
  ${PROJECT_SOURCE_DIR}/src/common/AVGuard.h
  ${PROJECT_SOURCE_DIR}/src/common/AVGuard.cpp
  ${PROJECT_SOURCE_DIR}/src/device/base/DeviceBase.h
  ${PROJECT_SOURCE_DIR}/src/device/base/DeviceBase.cpp
  ${PROJECT_SOURCE_DIR}/src/common/AVError.h
  ${PROJECT_SOURCE_DIR}/src/format/AVFormatBase.h
  ${PROJECT_SOURCE_DIR}/src/format/AVFormatBase.cpp
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
include_directories("${PROJECT_SOURCE_DIR}/src/format")
include_directories("${PROJECT_SOURCE_DIR}/src/codec/base")
include_directories("${PROJECT_SOURCE_DIR}/src/device/base")

link_directories("${FFMPEG_DIR}/lib")

source_group(TREE ${CMAKE_SOURCE_DIR} FILES ${AVCOMMON_SRC} ${AVDEVICE_SRC})

#set_target_properties(audiocapture PROPERTIES RESOURCE "${PROJECT_SOURCE_DIR}/src/device/Info.plist")

add_library(edision ${AVDEVICE_SRC} ${AVCOMMON_SRC})
# set_target_properties(audiocapture PROPERTIES MACOSX_BUNDLE_INFO_PLIST ${PROJECT_SOURCE_DIR}/src/device/Info.plist)

target_link_libraries(edision avformat avutil avdevice avcodec swresample logger)

if(IOS)
message("IOS 编译设置")

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

# function(assign_source_group)
#     foreach(_source IN ITEMS ${ARGN})
#         if (IS_ABSOLUTE "${_source}")
#             file(RELATIVE_PATH _source_rel "${CMAKE_CURRENT_SOURCE_DIR}" "${_source}")
#         else()
#             set(_source_rel "${_source}")
#         endif()
#         get_filename_component(_source_path "${_source_rel}" PATH)
#         string(REPLACE "/" "\\" _source_path_msvc "${_source_path}")
#         source_group("${_source_path_msvc}" FILES "${_source}")
#     endforeach()
# endfunction(assign_source_group)

# if(WIN32)
#   if(MINGW)
#     message("MINGW 编译设置")
#   else()
#     message("WIN32 编译设置")
#   endif()
# elseif(APPLE)
#   set(CMAKE_FRAMEWORK_PATH "../../deps/${BUILD_PLATFORM}")
#   if(IOS)
#     message("IOS 编译设置")
#   else()
#     message("MAC 编译设置")
#   endif()
# elseif(ANDROID)
#   message("ANDROID 编译设置")
# endif()

