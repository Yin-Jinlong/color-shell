cmake_minimum_required(VERSION 3.22)

project(commands CXX)

set(CMDS
        clear
        mkdir
        pwd
        rm
)

add_children(${CMDS})

set(BUILD_CMDS)
add_list_suffix(CMDS build- BUILD_CMDS)


add_custom_target(build-${PROJECT_NAME} ALL
        DEPENDS ${BUILD_CMDS}
)
