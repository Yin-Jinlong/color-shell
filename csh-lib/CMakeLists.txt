cmake_minimum_required(VERSION 3.22)

project("lib" CXX)

set(LIBRARY_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/../lib)
set(BIN_DIR ${PROJECT_SOURCE_DIR}/../bin)

set(
        LIB
        csh-lib
)

file(GLOB LIB_SRCS
        include/*.h
        src/*.cpp
        src/**/*.cpp
)

add_library(csh-lib SHARED ${LIB_SRCS})

add_custom_target(build-lib)

add_custom_command(TARGET build-lib POST_BUILD
        COMMAND echo Copying csh-lib...
)
copy_target_files(build-lib ${BIN_DIR} LIB)