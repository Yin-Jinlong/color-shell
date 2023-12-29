cmake_minimum_required(VERSION 3.22)

project("csh-commands" CXX)

set(BIN_DIR ${PROJECT_SOURCE_DIR}/../bin)

set(CMDS
        clear
        pwd
)

add_subdirectory(cmds)

add_custom_target(build-cmds)

add_custom_command(TARGET build-cmds POST_BUILD
        COMMAND echo Copying commands...
)

copy_target_files(build-cmds ${BIN_DIR} CMDS)

