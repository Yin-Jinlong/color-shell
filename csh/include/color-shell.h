#pragma once

#include <predef.h>
#include <vector>

class ColorShell {
private:
    /**
     * 路径
     */
    std::vector<std::wstring> paths;
public:
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
    bool run(std::wstring line, std::wstring &cmd, int &rc,std::wstring &err);
};
