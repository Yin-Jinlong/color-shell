cmake_minimum_required(VERSION 3.22)

project("color-shell" LANGUAGES C CXX)

set(CMAKE_C_STANDARD 23)
set(CMAKE_CXX_STANDARD 23)

set(BIN_DIR ${PROJECT_SOURCE_DIR}/bin)
set(COLOR_SHELL
        clear
        csh
        csh-lib
        pwd
)

include(${CMAKE_SOURCE_DIR}/cmake/copy-target.cmake)
include(${CMAKE_SOURCE_DIR}/cmake/msvc-options.cmake)

include_directories(
        include
)

link_directories(
        lib
)

add_subdirectory(commands)
add_subdirectory(csh)
add_subdirectory(csh-lib)

add_custom_target(build)

add_custom_command(TARGET build POST_BUILD
        COMMAND echo Copying color-shell...
)
copy_target_files(build ${BIN_DIR} COLOR_SHELL)