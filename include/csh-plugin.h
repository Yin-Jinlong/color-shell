#pragma once

#include <predef.h>
#include <Color.h>

#define CSH_CALL_FN_SUCCESS true
#define CSH_CALL_FN_FAILED false

typedef bool CallResult;

namespace csh {
    /**
     * 更新时机
     */
    enum class UpdateType {
        /**
         * 初始化
         */
        Init,
        /**
         * 工作目录改变
         */
        WorkDirChanged,
        /**
         * 目录更改
         */
        DirModified,
        /**
         * 每次(命令输入)
         */
        Update,
    };

    /**
     * 带颜色字符串的部分
     */
    typedef struct ColorStrPartStruct {
        /**
         * 字符串
         */
        str        str;
        /**
         * 颜色
         */
        csh::Color color;
    } ColorStrPart;
}


// 加载插件函数
typedef CallResult (*CShOnLoadPluginFn)(std::map<str, str> &config);

// 插件更新时机函数
typedef csh::UpdateType (*CShUpdateTypeFn)();

// 插件是否可显示函数
typedef bool (*CShCanShowFn)();

// 更新插件输出函数
typedef CallResult (*CShOnUpdateFn)(std::vector<csh::ColorStrPart> &parts);

