cmake_minimum_required(VERSION 3.22)

project(csh CXX)

include_directories(include)

file(GLOB CSH_SRCS
        ../include/*.h
        src/*.cpp
        src/**/*.cpp
)

set_msvc_utf_flags()

add_executable(csh ${CSH_SRCS})
target_link_libraries(csh csh-lib)

add_custom_command(TARGET csh POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
        $<TARGET_FILE:csh-lib> ${CMAKE_CURRENT_BINARY_DIR}
)

add_custom_target(build-csh ALL
        DEPENDS csh csh-lib
)
copy_target_files(build-csh csh ${BIN_DIR})
