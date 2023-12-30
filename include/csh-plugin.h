#pragma once

#include <predef.h>
#include <vector>
#include <map>
#include "Color.h"

#define CSH_CALL_FN_SUCCESS true
#define CSH_CALL_FN_FAILED false

typedef bool CallResult;

namespace csh {
    // 更新时机
    enum class UpdateType {
        INIT,
        WORK_DIR_CHANGED,
        DIR_MODIFIED,
        UPDATE,
    };

    typedef struct ColorStrPartStruct {
        std::wstring str;
        csh::Color color;
    } ColorStrPart;
}


// 加载插件函数
typedef CallResult (*CShOnLoadPluginFn)(std::map<std::wstring, std::wstring> &config);

// 插件更新时机函数
typedef csh::UpdateType (*CShUpdateTypeFn)();

// 插件是否可显示函数
typedef bool (*CShCanShowFn)();

// 更新插件输出函数
typedef CallResult (*CShOnUpdateFn)(std::vector<csh::ColorStrPart> &parts);

