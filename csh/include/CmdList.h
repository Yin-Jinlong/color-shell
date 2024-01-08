#pragma once

#include <predef.h>
#include <map>

namespace csh {
    /**
     * 命令列表
     */
    class CmdList {
    private:

        std::map<wstr, int> map;

        std::map<wstr, int> curMap;
    public:
        CmdList();

        ~CmdList();

        /**
         * 添加命令
         *
         * @param cmd 命令
         */
        void add(const wstr &cmd);

        /**
         * 添加当前目录命令
         *
         * @param cmd 命令
         */
        void addCurrent(const wstr &cmd);

        /**
         * 匹配命令
         *
         * @param cmd 命令
         * @param list 匹配结果
         */
        void match(const wstr &cmd, ARG_OUT std::vector<wstr> &list);

        /**
         * 匹配一个命令
         *
         * @param cmd 命令
         * @return 匹配结果
         */
        wstr matchOne(const wstr &cmd);

        /**
         * 移除命令
         *
         * @param cmd 命令
         */
        void remove(const wstr &cmd);

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
        bool operator[](const wstr &cmd);

        /**
         * 添加命令
         *
         * @param cmd 命令
         */
        void operator+=(const wstr &cmd);
    };

}
