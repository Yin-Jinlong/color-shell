#pragma once

#include "predef.h"
#include "CmdHistory.h"

namespace csh {
    /**
     * cd
     * @param path 路径
     * @return 错误码
     */
    extern DLL_OUT int cd(const str &path);

    extern DLL_OUT int history(const str &arg);

    extern DLL_OUT int updateIndexes(const str &arg);
}
