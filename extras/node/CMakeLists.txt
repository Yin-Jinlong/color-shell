cmake_minimum_required(VERSION 3.22)

project(csh-node CXX)

set_msvc_utf_flags()

file(GLOB
        CSH_NODE_SRCS
        src/*.cpp
        src/**/*.cpp
)

add_library(csh-node SHARED ${CSH_NODE_SRCS})
target_link_libraries(csh-node csh-lib)

add_custom_target(build-csh-node ALL
        DEPENDS csh-node
)
copy_target_files(build-csh-node csh-node ${EXTRAS_DIR}/node)
