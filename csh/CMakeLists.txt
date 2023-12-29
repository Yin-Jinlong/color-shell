cmake_minimum_required(VERSION 3.22)

project("csh" CXX)

set(BIN_DIR ${PROJECT_SOURCE_DIR}/../bin)

set(CSH
        csh
)

include_directories(include)

file(GLOB CSH_SRCS
        ../include/*.h
        src/*.cpp
        src/**/*.cpp
)

set_msvc_utf_flags()

add_executable(csh ${CSH_SRCS})

target_link_libraries(csh csh-lib)

add_custom_target(build-csh)
add_custom_command(TARGET build-csh POST_BUILD
        COMMAND echo Copying csh...
)
copy_target_files(build-csh ${BIN_DIR} CSH)
