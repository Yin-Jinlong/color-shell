### 第三方库 ###

if (NOT DEFINED ENV{GITHUB_DIR})
    message(SEND_ERROR "No github dir")
endif ()

set(GITHUB_DIR $ENV{GITHUB_DIR})

set(TPLS
        YAML_CPP
        TINY_UNICODE
        CACHE INTERNAL "" FORCE
)

# yaml-cpp
set(YAML_CPP_HOME
        "${GITHUB_DIR}/jbeder/yaml-cpp"
        CACHE INTERNAL "" FORCE
)
set(YAML_CPP_INCLUDE_DIR
        ${YAML_CPP_HOME}/include
        CACHE INTERNAL "" FORCE
)
set(YAML_CPP_LINK_DIR
        ${YAML_CPP_HOME}/build
        CACHE INTERNAL "" FORCE
)

# tiny-unicode
set(TINY_UNICODE_HOME
        "${GITHUB_DIR}/yin-jinlong/tiny-unicode"
        CACHE INTERNAL "" FORCE
)
set(TINY_UNICODE_INCLUDE_DIR
        ${TINY_UNICODE_HOME}/include
        CACHE INTERNAL "" FORCE
)
set(TINY_UNICODE_LINK_DIR
        ${TINY_UNICODE_HOME}/build
        CACHE INTERNAL "" FORCE)

# 检查

foreach (TPL ${TPLS})
    if (NOT ${TPL}_HOME)
        message(SEND_ERROR "Please set ${TPL}_HOME")
    endif ()
    if (NOT ${TPL}_INCLUDE_DIR)
        message(SEND_ERROR "Please set ${TPL}_INCLUDE_DIR")
    endif ()
    if (NOT ${TPL}_LINK_DIR)
        message(SEND_ERROR "Please set ${TPL}_LINK_DIR")
    endif ()
endforeach ()
