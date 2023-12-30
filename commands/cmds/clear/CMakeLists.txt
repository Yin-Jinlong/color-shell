cmake_minimum_required(VERSION 3.22)

project(clear C)

set_msvc_utf_flags()

add_executable(clear src/main.c)

add_custom_target(build-clear ALL
        DEPENDS clear
)
copy_target_files(build-clear clear ${BIN_DIR})
