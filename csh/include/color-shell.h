#pragma once

#include <predef.h>
#include <vector>

class ColorShell {
private:
    /**
     * 路径
     */
    std::vector<wstr> paths;
public:

    static constexpr const wchar_t *CD = L"cd";

    static constexpr const wchar_t *EXIT = L"exit";

    static constexpr const wchar_t *HISTORY = L"history";

    static constexpr const wchar_t *UPDATE_INDEXES = L"update-indexes";

    ColorShell();

    ~ColorShell();

    /**
     * 运行命令行
     *
     * @param line 命令行
     * @param [out] cmd  命令
     * @param [out] rc  返回码
     * @param [out] err 错误信息
     * @return 是否继续执行
     */
    bool run(wstr line, wstr &cmd, int &rc, wstr &err);

    /**
     * 更新当前目录命令索引
     */
    void updateCurrentIndexes();

    /**
     * 更新所有PATH命令索引
     */
    void updateIndexes();
};
