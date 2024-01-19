cmake_minimum_required(VERSION 3.22)

project(csh CXX)

file(GLOB CSH_SRCS
        include/*.h
        include/**/*.h
        src/*.cpp
        src/**/*.cpp
)

set_msvc_utf_flags()

set(CSH_LINK_LIBS
        csh-lib
        yaml-cpp
)

set(CSH_LINK_LIBS_DEBUG
        csh-lib
        yaml-cppd
)

add_executable(${PROJECT_NAME} ${CSH_SRCS})

if (CMAKE_BUILD_TYPE STREQUAL "Debug")
    target_link_libraries(${PROJECT_NAME} ${CSH_LINK_LIBS_DEBUG})
else ()
    target_link_libraries(${PROJECT_NAME} ${CSH_LINK_LIBS})
endif ()

add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
        $<TARGET_FILE:csh-lib> ${CMAKE_CURRENT_BINARY_DIR}
)

add_custom_target(build-${PROJECT_NAME} ALL
        DEPENDS ${PROJECT_NAME} csh-lib
)
copy_build_target_files(${PROJECT_NAME} ${BIN_DIR})
copy(build-${PROJECT_NAME} ${PROJECT_SOURCE_DIR}/config/config.yaml ${CONFIG_DIR})
