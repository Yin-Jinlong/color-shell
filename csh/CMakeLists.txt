cmake_minimum_required(VERSION 3.22)

project(csh CXX)

include_directories(include)

file(GLOB CSH_SRCS
        ../include/*.h
        src/*.cpp
        src/**/*.cpp
)

set_msvc_utf_flags()

add_executable(${PROJECT_NAME} ${CSH_SRCS})
target_link_libraries(${PROJECT_NAME} csh-lib)

add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
        $<TARGET_FILE:csh-lib> ${CMAKE_CURRENT_BINARY_DIR}
)

add_custom_target(build-${PROJECT_NAME} ALL
        DEPENDS ${PROJECT_NAME} csh-lib
)
copy_target_files(${PROJECT_NAME} ${BIN_DIR})
