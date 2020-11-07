set(EDISION_DEMO
    ${PROJECT_SOURCE_DIR}/test/main.cc
)

add_executable(edisionTest ${EDISION_DEMO})
target_link_libraries(edisionTest edision)