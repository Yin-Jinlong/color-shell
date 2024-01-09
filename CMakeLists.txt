cmake_minimum_required(VERSION 3.22)

project("color-shell" LANGUAGES C CXX)

set(CMAKE_C_STANDARD 23)
set(CMAKE_CXX_STANDARD 23)

set(OUT_DIR ${PROJECT_SOURCE_DIR}/out CACHE INTERNAL "输出目录" FORCE)
set(BIN_DIR ${OUT_DIR}/bin CACHE INTERNAL "可执行文件目录" FORCE)
set(CONFIG_DIR ${OUT_DIR}/config CACHE INTERNAL "配置及插件配置目录" FORCE)
set(EXTRAS_DIR ${OUT_DIR}/extras CACHE INTERNAL "插件目录" FORCE)

enable_testing()

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
add_subdirectory(extras)

add_custom_target(build ALL
        DEPENDS build-lib build-csh build-cmds build-extras
)

add_custom_command(TARGET build POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different ${PROJECT_SOURCE_DIR}/LICENSE.md ${OUT_DIR}/LICENSE.md
)
