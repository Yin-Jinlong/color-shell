#pragma once

#include <predef.h>
#include <vector>
#include <map>
#include "Color.h"

#define CSH_CALL_FN_SUCCESS true
#define CSH_CALL_FN_FAILED false

typedef bool CallResult;
typedef int UpdateTime;

typedef struct ColorStrPartStruct {
    std::wstring str;
    csh::Color color;
} ColorStrPart;


// 加载插件函数
typedef CallResult (*CShOnLoadPluginFn)(std::map<std::wstring, std::wstring> &config);

// 插件更新时机函数
typedef UpdateTime (*CShUpdateTimeFn)();

// 插件是否可显示函数
typedef bool (*CShCanShowFn)();

// 更新插件输出函数
typedef CallResult (*CShOnUpdateFn)(std::vector<ColorStrPart> &parts);

// 更新时机

// 每次
#define CSH_UT_EVERY_TIME (-1)
// 一次性，仅在程序启动时更新
#define CSH_UT_ONCE 0
// 目录修改时更新
#define CSH_UT_DIR_MODIFY 1<<0
// 工作目录变化时更新
#define CSH_UT_WORK_DIR_CHANGE 1<<1
