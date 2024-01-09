#pragma once

#include "predef.h"
#include "CmdHistory.h"

namespace csh {
    /**
     * cd
     * @param path 路径
     * @return 错误码
     */
    extern DLL_OUT int cd(const wstr &path);

    extern DLL_OUT int history(const wstr &arg);

    extern DLL_OUT int updateIndexes(const wstr &arg);
}
