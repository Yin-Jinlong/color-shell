#pragma once

#include <predef.h>

namespace csh {
    /**
     * 命令列表
     */
    class CmdList {
    private:

        std::map<str, int> map;

        std::map<str, int> curMap;
    public:
        CmdList();

        ~CmdList();

        /**
         * 添加命令
         *
         * @param cmd 命令
         */
        void add(const str &cmd);

        /**
         * 添加当前目录命令
         *
         * @param cmd 命令
         */
        void addCurrent(const str &cmd);

        /**
         * 匹配命令
         *
         * @param cmd 命令
         * @param list 匹配结果
         */
        void match(const str &cmd, ARG_OUT std::vector<str> &list);

        /**
         * 匹配一个命令
         *
         * @param cmd 命令
         * @return 匹配结果
         */
        str matchOne(const str &cmd);

        /**
         * 移除命令
         *
         * @param cmd 命令
         */
        void remove(const str &cmd);

        /**
         * 清空当前目录命令
         */
        void clearCurrent();

        /**
         * 清空命令
         */
        void clear();

        /**
         * 是否存在命令
         *
         * @param cmd 命令
         * @return 是否存在
         */
        bool operator[](const str &cmd);

        /**
         * 添加命令
         *
         * @param cmd 命令
         */
        void operator+=(const str &cmd);
    };

}
