cmake_minimum_required(VERSION 3.22)

project(csh-lib CXX)

set_msvc_utf_flags()

file(GLOB LIB_SRCS
        include/*.h
        src/*.cpp
        src/**/*.cpp
)

add_library(${PROJECT_NAME} SHARED ${LIB_SRCS})
target_link_libraries(${PROJECT_NAME}
        tiny-unicode
)

add_custom_target(build-${PROJECT_NAME} ALL
        DEPENDS ${PROJECT_NAME}
)
copy_target_files(${PROJECT_NAME} ${BIN_DIR})

add_subdirectory(test)
