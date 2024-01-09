cmake_minimum_required(VERSION 3.22)

project(csh-node CXX)

set_msvc_utf_flags()

file(GLOB
        CSH_NODE_SRCS
        src/*.cpp
        src/**/*.cpp
)

add_library(${PROJECT_NAME} SHARED ${CSH_NODE_SRCS})
target_link_libraries(${PROJECT_NAME} csh-lib)

add_custom_target(build-${PROJECT_NAME} ALL
        DEPENDS ${PROJECT_NAME}
)
copy_target_files(${PROJECT_NAME} ${EXTRAS_DIR}/node)
