cmake_minimum_required(VERSION 3.22)

project("color-shell" LANGUAGES C CXX)

### 第三方库 ###
set(YAML_CPP_PROJECT_PATH third_party/externals/jbeder/yaml-cpp)
set(TINY_UNICODE_PROJECT_PATH third_party/externals/yin-jinlong/tiny-unicode)


set(CMAKE_C_STANDARD 23)
set(CMAKE_CXX_STANDARD 23)

set(OUT_DIR ${PROJECT_SOURCE_DIR}/out CACHE INTERNAL "输出目录" FORCE)
set(BIN_DIR ${OUT_DIR}/bin CACHE INTERNAL "可执行文件目录" FORCE)
set(CONFIG_DIR ${OUT_DIR}/config CACHE INTERNAL "配置及插件配置目录" FORCE)
set(EXTRAS_DIR ${OUT_DIR}/extras CACHE INTERNAL "插件目录" FORCE)

set(SUBS
        csh-lib
        csh
        commands
        extras
)

set(THIRD_PARTIES
        ${YAML_CPP_PROJECT_PATH}
        ${TINY_UNICODE_PROJECT_PATH}
)

enable_testing()

include(${CMAKE_SOURCE_DIR}/cmake/copy-target.cmake)
include(${CMAKE_SOURCE_DIR}/cmake/msvc-options.cmake)
include(${CMAKE_SOURCE_DIR}/cmake/children-util.cmake)
include(${CMAKE_SOURCE_DIR}/cmake/list-util.cmake)

include_directories(
        include
        third_party/externals/jbeder/yaml-cpp
)

add_children(${SUBS})

set(BUILD_SUBS)
add_list_suffix(SUBS build- BUILD_SUBS)

add_custom_target(build ALL
        DEPENDS ${BUILD_SUBS}
)

add_custom_command(TARGET build POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different ${PROJECT_SOURCE_DIR}/LICENSE.md ${OUT_DIR}/LICENSE.md
)

foreach (TP ${THIRD_PARTIES})
    add_subdirectory(${TP})
endforeach ()
