cmake_minimum_required(VERSION 3.22)

project(lib CXX)

set_msvc_utf_flags()

file(GLOB LIB_SRCS
        include/*.h
        src/*.cpp
        src/**/*.cpp
)

add_library(csh-lib SHARED ${LIB_SRCS})

add_custom_target(build-lib ALL
        DEPENDS csh-lib
)
copy_target_files(build-lib csh-lib ${BIN_DIR})

add_subdirectory(test)
