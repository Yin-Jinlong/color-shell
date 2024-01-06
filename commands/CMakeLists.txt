cmake_minimum_required(VERSION 3.22)

project("csh-commands" CXX)

add_subdirectory(cmds)

add_custom_target(build-cmds ALL
        DEPENDS
        build-clear
        build-mkdir
        build-pwd
)
