cmake_minimum_required(VERSION 3.22)

project(mkdir CXX)

set_msvc_utf_flags()

add_executable(mkdir src/main.cpp)

add_custom_target(build-mkdir ALL
        DEPENDS mkdir
)
copy_target_files(build-mkdir mkdir ${BIN_DIR})
