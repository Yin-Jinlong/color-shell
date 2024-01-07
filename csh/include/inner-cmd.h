#pragma once

#include "predef.h"

namespace csh {
    /**
     * cd
     * @param path 路径
     * @return 错误码
     */
    extern DLL_OUT int cd(const wstr &path);
}
