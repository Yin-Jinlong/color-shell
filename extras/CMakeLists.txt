cmake_minimum_required(VERSION 3.22)

project(extras LANGUAGES C CXX)

add_subdirectory(git)

add_custom_target(build-extras ALL
        DEPENDS build-csh-git
)
