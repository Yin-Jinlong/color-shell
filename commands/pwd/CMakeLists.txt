cmake_minimum_required(VERSION 3.22)

project(pwd CXX)

set_msvc_utf_flags()

add_executable(${PROJECT_NAME} src/main.cpp)

add_custom_target(build-${PROJECT_NAME} ALL
        DEPENDS ${PROJECT_NAME}
)
copy_build_target_files(${PROJECT_NAME} ${BIN_DIR})
