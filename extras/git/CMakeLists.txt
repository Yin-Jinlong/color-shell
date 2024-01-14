cmake_minimum_required(VERSION 3.22)

project(csh-git CXX)

set_msvc_utf_flags()

file(GLOB
        CSH_GIT_SRCS
        src/*.cpp
        src/**/*.cpp
)

add_library(${PROJECT_NAME} SHARED ${CSH_GIT_SRCS})
target_link_libraries(${PROJECT_NAME} csh-lib)

add_custom_target(build-${PROJECT_NAME} ALL
        DEPENDS ${PROJECT_NAME}
)
copy_build_target_files(${PROJECT_NAME} ${EXTRAS_DIR}/git)
