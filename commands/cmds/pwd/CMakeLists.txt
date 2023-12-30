cmake_minimum_required(VERSION 3.22)

project(pwd C)

set_msvc_utf_flags()

add_executable(pwd src/main.c)

add_custom_target(build-pwd ALL
        DEPENDS pwd
)
copy_target_files(build-pwd pwd ${BIN_DIR})
