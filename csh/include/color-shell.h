#pragma once

#include <predef.h>

typedef int (*CShCmdFn)(const str &argLine);

class ColorShell {
private:
    /**
     * 路径
     */
    std::vector<str> paths;
public:

    static constexpr std::array<const char *, 4> EXTS = {".exe", ".cmd", ".bat", ".ps1"};

    static constexpr const char *CD = "cd";

    static constexpr const char *EXIT = "exit";

    static constexpr const char *HISTORY = "history";

    static constexpr const char *UPDATE_INDEXES = "update-indexes";

    static const std::map<str, CShCmdFn> INNER_CMDS;

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
    bool run(const str& line, str &cmd, int &rc, str &err);

    /**
     * 更新当前目录命令索引
     */
    void updateCurrentIndexes();

    /**
     * 更新所有PATH命令索引
     */
    void updateIndexes();
};
