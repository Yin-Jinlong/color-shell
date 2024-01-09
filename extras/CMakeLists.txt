cmake_minimum_required(VERSION 3.22)

project(extras LANGUAGES C CXX)

set(EXTRAS
        git
        node
)

add_children(${EXTRAS})

set(BUILD_EXTRAS)
add_list_suffix(EXTRAS build-csh- BUILD_EXTRAS)

add_custom_target(build-${PROJECT_NAME} ALL
        DEPENDS ${BUILD_EXTRAS}
)
