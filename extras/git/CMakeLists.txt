cmake_minimum_required(VERSION 3.22)

project(csh-git CXX)

set_msvc_utf_flags()

file(GLOB
        CSH_GIT_SRCS
        src/*.cpp
        src/**/*.cpp
)

add_library(csh-git SHARED ${CSH_GIT_SRCS})
target_link_libraries(csh-git csh-lib)

add_custom_target(build-csh-git ALL
        DEPENDS csh-git
)
copy_target_files(build-csh-git csh-git ${EXTRAS_DIR}/git)
